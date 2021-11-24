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

	file name            : Segmented_Image.h

	begin                : Jan 01 2007
	copyright            : (C) 2007 by Pawel Kuklik
	email                : kuklik@if.pw.edu.pl

	description          : Voxel,Segment_Seed and Segmented_Image_Class clasees
				are used to hold image during segmentation phase.
				Voxel: represents single voxel of the image.
				Segment_Seed: represents single seed in the image,
						used by Region Growing algorithm.

				Segmented_Image_Class: Holds all loaded image and
						functions used in image processing.
*****************************************************************************/
//---------------------------------------------------------------------------

#ifndef OK_RESULT
#define OK_RESULT 1
#endif

#ifndef FAILURE
#define FAILURE 0
#endif

// Number of different types of defined segments

#ifndef NOT_SEGMENTED
#define NOT_SEGMENTED 0
#endif

#ifndef BLOODPOOL_SEGMENT
#define BLOODPOOL_SEGMENT 1
#endif

#ifndef LA_SEGMENT
#define LA_SEGMENT 2
#endif

#ifndef RA_SEGMENT
#define RA_SEGMENT 3
#endif

#ifndef LV_SEGMENT
#define LV_SEGMENT 4
#endif

#ifndef RV_SEGMENT
#define RV_SEGMENT 5
#endif

#ifndef AORTA_SEGMENT
#define AORTA_SEGMENT 6
#endif

#ifndef CS_SEGMENT
#define CS_SEGMENT 7
#endif

#ifndef SPINE_SEGMENT
#define SPINE_SEGMENT 8
#endif

#ifndef CUSTOM1_SEGMENT
#define CUSTOM1_SEGMENT 9
#endif

#ifndef SEGMENTS_TYPES_NUMBER
#define SEGMENTS_TYPES_NUMBER 10 // = max id of segment +1
#endif


#include <vector>

#ifndef Segmented_ImageH
#define Segmented_ImageH

//---------------------------------------------------------------------------

class Voxel
{
	// Class representing single voxel of the image.

	public:

	//-------------------------------------------------------------------------
	// Intensity of the voxel
	//-------------------------------------------------------------------------
	int Intensity;

	//-------------------------------------------------------------------------
	// Value used during image filtering (to keep original intensities)
	//-------------------------------------------------------------------------
	int Old_Intensity;

	//-------------------------------------------------------------------------
	// Id of the segment to which voxel belongs (e.g.NOT_SEGMENTED, BLOODPOOL_SEGMENT, etc.)
	//-------------------------------------------------------------------------
	char Flag;

	//-------------------------------------------------------------------------
	// Flag used to mark voxels at the edge of the volume.
	// Used to find where to put reconstructed surface in 3D space (to geometrical cener
	// of the volume border voxels)
	// Values: GEO_FLAG_INTERIOR - if voxel is inside volume
	//         GEO_FLAG_BORDER - if voxel is at the border of the volume
	//-------------------------------------------------------------------------
	char Geometry_Flag;

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

};

//-------------------------------------------------------------------------------------

class Segment_Seed
{
	// Class represents single seed of the image. (Along with growed region)

	public:

	//-------------------------------------------------------------------------
	// Position of the seed in Image_Matrix[][][]
	//-------------------------------------------------------------------------
	int X,Y,Z;

	//-------------------------------------------------------------------------
	// Id of the segment which this seed denotes
	//-------------------------------------------------------------------------
	int Segment_Id;

	//-------------------------------------------------------------------------
	// Allocated pixels to segment during Region Growing algorithm
	//-------------------------------------------------------------------------
	std::vector <int> Xs;
	std::vector <int> Ys;
	std::vector <int> Zs;

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);
};


//-------------------------------------------------------------------------------------

class Segmented_Image_Class
{
	// Important class. It holds all image, seeds and reconstructed surface.

	public:

	Segmented_Image_Class();
	~Segmented_Image_Class();

	//-------------------------------------------------------------------------
	// Structure holds whole image (intensity and segment id of each voxel)
	//-------------------------------------------------------------------------
	std::vector<Voxel> Image_Matrix;

