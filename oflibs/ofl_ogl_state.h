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

//OGL_STATE: OpenGL State
//================================================================================
//This is a state tracking class, which also provides some stock shaders and
//functions.
//
//###State tracking:
//There is a matrix stack for modelview- and projection-matrix, and a state
//foreight light sources. Just like in OpenGL 1.x
//
//###Stock Shaders:
//There are shaders for texturing, phong shading and distance field alpha
//handling.
//
//Usage
//--------------------------------------------------------------------------------
//
//You need to specify an OpenGL header in as OFL_GL_HEADER. for example:  
//
//```
//	#define OFL_GL_HEADER <glad/glad.h>  
//	#define OFL_GL_HEADER <glew.h>
//```
//
//This module also uses GLM ... sorry I was too lazy to implement all
//these functions, and its not feasible anyway.
//
//
//API
//--------------------------------------------------------------------------------
//
//###Matrix-Stacks:
//There are two stacks. `PROJECITON` and `MODEL_VIEW`. You can change which is
//modified using the `matrixMode(...)` method.
//with `pushMatrix()` and `popMatrix()` you can either push a copy of the top-most
//matrix onto the stack or pop the top-most matrix from the stack.
//All other matrix modifying methods effect the top-most(current)
//matrix of the selected stack:  
//
//```
//void loadIdentity();//   sets the matrix as identity  
//void translate(..);//    translates the current matrix  
//void rotate(...);//      rotates the current matrix  
//void scale(...);//       scales the current matrix  
//void lookAt(...);//      multiplies the current matrix with a lookAt-matix  
//void ortho(...);//       multiplies the current matrix with an ortho-matix  
//void frustum(...);//     multiplies the current matrix with a frustum-matix  
//void perspective(...);// multiplies the current matrix with a persp.-matix  
//```
//###Lights:
//You can set Light parameters using the `setLight*(...)` methods.
//
//###Stock-shaders:
//You can select different stock shaders by calling `enable(...)` or `disable(...)`
//fordifferent features.
//The features are:
//
//* Texturing: Basic color mapping
//* Lighting: Basic per fragment lighting
//* DFAlpha: Iterpreting the Alpha channel as distance field.
//* NonStockShader: Use the set non stock shader. If this feature is
//enabled all uniforms and attributes will still be set, but your shader
//will be used.
//

--------------------------------------------------------------------------------

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

--------------------------------------------------------------------------------

<end_doc>
#endif 
#ifndef USING_OFL_OGL_STATE_H
 #define USING_OFL_OGL_STATE_H
 
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
 * @brief cross cross product only using the .xyz part.
 * @param a
 * @param b
 * @return cross(a.xyz,b.xyz)
 */
vec4 cross(const vec4& a, const vec4& b);

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
#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif

#include OFL_GL_HEADER

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <fstream>
#include <streambuf>
#include <map>
#include <vector>
#include <stack>


namespace ofl
{


#define NAME_MVM "mvm" // model view matrix
#define NAME_PJM "pjm" // projection matrix
#define NAME_NMM "nmm" // normal matrix
#define NAME_CMAP "cmap"

#define ID_MVM 0
#define ID_PJM 1
#define ID_NMM 2
#define ID_CMAP 3

#define NAME_POS_ATTR "position"
#define NAME_NRM_ATTR "normal"
#define NAME_CLR_ATTR "color"
#define NAME_TEX_ATTR "texcoord"


#define ALOC_POSITION 0
#define ALOC_NORMAL 1
#define ALOC_COLOR 2
#define ALOC_TEXCOORD 3
#define ALOC_TANGENT 4


enum MatrixMode
{
	MODELVIEW = 0,
	PROJECTION,
	MATRIXMODE_COUNT
};
enum StockShader
{
	PASS_THROUGH = 0,
	STOCK_SHADER_COUNT
};


enum Feature
{
	Texturing = 1,
	Lighting = 2,
	DFAlpha = 4,
	NonStockShader =8,
	FEATURE_STATE_COUNT = 9
};
inline std::string to_string(const Feature f)
{
	switch (f) {
	case Texturing: return "texturing";
	case Lighting: return "lighting";
	case DFAlpha: return "DF-alpha";
	case NonStockShader: return "non stock shader";
	case FEATURE_STATE_COUNT:return "FEATURE_STATE_COUNT";
	}
	return "";
}


const static std::string shader_stock_fs_base =  R"code(
// The base function.
void main()
{
	vec4 baseColor = calcBaseColor();
	vec4 color = calcColor(baseColor);
	color.a = calcAlpha(baseColor);
	FRAG_COLOR = color;"
}
)code";

