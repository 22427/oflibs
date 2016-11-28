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

#ifndef USING_OFL_VD_MD
#define USING_OFL_VD_MD
//VD: Vertex Data
//================================================================================
//This module constist of two structures the VertexData and VertexDataTools.
//The first is a simple, straight forward datastructure to store geometry in
//a renderable fashion.
//VertexDataTools contain methods to load and store vertex data and some 
//simple functions to calculate normals and tangents.
//
//Usage
//--------------------------------------------------------------------------------
//If you use glm, make sure, that glm.hpp is included before you include vd.h
//so that vd uses glm. If you don't use glm (which is fine) vd will use its own
//glm compatible, rudimentary mat4 and vec4 implementation.
//
//
//File-formats
//--------------------------------------------------------------------------------
//The .vd file format is a simple memory dump of a VertexData Object.
//The header is organized in "lines" each of 5 unsigned integers.
//The first line contains information about the other lines and the object.
//The other lines contain information about the attributes.
//The last one contains information about the size of the actual data.
//
//| 0          | 1            | 2       | 3              | 4               |
//|------------|--------------|---------|----------------|-----------------|
//| VDFF       | #lines-1     | version | #attributes    | primitive type  |
//| ATTRIBUTE0 | #elements    | type    | normalized?    | stride in bytes |
//| ATTRIBUTE1 | ...          | ...     | ...            | ...             |
//| #vertices  | sizeof(data) |#indices | sizeof(indeces)| index type      |
//
//Then comes the data as BLOB

#endif //USING_OFL_VD_MD

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
#ifndef USING_OFL_VD_H
#define USING_OFL_VD_H
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

/** @include vmath.md */

namespace ofl
{
enum Primitive
{
	POINTS = 0x0000,			// == GL_POINTS
	LINES = 0x0001,				// == GL_LINES
	//LINE_LOOP = 0x0002,		// == GL_LINE_LOOP
	LINE_STRIP = 0x0003,		// == GL_LINE_STRIP
	TRIANGLES = 0x0004,			// == GL_TRIANGLES
	TRIANGLE_STRIP = 0x0005,	// == GL_TRIANGLE_STRIP
	//TRIANGLE_FAN = 0x0006,	// == GL_TRIANGLE_FAN
	QUADS = 0x0007,				// == GL_QUADS
	QUAD_STRIP = 0x0008,		// == GL_QUAD_STRIP
	//POLYGON = 0x0009,			// == GL_POLYGON
};

enum Attribute
{
	POSITION =1,
	NORMAL = 2,
	TEXCOORD = 4,
	TANGENT = 8,
	COLOR = 16,
	ATTRIBUTE_LAST = COLOR
};

enum Types
{
	BYTE =0x1400,				// == GL_BYTE
	UNSIGNED_BYTE =0x1401,		// == GL_UNSIGNED_BYTE
	SHORT =0x1402,				// == GL_SHORT
	UNSIGNED_SHORT= 0x1403,		// == GL_UNSIGNED_SHORT
	INT= 0x1404,				// == GL_INT
	UNSIGNED_INT= 0x1405,		// == GL_UNSIGNED_INT
	FLOAT= 0x1406,				// == GL_FLOAT
	DOUBLE =0x140A,				// == GL_DOUBLE
};
class Vertex
{
	vec3 m_pos;
	vec3 m_normal;
	vec3 m_texcoord;
	vec3 m_tangent;
	vec4 m_color;
public:
	Vertex(const vec3& pos = vec3(0, 0, 0),
		   const vec3& nrm = vec3(0, 0, 0),
		   const vec4& clr = vec4(1, 1, 1, 1),
		   const vec3& tex = vec3(0, 0, 0),
		   const vec3& tan = vec3(0, 0, 0))
	{
		m_pos = pos;
		m_normal = nrm;
		m_color = clr;
		m_texcoord = tex;
		m_tangent = tan;
	}

	const vec3& pos() const { return m_pos; }
	const vec3& nrm() const { return m_normal; }
	const vec4& clr() const { return m_color;}
	const vec3& tex() const { return m_texcoord; }
	const vec3& tan() const { return m_tangent; }

	vec3& pos()  { return m_pos; }
	vec3& nrm()  { return m_normal; }
	vec4& clr()  { return m_color;}
	vec3& tex()  { return m_texcoord; }
	vec3& tan()  { return m_tangent; }

