#version 430 core

layout (location = 0) in vec3 posCurr;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(posCurr, 1.0);
}
