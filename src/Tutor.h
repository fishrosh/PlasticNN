/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tutor.h
 * Author: kamil
 *
 * Created on March 4, 2018, 12:44 PM
 */

#ifndef TUTOR_H
#define TUTOR_H

#include <sstream>
#include <fstream>

#include "ITeachable.h"
#include "DatasetManager.h"
#include "SmartMatrix.h"
#include "ErrorMeasure.h"
#include "NeuralNet.h"

template <class Real = double, class input_t = unsigned char>
class Tutor {
    
public:
    
    void SDG (ITeachable<Real, input_t>&, DatasetManager<Real, input_t>& dataset);
};

template <class input_t = unsigned char, class output_t = double>
void MakeResultVectorK(std::vector<output_t>& output, input_t input)
{
	for (input_t c = 0; c < 10; ++c)
	{
		if (c == input) {
                    output.push_back(1.0);
                }
                
		else output.push_back(0.0);
	}
}

template <class Real, class input_t>
void Tutor <Real, input_t>::SDG(ITeachable<Real, input_t>& student, DatasetManager<Real, input_t>& dataset) {

    ErrorMeasure error;
    error.SetTrialLimit(30000);

    UINT inputXSize = 28;
    UINT inputYSize = 28;

	NeuralNet network{ inputXSize * inputYSize,{ 15, 10 } };
	//NeuralNet network{ "logs/epoch_6_wbs" };

	std::cout << "Dataset is here." << std::endl;

	const long batch_size = 10;

	long epoch = 0;
	double ni = 0.3;

	std::ofstream ofs{ "Log", std::ios::out };
	while (error < 0.995)
	{
		std::ostringstream oss;
		oss << "logs/ohitssobig_" << epoch << "_wbsab";
		
		network.saveToFile(oss.str());
		long i = 0;
		++epoch;

		ofs << ":::::::::::: EPOCH " << epoch << " :::::::::::::" << std::endl;
		std::cout << ":::::::::::: EPOCH " << epoch << " :::::::::::::" << std::endl;
                
                Matrix in{ inputXSize * inputYSize };
                Matrix out{ 10 };
                std::vector<UINT> iko;
                        
                for (auto s : dataset) {
                    std::vector<Real> result;
                    MakeResultVectorK<>(result, s.result);
                    
                    in.addRow(s.input);
                    out.addRow(result);
                    
                    iko.push_back(5);
                    
                    if (in.M == batch_size) {
                        
			network.ResetLayers();
                        network.FeedFWD( in );
                        network.Backpropagate(out);
			network.Update(in, ni);
                        
                        in.reset();
                        out.reset();
                        
                        ofs << "{ " << epoch << " } " << "Batch :: " << i << " :: ";
			std::cout << "{ " << epoch << " } " << "Batch :: " << i << " :: ";
                        
                        std::vector<UINT> uko = network.TopValues();
                        for(int i{ 0 }; i < uko.size(); i++) {
                            std::cout << uko[i] << " ";
                            if (uko[i] == iko[i])
                                error.AddSuccess();
                            else error.AddFailure();
                        }
                        
                        iko.clear();

			std::cout << " :: CR :: " << error.GetHitRate() << std::endl;
			ofs << " :: CR :: " << error.GetHitRate() << std::endl;

			++i;
                    }
                }
                
		ni *= 0.98;
	}
	
	network.saveToFile("maximized_wbs");
}

#endif /* TUTOR_H */

