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

// 		Surface_Node_Set[i].set_value(Data_Point_Set_Ptr,Value_Id,value);
//		Data_Point_Set[dset].Data_Points[dp].get_value(Val_Name,Map_Values.get_values_table_ref());

//---------------------------------------------------------------------------
#pragma hdrstop

#include "Surface.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------

Surface_Class::Surface_Class()
{
	Mapping_System_Source = MAPPING_SYSTEM_ORIGIN_UNDEFINED;

	Data_Points_Filling_Threshold_mm = 10;

	Path_Start = -1;
	Path_Intermediate = -1;
	Path_End = -1;

	Pointed_Label = -1;

	Interpolation_Stop_Threshold = 0.005;

	Display_Geometry_Flag = true;
	Display_Whole_Dataset_Flag = true;


	Current_Data_Point_Set_Ptr = -1;
	Current_Data_Point_Ptr = -1;

	Geometry_Type = 0;

	Apex_Node_Ptr = -1;
	Septal_Anterior_Node_Ptr = -1;
	Septal_Lateral_Node_Ptr = -1;
	Anterior_Lateral_Node_Ptr = -1;

	Ap_SA_Intermediate=-1;
	Ap_SL_Intermediate=-1;
	Ap_AL_Intermediate=-1;

	Ant_Intermediate = -1;
	Sept_Intermediate = -1;
	Lat_Intermediate = -1;

	Pointed_Triangle = -1;

	Point_A_Located = false;
	Point_B_Located = false;
	Point_C_Located = false;

	Translation_Vector_X = 0;
	Translation_Vector_Y = 0;
	Translation_Vector_Z = 0;
}

//---------------------------------------------------------------------------

	//----------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------

Surface_Class::~Surface_Class()
{


}
//---------------------------------------------------------------------------

void Surface_Class::initialize_surface(TProgress_Form *Progress_Form)
		// call it after import, load or generation of new geometry
{
	Progress_Form->replace_last_line_with_text("Initializing surface...");
	Progress_Form->Show();
	Application->ProcessMessages();

   if( Data_Point_Set.size() == 0 )
   {
		// add empty data set (every geo should have one - otherwise app will not work properly -
		// too many checkpoints for dpset presence...
		Data_Point_Set_Class DS;
		Data_Point_Set.push_back(DS);
   }

   Current_Data_Point_Set_Ptr = 0;
   Current_Data_Point_Ptr = 0;

   find_neig_nodes_and_neig_triangles();
   calculate_areas_of_triangles();
   compute_paths_between_start_and_end();
   compute_paths_for_4_point_LV_segmentation();

	check_values_for_abnormal_values();
	compute_segments_centers(500);// Segments_Info.Segments.size());
	Images_Set.update_data_set();

	Progress_Form->add_text("Calculating closest nodes and data points...");
	Progress_Form->Show();
	Application->ProcessMessages();

   for(int ds=0;ds<Data_Point_Set.size();ds++)
   {
	   calculate_closest_nodes_and_data_points_ALL(ds);
	   calculate_segments_of_data_points(ds);
   }

   if( Map_Values.get_values_number() > 0 )
   Map_Values.Current_Map_Value_Name = Map_Values.get_value_name_according_to_ptr(0);

	Progress_Form->add_text("Initial interpolation...");
	Progress_Form->Show();
	Application->ProcessMessages();

   interpolate_all_values(0,0,Progress_Form);
   compute_min_max_values_on_surface();

   calculate_normals();
}

//---------------------------------------------------------------------------

int Surface_Class::save_object_to_stream(ofstream* File,TProgress_Form *Progress_Form)
{
	int Items_Number;

	Utils.save_String_to_File(File, "Surface");

	//-------------------------
	int version = 11;
	//-------------------------

	File->write((char*)&version, sizeof (int));

	Progress_Form->add_text("Saving values table...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Map_Values.save_object_to_stream(File);

	Progress_Form->add_text("Saving surface nodes...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Items_Number = Surface_Node_Set.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(long i=0;i<Items_Number;i++)
	Surface_Node_Set[i].save_object_to_stream(File);

	Progress_Form->add_text("Saving surface triangles...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Items_Number = Surface_Triangle_Set.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(long i=0;i<Items_Number;i++)
	Surface_Triangle_Set[i].save_object_to_stream(File);

	Progress_Form->add_text("Saving data point sets...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Items_Number = Data_Point_Set.size();
	File->write((char*)&Items_Number, sizeof (int));
	for(long i=0;i<Items_Number;i++)
	Data_Point_Set[i].save_object_to_stream(File);

	PASO_IS_Point_Set.save_object_to_stream(File);
	PASO_PM_Point_Set.save_object_to_stream(File);

	Progress_Form->add_text("Saving auxiliary data...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Utils.save_String_to_File(File,Name);

	File->write((char*)&Geometry_Type, sizeof (int));

	File->write((char*)&Apex_Node_Ptr, sizeof (long));
	File->write((char*)&Septal_Anterior_Node_Ptr, sizeof (long));
	File->write((char*)&Septal_Lateral_Node_Ptr, sizeof (long));
	File->write((char*)&Anterior_Lateral_Node_Ptr, sizeof (long));

	File->write((char*)&Ap_SA_Intermediate, sizeof (long));
	File->write((char*)&Ap_SL_Intermediate, sizeof (long));
	File->write((char*)&Ap_AL_Intermediate, sizeof (long));

	File->write((char*)&Ant_Intermediate, sizeof (long));
	File->write((char*)&Sept_Intermediate, sizeof (long));
	File->write((char*)&Lat_Intermediate, sizeof (long));

	int Size = Ablation_Points_List.size();
	File->write((char*)&Size, sizeof (int));
	for(int i=0;i<Size;i++)
	Ablation_Points_List[i].save_object_to_stream(File);

	Size = Labels.size();
	File->write((char*)&Size, sizeof (int));
	for(int i=0;i<Size;i++)
	Labels[i].save_object_to_stream(File);

	File->write((char*)&Path_Start, sizeof (long));
	File->write((char*)&Path_Intermediate, sizeof (long));
	File->write((char*)&Path_End, sizeof (long));

	Progress_Form->add_text("Saving images...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Images_Set.save_object_to_stream(File);

	// 3 Points located on surface used for image registration
	Registration_Point_A.save_object_to_stream(File);
	Registration_Point_B.save_object_to_stream(File);
	Registration_Point_C.save_object_to_stream(File);
	File->write((char*)&Point_A_Located, sizeof (bool));
	File->write((char*)&Point_B_Located, sizeof (bool));
	File->write((char*)&Point_C_Located, sizeof (bool));
	Registration_Point_A_normal.save_object_to_stream(File); // normals are used in displaying name
	Registration_Point_B_normal.save_object_to_stream(File);
	Registration_Point_C_normal.save_object_to_stream(File);

	File->write((char*)&Mapping_System_Source, sizeof (int));

	File->write((char*)&Translation_Vector_X, sizeof (double));
	File->write((char*)&Translation_Vector_Y, sizeof (double));
	File->write((char*)&Translation_Vector_Z, sizeof (double));

	File->write((char*)&Data_Points_Filling_Threshold_mm, sizeof (double));

	return 1;
}

//---------------------------------------------------------------------------

int Surface_Class::load_object_from_stream(ifstream* File,TProgress_Form *Progress_Form)
{
	int Items_Number;
	int version;

	AnsiString AS = Utils.load_String_from_File(File);
	if( AS != "Surface" )
	{
		ShowMessage("Corrupted file - loading surface segment");
		return -1;
	}

	File->read((char*)&version, sizeof (int));

	if( version == 11 )
	{

	Progress_Form->add_text("Loading values table...");
	Progress_Form->Show();
	Application->ProcessMessages();
	Map_Values.load_object_from_stream(File);

	Progress_Form->add_text("Loading surface nodes...");
	Progress_Form->Show();
	Application->ProcessMessages();

	File->read((char*)&Items_Number, sizeof (int));
	Surface_Node_Set.clear();
	Geometry_Vertex Node_item;
	Surface_Node_Set.assign(Items_Number,Node_item);
	for(long i=0;i<Items_Number;i++)
	Surface_Node_Set[i].load_object_from_stream(File);
	Progress_Form->add_text(IntToStr(Items_Number) + " surface nodes loaded.");

	Progress_Form->add_text("Loading triangles set...");
	Progress_Form->Show();
	Application->ProcessMessages();

	File->read((char*)&Items_Number, sizeof (int));
	Surface_Triangle_Set.clear();
	Geometry_Triangle Triangle_item;
	Surface_Triangle_Set.assign(Items_Number,Triangle_item);
	for(long i=0;i<Items_Number;i++)
	Surface_Triangle_Set[i].load_object_from_stream(File);
	Progress_Form->add_text(IntToStr(Items_Number) + " surface triangles loaded.");

	Progress_Form->add_text("Loading data points sets...");
	Progress_Form->Show();
	Application->ProcessMessages();

	File->read((char*)&Items_Number, sizeof (int));
	Data_Point_Set.clear();
	Data_Point_Set_Class Data_Point_Set_Class_item;
	Data_Point_Set.assign(Items_Number,Data_Point_Set_Class_item);
	for(long i=0;i<Items_Number;i++)
	Data_Point_Set[i].load_object_from_stream(File);

	Progress_Form->add_text(IntToStr(Items_Number) + " data point sets loaded.");
	Progress_Form->Show();
	Application->ProcessMessages();

	PASO_IS_Point_Set.load_object_from_stream(File);
	PASO_PM_Point_Set.load_object_from_stream(File);

	Progress_Form->add_text("Loading auxiliary data...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Name= Utils.load_String_from_File(File);

	File->read((char*)&Geometry_Type, sizeof (int));

	File->read((char*)&Apex_Node_Ptr, sizeof (long));
	File->read((char*)&Septal_Anterior_Node_Ptr, sizeof (long));
	File->read((char*)&Septal_Lateral_Node_Ptr, sizeof (long));

	File->read((char*)&Anterior_Lateral_Node_Ptr, sizeof (long));

	File->read((char*)&Ap_SA_Intermediate, sizeof (long));
	File->read((char*)&Ap_SL_Intermediate, sizeof (long));
	File->read((char*)&Ap_AL_Intermediate, sizeof (long));

	File->read((char*)&Ant_Intermediate, sizeof (long));
	File->read((char*)&Sept_Intermediate, sizeof (long));
	File->read((char*)&Lat_Intermediate, sizeof (long));

	int Size;

	File->read((char*)&Size, sizeof (int));
	Ablation_Points_List.clear();
	Ablation_Point_Class Ablation_Point_Class_Item;
	Ablation_Points_List.assign(Size,Ablation_Point_Class_Item);
	for(int i=0;i<Size;i++)
	Ablation_Points_List[i].load_object_from_stream(File);

	File->read((char*)&Size, sizeof (int));
	Labels.clear();
	Label_Class Label_Class_Item;
	Labels.assign(Size,Label_Class_Item);
	for(int i=0;i<Size;i++)
	Labels[i].load_object_from_stream(File);

	File->read((char*)&Path_Start, sizeof (long));
	File->read((char*)&Path_Intermediate, sizeof (long));
	File->read((char*)&Path_End, sizeof (long));

	Progress_Form->add_text("Auxiliary data loaded.");
	Progress_Form->Show();
	Application->ProcessMessages();

	Progress_Form->add_text("Loading images...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Images_Set.load_object_from_stream(File);

	// 3 Points located on surface used for image registration
	Registration_Point_A.load_object_from_stream(File);
	Registration_Point_B.load_object_from_stream(File);
	Registration_Point_C.load_object_from_stream(File);
	File->read((char*)&Point_A_Located, sizeof (bool));
	File->read((char*)&Point_B_Located, sizeof (bool));
	File->read((char*)&Point_C_Located, sizeof (bool));
	Registration_Point_A_normal.load_object_from_stream(File); // normals are used in displaying name
	Registration_Point_B_normal.load_object_from_stream(File);
	Registration_Point_C_normal.load_object_from_stream(File);

	File->read((char*)&Mapping_System_Source, sizeof (int));

	File->read((char*)&Translation_Vector_X, sizeof (double));
	File->read((char*)&Translation_Vector_Y, sizeof (double));
	File->read((char*)&Translation_Vector_Z, sizeof (double));

	File->read((char*)&Data_Points_Filling_Threshold_mm, sizeof (double));

	return 1;

	} // v11

	return -1;
}
//---------------------------------------------------------------------------


	//---------------------------------------------------------------------------------
	// Function calculating normals for triangles and nodes of the surface.
	// Returns OK_RESULT if ok.
	//---------------------------------------------------------------------------------

int Surface_Class::calculate_normals()
{

    //-------------------------------------------
	// Calculate normals to the triangles
    //-------------------------------------------
    float normal[3];
    float v1[3], v2[3];
	double d,x1,y1,z1,x2,y2,z2,x3,y3,z3;
	double Product_Sum=0;

	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	{
		x1 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].x;
		y1 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].y;
		z1 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].z;

		x2 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].x;
		y2 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].y;
		z2 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].z;

		x3 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].x;
		y3 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].y;
		z3 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].z;

		v1[0] = x2 - x1;
		v1[1] = y2 - y1;
		v1[2] = z2 - z1;

		v2[0] = x3 - x1;
		v2[1] = y3 - y1;
		v2[2] = z3 - z1;

		normal[0] = v1[1]*v2[2] - v1[2]*v2[1];
		normal[1] = v1[2]*v2[0] - v1[0]*v2[2];
		normal[2] = v1[0]*v2[1] - v1[1]*v2[0];

		d = sqrt( pow(normal[0],2) + pow(normal[1],2) + pow(normal[2],2) );

		if( d==0 )
			d = 1.0;

		normal[0] = normal[0]/d;
		normal[1] = normal[1]/d;
		normal[2] = normal[2]/d;

		Surface_Triangle_Set[i].normal[0] = normal[0];
		Surface_Triangle_Set[i].normal[1] = normal[1];
		Surface_Triangle_Set[i].normal[2] = normal[2];

		Product_Sum += x1*normal[0] + y1*normal[1] + z1*normal[2];
	  }

	// check for orientation in-out, if normals directed inward, invert
	if( Product_Sum < 0 )
	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	{
		Surface_Triangle_Set[i].normal[0] *= -1;
		Surface_Triangle_Set[i].normal[1] *= -1;
		Surface_Triangle_Set[i].normal[2] *= -1;
	}

	//-------------------------------------------
	// Calculate normals of the nodes
	//-------------------------------------------
	double x,y,z;

	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	{

	x = 0.0;
	y = 0.0;
	z = 0.0;

	for(unsigned long j=0;j<Surface_Node_Set[i].Neig_Triangles.size();j++)
    {
        x += Surface_Triangle_Set[Surface_Node_Set[i].Neig_Triangles[j]].normal[0];
        y += Surface_Triangle_Set[Surface_Node_Set[i].Neig_Triangles[j]].normal[1];
        z += Surface_Triangle_Set[Surface_Node_Set[i].Neig_Triangles[j]].normal[2];
    }

    if( Surface_Node_Set[i].Neig_Triangles.size() != 0 )
    {
		Surface_Node_Set[i].normal[0] = x / (double)Surface_Node_Set[i].Neig_Triangles.size();
		Surface_Node_Set[i].normal[1] = y / (double)Surface_Node_Set[i].Neig_Triangles.size();
		Surface_Node_Set[i].normal[2] = z / (double)Surface_Node_Set[i].Neig_Triangles.size();
    }

    }

    return 1;
}

//----------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// Function for smoothing the surface of the chamber.
	// Simple averaging algorithm implemented now.
	// Returns OK_RESULT if ok.
	//---------------------------------------------------------------------------------

int Surface_Class::smooth_map_geometry()
{
	double avx,avy,avz;

    for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	{

	avx = 0.0;
	avy = 0.0;
	avz = 0.0;

	if( Surface_Node_Set[i].Neighbors.size() > 0 )
	for(unsigned long j=0;j<Surface_Node_Set[i].Neighbors.size();j++)
	{
		avx += Surface_Node_Set[ Surface_Node_Set[i].Neighbors[j] ].x;
		avy += Surface_Node_Set[ Surface_Node_Set[i].Neighbors[j] ].y;
		avz += Surface_Node_Set[ Surface_Node_Set[i].Neighbors[j] ].z;
	}

	if( Surface_Node_Set[i].Neighbors.size() > 0 )
	{
        Surface_Node_Set[i].x = avx / Surface_Node_Set[i].Neighbors.size();
        Surface_Node_Set[i].y = avy / Surface_Node_Set[i].Neighbors.size();
        Surface_Node_Set[i].z = avz / Surface_Node_Set[i].Neighbors.size();
	}


	}

    return 1;
}

//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// Function for smoothing the values on surface
	// Simple averaging algorithm implemented now.
	// Returns OK_RESULT if ok.
	//---------------------------------------------------------------------------------

int Surface_Class::smooth_map_value(int Data_Point_Set_Ptr,int Value_Id)
{
	double v,av,Counter;

	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	{

	av = 0.0;
	Counter = 0;

	if( Surface_Node_Set[i].Neighbors.size() > 0 )
	for(unsigned long j=0;j<Surface_Node_Set[i].Neighbors.size();j++)
	{
		v = Surface_Node_Set[ Surface_Node_Set[i].Neighbors[j] ].
				get_value(Data_Point_Set_Ptr,Value_Id);
		if( v!= NOT_POSSIBLE_TO_CALCULATE_VALUE )
		{
			av += v;
			Counter++;
		}

	}

	if( Counter > 0 )
		Surface_Node_Set[i].set_value(Data_Point_Set_Ptr,Value_Id, av /Counter);
	}

    return 1;
}

//---------------------------------------------------------------------------

void Surface_Class::remove_smaller_disjointed_surface_parts()
{
/*
	int Current_Segmnent_Id;
	int Sth_Done;
	vector <int> Nodes_List;


    // Set nodes flags as -1 (not belonging to any segment)
	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	Surface_Node_Set[i].Flag_A = -1;

    // make walks until all nodes are flagged
    Current_Segmnent_Id = 1;
    Sth_Done = 1;
    while( Sth_Done > 0 )
    {

	Sth_Done = -1;

    for(unsigned long i=0;i<Surface_Node_Set.size();i++)
    if( Surface_Node_Set[i].Flag_A == -1)
    {

        // set seed
		Nodes_List.clear();
		Nodes_List.push_back(i);
        Surface_Node_Set[i].Flag_A = Current_Segmnent_Id;

		// Make walk
        while( Nodes_List.size() > 0 )
        {

		Surface_Node_Set[Nodes_List[0]].Flag_A = Current_Segmnent_Id;

        for(unsigned long k=0;k<Surface_Node_Set[Nodes_List[0]].Neighbors.size();k++)
        if( Surface_Node_Set[Surface_Node_Set[Nodes_List[0]].Neighbors[k]].Flag_A
			 != Current_Segmnent_Id )
		{
            Nodes_List.push_back(Surface_Node_Set[Nodes_List[0]].Neighbors[k]);
            Surface_Node_Set[Surface_Node_Set[Nodes_List[0]].Neighbors[k]].Flag_A
                = Current_Segmnent_Id;
        }

        // erase processed node
		Nodes_List.erase( &Nodes_List[0] );

        }

        Current_Segmnent_Id++;
        Sth_Done = 1;
    }


    } // while sth done

    if( Current_Segmnent_Id > 1 )
	{

	// Calculate histogram of the segments sizes and find the biggest segment
	long* Segments_Size_Histo; // number of nodes in each surface segment
	long max_segment,Max_Value_On_Map,Histo_Length = 0.5*Surface_Node_Set.size();
	Segments_Size_Histo = new long[ Histo_Length ];

	for(unsigned long i=0;i<Histo_Length;i++)
	Segments_Size_Histo[i] = 0;

	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	if( Surface_Node_Set[i].Flag_A >= 0)
	Segments_Size_Histo[Surface_Node_Set[i].Flag_A]++;

	Max_Value_On_Map = 0;
	for(unsigned long i=0;i<Histo_Length;i++)
	if( Max_Value_On_Map < Segments_Size_Histo[i] )
	{
		max_segment = i;
		Max_Value_On_Map = Segments_Size_Histo[i];
	}

	// Set all nodes except biggest segment as removed
	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	if( Surface_Node_Set[i].Flag_A != max_segment)
	Surface_Node_Set[i].Removed_Geometry_Vertex_Flag = ON;

	// set all triangles containing removed nodes as removed
	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	if( Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[0] ].Removed_Geometry_Vertex_Flag == ON ||
		Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[1] ].Removed_Geometry_Vertex_Flag == ON ||
		Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[2] ].Removed_Geometry_Vertex_Flag == ON )
	Surface_Triangle_Set[i].Removed_Flag = ON;

	}
*/
}


//------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// Below function checks whether Current_Data_Point_Set_Ptr is
	// within ragne of its set.
	// Returns ON if within range, OFF otherwise
	//---------------------------------------------------------------------------------

