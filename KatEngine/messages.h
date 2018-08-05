#pragma once

#include <iostream>
#include <GLFW\glfw3.h>


void FatalError(const char* message)
{
	std::cout << message << std::endl;
	system("pause");
	glfwTerminate();
	exit(-1);
}