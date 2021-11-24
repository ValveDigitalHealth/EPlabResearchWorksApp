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

#ifndef VALUES_LIST_CLASSH
#define VALUES_LIST_CLASSH

#include<vector>
#include "value_description_class.h"

#include "Utils.h"

//---------------------------------------------------------------------------

class Values_List_Class
{
	private:


	public:

	Values_List_Class();

	// ************************************************
	std::vector <Value_Description_Class> Values_Table;
	// ************************************************

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

	void add_value(Value_Description_Class V);
	void remove_value(AnsiString Value_Name);

	void change_value_name(AnsiString Old_Value_Name, AnsiString New_Value_Name);

	void set_current_value_according_to_ptr(int Value_Ptr);
	void set_current_value_according_to_name(AnsiString Value_Name);

	void set_value_minmax(int Val_Ptr,double Min, double Max);
	void set_current_value_minmax(double Min, double Max);

	std::vector <Value_Description_Class> *get_values_table_ref();

	int get_values_number();
	int get_value_ptr(AnsiString Value_Name);
	AnsiString get_value_name_according_to_ptr(int Value_Ptr);
	int get_value_type_according_to_ptr(int Value_Ptr);
	int get_value_interpolation_type_according_to_ptr(int Value_Ptr);

	int get_current_value_ptr();
	void get_current_value_minmax(double* Min, double* Max);

	bool get_current_value_inverted_flag();
	void set_current_value_inverted_flag(bool Flag);

	bool get_current_value_contours_display_flag();
	void set_current_value_contours_display_flag(bool Flag);

	double get_current_value_contours_interval();
	void set_current_value_contours_interval(double Interval);


	AnsiString get_current_value_unit();

	bool get_current_value_fixed_palette_flag();
	bool get_fixed_palette_flag(int Value_Ptr);
	void set_current_value_fixed_palette_flag(bool Flag);

	int get_LAT_value_ptr();
	int get_Voltage_Amplitude_value_ptr();

	bool is_current_value_Voltage_Amplitude_value();
	bool is_current_value_LAT_value();

	bool is_value_present(AnsiString Value_Name);

//	AnsiString find_LAT_value();
//	AnsiString find_Voltage_Amplitude_value();

	//---------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------
	// Currently displayed value on the map
	//---------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------
	AnsiString Current_Map_Value_Name;

	Utils_Tools Utils;

};

#endif
