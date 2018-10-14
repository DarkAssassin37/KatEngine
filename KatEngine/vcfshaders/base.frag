#version 410 core

in vec3 normal;
in vec2 texCoord;
out vec4 color;

uniform sampler2D t_albedo;
uniform sampler2D t_normal;
uniform sampler2D t_roughness;
uniform sampler2D t_metallic;

void main()
{
	vec3 cR = vec3(38, 0, 0) * dot(normal, vec3(0,0,1)) ;
	vec3 cG = vec3(0, 132, 0) * dot(normal, vec3(1,0,0));
	vec3 cB = vec3(0, 0, 247) * dot(normal, vec3(0,1,0));
	vec3 cRGB = cR + cG + cB + vec3(80, 80, 80);
	cRGB /= 255f; cRGB = clamp(cRGB, 0f, 1f);
	color = texture(t_albedo, texCoord);// + vec4(cRGB, 0.0f);
	//color = vec4(normal, 1.0f);

}
