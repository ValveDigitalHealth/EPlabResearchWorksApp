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

// #include <windows.h>

//#include <boost/algorithm/string.hpp>

#include <Vcl.Grids.hpp>
#include <Clipbrd.hpp>

#include <vector>
#include <fstream>
#include <iostream>

#ifndef UtilsH
#define UtilsH

//---------------------------------------------------------------------------

class Row
{
	public:

	AnsiString Element[2000];

	void clear();
};

//-----------------------------------------------------------------------------

class Row_vec
{
		public:
			std::vector <AnsiString> Elements;
};
//-----------------------------------------------------------------------------


class Utils_Tools
{
	public:

	std::vector <Row_vec> load_rectangular_csv_file_to_grid(AnsiString FileName);

	bool is_substring_present(AnsiString Main_String,AnsiString Searched_Phrase);


	AnsiString get_string_before_given_text(AnsiString Main_String,AnsiString Ending_Phrase);

	AnsiString get_string_after_given_text(AnsiString Main_String,AnsiString Ending_Phrase);

	AnsiString remove_substring_from_string(AnsiString Main_String,AnsiString String_2_Remove);

	AnsiString replace_substring_from_string(AnsiString Main_String,AnsiString String_2_Replace, AnsiString With_What);

	void read_one_column_file_into_vector(AnsiString Filename, std::vector <double> &Vec, double multiplier);

	AnsiString get_string_from_the_middle(AnsiString Main_String,AnsiString Start_Stop_String);

	// not debugged yet!
	AnsiString get_string_between_two_texts(AnsiString Main_String,AnsiString First_String,AnsiString Second_String,bool* Found_Flag);

	AnsiString get_string_after_given_occurence_of_specified_string(AnsiString Main_String,
					AnsiString Given_String, int Which_Occurence);

	int how_many_times_substring_present(AnsiString Main_String,AnsiString Given_String);


	AnsiString get_string_before_given_occurence_of_specified_string(AnsiString Main_String,
					AnsiString Given_String, int Which_Occurence);


	AnsiString get_string_after_last_occurence_of_specified_string(AnsiString Main_String,
					AnsiString Given_String);
	AnsiString get_string_before_last_occurence_of_specified_string(AnsiString Main_String,
					AnsiString Given_String);

	AnsiString get_file_name_from_full_path(AnsiString Full_Path);
	AnsiString get_only_path_from_full_path(AnsiString Full_Path);

	double get_number_from_string(std::string String);

	int is_pentaray_type_name_v2(AnsiString Name);
	bool is_lasso_type_name(AnsiString Name);

	void get_spline_and_electrode_from_pentray_name(AnsiString Name,int* Spline,int* Electrode);

	void set_combobox_selection(TComboBox *ComboBox,AnsiString Selection_Text);

	void save_ansistrings_variable_to_file(ofstream* File,AnsiString S1,AnsiString S2,AnsiString S3,int Type_Flag);
	void save_tdatetime_variable_to_file(ofstream* File,AnsiString S1,TDateTime TD,AnsiString S3,int Type_Flag);
	void save_int_variable_to_file(ofstream* File,AnsiString S1,int Val,AnsiString S3,int Type_Flag);

	void load_ansistrings_variable_from_file(ifstream* File,AnsiString S1,AnsiString* S2,AnsiString S3);
	void load_tdatetime_variable_from_file(ifstream* File,AnsiString S1,TDateTime* TD,AnsiString S3);
	void load_int_variable_from_file(ifstream* File,AnsiString S1,int* Val,AnsiString S3);

	void save_String_to_File(ofstream* File, AnsiString QS);

	void save_std_string_to_File(ofstream* File, std::string QS);
	std::string load_std_string_from_File(ifstream* File);

	AnsiString load_String_from_File(ifstream* File);

	void CopyCells_From_StringGrid_ToClipboard(TStringGrid& AGrid,  const TGridRect& ACells);
	AnsiString Cells_From_StringGrid_ToText(TStringGrid& AGrid, const TGridRect& ACells);

	bool is_surface_name_excluding_from_interpolation(AnsiString Name);

	AnsiString add_file_extension(AnsiString Filename,AnsiString Extension);

	bool is_string_an_ECG_channel_name(AnsiString Name);

	const char *Utils_Tools::ExtractAfterKey(const char *pcString, const char *pcKey);
};

//-----------------------------------------------------------------------------

class Mode_Type
{
	public:

	int Mode_Type_Code;
	int R,G,B;
	TColor Type_Color;
	AnsiString Mode_Type_Name;
};

//-----------------------------------------------------------------------------

class Mode_Types_Set
{
    public:

    std::vector <Mode_Type> Types;

	int read_mode_types(AnsiString Application_Directory, AnsiString Filename);
    int get_mode_type_code(AnsiString Name);
    TColor get_mode_type_color(int Code);
    AnsiString get_mode_type_name(int Code);
};

//-----------------------------------------------------------------------------

#endif



