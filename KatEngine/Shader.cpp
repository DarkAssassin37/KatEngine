#include "Shader.h"
#include "utils/io.h"
#include "messages.h"


Shader::Shader(const char* filepath, GLint shaderType)
{
	this->filepath = filepath;
	id = glCreateShader(shaderType);
	const GLchar* vertSource = loadStringFromFile(filepath);

	if (vertSource == NULL)
	{
		printf("Could not open shader file: %s", filepath);
		fatal_error("");
	}

	glShaderSource(id, 1, &vertSource, NULL);

	glCompileShader(id);

	int rvalue;
	glGetShaderiv(id, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue) {
		printf("Error in compiling shader %s\n", filepath);
		GLchar log[10240];
		GLsizei length;
		glGetShaderInfoLog(id, 10239, &length, log);
		printf("Compiler log:\n%s\n", log);
		fatal_error("");
	}

	delete[] vertSource;
}


Shader::~Shader()
{
	glDeleteShader(id);
}

Shader::operator GLuint() const
{
	return id;
}
