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

#ifndef Progress_Bar_FormH
#define Progress_Bar_FormH
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>

//---------------------------------------------------------------------------
class TProgress_Form : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *RichEditMain;
	TButton *Cancel_Button;
    void __fastcall FormResize(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);

	private:	// User declarations

	public:		// User declarations

	bool Cancel_Flag;

	__fastcall TProgress_Form(TComponent* Owner);

	void add_text(AnsiString Line);
	void replace_last_line_with_text(AnsiString Line);
	void clear();

//	std::chrono::steady_clock::time_point begin_time;
//	std::chrono::steady_clock::time_point end_time;

};
//---------------------------------------------------------------------------
extern PACKAGE TProgress_Form *Progress_Form;
//---------------------------------------------------------------------------
#endif
