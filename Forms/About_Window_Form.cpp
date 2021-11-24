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
#pragma hdrstop

#include "About_Window_Form.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAbout_Window_Form1 *About_Window_Form1;
//---------------------------------------------------------------------------
__fastcall TAbout_Window_Form1::TAbout_Window_Form1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAbout_Window_Form1::Close_ButtonClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TAbout_Window_Form1::FormResize(TObject *Sender)
{

//	About_Window_Form1->Width =  Info_RichEdit->Width + Image1->Width + 50;
//	About_Window_Form1->Height = Info_RichEdit->Height + Close_Button->Height + 90;

}
//---------------------------------------------------------------------------

void __fastcall TAbout_Window_Form1::FormShow(TObject *Sender)
{


/*
	ifstream df;
	AnsiString AS;
	int counter=0;

	df.open((Application_Directory + "\\biorca.trc").c_str(),ios::out|ios::binary);

	if( df.is_open() == false )
	ShowMessage("trc file not found");

	Info_RichEdit->Lines[0].clear();

	while( !df.eof() && counter < 1000)
	{
		AS = Utils.load_String_from_File(File);
		Info_RichEdit->Lines[0].add(AS);
		counter++;
	}
*/

}
//---------------------------------------------------------------------------

