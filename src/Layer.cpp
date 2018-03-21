#include "Layer.h"

double sigma(double z)
{
        if (std::isnan(z)) std::cout << "klopotek";
	return 1.0 / (1.0 + std::exp(-z));
}

double sigmaPrime(double z)
{
	return std::exp(z) / std::pow(std::exp(z) + 1.0, 2.0);
}

void Layer::FeedFWD(const Matrix& input)
{
	vOutput.concatenate(input * mWeights + Matrix{ input.size_m(), 1.0f } * vBiases);
        //std::cout << std::endl << "input" << std::endl << input;
       // std::cout << std::endl << "weights" << std::endl << mWeights;
        //std::cout << std::endl << "output" << std::endl << vOutput;
	vSigmas.concatenate(vOutput ^ sigma);
}

void Layer::Backpropagate(const Matrix& expectations)
{
	vErrors.concatenate(expectations & (vOutput ^ sigmaPrime));
        // std::cout << std::endl << "error" << std::endl << vErrors;
}

void Layer::Update(const Matrix& input, double ni)
{
	mWeights -= input.T() * vErrors * ni; input.T();
	vBiases -= Matrix{ 1.0, vErrors.size_m () } * vErrors * ni;
}

void Layer::ResetState()
{
	vOutput.reset();
	vSigmas.reset();
	vErrors.reset();
}

Layer::Layer(UINT in, UINT count)
	: vBiases{ 1, count, false }, mWeights{ in, count, false }, vOutput{ count }, vSigmas{ count }, vErrors{ count }
{}

Layer::Layer(double* data, UINT in, UINT count)
	: vBiases{ data + in * count, 1, count }, mWeights{ data, in, count }, vOutput{ count }, vSigmas{ count }, vErrors{ count }
{}

const Layer::Matrix& Layer::Output()
{
	return vOutput;
}

const Layer::Matrix& Layer::Sigmas()
{
	return vSigmas;
}

const Layer::Matrix& Layer::Weights()
{
	return mWeights; // sure it's not copying ?
}

const Layer::Matrix& Layer::Errors()
{
	return vErrors; // sure it's not copying ?
}

const Layer::Matrix& Layer::Biases()
{
	return vBiases; // sure it's not copying ?
}
