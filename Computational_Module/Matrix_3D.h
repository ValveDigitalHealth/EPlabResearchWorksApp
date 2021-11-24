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

// #include "Coordinates_Cube_Voxel.h"

#ifndef THREED_MATRIX_H
#define THREED_MATRIX_H

template <class T>
class Matrix_3D
{
	//----------------
	private:
	//----------------

	std::vector<T> Data;

	//----------------
	public:
	//----------------

	Matrix_3D()
	{
		1; //
	}

	long X,Y,Z; // size
/*
	int save_object_to_stream(ofstream& File,T tmp)
	{
		int version = 1;

		File.write((char*)&version, sizeof (int));

		File.write((char*)&X, sizeof (long));
		File.write((char*)&Y, sizeof (long));
		File.write((char*)&Z, sizeof (long));

		long S = Data.size();
		File.write((char*)&S, sizeof (long));
		for(long t=0;t<Data.size();t++)
		File.write((char*)&Data[t], sizeof (T));

		return 1;
	}

	//---------------------------------------------------------------------------------------------
	int load_object_from_stream(ifstream* File)
	{
		int version;
		File->read((char*)&version, sizeof (int));

		if( version == 1 )
		{
		File->read((char*)&X, sizeof (long));
		File->read((char*)&Y, sizeof (long));
		File->read((char*)&Z, sizeof (long));

		long TInt;
		T tmp;
		File->read((char*)&TInt, sizeof (long));
		Data.clear();
		Data.assign(TInt,tmp);
		for(long t=0;t<Data.size();t++)
		File->read((char*)&Data[t], sizeof (T));
		}
		return 1;
	}
*/

	//---------------------------------------------------------------------------------------------
	void create_matrix(long x, long y, long z, T Initial_Value) // elements in the matrix indexed from ZERO!!!!!
	{
		X = x; Y = y; Z = z;

		Data.clear();
		Data.assign(x*y*z,Initial_Value);
	}

	//---------------------------------------------------------------------------------------------
	T get_value(long x, long y, long z)
	{
		T tmp_val;

		long ptr = z*X*Y + y*X + x;
		if( ptr >= 0 && ptr < Data.size() )
		return Data[ptr];
		else
		return tmp_val;
	}
	//---------------------------------------------------------------------------------------------

	T* get_pointer_to_value(long x, long y, long z)
	{
		long ptr = z*X*Y + y*X + x;
		if( ptr >= 0 && ptr < Data.size() )
		return &Data[ptr];
		else
		return NULL;
	}

	//---------------------------------------------------------------------------------------------
	void set_value(long x, long y, long z, T v)
	{
	long ptr = z*X*Y + y*X + x;
	if( ptr >= 0 && ptr < Data.size() )
	Data[ptr] = v;
	}

	//---------------------------------------------------------------------------------------------

};

/*
template <class A_Type> class calc
{
  public:
	A_Type multiply(A_Type x, A_Type y);
	A_Type add(A_Type x, A_Type y);
};
template <class A_Type> A_Type calc<A_Type>::multiply(A_Type x,A_Type y)
{
  return x*y;
}
template <class A_Type> A_Type calc<A_Type>::add(A_Type x, A_Type y)
{
  return x+y;
}
*/

#endif

