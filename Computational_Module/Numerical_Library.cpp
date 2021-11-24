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
#include "Numerical_Library.h"
//---------------------------------------------------------------------------

Numerical_Library::Numerical_Library()
{

	for(int i=0;i<7;i++)
	{
		x7[i]=0;
		y7[i]=0;
	}
	for(int i=0;i<4;i++)
	{
		x4[i]=0;
		y4[i]=0;
	}

}

//------------------------------------------------------------------
void Numerical_Library::circ_rtest(std::vector<double>* Angle_Data, double* p_val,double* z_stat)
{
//
//   Computes Rayleigh test for non-uniformity of circular data.
//   H0: the population is uniformly distributed around the circle
//   HA: the populatoin is not distributed uniformly around the circle
//   Assumption: the distribution has maximally one mode and the data is
//   sampled from a von Mises distribution!
//
//   Input:
//     Angle_Data	sample of angles in radians
//
//   Output:
//     pval  p-value of Rayleigh's test
//     z     value of the z-statistic
//
// PHB 7/6/2008
//
// References:
//   Statistical analysis of circular data, N. I. Fisher
//   Topics in circular statistics, S. R. Jammalamadaka et al.
//   Biostatistical Analysis, J. H. Zar
//
// Circular Statistics Toolbox for Matlab

// By Philipp Berens, 2009
// berens@tuebingen.mpg.de - www.kyb.mpg.de/~berens/circStat.html

    double r =  circ_r(Angle_Data);
    double n = Angle_Data[0].size();

    // compute Rayleigh's R (equ. 27.1)
    double R = n*r;

    // compute Rayleigh's z (equ. 27.2)
    z_stat[0] = R*R / n;

    // compute p value using approxation in Zar, p. 617
    p_val[0] = exp(sqrt(1+4*n+4*(n*n-R*R))-(1+2*n));
}

//---------------------------------------------------------------------------

double Numerical_Library::circ_r(std::vector<double>* Angle_Data)
{
//   function r = circ_r(alpha)
//   Computes mean resultant std::vector length for circular data.
//
//   Input:
//     alpha	sample of angles in radians
//
//   Output:
//     r		mean resultant length
//
// PHB 7/6/2008
//
// References:
//   Statistical analysis of circular data, N.I. Fisher
//   Topics in circular statistics, S.R. Jammalamadaka et al.
//   Biostatistical Analysis, J. H. Zar
//
// Circular Statistics Toolbox for Matlab

// By Philipp Berens, 2009
// berens@tuebingen.mpg.de - www.kyb.mpg.de/~berens/circStat.html

    //  compute sum of cos and sin of angles
    // r = sum(exp(i*alpha));
    if( Angle_Data[0].size() > 0 )
    {

    double S_Sum=0,C_Sum=0;
    for(unsigned long i=0;i<Angle_Data[0].size();i++)
    {
        S_Sum += sin(Angle_Data[0][i]);
        C_Sum += cos(Angle_Data[0][i]);
    }

    // obtain length
    // r = abs(r)./sum(w);
    return sqrt( S_Sum*S_Sum + C_Sum*C_Sum )/(double)(Angle_Data[0].size());

    }
	else
	return 1000000;
}

//--------------------------------------------------------------------------

void Numerical_Library::find_max_peak_with_max_slopes(
	std::vector<double> *Signal,long Start, long Stop,double* Max, long *Max_Ptr,int Slope_Range_Ptr)
{
	Max[0] = std::numeric_limits<double>::min();
	double Slopes_Sum;

	for(long i=Start+Slope_Range_Ptr; i<Stop-Slope_Range_Ptr; i++)
	{

	Slopes_Sum = 0;

	// 1. Slopes up
	for(int j=-Slope_Range_Ptr;j<0;j++)
	Slopes_Sum += fabs(Signal[0][i+j] - Signal[0][i+j-1]);

	// 2. Slopes down
	for(int j=1;j<Slope_Range_Ptr;j++)
	Slopes_Sum += fabs(Signal[0][i+j] - Signal[0][i+j-1]);

	if( Slopes_Sum > Max[0] )
	{
		Max[0] = Slopes_Sum;
		Max_Ptr[0] = i;
	}

	}
}

//--------------------------------------------------------------------------

void Numerical_Library::find_min_max(std::vector<double> *Signal,
				double* Min, double* Max, long *Min_Ptr, long *Max_Ptr)
{
	Min[0] = std::numeric_limits<double>::max();
	Max[0] = -std::numeric_limits<double>::max();

	// 1. min, max, mean
	for(long i=0; i<Signal[0].size(); i++)
	{

	if( Signal[0][i] > Max[0] )
	{
		Max[0] = Signal[0][i];
		Max_Ptr[0] = i;
	}

	if( Signal[0][i] < Min[0] )
	{
		Min[0] = Signal[0][i];
		Min_Ptr[0] = i;
	}

	}
}

//--------------------------------------------------------------------------

template <typename TV>
void Numerical_Library::calculate_max_min_mean_vec_ranged(
				std::vector<TV> *Signal,
				long Begin,long End,double* Min, double* Max, double* Mean,
				double* SDev)
{
	Min[0] =  std::numeric_limits<double>::max();
	Max[0] = -std::numeric_limits<double>::max();
	Mean[0] = 0.0;
	SDev[0] = 0;
	double counter=0;

	// 1. min, max, mean
	for(long i=Begin; i<End; i++)
	if( i >= 0  && i < (signed)Signal[0].size() )
	{

	Mean[0] += Signal[0][i];

	if( Signal[0][i] > Max[0] )
		Max[0] = Signal[0][i];

	if( Signal[0][i] < Min[0] )
		Min[0] = Signal[0][i];

	counter++;

	}

	if( counter != 0 )
	Mean[0] /= counter;
	else
	Mean[0]=0;

	// 2. calculate stdev
	double SD = 0.0;
	for(long i=Begin; i<End; i++)
	if( i >= 0  && i < (signed)Signal[0].size() )
		SD += pow( (double)(Signal[0][i] - Mean[0]), 2.0 );

	if (SD > 0 && Signal[0].size() != 0 )
	SDev[0] = sqrt( SD / (double)( Signal[0].size()-1 ) );
}
//---------------------------------------------------------------------------

template void Numerical_Library::calculate_max_min_mean_vec_ranged<double>(
	std::vector<double> *Signal,long Begin,long End,double* Min, double* Max, double* Mean,double* SDev);
template void Numerical_Library::calculate_max_min_mean_vec_ranged<float>(
	std::vector<float> *Signal,long Begin,long End,double* Min, double* Max, double* Mean,double* SDev);
template void Numerical_Library::calculate_max_min_mean_vec_ranged<long>(
	std::vector<long> *Signal,long Begin,long End,double* Min, double* Max, double* Mean,double* SDev);

//---------------------------------------------------------------------------

void Numerical_Library::stdev_cor_vec_double(std::vector <double> *vec,
	double* average, double* SDev)
{
	double SD,av = 0.0;
	long i;

	// 1. claculate average
	av = 0.0;
	for( i = 0; i <(signed) vec[0].size(); i++ )
		av += vec[0][i];

	if(vec[0].size()!=0)
		av  /= vec[0].size();
	else
		av = 0.0;

	average[0] = av;

	// 2. calculate stdev

	SD = 0.0;
	for( i = 0; i <(signed) vec[0].size(); i++ )
		SD += pow( (double)(vec[0][i] - av), 2.0 );

	if (SD > 0 && vec[0].size() != 0 )
	SDev[0] = sqrt( SD / ( vec[0].size()-1 ) );
	else SDev[0] = 0;

}

//---------------------------------------------------------------------------

void Numerical_Library::stdev_cor_vec_long(std::vector <long> *vec,
	double* average, double* SDev)
{
	double SD,av = 0.0;
	long i;

	// 1. claculate average
	av = 0.0;
	for( i = 0; i <(signed) vec[0].size(); i++ )
		av += vec[0][i];

	if(vec[0].size()!=0)
		av  /= vec[0].size();
	else
		av = 0.0;

	average[0] = av;

	// 2. calculate stdev

	SD = 0.0;
	for( i = 0; i <(signed) vec[0].size(); i++ )
		SD += pow( (double)(vec[0][i] - av), 2.0 );

	if (SD > 0 && vec[0].size() != 0 )
	SDev[0] = sqrt( SD / ( vec[0].size()-1 ) );
	else SDev[0] = 0;

}

//---------------------------------------------------------------------------

std::vector <long> Numerical_Library::get_histogram(std::vector <double> *Data,
		double Box_Size, bool Use_SD_Box_Size, double* Bin_Size_from_SD)
{
	double v,min,max,average,SD;
	long Histogram_Size,tmp,ptr;
	std::vector <long> Histo;

	//-----------------------------------------
	// 1. calculate mim, max, av and SD
	//-----------------------------------------
	stdev_cor_vec_double(Data,&average, &SD);
	calculate_max_min_mean_vec_double(Data,&min, &max, &average);

	//-----------------------------------------
	// 2. Calculate bin size
	//-----------------------------------------
	double Bin_Size;
	if( Use_SD_Box_Size == true )
	{
		Bin_Size = 0.125*SD;
		Bin_Size_from_SD[0] = Bin_Size;
	}
	else
	{
		Bin_Size = Box_Size;
		Bin_Size_from_SD[0] = Bin_Size;
	}

	//-----------------------------------------
	// 3. Calculate number of bins and allocate histogram
	//-----------------------------------------
	if( Bin_Size != 0 )
	Histogram_Size = (max-min)/(double)Bin_Size;
	else
	{
		Bin_Size = 1;
		Bin_Size_from_SD[0] = Bin_Size;
		Histogram_Size = (max-min)/(double)Bin_Size;
	}

	Histo.assign(Histogram_Size,tmp);

	for(long k=0;k<Histogram_Size;k++)
    Histo[k]=0;

    //-----------------------------------------
    // 5. Fill histogram
	//-----------------------------------------
	if( max-min != 0 )
    for(unsigned long k=0;k<Data[0].size();k++)
    {
		v = Data[0][k];

        ptr = floor((Histogram_Size-1)*(v-min)/(max-min));

        if( ptr >= 0 && ptr < Histogram_Size )
            Histo[ptr]++;

	}

	return Histo;
}

//---------------------------------------------------------------------------

double Numerical_Library::get_percentile_from_histogram(std::vector<long>* Histo,
				 double Percentile, double Start_Value, double Box_Size)
{
	long Size = Histo[0].size();
	double v=0;

	if( Size > 2 )
	{

	// create cumulated histo
	std::vector<double>CHisto;
	CHisto.assign(Size,v);

	for(long i=0;i<Size;i++)
		CHisto[i] = 0;
	CHisto[0] = Histo[0][0];
	for(long i=1;i<Size;i++)
        CHisto[i] = CHisto[i-1] + Histo[0][i];

    // normalize
    if(CHisto[Size-1]!= 0)
    for(long i=0;i<Size;i++)
        CHisto[i] *= 100.0/CHisto[Size-1];

	double Ratio=-1000;
    int Box = 0;
    if( CHisto[Box]!=0 && CHisto[0] >= Percentile)
	{
		Box = 0;
        Ratio = (Percentile)/(CHisto[Box]);
    }
    else
    {
		while( CHisto[Box] < Percentile )
            Box++;

		// find ratio
//        if( CHisto[Box]-CHisto[Box-1] != 0 && Box-1>=0)
//		Ratio = (Percentile -CHisto[Box-1] )/(CHisto[Box]-CHisto[Box-1]);
//		else
//		Ratio = -1000;

		// find ratio
		if( Box+1 < Size )
		{
		if( CHisto[Box+1]-CHisto[Box-1] != 0 )
		if( Box-1>=0)
		Ratio = (Percentile -CHisto[Box-1] )/(CHisto[Box+1]-CHisto[Box-1]);
		}
		else
		{
		if( CHisto[Box]-CHisto[Box-1] != 0 )
		if( Box-1>=0)
		Ratio = (Percentile - CHisto[Box-1] )/(CHisto[Box]- CHisto[Box-1]);
		}

	}

	return Start_Value + (Box+Ratio)*Box_Size;

	}
	else
	return NOT_POSSIBLE_TO_CALCULATE_VALUE;
}

//---------------------------------------------------------------------------

double Numerical_Library::get_median(std::vector<double> Data)
{
  double median;
  size_t size = Data.size();

  sort(Data.begin(), Data.end());

  if (size  % 2 == 0)
	  median = (Data[size / 2 - 1] + Data[size / 2]) / 2;
  else
	  median = Data[size / 2];

  return median;
}

//---------------------------------------------------------------------------

int Numerical_Library::median_filter(std::vector<double> &Signal,long Window_Length)
{
	std::vector<double> Filtered_Signal;
	long N=Signal.size();
	double v=0;
	Filtered_Signal.assign(N,v);
	double tmp;

	std::vector <double> W;

	for(long t=0;t<Window_Length;t++)
	W.push_back(Signal[t]);

	for(long t=Window_Length/2;t<N-Window_Length/2;t++)
	{

	tmp = get_median(W);
	Filtered_Signal[t] = tmp;

	W.push_back(Signal[t+Window_Length/2]);
	W.erase( W.begin() );// !!!! debug this

	}

	for(long t=Window_Length/2;t<N-Window_Length/2;t++)
	Signal[t] = Filtered_Signal[t];

	return 1;
}

//----------------------------------------------------------------------------------

double Numerical_Library::get_MPC_between_electrograms(
					std::vector<double> *Electrogram_1,
					std::vector<double> *Electrogram_2,
					long Basic_Cycle_Length,
					int Electrogram_Type,long Delay,double Time_Step_ms)
{
	std::vector <double> S1;
	std::vector <double> S2;

	// 1. Get recomposed signals
	S1 = recompose_bipolar_signal( Electrogram_1,Basic_Cycle_Length,Electrogram_Type);
	S2 = recompose_bipolar_signal( Electrogram_2,Basic_Cycle_Length,Electrogram_Type);

	// 2. Get Hilbert phase
	S1 = get_Hilbert_phase_using_convolution_double(&S1,Time_Step_ms);
	S2 = get_Hilbert_phase_using_convolution_double(&S2,Time_Step_ms);

    // 3. Get coherence
	double Coh = get_phase_synchronization(&S1,&S2,1,1,true,Delay);

    S1.clear();
    S2.clear();
    
    return Coh;
}

//-----------------------------------------------------------------------------------

std::vector <double> Numerical_Library::recompose_bipolar_signal(
	std::vector<float> *Signal,int Wavelet_Period,int Trigger_Type)
{
	std::vector <double> Recomposed_Signal;
	double v=0;
	Recomposed_Signal.assign(Signal[0].size(),v);
	for(long t=0;t<(signed)Recomposed_Signal.size();t++)
	Recomposed_Signal[t]=0;
	int T2 = (double)Wavelet_Period/2.;

	//--------------------------------------
	// 1. Generate sinusoidal wavelet
	//--------------------------------------
	std::vector<double> SWavelet;
	SWavelet.assign(Wavelet_Period,v);
	for(long t=0;t<Wavelet_Period;t++)
	SWavelet[t] = sin(2*3.1415*(double)t/(double)Wavelet_Period);

	//--------------------------------------
	// 2. Recompose signal
	//--------------------------------------
	double dv;

	if( Trigger_Type == 0 ) // unipolar, down slope triggered
	for(long t=0;t<(signed)Signal[0].size()-1;t++)
	{
		for(long tp=-Wavelet_Period/2;tp<Wavelet_Period/2;tp++)
        if(t+tp>0 && t+tp < (signed)Signal[0].size() )
		{
		dv = Signal[0][t+1]-Signal[0][t];
		if( dv < 0 )
		Recomposed_Signal[t+tp] += -dv*
			SWavelet[tp+Wavelet_Period/2];
		}
	}

	if( Trigger_Type == 1 ) // bipolar, apmlitude triggered
	for(long t=0;t<(signed)Signal[0].size();t++)
	{
		for(long tp=-Wavelet_Period/2;tp<Wavelet_Period/2;tp++)
		if(t+tp>0 && t+tp < (signed)Signal[0].size() )
		Recomposed_Signal[t+tp] += fabs(Signal[0][t])*
			SWavelet[tp+Wavelet_Period/2];
	}

	if( Trigger_Type == 2 ) // AP, up slope
	for(long t=0;t<(signed)Signal[0].size()-1;t++)
	{
		for(long tp=-T2;tp<T2;tp++)
		if(t+tp>0 && t+tp < (signed)Signal[0].size() )
		{
		dv = Signal[0][t+1]-Signal[0][t];
		if( dv > 0 )
		Recomposed_Signal[t+tp] += dv*SWavelet[tp+T2];
		}
	}

	return Recomposed_Signal;
}

