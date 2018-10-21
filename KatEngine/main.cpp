#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "messages.h"
#include <ctime>
#include <string>
#include "utils/io.h"
#include "ProgramShader.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Plane.h"
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

using namespace std;
using namespace glm;

float winWidth = 640.0f, winHeight = 640.0f;
float camDist = 4.0f;
float cam_pitch = 0.0f, cam_yaw = 0.0f;
vec3 cam_pos(0,0,1);

mat4 getCamera()
{

	mat4 cam_rot = rotate(glm::identity<mat4>(), 2* pi<float>() * cam_yaw, vec3(0, 1, 0));
	cam_rot = rotate(cam_rot, pi<float>() * cam_pitch, vec3(1, 0, 0));
	cam_pos = vec3(cam_rot * vec4(0, 0, camDist, 1)) ;
	vec3 camUp = vec3(cam_rot * vec4(0, 1, 0, 1));

	mat4 projection = perspective(90.0f, winWidth/winHeight, 0.1f, 100.0f);
	mat4 view = lookAt(cam_pos, vec3(0, 0, 0), camUp);
	return projection * view;
}

#pragma region Callbacks

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
double a_xpos = 0.0f, a_ypos = 0.0f;
float mouseSensitivity = 0.001;

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (!captured) {
		lxpos = xpos;
		lypos = ypos;
		return;
	}


	double dy = lypos - ypos;//inverted Y
	double dx = lxpos - xpos;//inverted X


	a_ypos += dy * mouseSensitivity;
	a_xpos += dx * mouseSensitivity;

	if (a_ypos > 0.5f)
		a_ypos = 0.5f;
	if (a_ypos < -0.5f)
		a_ypos = -0.5f;

	cam_pitch = a_ypos;
	cam_yaw = a_xpos;
	/*if (cam_pitch > 0.5)
		cam_pitch = 0.5;

	if (cam_pitch < -0.5)
		cam_pitch = -0.5;*/

	lxpos = xpos;
	lypos = ypos;
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
	camDist += -yoffset * 0.1 * camDist;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	winWidth = width;
	winHeight = height;
	glViewport(0, 0, width, height);
}
#pragma endregion 

void glUniform3fg(int location, vec3 values)
{
	glUniform3f(location, values.x, values.y, values.z);
}


void APIENTRY openGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
	return;
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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	/*Now let's make a windows and get its context*/
	GLFWwindow* window = glfwCreateWindow((int)winWidth, (int)winHeight, "KatEngine", NULL, NULL);

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

	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openGlDebugCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	glViewport(0, 0, (int)winWidth, (int)winHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);

	/*Prepare ze battlefield*/

	/*Vertex Shader*/
	Shader vertShader("vcfshaders\\base.vert", Shader::VERTEX_SHADER);

	/*Fragment Shader*/
	Shader fragShader("vcfshaders\\base.frag", Shader::FRAGMENT_SHADER);

	/*Compute Shader*/
	Shader compShader("vcfshaders\\rmd.comp", Shader::COMPUTE_SHADER);

	/*Final Program*/
	ProgramShader progShader(vertShader, fragShader);

	ProgramShader csprogShader(compShader);

	glCheckError();




	/*Model bolly("Models\\sphere.obj");
	bolly.loadMeshes(0, 0);*/

	Plane plane;


	float interp = 1.0f; //folosit la interpolat doua modele de sfere pentru o animatie

	double lastTime = glfwGetTime();
	int nbFrames = 0;



	//Texture tex(R"(D:\Projects\C++\KatEngine\Pukman\winter.jpg)");
	//Texture tex2(R"(D:\Photos\20171712_105524.jpg)");
	Texture texbl(1024, 1024);//blank texture 

	mat4 passThroughcam = glm::identity<mat4>();
	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		/*Stopwatch - ms > fps :D 60 fps = < 16ms*/
		float currentTime = glfwGetTime();
		nbFrames++;

		if (currentTime - lastTime >= 1.0) { 
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		/*Update animatio interpolation uniform*/
		interp += 0.002f;
		//if (interp > 2.0f) interp = 0.0f;
		//texbl.clear();
		/*use the blank texture as compute shader frame buffer*/
		texbl.bindImage(0);

		/*Calculate the forward rays for the corners of the camera projection screen*/
		mat4 cam = getCamera();
		mat4 invcam = inverse(cam);

		vec4 calc = invcam * vec4(-1, -1, 0, 1); calc /= calc.w;
		vec3 ray00 = vec3(calc);
		calc = invcam * vec4(-1, 1, 0, 1); calc /= calc.w;
		vec3 ray01 = vec3(calc);
		calc = invcam * vec4(1, -1, 0, 1); calc /= calc.w;
		vec3 ray10 = vec3(calc);
		calc = invcam * vec4(1, 1, 0, 1); calc /= calc.w;
		vec3 ray11 = vec3(calc);

		glUseProgram(csprogShader);
		//glUniform1f(glGetUniformLocation(csprogShader, "roll"), interp*10);
		auto loc = glGetUniformLocation(csprogShader, "eye");
		glUniform3fg(0, cam_pos);
		glUniform3fg(1, ray01 - cam_pos);
		glUniform3fg(2, ray00 - cam_pos);
		glUniform3fg(3, ray11 - cam_pos);
		glUniform3fg(4, ray10 - cam_pos);
		glDispatchCompute(1024 / 8 , 1024 / 8, 1); //1024 512^2 threads in blocks of 16^2*/

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texbl.bindTexture(0);



		progShader.use();

		glUniformMatrix4fv(glGetUniformLocation(progShader.id, "mvp"), 1, GL_FALSE, &passThroughcam[0][0]);
		glUniform1f(glGetUniformLocation(progShader, "lerp_value"), interp > 1.0f ? 2 - interp : interp);
		//glUniform1f(glGetUniformLocation(progShader, "time"), currentTime);

		plane.draw();


		glfwSwapBuffers(window);
		glfwPollEvents();
		glCheckError();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
	
}