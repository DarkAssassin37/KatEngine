#include "PointCloud.h"
#include <glad/glad.h>
#include <vector>
#include <glm/vec3.hpp>

PointCloud::PointCloud(std::vector<glm::vec3>& points)
{
	Points = points;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);


	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

	glBufferData(GL_ARRAY_BUFFER, Points.size() * 3 * sizeof(float), &Points[0], GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}




PointCloud::~PointCloud()
{
}

void PointCloud::UpdatePoints()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, Points.size(), &Points[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointCloud::draw()
{
	glBindVertexArray(vao);

	glDrawArrays(GL_POINTS, 0, Points.size());
	//glDrawElements(GL_POINTS, Points.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}
