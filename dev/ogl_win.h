#pragma once
#include <string>
#include <vector>
#include <cstring>
#include <functional>

#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif
#include OFL_GL_HEADER

#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glx.h>

#include "vmath.h"
#include "user_io.h"

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
		WIN_LOST_FOCUS,
	};

	enum EventMask
	{
		EM_KEYBOARD_IO	=  1,
		EM_MOUSE_IO		=  2,
		EM_RESIZE		=  4,
		EM_REPOSITION	=  8
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
		uint event_mask;
		WindowAttributes();
	} window_attributes;
protected:
	friend class GLContext;
	friend class WinIO;
	std::vector<WinListener*> m_listeners;

	std::function<void(const Key /*key*/,const int /*action*/,const  uint /*mods*/,const  uint /*scancode*/)> on_key_change;
	std::function<void(const unsigned int /*c*/,const  Key::KeyActions /*action*/,const  uint /*mods*/)> on_char_input;
	std::function<void(const uint /*button*/, const int /*action*/,const uint /*mods*/,const double /*x*/, const double /*y*/)> on_mouse_button;
	std::function<void(const double /*x_scroll*/, const double /*y_scroll*/)> on_mouse_scroll;
	std::function<void(const double /*x*/, const double /*y*/)> on_mouse_move;

#ifdef linux
	Display * m_display;
	Window  m_win;
	Window  m_root;
	int m_screen;
	GLXFBConfig m_fbcfg;
	XVisualInfo* m_visinfo;
	uint m_key_modifiers;
	bool m_key_states[Key::MAX_KEYS];

	bool m_button_states[16];

	Atom wm_protocols;
	Atom wm_delete_window ;
	Atom net_wm_ping;
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


	void set_on_keyboard_input(std::function<void(const Key /*key*/,const int /*action*/,const  uint /*mods*/,const  uint /*scancode*/)>f)
	{on_key_change = f;}
	void set_on_char_input(std::function<void(const unsigned int /*c*/,const  Key::KeyActions /*action*/,const  uint /*mods*/)>f)
	{on_char_input = f;}
	void set_on_mouse_button(std::function<void(const uint /*button*/, const int /*action*/,const uint /*mods*/,const double /*x*/, const double /*y*/)>f)
	{on_mouse_button = f;}
	void set_on_mouse_scroll(std::function<void(const double /*x_scroll*/, const double /*y_scroll*/)> f)
	{on_mouse_scroll = f;}
	void set_on_mouse_move(std::function<void(const double /*x*/, const double /*y*/)> f)
	{on_mouse_move = f;}


};
class WinListener
{
public:
	virtual ~WinListener(){}
	virtual void event_window(Win::WindowEvent /*event*/){}
	virtual void event_window_moved(const int /*x*/, int /*y*/){}
	virtual void event_window_resized(const int /*w*/,const  int /*h*/){}
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



class WinIO : public UserIODevice
{
	Win* m_window;
	std::vector<UserIOListeners*> m_listeners;

	void event_keyboard_input(const Key key,const int action,const  uint mods,const  uint scancode)
	{
		for(auto& l : m_listeners)
		{
			l->event_keyboard(key,action,mods,scancode);
		}
	}
	void event_character_input(const unsigned int c,const  Key::KeyActions action,const  uint mods)
	{
		for(auto& l : m_listeners)
		{
			l->event_character(c,action,mods);
		}
	}
	void event_mouse_button(const uint button, const int action,const uint mods,const double x, const double y)
	{
		for(auto& l : m_listeners)
		{
			l->event_mouse_button(button,action,mods,x,y);
		}
	}
	void event_mouse_scroll(const double x_scroll, const double y_scroll)
	{
		for(auto& l : m_listeners)
		{
			l->event_mouse_scroll(x_scroll,y_scroll);
		}
	}
	void event_mouse_move(const double x, const double y)
	{
		for(auto& l : m_listeners)
		{
			l->event_mouse_move(x,y);
		}
	}

public:
	WinIO(Win* win)
	{
		using namespace std::placeholders;
		win->set_on_char_input(std::bind(&WinIO::event_character_input, this,_1,_2,_3));
		win->set_on_keyboard_input(std::bind(&WinIO::event_keyboard_input, this,_1,_2,_3,_4));
		win->set_on_mouse_button(std::bind(&WinIO::event_mouse_button, this,_1,_2,_3,_4,_5));
		win->set_on_mouse_move(std::bind(&WinIO::event_mouse_move, this,_1,_2));
		win->set_on_mouse_scroll(std::bind(&WinIO::event_mouse_scroll, this,_1,_2));
	}

	virtual void add_listener(UserIOListeners* l)
	{
		m_listeners.push_back(l);
	}
	virtual bool is_key_down(Key k){return m_window->m_key_states[k.id];}

	virtual uint modifyer_keys()
	{
		return m_window->m_key_modifiers;
	}
	virtual bool is_button_down(uint button)
	{
		if(button < 16)
			return m_window->m_button_states[button];
		return false;
	}

};
}
