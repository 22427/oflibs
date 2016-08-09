
#include <cstdio>
#include <fstream>
#include <set>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <ofl_open_gl_tools.h>
#include "stb_image.h"

using namespace ofl;

class StereoImageViewer :
		public Window_GLFW,
		public StereoCompositor
{
	bool render_on;
	GLuint image;

	GLuint load_images_to_texture(const std::string& left,
								  const std::string& right);
public:

	
	StereoImageViewer(ofl::WindowPreferences* wp) : Window_GLFW(wp)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
	
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.25f,0.225f,0.2f,1.0f);

		render_on = true;
		this->eventWindowSize(getWidth(),getHeight());
	}

	int go(std::vector<std::string> argv)
	{
		if(argv.size() < 3)
			return 1;

		image = load_images_to_texture(argv[1],argv[2]);
		enterRenderLoop();
		return 0;
	}


	virtual void eventWindow(int event)
	{
		if(event == WIN_CLOSE)
			render_on = false;
	}

	virtual bool renderAFrame(double /*tslf_s*/)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->composite(image);
		this->swapBuffers();
		return render_on;

	}
	virtual void eventCharacter(unsigned int c, int );

	virtual void eventWindowSize(int w, int h)
	{
		glViewport(0,0,w,h);
		this->resize(w,h);

	}
};

int main(int argc, char** argv)
{
	ofl::WindowPreferences wp;
	StereoImageViewer siv(&wp);
	std::vector<std::string> args;
	for(int i = 0 ; i< argc;i++)
	{
		args.push_back(argv[i]);
	}
	return siv.go(args);
}

#define OFL_IMPLEMENTATION
#include <ofl_open_gl_tools.h>
#undef OFL_IMPLEMENTATION
GLuint StereoImageViewer::load_images_to_texture(const std::string &left,
												 const std::string &right)
{
	int w, h, c;
	unsigned char* img;
	GLuint tex;
	img = stbi_load(left.c_str(),&w,&h,&c,0);
	for(int y = 0 ; y < h/2;y++)
	{
		for(int x = 0 ; x<w;x++)
		{
			for(int j = 0 ; j< c; j++)
				std::swap(img[(y*w+x)*c+j],img[((h-y-1)*w+x)*c+j]);

		}
	}
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D_ARRAY,tex);


	GLenum input_format = GL_RGB;

	switch (c)
	{
	case 1: input_format = GL_RED; break;
	case 2: input_format = GL_RG;break;
	case 3: input_format = GL_RGB;break;
	case 4: input_format = GL_RGBA;break;
	default: input_format = GL_RED;break;
	}

	glTexImage3D( GL_TEXTURE_2D_ARRAY,
				  0,
				  GL_RGB,
				  w, h,2,
				  0, input_format,GL_UNSIGNED_BYTE,NULL );


	glTexSubImage3D( GL_TEXTURE_2D_ARRAY,
					 0,
					 0,0,0,
					 w,h,1,
					 input_format,
					 GL_UNSIGNED_BYTE,
					 img);

	stbi_image_free(img);


	img = stbi_load(right.c_str(),&w,&h,&c,0);
	for(int y = 0 ; y < h/2;y++)
	{
		for(int x = 0 ; x<w;x++)
		{
			for(int j = 0 ; j< c; j++)
				std::swap(img[(y*w+x)*c+j],img[((h-y-1)*w+x)*c+j]);

		}
	}
	glTexSubImage3D( GL_TEXTURE_2D_ARRAY,
					 0,
					 0,0,1,
					 w,h,1,
					 input_format,
					 GL_UNSIGNED_BYTE,
					 img);
	stbi_image_free(img);


	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);
	return tex;
}

void StereoImageViewer::eventCharacter(unsigned int c, int)
{
	if(c == 'q')
		render_on = false;
	else if(c == 'm')
	{
		int m = static_cast<int>(m_cmode)+1;
		int mc = static_cast<int>(MODE_COUNT);
		CompositingMode cm = static_cast<CompositingMode>(m % mc);
		this->setCompositingMode(cm);
	}
	else if(c == 'n')
	{
		float m = static_cast<float>(m_cmode)-1.0f;
		float mc = static_cast<float>(MODE_COUNT);
		m = mod(m,mc);
		CompositingMode cm = static_cast<CompositingMode>(m);
		this->setCompositingMode(cm);
	}
	else if(c == 'f')
		this->setFullscreen(true);
	else if(c == 'F')
		this->setFullscreen(false);

}
