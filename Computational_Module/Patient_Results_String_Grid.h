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
#include <vcl.h> // for AnsiString type and ShowMessage window
#include <vector>
#include <fstream>

#ifndef Patient_Results_String_GridH
#define Patient_Results_String_GridH
//---------------------------------------------------------------------------

class Patient_Record
{
	public:

	Patient_Record();

	AnsiString Patient_Id;

	std::vector<AnsiString> Key_Names;
	std::vector<AnsiString> Values;

};

//---------------------------------------------------------------------------

class Patient_Results_String_Grid_Class
{
	private:

		std::vector <Patient_Record> Patients_List;

	public:

		Patient_Results_String_Grid_Class();

		std::vector<AnsiString> get_unique_keys_list();

		void add_data(AnsiString Patient_Id, AnsiString Key, AnsiString Value);
		AnsiString get_data(AnsiString Patient_Id, AnsiString Key);

		void save_database_to_csv_file(ofstream* File);
		void load_database_from_csv_file(ofstream* File);
};

#endif

