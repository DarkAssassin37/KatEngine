#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include "Texture.h"
#include <map>

glm::vec3 fromaiVect(aiVector3D v)
{
	return glm::vec3(v.x, v.y, v.z);
}


std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		const char * aiShit = (*mat->mProperties)->mKey.C_Str();
		const std::string texName(aiShit);

		using namespace std;


		auto it = Texture::textures.find(texName);
		if(it != Texture::textures.end())
		{
			continue;
		}
		aiString str; 
		mat->GetTexture(type, i, &str);
		
		std::cout << "break;";

		/*Texture texture;
		//texture.id = TextureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str;
		textures.push_back(texture);*/
	}
	return textures;
}


Mesh::Mesh(aiMesh *mesh, const aiScene *scene)
{
	using namespace glm;
	using namespace std;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.push_back(fromaiVect(mesh->mVertices[i]));
		if(mesh->mNormals != NULL)
		normals.push_back(fromaiVect(mesh->mNormals[i]));

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			uvs.emplace_back(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//albedo
	//normals
	//metallic
	//roughness
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		
		vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		vector<Texture> normalsMaps = loadMaterialTextures(material,
			aiTextureType_NORMALS, "texture_specular");
		vector<Texture> shininessMaps = loadMaterialTextures(material,
			aiTextureType_SHININESS, "texture_specular");
		/*textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());*/
	}
	
}


Mesh::~Mesh()
{
}
