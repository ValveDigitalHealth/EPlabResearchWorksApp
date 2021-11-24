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

#ifndef Application_Settings_FormH
#define Application_Settings_FormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>

#include "Application_Settings.h"

//---------------------------------------------------------------------------
class TApp_Settings_Form : public TForm
{
__published:	// IDE-managed Components
	TButton *Close_Button;
	TLabel *Label1;
	TEdit *MaxGeoNodes_Number_Edit;
	TButton *Cancel_Button;
	void __fastcall Close_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations

	__fastcall TApp_Settings_Form(TComponent* Owner);

	//-------------------------------------------------------------------------
	// Application settings
	//-------------------------------------------------------------------------
	Application_Settings_Class Application_Settings; // !!!! SHOULD BE SINGLETON - BE CAREFUL TO USE JUST THIS INSTANCE

	AnsiString Application_Directory;

	void initialize(); // has to be called on app startup (now, in onshow event in mainform)

};
//---------------------------------------------------------------------------
extern PACKAGE TApp_Settings_Form *App_Settings_Form;
//---------------------------------------------------------------------------
#endif