	//-------------------------------------------------------------------------
	// Size of the image (of the Image_Matrix object)
	//-------------------------------------------------------------------------
	int Image_X;
	int Image_Y;
	int Image_Z;

	//-------------------------------------------------------------------------
	// Resolution of the image in x,y,z directions in [mm]
	//-------------------------------------------------------------------------
	double X_Resolution;
	double Y_Resolution;
	double Z_Resolution;

    //-------------------------------------------------------------------------
	// Functions allocating and deallocating Image_Matrix
	//-------------------------------------------------------------------------
	int allocate_image_matrix(int new_Image_X,int new_Image_Y,int new_Image_Z);

	int clear_Image_Matrix();
	int clear_Image_Matrix_flags();

    //-------------------------------------------------------------------------
    // Max intensity in the image. Its used in display to scale the palette to
    // cover all range of intensities in image.
    //-------------------------------------------------------------------------
    double Max_Intensity;
	int calculate_max_intensity();

    //-------------------------------------------------------------------------
    // Function sets Flag of each voxel to given Flag_Id.
	// Used to clear segmentation.
    //-------------------------------------------------------------------------
    int set_flag(char Flag_Id);

    //-------------------------------------------------------------------------
    // Min and max thresholds for intensity. Voxels with values between are
    // marked as bloodpool
	//-------------------------------------------------------------------------
    double Bloodpool_Intensity_Threshold_Min;
	double Bloodpool_Intensity_Threshold_Max;

    //-------------------------------------------------------------------------
    // Sets flag of each voxel with intensity between Bloodpool_Intensity_Threshold_Min
    // and Bloodpool_Intensity_Threshold_Max as BLOODPOOL_SEGMENT.
    // Called by Build_Bloodpool_Volume_ButtonClick() (when bloodpool volume is builded)
    //-------------------------------------------------------------------------
	int propagate_blood_pool_in_whole_volume();

	//-------------------------------------------------------------------------
    // Because propagate_blood_pool_in_whole_volume() is slow (whole image is processed)
    // this function was created to speed up bloodpool processing
    // by user (e.g. changing thresholds level which should be reflected on bloodpool
	// area on slices images)
    //-------------------------------------------------------------------------
    int propagate_blood_pool_in_selected_slices(int Slice_X,int Slice_Y, int Slice_Z);

	//-------------------------------------------------------------------------
	// Function finds border voxels and assigns them Geometry_Flag =1
	//-------------------------------------------------------------------------
	void calculate_geometry_flags();

	//-------------------------------------------------------------------------
	// Function filtering the image. Filters described in source code of the
	// function.
	// Remark: at the moment, there is no Undo implemented for filtering (memory
	// rasons).
	//-------------------------------------------------------------------------
	int filter_image(int Filter_Type);

	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	std::vector<Segment_Seed> Seeds_List;

	//-------------------------------------------------------------------------
	// UNDO implementation. save_flags_to_undo_stack() saves flags (segmentation
	// flags) to Old_Intensities_storage, load_flags_from_undo_stack() loads them.
	//-------------------------------------------------------------------------
	int save_flags_to_undo_stack();
	int load_flags_from_undo_stack();

	//-------------------------------------------------------------------------
	// Vector holding flags of all 3D image. Used to impelemnt undo.
	//-------------------------------------------------------------------------
	std::vector<char*> Old_Intensities_storage;

	//-------------------------------------------------------------------------
	// Variable controlling how many Undo steps are possible.
	// Value set in constructor.
	//-------------------------------------------------------------------------
	int Max_Undo_Steps_Number;

	//-------------------------------------------------------------------------
	// Flags controlling whether given segment is displayed.
	//-------------------------------------------------------------------------
	bool Segments_Display_Flag[1000];

	//-------------------------------------------------------------------------
	// Flags controlling whether given segment is locked. Voxels of locked segment can not
	// change flag value (its handy when one chamber is segmented; it may be locked,
	// turned off from display and user may focus on segmentation of next chamber).
	//-------------------------------------------------------------------------
	bool Segments_Locked_Flag[1000];

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

};

#endif
