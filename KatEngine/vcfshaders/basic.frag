#version 430 core

in vec3 normal;
out vec4 color;

void main()
{
	vec3 partcol = vec3(0.8, 0.1, 0.4);
	color = vec4(partcol * normal, 1.0f);

}
