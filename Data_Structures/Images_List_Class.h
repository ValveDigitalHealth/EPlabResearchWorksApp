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

#ifndef Images_List_ClassH
#define Images_List_ClassH
//---------------------------------------------------------------------------

class Images_List_Class
{
	public:

	Images_List_Class();

	std::vector <Image_2D_Class> Images_2D_List;

	int Current_Fluoro_Image;

	void update_data_set(); // need to be called after each image modifiction/addition/removal/load

	int get_max_image_id();

	double Fluoro_Pixel_Size_In_mm;

	double Fluoro_Image_Position_X_Small[2];
	double Fluoro_Image_Position_Y_Small[2];

	int First_Image_For_Registration_Id;
	int Second_Image_For_Registration_Id;

	bool current_fluoro_image_valid();

	int get_registration_point_image_id(int Which_Reg_Point);

	void ijk_to_xyz(int i, int j, int Slice, double* x, double* y, double* z);
	void xyz_to_ijk(double x, double y, double z, int* i, int* j, int* Slice);


	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);


};

#endif
