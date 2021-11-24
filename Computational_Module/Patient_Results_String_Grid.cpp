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

#include "Patient_Results_String_Grid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

Patient_Record::Patient_Record()
{
	Patient_Id = "";

	Key_Names.clear();
	Values.clear();
}
//---------------------------------------------------------------------------

Patient_Results_String_Grid_Class::Patient_Results_String_Grid_Class()
{
	Patients_List.clear();
}

//---------------------------------------------------------------------------

void Patient_Results_String_Grid_Class::add_data(AnsiString Patient_Id, AnsiString Key, AnsiString Value)
{
	long Patient_Ptr = -1;

	// 1. Check if patient record is already present
	for(long p=0;p<Patients_List.size();p++)
	if( Patients_List[p].Patient_Id == Patient_Id )
	Patient_Ptr = p;

	// 2. if not found, create
	if( Patient_Ptr == -1 )
	{
		Patient_Record PTR;
		PTR.Patient_Id = Patient_Id;
		PTR.Key_Names.push_back(Key);
		PTR.Values.push_back(Value);

		Patients_List.push_back(PTR);
	}

	// 3. else, insert data
	if( Patient_Ptr >= 0 )
	{
		// find key
		long Key_Ptr=-1;
		for(long k=0;k<Patients_List[Patient_Ptr].Key_Names.size();k++)
		if( Patients_List[Patient_Ptr].Key_Names[k] == Key )
		Key_Ptr = k;

		// if key not found, create key-value pair
		if( Key_Ptr == -1 )
		{
			Patients_List[Patient_Ptr].Key_Names.push_back(Key);
			Patients_List[Patient_Ptr].Values.push_back(Value);
		}
		else // just assign/replace value
		{
			Patients_List[Patient_Ptr].Values[Key_Ptr] = Value;
		}

	}
}

//---------------------------------------------------------------------------

AnsiString Patient_Results_String_Grid_Class::get_data(AnsiString Patient_Id, AnsiString Key)
{
	long Patient_Ptr = -1;

	// 1. Check if patient record is present
	for(long p=0;p<Patients_List.size();p++)
	if( Patients_List[p].Patient_Id == Patient_Id )
	Patient_Ptr = p;

	// if not found, return empty
	if( Patient_Ptr == -1 )
	return "";

	if( Patient_Ptr != -1 ) // if found
	{
		// find key
		long Key_Ptr=-1;
		for(long k=0;k<Patients_List[Patient_Ptr].Key_Names.size();k++)
		if( Patients_List[Patient_Ptr].Key_Names[k] == Key )
		Key_Ptr = k;

		// if key not found, return empty
		if( Key_Ptr == -1 )
		return "";

		// if found, return value
		if( Key_Ptr != -1 )
			return Patients_List[Patient_Ptr].Values[Key_Ptr];
	}

	return ""; // just in case
}

//---------------------------------------------------------------------------

std::vector<AnsiString> Patient_Results_String_Grid_Class::get_unique_keys_list()
{
	std::vector<AnsiString> List;
	bool present;

	for(long p=0;p<Patients_List.size();p++)
	for(long k=0;k<Patients_List[p].Key_Names.size();k++)
	{
		// check if key already present
		present=false;
		for(long l=0;l<List.size();l++)
		if( List[l] == Patients_List[p].Key_Names[k] )
			present=true;

		if(!present)
		List.push_back(Patients_List[p].Key_Names[k]);
	}

	return List;
}

//---------------------------------------------------------------------------

void Patient_Results_String_Grid_Class::save_database_to_csv_file(ofstream* File)
{
	std::vector<AnsiString> Unique_Keys_List = get_unique_keys_list();

	for(long p=0;p<Patients_List.size();p++)
	{
		File[0] << Patients_List[p].Patient_Id.c_str() << ",";

		for(long l=0;l<Unique_Keys_List.size();l++)
		File[0] << get_data(Patients_List[p].Patient_Id, Unique_Keys_List[l]).c_str() << ",";

		File[0] << "\n";
	}
}

//---------------------------------------------------------------------------

