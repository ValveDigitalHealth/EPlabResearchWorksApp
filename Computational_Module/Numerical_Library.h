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
#ifndef Numerical_LibraryH
#define Numerical_LibraryH
//---------------------------------------------------------------------------

#include <vcl.h>

//---------------------------------------------------------------------------

#include <stdio.h>
#include <memory.h>
#include <time.h>
#include<stdint.h>
#include <limits>
#include <vector>
#include <complex>
#include <algorithm>    // std::sort
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <complex>
#include <iostream>
#include <valarray>

#include "vector_3d.h"
#include "Matrix_2D.h"
#include "Matrix_3D.h"
#include "Parametric_3D_Line_Class.h"
#include "Point_2D_Class.h"
#include "Point_3D_Class.h"
#include "ButterworthBandpass_Filter.h"
#include "openglpicking/maths_funcs_for_ray_picking.h"
#include "spline.h"
#include "Local_Activation.h"

#include<stdint.h>

typedef int TimeType;

//---------------------------------------------------------------------------

#ifndef NOT_POSSIBLE_TO_CALCULATE_VALUE
#define NOT_POSSIBLE_TO_CALCULATE_VALUE -10000
#endif

//-----------------------------------------------------------------------

class Numerical_Library
{
	public:

	Numerical_Library();

	Butterworth_BandPass_Filter Butt_Filter;

	//------------------------------------------------------------
	//------------------------------------------------------------
	// STATISTICAL TESTS
	//------------------------------------------------------------
	//------------------------------------------------------------

	// Rayleigh test for circular uniformity
	void circ_rtest(std::vector<double>* Angle_Data, double* p_val,double* z_stat);
	double circ_r(std::vector<double>* Angle_Data);

	//---------------------------------------------------------------------------
	// min max calculations
	//---------------------------------------------------------------------------
	void find_min_max(std::vector<double> *Signal,double* Min, double* Max, long *Min_Ptr, long *Max_Ptr);

	template <typename TV>
	void calculate_max_min_mean_vec_ranged(std::vector<TV> *Signal,
				long Begin,long End,double* Min, double* Max, double* Mean,double* SDev);

	long calculate_max_min_mean_vec_long(std::vector<long> *Signal,long Begin, long End,
		double* Min, double* Max, double* Mean, int* Peak_Direction);

	void calculate_max_min_mean_vec_double(std::vector<double> *Signal,
				double* Min, double* Max, double* Mean);

	void calculate_max_min_vec_double_ranged(std::vector<double> *Signal,
			long Begin, long End,double* Min, double* Max);

	void calculate_max_min_mean_SD_vec_double(std::vector<double> *Signal,
				double* Min, double* Max, double* Mean, double* SD);

	void calculate_max_min_mean_slope_vec_ranged(std::vector<double> *Signal,
				long Begin,long End,double* Max_slope, double* Min_slope);

	template <typename TV>
	double get_sum_of_vector(std::vector<TV> *Signal);

	double get_average_from_vector(std::vector<double> *Signal);

	//----------------------------------------------------------------------------
	// correlation calculations
	//----------------------------------------------------------------------------
	void correlation_coef_vec(std::vector <double> *vec1, std::vector <double> *vec2, double* corr);

	double correlation_coef_vec_different_size(std::vector<double> *v1,std::vector<double> *v2,
		long Start_Position_in_v1);
	double covariance_coef_vec_different_size(std::vector<double> *v1,std::vector<double> *v2,
		long Start_Position_in_v1);
	double psimilarity_coef_vec_different_size(std::vector<double> *v1,std::vector<double> *v2,
		long Start_Position_in_v1);

	double get_amplitude_of_egms_difference(std::vector<double> *v1,std::vector<double> *v2);


