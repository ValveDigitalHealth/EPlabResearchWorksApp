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
#pragma hdrstop

#include "data_io_class.h"

#pragma package(smart_init)

//------------------------------------------------------------------------------
Data_IO_Class::Data_IO_Class()
{

}

//------------------------------------------------------------------------------

AnsiString Data_IO_Class::import_NavX_DxL_folder(TFileListBox* Data_FileListBox,
	STUDY_Class *Study, TProgress_Form *Progress_Form,bool Append)
{
	std::vector <AnsiString> DxL_File_Names;
	std::vector <int> DxL_File_Ptrs;
	bool Geo_File_Found = false;

	//-------------------------------------
	// check if proper files present
	//-------------------------------------
	int Geo_File_Id = -1;

	for( int i=0; i < Data_FileListBox->Items[0].Capacity; i++ )
	if( Utils.is_substring_present( Data_FileListBox->Items[0].Strings[i],"ModelGroups.xml") )
	Geo_File_Id = i;

	// find DxLandmarkGeo.xml file
	if( Geo_File_Id == -1)
	{
		for( int i=0; i < Data_FileListBox->Items[0].Capacity; i++ )
		if( Utils.is_substring_present( Data_FileListBox->Items[0].Strings[i],"DxLandmarkGeo.xml") )
		Geo_File_Id = i;
	}

	if( Geo_File_Id == -1)
		return "DxLandmarkGeo.xml nor ModelGroups.xml file not found! If not present, re-do export according to instructions.";

	// update progress form
	Progress_Form->add_text("Geo file found: " + Data_FileListBox->Items[0].Strings[Geo_File_Id]);
	Progress_Form->Show();
	Application->ProcessMessages();
	if( Progress_Form->Cancel_Flag ) return "Cancelled";

	// find DxL files
	for( int i=0; i < Data_FileListBox->Items[0].Capacity; i++ )
	if( Utils.is_substring_present( Data_FileListBox->Items[0].Strings[i],"DxL_") )
	{
		DxL_File_Names.push_back( Data_FileListBox->Items[0].Strings[i] );
		DxL_File_Ptrs.push_back( i );
	}

	if( DxL_File_Names.size() == 0 )
		return "DxL maps files not found! Re-do export according to instructions.";

	// update progress form
	for(int i=0;i<DxL_File_Ptrs.size();i++)
	Progress_Form->add_text("DxL files found: " + Data_FileListBox->Items[0].Strings[DxL_File_Ptrs[i]]);
	Progress_Form->Show();
	if( Progress_Form->Cancel_Flag ) return "Cancelled";
	Application->ProcessMessages();

	Progress_Form->add_text("Parsing geometry file...");
	Progress_Form->Show();
	Application->ProcessMessages();

	pugi::xml_document Geometry_pugi_object;
	AnsiString Geo_File_Path = Data_Files_Path + "\\"+ Data_FileListBox->Items[0].Strings[Geo_File_Id];
	pugi::xml_parse_result Xml_reading_result = Geometry_pugi_object.load_file(Geo_File_Path.c_str());

	Progress_Form->add_text(Xml_reading_result.description());
	Progress_Form->Show();
	Application->ProcessMessages();

	if( !Xml_reading_result )
	return "Corrupted xml geometry file";

	//-------------------------------------
	// ask user which geometry to load
	//-------------------------------------
	vector <AnsiString> Geo_Names_List;

	pugi::xml_node Volumes = Geometry_pugi_object.child("DIF").child("DIFBody").child("Volumes");
	for (pugi::xml_node_iterator it=Volumes.begin(); it!=Volumes.end(); ++it)
		Geo_Names_List.push_back(it->attribute("name").value());

	if( Geo_Names_List.size() == 0 )
	return "No maps found in DxLandmarkGeo.xml file";

	Selection_List_Form->Caption = "Select geometry to load";
	Selection_List_Form->fill_selection(&Geo_Names_List);

	if( !Selection_List_Form->ShowModal() )
	return "Import cancelled";

	if( Selection_List_Form->Selected_Item_Ptr == -1 )
	return "No geometry to import selected";

	AnsiString Selected_Geometry;
	if( Selection_List_Form->Selected_Item_Ptr >= 0 &&
		Selection_List_Form->Selected_Item_Ptr < Geo_Names_List.size() )
	Selected_Geometry = Geo_Names_List[Selection_List_Form->Selected_Item_Ptr];
	else
	return "Selected geometry pointer out of range";

	Progress_Form->add_text("Geometry selected, importing...");
	Progress_Form->Show();
	Application->ProcessMessages();
	if( Progress_Form->Cancel_Flag )
		return "Cancelled";

	// --------------------------------------------------------------------
	// Import geometry file
	// --------------------------------------------------------------------
	// clear surfaces
	if( !Append )
	Study->Surfaces_List.clear();

	Surface_Class Suface_Class_Item;
	Study->Surfaces_List.push_back(Suface_Class_Item);
	Study->Current_Surface = Study->Surfaces_List.size()-1;
	Study->Surfaces_List[Study->Current_Surface].Surface_Node_Set.clear();
	Study->Surfaces_List[Study->Current_Surface].Surface_Triangle_Set.clear();
	Study->Surfaces_List[Study->Current_Surface].Name = Selected_Geometry;
	Study->Surfaces_List[Study->Current_Surface].Mapping_System_Source = MAPPING_SYSTEM_ORIGIN_NAVX;

	Study->Study_Source_Path = Data_Files_Path;
	Study->Study_Current_Path_And_File_Name =
		Utils.get_string_before_last_occurence_of_specified_string(Data_Files_Path,"\\") + "\\" +
		Utils.get_string_after_last_occurence_of_specified_string(Data_Files_Path,"\\") + " " +
			Selected_Geometry + ".eplab";

	AnsiString Result="";
	Result = load_navx_geo_file(Geo_File_Path,Selected_Geometry,
		&Study->Surfaces_List[Study->Current_Surface],&Geometry_pugi_object);
	if( Result.Length() > 3 )
		return Result;

	Progress_Form->Cursor = crArrow;

	// --------------------------------------------------------------------
	// Import data files with signals
	// --------------------------------------------------------------------
	int Type = 1; // Velocity import by default - if clients will have old navx, change
	Result="";
	Data_Point_Set_Class Data_Point_Set;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set.push_back(Data_Point_Set);
	int Current_Data_Point_Set = Study->Surfaces_List[Study->Current_Surface].Data_Point_Set.size()-1;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[Current_Data_Point_Set].Name = "NavX Dataset";

	// update progress form
	Progress_Form->add_text("Importing DxL files...");
	Progress_Form->Show();
	Application->ProcessMessages();
	if( Progress_Form->Cancel_Flag ) return "Cancelled";

	Progress_Form->Cursor = crHourGlass;

	//----------------------------------------------------------------------
	// set values descriptors (usun wyzej)
	//----------------------------------------------------------------------
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS; // based on data points
	Value_Desc.Displayed_In_Table = true;
/*
	Value_Desc.Value_Name = "ref LAT";
	Value_Desc.Unit = "ms";
	Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);
*/
	Value_Desc.Value_Name = "rov LAT";
	Value_Desc.Unit = "ms";
	Value_Desc.LAT_Value = true;
	Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);
	Value_Desc.LAT_Value = false;

	Value_Desc.Value_Name = "peak2peak";
	Value_Desc.Unit = "mV";
	Value_Desc.Voltage_Amplitude_Value = true;
	Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);
	Value_Desc.Voltage_Amplitude_Value = false;

	Value_Desc.Value_Name = "peak neg";
	Value_Desc.Unit = "mV";
	Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);

	Value_Desc.Value_Name = "CFE mean";
	Value_Desc.Unit = "";
	Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);

	Value_Desc.Value_Name = "CFE stdev";
	Value_Desc.Unit = "";
	Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);

	//------------------------------------------------------------------------------
	// DXL FILES READING LOOP
	//------------------------------------------------------------------------------
	for(unsigned long i=0; i<DxL_File_Names.size();i++)
	{
		Progress_Form->add_text("Importing file:" + DxL_File_Names[i] + "...");
		Progress_Form->Show();
		Application->ProcessMessages();

		if( Progress_Form->Cancel_Flag ) return "Cancelled";

		if( Type == 0 )
			Result = read_old_navx_data_file( DxL_File_Names[i],
				&Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[Current_Data_Point_Set] );
		if( Type == 1 )
			Result = read_navx_velocity_data_file(DxL_File_Names[i],
				&Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[Current_Data_Point_Set],Study );

		// check result
		if( Result.Length() > 3 )
			return Result;
	}

	//----------------------------------------------------
	// read ablation points list
	//----------------------------------------------------

	// find AutoMarkSummaryList.csv file
	int AutoMarkSummaryList_File_Id = -1;
	for( int i=0; i < Data_FileListBox->Items[0].Capacity; i++ )
	if( Utils.is_substring_present( Data_FileListBox->Items[0].Strings[i],"AutoMarkSummaryList.csv") )
	AutoMarkSummaryList_File_Id = i;

	if( AutoMarkSummaryList_File_Id >= 0 )
	read_navx_AutoMarkSummaryList_File(Data_FileListBox->Items[0].Strings[AutoMarkSummaryList_File_Id],
		&Study->Surfaces_List[Study->Current_Surface] );


	Progress_Form->Cursor = crArrow;

	// update progress form
	Progress_Form->add_text("DxL files imported, allocating data structures..." );
	Progress_Form->Show();
	Application->ProcessMessages();
	if( Progress_Form->Cancel_Flag ) return "Cancelled";

	Progress_Form->Show();
	Application->ProcessMessages();

	return "Import completed";
}

//------------------------------------------------------------------------------

int Data_IO_Class::get_ABL_xyz_from_Locations_file(AnsiString FileNamePath,long Time_Ptr,
	double *x, double *y, double *z)
{
	// this funciton opens Locations.csv file, searches for ABL channel id and reads xyz
	// of this channel at specific time
	// returns false if xyz not found

	//qt, kosiuk
	int Start_Pos=-1;
	int Channel_Id = -1;
	bool found;
	std::vector <Row_vec> Table;
	std::string SS;

	Table = Utils.load_rectangular_csv_file_to_grid(FileNamePath);

	//----------------------------------------------------
	// find abl channel id
	//----------------------------------------------------
	found = false;
	for(long row=1;row<(signed)Table.size();row++)
	if( !found )
	if( Table[row].Elements.size() > 0 )
	if( Table[row].Elements[1].Trim() == "ABL" && Table[row].Elements[2].Trim() == "D" )
	{
		Channel_Id = Table[row].Elements[0].Trim().ToInt();
		found = true;
	}

	if( found )
	{

	//----------------------------------------------------
	// find number of samples
	//----------------------------------------------------
	for(long row=1;row<(signed)Table.size();row++)
	if( Table[row].Elements.size() > 0 )
	if( Table[row].Elements[0].Trim() == "Number of samples (rows):" )
	Start_Pos = row;

	long Samples_Number = Table[Start_Pos].Elements[1].Trim().ToInt();

	if( Time_Ptr < 1 || Time_Ptr > Samples_Number )
	return -1; // pointed time_ptr outside data range in file

	//----------------------------------------------------
	// find column with abl xyz
	//----------------------------------------------------
	int Header_Row = Start_Pos + 1;
	found = false;
	for(long col=1;col<(signed)Table[Header_Row].Elements.size();col++)
	if( !found )
	if( Table[Header_Row].Elements[col].Trim() == "c"+IntToStr(Channel_Id)+"x" )
	{
		x[0] = Table[Header_Row].Elements[col].Trim().ToDouble();
		y[0] = Table[Header_Row].Elements[col+1].Trim().ToDouble();
		z[0] = Table[Header_Row].Elements[col+2].Trim().ToDouble();
		found = true;
	}

	if( found )
	return 1;
	else
	return -2; // column with abl xyz not found

	}
	else
	return -3; // channel ABL-D not found
}

//------------------------------------------------------------------------------

void Data_IO_Class::read_navx_AutoMarkSummaryList_File(AnsiString FileName,Surface_Class *Surface)
{
	//qt
	vector<Ablation_Point_Class> Ablation_Points_List;
	int Start_Pos=-1;
	Ablation_Point_Class ABL_Point;
	std::vector <Row_vec> Table;
	std::string SS;

	Table = Utils.load_rectangular_csv_file_to_grid(FileName);
//kosiuk
	for(long row=1;row<(signed)Table.size();row++)
	if( Table[row].Elements.size() > 0 )
	if( Table[row].Elements[0].Trim() == "RF Episode" )
	Start_Pos = row+1;

	if( Start_Pos > 0 )
	for(long row=Start_Pos;row<(signed)Table.size();row++)
	{
		ABL_Point.RF_Episode = Utils.get_number_from_string(Table[row].Elements[0].c_str());
		ABL_Point.Lesion_ID =  Utils.get_number_from_string(Table[row].Elements[1].c_str());

		ABL_Point.Start_Time =  Utils.get_number_from_string(Table[row].Elements[3].Trim().c_str());
		ABL_Point.End_Time =    Utils.get_number_from_string(Table[row].Elements[4].Trim().c_str());

		ABL_Point.Duration =        Utils.get_number_from_string(Table[row].Elements[5].c_str());
		ABL_Point.Is_Transition =   Utils.get_number_from_string(Table[row].Elements[6].c_str());

		ABL_Point.x =        Utils.get_number_from_string(Table[row].Elements[7].c_str());
		ABL_Point.y =        Utils.get_number_from_string(Table[row].Elements[8].c_str());
		ABL_Point.z =        Utils.get_number_from_string(Table[row].Elements[9].c_str());

		ABL_Point.Energy =        Utils.get_number_from_string(Table[row].Elements[10].c_str());
		ABL_Point.Avg_Power =        Utils.get_number_from_string(Table[row].Elements[11].c_str());
		ABL_Point.Avg_Temp =        Utils.get_number_from_string(Table[row].Elements[12].c_str());
		ABL_Point.Max_Temp =        Utils.get_number_from_string(Table[row].Elements[13].c_str());

		ABL_Point.Imp_Max =        Utils.get_number_from_string(Table[row].Elements[14].c_str());
		ABL_Point.Imp_Min =        Utils.get_number_from_string(Table[row].Elements[15].c_str());
		ABL_Point.Imp_Drop =        Utils.get_number_from_string(Table[row].Elements[16].c_str());
		ABL_Point.Imp_Drop_Perc =        Utils.get_number_from_string(Table[row].Elements[17].c_str());
		ABL_Point.Avg_Contact_Force_g =        Utils.get_number_from_string(Table[row].Elements[18].c_str());
		ABL_Point.Min_Contact_Force_g =        Utils.get_number_from_string(Table[row].Elements[19].c_str());
		ABL_Point.Max_Contact_Force_g =        Utils.get_number_from_string(Table[row].Elements[20].c_str());
		ABL_Point.FTI =        Utils.get_number_from_string(Table[row].Elements[21].c_str());
		ABL_Point.LSI =        Utils.get_number_from_string(Table[row].Elements[22].c_str());

		Ablation_Points_List.push_back(ABL_Point);
	}

	Surface->Ablation_Points_List = Ablation_Points_List;

}

