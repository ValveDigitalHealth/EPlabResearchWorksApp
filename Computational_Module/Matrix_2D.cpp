//---------------------------------------------------------------------------
/*

MIT LICENSE Copyright (c) 2021 Pawel Kuklik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the SOFTWARE.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include "Matrix_2D.h"
//---------------------------------------------------------------------------

Matrix_2D::Matrix_2D()
{
	create_matrix(2,2);
}

//---------------------------------------------------------------------------

void Matrix_2D::create_matrix(long x, long y)
{
	double tmp=0;

	X = x;
	Y = y;

	Data.clear();
	Data.assign(x*y,tmp);
}

//---------------------------------------------------------------------------
double Matrix_2D::get_value(long x, long y)
{
// X=3, Y=2; element (x=2,y=1): ptr= y*X + x; (=3+2=5)
	long ptr = y*X + x;
	if( ptr >= 0 && ptr < Data.size() )
	return Data[ptr];
	else
	return 0;
}

//---------------------------------------------------------------------------
void Matrix_2D::set_value(long x, long y, double v)
{
	long ptr = y*X + x;
	if( ptr >= 0 && ptr < Data.size() )
	Data[ptr] = v;
}

//---------------------------------------------------------------------------

std::vector<double> *Matrix_2D::get_data_vector_pointer()
{
	return &Data;
}

//---------------------------------------------------------------------------

void Matrix_2D::calculate_max_min_mean_SD_excluding_diagonal(
		double* Min, double* Max, double* Mean, double* SD)
{
	// 1. find max,min,mean from last DATA_LENGTH_FOR_PEAK_EXTRACTION
	// steps in roving and ref. cath
	Min[0] = 10000000;
	Max[0] = -10000000;
	Mean[0] = 0.0;
	double v,Counter=0;

	for(long i=0; i<X; i++)
	for(long j=0; j<Y; j++)
	if(i!=j)
	{

	v = get_value(i,j);

	Counter++;

	Mean[0] += v;

	if( v > Max[0] )
		Max[0] = v;

	if( v < Min[0] )
		Min[0] = v;
	}

	if( Counter != 0 )
	Mean[0] /= Counter;
	else
	Mean[0]=0;

	// 2. calculate stdev

	SD[0] = 0.0;
	for(long i=0; i<X; i++)
	for(long j=0; j<Y; j++)
	if(i!=j)
		SD[0] += std::pow( (double)(get_value(i,j) - Mean[0]), 2.0 );

	if (SD[0] > 0 && Counter != 0 )
	SD[0] = std::sqrt( SD[0] / ( Counter-1 ) );
	else SD[0] = 0;
}

//----------------------------------------------------------------------------------

