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
#include <vector>
#include <fstream>

#ifndef THREED_DOUBLE_MATRIX_H
#define THREED_DOUBLE_MATRIX_H

class Matrix_3D_double
{
	//----------------
	private:
	//----------------

	std::vector<double> Data;

	//----------------
	public:
	//----------------

	Matrix_3D_double()
	{
		1; //
	}

	long X,Y,Z; // size

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);


	//---------------------------------------------------------------------------------------------
	void create_matrix(long x, long y, long z, double Initial_Value) // elements in the matrix indexed from ZERO!!!!!
	{
		X = x; Y = y; Z = z;

		Data.clear();
		Data.assign(x*y*z,Initial_Value);
	}

	//---------------------------------------------------------------------------------------------
	double get_value(long x, long y, long z)
	{
		double tmp_val;

		long ptr = z*X*Y + y*X + x;
		if( ptr >= 0 && ptr < Data.size() )
		return Data[ptr];
		else
		return tmp_val;
	}
	//---------------------------------------------------------------------------------------------

	double* get_pointer_to_value(long x, long y, long z)
	{
		long ptr = z*X*Y + y*X + x;
		if( ptr >= 0 && ptr < Data.size() )
		return &Data[ptr];
		else
		return NULL;
	}

	//---------------------------------------------------------------------------------------------
	void set_value(long x, long y, long z, double v)
	{
	long ptr = z*X*Y + y*X + x;
	if( ptr >= 0 && ptr < Data.size() )
	Data[ptr] = v;
	}

	//---------------------------------------------------------------------------------------------

};

#endif
