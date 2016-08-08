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

//OGL_GEO: OpenGL Geometry
//================================================================================
//A class representing vertex Attribute data. You can generate a Geometry
//object from a VertexData-Object, so they are obviously connected.
//
//Usage
//--------------------------------------------------------------------------------
//You need to specify an OpenGL header in as `OFL_GL_HEADER`. for example:
//
//```cpp
//	#define OFL_GL_HEADER <glad/glad.h>  
//	#define OFL_GL_HEADER <glew.h>
//```
//
//This module also uses GLM ... sorry I was too lazy to implement all
//these functions, and its not feasible anyway.
//
//API
//--------------------------------------------------------------------------------
//You can pass the data on construction or vie the method uploadData(...).
//The	data is then uploaeded into OpenGL buffer objets and a corresponding
//VAO is created.
//Call the draw() method to provoke a glDrawElements() call.
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

//OGL_WIN: OpenGL Window
//================================================================================
//This module will - one day - help to create an OpenGL context, and give access
//to mouse and keyboard events. For now it is a simple GLFW wrapper, but the
//interface shall be the same in the finished library.
//

--------------------------------------------------------------------------------

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

--------------------------------------------------------------------------------

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

--------------------------------------------------------------------------------

//VDMAN: VertexData Manufacturer
//================================================================================
//This class provides functionality to create VertexData, the same intuitive
//way you might do it in OpenGL1.x. vertex per vertex in a given primitive
//mode.  
//It can also produce some simple geometric shapes:  
//
//- Plane
//- Box
//- Cone
//- UV-Sphere
//- Cylinder
//- Disk
//- Coordinate system
//
//Usage
//--------------------------------------------------------------------------------
//
//This tool creates VertexData, so the ofl_vd oflib is needed.
//
//API
//--------------------------------------------------------------------------------
//It is very easy, with `begin(PRIMITIVE)` you start to constuct a new
//VertexData object. With `color(...)`, `texCoord(...)`, and `normal(...)` you can
//modify the data used for these attributes. With a call of `vertex(...)` the
//vertex is finished and another is started. The state you set with `color(...)`
//etc. is not changed with a call of `vertex(...)`.
//
//Calling `finish()` returns the VertexData object and the Manufactuerer is
//ready to construct a new one.
//
//
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

//VRPV: Virtual Reality Projection & Viewmatrix
//================================================================================
//The toolkit consists of two main components: Screens and ScreenArrangements.
//Screens are the data structures you are interested in. A Screen represents a
//rectengular area in world space, on which you want to render. (This could be
//one side of a cave, one display in a display array, a power wall, etc.).
//
//A ScreenArrangement is a collection of screens. ScreenArangements can be
//loaded from a file, which is handy if you want to change the screen
//arragement without recompiling your program.
//
//Usage
//--------------------------------------------------------------------------------
//If you use glm, make sure, that glm.hpp is included before you include vrpv.h
//so that VRPV uses glm. If you don't use glm (which is fine) vrpv uses its own
//glm compatible, rudimentary mat4 and vec4 implementation.
//
//
//API
//--------------------------------------------------------------------------------
//You can create a ScreenArrangement and load screens by these two lines of
//code:
//
//```
//	ScreenArrangement sa;
//	sa.loadScreens("path/to/screens/file")
//```
//
//You can access a Screen using the `getScreen(...)` methods you can either
//access it via its name, specified in the screen-file or via its id
//(with id in [0,sa.countScreens()]):
//```
//	Screen* s = sa.getScreen(0); // by id
//	Screen*  s = sa.getScreen("screen0"); // by name
//```
//Now you can calculate projection and  view-matrices for this screen using
//the calculate_projection_and_view(...) method:
//
//```
//	mat4 projection,view;
//	vec4 eye_position;
//	float near;
//	float far;
//	s->calculate_projection_and_view(eye_position, near, far, projection, view);
//```
//Note: The eye_position must be in the same space as the screen corners in
//the screen-file.
//
//File-formats
//--------------------------------------------------------------------------------
//The screen files contain one line per screen. Each line constist of the
//name of the screen, and the position of three corners of the screen:
//bottom-left, bottom-right, top-left. You can use vec3 or vec4
//(when using vec4 make shure the w-component is 1), however when you saver the
//ScreenArrangement, vec4s will be used.
//Example:
//```
//	# <- the '#' marks a line as comment.
//	screen0 (0.0,0.0,0.0,1.0) (1.0,0.0,0.0,1.0) (0.0,1.0,0.0,1.0)
//	screen1 (0.0,1.0,0.0) (1.0,1.0,0.0) (0.0,2.0,0.0)
//```

--------------------------------------------------------------------------------

<end_doc>
#endif 
#ifndef USING_OFL_ALL_H
 #define USING_OFL_ALL_H
 
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
typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef glm::vec2 vec2;
typedef glm::mat4 mat4;
using namespace glm;
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
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

#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif

#include OFL_GL_HEADER



namespace ofl
{

#ifndef ALOC_POSITION
#define ALOC_POSITION 0
#define ALOC_NORMAL 1
#define ALOC_COLOR 2
#define ALOC_TEXCOORD 3
#define ALOC_TANGENT 4
#endif

/**
 * @brief The Geometry class represents a VAO, with an associated IBO and VBO.
 * It is due to ATIs special interpretation to the reference counting on buffer
 * objects in the case of an IBO, that we have to store the IBO, so I store the
 * VBO as well. It also stores the primitive type and the vertex count.
 */
class Geometry
{
protected:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_primitive;
	GLsizei m_vertice_count;

public:
	/**
	 * @brief Geometry creates a geometry from VertexData.
	 * Will create a VBO and a VAO uploading the data from vp.
	 * The attribute locations will be set to:
	 * 	position              0  (= ALOC_POSITION )
	 * 	normal                1  (= ALOC_NORMAL   )
	 * 	color                 2	 (= ALOC_TEXCOORD )
	 * 	texture coordinates   3  (= ALOC_COLOR    )
	 * 	tangent               4  (= ALOC_TANGENT  )
	 * @param vd The vertex data.
	 * @param destroy_vd If set true vd will be freed.
	 */
	Geometry(ofl::VertexData* ofl, bool destroy_vd = false);
	Geometry();

	/**
	 * @brief uploadData updates all the data storen with data from vp
	 * @param vp Source of the new data.
	 */
	void uploadData(ofl::VertexData* vp);

	/** Destroys the structure and releases all memory allocated on the GPU-
	 * and CPUside.
	 */
	~Geometry();

	/* this geometry draws itself.*/
	/**
	 * @brief draw This geometry draws itself.
	 * If patchsize is set, it will draw with GL_PATCH_VERTICES as primitive,
	 * and set the patchsize before drawing.
	 * @param patchsize The patchsize you choose or 0 for the stored primitive
	 * mode.
	 */
	void draw(int patchsize = 0)
	{
		if (m_vao)
		{
			glBindVertexArray(m_vao);
			if (patchsize > 0)
			{
				glPatchParameteri(GL_PATCH_VERTICES, patchsize);
				glDrawElements(GL_PATCHES, m_vertice_count,
							   GL_UNSIGNED_INT, nullptr);
			}
			else
			{
				glDrawElements(m_primitive, m_vertice_count,
							   GL_UNSIGNED_INT, nullptr);
			}
		}
		else
		{

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

			const size_t vertex_size = sizeof(ofl::Vertex);
			const ofl::Vertex v;


#define addr_diff(a,b) ((void*)((char*)a-(char*) b))
		glVertexAttribPointer(ALOC_POSITION, 3, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.pos()),&v));
		glEnableVertexAttribArray(ALOC_POSITION);

		glVertexAttribPointer(ALOC_NORMAL, 3, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.nrm()),&v));
		glEnableVertexAttribArray(ALOC_NORMAL);

		glVertexAttribPointer(ALOC_COLOR, 4, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.clr()),&v));
		glEnableVertexAttribArray(ALOC_COLOR);

		glVertexAttribPointer(ALOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.tex()),&v));
		glEnableVertexAttribArray(ALOC_TEXCOORD);

		glVertexAttribPointer(ALOC_TANGENT, 3, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.tan()),&v));
		glEnableVertexAttribArray(ALOC_TANGENT);
#undef addr_diff
			glDrawElements(
						m_primitive,
						m_vertice_count,
						GL_UNSIGNED_INT,
						nullptr);
		}
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

	default:
		break;
	}
	return "";
}






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

	void useCurrentShader();
};
}
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>


namespace ofl {


/**
 * @brief The Screen class is basically an area in world space you want to
 * render to. This might be a powerwall or the side of a cave or a segment
 * in a segmented display.
 */
class Screen
{
	vec4 m_corners[3];
	mat4 m_wall_space;
	mat4 m_inv_wall_space;

public:

	enum CORNER
	{
		BL=0,
		BR,
		TL
	} ;

