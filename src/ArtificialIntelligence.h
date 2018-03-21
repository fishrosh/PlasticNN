/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ArtificialIntelligence.h
 * Author: kamil
 *
 * Created on March 15, 2018, 9:06 PM
 */

#ifndef ARTIFICIALINTELLIGENCE_H
#define ARTIFICIALINTELLIGENCE_H

#include <iomanip>
#include "DataTypes.h"
#include "NeuralNet.h"

template <class Real = double>
class ArtificialIntelligence {
    
    ErrorMeasure error;
    
public:
    
    using Matrix = SmartMatrix;
    using ulong = unsigned long;
    using uint = unsigned int;
    
    Real learning_rate;
    std::string folderPath;
    
    ArtificialIntelligence(uint input_size, std::initializer_list<uint> layer_sizes);
    
    const std::unique_ptr <NeuralNet> network;
    
    virtual void OnPreLearning ();
    virtual void OnPreEpoch (ulong epoch);
    virtual void OnPreBatch (const BatchData& bd);
    virtual void OnPostBatch (const Matrix& results, const BatchData& bd);
    virtual void OnPostEpoch (ulong epoch);
    virtual void OnPostLearning ();
    
    virtual bool ContinueLearningCondition () const;
    virtual Real GetLearningRate () const { return learning_rate; }
    
};

template <class Real>
ArtificialIntelligence<Real>::ArtificialIntelligence(uint input_size, std::initializer_list<uint> layer_sizes)
    : network {std::make_unique<NeuralNet> (input_size, layer_sizes)}
{
    error.SetProbeSize(30000);
}

template <class Real>
void ArtificialIntelligence<Real>::OnPreLearning() {
    // TODO
}

template <class Real>
void ArtificialIntelligence<Real>::OnPreEpoch(ulong epoch) {
    
    std::ostringstream oss;
    oss << folderPath << "_" << std::setfill('0') << std::setw(4) << epoch << ".nnet";
    network->saveToFile(oss.str());

    Log::ln (":::::::::::: EPOCH " + std::to_string(epoch) + " :::::::::::::");
}

template <class Real>
void ArtificialIntelligence<Real>::OnPreBatch(const BatchData& bd) {
    // TODO 
}

template <class Real>
void ArtificialIntelligence<Real>::OnPostBatch(const Matrix& labels, const BatchData& bd) {
    Log::i("{ " + std::to_string(bd.epoch) + " } " + "Batch :: " + std::to_string(bd.batch) + " :: ");
    
    error.Evaluate(network->GetNetworkOutput(), labels);
    std::vector<uint> results = Interpreter<>::Interpret(network->GetNetworkOutput());

    for (const auto& v : results) {
        Log::i(" " + std::to_string(v));
    }

    Log::ln(" :: CR :: " + std::to_string(error.GetHitRate()));
}

template <class Real>
void ArtificialIntelligence<Real>::OnPostEpoch(ulong epoch) {
    learning_rate *= 0.98;
}

template <class Real>
void ArtificialIntelligence<Real>::OnPostLearning() {
    network->saveToFile(folderPath + "_max.nnet");
}

template <class Real>
bool ArtificialIntelligence<Real>::ContinueLearningCondition() const {
    return error > 0.1f; 
}


#endif /* ARTIFICIALINTELLIGENCE_H */

