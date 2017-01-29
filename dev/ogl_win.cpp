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
	XFree(m_fbcfg);
	XDestroyWindow(m_display,m_win);
	XCloseDisplay(m_display);
	memset(m_key_states,0,512);
	m_key_modifiers = 0;
};

void Win::init()
{
	m_display = XOpenDisplay( ":0.0");
	if ( !m_display )
		printf( "Cannot open X display\n" );

	m_screen   = DefaultScreen( m_display );
	::Window root_win = RootWindow( m_display, m_screen );

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
	winAttr.event_mask = StructureNotifyMask | KeyPressMask | ButtonPressMask | ButtonMotionMask | PointerMotionMask | ResizeRedirectMask;
	winAttr.colormap = XCreateColormap(m_display, root_win,m_visinfo->visual, AllocNone );


	unsigned int mask = CWBackPixmap | CWBorderPixel | CWColormap | CWEventMask;

	m_win = XCreateWindow ( m_display, root_win,
							window_attributes.pos_x, window_attributes.pos_y,
							window_attributes.width, window_attributes.height, window_attributes.border,
							m_visinfo->depth, InputOutput,
							m_visinfo->visual, mask, &winAttr ) ;
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

void Win::process_events()
{
	while ( XEventsQueued( m_display, QueuedAfterFlush ) )
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
				m_key_modifiers=m_key_modifiers| OFL_SHIFT;
			else if(sym==Key::LEFT_CONTROL || sym==Key::RIGHT_CONTROL)
				m_key_modifiers=m_key_modifiers| OFL_CTRL;
			else if(sym==Key::LEFT_ALT || sym==Key::RIGHT_ALT)
				m_key_modifiers=m_key_modifiers| OFL_ALT;
			else if(sym==Key::LEFT_SUPER || sym==Key::RIGHT_SUPER)
				m_key_modifiers=m_key_modifiers| OFL_SUPER;

			uint kc = sym&0x1ff;

			if(m_key_states[kc])
			{
				inject_event_key_change(kc,OFL_REPEAT,m_key_modifiers,kv->keycode);
				if(r)
					inject_event_character_input(static_cast<uint32_t>(chars[0]),OFL_REPEAT,m_key_modifiers);
			}
			else
			{
				inject_event_key_change(kc,OFL_PRESSED,m_key_modifiers,kv->keycode);
				if(r)
					inject_event_character_input(static_cast<uint32_t>(chars[0]),OFL_PRESSED,m_key_modifiers);
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
				m_key_modifiers=m_key_modifiers &~static_cast<uint>(OFL_SHIFT);
			else if(sym==Key::LEFT_CONTROL || sym==Key::RIGHT_CONTROL)
				m_key_modifiers=m_key_modifiers &~static_cast<uint>(OFL_CTRL);
			else if(sym==Key::LEFT_ALT || sym==Key::RIGHT_ALT)
				m_key_modifiers=m_key_modifiers & ~static_cast<uint>(OFL_ALT);
			else if(sym==Key::LEFT_SUPER || sym==Key::RIGHT_SUPER)
				m_key_modifiers=m_key_modifiers & ~static_cast<uint>(OFL_SUPER);

			uint kc = sym&0x1ff;
			inject_event_key_change(kc,OFL_RELEASED,m_key_modifiers,kv->keycode);
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
			inject_event_mouse_button(event.xbutton.button,OFL_PRESSED,m_key_modifiers);
		}
		else if(event.type == ButtonRelease)
		{
			inject_event_mouse_button(event.xbutton.button,OFL_RELEASED,m_key_modifiers);
		}
		else
		{
			printf("unknown: %d\n",event.type);
		}



	}
}

GLXContext createContext(Display *display, int screen, GLXFBConfig fbconfig, XVisualInfo *visinfo, ::Window window, int majorv, int minorv, bool core, bool debug)
{
#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
	typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

	// Verify GL driver supports glXCreateContextAttribsARB()
	//   Create an old-style GLX context first, to get the correct function ptr.
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;

	GLXContext ctx_old = glXCreateContext( display, visinfo, 0, True );
	if ( !ctx_old )
	{
		printf( "Could not even allocate an old-style GL context!\n" );
		return nullptr;
	}


	glXMakeCurrent ( display, window, ctx_old ) ;

	// Verify that GLX implementation supports the new context create call
	if ( strstr( glXQueryExtensionsString( display, screen ),
				 "GLX_ARB_create_context" ) != 0 )
		glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
				glXGetProcAddress( (const GLubyte *) "glXCreateContextAttribsARB" );

	if ( !glXCreateContextAttribsARB )
	{
		printf( "Can't create new-style GL context\n" );
		exit(1);
	}

	// Got the pointer.  Nuke old context.
	glXMakeCurrent( display, None, 0 );
	glXDestroyContext( display, ctx_old );

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
				m_window->m_visinfo,
				m_window->m_win,
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
	gladLoadGL();
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
}

#else
//TODO wgl implementation!!!!
#endif
}
