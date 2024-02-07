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

#pragma hdrstop

#include "About_Window_Form.h"

#include <windows.h>
#include <ShellAPI.h>

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
	Version_Text = "Version v.2.0.27";


	Info_RichEdit->Lines[0].Add("EPLab Research Works app.  " + Version_Text );
	Info_RichEdit->Lines[0].Add("\n");
	Info_RichEdit->Lines[0].Add("Under MIT License. For research purposes only - not for clinical use.");
	Info_RichEdit->Lines[0].Add("\n");
	Info_RichEdit->Lines[0].Add("Important:");
	Info_RichEdit->Lines[0].Add("EPLab Works app is an open source, free to use project. I maintain it in my free time (adding features, optimizing) and pay for website hosting. If you find the app useful in your work, please consider donating to aid this project and it’s further development.");
	Info_RichEdit->Lines[0].Add("\n");
	Info_RichEdit->Lines[0].Add("FFT by Laurent de Soras.");


 //	LinkLabel1->Link = "https://www.paypal.com/donate?token=rnLsPVya_ptzqvHtgJ27msHNw_qWt8KvqiQMDbyGBScKhtLO2c-i3JII_FlgRE2f4wg9ts5elQaVBEkr";

}
//---------------------------------------------------------------------------

void __fastcall TAbout_Window_Form1::Image1Click(TObject *Sender)
{
	LPCWSTR myLPCWSTR = ConvertCharToLPCWSTR("https://www.paypal.com/donate/?hosted_button_id=WYT9NWBY8L5UA");
	ShellExecute(NULL, L"open", myLPCWSTR, NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

// Function to convert a char* string to LPCWSTR string
LPCWSTR TAbout_Window_Form1::ConvertCharToLPCWSTR(const char* charString)
{
	int stringLength = strlen(charString) + 1;
	wchar_t* wideString = new wchar_t[stringLength];

	MultiByteToWideChar(CP_ACP, 0, charString, stringLength, wideString, stringLength);

	return wideString;
}

//---------------------------------------------------------------------------

