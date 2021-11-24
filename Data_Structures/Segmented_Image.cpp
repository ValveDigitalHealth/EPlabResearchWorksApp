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

#include <vcl.h>

#include "Segmented_Image.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <fstream>

//---------------------------------------------------------------------------

#pragma package(smart_init)



Segmented_Image_Class::Segmented_Image_Class()
{
	Max_Intensity = 1.0;

	X_Resolution = 1.0;
	Y_Resolution = 1.0;
	Z_Resolution = 1.0;

	Image_X = 2;
	Image_Y = 2;
	Image_Z = 2;

	allocate_image_matrix(Image_X,Image_Y,Image_Z);

	Max_Undo_Steps_Number = 5;
}

//----------------------------------------------------------------------------------------------

Segmented_Image_Class::~Segmented_Image_Class()
{
}

//----------------------------------------------------------------------------------------------

int Segmented_Image_Class::allocate_image_matrix(int new_Image_X,int new_Image_Y,int new_Image_Z)
{
	Image_X = new_Image_X;
	Image_Y = new_Image_Y;
	Image_Z = new_Image_Z;

	Voxel V;

	Image_Matrix.clear();
	Image_Matrix.assign(Image_X*Image_Y*Image_Z,V);

	if( Image_X > 10000 ) return FAILURE;
	if( Image_Y > 10000 ) return FAILURE;
	if( Image_Z > 10000 ) return FAILURE;

	clear_Image_Matrix();
}

//----------------------------------------------------------------------------------------------

int Segmented_Image_Class::clear_Image_Matrix()
{
	for(long i=0; i<Image_Matrix.size(); i++)
	{
		Image_Matrix[i].Intensity = 0;
		Image_Matrix[i].Old_Intensity = 0;
		Image_Matrix[i].Flag = NOT_SEGMENTED;
		Image_Matrix[i].Geometry_Flag = -1;
	}
}

//----------------------------------------------------------------------------------------------

int Segmented_Image_Class::clear_Image_Matrix_flags()
{
	for(long i=0; i<Image_Matrix.size(); i++)
	{
		Image_Matrix[i].Flag = NOT_SEGMENTED;
		Image_Matrix[i].Geometry_Flag = -1;
	}
}

//----------------------------------------------------------------------------------------------

int Segmented_Image_Class::calculate_max_intensity()
{
	Max_Intensity = 0;

	for(long i=0; i<Image_Matrix.size(); i++)
	if( Image_Matrix[i].Intensity > Max_Intensity )
		Max_Intensity = Image_Matrix[i].Intensity;

}

//-------------------------------------------------------------------------------------------------

