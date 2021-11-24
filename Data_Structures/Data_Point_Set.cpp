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

#include "Data_Point_Set.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

int Data_Point_Set_Class::save_object_to_stream(ofstream* File)
{
    int Items_Number,Size;

	int version = 2;

	File->write((char*)&version, sizeof (int));

    // ---

    Utils.save_String_to_File(File,Name);

	Items_Number = Data_Points.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(int i=0;i<Items_Number;i++)
	Data_Points[i].save_object_to_stream(File);

	Items_Number = Deleted_Data_Points.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(int i=0;i<Items_Number;i++)
	Deleted_Data_Points[i].save_object_to_stream(File);

	File->write((char*)&First_DP_Pointed_Ptr, sizeof (int));
	File->write((char*)&Second_DP_Pointed_Ptr, sizeof (int));
	File->write((char*)&Linear_CV_Value, sizeof (double));

    return 1;
}
//---------------------------------------------------------------------------

int Data_Point_Set_Class::load_object_from_stream(ifstream* File)
{
	int Items_Number;
	int version,Size;

	File->read((char*)&version, sizeof (int));

	if( version == 2 )
	{

	Name = Utils.load_String_from_File(File);

	File->read((char*)&Items_Number, sizeof (int));
	Data_Points.clear();
	Data_Point Data_Point_item;
	Data_Points.assign(Items_Number,Data_Point_item);
	for(int i=0;i<Items_Number;i++)
	{
//	Progress_Form->add_text(FloatToStr(i)+"/"+FloatToStr(Items_Number));
//	Progress_Form->Show();
//	Application->ProcessMessages();
//	if( i==3009)
//	int df=3434;

	Data_Points[i].load_object_from_stream(File);
	}

	File->read((char*)&Items_Number, sizeof (int));
	Deleted_Data_Points.clear();
	Deleted_Data_Points.assign(Items_Number,Data_Point_item);
	for(int i=0;i<Items_Number;i++)
	Deleted_Data_Points[i].load_object_from_stream(File);

	File->read((char*)&First_DP_Pointed_Ptr, sizeof (int));
	File->read((char*)&Second_DP_Pointed_Ptr, sizeof (int));
	File->read((char*)&Linear_CV_Value, sizeof (double));

	return 1;
	} // v2

	if( version == 1 )
	{

    Name = Utils.load_String_from_File(File);

	File->read((char*)&Items_Number, sizeof (int));
	Data_Points.clear();
	Data_Point Data_Point_item;
	Data_Points.assign(Items_Number,Data_Point_item);
	for(int i=0;i<Items_Number;i++)
	Data_Points[i].load_object_from_stream(File);

    File->read((char*)&First_DP_Pointed_Ptr, sizeof (int));
    File->read((char*)&Second_DP_Pointed_Ptr, sizeof (int));
    File->read((char*)&Linear_CV_Value, sizeof (double));

    return 1;
	} // v1

	return -1;
}
//---------------------------------------------------------------------------

Data_Point_Set_Class::Data_Point_Set_Class()
{
	First_DP_Pointed_Ptr = -1;
	Second_DP_Pointed_Ptr = -1;
	Linear_CV_Value = -1;

}

//---------------------------------------------------------------------------

	//----------------------------------------------------------------------
	// Destructor
    //----------------------------------------------------------------------

Data_Point_Set_Class::~Data_Point_Set_Class()
{



}

//---------------------------------------------------------------------------

    //---------------------------------------------------------------------
    // Get first free identifier for data point.
	// Used when new data point is created.
    //---------------------------------------------------------------------

int Data_Point_Set_Class::get_identifier_for_new_data_point()
{
    long max = 0;

    for(unsigned long i=0;i<Data_Points.size();i++)
	if( Data_Points[i].Identifier > max )
        max = Data_Points[i].Identifier;

    return max+1;
}

//---------------------------------------------------------------------------

void Data_Point_Set_Class::sort_data_points(AnsiString Value_Name,
		vector <Value_Description_Class> *Values_List,bool Descending_Order)
{
	// assign value used in sorting
	for(unsigned long i=0;i<Data_Points.size();i++)
	Data_Points[i].Value_Used_In_Sorting = Data_Points[i].get_value(Value_Name,Values_List);

	// sort
	if( Descending_Order )
	sort( Data_Points.begin(), Data_Points.end() );
	else
	sort( Data_Points.rbegin(), Data_Points.rend() ); // , std::greater<double>() );
}

