#pragma once
#include <string>
#include <map>
#include <glad/glad.h>
using namespace std;

class Texture
{
public:
	static map<string, map<int, int>> textures;

	static const int READ_ONLY = GL_READ_ONLY;
	static const int WRITE_ONLY = GL_WRITE_ONLY;
	static const int READ_WRITE = GL_READ_WRITE;

	unsigned int id;
	std::string type;
	std::string path;
	int width, height;
	
	Texture(const char * path);
	Texture(int width, int height);
	~Texture();

	void bindTexture(int unit);
	void bindImage(int unit, int access = GL_WRITE_ONLY);
};

