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
#ifndef TWOD_MATRIX_H
#define TWOD_MATRIX_H

#include <cmath>
#include <vector>

class Matrix_2D
{
	private:

	std::vector<double> Data;

	public:

	std::vector<double> *get_data_vector_pointer();

	Matrix_2D();

	long X,Y; // size

	void create_matrix(long x, long y); // elements in the matrix indexed from ZERO!!!!!

	double get_value(long x,long y);
	void set_value(long x, long y, double v);

	void calculate_max_min_mean_SD_excluding_diagonal(
		double* Min, double* Max, double* Mean, double* SD);

};
#endif