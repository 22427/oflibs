#include <cstdio>
#include "ogl_app.h"
#include "ogl_utils.h"
#include "ogl_geo.h"
#include "vd.h"
#include "vdman.h"
using namespace  ofl;


std::string vs =R"code(#version 430
layout(location = 0) uniform mat4 model_matrix;
layout(location = 1) uniform mat4 view_matrix;
layout(location = 2) uniform mat4 projection_matrix;
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_nrm;
layout(location = 2) in vec2 a_tex;
layout(location = 3) in vec4 a_color;
layout(location = 4) in vec3 a_tan;
out vec4 v_color;
void main()
{
	gl_Position = projection_matrix*view_matrix*model_matrix * vec4(a_pos,1.0);
	v_color = a_color;
}
)code";


std::string fs =R"code(#version 430
in vec4 v_color;
out vec4 f_color;
void main()
{
	f_color = v_color;
}
)code";

class Application : public OpenGLApplication
{

	bool m_goon;
	ShaderProgram* m_sp;
	mat4 mm;
	mat4 vm;
	mat4 pm;
	Geometry* geo;

public:
	Application()
	{
		init();
		m_sp = new ShaderProgram();
		m_goon = true;
		glClearColor(1.0f,0.2f,0.2f,1.0f);
		auto shader = Shader(GL_VERTEX_SHADER,vs);
		if(!shader.compile())
			printf("Compiling VS failed:\n%s\n",shader.read_log().c_str());
		m_sp->attach_shader(shader);

		auto fshader = Shader(GL_FRAGMENT_SHADER,fs);
		if(!fshader.compile())
			printf("Compiling FS failed:\n%s\n",fshader.read_log().c_str());
		m_sp->attach_shader(fshader);

		if(!m_sp->link())
		{
			printf("linking failed:\n%s\n",m_sp->read_log().c_str());
		}
		fflush(stdout);
		m_sp->use();
		pm = glm::perspective(1.0f,4.0f/3.0f,0.01f,100.0f);
		vm = glm::lookAt(vec3(50,50,50),vec3(0,0,0),vec3(0,1,0));

		setUniform(0,mm);
		setUniform(1,vm);
		setUniform(2,pm);

		VertexDataManufactory fac;
		geo = new Geometry(fac.create_uv_sphere(25.0f),true);
	}


	void event_window(Win::WindowEvent ev)
	{
		if(ev ==Win::WIN_CLOSE )
			m_goon = false;
	}
	bool render_one_frame(double tslf_s)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		geo->draw();

		this->m_context.swapBuffers();
		return m_goon;
	}


	void event_keyboard(const Key k, const int, const uint, const uint)
	{
		if(k == Key::ESCAPE || k == Key::Q)
			m_goon = false;
	}
};
int main()
{
	Application app;
	app.start_rendering();


	return 0;
}
