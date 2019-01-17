#version 430 core

in vec3 v_normal;
in vec2 v_texCoord;
out vec4 color;

uniform sampler2D t_albedo;
uniform sampler2D t_normal;
uniform sampler2D t_roughness;
uniform sampler2D t_metallic;

uniform float time;

void main()
{
	color = vec4(1.0);
	/*vec3 cR = vec3(38, 0, 0) * dot(v_normal, vec3(0,0,1)) ;
	vec3 cG = vec3(0, 132, 0) * dot(v_normal, vec3(1,0,0));
	vec3 cB = vec3(0, 0, 247) * dot(v_normal, vec3(0,1,0));
	vec3 cRGB = cR + cG + cB + vec3(80, 80, 80);*/

	float midLength = length(vec2(0.5,0.5) - v_texCoord);

	float cR = texture(t_albedo, v_texCoord * (1.0 - midLength * 0.01)).r;
	float cG = texture(t_albedo, v_texCoord * (1.0 - midLength * 0.02)).g;
	float cB = texture(t_albedo, v_texCoord * (1.0 - midLength * 0.025)).b;

	vec3 cRGB = vec3(cR, cG, cB);

	vec2 dir = normalize(vec2(0.1, 0.5));
	float speed1 = 0.1;
	vec3 snow = vec3(0.0f);

	snow += texture(t_roughness, v_texCoord + dir * speed1 * time).xyz;
	snow += texture(t_roughness, v_texCoord * 0.85 + dir * speed1 * time).xyz;
	snow += texture(t_roughness, v_texCoord * 0.7 + dir * speed1 * time).xyz;
	snow += texture(t_roughness, v_texCoord * 0.5 + dir * speed1 * time).xyz;
	snow += texture(t_roughness, v_texCoord * 0.3 + dir * speed1 * time).xyz;

	vec3 snowcolor = vec3(0.8);

	//cRGB /= 255.0f; cRGB = clamp(cRGB, 0.0f, 1.0f);
	//color = texture(t_albedo, v_texCoord);// + vec4(cRGB, 0.0f);
	color.xyz = cRGB * (1.0 - snow.r) + snowcolor * snow.r;

}
