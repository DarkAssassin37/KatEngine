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
#include "PointCloud.h"
#include "Terra.h"
#include <stb_image.h>
#include "SkyBox.hpp"
#include "ShaderG.hpp"
#define glCheckError() glCheckError_(__FILE__, __LINE__) 
#define KAT_TEXTURE_SIZE 1024

using namespace std;
using namespace glm;


struct PointLight {
	glm::vec3 position;

	float constant = 1.0f;
	float linear;
	float quadratic;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

float winWidth = 640.0f, winHeight = 640.0f;
float camDist = 2.0f;
float cam_pitch = 0.0f, cam_yaw = 0.0f;
float cam_speed = 1.0;
vec3 cam_pos(0,1,1);
mat4 projection, view;

GLuint shadowMapFBO, postEffectsFBO;
GLuint depthMapTexture, postEffectsTexture;
const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
GLfloat lightAngle;
vec3 lightDir;
vec3 cam_forward, cam_backward, cam_up, cam_top, cam_down, cam_left, cam_right;
PointLight pointLights[2];

mat4 getCamera()
{
	mat4 cam_rot = rotate(glm::identity<mat4>(), 2* pi<float>() * cam_yaw, vec3(0, 1, 0));
	cam_rot = rotate(cam_rot, pi<float>() * cam_pitch, vec3(1, 0, 0));
	//cam_pos = vec3(cam_rot * vec4(0, 0, camDist, 1)) ;
	
	cam_forward = vec3(cam_rot * vec4(0, 0, -1, 1));
	cam_backward = vec3(cam_rot * vec4(0, 0, 1, 1));
	cam_left = vec3(cam_rot * vec4(-1, 0, 0, 1));
	cam_right = vec3(cam_rot * vec4(1, 0, 0, 1));
	cam_top = vec3(cam_rot * vec4(0, 1, 0, 1));
	cam_down = vec3(cam_rot * vec4(0, -1, 0, 1));


	projection = perspective(45.0f, winWidth/winHeight, 0.001f, 20.0f);
	view = lookAt(cam_pos, cam_forward * 100.0f, cam_top);
	return projection * view;
}

#pragma region Callbacks

int keyPressed[512];
bool captured = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float amount = 0.01;
	if (action == GLFW_PRESS)
	{
		keyPressed[key] = 1;

		cam_pitch += key == GLFW_KEY_UP ? amount : 0 + key == GLFW_KEY_DOWN ? -amount : 0;
		cam_yaw += key == GLFW_KEY_LEFT ? amount : 0 + key == GLFW_KEY_RIGHT ? -amount : 0;
		
		if (key == GLFW_KEY_C)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			captured = true;
		}

		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			captured = false;
		}

		if (key == GLFW_KEY_V)
		{
			printf("Cam position: %ff, %ff, %ff", cam_pos.x, cam_pos.y, cam_pos.z);
			printf("Cam pitch: %ff", cam_pitch);
			printf("Cam yaw: %ff", cam_yaw);
			captured = true;
		}


		if (key == GLFW_KEY_1) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		if (key == GLFW_KEY_2) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (key == GLFW_KEY_3) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (action == GLFW_RELEASE)
		keyPressed[key] = 0;

}

