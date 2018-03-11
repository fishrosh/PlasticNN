// DumbedDownNeuralNet.cpp : Defines the entry point for the console application.
//

#include <queue>
#include <random>
#include <iostream>
#include <vector>
#include <sstream>

#include "NeuralNet.h"
#include "MNIST.h"
#include "Dumbed.h"
#include "ImageProcessor.h"

#define LINUX_PLATFORM

struct CorectnessRate
{
	std::queue<char> data;
	int sum{ 0 };
	UINT probe{ 30000 };
	
	double ratio() { return data.size() != 0 ? sum / static_cast<double>(data.size()) : 0.0; } 

	void add(char k)
	{
		data.push(k);
		sum += k;

		if (data.size() > probe) {
			sum -= data.front();
			data.pop();
		}
	}
};

std::ostream& operator<<(std::ostream& out, const NeuralNet& nn)
{
	return nn.Out(out);
}

UINT inputXSize = 28;
UINT inputYSize = 28;

double ReadInputImage(std::unique_ptr<char>& input, UINT i, UINT j, UINT topSize, long offset)
{
    UINT finalI = static_cast<UINT>(std::floor( i / static_cast<double>(topSize) * 28));
    UINT finalJ = static_cast<UINT>(std::floor( j / static_cast<double>(topSize) * 28));
    
    return static_cast<double>(static_cast<unsigned char>(*(input.get() + finalI * 28 + finalJ + offset))) / 255.0;
}

void MakeInputVectorS(std::vector<double>& output, std::unique_ptr<char>& input, int count, long offset)
{
    Randomize<> r;
    r.SetRangeReal(0.0, 0.1);
    r.SetRangeInt(0, 14);
    
    UINT sizeMod = r.GetInt();
    
    UINT sizeX = 21 + sizeMod;
    UINT sizeY = 21 + sizeMod;
    
    //UINT sizeX = 28;
    //UINT sizeY = 28;
    
    r.SetRangeInt(0, inputXSize - sizeX -1);
    UINT offsetX = r.GetInt();
    
    r.SetRangeInt(0, inputYSize - sizeY -1);
    UINT offsetY = r.GetInt();
    
    for (int i{ 0 }; i++ < inputXSize;)
    {
        for (int j{ 0 }; j++ < inputYSize;)
        {
            double o = 0.0f;
            
            if (i >= offsetX && i < offsetX + sizeX &&
                    j >= offsetY && j < offsetY + sizeY ) 
            {
                o = ReadInputImage(input, (i - offsetX), (j - offsetY), sizeX, offset);
                        //static_cast<double>(static_cast<unsigned char>(*(input.get() + (j - offsetY) + (i - offsetX) * 28 + offset))) / 255.0;
		if (o < 0 || o > 1) throw 1;
		
            }
            
            else {
                o = r.GetReal();
            }
            
            output.push_back(o);
            //if(o < 0.4) std::cout << ".";
            //else std::cout << "8";
        }
        
        //std::cout << std::endl;
    }
}

void MakeInputVector(std::vector<double>& output, std::unique_ptr<char>& input, int count, long offset)
{
	for (int i = 0; i < count; ++i)
	{
		double o = static_cast<double>(static_cast<unsigned char>(*(input.get() + i + offset))) / 255.0;
		if (o < 0 || o > 1) throw 1;
		output.push_back(o);
	}//*/
}

void MakeInputVector(std::vector<double>& output, std::unique_ptr<char>& input, int count)
{
	MakeInputVector(output, input, count, 0);
}

template <class input_t = unsigned char, class output_t = double>
void MakeResultVectorK(std::vector<output_t>& output, input_t input)
{
	for (input_t c = 0; c < 10; ++c)
	{
		if (c == input) {
                    output.push_back(1.0);
                }
                
		else output.push_back(0.0);
	}
}

void MakeResultVector(std::vector<double>& output, char input)
{
	for (double c = 0; c < 10; ++c)
	{
		if (c == static_cast<double>(static_cast<unsigned char>(input))) 
			output.push_back(1.0);
		else output.push_back(0.0);
	}
}

