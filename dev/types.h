#pragma once
#include <functional>
#include <numeric>
#include <cstdint>
#include <algorithm>
#include "dll.h"
#include "vmath.h"


/**
 * The type toolset is used to manage data types and their properties.
 */
namespace ofl
{

typedef  uint32_t uint;
typedef  uint8_t ubyte;
typedef  int8_t byte;


inline OFL_DLL_LOCAL const char* skip_comment(const char* code);
inline OFL_DLL_PUBLIC const char* skip_ws(const char* code);

template<typename T>
inline typename std::enable_if<std::is_floating_point<T>::value || std::is_class<T>::value, T>::type
float_to_nint(float /*f*/)
{
	static_assert(true, "Conversion only to integer types!");
}

/**
 * @brief float_to_nint converts a float in [0,1] to an unsigned integer in [0,max].
 * This is done with respect to the OpenGL spec.
 * @param f float to confert
 * @return the integer representing f
 */
template<typename T>
inline typename std::enable_if<std::is_unsigned<T>::value, T>::type
float_to_nint(const float f)
{
	return static_cast<T>(f*std::numeric_limits<T>::max()/*((1<<((sizeof(T)*8)))-1)*/);
}


/**
 * @brief float_to_nint converts a float in [-1,1] to an integer in [min,max].
 * This is done with respect to the OpenGL spec.
 * @param f float to confert
 * @return the integer representing f
 */
template<typename T>
inline typename std::enable_if<std::is_signed<T>::value, T>::type
float_to_nint(const float f)
{
	return static_cast<T>(f*((1<<((sizeof(T)*8)-1))-1));
}


template<typename T>
/**
 * @brief float_to_nint converts a float in [-1,1] to an integer t in [min,max],
 * or converts a float in [0,1] to an unsigned integer in [0,max].
 * @param f float to convert
 * @param t int/uint to convert into.
 */
inline void float_to_nint(const float f, T& t)
{
	t = float_to_nint<T>(f);// static_cast<T>(f*((1<<((sizeof(T)*8)))-1));
}




/**
 * @brief nint_to_float converts a nint in [min,max] to the corresponding float in [-1,1]
 * @param x nint to convert
 * @return converted float
 */
template<typename T>
inline typename std::enable_if<std::is_signed<T>::value, float>::type
nint_to_float(const T x)
{
	const T t = ((1<<((sizeof(T)*8)-1))-1);
	return std::max(static_cast<float>(x)/t,-1.0f);
}


/**
 * @brief nint_to_float converts a unsigned nint in [0,max] to the corresponding float in [0,1]
 * @param x unsigned nint to convert
 * @return converted float
 */
template<typename T>
inline typename std::enable_if<std::is_unsigned<T>::value, float>::type
nint_to_float(const T x)
{
	const T t = std::numeric_limits<T>::max();//((1<<((sizeof(T)*8)))-1);
	return static_cast<float>(x)/t;
}


enum TypeID
{
	INVALID = 0x00,
	BYTE =0x1400,				// == GL_BYTE
	UNSIGNED_BYTE =0x1401,		// == GL_UNSIGNED_BYTE

	SHORT =0x1402,				// == GL_SHORT
	UNSIGNED_SHORT= 0x1403,		// == GL_UNSIGNED_SHORT

	INT= 0x1404,				// == GL_INT
	UNSIGNED_INT= 0x1405,		// == GL_UNSIGNED_INT

	FLOAT= 0x1406,				// == GL_FLOAT
	DOUBLE =0x140A,				// == GL_DOUBLE
};

class OFL_DLL_PUBLIC Type
{
public:
	TypeID id;

	Type(const TypeID id = INVALID):id(id){}
	Type(const unsigned int id)	:id(static_cast<TypeID>(id)){}
	Type(const int id)	:id(static_cast<TypeID>(id)){}
	Type(const std::string& str);

	operator TypeID() const {return  id;}
	operator unsigned int() const {	return  static_cast<unsigned int>(id);}
	operator int() const {	return  static_cast<int>(id);}
	std::string to_string() const
	{
		switch (id)
		{
		case BYTE: return "byte";
		case UNSIGNED_BYTE: return "ubyte";
		case SHORT: return "short";
		case UNSIGNED_SHORT: return "ushort";
		case INT: return "int";
		case UNSIGNED_INT: return "uint";
		case FLOAT: return "float";
		case DOUBLE: return "double";
		case INVALID: return "invalid";
		}
		return "invalid";
	}

