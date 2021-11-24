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

#ifndef Segments_ListH
#define Segments_ListH

#include <fstream>
#include <vector>
#include <vcl.h>

#include <Segment.h>

//---------------------------------------------------------------------------

class Segments_List_Class
{
	public:

	AnsiString FilePath;

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object();
	int load_object();

	std::vector <Segment> Segments;

	int get_segment_id(AnsiString Name);
	AnsiString get_current_segment_name();
	AnsiString get_segment_name(int Segment_Id);
    int get_segment_ptr_from_name(AnsiString Name);

	void get_segment_color(int Segment_Id,int* R,int* G,int* B);

	// int get_segment_ptr(int Segment_Id);
};

#endif
