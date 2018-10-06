#pragma once
#include <glad/glad.h>

class VertexShader
{
public:
	GLuint id;
	const char* filepath;

	VertexShader(const char* filepath);
	~VertexShader();

	operator GLuint() const;
};