const static std::string shader_stock_fs_uniforms =  R"code(
// The uniforms for all stock fragment shaders.
#if __VERSION__ >= 130
	#define IN in
	#define OUT out
	#define TEXTURE2D texture
	out vec4 FRAG_COLOR;
#else
	#define IN varying
	#define OUT varying
	#define FRAG_COLOR gl_FragColor
	#define TEXTURE2D texture2D
#endif

IN vec4 clr;
IN vec2 tex;
IN vec4 pos_es;
IN vec3 nrm_es;
uniform sampler2D cmap;

struct Light
{
	vec4 pos;
	vec4 ambi; // ambi.w == spot_exp
	vec4 diff; // diff.w == spec.w == spot_cuttoff
	vec4 spec;
	vec4 atten;
};

struct Material
{
	 vec4 emiss;
	float shininess;
};
uniform Light light[8];

)code";



class Light
{
public:
	/** Ambient color of this light source.*/
	glm::vec4 ambient;
	/** Diffuse color of this light source.*/
	glm::vec4 diffuse;
	/** Specular color of this light source.*/
	glm::vec4 specular;

	/** Eye space position of this light source.*/
	glm::vec4 position;
	/** Direction of the beam starting at position.*/
	glm::vec4 spot_direction;
	/** Focus of the spot.*/
	//	float spot_exponent; <- is ambient.w
	/** Angle between spot direction and boundary*/
	//	float spot_cutoff; <- is diffuse.w


	/** The attenuation of this light source.
		.x - Constant attenuation
		.y - Linear attenuation
		.z - Quadratic attenuation
		.w - A constant attenuation factor
	*/
	glm::vec4 attenuation_factors;
	Light()
	{
		ambient = glm::vec4(0,0,0,0);
		diffuse = specular = glm::vec4(1,1,1,180);
		position = glm::vec4(0,0,0,1);
		spot_direction = glm::vec4(0,0,-1,0);
		attenuation_factors = glm::vec4(1,0,0,0);
	}
	~Light(){}
	/** Sets ambient color */
	void setAmbientColor(const glm::vec3& a)
	{
		ambient.r=a.r;
		ambient.g=a.g;
		ambient.b=a.b;
	}
	/** Sets specular color */
	void setSpecularColor(const glm::vec3& s)
	{
		specular.r = s.r;
		specular.g = s.g;
		specular.b = s.b;
	}
	/** Sets diffuse color */
	void setDiffuseColor(const glm::vec3& d)
	{
		diffuse.r = d.r;
		diffuse.g = d.g;
		diffuse.b = d.b;
	}
	/** Sets position of the light.
		 Note: if p.w == 0 the light source is a directional one.
		 The position then determines the direction the light comes from.
	*/
	void setPosition(const glm::vec4& p){position = p;}
	/** Sets spot direction*/
	void setSpotDirection(const glm::vec4& sd){spot_direction = sd;}
	/** Sets spot cutoff*/
	void setSpotCutoff(const float& spot_co){diffuse.w = spot_co;}
	/** Sets spot exponent*/
	void setSpotExponent(const float& spot_ex){ambient.w = spot_ex;}
	/** Sets attenuation values for constant, linear and quadratic, and w
		af.w/(af.x+af.y*d+af.z*d*d). d is the distance to the light source	*/
	void setAttenuationFactors(const glm::vec4& af){attenuation_factors = af;}

};



