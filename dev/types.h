#pragma once
#include <functional>
#include <numeric>
#include <cstdint>
#include <algorithm>
#include "dll.h"

typedef  uint32_t uint;
typedef  uint8_t ubyte;
typedef  int8_t byte;

/**
 * The type toolset is used to manage data types and their properties.
 */
namespace ofl
{
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
}
