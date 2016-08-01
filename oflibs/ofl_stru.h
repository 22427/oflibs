#if 0
<begin_doc>
The MIT License (MIT)
================================================================================

Copyright (c) 2016 Timon Zietlow

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

--------------------------------------------------------------------------------

What is this?
================================================================================
The oflibs are a set of usefull classes and tools dealing with all kinds of 
problems.  
My personal focus is in 3D-visualisation and VR, so most of these deal with 
these topics. OFLIBS stands for "**o**ne **f**ile **lib**rarie**s**". The basic 
idea is to write libraries in a single file, which you just have to include into
your project, and you are ready to go.
I find these usefull for fast prototypes and "experimental" code, which is what 
this collection currently is, so don't expect a well tested, woking toolset, but
maybe you find the one or two lines of code you were searching ;).
I'm constantly working whith these tools and I am shure they will get better 
over time.

Structure
--------------------------------------------------------------------------------
In oflibs/ you find the finished one file libraries. They are ready to use. The 
documentation to each library is also inside of the lib. Here is a short list of
the different modules and libraries:

- ofl_all: All oflibs in one 
- ofl_ogl_geo - A class reperesenting vertex-data on the GPU
- ofl_ogl_state - A state-wrapper imulating the "classic" OpenGL-fixed-function 
  states, including matrix stacks and lighting.
- ofl_ogl_win - An interface to create an OpenGL-Context with window and reading 
  events. Currently implemented using glfw.
- ofl_open_gl_tools: combines the above.
- ofl_socket: A class wrapping sockets.
- ofl_stru: Some string utilities. Used by many other oflibs.
- ofl_track - Tracking: An interface to the ART-DTrack2 tracking system.
- olf_vd - Vertex Data: A data structure representing geometry on the client 
  side. This is OpenGL independend. 
- ofl_vdman - Vertex Data MANufacturer: Allows you to create VertexData using
  the old begin(..) ... end() paradigma from fixed-function-GL, but is OpenGL
  independend.
- ofl_vmath - Vector MATH : Some simple vector operations, but no alternative to 
  GLM.
- ofl_vrpv - Virtual Reality Projection and View - A module to genereate view-
  and projection-matrices for VR systems like PowerWalls or CAVEs

How do oflibs work?
--------------------------------------------------------------------------------
You use the oflib as a regular header, for example:
```c_cpp
	#include <ofl_all.h>
```
Then in one .cpp-file of your project you include the oflib defining
`OFL_IMPLEMENTATION` before:
```c_cpp
	#define OFL_IMPLEMENTATION
	#include <ofl_all.h>
	#undef OFL_IMPLEMENTATION
```
That's it :)

Internally the oflib-header is organized in two parts, the declaration-part 
(that's what you would write into an ordinary header) and the implementation-part 
(that's what you would write into *.cpp files)
Example:
```c_cpp
	#ifndef OFL_ALL_H
	#define OFL_ALL_H
		// ALL DECLARATIONS
	#endif
	#ifdef OFL_IMPLEMENTATION
		// ALL THE IMPLEMENTATIONS
	#endif
```

Where can I get more information?
--------------------------------------------------------------------------------
Inside the oflib files there is the documentation for each module used. 
In the subdirectory tools/ there are also some more or less simple tools using 
the oflibs.




--------------------------------------------------------------------------------

<end_doc>
#endif 
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
#ifdef OFL_IMPLEMENTATION

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
#endif
