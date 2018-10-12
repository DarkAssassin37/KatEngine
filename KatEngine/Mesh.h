#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <assimp/scene.h>


class Mesh
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;
	std::string name;

	Mesh(aiMesh *mesh, const aiScene *scene);
	~Mesh();
};

