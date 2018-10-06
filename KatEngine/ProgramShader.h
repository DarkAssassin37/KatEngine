#pragma once
#include <glad/glad.h>

class ProgramShader
{
public:
	GLuint id;

	ProgramShader(GLuint shader1, GLuint shader2);
	~ProgramShader();

	void use() const;
};

