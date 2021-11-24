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

#include "Local_Activation.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

int Local_Activations_Vector::save_object_to_stream(std::ofstream* File)
{
	int version = 1;
	File->write((char*)&version, sizeof (int));

	long S=Local_Activations_vec.size();
	File->write((char*)&S, sizeof (long));
	for(long i=0;i<S;i++)
	Local_Activations_vec[i].save_object_to_stream(File);

	return 1;
}

//---------------------------------------------------------------------------

int Local_Activations_Vector::load_object_from_stream(std::ifstream* File)
{
	int Items_Number;
	int version;
	long S;

	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{
		File->read((char*)&S, sizeof (long));
		Local_Activations_vec.clear();
		Local_Activation_Class tmp;
		Local_Activations_vec.assign(S,tmp);
		for(long i=0;i<S;i++)
		Local_Activations_vec[i].load_object_from_stream(File);
	}

	return 1;
}

//---------------------------------------------------------------------------


int Local_Activation_Class::save_object_to_stream(std::ofstream* File)
{
	int version = 1;
	File->write((char*)&version, sizeof (int));

	File->write((char*)&Timing_In_EGM_ptr, sizeof (long));
	File->write((char*)&Activation_Start_Offset, sizeof (int));
	File->write((char*)&Activation_End_Offset, sizeof (int));


	File->write((char*)&Amplitude, sizeof (double));

	return 1;
}

//---------------------------------------------------------------------------

int Local_Activation_Class::load_object_from_stream(std::ifstream* File)
{
	int Items_Number;
	int version;

	File->read((char*)&version, sizeof (int));

	if( version == 1 )
	{
		File->read((char*)&Timing_In_EGM_ptr, sizeof (long));
		File->read((char*)&Activation_Start_Offset, sizeof (int));
		File->read((char*)&Activation_End_Offset, sizeof (int));

		File->read((char*)&Amplitude, sizeof (double));
	}
	return 1;
}

//---------------------------------------------------------------------------

Local_Activation_Class::Local_Activation_Class()
{

	 Timing_In_EGM_ptr = 0;

	 Activation_Start_Offset = 0;
	 Activation_End_Offset = 0;

	 Amplitude  = 0;
}

//---------------------------------------------------------------------------

