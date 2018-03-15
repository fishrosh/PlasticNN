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
#include "Tutor.h"
#include "ITeachable.h"
#include "DatasetManager.h"
#include "MNIST.h"
#include "Log.h"
#include "ImgIdentity.h"
#include "LabelProcessor.h"
#define LINUX_PLATFORM

extern void MNISTStochasticRun ();

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    Log::BindInstance(std::make_shared<Log> ());
    
    Tutor<> tutor;
    
    #ifndef LINUX_PLATFORM
	MNISTLoader images("C:\\Users\\Kamil\\Downloads\\biernaty\\mnist\\train-images.idx3-ubyte");
	MNISTLoader labels("C:\\Users\\Kamil\\Downloads\\biernaty\\mnist\\train-labels.idx1-ubyte");
    #else
	MNISTLoader images("/home/kamil/train-images.idx3-ubyte");
	MNISTLoader labels("/home/kamil/train-labels.idx1-ubyte");
    #endif // !LINUX_PLATFORM

     ITeachable<> teachable;
     DatasetManager<> dataset;
     
     std::shared_ptr<ImgIdentity<>> imgProc = std::make_shared <ImgIdentity<>> ();
     imgProc->inSizeX = 28;
     imgProc->inSizeY = 28;
     dataset.LinkImgProc(imgProc);
     
     std::shared_ptr<LabelProcessor<>> labelProc = std::make_shared <LabelProcessor<>> ();
     dataset.LinkLabelProc(labelProc);
     //imgProc->inMax = 255;
     
     dataset.LoadDataset(images, labels);
     
     tutor.SDG(teachable, dataset);//*/
    
   // MNISTStochasticRun ();

    return 0;
}

