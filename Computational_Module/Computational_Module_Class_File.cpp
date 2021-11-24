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

#include "Computational_Module_Class_File.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

Computational_Module_Class::Computational_Module_Class()
{
	Deflection_Detection_Alg = 5;
	Individual_Reference_Channel_Annotation = true;
	Automatic_Annotation_Flag = true;
	ROV_LAT_Annotation_Left_Edge_ms = -100;
	ROV_LAT_Annotation_Right_Edge_ms = 100;

	Deflection_SD_WL_ms = 30;
	Deflection_SD_Th  = 30;

	Min_Voltage_Threshold_for_LAT_annotation_mV = 0.1;

	Max_Distance_Data_Points_Projection_mm = 10;

	Microfractionation_Algorithm = 6;

	Window_Check_size_ms = 500;
    Window_Length_For_AF_Voltage_Calculation_ms = 500;

	Sliding_Window_Size_for_dispersion_calculation_ms = 15;
	Voltage_threshold_for_dispersion_calculation_mV = 0.05;
	Microfractionation_percentile_threshold = 90;

	Sliding_Window_Algorithm = EGM_DURATION_ALGORITHM_SD;
}

//---------------------------------------------------------------------------

int Computational_Module_Class::save_object_to_stream(ofstream* File)
{
	long S;

	int version = 2;

	File->write((char*)&version, sizeof (int));

	File->write((char*)&Deflection_Detection_Alg, sizeof (int));
	File->write((char*)&Individual_Reference_Channel_Annotation, sizeof (bool));
	File->write((char*)&Automatic_Annotation_Flag, sizeof (bool));
	File->write((char*)&Min_Voltage_Threshold_for_LAT_annotation_mV, sizeof (double));
	File->write((char*)&Max_Distance_Data_Points_Projection_mm, sizeof (double));
	File->write((char*)&ROV_LAT_Annotation_Left_Edge_ms, sizeof (int));
	File->write((char*)&ROV_LAT_Annotation_Right_Edge_ms, sizeof (int));
	File->write((char*)&Deflection_SD_WL_ms, sizeof (double));
	File->write((char*)&Deflection_SD_Th, sizeof (double));
	File->write((char*)&Sliding_Window_Algorithm, sizeof (int));

	return 1;
}
//---------------------------------------------------------------------------

int Computational_Module_Class::load_object_from_stream(ifstream* File)
{
	int version;
	double v=0;

	File->read((char*)&version, sizeof (int));

	if( version == 2 )
	{

	File->read((char*)&Deflection_Detection_Alg, sizeof (int));
	File->read((char*)&Individual_Reference_Channel_Annotation, sizeof (bool));
	File->read((char*)&Automatic_Annotation_Flag, sizeof (bool));
	File->read((char*)&Min_Voltage_Threshold_for_LAT_annotation_mV, sizeof (double));
	File->read((char*)&Max_Distance_Data_Points_Projection_mm, sizeof (double));
	File->read((char*)&ROV_LAT_Annotation_Left_Edge_ms, sizeof (int));
	File->read((char*)&ROV_LAT_Annotation_Right_Edge_ms, sizeof (int));
	File->read((char*)&Deflection_SD_WL_ms, sizeof (double));
	File->read((char*)&Deflection_SD_Th, sizeof (double));
	File->read((char*)&Sliding_Window_Algorithm, sizeof (int));

	return 1;
	} // v.2

	if( version == 1 )
	{

	File->read((char*)&Deflection_Detection_Alg, sizeof (int));
	File->read((char*)&Individual_Reference_Channel_Annotation, sizeof (bool));
	File->read((char*)&Automatic_Annotation_Flag, sizeof (bool));
	File->read((char*)&Min_Voltage_Threshold_for_LAT_annotation_mV, sizeof (double));
	File->read((char*)&v, sizeof (double)); // empty
	File->read((char*)&Max_Distance_Data_Points_Projection_mm, sizeof (double));
	File->read((char*)&ROV_LAT_Annotation_Left_Edge_ms, sizeof (int));
	File->read((char*)&ROV_LAT_Annotation_Right_Edge_ms, sizeof (int));
	File->read((char*)&Deflection_SD_WL_ms, sizeof (double));
	File->read((char*)&Deflection_SD_Th, sizeof (double));
	File->read((char*)&Sliding_Window_Algorithm, sizeof (int));

	return 1;
	} // v.1


	return -1;
}

//---------------------------------------------------------------------------