//------------------------------------------------------------------------------

vector <AnsiString> Data_IO_Class::get_list_of_geo_names_from_NavX_geo(AnsiString FileName,bool* Load_Succesfull)
{
	vector <AnsiString> Volumes_Names;
	ifstream dfile( FileName.c_str() );
	char string[20000];

	if( !dfile.is_open() )
	{
        Load_Succesfull[0] = false;
		return Volumes_Names;
    }

    //-----------------------------------------------
    // find all geos in the file
	//-----------------------------------------------

    while( !dfile.eof() )
	{
        dfile >> string;

        if( !strcmp( string,"<Volume")  )
		{
			dfile >>  string;
			Volumes_Names.push_back((AnsiString)string);
        }
    }

    if( Volumes_Names.size() > 0)
	Load_Succesfull[0] = true;
	else
	Load_Succesfull[0] = false;

	return Volumes_Names;
}

//------------------------------------------------------------------------------

AnsiString Data_IO_Class::load_navx_geo_file(AnsiString FileName,
			AnsiString Selected_Geo_Name,
			Surface_Class *Surface,pugi::xml_document *Geometry_pugi_object)
{
	vector <double> XYZ_Data;
	vector <long> T_Data;
	string temp;
	int found;
	AnsiString AS;

	pugi::xml_node Volumes = Geometry_pugi_object->child("DIF").
		child("DIFBody").child("Volumes");

	for(pugi::xml_node_iterator it=Volumes.begin(); it!=Volumes.end(); ++it)
	if(  (AnsiString)it->attribute("name").value() == Selected_Geo_Name )
	{

	// reading vertices
	pugi::xml_node Vertices = it->child("Vertices");
	long Nodes_Number = Vertices.attribute("number").as_int();
//	for (pugi::xml_node child = Vertices.first_child(); child; child = Vertices.next_sibling())
//	if (child.type() == pugi::node_pcdata)
	pugi::xml_node Vchild = Vertices.first_child();

		AS = (AnsiString)Vchild.value();

		// Storing the whole string into string stream
		stringstream ss;
		ss << AS;

		// Running loop till the end of the stream
		for(long t=0;t<3*Nodes_Number;t++) // while (!ss.eof())
		{
			// extracting word by word from stream
			ss >> temp;

			XYZ_Data.push_back(atof(temp.c_str()));

			// To save from space at the end of string
			temp = "";
		}

		int fd=34;

	// reading triangles
	pugi::xml_node Polygons = it->child("Polygons");
	long Triangles_Number = Polygons.attribute("number").as_int();
	pugi::xml_node Tchild = Polygons.first_child();

	AS = (AnsiString)Tchild.value();

		// Storing the whole string into string stream
		stringstream ss2;
		ss2 << AS;

		// Running loop till the end of the stream
		for(long t=0;t<3*Triangles_Number;t++) // while (!ss.eof())
		{
			// extracting word by word from stream
			ss2 >> temp;

			T_Data.push_back(atoi(temp.c_str())-1);

			// To save from space at the end of string
			temp = "";
		}
	} // through all volumes

	//--------------------------------------------------------------------
	// POST PROCESSING
	//--------------------------------------------------------------------

	// clear objects
	Surface->Surface_Node_Set.clear();
	Surface->Surface_Triangle_Set.clear();

	long Nodes_Number = XYZ_Data.size()/3;
	Geometry_Vertex N1;

	Surface->Surface_Node_Set.assign(Nodes_Number,N1);

	long Triangles_Number = T_Data.size()/3;
	Geometry_Triangle T1;
	Surface->Surface_Triangle_Set.assign(Triangles_Number,T1);

	for(unsigned long i=0;i<Surface->Surface_Node_Set.size();i++)
	{
		Surface->Surface_Node_Set[i].x = XYZ_Data[3*i];
		Surface->Surface_Node_Set[i].y = XYZ_Data[3*i+1];
		Surface->Surface_Node_Set[i].z = XYZ_Data[3*i+2];

		Surface->Surface_Node_Set[i].Original_x = Surface->Surface_Node_Set[i].x;
		Surface->Surface_Node_Set[i].Original_y = Surface->Surface_Node_Set[i].y;
		Surface->Surface_Node_Set[i].Original_z = Surface->Surface_Node_Set[i].z;
	}

	for(unsigned long i=0;i<Surface->Surface_Triangle_Set.size();i++)
	{
		Surface->Surface_Triangle_Set[i].Nodes[0] = T_Data[3*i];
		Surface->Surface_Triangle_Set[i].Nodes[1] = T_Data[3*i+1];
		Surface->Surface_Triangle_Set[i].Nodes[2] = T_Data[3*i+2];
		Surface->Surface_Triangle_Set[i].Segment_Id = -1;
	}

	return "Ok";
}

//------------------------------------------------------------------------------

AnsiString Data_IO_Class::read_old_navx_data_file(AnsiString FileName, Data_Point_Set_Class *Data_Point_Set)
{
	return "Not implemented";
}

//------------------------------------------------------------------------------

AnsiString Data_IO_Class::read_navx_velocity_data_file(AnsiString FileName,
				Data_Point_Set_Class *Data_Point_Set, STUDY_Class *Study)
{
	Row_vec Row1;
	vector <Row_vec> Rows;
	char c;
	int Columns_To_Read;
	long signal_size;
	double v,tmpv=0,v1,v2;
	AnsiString QS;

	//----------------------------------------------------------
	// Load file into Table (using comma as items saparator)
	//----------------------------------------------------------
	ifstream dfile(FileName.c_str());

	if( !dfile.is_open() )
		return "Unable to open data file "+FileName;

	long Iterator = 0;
	while( !dfile.eof() && Iterator < 100000 )
	{

	Row1.Elements.clear();
    QS = "";

    while(c!=10 && !dfile.eof()) // till end of line
	{
		dfile.get(c);
		if( c == 44 ) // comma
		{
			Row1.Elements.push_back(QS);
            QS = "";
			dfile.get(c);
		}

		if( c!= 10)  QS += c; // end of line?
	} // reading line

	Row1.Elements.push_back(QS);
    Rows.push_back(Row1);
	c = 0;

	Iterator++;

	} // reading file

	//----------------------------------------------------------------
	// find DxL file version
	//----------------------------------------------------------------
	AnsiString TT1 = Utils.get_string_after_last_occurence_of_specified_string(
		Rows[0].Elements[0].Trim()," ");
	TT1 = TT1.SubString(1,1);

	int DXLFile_ver = TT1.ToInt();

	double Import_File_Version = TT1.ToDouble();

	if( DXLFile_ver > 5 )
		return "Unsupported DxL version. Please contact Customer Support.";

	//----------------------------------------------------------------
	// find number of columns to read
	//----------------------------------------------------------------
	long Columns_Number_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "Total number of data points (columns):")
	Columns_Number_Pos = i;
	Columns_To_Read = Rows[Columns_Number_Pos].Elements[1].Trim().ToInt();

	AnsiString MapName = Rows[13].Elements[1];

	//----------------------------------------------------------
	// get Sampling_Rate
	//----------------------------------------------------------
	double Sampling_Rate;
	long Samp_Rate_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "Sample rate:")
	Samp_Rate_Pos = i;
	Sampling_Rate = (Rows[Samp_Rate_Pos].Elements[1].Trim()).ToDouble();

	//----------------------------------------------------------
	// get signal size
	//----------------------------------------------------------
	long Sig_Size_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "Exported seconds:")
	Sig_Size_Pos = i;
	signal_size = (Rows[Sig_Size_Pos].Elements[1].Trim()).ToInt()*Sampling_Rate;

	Data_Point_Set[0].Name = MapName;

	//----------------------------------------------------------
	// set up data point
	//----------------------------------------------------------
	Data_Point D;

	long Lng=0;

	Electrogram ee;
	D.Additional_Ref_Channels.clear();
	D.Additional_Ref_Channels.assign(3,ee);

	D.Roving_Signal.Time_Step_ms = 1000.0/Sampling_Rate;
	D.Reference_Signal.Time_Step_ms = 1000.0/Sampling_Rate;
	D.ECG_Signal.Time_Step_ms = 1000.0/Sampling_Rate;
	for(int e=0;e<2;e++)
	D.Additional_Ref_Channels[e].Time_Step_ms = 1000.0/Sampling_Rate;

	// find pt number col
	long Pt_Number_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "pt number:")
	Pt_Number_Pos = i;

//********8

	// Read rov trace name position
	long Rov_Name_Pos=-1;
	for(long i=0;i<80;i++)
	if( Rov_Name_Pos == -1 && Rows[i].Elements[0].Trim() == "rov trace:")
	Rov_Name_Pos = i;

	// Read ref trace name position
	long Ref_Name_Pos=-1;
	for(long i=0;i<80;i++)
	if( Ref_Name_Pos == -1 && Rows[i].Elements[0].Trim() == "ref trace:")
	Ref_Name_Pos = i;

	// S1 trace name position
	long S1_Name_Pos=-1;
	for(long i=0;i<80;i++)
	if( S1_Name_Pos == -1 && Rows[i].Elements[0].Trim() == "S 1 trace:")
	S1_Name_Pos = i;

	// S2 trace name position
	long S2_Name_Pos=-1;
	for(long i=0;i<80;i++)
	if( S2_Name_Pos == -1 && Rows[i].Elements[0].Trim() == "S 2 trace:")
	S2_Name_Pos = i;

	// S3 trace name position
	long S3_Name_Pos=-1;
	for(long i=0;i<80;i++)
	if( S3_Name_Pos == -1 && Rows[i].Elements[0].Trim() == "S 3 trace:")
	S3_Name_Pos = i;

//********8

	// rov signal pos
	long Rov_Sig_Pos = -1;
	for(long i=Rov_Name_Pos+1;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "rov trace:")
	Rov_Sig_Pos = i;

	// ref signal pos
	long Ref_Sig_Pos = -1;
	for(long i=Ref_Name_Pos+1;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "ref trace:")
	Ref_Sig_Pos = i;

	// S1 signal pos
	long S1_Sig_Pos = -1;
	for(long i=S1_Name_Pos+1;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "spare1 trace:")
	S1_Sig_Pos = i;

	// S2 signal pos
	long S2_Sig_Pos = -1;
	for(long i=S2_Name_Pos+1;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "spare2 trace:")
	S2_Sig_Pos = i;

	// S3 signal pos
	long S3_Sig_Pos = -1;
	for(long i=S3_Name_Pos+1;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "spare3 trace:")
	S3_Sig_Pos = i;

	// S3 signal pos end
	long S3_Sig_Pos_End = -1;
	for(long i=S3_Name_Pos+1;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "FFT spectrum is available for FFT maps only")
	S3_Sig_Pos_End = i;

	// HERE LENGHTS OF SIGNALS ARE CALCULATED (assuming order is the same)
	long Ref_Signal_Lenght = (S1_Sig_Pos-2) - (Ref_Sig_Pos+1) +1;
	long S1_Signal_Lenght = (S2_Sig_Pos-2) - (S1_Sig_Pos+1) +1;
	long S2_Signal_Lenght = (S3_Sig_Pos-2) - (S2_Sig_Pos+1) +1;
	long S3_Signal_Lenght = (S3_Sig_Pos_End-1) - (S3_Sig_Pos+1) +1;

