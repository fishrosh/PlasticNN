#pragma once

#include "Layer.h"
#include "FileLoader.h"

template <class Real = double>
class NeuralNet
{
public:
	using Layers = std::vector<Layer<Real>>;
        using Matrix = SmartMatrix<Real>;

	NeuralNet(UINT input, std::initializer_list<UINT> lrs);
	NeuralNet(std::string fileName);

	void FeedFWD(const Matrix& input);
	void Backpropagate(const Matrix& backput);
	void Update(Matrix& input, Real ni);

	void ResetLayers();
        
        const Matrix& GetNetworkOutput () { return NNet.back().GetLayerActivations (); }

	bool saveToFile(std::string fileName);

	std::ostream& Out(std::ostream& out) const;
        
private:

	std::vector<UINT> sizes;
	Layers NNet;
};

template <class Real>
NeuralNet<Real>::NeuralNet(UINT input, std::initializer_list<UINT> lrs)
{
	UINT sunt = input;
	sizes.push_back(sunt);
	for (UINT l : lrs)
	{
		NNet.push_back(Layer<Real>{ sunt, l });
		sunt = l;
		sizes.push_back(sunt);
	}
}

template <class Real>
NeuralNet<Real>::NeuralNet(std::string fileName)
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
		NNet.push_back(Layer<Real>{ values, sizes[i - 1], sizes[i] });
	}
}

template <class Real>
void NeuralNet<Real>::FeedFWD(const Matrix& input)
{
	const Matrix* cInput = &input;
	for (auto& l : NNet)
	{
		l.FeedFWD(*cInput);
		cInput = &l.Sigmas();
	}
}

template <class Real>
void NeuralNet<Real>::Backpropagate(const Matrix& backput)
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

template <class Real>
void NeuralNet<Real>::Update(Matrix& input, Real ni)
{
	Matrix const * cInput = &input;
	for (auto& l : NNet)
	{
		l.Update(*cInput, ni);
		cInput = &l.Sigmas();
	}
}

template <class Real>
void NeuralNet<Real>::ResetLayers()
{
	for (auto& l : NNet)
	{
		l.ResetState();
	}
}

template <class Real>
bool NeuralNet<Real>::saveToFile(std::string fileName)
{
	auto extractToFile = [](const Matrix& m, std::ofstream& ofs) {
		for (int i{ 0 }; i < m.RowCount(); ++i) {
			const std::vector<Real>& r = m.extractRow(i);
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

		for (auto& l : NNet) {
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

template <class Real>
std::ostream& NeuralNet<Real>::Out(std::ostream& out) const
{
	for (auto& l : NNet)
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