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
#include "Electrogram.h"
//---------------------------------------------------------------------------

	//----------------------------------------------------------------------
	// Constructor of the signal
    //----------------------------------------------------------------------

Electrogram::Electrogram()
{
	Name = "empty";
	Time_Step_ms = 1;
    ISI_Histogram_Box = 15;
}


//---------------------------------------------------------------------------

	//----------------------------------------------------------------------
	// Destructor of the signal
	//----------------------------------------------------------------------

Electrogram::~Electrogram()
{

}

//---------------------------------------------------------------------------

int Electrogram::save_object_to_stream(ofstream* File)
{
	long S;
	float v;

	//----------------------------
	int version = 4;
	//----------------------------

	File->write((char*)&version, sizeof (int));

	Utils.save_String_to_File(File,Name);

	S=Voltage_Values.size();
	File->write((char*)&S, sizeof (long));
	for(long i=0;i<S;i++)
	{
		v = Voltage_Values[i];
		File->write((char*)&v, sizeof (float));
	}

	S=Phase.size();
	File->write((char*)&S, sizeof (long));
	for(long i=0;i<S;i++)
	{
		v = Phase[i];
		File->write((char*)&v, sizeof (float));
	}
	File->write((char*)&Base_Cycle_Used_For_Phase_ptr, sizeof (double));

	File->write((char*)&Time_Step_ms, sizeof (double));

	S=vNAVX_Positions_In_Signal.size();
	File->write((char*)&S, sizeof (long));
	for(long i=0;i<S;i++)
	File->write((char*)&vNAVX_Positions_In_Signal[i], sizeof (long));

	S=vICL_Positions_In_Signal.size();
	File->write((char*)&S, sizeof (long));
	for(long i=0;i<S;i++)
	File->write((char*)&vICL_Positions_In_Signal[i], sizeof (long));

	S=vED_Positions_In_Signal.size();
	File->write((char*)&S, sizeof (long));
	for(long i=0;i<S;i++)
	File->write((char*)&vED_Positions_In_Signal[i], sizeof (long));

	S=vPeak_Positions.size();
	File->write((char*)&S, sizeof (long));
	for(long i=0;i<S;i++)
	File->write((char*)&vPeak_Positions[i], sizeof (long));

	Local_Activations.save_object_to_stream(File);

	S=Silence_Vector.size();
	File->write((char*)&S, sizeof (long));
	for(long i=0;i<S;i++)
	File->write((char*)&Silence_Vector[i], sizeof (int));

	return 1;
}
//---------------------------------------------------------------------------

int Electrogram::load_object_from_stream(ifstream* File)
{
	long Size;
	int version,tmpInt=0;
	long tmp_long;
	float v;

	File->read((char*)&version, sizeof (int));

	if( version == 4 )
	{

	Name= Utils.load_String_from_File(File);

	long S;

	File->read((char*)&S, sizeof (long));
	Voltage_Values.clear();
	Voltage_Values.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	{
		File->read((char*)&v, sizeof (float));
		Voltage_Values[i] = v;
	}


	File->read((char*)&S, sizeof (long));
	Phase.clear();
	Phase.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	{
		File->read((char*)&v, sizeof (float));
		Phase[i] = v;
	}
	File->read((char*)&Base_Cycle_Used_For_Phase_ptr, sizeof (double));


	File->read((char*)&Time_Step_ms, sizeof (double));

	File->read((char*)&S, sizeof (long));
	vNAVX_Positions_In_Signal.clear();
	vNAVX_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vNAVX_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vICL_Positions_In_Signal.clear();
	vICL_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vICL_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vED_Positions_In_Signal.clear();
	vED_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vED_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vPeak_Positions.clear();
	vPeak_Positions.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vPeak_Positions[i], sizeof (long));

	Local_Activations.load_object_from_stream(File);

	File->read((char*)&S, sizeof (long));
	Silence_Vector.clear();
	Silence_Vector.assign(S,tmpInt);
	for(long i=0;i<S;i++)
	File->read((char*)&Silence_Vector[i], sizeof (int));

	return 1;
	} // v 4

	if( version == 3 )
	{

	Name= Utils.load_String_from_File(File);

	long S;

	File->read((char*)&S, sizeof (long));
	Voltage_Values.clear();
	Voltage_Values.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	{
		File->read((char*)&v, sizeof (float));
		Voltage_Values[i] = v;
	}


	File->read((char*)&S, sizeof (long));
	Phase.clear();
	Phase.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	{
		File->read((char*)&v, sizeof (float));
		Phase[i] = v;
	}
	File->read((char*)&Base_Cycle_Used_For_Phase_ptr, sizeof (double));


	File->read((char*)&Time_Step_ms, sizeof (double));

	File->read((char*)&S, sizeof (long));
	vNAVX_Positions_In_Signal.clear();
	vNAVX_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vNAVX_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vICL_Positions_In_Signal.clear();
	vICL_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vICL_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vED_Positions_In_Signal.clear();
	vED_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vED_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vPeak_Positions.clear();
	vPeak_Positions.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vPeak_Positions[i], sizeof (long));

	Local_Activations.load_object_from_stream(File);

	return 1;
	} // v 3

	if( version == 2 )
	{

	Name= Utils.load_String_from_File(File);

	long S;
	File->read((char*)&S, sizeof (long));
	Voltage_Values.clear();
	Voltage_Values.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	{
		File->read((char*)&v, sizeof (float));
		Voltage_Values[i] = v;
	}

	File->read((char*)&Time_Step_ms, sizeof (double));

	File->read((char*)&S, sizeof (long));
	vNAVX_Positions_In_Signal.clear();
	vNAVX_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vNAVX_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vICL_Positions_In_Signal.clear();
	vICL_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vICL_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vED_Positions_In_Signal.clear();
	vED_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vED_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vPeak_Positions.clear();
	vPeak_Positions.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vPeak_Positions[i], sizeof (long));

	Local_Activations.load_object_from_stream(File);

	return 1;
	} // v2

	if( version == 1 )
	{

	Name= Utils.load_String_from_File(File);

	long S;
	File->read((char*)&S, sizeof (long));
	Voltage_Values.clear();
	Voltage_Values.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	{
		File->read((char*)&v, sizeof (float));
		Voltage_Values[i] = v;
	}

	File->read((char*)&Time_Step_ms, sizeof (double));

	File->read((char*)&S, sizeof (long));
	vNAVX_Positions_In_Signal.clear();
	vNAVX_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vNAVX_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vICL_Positions_In_Signal.clear();
	vICL_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vICL_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vED_Positions_In_Signal.clear();
	vED_Positions_In_Signal.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vED_Positions_In_Signal[i], sizeof (long));

	File->read((char*)&S, sizeof (long));
	vPeak_Positions.clear();
	vPeak_Positions.assign(S,tmp_long);
	for(long i=0;i<S;i++)
	File->read((char*)&vPeak_Positions[i], sizeof (long));

	return 1;
	} // v 1

	return -1;
}
//---------------------------------------------------------------------------

