#include "ogl_stereo_compositor.h"
#include <glad/glad.h>
namespace ofl {


void ofl::StereoCompositor::resize(const int width, const int height)
{
	m_width  = width;
	m_height = height;
	m_wh_dirty = true;
}

void ofl::StereoCompositor::setCompositingMode(ofl::StereoCompositor::CompositingMode cm)
{
	switch (m_cmode)
	{
	case SideBySide:
	case BottomTop:
		glViewport(0,0,m_width,m_height);
		break;
	case VerticalInterlace:
	case HorizontalInterlace:
	case CheckerboardInterlace:
		glDisable(GL_STENCIL_TEST);
		break;
	case QuadBuffered:
		glDrawBuffer(GL_BACK);
		break;
	case AnaglyphRedCyan:
	case AnaglyphYellowBlue:
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		break;
	case MODE_COUNT:break;
	}
	m_cmode = cm;

	switch (m_cmode)
	{
		case VerticalInterlace:
		case HorizontalInterlace:
		case CheckerboardInterlace:
		glEnable(GL_STENCIL_TEST);
		break;
	default:break;
	}
	m_wh_dirty = true;
}

ofl::StereoCompositor::StereoCompositor(const ofl::StereoCompositor::CompositingMode mode)
	:m_cmode(MODE_COUNT), m_width(0), m_height(0), m_wh_dirty(true)
{
	setCompositingMode(mode);

	const char* fragment_shader =
			"#version 330\n"
			"out vec4 clr; "
			"void main()"
			"{clr = vec4(1);}";
	int len = static_cast<int>(strlen(fragment_shader));
	const char* codes[] = {

		"#version 330\n"
		"uniform int width; uniform int height;\n"
		"void main()\n"
		"{\n"
		"float ow = 0.5/float(width);"
		"	float num = 2*float(gl_VertexID%2)-1;\n"
		"	float h = 2*float((gl_VertexID/2)*2)/width-1;\n"
		"	gl_Position = vec4(ow+h,num,0,1);\n"
		"}\n"
		,
		"#version 330\n"
		"uniform int width; uniform int height;\n"
		"void main()\n"
		"{\n"
		" float oh = 0.5/float(height);"
		"	float num = 2*float(gl_VertexID%2)-1;\n"
		"	float h = 2*float((gl_VertexID/2)*2)/height-1;\n"
		"	gl_Position = vec4(num,h+oh,0,1);\n"
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
	GLuint vs = 0;
	glShaderSource(fs,1,&fragment_shader,&len);
	glCompileShader(fs);

	for(int i =0 ; i < SideBySide ;i++)
	{
		m_stencil_shader[i] =glCreateProgram();
		vs = glCreateShader(GL_VERTEX_SHADER);
		len = static_cast<int>(strlen(codes[i]));
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
				GLchar* log = new GLchar[blen];
				glGetShaderInfoLog(vs, blen, &slen, log);
				printf("stencil shader log:\n%s\n", log);
				delete[] log;
			}
		}

		glAttachShader(m_stencil_shader[i],vs);
		glAttachShader(m_stencil_shader[i],fs);
		glLinkProgram(m_stencil_shader[i]);

		m_width_loc[i] = glGetUniformLocation(m_stencil_shader[i],"width");
		m_height_loc[i] = glGetUniformLocation(m_stencil_shader[i],"height");
	}


	const char* pp_vs_code =
			"#version 330\n"
			"out vec2 tex_coord;\n"
			"void main()\n"
			"{\n"
			"	tex_coord.x = float(gl_VertexID%2);\n"
			"	tex_coord.y = float(gl_VertexID>>1);\n"
			"	gl_Position = vec4(tex_coord*2-vec2(1,1),0,1);\n"

			"}\n";

	vs = glCreateShader(GL_VERTEX_SHADER);
	len = static_cast<int>(strlen(pp_vs_code));
	glShaderSource(vs,1,&pp_vs_code,&len);
	glCompileShader(vs);

