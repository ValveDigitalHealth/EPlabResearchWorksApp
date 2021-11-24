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
#ifndef DATA_IO_CLASS_H
#define DATA_IO_CLASS_H

#include <vcl.h>
#include <dir.h>

#include "Progress_Bar_Form.h"

#include "FileCtrl.hpp"

#include "Data_Structures/STUDY_Class.h"
#include "Utils.h"
#include "predefined_parameters.h"
#include "Data_Structures/value_description_class.h"

#include "Numerical_Library.h"

#include "pugixml.hpp"

#include "selection_form.h"

//------------------------------------------------------------------------------

class Data_IO_Class
{

	public:

	Data_IO_Class();

	AnsiString Application_Directory;

	Utils_Tools Utils;

	//-------------------------------------------------------------------------
	// Location of the current data io folder
	//-------------------------------------------------------------------------
	String Data_Files_Path;

	//---------------------------------------------------------------------------
	// NavX folder import
	//---------------------------------------------------------------------------
	AnsiString import_NavX_DxL_folder(TFileListBox* Data_FileListBox,
				STUDY_Class *Study,TProgress_Form *Progress_Form,bool Append);

	int get_ABL_xyz_from_Locations_file(AnsiString FileNamePath,long Time_Ptr,double *x, double *y, double *z);

	// geo load
	vector <AnsiString> get_list_of_geo_names_from_NavX_geo(AnsiString FileName,bool *Load_Succesfull);
	AnsiString load_navx_geo_file(AnsiString FileName, AnsiString Selected_Geo_Name,
		Surface_Class *Surface,pugi::xml_document *Geometry_pugi_object);

	// data load
	AnsiString read_navx_velocity_data_file(AnsiString FileName, Data_Point_Set_Class *Data_Point_Set, STUDY_Class *Study);
	AnsiString read_old_navx_data_file(AnsiString FileName, Data_Point_Set_Class *Data_Point_Set);

	void export_master_NavX_files_Geoff_v1(STUDY_Class *STUDY,TProgress_Form *Progress_Form,AnsiString FileName);

	void read_navx_AutoMarkSummaryList_File(AnsiString FileName,Surface_Class *Surface);

	//---------------------------------------------------------------------------
	// CARTO folder import
	//---------------------------------------------------------------------------
	AnsiString import_CARTO_folder(TFileListBox* Data_FileListBox,
				STUDY_Class *Study,TProgress_Form *Progress_Form,
				bool Import_All_ECG_Channels_Flag,bool Import_PASO_data,
				bool CS_As_Ref,bool All_Maps_Import, bool Append,bool Geoff_Export_Flag );
	AnsiString import_single_CARTO_map(TFileListBox* Data_FileListBox,AnsiString Mesh_File_Name,
				std::vector <AnsiString> All_Files_Names,
				STUDY_Class *Study,TProgress_Form *Progress_Form, bool Import_All_ECG_Channels_Flag,
				bool Import_PASO_data, bool CS_As_Ref, bool All_Maps_Import,bool Geoff_Export_Flag);

	void export_master_CARTO_files_Geoff_v1(STUDY_Class *STUDY,TProgress_Form *Progress_Form,AnsiString FileName);


	//---------------------------------------------------------------------------
	// vtk support
	//---------------------------------------------------------------------------
	AnsiString import_vtk_file(AnsiString FileName,STUDY_Class *STUDY,
			TProgress_Form *Progress_Form,bool Append);
	bool export_geo_as_vtk_file(AnsiString FileName,Surface_Class *Surface,bool Original_xyz);

	//---------------------------------------------------------------------------
	// stl import/export
	//---------------------------------------------------------------------------
	bool export_geo_as_stl_file(AnsiString FileName,Surface_Class *Surface,int Mode,
			Segments_List_Class* Segments_Info);


	//---------------------------------------------------------------------------
	// images import
	//---------------------------------------------------------------------------
	AnsiString import_images_files(TStrings* FileNames,STUDY_Class *STUDY,
			TProgress_Form *Progress_Form,bool Append);


	//---------------------------------------------------------------------------
	// Rhythmia map import
	//---------------------------------------------------------------------------
	AnsiString import_rhythmia_file(AnsiString Path, AnsiString FileName,STUDY_Class *STUDY,
			TProgress_Form *Progress_Form,bool Append);
	AnsiString import_octave_electrode_file(AnsiString FileName,STUDY_Class *STUDY,
			TProgress_Form *Progress_Form);


	//-------------------------------------------------------------------------
	// Various import/export functions
	//-------------------------------------------------------------------------
	void save_values_at_geometry_nodes(AnsiString FileName,STUDY_Class *STUDY);
	void save_values_at_data_points(AnsiString FileName,STUDY_Class *STUDY);

	//-------------------------------------------------------------------------
	// Application settings
	//-------------------------------------------------------------------------
	Application_Settings_Class Application_Settings;

	Numerical_Library PNUM;
};

//------------------------------------------------------------------------------

#endif // DATA_IO_CLASS_H
