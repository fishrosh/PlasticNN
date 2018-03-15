/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LabelProcessor.h
 * Author: kamil
 *
 * Created on March 11, 2018, 8:45 PM
 */

#ifndef LABELPROCESSOR_H
#define LABELPROCESSOR_H

template <class output_t = double, class input_t = unsigned char>
class LabelProcessor : public DataProcessor<output_t, input_t> {
public:
    
    using uint = unsigned int;
    
    void operator()(std::vector<output_t>& output, std::unique_ptr<input_t>& input, long offset) const override;
    long GetOutputLength () const override;
};

template <class output_t, class input_t>
void LabelProcessor<output_t, input_t>::operator ()(std::vector<output_t>& output, std::unique_ptr<input_t>& input, long offset) const {
    input_t label = (*(input.get() + offset));
    
    // !!!!!!!!!!! work on it
    for (input_t c = 0; c < 10; ++c)
	{
		if (c == label) {
                    output.push_back(1.0);
                }
                
		else output.push_back(0.0);
	}
}

template <class output_t, class input_t>
long LabelProcessor<output_t, input_t>::GetOutputLength() const {
    return 10;
}

#endif /* LABELPROCESSOR_H */

