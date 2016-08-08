#include <vector>
#include <string>
#include <fstream>

#include "ogl_state.h"

namespace ofl {


bool compileShader(GLuint shader)
{
	glCompileShader(shader);
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		printf("[ERR] Compiling shader failed: \n\t%s\n",errorLog.data());
		return false;
	}
	return true;
}

bool linkShader(GLuint shader_program)
{
	glLinkProgram(shader_program);
	GLint wasLinked ;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &wasLinked);
	if(wasLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(shader_program, maxLength, &maxLength, &errorLog[0]);
		printf("[ERR] Linking shader failed: \n\t%s\n",errorLog.data());
		return false;
	}
	return true;
}

std::string loadFile(const std::string& path)
{
	std::ifstream t(path);
	if(!t.is_open())
	{
		fprintf(stderr,"[ERR] Loading file \"%s\" failed!\n",path.c_str());
		return "";
	}
	std::string str = "";
	std::string line = "";
	while(std::getline(t,line))
	{
		str+= line + "\n";
	}
	return str;
}


ShaderWrap::ShaderWrap(GLuint handle)
{
	m_handle = handle;
}
ShaderWrap::~ShaderWrap()
{
	if(m_handle)
		glDeleteProgram(m_handle);
}

void ShaderWrap::learnUniform(const std::string& name, int id)
{
	m_uniformID2location[id]=glGetUniformLocation(m_handle, name.c_str());
	//	printf("s:%d,%s\t%d\t%d\n",m_handle, name.c_str(), m_uniformID2location[id],id);
}

int ShaderWrap::getUniformLocation(int id)
{
	if(m_uniformID2location.find(id) == m_uniformID2location.end())
	{
		return -2;
	}
	else
		return m_uniformID2location[id];
}

#define ___currMat m_matrix[m_matrixMode].top()
StateSimulator::~StateSimulator()
{
	for(int i = 0 ; i < NonStockShader;i++)
	{
		if(	m_stockshaders[i])
		{
			delete m_stockshaders[i];
			m_stockshaders[i] = nullptr;
		}
	}
}

void StateSimulator::enable (Feature f)
{
	printf("[ifo] %s: ON\n",to_string(f).c_str());
	
	if(m_features & f)
		return;
	m_features |= f;
	if(m_features < NonStockShader)
	{
		this->useShader(m_stockshaders[m_features]);
	}
}
void StateSimulator::disable(Feature f)
{
	printf("[ifo] %s: OFF\n",to_string(f).c_str());
	if(!(m_features & f))
		return;
	unsigned int mask = (unsigned int) -1;
	mask ^= f;
	m_features &= mask;
	if(m_features < NonStockShader)
	{
		this->useShader(m_stockshaders[m_features]);
	}
}