bool Surface_Class::data_points_set_ptr_in_range()
{
	if(Current_Data_Point_Set_Ptr >= 0 &&
	   (unsigned int)Current_Data_Point_Set_Ptr < Data_Point_Set.size())
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// Below function checks whether Current_Data_Point_Ptr is
	// within ragne of its set.
	// Returns ON if within range, OFF otherwise
	//---------------------------------------------------------------------------------

bool Surface_Class::data_point_ptr_in_range()
{
	if( data_points_set_ptr_in_range() )
	{
	if( Current_Data_Point_Ptr >= 0 &&
		(unsigned int)Current_Data_Point_Ptr <
			Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points.size() )
		return true;
	else
		return false;
	}

	return false;
}

//---------------------------------------------------------------------------

bool Surface_Class::deleted_data_point_ptr_in_range()
{
	if( data_points_set_ptr_in_range() )
	{
	if( Current_Deleted_Data_Point_Ptr >= 0 &&
		(unsigned int)Current_Deleted_Data_Point_Ptr <
			Data_Point_Set[Current_Data_Point_Set_Ptr].Deleted_Data_Points.size() )
		return true;
	else
		return false;
	}

	return false;
}

//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// This function projects collected data points values onto surface (to the closest node)
	// Nodes with projected values are initial condition for interpolation process.
	// Input parameter: which data point set is taken for projection onto this surface
	// Returns OK_RESULT if ok.
	//---------------------------------------------------------------------------------

void Surface_Class::project_data_points(int Data_Points_Set_Ptr)
{
	if( Surface_Node_Set.size() > 1 )
	if( Data_Points_Set_Ptr >= 0 && Data_Points_Set_Ptr < Data_Point_Set.size() )
	if( Data_Point_Set[Data_Points_Set_Ptr].Data_Points.size() > 0 )
	for(int val=0;val<Map_Values.get_values_number();val++)
	if( Map_Values.get_value_type_according_to_ptr(val) == VALUE_BASED_ON_DATA_POINTS ) // data point based value
	project_data_points_specific_value(Data_Points_Set_Ptr,val);
}

//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// This function projects collected data points values onto surface (to the closest node)
	// Nodes with projected values are initial condition for interpolation process.
	// Input parameter: which data point set is taken for projection onto this surface
	// Returns OK_RESULT if ok.
	//---------------------------------------------------------------------------------

void Surface_Class::project_data_points_specific_value(int Data_Points_Set_Ptr,int Value_Ptr)
{
	long min_ptr;
	long dpptr;
	double v,av,counter;
	AnsiString Val_Name = Map_Values.get_value_name_according_to_ptr(Value_Ptr);

	if( Surface_Node_Set.size() > 0 )
	{

	//-------------------------------------------------------------------
	// Clear colored flag and calculate initial nodes values
	//-------------------------------------------------------------------
	for(long j=0;j<(signed)Surface_Node_Set.size();j++)
	{
		Surface_Node_Set[j].coloured = 0;

		// calculate, as initial value, mean of all DP neigs
		av = 0; counter = 0;
		for(int dn=0;dn<Surface_Node_Set[j].Closest_Data_Points_Ptrs.size();dn++)
		if( Data_Point_Set[Data_Points_Set_Ptr].Data_Points[Surface_Node_Set[j]. // valid for mapping
				Closest_Data_Points_Ptrs[dn]].valid_for_mapping(Mapping_System_Source) )
		{
		v = Data_Point_Set[Data_Points_Set_Ptr].Data_Points[Surface_Node_Set[j].Closest_Data_Points_Ptrs[dn]].
					get_value(Val_Name,Map_Values.get_values_table_ref());

		if( v != NOT_POSSIBLE_TO_CALCULATE_VALUE )
		{
			av += v;
			counter++;
		}
		}

		if( counter > 0 )
		{
			av /= counter;
			Surface_Node_Set[j].set_value(Data_Points_Set_Ptr,Value_Ptr,av);
		}
		else
		Surface_Node_Set[j].set_value(Data_Points_Set_Ptr,Value_Ptr,NOT_POSSIBLE_TO_CALCULATE_VALUE);
	}

	// for each data point find closest point in scan surface
	// and transfer data
	// Main for
	for(long i=0;i<(signed)Data_Point_Set[Data_Points_Set_Ptr].Data_Points.size();i++)
	if( Data_Point_Set[Data_Points_Set_Ptr].Data_Points[i].valid_for_mapping(Mapping_System_Source) )
	if(  Data_Point_Set[Data_Points_Set_Ptr].Data_Points[i].Closest_Node_Id >= 0 &&
		 Data_Point_Set[Data_Points_Set_Ptr].Data_Points[i].Closest_Node_Id < Surface_Node_Set.size() )
	{

	min_ptr = Data_Point_Set[Data_Points_Set_Ptr].Data_Points[i].Closest_Node_Id;

	v = Data_Point_Set[Data_Points_Set_Ptr].Data_Points[i].get_value(Val_Name,Map_Values.get_values_table_ref());

	if( v != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{

	// transfer values
	if( Surface_Node_Set[min_ptr].coloured == 0 ) // not coloured yet
	{
		Surface_Node_Set[min_ptr].set_value(Data_Points_Set_Ptr,Value_Ptr,v);
		Surface_Node_Set[min_ptr].Type = Data_Point_Set[Data_Points_Set_Ptr].Data_Points[i].Type;
		Surface_Node_Set[min_ptr].coloured = 1;
	}
	else // calculate local mean
	{
		Surface_Node_Set[min_ptr].set_value(Data_Points_Set_Ptr,Value_Ptr,
			(Surface_Node_Set[min_ptr].get_value(Data_Points_Set_Ptr,Value_Ptr)*
			 (double) Surface_Node_Set[min_ptr].coloured + v )
			  / ( (double)Surface_Node_Set[min_ptr].coloured + 1.0)
			);

		Surface_Node_Set[min_ptr].coloured++;

		// This is problem. If there are data points with different types what to do?
		// Should there be priorities? E.g. SCAR_TYPE overrides all others
		Surface_Node_Set[min_ptr].Type = Data_Point_Set[Data_Points_Set_Ptr].Data_Points[i].Type;
	}
	} // valid value


	} // main for
	} // geo not empty
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// This function performs one step of linear interpolation of ONE VALUE
	// (using Diffusive Interpolation Scheme invented by author).
	// This is iterative scheme. Procedure continues until
	// threshold is crossed (Interpolation_Stop_Threshold).
	//
	// Input: v_1_ptr: which values are taken to inteprolation
	//             as defined in "atms_program_parameters.h" in "Defined values" section
	//        Continue_Interpolation_Flag - pointer to integer used to control whether
	//                    threshold is crossed. Values: ON if crossed and interpolation
    //                    should continue, OFF if not crossed and interp. should stop
	//        Data_Points_Set_Ptr - Which data point set is a source of the values
	//
	// Returns OK_RESULT if ok.
	//---------------------------------------------------------------------------------
//
int Surface_Class::interpolate_one_value_one_step(int v_1_ptr,
			bool* Continue_Interpolation_Flag,int Data_Points_Set_Ptr)
{
	long n_ptr;
	double old,v,dv;
	int sth_done = 0;
	double counter;

	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	if( Surface_Node_Set[i].coloured <= 0) // only not colored
	if( !Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
	if( Surface_Node_Set[i].get_value_no_range_checking(Data_Points_Set_Ptr,v_1_ptr) != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{

		v = 0; counter = 0;
		for(unsigned int n=0;n<Surface_Node_Set[i].Neighbors.size();n++)
		if( !Surface_Node_Set[Surface_Node_Set[i].Neighbors[n]].Removed_Geometry_Vertex_Flag )
		{
			dv = Surface_Node_Set[Surface_Node_Set[i].Neighbors[n]].
					get_value_no_range_checking(Data_Points_Set_Ptr,v_1_ptr);
			if( dv != NOT_POSSIBLE_TO_CALCULATE_VALUE )
			{
				v += dv;
				counter++;
			}
		}

		if( counter != 0 )
		v /= counter;

		old = Surface_Node_Set[i].get_value(Data_Points_Set_Ptr,v_1_ptr);

		Surface_Node_Set[i].set_value(Data_Points_Set_Ptr,v_1_ptr,v);

		if( old!=0 )
		if( (v - old)/old > Interpolation_Stop_Threshold )
		Continue_Interpolation_Flag[0] = true;
	}

	return OK_RESULT;
}

//------------------------------------------------------------------------------

AnsiString Surface_Class::calculate_AFCL_map(int Data_Point_Set_Id,
		double Base_AFCL_ms,Computational_Module_Class *Comp_Module_Ptr)
{
	Progress_Form->add_text("Calculating AFCL");
	Progress_Form->Show();
	Application->ProcessMessages();

	// add value to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS;
	Value_Desc.Displayed_In_Table = true;
	Value_Desc.Value_Name = "Mean AFCL";
	Value_Desc.Unit = "ms";
	Map_Values.add_value(Value_Desc);
	int Map_Ptr = Map_Values.get_value_ptr(Value_Desc.Value_Name);

	double AFCL_ms;

	for(long i=0;i<(signed)Data_Point_Set[Data_Point_Set_Id].Data_Points.size();i++)
	{

	if( i%15 == 1 )
	{
		Progress_Form->replace_last_line_with_text("Calculating AFCL Processing dp: " + IntToStr((int)i) + "/" +
			IntToStr((int)(Data_Point_Set[Data_Point_Set_Id].Data_Points.size())) );
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	double TimeStep_ms = Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Time_Step_ms;

	if( TimeStep_ms == 0 )
	{
	ShowMessage("Timestep=0 in AFCL calculations (surface.)");
	return "";
	}

	int EGM_Type = 0; ///!!!!!!!!!!!!!!!!!! JUST UNIPOLAR FOR NOW

	Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Phase =
		PNUM.get_electrogram_phase(
			&Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Voltage_Values,
			EGM_Type,Base_AFCL_ms/TimeStep_ms,TimeStep_ms);

	Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.
		Local_Activations.Local_Activations_vec =
		PNUM.get_local_activations_from_phase_signal(
			&Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Phase,
			EGM_Type,TimeStep_ms);

	double Size = Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.
		Local_Activations.Local_Activations_vec.size();

	if( Size > 1 )
	{

	AFCL_ms = (Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.
		Local_Activations.Local_Activations_vec[Size-1].Timing_In_EGM_ptr -
				Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.
		Local_Activations.Local_Activations_vec[0].Timing_In_EGM_ptr) / (Size-1);

	AFCL_ms *= TimeStep_ms;

	Data_Point_Set[Data_Point_Set_Id].Data_Points[i].set_value(Value_Desc.Value_Name,
				 AFCL_ms,Map_Values.get_values_table_ref() );

	} // if peaks size number > 1
	else
		Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points[i].set_value(
				 Value_Desc.Value_Name,
				 NOT_POSSIBLE_TO_CALCULATE_VALUE,Map_Values.get_values_table_ref() );
	} // going thru data points

	// interpolate map
	interpolate_specific_value(0,Data_Point_Set_Id,Map_Values.get_value_ptr("Mean AFCL"),Progress_Form);

	return Value_Desc.Value_Name;
}

//------------------------------------------------------------------------------

AnsiString Surface_Class::calculate_voltage_amplitude_map(int Data_Point_Set_Id,
			Computational_Module_Class *Comp_Module_Ptr)
{
	// add value to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS;
	Value_Desc.Displayed_In_Table = true;
	Value_Desc.Fixed_Palette = false;
	Value_Desc.Inverted_Palette = false;
	Value_Desc.Value_Name = "Mean AF voltage";
	Value_Desc.Unit = "mV";
	Map_Values.add_value(Value_Desc);
	int Map_Ptr = Map_Values.get_value_ptr(Value_Desc.Value_Name);

	double TimeStep_ms,Min,Max,Mean,SDev;
	int Window_Length_ptr;

	for(long i=0;i<(signed)Data_Point_Set[Data_Point_Set_Id].Data_Points.size();i++)
	{

	if( i%30 == 1 )
	{
		Progress_Form->replace_last_line_with_text("Processing dp: " + IntToStr((int)i) + "/" +
			IntToStr((int)(Data_Point_Set[Data_Point_Set_Id].Data_Points.size())) );
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	TimeStep_ms = Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Time_Step_ms;
	Window_Length_ptr = Comp_Module_Ptr->Window_Length_For_AF_Voltage_Calculation_ms/TimeStep_ms;

	long begin = 0; // 0.1*
		Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Voltage_Values.size();
	long end =
		Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Voltage_Values.size();

	int Windows_Number = (end-begin)/Window_Length_ptr;

	double mean=0;
	double counter=0;

	for (int kk = 0; kk < Windows_Number; kk++)
	{
	PNUM.calculate_max_min_mean_vec_ranged(
		&Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Voltage_Values,
		kk*Window_Length_ptr,(kk+1)*Window_Length_ptr,&Min, &Max, &Mean,&SDev);
	mean+= Max-Min;
	counter++;
	}

	if( counter != 0 )
	mean /= counter;

	Data_Point_Set[Data_Point_Set_Id].Data_Points[i].set_value(Value_Desc.Value_Name,
				 mean,Map_Values.get_values_table_ref() );

	} // through data points

	// interpolate map
	interpolate_specific_value(0,Data_Point_Set_Id,Map_Ptr,
			Progress_Form);

	return Value_Desc.Value_Name;
}

//------------------------------------------------------------------------------

AnsiString Surface_Class::calculate_egm_duration_map(int Data_Point_Set_Id,
		Computational_Module_Class *Comp_Module_Ptr)
{
	// add value to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS;
	Value_Desc.Displayed_In_Table = true;
	Value_Desc.Inverted_Palette = true;
	Value_Desc.Value_Name = "EGM duration";
	Value_Desc.Unit = "ms";
	Map_Values.add_value(Value_Desc);
	int EGMd_map_Ptr = Map_Values.get_value_ptr(Value_Desc.Value_Name);

	long Deflection_Start_Ptr,Deflection_Stop_Ptr;
	int Prev_Deflection_Detection_Alg = Comp_Module_Ptr->Deflection_Detection_Alg;
	double EGMd,TimeStep_ms;

	for(long i=0;i<(signed)Data_Point_Set[Data_Point_Set_Id].Data_Points.size();i++)
	{

	if( i%30 == 1 )
	{
		Progress_Form->replace_last_line_with_text("Processing dp: " + IntToStr((int)i) + "/" +
			IntToStr((int)(Data_Point_Set[Data_Point_Set_Id].Data_Points.size())) );
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	TimeStep_ms = Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Time_Step_ms;

	Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.vED_Positions_In_Signal.clear();

	Comp_Module_Ptr->Deflection_Detection_Alg = ONSET_OF_DEFLECTION_ALG;
	Comp_Module_Ptr->detect_single_activation(
	  &Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Voltage_Values,
	  Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Ref_Signal_Activation_ptr +
		Comp_Module_Ptr->ROV_LAT_Annotation_Left_Edge_ms/TimeStep_ms,
	  Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Ref_Signal_Activation_ptr +
		Comp_Module_Ptr->ROV_LAT_Annotation_Right_Edge_ms/TimeStep_ms,
	  &Deflection_Start_Ptr,TimeStep_ms);

	Comp_Module_Ptr->Deflection_Detection_Alg = OFFSET_OF_DEFLECTION_ALG;
	Comp_Module_Ptr->detect_single_activation(
	  &Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Voltage_Values,
	  Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Ref_Signal_Activation_ptr +
		Comp_Module_Ptr->ROV_LAT_Annotation_Left_Edge_ms/TimeStep_ms,
	  Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Ref_Signal_Activation_ptr +
		Comp_Module_Ptr->ROV_LAT_Annotation_Right_Edge_ms/TimeStep_ms,
	  &Deflection_Stop_Ptr,TimeStep_ms);

		if( Deflection_Start_Ptr != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
			Deflection_Stop_Ptr  != NOT_POSSIBLE_TO_CALCULATE_VALUE  )
		{
			Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.
				vED_Positions_In_Signal.push_back(Deflection_Start_Ptr);

			Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.
				vED_Positions_In_Signal.push_back(Deflection_Stop_Ptr);

			EGMd = (double)(Deflection_Stop_Ptr-Deflection_Start_Ptr)*TimeStep_ms;

			Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points[i].set_value(
				 Value_Desc.Value_Name,
				 EGMd,Map_Values.get_values_table_ref() );
		}
		else
			Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points[i].set_value(
				 Value_Desc.Value_Name,
				 NOT_POSSIBLE_TO_CALCULATE_VALUE,Map_Values.get_values_table_ref() );

	} // through data points

	// restore original detection algorithm
	Comp_Module_Ptr->Deflection_Detection_Alg = Prev_Deflection_Detection_Alg;

	// interpolate map
	interpolate_specific_value(0,Data_Point_Set_Id,EGMd_map_Ptr,
			Progress_Form);

	return Value_Desc.Value_Name;
}

//------------------------------------------------------------------------------

AnsiString Surface_Class::calculate_microfractionation_map(int Data_Point_Set_Id,
			Computational_Module_Class *Comp_Module_Ptr)
{
	Progress_Form->add_text("Calculating microfractionation map...");
	Progress_Form->Show();
	Application->ProcessMessages();

	// add value to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS;
	Value_Desc.Displayed_In_Table = true;
	Value_Desc.Inverted_Palette = true;
	Value_Desc.Value_Name = "Microfractionation";
	Value_Desc.Unit = "";
	Map_Values.add_value(Value_Desc);
	int Map_Ptr = Map_Values.get_value_ptr(Value_Desc.Value_Name);
	double microfrac;

	for(long i=0;i<(signed)Data_Point_Set[Data_Point_Set_Id].Data_Points.size();i++)
	{

	if( i%10 == 1 )
	{
		Progress_Form->replace_last_line_with_text("Calculating microfractionation, processing dp: " + IntToStr((int)i) + "/" +
			IntToStr((int)(Data_Point_Set[Data_Point_Set_Id].Data_Points.size())) );
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	double TimeStep_ms = Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Time_Step_ms;

	microfrac = PNUM.get_micro_fractionation(
		&Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Roving_Signal.Voltage_Values,
		Comp_Module_Ptr->Microfractionation_Algorithm,
		Comp_Module_Ptr->Microfractionation_percentile_threshold,
		50./TimeStep_ms );

	Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points[i].set_value(
				 Value_Desc.Value_Name, microfrac,Map_Values.get_values_table_ref() );

	} // through data points

	// interpolate map
	Progress_Form->add_text("Interpolating map...");
	interpolate_specific_value(0,Data_Point_Set_Id,Map_Ptr,
			Progress_Form);

	return Value_Desc.Value_Name;
}

//------------------------------------------------------------------------------

AnsiString Surface_Class::calculate_electric_dispersion_map(int Data_Point_Set_Id,
		int Algorithm,double Base_AFCL_ms,bool Auto_Range,double Disp_Range_ms,
		int Min_DP_Number, Computational_Module_Class *Comp_Module_Ptr)
{
	Progress_Form->add_text("Calculating electric dispersion map...");
	Progress_Form->Show();
	Application->ProcessMessages();

	// add value to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS;
	Value_Desc.Displayed_In_Table = true;
	Value_Desc.Inverted_Palette = true;
	Value_Desc.Value_Name = "Electric_dispersion";
	Value_Desc.Unit = "";
	Map_Values.add_value(Value_Desc);

	int Map_Ptr = Map_Values.get_value_ptr(Value_Desc.Value_Name);
	double disp,Min,Max,Mean,SDev,SUM,Continuous_Activity_Ratio;
	vector <long> GroupList;
	long Group_Id;
	bool Threshold_Crossed;
	std::vector <long> Processed_DP_Groups_Ids;
	std::vector <int> Silence_Vector;
	Silence_Vector.clear();
	long EGM_Length = Data_Point_Set[Data_Point_Set_Id].Data_Points[0].Roving_Signal.Voltage_Values.size();
	const int v=0;
	double TimeStep_ms = Data_Point_Set[Data_Point_Set_Id].Data_Points[0].Roving_Signal.Time_Step_ms;
	Silence_Vector.assign(EGM_Length,v); // 0 - no silence, 1 - silence in all EGMS
	long Sliding_window_size = Comp_Module_Ptr->Sliding_Window_Size_for_dispersion_calculation_ms/TimeStep_ms;
	double Threshold = Comp_Module_Ptr->Voltage_threshold_for_dispersion_calculation_mV;

	//--------------------------------------------------
	//--------------------------------------------------
	//--------------------------------------------------
	if( Algorithm == 0 ) // based on voltage threshold
	//--------------------------------------------------
	//--------------------------------------------------
	//--------------------------------------------------
	{

	//--------------------------------------------------
	// go through all data points
	//--------------------------------------------------
	for(long i=0;i<(signed)Data_Point_Set[Data_Point_Set_Id].Data_Points.size();i++)
	if( !PNUM.is_value_present_in_vector_long(&Processed_DP_Groups_Ids,
			Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Identifier ) )
	{

	Progress_Form->replace_last_line_with_text("Calculating electric dispersion, processing dp: " + IntToStr((int)i) + "/" +
			IntToStr((int)(Data_Point_Set[Data_Point_Set_Id].Data_Points.size())) );
	Progress_Form->Show();
	Application->ProcessMessages();

	Group_Id = Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Identifier;
	Processed_DP_Groups_Ids.push_back(Group_Id);

	// create list of data points with given group_id
	GroupList.clear();
	for(long dp=0;dp<Data_Point_Set[Data_Point_Set_Id].Data_Points.size();dp++)
	if( Data_Point_Set[Data_Point_Set_Id].Data_Points[dp].Identifier == Group_Id )
	GroupList.push_back(dp);

	for(long rr=0;rr<EGM_Length;rr++)
	Silence_Vector[rr]=0;

	// go through all sliding windows positions
	//-------------------------------------------------------------------
	for(long t=0.5*Sliding_window_size;t<EGM_Length-0.5*Sliding_window_size;t++)
	//-------------------------------------------------------------------
	{

	Threshold_Crossed = false;

	//-----------
	// go through data points only belonging to the group
	//-----------
	for(long j=0;j<(signed)GroupList.size();j++)
	{
		// get mean in current window
		PNUM.calculate_max_min_mean_vec_ranged(
			&Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.Voltage_Values,
				t-0.5*Sliding_window_size, t+0.5*Sliding_window_size,
				&Min, &Max, &Mean,&SDev);
		if( Max > Threshold || Min < -Threshold)
			Threshold_Crossed = true;

	} // thorugh all egms

	if(!Threshold_Crossed)
	{
		for(long tt= t-0.5*Sliding_window_size;tt<=t+0.5*Sliding_window_size;tt++)
		Silence_Vector[tt]=1;

		t += 0.5*Sliding_window_size; // jump half window because we know its silent anyway
	}

	} // through all time points

	SUM=0;
	for(long t=0;t<Silence_Vector.size();t++)
	if( Silence_Vector[t] == 1 )
		SUM++;

	if( Silence_Vector.size() != 0 )
	SUM /= (double) Silence_Vector.size();

	Continuous_Activity_Ratio = 1-SUM;

	if( GroupList.size() < Min_DP_Number )
	{
		Continuous_Activity_Ratio = 0;
		for(long rr=0;rr<EGM_Length;rr++) Silence_Vector[rr]=1;
	}

	// assign results: silence_vector and COR value
	for(long k=0;k<(signed)GroupList.size();k++)
	{
	Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[k]].set_value(
				 Value_Desc.Value_Name, Continuous_Activity_Ratio,
				 Map_Values.get_values_table_ref() );
	Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[k]].
		Roving_Signal.Silence_Vector = Silence_Vector;
	}

	} // through data points
	} // alg == 0

	//--------------------------------------------------
	//--------------------------------------------------
	//--------------------------------------------------
	if( Algorithm == 1 ) // based on local deflections
	//--------------------------------------------------
	//--------------------------------------------------
	//--------------------------------------------------
	{
//xxx
	int Activity_Range_ptr = Disp_Range_ms/TimeStep_ms; // width of the red region
	int Base_AFCL_ptr = Base_AFCL_ms / TimeStep_ms;

	//--------------------------------------------------
	// go through all data points
	//--------------------------------------------------
	for(long i=0;i<(signed)Data_Point_Set[Data_Point_Set_Id].Data_Points.size();i++)
	if( !PNUM.is_value_present_in_vector_long(&Processed_DP_Groups_Ids,
			Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Identifier ) )
	{

	Progress_Form->replace_last_line_with_text("Calculating electric dispersion, processing dp: " + IntToStr((int)i) + "/" +
			IntToStr((int)(Data_Point_Set[Data_Point_Set_Id].Data_Points.size())) );
	Progress_Form->Show();
	Application->ProcessMessages();

	Group_Id = Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Identifier;
	Processed_DP_Groups_Ids.push_back(Group_Id);

	// create list of data points with given group_id
	GroupList.clear();
	for(long dp=0;dp<Data_Point_Set[Data_Point_Set_Id].Data_Points.size();dp++)
	if( Data_Point_Set[Data_Point_Set_Id].Data_Points[dp].Identifier == Group_Id )
	GroupList.push_back(dp);

	// update range if automatic
	if(Auto_Range)
	Activity_Range_ptr = Base_AFCL_ms/(2.0*(double)GroupList.size())/TimeStep_ms;

	for(long rr=0;rr<EGM_Length;rr++)
	Silence_Vector[rr]=1;

	//-----------
	// go through data points only belonging to the group
	//-----------
	for(long j=0;j<(signed)GroupList.size();j++)
	for(long loc=0;loc<Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
		Local_Activations.Local_Activations_vec.size();loc++)
//	if( Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
//		Local_Activations.Local_Activations_vec[loc].Timing_In_EGM_ptr > Base_AFCL_ptr &&
//		Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
//		Local_Activations.Local_Activations_vec[loc].Timing_In_EGM_ptr < EGM_Length-Base_AFCL_ptr )
	for(long tt=Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
		Local_Activations.Local_Activations_vec[loc].Timing_In_EGM_ptr-Activity_Range_ptr;
			tt<Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
		Local_Activations.Local_Activations_vec[loc].Timing_In_EGM_ptr+Activity_Range_ptr;tt++)
	if( tt >= 0 && tt < Silence_Vector.size() )
	Silence_Vector[tt]=0;

	SUM=0;
	for(long t=0;t<Silence_Vector.size();t++)
	if( Silence_Vector[t] == 1 )
		SUM++;

	if( Silence_Vector.size() != 0 )
	SUM /= (double) Silence_Vector.size();

	Continuous_Activity_Ratio = 1-SUM;

	if( GroupList.size() < Min_DP_Number )
	{
		Continuous_Activity_Ratio = NOT_POSSIBLE_TO_CALCULATE_VALUE;
		for(long rr=0;rr<EGM_Length;rr++) Silence_Vector[rr]=1;
	}

	// assign results: silence_vector and COR value
	for(long k=0;k<(signed)GroupList.size();k++)
	{
	Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[k]].set_value(
				 Value_Desc.Value_Name, Continuous_Activity_Ratio,Map_Values.get_values_table_ref() );
	Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[k]].Roving_Signal.Silence_Vector = Silence_Vector;
	}


	} // through data points

	} // alg == 1


	//--------------------------------------------------
	//--------------------------------------------------
	//--------------------------------------------------
	if( Algorithm == 2 ) // based on surface activity mapping
	//--------------------------------------------------
	//--------------------------------------------------
	//--------------------------------------------------
	{
/*
	int Activity_Range_ptr = Disp_Range_ms/TimeStep_ms; // width of the red region
	int Base_AFCL_ptr = Base_AFCL_ms / TimeStep_ms;

	calculate_AFCL_map(Data_Point_Set_Id,Base_AFCL_ms,Comp_Module_Ptr);

	//--------------------------------------------------
	// go through all data points
	//--------------------------------------------------
	for(long i=0;i<(signed)Data_Point_Set[Data_Point_Set_Id].Data_Points.size();i++)
	if( !PNUM.is_value_present_in_vector_long(&Processed_DP_Groups_Ids,
			Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Identifier ) )
	{

	Progress_Form->replace_last_line_with_text("Calculating electric dispersion (alg3:surface), processing dp: " + IntToStr((int)i) + "/" +
			IntToStr((int)(Data_Point_Set[Data_Point_Set_Id].Data_Points.size())) );
	Progress_Form->Show();
	Application->ProcessMessages();

	Group_Id = Data_Point_Set[Data_Point_Set_Id].Data_Points[i].Identifier;
	Processed_DP_Groups_Ids.push_back(Group_Id);

	// create list of data points with given group_id
	GroupList.clear();
	for(long dp=0;dp<Data_Point_Set[Data_Point_Set_Id].Data_Points.size();dp++)
	if( Data_Point_Set[Data_Point_Set_Id].Data_Points[dp].Identifier == Group_Id )
	GroupList.push_back(dp);

	for(long rr=0;rr<EGM_Length;rr++)
	Silence_Vector[rr]=1;

	//--------------------------------------------------
	// 1. Map which nodes belong to the group
	//--------------------------------------------------

	//-----------
	// go through data points only belonging to the group
	//-----------
	for(long j=0;j<(signed)GroupList.size();j++)
	for(long loc=0;loc<Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
		Local_Activations.Local_Activations_vec.size();loc++)
//	if( Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
//		Local_Activations.Local_Activations_vec[loc].Timing_In_EGM_ptr > Base_AFCL_ptr &&
//		Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
//		Local_Activations.Local_Activations_vec[loc].Timing_In_EGM_ptr < EGM_Length-Base_AFCL_ptr )
	for(long tt=Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
		Local_Activations.Local_Activations_vec[loc].Timing_In_EGM_ptr-Activity_Range_ptr;
			tt<Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[j]].Roving_Signal.
		Local_Activations.Local_Activations_vec[loc].Timing_In_EGM_ptr+Activity_Range_ptr;tt++)
	if( tt >= 0 && tt < Silence_Vector.size() )
	Silence_Vector[tt]=0;

	//--------------------------------------------------
	// 2. Calculate "silence vectors" for each node
	//--------------------------------------------------



	}

	//--------------------------------------------------
	// 3. Calculate master silence vector based on all per-node silence vectors
	//--------------------------------------------------
	for( nodes )
	for( time )
	if( node[n].Silence_Vector[t] == 1 ) // silence time pont
	Silence_Vector[t]=0; // master silence vector marked


	SUM=0;
	for(long t=0;t<Silence_Vector.size();t++)
	if( Silence_Vector[t] == 1 )
		SUM++;

	if( Silence_Vector.size() != 0 )
	SUM /= (double) Silence_Vector.size();

	Continuous_Activity_Ratio = 1-SUM;

	if( GroupList.size() <= 2 )
	{
		Continuous_Activity_Ratio = 0;
		for(long rr=0;rr<EGM_Length;rr++) Silence_Vector[rr]=1;
	}

	// assign results: silence_vector and COR value
	for(long k=0;k<(signed)GroupList.size();k++)
	{
	Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[k]].set_value(
				 Value_Desc.Value_Name, Continuous_Activity_Ratio,Map_Values.get_values_table_ref() );
	Data_Point_Set[Data_Point_Set_Id].Data_Points[GroupList[k]].Roving_Signal.Silence_Vector = Silence_Vector;
	}


	} // through data points

*/
	} // alg == 2

	// interpolate map
	Progress_Form->add_text("Interpolating map...");
	interpolate_specific_value(0,Data_Point_Set_Id,Map_Ptr,
			Progress_Form);

	return Value_Desc.Value_Name;
}

//------------------------------------------------------------------------------
AnsiString Surface_Class::calculate_conduction_velocity(int Base_Map_Value_Ptr)
{
	AnsiString Base_Map_Value_Name =
		Map_Values.get_value_name_according_to_ptr(Base_Map_Value_Ptr);

	// add value to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_GEO_NODES;
	Value_Desc.Interpolation_Type = INTERPOLATION_TYPE_FLAG_NO_INTERPOLATION;
	Value_Desc.Displayed_In_Table = true;
	Value_Desc.Value_Name = "Velocity_based_on_" + Base_Map_Value_Name;
	Value_Desc.Unit = "m/s";
	Map_Values.add_value(Value_Desc);

	int CV_map_Ptr = Map_Values.get_value_ptr(Value_Desc.Value_Name);

	// clear values at triangles
	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
		Surface_Triangle_Set[i].Tmp_Flag = -1000;

	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
		Surface_Node_Set[i].coloured = 0;

	double OA,OB,AB,cos_teta,tOB,tOA,sin_teta;
	double tg_alfa,cos_alfa,tg_beta,cos_beta;
	int O_ptr,A_ptr,B_ptr;
	double min,CV;
	double Max_CV_for_Allessie = 4; // max accepted velocity

	// main loop
	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	if( Surface_Triangle_Set[i].Removed_Flag != ON )
	if( triangle_values_valid(i) )
	{

		min = get_min( Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].
							get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr),
					   Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].
							get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr),
					   Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].
							get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr));

	if( min == Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].
				get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr) )
		{   O_ptr = Surface_Triangle_Set[i].Nodes[0];
			A_ptr = Surface_Triangle_Set[i].Nodes[1];
			B_ptr = Surface_Triangle_Set[i].Nodes[2];
		}
	if( min == Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].
				get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr) )
		{   O_ptr = Surface_Triangle_Set[i].Nodes[1];
			A_ptr = Surface_Triangle_Set[i].Nodes[0];
			B_ptr = Surface_Triangle_Set[i].Nodes[2];
		}
	if( min == Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].
			get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr) )
		{
			O_ptr = Surface_Triangle_Set[i].Nodes[2];
			A_ptr = Surface_Triangle_Set[i].Nodes[0];
			B_ptr = Surface_Triangle_Set[i].Nodes[1];
		}

		OA = sqrt( pow(Surface_Node_Set[O_ptr].x-Surface_Node_Set[A_ptr].x,2)+
				   pow(Surface_Node_Set[O_ptr].y-Surface_Node_Set[A_ptr].y,2)+
				   pow(Surface_Node_Set[O_ptr].z-Surface_Node_Set[A_ptr].z,2) );

		AB = sqrt( pow(Surface_Node_Set[B_ptr].x-Surface_Node_Set[A_ptr].x,2)+
				   pow(Surface_Node_Set[B_ptr].y-Surface_Node_Set[A_ptr].y,2)+
				   pow(Surface_Node_Set[B_ptr].z-Surface_Node_Set[A_ptr].z,2) );

		OB = sqrt( pow(Surface_Node_Set[O_ptr].x-Surface_Node_Set[B_ptr].x,2)+
				   pow(Surface_Node_Set[O_ptr].y-Surface_Node_Set[B_ptr].y,2)+
				   pow(Surface_Node_Set[O_ptr].z-Surface_Node_Set[B_ptr].z,2) );

		tOA = Surface_Node_Set[A_ptr].get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr) -
			  Surface_Node_Set[O_ptr].get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr);
		tOB = Surface_Node_Set[B_ptr].get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr) -
			  Surface_Node_Set[O_ptr].get_value(Current_Data_Point_Set_Ptr,Base_Map_Value_Ptr);

		if( OA*OB != 0 )
		cos_teta = (OA*OA+OB*OB-AB*AB)/(2.0*OA*OB);
		else
		cos_teta = 0.0;

		sin_teta = sqrt( 1 - cos_teta*cos_teta );

		if( tOA != 0 )
		{
			if( sin_teta != 0 && tOA*OB != 0 )
			tg_alfa = ( (tOB*OA)/(tOA*OB) - cos_teta )/(sin_teta);
			else
			tg_alfa = 0.0;

			cos_alfa = 1.0/( sqrt(1+tg_alfa*tg_alfa ));
			Surface_Triangle_Set[i].Tmp_Flag = OA/tOA * cos_alfa;
		}

		if( tOB != 0 )
		{
			if( sin_teta != 0 && tOB*OA != 0 )
			tg_beta = ( (tOA*OB)/(tOB*OA) - cos_teta )/(sin_teta);
			else
			tg_beta = 0.0;

			cos_beta = 1.0/( sqrt(1+tg_beta*tg_beta ));
			Surface_Triangle_Set[i].Tmp_Flag = OB/tOB * cos_beta;
		}

		if( Surface_Triangle_Set[i].Tmp_Flag > Max_CV_for_Allessie )
			Surface_Triangle_Set[i].Tmp_Flag = -1000; // back to non-computed state
	} // through triangles

	// Transfer CV from triangles to nodes
	double counter;
	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	{
		Surface_Node_Set[i].Flag_A = 0;
		counter = 0;
		CV = 0;

		for(unsigned long k=0;k<Surface_Node_Set[i].Neig_Triangles.size();k++)
		if( Surface_Triangle_Set[ Surface_Node_Set[i].Neig_Triangles[k] ].Tmp_Flag != -1000 ) // get only computed values
		{
			Surface_Node_Set[i].coloured = 1; // it will be a source for interpolation
			counter++;
			CV += Surface_Triangle_Set[ Surface_Node_Set[i].Neig_Triangles[k] ].Tmp_Flag;
		}

		if( counter > 0 )
		Surface_Node_Set[i].set_value(Current_Data_Point_Set_Ptr,
								CV_map_Ptr,CV/(double)counter);
		else
		Surface_Node_Set[i].set_value(Current_Data_Point_Set_Ptr,
								CV_map_Ptr,NOT_POSSIBLE_TO_CALCULATE_VALUE);
	}
