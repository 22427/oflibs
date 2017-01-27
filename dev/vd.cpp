#include "vd.h"
#include "stru.h"
#include "vmath.h"
#include <glm/gtx/norm.hpp>
namespace ofl {


uint32_t Attribute::size() const
{
	return  (type.size())*elements;
}


Attribute::Attribute(const AttributeID id, const uint32_t elements, const Type type, const bool normalized, bool use_constant, const void* conststant)
	:attribute_id(id),
	  elements(elements),
	  type(type),
	  offset(0),
	  normalized(normalized),
	  use_constant(use_constant)

{
	if(use_constant)
	{
		memcpy(this->constant,conststant,size());
	}
	offset = 0;

}

bool Attribute::read(const void *zero, vec4 &res) const
{
	res = vec4(0.0f,0.0f,0.0f,1.0f);
	if(use_constant)
		zero = constant;
	zero = reinterpret_cast<const ubyte*>(zero)+ offset;


	const int elems = static_cast<int>(elements);
	if(type == BYTE)
	{
		const byte* v = reinterpret_cast<const byte*>(zero);
		for(int i = 0 ; i<elems;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}
	else if( type == UNSIGNED_BYTE )
	{
		const ubyte* v = reinterpret_cast<const ubyte*>(zero) + offset;
		for(int i = 0 ; i<elems;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}

	else if(type == SHORT)
	{
		const int16_t* v = reinterpret_cast<const int16_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}
	else if(type == UNSIGNED_SHORT )
	{
		const uint16_t* v = reinterpret_cast<const uint16_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}

	else if(type == INT)
	{
		const int32_t* v = reinterpret_cast<const int32_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}
	else if( type == UNSIGNED_INT )
	{
		const uint32_t* v = reinterpret_cast<const uint32_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}

	else if(type == FLOAT)
	{
		const float* v = reinterpret_cast<const float*>(zero);
		for(int i = 0 ; i<elems;i++)
			res[i] = v[i];
	}

	else if(type == DOUBLE)
	{
		const double* v = reinterpret_cast<const double*>(zero);
		for(int i = 0 ; i<elems;i++)
			res[i] = static_cast<float>(v[i]);
	}
	else
		return false;

	return true;
}

bool Attribute::read(const void *zero, vec3 &res) const
{
	res = vec3(0.0f,0.0f,0.0f);
	if(use_constant)
		zero = constant;
	zero = reinterpret_cast<const ubyte*>(zero)+ offset;
	const int elems = static_cast<int>(elements);
	if(type == BYTE)
	{
		const byte* v = reinterpret_cast<const byte*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}
	else if(type == UNSIGNED_BYTE )
	{
		const ubyte* v = reinterpret_cast<const ubyte*>(zero) + offset;
		for(int i = 0 ; i<elems && i<3;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}

	else if(type == SHORT)
	{
		const int16_t* v = reinterpret_cast<const int16_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}
	else if(type == UNSIGNED_SHORT )
	{
		const uint16_t* v = reinterpret_cast<const uint16_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}

	else if(type == INT)
	{
		const int32_t* v = reinterpret_cast<const int32_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}
	else if(type == UNSIGNED_INT )
	{
		const uint32_t* v = reinterpret_cast<const uint32_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}

	else if(type == FLOAT)
	{
		const float* v = reinterpret_cast<const float*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			res[i] = v[i];
	}

	else if(type == DOUBLE)
	{
		const double* v = reinterpret_cast<const double*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			res[i] = static_cast<float>(v[i]);
	}
	else
		return false;

	return true;
}

bool Attribute::read(const void *zero, vec2 &res) const
{
	res = vec2(0.0f,0.0f);
	if(use_constant)
		zero = constant;
	zero = reinterpret_cast<const ubyte*>(zero)+ offset;
	const int elems = static_cast<int>(elements);
	if(type == BYTE)
	{
		const byte* v = reinterpret_cast<const byte*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}
	else if(type == UNSIGNED_BYTE )
	{
		const ubyte* v = reinterpret_cast<const ubyte*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}

	else if(type == SHORT)
	{
		const int16_t* v = reinterpret_cast<const int16_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}
	else if(type == UNSIGNED_SHORT )
	{
		const uint16_t* v = reinterpret_cast<const uint16_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}

	else if(type == INT)
	{
		const int32_t* v = static_cast<const int32_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}
	else if(type == UNSIGNED_INT )
	{
		const uint32_t* v = static_cast<const uint32_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			res[i] = normalized? nint_to_float(v[i]): v[i];
	}

	else if(type == FLOAT)
	{
		const float* v = static_cast<const float*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			res[i] = v[i];
	}

	else if(type == DOUBLE)
	{
		const double* v = static_cast<const double*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			res[i] = static_cast<float>(v[i]);
	}
	else
		return false;

	return true;
}

bool Attribute::read(const void *zero, float &res) const
{
	res = 0.0f;
	if(use_constant)
		zero = constant;
	zero = static_cast<const ubyte*>(zero)+ offset;

	if(type == BYTE)
	{
		const byte* v = static_cast<const byte*>(zero);
		res = normalized? nint_to_float(*v): *v;
	}
	else if(type == UNSIGNED_BYTE )
	{
		const ubyte* v = static_cast<const ubyte*>(zero) + offset;
		res = normalized? nint_to_float(*v): *v;
	}

	else if(type == SHORT)
	{
		const int16_t* v = static_cast<const int16_t*>(zero);
		res = normalized? nint_to_float(*v): *v;
	}
	else if(type == UNSIGNED_SHORT )
	{
		const uint16_t* v = static_cast<const uint16_t*>(zero);
		res = normalized? nint_to_float(*v): *v;
	}

	else if(type == INT)
	{
		const int32_t* v = static_cast<const int32_t*>(zero);
		res = normalized? nint_to_float(*v): *v;
	}
	else if(type == UNSIGNED_INT )
	{
		const uint32_t* v = static_cast<const uint32_t*>(zero);
		res = normalized? nint_to_float(*v): *v;
	}

	else if(type == FLOAT)
	{
		const float* v = static_cast<const float*>(zero);
		res = *v;
	}

	else if(type == DOUBLE)
	{
		const double* v = static_cast<const double*>(zero);

		res = static_cast<float>(*v);
	}
	else
		return false;

	return true;
}

bool Attribute::write(void *zero, const vec4 &v) const
{
	if(use_constant)
		return false;
	zero = static_cast<ubyte*>(zero)+ offset;
	const int elems = static_cast<int>(elements);
	if(type == BYTE)
	{
		int8_t* z = static_cast<int8_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<int8_t>(normalized? float_to_nint<int8_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_BYTE )
	{
		uint8_t* z = static_cast<uint8_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<uint8_t>(normalized? float_to_nint<uint8_t>(v[i]): v[i]);
	}

	else if(type == SHORT)
	{
		int16_t* z = static_cast<int16_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<int16_t>(normalized? float_to_nint<int16_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_SHORT )
	{
		uint16_t* z = static_cast<uint16_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<uint16_t>(normalized? float_to_nint<uint16_t>(v[i]) : v[i]);
	}

	else if(type == INT)
	{
		int32_t* z = static_cast<int32_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<int32_t>(normalized? float_to_nint<int32_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_INT )
	{
		uint32_t* z = static_cast<uint32_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<uint32_t>(normalized? float_to_nint<uint32_t>(v[i]) : v[i]);
	}

	else if(type == FLOAT)
	{
		float* z = static_cast<float*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] =  v[i];
	}
	else if(type == DOUBLE )
	{
		double* z = static_cast<double*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<double>(v[i]);
	}
	else
		return false;

	return true;
}

bool Attribute::write(void *zero, const vec3 &v) const
{
	if(use_constant)
		return false;
	zero = static_cast<ubyte*>(zero)+ offset;
	const int elems = static_cast<int>(elements);
	if(type == BYTE)
	{
		int8_t* z = static_cast<int8_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<int8_t>(normalized? float_to_nint<int8_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_BYTE )
	{
		uint8_t* z = static_cast<uint8_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<uint8_t>(normalized? float_to_nint<uint8_t>(v[i]): v[i]);
	}

	else if(type == SHORT)
	{
		int16_t* z = static_cast<int16_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<int16_t>(normalized? float_to_nint<int16_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_SHORT )
	{
		uint16_t* z = static_cast<uint16_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<uint16_t>(normalized? float_to_nint<uint16_t>(v[i]) : v[i]);
	}

	else if(type == INT)
	{
		int32_t* z = static_cast<int32_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<int32_t>(normalized? float_to_nint<int32_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_INT )
	{
		uint32_t* z = static_cast<uint32_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<uint32_t>(normalized? float_to_nint<uint32_t>(v[i]) : v[i]);
	}

	else if(type == FLOAT)
	{
		float* z = static_cast<float*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] =  v[i];
	}
	else if(type == DOUBLE )
	{
		double* z = static_cast<double*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<double>(v[i]);
	}
	else
		return false;

	return true;
}

bool Attribute::write(void *zero, const vec2 &v) const
{
	if(use_constant)
		return false;
	zero = static_cast<ubyte*>(zero)+ offset;
	const int elems = static_cast<int>(elements);
	if(type == BYTE)
	{
		int8_t* z = static_cast<int8_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<int8_t>(normalized? float_to_nint<int8_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_BYTE )
	{
		uint8_t* z = static_cast<uint8_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<uint8_t>(normalized? float_to_nint<uint8_t>(v[i]): v[i]);
	}

	else if(type == SHORT)
	{
		int16_t* z = static_cast<int16_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<int16_t>(normalized? float_to_nint<int16_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_SHORT )
	{
		uint16_t* z = static_cast<uint16_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<uint16_t>(normalized? float_to_nint<uint16_t>(v[i]) : v[i]);
	}

	else if(type == INT)
	{
		int32_t* z = static_cast<int32_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<int32_t>(normalized? float_to_nint<int32_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_INT )
	{
		uint32_t* z = static_cast<uint32_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<uint32_t>(normalized? float_to_nint<uint32_t>(v[i]) : v[i]);
	}

	else if(type == FLOAT)
	{
		float* z = static_cast<float*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] =  v[i];
	}
	else if(type == DOUBLE )
	{
		double* z = static_cast<double*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<double>(v[i]);
	}
	else
		return false;

	return true;
}

bool Attribute::write(void *zero, const float v) const
{
	if(use_constant)
		return false;
	zero = static_cast<ubyte*>(zero)+ offset;

	if(type == BYTE)
	{
		int8_t* z = static_cast<int8_t*>(zero);

		*z = static_cast<int8_t>(normalized? float_to_nint<int8_t>(v) : v);
	}
	else if( type == UNSIGNED_BYTE )
	{
		uint8_t* z = static_cast<uint8_t*>(zero);

		*z = static_cast<uint8_t>(normalized? float_to_nint<uint8_t>(v): v);
	}

	else if(type == SHORT)
	{
		int16_t* z = static_cast<int16_t*>(zero);

		*z = static_cast<int16_t>(normalized? float_to_nint<int16_t>(v) : v);
	}
	else if( type == UNSIGNED_SHORT )
	{
		uint16_t* z = static_cast<uint16_t*>(zero);

		*z = static_cast<uint16_t>(normalized? float_to_nint<uint16_t>(v) : v);
	}

	else if(type == INT)
	{
		int32_t* z = static_cast<int32_t*>(zero);

		*z = static_cast<int32_t>(normalized? float_to_nint<int32_t>(v) : v);
	}
	else if( type == UNSIGNED_INT )
	{
		uint32_t* z = static_cast<uint32_t*>(zero);

		*z = static_cast<uint32_t>(normalized? float_to_nint<uint32_t>(v) : v);
	}

	else if(type == FLOAT)
	{
		float* z = static_cast<float*>(zero);

		*z =  v;
	}
	else if( type == DOUBLE )
	{
		double* z = static_cast<double*>(zero);

		*z = static_cast<double>(v);
	}
	else
		return false;

	return true;
}



bool Attribute::write_constant(const vec4 &v)
{
	void* zero = static_cast<ubyte*>(constant);
	const int elems = static_cast<int>(elements);
	if(type == BYTE)
	{
		int8_t* z = static_cast<int8_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<int8_t>(normalized? float_to_nint<int8_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_BYTE )
	{
		uint8_t* z = static_cast<uint8_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<uint8_t>(normalized? float_to_nint<uint8_t>(v[i]): v[i]);
	}

	else if(type == SHORT)
	{
		int16_t* z = static_cast<int16_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<int16_t>(normalized? float_to_nint<int16_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_SHORT )
	{
		uint16_t* z = static_cast<uint16_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<uint16_t>(normalized? float_to_nint<uint16_t>(v[i]) : v[i]);
	}

	else if(type == INT)
	{
		int32_t* z = static_cast<int32_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<int32_t>(normalized? float_to_nint<int32_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_INT )
	{
		uint32_t* z = static_cast<uint32_t*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<uint32_t>(normalized? float_to_nint<uint32_t>(v[i]) : v[i]);
	}

	else if(type == FLOAT)
	{
		float* z = static_cast<float*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] =  v[i];
	}
	else if(type == DOUBLE )
	{
		double* z = static_cast<double*>(zero);
		for(int i = 0 ; i<elems;i++)
			z[i] = static_cast<double>(v[i]);
	}
	else
		return false;

	return true;
}

bool Attribute::write_constant(const vec3 &v)
{
	void* zero = static_cast<ubyte*>(constant);
	const int elems = static_cast<int>(elements);

	if(type == BYTE)
	{
		int8_t* z = static_cast<int8_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<int8_t>(normalized? float_to_nint<int8_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_BYTE )
	{
		uint8_t* z = static_cast<uint8_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<uint8_t>(normalized? float_to_nint<uint8_t>(v[i]): v[i]);
	}

	else if(type == SHORT)
	{
		int16_t* z = static_cast<int16_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<int16_t>(normalized? float_to_nint<int16_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_SHORT )
	{
		uint16_t* z = static_cast<uint16_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<uint16_t>(normalized? float_to_nint<uint16_t>(v[i]) : v[i]);
	}

	else if(type == INT)
	{
		int32_t* z = static_cast<int32_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<int32_t>(normalized? float_to_nint<int32_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_INT )
	{
		uint32_t* z = static_cast<uint32_t*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<uint32_t>(normalized? float_to_nint<uint32_t>(v[i]) : v[i]);
	}

	else if(type == FLOAT)
	{
		float* z = static_cast<float*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] =  v[i];
	}
	else if(type == DOUBLE )
	{
		double* z = static_cast<double*>(zero);
		for(int i = 0 ; i<elems && i<3;i++)
			z[i] = static_cast<double>(v[i]);
	}
	else
		return false;

	return true;
}

bool Attribute::write_constant(const vec2 &v)
{
	void* zero = static_cast<ubyte*>(constant);
	const int elems = static_cast<int>(elements);

	if(type == BYTE)
	{
		int8_t* z = static_cast<int8_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<int8_t>(normalized? float_to_nint<int8_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_BYTE )
	{
		uint8_t* z = static_cast<uint8_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<uint8_t>(normalized? float_to_nint<uint8_t>(v[i]): v[i]);
	}

	else if(type == SHORT)
	{
		int16_t* z = static_cast<int16_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<int16_t>(normalized? float_to_nint<int16_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_SHORT )
	{
		uint16_t* z = static_cast<uint16_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<uint16_t>(normalized? float_to_nint<uint16_t>(v[i]) : v[i]);
	}

	else if(type == INT)
	{
		int32_t* z = static_cast<int32_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<int32_t>(normalized? float_to_nint<int32_t>(v[i]) : v[i]);
	}
	else if(type == UNSIGNED_INT )
	{
		uint32_t* z = static_cast<uint32_t*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<uint32_t>(normalized? float_to_nint<uint32_t>(v[i]) : v[i]);
	}

	else if(type == FLOAT)
	{
		float* z = static_cast<float*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] =  v[i];
	}
	else if(type == DOUBLE )
	{
		double* z = static_cast<double*>(zero);
		for(int i = 0 ; i<elems && i<2;i++)
			z[i] = static_cast<double>(v[i]);
	}
	else
		return false;

	return true;
}

bool Attribute::write_constant(const float v)
{
	void* zero = static_cast<ubyte*>(constant);
	if(type == BYTE)
	{
		int8_t* z = static_cast<int8_t*>(zero);

		*z = static_cast<int8_t>(normalized? float_to_nint<int8_t>(v) : v);
	}
	else if( type == UNSIGNED_BYTE )
	{
		uint8_t* z = static_cast<uint8_t*>(zero);

		*z = static_cast<uint8_t>(normalized? float_to_nint<uint8_t>(v): v);
	}

	else if(type == SHORT)
	{
		int16_t* z = static_cast<int16_t*>(zero);

		*z = static_cast<int16_t>(normalized? float_to_nint<int16_t>(v) : v);
	}
	else if( type == UNSIGNED_SHORT )
	{
		uint16_t* z = static_cast<uint16_t*>(zero);

		*z = static_cast<uint16_t>(normalized? float_to_nint<uint16_t>(v) : v);
	}

	else if(type == INT)
	{
		int32_t* z = static_cast<int32_t*>(zero);

		*z = static_cast<int32_t>(normalized? float_to_nint<int32_t>(v) : v);
	}
	else if( type == UNSIGNED_INT )
	{
		uint32_t* z = static_cast<uint32_t*>(zero);

		*z = static_cast<uint32_t>(normalized? float_to_nint<uint32_t>(v) : v);
	}

	else if(type == FLOAT)
	{
		float* z = static_cast<float*>(zero);

		*z =  v;
	}
	else if( type == DOUBLE )
	{
		double* z = static_cast<double*>(zero);

		*z = static_cast<double>(v);
	}
	else
		return false;

	return true;
}


Vertex::Vertex(const VertexConfiguration &c, void *b)
	:m_data(b), cfg(c)
{
	if(!m_data)
	{
		m_destroy_data = true;
		m_data = malloc(cfg.vertex_size());
	}
	else
		m_destroy_data = false;
}

Vertex::Vertex(const Vertex &o):m_data(nullptr),cfg(o.cfg)
{
	if(o.m_data)
	{
		m_data = static_cast<ubyte*>(malloc(cfg.vertex_size()));
		memcpy(m_data,o.m_data,cfg.vertex_size());
		m_destroy_data = true;
	}
}

void Vertex::set_data(ubyte *d)
{
	if(m_data && m_destroy_data)
		free(m_data);
	m_data = d;
}


bool Vertex::get_value(const AttributeID id, vec4 &v) const
{
	if(!has_attribute(id))
		return false;
	const Attribute& a = cfg.get_attribute_by_id(id);
	return a.read(m_data,v);
}


bool Vertex::get_value(const AttributeID id, vec3 &v) const
{
	if(!has_attribute(id))
		return false;
	const Attribute& a = cfg.get_attribute_by_id(id);
	return a.read(m_data,v);
}


bool Vertex::get_value(const AttributeID id, vec2 &v) const
{
	if(!has_attribute(id))
		return false;
	const Attribute& a = cfg.get_attribute_by_id(id);
	return a.read(m_data,v);
}


bool Vertex::get_value(const AttributeID id, float &v) const
{
	if(!has_attribute(id))
		return false;
	const Attribute& a = cfg.get_attribute_by_id(id);
	return a.read(m_data,v);
}


bool Vertex::set_value(const AttributeID id, const vec4 &v) const
{
	if(!has_attribute(id))
		return false;
	const Attribute& a = cfg.get_attribute_by_id(id);
	return a.write(m_data,v);
}


bool Vertex::set_value(const AttributeID id, const vec3 &v) const
{
	const Attribute& a = cfg.get_attribute_by_id(id);
	return a.write(m_data,v);
}


bool Vertex::set_value(const AttributeID id, const vec2 &v) const
{
	const Attribute& a = cfg.get_attribute_by_id(id);
	return a.write(m_data,v);
}


bool Vertex::set_value(const AttributeID id, const float v) const
{
	const Attribute& a = cfg.get_attribute_by_id(id);
	return a.write(m_data,v);
}

void *Vertex::get_value_ptr(const AttributeID id)
{
	if(cfg.has_attribute(id))
		return static_cast<ubyte*>(m_data)+cfg.get_attribute_by_id(id).offset;
	else
		return nullptr;
}

bool Vertex::has_attribute(const AttributeID id) const
{
	return cfg.has_attribute(id);
}

bool Vertex::is_equal(const Vertex &o) const
{
	for(uint i = 0 ; i< cfg.attribute_count();i++)
	{
		const Attribute& a = cfg.get_attribute(i);
		if(o.has_attribute(a.attribute_id))
		{
			vec4 me, you;
			get_value(a.attribute_id,me);
			o.get_value(a.attribute_id,you);

			if(me!=you)
				return false;
		}
	}
	return  true;
}

bool Vertex::operator ==(const Vertex &o) const
{
	vec4 aa;
	vec4 bb;


	const float eps = 200.0*std::numeric_limits<float>::epsilon();

	for (uint i = 0 ; i< cfg.attribute_count();i++)
	{
		const auto a = cfg.get_attribute(i);
		const auto b = o.cfg.get_attribute_by_id(a.attribute_id);

		a.read(m_data,aa);
		b.read(o.m_data,bb);

		for(int j = 0 ; j< 4;j++)
		{
			auto e = fabsf(aa[j]-bb[j]);

			if(e < eps)
				continue;
			else
				return false;
		}


	}
	return true;
}

bool Vertex::operator !=(const Vertex &o) const
{
	return  !(*this==o);
}



bool Vertex::operator <(const Vertex &o) const
{
	vec4 aa;
	vec4 bb;

	for (uint i = 0 ; i< cfg.attribute_count();i++)
	{
		const auto a = cfg.get_attribute(i);
		const auto b = o.cfg.get_attribute_by_id(a.attribute_id);

		a.read(m_data,aa);
		b.read(o.m_data,bb);

		const float eps = 200.0*std::numeric_limits<float>::epsilon();

		for(int j = 0 ; j< 4;j++)
		{
			auto e = aa[j]-bb[j];

			if(fabs(e) < eps)
				continue;
			if(e<0)
				return true;
			if(e>0)
				return false;
		}

	}
	return false;

}


VertexData::VertexData(Primitive primitive,
					   VertexConfiguration cfg,
					   const Type index_type ,
					   const uint res_vtx,
					   const uint res_idx )
	:
	  m_render_primitive(primitive),
	  m_index_type(index_type),
	  m_index_count(0u),
	  m_index_reserve(0u),
	  m_index_data(nullptr),
	  m_cfg(cfg),
	  m_vertex_data(nullptr),
	  m_vertex_count(0u),
	  m_vertex_reserve(0u)
{
	if(res_vtx)
		vertices_reserve(res_vtx);


	if(res_idx)
		indices_reserve(res_idx);

}

void VertexData::vertices_reserve(const uint c)
{
	if(m_vertex_reserve < c)
	{
		m_vertex_reserve = c;
		if(!m_vertex_data)
		{
			m_vertex_data = static_cast<ubyte*>(malloc(c*m_cfg.vertex_size()));
		}
		else
		{
			m_vertex_data = static_cast<ubyte*>(realloc(m_vertex_data,m_vertex_reserve*m_cfg.vertex_size()));
		}
	}
}

void VertexData::indices_reserve(const uint c)
{
	if(m_index_reserve < c)
	{
		m_index_reserve = c;
		if(!m_index_data)
		{
			m_index_data = static_cast<ubyte*>(malloc(c*m_index_type.size()));
		}
		else
		{
			m_index_data = static_cast<ubyte*>(realloc(m_index_data,m_index_reserve*m_index_type.size()));
		}
	}
}

void VertexData::push_back(const uint32_t &i)
{
	indices_reserve(m_index_count+1);


	if(m_index_reserve <= m_index_count+1)
	{
		if(!m_index_data)
		{
			indices_reserve(1u);
		}
		else
		{
			const auto to_reserve = std::min(m_index_reserve*2u,m_index_reserve+8u);
			indices_reserve(to_reserve);
		}
	}

	switch (m_index_type.id)
	{
	case UNSIGNED_BYTE: static_cast<uint8_t*>(m_index_data)[m_index_count] = static_cast<uint8_t>(i);break;
	case UNSIGNED_SHORT: static_cast<uint16_t*>(m_index_data)[m_index_count] = static_cast<uint16_t>(i);break;
	case UNSIGNED_INT: static_cast<uint32_t*>(m_index_data)[m_index_count] = static_cast<uint32_t>(i);break;
	case BYTE:case SHORT:case INT: case FLOAT: case DOUBLE: case INVALID:
		static_assert(true, "invalid index type!!!");
	}

	m_index_count++;
}

uint32_t VertexData::get_index(const uint32_t i) const
{
	assert(m_index_type.is_integer() && m_index_type.is_unsigned());

	switch (m_index_type.id)
	{
	case UNSIGNED_BYTE: return static_cast<uint8_t* >(m_index_data)[i];
	case UNSIGNED_SHORT:return static_cast<uint16_t*>(m_index_data)[i];
	case UNSIGNED_INT:  return static_cast<uint32_t*>(m_index_data)[i];
	case BYTE:case SHORT:case INT: case FLOAT: case DOUBLE: case INVALID:
		return ~0u;
	}
	return  ~0u;
}

void VertexData::set_index(const uint32_t i, const uint32_t v)
{
	assert(m_index_type.is_integer() && m_index_type.is_unsigned());

	switch (m_index_type.id)
	{
	case UNSIGNED_BYTE:  static_cast<uint8_t* >(m_index_data)[i] =
				static_cast<uint8_t >(v);break;
	case UNSIGNED_SHORT: static_cast<uint16_t*>(m_index_data)[i] =
				static_cast<uint16_t>(v);break;
	case UNSIGNED_INT:   static_cast<uint32_t*>(m_index_data)[i] =
				static_cast<uint32_t>(v);break;
	case BYTE:case SHORT:case INT: case FLOAT: case DOUBLE: case INVALID:
		break;
	}
}

uint VertexData::push_back(const Vertex &v)
{
	if(m_vertex_reserve <= m_vertex_count+1)
	{
		if(!m_vertex_data)
		{
			vertices_reserve(1u);
		}
		else
		{
			const auto to_reserve = std::min(m_vertex_reserve*2u,m_vertex_reserve+8u);
			vertices_reserve(to_reserve);
		}
	}
	memcpy(static_cast<ubyte*>(m_vertex_data)+m_vertex_count*m_cfg.vertex_size(),v.m_data,m_cfg.vertex_size());
	m_vertex_count++;
	return m_vertex_count-1;
}


Primitive VertexData::primitive() const {return  m_render_primitive;}

void VertexData::set_primitive(const Primitive &p){m_render_primitive = p;}




void handle_v(VertexData* vd, std::map<Vertex,uint32_t>& v_loc, const Vertex& v)
{
	uint32_t v_id = 0;
	if(v_loc.find(v)!= v_loc.end())
	{
		v_id = v_loc[v];
	}
	else
	{
		v_id =  static_cast<uint32_t>(vd->push_back(v));
		v_loc[v] = v_id;
	}
	vd->push_back(static_cast<const uint32_t>(v_id));
}

#define atoff(s) static_cast<float>(atof(s))
#define atoiu(s) static_cast<uint32_t>(atoi(s))

VertexData *VertexDataOperations::read_obj(FILE* f)
{

	// mesh loader the 10000ths ^^



	std::vector<vec3> positions;
	std::vector<vec3> normals;
	std::vector<vec2> tex_coords;

	std::map<Vertex,uint32_t> v_loc;

	std::string type;
	char* arg[4];


	char* line;
	size_t cnt = 0;
	Tokenizer ltkn(nullptr);
	Tokenizer tkn(nullptr);
	VertexConfiguration cfg;
	VertexData* vd = nullptr;
	bool configurated = false;

	uint  p_id = 0;
	uint  t_id = 0;
	uint  n_id = 0;
	void* line_sav = nullptr;

	while (getline(&line,&cnt,f) != -1)
	{
		line_sav = line;
		ltkn.set_base(line);
		// remove comment
		line = ltkn.get_token('#');//line.substr(0, line.find_first_of('#'));
		ltkn.set_base(line);
		ltkn.skip_white_spaces();

		if (!ltkn.get_rest()||!*(ltkn.get_rest())) // the line was an empty line or a comment
			continue;



		type = ltkn.get_token(' ');

		for (int i = 0; i < 4; i++)
		{
			ltkn.skip_white_spaces();
			arg[i] = ltkn.get_token(' ');
		}

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
			if(!configurated)
			{
				if(!positions.empty())
				{
					cfg.add_attribute(Attribute(AttributeID::ATTRIB_POSITION,3,FLOAT,false));
				}
				if(!tex_coords.empty())
				{
					cfg.add_attribute(Attribute(AttributeID::ATTRIB_TEXCOORD,2,FLOAT,false));
				}
				if(!normals.empty())
				{
					cfg.add_attribute(Attribute(AttributeID::ATTRIB_NORMAL,3,FLOAT,false));
				}
				configurated = true;
				vd = new VertexData(TRIANGLES,cfg);
			}

			Vertex v(cfg,nullptr);

			if (arg[3])
			{

			}
			else
			{
				if(normals.empty() && tex_coords.empty())  // only positions
				{
					for (int i = 0; i < 3; i++)
					{

						p_id  = atoiu(arg[i]) - 1;
						v.set_value(AttributeID::ATTRIB_POSITION,positions[p_id]);
						handle_v(vd,v_loc,v);
					}
				}
				else if (normals.empty())	// positions and texcoords
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.set_base(arg[i]);
						tkn.get_token_as(p_id,"/");
						tkn.get_token_as(t_id,"/");

						v.set_value(AttributeID::ATTRIB_POSITION, positions[p_id-1]);
						v.set_value(AttributeID::ATTRIB_TEXCOORD, tex_coords[t_id-1]);
						handle_v(vd,v_loc,v);
					}
				}
				else if (tex_coords.empty())	 // positions and normals
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.set_base(arg[i]);
						tkn.get_token_as(p_id,"/");
						tkn.get_token('/');
						tkn.get_token_as(n_id,"/");

						v.set_value(AttributeID::ATTRIB_POSITION, positions[p_id-1]);
						v.set_value(AttributeID::ATTRIB_NORMAL, tex_coords[n_id-1]);
						handle_v(vd,v_loc,v);
					}
				}
				else		// all three
				{
					for (int i = 0; i < 3; i++)
					{
						tkn.set_base(arg[i]);
						tkn.get_token_as(p_id,"/");
						tkn.get_token_as(t_id,"/");
						tkn.get_token_as(n_id,"/");

						v.set_value(AttributeID::ATTRIB_POSITION, positions[p_id-1]);
						v.set_value(AttributeID::ATTRIB_TEXCOORD, tex_coords[t_id-1]);
						v.set_value(AttributeID::ATTRIB_NORMAL, normals[n_id-1]);

						handle_v(vd,v_loc,v);
					}
				}
			}
		}


		type.clear();
		ltkn.set_base(nullptr);
	}
	tkn.set_base(nullptr);
	if(line_sav)
		free(line_sav);

	return vd;
}

VertexData *VertexDataOperations::read_vd(FILE *f)
{
	if(!f)
		return nullptr;

	union bd
	{
		uint32_t i;
		char n[4];
	};

	bd name;
	name.n[0] = ':';
	name.n[1] = 'V';
	name.n[2] = 'D';
	name.n[3] = ':';

	bd atr;
	atr.n[0] = 'a';
	atr.n[1] = 't';
	atr.n[2] = 'r';
	atr.n[3] = ':';


	bd vtx;
	vtx.n[0] = 'v';
	vtx.n[1] = 't';
	vtx.n[2] = 'x';
	vtx.n[3] = ':';

	bd idx;
	idx.n[0] = 'i';
	idx.n[1] = 'd';
	idx.n[2] = 'x';
	idx.n[3] = ':';



	uint32_t r[16];
	fread(&r,16,1,f);

	if(r[0] != name.i) // check the magic number
		return nullptr;
	if(r[2] != 0xffff) // check for endianness
	{
		printf("endianness not implemented yet!\n");
		return nullptr;
	}
	if(r[3] != 1) // check version. we only know version 1 so that should do!
		return  nullptr;

	fread(&r,20,1,f);
	Primitive primitive = static_cast<Primitive>(r[0]);
	Type index_type = r[1];
	uint32_t index_count = r[2];
	uint32_t attribute_count = r[3];
	uint32_t vertex_count = r[4];

	fread(r,8,1,f); // read "atr:" constant and the size of the attribute information
	if(r[0] != atr.i)
		return  nullptr;
	VertexConfiguration cfg;
	for(uint32_t i = 0 ; i<attribute_count;i++)
	{
		Attribute a;
		fread(r,14*4,1,f);
		a.attribute_id = static_cast<AttributeID>(r[0]);
		a.elements = static_cast<uint8_t>(r[1]);
		a.type = r[2];
		a.normalized = r[3] != 0;
		a.use_constant = r[4] != 0;
		a.offset= r[5];
		memcpy(a.constant,&r[6],32);
		cfg.add_attribute(a);
	}


	VertexData* res = new VertexData(primitive,cfg,vertex_count,index_type,index_count);
	res->m_index_count = index_count;
	res->m_vertex_count = vertex_count;


	fread(r,8,1,f); // read "vtx:" constant and the size of the attribute information
	if(r[0] != vtx.i)
		return  nullptr;
	fread(res->m_vertex_data,r[1],1,f);

	fread(r,8,1,f); // read "idx:" constant and the size of the attribute information
	if(r[0] != idx.i)
		return  nullptr;
	fread(res->m_index_data,r[1],1,f);

	return  res;



}

bool VertexDataOperations::write_vd(const VertexData *vd, FILE *f)
{
	if(!f)
		return false;
	union bd
	{
		uint32_t i;
		char n[4];
	};
	bd buffer[16];

	buffer[0].n[0] = ':';
	buffer[0].n[1] = 'V';
	buffer[0].n[2] = 'D';
	buffer[0].n[3] = ':';

	buffer[1].i = 0;
	buffer[1].i+= 5*4; // render prim, index t, index cnt, attrib cnt, vertex cnt
	buffer[1].i+= 2;
	buffer[1].i+= vd->m_cfg.attribute_count()*14*4; // attributes
	buffer[1].i+= 2;
	buffer[1].i+= vd->vertex_count()*vd->m_cfg.vertex_size(); //vertex data
	buffer[1].i+= 2;
	buffer[1].i+= vd->index_count()*vd->index_type().size(); //index data
	buffer[2].i = 0xffff;
	buffer[3].i = 1;

	fwrite(buffer,16,1,f);

	buffer[0].i = vd->m_render_primitive;
	buffer[1].i = vd->m_index_type;
	buffer[2].i = vd->m_index_count;
	buffer[3].i = vd->m_cfg.attribute_count();
	buffer[4].i = vd->m_vertex_count;
	fwrite(buffer,5*4,1,f);

	buffer[0].n[0] = 'a';
	buffer[0].n[1] = 't';
	buffer[0].n[2] = 'r';
	buffer[0].n[3] = ':';
	buffer[1].i =vd->m_cfg.attribute_count()*14*4;
	fwrite(buffer,2*4,1,f);

	for(uint32_t a  = 0 ; a <vd->m_cfg.attribute_count();a++)
	{
		auto aa = vd->m_cfg.get_attribute(a);
		buffer[0].i =  static_cast<uint32_t>(aa.attribute_id);
		buffer[1].i =  static_cast<uint32_t>(aa.elements);
		buffer[2].i =  static_cast<uint32_t>(aa.type);
		buffer[3].i =  static_cast<uint32_t>(aa.normalized);
		buffer[4].i =  static_cast<uint32_t>(aa.use_constant);
		buffer[5].i =  static_cast<uint32_t>(aa.offset);
		memcpy(&(buffer[6]),aa.constant,32);
		fwrite(buffer,14*4,1,f);
	}
	buffer[0].n[0] = 'v';
	buffer[0].n[1] = 't';
	buffer[0].n[2] = 'x';
	buffer[0].n[3] = ':';
	buffer[1].i = vd->vertex_count()*vd->m_cfg.vertex_size();
	fwrite(buffer,2*4,1,f);
	fwrite(vd->m_vertex_data,buffer[1].i,1,f);

	buffer[0].n[0] = 'i';
	buffer[0].n[1] = 'd';
	buffer[0].n[2] = 'x';
	buffer[0].n[3] = ':';
	buffer[1].i = static_cast<uint32_t>(vd->index_count()*vd->index_type().size());
	fwrite(buffer,2*4,1,f);

	fwrite(vd->m_index_data,buffer[1].i,1,f);
	return true;
}





VertexData *VertexDataOperations::read_ply(FILE* f)
{

	if (!f)
		return nullptr;

	Tokenizer tkn(nullptr);
	//get the headers information
	uint32_t vertex_count = 0;
	uint32_t face_count = 0;
	Type index_type = UNSIGNED_INT;
	std::vector<std::string> properties;
	std::vector<Type> prop_types;

	bool vertex_prop = false;
	bool face_prop = false;
	size_t n = 0;
	char* line = nullptr;
	while (getline(&line,&n,f) >= 0)
	{
		tkn.set_base(line);
		std::string op = tkn.get_token();
		if (op == "element")
		{
			std::string s = tkn.get_token();
			if (s == "vertex")
			{
				vertex_prop = true;
				face_prop = false;
				tkn.get_token_as(vertex_count,tkn.whitespaces);

			}
			else if (s == "face")
			{
				tkn.get_token_as(face_count,tkn.whitespaces);
				face_prop = true;
				vertex_prop = false;
			}
			else
			{
				printf("PLY loader ignored element %s\n", s.c_str());
			}
		}
		else if (op == "property" && vertex_prop)
		{
			std::string type = tkn.get_token();
			std::string name = tkn.get_token();
			Type dt = type;
			properties.push_back(name);
			prop_types.push_back( dt);
		}
		else if (op == "property" && face_prop)
		{
			std::string	res;
			tkn.get_token();//list
			tkn.get_token();// list length
			res = tkn.get_token();// type of list data!
			trim(res);
			index_type = res;
		}
		else if (op == "end_header")
		{
			break;
		}
	}
	VertexConfiguration cfg;

	class prop_sc
	{
	public:
		prop_sc(AttributeID a,
				int32_t c,
				Type t,
				bool n):a(a),c(c),t(t),n(n){}
		AttributeID a;
		int32_t c;
		Type t;
		int n;
	};
	std::vector<prop_sc> psc;

	uint32_t i = 0 ;


	Attribute aa[ATTRIB_COUNT];
	for(uint32_t i= 0 ; i< ATTRIB_COUNT;i++)
	{
		aa[i].type = INVALID;
		aa[i].attribute_id = static_cast<AttributeID>(i);
	}


	for(const auto&p : properties)
	{
		Attribute* a = nullptr;
		if((p[0] == 'x' ||p[0] == 'y' ||p[0] == 'z' ))
			a= &(aa[ATTRIB_POSITION]);
		else if(p[0] == 'r'||p[0] == 'g'||p[0] == 'b'||p[0] == 'a')
			a= &(aa[ATTRIB_COLOR]);
		else if((p[0] == 'n'))
			a= &(aa[ATTRIB_NORMAL]);
		else if(p[0] == 's'||p[0] == 't'||p[0] == 'p')
			a= &(aa[ATTRIB_TEXCOORD]);

		if(p[0] == 'x' || p[0] == 'r' || p[1] == 'x' || p[0] == 's')
			a->elements = 1;
		if(p[0] == 'y' || p[0] == 'g'|| p[1] == 'y' || p[0] == 't')
			a->elements = 2;
		if(p[0] == 'z' || p[0] == 'b'|| p[1] == 'z' || p[0] == 'p')
			a->elements = 3;
		if(p[0] == 'w' || p[0] == 'a'|| p[1] == 'w' || p[0] == 'q')
			a->elements = 4;

		a->use_constant = false;
		a->type =  prop_types.at(i);
		a->normalized = a->type.is_integer();
		prop_sc ppp(a->attribute_id,static_cast<int>(a->elements)-1,a->type,a->normalized);
		psc.push_back(ppp);

		i++;
	}
	for(uint32_t i= 0 ; i< ATTRIB_COUNT;i++)
	{
		if(aa[i].type != INVALID)
			cfg.add_attribute(aa[i]);
	}


	VertexData* vd = new VertexData(TRIANGLES,cfg,vertex_count,index_type,face_count*3);
	// now read the vertex data

	Vertex vtx(cfg,nullptr);

	for (uint32_t i = 0; i < vertex_count; i++)
	{
		getline(&line,&n,f);
		tkn.set_base(line);
		vec4 pos;
		vec4 nrm;
		vec4 clr;
		vec4 tex;

		float* r = nullptr;
		for (const auto& p: psc)
		{
			switch (p.a)
			{
			case ATTRIB_POSITION: r = &pos[p.c]; break;
			case ATTRIB_NORMAL: r = &nrm[p.c]; break;
			case ATTRIB_COLOR: r = &clr[p.c]; break;
			case ATTRIB_TEXCOORD: r = &tex[p.c]; break;
			default: continue;
			}

			tkn.get_token_as(*r,tkn.whitespaces);

			if(p.n)
				*r= static_cast<float>(static_cast<double>(*r)/p.t.max());
		}

		vtx.set_value(ATTRIB_POSITION,pos);
		vtx.set_value(ATTRIB_NORMAL,nrm);
		vtx.set_value(ATTRIB_COLOR,clr);
		vtx.set_value(ATTRIB_TEXCOORD,tex);
		vd->push_back(vtx);
	}
	// read the faces

	for (uint32_t i = 0; i < face_count; i++)
	{
		getline(&line,&n,f);
		tkn.set_base(line);
		auto verts_in_this_face = atoi(tkn.get_token());

		uint32_t vert0;
		uint32_t vert1;
		uint32_t vert2;
		tkn.get_token_as(vert0,tkn.whitespaces);
		tkn.get_token_as(vert1,tkn.whitespaces);
		for (int i = 2; i < verts_in_this_face; i+=1)
		{
			tkn.get_token_as(vert2,tkn.whitespaces);
			vd->push_back(vert0);
			vd->push_back(vert1);
			vd->push_back(vert2);
			vert1 = vert2;
		}
	}

	tkn.set_base(nullptr);
	free(line);
	return vd;
}

#if 0
VertexData *VertexDataTools::readOFF(const std::string &path)
{
	FILE* f = fopen(path.c_str(),"r");
	if(!f)
		return nullptr;

	VertexData* vd = new VertexData();
	char * line = NULL;
	size_t len = 0;
	ssize_t read = 0;

	uint vtx_cnt = 0;
	uint iv = 0;
	uint face_cnt = 0;
	uint it= 0;
	Vertex v;
	Tokenizer tkn("");
	while ((read = getline(&line, &len, f)) != -1)
	{
		tkn.setBase(line);
		tkn.skipWhiteSpaces();
		tkn.skipOverAll("OFF");
		tkn.skipWhiteSpaces();
		if(!tkn.getRest() || tkn.getRest()[0] == '#' || tkn.getRest()[0] == 0)
			continue;

		if(vtx_cnt == 0 && face_cnt == 0)
		{
			tkn.getTokenAs(vtx_cnt);
			tkn.getTokenAs(face_cnt);
			vd->data().reserve(vtx_cnt);
		}
		else if(iv < vtx_cnt)
		{
			tkn.getTokenAs(v.pos().x);
			tkn.getTokenAs(v.pos().y);
			tkn.getTokenAs(v.pos().z);
			vd->push_back(v);
			iv++;
		}
		else if (it < face_cnt)
		{
			uint i = 3;
			tkn.getTokenAs(i);
			if(it == 0)
			{
				if(i == 3)
					vd->setPrimitive(TRIANGLES);
				else if(i == 4)
					vd->setPrimitive(QUADS);
				vd->indices().reserve(i*face_cnt);
			}

			for(uint j = 0; j<i;j++)
			{
				uint q = 0;
				tkn.getTokenAs(q);
				vd->push_back(q);
			}
			it++;
		}

	}
	tkn.setBase(nullptr);
	fclose(f);
	if(line)
		free(line);
	return vd;
}

#endif



bool VertexDataOperations::write_obj(const VertexData *vd, FILE* f)
{
	if(!f)
		return false;

	struct obj_vert{int p;int t;int n;};
	typedef struct{obj_vert v[3];} obj_face;

	std::map<vec3,uint32_t,Comperator<vec3>> pos2id;
	int pid = 1;
	std::map<vec3,uint32_t,Comperator<vec3>> nrm2id;
	int nid = 1;
	std::map<vec2,uint32_t,Comperator<vec2>> tex2id;
	int tid = 1;
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


	for(unsigned int i = 0 ; i< vd->index_count();i+=consumed)
	{

		uint v[4];
		if (consumed >= 3 || i%2 ==1)
		{
			for (uint j = 0; j < vs; j++)
				v[j] = vd->get_index(i+j);
		}
		else
		{
			for (uint j = 0; j < vs; j++)
			{
				const uint idx = j==0? 1 : j==1? 0:j;
				v[j] = vd->get_index(i+idx);
			}
		}

		obj_face face;
		vec3 pos;
		vec3 nrm;
		vec2 tex;
		for (uint j = 0; j < vs; j++)
		{
			const Vertex& vtx = vd->get_vertex(v[j]);
			if(vtx.get_value(AttributeID::ATTRIB_POSITION,pos))
			{
				if(pos2id.find(pos) == pos2id.end())
				{
					fprintf(f,"v %f %f %f\n",pos.x,pos.y,pos.z);
					pos2id[pos] = pid;
					face.v[j].p = pid;
					pid ++;
				}
				else
					face.v[j].p = pos2id[pos];
			}
			else
				face.v[j].p = -1;

			if(vtx.get_value(AttributeID::ATTRIB_NORMAL,nrm))
			{
				if(nrm2id.find(nrm) == nrm2id.end())
				{
					fprintf(f,"vn %f %f %f\n",nrm.x,nrm.y,nrm.z);
					nrm2id[nrm] = nid;
					face.v[j].n = nid;
					nid++;
				}
				else
					face.v[j].n = nrm2id[nrm];
			}
			else
				face.v[j].n = -1;

			if(vtx.get_value(AttributeID::ATTRIB_TEXCOORD,tex))
			{
				if(tex2id.find(tex) == tex2id.end())
				{
					fprintf(f,"vt %f %f\n",tex.x,tex.y);
					tex2id[tex] = tid;
					face.v[j].t = tid;
					tid ++;
				}
				else
					face.v[j].t = tex2id[tex];
			}
			else
				face.v[j].t = -1;
		}

		fprintf(f,"f");
		for (uint j = 0; j < vs; j++)
		{
			if(face.v[j].t >= 0)
			{
				if(face.v[j].n >= 0)
					fprintf(f," %d/%d/%d",face.v[j].p,face.v[j].t,face.v[j].n);
				else
					fprintf(f," %d/%d",face.v[j].p,face.v[j].t);
			}
			else if(face.v[j].n >= 0)
			{
				fprintf(f," %d//%d",face.v[j].p,face.v[j].n);
			}
			else
			{
				fprintf(f," %d",face.v[j].p);
			}
		}
		fprintf(f,"\n");
	}
	return true;
}

bool VertexDataOperations::write_ply(const VertexData */*vd*/, FILE*/*f*/)
{
	return false;
}

bool VertexDataOperations::write_off(const VertexData *vd, FILE* f)
{

	if(!f)
		return false;
	uint vpf = 3;
	switch (vd->primitive())
	{
	case QUADS: vpf = 4; break;
	case TRIANGLES: vpf = 3; break;
	default: return false;
	}
	fprintf(f,"OFF\n%u %u %lu\n",vd->vertex_count(),vd->index_count()/vpf,0ul);

	for(const Vertex& vtx : *vd)
	{
		vec3 v;
		vtx.get_value(AttributeID::ATTRIB_POSITION,v);
		fprintf(f,"%f %f %f\n",v.x,v.y,v.z);
	}


	for(uint i = 0 ; i<vd->index_count(); i+= vpf)
	{

		fprintf(f,"%d",vpf);
		for(uint j=0 ; j<vpf;j++)
		{
			fprintf(f," %d",vd->get_index(i+j));
		}
		fprintf(f,"\n");
	}
	fclose(f);
	return true;
}

bool VertexDataOperations::write_to_file(
		const VertexData *vd,
		const std::string &p,
		VertexDataOperations::FileFormat f)
{


	std::string ending = paths::extension(p);
	if (f == FROM_PATH)
	{
		if(ending == "obj" || ending == "OBJ")
			f = OBJ;
		else if(ending == "ply" || ending == "PLY")
			f = PLY;
		else if(ending == "vd" || ending == "VD")
			f = VD;
		else if(ending == "off" || ending == "OFF")
			f = OFF;
	}

	FILE* file = fopen(p.c_str(),"w");
	if(!file)
		return false;
	bool res = false;
	switch (f)
	{
	case VD:
		res = write_vd(vd,file); break;
	case OBJ:
		res = write_obj(vd,file);break;
	case PLY:
		res = write_ply(vd,file);break;
	case OFF:
		res = write_off(vd,file);break;
	case FROM_PATH:
		res =  false;break;
	}
	fclose(file);
	return res;
}

VertexData* VertexDataOperations::read_from_file(
		const std::string &path,
		VertexDataOperations::FileFormat f)
{
	std::string ending = paths::extension(path);
	if (f == FROM_PATH)
	{
		if(ending == "obj" || ending == "OBJ")
			f = OBJ;
		else if(ending == "ply" || ending == "PLY")
			f = PLY;
		else if(ending == "vd" || ending == "VD")
			f = VD;
		else if(ending == "off" || ending == "OFF")
			f = OFF;
	}


	FILE* file = nullptr;
	file = fopen(path.c_str(),"r");
	if(!file)
		return nullptr;
	VertexData* res = nullptr;;
	switch (f) {
	case VD:
		res= read_vd(file); break;
	case OBJ:
		res= read_obj(file); break;
	case PLY:
		res= read_ply(file);break;
	case OFF:
		//		res= readOFF(file);break;
	case FROM_PATH:
		res= nullptr; break;
	}
	fclose(file);
	return res;
}

bool VertexDataOperations::recalculate_normals(VertexData *vd, AttributeID to_attribute)
{
	if(!vd->vertex_configuration().has_attribute(to_attribute))
	{
		return false;
	}

	//std::vector<Vertex>& verts =  vd->data();
	std::map<vec3, vec3,Comperator<vec3>> p2n;

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
		for (unsigned int i = 0; i < vd->index_count(); i += consumed)
		{
			vec3 v[4];
			if (consumed >= 3 || i%2 ==1)
			{
				for (uint j = 0; j < vs; j++)
				{
					vd->get_vertex(vd->get_index(i+j)).get_value(AttributeID::ATTRIB_POSITION,v[j]);
					//v[j] = &(verts.at(indices[i+j]).pos());
				}
			}
			else
			{
				for (uint j = 0; j < vs; j++)
				{
					const uint idx = j==0? 1 : j==1? 0:j;
					vd->get_vertex(vd->get_index(i+idx)).get_value(AttributeID::ATTRIB_POSITION,v[j]);
					//					v[j] = &(verts.at(indices[i+idx]).pos());
				}
			}

			int connected = -1;
			if(vs == 4) //QUADS
			{
				if(glm::length2((v[0])-(v[2])) < glm::length2((v[1])-(v[3])))
				{
					connected = 0;
				}
			}
			/* calculate normalized normal */
			vec3 n = glm::normalize(glm::cross((v[1])- (v[0]), (v[2]) - (v[0])));
			float area = 0.5f*glm::length(glm::cross((v[1]) - (v[0]), (v[2]) - (v[0])));

			/* add the normal to each corner of the triangle */
			for (uint j = 0; j < vs; j++)
			{
				const vec3 A = (v[(j+1)%vs])- (v[j]);
				const vec3 B = (v[(j-1)%vs])- (v[j]);
				const vec3 C = (v[(j+2)%vs])- (v[j]);
				vec3& r = p2n[(v[j])];
				if(vs == 4) // for a quad triangulate first
				{
					if(static_cast<int>(j % 2) != connected)
					{
						const vec3 cr = glm::cross(A,B);
						area = 0.5f* glm::length(cr);
						n = glm::normalize(cr);
						const float w = area/((glm::dot(A,A)*glm::dot(B,B)));
						r += n*w ;
					}
					else
					{
						vec3 cr = glm::cross(A,C);
						area = 0.5f* glm::length(cr);
						n = glm::normalize(cr);
						r += n*area/((glm::dot(A,A)*glm::dot(C,C)));

						cr = glm::cross(C,B);
						area = 0.5f* glm::length(cr);
						n = glm::normalize(cr);
						r += n*area/((glm::dot(C,C)*glm::dot(B,B)));

					}
				}
				else //for a triangle use the one time normal.
				{
					const float w = area/((glm::dot(A,A)*glm::dot(B,B)));
					r += n*w ;
				}
			}
		}

		/*normalize all normals*/
		for (auto& pn : p2n)
			pn.second = glm::normalize(pn.second);

		/* set all the normals */
		vec3 pos;
		for (Vertex& v : *(vd))
		{
			v.get_value(AttributeID::ATTRIB_POSITION,pos);
			v.set_value(to_attribute,p2n[pos]);
		}
	}
	return true;
}

bool VertexDataOperations::recalculate_tangents(VertexData *vd, AttributeID to_attribute)
{
	if(!vd->vertex_configuration().has_attribute(to_attribute))
	{
		return false;
	}

	std::vector<vec3> res_tans;
	res_tans.resize(vd->vertex_count());

	vec3 v[3];
	vec3 uv[3];

	for (unsigned int i = 0; i < vd->index_count(); i+=3)
	{
		for(uint j = 0 ; j< 3;j++)
		{
			auto vtx = vd->get_vertex(vd->get_index(i + j));
			vtx.get_value(AttributeID::ATTRIB_POSITION,v[j]);
			vtx.get_value(AttributeID::ATTRIB_TEXCOORD,uv[j]);

		}

		vec3 dPos1 = v[1] - v[0];
		vec3 dPos2 = v[2] - v[0];
		vec3 dUV1 =uv[1] -uv[0];
		vec3 dUV2 =uv[2] -uv[0];


		float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

		vec3 t = (dPos1 * dUV2.y - dPos2 * dUV1.y)*r;
		//		vec3 b = (dPos2 * dUV1.x - dPos1 * dUV2.x)*r;


		for(uint j = 0 ; j< 3;j++)
			res_tans[vd->get_index(i + j)] += t;


	}

	for (uint i  =0 ; i<vd->vertex_count() ; i++ )
	{
		vd->get_vertex(i).set_value(to_attribute,glm::normalize(res_tans[i]));
	}

	return true;
}

VertexData *VertexDataOperations::reconfigure(VertexData *data, const VertexConfiguration &new_config, bool null)
{
	VertexData* res = new VertexData(data->m_render_primitive,new_config,data->m_vertex_count,data->m_index_type,data->m_index_count);
	memcpy(res->indices(),data->indices(),data->index_count()*data->m_index_type.size());

	if(null)
		res->verties_null();

	for (uint i = 0 ; i< data->m_vertex_count;i++)
	{
		auto r = res->get_vertex(i);
		auto s = data->get_vertex(i);
		vec4 attr;
		for(uint32_t a = 0 ; a < s.cfg.attribute_count();a++)
		{
			auto aid = s.cfg.get_attribute(a).attribute_id;
			if(r.has_attribute(aid))
			{
				auto& att = r.cfg.get_attribute(aid);
				if(att.use_constant)
					att.write_constant(attr);
				else
				{
					s.get_value(aid,attr);
					r.set_value(aid,attr);
				}
			}
		}
	}

	return res;
}
}
