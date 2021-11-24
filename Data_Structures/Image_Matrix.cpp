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

#pragma hdrstop

#include "Image_Matrix.h"
#include <stdlib.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

Image_Matrix_Class::Image_Matrix_Class()
{

	double tmp=0;

	Image_Matrix.create_matrix(2,2,2,tmp);

	Min_X = 0;
	Max_X = 1;
	Min_Y = 0;
	Max_Y = 1;

	Min_Z = 1;
	Max_Z = 1;

	Max_Intensity = 1.0;

	X_Resolution = 1.0;
	Y_Resolution = 1.0;
	Z_Resolution = 1.0;
}

//----------------------------------------------------------------------------------------------

Image_Matrix_Class::~Image_Matrix_Class()
{

}

//----------------------------------------------------------------------------------------------

int Image_Matrix_Class::allocate_image_matrix(int new_Image_X,int new_Image_Y,int new_Image_Z)
{
	double tmp=0;
	Image_Matrix.create_matrix(new_Image_X,new_Image_Y,new_Image_Z,tmp);

	Min_X = 0.2*new_Image_X;
	Max_X = 0.8*new_Image_X;

	Min_Y = 0.2*new_Image_Y;
	Max_Y = 0.8*new_Image_Y;

	Min_Z = 0.2*new_Image_Z;
	Max_Z = 0.8*new_Image_Z;
}

//----------------------------------------------------------------------------------------------

int Image_Matrix_Class::calculate_max_intensity()
{

	Max_Intensity = 0;

	for( int i=0; i<Image_Matrix.X; i++)
	for( int j=0; j<Image_Matrix.Y; j++)
	for( int k=0; k<Image_Matrix.Z; k++)
	if( Image_Matrix.get_value(i,j,k) > Max_Intensity )
		Max_Intensity = Image_Matrix.get_value(i,j,k);

}

//----------------------------------------------------------------------------------------------

int Image_Matrix_Class::save_object_to_stream(ofstream* File)
{
	int version = 1;
	int v;
	File->write((char*)&version, sizeof (int));

	File->write((char*)&Image_Matrix.X, sizeof (int));
	File->write((char*)&Image_Matrix.Y, sizeof (int));
	File->write((char*)&Image_Matrix.Z, sizeof (int));

	for( int i=0; i<Image_Matrix.X; i++)
	for( int j=0; j<Image_Matrix.Y; j++)
	for( int k=0; k<Image_Matrix.Z; k++)
	{
		v = Image_Matrix.get_value(i,j,k);
		File->write((char*)&v, sizeof (int));
	}

	File->write((char*)&Max_Intensity, sizeof (double));

	File->write((char*)&Min_X, sizeof (int));
	File->write((char*)&Max_X, sizeof (int));
	File->write((char*)&Min_Y, sizeof (int));
	File->write((char*)&Max_Y, sizeof (int));
	File->write((char*)&Min_Z, sizeof (int));
	File->write((char*)&Max_Z, sizeof (int));

	File->write((char*)&X_Resolution, sizeof (double));
	File->write((char*)&Y_Resolution, sizeof (double));
	File->write((char*)&Z_Resolution, sizeof (double));

}
//---------------------------------------------------------------------------

int Image_Matrix_Class::load_object_from_stream(ifstream* File)
{
	int version,v;
	long X,Y,Z;

	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{

	File->read((char*)&X, sizeof (int));
	File->read((char*)&Y, sizeof (int));
	File->read((char*)&Z, sizeof (int));

	allocate_image_matrix(X,Y,Z);

	for( int i=0; i<X; i++)
	for( int j=0; j<Y; j++)
	for( int k=0; k<Z; k++)
	{
		File->read((char*)&v, sizeof (int));
		Image_Matrix.set_value(i,j,k,v);
	}

	File->read((char*)&Max_Intensity, sizeof (double));

	File->read((char*)&Min_X, sizeof (int));
	File->read((char*)&Max_X, sizeof (int));
	File->read((char*)&Min_Y, sizeof (int));
	File->read((char*)&Max_Y, sizeof (int));
	File->read((char*)&Min_Z, sizeof (int));
	File->read((char*)&Max_Z, sizeof (int));

	File->read((char*)&X_Resolution, sizeof (double));
	File->read((char*)&Y_Resolution, sizeof (double));
	File->read((char*)&Z_Resolution, sizeof (double));

	}
}
//---------------------------------------------------------------------------


