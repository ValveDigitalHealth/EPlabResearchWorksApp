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
#include <vcl.h>
#include <fstream>

#ifndef THREED_MATRIX_POINTER_H
#define THREED_MATRIX_POINTER_H

template <class T>
class Matrix_3D_pointer
{
	//----------------
	private:
	//----------------


	//----------------
	public:
	//----------------

	T* Data;

	long X,Y,Z; // size

	//---------------------------------------------------------------------------------------------

	// assignment operator      (nie dziala...)
	T& operator=(const T& other)
	{
		delete [] Data;

		X = other.X;
		Y = other.Y;
		Z = other.Z;

		create_matrix(X,Y,Z,0);

		for(long i=0;i<X;i++)
		for(long j=0;j<Y;j++)
		for(long k=0;k<Z;k++)
		set_value(i,j,k, other.get_value(i,j,k) );
	}

	//---------------------------------------------------------------------------------------------

	void clear(T Value)
	{
		for(long i=0;i<X*Y*Z;i++)
		Data[i] = Value;
	}

	//---------------------------------------------------------------------------------------------
	Matrix_3D_pointer()
	{
		Data = new T[2];
	}

	//---------------------------------------------------------------------------------------------
	~Matrix_3D_pointer()
	{
		delete [] Data;
	}

	//---------------------------------------------------------------------------

	int save_object_to_stream(ofstream* File)
	{
		int version = 1;

		File->write((char*)&version, sizeof (int));

		File->write((char*)&X, sizeof (long));
		File->write((char*)&Y, sizeof (long));
		File->write((char*)&Z, sizeof (long));

		for(long i=0;i<X*Y*Z;i++)
		File->write((char*)&Data[i], sizeof (T));
	}
	//---------------------------------------------------------------------------

	int load_object_from_stream(ifstream* File)
	{
		int version;
		double v=0;

		File->read((char*)&version, sizeof (int));

		if( version == 1 )
		{

		File->read((char*)&X, sizeof (long));
		File->read((char*)&Y, sizeof (long));
		File->read((char*)&Z, sizeof (long));

		create_matrix(X, Y, Z, 0);

		for(long i=0;i<X*Y*Z;i++)
		File->read((char*)&Data[i], sizeof (T));

		}

		return -1;
	}

	//---------------------------------------------------------------------------------------------

	void copy_data_from(T* DataB,long x, long y, long z)
	{
		create_matrix(x,y,z,0);

		for(long i=0;i<x*y*z;i++)
		Data[i] = DataB[i];

	}
	//---------------------------------------------------------------------------------------------

	void create_matrix(long x, long y, long z, T Initial_Value) // elements in the matrix indexed from ZERO!!!!!
	{
		X = x;
		Y = y;
		Z = z;

		delete [] Data;

		Data = new T[x*y*z];

		for(long i=0;i<x*y*z;i++)
		Data[i] = Initial_Value;
	}

	//---------------------------------------------------------------------------------------------
	T get_value(long x, long y, long z)
	{
		T tmp_val=0;

		long ptr = z*X*Y + y*X + x;
		if( ptr >= 0 && ptr < X*Y*Z )
		return Data[ptr];
		else
		return tmp_val;
	}
	//---------------------------------------------------------------------------------------------

	T* get_pointer_to_value(long x, long y, long z)
	{
		long ptr = z*X*Y + y*X + x;
		if( ptr >= 0 && ptr < X*Y*Z )
		return &Data[ptr];
		else
		return NULL;
	}

	//---------------------------------------------------------------------------------------------
	void set_value(long x, long y, long z, T v)
	{
	long ptr = z*X*Y + y*X + x;
	if( ptr >= 0 && ptr < X*Y*Z )
	Data[ptr] = v;
	}

	//---------------------------------------------------------------------------------------------

};

#endif



