#if 0
<begin_doc>
#ifndef USING_OFL_LICENSE_MD
#define USING_OFL_LICENSE_MD
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

#endif //USING_OFL_LICENSE_MD

--------------------------------------------------------------------------------

#ifndef USING_OFL_README_MD
#define USING_OFL_README_MD
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
//- ogl_stereo_compositor - A simple way to render in stereo image formats.
//- ofl_ogl_win - An interface to create an OpenGL-Context with window and reading 
//  events. Currently implemented using glfw.
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

#endif //USING_OFL_README_MD

--------------------------------------------------------------------------------

#ifndef USING_OFL_OGL_WIN_MD
#define USING_OFL_OGL_WIN_MD
//OGL_WIN: OpenGL Window
//================================================================================
//This module will - one day - help to create an OpenGL context, and give access
//to mouse and keyboard events. For now it is a simple GLFW wrapper, but the
//interface shall be the same in the finished library.
//

#endif //USING_OFL_OGL_WIN_MD

--------------------------------------------------------------------------------

<end_doc>
#endif 
#ifndef USING_OFL_OGL_WIN_H
#define USING_OFL_OGL_WIN_H

#include <string>
namespace ofl {


enum WindowEvents
{
	WIN_CLOSE,
	WIN_ICONIFY,
	WIN_RESTORE,
	WIN_GOT_FOCUS,
	WIN_LOST_FOCUS
};


enum OpenGLAPI
{
	OPEN_GL,
	OPEN_GL_ES
};
enum OpenGLProfile
{
	ANY,
	COMPATIBILITY,
	CORE
};
class WindowPreferences
{
public:
	std::string title;
	bool resizable;
	bool visible;
	bool decorated;
	bool focused;
	bool fullscreen;
	int width;
	int height;
	bool use_monitor_properties; // will use the monitors settings..
	int red_bits;
	int green_bits;
	int blue_bits;
	int alpha_bits;
	int depth_bits;
	int stencil_bits;
	int accum_red_bits;
	int accum_green_bits;
	int accum_blue_bits;
	int accum_alpha_bits;
	int aux_buffers;
	int samples;
	int refresh_rate;
	// ... until here
	bool stereo;
	bool srgb;
	bool double_buffer;
	OpenGLAPI opengl_api;
	int opengl_version_major;
	int opengl_version_minor;
	bool debug_context;
	OpenGLProfile opengl_profile;

	WindowPreferences()
	{
		title = "OpenGL-Window";
		width = 640;
		height = 480;
		resizable = true;
		visible = true;
		decorated = true;
		focused = true;
		fullscreen = false;
		use_monitor_properties = true;
		red_bits = 8;
		green_bits = 8;
		blue_bits = 8;
		alpha_bits = 8;
		depth_bits = 24;
		stencil_bits = 8;
		accum_red_bits = 0;
		accum_green_bits= 0;
		accum_blue_bits= 0;
		accum_alpha_bits= 0;
		aux_buffers= 0;
		samples = 0;
		refresh_rate= 60;
		stereo = false;
		srgb = false ;
		double_buffer = true;
		opengl_api = OPEN_GL;
		opengl_version_major=0;
		opengl_version_minor=0;
		debug_context=false;
		opengl_profile=COMPATIBILITY;
	}
};

class Window
{

public:
	Window(WindowPreferences* /*wp*/){}
	virtual ~Window();
	virtual void swapBuffers() = 0;
	virtual void makeCurrent() = 0;
	virtual void enterRenderLoop() = 0;
	virtual void setTitle(const std::string& title) = 0;
	virtual void setFullscreen(bool fs) = 0;

	virtual void eventKey(int /*key*/, int /*action*/, int /*mods*/, int /*scancode*/){}
	virtual void eventCharacter(unsigned int /*c*/, int /*mods*/){}
	virtual void eventMouseButton(int /*button*/, int /*action*/,int /*mods*/){}
	virtual void eventMouseScroll(double /*xScroll*/, double /*yScroll*/){}
	virtual void eventMouseMove(double /*x*/, double /*y*/){}
	virtual void eventWindow(int /*event*/){}
	virtual void eventWindowPosition(int /*x*/, int /*y*/){}
	virtual void eventWindowSize(int /*w*/, int /*h*/){}