	vec4 corner(const int i) const;
	/**
	 * @brief Screen Constructor. Consumes three corners of the Screen area
	 * The last corner is implied.
	 * @param blc Bottom left corner.
	 * @param trc Top right corner.
	 * @param tlc Top left corner.
	 */
	Screen(const vec4 blc,const vec4 brc, const vec4& tlc);

	/**
	 * @brief calculate_projection_and_view Calculates a projection- and a view
	 * matrix for a given eye position.
	 * Note for stereo rendering you have to call this twice, once per eye.
	 * @param eye The postition of the eye.
	 * @param near The near distance.
	 * @param far The far distance.
	 * @param projection output where the projection matrix will be written to
	 * @param view output where the view matrix will be written to
	 */
	void calculate_projection_and_view(
			const vec4& eye,
			const float near,
			const float far,
			mat4& projection_matrix,
			mat4& view_matrix) const;
};

/**
 * @brief The ScreenArrangement class groups Screens together. This might be use
 * full for a cave or a multi-segment-display. Each Screen is accessible via
 * its name or its id, or you just store a pointer to the screen you are
 * interested in. In that case this class is only usefull for loading and
 * storing ScreenArrangements
 */
class ScreenArrangement
{
	std::vector<Screen> m_screens;
	std::map<std::string,Screen*> m_by_name;
public:
	/**
	 * @brief addScreen Adds a Screen to the arrangement.
	 * @param scrn The screen you wish to add (will be copied).
	 * @param name The name of the screen.
	 * @return a pointer to the interal screen, which can be accessed as long
	 * as this Object exists.
	 */
	Screen* addScreen(const Screen scrn, const std::string& name);

	/**
	 * @brief getScreen Provides access to a screen via its id. If there is
	 * no screen with this id a nullptr is returned.
	 * @param id The id of the screen.
	 * @return The Screen you asked for or nullptr if it does not exist.
	 */
	const Screen* getScreen(const int id)const;

	/**
	 * @brief getScreen Provides access to a screen via its name. If there is
	 * no screen with this name a nullptr is returned.
	 * @param name name of the screen.
	 * @return	The Screen you asked for or nullptr if it does not exist.
	 */
	const Screen* getScreen(const std::string& name);

	/**
	 * @brief count_screens Returns the number of screens in this
	 * ScreenArrangement
	 * @return #Screens
	 */
	unsigned int countScreens() const;

	/**
	 * @brief addScreens Adds screens from a file to this arrangement.
	 * The format of the file should be:
	 * <name0> (blx,bly,blz,blw) (brx,bry,brz,brw) (tlx,tly,tlz,tlw)
	 * <name1> (blx,bly,blz,blw) (brx,bry,brz,brw) (tlx,tly,tlz,tlw)
	 * # ...
	 * # a line starting with # is a comment line.
	 * @param path to the file.
	 * @return true if everything went well. false if there was a problem with
	 * the file.
	 */
	bool loadScreens(const std::string& path);

	/**
	 * @brief saveScreens saves this ScreenArrangement to the disk
	 * @param path where you want to store this.
	 * @return true if ecerything went well, false if there was a problem with
	 * the file.
	 */
	bool saveScreens(const std::string& path);
};
}

#include <string>
namespace ofl {


enum WindowEvents
{
	WIN_CLOSE,
	WIN_ICONIFY,
	WIN_RESTORE,
	WIN_GOT_FOCUS,
	WIN_LOST_FOCUS
};


enum OpenGLAPI
{
	OPEN_GL,
	OPEN_GL_ES
};
enum OpenGLProfile
{
	ANY,
	COMPATIBILITY,
	CORE
};
class WindowPreferences
{
public:
	std::string title;
	bool resizable;
	bool visible;
	bool decorated;
	bool focused;
	bool fullscreen;
	int width;
	int height;
	bool use_monitor_properties; // will use the monitors settings..
	int red_bits;
	int green_bits;
	int blue_bits;
	int alpha_bits;
	int depth_bits;
	int stencil_bits;
	int accum_red_bits;
	int accum_green_bits;
	int accum_blue_bits;
	int accum_alpha_bits;
	int aux_buffers;
	int samples;
	int refresh_rate;
	// ... until here
	bool stereo;
	bool srgb;
	bool double_buffer;
	OpenGLAPI opengl_api;
	int opengl_version_major;
	int opengl_version_minor;
	bool debug_context;
	OpenGLProfile opengl_profile;

	WindowPreferences()
	{
		title = "OpenGL-Window";
		width = 640;
		height = 480;
		resizable = true;
		visible = true;
		decorated = true;
		focused = true;
		fullscreen = false;
		use_monitor_properties = true;
		red_bits = 8;
		green_bits = 8;
		blue_bits = 8;
		alpha_bits = 8;
		depth_bits = 24;
		stencil_bits = 8;
		accum_red_bits = 0;
		accum_green_bits= 0;
		accum_blue_bits= 0;
		accum_alpha_bits= 0;
		aux_buffers= 0;
		samples = 0;
		refresh_rate= 60;
		stereo = false;
		srgb = false ;
		double_buffer = true;
		opengl_api = OPEN_GL;
		opengl_version_major=0;
		opengl_version_minor=0;
		debug_context=false;
		opengl_profile=COMPATIBILITY;
	}
};

class Window
{

public:
	Window(WindowPreferences* /*wp*/){}
	virtual void swapBuffers() = 0;
	virtual void enterRenderLoop() = 0;
	virtual void setTitle(const std::string& title) = 0;
	virtual void setFullscreen(bool fs) = 0;

	virtual void eventKey(int /*key*/, int /*action*/, int /*mods*/, int /*scancode*/){}
	virtual void eventCharacter(unsigned int /*c*/, int /*mods*/){}
	virtual void eventMouseButton(int /*button*/, int /*action*/,int /*mods*/){}
	virtual void eventMouseScroll(double /*xScroll*/, double /*yScroll*/){}
	virtual void eventMouseMove(double /*x*/, double /*y*/){}
	virtual void eventWindow(int /*event*/){}
	virtual void eventWindowPosition(int /*x*/, int /*y*/){}
	virtual void eventWindowSize(int /*w*/, int /*h*/){}

	virtual bool renderAFrame(double /*tslf_s*/)
	{return true;}

	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const  = 0;
};

#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window_GLFW : public Window
{
protected:

	GLFWwindow* m_window;
	double m_tsLastFrame;
	static void error_callback(int error, const char* description)
	{
		fprintf(stderr,"[ERR] %d: %s\n",error,description);
	}

	static void key_callback(
			GLFWwindow* win,
			int key,
			int scancode,
			int action,
			int mods)
	{
		Window_GLFW* app = (Window_GLFW*) glfwGetWindowUserPointer(win);
		app->eventKey(key,action,mods,scancode);
	}

	static void charmods_callback(GLFWwindow* win, unsigned int c, int mods)
	{
		Window_GLFW* app=(Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventCharacter(c,mods);
	}

	static void cursor_position_callback(GLFWwindow* win, double xpos, double ypos)
	{
		Window_GLFW* app=(Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventMouseMove(xpos,ypos);
	}
	static void cursor_enter_callback(GLFWwindow* /*win*/, int /*entered*/)
	{
		//Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
	}
	static void mouse_button_callback(
			GLFWwindow* win,
			int button,
			int action,
			int mods)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventMouseButton(button,action,mods);
	}

	static void scroll_callback(GLFWwindow* win, double xoff, double yoff)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventMouseScroll(xoff,yoff);
	}


	static void win_resize_callback(GLFWwindow * win, int w , int h)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventWindowSize(w,h);
	}
	static void win_position_callback(GLFWwindow * win, int x , int y)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventWindowPosition(x,y);
	}
	static void win_iconify_callback(GLFWwindow * win, int iconified)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		if(iconified)
			app->eventWindow(WIN_ICONIFY);
		else
			app->eventWindow(WIN_RESTORE);
	}
	static void win_close_callback(GLFWwindow * win)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventWindow(WIN_CLOSE);
	}
	static void win_focus_callback(GLFWwindow * win, int got_focus)
	{
		Window_GLFW* app = (Window_GLFW*)glfwGetWindowUserPointer(win);
		if(got_focus)
			app->eventWindow(WIN_GOT_FOCUS);
		else
			app->eventWindow(WIN_LOST_FOCUS);
	}

	int m_win_res_w;
	int m_win_res_h;
	int m_win_pos_x;
	int m_win_pos_y;