int Segmented_Image_Class::propagate_blood_pool_in_selected_slices(int Slice_X,int Slice_Y, int Slice_Z)
{
/*
	// X slice
	for( int j=1; j<Image_Y-1; j++)
	for( int k=1; k<Image_Z-1; k++)
    if( Segments_Locked_Flag[Image_Matrix[Slice_X][j][k].Flag] == false )
    if(Image_Matrix[Slice_X][j][k].Intensity > Bloodpool_Intensity_Threshold_Min &&
       Image_Matrix[Slice_X][j][k].Intensity < Bloodpool_Intensity_Threshold_Max ) 
       Image_Matrix[Slice_X][j][k].Flag = BLOODPOOL_SEGMENT;
    else
       Image_Matrix[Slice_X][j][k].Flag = NOT_SEGMENTED;

    // geometry flags for x slice
    for( int j=1; j<Image_Y-1; j++)
    for( int k=1; k<Image_Z-1; k++)
    if( Image_Matrix[Slice_X][j][k].Flag == BLOODPOOL_SEGMENT )
	{
    if( Image_Matrix[Slice_X][j+1][k].Flag != BLOODPOOL_SEGMENT ||
		Image_Matrix[Slice_X][j-1][k].Flag != BLOODPOOL_SEGMENT ||
		Image_Matrix[Slice_X][j][k+1].Flag != BLOODPOOL_SEGMENT ||
        Image_Matrix[Slice_X][j][k-1].Flag != BLOODPOOL_SEGMENT )
	Image_Matrix[Slice_X][j][k].Geometry_Flag = GEO_FLAG_BORDER;
	else
    Image_Matrix[Slice_X][j][k].Geometry_Flag = GEO_FLAG_INTERIOR;
    }

    // Y slice
    for(long i=1; i<Image_X-1; i++)
	for( int k=1; k<Image_Z-1; k++)
    if( Segments_Locked_Flag[Image_Matrix[i][Slice_Y][k].Flag] == false )
    if(Image_Matrix[i][Slice_Y][k].Intensity > Bloodpool_Intensity_Threshold_Min &&
       Image_Matrix[i][Slice_Y][k].Intensity < Bloodpool_Intensity_Threshold_Max )
       Image_Matrix[i][Slice_Y][k].Flag = BLOODPOOL_SEGMENT;
    else
	Image_Matrix[i][Slice_Y][k].Flag = NOT_SEGMENTED;

    // geometry flags for x slice
    for(long i=1; i<Image_X-1; i++)
    for( int k=1; k<Image_Z-1; k++)
    if( Image_Matrix[i][Slice_Y][k].Flag == BLOODPOOL_SEGMENT )
	{
	if( Image_Matrix[i+1][Slice_Y][k].Flag != BLOODPOOL_SEGMENT ||
        Image_Matrix[i-1][Slice_Y][k].Flag != BLOODPOOL_SEGMENT ||
        Image_Matrix[i][Slice_Y][k+1].Flag != BLOODPOOL_SEGMENT ||
		Image_Matrix[i][Slice_Y][k-1].Flag != BLOODPOOL_SEGMENT )
	Image_Matrix[i][Slice_Y][k].Geometry_Flag = GEO_FLAG_BORDER;
	else
    Image_Matrix[i][Slice_Y][k].Geometry_Flag = GEO_FLAG_INTERIOR;
    }

    // Z slice
	for(long i=1; i<Image_X-1; i++)
	for( int j=1; j<Image_Y-1; j++)
    if( Segments_Locked_Flag[Image_Matrix[i][j][Slice_Z].Flag] == false )
    if(Image_Matrix[i][j][Slice_Z].Intensity > Bloodpool_Intensity_Threshold_Min &&
       Image_Matrix[i][j][Slice_Z].Intensity < Bloodpool_Intensity_Threshold_Max )
	   Image_Matrix[i][j][Slice_Z].Flag = BLOODPOOL_SEGMENT;
	else
    Image_Matrix[i][j][Slice_Z].Flag = NOT_SEGMENTED;

    // geometry flags for x slice
    for(long i=1; i<Image_X-1; i++)
    for( int j=1; j<Image_Y-1; j++)
    if( Image_Matrix[i][j][Slice_Z].Flag == BLOODPOOL_SEGMENT )
	{
    if( Image_Matrix[i+1][j][Slice_Z].Flag != BLOODPOOL_SEGMENT ||
        Image_Matrix[i-1][j][Slice_Z].Flag != BLOODPOOL_SEGMENT ||
        Image_Matrix[i][j+1][Slice_Z].Flag != BLOODPOOL_SEGMENT ||
        Image_Matrix[i][j-1][Slice_Z].Flag != BLOODPOOL_SEGMENT )
    Image_Matrix[i][j][Slice_Z].Geometry_Flag = GEO_FLAG_BORDER;
    else
	Image_Matrix[i][j][Slice_Z].Geometry_Flag = GEO_FLAG_INTERIOR;
	}

	return OK_RESULT;
*/
}
//-------------------------------------------------------------------------------------------------

int Segmented_Image_Class::propagate_blood_pool_in_whole_volume()
{
/*
	for(long i=0; i<Image_X; i++)
	for( int j=0; j<Image_Y; j++)
	for( int k=0; k<Image_Z; k++)
	Image_Matrix[i][j][k].Flag = NOT_SEGMENTED;

	for(long i=1; i<Image_X-1; i++)
	for( int j=1; j<Image_Y-1; j++)
	for( int k=1; k<Image_Z-1; k++)
	if( Segments_Locked_Flag[Image_Matrix[i][j][k].Flag] == false )
	if(Image_Matrix[i][j][k].Intensity > Bloodpool_Intensity_Threshold_Min &&
	   Image_Matrix[i][j][k].Intensity < Bloodpool_Intensity_Threshold_Max )
	   Image_Matrix[i][j][k].Flag = BLOODPOOL_SEGMENT;
	else
	   Image_Matrix[i][j][k].Flag = NOT_SEGMENTED;

	calculate_geometry_flags();

	return OK_RESULT;
*/

}
//-------------------------------------------------------------------------------------------------