//dt is in seconds
void keyHeldCallback(double dt)
{
	float dtf = (float)dt;
	if(keyPressed[GLFW_KEY_J])
		lightAngle += dt * 70.0;

	if (keyPressed[GLFW_KEY_L])
		lightAngle -= dt * 70.0;

	if (keyPressed[GLFW_KEY_LEFT_CONTROL])
		cam_speed = 0.1f;
	else cam_speed = 1.0f;

	if (keyPressed[GLFW_KEY_W])
		cam_pos += cam_forward * cam_speed * dtf;

	if (keyPressed[GLFW_KEY_A])
		cam_pos += cam_left * cam_speed * dtf;

	if (keyPressed[GLFW_KEY_S])
		cam_pos += cam_backward * cam_speed * dtf;

	if (keyPressed[GLFW_KEY_D])
		cam_pos += cam_right * cam_speed * dtf;

	if (keyPressed[GLFW_KEY_Z])
		cam_pos += cam_down * cam_speed * dtf;

	if (keyPressed[GLFW_KEY_X])
		cam_pos += cam_top * cam_speed * dtf;
	
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

void initFBOs()
{
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);
	//glGenFramebuffers(1, &postEffectsFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE

																		//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//create depth texture for FBO
	/*glGenTextures(1, &postEffectsTexture);
	glBindTexture(GL_TEXTURE_2D, postEffectsTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE

																		//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, postEffectsFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
}

mat4 computeLightSpaceTrMatrix()
{
	const GLfloat near_plane = 0.001f, far_plane = 10.0;
	float tube = 2.0f;
	mat4 lightProjection = ortho(-tube, tube, -tube, tube, near_plane, far_plane);

	vec3 lightDirTr = vec3(glm::rotate(mat4(1.0f), radians(lightAngle), vec3(0.0f, 1.0f, 0.0f)) * vec4(lightDir, 1.0f));
	mat4 lightView = lookAt(lightDirTr, vec3(0, 0, 0), vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}

void initLights()
{
	float linear = 3.0;
	float quadratic = 10.0;

	PointLight p1 = {
		vec3(0, 1, 0), //pos
		1.0,
		linear,
		quadratic,
		vec3(1.0f, 0.78f, 0.78f) * 0.1f, //ambient
		vec3(0.51f, 0.86f, 1.0f), //diffuse
		vec3(0.31f, 0.648f, 0.78f) //specular
	};

	PointLight p2 = {
		vec3(0, 1, -0.95),
		1.0,
		linear,
		quadratic,
		vec3(1.0f, 0.78f, 0.78f) * 0.1f,
		vec3(1.0f, 0.45f, 0.0f),
		vec3(0.31f, 0.648f, 0.78f)
	};

	pointLights[0] = p1;
	pointLights[1] = p2;

}

void sendLightUnif(GLuint shader)
{

	for (GLuint i = 0; i < 2; i++)
	{
		std::string number = std::to_string(i);
		glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].position").c_str()), 1, (GLfloat*)&pointLights[i].position.x);
		glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].ambient").c_str()), 1, &pointLights[i].ambient.x);
		glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].diffuse").c_str()), 1, &pointLights[i].diffuse.x);
		glUniform3fv(glGetUniformLocation(shader, ("pointLights[" + number + "].specular").c_str()), 1, &pointLights[i].specular.x);
		glUniform1f(glGetUniformLocation(shader, ("pointLights[" + number + "].constant").c_str()), pointLights[i].constant);
		glUniform1f(glGetUniformLocation(shader, ("pointLights[" + number + "].linear").c_str()), pointLights[i].linear);
		glUniform1f(glGetUniformLocation(shader, ("pointLights[" + number + "].quadratic").c_str()), pointLights[i].quadratic);
	}
}



vector<vec3> generatePointCloud()
{	
	Model frac(R"(E:\Programs\Mandelbulb3Dv199\Meshes\cuby.ply)");
	//return frac.meshes[0].vertices;
	vector<vec3> points;// = frac.meshes[0].vertices;
	vector<vec3> othpoints = frac.meshes[0].vertices;
	for(int i = 0; i < othpoints.size(); i+= 3)
	{
		othpoints[i] *= 100.0f;
		points.push_back(othpoints[i]);
	}
	return points;
	vec3 startPoint(-10.0, -10.0f, -10.0f);
	vec3 endPoint(-10.0, -10.0f, -10.0f);

	bool odd = false;
	for (float z = -1.0f; z < 1.05f; z += 0.1f)
	{
		for (float y = -1.0f; y < 1.05f; y += 0.1f)
		{
			for (float x = -1.0f; x < 1.05f; x += 0.1f)
			{
				if(z < x*y)
					points.emplace_back(x*16.0f + (odd ? 0.8f : 0.0f), z*14.0f, y*14.0f);
			}
			odd = !odd;
		}
	}

	return points;

}

void initGlobals()
{
	lightDir = vec3(1.0f, 1.0f, 0.0f);
}

GLuint LoadSkyBoxTextures(std::vector<const GLchar*> skyBoxFaces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height, n;
	unsigned char* image;
	int force_channels = 3;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < skyBoxFaces.size(); i++)
	{
		image = stbi_load(skyBoxFaces[i], &width, &height, &n, force_channels);
		if (!image) {
			fprintf(stderr, "ERROR: could not load %s\n", skyBoxFaces[i]);
			return false;
		}
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

gps::SkyBox mySkyBox;
gps::ShaderG skyboxShader;
void initSkyBox()
{
	std::vector<const GLchar*> faces;
	faces.push_back("textures/skybox/right.tga");
	faces.push_back("textures/skybox/left.tga");
	faces.push_back("textures/skybox/top.tga");
	faces.push_back("textures/skybox/bottom.tga");
	faces.push_back("textures/skybox/back.tga");
	faces.push_back("textures/skybox/front.tga");

	mySkyBox.Load(faces);
	skyboxShader.loadShader("vcfshaders/skyboxShader.vert", "vcfshaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"), 1, GL_FALSE,
		glm::value_ptr(view));

	projection = glm::perspective(glm::radians(45.0f), winWidth / winHeight, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1, GL_FALSE,
		glm::value_ptr(projection));
}

#define debug_LightDepth false

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
	glEnable(GL_BLEND);
	glClearColor(0.1, 0.0, 0.3, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initGlobals();
	initFBOs();
	initLights();

	/*Prepare ze battlefield*/

	/*Vertex Shader*/
	//Shader vertShader("vcfshaders\\scGeom\\dark.vert", Shader::VERTEX_SHADER);
	Shader vertShader("vcfshaders\\fractal\\dark.vert", Shader::VERTEX_SHADER);
	Shader mainvShader("vcfshaders\\main.vert", Shader::VERTEX_SHADER);
	Shader depthvShader("vcfshaders\\simpleDepthMap.vert", Shader::VERTEX_SHADER);
	Shader skyboxvShader("vcfshaders\\skyboxShader.vert", Shader::VERTEX_SHADER);

	/*Fragment Shader*/
	Shader fragShader("vcfshaders\\fractal\\dark.frag", Shader::FRAGMENT_SHADER);
	Shader mainfShader("vcfshaders\\main.frag", Shader::FRAGMENT_SHADER);
	Shader depthfShader("vcfshaders\\simpleDepthMap.frag", Shader::FRAGMENT_SHADER);
	Shader skyboxfShader("vcfshaders\\skyboxShader.frag", Shader::FRAGMENT_SHADER);

	/*Compute Shader*/
	Shader compShader("vcfshaders\\rmd.comp", Shader::COMPUTE_SHADER);

	/*Geometry Shader*/
	Shader geomShader("vcfshaders\\fractal\\dark.geom", Shader::GEOMETRY_SHADER);

	/*Final Program*/
	ProgramShader progShader(vertShader, geomShader, fragShader);
	ProgramShader mainShader(mainvShader, mainfShader);
	ProgramShader depthShader(depthvShader, depthfShader);
	//ProgramShader skyboxShader(skyboxvShader, skyboxfShader);



	ProgramShader csprogShader(compShader);

	glCheckError();

	//Terra terrain(512, 512, R"(D:\Projects\C++\KatEngine\Pukman\winter.jpg)");
	Texture tRocks(R"(Textures\Rocks.jpg)");
	Texture tLeaves(R"(Textures\Leaves.jpg)");
	Terra terrain(512, 512, R"(Textures\Map.png)");
	//Terra terrain(512, 512, R"(Textures\Map2.tif)");


	/*Model bolly("Models\\sphere.obj");
	bolly.loadMeshes(0, 0);*/
	initSkyBox();
	Plane plane;

	vector<vec3> points = generatePointCloud();

	PointCloud pt_cloud(points);


	float interp = 1.0f; //folosit la interpolat doua modele de sfere pentru o animatie

	double lastTime = glfwGetTime();
	int nbFrames = 0;


	//Texture tex(R"(D:\Projects\C++\KatEngine\Pukman\winter.jpg)");
	//Texture tex2(R"(D:\Photos\20171712_105524.jpg)");
	Texture texbl(KAT_TEXTURE_SIZE, KAT_TEXTURE_SIZE);//blank texture 
	Texture tex_perlin = Texture::FromGenFunction(KAT_TEXTURE_SIZE, KAT_TEXTURE_SIZE,[](int* dat, int w, int h)
	{
		srand(time(NULL));
		for(int y = 0; y < h; y++)
			for(int x = 0; x < w; x++)
			{
				dat[y*w + x] = rand() % 256;
			}
	});

	tex_perlin.generateMipmap();

	//Model cubeTexel("Models\\cubeTexel.obj");
	Model cubeTexel("Models\\circle16.obj");

	progShader.use();

	int loc1 = glGetUniformLocation(progShader, "geom_Vert");
	glUniform3fv(glGetUniformLocation(progShader, "geom_Vert"), cubeTexel.fVertices().size() * 6, (GLfloat*)&cubeTexel.fVertices()[0]);
	glUniform1iv(glGetUniformLocation(progShader, "geom_Ind"), cubeTexel.fIndices().size(), (GLint*)&cubeTexel.fIndices()[0]);
	glUniform1i(glGetUniformLocation(progShader, "geom_INum"), cubeTexel.fIndices().size());

	mainShader.use();
	sendLightUnif(mainShader);


	mat4 passThroughcam = glm::identity<mat4>();
	/*Render loop*/
	while (!glfwWindowShouldClose(window))
	{
		#pragma region Animations & Time
		/*Stopwatch - ms > fps :D 60 fps = < 16ms*/
		double currentTime = glfwGetTime();
		nbFrames++;

		if (currentTime - lastTime >= 1.0) { 
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
		}

		keyHeldCallback(currentTime - lastTime);
		lastTime = currentTime;

		/*Update animatio interpolation uniform*/
		interp += 0.002f;
		//if (interp > 2.0f) interp = 0.0f;
		//texbl.clear();
		/*use the blank texture as compute shader frame buffer*/
		//texbl.bindImage(0);
		//tex_perlin.bindTexture(1);
		#pragma endregion
		/*Calculate the forward rays for the corners of the camera projection screen*/
		mat4 cam = getCamera();
		mat4 invcam = inverse(cam);
		mat4 model = mat4(1.0);

		vec4 calc = invcam * vec4(-1, -1, 0, 1); calc /= calc.w;
		vec3 ray00 = vec3(calc);
		calc = invcam * vec4(-1, 1, 0, 1); calc /= calc.w;
		vec3 ray01 = vec3(calc);
		calc = invcam * vec4(1, -1, 0, 1); calc /= calc.w;
		vec3 ray10 = vec3(calc);
		calc = invcam * vec4(1, 1, 0, 1); calc /= calc.w;
		vec3 ray11 = vec3(calc);

		/*glUseProgram(csprogShader);
		//glUniform1f(glGetUniformLocation(csprogShader, "roll"), interp*10);
		auto loc = glGetUniformLocation(csprogShader, "eye");
		glUniform3fg(0, cam_pos);
		glUniform3fg(1, ray01 - cam_pos);
		glUniform3fg(2, ray00 - cam_pos);
		glUniform3fg(3, ray11 - cam_pos);
		glUniform3fg(4, ray10 - cam_pos);
		glDispatchCompute(KAT_TEXTURE_SIZE / 8 , KAT_TEXTURE_SIZE / 8, 1); //1024 512^2 threads in blocks of 16^2*/
		/*
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texbl.bindTexture(0);*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		/*progShader.use();

		glUniformMatrix4fv(glGetUniformLocation(progShader.id, "mvp"), 1, GL_FALSE, &cam[0][0]);
		//glUniform1f(glGetUniformLocation(progShader, "lerp_value"), interp > 1.0f ? 2 - interp : interp);
		glUniform1f(glGetUniformLocation(progShader, "time"), currentTime);

		pt_cloud.draw();*/

		#pragma region Shadows
		depthShader.use();


	    glUniformMatrix4fv(glGetUniformLocation(depthShader, "lightSpaceTrMatrix"), 1, GL_FALSE,
			value_ptr(computeLightSpaceTrMatrix()));

		model = mat4(1.0);
		glUniformMatrix4fv(glGetUniformLocation(depthShader, "model"),1, GL_FALSE, &model[0][0]);

#if !debug_LightDepth
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
#endif

		terrain.Draw();

#if !debug_LightDepth
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, (int)winWidth, (int)winHeight);
#endif
		//#pragma endregion
#if !debug_LightDepth
		mainShader.use();

		glUniform3fv(glGetUniformLocation(mainShader, "lightDir"), 1, glm::value_ptr(lightDir));
		glUniform3fv(glGetUniformLocation(mainShader, "dirlightColor"), 1, glm::value_ptr(vec3(0.8,0.9,1.0)));

		glUniformMatrix4fv(glGetUniformLocation(mainShader, "lightSpaceTrMatrix"), 1, GL_FALSE,
			value_ptr(computeLightSpaceTrMatrix()));

		/*glUniformMatrix4fv(glGetUniformLocation(mainShader, "lightSpaceTrMatrix"), 1, GL_FALSE,
			&cam[0][0]);*/


		glUniform3fv(glGetUniformLocation(mainShader, "camPosition"), 1, (GLfloat*)&cam_pos);

		glUniformMatrix4fv(glGetUniformLocation(mainShader, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(mainShader, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(mainShader, "projection"), 1, GL_FALSE, &projection[0][0]);

		mat3 normalMatrix = mat3(inverseTranspose(view*model));
		glUniformMatrix3fv(glGetUniformLocation(mainShader, "normalMatrix"), 1, GL_FALSE, value_ptr(normalMatrix));

		mat3 lightDirMatrix = glm::mat3(glm::inverseTranspose(view));
		glUniformMatrix3fv(glGetUniformLocation(mainShader, "lightDirMatrix"), 1, GL_FALSE, value_ptr(lightDirMatrix));


		//glUniformMatrix4fv(glGetUniformLocation(mainShader, "view"), 1, GL_FALSE, &lightView[0][0]);

		//glUniformMatrix4fv(glGetUniformLocation(mainShader, "projection"), 1, GL_FALSE, &model[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(mainShader, "projection"), 1, GL_FALSE, &lightProjection[0][0]);

		tRocks.bindTexture(0);
		glUniform1i(glGetUniformLocation(mainShader, "colorMap1"), 0);

		tLeaves.bindTexture(1);
		glUniform1i(glGetUniformLocation(mainShader, "colorMap2"), 1);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(mainShader, "shadowMap"), 3);

		terrain.Draw();
#endif
		skyboxShader.useShaderProgram();
		mySkyBox.Draw(skyboxShader.shaderProgram, view * model, projection);


		glfwSwapBuffers(window);
		glfwPollEvents();
		glCheckError();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
	
}