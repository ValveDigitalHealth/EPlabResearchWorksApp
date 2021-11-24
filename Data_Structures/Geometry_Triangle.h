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
/***************************************************************************

	file name            : triangle.h

	begin                : Jan 01 2007
	copyright            : (C) 2007 by Pawel Kuklik
	email                : kuklik@if.pw.edu.pl

	description          : Triangle is a class representing single triangle of the 3D
	surface. Keeps info about spatial position and assigned values. Used by
	EXTRACTOR_Surface class.

*****************************************************************************/

#include "predefined_parameters.h"

#include <fstream>
#include <vector>

#ifndef TRIANGLE_H
#define TRIANGLE_H

using namespace std;

//----------------------------------------------------------------------------

class Geometry_Triangle
{
	public:

        Geometry_Triangle();
        ~Geometry_Triangle();

		//------------------------------------------------------------------
		// "Pointers" of the nodes forming given triangle (in vector<Node> Surface_Node_Set
		// in EXTRACTOR_Surface object)
		//------------------------------------------------------------------
        long Nodes[3];

        //------------------------------------------------------------------
        // Normal to the triangle (outward)
        //------------------------------------------------------------------
        float normal[3];

        //------------------------------------------------------------------
        // Direction of the normal (sometimes this info is in the file with
        // surface data, default +1) (+1 is outward, -1 is inward)
        //------------------------------------------------------------------
        double Direction;

		//------------------------------------------------------------------
		// Flage whether triangle is removed
		// Values: ON, OFF
		//------------------------------------------------------------------
		int Removed_Flag;

		//------------------------------------------------------------------
		// Segment id of given triangle
		//------------------------------------------------------------------
		int Segment_Id;

		double Tmp_Flag;
		float tmp_vec[3]; // used for various calculations

		int GroupId;

		double Area_cm2;

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

	int load_blink_object_from_stream(ifstream* File);

	std::vector <int> Segmentation_Ids_For_Undo_Feature;
};

#endif


