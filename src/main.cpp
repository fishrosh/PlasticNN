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
#include "Dumbed.h"
#include "Tutor.h"
#include "ITeachable.h"
#include "DatasetManager.h"
#include "MNIST.h"
#define LINUX_PLATFORM

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    /*Tutor<> tutor;
    
    #ifndef LINUX_PLATFORM
	MNISTLoader images("C:\\Users\\Kamil\\Downloads\\biernaty\\mnist\\train-images.idx3-ubyte");
	MNISTLoader labels("C:\\Users\\Kamil\\Downloads\\biernaty\\mnist\\train-labels.idx1-ubyte");
    #else
	MNISTLoader images("/home/kamil/train-images.idx3-ubyte");
	MNISTLoader labels("/home/kamil/train-labels.idx1-ubyte");
    #endif // !LINUX_PLATFORM

     ITeachable<> teachable;
     DatasetManager<> dataset;
     
     std::shared_ptr<ImageProcessor<>> imgProc = std::make_shared <ImageProcessor<>> ();
     
     dataset.LoadDataset(images, labels);
     dataset.LinkImgProc(imgProc);
     
     tutor.SDG(teachable, dataset);*/
    
    MNISTStochasticRun ();

    return 0;
}

