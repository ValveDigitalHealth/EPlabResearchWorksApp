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

#ifndef NL_analysis_form_fileH
#define NL_analysis_form_fileH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>

#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <vcl.h>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <Vcl.Dialogs.hpp>

#include <fstream>
#include <math.h>
#include <vector>

#include "Ask_For_Single_Item_Form.h"
#include "predefined_parameters.h"
#include "FFTReal.h"
#include "Numerical_Library.h"
#include "Utils.h"
#include "ButterworthBandpass_Filter.h"
#include "Electrogram.h"
#include "values_vector.h"

class TNL_Analysis_Form : public TForm
{
    __published:	// IDE-managed Components
	TStringGrid *Primary_Data_StringGrid;
	TPageControl *PageControl1;
    TTabSheet *Poincare_TabSheet;
	TTabSheet *Filtering_TabSheet;
    TButton *PM_recalculate_Button;
    TLabel *PM_Delay_Label;
    TEdit *PM_Delay_Edit;
    TPaintBox *P_Map_PaintBox;
    TButton *Close_Button;
    TChart *Raw_Signals_Chart;
    TLineSeries *Series1;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Close1;
	TTabSheet *Correlation_TabSheet;
	TStringGrid *Secondary_Data_StringGrid;
	TChart *Scatter_Chart;
    TPointSeries *Series2;
    TLineSeries *Series3;
    TLabel *Range_Label;
	TTabSheet *Histogram_TabSheet;
    TChart *Histogram_Chart;
	TBarSeries *Series4;
    TLabel *Label3;
    TButton *Histo_Recalculate_Button;
    TEdit *Bin_Size_Edit;
	TTabSheet *FFT_TabSheet;
    TMenuItem *N1;
    TMenuItem *Exportcurrentdataserie1;
    TSaveDialog *SaveDialog1;
    TChart *FFT_Spectrum_Chart;
    TLineSeries *LineSeries1;
    TLabel *Stats_Label;
    TLabel *L_Label;
	TLabel *Volume_Label;
	TButton *Calculate_CORR_Table_Button;
	TPaintBox *CorrTable_PaintBox;
	TLabel *Label7;
	TEdit *Phase_Space_Resolution_Edit;
	TTabSheet *Wavelet_TabSheet;
	TPointSeries *Series8;
	TChart *Wavelet_Spectrum_Chart;
	TLineSeries *LineSeries2;
	TPointSeries *PointSeries1;
	TLabel *Label8;
	TEdit *Wavelet_Scaling_Min_Edit;
	TEdit *Wavelet_Scaling_Max_Edit;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TEdit *PDT_Edit;
	TEdit *PD_Blanking_Edit;
	TEdit *PDTW_Edit;
	TButton *Calculate_Wavelet_Spectrum_Button;
	TPopupMenu *DWT_Chart_Add_Remove_PopupMenu;
	TMenuItem *Addmarker1;
	TMenuItem *Removemarker1;
	TRadioGroup *Data_Source_RadioGroup;
	TTabSheet *Synchronization_TabSheet;
	TChart *Synchrogram_Chart;
	TPointSeries *PointSeries2;
	TButton *Calculate_Wavelet_For_All_Sigs_Button;
	TLineSeries *Series9;
	TMenuItem *ImportcurrentdataserieYvalue1;
	TOpenDialog *OpenDialog1;
	TRadioGroup *IndexORTime_RadioGroup;
	TCheckBox *CLs_Labels_Display_CheckBox;
	TRichEdit *ISI_RichEdit;
	TPaintBox *Synchronization_Matrix_PaintBox;
	TButton *Calculate_Synch_Table_Button;
	TRichEdit *Sych_Results_RichEdit;
	TCheckBox *Butt_band_40_250_3th_order_checkbox;
	TCheckBox *Rectification_checkbox;
	TCheckBox *Butt_low_20_3th_order_checkbox;
	TChart *Filtered_Signal_Chart;
	TLineSeries *Series6;
	TButton *Filter_All_Signals_Button;
	TChart *Cycle_Lengths_Chart;
	TTabSheet *Preak_Detection_TabSheet;
	TButton *Det_Peaks_Th_Button;
	TCheckBox *Range_Wavelet_Scaling_CheckBox;
	TLabel *Label5;
	TEdit *Wavelet_Step_Edit;
	TCheckBox *Disp_2nd_Signal_CheckBox;
	TRadioGroup *Wavelet_integrated_spect_RadioGroup;
	TPointSeries *Series10;
    TCheckBox *Normalize__CheckBox;
	TCheckBox *Hilb_Phase_CheckBox;
	TEdit *Normalization_Window_Edit;
	TCheckBox *Set_av_SD_CheckBox;
    TLabel *Secondary_Grid_Label;
    TPointSeries *Series5;
    TRichEdit *FFT_Results_RichEdit;
    TCheckBox *FilterAllSigs_byDefault_CheckBox;
    TChart *FullFFT_Chart;
    TLineSeries *Series11;
    TLabel *Label1;
    TLabel *Label4;
    TEdit *Min_DF_Edit;
    TEdit *Max_DF_Edit;
    TCheckBox *Sinusoidal_Recomposition_CheckBox;
	TLabel *Label6;
    TEdit *Sinsoidal_Wavelet_Period_Edit;
    TLineSeries *Series12;
    TRadioGroup *PDT_SignalMode_RadioGroup;
    TRichEdit *CLs_RichEdit;
    TRadioGroup *LCR_RadioGroup;
    TRadioGroup *Variability_Type_RadioGroup;
    TLabel *Label13;
    TEdit *C_Window_Size_Edit;
    TLabel *Label14;
	TEdit *LR_Window_Size_Edit;
    TTabSheet *Frac_TabSheet;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TEdit *CAFE_Ref_Edit;
    TEdit *CAFE_PP_Edit;
    TEdit *CAFE_Dur_Edit;
    TEdit *CAFE_DTh_Edit;
    TButton *Compute_NavX_CAFEs_Button;
    TPointSeries *Series13;
    TCheckBox *Sliding_Window_SD_CheckBox;
    TLabel *Label21;
	TEdit *SD_Window_Edit;
    TPointSeries *Series15;
	TRadioGroup *Electrogram_Type_RadioGroup;
	TLabel *Label22;
	TEdit *BW_Filter_Order_Edit;
	TLabel *Label23;
	TLabel *Label24;
	TEdit *BW_Filter_Up_Edit;
	TEdit *BW_Filter_Low_Edit;
	TCheckBox *Custom_Butt_Filter_CheckBox;
	TCheckBox *MedianFilter_CheckBox;
	TLabel *Label31;
	TEdit *Median_WL_Edit;
	TTabSheet *TimeShiftsTabSheet;
	TLabel *Label36;
	TLabel *Label37;
	TComboBox *Line_Thickness_ComboBox;
	TComboBox *ColorSet_ComboBox;
	TCheckBox *Background_grid_CheckBox;
	TCheckBox *ShSD_CheckBox;
	TTabSheet *Recur_TabSheet;
	TLabel *Label39;
	TLabel *Label40;
	TEdit *Rec_MPC_timestep_Edit;
	TEdit *Rec_11_Th_Edit;
	TPaintBox *Recurrence_Plot_PaintBox;
	TButton *Recurrence_ALL_Button;
	TButton *Recurrence_Pair_Button;
	TTabSheet *All_sigs_TabSheet;
	TPaintBox *Signals_Display_PaintBox;
	TLabel *Label41;
	TEdit *PercOcc_Block_Edit;
	TButton *PercOcc_Rec_Button;
	TLabel *Label42;
	TEdit *SPP_Edit;
	TTabSheet *TabSheet2;
	TButton *MultiSCale_Recomp_Button;
	TLabel *Label43;
	TLabel *Label44;
	TEdit *F1_Edit;
	TEdit *F2_Edit;
	TLabel *Label45;
	TEdit *F3_Edit;
	TPaintBox *MSC_PaintBox;
	TChart *SCM_Chart;
	TBarSeries *BarSeries1;
	TCheckBox *MCP_CheckBox;
	TMenuItem *Vwaveremoval1;
	TMenuItem *TakeVwavetemplatemorphologycurrentsegment1;
	TRadioGroup *WS_RadioGroup;
	TTabSheet *TabSheet3;
	TChart *Two_point_Causality_Chart;
	TPointSeries *PointSeries3;
	TLabel *Label15;
	TEdit *CMin_Edit;
	TEdit *CMax_Edit;
	TLabel *Label46;
	TEdit *CStep_Edit;
	TRadioGroup *CSource_RadioGroup;
	TRadioGroup *ECTC_RadioGroup;
	TLabel *Label47;
	TEdit *AFCL_Caus_Edit;
	TButton *Causality_Button;
	TLabel *Primary_Grid_Label;
	TMenuItem *Exports1;
	TMenuItem *N2;
	TMenuItem *ExportactivationstimesSinRecompHilb1;
	TTabSheet *TabSheet4;
	TChart *MPC_C_Chart1;
	TButton *MPC_Recalc_Button;
	TEdit *MS_Edit;
	TLabel *Label48;
	TButton *PreprocessandreclaclMPC_Button;
	TChart *Self_Convergence_Chart;
	TLineSeries *Series16;
	TLabel *Label49;
	TEdit *CnvST_Edit;
	TLineSeries *Series17;
	TLineSeries *Series18;
	TLineSeries *Series19;
	TLineSeries *Series20;
	TLineSeries *Series21;
	TLineSeries *Series22;
	TLineSeries *Series23;
	TLineSeries *Series24;
	TLineSeries *Series25;
	TLineSeries *Series26;
	TLineSeries *Series27;
	TLineSeries *Series28;
	TLineSeries *Series29;
	TLineSeries *Series30;
	TLineSeries *Series31;
	TLineSeries *Series32;
	TLineSeries *Series33;
	TLineSeries *Series34;
	TLineSeries *Series35;
	TLineSeries *Series36;
	TLineSeries *Series37;
	TLineSeries *Series38;
	TLineSeries *Series39;
	TLineSeries *Series40;
	TLineSeries *Series41;
	TLineSeries *Series42;
	TLineSeries *Series43;
	TLineSeries *Series44;
	TLineSeries *Series45;
	TLineSeries *Series46;
	TLineSeries *Series47;
	TLineSeries *Series48;
	TLineSeries *Series49;
	TLineSeries *Series50;
	TLineSeries *Series51;
	TButton *Self_Conv_Button;
	TButton *OneStarting_convergentSR_Button;
	TLabel *Label50;
	TEdit *SPC_Edit;
	TRadioGroup *MT1_RadioGroup;
	TCheckBox *Black_Markers_CheckBox;
	TLabel *Label51;
	TEdit *R11Edit;
	TButton *Synch_Movie_Start_Button;
	TButton *Synch_Movie_Pause_Button;
	TLineSeries *Series52;
	TLabel *Label53;
	TLabel *Label54;
	TButton *TPMPC_Button;
	TLabel *Label55;
	TLabel *Label56;
	TEdit *WS1_Edit;
	TEdit *WS2_Edit;
	TLineSeries *Series7;
	TLabel *Label57;
	TEdit *MAVCL_Edit;
	TMenuItem *ExportintervalsSinRecompHilbofalldatapoints1;
	TCheckBox *First_Derivative_CheckBox;
	TLabel *Label52;
	TEdit *SLWD_Edit;
	TLabel *Label58;
	TEdit *EST_Edit;
	TButton *CAR_Button;
	TLabel *COR_Label;
	TLabel *Label59;
	TEdit *BottomOffset_Edit;
	TButton *Repaint_All_Signals_Button;
	TCheckBox *Second_Derivative_CheckBox;
	TLabel *Label60;
	TEdit *Derivative_Range_Edit;
	TCheckBox *Sum_Abs_Window_CheckBox;
	TEdit *Sum_Abs_Window_Edit;
	TPointSeries *Series53;
	TChart *Resampled_Chart;
	TLineSeries *LineSeries4;
	TRichEdit *Hist_Tab_RichEdit;
	TCheckBox *Hist_Outliers_Remove_CheckBox;
	TChart *TimeShifts_Chart;
	TBarSeries *BarSeries2;
	TButton *CalculateTimeShiftHist_Button;
	TLabel *Label38;
	TButton *SelfConvergenceCurrentButton;
	TChart *TimeShiftsDynamicChart;
	TBarSeries *LineSeries3;
	TRichEdit *TimeShiftsRichEdit;
	TLabel *Label2;
	TEdit *OutThEdit;
	TLabel *Label62;
	TChart *AFCLs_Global_Chart;
	TLineSeries *Series54;
	TLineSeries *Series55;
	TRichEdit *LARAAFCLRichEdit;
	TCheckBox *Corr_V_Wave_Signal_CheckBox;
	TEdit *VWaveCorrStepEdit;
	TCheckBox *PSimilarity_CheckBox;
	TLineSeries *Series14;
	TCheckBox *XCorr_Filter_CheckBox;
	TCheckBox *SLWMM_CheckBox;
	TLabel *Label12;
	TEdit *SlidingmaxminEdit;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Close_ButtonClick(TObject *Sender);
    void __fastcall Primary_Data_StringGridClick(TObject *Sender);
    void __fastcall Close1Click(TObject *Sender);
	void __fastcall Secondary_Data_StringGridClick(TObject *Sender);
    void __fastcall Histo_Recalculate_ButtonClick(TObject *Sender);
    void __fastcall PageControl1Change(TObject *Sender);
    void __fastcall Exportcurrentdataserie1Click(TObject *Sender);
	void __fastcall Update_CORR_Table_ButtonClick(TObject *Sender);
	void __fastcall CorrTable_PaintBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Calculate_Wavelet_Spectrum_ButtonClick(TObject *Sender);
	void __fastcall Raw_Signals_ChartClickSeries(TCustomChart *Sender, TChartSeries *Series,
          int ValueIndex, TMouseButton Button, TShiftState Shift, int X,
          int Y);
	void __fastcall Raw_Signals_ChartMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Raw_Signals_ChartMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall Raw_Signals_ChartMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall Addmarker1Click(TObject *Sender);
	void __fastcall Removemarker1Click(TObject *Sender);
	void __fastcall PM_recalculate_ButtonClick(TObject *Sender);
	void __fastcall Data_Source_RadioGroupClick(TObject *Sender);
	void __fastcall Calculate_Wavelet_For_All_Sigs_ButtonClick(TObject *Sender);
	void __fastcall ImportcurrentdataserieYvalue1Click(TObject *Sender);
	void __fastcall CLs_Labels_Display_CheckBoxClick(TObject *Sender);
	void __fastcall IndexORTime_RadioGroupClick(TObject *Sender);
	void __fastcall Calculate_Synch_Table_ButtonClick(TObject *Sender);
	void __fastcall Synchronization_Matrix_PaintBoxPaint(TObject *Sender);

