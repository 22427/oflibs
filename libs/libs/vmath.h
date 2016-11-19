
#pragma once
#include "stru.h"
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
