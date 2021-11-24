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

	bool is_this_basket_name_Hamburg_Navx(AnsiString Name,
								AnsiString* Spline,AnsiString* Electrode,
								int* Spline_Id, int* Electrode_Id);
	bool is_this_basket_name_Maastricht_Navx(AnsiString Name,
								AnsiString* Spline,AnsiString* Electrode,
								int* Spline_Id, int* Electrode_Id);
	bool is_this_basket_name_Flinders_Navx(AnsiString Name,
								AnsiString* Spline,AnsiString* Electrode,
								int* Spline_Id, int* Electrode_Id);

	void get_mike_pope_bimap_names(AnsiString Filename,AnsiString *Patient_Id,
			AnsiString *S1S2,AnsiString *Stage,AnsiString *Chamber);

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
class Diagram_Element // used in state diagram construction
{
        public:

        Diagram_Element();

        int Mode_Type_Code;
        TColor Mode_Type_Color;

        int Phase_Code;
        AnsiString Phase_Name;

        int Transition_Code;
        AnsiString Transition_Name;

        int Type; // 0 if State, 1 if Transition

        int Counter; // how many occurences of this element were in time sequence

        // positioning
        int Cell_X;
        int Cell_Y; // position in 2D grid

		std::vector <int> Connections_Ptrs;

//        vector <int> Connections_Side_Source; // on which side of the box draw line
//        vector <int> Connections_Side_Target; // on which side of the box draw line

        void get_position_of_this_state(int Phase_Code,int* X,int* Y);

		std::vector <int> Sources;

        double Beginning_Time, Ending_Time; // time reference
};

//-----------------------------------------------------------------------------

class State_Diagram
{
    public:

	State_Diagram();

    AnsiString Case_Name;
    
    int Starting_Cell_X;
    int Starting_Cell_Y;

    int Cell_Width; // in pixels
    int Cell_Height; // in pixels
    int Box_Width; // in pixels, box is inside the cell, so it should be smaller
    int Box_Height; // in pixels, box is inside the cell, so it should be smaller

    TColor State_Box_Color;
	TColor Transition_Box_Color; // or maybe pass colors from phases definitions?

	std::vector <Diagram_Element> Time_Sequenced_Elements;
    bool time_sequenced_elements_proper_syntax();

	std::vector <Diagram_Element> Diagram_Elements;
    int get_ptr_of_this_state(int Phase_Code); // if state is present returns ptr, else returns -1
    int get_ptr_of_this_transition(int Transition_Code); // if transition is present returns ptr, else returns -1
    int is_this_sequence_present(int Phase_Code1,int Transition_Code,int Phase_Code2);
    int is_this_sequence_present(int Phase_Code1,int Transition_Code);
    int reduce_time_sequenced_elements_to_diagram();
    int which_box_is_at_this_location(int X,int Y);

    bool is_this_spot_free(int X,int Y);
	bool find_free_spot_around(int Ptr,int* X,int* Y);

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



