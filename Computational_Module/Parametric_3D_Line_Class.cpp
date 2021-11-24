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

//

#include"Parametric_3D_Line_Class.h"

//----------------------------------------------------------------------------------------

int Parametric_3D_line::save_object_to_stream(ofstream* File)
{
	File->write((char*)&a, sizeof (double));
	File->write((char*)&b, sizeof (double));
	File->write((char*)&c, sizeof (double));
	File->write((char*)&d, sizeof (double));
	File->write((char*)&e, sizeof (double));
	File->write((char*)&f, sizeof (double));
}

//---------------------------------------------------------------------------
int Parametric_3D_line::load_object_from_stream(ifstream* File)
{
	File->read((char*)&a, sizeof (double));
	File->read((char*)&b, sizeof (double));
	File->read((char*)&c, sizeof (double));
	File->read((char*)&d, sizeof (double));
	File->read((char*)&e, sizeof (double));
	File->read((char*)&f, sizeof (double));
}

//---------------------------------------------------------------------------
