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
#include "Utils.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

void Row::clear()
{
        for(int i=0;i<2000;i++)
                Element[i]="";
}

//---------------------------------------------------------------------------

void Utils_Tools::set_combobox_selection(TComboBox *ComboBox,AnsiString Selection_Text)
{
	for(int i=0;i<ComboBox->Items->Count;i++)
	if( ComboBox->Items->Strings[i].Trim() == Selection_Text.Trim() )
	ComboBox->ItemIndex = i;
}

//---------------------------------------------------------------------------

void Utils_Tools::read_one_column_file_into_vector(AnsiString Filename, std::vector <double> &Vec, double multiplier)
{
    ifstream df;
    double CL;
    char c[500];
    int count;

		Vec.clear();

		df.open(Filename.c_str());

        if( df.is_open() )
        {

        df >> count;
        for(int i=0;i<count;i++)
        {
			df >> c;
            CL = atof(c);
            Vec.push_back(multiplier*CL);
        }

        }

        df.close();
/*
///
        ofstream df2;
        df2.open(Filename.c_str());
        df2 << count -1 << "\n";
        for(int i=0;i<Vec.size()-1;i++)
        {
            df2 << Vec[i] << "\n";
        }


        df2.close();
*/
}

//---------------------------------------------------------------------------

int Utils_Tools::is_pentaray_type_name_v2(AnsiString Name)
{
	AnsiString Trimmed_Name = Name.Trim();

   if(  Trimmed_Name == "SPA-D-2" || Trimmed_Name == "SPA-2-3" || Trimmed_Name == "SPA-3-4" ||
		Trimmed_Name == "SPB-D-2" || Trimmed_Name == "SPB-2-3" || Trimmed_Name == "SPB-3-4" ||
		Trimmed_Name == "SPC-D-2" || Trimmed_Name == "SPC-2-3" || Trimmed_Name == "SPC-3-4" ||
		Trimmed_Name == "SPD-D-2" || Trimmed_Name == "SPD-2-3" || Trimmed_Name == "SPD-3-4" ||
		Trimmed_Name == "SPE-D-2" || Trimmed_Name == "SPE-2-3" || Trimmed_Name == "SPE-3-4" )
   return 1;

   if(  Trimmed_Name == "A---D-2" || Trimmed_Name == "A---2-3" || Trimmed_Name == "A---3-4" ||
		Trimmed_Name == "B---D-2" || Trimmed_Name == "B---2-3" || Trimmed_Name == "B---3-4" ||
		Trimmed_Name == "C---D-2" || Trimmed_Name == "C---2-3" || Trimmed_Name == "C---3-4" ||
		Trimmed_Name == "D---D-2" || Trimmed_Name == "D---2-3" || Trimmed_Name == "D---3-4" ||
		Trimmed_Name == "E---D-2" || Trimmed_Name == "E---2-3" || Trimmed_Name == "E---3-4" )
   return 2;

   return -1;
}

//---------------------------------------------------------------------------

AnsiString Utils_Tools::add_file_extension(AnsiString Filename,AnsiString Extension)
{
	if( !is_substring_present(Filename,Extension) )
	Filename += Extension;

	return Filename;
}

//---------------------------------------------------------------------------

bool Utils_Tools::is_lasso_type_name(AnsiString Name)
{
	AnsiString Trimmed_Name = Name.Trim();

//	if( is_substring_present(Trimmed_Name,"LAS")  ||
//		is_substring_present(Trimmed_Name,"Spir") ||
//		is_substring_present(Trimmed_Name,"Circ") ||
//		is_substring_present(Trimmed_Name,"LS") )
	if( is_substring_present(Trimmed_Name.UpperCase() ,"LAS")  ||
		is_substring_present(Trimmed_Name.UpperCase(),"SPIR") ||
		is_substring_present(Trimmed_Name.UpperCase(),"D") ||
		is_substring_present(Trimmed_Name.UpperCase(),"CIRC") ||
		is_substring_present(Trimmed_Name.UpperCase(),"SPI") ||
		is_substring_present(Trimmed_Name.UpperCase(),"LS") )
	return true;
	else
	return false;

}

//---------------------------------------------------------------------------

void Utils_Tools::get_spline_and_electrode_from_pentray_name(AnsiString Name,int* Spline,int* Electrode)
{
	AnsiString As,as1,as2,as3;

	As = Name.Trim();

    if( is_pentaray_type_name_v2(Name) == 1)
    {

    as1 = As.SubString(3,1); // example: A1,2
    as2 = As.SubString(5,1);
    as3 = As.SubString(7,1);

    // Get electrode number
	if( as2 == "1" ) Electrode[0] = 3;
	if( as2 == "D" ) Electrode[0] = 3;
	if( as2 == "2" ) Electrode[0] = 2;
    if( as2 == "3" ) Electrode[0] = 1;

    if( as1 == "A" ) Spline[0] = 0;
    if( as1 == "B" ) Spline[0] = 1;
    if( as1 == "C" ) Spline[0] = 2;
    if( as1 == "D" ) Spline[0] = 3;
    if( as1 == "E" ) Spline[0] = 4;

    }
    else
    if( is_pentaray_type_name_v2(Name) == 2)
	{

    as1 = As.SubString(1,1); // example: A1,2
    as2 = As.SubString(5,1);
    as3 = As.SubString(7,1);

    // Get electrode number
    if( as2 == "1" ) Electrode[0] = 3;
    if( as2 == "D" ) Electrode[0] = 3;
    if( as2 == "2" ) Electrode[0] = 2;
    if( as2 == "3" ) Electrode[0] = 1;

    if( as1 == "A" ) Spline[0] = 0;
    if( as1 == "B" ) Spline[0] = 1;
    if( as1 == "C" ) Spline[0] = 2;
    if( as1 == "D" ) Spline[0] = 3;
    if( as1 == "E" ) Spline[0] = 4;

    }
    else
    {
        Spline[0]=-1;
        Electrode[0]=-1;
	}
}

