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

#include <vcl.h>
#pragma hdrstop

#include "SinglePlot_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSingle_Plot_Form *Single_Plot_Form;
//---------------------------------------------------------------------------
__fastcall TSingle_Plot_Form::TSingle_Plot_Form(TComponent* Owner)
	: TForm(Owner)
{
	Sampling_Rate = 2034;
	Marked_Time = -1;
}
//---------------------------------------------------------------------------
void __fastcall TSingle_Plot_Form::Close_ButtonClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TSingle_Plot_Form::FormResize(TObject *Sender)
{
	Chart1->Left = 1;
	Chart1->Top = 1;

	Chart1->Width = Single_Plot_Form->Width-5;
	Chart1->Height = Single_Plot_Form->Height-100-Close_Button->Height;

	Close_Button->Left = 1;
	Close_Button->Top = Chart1->Top + Chart1->Height + 2;

	Mark_and_close_Button->Left = Close_Button->Left + Close_Button->Width + 15;
	Mark_and_close_Button->Top = Close_Button->Top;
}
//---------------------------------------------------------------------------

void __fastcall TSingle_Plot_Form::Mark_and_close_ButtonClick(TObject *Sender)
{
	// chart x axis is in seconds
	Marked_Time = Chart1->BottomAxis->Minimum*Sampling_Rate;

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

