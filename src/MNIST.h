#pragma once

#include <ios>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <bitset>
#include <memory>
#include <vector>

#include "DatasetLoader.h"

class MNISTLoader : public DatasetLoader<unsigned char>
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

	std::unique_ptr<uchar> FetchItem() override;
	std::unique_ptr<uchar> FetchAll() override;
        
	long GetUnitSize() const override;
	long GetSetSize() const override;

	bool is_done() const;
};