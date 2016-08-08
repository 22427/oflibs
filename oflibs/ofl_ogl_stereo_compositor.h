#if 0
<begin_doc>
//The MIT License (MIT)
//================================================================================
//
//Copyright (c) 2016 Timon Zietlow
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of
//this software and associated documentation files (the "Software"), to deal in
//the Software without restriction, including without limitation the rights to
//use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//the Software, and to permit persons to whom the Software is furnished to do so,
//subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
//COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

--------------------------------------------------------------------------------

//What is this?
//================================================================================
//The oflibs are a set of usefull classes and tools dealing with all kinds of 
//problems.  
//My personal focus is in 3D-visualisation and VR, so most of these deal with 
//these topics. OFLIBS stands for "**o**ne **f**ile **lib**rarie**s**". The basic 
//idea is to write libraries in a single file, which you just have to include into
//your project, and you are ready to go.
//I find these usefull for fast prototypes and "experimental" code, which is what 
//this collection currently is, so don't expect a well tested, woking toolset, but
//maybe you find the one or two lines of code you were searching ;).
//I'm constantly working whith these tools and I am shure they will get better 
//over time.
//
//Structure
//--------------------------------------------------------------------------------
//In oflibs/ you find the finished one file libraries. They are ready to use. The 
//documentation to each library is also inside of the lib. Here is a short list of
//the different modules and libraries:
//
//- ofl_all: All oflibs in one 
//- ofl_ogl_geo - A class reperesenting vertex-data on the GPU
//- ofl_ogl_state - A state-wrapper imulating the "classic" OpenGL-fixed-function 
//  states, including matrix stacks and lighting.
//- ofl_ogl_win - An interface to create an OpenGL-Context with window and reading 
//  events. Currently implemented using glfw.
//- ofl_open_gl_tools: combines the above.
//- ofl_socket: A class wrapping sockets.
//- ofl_stru: Some string utilities. Used by many other oflibs.
//- ofl_track - Tracking: An interface to the ART-DTrack2 tracking system.
//- olf_vd - Vertex Data: A data structure representing geometry on the client 
//  side. This is OpenGL independend. 
//- ofl_vdman - Vertex Data MANufacturer: Allows you to create VertexData using
//  the old begin(..) ... end() paradigma from fixed-function-GL, but is OpenGL
//  independend.
//- ofl_vmath - Vector MATH : Some simple vector operations, but no alternative to 
//  GLM.
//- ofl_vrpv - Virtual Reality Projection and View - A module to genereate view-
//  and projection-matrices for VR systems like PowerWalls or CAVEs
//
//How do oflibs work?
//--------------------------------------------------------------------------------
//You use the oflib as a regular header, for example:
//```c_cpp
	#include <ofl_all.h>
//```
//Then in one .cpp-file of your project you include the oflib defining
//`OFL_IMPLEMENTATION` before:
//```c_cpp
//	#define OFL_IMPLEMENTATION
	#include <ofl_all.h>
//	#undef OFL_IMPLEMENTATION
//```
//That's it :)
//
//Internally the oflib-header is organized in two parts, the declaration-part 
//(that's what you would write into an ordinary header) and the implementation-part 
//(that's what you would write into *.cpp files)
//Example:
//```c_cpp
//	#ifndef OFL_ALL_H
//	#define OFL_ALL_H
//		// ALL DECLARATIONS
//	#endif
//	#ifdef OFL_IMPLEMENTATION
//		// ALL THE IMPLEMENTATIONS
//	#endif
//```
//
//Where can I get more information?
//--------------------------------------------------------------------------------
//Inside the oflib files there is the documentation for each module used. 
//In the subdirectory tools/ there are also some more or less simple tools using 
//the oflibs.
//
//
//

--------------------------------------------------------------------------------

<end_doc>
#endif 
#ifndef USING_OFL_OGL_STEREO_COMPOSITOR_H
 #define USING_OFL_OGL_STEREO_COMPOSITOR_H
 
#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif

#include OFL_GL_HEADER
#include <cstring>
#include <cstdlib>
#include <cstdio>
namespace ofl
{
class StereoCompositor
{

public:
	enum CompositingMode
	{
		VerticalInterlace = 0,
		HorizontalInterlace = 1,
		CheckerboardInterlace = 2,
		SideBySide = 3,
		BottomTop ,
		QuadBuffered
	};
	enum Eye
	{
		Left = 0,
		Right = 1
	};

	void resize(const int width, const int height);

	void setMode(CompositingMode cm);
	StereoCompositor(const CompositingMode mode = VerticalInterlace);

	void setEye(const Eye eye);
protected:
	CompositingMode m_cmode;
	unsigned int m_width;
	unsigned int m_height;

	bool m_wh_dirty;


	GLuint m_shader[SideBySide];


	GLint m_width_loc[SideBySide];
	GLint m_height_loc[SideBySide];

	void create_stencil_buffer();



};
}

#endif //USING_OFL_OGL_STEREO_COMPOSITOR_H
#ifdef OFL_IMPLEMENTATION

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
#endif
