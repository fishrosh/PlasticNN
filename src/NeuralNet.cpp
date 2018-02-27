#include "NeuralNet.h"
#include "FileLoader.h"

int NeuralNet::HighestActivationValue()
{
	int highest = -1;
	double top = 0.0;

	for (int m = 0; m < NNet.back().Sigmas().M; ++m)
	{
		for (int n = 0; n < NNet.back().Sigmas().N; ++n)
		{
			if (NNet.back().Sigmas().take(m, n) > top)
			{
				highest = n;
				top = NNet.back().Sigmas().take(m, n);
			}
		}
	}

	return highest;
}

int NeuralNet::HighestActivationValue(UINT j)
{
	int highest = -1;
	double top = 0.0;

	Matrix x = NNet.back().Sigmas(j);
	for (int m = 0; m < x.M; ++m)
	{
		for (int n = 0; n < x.N; ++n)
		{
			if (x.take(m, n) > top)
			{
				highest = n;
				top = x.take(m, n);
			}
		}
	}

	return highest;
}

NeuralNet::NeuralNet(UINT input, std::initializer_list<UINT> lrs)
{
	UINT sunt = input;
	sizes.push_back(sunt);
	for (UINT l : lrs)
	{
		NNet.push_back(Layer{ RMG, sunt, l });
		sunt = l;
		sizes.push_back(sunt);
	}
}

NeuralNet::NeuralNet(std::string fileName)
{
	FileLoader fl{ fileName };
	int layerCount;
	fl.Get32(&layerCount);
	
	int trash;
	fl.Get32(&trash);
	
	for (int i{ 0 }; i < layerCount; ++i)
	{
		int layerSize;
		fl.Get32(&layerSize);
		std::cout << layerSize << " ";
		this->sizes.push_back(layerSize);
		std::cout << std::endl;
	}

	for (int i{ 1 }; i < layerCount; ++i)
	{
		long numOfItems = sizes[i - 1] * sizes[i] + sizes[i];
		std::allocator<double> alloc;
		double* values = alloc.allocate(numOfItems);
		for (int j{ 0 }; j < numOfItems; ++j)
		{
			fl.Get64(&values[j]);
		}
		NNet.push_back(Layer{ values, sizes[i - 1], sizes[i] });
	}
}

void NeuralNet::FeedFWD(const Matrix& input)
{
	const Matrix* cInput = &input;
	for (Layer& l : NNet)
	{
		l.FeedFWD(*cInput);
		cInput = &l.Sigmas();
	}
}

void NeuralNet::Backpropagate(const Matrix& backput)
{
	std::allocator<Matrix> alloc;
	Matrix* backprop = alloc.allocate(1);
	alloc.construct(backprop, NNet.back().Sigmas() - backput);

	for (auto it = NNet.rbegin(); it != NNet.rend(); ++it)
	{
		(*it).Backpropagate(*backprop);
		alloc.destroy(backprop);
		alloc.construct(backprop, (*it).Errors() * (*it).Weights().T());
                (*it).Weights().T();
	}
	alloc.destroy(backprop);
}

void NeuralNet::Update(Matrix& input, double ni)
{
	Matrix* cInput = &input;
	for (Layer& l : NNet)
	{
		l.Update(*cInput, ni);
		cInput = &l.Sigmas();
	}
}

void NeuralNet::FeedFWD(const Matrix& input, UINT num)
{
	Matrix cInput = input;
	for (Layer& l : NNet)
	{
		l.FeedFWD(cInput);
		cInput = l.Sigmas(num);
	}
}

void NeuralNet::Backpropagate(const Matrix& backput, UINT num)
{
	std::allocator<Matrix> alloc;
	Matrix* backprop = alloc.allocate(1);
	alloc.construct(backprop, NNet.back().Sigmas(num) - backput);

	for (auto it = NNet.rbegin(); it != NNet.rend(); ++it)
	{
		(*it).Backpropagate(*backprop);
		alloc.destroy(backprop);
		alloc.construct(backprop, (*it).Errors(num) * (*it).Weights().T());
                (*it).Weights().T();
	}
	alloc.destroy(backprop);
}

void NeuralNet::ResetLayers()
{
	for (Layer& l : NNet)
	{
		l.ResetState();
	}
}

bool NeuralNet::saveToFile(std::string fileName)
{
	auto extractToFile = [](const Matrix& m, std::ofstream& ofs) {
		for (int i{ 0 }; i < m.M; ++i) {
			const std::vector<double>& r = m.extractRow(i);
			for (double d : r) {
				ofs.write(reinterpret_cast<char*>(&d), sizeof d);
			}
		}
	};

	auto rvalToFile = [](std::ostream& ofs, long d) {
		decltype(d) a{ d };
		ofs.write(reinterpret_cast<char*>(&a), sizeof a);
	};

	std::ofstream ofs{ fileName.c_str(), std::ios::out | std::ios::binary };
	if (ofs.is_open())
	{
		rvalToFile(ofs, sizes.size());
		for (auto a : sizes) {
			ofs.write(reinterpret_cast<char*>(&a), sizeof a);
		}

		for (Layer& l : NNet) {
			extractToFile(l.Weights(), ofs);
			extractToFile(l.Biases(), ofs);
		}
		return true;
	}

	else {
		std::cout << "Unexpected problem on out stream." << std::endl;
		return false;
	}
}

std::ostream& NeuralNet::Out(std::ostream& out) const
{
	for (Layer l : NNet)
	{
		out << std::endl << "::::::::: LAYER :::::::::" << std::endl;
		out << l.Weights() << std::endl;
		out << l.Biases() << std::endl;
		// out << l.Output() << std::endl;
		// out << l.Sigmas() << std::endl;
		// out << l.Errors() << std::endl;
	}
	return out;
}