//********

	// find ref lat position
	long Ref_LAT_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "ref LAT:")
	Ref_LAT_Pos = i;

	// find rov lat position
	long Rov_LAT_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "rov LAT:")
	Rov_LAT_Pos = i;

	// find peak2peak postition
	long p2p_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "peak2peak:")
	p2p_Pos = i;

	// find peak neg pos
	long pn_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "peak neg:")
	pn_Pos = i;

	// find CAFE pos
	long CFE_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "CFE mean:")
	CFE_Pos = i;

	// find CFESD pos
	long CFESD_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "CFE stddev:")
	CFESD_Pos = i;

	// x pos
	long X_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "surfPt x:")
	X_Pos = i;

	// y pos
	long Y_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "surfPt y:")
	Y_Pos = i;

	// z pos
	long Z_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "surfPt z:")
	Z_Pos = i;

	// Read Displayed status pos
	long Disp_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "displayed:")
	Disp_Pos = i;

	// Read Utilized status pos
	long Util_Pos;
	for(long i=0;i<(signed)Rows.size();i++)
	if( Rows[i].Elements[0].Trim() == "utilized:")
	Util_Pos = i;

	//-------------------------------------------------------------
	//-------------------------------------------------------------
	// MAIN DATA READING LOOP
	//-------------------------------------------------------------
	for(long i=0;i<Columns_To_Read;i++)
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	{
		D.Identifier = (Rows[Pt_Number_Pos].Elements[i+1].Trim()).ToInt();

		//-----------------------
		// READING CHANNELS NAMES
		//-----------------------
		QS = Rows[Rov_Name_Pos].Elements[i+1].Trim();
		Utils.replace_substring_from_string(QS," ","-");
		D.Roving_Signal.Name = QS;

		QS = Rows[Ref_Name_Pos].Elements[i+1].Trim();
		if( QS != "..") // .. means it is empty
		{
			Utils.replace_substring_from_string(QS," ","-");
			D.Reference_Signal.Name = QS;
		}

		QS = Rows[S1_Name_Pos].Elements[i+1].Trim();
		if( QS != "..") // .. means it is empty
		{
			Utils.replace_substring_from_string(QS," ","-");
			D.ECG_Signal.Name = QS;
			D.Additional_Ref_Channels[0].Name = QS;
		}

		QS = Rows[S2_Name_Pos].Elements[i+1].Trim();
		if( QS != "..") // .. means it is empty
		{
			Utils.replace_substring_from_string(QS," ","-");
			D.Additional_Ref_Channels[1].Name = QS;
		}

		QS = Rows[S3_Name_Pos].Elements[i+1].Trim();
		if( QS != "..") // .. means it is empty
		{
			Utils.replace_substring_from_string(QS," ","-");
			D.Additional_Ref_Channels[2].Name = QS;
		}


		if( Import_File_Version < 5 )
		{
//		D.set_value("ref LAT",Rows[Ref_LAT_Pos].Elements[i+1].Trim().ToDouble(),
//				Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());

		D.set_value("rov LAT",Rows[Rov_LAT_Pos].Elements[i+1].Trim().ToDouble(),
				Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());
		}

		if( Import_File_Version >= 5 )
		{

		v1 = Rows[Ref_LAT_Pos].Elements[i+1].Trim().ToDouble(); // in sec
		v2 = Rows[Rov_LAT_Pos].Elements[i+1].Trim().ToDouble();

//		D.set_value("ref LAT",0,
//				Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());

		D.set_value("rov LAT",(v2-v1)*1000.0,
				Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());
		}

		D.set_value("peak2peak",Rows[p2p_Pos].Elements[i+1].Trim().ToDouble(),
			Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());

		D.set_value("peak neg",Rows[pn_Pos].Elements[i+1].Trim().ToDouble(),
			Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());

		D.set_value("CFE mean",Rows[CFE_Pos].Elements[i+1].Trim().ToDouble(),
			Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());

		D.set_value("CFE stdev",Rows[CFESD_Pos].Elements[i+1].Trim().ToDouble(),
			Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());

		D.x = (Rows[X_Pos].Elements[i+1].Trim()).ToDouble();
		D.y = (Rows[Y_Pos].Elements[i+1].Trim()).ToDouble();
		D.z = (Rows[Z_Pos].Elements[i+1].Trim()).ToDouble();

		D.Original_x = D.x;
		D.Original_y = D.y;
		D.Original_z = D.z;

		D.Displayed = Rows[Disp_Pos].Elements[i+1].Trim().ToInt();
		D.Utilized =  Rows[Util_Pos].Elements[i+1].Trim().ToInt();

		//0-----------------------------------
		// Reading roving signal
		//0-----------------------------------
		D.Roving_Signal.Voltage_Values.clear();
		if( Rov_Sig_Pos > 20 )
		for(long j=1;j<signal_size-1;j++)
		{
			if( Rows[Rov_Sig_Pos+j].Elements[i+1].Trim() != "")
			v =(Rows[Rov_Sig_Pos+j].Elements[i+1].Trim()).ToDouble();

			D.Roving_Signal.Voltage_Values.push_back(v);
		}

		//0-----------------------------------
		// Reading reference signal
		//0-----------------------------------
		D.Reference_Signal.Voltage_Values.clear();
		if( Ref_Sig_Pos > 70 )
		if( Ref_Signal_Lenght > 100)
		if( Rows[Ref_Sig_Pos].Elements[1].Trim() != "..") // if not empty
		for(long j=1;j<Ref_Signal_Lenght;j++)
		{
			if( Rows[Ref_Sig_Pos+j].Elements[i+1].Trim() != "")
			v =(Rows[Ref_Sig_Pos+j].Elements[i+1].Trim()).ToDouble();

			D.Reference_Signal.Voltage_Values.push_back(v);
		}

		//0-----------------------------------
		// Reading S1/ECG signal from Spare trace
		//0-----------------------------------
		D.Additional_Ref_Channels[0].Voltage_Values.clear();
		D.ECG_Signal.Voltage_Values.clear();
		if( S1_Sig_Pos > 70 )
		if( S1_Signal_Lenght > 100 )
		if( Rows[S1_Sig_Pos].Elements[1].Trim() != "..") // if not empty
		for(long j=1;j<S1_Signal_Lenght;j++)
		{
			if( Rows[S1_Sig_Pos+j].Elements[i+1].Trim() != "")
			v =(Rows[S1_Sig_Pos+j].Elements[i+1].Trim()).ToDouble();

			D.Additional_Ref_Channels[0].Voltage_Values.push_back(v);
			D.ECG_Signal.Voltage_Values.push_back(v);
		}

		//0-----------------------------------
		// Reading S2 signal from Spare trace
		//0-----------------------------------
		D.Additional_Ref_Channels[1].Voltage_Values.clear();
		if( S2_Sig_Pos > 70 )
		if( S2_Signal_Lenght > 100 )
		if( Rows[S2_Sig_Pos].Elements[1].Trim() != "..") // if not empty
		for(long j=1;j<S2_Signal_Lenght;j++)
		{
			if( Rows[S1_Sig_Pos+j].Elements[i+1].Trim() != "")
			v =(Rows[S1_Sig_Pos+j].Elements[i+1].Trim()).ToDouble();

			D.Additional_Ref_Channels[1].Voltage_Values.push_back(v);
		}

		//0-----------------------------------
		// Reading S3 signal from Spare trace
		//0-----------------------------------
		D.Additional_Ref_Channels[2].Voltage_Values.clear();
		if( S3_Sig_Pos > 70 )
		if( S3_Signal_Lenght > 100 )
		if( Rows[S3_Sig_Pos].Elements[1].Trim() != "..") // if not empty
		for(long j=1;j<S3_Signal_Lenght;j++)
		{
			if( Rows[S3_Sig_Pos+j].Elements[i+1].Trim() != "")
			v =(Rows[S3_Sig_Pos+j].Elements[i+1].Trim()).ToDouble();

			D.Additional_Ref_Channels[2].Voltage_Values.push_back(v);
		}

	// set position of ref and rov bars
	D.Ref_Signal_Activation_ptr = signal_size*0.5;
	double tt = signal_size*0.5 + (D.get_value("Local Activation Time",
		Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref() ))*
		Sampling_Rate/1000.;

	if( tt > 0 && tt < signal_size )
	D.Rov_Signal_Activation_ptr = tt;

	// finally, push D to the list (if name is valid)
	if( D.Roving_Signal.Name != "..")
	if( D.Utilized == 1 ) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Data_Point_Set[0].Data_Points.push_back(D);

	} // through all columns


	dfile.close();

	return "Ok";

}

//-----------------------------------------------------------------------------------------------

AnsiString Data_IO_Class::import_CARTO_folder(TFileListBox* Data_FileListBox,
				STUDY_Class *Study,TProgress_Form *Progress_Form, bool Import_All_ECG_Channels_Flag,
				bool Import_PASO_data, bool CS_As_Ref, bool All_Maps_Import, bool Append, bool Geoff_Export_Flag )
{
	std::vector <AnsiString> All_Files_Names;
	std::vector <AnsiString> Mesh_File_Names;
	std::vector <int> Mesh_File_Ptrs;

	Study->Study_Source_Path = Data_Files_Path;

	//---------------------------------------
	// prepare files names list
	//---------------------------------------
	for(long i=0; i < Data_FileListBox->Items[0].Capacity; i++ )
		All_Files_Names.push_back( Data_FileListBox->Items[0].Strings[i] );

	Progress_Form->add_text(IntToStr((int)All_Files_Names.size() ) + " files in the folder");
	Progress_Form->Show();
	Application->ProcessMessages();

	//---------------------------------------
	// find mesh files
	//---------------------------------------
	for( int i=0; i < All_Files_Names.size(); i++ )
	if( Utils.is_substring_present( All_Files_Names[i],".mesh") )
	{
		Mesh_File_Names.push_back( All_Files_Names[i] );
		Mesh_File_Ptrs.push_back(i);
	}

	if( Mesh_File_Names.size() == 0 )
	return "No .mesh files found. Terminating import. Go to www.eplabworks.com/help and find the problem in FAQ section.";

	// clear surfaces
	if( !Append )
	Study->Surfaces_List.clear();

	//--------------------------------------------------
	// prepare names of maps with data points numbers
	//--------------------------------------------------
	std::vector <AnsiString> Mesh_File_Names_With_DPs_Number;
	char st2[1];
	st2[0] = 'P';
	char string[20000];
	int Data_Points_No = 0;
	ifstream dfcar;
	for(int dcar=0;dcar<Mesh_File_Names.size();dcar++)
	{
		dfcar.open((Utils.replace_substring_from_string(Mesh_File_Names[dcar],".mesh","_car.txt")).c_str());

		if(dfcar.is_open())
		{
		while(!dfcar.eof())
		{
			// read line
			dfcar.getline(string,2000);
			if(string[0]==st2[0])
			Data_Points_No++;
		}

		Mesh_File_Names_With_DPs_Number.push_back(Mesh_File_Names[dcar]+"   (data points number: " + IntToStr(Data_Points_No)+")");
		}
		else
		Mesh_File_Names_With_DPs_Number.push_back(Mesh_File_Names[dcar]+" car file not found!");

		dfcar.close();
	}


	if( !All_Maps_Import )
	{

	Selection_List_Form->Caption = "Select geometry to load";
	Selection_List_Form->fill_selection(&Mesh_File_Names_With_DPs_Number);

	if( !Selection_List_Form->ShowModal() )
	return "Import cancelled";

	if( Selection_List_Form->Selected_Item_Ptr == -1 )
	return "No geometry to import selected";

	AnsiString Selected_Geometry;
	if( Selection_List_Form->Selected_Item_Ptr >= 0 &&
		Selection_List_Form->Selected_Item_Ptr < Mesh_File_Names.size() )
	Selected_Geometry = Mesh_File_Names[Selection_List_Form->Selected_Item_Ptr];
	else
	return "Selected geometry pointer out of range";

	Study->Study_Current_Path_And_File_Name =
		Utils.get_string_before_last_occurence_of_specified_string(Data_Files_Path,"\\") + "\\" +
		Utils.get_string_after_last_occurence_of_specified_string(Data_Files_Path,"\\") + " " +
			Selected_Geometry + ".eplab";

	return import_single_CARTO_map(Data_FileListBox,Selected_Geometry, All_Files_Names,
				Study,Progress_Form, Import_All_ECG_Channels_Flag,Import_PASO_data,
				CS_As_Ref, All_Maps_Import,Geoff_Export_Flag);

	}
	else
	{

	Study->Study_Current_Path_And_File_Name =
		Utils.get_string_before_last_occurence_of_specified_string(Data_Files_Path,"\\") + "\\" +
		Utils.get_string_after_last_occurence_of_specified_string(Data_Files_Path,"\\") + " ALL maps.eplab";

	for(int m=0;m<Mesh_File_Names.size();m++)
	import_single_CARTO_map(Data_FileListBox,Mesh_File_Names[m],All_Files_Names,
				Study,Progress_Form, Import_All_ECG_Channels_Flag,Import_PASO_data,
				CS_As_Ref, All_Maps_Import,Geoff_Export_Flag);

	return "Import completed";
	}
}

//-----------------------------------------------------------------------------------------------

