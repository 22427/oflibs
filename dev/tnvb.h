#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include "vmath.h"

#include "stru.h"
#pragma once
#include <cstdio>
#include <map>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "types.h"
#include "dll.h"
namespace ofl {

class OFL_DLL_PUBLIC TNVB
{
protected:
	std::map<std::string,std::vector<int32_t>> m_ints;
	std::map<std::string,std::vector<uint32_t>> m_uints;
	std::map<std::string,std::vector<float>> m_floats;
	std::map<std::string,std::vector<double>> m_doubles;
	std::map<std::string,std::vector<vec2>> m_vec2s;
	std::map<std::string,std::vector<vec3>> m_vec3s;
	std::map<std::string,std::vector<vec4>> m_vec4s;
	std::map<std::string,std::vector<mat3>> m_mat3s;
	std::map<std::string,std::vector<mat4>> m_mat4s;
	std::map<std::string,std::vector<std::string>> m_strings;
	std::map<std::string, std::map<std::string, TNVB*>> m_blocks;

	std::string m_type;
	std::string m_name;
public:
	TNVB(const std::string& type="", const std::string& name="");
	~TNVB()
	{
		// Destroy all child blocks
		for(auto& t: m_blocks)
			for(auto& n : t.second)
				delete n.second;
		m_blocks.clear();
	}

	/**
	 * @brief get_type gives access to this blocks type.
	 * @return this blocks type name.
	 */
	const std::string& type() const;
	/**
	 * @brief get_name gives acces to this blocks name
	 * @return the name of this block.
	 */
	const std::string& name() const;


	/**
	 * @brief get_type gives access to this blocks type.
	 * @return this blocks type name.
	 */
	std::string& type() ;
	/**
	 * @brief get_name gives acces to this blocks name
	 * @return the name of this block.
	 */
	std::string& name() ;


	/**
	 * @brief set Sets the id'th item in the array name.
	 * Note:
	 * If there are viewer then id+1 items, the array will be filled to set the
	 * id'th item.
	 * @param name of the array.
	 * @param v new value.
	 * @param id item id.
	 */
	template<typename T>
	void set(const std::string& name, const T v, const uint32_t id =0)
	{
		auto& a =get_array<T>(name);
		if(id >= a.size())
			a.resize(id+1);
		a[id] = v;
	}


	/**
	 * @brief get Gets the id'th item in the array name.
	 * Note:
	 * If there are viewer then id+1 items, the array will be filled to set the
	 * id'th item and an item with the default value will be returned.
	 * @param name of the array.
	 * @param id item id.
	 */
	template<typename T>
	T& get(const std::string& name, const uint32_t id =0)
	{
		auto& a =get_array<T>(name);
		if(id >= a.size())
			a.resize(id+1);
		return a[id];
	}

	/**
	 * @brief get_or_set returns the id'th value in the array with name name.
	 * If it does not exist it is initialized with default_value and returned.
	 * @param name of the array.
	 * @param default_value of the id'th element.
	 * @param id of the requested item.
	 * @return
	 */
	template<typename T>
	T& get_or_set(const std::string& name,const T& default_value, const uint32_t id =0)
	{
		auto& a =get_array<T>(name);
		if(id<= a.size())
		{
			a.resize(id+1,0);
			a[id] = default_value;
		}
		return a[id];
	}

	/**
	 * @brief get_array Gives access to an array with a given name.
	 * If the array with this name does not exist one is created.
	 * @param name name of the array.
	 * @return Reference to the array.
	 */
	template<typename T>
	std::vector<T>& get_array(const std::string& name);


	/**
	 * @brief get_array Gives access to an array with a given name.
	 * If the array with this name does not exist one is created.
	 * @param name name of the array.
	 * @return Reference to the array.
	 */
	template<typename T>
	const std::vector<T>& get_array(const std::string& name) const;

	/**
	 * @brief get_blocks Gives access to all blocks of a given type
	 * @param type of the blocks you wish to access.
	 * @return A map from block names to the actual blocks.
	 */
	std::map<std::string, TNVB*>& get_blocks(const std::string& type);

	/**
	 * @brief get_block Gives access to the block with the type and name
	 * @param type of the block.
	 * @param name of the block.
	 * @return The requested block. If it did not exist one will be created and
	 * returned.
	 */
	TNVB *&get_block(const std::string& type, const std::string& name);

	/**
	 * @brief has_block check if a block with a given type and name exists.
	 * @param type of the block.
	 * @param name of the block.
	 * @return true if the block is a subblock of this one. False if not.
	 */
	bool has_block(const std::string& type, const std::string& name);

	/**
	 * @brief has_blocks_of_type check if any block with a given type exists.
	 * @param type of the block.
	 * @return true if there are blocks of type in this one. False if not.
	 */
	bool has_blocks_of_type(const std::string& type);

