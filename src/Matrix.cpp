#include "stdafx.h"
/*
Matrix::Matrix(const DataSet& ds)
	: M(ds.size()), N(ds.size() > 0 ? ds.begin()->size() : 0)
{
	for (const std::vector<double>& v : ds)
	{
		dataset.push_back(std::vector<double>{});

		if (v.size() != N)
		{
			throw "Sizes do not match!";
		}

		for (double d : v)
		{
			dataset.back().push_back(d);
		}
	}
}

Matrix::Matrix(const Matrix& m)
	: M{ m.M }, N{ m.N }
{
            std::cout << "Warning! Matrix copy!" << std::endl;
	for (const Row& R : m.dataset)
	{
		this->dataset.push_back({});
		std::copy(R.begin(), R.end(), std::back_inserter( (this->dataset.back()) ));
	}
}

Matrix::Matrix(const Row& r)
	: M{ 0 }, N{ r.size() }
{
	this->addRow(r);
}

Matrix::Matrix(UINT n)
	: M{ 0 }, N{ n }
{

}

Matrix::Matrix(double val, UINT count)
	: M{ 1 }, N{ count }
{
	dataset.push_back(Row(count, val));
}

Matrix::Matrix(double* data, UINT m, UINT n)
	: M{ 0 }, N{ n }
{
	for (int i{ 0 }; i < m; ++i)
	{
		Row r;
		for (int j{ 0 }; j < n; ++j)
		{
			r.push_back(*data);
			data++;
		}
		this->addRow(r);
	}
}

void Matrix::addRow(const Row& r)
{
	if (r.size() != N)
		throw "Size do not match";

	++M;

	dataset.push_back({});
	std::copy(r.begin(), r.end(), std::back_inserter(dataset.back()));
}

void Matrix::concatenate(const Matrix& m)
{
	if (m.N != N)
		throw "Sizes do not match";

	for (const Row& r : m.dataset)
	{
		this->addRow(r);
	}
}

Matrix::Matrix(UINT m, UINT n)
	: M{ m }, N{ n }, dataset{ m, Row(n, 0.0 ) } // Row{ n, 0.0 } IS NOT WORKING!!!!! - lame should notify strousoup
{
	/*for (Row& r : dataset)
	{
		for (int i = 0; i < n; ++i)
		{
			r.push_back(0.0);
		}
	}//
}

Matrix& Matrix::operator=(const Matrix& other)
{
	if (this != &other)
	{
		this->M = 0;
		this->N = other.N;

		dataset.clear();

		this->concatenate(other);
	}
	return *this;
}

void Matrix::reset()
{
	dataset.clear();
	M = 0;
}

double Matrix::at(UINT m, UINT n) const
{
	return dataset.at(m).at(n);
}

Matrix Matrix::T() const
{
	DataSet nds;
	for (UINT u = 0; u < N; ++u)
	{
		Row r;
		for (UINT y = 0; y < M; ++y)
		{
			r.push_back(at(y, u));
		}

		nds.push_back(r);
	}

	return nds;
}

Matrix Matrix::operator*(const Matrix& m) const
{
	if (this->N != m.M) throw "Sizes do not match!";

	DataSet output;
	for (const Row& R : this->dataset)
	{
		Row r;
		for (UINT u = 0; u < m.N; ++u)
		{
			double sum = 0.0;
			for (UINT y = 0; y < m.M; ++y)
			{
				sum += m.at(y, u) * R.at(y);
			}

			r.push_back(sum);
		}

		output.push_back(r);
	}

	return output;
}

const Matrix::Row& Matrix::extractRow(UINT rNum) const
{
	if (rNum < 0 || rNum > M)
		throw "Number outside of range";

	return dataset[rNum];
}

Matrix Matrix::matrixFromRow(UINT rNum)
{
	return{ extractRow(rNum) };
}

Matrix Matrix::operator*(const double& o) const
{
	Matrix output{ *this };
	output *= o;
	return output;
}

Matrix Matrix::operator&(const Matrix& m) const
{
	Matrix output{ *this };
	output &= m;
	return output;
}

Matrix Matrix::operator+(const Matrix& m) const
{
	Matrix output{ *this };
	output += m;
	return output;
}

Matrix Matrix::operator-(const Matrix& m) const
{
	Matrix output{ *this };
	output -= m;
	return output;
}

Matrix Matrix::operator^(std::function<void(double&)> f) const
{
	Matrix output{ *this };
	output ^= f;
	return output;
}

void Matrix::operator*=(const double d)
{
	for (Row& R : dataset)
	{
		for (double& i : R)
		{
			i *= d;
		}
	}
}

void Matrix::operator&=(const Matrix& m)
{
	if (this->M != m.M || this->N != m.N) throw "Sizes do not match!";
	for (UINT u = 0; u < m.M; ++u)
	{
		for (UINT y = 0; y < m.N; ++y)
		{
			this->dataset.at(u).at(y) *= m.dataset.at(u).at(y);
		}
	}
}

void Matrix::operator+=(const Matrix& m)
{
	if (this->M != m.M || this->N != m.N) throw "Sizes do not match!";
	for (UINT u = 0; u < m.M; ++u)
	{
		for (UINT y = 0; y < m.N; ++y)
		{
			this->dataset.at(u).at(y) += m.dataset.at(u).at(y);
		}
	}
}

void Matrix::operator^=(std::function<void(double&)> f)
{
	for (Row& R : dataset)
	{
		for (double& i : R)
		{
			f(i);
		}
	}
}

void Matrix::operator-=(const Matrix& m)
{
	*this += m * -1.0;
}

std::ostream& operator<<(std::ostream& out, const Matrix& m)
{
	for (UINT i = 0; i < m.M; ++i)
	{
		out << std::endl;
		for (UINT j = 0; j < m.N; ++j)
		{
			out << m.at(i, j) << " ";
		}
	}

	return out;
}//*/