class ShaderWrap
{
	GLuint m_handle;
	std::map<int, int> m_uniformID2location;
public:
	ShaderWrap(GLuint handle);
	~ShaderWrap();
	void learnUniform(const std::string& name, int id);
	int getUniformLocation(int id);
	const GLuint& handle()const {return m_handle;}
};




//#define ___currMat m_matrix[m_matrixMode].top()
class StateSimulator
{
protected:
	std::stack<glm::mat4> m_matrix[MATRIXMODE_COUNT];
	MatrixMode m_matrixMode;

	ShaderWrap* m_stockshaders[NonStockShader];
	ShaderWrap* m_current_program;

	void setUniform(int id, const glm::mat4& mat);
	unsigned int m_features;

	bool m_dirty_lights[FEATURE_STATE_COUNT];
	bool m_dirty_mats[FEATURE_STATE_COUNT];

	void m_set_dirty_lights()
	{
		for(int i = 0 ; i<FEATURE_STATE_COUNT;i++)
			m_dirty_lights[i] = true;
		m_dirty_lights[m_features] = false;
	}
	void m_set_dirty_mats()
	{
		for(int i =0 ; i<FEATURE_STATE_COUNT; i++)
		{
			m_dirty_mats[i] = true;
		}
		m_dirty_mats[m_features] = false;
	}

	/** Will upload the current matrix.

	*/
	void m_upload_matrix();


	Light m_light[8];

//	std::string loadFile(std::string path);
//	bool compileShader(GLuint shader);
	public:
	StateSimulator(const std::string &path_to_stock_shaders = "resources/");
	~StateSimulator();
	void enable (Feature f);
	void disable(Feature f);

	void pushMatrix();
	void popMatrix();
	void loadIdentity();
	void loadMatrix(const glm::mat4& m);
	void matrixMode(MatrixMode m);
	void translate(float x, float y, float z);
	void translate(const glm::vec3& v);
	void rotate(float a, float x, float y, float z);
	void scale(float x, float y, float z);
	void lookAt(
		float ex,
		float ey,
		float ez,
		float tx,
		float ty,
		float tz,
		float ux,
		float uy,
		float uz);
	void ortho(float l, float r, float b, float t, float n, float f);
	void frustum(float l, float r, float b, float t, float n, float f);
	void perspective(float fovy, float ar, float n, float f);
	void useShader(ShaderWrap* sw, bool first_time =false );

	void setLightPosition(int light, const glm::vec4& p);
	void setLightAmbientColor(int light, const glm::vec3 c);
	void setLightDiffuseColor(int light, const glm::vec3 c);
	void setLightSpecularColor(int light, const glm::vec3 c);
	void setLightSpotDirection(int light, const glm::vec3 d);
	void setLightSpotCutOff(int light, const float& co);
	void setLightSpotExponent(int light, const float& ex);
	void setLightAttenuationFactors(int lignt, const glm::vec4& att);
	ShaderWrap* loadShaderFormFiles(const std::string& vs_path, const std::string& fs_path);
	void useCurrentShader();

};
}

#endif //USING_OFL_OGL_STATE_H
#ifndef USING_OFL_STRU_H
 #define USING_OFL_STRU_H
 
#endif //USING_OFL_STRU_H
#ifndef USING_OFL_VMATH_H
 #define USING_OFL_VMATH_H
 
#endif //USING_OFL_VMATH_H
#ifdef OFL_IMPLEMENTATION
#include <vector>
#include <string>
#include <fstream>


namespace ofl {


bool compileShader(GLuint shader)
{
	glCompileShader(shader);
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		printf("[ERR] Compiling shader failed: \n\t%s\n",errorLog.data());
		return false;
	}
	return true;
}

