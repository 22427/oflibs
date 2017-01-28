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







namespace ofl {


enum WindowEvent
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

enum KeyActions
{
	OFL_RELEASED = -1,
	OFL_REPEAT = 0,
	OFL_PRESSED = 1,
};

enum KeyModifiers
{
	OFL_SHIFT = 1,
	OFL_CTRL = 2,
	OFL_ALT = 4,
	OFL_SUPER = 8
};

enum class Key
{
	UNKNOWN			=	-1,
	SPACE			=	32,
	APOSTROPHE		=	39,		// '
	COMMA			=	44,		// ,
	MINUS			=	45,		// -
	PERIOD			=	46,		// .
	SLASH			=	47,		// /
	ZERO			=	48,
	ONE				=	49,
	TWO				=	50,
	THREE			=	51,
	FOUR			=	52,
	FIVE			=	53,
	SIX				=	54,
	SEVEN			=	55,
	EIGHT			=	56,
	NINE			=	57,
	SEMICOLON		=	59,		// ;
	EQUAL			=	61,		// =
	A				=	65,
	B				=	66,
	C				=	67,
	D				=	68,
	E				=	69,
	F				=	70,
	G				=	71,
	H				=	72,
	I				=	73,
	J				=	74,
	K				=	75,
	L				=	76,
	M				=	77,
	N				=	78,
	O				=	79,
	P				=	80,
	Q				=	81,
	R				=	82,
	S				=	83,
	T				=	84,
	U				=	85,
	V				=	86,
	W				=	87,
	X				=	88,
	Y				=	89,
	Z				=	90,
	LEFT_BRACKET	=	91,		// [
	BACKSLASH		=	92,
	RIGHT_BRACKET	=	93,		// ]
	GRAVE_ACCENT	=	96,		// `
	WORLD_1			=	161,	// #1
	WORLD_2			=	162,	// #2
	ESCAPE			=	256,
	ENTER			=	257,
	TAB				=	258,
	BACKSPACE		=	259,
	INSERT			=	260,
	DELETE			=	261,
	RIGHT			=	262,
	LEFT			=	263,
	DOWN			=	264,
	UP				=	265,
	PAGE_UP			=	266,
	PAGE_DOWN		=	267,
	HOME			=	268,
	END				=	269,
	CAPS_LOCK		=	280,
	SCROLL_LOCK		=	281,
	NUM_LOCK		=	282,
	PRINT_SCREEN	=	283,
	PAUSE			=	284,
	F1				=	290,
	F2				=	291,
	F3				=	292,
	F4				=	293,
	F5				=	294,
	F6				=	295,
	F7				=	296,
	F8				=	297,
	F9				=	298,
	F10				=	299,
	F11				=	300,
	F12				=	301,
	F13				=	302,
	F14				=	303,
	F15				=	304,
	F16				=	305,
	F17				=	306,
	F18				=	307,
	F19				=	308,
	F20				=	309,
	F21				=	310,
	F22				=	311,
	F23				=	312,
	F24				=	313,
	F25				=	314,
	NUM_ZERO		=	320,
	NUM_ONE			=	321,
	NUM_TWO			=	322,
	NUM_THREE		=	323,
	NUM_FOUR		=	324,
	NUM_FIVE		=	325,
	NUM_SIX			=	326,
	NUM_SEVEN		=	327,
	NUM_EIGHT		=	328,
	NUM_NEINE		=	329,
	NUM_DECIMAL		=	330,
	NUM_DIVIDE		=	331,
	NUM_MULTIPLY	=	332,
	NUM_SUBTRACT	=	333,
	NUM_ADD			=	334,
	NUM_ENTER		=	335,
	NUM_EQUAL		=	336,
	LEFT_SHIFT		=	340,
	LEFT_CONTROL	=	341,
	LEFT_ALT		=	342,
	LEFT_SUPER		=	343,
	RIGHT_SHIFT		=	344,
	RIGHT_CONTROL	=	345,
	RIGHT_ALT		=	346,
	RIGHT_SUPER		=	347,
	MENU			=	348,
	LAST			=	MENU
};

class WinListener
{
public:
	virtual void event_window(WindowEvent /*event*/){}
	virtual void event_moved(int /*x*/, int /*y*/){}
	virtual void event_resized(int /*w*/, int /*h*/){}
	virtual void event_key_change(int /*key*/, int /*action*/, int /*mods*/, int /*scancode*/){}
	virtual void event_character_input(unsigned int /*c*/, int /*mods*/){}
	virtual void event_mouse_button(int /*button*/, int /*action*/,int /*mods*/){}
	virtual void event_mouse_scroll(double /*xScroll*/, double /*yScroll*/){}
	virtual void event_mouse_move(double /*x*/, double /*y*/){}
};

class Win
{
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
#else
#endif
public:
	Win();

	~Win();


	void init();
	ivec2 getPosition();
	ivec2 getWindowSize();

	void process_events()
	{
		while ( XEventsQueued( m_display, QueuedAfterFlush ) )
		{
		  XEvent    event;
		  XNextEvent( m_display, &event );
		  if( event.xany.window != m_win )
			continue;

		  if (event.type==KeyPress)
		  {
				printf("%d\n",event.xkey.keycode);
		  }

		}
	}

	void inject_event_window(WindowEvent event)
	{
		for(auto l : m_listeners)
			l->event_window(event);
	}
	void inject_event_moved(int x, int y)
	{
		for(auto l : m_listeners)
			l->event_moved(x,y);
	}
	void inject_event_resized(int w, int h)
	{
		for(auto l : m_listeners)
			l->event_resized(w,h);
	}
	void inject_event_key_change(int key, int action, int mods, int scancode)
	{
		for(auto l : m_listeners)
			l->event_key_change(key,action,mods,scancode);
	}
	void inject_event_character_input(unsigned int /*c*/, int /*mods*/){}
	void inject_event_mouse_button(int /*button*/, int /*action*/,int /*mods*/){}
	void inject_event_mouse_scroll(double /*xScroll*/, double /*yScroll*/){}
	void inject_event_mouse_move(double /*x*/, double /*y*/){}
};


class GLContext
{
public:

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
