#if 0
<begin_doc>
#ifndef USING_OFL_LICENSE_MD
#define USING_OFL_LICENSE_MD
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

#endif //USING_OFL_LICENSE_MD

--------------------------------------------------------------------------------

#ifndef USING_OFL_README_MD
#define USING_OFL_README_MD
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
//- ogl_stereo_compositor - A simple way to render in stereo image formats.
//- ofl_ogl_win - An interface to create an OpenGL-Context with window and reading 
//  events. Currently implemented using glfw.
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

#endif //USING_OFL_README_MD

--------------------------------------------------------------------------------

#ifndef USING_OFL_TRACK_MD
#define USING_OFL_TRACK_MD
//TRACK : Tracking
//================================================================================
//This is basically an API to the ART network data stream. The data send by
//the ARTDTrack2 is received, and provided in one easy to use class.
//
//API
//--------------------------------------------------------------------------------
//Just create an TrackingData object. From here on you can ask the objekt of
//the current state of all tracked targets.
//After calling the method `start()` the object spawns a thread to handle the
//communication with the DTrack2 server. All public methods are save to call
//any time. You can use the `setTransformation(...)` method to set a
//transformation that  will be  applied to all positions and orientations.
//
//There are different methods to access different types of targets:
//
//* `get3ds()`returns all 3DoF-targets found in the last frame.
//* `get6d(id)`returns the selected 6DoF-Target*.
//* `get6di(id)`returns the selected Dtrack2-Internal-6DoF-Target*.
//* `getFlystick(id)` retunrs the selected Flystick*.
//
//*If there is no information a Target object will be returned,
//located in (0,0,0). If there are no new informations about a Target the last
//knonw information is returned.
//

#endif //USING_OFL_TRACK_MD

--------------------------------------------------------------------------------

#ifndef USING_OFL_VMATH_MD
#define USING_OFL_VMATH_MD
//VMATH : vector-maths
//================================================================================
//This is a set of classes containing the needed vector-math for the ofl tools.
//It is ment to be an backup data exchange format, if there is no GLM in your
//project.
//Note: Do not use this vector and matrix class. Use GLM or something else!
//GLM is also header only, and does - imho - a great job. These classes are
//only here, so you do not have to use GLM. There are two classes with a very
//limited set of methods: vec4 and mat4 a vector of 4 floats and a 4x4-matrix.
//
//Usage
//--------------------------------------------------------------------------------
//As mentioned above you should not use these classes for anything but
//exchanging data with oflibs. To enshure, that GLM is used (if you use it in
//your project) include `<glm/glm.hpp>` before you include this file.
//

#endif //USING_OFL_VMATH_MD

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
#ifndef USING_OFL_STRU_H
#define USING_OFL_STRU_H

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <iostream>
#include <ostream>
#include <istream>
#include <cstring>
#include <cmath>


namespace ofl
{

/**
 * @brief ltrim removes all whitespaces on the left side of s
 * @param s
 */
static inline void ltrim(std::string &s)
{
	s.erase(s.begin(),std::find_if(s.begin(), s.end(),
								   std::not1(std::ptr_fun<int, int>(std::isspace))));
}

/**
 * @brief ltrim removes all whitespaces on the right side of s
 * @param s
 */
static inline void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
						 std::not1(std::ptr_fun<int,int>(std::isspace))).base(),
			s.end());
}

/**
 * @brief ltrim removes all whitespaces on the both sides of s
 * @param s
 */
static inline void trim(std::string &s){ltrim(s);rtrim(s);}



/**
 * @brief The Tokenizer class is a tool to parse strings. It works linke strtok,
 * but it has a local state.
 * It works by a simple principle. There is a head pointer. Everytime you ask
 * for a token. the current head will be returned, and the first appearing
 * seperator character is searched and set to zero.
 * NOTE: Only _ONE_ character will be set to zero. Even if there are three
 * seperators in a row, only the first one will be set to zero.
 */
class Tokenizer
{
protected:
	char* m_base;
	char* m_rest;
public:
	static std::string whitespaces;
	Tokenizer(const std::string& base);
	~Tokenizer();
	/**
	 * @brief reset Will free the current base and set a new one.
	 * @param base The new base.
	 */
	void reset(const std::string& base);

	/**
	 * @brief getToken Will return the string until and without the seperator
	 * character:
	 * For example:
	 * "foo,bar..." ---getToken(',')--> "foo"
	 *
	 * @param separator The seperator character
	 * @return string untill the first appearence of seperator, or nullptr.
	 */
	char* getToken(char separator);

	/**
	 * @brief Will return the string till and without one! of the seperators!
	 * "foo;,.bar..." ---getToken(",.;",c)--> "foo",c=;
	 * @param separators String contianing all possible seperatos.
	 * @param sep Will contain the seperator actually found.
	 * @return string till the first appearence of a seperator, or nullptr.
	 */
	char* getToken(const std::string& separators = whitespaces,
				   char* sep = nullptr);


	/**
	 * @brief getTokenAs Will read a string untill one of the seperators appear
	 * and return it interpreted.
	 * @param separators String contianing all possible seperatos.
	 * @param sep Will contain the seperator actually found.
	 * @return
	 */
	template<typename T>
	bool getTokenAs(T& res,const std::string &separators = whitespaces,
					char *sep = nullptr)
	{
		throw "TYPE NOT SUPPORTED!!!";
		return false;
	}