/*
	// mark non-computed nodes as removed
	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	if( Surface_Node_Set[i].get_value(Current_Data_Point_Set_Ptr,CV_map_Ptr) == 0 )
	{
		Surface_Node_Set[i].Removed_Geometry_Vertex_Flag = 1;
		for(unsigned long k=0;k<Surface_Node_Set[i].Neig_Triangles.size();k++)
		Surface_Triangle_Set[ Surface_Node_Set[i].Neig_Triangles[k] ].Removed_Flag = ON;
	}
*/

	// interpolate

	// transfer CV to data points
	long node_ptr;
	for(long i=0;i<(signed)Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points.size();i++)
	{

	node_ptr = Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points[i].Closest_Node_Id;

	if( node_ptr >= 0 )
	Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points[i].set_value(
			 Value_Desc.Value_Name,
			 Surface_Node_Set[node_ptr].get_value(Current_Data_Point_Set_Ptr,CV_map_Ptr),
			 Map_Values.get_values_table_ref() );
	} // through data points

	return Value_Desc.Value_Name;
}

//---------------------------------------------------------------------------

double Surface_Class::get_min(double A,double B,double C)
{
	if( A <= B && A <= C )
	return A;

	if( B <= A && B <= C )
	return B;

	if( C <= A && C <= B )
	return C;

	return 0;
}
//-----------------------------------------------------------------------
double Surface_Class::get_middle(double A,double B,double C)
{
    if( (A <= C && A >= B)  ||( A <= B && A >= C))
	return A;

    if( (B <= C && B >= A)  || (B <= A && B >= C))
	return B;

    if( (C <= A && C >= B)  || (C <= B && C >= A))
	return C;

	return 0;
}

//-----------------------------------------------------------------------
double Surface_Class::get_max(double A,double B,double C)
{
    if( A >= B && A >= C )
    return A;

    if( B >= A && B >= C )
    return B;

    if( C >= A && C >= B )
    return C;

    return 0;
}

//----------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// function calculating  minimum and maximum values of the current value in
	// current data point set. Used to scale display palette.
	//---------------------------------------------------------------------------------

int Surface_Class::compute_min_max_values_on_surface()
{
	double Min_Value_On_Map = MAXDOUBLE;
	double Max_Value_On_Map = -MAXDOUBLE;
	int Val_Ptr = Map_Values.get_current_value_ptr();
	double v;

//	if( !Map_Values.get_current_value_fixed_palette_flag() ) // compute only if not fixed palette
	if( Val_Ptr >= 0 )
	{

	// case of value based on DPs
	if( Map_Values.get_value_type_according_to_ptr(Val_Ptr) == VALUE_BASED_ON_DATA_POINTS ) // data point based value
	if( data_points_set_ptr_in_range() )
	if( Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points.size() >= 3 )
	for(unsigned long j=0;j<Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points.size();j++)
	{
		v = Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points[j].get_value(
				Map_Values.Current_Map_Value_Name,Map_Values.get_values_table_ref());

		if( v != NOT_POSSIBLE_TO_CALCULATE_VALUE )
		{
			if( v >= Max_Value_On_Map )
				Max_Value_On_Map = v;

			if( v <= Min_Value_On_Map )
				Min_Value_On_Map = v;
		}
	}

	// case of value based on nodes
	if( Map_Values.get_value_type_according_to_ptr(Val_Ptr) == VALUE_BASED_ON_GEO_NODES ) // surface node based value
	if( data_points_set_ptr_in_range() )
	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	{
		v = Surface_Node_Set[n].get_value(Current_Data_Point_Set_Ptr,Val_Ptr);

		if( v != NOT_POSSIBLE_TO_CALCULATE_VALUE )
		{
		if( v >= Max_Value_On_Map )
			Max_Value_On_Map = v;

		if( v <= Min_Value_On_Map )
			Min_Value_On_Map = v;
		}
	}

	// assign values
	if( !Map_Values.get_fixed_palette_flag(Val_Ptr) )
	Map_Values.set_value_minmax(Val_Ptr,Min_Value_On_Map,Max_Value_On_Map);

	}

	return OK_RESULT;
}

//--------------------------------------------------------------------------

void Surface_Class::generate_contours(double Threshold_Interval)
{
	double Min,Max;
	double v1,v2,v3;
	double x1,y1,z1,x2,y2,z2,x3,y3,z3;
	double Lambda;
	double Distance_From_Geo = 0.1;

	Contour_Point P;
	Contour_Line L;

	int Current_Map_Value_Ptr = Map_Values.get_current_value_ptr();

	if( Current_Map_Value_Ptr >= 0 )
	if( data_points_set_ptr_in_range() == ON )
	{

	Map_Values.get_current_value_minmax(&Min, &Max);

	for(unsigned long t=0;t<Surface_Triangle_Set.size();t++)
	{

	x1 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].x+
		 Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].normal[0]*Distance_From_Geo;
	y1 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].y+
		 Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].normal[1]*Distance_From_Geo;
	z1 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].z+
		 Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].normal[2]*Distance_From_Geo;
	v1 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].
		get_value(Current_Data_Point_Set_Ptr,Current_Map_Value_Ptr);

	x2 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].x+
		 Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].normal[0]*Distance_From_Geo;
	y2 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].y+
		 Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].normal[1]*Distance_From_Geo;
	z2 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].z+
		 Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].normal[2]*Distance_From_Geo;
	v2 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].
		get_value(Current_Data_Point_Set_Ptr,Current_Map_Value_Ptr);

	x3 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].x+
		 Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].normal[0]*Distance_From_Geo;
	y3 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].y+
		 Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].normal[1]*Distance_From_Geo;
	z3 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].z+
		 Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].normal[2]*Distance_From_Geo;
	v3 = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].
		get_value(Current_Data_Point_Set_Ptr,Current_Map_Value_Ptr);

	if( v1 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v2 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v3 != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{

	// 1. find points
	Point_Set.clear();

	for(double v=Min;v<=Max;v+=Threshold_Interval)
	{
		if( v == v1 ) v1 += 0.0001;
		if( v == v2 ) v2 += 0.0001;
		if( v == v3 ) v3 += 0.0001;

		// v1 - v2 edge
		if( v1 != v2 )
		if( min(v1,v2) <= v && v <= max(v1,v2) )
		{
			P.v = v;
			Lambda = (v-v2)/(v1-v2);
			P.x = Lambda*x1 + (1-Lambda)*x2;
			P.y = Lambda*y1 + (1-Lambda)*y2;
			P.z = Lambda*z1 + (1-Lambda)*z2;
			Point_Set.push_back(P);
		}

		// v1 - v3 edge
		if( v1 != v3 )
		if( min(v1,v3) <= v && v <= max(v1,v3) )
		{
			P.v = v;
			Lambda = (v-v3)/(v1-v3);
			P.x = Lambda*x1 + (1-Lambda)*x3;
			P.y = Lambda*y1 + (1-Lambda)*y3;
			P.z = Lambda*z1 + (1-Lambda)*z3;
			Point_Set.push_back(P);
		}

		// v3 - v2 edge
		if( v3 != v2 )
		if( min(v3,v2) <= v && v <= max(v3,v2) )
		{
			P.v = v;
			Lambda = (v-v2)/(v3-v2);
			P.x = Lambda*x3 + (1-Lambda)*x2;
			P.y = Lambda*y3 + (1-Lambda)*y2;
			P.z = Lambda*z3 + (1-Lambda)*z2;
			Point_Set.push_back(P);
		}
	}

	// 2. Joind points in pairs
	for(unsigned long i=0;i<Point_Set.size();i++)
	for(unsigned long j=0;j<Point_Set.size();j++)
	if( i != j )
	if( Point_Set[i].Flag != 1 )
	if( Point_Set[j].Flag != 1 )
	if( Point_Set[i].v == Point_Set[j].v )
	{
		L.x1 = Point_Set[i].x;
		L.y1 = Point_Set[i].y;
		L.z1 = Point_Set[i].z;

		L.x2 = Point_Set[j].x;
		L.y2 = Point_Set[j].y;
		L.z2 = Point_Set[j].z;

		Contours_Set.push_back(L);

		Point_Set[i].Flag = 1;
		Point_Set[j].Flag = 1;
	}

	} // if all 3 values valid

	} // for: triangles
	} // valid current value
}

//-----------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------
	// Simple function returning total number of data points in all sets.
	//---------------------------------------------------------------------------------

long Surface_Class::get_data_point_total_number_in_all_sets()
{
	long number=0;

	for(unsigned long i=0;i<Data_Point_Set.size();i++)
		number += Data_Point_Set[i].Data_Points.size();

	return number;
}

//---------------------------------------------------------------------------

double Surface_Class::get_map_percentile(int Val_Id, int DP_Set, int Percentile)
{
	vector <double> List;
	List.clear();
	double v,v1,v2,v3;

	for(unsigned long i=0; i<Surface_Triangle_Set.size(); i++)
//	if( (Seg_Id != 0 && Surface_Triangle_Set[i].Segment_Id == Seg_Id) || Seg_Id == 0 )
	if( Surface_Triangle_Set[i].Removed_Flag != ON )
	{

	v1 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].get_value(DP_Set,Val_Id);
	v2 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].get_value(DP_Set,Val_Id);
	v3 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].get_value(DP_Set,Val_Id);

	v = ( v1+v2+v3 )/3.0;

	if( v1 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v2 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v3 != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	List.push_back(v);

	} // for all triangles

	double MinV, MaxV, Box_Size;
	long Min_Ptr, Max_Ptr;
	PNUM.find_min_max(&List,&MinV, &MaxV, &Min_Ptr, &Max_Ptr);

	std::vector <long> Histo = PNUM.get_histogram(&List,0,true,&Box_Size);

	return PNUM.get_percentile_from_histogram(&Histo,Percentile,MinV,Box_Size);
}

//---------------------------------------------------------------------------

int Surface_Class::calculate_statistics_for_Values(int Seg_Id,int Val_Id,
				double* av,double* median,double* min,double* max,double* sd,double* covc,
				int DP_Set,bool Exclude_Zero_Values)
{
	double v=0,v1=0,v2=0,v3=0;
	long n=0;
	vector <double> List;

	// types:
	// 0 : whole
	// 1 : scar area
	// 2 : BZ
	// 3 : healthy
	// 4 : not scar

	double Mean = 0.0;
	double Min = MAXDOUBLE;
	double Max = -MAXDOUBLE;
	double SD = 0.0;
	double Tot_Field = 0.0;
	double CovC = 0.0;

	List.clear();

	// Mean calculation
	for(unsigned long i=0; i<Surface_Triangle_Set.size(); i++)
	if( (Seg_Id != 0 && Surface_Triangle_Set[i].Segment_Id == Seg_Id) || Seg_Id == 0 )
	if( Surface_Triangle_Set[i].Removed_Flag != ON )
	if( triangle_values_valid_specific_value(i,Val_Id) )
	{

	v1 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].get_value(DP_Set,Val_Id);
	v2 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].get_value(DP_Set,Val_Id);
	v3 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].get_value(DP_Set,Val_Id);

	v = ( v1+v2+v3 )/3.0;

	if( v1 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v2 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v3 != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{
	 if( !Exclude_Zero_Values || (Exclude_Zero_Values && v1!=0 && v2!=0 && v3!=0))
	 {
		 // statistic calculation
		 if( v1 > Max ) Max = v1;
		 if( v1 < Min ) Min = v1;

		 if( v2 > Max ) Max = v2;
		 if( v2 < Min ) Min = v2;

		 if( v3 > Max ) Max = v3;
		 if( v3 < Min ) Min = v3;

		 Mean += Surface_Triangle_Set[i].Area_cm2*v;
		 Tot_Field += Surface_Triangle_Set[i].Area_cm2;
		 List.push_back(v);

	 }

	} // if triangle goes to statistics

	} // for all triangles


	if( Tot_Field != 0)
	{
		Mean /= Tot_Field;
	}
	else
	{
		Mean = 0;
		Min = 0;
		Max = 0;
	}


	n = 0;
	for(unsigned long i=0; i<Surface_Triangle_Set.size(); i++)
	if( (Seg_Id != 0 && Surface_Triangle_Set[i].Segment_Id == Seg_Id) || Seg_Id == 0 )
	if( Surface_Triangle_Set[i].Removed_Flag != ON)
	if( triangle_values_valid_specific_value(i,Val_Id) )
	{

	v1 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].get_value(DP_Set,Val_Id);
	v2 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].get_value(DP_Set,Val_Id);
	v3 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].get_value(DP_Set,Val_Id);

	v = ( v1+v2+v3 )/3.0;

	if( v1 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v2 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v3 != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{
	 if( !Exclude_Zero_Values || (Exclude_Zero_Values && v1!=0 && v2!=0 && v3!=0))
	 {
	   n++;
	   SD += Surface_Triangle_Set[i].Area_cm2*pow(v - Mean,2);
	 }

	} // if triangle falls into statistic

	} // for all triangles


	if( SD > 0 && Tot_Field != 0)
	{
		SD = sqrt(SD/Tot_Field);
	}
	else
	{
		SD = 0.0;
	}

	if(Mean!=0)
		CovC = 100.0*SD/Mean;
	else
		CovC = 0;

	av[0] = Mean;
	min[0] = Min;
	max[0] = Max;
	sd[0] = SD;
	covc[0] = CovC;

	if( List.size() > 3 )
	{
	sort(List.begin(),List.end());
	median[0] =List[ (int)(0.5*List.size()) ];
	}
	else
	median[0] = 0;

	return 1;
}

//---------------------------------------------------------------------------

int Surface_Class::calculate_area_taken_by_Value_within_range(int Seg_No,int Val_Id,
		double Threshold_Low,double Threshold_High,
		double* Area_Taken,double* Total_Area,int DP_Set)
{
	double v1=0,v2=0,v3=0;
	double area = 0;
	double tot_area = 0;

	for(unsigned long i=0; i<Surface_Triangle_Set.size(); i++)
	if( (Seg_No != 0 && Surface_Triangle_Set[i].Segment_Id == Seg_No) || Seg_No == 0 )
	if( Surface_Triangle_Set[i].Removed_Flag != ON )
	if( triangle_values_valid_specific_value(i,Val_Id) )
//	if( Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].Type != SCAR_TYPE )
//	if( Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].Type != SCAR_TYPE )
//	if( Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].Type != SCAR_TYPE )
	{

	v1 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].get_value(DP_Set,Val_Id);
	v2 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].get_value(DP_Set,Val_Id);
	v3 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].get_value(DP_Set,Val_Id);

	if( v1 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v2 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v3 != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{

	if( v1 >= Threshold_Low && v2 >= Threshold_Low && v3 >= Threshold_Low &&
		v1 <= Threshold_High && v2 <= Threshold_High && v3 <= Threshold_High )
	area += Surface_Triangle_Set[i].Area_cm2;

	tot_area += Surface_Triangle_Set[i].Area_cm2;

	}

	} // for all triangles

	Area_Taken[0] = area;
	Total_Area[0] = tot_area;

	return 1;
}

//---------------------------------------------------------------------------

int Surface_Class::calculate_area_taken_by_Value(int Seg_No,int Val_Id,
				double Threshold,int Direction,
				double* Area_Taken,double* Total_Area,int DP_Set)
{
	double v1=0,v2=0,v3=0;
	double area = 0;
	double tot_area = 0;

	for(unsigned long i=0; i<Surface_Triangle_Set.size(); i++)
	if( (Seg_No != 0 && Surface_Triangle_Set[i].Segment_Id == Seg_No) || Seg_No == 0 )
	if( Surface_Triangle_Set[i].Removed_Flag != ON )
	if( triangle_values_valid_specific_value(i,Val_Id) )
//	if( Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].Type != SCAR_TYPE )
//	if( Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].Type != SCAR_TYPE )
//	if( Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].Type != SCAR_TYPE )
	{

	v1 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].get_value(DP_Set,Val_Id);
	v2 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].get_value(DP_Set,Val_Id);
	v3 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].get_value(DP_Set,Val_Id);

	if( v1 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v2 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v3 != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{

	if( ( Direction==1 && ( v1 >= Threshold && v2 >= Threshold && v3 >= Threshold )) ||
		( Direction==-1 && ( v1 < Threshold || v2 < Threshold || v3 < Threshold) ) )
	area += Surface_Triangle_Set[i].Area_cm2;

	tot_area += Surface_Triangle_Set[i].Area_cm2;

	}

	} // for all triangles

	Area_Taken[0] = area;
	Total_Area[0] = tot_area;

	return 1;
}

//---------------------------------------------------------------------------

void Surface_Class::find_neig_nodes_and_neig_triangles()
{
	unsigned long n0,n1,n2;

	//-------------------------------------------
	// Find neig nodes for each node
	//-------------------------------------------
	long NN = Surface_Node_Set.size();
	long TT = Surface_Triangle_Set.size();

	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	{
		Surface_Node_Set[i].Neig_Triangles.clear();
		Surface_Node_Set[i].Neighbors.clear();
	}

	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	{

	n0 = Surface_Triangle_Set[i].Nodes[0];
	n1 = Surface_Triangle_Set[i].Nodes[1];
	n2 = Surface_Triangle_Set[i].Nodes[2];

	if( n0 >= Surface_Node_Set.size() ||
		n1 >= Surface_Node_Set.size() ||
		n2 >= Surface_Node_Set.size() )
	{
		ShowMessage("Critical error in parsing geometry section (find neigs)");
		exit(0);
	}

	if( n0 < Surface_Node_Set.size() &&
		n1 < Surface_Node_Set.size() &&
		n2 < Surface_Node_Set.size() )
	{

	// new neighs of n0
	if( !PNUM.is_value_present_in_vector_long(&Surface_Node_Set[n0].Neighbors,n1) )
	{
	Surface_Node_Set[n0].Neighbors.push_back(n1);
	}

	if( !PNUM.is_value_present_in_vector_long(&Surface_Node_Set[n0].Neighbors,n2) )
	{
	Surface_Node_Set[n0].Neighbors.push_back(n2);
	}

	// new neighs of n1
	if( !PNUM.is_value_present_in_vector_long(&Surface_Node_Set[n1].Neighbors,n0) )
	{
	Surface_Node_Set[n1].Neighbors.push_back(n0);
	}

	if( !PNUM.is_value_present_in_vector_long(&Surface_Node_Set[n1].Neighbors,n2) )
	{
	Surface_Node_Set[n1].Neighbors.push_back(n2);
	}

	// new neighs of n2
	if( !PNUM.is_value_present_in_vector_long(&Surface_Node_Set[n2].Neighbors,n0) )
	{
	Surface_Node_Set[n2].Neighbors.push_back(n0);
	}

	if( !PNUM.is_value_present_in_vector_long(&Surface_Node_Set[n2].Neighbors,n1) )
	{
	Surface_Node_Set[n2].Neighbors.push_back(n1);
	}

	}
	}

	//-------------------------------------------
	// Find neig triangles for each node
	//-------------------------------------------
	for(unsigned long j=0;j<Surface_Triangle_Set.size();j++)
	{
		n0 = Surface_Triangle_Set[j].Nodes[0];
		n1 = Surface_Triangle_Set[j].Nodes[1];
		n2 = Surface_Triangle_Set[j].Nodes[2];

	if( n0 < Surface_Node_Set.size() &&
		n1 < Surface_Node_Set.size() &&
		n2 < Surface_Node_Set.size() )
	{
		Surface_Node_Set[n0].Neig_Triangles.push_back(j);
		Surface_Node_Set[n1].Neig_Triangles.push_back(j);
		Surface_Node_Set[n2].Neig_Triangles.push_back(j);
	}
	}
}

