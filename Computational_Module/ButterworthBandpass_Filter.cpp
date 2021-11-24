//---------------------------------------------------------------------------

#pragma hdrstop

#include <fstream>

#include "ButterworthBandpass_Filter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

Butterworth_BandPass_Filter::Butterworth_BandPass_Filter()
{
}

//---------------------------------------------------------------------------

Butterworth_BandPass_Filter::~Butterworth_BandPass_Filter()
{
}

//---------------------------------------------------------------------------

void Butterworth_BandPass_Filter::Initialize_Filter(int Filter_Order_Init,
		double LowCutoff,double HighCutoff)
{
	Filter_Order = Filter_Order_Init;

	DenC = ComputeDenCoeffs(Filter_Order, LowCutoff,HighCutoff);
	NumC = ComputeNumCoeffs_v2(Filter_Order,LowCutoff,HighCutoff);

	double v=0;
	X.assign(2*Filter_Order+1,v);
	Y.assign(2*Filter_Order+1,v);

}

//---------------------------------------------------------------------------

double* Butterworth_BandPass_Filter::ComputeLP( int FilterOrder )
{
	double *NumCoeffs;
	int m;
	int i;

	NumCoeffs = (double *)calloc( FilterOrder+1, sizeof(double) );
	if( NumCoeffs == NULL ) return( NULL );

	NumCoeffs[0] = 1;
	NumCoeffs[1] = FilterOrder;
	m = FilterOrder/2;
	for( i=2; i <= m; ++i)
	{
		NumCoeffs[i] =(double) (FilterOrder-i+1)*NumCoeffs[i-1]/i;
		NumCoeffs[FilterOrder-i]= NumCoeffs[i];
	}
	NumCoeffs[FilterOrder-1] = FilterOrder;
	NumCoeffs[FilterOrder] = 1;

	return NumCoeffs;
}

//---------------------------------------------------------------------------

double* Butterworth_BandPass_Filter::ComputeHP( int FilterOrder )
{
	double *NumCoeffs;
	int i;

	NumCoeffs = ComputeLP(FilterOrder);
	if(NumCoeffs == NULL ) return( NULL );

	for( i = 0; i <= FilterOrder; ++i)
        if( i % 2 ) NumCoeffs[i] = -NumCoeffs[i];

    return NumCoeffs;
}

//---------------------------------------------------------------------------

double* Butterworth_BandPass_Filter::TrinomialMultiply( int FilterOrder, double *b, double *c )
{
	int i, j;
	double *RetVal;

	RetVal = (double *)calloc( 4 * FilterOrder, sizeof(double) );
	if( RetVal == NULL ) return( NULL );

	RetVal[2] = c[0];
	RetVal[3] = c[1];
	RetVal[0] = b[0];
	RetVal[1] = b[1];

	for( i = 1; i < FilterOrder; ++i )
	{
		RetVal[2*(2*i+1)]   += c[2*i] * RetVal[2*(2*i-1)]   - c[2*i+1] * RetVal[2*(2*i-1)+1];
        RetVal[2*(2*i+1)+1] += c[2*i] * RetVal[2*(2*i-1)+1] + c[2*i+1] * RetVal[2*(2*i-1)];

		for( j = 2*i; j > 1; --j )
        {
            RetVal[2*j]   += b[2*i] * RetVal[2*(j-1)]   - b[2*i+1] * RetVal[2*(j-1)+1] +
                c[2*i] * RetVal[2*(j-2)]   - c[2*i+1] * RetVal[2*(j-2)+1];
			RetVal[2*j+1] += b[2*i] * RetVal[2*(j-1)+1] + b[2*i+1] * RetVal[2*(j-1)] +
                c[2*i] * RetVal[2*(j-2)+1] + c[2*i+1] * RetVal[2*(j-2)];
		}

		RetVal[2] += b[2*i] * RetVal[0] - b[2*i+1] * RetVal[1] + c[2*i];
		RetVal[3] += b[2*i] * RetVal[1] + b[2*i+1] * RetVal[0] + c[2*i+1];
		RetVal[0] += b[2*i];
		RetVal[1] += b[2*i+1];
	}

	return RetVal;
}