//---------------------------------------------------------------------------

bool Utils_Tools::is_substring_present(AnsiString Main_String,AnsiString Searched_Phrase)
{
    bool present=false;
    AnsiString Sub;

    for(int i=0;i<Main_String.Length()-Searched_Phrase.Length()+2;i++)
    {
        Sub = Main_String.SubString(i,Searched_Phrase.Length());

        if( Sub == Searched_Phrase )
        present = true;
    }

	return present;
}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::get_string_after_last_occurence_of_specified_string(AnsiString Main_String,
					AnsiString Given_String)
{
	AnsiString Readed_Content="";
	std::vector <AnsiString> Set;

	for(int i=1;i<=Main_String.Length();i++)
	{

	if( Main_String.SubString(i,Given_String.Length()) == Given_String )
	{
		Set.push_back(Readed_Content);
		Readed_Content = "";
	}
	else
		Readed_Content += Main_String.SubString(i,1);
	}

//	Set.push_back(Readed_Content); // last item;

	return Readed_Content;

}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::get_string_before_last_occurence_of_specified_string(AnsiString Main_String,
					AnsiString Given_String)
{
	long Pos = -1;

	for(int i=1;i<=Main_String.Length();i++)
	if( Main_String.SubString(i,Given_String.Length()) == Given_String )
		Pos = i;

	AnsiString AS = Main_String.SubString(1,Pos-1);
	if( Pos > 0 )
	return AS;
	else
	return "";
}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::get_string_after_given_occurence_of_specified_string
		(AnsiString Main_String,AnsiString Given_String, int Which_Occurence)
{
	AnsiString Readed_Content="";
	std::vector <AnsiString> Set;
	int Strict_Hit_Count=0;

	for(int i=1;i<=Main_String.Length();i++)
	{

	if( Main_String.SubString(i,Given_String.Length()) == Given_String ||
		Main_String.SubString(i,Given_String.Length()) == "." )
	{
		Set.push_back(Readed_Content);
//        i+=Given_String.Length();
		Readed_Content = "";

		if( Main_String.SubString(i,Given_String.Length()) == Given_String )
		Strict_Hit_Count++;
	}
	else
		Readed_Content += Main_String.SubString(i,1);


	}

	Set.push_back(Readed_Content); // last item;

	if( Strict_Hit_Count >= Which_Occurence )
	return Set[Which_Occurence];
	else
	return "X";
}

//------------------------------------------------------------------------------
AnsiString Utils_Tools::get_string_before_given_occurence_of_specified_string
		(AnsiString Main_String,AnsiString Given_String, int Which_Occurence)
{
	AnsiString Readed_Content="";
	std::vector <AnsiString> Set;
	int Strict_Hit_Count=0;

	for(int i=1;i<=Main_String.Length();i++)
	{

	if( Main_String.SubString(i,Given_String.Length()) == Given_String ||
		Main_String.SubString(i,Given_String.Length()) == "." )
	{
		Set.push_back(Readed_Content);
		Readed_Content = "";

		if( Main_String.SubString(i,Given_String.Length()) == Given_String )
		Strict_Hit_Count++;
	}
	else
		Readed_Content += Main_String.SubString(i,1);


	}

	Set.push_back(Readed_Content); // last item;

//	if( Strict_Hit_Count >= Which_Occurence && Which_Occurence-1 >= 0 )
	if( Which_Occurence-1 >= 0 )
	return Set[Which_Occurence-1];
	else
	return "X";
}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::get_file_name_from_full_path(AnsiString Full_Path)
{
	AnsiString File_Name="";
	int pos=-1;
	bool found=false;

	// find last position of "/" or "\" characters
	for(int i=Full_Path.Length()-1;i>0;i--)
	if (!found)
	if( Full_Path.SubString(i,1) == "\\" ||
		Full_Path.SubString(i,1) == "/" )
	{
		pos = i;
		found = true;
	}

	return Full_Path.SubString(pos+1,Full_Path.Length()-pos);
}

//------------------------------------------------------------------------------
AnsiString Utils_Tools::get_only_path_from_full_path(AnsiString Full_Path)
{
	AnsiString File_Name="";
	int pos=-1;
	bool found=false;

	// find last position of "/" or "\" characters
	for(int i=Full_Path.Length()-1;i>0;i--)
	if (!found)
	if( Full_Path.SubString(i,1) == "\\" ||
		Full_Path.SubString(i,1) == "/" )
	{
		pos = i;
		found = true;
	}

	return Full_Path.SubString(0,pos);
}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::get_string_before_given_text(AnsiString Main_String,
                                                AnsiString Ending_Phrase)
{
    bool present=false;
    AnsiString Sub;
    int ptr;

    for(int i=0;i<Main_String.Length()-Ending_Phrase.Length()+2;i++)
	if( !present )
	{
        Sub = Main_String.SubString(i,Ending_Phrase.Length());

        if( Sub == Ending_Phrase )
        {
			present = true;
            ptr = i;
        }
    }

    if(present)
    {
        Sub = Main_String.SubString(0,ptr-1);
        return Sub;
    }
    else
        return "Unknown";

}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::get_string_after_given_text(AnsiString Main_String,
                                                AnsiString Ending_Phrase)
{
    bool present=false;
    AnsiString Sub;
    int ptr;

    for(int i=0;i<Main_String.Length()-Ending_Phrase.Length()+2;i++)
    {
        Sub = Main_String.SubString(i,Ending_Phrase.Length());

        if( Sub == Ending_Phrase )
        {
            present = true;
            ptr = i;
        }
    }

    if(present)
    {
        Sub = Main_String.SubString(ptr+1,Main_String.Length()-ptr);
        return Sub;
    }
    else
        return "Unknown";

}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::remove_substring_from_string(AnsiString Main_String,AnsiString String_2_Remove)
{
    AnsiString Sub;

    for(int i=0;i<Main_String.Length()-String_2_Remove.Length()+2;i++)
    {
        Sub = Main_String.SubString(i,String_2_Remove.Length());

        if( Sub == String_2_Remove )
            Main_String = Main_String.Delete(i,String_2_Remove.Length());
    }

    return Main_String;
}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::replace_substring_from_string(AnsiString Main_String,
		AnsiString String_2_Replace, AnsiString With_What)
{
	AnsiString Sub;

	for(int i=0;i<Main_String.Length()-String_2_Replace.Length()+2;i++)
	{
		Sub = Main_String.SubString(i,String_2_Replace.Length());

		if( Sub == String_2_Replace )
		{
			Main_String = Main_String.Delete(i,String_2_Replace.Length());
			Main_String.Insert(With_What,i);
		}
	}

	return Main_String;
}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::get_string_between_two_texts(AnsiString Main_String,
	AnsiString First_String,AnsiString Second_String,bool* Found_Flag)
{
	AnsiString Sub;
	int First=-1,Second=-1;

	for(int i=0;i<Main_String.Length()-Second_String.Length()+2;i++)
	{
		Sub = Main_String.SubString(i,First_String.Length());
		if( Sub == First_String && First == -1)
		First=i;

		Sub = Main_String.SubString(i,Second_String.Length());
		if( Sub == Second_String && Second == -1)
		Second=i;
	}

	if( First >= 0 && Second >= 0 )
	{
	Sub = Main_String.SubString(First+First_String.Length(),
				Second-First-First_String.Length());
	Found_Flag[0]=true;
	}
	else
	{
	Sub = "";
	Found_Flag[0]=false;
	}

    return Sub;
}