public:

	Window_GLFW(WindowPreferences* wp) : Window(wp)
	{
		m_tsLastFrame = 0;
		if (!glfwInit())
			exit(EXIT_FAILURE);
		glfwSetErrorCallback(error_callback);
		WindowPreferences default_wp;
		if(!wp)
		{
			wp= &default_wp;
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if(wp->use_monitor_properties)
		{

			wp->red_bits = mode->redBits;
			wp->green_bits = mode->greenBits;
			wp->blue_bits = mode->blueBits;
			wp->refresh_rate = mode->refreshRate;
		}

		glfwWindowHint(GLFW_RESIZABLE,wp->resizable);
		glfwWindowHint(GLFW_VISIBLE,wp->visible);
		glfwWindowHint(GLFW_DECORATED,wp->decorated);
		glfwWindowHint(GLFW_FOCUSED,wp->focused);
		glfwWindowHint(GLFW_RED_BITS,wp->red_bits);
		glfwWindowHint(GLFW_GREEN_BITS,wp->green_bits);
		glfwWindowHint(GLFW_BLUE_BITS,wp->blue_bits);
		glfwWindowHint(GLFW_ALPHA_BITS,wp->alpha_bits);
		glfwWindowHint(GLFW_DEPTH_BITS,wp->depth_bits);
		glfwWindowHint(GLFW_STENCIL_BITS,wp->stencil_bits);
		glfwWindowHint(GLFW_ACCUM_RED_BITS,wp->accum_red_bits);
		glfwWindowHint(GLFW_ACCUM_GREEN_BITS,wp->accum_green_bits);
		glfwWindowHint(GLFW_ACCUM_BLUE_BITS,wp->accum_blue_bits);
		glfwWindowHint(GLFW_ACCUM_ALPHA_BITS,wp->accum_alpha_bits);
		glfwWindowHint(GLFW_AUX_BUFFERS,wp->aux_buffers);
		glfwWindowHint(GLFW_SAMPLES,wp->samples);
		glfwWindowHint(GLFW_REFRESH_RATE,wp->refresh_rate);
		glfwWindowHint(GLFW_STEREO,wp->stereo);
		glfwWindowHint(GLFW_SRGB_CAPABLE,wp->srgb);


		glfwWindowHint(GLFW_DOUBLEBUFFER,wp->double_buffer);
		if(wp->opengl_version_major)
		{
			printf("gl version major %d\n",wp->opengl_version_major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,wp->opengl_version_major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,wp->opengl_version_minor);
		}
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,wp->debug_context);
		if(wp->opengl_version_major > 3 && wp->opengl_version_minor > 2)
		{
			if(wp->opengl_profile == ANY)
				glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_ANY_PROFILE);
			else if(wp->opengl_profile == COMPATIBILITY)
				glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);
			else
				glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		}
		if(wp->opengl_api == OPEN_GL)
			glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_API);
		else
			glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_ES_API);

		if(wp->fullscreen)
		{
			m_window = glfwCreateWindow(
						mode->width,
						mode->height,
						wp->title.c_str(),
						monitor,
						nullptr);
		}
		else
		{
			m_window = glfwCreateWindow(
						wp->width,
						wp->height,
						wp->title.c_str(),
						nullptr,
						nullptr);
		}
		if(m_window)
		{
			glfwMakeContextCurrent(m_window);
			glfwSetWindowUserPointer(m_window, this);
			glfwSetKeyCallback(m_window,key_callback);
			glfwSetCharModsCallback(m_window,charmods_callback);
			glfwSetCursorPosCallback(m_window,cursor_position_callback);
			glfwSetMouseButtonCallback(m_window,mouse_button_callback);
			glfwSetScrollCallback(m_window,scroll_callback);
			glfwSetWindowCloseCallback(m_window,win_close_callback);
			glfwSetWindowPosCallback(m_window,win_position_callback);
			glfwSetWindowFocusCallback(m_window,win_focus_callback);
			glfwSetWindowIconifyCallback(m_window,win_iconify_callback);
			glfwSetWindowSizeCallback(m_window, win_resize_callback);

			win_resize_callback(m_window,getWidth(),getHeight());

			if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
			{
				printf("Failed to initialize OpenGL context\n");
			}
		}
	}

	~Window_GLFW()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	virtual void swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	virtual void enterRenderLoop()
	{
		bool contin = true;
		glfwSetTime(0.0);
		while(contin)
		{
			double time = glfwGetTime();
			contin = this->renderAFrame(time-m_tsLastFrame);
			m_tsLastFrame = time;
			if(contin)
			{
				glfwPollEvents();
			}
		}
	}

	virtual void setTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_window,title.c_str());
	}

	virtual void setFullscreen(bool fs)
	{
		if(fs)
		{
			glfwGetWindowSize(m_window,&m_win_res_w,&m_win_res_h);
			glfwGetWindowPos(m_window,&m_win_pos_x,&m_win_pos_y);
			GLFWmonitor* mon = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(mon);
			glfwSetWindowMonitor(m_window,mon,0,0,mode->width,mode->height,mode->refreshRate);
		}
		else
		{
			GLFWmonitor* mon = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(mon);
			glfwSetWindowMonitor(m_window,nullptr,m_win_pos_x,m_win_pos_y,m_win_res_w,m_win_res_h,mode->refreshRate);
		}
	}

	virtual unsigned int getWidth() const
	{
		if(!m_window)
			return 0;
		int w;
		glfwGetWindowSize(m_window,&w,nullptr);
		return (unsigned int) w;
	}
	virtual unsigned int getHeight() const
	{
		if(!m_window)
			return 0;
		int h;
		glfwGetWindowSize(m_window,nullptr,&h);
		return (unsigned int) h;
	}
};

}


#include <vector>
#include <map>
#include <limits>
#include <numeric>
#include <cstdio>
#include <cstring>
#include <limits>
#include <vector>


/** @include vdman.md */

namespace ofl
{

class VertexDataManufacturer : public ofl::VertexDataTools
{

private:


	/**
	 * @brief m_normal_state Current normal state. With any call of .vertex(..)
	 * a vertex is created with this as a normal.
	 */
	vec3 m_normal_state;
	/**
	 * @brief m_color_state Current normal state. With any call of .vertex(..)
	 * a vertex is created with this as a color.
	 */
	vec4 m_color_state;
	/**
	 * @brief m_tex_coord_state Current normal state. With any call of
	 * .vertex(..) a vertex is created with this as a texture coordinate.
	 */
	vec3 tex_coord_state;

	/**
	 * @brief m_input_primitive The primitive mode chosen by begin(..).
	 */
	ofl::Primitive m_input_primitive;

private:
	/** Datastructures supporting the begin/end/finish operations.*/
	// A map to check if given vertex already exists and where it is.
	std::map<ofl::Vertex, unsigned int> vertex_ids;
	ofl::VertexData* current_mesh;

	// A primitive buffer to deal with quads.
	std::vector<unsigned int> primitive_buffer;

	void handlePrimitiveBuffer();

public:

	VertexDataManufacturer();
	~VertexDataManufacturer();

	/**
	 * @brief begin  Will set the GeomtryDataLoader in the corresponding
		OpenGL primitive mode.

		Supported modes are:
		TRIANGLES,
		POINTS,
		LINES,
		QUADS,

		LINE_STRIP,
		TRIANGLE_STRIP,
		QUAD_STRIP

		Note: Geometry created in this way is as dense packed as possible.
		For example...

		begin(TRIANGLES);
		vertex(0,0);
		vertex(1,0);
		vertex(0,1);

		vertex(0,1);
		vertex(1,0);
		vertex(1,1);
		finish();

		... will return the two triangles with four vertices.
	 * @param primitive
	 */
	void begin(ofl::Primitive primitive);



	/**
	 * @brief finish An alternative to end(). This will create a renderable
	 * geometry. After calling finish the manufacturer will then be empty and
	 * reusable. Creating a geometry once and rendering it again and again will
	 * be a lot faster then creating the geometry with begin(..) end() all over
	 * again. Note: You will have to free the geometry by yourself
	 * @return A Vertex data struct containing the vertex information.
	 */
	ofl::VertexData* finish();


	/**
	 * @brief color Sets the current color state
	 * @param color The color you wish to set.
	 */
	void color(const vec4& color);

	/**
	 * @brief color Sets the current color state
	 * @param color The color you wish to set.
	 */
	void color(const vec3& color);

	/**
	 * @brief color Sets the current color state.
	 * @param red The red part.
	 * @param green The green part.
	 * @param blue The blue part.
	 * @param alpha The alpha value.
	 */
	void color(
			const float& red,
			const float& green = 0.0f,
			const float& blue = 0.0f,
			const float& alpha = 1.0f);

	/**
	 * @brief normal Sets the current normal state
	 * @param normal The normal you wish to set.
	 */
	void normal(const vec3& normal);
	void normal(
			const float& x,
			const float& y = 0.0f,
			const float& z = 1.0f);

	/**
	 * @brief texCoord Sets the texture coordinate state
	 * @param tc The texture coordinate you want to set.
	 */
	void texCoord(const vec3& tc);
	void texCoord(const vec2& tc);
	void texCoord(const float& s,
				  const float& t = 0.0f,
				  const float& r = 0.0f);

