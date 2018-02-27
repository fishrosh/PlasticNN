#include "FileLoader.h"

FileLoader::FileLoader(std::string fileName)
{
	ifs.open(fileName, std::ios::binary | std::ios::in | std::ios::out);
	if (ifs.is_open())
	{
		std::cout << "File succesfully opened." << std::endl;
	}

	else {
		std::cout << "Having trouble opening file" << std::endl;
		throw "Holy cow!";
	}
}

bool FileLoader::Get8(char* ptr)
{
	if (ifs.good())
	{
		if (!ifs.eof())
		{
			ifs.read(ptr, 1);
			return true;
		}
	}
	return false;
}

bool FileLoader::Get32(int* ptr)
{
	for (int i = 0; i < 4; ++i)
	{
		if (!Get8(reinterpret_cast<char*>(ptr) + i)) return false;
	}
	return true;
}

bool FileLoader::Get64(double* ptr)
{
	for (int i = 0; i < 8; ++i)
	{
		if (!Get8(reinterpret_cast<char*>(ptr) + i)) return false;
	}
	return true;
}