//-------------------------------------------------------------------------------

double Surface_Class::get_mean_triangle_area()
{
	double av=0,counter=0;
	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	{
		av += Surface_Triangle_Set[i].Area_cm2;
		counter++;
	}

	if(counter!=0)
	return av/counter;
	else
    return 0;
}

//-------------------------------------------------------------------------------

void Surface_Class::calculate_areas_of_triangles()
{
	long n1,n2,n3;

	double x1,x2,x3,y1,y2,y3,z1,z2,z3;

	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	{
		n1 = Surface_Triangle_Set[i].Nodes[0];
		n2 = Surface_Triangle_Set[i].Nodes[1];
		n3 = Surface_Triangle_Set[i].Nodes[2];

		x1 = Surface_Node_Set[n3].x - Surface_Node_Set[n1].x;
		y1 = Surface_Node_Set[n3].y - Surface_Node_Set[n1].y;
		z1 = Surface_Node_Set[n3].z - Surface_Node_Set[n1].z;

		x2 = Surface_Node_Set[n2].x - Surface_Node_Set[n1].x;
		y2 = Surface_Node_Set[n2].y - Surface_Node_Set[n1].y;
		z2 = Surface_Node_Set[n2].z - Surface_Node_Set[n1].z;

		// product
		x3 = y1*z2 - y2*z1;
		y3 = -(x1*z2 - x2*z1);
		z3 = x1*y2 - x2*y1;

	   Surface_Triangle_Set[i].Area_cm2 = 0.5*sqrt( x3*x3 + y3*y3 + z3*z3 ) / 100.;

	}
}
//-----------------------------------------------------------------------------

long Surface_Class::get_closest_node_to_xyz(double x,double y,double z,
			double Max_Distance_Th_mm)
{
	double min_distance,distance;
	int min_ptr;

	min_distance = 100000000.0;
	min_ptr = -1;

	for(unsigned long j=0;j<Surface_Node_Set.size();j++)
	if( Surface_Node_Set[j].Removed_Geometry_Vertex_Flag != ON)
	if( fabs(Surface_Node_Set[j].x - x) < Max_Distance_Th_mm)
	if( fabs(Surface_Node_Set[j].y - y) < Max_Distance_Th_mm)
	if( fabs(Surface_Node_Set[j].z - z) < Max_Distance_Th_mm)
	{

	// find closest surface node
	distance = std::pow( Surface_Node_Set[j].x - x, 2 ) +
			   std::pow( Surface_Node_Set[j].y - y, 2 ) +
			   std::pow( Surface_Node_Set[j].z - z, 2 );

	if ( distance < min_distance )
	{
		min_distance = distance;
		min_ptr = j;
	}

	}

	if( std::sqrt(min_distance) < Max_Distance_Th_mm )
	return min_ptr;
	else
	return -1;
}

//-----------------------------------------------------------------------------

void Surface_Class::update_closest_nodes_and_dps_after_dp_removal(long DS,long DP)
{
	update_closest_nodes_and_dps_around_xyz(
		Data_Point_Set[DS].Data_Points[DP].x,
		Data_Point_Set[DS].Data_Points[DP].y,
		Data_Point_Set[DS].Data_Points[DP].x,DS);
}

//-----------------------------------------------------------------------------

void Surface_Class::update_closest_nodes_and_dps_after_dp_undeleted(long DS,long DP)
{
	update_closest_nodes_and_dps_around_xyz(
		Data_Point_Set[DS].Data_Points[DP].x,
		Data_Point_Set[DS].Data_Points[DP].y,
		Data_Point_Set[DS].Data_Points[DP].x,DS);
}

//---------------------------------------------------------------------------

void Surface_Class::update_closest_nodes_and_dps_around_xyz(double x, double y,
		double z,long Data_Point_Set_Id)
{
	if( Data_Point_Set_Id >= 0 && Data_Point_Set_Id < Data_Point_Set.size() )
	{

	//------------------------------------------xxx-----------------------------------
	// main loop through nodes
	//-----------------------------------------------------------------------------
	for(unsigned long j=0;j<Surface_Node_Set.size();j++)
	if( !Surface_Node_Set[j].Removed_Geometry_Vertex_Flag )
	if( fabs(Surface_Node_Set[j].x - x) < Data_Points_Filling_Threshold_mm)
	if( fabs(Surface_Node_Set[j].y - y) < Data_Points_Filling_Threshold_mm)
	if( fabs(Surface_Node_Set[j].z - z) < Data_Points_Filling_Threshold_mm)
	{
		if(j%50==1)
		{
			Progress_Form->replace_last_line_with_text("Finding closest data points to node:"+FloatToStr(j)+"/"+FloatToStr(Surface_Node_Set.size()));
			Progress_Form->Show();
			Application->ProcessMessages();
		}

		find_closest_data_point_to_node(j,Data_Point_Set_Id);
	}

	//-----------------------------------------------------------------------------
	// main loop through data points
	//-----------------------------------------------------------------------------
	for(unsigned long dp=0;dp<Data_Point_Set[Data_Point_Set_Id].Data_Points.size();dp++)
	if( fabs(Data_Point_Set[Data_Point_Set_Id].Data_Points[dp].x - x) < Data_Points_Filling_Threshold_mm)
	if( fabs(Data_Point_Set[Data_Point_Set_Id].Data_Points[dp].y - y) < Data_Points_Filling_Threshold_mm)
	if( fabs(Data_Point_Set[Data_Point_Set_Id].Data_Points[dp].z - z) < Data_Points_Filling_Threshold_mm)
	{
		if(dp%50==1)
		{
			Progress_Form->replace_last_line_with_text("Finding closest node to data point:"+FloatToStr(dp)+"/"+Data_Point_Set[Data_Point_Set_Id].Data_Points.size());
			Progress_Form->Show();
			Application->ProcessMessages();
		}

		find_closest_node_to_data_point(dp,Data_Point_Set_Id);
	}

	} // if dset in range
}


//-----------------------------------------------------------------------------

void Surface_Class::interpolate_specific_value(int Algorithm_Type, int Data_Point_Set_Ptr,
		int Value_Ptr,TProgress_Form *Progress_Form)
{
	if( Data_Point_Set_Ptr >= 0 && Data_Point_Set_Ptr < Data_Point_Set.size() )
	if( Data_Point_Set[Data_Point_Set_Ptr].Data_Points.size() > 2 )
   {

	if( Algorithm_Type == 0) // "diffusion" interpolation
	{
		int res;

		//---------------------------------------------------------------------------------------------
		// 1st. Project data points onto surface
		//---------------------------------------------------------------------------------------------
		project_data_points_specific_value(Data_Point_Set_Ptr,Value_Ptr);

		bool Interpolate_Process_Flag=true;
		while(Interpolate_Process_Flag)
		{
			Interpolate_Process_Flag=false;
			interpolate_one_value_one_step(Value_Ptr,&Interpolate_Process_Flag,Data_Point_Set_Ptr);
		}

	} // alg 0

   } // if in range and data points number > 2

   Progress_Form->Hide();
}

//-----------------------------------------------------------------------------

void Surface_Class::interpolate_LAT_value(int Algorithm_Type, int Data_Point_Set_Ptr,
		TProgress_Form *Progress_Form)
{
	int LAT_value_ptr = Map_Values.get_LAT_value_ptr();
	interpolate_specific_value(Algorithm_Type, Data_Point_Set_Ptr, LAT_value_ptr,Progress_Form);
}

//-----------------------------------------------------------------------------
void Surface_Class::interpolate_Bipolar_amplitude_value(int Algorithm_Type, int Data_Point_Set_Ptr,
		TProgress_Form *Progress_Form)
{
	int Volt_Amp_value_ptr = Map_Values.get_Voltage_Amplitude_value_ptr();
	interpolate_specific_value(Algorithm_Type, Data_Point_Set_Ptr, Volt_Amp_value_ptr,Progress_Form);
}

//-----------------------------------------------------------------------------

void Surface_Class::interpolate_all_values(int Algorithm_Type, int Data_Point_Set_Ptr,
	TProgress_Form *Progress_Form)
{
	if( Data_Point_Set_Ptr >= 0 &&
		Data_Point_Set_Ptr < Data_Point_Set.size() )
	if( Data_Point_Set[Data_Point_Set_Ptr].Data_Points.size() > 2 )
	{

	if( Algorithm_Type == 0) // "diffusion" interpolation
	{
		int res;
		bool Interpolate_Process_Flag;

		//---------------------------------------------------------------------------------------------
		// 1st. Project data points onto surface
		//---------------------------------------------------------------------------------------------
		Progress_Form->add_text("Projecting data points...");
		Progress_Form->Show();
		Application->ProcessMessages();
		project_data_points(Data_Point_Set_Ptr);

		for(int V=0;V<Map_Values.get_values_number();V++)
		if( Map_Values.get_value_interpolation_type_according_to_ptr(V) != INTERPOLATION_TYPE_FLAG_NO_INTERPOLATION )
		{
			Progress_Form->add_text("Interpolating value:" + Map_Values.get_value_name_according_to_ptr(V) + "...");
			Progress_Form->Show();
			Application->ProcessMessages();

			int ss=0;
			Interpolate_Process_Flag = true;
			while(Interpolate_Process_Flag && ss < 150 )
			{
				ss++;
				Interpolate_Process_Flag = false;
				interpolate_one_value_one_step(V,&Interpolate_Process_Flag,Data_Point_Set_Ptr);
			}

//            ShowMessage("interpolation steps="+IntToStr(ss));

		} // res > 0

	} // "diffusion" interpolation

	} // if in range and data points number > 2
}
//---------------------------------------------------------------------------

int Surface_Class::restore_original_data_points_locations(int Data_Point_Set_Ptr)
{
	for(unsigned long j=0;j<Data_Point_Set[Data_Point_Set_Ptr].Data_Points.size();j++)
	{

	Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].Original_x = Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].x;
	Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].Original_y = Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].y;
	Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].Original_z = Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].z;

	}

	calculate_closest_nodes_and_data_points_ALL(Data_Point_Set_Ptr);

	return 1;
}
//---------------------------------------------------------------------------

int Surface_Class::force_data_points_to_surface(int Data_Point_Set_Ptr)
{
	long neig_triangle_ptr;
	double Alpha,Beta,Gamma;
	double Alpha_min,Beta_min,Gamma_min;
	double dist,Min;

	for(unsigned long j=0;j<Data_Point_Set[Data_Point_Set_Ptr].Data_Points.size();j++)
	{

	long n = Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].Closest_Node_Id;

	if( n >= 0 && (unsigned long) n < Surface_Node_Set.size() )
	{

	Min = 1000000;

	for(long nt=0;nt<Surface_Node_Set[n].Neig_Triangles.size();nt++)
	{

	neig_triangle_ptr = Surface_Node_Set[n].Neig_Triangles[nt];

	PNUM.get_barycentric_coordinates_within_triangle_of_projected_3D_point(
		Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].x,
		Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].y,
		Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].z,
		Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[0]].x,
		Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[0]].y,
		Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[0]].z,
		Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[1]].x,
		Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[1]].y,
		Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[1]].z,
		Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[2]].x,
		Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[2]].y,
		Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[2]].z,
		&Alpha,&Beta,&Gamma);

		dist = std::pow(Alpha-0.33,2) + std::pow(Beta-0.33,2) + std::pow(Gamma-0.33,2);

		if( dist < Min )
		{
			Min = dist;
			Alpha_min = Alpha;
			Beta_min = Beta;
			Gamma_min = Gamma;
		}

	} // through all neig triangles

		Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].x =
			Alpha_min*Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[0]].x +
			Beta_min *Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[1]].x +
			Gamma_min*Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[2]].x;
		Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].y =
			Alpha_min*Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[0]].y +
			Beta_min *Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[1]].y +
			Gamma_min*Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[2]].y;
		Data_Point_Set[Data_Point_Set_Ptr].Data_Points[j].z =
			Alpha_min*Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[0]].z +
			Beta_min *Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[1]].z +
			Gamma_min*Surface_Node_Set[Surface_Triangle_Set[neig_triangle_ptr].Nodes[2]].z;

	} // if closest node present

	}

	calculate_closest_nodes_and_data_points_ALL(Data_Point_Set_Ptr);

	return 1;
}

//-----------------------------------------------------------------------

void Surface_Class::cut_geometry_to_confine_to_radius(double Radius)
{
	double dist;

	for(long n=0;n<(signed)Surface_Node_Set.size();n++)
	{
		dist = std::sqrt(
			std::pow( Surface_Node_Set[n].x,2) +
			std::pow( Surface_Node_Set[n].y,2) +
			std::pow( Surface_Node_Set[n].z,2) );
		if( dist > Radius )
			Surface_Node_Set[n].Removed_Geometry_Vertex_Flag = true;
	}

	// set all triangles containing removed nodes as removed
	for(long i=0;i<(signed)Surface_Triangle_Set.size();i++)
	if( Surface_Node_Set[
			Surface_Triangle_Set[i].Nodes[0] ].Removed_Geometry_Vertex_Flag ||
		Surface_Node_Set[
			Surface_Triangle_Set[i].Nodes[1] ].Removed_Geometry_Vertex_Flag ||
		Surface_Node_Set[
			Surface_Triangle_Set[i].Nodes[2] ].Removed_Geometry_Vertex_Flag )
	Surface_Triangle_Set[i].Removed_Flag = ON;
}
//---------------------------------------------------------------------------

int Surface_Class::generate_wheel_sample(TProgress_Form *Progress_Form,int Radius, double Nodes_Spacing)
{
	Name = "Wheel Geometry";
	generate_square_sample(Progress_Form,2*Radius,2*Radius,Nodes_Spacing);
	cut_geometry_to_confine_to_radius(Radius);
}

//-----------------------------------------------------------------------

int Surface_Class::generate_square_sample(TProgress_Form *Progress_Form,
		int Square_Width,int Square_Height,double Nodes_Spacing)
{
	Progress_Form->add_text("Generating geometry...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Name = "Rectangular Geometry";

	Geometry_Vertex node_t;
	Geometry_Triangle triangle_t;
	double mx,my,mz;
	int Pointer[200][200]; // Attention! Max size of the sample is limited here, dont put high value here, runtime error shows up

	// Clear structure
	Surface_Triangle_Set.clear();
	Surface_Node_Set.clear();

	// 1. generate nodes
	long n = 0;
	for(long j=0;j<Square_Height;j++)
	for(long i=0;i<Square_Width;i++)
	{
		node_t.x = 1+i*Nodes_Spacing;
		node_t.y = 2;
		node_t.z = 1+j*Nodes_Spacing;
		node_t.Identifier = n;
		Pointer[i][j] = n;
		n++;
		Surface_Node_Set.push_back(node_t);
	}

	//-------------------------------------------
	// 2. generate triangles
	//-------------------------------------------
	for(long i=0;i<Square_Width-1;i++)
	for(long j=0;j<Square_Height-1;j++)
	{
		triangle_t.Nodes[0] = Pointer[i][j];
		triangle_t.Nodes[1] = Pointer[i+1][j];
		triangle_t.Nodes[2] = Pointer[i+1][j+1];
		triangle_t.Direction = -1;
		Surface_Triangle_Set.push_back(triangle_t);

		triangle_t.Nodes[0] = Pointer[i][j];
		triangle_t.Nodes[1] = Pointer[i+1][j+1];
		triangle_t.Nodes[2] = Pointer[i][j+1];
		triangle_t.Direction = 1;
		Surface_Triangle_Set.push_back(triangle_t);
	}

	// center surface (!, so xyz are different than original)
	mx = 0;
	my = 0;
	mz = 0;
	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	{
		mx += Surface_Node_Set[i].x/Surface_Node_Set.size();
		my += Surface_Node_Set[i].y/Surface_Node_Set.size();
		mz += Surface_Node_Set[i].z/Surface_Node_Set.size();
	}

	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	{
		Surface_Node_Set[i].x -= mx;
		Surface_Node_Set[i].y -= my;
		Surface_Node_Set[i].z -= mz;
	}

	//-------------------------------------------
	// Find neig nodes for each node
	//-------------------------------------------
    for(unsigned long i=0;i<Surface_Node_Set.size();i++)
		Surface_Node_Set[i].Neighbors.clear();

    for(long i=0;i<Square_Width;i++)
    for(long j=0;j<Square_Height;j++)
	{

	n = Pointer[i][j];

	// top row
	if( i-1>=0 && j+1<Square_Height ) Surface_Node_Set[n].Neighbors.push_back(Pointer[i-1][j+1]);
    if( j+1<Square_Height )     Surface_Node_Set[n].Neighbors.push_back(Pointer[i][j+1]);
    if( i+1<Square_Width && j+1<Square_Height ) Surface_Node_Set[n].Neighbors.push_back(Pointer[i+1][j+1]);


	// middle row
    if( i-1>=0 )     Surface_Node_Set[n].Neighbors.push_back(Pointer[i-1][j]);
    if( i+1<Square_Width )     Surface_Node_Set[n].Neighbors.push_back(Pointer[i+1][j]);


	// bottom row
	if( i-1>=0 && j-1>=0 )     Surface_Node_Set[n].Neighbors.push_back(Pointer[i-1][j-1]);
	if( j-1>=0 )     Surface_Node_Set[n].Neighbors.push_back(Pointer[i][j+1]);
    if( i+1<Square_Width && j-1>=0 )     Surface_Node_Set[n].Neighbors.push_back(Pointer[i+1][j-1]);

	}

	//-------------------------------------------
	// Calculate neig triangles for each node
	//-------------------------------------------
	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
		Surface_Node_Set[i].Neig_Triangles.clear();

	for(unsigned long i=0;i<Surface_Node_Set.size();i++)
	for(unsigned long j=0;j<Surface_Triangle_Set.size();j++)
	{
		if( Surface_Triangle_Set[j].Nodes[0] == (signed)i ||
			Surface_Triangle_Set[j].Nodes[1] == (signed)i ||
			Surface_Triangle_Set[j].Nodes[2] == (signed)i )
		{
			Surface_Node_Set[i].Neig_Triangles.push_back(j);
		}
	}

	Value_Description_Class VDC;
	VDC.Value_Name = SEGMENTATION_VALUE_NAME;
	VDC.Interpolation_Type = INTERPOLATION_TYPE_FLAG_NO_INTERPOLATION;
	VDC.Displayed_In_Table = false;
	Map_Values.add_value(VDC);
	Map_Values.set_current_value_according_to_name(SEGMENTATION_VALUE_NAME);

	initialize_surface(Progress_Form);

//	center_geometry_and_data_points();

	return OK_RESULT;
}

//---------------------------------------------------------------------------

void Surface_Class::compute_paths_for_4_point_LV_segmentation()
{
	vector <long> vv;

	//----------------------------------------------------------------------------
	if( Ap_SA_Intermediate >= 0 )
	{
	Ap_SA_Path = get_shortest_path_between_nodes(Apex_Node_Ptr,Ap_SA_Intermediate);
	vv = get_shortest_path_between_nodes(Ap_SA_Intermediate,Septal_Anterior_Node_Ptr);

	Ap_SA_Path.insert( Ap_SA_Path.end(), vv.begin(), vv.end() );
	}
	else
	Ap_SA_Path = get_shortest_path_between_nodes(Apex_Node_Ptr,Septal_Anterior_Node_Ptr);
	//----------------------------------------------------------------------------


	//----------------------------------------------------------------------------
	if( Ap_SL_Intermediate >= 0 )
	{
	Ap_SL_Path = get_shortest_path_between_nodes(Apex_Node_Ptr,Ap_SL_Intermediate);
	vv = get_shortest_path_between_nodes(Ap_SL_Intermediate,Septal_Lateral_Node_Ptr);

	Ap_SL_Path.insert( Ap_SL_Path.end(), vv.begin(), vv.end() );
	}
	else
	Ap_SL_Path = get_shortest_path_between_nodes(Apex_Node_Ptr,Septal_Lateral_Node_Ptr);
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	if( Ap_AL_Intermediate >= 0 )
	{
	Ap_AL_Path = get_shortest_path_between_nodes(Apex_Node_Ptr,Ap_AL_Intermediate);
	vv = get_shortest_path_between_nodes(Ap_AL_Intermediate,Anterior_Lateral_Node_Ptr);

	Ap_AL_Path.insert( Ap_AL_Path.end(), vv.begin(), vv.end() );
	}
	else
	Ap_AL_Path = get_shortest_path_between_nodes(Apex_Node_Ptr,Anterior_Lateral_Node_Ptr);
	//----------------------------------------------------------------------------

	// ***************
	// ***************
	// ***************

	//----------------------------------------------------------------------------
	if( Sept_Intermediate >= 0 )
	{
	Septal_Base_Path = get_shortest_path_between_nodes(Septal_Anterior_Node_Ptr,Sept_Intermediate);
	vv = get_shortest_path_between_nodes(Sept_Intermediate,Septal_Lateral_Node_Ptr);

	Septal_Base_Path.insert( Septal_Base_Path.end(), vv.begin(), vv.end() );
	}
	else
	Septal_Base_Path = get_shortest_path_between_nodes(Septal_Anterior_Node_Ptr,Septal_Lateral_Node_Ptr);


	//----------------------------------------------------------------------------
	if( Ant_Intermediate >= 0 )
	{
	Anterior_Base_Path = get_shortest_path_between_nodes(Septal_Anterior_Node_Ptr,Ant_Intermediate);
	vv = get_shortest_path_between_nodes(Ant_Intermediate,Anterior_Lateral_Node_Ptr);

	Anterior_Base_Path.insert( Anterior_Base_Path.end(), vv.begin(), vv.end() );
	}
	else
	Anterior_Base_Path = get_shortest_path_between_nodes(Septal_Anterior_Node_Ptr,Anterior_Lateral_Node_Ptr);


	//----------------------------------------------------------------------------
	if( Lat_Intermediate >= 0 )
	{
	Lateral_Base_Path = get_shortest_path_between_nodes(Septal_Lateral_Node_Ptr,Lat_Intermediate);
	vv = get_shortest_path_between_nodes(Ant_Intermediate,Anterior_Lateral_Node_Ptr);

	Lateral_Base_Path.insert( Lateral_Base_Path.end(), vv.begin(), vv.end() );
	}
	else
	Lateral_Base_Path = get_shortest_path_between_nodes(Septal_Lateral_Node_Ptr,Anterior_Lateral_Node_Ptr);

}

//---------------------------------------------------------------------------

vector <long> Surface_Class::get_shortest_path_between_nodes(long Node_1,long Node_2 )
{
	vector <long> Path;
	Path.clear();

	if( Node_1 >= 0 && Node_1 < (signed)Surface_Node_Set.size() )
	if( Node_2 >= 0 && Node_2 < (signed)Surface_Node_Set.size() )
	{

	// clear 'visited' flag
	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	Surface_Node_Set[n].Flag_A = 0;

	Path.push_back(Node_1);
	Surface_Node_Set[Node_1].Flag_A = 1;

	double Dest_x = Surface_Node_Set[Node_2].x;
	double Dest_y = Surface_Node_Set[Node_2].y;
	double Dest_z = Surface_Node_Set[Node_2].z;
	double dist,min_dist;
	long last_ptr,neig_ptr,min_dist_ptr;

	bool destination_reached = false;
	bool sth_done=true;

	while( !destination_reached && sth_done )
	{

		last_ptr = Path.size()-1;
		min_dist = 1000000;
		min_dist_ptr = -1;

		sth_done = false;

		for(unsigned long n=0;n<Surface_Node_Set[ Path[last_ptr] ].Neighbors.size();n++)
		if( Surface_Node_Set[Surface_Node_Set[ Path[last_ptr] ].Neighbors[n] ].Flag_A == 0 )
		{
			neig_ptr = Surface_Node_Set[ Path[last_ptr] ].Neighbors[n];

			dist = std::pow(Surface_Node_Set[neig_ptr].x-Dest_x,2)+
				   std::pow(Surface_Node_Set[neig_ptr].y-Dest_y,2)+
				   std::pow(Surface_Node_Set[neig_ptr].z-Dest_z,2);

			if( dist < min_dist )
			{
				min_dist = dist;
				min_dist_ptr = neig_ptr;
			}
		}

		if( min_dist_ptr >= 0 )
		{
			Path.push_back(min_dist_ptr);
			Surface_Node_Set[min_dist_ptr].Flag_A = 1;
			sth_done = true;
		}

		if( min_dist_ptr == Node_2 )
		destination_reached = true;

	}  // while destination not reached yet

	} // if nodes in range

	return Path;
}

//---------------------------------------------------------------------------

double Surface_Class::get_path_length(long Node_1,long Node_2)
{
	vector <long> Path;
	Path = get_shortest_path_between_nodes(Node_1,Node_2);

	double dist=0;

	for(unsigned long p=1;p<Path.size();p++)
	dist += std:: sqrt(
		std::pow( Surface_Node_Set[Path[p]].x - Surface_Node_Set[Path[p-1]].x ,2)+
		std::pow( Surface_Node_Set[Path[p]].y - Surface_Node_Set[Path[p-1]].y ,2)+
		std::pow( Surface_Node_Set[Path[p]].z - Surface_Node_Set[Path[p-1]].z ,2));

	return dist;
}

//---------------------------------------------------------------------------

void Surface_Class::initiate_propagation(int Region_Code,long Seed_Node_Ptr)
{
/*
	if( STUDY.is_current_surface_in_range() )
	if( STUDY.Surfaces_List[STUDY.Current_Surface].Pointed_Triangle >= 0 )
	{

	STUDY.Surfaces_List[STUDY.Current_Surface].initiate_propagation(
		Segments_Info.get_segment_id(LV_ANTERIOR_SEGMENT_NAME),
		STUDY.Surfaces_List[STUDY.Current_Surface].Surface_Triangle_Set[
			STUDY.Surfaces_List[STUDY.Current_Surface].Pointed_Triangle].Nodes[0]);

	repaint_3D_panels();
	}
*/
	if( Seed_Node_Ptr >= 0 &&
		Seed_Node_Ptr < Surface_Node_Set.size() )
	{
	//--------------------------------------------------------
	// transfer paths to nodes (so that boundaries are set)
	//--------------------------------------------------------
	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	Surface_Node_Set[n].Flag_A=0;

	for(unsigned long n=0;n<  Ap_SA_Path.size();n++)
	Surface_Node_Set[Ap_SA_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<  Ap_SL_Path.size();n++)
	Surface_Node_Set[Ap_SL_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<  Ap_AL_Path.size();n++)
	Surface_Node_Set[Ap_AL_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<Anterior_Base_Path.size();n++)
	Surface_Node_Set[Anterior_Base_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<Septal_Base_Path.size();n++)
	Surface_Node_Set[Septal_Base_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<Lateral_Base_Path.size();n++)
	Surface_Node_Set[Lateral_Base_Path[n]].Flag_A=1;

	List.clear();

	List.push_back(Seed_Node_Ptr);

	Surface_Node_Set[Seed_Node_Ptr].Flag_A=1;
	Surface_Node_Set[Seed_Node_Ptr].GroupId=Region_Code;
	}

}
//---------------------------------------------------------------------------
void Surface_Class::propagate_region_one_step(int Region_Code)
{
/*
	if( STUDY.is_current_surface_in_range() )
	if( STUDY.Surfaces_List[STUDY.Current_Surface].Pointed_Triangle >= 0 )
	{

	STUDY.Surfaces_List[STUDY.Current_Surface].propagate_region_one_step(
		Segments_Info.get_segment_id(LV_ANTERIOR_SEGMENT_NAME) );

	repaint_3D_panels();
	}
*/

	//--------------------------------------------------------
	// main propagation
	//--------------------------------------------------------
		for(long l=List.size()-1;l>=0;l--)
		{

		for(unsigned long n=0;n<Surface_Node_Set[ List[l] ].Neighbors.size();n++)
		if( Surface_Node_Set[Surface_Node_Set[ List[l] ].Neighbors[n] ].Flag_A == 0 )
		{
			List.push_back( Surface_Node_Set[ List[l] ].Neighbors[n] );
			Surface_Node_Set[Surface_Node_Set[ List[l] ].Neighbors[n] ].Flag_A = 1;
			Surface_Node_Set[Surface_Node_Set[ List[l] ].Neighbors[n] ].GroupId = Region_Code;
		}
		}

	//--------------------------------------------------------
	// transfer regions to triangles segment_id variable
	//--------------------------------------------------------
	for(unsigned long t=0;t<Surface_Triangle_Set.size();t++)
	if( Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].GroupId == Region_Code ||
		Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].GroupId == Region_Code ||
		Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].GroupId == Region_Code )
	Surface_Triangle_Set[t].Segment_Id = Region_Code;

}