//------------------------------------------------------------------------------

AnsiString Utils_Tools::get_string_from_the_middle(AnsiString Main_String,AnsiString Start_Stop_String)
{
    AnsiString Sub;
    int Start=-1,Stop=-1;

    for(int i=0;i<Main_String.Length()-Start_Stop_String.Length()+2;i++)
    {
        Sub = Main_String.SubString(i,Start_Stop_String.Length());

        if( Sub == Start_Stop_String && Stop == -1)
        Start=i;

        if( Sub == Start_Stop_String && Start != -1)
        Stop=i;
    }

    Sub = Main_String.SubString(Start+Start_Stop_String.Length(),Stop-Start-Start_Stop_String.Length());

    return Sub;
}

//------------------------------------------------------------------------------


Diagram_Element::Diagram_Element()
{
        Counter = 0;

        Phase_Code = -1;
        Phase_Name = "N/A";

        Transition_Code = -1;
        Transition_Name = "N/A";

        Type = -1; // 0 if State, 1 if Transition

        Cell_X = -1;
        Cell_Y = -1; // position in 2D grid
}
//------------------------------------------------------------------------------

State_Diagram::State_Diagram()
{
    Starting_Cell_X = 1;
    Starting_Cell_Y = 1;

    Cell_Width = 120; // in pixels ///!!!!!! Its stored in plaque file!!!
    Cell_Height = 40; // in pixels

    Box_Width = 250; // in pixels, box is inside the cell, so it should be smaller
    Box_Height = 30; // in pixels, box is inside the cell, so it should be smaller

    State_Box_Color = clWhite;
    Transition_Box_Color = clGray; // or maybe pass colors from phases definitions?
}
//------------------------------------------------------------------------------

bool State_Diagram::time_sequenced_elements_proper_syntax()
{
    if(Time_Sequenced_Elements[0].Type != 0 ) // not starting with phase box
    return false;

    if(Time_Sequenced_Elements[Time_Sequenced_Elements.size()-1].Type != 0 )
        // not ending with phase box
    return false;

    int error=0;
	for(int e=1;e<(signed)Time_Sequenced_Elements.size()-1;e=e+2)
    {
        if(Time_Sequenced_Elements[e].Type != 1 ) error++;
        if(Time_Sequenced_Elements[e+1].Type != 0 ) error++;
    }

	if(error>0)
    return false;

    return true;
}

//------------------------------------------------------------------------------

int State_Diagram::get_ptr_of_this_state(int Phase_Code)
{
	for(int e=0;e<(signed)Diagram_Elements.size();e++)
    if(Diagram_Elements[e].Phase_Code == Phase_Code)
    return e;

    return -1;
}

//------------------------------------------------------------------------------
int State_Diagram::get_ptr_of_this_transition(int Transition_Code) // if transition is present returns ptr, else returns -1
{
	for(int e=0;e<(signed)Diagram_Elements.size();e++)
    if(Diagram_Elements[e].Transition_Code == Transition_Code)
    return e;

    return -1;
}            

//------------------------------------------------------------------------------

int State_Diagram::is_this_sequence_present(int Phase_Code1,int Transition_Code,int Phase_Code2)
{

	for(int e=0;e<(signed)Diagram_Elements.size()-2;e++)
    if(Diagram_Elements[e].Phase_Code == Phase_Code1)
    if(Diagram_Elements[e+1].Transition_Code == Transition_Code)
    if(Diagram_Elements[e+2].Phase_Code == Phase_Code2)
    return e+1;

    return -1;
}

//------------------------------------------------------------------------------

int State_Diagram::is_this_sequence_present(int Phase_Code1,int Transition_Code)
{
	for(int e=0;e<(signed)Diagram_Elements.size()-1;e++)
    if(Diagram_Elements[e].Phase_Code == Phase_Code1)
    if(Diagram_Elements[e+1].Transition_Code == Transition_Code)
    return e+1;

    return -1;
}

//------------------------------------------------------------------------------

