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
#include "Ablation_Point.h"
//---------------------------------------------------------------------------

Ablation_Point_Class::Ablation_Point_Class()
{
	Identifier = -1;
	Identifier_Hash = -1;
	Identifier_ID = -1;

	GeneratorMedianPower_W = 0;
	DirectSenseImpedanceBase_Ohm = 0;
	DirectSenseImpedanceMin_Ohm = 0;
	DirectSenseImpedanceDrop_Ohm  = 0;

		Session=0;
		ChannelID=0;
		SiteIndex=0;
		x=0;y=0;z=0;
		DurationTime=0;
		AverageForce=0;
		MaxTemperature=0;
		MaxPower=0;
		BaseImpedance=0;
		ImpedanceDrop=0;
		FTI=0;
		RFIndex=0;
		Deleted = false;
}
//---------------------------------------------------------------------------

int Ablation_Point_Class::save_object_to_stream(ofstream* File)
{
		int version = 5;

		File->write((char*)&version, sizeof (int));

		File->write((char*)&Identifier_Hash, sizeof (int));
		File->write((char*)&Identifier_ID, sizeof (int));

		File->write((char*)&Original_x, sizeof (double));
		File->write((char*)&Original_y, sizeof (double));
		File->write((char*)&Original_z, sizeof (double));

		File->write((char*)&Session, sizeof (double));
		File->write((char*)&ChannelID, sizeof (double));
		File->write((char*)&SiteIndex, sizeof (double));
		File->write((char*)&x, sizeof (double));
		File->write((char*)&y, sizeof (double));
		File->write((char*)&z, sizeof (double));
		File->write((char*)&DurationTime, sizeof (double));
		File->write((char*)&AverageForce, sizeof (double));
		File->write((char*)&MaxTemperature, sizeof (double));
		File->write((char*)&MaxPower, sizeof (double));
		File->write((char*)&BaseImpedance, sizeof (double));
		File->write((char*)&ImpedanceDrop, sizeof (double));
		File->write((char*)&FTI, sizeof (double));
		File->write((char*)&RFIndex, sizeof (double));
		File->write((char*)&TagIndexStatus, sizeof (double));

		File->write((char*)&Deleted, sizeof (bool));

		File->write((char*)&GeneratorMedianPower_W, sizeof (double));
		File->write((char*)&DirectSenseImpedanceBase_Ohm, sizeof (double));
		File->write((char*)&DirectSenseImpedanceMin_Ohm, sizeof (double));
		File->write((char*)&DirectSenseImpedanceDrop_Ohm, sizeof (double));

		// navx AutoMarkSummaryList
		File->write((char*)&RF_Episode, sizeof (int));
		File->write((char*)&Lesion_ID, sizeof (int));
		File->write((char*)&Is_Transition, sizeof (int));

		Utils.save_std_string_to_File(File,Start_Time);
		Utils.save_std_string_to_File(File,End_Time);

		File->write((char*)&Energy, sizeof (double));
		File->write((char*)&Avg_Power, sizeof (double));
		File->write((char*)&Avg_Temp, sizeof (double));
		File->write((char*)&Imp_Max, sizeof (double));
		File->write((char*)&Imp_Min, sizeof (double));
		File->write((char*)&Imp_Drop_Perc, sizeof (double));
		File->write((char*)&Avg_Contact_Force_g, sizeof (double));
		File->write((char*)&Min_Contact_Force_g, sizeof (double));
		File->write((char*)&Max_Contact_Force_g, sizeof (double));
		File->write((char*)&LSI, sizeof (double));

		return 1;
}
//---------------------------------------------------------------------------

