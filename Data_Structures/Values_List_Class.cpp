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

#include "Values_List_Class.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//--------------------------------------------------------------------------

Values_List_Class::Values_List_Class()
{
    Current_Map_Value_Name = "";
}

//--------------------------------------------------------------------------
int Values_List_Class::save_object_to_stream(ofstream* File)
{
	int Items_Number;

	int version = 1;

	File->write((char*)&version, sizeof (int));

	Items_Number = Values_Table.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(int i=0;i<Items_Number;i++)
	Values_Table[i].save_object_to_stream(File);

	return 1;
}
//---------------------------------------------------------------------------

int Values_List_Class::load_object_from_stream(ifstream* File)
{
	int Items_Number;
	int version;

	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{

	File->read((char*)&Items_Number, sizeof (int));
	Value_Description_Class TValue;
	Values_Table.clear();
	Values_Table.assign(Items_Number,TValue);
	for(int i=0;i<Items_Number;i++)
	Values_Table[i].load_object_from_stream(File);

	return 1;
	} // v1

	return -1;
}
//---------------------------------------------------------------------------

int Values_List_Class::get_current_value_ptr()
{
	return get_value_ptr(Current_Map_Value_Name);
}

//--------------------------------------------------------------------------

std::vector <Value_Description_Class> *Values_List_Class::get_values_table_ref()
{
	return  &Values_Table;
}

//--------------------------------------------------------------------------

AnsiString Values_List_Class::get_value_name_according_to_ptr(int Value_Ptr)
{
	if( Value_Ptr >= 0 && Value_Ptr < Values_Table.size() )
	return Values_Table[Value_Ptr].Value_Name;
}

//--------------------------------------------------------------------------
bool Values_List_Class::get_fixed_palette_flag(int Value_Ptr)
{
	if( Value_Ptr >= 0 && Value_Ptr < Values_Table.size() )
	return Values_Table[Value_Ptr].Fixed_Palette;
}

//--------------------------------------------------------------------------
int Values_List_Class::get_value_type_according_to_ptr(int Value_Ptr)
{
	if( Value_Ptr >= 0 && Value_Ptr < Values_Table.size() )
	return Values_Table[Value_Ptr].Type;
}

//--------------------------------------------------------------------------

int Values_List_Class::get_value_interpolation_type_according_to_ptr(int Value_Ptr)
{
	if( Value_Ptr >= 0 && Value_Ptr < Values_Table.size() )
	return Values_Table[Value_Ptr].Interpolation_Type;
}

//--------------------------------------------------------------------------


void Values_List_Class::set_current_value_according_to_ptr(int Value_Ptr)
{
	if( Value_Ptr >= 0 && Value_Ptr < Values_Table.size() )
	Current_Map_Value_Name = Values_Table[Value_Ptr].Value_Name;
}

//--------------------------------------------------------------------------

void Values_List_Class::set_current_value_according_to_name(AnsiString Value_Name)
{
	if( get_value_ptr(Value_Name) >= 0 )
	Current_Map_Value_Name = Value_Name;
}

//--------------------------------------------------------------------------

int Values_List_Class::get_values_number()
{
	return Values_Table.size();
}

//--------------------------------------------------------------------------

void Values_List_Class::change_value_name(AnsiString Old_Value_Name, AnsiString New_Value_Name)
{
	Value_Description_Class VC;

	// find value in list
	int ptr=-1;
	for(int v=0;v<Values_Table.size();v++)
	{
		VC = Values_Table[v];
		if( Old_Value_Name == VC.Value_Name )
			ptr = v;
	}

	if(ptr>=0)
	{
		Values_Table[ptr].Value_Name = New_Value_Name;
	}
}

//--------------------------------------------------------------------------

void Values_List_Class::remove_value(AnsiString Value_Name)
{
	Value_Description_Class VC;

	// find value in list
	int ptr=-1;
	for(int v=0;v<Values_Table.size();v++)
	{
		VC = Values_Table[v];
		if( Value_Name == VC.Value_Name )
			ptr = v;
	}

	if(ptr>=0)
	Values_Table.erase( Values_Table.begin() + ptr );
}

