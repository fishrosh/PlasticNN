#pragma once

#include "SmartMatrix.h"

using Matrix = SmartMatrix;

class RandomMatrixGenerator
{
	std::random_device rd;
	std::mt19937 genie;

public:

	RandomMatrixGenerator();
	Matrix operator()(UINT M, UINT N);
};

class Layer
{
	Matrix vBiases;
	Matrix mWeights;

	Matrix vErrors;
	Matrix vOutput;
	Matrix vSigmas;

public:

	Layer(RandomMatrixGenerator& rmg, UINT in, UINT count);
	Layer(double* data, UINT in, UINT count);

	void FeedFWD(const Matrix& input);
	void Backpropagate(const Matrix& expectation);
	void Update(Matrix& input, double ni);

	void ResetState();
        
        const Matrix& GetLayerActivations () { return vSigmas; }

	Matrix& Errors();
	Matrix& Weights();
	Matrix& Output();
	Matrix& Sigmas();
	Matrix& Biases();

	const Matrix Errors(UINT rNum);
	const Matrix Weights(UINT rNum);
	const Matrix Output(UINT rNum);
	const Matrix Sigmas(UINT rNum);
	const Matrix Biases(UINT rNum);
};