//---------------------------------------------------------------------------
void Data_Point_Set_Class::copy_normalized_voltage_values_to_data_points(int Movie_Slider_Activation_ptr)
{
    /*
    for(unsigned long i=0;i<Data_Points.size();i++)
    if( Movie_Slider_Activation_ptr > 0 &&
        (unsigned long) Movie_Slider_Activation_ptr < Data_Points[i].Normalized_Voltage.Voltage_Values.size() )
    {

    Data_Points[i].set_value(NORMALIZED_VOLTAGE_VALUE,
            Data_Points[i].Normalized_Voltage.Voltage_Values[Movie_Slider_Activation_ptr] );
    }
    */
}

//---------------------------------------------------------------------------

long Data_Point_Set_Class::restore_removed_data_points()
{
	long number = Deleted_Data_Points.size();

	for(unsigned int i=0;i<Deleted_Data_Points.size();i++)
		Data_Points.push_back(Deleted_Data_Points[i]);

	Deleted_Data_Points.clear();

	return number;
}

//---------------------------------------------------------------------------

long Data_Point_Set_Class::hard_delete_data_points_except_specified_type(int Type)
{
	long old_number = Data_Points.size();

	for(unsigned long i=0;i<Data_Points.size();i++)
	if( Data_Points[i].Type != Type )
	{
		Data_Points.erase( Data_Points.begin() + i );
		i--;
	}

	long new_number = Data_Points.size();

	return old_number - new_number;
}

//---------------------------------------------------------------------------

long Data_Point_Set_Class::delete_not_utlized_and_displayed_points()
{
	long old_number = Data_Points.size();

	for(unsigned int i=0;i<Data_Points.size();i++)
//	if( Data_Points[i].Displayed != 0 )
	if( Data_Points[i].Utilized == 0 )
	{
		Deleted_Data_Points.push_back(Data_Points[i]);
		Data_Points.erase( Data_Points.begin() + i );
		i--;
	}

	long new_number = Data_Points.size();

	return old_number - new_number;
}

//---------------------------------------------------------------------------
long Data_Point_Set_Class::delete_data_points_with_specified_value(AnsiString Value_Name,
			double Value,std::vector <Value_Description_Class> *Values_List)
{
	long old_number = Data_Points.size();

	for(unsigned int i=0;i<Data_Points.size();i++)
	if( Data_Points[i].get_value(Value_Name,Values_List) == Value )
	{
		Deleted_Data_Points.push_back(Data_Points[i]);
		Data_Points.erase( Data_Points.begin() + i );
		i--;
	 }

	long new_number = Data_Points.size();

	return old_number - new_number;
}
//---------------------------------------------------------------------------

long Data_Point_Set_Class::delete_data_points_with_different_id(long Id)
{
	long old_number = Data_Points.size();

	for(unsigned int i=0;i<Data_Points.size();i++)
	if( Data_Points[i].Identifier == Id )
	{
		Deleted_Data_Points.push_back(Data_Points[i]);
		Data_Points.erase( Data_Points.begin() + i );
		i--;
	 }

	long new_number = Data_Points.size();

	return old_number - new_number;
}

//---------------------------------------------------------------------------
long Data_Point_Set_Class::delete_data_points_with_value_greater_than(AnsiString Value_Name,
			double Value, std::vector <Value_Description_Class> *Values_List)
{
	long old_number = Data_Points.size();

	for(unsigned int i=0;i<Data_Points.size();i++)
	if( Data_Points[i].get_value(Value_Name,Values_List) > Value )
	{
		Deleted_Data_Points.push_back(Data_Points[i]);
		Data_Points.erase( Data_Points.begin() + i );
		i--;
	 }

	long new_number = Data_Points.size();

	return old_number - new_number;
}

