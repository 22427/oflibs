#pragma once

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
