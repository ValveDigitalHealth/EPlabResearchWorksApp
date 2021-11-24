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
#include "Phase_Singularity_Class.h"

#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------

Phase_Singularity_Class::Phase_Singularity_Class()
{
	Min_Time=0;
	Max_Time=0;

	Nodes_Ptrs.clear();
	Time_Points.clear();

	Instantaneous_AFCL_smpl = 0;

	Deleted = false;
}

//---------------------------------------------------------------------

int Phase_Singularity_Class::save_object_to_stream(ofstream* File)
{
	long Items_Number,Size;

	int version = 3;

	File->write((char*)&version, sizeof (int));

	File->write((char*)&Min_Time, sizeof (long));
	File->write((char*)&Max_Time, sizeof (long));

	File->write((char*)&Instantaneous_AFCL_smpl, sizeof (double ));

	Items_Number = Nodes_Ptrs.size();
	File->write((char*)&Items_Number, sizeof (long));
	for(long i=0;i<Items_Number;i++)
	{
		File->write((char*)&Nodes_Ptrs[i], sizeof (long));
		File->write((char*)&Time_Points[i], sizeof (long));
	}

	File->write((char*)&Deleted, sizeof (bool));
	File->write((char*)&Central_Node_Ptr, sizeof (long));

}

//-------------------------------------------------------------------------

int Phase_Singularity_Class::load_object_from_stream(ifstream* File)
{
	long Items_Number;
	int version;

	File->read((char*)&version, sizeof (int));

	if( version == 3 )
	{

	File->read((char*)&Min_Time, sizeof (long));
	File->read((char*)&Max_Time, sizeof (long));

	File->read((char*)&Instantaneous_AFCL_smpl, sizeof (double));

	File->read((char*)&Items_Number, sizeof (long));
	long t;
	Nodes_Ptrs.assign(Items_Number,t);
	Time_Points.assign(Items_Number,t);
	for(long i=0;i<Items_Number;i++)
	{
		File->read((char*)&Nodes_Ptrs[i], sizeof (long));
		File->read((char*)&Time_Points[i], sizeof (long));
	}

	File->read((char*)&Deleted, sizeof (bool));
	File->read((char*)&Central_Node_Ptr, sizeof (long));

	} // ver 3

	if( version == 1 )
	{

	File->read((char*)&Min_Time, sizeof (long));
	File->read((char*)&Max_Time, sizeof (long));

	File->read((char*)&Items_Number, sizeof (long));
	long t;
	Nodes_Ptrs.assign(Items_Number,t);
	Time_Points.assign(Items_Number,t);
	for(long i=0;i<Items_Number;i++)
	{
		File->read((char*)&Nodes_Ptrs[i], sizeof (long));
		File->read((char*)&Time_Points[i], sizeof (long));
	}

	} // ver 1

	if( version == 2 )
	{

	File->read((char*)&Min_Time, sizeof (long));
	File->read((char*)&Max_Time, sizeof (long));

	File->read((char*)&Instantaneous_AFCL_smpl, sizeof (double));

	File->read((char*)&Items_Number, sizeof (long));
	long t;
	Nodes_Ptrs.assign(Items_Number,t);
	Time_Points.assign(Items_Number,t);
	for(long i=0;i<Items_Number;i++)
	{
		File->read((char*)&Nodes_Ptrs[i], sizeof (long));
		File->read((char*)&Time_Points[i], sizeof (long));
	}

	} // ver 2

}
//---------------------------------------------------------------------------

