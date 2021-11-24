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

#include "Image_2D.h"
#include "glu.h"

#include <fstream>

#include <Jpeg.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma hdrstop

Image_2D_Class::Image_2D_Class()
{
	Identificator = 0;
	Displayed_Flag = true;

	Fluoro_Image_2D_scaling_points_X[0]=-1;
	Fluoro_Image_2D_scaling_points_Y[0]=-1;
	Fluoro_Image_2D_scaling_points_X[1]=-1;
	Fluoro_Image_2D_scaling_points_Y[1]=-1;

	Point_A_Located = false;
	Point_B_Located = false;
	Point_C_Located = false;
	Point_Rov_Located = false;

	LAO_RAO = 50;

	Slice_Thickness = 1;

	Distance_from_Emiter = 0;

//	Fluoro_Image_Bimap = new Graphics::TBitmap;
}

//---------------------------------------------------------------------------

Image_2D_Class::~Image_2D_Class()
{
// d
//	delete Fluoro_Image_Bimap;
}
//---------------------------------------------------------------------------

int Image_2D_Class::save_object_to_stream(ofstream* File)
{
	int version = 1;

	long Size;

	File->write((char*)&version, sizeof (int));

	//---------------------------------------------------------

	Utils.save_String_to_File(File, File_Name);

	File->write((char*)&Identificator, sizeof (int));

	File->write((char*)&LAO_RAO, sizeof (double));
	File->write((char*)&Slice_Thickness, sizeof (double));

	File->write((char*)&Registered_Status, sizeof (int));
	Registration_Point_A_ij.save_object_to_stream(File);
	Registration_Point_B_ij.save_object_to_stream(File);
	Registration_Point_C_ij.save_object_to_stream(File);
	Registration_Point_Rov_ij.save_object_to_stream(File);

	File->write((char*)&Point_A_Located, sizeof (bool));
	File->write((char*)&Point_B_Located, sizeof (bool));
	File->write((char*)&Point_C_Located, sizeof (bool));
	File->write((char*)&Point_Rov_Located, sizeof (bool));

	Line_A.save_object_to_stream(File);
	Line_B.save_object_to_stream(File);
	Line_C.save_object_to_stream(File);
	Line_Rov.save_object_to_stream(File);

	File->write((char*)&Catheter_Ptr, sizeof (int));

	File->write((char*)&Displayed_Flag, sizeof (bool));


	File->write((char*)&Fluoro_Image_Width, sizeof (int));
	File->write((char*)&Fluoro_Image_Height, sizeof (int));
	Size = Fluoro_Image_Data_vector.size();
	File->write((char*)&Size, sizeof (long));
	for(long i=0;i<Size;i++)
	File->write((char*)&Fluoro_Image_Data_vector[i], sizeof (unsigned char));

	File->write((char*)&Fluoro_Image_Width_Small, sizeof (int));
	File->write((char*)&Fluoro_Image_Height_Small, sizeof (int));
	Size = Fluoro_Image_Data_Small_vector.size();
	File->write((char*)&Size, sizeof (long));
	for(long i=0;i<Size;i++)
	File->write((char*)&Fluoro_Image_Data_Small_vector[i], sizeof (unsigned char));

	File->write((char*)&Fluoro_Image_2D_scaling_points_X[0], sizeof (int));
	File->write((char*)&Fluoro_Image_2D_scaling_points_X[1], sizeof (int));
	File->write((char*)&Fluoro_Image_2D_scaling_points_Y[0], sizeof (int));
	File->write((char*)&Fluoro_Image_2D_scaling_points_Y[1], sizeof (int));

	File->write((char*)&X_LU, sizeof (int));
	File->write((char*)&Y_LU, sizeof (int));
	File->write((char*)&X_RL, sizeof (int));
	File->write((char*)&Y_RL, sizeof (int));

}
//---------------------------------------------------------------------------

