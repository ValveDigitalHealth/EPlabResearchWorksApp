//---------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <vector>
#include <complex>
#include <math.h>

#ifndef ButterworthBandpass_FilterH
#define ButterworthBandpass_FilterH
//---------------------------------------------------------------------------

class Butterworth_BandPass_Filter
{
	public:

	Butterworth_BandPass_Filter();
	~Butterworth_BandPass_Filter();

	void save_filter_to_file(std::string Filename);

	std::vector<double> NumC;
	std::vector<double> DenC;

	int Filter_Order;

	void Initialize_Filter(int Filter_Order_Init,double LowCutoff,double HighCutoff);

	double* ComputeLP( int FilterOrder );
	double* ComputeHP( int FilterOrder );
	double* TrinomialMultiply( int FilterOrder,double *b, double *c );

	std::vector<double> ComputeNumCoeffs_v2(int FilterOrder,double Lcutoff, double Ucutoff);
	std::vector<double> ComputeDenCoeffs( int FilterOrder,double Lcutoff, double Ucutoff );

//	double* ComputeNumCoeffs_v2(int FilterOrder,double Lcutoff, double Ucutoff,double *DenC);
//	double* ComputeNumCoeffs(int FilterOrder); // to be removed
//	double* ComputeDenCoeffs( int FilterOrder,double Lcutoff, double Ucutoff );

	void filter(int np, double *x, double *y);

	std::vector <double> filter(std::vector <double> *Signal,bool Inverse);

	// real time filtering
	std::vector <double> X;
	std::vector <double> Y;
	double filter_sample(double Value);
};

#endif
