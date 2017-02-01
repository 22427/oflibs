#pragma once

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <iostream>
#include <ostream>
#include <istream>
#include <fstream>
#include <cstring>
#include <cmath>

#include "dll.h"
#include "vmath.h"
namespace ofl
{

/**
 * @brief ltrim removes all whitespaces on the left side of s
 * @param s
 */
static inline void ltrim(std::string &s)
{
	s.erase(s.begin(),std::find_if(s.begin(), s.end(),
								   std::not1(std::ptr_fun<int, int>(std::isspace))));
}

/**
 * @brief ltrim removes all whitespaces on the right side of s
 * @param s
 */
static inline void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
						 std::not1(std::ptr_fun<int,int>(std::isspace))).base(),
			s.end());
}

/**
 * @brief ltrim removes all whitespaces on the both sides of s
 * @param s
 */
static inline void trim(std::string &s){ltrim(s);rtrim(s);}



/**
 * @brief The Tokenizer class is a tool to parse strings. It works linke strtok,
 * but it has a local state.
 * It works by a simple principle. There is a head pointer. Everytime you ask
 * for a token. the current head will be returned, and the first appearing
 * seperator character is searched and set to zero.
 * NOTE: Only _ONE_ character will be set to zero. Even if there are three
 * seperators in a row, only the first one will be set to zero.
 */
class OFL_DLL_PUBLIC Tokenizer
{
protected:
	char* m_base;
	char* m_rest;
	bool m_delete_base;
public:
	static std::string whitespaces;
	Tokenizer(const std::string& base);
	Tokenizer(char* base);
	~Tokenizer();

	void set_base(char* base)
	{
		if(m_delete_base)
			free(base);
		m_base = base;
		m_rest = base;
		m_delete_base = false;
	}
	/**
	 * @brief reset Will free the current base and set a new one.
	 * @param base The new base.
	 */
	void reset(const std::string& base);

	/**
	 * @brief getToken Will return the string until and without the seperator
	 * character:
	 * For example:
	 * "foo,bar..." ---getToken(',')--> "foo"
	 *
	 * @param separator The seperator character
	 * @return string untill the first appearence of seperator, or nullptr.
	 */
	char* get_token(char separator);

	/**
	 * @brief Will return the string till and without one! of the seperators!
	 * "foo;,.bar..." ---getToken(",.;",c)--> "foo",c=;
	 * @param separators String contianing all possible seperatos.
	 * @param sep Will contain the seperator actually found.
	 * @return string till the first appearence of a seperator, or nullptr.
	 */
	char* get_token(const std::string& separators = whitespaces,
					char* sep = nullptr);


	/**
	 * @brief getTokenAs Will read a string untill one of the seperators appear
	 * and return it interpreted.
	 * @param separators String contianing all possible seperatos.
	 * @param sep Will contain the seperator actually found.
	 * @return
	 */
	template<typename T>
	bool get_token_as(T& res,const std::string &separators = whitespaces,
					  char *sep = nullptr)
	{
		throw "TYPE NOT SUPPORTED!!!";
		return false;
	}

	/**
	 * @brief skipOverAll Skipps all consecutive appearences of char in seps.
	 * Example:
	 * ".,;..:,;,,.foo...." ---skipOverAll(";.,:")--> "foo...."
	 * @param separators String contianing all possible seperatos.
	 */
	void skip_over_all(const std::string& seps);

	/**
	 * @brief skipOverAll Skipps all consecutive appearences of whitespaces.
	 * Example:
	 * "           foo...." ---skipWhiteSpaces()--> "foo...."
	 */
	void skip_white_spaces();

	/**
	 * @brief getRest The remaining string,
	 * @return
	 */
	char* get_rest(){ return m_rest; }


	/**
	 * @brief readEscString munches off an escaped string.
	 * >"hello \"World\" !"< -> >hello "World" !<
	 * @return
	 */
	bool read_esc_string(std::string& res)
	{
		std::string r;
		while(*m_rest != '"')
			m_rest++;
		if(!*m_rest)
			return false;
		while(*m_rest!= '"' && *m_rest)
		{
			if(*m_rest == '\\')
				m_rest++;
			r+=*m_rest;
			m_rest++;
		}

		if(!*m_rest)
			return false;

		res = r;
		return true;
	}

