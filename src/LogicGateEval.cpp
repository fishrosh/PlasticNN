/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "LogicGateEval.h"


int LogicGateTest::value() {
	return  !(a&&b) && (c || !d) ? 0 : 1;
	//return a && b || c;
	//return 0;
}

LogicGateTest::LogicGateTest()
	: rd{}, genie{ rd() }, distro(0, 1)
{
	randomize();
}

void LogicGateTest::randomize()
{
	a = distro(genie);
	b = distro(genie);
	c = distro(genie);
	d = distro(genie);
}

std::vector<double> LogicGateTest::GenerateInputVector()
{
	return std::vector<double>{ static_cast<double>(a), 
		static_cast<double>(b), 
		static_cast<double>(c), 
		static_cast<double>(d) };
}

std::vector<double> LogicGateTest::GenerateOutputVector()
{
	bool result = !(a&&b)&&(c || !d);
	return{ static_cast<double>(result), static_cast<double>(!result) };
}