AnsiString Data_IO_Class::import_single_CARTO_map(TFileListBox* Data_FileListBox,
				AnsiString Mesh_File_Name,std::vector <AnsiString> All_Files_Names,
				STUDY_Class *Study,TProgress_Form *Progress_Form, bool Import_All_ECG_Channels_Flag,
				bool Import_PASO_data, bool CS_As_Ref, bool All_Maps_Import, bool Geoff_Export_Flag)
{
	char String[20000];
	char string[20000];
	long TInt;
	AnsiString AS;
	int id,Data_Points_No,counter;
	char st2[1];
	st2[0] = 'P';
	int cnt,Columns_Number;
	char Line[20000],Info_Line[20000];

	int Rov_Id,ECG_Id,Uni1_Id,Uni2_Id,Ref1_Id,Ref2_Id,Ref3_Id;
	AnsiString Rov_Name,Uni1_Name,Uni2_Name,Ref1_Name,ECG_Name,Ref2_Name,Ref3_Name;
	int ECG_Ids[12];

	AnsiString Signals_Names[100];
	int ECG_file_version;
	bool tmpbool;
	double v=0;

	AnsiString FileNameBase_For_Geometry = Utils.get_string_before_given_text(Mesh_File_Name,".mesh");
	AnsiString FileNameBase = Utils.get_string_before_given_text(Mesh_File_Name,".mesh")+"_";

	// --------------------------------------------------------------------
	// 1. Import mesh file (geometry)
	// --------------------------------------------------------------------
	Progress_Form->add_text("Importing map: " + FileNameBase_For_Geometry);
	Progress_Form->add_text("Importing geometry...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Surface_Class Suface_Class_Item;
	Study->Surfaces_List.push_back(Suface_Class_Item);
	Study->Current_Surface = Study->Surfaces_List.size()-1;
	Study->Surfaces_List[Study->Current_Surface].Surface_Node_Set.clear();
	Study->Surfaces_List[Study->Current_Surface].Surface_Triangle_Set.clear();
	Study->Surfaces_List[Study->Current_Surface].Name = FileNameBase_For_Geometry;
	Study->Surfaces_List[Study->Current_Surface].Mapping_System_Source = MAPPING_SYSTEM_ORIGIN_CARTO;

//	for( int i=0; i < All_Files_Names.size(); i++ )
//	if( Utils.is_substring_present( All_Files_Names[i],".mesh") )
//	if( Utils.is_substring_present( All_Files_Names[i],FileNameBase_For_Geometry) )
//	{
		Study->Surfaces_List[Study->Current_Surface].Surface_Node_Set.clear();
		Study->Surfaces_List[Study->Current_Surface].Surface_Triangle_Set.clear();

		ifstream df;
		df.open(Mesh_File_Name.c_str()); //		df.open(All_Files_Names[i].c_str());

		df >> String;

		if( strcmp( String,"#TriangulatedMeshVersion2.0") )
		{
			ShowMessage("Unsupported version of the mesh file.");
			return "Unsupported version of the mesh file.";
		}

		long Nodes_Number, Triangles_Number;

		while(  strcmp(String,"NumVertex" ) && !df.eof() )
			df >> String;
		df >> String; // =
		df >> Nodes_Number;
		Geometry_Vertex N1;
//		Study->Surfaces_List[Study->Current_Surface].Surface_Node_Set.assign(TInt,N1);

		while(  strcmp(String,"NumTriangle" ) && !df.eof() )
			df >> String;
		df >> String; // =
		df >> Triangles_Number;
		Geometry_Triangle T1;
//		Study->Surfaces_List[Study->Current_Surface].Surface_Triangle_Set.assign(TInt,T1);

		// read nodes
		while(  strcmp(String,"[VerticesSection]" ) && !df.eof() )
			df >> String;
		for(long k=0;k<8;k++) df >> String;

		for(long j=0;j<Nodes_Number;j++)
		{
			df >> N1.Identifier;
			df >> String;
			df >> N1.x;
			df >> N1.y;
			df >> N1.z;
			N1.Original_x = N1.x;
			N1.Original_y =N1.y;
			N1.Original_z =N1.z;
			df >> N1.normal[0];
			df >> N1.normal[1];
			df >> N1.normal[2];
			df >> N1.GroupId;

			if( N1.GroupId != -1000000 )
			Study->Surfaces_List[Study->Current_Surface].Surface_Node_Set.push_back(N1);
		}

		// read triangles
		while(  strcmp(String,"[TrianglesSection]" ) && !df.eof() )
			df >> String;
		for(long k=0;k<8;k++) df >> String;

		for(long j=0;j<Triangles_Number;j++)
		{
			df >> String;
			df >> String;
			df >> T1.Nodes[0];
			df >> T1.Nodes[1];
			df >> T1.Nodes[2];
			df >> T1.normal[0];
			df >> T1.normal[1];
			df >> T1.normal[2];
			df >> T1.GroupId;

			if( T1.GroupId != -1000000 )
			Study->Surfaces_List[Study->Current_Surface].Surface_Triangle_Set.push_back(T1);
		}

		df.close();
//	}

	// --------------------------------------------------------------------
	// --------------------------------------------------------------------
	// --------------------------------------------------------------------
	// 2. Import car file (data points)
	// --------------------------------------------------------------------
	// --------------------------------------------------------------------
	// --------------------------------------------------------------------
	Progress_Form->add_text("Importing '*car.txt' file...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//-----------------------------------------------------
	// 2.a calculate how many data points there are
	//-----------------------------------------------------

	// find car file
	id = -1;
	for( int i=0; i < All_Files_Names.size(); i++ )
	{
	if( Utils.is_substring_present( All_Files_Names[i],"_car.txt") )
	if( Utils.is_substring_present( All_Files_Names[i],FileNameBase) )
	id=i;
	}

	if( id== -1 )
	   return "'*car.txt' file not found. Terminating import.";

	df.open(All_Files_Names[id].c_str());
	df.getline(string,2000);


	int Car_File_Version=0;
	if( Utils.is_substring_present( (AnsiString)string,"VERSION_4_0")) Car_File_Version = 4;
	if( Utils.is_substring_present( (AnsiString)string,"VERSION_5_0")) Car_File_Version = 5;
	if( Utils.is_substring_present( (AnsiString)string,"VERSION_6_0")) Car_File_Version = 6;
	if( Car_File_Version == 0 )
	{
		ShowMessage("Unsupported '*car.txt' file version. Contact support.");
		return "Unsupported '*car.txt' file version. Contact support.";
	}

	Data_Points_No = 0;
	while(!df.eof())
	{
		// read line
		df.getline(string,2000);
		if(string[0]==st2[0])
			Data_Points_No++;
	}

	df.clear();
	df.close();
	df.clear();

	//-----------------------------------------------------
	// 2.b allocate data points list
	//-----------------------------------------------------
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set.clear();
	Data_Point_Set_Class DS;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set.push_back(DS);
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points.clear();
	Data_Point DP;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points.assign(Data_Points_No,DP);

	// allocating ref channels (1 is 2nd ref, 2,3: for Geoff 20B poles which are HIS in his study)
	Electrogram ee;
	for(long d=0;d<Data_Points_No;d++)
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[d].
		Additional_Ref_Channels.assign(3,ee);

	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Name = "Base data points set";

   //-----------------------------------------------------
   // 2.c Read car file
   //-----------------------------------------------------
   df.open(All_Files_Names[id].c_str());

   long Dp_ptr=0;

   Value_Description_Class Value_Desc;

   Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS; // based on data points
   Value_Desc.Displayed_In_Table = true;

   Value_Desc.Value_Name = "Unipolar voltage";
   Value_Desc.Unit = "mV";
   Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);

   Value_Desc.Value_Name = "Bipolar voltage";
   Value_Desc.Unit = "mV";
   Value_Desc.Voltage_Amplitude_Value = true;
   Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);
   Value_Desc.Voltage_Amplitude_Value = false;

   Value_Desc.Value_Name = "Local Activation Time";
   Value_Desc.Unit = "ms";
   Value_Desc.LAT_Value = true;
   Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);
   Value_Desc.LAT_Value = false;

   Value_Desc.Value_Name = "Impedance";
   Value_Desc.Unit = "Ohm";
   Study->Surfaces_List[Study->Current_Surface].Map_Values.add_value(Value_Desc);

   df.getline(Line,20000); // read header line

   for(long d=0;d<Data_Points_No;d++)
   {
// xxx QT
	df.getline(Line,20000); // read line
	std::istringstream istringstream_Line(Line); // cast to istringstream

   istringstream_Line >> string; // "P"

   if(strcmp( string, "P")==0) // begining of the line
   {
	istringstream_Line >> string;
	istringstream_Line >> string;  Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].Identifier = atoi(string);
	istringstream_Line >> string;

	// x,y,z
	istringstream_Line >> string;  Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].x = atof(string);
	istringstream_Line >> string;  Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].y = atof(string);
	istringstream_Line >> string;  Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].z = atof(string);

	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].Original_x =
			 Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].x;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].Original_y =
			 Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].y;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].Original_z =
			 Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].z;

	// angular position variables, ignored
	istringstream_Line >> string;
	istringstream_Line >> string;
	istringstream_Line >> string;

	//  values UNI, BI and LAT
	istringstream_Line >> string;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].set_value(
		"Unipolar voltage", atof(string), Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());
	istringstream_Line >> string;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].set_value("Bipolar voltage", atof(string), Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());
	istringstream_Line >> string;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
		Data_Points[Dp_ptr].set_value("Local Activation Time", atof(string),
		Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());

	// set position of roving annotation bar
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].Ref_Signal_Activation_ptr = 1250;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].Rov_Signal_Activation_ptr =
	  1250+
	  Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].
		get_value("Local Activation Time",
			Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());

   istringstream_Line >> string;  // -10000 ? Impedance?
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].set_value(
		"Impedance", atof(string), Study->Surfaces_List[Study->Current_Surface].Map_Values.get_values_table_ref());

   // POINT TYPE
   istringstream_Line >> string;  // point label id (defined in "predefined paramets.h" CARTO_LABELS)
   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].Type = atoi(string);

   // LABEL ID
   istringstream_Line >> string;  // point type (defined in "predefined paramets.h" CARTO_TYPES)
   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Dp_ptr].LabelId = atoi(string);

   /* stuff below not used

   df >> string;  // point label size
   df >> string;  // point label
   df >> string;  // label color
   df >> string;  // T: catheter id
   df >> string;  // U: not used

   // read additions in v.5
   if( Car_File_Version >= 5 )
   {
	   df >> string;  // V: Wavefront annotation LAT timing
   }

   // read additions in v.6
   if( Car_File_Version >= 6 )
   {
	   df >> string;  // W: pattern matching v
	   df >> string;  // X: pattern matching v
	   df >> string;  // Y: pattern matching v
   }
*/

   Dp_ptr ++;

   } // line reading

   } // throug all dps

  df.clear();
  df.close();
  df.clear();

	//-----------------------------------------------------
	//-----------------------------------------------------
	//-----------------------------------------------------
	// 3. Load singals
	//-----------------------------------------------------
	//-----------------------------------------------------
	//-----------------------------------------------------
	Progress_Form->add_text("Importing files with electrograms...");
	Progress_Form->Show();
	Application->ProcessMessages();

	// go through all ECG files
	int Corr_DP;
	long Counter=0;
	for( int i=0; i < All_Files_Names.size(); i++ )
	if( Utils.is_substring_present( All_Files_Names[i],FileNameBase) )
	if( Utils.is_substring_present( All_Files_Names[i],"_ECG_Export.txt") )
	{
		// find which data point ptr is corresponding with given point
		for(long kk=0;kk<Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
			Data_Points.size();kk++)
		if( Utils.is_substring_present( All_Files_Names[i],
		 "_P"+IntToStr(Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
			Data_Points[kk].Identifier)+"_") )
		Corr_DP = kk;

		ifstream df;
		df.open(All_Files_Names[i].c_str());
		df.getline(Line,20000); // ver

	ECG_file_version = -1;
	if( Utils.is_substring_present( (AnsiString)Line,"ECG_Export_3.0") )
	ECG_file_version = 3;

	if( Utils.is_substring_present( (AnsiString)Line,"ECG_Export_4.0") )
	ECG_file_version = 4;

	if( ECG_file_version == -1 || ECG_file_version == 3 )
		return "Unsupported ECG file version: '" + (AnsiString)Line + "' Contact Support.";

	//---------------------------------------------------------
	if( ECG_file_version == 4 )
	//---------------------------------------------------------
	{

	Counter++;
	Progress_Form->replace_last_line_with_text("Importing electrogram file "+IntToStr((int)Counter+1)+
			"/"+IntToStr((int)Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
			Data_Points.size()+1));
	Progress_Form->Show();
	Application->ProcessMessages();

	// reading gain
	df.getline(Line,20000); // gain
	std::istringstream istringstream_Line(Line);
	istringstream_Line >> String;
	while( strcmp(String,"=" ) && !df.eof() )
		istringstream_Line >> String;
	istringstream_Line >> String;
	double Channel_Gain = ((AnsiString)(String)).ToDouble();
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Corr_DP].Roving_Signal.Time_Step_ms = 1;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Corr_DP].Reference_Signal.Time_Step_ms = 1;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Corr_DP].Reference_Signal_2.Time_Step_ms = 1;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Corr_DP].ECG_Signal.Time_Step_ms = 1;
	for(int e=0;e<3;e++)
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Corr_DP].Additional_Ref_Channels[e].Time_Step_ms = 1;
	for(int e=0;e<12;e++)
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[Corr_DP].All_ECG_Channels[e].Time_Step_ms = 1;

	df.getline(Info_Line,20000); // which is uni and which bipolar

	// find names of channels to read
	if( !CS_As_Ref )
	{
	Uni1_Name = Utils.get_string_between_two_texts(Info_Line,"Unipolar Mapping Channel=","Bipolar Mapping Channel=",&tmpbool).Trim()+"(";
	Uni1_Name = Uni1_Name.SubString(2,Uni1_Name.Length()-1);
	}
	else
	{
	Uni1_Name = "CS1-CS2";
	}

	Rov_Name = Utils.get_string_between_two_texts(Info_Line,"Bipolar Mapping Channel=", "Reference Channel=",&tmpbool).Trim()+"(";
	Ref1_Name = Uni1_Name.SubString(1,Uni1_Name.Length()-2)+Rov_Name.SubString(Rov_Name.Length()-1,1)+"(";
	Uni2_Name = Ref1_Name; // is this always correct?
	ECG_Name =  Utils.get_string_after_last_occurence_of_specified_string(Info_Line,"=")+"(";

	if( Geoff_Export_Flag ) // he had fixed channnels he wanted to be exported
	{
	Ref2_Name = "20B_1-2(";
	Ref3_Name = "20B_3-4(";
	}
	else
	{
	Ref2_Name = "20B_1-2("; // ? not sure what to put here....
	Ref3_Name = "20B_3-4(";
	}

	// find V2 name as reference channel
	df.getline(Line,20000);

	// assign names
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
		Data_Points[Corr_DP].Roving_Signal.Name = Rov_Name;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
		Data_Points[Corr_DP].Reference_Signal.Name = Uni1_Name;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
		Data_Points[Corr_DP].Reference_Signal_2.Name = Utils.remove_substring_from_string(Uni2_Name,"(");
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
		Data_Points[Corr_DP].ECG_Signal.Name = ECG_Name;

	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
		Data_Points[Corr_DP].Additional_Ref_Channels[0].Name = Ref1_Name;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
		Data_Points[Corr_DP].Additional_Ref_Channels[1].Name = Ref2_Name;
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
		Data_Points[Corr_DP].Additional_Ref_Channels[2].Name = Ref3_Name;
	// names of 12 ecg channels are assigned in data point contstructor

	// find positions of signals
	std::istringstream istringstream_Line3(Line);
	Columns_Number = 0;
	while( istringstream_Line3 >> String )
	{
		if( Utils.is_substring_present((AnsiString)String, Rov_Name ) &&
			Rov_Name.Length() > 0.5* strlen(String) )
		Rov_Id = Columns_Number;

		if( Utils.is_substring_present((AnsiString)String,Uni1_Name ) &&
			Uni1_Name.Length() > 0.5* strlen(String) )
		Uni1_Id = Columns_Number;

		if( Utils.is_substring_present((AnsiString)String,Uni2_Name ) &&
			Uni2_Name.Length() > 0.5* strlen(String) )
		Uni2_Id = Columns_Number;

		if( Utils.is_substring_present((AnsiString)String,Ref1_Name ) &&
			Ref1_Name.Length() > 0.5* strlen(String) )
		Ref1_Id = Columns_Number;

		if( Utils.is_substring_present((AnsiString)String,Ref2_Name ) &&
			Ref2_Name.Length() > 0.5* strlen(String) )
		Ref2_Id = Columns_Number;

		if( Utils.is_substring_present((AnsiString)String,Ref3_Name ) &&
			Ref3_Name.Length() > 0.5* strlen(String) )
		Ref3_Id = Columns_Number;

		if( Utils.is_substring_present((AnsiString)String,ECG_Name))
		ECG_Id = Columns_Number;

		if(  Utils.is_substring_present((AnsiString)String,"I(") &&
			!Utils.is_substring_present((AnsiString)String,"II(") &&
			!Utils.is_substring_present((AnsiString)String,"III("))
		ECG_Ids[0] = Columns_Number;
		if(  Utils.is_substring_present((AnsiString)String,"II(") &&
			!Utils.is_substring_present((AnsiString)String,"III("))
		ECG_Ids[1] = Columns_Number;
		if(  Utils.is_substring_present((AnsiString)String,"III("))
		ECG_Ids[2] = Columns_Number;

		if( Utils.is_substring_present((AnsiString)String,"aVL"))
		ECG_Ids[3] = Columns_Number;
		if( Utils.is_substring_present((AnsiString)String,"aVR"))
		ECG_Ids[4] = Columns_Number;
		if( Utils.is_substring_present((AnsiString)String,"aVF"))
		ECG_Ids[5] = Columns_Number;
		if( Utils.is_substring_present((AnsiString)String,"V1"))
		ECG_Ids[6] = Columns_Number;
		if( Utils.is_substring_present((AnsiString)String,"V2"))
		ECG_Ids[7] = Columns_Number;
		if( Utils.is_substring_present((AnsiString)String,"V3"))
		ECG_Ids[8] = Columns_Number;
		if( Utils.is_substring_present((AnsiString)String,"V4"))
		ECG_Ids[9] = Columns_Number;
		if( Utils.is_substring_present((AnsiString)String,"V5"))
		ECG_Ids[10] = Columns_Number;
		if( Utils.is_substring_present((AnsiString)String,"V6"))
		ECG_Ids[11] = Columns_Number;

		Signals_Names[Columns_Number]=String;
		Columns_Number++;
	}

	// load signals
	for(long Row=0;Row<2500;Row++)
	for(long Col=0;Col<Columns_Number;Col++)
	{
	   df >> v;

	   if( Col == Rov_Id )
	   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
	   Data_Points[Corr_DP].Roving_Signal.Voltage_Values.push_back(v*Channel_Gain);

	   if( Col == Uni1_Id )
	   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
	   Data_Points[Corr_DP].Reference_Signal.Voltage_Values.push_back(v*Channel_Gain);

	   if( Col == Uni2_Id )
	   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
	   Data_Points[Corr_DP].Reference_Signal_2.Voltage_Values.push_back(v*Channel_Gain);

	   if( Col == Ref1_Id )
	   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
	   Data_Points[Corr_DP].Additional_Ref_Channels[0].Voltage_Values.push_back(v*Channel_Gain);

	   if( Col == Ref2_Id )
	   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
	   Data_Points[Corr_DP].Additional_Ref_Channels[1].Voltage_Values.push_back(v*Channel_Gain);

	   if( Col == Ref3_Id )
	   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
	   Data_Points[Corr_DP].Additional_Ref_Channels[2].Voltage_Values.push_back(v*Channel_Gain);

	   if( Col == ECG_Id )
	   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
	   Data_Points[Corr_DP].ECG_Signal.Voltage_Values.push_back(v*Channel_Gain);

	   if( Import_All_ECG_Channels_Flag )
	   for(int e=0;e<12;e++)
	   if( Col == ECG_Ids[e] )
	   Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].
	   Data_Points[Corr_DP].All_ECG_Channels[e].Voltage_Values.push_back(v*Channel_Gain);
	}

	} // ver 4

	// clear "(" from cath names
	for(long dp=0;dp<Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points.size();dp++)
	{
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[dp].Roving_Signal.Name
	=	Utils.remove_substring_from_string(
		Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[dp].Roving_Signal.Name,"(");

	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[dp].Reference_Signal.Name
	=	Utils.remove_substring_from_string(
		Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[dp].Reference_Signal.Name,"(");

	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[dp].ECG_Signal.Name
	=	Utils.remove_substring_from_string(
		Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[dp].ECG_Signal.Name,"(");

	for(int e=0;e<3;e++)
	Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[dp].Additional_Ref_Channels[e].Name
	=	Utils.remove_substring_from_string(
		Study->Surfaces_List[Study->Current_Surface].Data_Point_Set[0].Data_Points[dp].Additional_Ref_Channels[e].Name,"(");

	}

	} // loading signals

	//-------------------------------------------------------------------
	//-------------------------------------------------------------------
	//-------------------------------------------------------------------
	// 4. IMPORTING ABLATION SITES
	//-------------------------------------------------------------------
	//-------------------------------------------------------------------
	//-------------------------------------------------------------------
	Ablation_Point_Class ABL_Point;
	ifstream abl_df;

	Progress_Form->add_text("Searching for ablation sites file 'VisiTagExport\\Sites.txt'...");
	Progress_Form->Show();
	Application->ProcessMessages();

