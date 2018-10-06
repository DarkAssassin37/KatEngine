#version 410

in vec3 normal;
out vec4 color;

void main()
{
	vec3 cRGB = vec3(38, 132, 247);

	//color = vec4(cRGB.xyz / 255f, 1.0f);
	color = vec4(normal, 1.0f);

}