#include "ogl_text.h"
#include "font.h"
namespace ofl
{
OpenGLTextFieldRenderer::OpenGLTextFieldRenderer(const int font_texture_unit):
	OpenGLTextFieldRenderer(ffont_data,ffont_width,ffont_height,ffont_channels,font_texture_unit)
{

}

OpenGLTextFieldRenderer::OpenGLTextFieldRenderer(const void *font, const uint font_w, const uint font_h, const uint font_c,const int font_texture_unit)
{
	m_uniforms_dirty = true;
	m_font_texture_unit = font_texture_unit;

	m_font_ar = static_cast<float>(font_w)/static_cast<float>(font_h);
	std::string vtx_code =
			"#version 430\n"
			"layout (std140) uniform tf_uniforms \n"
			"{"
			"	uniform ivec2 chars;\n"
			"	uniform vec2 char_size;\n"
			"	uniform vec2 anchor;\n"
			"	uniform vec2 padding;\n"
			"	uniform vec4 font_color;\n"
			"	uniform vec4 bg_color;\n"
			"	uniform mat4 mmtx;\n"
			"}\n;\n"
			"layout(location = 1) uniform mat4 pm;\n"
			"layout(location=0) in uint c;\n"
			"out vec2 tex_coord;\n"
			"void main()\n"
			"{\n"
			"	vec2 q;\n"
			"	q.x = float(gl_InstanceID%chars.x);\n"
			"	q.y = float(chars.y-(gl_InstanceID/chars.x));\n"
			"	int A = (gl_VertexID&2)>>1;"
			"	int B = (gl_VertexID&1);"
			"	vec2 q_pos = (vec2(float(B),float(A))+q)*char_size;\n"
			"	gl_Position = pm*mmtx*vec4(q_pos-(vec2(chars)*vec2(1.0,1+1.0/float(chars.y))*char_size*anchor),0,1);\n"
			"	tex_coord = vec2(float(c%16+B)/16.0,float(c/16+1-A)/16.0);\n"
			"}";

	std::string frag_code =
			"#version 430\n"
			"in vec2 tex_coord;\n"
			"out vec4 color;\n"
			"layout(location = 2) uniform sampler2D font;\n"
			"layout (std140) uniform tf_uniforms \n"
			"{"
			"	uniform ivec2 chars;\n"
			"	uniform vec2 char_size;\n"
			"	uniform vec2 anchor;\n"
			"	uniform vec2 padding;\n"
			"	uniform vec4 font_color;\n"
			"	uniform vec4 bg_color;\n"
			"	uniform mat4 mmtx;\n"
			"}\n ;\n"
			"void main()\n"
			"{\n"
			"	vec2 ts = vec2(-0.5,0.5)/vec2(textureSize(font,0));"
			"	color = mix(bg_color,font_color,smoothstep(0.5,0.5,texture(font,tex_coord+ts).x));\n"
			"}\n";


	Shader vtx(GL_VERTEX_SHADER,vtx_code);
	Shader frag(GL_FRAGMENT_SHADER,frag_code);

	if(!vtx.compile())
		printf("Compiling vertex shader failed:\n%s",vtx.read_log().c_str());
	if(!frag.compile())
		printf("Compiling fragment shader failed:\n%s",frag.read_log().c_str());
	m_shader_program.attach_shader(vtx);
	m_shader_program.attach_shader(frag);
	m_shader_program.link();

	m_shader_program.use();
	auto bindex = glGetUniformBlockIndex(m_shader_program.handle(), "tf_uniforms");

	glUniformBlockBinding(m_shader_program.handle(),bindex,0);

	glUniform1i(2,m_font_texture_unit);
	glGenTextures(1,&m_font_texture);
	glBindTexture(GL_TEXTURE_2D,m_font_texture);
	GLenum tex_format = GL_RED;
	if(font_c == 2)
		tex_format = GL_RG;
	else if(font_c == 3)
		tex_format = GL_RGB;
	else if(font_c == 4)
		tex_format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D,0,GL_RED,font_w,font_h,0,tex_format,GL_UNSIGNED_BYTE,font);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	OFL_DBG_ERROR_CHECK;
}

