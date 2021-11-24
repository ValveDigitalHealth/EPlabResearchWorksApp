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

#include "selection_form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#pragma hdrstop

TSelection_List_Form *Selection_List_Form;
//---------------------------------------------------------------------------
__fastcall TSelection_List_Form::TSelection_List_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSelection_List_Form::FormResize(TObject *Sender)
{

	Selection_List_Form->Width = ListBox->Left + ListBox->Width + 120;
	Selection_List_Form->Height = OK_Button->Top + OK_Button->Height + 80;

}
//---------------------------------------------------------------------------

void TSelection_List_Form::fill_selection(std::vector <AnsiString> *List)
{
	Items_List = List;

	Selected_Item_Ptr = -1;

	ListBox->Clear();

	for(int i=0;i<Items_List[0].size();i++)
	ListBox->AddItem(Items_List[0][i],this);

	ListBox->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TSelection_List_Form::OK_ButtonClick(TObject *Sender)
{
	Selected_Item_Ptr = ListBox->ItemIndex;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TSelection_List_Form::Cancel_ButtonClick(TObject *Sender)
{
	Selected_Item_Ptr = -1;
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TSelection_List_Form::ListBoxDblClick(TObject *Sender)
{
	Selected_Item_Ptr = ListBox->ItemIndex;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TSelection_List_Form::Extra_ButtonClick(TObject *Sender)
{
	if( Extra_Action_Name == "Add label")
	{

	Ask_For_Single_Item_Form1->Caption = "Label name: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk &&
	   Ask_For_Single_Item_Form1->Edit_Box->Text.Length() > 0 )
	{

		Items_List[0].push_back(Ask_For_Single_Item_Form1->Edit_Box->Text);

		ListBox->Clear();
		for(int i=0;i<Items_List[0].size();i++)
		ListBox->AddItem(Items_List[0][i],this);
	}
	}
}
//---------------------------------------------------------------------------

void __fastcall TSelection_List_Form::Extra_Button_2Click(TObject *Sender)
{
	if( Extra_Action_2_Name == "Remove label")
	if( ListBox->ItemIndex >= 0 && ListBox->ItemIndex < Items_List[0].size() )
	{
		Items_List[0].erase( Items_List[0].begin() + ListBox->ItemIndex );

		ListBox->Clear();
		for(int i=0;i<Items_List[0].size();i++)
		ListBox->AddItem(Items_List[0][i],this);
	}
}
//---------------------------------------------------------------------------

