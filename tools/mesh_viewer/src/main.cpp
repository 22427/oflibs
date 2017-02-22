
#include <cstdio>
#include <fstream>
#include <set>

#include <ofl_ogl.h>


using namespace ofl;

class App : public ofl::OpenGLApplication
{



public:
	bool render_one_frame(double tslf_s)
	{
		glClearColor(1,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_context.swapBuffers();
		return true;
	}
};
int main(){
	App a;
	a.start_rendering();
	return 0;
}

#define OFL_IMPLEMENTATION
#include <ofl_ogl.h>
