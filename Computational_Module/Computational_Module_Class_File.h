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

#ifndef Computational_Module_Class_FileH
#define Computational_Module_Class_FileH
//---------------------------------------------------------------------------

#include<vector>

#include <math.h>

#include "Progress_Bar_Form.h"
#include "predefined_parameters.h"
#include "Numerical_Library.h"

//----------------------------------------------------------------------------------------

class Computational_Module_Class
{
	public:

	Computational_Module_Class();

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

	Numerical_Library Numerical_Library_Obj;

	//----------------------------------------------------------------------
	// ANNOTATION PARAMETERS
	//----------------------------------------------------------------------
	int Deflection_Detection_Alg;
	bool Individual_Reference_Channel_Annotation;
	bool Automatic_Annotation_Flag;

	double Min_Voltage_Threshold_for_LAT_annotation_mV;

	double Max_Distance_Data_Points_Projection_mm;

	//----------------------------------------------------------------------
	// Left and right distance from the activation in Roving channel around
	// which activation in the Roving channel is searched.
	// Now values are the same in all channels (copied from Annotation Setup in Main_form)
	//----------------------------------------------------------------------
	int ROV_LAT_Annotation_Left_Edge_ms;
	int ROV_LAT_Annotation_Right_Edge_ms;

	// Sliding SD window annotation parameters
	double Deflection_SD_WL_ms;
	double Deflection_SD_Th;

	//----------------------------------------------------------------------------
	// Function finds ACTIVATION in the signal between Begin and
	// End positions using given algorithm and stores found position in
	// long *Activation_Ptr. Returns OK_RESULT if ok.
	//----------------------------------------------------------------------------
	template <typename TV>
	void detect_single_activation(std::vector<TV> *Signal,
			long Begin, long End, long *Activation_Ptr,double Time_Step_ms);

	int Sliding_Window_Algorithm;
/*
	void reposition_reference_bar_according_to_V_wave(STUDY_Class *STUDY,
		TProgress_Form *Progress_Form,vector<double> *V_Wave, int V_Wave_Source);
*/

	//----------------------------------------------------------------------
	// microFRACTIONATION
	//----------------------------------------------------------------------
	int Microfractionation_Algorithm;
	long Window_Check_size_ms;  // robimy check zeby byla ciagla???
	double Microfractionation_percentile_threshold;

	//----------------------------------------------------------------------
	// AF voltage amplitude
	//----------------------------------------------------------------------
	long Window_Length_For_AF_Voltage_Calculation_ms;

	//----------------------------------------------------------------------
	// Electric dispersion calculation
	//----------------------------------------------------------------------
	long Sliding_Window_Size_for_dispersion_calculation_ms;
	double Voltage_threshold_for_dispersion_calculation_mV;

};

#endif