	void setNormal(const vec3& n) { m_normal = n; }
	void setPosition(const vec3& p) { m_pos = p; }
	void setTexcoord(const vec2& u)
	{
		m_texcoord.x = u.x;
		m_texcoord.y = u.y;
		m_texcoord.z = 0;
	}
	void setTexcood(const vec3& u)	{ m_texcoord = u; }
	void setColor(const vec4& c) {	m_color = c; }
	void setTangent(const vec3& t)	{m_tangent = t; }

	bool operator == (const Vertex& o);
	bool operator < (const Vertex& o)	const;
};


/**
 * @brief The VertexData class represents vertex data in a renderable form.
 */
class VertexData
{
private:
	std::vector<Vertex> m_data;
	std::vector<uint32_t> m_indices_data;
	Primitive m_render_primitive;

public:
	VertexData(Primitive primitive = TRIANGLES);
	virtual ~VertexData();
	
	/**
	 * @brief push_back adds an index to the index list
	 * @param i the new index
	 */
	void push_back(const uint32_t& i)
	{
		this->m_indices_data.push_back(i);
	}
	
	/**
	 * @brief push_back adds a Vertex to the vertex list.
	 * @param v The new Vertex
	 * @return the index of the newly added vertex.
	 */
	size_t push_back(const Vertex& v);

	/**
	 * @brief data gives read access to the Vertex list
	 * @return The vertex list.
	 */
	const std::vector<Vertex>& data() const;
	
	/**
	 * @brief indices gives read access to the index list
	 * @return The index list
	 */
	const std::vector<uint32_t>& indices() const;
	
	/**
	 * @brief data gives write access to the Vertex list
	 * @return The vertex list.
	 */
	std::vector<Vertex>& data();
	
	/**
	 * @brief indices gives write access to the index list
	 * @return The index list
	 */
	std::vector<uint32_t>& indices();
	
	/**
	 * @brief primitive 
	 * @return The vertex datas primitive
	 */
	virtual Primitive primitive() const;
	/**
	 * @brief setPrimitive sets the primitive mode this vertex data is 
	 * constructed in.
	 * @param p new primitive 
	 */
	void setPrimitive(const Primitive& p);

	auto begin() -> decltype(m_data.begin())
	{return m_data.begin();}
	auto end() -> decltype(m_data.end())
	{return m_data.end();}
};


class VertexDataTools
{
protected:
	VertexData* readVD(const std::string& path);
	VertexData* readOBJ(const std::string& path);
	VertexData* readPLY(const std::string& path);

	bool writeVD(const VertexData* ofl, const std::string& path);
	bool writeOBJ(const VertexData* ofl, const std::string& path);
	bool writePLY(const VertexData* ofl, const std::string& path);
public:
	enum Format
	{
		OBJ,
		PLY,
		VD,
		FROM_PATH
	};
	/**
	 * @brief writeToFile writes the given vertexdaa into a file.
	 * @param vd Vertex data to write from
	 * @param path Path to write to
	 * @param f Format of the outpu file. if FROM_PATH is used, the format will
	 * be determined from the file ending
	 * @return true if everything went well, false if there was a problem.
	 */
	bool writeToFile(const VertexData* vd,const std::string& path, Format f=FROM_PATH);
	
	/**
	 * @brief readFromFile reads VertexData from a file
	 * @param path Path to the source file.
	 * @param f The format of the source file
	 * @return the VertexData read, or a nullptr, if something went wrong.
	 */
	VertexData* readFromFile(const std::string& path, Format f = FROM_PATH);

	void calculateNormals(VertexData* vd);
	void calculateTangents(VertexData* vd);
};
}

#endif //USING_OFL_VD_H
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
#ifndef USING_OFL_VD_CPP
#define USING_OFL_VD_CPP
#include <cstdio>
#include <fstream>
#include <map>


#define atoff(s) static_cast<float>(atof(s))
#define atoiu(s) static_cast<uint32_t>(atoi(s))