	void __fastcall r_ceckboxClick(TObject *Sender);
	void __fastcall Rectification_checkboxClick(TObject *Sender);
	void __fastcall Butt_low_20_3th_order_checkboxClick(TObject *Sender);
	void __fastcall Filter_All_Signals_ButtonClick(TObject *Sender);
	void __fastcall Det_Peaks_Th_ButtonClick(TObject *Sender);
	void __fastcall Disp_2nd_Signal_CheckBoxClick(TObject *Sender);
	void __fastcall Synchronization_Matrix_PaintBoxMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall Normalze_CheckBoxClick(TObject *Sender);
	void __fastcall Hilb_Phase_CheckBoxClick(TObject *Sender);
	void __fastcall P_Map_PaintBoxPaint(TObject *Sender);
	void __fastcall CorrTable_PaintBoxPaint(TObject *Sender);
    void __fastcall FormHide(TObject *Sender);
    void __fastcall Raw_Signals_ChartDblClick(TObject *Sender);
    void __fastcall InvTime_CheckBoxClick(TObject *Sender);
    void __fastcall Sinusoidal_Recomposition_CheckBoxClick(TObject *Sender);
    void __fastcall Compute_NavX_CAFEs_ButtonClick(TObject *Sender);
	void __fastcall MedianFilter_CheckBoxClick(TObject *Sender);
	void __fastcall Line_Thickness_ComboBoxChange(TObject *Sender);
	void __fastcall ColorSet_ComboBoxChange(TObject *Sender);
	void __fastcall Background_grid_CheckBoxClick(TObject *Sender);
	void __fastcall Raw_Signals_ChartZoom(TObject *Sender);
	void __fastcall Raw_Signals_ChartUndoZoom(TObject *Sender);
	void __fastcall Recurrence_Pair_ButtonClick(TObject *Sender);
	void __fastcall Signals_Display_PaintBoxPaint(TObject *Sender);
	void __fastcall Signals_Display_PaintBoxMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall Signals_Display_PaintBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Signals_Display_PaintBoxMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall PercOcc_Rec_ButtonClick(TObject *Sender);
	void __fastcall MultiSCale_Recomp_ButtonClick(TObject *Sender);
	void __fastcall TakeVwavetemplatemorphologycurrentsegment1Click(TObject *Sender);
	void __fastcall Causality_ButtonClick(TObject *Sender);
	void __fastcall ExportactivationstimesSinRecompHilb1Click(TObject *Sender);
	void __fastcall MPC_Recalc_ButtonClick(TObject *Sender);
	void __fastcall PreprocessandreclaclMPC_ButtonClick(TObject *Sender);
	void __fastcall Self_Conv_ButtonClick(TObject *Sender);
	void __fastcall OneStarting_convergentSR_ButtonClick(TObject *Sender);
	void __fastcall Black_Markers_CheckBoxClick(TObject *Sender);
	void __fastcall MCP_CheckBoxClick(TObject *Sender);
	void __fastcall Synch_Movie_Pause_ButtonClick(TObject *Sender);
	void __fastcall Synch_Movie_Start_ButtonClick(TObject *Sender);
	void __fastcall TPMPC_ButtonClick(TObject *Sender);
	void __fastcall ExportintervalsSinRecompHilbofalldatapoints1Click(TObject *Sender);
	void __fastcall CAR_ButtonClick(TObject *Sender);
	void __fastcall Repaint_All_Signals_ButtonClick(TObject *Sender);
	void __fastcall First_Derivative_CheckBoxClick(TObject *Sender);
	void __fastcall Second_Derivative_CheckBoxClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Hist_Outliers_Remove_CheckBoxClick(TObject *Sender);
	void __fastcall ShSD_CheckBoxClick(TObject *Sender);
	void __fastcall CalculateTimeShiftHist_ButtonClick(TObject *Sender);
	void __fastcall SelfConvergenceCurrentButtonClick(TObject *Sender);


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	private:	// User declarations
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	Utils_Tools UT;
	Numerical_Library Numerical_Library_Obj;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	public:		// User declarations
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	__fastcall TNL_Analysis_Form(TComponent* Owner);

