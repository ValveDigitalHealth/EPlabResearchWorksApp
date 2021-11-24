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
//----------------------------------------------------------------------------------
#include "Geometry_Triangle.h"
//----------------------------------------------------------------------------------

Geometry_Triangle::Geometry_Triangle()
{
    Removed_Flag = OFF;

	Nodes[0]=0;
	Nodes[1]=0;
	Nodes[2]=0;

	normal[0] = 0.0;
	normal[1] = 0.0;
	normal[2] = 0.0;

	Area_cm2 = 0;
	Direction = 0;

	Segmentation_Ids_For_Undo_Feature.clear();

    Segment_Id = -1;
}

//----------------------------------------------------------------------------------

Geometry_Triangle::~Geometry_Triangle()
{

}

//-------------------------------------------------------------------------------------------------

int Geometry_Triangle::save_object_to_stream(ofstream* File)
{
    int version = 1;

	File->write((char*)&version, sizeof (int));

    File->write((char*)&Nodes[0], sizeof (long));
    File->write((char*)&Nodes[1], sizeof (long));
    File->write((char*)&Nodes[2], sizeof (long));

	File->write((char*)&normal[0], sizeof (float));
	File->write((char*)&normal[1], sizeof (float));
	File->write((char*)&normal[2], sizeof (float));

	File->write((char*)&Direction, sizeof (double));

	File->write((char*)&Removed_Flag, sizeof (int));

	File->write((char*)&Segment_Id, sizeof (int));

	File->write((char*)&GroupId, sizeof (int));
	File->write((char*)&Area_cm2, sizeof (double));

    return 1;
}
//---------------------------------------------------------------------------

int Geometry_Triangle::load_object_from_stream(ifstream* File)
{
	int version;

	File->read((char*)&version, sizeof (int));

    if( version == 1 )
	{

    File->read((char*)&Nodes[0], sizeof (long));
    File->read((char*)&Nodes[1], sizeof (long));
    File->read((char*)&Nodes[2], sizeof (long));

	File->read((char*)&normal[0], sizeof (float));
	File->read((char*)&normal[1], sizeof (float));
	File->read((char*)&normal[2], sizeof (float));

	File->read((char*)&Direction, sizeof (double));

	File->read((char*)&Removed_Flag, sizeof (int));

	File->read((char*)&Segment_Id, sizeof (int));

	File->read((char*)&GroupId, sizeof (int));
	File->read((char*)&Area_cm2, sizeof (double));

    return 1;
    } // v1

    return -1;
}
//---------------------------------------------------------------------------