	/**
	 * @brief to_string converts this type to a string.
	 * @return string representing this type for example: "int", "ushort" etc.
	 */
	std::string to_string();


	/**
	 * @brief is_integer test if this type is an integer
	 * @return true if this is an integer
	 */
	bool is_integer() const { return !(id == DOUBLE || id == FLOAT);}

	/**
	 * @brief is_float test if this type is a floating point data type
	 * @return true if this is a float or a double
	 */
	bool is_float() const{return (id == DOUBLE || id == FLOAT);}

	/**
	 * @brief is_unsigned tests for a sign
	 * @return true if this has no sign
	 */
	bool is_unsigned() const
	{
		return (id==UNSIGNED_BYTE||id==UNSIGNED_SHORT||id==UNSIGNED_INT);
	}
	/**
	 * @brief is_unsigned tests for a sign
	 * @return true if this has a sign
	 */
	bool is_signed() const
	{
		return !(id==UNSIGNED_BYTE||id==UNSIGNED_SHORT||id==UNSIGNED_INT);
	}

	bool operator ==(const Type& o)const{return  o.id == id;}
	bool operator ==(const TypeID id)const{return  this->id == id;}
	bool operator !=(const TypeID id)const{	return  this->id != id;}
	bool operator !=(const Type& o) const{return  o.id != id;}

	/**
	 * @brief size returns the size of one element of this type.
	 * @return
	 */
	size_t size() const;

	/**
	 * @brief max gives the maximal value a variable of this type can store
	 * @return the max value in double
	 */
	double max() const;