int State_Diagram::which_box_is_at_this_location(int X,int Y)
{
	for(int e=0;e<(signed)Diagram_Elements.size();e++)
    if(Diagram_Elements[e].Cell_X == X)
    if(Diagram_Elements[e].Cell_Y == Y)
    return e;

    return -1;
}

//------------------------------------------------------------------------------

bool State_Diagram::is_this_spot_free(int X,int Y)
{
	for(int e=0;e<(signed)Diagram_Elements.size();e++)
    if( Diagram_Elements[e].Cell_X == X )
    if( Diagram_Elements[e].Cell_Y == Y )
    return false;

    return true;
}

//------------------------------------------------------------------------------

bool State_Diagram::find_free_spot_around(int Ptr,int* X,int* Y)
{
    int cX,cY;
    int Spacing = 2;

    cX = Diagram_Elements[Ptr].Cell_X;
    cY = Diagram_Elements[Ptr].Cell_Y;

    if( is_this_spot_free(cX,cY+Spacing) ) { X[0]=cX; Y[0]=cY+Spacing; return true; }
    if( is_this_spot_free(cX+Spacing,cY) ) { X[0]=cX+Spacing; Y[0]=cY; return true; }
    if( is_this_spot_free(cX+Spacing,cY-Spacing) ) { X[0]=cX+Spacing; Y[0]=cY-Spacing; return true; }
    if( is_this_spot_free(cX+Spacing,cY+Spacing) ) { X[0]=cX+Spacing; Y[0]=cY+Spacing; return true; }
    if( is_this_spot_free(cX,cY-Spacing) ) { X[0]=cX; Y[0]=cY-Spacing; return true; }

    if( is_this_spot_free(cX+2*Spacing,cY-Spacing) ) { X[0]=cX+2*Spacing; Y[0]=cY-Spacing; return true; }
    if( is_this_spot_free(cX+2*Spacing,cY) ) { X[0]=cX+2*Spacing; Y[0]=cY; return true; }
    if( is_this_spot_free(cX+2*Spacing,cY+Spacing) ) { X[0]=cX+2*Spacing; Y[0]=cY+Spacing; return true; }

    return false;
}

//------------------------------------------------------------------------------
int State_Diagram::reduce_time_sequenced_elements_to_diagram()
{
    int ptr,X,Y;
    int this_state_ptr;
	int prev_state_ptr;

    // put first element (pacing phase)
    Diagram_Elements.push_back(Time_Sequenced_Elements[0]);
    Diagram_Elements[0].Cell_X = Starting_Cell_X;
    Diagram_Elements[0].Cell_Y = Starting_Cell_Y;
//    Diagram_Elements[0].Counter = 1;

    // put second element (pacing termination)
    Diagram_Elements.push_back(Time_Sequenced_Elements[1]);
    Diagram_Elements[1].Cell_X = Starting_Cell_X;
    Diagram_Elements[1].Cell_Y = Starting_Cell_Y+1;
//    Diagram_Elements[1].Counter = 1;
    Diagram_Elements[1].Connections_Ptrs.push_back(0);

    // go through all sequential elements
    for(int e=2;e<(signed)Time_Sequenced_Elements.size();e++)
    {

    prev_state_ptr = get_ptr_of_this_state(Time_Sequenced_Elements[e-2].Phase_Code);
        this_state_ptr = get_ptr_of_this_state(Time_Sequenced_Elements[e].Phase_Code);

    // ok, lets begin
    ptr = is_this_sequence_present(Time_Sequenced_Elements[e-2].Phase_Code,
                                   Time_Sequenced_Elements[e-1].Transition_Code,
                                   Time_Sequenced_Elements[e].Phase_Code);


    //-----------------------------------------
	if( ptr >= 0 ) // if P1-T-P2 sequence present in diagram
    //-----------------------------------------
    {
        // increase transition and phase counter
//        Diagram_Elements[ptr].Counter++;
        this_state_ptr = get_ptr_of_this_state(Time_Sequenced_Elements[e].Phase_Code);
//        Diagram_Elements[this_state_ptr].Counter++;
    }
    else // if sequence not present in diagram
    {
            ptr = is_this_sequence_present(Time_Sequenced_Elements[e-2].Phase_Code,
                                           Time_Sequenced_Elements[e-1].Transition_Code);

            //-----------------------------------------
            if(ptr>=0) // if P1-T sequence present in diagram
            //-----------------------------------------
            {
                if( this_state_ptr >= 0 ) // is P2 present
                {
                    // just link T and P2
                    Diagram_Elements[this_state_ptr].Connections_Ptrs.push_back(ptr);
//                    Diagram_Elements[ptr].Counter++;
                }
                else // P2 not yet present
                {
                    // create P2 around T box and link with T
                    find_free_spot_around(ptr,&X,&Y);
                    Diagram_Elements.push_back(Time_Sequenced_Elements[e]);
                    Diagram_Elements[Diagram_Elements.size()-1].Cell_X = X;
					Diagram_Elements[Diagram_Elements.size()-1].Cell_Y = Y;
//                    Diagram_Elements[Diagram_Elements.size()-1].Counter = 1;
                    Diagram_Elements[Diagram_Elements.size()-1].Connections_Ptrs.
                            push_back(ptr);
                }
            }
            else // no P1-T existing
            {

                // find spot for transition box around P1 box
                find_free_spot_around(prev_state_ptr,&X,&Y);
                // push transition there
                Diagram_Elements.push_back(Time_Sequenced_Elements[e-1]);
                Diagram_Elements[Diagram_Elements.size()-1].Cell_X = X;
                Diagram_Elements[Diagram_Elements.size()-1].Cell_Y = Y;
//                Diagram_Elements[Diagram_Elements.size()-1].Counter = 1;
                Diagram_Elements[Diagram_Elements.size()-1].Connections_Ptrs.
                        push_back(prev_state_ptr);

                if( this_state_ptr >= 0 ) // is P2 present
                {
                    // link P2 with newly created transition
                    Diagram_Elements[this_state_ptr].Connections_Ptrs.
                        push_back(Diagram_Elements.size()-1);
                }
                else // no P2 present
                {
                    // find space around newly created transition and create P2 there
                    find_free_spot_around(Diagram_Elements.size()-1,&X,&Y);

                    Diagram_Elements.push_back(Time_Sequenced_Elements[e]);
                    Diagram_Elements[Diagram_Elements.size()-1].Cell_X = X;
                    Diagram_Elements[Diagram_Elements.size()-1].Cell_Y = Y;
//                    Diagram_Elements[Diagram_Elements.size()-1].Counter = 1;
                    Diagram_Elements[Diagram_Elements.size()-1].Connections_Ptrs.
                        push_back(Diagram_Elements.size()-2);
                }

            }

     } // if P1-T-P2 not existing

	} // through all source elements

	return 1;
}

