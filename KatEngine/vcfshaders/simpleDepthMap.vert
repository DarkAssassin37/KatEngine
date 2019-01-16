#version 430 core

layout (location = 0) in vec4 a_v4Position;
layout (location = 1) in vec4 a_v4FillColor;

uniform mat4 lightSpaceTrMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceTrMatrix * model * vec4(vec3(a_v4Position).xyz, 1.0f);
    //gl_Position = model * vec4(vec3(a_v4Position).xyz, 1.0f);
}