int Ablation_Point_Class::load_object_from_stream(ifstream* File)
{
	int version;
	File->read((char*)&version, sizeof (int));

	if( version == 5 )
	{
		File->read((char*)&Identifier_Hash, sizeof (int));
		File->read((char*)&Identifier_ID, sizeof (int));

		File->read((char*)&Original_x, sizeof (double));
		File->read((char*)&Original_y, sizeof (double));
		File->read((char*)&Original_z, sizeof (double));

		File->read((char*)&Session, sizeof (double));
		File->read((char*)&ChannelID, sizeof (double));
		File->read((char*)&SiteIndex, sizeof (double));
		File->read((char*)&x, sizeof (double));
		File->read((char*)&y, sizeof (double));
		File->read((char*)&z, sizeof (double));
		File->read((char*)&DurationTime, sizeof (double));
		File->read((char*)&AverageForce, sizeof (double));
		File->read((char*)&MaxTemperature, sizeof (double));
		File->read((char*)&MaxPower, sizeof (double));
		File->read((char*)&BaseImpedance, sizeof (double));
		File->read((char*)&ImpedanceDrop, sizeof (double));
		File->read((char*)&FTI, sizeof (double));
		File->read((char*)&RFIndex, sizeof (double));
		File->read((char*)&TagIndexStatus, sizeof (double));

		File->read((char*)&Deleted, sizeof (bool));

		File->read((char*)&GeneratorMedianPower_W, sizeof (double));
		File->read((char*)&DirectSenseImpedanceBase_Ohm, sizeof (double));
		File->read((char*)&DirectSenseImpedanceMin_Ohm, sizeof (double));
		File->read((char*)&DirectSenseImpedanceDrop_Ohm, sizeof (double));

		// navx AutoMarkSummaryList
		File->read((char*)&RF_Episode, sizeof (int));
		File->read((char*)&Lesion_ID, sizeof (int));
		File->read((char*)&Is_Transition, sizeof (int));

		Start_Time = Utils.load_std_string_from_File(File);
		End_Time = Utils.load_std_string_from_File(File);

		File->read((char*)&Energy, sizeof (double));
		File->read((char*)&Avg_Power, sizeof (double));
		File->read((char*)&Avg_Temp, sizeof (double));
		File->read((char*)&Imp_Max, sizeof (double));
		File->read((char*)&Imp_Min, sizeof (double));
		File->read((char*)&Imp_Drop_Perc, sizeof (double));
		File->read((char*)&Avg_Contact_Force_g, sizeof (double));
		File->read((char*)&Min_Contact_Force_g, sizeof (double));
		File->read((char*)&Max_Contact_Force_g, sizeof (double));
		File->read((char*)&LSI, sizeof (double));

		return 1;

	} // ver 5

	if( version == 4 )
	{
		File->read((char*)&Identifier_Hash, sizeof (int));
		File->read((char*)&Identifier_ID, sizeof (int));

		File->read((char*)&Original_x, sizeof (double));
		File->read((char*)&Original_y, sizeof (double));
		File->read((char*)&Original_z, sizeof (double));

		File->read((char*)&Session, sizeof (double));
		File->read((char*)&ChannelID, sizeof (double));
		File->read((char*)&SiteIndex, sizeof (double));
		File->read((char*)&x, sizeof (double));
		File->read((char*)&y, sizeof (double));
		File->read((char*)&z, sizeof (double));
		File->read((char*)&DurationTime, sizeof (double));
		File->read((char*)&AverageForce, sizeof (double));
		File->read((char*)&MaxTemperature, sizeof (double));
		File->read((char*)&MaxPower, sizeof (double));
		File->read((char*)&BaseImpedance, sizeof (double));
		File->read((char*)&ImpedanceDrop, sizeof (double));
		File->read((char*)&FTI, sizeof (double));
		File->read((char*)&RFIndex, sizeof (double));
		File->read((char*)&TagIndexStatus, sizeof (double));

		File->read((char*)&Deleted, sizeof (bool));

		File->read((char*)&GeneratorMedianPower_W, sizeof (double));
		File->read((char*)&DirectSenseImpedanceBase_Ohm, sizeof (double));
		File->read((char*)&DirectSenseImpedanceMin_Ohm, sizeof (double));
		File->read((char*)&DirectSenseImpedanceDrop_Ohm, sizeof (double));

		return 1;

	} // ver 4

	if( version == 3 )
	{
		File->read((char*)&Identifier_Hash, sizeof (int));
		File->read((char*)&Identifier_ID, sizeof (int));

		File->read((char*)&Original_x, sizeof (double));
		File->read((char*)&Original_y, sizeof (double));
		File->read((char*)&Original_z, sizeof (double));

		File->read((char*)&Session, sizeof (double));
		File->read((char*)&ChannelID, sizeof (double));
		File->read((char*)&SiteIndex, sizeof (double));
		File->read((char*)&x, sizeof (double));
		File->read((char*)&y, sizeof (double));
		File->read((char*)&z, sizeof (double));
		File->read((char*)&DurationTime, sizeof (double));
		File->read((char*)&AverageForce, sizeof (double));
		File->read((char*)&MaxTemperature, sizeof (double));
		File->read((char*)&MaxPower, sizeof (double));
		File->read((char*)&BaseImpedance, sizeof (double));
		File->read((char*)&ImpedanceDrop, sizeof (double));
		File->read((char*)&FTI, sizeof (double));
		File->read((char*)&RFIndex, sizeof (double));
		File->read((char*)&TagIndexStatus, sizeof (double));

		File->read((char*)&Deleted, sizeof (bool));

		return 1;

	} // ver 3

	if( version == 2 )
	{

		File->read((char*)&Session, sizeof (double));
		File->read((char*)&ChannelID, sizeof (double));
		File->read((char*)&SiteIndex, sizeof (double));
		File->read((char*)&x, sizeof (double));
		File->read((char*)&y, sizeof (double));
		File->read((char*)&z, sizeof (double));
		File->read((char*)&DurationTime, sizeof (double));
		File->read((char*)&AverageForce, sizeof (double));
		File->read((char*)&MaxTemperature, sizeof (double));
		File->read((char*)&MaxPower, sizeof (double));
		File->read((char*)&BaseImpedance, sizeof (double));
		File->read((char*)&ImpedanceDrop, sizeof (double));
		File->read((char*)&FTI, sizeof (double));
		File->read((char*)&RFIndex, sizeof (double));
		File->read((char*)&TagIndexStatus, sizeof (double));

		File->read((char*)&Deleted, sizeof (bool));

		return 1;

	} // ver 2

	if( version == 1 )
	{

		File->read((char*)&Session, sizeof (double));
		File->read((char*)&ChannelID, sizeof (double));
		File->read((char*)&SiteIndex, sizeof (double));
		File->read((char*)&x, sizeof (double));
		File->read((char*)&y, sizeof (double));
		File->read((char*)&z, sizeof (double));
		File->read((char*)&DurationTime, sizeof (double));
		File->read((char*)&AverageForce, sizeof (double));
		File->read((char*)&MaxTemperature, sizeof (double));
		File->read((char*)&MaxPower, sizeof (double));
		File->read((char*)&BaseImpedance, sizeof (double));
		File->read((char*)&ImpedanceDrop, sizeof (double));
		File->read((char*)&FTI, sizeof (double));
		File->read((char*)&RFIndex, sizeof (double));

		File->read((char*)&Deleted, sizeof (bool));

		return 1;

	} // ver 1

	return -1;
}
//---------------------------------------------------------------------------

