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

#include "STUDY_Class.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

STUDY_Class::STUDY_Class()
{
	Current_Surface = -1;
}

//---------------------------------------------------------------------------------

STUDY_Class::~STUDY_Class()
{
	//
}

//---------------------------------------------------------------------------------

int STUDY_Class::save_study(AnsiString StudyPathandName,TProgress_Form *Progress_Form)
{
	// check if there is enough space on a drive
	Study_Current_Path_And_File_Name = StudyPathandName;

	ofstream Study_File;
	Study_File.open(StudyPathandName.c_str(),ios::out|ios::binary);

	if( Study_File.is_open() == false )
	return -1;

	//-------------------------------------------------------------
	// save signature of the file
	//-------------------------------------------------------------
	double Signature = 5555.7777111; // signature of free-source version
	Study_File.write((char*)&Signature, sizeof (double));

	//-------------------------------------------------------------
	// save version of the file
	//-------------------------------------------------------------
	int version = 3;
	Study_File.write((char*)&version, sizeof (int));

	//-------------------------------------------------------------
	// save data
	//-------------------------------------------------------------
	Utils.save_String_to_File(&Study_File, Study_Source_Path);
	Utils.save_String_to_File(&Study_File, Study_Current_Path_And_File_Name);

	// save surfaces
	int Items_Number = Surfaces_List.size();
	Study_File.write((char*)&Items_Number, sizeof (int));
	for(int i=0;i<Items_Number;i++)
	{
		Surfaces_List[i].save_object_to_stream(&Study_File,Progress_Form);
		Progress_Form->add_text("Surface " + IntToStr(i) + " saved.");
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	Comp_Module.save_object_to_stream(&Study_File);

	Study_File.close();

	return 1;
}

//---------------------------------------------------------------------------

int STUDY_Class::load_study(AnsiString StudyPathandName,TProgress_Form *Progress_Form)
{
	int Items_Number,Size;
	ifstream Study_File;
	int Result;

	// check if there is enough space on a drive

	Study_File.open(StudyPathandName.c_str(),ios::in|ios::binary);

	if(	Study_File.is_open() == false )
	return -1;

	//-------------------------------------------------------------
	// load signature of the file
	//-------------------------------------------------------------
	double Signature;
	Study_File.read((char*)&Signature, sizeof (double));

	if( Signature != 5555.77771 )
	return -2;

	Progress_Form->add_text("File signature verified...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//-------------------------------------------------------------
	// load version of the file
	//-------------------------------------------------------------
	int version;
	Study_File.read((char*)&version, sizeof (int));

	// update study name
	Study_Current_Path_And_File_Name = StudyPathandName;

	//-------------------------------------------------------------
	if( version == 3 )
	//-------------------------------------------------------------
	{

	Progress_Form->add_text("File version: " + IntToStr(version));
	Progress_Form->Show();
	Application->ProcessMessages();

	Study_Source_Path = Utils.load_String_from_File(&Study_File);
	Study_Current_Path_And_File_Name = Utils.load_String_from_File(&Study_File);

	// load surfaces
	Study_File.read((char*)&Items_Number, sizeof (int));
	Surface_Class Suface_Class_item;
	Surfaces_List.clear();
	Surfaces_List.assign(Items_Number,Suface_Class_item);
	for(int i=0;i<Items_Number;i++)
	{
		Progress_Form->add_text("Loading surface " + IntToStr(i+1) + "/" + IntToStr(Items_Number));
		Progress_Form->Show();
		Application->ProcessMessages();
		Surfaces_List[i].load_object_from_stream(&Study_File,Progress_Form);
	}

	Comp_Module.load_object_from_stream(&Study_File);

	return 1;

	} // ver3

	//-------------------------------------------------------------
	if( version == 2 )
	//-------------------------------------------------------------
	{

	Progress_Form->add_text("File version: " + IntToStr(version));
	Progress_Form->Show();
	Application->ProcessMessages();

	Study_Current_Path_And_File_Name = Utils.load_String_from_File(&Study_File);

	// load surfaces
	Study_File.read((char*)&Items_Number, sizeof (int));
	Surface_Class Suface_Class_item;
	Surfaces_List.clear();
	Surfaces_List.assign(Items_Number,Suface_Class_item);
	for(int i=0;i<Items_Number;i++)
	{
		Progress_Form->add_text("Loading surface " + IntToStr(i+1) + "/" + IntToStr(Items_Number));
		Progress_Form->Show();
		Application->ProcessMessages();
		Surfaces_List[i].load_object_from_stream(&Study_File,Progress_Form);
	}

	Comp_Module.load_object_from_stream(&Study_File);

	return 1;

	} // ver2


	//-------------------------------------------------------------
	if( version == 1 )
	//-------------------------------------------------------------
	{

	Progress_Form->add_text("File version: " + IntToStr(version));
	Progress_Form->Show();
	Application->ProcessMessages();

	Study_Current_Path_And_File_Name = Utils.load_String_from_File(&Study_File);

	// load surfaces
	Study_File.read((char*)&Items_Number, sizeof (int));
	Surface_Class Suface_Class_item;
	Surfaces_List.clear();
	Surfaces_List.assign(Items_Number,Suface_Class_item);
	for(int i=0;i<Items_Number;i++)
	{
		Progress_Form->add_text("Loading surface " + IntToStr(i+1) + "/" + IntToStr(Items_Number));
		Progress_Form->Show();
		Application->ProcessMessages();
		Surfaces_List[i].load_object_from_stream(&Study_File,Progress_Form);
	}

	return 1;

	} // ver1

	Application->ProcessMessages();

	Study_File.close();

	return -3;
}

//---------------------------------------------------------------------------

int STUDY_Class::load_study_append2nd_one(AnsiString StudyPathandName,TProgress_Form *Progress_Form)
{
	int Items_Number,Size;
	ifstream Study_File;
	int Result;

	// check if there is enough space on a drive

	Study_File.open(StudyPathandName.c_str(),ios::in|ios::binary);

	if(	Study_File.is_open() == false )
	return -1;

	//-------------------------------------------------------------
	// load signature of the file
	//-------------------------------------------------------------
	double Signature;
	Study_File.read((char*)&Signature, sizeof (double));

	if( Signature != 5555.77771 )
	return -2;

	Progress_Form->add_text("File signature verified...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//-------------------------------------------------------------
	// load version of the file
	//-------------------------------------------------------------
	int version;
	Study_File.read((char*)&version, sizeof (int));

	// update study name
	Study_Current_Path_And_File_Name = StudyPathandName;

	//-------------------------------------------------------------
	if( version == 3 )
	//-------------------------------------------------------------
	{

	Progress_Form->add_text("File version: " + IntToStr(version));
	Progress_Form->Show();
	Application->ProcessMessages();

	Study_Source_Path = Utils.load_String_from_File(&Study_File);
	Study_Current_Path_And_File_Name = Utils.load_String_from_File(&Study_File);

	// load surfaces
	Progress_Form->add_text("Loading surface");

	Study_File.read((char*)&Items_Number, sizeof (int));
    // assumption here: just 1 surface present
	Surface_Class Suface_Class_item;
	Surfaces_List.push_back(Suface_Class_item);
	Progress_Form->Show();
	Application->ProcessMessages();
	Surfaces_List[1].load_object_from_stream(&Study_File,Progress_Form);

	Comp_Module.load_object_from_stream(&Study_File);

	return 1;

	} // ver3

	Application->ProcessMessages();

	Study_File.close();

	return -3;
}

//---------------------------------------------------------------------------

void STUDY_Class::get_coordinates_of_nearest_surface_point(
												  double original_x,
												  double original_y,
												  double original_z,
												  double* Nearest_x,
												  double* Nearest_y,
												  double* Nearest_z )
{
    double distance, min_distance = MAXDOUBLE;
	double x,y,z;

	for(unsigned int S=0;S<Surfaces_List.size();S++)
    for(unsigned int i=0;i<Surfaces_List[S].Surface_Node_Set.size();i++)
	{
		distance = std::pow(original_x-Surfaces_List[S].Surface_Node_Set[i].x,2)+
				   std::pow(original_y-Surfaces_List[S].Surface_Node_Set[i].y,2)+
				   std::pow(original_z-Surfaces_List[S].Surface_Node_Set[i].z,2);

		if( distance < min_distance )
		{
			min_distance = distance;
			x = Surfaces_List[S].Surface_Node_Set[i].x;
			y = Surfaces_List[S].Surface_Node_Set[i].y;
			z = Surfaces_List[S].Surface_Node_Set[i].z;
		}
	}

	if( min_distance < MAXDOUBLE )
	{
		Nearest_x[0] = x;
		Nearest_y[0] = y;
		Nearest_z[0] = z;
	}

}
//---------------------------------------------------------------------------------

bool STUDY_Class::is_current_surface_in_range()
{
	if( Current_Surface >= 0 && Surfaces_List.size() > 0 &&
        Current_Surface < Surfaces_List.size() )
    return true;
	else
	{
		// ShowMessage("No geometry selected/loaded");
		return false;
	}
}

//---------------------------------------------------------------------------

bool STUDY_Class::is_valid_data_point_selected()
{
    if( is_current_surface_in_range() )
	if( Surfaces_List[Current_Surface].data_point_ptr_in_range() )
	return true;

    return false;
}

//---------------------------------------------------------------------------

bool STUDY_Class::is_valid_data_point_set_selected()
{
    if( is_current_surface_in_range() )
    if( Surfaces_List[Current_Surface].data_points_set_ptr_in_range() )
    return true;

	return false;
}

//---------------------------------------------------------------------------
bool STUDY_Class::is_valid_current_value_selected()
{
    if( is_current_surface_in_range() )
	if( is_valid_data_point_set_selected() )
    if( Surfaces_List[Current_Surface].Map_Values.get_current_value_ptr() >= 0 &&
        Surfaces_List[Current_Surface].Map_Values.get_current_value_ptr() <
			Surfaces_List[Current_Surface].Map_Values.get_values_number() )
     return true;

	return false;
}
//---------------------------------------------------------------------------

int STUDY_Class::get_current_surface_ptr()
{
	if( Current_Surface < Surfaces_List.size() )
	return Current_Surface;
	else
	{
		ShowMessage("Current_Surface out of range in STUDY module. Set to 0.");
		Current_Surface = 0;
		return Current_Surface;
	}
}
//---------------------------------------------------------------------------

void STUDY_Class::perform_17_nuclear_segmentation(TProgress_Form *Progress_Form,int Source_Surface,
		Segments_List_Class* Segments_Info)
{
	Progress_Form->add_text("Transforms currently selected value to polar plot using three segments: anterior,lateral and septal");
	Progress_Form->Show();
	Application->ProcessMessages();

	//---------------------------------------
	// 1. Generate wheel geometry as second geometry
	//---------------------------------------
	Progress_Form->add_text("Generating supporting structures...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Surface_Class Suface_Class_Item;
	Surfaces_List.push_back(Suface_Class_Item);
	int Wheel_Surf = Surfaces_List.size()-1; // *********************
	double Radius = 50; // assuming plaque is 100x100
	Surfaces_List[Wheel_Surf].generate_wheel_sample(Progress_Form,Radius,1);

	//---------------------------------------
	// 2.Mark 17 segments on wheel geometry
	//---------------------------------------
	Progress_Form->add_text("Marking 17 segments on auxiliary...");
	Progress_Form->Show();
	Application->ProcessMessages();

	double Shift = -0.15;

	// inner ring
	double if1 = M_PI/4. +  Shift;
	double if2 = 3.*M_PI/4.+ Shift;
	double if3 = -M_PI/4.+ Shift;
	double if4 = -3.*M_PI/4.+ Shift;

	// outer rings
	double f1 = 0 + Shift;
	double f2 = M_PI/3.+ Shift;
	double f3 = 2*M_PI/3.+ Shift;
	double f4 = M_PI + Shift;
	double f5 = -2*M_PI/3.+ Shift;
	double f6 = -M_PI/3.+ Shift;

	double R1 = 10;
	double R2 = R1 + 15;
	double R3 = R2 + 12.5;

	double cx,cy,cz;
	double R,fi;
	int Seg;

	for(long t=0;t<(signed)Surfaces_List[Wheel_Surf].Surface_Triangle_Set.size();t++)
	{
		Surfaces_List[Wheel_Surf].get_xyz_centre_of_triangle(t,&cx,&cy,&cz);

		// find R and fi
		// !!!!!!!!!!!!!!! ASSUMING GEO IS CENTERED !!!!!!!!!!!!!!!!

		R = std::sqrt( std::pow(cx,2)+std::pow(cy,2)+std::pow(cz,2) );
		fi = atan2(cz,cx);

		Seg = -1;

		// centre
		if( R < R1 ) Seg = Segments_Info->get_segment_id("S17");

		// first outer ring
		if( R > R1 && R < R2 &&  fi > if3 && fi < if1 ) Seg = Segments_Info->get_segment_id("S14");
		if( R > R1 && R < R2 &&  fi > if1 && fi < if2 ) Seg = Segments_Info->get_segment_id("S13");
		if( R > R1 && R < R2 &&  ( fi > if2 || fi < if4)  ) Seg = Segments_Info->get_segment_id("S16");
		if( R > R1 && R < R2 &&  fi > if4 && fi < if3 ) Seg = Segments_Info->get_segment_id("S15");

		// second outer ring
		if( R > R2 && R < R3 &&  fi > f1 && fi < f2 ) Seg = Segments_Info->get_segment_id("S8");
		if( R > R2 && R < R3 &&  fi > f2 && fi < f3 ) Seg = Segments_Info->get_segment_id("S7");
		if( R > R2 && R < R3 &&  fi > f3 && fi < f4 ) Seg = Segments_Info->get_segment_id("S12");
		if( R > R2 && R < R3 &&  fi < f1 && fi > f6 ) Seg = Segments_Info->get_segment_id("S9");
		if( R > R2 && R < R3 &&  fi < f6 && fi > f5 ) Seg = Segments_Info->get_segment_id("S10");
		if( R > R2 && R < R3 &&  fi < f5 ||
			R > R2 && R < R3 &&  fi > f4 ) Seg = Segments_Info->get_segment_id("S11");

		// third outer ring
		if( R > R3 &&  fi > f1 && fi < f2 ) Seg = Segments_Info->get_segment_id("S2");
		if( R > R3 &&  fi > f2 && fi < f3 ) Seg = Segments_Info->get_segment_id("S1");
		if( R > R3 &&  fi > f3 && fi < f4 ) Seg = Segments_Info->get_segment_id("S6");
		if( R > R3 &&  fi < f1 && fi > f6 ) Seg = Segments_Info->get_segment_id("S3");
		if( R > R3 &&  fi < f6 && fi > f5 ) Seg = Segments_Info->get_segment_id("S4");
		if( R > R3 &&  fi < f5 ||
			R > R3 &&  fi > f4) Seg = Segments_Info->get_segment_id("S5");

		Surfaces_List[Wheel_Surf].Surface_Triangle_Set[t].Segment_Id = Seg;
	}

	//---------------------------------------
	// 3. Propagate segments back
	//---------------------------------------
	Progress_Form->add_text("Marking segments on original geometry...");
	Progress_Form->Show();
	Application->ProcessMessages();

	long P1,P2;
	vector <long> Path_Left;
	vector <long> Path_Right;
	vector <long> Path_Base;
	double Phi_Offset;
	long Node_Id,nt;
	int Region_Id;

	// zero Flag_A and B
	for(long Node_Id=0;Node_Id<(signed)Surfaces_List[Source_Surface].Surface_Node_Set.size();Node_Id++)
	{
		Surfaces_List[Source_Surface].Surface_Node_Set[Node_Id].Flag_A = -1;
		Surfaces_List[Source_Surface].Surface_Node_Set[Node_Id].Flag_B = -1;
	}

	//------------------------------------------------------
	// Go through all nodes in 3D map within segments
	//------------------------------------------------------
	int Anterior_Region_Id = Segments_Info->get_segment_id(LV_ANTERIOR_SEGMENT_NAME);
	int Lateral_Region_Id = Segments_Info->get_segment_id(LV_LATERAL_SEGMENT_NAME);
	int Septal_Region_Id = Segments_Info->get_segment_id(LV_SEPTAL_SEGMENT_NAME);

	double Apex_Dist,d1,d2,D1,dp_to_base,dp_left,dp_right,Phi,new_x,new_y,new_z;
	long closest_node,closest_triangle;

	//-----------------------------------------------------------------------------------------
	for(long Node_Id=0;Node_Id<(signed)Surfaces_List[Source_Surface].Surface_Node_Set.size();Node_Id++)
	//-----------------------------------------------------------------------------------------
	{
		if( Node_Id % 50 == 1 )
		{
		Progress_Form->replace_last_line_with_text("Processing node: "+FloatToStr(Node_Id)+"/"+
			FloatToStr(Surfaces_List[Source_Surface].Surface_Node_Set.size()));
		Progress_Form->Show();
		Application->ProcessMessages();
		}

	Region_Id = Surfaces_List[Source_Surface].Surface_Triangle_Set[
		Surfaces_List[Source_Surface].
			Surface_Node_Set[Node_Id].Neig_Triangles[0] ].Segment_Id;

	if( Region_Id == Anterior_Region_Id || Region_Id == Septal_Region_Id || Region_Id == Lateral_Region_Id  )  // > 0 later
	{

	if( Region_Id == Anterior_Region_Id  ) // anterior
	{
		P1 = Surfaces_List[Source_Surface].Septal_Anterior_Node_Ptr;
		P2 = Surfaces_List[Source_Surface].Anterior_Lateral_Node_Ptr;
		Path_Left =  Surfaces_List[Source_Surface].Ap_SA_Path;
		Path_Right = Surfaces_List[Source_Surface].Ap_AL_Path;
		Path_Base = Surfaces_List[Source_Surface].Anterior_Base_Path;
		Phi_Offset = 0;
	}

	if( Region_Id == Lateral_Region_Id  ) // lateral
	{
		P1 = Surfaces_List[Source_Surface].Anterior_Lateral_Node_Ptr;
		P2 = Surfaces_List[Source_Surface].Septal_Lateral_Node_Ptr;
		Path_Left =  Surfaces_List[Source_Surface].Ap_AL_Path;
		Path_Right = Surfaces_List[Source_Surface].Ap_SL_Path;
		Path_Base = Surfaces_List[Source_Surface].Lateral_Base_Path;
		Phi_Offset = 2.*M_PI/3.;
	}

	if( Region_Id == Septal_Region_Id  ) // septal
	{
		P1 = Surfaces_List[Source_Surface].Septal_Lateral_Node_Ptr;
		P2 = Surfaces_List[Source_Surface].Septal_Anterior_Node_Ptr;
		Path_Left =  Surfaces_List[Source_Surface].Ap_SL_Path;
		Path_Right = Surfaces_List[Source_Surface].Ap_SA_Path;
		Path_Base = Surfaces_List[Source_Surface].Septal_Base_Path;
		Phi_Offset = 2.*2.*M_PI/3. ;
	}

	//------------------------------------------------------
	//  2. Calculate normalized distance from apex
	//------------------------------------------------------
	Apex_Dist = Surfaces_List[Source_Surface].get_path_length(
		Node_Id,Surfaces_List[Source_Surface].Apex_Node_Ptr);

	d1 = Surfaces_List[Source_Surface].get_path_length(
				P1,Surfaces_List[Source_Surface].Apex_Node_Ptr);
	d2 = Surfaces_List[Source_Surface].get_path_length(
				P2,Surfaces_List[Source_Surface].Apex_Node_Ptr);
	if( d1+d2 != 0 )
		D1 = 2*Apex_Dist/(d1+d2);

	dp_to_base = Surfaces_List[Source_Surface].get_distance_to_path(Node_Id,&Path_Base);

	if( Apex_Dist+dp_to_base != 0 )
	D1 =  Apex_Dist / (Apex_Dist+dp_to_base);
	else
	ShowMessage("Apex_Dist+dp_to_base == 0 in 17segments marking.");

	//  3. Calculate phi
	dp_left = Surfaces_List[Source_Surface].get_distance_to_path(Node_Id,&Path_Left);
	dp_right = Surfaces_List[Source_Surface].get_distance_to_path(Node_Id,&Path_Right);

	if( dp_left+dp_right == 0 )
	1; // ShowMessage("dp_left+dp_right == 0 in PropagatesegemntsbackfromWheelto3DMap1Click() ");
	else
	Phi = Phi_Offset + 2.*M_PI/3. * dp_left / (dp_left+dp_right);

	//------------------------------------------------------
	// find xyz of the node in polar plot wheel
	//------------------------------------------------------
	new_x = Radius*D1*cos(Phi);
	new_y = 0;
	new_z = Radius*D1*sin(Phi);

	closest_node = Surfaces_List[Wheel_Surf].get_closest_node_to_xyz(new_x,new_y,new_z,1000);
	closest_triangle = Surfaces_List[Wheel_Surf].Surface_Node_Set[closest_node].Neig_Triangles[0];

	Surfaces_List[Source_Surface].Surface_Node_Set[Node_Id].Flag_B =
		Surfaces_List[Wheel_Surf].Surface_Triangle_Set[closest_triangle].Segment_Id;

	} // if node is in region

	} // through all nodes


	// assign found segments from nodes to triangles
	int count;
	for(long t=0;t<(signed)Surfaces_List[Source_Surface].Surface_Triangle_Set.size();t++)
	{
		// set not segmented as default
		Surfaces_List[Source_Surface].Surface_Triangle_Set[t].Segment_Id = -1;

		count=0;
		if( Surfaces_List[Source_Surface].Surface_Node_Set[
				Surfaces_List[Source_Surface].Surface_Triangle_Set[t].Nodes[0] ].Flag_B > 0 )
		count++;

		if( Surfaces_List[Source_Surface].Surface_Node_Set[
				Surfaces_List[Source_Surface].Surface_Triangle_Set[t].Nodes[1] ].Flag_B > 0 )
		count++;

		if( Surfaces_List[Source_Surface].Surface_Node_Set[
				Surfaces_List[Source_Surface].Surface_Triangle_Set[t].Nodes[2] ].Flag_B > 0 )
		count++;

		if(count >= 1)
		Surfaces_List[Source_Surface].Surface_Triangle_Set[t].Segment_Id =
			Surfaces_List[Source_Surface].Surface_Node_Set[
				Surfaces_List[Source_Surface].Surface_Triangle_Set[t].Nodes[0]].Flag_B;
	}

//	Surfaces_List[Wheel_Surf].Display_Flag = false;
	// !!!!!!!!!!!!1 moze tutaj skasuj wheel geometry????

	//---------------------------------------------------------------
	// smooth borders
	//---------------------------------------------------------------
	Progress_Form->add_text("Smoothing segmentation...");
	Progress_Form->Show();
	Application->ProcessMessages();

	// 1. Shrink one step
	for(int s=1;s<=17;s++)
	{
		Seg = Segments_Info->get_segment_id("S"+IntToStr(s));
		Surfaces_List[Current_Surface].
			segmentation_shrink_segment_one_step( Seg,false );
	}

	// 2. Grow one step
	for(int s=1;s<=17;s++)
	{
		Seg = Segments_Info->get_segment_id("S"+IntToStr(s));
		Surfaces_List[Current_Surface].
			segmentation_expand_segment_one_step( Seg,false );
	}

	// 3. Grow one step again to fill empty spots
	for(int s=1;s<=17;s++)
	{
		Seg = Segments_Info->get_segment_id("S"+IntToStr(s));
		Surfaces_List[Current_Surface].
			segmentation_expand_segment_one_step( Seg,true ); // block existing == true
	}

	Surfaces_List[Source_Surface].compute_segments_centers(Segments_Info->Segments.size());
	Surfaces_List[Wheel_Surf].compute_segments_centers(Segments_Info->Segments.size());

	// change map value to "segmentats"
	Surfaces_List[Source_Surface].Map_Values.set_current_value_according_to_name(SEGMENTATION_VALUE_NAME);

	//---------------------------------------
	// REMOVE WHEEL GEOMETRY
	//---------------------------------------
	Surfaces_List.erase(Surfaces_List.begin() + Wheel_Surf );
}
//---------------------------------------------------------------------------

AnsiString STUDY_Class::get_segment_name_at_given_node(int Geometry_Id,long Node_Id,
	Segments_List_Class* Segments_Info)
{
	if( is_current_surface_in_range() )
	{
	int Seg_Id = -1;

	long Neig_T = Surfaces_List[Current_Surface].Surface_Node_Set[Node_Id].Neig_Triangles[0];
	if( Neig_T >= 0 && Neig_T < Surfaces_List[Current_Surface].Surface_Triangle_Set.size() )
		Seg_Id = Surfaces_List[Current_Surface].Surface_Triangle_Set[Neig_T].Segment_Id;

	return Segments_Info->get_segment_name(Seg_Id);
	}
}
//---------------------------------------------------------------------------

int STUDY_Class::get_segment_id_at_given_node(int Geometry_Id,long Node_Id)
{
	if( is_current_surface_in_range() )
	{
	int Seg_Id = -1;

	long Neig_T = Surfaces_List[Current_Surface].Surface_Node_Set[Node_Id].Neig_Triangles[0];
	if( Neig_T >= 0 && Neig_T < Surfaces_List[Current_Surface].Surface_Triangle_Set.size() )
		Seg_Id = Surfaces_List[Current_Surface].Surface_Triangle_Set[Neig_T].Segment_Id;

	return Seg_Id;
	}
}

//---------------------------------------------------------------------------

void STUDY_Class::calculate_inter_chamber_coherence_map(int Base_Surface,int Target_Surface,
		int Base_Surface_DS,int Target_Surface_DS,double Base_AFCL_ms)
{
	if(   Base_Surface >= 0 && Base_Surface < Surfaces_List.size() &&
		Target_Surface >= 0 && Target_Surface < Surfaces_List.size() )
	if(   Base_Surface_DS >= 0 && Base_Surface_DS < Surfaces_List[Base_Surface].Data_Point_Set.size() &&
		Target_Surface_DS >= 0 && Target_Surface_DS < Surfaces_List[Base_Surface].Data_Point_Set.size() )
	{

	Progress_Form->add_text("Calculating Inter Chamber Coherence");
	Progress_Form->add_text("Calculating Inter Chamber Coherence");
	Progress_Form->Show();
	Application->ProcessMessages();

	int Surface1=Base_Surface;
	int Surface2=Target_Surface;
	int dset = 0; // at the moment ignore case when dpsets number > 1

	// add value to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS;
	Value_Desc.Displayed_In_Table = true;
	Value_Desc.Value_Name = "Inter-chamber coherence";
	Value_Desc.Unit = "";
	Surfaces_List[Surface1].Map_Values.add_value(Value_Desc);
	int Map_Ptr = Surfaces_List[Surface1].Map_Values.get_value_ptr(Value_Desc.Value_Name);

	double MPC,MPC_max;

	double Time_Step_ms = Surfaces_List[Base_Surface].Data_Point_Set[Base_Surface_DS].Data_Points[0].Roving_Signal.Time_Step_ms;

	for(long dp1=0;dp1<Surfaces_List[Surface1].Data_Point_Set[dset].Data_Points.size();dp1++)
	{

	MPC_max = 0;

	Progress_Form->replace_last_line_with_text(
		"Calculating interchamber coherence of geo:" +IntToStr(Surface1) + " Data point: " +
		IntToStr((int)dp1) + "/" + IntToStr((int)Surfaces_List[Surface1].
			Data_Point_Set[dset].Data_Points.size()));
	Progress_Form->Show();
	Application->ProcessMessages();

	for(long dp2=0;dp2<Surfaces_List[Surface2].Data_Point_Set[dset].Data_Points.size();dp2+=20)
	{

	MPC = PNUM.get_phase_synchronization(
					&Surfaces_List[Surface1].Data_Point_Set[dset].Data_Points[dp1].
					Roving_Signal.Phase,
					&Surfaces_List[Surface2].Data_Point_Set[dset].Data_Points[dp2].
					Roving_Signal.Phase,1,1,false,0);
	if( MPC > MPC_max )
	MPC_max = MPC;

	} // dp2 for

	Surfaces_List[Surface1].Data_Point_Set[dset].Data_Points[dp1].set_value(
			Value_Desc.Value_Name,MPC_max,
			Surfaces_List[Surface1].Map_Values.get_values_table_ref() );
	} // dp1 for

	} // if all in range
}

//---------------------------------------------------------------------------

int STUDY_Class::compute_min_max_values()
{
	double minv,maxv;
	Min_Value_On_All_Maps=1000000;
	Max_Value_On_All_Maps=-1000000;

	for(int S=0;S<Surfaces_List.size();S++)
	if( Surfaces_List[S].Display_Whole_Dataset_Flag )
	{
		Surfaces_List[S].compute_min_max_values_on_surface();
		Surfaces_List[S].Map_Values.get_current_value_minmax(&minv,&maxv);

		if( minv < Min_Value_On_All_Maps ) Min_Value_On_All_Maps = minv;
		if( maxv > Max_Value_On_All_Maps ) Max_Value_On_All_Maps = maxv;
	}
}

//---------------------------------------------------------------------------

void STUDY_Class::add_empty_surface(AnsiString Name)
{
	Surface_Class SS;
	SS.Name = Name;
	Surfaces_List.push_back(SS);

	int sptr = Surfaces_List.size()-1;

	Data_Point_Set_Class DPSet;
	DPSet.Name = "Base data points set";
	Surfaces_List[sptr].Data_Point_Set.push_back(DPSet);
}
//---------------------------------------------------------------------------

void STUDY_Class::post_study_load_processing()
{
	int dset;

	Current_Surface = 0;
	Surfaces_List[0].Display_Whole_Dataset_Flag = true;

	for(int S=0;S<Surfaces_List.size();S++)
	Surfaces_List[S].initialize_surface(Progress_Form);

	compute_min_max_values();
}

//---------------------------------------------------------------------------

void STUDY_Class::perform_one_step_of_triangles_fitting(
		int Surface_1,int Surface_2,   // surface 2 is being rotated/moved
		int Surface_3,bool Fit_3rd_along_wiht_2nd,
		int Surface_4,bool Fit_4th_along_wiht_2nd )
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

//     dcs: Surfaces_List[Surface_1].Registration_Point_A     : Intersection_Point_A
//     sns: Surfaces_List[Surface_2].Registration_Point_A     : Registration_Point_A (obracane)

// 1.
F1x = -Spring_Constant*(Surfaces_List[Surface_1].Registration_Point_A.x - Surfaces_List[Surface_2].Registration_Point_A.x);
F1y = -Spring_Constant*(Surfaces_List[Surface_1].Registration_Point_A.y - Surfaces_List[Surface_2].Registration_Point_A.y);
F1z = -Spring_Constant*(Surfaces_List[Surface_1].Registration_Point_A.z - Surfaces_List[Surface_2].Registration_Point_A.z);

F2x = -Spring_Constant*(Surfaces_List[Surface_1].Registration_Point_B.x - Surfaces_List[Surface_2].Registration_Point_B.x);
F2y = -Spring_Constant*(Surfaces_List[Surface_1].Registration_Point_B.y - Surfaces_List[Surface_2].Registration_Point_B.y);
F2z = -Spring_Constant*(Surfaces_List[Surface_1].Registration_Point_B.z - Surfaces_List[Surface_2].Registration_Point_B.z);

F3x = -Spring_Constant*(Surfaces_List[Surface_1].Registration_Point_C.x - Surfaces_List[Surface_2].Registration_Point_C.x);
F3y = -Spring_Constant*(Surfaces_List[Surface_1].Registration_Point_C.y - Surfaces_List[Surface_2].Registration_Point_C.y);
F3z = -Spring_Constant*(Surfaces_List[Surface_1].Registration_Point_C.z - Surfaces_List[Surface_2].Registration_Point_C.z);

// 2. Center registration points of [0] surface

Tx = (Surfaces_List[Surface_1].Registration_Point_A.x +
	  Surfaces_List[Surface_1].Registration_Point_B.x +
	  Surfaces_List[Surface_1].Registration_Point_C.x)/3.0;

Ty = (Surfaces_List[Surface_1].Registration_Point_A.y +
	  Surfaces_List[Surface_1].Registration_Point_B.y +
	  Surfaces_List[Surface_1].Registration_Point_C.y)/3.0;

Tz = (Surfaces_List[Surface_1].Registration_Point_A.z +
	  Surfaces_List[Surface_1].Registration_Point_B.z +
	  Surfaces_List[Surface_1].Registration_Point_C.z)/3.0;

		Surfaces_List[Surface_1].Registration_Point_A.x -= Tx;
		Surfaces_List[Surface_1].Registration_Point_A.y -= Ty;
		Surfaces_List[Surface_1].Registration_Point_A.z -= Tz;

		Surfaces_List[Surface_1].Registration_Point_B.x -= Tx;
		Surfaces_List[Surface_1].Registration_Point_B.y -= Ty;
		Surfaces_List[Surface_1].Registration_Point_B.z -= Tz;

		Surfaces_List[Surface_1].Registration_Point_C.x -= Tx;
		Surfaces_List[Surface_1].Registration_Point_C.y -= Ty;
		Surfaces_List[Surface_1].Registration_Point_C.z -= Tz;
/*
		for(i=0;i<(signed)Surfaces_List[Surface_1].Surface_Node_Set.size();i++)
		{
			Surfaces_List[Surface_1].Surface_Node_Set[i].x += Tx;
			Surfaces_List[Surface_1].Surface_Node_Set[i].y += Ty;
			Surfaces_List[Surface_1].Surface_Node_Set[i].z += Tz;
		}
*/

// 3. M = rxF

M1x = Surfaces_List[Surface_1].Registration_Point_A.y*F1z - Surfaces_List[Surface_1].Registration_Point_A.z*F1y;
M1y = Surfaces_List[Surface_1].Registration_Point_A.z*F1x - Surfaces_List[Surface_1].Registration_Point_A.x*F1z;
M1z = Surfaces_List[Surface_1].Registration_Point_A.x*F1y - Surfaces_List[Surface_1].Registration_Point_A.y*F1x;

M2x = Surfaces_List[Surface_1].Registration_Point_B.y*F2z - Surfaces_List[Surface_1].Registration_Point_B.z*F2y;
M2y = Surfaces_List[Surface_1].Registration_Point_B.z*F2x - Surfaces_List[Surface_1].Registration_Point_B.x*F2z;
M2z = Surfaces_List[Surface_1].Registration_Point_B.x*F2y - Surfaces_List[Surface_1].Registration_Point_B.y*F2x;

M3x = Surfaces_List[Surface_1].Registration_Point_C.y*F3z - Surfaces_List[Surface_1].Registration_Point_C.z*F3y;
M3y = Surfaces_List[Surface_1].Registration_Point_C.z*F3x - Surfaces_List[Surface_1].Registration_Point_C.x*F3z;
M3z = Surfaces_List[Surface_1].Registration_Point_C.x*F3y - Surfaces_List[Surface_1].Registration_Point_C.y*F3x;

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
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
// Now we have all rotations needed to calculate movement,
// so, lets do it! (FOR SURFACE_2)
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------

	//----------------------------------------------------------------
	// rotate registration points on 3D goemetry
	//----------------------------------------------------------------
	for(i=0;i<(signed)Surfaces_List[Surface_2].Surface_Node_Set.size();i++)
	{

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,-fi1,3);

	Surfaces_List[Surface_2].Surface_Node_Set[i].x -= dx_from_F;
	Surfaces_List[Surface_2].Surface_Node_Set[i].y -= dy_from_F;
	Surfaces_List[Surface_2].Surface_Node_Set[i].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// data points
	//----------------------------------------------------------------
	for(long dset=0;dset<(signed)Surfaces_List[Surface_2].Data_Point_Set.size();dset++)
	for(long d=0;d<(signed)Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points.size();d++)
	{
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,-fi1,3);

	Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x -= dx_from_F;
	Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y -= dy_from_F;
	Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// ablation points
	//----------------------------------------------------------------
	for(long d=0;d<(signed)Surfaces_List[Surface_2].Ablation_Points_List.size();d++)
	{
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].x,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].y,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].x,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].y,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].x,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].y,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].x,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].y,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].x,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].y,
				  &Surfaces_List[Surface_2].Ablation_Points_List[d].z,-fi1,3);

	Surfaces_List[Surface_2].Ablation_Points_List[d].x -= dx_from_F;
	Surfaces_List[Surface_2].Ablation_Points_List[d].y -= dy_from_F;
	Surfaces_List[Surface_2].Ablation_Points_List[d].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// rotate registration points on 3D goemetry
	//----------------------------------------------------------------
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,fi1,3);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,fi2,2);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,-fi1,3);

	Surfaces_List[Surface_2].Registration_Point_A.x -= dx_from_F;
	Surfaces_List[Surface_2].Registration_Point_A.y -= dy_from_F;
	Surfaces_List[Surface_2].Registration_Point_A.z -= dz_from_F;

	Surfaces_List[Surface_2].Registration_Point_B.x -= dx_from_F;
	Surfaces_List[Surface_2].Registration_Point_B.y -= dy_from_F;
	Surfaces_List[Surface_2].Registration_Point_B.z -= dz_from_F;

	Surfaces_List[Surface_2].Registration_Point_C.x -= dx_from_F;
	Surfaces_List[Surface_2].Registration_Point_C.y -= dy_from_F;
	Surfaces_List[Surface_2].Registration_Point_C.z -= dz_from_F;

	//-------------------------------------------------------------------------
	if(Fit_3rd_along_wiht_2nd) // do it also for surface_3 if requested
	//-------------------------------------------------------------------------
	{

	 for(i=0;i<(signed)Surfaces_List[Surface_3].Surface_Node_Set.size();i++)
	 {
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,-fi1,3);

	Surfaces_List[Surface_3].Surface_Node_Set[i].x -= dx_from_F;
	Surfaces_List[Surface_3].Surface_Node_Set[i].y -= dy_from_F;
	Surfaces_List[Surface_3].Surface_Node_Set[i].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// data points
	//----------------------------------------------------------------
	for(long dset=0;dset<(signed)Surfaces_List[Surface_3].Data_Point_Set.size();dset++)
	for(long d=0;d<(signed)Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points.size();d++)
	{
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,-fi1,3);

	Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x -= dx_from_F;
	Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y -= dy_from_F;
	Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// rotate registration points on 3D goemetry
	//----------------------------------------------------------------
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,fi1,3);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,fi2,2);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,-fi1,3);

	Surfaces_List[Surface_3].Registration_Point_A.x -= dx_from_F;
	Surfaces_List[Surface_3].Registration_Point_A.y -= dy_from_F;
	Surfaces_List[Surface_3].Registration_Point_A.z -= dz_from_F;

	Surfaces_List[Surface_3].Registration_Point_B.x -= dx_from_F;
	Surfaces_List[Surface_3].Registration_Point_B.y -= dy_from_F;
	Surfaces_List[Surface_3].Registration_Point_B.z -= dz_from_F;

	Surfaces_List[Surface_3].Registration_Point_C.x -= dx_from_F;
	Surfaces_List[Surface_3].Registration_Point_C.y -= dy_from_F;
	Surfaces_List[Surface_3].Registration_Point_C.z -= dz_from_F;

	}

	//-------------------------------------------------------------------------
	if(Fit_4th_along_wiht_2nd) // do it also for surface_3 if requested
	//-------------------------------------------------------------------------
	{

	 for(i=0;i<(signed)Surfaces_List[Surface_4].Surface_Node_Set.size();i++)
	 {
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,-fi1,3);

	Surfaces_List[Surface_4].Surface_Node_Set[i].x -= dx_from_F;
	Surfaces_List[Surface_4].Surface_Node_Set[i].y -= dy_from_F;
	Surfaces_List[Surface_4].Surface_Node_Set[i].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// data points
	//----------------------------------------------------------------
	for(long dset=0;dset<(signed)Surfaces_List[Surface_4].Data_Point_Set.size();dset++)
	for(long d=0;d<(signed)Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points.size();d++)
	{
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,-fi1,3);

	Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x -= dx_from_F;
	Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y -= dy_from_F;
	Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// rotate registration points on 3D goemetry
	//----------------------------------------------------------------
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,fi1,3);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,fi2,2);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,-fi1,3);

	Surfaces_List[Surface_4].Registration_Point_A.x -= dx_from_F;
	Surfaces_List[Surface_4].Registration_Point_A.y -= dy_from_F;
	Surfaces_List[Surface_4].Registration_Point_A.z -= dz_from_F;

	Surfaces_List[Surface_4].Registration_Point_B.x -= dx_from_F;
	Surfaces_List[Surface_4].Registration_Point_B.y -= dy_from_F;
	Surfaces_List[Surface_4].Registration_Point_B.z -= dz_from_F;

	Surfaces_List[Surface_4].Registration_Point_C.x -= dx_from_F;
	Surfaces_List[Surface_4].Registration_Point_C.y -= dy_from_F;
	Surfaces_List[Surface_4].Registration_Point_C.z -= dz_from_F;

	} // moving 4th geo as well

	// translate back map0
	Surfaces_List[Surface_1].Registration_Point_A.x += Tx;
	Surfaces_List[Surface_1].Registration_Point_A.y += Ty;
	Surfaces_List[Surface_1].Registration_Point_A.z += Tz;

	Surfaces_List[Surface_1].Registration_Point_B.x += Tx;
	Surfaces_List[Surface_1].Registration_Point_B.y += Ty;
	Surfaces_List[Surface_1].Registration_Point_B.z += Tz;

	Surfaces_List[Surface_1].Registration_Point_C.x += Tx;
	Surfaces_List[Surface_1].Registration_Point_C.y += Ty;
	Surfaces_List[Surface_1].Registration_Point_C.z += Tz;
}

