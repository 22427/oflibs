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

class Key
{
public:
	enum KeyID
	{
		UNKNOWN			=	XK_VoidSymbol&0x01ff,
		SPACE			=	XK_space&0x01ff,
		APOSTROPHE		=	XK_apostrophe&0x01ff,		// '
		COMMA			=	XK_comma&0x01ff,		// ,
		MINUS			=	XK_minus&0x01ff,		// -
		PERIOD			=	XK_period&0x01ff,		// .
		SLASH			=	XK_slash&0x01ff,		// /
		ZERO			=	XK_0&0x01ff,
		ONE				=	XK_1&0x01ff,
		TWO				=	XK_2&0x01ff,
		THREE			=	XK_3&0x01ff,
		FOUR			=	XK_4&0x01ff,
		FIVE			=	XK_5&0x01ff,
		SIX				=	XK_6&0x01ff,
		SEVEN			=	XK_7&0x01ff,
		EIGHT			=	XK_8&0x01ff,
		NINE			=	XK_9&0x01ff,
		SEMICOLON		=	XK_semicolon&0x01ff,		// ;
		EQUAL			=	XK_equal&0x01ff,		// =
		A				=	XK_A&0x01ff,
		B				=	XK_B&0x01ff,
		C				=	XK_C&0x01ff,
		D				=	XK_D&0x01ff,
		E				=	XK_E&0x01ff,
		F				=	XK_F&0x01ff,
		G				=	XK_G&0x01ff,
		H				=	XK_H&0x01ff,
		I				=	XK_I&0x01ff,
		J				=	XK_J&0x01ff,
		K				=	XK_K&0x01ff,
		L				=	XK_L&0x01ff,
		M				=	XK_M&0x01ff,
		N				=	XK_N&0x01ff,
		O				=	XK_O&0x01ff,
		P				=	XK_P&0x01ff,
		Q				=	XK_Q&0x01ff,
		R				=	XK_R&0x01ff,
		S				=	XK_S&0x01ff,
		T				=	XK_T&0x01ff,
		U				=	XK_U&0x01ff,
		V				=	XK_V&0x01ff,
		W				=	XK_W&0x01ff,
		X				=	XK_X&0x01ff,
		Y				=	XK_Y&0x01ff,
		Z				=	XK_Z&0x01ff,
		LEFT_BRACKET	=	XK_bracketleft&0x01ff,		// [
		BACKSLASH		=	XK_backslash&0x01ff,
		RIGHT_BRACKET	=	XK_bracketright&0x01ff,		// ]
		GRAVE_ACCENT	=	XK_grave&0x01ff,		// `
		ESCAPE			=	XK_Escape&0x01ff,
		ENTER			=	XK_Return&0x01ff,
		TAB				=	XK_Tab&0x01ff,
		BACKSPACE		=	XK_BackSpace&0x01ff,
		INSERT			=	XK_Insert&0x01ff,
		DELETE			=	XK_Delete&0x01ff,
		RIGHT			=	XK_Right&0x01ff,
		LEFT			=	XK_Left&0x01ff,
		DOWN			=	XK_Down&0x01ff,
		UP				=	XK_Up&0x01ff,
		PAGE_UP			=	XK_Page_Up&0x01ff,
		PAGE_DOWN		=	XK_Page_Down&0x01ff,
		HOME			=	XK_Home&0x01ff,
		END				=	XK_End&0x01ff,
		CAPS_LOCK		=	XK_Caps_Lock&0x01ff,
		SCROLL_LOCK		=	XK_Scroll_Lock&0x01ff,
		NUM_LOCK		=	XK_Num_Lock&0x01ff,
		PRINT_SCREEN	=	XK_Print&0x01ff,
		PAUSE			=	XK_Pause&0x01ff,
		F1				=	XK_F1&0x01ff,
		F2				=	XK_F2&0x01ff,
		F3				=	XK_F3&0x01ff,
		F4				=	XK_F4&0x01ff,
		F5				=	XK_F5&0x01ff,
		F6				=	XK_F6&0x01ff,
		F7				=	XK_F7&0x01ff,
		F8				=	XK_F8&0x01ff,
		F9				=	XK_F9&0x01ff,
		F10				=	XK_F10&0x01ff,
		F11				=	XK_F11&0x01ff,
		F12				=	XK_F12&0x01ff,
		F13				=	XK_F13&0x01ff,
		F14				=	XK_F14&0x01ff,
		F15				=	XK_F15&0x01ff,
		F16				=	XK_F16&0x01ff,
		F17				=	XK_F17&0x01ff,
		F18				=	XK_F18&0x01ff,
		F19				=	XK_F19&0x01ff,
		F20				=	XK_F20&0x01ff,
		F21				=	XK_F21&0x01ff,
		F22				=	XK_F22&0x01ff,
		F23				=	XK_F23&0x01ff,
		F24				=	XK_F24&0x01ff,
		F25				=	XK_F25&0x01ff,
		NUM_ZERO		=	XK_KP_0&0x01ff,
		NUM_ONE			=	XK_KP_1&0x01ff,
		NUM_TWO			=	XK_KP_2&0x01ff,
		NUM_THREE		=	XK_KP_3&0x01ff,
		NUM_FOUR		=	XK_KP_4&0x01ff,
		NUM_FIVE		=	XK_KP_5&0x01ff,
		NUM_SIX			=	XK_KP_6&0x01ff,
		NUM_SEVEN		=	XK_KP_7&0x01ff,
		NUM_EIGHT		=	XK_KP_8&0x01ff,
		NUM_NEINE		=	XK_KP_9&0x01ff,
		NUM_DECIMAL		=	XK_KP_Decimal&0x01ff,
		NUM_DIVIDE		=	XK_KP_Divide&0x01ff,
		NUM_MULTIPLY	=	XK_KP_Multiply&0x01ff,
		NUM_SUBTRACT	=	XK_KP_Subtract&0x01ff,
		NUM_ADD			=	XK_KP_Add&0x01ff,
		NUM_ENTER		=	XK_KP_Enter&0x01ff,
		NUM_EQUAL		=	XK_KP_Equal&0x01ff,
		LEFT_SHIFT		=	XK_Shift_L&0x01ff,
		LEFT_CONTROL	=	XK_Control_L&0x01ff,
		LEFT_ALT		=	XK_Alt_L&0x01ff,
		LEFT_SUPER		=	XK_Super_L&0x01ff,
		RIGHT_SHIFT		=	XK_Shift_R&0x01ff,
		RIGHT_CONTROL	=	XK_Control_R&0x01ff,
		RIGHT_ALT		=	XK_Alt_R&0x01ff,
		RIGHT_SUPER		=	XK_Super_R&0x01ff,
		MENU			=	XK_Menu&0x01ff,
		MAX_KEYS		=   0x0200
	};

