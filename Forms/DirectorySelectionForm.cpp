//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DirectorySelectionForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDirectory_Selection_Form *Directory_Selection_Form;
//---------------------------------------------------------------------------
__fastcall TDirectory_Selection_Form::TDirectory_Selection_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDirectory_Selection_Form::Close_ButtonClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TDirectory_Selection_Form::Cancel_ButtonClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
