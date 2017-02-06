#pragma once

// OFL_IB GL
#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif
#include OFL_GL_HEADER
// OFL_IB
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "vmath.h"
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
		BottomTop =4,
		AnaglyphRedCyan=5,
		AnaglyphYellowBlue=6,
		QuadBuffered=7,
		MODE_COUNT = 8
	};
	enum Eye
	{
		Left = 0,
		Right = 1
	};
	
	StereoCompositor(const CompositingMode mode = VerticalInterlace);
	
	void resize(const int width, const int height);
	void setCompositingMode(CompositingMode cm);

	void setEye(const Eye eye);
	void composite(GLuint left_right_texture_array);
	
	mat4 getAsymetricProjection(
			Eye eye,
			float eye_dist,
			float fovy,
			float near,
			float far,
			float focal_length);
protected:
	CompositingMode m_cmode;
	int m_width;
	int m_height;

	bool m_wh_dirty;


	GLuint m_stencil_shader[3];
	GLint m_width_loc[3];
	GLint m_height_loc[3];
	
	GLuint m_post_processing_shader[MODE_COUNT];
	void create_stencil_buffer();



};
}
