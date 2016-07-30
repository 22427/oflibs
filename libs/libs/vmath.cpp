#include "vmath.h"




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
		stru::trim(elem);
		res[i] = atof(elem.c_str());
		str = str.substr(loc+1);
		if(loc == loc_end)
			break;
	}
	return res;


}


