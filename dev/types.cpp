#include "types.h"
namespace ofl
{

Type::Type(const std::string &str)
{
	if(str == "float" || str == "FLOAT"|| str == "single")
		id = FLOAT;
	else if(str == "double" || str == "DOUBLE")
		id = DOUBLE;
	else if(str == "int" || str == "INT")
		id = INT;
	else if(str == "uint" || str == "UNSIGNED_INT" || str == "unsigned int" || str == "UNSIGNED INT")
		id = UNSIGNED_INT;

	else if(str == "short" || str == "SHORT")
		id = SHORT;
	else if(str == "ushort" || str == "UNSIGNED_SHORT" || str == "unsigned short" || str == "UNSIGNED SHORT")
		id = UNSIGNED_SHORT;
	else if(str == "char" || str == "byte"|| str == "BYTE")
		id = BYTE;
	else
		id = UNSIGNED_BYTE;
}

std::string Type::to_string()
{
	switch (id)
	{
	case TypeID::BYTE: return  "char";
	case TypeID::UNSIGNED_BYTE: return  "uchar";
	case TypeID::SHORT: return  "short";
	case TypeID::UNSIGNED_SHORT: return  "ushort";
	case TypeID::INT: return  "int";
	case TypeID::UNSIGNED_INT: return  "uint";
	case TypeID::FLOAT: return  "float";
	case TypeID::DOUBLE: return  "double";
	case INVALID: return "INVALID";
	}
	return  "";
}

size_t Type::size() const
{
	switch (id)
	{
	case BYTE:
	case UNSIGNED_BYTE:
		return  1;
	case SHORT:
	case UNSIGNED_SHORT:
		return  2;
	case INT:
	case UNSIGNED_INT:
	case FLOAT:
		return  4;
	case DOUBLE:
		return  8;
	case INVALID: static_assert(true,"unknown type!");
	}
	return  0;
}

double Type::max() const
{
	switch (id)
	{
	case BYTE: return std::numeric_limits<int8_t>::max();
	case UNSIGNED_BYTE: return std::numeric_limits<uint8_t>::max();
	case SHORT: return std::numeric_limits<int16_t>::max();
	case UNSIGNED_SHORT: return std::numeric_limits<uint16_t>::max();
	case INT: return std::numeric_limits<int32_t>::max();
	case UNSIGNED_INT: return std::numeric_limits<uint32_t>::max();
	case FLOAT: return static_cast<double>(std::numeric_limits<float>::max());
	case DOUBLE: return std::numeric_limits<double>::max();
	case INVALID: static_assert(true,"unknown type!");
	}
	return  0;
}

double Type::min() const
{
	switch (id)
	{
	case BYTE: return std::numeric_limits<int8_t>::min();
	case UNSIGNED_BYTE: return std::numeric_limits<uint8_t>::min();
	case SHORT: return std::numeric_limits<int16_t>::min();
	case UNSIGNED_SHORT: return std::numeric_limits<uint16_t>::min();
	case INT: return std::numeric_limits<int32_t>::min();
	case UNSIGNED_INT: return std::numeric_limits<uint32_t>::min();
	case FLOAT: return static_cast<double>(std::numeric_limits<float>::min());
	case DOUBLE: return std::numeric_limits<double>::min();
	case INVALID: static_assert(true,"unknown type!");
	}
	return  0;
}
}