bool linkShader(GLuint shader_program)
{
	glLinkProgram(shader_program);
	GLint wasLinked ;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &wasLinked);
	if(wasLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(shader_program, maxLength, &maxLength, &errorLog[0]);
		printf("[ERR] Linking shader failed: \n\t%s\n",errorLog.data());
		return false;
	}
	return true;
}

std::string loadFile(const std::string& path)
{
	std::ifstream t(path);
	if(!t.is_open())
	{
		fprintf(stderr,"[ERR] Loading file \"%s\" failed!\n",path.c_str());
		return "";
	}
	std::string str = "";
	std::string line = "";
	while(std::getline(t,line))
	{
		str+= line + "\n";
	}
	return str;
}


ShaderWrap::ShaderWrap(GLuint handle)
{
	m_handle = handle;
}
ShaderWrap::~ShaderWrap()
{
	if(m_handle)
		glDeleteProgram(m_handle);
}

void ShaderWrap::learnUniform(const std::string& name, int id)
{
	m_uniformID2location[id]=glGetUniformLocation(m_handle, name.c_str());
	//	printf("s:%d,%s\t%d\t%d\n",m_handle, name.c_str(), m_uniformID2location[id],id);
}

int ShaderWrap::getUniformLocation(int id)
{
	if(m_uniformID2location.find(id) == m_uniformID2location.end())
	{
		return -2;
	}
	else
		return m_uniformID2location[id];
}

#define ___currMat m_matrix[m_matrixMode].top()
StateSimulator::~StateSimulator()
{
	for(int i = 0 ; i < NonStockShader;i++)
	{
		if(	m_stockshaders[i])
		{
			delete m_stockshaders[i];
			m_stockshaders[i] = nullptr;
		}
	}
}

void StateSimulator::enable (Feature f)
{
	printf("[ifo] %s: ON\n",to_string(f).c_str());
	
	if(m_features & f)
		return;
	m_features |= f;
	if(m_features < NonStockShader)
	{
		this->useShader(m_stockshaders[m_features]);
	}
}
void StateSimulator::disable(Feature f)
{
	printf("[ifo] %s: OFF\n",to_string(f).c_str());
	if(!(m_features & f))
		return;
	unsigned int mask = (unsigned int) -1;
	mask ^= f;
	m_features &= mask;
	if(m_features < NonStockShader)
	{
		this->useShader(m_stockshaders[m_features]);
	}
}

