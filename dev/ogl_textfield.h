#pragma once

#include "dll.h"
#include "ogl_utils.h"
#include <algorithm>
namespace ofl
{
	class OpenGLTextField
	{
		GLuint m_texture;
		GLuint m_buffer;

		GLuint m_ubo;

		struct uniform_data
		{
			ivec2 chars;
			vec2 char_size;
			vec4 font_color;
			vec4 bg_color;
			mat4 mmtx;
		} ud;
		bool m_ud_dirty;

		void upload_ud()
		{
			if(m_ud_dirty)
			{
				m_ud_dirty = false;

				glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);
				glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(ud),&ud);
			}
		}

	public:
		friend class OpenGLTextFieldRenderer;

		~OpenGLTextField()
		{
			glDeleteTextures(1,&m_texture);
			glDeleteBuffers(1,&m_buffer);
			glDeleteBuffers(1,&m_ubo);
		}
		OpenGLTextField(const GLuint width_c, const GLuint height_c, const float cw,const  float ch)

		{
			m_ud_dirty = true;

			ud.chars = ivec2(width_c,height_c);
			ud.char_size = vec2(cw,ch);
			ud.font_color = vec4(1);
			ud.bg_color = vec4(0,0,0,1);


			glGenBuffers(1,&m_ubo);

			glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);
			glBufferData(GL_UNIFORM_BUFFER,sizeof(ud),nullptr,GL_STATIC_DRAW);
			upload_ud();


			glGenBuffers(1,&m_buffer);

			glBindBuffer(GL_ARRAY_BUFFER,m_buffer);
			glBufferData(GL_ARRAY_BUFFER,ud.chars.x*ud.chars.y,nullptr,GL_DYNAMIC_DRAW);
			auto b = glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
			memset(b,0,ud.chars.x*ud.chars.y);
			glUnmapBuffer(GL_ARRAY_BUFFER);


			glBindBuffer(GL_TEXTURE_BUFFER,m_buffer);
			glGenTextures(1,&m_texture);
			// bind the texture object
			glBindTexture(GL_TEXTURE_BUFFER,m_texture);
			// connect the texture object with the buffer. Note this is part of the objects state!
			glTexBuffer(GL_TEXTURE_BUFFER,GL_R8UI,m_buffer);

		}
		void resize(const ivec2& chars)
		{
			ud.chars = chars;
			glBindBuffer(GL_TEXTURE_BUFFER,m_buffer);
			glBufferData(GL_TEXTURE_BUFFER,ud.chars.x*ud.chars.y,nullptr,GL_DYNAMIC_DRAW);
			m_ud_dirty = true;
		}

		void set_font_color(const vec4& color)
		{
			ud.font_color= color;
			m_ud_dirty = true;
		}

		void set_bg_color(const vec4& color)
		{
			ud.bg_color= color;
			m_ud_dirty = true;
		}

		void set_char_size(const vec2& size)
		{
			ud.char_size= size;
			m_ud_dirty = true;
		}

		void set_char_width(const float width)
		{
			float a = ud.char_size.y/ud.char_size.x;
			ud.char_size.x = width;
			ud.char_size.y =  a * width;
			m_ud_dirty = true;
		}

		void set_char_height(const float height)
		{
			float a = ud.char_size.x/ud.char_size.y;
			ud.char_size.x =  a * height;
			ud.char_size.y = height;
			m_ud_dirty = true;
		}

		void write_text(const std::string& str,const GLuint from = 0)
		{
			if(from >= ud.chars.x*ud.chars.y)
				return;

			glBindBuffer(GL_TEXTURE_BUFFER,m_buffer);
			char* b = static_cast<char*>(glMapBuffer(GL_TEXTURE_BUFFER,GL_WRITE_ONLY));
			memcpy(b+from,str.c_str(),std::min(ud.chars.x*ud.chars.y-from,static_cast<GLuint>(str.length())));
			glUnmapBuffer(GL_TEXTURE_BUFFER);
		}
	};

	class OpenGLTextFieldRenderer
	{
		ShaderProgram sp;
		GLuint m_font_texture;
		GLint font_texture_uloc;
		GLint text_buffer_uloc;
		std::vector<OpenGLTextField*> m_tfs;
		float m_font_ar;
	public:
		OpenGLTextFieldRenderer(void* font, const uint font_w, const uint font_h,const uint font_c =1,int font_type = 0);

		OpenGLTextField* create_text_field(const GLuint width, const GLuint height)
		{
			m_tfs.push_back(new OpenGLTextField(width,height,m_font_ar*0.2f,0.2f));
			return m_tfs.back();
		}
		void render()
		{
			sp.use();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_font_texture);
			glActiveTexture(GL_TEXTURE0);
			for(auto tf : m_tfs)
			{
				glBindTexture(GL_TEXTURE_BUFFER, tf->m_texture);
				tf->upload_ud();
				glBindBufferBase(GL_UNIFORM_BUFFER,0,tf->m_ubo);
				glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,4,tf->ud.chars.x*tf->ud.chars.y);

			}
		}
	};
}
