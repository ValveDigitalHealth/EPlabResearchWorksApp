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
#include "Progress_Bar_Form.h"

#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProgress_Form *Progress_Form;
//---------------------------------------------------------------------------
__fastcall TProgress_Form::TProgress_Form(TComponent* Owner)
	: TForm(Owner)
{
	Cancel_Flag = false;
}
//---------------------------------------------------------------------------
void __fastcall TProgress_Form::FormResize(TObject *Sender)
{
/*
	RichEditMain->Left = 5;
	RichEditMain->Top = 5;
	RichEditMain->Width = Progress_Form->Width - 10;
	RichEditMain->Height = 0.9*RichEditMain->Height - Progress_Form->Top;

	Cancel_Button->Left = 5;
	Cancel_Button->Top = RichEditMain->Top + RichEditMain->Height + 5;
	Cancel_Button->Width = RichEditMain->Width;
	Cancel_Button->Height = 0.1*RichEditMain->Height-10;
*/
}
//---------------------------------------------------------------------------

void __fastcall TProgress_Form::Cancel_ButtonClick(TObject *Sender)
{
//	Cancel_Flag = true;
    ModalResult = mrOk;
	Progress_Form->Hide();
}
//---------------------------------------------------------------------------

void TProgress_Form::add_text(AnsiString Line)
{
//	end_time = begin_time;
//	begin_time = std::chrono::steady_clock::now();

//	RichEditMain->Lines[0].Add(Line + "   (last operation: " + FloatToStr(std::chrono::duration_cast<std::chrono::milliseconds>(begin_time-end_time).count()) + " ms)");

	RichEditMain->Lines[0].Add(Line);

	SendMessage(RichEditMain->Handle,WM_VSCROLL, SB_BOTTOM, 0);
}

//---------------------------------------------------------------------------

void TProgress_Form::replace_last_line_with_text(AnsiString Line)
{
	RichEditMain->Lines[0].Delete(RichEditMain->Lines[0].Count - 1 );
	RichEditMain->Lines[0].Add(Line);

	SendMessage(RichEditMain->Handle,WM_VSCROLL, SB_BOTTOM, 0);
}

//---------------------------------------------------------------------------

void TProgress_Form::clear()
{
	RichEditMain->Lines[0].Clear();
}

//---------------------------------------------------------------------------