	/**
	 * @brief skipOverAll Skipps all consecutive appearences of char in seps.
	 * Example:
	 * ".,;..:,;,,.foo...." ---skipOverAll(";.,:")--> "foo...."
	 * @param separators String contianing all possible seperatos.
	 */
	void skipOverAll(const std::string& seps);

	/**
	 * @brief skipOverAll Skipps all consecutive appearences of whitespaces.
	 * Example:
	 * "           foo...." ---skipWhiteSpaces()--> "foo...."
	 */
	void skipWhiteSpaces();

	/**
	 * @brief getRest The remaining string,
	 * @return
	 */
	char* getRest(){ return m_rest; }
};


template<> inline bool Tokenizer::getTokenAs<int>(
		int& res,
		const std::string &seps,
		char *sep )
{
	char* c = getToken(seps,sep);
	if(c)
		res = atoi(c);
	return c;
}

template<> inline bool Tokenizer::getTokenAs<float>(
		float& res,
		const std::string &seps ,
		char *sep )
{
	char* c = getToken(seps,sep);
	if(c)
		res = atof(c);
	return c;
}

template<> inline bool Tokenizer::getTokenAs<double>(
		double& res,
		const std::string &seps ,
		char *sep )
{
	char* c = getToken(seps,sep);
	if(c)
		res = atof(c);
	return c;
}

template<> inline bool Tokenizer::getTokenAs<bool>(
		bool& res,
		const std::string &seps ,
		char *sep )
{
	char* c=  getToken(seps,sep);
	if(c)
		res = strcmp(c,"false");
	return c;
}


}
namespace paths
{


static inline bool is_directory(const std::string& p)
{
	return p.at(p.length()-1) == '/';
}
static inline bool is_relative(const std::string& p)
{
	if(p.length()<2)
		return false;
	return p.at(0) != '/' && p.at(1) != ':';
}

static inline std::string file(const std::string& p)
{
	if(is_directory(p))
		return "";

	return p.substr(p.find_last_of('/')+1);
}

static inline std::string filename(const std::string& p)
{
	if(is_directory(p))
		return "";
	auto locd = p.find_last_of('.');
	auto locs =  p.find_last_of('/');
	if(locs == p.npos)
		locs = 0;
	else
		locs++;
	if(locd < locs)
		locd = p.npos;

	return p.substr(locs,locd-locs);
}

static inline std::string extension(const std::string& p)
{
	if(is_directory(p))
		return "";
	auto loc = p.find_last_of('.');
	auto sloc = loc > p.find_last_of('/');
	if(loc > sloc || sloc == p.npos)
		return p.substr(loc+1);
	return "";
}

static inline std::string without_extension(const std::string& p)
{
	if(is_directory(p))
		return p;
	auto loc = p.find_last_of('.');
	return p.substr(0,loc);
}
}

#endif //USING_OFL_STRU_H
#ifndef USING_OFL_TRACK_H
#define USING_OFL_TRACK_H
#ifndef USING_OFL_VMATH_H
#define USING_OFL_VMATH_H

#include <string>
#include <cmath>


#ifdef GLM_INCLUDED
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#endif


namespace ofl
{
#ifdef GLM_INCLUDED
typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef glm::vec2 vec2;
typedef glm::mat4 mat4;
using namespace glm;
#else


/**
 * @brief The vec2 class a vector of 3 floats.
 */
class vec2
{
public:

	union
	{
		struct{ float x, y ;};
		struct{ float r, g ;};
		struct{ float s, t ;};
		float data[2];
	};

	vec2(	const float x = 0,
			const float y = 0);

	float& operator[](const unsigned int i);
	float operator[](const unsigned int i) const;

	vec2 operator + (const vec2& v)const;
	vec2& operator +=(const vec2& v);
	vec2 operator - (const vec2& v)const;
	vec2 operator * (const vec2& v)const;
	vec2& operator *=(const vec2& v);
	vec2 operator / (const vec2& v)const;
	vec2 operator *(const float f) const;
	vec2 operator /(const float f)const;

};


/**
 * @brief The vec4 class a vector of 3 floats.
 */
class vec3
{
public:

	union
	{
		struct{ float x, y, z ;};
		struct{ float r, g, b ;};
		struct{ float s, t, p ;};
		float data[3];
	};

	vec3(	const float x = 0,
			const float y = 0,
			const float z = 0);

	vec3(const vec2& vec, const float z = 0.0f)
	{
		memcpy(data,vec.data,2*sizeof(float));
		this->z = z;
	}

	float& operator[](const unsigned int i);
	float operator[](const unsigned int i) const;

	vec3 operator + (const vec3& v)const;
	vec3& operator +=(const vec3& v);
	vec3 operator - (const vec3& v)const;
	vec3 operator * (const vec3& v)const;
	vec3 operator / (const vec3& v)const;
	vec3 operator *(const float f) const;
	vec3 operator /(const float f)const;

};

/**
 * @brief The vec4 class a vector of 4 floats.
 */
class vec4
{
public:

	union
	{
		struct{ float x, y, z, w; };
		struct{ float r, g, b, a; };
		struct{ float s, t, p, q; };
		float data[4];
	};

