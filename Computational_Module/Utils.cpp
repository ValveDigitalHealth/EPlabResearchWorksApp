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
int Utils_Tools::how_many_times_substring_present(AnsiString Main_String,AnsiString Given_String)
{
	int Strict_Hit_Count=0;

	for(int i=0;i<=Main_String.Length();i++)
	if( Main_String.SubString(i,Given_String.Length()) == Given_String )
	{
		Strict_Hit_Count++;
	}

	return Strict_Hit_Count;
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
	{
		ShowMessage("Unable to find or open " + FileName);
		return Rows;
	}

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


