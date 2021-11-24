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
// #include "AAMS_constants.h"

#include<fstream>
#include<vcl.h>
#include "predefined_parameters.h"

#ifndef OpenGL_Panel_Display_ParametersH
#define OpenGL_Panel_Display_ParametersH
//---------------------------------------------------------------------------

class OpenGL_Panel_Display_Parameters_class
{
	public:

	OpenGL_Panel_Display_Parameters_class();

	//-----------------------------------------------------------
	// Variables used to set perspective on OpenGL panel
	// (gluPerspective() function called each time panel is
	// resized)
	//-----------------------------------------------------------
	double zNear, zFar;

	//-----------------------------------------------------------
	// Translation in X and Y axes.Used to implement
	// Pan transformation.
	//-----------------------------------------------------------
	int Translation_X,Translation_Y;

	//-----------------------------------------------------------
	// Flags set on when dragging over openGL panel occurs
	// Values: ON, OFF. Used in rotation, pan and zoom
	//-----------------------------------------------------------
	bool OpenGL_Panel_Dragging_Flag;

	//-----------------------------------------------------------
	// Zoom factor - distance from (0,0,0) point
	// in 3D world
	//-----------------------------------------------------------
	double ZoomFactor;

	int Font_Type;
    double Text_Dist_From_Geo;

	int Seed_Cross_Size_3D;

	//-----------------------------------------------------------
	// Head, arms and legs position of patient doll for both panels
	// Position of the begining of the head,leg,arm.
	//-----------------------------------------------------------
	double Head_z_Coordinate;
	double Arms_x_Coordinate;
	double Legs_z_Coordinate;

	double Data_Point_Size;
	bool Display_Data_Points;
    bool Display_DP_Name;
	bool Autoscale_Data_Point_Size;
	int DP_Size_Scaling_Ratio;

	double Electrode_Point_Size;
    double Scan_Surface_Point_Size;

	double Panel_3D_Background_Color; // 0 - black, 1 - white

	bool Display_Pointing_Cross;

	bool Display_Labels;
	double Label_Point_Size_With_Respect_To_DPSize;

	bool Display_Images_In_3D_Panel;
	bool Limit_Images_Display_Bottom;
	bool Limit_Images_Display_Top;
	bool Remove_Blue_Images_Background;
	bool Remove_Green_Images_Background;
	double Y_Cut;

	bool Display_17_LV_Markers;

	//-----------------------------------------------------------
	// Function: Read_Palette
	// Description: Reads palette used for color display of 3D maps
	//              Copied from CARTO system. 255 colors, RGB channels for each color
	//              Can be easily modified in case of (c) of CARTO palette
	// Input parameters: void
	// Returns: int (OK_RESULT if success, FILE_NOT_FOUND if no palette file found)
	//-----------------------------------------------------------
	int Read_Palette(AnsiString App_Directory);
	//-----------------------------------------------------------
	// Palette used to colorcode values in OpenGL panel.
	// 256 colors, 3 channels for each color (R,G,B)
	//-----------------------------------------------------------
	double Palette[256][3];
	double Thermal_Pal[256][3];    // CARTO palette, readed from file
	double Original_Thermal_Pal[256][3];
	void set_palette(); // 1: thermal, 2: white to red
	int Palette_Type;


	double Transparency_Level;

	bool Display_Normals;
	bool Display_Geo_Nodes;
	bool Display_Geo_Edges;

	int Contour_Line_Thickness;

	bool Display_Paths;

	//-----------------------------------------------------------
	// ablation points display
	//-----------------------------------------------------------
	double Abl_Point_Size;
	bool Display_Ablation_Points;

	bool Paint_Registration_Points;

	bool Display_3D_Points_Cloud;

	// arrow
	bool Display_Arrows_Flag;
	double Arrow_Thickness,Arrow_Lenght_Scaling_Factor,Arrow_Dist_From_Surface;
};

#endif

