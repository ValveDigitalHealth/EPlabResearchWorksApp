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

#include "Label.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

int Label_Class::save_object_to_stream(ofstream* File)
{
		int version = 1;

		File->write((char*)&version, sizeof (int));

		Utils.save_String_to_File(File, Name);

		File->write((char*)&x, sizeof (double));
		File->write((char*)&y, sizeof (double));
		File->write((char*)&z, sizeof (double));

		File->write((char*)&nx, sizeof (double));
		File->write((char*)&ny, sizeof (double));
		File->write((char*)&nz, sizeof (double));

		return 1;
}
//---------------------------------------------------------------------------

int Label_Class::load_object_from_stream(ifstream* File)
{
	int version;
	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{
		Name = Utils.load_String_from_File(File);

		File->read((char*)&x, sizeof (double));
		File->read((char*)&y, sizeof (double));
		File->read((char*)&z, sizeof (double));

		File->read((char*)&nx, sizeof (double));
		File->read((char*)&ny, sizeof (double));
		File->read((char*)&nz, sizeof (double));

		return 1;

	} // ver 2

	return -1;
}
//---------------------------------------------------------------------------