#pragma once
#include <glad/glad.h>

class Shader
{
public:
	static const GLuint VERTEX_SHADER = GL_VERTEX_SHADER;
	static const GLuint FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
	static const GLuint COMPUTE_SHADER = GL_COMPUTE_SHADER;
	GLuint id;
	const char* filepath;

	Shader(const char* filepath, GLint shaderType);
	~Shader();

	operator GLuint() const;
};