using namespace ofl;
namespace ofl {

bool Vertex::operator ==(const Vertex &o)
{
	bool to_ret = true;
	float* data = reinterpret_cast<float*>(this);
	const float* odata = reinterpret_cast<const float*>(&o);
	const float eps = std::numeric_limits<float>::epsilon();
	float d;
	for (uint i = 0; i < sizeof(Vertex)/sizeof(float); i++)
	{
		d = fabsf(odata[i] - data[i]);
		to_ret = to_ret && d <= eps;
	}
	return to_ret;
}

bool Vertex::operator <(const Vertex &o) const
{
	const float* data = reinterpret_cast<const float*>(this);
	const float* odata = reinterpret_cast<const float*>(&o);
	for (uint i = 0; i < sizeof(Vertex)/sizeof(float); i++)
	{
		const float& a = data[i];
		const float& b = odata[i];
		if (a < b)
			return true;
		else if (a > b)
			return false;
		else
			continue;
	}
	return false;
}

VertexData::VertexData(Primitive primitive)
{
	m_render_primitive = primitive;
}


VertexData::~VertexData()
{

}



size_t VertexData::push_back(const Vertex &v)
{
	m_data.push_back(v);
	return m_data.size()-1;
}

const std::vector<Vertex>& VertexData::data() const
{
	return m_data;
}

const std::vector<uint32_t>& VertexData::indices() const
{
	return m_indices_data;
}

std::vector<Vertex> &VertexData::data()
{
	return m_data;
}

std::vector<uint32_t>& VertexData::indices()
{
	return m_indices_data;
}

Primitive VertexData::primitive() const
{
	return m_render_primitive;
}

void VertexData::setPrimitive(const Primitive &p)
{
	m_render_primitive = p;
}



VertexData *VertexDataTools::readVD(const std::string &path)
{
	VertexData* vd = new VertexData();

	FILE* f = fopen(path.c_str(),"rb");
	uint32_t first_line[5];
	uint32_t last_line[5];
	fread(first_line,5,4,f);

	for(unsigned int i = 0; i<first_line[3]+1;i++)
		fread(last_line,5,4,f);

	vd->data().resize(last_line[0]);
	vd->indices().resize(last_line[2]);

	fread(vd->data().data(),last_line[1],1,f);
	fread(vd->indices().data(),last_line[3],1,f);
	fclose(f);
	return vd;
}



void handle_v(VertexData* vd, std::map<Vertex,int>& v_loc, const Vertex& v)
{
	int v_id = 0;
	if(v_loc.find(v)!= v_loc.end())
	{
		v_id = v_loc[v];
	}
	else
	{
		v_id =  static_cast<int>(vd->push_back(v));
		v_loc[v] = v_id;
	}
	vd->push_back(static_cast<const uint32_t>(v_id));
}

VertexData *VertexDataTools::readOBJ(const std::string &path)
{

	// mesh loader the 10000ths ^^

	std::ifstream fstream(path.c_str());
	if (!fstream.is_open())
		return nullptr;
	VertexData* vd = new VertexData();

	std::vector<vec3> positions;
	std::vector<vec3> normals;
	std::vector<vec2> tex_coords;

	std::map<Vertex,int> v_loc;

	std::string line;
	std::string type;
	char* arg[4];

	Tokenizer tkn("");
	Tokenizer attrib_tkn("");
	while (std::getline(fstream, line))
	{
		// remove comment
		line = line.substr(0, line.find_first_of('#'));
		trim(line);

		if (line.empty()) // the line was an empty line or a comment
			continue;

		attrib_tkn.reset(line);

		type = attrib_tkn.getToken(' ');

		for (int i = 0; i < 4; i++)
		{
			attrib_tkn.skipWhiteSpaces();
			arg[i] = attrib_tkn.getToken(' ');
		}


		trim(type);

		if (type == "v")
		{
			positions.push_back(vec3(atoff(arg[0]),atoff(arg[1]),
					atoff(arg[2])));
		}
		else if (type == "vn")
		{
			normals.push_back(vec3(atoff(arg[0]), atoff(arg[1]),
					atoff(arg[2])));
		}
		else if (type == "vt")
		{
			tex_coords.push_back(vec2(atoff(arg[0]),
								 atoff(arg[1])));
		}
		else if (type == "f")
		{
			Vertex v;

			if (arg[3])
			{

			}
			else
			{
				if(normals.empty() && tex_coords.empty())  // only positions
				{
					for (int i = 0; i < 3; i++)
					{

						uint32_t p_id  = atoiu(arg[i]) - 1;

						v.pos() = positions[p_id];
						handle_v(vd,v_loc,v);
					}
				}
				else if (normals.empty())	// positions and texcoords
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.reset(std::string(arg[i]));
						uint  p_id = atoiu(tkn.getToken('/')) - 1;
						uint t_id = atoiu(tkn.getRest()) - 1;
						v.pos() = positions[p_id];
						v.setTexcoord(tex_coords[t_id]);
						handle_v(vd,v_loc,v);
					}
				}
				else if (tex_coords.empty())	 // positions and normals
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.reset(std::string(arg[i]));
						uint  p_id = atoiu(tkn.getToken('/')) - 1;
						tkn.getToken('/');
						uint  n_id = atoiu(tkn.getRest()) - 1;

						v.pos() = positions[p_id];
						v.nrm() = normals[n_id];

						handle_v(vd,v_loc,v);
					}
				}
				else		// all three
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.reset(std::string(arg[i]));
						uint  p_id = atoiu(tkn.getToken('/')) - 1;
						uint  t_id = atoiu(tkn.getToken('/')) - 1;
						uint  n_id = atoiu(tkn.getRest()) - 1;

