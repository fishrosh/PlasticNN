#pragma once

#include "Layer.h"

class NeuralNet
{
public:
	using Layers = std::vector<Layer>;
        using Matrix = SmartMatrix<double>;

	NeuralNet(UINT input, std::initializer_list<UINT> lrs);
	NeuralNet(std::string fileName);

	void FeedFWD(const Matrix& input);
	void Backpropagate(const Matrix& backput);
	void Update(Matrix& input, double ni);

	void ResetLayers();
        
        const Matrix& GetNetworkOutput () { return NNet.back().GetLayerActivations (); }

	bool saveToFile(std::string fileName);

	std::ostream& Out(std::ostream& out) const;
        
private:

	std::vector<UINT> sizes;
	Layers NNet;
};