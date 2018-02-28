/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LogicGateEval.h
 * Author: kamil
 *
 * Created on February 27, 2018, 8:47 PM
 */

#ifndef LOGICGATEEVAL_H
#define LOGICGATEEVAL_H

#include <random>
#include <vector>

class LogicGateTest
{
	bool a, b, c, d;

	std::random_device rd;
	std::mt19937 genie;
	std::uniform_int_distribution<> distro;

public:

	LogicGateTest();

	void randomize();

	std::vector<double> GenerateInputVector();
	std::vector<double> GenerateOutputVector();

	int value();
};

#endif /* LOGICGATEEVAL_H */

