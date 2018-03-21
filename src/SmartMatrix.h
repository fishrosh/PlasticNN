#pragma once

#include <memory>
#include <vector>
#include <iterator>
#include <functional>
#include <iostream>

#include "Randomize.h"
#include "MultithreadMultiplicator.h"

#define UINT unsigned int

template <class Real>
class SmartMatrix;

template <class Real>
using XMatrix = std::unique_ptr<SmartMatrix<Real>>;
// using Real = double;

#include <thread>
#include <chrono>
#include <mutex>



template <class Real = double>
class SmartMatrix
{
public:

    using Row = std::unique_ptr<std::vector<Real>>;
    using DataSet = std::unique_ptr<std::vector<Row>>;
    using Matrix = SmartMatrix;
    
    using ulong = unsigned long;

private:

    mutable bool transposition_flag{ false };
	DataSet dataset;
	
        UINT M, N;

public:
    
    

	SmartMatrix() = delete;
	SmartMatrix(UINT n);
        SmartMatrix(Real val, UINT n);
        SmartMatrix(Real* data, UINT m, UINT n);
        SmartMatrix(UINT m, Real val);
	SmartMatrix(UINT m, UINT n, bool zeroed = false);
	SmartMatrix(Row);
	SmartMatrix(DataSet);

	SmartMatrix(const Matrix&);
	SmartMatrix(Matrix&&);
	SmartMatrix(XMatrix<Real>);

	Matrix& operator=(const Matrix&);
	Matrix& operator=(Matrix&&);
	Matrix& operator=(XMatrix<Real>);
        
    XMatrix<Real> operator*(const Matrix&) const;
        
    XMatrix<Real> operator+(const Matrix&) const;
    XMatrix<Real> operator-(const Matrix&) const;
	XMatrix<Real> operator&(const Matrix&) const;

	XMatrix<Real> operator^(std::function<Real(Real)>) const;
        
    XMatrix<Real> operator+(XMatrix<Real>) const;
    XMatrix<Real> operator-(XMatrix<Real>) const;
	XMatrix<Real> operator&(XMatrix<Real>) const;
        
    Matrix& operator+=(const Matrix&);
    Matrix& operator-=(const Matrix&);
	Matrix& operator&=(const Matrix&);

	Matrix& operator*=(const Real g);

	Matrix& operator^=(std::function<Real(Real)>);
        
    // Matrix& T() const;
    const Matrix& T() const;

	// tb removed in the final version
	void addRow(const std::vector<Real>& r);
	void concatenate(const Matrix& m);
        void concatenate(XMatrix<Real>);

	void reset();

	const std::vector<Real>& extractRow(UINT rNum) const;
	Matrix matrixFromRow(UINT rNum);
	// eotbr

	std::ostream& Out(std::ostream& out) const;

	Real take(UINT i, UINT j) const;
        
    Real& at(UINT i, UINT j);
    ulong RowCount () const { return M; };
    ulong RowLength () const;
    
    UINT size_m() const;
    UINT size_n() const;

	// helpers
private:

	void deep_cpy(const Matrix& other);
	void fill_each(std::function<Real(UINT, UINT)> f);
    void fill_each(std::function<Real()> f, int a);
    void multiply_unit(Real& out, const Matrix& m1, const Matrix& m2);
    void for_each(std::function<void(Real&, Real)> f, const Matrix& other);
    void for_each(std::function<void(Real&)> f);
       
};

class MultithreadMultiplicator
{
    using Matrix = SmartMatrix<double>;
    
    //const Matrix& m1;
    //const Matrix& m2;
    
    const UINT MAX_CONCURRENCY = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 4;
    
    std::vector<std::thread> threads;
    
public:
    
    MultithreadMultiplicator()
        {}
    
    void operator()(
        UINT i, const Matrix& m1, const Matrix& m2, Matrix& otpot
    )
    {
        if (threads.size() < MAX_CONCURRENCY)
            threads.push_back( std::thread(&MultithreadMultiplicator::runThread, this, i, std::ref(m1), std::ref(m2), std::ref(otpot)) );
        else {
            for (std::thread& t: threads)
                t.join();
            
            threads.clear();
            threads.push_back( std::thread(&MultithreadMultiplicator::runThread, this, i, std::ref(m1), std::ref(m2), std::ref(otpot)) );
        }
    }
    
    ~MultithreadMultiplicator() {
        for (std::thread& t: threads)
            t.join();
    }
    
private:
    
    void runThread( UINT i, const Matrix& m1, const Matrix& m2, Matrix& otpot)
    {
        for (int j{ 0 }; j < m2.size_n (); ++j) {
            for (int m{ 0 }; m < m1.size_n (); ++m)
            {
                otpot.at(i,j) += m1.take(i, m) * m2.take(m, j);
            }
                                        }
    }
    