//---------------------------------------------------------------------------------

void STUDY_Class::get_mean_distance_between_geometries(int Geometry_1, int Geometry_2,
	double* mean,double* SD,double* Median, double Max_Dist_Threshold)
{
	double dist;
	long nt,n1,n2,n3;
	std::vector <double> Dist_Vec;
	Dist_Vec.clear();

	if( Geometry_1 >=0 && Geometry_1 < Surfaces_List.size() )
	if( Geometry_2 >=0 && Geometry_2 < Surfaces_List.size() )
	{

	find_corresponding_nodes_in_2_geometries(Geometry_1,Geometry_2,Max_Dist_Threshold);

	for(long n=0;n<Surfaces_List[Geometry_1].Surface_Node_Set.size();n++)
	if( !Surfaces_List[Geometry_1].Surface_Node_Set[n].Removed_Geometry_Vertex_Flag )
	{

		long n2 = Surfaces_List[Geometry_1].Surface_Node_Set[n].Closest_Node_Ptr;

		if( n2 >= 0 )
		{
			for(long t=0;t<Surfaces_List[Geometry_2].Surface_Node_Set[n2].Neig_Triangles.size();t++)
			if( Surfaces_List[Geometry_2].Surface_Triangle_Set[
				Surfaces_List[Geometry_2].Surface_Node_Set[n2].Neig_Triangles[t] ].Removed_Flag == OFF )
			{
			nt = Surfaces_List[Geometry_2].Surface_Node_Set[n2].Neig_Triangles[t];
			n1 = Surfaces_List[Geometry_2].Surface_Triangle_Set[nt].Nodes[0];
			n2 = Surfaces_List[Geometry_2].Surface_Triangle_Set[nt].Nodes[0];
			n3 = Surfaces_List[Geometry_2].Surface_Triangle_Set[nt].Nodes[0];

			dist = PNUM.get_distance_between_3D_point_and_triangle(
				Surfaces_List[Geometry_1].Surface_Node_Set[n].x,
				Surfaces_List[Geometry_1].Surface_Node_Set[n].y,
				Surfaces_List[Geometry_1].Surface_Node_Set[n].z,

				Surfaces_List[Geometry_2].Surface_Node_Set[n1].x,
				Surfaces_List[Geometry_2].Surface_Node_Set[n1].y,
				Surfaces_List[Geometry_2].Surface_Node_Set[n1].z,

				Surfaces_List[Geometry_2].Surface_Node_Set[n2].x,
				Surfaces_List[Geometry_2].Surface_Node_Set[n2].y,
				Surfaces_List[Geometry_2].Surface_Node_Set[n2].z,

				Surfaces_List[Geometry_2].Surface_Node_Set[n3].x,
				Surfaces_List[Geometry_2].Surface_Node_Set[n3].y,
				Surfaces_List[Geometry_2].Surface_Node_Set[n3].z );

			Dist_Vec.push_back(dist);

			} // though neig triangles
		} // if corresponding node found
	} // through all nodes of geo1

	PNUM.stdev_cor_vec_double(&Dist_Vec, mean, SD);

	// get median
	Median[0] = PNUM.get_median(Dist_Vec);

	} // if in range
}

