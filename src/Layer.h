#pragma once

#include "SmartMatrix.hpp"
#include <exception>

struct SigmaNanException : public std::exception {
    const char * what () const noexcept override { return "Computing sigma with NaN parameter"; }
};

template <class Real = double>
Real sigma(Real z)
{
        if (std::isnan(z)) throw new SigmaNanException ();
	return static_cast<Real>(1) / (static_cast<Real>(1) + std::exp(-z));
}

template <class Real = double>
Real sigmaPrime(Real z)
{
	return std::exp(z) / std::pow(std::exp(z) + static_cast<Real>(1), static_cast<Real>(2));
}

template <class Real>
class Layer
{
public:
        
        using Matrix = SmartMatrix<Real>;

	Layer(UINT in, UINT count);
	Layer(Real* data, UINT in, UINT count);

	void FeedFWD(const Matrix& input);
	void Backpropagate(const Matrix& expectation);
	void Update(const Matrix& input, Real ni);

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

template <class Real>
void Layer<Real>::FeedFWD(const Matrix& input)
{
	vOutput.concatenate(input * mWeights + Matrix{ input.size_m(), 1.0f } * vBiases);
	vSigmas.concatenate(vOutput ^ sigma<Real>);
}

template <class Real>
void Layer<Real>::Backpropagate(const Matrix& expectations)
{
	vErrors.concatenate(expectations & (vOutput ^ sigmaPrime<Real>));
}

template <class Real>
void Layer<Real>::Update(const Matrix& input, Real ni)
{
	mWeights -= input.T() * vErrors * ni; input.T();
	vBiases -= Matrix{ static_cast<Real>(1), vErrors.size_m () } * vErrors * ni;
}

template <class Real>
void Layer<Real>::ResetState()
{
	vOutput.reset();
	vSigmas.reset();
	vErrors.reset();
}

template <class Real>
Layer<Real>::Layer(UINT in, UINT count)
	: vBiases{ 1, count, false }, mWeights{ in, count, false }, vOutput{ count }, vSigmas{ count }, vErrors{ count }
{}

template <class Real>
Layer<Real>::Layer(Real* data, UINT in, UINT count)
	: vBiases{ data + in * count, 1, count }, mWeights{ data, in, count }, vOutput{ count }, vSigmas{ count }, vErrors{ count }
{}

template <class Real>
const typename Layer<Real>::Matrix& Layer<Real>::Output()
{
	return vOutput;
}

template <class Real>
const typename Layer<Real>::Matrix& Layer<Real>::Sigmas()
{
	return vSigmas;
}

template <class Real>
const typename Layer<Real>::Matrix& Layer<Real>::Weights()
{
	return mWeights;
}

template <class Real>
const typename Layer<Real>::Matrix& Layer<Real>::Errors()
{
	return vErrors;
}

template <class Real>
const typename Layer<Real>::Matrix& Layer<Real>::Biases()
{
	return vBiases; 
}
