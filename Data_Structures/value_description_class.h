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
#include <fstream>
#include<vcl.h>
#include "Utils.h"
#include "predefined_parameters.h"

#ifndef VALUE_DESCRIPTION_CLASS_H
#define VALUE_DESCRIPTION_CLASS_H

using namespace std;

class Value_Description_Class
{

	public:

	Value_Description_Class();

	// ATTENTION: values table corresopnds with values in nodes and data points. That is,
	// e.g. 5 values defined in table and EXACTLY 5 values defined in node and dp value vectors

	AnsiString Value_Name;
	AnsiString Unit;

	bool Displayed_In_Table;

	int Type; // 0 - based on data points (so with interpolation), 1 - based on surface
	int Interpolation_Type; // 0 - no interpolation, 1 - full, 2 - ?

	bool Fixed_Palette;

	// min and max values on map
	double Min_Value_On_Map, Max_Value_On_Map;

	bool Inverted_Palette;

	// contours generation
	bool Display_Isovalue_Contours;
	double Contour_Interval;

	// is value used as basis for LAT,CV,amplitude
	bool LAT_Value;
	bool Voltage_Amplitude_Value;

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

	Utils_Tools Utils;
};

#endif // VALUE_DESCRIPTION_CLASS_H
