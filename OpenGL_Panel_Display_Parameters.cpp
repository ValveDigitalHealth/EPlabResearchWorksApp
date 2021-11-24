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

#include "OpenGL_Panel_Display_Parameters.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
OpenGL_Panel_Display_Parameters_class::OpenGL_Panel_Display_Parameters_class()
{
	  // 3D world orientation initial parameters
	  ZoomFactor   = -130.0;
	  zNear        = 0.1F;
	  zFar         = 12200.0F;
	  Translation_X = 0.0;
	  Translation_Y = 0.0;

	Seed_Cross_Size_3D = 7;

	OpenGL_Panel_Dragging_Flag = false;

	// Initial coordinates for patient doll parts
	Head_z_Coordinate = 60;
	Arms_x_Coordinate = 40;
	Legs_z_Coordinate = -50;

	Font_Type = 8;

	Data_Point_Size = 1;
	Display_Data_Points = true;
	Display_DP_Name = false;
	Autoscale_Data_Point_Size = true;
	DP_Size_Scaling_Ratio = 80;

	Electrode_Point_Size = 0.5;
	Scan_Surface_Point_Size = 0.3;

	Display_Pointing_Cross = true;

	Panel_3D_Background_Color = 1; //  white

	Text_Dist_From_Geo = 1;

	Transparency_Level = 1;

	Contour_Line_Thickness = 1;

	Display_Paths = true;
	Display_Geo_Nodes = false;
	Display_Geo_Edges = false;
    Display_3D_Points_Cloud = true;

	Display_Images_In_3D_Panel = true;
	Limit_Images_Display_Bottom = false;
	Limit_Images_Display_Top = false;
	Remove_Blue_Images_Background = false;
	Remove_Green_Images_Background = false;

	Paint_Registration_Points = true;

	Abl_Point_Size = 1;
	Display_Ablation_Points = true;

	Display_Labels = true;
	Label_Point_Size_With_Respect_To_DPSize = 3;

	Display_17_LV_Markers = true;

	Display_Arrows_Flag = false;
	Arrow_Thickness = 0.15;
	Arrow_Lenght_Scaling_Factor = 0.1;
	Arrow_Dist_From_Surface = 1;
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Display_Parameters_class::set_palette() // 0: thermal, 1: white to red
{
	  // Original_ThermalPal can be used as to restore original palette after
	  // palette transformations
	  if( Palette_Type == 0 )
	  for( int i = 0; i < 256; i++ )
	  {
			Palette[i][0] = Original_Thermal_Pal[i][0];
			Palette[i][1] = Original_Thermal_Pal[i][1];
			Palette[i][2] = Original_Thermal_Pal[i][2];
	  }

	// white to red palette
	  if( Palette_Type == 1 )
	  for( int i = 0; i < 256; i++ )
	  {
			Palette[i][0] = 255;
			Palette[i][1] = 255-i;
			Palette[i][2] = 255-i;
	  }
}

//---------------------------------------------------------------------------

int OpenGL_Panel_Display_Parameters_class::Read_Palette(AnsiString App_Directory)
{
	ifstream dfile((App_Directory+"\\Thermal_Palette.inc").c_str());

	AnsiString cc = GetCurrentDir();

	if( dfile == NULL )
		{
			ShowMessage("\n Error : can't open 'Thermal_Palette.inc' file." );
			return FILE_NOT_FOUND;
		}

	  for( int i = 0; i < 256; i++ )
	  {
			dfile >> Thermal_Pal[i][0];
			dfile >> Thermal_Pal[i][1];
			dfile >> Thermal_Pal[i][2];
	  }

	  // Original_ThermalPal can be used as to restore original palette after
	  // palette transformations
	  for( int i = 0; i < 256; i++ )
	  {
			Original_Thermal_Pal[i][0] = Thermal_Pal[i][0];
			Original_Thermal_Pal[i][1] = Thermal_Pal[i][1];
			Original_Thermal_Pal[i][2] = Thermal_Pal[i][2];
	  }

	// set thermal as default
	Palette_Type = 0;
	set_palette();

	  return OK_RESULT;
}

//---------------------------------------------------------------------------