//	AnsiString SitesFilePath = Data_FileListBox->Directory + "\\" + "VisiTagExport\\Sites.txt";
	AnsiString SitesFilePath = Study->Study_Source_Path + "\\" + "VisiTagExport\\Sites.txt";


	abl_df.open(SitesFilePath.c_str());

	if( abl_df.is_open() )
	{

	Study->Surfaces_List[Study->Current_Surface].Ablation_Points_List.clear();

	int Pass = 0;
	abl_df >> string; if( !strcmp(string,"Session") ) Pass++;
	abl_df >> string; if( !strcmp(string,"ChannelID") ) Pass++;
	abl_df >> string; if( !strcmp(string,"SiteIndex") ) Pass++;
	abl_df >> string; if( !strcmp(string,"X") ) Pass++;
	abl_df >> string; if( !strcmp(string,"Y") ) Pass++;
	abl_df >> string; if( !strcmp(string,"Z") ) Pass++;
	abl_df >> string; if( !strcmp(string,"DurationTime") ) Pass++;
	abl_df >> string; if( !strcmp(string,"AverageForce") ) Pass++;
	abl_df >> string; if( !strcmp(string,"MaxTemperature") ) Pass++;
	abl_df >> string; if( !strcmp(string,"MaxPower") ) Pass++;
	abl_df >> string; if( !strcmp(string,"BaseImpedance") ) Pass++;
	abl_df >> string; if( !strcmp(string,"ImpedanceDrop") ) Pass++;
	abl_df >> string; if( !strcmp(string,"FTI") ) Pass++;
	abl_df >> string; if( !strcmp(string,"RFIndex") ) Pass++;
	abl_df >> string; if( !strcmp(string,"TagIndexStatus") ) Pass++;

	if( Pass == 14 )
	{

	Counter = 0;
	while( !abl_df.eof() && Counter < 10000)
	{
		if( Counter == 0 ) // becuase first data item was read checking headers
		{
			ABL_Point.Session = atof(string); // session
		}
		else
		{
			abl_df >> v;
			ABL_Point.Session = v; // session
		}

		abl_df >> v; ABL_Point.ChannelID = v; // channel id
		abl_df >> v; ABL_Point.SiteIndex = v; // site index
		abl_df >> v; ABL_Point.x = v; // x
		abl_df >> v; ABL_Point.y = v; // y
		abl_df >> v; ABL_Point.z = v; // z
		abl_df >> v; ABL_Point.DurationTime = v; // duration time
		abl_df >> v; ABL_Point.AverageForce = v;
		abl_df >> v; ABL_Point.MaxTemperature = v;
		abl_df >> v; ABL_Point.MaxPower = v;
		abl_df >> v; ABL_Point.BaseImpedance = v;
		abl_df >> v; ABL_Point.ImpedanceDrop = v;
		abl_df >> v; ABL_Point.FTI = v;
		abl_df >> v; ABL_Point.RFIndex = v;

		if( !abl_df.eof() )
		Study->Surfaces_List[Study->Current_Surface].Ablation_Points_List.push_back(ABL_Point);

		Counter++;
	}

	}
	else
	if( Pass == 15 )
	{

	Counter = 0;
	while( !abl_df.eof() && Counter < 10000)
	{
		abl_df >> v; ABL_Point.Session = v; // session
		abl_df >> v; ABL_Point.ChannelID = v; // channel id
		abl_df >> v; ABL_Point.SiteIndex = v; // site index
		abl_df >> v; ABL_Point.x = v; // x
		abl_df >> v; ABL_Point.y = v; // y
		abl_df >> v; ABL_Point.z = v; // z
		abl_df >> v; ABL_Point.DurationTime = v; // duration time
		abl_df >> v; ABL_Point.AverageForce = v;
		abl_df >> v; ABL_Point.MaxTemperature = v;
		abl_df >> v; ABL_Point.MaxPower = v;
		abl_df >> v; ABL_Point.BaseImpedance = v;
		abl_df >> v; ABL_Point.ImpedanceDrop = v;
		abl_df >> v; ABL_Point.FTI = v;
		abl_df >> v; ABL_Point.RFIndex = v;
		abl_df >> v; ABL_Point.TagIndexStatus = v;

		if( !abl_df.eof() )
		Study->Surfaces_List[Study->Current_Surface].Ablation_Points_List.push_back(ABL_Point);

		Counter++;
	}

	}
	else
	ShowMessage("Unknown file format or other error occurred. E155");


	abl_df.close();

		Progress_Form->add_text( IntToStr((int)Study->Surfaces_List[Study->Current_Surface].
			Ablation_Points_List.size()) + " ablation points loaded");
		Progress_Form->Show();
		Application->ProcessMessages();

	}
	else
	{
		Progress_Form->add_text("'WiseTagExport\\Sites.txt' file not found.");
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	//-------------------------------------------------------------------
	//-------------------------------------------------------------------
	//-------------------------------------------------------------------
	// 5. IMPORTING PASO POINTS
	//-------------------------------------------------------------------
	//--------------ATTENTION! DATA_FILES_LIST_BOX IS CHANGED HERE!!!!---
	//-------------------------------------------------------------------
	if( Import_PASO_data )
	{

	ifstream psdf;

	Progress_Form->add_text("Searching for PASO folder...");
	Progress_Form->Show();
	Application->ProcessMessages();

//	AnsiString PASO_FilePath = Data_FileListBox->Directory + "\\" + "PaSoExport";
	AnsiString PASO_FilePath = Study->Study_Source_Path + "\\" + "PaSoExport";

	if( DirectoryExists(PASO_FilePath) ) // directory exists
	{

	Data_FileListBox->ApplyFilePath(PASO_FilePath); // <-------------!!!!------------------------
	Data_FileListBox->Update();

	// PASO points are stored in separate data points sets
	// (PASO_IS_Point_Set and PASO_PM_Point_Set)
	Data_Point DP2;

	//---------------------------------------
	// 0. Prepare all filenames list
	//---------------------------------------
	std::vector <AnsiString> All_Files_Names;
	for( int i=0; i < Data_FileListBox->Items[0].Capacity; i++ )
	All_Files_Names.push_back(
		Utils.get_file_name_from_full_path(Data_FileListBox->Items[0].Strings[i] ) );

	//---------------------------------------
	// 1. prepare PM and IS files names list
	//---------------------------------------
	std::vector <AnsiString> PM_Files_Names;
	std::vector <AnsiString> IS_Files_Names;

	for( int i=0; i < All_Files_Names.size(); i++ )
	if( Utils.is_substring_present( All_Files_Names[i],"IS") &&
		!Utils.is_substring_present( All_Files_Names[i],"Corr")) // to ignore ISISCorrelations.txt file
	IS_Files_Names.push_back( All_Files_Names[i] );

	for( int i=0; i < All_Files_Names.size(); i++ )
	if( Utils.is_substring_present( All_Files_Names[i],"PM") &&
		!Utils.is_substring_present( All_Files_Names[i],"Corr")) // to ignore PMISCorrelations.txt file
	PM_Files_Names.push_back( All_Files_Names[i] );

	Progress_Form->add_text(IntToStr((int)PM_Files_Names.size() ) + " PM files in the folder");
	Progress_Form->add_text(IntToStr((int)IS_Files_Names.size() ) + " IS files in the folder");
	Progress_Form->Show();
	Application->ProcessMessages();

	//-----------------------------------------------------
	// 2. Load IS singals
	//-----------------------------------------------------
	Progress_Form->add_text("Importing IS files ...");
	Progress_Form->Show();
	Application->ProcessMessages();

	long Counter2=0,longvar;
	Study->Surfaces_List[Study->Current_Surface].PASO_IS_Point_Set.Data_Points.clear();

	// go through all IS files
	long Id,Counter=0;
	for( int i=0; i < IS_Files_Names.size(); i++ )
	{
		Counter++;
		Progress_Form->replace_last_line_with_text("Importing IS file "+IntToStr((int)Counter)+
			"/"+IntToStr((int)IS_Files_Names.size()));
		Progress_Form->Show();
		Application->ProcessMessages();

		psdf.open(IS_Files_Names[i].c_str());

		psdf >> String; // ID:
		psdf >> Id; DP2.Identifier = Id;
		psdf >> String; // Name: ((!!!!!!!!!!!!! attention, name of IS point is in Roving_Signal
		psdf >> String; DP2.Roving_Signal.Name = (AnsiString)String;

		while( strcmp(String,"ECG:" ) && !psdf.eof() )
		psdf >> String;

		// reading ECG channels names
		for(int n=0;n<12;n++)
		psdf >> String;

		// clearing values
		for(int n=0;n<12;n++)
		DP2.All_ECG_Channels[n].Voltage_Values.clear();

		// reading signals
		Counter2 = 0;
		while( !psdf.eof() && Counter2 < 10000000 )
		{
			for(int n=0;n<12;n++)
			{
				psdf >> longvar;
				DP2.All_ECG_Channels[n].Voltage_Values.push_back(longvar);
				Counter2++;
			}
		}

		Study->Surfaces_List[Study->Current_Surface].PASO_IS_Point_Set.Data_Points.push_back(DP2);

		psdf.close();

	} // reading IS files

	//-----------------------------------------------------
	// 2. Load PM singals
	//-----------------------------------------------------
	Progress_Form->add_text("Importing PM files ...");
	Progress_Form->Show();
	Application->ProcessMessages();

	// go through all PM files
	Counter=0;
	Study->Surfaces_List[Study->Current_Surface].PASO_PM_Point_Set.Data_Points.clear();

	for( int i=0; i < PM_Files_Names.size(); i++ )
	{
		Counter++;
		Progress_Form->replace_last_line_with_text("Importing PM file "+IntToStr((int)Counter)+
			"/"+IntToStr((int)PM_Files_Names.size()));
		Progress_Form->Show();
		Application->ProcessMessages();

		psdf.open(PM_Files_Names[i].c_str());

		psdf >> String; // ID:
		psdf >> Id; DP2.Identifier = Id;
		psdf >> String; // Name: ((!!!!!!!!!!!!! attention, name of PM point PM in Roving_Signal
		psdf >> String; DP2.Roving_Signal.Name = (AnsiString)String;

		while( strcmp(String,"ECG:" ) && !psdf.eof() )
		psdf >> String;

		// reading ECG channels names
		for(int n=0;n<12;n++)
		psdf >> String;

		// clearing values
		for(int n=0;n<12;n++)
		DP2.All_ECG_Channels[n].Voltage_Values.clear();

		// reading signals
		Counter2 = 0;
		while( !psdf.eof() && Counter2 < 10000000 )
		{
			for(int n=0;n<12;n++)
			{
				psdf >> longvar;
				DP2.All_ECG_Channels[n].Voltage_Values.push_back(longvar);
				Counter2++;
			}
		}

		Study->Surfaces_List[Study->Current_Surface].PASO_PM_Point_Set.Data_Points.push_back(DP2);

		psdf.close();

	} // reading PM files

	}
	else
	{
		Progress_Form->add_text("PASO folder not found.");
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	} // if importing PASO data

	return "Import completed";

}

//-----------------------------------------------------------------------------------------------

AnsiString Data_IO_Class::import_images_files(TStrings* FileNames,STUDY_Class *STUDY,
			TProgress_Form *Progress_Form,bool Append)
{
	Image_2D_Class Image_2D;
	int Result,Last_Item_Pos;
	long Counter=0;

	//	if(Append)

	if( STUDY->is_current_surface_in_range() )
	{

	for(int F=0;F<FileNames->Count;F++)
	if( Utils.is_substring_present( FileNames->Strings[F],".bmp") )
	{
		Progress_Form->replace_last_line_with_text("Importing file " + IntToStr(F+1) + "/" + IntToStr((int)FileNames->Count ) );
		Progress_Form->Show();
		Application->ProcessMessages();

		// find max existing id
		int Max_Existing_Id = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.get_max_image_id();

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.push_back(Image_2D);
		Last_Item_Pos = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size()-1;

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].File_Name =
			FileNames->Strings[F];

		Result = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.
			Images_2D_List[Last_Item_Pos].read_fluoro_image();

		if( Result == OK_RESULT )
		{
			Counter++;

			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.
				Images_2D_List[Last_Item_Pos].Identificator = Max_Existing_Id + 1;

			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.
				Images_2D_List[Last_Item_Pos].Capture_Time = Now(); // CurrentDateTime();

			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].Label = "";

			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].LAO_RAO = 0;

			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].Registered_Status = 0;

			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].X_LU = 0;
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].Y_LU = 0;
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].X_RL =
				STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].Fluoro_Image_Width;
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].Y_RL =
				STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Last_Item_Pos].Fluoro_Image_Height;
		}
		else
		{
				STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.erase(
					STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.begin() + Last_Item_Pos);
				ShowMessage("Image loading error "+IntToStr(Result));
		}
	}


	return "Import completed";

	}
	else
	return "Geo ptr not in range";

}

