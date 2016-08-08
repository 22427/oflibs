#include "ogl_stereo_compositor.h"

void ofl::StereoCompositor::resize(const int width, const int height)
{
	m_width  = width;
	m_height = height;
	m_wh_dirty = true;
}

void ofl::StereoCompositor::setMode(ofl::StereoCompositor::CompositingMode cm)
{
	m_cmode = cm;
	m_wh_dirty = true;
}

ofl::StereoCompositor::StereoCompositor(const ofl::StereoCompositor::CompositingMode mode)
	:m_cmode(mode), m_width(0), m_height(0), m_wh_dirty(true)
{
	glEnable(GL_STENCIL_TEST);
	const char* fragment_shader =
			"#version 330\n"
			"out vec4 clr; "
			"void main()"
			"{clr = vec4(1);}";
	int fs_len = strlen(fragment_shader);
	const char* codes[] = {

		"#version 330\n"
		"uniform int width; uniform int height;\n"
		"void main()\n"
		"{\n"
		"	float num = 2*(gl_VertexID%2)-1;\n"
		"	float w = 2*float((gl_VertexID/2)*2)/width-1;\n"
		"	gl_Position = vec4(w,num,0,1);\n"
		"}",

		"#version 330\n"
		"uniform int width; uniform int height;\n"
		"void main()\n"
		"{\n"
		"	float num = 2*float(gl_VertexID%2)-1;\n"
		"	float h = 2*float((gl_VertexID/2)*2)/height-1;\n"
		"	gl_Position = vec4(num,h,0,1);\n"
		"}\n"
		,
		"#version 330\n"
		"uniform int width; uniform int height;\n"
		"void main()\n"
		"{\n"
		"	float num = 2*float(gl_VertexID%2)-1;\n"
		"	float h = 2*float((gl_VertexID/2)*2)/height-1;\n"
		"	gl_Position = vec4(num,h,0,1);\n"
		"}"
	};
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs,1,&fragment_shader,&fs_len);
	glCompileShader(fs);

	for(int i =0 ; i < SideBySide ;i++)
	{
		m_shader[i] =glCreateProgram();
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		int len = strlen(codes[i]);
		glShaderSource(vs,1,&codes[i],&len);
		glCompileShader(vs);
		GLint compiled;

		glGetShaderiv(vs, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLint blen = 0;
			GLsizei slen = 0;

			glGetShaderiv(vs, GL_INFO_LOG_LENGTH , &blen);
			if (blen > 1)
			{
				GLchar* compiler_log = (GLchar*)malloc(blen);
				glGetShaderInfoLog(vs, blen, &slen, compiler_log);
				printf("compiler_log: %s\n", compiler_log);
				free (compiler_log);
			}
		}

		glAttachShader(m_shader[i],vs);
		glAttachShader(m_shader[i],fs);
		glLinkProgram(m_shader[i]);

		m_width_loc[i] = glGetUniformLocation(m_shader[i],"width");
		m_height_loc[i] = glGetUniformLocation(m_shader[i],"height");
	}


}

void ofl::StereoCompositor::setEye(const ofl::StereoCompositor::Eye eye)
{
	switch (m_cmode)
	{
	case SideBySide:
		glViewport(m_width/2*eye,0,m_width/2,m_height);
		break;
	case BottomTop:
		glViewport(0,m_height/2*eye,m_width,m_height/2);
		break;
	case VerticalInterlace:
	case HorizontalInterlace:
	case CheckerboardInterlace:
		if(m_wh_dirty)
			create_stencil_buffer();
		glStencilFunc(GL_EQUAL, eye, 0xFF);
		break;
	case QuadBuffered:
		glDrawBuffer(GL_BACK_LEFT+ eye);
		break;
	default:
		break;
	}
}

void ofl::StereoCompositor::create_stencil_buffer()
{
	glClear(GL_DEPTH_BUFFER_BIT| GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)

	// draw stencil pattern
	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF

	glUseProgram(m_shader[m_cmode]);
	if(m_wh_dirty)
	{
		glUniform1i(m_width_loc[m_cmode],m_width);
		glUniform1i(m_height_loc[m_cmode],m_height);
		m_wh_dirty = false;
	}
	if(m_cmode == VerticalInterlace)
		glDrawArrays(GL_LINES,0,m_width);
	else if(m_cmode == HorizontalInterlace)
		glDrawArrays(GL_LINES,0,m_height);
	else if(m_cmode == CheckerboardInterlace)
		glDrawArrays(GL_LINES,0,1);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}
