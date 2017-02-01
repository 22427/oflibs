#pragma once
#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif
#include OFL_GL_HEADER
#include <map>
#include <vector>
#include "vmath.h"



namespace ofl
{

#ifdef NDEBUG
#define OFL_DBG_ERROR_CHECK do{}while(0)
#else
#define OFL_DBG_ERROR_CHECK do {const auto err = glGetError();if(err != GL_NO_ERROR) printf("[ERR] %s - %d: %s\n",__FILE__,__LINE__,get_gl_error_string(err));}while(0)
#endif


std::string get_gl_error_string(const GLenum err)
{
	switch (err)
	{
	case GL_NO_ERROR: return "GL_NO_ERROR";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	}
	return "UNKNOWN_GL_ERROR";
}


class Shader
{
	GLuint m_handle;
public:
	friend class ShaderProgram;
	Shader(GLenum type) : m_handle(0)
	{
		m_handle = glCreateShader(type);
	}

	Shader(GLenum type,const std::string& code) : m_handle(0)
	{
		m_handle = glCreateShader(type);
		if(m_handle)
			set_code(code);
	}

	~Shader()
	{
		if(m_handle)
			glDeleteShader(m_handle);
	}

	void set_code(const std::string& code)
	{
		const char* c = code.c_str();
		glShaderSource(m_handle,1,&c,nullptr);
	}

	std::string read_log()
	{
		GLint length = 0;
		glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> error_log(length);
		glGetShaderInfoLog(m_handle, length, &length, &error_log[0]);
		return error_log.data();
	}

	bool compile()
	{
		glCompileShader(m_handle);
		GLint is_compiled = 0;
		glGetShaderiv(m_handle, GL_COMPILE_STATUS, &is_compiled);
		return  is_compiled == GL_TRUE;
	}
	const GLuint& handle()const {return m_handle;}
};

class ShaderProgram
{
	GLuint m_handle;
	std::map<int, int> m_uniformID2location;
public:
	ShaderProgram()
	{
		m_handle = glCreateProgram();
	}
	~ShaderProgram()
	{
		if(m_handle)
			glDeleteProgram(m_handle);
	}

	void use()
	{
		glUseProgram(m_handle);
	}

	void bind_attribute_location(const GLuint loc, const std::string& name)
	{
		glBindAttribLocation(m_handle,loc,name.c_str());
	}

	void attach_shader(const Shader& s)
	{
		glAttachShader(m_handle,s.m_handle);
	}

	bool link()
	{
		glLinkProgram(m_handle);
		GLint was_linked ;
		glGetProgramiv(m_handle, GL_LINK_STATUS, &was_linked);

		return  was_linked == GL_TRUE;
	}

	std::string read_log()
	{
		GLint length = 0;
		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> error_log(length);
		glGetProgramInfoLog(m_handle, length, &length, &error_log[0]);
		return  error_log.data();

	}

	int get_uniform_location(int id)
	{
		auto it = m_uniformID2location.find(id);
		if( it != m_uniformID2location.end())
			return (*it).second;
		else
			return  -1;
	}

	int get_uniform_location(const std::string& uniform_name)
	{
		return glGetUniformLocation(m_handle,uniform_name.c_str());
	}

	void learn_uniform_location(const std::string& name, const int id)
	{
		m_uniformID2location[id] = get_uniform_location(name);
	}

	const GLuint& handle()const {return m_handle;}
};
}