//--------------------------------------------------------------------------

bool Values_List_Class::is_value_present(AnsiString Value_Name)
{
	Value_Description_Class VC;

	// find value in list
	int ptr=-1;
	for(int v=0;v<Values_Table.size();v++)
	{
		VC = Values_Table[v];
		if( Value_Name == VC.Value_Name )
			ptr = v;
	}

	if(ptr>=0)
	return true;
	else
	return false;
}

//--------------------------------------------------------------------------

void Values_List_Class::add_value(Value_Description_Class V)
{
	Value_Description_Class VC;

	// check if value already exists
	bool exists = false;
	for(int v=0;v<Values_Table.size();v++)
	{
		VC = Values_Table[v];
		if( V.Value_Name == VC.Value_Name )
		exists = true;
	}

	if(!exists)
	Values_Table.push_back(V);
}

//--------------------------------------------------------------------------

int Values_List_Class::get_LAT_value_ptr()
{
	Value_Description_Class VC;

	for(int v=0;v<Values_Table.size();v++)
	{
		VC = Values_Table[v];
		if( VC.LAT_Value )
		return v;
	}
	return -1;
}

//--------------------------------------------------------------------------

int Values_List_Class::get_Voltage_Amplitude_value_ptr()
{
	Value_Description_Class VC;

	for(int v=0;v<Values_Table.size();v++)
	{
		VC = Values_Table[v];
		if( VC.Voltage_Amplitude_Value )
		return v;
	}
	return -1;
}
//--------------------------------------------------------------------------

int Values_List_Class::get_value_ptr(AnsiString Value_Name)
{
	int ptr = -1;
	Value_Description_Class VC;

	for(int v=0;v<Values_Table.size();v++)
	{
		VC = Values_Table[v];
		if( Value_Name == VC.Value_Name )
		ptr = v;
	}

	if( ptr >= 0 )
	{
		return ptr;
    }
	else
    {
		return -1;
	}

}

//--------------------------------------------------------------------------
void Values_List_Class::get_current_value_minmax(double* Min, double* Max)
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);
	Min[0] = 0;
	Max[0] = 1;

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
	{
		Min[0] = Values_Table[Current_Map_Value_Ptr].Min_Value_On_Map;
		Max[0] = Values_Table[Current_Map_Value_Ptr].Max_Value_On_Map;
	}
}

//--------------------------------------------------------------------------
bool Values_List_Class::get_current_value_inverted_flag()
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		return Values_Table[Current_Map_Value_Ptr].Inverted_Palette;

	return false;
}
//--------------------------------------------------------------------------

void Values_List_Class::set_current_value_inverted_flag(bool Flag)
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		Values_Table[Current_Map_Value_Ptr].Inverted_Palette = Flag;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

bool Values_List_Class::get_current_value_contours_display_flag()
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		return Values_Table[Current_Map_Value_Ptr].Display_Isovalue_Contours;

	return false;
}
//--------------------------------------------------------------------------

void Values_List_Class::set_current_value_contours_display_flag(bool Flag)
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		Values_Table[Current_Map_Value_Ptr].Display_Isovalue_Contours = Flag;
}
//--------------------------------------------------------------------------
double Values_List_Class::get_current_value_contours_interval()
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		return Values_Table[Current_Map_Value_Ptr].Contour_Interval;

	return false;
}

//--------------------------------------------------------------------------

void Values_List_Class::set_current_value_contours_interval(double Interval)
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		Values_Table[Current_Map_Value_Ptr].Contour_Interval = Interval;
}
//--------------------------------------------------------------------------

bool Values_List_Class::get_current_value_fixed_palette_flag()
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		return Values_Table[Current_Map_Value_Ptr].Fixed_Palette;

	return false;
}

//--------------------------------------------------------------------------

