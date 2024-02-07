//---------------------------------------------------------------------------

#include<vector>

#ifndef Closest_DPs_to_node_classH
#define Closest_DPs_to_node_classH
//---------------------------------------------------------------------------

class Closest_DPs_To_Node_Class
{
	public:

	Closest_DPs_To_Node_Class();

		long Closest_Data_Point_Id;          // to ma byc zlaezne od dpsetu...
		long Closest_Data_Point_Id_Unlimited;
		std::vector <long> Closest_Data_Points_Ptrs; // not saved
		std::vector <float> Closest_Data_Points_Distances; // not saved
		bool Data_Point_Nearby;

};
#endif