//---------------------------------------------------------------------------

std::vector<double> Butterworth_BandPass_Filter::ComputeNumCoeffs_v2(int FilterOrder,
				double Lcutoff, double Ucutoff)
{
	double *TCoeffs;
	double *NumCoeffs;
	std::complex<double> *NormalizedKernel;
	int i;

	NumCoeffs = (double *)calloc( 2*FilterOrder+1, sizeof(double) );
	NormalizedKernel = (std::complex<double> *)calloc( 2*FilterOrder+1, sizeof(std::complex<double>) );
	TCoeffs = ComputeHP(FilterOrder);

	for( i = 0; i < FilterOrder; ++i)
	{
		NumCoeffs[2*i] = TCoeffs[i];
        NumCoeffs[2*i+1] = 0.0;
	}
	NumCoeffs[2*FilterOrder] = TCoeffs[FilterOrder];
    double cp[2];
    double Bw, Wn;
	cp[0] = 2*2.0*tan(M_PI * Lcutoff/ 2.0);
	cp[1] = 2*2.0*tan(M_PI * Ucutoff / 2.0);

    Bw = cp[1] - cp[0];
    //center frequency
    Wn = sqrt(cp[0]*cp[1]);
    Wn = 2*atan2(Wn,4);
	double kern;
	const std::complex<double> result = std::complex<double>(-1,0);

	for(int k = 0; k<2*FilterOrder+1; k++)
    {
		NormalizedKernel[k] = exp(-sqrt(result)*Wn*(double)k);
    }
    double b=0;
    double den=0;
	for(int d = 0; d<2*FilterOrder+1; d++)
	{
		b+=real(NormalizedKernel[d]*NumCoeffs[d]);
		den+=real(NormalizedKernel[d]*DenC[d]);
	}
	for(int c = 0; c<2*FilterOrder+1; c++)
	{
		NumCoeffs[c]=(NumCoeffs[c]*den)/b;
	}

	free(TCoeffs);

	std::vector<double> NumCoeffs_vec;
	for(int i=0;i<2*FilterOrder+1;i++)
	NumCoeffs_vec.push_back(NumCoeffs[i]);

	return NumCoeffs_vec;

//	return NumCoeffs;
}

//--------------------------------------------------------------------------
/*

double* Butterworth_BandPass_Filter::ComputeNumCoeffs_v2(int FilterOrder,
				double Lcutoff, double Ucutoff,double *DenC)
{
	double *TCoeffs;
	double *NumCoeffs;
	std::complex<double> *NormalizedKernel;
	int i;

	NumCoeffs = (double *)calloc( 2*FilterOrder+1, sizeof(double) );
	if( NumCoeffs == NULL ) return( NULL );

	NormalizedKernel = (std::complex<double> *)calloc( 2*FilterOrder+1, sizeof(std::complex<double>) );
	if( NormalizedKernel == NULL ) return( NULL );

	TCoeffs = ComputeHP(FilterOrder);
	if( TCoeffs == NULL ) return( NULL );

	for( i = 0; i < FilterOrder; ++i)
	{
		NumCoeffs[2*i] = TCoeffs[i];
		NumCoeffs[2*i+1] = 0.0;
	}
	NumCoeffs[2*FilterOrder] = TCoeffs[FilterOrder];
	double cp[2];
	double Bw, Wn;
	cp[0] = 2*2.0*tan(M_PI * Lcutoff/ 2.0);
    cp[1] = 2*2.0*tan(M_PI * Ucutoff / 2.0);

    Bw = cp[1] - cp[0];
    //center frequency
    Wn = sqrt(cp[0]*cp[1]);
    Wn = 2*atan2(Wn,4);
	double kern;
	const std::complex<double> result = std::complex<double>(-1,0);

	for(int k = 0; k<2*FilterOrder+1; k++)
    {
		NormalizedKernel[k] = exp(-sqrt(result)*Wn*(double)k);
    }
    double b=0;
	double den=0;
	for(int d = 0; d<2*FilterOrder+1; d++)
    {
		b+=real(NormalizedKernel[d]*NumCoeffs[d]);
        den+=real(NormalizedKernel[d]*DenC[d]);
    }
	for(int c = 0; c<2*FilterOrder+1; c++)
    {
        NumCoeffs[c]=(NumCoeffs[c]*den)/b;
	}

    free(TCoeffs);
	return NumCoeffs;
}

//--------------------------------------------------------------------------
double* Butterworth_BandPass_Filter::ComputeNumCoeffs(int FilterOrder)
{
	double *TCoeffs;
	double *NumCoeffs;
	int i;

	NumCoeffs = (double *)calloc( 2*FilterOrder+1, sizeof(double) );
	if( NumCoeffs == NULL ) return( NULL );

    TCoeffs = ComputeHP(FilterOrder);
	if( TCoeffs == NULL ) return( NULL );

	for( i = 0; i < FilterOrder; ++i)
    {
		NumCoeffs[2*i] = TCoeffs[i];
        NumCoeffs[2*i+1] = 0.0;
    }
	NumCoeffs[2*FilterOrder] = TCoeffs[FilterOrder];

	free(TCoeffs);

	return NumCoeffs;
}
*/

