#include "VertexShader.h"
#include "utils/io.h"
#include "messages.h"


VertexShader::VertexShader(const char* filepath)
{
	this->filepath = filepath;
	id = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSource = loadStringFromFile(filepath);

	glShaderSource(id, 1, &vertSource, NULL);

	glCompileShader(id);
	/*if (!check_shader_compilation(id, "VERTEX "))
		fatal_error("");*/

	int rvalue;
	glGetShaderiv(id, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue) {
		fprintf(stdout, "Error in compiling the vertex shader\n");
		GLchar log[10240];
		GLsizei length;
		glGetShaderInfoLog(id, 10239, &length, log);
		fprintf(stdout, "Compiler log:\n%s\n", log);
		fatal_error("");
	}

	delete[] vertSource;
}


VertexShader::~VertexShader()
{
	glDeleteShader(id);
}

VertexShader::operator GLuint() const
{
	return id;
}
