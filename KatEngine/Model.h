#pragma once
#include <vector>
#include "Mesh.h"
#include <glm/glm.hpp>
#include <assimp/scene.h>


class Model
{
public:

	std::vector<Mesh> meshes;

	unsigned int vao, vbo_vertCurr, vbo_vertNext, vbo_normalsCurr, vbo_normalsNext, vbo_uvs, ebo;

	Model();
	Model(const char* filepath);
	~Model();

	void loadMeshes(int current, int next);
	void setInterpolation(float interp);
	void draw();


	std::vector<glm::vec3> fVertices();
	std::vector<unsigned> fIndices();
	std::vector<glm::vec3> fNormals();

private:
	const char* filepath;
	void processNode(aiNode* node, const aiScene* scene);
};