int Image_2D_Class::load_object_from_stream(ifstream* File)
{
	long Size;
	int version;
	unsigned char usc;

	File->read((char*)&version, sizeof (int));

	//---------------------------------------------------------------------
	if( version == 1 )
	//---------------------------------------------------------------------
	{

	File_Name = Utils.load_String_from_File(File);

	File->read((char*)&Identificator, sizeof (int));

	File->read((char*)&LAO_RAO, sizeof (double));
	File->read((char*)&Slice_Thickness, sizeof (double));

	File->read((char*)&Registered_Status, sizeof (int));

	Registration_Point_A_ij.load_object_from_stream(File);
	Registration_Point_B_ij.load_object_from_stream(File);
	Registration_Point_C_ij.load_object_from_stream(File);
	Registration_Point_Rov_ij.load_object_from_stream(File);

	File->read((char*)&Point_A_Located, sizeof (bool));
	File->read((char*)&Point_B_Located, sizeof (bool));
	File->read((char*)&Point_C_Located, sizeof (bool));
	File->read((char*)&Point_Rov_Located, sizeof (bool));

	Line_A.load_object_from_stream(File);
	Line_B.load_object_from_stream(File);
	Line_C.load_object_from_stream(File);
	Line_Rov.load_object_from_stream(File);

	File->read((char*)&Catheter_Ptr, sizeof (int));

	File->read((char*)&Displayed_Flag, sizeof (bool));

	File->read((char*)&Fluoro_Image_Width, sizeof (int));
	File->read((char*)&Fluoro_Image_Height, sizeof (int));
	File->read((char*)&Size, sizeof (long));
	Fluoro_Image_Data_vector.clear();
	Fluoro_Image_Data_vector.assign(Size,usc);
	for(long i=0;i<Size;i++)
	File->read((char*)&Fluoro_Image_Data_vector[i], sizeof (unsigned char));

	File->read((char*)&Fluoro_Image_Width_Small, sizeof (int));
	File->read((char*)&Fluoro_Image_Height_Small, sizeof (int));
	File->read((char*)&Size, sizeof (long));
	Fluoro_Image_Data_Small_vector.clear();
	Fluoro_Image_Data_Small_vector.assign(Size,usc);
	for(long i=0;i<Size;i++)
	File->read((char*)&Fluoro_Image_Data_Small_vector[i], sizeof (unsigned char));

	File->read((char*)&Fluoro_Image_2D_scaling_points_X[0], sizeof (int));
	File->read((char*)&Fluoro_Image_2D_scaling_points_X[1], sizeof (int));
	File->read((char*)&Fluoro_Image_2D_scaling_points_Y[0], sizeof (int));
	File->read((char*)&Fluoro_Image_2D_scaling_points_Y[1], sizeof (int));

	File->read((char*)&X_LU, sizeof (int));
	File->read((char*)&Y_LU, sizeof (int));
	File->read((char*)&X_RL, sizeof (int));
	File->read((char*)&Y_RL, sizeof (int));

	} // ver1

}
//---------------------------------------------------------------------------

