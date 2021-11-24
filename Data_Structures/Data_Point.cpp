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

#include "Data_Point.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

Data_Point::Data_Point()
{
	Identifier = 0;

    x = 0.0;
    y = 0.0;
    z = 0.0;

    Type = DP_POINT_TYPE_NORMAL;

    Ref_Signal_Activation_ptr = 0;
    Rov_Signal_Activation_ptr = 0;

    Ref_Signal_Activation_ptr_Old = 0;
	Rov_Signal_Activation_ptr_Old = 0;

    Signal_Used_To_Activation_Detection = -1;

	Selected_Peak_For_Annotation = 0;

    Closest_Node_Id = -1;

    Utilized = 1;  // navx import stuff
	Displayed = 1;

	R=0;
	G=0;
	B=0;

	All_ECG_Channels[0].Name = "I";
	All_ECG_Channels[1].Name = "II";
	All_ECG_Channels[2].Name = "III";
	All_ECG_Channels[3].Name = "avL";
	All_ECG_Channels[4].Name = "avR";
	All_ECG_Channels[5].Name = "avF";
	All_ECG_Channels[6].Name = "V1";
	All_ECG_Channels[7].Name = "V2";
	All_ECG_Channels[8].Name = "V3";
	All_ECG_Channels[9].Name = "V4";
	All_ECG_Channels[10].Name = "V5";
	All_ECG_Channels[11].Name = "V6";

}
//---------------------------------------------------------------------------

Data_Point::~Data_Point()
{


}

//---------------------------------------------------------------------------

int Data_Point::save_object_to_stream(ofstream* File)
{
	long S;

	int version = 5;

	File->write((char*)&version, sizeof (int));

	File->write((char*)&Identifier, sizeof (int));
	File->write((char*)&Type, sizeof (int));

	File->write((char*)&x, sizeof (double));
	File->write((char*)&y, sizeof (double));
	File->write((char*)&z, sizeof (double));

	File->write((char*)&Original_x, sizeof (double));
	File->write((char*)&Original_y, sizeof (double));
	File->write((char*)&Original_z, sizeof (double));

	S=Values_Set.size();
	File->write((char*)&S, sizeof (int));
	for(int i=0;i<S;i++)
	File->write((char*)&Values_Set[i], sizeof (double));

	Reference_Signal.save_object_to_stream(File);
	Roving_Signal.save_object_to_stream(File);
	ECG_Signal.save_object_to_stream(File);
	Reference_Signal_2.save_object_to_stream(File);

	// expanded signals
	for(int k=0;k<12;k++)
	All_ECG_Channels[k].save_object_to_stream(File);

	S=Additional_Ref_Channels.size();
	File->write((char*)&S, sizeof (int));
	for(int i=0;i<S;i++)
	Additional_Ref_Channels[i].save_object_to_stream(File);

	File->write((char*)&Ref_Signal_Activation_ptr, sizeof (long));
	File->write((char*)&Rov_Signal_Activation_ptr, sizeof (long));
	File->write((char*)&Ref_Signal_Activation_ptr_Old, sizeof (long));
	File->write((char*)&Old_Ref_Position, sizeof (long));
	File->write((char*)&Rov_Signal_Activation_ptr_Old, sizeof (long));
	File->write((char*)&Old_Rov_Position, sizeof (long));
	File->write((char*)&Original_z, sizeof (long));
	File->write((char*)&Original_z, sizeof (long));

	File->write((char*)&Signal_Used_To_Activation_Detection, sizeof (int));
	File->write((char*)&Selected_Peak_For_Annotation, sizeof (int));

	S=V_Wave.size();
	File->write((char*)&S, sizeof (long));
	for(long i=0;i<S;i++)
	File->write((char*)&V_Wave[i], sizeof (double));

	File->write((char*)&Closest_Node_Id, sizeof (long));
	File->write((char*)&Utilized, sizeof (int));
	File->write((char*)&Displayed, sizeof (int));

	return 1;
}
//---------------------------------------------------------------------------

