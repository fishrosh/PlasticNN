/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SmartMatrix.hpp
 * Author: kamil
 *
 * Created on March 25, 2018, 3:53 PM
 */

#ifndef SMARTMATRIX_HPP
#define SMARTMATRIX_HPP

#include "SmartMatrix.h"
#include "MultithreadMultiplicator.h"

/*
 * 
 * 
 * CONSTRUCTORS
 * 
 * 
 */

template <class Real>
SmartMatrix<Real>::SmartMatrix(UINT n)
	: N{ n }, M{ 0 }, dataset{ std::make_unique<std::vector<Row>>() }
{}

template <class Real>
SmartMatrix<Real>::SmartMatrix(UINT m, UINT n, bool zeroed)
	: N{ n }, M{ m }, dataset{ std::make_unique<std::vector<Row>>() }
{
        Randomize<> r;
        
	auto lambda = [&r](UINT a, UINT b)->Real { return r.GetReal(); };
        auto zlambda = [&r](UINT a, UINT b)->Real { return 0.0; };
        
	if (zeroed) fill_each(zlambda);
        else fill_each(lambda);
}
        
template <class Real>
SmartMatrix<Real>::SmartMatrix(Real val, unsigned int n)
    :   N{ n }, M{ 1 }, dataset{ std::make_unique<std::vector<Row>>() }
{
        dataset->push_back( std::make_unique<std::vector<Real>>( n, val ));
}
    
template <class Real>
SmartMatrix<Real>::SmartMatrix(Real* data, unsigned int m, unsigned int n)
{}
    
template <class Real>
SmartMatrix<Real>::SmartMatrix(unsigned int m, Real val)
    :   N{ 1 }, M{ m }, dataset{ std::make_unique<std::vector<Row>>() }
{
        for (int i{ 0 }; i++ < m;) dataset->push_back( std::make_unique<std::vector<Real>>( 1, val ));
}

template <class Real>
SmartMatrix<Real>::SmartMatrix(Row r)
	: N{ ( UINT )r->size() }, M{ 1 }, dataset{ std::make_unique<std::vector<Row>>() }
{
	dataset->push_back(std::move(r));
}

template <class Real>
SmartMatrix<Real>::SmartMatrix(DataSet d) 
    : 
    N{ ( UINT )d->size() > 0 ? ( UINT )( *d )[0]->size() : 0 }, 
    M{ ( UINT )d->size() }, dataset{ std::move(d) }
{
}

template <class Real>
SmartMatrix<Real>::SmartMatrix(const Matrix& other)
	: N{ other.N }, M{ other.M }, dataset{ std::make_unique<std::vector<Row>>() }, transposition_flag{ other.transposition_flag }
{
	deep_cpy(other);
} 

template <class Real>
SmartMatrix<Real>::SmartMatrix(Matrix&& other)
	: N{ other.N }, M{ other.M }, dataset{ std::move(other.dataset) }, transposition_flag{ other.transposition_flag }
{}

template <class Real>
SmartMatrix<Real>::SmartMatrix(XMatrix<Real> other)
	: N{ other->N }, M{ other->M }, dataset{ std::move(other->dataset) }, transposition_flag{ other->transposition_flag }
{}
        
/*
 * 
 * 
 * ASSIGNMENT AND MOVE
 * 
 * 
 */

template <class Real>        
SmartMatrix<Real>& SmartMatrix<Real>::operator=(const Matrix& other)
{
	if (&other != this)
	{
		M = other.M; 
		N = other.N;
                
                transposition_flag = other.transposition_flag;

		(*this).dataset->clear();
		(*this).deep_cpy(other);
	}
	return *this;
}

template <class Real>        
SmartMatrix<Real>& SmartMatrix<Real>::operator=(Matrix&& other)
{
	if (&other != this)
	{
		M = other.M;
		N = other.N;
                
                transposition_flag = other.transposition_flag;

		(*this).dataset->clear();
		(*this).dataset = std::move(other.dataset);
	}
	return *this;
}

template <class Real>        
SmartMatrix<Real>& SmartMatrix<Real>::operator=(XMatrix<Real> other)
{
	M = other->M;
	N = other->N;

	transposition_flag = other->transposition_flag;

	(*this).dataset->clear();
	(*this).dataset = std::move(other->dataset);

	return *this;
}

/*
 * 
 * 
 * OPERATORS
 * 
 * 
 */

template <class Real>
XMatrix<Real> SmartMatrix<Real>::operator *(const Matrix& other) const
{
    if (this->size_n() != other.size_m()) throw MultiplySizeException {};
    
    XMatrix<Real> output = std::make_unique<Matrix>( this->size_m (), other.size_n(), true );
    
    Multiplicator<Real>::Instance().Multiply(this, &other, output.get());
    
    return output;
}