void OpenGLTextField::upload_ud()
{
	if(m_ud_dirty)
	{
		m_ud_dirty = false;

		glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);
		glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(ud),&ud);
		OFL_DBG_ERROR_CHECK;
	}
}

OpenGLTextField::~OpenGLTextField()
{
	glDeleteVertexArrays(1,&m_vao);
	glDeleteBuffers(1,&m_buffer);
	glDeleteBuffers(1,&m_ubo);
}

OpenGLTextField::OpenGLTextField(const GLuint width_c, const GLuint height_c, const float cw, const float ch)

{
	OFL_DBG_ERROR_CHECK;
	m_ud_dirty = true;

	ud.chars = ivec2(width_c,height_c);
	ud.char_size = vec2(cw,ch);
	ud.font_color = vec4(1);
	ud.bg_color = vec4(0,0,0,1);
	ud.anchor = vec2(0.0f,0.0f);

	glGenBuffers(1,&m_ubo);
	OFL_DBG_ERROR_CHECK;
	glBindBuffer(GL_UNIFORM_BUFFER,m_ubo);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(ud),nullptr,GL_STATIC_DRAW);
	upload_ud();

	OFL_DBG_ERROR_CHECK;
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1,&m_buffer);
	OFL_DBG_ERROR_CHECK;

	glBindBuffer(GL_ARRAY_BUFFER,m_buffer);
	void* data = malloc(ud.chars.x*ud.chars.y);
	bzero(data,ud.chars.x*ud.chars.y);
	glBufferData(GL_ARRAY_BUFFER,ud.chars.x*ud.chars.y,data,GL_DYNAMIC_DRAW);
	free(data);

	OFL_DBG_ERROR_CHECK;

	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(0,1,GL_UNSIGNED_BYTE,0,nullptr);
	glVertexAttribDivisor(0,1);



	OFL_DBG_ERROR_CHECK;

}

void OpenGLTextField::resize(const ivec2 &chars)
{
	ud.chars = chars;
	glBindBuffer(GL_ARRAY_BUFFER,m_buffer);
	void* data = malloc(ud.chars.x*ud.chars.y);
	bzero(data,ud.chars.x*ud.chars.y);
	glBufferData(GL_ARRAY_BUFFER,ud.chars.x*ud.chars.y,data,GL_DYNAMIC_DRAW);
	free(data);
	m_ud_dirty = true;
}

void OpenGLTextField::set_font_color(const vec4 &color)
{
	ud.font_color= color;
	m_ud_dirty = true;
}

void OpenGLTextField::set_bg_color(const vec4 &color)
{
	ud.bg_color= color;
	m_ud_dirty = true;
}

void OpenGLTextField::set_char_size(const vec2 &size)
{
	ud.char_size= size;
	m_ud_dirty = true;
}

void OpenGLTextField::set_anchor(const vec2 &an)
{
	ud.anchor = an;
	m_ud_dirty = true;
}

void OpenGLTextField::set_char_width(const float width)
{
	float a = ud.char_size.y/ud.char_size.x;
	ud.char_size.x = width;
	ud.char_size.y =  a * width;
	m_ud_dirty = true;
}

void OpenGLTextField::set_char_height(const float height)
{
	float a = ud.char_size.x/ud.char_size.y;
	ud.char_size.x =  a * height;
	ud.char_size.y = height;
	m_ud_dirty = true;
}

void OpenGLTextField::set_model_matrix(const mat4 &mtx)
{
	ud.mmtx =  mtx;
	m_ud_dirty = true;
}

void OpenGLTextField::write_text(const std::string &str, const GLint from)
{
	if(from >= ud.chars.x*ud.chars.y)
		return;
	glBindBuffer(GL_ARRAY_BUFFER,m_buffer);
	glBufferSubData(GL_ARRAY_BUFFER,from,std::min(ud.chars.x*ud.chars.y-from,static_cast<GLint>(str.length())),str.c_str());
	OFL_DBG_ERROR_CHECK;
}
}
