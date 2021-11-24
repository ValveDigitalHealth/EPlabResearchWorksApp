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

#include<vcl.h>
#include<vector>

#include"Point_2D_Class.h"
#include"Point_3D_Class.h"
#include"Parametric_3D_Line_Class.h"
#include "Utils.h"

#ifndef Image_2DH
#define Image_2DH
//---------------------------------------------------------------------------
class Image_2D_Class
{
	public:


	Image_2D_Class();
	~Image_2D_Class();

	int Identificator;

	AnsiString File_Name;

	TDateTime Capture_Time;
	AnsiString Label;

	double Slice_Thickness;

    double Y_Coordinate; // in this implementation, slices are in XZ plane

	double LAO_RAO; // in degrees [0-360] positive if LAO, negative if RAO

	double Distance_from_Emiter; // in mm. ADD TO SAVE/LOAD!

	double Image_Width_mm; // in mm. ADD TO SAVE/LOAD!
	double Image_Height_mm; // in mm. ADD TO SAVE/LOAD!

	int Registered_Status; // 0 if not used in registartion, 1 if first, 2 if second

	Point_2D Registration_Point_A_ij;
	Point_2D Registration_Point_B_ij;
	Point_2D Registration_Point_C_ij;
	Point_2D Registration_Point_Rov_ij;
	bool Point_A_Located;
	bool Point_B_Located;
	bool Point_C_Located;
	bool Point_Rov_Located;

	Parametric_3D_line Line_A;
	Parametric_3D_line Line_B;
	Parametric_3D_line Line_C;
	Parametric_3D_line Line_Rov;

/*
	void compute_3point_registration_line_parameters_with_perspective(int Which_Line,
								double Fluoro_Pixel_Size_In_mm); // 0-A, 1-B, 2-C

	void compute_3point_registration_line_parameters_no_perspective(int Which_Line,
								double Fluoro_Pixel_Size_In_mm); // 0-A, 1-B, 2-C
*/

	int Catheter_Ptr;

	bool Displayed_Flag;

//	double Fluoro2D_Pixel_Scale;

	int read_fluoro_image();

	int Fluoro_Image_Width;
	int Fluoro_Image_Height;
	std::vector <unsigned char> Fluoro_Image_Data_vector;

	int Fluoro_Image_Width_Small;
	int Fluoro_Image_Height_Small;
	std::vector <unsigned char> Fluoro_Image_Data_Small_vector;

	// two points used to scale the image [in pixel units]
	int Fluoro_Image_2D_scaling_points_X[2],Fluoro_Image_2D_scaling_points_Y[2];

	// Region of interest for display
	int X_LU,Y_LU,X_RL,Y_RL;

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

	Utils_Tools Utils;
};

//---------------------------------------------------------------------------

#endif
