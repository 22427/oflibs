#pragma once
#include <chrono>
#include "ogl_win.h"

namespace ofl
{
class OpenGLApplication : public WinListener, public UserIOListeners
{
protected:
	Win m_window;
	WinIO m_io;

	GLContext m_context;
public:

	virtual void event_keyboard(const Key /*key*/,const int /*action*/,const  uint /*mods*/,const  uint /*scancode*/){}
	virtual void event_character(const unsigned int /*c*/,const  Key::KeyActions /*action*/,const  uint /*mods*/){}
	virtual void event_mouse_button(const uint /*button*/, const int /*action*/,const uint /*mods*/,const double /*x*/, const double /*y*/){}
	virtual void event_mouse_scroll(double /*xScroll*/, double /*yScroll*/){}
	virtual void event_mouse_move(double /*x*/, double /*y*/){}

	virtual void event_window(Win::WindowEvent /*event*/){}
	virtual void event_window_moved(const int /*x*/, int /*y*/){}
	virtual void event_window_resized(const int /*w*/,const  int /*h*/){}

	virtual bool render_one_frame(double tslf_s) = 0;

	OpenGLApplication() :m_context(&m_window),m_io(&m_window)
	{

	}

	void init()
	{
		m_window.init();
		m_context.init();
	}

	void start_rendering()
	{
		auto start = std::chrono::high_resolution_clock::now();
		auto finish = std::chrono::high_resolution_clock::now();
		double tslf;
		do
		{
			finish = std::chrono::high_resolution_clock::now();
			tslf = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
			start = finish;
			m_window.process_events();
		}while(render_one_frame(tslf));
	}

};
}
