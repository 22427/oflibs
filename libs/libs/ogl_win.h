#pragma once

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
	virtual void swapBuffers() = 0;
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

	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const  = 0;
};

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
		Window_GLFW* app = (Window_GLFW*) glfwGetWindowUserPointer(win);
		app->eventKey(key,action,mods,scancode);
	}

	static void charmods_callback(GLFWwindow* win, unsigned int c, int mods)
	{
		Window_GLFW* app=(Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventCharacter(c,mods);
	}

	static void cursor_position_callback(GLFWwindow* win, double xpos, double ypos)
	{
		Window_GLFW* app=(Window_GLFW*)glfwGetWindowUserPointer(win);
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
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventMouseButton(button,action,mods);
	}

	static void scroll_callback(GLFWwindow* win, double xoff, double yoff)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventMouseScroll(xoff,yoff);
	}


	static void win_resize_callback(GLFWwindow * win, int w , int h)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventWindowSize(w,h);
	}
	static void win_position_callback(GLFWwindow * win, int x , int y)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventWindowPosition(x,y);
	}
	static void win_iconify_callback(GLFWwindow * win, int iconified)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		if(iconified)
			app->eventWindow(WIN_ICONIFY);
		else
			app->eventWindow(WIN_RESTORE);
	}
	static void win_close_callback(GLFWwindow * win)
	{
		Window_GLFW* app= (Window_GLFW*)glfwGetWindowUserPointer(win);
		app->eventWindow(WIN_CLOSE);
	}
	static void win_focus_callback(GLFWwindow * win, int got_focus)
	{
		Window_GLFW* app = (Window_GLFW*)glfwGetWindowUserPointer(win);
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

			if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
			{
				printf("Failed to initialize OpenGL context\n");
			}
		}
	}

	~Window_GLFW()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	virtual void swapBuffers()
	{
		glfwSwapBuffers(m_window);
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

	virtual unsigned int getWidth() const
	{
		if(!m_window)
			return 0;
		int w;
		glfwGetWindowSize(m_window,&w,nullptr);
		return (unsigned int) w;
	}
	virtual unsigned int getHeight() const
	{
		if(!m_window)
			return 0;
		int h;
		glfwGetWindowSize(m_window,nullptr,&h);
		return (unsigned int) h;
	}
};

}
