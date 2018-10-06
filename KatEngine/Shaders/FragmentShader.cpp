#include "FragmentShader.h"
#include "messages.h"
#include "utils/io.h"


FragmentShader::FragmentShader(const char* filepath)
{
	this->filepath = filepath;
	id = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar * vertSource = loadStringFromFile(filepath);

	glShaderSource(id, 1, &vertSource, NULL);

	glCompileShader(id);
	if (!check_shader_compilation(id, "Vertex "))
		fatal_error("");

	delete[] vertSource;
}


FragmentShader::~FragmentShader()
{
	glDeleteShader(id);
}

FragmentShader::operator GLuint() const
{
	return id;
}
