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
#ifndef OpenglPanelH
#define OpenglPanelH

#include "Display_Options_Form.h"

#include <Vcl.ExtCtrls.hpp>

#include <glm-0.9.9.6/glm/glm.hpp>
#include <glm-0.9.9.6/glm/gtc/type_ptr.hpp>
#include <glm-0.9.9.6/glm/gtx/intersect.hpp>

#include <gl/gl.h>
#include <gl/glu.h>

#include <stdio.h>
#include <float.h>
#include <math.h>
#include <windows.h>

#include <freeglut/include/GL/freeglut.h>
#pragma comment(lib, "freeglut")

#include "openglpicking/maths_funcs_for_ray_picking.h"

#include <ArcBall.h>

#include "OpenGL_Panel_Display_Parameters.h"
#include "Segments_List.h"
#include "STUDY_Class.h"

//---------------------------------------------------------------------------

class OpenGL_Panel_Class
{
	public:

	OpenGL_Panel_Class();

	void initialize();
	void delete_contexts();

	TPanel *OpenGL_Panel;

	HDC hdc;
	HGLRC hrc;
	int PixelFormat;
	void SetPixelFormatDescriptor();

	void opengl_panel_resized();

	void map_display_panels_paint();
	void paint_3D_contents();

	void opengl_panel_apply_transformations();

	void draw_palette();
	void get_current_3D_panel_dimensions(double* Width, double* Height);

	void paint_sphere(double Radius);

	//----------------------------------------------------------------------
	// OPENGL PANEL MOUSE SUPPORT
	//----------------------------------------------------------------------
	OpenGL_Panel_Display_Parameters_class OpenGL_Panel_Display_Parameters;

	bool Resizing_Dragging_Mode;
	bool Segmentation_Painting_Happened_Flag;
	int Mouse_Position_X,Mouse_Position_Y;

	//-----------------------------------------------------------
	// segmentation support
	//-----------------------------------------------------------
	Segments_List_Class* Segments_Info;

	//-----------------------------------------------------------
	// Below variables used to implement ArcBall rotation
	//-----------------------------------------------------------
	ArcBallT    ArcBall; // ArcBall object
	Matrix3fT   LastRot; // Last rotation
	Matrix3fT   ThisRot; // Current rotation
	Matrix4fT   Transform; // Resulted transform matrix
	Point2fT    MousePt; // Mouse position

	void rotate_opengl_panels_world();

	//----------------------------------------------------------------------
	// MOUSE SUPPORT
	//----------------------------------------------------------------------
	int MouseStatus;
	TMouseButton MouseButton;
	int Mouse_X,Mouse_Y;

	//-------------------------------------------------------------------------
	// Function finds what element is currently pointed in Main_3D_Panel window
	//-------------------------------------------------------------------------

	// Ray casting support. Based on:
	// http://antongerdelan.net/opengl/raycasting.html
	// https://github.com/capnramses/antons_opengl_tutorials_book/blob/master/07_ray_picking/main.cpp
	vec3 Camera_Position;
	vec3 World_Ray;
	mat4 View_matrix_for_ray_casting;
	mat4 Proj_matrix_for_ray_casting;
	vec3 get_ray_from_mouse( float mouse_x, float mouse_y );

	bool ray_sphere_intersection_test(vec3 ray_origin_wor, vec3 ray_direction_wor, vec3 sphere_centre_wor,
			float sphere_radius, float* intersection_distance );

	bool ray_triangle_intersection_test(
			glm::vec3 ray_origin_wor, glm::vec3 ray_direction_wor,
			glm::vec3 v0,glm::vec3 v1,glm::vec3 v2,
			float* intersection_distance,glm::vec2* uv );

	void MouseFeedBack(int X, int Y,bool Segmenting);
	long closest_dp_clicked,closest_Surface_clicked_dp;
	long closest_Surface_clicked_label,closest_label_clicked;
	long closest_triangle_clicked,closest_Surface_clicked_tt;

	void set_Rhythmia_AP_view();
	void set_Rhythmia_PA_view();

	void set_NavX_AP_view();
	void set_NavX_PA_view();

	void set_CARTO_AP_view();
	void set_CARTO_PA_view();

	//-------------------------------------------------------------
	//-------------------------------------------------------------
	STUDY_Class* STUDY; // POINTER TO MAIN DATA STRUCTURE
	//-------------------------------------------------------------
	//-------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Function paints Data points of the given geometry and dp set
	//-------------------------------------------------------------------------
	void paint_data_points(int Surface_Ptr,int Data_Point_Set_Ptr);
	void autoscale_data_point_size();

	//------------------------------------
	// surface display
	//------------------------------------
	void prepare_colors_for_display(); // has to be called after every action modyfing displayed values
	void paint_surface(int Surface_Ptr,int DP_Set,bool For_Mouse_Feedback);
	void paint_geometry_nodes(int Surface_Ptr);

	//------------------------------------
	// Ablation points and labels display
	//------------------------------------
	void paint_ablation_points(int Surface_Ptr);
	void paint_labels(int Surface_Ptr);

	void paint_17_LV_segmentation_markers(int Surface_Ptr);

	void draw_fluoro_images_3D(int Surface_Ptr);

	void renderBitmapString(float x,float y,float z,void *font,char *string);

	void set_initial_zoom_in_3D_panel();

	void draw_Arrow(double x1,double y1,double z1,double x2,double y2,double z2,double D);

};

#endif
