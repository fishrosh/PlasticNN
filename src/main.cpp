/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: kamil
 *
 * Created on February 27, 2018, 8:20 PM
 */

#include <cstdlib>
#include <memory>
#include <thread>
#include <chrono>
#include "Tutor.h"
#include "ITeachable.h"
#include "DatasetManager.h"
#include "MNIST.h"
#include "Log.h"
#include "ImgIdentity.h"
#include "LabelProcessor.h"
#include "ArtificialIntelligence.h"
#define LINUX_PLATFORM

extern void MNISTStochasticRun ();

using namespace std;

template <class Real, class input_t>
void Program () {

    Tutor<Real, input_t> tutor;

    #ifndef LINUX_PLATFORM
        MNISTLoader images("C:\\Users\\Kamil\\Downloads\\biernaty\\mnist\\train-images.idx3-ubyte");
        MNISTLoader labels("C:\\Users\\Kamil\\Downloads\\biernaty\\mnist\\train-labels.idx1-ubyte");
    #else
        MNISTLoader images("/home/kamil/train-images.idx3-ubyte");
        MNISTLoader labels("/home/kamil/train-labels.idx1-ubyte");
    #endif 

     DatasetManager<Real, input_t> dataset;

     std::shared_ptr<ImgIdentity<Real, input_t>> imgProc = std::make_shared <ImgIdentity<Real, input_t>> ();
     imgProc->inSizeX = 28;
     imgProc->inSizeY = 28;
     dataset.LinkImgProc(imgProc);

     std::shared_ptr<LabelProcessor<Real, input_t>> labelProc = std::make_shared <LabelProcessor<Real, input_t>> ();
     dataset.LinkLabelProc(labelProc);

     dataset.LoadDataset(images, labels);

    UINT inputXSize = 28;
    UINT inputYSize = 28;

    ArtificialIntelligence<Real> ai {inputXSize * inputYSize,{ 15, 10 }};
    
    ai.learning_rate = 0.3;
    ai.folderPath = "logs/mnist";
    
    std::this_thread::sleep_for(1s);

     tutor.SDG(ai, dataset);
}

/*
 * 
 */
int main(int argc, char** argv) {
    
    
    Log::BindInstance(std::make_shared<Log> ());
    Program <float, unsigned char> ();

    return 0;
}

