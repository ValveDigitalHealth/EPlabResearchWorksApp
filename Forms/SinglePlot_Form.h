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

#ifndef SinglePlot_FormH
#define SinglePlot_FormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>

//---------------------------------------------------------------------------
class TSingle_Plot_Form : public TForm
{
__published:	// IDE-managed Components
	TChart *Chart1;
	TButton *Close_Button;
	TLineSeries *Series2;
	TLineSeries *Series1;
	TLineSeries *Series3;
	TLineSeries *Series4;
	TLineSeries *Series5;
	TLineSeries *Series6;
	TLineSeries *Series7;
	TLineSeries *Series8;
	TLineSeries *Series9;
	TLineSeries *Series10;
	TLineSeries *Series11;
	TLineSeries *Series12;
	TButton *Mark_and_close_Button;
	TLineSeries *Series13;
	void __fastcall Close_ButtonClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Mark_and_close_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TSingle_Plot_Form(TComponent* Owner);

	double Sampling_Rate;
    long Marked_Time;
};
//---------------------------------------------------------------------------
extern PACKAGE TSingle_Plot_Form *Single_Plot_Form;
//---------------------------------------------------------------------------
#endif
