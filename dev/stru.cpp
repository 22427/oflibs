#include "stru.h"

namespace ofl
{
Tokenizer::Tokenizer(const std::string& base)
{
	this->m_base = new char[base.length() + 1];
	memcpy(this->m_base, base.data(), base.length() + 1);
	this->m_rest = this->m_base;
	m_delete_base = true;
}

Tokenizer::Tokenizer(char *base)
{
	m_delete_base = false;
	m_base = m_rest = base;
}

Tokenizer::~Tokenizer()
{
	if(m_delete_base && m_base)
		delete[] m_base;
}


char* Tokenizer::get_token(char separator)
{
	char* to_ret = m_rest;

	if (*m_rest == 0)
		return nullptr;

	while (*m_rest && *m_rest != separator)
	{
		m_rest++;
	}

	while (*m_rest && *m_rest == separator)
	{
		*m_rest =0;
		m_rest++;
	}
	return to_ret;
}

bool contains(const std::string& str, const char c)
{
	for(const auto& cc : str)
	{
		if(cc == c)
		{
			return true;
		}
	}
	return false;
}
char* Tokenizer::get_token(const std::string& separators, char* sep)
{
	char* to_ret = m_rest;

	if (*m_rest == 0)
		return nullptr;

	while (*m_rest && !contains(separators,*m_rest))
	{
		m_rest++;
	}

	if(sep)
		*sep = *m_rest;

	while (*m_rest && contains(separators,*m_rest))
	{
		*m_rest = 0;
		m_rest++;
	}

	return to_ret;
}


void Tokenizer::skip_over_all(const std::string& seps)
{
	while(*m_rest && contains(seps,*m_rest))
	{
		m_rest++;
	}
}
void Tokenizer::skip_white_spaces()
{
	while(*m_rest && isspace(*m_rest))
		m_rest++;
}


void Tokenizer::reset(const std::string& base)
{
	if(m_delete_base && m_base)
		delete[] m_base;
	m_delete_base  = true;
	this->m_base = new char[base.length() + 1];
	memcpy(this->m_base, base.data(), base.length() + 1);
	this->m_rest = this->m_base;
}


std::string Tokenizer::whitespaces = " \t\n\v\f\r";
}