//---------------------------------------------------------------------------
int Surface_Class::propagate_region(int Region_Code,long Seed_Node_Ptr)
{
	if( Seed_Node_Ptr >= 0 &&
		Seed_Node_Ptr < Surface_Node_Set.size() )
	{
	//--------------------------------------------------------
	// transfer paths to nodes (so that boundaries are set)
	//--------------------------------------------------------
	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	Surface_Node_Set[n].Flag_A=0;

	for(unsigned long n=0;n<  Ap_SA_Path.size();n++)
	Surface_Node_Set[Ap_SA_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<  Ap_SL_Path.size();n++)
	Surface_Node_Set[Ap_SL_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<  Ap_AL_Path.size();n++)
	Surface_Node_Set[Ap_AL_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<Anterior_Base_Path.size();n++)
	Surface_Node_Set[Anterior_Base_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<Septal_Base_Path.size();n++)
	Surface_Node_Set[Septal_Base_Path[n]].Flag_A=1;

	for(unsigned long n=0;n<Lateral_Base_Path.size();n++)
	Surface_Node_Set[Lateral_Base_Path[n]].Flag_A=1;

	//--------------------------------------------------------
	// main propagation
	//--------------------------------------------------------
	bool sth_done=true;

	vector <long> List;
	List.push_back(Seed_Node_Ptr);

	Surface_Node_Set[Seed_Node_Ptr].Flag_A=1;
	Surface_Node_Set[Seed_Node_Ptr].GroupId=Region_Code;

	while(sth_done)
	{
		sth_done=false;

		for(long l=List.size()-1;l>=0;l--)
		{

		for(unsigned long n=0;n<Surface_Node_Set[ List[l] ].Neighbors.size();n++)
		if( Surface_Node_Set[Surface_Node_Set[ List[l] ].Neighbors[n] ].Flag_A == 0 )
		{
			List.push_back( Surface_Node_Set[ List[l] ].Neighbors[n] );
			Surface_Node_Set[Surface_Node_Set[ List[l] ].Neighbors[n] ].Flag_A = 1;
			Surface_Node_Set[Surface_Node_Set[ List[l] ].Neighbors[n] ].GroupId = Region_Code;
			sth_done=true;
		}
		}
	}
	//--------------------------------------------------------
	// transfer regions to triangles segment_id variable
	//--------------------------------------------------------
	for(unsigned long t=0;t<Surface_Triangle_Set.size();t++)
	if( Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].GroupId == Region_Code ||
		Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].GroupId == Region_Code ||
		Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].GroupId == Region_Code )
	Surface_Triangle_Set[t].Segment_Id = Region_Code;

	return 1;
	}
}

//---------------------------------------------------------------------------

double Surface_Class::get_distance_to_path(long Node_Id,vector <long> *Path)
{
	double dist,min_dist=1000000;

    for(unsigned long p=0;p<Path[0].size();p++)
	{

	dist = std:: sqrt(
		std::pow( Surface_Node_Set[Path[0][p]].x - Surface_Node_Set[Node_Id].x ,2)+
		std::pow( Surface_Node_Set[Path[0][p]].y - Surface_Node_Set[Node_Id].y ,2)+
		std::pow( Surface_Node_Set[Path[0][p]].z - Surface_Node_Set[Node_Id].z ,2));

	if( dist < min_dist ) min_dist = dist;

	}

	return min_dist;
}

//---------------------------------------------------------------------------

void Surface_Class::get_xyz_centre_of_triangle(long Triangle_Id,double* x,double* y,double* z)
{
	long n1 = Surface_Triangle_Set[Triangle_Id].Nodes[0];
	long n2 = Surface_Triangle_Set[Triangle_Id].Nodes[1];
	long n3 = Surface_Triangle_Set[Triangle_Id].Nodes[2];

	x[0] = (Surface_Node_Set[n1].x+Surface_Node_Set[n2].x+Surface_Node_Set[n3].x)/3.0;
	y[0] = (Surface_Node_Set[n1].y+Surface_Node_Set[n2].y+Surface_Node_Set[n3].y)/3.0;
	z[0] = (Surface_Node_Set[n1].z+Surface_Node_Set[n2].z+Surface_Node_Set[n3].z)/3.0;
}

//---------------------------------------------------------------------------

void Surface_Class::downsample_mesh(AnsiString AppPath, double Ratio,TProgress_Form *Progress_Form)
{
	AnsiString CurrentDir = GetCurrentDir();

	// update progress form
	Progress_Form->add_text("Max geo nodes number exceeded, simplifying mesh...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//------------------------------
	// 1. save geo to obj file
	//------------------------------
	ofstream df( (CurrentDir+"\\simplify_mesh_tmp.txt").c_str());

	if( !df.is_open() )
	ShowMessage("cant create mesh geo file (surface.cpp)");

	long Old_N_Number = Surface_Node_Set.size();

	for(long n=0;n<(signed)Surface_Node_Set.size();n++)
	{
		df <<"v ";
		df << Surface_Node_Set[n].x << " ";
		df << Surface_Node_Set[n].y << " ";
		df << Surface_Node_Set[n].z << " ";
		df << endl;
	}

	for(long n=0;n<(signed)Surface_Triangle_Set.size();n++)
	{
		df <<"f ";
		df << Surface_Triangle_Set[n].Nodes[0]+1 << " ";
		df << Surface_Triangle_Set[n].Nodes[1]+1 << " ";
		df << Surface_Triangle_Set[n].Nodes[2]+1 << " ";
		df << endl;
	}
	df.close();

	//------------------------------
	// 2. run simplify .exe
	//------------------------------
	AnsiString quote = "\"";
	AnsiString Command = "cmd /C " + quote + " " + quote + AppPath + "simplify_mesh.exe" + " " + quote + " " + quote + CurrentDir +"\\simplify_mesh_tmp.txt" + quote + " " + quote + CurrentDir +"\\simplify_mesh_result_tmp.txt"+ quote + " " + FloatToStr(Ratio) + quote;
/*
ShowMessage(Command);
ofstream dd("command.txt");
dd << Command.c_str();
dd.close();
*/
	system( (Command).c_str() );

	//------------------------------
	// 3. load mesh
	//------------------------------
	FILE* fn;
	if ((fn = fopen((CurrentDir+"\\simplify_mesh_result_tmp.txt").c_str(), "rb")) == NULL)
	{
		ShowMessage( "Output file mesh decimation not found! (surface.cpp)");
	}
	else
	{

	Surface_Node_Set.clear();
	Surface_Triangle_Set.clear();
	Geometry_Vertex N;
	Geometry_Triangle T;
	T.Segment_Id = -1;
	char line[1000];
	memset ( line,0,1000 );
	long n1,n2,n3;

	while(fgets( line, 1000, fn ) != NULL)
	{
			if ( line[0] == 'v' )
			{
				if ( line[1] == ' ' )
				if(sscanf(line,"v %lf %lf %lf",
					&N.x,&N.y,&N.z)==3)
				{
					Surface_Node_Set.push_back(N);
				}
			}

			if ( line[0] == 'f' )
			{
				if(sscanf(line,"f %d %d %d",&n1,&n2,&n3)==3)
				{
					T.Nodes[0] = n1-1;
					T.Nodes[1] = n2-1;
					T.Nodes[2] = n3-1;
					Surface_Triangle_Set.push_back(T);
				}
			}
	}
	fclose(fn);

	long New_N_Number = Surface_Node_Set.size();

	for(long i=0;i<(signed)Surface_Node_Set.size();i++)
	{
		Surface_Node_Set[i].Original_x = Surface_Node_Set[i].x;
		Surface_Node_Set[i].Original_y = Surface_Node_Set[i].y;
		Surface_Node_Set[i].Original_z = Surface_Node_Set[i].z;
	}

	// update progress form
	Progress_Form->add_text("Geometry nodes number decreased from " + FloatToStr(Old_N_Number) + " to " + FloatToStr(New_N_Number) + "...");
	Progress_Form->Show();
	Application->ProcessMessages();

	// Update neigs and triangles neigs
	find_neig_nodes_and_neig_triangles();
	smooth_map_geometry(); // !!!!!!!!!!!!!!!!!!!!!
	calculate_normals();

	}

/*           this version of code uses app directory for temp mesh files

//	AnsiString CurrentDir = GetCurrentDir();
//	SetCurrentDir(AppPath);

	// update progress form
	Progress_Form->add_text("Max geo nodes number exceeded, simplifying mesh...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//------------------------------
	// 1. save geo to obj file
	//------------------------------
	ofstream df( (AppPath+"simplify_mesh_tmp.txt").c_str());
//	ofstream df( (CurrentDir+"\\simplify_mesh_tmp.txt").c_str());

	if( !df.is_open() )
	ShowMessage("cant create mesh geo file (surface.cpp)");

	long Old_N_Number = Surface_Node_Set.size();

	for(long n=0;n<(signed)Surface_Node_Set.size();n++)
	{
		df <<"v ";
		df << Surface_Node_Set[n].x << " ";
		df << Surface_Node_Set[n].y << " ";
		df << Surface_Node_Set[n].z << " ";
		df << endl;
	}

	for(long n=0;n<(signed)Surface_Triangle_Set.size();n++)
	{
		df <<"f ";
		df << Surface_Triangle_Set[n].Nodes[0]+1 << " ";
		df << Surface_Triangle_Set[n].Nodes[1]+1 << " ";
		df << Surface_Triangle_Set[n].Nodes[2]+1 << " ";
		df << endl;
	}
	df.close();

	//------------------------------
	// 2. run simplify .exe
	//------------------------------
	AnsiString quote = "\"";
//	AnsiString Command = "cmd /C " + quote + quote + AppPath + "simplify_mesh.exe" + quote + " " + quote + CurrentDir+"\\simplify_mesh_tmp.txt" + quote + " " + quote + CurrentDir+"\\simplify_mesh_result_tmp.txt"+ quote + " " + FloatToStr(Ratio) + " > " + quote + CurrentDir+"\\simplify.out.txt" + quote + quote;
	AnsiString Command = "cmd /C " + quote + quote + AppPath + "simplify_mesh.exe" + " " + quote + AppPath +"\\simplify_mesh_tmp.txt" + quote + " " + quote + AppPath +"\\simplify_mesh_result_tmp.txt"+ quote + " " + FloatToStr(Ratio) + quote;

	system( (Command).c_str() );

	//------------------------------
	// 3. load mesh
	//------------------------------
	FILE* fn;
	if ((fn = fopen((AppPath+"simplify_mesh_result_tmp.txt").c_str(), "rb")) == NULL)
//	if ((fn = fopen((CurrentDir+"\\simplify_mesh_result_tmp.txt").c_str(), "rb")) == NULL)
	{
		ShowMessage( "Output file mesh decimation not found! (surface.cpp)");
	}
	else
	{

	Surface_Node_Set.clear();
	Surface_Triangle_Set.clear();
	Geometry_Vertex N;
	Geometry_Triangle T;
	T.Segment_Id = -1;
	char line[1000];
	memset ( line,0,1000 );
	long n1,n2,n3;

	while(fgets( line, 1000, fn ) != NULL)
	{
			if ( line[0] == 'v' )
			{
				if ( line[1] == ' ' )
				if(sscanf(line,"v %lf %lf %lf",
					&N.x,&N.y,&N.z)==3)
				{
					Surface_Node_Set.push_back(N);
				}
			}

			if ( line[0] == 'f' )
			{
				if(sscanf(line,"f %d %d %d",&n1,&n2,&n3)==3)
				{
					T.Nodes[0] = n1-1;
					T.Nodes[1] = n2-1;
					T.Nodes[2] = n3-1;
					Surface_Triangle_Set.push_back(T);
				}
			}
	}
	fclose(fn);

	long New_N_Number = Surface_Node_Set.size();

	for(long i=0;i<(signed)Surface_Node_Set.size();i++)
	{
		Surface_Node_Set[i].Original_x = Surface_Node_Set[i].x;
		Surface_Node_Set[i].Original_y = Surface_Node_Set[i].y;
		Surface_Node_Set[i].Original_z = Surface_Node_Set[i].z;
	}

	// update progress form
	Progress_Form->add_text("Geometry nodes number decreased from " + FloatToStr(Old_N_Number) + " to " + FloatToStr(New_N_Number) + "...");
	Progress_Form->Show();
	Application->ProcessMessages();

	// Update neigs and triangles neigs
	find_neig_nodes_and_neig_triangles();
	smooth_map_geometry(); // !!!!!!!!!!!!!!!!!!!!!
	calculate_normals();

//	SetCurrentDir(CurrentDir);

	}
*/

}

//---------------------------------------------------------------------------

double Surface_Class::get_volume()
{
	double rx0,ry0,rz0,rx1,ry1,rz1,rx2,ry2,rz2;
	double mx,my,mz, dv;

	double Volume = 0.0;

    for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	{

	rx0 = Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[0] ].x;
	ry0 = Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[0] ].y;
	rz0 = Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[0] ].z;

	rx1 = Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[1] ].x;
	ry1 = Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[1] ].y;
	rz1 = Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[1] ].z;

	rx2 = Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[2] ].x;
	ry2 = Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[2] ].y;
	rz2 = Surface_Node_Set[ Surface_Triangle_Set[i].Nodes[2] ].z;

	Surface_Triangle_Set[i].tmp_vec[0] = (rz2-rz0)*(ry1-ry0)-(rz1-rz0)*(ry2-ry0);
	Surface_Triangle_Set[i].tmp_vec[1] = (rx2-rx0)*(rz1-rz0)-(rx1-rx0)*(rz2-rz0);
	Surface_Triangle_Set[i].tmp_vec[2] = (ry2-ry0)*(rx1-rx0)-(ry1-ry0)*(rx2-rx0);

	// /2.0 because triangle area is calculated, not romb
	Surface_Triangle_Set[i].tmp_vec[0] = Surface_Triangle_Set[i].tmp_vec[0] / 2.0;
	Surface_Triangle_Set[i].tmp_vec[1] = Surface_Triangle_Set[i].tmp_vec[1] / 2.0;
	Surface_Triangle_Set[i].tmp_vec[2] = Surface_Triangle_Set[i].tmp_vec[2] / 2.0;

	mx = (rx0 + rx1 + rx2)/3.0;
	my = (ry0 + ry1 + ry2)/3.0;
	mz = (rz0 + rz1 + rz2)/3.0;

//    dv = Surface_Triangle_Set[i].Direction*
	dv = 1*
						(mx*Surface_Triangle_Set[i].tmp_vec[0] +
						 my*Surface_Triangle_Set[i].tmp_vec[1] +
						 mz*Surface_Triangle_Set[i].tmp_vec[2] )/3.0;

	Volume = Volume + dv;

	}

	// thats all, move out

	Volume = Volume / 1000.0; // to be in cm3

	return Volume;
}

//--------------------------------------------------------------------

double Surface_Class::get_area_of_given_segment_cm2(int Segment_Id)
{
	double Area = 0.0;

	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	if( Surface_Triangle_Set[i].Segment_Id == Segment_Id)
	Area += Surface_Triangle_Set[i].Area_cm2;

	return Area;
}

//---------------------------------------------------------------------------
void Surface_Class::rescale_geometry(double Scale)
{
	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	{
		Surface_Node_Set[n].x *= Scale;
		Surface_Node_Set[n].y *= Scale;
		Surface_Node_Set[n].z *= Scale;
	}
}

//---------------------------------------------------------------------------

void Surface_Class::rescale_geometry_to_reach_specified_volume(double Target_Volume)
{
	double Rescaled_Volume;

	// remember original xyz
	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	{
		Surface_Node_Set[n].tmpx = Surface_Node_Set[n].x;
		Surface_Node_Set[n].tmpy = Surface_Node_Set[n].y;
		Surface_Node_Set[n].tmpz = Surface_Node_Set[n].z;
	}

	// find equation: Volume = A*scaling_factor + B
	// we know that Target_Volume = A*target_scaling_factor + B
	// and need two equations:
	// V1 = A*0.95+B
	// V2 = A*1.05+B
	// A = (V2-V1)/(1.05-0.95)
	// B = V1 - A*95

	double V1,V2,A,B;

	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	{
		Surface_Node_Set[n].x *= 0.95;
		Surface_Node_Set[n].y *= 0.95;
		Surface_Node_Set[n].z *= 0.95;
	}
	V1 = get_volume();

	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
		{
			Surface_Node_Set[n].x = Surface_Node_Set[n].tmpx;
			Surface_Node_Set[n].y = Surface_Node_Set[n].tmpy;
			Surface_Node_Set[n].z = Surface_Node_Set[n].tmpz;
		}

	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	{
		Surface_Node_Set[n].x *= 1.05;
		Surface_Node_Set[n].y *= 1.05;
		Surface_Node_Set[n].z *= 1.05;
	}
	V2 = get_volume();

	A = (V2-V1)/0.1;
	B = V1 - A*0.95;
	if( A==0 ) A=1;
	double Target_scaling_factor = (Target_Volume - B)/A;

	// perform final sclaing
	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
		{
			Surface_Node_Set[n].x = Surface_Node_Set[n].tmpx;
			Surface_Node_Set[n].y = Surface_Node_Set[n].tmpy;
			Surface_Node_Set[n].z = Surface_Node_Set[n].tmpz;
		}

	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	{
		Surface_Node_Set[n].x *= Target_scaling_factor;
		Surface_Node_Set[n].y *= Target_scaling_factor;
		Surface_Node_Set[n].z *= Target_scaling_factor;
	}

/*

	double Best_Fit=-1,Min_Volume_Diff=100000000;
	for(double Scaling_Factor=0.995;Scaling_Factor<1.005;Scaling_Factor+=0.0001)
	{
		for(unsigned long n=0;n<Surface_Node_Set.size();n++)
		{
			Surface_Node_Set[n].x *= Scaling_Factor;
			Surface_Node_Set[n].y *= Scaling_Factor;
			Surface_Node_Set[n].z *= Scaling_Factor;
		}

		Rescaled_Volume = get_volume();

		if( fabs(Rescaled_Volume - Target_Volume) < Min_Volume_Diff )
		{
			Min_Volume_Diff = fabs(Rescaled_Volume - Target_Volume);
			Best_Fit = Scaling_Factor;
		}

		// restore original coordinates
		for(unsigned long n=0;n<Surface_Node_Set.size();n++)
		{
			Surface_Node_Set[n].x = Surface_Node_Set[n].tmpx;
			Surface_Node_Set[n].y = Surface_Node_Set[n].tmpy;
			Surface_Node_Set[n].z = Surface_Node_Set[n].tmpz;
		}
	}

	// do final rescaling
	for(unsigned long n=0;n<Surface_Node_Set.size();n++)
	{
		Surface_Node_Set[n].x *= Best_Fit;
		Surface_Node_Set[n].y *= Best_Fit;
		Surface_Node_Set[n].z *= Best_Fit;
	}

*/
}

//---------------------------------------------------------------------------

bool Surface_Class::segmentation_expand_segment_one_step(int Segment_Id,bool Block_Existing)
{
	bool sth_done = false;
	long Node_ptr;

	if( Segment_Id >= 0 )
	{

	vector <long> Triangles_To_Paint;

//	save_segmentation_state();

	for(long i=0;i<(signed)Surface_Triangle_Set.size();i++)
	if( Surface_Triangle_Set[i].Segment_Id <= 0 ||
		Surface_Triangle_Set[i].Segment_Id > 0 && !Block_Existing  )
	{

	// vertices of triange i
	for(int tn=0;tn<3;tn++)
	{

	Node_ptr = Surface_Triangle_Set[i].Nodes[tn];

	// for triangles around node tn
	for(int neig_t=0;neig_t<(signed)Surface_Node_Set[Node_ptr].Neig_Triangles.size();neig_t++)
	{
		long N2 = Surface_Node_Set[Node_ptr].Neig_Triangles[neig_t];

		if(Surface_Triangle_Set[N2].Segment_Id == Segment_Id )
		{
			Triangles_To_Paint.push_back(i);
			sth_done = true;
		}
	}
	}
	}

	for(long t=0;t<(signed)Triangles_To_Paint.size();t++)
		Surface_Triangle_Set[Triangles_To_Paint[t]].Segment_Id = Segment_Id;
	}

	return sth_done;
}

//---------------------------------------------------------------------------
void Surface_Class::segmentation_shrink_segment_one_step(int Segment_Id,bool Block_Existing)
{
	if( Segment_Id >= 0 )
	{

	bool Border_Triangle;
	vector <long> Triangles_To_Clear;

//	save_segmentation_state();

	for(long i=0;i<(signed)Surface_Triangle_Set.size();i++)
	if(Surface_Triangle_Set[i].Segment_Id == Segment_Id )
	{

	// check if has not-segmented neighbor
	Border_Triangle = false;

	for(int tn=0;tn<3;tn++)
	{

	long Node_ptr = Surface_Triangle_Set[i].Nodes[tn];

	// for triangles around node tn
	for(int neig_t=0;neig_t<(signed)Surface_Node_Set[Node_ptr].Neig_Triangles.size();neig_t++)
	{
		long N2 = Surface_Node_Set[Node_ptr].Neig_Triangles[neig_t];

		if(Surface_Triangle_Set[N2].Segment_Id != Segment_Id )
		Border_Triangle = true;
	}

	}

	if( Border_Triangle )
	Triangles_To_Clear.push_back(i);

	}

	for(long t=0;t<(signed)Triangles_To_Clear.size();t++)
		Surface_Triangle_Set[Triangles_To_Clear[t]].Segment_Id = -1;

	}
}

//---------------------------------------------------------------------------

double Surface_Class::get_total_area()
{
	double Total_Area=0;

	for(unsigned long  i=0; i < Surface_Triangle_Set.size(); i++)
	if( Surface_Triangle_Set[i].Removed_Flag != ON )
		Total_Area += Surface_Triangle_Set[i].Area_cm2;

	return Total_Area;
}

//---------------------------------------------------------------------------

bool Surface_Class::is_segment_present_at_data_point(int Dset,int DP,int Segment_Id)
{
	long NPtr = Data_Point_Set[Dset].Data_Points[DP].Closest_Node_Id;
	long T_Ptr;
	bool Segment_Present = false;

	// check if neig triangle has segment_id
	if( NPtr >= 0 && NPtr < Surface_Node_Set.size() )
	for(int neig=0;neig<Surface_Node_Set[NPtr].Neig_Triangles.size();neig++)
	{
		T_Ptr = Surface_Node_Set[NPtr].Neig_Triangles[neig];

		if( T_Ptr >= 0 && T_Ptr < Surface_Triangle_Set.size() )
		if(Surface_Triangle_Set[T_Ptr].Segment_Id == Segment_Id )
		Segment_Present = true;
	}

	return Segment_Present;
}

//---------------------------------------------------------------------------