void Segmented_Image_Class::calculate_geometry_flags()
{
/*
	for(long i=1; i<Image_X-1; i++)
	for( int j=1; j<Image_Y-1; j++)
	for( int k=1; k<Image_Z-1; k++)
	if( Image_Matrix[i][j][k].Flag != NOT_SEGMENTED )
	{

	if( Image_Matrix[i+1][j][k].Flag != Image_Matrix[i][j][k].Flag ||
		Image_Matrix[i-1][j][k].Flag != Image_Matrix[i][j][k].Flag ||
		Image_Matrix[i][j+1][k].Flag != Image_Matrix[i][j][k].Flag ||
		Image_Matrix[i][j-1][k].Flag != Image_Matrix[i][j][k].Flag ||
		Image_Matrix[i][j][k+1].Flag != Image_Matrix[i][j][k].Flag ||
		Image_Matrix[i][j][k-1].Flag != Image_Matrix[i][j][k].Flag )
	Image_Matrix[i][j][k].Geometry_Flag = GEO_FLAG_BORDER;
	else
	Image_Matrix[i][j][k].Geometry_Flag = GEO_FLAG_INTERIOR;

	}
*/
}

//-------------------------------------------------------------------------------------------------

int Segmented_Image_Class::set_flag(char Flag_Id)
{
	for(long i=0; i<Image_Matrix.size(); i++)
	Image_Matrix[i].Flag = Flag_Id;

	return OK_RESULT;
}

//-------------------------------------------------------------------------------------------------