	const char* pp_codes[] = {
		// Vertical-------------------------------------------------------------
		"#version 330\n"
		"in vec2 tex_coord;\n"
		"layout(location = 0) out vec4 clr; \n"
		"layout(binding = 99) uniform sampler2DArray textures;\n"
		"void main()\n"
		"{\n"
		"	float eye = float(int(gl_FragCoord.x)%2);"
		"	clr=texture(textures, vec3(tex_coord,eye));"
		"}",
		// Horizontal-----------------------------------------------------------
		"#version 330\n"
		"in vec2 tex_coord;\n"
		"layout(location = 0) out vec4 clr; \n"
		"layout(binding = 99) uniform sampler2DArray textures;\n"
		"void main()\n"
		"{\n"
		"	float eye = float(int(gl_FragCoord.y)%2);"
		"	clr=texture(textures, vec3(tex_coord,eye));"
		"}",
		// checker--------------------------------------------------------------
		"#version 330\n"
		"in vec2 tex_coord;\n"
		"layout(location = 0) out vec4 clr; \n"
		"layout(binding = 99) uniform sampler2DArray textures;\n"
		"void main()\n"
		"{\n"
		"	float eye = float(int(gl_FragCoord.x+gl_FragCoord.y)%2);"
		"	clr=texture(textures, vec3(tex_coord,eye));"
		"}",
		// sbs -----------------------------------------------------------------
		"#version 330\n"
		"in vec2 tex_coord;\n"
		"layout(location = 0) out vec4 clr; \n"
		"layout(binding = 99) uniform sampler2DArray textures;\n"
		"void main()\n"
		"{\n"
		"	float eye = int(2.0*tex_coord.x);\n"
		"	vec3 texc = vec3(tex_coord,eye);"
		"	texc.x = mod(2.0*tex_coord.x,1.0);"
		"	clr=texture(textures, texc);"
		"}",

		// bt -----------------------------------------------------------------
		"#version 330\n"
		"in vec2 tex_coord;\n"
		"layout(location = 0) out vec4 clr; \n"
		"layout(binding = 99) uniform sampler2DArray textures;\n"
		"void main()\n"
		"{\n"
		"	float eye = int(2.0*tex_coord.y);"
		"	vec3 texc = vec3(tex_coord,eye);"
		"	texc.y = mod(2.0*tex_coord.y,1.0);"
		"	clr=texture(textures, texc);"
		"}",

		// anaglyph red cyan --------------------------------------------------
		"#version 330\n"
		"in vec2 tex_coord;\n"
		"layout(location = 0) out vec4 clr; \n"
		"layout(binding = 99) uniform sampler2DArray textures;\n"
		"void main()\n"
		"{\n"
		"	vec4 l = texture(textures, vec3(tex_coord,0));\n"
		"	vec4 r = texture(textures, vec3(tex_coord,1));\n"
		"	float rg = 0.2126* r.r + 0.7152* r.g + 0.0722* r.b;\n"
		"	float lg = 0.2126* l.r + 0.7152* l.g + 0.0722* l.b;\n"
		"	clr= vec4(lg,rg,rg,1.0);"
		"}",

		// anaglyph yellow blue ------------------------------------------------
		"#version 330\n"
		"in vec2 tex_coord;\n"
		"layout(location = 0) out vec4 clr; \n"
		"layout(binding = 99) uniform sampler2DArray textures;\n"
		"void main()\n"
		"{\n"
		"	vec4 l = texture(textures, vec3(tex_coord,0));\n"
		"	vec4 r = texture(textures, vec3(tex_coord,1));\n"
		"	float rg = 0.2126* r.r + 0.7152* r.g + 0.0722* r.b;\n"
		"	float lg = 0.2126* l.r + 0.7152* l.g + 0.0722* l.b;\n"
		"	clr= vec4(lg,lg,rg,1.0);"
		"}",

		// quad buffering ------------------------------------------------------
		"#version 330\n"
		"in vec2 tex_coord;\n"
		"layout(location = 0) out vec4 clr_l; \n"
		"layout(location = 1) out vec4 clr_r; \n"

		"layout(binding = 99)  uniform sampler2DArray textures;\n"
		"void main()\n"
		"{\n"
		"	clr_l = texture(textures, vec3(tex_coord,0));\n"
		"	clr_r = texture(textures, vec3(tex_coord,1));\n"
		"}"
	};

	for(int i =0 ; i < MODE_COUNT ;i++)
	{
		m_post_processing_shader[i] =glCreateProgram();
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		len = static_cast<int>(strlen(pp_codes[i]));
		glShaderSource(fs,1,&pp_codes[i],&len);
		glCompileShader(fs);
		GLint compiled;

		glGetShaderiv(fs, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLint blen = 0;
			GLsizei slen = 0;

			glGetShaderiv(fs, GL_INFO_LOG_LENGTH , &blen);
			if (blen > 1)
			{
				GLchar* log = new GLchar[blen];
				glGetShaderInfoLog(vs, blen, &slen, log);
				printf("pp shader log:\n%s\n", log);
				delete[] log;
			}
		}

		glAttachShader(m_post_processing_shader[i],vs);
		glAttachShader(m_post_processing_shader[i],fs);
		glLinkProgram(m_post_processing_shader[i]);
		glUseProgram(m_post_processing_shader[i]);
		glUniform1i(glGetUniformLocation(m_post_processing_shader[i],"textures")
					,12);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;
	case AnaglyphRedCyan:
		if(eye==Left)
			glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
		else
			glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_FALSE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;
	case AnaglyphYellowBlue:
		if(eye==Left)
			glColorMask(GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE);
		else
			glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_FALSE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;
	case MODE_COUNT:break;


	}

}

void ofl::StereoCompositor::composite(GLuint left_right_texture_array)
{
	glActiveTexture(GL_TEXTURE0+12);
	glBindTexture(GL_TEXTURE_2D_ARRAY,left_right_texture_array);
	if(m_cmode == QuadBuffered)
	{
		const GLenum buffs[]={GL_BACK_LEFT, GL_BACK_RIGHT};
		glDrawBuffers(2,buffs);
	}
	glUseProgram(m_post_processing_shader[m_cmode]);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}

mat4 ofl::StereoCompositor::getAsymetricProjection(
		ofl::StereoCompositor::Eye eye,
		float eye_dist,
		float fovy,
		float near,
		float far,
		float focal_length)
{
	float ar = static_cast<float>(m_width)/m_height;
	float top = near * tanf(fovy/ 2.0f);
	float bottom = -top;
	float eye_sep = eye_dist * (static_cast<float>(eye)*2.0f-1.0f);
	float delta = 0.5f * eye_sep * near / focal_length;
	float left = -ar * top - delta;
	float right = ar * top - delta;
	return frustum(left,right,bottom,top,near,far);
}

void ofl::StereoCompositor::create_stencil_buffer()
{	
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);

	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_stencil_shader[m_cmode]);
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
}
