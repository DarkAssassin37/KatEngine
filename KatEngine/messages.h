#pragma once

#include <iostream>
#include <GLFW\glfw3.h>

inline void fatal_error()
{
	system("pause");
	glfwTerminate();
	exit(-1);
}

inline void fatal_error(const char* message)
{
	std::cout << message << std::endl;
	system("pause");
	glfwTerminate();
	exit(-1);
}