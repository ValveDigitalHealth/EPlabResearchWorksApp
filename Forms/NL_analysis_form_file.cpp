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

#include "NL_analysis_form_file.h"

#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNL_Analysis_Form *NL_Analysis_Form;
//---------------------------------------------------------------------------
__fastcall TNL_Analysis_Form::TNL_Analysis_Form(TComponent* Owner)
	: TForm(Owner)
{
	Echo = true;
	Signals_Amplitude_Amplify = 100;
	Signals_Window_PaintBox_Bitmap = new Graphics::TBitmap;

	VT_FFT_Threshold_1 = 80; // Hz
	VT_FFT_Threshold_2 = 400; // Hz

	Peaks_Vector_Size = 2;
	Peaks_Vector = new long[2];
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::FormDestroy(TObject *Sender)
{
	clear_all_plots_and_data();

	delete Signals_Window_PaintBox_Bitmap;
		delete [] Peaks_Vector;
}
//---------------------------------------------------------------------------
void __fastcall TNL_Analysis_Form::FormHide(TObject *Sender)
{
	clear_all_plots_and_data();
}
//---------------------------------------------------------------------------

int TNL_Analysis_Form::clear_all_plots_and_data()
{
	Raw_Signals_Chart->Series[0]->Clear();
	Raw_Signals_Chart->Series[1]->Clear();
	Raw_Signals_Chart->Series[2]->Clear();
	Raw_Signals_Chart->Series[3]->Clear();

	Filtered_Signal_Chart->Series[0]->Clear();

	Histogram_Chart->Series[0]->Clear();
	Wavelet_Spectrum_Chart->Series[0]->Clear();
	Wavelet_Spectrum_Chart->Series[1]->Clear();
	Cycle_Lengths_Chart->Series[0]->Clear();
	Scatter_Chart->Series[0]->Clear();
	Scatter_Chart->Series[1]->Clear();
	Synchrogram_Chart->Series[0]->Clear();

	Histogram_Data.clear();

	for(int i=0;i<100;i++)
	for(int j=0;j<100;j++)
	{
		Correlation_Matrix[i][j] = 0;
//		Synchrogram_Result_Matrix[i][j] = 0;
	}

//	Synchrogram.Synchrogram_Data.clear();

	return 1;
}

//---------------------------------------------------------------------------

void TNL_Analysis_Form::update_data_list()
{
	if( Data_Items.size() > 0 )
	{

	Primary_Data_StringGrid->RowCount = Data_Items.size()+1;
	Primary_Data_StringGrid->ColCount = 2;
	Primary_Data_StringGrid->FixedRows = 1;
	Primary_Data_StringGrid->Cells[0][0] = "Name";
	Primary_Data_StringGrid->Cells[1][0] = "Points number";

	for(int i=0;i<Data_Items.size();i++)
	{
		Primary_Data_StringGrid->Cells[0][i+1] = Data_Items[i]->Name;
		Primary_Data_StringGrid->Cells[1][i+1] = Data_Items[i]->Voltage_Values.size();
	}

	Secondary_Data_StringGrid->RowCount = Data_Items.size();
	Secondary_Data_StringGrid->ColCount = 1;
	Secondary_Data_StringGrid->Cells[0][0] = "Name";

	for(int i=0;i<Data_Items.size();i++)
	{
		Secondary_Data_StringGrid->Cells[0][i] = Data_Items[i]->Name;
	}

	}

}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::FormResize(TObject *Sender)
{
	Primary_Data_StringGrid->Left = 5;
	Primary_Data_StringGrid->Top = 30;
	Primary_Data_StringGrid->Width = 200;
	Primary_Data_StringGrid->Height = 300;
	Primary_Data_StringGrid->DefaultColWidth = Primary_Data_StringGrid->Width/2-5;

	Data_Source_RadioGroup->Top = Primary_Data_StringGrid->Top +
					Primary_Data_StringGrid->Height + 10;

	Close_Button->Top = Data_Source_RadioGroup->Top +
					Data_Source_RadioGroup->Height + 10;

	Primary_Grid_Label->Top = 5;
  	Primary_Grid_Label->Left = Primary_Data_StringGrid->Left;

	Secondary_Data_StringGrid->Left = Primary_Data_StringGrid->Left+
		Primary_Data_StringGrid->Width + 10;
	Secondary_Data_StringGrid->Top = 30;
	Secondary_Data_StringGrid->Width = 100;
	Secondary_Data_StringGrid->Height = Primary_Data_StringGrid->Height;
	Secondary_Data_StringGrid->DefaultColWidth = Secondary_Data_StringGrid->Width-5;

	Secondary_Grid_Label->Top = 5;
  	Secondary_Grid_Label->Left = Secondary_Data_StringGrid->Left;

	Disp_2nd_Signal_CheckBox->Top = 5;
  	Disp_2nd_Signal_CheckBox->Left = Secondary_Data_StringGrid->Left+
            Secondary_Data_StringGrid->Width+5;    

	Raw_Signals_Chart->Left = Secondary_Data_StringGrid->Left + Secondary_Data_StringGrid->Width + 5;
	Raw_Signals_Chart->Top = Secondary_Data_StringGrid->Top;
	Raw_Signals_Chart->Width = NL_Analysis_Form->Width -
			  Primary_Data_StringGrid->Width -
			  Secondary_Data_StringGrid->Width - 30;
	Raw_Signals_Chart->Height = 0.15*NL_Analysis_Form->Height;

	Filtered_Signal_Chart->Left = Raw_Signals_Chart->Left;
	Filtered_Signal_Chart->Top = Raw_Signals_Chart->Top+Raw_Signals_Chart->Height+5;
	Filtered_Signal_Chart->Width = Raw_Signals_Chart->Width;
	Filtered_Signal_Chart->Height = Raw_Signals_Chart->Height;

	PageControl1->Left = Raw_Signals_Chart->Left;
	PageControl1->Top = Filtered_Signal_Chart->Top + Filtered_Signal_Chart->Height + 10;
	PageControl1->Width = Raw_Signals_Chart->Width + 30;
	PageControl1->Height = NL_Analysis_Form->Height - 2*Raw_Signals_Chart->Height - 50;

	Wavelet_Spectrum_Chart->Width = Raw_Signals_Chart->Width;

	P_Map_PaintBox->Height = 400;
	P_Map_PaintBox->Width = 400;

	Signals_Window_PaintBox_Bitmap->Width = Raw_Signals_Chart->Width;
	Signals_Window_PaintBox_Bitmap->Height = All_sigs_TabSheet->Height - 140;
	Signals_Display_PaintBox->Width = Signals_Window_PaintBox_Bitmap->Width;
	Signals_Display_PaintBox->Height = Signals_Window_PaintBox_Bitmap->Height;

	TimeShiftsDynamicChart->Width = PageControl1->Width - 60;
	AFCLs_Global_Chart->Left = TimeShiftsDynamicChart->Left;
	AFCLs_Global_Chart->Width = TimeShiftsDynamicChart->Width;
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::FormShow(TObject *Sender)
{
	update_data_list();
	Filter_All_Signals_ButtonClick(this);
	recalculate_current_tab();

	CAR_ButtonClick(this);

	repaint_current_tab();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Close_ButtonClick(TObject *Sender)
{
    ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Primary_Data_StringGridClick(TObject *Sender)
{
	Selected_Data_Item_1 = Primary_Data_StringGrid->Row-1;
	Selected_Data_Item_2 = Secondary_Data_StringGrid->Row;

	recalculate_current_tab();
	repaint_current_tab();
}

//---------------------------------------------------------------------------

void TNL_Analysis_Form::recalculate_current_tab()
{
	Selected_Data_Item_1 = Primary_Data_StringGrid->Row-1;
	Selected_Data_Item_2 = Secondary_Data_StringGrid->Row;

	if( selected_data_items_in_range() )
	{

	Data_Items[Selected_Data_Item_1]->Min_Ptr =
		Raw_Signals_Chart->BottomAxis->Minimum/Data_Items[Selected_Data_Item_1]->Time_Step_ms;
	Data_Items[Selected_Data_Item_1]->Max_Ptr =
		Raw_Signals_Chart->BottomAxis->Maximum/Data_Items[Selected_Data_Item_1]->Time_Step_ms;

	if(Data_Items[Selected_Data_Item_1]->Min_Ptr<0)
		Data_Items[Selected_Data_Item_1]->Min_Ptr = 0;
	if( Data_Items[Selected_Data_Item_1]->Max_Ptr <= 0 ||
		Data_Items[Selected_Data_Item_1]->Max_Ptr >= Data_Items[Selected_Data_Item_1]->Voltage_Values.size() )
		Data_Items[Selected_Data_Item_1]->Max_Ptr = Data_Items[Selected_Data_Item_1]->Voltage_Values.size() - 1;

	int begining_ptr = max( Data_Items[Selected_Data_Item_1]->Min_Ptr,
							Data_Items[Selected_Data_Item_2]->Min_Ptr );
	int end_ptr = min( Data_Items[Selected_Data_Item_1]->Max_Ptr,
					   Data_Items[Selected_Data_Item_2]->Max_Ptr );

	std::vector <double> Data_Vec1 = get_data_for_analysis(Selected_Data_Item_1);
	std::vector <double> Data_Vec2 = get_data_for_analysis(Selected_Data_Item_2);

	if( Data_Vec1.size() > 0 )
	if( Data_Vec2.size() > 0 )
	{

	// filtering tab
	if( PageControl1->ActivePage->Name == "Filtering_TabSheet" )
	{
		filter_current_signal();
	}

	// filtering tab
	if( PageControl1->ActivePage->Name == "Histogram_TabSheet" )
	{
		recalculate_histogram();
	}

	// FFT tab
	if( PageControl1->ActivePage->Name == "FFT_TabSheet" )
	{
		calculate_FFT_spectrum(&Data_Vec1,Data_Items[Selected_Data_Item_1]->Min_Ptr,
										  Data_Items[Selected_Data_Item_1]->Max_Ptr);
    }

	//----------------------------------------------------------------
	if( PageControl1->ActivePage->Name == "Correlation_TabSheet" )
	//----------------------------------------------------------------
	{

	double min1,max1,av1;
	double min2,max2,av2;
	Numerical_Library_Obj.calculate_max_min_mean_vec_double(&Data_Vec1,&min1,&max1,&av1);
	Numerical_Library_Obj.calculate_max_min_mean_vec_double(&Data_Vec2,&min2,&max2,&av2);
/*
	// compute volume of phase space
	double Coarsening = Phase_Space_Resolution_Edit->Text.ToDouble();
	long N = (max(max1,max2)-min(min1,min2))/Coarsening;  //=500;
	Matrix_2D Phase_Space;

	if( N > 5000 )
	ShowMessage("Too big phase space! Increase coarsening. Calculations interrupted.");
	else
	if( N > 3 )
	{

	Phase_Space.create_matrix(N,N);

	Numerical_Library_Obj.create_phase_space_from_2_signals(
		&Data_Vec1,&Data_Vec2,0,Data_Vec1.size(),Coarsening,Phase_Space,N);

	double Val;
	Val = Numerical_Library_Obj.get_occupied_volume_of_phase_space_percentage(Phase_Space,N);
	Volume_Label->Caption = "Perc Occupied= " + FloatToStrF(Val,ffGeneral,5,5);

	Scatter_Chart->Series[0]->Clear();
	Scatter_Chart->Series[1]->Clear();
	Scatter_Chart->BottomAxis->Title->Caption = Data_Items[Selected_Data_Item_1]->Name;
	Scatter_Chart->LeftAxis->Title->Caption = Data_Items[Selected_Data_Item_2]->Name;

	for(long n1=0;n1<N;n1++)
	for(long n2=0;n2<N;n2++)
	if( Phase_Space[n1][n2] > 0)
	Scatter_Chart->Series[0]->AddXY(Coarsening*(n1-N/2),Coarsening*(n2-N/2));

	} // phase space size ok

*/

	} // corrtab open

	//----------------------------------------------------------------
	if( PageControl1->ActivePage->Name == "Synchronization_TabSheet" )
	//----------------------------------------------------------------
	{

	if( (Raw_Signals_Chart->BottomAxis->Maximum -
		 Raw_Signals_Chart->BottomAxis->Minimum ) *Data_Items[Selected_Data_Item_1]->Time_Step_ms
		 < 120000 )
	{

	Synchrogram_Chart->Series[0]->Clear();
	for(long k=0;k<Data_Vec1.size();k++)
	Synchrogram_Chart->Series[0]->AddXY(Data_Vec1[k],Data_Vec2[k]);

	Sych_Results_RichEdit->Lines[0].Clear();
	Synchrogram_Chart->BottomAxis->Title->Caption = "Signal A";
	Synchrogram_Chart->LeftAxis->Title->Caption = "Signal B";
	Sych_Results_RichEdit->Lines[0].Add("Synchronization= "+FloatToStrF(
			Numerical_Library_Obj.get_phase_synchronization(
			&Data_Vec1,&Data_Vec2,1,1,true,0),ffGeneral,3,2));

	}
	else
	ShowMessage("Visualization stopped since too long signal was taken (>2 min)");

	} // synch tab open


	} // if data vectors not empty

    } // sets in range
}
//---------------------------------------------------------------------------

void TNL_Analysis_Form::repaint_current_tab()
{
	double Av,Cnt,SD;

    //gogh

	if( selected_data_items_in_range() )
	if( Data_Items[Selected_Data_Item_1]->Voltage_Values.size() > 0 )
	{

	Raw_Signals_Chart->Series[0]->Clear();
	Raw_Signals_Chart->Series[0]->Pen->Width = 1;
	Raw_Signals_Chart->Series[1]->Clear();
	Raw_Signals_Chart->Series[2]->Clear();
	Raw_Signals_Chart->Series[3]->Clear();
	Raw_Signals_Chart->Series[4]->Clear();
	Raw_Signals_Chart->Series[5]->Clear();

	int set2 = Secondary_Data_StringGrid->Row;

	// replot raw signals
	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Voltage_Values.size();i++)
	Raw_Signals_Chart->Series[0]->AddXY(
		i*Data_Items[Selected_Data_Item_1]->Time_Step_ms,
		Data_Items[Selected_Data_Item_1]->Voltage_Values[i] );

	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.size();i++)
	Raw_Signals_Chart->Series[1]->AddXY(
		Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal[i]*
		Data_Items[Selected_Data_Item_1]->Time_Step_ms,0 );

	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.size();i++)
	Raw_Signals_Chart->Series[2]->AddXY(
		Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal[i]*
		Data_Items[Selected_Data_Item_1]->Time_Step_ms,0 );

	// second signal if checkbox checked
	if( Disp_2nd_Signal_CheckBox->State == cbChecked )
	{
		for(long i=0;i<Data_Items[Selected_Data_Item_2]->Voltage_Values.size();i++)
			Raw_Signals_Chart->Series[2]->AddXY(
					i*Data_Items[Selected_Data_Item_2]->Time_Step_ms,Data_Items[set2]->Voltage_Values[i] );

		for(long i=0;i<Data_Items[set2]->Peaks_Positions_In_Signal.size();i++)
			Raw_Signals_Chart->Series[3]->AddXY(
				Data_Items[Selected_Data_Item_2]->Peaks_Positions_In_Signal[i]*
				Data_Items[Selected_Data_Item_2]->Time_Step_ms,0 );
	}

	// CAFE peaks
	for(long i=0;i<Data_Items[Selected_Data_Item_1]->NavX_CAFE_Positions.size();i++)
	Raw_Signals_Chart->Series[4]->AddXY(
		Data_Items[Selected_Data_Item_1]->NavX_CAFE_Positions[i]*
		Data_Items[Selected_Data_Item_1]->Time_Step_ms,0 );

	// Custom peaks
	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.size();i++)
	Raw_Signals_Chart->Series[5]->AddXY(
		Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal[i]*
		Data_Items[Selected_Data_Item_1]->Time_Step_ms,0 );

	// filtered singal
	Filtered_Signal_Chart->Series[0]->Clear();
	Filtered_Signal_Chart->Series[1]->Clear();
	for(long i=Data_Items[Selected_Data_Item_1]->Min_Ptr;
		i<Data_Items[Selected_Data_Item_1]->Max_Ptr;i++)
	if( i>=0 )
	if( i<Data_Items[Selected_Data_Item_1]->Filtered_Signal.size() )
	Filtered_Signal_Chart->Series[0]->AddXY(
		i*Data_Items[Selected_Data_Item_1]->Time_Step_ms,
		Data_Items[Selected_Data_Item_1]->Filtered_Signal[i] );

	Filtered_Signal_Chart->Series[2]->Clear();
	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.size();i++)
	Filtered_Signal_Chart->Series[2]->AddXY(
		Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal[i]*
		Data_Items[Selected_Data_Item_1]->Time_Step_ms,0 );

	// second signal if checkbox checked
	if( Disp_2nd_Signal_CheckBox->State == cbChecked )
	{
	for(long i=Data_Items[Selected_Data_Item_2]->Min_Ptr;
		i<Data_Items[Selected_Data_Item_2]->Max_Ptr;i++)
    if( i>=0 )
	if( i<Data_Items[Selected_Data_Item_2]->Filtered_Signal.size() )
	Filtered_Signal_Chart->Series[1]->AddXY(
		i*Data_Items[Selected_Data_Item_2]->Time_Step_ms,
		Data_Items[Selected_Data_Item_2]->Filtered_Signal[i] );
    }

	// peak detection
	if( PageControl1->ActivePage->Name == "Preak_Detection_TabSheet" )
	{
	CLs_RichEdit->Lines[0].Clear();
	CLs_RichEdit->Lines[0].Add("CL mean= "+FloatToStrF(
		Data_Items[Selected_Data_Item_1]->Average_ISI,ffGeneral,3,2 ));
	CLs_RichEdit->Lines[0].Add("CL SD= "+FloatToStrF(
        Data_Items[Selected_Data_Item_1]->SD_of_ISI,ffGeneral,3,2 ));
    }

	// poincare map
	if( PageControl1->ActivePage->Name == "Poincare_TabSheet" )
	{
		P_Map_PaintBoxPaint(this);
	}

	// correlations
	if( PageControl1->ActivePage->Name == "Correlation_TabSheet" )
	{
		paint_R_matrix();
	}

	// FFT tab
	if( PageControl1->ActivePage->Name == "FFT_TabSheet" )
	{
        paint_FFT_spectrum();
    }

	// histograms
	if( PageControl1->ActivePage->Name == "Histogram_TabSheet" )
	{
		// plot histogram
		Histogram_Chart->Series[0]->Clear();
		for(long k=0;k<Histogram_Data.size();k++)
		Histogram_Chart->Series[0]->AddXY(Histo_Min+k*Histo_Bin_Size,Histogram_Data[k] );
	}

	// wavelet spectrum
	if( PageControl1->ActivePage->Name == "Wavelet_TabSheet" )
	{
		// plot CWT line
		Wavelet_Spectrum_Chart->Series[0]->Clear();
		for(long k=1;k<Data_Items[Selected_Data_Item_1]->Wavelet_Spectrum.size();k++)
		Wavelet_Spectrum_Chart->Series[0]->AddXY(
			k*Data_Items[Selected_Data_Item_1]->Time_Step_ms,
			Data_Items[Selected_Data_Item_1]->Wavelet_Spectrum[k],"",clRed);

		// plot markers
		Wavelet_Spectrum_Chart->Series[1]->Clear();
		for(long k=0;k<Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.size();k++)
		Wavelet_Spectrum_Chart->Series[1]->AddXY(
			Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal[k]*
			Data_Items[Selected_Data_Item_1]->Time_Step_ms,0);

		fill_wavelet_CL_plots();
	}

	// synchrogram
	if( PageControl1->ActivePage->Name == "Synchronization_TabSheet" )
	{
		paint_Synchrogram_R_matrix();
	}

	// synchrogram
	if( PageControl1->ActivePage->Name == "All_sigs_TabSheet" )
	{
		Signals_Display_PaintBoxPaint(this);
	}

/*
	Av=0; Cnt=0;
	for(long i=Data_Items[Selected_Data_Item]->Min_Ptr;
		i<Data_Items[Selected_Data_Item]->Max_Ptr;i++)
	{
		Av+= Data_Items[Selected_Data_Item]->Voltage_Values[i];
		Cnt++;
	}

	if(Av!=0) Av /= Cnt;

	SD=0;
	for(long i=Data_Items[Selected_Data_Item]->Min_Ptr;
		i<Data_Items[Selected_Data_Item]->Max_Ptr;i++)
	SD += std::pow(Data_Items[Selected_Data_Item]->Voltage_Values[i]-Av,2);

	if(Cnt!=0)
	SD = sqrt(SD/Cnt);

	Stats_Label->Caption = "Average/SD:  "+ FloatToStrF(Av,ffGeneral,3,2 )+ " / "+FloatToStrF(SD,ffGeneral,3,2 );

	L_Label->Caption = "Length [units]:  "+ IntToStr( (int)(
			Data_Items[Selected_Data_Item]->Data_X[Data_Items[Selected_Data_Item]->Max_Ptr]-
			Data_Items[Selected_Data_Item]->Data_X[Data_Items[Selected_Data_Item]->Min_Ptr] ));

*/

	} // if selected item in range
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Close1Click(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void TNL_Analysis_Form::fill_wavelet_CL_plots()
{
	long k;

	if( selected_data_items_in_range() )
	if( Data_Items[Selected_Data_Item_1]->Voltage_Values.size() > 1 )
	{

	//-----------------------------------------------------------------
	// CLs chart
	//-----------------------------------------------------------------
	Cycle_Lengths_Chart->Series[0]->Clear();

	if(CLs_Labels_Display_CheckBox->State == cbChecked )
	Cycle_Lengths_Chart->Series[0]->Marks->Visible = true;
	else
	Cycle_Lengths_Chart->Series[0]->Marks->Visible = false;

	for(k=0;k<Data_Items[Selected_Data_Item_1]->ISIs.size();k++)
	{
		if( IndexORTime_RadioGroup->ItemIndex == 0 )
		Cycle_Lengths_Chart->Series[0]->AddXY(k,
				Data_Items[Selected_Data_Item_1]->ISIs[k],"",clGreen);

		if( IndexORTime_RadioGroup->ItemIndex == 1 )
		Cycle_Lengths_Chart->Series[0]->AddXY(
			Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal[k],
			Data_Items[Selected_Data_Item_1]->ISIs[k],"",clGreen);
	}

	//-------------------------------------
	// calculate max in ISI histogram
	//-------------------------------------
	double max = 0.0;
	long max_ptr;
	for(long k=0;k<Data_Items[Selected_Data_Item_1]->ISI_Histogram.size();k++)
	if(Data_Items[Selected_Data_Item_1]->ISI_Histogram[k] >= max )
	{
		max = Data_Items[Selected_Data_Item_1]->ISI_Histogram[k];
		max_ptr = k;
	}

	Data_Items[Selected_Data_Item_1]->DF_From_Max_Freq_ISI =
		1000.0/(Data_Items[Selected_Data_Item_1]->Min_ISI+(max_ptr+0.5)*
        Data_Items[Selected_Data_Item_1]->ISI_Histogram_Box);

	//-----------------------------------------------------------------
	// labels
	//-----------------------------------------------------------------
	ISI_RichEdit->Lines[0].Clear();
	ISI_RichEdit->Lines[0].Add("Min ISIs[ms]= "+AnsiString::FloatToStrF(Data_Items[Selected_Data_Item_1]->Min_ISI,AnsiString::sffGeneral,3,2));
	ISI_RichEdit->Lines[0].Add("Max ISIs[ms]= "+AnsiString::FloatToStrF(Data_Items[Selected_Data_Item_1]->Max_ISI,AnsiString::sffGeneral,3,2));
	ISI_RichEdit->Lines[0].Add("Mean ISIs[ms]= "+AnsiString::FloatToStrF(Data_Items[Selected_Data_Item_1]->Average_ISI,AnsiString::sffGeneral,3,2));
	ISI_RichEdit->Lines[0].Add("SD of ISIs[ms]= "+AnsiString::FloatToStrF(Data_Items[Selected_Data_Item_1]->SD_of_ISI,AnsiString::sffGeneral,3,2));
	ISI_RichEdit->Lines[0].Add("DF from ISI[Hz]= "+AnsiString::FloatToStrF(Data_Items[Selected_Data_Item_1]->DF_From_ISI,AnsiString::sffGeneral,3,2));
	ISI_RichEdit->Lines[0].Add("DF from most freq. ISI[Hz]= "+AnsiString::FloatToStrF(Data_Items[Selected_Data_Item_1]->DF_From_Max_Freq_ISI,AnsiString::sffGeneral,3,2));

/*
	DWT_filtered_Chart->Title->Text->Clear();
	DWT_filtered_Chart->Title->Text->Add(
	 "DF from mean ISI= "+ FloatToStrF(Data_Items[Selected_Data_Item]->DF_From_ISI,AnsiString::sffGeneral,3,2)+
	 "    DF from most freq. ISI= "+ FloatToStrF(Data_Items[Selected_Data_Item]->DF_From_Max_Freq_ISI,AnsiString::sffGeneral,3,2)+
	 "    Min ISI= "+ FloatToStrF(Data_Items[Selected_Data_Item]->Min_ISI,AnsiString::sffGeneral,3,2)+
	 "    Max ISI= " + FloatToStrF(Data_Items[Selected_Data_Item]->Max_ISI,AnsiString::sffGeneral,3,2)+
	 "    Mean ISI= " + FloatToStrF(Data_Items[Selected_Data_Item]->Average_ISI,AnsiString::sffGeneral,3,2));
*/
	} // if in range

}

//---------------------------------------------------------------------------
void __fastcall TNL_Analysis_Form::Secondary_Data_StringGridClick(TObject *Sender)
{
    Selected_Data_Item_1 = Primary_Data_StringGrid->Row-1;
    Selected_Data_Item_2 = Secondary_Data_StringGrid->Row;

	recalculate_current_tab();
    repaint_current_tab();
}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Histo_Recalculate_ButtonClick(
	  TObject *Sender)
{
	recalculate_histogram();
	repaint_current_tab();
}
//---------------------------------------------------------------------------

int TNL_Analysis_Form::recalculate_histogram()
{
	Histo_Bin_Size = Bin_Size_Edit->Text.ToDouble();
	double mean;

	Histogram_Chart->Title->Text->Clear();
	Histogram_Chart->Title->Text->Add("Histogram");

	std::vector <double> Data_Vec;
	Data_Vec.clear();
	Data_Vec = get_data_for_analysis(Selected_Data_Item_1);

	if( Data_Vec.size() > 3 )
	{

	Numerical_Library_Obj.calculate_max_min_mean_vec_double(&Data_Vec,
				&Histo_Min, &Histo_Max, &mean);
	Histogram_Data.clear();
	Histogram_Data = Numerical_Library_Obj.
		get_histogram(&Data_Vec,Histo_Bin_Size,ShSD_CheckBox->State,&Histo_Bin_Size);

	// if requested, remove outliers from the histogram
	std::vector <double> Data_Vec_without_outliers;
	if( Hist_Outliers_Remove_CheckBox->State == cbChecked )
	{
		double p5 =  Numerical_Library_Obj.get_percentile_from_histogram(
		 &Histogram_Data,5,Histo_Min,Histo_Bin_Size);
		double p95 =  Numerical_Library_Obj.get_percentile_from_histogram(
		 &Histogram_Data,95,Histo_Min,Histo_Bin_Size);

		 for(int i=0;i<Data_Vec.size();i++)
		 {
			if( Data_Vec[i] > p5 &&
				Data_Vec[i] < p95 )
			Data_Vec_without_outliers.push_back(Data_Vec[i]);
		 }

		Numerical_Library_Obj.calculate_max_min_mean_vec_double(&Data_Vec_without_outliers,
				&Histo_Min, &Histo_Max, &mean);
		Histogram_Data.clear();
		Histogram_Data = Numerical_Library_Obj.get_histogram(&Data_Vec_without_outliers,
			Histo_Bin_Size,ShSD_CheckBox->State,&Histo_Bin_Size);
	}

	// Shen analysis
	Histogram_Shen = Numerical_Library_Obj.get_Shannon_Entropy(&Data_Vec,
				ShSD_CheckBox->Checked,Histo_Bin_Size );
	Histogram_Chart->Title->Text->Add("Shannon entropy= "+FloatToStr(Histogram_Shen));

	// cacluate percentiles
	double val;
	Hist_Tab_RichEdit->Lines[0].Clear();
	for(int Perc=5;Perc<=95;Perc+=5)
	{
		val = Numerical_Library_Obj.get_percentile_from_histogram(
		 &Histogram_Data,Perc,Histo_Min,Histo_Bin_Size);

		if( Perc == 5 )
			Percentile_5 = val;
		Hist_Tab_RichEdit->Lines[0].Add(IntToStr(Perc)+" percentile = " + FloatToStrF(val,ffGeneral,5,5) );
	}

	} // if data std::vector not empty
}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::PageControl1Change(TObject *Sender)
{
	recalculate_current_tab();
	repaint_current_tab();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Exportcurrentdataserie1Click(
	  TObject *Sender)
{
	ofstream df;

	if( selected_data_items_in_range() )
	if(SaveDialog1->Execute())
	{

	df.open(SaveDialog1->FileName.c_str());

	df << Data_Items[Selected_Data_Item_1]->Max_Ptr - Data_Items[Selected_Data_Item_1]->Min_Ptr << "\n";

	for(long i=Data_Items[Selected_Data_Item_1]->Min_Ptr;
			i<Data_Items[Selected_Data_Item_1]->Max_Ptr;i++)
	{
		df << Data_Items[Selected_Data_Item_1]->Voltage_Values[i] << " ";
		df << "\n";
	}

	df.close();

	}

}
//---------------------------------------------------------------------------


void __fastcall TNL_Analysis_Form::Update_CORR_Table_ButtonClick(TObject *Sender)
{
	// set parameters of phase space
	double min1,max1,av1,SD;
	double min2,max2,av2;
	double Coarsening = Phase_Space_Resolution_Edit->Text.ToDouble();
	Matrix_2D Phase_Space;
	long N;

	Correlation_Matrix_Size = Data_Items.size();
	for(long i=0;i<Correlation_Matrix_Size;i++)
	for(long j=0;j<Correlation_Matrix_Size;j++)
		Correlation_Matrix[i][j]=0;

	for(int DI1=0;DI1<Data_Items.size();DI1++)
	for(int DI2=0;DI2<DI1;DI2++)
	{

	std::vector <double> Data_Vec1 = get_data_for_analysis(DI1);
	std::vector <double> Data_Vec2 = get_data_for_analysis(DI2);

	Numerical_Library_Obj.calculate_max_min_mean_vec_ranged(&Data_Vec1,0,Data_Vec1.size(),&min1,&max1,&av1,&SD);
	Numerical_Library_Obj.calculate_max_min_mean_vec_ranged(&Data_Vec2,0,Data_Vec1.size(),&min2,&max2,&av2,&SD);
/*
	// compute volume of phase space
	N = (max(max1,max2)-min(min1,min2))/Coarsening;  //=500;
	Phase_Space = Numerical_Library_Obj.matrix_longs(N,N);

	if( N > 5000 )
	ShowMessage("Too big phase space! Increase coarsening. Calculations interrupted.");
	else
	if( N > 3 )
	{

	Numerical_Library_Obj.create_phase_space_from_2_signals(
		&Data_Vec1,&Data_Vec2,0,Data_Vec1.size(),Coarsening,Phase_Space,N);

	Correlation_Matrix[DI1][DI2] =
			Numerical_Library_Obj.get_occupied_volume_of_phase_space_percentage(
				Phase_Space,N);

	} // phase space size ok
*/

	} // through all pairs of signals

	double min=MAXDOUBLE, max=-MAXDOUBLE;
	for(int DI1=0;DI1<Correlation_Matrix_Size;DI1++)
	for(int DI2=0;DI2<DI1;DI2++)
	{
		Correlation_Matrix[DI2][DI1]=Correlation_Matrix[DI1][DI2];

		if( DI1 != DI2 )
		{
		if(  Correlation_Matrix[DI1][DI2] > max ) max = Correlation_Matrix[DI1][DI2];
		if(  Correlation_Matrix[DI1][DI2] < min ) min = Correlation_Matrix[DI1][DI2];
		}
	}
/*
	ofstream df("Corr_Table.csv");
	for(long i=0;i<Correlation_Matrix_Size;i++)
	{
	for(long j=0;j<Correlation_Matrix_Size;j++)
		df << Correlation_Matrix[i][j] << ",";
	df << endl;
	}
	df.close();
*/
	Range_Label->Caption = "Values spread= "+FloatToStrF(max-min,ffGeneral,3,2 );
	repaint_current_tab();
}
//---------------------------------------------------------------------------

void TNL_Analysis_Form::paint_R_matrix()
{
	if( Data_Items.size() > 0 )
	if( Correlation_Matrix_Size > 0 )
	if( Correlation_Matrix_Size == Data_Items.size() )
	{

	  // draw result matrix
	  double wx,c,max=0,min=1000000000;
	  for(int  i=0; i<Correlation_Matrix_Size; i++ )
	  for(int  j=0; j<Correlation_Matrix_Size; j++ )
	  if( Correlation_Matrix[i][j] != 0 )
	  {
		  if( max < Correlation_Matrix[i][j] ) max = Correlation_Matrix[i][j];
		  if( min > Correlation_Matrix[i][j] ) min = Correlation_Matrix[i][j];
	  }

	  if (min==max) { min=0; max=1; }

	  wx = (double)(CorrTable_PaintBox->Width) / (double)(Data_Items.size());
	  CorrTable_PaintBox->Canvas->Brush->Color=clBlack;
	  CorrTable_PaintBox->Canvas->FillRect( Rect( 0, 0, CorrTable_PaintBox->Width,
		CorrTable_PaintBox->Height));

	  for(int  i=0; i<Correlation_Matrix_Size; i++ )
	  for(int  j=0; j<Correlation_Matrix_Size; j++ )
	  {

	  if( max-min != 0 )
	  c = 255-255*(Correlation_Matrix[i][j]-min)/(max-min);

	  if( c > 255 ) c = 255;
	  if( c< 0 ) c = 0;

	  CorrTable_PaintBox->Canvas->Brush->Color=(TColor)( RGB(c,c,c) );

	  CorrTable_PaintBox->Canvas->FillRect( Rect( i*wx, j*wx, (i+1)*wx, (j+1)*wx ));

	  } // for

	int set1 = Primary_Data_StringGrid->Row-1;
	int set2 = Secondary_Data_StringGrid->Row;

	int point_size=4;
	CorrTable_PaintBox->Canvas->Pen->Color=clRed;
	CorrTable_PaintBox->Canvas->FillRect( Rect(
		(set1+0.5)*wx-point_size, (set2+0.5)*wx-point_size,
		(set1+0.5)*wx+point_size, (set2+0.5)*wx+point_size ));

	}
}

//---------------------------------------------------------------------------

void TNL_Analysis_Form::paint_Synchrogram_R_matrix()
{
/*
	if( Data_Items.size() > 0 )
	if( Synchrogram_Result_Matrix_Size > 0 )
	if( Synchrogram_Result_Matrix_Size == Data_Items.size() )
	{

	  // draw result matrix
	  double wx,c,max=0,min=1000000000;
	  for(int  i=0; i<Synchrogram_Result_Matrix_Size; i++ )
	  for(int  j=0; j<Synchrogram_Result_Matrix_Size; j++ )
	  if (Synchrogram_Result_Matrix[i][j] != 0 )
	  {
		  if( max < Synchrogram_Result_Matrix[i][j] ) max = Synchrogram_Result_Matrix[i][j];
		  if( min > Synchrogram_Result_Matrix[i][j] ) min = Synchrogram_Result_Matrix[i][j];
	  }

	  if (min==max) { min=0; max=1; }

	  wx = (double)(Synchronization_Matrix_PaintBox->Width) / (double)(Data_Items.size());
	  Synchronization_Matrix_PaintBox->Canvas->Brush->Color=clBlack;
	  Synchronization_Matrix_PaintBox->Canvas->FillRect( Rect( 0, 0,
		Synchronization_Matrix_PaintBox->Width,
		Synchronization_Matrix_PaintBox->Height));

	  for(int  i=0; i<Synchrogram_Result_Matrix_Size; i++ )
	  for(int  j=0; j<Synchrogram_Result_Matrix_Size; j++ )
	  {

	  if( max-min != 0 )
	  c = 255*(Synchrogram_Result_Matrix[i][j]-min)/(max-min);

	  if( c > 255 ) c = 255;
	  if( c< 0 ) c = 0;

	  Synchronization_Matrix_PaintBox->Canvas->Brush->Color=(TColor)( RGB(c,c,c) );

	  Synchronization_Matrix_PaintBox->Canvas->FillRect( Rect( i*wx, j*wx, (i+1)*wx, (j+1)*wx ));

	  } // for

	int set1 = Primary_Data_StringGrid->Row-1;
	int set2 = Secondary_Data_StringGrid->Row;

	int point_size=4;
	Synchronization_Matrix_PaintBox->Canvas->Pen->Color=clRed;
	Synchronization_Matrix_PaintBox->Canvas->FillRect( Rect(
		(set1+0.5)*wx-point_size, (set2+0.5)*wx-point_size,
		(set1+0.5)*wx+point_size, (set2+0.5)*wx+point_size ));

	}
*/
}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Calculate_Wavelet_Spectrum_ButtonClick(TObject *Sender)
{
	//!!!!!!!!!!!!!!!!!
	PDTW_Edit->Text = 2000;

	recalculate_wavelet_spectrum_page(Selected_Data_Item_1);
	repaint_current_tab();
}

//---------------------------------------------------------------------------

void TNL_Analysis_Form::recalculate_wavelet_spectrum_page(int Selected_Data_Item)
{
	long k;
	double Start,Stop;

	if( selected_data_items_in_range() )
	{

	if( Data_Items[Selected_Data_Item]->Time_Step_ms == 0 )
	ShowMessage("Time step not set!");
	else
	{

	Start = Data_Items[Selected_Data_Item]->Min_Ptr;
	Stop = Data_Items[Selected_Data_Item]->Max_Ptr;

	std::vector <double> *Data;
	if( WS_RadioGroup->ItemIndex == 0 )
	Data = &Data_Items[Selected_Data_Item]->Voltage_Values;
	if( WS_RadioGroup->ItemIndex == 1 )
	Data = &Data_Items[Selected_Data_Item]->Filtered_Signal;

	double Scaling_Factor_Min = Wavelet_Scaling_Min_Edit->Text.ToDouble();
	double Scaling_Factor_Max = Wavelet_Scaling_Max_Edit->Text.ToDouble();

	if( Start < 0 || Start >= Data_Items[Selected_Data_Item]->Voltage_Values.size() )
		Start = 0;
	if( Stop <= 0 || Stop >= Data_Items[Selected_Data_Item]->Voltage_Values.size() )
		Stop = Data_Items[Selected_Data_Item]->Voltage_Values.size() - 1;

	if( Range_Wavelet_Scaling_CheckBox->State == cbUnchecked )
	Data_Items[Selected_Data_Item]->Wavelet_Spectrum =
	Numerical_Library_Obj.make_Continious_Wavelet_Transformation(
		Data,Start,Stop,Scaling_Factor_Min);
	else
	{
	double tmp,Step = Wavelet_Step_Edit->Text.ToDouble();
	int Number = (Scaling_Factor_Max-Scaling_Factor_Min)/Step;

	std::vector <Values_Vector> Wavelet_Spectras;
	Values_Vector VC;

	Wavelet_Spectras.assign(Number,VC);

	for(int k=0;k<Number;k++)
	Wavelet_Spectras[k].Values =
	Numerical_Library_Obj.make_Continious_Wavelet_Transformation(
		Data,Start,Stop,Scaling_Factor_Min+k*Step);

	Data_Items[Selected_Data_Item]->Wavelet_Spectrum.clear();
	tmp=0;
	Data_Items[Selected_Data_Item]->Wavelet_Spectrum.assign(
		Wavelet_Spectras[0].Values.size(),tmp);

	if( Wavelet_integrated_spect_RadioGroup->ItemIndex == 0 ) // mean
	{
	for(int timepoint=0;timepoint<Wavelet_Spectras[0].Values.size();timepoint++)
	for(int spectra_no=0;spectra_no<Number;spectra_no++)
	Data_Items[Selected_Data_Item]->Wavelet_Spectrum[timepoint] +=
		Wavelet_Spectras[spectra_no].Values[timepoint];

	for(int timepoint=0;timepoint<Wavelet_Spectras[0].Values.size();timepoint++)
	Data_Items[Selected_Data_Item]->Wavelet_Spectrum[timepoint] /= (double)Number;
	}

	if( Wavelet_integrated_spect_RadioGroup->ItemIndex == 1 ) // max
	{
		for(int timepoint=0;timepoint<Wavelet_Spectras[0].Values.size();timepoint++)
		{
			double max=0;
			for(int spectra_no=0;spectra_no<Number;spectra_no++)
			if( Wavelet_Spectras[spectra_no].Values[timepoint] > max )
			max = Wavelet_Spectras[spectra_no].Values[timepoint];

			Data_Items[Selected_Data_Item]->Wavelet_Spectrum[timepoint] = max;
		}

	}

	} // calculating wavelet spectrum

	// calculate peaks positions
	Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal.clear();

	Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal =
		Numerical_Library_Obj.get_peak_positions(
			&Data_Items[Selected_Data_Item]->Wavelet_Spectrum,
			PDT_Edit->Text.ToDouble(),
			PDTW_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item]->Time_Step_ms,
			PD_Blanking_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item]->Time_Step_ms);

	// Set peak positions in respect to beginning of signal, not
	// beginning of selected fragment of signal
	for(k=0;k<(signed)Data_Items[Selected_Data_Item]->
			Peaks_Positions_In_Signal.size();k++)
	Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal[k]+=Start;

	// caluclate ISI stuff
	calculate_ISIs_from_wavelet_peaks(Selected_Data_Item);

	}
	}
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Calculate_Wavelet_For_All_Sigs_ButtonClick(TObject *Sender)
{
	for(int i=0;i<Data_Items.size();i++)
	{
		recalculate_wavelet_spectrum_page(i);
		repaint_current_tab();
	}
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Raw_Signals_ChartClickSeries(TCustomChart *Sender,
		  TChartSeries *Series, int ValueIndex, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	Dragged_Series = Series;
	Dragged_Value_Index = ValueIndex;

	if( Button == mbLeft )
	Marker_Dragging_Flag = true;

	if( Button == mbRight )
	{

	DWT_Chart_Add_Remove_PopupMenu->PopupPoint.x = Raw_Signals_Chart->Left+X+10;
	DWT_Chart_Add_Remove_PopupMenu->PopupPoint.y = Raw_Signals_Chart->Top+Y+10;
	DWT_Chart_Add_Remove_PopupMenu->Popup(DWT_Chart_Add_Remove_PopupMenu->PopupPoint.x,
										  DWT_Chart_Add_Remove_PopupMenu->PopupPoint.y);
	}
}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Raw_Signals_ChartMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	Mouse_Button_Down = 1;
	WHICH_BUTTON = Button;

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Raw_Signals_ChartMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
	if( Marker_Dragging_Flag == true && Dragged_Series == Raw_Signals_Chart->Series[1] )
	// To prevent from sliding out of value range
	if( Dragged_Series->XScreenToValue(X) < Raw_Signals_Chart->Series[0]->MaxXValue() )
	if( Dragged_Series->XScreenToValue(X) > Raw_Signals_Chart->Series[0]->MinXValue() )
	// To prevent from moving behind next marker
    if( Dragged_Value_Index == Dragged_Series->Count()-1 ||
        Dragged_Series->XScreenToValue(X) < Dragged_Series->XValue[Dragged_Value_Index+1] )
    // To prevent from moving before previous marker
    if( Dragged_Value_Index ==  0 ||
        Dragged_Series->XScreenToValue(X) > Dragged_Series->XValue[Dragged_Value_Index-1] )
	{

    Dragged_Series->XValue[Dragged_Value_Index] = Dragged_Series->XScreenToValue(X);
    update_Peak_Positions_in_Data_Vector();
//    ISI_CWT_TabSheetShow(this);

    }

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Raw_Signals_ChartMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	Mouse_Button_Down = 0;
	Marker_Dragging_Flag = false;

	Filtered_Signal_Chart->BottomAxis->SetMinMax(Raw_Signals_Chart->BottomAxis->Minimum,
												 Raw_Signals_Chart->BottomAxis->Maximum );

	recalculate_current_tab();
	repaint_current_tab();

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Addmarker1Click(TObject *Sender)
{

    Raw_Signals_ChartDblClick(this);
/*
    if( Dragged_Series->XValue[Dragged_Value_Index]+30 < Raw_Signals_Chart->Series[0]->MaxXValue() )
    {
        Dragged_Series->AddXY(Dragged_Series->XValue[Dragged_Value_Index]+30,0.0 );
        update_Peak_Positions_in_Data_Vector();
//        ISI_CWT_TabSheetShow(this);
//        Peak_Positions_Calcuated = true;
    }
	else
	ShowMessage("Not enough signal left to put a new marker.");
*/
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Removemarker1Click(TObject *Sender)
{
	if( Dragged_Series->Count() > 2 ) // To prevent from removing all markers
	{
		Dragged_Series->Delete(Dragged_Value_Index);
		update_Peak_Positions_in_Data_Vector();
//        ISI_CWT_TabSheetShow(this);
//        Peak_Positions_Calcuated = true;

	}

}
//---------------------------------------------------------------------------

void TNL_Analysis_Form::update_Peak_Positions_in_Data_Vector()
{
	if( selected_data_items_in_range() )
	{

	long t=0;
	long N = Raw_Signals_Chart->Series[1]->Count();
	Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.clear();
	Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.assign(N,t);

	for(long k=0;k<(signed)Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.size();k++)
	Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal[k] =
		Raw_Signals_Chart->Series[1]->XValue[k];

	calculate_ISIs_from_wavelet_peaks(Selected_Data_Item_1);

	fill_wavelet_CL_plots();

	}
}

//------------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::PM_recalculate_ButtonClick(TObject *Sender)
{
	repaint_current_tab();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Data_Source_RadioGroupClick(TObject *Sender)
{
	if( Echo )
	{
		recalculate_current_tab();
		repaint_current_tab();
	}
}
//---------------------------------------------------------------------------


void __fastcall TNL_Analysis_Form::ImportcurrentdataserieYvalue1Click(TObject *Sender)

{
	ifstream df;
	double v;
	long counter=0;

	if(OpenDialog1->Execute())
	{

	df.open(OpenDialog1->FileName.c_str());

	Electrogram DI;

	long Length;
	df >> Length;

	for(long i=0;i<Length;i++)
	{
		df >> v; DI.Voltage_Values.push_back(v);
	}

	DI.Name = UT.get_file_name_from_full_path(OpenDialog1->FileName);
	DI.Time_Step_ms = 1;
	Data_Items.push_back(&DI);
	df.close();

	}

	update_data_list();
	repaint_current_tab();
}
//---------------------------------------------------------------------------

bool TNL_Analysis_Form::selected_data_items_in_range()
{
	if( Selected_Data_Item_1 >= 0 && Selected_Data_Item_1 < Data_Items.size() &&
		Selected_Data_Item_2 >= 0 && Selected_Data_Item_2 < Data_Items.size() )
	if( Data_Items[Selected_Data_Item_1]->Voltage_Values.size() > 0 &&
		Data_Items[Selected_Data_Item_2]->Voltage_Values.size() > 0 )
	return true;
	else
	return false;
}
//---------------------------------------------------------------------------

void TNL_Analysis_Form::calculate_ISIs_from_wavelet_peaks(int Selected_Data_Item)
{
    if( Selected_Data_Item>=0 && Selected_Data_Item<Data_Items.size() )
	if( Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal.size() > 1 )
	{

	// create ISI table
	Data_Items[Selected_Data_Item]->ISIs.clear();

	for(long k=1;k<(signed)Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal.size();k++)
	if( Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal[k]-
			   Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal[k-1] > 0 )
		Data_Items[Selected_Data_Item]->ISIs.push_back(
			  (Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal[k]-
			   Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal[k-1])*
				Data_Items[Selected_Data_Item]->Time_Step_ms);
	else
	int sdfs=34;


	// calculate mean and SD
	Numerical_Library_Obj.stdev_cor_vec(&Data_Items[Selected_Data_Item]->ISIs,
			  &Data_Items[Selected_Data_Item]->Average_ISI,
			  &Data_Items[Selected_Data_Item]->SD_of_ISI);

	if(Data_Items[Selected_Data_Item]->Average_ISI!=0)
		Data_Items[Selected_Data_Item]->DF_From_ISI = 1000.0/
			Data_Items[Selected_Data_Item]->Average_ISI;
	else
		Data_Items[Selected_Data_Item]->DF_From_ISI = 0.0;

	// find minmax
	Data_Items[Selected_Data_Item]->Min_ISI = 100000.0;
	Data_Items[Selected_Data_Item]->Max_ISI = -1000000.0;
	for(long k=0;k<Data_Items[Selected_Data_Item]->ISIs.size();k++)
	{
		if( Data_Items[Selected_Data_Item]->Max_ISI < Data_Items[Selected_Data_Item]->ISIs[k] )
			Data_Items[Selected_Data_Item]->Max_ISI = Data_Items[Selected_Data_Item]->ISIs[k];
		if( Data_Items[Selected_Data_Item]->Min_ISI > Data_Items[Selected_Data_Item]->ISIs[k] )
			Data_Items[Selected_Data_Item]->Min_ISI = Data_Items[Selected_Data_Item]->ISIs[k];
	}

	if( Data_Items[Selected_Data_Item]->Min_ISI == Data_Items[Selected_Data_Item]->Max_ISI )
	Data_Items[Selected_Data_Item]->Max_ISI += 1.0;

	//-------------------------------------
	// calculate ISI histogram
	//-------------------------------------

	Data_Items[Selected_Data_Item]->ISI_Histogram.clear();
	long ISI_Histogram_Size = (Data_Items[Selected_Data_Item]->Max_ISI -
							  Data_Items[Selected_Data_Item]->Min_ISI)
					/Data_Items[Selected_Data_Item]->ISI_Histogram_Box;
	int tmpi=0;

	if( ISI_Histogram_Size == 0 ) ISI_Histogram_Size = 1;
    if( ISI_Histogram_Size > 10000 ) ShowMessage("ISI hist error X22");

	Data_Items[Selected_Data_Item]->ISI_Histogram.assign(ISI_Histogram_Size,tmpi);

	for(long k=0;k<Data_Items[Selected_Data_Item]->ISI_Histogram.size();k++)
		Data_Items[Selected_Data_Item]->ISI_Histogram[k]=0;

	for(long k=0;k<Data_Items[Selected_Data_Item]->ISIs.size();k++)
    {
    long ptr = (Data_Items[Selected_Data_Item]->ISI_Histogram.size()-1)*
	(Data_Items[Selected_Data_Item]->ISIs[k] - Data_Items[Selected_Data_Item]->Min_ISI)/
	(Data_Items[Selected_Data_Item]->Max_ISI - Data_Items[Selected_Data_Item]->Min_ISI);

    if( ptr >= 0 && ptr < Data_Items[Selected_Data_Item]->ISI_Histogram.size() )
	Data_Items[Selected_Data_Item]->ISI_Histogram[ptr]++;
	}

  }
}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::CLs_Labels_Display_CheckBoxClick(TObject *Sender)

{
	fill_wavelet_CL_plots();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::IndexORTime_RadioGroupClick(TObject *Sender)
{
	fill_wavelet_CL_plots();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Calculate_Synch_Table_ButtonClick(TObject *Sender)
{
/*
	Synchrogram_Result_Matrix_Size = Data_Items.size();

	for(long i=0;i<Synchrogram_Result_Matrix_Size;i++)
	for(long j=0;j<Synchrogram_Result_Matrix_Size;j++)
		Synchrogram_Result_Matrix[i][j]=1;

	for(int DI1=0;DI1<Data_Items.size();DI1++)
	for(int DI2=0;DI2<DI1;DI2++)
	{

	std::vector <double> Data_Vec1 = get_data_for_analysis(DI1);
	std::vector <double> Data_Vec2 = get_data_for_analysis(DI2);

	Synchrogram_Result_Matrix[DI1][DI2] = Numerical_Library_Obj.
		get_phase_synchronization(&Data_Vec1,&Data_Vec2,1,1,true,0);

	} // through all pairs of signals

	double min=MAXDOUBLE,max=-MAXDOUBLE,mean=0,Counter=0;
	for(int DI1=0;DI1<Synchrogram_Result_Matrix_Size;DI1++)
	for(int DI2=0;DI2<DI1;DI2++)
	{
		Synchrogram_Result_Matrix[DI2][DI1]=Synchrogram_Result_Matrix[DI1][DI2];

		if( DI1 != DI2 )
		{
		if(  Synchrogram_Result_Matrix[DI1][DI2] > max ) max = Synchrogram_Result_Matrix[DI1][DI2];
		if(  Synchrogram_Result_Matrix[DI1][DI2] < min ) min = Synchrogram_Result_Matrix[DI1][DI2];

		mean += Synchrogram_Result_Matrix[DI1][DI2];
		Counter++;
		}
	}

	Min_Synch = min;
	Max_Synch = max;
	if( Counter != 0 )
		mean /= Counter;

	Sych_Results_RichEdit->Lines[0].Clear();
	Sych_Results_RichEdit->Lines[0].Add("min= "+FloatToStrF(min,ffGeneral,3,2)+"\n max= "+
		FloatToStrF(max,ffGeneral,3,2)+"\n mean= "+FloatToStrF(mean,ffGeneral,3,2) );

	paint_Synchrogram_R_matrix();

*/
}
//---------------------------------------------------------------------------
void __fastcall TNL_Analysis_Form::Synchronization_Matrix_PaintBoxPaint(TObject *Sender)
{
	paint_Synchrogram_R_matrix();
}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::r_ceckboxClick(TObject *Sender)

{
	if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
	Filter_All_Signals_ButtonClick(this);
	else
	filter_current_signal();
	repaint_current_tab();

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Rectification_checkboxClick(TObject *Sender)
{
    if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
    Filter_All_Signals_ButtonClick(this);
    else    
	filter_current_signal();
	repaint_current_tab();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Butt_low_20_3th_order_checkboxClick(TObject *Sender)

{
    if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
	Filter_All_Signals_ButtonClick(this);
    else    
	filter_current_signal();
	repaint_current_tab();

}
//---------------------------------------------------------------------------

void TNL_Analysis_Form::filter_current_signal()
{
   if( selected_data_items_in_range() )
   {

	double TimeStep = Data_Items[0]->Time_Step_ms;
	double Sampling_Per_Sec = 1000.0/TimeStep;

	Data_Items[Selected_Data_Item_1]->Filtered_Signal.clear();
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Data_Items[Selected_Data_Item_1]->Voltage_Values;

	std::vector<double> v3 = Data_Items[Selected_Data_Item_1]->Filtered_Signal;

	int Derivative_Range = Derivative_Range_Edit->Text.ToInt();

	//--------------------------------------------------------------
	// 1st Derivative
	//--------------------------------------------------------------
	if( First_Derivative_CheckBox->State == cbChecked )
	{
		std::vector <double> V;
		V = Data_Items[Selected_Data_Item_1]->Filtered_Signal;

		for(long t=Derivative_Range;t<Data_Items[Selected_Data_Item_1]->Filtered_Signal.size()-Derivative_Range;t++)
		Data_Items[Selected_Data_Item_1]->Filtered_Signal[t]=V[t+Derivative_Range]-V[t-Derivative_Range];
	}

	//--------------------------------------------------------------
	// 2nd Derivative
	//--------------------------------------------------------------
	if( Second_Derivative_CheckBox->State == cbChecked )
	{
		std::vector <double> V;
		V = Data_Items[Selected_Data_Item_1]->Filtered_Signal;

		for(long t=Derivative_Range;t<Data_Items[Selected_Data_Item_1]->Filtered_Signal.size()-Derivative_Range;t++)
		Data_Items[Selected_Data_Item_1]->Filtered_Signal[t]=
			V[t-Derivative_Range]+V[t+Derivative_Range]-2.0*V[Derivative_Range];
	}

	//--------------------------------------------------------------
	// Notch filter
	//--------------------------------------------------------------

	if( Butt_band_40_250_3th_order_checkbox->State == cbChecked )
	{
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.Butt_band_40_250_3th_order(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,true);
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.Butt_band_40_250_3th_order(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,false);
	}

	if( Rectification_checkbox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.rectify_signal(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal);

	if( Butt_low_20_3th_order_checkbox->State == cbChecked )
	{
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.Butt_low_20_3th_order(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,false);

	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.Butt_low_20_3th_order(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,true);
	}

	if( Custom_Butt_Filter_CheckBox->State == cbChecked )
	{
		double LC = BW_Filter_Low_Edit->Text.ToDouble()/(Sampling_Per_Sec/2.0);
		double HC = BW_Filter_Up_Edit->Text.ToDouble()/(Sampling_Per_Sec/2.0);

		Numerical_Library_Obj.Butt_Filter.Initialize_Filter(
						  BW_Filter_Order_Edit->Text.ToInt(),LC,HC);

		Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.
			Butt_Filter.filter(&Data_Items[Selected_Data_Item_1]->Filtered_Signal,false);

		Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.
			Butt_Filter.filter(&Data_Items[Selected_Data_Item_1]->Filtered_Signal,true);
	}

	if( MedianFilter_CheckBox->State == cbChecked )
	{
		double WL = Median_WL_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item_1]->Time_Step_ms;

		Numerical_Library_Obj.
			median_filter(Data_Items[Selected_Data_Item_1]->Filtered_Signal,WL);
	}

	if( SLWMM_CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.
		get_sliding_maxmin_of_signal(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,SlidingmaxminEdit->Text.ToDouble()/
								Data_Items[Selected_Data_Item_1]->Time_Step_ms);

	if( Sliding_Window_SD_CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.
		get_sliding_SD_of_signal(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,SD_Window_Edit->Text.ToDouble()/
								Data_Items[Selected_Data_Item_1]->Time_Step_ms);

	if( Sum_Abs_Window_CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.get_abs_sum_in_window(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,Sum_Abs_Window_Edit->Text.ToDouble()/
								Data_Items[Selected_Data_Item_1]->Time_Step_ms);

	if( Normalize__CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.normalize_signal(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,Normalization_Window_Edit->Text.ToDouble()/
								Data_Items[Selected_Data_Item_1]->Time_Step_ms);

	if( Set_av_SD_CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal =
		Numerical_Library_Obj.set_av_to_0_SD_to_1(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal);


	if( Sinusoidal_Recomposition_CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.recompose_bipolar_signal(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,
			Sinsoidal_Wavelet_Period_Edit->Text.ToDouble()/
			 Data_Items[Selected_Data_Item_1]->Time_Step_ms,
			 Electrogram_Type_RadioGroup->ItemIndex);

	if( Hilb_Phase_CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.
		get_Hilbert_phase_using_convolution_double(
			&Data_Items[Selected_Data_Item_1]->Filtered_Signal,
			Data_Items[Selected_Data_Item_1]->Time_Step_ms );


	//--------------------------------------------------------------
	// correlaiton with V wave
	//--------------------------------------------------------------
	if( Corr_V_Wave_Signal_CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.
		get_correlation_vector(&Data_Items[Selected_Data_Item_1]->Filtered_Signal,
				&V_Wave,VWaveCorrStepEdit->Text.ToDouble()/
				Data_Items[Selected_Data_Item_1]->Time_Step_ms);

	if( XCorr_Filter_CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.
		get_xcorrelation_vector(&Data_Items[Selected_Data_Item_1]->Filtered_Signal,
				&V_Wave,VWaveCorrStepEdit->Text.ToDouble()/
				Data_Items[Selected_Data_Item_1]->Time_Step_ms);


	if( PSimilarity_CheckBox->State == cbChecked )
	Data_Items[Selected_Data_Item_1]->Filtered_Signal = Numerical_Library_Obj.
		get_psimilarity_vector(&Data_Items[Selected_Data_Item_1]->Filtered_Signal,
				&V_Wave,VWaveCorrStepEdit->Text.ToDouble()/
				Data_Items[Selected_Data_Item_1]->Time_Step_ms);
   }
}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Filter_All_Signals_ButtonClick(TObject *Sender)
{
	int Old_Sig = Selected_Data_Item_1;

	for(long i=0;i<Data_Items.size();i++)
	{
		Selected_Data_Item_1 = i;
		filter_current_signal();
	}

	Selected_Data_Item_1 = Old_Sig;

	repaint_current_tab();
}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Det_Peaks_Th_ButtonClick(TObject *Sender)
{
	for(long i=0;i<Data_Items.size();i++)
		detect_peaks_in_signal(i);

	repaint_current_tab();
}

//---------------------------------------------------------------------------

void TNL_Analysis_Form::detect_peaks_in_signal(int Selected_Data_Item)
{
	if( Data_Items[Selected_Data_Item_1]->Time_Step_ms == 0 )
	ShowMessage("Time step not set!");
	else
	{

	std::vector <double> Data_Vec1 = get_data_for_analysis(Selected_Data_Item);

	Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal.clear();

	//double dt = Data_Items[Selected_Data_Item]->Time_Step_ms;

	//--------------------------------------------------------------------
	if( PDT_SignalMode_RadioGroup->ItemIndex ==  0 )
	//--------------------------------------------------------------------
	{

	Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal =
		Numerical_Library_Obj.get_peak_positions(
			&Data_Vec1,
			PDT_Edit->Text.ToDouble(),
			PDTW_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item]->Time_Step_ms,
			PD_Blanking_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item]->Time_Step_ms);
	}

	//--------------------------------------------------------------------
	if( PDT_SignalMode_RadioGroup->ItemIndex ==  1 )
	//--------------------------------------------------------------------
	{
	std::vector <double> DiffY;

	double tmp=0;
	DiffY.push_back(tmp);

	for(long t=0;t<(signed)Data_Vec1.size()-1;t++)
	if( -(Data_Vec1[t+1]-Data_Vec1[t]) > 0 ) // to get only negative peaks
	DiffY.push_back( -(Data_Vec1[t+1]-Data_Vec1[t]) );
	else
	DiffY.push_back( 0 );

	Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal =
		Numerical_Library_Obj.get_peak_positions(
			&DiffY,
			PDT_Edit->Text.ToDouble(),
			PDTW_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item]->Time_Step_ms,
			PD_Blanking_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item]->Time_Step_ms);
	}

	//--------------------------------------------------------------------
	if( PDT_SignalMode_RadioGroup->ItemIndex == 2 ) // max down slope
	//--------------------------------------------------------------------
	{

	double diff,tmp=0,max=100000,max_ptr=-1;

	for(long t=0;t<(signed)Data_Vec1.size()-1;t++)
	{
		diff = Data_Vec1[t+1]-Data_Vec1[t];
		if( diff < max )
		{
			max = diff;
			max_ptr = t;
		}
	}

	Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal.push_back(max_ptr);

	}

	//--------------------------------------------------------------------
	if( PDT_SignalMode_RadioGroup->ItemIndex == 3 ) // zero crossing
	//--------------------------------------------------------------------
	{

	double max_ptr=-1;

	for(long t=0;t<(signed)Data_Vec1.size()-1;t++)
	if( Data_Vec1[t] > 0 && Data_Vec1[t+1] < 0 )
	if( max_ptr == -1 )
		max_ptr = t;

	Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal.push_back(max_ptr);

	}

	//--------------------------------------------------------------------
	if( PDT_SignalMode_RadioGroup->ItemIndex == 4 ) // max val zero crossing
	//--------------------------------------------------------------------
	{

	double diff,tmp=0,max=0,max_ptr=-1;

	for(long t=1;t<(signed)Data_Vec1.size()-1;t++)
	if( Data_Vec1[t-1] > 0 && Data_Vec1[t+1] < 0 )
	{
		diff = Data_Vec1[t-1]-Data_Vec1[t+1];
		if( diff > max )
		{
			max = diff;
			max_ptr = t;
		}
	}

	Data_Items[Selected_Data_Item]->Peaks_Positions_In_Signal.push_back(max_ptr);

	}

	// caluclate ISI stuff
	calculate_ISIs_from_wavelet_peaks(Selected_Data_Item);

	} // dt != 0
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Disp_2nd_Signal_CheckBoxClick(TObject *Sender)

{
	repaint_current_tab();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Synchronization_Matrix_PaintBoxMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
/*
	double wx;
	if( Data_Items.size() != 0 )
		wx = (double)(Synchronization_Matrix_PaintBox->Width)
			/ (double)(Data_Items.size());
	else
		wx = 1;

	int i_ptr = (double)X / wx;
	int j_ptr = (double)Y / wx;


	if( i_ptr >= 0 && i_ptr < Synchrogram_Result_Matrix_Size )
	if( j_ptr >= 0 && j_ptr < Synchrogram_Result_Matrix_Size )
	{

	Secondary_Data_StringGrid->Row = j_ptr;
	Primary_Data_StringGrid->Row = i_ptr+1;

	recalculate_current_tab();
	repaint_current_tab();

	}
*/
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::CorrTable_PaintBoxMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	double wx;
	if( Data_Items.size() != 0 )
		wx = (double)(CorrTable_PaintBox->Width)
			/ (double)(Data_Items.size());
	else
		wx = 1;

	int i_ptr = (double)X / wx;
	int j_ptr = (double)Y / wx;


	if( i_ptr >= 0 && i_ptr < Correlation_Matrix_Size )
	if( j_ptr >= 0 && j_ptr < Correlation_Matrix_Size )
	{

	Secondary_Data_StringGrid->Row = j_ptr;
	Primary_Data_StringGrid->Row = i_ptr+1;

	recalculate_current_tab();
	repaint_current_tab();

	}
}

//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Normalze_CheckBoxClick(TObject *Sender)
{
    if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
    Filter_All_Signals_ButtonClick(this);
    else    
	filter_current_signal();
	repaint_current_tab();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Hilb_Phase_CheckBoxClick(TObject *Sender)
{
	if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
    Filter_All_Signals_ButtonClick(this);
    else    
	filter_current_signal();
	repaint_current_tab();
}
//---------------------------------------------------------------------------

std::vector <double> TNL_Analysis_Form::get_data_for_analysis(int Set_Id)
{
	std::vector <double> Result;

	if( Set_Id >= 0 && Set_Id < Data_Items.size() )
	{

	long Start = Raw_Signals_Chart->BottomAxis->Minimum/Data_Items[Set_Id]->Time_Step_ms;
	long Stop = Raw_Signals_Chart->BottomAxis->Maximum/Data_Items[Set_Id]->Time_Step_ms;

	if(Start<0)
		Start = 0;
	if( Stop <= 0 || Stop >= Data_Items[Set_Id]->Voltage_Values.size() )
		Stop = Data_Items[Set_Id]->Voltage_Values.size() - 1;

// !!!!!!!!!!!!!!!!!!!!!!!!! 16.II.2018 - not sure how this will affect the program
	Start = 1;
	Stop = Data_Items[Set_Id]->Voltage_Values.size()-1;


	if( Data_Source_RadioGroup->ItemIndex == 0 )
	for(long i=Start;i<Stop;i++)
	Result.push_back(Data_Items[Set_Id]->Voltage_Values[i]);

	if( Data_Source_RadioGroup->ItemIndex == 1 )
	if( Start >= 0 && Stop < Data_Items[Set_Id]->Filtered_Signal.size() )
	for(long i=Start;i<Stop;i++)
	Result.push_back(Data_Items[Set_Id]->Filtered_Signal[i]);

	if( Data_Source_RadioGroup->ItemIndex == 2 )
	for(long i=0;i<(signed)Data_Items[Set_Id]->Peaks_Positions_In_Signal.size();i++)
	Result.push_back(Data_Items[Set_Id]->Peaks_Positions_In_Signal[i]);

	if( Data_Source_RadioGroup->ItemIndex == 3 )
	for(long i=1;i<(signed)Data_Items[Set_Id]->ISIs.size();i++)
	Result.push_back(Data_Items[Set_Id]->ISIs[i]);
//	Result.push_back(Data_Items[Set_Id]->Peaks_Positions_In_Signal[i]-
//					 Data_Items[Set_Id]->Peaks_Positions_In_Signal[i-1] );
	}

	return Result;
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::P_Map_PaintBoxPaint(TObject *Sender)
{
	int Delay = PM_Delay_Edit->Text.ToDouble();

	if( selected_data_items_in_range() )
	{

	std::vector <double> Data_Vec = get_data_for_analysis(Selected_Data_Item_1);

	// clear paiting area
	P_Map_PaintBox->Canvas->Brush->Color = clWhite;
	P_Map_PaintBox->Canvas->FillRect(
			Rect(0,0,P_Map_PaintBox->Width,P_Map_PaintBox->Height));

	P_Map_PaintBox->Canvas->Pen->Color = clBlack;
	P_Map_PaintBox->Canvas->Brush->Color = clBlack;


	// find min,max
	double min=1000000,max=-1000000;
	for(long i=0;i<(signed)Data_Vec.size();i++)
	{
		if( Data_Vec[i] < min )
			min = Data_Vec[i];
		if( Data_Vec[i] > max )
			max = Data_Vec[i];
	}

	if(max == min) max = min+1;

	// paint
	int cx,cy,old_cx,old_cy;
	for(long i=Delay;i<(signed)Data_Vec.size();i++)
	{
		cx = (Data_Vec[i]-min)/(max-min)*P_Map_PaintBox->Width;
		cy = (Data_Vec[i-Delay]-min)/(max-min)*P_Map_PaintBox->Height;

		if( i==Delay)
		{ old_cx = cx; old_cy = cy; }

		P_Map_PaintBox->Canvas->MoveTo(old_cx,old_cy);
		P_Map_PaintBox->Canvas->FillRect(Rect(old_cx-1,old_cy-1,old_cx+1,old_cy+1));
		P_Map_PaintBox->Canvas->LineTo(cx,cy);

		old_cx = cx;
		old_cy = cy;
	}

	// put ending
	P_Map_PaintBox->Canvas->FillRect(Rect(cx-3,cy-3,cx+3,cy+3));

	} // if data in range
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::CorrTable_PaintBoxPaint(TObject *Sender)
{
	paint_R_matrix();
}
//---------------------------------------------------------------------------

int TNL_Analysis_Form::calculate_FFT_spectrum(std::vector <double> *Data_Vec, long Start, long Stop)
{
	//-----------------------------------------------------------------------
	// allocate data structure
	//-----------------------------------------------------------------------
	if( Start < 0 || Start >= Data_Items[Selected_Data_Item_1]->Voltage_Values.size() )
		Start = 0;
	if( Stop <= 0 || Stop >= Data_Items[Selected_Data_Item_1]->Voltage_Values.size() )
		Stop = Data_Items[Selected_Data_Item_1]->Voltage_Values.size() - 1;

	if( Stop - Start > 10 )
	{

	// round up to closest power of 2
	double d = floor( log10(Stop-Start)/log10(2) );
//	Signal_for_FFT_Size = pow(2,d+1);
	Signal_for_FFT_Size = pow(2,d+3); // to make signal smoother (looked more like fft result in matlab)

	delete [] Signal_for_FFT;
	Signal_for_FFT = new double[Signal_for_FFT_Size];

	delete [] FFT_spectrum;
	FFT_spectrum = new double[Signal_for_FFT_Size];

	Resampled_Signal_For_FFT =
		Numerical_Library_Obj.resample(Data_Vec, Start, Stop, Signal_for_FFT_Size);

	//ofstream df("resampled_sig.csv");
	for(long k=0;k<Signal_for_FFT_Size;k++)
	{
		//df << Resampled_Signal_For_FFT[k] << endl;
		Signal_for_FFT[k] = Resampled_Signal_For_FFT[k];
		FFT_spectrum[k]=0;
	}
	//df.close();

	Sampling_Per_Sec_of_Rescaled_Signal = 1000.0/Data_Items[0]->Time_Step_ms*
		Signal_for_FFT_Size/((double)Stop - (double)Start);

	//-----------------------------------------------------------------------
	// Hanning windowing
	//-----------------------------------------------------------------------
/*
	for(long k=0;k<Signal_for_FFT_Size;k++)
		Signal_for_FFT[k] *= 0.54-0.46*cos(6.283185307*((double)k-1.0)/
		((double)Signal_for_FFT_Size-2.0));
*/
	//-----------------------------------------------------------------------
	// perform FFT // (old)
	//-----------------------------------------------------------------------
	// FFT object initialized here
	FFTReal	fft(Signal_for_FFT_Size);
	fft.do_fft(FFT_spectrum, Signal_for_FFT);

	//-----------------------------------------------------------------------
	// Find DF in 1-12 Hz range
	//-----------------------------------------------------------------------
	double Freq,v;
	double Min_DF = Min_DF_Edit->Text.ToDouble();
	double Max_DF = Max_DF_Edit->Text.ToDouble();
	double Max_DF_Amplitude = -MAXDOUBLE;

	for(long k=    1*(Signal_for_FFT_Size)/Sampling_Per_Sec_of_Rescaled_Signal;
			  k<12.0*(Signal_for_FFT_Size)/Sampling_Per_Sec_of_Rescaled_Signal;k++)
	{
		Freq = Sampling_Per_Sec_of_Rescaled_Signal*k/(Signal_for_FFT_Size);

		v = pow((double)FFT_spectrum[k],2.0)+
			pow((double)FFT_spectrum[(long)(0.5*Signal_for_FFT_Size)+k],2.0);

		if( v > Max_DF_Amplitude && Freq > Min_DF && Freq < Max_DF )
		{
			Max_DF_Amplitude = v;
			DF = Freq;
		}
	}

	//-----------------------------------------------------------------------
	// Calculate FFTr
	//-----------------------------------------------------------------------
	double HighFArea=0,LowFArea=0;

	for(long  k=0;k<Signal_for_FFT_Size/2;k++)
	{
		Freq = Sampling_Per_Sec_of_Rescaled_Signal*k/(Signal_for_FFT_Size);

		v =     pow((double)FFT_spectrum[k],2.0)+
				pow((double)FFT_spectrum[(long)(0.5*Signal_for_FFT_Size)+k],2.0);
		if(Freq>=1 && Freq<VT_FFT_Threshold_1)
			LowFArea += v;
		if(Freq>=VT_FFT_Threshold_1 && Freq<VT_FFT_Threshold_2)
			HighFArea += v;
	}

	if( LowFArea + HighFArea != 0)
	FFTr = 100.*HighFArea / (LowFArea + HighFArea);
	else
	FFTr = NOT_POSSIBLE_TO_CALCULATE_VALUE;

	//------------------------------------------------------------
	// Calculate regularity and organization index
	//------------------------------------------------------------
	double RI_Band = 0.5; //0.375;
	double OI_Band = 0.5; //0.375;
	double FFT_Left_Cut = 3;
	double FFT_Right_Cut = 15;
	double Coef = (Signal_for_FFT_Size)/Sampling_Per_Sec_of_Rescaled_Signal;
	double Nom,Den;

	Nom = get_FFT_longegral(DF-RI_Band,DF+RI_Band,Coef,Signal_for_FFT_Size, FFT_spectrum);
	Den = get_FFT_longegral(FFT_Left_Cut,FFT_Right_Cut,Coef,Signal_for_FFT_Size, FFT_spectrum);
	if( Den != 0 )
		DF_Regularity_Index = Nom/Den;
	else
		DF_Regularity_Index = NOT_POSSIBLE_TO_CALCULATE_VALUE;

	// Calculate organization index
    Nom = 0.0;
    int k=1;
    if( DF > 0 )
    while( DF*k < FFT_Right_Cut )
	{
        Nom += get_FFT_longegral(k*DF-OI_Band,k*DF+OI_Band,Coef,Signal_for_FFT_Size, FFT_spectrum);
        k++;
    }

    if( Den != 0 )
        DF_Organization_Index = Nom/Den;
	else
		DF_Organization_Index = NOT_POSSIBLE_TO_CALCULATE_VALUE;


	} // if signal size > 10

	return 1;
}

//---------------------------------------------------------------------------

double TNL_Analysis_Form::get_FFT_longegral(double Start_Hz,double Stop_Hz,
    double Coef,long FFT_Data_Length, double* FFT_Vector)
{
    double longegral = 0;

    // [Polonger] = [Hz] * Coef

    for(long i=Start_Hz*Coef;i<Stop_Hz*Coef;i++)
    if(i>=0 && i<FFT_Data_Length)
    {

	longegral += pow((double)FFT_Vector[i],2.0)+
                pow((double)FFT_Vector[(long)(0.5*FFT_Data_Length)+i],2.0);

    }

    return longegral;
}

//---------------------------------------------------------------------------

void TNL_Analysis_Form::paint_FFT_spectrum()
{
    //-----------------------------------------------------------------------
    // display signal and FFT
    //-----------------------------------------------------------------------
	double v,Freq;

	// paint FFT spectrum std::vector
	FullFFT_Chart->Series[0]->Clear();
	for(long k=0;k<Signal_for_FFT_Size;k++)
	FullFFT_Chart->Series[0]->AddXY(k,FFT_spectrum[k]);

	// plot resampled singal
	Resampled_Chart->Series[0]->Clear();
	for(long k=0;k<Signal_for_FFT_Size;k++)
	Resampled_Chart->Series[0]->AddXY(k,Resampled_Signal_For_FFT[k] );

	// plot FFT spectrum
	FFT_Spectrum_Chart->Series[0]->Clear();
	FFT_Spectrum_Chart->Series[1]->Clear();
	for(long  k=0;k<Signal_for_FFT_Size/2;k++)
	{
		Freq = Sampling_Per_Sec_of_Rescaled_Signal*k/(Signal_for_FFT_Size);

		v =     pow((double)FFT_spectrum[k],2.0)+
				pow((double)FFT_spectrum[(long)(0.5*Signal_for_FFT_Size)+k],2.0);

		FFT_Spectrum_Chart->Series[0]->AddXY(Freq,v);
	}

//	FFT_Spectrum_Chart->Series[1]->AddXY(DF,Max_DF_Amplitude);

	FFT_Results_RichEdit->Lines[0].Clear();
	FFT_Results_RichEdit->Lines[0].Add("DF= "+FloatToStrF(DF,ffGeneral,3,2 ));
	FFT_Results_RichEdit->Lines[0].Add("Regularity_Index= "+FloatToStrF(DF_Regularity_Index,ffGeneral,3,2 ));
	FFT_Results_RichEdit->Lines[0].Add("Organization_Index= "+FloatToStrF(DF_Organization_Index,ffGeneral,3,2 ));
	FFT_Results_RichEdit->Lines[0].Add("\nFFTr= "+FloatToStrF(FFTr,ffGeneral,3,2 ));
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Raw_Signals_ChartDblClick(TObject *Sender)
{
	if( selected_data_items_in_range() )
    // if marker clicked (and not the last one
    if( Dragged_Value_Index >= 0 && Dragged_Value_Index < Dragged_Series->Count()-1 )
    {

	long Start = Dragged_Series->XValue[Dragged_Value_Index]+
                 0.25*( Dragged_Series->XValue[Dragged_Value_Index+1] -
                        Dragged_Series->XValue[Dragged_Value_Index] );

    long Stop = Dragged_Series->XValue[Dragged_Value_Index]+
                 0.75*( Dragged_Series->XValue[Dragged_Value_Index+1] -
                        Dragged_Series->XValue[Dragged_Value_Index] );

    if( Data_Items[Selected_Data_Item_1]->Time_Step_ms != 0 )
    {
        Start = Start / Data_Items[Selected_Data_Item_1]->Time_Step_ms;
        Stop  = Stop  / Data_Items[Selected_Data_Item_1]->Time_Step_ms;
    }

    // find max slope in the signal
	std::vector <double> Data_Vec_1 = get_data_for_analysis(Selected_Data_Item_1);

	// replot raw signals
    double Max_Slope = 0;
    long Max_Slope_Pos = -1;

	for(long i=Start+1;i<Stop;i++)
	if( i>0 && i<(signed)Data_Vec_1.size() )
    if( fabs( Data_Vec_1[i]-Data_Vec_1[i-1]) > Max_Slope )
    {
        Max_Slope = fabs( Data_Vec_1[i]-Data_Vec_1[i-1]);
        Max_Slope_Pos = i;
    }

    Dragged_Series->AddXY(Max_Slope_Pos*Data_Items[Selected_Data_Item_1]->Time_Step_ms,0.0 );
    update_Peak_Positions_in_Data_Vector();

    Dragged_Value_Index = -1;

    }
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::InvTime_CheckBoxClick(TObject *Sender)
{
    if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
    Filter_All_Signals_ButtonClick(this);
    else
	filter_current_signal();
	repaint_current_tab();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Sinusoidal_Recomposition_CheckBoxClick(TObject *Sender)
{
    if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
    Filter_All_Signals_ButtonClick(this);
    else
	filter_current_signal();
	repaint_current_tab();
}
//---------------------------------------------------------------------------




void __fastcall TNL_Analysis_Form::Compute_NavX_CAFEs_ButtonClick(TObject *Sender)

{
	if( Data_Items.size() > 0 )
	{

    double Time_Step_ms = Data_Items[0]->Time_Step_ms;

    if( Time_Step_ms != 0 )
    {

	double Ref = CAFE_Ref_Edit->Text.ToDouble()/Time_Step_ms;
	double PP = CAFE_PP_Edit->Text.ToDouble();
	double Dur = CAFE_Dur_Edit->Text.ToDouble()/Time_Step_ms;
	double STh = CAFE_DTh_Edit->Text.ToDouble()*Time_Step_ms;

	for(long i=0;i<(signed)Data_Items.size();i++)
	{

	std::vector <double> Data_Vec1 = get_data_for_analysis(i);

	Data_Items[i]->NavX_CAFE_Positions = Numerical_Library_Obj.
        calculate_NAVX_CAFE(1,Data_Vec1.size()-1,
            &Data_Vec1,Ref,PP,Dur,STh,&Data_Items[i]->NavX_CAFE);

	Data_Items[i]->NavX_CAFE *= Time_Step_ms; // to convert value to ms

	if(Echo)
	ShowMessage("CFAE= "+FloatToStr(Data_Items[i]->NavX_CAFE));

    }

    }
    }
}
//---------------------------------------------------------------------------





void __fastcall TNL_Analysis_Form::MedianFilter_CheckBoxClick(TObject *Sender)
{
	if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
	Filter_All_Signals_ButtonClick(this);
	else
	filter_current_signal();
	repaint_current_tab();

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Line_Thickness_ComboBoxChange(TObject *Sender)

{
	Raw_Signals_Chart->Series[0]->Pen->Width = Line_Thickness_ComboBox->Text.ToInt();
	Raw_Signals_Chart->Series[2]->Pen->Width = Line_Thickness_ComboBox->Text.ToInt();

	Filtered_Signal_Chart->Series[0]->Pen->Width = Line_Thickness_ComboBox->Text.ToInt();
	Filtered_Signal_Chart->Series[1]->Pen->Width = Line_Thickness_ComboBox->Text.ToInt();

	Raw_Signals_Chart->Repaint();
	Filtered_Signal_Chart->Repaint();
}
//---------------------------------------------------------------------------


void __fastcall TNL_Analysis_Form::ColorSet_ComboBoxChange(TObject *Sender)
{

	if( ColorSet_ComboBox->ItemIndex == 0 )
	{
	Raw_Signals_Chart->Series[0]->Color = clBlue;
	Raw_Signals_Chart->Series[2]->Color = clRed;

	Filtered_Signal_Chart->Series[0]->Color = clBlue;
	Filtered_Signal_Chart->Series[1]->Color = clRed;

	}

	if( ColorSet_ComboBox->ItemIndex == 1 )
	{
	Raw_Signals_Chart->Series[0]->Color = clBlack;
	Raw_Signals_Chart->Series[2]->Color = clMedGray;

	Filtered_Signal_Chart->Series[0]->Color = clBlack;
	Filtered_Signal_Chart->Series[1]->Color = clMedGray;

	}

	Raw_Signals_Chart->Repaint();
	Filtered_Signal_Chart->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Background_grid_CheckBoxClick(TObject *Sender)

{
	if( Background_grid_CheckBox->State )
	{
	Raw_Signals_Chart->LeftAxis->Grid->Visible = true;
	Raw_Signals_Chart->BottomAxis->Grid->Visible = true;
	Filtered_Signal_Chart->LeftAxis->Grid->Visible = true;
	Filtered_Signal_Chart->BottomAxis->Grid->Visible = true;
	}
	else
	{
	Raw_Signals_Chart->LeftAxis->Grid->Visible = false;
	Raw_Signals_Chart->BottomAxis->Grid->Visible = false;
	Filtered_Signal_Chart->LeftAxis->Grid->Visible = false;
	Filtered_Signal_Chart->BottomAxis->Grid->Visible = false;
	}

	Raw_Signals_Chart->Repaint();
	Filtered_Signal_Chart->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Raw_Signals_ChartZoom(TObject *Sender)
{
	Filtered_Signal_Chart->BottomAxis->SetMinMax(Raw_Signals_Chart->BottomAxis->Minimum,
												 Raw_Signals_Chart->BottomAxis->Maximum );

	recalculate_current_tab();
	repaint_current_tab();

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Raw_Signals_ChartUndoZoom(TObject *Sender)
{
	Filtered_Signal_Chart->BottomAxis->SetMinMax(Raw_Signals_Chart->BottomAxis->Minimum,
												 Raw_Signals_Chart->BottomAxis->Maximum );

	recalculate_current_tab();
	repaint_current_tab();

}
//---------------------------------------------------------------------------


void __fastcall TNL_Analysis_Form::Recurrence_Pair_ButtonClick(TObject *Sender)
{
/*
	// 1. Allocate diagram
	Recurrence_Plot_PaintBox->Width = Recurrence_Plot_PaintBox->Height;
	Recurrence_Diagram.allocate_diagram(Recurrence_Plot_PaintBox->Width);

	Selected_Data_Item_1 = Primary_Data_StringGrid->Row-1;
	Selected_Data_Item_2 = Secondary_Data_StringGrid->Row;

	if( selected_data_items_in_range() )
	{

	std::vector <double> Data_Vec1 = get_data_for_analysis(Selected_Data_Item_1);
	std::vector <double> Data_Vec2 = get_data_for_analysis(Selected_Data_Item_2);

	// 2. Calculate diagram
	double Threshold = Rec_11_Th_Edit->Text.ToDouble();
	long Diag_Size = Recurrence_Diagram.get_diagram_size();
	long ptr1,ptr2;

	for(long t1=0;t1<Diag_Size;t1++)
	for(long t2=0;t2<Diag_Size;t2++)
	{
		ptr1 = (double)t1*(double)Data_Vec1.size()/(double)Diag_Size;
		ptr2 = (double)t2*(double)Data_Vec2.size()/(double)Diag_Size;

		if( ptr1 > 0 && ptr1 < (signed)Data_Vec1.size() )
		if( ptr2 > 0 && ptr2 < (signed)Data_Vec2.size() )
			Recurrence_Diagram.Diagram_Values.set_value(t1,t2,
				min( fabs(Data_Vec1[ptr1]-Data_Vec2[ptr2]),
					2*M_PI-fabs(Data_Vec1[ptr1]-Data_Vec2[ptr2]))/M_PI);
	}


	}
*/
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Signals_Display_PaintBoxPaint(TObject *Sender)
{
	if( selected_data_items_in_range() )
	{

	long ptr,ptr1,ptr2,i,j;
	double x1,y1,x2,y2,x3;
	int pos;
	double Vertical_Spacing;
	double Start = Data_Items[Selected_Data_Item_1]->Min_Ptr;
	double Stop = Data_Items[Selected_Data_Item_1]->Max_Ptr;

	// clear signals box
	Signals_Window_PaintBox_Bitmap->Canvas->Brush->Color = (TColor)RGB(230,230,230);
	Signals_Window_PaintBox_Bitmap->Canvas->FillRect(Signals_Display_PaintBox->ClientRect );
/*
	//-----------------------------
	// ELECTRICAL SILENCE VECTOR (based on threshold, for Mario study)
	//-----------------------------
	Signals_Window_PaintBox_Bitmap->Canvas->Brush->Color=(TColor)( RGB(255,182,193) );
	long EGM_Length = Data_Items[0]->Voltage_Values.size();

	if( Silence_Vector.size() > 1 )
	for(long j=0;j<Signals_Display_PaintBox->Width;j++)
	{

	ptr1 = Start+(double)j/(double)Signals_Display_PaintBox->Width*(Stop-Start);

	if( ptr1 >= 0 && ptr1 <(signed)Silence_Vector.size() )
	if( Silence_Vector[ptr1] == 0 )
	{
		x1 = j;
		Signals_Window_PaintBox_Bitmap->Canvas->FillRect(
			Rect( x1,0,x1+1,Signals_Display_PaintBox->Height ) );
	}
	}
*/

	//-----------------------------
	// % of CL occupied
	//-----------------------------
	double Block_size = 0.5*PercOcc_Block_Edit->Text.ToDouble()/
			Data_Items[0]->Time_Step_ms;
	Signals_Window_PaintBox_Bitmap->Canvas->Brush->Color=(TColor)( RGB(255,182,193) );

	if( MCP_CheckBox->State == cbChecked )
	for(long i=0;i<(signed)Data_Items.size();i++)
	for(long p=0;p<(signed)Data_Items[i]->Peaks_Positions_In_Signal.size();p++)
	{

	x1 = (Data_Items[i]->Peaks_Positions_In_Signal[p]-Block_size-Start)/(Stop-Start)*
			(double)Signals_Display_PaintBox->Width;
	x2 = (Data_Items[i]->Peaks_Positions_In_Signal[p]+Block_size-Start)/(Stop-Start)*
			(double)Signals_Display_PaintBox->Width;

	Signals_Window_PaintBox_Bitmap->Canvas->FillRect(
		Rect( x1,0,x2,Signals_Display_PaintBox->Height ) );

	} // for all peaks


	std::vector <double> Data_Vec1;

	//------------------------------
	//------------------------------
	// SIGNALS PAINT
	//------------------------------
	//------------------------------
	double Bottom_Offset = BottomOffset_Edit->Text.ToDouble();

	// current signals display
	for(int i=0;i<(signed)Data_Items.size();i++)
	{

	Data_Vec1 = get_data_for_analysis(i);

	Vertical_Spacing = (Signals_Display_PaintBox->Height-Bottom_Offset)*(double)(i+1)/(double)(Data_Items.size()+1);

	// set line width
	Signals_Window_PaintBox_Bitmap->Canvas->Pen->Width = 1;

	// set color
	Signals_Window_PaintBox_Bitmap->Canvas->Pen->Color = clBlack;
	Signals_Window_PaintBox_Bitmap->Canvas->Font->Color = clBlack;
	Signals_Window_PaintBox_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;


	for(long j=0;j<Signals_Display_PaintBox->Width;j++)
	{

	ptr1 = Start+(double)j/(double)Signals_Display_PaintBox->Width*(Stop-Start);
	ptr2 = Start+(double)(j+1)/(double)Signals_Display_PaintBox->Width*(Stop-Start);

	if( ptr1 <(signed)Data_Vec1.size() &&  ptr2 < (signed)Data_Vec1.size() )
	{

	x1 = j;
	y1 = Vertical_Spacing-Data_Vec1[ptr1]*Signals_Amplitude_Amplify;

	x2 = j+1;
	y2 = Vertical_Spacing-Data_Vec1[ptr2]*Signals_Amplitude_Amplify;

	Signals_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Signals_Window_PaintBox_Bitmap->Canvas->LineTo( x2, y2 );

	}

	} // for all pixels


	//-------------------------------------
	// display name
	//-------------------------------------
	Signals_Window_PaintBox_Bitmap->Canvas->Brush->Color=(TColor)( RGB(200,200,200) );
	Signals_Window_PaintBox_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;
	Signals_Window_PaintBox_Bitmap->Canvas->TextFlags = ETO_OPAQUE;
	Signals_Window_PaintBox_Bitmap->Canvas->TextOutW(5,
		Signals_Display_PaintBox->Height*(double)(i+1)/(double)(Data_Items.size()+1)-5,Data_Items[i]->Name);


	//-------------------------------------
	// LOCAL DEFLECTIONS
	//-------------------------------------
	int BS = 3;
	for(int act=0;act<Data_Items[i]->Peaks_Positions_In_Signal.size();act++)
	if( Data_Items[i]->Peaks_Positions_In_Signal[act] > 0 )
	if( Data_Items[i]->Peaks_Positions_In_Signal[act] < Data_Vec1.size() )
	{
		Vertical_Spacing = Signals_Display_PaintBox->Height*(double)(i+1)/(double)(Data_Items.size()+1);

		x1 = (Data_Items[i]->Peaks_Positions_In_Signal[act]-Start)/(Stop-Start)*
				(double)Signals_Display_PaintBox->Width;
//		y1 = Vertical_Spacing-Data_Vec1[Data_Items[i]->Peaks_Positions_In_Signal[act]]*Signals_Amplitude_Amplify;
		y1 = Vertical_Spacing;

		Signals_Window_PaintBox_Bitmap->Canvas->Brush->Color= clRed;
		Signals_Window_PaintBox_Bitmap->Canvas->FillRect( Rect( x1-BS,y1-BS,x1+BS,y1+BS) );
	}

	} // through signals



	//-------------------------------------
	// CONDUCTION RIPPLES
	//-------------------------------------
/*
	int BS = MS_Edit->Text.ToDouble();

	for(long r=0;r<(signed)Conduction_Ripples.size();r++)
	for(long p=0;p<(signed)Conduction_Ripples[r].Activation_Ids.size();p++)
	{
	long DP = Conduction_Ripples[r].Data_Point_Ids[p];
	long act = Conduction_Ripples[r].Activation_Ids[p];

	if( DP >= 0 && DP < (signed)Data_Items.size() )
	if( act >= 0 && act < (signed)Data_Items[DP].Peaks_Positions_In_Signal.size() )
	{

	Vertical_Spacing = Signals_Display_PaintBox->Height*(double)(DP+1)/(double)(Data_Items.size()+1);

	x1 = (Data_Items[DP].Peaks_Positions_In_Signal[act]-Start)/(Stop-Start)*
			(double)Signals_Display_PaintBox->Width;
	y1 = Vertical_Spacing;

	// generate random color
	double R = 255*((int)(100*(long)r)%3) / 2.0;
	double G = 255*((int)(100*(long)r)%7) / 6.0;
	double B = 255*((int)(100*(long)r)%5) / 4.0;

	if( Black_Markers_CheckBox->State == cbChecked )
	{
		R = 10; G = 10; B = 10;
	}

	Signals_Window_PaintBox_Bitmap->Canvas->Brush->Color=(TColor)RGB(R,G,B);
	// Signals_Window_PaintBox_Bitmap->Canvas->Brush->Color= clRed;

	Signals_Window_PaintBox_Bitmap->Canvas->FillRect( Rect( x1-BS,y1-BS,x1+BS,y1+BS) );
	}
	}
*/

	Signals_Window_PaintBox_Bitmap->Canvas->Pen->Width = 1;
	Signals_Display_PaintBox->Canvas->Draw(0, 0, Signals_Window_PaintBox_Bitmap);

    }
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Signals_Display_PaintBoxMouseMove(TObject *Sender,
		  TShiftState Shift, int X, int Y)
{
	//----------------------------------------------------------
	// RIHGT BUTTON
	//----------------------------------------------------------
	if( MouseButton == mbRight && MouseStatus ==  MB_DOWN1)
	{

		if (Y > Mouse_Position_Y )
			Signals_Amplitude_Amplify *= 1.5;
		if (Y < Mouse_Position_Y )
			Signals_Amplitude_Amplify /= 1.5;

		Signals_Display_PaintBoxPaint(this);

	Mouse_Position_X = X;
	Mouse_Position_Y = Y;

	}

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Signals_Display_PaintBoxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
   MouseButton = Button;
   MouseStatus = MB_DOWN1;

   Mouse_Position_X = X;
   Mouse_Position_Y = Y;

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Signals_Display_PaintBoxMouseUp(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	MouseButton = Button;
	MouseStatus = MB_UP1;

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::PercOcc_Rec_ButtonClick(TObject *Sender)
{
/*
	double Block_size = 0.5*PercOcc_Block_Edit->Text.ToDouble()/Data_Items[0]->Time_Step_ms;
	double Max_LAT_Difference = 100./Data_Items[0]->Time_Step_ms;
	bool Max_LAT_Difference_Crossed;

	Data_Source_RadioGroup->ItemIndex = 1;
	Det_Peaks_Th_ButtonClick(this);

	//-----------------------------------------------------------------
	// 2. Calculate PIVOTING VECTOR
	//-----------------------------------------------------------------
	// starting from first point (DP=0) at the moment, later may need to change
	int score,tmp,DP = 0,PP;
	long min_diff,min_ptr,diff;
	Pivoting_Scores.clear();
	Pivoting_Events_Lengths.clear();
	Pivoting_Events_Start.clear();
	double Total_Length;
	Conduction_Ripple RP;
	Conduction_Ripples.clear();

	// create masks (to know which activations were already visited)
	bool flag=false;
	for(long DP=0;DP<Data_Items.size();DP++)
		Data_Items[DP].Peaks_Positions_In_Signal_Flags.assign(
			Data_Items[DP].Peaks_Positions_In_Signal.size(),flag);

	// MAIN FORS
	for(long DP=0;DP<Data_Items.size();DP++)
	for(long p=1;p<Data_Items[DP].Peaks_Positions_In_Signal.size()-1;p++)
	if( !Data_Items[DP].Peaks_Positions_In_Signal_Flags[p] )
	{

	RP.clear();
	// initial point
	RP.Activation_Ids.push_back(p);
	RP.Data_Point_Ids.push_back(DP);

	score = 0;
	Total_Length = 0;
	Data_Items[DP].Peaks_Positions_In_Signal_Flags[p] = true; // visited

	// go forward the catheter
	PP=p;
	Max_LAT_Difference_Crossed=false;
	for(long i=DP;i<(signed)Data_Items.size()-1;i++)
	if( !Max_LAT_Difference_Crossed )
	{
		// find peak position in next electrogram
		min_diff=100000; min_ptr=-1;
		for(long p2=0;p2<Data_Items[i+1].Peaks_Positions_In_Signal.size();p2++)
		if( !Data_Items[i+1].Peaks_Positions_In_Signal_Flags[p2] )
		{
			diff = Data_Items[i+1].Peaks_Positions_In_Signal[p2]-
				   Data_Items[i]->Peaks_Positions_In_Signal[PP];
			if( fabs(diff) < fabs(min_diff) )
			{
				min_diff = diff;
				min_ptr = p2;
			}
		}

		PP = min_ptr;

		if( fabs(min_diff) < Max_LAT_Difference )
		{
			Total_Length += min_diff;
			if( min_diff > 0 )
				score++;
			if( min_diff < 0 )
				score--;
			Data_Items[i+1].Peaks_Positions_In_Signal_Flags[min_ptr] = true;//visited
			RP.Activation_Ids.push_back(min_ptr);
			RP.Data_Point_Ids.push_back(i+1);
		}
		else
		Max_LAT_Difference_Crossed=true;

	} // through all electrodes forward

	// go backward the catheter
	PP=p;
	Max_LAT_Difference_Crossed=false;
	for(long i=DP;i>0;i--)
	if( !Max_LAT_Difference_Crossed )
	{
		// find peak position in next electrogram
		min_diff=100000; min_ptr=-1;
		for(long p2=0;p2<Data_Items[i-1].Peaks_Positions_In_Signal.size();p2++)
		if( !Data_Items[i-1].Peaks_Positions_In_Signal_Flags[p2] )
		{
			diff = Data_Items[i]->Peaks_Positions_In_Signal[PP]-
				   Data_Items[i-1].Peaks_Positions_In_Signal[p2];
			if( fabs(diff) < fabs(min_diff) )
			{
				min_diff = diff;
				min_ptr = p2;
			}
		}

		PP = min_ptr;

		if( fabs(min_diff) < Max_LAT_Difference )
		{
			Total_Length += min_diff;
			if( min_diff > 0 )
				score++;
			if( min_diff < 0 )
				score--;
			Data_Items[i-1].Peaks_Positions_In_Signal_Flags[min_ptr] = true;//visited
			RP.Activation_Ids.push_back(min_ptr);
			RP.Data_Point_Ids.push_back(i-1);
		}
		else
		Max_LAT_Difference_Crossed=true;

	} // through all electrodes forward

	Pivoting_Scores.push_back(fabs(score));
	Pivoting_Events_Lengths.push_back(Data_Items[0]->Time_Step_ms*fabs(Total_Length));
	Pivoting_Events_Start.push_back(Data_Items[0]->Time_Step_ms*Data_Items[DP].Peaks_Positions_In_Signal[p]);
	Conduction_Ripples.push_back(RP);

	} // for all peaks

	//---------------------------------------------------------------
	// 3. CALCULATE CONTINUOUS ACTIVITY FRACTION
	//---------------------------------------------------------------
	std::vector <double> Data_Vec = get_data_for_analysis(0);
	std::vector <int> Occupancy_Vector;
	double v=0;
	Occupancy_Vector.assign(Data_Vec.size(),v);

	for(long d=0;d<(signed)Data_Items.size();d++)
	for(long p=0;p<(signed)Data_Items[d]->Peaks_Positions_In_Signal.size();p++)
	for(long t2= Data_Items[d]->Peaks_Positions_In_Signal[p]-Block_size;
			 t2<=Data_Items[d]->Peaks_Positions_In_Signal[p]+Block_size;t2++)
	if( t2>=0 && t2 < (signed)Occupancy_Vector.size() )
		Occupancy_Vector[t2]=1;

	double SUM=0;
	for(long t=0;t<Occupancy_Vector.size();t++)
	if( Occupancy_Vector[t] == 1 )
		SUM++;

	if( Occupancy_Vector.size() != 0 )
	SUM /= (double) Occupancy_Vector.size();

	Continuous_Activity_Ratio = SUM;

	if( Echo )
	{
	RichEdit_Form->RichEdit1->Clear();
	RichEdit_Form->RichEdit1->Lines[0].Add( "Continuous_Activity_Ratio= "+FloatToStr(Continuous_Activity_Ratio)+"\n\n PIVOTING SCORES:" );
	for(long p=0;p<Pivoting_Scores.size();p++)
	RichEdit_Form->RichEdit1->Lines[0].Add( IntToStr((int)Pivoting_Scores[p]) );
	RichEdit_Form->ShowModal();
	}

	Signals_Display_PaintBoxPaint(this);
*/
}
//---------------------------------------------------------------------------


void __fastcall TNL_Analysis_Form::MultiSCale_Recomp_ButtonClick(TObject *Sender)
{
	double Min =  F1_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item_1]->Time_Step_ms;
	double Max =  F2_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item_1]->Time_Step_ms;
	double Step = F3_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item_1]->Time_Step_ms;
	double v;
	int R,G,B;
	double wx = (double)(MSC_PaintBox->Width) / (double)(Data_Items[Selected_Data_Item_1]->Voltage_Values.size());
	double wy = (double)(MSC_PaintBox->Height) / (Max-Min);
	Number_of_Phase_Transitions.clear();

		NL_Analysis_Form->Butt_band_40_250_3th_order_checkbox->State = cbUnchecked;
		NL_Analysis_Form->Rectification_checkbox->State = cbUnchecked;
		NL_Analysis_Form->Butt_low_20_3th_order_checkbox->State = cbUnchecked;
		NL_Analysis_Form->Normalize__CheckBox->State = cbUnchecked;
		NL_Analysis_Form->Sliding_Window_SD_CheckBox->State = cbUnchecked;
		NL_Analysis_Form->Set_av_SD_CheckBox->State = cbUnchecked;
		NL_Analysis_Form->Sinusoidal_Recomposition_CheckBox->State = cbChecked;
		NL_Analysis_Form->Hilb_Phase_CheckBox->State = cbChecked;

	  MSC_PaintBox->Canvas->Brush->Color=clBlack;
	  MSC_PaintBox->Canvas->FillRect( Rect( 0, 0, MSC_PaintBox->Width,MSC_PaintBox->Height));
	  SCM_Chart->Series[0]->Clear();

	  for(long st=Min; st<Max; st+=Step )
	  {

	  // compute phase
	  Sinsoidal_Wavelet_Period_Edit->Text = IntToStr((int)(st*Data_Items[Selected_Data_Item_1]->Time_Step_ms));
	  filter_current_signal();

	  // plot line
	  for(long t=0; t<Data_Items[Selected_Data_Item_1]->Voltage_Values.size(); t++ )
	  {
	  v = Data_Items[Selected_Data_Item_1]->Filtered_Signal[t];
	  Numerical_Library_Obj.get_rgb_colors_from_ranged_variable(-3.3,3.3,v,&R, &G,&B);
	  MSC_PaintBox->Canvas->Brush->Color=(TColor)( RGB(R,G,B) );
	  MSC_PaintBox->Canvas->FillRect( Rect( t*wx, (st-Min)*wy, (t+1)*wx, (st-Min+Step)*wy ));
	  }

	  // fill histo
	  Det_Peaks_Th_ButtonClick(this);
	  SCM_Chart->Series[0]->AddXY(st*Data_Items[Selected_Data_Item_1]->Time_Step_ms,
			Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.size());
	  Number_of_Phase_Transitions.push_back(
			Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.size());
	  } // for


	double a,b,F,RR;
	Numerical_Library_Obj.fit_line_vertical_offsets_vec(
		&Number_of_Phase_Transitions,&a, &b, &F, &RR);
	/*
	ofstream df("vvv.csv");
	for(long i=0;i<Number_of_Phase_Transitions.size();i++)
	df << Number_of_Phase_Transitions[i] << endl;
	df.close();
	*/

	Direction_Coef_NPT = a;
	SCM_Chart->Title->Caption = "Number of phase transitions (a= " +
		FloatToStrF(a,ffGeneral,3,3)+")";

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::TakeVwavetemplatemorphologycurrentsegment1Click(TObject *Sender)
{

	// Take V wave
	long V_Wave_Start = Raw_Signals_Chart->BottomAxis->Minimum/
				Data_Items[Selected_Data_Item_1]->Time_Step_ms;
	if(V_Wave_Start<0)
		V_Wave_Start = 0;

	long V_Wave_Stop = Raw_Signals_Chart->BottomAxis->Maximum/
				Data_Items[Selected_Data_Item_1]->Time_Step_ms;

	if( V_Wave_Stop <= 0 || V_Wave_Stop > Data_Items[Selected_Data_Item_1]->Voltage_Values.size() )
		V_Wave_Stop = Data_Items[Selected_Data_Item_1]->Voltage_Values.size() - 1;

	V_Wave.clear();
	for(long i=V_Wave_Start;i<=V_Wave_Stop;i++)
	V_Wave.push_back( Data_Items[Selected_Data_Item_1]->Voltage_Values[i] );

	// add V wave dash line to main display
	Raw_Signals_Chart->Series[6]->Clear();

	for(long i=V_Wave_Start;i<=V_Wave_Stop;i++)
	Raw_Signals_Chart->Series[6]->AddXY(i,0);

	Raw_Signals_Chart->UndoZoom();
	Filtered_Signal_Chart->UndoZoom();

//	Data_Items[0].

/*                xxx
	// Create similarity coef std::vector
	double* Sim_Vector = new double[Data_Items[Selected_Data_Item_1]->Filtered_Signal.size() ];
	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Filtered_Signal.size();i++)
	Sim_Vector[i] = 0;

	// Calculate sim std::vector
	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Filtered_Signal.size();i++)
	if( i-0.5*V_Wave_Size >= 0 )
	if( i+0.5*V_Wave_Size < Data_Items[Selected_Data_Item_1]->Filtered_Signal.size() )
	Sim_Vector[i] = std::pow(Numerical_Library_Obj.get_similarity_coefficient(
		&Data_Items[Selected_Data_Item_1]->Filtered_Signal,V_Wave,
		i-0.5*V_Wave_Size,i+0.5*V_Wave_Size,0,V_Wave_Size),2);

	// find peaks in sim std::vector
	std::vector <long> Peaks_Vector;
	Peaks_Vector = Numerical_Library_Obj.calculate_peak_positions
		(Sim_Vector,Data_Items[Selected_Data_Item_1]->Voltage_Values.size(),0.8);

	// plot results
	Wavelet_Spectrum_Chart->Series[1]->Clear();
	Wavelet_Spectrum_Chart->Series[0]->Clear();

	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Voltage_Values.size();i++)
	Wavelet_Spectrum_Chart->Series[0]->AddXY(i,Sim_Vector[i],"",clBlack);

	ofstream df("sig Peaks std::vector.csv");
	for(long i=0;i<Peaks_Vector_Size;i++)
	df << Peaks_Vector[i] << endl;
	df.close();

	ofstream df2("sig sim_vector.csv");
	for(long i=0;i<Data_Items[Selected_Data_Item_1]->Voltage_Values.size();i++)
	df2 << Sim_Vector[i] << endl;
	df2.close();

//	Data_Items[Selected_Data_Item_1]->Wavelet_Spectrum
//
		// plot CWT line
		Wavelet_Spectrum_Chart->Series[0]->Clear();
		for(long k=1;k<Data_Items[Selected_Data_Item_1]->Wavelet_Spectrum.size();k++)
		Wavelet_Spectrum_Chart->Series[0]->AddXY(k*Data_Items[Selected_Data_Item_1]->Time_Step_ms,
			Data_Items[Selected_Data_Item_1]->Wavelet_Spectrum[k],"",clRed);

		// plot markers
		Wavelet_Spectrum_Chart->Series[1]->Clear();
		for(long k=0;k<Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.size();k++)
		Wavelet_Spectrum_Chart->Series[1]->AddXY(
			Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal[k]*
			Data_Items[Selected_Data_Item_1]->Time_Step_ms,0);


	for(long k=0;k<Peaks_Vector.size();k++)
	Wavelet_Spectrum_Chart->Series[1]->AddXY(Peaks_Vector[k],Sim_Vector[Peaks_Vector[k]],"",clGreen);

	Wavelet_Spectrum_Chart->Series[1]->Marks->Visible = true;

//    Signal_PageControl->ActivePageIndex = 0;
//    Analysis_PageControl->ActivePageIndex = 1;

*/

/*
	// remove V waves
	double y1,y2,x1,x2,a,v;
	for(long k=0;k<Peaks_Vector_Size;k++)
	if( Peaks_Vector[k]-0.5*V_Wave_Size >= 0 )
	if( Peaks_Vector[k]+0.5*V_Wave_Size < Data_Items[Selected_Data_Item_1]->Data_X.size() )
	for(long i=Peaks_Vector[k]-0.5*V_Wave_Size;i<Peaks_Vector[k]+0.5*V_Wave_Size;i++)
    {

    if( Filter_Options_Form1->longerpolate_V_Wave_Removal_CheckBox->State == cbChecked )
	{
    x1 = Peaks_Vector[k]-0.5*V_Wave_Size;
    x2 = Peaks_Vector[k]+0.5*V_Wave_Size;
	y1 = Data_Items[Selected_Data_Item_1]->Data_Vector[(long)x1];
	y2 = Data_Items[Selected_Data_Item_1]->Data_Vector[(long)x2];

    if(x2-x1 != 0 )
    a = (y2-y1)/(x2-x1);
    else
    a = 0;

    v = a*(i-x1)+y1;
    }
    else
    v = 0.0;

    Data_Items[Selected_Data_Item_1]->Data_Vector[i] = v;
    Data_Items[Selected_Data_Item_1]->Filtered_Data_Vector[i] = v;
    }
*/
/*
	Raw_Signals_Chart->Series[0]->Clear();
	Raw_Signals_Chart->UndoZoom();
	for(long k=1;k<Data_Items[Selected_Data_Item_1]->Filtered_Data_Length;k++)
		Raw_Signals_Chart->Series[0]->AddXY(k*Data_Items[Selected_Data_Item_1]->Time_Step_ms,
			Data_Items[Selected_Data_Item_1]->Filtered_Data_Vector[k],"",clBlack);
*/
//    delete [] Peaks_Vector;

/*
	ShowMessage("Analysis done.");

	}
	else
	ShowMessage("Select fragment of signal with V wave smaller than 2000 ms");
*/

}
//---------------------------------------------------------------------------


void __fastcall TNL_Analysis_Form::Causality_ButtonClick(TObject *Sender)
{

	long k;
	double Start,Stop;

	if( selected_data_items_in_range() )
	{

	double ts = Data_Items[Selected_Data_Item_1]->Time_Step_ms;

	if( Data_Items[Selected_Data_Item_1]->Time_Step_ms == 0 ||
		Data_Items[Selected_Data_Item_2]->Time_Step_ms == 0 )
	ShowMessage("Time step not set!");
	else
	{

	Start = Data_Items[Selected_Data_Item_1]->Min_Ptr;
	Stop = Data_Items[Selected_Data_Item_1]->Max_Ptr;

	if( Start < 0 || Start >= Data_Items[Selected_Data_Item_1]->Voltage_Values.size() )
		Start = 0;
	if( Stop <= 0 || Stop >= Data_Items[Selected_Data_Item_1]->Voltage_Values.size() )
		Stop = Data_Items[Selected_Data_Item_1]->Voltage_Values.size() - 1;

	std::vector <double> *Data_1;
	if( CSource_RadioGroup->ItemIndex == 0 )
	Data_1 = &Data_Items[Selected_Data_Item_1]->Voltage_Values;
	if( CSource_RadioGroup->ItemIndex == 1 )
	Data_1 = &Data_Items[Selected_Data_Item_1]->Filtered_Signal;

	std::vector <double> *Data_2;
	if( CSource_RadioGroup->ItemIndex == 0 )
	Data_2 = &Data_Items[Selected_Data_Item_2]->Voltage_Values;
	if( CSource_RadioGroup->ItemIndex == 1 )
	Data_2 = &Data_Items[Selected_Data_Item_2]->Filtered_Signal;

	long Delay_Min = CMin_Edit->Text.ToInt()/ts;
	long Delay_Max = CMax_Edit->Text.ToInt()/ts;
	long Delay_Step = CStep_Edit->Text.ToInt()/ts;
	long Basic_Cycle_Length = AFCL_Caus_Edit->Text.ToDouble()/ts;
	long Electrogram_Type = ECTC_RadioGroup->ItemIndex;  // unpiolar

	int Number = (Delay_Max-Delay_Min)/Delay_Step;

	std::vector <double> MPC_Spectrum;
	MPC_Spectrum.clear();
	double tmp=0;
	MPC_Spectrum.assign(2*Number+10,tmp);

	Two_point_Causality_Chart->Series[0]->Clear();

	for(int k=0;k<Number;k++)
	{
	MPC_Spectrum[k+Number/2] = Numerical_Library_Obj.
		get_MPC_between_electrograms(Data_1,Data_2,Basic_Cycle_Length,Electrogram_Type,
			Delay_Min+k*Delay_Step,ts);
	MPC_Spectrum[Number/2-k] = Numerical_Library_Obj.
		get_MPC_between_electrograms(Data_1,Data_2,Basic_Cycle_Length,Electrogram_Type,
			-(Delay_Min+k*Delay_Step),ts );

	Two_point_Causality_Chart->Series[0]->AddXY( ts*(Delay_Min+k*Delay_Step),
												 MPC_Spectrum[k+Number/2]);
	Two_point_Causality_Chart->Series[0]->AddXY( -ts*(Delay_Min+k*Delay_Step),
												 MPC_Spectrum[Number/2-k]);
	}

	}
	}

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::ExportactivationstimesSinRecompHilb1Click(TObject *Sender)

{
	ofstream df;

	// 1. Do all signal processing
	if( MessageDlg("Perform all signal processing steps?",
	mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0) )
	{
		Sinusoidal_Recomposition_CheckBox->State = cbChecked;
		Hilb_Phase_CheckBox->State = cbChecked;
		Filter_All_Signals_ButtonClick(this);
		Det_Peaks_Th_ButtonClick(this);
	}

	if(SaveDialog1->Execute())
	{

	df.open(SaveDialog1->FileName.c_str());

	for(int d=0;d<Data_Items.size();d++)
	{
		df << Data_Items[d]->Name.c_str() << ",";

		for(long i=0;i<Data_Items[d]->Peaks_Positions_In_Signal.size();i++)
			df << Data_Items[d]->Peaks_Positions_In_Signal[i] << ",";

		df << "\n";
	}

	df.close();

	}

	ShowMessage("Exporting activation times done.");
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::MPC_Recalc_ButtonClick(TObject *Sender)
{
	if( selected_data_items_in_range() )
	{

	//---------------------------------------------------------------------
	// 1. Get peak positions of electrogram wiht smallest SD of intervals (most stable)
	//---------------------------------------------------------------------
	double average,SD,min_SD=10000;
	int min_ptr=-1;

	std::vector <long> Diffs;

	for(long i=0;i<Data_Items.size();i++)
	{
		Diffs.clear();
		for(long j=1;j<Data_Items[i]->Peaks_Positions_In_Signal.size();j++)
		Diffs.push_back(Data_Items[i]->Peaks_Positions_In_Signal[j]-
						Data_Items[i]->Peaks_Positions_In_Signal[j-1]);

		Numerical_Library_Obj.stdev_cor_vec_long(&Diffs, &average, &SD);
		if( SD < min_SD )
		{
			min_SD = SD;
			min_ptr = i;
		}
	}

	if( min_ptr >= 0 )
	{

	Data_Item_With_Most_Stable_AFCLs = min_ptr;

	//---------------------------------------------------------------------
	// 2. Main for loop
	//---------------------------------------------------------------------
	MPC_C_Chart1->Series[0]->Clear();
	MPCs_of_Consecutive_Peaks.clear();
	double MPC,TA,TB;
	std::vector <double> Signal_A, Signal_B;


	// go through all peaks
	for(long i=0;i<Data_Items[min_ptr]->Peaks_Positions_In_Signal.size()-1;i++)
	{
		Signal_A.clear();
		Signal_B.clear();
		TA = Data_Items[min_ptr]->Peaks_Positions_In_Signal[i];
		TB = Data_Items[min_ptr]->Peaks_Positions_In_Signal[i+1];

		//----------------------------------
		if( MT1_RadioGroup->ItemIndex == 0 )   // MPC approach
		//----------------------------------
		{
			for(long d=0;d<Data_Items.size();d++)
			{
				Signal_A.push_back( Data_Items[d]->Filtered_Signal[TA]);
				Signal_B.push_back( Data_Items[d]->Filtered_Signal[TB]);
			}

			MPC = Numerical_Library_Obj.get_phase_synchronization(
							&Signal_A,&Signal_B,1,1,false,0);
		}

		//----------------------------------
		if( MT1_RadioGroup->ItemIndex == 1 )  // correlation approach
		//----------------------------------
		{
			// fill signal_a with positions of activaitons in 1st train
			for(long d=0;d<Data_Items.size();d++)
			{
				long min2 = 100000; long min_ptr2 = -1;
				for(long j=0;j<Data_Items[d]->Peaks_Positions_In_Signal.size();j++)
				if( fabs(Data_Items[d]->Peaks_Positions_In_Signal[j] - TA) < min2 )
				{
					min2 = fabs(Data_Items[d]->Peaks_Positions_In_Signal[j] - TA);
					min_ptr2 = j;
				}

				if( min_ptr2 >= 0 )
				Signal_A.push_back( TA - Data_Items[d]->Peaks_Positions_In_Signal[min_ptr2] );
			}

			// fill signal_b with positions of activaitons in 2nd train
			for(long d=0;d<Data_Items.size();d++)
			{
				long min2 = 100000; long min_ptr2 = -1;
				for(long j=0;j<Data_Items[d]->Peaks_Positions_In_Signal.size();j++)
				if( fabs(Data_Items[d]->Peaks_Positions_In_Signal[j] - TB) < min2 )
				{
					min2 = fabs(Data_Items[d]->Peaks_Positions_In_Signal[j] - TB);
					min_ptr2 = j;
				}

				if( min_ptr2 >= 0 )
				Signal_B.push_back( TB - Data_Items[d]->Peaks_Positions_In_Signal[min_ptr2] );
			}

			Numerical_Library_Obj.correlation_coef_vec(&Signal_A,&Signal_B,&MPC);
		}

		MPCs_of_Consecutive_Peaks.push_back(MPC);
		// MPC_C_Chart1->Series[0]->AddY(MPC);
		MPC_C_Chart1->Series[0]->AddXY(TA*Data_Items[Selected_Data_Item_1]->Time_Step_ms,MPC,"",clGreen);

	} // thru all peaks

	// find trains
	double Begin,End;
	Train_Begin_ptr.clear();
	Train_End_ptr.clear();
	Train_Begin_ms.clear();
	Train_Length_ms.clear();
	Train_Mean_Corr.clear();
	Train_SD_Corr.clear();

	std::vector <double> Corrs;
	double average,SDev;

	for(long p=0;p<MPCs_of_Consecutive_Peaks.size();p++)
	{

		if(MPCs_of_Consecutive_Peaks[p] > MPC_Threshold )
		{
			Train_Begin_ptr.push_back(p);

			Begin = Data_Items[min_ptr]->Peaks_Positions_In_Signal[p]*Data_Items[Selected_Data_Item_1]->Time_Step_ms;
			Train_Begin_ms.push_back( Begin );

			Corrs.clear();
			p++;
			while(MPCs_of_Consecutive_Peaks[p] > MPC_Threshold && p < MPCs_of_Consecutive_Peaks.size() )
			{
				Corrs.push_back(MPCs_of_Consecutive_Peaks[p]);
				p++;
			}

			Train_End_ptr.push_back(p);
			End = Data_Items[min_ptr]->Peaks_Positions_In_Signal[p]*Data_Items[Selected_Data_Item_1]->Time_Step_ms;
			Train_Length_ms.push_back(End - Begin );

			Numerical_Library_Obj.stdev_cor_vec_double(&Corrs,&average, &SDev);
			Train_Mean_Corr.push_back(average);
			Train_SD_Corr.push_back(SDev);
		}
	}

	} // base signal found
    }
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::PreprocessandreclaclMPC_ButtonClick(TObject *Sender)

{
		NL_Analysis_Form->Butt_band_40_250_3th_order_checkbox->State = cbUnchecked;
		NL_Analysis_Form->Rectification_checkbox->State = cbUnchecked;
		NL_Analysis_Form->Butt_low_20_3th_order_checkbox->State = cbUnchecked;
		NL_Analysis_Form->Normalize__CheckBox->State = cbUnchecked;
		NL_Analysis_Form->Sliding_Window_SD_CheckBox->State = cbUnchecked;
		NL_Analysis_Form->Set_av_SD_CheckBox->State = cbUnchecked;
		NL_Analysis_Form->Sinsoidal_Wavelet_Period_Edit->Text = "180";
		NL_Analysis_Form->Sinusoidal_Recomposition_CheckBox->State = cbChecked;
		NL_Analysis_Form->Hilb_Phase_CheckBox->State = cbChecked;
		NL_Analysis_Form->Filter_All_Signals_ButtonClick(this);
		NL_Analysis_Form->Data_Source_RadioGroup->ItemIndex = 1;

		NL_Analysis_Form->PDT_SignalMode_RadioGroup->ItemIndex = 1; // derivative
		NL_Analysis_Form->PDT_Edit->Text = "0.1";
		NL_Analysis_Form->PD_Blanking_Edit->Text = "80";
		NL_Analysis_Form->PDTW_Edit->Text = "500";
		NL_Analysis_Form->Det_Peaks_Th_ButtonClick(this);
		NL_Analysis_Form->PercOcc_Rec_ButtonClick(this);

		NL_Analysis_Form->MPC_Threshold = 0.85;
		NL_Analysis_Form->MPC_Recalc_ButtonClick(this);

//		NL_Analysis_Form->Sinusoidal_Recomposition_CheckBox->State = cbUnchecked;
//		NL_Analysis_Form->Hilb_Phase_CheckBox->State = cbUnchecked;

}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Self_Conv_ButtonClick(TObject *Sender)
{
	if( selected_data_items_in_range() )
	{

	double Min =  F1_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item_1]->Time_Step_ms;
	double Max =  F2_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item_1]->Time_Step_ms;
	double Step = F3_Edit->Text.ToDouble()/Data_Items[Selected_Data_Item_1]->Time_Step_ms;

	Number_of_Phase_Transitions.clear();

	NL_Analysis_Form->Butt_band_40_250_3th_order_checkbox->State = cbUnchecked;
	NL_Analysis_Form->Rectification_checkbox->State = cbUnchecked;
	NL_Analysis_Form->Butt_low_20_3th_order_checkbox->State = cbUnchecked;
	NL_Analysis_Form->Normalize__CheckBox->State = cbUnchecked;
	NL_Analysis_Form->Sliding_Window_SD_CheckBox->State = cbUnchecked;
	NL_Analysis_Form->Set_av_SD_CheckBox->State = cbUnchecked;
	NL_Analysis_Form->Sinusoidal_Recomposition_CheckBox->State = cbChecked;
	NL_Analysis_Form->Hilb_Phase_CheckBox->State = cbChecked;

	//-------------------------------------------------------------------------
	// SELF CONVERGENCE CALCULATIONS
	//-------------------------------------------------------------------------
	TChartSeries *LineSeries[100];
	int Counter=0;

/*
	Self_Convergence_Chart->RemoveAllSeries();
	for(long st=Min; st<Max; st+=Step )
	{
		LineSeries[0]= new TChartSeries(Self_Convergence_Chart);
		Self_Convergence_Chart->AddSeries( LineSeries[0] );
	}
*/
	int Convergence_Steps = CnvST_Edit->Text.ToInt();
	double Period;
	std::vector <double> Convergent_Values;

	for(long st=Min; st<Max; st+=Step ) // unit: []
	{

	  Period = st; // initial period, unit: []

	  Self_Convergence_Chart->Series[Counter]->Clear();
	  Self_Convergence_Chart->Series[Counter]->AddXY(0,Period*Data_Items[Selected_Data_Item_1]->Time_Step_ms);

	  for(int S=1;S<=Convergence_Steps;S++)
	  {
		  // compute phase
		  Sinsoidal_Wavelet_Period_Edit->Text = IntToStr((int)(Period*Data_Items[Selected_Data_Item_1]->Time_Step_ms));
		  filter_current_signal();

		  // get AFCL
		  detect_peaks_in_signal(Selected_Data_Item_1);
		  Period = Data_Items[Selected_Data_Item_1]->Average_ISI/Data_Items[Selected_Data_Item_1]->Time_Step_ms; // unit: []

		  // plot a point
		  Self_Convergence_Chart->Series[Counter]->AddXY(S,Period*Data_Items[Selected_Data_Item_1]->Time_Step_ms);
	  }

	  Convergent_Values.push_back(Period*Data_Items[Selected_Data_Item_1]->Time_Step_ms);
	  Counter++;
	}


	// now select the ultimate period from the list
	double Ultimate_Period,mean,SD;

	Ultimate_Period = Numerical_Library_Obj.get_median(Convergent_Values);
	Numerical_Library_Obj.stdev_cor_vec(&Convergent_Values,
			  &mean,&SD);

	Data_Items[Selected_Data_Item_1]->Mean_AFCL_From_Convergent_Sinusoidal_Recomposition = Ultimate_Period;
	Data_Items[Selected_Data_Item_1]->Mean_AFCL_From_Convergent_Sinusoidal_Recomposition_SD = SD;

	}
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::OneStarting_convergentSR_ButtonClick(TObject *Sender)
{
	if( selected_data_items_in_range() )
	{

	Number_of_Phase_Transitions.clear();
	NL_Analysis_Form->Butt_band_40_250_3th_order_checkbox->State = cbUnchecked;
	NL_Analysis_Form->Rectification_checkbox->State = cbUnchecked;
	NL_Analysis_Form->Butt_low_20_3th_order_checkbox->State = cbUnchecked;
	NL_Analysis_Form->Normalize__CheckBox->State = cbUnchecked;
	NL_Analysis_Form->Sliding_Window_SD_CheckBox->State = cbUnchecked;
	NL_Analysis_Form->Set_av_SD_CheckBox->State = cbUnchecked;
	NL_Analysis_Form->Sinusoidal_Recomposition_CheckBox->State = cbChecked;
	NL_Analysis_Form->Hilb_Phase_CheckBox->State = cbChecked;

	NL_Analysis_Form->Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.clear();

	//-------------------------------------------------------------------------
	// SELF CONVERGENCE CALCULATIONS
	//-------------------------------------------------------------------------
// Data_Items[0]->Time_Step_ms

	double Start = SPC_Edit->Text.ToDouble(); // unit: ms
	int Convergence_Steps = CnvST_Edit->Text.ToInt();
	double Period,Period_SD;

	Period = Start; // initial period, unit: ms

	Self_Convergence_Chart->Series[0]->Clear();
	Self_Convergence_Chart->Series[0]->AddXY(0,Period);

	for(int i=1;i<Self_Convergence_Chart->SeriesCount();i++)
	Self_Convergence_Chart->Series[i]->Clear();

	for(int S=1;S<=Convergence_Steps;S++)
	{
		  // compute phase
		  Sinsoidal_Wavelet_Period_Edit->Text = FloatToStr(Period);
		  filter_current_signal();

		  // get AFCL
		  Data_Source_RadioGroup->ItemIndex = 1; // set filtered as a source
		  detect_peaks_in_signal(Selected_Data_Item_1);
		  Period = Data_Items[Selected_Data_Item_1]->Average_ISI; // unit: ms
		  Period_SD = Data_Items[Selected_Data_Item_1]->SD_of_ISI; // unit: ms
// xxx
		  // plot a point
		  Self_Convergence_Chart->Series[0]->AddXY(S,Period);
	}

	Data_Items[Selected_Data_Item_1]->Mean_AFCL_From_Convergent_Sinusoidal_Recomposition = Period;
	Data_Items[Selected_Data_Item_1]->Mean_AFCL_From_Convergent_Sinusoidal_Recomposition_SD = Period_SD;

	Sinsoidal_Wavelet_Period_Edit->Text = FloatToStr(Data_Items[Selected_Data_Item_1]->Mean_AFCL_From_Convergent_Sinusoidal_Recomposition);

	filter_current_signal();
	// Filter_All_Signals_ButtonClick(this);
	detect_peaks_in_signal(Selected_Data_Item_1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Black_Markers_CheckBoxClick(TObject *Sender)
{
	Signals_Display_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::MCP_CheckBoxClick(TObject *Sender)
{
		Signals_Display_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Synch_Movie_Pause_ButtonClick(TObject *Sender)
{
	Playing_Synch_Movie = false;
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Synch_Movie_Start_ButtonClick(TObject *Sender)

{
	Playing_Synch_Movie = true;

	double WL = R11Edit->Text.ToInt();

	while( Playing_Synch_Movie )
	{
		Raw_Signals_Chart->BottomAxis->Minimum += WL/Data_Items[Selected_Data_Item_1]->Time_Step_ms;
		Raw_Signals_Chart->BottomAxis->Maximum += WL/Data_Items[Selected_Data_Item_1]->Time_Step_ms;

		if( Raw_Signals_Chart->BottomAxis->Maximum > Raw_Signals_Chart->Series[0]->Count() )
		Playing_Synch_Movie = false;

		recalculate_current_tab();

		repaint_current_tab();

		Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::TPMPC_ButtonClick(TObject *Sender)
{
/*
	long k;
	double Start;
	std::vector <double> Data_1;
	std::vector <double> Data_2;

	if( selected_data_items_in_range() )
	{

	double ts = Data_Items[Selected_Data_Item_1]->Time_Step_ms;
	double W_Size = WS1_Edit->Text.ToDouble() / ts;
	double W_Step = WS2_Edit->Text.ToDouble() / ts;
	long Basic_Cycle_Length = AFCL_Caus_Edit->Text.ToDouble()/ts;
	long Electrogram_Type = ECTC_RadioGroup->ItemIndex;  // unpiolar

	if( Data_Items[Selected_Data_Item_1]->Time_Step_ms == 0 ||
		Data_Items[Selected_Data_Item_2]->Time_Step_ms == 0 )
	ShowMessage("Time step not set!");
	else
	{

	Start = Data_Items[Selected_Data_Item_1]->Min_Ptr;

	long Spectrum_Size = Data_Items[Selected_Data_Item_1]->Filtered_Signal.size()/W_Size;
	std::vector <double> MPC_Spectrum;
	MPC_Spectrum.clear();
	double tmp=0;
	MPC_Spectrum.assign(Spectrum_Size,tmp);

	MPC_C_Chart1->Series[0]->Clear();
	CL_CL_MPC_Form->MPC_Chart->Series[0]->Clear();

	for(int k=0;k<Data_Items[Selected_Data_Item_1]->Filtered_Signal.size()/W_Step-1;k++)
	{

	Start = k*W_Step;

	Data_1.clear();
	Data_2.clear();

	for(long t=0;t<W_Size;t++)
	{
		Data_1.push_back( Data_Items[Selected_Data_Item_1]->Filtered_Signal[Start+t] );
		Data_2.push_back( Data_Items[Selected_Data_Item_2]->Filtered_Signal[Start+t] );
	}

	MPC_Spectrum[k] = Numerical_Library_Obj.
		get_MPC_between_electrograms(&Data_1,&Data_2,Basic_Cycle_Length,Electrogram_Type,0,ts);

	MPC_C_Chart1->Series[0]->AddXY( (Start+0.5*W_Step)*ts, MPC_Spectrum[k]);
	CL_CL_MPC_Form->MPC_Chart->Series[0]->AddXY( (Start+0.5*W_Step)*ts, MPC_Spectrum[k]);

	}

	}
	}
*/
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::ExportintervalsSinRecompHilbofalldatapoints1Click(TObject *Sender)

{
	ofstream df;

	// 1. Do all signal processing
	if( MessageDlg("Perform all signal processing steps?",
	mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0) )
	{
		Sinusoidal_Recomposition_CheckBox->State = cbChecked;
		Hilb_Phase_CheckBox->State = cbChecked;
		Filter_All_Signals_ButtonClick(this);
		Det_Peaks_Th_ButtonClick(this);
	}

	if(SaveDialog1->Execute())
	{

	df.open(SaveDialog1->FileName.c_str());

	for(int d=0;d<Data_Items.size();d++)
	{
		df << Data_Items[d]->Name.c_str() << ",";

		for(long i=1;i<Data_Items[d]->Peaks_Positions_In_Signal.size();i++)
			df << Data_Items[d]->Peaks_Positions_In_Signal[i]-
				  Data_Items[d]->Peaks_Positions_In_Signal[i-1] << ",";

		df << "\n";
	}

	df.close();

	}

	ShowMessage("Exporting intervals done.");

}
//---------------------------------------------------------------------------


void __fastcall TNL_Analysis_Form::CAR_ButtonClick(TObject *Sender)
{
	if( Data_Items.size() > 0 )
	{

	long Sliding_window_size = SLWD_Edit->Text.ToDouble()/Data_Items[0]->Time_Step_ms;
	double Threshold = EST_Edit->Text.ToDouble();

	long EGM_Length = Data_Items[0]->Voltage_Values.size();
	Silence_Vector.clear();
	int v=0;
	Silence_Vector.assign(EGM_Length,v); // 0 - no silence, 1 - silence in all EGMS
	bool Threshold_Crossed;
	double Min,Max,Mean,SDev;

	//-------------------------------------------------------------------
	for(long t=0.5*Sliding_window_size;t<EGM_Length-0.5*Sliding_window_size;t++)
	//-------------------------------------------------------------------
	{

	Threshold_Crossed = false;

	//---------------------------------------------------------------
	for(long d=0;d<(signed)Data_Items.size();d++)
	//---------------------------------------------------------------
	{
		// get mean in current window
		Numerical_Library_Obj.calculate_max_min_mean_vec_ranged(
			&Data_Items[d]->Voltage_Values,t-0.5*Sliding_window_size, t+0.5*Sliding_window_size,
				&Min, &Max, &Mean,&SDev);
		if( Max > Threshold || Min < -Threshold)
			Threshold_Crossed = true;

	} // thorugh all egms

	if(!Threshold_Crossed)
	for(long tt= t-0.5*Sliding_window_size;tt<=t+0.5*Sliding_window_size;tt++)
		Silence_Vector[tt]=1;

	} // through all time points


	double SUM=0;
	for(long t=0;t<Silence_Vector.size();t++)
	if( Silence_Vector[t] == 1 )
		SUM++;

	if( Silence_Vector.size() != 0 )
	SUM /= (double) Silence_Vector.size();

	Continuous_Activity_Ratio = 1-SUM;

	COR_Label->Caption = "Continuous_Activity_Ratio= "+FloatToStr(Continuous_Activity_Ratio);
	Signals_Display_PaintBoxPaint(this);

	}
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Repaint_All_Signals_ButtonClick(TObject *Sender)
{
	Signals_Display_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::First_Derivative_CheckBoxClick(TObject *Sender)

{
    if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
    Filter_All_Signals_ButtonClick(this);
    else
	filter_current_signal();
	repaint_current_tab();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Second_Derivative_CheckBoxClick(TObject *Sender)

{
    if( FilterAllSigs_byDefault_CheckBox->State == cbChecked )
    Filter_All_Signals_ButtonClick(this);
    else
	filter_current_signal();
	repaint_current_tab();
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::Hist_Outliers_Remove_CheckBoxClick(TObject *Sender)

{
	Histo_Recalculate_ButtonClick(this);
}
//---------------------------------------------------------------------------


void __fastcall TNL_Analysis_Form::ShSD_CheckBoxClick(TObject *Sender)
{
	Histo_Recalculate_ButtonClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::CalculateTimeShiftHist_ButtonClick(TObject *Sender)
{
	if( selected_data_items_in_range() )
	{

	double mean;

	Data_Source_RadioGroup->ItemIndex = 2; // peak positions

	std::vector <double> Data_Vec_1;
	Data_Vec_1.clear();
	Data_Vec_1 = get_data_for_analysis(Selected_Data_Item_1);

	std::vector <double> Data_Vec_2;
	Data_Vec_2.clear();
	Data_Vec_2 = get_data_for_analysis(Selected_Data_Item_2);

	double OutlierTheshold_ptr = OutThEdit->Text.ToDouble();
	OutlierTheshold_ptr = OutlierTheshold_ptr / Data_Items[Selected_Data_Item_1]->Time_Step_ms;

	if( Data_Vec_1.size() > 3 && Data_Vec_2.size() > 3)
	{

	std::vector <double> TimeShifts_All;
	std::vector <double> TimeShifts;
	std::vector <bool> TimeShifts_Outlier_Flag;

	double diff,min_diff;
	for(long i1=0;i1<Data_Vec_1.size();i1++)
	{
		min_diff = 100000;
		for(long i2=0;i2<Data_Vec_2.size();i2++)
		{
			diff = Data_Vec_2[i2] - Data_Vec_1[i1];
			if( fabs(diff) < fabs(min_diff) )
				min_diff = diff;
		}
		TimeShifts_All.push_back(min_diff*Data_Items[Selected_Data_Item_1]->Time_Step_ms);

		if( fabs(min_diff) < OutlierTheshold_ptr )
		TimeShifts_Outlier_Flag.push_back(false);
		else
		TimeShifts_Outlier_Flag.push_back(true);
	}

	// prepare std::vector of time shifts within outlier
	for(long t=0;t<TimeShifts_All.size();t++)
	if( !TimeShifts_Outlier_Flag[t] )
	TimeShifts.push_back(TimeShifts_All[t]);

	Numerical_Library_Obj.calculate_max_min_mean_vec_double(&TimeShifts,
				&Histo_Min, &Histo_Max, &mean);
	Histogram_Data.clear();
	Histogram_Data = Numerical_Library_Obj.get_histogram(&TimeShifts,
		Histo_Bin_Size,ShSD_CheckBox->State,&Histo_Bin_Size);

	// plot histogram
	TimeShifts_Chart->Series[0]->Clear();
	for(long k=0;k<Histogram_Data.size();k++)
	if( Histo_Min+k*Histo_Bin_Size > 0 )
	TimeShifts_Chart->Series[0]->AddXY(Histo_Min+k*Histo_Bin_Size,Histogram_Data[k],"",clRed );
	else
	TimeShifts_Chart->Series[0]->AddXY(Histo_Min+k*Histo_Bin_Size,Histogram_Data[k],"",clBlue );

	// plot dynamic time shifts
	TimeShiftsDynamicChart->Series[0]->Clear();
	for(long k=0;k<TimeShifts_All.size();k++)
	if( !TimeShifts_Outlier_Flag[k] )
	{
		if( TimeShifts_All[k] > 0 )
			TimeShiftsDynamicChart->Series[0]->AddXY(Data_Vec_1[k]*Data_Items[Selected_Data_Item_1]->Time_Step_ms,
				TimeShifts_All[k],"",clRed );
		else
			TimeShiftsDynamicChart->Series[0]->AddXY(Data_Vec_1[k]*Data_Items[Selected_Data_Item_1]->Time_Step_ms,
				TimeShifts_All[k],"",clBlue );
	}

	LA_faster_Than_RA=0;

	for(long k=0;k<TimeShifts.size();k++)
	if( TimeShifts[k] > 0 )
	LA_faster_Than_RA++;

	LA_faster_Than_RA = 100.*LA_faster_Than_RA/(double)TimeShifts.size();

	TimeShiftsRichEdit->Lines[0].Clear();
	TimeShiftsRichEdit->Lines[0].Add("% of times LA is earlier than RA = "+FloatToStrF(LA_faster_Than_RA,ffGeneral,3,2));

	} // if data std::vector not empty

	}
}
//---------------------------------------------------------------------------

void __fastcall TNL_Analysis_Form::SelfConvergenceCurrentButtonClick(TObject *Sender)
{
	if( selected_data_items_in_range() )
	{

	Number_of_Phase_Transitions.clear();
	NL_Analysis_Form->Butt_band_40_250_3th_order_checkbox->State = cbUnchecked;
	NL_Analysis_Form->Rectification_checkbox->State = cbUnchecked;
	NL_Analysis_Form->Butt_low_20_3th_order_checkbox->State = cbUnchecked;
	NL_Analysis_Form->Normalize__CheckBox->State = cbUnchecked;
	NL_Analysis_Form->Sliding_Window_SD_CheckBox->State = cbUnchecked;
	NL_Analysis_Form->Set_av_SD_CheckBox->State = cbUnchecked;
	NL_Analysis_Form->Sinusoidal_Recomposition_CheckBox->State = cbChecked;
	NL_Analysis_Form->Hilb_Phase_CheckBox->State = cbChecked;

	NL_Analysis_Form->Data_Items[Selected_Data_Item_1]->Peaks_Positions_In_Signal.clear();

	NL_Analysis_Form->PDT_SignalMode_RadioGroup->ItemIndex = 1; // slope detection

	//-------------------------------------------------------------------------
	// SELF CONVERGENCE CALCULATIONS
	//-------------------------------------------------------------------------
// Data_Items[0]->Time_Step_ms

	double Start = SPC_Edit->Text.ToDouble(); // unit: ms
	int Convergence_Steps = CnvST_Edit->Text.ToInt();
	double Period,Period_SD;

	Period = Start; // initial period, unit: ms

	Self_Convergence_Chart->Series[0]->Clear();
	Self_Convergence_Chart->Series[0]->AddXY(0,Period);

	for(int i=1;i<Self_Convergence_Chart->SeriesCount();i++)
	Self_Convergence_Chart->Series[i]->Clear();

	for(int S=1;S<=Convergence_Steps;S++)
	{
		  // compute phase
		  Sinsoidal_Wavelet_Period_Edit->Text = FloatToStr(Period);
		  filter_current_signal();

		  // get AFCL
		  Data_Source_RadioGroup->ItemIndex = 1; // set filtered as a source
		  detect_peaks_in_signal(Selected_Data_Item_1);
		  Period = Data_Items[Selected_Data_Item_1]->Average_ISI; // unit: ms
		  Period_SD = Data_Items[Selected_Data_Item_1]->SD_of_ISI; // unit: ms

		  // plot a point
		  Self_Convergence_Chart->Series[0]->AddXY(S,Period);
	}

	Data_Items[Selected_Data_Item_1]->Mean_AFCL_From_Convergent_Sinusoidal_Recomposition = Period;
	Data_Items[Selected_Data_Item_1]->Mean_AFCL_From_Convergent_Sinusoidal_Recomposition_SD = Period_SD;
	Sinsoidal_Wavelet_Period_Edit->Text = FloatToStr(Data_Items[Selected_Data_Item_1]->Mean_AFCL_From_Convergent_Sinusoidal_Recomposition);

	}
}
//---------------------------------------------------------------------------

void TNL_Analysis_Form::reset_and_clear_displays()
{
	Raw_Signals_Chart->UndoZoom();
	Filtered_Signal_Chart->UndoZoom();
}
//---------------------------------------------------------------------------


