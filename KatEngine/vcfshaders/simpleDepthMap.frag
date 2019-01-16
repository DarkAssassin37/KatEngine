#version 430 core

out vec4 fColor;

void main()
{
	fColor = vec4(gl_FragCoord.z);
	fColor.w = 1.0;
}
