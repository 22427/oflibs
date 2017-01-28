
#include <cstdio>
#include <fstream>
#include <set>

#include <ofl_ogl.h>

#if 0
using namespace ofl;

class MeshViewer :
		public ofl::Window_GLFW,
		public ofl::StateSimulator
{
	bool render_on;
	Geometry* to_show;
	float to_show_angle;
	unsigned int fno;
	float scaling;
	glm::vec3 origin;
	glm::vec3 cam_pos;
public:
	MeshViewer(ofl::WindowPreferences* wp) : Window_GLFW(wp)
	{
		fno = 0;
		to_show_angle= 0;
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.25f,0.225f,0.2f,1.0f);

		matrixMode(PROJECTION);
		loadIdentity();
		perspective(45.0f,
					static_cast<float>(getWidth())/
					static_cast<float>(getHeight()),
					0.001f,10.0f);
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
		VertexData* vd;
		vd = VertexDataOperations::read_from_file(argv[1]);
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
			vec3 p;
			v.get_value(ATTRIB_POSITION,p);
			for(int i = 0 ; i<3;i++)
			{
				bb_min[i] = std::min(p[i],bb_min[i]);
				bb_max[i] = std::max(p[i],bb_max[i]);
			}

		}

		origin = (bb_min+bb_max)*0.5f;
		origin.y = bb_min.y;
#define fmax(x,y) (x>y? x:y)
		scaling =  1.0f/(fmax(
					fmax(fmax(bb_min.x,bb_min.y),fmax(bb_max.x,bb_max.y)),
					fmax(bb_max.z,bb_min.z)));
#undef fmax
		to_show = new Geometry(vd,true);
		enterRenderLoop();
		return 0;
	}


	virtual void eventWindow(int event);

	bool renderAFrame(double tslf_s)
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->matrixMode(MODELVIEW);

		this->pushMatrix();
		to_show_angle += fmod(tslf_s*0.5,2.0*M_PI);

		scale(scaling,scaling,scaling);
		rotate(to_show_angle,0,1,0);
		translate(-origin);
		to_show->draw();
		this->popMatrix();
		this->swapBuffers();
		return render_on;

	}
	void eventCharacter(unsigned int c, int )
	{
		if(c == 'q')
			render_on = false;
		else if(c == 't')
			enable(Texturing);
		else if(c == 'T')
			disable(Texturing);
		else if(c == 'd')
			enable(DFAlpha);
		else if(c == 'D')
			disable(DFAlpha);
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
	void eventMouseScroll(double /*x*/, double y)
	{
		matrixMode(MODELVIEW);
		loadIdentity();

		vec3 cam_dir = glm::normalize(cam_pos);
		float len = glm::length(cam_pos);
		len *= 1.0-(y*0.05);
		cam_pos = cam_dir*len;
		lookAt(cam_pos.x,cam_pos.y,cam_pos.z,0,0.5,0,0,1,0);
	}
	void eventWindowSize(int w, int h)
	{
		glViewport(0,0,w,h);
		this->matrixMode(PROJECTION);
		loadIdentity();

		perspective(45.0f,static_cast<float>(w)/static_cast<float>(h),0.001f,10.0f);


	}
};

int main(int argc, char** argv)
{
	ofl::WindowPreferences wp;
	wp.stereo = false;
	MeshViewer mv(&wp);
	std::vector<std::string> args;
	for(int i = 0 ; i< argc;i++)
	{
		args.push_back(argv[i]);
	}
	return mv.go(args);
}

#define OFL_IMPLEMENTATION
#include <ofl_all.h>

void MeshViewer::eventWindow(int event)
{
	if(event == WIN_CLOSE)
		render_on = false;
}
#endif
int main(){return 0;}