//-----------------------------------------------------------------------------------

std::vector <double> Numerical_Library::recompose_bipolar_signal(
	std::vector<double> *Signal,int Wavelet_Period,int Trigger_Type)
{
	std::vector <double> Recomposed_Signal;
	double v=0;
	Recomposed_Signal.assign(Signal[0].size(),v);
	for(long t=0;t<(signed)Recomposed_Signal.size();t++)
	Recomposed_Signal[t]=0;
	int T2 = (double)Wavelet_Period/2.;

	//--------------------------------------
	// 1. Generate sinusoidal wavelet
	//--------------------------------------
	std::vector<double> SWavelet;
	SWavelet.assign(Wavelet_Period,v);
	for(long t=0;t<Wavelet_Period;t++)
	SWavelet[t] = sin(2*3.1415*(double)t/(double)Wavelet_Period);

	//--------------------------------------
	// 2. Recompose signal
	//--------------------------------------
	double dv;

	if( Trigger_Type == 0 ) // unipolar, down slope triggered
	for(long t=0;t<(signed)Signal[0].size()-1;t++)
	{
		for(long tp=-Wavelet_Period/2;tp<Wavelet_Period/2;tp++)
		if(t+tp>0 && t+tp < (signed)Signal[0].size() )
		{
		dv = Signal[0][t+1]-Signal[0][t];
		if( dv < 0 )
		Recomposed_Signal[t+tp] += -dv*
			SWavelet[tp+Wavelet_Period/2];
		}
	}

	if( Trigger_Type == 1 ) // bipolar, apmlitude triggered
	for(long t=0;t<(signed)Signal[0].size();t++)
	{
		for(long tp=-Wavelet_Period/2;tp<Wavelet_Period/2;tp++)
		if(t+tp>0 && t+tp < (signed)Signal[0].size() )
		Recomposed_Signal[t+tp] += fabs(Signal[0][t])*
			SWavelet[tp+Wavelet_Period/2];
	}

	if( Trigger_Type == 2 ) // AP, up slope
	for(long t=0;t<(signed)Signal[0].size()-1;t++)
	{
		for(long tp=-T2;tp<T2;tp++)
		if(t+tp>0 && t+tp < (signed)Signal[0].size() )
		{
		dv = Signal[0][t+1]-Signal[0][t];
		if( dv > 0 )
		Recomposed_Signal[t+tp] += dv*SWavelet[tp+T2];
		}
	}

	return Recomposed_Signal;
}

//-----------------------------------------------------------------------------------

std::vector <long> Numerical_Library::recompose_bipolar_signal(
	std::vector<long> *Signal,int Wavelet_Period,int Trigger_Type)
{
	std::vector <long> Recomposed_Signal;
	double v=0;
	Recomposed_Signal.assign(Signal[0].size(),v);
	for(long t=0;t<(signed)Recomposed_Signal.size();t++)
	Recomposed_Signal[t]=0;
	int T2 = (double)Wavelet_Period/2.;

	//--------------------------------------
	// 1. Generate sinusoidal wavelet
	//--------------------------------------
	std::vector<double> SWavelet;
	SWavelet.assign(Wavelet_Period,v);
	for(long t=0;t<Wavelet_Period;t++)
	SWavelet[t] = sin(2*3.1415*(double)t/(double)Wavelet_Period);

	//--------------------------------------
	// 2. Recompose signal
	//--------------------------------------
	double dv;

	if( Trigger_Type == 0 ) // unipolar, down slope triggered
	for(long t=0;t<(signed)Signal[0].size()-1;t++)
	{
		for(long tp=-Wavelet_Period/2;tp<Wavelet_Period/2;tp++)
		if(t+tp>0 && t+tp < (signed)Signal[0].size() )
		{
		dv = Signal[0][t+1]-Signal[0][t];
		if( dv < 0 )
		Recomposed_Signal[t+tp] += -dv*
			SWavelet[tp+Wavelet_Period/2];
		}
	}

	if( Trigger_Type == 1 ) // bipolar, apmlitude triggered
	for(long t=0;t<(signed)Signal[0].size();t++)
	{
		for(long tp=-Wavelet_Period/2;tp<Wavelet_Period/2;tp++)
		if(t+tp>0 && t+tp < (signed)Signal[0].size() )
		Recomposed_Signal[t+tp] += std::fabs((double)Signal[0][t])*
			SWavelet[tp+Wavelet_Period/2];
	}

	if( Trigger_Type == 2 ) // AP, up slope
	for(long t=0;t<(signed)Signal[0].size()-1;t++)
	{
		for(long tp=-T2;tp<T2;tp++)
		if(t+tp>0 && t+tp < (signed)Signal[0].size() )
		{
		dv = Signal[0][t+1]-Signal[0][t];
		if( dv > 0 )
		Recomposed_Signal[t+tp] += dv*SWavelet[tp+T2];
		}
	}

	return Recomposed_Signal;
}

//----------------------------------------------------------------------------------

double Numerical_Library::get_micro_fractionation(
	std::vector<double> *Signal,int Algorithm, double Percentile_Threshold,
	int Window_Size)
{
	//-----------------------------------------------------
	// normalized to SD
	//-----------------------------------------------------
	if( Algorithm == 0 )
	{

	double average, SDev;
	stdev_cor_vec_double(Signal, &average, &SDev);

	// calculate "integral"
	double Integral = 0;

	for(long t=1;t< Signal[0].size(); t++)
	Integral += fabs( Signal[0][t+1]-Signal[0][t] );

	if( Signal[0].size() != 0 )
	Integral /= (double)(Signal[0].size());

	if( SDev != 0 )
	Integral /= SDev;

	return Integral;

	}

	//-----------------------------------------------------
	// normalized to 90% percentile
	//-----------------------------------------------------
	if( Algorithm == 1 )
	{

	// calculate "integral"
	double Integral = 0;

	for(long t=1;t< Signal[0].size(); t++)
	Integral += fabs( Signal[0][t+1]-Signal[0][t] );

	if( Signal[0].size() != 0 )
	Integral /= (double)(Signal[0].size());

	double min,max,mean,SD;

	calculate_max_min_mean_vec_double(Signal,&min, &max, &mean);

	stdev_cor_vec_double(Signal,&mean, &SD);

	double t;
	std::vector <long> Histo = get_histogram(Signal,0,true,&t);

	double CI10 = get_percentile_from_histogram(&Histo,10,min, 0.125*SD);
	double CI90 = get_percentile_from_histogram(&Histo,90,min, 0.125*SD);

	if( CI90-CI10 != 0 )
	Integral /= CI90-CI10;

	return Integral;

	}

	//-----------------------------------------------------
	// normalized to max-min
	//-----------------------------------------------------
	if( Algorithm == 2 )
	{

	// calculate "integral"
	double Integral = 0;

	for(long t=1;t< Signal[0].size(); t++)
	Integral += fabs( Signal[0][t+1]-Signal[0][t] );

	if( Signal[0].size() != 0 )
	Integral /= (double)(Signal[0].size());

	Integral *= 100; // to scale it little bit up

	double min,max,mean,SD;
	calculate_max_min_mean_vec_double(Signal,&min, &max, &mean);

	if( max-min != 0 )
	Integral /= max-min;

	return Integral;

	}

	//-----------------------------------------------------
	// calculated within 10-90 percentile box
	//-----------------------------------------------------
	if( Algorithm == 4 )
	{

	// calculate 10 and 90 percentiles
	double min,max,mean,SD;
	calculate_max_min_mean_vec_double(Signal,&min, &max, &mean);
	stdev_cor_vec_double(Signal,&mean, &SD);
	double t;
	std::vector <long> Histo = get_histogram(Signal,0,true,&t);
	double CI_lower = get_percentile_from_histogram(&Histo,100-Percentile_Threshold,min, 0.125*SD);
	double CI_upper = get_percentile_from_histogram(&Histo,Percentile_Threshold,min, 0.125*SD);

	// calculate "integral" within the box limited by percnentiles
	double Integral=0;
	for(long t=1;t< Signal[0].size(); t++)
	if( Signal[0][t] > CI_lower && Signal[0][t] < CI_upper )
	{
		Integral += fabs( Signal[0][t+1]-Signal[0][t] );
	}

	// normalize to the "box" (rectangle area within which signal length was computed)
	Integral /= Signal[0].size()*(CI_upper-CI_lower);
//	Integral /= Signal[0].size()*(max-min);

	// multiply it to be just larger number
	Integral *= 1000;

	return Integral;
	}

	//-----------------------------------------------------
	// rectification + sliding window + 1/SD
	//-----------------------------------------------------
	if( Algorithm == 5 )
	{

	std::vector<double> FSignal;

	// 1. Sliding window with sum
	FSignal = get_abs_sum_in_window(Signal,Window_Size);

	// 2. Calculate SD
	double min,max,mean,SD;
	stdev_cor_vec_double(&FSignal,&mean, &SD);

	// 3. return 1/SD
	if(  SD*mean != 0 )
	return 1.0/(SD*mean);
	else
	return 0;

	} // alg 5

	//-----------------------------------------------------
	// sliding window max-min
	//-----------------------------------------------------
	if( Algorithm == 6 )
	{
	double Min,Max;
//xxx
	std::vector<double> FSignal = get_sliding_maxmin_of_signal(Signal,Window_Size);

	double AvC=0;
	double Counter=0;
	double WindowSize_Mutliplier=10;

	while( (Counter+1)*Window_Size*WindowSize_Mutliplier < FSignal.size() )
	{
		calculate_max_min_vec_double_ranged(&FSignal,
			Counter*Window_Size*WindowSize_Mutliplier,
		(Counter+1)*Window_Size*WindowSize_Mutliplier, &Min, &Max);

		if(Max!=0)
		{
			AvC+=Min/Max;
			Counter++;
		}
	}

	if( Counter!=0 )
	return AvC/Counter;
	else
    return 0;
/*
	calculate_max_min_vec_double_ranged(
	&FSignal,
	0.1*FSignal.size(),
	0.9*FSignal.size(), &Min, &Max);

	if(Max!=0)
	return Min/Max;
	else
	return 0;
*/

	}

	return 0;
}

//----------------------------------------------------------------------------------

void Numerical_Library::calculate_max_min_vec_double_ranged(std::vector<double> *Signal,
			long Begin, long End,double* Min, double* Max)
{
	Min[0] =  std::numeric_limits<double>::max();
	Max[0] = -std::numeric_limits<double>::max();

	// 1. min, max, mean
	for(long i=Begin; i<End; i++)
	if( i >= 0  && i < (signed)Signal[0].size() )
	{
		if( Signal[0][i] > Max[0] )
			Max[0] = Signal[0][i];

		if( Signal[0][i] < Min[0] )
			Min[0] = Signal[0][i];
	}
}

//----------------------------------------------------------------------------------

double Numerical_Library::get_phase_synchronization(
	std::vector<double> *Signal_A,std::vector<double> *Signal_B,int n,int m,
		bool Take_80_Perc_Flag,long Delay)
{
    double Sins=0,Coss=0;
	double Coherence=0;

	if( Signal_A[0].size() > 0 && Signal_B[0].size() > 0 )
	//	&& Signal_A[0].size() == Signal_B[0].size() )
	{

	//--------------------------------------------------------------------
	// 1. Unwrap signals
	//--------------------------------------------------------------------
	std::vector <double> S1,S2;
	S1 = unwrap_phase(Signal_A);
	S2 = unwrap_phase(Signal_B);

	long Length = std::min(S1.size(),S2.size());

	//--------------------------------------------------------------------
	// 2. Compute synchronization
	//--------------------------------------------------------------------

	Take_80_Perc_Flag = false; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	long absDelay = std::abs((int)Delay);

	if( !Take_80_Perc_Flag )
	{
		if( Delay >= 0 )
		for(long t=Delay;t<(signed)Length;t++)
		{
			Sins += sin((double)n*S1[t]-(double)m*S2[t-Delay]);
			Coss += cos((double)n*S1[t]-(double)m*S2[t-Delay]);
		}

		if( Delay < 0 )
		for(long t=0;t<(signed)Length-absDelay;t++)
		{
			Sins += sin((double)n*S1[t]-(double)m*S2[t+absDelay]);
			Coss += cos((double)n*S1[t]-(double)m*S2[t+absDelay]);
		}

		Coherence = sqrt( Sins*Sins + Coss*Coss )/(double)(Length-absDelay);
	}
/*
	if( Take_80_Perc_Flag )
	{
		for(long t=0.1*Length;t<=0.9*Length;t++)
		{
			Sins += sin((double)n*S1[t]-(double)m*S2[t]);
			Coss += cos((double)n*S1[t]-(double)m*S2[t]);
		}
		Coherence = sqrt( Sins*Sins + Coss*Coss )/(double)(0.8*Length);
	}
 */

	return Coherence;

	}
	else
	return -1000000;
}

//------------------------------------------------------------------------------

std::vector <double> Numerical_Library::unwrap_phase(std::vector <double> *Phase_Vec)
{
	std::vector <double> Unwrapped_Phase;
	Unwrapped_Phase = Phase_Vec[0];
	long N = Phase_Vec[0].size();
	double tmp=0;

	std::vector <double> dp;
	std::vector <double> dps;
	std::vector <double> dp_corr;
	std::vector <double> cumsum;

	dp.assign(N,tmp);
	dps = dp;
	dp_corr = dp;
	cumsum = dp;

	double cutoff = M_PI;               /* default value in matlab */

   // incremental phase variation
   // MATLAB: dp = diff(p, 1, 1);
	for (long j = 0; j < N-1; j++)
	  dp[j] = Phase_Vec[0][j+1] - Phase_Vec[0][j];

   // equivalent phase variation in [-pi, pi]
   // MATLAB: dps = mod(dp+dp,2*pi) - pi;
	for (long j = 0; j < N-1; j++)
	  dps[j] = (dp[j]+M_PI) - floor((dp[j]+M_PI) / (2*M_PI))*(2*M_PI) - M_PI;

   // preserve variation sign for +pi vs. -pi
   // MATLAB: dps(dps==pi & dp>0,:) = pi;
	for (long j = 0; j < N-1; j++)
	  if ((dps[j] == -M_PI) && (dp[j] > 0))
		dps[j] = M_PI;

   // incremental phase correction
   // MATLAB: dp_corr = dps - dp;
	for (long j = 0; j < N-1; j++)
	  dp_corr[j] = dps[j] - dp[j];

   // Ignore correction when incremental variation is smaller than cutoff
   // MATLAB: dp_corr(abs(dp)<cutoff,:) = 0;
	for (long j = 0; j < N-1; j++)
	  if (fabs(dp[j]) < cutoff)
		dp_corr[j] = 0;

   // Find cumulative sum of deltas
   // MATLAB: cumsum = cumsum(dp_corr, 1);
	cumsum[0] = dp_corr[0];
	for (long j = 1; j < N-1; j++)
	  cumsum[j] = cumsum[j-1] + dp_corr[j];

   // Integrate corrections and add to P to produce smoothed phase values
   // MATLAB: p(2:m,:) = p(2:m,:) + cumsum(dp_corr,1);
	for (long j = 1; j < N; j++)
	  Unwrapped_Phase[j] += cumsum[j-1];

	return Unwrapped_Phase;
}

//---------------------------------------------------------------------------

void Numerical_Library::convol(std::vector<double> *source, std::vector<double> *target,
			std::vector<double> *filt, int npt, int lfilt)
{

	long i, l;
	double yt;

	for (l=1; l<=npt-lfilt+1; l++)
	{
		yt = 0.0;
		for (i=2; i<lfilt; i++)
			yt = yt+source[0][l+i-1]*filt[0][lfilt+1-i];
		target[0][l] = yt;
	}

	// shifting lfilt/1+1/2 points
	for (i=1; i<=npt-lfilt; i++)
	{
		target[0][i] = 0.5*(target[0][i]+target[0][i+1]);
	}

	for (i=npt-lfilt; i>=1; i--)
		target[0][i+lfilt/2]=target[0][i];

	// writing zeros
	for (i=1; i<=lfilt/2; i++)
	{
		target[0][i] = 0.0;
		target[0][npt+1-i] = 0.0;
	}

}


