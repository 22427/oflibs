#if __VERSION__ >= 130
	#define IN in
	#define OUT out
	out vec4 FRAG_COLOR
#else
	#define IN attribute
	#define OUT varying
	#define FRAG_COLOR gl_FragColor
#endif

// The stock vertex shader

// uniforms for modelview and projection matrix
uniform mat4 mvm;
uniform mat4 pjm;
uniform mat4 nmm;
// the attributes
IN vec3 position;
IN vec3 normal;
IN vec2 texcoord;
IN vec4 color;
// the varyings passed to the fragment shader
// position and normal in eye-space for the lighting calculations
OUT vec4 pos_es;
OUT vec3 nrm_es;
// the color and texture coordinates of this vertex
OUT vec4 clr;
OUT vec2 tex;

void main()
{
	pos_es = mvm*vec4(position,1);
	gl_Position = pjm*pos_es;
	nrm_es = (nmm*vec4(normal,0)).xyz;
	
	clr = color;
	tex = texcoord;
}