int Segmented_Image_Class::filter_image(int Filter_Type)
{
/*
	long Intensity;
	int N=1; // Radius of neigborhood taken
    vector<int> List;
    double Factor;

    // Clear Old_Intensity variable which will store filtered value
    for(long i=0; i<Image_X; i++)
    for( int j=0; j<Image_Y; j++)
    for( int k=0; k<Image_Z; k++)
        Image_Matrix[i][j][k].Old_Intensity = Image_Matrix[i][j][k].Intensity;

    //--------------------------------------------------
    // Mean filter
    // Just average intensity from NxNxN cube
    //--------------------------------------------------
	if( Filter_Type == 0 )
    {

    Factor = pow(2*N+1,3); // number of elements taken to average

	for(long i=N; i<Image_X-N; i++)
    for( int j=N; j<Image_Y-N; j++)
    for( int k=N; k<Image_Z-N; k++)
	{

    Intensity = 0;

    for(long i1=-N; i1<=N; i1++)
    for( int j1=-N; j1<=N; j1++)
    for( int k1=-N; k1<=N; k1++)
    Intensity += Image_Matrix[i+i1][j+j1][k+k1].Intensity;

    Image_Matrix[i][j][k].Old_Intensity = Intensity/Factor;

    }
    }

	//--------------------------------------------------
    // Median filter
    // Median value from list of surrounding pixels intensities
    //--------------------------------------------------
    if( Filter_Type == 1 )
    for(long i=N; i<Image_X-N; i++)
    for( int j=N; j<Image_Y-N; j++)
    for( int k=N; k<Image_Z-N; k++)
    {

    List.clear();

    for(long i1=-N; i1<=N; i1++)
    for( int j1=-N; j1<=N; j1++)
    for( int k1=-N; k1<=N; k1++)
    List.push_back(Image_Matrix[i+i1][j+j1][k+k1].Intensity);

    sort(List.begin(),List.end());

	Image_Matrix[i][j][k].Old_Intensity = List[ (int)(0.5*List.size()) ];

    }

    //--------------------------------------------------
    // Gaussian filter
    // 5x5 Convolution filter with gaussian bell with sigma 1.4
    //--------------------------------------------------
    if( Filter_Type == 2 )
    {

    int Gauss_Filter[5][5];

    Gauss_Filter[0][0] = 2;
    Gauss_Filter[0][1] = 4;
    Gauss_Filter[0][2] = 5;
    Gauss_Filter[0][3] = 4;
    Gauss_Filter[0][4] = 2;

	Gauss_Filter[1][0] = 4;
	Gauss_Filter[1][1] = 9;
    Gauss_Filter[1][2] = 12;
    Gauss_Filter[1][3] = 9;
    Gauss_Filter[1][4] = 4;

    Gauss_Filter[2][0] = 5;
    Gauss_Filter[2][1] = 12;
	Gauss_Filter[2][2] = 15;
    Gauss_Filter[2][3] = 12;
    Gauss_Filter[2][4] = 5;

    Gauss_Filter[3][0] = 4;
    Gauss_Filter[3][1] = 9;
    Gauss_Filter[3][2] = 12;
    Gauss_Filter[3][3] = 9;
    Gauss_Filter[3][4] = 4;

    Gauss_Filter[4][0] = 2;
    Gauss_Filter[4][1] = 4;
    Gauss_Filter[4][2] = 5;
    Gauss_Filter[4][3] = 4;
    Gauss_Filter[4][4] = 2;

    double Weight = 1./159.;

    for(long i=2; i<Image_X-2; i++)
    for( int j=2; j<Image_Y-2; j++)
    for( int k=2; k<Image_Z-2; k++)
	{

    Intensity = 0;

    for(long i1=-2; i1<=2; i1++)
    for( int j1=-2; j1<=2; j1++)
    Intensity += Image_Matrix[i+i1][j+j1][k].Intensity*Gauss_Filter[2+i1][2+j1];

    Image_Matrix[i][j][k].Old_Intensity = Weight*Intensity;

    }

	}

    //--------------------------------------------------
    // "Conservative Smoothing" filter
    // Searches for min and max in neighbrhood, if central pixel is greater or
    // smaller than min, max, it takes corresponding extreme value
    //--------------------------------------------------
    int Min,Max;
    if( Filter_Type == 3 )
    for(long i=N; i<Image_X-N; i++)
    for( int j=N; j<Image_Y-N; j++)
    for( int k=N; k<Image_Z-N; k++)
    {

    Min = 1000000;
    Max = -1000000;

    for(long i1=-N; i1<=N; i1++)
    for( int j1=-N; j1<=N; j1++)
    for( int k1=-N; k1<=N; k1++)
    if( i1!=0 && j1!=0 && k1!=0 ) // dont take current pixel
    {
        if(Image_Matrix[i+i1][j+j1][k+k1].Intensity > Max)
        Max = Image_Matrix[i+i1][j+j1][k+k1].Intensity;

        if(Image_Matrix[i+i1][j+j1][k+k1].Intensity < Min)
        Min = Image_Matrix[i+i1][j+j1][k+k1].Intensity;
	}

    if( Image_Matrix[i][j][k].Intensity > Max)
		Image_Matrix[i][j][k].Old_Intensity = Max;

    if( Image_Matrix[i][j][k].Intensity < Min)
        Image_Matrix[i][j][k].Old_Intensity = Min;

	if( Image_Matrix[i][j][k].Intensity >= Min && Image_Matrix[i][j][k].Intensity <= Max)
        Image_Matrix[i][j][k].Old_Intensity = Image_Matrix[i][j][k].Intensity;

    }


    //--------------------------------------------------
    // Sharpening filter
    // 3x3 Convolution filter, laplacian type
	//--------------------------------------------------
    if( Filter_Type == 4 )
    {

    for(long i=N; i<Image_X-N; i++)
    for( int j=N; j<Image_Y-N; j++)
    for( int k=N; k<Image_Z-N; k++)
    Image_Matrix[i][j][k].Old_Intensity = Image_Matrix[i][j][k].Intensity;

    int Laplace_Filter[3][3];

    Laplace_Filter[0][0] = -1;
    Laplace_Filter[0][1] = -2;
	Laplace_Filter[0][2] = -1;

    Laplace_Filter[1][0] = -2;
	Laplace_Filter[1][1] = 12;
    Laplace_Filter[1][2] = -2;

    Laplace_Filter[2][0] = -1;
    Laplace_Filter[2][1] = -2;
    Laplace_Filter[2][2] = -1;

    double Weight = 1./16.;

    for(long i=2; i<Image_X-2; i++)
    for( int j=2; j<Image_Y-2; j++)
    for( int k=2; k<Image_Z-2; k++)
	{

    Intensity = 0;

    for(long i1=-1; i1<=1; i1++)
    for( int j1=-1; j1<=1; j1++)
    Intensity += Image_Matrix[i+i1][j+j1][k].Intensity*Laplace_Filter[1+i1][1+j1];

    Image_Matrix[i][j][k].Old_Intensity += Weight*Intensity;

    }

    }

    //--------------------------------------------------
	// Dominant value filter
    // The most frequent intensity is taken
    //--------------------------------------------------
	if( Filter_Type == 5 )
    {

    int min,max,Histo_Size=12,max2;
    int* Histo = new int[Histo_Size];
    for (int i=0;i<Histo_Size;i++)
        Histo[i] = 0;

    for(long i=N; i<Image_X-N; i++)
    for( int j=N; j<Image_Y-N; j++)
	for( int k=N; k<Image_Z-N; k++)
    {

    List.clear();

    for(long i1=-N; i1<=N; i1++)
    for( int j1=-N; j1<=N; j1++)
    for( int k1=-N; k1<=N; k1++)
    List.push_back(Image_Matrix[i+i1][j+j1][k+k1].Intensity);

    // find min max
	min = 10000;
    max = -10000;
    for (int i=0;i<List.size();i++)
    {
        if( List[i] > max )
        max = List[i];

        if( List[i] < min )
        min = List[i];
	}

    if( max == min )
    max = min + 1;

    for (int i=0;i<Histo_Size;i++)
        Histo[i] = 0;

	// histogram intensities
    for (int i=0;i<List.size();i++)
    Histo[(int)((Histo_Size-1)*(List[i]-min)/(double)(max-min))]++;

    // find most frequent intensity
    max2 = 0;

    for (int i=0;i<Histo_Size;i++)
    if( Histo[i] > max2 )
    {
        max2 = Histo[i];
        Intensity = min+(max-min)*(double)i/(double)Histo_Size;
    }

    Image_Matrix[i][j][k].Old_Intensity = Intensity;

    } // for

    } // most freq filter



	// Replace image with filtered values
	for(long i=0; i<Image_X; i++)
	for( int j=0; j<Image_Y; j++)
	for( int k=0; k<Image_Z; k++)
	Image_Matrix[i][j][k].Intensity = Image_Matrix[i][j][k].Old_Intensity;

	calculate_max_intensity();

	return OK_RESULT;
*/
}