	AnsiString Study_Name;

	bool Echo;

	//**********************************************
	//**********************************************
	std::vector <Electrogram*> Data_Items;

	int Selected_Data_Item_1;
	int Selected_Data_Item_2;

	std::vector <double> get_data_for_analysis(int Set_Id);

	bool selected_data_items_in_range();

	void update_data_list();

	//**********************************************
	//**********************************************

	bool Hold_Recalculations;
	void repaint_current_tab();

	void recalculate_wavelet_spectrum_page(int Selected_Data_Item);
	void calculate_ISIs_from_wavelet_peaks(int Selected_Data_Item);
	void fill_wavelet_CL_plots();

	void detect_peaks_in_signal(int Selected_Data_Item);

	double Histo_Bin_Size;
	double Histo_Min,Histo_Max;
	std::vector <long> Histogram_Data;
	double Percentile_5;
	double Histogram_Shen;
	int recalculate_histogram();


	//-----------------------------------------------------------------------
	// FFT SUPPORT
	//-----------------------------------------------------------------------
	std::vector <double> Resampled_Signal_For_FFT;
	double* Signal_for_FFT;
	long Signal_for_FFT_Size;
	double Sampling_Per_Sec_of_Rescaled_Signal; // OF RESCALED SIGNAL; BEFORE, WAS: = 1000.0/Data_Items[0].Time_Step;
	double* FFT_spectrum;
	double DF,Max_DF_Amplitude;
	double DF_Regularity_Index,DF_Organization_Index;
	int calculate_FFT_spectrum(std::vector <double> *Data_Vec, long Start, long Stop);
	double get_FFT_longegral(double Start_Hz,double Stop_Hz, double Coef,
		long FFT_Data_Length, double* FFT_Vector);
	void paint_FFT_spectrum();
	double VT_FFT_Threshold_1; // Hz
	double VT_FFT_Threshold_2; // Hz
	double FFTr; // fft ratio (for VT frequency analysis)