	std::vector <double> get_correlation_vector(std::vector <double> *v1,
		std::vector <double> *v2, long Step  );
	std::vector <double> get_xcorrelation_vector(std::vector <double> *v1,
		std::vector <double> *v2, long Step  );
	std::vector <double> get_covariance_vector(std::vector <double> *v1,
		std::vector <double> *v2, long Step  );
	std::vector <double> get_psimilarity_vector(std::vector <double> *v1,
		std::vector <double> *v2, long Step  );

	void stdev_cor_vec(std::vector <double> *vec, double* average, double* SDev);
	void covariance_vec(std::vector <double> *vec1, std::vector <double> *vec2, double* cov);

	void stdev_cor_vec_double(std::vector <double> *vec, double* average, double* SDev);
	void stdev_cor_vec_long(std::vector <long> *vec, double* average, double* SDev);

	double get_non_zero_mean(double v1, double v2);

	// Histogramming
	std::vector <long> get_histogram(
		std::vector <double> *Data,
		double Box_Size,
		bool Use_SD_Box_Size,
		double* Bin_Size_from_SD);

	double get_percentile_from_histogram(std::vector<long>* Histo,double Percentile,
						double Start_Value, double Box_Size);

	// Filtering
	double get_median(std::vector<double> Data);
	int median_filter(std::vector<double> &Signal,long Window_Length);

	void rescale_signal(std::vector <double>* Signal,double Min,double Max);


	// Hilbert transfrom using convolution
	void convol(std::vector<double> *source, std::vector<double> *target,
		std::vector<double> *filt, int npt, int lfilt);
	std::vector <double> get_Hilbert_phase_using_convolution_double(std::vector<double> *Signal,
			double Time_Step_ms);

	std::vector <double> get_electrogram_phase(
			std::vector <double>* Signal,int EGM_Type, double Base_Period_ptr,double Time_Step_ms);
	std::vector <Local_Activation_Class> get_local_activations_from_phase_signal(
			std::vector <double>* Signal,int EGM_Type, double Time_Step_ms);


	  // DYSSYNCHRONY FUNCTIONS
	  std::vector <double> recompose_bipolar_signal(std::vector<double> *Signal,int Wavelet_Period,int Electrogram_Type);
	  std::vector <double> recompose_bipolar_signal(std::vector<float> *Signal,int Wavelet_Period,int Electrogram_Type);
	  std::vector <long>   recompose_bipolar_signal(std::vector<long> *Signal,int Wavelet_Period,int Trigger_Type);

	  double get_phase_synchronization(std::vector<double> *Signal_A,
		std::vector<double> *Signal_B,int n,int m,bool Take_80_Perc_Flag,long Delay);
	  double get_phase_synchronization_long(std::vector<long> *Signal_A,
		std::vector<long> *Signal_B,int n,int m,bool Take_80_Perc_Flag,long Delay);

	  std::vector <double> unwrap_phase(std::vector <double> *Phase_Vec);

	  double get_MPC_between_electrograms(std::vector<double> *Electrogram_1,
                                          std::vector<double> *Electrogram_2,
										  long Basic_Cycle_Length,int Electrogram_Type,
										  long Delay,double Time_Step_ms);

	  void get_rgb_colors_from_ranged_variable(double Min,double Max, double Value,
											   int* R, int* G,int* B);

	std::vector <double> Butt_band_40_250_3th_order(std::vector <double> *Signal, bool Inverse);
	std::vector <double> rectify_signal(std::vector <double> *Signal);
	std::vector <double> Butt_low_20_3th_order(std::vector <double> *Signal, bool Inverse);
	double x7[7],y7[7];
	double x4[7],y4[7];


	long get_position_of_peak(std::vector<long> *Signal,long Start, long Stop);


	std::vector <long> calculate_peak_positions(double* values,long n,double threshold);
	std::vector <long> calculate_peak_positions_vec(std::vector<double> *values,
		double threshold,double margin_ratio);

	void find_max_peak_with_max_slopes(std::vector<double> *Signal,long Start, long Stop,
										double* Max, long *Max_Ptr,int Slope_Range_Ptr);


