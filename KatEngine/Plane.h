#pragma once
#include "Model.h"
class Plane
{
public:
	unsigned int vao, vbo, ebo, uvs;

	Plane();
	~Plane();

	void draw();
};