template <typename TV>
void Computational_Module_Class::detect_single_activation(
		std::vector<TV> *Signal,long Begin, long End,
			long *Activation_Ptr,double Time_Step_ms)
{
	// assign non poss to calc value as default
	Activation_Ptr[0] = NOT_POSSIBLE_TO_CALCULATE_VALUE;

	// here test whether voltage is above required threshold
	bool Min_Voltage_Threshold_Crossed = false;
	for(long i=Begin;i<End;i++)
	if( i>=0 && i<Signal[0].size() )
	if(fabs(Signal[0][i]) > Min_Voltage_Threshold_for_LAT_annotation_mV )
	Min_Voltage_Threshold_Crossed = true;

	if( Min_Voltage_Threshold_Crossed )
	{


	//------------------------------------------------------------
	//------------------------------------------------------------
	if(Deflection_Detection_Alg ==MAX_VALUE_LAT_DETECTION_ALG)
	{

	double max = std::numeric_limits< double >::min();
	long max_ptr;

	for(long i=Begin;i<End;i++)
	if( i>=0 && i<Signal[0].size() )
	if(Signal[0][i] > max)
	{
		max = Signal[0][i];
		max_ptr = i;
	}

	Activation_Ptr[0] = max_ptr;

	}

	//------------------------------------------------------------
	//------------------------------------------------------------
	if(Deflection_Detection_Alg ==MIN_VALUE_LAT_DETECTION_ALG)
	{

	double min = std::numeric_limits< double >::max();
	long min_ptr;

	for(long i=Begin;i<End;i++)
	if( i>=0 && i<Signal[0].size() )
	if(Signal[0][i] < min)
	{
		min = Signal[0][i];
		min_ptr = i;
	}

	Activation_Ptr[0] = min_ptr;

	}

    //------------------------------------------------------------
    //------------------------------------------------------------
    if(Deflection_Detection_Alg ==MAX_DOWN_SLOPE_LAT_DETECTION_ALG)
	{

    double min = std::numeric_limits< double >::max();
	long min_ptr;

    for(long i=Begin;i<End-1;i++)
	if( i>=0 && i<Signal[0].size() )
	if(Signal[0][i+1]-Signal[0][i] < min)
	{
		min = Signal[0][i+1]-Signal[0][i];
        min_ptr = i;
	}

	Activation_Ptr[0] = min_ptr;

	}

	//------------------------------------------------------------
	//------------------------------------------------------------
	if(Deflection_Detection_Alg ==MAX_UP_SLOPE_LAT_DETECTION_ALG)
	{

	double max = std::numeric_limits< double >::min();
	long max_ptr;

	for(long i=Begin;i<End-1;i++)
	if( i>=0 && i<Signal[0].size() )
	if(Signal[0][i+1]-Signal[0][i] > max)
	{
		max = Signal[0][i+1]-Signal[0][i];
		max_ptr = i;
	}

	Activation_Ptr[0] = max_ptr;

	}

	//------------------------------------------------------------
	if(Deflection_Detection_Alg ==MAX_UPDOWN_SLOPE_LAT_DETECTION_ALG)
	{

	double max = std::numeric_limits< double >::min();
	long max_ptr;

	for(long i=Begin;i<End-1;i++)
	if( i>=0 && i<Signal[0].size() )
	if(std::fabs((double)(Signal[0][i+1]-Signal[0][i])) > max)
	{
		max = fabs((double)(Signal[0][i+1]-Signal[0][i]));
		max_ptr = i;
	}

	Activation_Ptr[0] = max_ptr;

	}

	//------------------------------------------------------------
	if(Deflection_Detection_Alg ==MAX_DEVIATION_LAT_DETECTION_ALG)
	{

	double max = std::numeric_limits< double >::min();
	long max_ptr;

	for(long i=Begin;i<End;i++)
	if( i>=0 && i<Signal[0].size() )
	{
		if(fabs((double)(Signal[0][i])) > max)
		{
			max = fabs((double)Signal[0][i]);
			max_ptr = i;
		}
	}

	Activation_Ptr[0] = max_ptr;

	}

	//------------------------------------------------------------
	if(Deflection_Detection_Alg ==CROSSING_ZERO_LINE_DETECTION_ALG )
	{

	double Min=std::numeric_limits< double >::max();
	double Max=std::numeric_limits< double >::min();
	double Min_Ptr=-1,Max_Ptr=-1;
	for(long i=Begin;i<End;i++)
	if( i>=0 && i<Signal[0].size() )
	{
		if( Signal[0][i] > Max )
		{
			Max = Signal[0][i];
			Max_Ptr = i;
		}
		if( Signal[0][i] < Min )
		{
			Min = Signal[0][i];
			Min_Ptr= i;
		}
	}

	long crossing_ptr=-10000;
	for(long i=std::min(Min_Ptr,Max_Ptr);i<std::max(Min_Ptr,Max_Ptr);i++)
	if( i>=2 && i<Signal[0].size()-2 )
	if( crossing_ptr == -10000 )
	{
		if( Signal[0][i-1]*Signal[0][i+1] < 0 &&
			Signal[0][i-2]*Signal[0][i+2] < 0 )
		{
			crossing_ptr = i;
		}
	}

	Activation_Ptr[0] = crossing_ptr;

	}

	//------------------------------------------------------------
	if(Deflection_Detection_Alg ==ONSET_OF_DEFLECTION_ALG ||
	   Deflection_Detection_Alg ==OFFSET_OF_DEFLECTION_ALG 	)
	//------------------------------------------------------------
	{
	long Position=-1;

	// expand window if possible
	if( Begin - 0.5*Deflection_SD_WL_ms/Time_Step_ms >= 0 )
	Begin -= 0.5*Deflection_SD_WL_ms/Time_Step_ms;

	if( End + 0.5*Deflection_SD_WL_ms/Time_Step_ms < Signal[0].size() )
	End += 0.5*Deflection_SD_WL_ms/Time_Step_ms;

	// get SD signal
	std::vector <double> Original_Signal;
	for(long i=Begin;i<End-1;i++)
	if( i>=0 && i<Signal[0].size() )
	Original_Signal.push_back(Signal[0][i]);

	std::vector <double> Filtered_Signal;

	if( Sliding_Window_Algorithm == 0 || Sliding_Window_Algorithm == EGM_DURATION_ALGORITHM_SD )
	Filtered_Signal = Numerical_Library_Obj.
		get_sliding_SD_of_signal(&Original_Signal,Deflection_SD_WL_ms/Time_Step_ms);

	if( Sliding_Window_Algorithm == EGM_DURATION_ALGORITHM_INTEGRAL )
	Filtered_Signal = Numerical_Library_Obj.
		get_abs_sum_in_window(&Original_Signal,Deflection_SD_WL_ms/Time_Step_ms);

	if( Sliding_Window_Algorithm == EGM_DURATION_ALGORITHM_INTEGRAL_AND_SD )
	{
	Filtered_Signal = Numerical_Library_Obj.
		get_sliding_SD_of_signal(&Original_Signal,Deflection_SD_WL_ms/Time_Step_ms);
	Filtered_Signal = Numerical_Library_Obj.
		get_abs_sum_in_window(&Filtered_Signal,Deflection_SD_WL_ms/Time_Step_ms);
	}

	// find threshold cutoff value
	double Min=std::numeric_limits< double >::max();
	double Max=std::numeric_limits< double >::min(),Max_Ptr=-1;
	for(long i=0;i<Filtered_Signal.size();i++)
	{
		if( Filtered_Signal[i] > Max )
		{
			Max = Filtered_Signal[i];
			Max_Ptr = i;
		}

		if( Filtered_Signal[i] < Min )
		{
			Min = Filtered_Signal[i];
		}
	}

	double Threshold = Deflection_SD_Th/100.*Max;
	Position = -1;

	// find onset or offset  (within window)
	if(Deflection_Detection_Alg ==ONSET_OF_DEFLECTION_ALG)
	{
		for(long t=0;t<=Max_Ptr;t++)
		if( Position == -1 ) // if Position not yet found
		if( Filtered_Signal[t] >= Threshold )
		Position = t + Begin;
	}

	if(Deflection_Detection_Alg ==OFFSET_OF_DEFLECTION_ALG)
	{
		for(long t=Filtered_Signal.size();t>=Max_Ptr;t--)
		if( Position == -1 ) // if Position not yet found
		if( Filtered_Signal[t] >= Threshold )
		Position = t + Begin;
	}

	if( Position != -1 )
	Activation_Ptr[0] = Position;
	else
	Activation_Ptr[0] = NOT_POSSIBLE_TO_CALCULATE_VALUE;

	}

	//------------------------------------------------------------

	} // Min_Voltage_Threshold_Crossed true
}