	KeyID id;
	Key(KeyID id){this->id = id;}
	Key(uint32_t id){this->id = static_cast<KeyID>(id);}
	Key(KeySym id){this->id = static_cast<KeyID>(id);}

	operator KeyID(){return id;}
	operator uint32_t(){return static_cast<uint32_t>(id);}
	operator KeySym(){return static_cast<KeySym>(id);}
};
class WinListener
{
public:
	virtual ~WinListener(){}
	virtual void event_window(WindowEvent /*event*/){}
	virtual void event_moved(int /*x*/, int /*y*/){}
	virtual void event_resized(int /*w*/, int /*h*/){}
	virtual void event_key_change(Key /*key*/, int /*action*/, uint /*mods*/, uint /*scancode*/){}
	virtual void event_character_input(unsigned int /*c*/, uint /*mods*/){}
	virtual void event_mouse_button(int /*button*/, int /*action*/,uint /*mods*/){}
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
	void inject_event_key_change(Key key, int action, uint mods, uint scancode)
	{
		for(auto l : m_listeners)
			l->event_key_change(key,action,mods,scancode);
	}
	void inject_event_character_input(unsigned int c, KeyActions action, uint mods){}
	void inject_event_mouse_button(uint button, KeyActions action ,uint mods){}
	void inject_event_mouse_scroll(double /*xScroll*/, double /*yScroll*/){}
	void inject_event_mouse_move(double x, double y){}
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
