#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include "messages.h"

std::map<std::string, std::map<int,int>> Texture::textures;

Texture::Texture(const char * path)
{
	id = 0;
	int width, height, nrChannels;//R"(D:\Projects\C++\KatEngine\Pukman\winter.jpg)"
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!data)
		fatal_error("Couldn't load texture image");
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}




Texture::~Texture()
{
}

void Texture::bindTexture(int texID)
{
	glActiveTexture(GL_TEXTURE0 + texID);
	glBindTexture(GL_TEXTURE0 + texID, id);
}