StateSimulator::StateSimulator(const std::string& path_to_stock_shaders)
{
	m_features = 0;
	m_matrixMode = MODELVIEW;
	for(int i = 0; i<MATRIXMODE_COUNT;i++)
	{
		m_matrix[i].push(glm::mat4(1));
		m_dirty_mats[i] = true;
	}
	for(int i = 0 ; i < NonStockShader;i++)
	{
		m_stockshaders[i] = nullptr;
	}
	for(int i = 0 ; i < NonStockShader+1;i++)
	{
		m_dirty_lights[i] = true;
	}
	// create the vertex shader for every stock program
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	std::string vshader_src = loadFile(path_to_stock_shaders+"stock.glsl.vs");
	
	const char* cstr = vshader_src.c_str();
	int len = vshader_src.length();
	glShaderSource(vshader,1,&cstr, &len);
	if(! compileShader(vshader))
	{
		glDeleteShader(vshader);
		return;
	}
	
	// Load all 4 stock shaders. They all share the same vertex stage
	// so we will load 4 different fragment-shaders and combine them with
	// the vertex-shader to 4 different shader-programms
	//
	for(int i = 0; i < NonStockShader ; i++)
	{
		// generate Fragment shaders
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
		// load the source code from a file
		std::string fshader_src = loadFile(path_to_stock_shaders+"stock_"+std::to_string(i)+".glsl.fs");
		cstr = fshader_src.c_str();
		len = fshader_src.length();
		// attach the code to the shader object
		glShaderSource(fshader,1,&cstr,&len);
		// compile and check for errors
		printf("[ifo] Compiling fragment shader for mode %d...\n",i);
		if(!compileShader(fshader))
		{
			printf("CODE:\n%s\n",cstr);
			glDeleteShader(fshader);
			return;
		}
		else
		{
			printf("[ifo] ... done!\n");
		}
		// generate the program.
		GLuint prog = glCreateProgram();
		// add vertex and fragment shader
		glAttachShader(prog,vshader);
		glAttachShader(prog,fshader);
		// bind the attribute locations. This is due to backwards
		// compatibility, with newer GL/GLSL versions you can define the
		// location in the shader.
		glBindAttribLocation(prog, ALOC_POSITION, NAME_POS_ATTR);
		glBindAttribLocation(prog, ALOC_NORMAL, NAME_NRM_ATTR);
		glBindAttribLocation(prog, ALOC_COLOR, NAME_CLR_ATTR);
		glBindAttribLocation(prog, ALOC_TEXCOORD, NAME_TEX_ATTR);
		// link the program
		linkShader(prog);
		// wrap the program and store it for later use
		this->m_stockshaders[i] = new ShaderWrap(prog);
		// use it for the first time to learn the uniform mappings
		this->useShader(m_stockshaders[i],true);
		// delete the fragment-shader we don't need it anymore
		glDeleteShader(fshader);
	}
	// Use stock-shader 0 (no lighting, no texturing)
	this->useShader(m_stockshaders[0]);
	// finally delete the vertex-shader it has served us well =)
	glDeleteShader(vshader);
}


void StateSimulator::m_upload_matrix()
{
	glUniformMatrix4fv(
				m_current_program->getUniformLocation(m_matrixMode),
				1,
				GL_FALSE,
				glm::value_ptr(___currMat));
	if(m_matrixMode == MODELVIEW)
	{
		auto normal_mat = glm::transpose(glm::inverse(m_matrix[MODELVIEW].top()));
		
		glUniformMatrix4fv(
					m_current_program->getUniformLocation(ID_NMM),
					1,
					GL_FALSE,
					glm::value_ptr(normal_mat));
	}
}
void StateSimulator::pushMatrix()
{
	m_matrix[m_matrixMode].push(m_matrix[m_matrixMode].top());
}
void StateSimulator::popMatrix()
{
	if(m_matrix[m_matrixMode].size() >1)
	{
		m_matrix[m_matrixMode].pop();
		m_upload_matrix();
		m_set_dirty_mats();
	}
}
void StateSimulator::loadIdentity()
{
	m_matrix[m_matrixMode].top() = glm::mat4(1);
	m_upload_matrix();
	m_set_dirty_mats();
}

void StateSimulator::loadMatrix(const mat4 &m)
{
	m_matrix[m_matrixMode].top() = m;
	m_upload_matrix();
	m_set_dirty_mats();
}

void StateSimulator::matrixMode(MatrixMode m)
{
	m_matrixMode = m;
}
void StateSimulator::translate(float x, float y, float z)
{
	//	auto m = glm::translate(glm::mat4(1.0f),glm::vec3(x,y,z));
	
	___currMat = glm::translate(___currMat,glm::vec3(x,y,z));
	m_upload_matrix();
	m_set_dirty_mats();
}