//-----------------------------------------------------------------------------------------------

bool Data_IO_Class::export_geo_as_vtk_file(AnsiString FileName,Surface_Class *Surface,bool Original_xyz)
{
	ofstream outputfile;

	outputfile.open( FileName.c_str() );

	outputfile << "# vtk DataFile Version 3.0" << endl;
	outputfile << "PatientData Geo_AFCAM" << endl;
	outputfile << "ASCII" << endl;
	outputfile << "DATASET POLYDATA" << endl;

	outputfile << "POINTS " << Surface->Surface_Node_Set.size() << " float" << endl;

	double tx = Surface->Translation_Vector_X;
	double ty = Surface->Translation_Vector_Y;
	double tz = Surface->Translation_Vector_Z;

	// VERTICES
	for(long i=0;i<Surface->Surface_Node_Set.size();i++)
	{
		if( !Original_xyz )
		outputfile << Surface->Surface_Node_Set[i].x << " " << Surface->Surface_Node_Set[i].y << " " << Surface->Surface_Node_Set[i].z << endl;
		else
		{
		outputfile << Surface->Surface_Node_Set[i].x - tx << " ";
		outputfile << Surface->Surface_Node_Set[i].y - ty << " ";
		outputfile << Surface->Surface_Node_Set[i].z - tz << endl;
		}
	}

	// FACES
	outputfile << endl;
	outputfile << "POLYGONS " << Surface->Surface_Triangle_Set.size() << " " << (int)(4.*Surface->Surface_Triangle_Set.size()) << endl;

	for(long i=0;i<Surface->Surface_Triangle_Set.size();i++)
	{
		outputfile << "3 " << Surface->Surface_Triangle_Set[i].Nodes[0] << " " << Surface->Surface_Triangle_Set[i].Nodes[1] << " " << Surface->Surface_Triangle_Set[i].Nodes[2] << endl;
//		outputfile << "3 " << Surface->Surface_Triangle_Set[i].Nodes[0]+1 << " " << Surface->Surface_Triangle_Set[i].Nodes[1] << " " << Surface->Surface_Triangle_Set[i].Nodes[2] << endl;
	}

	outputfile.close();
}

//-----------------------------------------------------------------------------------------------

AnsiString Data_IO_Class::import_vtk_file(AnsiString FileName,STUDY_Class *STUDY,
			TProgress_Form *Progress_Form,bool Append)
{
	char String[20000];
	long TInt,TInt2;
	Geometry_Vertex node;
	double v;

	// clear surfaces
	if( !Append )
	STUDY->Surfaces_List.clear();

	Surface_Class Suface_Class_Item;
	STUDY->Surfaces_List.push_back(Suface_Class_Item);
	STUDY->Current_Surface = STUDY->Surfaces_List.size() -1;
	AnsiString GeoName = Utils.get_string_after_last_occurence_of_specified_string(FileName,"\\");
	STUDY->Surfaces_List[STUDY->Current_Surface].Name = GeoName;

	if( STUDY->is_current_surface_in_range() )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.clear();
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.clear();

	// add empty dataset
	Data_Point_Set_Class DS;
    STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.push_back(DS);

	ifstream df;
	df.open(FileName.c_str());

	df >> String;
	df >> String;

	if( strcmp( String,"vtk") )
	{
		return "Not proper vtk file format. E134";
	}
	else
	{

		while(  strcmp(String,"POINTS" ) && !df.eof() )
			df >> String;

		df >> TInt;
		df >> String;

		for(long tt=0;tt<TInt;tt++)
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.push_back(node);

		for(long tt=0;tt<TInt;tt++)
		{
			df >> STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].x;
			df >> STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].y;
			df >> STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].z;

			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].Original_x =
				STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].x;
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].Original_y =
				STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].y;
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].Original_z =
				STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].z;

		}

		while(  strcmp(String,"POLYGONS" ) && !df.eof() )
			df >> String;

		df >> TInt;
		df >> TInt2;
		Geometry_Triangle triangle;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.assign(TInt,triangle);

		for(long tt=0;tt<TInt;tt++)
		{
			df >> String;
			df >> STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[tt].Nodes[0];
			df >> STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[tt].Nodes[1];
			df >> STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[tt].Nodes[2];
		}

		df.close();
	}

	return "Import completed";

	}
	else
	return "Geo ptr not in range";
}

//-----------------------------------------------------------------------------------------------

AnsiString Data_IO_Class::import_rhythmia_file(AnsiString Path, AnsiString FileName,STUDY_Class *STUDY,
			TProgress_Form *Progress_Form,bool Append)
{
	char String[20000],Map_Name[20000];
	long TInt,TInt2,Electrogram_Length;
	Geometry_Vertex node;
	double v,Ref;
	Data_Point_Set_Class DSet;
	Data_Point DP;
	int dset,Flag;
	Data_Point D;
	double Sampling_Rate,x,y,z;

	STUDY->Study_Source_Path = Path;
	STUDY->Study_Current_Path_And_File_Name = Path + "\\" + FileName + ".eplab";

	// clear surfaces
	if( !Append )
	STUDY->Surfaces_List.clear();

	Surface_Class Suface_Class_Item;
	STUDY->Surfaces_List.push_back(Suface_Class_Item);
	STUDY->Current_Surface = STUDY->Surfaces_List.size() -1;
	AnsiString GeoName = Utils.get_string_after_last_occurence_of_specified_string(FileName,"\\");
	STUDY->Surfaces_List[STUDY->Current_Surface].Name = GeoName;

  if( STUDY->is_current_surface_in_range() )
  {
	STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source = MAPPING_SYSTEM_ORIGIN_RHYTHMIA;

	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.clear();
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.clear();

	ifstream df;
	df.open((Path + "\\" + FileName).c_str());

	df >> String;
	df >> String;

	// update progress form
	Progress_Form->add_text("Reading geometry section...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//-------------------------------------------------------------------
	// READING GEOMETRY SECTION
	//-------------------------------------------------------------------
	if( strcmp( String,"vtk") )
	{
		df.close();
		return "Not proper vtk file format or intermediary file not found. E136";
	}
	else
	{
		while(  strcmp(String,"POINTS" ) && !df.eof() )
			df >> String;

		df >> TInt;
		df >> String;

		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.assign(TInt,node);

		for(long tt=0;tt<TInt;tt++)
		{
			df >> x; STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].x = x;
			df >> y; STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].y = y;
			df >> z; STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].z = z;

			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].Original_x = x;
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].Original_y = y;
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[tt].Original_z = z;
		}

		while(  strcmp(String,"POLYGONS" ) && !df.eof() )
			df >> String;

		df >> TInt;
		df >> TInt2;
		Geometry_Triangle triangle;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.assign(TInt,triangle);

		for(long tt=0;tt<TInt;tt++)
		{
			df >> String;
			df >> STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[tt].Nodes[0];
			df >> STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[tt].Nodes[1];
			df >> STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[tt].Nodes[2];
		}

	//-------------------------------------------------------------------
	// READING DATA POINTS SECTION
	//-------------------------------------------------------------------
	df >> String;

	 if( strcmp( String,"Rhythmia_Electrograms_Matlab_Export_file_v_8") )
	 {
		df.close();
		return "Invalid data point export section version. E135";
	 }
	 else
	 {
		// update progress form
		Progress_Form->add_text("Reading data points section...");
		Progress_Form->Show();
		Application->ProcessMessages();

		df >> Map_Name;
		df >> Sampling_Rate;
		df >> String;  // "data points number" text
		df >> TInt;            // data points number

		// create values
		Value_Description_Class Value_Desc;
		Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS; // based on data points
		Value_Desc.Displayed_In_Table = true;

		Value_Desc.Value_Name = "bipolar_voltage";
		Value_Desc.Unit = "mV";
		Value_Desc.Voltage_Amplitude_Value = true;
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);
		Value_Desc.Voltage_Amplitude_Value = false;

		Value_Desc.Value_Name = "unipolar_voltage";
		Value_Desc.Unit = "mV";
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);

		Value_Desc.Value_Name = "LAT_bipolar_voltage";
		Value_Desc.Unit = "ms";
		Value_Desc.LAT_Value = true;
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);
		Value_Desc.LAT_Value = false;

		Value_Desc.Value_Name = "LAT_unipolar_voltage";
		Value_Desc.Unit = "ms";
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);

		// allocate data points set and points
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.push_back(DSet);

		dset = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size()-1;

		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Name =
			(AnsiString)(Map_Name);

		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points.clear();

		//----------------------------------------------------------
		// set up data point
		//----------------------------------------------------------
		long Lng=0;
		string input_string;
		D.Roving_Signal.Time_Step_ms = 1000.0/Sampling_Rate;
		D.Reference_Signal.Time_Step_ms = 1000.0/Sampling_Rate;
		D.ECG_Signal.Time_Step_ms = 1000.0/Sampling_Rate;

		for(long dp=0;dp<TInt;dp++)
		{
			df >> v; D.x = v;
			df >> v; D.y = v;
			df >> v; D.z = v;

			D.Original_x = D.x;
			D.Original_y = D.y;
			D.Original_z = D.z;

			df >> D.Identifier;

			df >> input_string;
			if( input_string != "NaN" && input_string != "Inf") v = std::stof(input_string); else v=0;
			D.set_value("unipolar_voltage",v,STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());
			df >> input_string;
			if( input_string != "NaN" && input_string != "Inf") v = std::stof(input_string); else v=0;
			D.set_value("bipolar_voltage",v,STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

			df >> input_string;
			if( input_string != "NaN" && input_string != "Inf") v = std::stof(input_string); else v=0;
			D.set_value("LAT_unipolar_voltage",v,STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());
			df >> input_string;
			if( input_string != "NaN" && input_string != "Inf") v = std::stof(input_string); else v=0;
			D.set_value("LAT_bipolar_voltage",v,STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

			//----------------------------------
			//  BIPOLAR SIGNAL
			//----------------------------------
			df >> String;
			df >> Flag;

			D.Roving_Signal.Voltage_Values.clear();

			if( Flag == 1 )
			{

			// ecg channel name
			df >> String;
			D.Roving_Signal.Name = Utils.replace_substring_from_string((AnsiString)String," ","_");

			df >> Electrogram_Length;

			D.Roving_Signal.Time_Step_ms = 1000./9.536740000000000e+02; ///!!!!!!1

			 for(long t=0;t<Electrogram_Length;t++)
			 {
				df >> v;
				D.Roving_Signal.Voltage_Values.push_back(v);
			 }

			} // if reading bipolar

			//----------------------------------
			//  UNIPOLAR SIGNAL
			//----------------------------------
			df >> String;
			df >> Flag;

			D.Reference_Signal.Voltage_Values.clear();

			if( Flag == 1 )
			{

			// ecg channel name
			df >> String;
			D.Reference_Signal.Name = Utils.replace_substring_from_string((AnsiString)String," ","_");

			df >> Electrogram_Length;
			D.Reference_Signal.Time_Step_ms = 1000./9.536740000000000e+02; ///!!!!!!1

			 for(long t=0;t<Electrogram_Length;t++)
			 {
				df >> v;
				D.Reference_Signal.Voltage_Values.push_back(v);
			 }
			} // reading unipolar

			//----------------------------------
			//  ECG SIGNAL
			//----------------------------------
			df >> String;
			df >> Flag;

			D.ECG_Signal.Voltage_Values.clear();

			if( Flag == 1 )
			{

			// ecg channel name
			df >> String;
			D.ECG_Signal.Name = Utils.replace_substring_from_string((AnsiString)String," ","_");

			df >> Electrogram_Length;

			D.ECG_Signal.Time_Step_ms = 1000./Sampling_Rate;

			 for(long t=0;t<Electrogram_Length;t++)
			 {
				df >> v;
				D.ECG_Signal.Voltage_Values.push_back(v);
			 }

			} // reading ECG

			// ref time
			df >> Ref;
			D.Ref_Signal_Activation_ptr = Ref;
			D.Rov_Signal_Activation_ptr = Ref +
				Sampling_Rate/1000.0*
				D.get_value("LAT_bipolar_voltage",STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

			STUDY->Surfaces_List[STUDY->Current_Surface].
				Data_Point_Set[dset].Data_Points.push_back(D);

		} // through all dps

	 } // if dp section valid

	} // if geo section valid

	// update progress form
	Progress_Form->add_text("Import completed...");
	Progress_Form->Show();
	Application->ProcessMessages();

	df.close();

	return "Import completed";

  } // geo ptr in range
  else
  return "Geo ptr not in range";
}

