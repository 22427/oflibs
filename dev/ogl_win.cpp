#include "ogl_win.h"

namespace ofl
{
#ifdef linux
Win::Win()
{

	m_display = nullptr;
	m_visinfo = nullptr;
	m_fbcfg = nullptr;
	m_win = m_screen = 0;

}


Win::~Win()
{
	XFree(m_visinfo);

	XDestroyWindow(m_display,m_win);
	XCloseDisplay(m_display);

	memset(m_key_states,0,Key::MAX_KEYS);
	memset(m_button_states,0,16);
	m_key_modifiers = 0;
};

void Win::init()
{
	m_display = XOpenDisplay(nullptr);
	if ( !m_display )
		printf( "Cannot open X display\n" );

	m_screen   = DefaultScreen( m_display );
	::Window m_root = RootWindow( m_display, m_screen );

	if ( !glXQueryExtension( m_display, 0, 0 ) )
		printf( "X Server doesn't support GLX extension\n" );

	int attribs [ 100 ] ;
	attribs[0]  = GLX_X_RENDERABLE  ; attribs[1]  = True;
	attribs[2]  = GLX_DRAWABLE_TYPE ; attribs[3]  = GLX_WINDOW_BIT;
	attribs[4]  = GLX_RENDER_TYPE   ; attribs[5]  = GLX_RGBA_BIT;
	attribs[6]  = GLX_RED_SIZE      ; attribs[7]  = window_attributes.red_bits;
	attribs[8]  = GLX_GREEN_SIZE    ; attribs[9]  = window_attributes.green_bits;
	attribs[10] = GLX_BLUE_SIZE     ; attribs[11] = window_attributes.blue_bits;
	attribs[12] = GLX_ALPHA_SIZE    ; attribs[13] = window_attributes.alpha_bits;
	attribs[14] = GLX_DEPTH_SIZE    ; attribs[15] = window_attributes.depth_bits;
	attribs[16] = GLX_STENCIL_SIZE  ; attribs[17] = window_attributes.stencil_bits;
	attribs[18] = GLX_DOUBLEBUFFER  ; attribs[19] = window_attributes.double_buffer?True:False;
	attribs[20] = GLX_SAMPLE_BUFFERS; attribs[21] = 1;
	attribs[22] = GLX_SAMPLES       ; attribs[23] = window_attributes.samples;
	attribs[24] = GLX_STEREO        ; attribs[25] = window_attributes.stereo?True:False;
	attribs[26] = GLX_AUX_BUFFERS   ; attribs[27] = window_attributes.aux_buffers;
	attribs[28] = GLX_ACCUM_RED_SIZE    ; attribs[29] = window_attributes.accum_red_bits;
	attribs[30] = GLX_ACCUM_GREEN_SIZE  ; attribs[31] = window_attributes.accum_green_bits;
	attribs[32] = GLX_ACCUM_BLUE_SIZE   ; attribs[33] = window_attributes.accum_blue_bits;
	attribs[34] = GLX_ACCUM_ALPHA_SIZE  ; attribs[35] = window_attributes.accum_alpha_bits;
	attribs[36] = None;


	int fbcount;
	GLXFBConfig *fbc = glXChooseFBConfig( m_display, m_screen,
										  attribs, &fbcount );
	if ( fbc )
	{
		if ( fbcount >= 1 )
			m_fbcfg = fbc[0];
		else
			printf("TODO: error no fbc found!\n");
		XFree( fbc );
	}
	else
		printf("TODO: error no fbc found!\n");


	m_visinfo = glXGetVisualFromFBConfig( m_display, m_fbcfg );
	if ( !m_visinfo )
	{
		printf( "Failed to get XVisualInfo\n" );
	}

	XSetWindowAttributes winAttr ;
	winAttr.border_pixel = window_attributes.border;
	winAttr.background_pixel = 0;
	winAttr.background_pixmap = 0;
	winAttr.event_mask  = StructureNotifyMask | ClientMessage;
	winAttr.colormap = XCreateColormap(m_display, m_root,m_visinfo->visual, AllocNone );



	if(window_attributes.event_mask & EM_MOUSE_IO)
		winAttr.event_mask |= ButtonPressMask |ButtonReleaseMask| ButtonMotionMask | PointerMotionMask;
	if(window_attributes.event_mask & EM_KEYBOARD_IO)
		winAttr.event_mask |= KeyPressMask | KeyReleaseMask;
	if(window_attributes.event_mask & EM_RESIZE)
		winAttr.event_mask |= ResizeRedirectMask ;
	if(window_attributes.event_mask & EM_REPOSITION)
		winAttr.event_mask |= ConfigureNotify;


	winAttr.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
			PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
			ExposureMask | FocusChangeMask | VisibilityChangeMask |
			EnterWindowMask | LeaveWindowMask | PropertyChangeMask;



	unsigned int mask = CWBackPixmap | CWBorderPixel | CWColormap | CWEventMask;

	m_win = XCreateWindow ( m_display, m_root,
							window_attributes.pos_x, window_attributes.pos_y,
							window_attributes.width, window_attributes.height, window_attributes.border,
							m_visinfo->depth, InputOutput,
							m_visinfo->visual, mask, &winAttr ) ;


	wm_protocols     = XInternAtom( m_display, "WM_PROTOCOLS"    , False);
	wm_delete_window = XInternAtom( m_display, "WM_DELETE_WINDOW", False);
	net_wm_ping = XInternAtom(m_display, "_NET_WM_PING", False);

	Atom ps[] = {wm_delete_window,net_wm_ping};
	XSetWMProtocols( m_display,m_win, ps, 2);



	// set the window title
	XStoreName(m_display,m_win,window_attributes.title.c_str());
	// set the decoration
	auto mh = XInternAtom(m_display, "_MOTIF_WM_HINTS", False);
	if (window_attributes.decorated)
	{
		XDeleteProperty(m_display,
						m_win,
						mh);
	}
	else
	{
		struct
		{
			unsigned long flags;
			unsigned long functions;
			unsigned long decorations;
			long input_mode;
			unsigned long status;
		} hints;

		hints.flags = 2;       // Set decorations
		hints.decorations = 0; // No decorations

		XChangeProperty(m_display, m_win,
						mh,
						mh, 32,
						PropModeReplace,
						(unsigned char*) &hints,
						sizeof(hints) / sizeof(long));
	}
}

void Win::add_listener(WinListener *l)
{
	m_listeners.push_back(l);
}

void Win::process_events()
{
	int count = XPending(m_display);
	while (count--)//XEventsQueued( m_display, QueuedAfterFlush ) )
	{
		XEvent    event;
		XNextEvent( m_display, &event );
		if( event.xany.window != m_win )
			continue;

		if (event.type==KeyPress)
		{
			XKeyEvent* kv = reinterpret_cast<XKeyEvent*>(&event);

			KeySym sym;

			char chars[4];
			memset(chars,0,4);
			auto r = XLookupString(kv,chars,4,&sym,nullptr);

			if(sym==Key::LEFT_SHIFT || sym==Key::RIGHT_SHIFT)
				m_key_modifiers=m_key_modifiers| Key::MOD_SHIFT;
			else if(sym==Key::LEFT_CONTROL || sym==Key::RIGHT_CONTROL)
				m_key_modifiers=m_key_modifiers| Key::MOD_CTRL;
			else if(sym==Key::LEFT_ALT || sym==Key::RIGHT_ALT)
				m_key_modifiers=m_key_modifiers| Key::MOD_ALT;
			else if(sym==Key::LEFT_SUPER || sym==Key::RIGHT_SUPER)
				m_key_modifiers=m_key_modifiers| Key::MOD_SUPER;

			uint kc = sym&0x1ff;

			if(m_key_states[kc])
			{
				inject_event_key_change(kc,Key::REPEAT,m_key_modifiers,kv->keycode);
				if(r)
					inject_event_character_input(static_cast<uint32_t>(chars[0]),Key::REPEAT,m_key_modifiers);
			}
			else
			{
				inject_event_key_change(kc,Key::PRESSED,m_key_modifiers,kv->keycode);
				if(r)
					inject_event_character_input(static_cast<uint32_t>(chars[0]),Key::PRESSED,m_key_modifiers);
				m_key_states[kc] = true;
			}


		}
		else if (event.type==KeyRelease)
		{
			XKeyEvent* kv = reinterpret_cast<XKeyEvent*>(&event);
			KeySym sym;
			char chars[4];
			memset(chars,0,4);
			XLookupString(kv,chars,4,&sym,nullptr);

			if(sym==Key::LEFT_SHIFT || sym==Key::RIGHT_SHIFT)
				m_key_modifiers=m_key_modifiers &~static_cast<uint>(Key::MOD_SHIFT);
			else if(sym==Key::LEFT_CONTROL || sym==Key::RIGHT_CONTROL)
				m_key_modifiers=m_key_modifiers &~static_cast<uint>(Key::MOD_CTRL);
			else if(sym==Key::LEFT_ALT || sym==Key::RIGHT_ALT)
				m_key_modifiers=m_key_modifiers & ~static_cast<uint>(Key::MOD_ALT);
			else if(sym==Key::LEFT_SUPER || sym==Key::RIGHT_SUPER)
				m_key_modifiers=m_key_modifiers & ~static_cast<uint>(Key::MOD_SUPER);

			uint kc = sym&0x1ff;
			inject_event_key_change(kc,Key::RELEASED,m_key_modifiers,kv->keycode);
			m_key_states[kc]=false;
		}
		else if (event.type == ResizeRequest)
		{
			inject_event_resized(event.xresizerequest.width,event.xresizerequest.height);
		}
		else if(event.type == ConfigureNotify)
		{
			inject_event_moved(event.xconfigure.x,event.xconfigure.y);
		}
		else if(event.type == MotionNotify)
		{
			inject_event_mouse_move(event.xmotion.x,event.xmotion.y);
		}
		else if(event.type == ButtonPress)
		{
			const auto& bev = event.xbutton;
			if(m_button_states[bev.button])
				inject_event_mouse_button(bev.button,Key::REPEAT,m_key_modifiers,bev.x,bev.y);
			else
			{
				inject_event_mouse_button(bev.button,Key::PRESSED,m_key_modifiers,bev.x,bev.y);
				m_button_states[bev.button] = true;
			}
		}
		else if(event.type == ButtonRelease)
		{
			const auto& bev = event.xbutton;
			inject_event_mouse_button(bev.button,Key::RELEASED,m_key_modifiers,bev.x,bev.y);
			m_button_states[bev.button] = false;
		}
		else if(event.type == ClientMessage)
		{
			const auto & c = event.xclient;
			Atom protocol = None;
			if(c.message_type == wm_protocols)
			{
				protocol = c.data.l[0];
				if (protocol == None)
					return;
			}

			if (protocol == wm_delete_window)
			{
				fflush(stdout);
				inject_event_window(WIN_CLOSE);
			}
			else if (protocol == net_wm_ping)
			{
				// The window manager is checking if we are still here ...
				// lets say yes!
				XEvent reply = event;
				reply.xclient.window = m_root;

				XSendEvent(m_display, m_root,
						   False,
						   SubstructureNotifyMask | SubstructureRedirectMask,
						   &reply);
			}


		}
		else if(event.type == PropertyNotify)
		{
			// todo handle this ....
		}
		else
		{
			// handle other events
		}
	}
}

void Win::inject_event_window(Win::WindowEvent event)
{
	for(auto l : m_listeners)
		l->event_window(event);
}

void Win::inject_event_moved(const int x, const int y)
{
	for(auto l : m_listeners)
		l->event_window_moved(x,y);
}

void Win::inject_event_resized(const int w, const int h)
{
	for(auto l : m_listeners)
		l->event_window_resized(w,h);
}

void Win::inject_event_key_change(const Key key, const int action, const uint mods, const uint scancode)
{
	if(on_key_change)
		on_key_change(key,action,mods,scancode);
}

void Win::inject_event_character_input(const unsigned int c, const Key::KeyActions action, const uint mods)
{
	if(on_char_input)
		on_char_input(c,action,mods);
}

void Win::inject_event_mouse_button(const uint button, const Key::KeyActions action, const uint mods, const double x, const double y)
{
	if(on_mouse_button)
		on_mouse_button(button,action,mods,x,y);
}

void Win::inject_event_mouse_scroll(const double x_scroll, const double y_scroll)
{
	if(on_mouse_scroll)
		on_mouse_scroll(x_scroll,y_scroll);
}

void Win::inject_event_mouse_move(const double x, const double y)
{
	if(on_mouse_move)
		on_mouse_move(x,y);
}

GLXContext createContext(Display *display, int screen, GLXFBConfig fbconfig, int majorv, int minorv, bool core, bool debug)
{
#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
	typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

	// Verify GL driver supports glXCreateContextAttribsARB()
	//   Create an old-style GLX context first, to get the correct function ptr.
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;

	if ( strstr( glXQueryExtensionsString( display, screen ),
				 "GLX_ARB_create_context" ) != 0 )
		glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
				glXGetProcAddress( (const GLubyte *) "glXCreateContextAttribsARB" );


	static int context_attribs[12];

	context_attribs[0] = GLX_CONTEXT_MAJOR_VERSION_ARB;  context_attribs[1] =majorv;
	context_attribs[2] = GLX_CONTEXT_MINOR_VERSION_ARB;  context_attribs[3] =minorv;
	context_attribs[4] = GLX_CONTEXT_PROFILE_MASK_ARB;
	if(core)
		context_attribs[5] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
	else
		context_attribs[5] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;

	context_attribs[6] = GLX_CONTEXT_FLAGS_ARB;  context_attribs[7] =0;
	if(debug)
		context_attribs[7] = context_attribs[7]| GLX_CONTEXT_DEBUG_BIT_ARB;
	context_attribs[8] = None;

	//GLX_CONTEXT_FLAGS_ARB       , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,

	GLXContext context = glXCreateContextAttribsARB( display, fbconfig, 0,
													 True, context_attribs );

	// Forcably wait on any resulting X errors
	XSync( display, False );

	if ( !context )
	{
		printf( "Failed to allocate a GL context\n" );
		return nullptr;
	}
	return context;
}


GLContext::~GLContext()
{
	if(m_window)
		glXDestroyContext(m_window->m_display,m_cntxt);
}

void GLContext::init()
{
	m_cntxt = createContext(
				m_window->m_display,
				m_window->m_screen,
				m_window->m_fbcfg,
				opengl_version_major,
				opengl_version_minor,
				opengl_profile==CORE,
				opengl_debug_context);

	if ( ! glXMakeCurrent( m_window->m_display, m_window->m_win, m_cntxt ) )
		printf( "glXMakeCurrent failed.\n" );

	if ( ! glXIsDirect ( m_window->m_display, glXGetCurrentContext() ) )
		printf( "Indirect GLX rendering context obtained\n" );

	// Display the window
	XMapWindow( m_window->m_display, m_window->m_win );

	if ( ! glXMakeCurrent( m_window->m_display, m_window->m_win, m_cntxt) )
		printf( "glXMakeCurrent failed.\n" );
	auto res = gladLoadGL();
	printf("glad load GL: %d\n",res);
}

void GLContext::swapBuffers()
{
	glXSwapBuffers(m_window->m_display,m_window->m_win);
}

void GLContext::makeCurrent()
{
	glXMakeCurrent( m_window->m_display, m_window->m_win, m_cntxt);
}

Win::WindowAttributes::WindowAttributes()
{
	title = "Window";
	width = 800;
	height = 600;
	pos_x = 80;
	pos_y = 60;
	decorated = true;
	border = 0;
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
	stereo = false;
	srgb = false ;
	double_buffer = true;
	event_mask = 0;
}

#else
//TODO wgl implementation!!!!
#endif
}
