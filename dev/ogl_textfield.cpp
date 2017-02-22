#include "ogl_textfield.h"


ofl::OpenGLTextFieldRenderer::OpenGLTextFieldRenderer(void *font, const uint font_w, const uint font_h, const uint font_c, int font_type)
{

	m_font_ar = static_cast<float>(font_w)/static_cast<float>(font_h);
	std::string vtx_code =
			"#version 430\n"
			"layout (std140) uniform tf_uniforms \n"
			"{"
			"	uniform ivec2 chars;\n"
			"	uniform vec2 char_size;\n"
			"	uniform vec4 font_color;\n"
			"	uniform vec4 bg_color;\n"
			"	uniform mat4 mmtx;\n"
			"}\n;\n"
			""
			"layout(location = 1) uniform samplerBuffer text;\n"
			"out vec2 tex_coord;\n"
			"void main()\n"
			"{\n"
			"	vec2 q;\n"
			"	q.x = float(gl_InstanceID%3);\n"
			"	q.y = float(gl_InstanceID/3);\n"
			"	int A = (gl_VertexID&2)>>1;"
			"	int B = (gl_VertexID&1);"
			"	vec2 q_pos = (vec2(float(B),float(A))+q)*char_size;\n"
			"	gl_Position = vec4(q_pos,0,1);\n"
			"	uint c = uint(texelFetch(text,gl_InstanceID).x);\n"
			"	tex_coord = vec2(float(c%16)/16.0,float(c/16)/16.0);\n"
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
			"	uniform vec4 font_color;\n"
			"	uniform vec4 bg_color;\n"
			"	uniform mat4 mmtx;\n"
			"}\n ;\n"
			"void main()\n"
			"{\n"
			"	color = vec4(tex_coord,1.0,1.0 );\n"
			"}\n";

	Shader vtx(GL_VERTEX_SHADER,vtx_code);
	Shader frag(GL_FRAGMENT_SHADER,frag_code);

	if(!vtx.compile())
		printf("Compiling vertex shader failed:\n%s",vtx.read_log().c_str());
	if(!frag.compile())
		printf("Compiling fragment shader failed:\n%s",frag.read_log().c_str());
	sp.attach_shader(vtx);
	sp.attach_shader(frag);
	sp.link();

	sp.use();
	auto bindex = glGetUniformBlockIndex(sp.handle(), "tf_uniforms");

	glUniformBlockBinding(sp.handle(),bindex,0);

	glUniform1i(2,1);
	glUniform1i(1,0);

	glGenTextures(1,&m_font_texture);
	glBindTexture(GL_TEXTURE_2D,m_font_texture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RED,font_w,font_h,0,GL_RED,GL_UNSIGNED_BYTE,font);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
