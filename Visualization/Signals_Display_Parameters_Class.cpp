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

#include "Signals_Display_Parameters_Class.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Signals_Display_Parameters::Signals_Display_Parameters()
{
	Displayed_Segment_Length_ptr = 1000;
	Signals_Amplitude_Amplify = 100;

	Local_Signal_Zoom = 100;
	Reference_Signal_Zoom = 100;
	ECG_Signal_Zoom = 100;

	LC=true;
	REF=true;
	ECG=true;

	LC_Pos = 0.2;
	REF_Pos = 0.5;
	ECG_Pos = 0.8;

	Display_Channels_Names = true;

	Whole_Caliper_Dragging_Mode = false;
	Dragged_Caliper_Ptr = -1;
}


//---------------------------------------------------------------------------

