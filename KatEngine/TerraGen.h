#pragma once
#include <glad/glad.h>
#include "ProgramShader.h"

class TerraGen
{
public:
	TerraGen();

	void Draw();

	~TerraGen();

	GLuint onemoreProgShader;
	GLuint generatedVAO;
	void DrawArrayVAO(GLuint vao, GLuint program);
	GLuint GenerateObject();
	GLuint Precompute();
};