	vec4(	const float x = 0,
			const float y = 0,
			const float z = 0,
			const float w = 1);

	vec4(const vec3& vec, const float w = 1.0f)
	{
		memcpy(data,vec.data,3*sizeof(float));
		this->w = w;
	}

	vec4(const vec2& vec, const float z = 0.0f, const float w = 1.0f)
	{
		memcpy(data,vec.data,2*sizeof(float));
		this->z = z;
		this->w = w;
	}

	vec4(const vec2& v1,const vec2& v2)
	{
		x = v1.x;
		y = v1.y;
		z = v2.x;
		w = v2.y;
	}

	float& operator[](const unsigned int i);
	float operator[](const unsigned int i) const;

	vec4 operator + (const vec4& v)const;
	vec4 operator - (const vec4& v)const;
	vec4 operator * (const vec4& v)const;
	vec4 operator / (const vec4& v)const;
	vec4 operator *(const float f) const;
	vec4 operator /(const float f)const;
	operator vec3() {return vec3(x,y,z);}
};


/**
 * @brief The mat4 class is the backup class if there is no glm in your project
 */
class mat4
{
	vec4 m_data[4];
public:
	mat4(float diag = 1.0f);
	mat4(const vec4& c0 ,const vec4& c1 ,const vec4& c2 ,const vec4& c3);
	vec4& operator[](int i);
	const vec4& operator[](int i) const;
	float* data(){return &(m_data[0].data[0]);}
};

/**
 * @brief operator * Matrix-Vector multiplication.
 * @param M Matrix M
 * @param v Matrix v
 * @return M*v
 */
vec4 operator*(	const mat4 & M,	const vec4 & v	);

/**
 * @brief operator * Matrix-Matrix multiplication.
 * @param A Matrix A
 * @param B Matrix B
 * @return A*B
 */
mat4 operator* ( const mat4 & A, const mat4 & B);

vec4 normalize(const vec4& v);

vec3 normalize(const vec3& v);

/**
 * @brief frustum calculates a projection matrix based on a frustum.
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param nearVal
 * @param farVal
 * @return The projection matrix.
 */
mat4 frustum(
		const float & left,
		const float & right,
		const float & bottom,
		const float & top,
		const float & nearVal,
		const float & farVal);

/**
 * @brief inverse calculates the inverse of a mat4.
 * @param mat
 * @return mat^(-1)
 */
mat4 inverse(const mat4& m);

float length(const vec3& a);
float length2(const vec3& a);

float length(const vec4& a);
float length2(const vec4& a);


float distance(const vec3& a, const vec3& b);
float distance2(const vec3& a, const vec3& b);

/**
 * @brief cross cross product.
 * @param a
 * @param b
 * @return a x b
 */
vec3 cross(const vec3& a, const vec3& b);

/**
 * @brief dot scalar product.
 * @param a
 * @param b
 * @return <a,b>
 */
float dot(const vec3& a, const vec3& b);


/**
 * @brief translation Creates a translation Matrix to v
 * @param v The translation
 * @return
 */
mat4 translate(const mat4& m,const vec4& v);
#endif


/// A common interface for glm and our little matrix and vector toolset.

#define INF std::numeric_limits<float>::infinity()
#define qNaN std::numeric_limits<float>::quiet_NaN()

#define vec2NaN vec2(qNaN,qNaN)
#define vec3NaN vec3(qNaN,qNaN,qNaN)
#define vec4NaN vec4(qNaN,qNaN,qNaN,qNaN)
#define vecIsNaN(v) (!(v.x==v.x))



/**
 * @brief to_string Turns a vector into a string.
 * @param v the vector to transform
 * @return "(%f,%f,%f,%f)",v.x,v.y,v.z,v.w
 */
std::string to_string (const vec4& v);

/**
 * @brief read_from_string Parses a vec4 from a string and consumes the pared
 * data from the string.
 * @param str "(x,y,z,w)....."
 * @return vec4(x,y,z,w)
 */
vec4 read_from_string(std::string& str);



mat4 transpose(const mat4& m);


bool operator  < (const vec4& a, const vec4& b);
bool operator == (const vec4& a, const vec4& b);

bool operator  < (const vec3& a, const vec3& b);
bool operator == (const vec3& a, const vec3& b);
/* small coperator class for the map p2n in
calculateNormals.*/
class compare_vec_4
{
public:
	bool operator()(const vec4 a, const vec4 b)
	{
		return 	a < b;
	}
};


class compare_vec_3
{
public:
	bool operator()(const vec3 a, const vec3 b)
	{
		return 	a < b;
	}
};


}

#endif //USING_OFL_VMATH_H

#include <sstream>
#include <vector>
#include <mutex>
#include <map>
#include <thread>

namespace ofl {

class Target
{
protected:
	unsigned int m_id;
	unsigned int m_frame_no;
public:
	unsigned int id()const {return m_id;}
	unsigned int frame_no()const {return m_frame_no;}
	Target(unsigned int id = 0, unsigned int frame_no = 0)
		:m_id(id),m_frame_no(frame_no)
	{}
	virtual const vec4& position() const = 0;
	virtual void transform(const mat4& T) = 0;
};



class Target3DOF : public Target
{
protected:
	vec4 m_position;

public:
	Target3DOF(unsigned int id = 0, unsigned int frame_no = 0,
			   const vec4& pos =vec4(0)
			   );
	virtual void transform(const mat4& T);

