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
#include <vcl.h>

#include <vector>

// #include "Electrogram.h"

#ifndef Signals_Display_Parameters_ClassH
#define Signals_Display_Parameters_ClassH
//---------------------------------------------------------------------------

class Signals_Display_Parameters
{
	public:

	Signals_Display_Parameters();

	//------------------------------------------------
//    std::vector<Electrogram*> Signals;
	//------------------------------------------------

	double Displayed_Segment_Length_ptr;

    bool Display_Channels_Names;

	double Signals_Amplitude_Amplify;

	double Local_Signal_Zoom;
	double Reference_Signal_Zoom;
	double ECG_Signal_Zoom;

	// parameters used to position signals in annotation window
	bool LC,REF,ECG;
	double LC_Pos,REF_Pos,ECG_Pos;

	int Annotation_Display_Start;

	bool Reference_Annotation_Dragged;
	bool Local_Signal_Annotation_Dragged;

	//----------------------------------------------------------------------
	// MOUSE SUPPORT
	//----------------------------------------------------------------------
	int MouseStatus;
	TMouseButton MouseButton;
//	int Mouse_X,Mouse_Y;
	int Mouse_Position_X,Mouse_Position_Y;


	//-----------------------------------------------------------
	// Calipers section
	//-----------------------------------------------------------
	// Positions of the calipers
	// E.g. left bar of the 3rd calipper is Left_Caliper_Edges[2]
	//------------------------------------------------------------------
	std::vector<long> Left_Caliper_Edges,Right_Caliper_Edges;

	// Pointer to the current calipper
	// (e.g. in Main_Signal_Thread->Right_Caliper_Edges[] )
	//-----------------------------------------------------------
	int Current_Caliper;
	//-----------------------------------------------------------
	// Flag. If ON, whole caliper dragging mode occurs.
	// Set ON when lower bar of calipper is hitted and dragged.
	//-----------------------------------------------------------
	bool Whole_Caliper_Dragging_Mode;

	//-----------------------------------------------------------
	// Pointer of the currently dragged calipper
	//-----------------------------------------------------------
	int Dragged_Caliper_Ptr;
	//-----------------------------------------------------------
	// Variable storing info which edge of calipper is dragged.
	// Values: 1 if left, 2 if right.
	//-----------------------------------------------------------
	int Left_Or_Right_Edge_Dragged;

};

#endif
