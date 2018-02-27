#pragma once

#include <fstream>
#include <string>
#include <iostream>

class FileLoader
{
	std::ifstream ifs;

public:

	FileLoader(std::string);

	bool Get8(char* ptr);
	bool Get16();
	bool Get32(int* ptr);
	bool Get64(double* ptr);
};