	const vec4& position() const;
};


class Target6DOF : public Target3DOF
{
protected:
	mat4 m_orientation;
public:
	Target6DOF(unsigned int id = 0,
			   unsigned int frame_no = 0,
			   const vec4& pos = vec4(0),
			   const mat4& orientation = mat4(0));
	void transform(const mat4& T);

	const mat4& orientation() const;
};

class TargetFlystick : public Target6DOF
{
protected:
	std::vector<unsigned char> m_button_state;
public:
	enum ButtonState
	{
		released = 0,
		pressed,
	};
	TargetFlystick(unsigned int id = 0,
				   unsigned int frame_no =0 ,
				   std::vector<unsigned char> b = std::vector<unsigned char>(8,0),
				   const vec4& pos = vec4(0),
				   const mat4& orientation = mat4(0));

	ButtonState button(const unsigned int id) const;
};


class TrackingData
{
	std::vector<Target3DOF> m_t3d;
	std::mutex m_t3d_mtx;
	std::map<int,Target6DOF> m_t6d;
	std::mutex m_t6d_mtx;
	std::map<int,Target6DOF> m_t6di;
	std::mutex m_t6di_mtx;
	std::map<int,TargetFlystick> m_tfly;
	std::mutex m_tfly_mtx;

	mat4 m_transformation;
	unsigned int m_curr_frame_no;

	bool m_goon;
	std::mutex m_goon_mtx;

	std::thread m_thread;

	Socket m_sock;

	void lock_all();

	void unlock_all();


	void loop();

public:
	TrackingData(
			const std::string& address,
			const std::string& port);
	~TrackingData();
	void start();

	void stop();

	void setTransformation(const mat4& mat);

	std::vector<Target3DOF> get3ds();
	Target6DOF get6d(const unsigned int id);
	Target6DOF get6di(const unsigned int id);
	Target6DOF getFlystick(const unsigned int id);


};

}

#endif //USING_OFL_TRACK_H
#ifdef OFL_IMPLEMENTATION
#ifndef USING_OFL_STRU_CPP
#define USING_OFL_STRU_CPP

namespace ofl
{
Tokenizer::Tokenizer(const std::string& base)
{
	this->m_base = new char[base.length() + 1];
	memcpy(this->m_base, base.data(), base.length() + 1);
	this->m_rest = this->m_base;
}

Tokenizer::~Tokenizer()
{
	delete[] m_base;
}

char* Tokenizer::getToken(char separator)
{
	char* to_ret = m_rest;

	if (*m_rest == 0)
		return nullptr;

	while (*m_rest && *m_rest != separator)
	{
		m_rest++;
	}

	if(*m_rest)
	{
		*m_rest =0;
		m_rest++;
	}
	return to_ret;
}

bool contains(const std::string& str, const char c)
{
	for(const auto& cc : str)
	{
		if(cc == c)
		{
			return true;
		}
	}
	return false;
}
char* Tokenizer::getToken(const std::string& separators, char* sep)
{
	char* to_ret = m_rest;

	if (*m_rest == 0)
		return nullptr;

	while (*m_rest && !contains(separators,*m_rest))
	{
		m_rest++;
	}
	if(sep)
		*sep = *m_rest;
	return to_ret;
}


void Tokenizer::skipOverAll(const std::string& seps)
{
	while(*m_rest && contains(seps,*m_rest))
	{
		m_rest++;
	}
}
void Tokenizer::skipWhiteSpaces()
{
	while(*m_rest && isspace(*m_rest))
		m_rest++;
}


void Tokenizer::reset(const std::string& base)
{
	delete[] this->m_base;
	this->m_base = new char[base.length() + 1];
	memcpy(this->m_base, base.data(), base.length() + 1);
	this->m_rest = this->m_base;
}


std::string Tokenizer::whitespaces = " \t\n\v\f\r";
}

#endif //USING_OFL_STRU_CPP
#ifndef USING_OFL_TRACK_CPP
#define USING_OFL_TRACK_CPP

using namespace ofl;
Target3DOF::Target3DOF(unsigned int id, unsigned int frame_no, const vec4 &pos)
	:Target(id, frame_no)
{
	m_position = pos;
}

void Target3DOF::transform(const mat4 &T)
{
	m_position = T*m_position;
}

const vec4 &Target3DOF::position() const
{
	return m_position;
}

Target6DOF::Target6DOF(unsigned int id,
					   unsigned int frame_no,
					   const vec4 &pos,
					   const mat4 &orientation)
	: Target3DOF(id,frame_no,pos)
{
	m_orientation = mat4(orientation);
}

void Target6DOF::transform(const mat4 &T)
{
	m_orientation = T*m_orientation;
	m_position = T*m_position;
}

const mat4 &Target6DOF::orientation() const
{
	return m_orientation;
}

TargetFlystick::TargetFlystick(unsigned int id, unsigned int frame_no,
							   std::vector<unsigned char> b,
							   const vec4 &pos,
							   const mat4 &orientation)
	: Target6DOF(id,frame_no,pos,orientation), m_button_state (b)
{}