int Data_Point::load_object_from_stream(ifstream* File)
{
	int version;
	double v=0;

	File->read((char*)&version, sizeof (int));

	if( version == 6 )
	{

	File->read((char*)&Identifier, sizeof (int));
	File->read((char*)&Type, sizeof (int));

	File->read((char*)&x, sizeof (double));
	File->read((char*)&y, sizeof (double));
	File->read((char*)&z, sizeof (double));

	File->read((char*)&Original_x, sizeof (double));
	File->read((char*)&Original_y, sizeof (double));
	File->read((char*)&Original_z, sizeof (double));

	int S;
	File->read((char*)&S, sizeof (int));
	Values_Set.clear();
	Values_Set.assign(S,v);
	for(int i=0;i<S;i++)
	File->read((char*)&Values_Set[i], sizeof (double));

	Reference_Signal.load_object_from_stream(File);
	Roving_Signal.load_object_from_stream(File);
	ECG_Signal.load_object_from_stream(File);
	Reference_Signal_2.load_object_from_stream(File);

	// expanded signals
	for(int k=0;k<12;k++)
	All_ECG_Channels[k].load_object_from_stream(File);

	Electrogram ee;
	File->read((char*)&S, sizeof (int));
	Additional_Ref_Channels.clear();
	Additional_Ref_Channels.assign(S,ee);
	for(int i=0;i<S;i++)
	Additional_Ref_Channels[i].load_object_from_stream(File);

	File->read((char*)&Ref_Signal_Activation_ptr, sizeof (long));
	File->read((char*)&Rov_Signal_Activation_ptr, sizeof (long));
	File->read((char*)&Ref_Signal_Activation_ptr_Old, sizeof (long));
	File->read((char*)&Old_Ref_Position, sizeof (long));
	File->read((char*)&Rov_Signal_Activation_ptr_Old, sizeof (long));
	File->read((char*)&Old_Rov_Position, sizeof (long));
	File->read((char*)&Original_z, sizeof (long));
	File->read((char*)&Original_z, sizeof (long));

	File->read((char*)&Signal_Used_To_Activation_Detection, sizeof (int));
	File->read((char*)&Selected_Peak_For_Annotation, sizeof (int));

	File->read((char*)&S, sizeof (long));
	V_Wave.clear();
	double tmp;
	V_Wave.assign(S,tmp);
	for(long i=0;i<S;i++)
	File->read((char*)&V_Wave[i], sizeof (double));

	File->read((char*)&Closest_Node_Id, sizeof (long));
	File->read((char*)&Utilized, sizeof (int));
	File->read((char*)&Displayed, sizeof (int));

	return 1;
	} // v.6

	return -1;
}

//---------------------------------------------------------------------------

double Data_Point::get_value(AnsiString Value_Name, vector <Value_Description_Class> *Values_List)
{
	Value_Description_Class VC;
	int ptr=-1;

	// check if values vector size corresponds with values list size
	if( Values_List[0].size() != Values_Set.size() )
	{

		// ShowMessage("data point GET value list and vector sizes differ, expanding" + SourceFileName);

		if( Values_Set.size() < Values_List[0].size() )
		{
			while( Values_Set.size() < Values_List[0].size() )
				Values_Set.push_back(0.0);
		}
	}

	// find value ptr
	for(int v=0;v<Values_List->size();v++)
	{
		VC = Values_List[0][v];
		if( Value_Name == VC.Value_Name )
		ptr = v;
	}

	if( ptr >= 0 && ptr < Values_Set.size() )
	{
		return Values_Set[ ptr ];
	}
	else
	return 0;
}

//---------------------------------------------------------------------------

bool Data_Point::set_value(AnsiString Value_Name, double Value,
		std::vector <Value_Description_Class> *Values_List)
{
	Value_Description_Class VC;
	int value_ptr=-1;
	double tmpv=0;

	// check if value is present in master Values_List, if not, add
	for(int v=0;v<Values_List[0].size();v++)
	{
		VC = Values_List[0][v];
		if( Value_Name == VC.Value_Name )
		value_ptr = v;
	}

	if( value_ptr == -1 )
	{
		ShowMessage("adding value to master values list, set_value "+ SourceFileName);
		Value_Description_Class VDC;
		VDC.Value_Name = "Value_Name";
		Values_List[0].push_back(VDC);
		value_ptr = Values_List[0].size()-1;
	}

	// check if value_ptr is in range
	if( value_ptr >= Values_Set.size() )
	{
//		ShowMessage("expanding dp values list, set_value "+ SourceFileName);
		while( value_ptr >= Values_Set.size() )
		Values_Set.push_back(tmpv);
	}

	Values_Set[value_ptr] = Value;

	return true;
}
//---------------------------------------------------------------------------

int Data_Point::get_values_number()
{
    return Values_Set.size();
}

