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

#include "Display_Options_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TDisplay_Options_Form_1 *Display_Options_Form_1;

//---------------------------------------------------------------------------
__fastcall TDisplay_Options_Form_1::TDisplay_Options_Form_1(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TDisplay_Options_Form_1::OK_ButtonClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------


void __fastcall TDisplay_Options_Form_1::FormResize(TObject *Sender)
{
    Display_Options_Form_1->Width = OK_Button->Left + OK_Button->Width + 80;
    Display_Options_Form_1->Height = OK_Button->Top + OK_Button->Height + 80;
}
//---------------------------------------------------------------------------

void __fastcall TDisplay_Options_Form_1::Fixed_Palette_Range_CheckBoxClick(TObject *Sender)

{
//	Fixed_Palette_Range_Min_Edit->Text = "0.5";
//	Fixed_Palette_Range_Max_Edit->Text = "1.5";

}
//---------------------------------------------------------------------------

void __fastcall TDisplay_Options_Form_1::P1_Limits_ButtonClick(TObject *Sender)

{
	Fixed_Palette_Range_Min_Edit->Text = "0.2";
	Fixed_Palette_Range_Max_Edit->Text = "1.0";
	Fixed_Palette_Range_CheckBox->State = cbChecked;
}
//---------------------------------------------------------------------------

void __fastcall TDisplay_Options_Form_1::P2_Limits_ButtonClick(TObject *Sender)
{
	Fixed_Palette_Range_Min_Edit->Text = "0.5";
	Fixed_Palette_Range_Max_Edit->Text = "1.5";
	Fixed_Palette_Range_CheckBox->State = cbChecked;
}
//---------------------------------------------------------------------------

void __fastcall TDisplay_Options_Form_1::Cancel_ButtonClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------






