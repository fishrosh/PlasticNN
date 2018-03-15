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

RandomMatrixGenerator::RandomMatrixGenerator()
	: rd{}, genie{ rd() }
{}

Matrix RandomMatrixGenerator::operator()(UINT M, UINT N)
{
	Matrix output{ 1 };
	/* std::uniform_real_distribution<> distro{-1.0, 1.0};
	for (UINT u = 0; u < M; ++u)
	{
		output.push_back(Matrix::Row{});
		for (UINT y = 0; y < N; ++y)
		{
			output.back().push_back(distro(genie));
		}
	} */
	return output;
}

void Layer::FeedFWD(const Matrix& input)
{
	vOutput.concatenate(input * mWeights + Matrix{ input.M, 1.0f } * vBiases);
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

void Layer::Update(Matrix& input, double ni)
{
	mWeights -= input.T() * vErrors * ni; input.T();
	vBiases -= Matrix{ 1.0, vErrors.M } * vErrors * ni;
}

void Layer::ResetState()
{
	vOutput.reset();
	vSigmas.reset();
	vErrors.reset();
}

Layer::Layer(RandomMatrixGenerator& rmg, UINT in, UINT count)
	: vBiases{ 1, count, false }, mWeights{ in, count, false }, vOutput{ count }, vSigmas{ count }, vErrors{ count }
{}

Layer::Layer(double* data, UINT in, UINT count)
	: vBiases{ data + in * count, 1, count }, mWeights{ data, in, count }, vOutput{ count }, vSigmas{ count }, vErrors{ count }
{}

Matrix& Layer::Output()
{
	return vOutput;
}

Matrix& Layer::Sigmas()
{
	return vSigmas;
}

Matrix& Layer::Weights()
{
	return mWeights; // sure it's not copying ?
}

Matrix& Layer::Errors()
{
	return vErrors; // sure it's not copying ?
}

Matrix& Layer::Biases()
{
	return vBiases; // sure it's not copying ?
}

const Matrix Layer::Output(UINT rNum)
{
	return vOutput.matrixFromRow(rNum);
}

const Matrix Layer::Sigmas(UINT rNum)
{
	return vSigmas.matrixFromRow(rNum);
}

const Matrix Layer::Weights(UINT rNum)
{
	return mWeights.matrixFromRow(rNum); // sure it's not copying ?
}

const Matrix Layer::Errors(UINT rNum)
{
	return vErrors.matrixFromRow(rNum); // sure it's not copying ?
}

const Matrix Layer::Biases(UINT rNum)
{
	return vBiases.matrixFromRow(rNum); // sure it's not copying ?
}
