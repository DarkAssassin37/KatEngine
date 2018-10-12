#pragma once
#include <string>
#include <map>
using namespace std;

class Texture
{
public:
	static map<string, map<int, int>> textures;

	unsigned int id;
	std::string type;
	std::string path;
	
	Texture(const char * path);
	~Texture();

	void bindTexture(int texID);
};

