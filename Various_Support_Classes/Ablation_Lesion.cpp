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

#include "Ablation_Lesion.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
int Ablation_Lesion_Class::save_object_to_stream(ofstream* File)
{
	int version = 1;
	File->write((char*)&version, sizeof (int));

	int Size = Ablation_Points_List.size();
	File->write((char*)&Size, sizeof (int));

	for(int i=0;i<Size;i++)
	Ablation_Points_List[i].save_object_to_stream(File);
}
//---------------------------------------------------------------------------

int Ablation_Lesion_Class::load_object_from_stream(ifstream* File)
{
	int version;
	int Size;

	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{

	File->read((char*)&Size, sizeof (int));
	Ablation_Points_List.clear();
	Ablation_Point_Class Ablation_Point_Class_Item;
	Ablation_Points_List.assign(Size,Ablation_Point_Class_Item);
	for(int i=0;i<Size;i++)
	Ablation_Points_List[i].load_object_from_stream(File);


	}
}
//---------------------------------------------------------------------------

