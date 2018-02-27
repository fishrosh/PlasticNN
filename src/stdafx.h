// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#ifndef LINUX_PLATFORM
#include <stdio.h>
#include <tchar.h>
#endif

#include <vector>
#include <iostream>
#include <random>
#include <memory>
#include <queue>
#include <iomanip>
#include <sstream>


// TODO: reference additional headers your program requires here
//#include "Matrix.h"
#include "MNIST.h"
#include "Layer.h"
#include "NeuralNet.h"
#include "FileLoader.h"
#include "SmartMatrix.h"