//-------------------------------------------------------------------------------------------------

int Segmented_Image_Class::save_flags_to_undo_stack()
{
/*
	// check for size and remove last one if needed
	if( Old_Intensities_storage.size() > Max_Undo_Steps_Number )
	{
		delete [] Old_Intensities_storage[0];
		Old_Intensities_storage.erase(&Old_Intensities_storage[0]);
	}

	char* Flags_Table = new char[Image_X*Image_Y*Image_Z];

	for(long i=0; i<Image_X; i++)
	for( int j=0; j<Image_Y; j++)
	for( int k=0; k<Image_Z; k++)
	Flags_Table[ k*Image_X*Image_Y + j*Image_X + i ] = Image_Matrix[i][j][k].Flag;

	Old_Intensities_storage.push_back(Flags_Table);

	return OK_RESULT;
*/
}

//-------------------------------------------------------------------------------------------------

int Segmented_Image_Class::load_flags_from_undo_stack()
{
/*
	int i,j,k;

	if( Old_Intensities_storage.size() > 0 )
	{

	char* Flags_Table = Old_Intensities_storage[ Old_Intensities_storage.size()-1 ];

	for( long f=0; f<Image_X*Image_Y*Image_Z; f++)
	{

	k = (int)((double)(f)/(double)(Image_X*Image_Y));
	j = (int)((double)(f - k*Image_X*Image_Y)/(double)(Image_X));
	i = f - k*Image_X*Image_Y - j*Image_X;

	Image_Matrix[i][j][k].Flag  = Flags_Table[f];

	}

	delete [] Flags_Table;
	Old_Intensities_storage.erase(&Old_Intensities_storage[Old_Intensities_storage.size()-1]);

	return OK_RESULT;

	}
*/
}

//-------------------------------------------------------------------------------------------------

int Voxel::save_object_to_stream(ofstream* File)
{
	int version = 1;
	File->write((char*)&version, sizeof (int));

	File->write((char*)&Intensity, sizeof (int));
	File->write((char*)&Flag, sizeof (char));
	File->write((char*)&Geometry_Flag, sizeof (char));
}
//---------------------------------------------------------------------------

int Voxel::load_object_from_stream(ifstream* File)
{
	int version;
	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{

	File->read((char*)&Intensity, sizeof (int));
	File->read((char*)&Flag, sizeof (char));
	File->read((char*)&Geometry_Flag, sizeof (char));

	}
}
//---------------------------------------------------------------------------