//----------------------------------------------------------------------------------

std::vector <double> Numerical_Library::get_Hilbert_phase_using_convolution_double(
	std::vector<double> *Signal,double Time_Step_ms)
{

	double v=0;
	long lfilt = 256; // length of the hilbert envelope in ms

	if( Time_Step_ms != 0 )
	lfilt /= Time_Step_ms;
//	else
 //	ShowMessage("Time_Step_ms 0 in PNumericalLib, hilbert phase calculations.");

	if( lfilt%2 == 1 ) // to keep it even number
	lfilt++;

	std::vector<double> hilb;
	std::vector<double> x;
	std::vector<double> xh;
	std::vector<double> ampl;
	std::vector<double> phase;

	double pi, xt, xht;
	pi = M_PI;

	long N; // length of signal
	N = Signal[0].size();

	if( N > lfilt )
	{

	x.assign(N,v); //  = new double[N];
	xh.assign(N,v); // = new double[N];
	ampl.assign(N,v); // = new double[N];
	phase.assign(N,v); // = new double[N];

	for(long i=0;i<N;i++)
	{
		x[i] = Signal[0][i];
		xh[i] = 0;
		ampl[i] = 0;
		phase[i] = 0;
	}

	// generation of the envelope
	hilb.assign((int)lfilt,v); // = new double[lfilt];

	for (long i=0; i<lfilt; i++)
		hilb[i]=1.0/(((double)i-(double)lfilt/2.0)-0.5)/pi;


	// Hilbert transform
	convol(&x, &xh, &hilb, N-1, lfilt);

	// Ampl and phase
	for (long i=1; i<=N-1; i++)
//	for (long i=lfilt/2+1; i<=N-1-lfilt/2; i++)
	{
		xt = x[i];
		xht = xh[i];

		ampl[i] = sqrt(xt*xt+xht*xht);
		if( xht!=0 && xt!=0 )
		phase[i] = atan2(xt,-xht); // modified so phase inv is at deflection
		else
		phase[i] = 0;
	}

	// set range to -pi to +pi
	rescale_signal(&phase,-M_PI,+M_PI);

	}

	return phase;
}

//----------------------------------------------------------------------------------

void Numerical_Library::rescale_signal(std::vector <double>* Signal,
	double Min,double Max)
{
	// find min max
	double Smin,Smax,Smean;
	calculate_max_min_mean_vec_double(Signal,&Smin, &Smax, &Smean);

	if( Smax!=Smin && Smin!=Smean && Smax!=Smean )
	{

	// transform signal
	double a = (Max-Min)/(Smax-Smin);
	double b = Max - a*Smax;

	for(long t=0;t<(signed)Signal[0].size();t++)
	Signal[0][t] = a*Signal[0][t]+b;

	}
}

//----------------------------------------------------------------------------------


void Numerical_Library::calculate_max_min_mean_SD_vec_double(std::vector<double> *Signal,
				double* Min, double* Max, double* Mean, double* SD)
{
	// 1. find max,min,mean from last DATA_LENGTH_FOR_PEAK_EXTRACTION
	// steps in roving and ref. cath
	Min[0] = std::numeric_limits< double >::max();
	Max[0] = std::numeric_limits< double >::min();
	Mean[0] = 0.0;

	for(long i=0; i<(signed)Signal[0].size(); i++)
	{

	Mean[0] += Signal[0][i];

	if( Signal[0][i] > Max[0] )
		Max[0] = Signal[0][i];

	if( Signal[0][i] < Min[0] )
		Min[0] = Signal[0][i];
	}

	if( Signal[0].size() != 0 )
	Mean[0] /= (double)(Signal[0].size());
	else
	Mean[0]=0;

	// 2. calculate stdev

	SD[0] = 0.0;
	for(long i = 0; i <(signed) Signal[0].size(); i++ )
		SD[0] += pow( (double)(Signal[0][i] - Mean[0]), 2.0 );

	if (SD[0] > 0 && Signal[0].size() != 0 )
	SD[0] = sqrt( SD[0] / ( Signal[0].size()-1 ) );
	else SD[0] = 0;
}

//----------------------------------------------------------------------------------

void Numerical_Library::calculate_max_min_mean_slope_vec_ranged(vector <double> *Signal,
				long Begin,long End,double* Max_slope, double* Min_slope)
{

	Min_slope[0] = std::numeric_limits< double >::max();
	Max_slope[0] = std::numeric_limits< double >::min();

	for(long i=2; i<(signed)Signal[0].size()-2; i++)
	{

	if( Signal[0][i+1] - Signal[0][i] > Max_slope[0] )
		Max_slope[0] = Signal[0][i+1] - Signal[0][i];

	if( Signal[0][i+1] - Signal[0][i] < Min_slope[0] )
		Min_slope[0] = Signal[0][i+1] - Signal[0][i];

	}
}

//----------------------------------------------------------------------------------

void Numerical_Library::calculate_max_min_mean_vec_double(
		std::vector<double> *Signal,double* Min, double* Max, double* Mean)
{
	long i;

	// 1. find max,min,mean from last DATA_LENGTH_FOR_PEAK_EXTRACTION
	// steps in roving and ref. cath
	Min[0] = std::numeric_limits< double >::max();
	Max[0] = std::numeric_limits< double >::min();
	Mean[0] = 0.0;

	for(i=0; i<(signed)Signal[0].size(); i++)
	{

	Mean[0] += Signal[0][i];

	if( Signal[0][i] > Max[0] )
		Max[0] = Signal[0][i];

	if( Signal[0][i] < Min[0] )
		Min[0] = Signal[0][i];
	}

	if( Signal[0].size() != 0 )
	Mean[0] /= (double)(Signal[0].size());
	else
	Mean[0]=0;
}

//----------------------------------------------------------------------------------

void Numerical_Library::get_rgb_colors_from_ranged_variable(double Min,double Max, double Value,
											   int* R, int* G,int* B)
{
	double ratio = 2 * (Value-Min) / (Max - Min);
	B[0] = std::max(0., 255*(1 - ratio));
	R[0] = std::max(0., 255*(ratio - 1));
	G[0] = 255 - B[0] - R[0];
}

//----------------------------------------------------------------------------------

std::vector <double> Numerical_Library::Butt_band_40_250_3th_order(std::vector <double> *Signal, bool Inverse)
{
	double gain  = 5.090882386e+01;
	std::vector <double> Result;
	std::vector <double> Rev_Result;

	for(int i=0;i<7;i++)
	{ x7[i]=0; y7[i]=0; }

	//--------------------------------------------
	if( ! Inverse )
	//--------------------------------------------
	{

	for(long k = 0; k < (signed)Signal[0].size(); k ++ )
	{
		x7[0] = x7[1];
		x7[1] = x7[2];
		x7[2] = x7[3];
		x7[3] = x7[4];
		x7[4] = x7[5];
		x7[5] = x7[6];
		x7[6] = Signal[0][k];// / gain;

		y7[0] = y7[1];
		y7[1] = y7[2];
		y7[2] = y7[3];
		y7[3] = y7[4];
		y7[4] = y7[5];
		y7[5] = y7[6];
		y7[6] =
	   ( -1/gain * x7[0])
	 + (  0/gain * x7[1])
	 + (  3/gain * x7[2])
	 + (  0/gain * x7[3])
	 + ( -3/gain * x7[4])
	 + (  0/gain * x7[5])
	 + (  1/gain * x7[6])

	 + ( -0.2662615629 * y7[0])
	 + (  1.8474876957 * y7[1])
	 + ( -5.4914172544 * y7[2])
	 + (  9.0007408198 * y7[3])
	 + ( -8.5801614461 * y7[4])
	 + (  4.4891238319 * y7[5]);

		Result.push_back(y7[6]);
	  }

	  return Result;

      } // no inverse

    //--------------------------------------------
	if( Inverse )
    //--------------------------------------------
    {

	for(long k = 0; k <(signed) Signal[0].size(); k ++ )
	{
		x7[0] = x7[1];
		x7[1] = x7[2];
		x7[2] = x7[3];
		x7[3] = x7[4];
		x7[4] = x7[5];
		x7[5] = x7[6];
		x7[6] = Signal[0][Signal[0].size()-1-k];// / gain;

		y7[0] = y7[1];
		y7[1] = y7[2];
		y7[2] = y7[3];
		y7[3] = y7[4];
		y7[4] = y7[5];
		y7[5] = y7[6];
		y7[6] =
	   ( -1/gain * x7[0])
	 + (  0/gain * x7[1])
	 + (  3/gain * x7[2])
	 + (  0/gain * x7[3])
	 + ( -3/gain * x7[4])
	 + (  0/gain * x7[5])
	 + (  1/gain * x7[6])

	 + ( -0.2662615629 * y7[0])
	 + (  1.8474876957 * y7[1])
	 + ( -5.4914172544 * y7[2])
	 + (  9.0007408198 * y7[3])
	 + ( -8.5801614461 * y7[4])
	 + (  4.4891238319 * y7[5]);

		Result.push_back(y7[6]);
	}

	// inverse result
	for(long k=0;k<(signed)Result.size();k++)
	Rev_Result.push_back(Result[Result.size()-1-k]);

	return Rev_Result;

	  } // inverse
	return Result;
}

//---------------------------------------------------------------------------
std::vector <double> Numerical_Library::rectify_signal(std::vector <double> *Signal)
{
	std::vector <double> Result;

	for(long i=0;i<(signed)Signal[0].size();i++)
	Result.push_back(fabs(Signal[0][i]));

	return Result;
}

//---------------------------------------------------------------------------
std::vector <double> Numerical_Library::Butt_low_20_3th_order(std::vector <double> *Signal, bool Inverse)
{
    double gain;
	std::vector <double> Result;
	std::vector <double> Rev_Result;
    x4[0]=0;x4[1]=0;x4[2]=0;x4[3]=0;

    //--------------------------------------------
    if( ! Inverse )
    //--------------------------------------------
    {

    gain = 4.553605266e+03;
	for(long k = 0; k < (signed)Signal[0].size(); k ++ )
    {
        x4[0] = x4[1];
        x4[1] = x4[2];
		x4[2] = x4[3];
        x4[3] = Signal[0][k] / gain;
        y4[0] = y4[1];
        y4[1] = y4[2];
        y4[2] = y4[3];
        y4[3] =   (x4[0] + x4[3]) + 3 * (x4[1] + x4[2])
                     + (  0.7776385602 * y4[0]) + ( -2.5282312191 * y4[1])
                     + (  2.7488358092 * y4[2]);
        Result.push_back(y4[3]);
	  }

/*
	for(long k = 0; k < Signal[0].size(); k ++ )
	{
		x4[0] = x4[1];
		x4[1] = x4[2];
		x4[2] = x4[3];
		x4[3] = Signal[0][k] / gain;

		y4[0] = y4[1];
		y4[1] = y4[2];
		y4[2] = y4[3];
		y4[3] =
	   (  1 * x4[0])
	 + (  3 * x4[1])
	 + (  3 * x4[2])
	 + (  1 * x4[3])

	 + ( 0.8837484610 * y4[0])
	 + ( -2.7604268125 * y4[1])
	 + ( 2.8764564523 * y4[2]);

		Result.push_back(y4[3]);
    }
*/


    return Result;

    }

    //--------------------------------------------
    if( Inverse )
    //--------------------------------------------
    {

    gain = 4.553605266e+03;
	for(long k = 0; k < (signed)Signal[0].size(); k ++ )
    {
        x4[0] = x4[1];
        x4[1] = x4[2];
        x4[2] = x4[3];
        x4[3] = Signal[0][Signal[0].size()-1-k] / gain;
        y4[0] = y4[1];
        y4[1] = y4[2];
        y4[2] = y4[3];
        y4[3] =   (x4[0] + x4[3]) + 3 * (x4[1] + x4[2])
                     + (  0.7776385602 * y4[0]) + ( -2.5282312191 * y4[1])
                     + (  2.7488358092 * y4[2]);
        Result.push_back(y4[3]);
      }

    /*
	for(long k = 0; k < Signal[0].size(); k ++ )
	{
		x4[0] = x4[1];
		x4[1] = x4[2];
		x4[2] = x4[3];
		x4[3] = Signal[0][Signal[0].size()-1-k] / gain;

		y4[0] = y4[1];
		y4[1] = y4[2];
		y4[2] = y4[3];
		y4[3] =
	   (  1 * x4[0])
	 + (  3 * x4[1])
	 + (  3 * x4[2])
	 + (  1 * x4[3])

	 + ( 0.8837484610 * y4[0])
	 + ( -2.7604268125 * y4[1])
	 + ( 2.8764564523 * y4[2]);

		Result.push_back(y4[3]);
	}
    */

    // inverse result
	for(long k=0;k<(signed)Result.size();k++)
    Rev_Result.push_back(Result[Result.size()-1-k]);

    return Rev_Result;
	}
	return Result;
}

//---------------------------------------------------------------------------

long Numerical_Library::calculate_max_min_mean_vec_long(std::vector<long> *Signal,long Begin, long End,
		double* Min, double* Max, double* Mean, int* Peak_Direction)
{
	long i;

	// 1. find max,min,mean from last DATA_LENGTH_FOR_PEAK_EXTRACTION
	// steps in roving and ref. cath
	Min[0] = 100000000;
	Max[0] = -100000000;
	Mean[0] = 0.0;

	if( Begin == End ) End++;
	if( Begin < 0 )
		Begin = 0;

	if( Begin < End )
	{

	for(i=Begin; i<End; i++)
	{

	Mean[0] += Signal[0][i];

	if( Signal[0][i] > Max[0] )
		Max[0] = Signal[0][i];

	if( Signal[0][i] < Min[0] )
		Min[0] = Signal[0][i];

	}

	Mean[0] /= (End-Begin);

	// set peak direction
	if( fabs( Max[0] - Mean[0] ) > fabs( Min[0] - Mean[0] ) )
		Peak_Direction[0] = 1;
	else
		Peak_Direction[0] = -1;

	return 1;

	}
	else
	{

	Min[0] = 0.0;
	Max[0] = 1.0;
	Mean[0] = 0.0;

	return -1;

	}
}

//------------------------------------------------------------------------------

long Numerical_Library::get_position_of_peak(std::vector<long> *Signal,long Start, long Stop)
{
	double Max=-1000000,Max_Pos,Min=1000000,Min_Pos,v;

	for(long i=Start+1; i<Stop-1; i++)
	{

	v = Signal[0][i];

	if( v > Max )
	{
		Max = v;
		Max_Pos = i;
	}

	if( v < Min )
	{
		Min = v;
		Min_Pos = i;
	}

	}

	if( fabs(Max) > fabs(Min) )
	return Max_Pos;
	else
	return Min_Pos;
}

//------------------------------------------------------------------------------

std::vector <double> Numerical_Library::get_correlation_vector(std::vector <double> *v1,
		std::vector <double> *v2, long Step  )
{
	std::vector <double> Corrs;
	double v=0;
	Corrs.assign(v1[0].size(),v);
	double Corr=0;
	long Nshift = 0.5*v2[0].size();

	for(long t=0;t<v1[0].size()-v2[0].size();t++)
	{
		if(t%Step==0)
		Corr = correlation_coef_vec_different_size(v1,v2,t);

		Corrs[t+Nshift] = Corr;
	}

	return Corrs;
}

//------------------------------------------------------------------------------

std::vector <double> Numerical_Library::get_xcorrelation_vector(std::vector <double> *v1,
		std::vector <double> *v2, long Step  )
{
	std::vector <double> Corrs;
	double v=0,Min1,Min2,Max1,Max2;
	Corrs.assign(v1[0].size(),v);
	double Corr=0;
	long Nshift = 0.5*v2[0].size(), Min_Ptr1,Min_Ptr2,Max_Ptr1,Max_Ptr2;

	find_min_max(v2,&Min2, &Max2, &Min_Ptr2, &Max_Ptr2);

	for(long t=0;t<v1[0].size()-v2[0].size();t++)
	{
		if(t%Step==0)
		Corr = correlation_coef_vec_different_size(v1,v2,t);

		// calculate amplitudes
		find_min_max(v1,&Min1, &Max1, &Min_Ptr1, &Max_Ptr1);

		if( fabs((Max2-Min2) - (Max1-Min1)) != 0 )
		Corr /= fabs((Max2-Min2) - (Max1-Min1));

		Corrs[t+Nshift] = Corr;
	}

	return Corrs;
}