	/**
	 * @brief getTokenTillClosing Returns the inlay of an area marked by opening and closing chars:
	 * "  { 1,2,{3}}" --getTokenTillClosing('{','}')--> " 1,2,{3}"
	 * @param opening
	 * @param closing
	 * @return
	 */
	char* get_token_till_closing(char opening, char closing)
	{

		while(*m_rest && *m_rest!=opening)
		{
			m_rest++;
		}
		m_rest++;
		char* res = m_rest;
		if(!m_rest)
			return nullptr;


		int count = 1;
		while(count > 0)
		{
			m_rest++;
			if(!*m_rest)
				return nullptr;
			if(*m_rest == opening)
				count++;
			if(*m_rest == closing)
				count--;
		}

		if(!*m_rest)
			return nullptr;
		*m_rest = 0;
		m_rest++;
		return res;
	}
};


template<> inline bool Tokenizer::get_token_as<int>(
		int& res,
		const std::string &seps,
		char *sep )
{
	char* c = get_token(seps,sep);
	if(c)
		res = atoi(c);
	return c;
}

template<> inline bool Tokenizer::get_token_as<uint>(
		uint& res,
		const std::string &seps,
		char *sep )
{
	char* c = get_token(seps,sep);
	if(c)
		res = static_cast<uint>(atoi(c));
	return c;
}

template<> inline bool Tokenizer::get_token_as<float>(
		float& res,
		const std::string &seps ,
		char *sep )
{
	char* c = get_token(seps,sep);
	if(c)
		res = atof(c);
	return c;
}


template<> inline bool Tokenizer::get_token_as<double>(
		double& res,
		const std::string &seps ,
		char *sep )
{
	char* c = get_token(seps,sep);
	if(c)
		res = atof(c);
	return c;
}

template<> inline bool Tokenizer::get_token_as<bool>(
		bool& res,
		const std::string &seps ,
		char *sep )
{
	char* c=  get_token(seps,sep);
	if(c)
		res = strcmp(c,"false");
	return c;
}


template<> inline bool Tokenizer::get_token_as<glm::vec4>(
		glm::vec4& res,
		const std::string &seps,
		char *sep )
{
	// should look like "(x,y,z,w)"
	char* c = get_token(seps,sep);
	if(!c)
		return  false;

	Tokenizer t(c);

	t.get_token('(');
	bool r =  t.get_token_as(res.x,",") && t.get_token_as(res.y,",") && t.get_token_as(res.z,",") && t.get_token_as(res.w,")");
	t.set_base(nullptr);
	return r;
}


template<> inline bool Tokenizer::get_token_as<glm::mat4>(
		glm::mat4& res,
		const std::string &seps,
		char *sep )
{
	// should look like "((x,y,z,w),(x,y,z,w),(x,y,z,w),(x,y,z,w))"
	char* c = get_token(seps,sep);
	if(!c)
		return  false;

	Tokenizer t(c);

	t.get_token('(');
	bool r =  t.get_token_as(res[0],",") && t.get_token_as(res[1],",") && t.get_token_as(res[2],",") && t.get_token_as(res[3],")");
	t.set_base(nullptr);
	return r;
}

inline std::string load_file(const std::string& path)
{
	std::ifstream t(path);
	if(!t.is_open())
	{
		return "";
	}
	std::string str((std::istreambuf_iterator<char>(t)),
					 std::istreambuf_iterator<char>());
	t.close();
	return str;

}


namespace paths
{


static inline bool is_directory(const std::string& p)
{
	return p.at(p.length()-1) == '/';
}
static inline bool is_relative(const std::string& p)
{
	if(p.length()<2)
		return false;
	return p.at(0) != '/' && p.at(1) != ':';
}

static inline std::string file(const std::string& p)
{
	if(is_directory(p))
		return "";

	return p.substr(p.find_last_of('/')+1);
}

static inline std::string filename(const std::string& p)
{
	if(is_directory(p))
		return "";
	auto locd = p.find_last_of('.');
	auto locs =  p.find_last_of('/');
	if(locs == p.npos)
		locs = 0;
	else
		locs++;
	if(locd < locs)
		locd = p.npos;

	return p.substr(locs,locd-locs);
}

static inline std::string extension(const std::string& p)
{
	if(is_directory(p))
		return "";
	auto loc = p.find_last_of('.');
	auto sloc = loc > p.find_last_of('/');
	if(loc > sloc || sloc == p.npos)
		return p.substr(loc+1);
	return "";
}

static inline std::string without_extension(const std::string& p)
{
	if(is_directory(p))
		return p;
	auto loc = p.find_last_of('.');
	return p.substr(0,loc);
}

}
}