void StateSimulator::translate(const glm::vec3 &v)
{
	___currMat = glm::translate(___currMat,v);
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::rotate(float a, float x, float y, float z)
{
	___currMat = glm::rotate(___currMat,a,glm::vec3(x,y,z));
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::scale(float x, float y, float z)
{
	___currMat = glm::scale(___currMat,glm::vec3(x,y,z));
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::lookAt(
		float ex,
		float ey,
		float ez,
		float tx,
		float ty,
		float tz,
		float ux,
		float uy,
		float uz
		)
{
	___currMat *= glm::lookAt(
				glm::vec3(ex,ey,ez),
				glm::vec3(tx,ty,tz),
				glm::vec3(ux,uy,uz));
	m_upload_matrix();
	m_set_dirty_mats();
}

void StateSimulator::ortho(
		float l,
		float r,
		float b,
		float t,
		float n,
		float f)
{
	___currMat *= glm::ortho(l,r,b,t,n,f);
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::frustum(
		float l,
		float r,
		float b,
		float t,
		float n,
		float f)
{
	___currMat *= glm::frustum(l,r,b,t,n,f);
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::perspective(float fovy, float ar, float n, float f)
{
	___currMat *= glm::perspective(fovy,ar,n,f);
	m_upload_matrix();
	m_set_dirty_mats();
}

#define ID_LIGHTS 10
#define ID_LIGHT_POS(x)   (0+x*5+ID_LIGHTS)
#define ID_LIGHT_AMBI(x)  (1+x*5+ID_LIGHTS)
#define ID_LIGHT_DIFF(x)  (2+x*5+ID_LIGHTS)
#define ID_LIGHT_SPEC(x)  (3+x*5+ID_LIGHTS)
#define ID_LIGHT_ATTEN(x) (4+x*5+ID_LIGHTS)

#define NAME_LIGHT_POS(x)   "light["+std::to_string(x)+"].pos"
#define NAME_LIGHT_AMBI(x)  "light["+std::to_string(x)+"].ambi"
#define NAME_LIGHT_DIFF(x)  "light["+std::to_string(x)+"].diff"
#define NAME_LIGHT_SPEC(x)  "light["+std::to_string(x)+"].spec"
#define NAME_LIGHT_ATTEN(x) "light["+std::to_string(x)+"].atten"
void setLightUniforms(const Light& l, int i, ShaderWrap* sw)
{
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_POS(i))  ,1,glm::value_ptr(l.position));
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_AMBI(i)) ,1,glm::value_ptr(l.ambient));
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_DIFF(i)) ,1,glm::value_ptr(l.diffuse));
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_SPEC(i)) ,1,glm::value_ptr(l.specular));
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_ATTEN(i)),1,glm::value_ptr(l.attenuation_factors));
	
}
void StateSimulator::useShader(ShaderWrap* sw, bool first_time )
{
	glUseProgram(sw->handle());
	if(first_time)
	{
		sw->learnUniform(NAME_MVM,ID_MVM);
		sw->learnUniform(NAME_PJM,ID_PJM);
		sw->learnUniform(NAME_CMAP,ID_CMAP);
		sw->learnUniform(NAME_NMM,ID_NMM);
		for(int i = 0 ; i<8;i++)
		{
			sw->learnUniform(NAME_LIGHT_POS(i),ID_LIGHT_POS(i));
			sw->learnUniform(NAME_LIGHT_AMBI(i),ID_LIGHT_AMBI(i));
			sw->learnUniform(NAME_LIGHT_DIFF(i),ID_LIGHT_DIFF(i));
			sw->learnUniform(NAME_LIGHT_SPEC(i),ID_LIGHT_SPEC(i));
			sw->learnUniform(NAME_LIGHT_ATTEN(i),ID_LIGHT_ATTEN(i));
		}
	}
	
	m_current_program = sw;
	if(m_dirty_mats[m_features])
	{
		for(int i  = 0 ; i<MATRIXMODE_COUNT;i++)
		{
			glUniformMatrix4fv(
						m_current_program->getUniformLocation(i),
						1,
						GL_FALSE,
						glm::value_ptr(m_matrix[i].top()));
			if(i == MODELVIEW)
			{
				auto normal_mat = glm::transpose(glm::inverse(m_matrix[MODELVIEW].top()));
				glUniformMatrix4fv(
							m_current_program->getUniformLocation(ID_NMM),
							1,
							GL_FALSE,
							glm::value_ptr(normal_mat));
			}
		}
		m_dirty_mats[m_features] = false;
	}
	if((m_features & Lighting) && m_dirty_lights[m_features])
	{
		for(int i = 0 ; i< 8;i++)
		{
			setLightUniforms(m_light[i],i,m_current_program);
		}
		m_dirty_lights[m_features] = false;
	}
}