//------------------------------------------------------------------------------

int Mode_Types_Set::read_mode_types(AnsiString Application_Directory, AnsiString Filename)
{
    AnsiString As;
	std::vector <Row> Rows;
    int Current_Column;
    int Iterator;
    char string[2000];
    Row Row1;
    char c;
    TColor TC;

    ifstream dfile((Application_Directory+Filename).c_str());

	if( dfile == NULL )
    {
			ShowMessage("\n Error : can't open "+Filename );
			return -1;
    }
    else
    {


    Iterator = 0;
    while( !dfile.eof() && Iterator < 10000 )
    {

    Row1.clear();
    Current_Column = 0;

    while(c!=10 && !dfile.eof()) // till end of line
    {
        dfile.get(c);
        if( c == 32 || c == 9 ) // space or tab
            {
                Current_Column++;
                dfile.get(c);
            }

        if( c!= 10 && !dfile.eof() )
			Row1.Element[Current_Column] += c;
    } // reading line

    Rows.push_back(Row1);
    c = 0;

    Iterator++;

    } // reading file

    Mode_Type MT;
    Types.clear();


    for(int i=1;i<(signed)Rows.size();i++)
    if( Rows[i].Element[0].Trim() != "" )
    {
        MT.Mode_Type_Code = Rows[i].Element[0].ToInt();

        MT.R = Rows[i].Element[1].ToInt();
        MT.R = Rows[i].Element[2].ToInt();
        MT.R = Rows[i].Element[3].ToInt();

        MT.Type_Color= (TColor)RGB(Rows[i].Element[1].ToInt(),
                                   Rows[i].Element[2].ToInt(),
                                   Rows[i].Element[3].ToInt());

        As = "";
        for(int k=4;k<20;k++)
		As += Rows[i].Element[k].Trim()+" ";

        MT.Mode_Type_Name = As.Trim();

        Types.push_back(MT);
    }

    dfile.clear();
    dfile.close();

    return 1;

    }

	return -1;

}
//---------------------------------------------------------------------------------

TColor Mode_Types_Set::get_mode_type_color(int Code)
{
    for(int i=0;i<(signed)Types.size();i++)
    if( Code == Types[i].Mode_Type_Code )
    return Types[i].Type_Color;

	return clBlack;
}
//---------------------------------------------------------------------------

