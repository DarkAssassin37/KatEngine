#include "Mesh.h"



Mesh::Mesh(tinyobj::shape_t obj)
{
	for (unsigned int i = 0; i < obj.mesh.positions.size() / 3; i++)
		vertices.emplace_back(obj.mesh.positions[3 * i + 0], obj.mesh.positions[3 * i + 1], obj.mesh.positions[3 * i + 2]);

	for (unsigned int i = 0; i < obj.mesh.normals.size() / 3; i++)
		normals.emplace_back(obj.mesh.normals[3 * i + 0], obj.mesh.normals[3 * i + 1], obj.mesh.normals[3 * i + 2]);

	for (unsigned int i = 0; i < obj.mesh.texcoords.size() / 2; i++)
		uvs.emplace_back(obj.mesh.texcoords[2 * i + 0], obj.mesh.texcoords[2 * i + 1]);

	indices = obj.mesh.indices;
	
	/*for (size_t f = 0; f < obj.mesh.indices.size() / 3; f++) {
		int vertexId1 = obj.mesh.indices[3 * f + 0];
		int vertexId2 = obj.mesh.indices[3 * f + 1];
		int vertexId3 = obj.mesh.indices[3 * f + 2];

	}*/
}


Mesh::~Mesh()
{
}