StateSimulator::StateSimulator(const std::string& path_to_stock_shaders)
{
	m_features = 0;
	m_matrixMode = MODELVIEW;
	for(int i = 0; i<MATRIXMODE_COUNT;i++)
	{
		m_matrix[i].push(glm::mat4(1));
		m_dirty_mats[i] = true;
	}
	for(int i = 0 ; i < NonStockShader;i++)
	{
		m_stockshaders[i] = nullptr;
	}
	for(int i = 0 ; i < NonStockShader+1;i++)
	{
		m_dirty_lights[i] = true;
	}
	// create the vertex shader for every stock program
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	std::string vshader_src = loadFile(path_to_stock_shaders+"stock.glsl.vs");
	
	const char* cstr = vshader_src.c_str();
	int len = vshader_src.length();
	glShaderSource(vshader,1,&cstr, &len);
	if(! compileShader(vshader))
	{
		glDeleteShader(vshader);
		return;
	}
	
	// Load all 4 stock shaders. They all share the same vertex stage
	// so we will load 4 different fragment-shaders and combine them with
	// the vertex-shader to 4 different shader-programms
	//
	for(int i = 0; i < NonStockShader ; i++)
	{
		// generate Fragment shaders
		GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
		// load the source code from a file
		std::string fshader_src = loadFile(path_to_stock_shaders+"stock_"+std::to_string(i)+".glsl.fs");
		cstr = fshader_src.c_str();
		len = fshader_src.length();
		// attach the code to the shader object
		glShaderSource(fshader,1,&cstr,&len);
		// compile and check for errors
		printf("[ifo] Compiling fragment shader for mode %d...\n",i);
		if(!compileShader(fshader))
		{
			printf("CODE:\n%s\n",cstr);
			glDeleteShader(fshader);
			return;
		}
		else
		{
			printf("[ifo] ... done!\n");
		}
		// generate the program.
		GLuint prog = glCreateProgram();
		// add vertex and fragment shader
		glAttachShader(prog,vshader);
		glAttachShader(prog,fshader);
		// bind the attribute locations. This is due to backwards
		// compatibility, with newer GL/GLSL versions you can define the
		// location in the shader.
		glBindAttribLocation(prog, ALOC_POSITION, NAME_POS_ATTR);
		glBindAttribLocation(prog, ALOC_NORMAL, NAME_NRM_ATTR);
		glBindAttribLocation(prog, ALOC_COLOR, NAME_CLR_ATTR);
		glBindAttribLocation(prog, ALOC_TEXCOORD, NAME_TEX_ATTR);
		// link the program
		linkShader(prog);
		// wrap the program and store it for later use
		this->m_stockshaders[i] = new ShaderWrap(prog);
		// use it for the first time to learn the uniform mappings
		this->useShader(m_stockshaders[i],true);
		// delete the fragment-shader we don't need it anymore
		glDeleteShader(fshader);
	}
	// Use stock-shader 0 (no lighting, no texturing)
	this->useShader(m_stockshaders[0]);
	// finally delete the vertex-shader it has served us well =)
	glDeleteShader(vshader);
}


