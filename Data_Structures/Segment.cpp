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

#include "Segment.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


int Segment::save_object_to_stream(ofstream* File,int version)
{
	File[0] << Code << " ";

	File[0] << Name.c_str() << " ";

	File[0] << Color_R << " ";
	File[0] << Color_G << " ";
	File[0] << Color_B << " ";

	File[0] << "\n";

	// double R,G,B;
	return 1;
}

//---------------------------------------------------------------------------

int Segment::load_object_from_stream(ifstream* File,int version)
{
	int col;
	double R,G,B;
	long C;
	char String[2000];

	Code = -1000;

	Name = "";

	if( version == 2 )
	{

	File[0] >> Code;
	File[0] >> String;
	Name = AnsiString(String);

	File[0] >> Color_R;
	File[0] >> Color_G;
	File[0] >> Color_B;

/*
	File[0] >> R;
	File[0] >> G;
	File[0] >> B;
	Color = (TColor)RGB(R,G,B);
*/
	}

	if( version == 1 )
	{

	File[0] >> Code;
	File[0] >> String;
	Name = AnsiString(String);

	}

	if( Name.Length() > 1 )
	return -1;
}
//---------------------------------------------------------------------------

