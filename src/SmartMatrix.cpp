#include <thread>

#include "SmartMatrix.h"

SmartMatrix::SmartMatrix(UINT n)
	: N{ n }, M{ 0 }, dataset{ std::make_unique<std::vector<Row>>() }
{}

SmartMatrix::SmartMatrix(UINT m, UINT n, bool zeroed)
	: N{ n }, M{ m }, dataset{ std::make_unique<std::vector<Row>>() }
{
        Randomize<> r;
        
	auto lambda = [&r](UINT a, UINT b)->double { return r.GetReal(); };
        auto zlambda = [&r](UINT a, UINT b)->double { return 0.0; };
        
	if (zeroed) fill_each(zlambda);
        else fill_each(lambda);
}
        
SmartMatrix::SmartMatrix(double val, unsigned int n)
    :   N{ n }, M{ 1 }, dataset{ std::make_unique<std::vector<Row>>() }
{
        dataset->push_back( std::make_unique<std::vector<double>>( n, val ));
}
    
    SmartMatrix::SmartMatrix(double* data, unsigned int m, unsigned int n)
    {}
    
SmartMatrix::SmartMatrix(unsigned int m, double val)
    :   N{ 1 }, M{ m }, dataset{ std::make_unique<std::vector<Row>>() }
{
        for (int i{ 0 }; i++ < m;) dataset->push_back( std::make_unique<std::vector<double>>( 1, val ));
}

SmartMatrix::SmartMatrix(Row r)
	: N{ ( UINT )r->size() }, M{ 1 }, dataset{ std::make_unique<std::vector<Row>>() }
{
	dataset->push_back(std::move(r));
}

SmartMatrix::SmartMatrix(DataSet d) 
    : 
    N{ ( UINT )d->size() > 0 ? ( UINT )( *d )[0]->size() : 0 }, 
    M{ ( UINT )d->size() }, dataset{ std::move(d) }
{
	if (N < 1) throw "Problems!";
}

SmartMatrix::SmartMatrix(const Matrix& other)
	: N{ other.N }, M{ other.M }, dataset{ std::make_unique<std::vector<Row>>() }, transposition_flag{ other.transposition_flag }
{
	deep_cpy(other);
} 

SmartMatrix::SmartMatrix(Matrix&& other)
	: N{ other.N }, M{ other.M }, dataset{ std::move(other.dataset) }, transposition_flag{ other.transposition_flag }
{}

SmartMatrix::SmartMatrix(XMatrix other)
	: N{ other->N }, M{ other->M }, dataset{ std::move(other->dataset) }, transposition_flag{ other->transposition_flag }
{}

SmartMatrix& SmartMatrix::operator=(const Matrix& other)
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

SmartMatrix& SmartMatrix::operator=(Matrix&& other)
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

SmartMatrix& SmartMatrix::operator=(XMatrix other)
{
	M = other->M;
	N = other->N;

	transposition_flag = other->transposition_flag;

	(*this).dataset->clear();
	(*this).dataset = std::move(other->dataset);

	return *this;
}

std::ostream& SmartMatrix::Out(std::ostream& out) const
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

SmartMatrix::~SmartMatrix()
{
}

#include <chrono>
#include <mutex>

