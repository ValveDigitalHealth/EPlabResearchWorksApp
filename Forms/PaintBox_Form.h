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

#ifndef PaintBox_FormH
#define PaintBox_FormH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Jpeg.hpp>
#include <vector>

//---------------------------------------------------------------------------
class TPaintBoxForm : public TForm
{
__published:	// IDE-managed Components
	TPaintBox *PaintBox1;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *File2;
	TLabel *Top_Label;
	void __fastcall File2Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall PaintBox1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall PaintBox1Paint(TObject *Sender);


	private:	// User declarations

	public:		// User declarations

	__fastcall TPaintBoxForm(TComponent* Owner);

	void save_window_to_jpg(AnsiString Filename);

	Graphics::TBitmap *PaintBox_Bitmap;

    double Custom_Size_Var;
};
//---------------------------------------------------------------------------
extern PACKAGE TPaintBoxForm *PaintBoxForm;
//---------------------------------------------------------------------------
#endif
