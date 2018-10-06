#version 410

layout (location = 0) in vec3 posCurr;
layout (location = 1) in vec3 posNext;
layout (location = 2) in vec3 normCurr;
layout (location = 3) in vec3 normNext;
layout (location = 4) in vec3 uvs;


uniform mat4 mvp;
uniform float lerp_value;

out vec3 normal;


void lerp(in vec3 v1, in vec3 v2, in float value, out vec3 res)
{
	res = v1 * value + v2 * (1-value);
}

void main()
{
	vec3 pos; 
	lerp(posCurr.xyz, posNext.xyz, lerp_value, pos);
	gl_Position = vec4(pos, 1.0);
	
	lerp(normCurr, normNext, lerp_value, normal);

}