template <class Real>
SmartMatrix<Real>& SmartMatrix<Real>::operator *=(const Real g)
{
    auto lambda = [g](Real& a) { a *= g; };
    for_each(lambda);
    return *this;
}

template <class Real>
XMatrix<Real> SmartMatrix<Real>::operator^(std::function<Real(Real)> f) const
{
	XMatrix<Real> output = std::make_unique<Matrix>(*this);
	*output ^= f;
	return output;
}

/* ********************** */
/* BINARY MATRIX x MATRIX */
/* ********************** */

template <class Real>
XMatrix<Real> SmartMatrix<Real>::operator +(const Matrix& other) const
{
    XMatrix<Real> output = std::make_unique<Matrix>( *this );
    *output += other;
    return output;
}

template <class Real>
XMatrix<Real> SmartMatrix<Real>::operator -(const Matrix& other) const
{
    XMatrix<Real> output = std::make_unique<Matrix>( *this );
    *output -= other;
    return output;
}

template <class Real>
XMatrix<Real> SmartMatrix<Real>::operator &(const Matrix& other) const
{
    XMatrix<Real> output = std::make_unique<Matrix>(*this);
    *output &= other;
    return output;
}

/* *********************** */
/* BINARY MATRIX x XMATRIX */
/* *********************** */

template <class Real>
XMatrix<Real> SmartMatrix<Real>::operator +(XMatrix<Real> other) const
{
    *other += *this;
    return other;
}

template <class Real>
XMatrix<Real> SmartMatrix<Real>::operator -(XMatrix<Real> other) const
{
    *other -= *this;
	*other *= -1.0;
    return other;
}

template <class Real>
XMatrix<Real> SmartMatrix<Real>::operator &(XMatrix<Real> other) const
{
	*other &= *this;
	return other;
}

/* *************************** */
/* ASSIGNMENT MATRIX to MATRIX */
/* *************************** */

template <class Real>
SmartMatrix<Real>& SmartMatrix<Real>::operator +=(const Matrix& other)
{
    auto lambda = [](Real& a, Real b) { a += b; };
    for_each(lambda, other);
    return *this;
}

template <class Real>
SmartMatrix<Real>& SmartMatrix<Real>::operator -=(const Matrix& other)
{
    auto lambda = [](Real& a, Real b) { a -= b; };
    for_each(lambda, other);
    return *this;
}

template <class Real>
SmartMatrix<Real>& SmartMatrix<Real>::operator &=(const Matrix& other)
{
	auto lambda = [](Real& a, Real b) { a *= b; };
	for_each(lambda, other);
	return *this;
}

template <class Real>
SmartMatrix<Real>& SmartMatrix<Real>::operator ^=(std::function<Real(Real)> f)
{
	auto lambda = [&f](Real& a) { a = f(a); };
	for_each(lambda);
	return *this;
}

/*
 * 
 * 
 * 
 * METHODS
 * 
 * 
 * 
 */

template <class Real>
std::ostream& SmartMatrix<Real>::Out(std::ostream& out) const
{
	for (auto& r : *dataset)
	{
		out << std::endl;
                for (auto& d : *r)
                {
                    out << d << " ";
                }
	}

	return out;
}

template <class Real>
void SmartMatrix<Real>::concatenate(const Matrix& other)
{
        if (this->size_n() != other.size_n()) throw ConcatenateSizeException {};

        deep_cpy(other);
        M += other.M;
}

template <class Real>
void SmartMatrix<Real>::concatenate(XMatrix<Real> other)
{
        if (this->size_n() != other->size_n()) throw ConcatenateSizeException {};

        for (auto& r : *other->dataset)
        {
            this->dataset->push_back( std::move(r));
        }

        M += other->M;
}

template <class Real>
void SmartMatrix<Real>::concatenate(const std::vector<Real>& v)
{
    if (v.size() != size_n()) throw ConcatenateSizeException {};
    
    Row copy = std::make_unique<std::vector<Real>>();
    std::copy(v.begin(), v.end(), std::back_inserter(*copy));
    (*this).dataset->push_back(std::move(copy));

    if (transposition_flag) ++N;
    else ++M;
}

template <class Real>
const std::vector<Real>& SmartMatrix<Real>::extractRow(UINT j) const
{
	return *( ( *this->dataset )[j].get() );
}

template <class Real>
void SmartMatrix<Real>::reset()
{
	dataset->clear();
	M = 0;
	transposition_flag = false;
}