	virtual bool renderAFrame(double /*tslf_s*/)
	{return true;}

	virtual int getWidth() const = 0;
	virtual int getHeight() const  = 0;
};
Window::~Window(){}


#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window_GLFW : public Window
{
protected:

	GLFWwindow* m_window;
	double m_tsLastFrame;
	static void error_callback(int error, const char* description)
	{
		fprintf(stderr,"[ERR] %d: %s\n",error,description);
	}

	static void key_callback(
			GLFWwindow* win,
			int key,
			int scancode,
			int action,
			int mods)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		app->eventKey(key,action,mods,scancode);
	}

	static void charmods_callback(GLFWwindow* win, unsigned int c, int mods)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		app->eventCharacter(c,mods);
	}

	static void cursor_position_callback(GLFWwindow* win, double xpos, double ypos)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		app->eventMouseMove(xpos,ypos);
	}
	static void cursor_enter_callback(GLFWwindow* /*win*/, int /*entered*/)
	{
		//Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
	}
	static void mouse_button_callback(
			GLFWwindow* win,
			int button,
			int action,
			int mods)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		app->eventMouseButton(button,action,mods);
	}

	static void scroll_callback(GLFWwindow* win, double xoff, double yoff)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		app->eventMouseScroll(xoff,yoff);
	}


	static void win_resize_callback(GLFWwindow * win, int w , int h)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		app->eventWindowSize(w,h);
	}
	static void win_position_callback(GLFWwindow * win, int x , int y)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		app->eventWindowPosition(x,y);
	}
	static void win_iconify_callback(GLFWwindow * win, int iconified)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		if(iconified)
			app->eventWindow(WIN_ICONIFY);
		else
			app->eventWindow(WIN_RESTORE);
	}
	static void win_close_callback(GLFWwindow * win)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		app->eventWindow(WIN_CLOSE);
	}
	static void win_focus_callback(GLFWwindow * win, int got_focus)
	{
		Window_GLFW* app=static_cast<Window_GLFW*>(glfwGetWindowUserPointer(win));
		if(got_focus)
			app->eventWindow(WIN_GOT_FOCUS);
		else
			app->eventWindow(WIN_LOST_FOCUS);
	}

	int m_win_res_w;
	int m_win_res_h;
	int m_win_pos_x;
	int m_win_pos_y;

