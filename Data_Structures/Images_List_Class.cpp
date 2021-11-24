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

#include <fstream>

#include "Images_List_Class.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Images_List_Class::Images_List_Class()
{
	Fluoro_Pixel_Size_In_mm = 0.1;

	Fluoro_Image_Position_X_Small[0] = 0;
	Fluoro_Image_Position_Y_Small[0] = 0;

	Fluoro_Image_Position_X_Small[1] = 200;
	Fluoro_Image_Position_Y_Small[1] = 200;

	First_Image_For_Registration_Id = -1;
	Second_Image_For_Registration_Id = -1;

	Current_Fluoro_Image = 0;
}

//---------------------------------------------------------------------------

bool Images_List_Class::current_fluoro_image_valid()
{
	if( Current_Fluoro_Image >= 0 &&
		Current_Fluoro_Image < Images_2D_List.size() )
	return true;
	else
	return false;
}

//---------------------------------------------------------------------------

int Images_List_Class::save_object_to_stream(ofstream* File)
{
	int version = 1;

	File->write((char*)&version, sizeof (int));

	int Items_Number = Images_2D_List.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(int i=0;i<Items_Number;i++)
	Images_2D_List[i].save_object_to_stream(File);

	File->write((char*)&Current_Fluoro_Image, sizeof (int));

	File->write((char*)&Fluoro_Pixel_Size_In_mm, sizeof (double));

	File->write((char*)&Fluoro_Image_Position_X_Small[0], sizeof (double));
	File->write((char*)&Fluoro_Image_Position_X_Small[1], sizeof (double));

	File->write((char*)&Fluoro_Image_Position_Y_Small[0], sizeof (double));
	File->write((char*)&Fluoro_Image_Position_Y_Small[1], sizeof (double));

	File->write((char*)&First_Image_For_Registration_Id, sizeof (int));

	File->write((char*)&Second_Image_For_Registration_Id, sizeof (int));
}

//---------------------------------------------------------------------------------

int Images_List_Class::load_object_from_stream(ifstream* File)
{
	int Items_Number;

	int version;
	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{

	File->read((char*)&Items_Number, sizeof (int));
	Image_2D_Class Image_2D_Class_item;
	Images_2D_List.clear();
	Images_2D_List.assign(Items_Number,Image_2D_Class_item);
	for(int i=0;i<Items_Number;i++)
	Images_2D_List[i].load_object_from_stream(File);

	File->read((char*)&Current_Fluoro_Image, sizeof (int));

	File->read((char*)&Fluoro_Pixel_Size_In_mm, sizeof (double));

	File->read((char*)&Fluoro_Image_Position_X_Small[0], sizeof (double));
	File->read((char*)&Fluoro_Image_Position_X_Small[1], sizeof (double));

	File->read((char*)&Fluoro_Image_Position_Y_Small[0], sizeof (double));
	File->read((char*)&Fluoro_Image_Position_Y_Small[1], sizeof (double));

	File->read((char*)&First_Image_For_Registration_Id, sizeof (int));

	File->read((char*)&Second_Image_For_Registration_Id, sizeof (int));

	}
}
//---------------------------------------------------------------------------------

int Images_List_Class::get_registration_point_image_id(int Which_Reg_Point)
{
	for(int i=0;i<Images_2D_List.size();i++)
	{
		if (Which_Reg_Point == 0)
		if( Images_2D_List[i].Point_A_Located )
		return i;

		if (Which_Reg_Point == 1)
		if( Images_2D_List[i].Point_B_Located )
		return i;

		if (Which_Reg_Point == 2)
		if( Images_2D_List[i].Point_C_Located )
		return i;
	}

    return -1;
}
//---------------------------------------------------------------------------------

void Images_List_Class::update_data_set()
{
	if( Images_2D_List.size() > 0 )
	{

	// compute range of Y coordinates
	double Slice_Y=0;
	for(int s=0;s<=Images_2D_List.size();s++)
	Slice_Y += Images_2D_List[s].Slice_Thickness;

	Images_2D_List[0].Y_Coordinate = -0.5*Slice_Y; // so that images are centered in xyz system

	Slice_Y = Images_2D_List[0].Y_Coordinate + 0.5*Images_2D_List[0].Slice_Thickness;
	for(int s=1;s<=Images_2D_List.size();s++)
	{
		Slice_Y += 0.5*Images_2D_List[s].Slice_Thickness;
		Images_2D_List[s].Y_Coordinate = Slice_Y;
		Slice_Y += 0.5*Images_2D_List[s].Slice_Thickness;
	}
	}
}

//---------------------------------------------------------------------------------

int Images_List_Class::get_max_image_id()
{
	int Max_Id = 0;
	for(int s=0;s<Images_2D_List.size();s++)
	if( Images_2D_List[s].Identificator > Max_Id )
	Max_Id = Images_2D_List[s].Identificator;

    return Max_Id;
}
//---------------------------------------------------------------------------------

void Images_List_Class::xyz_to_ijk(double x, double y, double z, int* i, int* j, int* Slice)
{
	if( Images_2D_List.size() >= 0 )
	if( Fluoro_Pixel_Size_In_mm != 0 )
	{

	double Min_Dist = 100000;
	int Min_Dist_Ptr = -1;
	for(int s=0;s<Images_2D_List.size();s++)
	if( fabs(Images_2D_List[s].Y_Coordinate - y) < Min_Dist )
	{
		Min_Dist = fabs(Images_2D_List[s].Y_Coordinate - y);
		Min_Dist_Ptr = s;
	}

	Slice[0] = Min_Dist_Ptr;

	if( Slice[0] >= 0 && Slice[0] < Images_2D_List.size() )
	{
	i[0] = -(x/Fluoro_Pixel_Size_In_mm) + 0.5*Images_2D_List[Slice[0]].Fluoro_Image_Width;
	j[0] = -(z/Fluoro_Pixel_Size_In_mm )+ 0.5*Images_2D_List[Slice[0]].Fluoro_Image_Height;
	}

	}
}
//---------------------------------------------------------------------------------

void Images_List_Class::ijk_to_xyz(int i, int j, int Slice, double* x, double* y, double* z)
{
	if( Slice >= 0 && Slice < Images_2D_List.size() )
	if( Images_2D_List[Slice].Fluoro_Image_Height != 0 &&
		Images_2D_List[Slice].Fluoro_Image_Width != 0 )
	{
		x[0] = -(i-0.5*Images_2D_List[Slice].Fluoro_Image_Width)*Fluoro_Pixel_Size_In_mm;
		y[0] = Images_2D_List[Slice].Y_Coordinate;
		z[0] = -(j-0.5*Images_2D_List[Slice].Fluoro_Image_Height)*Fluoro_Pixel_Size_In_mm;
	}

	// and now do rotations/translations of choice further modyfiying xyz
/*
	if( Fluoro_Image_Height != 0 && Fluoro_Image_Width != 0 )
	{

	double fi = 2*M_PI*LAO_RAO/360.0;

	double x1 = (i-Fluoro_Image_Width/2.0)*Fluoro_Pixel_Size_In_mm;

	x[0] =  -x1*cos(fi) - Distance_of_Image_From_Patient_Centre*sin(fi);
	y[0] =  -x1*sin(fi) + Distance_of_Image_From_Patient_Centre*cos(fi);
	z[0] = -(j-Fluoro_Image_Height/2.0)*Fluoro_Pixel_Size_In_mm;

	}
*/

}

//---------------------------------------------------------------------------