						v.pos() = positions[p_id];
						v.setTexcoord(tex_coords[t_id]);
						v.nrm() = normals[n_id];

						handle_v(vd,v_loc,v);

					}
				}
			}
		}
	}
	fstream.close();
	vd->setPrimitive(TRIANGLES);
	/*if (normals.empty())
		   calculateNormals();*/
	return vd;
}

enum PLY_DATA_TYPE
{
	plyFLOAT,
	plyDOUBLE,
	plyUINT,
	plyINT,
	plyUSHORT,
	plySHORT,
	plyUCHAR,
	plyCHAR,

};

VertexData *VertexDataTools::readPLY(const std::string &path)
{
	std::ifstream fstream(path.c_str());

	if (!fstream.is_open())
		return nullptr;
	VertexData* vd = new VertexData();

	std::string line;

	Tokenizer tkn("");
	//get the headers information
	int vertex_count = 0;
	int face_count = 0;

	std::vector<std::string> properties;
	std::vector<PLY_DATA_TYPE> prop_types;

	bool vertex_prop = false;

	while (std::getline(fstream, line))
	{
		tkn.reset(line);
		std::string op = tkn.getToken(' ');
		if (op == "element")
		{
			std::string s = tkn.getToken(' ');
			if (s == "vertex")
			{
				vertex_prop = true;
				vertex_count = atoi(tkn.getRest());
			}
			else if (s == "face")
			{
				face_count = atoi(tkn.getRest());
				vertex_prop = false;
			}
			else
			{
				printf("PLY loader ignored element %s\n", s.c_str());
			}
		}
		else if (op == "property" && vertex_prop)
		{
			std::string type = tkn.getToken(' ');
			std::string name = tkn.getToken(' ');

			PLY_DATA_TYPE dt = plyFLOAT;
			if (type == "float") dt = plyFLOAT;
			else if (type == "double") dt = plyDOUBLE;
			else if (type == "uint") dt = plyUINT;
			else if (type == "int") dt = plyINT;
			else if (type == "ushort") dt = plyUSHORT;
			else if (type == "short") dt = plySHORT;
			else if (type == "uchar") dt = plyUCHAR;
			else if (type == "char") dt = plyCHAR;
			properties.push_back(name);
			prop_types.push_back( dt);
		}
		else if (op == "end_header")
		{
			break;
		}

	}

	// now read the vertex data


	Vertex vtx;

	for (int i = 0; i < vertex_count; i++)
	{
		std::getline(fstream, line);
		tkn.reset(line);
		for (unsigned int j = 0; j < properties.size(); j++)
		{
			const auto& prp = properties[j];
			const auto& typ = prop_types[j];

			float val = static_cast<float>(atof(tkn.getToken(' ')));

			if (typ == plyUCHAR)
				val /= 255.0f;

			if (prp[0] == 'x') vtx.pos()[0] = (val);
			else if (prp[0] == 'y') vtx.pos()[1] = (val);
			else if (prp[0] == 'z') vtx.pos()[2] = (val);
			else if (prp[0] == 's') vtx.tex()[0] = (val);
			else if (prp[0] == 't') vtx.tex()[1] = (val);
			else if (prp[0] == 'r') vtx.clr()[0] = (val);
			else if (prp[0] == 'g') vtx.clr()[1] = (val);
			else if (prp[0] == 'b') vtx.clr()[2] = (val);
			else if (prp[0] == 'n')
			{
				if( prp[1] == 'x') vtx.nrm()[0] = (val);
				else if (prp[1] == 'y') vtx.nrm()[1] = (val);
				else if (prp[1] == 'z') vtx.nrm()[2] = (val);
			}

		}
		vd->push_back(vtx);
	}
	// read the faces

	for (int i = 0; i < face_count; i++)
	{
		std::getline(fstream, line);
		tkn.reset(line);
		auto verts_in_this_face = atoi(tkn.getToken(' '));

		uint32_t vert0 = static_cast<uint32_t>(atoi(tkn.getToken(' ')));
		uint32_t vert1 = static_cast<uint32_t>(atoi(tkn.getToken(' ')));

		for (int i = 2; i < verts_in_this_face; i+=1)
		{
			uint32_t vert2 = static_cast<uint32_t>(atoi(tkn.getToken(' ')));
			vd->push_back(vert0);
			vd->push_back(vert1);
			vd->push_back(vert2);
			vert1 = vert2;
		}
	}
	fstream.close();
	return vd;
}