TargetFlystick::ButtonState TargetFlystick::button(const unsigned int id) const
{
	return (ButtonState)m_button_state[id];
}

void TrackingData::lock_all()
{
	m_t3d_mtx.lock();
	m_t6d_mtx.lock();
	m_t6di_mtx.lock();
	m_tfly_mtx.lock();
}

void TrackingData::unlock_all()
{
	m_t3d_mtx.unlock();
	m_t6d_mtx.unlock();
	m_t6di_mtx.unlock();
	m_tfly_mtx.unlock();
}



std::vector<float> readARTBlob(ofl::Tokenizer& tkn)
{
	std::vector<float> res;
	ofl::Tokenizer t(tkn.getToken(']'));
	t.skipOverAll("["+t.whitespaces);

	std::string seperators = ","+t.whitespaces;
	float f;
	while(t.getTokenAs(f,seperators))
	{
		res.push_back(f);
	}
	return res;
}

mat4 mat4_from_array(const std::vector<float> &vec)
{
	mat4 res(1);
	int s = sqrt(vec.size());
	s = std::min(s,4);
	for(int i =0 ; i<s;i++)
	{
		for( int j = 0 ; j <s;j++)
		{
			res[i][j] = vec[i*s+j];
		}
	}
	return res;
}

Target6DOF read6d(ofl::Tokenizer &tkn, int frame_no)
{
	std::vector<float> head = readARTBlob(tkn);
	std::vector<float> pos_eul = readARTBlob(tkn);
	mat4 mat = mat4_from_array(readARTBlob(tkn));

	return Target6DOF(head[0],
			frame_no,
			vec4(pos_eul[0],pos_eul[1],pos_eul[2]),
			mat);

}

Target3DOF  read3d(ofl::Tokenizer &tkn, int frame_no)
{
	std::vector<float> head = readARTBlob(tkn);
	std::vector<float> pos = readARTBlob(tkn);

	return Target3DOF(head[0],
			frame_no,
			vec4(pos[0],pos[1],pos[2]));
}

TargetFlystick  read6df(ofl::Tokenizer &tkn, int frame_no)
{
	std::vector<float> head = readARTBlob(tkn);
	std::vector<float> pos_eul = readARTBlob(tkn);
	mat4 mat = mat4_from_array(readARTBlob(tkn));

	std::vector<unsigned char> buttons;
	uint8_t b = head[2];
	for(unsigned int i =0 ; i<8;i++ )
	{
		buttons.push_back(b&(1<<i));
	}
	return TargetFlystick(head[0],frame_no,buttons,
			vec4(pos_eul[0],pos_eul[1],pos_eul[2]),
			mat);
}


void TrackingData::loop()
{
	m_goon_mtx.lock();

	int buffer_size = 8*1024;
	char* buffer = (char*)malloc(buffer_size);

	while(m_goon)
	{
		m_goon_mtx.unlock();
		int read = m_sock.read_data(buffer,buffer_size);
		if(read < buffer_size)
			buffer[read] = 0;

		std::stringstream strm(buffer);
		std::string l;

		while(std::getline(strm,l))
		{
			ofl::Tokenizer line(l);
			std::string lt = line.getToken(' ');
			int cnt = 0;
			line.getTokenAs(cnt," ");

			if(lt == "fr")
			{
				m_curr_frame_no = cnt;
			}
			else if(lt == "6d")
			{
				m_t6d_mtx.lock();
				for (int i = 0 ; i< cnt; i++)
				{
					Target6DOF target = read6d(line,m_curr_frame_no);
					target.transform(m_transformation);
					m_t6d[target.id()] = target;
				}
				m_t6d_mtx.unlock();
			}
			else if(lt == "6di")
			{
				m_t6di_mtx.lock();
				for (int i = 0 ; i< cnt; i++)
				{
					Target6DOF target = read6d(line,m_curr_frame_no);
					target.transform(m_transformation);
					m_t6di[target.id()] = target;
				}
				m_t6di_mtx.unlock();
			}
			else if(lt == "3d")
			{
				m_t6d_mtx.lock();
				m_t3d.clear();
				for (int i = 0 ; i< cnt; i++)
				{
					Target3DOF target = read3d(line,m_curr_frame_no);
					target.transform(m_transformation);
					m_t3d.push_back(target);
				}
				m_t6d_mtx.unlock();

			}
			else if(lt == "6df")
			{
				m_tfly_mtx.lock();
				for (int i = 0 ; i< cnt; i++)
				{
					TargetFlystick target = read6df(line,m_curr_frame_no);
					target.transform(m_transformation);
					m_tfly[target.id()] = target;
				}
				m_tfly_mtx.unlock();
			}
		}

		m_goon_mtx.lock();
	}
	m_goon_mtx.unlock();
}

TrackingData::TrackingData(const std::string &address, const std::string &port)
	:m_sock(address,port,SOCK_DGRAM)
{

	m_goon = false;
	m_curr_frame_no = 0;
	m_sock.bind();
}

TrackingData::~TrackingData()
{
	stop();
	m_sock.close();
}

void TrackingData::start()
{
	m_goon_mtx.lock();
	m_goon = true;
	m_goon_mtx.unlock();
	if(!m_thread.joinable())
		m_thread = std::thread(&TrackingData::loop,this);
}

