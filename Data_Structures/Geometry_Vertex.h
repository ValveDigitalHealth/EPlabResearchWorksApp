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

	file name            : Geometry_Vertex.h

	copyright            : (C) 2017 by Pawel Kuklik
	email                : pawel.kuklik@gmail.com

	description          : Geometry_Vertex is a class representing single Geometry_Vertex of the 3D
						   surface. Keeps info about spatial position and assigned values.

*****************************************************************************/

#include <fstream>
#include <vector>
#include <vcl.h>

#include "value_description_class.h"
#include "values_vector.h"
#include "Pawel_Numerical_Library.h"

#ifndef Geometry_Vertex_H
#define Geometry_Vertex_H

using namespace std;

//----------------------------------------------------------------------------

class Geometry_Vertex
{
	private:

		AnsiString SourceFileName = ": Geometry_Vertex";

		//------------------------------------------------------------------
		// Values assigned to nodes (consecutive items correspond with data point sets)
		//------------------------------------------------------------------
		vector <Values_Vector> Vertex_Values; // one vector per data set, and inside a vector with values

	public:

		Geometry_Vertex();
		~Geometry_Vertex();

		//---------------------------------------------------------
		// Save/load object to/from stream
		//---------------------------------------------------------
		int save_object_to_stream(ofstream* File);
		int load_object_from_stream(ifstream* File);

		void clear_all_values();

		// ATTENTION: HERE VALUES ARE SET/GET NOT BY NAME BUT BY PTR
		// since its used only in display/interpolation, it should be sufficient
		double get_value(int Data_Point_Set_Ptr, int Value_Ptr);
		double get_value_no_range_checking(int Data_Point_Set_Ptr, int Value_Ptr);
		int set_value(int Data_Point_Set_Ptr, int Value_Ptr, double New_Value);
		bool is_value_valid(int Data_Point_Set_Ptr,int Value_Ptr);

		int Type;
		int Identifier;
		int GroupId;

		//------------------------------------------------------------------
		// Spatial position of the Geometry_Vertex
		//------------------------------------------------------------------
		double x;
		double y;
		double z;

		double Original_x;
		double Original_y;
		double Original_z;

		//------------------------------------------------------------------
		// Flag whether Geometry_Vertex is removed from surface
		// values: ON and OFF
		//------------------------------------------------------------------
		bool Removed_Geometry_Vertex_Flag;

		//------------------------------------------------------------------
		// Normal vector to the Geometry_Vertex (average of the normals of neig. triangles)
		//------------------------------------------------------------------
		float normal[3];

		//------------------------------------------------------------------
		// Number of the neig. Geometry_Vertexs and their "pointers" in Surface_Geometry_Vertex_Set[]
		// in EXTRACTOR_Surface object
		//------------------------------------------------------------------
		std::vector <long> Neighbors;
		long n12;

		//------------------------------------------------------------------
		// Number of the neig. triangles and their "pointers" in Surface_Triangle_Set[]
		// in EXTRACTOR_Surface object
		//------------------------------------------------------------------
		std::vector <long> Neig_Triangles;

		long Closest_Data_Point_Id;
		long Closest_Data_Point_Id_Unlimited;
		vector <long> Closest_Data_Points_Ptrs; // not saved
		vector <float> Closest_Data_Points_Distances; // not saved

		bool Data_Point_Nearby;

		long Closest_Node_Ptr; // used in fitting 2 geos

		//------------------------------------------------------------------
		// Temporarys flag used in some algorithms
		//------------------------------------------------------------------
		long Flag_A,Flag_B;
		double coloured;
		double tmp_val,tmpx,tmpy,tmpz;
		float R,G,B;
};

#endif
