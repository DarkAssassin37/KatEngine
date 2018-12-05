#pragma once
#include <vector>
#include <glm/vec3.hpp>

class PointCloud
{
public:
	unsigned int vao, vbo, uvs;

	std::vector<glm::vec3> Points;

	PointCloud(std::vector<glm::vec3>& points);
	~PointCloud();

	void UpdatePoints();

	void draw();
};

