#version 430 core

layout (location = 0) in vec4 a_v4Position;
layout (location = 1) in vec4 a_v4FillColor;

out vec4 v_v4FillColor;

void main()

{

      v_v4FillColor = a_v4FillColor;
      gl_Position = a_v4Position;

}