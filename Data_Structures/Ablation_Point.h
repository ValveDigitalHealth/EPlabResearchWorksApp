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
#ifndef Ablation_PointH
#define Ablation_PointH

#include "Utils.h"
#include <fstream>
#include<vcl.h>

//---------------------------------------------------------------------------

class Ablation_Point_Class
{
	public:

	Utils_Tools Utils;

	Ablation_Point_Class();

	double x,y,z;
	double Projected_x,Projected_y,Projected_z;


	double Session,ChannelID,SiteIndex,DurationTime,AverageForce,
				MaxTemperature,MaxPower,BaseImpedance,ImpedanceDrop,FTI,RFIndex,TagIndexStatus;

	double GeneratorMedianPower_W,DirectSenseImpedanceBase_Ohm;
	double DirectSenseImpedanceMin_Ohm,DirectSenseImpedanceDrop_Ohm;

	double Energy_Delivered,Time_of_Energy_Delivery,Power;

	std::string Start_Time,End_Time;

	bool Deleted;

	int Identifier;
	int Identifier_Hash;
	int Identifier_ID;
	double Original_x,Original_y,Original_z;

	//---------------------------------------------------------
	// Save/load object to/from stream
	//---------------------------------------------------------
	int save_object_to_stream(ofstream* File);
	int load_object_from_stream(ifstream* File);

	// unique for navx section (AutoMarkSummaryList)
	int RF_Episode, Lesion_ID,Is_Transition;
	double Duration,Energy,Avg_Power, Avg_Temp, Max_Temp,Imp_Max,Imp_Min,Imp_Drop,Imp_Drop_Perc;
	double Avg_Contact_Force_g, Min_Contact_Force_g, Max_Contact_Force_g,LSI;

};


#endif
