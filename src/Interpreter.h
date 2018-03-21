/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Interpreter.h
 * Author: kamil
 *
 * Created on March 15, 2018, 7:47 PM
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

template <class network_t = double, class results_t = unsigned int>
class Interpreter {
public:
    
    using Matrix = SmartMatrix<network_t>;
    
    static std::vector<results_t> Interpret(const Matrix& network_output);
};

template <class network_t, class results_t>
std::vector<results_t> Interpreter<network_t, results_t>::Interpret(const Matrix& network_output) {
    std::vector<results_t> output;
    for (ulong i = 0; i < network_output.RowCount (); ++i) {
        auto& output_row = network_output.extractRow(i);
        auto out_max = std::max_element (output_row.begin(), output_row.end());
        output.push_back (std::distance (output_row.begin(), out_max));
    }
    
    return output;
}

#endif /* INTERPRETER_H */

