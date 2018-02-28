/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ImageProcessor.h
 * Author: kamil
 *
 * Created on February 27, 2018, 9:02 PM
 */

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "Randomize.h"

#ifndef UINT
#define UINT unsigned int
#endif

template <class input_t = unsigned char, class output_t = double>
class ImageProcessor {
    
    UINT midSizeX = 28;
    UINT midSizeY = 28;
    
public:
    
    UINT outSizeX = 28;
    UINT outSizeY = 28;
    
    UINT inSizeX = 28;
    UINT inSizeY = 28;
    
    input_t inMax;
    input_t inMin;
    
    void MakeInputVector(std::vector<output_t>& output, std::unique_ptr<input_t>& input, long offset);
    void MakeInputVectorUNP(std::vector<output_t>& output, std::unique_ptr<input_t>& input, long offset);
    
private:
        
    output_t ReadInputImage(std::unique_ptr<input_t>& input, UINT i, UINT j, long offset);
      
};

template <class input_t, class output_t>
void ImageProcessor<input_t, output_t>::MakeInputVector(std::vector<output_t>& output, std::unique_ptr<input_t>& input, long offset)
{
    Randomize<> r;
    
    r.SetRangeReal(0.0, 0.1);
    r.SetRangeInt(0, 14);
    
    UINT sizeMod = r.GetInt();
    
    midSizeX = 21 + sizeMod;
    midSizeY = 21 + sizeMod;
    
    r.SetRangeInt(0, outSizeX - midSizeX -1);
    UINT offsetX = r.GetInt();
    
    r.SetRangeInt(0, outSizeY - midSizeY -1);
    UINT offsetY = r.GetInt();
    
    for (int i{ 0 }; i++ < outSizeX;)
    {
        for (int j{ 0 }; j++ < outSizeY;)
        {
            double o = 0.0f;
            
            if (i >= offsetX && i < offsetX + midSizeX &&
                    j >= offsetY && j < offsetY + midSizeY ) 
            {
                o = ReadInputImage(input, (i - offsetX), (j - offsetY), offset);
            }
            
            else {
                o = r.GetReal();
            }
            
            output.push_back(o);
        }
    }
}

// !!!!!!!!!!!!!!
// CHANGE MNISTLOADER to unsigned char
template <class input_t, class output_t>
void ImageProcessor<input_t, output_t>::MakeInputVectorUNP(std::vector<output_t>& output, std::unique_ptr<input_t>& input, long offset)
{
	for (UINT i = 0; i < inSizeX * inSizeY; ++i)
	{
		double o = static_cast<output_t>(*(input.get() + i + offset)) / inMax;
		output.push_back(o);
	}
}

template <class input_t, class output_t>
output_t ImageProcessor<input_t, output_t>::ReadInputImage(std::unique_ptr<input_t>& input, UINT i, UINT j, long offset)
{
    UINT finalI = static_cast<UINT>(std::floor( i / static_cast<double>(midSizeX) * inSizeX));
    UINT finalJ = static_cast<UINT>(std::floor( j / static_cast<double>(midSizeY) * inSizeY));
    
    return static_cast<output_t>(*(input.get() + finalI * inSizeY + finalJ + offset)) / inMax;
}

#endif /* IMAGEPROCESSOR_H */

