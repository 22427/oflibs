#pragma once
#include "vmath.h"
#include <string>
#include <vector>
#include <cstring>

#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif
#include OFL_GL_HEADER

#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>





#include "keys.h"

namespace ofl {






class WinListener;

class Win
{
public:
	enum WindowEvent
	{
		WIN_CLOSE,
		WIN_ICONIFY,
		WIN_RESTORE,
		WIN_GOT_FOCUS,
		WIN_LOST_FOCUS
	};
	class WindowAttributes
	{
	public:
		std::string title;
		bool resizable;
		bool visible;
		bool decorated;
		int border;
		int width;
		int height;
		int pos_x;
		int pos_y;
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
		bool stereo;
		bool srgb;
		bool double_buffer;
		WindowAttributes();
	} window_attributes;
protected:
	friend class GLContext;
	std::vector<WinListener*> m_listeners;


#ifdef linux
	Display * m_display;
	Window  m_win;
	int m_screen;
	GLXFBConfig m_fbcfg;
	XVisualInfo* m_visinfo;
	uint m_key_modifiers;
	bool m_key_states[Key::MAX_KEYS];
#else
#endif
public:
	Win();
	~Win();

	void init();
	ivec2 getPosition();
	ivec2 getWindowSize();


	void process_events();

	void inject_event_window(WindowEvent event);
	void inject_event_moved(const int x, const int y);
	void inject_event_resized(const int w, const int h);
	void inject_event_key_change(const Key key, const int action, const uint mods, const uint scancode);
	void inject_event_character_input(const unsigned int c,const Key::KeyActions action,const uint mods);
	void inject_event_mouse_button(const uint button,const Key::KeyActions action ,const uint mods, const double x, const double y);
	void inject_event_mouse_scroll(const double x_scroll, const double y_scroll);
	void inject_event_mouse_move(const double x, const double y);
};
class WinListener
{
public:
	virtual ~WinListener(){}
	virtual void event_window(Win::WindowEvent /*event*/){}
	virtual void event_moved(const int /*x*/, int /*y*/){}
	virtual void event_resized(const int /*w*/,const  int /*h*/){}
	virtual void event_key_change(const Key /*key*/,const int /*action*/,const  uint /*mods*/,const  uint /*scancode*/){}
	virtual void event_character_input(const unsigned int /*c*/,const  Key::KeyActions /*action*/,const  uint /*mods*/){}
	virtual void event_mouse_button(const uint /*button*/, const int /*action*/,const uint /*mods*/,const double /*x*/, const double /*y*/){}
	virtual void event_mouse_scroll(const double /*x_scroll*/, const double /*y_scroll*/){}
	virtual void event_mouse_move(const double /*x*/, const double /*y*/){}
};




class GLContext
{
public:
	enum OpenGLProfile
	{
		ANY,
		COMPATIBILITY,
		CORE
	};
	int opengl_version_major;
	int opengl_version_minor;
	OpenGLProfile opengl_profile;
	bool opengl_debug_context;
protected:
	Win* m_window;

#ifdef linux
	GLXContext m_cntxt;
#else
#endif

public:
	GLContext(Win* win)
	{
		m_window = win;
		opengl_version_major = 1;
		opengl_version_minor = 0;
		opengl_debug_context = false;
		opengl_profile = COMPATIBILITY;
	}
	~GLContext();
	void init();
	void swapBuffers();
	void makeCurrent();
};


class WinIOListeners
{
public:
	virtual void eventKey(int /*key*/, int /*action*/, int /*mods*/, int /*scancode*/){}
	virtual void eventCharacter(unsigned int /*c*/, int /*mods*/){}
	virtual void eventMouseButton(int /*button*/, int /*action*/,int /*mods*/){}
	virtual void eventMouseScroll(double /*xScroll*/, double /*yScroll*/){}
	virtual void eventMouseMove(double /*x*/, double /*y*/){}
};
class WinIO
{
	Win* m_window;
	std::vector<WinListener*> m_listeners;
public:
	WinIO(Win* win)
	{
		m_window = win;
	}
};
}
