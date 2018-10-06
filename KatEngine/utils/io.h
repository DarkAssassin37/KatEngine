#pragma once
#include <fstream>

/*don't forget to delete memory*/
inline char* loadStringFromFile(const char*const filepath)
{
	char* output = NULL;
	std::ifstream file(filepath, std::ios::in | std::ios::ate | std::ios::binary);
	if (file.is_open())
	{
		const int size = file.tellg();
		output = new char[size + 1];
		file.seekg(std::ios::beg);
		file.read(output, size);
		output[size] = 0;
		file.close();
	}
	return output;
}