//---------------------------------------------------------------------------
long Data_Point_Set_Class::delete_data_points_with_rowing_signal_lower_than(
	double Threshold,double Window_Length_For_AF_Voltage_Calculation_ms)
{
	long old_number = Data_Points.size();
	double Min, Max;
	long Min_Ptr, Max_Ptr;
	double TimeStep_ms,Mean,SDev;
	int Window_Length_ptr;
/*
	for(unsigned int i=0;i<Data_Points.size();i++)
	{
//xxx
	PNUM.find_min_max(&Data_Points[i].Roving_Signal.Voltage_Values,&Min, &Max, &Min_Ptr, &Max_Ptr);

	if( fabs(Max) < Threshold && fabs(Min) < Threshold )
	{
		Deleted_Data_Points.push_back(Data_Points[i]);
		Data_Points.erase( Data_Points.begin() + i );
		i--;
	}
	}
*/
	for(long i=0;i<(signed)Data_Points.size();i++)
	{

	TimeStep_ms = Data_Points[i].Roving_Signal.Time_Step_ms;
	Window_Length_ptr = Window_Length_For_AF_Voltage_Calculation_ms/TimeStep_ms;

	long begin = 0; // 0.1*
	long end = Data_Points[i].Roving_Signal.Voltage_Values.size();
	int Windows_Number = (end-begin)/Window_Length_ptr;

	bool Low_Voltage_Segment_Present = false;

	for (int kk = 0; kk < Windows_Number; kk++)
	{
	PNUM.calculate_max_min_mean_vec_ranged(
		&Data_Points[i].Roving_Signal.Voltage_Values,
		kk*Window_Length_ptr,(kk+1)*Window_Length_ptr,&Min, &Max, &Mean,&SDev);

	if( !Low_Voltage_Segment_Present ) // so far all segments have big enough voltage
	if( fabs(Max) < Threshold && fabs(Min) < Threshold )
	Low_Voltage_Segment_Present = true;

	}

	if( Low_Voltage_Segment_Present )
	{
		Deleted_Data_Points.push_back(Data_Points[i]);
		Data_Points.erase( Data_Points.begin() + i );
		i--;
	}



	} // through data points

	long new_number = Data_Points.size();

	return old_number - new_number;
}

//---------------------------------------------------------------------------

void Data_Point_Set_Class::undelete_data_point(long DP_Ptr)
{
	if( DP_Ptr >= 0 && DP_Ptr < Deleted_Data_Points.size() )
	{
	Data_Points.push_back(Deleted_Data_Points[DP_Ptr]);
	Deleted_Data_Points.erase( Deleted_Data_Points.begin() + DP_Ptr );
	}
}

//---------------------------------------------------------------------------

void Data_Point_Set_Class::delete_data_point(long DP_Ptr)
{
	if( DP_Ptr >= 0 && DP_Ptr < Data_Points.size() )
	{
	Deleted_Data_Points.push_back(Data_Points[DP_Ptr]);
	Data_Points.erase( Data_Points.begin() + DP_Ptr );
	}
}
//---------------------------------------------------------------------------

long Data_Point_Set_Class::delete_data_points_with_value_lower_than(AnsiString Value_Name,
				double Value, std::vector <Value_Description_Class> *Values_List)
{
	long old_number = Data_Points.size();

	for(unsigned int i=0;i<Data_Points.size();i++)
	if( Data_Points[i].get_value(Value_Name,Values_List) < Value )
	{
		Deleted_Data_Points.push_back(Data_Points[i]);
		Data_Points.erase( Data_Points.begin() + i );
		i--;
	 }

	long new_number = Data_Points.size();

	return old_number - new_number;
}

//---------------------------------------------------------------------------

void Data_Point_Set_Class::delete_datapoints_with_only_zeros()
{
/*
	double Min,Max,Mean;

    for(unsigned long dp=0;dp<Data_Points.size();dp++)
	{
													// not finished
//	PNUM.calculate_max_min_mean_vec_double_ranged(
//	&Data_Points.Voltage_Values,1,Data_Points.Voltage_Values.size()-1,
//	&Min, &Max, &Mean);

	if( Max-Min < 0.00001 )
	Data_Points[dp].Deleted = ON;

	}
*/
}

//---------------------------------------------------------------------------

void Data_Point_Set_Class::group_consecutive_data_points_identifiers()
{
	if( Data_Points.size() > 1 )
	{

	int C_Id=1;
	int Start = 0;
	int Stop;

    for(unsigned int k=1;k<Data_Points.size();k++)
	{
		Start = k-1;

		while( Data_Points[k].Identifier == Data_Points[k-1].Identifier + 1 )
		k++;

		Stop = k-1;

		for(int i=Start;i<=Stop;i++)
		 Data_Points[i].Identifier = C_Id;

		C_Id++;
	}

	} // if

}
//---------------------------------------------------------------------------