//------------------------------------------------------------------------------

double Numerical_Library::get_amplitude_of_egms_difference(std::vector<double> *v1,std::vector<double> *v2)
{
	double Min, Max;
	long Min_Ptr, Max_Ptr;

	std::vector<double> diff;

	for (long i = 1; i < v1[0].size()-1; i++)
		diff.push_back(v1[0][i] - v2[0][i]);

	find_min_max(&diff,&Min,&Max, &Min_Ptr, &Max_Ptr);

	return Max-Min;
}

//------------------------------------------------------------------------------

double Numerical_Library::correlation_coef_vec_different_size(
		std::vector<double> *vec1,std::vector<double> *vec2,
		long Start_Position_in_vec1)
{

	if( Start_Position_in_vec1 >= 0 && Start_Position_in_vec1 < vec1[0].size() &&
		vec2[0].size() > 0 &&
		Start_Position_in_vec1 + vec2[0].size() < vec1[0].size() ) // V wave is witihn v1 vector
	{
	double corr,sum_X = 0, sum_Y = 0, sum_XY = 0;
	double squareSum_X = 0, squareSum_Y = 0;
	double n = vec2[0].size();

	for (long i = 0; i < n; i++)
	{
		// sum of elements of array X.
		sum_X = sum_X + vec1[0][Start_Position_in_vec1+i];

		// sum of elements of array Y.
		sum_Y = sum_Y + vec2[0][i];

		// sum of X[i] * Y[i].
		sum_XY = sum_XY + vec1[0][Start_Position_in_vec1+i] * vec2[0][i];

		// sum of square of array elements.
		squareSum_X = squareSum_X + vec1[0][Start_Position_in_vec1+i] * vec1[0][Start_Position_in_vec1+i];
		squareSum_Y = squareSum_Y + vec2[0][i] * vec2[0][i];
	}

	// use formula for calculating correlation coefficient.
	double den = sqrt((n * squareSum_X - sum_X * sum_X)
					  * (n * squareSum_Y - sum_Y * sum_Y));

	if( den != 0 )
	corr = (double )(n * sum_XY - sum_X * sum_Y) / den ;
	else
	corr = 0;

	return corr;

	}
	else
	return 0;

/*
	if( Start_Position_in_vec1 >= 0 && Start_Position_in_vec1 < vec1[0].size() &&
		vec2[0].size() > 0 &&
		Start_Position_in_vec1 + vec2[0].size() < vec1[0].size() ) // V wave is witihn v1 vector
	{

	long n = vec2[0].size();

	double av1,av2;
	double SD1,SD2;

	av1 = 0.0;
	SD1 = 0.0;
	for(long k=Start_Position_in_vec1;k<Start_Position_in_vec1+n;k++)
		av1 += vec1[0][k];
	av1 /= (double)n;

	for(long k=Start_Position_in_vec1;k<Start_Position_in_vec1+n;k++)
		SD1 += pow(vec1[0][k]-av1,2.0);

	SD1 = sqrt(SD1/(double)n);

	av2 = 0;
	SD2 = 0;
	for(long k=0;k<n;k++)
		av2 += vec2[0][k];
	av2 /= (double)n;

	for(long k=0;k<n;k++)
		SD2 += pow(vec2[0][k]-av2,2.0);

	SD2 = sqrt(SD2/(double)n);

	// 2. calculate covariance
	double IFG = 0;
	for(long k=0;k<n;k++)
		IFG += (vec1[0][Start_Position_in_vec1+k]-av1)*(vec2[0][k]-av2);

	// 3. calculate correlation
	if( SD1 != 0 && SD2 != 0 )
		IFG = IFG/((double)(n)*SD1*SD2);
	else
		IFG = 0.0;

	return IFG;
	}
	else
	return NOT_POSSIBLE_TO_CALCULATE_VALUE;
*/
}

//------------------------------------------------------------------------------------------------------
std::vector <double> Numerical_Library::get_covariance_vector(std::vector <double> *v1,
		std::vector <double> *v2, long Step  )
{
	std::vector <double> Corrs;
	double v=0;
	Corrs.assign(v1[0].size(),v);
	double Corr=0;
	long Nshift = 0.5*v2[0].size();

	for(long t=0;t<v1[0].size()-v2[0].size();t++)
	{
		if(t%Step==0)
		Corr = covariance_coef_vec_different_size(v1,v2,t);

		Corrs[t+Nshift] = Corr;
	}

	return Corrs;
}

//------------------------------------------------------------------------------

double Numerical_Library::covariance_coef_vec_different_size(
		std::vector<double> *vec1,std::vector<double> *vec2,
		long Start_Position_in_vec1)
{
	if( Start_Position_in_vec1 >= 0 && Start_Position_in_vec1 < vec1[0].size() &&
		vec2[0].size() > 0 &&
		Start_Position_in_vec1 + vec2[0].size() < vec1[0].size() ) // V wave is witihn v1 vector
	{

	double sum_X = 0, sum_Y = 0, sum_XY = 0;
	double squareSum_X = 0, squareSum_Y = 0;
	double n = vec2[0].size();

	for (long i = 0; i < n; i++)
	{
		// sum of elements of array X.
		sum_X = sum_X + vec1[0][Start_Position_in_vec1+i];

		// sum of elements of array Y.
		sum_Y = sum_Y + vec2[0][i];

		// sum of X[i] * Y[i].
		sum_XY = sum_XY + vec1[0][Start_Position_in_vec1+i] * vec2[0][i];

		// sum of square of array elements.
		squareSum_X = squareSum_X + vec1[0][Start_Position_in_vec1+i] * vec1[0][Start_Position_in_vec1+i];
		squareSum_Y = squareSum_Y + vec2[0][i] * vec2[0][i];
	}

	// use formula for calculating correlation coefficient.
//	double corr = (double )(n * sum_XY - sum_X * sum_Y)
//				  / sqrt((n * squareSum_X - sum_X * sum_X)
//					  * (n * squareSum_Y - sum_Y * sum_Y));
	double covariance = (double )(n * sum_XY - sum_X * sum_Y);

	return covariance;

	}
	else
	return NOT_POSSIBLE_TO_CALCULATE_VALUE;
}

//------------------------------------------------------------------------------------------------------

std::vector <double> Numerical_Library::get_psimilarity_vector(std::vector <double> *v1,
		std::vector <double> *v2, long Step  )
{
	std::vector <double> Corrs;
	double v=0;
	Corrs.assign(v1[0].size(),v);
	double Corr=0;
	long Nshift = 0.5*v2[0].size();

	for(long t=0;t<v1[0].size()-v2[0].size();t++)
	{
		if(t%Step==0)
		Corr = psimilarity_coef_vec_different_size(v1,v2,t);

		Corrs[t+Nshift] = Corr;
	}

	return Corrs;
}

//------------------------------------------------------------------------------------------------------

double Numerical_Library::psimilarity_coef_vec_different_size(
		std::vector<double> *vec1,std::vector<double> *vec2,
		long Start_Position_in_vec1)
{
	if( Start_Position_in_vec1 >= 0 && Start_Position_in_vec1 < vec1[0].size() &&
		vec2[0].size() > 0 &&
		Start_Position_in_vec1 + vec2[0].size() < vec1[0].size() ) // V wave is witihn v1 vector
	{

	double sum_X = 0;
	long n = vec2[0].size();

	double av1,av2;

	av1 = 0.0;
	for(long k=Start_Position_in_vec1;k<Start_Position_in_vec1+n;k++)
		av1 += vec1[0][k];
	av1 /= (double)n;

	av2 = 0;
	for(long k=0;k<n;k++)
		av2 += vec2[0][k];
	av2 /= (double)n;


	for (long i = 0; i < n; i++)
	{
		// sum of differences between vectors
		sum_X = sum_X + fabs(
			(vec1[0][Start_Position_in_vec1+i]-av1) -(vec2[0][i]-av2) );
	}

	double similarity;

	if( sum_X != 0 )
	return 1./sum_X;
	else
	return 100000; // sumX = 0 means perfect fit

	}
	else
	return 0;
}

//------------------------------------------------------------------------------------------------------

std::vector <long> Numerical_Library::calculate_peak_positions_vec(
	std::vector<double> *values,double threshold,double margin_ratio)
{
	std::vector<long> tmp_Rs;
	long i, tmp_Rs_Number,tt=0;
	long counter, R_begin, R_end, R_prev;
	double R_Threshold_Up,max;
	double current_max = -1000.0; // ,prev_max = -1000.0;
	long max_ptr;

	tmp_Rs.assign(values[0].size(),tt);

	std::vector <long> Peaks;

	for( i=0; i < values[0].size(); i ++ )
		tmp_Rs[i] = 0;

	// find initial max value
	max = 0.0;
	for( i=0; i < values[0].size(); i++ )
	if( values[0][i] > max )
		max = values[0][i];

	// Method.3 threshold calculated on window

	// set thresholds
	R_Threshold_Up =  threshold*max;

	counter = 0;
	tmp_Rs_Number = 0;
	R_prev = 0; // location of previous R polong

	while( counter < values[0].size() )	// go through
	{
			if( (values[0][ counter ] > R_Threshold_Up &&
				counter - R_prev > 2 && R_prev != 0 )
				||
				( values[0][ counter ] > R_Threshold_Up &&
				R_prev == 0 ) )
			{
				// ok, we have a beginning of R!!!
				R_begin = counter;
//                prev_max = current_max;
				current_max = -1000;

				// search for end...
				while( values[0][ counter ] > R_Threshold_Up &&
							counter < values[0].size()-1 )
				{
					if( values[0][ counter ] > current_max )
					{
						current_max = values[0][ counter ];
						max_ptr = counter;
					}

					counter++;
				}

				// ok, we have end!
				R_end = counter;

				// write this R to tmp Rs table if we have beginning and the end
				if( R_end > R_begin && counter < values[0].size()-1 )
				{
					tmp_Rs[ tmp_Rs_Number ] = max_ptr; // R_begin + 0.5*( R_end - R_begin );
					tmp_Rs_Number++;
					R_prev = counter;

				}

			}  // peak founded

		// ok, go on.
		counter++;

		}  // main while when R is up


	// now, create true positions std::vector
	for( i=0; i < tmp_Rs_Number; i++ )
	if( tmp_Rs[i] > margin_ratio*values[0].size() && tmp_Rs[i] < (1.0-margin_ratio)*values[0].size() )
		Peaks.push_back(tmp_Rs[i]);

	return Peaks;
}

//------------------------------------------------------------------------------------------------------

std::vector <long> Numerical_Library::calculate_peak_positions(double* values,long n,double threshold)
{
	std::vector<long> tmp_Rs;
	long i, tmp_Rs_Number,tt=0;
	long counter, R_begin, R_end, R_prev;
	double R_Threshold_Up,max;
	double current_max = -1000.0; // ,prev_max = -1000.0;
	long max_ptr;

	tmp_Rs.assign(n,tt);

	std::vector <long> Peaks;

	for( i=0; i < n; i ++ )
		tmp_Rs[i] = 0;

	// find initial max value
	max = 0.0;
	for( i=0; i < n; i++ )
    if( values[i] > max )
		max = values[i];

    // Method.3 threshold calculated on window

    // set thresholds
	R_Threshold_Up =  threshold*max;

    counter = 0;
    tmp_Rs_Number = 0;
    R_prev = 0; // location of previous R polong

	while( counter < n )	// go through
    {
            if( (values[ counter ] > R_Threshold_Up &&
				counter - R_prev > 2 && R_prev != 0 )
                ||
                ( values[ counter ] > R_Threshold_Up &&
                R_prev == 0 ) )
			{
				// ok, we have a beginning of R!!!
				R_begin = counter;
//                prev_max = current_max;
                current_max = -1000;

				// search for end...
				while( values[ counter ] > R_Threshold_Up &&
                            counter < n-1 )
                {
                    if( values[ counter ] > current_max )
					{
						current_max = values[ counter ];
                        max_ptr = counter;
                    }

                    counter++;
                }

				// ok, we have end!
				R_end = counter;

				// write this R to tmp Rs table if we have beginning and the end
                if( R_end > R_begin && counter < n-1 )
				{
					tmp_Rs[ tmp_Rs_Number ] = max_ptr; // R_begin + 0.5*( R_end - R_begin );
                    tmp_Rs_Number++;
                    R_prev = counter;

                }

			}  // peak founded

		// ok, go on.
		counter++;

		}  // main while when R is up


	// now, create true positions std::vector
	for( i=0; i < tmp_Rs_Number; i++ )
		Peaks.push_back(tmp_Rs[i]);

	return Peaks;
}

//-----------------------------------------------------------------------------------

double Numerical_Library::get_min(double A,double B,double C)
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
double Numerical_Library::get_middle(double A,double B,double C)
{
    if( (A <= C && A >= B)  || (A <= B && A >= C))
    return A;

    if( (B <= C && B >= A)  || (B <= A && B >= C))
    return B;

    if( (C <= A && C >= B)  || (C <= B && C >= A))
    return C;

    return 0;
}

//-----------------------------------------------------------------------
double Numerical_Library::get_max(double A,double B,double C)
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

int Numerical_Library::get_triangle_based_CV(
									 double N1_x,double N1_y,double N1_z,
									 double N2_x,double N2_y,double N2_z,
									 double N3_x,double N3_y,double N3_z,
									 double LAT_N1,double LAT_N2,double LAT_N3,
									 double* CV_magnitude,
									 double* CV_x, double* CV_y, double* CV_z, bool* CV_valid)
{
	double O_x,O_y,O_z;
	double A_x,A_y,A_z;
	double B_x,B_y,B_z;
	double LAT_O,LAT_A,LAT_B;
    double min;
    double obx,oby,obz;
    double oax,oay,oaz;

	CV_valid[0] = true;

	if( LAT_N1 == LAT_N2 || LAT_N1 == LAT_N3 || LAT_N2 == LAT_N3 )
	{
		CV_valid[0] = false;
		return -1;
	}

	min = get_min(      LAT_N1,LAT_N2,LAT_N3);
//	middle = get_middle(LAT_N1,LAT_N2,LAT_N3);
//	max = get_max(      LAT_N1,LAT_N2,LAT_N3);

	if( min == LAT_N1 )
		{
			O_x = N1_x; O_y = N1_y; O_z = N1_z; LAT_O = LAT_N1;
			A_x = N2_x; A_y = N2_y; A_z = N2_z; LAT_A = LAT_N2;
			B_x = N3_x; B_y = N3_y; B_z = N3_z; LAT_B = LAT_N3;
		}
	if( min == LAT_N2 )
		{
			O_x = N2_x; O_y = N2_y; O_z = N2_z; LAT_O = LAT_N2;
			A_x = N1_x; A_y = N1_y; A_z = N1_z; LAT_A = LAT_N1;
			B_x = N3_x; B_y = N3_y; B_z = N3_z; LAT_B = LAT_N3;
		}
	if( min == LAT_N3 )
		{
			O_x = N3_x; O_y = N3_y; O_z = N3_z; LAT_O = LAT_N3;
			A_x = N2_x; A_y = N2_y; A_z = N2_z; LAT_A = LAT_N2;
			B_x = N1_x; B_y = N1_y; B_z = N1_z; LAT_B = LAT_N1;
		}

    double OA,OB,AB,cos_teta,tOB,tOA,sin_teta;
    double tg_alfa,cos_alfa,tg_beta,cos_beta;
//    double alfa,beta,tg_alfa,cos_alfa,tg_beta,cos_beta;
    double CV=0;

        OA = sqrt( std::pow(O_x-A_x,2)+
                   std::pow(O_y-A_y,2)+
                   std::pow(O_z-A_z,2) );

        AB = sqrt( std::pow(B_x-A_x,2)+
                   std::pow(B_y-A_y,2)+
                   std::pow(B_z-A_z,2) );

        OB = sqrt( std::pow(O_x-B_x,2)+
                   std::pow(O_y-B_y,2)+
                   std::pow(O_z-B_z,2) );

		tOA = LAT_A - LAT_O;
		tOB = LAT_B - LAT_O;

        if( OA*OB != 0 )
        cos_teta = (OA*OA+OB*OB-AB*AB)/(2.0*OA*OB);
        else
		{
            cos_teta = 0.0;
			CV_valid[0] = false;
			return -1;
		}

		sin_teta = sqrt( 1 - cos_teta*cos_teta );

		if( tOA != 0 )
		{
            if( sin_teta != 0 && tOA*OB != 0 )
			tg_alfa = ( (tOB*OA)/(tOA*OB) - cos_teta )/(sin_teta);
			else
			{
			tg_alfa = 0.0;
			CV_valid[0] = false;
			}

			cos_alfa = 1.0/( sqrt(1+tg_alfa*tg_alfa ));
			CV = OA/tOA * cos_alfa;

			// compute x,y components
			oax = A_x - O_x;
			oay = A_y - O_y;
			oaz = A_z - O_z;
		}

		if( tOB != 0 )
		{
			if( sin_teta != 0 && tOB*OA != 0 )
			tg_beta = ( (tOA*OB)/(tOB*OA) - cos_teta )/(sin_teta);
			else
			{
			tg_beta = 0.0;
			CV_valid[0] = false;
			}

			cos_beta = 1.0/( sqrt(1+tg_beta*tg_beta ));
			CV = OB/tOB * cos_beta;

			obx = B_x - O_x;
			oby = B_y - O_y;
			obz = B_z - O_z;
		}

	// COMPUTATION OF DIRECTIONALITY OF CV std::vector
    double a,b;
	double c1 = oax*obx + oay*oby + oaz*obz;

	b = ( CV*OA*cos_alfa - CV*OB*cos_beta*OA*OA/c1 ) /
		( c1 - OB*OB*OA*OA/c1 );

	a = CV*OB*cos_beta/c1 - b*OB*OB/c1;

	CV_x[0] = a*oax+b*obx;
	CV_y[0] = a*oay+b*oby;
	CV_z[0] = a*oaz+b*obz;

	CV_magnitude[0] = CV;

	return 1;
}