//---------------------------------------------------------------------------

std::vector<double> Butterworth_BandPass_Filter::ComputeDenCoeffs( int FilterOrder,
		double Lcutoff, double Ucutoff )
{
	int k;            // loop variables
	double theta;     // PI * (Ucutoff - Lcutoff) / 2.0
	double cp;        // cosine of phi
    double st;        // sine of theta
    double ct;        // cosine of theta
    double s2t;       // sine of 2*theta
    double c2t;       // cosine 0f 2*theta
	double *RCoeffs;     // z^-2 coefficients
    double *TCoeffs;     // z^-1 coefficients
	double *DenomCoeffs;     // dk coefficients
	double PoleAngle;      // pole angle
	double SinPoleAngle;     // sine of pole angle
    double CosPoleAngle;     // cosine of pole angle
	double a;         // workspace variables

	cp = cos(M_PI * (Ucutoff + Lcutoff) / 2.0);
    theta = M_PI * (Ucutoff - Lcutoff) / 2.0;
	st = sin(theta);
    ct = cos(theta);
    s2t = 2.0*st*ct;        // sine of 2*theta
	c2t = 2.0*ct*ct - 1.0;  // cosine of 2*theta

	RCoeffs = (double *)calloc( 2 * FilterOrder, sizeof(double) );
	TCoeffs = (double *)calloc( 2 * FilterOrder, sizeof(double) );

	for( k = 0; k < FilterOrder; ++k )
	{
		PoleAngle = M_PI * (double)(2*k+1)/(double)(2*FilterOrder);
		SinPoleAngle = sin(PoleAngle);
		CosPoleAngle = cos(PoleAngle);
        a = 1.0 + s2t*SinPoleAngle;
		RCoeffs[2*k] = c2t/a;
        RCoeffs[2*k+1] = s2t*CosPoleAngle/a;
        TCoeffs[2*k] = -2.0*cp*(ct+st*SinPoleAngle)/a;
        TCoeffs[2*k+1] = -2.0*cp*st*CosPoleAngle/a;
    }

	DenomCoeffs = TrinomialMultiply(FilterOrder, TCoeffs, RCoeffs );
	free(TCoeffs);
	free(RCoeffs);

	DenomCoeffs[1] = DenomCoeffs[0];
	DenomCoeffs[0] = 1.0;
	for( k = 3; k <= 2*FilterOrder; ++k )
		DenomCoeffs[k] = DenomCoeffs[2*k-2];

	std::vector<double> DenomCoeffs_vec;
	for(int i=0;i<4*FilterOrder;i++)
	DenomCoeffs_vec.push_back(DenomCoeffs[i]);

	return DenomCoeffs_vec;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void Butterworth_BandPass_Filter::filter(int np, double *x, double *y)
{
	int i,j;

	y[0]=NumC[0] * x[0];

	for (i=1;i<Filter_Order+1;i++)
	{
		y[i]=0.0;

		for (j=0;j<i+1;j++)
			y[i]=y[i]+NumC[j]*x[i-j];

		for (j=0;j<i;j++)
			y[i]=y[i]-DenC[j+1]*y[i-j-1];
	}

	for (i=Filter_Order+1;i<np+1;i++)
	{
		y[i]=0.0;

		for (j=0;j<Filter_Order+1;j++)
			y[i]=y[i]+NumC[j]*x[i-j];

		for (j=0;j<Filter_Order;j++)
			y[i]=y[i]-DenC[j+1]*y[i-j-1];
	}
}


//---------------------------------------------------------------------------

std::vector <double> Butterworth_BandPass_Filter::filter(std::vector <double> *Signal,
				bool Inverse)
{
	double v=0;

	X.clear();
	X.assign(2*Filter_Order+1,v);
	Y.clear();
	Y.assign(2*Filter_Order+1,v);

	std::vector <double> Result;
	std::vector <double> Rev_Result;

	//--------------------------------------------
	if( ! Inverse )
	//--------------------------------------------
	{

	for(long k = 0; k < Signal[0].size(); k ++ )
	{
		for(int i=0;i<2*Filter_Order;i++)
			X[i]=X[i+1];
		X[2*Filter_Order] = Signal[0][k];

		for(int i=0;i<2*Filter_Order;i++)
		Y[i]=Y[i+1];

		Y[2*Filter_Order] = 0;
		for(int i=0;i<2*Filter_Order+1;i++)
			Y[2*Filter_Order] += NumC[2*Filter_Order-i]*X[i];

		for(int i=0;i<2*Filter_Order;i++)
			Y[2*Filter_Order] += -DenC[2*Filter_Order-i]*Y[i];

		Result.push_back(Y[2*Filter_Order]);
	}

	  return Result;
	}

	//--------------------------------------------
	if( Inverse )
	//--------------------------------------------
	{

	for(long k = 0; k < Signal[0].size(); k ++ )
	{
		for(int i=0;i<2*Filter_Order;i++)
			X[i]=X[i+1];
		X[2*Filter_Order] = Signal[0][Signal[0].size()-1-k];

		for(int i=0;i<2*Filter_Order;i++)
		Y[i]=Y[i+1];

		Y[2*Filter_Order] = 0;
		for(int i=0;i<2*Filter_Order+1;i++)
			Y[2*Filter_Order] += NumC[2*Filter_Order-i]*X[i];

		for(int i=0;i<2*Filter_Order;i++)
			Y[2*Filter_Order] += -DenC[2*Filter_Order-i]*Y[i];

		Result.push_back(Y[2*Filter_Order]);
	}

	// inverse result
	for(long k=0;k<Result.size();k++)
	Rev_Result.push_back(Result[Result.size()-1-k]);

	return Rev_Result;

	}
}

//---------------------------------------------------------------------------

double Butterworth_BandPass_Filter::filter_sample(double Value)
{
	for(int i=0;i<2*Filter_Order;i++)
	X[i]=X[i+1];

	X[2*Filter_Order] = Value;

	for(int i=0;i<2*Filter_Order;i++)
		Y[i]=Y[i+1];

	Y[2*Filter_Order] = 0;

	for(int i=0;i<2*Filter_Order+1;i++)
		Y[2*Filter_Order] += NumC[2*Filter_Order-i]*X[i];

	for(int i=0;i<2*Filter_Order;i++)
		Y[2*Filter_Order] += -DenC[2*Filter_Order-i]*Y[i];

	return Y[2*Filter_Order];
}

//---------------------------------------------------------------------------

void Butterworth_BandPass_Filter::save_filter_to_file(std::string Filename)
{
	ofstream df(Filename.c_str());

	df << "Nom:\n";

	for(int n=0;n<NumC.size();n++)
	df << NumC[n] << ", ";

	df << "\nDen:\n";

	for(int n=0;n<DenC.size();n++)
	df << DenC[n] << ", ";

	df.close();
}

//---------------------------------------------------------------------------

