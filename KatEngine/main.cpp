#include <iostream>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "messages.h"
#include <time.h>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void countingTo10()
{
	using namespace std;
	string names[] = { "ichi", "ni", "san", "yon", "go", "roku", "nana", "hatchi", "kyuu", "juu" };
	srand(time(NULL));

	int len = sizeof(names) / sizeof(string);
	string buffy;

	while (true)
	{
		int ci = rand() % len;
		cout << ci + 1 << endl;
		cin >> buffy;
		cout << names[ci] << endl;
		
		system("pause>nul");
		system("cls");

	}
	//ichi ni hatchi yon go roku
	/*
	ichi ni san yon go roku
	the quick brown fox jumped over the lazy dog
	the quick brown fox jumped over the lazy dog
	the quick brown fox jumped over the lazy dog
	the quick brown fox jumped over the lazy dog
	the quick brown fox jumped over the lazy dog
	  over the lazy dog 
	
	*/
}


int main()
{
	using namespace std;

	if (!glfwInit())
		FatalError("GLFW init error");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

	if (window == NULL)
		FatalError("Failed to create GLFW window");

	std::cout << "Hello world!\n\n";

	glViewport(0, 0, 640, 480);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
	
}