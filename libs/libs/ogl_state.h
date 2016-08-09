#pragma once
#ifndef OFL_GL_HEADER
#define OFL_GL_HEADER <glad/glad.h>
#endif

#include OFL_GL_HEADER

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vmath.h"

#include <fstream>
#include <streambuf>
#include <map>
#include <vector>
#include <stack>


namespace ofl
{


#define NAME_MVM "mvm" // model view matrix
#define NAME_PJM "pjm" // projection matrix
#define NAME_NMM "nmm" // normal matrix
#define NAME_CMAP "cmap"

#define ID_MVM 0
#define ID_PJM 1
#define ID_NMM 2
#define ID_CMAP 3

#define NAME_POS_ATTR "position"
#define NAME_NRM_ATTR "normal"
#define NAME_CLR_ATTR "color"
#define NAME_TEX_ATTR "texcoord"


#define ALOC_POSITION 0
#define ALOC_NORMAL 1
#define ALOC_COLOR 2
#define ALOC_TEXCOORD 3
#define ALOC_TANGENT 4


enum MatrixMode
{
	MODELVIEW = 0,
	PROJECTION,
	MATRIXMODE_COUNT
};
enum StockShader
{
	PASS_THROUGH = 0,
	STOCK_SHADER_COUNT
};


enum Feature
{
	Texturing = 1,
	Lighting = 2,
	DFAlpha = 4,
	NonStockShader =8,
	FEATURE_STATE_COUNT = 9
};
inline std::string to_string(const Feature f)
{
	switch (f) {
	case Texturing: return "texturing";
	case Lighting: return "lighting";
	case DFAlpha: return "DF-alpha";
	case NonStockShader: return "non stock shader";

	default:
		break;
	}
	return "";
}






class Light
{
public:
	/** Ambient color of this light source.*/
	glm::vec4 ambient;
	/** Diffuse color of this light source.*/
	glm::vec4 diffuse;
	/** Specular color of this light source.*/
	glm::vec4 specular;

	/** Eye space position of this light source.*/
	glm::vec4 position;
	/** Direction of the beam starting at position.*/
	glm::vec4 spot_direction;
	/** Focus of the spot.*/
	//	float spot_exponent; <- is ambient.w
	/** Angle between spot direction and boundary*/
	//	float spot_cutoff; <- is diffuse.w


	/** The attenuation of this light source.
		.x - Constant attenuation
		.y - Linear attenuation
		.z - Quadratic attenuation
		.w - A constant attenuation factor
	*/
	glm::vec4 attenuation_factors;
	Light()
	{
		ambient = glm::vec4(0,0,0,0);
		diffuse = specular = glm::vec4(1,1,1,180);
		position = glm::vec4(0,0,0,1);
		spot_direction = glm::vec4(0,0,-1,0);
		attenuation_factors = glm::vec4(1,0,0,0);
	}
	~Light(){}
	/** Sets ambient color */
	void setAmbientColor(const glm::vec3& a)
	{
		ambient.r=a.r;
		ambient.g=a.g;
		ambient.b=a.b;
	}
	/** Sets specular color */
	void setSpecularColor(const glm::vec3& s)
	{
		specular.r = s.r;
		specular.g = s.g;
		specular.b = s.b;
	}
	/** Sets diffuse color */
	void setDiffuseColor(const glm::vec3& d)
	{
		diffuse.r = d.r;
		diffuse.g = d.g;
		diffuse.b = d.b;
	}
	/** Sets position of the light.
		 Note: if p.w == 0 the light source is a directional one.
		 The position then determines the direction the light comes from.
	*/
	void setPosition(const glm::vec4& p){position = p;}
	/** Sets spot direction*/
	void setSpotDirection(const glm::vec4& sd){spot_direction = sd;}
	/** Sets spot cutoff*/
	void setSpotCutoff(const float& spot_co){diffuse.w = spot_co;}
	/** Sets spot exponent*/
	void setSpotExponent(const float& spot_ex){ambient.w = spot_ex;}
	/** Sets attenuation values for constant, linear and quadratic, and w
		af.w/(af.x+af.y*d+af.z*d*d). d is the distance to the light source	*/
	void setAttenuationFactors(const glm::vec4& af){attenuation_factors = af;}

};



class ShaderWrap
{
	GLuint m_handle;
	std::map<int, int> m_uniformID2location;
public:
	ShaderWrap(GLuint handle);
	~ShaderWrap();
	void learnUniform(const std::string& name, int id);
	int getUniformLocation(int id);
	const GLuint& handle()const {return m_handle;}
};




//#define ___currMat m_matrix[m_matrixMode].top()
class StateSimulator
{
protected:
	std::stack<glm::mat4> m_matrix[MATRIXMODE_COUNT];
	MatrixMode m_matrixMode;

	ShaderWrap* m_stockshaders[NonStockShader];
	ShaderWrap* m_current_program;

	void setUniform(int id, const glm::mat4& mat);
	unsigned int m_features;

	bool m_dirty_lights[FEATURE_STATE_COUNT];
	bool m_dirty_mats[FEATURE_STATE_COUNT];

	void m_set_dirty_lights()
	{
		for(int i = 0 ; i<FEATURE_STATE_COUNT;i++)
			m_dirty_lights[i] = true;
		m_dirty_lights[m_features] = false;
	}
	void m_set_dirty_mats()
	{
		for(int i =0 ; i<FEATURE_STATE_COUNT; i++)
		{
			m_dirty_mats[i] = true;
		}
		m_dirty_mats[m_features] = false;
	}

	/** Will upload the current matrix.

	*/
	void m_upload_matrix();


	Light m_light[8];

//	std::string loadFile(std::string path);
//	bool compileShader(GLuint shader);
	public:
	StateSimulator(const std::string &path_to_stock_shaders = "resources/");
	~StateSimulator();
	void enable (Feature f);
	void disable(Feature f);

	void pushMatrix();
	void popMatrix();
	void loadIdentity();
	void loadMatrix(const glm::mat4& m);
	void matrixMode(MatrixMode m);
	void translate(float x, float y, float z);
	void translate(const glm::vec3& v);
	void rotate(float a, float x, float y, float z);
	void scale(float x, float y, float z);
	void lookAt(
		float ex,
		float ey,
		float ez,
		float tx,
		float ty,
		float tz,
		float ux,
		float uy,
		float uz);
	void ortho(float l, float r, float b, float t, float n, float f);
	void frustum(float l, float r, float b, float t, float n, float f);
	void perspective(float fovy, float ar, float n, float f);
	void useShader(ShaderWrap* sw, bool first_time =false );

	void setLightPosition(int light, const glm::vec4& p);
	void setLightAmbientColor(int light, const glm::vec3 c);
	void setLightDiffuseColor(int light, const glm::vec3 c);
	void setLightSpecularColor(int light, const glm::vec3 c);
	void setLightSpotDirection(int light, const glm::vec3 d);
	void setLightSpotCutOff(int light, const float& co);
	void setLightSpotExponent(int light, const float& ex);
	void setLightAttenuationFactors(int lignt, const glm::vec4& att);
	ShaderWrap* loadShaderFormFiles(const std::string& vs_path, const std::string& fs_path);
	void useCurrentShader();

};
}
