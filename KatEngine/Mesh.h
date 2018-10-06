#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <tiny_obj_loader.h>


class Mesh
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;

	Mesh(tinyobj::shape_t obj);
	~Mesh();
};

