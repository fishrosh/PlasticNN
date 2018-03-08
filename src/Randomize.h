/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Randomize.h
 * Author: kamil
 *
 * Created on February 25, 2018, 1:27 PM
 */
#pragma once

#ifndef RANDOMIZE_H
#define RANDOMIZE_H

#include <random>

template <class Real = double, class Int = int>
class Randomize
{
    std::random_device rd;
    std::mt19937 generator;
    
    std::uniform_real_distribution<Real> distribution_real;
    std::uniform_int_distribution<Int> distribution_integer;
    
public:
    
    Randomize();
    
    void SetRangeReal(Real min, Real max);
    void SetRangeInt(Int min, Int max);
    
    Real GetReal ();
    Int GetInt ();
};

/*
 * 
 * CONSTRUCTORS
 * 
 */

template <class Real, class Int>
Randomize<Real, Int>::Randomize()
    :   
    rd{}, 
    generator{ rd() }, 
    distribution_real{ static_cast<Real>(-1.0), static_cast<Real>(1.0) }, 
    distribution_integer{ static_cast<Int>(0), static_cast<Int>(1) }
{}
    
/*
 * 
 * PUBLIC METHODS
 * 
 */
    
template <class Real, class Int>
void Randomize<Real, Int>::SetRangeReal(Real min, Real max)
{
    distribution_real = std::uniform_real_distribution<>{ min, max };
}

template <class Real, class Int>
void Randomize<Real, Int>::SetRangeInt(Int min, Int max)
{
    distribution_integer = std::uniform_int_distribution<>{ min, max };
}

template <class Real, class Int>
Real Randomize<Real, Int>::GetReal()
{
    return distribution_real(generator);
}

template <class Real, class Int>
Int Randomize<Real, Int>::GetInt()
{
    return distribution_integer(generator);
}

#endif /* RANDOMIZE_H */

