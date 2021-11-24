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

#include "StringGridForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStringGrid_Form *StringGrid_Form;
//---------------------------------------------------------------------------
__fastcall TStringGrid_Form::TStringGrid_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TStringGrid_Form::Close_ButtonClick(TObject *Sender)
{
//StringGrid1->move
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TStringGrid_Form::StringGrid1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  TGridRect T;
  T.Left = 0;
  T.Right = StringGrid1->ColCount;
  T.Top = 0;
  T.Bottom = StringGrid1->RowCount;

  StringGrid1->Selection = T;

  if (Key == 'C' && Shift.Contains(ssCtrl))
  {
    UT.CopyCells_From_StringGrid_ToClipboard(
      *StringGrid1,
      StringGrid1->Selection
      );
  }

}
//---------------------------------------------------------------------------
void __fastcall TStringGrid_Form::CopyTableHButtonClick(TObject *Sender)
{
  TGridRect T;

  T.Left = 0;
  T.Right = StringGrid1->ColCount-1;
  T.Top = 0;
  T.Bottom = StringGrid1->RowCount;

  StringGrid1->Selection = T;

  UT.CopyCells_From_StringGrid_ToClipboard(*StringGrid1,StringGrid1->Selection);

}
//---------------------------------------------------------------------------
void __fastcall TStringGrid_Form::CopyTableButtonClick(TObject *Sender)
{
  TGridRect T;

  T.Left = 1;
  T.Right = StringGrid1->ColCount-1;
  T.Top = 1;
  T.Bottom = StringGrid1->RowCount;

  StringGrid1->Selection = T;

  UT.CopyCells_From_StringGrid_ToClipboard(*StringGrid1,StringGrid1->Selection);

}
//---------------------------------------------------------------------------
void __fastcall TStringGrid_Form::FormResize(TObject *Sender)
{
	StringGrid1->Width = StringGrid_Form->Width-20;
	StringGrid1->Height = StringGrid_Form->Height - Close_Button->Height - 60;

	StringGrid_Form->StringGrid1->DefaultColWidth =
		((double)StringGrid_Form->Width-20.) / (double)StringGrid_Form->StringGrid1->ColCount-10;

	CopyTableButton->Top = StringGrid1->Top + StringGrid1->Height + 10;
	CopyTableHButton->Top = StringGrid1->Top + StringGrid1->Height + 10;
	Close_Button->Top = StringGrid1->Top + StringGrid1->Height + 10;

}
//---------------------------------------------------------------------------
void __fastcall TStringGrid_Form::FormShow(TObject *Sender)
{

   if( StringGrid1->ColCount != 0 )
   for(int c=0;c<StringGrid1->ColCount;c++)
   StringGrid1->ColWidths[c] = (int)(StringGrid1->Width / (StringGrid1->ColCount) - 10 );

}
//---------------------------------------------------------------------------

