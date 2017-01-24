#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include "vmath.h"

#include "stru.h"
namespace ofl
{
class TypeNameValueBlock
{
protected:
	std::map<std::string,std::vector<int32_t>> m_ints;
	std::map<std::string,std::vector<uint32_t>> m_uints;
	std::map<std::string,std::vector<float>> m_floats;
	std::map<std::string,std::vector<vec4>> m_vec4s;
	std::map<std::string,std::vector<mat4>> m_mat4s;
	std::map<std::string,std::vector<std::string>> m_strings;
	std::map<std::string, std::map<std::string, TypeNameValueBlock*>> m_blocks;

	std::string m_type;
	std::string m_name;
public:
	TypeNameValueBlock(const std::string& type, const std::string& name)
		:m_type(type), m_name(name)
	{
	}

	template<typename T>
	void set(const std::string& name, const T v, const uint32_t id =0)
	{
		auto& a =getArray<T>(name);
		if(id< a.size())
			a.resize(id+1,0);
		a[id] = v;
	}


	template<typename T>
	T& get(const std::string& name, const uint32_t id =0)
	{
		auto& a =getArray<T>(name);
		if(id< a.size())
			a.resize(id+1,0);
		return a[id];
	}

	template<typename T>
	T& get_or_set(const std::string& name,const T& default_value, const uint32_t id =0)
	{
		auto& a =getArray<T>(name);
		if(id< a.size())
		{
			a.resize(id+1,0);
			a[id] = default_value;
		}
		return a[id];
	}

	template<typename T>
	std::vector<T>& getArray(const std::string& name);

	std::map<std::string, TypeNameValueBlock*>& getBlocks(const std::string& type)
	{
		return m_blocks[type];
	}

	TypeNameValueBlock* getBlock(const std::string& type, const std::string& name)
	{
		if(m_blocks[type].find(name) == m_blocks[type].end())
			m_blocks[type][name] = new TypeNameValueBlock(type,name);
		return m_blocks[type][name];
	}

	bool has_block(const std::string& type, const std::string& name);

	bool has_blocks_of_type(const std::string& type);

	std::string get_type() const;
	std::string get_name() const;

};





template<>
inline std::vector<int32_t>& TypeNameValueBlock::getArray(const std::string& name)
{
	return m_ints[name];
}

template<>
inline std::vector<uint32_t>& TypeNameValueBlock::getArray(const std::string& name)
{
	return m_uints[name];
}
template<>
inline std::vector<float>& TypeNameValueBlock::getArray(const std::string& name)
{
	return m_floats[name];
}
template<>
inline std::vector<vec4>& TypeNameValueBlock::getArray(const std::string& name)
{
	return m_vec4s[name];
}

template<>
inline std::vector<mat4>& TypeNameValueBlock::getArray(const std::string& name)
{
	return m_mat4s[name];
}

template<>
inline std::vector<std::string>& TypeNameValueBlock::getArray(const std::string& name)
{
	return m_strings[name];
}


TypeNameValueBlock* load_TNVB_from_string(char* s, const std::string& btype="", const std::string& bname = "");
}
