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

#include "PaintBox_6_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPaintBox_Six_Form *PaintBox_Six_Form;
//---------------------------------------------------------------------------
__fastcall TPaintBox_Six_Form::TPaintBox_Six_Form(TComponent* Owner)
	: TForm(Owner)
{
	PaintBox_Bitmap1 = new Graphics::TBitmap;
	PaintBox_Bitmap2 = new Graphics::TBitmap;
	PaintBox_Bitmap3 = new Graphics::TBitmap;
	PaintBox_Bitmap4 = new Graphics::TBitmap;
	PaintBox_Bitmap5 = new Graphics::TBitmap;
	PaintBox_Bitmap6 = new Graphics::TBitmap;

}
//---------------------------------------------------------------------------
void __fastcall TPaintBox_Six_Form::File2Click(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TPaintBox_Six_Form::FormResize(TObject *Sender)
{
	int W = 0.5*PaintBox_Six_Form->Height-100;

	Filename_Label->Left = 3;
	Filename_Label->Top = 3;

	Label1->Left = 3;
	Label1->Top = Filename_Label->Top + Filename_Label->Height + 3;
	PaintBox1->Left = Label1->Left;
	PaintBox1->Top = Label1->Top + Label1->Height + 5;
	PaintBox1->Width = W;
	PaintBox1->Height = W;
	PaintBox_Bitmap1->Width = W;
	PaintBox_Bitmap1->Height = W;

	Label2->Left = PaintBox1->Left + W + 3;
	Label2->Top = Label1->Top;
	PaintBox2->Left = Label2->Left;
	PaintBox2->Top = PaintBox1->Top;
	PaintBox2->Width = W;
	PaintBox2->Height = W;
	PaintBox_Bitmap2->Width = W;
	PaintBox_Bitmap2->Height = W;

	Label3->Left = PaintBox2->Left + W + 3;
	Label3->Top = Label1->Top;
	PaintBox3->Left = Label3->Left;
	PaintBox3->Top = PaintBox1->Top;
	PaintBox3->Width = W;
	PaintBox3->Height = W;
	PaintBox_Bitmap3->Width = W;
	PaintBox_Bitmap3->Height = W;
///
	Label4->Left = 3;
	Label4->Top = PaintBox1->Top + PaintBox1->Height + 5;
	PaintBox4->Left = Label4->Left;
	PaintBox4->Top = Label4->Top + Label4->Height + 5;
	PaintBox4->Width = W;
	PaintBox4->Height = W;
	PaintBox_Bitmap4->Width = W;
	PaintBox_Bitmap4->Height = W;

	Label5->Left = PaintBox4->Left + W + 3;
	Label5->Top = Label4->Top;
	PaintBox5->Left = Label5->Left;
	PaintBox5->Top = Label5->Top + Label5->Height + 5;
	PaintBox5->Width = W;
	PaintBox5->Height = W;
	PaintBox_Bitmap5->Width = W;
	PaintBox_Bitmap5->Height = W;

	Label6->Left = PaintBox5->Left + W + 3;
	Label6->Top = Label4->Top;
	PaintBox6->Left = Label6->Left;
	PaintBox6->Top = Label6->Top + Label6->Height + 5;
	PaintBox6->Width = W;
	PaintBox6->Height = W;
	PaintBox_Bitmap6->Width = W;
	PaintBox_Bitmap6->Height = W;
}
//---------------------------------------------------------------------------
void __fastcall TPaintBox_Six_Form::FormDestroy(TObject *Sender)
{
	delete PaintBox_Bitmap1;
	delete PaintBox_Bitmap2;
	delete PaintBox_Bitmap3;
	delete PaintBox_Bitmap4;
	delete PaintBox_Bitmap5;
	delete PaintBox_Bitmap6;

}
//---------------------------------------------------------------------------
void __fastcall TPaintBox_Six_Form::FormPaint(TObject *Sender)
{
	PaintBox1->Canvas->Draw(0, 0, PaintBox_Bitmap1);
	PaintBox2->Canvas->Draw(0, 0, PaintBox_Bitmap2);
	PaintBox3->Canvas->Draw(0, 0, PaintBox_Bitmap3);
	PaintBox4->Canvas->Draw(0, 0, PaintBox_Bitmap4);
	PaintBox5->Canvas->Draw(0, 0, PaintBox_Bitmap5);
	PaintBox6->Canvas->Draw(0, 0, PaintBox_Bitmap6);
}
//---------------------------------------------------------------------------
void TPaintBox_Six_Form::save_window_to_jpg(AnsiString Filename)
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

