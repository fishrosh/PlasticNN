/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DataProcessor.h
 * Author: kamil
 *
 * Created on March 11, 2018, 8:22 PM
 */

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <vector>
#include <memory>

template <class output_t = double, class input_t = unsigned char>
class DataProcessor {
public:
    
    virtual void operator()(std::vector<output_t>& output, std::unique_ptr<input_t>& input, long offset) const = 0;
    virtual long GetOutputLength () const = 0;
}; 

#endif /* DATAPROCESSOR_H */

