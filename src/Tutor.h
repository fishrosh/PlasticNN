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
#include "ArtificialIntelligence.h"
#include "DataTypes.h"

template <class Real = double, class input_t = unsigned char>
class Tutor {
    
public:
    
    void SDG (ArtificialIntelligence<>& ai, DatasetManager<Real, input_t>& dataset);
};

template <class Real, class input_t>
void Tutor <Real, input_t>::SDG(ArtificialIntelligence<>& ai, DatasetManager<Real, input_t>& dataset) {

    BatchData bd;
    
    ai.OnPreLearning();
    
    while (ai.ContinueLearningCondition())
    {
        ++bd.epoch;
        bd.batch = 0;
        
        ai.OnPreEpoch(bd.epoch);
        
        for (auto s : dataset) {
            ++bd.batch;
            
            ai.OnPreBatch(bd);
            
            ai.network->ResetLayers();
            ai.network->FeedFWD( s.in );
            ai.network->Backpropagate(s.out);
            ai.network->Update(s.in, ai.GetLearningRate());
            
            ai.OnPostBatch(s.out, bd);
        }

        ai.OnPostEpoch(bd.epoch);
    }

    ai.OnPostLearning();
}

#endif /* TUTOR_H */