	double Correlation_Matrix[100][100]; // assumption: there is less than 100 data items in list
	int Correlation_Matrix_Size;
	void paint_R_matrix();

	//-----------------------------------------------------------------------
	// Mouse support
	//-----------------------------------------------------------------------
	typedef enum {
		  MB_DOWN1,
		  MB_UP1
		  } Emouse;
	Emouse MouseStatus;
	TMouseButton MouseButton;
	int Mouse_Position_X,Mouse_Position_Y;

	//-----------------------------------------------------------------------
	// Chart dragging support
	//-----------------------------------------------------------------------
	TChartSeries *Dragged_Series;
	bool Marker_Dragging_Flag;
	long Dragged_Value_Index;
	long Mouse_Button_Down;
	TMouseButton WHICH_BUTTON;

	void update_Peak_Positions_in_Data_Vector();

	void filter_current_signal();

	void recalculate_current_tab();

	int clear_all_plots_and_data();

	double Min_Synch,Max_Synch;


	Butterworth_BandPass_Filter Butterworth_BandPass_Filter_obj;

	Graphics::TBitmap *Signals_Window_PaintBox_Bitmap;
	double Signals_Amplitude_Amplify;

	double Continuous_Activity_Ratio;

	// multiscale sinusoidal recomp stuff
	std::vector <double> Number_of_Phase_Transitions;
	double Direction_Coef_NPT;

	std::vector<double> V_Wave;
	long V_Wave_Start,V_Wave_Stop;
	int V_Wave_Source; // 1 - ref, 2 - ECG

	long Peaks_Vector_Size;
	long* Peaks_Vector;


	// PATTERN TRAINS
	double MPC_Threshold;
	std::vector<double> MPCs_of_Consecutive_Peaks;
	std::vector<long> Train_Begin_ptr;
	std::vector<long> Train_End_ptr;
	std::vector<double> Train_Begin_ms;
	std::vector<double> Train_Length_ms;
	std::vector<double> Train_Mean_Corr;
	std::vector<double> Train_SD_Corr;

	long Data_Item_With_Most_Stable_AFCLs;


	// continuous activity support stuff
	std::vector <int> Silence_Vector;


	void paint_Synchrogram_R_matrix();

	bool Playing_Synch_Movie;

	void reset_and_clear_displays();

	double TimeShiftsAssymetry_Level;
	vector <double> LA_CLs;
	vector <double> RA_CLs;

    double LA_faster_Than_RA;
};

//---------------------------------------------------------------------------

extern PACKAGE TNL_Analysis_Form *NL_Analysis_Form;
//---------------------------------------------------------------------------
#endif
