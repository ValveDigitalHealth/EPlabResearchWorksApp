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

#ifndef Geoemtric_Transformation_UnitH
#define Geoemtric_Transformation_UnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TGemetric_Transformation_Form : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TEdit *X_Edit;
    TEdit *Y_Edit;
    TEdit *Z_Edit;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *Teta_Edit;
    TEdit *Fi_Edit;
    TEdit *aX_Edit;
    TEdit *aY_Edit;
    TButton *OK_Button;
    TButton *Cancel_Button;
    TEdit *aZ_Edit;
    TLabel *Label4;
    TEdit *Seg_No_Edit;
	TLabel *Label5;
    void __fastcall OK_ButtonClick(TObject *Sender);
    void __fastcall Cancel_ButtonClick(TObject *Sender);
	void __fastcall aX_EditChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TGemetric_Transformation_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGemetric_Transformation_Form *Gemetric_Transformation_Form;
//---------------------------------------------------------------------------
#endif
