#include "Model.h"
#include "utils/io.h"
#include <string>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <tiny_obj_loader.h>

using namespace glm;

Model::Model(const char* filepath)
{
	this->filepath = filepath;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = LoadObj(shapes, materials, err, filepath);

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	for (unsigned int i = 0; i < shapes.size(); i++)
		meshes.emplace_back(shapes[i]);

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
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertNext);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normalsCurr);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normalsNext);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	if(meshes[0].uvs.size() > 0)
		glBufferData(GL_ARRAY_BUFFER, meshes[0].uvs.size() * sizeof(vec2), &meshes[0].uvs[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[0].indices.size() * sizeof(unsigned int), &meshes[0].indices[0], GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, meshes[current].normals.size() * sizeof(vec3), &meshes[current].normals[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normalsNext);
	glBufferData(GL_ARRAY_BUFFER, meshes[next].normals.size() * sizeof(vec3), &meshes[next].normals[0], GL_DYNAMIC_DRAW);

}


void Model::draw()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glDrawElements(GL_TRIANGLES, meshes[0].indices.size(), GL_UNSIGNED_INT, NULL);
}
