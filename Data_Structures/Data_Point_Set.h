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

#ifndef Data_Point_SetH
#define Data_Point_SetH

#include <vector>

#include "Data_Structures/Data_Point.h"
#include "predefined_parameters.h"
#include "value_description_class.h"
#include "Utils.h"
#include "Path.h"
#include "Segments_List.h"
#include "Numerical_Library.h"

//---------------------------------------------------------------------------

class Data_Point_Set_Class
{
	public:

	Utils_Tools Utils;
	Numerical_Library PNUM;

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

	vector <Path_Class> Paths;
	int Current_Path;

	Data_Point_Set_Class();
	~Data_Point_Set_Class();

	//---------------------------------------------------------------------
	// Name of the data set
	//---------------------------------------------------------------------
	AnsiString Name;

	//---------------------------------------------------------------------
	// Vector of data points forming this set
	//---------------------------------------------------------------------
	std::vector<Data_Point> Data_Points;
	std::vector<Data_Point> Deleted_Data_Points;
	//---------------------------------------------------------------------

	long get_valid_data_points_number(AnsiString Value_Name,
				std::vector <Value_Description_Class> *Values_List);

	//---------------------------------------------------------------------
	// Get first free identifier for data point.
	// Used when new data point is created.
	//---------------------------------------------------------------------
	int get_identifier_for_new_data_point();

	int First_DP_Pointed_Ptr;
	int Second_DP_Pointed_Ptr;
	double Linear_CV_Value;

	void copy_normalized_voltage_values_to_data_points(int Movie_Slider_Activation_ptr);

	void sort_data_points(AnsiString Value_Name,
		vector <Value_Description_Class> *Values_List,bool Descending_Order);

	void group_consecutive_data_points_identifiers();

	void undelete_data_point(long DP_Ptr);
	void delete_data_point(long DP_Ptr);

	long delete_not_utlized_and_displayed_points();
	void delete_datapoints_with_only_zeros();
	long delete_data_points_with_specified_value(AnsiString Value_Name, double Value, std::vector<Value_Description_Class> *Values_List);
	long delete_data_points_with_value_greater_than(AnsiString Value_Name, double Value,std::vector <Value_Description_Class> *Values_List);
	long delete_data_points_with_value_lower_than(AnsiString Value_Name,double Value,std::vector <Value_Description_Class> *Values_List);
	long delete_data_points_with_different_id(long Id);
	long delete_data_points_with_rowing_signal_lower_than(double Threshold,
			double Window_Length_For_AF_Voltage_Calculation_ms);
	long hard_delete_data_points_except_specified_type(int Type);

	long restore_removed_data_points();

	int get_ptr_of_closest_data_point(double x,double y,double z,
				bool Use_Threshold,double Threshold);

	void calculate_neighboring_data_points(double Range_mm);
	void apply_average_spatial_filter(double Range_mm,AnsiString Value_Name,std::vector <Value_Description_Class> *Values_List);

	//----------------------------------------------------------------------
	// signal processing for whole datapoint set
	//----------------------------------------------------------------------
	double get_global_instantaneous_AFCL_ptr(long TimePoint_Ptr,long Window_Size_Ptr);

	void get_regional_instantaneous_AFCL_ptr(long TimePoint_Ptr, vector <bool> Segments_Present,
			vector <double> *Segments_Means, Segments_List_Class *Segments_Info,long Window_Size_Ptr);

    double get_percentile_of_value(AnsiString Value_Name,
				std::vector <Value_Description_Class> *Values_List, double Percentile);
};


#endif
