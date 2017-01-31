#include <cstdio>
#include "ogl_app.h"

using namespace  ofl;

class Application : public OpenGLApplication
{

	bool m_goon;
public:
	Application()
	{
		init();
		m_goon = true;
		glClearColor(1.0f,1.0f,1.0f,1.0f);
	}

	void event_window(Win::WindowEvent ev)
	{
		if(ev ==Win::WIN_CLOSE )
			m_goon = false;
	}
	bool render_one_frame(double tslf_s)
	{
		glClear(GL_COLOR_BUFFER_BIT);

	//	this->m_context.swapBuffers();
		return m_goon;
	}
};
int main()
{
	Application app;
	app.start_rendering();
//	Win* w = new Win();
//	w->init();
//	GLContext *cntxt = new GLContext(w);
//	cntxt->init();

//	int i = 0 ;
//	for(;;)
//	{
//		if(i%2)
//			glClearColor(1.f,1.f,1.f,1.f);
//		else
//			glClearColor(0.f,0.f,0.f,1.f);
//		glClear(GL_COLOR_BUFFER_BIT);
//		cntxt->swapBuffers();
//		w->process_events();
//	}

//	delete cntxt;
//	delete w;


	return 0;
}
