// The uniforms for all stock fragment shaders.

#if __VERSION__ >= 130
	#define IN in
	#define OUT out
	#define TEXTURE2D texture
	out vec4 FRAG_COLOR;
#else
	#define IN varying
	#define OUT varying
	#define FRAG_COLOR gl_FragColor
	#define TEXTURE2D texture2D
#endif

IN vec4 clr;
IN vec2 tex;
IN vec4 pos_es;
IN vec3 nrm_es;
uniform sampler2D cmap;

struct Light
{
	vec4 pos;
	vec4 ambi; // ambi.w == spot_exp
	vec4 diff; // diff.w == spec.w == spot_cuttoff
	vec4 spec; 
	vec4 atten;
};

struct Material
{
	vec4 emiss;
	float shininess;
};
uniform Light light[8];

float calcAlpha(vec4 baseColor)
{
	return baseColor.a;
}
// Just pass the base color on to the clr
vec4 calcColor(vec4 baseColor)
{
	return baseColor;
}
// Get the base color from the colormap
vec4 calcBaseColor()
{
	return  TEXTURE2D(cmap,tex);
}

// The base function.
void main()
{
	vec4 baseColor = calcBaseColor();
	vec4 color = calcColor(baseColor);
	color.a = calcAlpha(baseColor);
	FRAG_COLOR = color;
}
