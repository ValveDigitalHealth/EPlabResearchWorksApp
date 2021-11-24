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

#include "Bar_Plot_Form.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBarPlot_Form *BarPlot_Form;
//---------------------------------------------------------------------------
__fastcall TBarPlot_Form::TBarPlot_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBarPlot_Form::FormResize(TObject *Sender)
{
	Chart1->Left = 3;
	Chart1->Width = BarPlot_Form->Width - 10;
	Chart1->Height = 0.8*BarPlot_Form->Height - 10;

}
//---------------------------------------------------------------------------
void __fastcall TBarPlot_Form::Exit1Click(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void TBarPlot_Form::save_window_to_jpg(AnsiString Filename)
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