int Surface_Class::get_data_point_segment_id(int Dset,int DP)
{
	long NPtr = Data_Point_Set[Dset].Data_Points[DP].Closest_Node_Id;
	long TPtr;

	if( NPtr >= 0 && NPtr < Surface_Node_Set.size() )
	{
		TPtr = Surface_Node_Set[ NPtr ].Neig_Triangles[0];

		if( TPtr >= 0 )
		return Surface_Triangle_Set[TPtr].Segment_Id;
	}

	return -1;
}

//---------------------------------------------------------------------------
long Surface_Class::get_number_of_data_points_in_segment(int Dset,int Segment_Id,
		AnsiString Value_Name,vector <Value_Description_Class> *Values_List)
{
	long Number=0;
	long NPtr,TPtr;

	for(unsigned long d=0;d<Data_Point_Set[Dset].Data_Points.size();d++)
	if( Data_Point_Set[Dset].Data_Points[d].get_value(Value_Name, Values_List )
		!= NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{

	NPtr = Data_Point_Set[Dset].Data_Points[d].Closest_Node_Id;

	if( NPtr >= 0 )
	{
		TPtr = Surface_Node_Set[ NPtr ].Neig_Triangles[0];

		if( TPtr >= 0 )
		if( Surface_Triangle_Set[TPtr].Segment_Id == Segment_Id )
		Number++;
	}
	}

	return Number;
}

//---------------------------------------------------------------------------

void Surface_Class::make_geometrical_transformation(
			double X,double Y, double Z,
			double Teta, double Fi,  double aX, double aY, double aZ, int Seg_No)
{
	//-------------------------------------------------
	// 1st Translation
	//-------------------------------------------------
	for(long i=0; i < (signed)Surface_Node_Set.size(); i++ )
	if( Seg_No==0 || is_node_part_of_segment( i,Seg_No)==ON )
	{
		Surface_Node_Set[i].x += X;
		Surface_Node_Set[i].y += Y;
		Surface_Node_Set[i].z += Z;
	}

	for(long dset=0;dset<Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)Data_Point_Set[dset].Data_Points.size();i++)
	{
		Data_Point_Set[dset].Data_Points[i].x += X;
		Data_Point_Set[dset].Data_Points[i].y += Y;
		Data_Point_Set[dset].Data_Points[i].z += Z;
	}

	for(long i=0;i<(signed)Ablation_Points_List.size();i++)
	{
		Ablation_Points_List[i].x += X;
		Ablation_Points_List[i].y += Y;
		Ablation_Points_List[i].z += Z;
	}

	for(long i=0;i<(signed)Labels.size();i++)
	{
		Labels[i].x += X;
		Labels[i].y += Y;
		Labels[i].z += Z;
	}

	translate_ABC_spatial_registration_markers(-X,-Y,-Z);

	//-------------------------------------------------
	// Scaling
	//-------------------------------------------------
	for(long i=0; i < (signed)Surface_Node_Set.size(); i++ )
	if( Seg_No==0 || is_node_part_of_segment( i,Seg_No)==ON )
	{
		Surface_Node_Set[i].x *= aX;
		Surface_Node_Set[i].y *= aY;
		Surface_Node_Set[i].z *= aZ;
	}

	for(long dset=0;dset<Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)Data_Point_Set[dset].Data_Points.size();i++)
	{
		Data_Point_Set[dset].Data_Points[i].x *= aX;
		Data_Point_Set[dset].Data_Points[i].y *= aY;
		Data_Point_Set[dset].Data_Points[i].z *= aZ;
	}

	for(long i=0;i<(signed)Ablation_Points_List.size();i++)
	{
		Ablation_Points_List[i].x *= aX;
		Ablation_Points_List[i].y *= aY;
		Ablation_Points_List[i].z *= aZ;
	}

	for(long i=0;i<(signed)Labels.size();i++)
	{
		Labels[i].x *= aX;
		Labels[i].y *= aY;
		Labels[i].z *= aZ;
	}

	scale_ABC_spatial_registration_markers(aX,aY,aZ);

//     DODAJ REG POINTS, abl, labels DO SCALINGU I ROTACIJ

	//-------------------------------------------------
	// Rotation
	//-------------------------------------------------
	double Teta_P,R,CosFi,SinFi;

	if( Teta != 0 )
	for(long i=0; i < (signed)Surface_Node_Set.size(); i++ )
	if( Seg_No==0 || is_node_part_of_segment( i,Seg_No)==ON )
	{

	  Teta_P = atan2( sqrt(pow(Surface_Node_Set[i].x,2)+
						 pow(Surface_Node_Set[i].y,2) ),Surface_Node_Set[i].z );

	  R = sqrt( pow(Surface_Node_Set[i].x,2)+pow(Surface_Node_Set[i].y,2)+
				pow(Surface_Node_Set[i].z,2) );

	  if( pow(Surface_Node_Set[i].x,2)+ pow(Surface_Node_Set[i].y,2) != 0 )
	  CosFi = Surface_Node_Set[i].x / sqrt(pow(Surface_Node_Set[i].x,2)+
						 pow(Surface_Node_Set[i].y,2) );
	  else
	  CosFi = 0.0;

	  if( pow(Surface_Node_Set[i].x,2)+ pow(Surface_Node_Set[i].y,2) != 0 )
	  SinFi = Surface_Node_Set[i].y / sqrt(pow(Surface_Node_Set[i].x,2)+
						 pow(Surface_Node_Set[i].y,2) );
	  else
	  SinFi = 1.0;


	  // new coordinates
	  Surface_Node_Set[i].x = R*CosFi*sin(Teta_P+Teta);
	  Surface_Node_Set[i].y = R*SinFi*sin(Teta_P+Teta);
	  Surface_Node_Set[i].z = R*cos(Teta_P+Teta);

	}

	for(long dset=0;dset<Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)Data_Point_Set[dset].Data_Points.size();i++)
	{

	  Teta_P = atan2( sqrt(pow(Data_Point_Set[dset].Data_Points[i].x,2)+
						   pow(Data_Point_Set[dset].Data_Points[i].y,2) ),
							 Data_Point_Set[dset].Data_Points[i].z );

	  R = sqrt( pow(Data_Point_Set[dset].Data_Points[i].x,2)+
				pow(Data_Point_Set[dset].Data_Points[i].y,2)+
				pow(Data_Point_Set[dset].Data_Points[i].z,2) );

	  if( pow(Data_Point_Set[dset].Data_Points[i].x,2)+
		  pow(Data_Point_Set[dset].Data_Points[i].y,2) != 0 )
	  CosFi = Data_Point_Set[dset].Data_Points[i].x /
		sqrt(pow(Data_Point_Set[dset].Data_Points[i].x,2)+
						 pow(Data_Point_Set[dset].Data_Points[i].y,2) );
	  else
	  CosFi = 0.0;

	  if( pow(Data_Point_Set[dset].Data_Points[i].x,2)+
		  pow(Data_Point_Set[dset].Data_Points[i].y,2) != 0 )
	  SinFi = Data_Point_Set[dset].Data_Points[i].y /
				sqrt(pow(Data_Point_Set[dset].Data_Points[i].x,2)+
						 pow(Data_Point_Set[dset].Data_Points[i].y,2) );
	  else
	  SinFi = 1.0;

		Data_Point_Set[dset].Data_Points[i].x = R*CosFi*sin(Teta_P+Teta);
		Data_Point_Set[dset].Data_Points[i].y = R*SinFi*sin(Teta_P+Teta);
		Data_Point_Set[dset].Data_Points[i].z = R*cos(Teta_P+Teta);
	}

}


//--------------------------------------------------------------------------

int Surface_Class::is_node_part_of_segment( int Node_Ptr, int Seg_No)
{
	int Flag = 0;

	for(unsigned long k=0;k<Surface_Node_Set[Node_Ptr].Neig_Triangles.size();k++)
	if( Surface_Triangle_Set[ Surface_Node_Set[Node_Ptr].Neig_Triangles[k] ].Segment_Id == Seg_No )
	Flag = 1;

	if( Flag == 0 )
	return OFF;
	else
	return ON;

}

//--------------------------------------------------------------------------

void Surface_Class::restore_original_xyz_coordinates_from_import()
{
// qt
	if( Surface_Node_Set.size() > 0 )
	{

	for(long i=0;i<(signed)Surface_Node_Set.size();i++)
	{
		Surface_Node_Set[i].x = Surface_Node_Set[i].Original_x;
		Surface_Node_Set[i].y = Surface_Node_Set[i].Original_y;
		Surface_Node_Set[i].z = Surface_Node_Set[i].Original_z;
	}

	for(long dset=0;dset<Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)Data_Point_Set[dset].Data_Points.size();i++)
	{
		Data_Point_Set[dset].Data_Points[i].x = Data_Point_Set[dset].Data_Points[i].Original_x;
		Data_Point_Set[dset].Data_Points[i].y = Data_Point_Set[dset].Data_Points[i].Original_y;
		Data_Point_Set[dset].Data_Points[i].z = Data_Point_Set[dset].Data_Points[i].Original_z;
	}

	Translation_Vector_X = 0;
	Translation_Vector_Y = 0;
	Translation_Vector_Z = 0;

	}
}

//--------------------------------------------------------------------------

void Surface_Class::center_geometry_and_data_points()
{
	if( Surface_Node_Set.size() > 0 )
	{

	double mx=0,my=0,mz=0;
	long cnt=0;

	for(long dset=0;dset<Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)Data_Point_Set[dset].Data_Points.size();i++)
	{
		mx += Data_Point_Set[dset].Data_Points[i].x;
		my += Data_Point_Set[dset].Data_Points[i].y;
		mz += Data_Point_Set[dset].Data_Points[i].z;
		cnt++;
	}

	for(long i=0;i<(signed)Surface_Node_Set.size();i++)
	{
		mx += Surface_Node_Set[i].x;
		my += Surface_Node_Set[i].y;
		mz += Surface_Node_Set[i].z;
		cnt++;
	}

	if(cnt!=0)
	{
		mx /= cnt;
		my /= cnt;
		mz /= cnt;
	}

	Translation_Vector_X -= mx;
	Translation_Vector_Y -= my;
	Translation_Vector_Z -= mz;

	//--------------------------------------------------------------------
	// now do translation
	//--------------------------------------------------------------------

	for(long i=0;i<(signed)Surface_Node_Set.size();i++)
	{
		Surface_Node_Set[i].x -= mx;
		Surface_Node_Set[i].y -= my;
		Surface_Node_Set[i].z -= mz;
	}

	for(long dset=0;dset<Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)Data_Point_Set[dset].Data_Points.size();i++)
	{
		Data_Point_Set[dset].Data_Points[i].x -= mx;
		Data_Point_Set[dset].Data_Points[i].y -= my;
		Data_Point_Set[dset].Data_Points[i].z -= mz;
	}

	for(long i=0;i<(signed)Ablation_Points_List.size();i++)
	{
		Ablation_Points_List[i].x -= mx;
		Ablation_Points_List[i].y -= my;
		Ablation_Points_List[i].z -= mz;
	}

	translate_ABC_spatial_registration_markers(mx,my,mz);

	}

	compute_segments_centers(500);
}

//--------------------------------------------------------------------------

void Surface_Class::expand_seed(int Value_Ptr,int Data_Point_Set_Ptr,
				double Threshold, int Direction, Island_on_3D_Map *Island,
				bool Only_Nodes_With_Close_DP)
{
	long node_ptr,neig_node_ptr;

	// main loop of identyfying islands
	bool sth_done = true;
	while(sth_done)
	{
		sth_done = false;

		for(long i=0;i<Island->Nodes_Ptrs.size();i++)
		{

		node_ptr = Island[0].Nodes_Ptrs[i];

		// check if neigs also below threshold
		for(int n=0;n<Surface_Node_Set[node_ptr].Neighbors.size();n++)
		{
			neig_node_ptr = Surface_Node_Set[node_ptr].Neighbors[n];

			if( Surface_Node_Set[neig_node_ptr].Flag_A == 0 )
			if( Only_Nodes_With_Close_DP && Surface_Node_Set[neig_node_ptr].Flag_A == 0 &&
											Surface_Node_Set[neig_node_ptr].Data_Point_Nearby || // only mapped nodes!!!
			   !Only_Nodes_With_Close_DP && Surface_Node_Set[neig_node_ptr].Flag_A == 0 )
			if( Direction == -1 && Surface_Node_Set[neig_node_ptr].get_value(Data_Point_Set_Ptr,Value_Ptr) < Threshold ||
				Direction == +1 && Surface_Node_Set[neig_node_ptr].get_value(Data_Point_Set_Ptr,Value_Ptr) > Threshold )
			{
				sth_done = true;
				Surface_Node_Set[neig_node_ptr].Flag_A = 1; // mark as visited
				Island[0].Nodes_Ptrs.push_back(neig_node_ptr);
			}
		}// thru all neigs

		} // thru all nodes of island

	} // while sth done
}

//--------------------------------------------------------------------------

std::vector<Island_on_3D_Map> Surface_Class::get_islands(int Value_Ptr,
		int Data_Point_Set_Ptr,double Threshold,int Direction,bool Only_Nodes_With_Close_DP,
		double Min_Island_Area)
{
	std::vector<Island_on_3D_Map> Islands;
	Island_on_3D_Map Island;
	long Seed_Ptr;

	//---------------------------------------------
	// clear flags (mark all flags as not-visited)
	//---------------------------------------------
	for(long n=0;n<Surface_Node_Set.size();n++)
	Surface_Node_Set[n].Flag_A = 0;

	//---------------------------------------------
	// main loop of identyfying islands
	//---------------------------------------------
	bool sth_done = true;
	while(sth_done)
	{
		sth_done = false;

		// find seed
		Seed_Ptr = -1;
		for(long n=0;n<Surface_Node_Set.size();n++)
		if( Only_Nodes_With_Close_DP && Surface_Node_Set[n].Flag_A == 0 && Surface_Node_Set[n].Data_Point_Nearby || // only mapped nodes!!!
			!Only_Nodes_With_Close_DP && Surface_Node_Set[n].Flag_A == 0 )
		if( Direction == -1 && Surface_Node_Set[n].get_value(Data_Point_Set_Ptr,Value_Ptr) < Threshold ||
			Direction == +1 && Surface_Node_Set[n].get_value(Data_Point_Set_Ptr,Value_Ptr) > Threshold )
		Seed_Ptr = n;

		if( Seed_Ptr >= 0 )
		{

		sth_done = true;

		// create island seed
		Island.Nodes_Ptrs.clear();
		Island.Nodes_Ptrs.push_back(Seed_Ptr);

		Surface_Node_Set[Seed_Ptr].Flag_A = 1; // mark as visited

		// expand seed
		expand_seed(Value_Ptr,Data_Point_Set_Ptr,Threshold,Direction,
					&Island,Only_Nodes_With_Close_DP);

		// add island to the list
		Islands.push_back(Island);

		} // seed found
	}

	//---------------------------------------------
	// find island center nodes
	//---------------------------------------------
	for(int i=0;i<Islands.size();i++)
	{
		// find xyz center
		double cx=0,cy=0,cz=0,count=0;

		for(long n=0;n<Islands[i].Nodes_Ptrs.size();n++)
		{
			cx += Surface_Node_Set[ Islands[i].Nodes_Ptrs[n] ].x;
			cy += Surface_Node_Set[ Islands[i].Nodes_Ptrs[n] ].y;
			cz += Surface_Node_Set[ Islands[i].Nodes_Ptrs[n] ].z;
			count++;
		}

		if( count != 0 )
		{
			cx /= count; cy /= count; cz /= count;
		}

		Islands[i].Geometric_Center_x = cx;
		Islands[i].Geometric_Center_y = cy;
		Islands[i].Geometric_Center_z = cz;

		// find which node is closest to this point
		Islands[i].Center_Node = get_closest_node_to_xyz(cx,cy,cz,1000);
	}

	//---------------------------------------------
	// find island triangles
	//---------------------------------------------
	for(int i=0;i<Islands.size();i++)
	{

	Islands[i].Triangles_Ptrs.clear();
	Islands[i].Area = 0;

	for(long t=0;t<Surface_Triangle_Set.size();t++)
	if( PNUM.is_value_present_in_vector_long(&Islands[i].Nodes_Ptrs,
					Surface_Triangle_Set[t].Nodes[0] ) &&
			PNUM.is_value_present_in_vector_long(&Islands[i].Nodes_Ptrs,
					Surface_Triangle_Set[t].Nodes[1] ) &&
			PNUM.is_value_present_in_vector_long(&Islands[i].Nodes_Ptrs,
					Surface_Triangle_Set[t].Nodes[2] ) )
	{
		Islands[i].Triangles_Ptrs.push_back(t);
		Islands[i].Area += Surface_Triangle_Set[t].Area_cm2;
	}

	}

	//---------------------------------------------
	// remove islands with no actual area
	//---------------------------------------------
	for(int i=0;i<Islands.size();i++)
	if( Islands[i].Area == 0 )
	{
		Islands.erase(Islands.begin() + i );
		i--;
	}

	//---------------------------------------------
	// remove small islands
	//---------------------------------------------
	for(int i=0;i<Islands.size();i++)
	if( Islands[i].Area < Min_Island_Area )
	{
		Islands.erase(Islands.begin() + i );
		i--;
	}

	return Islands;
}

//--------------------------------------------------------------------------

bool Surface_Class::get_islands_statistics(int Value_Ptr,int Data_Point_Set_Ptr,
		double Threshold,int Direction,int *Number,double *PercArea, double *Area,
		bool Only_Nodes_With_Close_DP,double Min_Island_Area)
{
	std::vector<Island_on_3D_Map> Islands =
		get_islands(Value_Ptr,Data_Point_Set_Ptr,
					Threshold,Direction,Only_Nodes_With_Close_DP,Min_Island_Area);

	// calculate stats
	Number[0] = Islands.size();

	Area[0] = 0;
	for(int i=0;i<Islands.size();i++)
		Area[0] += Islands[i].Area;

	PercArea[0] = Area[0] / get_total_area();

	return true;
}

//--------------------------------------------------------------------------

std::vector<double> Surface_Class::get_data_of_biggest_island(int Value_Ptr,int Data_Point_Set_Ptr,
		double Threshold, int Direction, bool Only_Nodes_With_Close_DP)
{
	std::vector<double> Data;

	std::vector<Island_on_3D_Map> Islands =
		get_islands(Value_Ptr,Data_Point_Set_Ptr,
					Threshold,Direction,Only_Nodes_With_Close_DP,0);

	// find largest island
	double Max=0,Max_Ptr=-1;

	for(int i=0;i<Islands.size();i++)
	if( Islands[i].Area > Max )
	{
		Max = Islands[i].Area;
		Max_Ptr = i;
	}

	// get data of island
	if( Max_Ptr >= 0 )
	{

	for(long i=0;i<Islands[Max_Ptr].Nodes_Ptrs.size();i++)
	Data.push_back(
		Surface_Node_Set[Islands[Max_Ptr].Nodes_Ptrs[i]].
			get_value(Data_Point_Set_Ptr,Value_Ptr));
	}

	return Data;
}

//--------------------------------------------------------------------------

std::vector<long> Surface_Class::get_nodes_of_biggest_island(int Value_Ptr,int Data_Point_Set_Ptr,
		double Threshold, int Direction, bool Only_Nodes_With_Close_DP)
{
	std::vector<long> Nodes;

	std::vector<Island_on_3D_Map> Islands =
		get_islands(Value_Ptr,Data_Point_Set_Ptr,
					Threshold,Direction,Only_Nodes_With_Close_DP,0);

	// find largest island
	double Max=0,Max_Ptr=-1;

	for(int i=0;i<Islands.size();i++)
	if( Islands[i].Area > Max )
	{
		Max = Islands[i].Area;
		Max_Ptr = i;
	}

	// get data of island
	if( Max_Ptr >= 0 )
	Nodes = Islands[Max_Ptr].Nodes_Ptrs;

	return Nodes;
}

//--------------------------------------------------------------------------

std::vector<long> Surface_Class::get_nodes_of_island_with_max_value(int Value_Ptr,
		int Data_Point_Set_Ptr,double Threshold, int Direction, bool Only_Nodes_With_Close_DP)
{
	std::vector<long> Nodes;

	std::vector<Island_on_3D_Map> Islands =
		get_islands(Value_Ptr,Data_Point_Set_Ptr,Threshold,Direction,
			Only_Nodes_With_Close_DP,0);

	// find island with max value
	double Max=-100000000,Max_Ptr=-1;

	for(int i=0;i<Islands.size();i++)
	for(long n=0;n<Islands[i].Nodes_Ptrs.size();n++)
	if( Surface_Node_Set[Islands[i].Nodes_Ptrs[n]].
			get_value(Data_Point_Set_Ptr,Value_Ptr) > Max )
	{
		Max = Surface_Node_Set[Islands[i].Nodes_Ptrs[n]].
			get_value(Data_Point_Set_Ptr,Value_Ptr);
		Max_Ptr = i;
	}

	// get data of island
	if( Max_Ptr >= 0 )
	Nodes = Islands[Max_Ptr].Nodes_Ptrs;

	return Nodes;
}
//--------------------------------------------------------------------------

void Surface_Class::restore_previous_segmentation_state()
{
	if( Surface_Triangle_Set.size() > 0 )
	if( Surface_Triangle_Set[0].Segmentation_Ids_For_Undo_Feature.size() > 0 )
	for(long t=0;t<Surface_Triangle_Set.size();t++)
	{

	long ptr = Surface_Triangle_Set[t].Segmentation_Ids_For_Undo_Feature.size()-1;

	if( ptr >= 0 )
	{
		if( Surface_Triangle_Set[t].Removed_Flag == OFF )
		Surface_Triangle_Set[t].Segment_Id =
			Surface_Triangle_Set[t].Segmentation_Ids_For_Undo_Feature[ptr];

		Surface_Triangle_Set[t].Segmentation_Ids_For_Undo_Feature.erase(
			Surface_Triangle_Set[t].Segmentation_Ids_For_Undo_Feature.begin() + ptr );
	}
	}
}

//------------------------------------------------------------------------

void Surface_Class::save_segmentation_state()
{
	int MAX_UNDO_STEPS_NUMBER = 50;

	if( Surface_Triangle_Set.size() > 0 )
	{

	for(long t=0;t<Surface_Triangle_Set.size();t++)
//	if( Surface_Triangle_Set[t].Removed_Flag == OFF )
	Surface_Triangle_Set[t].Segmentation_Ids_For_Undo_Feature.push_back(
		Surface_Triangle_Set[t].Segment_Id);

	if( Surface_Triangle_Set[0].Segmentation_Ids_For_Undo_Feature.size() > MAX_UNDO_STEPS_NUMBER )
	{
		for(long t=0;t<Surface_Triangle_Set.size();t++)
//		if( Surface_Triangle_Set[t].Removed_Flag == OFF )
		Surface_Triangle_Set[t].Segmentation_Ids_For_Undo_Feature.erase(
			Surface_Triangle_Set[t].Segmentation_Ids_For_Undo_Feature.begin() );
	}

	}
}
//------------------------------------------------------------------------

void Surface_Class::compute_segments_centers(int Max_Segments_Number)
{
	long CN;
	double cx,cy,cz;
	double Counter,N_ptr;

	// check if size is consistent
	if( Segments_Centers_X.size() != Max_Segments_Number )
	{
		double v=0;
		bool tmpb;
		Segments_Centers_X.clear();
		Segments_Centers_X_normal.clear();
		Segments_Centers_X.assign(Max_Segments_Number,v);
		Segments_Centers_X_normal.assign(Max_Segments_Number,v);

		Segments_Centers_Y.clear();
		Segments_Centers_Y_normal.clear();
		Segments_Centers_Y.assign(Max_Segments_Number,v);
		Segments_Centers_Y_normal.assign(Max_Segments_Number,v);

		Segments_Centers_Z.clear();
		Segments_Centers_Z_normal.clear();
		Segments_Centers_Z.assign(Max_Segments_Number,v);
		Segments_Centers_Z_normal.assign(Max_Segments_Number,v);

		Segments_Present.clear();
		Segments_Present.assign(Max_Segments_Number,tmpb);
	}

	// find which segments are present
	for(int S=0;S<Max_Segments_Number;S++)
	{
		Segments_Present[S] = false;
		Segments_Centers_X[S]=0;
		Segments_Centers_Y[S]=0;
		Segments_Centers_Z[S]=0;
	}

	for(long j=0;j<(signed)Surface_Triangle_Set.size();j++)
	if( Surface_Triangle_Set[j].Removed_Flag != ON)
	if( Surface_Triangle_Set[j].Segment_Id >= 0 )
	Segments_Present[ Surface_Triangle_Set[j].Segment_Id ] = true;

	for(int S=0;S<Max_Segments_Number;S++)
	if( Segments_Present[S] )
	{

	Counter = 0;
	cx=0,cy=0,cz=0;

	for( long t=0; t<(signed)Surface_Triangle_Set.size(); t++)
	if( Surface_Triangle_Set[t].Removed_Flag != ON )
	if(Surface_Triangle_Set[t].Segment_Id == S )
	if( Segments_Present[S] )
	{
			N_ptr = Surface_Triangle_Set[t].Nodes[0];

			cx += Surface_Node_Set[N_ptr].x;
			cy += Surface_Node_Set[N_ptr].y;
			cz += Surface_Node_Set[N_ptr].z;

			Counter++;
	}

	if( Counter != 0 )
	{
			cx /= (double)Counter;
			cy /= (double)Counter;
			cz /= (double)Counter;
	}

	// find normal
	CN = get_closest_node_to_xyz(cx,cy,cz,10000);

	if( Counter > 0 )
	if( CN >= 0 && CN < Surface_Node_Set.size() )
	{

		Segments_Centers_X[S] = Surface_Node_Set[CN].x;
		Segments_Centers_Y[S] = Surface_Node_Set[CN].y;
		Segments_Centers_Z[S] = Surface_Node_Set[CN].z;

		Segments_Centers_X_normal[S] = Surface_Node_Set[CN].normal[0];
		Segments_Centers_Y_normal[S] = Surface_Node_Set[CN].normal[1];
		Segments_Centers_Z_normal[S] = Surface_Node_Set[CN].normal[2];
	}

	} // through all present segments
}

