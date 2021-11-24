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
#include "value_description_class.h"

//---------------------------------------------------------------------------

Value_Description_Class::Value_Description_Class()
{
	Value_Name = "";
	Unit = "";
	Displayed_In_Table = false;

	Type = 0; // 0 - based on data points (so with interpolation), 1 - based on surface
	Interpolation_Type = INTERPOLATION_TYPE_FLAG_NORMAL_INTERPOLATION;
	Fixed_Palette = false;
	Inverted_Palette = false;

	// contours generation
	Display_Isovalue_Contours = false;
	Contour_Interval = 5;

	LAT_Value = false;
	Voltage_Amplitude_Value = false;
}

//---------------------------------------------------------------------------

int Value_Description_Class::save_object_to_stream(ofstream* File)
{
	int version = 1;

	File->write((char*)&version, sizeof (int));

	// ---
	Utils.save_String_to_File(File,Value_Name);
	Utils.save_String_to_File(File,Unit);
	File->write((char*)&Displayed_In_Table, sizeof (bool));

	File->write((char*)&Type, sizeof (int));
	File->write((char*)&Interpolation_Type, sizeof (int));
	File->write((char*)&Fixed_Palette, sizeof (bool));
	File->write((char*)&Inverted_Palette, sizeof (bool));

	File->write((char*)&LAT_Value, sizeof (bool));
	File->write((char*)&Voltage_Amplitude_Value, sizeof (bool));

	return 1;
}
//---------------------------------------------------------------------------

int Value_Description_Class::load_object_from_stream(ifstream* File)
{
	int Items_Number;
	int version,Size;

	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{
		Value_Name = Utils.load_String_from_File(File);
		Unit = Utils.load_String_from_File(File);
		File->read((char*)&Displayed_In_Table, sizeof (bool));

		File->read((char*)&Type, sizeof (int));
		File->read((char*)&Interpolation_Type, sizeof (int));
		File->read((char*)&Fixed_Palette, sizeof (bool));
		File->read((char*)&Inverted_Palette, sizeof (bool));

		File->read((char*)&LAT_Value, sizeof (bool));
		File->read((char*)&Voltage_Amplitude_Value, sizeof (bool));

	return 1;
	} // v1

	return -1;
}
//---------------------------------------------------------------------------