	/**
	 * @brief min gives the minimal value a variable of this type can store
	 * @return the min value in double
	 */
	double min() const;


};

/**
 * @brief to_string converts v to a human readable string.
 * @param v
 * @return
 */template <typename T>
OFL_DLL_PUBLIC inline std::string to_string(const T& v)
{
	return std::to_string(v);
}


/** READABLE STRING************************************************************/
/**
 * @brief readFromString reads from readable string.
 * @param str String to read from
 * @param end Pointer to the first not read character.
 * @return The read value
 */
template<typename T>
OFL_DLL_PUBLIC inline T read_from_string (const char* str, const char** end = nullptr);


/**
 * @brief writeToString transforms a given value into a readaable string
 * @param v
 * @return
 */template <typename T>
OFL_DLL_PUBLIC inline std::string write_to_string(const T& v)
{
	return std::to_string(v);
}



/** write_to_string ***********************************************************/

template <>
OFL_DLL_PUBLIC inline std::string to_string(const bool& v)
{
	if(v)
		return "true";
	else
		return "false";
}

template <>
OFL_DLL_PUBLIC inline  std::string to_string(const vec2& v)
{
	return "("+std::to_string(v.x)+","+std::to_string(v.y)+")";
}

template <>
OFL_DLL_PUBLIC inline std::string to_string(const vec3& v)
{
	return "("+to_string(v.x)+","+to_string(v.y)+","+to_string(v.z)+")";
}

template <>
OFL_DLL_PUBLIC inline std::string to_string(const vec4& v)
{
	return "("+to_string(v.x)+","+to_string(v.y)+","+to_string(v.z)+","+to_string(v.w)+")";
}

template <>
OFL_DLL_PUBLIC inline std::string to_string(const mat4& v)
{
	return "("+to_string(v[0])+","+to_string(v[1])+","+to_string(v[2])+","+to_string(v[3])+")";
}

template <>
OFL_DLL_PUBLIC inline std::string to_string(const std::string& v)
{
	return v;
}

template <typename T>
OFL_DLL_PUBLIC inline std::string to_string(const std::vector<T>& v)
{
	std::string res;
	bool first = true;
	for(const auto& i:v)
	{
		if(first)
		{
			res+=write_to_string(i);
			first = false;
		}
		else
			res+=","+write_to_string(i);
	}
	return res;
}



/** read_from_string **********************************************************/
template<>
OFL_DLL_PUBLIC inline int read_from_string (const char* str, const char** end)
{
	return  strtol(str,const_cast<char**>(end),10);
}

template<>
OFL_DLL_PUBLIC inline uint read_from_string (const char* str, const char** end)
{
	return strtol(str,const_cast<char**>(end),10);
}

template<>
OFL_DLL_PUBLIC inline float read_from_string (const char* str, const char** end)
{
	return strtof(str,const_cast<char**>(end));
}

template<>
OFL_DLL_PUBLIC inline double read_from_string (const char* str, const char** end)
{
	return strtod(str,const_cast<char**>(end));
}

template<>
OFL_DLL_PUBLIC inline bool read_from_string (const char* str, const char** end)
{
	str = skip_ws(str);
	int i=0;
	const char* t = "true";
	const char* f = "false";
	bool guess = true;
	for(i = 0 ; i< 5;i++)
	{
		if(guess && str[i] == t[i])
		{
			guess = true;
			if(i == 3)
				break;
		}
		else if(str[i] != t[i] && str[i] == f[i])
		{
			guess = false;
		}
		else
		{
			i = 0;
			guess = false;
			break;
		}
	}

	if(end)
		*end = str+i;
	return guess;
}

/**
 * Reads from an escaped string of the form:
 * const char* c = "\"text some \"special\" text with a backslash \\\"";
 */
template<>
OFL_DLL_PUBLIC inline std::string read_from_string (const char* str, const char** end)
{
	std::string res;
	const char* s = skip_ws(str);
	s++;
	while(*s && *s != '"')
	{
		if(*s == '\\')
			s++;
		res+=*s;
		s++;
	}
	if(*s=='"')
		s++;
	if(end)
		*end = s;
	return res;
}

/**
 * Reads a vec2 :
 * const char* c = "(x,y)";
 */
template<>
OFL_DLL_PUBLIC inline vec2 read_from_string (const char* str, const char** end)
{
	vec2 res;
	const char* s = skip_ws(str);
	s++;
	if(*s)
	{
		res.x = read_from_string<float>(s,&s);
		s = skip_ws(s);
		s++;
	}
	if(*s)
	{
		res.y = read_from_string<float>(s,&s);
		s = skip_ws(s);
		s++;
	}
	if(end)
		*end = s;
	return res;
}

/**
 * Reads a vec3 :
 * const char* c = "(x,y,z)";
 */
template<>
OFL_DLL_PUBLIC inline vec3 read_from_string (const char* str, const char** end)
{
	vec3 res;
	const char* s = skip_ws(str);
	s++;

	if(*s)
	{
		res.x = read_from_string<float>(s,&s);
		s = skip_ws(s);
		s++;
	}
	if(*s)
	{
		res.y = read_from_string<float>(s,&s);
		s = skip_ws(s);
		s++;
	}
	if(*s)
	{
		res.z = read_from_string<float>(s,&s);
		s = skip_ws(s);
		s++;
	}

	if(end)
		*end = s;
	return res;
}

/**
 * Reads a vec4 :
 * const char* c = "(x,y,z,w)";
 */
template<>
OFL_DLL_PUBLIC inline vec4 read_from_string (const char* str, const char** end)
{
	vec4 res;
	const char* s = skip_ws(str);
	s++;

	if(*s)
	{
		res.x = read_from_string<float>(s,&s);
		s = skip_ws(s);if(end)
			*end = s;
		s++;
		s = skip_ws(s);
	}
	if(*s)
	{
		res.y = read_from_string<float>(s,&s);
		s = skip_ws(s);
		s++;
		s = skip_ws(s);
	}
	if(*s)
	{
		res.z = read_from_string<float>(s,&s);
		s = skip_ws(s);
		s++;
		s = skip_ws(s);
	}
	if(*s)
	{
		res.w = read_from_string<float>(s,&s);
		s = skip_ws(s);
		s++;
		s = skip_ws(s);
	}

	if(end)
		*end = s;
	return res;
}


/**
 * Reads a mat3 :
 * const char* c = "(( x , y ,z ),( x , y ,z),( x , y ,z ),( x , y ,z ))";
 */
template<> //
OFL_DLL_PUBLIC inline mat3 read_from_string (const char* str, const char** end)
{
	mat3 res;
	str = skip_ws(str);
	str++;

	if(*str)
	{
		res[0] = read_from_string<vec3>(str,&str);
		str = skip_ws(str);
		str++;
		str = skip_ws(str);
	}
	if(*str)
	{
		res[1] = read_from_string<vec3>(str,&str);
		str = skip_ws(str);
		str++;
		str = skip_ws(str);
	}
	if(*str)
	{
		res[2] = read_from_string<vec3>(str,&str);
		str = skip_ws(str);
		str++;
		str = skip_ws(str);
	}


	if(end)
		*end = str;
	return res;
}

/**
 * Reads a mat4 :
 * const char* c = "(( x , y ,z, w ),( x , y ,z, w ),( x , y ,z, w ),( x , y ,z, w ))";
 */
template<> //
OFL_DLL_PUBLIC inline mat4 read_from_string (const char* str, const char** end)
{
	mat4 res;
	str = skip_ws(str);
	str++;

	if(*str)
	{
		res[0] = read_from_string<vec4>(str,&str);
		str = skip_ws(str);
		str++;
		str = skip_ws(str);
	}
	if(*str)
	{
		res[1] = read_from_string<vec4>(str,&str);
		str = skip_ws(str);
		str++;
		str = skip_ws(str);
	}
	if(*str)
	{
		res[2] = read_from_string<vec4>(str,&str);
		str = skip_ws(str);
		str++;
		str = skip_ws(str);
	}
	if(*str)
	{
		res[3] = read_from_string<vec4>(str,&str);
		str = skip_ws(str);
		str++;
		str = skip_ws(str);
	}

	if(end)
		*end = str;
	return res;
}

/**
 *	Reads a vector of one of the above. elements are seperated with ','.
 *  const char* c = "1,2,3,4";
 */
template<typename T>
inline std::vector<T> read_vector_from_string(const char* str, const char** end = nullptr)
{
	std::vector<T> res;
	while(*str)
	{
		res.push_back(read_from_string<T>(str,&str));
		str = skip_ws(str);
		if(*str != ',')
			break;
		str++;
		str = skip_ws(str);
	}
	if(end)
		*end = str;
	return res;
}



/** write_to_string ***********************************************************/

template <>
OFL_DLL_PUBLIC inline std::string write_to_string(const vec2& v)
{
	return "("+std::to_string(v.x)+","+std::to_string(v.y)+")";
}

template <>
OFL_DLL_PUBLIC inline std::string write_to_string(const vec3& v)
{
	return "("+write_to_string(v.x)+","+write_to_string(v.y)+","+write_to_string(v.z)+")";
}

template <>
OFL_DLL_PUBLIC inline std::string write_to_string(const vec4& v)
{
	return "("+write_to_string(v.x)+","+write_to_string(v.y)+","+write_to_string(v.z)+","+write_to_string(v.w)+")";
}


template <>
OFL_DLL_PUBLIC inline std::string write_to_string(const mat3& v)
{
	return "("+write_to_string(v[0])+","+write_to_string(v[1])+","+write_to_string(v[2])+")";
}

template <>
OFL_DLL_PUBLIC inline std::string write_to_string(const mat4& v)
{
	return "("+write_to_string(v[0])+","+write_to_string(v[1])+","+write_to_string(v[2])+","+write_to_string(v[3])+")";
}

template <>
OFL_DLL_PUBLIC inline std::string write_to_string(const std::string& v)
{
	std::string res = "\"";
	for(const char c :v)
	{
		if(c == '\\' || c== '\"')
			res+="\\";
		res+=c;
	}
	return res+"\"";
}

template <typename T>
OFL_DLL_PUBLIC inline std::string write_to_string(const std::vector<T>& v)
{
	std::string res;
	bool first = true;
	for(const auto& i:v)
	{
		if(first)
		{
			res+=write_to_string(i);
			first = false;
		}
		else
			res+=","+write_to_string(i);
	}
	return res;
}

inline OFL_DLL_LOCAL const char* skip_comment(const char* code)
{
	const char* res = code;
	while(*res && *res != '\n')
	{
		res++;
	}
	return res;
}


inline OFL_DLL_PUBLIC   const char* skip_ws(const char* code)
{
	const char* res = code;
	while(*res)
	{
		if(!isspace(*res))
		{
			if(*res == '#')
				res = skip_comment(res);
			else
				break;
		}
		res++;
	}
	return res;
}


}
