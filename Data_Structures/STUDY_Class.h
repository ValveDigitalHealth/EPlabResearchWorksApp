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
#ifndef STUDY_ClassH
#define STUDY_ClassH

#include "Surface.h"

#include "Numerical_Library.h"
#include "value_description_class.h"
#include "values_vector.h"
#include "Segments_List.h"
#include "Patient_Data.h"

#include "predefined_parameters.h"
#include "Progress_Bar_Form.h"

#include "Image_Matrix.h"
#include "Segmented_Image.h"


#include<vector>

//---------------------------------------------------------------------------

class STUDY_Class
{
	public:

	STUDY_Class();
	~STUDY_Class();

	Utils_Tools Utils;
	Computational_Module_Class Comp_Module; // !!!! SHOULD BE SINGLETON - BE CAREFUL TO USE JUST THIS INSTANCE
	Numerical_Library PNUM;

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	AnsiString Study_Source_Path; // path to file/folder used as a source - MODIFIED JUST ONCE
	AnsiString Study_Current_Path_And_File_Name; // path to current path and filename (updated every time save is performed)

	void post_study_load_processing();

	int save_study(AnsiString StudyPathandName,TProgress_Form *Progress_Form);
	int load_study(AnsiString StudyPathandName,TProgress_Form *Progress_Form);
	int load_study_append2nd_one(AnsiString StudyPathandName,TProgress_Form *Progress_Form);

	//---------------------------------------------------------
	// STUDY DETAILS (INCLUDING PATIENT DETAILS)
	//---------------------------------------------------------
	Patient_Data_Class Patient_Data;

	//---------------------------------------------------------
	// LIST OF 3D RECONSTRUCTED GEOMETRIES
	//---------------------------------------------------------
	std::vector <Surface_Class> Surfaces_List;


	void add_empty_surface(AnsiString Name);

	int get_current_surface_ptr();

	unsigned int Current_Surface;
	unsigned int Previous_Current_Surface;

	bool is_current_surface_in_range();
	bool is_valid_data_point_set_selected();
	bool is_valid_data_point_selected();
	bool is_valid_current_value_selected();

	void get_coordinates_of_nearest_surface_point(double original_x, double original_y, double original_z,
												  double* Nearest_x, double* Nearest_y, double* Nearest_z );


	int get_segment_id_at_given_node(int Geometry_Id,long Node_Id);
	AnsiString get_segment_name_at_given_node(int Geometry_Id,long Node_Id,
			Segments_List_Class* Segments_Info);

	void perform_17_nuclear_segmentation(TProgress_Form *Progress_Form,
				int Source_Surface,Segments_List_Class* Segments_Info);

	void calculate_inter_chamber_coherence_map(int Base_Surface,int Target_Surface,
		int Base_Surface_DS,int Target_Surface_DS,double Base_AFCL_ms);

	int compute_min_max_values();
	double Min_Value_On_All_Maps,Max_Value_On_All_Maps;

	AnsiString project_surface_onto_other_surface_ray_intersection_as_value(int Projected_Geometry,
		int Base_Geometry,double Threshold,int Data_Point_Set_Ptr, AnsiString Geo_Value_Name,
		TProgress_Form *Progress_Form,bool In_Out_Test);

	//-----------------------------------------------------
	// geo fitting suppport
	//-----------------------------------------------------
	void find_corresponding_nodes_in_2_geometries(int Surface_1,int Surface_2,
				double Max_Dist_Threshold);
	void get_mean_distance_between_geometries(int Geometry_1, int Geometry_2,
		double* mean,double* SD,double* Median, double Max_Dist_Threshold);
	void perform_one_step_of_triangles_fitting(
		int Surface_1,int Surface_2,
		int Surface_3,bool Fit_3rd_along_wiht_2nd,
		int Surface_4,bool Fit_4th_along_wiht_2nd );
	void one_step_of_whole_cloud_fitting(int Surface_1,int Surface_2,
			int Surface_3,bool Fit_3rd_along_wiht_2nd,
			int Surface_4,bool Fit_4th_along_wiht_2nd,double Spring_Constant,double dtF,double dtM);

	void center_geometries(int Start_Geo,int Stop_Geo);

};

#endif
