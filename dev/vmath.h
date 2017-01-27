

#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

namespace ofl
{

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;


inline bool operator<(const vec2& a , const vec2& b)
{
	const float eps = std::numeric_limits<float>::epsilon();

	for(int j = 0 ; j< 2;j++)
	{
		auto e = a[j]-b[j];

		if(fabsf(e) < eps)
			continue;
		if(e<0)
			return true;
		if(e>0)
			return false;
	}
	return  false;
}


inline bool operator==(const vec2& a , const vec2& b)
{
	const float eps = std::numeric_limits<float>::epsilon();

	for(int j = 0 ; j< 2;j++)
	{
		auto e = a[j]-b[j];

		if(fabsf(e) < eps)
			continue;
		return  false;
	}
	return true;
}



inline bool operator<(const vec3& a , const vec3& b)
{
	const float eps = std::numeric_limits<float>::epsilon();

	for(int j = 0 ; j< 3;j++)
	{
		auto e = a[j]-b[j];

		if(fabsf(e) < eps)
			continue;
		if(e<0)
			return true;
		if(e>0)
			return false;
	}
	return  false;
}

inline bool operator==(const vec3& a , const vec3& b)
{
	const float eps = std::numeric_limits<float>::epsilon();

	for(int j = 0 ; j< 3;j++)
	{
		auto e = a[j]-b[j];

		if(fabsf(e) < eps)
			continue;
		return  false;
	}
	return  true;
}


inline bool operator<(const vec4& a , const vec4& b)
{
	const float eps = std::numeric_limits<float>::epsilon();

	for(int j = 0 ; j< 4;j++)
	{
		auto e = a[j]-b[j];

		if(fabsf(e) < eps)
			continue;
		if(e<0)
			return true;
		if(e>0)
			return false;
	}
	return false;
}

inline bool operator==(const vec4& a , const vec4& b)
{
	const float eps = std::numeric_limits<float>::epsilon();

	for(int j = 0 ; j< 4;j++)
	{
		auto e = a[j]-b[j];

		if(fabsf(e) < eps)
			continue;
		return  false;
	}
	return false;
}

template<typename T>
class Comperator
{
	public:
	bool operator()(const T& a, const T&b)
	{
		return a <b;
	}
};


}
