#pragma once

#include "Layer.h"

class NeuralNet
{
	using Layers = std::vector<Layer>;

	std::vector<UINT> sizes;

	RandomMatrixGenerator RMG;
	Layers NNet;

public:

	NeuralNet(UINT input, std::initializer_list<UINT> lrs);
	NeuralNet(std::string fileName);

	void FeedFWD(const Matrix& input);
	void Backpropagate(const Matrix& backput);
	void Update(Matrix& input, double ni);

	void FeedFWD(const Matrix& input, UINT num);
	void Backpropagate(const Matrix& backput, UINT num);
	void Update(const Matrix& input, double ni, UINT num);

	void ResetLayers();

	int HighestActivationValue();
	int HighestActivationValue(UINT j);
        
        std::vector<UINT> TopValues() { return NNet.back().TopValues(); }

	bool saveToFile(std::string fileName);

	std::ostream& Out(std::ostream& out) const;
};