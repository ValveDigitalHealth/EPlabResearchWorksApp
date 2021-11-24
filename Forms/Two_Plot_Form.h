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

#ifndef Two_Plot_FormH
#define Two_Plot_FormH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>

#include <Jpeg.hpp>

//---------------------------------------------------------------------------
class TTwoPlot_Form : public TForm
{
__published:	// IDE-managed Components
    TChart *First_Chart;
    TLineSeries *Series1;
    TLineSeries *Series2;
    TButton *Close_Button;
	TLineSeries *Series3;
	TLineSeries *Series4;
	TLineSeries *Series5;
	TLineSeries *Series6;
	TLineSeries *Series7;
	TLineSeries *Series8;
	TLineSeries *Series9;
	TLineSeries *Series10;
	TLineSeries *Series21;
	TChart *Second_Chart;
	TLineSeries *LineSeries1;
	TLineSeries *LineSeries2;
	TLineSeries *LineSeries3;
	TLineSeries *LineSeries4;
	TLineSeries *LineSeries5;
	TLineSeries *LineSeries6;
	TLineSeries *LineSeries7;
	TLineSeries *LineSeries8;
	TLineSeries *LineSeries9;
	TLineSeries *LineSeries10;
	TLineSeries *LineSeries11;
	TPointSeries *Series11;
	TPointSeries *Series12;
	TPointSeries *Series13;
	TPointSeries *Series14;
	TPointSeries *Series15;
	TPointSeries *Series16;
	TChart *One_Line_Chart_Top;
	TLineSeries *Series17;
	TLineSeries *Series18;
	TLineSeries *Series19;
	TChart *One_Line_Chart_Bottom;
	TLineSeries *LineSeries12;
	TLineSeries *LineSeries13;
	TLineSeries *LineSeries14;
    void __fastcall Close_ButtonClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TTwoPlot_Form(TComponent* Owner);

    void save_window_to_jpg(AnsiString Filename);
};
//---------------------------------------------------------------------------
extern PACKAGE TTwoPlot_Form *TwoPlot_Form;
//---------------------------------------------------------------------------
#endif
