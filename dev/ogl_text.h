#pragma once

#include "dll.h"
#include "ogl_utils.h"
#include <algorithm>

namespace ofl
{
	class OpenGLTextField
	{
		GLuint m_vao;
		GLuint m_buffer;

		GLuint m_ubo;

		struct uniform_data
		{
			ivec2 chars;
			vec2 char_size;
			vec2 anchor;
			vec2 padding;
			vec4 font_color;
			vec4 bg_color;
			mat4 mmtx;
		} ud;
		bool m_ud_dirty;

		void upload_ud();

	public:
		friend class OpenGLTextFieldRenderer;

		~OpenGLTextField();
		OpenGLTextField(const GLuint width_c, const GLuint height_c, const float cw,const  float ch);
		void resize(const ivec2& chars);
		void set_font_color(const vec4& color);
		void set_bg_color(const vec4& color);
		void set_char_size(const vec2& size);
		void set_anchor(const vec2& an);
		void set_char_width(const float width);
		void set_char_height(const float height);
		void set_model_matrix(const mat4& mtx);
		void write_text(const std::string& str,const GLint from = 0);
	};

	class OpenGLTextFieldRenderer
	{

		ShaderProgram m_shader_program;
		GLuint m_font_texture;
		std::vector<OpenGLTextField*> m_tfs;
		float m_font_ar;

		// uniform data
		bool m_uniforms_dirty;
		mat4 m_view_projection_matrix;	// loc = 1
		int m_font_texture_unit;		// loc = 2

	public:
		OpenGLTextFieldRenderer(const int font_texture_unit = 1);
		OpenGLTextFieldRenderer(const void* font, const uint font_w, const uint font_h,const uint font_c =1, const int font_texture_unit = 1);

		OpenGLTextField* create_text_field(const GLuint width_c, const GLuint height_c,const float char_height=0.05)
		{

			m_tfs.push_back(new OpenGLTextField(width_c,height_c,m_font_ar*char_height,char_height));
			return m_tfs.back();
		}

		void set_view_projection_matrix(const mat4& vpm)
		{
			m_view_projection_matrix =vpm;
			m_uniforms_dirty = true;
		}
		void render()
		{
			m_shader_program.use();

			if(m_uniforms_dirty)
			{
				m_uniforms_dirty = false;
				glUniformMatrix4fv(1,1,GL_FALSE,glm::value_ptr(m_view_projection_matrix));
			}
			if(m_font_texture_unit>=0)
			{
				glActiveTexture(GL_TEXTURE0+m_font_texture_unit);
				glBindTexture(GL_TEXTURE_2D, m_font_texture);
			}
			for(auto tf : m_tfs)
			{
				glBindVertexArray(tf->m_vao);
				tf->upload_ud();
				glBindBufferBase(GL_UNIFORM_BUFFER,0,tf->m_ubo);
				glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,4,tf->ud.chars.x*tf->ud.chars.y);

			}
			OFL_DBG_ERROR_CHECK;
		}



	};
}