//-----------------------------------------------------------------------------------

void Numerical_Library::rotate_xyz_in_cartesian_system(
			double* x,double* y,double* z, double angle, int which_axis)
{
	double tx,ty,tz,s,c;

	tx = x[0];
	ty = y[0];
	tz = z[0];

	s = sin(angle);
	c = cos(angle);

	if( which_axis == 1 ) // x
	{
		x[0] =  tx;
		y[0] =  ty*c + tz*s;
		z[0] = -ty*s + tz*c;
	}

	if( which_axis == 2 ) // Y
	{
		x[0] =  tx*c + tz*s;
		y[0] =  ty;
        z[0] = -tx*s + tz*c;
    }

    if( which_axis == 3 ) // Z
	{
        x[0] =  tx*c + ty*s;
        y[0] = -tx*s + ty*c;
		z[0] = tz;
    }


}

//-----------------------------------------------------------------------------------

long Numerical_Library::get_number_of_words_causality_alphabet_method(std::vector <long> *Sender,
			std::vector <long> *Receiver,int Word_Length)
{
	// sender and receiver: SHOULD CONTAIN TIMINGS OF CONSECUTIVE ACTIVATIONS/EVENTS

	long Number_of_Words_Send = 0;

	std::vector <double> Sequence_A;
	std::vector <double> Sequence_B;

	// go through events in main signal (sender)
	for(long T=0;T<(signed)Sender[0].size()-Word_Length;T++)
	{

		// get SENDER sequence form T time point
		Sequence_A.clear();
		for(int t1=0;t1<Word_Length+2;t1++)
		if( T+t1 < (signed)Sender[0].size() )
		Sequence_A.push_back(Sender[0][T+t1]);

		// find corresonding position in receiver
		long Receiver_ptr=-1;
		long min = 100000;
		for(long T2=0;T2<(signed)Sender[0].size()-Word_Length;T2++)
		if( Receiver[0][T2] - Sender[0][T] > 0 &&
			Receiver[0][T2] - Sender[0][T] < min )
		{
			min = Receiver[0][T2] - Sender[0][T];
			Receiver_ptr = T2;
		}

		// get RECEIVER sequence form T time point
		Sequence_B.clear();
		for(int t1=0;t1<Word_Length+2;t1++)
		if( Receiver_ptr+t1 < (signed)Receiver[0].size() )
		Sequence_B.push_back(Receiver[0][Receiver_ptr+t1]);

		if( Receiver[0][Receiver_ptr] > Sender[0][T] ) // event at receiver is later than in sender
		// check if sequences are similar
        if( (signed)Sequence_A.size() == Word_Length+2 )
        if( (signed)Sequence_B.size() == Word_Length+2 )
		{
			int Score=0;

			for(int s=0;s<(signed)Sequence_A.size()-2;s++)
            if( (Sequence_A[s+1]-Sequence_A[s] >= Sequence_A[s+2]-Sequence_A[s+1] &&
                 Sequence_B[s+1]-Sequence_B[s] >= Sequence_B[s+2]-Sequence_B[s+1])
				||
                (Sequence_A[s+1]-Sequence_A[s] <= Sequence_A[s+2]-Sequence_A[s+1] &&
                 Sequence_B[s+1]-Sequence_B[s] <= Sequence_B[s+2]-Sequence_B[s+1] ))
			Score++;

			if(Score == (signed)Sequence_A.size()-2 )
			Number_of_Words_Send++;

		}
	}

	return Number_of_Words_Send;
}

//-----------------------------------------------------------------------------------

bool Numerical_Library::is_value_present_in_vector_long(std::vector<long> *Vec,long Value)
{
	for(unsigned long i=0;i<Vec[0].size();i++)
	if( Vec[0][i]== Value )
	return true;

	return false;
}

//-----------------------------------------------------------------------------------

bool Numerical_Library::is_value_present_int(int* Set,int Size, int Val)
{
	for(long i=0;i<Size;i++)
	if( Set[i]== Val )
	return true;

	return false;
}


//---------------------------------------------------------------------------
void Numerical_Library::rotate_vector
	(double* x,double* y,double* z, double angle, int which_axis)
{
    double tx,ty,tz,s,c;

	tx = x[0];
    ty = y[0];
    tz = z[0];
    s = sin(angle);
    c = cos(angle);

    if( which_axis == 1 ) // x
    {
        x[0] =  tx;
        y[0] =  ty*c + tz*s;
        z[0] = -ty*s + tz*c;
    }

    if( which_axis == 2 ) // Y
    {
        x[0] =  tx*c + tz*s;
        y[0] =  ty;
		z[0] = -tx*s + tz*c;
	}

	if( which_axis == 3 ) // Z
	{
		x[0] =  tx*c + ty*s;
		y[0] = -tx*s + ty*c;
		z[0] = tz;
	}

}
//--------------------------------------------------------------------------

long Numerical_Library::get_min_difference(long T,std::vector <long> *Vec)
{
	long abs_min_diff = 1000000000;
	long min_diff;

	for(long i=0;i<Vec[0].size();i++)
	{
		if( std::fabs((double)Vec[0][i]-T) > abs_min_diff )
		{
			abs_min_diff = std::fabs((double)Vec[0][i]-T);
			min_diff = Vec[0][i]-T;
		}
	}

	return min_diff;
}

//---------------------------------------------------------------------------
/*
template <class T1>
std::vector <T1> Numerical_Library::resample(std::vector <T1> *X, long Start, long Stop, long Target_Length)
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
*/
//---------------------------------------------------------------------------

double Numerical_Library::Dot_Product(Point_3D* A,Point_3D* B)
{
	return A[0].x*B[0].x + A[0].y*B[0].y + A[0].z*B[0].z;
}

//---------------------------------------------------------------------------

double Numerical_Library::get_distance_between_3D_point_and_triangle(
													  double Px,double Py,double Pz,
													  double T1x,double T1y,double T1z,
													  double T2x,double T2y,double T2z,
													  double T3x,double T3y,double T3z )
{
// https://www.geometrictools.com/GTEngine/Include/Mathematics/GteDistPointTriangleExact.h
// https://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf
	Point_3D diff;
	diff.x = Px - T1x;
	diff.y = Py - T1y;
	diff.z = Pz - T1z; // v[0] is T1
					   // v[1] is T2
					   // v[2] is T3
	Point_3D edge0;
	edge0.x = T2x - T1x;
	edge0.y = T2y - T1y;
	edge0.z = T2z - T1z; // std::vector<N, Rational> edge0 = triangle.v[1] - triangle.v[0];

	Point_3D edge1;
	edge1.x = T3x - T1x;
	edge1.y = T3y - T1y;
	edge1.z = T3z - T1z; // std::vector<N, Rational> edge1 = triangle.v[2] - triangle.v[0];

	double a00 = Dot_Product(&edge0, &edge0);
	double a01 = Dot_Product(&edge0, &edge1);
	double a11 = Dot_Product(&edge1, &edge1);
	double b0 = -Dot_Product(&diff, &edge0);
	double b1 = -Dot_Product(&diff, &edge1);
	double const zero = (double)0;
	double const one = (double)1;
	double det = a00 * a11 - a01 * a01;
	double t0 = a01 * b1 - a11 * b0;
	double t1 = a01 * b0 - a00 * b1;

	if( a00!=0 && a01!=0 && a11!=0 && b0!=0 && b1!=0 && det!=0 )
	{

	if (t0 + t1 <= det)
	{
		if (t0 < zero)
		{
			if (t1 < zero)  // region 4
			{
				if (b0 < zero)
				{
					t1 = zero;
					if (-b0 >= a00)  // V1
					{
						t0 = one;
					}
					else  // E01
					{
						t0 = -b0 / a00;
					}
                }
				else
				{
					t0 = zero;
                    if (b1 >= zero)  // V0
                    {
                        t1 = zero;
					}
					else if (-b1 >= a11)  // V2
                    {
                        t1 = one;
                    }
                    else  // E20
                    {
                        t1 = -b1 / a11;
					}
                }
            }
            else  // region 3
			{
				t0 = zero;
				if (b1 >= zero)  // V0
                {
                    t1 = zero;
				}
                else if (-b1 >= a11)  // V2
				{
					t1 = one;
                }
                else  // E20
				{
                    t1 = -b1 / a11;
				}
            }
		}
		else if (t1 < zero)  // region 5
		{
            t1 = zero;
            if (b0 >= zero)  // V0
            {
				t0 = zero;
            }
            else if (-b0 >= a00)  // V1
            {
                t0 = one;
            }
            else  // E01
            {
				t0 = -b0 / a00;
			}
		}
		else  // region 0, interior
		{
			double invDet = one / det;
			t0 *= invDet;
			t1 *= invDet;
		}
	}
	else
	{
		double tmp0, tmp1, numer, denom;

		if (t0 < zero)  // region 2
		{
			tmp0 = a01 + b0;
			tmp1 = a11 + b1;
			if (tmp1 > tmp0)
			{
				numer = tmp1 - tmp0;
				denom = a00 - ((double)2)*a01 + a11;
				if (numer >= denom)  // V1
				{
					t0 = one;
					t1 = zero;
				}
				else  // E12
				{
					t0 = numer / denom;
					t1 = one - t0;
				}
			}
			else
			{
				t0 = zero;
				if (tmp1 <= zero)  // V2
				{
					t1 = one;
				}
				else if (b1 >= zero)  // V0
				{
					t1 = zero;
				}
				else  // E20
				{
					t1 = -b1 / a11;
				}
			}
		}
		else if (t1 < zero)  // region 6
		{
			tmp0 = a01 + b1;
			tmp1 = a00 + b0;
			if (tmp1 > tmp0)
			{
				numer = tmp1 - tmp0;
				denom = a00 - ((double)2)*a01 + a11;
				if (numer >= denom)  // V2
				{
					t1 = one;
					t0 = zero;
				}
				else  // E12
				{
					t1 = numer / denom;
					t0 = one - t1;
				}
			}
			else
			{
				t1 = zero;
				if (tmp1 <= zero)  // V1
				{
					t0 = one;
				}
				else if (b0 >= zero)  // V0
				{
					t0 = zero;
				}
				else  // E01
				{
					t0 = -b0 / a00;
				}
			}
		}
		else  // region 1
		{
			numer = a11 + b1 - a01 - b0;
			if (numer <= zero)  // V2
			{
				t0 = zero;
				t1 = one;
			}
			else
			{
				denom = a00 - ((double)2)*a01 + a11;
				if (numer >= denom)  // V1
				{
					t0 = one;
					t1 = zero;
				}
				else  // 12
				{
					t0 = numer / denom;
					t1 = one - t0;
				}
			}
		}
	}

	} // if all variables != 0

	double Bary1 = one - t0 - t1;
	double Bary2 = t0;
	double Bary3 = t1;

	Point_3D closest;
	closest.x = T1x + t0 * edge0.x + t1 * edge1.x;
	closest.y = T1y + t0 * edge0.y + t1 * edge1.y;
	closest.z = T1z + t0 * edge0.z + t1 * edge1.z;

	diff.x = Px - closest.x;
	diff.y = Py - closest.y;
	diff.z = Pz - closest.z;

	double Distance = Dot_Product(&diff, &diff);

	return sqrt(Distance);
}

//---------------------------------------------------------------------------

double Numerical_Library::get_non_zero_mean(double v1, double v2)
{
	if( v1!=0 && v2!=0 )
	return 0.5*(v1+v2);

	if( v1==0 && v2!=0)
	return v2;

	if( v1!=0 && v2==0)
	return v1;

	if( v1==0 && v2==0)
	return 0;

	return 0;
}

//---------------------------------------------------------------------------

void Numerical_Library::correlation_coef_vec(std::vector <double> *vec1,
	std::vector <double> *vec2, double* corr)
{
/*
	double av, cov, sd1, sd2;

	covariance_vec(vec1,vec2,&cov);
	stdev_cor_vec(vec1,&av,&sd1);
	stdev_cor_vec(vec2,&av,&sd2);

	if(sd1!=0 && sd2!=0 )
	corr[0] = cov/(sd1*sd2);
	else
	corr[0] = -1000.0;
*/
	if( vec1[0].size() == vec2[0].size() )
	{

	// taken from: https://www.geeksforgeeks.org/program-find-correlation-coefficient/

	double sum_X = 0, sum_Y = 0, sum_XY = 0;
	double squareSum_X = 0, squareSum_Y = 0;
	double n = vec1[0].size();

	for(long i = 0; i < n; i++)
	{
		// sum of elements of array X.
		sum_X = sum_X + vec1[0][i];

		// sum of elements of array Y.
		sum_Y = sum_Y + vec2[0][i];

		// sum of X[i] * Y[i].
		sum_XY = sum_XY + vec1[0][i] * vec2[0][i];

		// sum of square of array elements.
		squareSum_X = squareSum_X + vec1[0][i] * vec1[0][i];
		squareSum_Y = squareSum_Y + vec2[0][i] * vec2[0][i];
	}

	double den = sqrt((n * squareSum_X - sum_X * sum_X)
					  * (n * squareSum_Y - sum_Y * sum_Y));
	// use formula for calculating correlation coefficient.
	if( den != 0 )
	corr[0] = (double )(n * sum_XY - sum_X * sum_Y) / den;
	else
    corr[0] = NOT_POSSIBLE_TO_CALCULATE_VALUE;

	}
	else
	corr[0] = NOT_POSSIBLE_TO_CALCULATE_VALUE;
}

//---------------------------------------------------------------------------

void Numerical_Library::stdev_cor_vec(std::vector <double> *vec,
	double* average, double* SDev)
{
	double SD,av = 0.0;
	long i;

	// 1. claculate average
	av = 0.0;
	for( i = 0; i <(signed) vec[0].size(); i++ )
		av += vec[0][i];

	if(vec[0].size()!=0)
		av  /= vec[0].size();
	else
		av = 0.0;

	average[0] = av;

	// 2. calculate stdev
	SD = 0.0;
	for( i = 0; i <(signed) vec[0].size(); i++ )
		SD += pow( (double)(vec[0][i] - av), 2.0 );

	if (SD > 0 && vec[0].size() != 0 )
	SDev[0] = sqrt( SD / ( vec[0].size()-1 ) );
	else SDev[0] = 0;
}

//---------------------------------------------------------------------------

void Numerical_Library::covariance_vec(std::vector <double> *vec1,
				std::vector <double> *vec2, double* cov)
{
	double av1 = 0.0, av2 = 0.0, cov1 = 0.0;

	long n = vec1[0].size();

	if( n > 2 )
	{

	// 1. claculate averages
	for(long i = 0; i < n; i++ )
	{
		av1 += vec1[0][i];
		av2 += vec2[0][i];
	}

	av1  /= n;
	av2  /= n;

	// 2. covariance
	for(long i = 0; i < n; i++ )
		cov1 += (vec1[0][i]-av1)*(vec2[0][i]-av2);

	cov1 = cov1/n;

	cov[0] = cov1;
	}
	else
	cov[0] = -1000;
}

