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

#include "PaintBox_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPaintBoxForm *PaintBoxForm;
//---------------------------------------------------------------------------
__fastcall TPaintBoxForm::TPaintBoxForm(TComponent* Owner)
	: TForm(Owner)
{
	PaintBox_Bitmap = new Graphics::TBitmap;

}
//---------------------------------------------------------------------------
void __fastcall TPaintBoxForm::File2Click(TObject *Sender)
{
	ModalResult = mrOk;

}
//---------------------------------------------------------------------------
void __fastcall TPaintBoxForm::FormDestroy(TObject *Sender)
{
	delete PaintBox_Bitmap;

}
//---------------------------------------------------------------------------
void __fastcall TPaintBoxForm::FormResize(TObject *Sender)
{
	PaintBox_Bitmap->Width = PaintBox1->Width;
	PaintBox_Bitmap->Height = PaintBox1->Height;
}
//---------------------------------------------------------------------------
void __fastcall TPaintBoxForm::PaintBox1MouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y)
{
	int MX = (double)X/(double)PaintBox1->Width*Custom_Size_Var;
	int MY = (double)(PaintBox1->Height-Y)/(double)PaintBox1->Height*Custom_Size_Var;

	Top_Label->Caption = "XY: ["+
		IntToStr(MX)+","+
		IntToStr(MY)+"]";
}
//---------------------------------------------------------------------------

void __fastcall TPaintBoxForm::PaintBox1Paint(TObject *Sender)
{
	PaintBoxForm->PaintBox1->Canvas->Draw(0, 0, PaintBox_Bitmap);
}
//---------------------------------------------------------------------------

void TPaintBoxForm::save_window_to_jpg(AnsiString Filename)
{
	Graphics::TBitmap *BitMap = new Graphics::TBitmap();
	BitMap->Height = this->Height;
	BitMap->Width = this->Width;
	BringToFront();
	Repaint();
	BitMap->Canvas->CopyRect(Rect(0,0,BitMap->Width,BitMap->Height), this->Canvas,
							 Rect(0,0,BitMap->Width,BitMap->Height) );
	std::auto_ptr<TJPEGImage> jp(new TJPEGImage());
	jp->Assign(BitMap);
	jp->SaveToFile(Filename);
	delete BitMap;
}
//---------------------------------------------------------------------------

