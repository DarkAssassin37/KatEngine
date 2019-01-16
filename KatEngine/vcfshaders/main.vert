#version 430 core

layout (location = 0) in vec4 a_v4Position;
layout (location = 1) in vec4 a_v4Normal;

out vec4 v_v4Normal;
out vec4 v_v4Position;
out vec4 fragPosLightSpace;

uniform mat4 lightSpaceTrMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()

{

      v_v4Normal = a_v4Normal;
	  v_v4Position = a_v4Position;
	  fragPosLightSpace = lightSpaceTrMatrix * model * v_v4Position;

	  //vec3 terroffset = vec3(0.0,0.0,1.0) * a_v4FillColor.w * 1.0;

      gl_Position = projection * view * model * vec4(a_v4Position.xyz, 1.0);

}