//-----------------------------------------------------------------------------------

double Numerical_Library::get_Shannon_Entropy(std::vector<double> *signal,bool Use_SD, double Bin_Size)
{
	double SH=NOT_POSSIBLE_TO_CALCULATE_VALUE,p,v,min,max,average,SD;
	long Histogram_Size,tmp,ptr;
	std::vector <long> Histo;
	long length = signal[0].size();

	//-----------------------------------------
	// 1. calculate av and SD
	//-----------------------------------------
	stdev_cor_vec(signal, &average, &SD );

	//-----------------------------------------
	// 2. find min max average of voltage
	//-----------------------------------------
	min=1000000000; max=-100000000;  average = 0;
	for(long k=0;k<length;k++)
	{
		if(signal[0][k]>max) max=signal[0][k];
		if(signal[0][k]<min) min=signal[0][k];
	}

	if( min==max) { min=0; max=1; }

    //-----------------------------------------
    // 3. Calculate bin size
    //-----------------------------------------
    if( Use_SD == true )
	{
        Bin_Size = 0.125*SD;
	}

    //-----------------------------------------
	// 4. Calculate number of bins and allocate histogram
	//-----------------------------------------
	if( Bin_Size != 0 )
	Histogram_Size = (max-min)/(double)Bin_Size;
	else
	{
		Bin_Size = 1;
		Histogram_Size = (max-min)/(double)Bin_Size;
	}

	long Total=0;

	if( Histogram_Size > 0 )
	{

	Histo.assign(Histogram_Size,tmp);

	for(long k=0;k<Histogram_Size;k++)
	Histo[k]=0;

	//-----------------------------------------
	// 5. Fill histogram
	//-----------------------------------------
	for(long k=0;k<length;k++)
	{
		v = signal[0][k];

		ptr = floor((Histogram_Size-1)*(v-min)/(max-min));

		if( ptr >= 0 && ptr < Histogram_Size )
			Histo[ptr]++;

	}

	//-----------------------------------------
	// 6. Calculate Shannon Entropy
	//-----------------------------------------
	SH = get_Shannon_Entropy_from_histogram(&Histo);

	}

	return SH;
}

//-----------------------------------------------------------------------------------

double Numerical_Library::get_Shannon_Entropy_from_histogram(std::vector<long> *Histogram)
{
	double Total=0,SH=0,p;

	for(long ll=0;ll<Histogram[0].size();ll++)
		Total += Histogram[0][ll];

	if(Total!=0)
	for(long ll=0;ll<Histogram[0].size();ll++)
	{
		p = (double)Histogram[0][ll]/Total;
		if(p>0)
		SH += -p*log(p);
	}

	return SH;
}

//-----------------------------------------------------------------------------------

std::vector <double> Numerical_Library::make_Continious_Wavelet_Transformation
	(std::vector <double> *Data,long Start, long Stop, double Scaling_Factor )
{
	double tau,tmp;

	std::vector <std::complex<double> > v;

	std::vector <double> Wavelet_Filtered_Vector;
	Wavelet_Filtered_Vector.assign(Stop-Start,tmp);

	for(long i=0;i<Wavelet_Filtered_Vector.size();i++)
	{
		tau = 0.001*i; // time shift
		v.push_back( CWT(Data,Start,Stop,0.001,Scaling_Factor,tau) );
	}

	for(long k=0;k<Wavelet_Filtered_Vector.size();k++)
		Wavelet_Filtered_Vector[k] = pow(v[k].real(),2.0)+pow(v[k].imag(),2.0);

	return Wavelet_Filtered_Vector;
}

//---------------------------------------------------------------------------

std::complex<double> Numerical_Library::CWT(std::vector<double> *Data,long Start,
				long Stop,double timestep,double a,double tau)
{
	double time;
	double L=0,R=0;
	std::complex<double> v(0.0,0.0);
	double Morlet_Threshold = 0.01;

	if( timestep > 0 )
	{
		L = (-a*sqrt(-2.0*log(Morlet_Threshold))+tau)/timestep;
		R = ( a*sqrt(-2.0*log(Morlet_Threshold))+tau)/timestep;
	}

	if(L<0 || R > Stop-Start )
		return 0.0;

//	if( a==0)
//	ShowMessage("a==0 in CWT - PnumLib");

	for(long i=L;i<R;i++)
	{
		time = i*timestep;
		v += Data[0][Start+i]*
			morlet_wavelet((double)((time-tau)/a));
	}

	return v;
}

//---------------------------------------------------------------------------

std::complex<double> Numerical_Library::morlet_wavelet(double t)
{
	std::complex<double> v;
	double expv = exp(-0.5*t*t);
v.real(cos(5.5*t)*expv);
v.imag(sin(5.5*t)*expv);
    //v._Val[0] = cos(5.5*t)*expv;
    //v._Val[1] = sin(5.5*t)*expv;

	return v;
}

//---------------------------------------------------------------------------

template <class T1>
std::vector <long> Numerical_Library::get_peak_positions(std::vector<T1> *Data,
	double PDT,long Peak_Detection_Window,long Blanking_Window)
{
	std::vector<long> tmp_Rs;
	long counter, R_begin, R_end, R_prev;
	double R_Threshold_Up, R_Threshold_Down,max;
	double current_max = -1000.0,prev_max = -1000.0;
	long max_ptr;

	// find initial max value
	max = -10000000.0;
	for(long i=0; i < Peak_Detection_Window; i++ )
	if( Data[0][i] > max )
		max = Data[0][i];

	// Method.3 threshold calculated on window

	// set thresholds
	R_Threshold_Up =  PDT*max;

	counter = 0;
	R_prev = 0; // location of previous R polong

	while( counter < Data[0].size() )	// go through
	{
			if( Data[0][counter] > R_Threshold_Up &&
				counter - R_prev > 2 && R_prev != 0
				||
				Data[0][counter] > R_Threshold_Up &&
				R_prev == 0 )
			{
				// ok, we have a beginning of R!!!
				R_begin = counter;
				prev_max = current_max;
				current_max = -1000;

				// search for end...
				while( Data[0][counter] > R_Threshold_Up && counter < Data[0].size() )
				{
					if( Data[0][counter] > current_max )
					{
						current_max = Data[0][counter];
						max_ptr = counter;
					}
					counter++;
				}

				// ok, we have end!
				R_end = counter;

				// write this R to tmp Rs table if we have beginning and the end
				if( tmp_Rs.size() >= 2 )
				{
					if( tmp_Rs[tmp_Rs.size()-1] - tmp_Rs[tmp_Rs.size()-2]  >= Blanking_Window )
					tmp_Rs.push_back(max_ptr);
				}
				else
				if( R_end > R_begin )
				{
					tmp_Rs.push_back(max_ptr);
				}

				R_prev = max_ptr;

			// move counter forward by Blanking window
			counter += Blanking_Window;

			// update PDT
			max = 0.0;
			for(long i=counter-0.5*Peak_Detection_Window;
					 i<counter+0.5*Peak_Detection_Window; i++ )
			if( i>0 && i<Data[0].size() )
			if( Data[0][i] > max )
				max = Data[0][i];

			R_Threshold_Up =  PDT*max;

			}  // peak founded

		// ok, go on.
		counter++;

	}  // main while when R is up

	return tmp_Rs;
}

template std::vector <long> Numerical_Library::get_peak_positions<long>(
	std::vector<long> *Data,double PDT,long Peak_Detection_Window,long Blanking_Window);
template std::vector <long> Numerical_Library::get_peak_positions<double>(
	std::vector<double> *Data,double PDT,long Peak_Detection_Window,long Blanking_Window);

//---------------------------------------------------------------------------
std::vector <double> Numerical_Library::normalize_signal(
	std::vector<double> *Signal,int Window_Length)
{
	double min,max;
	long Data_Length = Signal[0].size();
	double a,b;
	std::vector <double> Result;
	Result = Signal[0];

	//-------------------------------
	// go through all windows (except last one, it has different length)
	//-------------------------------
	if( Window_Length != 0 )
	for(long i=0;i<(double)(Data_Length)/(double)Window_Length;i++)
	{
		if( (i+1)*Window_Length < Data_Length )
		{
			min = std::numeric_limits< double >::max();
			max = std::numeric_limits< double >::min();

			for(long j=i*Window_Length;j<(i+1)*Window_Length;j++)
			if( j>0 && j<Data_Length-1 )
			{
				if( Signal[0][j] > max )
				max = Signal[0][j];

				if( Signal[0][j] < min )
				min = Signal[0][j];
			}
		}

		if( min != std::numeric_limits< double >::max() )
		if( max != std::numeric_limits< double >::min() )
		if( max != 0 )
		if(max!=min)
		{
			a = 1.0/(max-min);
//			a = 1.0/(max);
			b = -min*a;
		}
		else
		{
			a=0;
			b=0;
		}

		for(long j=i*Window_Length;j<(i+1)*Window_Length;j++)
		if( j>=0 && j<= Data_Length-1 )
			Result[j] =a*(double)Signal[0][j]+b;
	}

	//-------------------------------
	// normalize last window (its not covered in the loop)
	//-------------------------------
	if( Window_Length < Data_Length )
	{

	long begin = Data_Length - Window_Length -1;
	long end = Data_Length-1;

	min = std::numeric_limits< double >::max();
	max = std::numeric_limits< double >::min();

	for(long j=begin;j<end;j++)
	{
		if( Signal[0][j] > max )
		max = Signal[0][j];

		if( Signal[0][j] < min )
		min = Signal[0][j];
	}

	if( min != std::numeric_limits< double >::max() )
	if( max != std::numeric_limits< double >::min() )
	if( max != 0 )
	if(max!=min)
	{
			a = 1.0/(max-min);
//			a = 1.0/(max);
			b = -min*a;
	}
	else
	{
		a=0;
		b=0;
	}

	for(long j=begin;j<end;j++)
	Result[j] =a*(double)Signal[0][j]+b;

	}

	return Result;
}

//---------------------------------------------------------------------------

std::vector <double> Numerical_Library::get_sliding_SD_of_signal(
	std::vector<double> *Signal,int Window_Length)
{
	long Data_Length = Signal[0].size();
	std::vector <double> Result;
	Result = Signal[0];
	double SD;

	//-------------------------------
	// pad beginning and end of signal with zeros
	//-------------------------------
	for(long i=0;i<Window_Length/2;i++)
	{
		Result[i]=0;
		Result[Data_Length-1-i]=0;
	}

	//-------------------------------
	// slide with SD
	//-------------------------------
	for(long i=Window_Length/2;i<Data_Length-Window_Length/2;i++)
	{
		calculate_SD_vec_double_ranged(Signal,i-Window_Length/2,i+Window_Length/2,&SD);
        Result[i]=SD;
	}

	return Result;
}

//---------------------------------------------------------------------------

std::vector <double> Numerical_Library::get_sliding_maxmin_of_signal(
	std::vector<double> *Signal,int Window_Length)
{
	std::vector <double> Result;
	Result = Signal[0];
	double Min, Max;

	//-------------------------------
	// slide window
	//-------------------------------
	for(long i=0;i<Signal[0].size();i++)
	{
		calculate_max_min_vec_double_ranged(Signal,i-Window_Length/2.,i+Window_Length/2.,&Min, &Max);
		Result[i]=Max-Min;
	}

	return Result;
}

//---------------------------------------------------------------------------

std::vector <double> Numerical_Library::get_abs_sum_in_window(
	std::vector<double> *Signal,long Window_Length)
{
	long Data_Length = Signal[0].size();
	std::vector <double> Result;
	Result = Signal[0];
	double Sum;

	//-------------------------------
	// pad beginning and end of signal with zeros
	//-------------------------------
	for(long i=0;i<Window_Length/2;i++)
	{
		Result[i]=0;
		Result[Data_Length-1-i]=0;
	}

	//-------------------------------
	// calculate sum in sliding window
	//-------------------------------
	for(long i=Window_Length/2+1;i<Data_Length-Window_Length/2-1;i++)
	{
		Sum = 0;
		for(long t=-Window_Length/2;t<=Window_Length/2;t++)
		Sum += fabs(Signal[0][i+t]);
		Result[i] = Sum;
	}

	return Result;
}

//---------------------------------------------------------------------------
void Numerical_Library::calculate_SD_vec_double_ranged(
				std::vector<double> *Signal,long Begin,long End,double* SD)
{
	double av = 0.0;

	// 1. claculate average
	av = 0.0;
	for(long i=Begin; i<End; i++ )
		av += Signal[0][i];

	if(End-Begin !=0)
		av  /= (double)(End-Begin);
	else
		av = 0.0;

	// 2. calculate stdev

	SD[0] = 0.0;
	for(long i=Begin; i<End; i++ )
		SD[0] += pow( (double)(Signal[0][i] - av), 2.0 );

	if ( End-Begin != 0 )
	SD[0] = sqrt( SD[0] / (double)(End-Begin) );
	else
	SD[0] = 0;

}

//---------------------------------------------------------------------------

std::vector <double> Numerical_Library::set_av_to_0_SD_to_1(std::vector<double> *Signal)
{
	//-------------------------------
	// set mean=0, SD=1
	//-------------------------------
	std::vector <double> Result;
	Result = Signal[0];

	double av,SD;
	stdev_cor_vec(Signal, &av, &SD);

	if( SD!=0 )
	for(long j=0;j<Result.size();j++)
	Result[j] = (Result[j]-av)/SD;

	return Result;
}


//-----------------------------------------------------------------------------------

std::vector <double> Numerical_Library::calculate_NAVX_CAFE(long Start,long Stop,
	std::vector <double>* Signal,double Refraction_Time,double PP_Sensitivity,
	double Duration, double Downstroke_Threshold, double* CAFE)
{
	double max;
	int Found,max_ptr;
	double Sum = 0;
	double counter = 0;

	std::vector <double> vNAVX_Positions_In_Signal;
	vNAVX_Positions_In_Signal.clear();

	if(Start>=0 && Stop < Signal[0].size())
	{

    for(long i=Start+1;i<Stop-Refraction_Time;i++)
    {

	Found = 0;

    // 1. if not in refractory time
	if( vNAVX_Positions_In_Signal.size() == 0 ||
        i - vNAVX_Positions_In_Signal[vNAVX_Positions_In_Signal.size()-1] > Refraction_Time )

    // 2. if value crossess pp threshold
    if( Signal[0][i] >  PP_Sensitivity ||
        Signal[0][i] < -PP_Sensitivity )  // value or maybe check peak like in ICL?

    // 3. stroke down in Duration window
    if( i+Duration < Signal[0].size() )
	{
		Found = 0;
        max = 0;
        max_ptr = -1;
        for(long k=1;k<Duration;k++)
        if( -(Signal[0][i+k]-Signal[0][i+k-1]) > Downstroke_Threshold )
        {
            max = -(Signal[0][i+k]-Signal[0][i+k-1]);
            max_ptr = i+k;
            Found = 1;
        }
	}

	if( Found == 1 )
    vNAVX_Positions_In_Signal.push_back(max_ptr);

    } // locating activations

    // calculate CAFE as mean interval
    for(long j=1;j<(signed)vNAVX_Positions_In_Signal.size();j++)
    if( vNAVX_Positions_In_Signal[j-1] > Start &&
		vNAVX_Positions_In_Signal[j-1] < Stop )
    if( vNAVX_Positions_In_Signal[j] > Start &&
        vNAVX_Positions_In_Signal[j] < Stop )
//    if( vNAVX_Positions_In_Signal[j]-vNAVX_Positions_In_Signal[j-1] < 500 )
    {
        Sum += vNAVX_Positions_In_Signal[j]-vNAVX_Positions_In_Signal[j-1];
		counter++;
    }

	if(counter > 0 )
	Sum /= counter;
	else
	Sum = -1;

	} // if Start/Stop within range

	CAFE[0] = Sum;
	return vNAVX_Positions_In_Signal;

}

