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

#ifndef PaintBox_6_FormH
#define PaintBox_6_FormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Jpeg.hpp>
#include <vector>

//---------------------------------------------------------------------------
class TPaintBox_Six_Form : public TForm
{
__published:	// IDE-managed Components
	TPaintBox *PaintBox1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TPaintBox *PaintBox2;
	TPaintBox *PaintBox3;
	TPaintBox *PaintBox4;
	TPaintBox *PaintBox5;
	TPaintBox *PaintBox6;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *File2;
	TLabel *Filename_Label;
	void __fastcall File2Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);

	private:	// User declarations

	public:		// User declarations

	__fastcall TPaintBox_Six_Form(TComponent* Owner);

	void save_window_to_jpg(AnsiString Filename);

	Graphics::TBitmap *PaintBox_Bitmap1;
	Graphics::TBitmap *PaintBox_Bitmap2;
	Graphics::TBitmap *PaintBox_Bitmap3;
	Graphics::TBitmap *PaintBox_Bitmap4;
	Graphics::TBitmap *PaintBox_Bitmap5;
	Graphics::TBitmap *PaintBox_Bitmap6;

};
//---------------------------------------------------------------------------
extern PACKAGE TPaintBox_Six_Form *PaintBox_Six_Form;
//---------------------------------------------------------------------------
#endif
