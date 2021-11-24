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

#ifndef Data_PointH
#define Data_PointH

#include "value_description_class.h"
#include "predefined_parameters.h"
#include "Numerical_Library.h"
#include "Computational_Module_Class_File.h"
#include "Electrogram.h"

//---------------------------------------------------------------------------

class Data_Point
{

	//*****************
	private:
	//*****************

	AnsiString SourceFileName = ": Data_Point";

	//----------------------------------------------------------------------
	// Values recorded in given data point
	//----------------------------------------------------------------------
	std::vector <double> Values_Set;


	//*****************
	public:
	//*****************

	Data_Point();
	~Data_Point();

	Numerical_Library Numerical_Library_Obj;

	//------------------------------------------------------------------
	// Identifier of the data point
	//------------------------------------------------------------------
	int Identifier;

	//--------------------------------
	// DATA POINT PROPERTIES
	//--------------------------------
	int Type;
	int LabelId;
	int Utilized;  // navx import stuff
	int Displayed;

	AnsiString data_point_type(int Mapping_System_Source);

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

    bool valid_for_mapping(int Mapping_System_Source);

	bool set_value(AnsiString Value_Name, double Value, std::vector <Value_Description_Class> *Values_List);
	double get_value(AnsiString Value_Name, vector <Value_Description_Class> *Values_List);
	int get_values_number();

	double Value_Used_In_Sorting;

	friend bool operator<(const Data_Point &P1, const Data_Point &P2)
	{
		if( P1.Value_Used_In_Sorting > P2.Value_Used_In_Sorting )
		return true;
		else
		return false;
	}

	// !!!!!!!!!! add to load/save
	std::vector <int> Phase_Singularities_Present; // presence of PS in time

	// SAVE IT !!!
	// Used in Template Matching calcualtion
	int Signal_Used_To_Activation_Detection;


	bool is_data_point_value_valid(AnsiString Value_Name, vector <Value_Description_Class> *Values_List);

	//------------------------------------------------------------------
	// spatial position - copied from position of the ROV catheter during
	// "Record data point" operation
	//------------------------------------------------------------------
	double x,y,z;
	double Original_x,Original_y,Original_z;

	//----------------------------------------------------------------------
	// Signals recorded in location of this data point (roving cath) and
	// recorded simultaneously in reference and ECG channels
	// Filled during "Record data point" operation and used afterwards
	// for extraction of the values and display
	//----------------------------------------------------------------------
	Electrogram Roving_Signal;
	Electrogram Reference_Signal,Reference_Signal_2;
	Electrogram ECG_Signal;

	// expanded signals
	Electrogram All_ECG_Channels[12];
	std::vector<Electrogram> Additional_Ref_Channels;

	//----------------------------------------------------------------------
	// Annotation variables
	// Pointing position of the activation in Rov and Ref signals
	// (Roving_Signal and Reference_Signal). POINTERS in a vector "vector<int> Voltage_Values"
	// in Signal_Class.
	// "Old" values used to restore initial annotation (automatic) by button "Rst"
	// on left side of Annotation box
	//----------------------------------------------------------------------
	long Ref_Signal_Activation_ptr;
	long Rov_Signal_Activation_ptr;

	long Ref_Signal_Activation_ptr_Old; // for storing original annotation
	long Old_Ref_Position; // for undo
	long Rov_Signal_Activation_ptr_Old;
	long Old_Rov_Position; // for undo

	int Selected_Peak_For_Annotation;

	std::vector <double> V_Wave;

	// neighbours
	long Closest_Node_Id;
	std::vector<long> Neighboring_DPs;
	double Neighboring_DPs_Range_mm;


	int find_roving_catheter_LAT_in_data_point(Computational_Module_Class *Comp_Module);

	void calculate_values_in_data_point(std::vector <Value_Description_Class> *Values_List,
			Computational_Module_Class *Comp_Module);

	//-----------------------------------------------------------
	// RUNTIME, TEMPORARY VARIABLES
	//-----------------------------------------------------------
	double R,G,B;
	int Segment_Id;
	double tmp,tmp1,tmp2;
	int Flag_A;
};

#endif

