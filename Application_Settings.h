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
#ifndef Application_SettingsH
#define Application_SettingsH

#include "predefined_parameters.h"

#include <vector>
#include <vcl.h>

//---------------------------------------------------------------------------

class Application_Settings_Class
{

	public:

	// Constructor
	Application_Settings_Class();

	AnsiString RootDirectory;

	AnsiString Main_Window_Title;

	//-----------------------------------------------------------
	// Thresohlds for differentiating tissue state
	//-----------------------------------------------------------
	double Scar_BZ_Voltage_Threshold;
	double BZ_Healthy_Voltage_Threshold;

	double Conduction_Block_CV_Threshold;
	double Slow_Conduction_Threshold;

	long Max_Number_of_Geometry_Nodes;
};
#endif