void StateSimulator::m_upload_matrix()
{
	glUniformMatrix4fv(
				m_current_program->getUniformLocation(m_matrixMode),
				1,
				GL_FALSE,
				glm::value_ptr(___currMat));
	if(m_matrixMode == MODELVIEW)
	{
		auto normal_mat = glm::transpose(glm::inverse(m_matrix[MODELVIEW].top()));
		
		glUniformMatrix4fv(
					m_current_program->getUniformLocation(ID_NMM),
					1,
					GL_FALSE,
					glm::value_ptr(normal_mat));
	}
}
void StateSimulator::pushMatrix()
{
	m_matrix[m_matrixMode].push(m_matrix[m_matrixMode].top());
}
void StateSimulator::popMatrix()
{
	if(m_matrix[m_matrixMode].size() >1)
	{
		m_matrix[m_matrixMode].pop();
		m_upload_matrix();
		m_set_dirty_mats();
	}
}
void StateSimulator::loadIdentity()
{
	m_matrix[m_matrixMode].top() = glm::mat4(1);
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::matrixMode(MatrixMode m)
{
	m_matrixMode = m;
}
void StateSimulator::translate(float x, float y, float z)
{
	//	auto m = glm::translate(glm::mat4(1.0f),glm::vec3(x,y,z));
	
	___currMat = glm::translate(___currMat,glm::vec3(x,y,z));
	m_upload_matrix();
	m_set_dirty_mats();
}

void StateSimulator::translate(const glm::vec3 &v)
{
	___currMat = glm::translate(___currMat,v);
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::rotate(float a, float x, float y, float z)
{
	___currMat = glm::rotate(___currMat,a,glm::vec3(x,y,z));
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::scale(float x, float y, float z)
{
	___currMat = glm::scale(___currMat,glm::vec3(x,y,z));
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::lookAt(
		float ex,
		float ey,
		float ez,
		float tx,
		float ty,
		float tz,
		float ux,
		float uy,
		float uz
		)
{
	___currMat *= glm::lookAt(
				glm::vec3(ex,ey,ez),
				glm::vec3(tx,ty,tz),
				glm::vec3(ux,uy,uz));
	m_upload_matrix();
	m_set_dirty_mats();
}

void StateSimulator::ortho(
		float l,
		float r,
		float b,
		float t,
		float n,
		float f)
{
	___currMat *= glm::ortho(l,r,b,t,n,f);
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::frustum(
		float l,
		float r,
		float b,
		float t,
		float n,
		float f)
{
	___currMat *= glm::frustum(l,r,b,t,n,f);
	m_upload_matrix();
	m_set_dirty_mats();
}
void StateSimulator::perspective(float fovy, float ar, float n, float f)
{
	___currMat *= glm::perspective(fovy,ar,n,f);
	m_upload_matrix();
	m_set_dirty_mats();
}

#define ID_LIGHTS 10
#define ID_LIGHT_POS(x)   (0+x*5+ID_LIGHTS)
#define ID_LIGHT_AMBI(x)  (1+x*5+ID_LIGHTS)
#define ID_LIGHT_DIFF(x)  (2+x*5+ID_LIGHTS)
#define ID_LIGHT_SPEC(x)  (3+x*5+ID_LIGHTS)
#define ID_LIGHT_ATTEN(x) (4+x*5+ID_LIGHTS)

#define NAME_LIGHT_POS(x)   "light["+std::to_string(x)+"].pos"
#define NAME_LIGHT_AMBI(x)  "light["+std::to_string(x)+"].ambi"
#define NAME_LIGHT_DIFF(x)  "light["+std::to_string(x)+"].diff"
#define NAME_LIGHT_SPEC(x)  "light["+std::to_string(x)+"].spec"
#define NAME_LIGHT_ATTEN(x) "light["+std::to_string(x)+"].atten"
void setLightUniforms(const Light& l, int i, ShaderWrap* sw)
{
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_POS(i))  ,1,glm::value_ptr(l.position));
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_AMBI(i)) ,1,glm::value_ptr(l.ambient));
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_DIFF(i)) ,1,glm::value_ptr(l.diffuse));
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_SPEC(i)) ,1,glm::value_ptr(l.specular));
	glUniform4fv(sw->getUniformLocation(ID_LIGHT_ATTEN(i)),1,glm::value_ptr(l.attenuation_factors));
	
}
void StateSimulator::useShader(ShaderWrap* sw, bool first_time )
{
	glUseProgram(sw->handle());
	if(first_time)
	{
		sw->learnUniform(NAME_MVM,ID_MVM);
		sw->learnUniform(NAME_PJM,ID_PJM);
		sw->learnUniform(NAME_CMAP,ID_CMAP);
		sw->learnUniform(NAME_NMM,ID_NMM);
		for(int i = 0 ; i<8;i++)
		{
			sw->learnUniform(NAME_LIGHT_POS(i),ID_LIGHT_POS(i));
			sw->learnUniform(NAME_LIGHT_AMBI(i),ID_LIGHT_AMBI(i));
			sw->learnUniform(NAME_LIGHT_DIFF(i),ID_LIGHT_DIFF(i));
			sw->learnUniform(NAME_LIGHT_SPEC(i),ID_LIGHT_SPEC(i));
			sw->learnUniform(NAME_LIGHT_ATTEN(i),ID_LIGHT_ATTEN(i));
		}
	}
	
	m_current_program = sw;
	if(m_dirty_mats[m_features])
	{
		for(int i  = 0 ; i<MATRIXMODE_COUNT;i++)
		{
			glUniformMatrix4fv(
						m_current_program->getUniformLocation(i),
						1,
						GL_FALSE,
						glm::value_ptr(m_matrix[i].top()));
			if(i == MODELVIEW)
			{
				auto normal_mat = glm::transpose(glm::inverse(m_matrix[MODELVIEW].top()));
				glUniformMatrix4fv(
							m_current_program->getUniformLocation(ID_NMM),
							1,
							GL_FALSE,
							glm::value_ptr(normal_mat));
			}
		}
		m_dirty_mats[m_features] = false;
	}
	if((m_features & Lighting) && m_dirty_lights[m_features])
	{
		for(int i = 0 ; i< 8;i++)
		{
			setLightUniforms(m_light[i],i,m_current_program);
		}
		m_dirty_lights[m_features] = false;
	}
}