void StateSimulator::setLightPosition(int light, const glm::vec4& p)
{
	glm::vec4 p_es = m_matrix[MODELVIEW].top()*p;
	m_light[light].setPosition(p_es);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
}
void StateSimulator::setLightAmbientColor(int light, const glm::vec3 c)
{
	m_light[light].setAmbientColor(c);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
}
void StateSimulator::setLightDiffuseColor(int light, const glm::vec3 c)
{
	m_light[light].setDiffuseColor(c);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
	
}
void StateSimulator::setLightSpecularColor(int light, const glm::vec3 c)
{
	m_light[light].setSpecularColor(c);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
}
void StateSimulator::setLightSpotDirection(int light, const glm::vec3 d)
{
	m_light[light].setSpotDirection(glm::vec4(d,0));
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
	
}
void StateSimulator::setLightSpotCutOff(int light, const float& co)
{
	m_light[light].setSpotCutoff(co);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
	
}
void StateSimulator::setLightSpotExponent(int light, const float& ex)
{
	m_light[light].setSpotExponent(ex);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
	
}
void StateSimulator::setLightAttenuationFactors(int light, const glm::vec4& att)
{
	m_light[light].setAttenuationFactors(att);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
}

ShaderWrap* StateSimulator::loadShaderFormFiles(const std::string &vs_path, const std::string &fs_path)
{
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	std::string vshader_src = loadFile(vs_path);
	
	const char* cstr = vshader_src.c_str();
	int len = vshader_src.length();
	glShaderSource(vshader,1,&cstr, &len);
	if(! compileShader(vshader))
	{
		glDeleteShader(vshader);
		return nullptr;
	}
	
	// generate Fragment shaders
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	// load the source code from a file
	std::string fshader_src = loadFile(fs_path);
	cstr = fshader_src.c_str();
	len = fshader_src.length();
	// attach the code to the shader object
	glShaderSource(fshader,1,&cstr,&len);
	// compile and check for errors
	if(!compileShader(fshader))
	{
		printf("CODE:\n%s\n",cstr);
		glDeleteShader(fshader);
		return nullptr;
	}
	else
	{
		// generate the program.
		GLuint prog = glCreateProgram();
		// add vertex and fragment shader
		glAttachShader(prog,vshader);
		glAttachShader(prog,fshader);
		// bind the attribute locations. This is due to backwards
		// compatibility, with newer GL/GLSL versions you can define the
		// location in the shader.
		glBindAttribLocation(prog, ALOC_POSITION, NAME_POS_ATTR);
		glBindAttribLocation(prog, ALOC_NORMAL, NAME_NRM_ATTR);
		glBindAttribLocation(prog, ALOC_COLOR, NAME_CLR_ATTR);
		glBindAttribLocation(prog, ALOC_TEXCOORD, NAME_TEX_ATTR);
		// link the program
		linkShader(prog);
		// use it for the first time to learn the uniform mappings
		ShaderWrap* result= new ShaderWrap(prog);
		this->useShader(result,true);
		// delete the fragment-shader we don't need it anymore
		glDeleteShader(fshader);
		return result;
	}
}
void StateSimulator::useCurrentShader()
{
	useShader(m_current_program);
}
}

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

vec4 cross(const vec4& a, const vec4& b)
{
	return vec4(a.y * b.z - b.y * a.z,
				a.z * b.x - b.z * a.x,
				a.x * b.y - b.x * a.y,
				0);
}



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
#endif
