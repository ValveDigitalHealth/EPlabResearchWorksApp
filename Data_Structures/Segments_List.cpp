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

#include "Segments_List.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


int Segments_List_Class::save_object()
{
	ofstream dfile;
	dfile.open(FilePath.c_str());
	int version = 2;

	if( dfile == NULL )
	{
		ShowMessage("\n Error : can't open segments list file:" + FilePath );
		return -1;
	}
	else
	{

	dfile << "Segments_Names_File_v_" << version << "\n";

	for(long i=0;i<Segments.size();i++)
	Segments[i].save_object_to_stream(&dfile,version);

	dfile.close();

	return 1;

	}
}

//---------------------------------------------------------------------------

int Segments_List_Class::load_object()
{
	ifstream dfile((FilePath).c_str());
	Segment Seg;
	int version = -1;

	if( dfile == NULL )
	{
		ShowMessage("\n Error : can't open segments list file:" + FilePath );
		return -1;
	}
	else
	{

	char string[2000];

	dfile >> string;


	if( !strcmp("Segments_Names_File_v_1",string) )
	version = 1;

	if( !strcmp("Segments_Names_File_v_2",string) )
	version = 2;

	if(version > 0 )
	{
		int counter=0;
		while( !dfile.eof() && counter < 10000 )
		{
			if( Seg.load_object_from_stream(&dfile,version) )
			Segments.push_back(Seg);

			counter++;
		}
		dfile.close();
/*
		// temporary: generate colors
		for(long i=0;i<Segments.size();i++)
		{

		double Area_Color = (double)i/(double)(Segments.size()+10);

		double B = ((int)(100*Area_Color)%3) / 2.0;
		double G = ((int)(100*Area_Color)%7) / 6.0;
		double R = ((int)(100*Area_Color)%5) / 4.0;

		// to prevent gray color
		if( fabs(R-G)<0.1 && fabs(B-G)<0.1 && fabs(R-B)<0.1 )
		R = 1-R;

		Segments[i].Color_R = 255*R;
		Segments[i].Color_G = 255*G;
		Segments[i].Color_B = 255*B;

		}
		save_object();
*/

		return 1;
	}

	ShowMessage("Segments coding file Segments_Codes.txt corrupted. Contact support.");
	return -1;

	}
}

//---------------------------------------------------------------------------

int Segments_List_Class::get_segment_id(AnsiString Name)
{
	int ptr=-1;

	for(long i=0;i<(signed)Segments.size();i++)
	if( Segments[i].Name == Name )
	ptr = i;

	if( ptr >= 0 )
	return Segments[ptr].Code;
	else
	{
		ShowMessage("Segment not found get_segment_id() ");

		// add missing segment to the list
		Segment Seg;

		Seg.Name = Name;
		Seg.Code = Segments[ Segments.size()-1 ].Code + 1;
		Seg.Color_R = 100;
		Seg.Color_G = 0;
		Seg.Color_B = 200;

		Segments.push_back( Seg );

		// save segments list
		save_object();

		return Segments[ Segments.size()-1].Code;
	}
}

//---------------------------------------------------------------------------

AnsiString Segments_List_Class::get_segment_name(int Segment_Id)
{
	int ptr=0;

	for(long i=0;i<(signed)Segments.size();i++)
	if( Segments[i].Code == Segment_Id )
	ptr = i;

	if( ptr <(signed) Segments.size() )
	return Segments[ptr].Name;
	else
	return "Not_Recognized";
}

//---------------------------------------------------------------------------

int Segments_List_Class::get_segment_ptr_from_name(AnsiString Name)
{
	int ptr=-1;

	for(long i=0;i<(signed)Segments.size();i++)
	if( Segments[i].Name == Name )
	ptr = i;

	return ptr;
}
//---------------------------------------------------------------------------

void Segments_List_Class::get_segment_color(int Segment_Id,int* R,int* G,int* B)
{
	int ptr=0;

	for(long i=0;i<(signed)Segments.size();i++)
	if( Segments[i].Code == Segment_Id )
	ptr = i;

	if( ptr <(signed) Segments.size() )
	{
		R[0] = Segments[ptr].Color_R;
		G[0] = Segments[ptr].Color_G;
		B[0] = Segments[ptr].Color_B;
	}
	else
	{
		R[0] = 0;
		G[0] = 0;
		B[0] = 0;
	}
}

//---------------------------------------------------------------------------

