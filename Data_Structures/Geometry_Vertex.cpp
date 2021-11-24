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
//------------------------------------------------------------------------------
#include "Geometry_Vertex.h"
//------------------------------------------------------------------------------

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------

Geometry_Vertex::Geometry_Vertex()
{
	// Here just setting initial values of the variables
	x = 0.0;
	y = 0.0;
	z = 0.0;

	Original_x = 0.0;
	Original_y = 0.0;
	Original_z = 0.0;

	Flag_A = 0;

	Removed_Geometry_Vertex_Flag = false;

    normal[0] = 0.0;
	normal[1] = 0.0;
    normal[2] = 0.0;

    coloured = 0;

	Closest_Data_Point_Id = -1;

	R=0;
	G=0;
	B=0;
}

//---------------------------------------------------------------------------------

	//----------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------


Geometry_Vertex::~Geometry_Vertex()
{

}

//----------------------------------------------------------------------------------

void Geometry_Vertex::clear_all_values()
{
	Values_Vector V;

	for(unsigned int i=0;i<Vertex_Values.size();i++)
	{

	V = Vertex_Values[i];

	for(unsigned int j=0;j<V.Values.size();j++)
		set_value(i,j,0);
	}
}

//----------------------------------------------------------------------------------

double Geometry_Vertex::get_value_no_range_checking(int Data_Point_Set_Ptr, int Value_Ptr)
{
	return Vertex_Values[Data_Point_Set_Ptr].Values[Value_Ptr];
}

//----------------------------------------------------------------------------------

double Geometry_Vertex::get_value(int Data_Point_Set_Ptr, int Value_Ptr)
{
	Values_Vector V;

	if( Data_Point_Set_Ptr >= 0 && (unsigned int) Data_Point_Set_Ptr < Vertex_Values.size() )
	{
		V = Vertex_Values[Data_Point_Set_Ptr];

		if( Value_Ptr >= 0 && (unsigned int)Value_Ptr < V.Values.size() )
			return V.Values[Value_Ptr];
		else
		{

			//ShowMessage("get value in geo vertex error: value out of range (geometry_vertex.cpp)");
			return 0;
		}
	}
	else
	{
		//ShowMessage("get value in geo vertex error: dpset out of range (geometry_vertex.cpp)");
		return 0;
	}
}

//----------------------------------------------------------------------------------

int Geometry_Vertex::set_value(int Data_Point_Set_Ptr, int Value_Ptr, double New_Value)
{
	Values_Vector V;
	double tmpV=0;

	if( Data_Point_Set_Ptr < 0 || Value_Ptr < 0 )
	{
		ShowMessage("dpset or value ptr negative " + SourceFileName);
		return -1;
	}

	// check if data point set ptr is within range, otherwise expand data structure
	if( (unsigned int)Data_Point_Set_Ptr >= Vertex_Values.size() )
	{
		while( (unsigned int)Data_Point_Set_Ptr >= Vertex_Values.size() )
		Vertex_Values.push_back(V);
	}

	// check if value ptr is within range, otherwise expand data structure
	if( (unsigned int)Value_Ptr >= Vertex_Values[Data_Point_Set_Ptr].Values.size() )
	{
		while( (unsigned int)Value_Ptr >= Vertex_Values[Data_Point_Set_Ptr].Values.size() )
		Vertex_Values[Data_Point_Set_Ptr].Values.push_back(tmpV);
	}

	V = Vertex_Values[Data_Point_Set_Ptr];
	V.Values[Value_Ptr] = New_Value;
	Vertex_Values[Data_Point_Set_Ptr] = V;

	return 1;
}
//----------------------------------------------------------------------------------