	int get_triangle_based_CV(		 double N1_x,double N1_y,double N1_z,
									 double N2_x,double N2_y,double N2_z,
									 double N3_x,double N3_y,double N3_z,
									 double LAT_N1,double LAT_N2,double LAT_N3,
									 double* CV_magnitude,
									 double* CV_x, double* CV_y, double* CV_z, bool* CV_valid);

	void rotate_xyz_in_cartesian_system(double* x,double* y,double* z, double angle, int which_axis);
	double get_min(double A,double B,double C);
	double get_middle(double A,double B,double C);
	double get_max(double A,double B,double C);

	// CAUSALITY ANALYSIS

	long get_number_of_words_causality_alphabet_method(std::vector <long> *Sender,
			std::vector <long> *Receiver,int Word_Length);

	void rotate_vector(double* x,double* y,double* z, double angle, int which_axis);

	bool is_value_present_in_vector_long(std::vector<long> *Vec,long Value);

	template <class TV>
	bool is_value_present_in_vector(std::vector<TV> *Vec,TV Value)
	{
		for(unsigned long i=0;i<Vec[0].size();i++)
		if( Vec[0][i]== Value )
		return true;

		return false;
	}

//--------------------------------------------------------------------------


	bool is_value_present_int(int* Set,int Size, int Val);

	long get_min_difference(long T,std::vector <long> *Vec);

	double get_micro_fractionation(std::vector<double> *Signal,int Algorithm,
		double Percentile_Threshold, int Window_Size);


	//-----------------------------------------------------------------------
	template <class T1>
	std::vector <T1> downsample_by_factor(std::vector <T1> *X, long Factor)
	{
		std::vector <T1> Y;
		for(long i=0;i<X[0].size();i+=Factor)
			Y.push_back(X[0][i]);
		return Y;
	}
	//-----------------------------------------------------------------------

	//-----------------------------------------------------------------------
	template <class T1>
	std::vector <T1> resample(std::vector <T1> *X, long Start, long Stop, long Target_Length)
	{
	std::vector <T1> Y;
	long Xs = Stop-Start; // X[0].size();
	long L,R;
	double Pos,v,vL,vP;

	for(long i=0;i<Target_Length;i++)
	{
		L = Start+floor((double)i/(double)Target_Length*Xs);
		Pos = Start+(double)i/(double)Target_Length*Xs - L;
		R = L+1;

		if( L < X[0].size() && R < X[0].size() )
		{
			vL = X[0][L];
			vP = X[0][R];
			v = (1-Pos)*vL + Pos*vP;
		}
		else
		v = 0;

		Y.push_back(v);
	}


	return Y;
	}
	//-----------------------------------------------------------------------

	double Dot_Product(Point_3D* A,Point_3D* B);

	double get_distance_between_3D_point_and_triangle(double Px,double Py,double Pz,
													  double T1x,double T1y,double T1z,
													  double T2x,double T2y,double T2z,
													  double T3x,double T3y,double T3z );

	double get_Shannon_Entropy(std::vector<double> *signal, bool Use_SD, double Bin_Size);
	double get_Shannon_Entropy_from_histogram(std::vector<long> *Histogram);

	std::complex <double> morlet_wavelet(double t);
	std::vector <double> make_Continious_Wavelet_Transformation(
			std::vector <double> *Data,long Start, long Stop, double Scaling_Factor );
	std::complex<double> CWT(std::vector<double> *Data,long Start,long Stop,
							double timestep,double a,double tau);

	//----------------------------------------------------------------------------
	// Peak detection stuff
	// Peak_Detection_Window and Blanking_Window should be in sampling units (not ms)
	//----------------------------------------------------------------------------
	template <class T1>
	std::vector <long> get_peak_positions(std::vector<T1> *Data,
		double PDT,long Peak_Detection_Window,long Blanking_Window);