public:

	Window_GLFW(WindowPreferences* wp) : Window(wp)
	{
		m_tsLastFrame = 0;
		if (!glfwInit())
			exit(EXIT_FAILURE);
		glfwSetErrorCallback(error_callback);
		WindowPreferences default_wp;
		if(!wp)
		{
			wp= &default_wp;
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if(wp->use_monitor_properties)
		{

			wp->red_bits = mode->redBits;
			wp->green_bits = mode->greenBits;
			wp->blue_bits = mode->blueBits;
			wp->refresh_rate = mode->refreshRate;
		}

		glfwWindowHint(GLFW_RESIZABLE,wp->resizable);
		glfwWindowHint(GLFW_VISIBLE,wp->visible);
		glfwWindowHint(GLFW_DECORATED,wp->decorated);
		glfwWindowHint(GLFW_FOCUSED,wp->focused);
		glfwWindowHint(GLFW_RED_BITS,wp->red_bits);
		glfwWindowHint(GLFW_GREEN_BITS,wp->green_bits);
		glfwWindowHint(GLFW_BLUE_BITS,wp->blue_bits);
		glfwWindowHint(GLFW_ALPHA_BITS,wp->alpha_bits);
		glfwWindowHint(GLFW_DEPTH_BITS,wp->depth_bits);
		glfwWindowHint(GLFW_STENCIL_BITS,wp->stencil_bits);
		glfwWindowHint(GLFW_ACCUM_RED_BITS,wp->accum_red_bits);
		glfwWindowHint(GLFW_ACCUM_GREEN_BITS,wp->accum_green_bits);
		glfwWindowHint(GLFW_ACCUM_BLUE_BITS,wp->accum_blue_bits);
		glfwWindowHint(GLFW_ACCUM_ALPHA_BITS,wp->accum_alpha_bits);
		glfwWindowHint(GLFW_AUX_BUFFERS,wp->aux_buffers);
		glfwWindowHint(GLFW_SAMPLES,wp->samples);
		glfwWindowHint(GLFW_REFRESH_RATE,wp->refresh_rate);
		glfwWindowHint(GLFW_STEREO,wp->stereo);
		glfwWindowHint(GLFW_SRGB_CAPABLE,wp->srgb);


		glfwWindowHint(GLFW_DOUBLEBUFFER,wp->double_buffer);
		if(wp->opengl_version_major)
		{
			printf("gl version major %d\n",wp->opengl_version_major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,wp->opengl_version_major);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,wp->opengl_version_minor);
		}
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,wp->debug_context);
		if(wp->opengl_version_major > 3 && wp->opengl_version_minor > 2)
		{
			if(wp->opengl_profile == ANY)
				glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_ANY_PROFILE);
			else if(wp->opengl_profile == COMPATIBILITY)
				glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);
			else
				glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		}
		if(wp->opengl_api == OPEN_GL)
			glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_API);
		else
			glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_ES_API);

		if(wp->fullscreen)
		{
			m_window = glfwCreateWindow(
						mode->width,
						mode->height,
						wp->title.c_str(),
						monitor,
						nullptr);
		}
		else
		{
			m_window = glfwCreateWindow(
						wp->width,
						wp->height,
						wp->title.c_str(),
						nullptr,
						nullptr);
		}
		if(m_window)
		{
			glfwMakeContextCurrent(m_window);
			glfwSetWindowUserPointer(m_window, this);
			glfwSetKeyCallback(m_window,key_callback);
			glfwSetCharModsCallback(m_window,charmods_callback);
			glfwSetCursorPosCallback(m_window,cursor_position_callback);
			glfwSetMouseButtonCallback(m_window,mouse_button_callback);
			glfwSetScrollCallback(m_window,scroll_callback);
			glfwSetWindowCloseCallback(m_window,win_close_callback);
			glfwSetWindowPosCallback(m_window,win_position_callback);
			glfwSetWindowFocusCallback(m_window,win_focus_callback);
			glfwSetWindowIconifyCallback(m_window,win_iconify_callback);
			glfwSetWindowSizeCallback(m_window, win_resize_callback);

			if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
			{
				printf("Failed to initialize OpenGL context\n");
			}
		}
	}

	virtual ~Window_GLFW();

	virtual void swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}
	virtual void makeCurrent()
	{
		glfwMakeContextCurrent(m_window);
	}
	virtual void enterRenderLoop()
	{
		bool contin = true;
		glfwSetTime(0.0);
		while(contin)
		{
			double time = glfwGetTime();
			contin = this->renderAFrame(time-m_tsLastFrame);
			m_tsLastFrame = time;
			if(contin)
			{
				glfwPollEvents();
			}
		}
	}

	virtual void setTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_window,title.c_str());
	}

	virtual void setFullscreen(bool fs)
	{
		if(fs)
		{
			glfwGetWindowSize(m_window,&m_win_res_w,&m_win_res_h);
			glfwGetWindowPos(m_window,&m_win_pos_x,&m_win_pos_y);
			GLFWmonitor* mon = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(mon);
			glfwSetWindowMonitor(m_window,mon,0,0,mode->width,mode->height,mode->refreshRate);
		}
		else
		{
			GLFWmonitor* mon = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(mon);
			glfwSetWindowMonitor(m_window,nullptr,m_win_pos_x,m_win_pos_y,m_win_res_w,m_win_res_h,mode->refreshRate);
		}
	}

	virtual int getWidth() const
	{
		if(!m_window)
			return 0;
		int w;
		glfwGetWindowSize(m_window,&w,nullptr);
		return  w;
	}
	virtual int getHeight() const
	{
		if(!m_window)
			return 0;
		int h;
		glfwGetWindowSize(m_window,nullptr,&h);
		return  h;
	}
};

Window_GLFW::~Window_GLFW()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


}

#endif //USING_OFL_OGL_WIN_H
#ifdef OFL_IMPLEMENTATION
#endif