//---------------------------------------------------------------------------
void Surface_Class::invert_normals()
{
	for(long i=0;i<Surface_Node_Set.size();i++)
	{
		Surface_Node_Set[i].normal[0] *= -1;
		Surface_Node_Set[i].normal[1] *= -1;
		Surface_Node_Set[i].normal[2] *= -1;
	}

	for(long j=0;j<Surface_Triangle_Set.size();j++)
	{
		Surface_Triangle_Set[j].normal[0] *= -1;
		Surface_Triangle_Set[j].normal[1] *= -1;
		Surface_Triangle_Set[j].normal[2] *= -1;
	}
}

//---------------------------------------------------------------------------

void Surface_Class::get_surface_dimensions(double* x,double* y,double* z)
{
	double xmin = MAXDOUBLE;
	double xmax = MINDOUBLE;
	double ymin = MAXDOUBLE;
	double ymax = MINDOUBLE;
	double zmin = MAXDOUBLE;
	double zmax = MINDOUBLE;

	for(long n=0;n<Surface_Node_Set.size();n++)
	{
		if(   Surface_Node_Set[n].x > xmax )
		xmax = Surface_Node_Set[n].x;
		if(   Surface_Node_Set[n].x < xmin )
		xmin = Surface_Node_Set[n].x;

		if(   Surface_Node_Set[n].y > ymax )
		ymax = Surface_Node_Set[n].y;
		if(   Surface_Node_Set[n].y < ymin )
		ymin = Surface_Node_Set[n].y;

		if(   Surface_Node_Set[n].z > zmax )
		zmax = Surface_Node_Set[n].z;
		if(   Surface_Node_Set[n].z < zmin )
		zmin = Surface_Node_Set[n].z;
	}

	x[0] = xmax-xmin;
	y[0] = ymax-ymin;
	z[0] = zmax-zmin;
}

//---------------------------------------------------------------------------

void Surface_Class::compute_paths_between_start_and_end()
{
	vector <long> vv;

	//----------------------------------------------------------------------------
	if( Path_Intermediate >= 0 )
	{
	Path_Vector = get_shortest_path_between_nodes(Path_Start,Path_Intermediate);
	vv = get_shortest_path_between_nodes(Path_Intermediate,Path_End);

	Path_Vector.insert( Path_Vector.end(), vv.begin(), vv.end() );
	}
	else
	Path_Vector = get_shortest_path_between_nodes(Path_Start,Path_End);
	//----------------------------------------------------------------------------

//Path_Vector = get_shortest_path_between_nodes(Path_Start,Path_End);
}

//---------------------------------------------------------------------------

double Surface_Class::get_euclidean_distance_between_nodes(long n1, long n2)
{
	if(n1>=0 && n1 < Surface_Node_Set.size() )
	if(n2>=0 && n2 < Surface_Node_Set.size() )
	{

	return std::sqrt(
		std::pow( Surface_Node_Set[n1].x - Surface_Node_Set[n2].x, 2) +
		std::pow( Surface_Node_Set[n1].y - Surface_Node_Set[n2].y, 2) +
		std::pow( Surface_Node_Set[n1].z - Surface_Node_Set[n2].z, 2) );
	}
	else
	return -1;
}

//---------------------------------------------------------------------------

bool Surface_Class::node_values_valid(long Node_Ptr)
{
/*
	bool Valid = false;

	if( !Surface_Node_Set[Node_Ptr].Removed_Geometry_Vertex_Flag )
	if( Surface_Node_Set[Node_Ptr].Closest_Data_Point_Id >= 0 )
	if( data_points_set_ptr_in_range() )
	if( Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points[
		Surface_Node_Set[Node_Ptr].Closest_Data_Point_Id].
					get_value(Map_Values.Current_Map_Value_Name,
						Map_Values.get_values_table_ref()) != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	Valid = true;

	return Valid;
*/
	int Value_Id = Map_Values.get_value_ptr(Map_Values.Current_Map_Value_Name);
	bool Valid = false;

	if( data_points_set_ptr_in_range() )
	if( Surface_Node_Set[Node_Ptr].get_value(Current_Data_Point_Set_Ptr,Value_Id) != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	Valid = true;

	return Valid;

}

//---------------------------------------------------------------------------

bool Surface_Class::node_values_valid_specific_value(long Node_Ptr,long Val_Ptr)
{
/*
	bool Valid = false;

	if( !Surface_Node_Set[Node_Ptr].Removed_Geometry_Vertex_Flag )
	if( Surface_Node_Set[Node_Ptr].Closest_Data_Point_Id >= 0 )
	if( data_points_set_ptr_in_range() )
	if( Data_Point_Set[Current_Data_Point_Set_Ptr].Data_Points[
		Surface_Node_Set[Node_Ptr].Closest_Data_Point_Id].
					get_value(Map_Values.get_value_name_according_to_ptr(Val_Ptr),
						Map_Values.get_values_table_ref()) != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	Valid = true;

	return Valid;
*/
	bool Valid = false;

	if( data_points_set_ptr_in_range() )
	if( Surface_Node_Set[Node_Ptr].get_value(Current_Data_Point_Set_Ptr,Val_Ptr) != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	Valid = true;

	return Valid;

}

//---------------------------------------------------------------------------

bool Surface_Class::triangle_values_valid(long Triangle_Ptr)
{
	bool Valid = false;

	if( Surface_Triangle_Set[Triangle_Ptr].Removed_Flag != ON)
	if( node_values_valid( Surface_Triangle_Set[Triangle_Ptr].Nodes[0] ) )
	if( node_values_valid( Surface_Triangle_Set[Triangle_Ptr].Nodes[1] ) )
	if( node_values_valid( Surface_Triangle_Set[Triangle_Ptr].Nodes[2] ) )
	Valid = true;

	return Valid;
}

//---------------------------------------------------------------------------

bool Surface_Class::triangle_values_valid_specific_value(long Triangle_Ptr,long Val_Ptr)
{
	bool Valid = false;

	if( Surface_Triangle_Set[Triangle_Ptr].Removed_Flag != ON)
	if( node_values_valid_specific_value( Surface_Triangle_Set[Triangle_Ptr].Nodes[0],Val_Ptr ) )
	if( node_values_valid_specific_value( Surface_Triangle_Set[Triangle_Ptr].Nodes[1],Val_Ptr ) )
	if( node_values_valid_specific_value( Surface_Triangle_Set[Triangle_Ptr].Nodes[2],Val_Ptr ) )
	Valid = true;

	return Valid;
}

//---------------------------------------------------------------------------

void Surface_Class::translate_ABC_spatial_registration_markers(double x,double y,double z)
{
	Registration_Point_A.x -= x;
	Registration_Point_A.y -= y;
	Registration_Point_A.z -= z;

	Registration_Point_B.x -= x;
	Registration_Point_B.y -= y;
	Registration_Point_B.z -= z;

	Registration_Point_C.x -= x;
	Registration_Point_C.y -= y;
	Registration_Point_C.z -= z;
}

//---------------------------------------------------------------------------

void Surface_Class::scale_ABC_spatial_registration_markers(double x,double y,double z)
{
	Registration_Point_A.x *= x;
	Registration_Point_A.y *= y;
	Registration_Point_A.z *= z;

	Registration_Point_B.x *= x;
	Registration_Point_B.y *= y;
	Registration_Point_B.z *= z;

	Registration_Point_C.x *= x;
	Registration_Point_C.y *= y;
	Registration_Point_C.z *= z;
}

//---------------------------------------------------------------------------
void Surface_Class::perform_one_step_of_triangles_ABC_fitting(Point_3D FRegistration_Point_A,
			Point_3D FRegistration_Point_B, Point_3D FRegistration_Point_C )
{
	long i,j,k;
	double dx_from_F,dy_from_F,dz_from_F,f,d;
	int reversed_z_direction;

	double F1x,F1y,F1z,F2x,F2y,F2z,F3x,F3y,F3z;
	double M1x,M1y,M1z,M2x,M2y,M2z,M3x,M3y,M3z,Mx,My,Mz;
	double Tx,Ty,Tz;
	double d1,d2,d3,fi1,fi2,fi3;
	double Spring_Constant;
	double dtF, dtM; // time steps in F movement and rotation (M)

	Spring_Constant = 5.0;
	dtF = 0.005;
	dtM = 0.00001;

	// steps:
// 1. calculate forces acting on flagged nodes
// 2. center data cloud (remember translation)
// 3. calculate momentum acting on data cloud   M = r x F
// 4. calculate translation of d.c. due to forces F
// 5. calculate rotation of d.c. due to momenum M
// 6. Rotate d.c., translate d.c.
// 7. Translate d.c. to initial position

//     dcs: FRegistration_Point_A     : Intersection_Point_A
//     sns: FRegistration_Point_A     : Geometry_Model.FRegistration_Point_A (obracane)

// 1.

F1x = -Spring_Constant*(FRegistration_Point_A.x - Registration_Point_A.x);
F1y = -Spring_Constant*(FRegistration_Point_A.y - Registration_Point_A.y);
F1z = -Spring_Constant*(FRegistration_Point_A.z - Registration_Point_A.z);

F2x = -Spring_Constant*(FRegistration_Point_B.x - Registration_Point_B.x);
F2y = -Spring_Constant*(FRegistration_Point_B.y - Registration_Point_B.y);
F2z = -Spring_Constant*(FRegistration_Point_B.z - Registration_Point_B.z);

F3x = -Spring_Constant*(FRegistration_Point_C.x - Registration_Point_C.x);
F3y = -Spring_Constant*(FRegistration_Point_C.y - Registration_Point_C.y);
F3z = -Spring_Constant*(FRegistration_Point_C.z - Registration_Point_C.z);

// 2. Center registration points of [0] surface

Tx = (FRegistration_Point_A.x +
	  FRegistration_Point_B.x +
	  FRegistration_Point_C.x)/3.0;

Ty = (FRegistration_Point_A.y +
	  FRegistration_Point_B.y +
	  FRegistration_Point_C.y)/3.0;

Tz = (FRegistration_Point_A.z +
	  FRegistration_Point_B.z +
	  FRegistration_Point_C.z)/3.0;

		FRegistration_Point_A.x -= Tx;
		FRegistration_Point_A.y -= Ty;
		FRegistration_Point_A.z -= Tz;

		FRegistration_Point_B.x -= Tx;
		FRegistration_Point_B.y -= Ty;
		FRegistration_Point_B.z -= Tz;

		FRegistration_Point_C.x -= Tx;
		FRegistration_Point_C.y -= Ty;
		FRegistration_Point_C.z -= Tz;

// 3. M = rxF

M1x = FRegistration_Point_A.y*F1z - FRegistration_Point_A.z*F1y;
M1y = FRegistration_Point_A.z*F1x - FRegistration_Point_A.x*F1z;
M1z = FRegistration_Point_A.x*F1y - FRegistration_Point_A.y*F1x;

M2x = FRegistration_Point_B.y*F2z - FRegistration_Point_B.z*F2y;
M2y = FRegistration_Point_B.z*F2x - FRegistration_Point_B.x*F2z;
M2z = FRegistration_Point_B.x*F2y - FRegistration_Point_B.y*F2x;

M3x = FRegistration_Point_C.y*F3z - FRegistration_Point_C.z*F3y;
M3y = FRegistration_Point_C.z*F3x - FRegistration_Point_C.x*F3z;
M3z = FRegistration_Point_C.x*F3y - FRegistration_Point_C.y*F3x;

Mx = (M1x+M2x+M3x)/3.0;
My = (M1y+M2y+M3y)/3.0;
Mz = (M1z+M2z+M3z)/3.0;

	//------------------------------------------------------------------
	// 4. calculate translation of d.c. due to forces F
	//------------------------------------------------------------------

	dx_from_F = dtF*(F1x + F2x + F3x);
	dy_from_F = dtF*(F1y + F2y + F3y);
	dz_from_F = dtF*(F1z + F2z + F3z);

	//------------------------------------------------------------------
	// 5. calculate rotation matrix of d.c. due to M=rXF
	//------------------------------------------------------------------

	//  calculate rotation angle due to M
   fi3 = dtM*(sqrt( pow(Mx,2)+pow(My,2)+pow(Mz,2) ));

	// rotation around z-axis to set y=0
	if(Mx!=0)
	{
		d = atan(fabs(My/Mx));

			// 1st quarter
			if( Mx > 0 && My > 0 )
				fi1 = d;

			// 2nd quarter
			if( Mx < 0 && My > 0 )
				fi1 = M_PI - d;

			// 3rd quarter
			if( Mx < 0 && My < 0 )
                fi1 = M_PI + d;

			// 4th quarter
            if( Mx > 0 && My < 0 )
				fi1 = 2.0*M_PI - d;
    }
	else
	{
		if(My > 0.0 )
				 fi1 = M_PI_2;
			else
				 fi1 = 3.0*M_PI_2;
	}

	PNUM.rotate_vector(&Mx,&My,&Mz,fi1,3); // rotate M around z-axis


	// rotation around y-axis to set x=0
	if(Mz!=0)
	{
		d = atan(fabs(Mx/Mz));

			// 1st quarter
			if( Mz > 0 && Mx > 0 )
				fi2 = d;

            // 2nd quarter
            if( Mz < 0 && Mx > 0 )
				fi2 = M_PI - d;

            // 3rd quarter
            if( Mz < 0 && Mx < 0 )
				fi2 = M_PI + d;

			// 4th quarter
			if( Mz > 0 && Mx < 0 )
				fi2 = 2.0*M_PI - d;
	}
	else
	{
		if(Mx > 0.0 )
				 fi2 = M_PI_2;
			else
				 fi2 = 3.0*M_PI_2;
	}

	PNUM.rotate_vector(&Mx,&My,&Mz,-fi2,2); // rotate M around y-axis

// ???? , maybe atan() does it, acknowledge direction of M vector
// if its directed downward, rotation is in opposite direction
if( Mz < 0 ) fi3 = -fi3;

//---------------------------------------------------------
// Now we have all rotations needed to calculate movement,
// so, lets do it! (FOR SURFACE_2)
//---------------------------------------------------------

	//----------------------------------------------------------------
	// 3D goemetry nodes
	//----------------------------------------------------------------
	for(i=0;i<(signed)Surface_Node_Set.size();i++)
	{

	PNUM.rotate_vector(
				  &Surface_Node_Set[i].x,
				  &Surface_Node_Set[i].y,
				  &Surface_Node_Set[i].z,fi1,3);

	PNUM.rotate_vector(
				  &Surface_Node_Set[i].x,
				  &Surface_Node_Set[i].y,
				  &Surface_Node_Set[i].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surface_Node_Set[i].x,
				  &Surface_Node_Set[i].y,
				  &Surface_Node_Set[i].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surface_Node_Set[i].x,
				  &Surface_Node_Set[i].y,
				  &Surface_Node_Set[i].z,fi2,2);

	PNUM.rotate_vector(
				  &Surface_Node_Set[i].x,
				  &Surface_Node_Set[i].y,
				  &Surface_Node_Set[i].z,-fi1,3);

	Surface_Node_Set[i].x -= dx_from_F;
	Surface_Node_Set[i].y -= dy_from_F;
	Surface_Node_Set[i].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// data points
	//----------------------------------------------------------------
	for(long dset=0;dset<(signed)Data_Point_Set.size();dset++)
	for(long d=0;d<(signed)Data_Point_Set[dset].Data_Points.size();d++)
	{
	PNUM.rotate_vector(
				  &Data_Point_Set[dset].Data_Points[d].x,
				  &Data_Point_Set[dset].Data_Points[d].y,
				  &Data_Point_Set[dset].Data_Points[d].z,fi1,3);

	PNUM.rotate_vector(
				  &Data_Point_Set[dset].Data_Points[d].x,
				  &Data_Point_Set[dset].Data_Points[d].y,
				  &Data_Point_Set[dset].Data_Points[d].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Data_Point_Set[dset].Data_Points[d].x,
				  &Data_Point_Set[dset].Data_Points[d].y,
				  &Data_Point_Set[dset].Data_Points[d].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Data_Point_Set[dset].Data_Points[d].x,
				  &Data_Point_Set[dset].Data_Points[d].y,
				  &Data_Point_Set[dset].Data_Points[d].z,fi2,2);

	PNUM.rotate_vector(
				  &Data_Point_Set[dset].Data_Points[d].x,
				  &Data_Point_Set[dset].Data_Points[d].y,
				  &Data_Point_Set[dset].Data_Points[d].z,-fi1,3);

	Data_Point_Set[dset].Data_Points[d].x -= dx_from_F;
	Data_Point_Set[dset].Data_Points[d].y -= dy_from_F;
	Data_Point_Set[dset].Data_Points[d].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// ablation points
	//----------------------------------------------------------------
	for(long a=0;a<(signed)Ablation_Points_List.size();a++)
	{
	PNUM.rotate_vector(
				  &Ablation_Points_List[a].x,
				  &Ablation_Points_List[a].y,
				  &Ablation_Points_List[a].z,fi1,3);

	PNUM.rotate_vector(
				  &Ablation_Points_List[a].x,
				  &Ablation_Points_List[a].y,
				  &Ablation_Points_List[a].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Ablation_Points_List[a].x,
				  &Ablation_Points_List[a].y,
				  &Ablation_Points_List[a].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Ablation_Points_List[a].x,
				  &Ablation_Points_List[a].y,
				  &Ablation_Points_List[a].z,fi2,2);

	PNUM.rotate_vector(
				  &Ablation_Points_List[a].x,
				  &Ablation_Points_List[a].y,
				  &Ablation_Points_List[a].z,-fi1,3);

	Ablation_Points_List[a].x -= dx_from_F;
	Ablation_Points_List[a].y -= dy_from_F;
	Ablation_Points_List[a].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// labels
	//----------------------------------------------------------------
	for(long a=0;a<(signed)Labels.size();a++)
	{
	PNUM.rotate_vector(
				  &Labels[a].x,
				  &Labels[a].y,
				  &Labels[a].z,fi1,3);

	PNUM.rotate_vector(
				  &Labels[a].x,
				  &Labels[a].y,
				  &Labels[a].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Labels[a].x,
				  &Labels[a].y,
				  &Labels[a].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Labels[a].x,
				  &Labels[a].y,
				  &Labels[a].z,fi2,2);

	PNUM.rotate_vector(
				  &Labels[a].x,
				  &Labels[a].y,
				  &Labels[a].z,-fi1,3);

	Labels[a].x -= dx_from_F;
	Labels[a].y -= dy_from_F;
	Labels[a].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// registration points on 3D goemetry
	//----------------------------------------------------------------
	PNUM.rotate_vector(
		&Registration_Point_A.x,&Registration_Point_A.y,&Registration_Point_A.z,fi1,3);
	PNUM.rotate_vector(
		&Registration_Point_B.x,&Registration_Point_B.y,&Registration_Point_B.z,fi1,3);
	PNUM.rotate_vector(
		&Registration_Point_C.x,&Registration_Point_C.y,&Registration_Point_C.z,fi1,3);

	PNUM.rotate_vector(
		&Registration_Point_A.x,&Registration_Point_A.y,&Registration_Point_A.z,-fi2,2);
	PNUM.rotate_vector(
		&Registration_Point_B.x,&Registration_Point_B.y,&Registration_Point_B.z,-fi2,2);
	PNUM.rotate_vector(
		&Registration_Point_C.x,&Registration_Point_C.y,&Registration_Point_C.z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
		&Registration_Point_A.x,&Registration_Point_A.y,&Registration_Point_A.z,fi3,3);
	PNUM.rotate_vector(
		&Registration_Point_B.x,&Registration_Point_B.y,&Registration_Point_B.z,fi3,3);
	PNUM.rotate_vector(
		&Registration_Point_C.x,&Registration_Point_C.y,&Registration_Point_C.z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
		&Registration_Point_A.x,&Registration_Point_A.y,&Registration_Point_A.z,fi2,2);
	PNUM.rotate_vector(
		&Registration_Point_B.x,&Registration_Point_B.y,&Registration_Point_B.z,fi2,2);
	PNUM.rotate_vector(
		&Registration_Point_C.x,&Registration_Point_C.y,&Registration_Point_C.z,fi2,2);

	PNUM.rotate_vector(
		&Registration_Point_A.x,&Registration_Point_A.y,&Registration_Point_A.z,-fi1,3);
	PNUM.rotate_vector(
		&Registration_Point_B.x,&Registration_Point_B.y,&Registration_Point_B.z,-fi1,3);
	PNUM.rotate_vector(
		&Registration_Point_C.x,&Registration_Point_C.y,&Registration_Point_C.z,-fi1,3);

	Registration_Point_A.x -= dx_from_F;
	Registration_Point_A.y -= dy_from_F;
	Registration_Point_A.z -= dz_from_F;

	Registration_Point_B.x -= dx_from_F;
	Registration_Point_B.y -= dy_from_F;
	Registration_Point_B.z -= dz_from_F;

	Registration_Point_C.x -= dx_from_F;
	Registration_Point_C.y -= dy_from_F;
	Registration_Point_C.z -= dz_from_F;
}

//---------------------------------------------------------------------------

void Surface_Class::check_values_for_abnormal_values()
{
	double v;
	AnsiString Value_Name;

	for(int ds=0; ds<Data_Point_Set.size(); ds++)
	for(int V=0;V<Map_Values.get_values_number();V++)
	{

	// check nodes values
	for (long i=0; i<Surface_Node_Set.size(); i++)
	{
		v = Surface_Node_Set[i].get_value(ds,V);

		if( fabs(v) > 10000000 )
			Surface_Node_Set[i].set_value(ds,V,-10000);
	}

	Value_Name = Map_Values.get_value_name_according_to_ptr(V);

	// check data points values
	for (long dp=0; dp<Data_Point_Set[ds].Data_Points.size(); dp++)
	{
		v = Data_Point_Set[ds].Data_Points[dp].get_value(
				Map_Values.get_value_name_according_to_ptr(V),
				Map_Values.get_values_table_ref());

		if( fabs(v) > 10000000 )
			Data_Point_Set[ds].Data_Points[dp].set_value(Value_Name,
					-10000,Map_Values.get_values_table_ref() );
	}

	}
}

//---------------------------------------------------------------------------

void Surface_Class::create_value_based_elevation_in_surface(int Data_Point_Set_Ptr,
		AnsiString Value_Name, double Threshold, int Direction, double Height_mm)
{
	int Type = 2;
	int Value_Id = Map_Values.get_value_ptr(Value_Name);
	double n0,n1,n2,v;

	for(long i=0; i < (signed)Surface_Node_Set.size(); i++ )
	if( Surface_Node_Set[i].Flag_A == 0 )
	{

	v = Surface_Node_Set[i].get_value(Data_Point_Set_Ptr,Value_Id);

	if( Surface_Node_Set[i].is_value_valid(Data_Point_Set_Ptr,Value_Id) &&
		( Direction==1  && v > Threshold || Direction==-1 && v < Threshold)  )
	{
		Surface_Node_Set[i].Flag_A = 1; // mark as moved

		n0 = Surface_Node_Set[i].normal[0];
		n1 = Surface_Node_Set[i].normal[1];
		n2 = Surface_Node_Set[i].normal[2];

		if( Type == 1 ) // depression
		{
		Surface_Node_Set[i].x -= Surface_Node_Set[i].normal[0]*Height_mm;
		Surface_Node_Set[i].y -= Surface_Node_Set[i].normal[1]*Height_mm;
		Surface_Node_Set[i].z -= Surface_Node_Set[i].normal[2]*Height_mm;

//		dodaj dps (niepotrzebne, bo tyko geo b?dzie eksportowane do Rhythmi - AFCAM)

		}

		if( Type == 2 ) // elevation
		{
		Surface_Node_Set[i].x += Surface_Node_Set[i].normal[0]*Height_mm;
		Surface_Node_Set[i].y += Surface_Node_Set[i].normal[1]*Height_mm;
		Surface_Node_Set[i].z += Surface_Node_Set[i].normal[2]*Height_mm;
		}

	}
	}

//	calculate_normals();
}

//---------------------------------------------------------------------------

void Surface_Class::create_deflection_in_surface(double Cx,double Cy, double Cz,double Radius, double Depth,int Type)
{
	// Types: 1: uniform depression
	 // 2: uniform elevation

	double dist;

	for(long i=0; i < (signed)Surface_Node_Set.size(); i++ )
	{

	dist = std::sqrt(
		std::pow(Surface_Node_Set[i].x - Cx,2)+
		std::pow(Surface_Node_Set[i].y - Cy,2)+
		std::pow(Surface_Node_Set[i].z - Cz,2) );

	if( dist <= Radius)
	{
		double n0 = Surface_Node_Set[i].normal[0];
		double n1 = Surface_Node_Set[i].normal[1];
		double n2 = Surface_Node_Set[i].normal[2];

		if( Type == 1 ) // depression
		{
		Surface_Node_Set[i].x -= Surface_Node_Set[i].normal[0]*Depth;
		Surface_Node_Set[i].y -= Surface_Node_Set[i].normal[1]*Depth;
		Surface_Node_Set[i].z -= Surface_Node_Set[i].normal[2]*Depth;
		}

		if( Type == 2 ) // elevation
		{
		Surface_Node_Set[i].x += Surface_Node_Set[i].normal[0]*Depth;
		Surface_Node_Set[i].y += Surface_Node_Set[i].normal[1]*Depth;
		Surface_Node_Set[i].z += Surface_Node_Set[i].normal[2]*Depth;
		}

	}

	}

	calculate_normals();
}

//---------------------------------------------------------------------------

void Surface_Class::upsample_mesh()
{
	Geometry_Vertex New_Vertex;
	vector<Geometry_Triangle> Surface_Triangle_Set_New;
	Geometry_Triangle New_T1,New_T2,New_T3;

	// 1. get mean field of the triangle
	calculate_areas_of_triangles();
	double mean_area = get_mean_triangle_area();

	// 2. go through all triangles
	for(long t=0;t<(signed)Surface_Triangle_Set.size();t++)
	{

	if( Surface_Triangle_Set[t].Area_cm2 > 0.5*mean_area )
	{
		New_Vertex = Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ];

		// new node coords
		New_Vertex.x = (
			Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].x +
			Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].x +
			Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].x )/3.0;
		New_Vertex.y = (
			Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].y +
			Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].y +
			Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].y )/3.0;
		New_Vertex.z = (
			Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[0] ].z +
			Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[1] ].z +
			Surface_Node_Set[ Surface_Triangle_Set[t].Nodes[2] ].z )/3.0;

		// add new vertex to the list
		Surface_Node_Set.push_back(New_Vertex);

		// create new 3 triangles
		New_T1.Nodes[0] = Surface_Triangle_Set[t].Nodes[0];
		New_T1.Nodes[1] = Surface_Triangle_Set[t].Nodes[1];
		New_T1.Nodes[2] = Surface_Node_Set.size()-1;

		New_T2.Nodes[0] = Surface_Triangle_Set[t].Nodes[1];
		New_T2.Nodes[1] = Surface_Triangle_Set[t].Nodes[2];
		New_T2.Nodes[2] = Surface_Node_Set.size()-1;

		New_T3.Nodes[0] = Surface_Triangle_Set[t].Nodes[0];
		New_T3.Nodes[1] = Surface_Node_Set.size()-1;
		New_T3.Nodes[2] = Surface_Triangle_Set[t].Nodes[2];

		Surface_Triangle_Set_New.push_back(New_T1);
		Surface_Triangle_Set_New.push_back(New_T2);
		Surface_Triangle_Set_New.push_back(New_T3);
	}
	else // triangle not to be modified
		Surface_Triangle_Set_New.push_back(Surface_Triangle_Set[t]);

	}

	Surface_Triangle_Set.clear();
	Surface_Triangle_Set = Surface_Triangle_Set_New;

	find_neig_nodes_and_neig_triangles();

	calculate_normals();
}