void MNISTStochasticRun()
{
        std::cout << "Heya heyo!" << std::endl;

#ifndef LINUX_PLATFORM
	MNISTLoader images("C:\\Users\\Kamil\\Downloads\\biernaty\\mnist\\train-images.idx3-ubyte");
	MNISTLoader labels("C:\\Users\\Kamil\\Downloads\\biernaty\\mnist\\train-labels.idx1-ubyte");
#else
	MNISTLoader images("/home/kamil/train-images.idx3-ubyte");
	MNISTLoader labels("/home/kamil/train-labels.idx1-ubyte");
#endif // !LINUX_PLATFORM

	if (images.GetSetSize() != labels.GetSetSize())
		throw "Sizes do not match";

	std::vector<double> input;
	std::vector<double> results;

	CorectnessRate cr;
        Randomize<> rand;
        
        ImageProcessor<> imgProc;
        imgProc.inMax = std::numeric_limits<unsigned char>::max();

	NeuralNet network{ inputXSize * inputYSize,{ 15, 10 } };
	//NeuralNet network{ "logs/epoch_6_wbs" };

	std::unique_ptr<unsigned char> images_ptr = images.FetchAll();
	std::unique_ptr<unsigned char> labels_ptr = labels.FetchAll();

	std::cout << "Dataset is here." << std::endl;

	std::vector<bool> uses( images.GetSetSize(), false );

	const long stochasticSetSize = 10;

	long epoch = 0;
	double ni = 0.3;

	std::ofstream ofs{ "Log", std::ios::out };
	while (cr.ratio() < 0.995)
	{
		std::ostringstream oss;
		oss << "logs/ohitssobig_" << epoch << "_wbsab";
		
		network.saveToFile(oss.str());
		
		long useCount = 0;
		long i = 0;
		++epoch;

		ofs << ":::::::::::: EPOCH " << epoch << " :::::::::::::" << std::endl;
		std::cout << ":::::::::::: EPOCH " << epoch << " :::::::::::::" << std::endl;
                

		while (useCount < images.GetSetSize())
		{
			network.ResetLayers();

			ofs << "{ " << epoch << " } " << "Batch :: " << i << " :: ";
			std::cout << "{ " << epoch << " } " << "Batch :: " << i << " :: ";
                        
                        Matrix in{ inputXSize * inputYSize };
                        Matrix out{ 10 };
                        
                        std::vector<UINT> iko;

			for (int j{ 0 }; j < stochasticSetSize && useCount < images.GetSetSize(); ++j)
			{
                            rand.SetRangeInt(0, images.GetSetSize() - useCount - 1);
                            
                                long offset = rand.GetInt();
				long fOffset = 0;
				long index = 0;

				while (fOffset != offset)
				{
					if (!uses[index]) ++fOffset;
					++index;
				}

				uses[index] = true;
				useCount++;

				//MakeInputVector(input, images_ptr, 784, index * images.GetUnitSize());
				//MakeResultVector(results, *(labels_ptr.get() + index * labels.GetUnitSize()));
                                
                                imgProc.MakeInputVectorUNP(input, images_ptr, index * images.GetUnitSize());
				MakeResultVectorK<>(results, *(labels_ptr.get() + index * labels.GetUnitSize()));
                                
                                in.addRow(input);
                                out.addRow(results);


                                iko.push_back((int)(unsigned char)(*(labels_ptr.get() + index * labels.GetUnitSize())));
				/*std::cout << network.HighestActivationValue(j) << " ";
				ofs << network.HighestActivationValue(j) << " ";

				if (network.HighestActivationValue(j) == (int)(unsigned char)(*(labels_ptr.get() + index * labels.GetUnitSize())))
					cr.add(1);
				else cr.add(0);//*/

				results.clear();
				input.clear();
			}

                        
                        network.FeedFWD( in );
                        network.Backpropagate(out);
			network.Update(in, ni);
                        
                        std::vector<UINT> uko = network.TopValues();
                        for(int i{ 0 }; i < uko.size(); i++) {
                            std::cout << uko[i] << " ";
                            if (uko[i] == iko[i])
                                cr.add(1);
                            else cr.add(0);
                        }

			std::cout << " :: CR :: " << cr.ratio() << std::endl;
			ofs << " :: CR :: " << cr.ratio() << std::endl;

			++i;
		}
		
		
		ni *= 0.98;

		uses.clear();
		uses.insert(uses.end(), images.GetSetSize(), false);
	}
	
	network.saveToFile("maximized_wbs");
}

void smartMatrixTest()
{
    SmartMatrix sm{ 2, 2, false };
    SmartMatrix ms{ 2, 2, false };
    SmartMatrix gs{ 3, 2, false };
    SmartMatrix ss{ 3, 2, false };
    
    std::cout << sm << std::endl;
    std::cout << ms << std::endl;
    
    std::cout << sm * ms << std::endl;
    std::cout << sm + gs  << std::endl;
    std::cout << sm - gs  << std::endl;
}