template void Computational_Module_Class::detect_single_activation<double>(
		std::vector<double> *Signal,long Begin, long End,
			long *Activation_Ptr,double Time_Step_ms);

template void Computational_Module_Class::detect_single_activation<float>(
		std::vector<float> *Signal,long Begin, long End,
			long *Activation_Ptr,double Time_Step_ms);

//----------------------------------------------------------------------------------------
/*

void Computational_Module_Class::reposition_reference_bar_according_to_V_wave(STUDY_Class *STUDY,
		TProgress_Form *Progress_Form,vector<double> *V_Wave, int V_Wave_Source)
{
	//--------------------------------------------------------
	// 1. Create similarity coef vector
	//--------------------------------------------------------
	double v=0;
	std::vector <double> Sim_Vector;
	Sim_Vector.assign( ,v);

	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Filtered_Signal.size();i++)
	Sim_Vector[i] = 0;

	// Calculate sim vector
	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Filtered_Signal.size();i++)
	if( i-0.5*V_Wave_Size >= 0 )
	if( i+0.5*V_Wave_Size < Data_Items[Selected_Data_Item_1]->Filtered_Signal.size() )
	Sim_Vector[i] = std::pow(Numerical_Library_Obj.get_similarity_coefficient(
		&Data_Items[Selected_Data_Item_1]->Filtered_Signal,V_Wave,
		i-0.5*V_Wave_Size,i+0.5*V_Wave_Size,0,V_Wave_Size),2);

	// find peaks in sim vector
	vector <long> Peaks_Vector;
	Peaks_Vector = Numerical_Library_Obj.calculate_peak_positions
		(Sim_Vector,Data_Items[Selected_Data_Item_1]->Voltage_Values.size(),0.8);

}
*/

//----------------------------------------------------------------------------------------