int Image_2D_Class::read_fluoro_image()
{
	TJPEGImage* Fluoro_JPEG_Image;
	Fluoro_JPEG_Image = new TJPEGImage;
	GLubyte Glbyte;

	Graphics::TBitmap* Fluoro_Image_Bitmap;
	Fluoro_Image_Bitmap = new Graphics::TBitmap;

	AnsiString Extension = File_Name.SubString( File_Name.Length()-2,3);

	if( Extension.LowerCase() == "jpg" || Extension.LowerCase() == "jpeg")
	{
		Fluoro_JPEG_Image->LoadFromFile(File_Name);

		// Assign image data to bitmap (its not accessible in JPEG object)
		Fluoro_Image_Bitmap->Assign(Fluoro_JPEG_Image);
	}
	else
	if( Extension.LowerCase() == "bmp" )
	{
		Fluoro_Image_Bitmap->LoadFromFile(File_Name);
	}
	else
	{
		return -1;  // put error message here later on
	}

	//------------------------------------------------------------------------
	// Create Image data array
	//------------------------------------------------------------------------
	Fluoro_Image_Data_vector.clear();
	Fluoro_Image_Width = Fluoro_Image_Bitmap->Width;
	Fluoro_Image_Height = Fluoro_Image_Bitmap->Height;
	Fluoro_Image_Data_vector.assign(Fluoro_Image_Height*Fluoro_Image_Width*3,Glbyte);

	Fluoro_Image_Bitmap->HandleType=bmDIB;    // allows use of ScanLine
	Fluoro_Image_Bitmap->PixelFormat=pf32bit; // 32bit the same as int so we can use int* for pixels pointer

	DWORD *BLine;
	for(int j=0;j<Fluoro_Image_Bitmap->Height;j++)
	{
		BLine = (DWORD*)Fluoro_Image_Bitmap->ScanLine[j];

		for (long i=0; i < Fluoro_Image_Width; i++)
		{

		Fluoro_Image_Data_vector[3*((Fluoro_Image_Height-1-j)*Fluoro_Image_Width+i)+0] =
			(GLubyte) GetBValue(BLine[i]);

		Fluoro_Image_Data_vector[3*((Fluoro_Image_Height-1-j)*Fluoro_Image_Width+i)+1] =
			(GLubyte) GetGValue(BLine[i]);

		Fluoro_Image_Data_vector[3*((Fluoro_Image_Height-1-j)*Fluoro_Image_Width+i)+2] =
			(GLubyte) GetRValue(BLine[i]);
		}
	}

	//------------------------------------------------------------------------
	// Create smaller version of Image data array (for display)
	//------------------------------------------------------------------------
	Fluoro_Image_Data_Small_vector.clear();
	Fluoro_Image_Width_Small = 150;
	if( Fluoro_Image_Width!=0)
	Fluoro_Image_Height_Small = 150;//*Fluoro_Image_Height/Fluoro_Image_Width;

	Fluoro_Image_Data_Small_vector.assign(Fluoro_Image_Height_Small*Fluoro_Image_Width_Small*3,Glbyte);
	double CoefX,CoefY;

	// read pixel data from bitmap to Fluoro_Image_Data array
	for (long i = 0; i < Fluoro_Image_Width_Small; i++)
	for (long j = 0; j < Fluoro_Image_Height_Small; j++)
	{

	CoefX = (double)Fluoro_Image_Bitmap->Width/(double)Fluoro_Image_Width_Small;
	CoefY = (double)Fluoro_Image_Bitmap->Height/(double)Fluoro_Image_Height_Small;

	Fluoro_Image_Data_Small_vector[3*(j*Fluoro_Image_Width_Small+i)+0] =
		(GLubyte) GetRValue(Fluoro_Image_Bitmap->Canvas->Pixels[(int)(CoefX*i)]
															   [(int)(CoefY*j)]);

	Fluoro_Image_Data_Small_vector[3*(j*Fluoro_Image_Width_Small+i)+1] =
		(GLubyte) GetGValue(Fluoro_Image_Bitmap->Canvas->Pixels[(int)(CoefX*i)]
															   [(int)(CoefY*j)]);

	Fluoro_Image_Data_Small_vector[3*(j*Fluoro_Image_Width_Small+i)+2] =
		(GLubyte) GetBValue(Fluoro_Image_Bitmap->Canvas->Pixels[(int)(CoefX*i)]
															   [(int)(CoefY*j)]);
	}

	delete Fluoro_JPEG_Image;
	delete Fluoro_Image_Bitmap;

	return 1;
}

//---------------------------------------------------------------------------

