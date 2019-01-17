#version 410 core

in vec3 textureCoordinates;
out vec4 color;

uniform samplerCube skybox;

void main()
{
	vec3 fogColor = vec3(0.9f,0.9f,1.0f)*0.7;
	float t = dot(textureCoordinates, vec3(0.0,1.0,0.0));
	t = 1-t;
	t = clamp(t, 0.0, 1.0);
	vec3 tempCol = fogColor * t + texture(skybox, textureCoordinates).xyz * vec3(1.0,1.0,1.0) * (1-t);
	color = vec4(tempCol, 1.0);
}