	bool has_ints(const std::string& name) const {return m_ints.find(name) != m_ints.end();}
	bool has_uints(const std::string& name) const {return m_uints.find(name) != m_uints.end();}
	bool has_floats(const std::string& name) const {return m_floats.find(name) != m_floats.end();}
	bool has_doubles(const std::string& name) const {return m_doubles.find(name) != m_doubles.end();}
	bool has_vec2s(const std::string& name) const {return m_vec2s.find(name) != m_vec2s.end();}
	bool has_vec3s(const std::string& name) const {return m_vec3s.find(name) != m_vec3s.end();}
	bool has_vec4s(const std::string& name) const {return m_vec4s.find(name) != m_vec4s.end();}
	bool has_mat3s(const std::string& name) const {return m_mat3s.find(name) != m_mat3s.end();}
	bool has_mat4s(const std::string& name) const {return m_mat4s.find(name) != m_mat4s.end();}
	bool has_strings(const std::string& name) const {return m_strings.find(name) != m_strings.end();}




	const std::map<std::string,std::vector<int32_t>>& get_ints() const;
	const std::map<std::string,std::vector<uint32_t>>& get_uints() const;
	const std::map<std::string,std::vector<float>>& get_floats() const;
	const std::map<std::string,std::vector<double>>& get_doubles() const;
	const std::map<std::string,std::vector<vec2>>& get_vec2s() const;
	const std::map<std::string,std::vector<vec3>>& get_vec3s() const;
	const std::map<std::string,std::vector<vec4>>& get_vec4s() const;
	const std::map<std::string,std::vector<mat4>>& get_mat4s() const;
	const std::map<std::string,std::vector<mat3>>& get_mat3s() const;
	const std::map<std::string,std::vector<std::string>>& get_strings ()const;
	const std::map<std::string, std::map<std::string, TNVB*> >& get_blocks ()const;


};

class TNVBOperations
{
public:
	static OFL_DLL_PUBLIC TNVB *read_from_string(const char* code,
												 const char** end,
												 const std::string& type ="",
												 const std::string& name ="" );
	static OFL_DLL_PUBLIC  std::string write_to_string(const TNVB& v);


	static OFL_DLL_PUBLIC TNVB *read_from_file(const std::string& path);
	static OFL_DLL_PUBLIC bool write_to_file(const TNVB* tnvb ,const std::string& path);
};



template<>
inline std::vector<int32_t>& TNVB::get_array(const std::string& name)
{
	return m_ints[name];
}

template<>
inline std::vector<uint32_t>& TNVB::get_array(const std::string& name)
{
	return m_uints[name];
}
template<>
inline std::vector<float>& TNVB::get_array(const std::string& name)
{
	return m_floats[name];
}
template<>
inline std::vector<double>& TNVB::get_array(const std::string& name)
{
	return m_doubles[name];
}
template<>
inline std::vector<vec2>& TNVB::get_array(const std::string& name)
{
	return m_vec2s[name];
}
template<>
inline std::vector<vec3>& TNVB::get_array(const std::string& name)
{
	return m_vec3s[name];
}
template<>
inline std::vector<vec4>& TNVB::get_array(const std::string& name)
{
	return m_vec4s[name];
}
template<>
inline std::vector<mat4>& TNVB::get_array(const std::string& name)
{
	return m_mat4s[name];
}

template<>
inline std::vector<mat3>& TNVB::get_array(const std::string& name)
{
	return m_mat3s[name];
}
template<>
inline std::vector<std::string>& TNVB::get_array(const std::string& name)
{
	return m_strings[name];
}



template<>
inline const std::vector<int32_t>& TNVB::get_array(const std::string& name) const
{
	return (*(m_ints.find(name))).second;
}

template<>
inline const std::vector<uint32_t>& TNVB::get_array(const std::string& name) const
{
	return (*(m_uints.find(name))).second;
}
template<>
inline const std::vector<float>& TNVB::get_array(const std::string& name) const
{
	return (*(m_floats.find(name))).second;
}
template<>
inline const std::vector<double>& TNVB::get_array(const std::string& name) const
{
	return (*(m_doubles.find(name))).second;
}
template<>
inline const std::vector<vec2>& TNVB::get_array(const std::string& name) const
{
	return (*(m_vec2s.find(name))).second;
}
template<>
inline const std::vector<vec3>& TNVB::get_array(const std::string& name) const
{
	return (*(m_vec3s.find(name))).second;
}
template<>
inline const std::vector<vec4>& TNVB::get_array(const std::string& name) const
{
	return (*(m_vec4s.find(name))).second;
}
template<>
inline const std::vector<mat4>& TNVB::get_array(const std::string& name) const
{
	return (*(m_mat4s.find(name))).second;
}

template<>
inline const std::vector<mat3>& TNVB::get_array(const std::string& name) const
{
	return (*(m_mat3s.find(name))).second;
}
template<>
inline const std::vector<std::string>& TNVB::get_array(const std::string& name) const
{
	return (*(m_strings.find(name))).second;
}





}
