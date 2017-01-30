

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


typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;

typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;

class BoundingBox3
{
	vec3 m_min;
	vec3 m_max;
public:
	BoundingBox3():m_min(NAN),m_max(NAN)
	{
	}

	void add_point(const vec3& p)
	{
		if(std::isnan(m_min.x))
			m_min = m_max = p;
		else
		{
			for(uint32_t i= 0 ; i< 3;i++)
			{
				m_min[i] = m_min[i]<p[i]?m_min[i]:p[i];
				m_max[i] = m_max[i]>p[i]?m_max[i]:p[i];
			}
		}
	}

	void add_point(const vec4& p)
	{
		add_point(vec3(p));
	}

	void add_point(const vec2& p)
	{
		add_point(vec3(p,0.0f));
	}

	vec3 closest_point(const vec3& p)
	{
		vec3 res;
		for(uint32_t i = 0; i< 3;i++)
			res[i] = (p[i]<m_min[i])? m_min[i]:((p[i]>m_max[i])? m_max[i]:p[i]);
		return res;
	}

	vec3 closest_point(const vec3& p, bool& inside)
	{
		vec3 res;
		inside = true;
		for(uint32_t i = 0; i< 3;i++)
		{
			res[i] = (p[i]<m_min[i])? m_min[i]:((p[i]>m_max[i])? m_max[i]:p[i]);
			inside &= p[i]>=m_min[i] && p[i]<=m_max[i];
		}
		return res;
	}

	bool inside(const vec3& p)
	{
		bool res = true;
		for(uint32_t i = 0; i< 3;i++)
			res &= p[i]>=m_min[i] && p[i]<=m_max[i];
		return  res;
	}

	float dist_hull(const vec3& p)
	{
		return glm::distance(p,closest_point(p));
	}

	float dist_volume(const vec3& p)
	{
		return inside(p)?0.0f:glm::distance(p,closest_point(p));
	}

	float dist_volume2(const vec3& p)
	{
		return inside(p)?0.0f:glm::distance2(p,closest_point(p));
	}

	// negative for points on the outside
	float signed_dist(const vec3& p)
	{
		bool in= true;
		auto r = glm::distance(p,closest_point(p,in));
		if(!in)
			r*=-1.0f;
		return r;
	}


};

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
