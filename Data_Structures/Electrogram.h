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
#ifndef Electrogram_classH
#define Electrogram_classH

#include <fstream>
#include <vector>
#include <vcl.h>

#include "Utils.h"

#include "Local_Activation.h"

//---------------------------------------------------------------------------

class Electrogram
{

	public:

	Electrogram();
	~Electrogram();

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

	Local_Activations_Vector Local_Activations;

	//----------------------------------------------------------------------
	// Name of the catheter readed as in file exported from Lab Pro
	//----------------------------------------------------------------------
	AnsiString Name;

	//----------------------------------------------------------------------
	// Time Sampling of the signal.
	//----------------------------------------------------------------------
	double Time_Step_ms;

	//----------------------------------------------------------------------
	// Vector of numbers. Here actual values of signal are stored
	//----------------------------------------------------------------------
	std::vector<double> Voltage_Values;
	std::vector<double> Filtered_Signal; //????? should we remove it???

	std::vector<double> Phase;
	double Base_Cycle_Used_For_Phase_ptr;

	//----------------------------------------------------------------------
	// Position of the activations in this signal. Calculated by
	// calculate_peak_positions() function.
	// In ptr units, not ms!
	//----------------------------------------------------------------------
	std::vector <long> vNAVX_Positions_In_Signal;
	std::vector <long> vICL_Positions_In_Signal;
	std::vector <long> vED_Positions_In_Signal;
	std::vector <long> vPeak_Positions;

	Utils_Tools Utils;

	//---------------------------------------------------
	// ISI support
	//---------------------------------------------------
	std::vector <double> ISIs; // inter-spike intervals
	std::vector <int> ISI_Histogram;
	double Min_ISI,Max_ISI,Average_ISI,SD_of_ISI;
	double DF_From_ISI,DF_From_Max_Freq_ISI;
	double ISI_Histogram_Box; // has to be set!

	// displayed segment
	long Min_Ptr, Max_Ptr; // limits of display
	void set_minmax_limits();

	double Mean_AFCL_From_Convergent_Sinusoidal_Recomposition;
	double Mean_AFCL_From_Convergent_Sinusoidal_Recomposition_SD;

	// MPD data
	double Roving_Bar_Postion;
	double Reference_Bar_Position;

	//------------------------------------------------------------------
	// PEAK ANALYSIS SUPPORT
	//------------------------------------------------------------------
	std::vector <long> Peaks_Positions_In_Signal;
	std::vector <bool> Peaks_Positions_In_Signal_Flags; // used in some analysis
	double Peak_Interval_Mean;
	double Peak_Interval_SD;
	double Last_Peak_Position_Relative_To_Roving_Position;

	std::vector <double> Wavelet_Spectrum;

	//------------------------------------------------------------------
	// NAVX CAFE SUPPORT
	//------------------------------------------------------------------
	double NavX_CAFE;
	std::vector <double> NavX_CAFE_Positions;

	//------------------------------------------------------------------
	// continuous activity support stuff: flag whether there is elecrical
	// activity at gien sample (legnth should be == to voltage_values)
	//------------------------------------------------------------------
	std::vector <int> Silence_Vector;

};

#endif