//------------------------------------------------------------------------------
void Numerical_Library::fit_line_vertical_offsets_vec(std::vector <double> *y,
									 double* a, double* b, double* F, double* R)
{
	 if( y[0].size() > 1 )
	 {

	 double mx,my,t1,t2;
	 long i;

	// calc means
	 mx = 0.0;
	 my = 0.0;
	 for(i=0;i<y[0].size();i++)
	 {
		mx += i;
		my += y[0][i];
	 }

	 mx /= y[0].size();
	 my /= y[0].size();

	 // calc. a
	 t1 = 0.0;
	 t2 = 0.0;
	 for(i=0;i<y[0].size();i++)
	 {
		 t1 += (i-mx)*(y[0][i]-my);
		 t2 += std::pow(i-mx,2);
	 }

	 if(t2!=0)
		 a[0] = t1/t2;
	 else
		 a[0] = 0.0;

	 b[0] = my - a[0]*mx;

	 // find R^2

	 //    correlation_coef(x, y, n, R);
	double Sum=0;
	 for(i=0;i<y[0].size();i++)
		Sum += std::pow( fabs(a[0]*i - y[0][i] + b[0])/sqrt( a[0]*a[0] + 1 ) , 2);

	Sum = sqrt( Sum );

	Sum /= y[0].size()+1;

	R[0] = Sum;

	 }
	 else
	 {
		 a[0] = 0.0;
		 b[0] = 0.0;
		 F[0] = 0.0;
		 R[0] = 0.0;
	 }
}

//------------------------------------------------------------------------------

void Numerical_Library::fit_line_perpendicular_offsets(double* x, double* y, long start, long stop,
							 double* a, double* b, double* F, double* R)
{
	 long n = stop - start + 1;

	 if( n > 1 )
	 {

     double mx,my,t1,t2;
     long i;

     // calc means
     mx = 0.0;
	 my = 0.0;
     for(i=start;i<=stop;i++)
     {
        mx += x[i];
        my += y[i];
     }

     mx /= double(n);
     my /= double(n);

     // calc sum of squares
     double sx2=0,sy2=0;
     for(i=start;i<=stop;i++)
     {
        sx2 += x[i]*x[i];
        sy2 += y[i]*y[i];
     }

     //calc cross prod sum
     double sxy=0;
     for(i=start;i<=stop;i++)
	 {
        sxy += x[i]*y[i];
     }

     // clac B
     double B;
     B = 0.5*( (sy2-(double)n*my*my) - (sx2-(double)n*mx*mx) )/( (double)n*mx*my - sxy );
     a[0] = -B + sqrt(B*B+1);
     b[0] = my - a[0]*mx;


     }
     else
     {
         a[0] = 0.0;
         b[0] = 0.0;
         F[0] = 0.0;

     }

     // find R^2

     //    correlation_coef(x, y, n, R);
    double Sum=0;
    for(long i=start;i<=stop;i++)
        Sum += std::pow( fabs(a[0]*x[i] - y[i] + b[0])/sqrt( a[0]*a[0] + 1 ) , 2);

    if(Sum>0)
    Sum = sqrt( Sum );

	if( stop-start+1 != 0 )
	Sum /= stop-start+1;

	R[0] = Sum;

}

//------------------------------------------------------------------------------

void Numerical_Library::fit_line_diff_phi_offsets(double* x, double* y, long start, long stop,
							 double* a, double* b, double* F, double* R)
{
	 long n = stop - start + 1;

	 if( n > 1 )
	 {

	 double mx,my;
	 long i,k;

     // calc means
     mx = 0.0;
     my = 0.0;
     for(i=start;i<=stop;i++)
     {
        mx += x[i];
		my += y[i];
     }

     mx /= double(n);
	 my /= double(n);

     //
     double R,R_min=1000000,phi0,phi0_min;
     int steps = 720;
     double phi_step = 2.*M_PI/(double)steps;
     double t1,t2,t3,t4;

     // go through all angles
	 for(i=0;i<=steps;i++)
     {
        phi0 = i*phi_step;

        R=0;
        for(k=start;k<stop;k++)
		{
            t1 = fabs(phi0 - atan2(y[k]-my,x[k]-mx));
            t2 = fabs(phi0 + M_PI - atan2(y[k]-my,x[k]-mx));
			t3 = fabs(phi0 - M_PI - atan2(y[k]-my,x[k]-mx));
            t4 = std::min( std::min(t1,t2), std::min(t2,t3) );
            R+=t4;
        }

        if( R <= R_min )
        {
			R_min = R;
            phi0_min = phi0;
        }
	 }

	 a[0] = tan( phi0_min );
	 b[0] = my - a[0]*mx;

     }
     else
     {
         a[0] = 0.0;
         b[0] = 0.0;
         F[0] = 0.0;

     }

     // find R^2

	 //    correlation_coef(x, y, n, R);
	double Sum=0;
	for(long i=start;i<=stop;i++)
		Sum += std::pow( fabs(a[0]*x[i] - y[i] + b[0])/sqrt( a[0]*a[0] + 1 ) , 2);

	if(Sum>0)
	Sum = sqrt( Sum );

	if( stop-start+1 != 0 )
	Sum /= stop-start+1;

	R[0] = Sum;

}

//------------------------------------------------------------------------------

void Numerical_Library::find_crossing_point_3D_parametric_lines(
			Parametric_3D_line Line1,
			Parametric_3D_line Line2,
			double* Distance,
			Point_3D Crossing_Point )
{
	// distance function: C(t1,t2)=(x1(t1)-x2(t2))^2+(y1-y2)^2+(z1-z2)^2
	// crossing point is when dC/dt1=dC/dt2=0
	// from this condition t1 and t2 are found giving value of C(t1,t2) and
	// coordinates of the crossing


}

//--------------------------------------------------------------------------------

bool Numerical_Library::find_perpendicular_parametric_line_from_3_points(
		Point_3D Point_A, // coordinates of the points
		Point_3D Point_B, // line will go through point A
		Point_3D Point_C,
		Parametric_3D_line* Line )
{
	// returns true if calculations are successfull, false otherwise

	// assumptions and initial conditions:

	Line->b = Point_A.x;
	Line->d = Point_A.y;
	Line->f = Point_A.z;

	// rest of parameters is taken by computing normal vector to the plane
	// which is vector product of AB and AC vectors

	get_vector_product(Point_B.x-Point_A.x,Point_B.y-Point_A.y,Point_B.z-Point_A.z,
					   Point_C.x-Point_A.x,Point_C.y-Point_A.y,Point_C.z-Point_A.z,
					   &Line->a,&Line->c,&Line->e);

	return true;
}

//--------------------------------------------------------------------------------
void Numerical_Library::find_line_pasing_two_points_in_3D(
		Point_3D Point_A, // coordinates of the points
		Point_3D Point_B, // line will go through point A and B
		Parametric_3D_line* Line )
{
	Line->a = Point_B.x - Point_A.x;
	Line->c = Point_B.y - Point_A.y;
	Line->e = Point_B.z - Point_A.z;

	Line->b = Point_A.x;
	Line->d = Point_A.y;
	Line->f = Point_A.z;
}

//--------------------------------------------------------------------------------

bool Numerical_Library::solve_set_of_linear_equations_with_2_unknowns(
			double a1,double b1,double c1, // |a1*x+b1*y=c1|
			double a2,double b2,double c2, // |a2*x+b2*y=c2|
			double* x,double* y)
{

	double Determinant = a1*b2-a2*b1;

	if( Determinant!=0 )
	{
		x[0] = (c1*b2-c2*b1)/Determinant;
		y[0] = (a1*c2-a2*c1)/Determinant;
		return true;
	}
	else
	return false;
}

//--------------------------------------------------------------------------------

void Numerical_Library::get_vector_product(double a1,double a2,double a3,
							double b1,double b2,double b3,
							double* x,double* y,double* z)
{
	x[0] =  a2*b3-a3*b2;
	y[0] = -a1*b3+a3*b1;
	z[0] =  a2*b1-a1*b2;
}


//--------------------------------------------------------------------------------

void Numerical_Library::rotate_xyz_in_polar_system(double* x,double* y,double* z,double Azimuth,double Zenith)
{

	double r = sqrt( x[0]*x[0] + y[0]*y[0] + z[0]*z[0] );
	double Curr_Azimuth,Curr_Zenith;

	if( x[0] == 0 && y[0] == 0 )
	Curr_Azimuth = 0;
	else
	Curr_Azimuth = atan2( y[0], x[0] );

	Curr_Zenith = atan2( sqrt(x[0]*x[0]+y[0]*y[0]), z[0] );

	x[0] = r*cos(Curr_Azimuth+Azimuth)*sin(Curr_Zenith+Zenith);
	y[0] = r*sin(Curr_Azimuth+Azimuth)*sin(Curr_Zenith+Zenith);
	z[0] = r*cos(Curr_Zenith+Zenith);

}
//---------------------------------------------------------------------------

bool Numerical_Library::intersectRayTriangle(Vector3d orig, Vector3d dir,
		Vector3d vert0, Vector3d vert1, Vector3d vert2,
		Vector3d baryPosition, double *distance)
{
		Vector3d TV;
		Vector3d dist;

		// find vectors for two edges sharing vert0
		Vector3d edge1 = TV.subAndAssign(vert1,vert0);
		Vector3d edge2 = TV.subAndAssign(vert2,vert0);

		// begin calculating determinant - also used to calculate U parameter
		Vector3d p = TV.crossAndAssign(dir, edge2);

		// if determinant is near zero, ray lies in plane of triangle
		double det = TV.dotproduct(edge1, p);

		Vector3d Perpendicular;

		if(det > std::numeric_limits<double>::epsilon())
		{
			// calculate distance from vert0 to ray origin
			dist = TV.subAndAssign(orig,vert0);

			// calculate U parameter and test bounds
			baryPosition.x = TV.dotproduct(dist, p);
			if(baryPosition.x < static_cast<double>(0) || baryPosition.x > det)
				return false;

			// prepare to test V parameter
			Perpendicular = TV.crossAndAssign(dist, edge1);

			// calculate V parameter and test bounds
			baryPosition.y = TV.dotproduct(dir, Perpendicular);
			if((baryPosition.y < static_cast<double>(0)) || ((baryPosition.x + baryPosition.y) > det))
				return false;
		}
		else if(det < -std::numeric_limits<double>::epsilon())
		{
			// calculate distance from vert0 to ray origin
			Vector3d dist = TV.subAndAssign(orig,vert0);

			// calculate U parameter and test bounds
			baryPosition.x = TV.dotproduct(dist, p);
			if((baryPosition.x > static_cast<double>(0)) || (baryPosition.x < det))
				return false;

			// prepare to test V parameter
			Perpendicular = TV.crossAndAssign(dist, edge1);

			// calculate V parameter and test bounds
			baryPosition.y = TV.dotproduct(dir, Perpendicular);
			if((baryPosition.y > static_cast<double>(0)) || (baryPosition.x + baryPosition.y < det))
				return false;
		}
		else
			return false; // ray is parallel to the plane of the triangle

		double inv_det = static_cast<double>(1) / det;

		// calculate distance, ray intersects triangle
		distance[0] = TV.dotproduct(edge2, Perpendicular) * inv_det;
		baryPosition.scale(inv_det);

		return true;
}

//---------------------------------------------------------------------------

void Numerical_Library::get_barycentric_coordinates_within_triangle_of_projected_3D_point(
			double Origin_x,double Origin_y,double Origin_z,
			double T1_x,double T1_y,double T1_z,
			double T2_x,double T2_y,double T2_z,
			double T3_x,double T3_y,double T3_z,
			double *Alpha,double *Beta,double *Gamma)
{
Vector3d TV;

	Vector3d Q(Origin_x,Origin_y,Origin_z);

	Vector3d T1(T1_x,T1_y,T1_z);
	Vector3d T2(T2_x,T2_y,T2_z);
	Vector3d T3(T3_x,T3_y,T3_z);

	// find vectors for two edges sharing vert0
	Vector3d u = TV.subAndAssign(T2,T1);
	Vector3d v = TV.subAndAssign(T3,T1);
	Vector3d n = TV.crossAndAssign(u,v);

	Vector3d w = TV.subAndAssign(Q,T1);

	Vector3d u_w = TV.crossAndAssign(u,w);
	Vector3d w_v = TV.crossAndAssign(w,v);

	float gamma = TV.dotproduct(u_w,n) / TV.dotproduct(n,n);   // u.cross(w).dot(n) / n.dot(n);
	float beta =  TV.dotproduct(w_v,n) / TV.dotproduct(n,n);   // w.cross(v).dot(n) / n.dot(n);
	float alpha = 1 - gamma - beta;

	Alpha[0] = alpha;
	Beta[0] = beta;
	Gamma[0] = gamma;

	/* https://gamedev.stackexchange.com/questions/28781/easy-way-to-project-point-onto-triangle-or-plane
	// u=P2?P1
	Eigen::Vector3f u = triangle_vertex_1 - triangle_vertex_0;
	// v=P3?P1
	Eigen::Vector3f v = triangle_vertex_2 - triangle_vertex_0;
	// n=u×v
	Eigen::Vector3f n = u.cross(v);
	// w=P?P1
	Eigen::Vector3f w = query_point - triangle_vertex_0;

	// Barycentric coordinates of the projection P?of P onto T:
	// ?=[(u×w)?n]/n²
	float gamma = u.cross(w).dot(n) / n.dot(n);
	// ?=[(w×v)?n]/n²
	float beta = w.cross(v).dot(n) / n.dot(n);
	float alpha = 1 - gamma - beta;
	// The point P? lies inside T if:
	return ((0 <= alpha) && (alpha <= 1) &&
			(0 <= beta)  && (beta  <= 1) &&
			(0 <= gamma) && (gamma <= 1));

	*/
}

//---------------------------------------------------------------------------
/* ten wyzej dziala, zostawilem ponizszy kod anyway, dla referencji

bool Numerical_Library::rayTriangleIntersect(
	const Vec3f &orig, const Vec3f &dir,
	const Vec3f &v0, const Vec3f &v1, const Vec3f &v2,
	float &t)
{
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution

    // compute plane's normal
    Vec3f v0v1 = v1 - v0;
    Vec3f v0v2 = v2 - v0;
    // no need to normalize
    Vec3f N = v0v1.crossProduct(v0v2); // N
    float area2 = N.length();

    // Step 1: finding P

    // check if ray and plane are parallel ?
    float NdotRayDirection = N.dotProduct(dir);
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0
        return false; // they are parallel so they don't intersect !

	// compute d parameter using equation 2
    float d = N.dotProduct(v0);

    // compute t (equation 3)
	t = (N.dotProduct(orig) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (t < 0) return false; // the triangle is behind

	// compute the intersection point using equation 1
    Vec3f P = orig + t * dir;

    // Step 2: inside-outside test
	Vec3f C; // vector perpendicular to triangle's plane

    // edge 0
    Vec3f edge0 = v1 - v0;
    Vec3f vp0 = P - v0;
    C = edge0.crossProduct(vp0);
    if (N.dotProduct(C) < 0) return false; // P is on the right side

    // edge 1
    Vec3f edge1 = v2 - v1;
    Vec3f vp1 = P - v1;
    C = edge1.crossProduct(vp1);
    if (N.dotProduct(C) < 0)  return false; // P is on the right side

    // edge 2
	Vec3f edge2 = v0 - v2;
    Vec3f vp2 = P - v2;
    C = edge2.crossProduct(vp2);
    if (N.dotProduct(C) < 0) return false; // P is on the right side;

    return true; // this ray hits the triangle
}
*/
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

unsigned char Numerical_Library::ToByte(bool Bits[8])
{
	unsigned char c=0;
	for(int i=0;i<8;i++)
	if( Bits[i] )
	c |= 1 << i;

	return c;
}

//---------------------------------------------------------------------------

void Numerical_Library::FromByte(unsigned char c,bool Bits[8])
{
	for(int i=0;i<8;i++)
	Bits[i] = (c& (1<<i)) != 0;
}

//---------------------------------------------------------------------------

std::vector <double> Numerical_Library::get_electrogram_phase(
			std::vector <double>* Signal,int EGM_Type, double Base_Period_ptr,double Time_Step_ms)
{
	std::vector <double> Filtered_Signal;

	Filtered_Signal = recompose_bipolar_signal(Signal,Base_Period_ptr,EGM_Type);
	Filtered_Signal = get_Hilbert_phase_using_convolution_double(&Filtered_Signal,Time_Step_ms );

	return Filtered_Signal;
}

//---------------------------------------------------------------------------

