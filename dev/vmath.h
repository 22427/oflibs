

#pragma once
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace ofl
{


template <typename T>
inline bool eq(const T&a,const T&b)
{
	return a==b;
}

inline bool eq(const float& a,const float& b)
{
	return std::nextafter(a, std::numeric_limits<float>::lowest()) <= b
		&& std::nextafter(a, std::numeric_limits<float>::max()) >= b;
}

inline bool eq(const double& a,const double& b)
{
	return std::nextafter(a, std::numeric_limits<double>::lowest()) <= b
		&& std::nextafter(a, std::numeric_limits<double>::max()) >= b;
}


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

	BoundingBox3(const vec3& min=vec3(NAN), const vec3& max=vec3(NAN))
		:m_min(min),m_max(max)
	{
	}

	BoundingBox3(const float minx,const float miny,const float minz,
				 const float maxx,const float maxy,const float maxz)
		:m_min(minx,miny,minz),m_max(maxx,maxy,maxz)
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

	vec3& min() {return m_min;}
	const vec3& min() const {return m_min;}
	vec3& max() {return m_max;}
	const vec3& max() const {return m_max;}

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
	for(int j = 0 ; j< 2;j++)
	{
		if(eq(a[j],b[j]))
			continue;
		if(a[j]<b[j])
			return true;
		else
			return false;
	}
	return  false;
}


inline bool operator==(const vec2& a , const vec2& b)
{
	for(int j = 0 ; j< 2;j++)
	{
		if(!eq(a[j],b[j]))
			return  false;
	}
	return true;
}



inline bool operator<(const vec3& a , const vec3& b)
{

	for(int j = 0 ; j< 3;j++)
	{
		if(eq(a[j],b[j]))
			continue;
		if(a[j]<b[j])
			return true;
		else
			return false;
	}
	return  false;
}

inline bool operator==(const vec3& a , const vec3& b)
{
	for(int j = 0 ; j< 3;j++)
	{
		if(!eq(a[j],b[j]))
			return false;
	}
	return true;
}


inline bool operator<(const vec4& a , const vec4& b)
{
	for(int j = 0 ; j< 4;j++)
	{
		if(eq(a[j],b[j]))
			continue;
		if(a[j]<b[j])
			return true;
		else
			return false;
	}
	return false;
}

inline bool operator==(const vec4& a , const vec4& b)
{
	for(int j = 0 ; j< 4;j++)
	{
		if(!eq(a[j],b[j]))
			return false;
	}
	return true;
}

template<typename T>
class Comperator
{
public:
	bool operator()(const T& a, const T&b)
	{
		return a < b;
	}
};


/**
 * @brief get_transformation Gives a transformation matrix for T*fs[i] = ts[i]
 * @param fs 4 points in one space
 * @param ts the same 4 points in target space
 * @return T
 */
inline mat4 get_transformation(const std::vector<vec3>& fs, const std::vector<vec3>& ts)
{
	const mat4 F(vec4(fs[1]-fs[0],0.0f),vec4(fs[2]-fs[0],0.0f),vec4(fs[3]-fs[0],0.0f),vec4(fs[0],1.0f));
	const mat4 T(vec4(ts[1]-ts[0],0.0f),vec4(ts[2]-ts[0],0.0f),vec4(ts[3]-ts[0],0.0f),vec4(ts[0],1.0f));

	return T*glm::inverse(F) ;
}

typedef  mat4 mat_tri;
/**
 * @brief calc_triangle_matrix calculates the "triangel matrix" for a given triangle.
 * @param t1 first vertex of the triangle
 * @param t2 second vertex of the triangle
 * @param t3 third vertex of the triangle
 * @return the triangle matrix
 */
inline mat_tri calc_triangle_matrix(
		const vec3& t1,
		const vec3& t2,
		const vec3& t3)
{
	vec3 a = t2-t1;
	vec3 b = t3-t1;
	vec3 n = glm::cross(a,b);
	n = glm::normalize(n);

	return  mat4(vec4(a,0),vec4(b,0),vec4(n,0),vec4(t1,1));
}

/**
 * @brief closest_point gives the closest point to p on the triangle T
 * @param p The point you are interested in
 * @param T The triangle matrix
 * @param iT T^-1
 * @return the closest point on the triangle T
 */
inline vec3 closest_point(
		const vec3& p,
		const mat_tri& T,
		const mat_tri& iT)
{
	vec3 pt = vec3(iT * vec4(p,1.0f));

	pt.z = 1.0f-pt.x-pt.y;

	if(pt.z < 0)
	{
		pt.z = 0;
		const float f = 0.5f*sqrt(2.0f);
		const vec3 diag(f,-f,0);
		pt = vec3(0,1,0) + diag* glm::dot(diag,pt-vec3(0,1,0));
	}

	pt.x = pt.x<0.0f?0.0f : (pt.x > 1? 1.0f: pt.x);
	pt.y = pt.y<0.0f?0.0f : (pt.y > 1? 1.0f: pt.y);
	pt.z = 0;

	return vec3(T * vec4(pt,1));
}

/**
 * @brief closest_point gives the closest point to p on the triangle T
 * @param p The point you are interested in
 * @param T The triangle matrix
 * @return the closest point on the triangle T
 */
inline vec3 closest_point(
		const vec3& p,
		const mat_tri& T)
{
	const mat4 iT = glm::inverse(T);
	return closest_point(p,T,iT);
}





}
