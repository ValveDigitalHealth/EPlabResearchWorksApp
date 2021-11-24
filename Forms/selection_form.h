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

#ifndef selection_formH
#define selection_formH

//---------------------------------------------------------------------------
/*
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
*/

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>


#include "Ask_For_Single_Item_Form.h"

#include <vector>

//---------------------------------------------------------------------------
class TSelection_List_Form : public TForm
{
__published:	// IDE-managed Components
    TButton *OK_Button;
    TButton *Cancel_Button;
	TButton *Extra_Button;
	TButton *Extra_Button_2;
	TListBox *ListBox;
    void __fastcall OK_ButtonClick(TObject *Sender);
    void __fastcall Cancel_ButtonClick(TObject *Sender);
    void __fastcall ListBoxDblClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
	void __fastcall Extra_ButtonClick(TObject *Sender);
	void __fastcall Extra_Button_2Click(TObject *Sender);
private:	// User declarations

	public:		// User declarations

	__fastcall TSelection_List_Form(TComponent* Owner);

    std::vector <AnsiString> *Items_List;
	void fill_selection(std::vector <AnsiString> *List);

	int Selected_Item_Ptr;

	AnsiString Extra_Action_Name,Extra_Action_2_Name;
};
//---------------------------------------------------------------------------
extern PACKAGE TSelection_List_Form *Selection_List_Form;
//---------------------------------------------------------------------------
#endif