void TrackingData::stop()
{
	m_goon_mtx.lock();
	m_goon = false;
	m_goon_mtx.unlock();
	m_thread.join();
}

void TrackingData::setTransformation(const mat4 &mat)
{
	lock_all();
	m_transformation = mat;
	unlock_all();
}

std::vector<Target3DOF> TrackingData::get3ds()
{
	std::lock_guard<std::mutex>lock(m_t3d_mtx);
	return m_t3d;
}

Target6DOF TrackingData::get6d(const unsigned int id)
{
	std::lock_guard<std::mutex>lock(m_t6d_mtx);
	return m_t6d[id];
}

Target6DOF TrackingData::get6di(const unsigned int id)
{
	std::lock_guard<std::mutex>lock(m_t6di_mtx);
	return m_t6di[id];
}

Target6DOF TrackingData::getFlystick(const unsigned int id)
{
	std::lock_guard<std::mutex>lock(m_tfly_mtx);
	return m_tfly[id];
}


#endif //USING_OFL_TRACK_CPP
#ifndef USING_OFL_VMATH_CPP
#define USING_OFL_VMATH_CPP


namespace ofl
{

#ifdef GLM_INCLUDED

using namespace  glm;
#else

////// mat4 ////////////////////////////////////////////////////////////////////

mat4::mat4(float diag)
{
	for(unsigned int i =0 ; i< 4;i++)
	{
		m_data[i] = vec4(0,0,0,0);
		m_data[i][i] = diag;
	}
}

mat4::mat4(const vec4 &c0, const vec4 &c1, const vec4 &c2, const vec4 &c3)
{
	m_data[0] = c0; m_data[1] = c1; m_data[2] = c2; m_data[3] = c3;
}

vec4 &mat4::operator[](int i)	{return m_data[i];}

const vec4 &mat4::operator[](int i) const {return m_data[i];}

////// vec2 ////////////////////////////////////////////////////////////////////

vec2::vec2(const float x, const float y)
	:x(x),y(y)
{}

float &vec2::operator[](const unsigned int i){return data[i];}

float vec2::operator[](const unsigned int i) const{return data[i];}

vec2 vec2::operator +(const vec2 &v) const
{
	vec2 res = *this;
	for(int i =0 ; i< 2;i++)
		res.data[i] += v.data[i];
	return res;
}

vec2& vec2::operator +=(const vec2& v)
{
	vec2& res = *this;
	for(int i =0 ; i< 2;i++)
		res.data[i] += v.data[i];
	return res;
}


vec2 vec2::operator -(const vec2 &v) const
{
	vec2 res = *this;
	for(int i =0 ; i< 2;i++)
		res.data[i] -= v.data[i];
	return res;
}

vec2 vec2::operator *(const vec2 &v) const
{
	vec2 res = *this;
	for(int i =0 ; i< 2;i++)
		res.data[i] *= v.data[i];
	return res;
}
vec2& vec2::operator *=(const vec2& v)
{
	vec2& res = *this;
	for(int i =0 ; i< 2;i++)
		res.data[i] *= v.data[i];
	return res;
}

vec2 vec2::operator /(const vec2 &v) const
{
	vec2 res = *this;
	for(int i =0 ; i< 2;i++)
		res.data[i] /= v.data[i];
	return res;
}

vec2 vec2::operator *(const float f) const
{
	vec2 res = *this;
	for(int i =0 ; i< 2;i++)
		res.data[i] *=f;
	return res;
}

vec2 vec2::operator /(const float f) const
{
	vec2 res = *this;
	const float finv = 1.0f/f;
	for(int i =0 ; i< 2;i++)
		res.data[i] *=finv;
	return res;
}




////// vec3 ////////////////////////////////////////////////////////////////////

vec3::vec3(const float x, const float y, const float z)
	:x(x),y(y),z(z)
{}

float &vec3::operator[](const unsigned int i){return data[i];}

float vec3::operator[](const unsigned int i) const{return data[i];}

vec3 vec3::operator +(const vec3 &v) const
{
	vec3 res = *this;
	for(int i =0 ; i< 3;i++)
		res.data[i] += v.data[i];
	return res;
}
vec3& vec3::operator +=(const vec3& v)
{
	vec3& res = *this;
	for(int i =0 ; i< 3;i++)
		res.data[i] += v.data[i];
	return res;
}

vec3 vec3::operator -(const vec3 &v) const
{
	vec3 res = *this;
	for(int i =0 ; i< 3;i++)
		res.data[i] -= v.data[i];
	return res;
}

vec3 vec3::operator *(const vec3 &v) const
{
	vec3 res = *this;
	for(int i =0 ; i< 3;i++)
		res.data[i] *= v.data[i];
	return res;
}

vec3 vec3::operator /(const vec3 &v) const
{
	vec3 res = *this;
	for(int i =0 ; i< 3;i++)
		res.data[i] /= v.data[i];
	return res;
}

vec3 vec3::operator *(const float f) const
{
	vec3 res = *this;
	for(int i =0 ; i< 3;i++)
		res.data[i] *=f;
	return res;
}

vec3 vec3::operator /(const float f) const
{
	vec3 res = *this;
	const float finv = 1.0f/f;
	for(int i =0 ; i< 3;i++)
		res.data[i] *=finv;
	return res;
}


////// vec4 ////////////////////////////////////////////////////////////////////

vec4::vec4(const float x, const float y, const float z, const float w)
	:x(x),y(y),z(z),w(w)
{}

float &vec4::operator[](const unsigned int i){return data[i];}

float vec4::operator[](const unsigned int i) const{return data[i];}

vec4 vec4::operator +(const vec4 &v) const
{
	vec4 res = *this;
	for(int i =0 ; i< 4;i++)
		res.data[i] += v.data[i];
	return res;
}

vec4 vec4::operator -(const vec4 &v) const
{
	vec4 res = *this;
	for(int i =0 ; i< 4;i++)
		res.data[i] -= v.data[i];
	return res;
}

vec4 vec4::operator *(const vec4 &v) const
{
	vec4 res = *this;
	for(int i =0 ; i< 4;i++)
		res.data[i] *= v.data[i];
	return res;
}

vec4 vec4::operator /(const vec4 &v) const
{
	vec4 res = *this;
	for(int i =0 ; i< 4;i++)
		res.data[i] /= v.data[i];
	return res;
}

vec4 vec4::operator *(const float f) const
{
	vec4 res = *this;
	for(int i =0 ; i< 4;i++)
		res.data[i] *=f;
	return res;
}

vec4 vec4::operator /(const float f) const
{
	vec4 res = *this;
	const float finv = 1.0f/f;
	for(int i =0 ; i< 4;i++)
		res.data[i] *=finv;
	return res;
}


////////////////////////////////////////////////////////////////////////////////


vec3 normalize(const vec3& v)
{
	return v/length(v);
}

vec4 normalize(const vec4& v)
{
	return v / length(v);
}

float dot(const vec4& a, const vec4& b)
{
	float res = 0;
	for(int i =0  ;i<4;i++)
	{
		res += a[i]*b[i];
	}
	return res;
}

/*
 * Source: glm
 */
mat4 frustum(
		const float& l,
		const float& r,
		const float& b,
		const float& t,
		const float& n,
		const float& f
		)
{
	mat4 res(0);
	res[0][0] = (2.0f*n)/(r-l);
	res[1][1] = (2.0f*n)/(t-b);
	res[2][0] = (r+l)/(r-l);
	res[2][1] = (t+b)/(t-b);
	res[2][2] = -(f+n)/(f-n);
	res[2][3] = -1.0f;
	res[3][2] = -(2.0f*f*n)/(f-n);
	return res;
}

mat4 operator* ( const mat4 & A, const mat4 & B)
{
	mat4 res(0);
	for(int i = 0; i<4;i++)
	{
		res[i] = A[0]*B[i][0] + A[1]*B[i][1] + A[2]*B[i][2] + A[3]*B[i][3];
	}
	return res;
}


vec4 operator*(	const mat4 & m,	const vec4 & v	)
{
	vec4 res;
	for(int i = 0 ; i< 4; i++)
	{
		vec4 row(m[0][i],m[1][i],m[2][i],m[3][i]);
		res[i] = dot(row,v);
	}
	return res;
}



mat4 inverse(const mat4 &m)
{
	mat4 res;
#define DET(x1,x2,x3,y1,y2,y3,z1,z2,z3)(x1*(y2*z3-y3*z2)+y1*(z2*x3-x2*z3)+z1*(x2*y3-x3*y2))
	const float c00 =  DET(m[1][1],m[1][2],m[1][3],m[2][1],m[2][2],m[2][3],m[3][1],m[3][2],m[3][3]);
	const float c01 = -DET(m[1][2],m[1][3],m[1][0],m[2][2],m[2][3],m[2][0],m[3][2],m[3][3],m[3][0]);
	const float c02 =  DET(m[1][3],m[1][0],m[1][1],m[2][3],m[2][0],m[2][1],m[3][3],m[3][0],m[3][1]);
	const float c03 = -DET(m[1][0],m[1][1],m[1][2],m[2][0],m[2][1],m[2][2],m[3][0],m[3][1],m[3][2]);
	const float d = 1.0f / (m[0][0] * c00 + m[0][1] * c01 + m[0][2] * c02 + m[0][3] * c03);
	const float c10 = -DET(m[2][1],m[2][2],m[2][3],m[3][1],m[3][2],m[3][3],m[0][1],m[0][2],m[0][3]);
	const float c11 =  DET(m[2][2],m[2][3],m[2][0],m[3][2],m[3][3],m[3][0],m[0][2],m[0][3],m[0][0]);
	const float c12 = -DET(m[2][3],m[2][0],m[2][1],m[3][3],m[3][0],m[3][1],m[0][3],m[0][0],m[0][1]);
	const float c13 =  DET(m[2][0],m[2][1],m[2][2],m[3][0],m[3][1],m[3][2],m[0][0],m[0][1],m[0][2]);
	const float c20 =  DET(m[3][1],m[3][2],m[3][3],m[0][1],m[0][2],m[0][3],m[1][1],m[1][2],m[1][3]);
	const float c21 = -DET(m[3][2],m[3][3],m[3][0],m[0][2],m[0][3],m[0][0],m[1][2],m[1][3],m[1][0]);
	const float c22 =  DET(m[3][3],m[3][0],m[3][1],m[0][3],m[0][0],m[0][1],m[1][3],m[1][0],m[1][1]);
	const float c23 = -DET(m[3][0],m[3][1],m[3][2],m[0][0],m[0][1],m[0][2],m[1][0],m[1][1],m[1][2]);
	const float c30 = -DET(m[0][1],m[0][2],m[0][3],m[1][1],m[1][2],m[1][3],m[2][1],m[2][2],m[2][3]);
	const float c31 =  DET(m[0][2],m[0][3],m[0][0],m[1][2],m[1][3],m[1][0],m[2][2],m[2][3],m[2][0]);
	const float c32 = -DET(m[0][3],m[0][0],m[0][1],m[1][3],m[1][0],m[1][1],m[2][3],m[2][0],m[2][1]);
	const float c33 =  DET(m[0][0],m[0][1],m[0][2],m[1][0],m[1][1],m[1][2],m[2][0],m[2][1],m[2][2]);
#undef DET
#undef m
	res[0][0] = c00*d; res[1][0] = c10*d;res[2][0] = c20*d; res[3][0] = c30*d;
	res[0][1] = c01*d; res[1][1] = c11*d;res[2][1] = c21*d; res[3][1] = c31*d;
	res[0][2] = c02*d; res[1][2] = c12*d;res[2][2] = c22*d; res[3][2] = c32*d;
	res[0][3] = c03*d; res[1][3] = c13*d;res[2][3] = c23*d; res[3][3] = c33*d;
	return res;
}



mat4 transpose(const mat4& m)
{
	mat4 r;
	for(int i = 0 ; i<4;i++)
		for(int j = i ; j < 4;j++)
		{
			r[i][j] = m[j][i];
		}
	return r;
}



float length(const vec3& a)
{
	return sqrt(length2(a));
}
float length2(const vec3& a)
{
	return a.x*a.x+a.y*a.y+a.z*a.z;
}
float length(const vec4& a)
{
	return sqrt(length2(a));
}
float length2(const vec4& a)
{
return a.x*a.x+a.y*a.y+a.z*a.z+a.w*a.w;
}

float distance(const vec3 &a, const vec3 &b)
{
	return length(a-b);
}

float distance2(const vec3 &a, const vec3 &b)
{
	return length2(a-b);
}


vec3 cross(const vec3& a, const vec3& b)
{
	return vec3(a.y * b.z - b.y * a.z,
				a.z * b.x - b.z * a.x,
				a.x * b.y - b.x * a.y
				);
}

float dot(const vec3& a, const vec3& b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

mat4 translate(const mat4&m,const vec4& v)
{
	mat4 r = m;
	r[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
	return r;
}





#endif



bool operator < (const vec4& a, const vec4& b)
{

	if (fabs(a.x - b.x) < std::numeric_limits<float>::epsilon())
	{
		if (fabs(a.y - b.y) < std::numeric_limits<float>::epsilon())
		{
			if (fabs(a.z - b.z) < std::numeric_limits<float>::epsilon())
			{
				return a.w < b.w;
			}
			else
				return a.z < b.z;
		}
		else
		{
			return a.y < b.y;
		}
	}
	else
	{
		return a.x < b.x;
	}
}
bool operator == (const vec4& a, const vec4& b)
{
	return fabs(a.x - b.x) < std::numeric_limits<float>::epsilon() &&
			fabs(a.y - b.y) < std::numeric_limits<float>::epsilon() &&
			fabs(a.z - b.z) < std::numeric_limits<float>::epsilon() &&
			fabs(a.w - b.w) < std::numeric_limits<float>::epsilon();
}


bool operator < (const vec3& a, const vec3& b)
{
	if (fabs(a.x - b.x) < std::numeric_limits<float>::epsilon())
	{
		if (fabs(a.y - b.y) < std::numeric_limits<float>::epsilon())
		{
			return a.z < b.z;
		}
		else
		{
			return a.y < b.y;
		}
	}
	else
	{
		return a.x < b.x;
	}
}
bool operator == (const vec3& a, const vec3& b)
{
	return fabs(a.x - b.x) < std::numeric_limits<float>::epsilon() &&
			fabs(a.y - b.y) < std::numeric_limits<float>::epsilon() &&
			fabs(a.z - b.z) < std::numeric_limits<float>::epsilon();
}

std::string to_string (const vec4& v)
{
	return std::string("(")+
			std::to_string(v.x)+','+
			std::to_string(v.y)+','+
			std::to_string(v.z)+','+
			std::to_string(v.w)+')';
}

vec4 read_from_string(std::string& str)
{
	vec4 res(0.0f,0.0f,0.0f,1.0f);
	str = str.substr(str.find_first_of('(')+1);

	size_t loc_end = 0 ;
	for(int i = 0 ; i<4;i++)
	{
		auto loc = str.find(',');
		loc_end = str.find_first_of(')');
		if(loc > loc_end)
		{
			loc = loc_end;
		}
		std::string elem = str.substr(0,loc);
		ofl::trim(elem);
		res[i] = atof(elem.c_str());
		str = str.substr(loc+1);
		if(loc == loc_end)
			break;
	}
	return res;


}


}

#endif //USING_OFL_VMATH_CPP
#endif