	/**
	*/
	/**
	 * @brief vertex Creates a vertex using the current state values
	 * for color, normal and texture coordinate.
	 * @param vertex The position of the vertex
	 */
	void vertex(const vec4& vertex);
	void vertex(const vec2& vertex);
	void vertex(const vec3& vertex);
	void vertex(
			const float& x,
			const float& y = 0.0f,
			const float& z = 0.0f,
			const float& w = 1.0f);
	void vertex(const float * vertex);

	void vertex(const ofl::Vertex& vertex);


	/**
	 * @brief createBox Will create all vertices and faces needed to render a
	 * box. Texture coordinates and normals will be correct. The UV layout is:
	 *
	 *     0,1______  ______  0.7,0.9
	 *       |      ||      |
	 *       | BTM  ||	-z  |
	 *  0,0.7|______||______|______ 1,0.6
	 * 0.1,0.6|      |      |      |
	 *        |  -x  |  TOP |   +x |
	 * 0.1,0.3|______|______|______| 1.0.3
	 *               |      |
	 *               |  +z  |
	 *         0.4,0 |______| 0.7,0
	 *
	 *
	 * @param w width of the box
	 * @param h height of the box
	 * @param d depth of the box
	 * @return
	 */
	ofl::VertexData* createBox(
			float w = 1.0f,
			float h = 1.0f,
			float d = 1.0f);


	/** @brief createPlane Will create all vertices and faces needed to render a
	 * plane. Texture coordinates and normals will be correct. The UV layout is
	 *   0,1 ______1,1
	 *      |      |
	 *      |      |
	 *      |______|
	 *   0,0       1,0
	 * The primitive mode will be a TRIANGLE_STRIP.
	 * @param w width of the plane
	 * @param h height of the plane
	 * @param tess_w tesselation in width
	 * @param tess_h tesselation in height
	 * @return
	 */

	ofl::VertexData* createPlane(
			float w = 1.0f,
			float h = 1.0f,
			unsigned int tess_w = 1,
			unsigned int tess_h = 1);


	/**
	 * @brief createCoordinateSystem Will create a colorfull coordinate system
	 * @return The coordinate system
	 */
	ofl::VertexData* createCoordinateSystem();


	/**
	 * @brief createUVSphere Will create all vertices and faces needed to
	 * render a UV-Shpere.  Normals will be set correct!
	 *
	 *     0,1 _____________1,1
	 *        |       /    \|		The top and bottom hemisphere are seperated.
	 *        | TOP> |      |
	 *        |       \____/|
	 *        |/    \       |
	 *        |      |< BOT	|
	 *        |\____/_______|
	 *		 0,0            1,0
	 *
	 * @param radius of the sphere.
	 * @param slices the number of slices between the poles, parallel to the
	 *        equator. This number should allways be odd! otherwise the default
	 *        uv-coordinates wont work.
	 * @param stacks
	 * @return
	 */
	ofl::VertexData* createUVSphere(
			float radius = 1,
			unsigned int slices = 32,
			unsigned int stacks = 16);



	/**
	 * @brief createCylinder Will create all vertices and faces needed to render
	 * a cylinder. Normals and tangents will be set correct. The UV layout is
	 *
	 *         circumference
	 *     0,1 ____________1,1
	 *        |             | h
	 *        |             | e
	 *        |             | i
	 *        |             | g
	 *        |             | h
	 *        |_____________| t
	 *		 0,0       q+.01,0
	 *
	 * @param radius of the cylinder.
	 * @param height of the cylinder.
	 * @param slices segments around the circumference of the cylinder
	 * @param stacks segments along the length of the cylinder
	 * @return
	 */
	ofl::VertexData* createCylinder(
			float radius = 1,
			float height = 1,
			unsigned int slices = 32,
			unsigned int stacks = 1);


	/**
	 * @brief createCone Will create a cone.
	 * Normals and tangents will be set correctly.
	 * The UV layout is
	 *
	 *     0,1 ____________1,1
	 *        | /         \ |    T is  the  top border  with a raduis of 0.01.
	 *        |/     _T    \|    The  outer circle   b  has  a  radius  of 0.5.
	 *        |     / \     |    Note: this  mapping is  far from good, but it
	 *        |     \_/     |    is ok to use it, especially for pointed cones.
	 *        |\          b/|
	 *        |_\_________/_|
	 *		 0,0       q+.01,0
	 *
	 * @param baseRadius the base radius on the XY-plane
	 * @param topRadius the top radius, parallel to the XY-plane
	 * @param height hight of the cone
	 * @param slices segments around the circumference of the cone
	 * @param stacks segments along the length of the cone
	 * @return
	 */
	ofl::VertexData* createCone(
			float baseRadius = 1,
			float topRadius = 0,
			float height = 1,
			unsigned int slices = 32,
			unsigned int stacks = 1);


	/**
	 * @brief createDisk Will create a disk.
	 *  Normals and tangents will be set correctly.
	 * The UV layout is
	 *     0,1 ____________1,1
	 *        | /         \ |    T has the  inner radius/outer radius.
	 *        |/     _T    \|    The  outer circle   b  has  a  radius of 2 Pi.
	 *        |     / \     |
	 *        |     \_/     |
	 *        |\          b/|
	 *        |_\_________/_|
	 *		 0,0       q+.01,0
	 * @param innerRadius the inner radius of the disk.
	 * @param outerRadius the outer radius of the disk.
	 * @param slices segments around the circumference of the disk(Pizza slices)
	 * @param loops loops between inner and outer radius
	 * @return
	 */
	ofl::VertexData* createDisk(
			float innerRadius = 0,
			float outerRadius = 1,
			unsigned int slices = 32,
			unsigned int loops = 1);



	/** Shortcut function to add a vertex with parameters.
		The same operation like:
		.texCoord(t);
		.normal(n);
		.color(c);
		.vertex(p);
	*/
	void addVertex(vec3 p, vec2 t, vec3 n, vec4 c);

	/** Shortcut function to add a whole triangle with parameters.
	The same result like:
	addVertex(p1,t1,n1,c1);
	addVertex(p2,t2,n2,c2);
	addVertex(p3,t3,n3,c3);
	*/
	void addTriangle(
			vec3 p1          , vec3 p2          , vec3 p3,
			vec2 t1= vec2NaN, vec2 t2= vec2NaN, vec2 t3= vec2NaN,
			vec3 n1= vec3NaN, vec3 n2= vec3NaN, vec3 n3= vec3NaN,
			vec4 c1= vec4NaN, vec4 c2= vec4NaN, vec4 c3= vec4NaN
			);


	void addQuad(
			vec3 p1          , vec3 p2          ,
			vec3 p3          , vec3 p4          ,
			vec2 t1 = vec2NaN, vec2 t2 = vec2NaN,
			vec2 t3 = vec2NaN, vec2 t4 = vec2NaN,
			vec3 n1 = vec3NaN, vec3 n2 = vec3NaN,
			vec3 n3 = vec3NaN, vec3 n4 = vec3NaN,
			vec4 c1 = vec4NaN, vec4 c2 = vec4NaN,
			vec4 c3 = vec4NaN, vec4 c4 = vec4NaN
			);

};

}
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



#endif //USING_OFL_ALL_H
#ifndef USING_OFL_OGL_GEO_H
 #define USING_OFL_OGL_GEO_H
 
#endif //USING_OFL_OGL_GEO_H
#ifndef USING_OFL_OGL_STATE_H
 #define USING_OFL_OGL_STATE_H
 
#endif //USING_OFL_OGL_STATE_H
#ifndef USING_OFL_OGL_WIN_H
 #define USING_OFL_OGL_WIN_H
 
#endif //USING_OFL_OGL_WIN_H
#ifndef USING_OFL_SOCKET_H
 #define USING_OFL_SOCKET_H
 
#endif //USING_OFL_SOCKET_H
#ifndef USING_OFL_STRU_H
 #define USING_OFL_STRU_H
 
#endif //USING_OFL_STRU_H
#ifndef USING_OFL_TRACK_H
 #define USING_OFL_TRACK_H
 
#endif //USING_OFL_TRACK_H
#ifndef USING_OFL_VD_H
 #define USING_OFL_VD_H
 
#endif //USING_OFL_VD_H
#ifndef USING_OFL_VDMAN_H
 #define USING_OFL_VDMAN_H
 
#endif //USING_OFL_VDMAN_H
#ifndef USING_OFL_VMATH_H
 #define USING_OFL_VMATH_H
 
#endif //USING_OFL_VMATH_H
#ifndef USING_OFL_VRPV_H
 #define USING_OFL_VRPV_H
 
#endif //USING_OFL_VRPV_H
#ifdef OFL_IMPLEMENTATION