//---------------------------------------------------------------------------

void Surface_Class::create_labels_for_data_points_with_specified_type(
				int Type, int DataPointsSetPtr, AnsiString LabelName)
{
	Label_Class Label_Class_Item;
	Label_Class_Item.Name = LabelName;

	if( DataPointsSetPtr >= 0 && DataPointsSetPtr < Data_Point_Set.size() )
	for(long d=0;d<(signed)Data_Point_Set[DataPointsSetPtr].Data_Points.size();d++)
	if( Data_Point_Set[DataPointsSetPtr].Data_Points[d].Type == Type )
	{
		Label_Class_Item.x = Data_Point_Set[DataPointsSetPtr].Data_Points[d].x;
		Label_Class_Item.y = Data_Point_Set[DataPointsSetPtr].Data_Points[d].y;
		Label_Class_Item.z = Data_Point_Set[DataPointsSetPtr].Data_Points[d].z;

		if( Data_Point_Set[DataPointsSetPtr].Data_Points[d].Closest_Node_Id >= 0 &&
			Data_Point_Set[DataPointsSetPtr].Data_Points[d].Closest_Node_Id < Surface_Node_Set.size() )
		{
		Label_Class_Item.nx = Surface_Node_Set[ Data_Point_Set[DataPointsSetPtr].Data_Points[d].Closest_Node_Id ].normal[0];
		Label_Class_Item.ny = Surface_Node_Set[ Data_Point_Set[DataPointsSetPtr].Data_Points[d].Closest_Node_Id ].normal[1];
		Label_Class_Item.nz = Surface_Node_Set[ Data_Point_Set[DataPointsSetPtr].Data_Points[d].Closest_Node_Id ].normal[2];
		}

		Labels.push_back( Label_Class_Item );
	}
}

//---------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------

long Surface_Class::get_central_node(vector <long> *Nodes_Ptrs)
{
	double cx=0,cy=0,cz=0;
	long Counter;

	for(long n=0;n<Nodes_Ptrs[0].size();n++)
	{
		cx += Surface_Node_Set[Nodes_Ptrs[0][n]].x;
		cy += Surface_Node_Set[Nodes_Ptrs[0][n]].y;
		cz += Surface_Node_Set[Nodes_Ptrs[0][n]].z;
	}

	if( Nodes_Ptrs[0].size() > 0 )
	{
		cx /= (double)(Nodes_Ptrs[0].size());
		cy /= (double)(Nodes_Ptrs[0].size());
		cz /= (double)(Nodes_Ptrs[0].size());
	}

	long N = get_closest_node_to_xyz(cx,cy,cz,1000);

	return N;
}


//---------------------------------------------------------------------------

bool Surface_Class::is_segment_present(int Segment_Id)
{
	bool present=false;
	for(long j=0;j<(signed)Surface_Triangle_Set.size();j++)
	if( Surface_Triangle_Set[j].Segment_Id == Segment_Id)
	present = true;

	return present;
}

//---------------------------------------------------------------------------

void Surface_Class::calculate_segments_of_data_points(int Dset)
{
	for(long i=0;i<(signed)Data_Point_Set[Dset].Data_Points.size();i++)
		Data_Point_Set[Dset].Data_Points[i].Segment_Id = get_data_point_segment_id(Dset,i);
}

//---------------------------------------------------------------------------

void Surface_Class::calculate_closest_nodes_and_data_points_ALL(int Data_Point_Set_Id)
{
	if( Data_Point_Set_Id >= 0 && Data_Point_Set_Id < Data_Point_Set.size() )
	{

	//-----------------------------------------------------------------------------
	// main loop through nodes
	//-----------------------------------------------------------------------------
	for(unsigned long j=0;j<Surface_Node_Set.size();j++)
	if( !Surface_Node_Set[j].Removed_Geometry_Vertex_Flag )
	{
		if(j%50==1)
		{
			Progress_Form->replace_last_line_with_text("Finding closest data points to node:"+FloatToStr(j)+"/"+FloatToStr(Surface_Node_Set.size()));
			Progress_Form->Show();
			Application->ProcessMessages();
		}

		find_closest_data_point_to_node(j,Data_Point_Set_Id);
	}

	//-----------------------------------------------------------------------------
	// main loop through data points
	//-----------------------------------------------------------------------------
	for(unsigned long dp=0;dp<Data_Point_Set[Data_Point_Set_Id].Data_Points.size();dp++)
	{
		if(dp%50==1)
		{
			Progress_Form->replace_last_line_with_text("Finding closest node to data point:"+FloatToStr(dp)+"/"+Data_Point_Set[Data_Point_Set_Id].Data_Points.size());
			Progress_Form->Show();
			Application->ProcessMessages();
		}

		find_closest_node_to_data_point(dp,Data_Point_Set_Id);
	}

	} // if dset in range
}

//---------------------------------------------------------------------------

void Surface_Class::find_closest_data_point_to_node(long Node_Ptr,long Data_Point_Set_Id)
{
	// calculates both, closest DP and closest set of DPs
	if( Node_Ptr >= 0 && Node_Ptr < Surface_Node_Set.size() )
	if( Data_Point_Set_Id >= 0 && Data_Point_Set_Id < Data_Point_Set.size() )
	{

	double min_distance,distance;
	long min_ptr;

	// clear values
	Surface_Node_Set[Node_Ptr].Closest_Data_Point_Id = -1;
	Surface_Node_Set[Node_Ptr].Closest_Data_Points_Ptrs.clear();
	Surface_Node_Set[Node_Ptr].Closest_Data_Points_Distances.clear();

	min_distance = 100000000.0;
	min_ptr = -1;

	for(long dp_ptr=0;dp_ptr<(signed)Data_Point_Set[Data_Point_Set_Id].Data_Points.size();dp_ptr++)
	if( Data_Point_Set[Data_Point_Set_Id].Data_Points[dp_ptr].valid_for_mapping(Mapping_System_Source) )
	if( fabs( Surface_Node_Set[Node_Ptr].x - Data_Point_Set[Data_Point_Set_Id].Data_Points[dp_ptr].x ) < Data_Points_Filling_Threshold_mm)
	if( fabs( Surface_Node_Set[Node_Ptr].y - Data_Point_Set[Data_Point_Set_Id].Data_Points[dp_ptr].y ) < Data_Points_Filling_Threshold_mm)
	if( fabs( Surface_Node_Set[Node_Ptr].z - Data_Point_Set[Data_Point_Set_Id].Data_Points[dp_ptr].z ) < Data_Points_Filling_Threshold_mm)
	{

	// find closest surface node
	distance = sqrt(std::pow( Surface_Node_Set[Node_Ptr].x - Data_Point_Set[Data_Point_Set_Id].Data_Points[dp_ptr].x , 2 ) +
					std::pow( Surface_Node_Set[Node_Ptr].y - Data_Point_Set[Data_Point_Set_Id].Data_Points[dp_ptr].y , 2 ) +
					std::pow( Surface_Node_Set[Node_Ptr].z - Data_Point_Set[Data_Point_Set_Id].Data_Points[dp_ptr].z , 2 ));

	// since this DP is closer than Max_Dist_Th
	Surface_Node_Set[Node_Ptr].Closest_Data_Points_Ptrs.push_back(dp_ptr);
	Surface_Node_Set[Node_Ptr].Closest_Data_Points_Distances.push_back(distance);

	if ( distance < min_distance )
	{
		min_distance = distance;
		min_ptr = dp_ptr;
	}

	} // through all data points in neighbourhood

	// assign the closest one
	if( min_ptr >= 0 && min_distance < Data_Points_Filling_Threshold_mm )
	{
		Surface_Node_Set[Node_Ptr].Closest_Data_Point_Id = min_ptr;
		Surface_Node_Set[Node_Ptr].Closest_Data_Point_Id_Unlimited = min_ptr;
		Surface_Node_Set[Node_Ptr].Type = NORMAL_TYPE;
		Surface_Node_Set[Node_Ptr].Data_Point_Nearby = true;
	}
	else
	{
		Surface_Node_Set[Node_Ptr].Closest_Data_Point_Id = -1;
		Surface_Node_Set[Node_Ptr].Closest_Data_Point_Id_Unlimited = min_ptr;
		Surface_Node_Set[Node_Ptr].Type = NORMAL_TYPE;
		Surface_Node_Set[Node_Ptr].Data_Point_Nearby = false;
	}

	} // if dset in range
}

//---------------------------------------------------------------------------

void Surface_Class::find_closest_node_to_data_point(long DP_Ptr,long Data_Point_Set_Id)
{
	if( Data_Point_Set_Id >= 0 && Data_Point_Set_Id < Data_Point_Set.size() )
	if( DP_Ptr >= 0 && DP_Ptr < Data_Point_Set[Data_Point_Set_Id].Data_Points.size() )
	{
	Data_Point_Set[Data_Point_Set_Id].Data_Points[DP_Ptr].Closest_Node_Id =
		get_closest_node_to_xyz(Data_Point_Set[Data_Point_Set_Id].Data_Points[DP_Ptr].x,
								Data_Point_Set[Data_Point_Set_Id].Data_Points[DP_Ptr].y,

								Data_Point_Set[Data_Point_Set_Id].Data_Points[DP_Ptr].z,

								Data_Points_Filling_Threshold_mm );

	} // if dset in range
}

//---------------------------------------------------------------------------

void Surface_Class::set_phase_value_of_map_to_specific_time_point(long TimePoint_ptr)
{
	int Map_Ptr = Map_Values.get_value_ptr("Instantaneous phase");
	int dset = Current_Data_Point_Set_Ptr;

	for(long i=0;i<(signed)Data_Point_Set[dset].Data_Points.size();i++)
	if( TimePoint_ptr >= 0 && TimePoint_ptr <
		Data_Point_Set[dset].Data_Points[i].Roving_Signal.Phase.size() )
	{

	Data_Point_Set[dset].Data_Points[i].set_value(
		"Instantaneous phase",
		Data_Point_Set[dset].Data_Points[i].Roving_Signal.Phase[TimePoint_ptr],
		Map_Values.get_values_table_ref() );
	}

	// transfer values from dps to surface nodes
	long Closest_DP_Ptr;
	if( Surface_Node_Set.size() != Data_Point_Set[dset].Data_Points.size())
	project_data_points_specific_value(dset,Map_Ptr);
	else
	for(long j=0;j<(signed)Surface_Node_Set.size();j++)
	{
		Closest_DP_Ptr = Surface_Node_Set[j].Closest_Data_Point_Id;
		if( Closest_DP_Ptr > 0 )
		Surface_Node_Set[j].set_value(dset,Map_Ptr,
			Data_Point_Set[dset].Data_Points[Closest_DP_Ptr].
					get_value("Instantaneous phase",Map_Values.get_values_table_ref()) );
	}
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

std::vector <long> Surface_Class::get_histogram_of_value_gradient(
	int Data_Point_Set_Ptr,int Segment_Id,double Neighb_Range_mm, AnsiString Value_Name,
	double* Min, double* Box_Size,bool Only_DPs_With_Specified_Flag_A,int Flag_A_Value)
{
	std::vector <double> Gradient_Values;
	std::vector <double> DP_Values_Within_Neighb;
	double dist,v, MinV, MaxV;
	long Min_Ptr, Max_Ptr,T_Ptr;
	double RangeSquared = Neighb_Range_mm*Neighb_Range_mm;
	std::vector <long> Result_Histo;
	bool Segment_Present;

	for(long dp1=0;dp1<(signed)Data_Point_Set[Data_Point_Set_Ptr].Data_Points.size();dp1++)
	if( !Only_DPs_With_Specified_Flag_A ||
		(Only_DPs_With_Specified_Flag_A && Data_Point_Set[Data_Point_Set_Ptr].
			Data_Points[dp1].Flag_A == Flag_A_Value) )
	if( Data_Point_Set[Data_Point_Set_Ptr].Data_Points[dp1].Closest_Node_Id >= 0)
	if( Segment_Id <= 0 ||
		(Segment_Id>0 && is_segment_present_at_data_point(Data_Point_Set_Ptr,dp1,Segment_Id) ) )
	{

	DP_Values_Within_Neighb.clear();

	for(long dp2=0;dp2<(signed)Data_Point_Set[Data_Point_Set_Ptr].Data_Points.size();dp2++)
	if(dp1!=dp2)
	{

	dist = 	    std::pow( Data_Point_Set[Data_Point_Set_Ptr].Data_Points[dp1].x-
						  Data_Point_Set[Data_Point_Set_Ptr].Data_Points[dp2].x ,2)+
				std::pow( Data_Point_Set[Data_Point_Set_Ptr].Data_Points[dp1].y-
						  Data_Point_Set[Data_Point_Set_Ptr].Data_Points[dp2].y ,2)+
				std::pow( Data_Point_Set[Data_Point_Set_Ptr].Data_Points[dp1].z-
						  Data_Point_Set[Data_Point_Set_Ptr].Data_Points[dp2].z ,2);

	if( dist < RangeSquared )
	{

	v = Data_Point_Set[Data_Point_Set_Ptr].Data_Points[dp2].get_value(Value_Name,Map_Values.get_values_table_ref());
	if( v!= NOT_POSSIBLE_TO_CALCULATE_VALUE )
	DP_Values_Within_Neighb.push_back(v);

	}
	}

	if( DP_Values_Within_Neighb.size() >= 4 )
	{
		PNUM.find_min_max(&DP_Values_Within_Neighb,&MinV, &MaxV, &Min_Ptr, &Max_Ptr);
		Gradient_Values.push_back(MaxV-MinV);
	}

	}

	PNUM.find_min_max(&Gradient_Values,&MinV, &MaxV, &Min_Ptr, &Max_Ptr);
	Min[0] = MinV;

	if( Gradient_Values.size() < 2 )
	ShowMessage("Emtpy values vec e3349");

	if( Gradient_Values.size() > 1 )
	return PNUM.get_histogram(&Gradient_Values,0,true, Box_Size);
	else
	return Result_Histo;
}

//---------------------------------------------------------------------------

int Surface_Class::smooth_normals(double Range)
{
	double mx,my,mz,Counter;

	for(long i=0; i<(signed)
			Surface_Node_Set.size();i++)
	{

	mx = 0; my = 0; mz = 0; Counter = 0;

	for(long j=0; j<(signed)Surface_Node_Set.size();j++)
	if( fabs( Surface_Node_Set[j].x - Surface_Node_Set[i].x) < Range )
	if( fabs( Surface_Node_Set[j].y - Surface_Node_Set[i].y) < Range )
	if( fabs( Surface_Node_Set[j].z - Surface_Node_Set[i].z) < Range )
	{
		mx += Surface_Node_Set[j].normal[0];
		my += Surface_Node_Set[j].normal[1];
		mz += Surface_Node_Set[j].normal[2];
		Counter++;
	}

	if( Counter != 0 )
	{
	Surface_Node_Set[i].tmpx = mx/Counter;
	Surface_Node_Set[i].tmpy = my/Counter;
	Surface_Node_Set[i].tmpz = mz/Counter;

	}
	} // go thourgh every geo node

	for(long i=0; i<(signed)Surface_Node_Set.size();i++)
	{
	Surface_Node_Set[i].normal[0] = Surface_Node_Set[i].tmpx;
	Surface_Node_Set[i].normal[1] = Surface_Node_Set[i].tmpy;
	Surface_Node_Set[i].normal[2] = Surface_Node_Set[i].tmpz;
	}

	// transfer to triangles
	long n1,n2,n3;
	for(unsigned long i=0;i<Surface_Triangle_Set.size();i++)
	{
		n1 = Surface_Triangle_Set[i].Nodes[0];
		n2 = Surface_Triangle_Set[i].Nodes[1];
		n3 = Surface_Triangle_Set[i].Nodes[2];

		Surface_Triangle_Set[i].normal[0] = ( Surface_Node_Set[n1].normal[0] + Surface_Node_Set[n1].normal[0] + Surface_Node_Set[n1].normal[0] ) / 3.0;
		Surface_Triangle_Set[i].normal[1] = ( Surface_Node_Set[n2].normal[1] + Surface_Node_Set[n2].normal[1] + Surface_Node_Set[n2].normal[1] ) / 3.0;
		Surface_Triangle_Set[i].normal[2] = ( Surface_Node_Set[n3].normal[2] + Surface_Node_Set[n3].normal[2] + Surface_Node_Set[n3].normal[2] ) / 3.0;
	}

}

//--------------------------------------------------------------------------

void Surface_Class::get_nodes_ptrs_with_minmax_value(int DSet, int Val_Ptr, long* Min_Node_Ptr, long* Max_Node_Ptr )
{
	double v,minv=10000000,maxv=-10000000;
	long min_ptr=-1,max_ptr=-1;

	for(unsigned long i=0; i<Surface_Node_Set.size(); i++)
	if( !Surface_Node_Set[i].Removed_Geometry_Vertex_Flag)
	if( node_values_valid_specific_value(i,Val_Ptr) )
	{

	v = Surface_Node_Set[i].get_value(DSet,Val_Ptr);

	if( v != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{
		 if( v > maxv )
		 {
			 maxv = v;
			 max_ptr = i;
		 }

		 if( v < minv )
		 {
			 minv = v;
			 min_ptr = i;
		 }

	}
	}

	Min_Node_Ptr[0] = min_ptr;
	Max_Node_Ptr[0] = max_ptr;
}

//---------------------------------------------------------------------------

void Surface_Class::get_segment_id_with_minmax_value(int DSet, int Val_Ptr, int* Min_Segment_Id, int* Max_Segment_Id )
{
	double v,v1,v2,v3,minv=10000000,maxv=-10000000;
	long min_ptr=-1,max_ptr=-1;

	for(unsigned long i=0; i<Surface_Triangle_Set.size(); i++)
	if( Surface_Triangle_Set[i].Removed_Flag != ON )
	if( triangle_values_valid_specific_value(i,Val_Ptr) )
	{

	v1 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[0]].get_value(DSet,Val_Ptr);
	v2 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[1]].get_value(DSet,Val_Ptr);
	v3 = Surface_Node_Set[Surface_Triangle_Set[i].Nodes[2]].get_value(DSet,Val_Ptr);

	v = ( v1+v2+v3 )/3.0;

	if( v1 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v2 != NOT_POSSIBLE_TO_CALCULATE_VALUE &&
		v3 != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{
		 if( v > maxv )
		 {
			 maxv = v;
			 max_ptr = i;
		 }

		 if( v < minv )
		 {
			 minv = v;
			 min_ptr = i;
		 }

	}
	}

	Min_Segment_Id[0] = Surface_Triangle_Set[min_ptr].Segment_Id;
	Max_Segment_Id[0] = Surface_Triangle_Set[max_ptr].Segment_Id;
}

//---------------------------------------------------------------------------

void Surface_Class::segment_disjonted_elements()
{
	int Segment_Id = 0;

	//------------------------------------------
	// 1. Clear segments
	//------------------------------------------
	for(long t=0;t<(signed)Surface_Triangle_Set.size();t++)
		Surface_Triangle_Set[t].Segment_Id = -1;

	//------------------------------------------
	// 2. Perform watershed procedures untill everthing is segmented
	//------------------------------------------
	bool sth_done = true;

	while(sth_done)
	{

	sth_done = false;

	Segment_Id++;

	for(long t=0;t<(signed)Surface_Triangle_Set.size();t++)
	if( !sth_done && Surface_Triangle_Set[t].Segment_Id < 0 )
	{

	// set seed
	Surface_Triangle_Set[t].Segment_Id = Segment_Id;

	// expand
	while( segmentation_expand_segment_one_step(Segment_Id,true) )
	{
		Progress_Form->add_text("Expanding triangle: " + FloatToStr(t));
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	sth_done = true;

	}
		Progress_Form->Hide();
	}
}

//---------------------------------------------------------------------------

long Surface_Class::node_xyz_present(std::vector<Geometry_Vertex> *Node_Set,double x, double y, double z)
{
	long node_ptr = -1;

	for(long n=0;n<(signed)Node_Set[0].size();n++)
	if( Node_Set[0][n].x == x && Node_Set[0][n].y == y && Node_Set[0][n].z == z )
	node_ptr = n;

	return node_ptr;
}

//---------------------------------------------------------------------------

void Surface_Class::remove_surface_related_with_specific_segment(int Segment_Id)
{
	long node_ptr,ptr;

	std::vector<Geometry_Vertex> New_Surface_Node_Set;
	std::vector<Geometry_Triangle> New_Surface_Triangle_Set;

	for(long t=0;t<(signed)Surface_Triangle_Set.size();t++)
	if( Surface_Triangle_Set[t].Segment_Id == Segment_Id )
	{
		New_Surface_Triangle_Set.push_back(Surface_Triangle_Set[t]);
		ptr = New_Surface_Triangle_Set.size()-1;

		for(int k=0;k<3;k++)
		{
		node_ptr = node_xyz_present(
			&New_Surface_Node_Set,
			Surface_Node_Set[New_Surface_Triangle_Set[ptr].Nodes[k]].x,
			Surface_Node_Set[New_Surface_Triangle_Set[ptr].Nodes[k]].y,
			Surface_Node_Set[New_Surface_Triangle_Set[ptr].Nodes[k]].z);

		if( node_ptr < 0 )
		{
			New_Surface_Node_Set.push_back(Surface_Node_Set[
				New_Surface_Triangle_Set[ptr].Nodes[k]]);
			node_ptr = New_Surface_Node_Set.size()-1;
		}

		New_Surface_Triangle_Set[ptr].Nodes[k] = node_ptr;

		}

	}

	Surface_Node_Set = New_Surface_Node_Set;
	Surface_Triangle_Set = New_Surface_Triangle_Set;

	// remember to call initialize_surface() in parent function (to calculate neigs etc.)
}

//---------------------------------------------------------------------------

long Surface_Class::delete_floating_data_points(int DSet_Ptr, double Distance_Th,bool Hard_Delete)
{
	double dist;
	long node_ptr;

	if(Hard_Delete)
	{

	long old_number = Data_Point_Set[DSet_Ptr].Data_Points.size();

	for(unsigned long i=0;i<Data_Point_Set[DSet_Ptr].Data_Points.size();i++)
	{
		node_ptr = get_closest_node_to_xyz(
			Data_Point_Set[DSet_Ptr].Data_Points[i].x,
			Data_Point_Set[DSet_Ptr].Data_Points[i].y,
			Data_Point_Set[DSet_Ptr].Data_Points[i].z,Distance_Th);

	if( node_ptr < 0 )
	{
		Data_Point_Set[DSet_Ptr].Data_Points.erase(Data_Point_Set[DSet_Ptr].Data_Points.begin() + i );
		i--;
	}

	}

	long new_number = Data_Point_Set[DSet_Ptr].Data_Points.size();

	return old_number - new_number;

	}
	else
	ShowMessage("Soft delete not implemented yet");
}

//---------------------------------------------------------------------------

double Surface_Class::get_path_value_difference(long Node_1,long Node_2)
{
	double val=0;
	int Val_Ptr = Map_Values.get_current_value_ptr();

	if(data_points_set_ptr_in_range())
	{

	double v1 = Surface_Node_Set[Node_1].get_value(Current_Data_Point_Set_Ptr,Val_Ptr);
	double v2 = Surface_Node_Set[Node_2].get_value(Current_Data_Point_Set_Ptr,Val_Ptr);

	if(v1!= NOT_POSSIBLE_TO_CALCULATE_VALUE)
	if(v2!= NOT_POSSIBLE_TO_CALCULATE_VALUE)
	val = v2-v1;

	}
	return val;
}

//---------------------------------------------------------------------------


