#pragma once

#include <iostream>
#include <GLFW\glfw3.h>


inline void fatal_error(const char* message)
{
	std::cout << message << std::endl;
	system("pause");
	glfwTerminate();
	exit(-1);
}

inline int check_shader_compilation(GLuint shader, const char* extraInfo = "")
{
	int  success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << extraInfo << "Shader failed to compile:\n" << infoLog << std::endl;
	}
	return success;
}