/*
void Image_2D_Class::compute_3point_registration_line_parameters_no_perspective(int Which_Line,
							double Fluoro_Pixel_Size_In_mm) // 0-A, 1-B, 2-C
{
	Point_3D Point_A;
	Point_3D Point_B;
	Point_3D Point_C;
	Point_3D Point_Rov;
	double x,y,z;

	double Distance_of_Image_From_Patient_Centre = 30; // no need for that in fact...

	if( Which_Line==0)
	ij_to_xyz(Registration_Point_A_ij.x,Registration_Point_A_ij.y,
				&x,&y,&z,Distance_of_Image_From_Patient_Centre,Fluoro_Pixel_Size_In_mm);
	if( Which_Line==1)
	ij_to_xyz(Registration_Point_B_ij.x,Registration_Point_B_ij.y,
				&x,&y,&z,Distance_of_Image_From_Patient_Centre,Fluoro_Pixel_Size_In_mm);
	if( Which_Line==2)
	ij_to_xyz(Registration_Point_C_ij.x,Registration_Point_C_ij.y,
				&x,&y,&z,Distance_of_Image_From_Patient_Centre,Fluoro_Pixel_Size_In_mm);
	if( Which_Line==3)
	ij_to_xyz(Registration_Point_Rov_ij.x,Registration_Point_Rov_ij.y,
				&x,&y,&z,Distance_of_Image_From_Patient_Centre,Fluoro_Pixel_Size_In_mm);

	Point_A.x = x;
	Point_A.y = y;
	Point_A.z = z;

	ij_to_xyz(0,0,&x,&y,&z,Distance_of_Image_From_Patient_Centre,Fluoro_Pixel_Size_In_mm);
	Point_B.x = x;
	Point_B.y = y;
	Point_B.z = z;

	ij_to_xyz(3,3,&x,&y,&z,Distance_of_Image_From_Patient_Centre,Fluoro_Pixel_Size_In_mm);
	Point_C.x = x;
	Point_C.y = y;
	Point_C.z = z;


	if( Which_Line==0)
	Numerical_Algorithms.find_perpendicular_parametric_line_from_3_points(
		Point_A, // coordinates of the points
		Point_B, // line will go through point A
		Point_C, &Line_A );

	if( Which_Line==1)
	Numerical_Algorithms.find_perpendicular_parametric_line_from_3_points(
		Point_A, // coordinates of the points
		Point_B, // line will go through point A
		Point_C, &Line_B );

	if( Which_Line==2)
	Numerical_Algorithms.find_perpendicular_parametric_line_from_3_points(
		Point_A, // coordinates of the points
		Point_B, // line will go through point A
		Point_C, &Line_C );

	if( Which_Line==3)
	Numerical_Algorithms.find_perpendicular_parametric_line_from_3_points(
		Point_A, // coordinates of the points
		Point_B, // line will go through point A
		Point_C, &Line_Rov );
}
//---------------------------------------------------------------------------

void Image_2D_Class::compute_3point_registration_line_parameters_with_perspective
		(int Which_Line,double Fluoro_Pixel_Size_In_mm) // 0-A, 1-B, 2-C
{
// this one assumes that line should pass through emitter

	Point_3D Point_A;
	Point_3D Point_B;
	Point_3D Point_C;
	Point_3D Point_Rov;
	double x,y,z;

	if( Which_Line==0)
	ij_to_xyz(Registration_Point_A_ij.x,Registration_Point_A_ij.y,
				&x,&y,&z,0.5*Distance_from_Emiter,Fluoro_Pixel_Size_In_mm);
	if( Which_Line==1)
	ij_to_xyz(Registration_Point_B_ij.x,Registration_Point_B_ij.y,
				&x,&y,&z,0.5*Distance_from_Emiter,Fluoro_Pixel_Size_In_mm);
	if( Which_Line==2)
	ij_to_xyz(Registration_Point_C_ij.x,Registration_Point_C_ij.y,
				&x,&y,&z,0.5*Distance_from_Emiter,Fluoro_Pixel_Size_In_mm);
	if( Which_Line==3)
	ij_to_xyz(Registration_Point_Rov_ij.x,Registration_Point_Rov_ij.y,
				&x,&y,&z,0.5*Distance_from_Emiter,Fluoro_Pixel_Size_In_mm);

	Point_A.x = x;
	Point_A.y = y;
	Point_A.z = z;

	// now calculate x,y,z of emitter
	ij_to_xyz(0.5*Fluoro_Image_Width,0.5*Fluoro_Image_Height,&x,&y,&z,
			0.5*Distance_from_Emiter,Fluoro_Pixel_Size_In_mm);
	Point_B.x = -x;
	Point_B.y = -y;
	Point_B.z = -z;

	if( Which_Line==0)
	Numerical_Algorithms.find_line_pasing_two_points_in_3D(
		Point_A, // coordinates of the points
		Point_B, // line will go through point A
		&Line_A );

	if( Which_Line==1)
	Numerical_Algorithms.find_line_pasing_two_points_in_3D(
		Point_A, // coordinates of the points
		Point_B, // line will go through point A
		&Line_B );

	if( Which_Line==2)
	Numerical_Algorithms.find_line_pasing_two_points_in_3D(
		Point_A, // coordinates of the points
		Point_B, // line will go through point A
		&Line_C );

	if( Which_Line==3)
	Numerical_Algorithms.find_line_pasing_two_points_in_3D(
		Point_A, // coordinates of the points
		Point_B, // line will go through point A
		&Line_Rov );
}

//---------------------------------------------------------------------------
*/



