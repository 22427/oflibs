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
	return step(0.5,baseColor.a);
}
// The light equation

vec4 calcColor(vec4 baseColor)
{
	vec3 E = normalize(-pos_es.xyz); 
	vec3 N = normalize(nrm_es.xyz); 
	vec3 light_clr= vec3(0,0,0); 

	for(int i =0 ; i< 1;i++) 
	{ 		
		vec3 Iamb =  light[i].ambi.rgb; 
		vec3 L = light[i].pos.xyz - pos_es.xyz; 
		float d = length(L); 
		L = L/d; 
		float NdotL = dot(N,L); 
		vec3 Idiff = light[i].diff.rbg * max(NdotL, 0.0); 
		vec3 H = normalize(L+E); 
		vec3 Ispec =  light[i].spec.rgb * pow(max(dot(N,H),0.0),20)*step(0.0,NdotL);  
		float d_2 = d*d; 
		float atten = light[i].atten.w/
			(light[i].atten.x+light[i].atten.y*d+light[i].atten.z*d_2);  
		light_clr+= (Iamb+Idiff+Ispec) * atten; 
	} 
	return vec4(light_clr*baseColor.rgb,baseColor.a);
}

// Just get the base color from the vertex color 
vec4 calcBaseColor()
{
	return clr;
}
// The base function.
void main()
{
	vec4 baseColor = calcBaseColor();
	vec4 color = calcColor(baseColor);
	color.a = calcAlpha(baseColor);
	FRAG_COLOR = color;
}