int Data_Point_Set_Class::get_ptr_of_closest_data_point(double x,double y,double z,
						bool Use_Threshold,double Threshold)
{
	double dist,min_dist=100000000,min_dist_ptr=-1;

	for(unsigned int d=0;d<Data_Points.size();d++)
	{

	dist = 	std::pow(Data_Points[d].x-x,2)+
			std::pow(Data_Points[d].y-y,2)+
			std::pow(Data_Points[d].z-z,2);

	if( dist < min_dist )
	{
	   min_dist = dist;
	   min_dist_ptr = d;
	}

	}

	if( !Use_Threshold || (Use_Threshold && min_dist < Threshold) )
		return min_dist_ptr;
	else
		return -1;
}

//---------------------------------------------------------------------------

long Data_Point_Set_Class::get_valid_data_points_number(AnsiString Value_Name,
		std::vector <Value_Description_Class> *Values_List)
{
	long count = 0;

	for(unsigned long i=0;i<Data_Points.size();i++)
	if( Data_Points[i].get_value(Value_Name, Values_List ) != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	count ++;

	return count;
}
//---------------------------------------------------------------------------

void Data_Point_Set_Class::calculate_neighboring_data_points(double Range_mm)
{
	double distance,Range_squared = Range_mm*Range_mm;

	for(unsigned long dp1=0;dp1<Data_Points.size();dp1++)
	{

	Data_Points[dp1].Neighboring_DPs.clear();
	Data_Points[dp1].Neighboring_DPs_Range_mm = Range_mm;

	for(unsigned long dp2=0;dp2<Data_Points.size();dp2++)
	if( dp1 != dp2 )
	if( fabs(Data_Points[dp1].x - Data_Points[dp2].x) < Range_mm)
	if( fabs(Data_Points[dp1].y - Data_Points[dp2].y) < Range_mm)
	if( fabs(Data_Points[dp1].z - Data_Points[dp2].z) < Range_mm)
	{
		// find closest surface node
		distance = std::pow( Data_Points[dp1].x - Data_Points[dp2].x, 2 ) +
				   std::pow( Data_Points[dp1].y - Data_Points[dp2].y, 2 ) +
				   std::pow( Data_Points[dp1].z - Data_Points[dp2].z, 2 );

		if( distance < Range_squared )
		Data_Points[dp1].Neighboring_DPs.push_back(dp2);
	}

	}
}
//---------------------------------------------------------------------------

void Data_Point_Set_Class::apply_average_spatial_filter(double Range_mm,AnsiString Value_Name,
	std::vector <Value_Description_Class> *Values_List)
{
	double Sum,Counter;
	long Neig_DP;

	calculate_neighboring_data_points(Range_mm);

	for(unsigned long dp1=0;dp1<Data_Points.size();dp1++)
	if( Data_Points[dp1].is_data_point_value_valid(Value_Name,Values_List) )
	{

	Sum = 0;
	Counter = 0;
	Data_Points[dp1].tmp1 = NOT_POSSIBLE_TO_CALCULATE_VALUE;

	Sum += Data_Points[dp1].get_value(Value_Name,Values_List);
	Counter++;

	for(unsigned long n=0;n<Data_Points[dp1].Neighboring_DPs.size();n++)
	{

	Neig_DP = Data_Points[dp1].Neighboring_DPs[n];

	if( Data_Points[Neig_DP].is_data_point_value_valid(Value_Name,Values_List) )
	{
		Sum += Data_Points[Neig_DP].get_value(Value_Name,Values_List);
		Counter++;
	}
	}

	if( Counter >= 3 )
	Data_Points[dp1].tmp1 = Sum/Counter;

	}

	// assign computed values
	for(unsigned long dp1=0;dp1<Data_Points.size();dp1++)
	if( Data_Points[dp1].is_data_point_value_valid(Value_Name,Values_List) )
	if( Data_Points[dp1].tmp1 != NOT_POSSIBLE_TO_CALCULATE_VALUE )
		Data_Points[dp1].set_value(Value_Name,Data_Points[dp1].tmp1,Values_List);
}

//---------------------------------------------------------------------------

double Data_Point_Set_Class::get_global_instantaneous_AFCL_ptr(long TimePoint_Ptr,long Window_Size_Ptr)
{
	long Beginning,Ending,Counter=0;
	double Global_AFCL_ptrs=0;
	long WLR = Window_Size_Ptr/2.0;

	for(int Data_Point=0;Data_Point<Data_Points.size();Data_Point+=2)
	for(int a=1;a<Data_Points[Data_Point].Roving_Signal.Local_Activations.Local_Activations_vec.size();a++)
	{
		Beginning = Data_Points[Data_Point].Roving_Signal.Local_Activations.Local_Activations_vec[a-1].Timing_In_EGM_ptr;
		Ending = Data_Points[Data_Point].Roving_Signal.Local_Activations.Local_Activations_vec[a].Timing_In_EGM_ptr;

		if( Beginning > TimePoint_Ptr-WLR && Ending < TimePoint_Ptr+WLR  )
		{
			Global_AFCL_ptrs += Ending - Beginning;
			Counter++;
		}
	} // thru all activaitons

	if( Counter != 0 )
	Global_AFCL_ptrs /= (double)Counter;

	return Global_AFCL_ptrs;
}

//---------------------------------------------------------------------------

void Data_Point_Set_Class::get_regional_instantaneous_AFCL_ptr(long TimePoint_Ptr, vector <bool> Segments_Present,
			vector <double> *Segments_Means, Segments_List_Class *Segments_Info,long Window_Size_Ptr)
{
	long Beginning,Ending;
	int Seg_Ptr;
	double v=0,AFCL_ptrs,Counter;
	long WLR = Window_Size_Ptr/2.0;

	vector <double> Counters,Means;
	Counters.assign(Segments_Present.size(),v);
	Means.assign(Segments_Present.size(),v);

	for(int Data_Point=0;Data_Point<Data_Points.size();Data_Point+=2)
	if( Data_Points[Data_Point].Segment_Id > 0 )
	{

	// find seg_ptr
	for(int i=0;i<Segments_Info->Segments.size();i++)
	if( Segments_Info->Segments[i].Code == Data_Points[Data_Point].Segment_Id )
	Seg_Ptr = i;

	if( Segments_Present[Seg_Ptr] )
	for(int a=1;a<Data_Points[Data_Point].Roving_Signal.Local_Activations.Local_Activations_vec.size();a++)
	{
		Beginning = Data_Points[Data_Point].Roving_Signal.Local_Activations.Local_Activations_vec[a-1].Timing_In_EGM_ptr;
		Ending = Data_Points[Data_Point].Roving_Signal.Local_Activations.Local_Activations_vec[a].Timing_In_EGM_ptr;

//		if( TimePoint_Ptr > Beginning && TimePoint_Ptr < Ending )
		if( Beginning > TimePoint_Ptr-WLR && Ending < TimePoint_Ptr+WLR  )
		{
			Means[Seg_Ptr] += Ending - Beginning;
			Counters[Seg_Ptr]++;
		}
	} // thru all activaitons
	}

	// divide by counter
	for(int seg=0;seg<Means.size();seg++)
	{
		if( Segments_Present[seg] && Counters[seg] > 0 )
		Means[seg] /= Counters[seg];
		else
		Means[seg] = 0;
	}

	for(int seg=0;seg<Means.size();seg++)
	Segments_Means[0][seg] = Means[seg];

}

//---------------------------------------------------------------------------

double Data_Point_Set_Class::get_percentile_of_value(AnsiString Value_Name,
				std::vector <Value_Description_Class> *Values_List, double Percentile)
{
	vector <double> Values;
	for(long d=0;d<(signed)Data_Points.size();d++)
	Values.push_back( Data_Points[d].get_value(Value_Name,Values_List));

	double min,max,mean,SD;
	PNUM.calculate_max_min_mean_vec_double(&Values,&min, &max, &mean);
	PNUM.stdev_cor_vec_double(&Values,&mean, &SD);

	double t;
	vector <long> Histo = PNUM.get_histogram(&Values,0,true,&t);
	double Perc = PNUM.get_percentile_from_histogram(&Histo,Percentile,min, 0.125*SD);

	return Perc;
}

//---------------------------------------------------------------------------

