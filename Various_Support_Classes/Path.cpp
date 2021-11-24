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

#include "Path.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-------------------------------------------------------------------------

int Path_Class::save_object_to_stream(ofstream* File)
{
	int Items_Number,Size;

	int version = 1;

	File->write((char*)&version, sizeof (int));

	File->write((char*)&Type, sizeof (int));

	File->write((char*)&x1, sizeof (double));
	File->write((char*)&y1, sizeof (double));
	File->write((char*)&z1, sizeof (double));

	File->write((char*)&x2, sizeof (double));
	File->write((char*)&y2, sizeof (double));
	File->write((char*)&z2, sizeof (double));

	File->write((char*)&Distance, sizeof (double));
	File->write((char*)&LAT_Difference, sizeof (double));

	return 1;
}

//-------------------------------------------------------------------------

int Path_Class::load_object_from_stream(ifstream* File)
{
	int Items_Number;
	int version;

	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{

	File->read((char*)&Type, sizeof (int));

	File->read((char*)&x1, sizeof (double));
	File->read((char*)&y1, sizeof (double));
	File->read((char*)&z1, sizeof (double));

	File->read((char*)&x2, sizeof (double));
	File->read((char*)&y2, sizeof (double));
	File->read((char*)&z2, sizeof (double));

	File->read((char*)&Distance, sizeof (double));
	File->read((char*)&LAT_Difference, sizeof (double));

	}

	return 1;
}
//---------------------------------------------------------------------------
