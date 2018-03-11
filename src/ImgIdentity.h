/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DP_Identity.h
 * Author: kamil
 *
 * Created on March 11, 2018, 8:29 PM
 */

#ifndef DP_IDENTITY_H
#define DP_IDENTITY_H

#include <limits>

template <class output_t = double, class input_t = unsigned char>
class ImgIdentity : public DataProcessor<output_t, input_t> {
public:
    
    using uint = unsigned int;
    
    uint inSizeX;
    uint inSizeY;
    
    void operator()(std::vector<output_t>& output, std::unique_ptr<input_t>& input, long offset) const override;
};

template <class output_t, class input_t>
void ImgIdentity<output_t, input_t>::operator ()(std::vector<output_t>& output, std::unique_ptr<input_t>& input, long offset) const {
    for (uint i = 0; i < inSizeX * inSizeY; ++i)
    {
            output_t o = static_cast<output_t>(*(input.get() + i + offset * inSizeX * inSizeY)) / std::numeric_limits<input_t>::max();
            output.push_back(o);
    }
}

#endif /* DP_IDENTITY_H */

