#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "messages.h"
#include <ctime>
#include <string>
#include "utils/io.h"
#include "Shaders/FragmentShader.h"
#include "ProgramShader.h"
#include "Shaders/VertexShader.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "Model.h"


using namespace std;
using namespace glm;

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

void testingStuff()
{
	using namespace glm;
	vec3 a(1, 1, 0);
	vec3 b(1, 2, 0);
	vec3 c=  cross(a, b);
}

float cam_pitch = 0.0f, cam_yaw = 0.0f;

mat4 getCamera()
{
	
	mat4 cam_rot = rotate(glm::identity<mat4>(), 2 * pi<float>() * cam_pitch, vec3(1, 0, 0));
	cam_rot = rotate(cam_rot, 2 * pi<float>() * cam_yaw, vec3(0, 1, 0));
	vec3 camPos = vec3(cam_rot * vec4(0, 0, 1, 1)) * 1.1f;
	vec3 camUp = vec3(cam_rot * vec4(0, 1, 0, 1));

	mat4 projection = perspective(90.0f, 1.0f, 0.1f, 100.0f);
	mat4 view = lookAt(camPos, vec3(0, 0, 0), camUp);
	return projection * view;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float amount = 0.01;
	if (action == GLFW_PRESS)
	{
		cam_pitch += key == GLFW_KEY_UP ? amount : 0 + key == GLFW_KEY_DOWN ? -amount : 0;
		cam_yaw += key == GLFW_KEY_LEFT ? amount : 0 + key == GLFW_KEY_RIGHT ? -amount : 0;
	}

}




int main()
{
	using namespace std;


	/*Init GLFW*/
	if (!glfwInit())
		fatal_error("GLFW init error");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*Now let's make a windows and get its context*/
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

	if (window == NULL)
		fatal_error("Failed to create GLFW window");

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	/*tie the context with glad to opengl*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		fatal_error("Failed to initialize GLAD");


	/*Prepare the battlefield*/
	/*Vertex Shader*/
	VertexShader vertShader("base.vert");

	/*Fragment Shader*/
	FragmentShader fragShader("base.frag");

	/*Final Program*/
	ProgramShader progShader((GLuint)vertShader, (GLuint)fragShader);

	/*We finally get to use it*/
	progShader.use();


	glViewport(0, 0, 640, 480);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //auto-resize

	Model bolly("pukman.obj");
	bolly.loadMeshes(0, 1);

	float interp = 0.0f;
	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		interp += 0.02f;
		if (interp > 1.0f) interp = 0.0f;

		mat4 cam = getCamera();
		glUniformMatrix4fv(0, 16, GL_FALSE, 0);

		bolly.draw();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
	
}