bool VertexDataTools::writeVD(const VertexData *vd, const std::string &path)
{
	FILE* f = fopen(path.c_str(),"wb");
	if(!f)
		return 0;
	std::string prefix = "VDFF";
	uint32_t hline[5];

	uint32_t num_attrib = static_cast<uint32_t>(::log2(ATTRIBUTE_LAST)+1.0);
	hline[0] = *(reinterpret_cast<const uint32_t*>(prefix.c_str()));
	hline[1] = (num_attrib+1)*5*sizeof(uint32_t); //headr size
	hline[2] = 1; // version
	hline[3] = num_attrib;
	hline[4] = vd->primitive();

	fwrite(hline,1,5*sizeof(uint32_t),f);
	const Vertex v = vd->data()[0];

#define addr_diff(a,b) static_cast<unsigned int>(\
		((reinterpret_cast<const char*>(a)-\
		  reinterpret_cast<const char*>(b))))

	hline[0] = POSITION;
	hline[1] = 3;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.pos()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
	hline[0] = NORMAL;
	hline[1] = 3;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.nrm()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
	hline[0] = TEXCOORD;
	hline[1] = 3;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.tex()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
	hline[0] = TANGENT;
	hline[1] = 3;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.tan()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
	hline[0] = COLOR;
	hline[1] = 4;
	hline[2] = FLOAT;
	hline[3] = 0;
	hline[4] = addr_diff(&(v.clr()),&v);
	fwrite(hline,1,5*sizeof(uint32_t),f);
#undef addr_diff

	hline[0] = static_cast<uint32_t>(vd->data().size());
	hline[1] = static_cast<uint32_t>(vd->data().size()*sizeof(Vertex));
	hline[2] = static_cast<uint32_t>(vd->indices().size());
	hline[3] = static_cast<uint32_t>(vd->indices().size() * sizeof(uint32_t));
	hline[4] = UNSIGNED_INT;
	fwrite(hline,1,5*sizeof(uint32_t),f);

	fwrite(vd->data().data(),vd->data().size(),sizeof(Vertex),f);
	fwrite(vd->indices().data(),vd->indices().size(),sizeof(uint32_t),f);
	fclose(f);
	return true;
}