template <class Real>
Real& SmartMatrix<Real>::at(UINT i, UINT j) 
{
    return !transposition_flag ? (*(( *dataset )[i]))[j] : (*(( *dataset )[j]))[i];
}

template <class Real>
const Real& SmartMatrix<Real>::at(UINT i, UINT j) const 
{
    return !transposition_flag ? (*(( *dataset )[i]))[j] : (*(( *dataset )[j]))[i];
}

template <class Real>
Real SmartMatrix<Real>::take(UINT i, UINT j) const
{
    return !transposition_flag ? (*(( *dataset )[i]))[j] : (*(( *dataset )[j]))[i];
}

template <class Real>
UINT SmartMatrix<Real>::size_m() const
{
    return this->transposition_flag ? N : M;
}

template <class Real>
UINT SmartMatrix<Real>::size_n() const
{
    return this->transposition_flag ? M : N;
}

template <class Real>
const SmartMatrix<Real>& SmartMatrix<Real>::T() const
{
    transposition_flag = !transposition_flag;
    return *this;
}

/*
 * 
 * 
 * HELPERS
 * 
 * 
 */

template <class Real>
void SmartMatrix<Real>::deep_cpy(const Matrix& other)
{
        // std::cout << "WARNING! deep copy" << std::endl;
	for (auto& r : *other.dataset)
	{
		Row copy = std::make_unique<std::vector<Real>>();
		std::copy(r->begin(), r->end(), std::back_inserter(*copy));
		(*this).dataset->push_back(std::move(copy));
	}
}

template <class Real>
void SmartMatrix<Real>::fill_each(std::function<Real(UINT, UINT)> f)
{
	if (dataset->empty())
	{
		for (UINT i{ 0 }; i < M; ++i)
		{
			Row r = std::make_unique<std::vector<Real>>();
			for (UINT j{ 0 }; j < N; ++j)
			{
				(*r).push_back(f(i, j));
			}
			dataset->push_back(std::move(r));
		}
	}
}

template <class Real>
void SmartMatrix<Real>::fill_each(std::function<Real()> f, int a)
{
	if (dataset->empty())
	{
		for (UINT i{ 0 }; i < M; ++i)
		{
			Row r = std::make_unique<std::vector<Real>>();
			for (UINT j{ 0 }; j < N; ++j)
			{
				(*r).push_back(f());
			}
			dataset->push_back(std::move(r));
		}
	}
}

template <class Real>
void SmartMatrix<Real>::for_each(std::function<void(Real&, Real)> f, const Matrix& other)
{
    if (this->M != other.M || this->N != other.N) throw MultiplySizeException {};
    
    for (UINT i{ 0 }; i < other.size_m(); ++i) 
    {
        for (UINT j{ 0 }; j < other.size_n(); ++j) 
        {
            f(this->at(i,j), other.take(i,j));
        }
    }
}

template <class Real>
void SmartMatrix<Real>::for_each(std::function<void(Real&)> f)
{
    for (UINT i{ 0 }; i < size_m(); ++i) 
    {
        for (UINT j{ 0 }; j < size_n(); ++j) 
        {
            f(this->at(i,j));
        }
    }
}

/*
 * 
 * 
 * 
 * LOOSE OPERATORS
 * 
 * 
 * 
 */

template <class Real>
std::ostream& operator<<(std::ostream& out, const SmartMatrix<Real>& sm)
{
    sm.Out(out);
	return out;
}

template <class Real>
std::ostream& operator<<(std::ostream& out, XMatrix<Real> xm)
{
    xm->Out(out);
	return out;
}

/* *********************** */
/* BINARY XMATRIX x MATRIX */
/* *********************** */

template <class Real>
XMatrix<Real> operator+(XMatrix<Real> xm, XMatrix<Real> sm)
{
    *xm += *sm;
    return xm;
}

template <class Real>
XMatrix<Real> operator+(XMatrix<Real> xm, const SmartMatrix<Real>& sm)
{
    *xm += sm;
    return xm;
}

template <class Real>
XMatrix<Real> operator-(XMatrix<Real> xm, const SmartMatrix<Real>& sm)
{
    *xm -= sm;
    return xm;
}

template <class Real>
XMatrix<Real> operator&(XMatrix<Real> xm, const SmartMatrix<Real>& sm)
{
	*xm &= sm;
	return xm;
}

template <class Real>
XMatrix<Real> operator^(XMatrix<Real> xm, std::function<Real(Real)> f)
{
	*xm ^= f;
	return xm;
}

template <class Real>
XMatrix<Real> operator*(XMatrix<Real> xm, const Real g)
{
    *xm *= g;
    return xm;
}

#endif /* SMARTMATRIX_HPP */

