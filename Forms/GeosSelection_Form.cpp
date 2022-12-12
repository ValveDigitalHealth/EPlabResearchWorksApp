//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GeosSelection_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGeos_Selection_Form *Geos_Selection_Form;
//---------------------------------------------------------------------------
__fastcall TGeos_Selection_Form::TGeos_Selection_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGeos_Selection_Form::Proceed_ButtonClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TGeos_Selection_Form::Cancel_ButtonClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