//-----------------------------------------------------------------------------------------------

AnsiString Data_IO_Class::import_octave_electrode_file(AnsiString FileName,STUDY_Class *STUDY,
			TProgress_Form *Progress_Form)
{
	char String[20000],Map_Name[20000];
	long TInt,TInt2,Electrogram_Length;
	Geometry_Vertex node;
	double v,Ref;
	Data_Point_Set_Class DSet;
	Data_Point DP;
	int dset,Flag;
	Data_Point D;
	double Sampling_Rate;

	STUDY->Study_Source_Path = FileName;
	STUDY->Study_Current_Path_And_File_Name = FileName + ".eplab";

  if( STUDY->is_current_surface_in_range() )
  {
	STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source = MAPPING_SYSTEM_ORIGIN_RHYTHMIA;

	ifstream df;
	df.open(FileName.c_str());

	//-------------------------------------------------------------------
	// READING DATA POINTS SECTION
	//-------------------------------------------------------------------
		// update progress form
		Progress_Form->add_text("Reading data points section...");
		Progress_Form->Show();
		Application->ProcessMessages();

	df >> String;
	if(	strcmp( String,"Rhythmia_Electrograms_Matlab_Export_file_v_7") )
	{
		ShowMessage("Unsupported version or not proper file format. E234");
	}
	else
	{
		int version = Utils.get_string_after_last_occurence_of_specified_string(String,"_").ToInt();

		df >> String;
		df >> TInt;            // data points number

		// create values
		Value_Description_Class Value_Desc;
		Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS; // based on data points
		Value_Desc.Displayed_In_Table = true;

		Value_Desc.Value_Name = "bipolar_voltage";
		Value_Desc.Unit = "mV";
		Value_Desc.Voltage_Amplitude_Value = true;
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);
		Value_Desc.Voltage_Amplitude_Value = false;

		Value_Desc.Value_Name = "unipolar_voltage";
		Value_Desc.Unit = "mV";
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);

		Value_Desc.Value_Name = "LAT_bipolar_voltage";
		Value_Desc.Unit = "ms";
		Value_Desc.LAT_Value = true;
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);
		Value_Desc.LAT_Value = false;

		Value_Desc.Value_Name = "LAT_unipolar_voltage";
		Value_Desc.Unit = "ms";
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);

		// allocate data points set and points
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.push_back(DSet);
		dset = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size()-1;
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Name = "DPset";
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points.clear();

		//----------------------------------------------------------
		// set up data point
		//----------------------------------------------------------
		long Lng=0;
		string input_string;
		D.Roving_Signal.Time_Step_ms = 1000.0/Sampling_Rate;
		D.Reference_Signal.Time_Step_ms = 1000.0/Sampling_Rate;
		D.ECG_Signal.Time_Step_ms = 1000.0/Sampling_Rate;

		STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points.clear();
		STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points.assign(TInt,DP);

		for(long dp=0;dp<TInt;dp++)
		{
			df >> v;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].x = v;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].Original_x = v;
			df >> v;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].y = v;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].Original_y = v;
			df >> v;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].z = v;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].Original_z = v;

			df >> STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].Identifier;

			df >> input_string;
			if( input_string != "NaN" && input_string != "Inf") v = std::stof(input_string); else v=0;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].set_value("unipolar_voltage",v,STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());
			df >> input_string;
			if( input_string != "NaN" && input_string != "Inf") v = std::stof(input_string); else v=0;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].set_value("bipolar_voltage",v,STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

			//----------------------------------
			//  BIPOLAR SIGNAL
			//----------------------------------
			df >> String;
			df >> Flag;

			if( Flag == 1 )
			{

			// ecg channel name
			df >> String;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].Roving_Signal.Name = (AnsiString)String;

			df >> Electrogram_Length;
			STUDY->Surfaces_List[0].Data_Point_Set[0].
				Data_Points[dp].Roving_Signal.Time_Step_ms = 1000./9.536740000000000e+02; ///!!!!!!1

			for(long t=0;t<Electrogram_Length;t++)
			{
				df >> v;
				STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].
						Roving_Signal.Voltage_Values.push_back(v/32000);
			}
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].Ref_Signal_Activation_ptr =
				0.5*STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].Roving_Signal.Voltage_Values.size();

			} // if reading bipolar


			//----------------------------------
			//  UNIPOLAR SIGNAL
			//----------------------------------
			df >> String;
			df >> Flag;

			if( Flag == 1 )
			{

			// ecg channel name
			df >> String;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].
				Reference_Signal.Name = (AnsiString)String;

			df >> Electrogram_Length;

			STUDY->Surfaces_List[0].Data_Point_Set[0].
				Data_Points[dp].Reference_Signal.Time_Step_ms = 1000./9.536740000000000e+02; ///!!!!!!1

			for(long t=0;t<Electrogram_Length;t++)
			{
				df >> v;
				STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].
						Reference_Signal.Voltage_Values.push_back(v/32000);
			}
			} // reading unipolar

			if( version >= 5)
			{

			//----------------------------------
			//  ECG SIGNAL
			//----------------------------------
			df >> String;
			df >> Flag;

			if( Flag == 1 )
			{

			// ecg channel name
			df >> String;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].
				ECG_Signal.Name = (AnsiString)String;

			df >> Electrogram_Length;

			STUDY->Surfaces_List[0].Data_Point_Set[0].
				Data_Points[dp].ECG_Signal.Time_Step_ms = 1000./9.536740000000000e+02; ///!!!!!!1

			for(long t=0;t<Electrogram_Length;t++)
			{
			df >> v;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].
				ECG_Signal.Voltage_Values.push_back(v/32000);
			}

			} // reading ECG

			// ref time
			df >>  Ref;
			STUDY->Surfaces_List[0].Data_Point_Set[0].Data_Points[dp].
				Ref_Signal_Activation_ptr = Ref;

			}
		}
		df.close();
	}

	// update progress form
	Progress_Form->add_text("Import completed...");
	Progress_Form->Show();
	Application->ProcessMessages();

	df.close();

	return "Import completed";

  } // geo ptr in range
  else
  return "Geo ptr not in range";
}

//-----------------------------------------------------------------------------------------------

void Data_IO_Class::export_master_CARTO_files_Geoff_v1(STUDY_Class *STUDY,
		TProgress_Form *Progress_Form,AnsiString Path)
{
   if( STUDY->is_current_surface_in_range() )
   if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
   {

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	AnsiString S=","; // separator (csv file by default)

	AnsiString FullPath = Path+"\\PARSED FILES CARTO " +
		STUDY->Surfaces_List[STUDY->Current_Surface].Name  + "\\";

	mkdir( FullPath.c_str() );

	// update progress form
	Progress_Form->add_text("Saving parsed files...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//-------------------------------------------------------
	// 1. MASTER FILE WITH DPS AND EGMS
	//-------------------------------------------------------
	// update progress form
	Progress_Form->add_text("Saving parsed EGM file...");
	Progress_Form->Show();
	Application->ProcessMessages();

	ofstream df( (FullPath + "DataPoints_With_EGMS.csv").c_str());

	// header
	df << "PointOrderId" << S.c_str() << "PointGroupId" << S.c_str() << "X" << S.c_str() << "Y" << S.c_str() << "Z" << S.c_str();
	df << "ECG_Site_Name" << S.c_str() << "Data_Type_1ecg_2bip_3uni" << S.c_str() << "Bipolar_Voltage" << S.c_str();
	df << "Unipolar_Voltage" << S.c_str() << "LAT" << S.c_str();
	df << "Impedance" << S.c_str() << "EGM_Data_Start" << S.c_str();
	df << "\n";

	// lines
	for(long dp=0;dp<STUDY->Surfaces_List[STUDY->Current_Surface].
				Data_Point_Set[dset].Data_Points.size();dp++)
	{
		for(int it=0;it<14;it++)
		{
			df << dp+1 << S.c_str();

			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[dp].Identifier << S.c_str();

			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[dp].x << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[dp].y << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[dp].z << S.c_str();

			// channels names
			AnsiString Name;
			if(it==0) Name = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Name;
			if(it==1) Name = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Name;
			if(it>=2) Name = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].All_ECG_Channels[it-2].Name.c_str();

			df << Name.c_str() << S.c_str();

			// type (1: ecg, 2: bipolar, 3: unipolar)
			if( Utils.is_string_an_ECG_channel_name(Name) )
			df << "1" << S.c_str();
			else
			if( Utils.is_substring_present(Name, "-" ) )
			df << "2" << S.c_str();
			else
			df << "3" << S.c_str();

			// car file data
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("Bipolar voltage",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("Unipolar voltage",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("Local Activation Time",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("Impedance",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();

			if(it==0)
			for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Voltage_Values.size();t++)
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Voltage_Values[t] << S.c_str();

			if(it==1)
			for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Voltage_Values.size();t++)
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Voltage_Values[t] << S.c_str();

			if(it>=2)
			for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].All_ECG_Channels[it-2].Voltage_Values.size();t++)
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].All_ECG_Channels[it-2].Voltage_Values[t] << S.c_str();

			df << "\n";
		}

		// additional ref channels
		for(int it=0;it<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels.size();it++)
		{
			df << dp+1 << S.c_str();

			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[dp].Identifier << S.c_str();

			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[dp].x << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[dp].y << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[dp].z << S.c_str();

			// channels names
			df <<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels[it].Name.c_str() << S.c_str();

			// type (1: ecg, 2: bipolar, 3: unipolar)
			if( Utils.is_string_an_ECG_channel_name( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels[it].Name ) )
			df << "1" << S.c_str();
			else
			if( Utils.is_substring_present( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels[it].Name, "-" ) )
			df << "2" << S.c_str();
			else
			df << "3" << S.c_str();

			// car file data
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("Bipolar voltage",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("Unipolar voltage",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("Local Activation Time",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("Impedance",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();

			for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels[it].Voltage_Values.size();t++)
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels[it].Voltage_Values[t] << S.c_str();

			df << "\n";
		}
	}

	df.close();

	//-------------------------------------------------------
	// 2. GEO - VERTICES
	//-------------------------------------------------------
	Progress_Form->add_text("Saving parsed geometry vertices file...");
	Progress_Form->Show();
	Application->ProcessMessages();
	ofstream df2( (FullPath + "Geometry_Vertices_xyz.csv").c_str());

	for(long n=0;n<STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();n++)
	{
		df2 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].x << ",";
		df2 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].y << ",";
		df2 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].z << "\n";
	}

	df2.close();

	//-------------------------------------------------------
	// 2.1 GEO - NORMALS TO VERTICES
	//-------------------------------------------------------
	Progress_Form->add_text("Saving parsed geometry vertices normals file...");
	Progress_Form->Show();
	Application->ProcessMessages();
	ofstream df21( (FullPath + "Geometry_Vertices_normals.csv").c_str());

	for(long n=0;n<STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();n++)
	{
		df21 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].normal[0] << ",";
		df21 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].normal[1] << ",";
		df21 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].normal[2] << "\n";
	}

	df21.close();

	//-------------------------------------------------------
	// 3. GEO - TRIANGLES
	//-------------------------------------------------------
	Progress_Form->add_text("Saving parsed geometry triangles file...");
	Progress_Form->Show();
	Application->ProcessMessages();
	ofstream df3( (FullPath + "Geometry_Triangles_node_ptrs.csv").c_str());

	for(long n=0;n<STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();n++)
	{
		df3 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[n].Nodes[0]+1 << ",";
		df3 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[n].Nodes[1]+1 << ",";
		df3 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[n].Nodes[2]+1 << "\n";
	}

	df3.close();

	//-------------------------------------------------------
	// 4. ABLATION SITES
	//-------------------------------------------------------
	Progress_Form->add_text("Saving parsed ablation points list file...");
	Progress_Form->Show();
	Application->ProcessMessages();
	ofstream df4( (FullPath + "Ablation_Sites.csv").c_str());

	df4 << "X,Y,Z" << endl;

	for(long n=0;n<STUDY->Surfaces_List[STUDY->Current_Surface].Ablation_Points_List.size();n++)
	{
		df4 << STUDY->Surfaces_List[STUDY->Current_Surface].Ablation_Points_List[n].x << ",";
		df4 << STUDY->Surfaces_List[STUDY->Current_Surface].Ablation_Points_List[n].y << ",";
		df4 << STUDY->Surfaces_List[STUDY->Current_Surface].Ablation_Points_List[n].z << "\n";
	}

	df4.close();

	//-------------------------------------------------------
	// 5. PM files
	//-------------------------------------------------------
	Progress_Form->add_text("Saving parsed PASO: PM files...");
	Progress_Form->Show();
	Application->ProcessMessages();
	ofstream df5( (FullPath + "PASO PM files.csv").c_str());

	df5 << "Id,x,y,z,ECG_Channel_Name,EGM" << endl;

	for(long dp=0;dp<STUDY->Surfaces_List[STUDY->Current_Surface].
				PASO_PM_Point_Set.Data_Points.size();dp++)
	{
		for(int it=0;it<12;it++)
		{
			df5 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_PM_Point_Set.
				Data_Points[dp].Identifier << S.c_str();

			df5 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_PM_Point_Set.
				Data_Points[dp].x << S.c_str();
			df5 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_PM_Point_Set.
				Data_Points[dp].y << S.c_str();
			df5 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_PM_Point_Set.
				Data_Points[dp].z << S.c_str();

			df5 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_PM_Point_Set.
				Data_Points[dp].All_ECG_Channels[it].Name << S.c_str();


			for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].PASO_PM_Point_Set.
				Data_Points[dp].All_ECG_Channels[it].Voltage_Values.size();t++)
			df5 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_PM_Point_Set.
				Data_Points[dp].All_ECG_Channels[it].Voltage_Values[t] << S.c_str();

			df5 << endl;
		}
	}

   df5.close();

	//-------------------------------------------------------
	// 6. IS files
	//-------------------------------------------------------
	Progress_Form->add_text("Saving parsed PASO: IS files...");
	Progress_Form->Show();
	Application->ProcessMessages();
	ofstream df6( (FullPath + "PASO IS files.csv").c_str());

	df6 << "Id,x,y,z,ECG_Channel_Name,EGM" << endl;

	for(long dp=0;dp<STUDY->Surfaces_List[STUDY->Current_Surface].
				PASO_IS_Point_Set.Data_Points.size();dp++)
	{
		for(int it=0;it<12;it++)
		{
			df6 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_IS_Point_Set.
				Data_Points[dp].Identifier << S.c_str();

			df6 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_IS_Point_Set.
				Data_Points[dp].x << S.c_str();
			df6 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_IS_Point_Set.
				Data_Points[dp].y << S.c_str();
			df6 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_IS_Point_Set.
				Data_Points[dp].z << S.c_str();

			df6 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_IS_Point_Set.
				Data_Points[dp].All_ECG_Channels[it].Name << S.c_str();


			for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].PASO_IS_Point_Set.
				Data_Points[dp].All_ECG_Channels[it].Voltage_Values.size();t++)
			df6 << STUDY->Surfaces_List[STUDY->Current_Surface].PASO_IS_Point_Set.
				Data_Points[dp].All_ECG_Channels[it].Voltage_Values[t] << S.c_str();

			df6 << endl;
		}
	}

   df6.close();

	Progress_Form->add_text("Parsed files saved in: " + FullPath);
	Progress_Form->add_text("Press CLOSE to close this window");
	Progress_Form->Show();
	Application->ProcessMessages();

   } // if all in range
}

