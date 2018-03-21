#pragma once

#include "SmartMatrix.h"


class Layer
{
public:
        
        using Matrix = SmartMatrix<double>;

	Layer(UINT in, UINT count);
	Layer(double* data, UINT in, UINT count);

	void FeedFWD(const Matrix& input);
	void Backpropagate(const Matrix& expectation);
	void Update(const Matrix& input, double ni);

	void ResetState();
        
        const Matrix& GetLayerActivations () const { return vSigmas; }

	const Matrix& Errors();
	const Matrix& Weights();
	const Matrix& Output();
	const Matrix& Sigmas();
	const Matrix& Biases();
        
private:
        
	Matrix vBiases;
	Matrix mWeights;

	Matrix vErrors;
	Matrix vOutput;
	Matrix vSigmas;
};