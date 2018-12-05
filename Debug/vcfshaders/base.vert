#version 430 core

layout (location = 0) in vec3 posCurr;
layout (location = 1) in vec3 normCurr;
layout (location = 2) in vec2 uvs;


uniform mat4 mvp;
uniform float time;

out vec3 v_normal;
out vec2 v_texCoord;


void main()
{
	gl_Position = mvp * vec4(posCurr, 1.0);

	v_normal = normCurr;
	v_texCoord = uvs;

}