//---------------------------------------------------------------------------------

void STUDY_Class::find_corresponding_nodes_in_2_geometries(int Surface_1,int Surface_2,
			double Max_Dist_Threshold)
{

	long min_ptr=-1;
	double distance,min,dx,dy,dz;

	if( Surface_1 >= 0 && Surface_1 < Surfaces_List.size() &&
		Surface_2 >= 0 && Surface_2 < Surfaces_List.size() )
	{

	// clear flag
	for(long i=0;i<(signed)Surfaces_List[Surface_1].Surface_Node_Set.size();i++)
	Surfaces_List[Surface_1].Surface_Node_Set[i].Closest_Node_Ptr = -1;

	// for each data point find closest point in scan surface
	// and store it as Closest_Node_Ptr
	for(long i=0;i<(signed)Surfaces_List[Surface_1].Surface_Node_Set.size();i++)
	if( !Surfaces_List[Surface_1].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
	{

	Surfaces_List[Surface_1].Surface_Node_Set[i].Closest_Node_Ptr = -1;
	min = 100000.0;

	for(long j=0;j<(signed)Surfaces_List[Surface_2].Surface_Node_Set.size();j++)
	if( !Surfaces_List[Surface_2].Surface_Node_Set[j].Removed_Geometry_Vertex_Flag )
	{

	dx = fabs( Surfaces_List[Surface_1].Surface_Node_Set[i].x - Surfaces_List[Surface_2].Surface_Node_Set[j].x );
	dy = fabs( Surfaces_List[Surface_1].Surface_Node_Set[i].y - Surfaces_List[Surface_2].Surface_Node_Set[j].y );
	dz = fabs( Surfaces_List[Surface_1].Surface_Node_Set[i].z - Surfaces_List[Surface_2].Surface_Node_Set[j].z );

	if( dx < Max_Dist_Threshold && dy < Max_Dist_Threshold && dz < Max_Dist_Threshold )
	{

	distance = pow( dx, 2 )+pow( dy, 2 )+pow( dz, 2 );

	if( distance < min )
	{
		min = distance;
		min_ptr = j;
	}

	}

	}// for

	if( std::sqrt(min) < Max_Dist_Threshold )
	Surfaces_List[Surface_1].Surface_Node_Set[i].Closest_Node_Ptr = min_ptr;
	else
	Surfaces_List[Surface_1].Surface_Node_Set[i].Closest_Node_Ptr = -1;

	} // main for

	}

// EXPERIMENTAL CODE!!!! REPACED DISTANCE AS SQRT(POW()) WITH JUST SUM OF DIFFERENCES
/*
	long min_ptr=-1;
	double distance,min,dx,dy,dz;

	if( Surface_1 >= 0 && Surface_1 < Surfaces_List.size() &&
		Surface_2 >= 0 && Surface_2 < Surfaces_List.size() )
	{

	// clear flag
	for(long i=0;i<(signed)Surfaces_List[Surface_1].Surface_Node_Set.size();i++)
	Surfaces_List[Surface_1].Surface_Node_Set[i].Closest_Node_Ptr = -1;

	// for each data point find closest point in scan surface
	// and store it as Closest_Node_Ptr
	for(long i=0;i<(signed)Surfaces_List[Surface_1].Surface_Node_Set.size();i++)
//	if( !Surfaces_List[Surface_1].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
	{

	Surfaces_List[Surface_1].Surface_Node_Set[i].Closest_Node_Ptr = -1;
	min = 100000.0;

	for(long j=0;j<(signed)Surfaces_List[Surface_2].Surface_Node_Set.size();j++)
//	if( !Surfaces_List[Surface_2].Surface_Node_Set[j].Removed_Geometry_Vertex_Flag )
	{

	dx = fabs( Surfaces_List[Surface_1].Surface_Node_Set[i].x - Surfaces_List[Surface_2].Surface_Node_Set[j].x );
	dy = fabs( Surfaces_List[Surface_1].Surface_Node_Set[i].y - Surfaces_List[Surface_2].Surface_Node_Set[j].y );
	dz = fabs( Surfaces_List[Surface_1].Surface_Node_Set[i].z - Surfaces_List[Surface_2].Surface_Node_Set[j].z );

//	if( dx < Max_Dist_Threshold && dy < Max_Dist_Threshold && dz < Max_Dist_Threshold )
//	{

//	distance = pow( dx, 2 )+pow( dy, 2 )+pow( dz, 2 );
	distance = dx+dy+dz;

	if( distance < min )
	{
		min = distance;
		min_ptr = j;
	}

//	}

	}// for

	if( std::sqrt(min) < Max_Dist_Threshold )
	Surfaces_List[Surface_1].Surface_Node_Set[i].Closest_Node_Ptr = min_ptr;
	else
	Surfaces_List[Surface_1].Surface_Node_Set[i].Closest_Node_Ptr = -1;

	} // main for

	}
	*/
}

//---------------------------------------------------------------------------------
	// surface_2 is actually moved
void STUDY_Class::one_step_of_whole_cloud_fitting(
			int Surface_1,int Surface_2,int Surface_3,bool Fit_3rd_along_wiht_2nd,
			int Surface_4,bool Fit_4th_along_wiht_2nd,
			double Spring_Constant,double dtF,double dtM)

{
	double d1,d2,d3,fi1,fi2,fi3,f,d;
	double F1x,F1y,F1z,Fx,Fy,Fz;
	double M1x,M1y,M1z,Mx,My,Mz;
	double Tx,Ty,Tz;
	long k,surf_node_ptr;
	double dx_from_F,dy_from_F,dz_from_F;
	double F[3],min,v;

/* old values
	Spring_Constant = 5; // 15.0;
	dtF = 0.008;
	dtM = 0.0001;
*/

// steps:
// 1. calculate mean forces acting on all nodes
// 2. center data cloud (remember translation)
// 3. calculate momentum acting on data cloud   M = r x F
// 4. calculate translation of d.c. due to forces F
// 5. calculate rotation of d.c. due to momenum M
// 6. Rotate d.c., translate d.c.
// 7. Translate d.c. to initial position

   // 1.
   Fx = 0.0;
   Fy = 0.0;
   Fz = 0.0;
   Tx = 0.0;
   Ty = 0.0;
   Tz = 0.0;

	// datapoint set is mapped on Surface1,
	// surfacenode na surface2

   //// Forces are stored in Surfaces_List[Surface_1].Surface_Node_Set[i].normal[3]
   for(long i=0;i<(signed)Surfaces_List[Surface_1].Surface_Node_Set.size();i++)
   if( !Surfaces_List[Surface_1].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
   {

	// calculate forces
	surf_node_ptr = Surfaces_List[Surface_1].Surface_Node_Set[i].Closest_Node_Ptr;

	if( surf_node_ptr >= 0 )
	{
		Surfaces_List[Surface_1].Surface_Node_Set[i].normal[0] = -Spring_Constant*
		 (Surfaces_List[Surface_1].Surface_Node_Set[i].x -
		  Surfaces_List[Surface_2].Surface_Node_Set[surf_node_ptr].x);

		Surfaces_List[Surface_1].Surface_Node_Set[i].normal[1] = -Spring_Constant*
		 (Surfaces_List[Surface_1].Surface_Node_Set[i].y -
		  Surfaces_List[Surface_2].Surface_Node_Set[surf_node_ptr].y);

		Surfaces_List[Surface_1].Surface_Node_Set[i].normal[2] = -Spring_Constant*
		 (Surfaces_List[Surface_1].Surface_Node_Set[i].z -
		  Surfaces_List[Surface_2].Surface_Node_Set[surf_node_ptr].z);

	// calculate total force
	Fx += Surfaces_List[Surface_1].Surface_Node_Set[i].normal[0];
	Fy += Surfaces_List[Surface_1].Surface_Node_Set[i].normal[1];
	Fz += Surfaces_List[Surface_1].Surface_Node_Set[i].normal[2];

	}

	}

	// find center of the mass
	for(long i=0;i<(signed)Surfaces_List[Surface_2].Surface_Node_Set.size();i++)
	if( !Surfaces_List[Surface_2].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
	{
		Tx += Surfaces_List[Surface_2].Surface_Node_Set[i].x;
		Ty += Surfaces_List[Surface_2].Surface_Node_Set[i].y;
		Tz += Surfaces_List[Surface_2].Surface_Node_Set[i].z;
	}

	Tx = Tx / Surfaces_List[Surface_2].Surface_Node_Set.size();
	Ty = Ty / Surfaces_List[Surface_2].Surface_Node_Set.size();
	Tz = Tz / Surfaces_List[Surface_2].Surface_Node_Set.size();


	// average force
   Fx = Fx / Surfaces_List[Surface_1].Surface_Node_Set.size();
   Fy = Fy / Surfaces_List[Surface_1].Surface_Node_Set.size();
   Fz = Fz / Surfaces_List[Surface_1].Surface_Node_Set.size();

   // 3. M = rxF
   Mx = 0.0;
   My = 0.0;
   Mz = 0.0;

   // calculate momentum
   for(long i=0;i<(signed)Surfaces_List[Surface_1].Surface_Node_Set.size();i++)
   if( !Surfaces_List[Surface_1].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
   {

	// center data cloud
	Surfaces_List[Surface_1].Surface_Node_Set[i].x += Tx;
	Surfaces_List[Surface_1].Surface_Node_Set[i].y += Ty;
    Surfaces_List[Surface_1].Surface_Node_Set[i].z += Tz;

    // calculate M=rxF
	Mx += Surfaces_List[Surface_1].Surface_Node_Set[i].y*Surfaces_List[Surface_1].Surface_Node_Set[i].normal[2] -
          Surfaces_List[Surface_1].Surface_Node_Set[i].z*Surfaces_List[Surface_1].Surface_Node_Set[i].normal[1];

    My += Surfaces_List[Surface_1].Surface_Node_Set[i].z*Surfaces_List[Surface_1].Surface_Node_Set[i].normal[0] -
		  Surfaces_List[Surface_1].Surface_Node_Set[i].x*Surfaces_List[Surface_1].Surface_Node_Set[i].normal[2];

	Mz += Surfaces_List[Surface_1].Surface_Node_Set[i].x*Surfaces_List[Surface_1].Surface_Node_Set[i].normal[1] -
          Surfaces_List[Surface_1].Surface_Node_Set[i].y*Surfaces_List[Surface_1].Surface_Node_Set[i].normal[0];

	}

   // averagning M
   // WARNING!!!! DIVIDED BY ALL POINTS NOT EXCLUDING REMOVED ONES
   Mx = Mx / Surfaces_List[Surface_1].Surface_Node_Set.size();
   My = My / Surfaces_List[Surface_1].Surface_Node_Set.size();
   Mz = Mz / Surfaces_List[Surface_1].Surface_Node_Set.size();

   // 4. dr from F
   dx_from_F = dtF*Fx;
   dy_from_F = dtF*Fy;
   dz_from_F = dtF*Fz;

   // 5. calculate rotation matrix of d.c. due to M=rXF

   // rotation angle due to M
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

	// acknowledge direction of M vector
	// if its directed downward, rotation is in opposite direction
   if( Mz < 0 ) fi3 = -fi3;

	// move back Surface_1
	for(long i=0;i<(signed)Surfaces_List[Surface_1].Surface_Node_Set.size();i++)
	if( !Surfaces_List[Surface_1].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
	{
		Surfaces_List[Surface_1].Surface_Node_Set[i].x -= Tx;
		Surfaces_List[Surface_1].Surface_Node_Set[i].y -= Ty;
		Surfaces_List[Surface_1].Surface_Node_Set[i].z -= Tz;
	}


 //---------------------------------------------------------------------
 //---------------------------------------------------------------------
 // PERFORM ACTUAL MOVMENT OF Surface_2
 //---------------------------------------------------------------------
 //---------------------------------------------------------------------

	for(long i=0;i<Surfaces_List[Surface_2].Surface_Node_Set.size();i++)
	if( !Surfaces_List[Surface_2].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
	{

	PNUM.rotate_vector(&Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,fi1,3);

	PNUM.rotate_vector(&Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(&Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(&Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,fi2,2);

	PNUM.rotate_vector(&Surfaces_List[Surface_2].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_2].Surface_Node_Set[i].z,-fi1,3);

	Surfaces_List[Surface_2].Surface_Node_Set[i].x -= dx_from_F;
	Surfaces_List[Surface_2].Surface_Node_Set[i].y -= dy_from_F;
	Surfaces_List[Surface_2].Surface_Node_Set[i].z -= dz_from_F;
	}

	//----------------------------------------------------------------
	// data points
	//----------------------------------------------------------------
	for(long dset=0;dset<(signed)Surfaces_List[Surface_2].Data_Point_Set.size();dset++)
	for(long d=0;d<(signed)Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points.size();d++)
	{
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z,-fi1,3);

	Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].x -= dx_from_F;
	Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].y -= dy_from_F;
	Surfaces_List[Surface_2].Data_Point_Set[dset].Data_Points[d].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// rotate registration points on 3D goemetry
	//----------------------------------------------------------------
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,fi1,3);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,fi2,2);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_A.x,&Surfaces_List[Surface_2].Registration_Point_A.y,&Surfaces_List[Surface_2].Registration_Point_A.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_B.x,&Surfaces_List[Surface_2].Registration_Point_B.y,&Surfaces_List[Surface_2].Registration_Point_B.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_2].Registration_Point_C.x,&Surfaces_List[Surface_2].Registration_Point_C.y,&Surfaces_List[Surface_2].Registration_Point_C.z,-fi1,3);

	Surfaces_List[Surface_2].Registration_Point_A.x -= dx_from_F;
	Surfaces_List[Surface_2].Registration_Point_A.y -= dy_from_F;
	Surfaces_List[Surface_2].Registration_Point_A.z -= dz_from_F;

	Surfaces_List[Surface_2].Registration_Point_B.x -= dx_from_F;
	Surfaces_List[Surface_2].Registration_Point_B.y -= dy_from_F;
	Surfaces_List[Surface_2].Registration_Point_B.z -= dz_from_F;

	Surfaces_List[Surface_2].Registration_Point_C.x -= dx_from_F;
	Surfaces_List[Surface_2].Registration_Point_C.y -= dy_from_F;
	Surfaces_List[Surface_2].Registration_Point_C.z -= dz_from_F;

	//-----------------------------------------------
	if(Fit_3rd_along_wiht_2nd)
	//-----------------------------------------------
	{

	for(long i=0;i<Surfaces_List[Surface_3].Surface_Node_Set.size();i++)
	if( !Surfaces_List[Surface_3].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
	{

	PNUM.rotate_vector(&Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,fi1,3);

	PNUM.rotate_vector(&Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(&Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(&Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,fi2,2);

	PNUM.rotate_vector(&Surfaces_List[Surface_3].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_3].Surface_Node_Set[i].z,-fi1,3);

	Surfaces_List[Surface_3].Surface_Node_Set[i].x -= dx_from_F;
	Surfaces_List[Surface_3].Surface_Node_Set[i].y -= dy_from_F;
	Surfaces_List[Surface_3].Surface_Node_Set[i].z -= dz_from_F;
	}

	//----------------------------------------------------------------
	// data points
	//----------------------------------------------------------------
	for(long dset=0;dset<(signed)Surfaces_List[Surface_3].Data_Point_Set.size();dset++)
	for(long d=0;d<(signed)Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points.size();d++)
	{
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z,-fi1,3);

	Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].x -= dx_from_F;
	Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].y -= dy_from_F;
	Surfaces_List[Surface_3].Data_Point_Set[dset].Data_Points[d].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// rotate registration points on 3D goemetry
	//----------------------------------------------------------------
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,fi1,3);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,fi2,2);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_A.x,&Surfaces_List[Surface_3].Registration_Point_A.y,&Surfaces_List[Surface_3].Registration_Point_A.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_B.x,&Surfaces_List[Surface_3].Registration_Point_B.y,&Surfaces_List[Surface_3].Registration_Point_B.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_3].Registration_Point_C.x,&Surfaces_List[Surface_3].Registration_Point_C.y,&Surfaces_List[Surface_3].Registration_Point_C.z,-fi1,3);

	Surfaces_List[Surface_3].Registration_Point_A.x -= dx_from_F;
	Surfaces_List[Surface_3].Registration_Point_A.y -= dy_from_F;
	Surfaces_List[Surface_3].Registration_Point_A.z -= dz_from_F;

	Surfaces_List[Surface_3].Registration_Point_B.x -= dx_from_F;
	Surfaces_List[Surface_3].Registration_Point_B.y -= dy_from_F;
	Surfaces_List[Surface_3].Registration_Point_B.z -= dz_from_F;

	Surfaces_List[Surface_3].Registration_Point_C.x -= dx_from_F;
	Surfaces_List[Surface_3].Registration_Point_C.y -= dy_from_F;
	Surfaces_List[Surface_3].Registration_Point_C.z -= dz_from_F;

	} // surface 3 movmement


	//-----------------------------------------------
	if(Fit_4th_along_wiht_2nd)
	//-----------------------------------------------
	{

	for(long i=0;i<Surfaces_List[Surface_4].Surface_Node_Set.size();i++)
	if( !Surfaces_List[Surface_4].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
	{

	PNUM.rotate_vector(&Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,fi1,3);

	PNUM.rotate_vector(&Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(&Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(&Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,fi2,2);

	PNUM.rotate_vector(&Surfaces_List[Surface_4].Surface_Node_Set[i].x,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].y,
				  &Surfaces_List[Surface_4].Surface_Node_Set[i].z,-fi1,3);

	Surfaces_List[Surface_4].Surface_Node_Set[i].x -= dx_from_F;
	Surfaces_List[Surface_4].Surface_Node_Set[i].y -= dy_from_F;
	Surfaces_List[Surface_4].Surface_Node_Set[i].z -= dz_from_F;
	}

	//----------------------------------------------------------------
	// data points
	//----------------------------------------------------------------
	for(long dset=0;dset<(signed)Surfaces_List[Surface_4].Data_Point_Set.size();dset++)
	for(long d=0;d<(signed)Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points.size();d++)
	{
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,fi1,3);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,fi2,2);

	PNUM.rotate_vector(
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y,
				  &Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z,-fi1,3);

	Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].x -= dx_from_F;
	Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].y -= dy_from_F;
	Surfaces_List[Surface_4].Data_Point_Set[dset].Data_Points[d].z -= dz_from_F;

	}

	//----------------------------------------------------------------
	// rotate registration points on 3D goemetry
	//----------------------------------------------------------------
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,fi1,3);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,-fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,-fi2,2);

	// main rotation (fi3)
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,fi3,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,fi3,3);

	// reverse rotation
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,fi2,2);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,fi2,2);

	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_A.x,&Surfaces_List[Surface_4].Registration_Point_A.y,&Surfaces_List[Surface_4].Registration_Point_A.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_B.x,&Surfaces_List[Surface_4].Registration_Point_B.y,&Surfaces_List[Surface_4].Registration_Point_B.z,-fi1,3);
	PNUM.rotate_vector(
		&Surfaces_List[Surface_4].Registration_Point_C.x,&Surfaces_List[Surface_4].Registration_Point_C.y,&Surfaces_List[Surface_4].Registration_Point_C.z,-fi1,3);

	Surfaces_List[Surface_4].Registration_Point_A.x -= dx_from_F;
	Surfaces_List[Surface_4].Registration_Point_A.y -= dy_from_F;
	Surfaces_List[Surface_4].Registration_Point_A.z -= dz_from_F;

	Surfaces_List[Surface_4].Registration_Point_B.x -= dx_from_F;
	Surfaces_List[Surface_4].Registration_Point_B.y -= dy_from_F;
	Surfaces_List[Surface_4].Registration_Point_B.z -= dz_from_F;

	Surfaces_List[Surface_4].Registration_Point_C.x -= dx_from_F;
	Surfaces_List[Surface_4].Registration_Point_C.y -= dy_from_F;
	Surfaces_List[Surface_4].Registration_Point_C.z -= dz_from_F;

	} // surface 4 movmement

}

