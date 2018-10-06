#include "ProgramShader.h"
#include "messages.h"


ProgramShader::ProgramShader(GLuint shader1, GLuint shader2)
{
	id = glCreateProgram();
	glAttachShader(id, shader1);
	glAttachShader(id, shader2);
	glLinkProgram(id);


	if (!check_shader_compilation(id, "Program "))
		fatal_error("");
}

ProgramShader::~ProgramShader()
{
	glDeleteProgram(id);
}

void ProgramShader::use() const
{
	glUseProgram(id);
}
