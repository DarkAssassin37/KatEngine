#include "Model.h"
#include "utils/io.h"
#include <string>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "messages.h"

using namespace glm;

Model::Model()
{
}

Model::Model(const char* filepath)
{
	this->filepath = filepath;
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		fatal_error(importer.GetErrorString());

	processNode(scene->mRootNode, scene);

	vao = vbo_vertCurr = vbo_vertNext = vbo_normalsCurr = vbo_normalsNext = vbo_uvs = 0;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_vertCurr);
	glGenBuffers(1, &vbo_vertNext);
	glGenBuffers(1, &vbo_normalsCurr);
	glGenBuffers(1, &vbo_normalsNext);
	glGenBuffers(1, &vbo_uvs);
	glGenBuffers(1, &ebo);


	glBindVertexArray(vao);


	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertCurr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertNext);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normalsCurr);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normalsNext);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	if(!meshes[0].uvs.empty())
		glBufferData(GL_ARRAY_BUFFER, meshes[0].uvs.size() * sizeof(vec2), &meshes[0].uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[0].indices.size() * sizeof(unsigned int), &meshes[0].indices[0], GL_STATIC_DRAW);

}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(mesh, scene);
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


Model::~Model()
{
}

void Model::loadMeshes(int current, int next)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertCurr);
	glBufferData(GL_ARRAY_BUFFER, meshes[current].vertices.size() * sizeof(vec3), &meshes[current].vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertNext);
	glBufferData(GL_ARRAY_BUFFER, meshes[next].vertices.size() * sizeof(vec3), &meshes[next].vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normalsCurr);
	if(!meshes[current].normals.empty())
	glBufferData(GL_ARRAY_BUFFER, meshes[current].normals.size() * sizeof(vec3), &meshes[current].normals[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normalsNext);
	if (!meshes[next].normals.empty())
	glBufferData(GL_ARRAY_BUFFER, meshes[next].normals.size() * sizeof(vec3), &meshes[next].normals[0], GL_DYNAMIC_DRAW);

}


void Model::draw()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glDrawElements(GL_TRIANGLES, meshes[0].indices.size(), GL_UNSIGNED_INT, NULL);
}

std::vector<vec3> Model::fVertices()
{
	return meshes[0].vertices;
}

std::vector<unsigned> Model::fIndices()
{
	return meshes[0].indices;
}

std::vector<vec3> Model::fNormals()
{
	return meshes[0].normals;
}