void StateSimulator::setLightPosition(int light, const glm::vec4& p)
{
	glm::vec4 p_es = m_matrix[MODELVIEW].top()*p;
	m_light[light].setPosition(p_es);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
}
void StateSimulator::setLightAmbientColor(int light, const glm::vec3 c)
{
	m_light[light].setAmbientColor(c);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
}
void StateSimulator::setLightDiffuseColor(int light, const glm::vec3 c)
{
	m_light[light].setDiffuseColor(c);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
	
}
void StateSimulator::setLightSpecularColor(int light, const glm::vec3 c)
{
	m_light[light].setSpecularColor(c);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
}
void StateSimulator::setLightSpotDirection(int light, const glm::vec3 d)
{
	m_light[light].setSpotDirection(glm::vec4(d,0));
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
	
}
void StateSimulator::setLightSpotCutOff(int light, const float& co)
{
	m_light[light].setSpotCutoff(co);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
	
}
void StateSimulator::setLightSpotExponent(int light, const float& ex)
{
	m_light[light].setSpotExponent(ex);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
	
}
void StateSimulator::setLightAttenuationFactors(int light, const glm::vec4& att)
{
	m_light[light].setAttenuationFactors(att);
	setLightUniforms(m_light[light],light,m_current_program);
	m_set_dirty_lights();
}

ShaderWrap* StateSimulator::loadShaderFormFiles(const std::string &vs_path, const std::string &fs_path)
{
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	std::string vshader_src = loadFile(vs_path);
	
	const char* cstr = vshader_src.c_str();
	int len = vshader_src.length();
	glShaderSource(vshader,1,&cstr, &len);
	if(! compileShader(vshader))
	{
		glDeleteShader(vshader);
		return nullptr;
	}
	
	// generate Fragment shaders
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	// load the source code from a file
	std::string fshader_src = loadFile(fs_path);
	cstr = fshader_src.c_str();
	len = fshader_src.length();
	// attach the code to the shader object
	glShaderSource(fshader,1,&cstr,&len);
	// compile and check for errors
	if(!compileShader(fshader))
	{
		printf("CODE:\n%s\n",cstr);
		glDeleteShader(fshader);
		return nullptr;
	}
	else
	{
		// generate the program.
		GLuint prog = glCreateProgram();
		// add vertex and fragment shader
		glAttachShader(prog,vshader);
		glAttachShader(prog,fshader);
		// bind the attribute locations. This is due to backwards
		// compatibility, with newer GL/GLSL versions you can define the
		// location in the shader.
		glBindAttribLocation(prog, ALOC_POSITION, NAME_POS_ATTR);
		glBindAttribLocation(prog, ALOC_NORMAL, NAME_NRM_ATTR);
		glBindAttribLocation(prog, ALOC_COLOR, NAME_CLR_ATTR);
		glBindAttribLocation(prog, ALOC_TEXCOORD, NAME_TEX_ATTR);
		// link the program
		linkShader(prog);
		// use it for the first time to learn the uniform mappings
		ShaderWrap* result= new ShaderWrap(prog);
		this->useShader(result,true);
		// delete the fragment-shader we don't need it anymore
		glDeleteShader(fshader);
		return result;
	}
}
void StateSimulator::useCurrentShader()
{
	useShader(m_current_program);
}
}