namespace ofl
{

Geometry::Geometry(VertexData* vd, bool destroy_vd)
{

	m_vbo = m_vao = m_ibo = 0;
	if (glGenVertexArrays)
		glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_vbo);


	uploadData(vd);
	if(destroy_vd)
		delete vd;
}

Geometry::Geometry()
{
	m_vbo = m_vao = m_ibo = 0;
	if (glGenVertexArrays)
		glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_ibo);
	glGenBuffers(1, &m_vbo);
}

void Geometry::uploadData(VertexData *vd)
{
	if (m_vao)
		glBindVertexArray(m_vao);

	const size_t vertex_size = sizeof(Vertex);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vd->indices().size() *
				 sizeof(unsigned int), vd->indices().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER,
				 vertex_size* vd->data().size(),
				 vd->data().data(),
				 GL_STATIC_DRAW);

	const Vertex v = vd->data()[0];



	if (m_vao)
	{
#define addr_diff(a,b) ((void*)((char*)a-(char*) b))
		glVertexAttribPointer(ALOC_POSITION, 3, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.pos()),&v));
		glEnableVertexAttribArray(ALOC_POSITION);

		glVertexAttribPointer(ALOC_NORMAL, 3, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.nrm()),&v));
		glEnableVertexAttribArray(ALOC_NORMAL);

		glVertexAttribPointer(ALOC_COLOR, 4, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.clr()),&v));
		glEnableVertexAttribArray(ALOC_COLOR);

		glVertexAttribPointer(ALOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.tex()),&v));
		glEnableVertexAttribArray(ALOC_TEXCOORD);

		glVertexAttribPointer(ALOC_TANGENT, 3, GL_FLOAT, GL_FALSE,
							  vertex_size, addr_diff(&(v.tan()),&v));
		glEnableVertexAttribArray(ALOC_TANGENT);
#undef addr_diff
	}

	m_primitive = vd->primitive();
	m_vertice_count = vd->indices().size();
}

Geometry::~Geometry()
{
	if(m_vbo)
		glDeleteBuffers(1, &m_vbo);
	if(m_ibo)
		glDeleteBuffers(1, &m_ibo);
	if(m_vao)
		glDeleteVertexArrays(1, &m_vao);
}

}
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
	glGetShaderiv(shader_program, GL_LINK_STATUS, &wasLinked);
	if(wasLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader_program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader_program, maxLength, &maxLength, &errorLog[0]);
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

#include <cstdio>
#include <fstream>
#include <map>

