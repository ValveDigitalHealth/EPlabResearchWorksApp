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
/***************************************************************************

    file name            : Image_Matrix.h

    begin                : Jan 01 2007
    copyright            : (C) 2007 by Pawel Kuklik
    email                : kuklik@if.pw.edu.pl

	description          : Image_Matrix_Class is used in 1st step of the segmentation
				- selecting Volume of Interest. Holds all image loaded
				with lower resolution to save memory and speed up processing.

*****************************************************************************/
//---------------------------------------------------------------------------

#include "Matrix_3D_double.h"

#include <fstream>

#ifndef Image_MatrixH
#define Image_MatrixH
//---------------------------------------------------------------------------

class Image_Matrix_Class
{

	public:

	Image_Matrix_Class();
	~Image_Matrix_Class();

	//-------------------------------------------------------------------------
	// Structure holding intensities of the 3D image
	//-------------------------------------------------------------------------
	Matrix_3D_double Image_Matrix;

	//-------------------------------------------------------------------------
	// Functions allocating and deallocating Image_Matrix
	//-------------------------------------------------------------------------
	int allocate_image_matrix(int new_Image_X,int new_Image_Y,int new_Image_Z);

	//-------------------------------------------------------------------------
	// Max intensity in the image, and functions calculating this value.
	// Used in display to scale palette.
	//-------------------------------------------------------------------------
	double Max_Intensity;
	int calculate_max_intensity();

	//-------------------------------------------------------------------------
	// Variables defining Volume of Interest (which part of the image will go
	// to segmentation phase.
	//-------------------------------------------------------------------------
	int Min_X,Max_X;
	int Min_Y,Max_Y;
	int Min_Z,Max_Z;

	//-------------------------------------------------------------------------
	// Resolution of the image.
	//-------------------------------------------------------------------------
	double X_Resolution; // X resolution [mm]
	double Y_Resolution; // Y resolution [mm]
	double Z_Resolution; // slice thickness [mm]

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

};

#endif

