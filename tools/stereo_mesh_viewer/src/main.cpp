
#include <cstdio>
#include <fstream>
#include <set>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ofl_ogl_win.h>
#include <ofl_vd.h>
#include <ofl_vdman.h>
#include <ofl_ogl_state.h>
#include <ofl_ogl_geo.h>
#include <ofl_ogl_stereo_compositor.h>


using namespace ofl;

class StereoMeshViewer : public ofl::VertexDataTools, public ofl::Window_GLFW, public ofl::StateSimulator, public StereoCompositor
{
	bool render_on;
	Geometry* to_show;
	float to_show_angle;
	unsigned int fno;
	float scaling;
	glm::vec3 origin;
	glm::vec3 cam_pos;
	glm::mat4 projection_left;
	glm::mat4 projection_right;
public:
	StereoMeshViewer(ofl::WindowPreferences* wp) : Window_GLFW(wp)
	{
		fno = 0;
		to_show_angle= 0;
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.25,0.225,0.2,1);

		matrixMode(PROJECTION);
		loadIdentity();
		perspective(1.5f,(float)getWidth()/(float)getHeight(),0.001f,10.0f);
		enable(Lighting);
		cam_pos = vec3(0,0.5,3);
		matrixMode(MODELVIEW);
		loadIdentity();
		lookAt(cam_pos.x,cam_pos.y,cam_pos.z,0,0.5,0,0,1,0);

		setLightPosition(0,glm::vec4(1.0,1.0,2.0,1));
		setLightDiffuseColor(0,glm::vec3(1.0,1.0,1.0));
		setLightSpecularColor(0,glm::vec3(1.0,0.75,0.2));
		setLightAmbientColor(0,glm::vec3(0.1,0.1,0.1));
		setLightAttenuationFactors(0,glm::vec4(1,1,0,1));
		render_on = true;
	}

	int go(std::vector<std::string> argv)
	{
		if(argv.size() < 2)
			return 1;
		this->eventWindowSize(getWidth(),getHeight());
		VertexData* vd;
		vd = readFromFile(argv[1]);
		if(!vd)
			return 1;
		glm::vec3 bb_min(std::numeric_limits<float>::max(),
					std::numeric_limits<float>::max(),
					std::numeric_limits<float>::max());
		glm::vec3 bb_max(std::numeric_limits<float>::min(),
					std::numeric_limits<float>::min(),
					std::numeric_limits<float>::min());
		for(const Vertex& v : *vd)
		{
			const vec3& p = v.pos();
			for(int i = 0 ; i<3;i++)
			{
				bb_min[i] = std::min(p[i],bb_min[i]);
				bb_max[i] = std::max(p[i],bb_max[i]);
			}

		}

		origin = (bb_min+bb_max)*0.5f;
		origin.y = bb_min.y;
#define fmax(x,y) std::max(fabs(x),fabs(y))
		scaling =  1.0f/(fmax(
					fmax(fmax(bb_min.x,bb_min.y),fmax(bb_max.x,bb_max.y)),
					fmax(bb_max.z,bb_min.z)));
#undef fmax
		to_show = new Geometry(vd,true);
		enterRenderLoop();
		return 0;
	}


	virtual void eventWindow(int event)
	{
		if(event == WIN_CLOSE)
			render_on = false;
	}

	void renderObject(double tslf_s)
	{
		this->matrixMode(MODELVIEW);
		this->pushMatrix();

		to_show_angle += fmod(tslf_s*0.5,2.0*M_PI);

		scale(scaling,scaling,scaling);
		rotate(to_show_angle,0,1,0);
		translate(-origin);
		to_show->draw();
		this->popMatrix();

	}

	virtual bool renderAFrame(double tslf_s)
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		this->useCurrentShader();


		this->matrixMode(MODELVIEW);
		this->pushMatrix();
		this->translate( 0.04f,0,0);

		this->matrixMode(PROJECTION);
		setEye(Left);
		this->loadMatrix(projection_left);

		renderObject(tslf_s);

		this->matrixMode(MODELVIEW);
		this->translate(-0.08f,0,0);

		this->matrixMode(PROJECTION);
		setEye(Right);
		this->loadMatrix(projection_right);

		renderObject(tslf_s);

		this->matrixMode(MODELVIEW);
		this->popMatrix();
		this->swapBuffers();
		return render_on;

	}
	virtual void eventCharacter(unsigned int c, int )
	{
		if(c == 'q')
			render_on = false;
		else if(c == 't')
			enable(Texturing);
		else if(c == 'T')
			disable(Texturing);
		else if(c == 'm')
		{
			int m = static_cast<int>(m_cmode)+1;
			int mc = static_cast<int>(MODE_COUNT);
			CompositingMode cm = static_cast<CompositingMode>(m % mc);
			this->setCompositingMode(cm);
		}
		else if(c == 'n')
		{
			float m = static_cast<float>(m_cmode)-1.0f;
			float mc = static_cast<float>(MODE_COUNT);
			m = mod(m,mc);
			CompositingMode cm = static_cast<CompositingMode>(m);
			this->setCompositingMode(cm);
		}
		else if(c == 'l')
			enable(Lighting);
		else if(c == 'L')
			disable(Lighting);
		else if(c == 'f')
			this->setFullscreen(true);
		else if(c == 'F')
			this->setFullscreen(false);
		else if(c == 'w')
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if(c == 'W')
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	virtual void eventMouseScroll(double /*x*/, double y)
	{
		matrixMode(MODELVIEW);
		loadIdentity();

		vec3 cam_dir = normalize(cam_pos);
		float len = length(cam_pos);
		len *= 1.0-(y*0.05);
		cam_pos = cam_dir*len;
		lookAt(cam_pos.x,cam_pos.y,cam_pos.z,0,0.5,0,0,1,0);
	}
	virtual void eventWindowSize(int w, int h)
	{
		glViewport(0,0,w,h);
		this->resize(w,h);

		projection_left = getAsymetricProjection(Left,0.08f,1.5f,0.001f,10.0f,0.75f);
		projection_right = getAsymetricProjection(Right,0.08f,1.5f,0.001f,10.0f,0.75f);

	}
};

int main(int argc, char** argv)
{
	ofl::WindowPreferences wp;
	StereoMeshViewer mv(&wp);
	std::vector<std::string> args;
	for(int i = 0 ; i< argc;i++)
	{
		args.push_back(argv[i]);
	}
	return mv.go(args);
}

#define OFL_IMPLEMENTATION
#include <ofl_ogl_win.h>
#include <ofl_vd.h>
#include <ofl_vdman.h>
#include <ofl_ogl_state.h>
#include <ofl_ogl_geo.h>
#include <ofl_ogl_stereo_compositor.h>