bool VertexDataTools::writeOBJ(const VertexData *vd, const std::string &path)
{
	FILE* f = fopen(path.c_str(),"w");
	if(!f)
		return false;

	struct obj_vert{int p;int t;int n;};
	typedef struct{obj_vert v[3];} obj_face;

	std::map<vec3,int,compare_vec_3> pos2id;
	int pid = 1;
	std::map<vec3,int,compare_vec_3> nrm2id;
	int nid = 1;
	std::map<vec3,int,compare_vec_3> tex2id;
	int tid = 1;
	//std::vector<obj_face> faces;
	const Primitive& prim = vd->primitive();
	uint consumed = 0;
	if(prim == TRIANGLES)
		consumed = 3;
	else if(prim == TRIANGLE_STRIP)
		consumed = 1;
	else if(prim == QUADS)
		consumed = 4;
	else if(prim == QUAD_STRIP)
		consumed = 2;
	uint vs = 4 -consumed%2;


	for(unsigned int i = 0 ; i< vd->indices().size();i+=consumed)
	{

		uint v[4];
		if (consumed >= 3 || i%2 ==1)
		{
			for (uint j = 0; j < vs; j++)
			{
				v[j] = vd->indices()[i+j];
			}
		}
		else
		{
			for (uint j = 0; j < vs; j++)
			{
				const uint idx = j==0? 1 : j==1? 0:j;
				v[j] = vd->indices()[i+idx];
			}
		}

		obj_face face;
		for (uint j = 0; j < vs; j++)
		{
			const Vertex& vtx = vd->data()[v[j]];
			if(pos2id.find(vtx.pos()) == pos2id.end())
			{
				fprintf(f,"v %f %f %f\n",vtx.pos().x,vtx.pos().y,vtx.pos().z);
				pos2id[vtx.pos()] = pid;
				face.v[j].p = pid;
				pid ++;
			}
			else
			{
				face.v[j].p = pos2id[vtx.pos()];
			}

			if(nrm2id.find(vtx.nrm()) == nrm2id.end())
			{
				fprintf(f,"vn %f %f %f\n",vtx.nrm().x,vtx.nrm().y,vtx.nrm().z);
				nrm2id[vtx.nrm()] = nid;
				face.v[j].n = nid;
				nid ++;
			}
			else
			{
				face.v[j].n = nrm2id[vtx.nrm()];
			}

			if(tex2id.find(vtx.tex()) == tex2id.end())
			{
				fprintf(f,"vt %f %f\n",vtx.tex().x,vtx.tex().y);
				tex2id[vtx.tex()] = tid;
				face.v[j].t = tid;
				tid ++;
			}
			else
			{
				face.v[j].t = tex2id[vtx.tex()];
			}
		}
		fprintf(f,"f");
		for (uint j = 0; j < vs; j++)
		{
			fprintf(f," %d/%d/%d",face.v[j].p,face.v[j].t,face.v[j].n);
		}
		fprintf(f,"\n");
	}

	fclose(f);
	return true;
}

bool VertexDataTools::writePLY(const VertexData */*vd*/, const std::string &/*path*/)
{
	return false;
}

bool VertexDataTools::writeToFile(
		const VertexData *vd,
		const std::string &p,
		VertexDataTools::Format f)
{


	std::string ending = paths::extension(p);
	if (f == FROM_PATH)
	{
		if(ending == "obj" || ending == "OBJ")
			f = OBJ;
		else if(ending == "ply" || ending == "PLY")
			f = PLY;
		else if(ending == "vd" || ending == "vd")
			f = VD;
	}

	switch (f)
	{
	case VD:
		return writeVD(vd,p);
	case OBJ:
		return writeOBJ(vd,p);
	case PLY:
		return writePLY(vd,p);
	case FROM_PATH:
		return false;
	}
	return false;
}

VertexData* VertexDataTools::readFromFile(
		const std::string &path,
		VertexDataTools::Format f)
{
	std::string ending = paths::extension(path);
	if (f == FROM_PATH)
	{
		if(ending == "obj" || ending == "OBJ")
			f = OBJ;
		else if(ending == "ply" || ending == "PLY")
			f = PLY;
		else if(ending == "vd" || ending == "vd")
			f = VD;
	}
	switch (f) {
	case VD:
		return readVD(path);
	case OBJ:
		return readOBJ(path);
	case PLY:
		return readPLY(path);
	case FROM_PATH:
		return nullptr;
	}
	return nullptr;
}

