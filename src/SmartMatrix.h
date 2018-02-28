#pragma once

#include <memory>
#include <vector>
#include <iterator>
#include <functional>
#include <iostream>

#include "Randomize.h"

#define UINT unsigned int

class SmartMatrix;

using XMatrix = std::unique_ptr<SmartMatrix>;
using Real = double;

class SmartMatrix
{
public:

    using Row = std::unique_ptr<std::vector<Real>>;
    using DataSet = std::unique_ptr<std::vector<Row>>;
    using Matrix = SmartMatrix;

private:

    bool transposition_flag{ false };
	DataSet dataset;

public:
    
    
	UINT M, N;

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
	SmartMatrix(XMatrix);

	Matrix& operator=(const Matrix&);
	Matrix& operator=(Matrix&&);
	Matrix& operator=(XMatrix);
        
    XMatrix operator*(const Matrix&) const;
        
    XMatrix operator+(const Matrix&) const;
    XMatrix operator-(const Matrix&) const;
	XMatrix operator&(const Matrix&) const;

	XMatrix operator^(std::function<Real(Real)>) const;
        
    XMatrix operator+(XMatrix) const;
    XMatrix operator-(XMatrix) const;
	XMatrix operator&(XMatrix) const;
        
    Matrix& operator+=(const Matrix&);
    Matrix& operator-=(const Matrix&);
	Matrix& operator&=(const Matrix&);

	Matrix& operator*=(const Real g);

	Matrix& operator^=(std::function<Real(Real)>);
        
    Matrix& T();

	// tb removed in the final version
	void addRow(const std::vector<Real>& r);
	void concatenate(const Matrix& m);
        void concatenate(XMatrix);

	void reset();

	const std::vector<Real>& extractRow(UINT rNum) const;
	Matrix matrixFromRow(UINT rNum);
        
        std::vector<UINT> TopValues();
	// eotbr

	std::ostream& Out(std::ostream& out) const;

	Real take(UINT i, UINT j) const;

	~SmartMatrix();
        
    Real& at(UINT i, UINT j);

	// helpers
private:

	void deep_cpy(const Matrix& other);
	void fill_each(std::function<Real(UINT, UINT)> f);
    void fill_each(std::function<Real()> f, int a);
    void multiply_unit(Real& out, const Matrix& m1, const Matrix& m2);
    void for_each(std::function<void(Real&, Real)> f, const Matrix& other);
    void for_each(std::function<void(Real&)> f);
        
        
    UINT get_m() const;
    UINT get_n() const;
        

};

std::ostream& operator<<(std::ostream& out, const SmartMatrix& sm);
std::ostream& operator<<(std::ostream& out, XMatrix sm);

XMatrix operator+(XMatrix xm, const SmartMatrix& m);
XMatrix operator-(XMatrix xm, const SmartMatrix& m);
XMatrix operator&(XMatrix xm, const SmartMatrix& m);

XMatrix operator^(XMatrix xm, std::function<Real(Real)>);
XMatrix operator*(XMatrix xm, const Real g);

//SmartMatrix operator+(SmartMatrix&& mref, const SmartMatrix& m);
//SmartMatrix operator-(SmartMatrix&& mref, const SmartMatrix& m);

