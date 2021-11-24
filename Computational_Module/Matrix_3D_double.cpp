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
#include "Matrix_3D_double.h"
//---------------------------------------------------------------------------

int Matrix_3D_double::save_object_to_stream(ofstream* File)
{
		int version = 1;
		File->write((char*)&version, sizeof (int));

		File->write((char*)&X, sizeof (long));
		File->write((char*)&Y, sizeof (long));
		File->write((char*)&Z, sizeof (long));

		long S = Data.size();
		File->write((char*)&S, sizeof (long));
		for(long t=0;t<Data.size();t++)
		File->write((char*)&Data[t], sizeof (double));

		return 1;
}

//---------------------------------------------------------------------------------------------

int Matrix_3D_double::load_object_from_stream(ifstream* File)
{
		int version;
		File->read((char*)&version, sizeof (int));

		if( version == 1 )
		{
		File->read((char*)&X, sizeof (long));
		File->read((char*)&Y, sizeof (long));
		File->read((char*)&Z, sizeof (long));

		long TInt;
		double tmp;
		File->read((char*)&TInt, sizeof (long));
		Data.clear();
		Data.assign(TInt,tmp);
		for(long t=0;t<Data.size();t++)
		File->read((char*)&Data[t], sizeof (double));
		}

		return 1;
}

//---------------------------------------------------------------------------------------------

/*
template <class T>
void Matrix_3D<T>::create_matrix(long x, long y, long z)
{
	T tmp;

	X = x;
	Y = y;
	Z = z;

	Data.clear();
	Data.assign(x*y*z,tmp);
}

//---------------------------------------------------------------------------
template <class T>
T Matrix_3D<T>::get_value(long x, long y, long z)
{
	long ptr = z*X*Y + y*X + x;
	if( ptr >= 0 && ptr < Data.size() )
	return Data[ptr];
	else
	return 0;
}

//---------------------------------------------------------------------------
template <class T>
void Matrix_3D<T>::set_value(long x, long y, long z, T v)
{
	long ptr = z*X*Y + y*X + x;
	if( ptr >= 0 && ptr < Data.size() )
	Data[ptr] = v;
}

//---------------------------------------------------------------------------
*/