//---------------------------------------------------------------------------------

void STUDY_Class::center_geometries(int Start_Geo,int Stop_Geo)
{
	if( Start_Geo >= 0 && Start_Geo < Surfaces_List.size() &&
		Stop_Geo >= 0 && Stop_Geo < Surfaces_List.size() )
	{
	double mx=0,my=0,mz=0;
	long cnt=0;

	// 1. Compute translation vector using ALL geometries!!!
	for(long S=Start_Geo;S<=Stop_Geo;S++)
	for(long i=0;i<(signed)Surfaces_List[S].Surface_Node_Set.size();i++)
	{
			mx += Surfaces_List[S].Surface_Node_Set[i].x;
			my += Surfaces_List[S].Surface_Node_Set[i].y;
			mz += Surfaces_List[S].Surface_Node_Set[i].z;
			cnt++;
	}

	if(cnt!=0)
	{
		mx /= cnt;
		my /= cnt;
		mz /= cnt;
	}

	for(long S=Start_Geo;S<=Stop_Geo;S++)
	{
		Surfaces_List[S].Translation_Vector_X -= mx;
		Surfaces_List[S].Translation_Vector_Y -= my;
		Surfaces_List[S].Translation_Vector_Z -= mz;
	}

	// Translate ALL geos
	for(long S=Start_Geo;S<=Stop_Geo;S++)
	for(long i=0;i<(signed)Surfaces_List[S].Surface_Node_Set.size();i++)
	{
		Surfaces_List[S].Surface_Node_Set[i].x -= mx;
		Surfaces_List[S].Surface_Node_Set[i].y -= my;
		Surfaces_List[S].Surface_Node_Set[i].z -= mz;
	}

	// translate all data points in all sets
	for(long S=Start_Geo;S<=Stop_Geo;S++)
	for(long dset=0;dset<Surfaces_List[S].Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)Surfaces_List[S].Data_Point_Set[dset].Data_Points.size();i++)
	{
		Surfaces_List[S].Data_Point_Set[dset].Data_Points[i].x -= mx;
		Surfaces_List[S].Data_Point_Set[dset].Data_Points[i].y -= my;
		Surfaces_List[S].Data_Point_Set[dset].Data_Points[i].z -= mz;
	}

	for(long S=Start_Geo;S<=Stop_Geo;S++)
	for(long dset=0;dset<Surfaces_List[S].Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)Surfaces_List[S].Ablation_Points_List.size();i++)
	{
		Surfaces_List[S].Ablation_Points_List[i].x -= mx;
		Surfaces_List[S].Ablation_Points_List[i].y -= my;
		Surfaces_List[S].Ablation_Points_List[i].z -= mz;
	}

	// center ABC spatial registration points
	for(long S=Start_Geo;S<=Stop_Geo;S++)
	Surfaces_List[S].translate_ABC_spatial_registration_markers(mx,my,mz);

	for(long S=Start_Geo;S<=Stop_Geo;S++)
	Surfaces_List[S].compute_segments_centers(500);

