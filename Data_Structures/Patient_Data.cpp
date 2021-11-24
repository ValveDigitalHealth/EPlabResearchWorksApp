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

#include "Patient_Data.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
int Patient_Data_Class::save_object_to_stream(ofstream* File)
{
	int version = 2;
	int Size;

	File->write((char*)&version, sizeof (int));

	Size = Patient_Id.Length();
	File->write((char*)&Size, sizeof (int));
	if(Size!=0) File->write((char*)Patient_Id.c_str(),Size);

	Size = Patient_FirstName.Length();
	File->write((char*)&Size, sizeof (int));
	if(Size!=0) File->write((char*)Patient_FirstName.c_str(),Size);

	Size = Patient_Middle_Name.Length();
	File->write((char*)&Size, sizeof (int));
	if(Size!=0) File->write((char*)Patient_Middle_Name.c_str(),Size);

	Size = Patient_LastName.Length();
	File->write((char*)&Size, sizeof (int));
	if(Size!=0) File->write((char*)Patient_LastName.c_str(),Size);


	File->write((char*)&Patient_DOB, sizeof (TDateTime));
	File->write((char*)&Procedure_Date, sizeof (TDateTime));

	File->write((char*)&Patient_Gender, sizeof (int));

	Size = Attending_Physician.Length();
	File->write((char*)&Size, sizeof (int));
	if(Size!=0) File->write((char*)Attending_Physician.c_str(),Size);

	Size = Study_Comments.Length();
	File->write((char*)&Size, sizeof (int));
	if(Size!=0) File->write((char*)Study_Comments.c_str(),Size);

}
//---------------------------------------------------------------------------

int Patient_Data_Class::load_object_from_stream(ifstream* File)
{
	int Size;
	int version;
	unsigned char usc;

	File->read((char*)&version, sizeof (int));

	//---------------------------------------------------------------------
	if( version == 1 )
	//---------------------------------------------------------------------
	{

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Patient_Id.SetLength(Size);
		File->read((char*)Patient_Id.data(),Size);
	}

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Patient_FirstName.SetLength(Size);
		File->read((char*)Patient_FirstName.data(),Size);
	}

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Patient_Middle_Name.SetLength(Size);
		File->read((char*)Patient_Middle_Name.data(),Size);
	}

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Patient_LastName.SetLength(Size);
		File->read((char*)Patient_LastName.data(),Size);
	}


	File->read((char*)&Patient_DOB,sizeof (TDateTime));

	File->read((char*)&Patient_Gender,sizeof (int));


	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Attending_Physician.SetLength(Size);
		File->read((char*)Attending_Physician.data(),Size);
	}

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Study_Comments.SetLength(Size);
		File->read((char*)Study_Comments.data(),Size);
	}

	} // version 1


	//---------------------------------------------------------------------
	if( version == 2 )
	//---------------------------------------------------------------------
	{

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Patient_Id.SetLength(Size);
		File->read((char*)Patient_Id.data(),Size);
	}

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Patient_FirstName.SetLength(Size);
		File->read((char*)Patient_FirstName.data(),Size);
	}

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Patient_Middle_Name.SetLength(Size);
		File->read((char*)Patient_Middle_Name.data(),Size);
	}

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Patient_LastName.SetLength(Size);
		File->read((char*)Patient_LastName.data(),Size);
	}


	File->read((char*)&Patient_DOB,sizeof (TDateTime));

	File->read((char*)&Procedure_Date,sizeof (TDateTime));


	File->read((char*)&Patient_Gender,sizeof (int));

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Attending_Physician.SetLength(Size);
		File->read((char*)Attending_Physician.data(),Size);
	}

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		Study_Comments.SetLength(Size);
		File->read((char*)Study_Comments.data(),Size);
	}

	} // version 2

}



//---------------------------------------------------------------------------
