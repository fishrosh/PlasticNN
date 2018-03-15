/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tutor.h
 * Author: kamil
 *
 * Created on March 4, 2018, 12:44 PM
 */

#ifndef TUTOR_H
#define TUTOR_H

#include <sstream>
#include <fstream>
#include <string>

#include "ITeachable.h"
#include "DatasetManager.h"
#include "SmartMatrix.h"
#include "ErrorMeasure.h"
#include "NeuralNet.h"
#include "Log.h"
#include "Interpreter.h"

template <class Real = double, class input_t = unsigned char>
class Tutor {
    
public:
    
    void SDG (ITeachable<Real, input_t>&, DatasetManager<Real, input_t>& dataset);
};

template <class Real, class input_t>
void Tutor <Real, input_t>::SDG(ITeachable<Real, input_t>& student, DatasetManager<Real, input_t>& dataset) {

    ErrorMeasure error;
    error.SetTrialLimit(30000);

    UINT inputXSize = 28;
    UINT inputYSize = 28;

    NeuralNet network{ inputXSize * inputYSize,{ 15, 10 } };
    //NeuralNet network{ "logs/epoch_6_wbs" };

    long epoch = 0;
    double ni = 0.3;
    while (error < 0.995)
    {
        std::ostringstream oss;
        oss << "logs/ohitssobig_" << epoch << "_wbsab";
        network.saveToFile(oss.str());
        
        ++epoch;

        Log::ln (":::::::::::: EPOCH " + std::to_string(epoch) + " :::::::::::::");
        ulong i = 0;
        for (auto s : dataset) {
            network.ResetLayers();
            network.FeedFWD( s.in );
            network.Backpropagate(s.out);
            network.Update(s.in, ni);

            Log::i("{ " + std::to_string(epoch) + " } " + "Batch :: " + std::to_string(++i) + " :: ");
            
            error.Evaluate(network.GetNetworkOutput(), s.out);
            std::vector<uint> results = Interpreter<>::Interpret(network.GetNetworkOutput());
            
            for (const auto& v : results) {
                Log::i(" " + std::to_string(v));
            }

            Log::ln(" :: CR :: " + std::to_string(error.GetHitRate()));
        }

        ni *= 0.98;
    }

    network.saveToFile("maximized_wbs");
}

#endif /* TUTOR_H */