//-----------------------------------------------------------------------------------------------

void Data_IO_Class::export_master_NavX_files_Geoff_v1(STUDY_Class *STUDY,
		TProgress_Form *Progress_Form,AnsiString Path)
{
   if( STUDY->is_current_surface_in_range() )
//   if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
   {

   // assumption: it will be always dset=0, navx DxL folder always has just one data point set/map
	STUDY->Current_Surface = 0;
	int dset = 0; // STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	AnsiString Channel_Name,S=","; // separator (csv file by default)

	AnsiString FullPath = Path+"\\PARSED FILES NAVX\\";

	mkdir( FullPath.c_str() );

	// update progress form
	Progress_Form->add_text("Saving parsed files...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//-------------------------------------------------------
	// 1. MASTER FILE WITH DPS AND EGMS
	//-------------------------------------------------------
	// update progress form
	Progress_Form->add_text("Saving parsed EGM file...");
	Progress_Form->Show();
	Application->ProcessMessages();

	ofstream df( (FullPath + "DataPoints_With_EGMS.csv").c_str());

	// header
	df << "PointOrderId" << S.c_str() << "PointGroupId" << S.c_str() << "X" << S.c_str() << "Y" << S.c_str() << "Z" << S.c_str();
	df << "ECG_Site_Name" << S.c_str() << "Data_Type_1ecg_2bip_3uni_4unknown" << S.c_str() << "peak2peak" << S.c_str();
	df << "rov LAT" << S.c_str() << /*"ref LAT" << */ S.c_str();
	df << "EGM_Data_Start" << S.c_str();
	df << "\n";

	// lines
	for(long dp=0;dp<STUDY->Surfaces_List[STUDY->Current_Surface].
				Data_Point_Set[dset].Data_Points.size();dp++)
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels.size() == 3 )
	{
		for(int it=0;it<5;it++)
		if( it==0 && STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Name != "empty" ||
			it==1 && STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Name != "empty" ||
			it>=2 && STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels[it-2].Name  != "empty"
		  )
		{
			df << dp+1 << S.c_str();

			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Identifier << S.c_str();

			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].x << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].y << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].z << S.c_str();

			// channels names
			if(it==0) Channel_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Name;
			if(it==1) Channel_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Name;
			if(it>=2) Channel_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels[it-2].Name;

			df << Channel_Name.c_str() << S.c_str();

			// type (1: ecg, 2: bipolar, 3: unipolar, 4: unknown)
			if( Utils.is_string_an_ECG_channel_name(Channel_Name) ||
				Utils.is_substring_present(Channel_Name,"ECG") ||
				Utils.is_substring_present(Channel_Name,"ecg") )
			df << "1" << S.c_str();
			else
			df << "4" << S.c_str();

			// data points values
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("peak2peak",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].get_value("rov LAT",
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();
//			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
//				Data_Points[dp].get_value("ref LAT",
 //				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << S.c_str();

			if(it==0)
			for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Voltage_Values.size();t++)
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Voltage_Values[t] << S.c_str();

			if(it==1)
			for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Voltage_Values.size();t++)
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Voltage_Values[t] << S.c_str();

			if(it>=2)
			for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels[it-2].Voltage_Values.size();t++)
			df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Additional_Ref_Channels[it-2].Voltage_Values[t] << S.c_str();

			df << "\n";
		}
	}

	df.close();

	//-------------------------------------------------------
	// 2. GEO - VERTICES
	//-------------------------------------------------------
	Progress_Form->add_text("Saving parsed geometry vertices file...");
	Progress_Form->Show();
	Application->ProcessMessages();
	ofstream df2( (FullPath + "Geometry_Vertices_xyz.csv").c_str());

	for(long n=0;n<STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();n++)
	{
		df2 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].x << ",";
		df2 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].y << ",";
		df2 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].z << "\n";
	}

	df2.close();

	//-------------------------------------------------------
	// 2.1 GEO - NORMALS TO VERTICES
	//-------------------------------------------------------
	Progress_Form->add_text("Saving parsed geometry vertices normals file...");
	Progress_Form->Show();
	Application->ProcessMessages();
	ofstream df21( (FullPath + "Geometry_Vertices_normals.csv").c_str());

	for(long n=0;n<STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();n++)
	{
		df21 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].normal[0] << ",";
		df21 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].normal[1] << ",";
		df21 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n].normal[2] << "\n";
	}

	df21.close();

	//-------------------------------------------------------
	// 3. GEO - TRIANGLES
	//-------------------------------------------------------
	Progress_Form->add_text("Saving parsed geometry triangles file...");
	Progress_Form->Show();
	Application->ProcessMessages();
	ofstream df3( (FullPath + "Geometry_Triangles_node_ptrs.csv").c_str());

	for(long n=0;n<STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();n++)
	{
		df3 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[n].Nodes[0]+1 << ",";
		df3 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[n].Nodes[1]+1 << ",";
		df3 << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[n].Nodes[2]+1 << "\n";
	}

	df3.close();

	Progress_Form->add_text("Parsed files saved in: " + FullPath);
	Progress_Form->add_text("Press CLOSE to close this window");
	Progress_Form->Show();
	Application->ProcessMessages();

   } // if all in range
}

//-----------------------------------------------------------------------------------------------
bool Data_IO_Class::export_geo_as_stl_file(AnsiString FileName,Surface_Class *Surface,
		int Mode,Segments_List_Class* Segments_Info)
{
	//binary file
	std::string header_info = "EPLab Works stl export file";
	char head[80];
	std::strncpy(head,header_info.c_str(),sizeof(head)-1);
	char attribute[2] = "0";
	float v;

	std::ofstream myfile;

	FileName = Utils.add_file_extension(FileName,".stl");

	myfile.open((FileName).c_str(),  std::ios::out | std::ios::binary);
	myfile.write(head,sizeof(head));
	long n0,n1,n2,tt = Surface->Surface_Triangle_Set.size();
	myfile.write((char*)&tt,4);
	bool bits_b[8],bits_g[8],bits_r[8],bits_part1[8],bits_part2[8];
	char a1,a2;
	int color,color_R,color_G,color_B;

	//write down every triangle
	for(long t=0;t<Surface->Surface_Triangle_Set.size();t++)
	{

	n0 = Surface->Surface_Triangle_Set[t].Nodes[0];
	n1 = Surface->Surface_Triangle_Set[t].Nodes[1];
	n2 = Surface->Surface_Triangle_Set[t].Nodes[2];

	//normal vector coordinates
	v = Surface->Surface_Triangle_Set[t].normal[0];
	myfile.write((char*)&v, 4);
	v = Surface->Surface_Triangle_Set[t].normal[1];
	myfile.write((char*)&v, 4);
	v = Surface->Surface_Triangle_Set[t].normal[2];
	myfile.write((char*)&v, 4);

	//p1 coordinates
	v = Surface->Surface_Node_Set[n0].x;
	myfile.write((char*)&v, 4);
	v = Surface->Surface_Node_Set[n0].y;
	myfile.write((char*)&v, 4);
	v = Surface->Surface_Node_Set[n0].z;
	myfile.write((char*)&v, 4);

	//p2 coordinates
	v = Surface->Surface_Node_Set[n1].x;
	myfile.write((char*)&v, 4);
	v = Surface->Surface_Node_Set[n1].y;
	myfile.write((char*)&v, 4);
	v = Surface->Surface_Node_Set[n1].z;
	myfile.write((char*)&v, 4);

	//p3 coordinates
	v = Surface->Surface_Node_Set[n2].x;
	myfile.write((char*)&v, 4);
	v = Surface->Surface_Node_Set[n2].y;
	myfile.write((char*)&v, 4);
	v = Surface->Surface_Node_Set[n2].z;
	myfile.write((char*)&v, 4);

	if(Surface->Surface_Triangle_Set[t].Segment_Id <= 0 )
	{

	color = 32*0.33*(Surface->Surface_Node_Set[n0].B+
					 Surface->Surface_Node_Set[n1].B+
					 Surface->Surface_Node_Set[n2].B);
	PNUM.FromByte((char)color,bits_b);

	color = 32*0.33*(Surface->Surface_Node_Set[n0].G+
					 Surface->Surface_Node_Set[n1].G+
					 Surface->Surface_Node_Set[n2].G);
	PNUM.FromByte((char)color,bits_g);

	color = 32*0.33*(Surface->Surface_Node_Set[n0].R+
					 Surface->Surface_Node_Set[n1].R+
					 Surface->Surface_Node_Set[n2].R);
	PNUM.FromByte((char)color,bits_r);

	}
	else
	{

	Segments_Info->get_segment_color(Surface->Surface_Triangle_Set[t].Segment_Id,
			&color_R,&color_G,&color_B);

	PNUM.FromByte((char)color_R,bits_r);
	PNUM.FromByte((char)color_G,bits_g);
	PNUM.FromByte((char)color_B,bits_b);

	}


	bits_part1[0] = bits_b[0];
	bits_part1[1] = bits_b[1];
	bits_part1[2] = bits_b[2];
	bits_part1[3] = bits_b[3];
	bits_part1[4] = bits_b[4];

	bits_part1[5] = bits_g[0];
	bits_part1[6] = bits_g[1];
	bits_part1[7] = bits_g[2];

	bits_part2[0] = bits_g[3];
	bits_part2[1] = bits_g[4];

	bits_part2[2] = bits_r[0];
	bits_part2[3] = bits_r[1];
	bits_part2[4] = bits_r[2];
	bits_part2[5] = bits_r[3];
	bits_part2[6] = bits_r[4];

	bits_part2[7] = true;

	a1 = PNUM.ToByte(bits_part1);
	a2 = PNUM.ToByte(bits_part2);
	myfile.write((char*)&a1,1);
	myfile.write((char*)&a2,1);

	}

	myfile.close();

	return true;
}
//---------------------------------------------------------------------------

void Data_IO_Class::save_values_at_geometry_nodes(AnsiString FileName,STUDY_Class *STUDY)
{
	ofstream df;
	df.open(FileName.c_str());

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	Progress_Form->add_text("Exporting values at geometry nodes...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//-------------
	// header
	//-------------
	df << "Id,";
	for(int k=0;k<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table.size();k++)
	df << STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(k) << ",";
	df << "\n";

	// find node ptr of earliest site
	double dist;
	int LAT_Value_Ptr = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_LAT_value_ptr();
	long Min_Node_Ptr, Max_Node_Ptr;
	STUDY->Surfaces_List[STUDY->Current_Surface].
		get_nodes_ptrs_with_minmax_value(dset, LAT_Value_Ptr, &Min_Node_Ptr, &Max_Node_Ptr );

	//-------------
	// values
	//-------------
	for(unsigned long i=0; i<STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size(); i++)
	{

	df << i << ",";

	for(int k=0;k<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table.size();k++)
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].get_value(dset,k) != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	df << STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].get_value(dset,k) << ",";
	else
	df << ",";

	df << "\n";

	}

	df.close();
}

//---------------------------------------------------------------------------

void Data_IO_Class::save_values_at_data_points(AnsiString FileName,STUDY_Class *STUDY)
{
	ofstream df;
	df.open(FileName.c_str());
    double v;

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	Progress_Form->add_text("Exporting values at geometry nodes...");
	Progress_Form->Show();
	Application->ProcessMessages();

	//-------------
	// header
	//-------------
	df << "Id,";
	for(int k=0;k<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table.size();k++)
	df << STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(k) << ",";

	df << "\n";

	//-------------
	// values
	//-------------
	for(long i=0;i<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points.size();i++)
	{

	df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[i].Identifier << ",";

	for(int k=0;k<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table.size();k++)
	{
	v = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[i].get_value(
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(k),
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

	if( v!= NOT_POSSIBLE_TO_CALCULATE_VALUE )
	df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[i].get_value(
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(k),
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref()) << ",";
	else
	df << ",";

	}
	df << "\n";

	}

	df.close();
}

//---------------------------------------------------------------------------

