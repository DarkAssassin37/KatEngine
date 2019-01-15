#pragma once
#include "ProgramShader.h"

class Terra
{
public:
	unsigned int vao, vbo, ebo;
	Terra(int vertWidth, int vertHeight, const char * texturePath);

	void Draw();

	~Terra();
private:
	int GROUP_SIZE_WIDTH = 8;
	int GROUP_SIZE_HEIGHT = 8;
	int width, height;
	ProgramShader onemoreProgShader;

	void precomputeEBO();
};

