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
#include "Texture.h"


using namespace std;
using namespace glm;

float winWidth = 640.0f, winHeight = 480.0f;
float camDist = 10.0f;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	winWidth = width;
	winHeight = height;
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
	
	mat4 cam_rot = rotate(glm::identity<mat4>(), 2* pi<float>() * cam_yaw, vec3(0, 1, 0));
	cam_rot = rotate(cam_rot, pi<float>() * cam_pitch, vec3(1, 0, 0));
	vec3 camPos = vec3(cam_rot * vec4(0, 0, camDist, 1)) ;
	vec3 camUp = vec3(cam_rot * vec4(0, 1, 0, 1));

	mat4 projection = perspective(90.0f, winWidth/winHeight, 0.1f, 100.0f);
	mat4 view = lookAt(camPos, vec3(0, 0, 0), camUp);
	return projection * view;
}

bool captured = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float amount = 0.01;
	if (action == GLFW_PRESS)
	{
		cam_pitch += key == GLFW_KEY_UP ? amount : 0 + key == GLFW_KEY_DOWN ? -amount : 0;
		cam_yaw += key == GLFW_KEY_LEFT ? amount : 0 + key == GLFW_KEY_RIGHT ? -amount : 0;
		if (key == GLFW_KEY_C)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			captured = true;
		}
		else if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			captured = false;
		}
	}

}

double lxpos, lypos;

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!captured) return;

	lxpos = xpos;
	lypos = ypos;

	double a_ypos = 0.0f;
	double a_xpos = 0.0f;

	double dy = ypos - lypos;


	if (abs(ypos) > winHeight)
	{
		if (dy > 0)
			a_ypos = winHeight;
		else
			a_ypos = winHeight - dy;
	}

	float amount = 1;
	cam_pitch = -(a_ypos / winHeight - 0.5) * amount;
	cam_yaw = -(a_xpos / winWidth - 0.5) * amount;
	/*if (cam_pitch > 0.5)
		cam_pitch = 0.5;

	if (cam_pitch < -0.5)
		cam_pitch = -0.5;*/
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			captured = !captured;
			glfwSetInputMode(window, GLFW_CURSOR, captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camDist += yoffset * 0.1 * camDist;
}



int main()
{
	using namespace std;


	/*Init GLFW*/
	if (!glfwInit())
		fatal_error("GLFW init error");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/*Now let's make a windows and get its context*/
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

	if (window == NULL)
		fatal_error("Failed to create GLFW window");

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

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

	Model bolly("Models\\plane.obj");
	bolly.loadMeshes(0, 0);

	float interp = 1.0f;

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	glEnable(GL_DEPTH_TEST);


	Texture tex(R"(D:\Projects\C++\KatEngine\Pukman\winter.jpg)");
	Texture tex2(R"(D:\Photos\20171712_105524.jpg)");

	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		nbFrames++;

		if (currentTime - lastTime >= 1.0) { 
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		tex.bindTexture(0);
		tex.bindTexture(1);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		interp += 0.002f;
		if (interp > 2.0f) interp = 0.0f;

		mat4 cam = getCamera();
		
		glUniformMatrix4fv(glGetUniformLocation(progShader.id, "mvp"), 1, GL_FALSE, &cam[0][0]);
		glUniform1f(glGetUniformLocation(progShader.id, "lerp_value"), interp > 1.0f ? 2 - interp : interp);

		bolly.draw();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
	
}