bool Utils_Tools::is_this_basket_name_Hamburg_Navx(AnsiString Name,
								AnsiString* Spline,AnsiString* Electrode,
								int* Spline_Id, int* Electrode_Id )
{
// syntax: A-D, A-2, etc...

	AnsiString Trimmed_Name = Name.Trim();
	AnsiString L1 = Trimmed_Name.SubString(1,1);
	AnsiString L2 = Trimmed_Name.SubString(2,1);
	AnsiString L3 = Trimmed_Name.SubString(3,1);

	if( L1 == "A" || L1 == "B" || L1 == "C" || L1 == "D" || L1 == "E" ||
		L1 == "F" || L1 == "G" || L1 == "H" )
	if( L2 == "-" )
	if( L3 == "D" || L3 == "1" || L3 == "2" || L3 == "3" || L3 == "4" || L3 == "5" || L3 == "6" ||
		L3 == "7" || L3 == "8")
	{
	Spline[0] = L1;
	Electrode[0] = L3;

		if( Spline[0] == "A" || Spline[0] == "1") Spline_Id[0] = 0;
		if( Spline[0] == "B" || Spline[0] == "2") Spline_Id[0] = 1;
		if( Spline[0] == "C" || Spline[0] == "3") Spline_Id[0] = 2;
		if( Spline[0] == "D" || Spline[0] == "4") Spline_Id[0] = 3;
		if( Spline[0] == "E" || Spline[0] == "5") Spline_Id[0] = 4;
		if( Spline[0] == "F" || Spline[0] == "6") Spline_Id[0] = 5;
		if( Spline[0] == "G" || Spline[0] == "7") Spline_Id[0] = 6;
		if( Spline[0] == "H" || Spline[0] == "8") Spline_Id[0] = 7;

		if( Electrode[0] == "1" || Electrode[0] == "D" ) Electrode_Id[0] = 0;
		if( Electrode[0] == "2" ) Electrode_Id[0] = 1;
		if( Electrode[0] == "3" ) Electrode_Id[0] = 2;
		if( Electrode[0] == "4" ) Electrode_Id[0] = 3;
		if( Electrode[0] == "5" ) Electrode_Id[0] = 4;
		if( Electrode[0] == "6" ) Electrode_Id[0] = 5;
		if( Electrode[0] == "7" ) Electrode_Id[0] = 6;
		if( Electrode[0] == "8" ) Electrode_Id[0] = 7;

	return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool Utils_Tools::is_this_basket_name_Maastricht_Navx(AnsiString Name,
								AnsiString* Spline,AnsiString* Electrode,
								int* Spline_Id, int* Electrode_Id)
{
// syntax: 48A-D, 48A-2, etc...

	AnsiString Trimmed_Name = Name.Trim();
	AnsiString L1 = Trimmed_Name.SubString(3,1);
	AnsiString L2 = Trimmed_Name.SubString(4,1);
	AnsiString L3 = Trimmed_Name.SubString(5,1);

	if( L1 == "A" || L1 == "B" || L1 == "C" || L1 == "D" || L1 == "E" ||
		L1 == "F" || L1 == "G" || L1 == "H" )
	if( L2 == "-" )
	if( L3 == "D" || L3 == "1" || L3 == "2" || L3 == "3" || L3 == "4" || L3 == "5" || L3 == "6" ||
		L3 == "7" || L3 == "8")
	{
	Spline[0] = L1;
	Electrode[0] = L3;

		if( Spline[0] == "A" || Spline[0] == "1") Spline_Id[0] = 0;
		if( Spline[0] == "B" || Spline[0] == "2") Spline_Id[0] = 1;
		if( Spline[0] == "C" || Spline[0] == "3") Spline_Id[0] = 2;
		if( Spline[0] == "D" || Spline[0] == "4") Spline_Id[0] = 3;
		if( Spline[0] == "E" || Spline[0] == "5") Spline_Id[0] = 4;
		if( Spline[0] == "F" || Spline[0] == "6") Spline_Id[0] = 5;
		if( Spline[0] == "G" || Spline[0] == "7") Spline_Id[0] = 6;
		if( Spline[0] == "H" || Spline[0] == "8") Spline_Id[0] = 7;

		if( Electrode[0] == "1" || Electrode[0] == "D" ) Electrode_Id[0] = 0;
		if( Electrode[0] == "2" ) Electrode_Id[0] = 1;
		if( Electrode[0] == "3" ) Electrode_Id[0] = 2;
		if( Electrode[0] == "4" ) Electrode_Id[0] = 3;
		if( Electrode[0] == "5" ) Electrode_Id[0] = 4;
		if( Electrode[0] == "6" ) Electrode_Id[0] = 5;
		if( Electrode[0] == "7" ) Electrode_Id[0] = 6;
		if( Electrode[0] == "8" ) Electrode_Id[0] = 7;

	return true;
	}

   return false;
}

//---------------------------------------------------------------------------
bool Utils_Tools::is_this_basket_name_Flinders_Navx(AnsiString Name,
								AnsiString* Spline,AnsiString* Electrode,
								int* Spline_Id, int* Electrode_Id)
{
// syntax: A48-D,


// bylo 48A-2, etc...

	AnsiString Trimmed_Name = Name.Trim();
	AnsiString L1 = Trimmed_Name.SubString(1,1);
	AnsiString L2 = Trimmed_Name.SubString(4,1);
	AnsiString L3 = Trimmed_Name.SubString(5,1);

	if( L1 == "A" || L1 == "B" || L1 == "C" || L1 == "D" || L1 == "E" ||
		L1 == "F" || L1 == "G" || L1 == "H" )
	if( L2 == "-" )
	if( L3 == "D" || L3 == "1" || L3 == "2" || L3 == "3" || L3 == "4" || L3 == "5" || L3 == "6" ||
		L3 == "7" || L3 == "8")
	{
	Spline[0] = L1;
	Electrode[0] = L3;

		if( Spline[0] == "A" || Spline[0] == "1") Spline_Id[0] = 0;
		if( Spline[0] == "B" || Spline[0] == "2") Spline_Id[0] = 1;
		if( Spline[0] == "C" || Spline[0] == "3") Spline_Id[0] = 2;
		if( Spline[0] == "D" || Spline[0] == "4") Spline_Id[0] = 3;
		if( Spline[0] == "E" || Spline[0] == "5") Spline_Id[0] = 4;
		if( Spline[0] == "F" || Spline[0] == "6") Spline_Id[0] = 5;
		if( Spline[0] == "G" || Spline[0] == "7") Spline_Id[0] = 6;
		if( Spline[0] == "H" || Spline[0] == "8") Spline_Id[0] = 7;

		if( Electrode[0] == "1" || Electrode[0] == "D" ) Electrode_Id[0] = 0;
		if( Electrode[0] == "2" ) Electrode_Id[0] = 1;
		if( Electrode[0] == "3" ) Electrode_Id[0] = 2;
		if( Electrode[0] == "4" ) Electrode_Id[0] = 3;
		if( Electrode[0] == "5" ) Electrode_Id[0] = 4;
		if( Electrode[0] == "6" ) Electrode_Id[0] = 5;
		if( Electrode[0] == "7" ) Electrode_Id[0] = 6;
		if( Electrode[0] == "8" ) Electrode_Id[0] = 7;

	return true;
	}

   return false;
}

//---------------------------------------------------------------------------

void Utils_Tools::save_ansistrings_variable_to_file(ofstream* File,AnsiString S1,AnsiString S2,AnsiString S3,int Type_Flag)
{
	// just line with variable name and value
	if( Type_Flag == 1 )
	{
		File[0] << "\n";
		File[0] << S1.Trim().c_str() << "\t";
		File[0] << S2.Trim().c_str() << "\t";
		File[0] << S3.Trim().c_str();
	}

	// just value of the variable
	if( Type_Flag == 2 )
	{
		File[0] << remove_substring_from_string(S2.Trim(),",").c_str() << ",";
	}

	// just name of the variable (cut seven characters to get rid of "START")
	if( Type_Flag == 3 )
	{
		File[0] << (S1.SubString(1,S1.Length()-7)).Trim().c_str() << ",";
	}
}

//---------------------------------------------------------------------------

void Utils_Tools::load_ansistrings_variable_from_file(ifstream* File,AnsiString S1,AnsiString* S2,AnsiString S3)
{
	char string[2000];
	AnsiString Result="";
	File[0].clear();
	File[0].seekg(0, File[0].beg);

	while( !File[0].eof() )
	{
		File[0] >> string;

		if( !strcmp(string,S1.c_str()) )
		while( strcmp(string,S3.c_str()))
		{
			File[0] >> string;
			if( strcmp(string,S3.c_str()) )
			Result += (AnsiString)string+" ";
		}
	}

	S2[0] = Result;
}

//---------------------------------------------------------------------------

void Utils_Tools::save_tdatetime_variable_to_file(ofstream* File,AnsiString S1,TDateTime TD,AnsiString S3,int Type_Flag)
{
	// just line with variable name and value
	if( Type_Flag == 1 )
	{
		File[0] << "\n";
		File[0] << S1.Trim().c_str() << "\t";
		File[0] << TD.Val << "\t";
		File[0] << S3.Trim().c_str();
	}

	// just value of the variable
	if( Type_Flag == 2 )
	{
		AnsiString dfff = TD.DateString();
		File[0] << dfff.c_str() << ",";
	}

	// just name of the variable (cut seven characters to get rid of "START")
	if( Type_Flag == 3 )
	{
		File[0] << (S1.SubString(1,S1.Length()-7)).Trim().c_str() << ",";
	}
}

//---------------------------------------------------------------------------

void Utils_Tools::load_tdatetime_variable_from_file(ifstream* File,AnsiString S1,TDateTime* TD,AnsiString S3)
{
	char string[2000];
	double Val;
	File[0].clear();
	File[0].seekg(0, File[0].beg);

	while( !File[0].eof() )
	{
		File[0] >> string;

		if( !strcmp(string,S1.c_str()) )
		{
			File[0] >> Val;
		}
	}
	TD[0].Val = Val;
}

//---------------------------------------------------------------------------

void Utils_Tools::save_int_variable_to_file(ofstream* File,AnsiString S1,int Val,AnsiString S3,int Type_Flag)
{
	// just line with variable name and value
	if( Type_Flag == 1 )
	{
		File[0] << "\n";
		File[0] << S1.Trim().c_str() << "\t";
		File[0] << Val << "\t";
		File[0] << S3.Trim().c_str();
	}

	// just value of the variable
	if( Type_Flag == 2 )
	{
		File[0] << Val << ",";
	}

	// just name of the variable (cut seven characters to get rid of "START")
	if( Type_Flag == 3 )
	{
		File[0] << (S1.SubString(1,S1.Length()-7)).Trim().c_str() << ",";
	}
}

//---------------------------------------------------------------------------

void Utils_Tools::load_int_variable_from_file(ifstream* File,AnsiString S1,int* Val,AnsiString S3)
{
	char string[2000];
	int Value=0;
	File[0].clear();
	File[0].seekg(0, File[0].beg);

	while( !File[0].eof() )
	{
		File[0] >> string;

		if( !strcmp(string,S1.c_str()) )
		{
			File[0] >> Value;
		}
	}

	Val[0] = Value;
}

//---------------------------------------------------------------------------
/*
bool Utils_Tools::is_this_basket_name_Flinders_Navx(AnsiString Name,
								AnsiString* Spline,AnsiString* Electrode,
								int* Spline_Id, int* Electrode_Id)
{
// syntax: (e.g. 'A60,4')

	AnsiString Trimmed_Name = Name.Trim();
	AnsiString L1 = Trimmed_Name.SubString(1,1);
	AnsiString L3 = Trimmed_Name.SubString(5,1);

	if( L1 == "A" || L1 == "B" || L1 == "C" || L1 == "D" || L1 == "E" ||
		L1 == "F" || L1 == "G" || L1 == "H" )
	if( L3 == "D" || L3 == "1" || L3 == "2" || L3 == "3" || L3 == "4" || L3 == "5" || L3 == "6" ||
		L3 == "7" || L3 == "8")
	{
	Spline[0] = L1;
	Electrode[0] = L3;

		if( Spline[0] == "A" || Spline[0] == "1") Spline_Id[0] = 0;
		if( Spline[0] == "B" || Spline[0] == "2") Spline_Id[0] = 1;
		if( Spline[0] == "C" || Spline[0] == "3") Spline_Id[0] = 2;
		if( Spline[0] == "D" || Spline[0] == "4") Spline_Id[0] = 3;
		if( Spline[0] == "E" || Spline[0] == "5") Spline_Id[0] = 4;
		if( Spline[0] == "F" || Spline[0] == "6") Spline_Id[0] = 5;
		if( Spline[0] == "G" || Spline[0] == "7") Spline_Id[0] = 6;
		if( Spline[0] == "H" || Spline[0] == "8") Spline_Id[0] = 7;

		if( Electrode[0] == "1" || Electrode[0] == "D" ) Electrode_Id[0] = 0;
		if( Electrode[0] == "2" ) Electrode_Id[0] = 1;
		if( Electrode[0] == "3" ) Electrode_Id[0] = 2;
		if( Electrode[0] == "4" ) Electrode_Id[0] = 3;
		if( Electrode[0] == "5" ) Electrode_Id[0] = 4;
		if( Electrode[0] == "6" ) Electrode_Id[0] = 5;
		if( Electrode[0] == "7" ) Electrode_Id[0] = 6;
		if( Electrode[0] == "8" ) Electrode_Id[0] = 7;

	return true;
	}

   return false;
}
*/

//---------------------------------------------------------------------------

void Utils_Tools::save_std_string_to_File(ofstream* File, std::string QS)
{
	long Size = QS.size();

	File->write((char*)&Size, sizeof (int));

	if(Size!=0)
		File->write((char*)QS.c_str(),Size);
}

//---------------------------------------------------------------------------

std::string Utils_Tools::load_std_string_from_File(ifstream* File)
{
	int Size = 0;
	std::string AS;

	File->read((char*)&Size, sizeof (int));


	if(Size != 0 )
	{
		AS.resize(Size);
		File->read((char *)&AS[0], Size);
		return AS;
	}
	return "";
}

//---------------------------------------------------------------------------

void Utils_Tools::save_String_to_File(ofstream* File, AnsiString QS)
{
	long Size = QS.Length();

	File->write((char*)&Size, sizeof (int));

	if(Size!=0)
		File->write((char*)QS.c_str(),Size);
}

//---------------------------------------------------------------------------

AnsiString Utils_Tools::load_String_from_File(ifstream* File)
{
	int Size = 0;
	AnsiString AS;

	File->read((char*)&Size, sizeof (int));
	if(Size != 0 )
	{
		AS.SetLength(Size);
		File->read((char*)AS.data(),Size);
		return AS;
	}
	return "";
}

//---------------------------------------------------------------------------

AnsiString Utils_Tools::Cells_From_StringGrid_ToText(TStringGrid& AGrid, const TGridRect& ACells)
{
  //
  // compute the total size of the
  // buffer required to hold the text
  // of each cell plus the tab and
  // CR/LF delimiters
  //
  int text_len = 0;
  for (int row = ACells.Top;row <= ACells.Bottom;++row)
  {
    for (int col = ACells.Left;col <= ACells.Right;++col)
    {
      text_len += AGrid.Cells[col][row].Length();
      if (col < ACells.Right)
        ++text_len;
    }
    if (row < ACells.Bottom)
      text_len += 2;
  }

  //
  // fill the AnsiString with the
  // text of each cell in a tab-
  // and CR/LF-delimited format
  //
  AnsiString text;
  text.SetLength(text_len);
  text = "";
  for (int row = ACells.Top;row <= ACells.Bottom;++row)
  {
    for (int col = ACells.Left;col <= ACells.Right;++col)
    {
      text += AGrid.Cells[col][row];
      if (col < ACells.Right)
	text += '\t';
    }
    if (row < ACells.Bottom)
      text += "\r\n";
  }
  return text;
}

//---------------------------------------------------------------------------

void Utils_Tools::CopyCells_From_StringGrid_ToClipboard(TStringGrid& AGrid,  const TGridRect& ACells)
{
  // grab the formatted text
  AnsiString text(Cells_From_StringGrid_ToText(AGrid, ACells));
  // copy the text to the clipboard
  Clipboard()->AsText = text;
}

//---------------------------------------------------------------------------

bool Utils_Tools::is_surface_name_excluding_from_interpolation(AnsiString Name)
{
	if(
		is_substring_present(Name,"composite") ||
		is_substring_present(Name,"superimposed") ||
		is_substring_present(Name,"overlap")
	   )
	return true;
	else
	return false;
}

//---------------------------------------------------------------------------

bool Utils_Tools::is_string_an_ECG_channel_name(AnsiString Name)
{
	AnsiString CN = Name.LowerCase().Trim();

	if( CN == "i" || CN == "ii" || CN == "iii" ||
		CN == "avf" || CN == "avr" || CN == "avl" ||
		CN == "v1" || CN == "v2" || CN == "v3" ||
		CN == "v4" || CN == "v5" || CN == "v6" )
	return true;
	else
	return false;
}

//---------------------------------------------------------------------------

const char *Utils_Tools::ExtractAfterKey(const char *pcString, const char *pcKey)
{
	// used to support NavX research cable module
	const char *pc = strstr(pcString, pcKey);
	if(pc) {
		// found the key, so move to the end of it
		pc += strlen(pcKey);
		// move to the first non-space character
		while(pc[0]==' ') pc++;
	}
	return pc;
}

//---------------------------------------------------------------------------

void Utils_Tools::get_mike_pope_bimap_names(AnsiString Filename,AnsiString *Patient_Id,
			AnsiString *S1S2,AnsiString *Stage,AnsiString *Chamber)
{
	if( is_substring_present(Filename," S1") )
	S1S2[0] = "S1";
	if( is_substring_present(Filename," S2") )
	S1S2[0] = "S2";

	Patient_Id[0] = get_string_before_given_occurence_of_specified_string(Filename," ",1);

	if( is_substring_present(Filename," prePVI") )
	Stage[0] = "prePVI";
	if( is_substring_present(Filename," postPVI") )
	Stage[0] = "postPVI";
	if( is_substring_present(Filename," postPWI") )
	Stage[0] = "postPWI";
	if( is_substring_present(Filename," postABLATION") )
	Stage[0] = "postABLATION";

	if( is_substring_present(Filename," LA") )
	Chamber[0] = "LA";
	if( is_substring_present(Filename," RA") )
	Chamber[0] = "RA";

}

//---------------------------------------------------------------------------

double Utils_Tools::get_number_from_string(std::string String)
{
	return atof(String.c_str());
}

//---------------------------------------------------------------------------

std::vector <Row_vec> Utils_Tools::load_rectangular_csv_file_to_grid(AnsiString FileName)
{
	//----------------------------------------------------------
	// Load file into Table (using comma as items saparator)
	//----------------------------------------------------------
	Row_vec Row1;
	char c;
	std::vector <Row_vec> Rows;
	AnsiString QS;

	ifstream dfile(FileName.c_str());

	if( !dfile.is_open() )
		return Rows;

	long Iterator = 0;
	while( !dfile.eof() && Iterator < 100000 )
	{

	Row1.Elements.clear();
	QS = "";

	while(c!=10 && !dfile.eof()) // till end of line
	{
		dfile.get(c);
		if( c == 44 ) // comma
		{
			Row1.Elements.push_back(QS);
			QS = "";
//			dfile.get(c);
		}

		if( c != 10 && c != 44)  QS += c; // end of line?
	} // reading line

	if( !dfile.eof() )
	{
		Row1.Elements.push_back(QS);
		Rows.push_back(Row1);
		c = 0;
	}

	Iterator++;
	} // reading file

	dfile.close();

	return Rows;
}

//---------------------------------------------------------------------------


