#if 0
<begin_doc>
//The MIT License (MIT)
//================================================================================
//
//Copyright (c) 2016 Timon Zietlow
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of
//this software and associated documentation files (the "Software"), to deal in
//the Software without restriction, including without limitation the rights to
//use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//the Software, and to permit persons to whom the Software is furnished to do so,
//subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

--------------------------------------------------------------------------------

//What is this?
//================================================================================
//The oflibs are a set of usefull classes and tools dealing with all kinds of 
//problems.  
//My personal focus is in 3D-visualisation and VR, so most of these deal with 
//these topics. OFLIBS stands for "**o**ne **f**ile **lib**rarie**s**". The basic 
//idea is to write libraries in a single file, which you just have to include into
//your project, and you are ready to go.
//I find these usefull for fast prototypes and "experimental" code, which is what 
//this collection currently is, so don't expect a well tested, woking toolset, but
//maybe you find the one or two lines of code you were searching ;).
//I'm constantly working whith these tools and I am shure they will get better 
//over time.
//
//Structure
//--------------------------------------------------------------------------------
//In oflibs/ you find the finished one file libraries. They are ready to use. The 
//documentation to each library is also inside of the lib. Here is a short list of
//the different modules and libraries:
//
//- ofl_all: All oflibs in one 
//- ofl_ogl_geo - A class reperesenting vertex-data on the GPU
//- ofl_ogl_state - A state-wrapper imulating the "classic" OpenGL-fixed-function 
//  states, including matrix stacks and lighting.
//- ofl_ogl_win - An interface to create an OpenGL-Context with window and reading 
//  events. Currently implemented using glfw.
//- ofl_open_gl_tools: combines the above.
//- ofl_socket: A class wrapping sockets.
//- ofl_stru: Some string utilities. Used by many other oflibs.
//- ofl_track - Tracking: An interface to the ART-DTrack2 tracking system.
//- olf_vd - Vertex Data: A data structure representing geometry on the client 
//  side. This is OpenGL independend. 
//- ofl_vdman - Vertex Data MANufacturer: Allows you to create VertexData using
//  the old begin(..) ... end() paradigma from fixed-function-GL, but is OpenGL
//  independend.
//- ofl_vmath - Vector MATH : Some simple vector operations, but no alternative to 
//  GLM.
//- ofl_vrpv - Virtual Reality Projection and View - A module to genereate view-
//  and projection-matrices for VR systems like PowerWalls or CAVEs
//
//How do oflibs work?
//--------------------------------------------------------------------------------
//You use the oflib as a regular header, for example:
//```c_cpp
	#include <ofl_all.h>
//```
//Then in one .cpp-file of your project you include the oflib defining
//`OFL_IMPLEMENTATION` before:
//```c_cpp
//	#define OFL_IMPLEMENTATION
	#include <ofl_all.h>
//	#undef OFL_IMPLEMENTATION
//```
//That's it :)
//
//Internally the oflib-header is organized in two parts, the declaration-part 
//(that's what you would write into an ordinary header) and the implementation-part 
//(that's what you would write into *.cpp files)
//Example:
//```c_cpp
//	#ifndef OFL_ALL_H
//	#define OFL_ALL_H
//		// ALL DECLARATIONS
//	#endif
//	#ifdef OFL_IMPLEMENTATION
//		// ALL THE IMPLEMENTATIONS
//	#endif
//```
//
//Where can I get more information?
//--------------------------------------------------------------------------------
//Inside the oflib files there is the documentation for each module used. 
//In the subdirectory tools/ there are also some more or less simple tools using 
//the oflibs.
//
//
//

--------------------------------------------------------------------------------

<end_doc>
#endif 
#ifndef USING_OFL_SOCKET_H
 #define USING_OFL_SOCKET_H
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
	struct addrinfo * a_info;
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

	virtual ~Socket()
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
									reinterpret_cast<struct sockaddr*>( addr),
									addrlen);
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

#endif //USING_OFL_SOCKET_H
#ifdef OFL_IMPLEMENTATION
#endif
