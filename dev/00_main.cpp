#include <cstdio>
#include "ogl_app.h"
#include "ogl_utils.h"
#include "ogl_geo.h"
#include "font.h"
#include "vd.h"
#include "vdman.h"
#include "vmath.h"
#include "tnvb.h"
using namespace  ofl;
#include "ogl_text.h"
#include <stb_image.h>
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
	OpenGLTextFieldRenderer* tfr;
	OpenGLTextField* tf;

public:
	Application()
	{
		init();
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		int w, h, c;
		auto fnt = stbi_load("../../resources/fonts/whitrabt_df.png",&w,&h,&c,1);

		tfr=new OpenGLTextFieldRenderer(fnt,w,h);

		free(fnt);

		tf =tfr->create_text_field(6,1);
		tf->set_model_matrix(glm::translate(mat4(1),glm::vec3(0.0f,1.0f,0.0f)));
		tf->set_anchor(vec2(0.5,1));
		tf->set_bg_color(vec4(0,0,0,0.5));

		auto t = tfr->create_text_field(80,30);
		t->write_text("Franz jagt im komplett verwahrlosten Taxi quer durch Bayern! FRANZ JAGT IM KOMPLETT VERWAHRLOSTEN TAXI QUER DURCH BAYERN!");
		t->set_anchor(vec2(0.5,0.5));


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

		tf->write_text(std::to_string(1.0/tslf_s),0);
		tfr->render();

		this->m_context.swapBuffers();
		return m_goon;
	}


	void event_keyboard(const Key k, const int, const uint, const uint)
	{
		if(k == Key::ESCAPE || k == Key::Q)
			m_goon = false;
	}

	void event_window_resized(const int w, const int h)
	{
		glViewport(0,0,w,h);
		pm  = glm::perspective(1.0f,static_cast<float>(w)/h,0.01f,100.0f);
		setUniform(2,pm);
	}
};

void print(const mat4& m)
{
	auto f = glm::value_ptr(m);
	for(int i = 0 ; i <16;i++)
	{

		if(i%4==0)
			printf(" | ");
		printf("%f,",f[i]);

	}
}

int main(int argc, char** argv)
{

	Application app;
	app.start_rendering();

	return 0;
}

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
