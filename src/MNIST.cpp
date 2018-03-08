#include "MNIST.h"

bool MNISTLoader::Get8(char* ptr)
{
	if (ifs.good())
	{
		if (!ifs.eof())
		{
			ifs.read(ptr, 1);
			//std::cout << *ptr;
			return true;
		}
	}
	return false;
}

bool MNISTLoader::Get32(int* ptr)
{
	for (int i = 3; i > -1; --i)
	{
		if (!Get8(reinterpret_cast<char*>(ptr) + i)) return false;
	}
	return true;
}

MNISTLoader::MNISTLoader(std::string filePath)
{
	ifs.open(filePath, std::ios::binary | std::ios::in | std::ios::out);
	if (ifs.is_open())
	{
            std::cout << "File loaded succesfully." << std::endl;
            
		char dims = 0;
		char s = 0;
		int count = 0;

		// data we don't need
		Get8(&s);
		Get8(&s);
		Get8(&s);

		// number of dimensions
		Get8(&dims);

		// count in respective dimensions
		for (int i = 0; i < dims; ++i)
		{
			Get32(&count);
			dimCount.push_back(count);
		}

		for (int i : dimCount)
		{
			std::cout << i << std::endl;
		}
	}
        
        else {
            std::cout << "Could not load file." << std::endl;
        }
}

std::unique_ptr<MNISTLoader::uchar> MNISTLoader::FetchItem()
{
	if (currItem < dimCount[0])
	{
		std::allocator<uchar> myAlloc;
		long unitSize = GetUnitSize();
		uchar* output = myAlloc.allocate(unitSize);
		for (int i = 0; i < unitSize; ++i)
		{
			Get8(reinterpret_cast<char *>(output + i));
		}
		currItem++;
		return std::unique_ptr<uchar>(output);
	}
	return nullptr;
}

std::unique_ptr<MNISTLoader::uchar> MNISTLoader::FetchAll()
{
	if (currItem == 0)
	{
		std::allocator<uchar> myAlloc;
		long unitSize = GetUnitSize();
		uchar* output = myAlloc.allocate(unitSize * dimCount[0]);
		for (long i = 0; i < unitSize * dimCount[0]; ++i)
		{
			Get8(reinterpret_cast<char *>(output + i));
                        if (i < 10000) {
                            if ((int)*(output + i)<122) std::cout << 0;
                            else std::cout << 1;
                            if (i%28==0) std::cout << std::endl;
                        }
		}
		currItem = dimCount[0];
		return std::unique_ptr<uchar>(output);
	}

	return nullptr;
}

bool MNISTLoader::is_done() const
{
	if (currItem >= dimCount[0]) return true;
	else return false;
}

long MNISTLoader::GetUnitSize() const
{
	long unitSize = 1;
	for (int i = 1; i < dimCount.size(); ++i)
	{
		unitSize *= dimCount[i];
	}
	return unitSize;
}

long MNISTLoader::GetSetSize() const
{
	return dimCount[0];
}