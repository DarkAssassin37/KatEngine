#pragma once
#include <glad/glad.h>

class FragmentShader
{
public:
	GLuint id;
	const char* filepath;

	FragmentShader(const char* filepath);
	~FragmentShader();


	operator GLuint() const;
};