//---------------------------------------------------------------------------

    //----------------------------------------------------------------------
    // Function calculating position of activations in Roving and Reference
    // signals used for assessment of local activation time (LAT) value
    // Returns OK_RESULT if operation successful
    // Input parameters: int LAT_Annotation_Left_Edge and int LAT_Annotation_Right_Edge
    // are setting the boundaries between which activation is searched
	// int LAT_Detection_Algorithm sets which algrithm is used to find activation
	// (see "atms_program_parameters.cpp")
	// All parameters setted in Annotation Setup (from main menu)
	//----------------------------------------------------------------------

int Data_Point::find_roving_catheter_LAT_in_data_point(Computational_Module_Class *Comp_Module)
{
	int Result=0;

	// Activation is searched around location of the activation in Ref channel
	// (Ref_Signal_Activation_ptr) in range specified by LAT_Annotation_Left_Edge and
	// LAT_Annotation_Left_Edge. Because LAT_Annotation_Left_Edge is in ms, it must be
	// divided by Roving_Signal.Catheter_Sampling to get pointers in the signal

	if( Comp_Module->Deflection_Detection_Alg != ACCORDING_TO_LOCAL_PEAK_ALG )
	Comp_Module->detect_single_activation(
	  &Roving_Signal.Voltage_Values,
	  Ref_Signal_Activation_ptr + Comp_Module->ROV_LAT_Annotation_Left_Edge_ms/Roving_Signal.Time_Step_ms,
	  Ref_Signal_Activation_ptr + Comp_Module->ROV_LAT_Annotation_Right_Edge_ms/Roving_Signal.Time_Step_ms,
	  &Rov_Signal_Activation_ptr,
	  Roving_Signal.Time_Step_ms);
	else
	{
	// set according to local peak (which one??? now is the last one...)
	for(long p=0;p<Roving_Signal.vPeak_Positions.size();p++)
	if( Roving_Signal.vPeak_Positions[p] > Ref_Signal_Activation_ptr +
		Comp_Module->ROV_LAT_Annotation_Left_Edge_ms/Roving_Signal.Time_Step_ms )
	if( Roving_Signal.vPeak_Positions[p] < Ref_Signal_Activation_ptr +
		Comp_Module->ROV_LAT_Annotation_Right_Edge_ms/Roving_Signal.Time_Step_ms )
	Rov_Signal_Activation_ptr = Roving_Signal.vPeak_Positions[p];

	}

	return Result;
}

//---------------------------------------------------------------------------

	//----------------------------------------------------------------------
	// Function calculates LAT and Bipolar voltage values using Ref_Signal_Activation_ptr
	// and Rov_Signal_Activation_ptr values
	//----------------------------------------------------------------------