//	if( Echo )
//	ShowMessage("Translation vector: [" + FloatToStrF(mx,ffGeneral,3,2 )+","+ FloatToStrF(my,ffGeneral,3,2 )+","+ FloatToStrF(mz,ffGeneral,3,2 )+"]");
	}
}

//---------------------------------------------------------------------------------

AnsiString STUDY_Class::project_surface_onto_other_surface_ray_intersection_as_value(
		int Projected_Geometry,int Base_Geometry,double Threshold,
		int Data_Point_Set_Ptr, AnsiString Geo_Value_Name,
		TProgress_Form *Progress_Form,bool In_Out_Test)
{
	Vector3d Orig,Dir_Out,Dir_In,Vert1,Vert2,Vert3,BaryPosition;
	long n0,n1,n2,nt0,nt1,nt2,Crossed_Triangle_Ptr;
	bool Result,One_of_Triangles_Crossed;
	double Distance,origin_x,origin_y,origin_z;
	double tested_t_x,tested_t_y,tested_t_z;
	double Distance_In,Distance_Out;
	bool Test_In, Test_Out;

	// add geo projection value to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_GEO_NODES;
	Value_Desc.Interpolation_Type = INTERPOLATION_TYPE_FLAG_NO_INTERPOLATION;
	Value_Desc.Displayed_In_Table = false;
	Value_Desc.Value_Name = Geo_Value_Name;
	Value_Desc.Unit = "";
	Surfaces_List[Base_Geometry].Map_Values.add_value(Value_Desc);
	int Value_Id = Surfaces_List[Base_Geometry].Map_Values.get_value_ptr(Value_Desc.Value_Name);
	if(Value_Id<0)
	ShowMessage("Value empty studyclass project surf");

	// thru all triangels of base geometry
	for(long tt=0;tt<Surfaces_List[Base_Geometry].Surface_Triangle_Set.size();tt++)
	{

	if( tt%30 == 1 )
	{
		Progress_Form->replace_last_line_with_text("Processing triangle: " + IntToStr((int)tt) + "/" +
			IntToStr((int)(Surfaces_List[Base_Geometry].Surface_Triangle_Set.size())) );
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	n0 = Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].Nodes[0];
	n1 = Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].Nodes[1];
	n2 = Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].Nodes[2];

	origin_x = (Surfaces_List[Base_Geometry].Surface_Node_Set[n0].x +
				Surfaces_List[Base_Geometry].Surface_Node_Set[n1].x +
				Surfaces_List[Base_Geometry].Surface_Node_Set[n2].x )/3.0;

	origin_y = (Surfaces_List[Base_Geometry].Surface_Node_Set[n0].y +
				Surfaces_List[Base_Geometry].Surface_Node_Set[n1].y +
				Surfaces_List[Base_Geometry].Surface_Node_Set[n2].y )/3.0;

	origin_z = (Surfaces_List[Base_Geometry].Surface_Node_Set[n0].z +
				Surfaces_List[Base_Geometry].Surface_Node_Set[n1].z +
				Surfaces_List[Base_Geometry].Surface_Node_Set[n2].z )/3.0;

	// create ray from normals

	// origin is the geo center of the triangle
	Orig = Vector3d( origin_x, origin_y, origin_z );

	// Direction is the triangle normal
	Dir_Out = Vector3d ( Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].normal[0],
					 Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].normal[1],
					 Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].normal[2] );
	Dir_In = Vector3d ( -Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].normal[0],
					 -Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].normal[1],
					 -Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].normal[2] );

	// thru all triangels of the source geometry (the one to be projected)
	One_of_Triangles_Crossed = false;
	Crossed_Triangle_Ptr = -1;

	for(long ts=0;ts<Surfaces_List[Projected_Geometry].Surface_Triangle_Set.size();ts++)
	if( !One_of_Triangles_Crossed )
	{

	nt0 = Surfaces_List[Projected_Geometry].Surface_Triangle_Set[ts].Nodes[0];
	nt1 = Surfaces_List[Projected_Geometry].Surface_Triangle_Set[ts].Nodes[1];
	nt2 = Surfaces_List[Projected_Geometry].Surface_Triangle_Set[ts].Nodes[2];

	tested_t_x = (Surfaces_List[Projected_Geometry].Surface_Node_Set[nt0].x +
				  Surfaces_List[Projected_Geometry].Surface_Node_Set[nt1].x +
				  Surfaces_List[Projected_Geometry].Surface_Node_Set[nt2].x )/3.0;

	tested_t_y = (Surfaces_List[Projected_Geometry].Surface_Node_Set[nt0].y +
				  Surfaces_List[Projected_Geometry].Surface_Node_Set[nt1].y +
				  Surfaces_List[Projected_Geometry].Surface_Node_Set[nt2].y )/3.0;

	tested_t_z = (Surfaces_List[Projected_Geometry].Surface_Node_Set[nt0].z +
				  Surfaces_List[Projected_Geometry].Surface_Node_Set[nt1].z +
				  Surfaces_List[Projected_Geometry].Surface_Node_Set[nt2].z )/3.0;

	// do test only if triangle within range
	if( fabs(tested_t_x - origin_x) < Threshold &&
		fabs(tested_t_y - origin_y) < Threshold &&
		fabs(tested_t_z - origin_z) < Threshold )
	{

	Vert1 = Vector3d( Surfaces_List[Projected_Geometry].Surface_Node_Set[nt0].x,
					  Surfaces_List[Projected_Geometry].Surface_Node_Set[nt0].y,
					  Surfaces_List[Projected_Geometry].Surface_Node_Set[nt0].z );
	Vert2 = Vector3d ( Surfaces_List[Projected_Geometry].Surface_Node_Set[nt1].x,
					   Surfaces_List[Projected_Geometry].Surface_Node_Set[nt1].y,
					   Surfaces_List[Projected_Geometry].Surface_Node_Set[nt1].z );
	Vert3 = Vector3d ( Surfaces_List[Projected_Geometry].Surface_Node_Set[nt2].x,
					   Surfaces_List[Projected_Geometry].Surface_Node_Set[nt2].y,
					   Surfaces_List[Projected_Geometry].Surface_Node_Set[nt2].z );


	// 	Compute the intersection of a ray and a triangle.

	// just out test
	if(!In_Out_Test)
	if( PNUM.intersectRayTriangle(Orig, Dir_Out,Vert1,Vert2,Vert3,BaryPosition, &Distance ) )
	if( Distance < Threshold )
	{
		One_of_Triangles_Crossed = true;
		Crossed_Triangle_Ptr = ts;
	}

	// both, in and out tests
	if( In_Out_Test)
	{

	Test_In =  PNUM.intersectRayTriangle(Orig, Dir_In,Vert1,Vert2,Vert3,BaryPosition, &Distance_In);
	Test_Out = PNUM.intersectRayTriangle(Orig, Dir_Out,Vert1,Vert2,Vert3,BaryPosition, &Distance_Out);

	if( (Test_In && Distance_In < Threshold) || (Test_Out && Distance_Out < Threshold) )
	{
		One_of_Triangles_Crossed = true;
		Crossed_Triangle_Ptr = ts;
	}

	}

	} // if triangle within threshold

	} // thru source surf

	// store which triangle was crossed in target geometry
	Surfaces_List[Base_Geometry].Surface_Node_Set[n0].Flag_A = Crossed_Triangle_Ptr;
	Surfaces_List[Base_Geometry].Surface_Node_Set[n1].Flag_A = Crossed_Triangle_Ptr;
	Surfaces_List[Base_Geometry].Surface_Node_Set[n2].Flag_A = Crossed_Triangle_Ptr;
	Surfaces_List[Base_Geometry].Surface_Triangle_Set[tt].Tmp_Flag = Crossed_Triangle_Ptr;

	if( One_of_Triangles_Crossed )
	{
		Surfaces_List[Base_Geometry].Surface_Node_Set[n0].set_value(Data_Point_Set_Ptr,Value_Id,1);
		Surfaces_List[Base_Geometry].Surface_Node_Set[n1].set_value(Data_Point_Set_Ptr,Value_Id,1);
		Surfaces_List[Base_Geometry].Surface_Node_Set[n2].set_value(Data_Point_Set_Ptr,Value_Id,1);
	}
	else
	{
		Surfaces_List[Base_Geometry].Surface_Node_Set[n0].set_value(Data_Point_Set_Ptr,Value_Id,0);
		Surfaces_List[Base_Geometry].Surface_Node_Set[n1].set_value(Data_Point_Set_Ptr,Value_Id,0);
		Surfaces_List[Base_Geometry].Surface_Node_Set[n2].set_value(Data_Point_Set_Ptr,Value_Id,0);
	}

	} // thru target surf triangles

	Progress_Form->Hide();

	return Value_Desc.Value_Name;
}


//---------------------------------------------------------------------------------


