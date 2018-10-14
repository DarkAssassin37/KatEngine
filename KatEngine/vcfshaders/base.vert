#version 410 core

layout (location = 0) in vec3 posCurr;
layout (location = 1) in vec3 normCurr;
layout (location = 2) in vec2 uvs;


uniform mat4 mvp;
uniform float time;

out vec3 normal;
out vec2 texCoord;


void main()
{
	gl_Position = mvp * vec4(posCurr, 1.0);

  normal = normCurr;
	texCoord = uvs;

}