void VertexDataTools::calculateNormals(VertexData *vd)
{
	std::vector<Vertex>& verts =  vd->data();

	const std::vector<uint32_t>& indices = vd->indices();

	std::map<vec3, vec3, compare_vec_3> p2n;

	Primitive prim = vd->primitive();


	uint consumed = 0;
	if(prim == TRIANGLES)
		consumed = 3;
	else if(prim == TRIANGLE_STRIP)
		consumed = 1;
	else if(prim == QUADS)
		consumed = 4;
	else if(prim == QUAD_STRIP)
		consumed = 2;
	uint vs = 4 -consumed%2;
	if (prim != 0)
	{
		for (unsigned int i = 0; i < indices.size(); i += consumed)
		{
			const vec3* v[4];
			if (consumed >= 3 || i%2 ==1)
			{
				for (uint j = 0; j < vs; j++)
				{
					v[j] = &(verts.at(indices[i+j]).pos());
				}
			}
			else
			{
				for (uint j = 0; j < vs; j++)
				{
					const uint idx = j==0? 1 : j==1? 0:j;
					v[j] = &(verts.at(indices[i+idx]).pos());
				}
			}

			int connected = -1;
			if(vs == 4) //QUADS
			{
				if(length2(*(v[0])-*(v[2])) < length2(*(v[1])-*(v[3])))
				{
					connected = 0;
				}
			}
			/* calculate normalized normal */
			vec3 n = normalize(cross(*(v[1])- *(v[0]), *(v[2]) - *(v[0])));
			float area = 0.5f*length(cross(*(v[1])- *(v[0]), *(v[2]) - *(v[0])));

			/* add the normal to each corner of the triangle */
			for (uint j = 0; j < vs; j++)
			{
				const vec3 A = *(v[(j+1)%vs])- *(v[j]);
				const vec3 B = *(v[(j-1)%vs])- *(v[j]);
				const vec3 C = *(v[(j+2)%vs])- *(v[j]);
				vec3& r = p2n[*(v[j])];
				if(vs == 4) // for a quad triangulate first
				{
					if(static_cast<int>(j % 2) != connected)
					{
						const vec3 cr = cross(A,B);
						area = 0.5f* length(cr);
						n = normalize(cr);
						const float w = area/((dot(A,A)*dot(B,B)));
						r += n*w ;
					}
					else
					{
						vec3 cr = cross(A,C);
						area = 0.5f* length(cr);
						n = normalize(cr);
						r += n*area/((dot(A,A)*dot(C,C)));

						cr = cross(C,B);
						area = 0.5f* length(cr);
						n = normalize(cr);
						r += n*area/((dot(C,C)*dot(B,B)));

					}
				}
				else //for a triangle use the one time normal.
				{
					const float w = area/((dot(A,A)*dot(B,B)));
					r += n*w ;
				}
			}
		}

		/*normalize all normals*/
		for (auto& pn : p2n)
			pn.second = normalize(pn.second);

		/* set all the normals */
		for (Vertex& v : verts)
		{
			v.nrm() = p2n[v.pos()];
		}
	}
}

void VertexDataTools::calculateTangents(VertexData *vd)
{

	for (auto& vert : vd->data())
	{
		vert.setTangent(vec3(0,0,0));
	}

	std::vector<Vertex>& vertex_data = vd->data();
	std::vector<uint32_t>& indices = vd->indices();

	for (unsigned int i = 0; i < indices.size(); i+=3)
	{
		const vec3  v0 = vertex_data[indices[i + 0]].pos();

		const vec3  v1 = vertex_data[indices[i + 1]].pos();
		const vec3  v2 = vertex_data[indices[i + 2]].pos();

		const vec3  uv0 = vertex_data[indices[i + 0]].tex();
		const vec3  uv1 = vertex_data[indices[i + 1]].tex();
		const vec3  uv2 = vertex_data[indices[i + 2]].tex();


		vec3 dPos1 = v1 - v0;
		vec3 dPos2 = v2 - v0;
		vec3 dUV1 = uv1 - uv0;
		vec3 dUV2 = uv2 - uv0;


		float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

		vec3 t = (dPos1 * dUV2.y - dPos2 * dUV1.y)*r;
		//		vec3 b = (dPos2 * dUV1.x - dPos1 * dUV2.x)*r;

		vertex_data[indices[i + 0]].tan() += t;
		vertex_data[indices[i + 1]].tan() += t;
		vertex_data[indices[i + 2]].tan() += t;

	}

	for (auto& vert : vertex_data)
	{
		vec3 nt = normalize(vert.tan());
		vert.setTangent(vec3(nt));
	}


}




}

#endif //USING_OFL_VD_CPP
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
