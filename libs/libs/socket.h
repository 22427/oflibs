#pragma once
#ifndef _WIN32
#include <sys/types.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#else
#include <WinSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

#include <iostream>

namespace ofl
{
/**
 * @brief The Socket class is a simple socket abstraction layer, providing
 * socket functionality for Windows and Unix-Systems alike.
 */
class Socket
{

#ifdef _WIN32
	SOCKET sock_handle;
	static bool wsa_setup;
	Socket(SOCKET sock_handle)
	{
		if (!wsa_setup)
		{
			int iResult;
			WSADATA wsaData;
			// Initialize Winsock
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {
				printf("WSAStartup failed: %d\n", iResult);
			}
			wsa_setup = true;
		}
		this->sock_handle = sock_handle;
		this->a_info = nullptr;
	}
#else
	int sock_handle;

	Socket(int sock_handle)
	{
		this->sock_handle = sock_handle;
		this->a_info = nullptr;
	}
#endif

	struct addrinfo * a_info;
	bool connected;

public:

	void plotError(std::string&& msg)
	{
		fprintf(stderr,"[ E ] Socket: %s \n",msg.c_str());
#ifndef _WIN32
		fprintf(stderr,"[ E ] Socket: %s \n\t %s",msg.c_str(),strerror(errno));
#else
		char buffer[256];
		strerror_s(buffer, 256, errno);
		std::cerr << buffer << "\n";
#endif
	}
	Socket(const std::string address, const std::string port,
		   int type = SOCK_STREAM , int family = AF_UNSPEC)
	{
#ifdef _WIN32
		if (!wsa_setup)
		{
			int iResult;
			WSADATA wsaData;
			// Initialize Winsock
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {
				printf("WSAStartup failed: %d\n", iResult);
			}
			wsa_setup = true;
		}
#endif
		this->a_info = nullptr;
		struct addrinfo hints;
		memset(&hints,0,sizeof(hints));
		hints.ai_family = family;
		hints.ai_socktype = type;
		if(address.empty())
			hints.ai_flags = AI_PASSIVE;
		int status;
		const char* addr = address.c_str();
		if(address.empty())
			addr = NULL;
		if((status = getaddrinfo(addr,
								 port.c_str(),&hints,&a_info)) != 0)
		{
			plotError("Socket: getAdressInfo failed");
		}
		else
		{
			sock_handle = socket(
						a_info->ai_family,
						a_info->ai_socktype,
						a_info->ai_protocol);
			if(sock_handle < 0)
			{
				plotError(std::string("socket() failed"));
			}

		}
	}

	~Socket()
	{
		close();
	}

	Socket(char* address, char* port,
		   int type = SOCK_STREAM, int family = AF_UNSPEC)
	{

#ifdef _WIN32
		if (!wsa_setup)
		{
			int iResult;
			WSADATA wsaData;
			// Initialize Winsock
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {
				plotError("WSAStartup failed!");
			}
			wsa_setup = true;
		}
#endif
		this->a_info = nullptr;
		struct addrinfo hints;
		memset(&hints,0,sizeof(hints));
		hints.ai_family = family;
		hints.ai_socktype = type;
		if(!address)
			hints.ai_flags = AI_PASSIVE;
		int status;
		if((status = getaddrinfo(address,port,&hints,&a_info)) != 0)
		{
			plotError("Socket: getAdressInfo failed!");
		}
		else
		{
			sock_handle = socket(
						a_info->ai_family,
						a_info->ai_socktype,
						a_info->ai_protocol);
			if(sock_handle < 0)
			{
				plotError(std::string("socket() failed"));
			}
		}
	}
	int bind(struct addrinfo* a_info = nullptr)
	{
		int status;
		struct addrinfo* a;
		if (a_info)
			a = a_info;
		else
			a = this->a_info;


		status = ::bind(sock_handle,
						a->ai_addr,
						a->ai_addrlen);
		if(status<0)
		{
			plotError(std::string("bind() failed"));
		}
		return status;
	}

	int connect(struct addrinfo* a_info = nullptr)
	{
		struct addrinfo* a;
		if (a_info)
			a = a_info;
		else
			a = this->a_info;

		if(!a)
		{
			plotError(" connect() impossible:\n no address"
						"information. This sockets constructor failed or it \n"
						"was created by an accept call and is already \n"
						"connected.");
			return -1;
		}

		int status = ::connect(
					sock_handle,
					a->ai_addr,
					a->ai_addrlen);

		if(status < 0)
		{
			plotError(std::string("connect() failed"));
		}
		return status;
	}

	int listen(int backlog)
	{
		int status = ::listen(sock_handle, backlog);

		if(status < 0)
		{
			plotError(std::string("listen() failed"));
		}
		return status;
	}

	Socket* accept()
	{
		struct sockaddr_storage opp_addr;
		socklen_t addr_size = sizeof(opp_addr);

		int new_sock =::accept(
					this->sock_handle,
					(struct sockaddr*) &opp_addr,
					&addr_size);
		if(new_sock < 0)
		{
			plotError(std::string("accept() failed"));
		}
		return new Socket(new_sock);
	}

	int send(const void* data, const size_t data_len, int flags = 0 )
	{
#ifndef _WIN32
	int status =  ::send(this->sock_handle,data,data_len,flags);
#else
	int status = ::send(this->sock_handle,(const char*) data, data_len, flags);
#endif
		if(status < 0)
		{
			plotError(std::string("send() failed"));
		}
		return status;
	}

	int sendAll(const void* data, int data_len, int flags = 0)
	{
		int send_data = 0 ;
		while(send_data < data_len)
		{

			int status = ::send(this->sock_handle,
								(const char*)data + send_data, data_len, flags);
			if(status < 0)
			{
				plotError(std::string("send() in sendAll() failed"));
				return -1;
			}
			send_data+= status;
		}
		return send_data;
	}
	int recv(void * buffer, const size_t len, int flags = 0)
	{
#ifndef _WIN32
		int status = ::recv(this->sock_handle,buffer,len,flags);
#else
		int status = ::recv(this->sock_handle, (char*) buffer, len, flags);
#endif
		if(status < 0)
		{
			plotError(std::string("recv() failed"));
		}
		return status;
	}

	void close()
	{
#ifndef _WIN32
		::close(this->sock_handle);
#else
		closesocket(this->sock_handle);
#endif
	}

	int getpeername(struct sockaddr_storage* addr, socklen_t* addrlen)
	{

		int status =  ::getpeername(this->sock_handle,
									(struct sockaddr*) addr,addrlen);
		if(status < 0)
		{
			plotError(std::string("getpeername() failed"));
		}
		return status;
	}

	virtual int write_data(const void* data, const size_t len)
	{
		return this->send(data, len);
	}
	virtual int read_data(void* buffer, const size_t  len)
	{
		return	this->recv(buffer, len);
	}
};
}
