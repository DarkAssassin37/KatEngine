#include "ComputeShader.h"
#include "utils/io.h"
#include "messages.h"



ComputeShader::ComputeShader(const char* filepath)
{
	this->filepath = filepath;
	id = glCreateShader(GL_COMPUTE_SHADER);
	const GLchar* compSource = loadStringFromFile(filepath);

	glShaderSource(id, 1, &compSource, NULL);

	glCompileShader(id);
	/*if (!check_shader_compilation(id, "Compute "))
		fatal_error("");
		*/
	int rvalue;
	glGetShaderiv(id, GL_COMPILE_STATUS, &rvalue);
	if (!rvalue) {
		fprintf(stdout, "Error in compiling the compute shader\n");
		GLchar log[10240];
		GLsizei length;
		glGetShaderInfoLog(id, 10239, &length, log);
		fprintf(stdout, "Compiler log:\n%s\n", log);
		fatal_error("");
	}

	delete[] compSource;
}


ComputeShader::~ComputeShader()
{
	glDeleteShader(id);
}

ComputeShader::operator GLuint() const
{
	return id;
}

