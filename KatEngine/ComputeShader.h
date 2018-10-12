#pragma once
#include <glad/glad.h>

class ComputeShader
{
public:
	GLuint id;
	const char* filepath;

	ComputeShader(const char* filepath);
	~ComputeShader();


	operator GLuint() const;
};

