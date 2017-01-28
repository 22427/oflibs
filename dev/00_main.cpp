#include <cstdio>
#include "ogl_win.h"

using namespace  ofl;
int main()
{
	Win* w = new Win();
	w->init();
	GLContext *cntxt = new GLContext(w);
	cntxt->init();

	int i = 0 ;
	for(;;)
	{
		if(i%2)
			glClearColor(1.f,1.f,1.f,1.f);
		else
			glClearColor(0.f,0.f,0.f,1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		cntxt->swapBuffers();
		w->process_events();
	}

	delete cntxt;
	delete w;


	return 0;
}