int Segment_Seed::save_object_to_stream(ofstream* File)
{
	int version = 1;
	File->write((char*)&version, sizeof (int));

	File->write((char*)&X, sizeof (int));
	File->write((char*)&Y, sizeof (int));
	File->write((char*)&Z, sizeof (int));

	File->write((char*)&Segment_Id, sizeof (int));

	int Items_Number;

	Items_Number = Xs.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(int i=0;i<Items_Number;i++)
	File->write((char*)&Xs[i], sizeof (int));

	Items_Number = Ys.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(int i=0;i<Items_Number;i++)
	File->write((char*)&Ys[i], sizeof (int));

	Items_Number = Zs.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(int i=0;i<Items_Number;i++)
	File->write((char*)&Zs[i], sizeof (int));


}
//---------------------------------------------------------------------------

int Segment_Seed::load_object_from_stream(ifstream* File)
{
	int version;
	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{

	File->read((char*)&X, sizeof (int));
	File->read((char*)&Y, sizeof (int));
	File->read((char*)&Z, sizeof (int));

	File->read((char*)&Segment_Id, sizeof (int));

	int Items_Number,tmp;

	File->read((char*)&Items_Number, sizeof (int));
	Xs.clear();
	Xs.assign(Items_Number,tmp);
	for(int i=0;i<Items_Number;i++)
	File->read((char*)&Xs[i], sizeof (int));

	File->read((char*)&Items_Number, sizeof (int));
	Ys.clear();
	Ys.assign(Items_Number,tmp);
	for(int i=0;i<Items_Number;i++)
	File->read((char*)&Ys[i], sizeof (int));

	File->read((char*)&Items_Number, sizeof (int));
	Zs.clear();
	Zs.assign(Items_Number,tmp);
	for(int i=0;i<Items_Number;i++)
	File->read((char*)&Zs[i], sizeof (int));

	}
}

//-------------------------------------------------------------------------------------------------

int Segmented_Image_Class::save_object_to_stream(ofstream* File)
{
	int Items_Number;

	int version = 1;
	File->write((char*)&version, sizeof (int));

	File->write((char*)&Image_X, sizeof (int));
	File->write((char*)&Image_Y, sizeof (int));
	File->write((char*)&Image_Z, sizeof (int));

	for(long i=0; i<Image_Matrix.size(); i++)
	Image_Matrix[i].save_object_to_stream(File);

	File->write((char*)&X_Resolution, sizeof (double));
	File->write((char*)&Y_Resolution, sizeof (double));
	File->write((char*)&Z_Resolution, sizeof (double));

	File->write((char*)&Max_Intensity, sizeof (double));

	File->write((char*)&Bloodpool_Intensity_Threshold_Min, sizeof (double));
	File->write((char*)&Bloodpool_Intensity_Threshold_Max, sizeof (double));

	Items_Number = Seeds_List.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(int i=0;i<Items_Number;i++)
	Seeds_List[i].save_object_to_stream(File);

}
//---------------------------------------------------------------------------

int Segmented_Image_Class::load_object_from_stream(ifstream* File)
{
	int Items_Number;

	int version;
	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{

	File->read((char*)&Image_X, sizeof (int));
	File->read((char*)&Image_Y, sizeof (int));
	File->read((char*)&Image_Z, sizeof (int));

	allocate_image_matrix(Image_X,Image_Y,Image_Z);

	for(long i=0; i<Image_X; i++)
	for( int j=0; j<Image_Y; j++)
	for( int k=0; k<Image_Z; k++)
	Image_Matrix[i].load_object_from_stream(File);

	File->read((char*)&X_Resolution, sizeof (double));
	File->read((char*)&Y_Resolution, sizeof (double));
	File->read((char*)&Z_Resolution, sizeof (double));

	File->read((char*)&Max_Intensity, sizeof (double));

	File->read((char*)&Bloodpool_Intensity_Threshold_Min, sizeof (double));
	File->read((char*)&Bloodpool_Intensity_Threshold_Max, sizeof (double));

	File->read((char*)&Items_Number, sizeof (int));
	Segment_Seed Segment_Seed_item;
	Seeds_List.clear();
	Seeds_List.assign(Items_Number,Segment_Seed_item);
	for(int i=0;i<Items_Number;i++)
	Seeds_List[i].load_object_from_stream(File);

	}
}
//---------------------------------------------------------------------------