void Data_Point::calculate_values_in_data_point(std::vector <Value_Description_Class> *Values_List,
		Computational_Module_Class *Comp_Module)
{
	Value_Description_Class VC;
	int ptr=-1;
	double v;

	//----------------------------------------------------------------------
	// find which values are LAT and voltage maps (for roving signal analysis)
	//----------------------------------------------------------------------
	AnsiString LAT_Value_Name = "";
	for(int v=0;v<Values_List->size();v++)
	{
		VC = Values_List[0][v];
		if( VC.LAT_Value )
		LAT_Value_Name = VC.Value_Name;
	}

	AnsiString Voltage_Amplitude_Value_Name = "";
	for(int v=0;v<Values_List->size();v++)
	{
		VC = Values_List[0][v];
		if( VC.Voltage_Amplitude_Value )
		Voltage_Amplitude_Value_Name = VC.Value_Name;
	}

	if( Voltage_Amplitude_Value_Name != "" && LAT_Value_Name != "" )
	{

	// necesssary ????
	if( Roving_Signal.Time_Step_ms == 0 )
	Roving_Signal.Time_Step_ms = 1;

	//----------------------------------------------------------------------
	// Set LAT time in data point
	//----------------------------------------------------------------------
	double v=0;
	if( Rov_Signal_Activation_ptr != NOT_POSSIBLE_TO_CALCULATE_VALUE ) // if rov valid
	v = ( Rov_Signal_Activation_ptr - Ref_Signal_Activation_ptr)
		* Roving_Signal.Time_Step_ms;
	else
	v = NOT_POSSIBLE_TO_CALCULATE_VALUE;

	set_value(LAT_Value_Name,v,Values_List);

	//----------------------------------------------------------------------
	// Calculate voltage amplitude
	//----------------------------------------------------------------------
	if( Rov_Signal_Activation_ptr != NOT_POSSIBLE_TO_CALCULATE_VALUE ) // if rov valid
//		Comp_Module->Deflection_Detection_Alg != ONSET_OF_DEFLECTION_ALG &&
//		Comp_Module->Deflection_Detection_Alg != OFFSET_OF_DEFLECTION_ALG ) // skip if onset/offset is calculated
	{

	double min,max,mean,SD;
	long Start,Stop;

	double p_rov = Rov_Signal_Activation_ptr;
	double p1 = Ref_Signal_Activation_ptr +
			Comp_Module->ROV_LAT_Annotation_Left_Edge_ms/Roving_Signal.Time_Step_ms;
	double p2 = Rov_Signal_Activation_ptr-
			ACTIVATION_HALF_WIDTH_FOR_PEAK_TO_PEAK_EXTRACTION_MS/Roving_Signal.Time_Step_ms;
	double p3 = Ref_Signal_Activation_ptr +
			Comp_Module->ROV_LAT_Annotation_Right_Edge_ms/Roving_Signal.Time_Step_ms;
	double p4 = Rov_Signal_Activation_ptr +
			ACTIVATION_HALF_WIDTH_FOR_PEAK_TO_PEAK_EXTRACTION_MS/Roving_Signal.Time_Step_ms;

	long Left_Min_Dist = 1000000,Right_Min_Dist = 1000000;

	if( p_rov > p1 && p_rov - p1 < Left_Min_Dist )  { Left_Min_Dist =  p_rov - p1; Start = p1; }
	if( p_rov < p1 && p1 - p_rov < Right_Min_Dist ) { Right_Min_Dist = p1 - p_rov; Stop = p1; }

	if( p_rov > p2 && p_rov - p2 < Left_Min_Dist )  { Left_Min_Dist =  p_rov - p2; Start = p2; }
	if( p_rov < p2 && p2 - p_rov < Right_Min_Dist ) { Right_Min_Dist = p2 - p_rov; Stop = p2; }

	if( p_rov > p3 && p_rov - p3 < Left_Min_Dist )  { Left_Min_Dist =  p_rov - p3; Start = p3; }
	if( p_rov < p3 && p3 - p_rov < Right_Min_Dist ) { Right_Min_Dist = p3 - p_rov; Stop = p3; }

	if( p_rov > p4 && p_rov - p4 < Left_Min_Dist )  { Left_Min_Dist =  p_rov - p4; Start = p4; }
	if( p_rov < p4 && p4 - p_rov < Right_Min_Dist ) { Right_Min_Dist = p4 - p_rov; Stop = p4; }

	// bipolar signal (assumed to be Roving !!!!!!!!!!!!!!!!!!!!!)
	Numerical_Library_Obj.calculate_max_min_mean_vec_ranged(
		 &Roving_Signal.Voltage_Values,Start,Stop,&min,&max,&mean,&SD);

	v = max - min;
	set_value(Voltage_Amplitude_Value_Name,v,Values_List);

	}
	else
	set_value(Voltage_Amplitude_Value_Name,NOT_POSSIBLE_TO_CALCULATE_VALUE,Values_List);

	}
}

//---------------------------------------------------------------------------

bool Data_Point::is_data_point_value_valid(AnsiString Value_Name, vector <Value_Description_Class> *Values_List)
{
	if( get_value(Value_Name,Values_List) != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		( Type == DP_POINT_TYPE_NORMAL || Type == CARTO_POINT_TYPE_SCAR ) // only normal type is accepted
	  )
	return true;
	else
	return false;
}
//---------------------------------------------------------------------------

bool Data_Point::valid_for_mapping(int Mapping_System_Source)
{
	if( Type == DP_POINT_TYPE_NORMAL )
	return true;
	else
	return false;
}

//---------------------------------------------------------------------------

AnsiString Data_Point::data_point_type(int Mapping_System_Source)
{
//	if( Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_CARTO )
//	{
		if( Type == DP_POINT_TYPE_NORMAL ) return "Normal";
		if( Type == CARTO_POINT_TYPE_LOCATION_ONLY ) return "LO";
		if( Type == CARTO_POINT_TYPE_SCAR ) return "Scar";
		if( Type == CARTO_POINT_TYPE_FLOATING ) return "Floating";
		if( Type == CARTO_POINT_TYPE_TE ) return "TE";
//	}

	return "Unfefined";
}

//---------------------------------------------------------------------------

