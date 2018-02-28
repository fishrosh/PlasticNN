#pragma once

#include <ios>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <bitset>
#include <memory>
#include <vector>

class MNISTLoader
{
    
public:
    
    using uchar = unsigned char;
    
private:
    
	std::ifstream ifs;
	std::vector<int> dimCount;

	int currItem{ 0 };

	bool Get8(char* ptr);
	bool Get16();
	bool Get32(int* ptr);
	bool Get64();

public:

	MNISTLoader(std::string filePath);

	std::unique_ptr<char> FetchItem();
	std::unique_ptr<uchar> FetchAll();

	bool is_done() const;
	long get_unit_size() const;
	long get_set_size() const;
};