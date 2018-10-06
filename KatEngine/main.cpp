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

	/*Prepare the traingle*/
	vector<vec3> vertices = {
		vec3(-0.5f, -0.5f, 0.0f),
		vec3(0.5f, -0.5f, 0.0f),
		vec3(0.0f,  0.5f, 0.0f)
	};

	vector<vec3> colors = {
		vec3(0.0f, 0.8f, 0.9f),
		vec3(0.9f, 0.6f, 0.1f),
		vec3(0.8f,  0.0f, 0.8f)
	};

	vector<vec3> transVertices;


	GLuint vbo = 0;
	glGenBuffers(1, &vbo);

	GLuint vbo_cols = 0;
	glGenBuffers(1, &vbo_cols);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	

	GLuint vao;
	glGenVertexArrays(1, &vao);


	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);



		mat4 cam = getCamera();

		transVertices.clear();
		transVertices.reserve(vertices.size());
		for (vec3 v : vertices)
		{
			transVertices.emplace_back(cam * vec4(v, 1));
		}





		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_cols);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindVertexArray(0);


		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, transVertices.size() * sizeof(vec3), &transVertices[0], GL_DYNAMIC_DRAW);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_cols);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(vec3), &colors[0], GL_DYNAMIC_DRAW);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
	
}