	std::vector <double> normalize_signal(std::vector<double> *Signal,int Window_Length);
	std::vector <double> get_sliding_SD_of_signal(std::vector<double> *Signal,int Window_Length);
	std::vector <double> get_sliding_maxmin_of_signal(std::vector<double> *Signal,int Window_Length);
	std::vector <double> get_abs_sum_in_window(std::vector<double> *Signal,long Window_Length);
	std::vector <double> set_av_to_0_SD_to_1(std::vector<double> *Signal);

	void calculate_SD_vec_double_ranged(std::vector<double> *Signal,long Begin,long End,double* SD);

	std::vector <double> calculate_NAVX_CAFE(long Start,long Stop,
		std::vector <double>* Signal,double Refraction_Time,double PP_Sensitivity,
		double Duration, double Downstroke_Threshold, double* CAFE);

	void fit_line_vertical_offsets(double* x, double* y, long start, long stop,
							 double* a, double* b, double* F, double* R);

	void fit_line_vertical_offsets_vec(std::vector <double> *y,
									 double* a, double* b, double* F, double* R);

	void fit_line_perpendicular_offsets(double* x, double* y, long start, long stop,
							 double* a, double* b, double* F, double* R);

	void fit_line_diff_phi_offsets(double* x, double* y, long start, long stop,
							 double* a, double* b, double* F, double* R);


	void find_crossing_point_3D_parametric_lines(
			Parametric_3D_line Line1,
			Parametric_3D_line Line2,
			double* Distance,
			Point_3D Crossing_Point );

	bool find_perpendicular_parametric_line_from_3_points(
		Point_3D Point_A, // coordinates of the points
		Point_3D Point_B, // line will go through point A
		Point_3D Point_C,
		Parametric_3D_line* Line );

	bool solve_set_of_linear_equations_with_2_unknowns(
			double a1,double b1,double c1, // |a1*x+b1*y=c1|
			double a2,double b2,double c2, // |a2*x+b2*y=c2|
			double* x,double* y);

	void get_vector_product(double a1,double a2,double a3,
							double b1,double b2,double b3,
							double* x,double* y,double* z);

	void find_line_pasing_two_points_in_3D(
		Point_3D Point_A, // coordinates of the points
		Point_3D Point_B, // line will go through point A and B
		Parametric_3D_line* Line );

	void rotate_xyz_in_polar_system(double* x,double* y,double* z,double Azimuth,double Zenith);

	bool intersectRayTriangle(Vector3d orig, Vector3d dir,
		Vector3d vert0, Vector3d vert1, Vector3d vert2,
		Vector3d baryPosition, double *distance);

	void get_barycentric_coordinates_within_triangle_of_projected_3D_point(
			double Origin_x,double Origin_y,double Origin_z,
			double T1_x,double T1_y,double T1_z,
			double T2_x,double T2_y,double T2_z,
			double T3_x,double T3_y,double T3_z,
			double *Alpha,double *Beta,double *Gamma);

	unsigned char ToByte(bool Bits[8]);
	void FromByte(unsigned char c,bool Bits[8]);

	double sign(double val);

	//-------------------------------------------------------------------------------
	// transfer entropy support
	//-------------------------------------------------------------------------------
	std::vector<int> create_01_signal_from_egm(
		std::vector<double> *Signal_1,std::vector<Local_Activation_Class> *Activations_Positions,int Width_samples);

	std::vector<int> binary2asdf(std::vector<int> *V);

	void get_Transfer_Entropy(std::vector<int> *Binary_Signal_1,std::vector<int> *Binary_Signal_2,
		double* TE_00,double* TE_01,double* TE_10,double* TE_11,int y_delay);

	void transent_1(TimeType **all_series, const size_t series_count,
		 const size_t *series_lengths,
		 const TimeType y_delay,
		 const TimeType duration,
		 double *te_result);

	void transent_ho(TimeType **all_series, const size_t series_count,
		 const size_t *series_lengths,
		 const size_t x_order, const size_t y_order,
		 const TimeType y_delay,
		 const TimeType duration,
		 double *te_result);

// ===========================================================================

};

#endif