using namespace ofl;
namespace ofl {

bool Vertex::operator ==(const Vertex &o)
{
	bool to_ret = true;
	float* data = (float*)this;
	const float* odata = (const float*)&o;
	const float eps = std::numeric_limits<float>::epsilon();
	float d;
	for (uint i = 0; i < sizeof(Vertex)/sizeof(float); i++)
	{
		d = fabs(odata[i] - data[i]);
		to_ret = to_ret && d <= eps;
	}
	return to_ret;
}

bool Vertex::operator <(const Vertex &o) const
{
	float* data = (float*)this;
	const float* odata = (const float*)&o;
	for (uint i = 0; i < sizeof(Vertex)/sizeof(float); i++)
	{
		const float& a = data[i];
		const float& b = odata[i];
		if (a < b)
			return true;
		else if (a == b)
			continue;
		else
			return false;
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
		v_id =  vd->push_back(v);
		v_loc[v] = v_id;
	}
	vd->push_back(v_id);
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
			positions.push_back(vec3(atof(arg[0]),atof(arg[1]),
					atof(arg[2])));
		}
		else if (type == "vn")
		{
			normals.push_back(vec3(atof(arg[0]), atof(arg[1]),
					atof(arg[2])));
		}
		else if (type == "vt")
		{
			tex_coords.push_back(vec2(atof(arg[0]),
								 atof(arg[1])));
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

						int p_id  = atoi(arg[i]) - 1;

						v.pos() = positions[p_id];
						handle_v(vd,v_loc,v);
					}
				}
				else if (normals.empty())	// positions and texcoords
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.reset(std::string(arg[i]));
						int  p_id = atoi(tkn.getToken('/')) - 1;
						int t_id = atoi(tkn.getRest()) - 1;
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
						int  p_id = atoi(tkn.getToken('/')) - 1;
						tkn.getToken('/');
						int  n_id = atoi(tkn.getRest()) - 1;

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
						int  p_id = atoi(tkn.getToken('/')) - 1;
						int  t_id = atoi(tkn.getToken('/')) - 1;
						int  n_id = atoi(tkn.getRest()) - 1;

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
	unsigned int vertex_count;
	unsigned int face_count;

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

	for (unsigned int i = 0; i < vertex_count; i++)
	{
		std::getline(fstream, line);
		tkn.reset(line);
		for (unsigned int j = 0; j < properties.size(); j++)
		{
			const auto& prp = properties[j];
			const auto& typ = prop_types[j];

			auto val = (float)atof(tkn.getToken(' '));

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

	for (unsigned int i = 0; i < face_count; i++)
	{
		std::getline(fstream, line);
		tkn.reset(line);
		auto verts_in_this_face = atoi(tkn.getToken(' '));

		int vert0 = atoi(tkn.getToken(' '));
		int vert1 = atoi(tkn.getToken(' '));

		for (int i = 2; i < verts_in_this_face; i+=1)
		{
			int vert2 = atoi(tkn.getToken(' '));
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

	int num_attrib = ::log2(ATTRIBUTE_LAST)+1;
	hline[0] = *((uint32_t*) prefix.c_str());
	hline[1] = (num_attrib+1)*5*sizeof(uint32_t); //headr size
	hline[2] = 1; // version
	hline[3] = num_attrib;
	hline[4] = vd->primitive();

	fwrite(hline,1,5*sizeof(uint32_t),f);
	const Vertex v = vd->data()[0];

#define addr_diff(a,b) (((char*)a-(char*) b))
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

	hline[0] = vd->data().size();
	hline[1] = vd->data().size()*sizeof(Vertex);
	hline[2] = vd->indices().size();
	hline[3] =  vd->indices().size() * sizeof(uint32_t);
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
	int consumed = 0;
	if(prim == TRIANGLES)
		consumed = 3;
	else if(prim == TRIANGLE_STRIP)
		consumed = 1;
	else if(prim == QUADS)
		consumed = 4;
	else if(prim == QUAD_STRIP)
		consumed = 2;
	int vs = 4 -consumed%2;


	for(unsigned int i = 0 ; i< vd->indices().size();i+=consumed)
	{

		int v[4];
		if (consumed >= 3 || i%2 ==1)
		{
			for (int j = 0; j < vs; j++)
			{
				v[j] = vd->indices()[i+j];
			}
		}
		else
		{
			for (int j = 0; j < vs; j++)
			{
				const int idx = j==0? 1 : j==1? 0:j;
				v[j] = vd->indices()[i+idx];
			}
		}

		obj_face face;
		for (int j = 0; j < vs; j++)
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
		for (int j = 0; j < vs; j++)
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
	default:
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

	default:
		return nullptr;
	}
}

void VertexDataTools::calculateNormals(VertexData *vd)
{
	std::vector<Vertex>& verts =  vd->data();

	const std::vector<uint32_t>& indices = vd->indices();

	std::map<vec3, vec3, compare_vec_3> p2n;

	Primitive prim = vd->primitive();


	int consumed = 0;
	if(prim == TRIANGLES)
		consumed = 3;
	else if(prim == TRIANGLE_STRIP)
		consumed = 1;
	else if(prim == QUADS)
		consumed = 4;
	else if(prim == QUAD_STRIP)
		consumed = 2;
	int vs = 4 -consumed%2;
	if (prim != 0)
	{
		for (unsigned int i = 0; i < indices.size(); i += consumed)
		{
			const vec3* v[4];
			if (consumed >= 3 || i%2 ==1)
			{
				for (int j = 0; j < vs; j++)
				{
					v[j] = &(verts.at(indices[i+j]).pos());
				}
			}
			else
			{
				for (int j = 0; j < vs; j++)
				{
					const int idx = j==0? 1 : j==1? 0:j;
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
			float area = 0.5*length(cross(*(v[1])- *(v[0]), *(v[2]) - *(v[0])));

			/* add the normal to each corner of the triangle */
			for (int j = 0; j < vs; j++)
			{
				const vec3 A = *(v[(j+1)%vs])- *(v[j]);
				const vec3 B = *(v[(j-1)%vs])- *(v[j]);
				const vec3 C = *(v[(j+2)%vs])- *(v[j]);
				vec3& r = p2n[*(v[j])];
				if(vs == 4) // for a quad triangulate first
				{
					if(j % 2 != connected)
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
#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;


namespace ofl {
using namespace ofl;

#define RESET_PRIMITIVE 0x0000FFFF


VertexDataManufacturer::VertexDataManufacturer()
{
	this->m_input_primitive = TRIANGLES;
	this->m_normal_state = vec3(0, 0, 1);
	this->tex_coord_state = vec3(0, 0, 0);
	this->m_color_state = vec4(1, 1, 1, 1);
	this->current_mesh = new VertexData();

}
VertexDataManufacturer::~VertexDataManufacturer()
{
	delete current_mesh;
}


void VertexDataManufacturer::begin(Primitive primitive)
{
	m_input_primitive = primitive;
}

void VertexDataManufacturer::handlePrimitiveBuffer()
{
	if (primitive_buffer.empty())
		return;

	if (m_input_primitive == QUADS)
	{
		if (primitive_buffer.size() == 4)
		{

			current_mesh->push_back(primitive_buffer[0]);
			current_mesh->push_back(primitive_buffer[1]);
			current_mesh->push_back(primitive_buffer[2]);
			current_mesh->push_back(primitive_buffer[0]);
			current_mesh->push_back(primitive_buffer[2]);
			current_mesh->push_back(primitive_buffer[3]);
			primitive_buffer.clear();
		}
	}
	else
	{
		current_mesh->push_back(primitive_buffer[0]);
		primitive_buffer.clear();
	}
	return;
}



#if 0
/**
This will end the current primitive run.
Note: If you combine different primitive types the whole patch will work
in TRIANGLES/LINES mode.
You can however combine TRIANGLE_STRIP and QUAD_STRIP since they
are both represented in TRIANGLE_STRIPs.
*/
void VertexDataManufacturer::end()
{


}

#endif

VertexData* VertexDataManufacturer::finish()
{
	current_mesh->setPrimitive(m_input_primitive);
	auto res = this->current_mesh;
	this->current_mesh = new VertexData();
	this->vertex_ids.clear();
	return res;
}



void VertexDataManufacturer::color(const vec4& color)
{
	if (!vecIsNaN(color))
		m_color_state = color;
}
void VertexDataManufacturer::color(const vec3& color)
{
	if (!vecIsNaN(color))
		m_color_state = vec4(color,1.0f);
}

void VertexDataManufacturer::color(
		const float& red,
		const float& green,
		const float& blue,
		const float& alpha)
{
	m_color_state = vec4(red, green, blue, alpha);
}


void VertexDataManufacturer::normal(const vec3& normal)
{
	if (!vecIsNaN(normal))
		m_normal_state = normal;
}

void VertexDataManufacturer::normal(const float& x,
									const float& y ,
									const float& z )
{
	m_normal_state = vec3(x,y,z);
}

void VertexDataManufacturer::texCoord(const vec3& tc)
{
	if (!vecIsNaN(tc))
		this->tex_coord_state = tc;
}

void VertexDataManufacturer::texCoord(const vec2& tc)
{
	if (!vecIsNaN(tc))
		this->tex_coord_state = vec3(tc,0.0f);
}
void VertexDataManufacturer::texCoord(
		const float& s,
		const float& t ,
		const float& r )
{
	this->tex_coord_state = vec3(s,t,r);
}

/** creates a vertex using the setup values for color, normal and uv.
*/
void VertexDataManufacturer::vertex(const vec4& vertex)
{
	const vec3  vert = vec3(vertex.x,vertex.y,vertex.z);
	Vertex v(vert, m_normal_state, m_color_state, tex_coord_state);
	uint32_t id = 0;

	if (this->vertex_ids.find(v) != vertex_ids.end())
	{
		id = vertex_ids[v];
	}
	else
	{
		id = this->current_mesh->data().size();
		this->current_mesh->push_back(v);
		vertex_ids[v] = id;
	}

	this->primitive_buffer.push_back(id);
	this->handlePrimitiveBuffer();

}

/** Internal shortcut for normal and tangent caluclations.
*/
void VertexDataManufacturer::vertex(const Vertex& vertex)
{
	Vertex v = vertex;
	uint32_t id = 0;

	if (this->vertex_ids.find(v) != vertex_ids.end())
	{
		id = vertex_ids[v];
	}
	else
	{
		id = current_mesh->data().size();
		current_mesh->push_back(v);
		vertex_ids[v] = id;
	}

	this->primitive_buffer.push_back(id);
	this->handlePrimitiveBuffer();

}


void VertexDataManufacturer::vertex(const vec2& vertex)
{
	this->vertex(vec4(vertex, 0.0f, 1.0f));
}
void VertexDataManufacturer::vertex(const vec3& vertex)
{
	this->vertex(vec4(vertex, 1.0f));
}
void VertexDataManufacturer::vertex(
		const float& x,
		const float& y ,
		const float& z ,
		const float& w)
{
	this->vertex(vec4(x, y, z, w));
}



VertexData* VertexDataManufacturer::createBox(
		float x ,
		float y ,
		float z)
{

	this->begin(QUADS);
	float i = 0.5f;
	vec3 a(-i*x, -i*y, i*z);
	vec3 b(i*x, -i*y, i*z);
	vec3 c(-i*x, i*y, i*z);
	vec3 d(i*x, i*y, i*z);
	vec3 e(-i*x, -i*y, -i*z);
	vec3 f(i*x, -i*y, -i*z);
	vec3 g(-i*x, i*y, -i*z);
	vec3 h(i*x, i*y, -i*z);

	vec3 n(0, 0, 1);
	this->addQuad(a, b, d, c,
				  vec2(0.4, 0.0),vec2(0.7,0.0), vec2(0.7,0.3), vec2(0.4,0.3),
				  n,n,n,n);

	n = vec3(1, 0, 0);
	this->addQuad(b, f, h, d,
				  vec2(1.0, 0.3),vec2(1.0, 0.6), vec2(0.7, 0.6), vec2(0.7, 0.3),
				  n, n, n, n);

	n = vec3(0, 0, -1);
	this->addQuad(f, e, g, h,
				  vec2(0.7, 0.9),vec2(0.4, 0.9), vec2(0.4, 0.6), vec2(0.7, 0.6),
				  n, n, n, n);

	n = vec3(-1, 0, 0);
	this->addQuad(e, a, c, g,
				  vec2(0.1, 0.6),vec2(0.1, 0.3), vec2(0.4, 0.3), vec2(0.4, 0.6),
				  n, n, n, n);

	n = vec3(0, 1, 0);
	this->addQuad(c, d, h, g,
				  vec2(0.4, 0.3),vec2(0.7, 0.3), vec2(0.7, 0.6), vec2(0.4, 0.6),
				  n, n, n, n);

	n = vec3(0, -1, 0);
	this->addQuad(a, b, f, e,
				  vec2(0.0, 0.7),vec2(0.3, 0.7), vec2(0.3, 1.0), vec2(0.0, 1.0),
				  n, n, n, n);

	auto res = this->finish();
	this->calculateTangents(res);
	return res;

}

//******************************************************************************
VertexData* VertexDataManufacturer::createPlane(
		float w ,
		float h ,
		unsigned int tess_w ,
		unsigned int tess_h)
{
	float d_w = w / tess_w;
	float d_h = h / tess_h;

	vec3 offset = vec3(-w / 2, -h / 2, 0);
	this->begin(QUADS);
	normal(0.0f, 0.0f, 1.0f);

	for (unsigned int x = 0; x < tess_w; x++)
	{
		for (unsigned int y = 0; y < tess_h; y++)
		{
			this->addQuad(
						vec3((x + 0)*d_w, (y + 0)*d_h, 0) + offset,
						vec3((x + 1)*d_w, (y + 0)*d_h, 0) + offset,
						vec3((x + 1)*d_w, (y + 1)*d_h, 0) + offset,
						vec3((x + 0)*d_w, (y + 1)*d_h, 0) + offset,
						vec2((float)(x + 0) / tess_w, (float)(y + 0) / tess_h),
						vec2((float)(x + 1) / tess_w, (float)(y + 0) / tess_h),
						vec2((float)(x + 1) / tess_w, (float)(y + 1) / tess_h),
						vec2((float)(x + 0) / tess_w, (float)(y + 1) / tess_h));
		}
	}

	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}

VertexData* VertexDataManufacturer::createCoordinateSystem()
{
	this->begin(LINES);
	this->color(vec3(0.9f, 0.0f, 0.0f));
	this->vertex(0.0f, 0.0f, 0.0f);
	this->vertex(1.0f, 0.0f, 0.0f);

	this->vertex(1.0f, 0.05f, 0.0f);
	this->vertex(1.0f, -0.05f, 0.0f);

	this->vertex(1.0f, 0.05f, 0.0f);
	this->vertex(1.05f, 0.0f, 0.0f);
	this->vertex(1.05f, 0.0f, 0.0f);
	this->vertex(1.0f, -0.05f, 0.0f);

	this->color(vec3(0.0f, 0.9f, 0.0f));
	this->vertex(0.0f, 0.0f, 0.0f);
	this->vertex(0.0f, 1.0f, 0.0f);

	this->vertex(0.05f, 1.0f, 0.0f);
	this->vertex(-0.05f, 1.0f, 0.0f);

	this->vertex(0.05f, 1.0f, 0.0f);
	this->vertex(0.00f, 1.05f, 0.0f);
	this->vertex(0.00f, 1.05f, 0.0f);
	this->vertex(-0.05f, 1.0f, 0.0f);

	this->color(vec3(0.0f, 0.0f, 0.9f));
	this->vertex(0.0f, 0.0f, 0.0f);
	this->vertex(0.0f, 0.0f, 1.0f);

	this->vertex(0.0f, 0.05f, 1.0f);
	this->vertex(0.0f, -0.05f, 1.0f);

	this->vertex(0.0f, 0.05f, 1.0f);
	this->vertex(0.0f, 0.0f, 1.05f);
	this->vertex(0.0f, 0.0f, 1.05f);
	this->vertex(0.0f, -0.05f, 1.0f);

	return this->finish();
}

//******************************************************************************

vec2 colorTexCoordFramPosition(const vec3& pos,const float& offset)
{
	const float pi = 3.14159265359f;
	vec2 uv = vec2(pos.x, pos.y);
	float factor = 1.0f - (sin(fabs(pos.z) * pi)*0.1f);
	uv *= factor;
	uv += vec2(1);
	uv *= vec2(0.5);
	uv += (offset);

	uv *= M_SQRT2 / (1.0 + M_SQRT2);
	return uv;
}

VertexData* VertexDataManufacturer::createUVSphere(
		float radius ,
		unsigned int slices ,
		unsigned int stacks )
{
	this->begin(TRIANGLES);
	const float pi = 3.14159265359f;
	const float st_step = pi / stacks;
	const float sl_step = pi*2.0f / slices;
	for (unsigned int st = 0; st < stacks; st++)
	{
		const float st_r1 = st_step*st;
		const float st_r2 = st_step*st + st_step;
		for (unsigned int sl = 0; sl < slices; sl++)
		{
			const float sl_r1 = sl_step*sl;
			const float sl_r2 = sl_step*sl + sl_step;

			const float sin_o1 = sin(st_r1);
			const float sin_o2 = sin(st_r2);

			const float cos_o1 = cos(st_r1);
			const float cos_o2 = cos(st_r2);

			const float sin_d1 = sin(sl_r1);
			const float sin_d2 = sin(sl_r2);

			const float cos_d1 = cos(sl_r1);
			const float cos_d2 = cos(sl_r2);

			float tex_offset = 0.0f;
			if (st >= stacks / 2u)
				tex_offset = M_SQRT2*0.5f;


			/*Compute positions and texCoords for the current quad:*/
			vec3 pos[4];
			vec2 uv[4];
			pos[0] = vec3(sin_o1*cos_d1, sin_o1*sin_d1, cos_o1);
			pos[1] = vec3(sin_o2*cos_d1, sin_o2*sin_d1, cos_o2);
			pos[2] = vec3(sin_o2*cos_d2, sin_o2*sin_d2, cos_o2);
			pos[3] = vec3(sin_o1*cos_d2, sin_o1*sin_d2, cos_o1);

			for (int i = 0; i < 4;i++)
				uv[i] = colorTexCoordFramPosition(pos[i], tex_offset);

			/* Draw a quad unless you are about do draw the last and the firrst
			   stack.
			*/
			if (st != 0 && st< stacks - 1)
			{
				texCoord(uv[0]);
				normal(pos[0]);
				vertex(pos[0]*radius);

				texCoord(uv[1]);
				normal(pos[1]);
				vertex(pos[1]*radius);

				texCoord(uv[2]);
				normal(pos[2]);
				vertex(pos[2]*radius);

				texCoord(uv[0]);
				normal(pos[0]);
				vertex(pos[0]*radius);

				texCoord(uv[2]);
				normal(pos[2]);
				vertex(pos[2]*radius);

				texCoord(uv[3]);
				normal(pos[3]);
				vertex(pos[3]*radius);
			}
			else
			{
				/* If it is the last stack:*/
				if (st != 0)
				{
					texCoord(uv[0]);
					normal(pos[0]);
					vertex(pos[0]*radius);

					texCoord(uv[2]);
					normal(pos[2]);
					vertex(pos[2]*radius);

					texCoord(uv[3]);
					normal(pos[3]);
					vertex(pos[3]*radius);
				}
				/* If it is the first stack:*/
				else
				{
					texCoord(uv[0]);
					normal(pos[0]);
					vertex(pos[0]*radius);

					texCoord(uv[1]);
					normal(pos[1]);
					vertex(pos[1]*radius);

					texCoord(uv[2]);
					normal(pos[2]);
					vertex(pos[2]*radius);
				}
			}
		}
	}

	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}

//******************************************************************************

VertexData* VertexDataManufacturer::createCylinder(
		float radius,
		float height ,
		unsigned int slices ,
		unsigned int stacks )
{
	this->begin(QUADS);


	float nz = 0;

	/*Used to implement the UV mapping*/
	const float twoPi = 2.0f*3.14159265359f;
	float ar = height / (twoPi*radius);
	if (ar > 1)
		ar = 1;

	float circum_step = 2.0f*3.14159265359f / slices;
	float height_step = height / stacks;
	for (unsigned int st = 0; st < stacks ; st++) // stacks
	{
		float zlow = st *  height_step;
		float zhigh = (1+st) * height / stacks;
		for (unsigned int i = 0; i < slices; i++) // slices
		{

			/* Some precomputations we need for both normals and positions */
			float ax, bx, ay, by;
			ax = sin((i + 0)*circum_step);
			ay = cos((i + 0)*circum_step);
			bx = sin((i + 1)*circum_step);
			by = cos((i + 1)*circum_step);

			/* Create a quad.*/
			this->texCoord(((i + 0)*circum_step) / twoPi, zlow / height);
			normal(normalize(vec3(ax, ay, nz)));
			this->vertex(radius*ax, radius*ay, zlow);

			this->texCoord(((i + 1)*circum_step) / twoPi, zlow / height);
			normal(normalize(vec3(bx, by, nz)));
			this->vertex(radius*bx, radius*by, zlow);

			this->texCoord(((i + 1)*circum_step) / twoPi, zhigh / height);
			normal(normalize(vec3(bx, by, nz)));
			this->vertex(radius*bx, radius*by, zhigh);

			this->texCoord(((i + 0)*circum_step) / twoPi, zhigh / height);
			normal(normalize(vec3(ax, ay, nz)));
			this->vertex(radius*ax, radius*ay, zhigh);
		}
	}
	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}

VertexData* VertexDataManufacturer::createCone(
		float baseRadius,
		float topRadius,
		float height,
		unsigned int slices,
		unsigned int stacks)

{
	this->begin(QUADS);
	float circum_step = 2.0f*3.141595654f / slices;
	float radius_step = (topRadius - baseRadius) / stacks;
	float height_step = height / stacks;
	const float inner_radius = 0.01f;
	float nz = tan((baseRadius - topRadius) / height);

	for (unsigned int st = 0; st < stacks; st++) // stacks
	{
		float zlow = st *  height_step;
		float zhigh = (1 + st) * height / stacks;
		float rlow = baseRadius + st*radius_step;
		float rhigh = baseRadius + (st + 1)*radius_step;

		float uv_r_low = ((1.0f) - ((float)st / stacks));
		uv_r_low = (0.5f - inner_radius) * uv_r_low + inner_radius;

		float uv_r_high = ((1.0f) - ((1.0f + (float)st) / stacks));
		uv_r_high = (0.5f - inner_radius) * uv_r_high + inner_radius;;

		for (unsigned int i = 0; i < slices; i++) // slices
		{
			/* Some precomputations we need for both normals and positions */
			float ax, bx, ay, by;
			ax = sin((i + 0)*circum_step);
			ay = cos((i + 0)*circum_step);
			bx = sin((i + 1)*circum_step);
			by = cos((i + 1)*circum_step);

			/* Create a quad.*/
			this->texCoord(uv_r_low * ax + 0.5f, uv_r_low*ay + 0.5f);
			normal(normalize(vec3(ax, ay, nz)));
			this->vertex(rlow*ax, rlow*ay, zlow);

			this->texCoord(uv_r_low * bx + 0.5f, uv_r_low*by + 0.5f);
			normal(normalize(vec3(bx, by, nz)));
			this->vertex(rlow*bx, rlow*by, zlow);

			this->texCoord(uv_r_high * bx + 0.5f, uv_r_high*by + 0.5f);
			normal(normalize(vec3(bx, by, nz)));
			this->vertex(rhigh*bx, rhigh*by, zhigh);

			this->texCoord(uv_r_high * ax + 0.5f, uv_r_high*ay + 0.5f);
			normal(normalize(vec3(ax, ay, nz)));
			this->vertex(rhigh*ax, rhigh*ay, zhigh);
		}
	}
	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}


VertexData* VertexDataManufacturer::createDisk(
		float innerRadius ,
		float outerRadius ,
		unsigned int slices ,
		unsigned int loops )
{

	this->begin(TRIANGLES);
	float circum_step = 2.0f*3.141595654f / slices;
	float radius_step = (outerRadius - innerRadius) / loops;

	normal(vec3(0, 0, 1));
	for (unsigned int st = 0; st < loops; st++) // stacks
	{
		float rlow = innerRadius + st*radius_step;
		float rhigh = innerRadius + (st + 1)*radius_step;
		for (unsigned int i = 0; i < slices; i++) // slices
		{

			/* Some precomputations we need for both normals and positions */
			float ax, bx, ay, by;
			ax = sin((i + 0)*circum_step);
			ay = cos((i + 0)*circum_step);
			bx = sin((i + 1)*circum_step);
			by = cos((i + 1)*circum_step);

			/* Create a quad.*/

			vec3 pos[4];
			vec2 uv[4];
			pos[0] = vec3(rlow*ax, rlow*ay, 0);
			pos[1] = vec3(rlow*bx, rlow*by, 0);
			pos[2] = vec3(rhigh*bx, rhigh*by, 0);
			pos[3] = vec3(rhigh*ax, rhigh*ay, 0);

			for (int i = 0; i < 4; i++)
			{
				uv[i] = vec2(pos[i].x / (2 * outerRadius) + 0.5,
							 pos[i].y / (2 * outerRadius) + 0.5);
			}

			if (st != 0)
			{
				this->texCoord(uv[0]);
				this->vertex(pos[0]);
				this->texCoord(uv[1]);
				this->vertex(pos[1]);
				this->texCoord(uv[2]);
				this->vertex(pos[2]);
			}
			this->texCoord(uv[0]);
			this->vertex(pos[0]);
			this->texCoord(uv[2]);
			this->vertex(pos[2]);
			this->texCoord(uv[3]);
			this->vertex(pos[3]);
		}
	}
	auto res = this->finish();
	this->calculateTangents(res);
	return res;
}



void VertexDataManufacturer::addVertex(vec3 p, vec2 t, vec3 n, vec4 c)
{
	texCoord(t);
	normal(n);
	color(c);
	vertex(p);
}

void VertexDataManufacturer::addTriangle(
		vec3 p1, vec3 p2, vec3 p3,
		vec2 t1  , vec2 t2  , vec2 t3  ,
		vec3 n1  , vec3 n2  , vec3 n3 ,
		vec4 c1  , vec4 c2  , vec4 c3
		)
{
	vec3 n = normalize(cross(p2 - p1, p3 - p1));
	if (vecIsNaN(n1))
	{
		n1 = n;
	}
	if (vecIsNaN(n2))
	{
		n2 = n;
	}
	if (vecIsNaN(n3))
	{
		n3 = n;
	}

	if (m_input_primitive == QUADS ||
			m_input_primitive == QUAD_STRIP)
		return;
	if (m_input_primitive == LINES)
	{
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);

		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);

		addVertex(p3, t3, n3, c3);
		addVertex(p1, t1, n1, c1);
	}

	else if (m_input_primitive == LINE_STRIP)
	{
		this->current_mesh->push_back(RESET_PRIMITIVE);
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
		addVertex(p1, t1, n1, c1);
	}
	else
	{
		if (m_input_primitive == TRIANGLE_STRIP)
			this->current_mesh->push_back(RESET_PRIMITIVE);
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
	}
}




void VertexDataManufacturer::addQuad(
		vec3 p1, vec3 p2,
		vec3 p3, vec3 p4,
		vec2 t1, vec2 t2,
		vec2 t3, vec2 t4,
		vec3 n1, vec3 n2,
		vec3 n3, vec3 n4,
		vec4 c1, vec4 c2,
		vec4 c3, vec4 c4
		)
{
	if (vecIsNaN(n1))
	{
		n1 = m_normal_state;
	}
	if (vecIsNaN(n2))
	{
		n2 = m_normal_state;
	}
	if (vecIsNaN(n3))
	{
		n3 = m_normal_state;
	}
	if (vecIsNaN(n4))
	{
		n4 = m_normal_state;
	}

	if (m_input_primitive == TRIANGLES)
	{
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);

		addVertex(p1, t1, n1, c1);
		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);
	}
	if (m_input_primitive == QUADS)
	{
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);
	}
	else if (m_input_primitive == LINES)
	{
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);

		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);

		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);

		addVertex(p4, t4, n4, c4);
		addVertex(p1, t1, n1, c1);
	}

	else if (m_input_primitive == LINE_STRIP)
	{
		this->current_mesh->push_back(RESET_PRIMITIVE);
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);
		addVertex(p1, t1, n1, c1);
	}
	else
	{
		if (m_input_primitive == TRIANGLE_STRIP ||
				m_input_primitive == QUAD_STRIP)
			this->current_mesh->push_back(RESET_PRIMITIVE);
		addVertex(p1, t1, n1, c1);
		addVertex(p2, t2, n2, c2);
		addVertex(p3, t3, n3, c3);
		addVertex(p4, t4, n4, c4);
	}

}

}




#ifdef GLM_INCLUDED

using namespace  glm;
#else

////// mat4 ////////////////////////////////////////////////////////////////////

mat4::mat4(float diag)
{
	for(int i =0 ; i< 4;i++)
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



namespace ofl {


////// Screen //////////////////////////////////////////////////////////////////

vec4 Screen::corner(const int i) const
{
	return m_wall_space * m_corners[i];
}

Screen::Screen(const vec4 blc, const vec4 brc, const vec4 &tlc)
{

	m_corners[BL] = blc;
	m_corners[BR] = brc;
	m_corners[TL] = tlc;

	vec4 x = normalize(m_corners[BR]-m_corners[BL]);
	vec4 y = normalize(m_corners[TL]-m_corners[BL]);
	vec4 z = cross(x,y);


	m_wall_space = mat4(x,y,z,vec4(0,0,0,1));
	m_inv_wall_space = inverse(m_wall_space);

	for(int i = 0 ; i<3;i++)
	{
		m_corners[i] = m_inv_wall_space*m_corners[i];
	}
}

void Screen::calculate_projection_and_view(
		const vec4 &eye,
		const float near,
		const float far,
		mat4 &projection_matrix,
		mat4 &view_matrix) const
{

	vec4 corner[3];

	// eye to wallspace
	vec4 eyepos_wall= m_inv_wall_space * eye;

	// calculate the frustum
	for (int i=0; i<3; i++)
	{
		float f;

		corner[i]= m_corners[i] - eyepos_wall;
		f= near / corner[i][2];
		corner[i][0] *= f;
		corner[i][1] *= f;
	}

	// generate projection matrix
	projection_matrix = frustum(
				corner[0][0],
			corner[1][0],
			corner[0][1],
			corner[2][1], near, far);

	// generate view matrix
	view_matrix =   translate(m_wall_space,eye*-1.0f);
}

////// ScreenArrangement ///////////////////////////////////////////////////////

Screen *ScreenArrangement::addScreen(const Screen scrn, const std::string &name)
{
	m_screens.push_back(scrn);
	m_by_name[name] = &m_screens.back();
	return &m_screens.back();
}

const Screen *ScreenArrangement::getScreen(const int id) const
{
	if(id >= (int)m_screens.size())
		return nullptr;
	return &m_screens[id];
}

const Screen *ScreenArrangement::getScreen(const std::string &name)
{
	if(m_by_name.find(name) == m_by_name.end())
		return nullptr;
	return m_by_name[name];
}

unsigned int ScreenArrangement::countScreens() const {return m_screens.size();}

bool ScreenArrangement::loadScreens(const std::string &path)
{
	std::ifstream file (path);
	if(!file.is_open())
		return false;

	std::string line;
	while(std::getline(file,line))
	{
		ofl::trim(line);
		if(line.empty()|| line.at(0)=='#') continue;

		vec4 bl,br,tl;
		std::string name = line.substr(0,line.find(' '));
		line = line.substr(line.find(' ')+1);

		bl = read_from_string(line);
		br = read_from_string(line);
		tl = read_from_string(line);

		this->addScreen(Screen(bl,br,tl),name);
	}

	file.close();
	return true;
}

bool ScreenArrangement::saveScreens(const std::string &path)
{
	std::ofstream file (path);
	if(!file.is_open())
		return false;

	for(const std::pair<std::string,Screen*>& sp : m_by_name)
	{
		const Screen& s = *(sp.second);
		file<<sp.first;
		for(int i =0  ; i< 3; i++)
			file<<" "<<to_string(s.corner(i));
		file<<"\n";
	}
	file.close();
	return true;
}
}
#endif