int Geometry_Vertex::save_object_to_stream(ofstream* File)
{
	int version = 4;
	File->write((char*)&version, sizeof (int));

	File->write((char*)&x, sizeof (double));
	File->write((char*)&y, sizeof (double));
	File->write((char*)&z, sizeof (double));

	File->write((char*)&Original_x, sizeof (double));
	File->write((char*)&Original_y, sizeof (double));
	File->write((char*)&Original_z, sizeof (double));

	File->write((char*)&Removed_Geometry_Vertex_Flag, sizeof (bool));

	File->write((char*)&normal[0], sizeof (float));
	File->write((char*)&normal[1], sizeof (float));
	File->write((char*)&normal[2], sizeof (float));

	long NN = Neighbors.size();
	File->write((char*)&NN, sizeof (long));
	for(unsigned long i=0;i<Neighbors.size();i++)
	File->write((char*)&Neighbors[i], sizeof (long));

	NN = Neig_Triangles.size();
	File->write((char*)&NN, sizeof (long));
	for(unsigned long i=0;i<Neig_Triangles.size();i++)
	File->write((char*)&Neig_Triangles[i], sizeof (long));

	File->write((char*)&Type, sizeof (int));
	File->write((char*)&Identifier, sizeof (int));
	File->write((char*)&GroupId, sizeof (int));

	File->write((char*)&Data_Point_Nearby, sizeof (bool));

	int ds1 = Vertex_Values.size();
	Values_Vector VV;
	double v;
	File->write((char*)&ds1, sizeof (int));

	for(int i=0;i<ds1;i++) // throuhg all value data point sets
	{
		VV = Vertex_Values[i];
		int ds2 = VV.Values.size();
		File->write((char*)&ds2, sizeof (int));

		for(int j=0;j<ds2;j++)
		{
			v = get_value(i,j);
			File->write((char*)&v, sizeof (double));
		}
	}

	File->write((char*)&Closest_Data_Point_Id, sizeof (long));
	File->write((char*)&Closest_Data_Point_Id_Unlimited, sizeof (long));

	return 1;
}

//---------------------------------------------------------------------------

int Geometry_Vertex::load_object_from_stream(ifstream* File)
{
	int version;

	File->read((char*)&version, sizeof (int));

	if( version == 4 )
	{

	File->read((char*)&x, sizeof (double));
	File->read((char*)&y, sizeof (double));
	File->read((char*)&z, sizeof (double));

	File->read((char*)&Original_x, sizeof (double));
	File->read((char*)&Original_y, sizeof (double));
	File->read((char*)&Original_z, sizeof (double));

	File->read((char*)&Removed_Geometry_Vertex_Flag, sizeof (bool));

	File->read((char*)&normal[0], sizeof (float));
	File->read((char*)&normal[1], sizeof (float));
	File->read((char*)&normal[2], sizeof (float));

    long NN=0,tmpL=0;

	File->read((char*)&NN, sizeof (long));
	Neighbors.assign(NN,tmpL);
	for(long i=0;i<NN;i++)
    File->read((char*)&Neighbors[i], sizeof (long));

	File->read((char*)&NN, sizeof (long));
	Neig_Triangles.assign(NN,tmpL);
	for(long i=0;i<NN;i++)
	File->read((char*)&Neig_Triangles[i], sizeof (long));

	File->read((char*)&Type, sizeof (int));
	File->read((char*)&Identifier, sizeof (int));
	File->read((char*)&GroupId, sizeof (int));

	File->read((char*)&Data_Point_Nearby, sizeof (bool));

	int ds1,ds2;
	double v;
	File->read((char*)&ds1, sizeof (int));

	for(int i=0;i<ds1;i++)
	{
		File->read((char*)&ds2, sizeof (int));


		for(int j=0;j<ds2;j++)
		{
			File->read((char*)&v, sizeof (double));
			set_value(i,j,v);
		}
	}

	File->read((char*)&Closest_Data_Point_Id, sizeof (long));
	File->read((char*)&Closest_Data_Point_Id_Unlimited, sizeof (long));


	return 1;

	} // v4


    return -1;
}
//---------------------------------------------------------------------------

bool Geometry_Vertex::is_value_valid(int Data_Point_Set_Ptr, int Value_Ptr)
{
	if( get_value(Data_Point_Set_Ptr,Value_Ptr) != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	return true;
	else
	return false;
}
//---------------------------------------------------------------------------