void Values_List_Class::set_current_value_fixed_palette_flag(bool Flag)
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		Values_Table[Current_Map_Value_Ptr].Fixed_Palette = Flag;
}

//--------------------------------------------------------------------------

AnsiString Values_List_Class::get_current_value_unit()
{
	 int ptr = get_value_ptr(Current_Map_Value_Name);

	 if( ptr>=0 && ptr < Values_Table.size())
		return Values_Table[ptr].Unit;
	 else
		 return "";
}

//--------------------------------------------------------------------------

void Values_List_Class::set_value_minmax(int Val_Ptr,double Min, double Max)
{
	if( Val_Ptr >= 0 && Val_Ptr < Values_Table.size() )
	{
		Values_Table[Val_Ptr].Min_Value_On_Map = Min;
		Values_Table[Val_Ptr].Max_Value_On_Map = Max;
	}
}

//--------------------------------------------------------------------------

void Values_List_Class::set_current_value_minmax(double Min, double Max)
{
	int Val_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Val_Ptr >= 0 && Val_Ptr < Values_Table.size() )
	{
		Values_Table[Val_Ptr].Min_Value_On_Map = Min;
		Values_Table[Val_Ptr].Max_Value_On_Map = Max;
	}
}

//--------------------------------------------------------------------------
/*
AnsiString Values_List_Class::find_LAT_value()
{
	Value_Description_Class VC;
	int ptr = -1;

	for(int v=0;v<Values_Table.size();v++)
	{
		VC = Values_Table[v];
		if( Utils.is_substring_present(VC.Value_Name,"rov LAT") || // NavX
			Utils.is_substring_present(VC.Value_Name,"Local Activation Time") // CARTO
			)
		ptr = v;
	}

	if( ptr >= 0 )
	return Values_Table[ptr].Value_Name;
	else
	{
		 // create and add value
		Value_Description_Class VDC;
		VDC.Value_Name = "Local Activation Time";
		VDC.Interpolation_Type = INTERPOLATION_TYPE_FLAG_NORMAL_INTERPOLATION;
		VDC.Unit = "ms";
		VDC.Displayed_In_Table = true;
		add_value(VDC);
		return get_value_ptr(VDC.Value_Name);
	}
}

//--------------------------------------------------------------------------

AnsiString Values_List_Class::find_Voltage_Amplitude_value()
{
	Value_Description_Class VC;
	int ptr = -1;

	for(int v=0;v<Values_Table.size();v++)
	{
		VC = Values_Table[v];
		if( Utils.is_substring_present(VC.Value_Name,"peak2peak") || // NavX
			Utils.is_substring_present(VC.Value_Name,"Bipolar voltage") // CARTO (for now only bipolar)
		)
		ptr = v;
	}

	if( ptr >= 0 )
	return Values_Table[ptr].Value_Name;
	else
	{
		 // create and add value
		Value_Description_Class VDC;
		VDC.Value_Name = "Voltage amplitude";
		VDC.Interpolation_Type = INTERPOLATION_TYPE_FLAG_NORMAL_INTERPOLATION;
		VDC.Unit = "mV";
		VDC.Displayed_In_Table = true;
		add_value(VDC);
		return get_value_ptr(VDC.Value_Name);
	}
}
*/

//--------------------------------------------------------------------------
bool Values_List_Class::is_current_value_LAT_value()
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		return Values_Table[Current_Map_Value_Ptr].LAT_Value ;

	return false;
}
//--------------------------------------------------------------------------

bool Values_List_Class::is_current_value_Voltage_Amplitude_value()
{
	int Current_Map_Value_Ptr = get_value_ptr(Current_Map_Value_Name);

	if( Current_Map_Value_Ptr >= 0 && Current_Map_Value_Ptr < Values_Table.size() )
		return Values_Table[Current_Map_Value_Ptr].Voltage_Amplitude_Value ;

	return false;
}
//--------------------------------------------------------------------------

