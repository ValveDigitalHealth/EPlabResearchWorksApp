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
#ifndef SurfaceH
#define SurfaceH

#include <vector>
#include <fstream>
#include <math.h>
#include <algorithm>

#include "Progress_Bar_Form.h"

#include "Ablation_Point.h"
#include "Application_Settings.h"
#include "Geometry_Triangle.h"
#include "Geometry_Vertex.h"
#include "Data_Point_Set.h"
#include "Label.h"
#include "Phase_Singularity_Class.h"
#include "predefined_parameters.h"
#include "Numerical_Library.h"
#include "Values_List_Class.h"
#include "Images_List_Class.h"
#include "Contour_Point_Class.h"
#include "Contour_Line_Class.h"
#include "Island_on_3D_map.h"
#include "Recurrence_Diagram_Class.h"
#include "List_of_pairs.h"

class Surface_Class
{
	//---------------------------------------------------------
	//---------------------------------------------------------
	private:
	//---------------------------------------------------------
	//---------------------------------------------------------

	AnsiString SourceFileName = ": Surface";

	//---------------------------------------------------------
	//---------------------------------------------------------
	public:
	//---------------------------------------------------------
	//---------------------------------------------------------

	Utils_Tools Utils;
	Numerical_Library PNUM;

	Surface_Class();
	~Surface_Class();

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File,TProgress_Form *Progress_Form);
	int load_object_from_stream(ifstream* File,TProgress_Form *Progress_Form);

	//---------------------------------------------------------------------------------
	// Name of this surface and basic info
	//---------------------------------------------------------------------------------
	AnsiString Name;
	int Geometry_Type; // 0:nothing special, 1:rectangle, 2:3Dgeo
	int Mapping_System_Source; // defined in "predefined_parameters.h"

    // range of interpolation around each data point
	double Data_Points_Filling_Threshold_mm;

	//---------------------------------------------------------------------------------
	// Initializaiton
	//---------------------------------------------------------------------------------
	void initialize_surface(TProgress_Form *Progress_Form); // call it after import, load or generation of new geometry

	//---------------------------------------------------------------------------------
	// Sets of the elemtents forming 3D map
	//---------------------------------------------------------------------------------

	std::vector<Geometry_Vertex> Surface_Node_Set;
	std::vector<Geometry_Triangle> Surface_Triangle_Set;

	std::vector<Data_Point_Set_Class> Data_Point_Set; // Set of data point sets

	// PASO points
	Data_Point_Set_Class PASO_IS_Point_Set;
	Data_Point_Set_Class PASO_PM_Point_Set;

	// Set of ablations points
	std::vector<Ablation_Point_Class> Ablation_Points_List;

	// Set of labels
	std::vector<Label_Class> Labels;
	void create_labels_for_data_points_with_specified_type(int Type,
		int DataPointsSetPtr, AnsiString LabelName);

	//---------------------------------------------------------
	// IMAGES DATA
	//---------------------------------------------------------
	Images_List_Class Images_Set;

	//---------------------------------------------------------------------------------
	// SURFACE VALUES SUPPORT
	//---------------------------------------------------------------------------------
	Values_List_Class Map_Values;

	bool node_values_valid(long Node_Ptr);
	bool node_values_valid_specific_value(long Node_Ptr,long Val_Ptr);
	bool triangle_values_valid(long Triangle_Ptr);
	bool triangle_values_valid_specific_value(long Triangle_Ptr,long Val_Ptr);

	int calculate_statistics_for_Values(int Seg_No,int Val_Id,
				double* av,double* median,double* min,double* max,double* sd,double* covc,
				int DP_Set,bool Exclude_Zero_Values);

	int calculate_area_taken_by_Value(int Seg_No,int Val_Id,double Threshold,int Direction,
				double* Area_Taken,double* Total_Area,int DP_Set);
	int calculate_area_taken_by_Value_within_range(int Seg_No,int Val_Id,
		double Threshold_Low,double Threshold_High,
		double* Area_Taken,double* Total_Area,int DP_Set);

	double get_map_percentile(int Val_Id, int DP_Set,int Percentile);

	int smooth_map_value(int Data_Point_Set_Ptr,int Value_Id);
	int compute_min_max_values_on_surface();

	void check_values_for_abnormal_values();

	// for display purposes, calculated in OpenGL_Panel_Class::prepare_colors_for_display()
	double Mean_Map_Value,SD_of_Map_Value,Volume,Area;

	//---------------------------------------------------------------------------------
	// Currently selected and pointed items
	//---------------------------------------------------------------------------------
	long Pointed_Triangle;

	long Current_Data_Point_Set_Ptr;
	long Current_Data_Point_Ptr;
	long Current_Deleted_Data_Point_Ptr;
	long Previous_Current_Data_Point_Ptr;
	int Pointed_Label;

	// Check whether selected items are in range
	bool data_points_set_ptr_in_range();
	bool data_point_ptr_in_range();
	bool deleted_data_point_ptr_in_range();


	bool Display_Geometry_Flag;
	bool Display_Whole_Dataset_Flag;

	//---------------------------------------------------------------------------------
	// DATA POINTS SUPPORT
	//---------------------------------------------------------------------------------
	long  get_data_point_total_number_in_all_sets();

	int get_data_point_segment_id(int Dset, int DP);
    bool is_segment_present_at_data_point(int Dset,int DP,int Segment_Id);

	long get_number_of_data_points_in_segment(int Dset,int Segment_Id,AnsiString Value_Name,
						vector <Value_Description_Class> *Values_List);

	int force_data_points_to_surface(int Data_Point_Set_Ptr);
	int restore_original_data_points_locations(int Data_Point_Set_Ptr);

	long delete_floating_data_points(int DSet_Ptr, double Distance_Th,bool Hard_Delete);

	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	// GEOMETRY MANIPULATION FUNCTIONS
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	void remove_smaller_disjointed_surface_parts();
	void remove_surface_related_with_specific_segment(int Segment_Id);
	long node_xyz_present(std::vector<Geometry_Vertex> *Node_Set,double x, double y, double z);

	void get_surface_dimensions(double* x,double* y,double* z);

	void create_deflection_in_surface(double Cx,double Cy, double Cz,
						double Radius, double Depth,int Type); // 2: uniform elevation

	void center_geometry_and_data_points();
	double Translation_Vector_X,Translation_Vector_Y,Translation_Vector_Z;

	void restore_original_xyz_coordinates_from_import();

	void downsample_mesh(AnsiString AppPath, double Ratio, TProgress_Form *Progress_Form);
	void upsample_mesh();

	int smooth_map_geometry();

	void segment_disjonted_elements();

	int calculate_normals();
	void invert_normals();
	int smooth_normals(double Range);

	double get_volume();
	double get_total_area();
	void rescale_geometry_to_reach_specified_volume(double Target_Volume);
	void rescale_geometry(double Scale);

	void make_geometrical_transformation(double X, double Y, double Z,
						double Teta, double Fi, double aX, double aY, double aZ, int Seg_No);
	void get_xyz_centre_of_triangle(long Triangle_Id,double* x,double* y,double* z);
	void calculate_areas_of_triangles();
    double get_mean_triangle_area();

	int generate_square_sample(TProgress_Form *Progress_Form,int Square_Width,int Square_Height, double Nodes_Spacing);
	int generate_wheel_sample(TProgress_Form *Progress_Form,int Radius, double Nodes_Spacing);

	void cut_geometry_to_confine_to_radius(double Radius);


	//---------------------------------------------------------
	// INTERPOLATION
	//---------------------------------------------------------

	// "diffusion" interpolation
	void project_data_points(int Data_Points_Set_Ptr);
	void project_data_points_specific_value(int Data_Points_Set_Ptr,int Value_Ptr);

	double Interpolation_Stop_Threshold;
	int interpolate_one_value_one_step(int v_1_ptr,
			bool* Continue_Interpolation_Flag,int Data_Points_Set_Ptr);

	void interpolate_specific_value(int Algorithm_Type, int Data_Point_Set_Ptr, int Value_Ptr,
		TProgress_Form *Progress_Form);

	void interpolate_all_values(int Algorithm_Type, int Data_Point_Set_Ptr,
		TProgress_Form *Progress_Form);

	void interpolate_LAT_value(int Algorithm_Type, int Data_Point_Set_Ptr,
		TProgress_Form *Progress_Form);

	void interpolate_Bipolar_amplitude_value(int Algorithm_Type, int Data_Point_Set_Ptr,
		TProgress_Form *Progress_Form);


	//-----------------------------------------------------------------
	// closest nodes-dp's
	//-----------------------------------------------------------------

	void calculate_closest_nodes_and_data_points_ALL(int Data_Point_Set_Id);       // ok
	void update_closest_nodes_and_dps_around_xyz(double x, double y, double z,long Data_Point_Set_Id); // ok

	void find_closest_data_point_to_node(long Node_Ptr,long Data_Point_Set_Id);  // ok
	void find_closest_node_to_data_point(long DP_Ptr,long Data_Point_Set_Id);    // ok


	void update_closest_nodes_and_dps_after_dp_removal(long DS,long DP); // ok
	void update_closest_nodes_and_dps_after_dp_undeleted(long DS,long DP);


	void find_neig_nodes_and_neig_triangles();

	long get_closest_node_to_xyz(double x,double y,double z,double Max_Distance_Th_mm);

	//---------------------------------------------------------------------------------
	// VARIOUS MAPS CALCULATION FUNCTIONS
	//---------------------------------------------------------------------------------
	AnsiString calculate_conduction_velocity(int Base_Map_Value_Ptr);
	AnsiString calculate_egm_duration_map(int Data_Point_Set_Id,Computational_Module_Class *Comp_Module_Ptr);
	AnsiString calculate_AFCL_map(int Data_Point_Set_Id,double Base_AFCL_ms,Computational_Module_Class *Comp_Module_Ptr);

	AnsiString calculate_voltage_amplitude_map(int Data_Point_Set_Id,Computational_Module_Class *Comp_Module_Ptr);

	AnsiString calculate_microfractionation_map(int Data_Point_Set_Id,Computational_Module_Class *Comp_Module_Ptr);

	AnsiString calculate_electric_dispersion_map(int Data_Point_Set_Id,int Algorithm,
		double Base_AFCL_ms,bool Auto_Range,double Disp_Range_ms,int Min_DP_Number,
		Computational_Module_Class *Comp_Module_Ptr);


	double get_min(double A,double B,double C);
	double get_middle(double A,double B,double C);
	double get_max(double A,double B,double C);

	//----------------------------------------------------------------------
	// Contours support
	//----------------------------------------------------------------------
	void generate_contours(double Threshold_Interval);
	vector <Contour_Point> Point_Set;
	vector <Contour_Line> Contours_Set;

	//---------------------------------------------------------------------------------
	// Paths
	//---------------------------------------------------------------------------------
	std::vector<long> Path_Vector;
	long Path_Start, Path_Intermediate, Path_End;
	void compute_paths_between_start_and_end();


	//---------------------------------------------------------------------------------
	// SEGMENTS SUPPORT
	//---------------------------------------------------------------------------------
	void restore_previous_segmentation_state();
	void save_segmentation_state();

	void calculate_segments_of_data_points(int Dset);

	double get_area_of_given_segment_cm2(int Segment_Id);
	bool segmentation_expand_segment_one_step(int Segment_Id,bool Block_Existing);
	void segmentation_shrink_segment_one_step(int Segment_Id,bool Block_Existing);

	void compute_segments_centers(int Max_Segments_Number);
	vector<bool> Segments_Present;
	vector<double> Segments_Centers_X;
	vector<double> Segments_Centers_Y;
	vector<double> Segments_Centers_Z;
	vector<double> Segments_Centers_X_normal;
	vector<double> Segments_Centers_Y_normal;
	vector<double> Segments_Centers_Z_normal;

	bool is_segment_present(int Segment_Id);

	// 3-polar plot support
	long Apex_Node_Ptr;
	long Septal_Anterior_Node_Ptr;
	long Septal_Lateral_Node_Ptr;
	long Anterior_Lateral_Node_Ptr;

	long Ap_SA_Intermediate;
	long Ap_SL_Intermediate;
	long Ap_AL_Intermediate;

	long Ant_Intermediate;
	long Sept_Intermediate;
	long Lat_Intermediate;

	vector<long> Ap_SA_Path; // lists of nodes forming a shortest path
	vector<long> Ap_SL_Path;
	vector<long> Ap_AL_Path;
	vector<long> Septal_Base_Path;
	vector<long> Anterior_Base_Path;
	vector<long> Lateral_Base_Path;
	void compute_paths_for_4_point_LV_segmentation();
	vector <long> get_shortest_path_between_nodes(long Node_1, long Node_2);
	double get_path_length(long Node_1,long Node_2);
	double get_path_value_difference(long Node_1,long Node_2);
	double get_distance_to_path(long Node_Id,vector <long> *Path);
	int propagate_region(int Region_Code,long Seed_Node_Ptr);

	void initiate_propagation(int Region_Code,long Seed_Node_Ptr);
	void propagate_region_one_step(int Region_Code);
	vector <long> List;


	double get_euclidean_distance_between_nodes(long n1, long n2);

	int is_node_part_of_segment( int Node_Ptr, int Seg_No);

	void get_segment_id_with_minmax_value(int DSet, int Val_Ptr, int* Min_Segment_Id, int* Max_Segment_Id );
	void get_nodes_ptrs_with_minmax_value(int DSet, int Val_Ptr, long* Min_Node_Ptr, long* Max_Node_Ptr );


	//-------------------------------
	// ISLANDS ANALYSIS
	//-------------------------------

	bool get_islands_statistics(int Value_Ptr,int Data_Point_Set_Ptr,
		double Threshold, int Direction, int *Number,double *PercArea,
		double *Area,bool Only_Nodes_With_Close_DP, double Min_Island_Area);

	std::vector<Island_on_3D_Map> get_islands(int Value_Ptr,int Data_Point_Set_Ptr,
		double Threshold,int Direction,bool Only_Nodes_With_Close_DP,double Min_Island_Area);

	void expand_seed(int Value_Ptr,int Data_Point_Set_Ptr,double Threshold,
					 int Direction,Island_on_3D_Map *Island,bool Only_Nodes_With_Close_DP);

	std::vector<double> get_data_of_biggest_island(int Value_Ptr,int Data_Point_Set_Ptr,
		double Threshold, int Direction, bool Only_Nodes_With_Close_DP);

	std::vector<long> get_nodes_of_biggest_island(int Value_Ptr,int Data_Point_Set_Ptr,
		double Threshold, int Direction, bool Only_Nodes_With_Close_DP);

	std::vector<long> get_nodes_of_island_with_max_value(int Value_Ptr,int Data_Point_Set_Ptr,
		double Threshold, int Direction, bool Only_Nodes_With_Close_DP);

	//-------------------------------------------------------------------------
	// 3 Points located on surface used for image registration
	//-------------------------------------------------------------------------
	Point_3D Registration_Point_A;
	Point_3D Registration_Point_B;
	Point_3D Registration_Point_C;
	bool Point_A_Located;
	bool Point_B_Located;
	bool Point_C_Located;
	Point_3D Registration_Point_A_normal; // normals are used in displaying name
	Point_3D Registration_Point_B_normal;
	Point_3D Registration_Point_C_normal;
	void translate_ABC_spatial_registration_markers(double x,double y,double z);
	void scale_ABC_spatial_registration_markers(double x,double y,double z);

	void perform_one_step_of_triangles_ABC_fitting(
			Point_3D FRegistration_Point_A,
			Point_3D FRegistration_Point_B,
			Point_3D FRegistration_Point_C );

	void set_phase_value_of_map_to_specific_time_point(long TimePoint_ptr);

	long get_central_node(vector <long> *Nodes_Ptrs);

	std::vector <long> get_histogram_of_value_gradient(int Data_Point_Set_Ptr,int Segment_Id,
		double Neighb_Range_mm, AnsiString Value_Name,
		double* Min, double* Box_Size,bool Only_DPs_With_Specified_Flag_A,int Flag_A_Value);

	void create_value_based_elevation_in_surface(int Data_Point_Set_Ptr,
		AnsiString Value_Name,double Threshold, int Direction, double Height_mm);

};

//---------------------------------------------------------------------------

#endif
