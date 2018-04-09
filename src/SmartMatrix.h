#pragma once

#include <memory>
#include <vector>
#include <iterator>
#include <functional>
#include <iostream>

#include "Randomize.h"

#define UINT unsigned int

struct MultiplySizeException : public std::exception {
    const char * what () const noexcept override { return "Matrices A and B cannot be multiplied due to size differences!"; }
};

struct ConcatenateSizeException : public std::exception {
    const char * what () const noexcept override { return "Concatenation cannot proceed due to differences in sizes!"; }
};

template <class Real>
class SmartMatrix;

template <class Real>
using XMatrix = std::unique_ptr<SmartMatrix<Real>>;

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

        const Matrix& T() const;

        void concatenate(const std::vector<Real>& r);
        void concatenate(const Matrix& m);
        void concatenate(XMatrix<Real>);

        void reset();

        const std::vector<Real>& extractRow(UINT rNum) const;

        std::ostream& Out(std::ostream& out) const;

        Real take(UINT i, UINT j) const;

        Real& at(UINT i, UINT j);
        const Real& at(UINT i, UINT j) const;
        
        ulong RowCount () const { return M; };
        ulong RowLength () const;

        UINT size_m() const;
        UINT size_n() const;
        
        struct Iterator {
            
            ulong i () const { return _i; }
            ulong j () const { return _j; }
            
            Real& operator * () const { return _m.at (_i, _j); }
            
            Iterator& operator++ () {
                ++_i; ++_j; return *this; // LOL change this
            }
            
            bool operator==(const Iterator& other) const {
                return this->_i == other._i && this->_j == other._j;
            }
            
            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }
            
            Iterator (Matrix& m) : _m(m), _i(0), _j(0) {}
            Iterator (Matrix& m, ulong i, ulong j) : _m(m), _i(i), _j(j) {}
            
        private:
            
            Matrix& _m;
            ulong _i, _j;
        };
        
        Iterator begin () { return Iterator {*this}; }
        Iterator end () { return Iterator {*this, size_m (), 0 }; }
        
private:

	void deep_cpy(const Matrix& other);
	void fill_each(std::function<Real(UINT, UINT)> f);
        void fill_each(std::function<Real()> f, int a);
        void multiply_unit(Real& out, const Matrix& m1, const Matrix& m2);
        void for_each(std::function<void(Real&, Real)> f, const Matrix& other);
        void for_each(std::function<void(Real&)> f);
       
};

template <class Real = double>
std::ostream& operator<<(std::ostream& out, const SmartMatrix<Real>& sm);

template <class Real = double>
std::ostream& operator<<(std::ostream& out, XMatrix<Real> sm);

template <class Real = double> XMatrix<Real> operator+(XMatrix<Real> xm, XMatrix<Real> sm);
template <class Real = double> XMatrix<Real> operator+(XMatrix<Real> xm, const SmartMatrix<Real>& m);
template <class Real = double> XMatrix<Real> operator-(XMatrix<Real> xm, const SmartMatrix<Real>& m);
template <class Real = double> XMatrix<Real> operator&(XMatrix<Real> xm, const SmartMatrix<Real>& m);

template <class Real = double> XMatrix<Real> operator^(XMatrix<Real> xm, std::function<Real(Real)>);
template <class Real = double> XMatrix<Real> operator*(XMatrix<Real> xm, const Real g);