std::vector <Local_Activation_Class> Numerical_Library::get_local_activations_from_phase_signal(
			std::vector <double>* Signal,
			int EGM_Type, double Time_Step_ms)
{
	// 2. Caclulate peaks
	vector <double> DiffY;
	double tmp=0;
	DiffY.push_back(tmp);

	for(long t=0;t<(signed)Signal[0].size()-1;t++)
	if( -(Signal[0][t+1]-Signal[0][t]) > 0 ) // to get only negative peaks
	DiffY.push_back( -(Signal[0][t+1]-Signal[0][t]) );
	else
	DiffY.push_back( 0 );

	std::vector<long> Peaks = get_peak_positions(&DiffY,0.1,500.0/Time_Step_ms,80./Time_Step_ms);
//		double PDT,long Peak_Detection_Window,long Blanking_Window);

	// 3. Create loc act object
	Local_Activation_Class LA;
	std::vector <Local_Activation_Class> LAS;

	for(long p=0;p<Peaks.size();p++)
	{
		LA.Timing_In_EGM_ptr = Peaks[p];
		LAS.push_back(LA);
	}

	return LAS;
}


//---------------------------------------------------------------------------

double Numerical_Library::get_average_from_vector(std::vector<double> *Signal)
{
	double av=0;
	long Counter=0;

	for(long i=0;i<Signal[0].size();i++)
	{
		av += Signal[0][i];
		Counter++;
	}

	if(Counter>0)
	return av/(double)Counter;
	else
	return 0;
}

//---------------------------------------------------------------------------

template <typename TV>
double Numerical_Library::get_sum_of_vector(std::vector<TV> *Signal)
{
	double Sum=0;

	for(long i=0;i<Signal[0].size();i++)
	Sum += Signal[0][i];

	return Sum;
}
template double Numerical_Library::get_sum_of_vector(std::vector<double> *Signal);
template double Numerical_Library::get_sum_of_vector(std::vector<long> *Signal);

//---------------------------------------------------------------------------

double Numerical_Library::sign(double val)
{
	if( val >= 0 )
	return 1;
	else
	return -1;
}

//---------------------------------------------------------------------------

std::vector<int> Numerical_Library::create_01_signal_from_egm(
	std::vector<double> *Signal_1,std::vector<Local_Activation_Class> *Activations_Positions,int Width_samples)
{
	std::vector<int> Result;
	int tmp=0;
	Result.assign(Signal_1[0].size(),tmp);

	for(long t=0;t<Activations_Positions[0].size()-1;t++)
	if(Activations_Positions[0][t].Timing_In_EGM_ptr >= 0 && Activations_Positions[0][t].Timing_In_EGM_ptr + Width_samples < Result.size() )
	{
		for(int w=0;w<Width_samples;w++)
		Result[Activations_Positions[0][t].Timing_In_EGM_ptr+w]=1;
	}

	return Result;
}

//---------------------------------------------------------------------------

std::vector<int> Numerical_Library::binary2asdf(std::vector<int> *V)
{
	std::vector<int> Result;

	for(long t=0;t<V[0].size();t++)
	if(V[0][t] == 1 )
	{
		Result.push_back(t+1);
	}

	return Result;
}

//---------------------------------------------------------------------------

void Numerical_Library::get_Transfer_Entropy(
	std::vector<int> *Binary_Signal_1,
	std::vector<int> *Binary_Signal_2,
	double* TE_00,double* TE_01,double* TE_10,double* TE_11,
	int y_delay)
{
	const size_t series_count = 2;
	double *te_result;
	int **all_series;

	std::vector<int> asdf_1 = binary2asdf(Binary_Signal_1);
	std::vector<int> asdf_2 = binary2asdf(Binary_Signal_2);

	const int duration = Binary_Signal_1[0].size(); // length of the raw egm signal

	size_t* series_lengths = (size_t*)malloc(sizeof(size_t) * series_count);

	series_lengths[0] = asdf_1.size();
	series_lengths[1] = asdf_2.size();

	all_series = (int**)malloc(sizeof(int*) * series_count);
	for (int i = 0; i < series_count; ++i)
		all_series[i] = (int*)malloc(sizeof(int) * series_lengths[i]);

//	ofstream as1,as2;
//	as1.open("C:\\A_01_asdf.csv");
//	as2.open("C:\\B_01_asdf.csv");

	for(long i=0;i<asdf_1.size();i++)
	{
		all_series[0][i]=asdf_1[i];
//		as1 << asdf_1[i] << endl;
	}

	for(long i=0;i<asdf_2.size();i++)
	{
		all_series[1][i]=asdf_2[i];
//		as2 << asdf_2[i] << endl;
	}

//	as1.close();
//	as2.close();

	// Create result matrix
	te_result = (double*)malloc(sizeof(double) * series_count * series_count);

	transent_1(all_series, series_count,series_lengths,
			   y_delay, duration,te_result);

	TE_00[0] = te_result[0];
	TE_01[0] = te_result[1];
	TE_10[0] = te_result[2];
	TE_11[0] = te_result[3];

	// w release sprawdz czy sa dobre wartosci
//	ShowMessage(FloatToStr(TE_00[0])+","+FloatToStr(TE_01[0])+","+FloatToStr(TE_10[0])+","+FloatToStr(TE_11[0]));

	// Clean up
	for(long i = 0; i < series_count; ++i)
	free(all_series[i]);

	free(all_series);
	free(te_result);
}

//---------------------------------------------------------------------------
/*
int read_int(FILE *fp) {
  unsigned char buffer[4];
  fread(buffer, 1, 4, fp);
  return (((int)buffer[0] << 24) | ((int)buffer[1] << 16) | ((int)buffer[2] << 8) | (int)buffer[3]);
}

// ===========================================================================
*/
/*
int main(int argc, char *argv[]) {

  FILE *fp;
  size_t i, j;
  size_t series_count;
  size_t x_order, y_order;
  int  y_delay, duration;
  int  **all_series;
  double *te_result;

  if (argc < 4) {
	printf("Usage: transent series_file results_file y_delay [x_order] [y_order]\n");
	return (0);
  }

  // Read in time series
  fp = fopen(argv[1], "r");
  duration = read_int(fp), series_count = read_int(fp);

  all_series = (int **)malloc(sizeof(int *) * series_count);

  size_t* series_lengths = (size_t*)malloc(sizeof(size_t) * series_count);

  for (i = 0; i < series_count; ++i)
  {
	series_lengths[i] = read_int(fp) + 1; // +1 for terminator
	all_series[i] = (int *)malloc(sizeof(int ) * series_lengths[i]);
  }

  for (i = 0; i < series_count; ++i)
  {
	for (j = 0; j < series_lengths[i] - 1; ++j)
	{
	  all_series[i][j] = read_int(fp);
    }

	all_series[i][j] = duration + 1; // terminator
  }

  fclose(fp);

  // Extract arguments
  y_delay = (int )atoi(argv[3]);

  if (argc > 4) {
    x_order = (size_t)atoi(argv[4]);
  }
  else {
    x_order = 1;
  }

  if (argc > 5) {
	y_order = (size_t)atoi(argv[5]);
  }
  else {
	y_order = 1;
  }

  // Create result matrix
	te_result = (double*)malloc(sizeof(double) * series_count * series_count);

  // Do calculation
  if ((x_order == 1) && (y_order == 1)) {
	transent_1(all_series, series_count,
			   series_lengths,
			   y_delay, duration,
			   te_result);
  } else {
    transent_ho(all_series, series_count,
				series_lengths,
                x_order, y_order,
                y_delay, duration,
                te_result);
  }

  fp = fopen(argv[2], "w");

  for (i = 0; i < series_count; ++i) {
    for (j = 0; j < series_count; ++j) {
      fprintf(fp, "%e ", te_result[(i * series_count) + j]);
	}

    fprintf(fp, "\n");
  }

  fclose(fp);

  // Clean up
  for (i = 0; i < series_count; ++i) {
	free(all_series[i]);
  }

  free(all_series);
  free(series_lengths);
  free(te_result);

  return (0);
}

*/

//---------------------------------------------------------------------------------------

/* Computes the first-order transfer entropy matrix for all pairs. */
void Numerical_Library::transent_1(int  **all_series, const size_t series_count,
 const size_t *series_lengths,
 const int  y_delay,
 const int  duration,
 double *te_result)
{

  /* Constants */
  const size_t x_order = 1, y_order = 1,
			   num_series = 3,
               num_counts = 8,
               num_x = 4,
               num_y = 2;

  /* Locals */
  int  counts[num_counts];
  uint64_t code;
  size_t k, l, idx, c1, c2;
  double te_final, prob_1, prob_2, prob_3;

  int  *ord_iter[num_series];
  int  *ord_end[num_series];

  int  ord_times[num_series];
  int  ord_shift[num_series];

  const size_t window = y_order + y_delay;
  const int  end_time = duration - window + 1;
  int  cur_time, next_time;

  /* Calculate TE */
  int  *i_series, *j_series;
  size_t i_size, j_size;
  size_t i, j;

  for (i = 0; i < series_count; ++i) {
    for (j = 0; j < series_count; ++j) {

      /* Extract series */
      i_size = series_lengths[i];
	  i_series = all_series[i];

      j_size = series_lengths[j];
      j_series = all_series[j];

      /* Order is x^(k+1), y^(l) */
	  idx = 0;

      /* x^(k+1) */
      for (k = 0; k < (x_order + 1); ++k) {
        ord_iter[idx] = i_series;
        ord_end[idx] = i_series + i_size;
		ord_shift[idx] = (window - 1) - k;

		while (*(ord_iter[idx]) < ord_shift[idx] + 1) {
          ++(ord_iter[idx]);
        }

        ord_times[idx] = *(ord_iter[idx]) - ord_shift[idx];
        ++idx;
      }

	  /* y^(l) */
      for (k = 0; k < y_order; ++k) {
        ord_iter[idx] = j_series;
        ord_end[idx] = j_series + j_size;
        ord_shift[idx] = (window - 1) - y_delay -k;

        while (*(ord_iter[idx]) < ord_shift[idx] + 1) {
          ++(ord_iter[idx]);
        }

        ord_times[idx] = *(ord_iter[idx]) - ord_shift[idx];
        ++idx;
      }

      /* Count spikes */
      memset(counts, 0, sizeof(int ) * num_counts);

      /* Get minimum next time bin */
	  cur_time = ord_times[0];
	  for (k = 1; k < num_series; ++k) {
        if (ord_times[k] < cur_time) {
		  cur_time = ord_times[k];
        }
      }

      while (cur_time <= end_time) {

        code = 0;
        next_time = end_time + 1;

        /* Calculate hash code for this time bin */
        for (k = 0; k < num_series; ++k) {
          if (ord_times[k] == cur_time) {
            code |= 1 << k;

			/* Next spike for this neuron */
			++(ord_iter[k]);

            if (ord_iter[k] == ord_end[k]) {
              ord_times[k] = end_time + 1;
            }
            else {
              ord_times[k] = *(ord_iter[k]) - ord_shift[k];
            }
          }

		  /* Find minimum next time bin */
          if (ord_times[k] < next_time) {
            next_time = ord_times[k];
          }
        }

        ++(counts[code]);
        cur_time = next_time;

      } /* while spikes left */

      /* Fill in zero count */
	  counts[0] = end_time;
      for (k = 1; k < num_counts; ++k) {
        counts[0] -= counts[k];
      }

      /* ===================================================================== */

      /* Use counts to calculate TE */
      te_final = 0;

      /* Order is x^(k), y^(l), x(n+1) */
      for (k = 0; k < num_counts; ++k) {
        prob_1 = (double)counts[k] / (double)end_time;

		if (prob_1 == 0) {
		  continue;
		}

        prob_2 = (double)counts[k] / (double)(counts[k] + counts[k ^ 1]);

        c1 = 0;
        c2 = 0;

		for (l = 0; l < num_y; ++l) {
          idx = (k & (num_x - 1)) + (l << (x_order + 1));
          c1 += counts[idx];
          c2 += (counts[idx] + counts[idx ^ 1]);
        }

        prob_3 = (double)c1 / (double)c2;

        te_final += (prob_1 * log2(prob_2 / prob_3));
      }

	  te_result[(j * series_count) + i] = te_final;

    } /* for j */

  } /* for i */

} /* transent_1 */

//---------------------------------------------------------------------------------------

/* Computes the higher-order transfer entropy matrix for all pairs. */
void Numerical_Library::transent_ho(int  **all_series, const size_t series_count,
 const size_t *series_lengths,
 const size_t x_order, const size_t y_order,
 const int  y_delay,
 const int  duration,
 double *te_result)
{

  /* Constants */
  const size_t num_series = 1 + y_order + x_order,
			   num_counts = (size_t)pow(2, num_series),
			   num_x = (size_t)pow(2, x_order + 1),
			   num_y = (size_t)pow(2, y_order);

  /* Locals */
  int  *counts = (int *)malloc(sizeof(int ) * num_counts);
  uint64_t code;
  size_t k, l, idx, c1, c2;
  double te_final, prob_1, prob_2, prob_3;

  int  *ord_iter[num_series];
  int  *ord_end[num_series];

  int  ord_times[num_series];
  int  ord_shift[num_series];

  const size_t window = (y_order + y_delay) > (x_order + 1) ? (y_order + y_delay) : (x_order + 1);
  const int  end_time = duration - window + 1;
  int  cur_time, next_time;

  /* Calculate TE */
  int  *i_series, *j_series;
  size_t i_size, j_size;
  size_t i, j;

  for (i = 0; i < series_count; ++i) {
    for (j = 0; j < series_count; ++j) {

      /* Extract series */
      i_size = series_lengths[i];
      i_series = all_series[i];

      j_size = series_lengths[j];
      j_series = all_series[j];

      /* Order is x^(k+1), y^(l) */
	  idx = 0;

      /* x^(k+1) */
      for (k = 0; k < (x_order + 1); ++k) {
        ord_iter[idx] = i_series;
		ord_end[idx] = i_series + i_size;
        ord_shift[idx] = (window - 1) - k;

        while (*(ord_iter[idx]) < ord_shift[idx] + 1) {
          ++(ord_iter[idx]);
        }

        ord_times[idx] = *(ord_iter[idx]) - ord_shift[idx];
        ++idx;
      }

      /* y^(l) */
      for (k = 0; k < y_order; ++k) {
        ord_iter[idx] = j_series;
        ord_end[idx] = j_series + j_size;
        ord_shift[idx] = -k;
        ord_times[idx] = *(ord_iter[idx]) - ord_shift[idx];
        ++idx;
	  }

      /* Count spikes */
      memset(counts, 0, sizeof(int ) * num_counts);

      /* Get minimum next time bin */
      cur_time = ord_times[0];
      for (k = 1; k < num_series; ++k) {
        if (ord_times[k] < cur_time) {
          cur_time = ord_times[k];
        }
      }

      while (cur_time <= end_time) {

        code = 0;
        next_time = end_time + 1;

        /* Calculate hash code for this time bin */
        for (k = 0; k < num_series; ++k) {
          if (ord_times[k] == cur_time) {
            code |= 1 << k;

            /* Next spike for this neuron */
            ++(ord_iter[k]);

            if (ord_iter[k] == ord_end[k]) {
			  ord_times[k] = end_time + 1;
            }
            else {
              ord_times[k] = *(ord_iter[k]) - ord_shift[k];
            }
          }

		  /* Find minimum next time bin */
          if (ord_times[k] < next_time) {
            next_time = ord_times[k];
          }
        }

        ++(counts[code]);
        cur_time = next_time;

      } /* while spikes left */

	  /* Fill in zero count */
	  counts[0] = end_time;
      for (k = 1; k < num_counts; ++k) {
        counts[0] -= counts[k];
      }

      /* ===================================================================== */

      /* Use counts to calculate TE */
      te_final = 0;

      /* Order is x^(k), y^(l), x(n+1) */
      for (k = 0; k < num_counts; ++k) {
        prob_1 = (double)counts[k] / (double)end_time;

        if (prob_1 == 0) {
          continue;
        }

        prob_2 = (double)counts[k] / (double)(counts[k] + counts[k ^ 1]);

        c1 = 0;
        c2 = 0;

        for (l = 0; l < num_y; ++l) {
          idx = (k & (num_x - 1)) + (l << (x_order + 1));
          c1 += counts[idx];
		  c2 += (counts[idx] + counts[idx ^ 1]);
        }

        prob_3 = (double)c1 / (double)c2;

        te_final += (prob_1 * log2(prob_2 / prob_3));
      }

      te_result[(j * series_count) + i] = te_final;

    } /* for j */

  } /* for i */

  /* Clean up */
  free(counts);

} /* transent_ho */

//---------------------------------------------------------------------------------------