    void changeThreadCount(UINT amount) {
        //std::lock_guard<std::mutex> lock(myMutex);
        //MRunningThreadCount += amount;
    }
};//*/

template <class Real = double>
std::ostream& operator<<(std::ostream& out, const SmartMatrix<Real>& sm);

template <class Real = double>
std::ostream& operator<<(std::ostream& out, XMatrix<Real> sm);

template <class Real = double> XMatrix<Real> operator+(XMatrix<Real> xm, const SmartMatrix<Real>& m);
template <class Real = double> XMatrix<Real> operator-(XMatrix<Real> xm, const SmartMatrix<Real>& m);
template <class Real = double> XMatrix<Real> operator&(XMatrix<Real> xm, const SmartMatrix<Real>& m);

template <class Real = double> XMatrix<Real> operator^(XMatrix<Real> xm, std::function<Real(Real)>);
template <class Real = double> XMatrix<Real> operator*(XMatrix<Real> xm, const Real g);

//SmartMatrix operator+(SmartMatrix&& mref, const SmartMatrix& m);
//SmartMatrix operator-(SmartMatrix&& mref, const SmartMatrix& m);

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
	if (N < 1) throw "Problems!";
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
    XMatrix<Real> output = std::make_unique<Matrix>( this->size_m (), other.size_n(), true );
    
    if (this->size_n() == other.size_m()) {
        
        MultithreadMultiplicator mr;
        for (UINT i{ 0 }; i < output->size_m (); ++i) {
            
            
            if (this->size_n () > 1) {
                mr(i, *this, other, *output);
            } 
            
            else {
                for (UINT j{ 0 }; j < output->size_n (); ++j) 
                {
                    for (int m{ 0 }; m < this->size_n (); ++m)
                    {
                        output->at(i, j) += this->take(i, m) * other.take(m, j);
                    }//*/
                }
            }
        }
    }
    
    else {
        std::cout << "Cannot multiply - sizes do not match!" << std::endl;
    }
    
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
	if (other.transposition_flag == this->transposition_flag)
	{
		if (this->size_n() == other.size_n())
		{
			deep_cpy(other);
                        M += other.M;
		}
                
                else {
                    std::cout << "Concatenate - sizes do not match" << std::endl;
                }
	}
        
        else {
            std::cout << "Flags do not match" << std::endl;
        }
}

template <class Real>
void SmartMatrix<Real>::concatenate(XMatrix<Real> other)
{
    if (other->transposition_flag == this->transposition_flag)
	{
		if (this->size_n() == other->size_n())
		{
                    for (auto& r : *other->dataset)
                    {
                        this->dataset->push_back( std::move(r));
                    }
                        M += other->M;
		}
                
                else {
                    std::cout << "Concatenate - sizes do not match" << std::endl;
                }
	}
        
        else {
            std::cout << "Flags do not match" << std::endl;
        }
}

template <class Real>
void SmartMatrix<Real>::addRow(const std::vector<Real>& v)
{
	if (v.size() == size_n())
	{
		Row copy = std::make_unique<std::vector<Real>>();
		std::copy(v.begin(), v.end(), std::back_inserter(*copy));
		(*this).dataset->push_back(std::move(copy));

		if (transposition_flag) ++N;
		else ++M;
	}
        
        else {
            std::cout << "Cannot add row - sizes do not match" << std::endl;
        }
}

template <class Real>
const std::vector<Real>& SmartMatrix<Real>::extractRow(UINT j) const
{
	return *( ( *this->dataset )[j].get() );
}

template <class Real>
SmartMatrix<Real> SmartMatrix<Real>::matrixFromRow(UINT j)
{
	Matrix output{ size_n() };
	output.addRow(extractRow(j));
	return output;
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
        //std::cout << "WARNING! deep copy" << std::endl;
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
    if (this->M == other.M && this->N == other.N)
    {
        for (UINT i{ 0 }; i < other.size_m(); ++i) 
        {
            for (UINT j{ 0 }; j < other.size_n(); ++j) 
            {
                // this->at(i,j) += other.take(i,j);
                f(this->at(i,j), other.take(i,j));
                if(std::isnan(this->at(i,j)) || std::isnan(other.take(i,j))) 
                    std::cout << "fsafdzassdfds";
            }
        }
    }
    
    else {
        std::cout << "Sizes do not match!" << std::endl;
    }
}

template <class Real>
void SmartMatrix<Real>::for_each(std::function<void(Real&)> f)
{
    for (UINT i{ 0 }; i < size_m(); ++i) 
    {
        for (UINT j{ 0 }; j < size_n(); ++j) 
        {
            // this->at(i,j) += other.take(i,j);
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