class MultithreadMultiplicator
{
    using Matrix = SmartMatrix;
    
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
        for (int j{ 0 }; j < m2.N; ++j) {
            for (int m{ 0 }; m < m1.N; ++m)
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

XMatrix SmartMatrix::operator *(const Matrix& other) const
{
    XMatrix output = std::make_unique<Matrix>( this->M, other.N, true );
    
    if (this->N == other.M) {
        
        MultithreadMultiplicator mr;
        for (UINT i{ 0 }; i < output->M; ++i) {
            
            
            if (this->N > 1) {
                mr(i, *this, other, *output);
            } 
            
            else {
                for (UINT j{ 0 }; j < output->N; ++j) 
                {
                    for (int m{ 0 }; m < this->N; ++m)
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

SmartMatrix& SmartMatrix::operator *=(const double g)
{
    auto lambda = [g](double& a) { a *= g; };
    for_each(lambda);
    return *this;
}

XMatrix SmartMatrix::operator^(std::function<double(double)> f) const
{
	XMatrix output = std::make_unique<Matrix>(*this);
	*output ^= f;
	return output;
}

/* ********************** */
/* BINARY MATRIX x MATRIX */
/* ********************** */

XMatrix SmartMatrix::operator +(const Matrix& other) const
{
    XMatrix output = std::make_unique<Matrix>( *this );
    *output += other;
    return output;
}

XMatrix SmartMatrix::operator -(const Matrix& other) const
{
    XMatrix output = std::make_unique<Matrix>( *this );
    *output -= other;
    return output;
}

XMatrix SmartMatrix::operator &(const Matrix& other) const
{
	XMatrix output = std::make_unique<Matrix>(*this);
	*output &= other;
	return output;
}

/* *********************** */
/* BINARY MATRIX x XMATRIX */
/* *********************** */

XMatrix SmartMatrix::operator +(XMatrix other) const
{
    *other += *this;
    return other;
}

XMatrix SmartMatrix::operator -(XMatrix other) const
{
    *other -= *this;
	*other *= -1.0;
    return other;
}

XMatrix SmartMatrix::operator &(XMatrix other) const
{
	*other &= *this;
	return other;
}

/* *************************** */
/* ASSIGNMENT MATRIX to MATRIX */
/* *************************** */

SmartMatrix& SmartMatrix::operator +=(const Matrix& other)
{
    auto lambda = [](double& a, double b) { a += b; };
    for_each(lambda, other);
    return *this;
}

SmartMatrix& SmartMatrix::operator -=(const Matrix& other)
{
    auto lambda = [](double& a, double b) { a -= b; };
    for_each(lambda, other);
    return *this;
}

SmartMatrix& SmartMatrix::operator &=(const Matrix& other)
{
	auto lambda = [](double& a, double b) { a *= b; };
	for_each(lambda, other);
	return *this;
}

SmartMatrix& SmartMatrix::operator ^=(std::function<double(double)> f)
{
	auto lambda = [&f](double& a) { a = f(a); };
	for_each(lambda);
	return *this;
}

/* *************************** */
/* TO BE REMOVED FROM FINAL VER */
/* *************************** */

void SmartMatrix::concatenate(const Matrix& other)
{
	if (other.transposition_flag == this->transposition_flag)
	{
		if (this->get_n() == other.get_n())
		{
			deep_cpy(other);

			if (transposition_flag) N += other.N;
			else M += other.M;
		}
                
                else {
                    std::cout << "Concatenate - sizes do not match" << std::endl;
                }
	}
        
        else {
            std::cout << "Flags do not match" << std::endl;
        }
}

void SmartMatrix::concatenate(XMatrix other)
{
    if (other->transposition_flag == this->transposition_flag)
	{
		if (this->get_n() == other->get_n())
		{
                    for (auto& r : *other->dataset)
                    {
                        this->dataset->push_back( std::move(r));
                    }

			if (transposition_flag) N += other->N;
			else M += other->M;
		}
                
                else {
                    std::cout << "Concatenate - sizes do not match" << std::endl;
                }
	}
        
        else {
            std::cout << "Flags do not match" << std::endl;
        }
}

void SmartMatrix::addRow(const std::vector<double>& v)
{
	if (v.size() == get_n())
	{
		Row copy = std::make_unique<std::vector<double>>();
		std::copy(v.begin(), v.end(), std::back_inserter(*copy));
		(*this).dataset->push_back(std::move(copy));

		if (transposition_flag) ++N;
		else ++M;
	}
        
        else {
            std::cout << "Cannot add row - sizes do not match" << std::endl;
        }
}

const std::vector<double>& SmartMatrix::extractRow(UINT j) const
{
	return *( ( *this->dataset )[j].get() );
}

SmartMatrix SmartMatrix::matrixFromRow(UINT j)
{
	Matrix output{ get_n() };
	output.addRow(extractRow(j));
	return output;
}

std::vector<UINT> SmartMatrix::TopValues()
{
    std::vector<UINT> output;
    
    for (auto& r : *this->dataset)
    {
        int i{ 0 };
        int fIndex{ -1 };
        double highest = -1.0f;
        
        for (double g : *r)
        {
            if (g > highest) {
                fIndex = i;
                highest = g;
                
            }
            i++;
        }
        
        output.push_back(fIndex);
    }
    
    return output;
}

void SmartMatrix::reset()
{
	dataset->clear();
	M = 0;
	transposition_flag = false;
}
/* *************************** */
/* ASSIGNMENT MATRIX to MATRIX */
/* *************************** */

SmartMatrix& SmartMatrix::T()
{
    transposition_flag = !transposition_flag;
    std::swap(M, N);
    return *this;
}

void SmartMatrix::deep_cpy(const Matrix& other)
{
        //std::cout << "WARNING! deep copy" << std::endl;
	for (auto& r : *other.dataset)
	{
		Row copy = std::make_unique<std::vector<double>>();
		std::copy(r->begin(), r->end(), std::back_inserter(*copy));
		(*this).dataset->push_back(std::move(copy));
	}
}

void SmartMatrix::fill_each(std::function<double(UINT, UINT)> f)
{
	if (dataset->empty())
	{
		for (UINT i{ 0 }; i < M; ++i)
		{
			Row r = std::make_unique<std::vector<double>>();
			for (UINT j{ 0 }; j < N; ++j)
			{
				(*r).push_back(f(i, j));
			}
			dataset->push_back(std::move(r));
		}
	}
}

void SmartMatrix::fill_each(std::function<double()> f, int a)
{
	if (dataset->empty())
	{
		for (UINT i{ 0 }; i < M; ++i)
		{
			Row r = std::make_unique<std::vector<double>>();
			for (UINT j{ 0 }; j < N; ++j)
			{
				(*r).push_back(f());
			}
			dataset->push_back(std::move(r));
		}
	}
}

void SmartMatrix::for_each(std::function<void(double&, double)> f, const Matrix& other)
{
    if (this->M == other.M && this->N == other.N)
    {
        for (UINT i{ 0 }; i < other.get_m(); ++i) 
        {
            for (UINT j{ 0 }; j < other.get_n(); ++j) 
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

void SmartMatrix::for_each(std::function<void(double&)> f)
{
    for (UINT i{ 0 }; i < get_m(); ++i) 
    {
        for (UINT j{ 0 }; j < get_n(); ++j) 
        {
            // this->at(i,j) += other.take(i,j);
            f(this->at(i,j));
        }
    }
}


double& SmartMatrix::at(UINT i, UINT j) 
{
    return !transposition_flag ? (*(( *dataset )[i]))[j] : (*(( *dataset )[j]))[i];
}

double SmartMatrix::take(UINT i, UINT j) const
{
    return !transposition_flag ? (*(( *dataset )[i]))[j] : (*(( *dataset )[j]))[i];
}

UINT SmartMatrix::get_m() const
{
    return this->transposition_flag ? N : M;
}

UINT SmartMatrix::get_n() const
{
    return this->transposition_flag ? M : N;
}

std::ostream& operator<<(std::ostream& out, const SmartMatrix& sm)
{
    sm.Out(out);
	return out;
}

std::ostream& operator<<(std::ostream& out, XMatrix xm)
{
    xm->Out(out);
	return out;
}

/* *********************** */
/* BINARY XMATRIX x MATRIX */
/* *********************** */

XMatrix operator+(XMatrix xm, const SmartMatrix& sm)
{
    *xm += sm;
    return xm;
}

XMatrix operator-(XMatrix xm, const SmartMatrix& sm)
{
    *xm -= sm;
    return xm;
}

XMatrix operator&(XMatrix xm, const SmartMatrix& sm)
{
	*xm &= sm;
	return xm;
}

XMatrix operator^(XMatrix xm, std::function<double(double)> f)
{
	*xm ^= f;
	return xm;
}

XMatrix operator*(XMatrix xm, const double g)
{
    *xm *= g;
    return xm;
}