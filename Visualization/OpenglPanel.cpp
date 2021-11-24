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
#include "OpenglPanel.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

OpenGL_Panel_Class::OpenGL_Panel_Class()
{
	MouseStatus = MB_UP;
	Camera_Position = vec3( 0.0f, 0.0f, 0.0f );
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::initialize()
{
	//----------------------------------------------
	// initialize opengl panel
	//----------------------------------------------
	hdc = GetDC(this->OpenGL_Panel->Handle);
	SetPixelFormatDescriptor();
	hrc = wglCreateContext(hdc);

	//---------------------------------
	bool Result = wglMakeCurrent(hdc, hrc); // inclusion of this causes erorr....
	//---------------------------------

	DWORD error = GetLastError();

	// Light sources parameters
	GLfloat  ambientLight[] = {  0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat  diffuseLight[] = {  0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat  specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f};
	GLfloat  specref[] =  {      0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPos0[] = {   2000,  2000,  2000, 1.0};
	GLfloat lightPos1[] = {   -2000,  -2000,  -2000, 1.0};

	// enable light
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);

	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT1,GL_SPECULAR,specularLight);
	glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);

	// surface material properties
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	glMateriali(GL_FRONT,GL_SHININESS,20.0);
	glMaterialfv(GL_BACK, GL_SPECULAR,specref);
	glMateriali(GL_BACK,GL_SHININESS,20.0);

	// VARIOUS SETTINGS
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_AUTO_NORMAL);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glDisable (GL_CULL_FACE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glFrontFace(GL_CCW);

	// to enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// init glut
	int foo = 1;
	char * bar[1] = {" "};
	glutInit(&foo, bar);

	GLuint VertexArrayID;
//	glGenVertexArrays(1, &VertexArrayID);
//	glBindVertexArray(VertexArrayID);

// */

}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::delete_contexts()
{
  //	ReleaseDC(hdc);
	wglDeleteContext(hrc);
}
//---------------------------------------------------------------------------

void OpenGL_Panel_Class::SetPixelFormatDescriptor()
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,0,
		0,0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
		};

	PixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, PixelFormat, &pfd);
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::set_Rhythmia_AP_view()
{

	ThisRot.M[0]	=	-0.995245;
	ThisRot.M[1]	=	0.0624967;
	ThisRot.M[2]	=	-0.0747189;
	ThisRot.M[3]	=	0.0717065;
	ThisRot.M[4]	=	0.989215;
	ThisRot.M[5]	=	-0.12772;
	ThisRot.M[6]	=	0.0659311;
	ThisRot.M[7]	=	-0.13247;
	ThisRot.M[8]	=	-0.988993;
	LastRot.M[0]	=	-0.996682;
	LastRot.M[1]	=	0.0809881;
	LastRot.M[2]	=	0.00817254;
	LastRot.M[3]	=	0.0813403;
	LastRot.M[4]	=	0.994745;
	LastRot.M[5]	=	0.0621784;
	LastRot.M[6]	=	-0.00309376;
	LastRot.M[7]	=	0.0626372;
	LastRot.M[8]	=	-0.998033;
	Transform.M[0]	=	-0.995305;
	Transform.M[1]	=	0.0625005;
	Transform.M[2]	=	-0.0747234;
	Transform.M[3]	=	0;
	Transform.M[4]	=	0.0717108;
	Transform.M[5]	=	0.989275;
	Transform.M[6]	=	-0.127727;
	Transform.M[7]	=	0;
	Transform.M[8]	=	0.0659351;
	Transform.M[9]	=	-0.132478;
	Transform.M[10]	=	-0.989053;
	Transform.M[11]	=	0;
	Transform.M[12]	=	0;
	Transform.M[13]	=	0;
	Transform.M[14]	=	0;
	Transform.M[15]	=	1;
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::set_Rhythmia_PA_view()
{

	ThisRot.M[0]	=	0.968739;
	ThisRot.M[1]	=	0.219304;
	ThisRot.M[2]	=	-0.115982;
	ThisRot.M[3]	=	-0.194507;
	ThisRot.M[4]	=	0.961602;
	ThisRot.M[5]	=	0.193621;
	ThisRot.M[6]	=	0.15399;
	ThisRot.M[7]	=	-0.165009;
	ThisRot.M[8]	=	0.974198;
	LastRot.M[0]	=	0.976758;
	LastRot.M[1]	=	0.18704;
	LastRot.M[2]	=	-0.104695;
	LastRot.M[3]	=	-0.156678;
	LastRot.M[4]	=	0.956327;
	LastRot.M[5]	=	0.24676;
	LastRot.M[6]	=	0.146277;
	LastRot.M[7]	=	-0.224621;
	LastRot.M[8]	=	0.963406;
	Transform.M[0]	=	0.968815;
	Transform.M[1]	=	0.219321;
	Transform.M[2]	=	-0.115991;
	Transform.M[3]	=	0;
	Transform.M[4]	=	-0.194522;
	Transform.M[5]	=	0.961678;
	Transform.M[6]	=	0.193636;
	Transform.M[7]	=	0;
	Transform.M[8]	=	0.154002;
	Transform.M[9]	=	-0.165022;
	Transform.M[10]	=	0.974274;
	Transform.M[11]	=	0;
	Transform.M[12]	=	0;
	Transform.M[13]	=	0;
	Transform.M[14]	=	0;
	Transform.M[15]	=	1;
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::set_NavX_AP_view()
{
ThisRot.M[0] = 0.999948;
ThisRot.M[1] = 0.00398936;
ThisRot.M[2] = -0.00943229;
ThisRot.M[3] = -0.00940014;
ThisRot.M[4] = -0.00798492;
ThisRot.M[5] = -0.999924;
ThisRot.M[6] = -0.00406445;
ThisRot.M[7] = 0.999961;
ThisRot.M[8] = -0.00794745;

LastRot.M[0] = 0.999368;
LastRot.M[1] = -0.0323501;
LastRot.M[2] = 0.0147354;
LastRot.M[3] = 0.0151443;
LastRot.M[4] = 0.0124302;
LastRot.M[5] = -0.999808;
LastRot.M[6] = 0.0321607;
LastRot.M[7] = 0.9994;
LastRot.M[8] = 0.0129118;

Transform.M[0] = 0.999975;
Transform.M[1] = 0.00398947;
Transform.M[2] = -0.00943255;
Transform.M[3] = 0;
Transform.M[4] = -0.0094004;
Transform.M[5] = -0.00798514;
Transform.M[6] = -0.999951;
Transform.M[7] = 0;
Transform.M[8] = -0.00406456;
Transform.M[9] = 0.999988;
Transform.M[10] = -0.00794767;
Transform.M[11] = 0;
Transform.M[12] = 0;
Transform.M[13] = 0;
Transform.M[14] = 0;
Transform.M[15] = 1;
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::set_NavX_PA_view()
{
ThisRot.M[0] = -0.999966;
ThisRot.M[1] = 0.000840668;
ThisRot.M[2] = 0.00824017;
ThisRot.M[3] = 0.00821256;
ThisRot.M[4] = -0.0287223;
ThisRot.M[5] = 0.999553;
ThisRot.M[6] = 0.00107703;
ThisRot.M[7] = 0.999588;
ThisRot.M[8] = 0.0287149;

LastRot.M[0] = -0.999882;
LastRot.M[1] = 0.0152847;
LastRot.M[2] = -0.001657;
LastRot.M[3] = -0.00209034;
LastRot.M[4] = -0.0283749;
LastRot.M[5] = 0.999595;
LastRot.M[6] = 0.0152316;
LastRot.M[7] = 0.999481;
LastRot.M[8] = 0.028404;

Transform.M[0] = -0.999971;
Transform.M[1] = 0.000840672;
Transform.M[2] = 0.00824021;
Transform.M[3] = 0;
Transform.M[4] = 0.0082126;
Transform.M[5] = -0.0287225;
Transform.M[6] = 0.999558;
Transform.M[7] = 0;
Transform.M[8] = 0.00107704;
Transform.M[9] = 0.999592;
Transform.M[10] = 0.028715;
Transform.M[11] = 0;
Transform.M[12] = 0;
Transform.M[13] = 0;
Transform.M[14] = 0;
Transform.M[15] = 1;
}

//---------------------------------------------------------------------------
void OpenGL_Panel_Class::set_CARTO_AP_view()
{
	ThisRot.M[0]	=	0.98528;
	ThisRot.M[1]	=	-0.162059;
	ThisRot.M[2]	=	0.0544185;
	ThisRot.M[3]	=	0.152528;
	ThisRot.M[4]	=	0.977117;
	ThisRot.M[5]	=	0.148251;
	ThisRot.M[6]	=	-0.0771987;
	ThisRot.M[7]	=	-0.137768;
	ThisRot.M[8]	=	0.987452;
	LastRot.M[0]	=	0.968588;
	LastRot.M[1]	=	-0.156441;
	LastRot.M[2]	=	-0.193297;
	LastRot.M[3]	=	0.161054;
	LastRot.M[4]	=	0.986911;
	LastRot.M[5]	=	0.00828835;
	LastRot.M[6]	=	0.189471;
	LastRot.M[7]	=	-0.039159;
	LastRot.M[8]	=	0.981106;
	Transform.M[0]	=	0.985334;
	Transform.M[1]	=	-0.162068;
	Transform.M[2]	=	0.0544215;
	Transform.M[3]	=	0;
	Transform.M[4]	=	0.152537;
	Transform.M[5]	=	0.97717;
	Transform.M[6]	=	0.148259;
	Transform.M[7]	=	0;
	Transform.M[8]	=	-0.0772029;
	Transform.M[9]	=	-0.137775;
	Transform.M[10]	=	0.987506;
	Transform.M[11]	=	0;
	Transform.M[12]	=	0;
	Transform.M[13]	=	0;
	Transform.M[14]	=	0;
	Transform.M[15]	=	1;
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::set_CARTO_PA_view()
{
	ThisRot.M[0]	=	-0.942755;
	ThisRot.M[1]	=	-0.216154;
	ThisRot.M[2]	=	0.25395;
	ThisRot.M[3]	=	-0.249204;
	ThisRot.M[4]	=	0.96266;
	ThisRot.M[5]	=	-0.10575;
	ThisRot.M[6]	=	-0.221609;
	ThisRot.M[7]	=	-0.162981;
	ThisRot.M[8]	=	-0.96142;
	LastRot.M[0]	=	-0.953097;
	LastRot.M[1]	=	-0.204373;
	LastRot.M[2]	=	0.223245;
	LastRot.M[3]	=	-0.240988;
	LastRot.M[4]	=	0.958676;
	LastRot.M[5]	=	-0.15121;
	LastRot.M[6]	=	-0.183117;
	LastRot.M[7]	=	-0.197917;
	LastRot.M[8]	=	-0.962964;
	Transform.M[0]	=	-0.942832;
	Transform.M[1]	=	-0.216172;
	Transform.M[2]	=	0.25397;
	Transform.M[3]	=	0;
	Transform.M[4]	=	-0.249224;
	Transform.M[5]	=	0.962739;
	Transform.M[6]	=	-0.105758;
	Transform.M[7]	=	0;
	Transform.M[8]	=	-0.221627;
	Transform.M[9]	=	-0.162994;
	Transform.M[10]	=	-0.961498;
	Transform.M[11]	=	0;
	Transform.M[12]	=	0;
	Transform.M[13]	=	0;
	Transform.M[14]	=	0;
	Transform.M[15]	=	1;
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::map_display_panels_paint()
{

//	_clear87();
//	_control87(MCW_EM, MCW_EM);
/*
		CONTEXT myContext;
		RtlCaptureContext(&myContext);

		ExceptionInfo->ContextRecord->FltSave.ControlWord = ctrlwrd;
		ExceptionInfo->ContextRecord->FltSave.StatusWord = 0;
		ExceptionInfo->ContextRecord->FltSave.MxCsr = myContext.FltSave.MxCsr;
		ExceptionInfo->ContextRecord->FltSave.MxCsr_Mask = myContext.FltSave.MxCsr_Mask;
		ExceptionInfo->ContextRecord->MxCsr = myContext.MxCsr;
*/
	// clear panel
	if( OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color == 0 )
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if( OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color == 1 )
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

//	glClear(GL_COLOR_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw palette
	draw_palette();

	// Rotate and translate
	opengl_panel_apply_transformations();

	// Draw patient doll
//	draw_patient_doll();

	// call paint function for current panel
	paint_3D_contents();

	glFlush();

	SwapBuffers(hdc);
}
//---------------------------------------------------------------------------

	//-----------------------------------------------------------
	// Function controlling repainting of the OpenGL panel. Called by
	// paint event of OpenGL panel
	// Calls paint_map() function which actually paints something.
	//-----------------------------------------------------------

void OpenGL_Panel_Class::paint_3D_contents()
{
	double Sphere_Size;
	double x,y,z,color;

	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	// Paint surfaces // gogh
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	for(long S=0;S<STUDY->Surfaces_List.size();S++)
	if( STUDY->Surfaces_List[S].Display_Whole_Dataset_Flag )
	{
		paint_geometry_nodes(S);

		paint_surface(S,STUDY->Surfaces_List[S].Current_Data_Point_Set_Ptr,false);

		paint_data_points(S,STUDY->Surfaces_List[S].Current_Data_Point_Set_Ptr);

		paint_ablation_points(S);

		paint_labels(S);

		paint_17_LV_segmentation_markers(S);

		draw_fluoro_images_3D(S);

	} // going through all surfaces

}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::paint_geometry_nodes(int Surface_Ptr)
{
	double x,y,z;

	if( OpenGL_Panel_Display_Parameters.Display_Geo_Nodes )
	{

	for(long i=0; i <(signed) STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set.size(); i++)
	if( !STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[i].Removed_Geometry_Vertex_Flag )
	{
		x = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[i].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[i].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[i].z;

		glColor4f(0.7,0.7,0.7,OpenGL_Panel_Display_Parameters.Transparency_Level);

		glPushMatrix();
		glTranslatef(x, y, z);
		paint_sphere( 0.3*OpenGL_Panel_Display_Parameters.Data_Point_Size);
		glPopMatrix();
	}

	} // for if
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::paint_ablation_points(int Surface_Ptr)
{
	double x,y,z;

	if( OpenGL_Panel_Display_Parameters.Display_Ablation_Points )
	for(long i=0;i<STUDY->Surfaces_List[Surface_Ptr].Ablation_Points_List.size();i++)
	if( STUDY->Surfaces_List[Surface_Ptr].Ablation_Points_List[i].Deleted != ON )
	{
		x = STUDY->Surfaces_List[Surface_Ptr].Ablation_Points_List[i].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Ablation_Points_List[i].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Ablation_Points_List[i].z;

		glColor4f(0.9,0.8,0.8,OpenGL_Panel_Display_Parameters.Transparency_Level);

		glPushMatrix();
		glTranslatef(x, y, z);
		paint_sphere(1.4*OpenGL_Panel_Display_Parameters.Data_Point_Size);
		glPopMatrix();
	}
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::paint_labels(int Surface_Ptr)
{
	double x,y,z;
	double nx,ny,nz;
	double DistsN = OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo; // distance between geometry and text

	if( OpenGL_Panel_Display_Parameters.Display_Labels )
	for(long i=0;i<STUDY->Surfaces_List[Surface_Ptr].Labels.size();i++)
	{
		x = STUDY->Surfaces_List[Surface_Ptr].Labels[i].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Labels[i].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Labels[i].z;

		nx = STUDY->Surfaces_List[Surface_Ptr].Labels[i].nx;
		ny = STUDY->Surfaces_List[Surface_Ptr].Labels[i].ny;
		nz = STUDY->Surfaces_List[Surface_Ptr].Labels[i].nz;

		glColor4f(0.5,0.5,0.5,OpenGL_Panel_Display_Parameters.Transparency_Level);

		glPushMatrix();
		glTranslatef(x, y, z);
		paint_sphere( OpenGL_Panel_Display_Parameters.Label_Point_Size_With_Respect_To_DPSize*
					  OpenGL_Panel_Display_Parameters.Data_Point_Size);
		glPopMatrix();

		// name
		glColor4f( 0.0f,0.0f,0.0f,OpenGL_Panel_Display_Parameters.Transparency_Level);
		renderBitmapString(x+DistsN*nx,y+DistsN*ny,z+DistsN*nz,
			(void*)OpenGL_Panel_Display_Parameters.Font_Type,
				STUDY->Surfaces_List[Surface_Ptr].Labels[i].Name.c_str() );
	}
}


//---------------------------------------------------------------------------

void OpenGL_Panel_Class::get_current_3D_panel_dimensions(double* Width, double* Height)
{
		Width[0] = OpenGL_Panel->Width;
		Height[0] = OpenGL_Panel->Height;
}

//--------------------------------------------------------------------------------------------

	//-----------------------------------------------------------
	// Function: draw_palette
	// Description: Draws palette on pointed panel
	// Input parameters: which panel (FIRST_MAP_PANEL or SECOND_MAP_PANEL)
	// Returns: void
	//-----------------------------------------------------------

void OpenGL_Panel_Class::draw_palette()
{
	double PW,PH;
	get_current_3D_panel_dimensions(&PW,&PH);

	double P_Width = 20;   // width of palette in pixels
	double P_Height = PH/3.; // height of palette in pixels
	double P_Dens = 80;   // how many rectangles are building palette image
	double Left = 10; // distance from left edge
	double Top = P_Height;
	double x,y,z,ptr;

	AnsiString Label, Unit;

	double Transparency_Level = 0.9;

	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, PW, 0, PH);

	// Turn off lighting (to avoid effects from the light source position)
	glDisable(GL_LIGHTING);

	glBegin(GL_QUADS);

	bool Reversed_Palette = false;
	if( STUDY->is_current_surface_in_range() )
	Reversed_Palette = STUDY->Surfaces_List[STUDY->Current_Surface].
		Map_Values.get_current_value_inverted_flag();

	// painting
	for(long i=0;i<P_Dens;i++)
	{
		x = Left;
		y = Top + i*P_Height/P_Dens-1;
		ptr= i*255.0/P_Dens;
		if (Reversed_Palette) ptr = 255-ptr;
		glColor4f( 1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][2]/255.0,OpenGL_Panel_Display_Parameters.Transparency_Level);
		glVertex3f( x, PH-y, 0 );

		x = Left+P_Width;
		y = Top + i*P_Height/P_Dens-1;
		glColor4f( 1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][2]/255.0,OpenGL_Panel_Display_Parameters.Transparency_Level);
		glVertex3f( x, PH-y, 0 );

		x = Left+P_Width;
		y = Top + (i+1)*P_Height/P_Dens+1;
		ptr= (i+1)*255.0/P_Dens;
		if (Reversed_Palette) ptr = 255-ptr;
		glColor4f( 1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][2]/255.0,OpenGL_Panel_Display_Parameters.Transparency_Level);
		glVertex3f( x, PH-y, 0 );

		x = Left;
		y = Top + (i+1)*P_Height/P_Dens+1;
		glColor4f( 1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][0]/255.0,
				   1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][1]/255.0,
				   1.0f*OpenGL_Panel_Display_Parameters.Palette[(int)ptr][2]/255.0,OpenGL_Panel_Display_Parameters.Transparency_Level);
		glVertex3f( x, PH-y, 0 );
	}

	glEnd();

	glColor4f( 0.0f,0.0f,0.0f,0.9f);

	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	Unit = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_unit();

	double Min_Value_On_Maps,Max_Value_On_Maps;
	Min_Value_On_Maps = STUDY->Min_Value_On_All_Maps;
	Max_Value_On_Maps = STUDY->Max_Value_On_All_Maps;

	Label = FloatToStrF(Min_Value_On_Maps,ffGeneral,3,2 );
	Label += " "+Unit;

	renderBitmapString(
			Left+P_Width+5,
			Top,
			0,
			(void*)OpenGL_Panel_Display_Parameters.Font_Type,
			Label.c_str());

	Label = FloatToStrF(Max_Value_On_Maps,ffGeneral,3,2 );
	Label += " "+Unit;

	renderBitmapString(
			Left+P_Width+5,
			Top+P_Height,
			0,
			(void*)OpenGL_Panel_Display_Parameters.Font_Type,
			Label.c_str());

	// write average value of current map
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

	double av = STUDY->Surfaces_List[STUDY->Current_Surface].Mean_Map_Value;
	double SD = STUDY->Surfaces_List[STUDY->Current_Surface].SD_of_Map_Value;
	double vol = STUDY->Surfaces_List[STUDY->Current_Surface].Volume;
	double area = STUDY->Surfaces_List[STUDY->Current_Surface].Area;
	long DPValid_No = STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[dset].get_valid_data_points_number(Current_Value_Name,
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref() );

	Label = "Mean= "+FloatToStrF(av,ffGeneral,3,2 )+" SD="+FloatToStrF(SD,ffGeneral,3,2 );
	Label += " Data points#= " + IntToStr( (int)STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[dset].Data_Points.size()) + " (valid: " + IntToStr((int)DPValid_No) + ")";
	Label += " Vol= "+ FloatToStrF(vol,ffGeneral,3,2 ) + "[cm3]";
//	Label += " Area[cm2]= "+ FloatToStrF(area,ffGeneral,3,2 );

	renderBitmapString(10,10,0,
			(void*)OpenGL_Panel_Display_Parameters.Font_Type,Label.c_str());
	}

	}

	glEnable(GL_LIGHTING);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);		// Select the modelview matrix
	glLoadIdentity();
}
//---------------------------------------------------------------------------

void OpenGL_Panel_Class::opengl_panel_resized()
{
	double PW,PH;
	double fovy = 45;

	get_current_3D_panel_dimensions(&PW,&PH);

	if( PW > 0 && PH > 0 )
	{
	glViewport(0,0,(GLsizei)PW,(GLsizei)PH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(PH!=0)
	gluPerspective(fovy, (GLdouble)PW/(GLdouble)PH,
			OpenGL_Panel_Display_Parameters.zNear,
			OpenGL_Panel_Display_Parameters.zFar);

  // calculate proj_mat variable for ray casting
  // input variables
	float aspect = (float)PW / (float)PH; // aspect ratio
	Proj_matrix_for_ray_casting = perspective( fovy, aspect,
		OpenGL_Panel_Display_Parameters.zNear,
		OpenGL_Panel_Display_Parameters.zFar );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	ArcBall.setBounds((GLfloat)PW,(GLfloat)PH);
	}
}

//---------------------------------------------------------------------------

	//-----------------------------------------------------------
	// Function called before painting openGL panel. Makes all
	// transformations (rotation,zoom) to camera position in 3D
	// world
	//-----------------------------------------------------------

void OpenGL_Panel_Class::opengl_panel_apply_transformations()
{
	// Load identity matrix - reset all transformations
	glLoadIdentity();

	// Translate (Pan and Zoom efects)
	glTranslatef(OpenGL_Panel_Display_Parameters.Translation_X,
				 OpenGL_Panel_Display_Parameters.Translation_Y,
				 OpenGL_Panel_Display_Parameters.ZoomFactor);

	// calculate camera position
	glm::vec4 CAMPOS( -OpenGL_Panel_Display_Parameters.Translation_X,
					  -OpenGL_Panel_Display_Parameters.Translation_Y,
					  -OpenGL_Panel_Display_Parameters.ZoomFactor, 1); // 1 to keep translation

	glm::mat4 matrix = glm::make_mat4(Transform.M);
	glm::mat4 matrixT = glm::transpose(matrix);
	glm::vec3 CC = glm::vec3(matrixT * CAMPOS);
	Camera_Position.v[0] = CC[0];
	Camera_Position.v[1] = CC[1];
	Camera_Position.v[2] = CC[2];

	// Rotate world
	glMultMatrixf(Transform.M);
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::rotate_opengl_panels_world()
{
	Quat4fT ThisQuat;

	ArcBall.drag(&MousePt, &ThisQuat);
	Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);
	Matrix3fMulMatrix3f(&ThisRot, &LastRot);
	Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
}

//---------------------------------------------------------------------------

/* takes mouse position on screen and return ray in world coords */
vec3 OpenGL_Panel_Class::get_ray_from_mouse( float mouse_x, float mouse_y )
{
  // screen space (viewport coordinates)
  float x = ( 2.0f * mouse_x ) / OpenGL_Panel->Width - 1.0f;
  float y = 1.0f - ( 2.0f * mouse_y ) / OpenGL_Panel->Height;
  float z = 1.0f;

  // normalised device space
  vec3 ray_nds = vec3( x, y, z );

  // clip space
  vec4 ray_clip = vec4( ray_nds.v[0], ray_nds.v[1], -1.0, 1.0 );

  // eye space
  vec4 ray_eye = inverse( Proj_matrix_for_ray_casting ) * ray_clip;
  ray_eye      = vec4( ray_eye.v[0], ray_eye.v[1], -1.0, 0.0 );

  glGetFloatv(GL_MODELVIEW_MATRIX, View_matrix_for_ray_casting.m);

  double p0 = View_matrix_for_ray_casting.m[0];
  double p1 = View_matrix_for_ray_casting.m[1];
  double p2 = View_matrix_for_ray_casting.m[2];
  double p3 = View_matrix_for_ray_casting.m[3];
  double p4 = View_matrix_for_ray_casting.m[4];
  double p5 = View_matrix_for_ray_casting.m[5];
  double p6 = View_matrix_for_ray_casting.m[6];

  //View_matrix_for_ray_casting.
  vec3 ray_wor = vec3( inverse( View_matrix_for_ray_casting ) * ray_eye );

  // don't forget to normalise the vector at some point
  ray_wor = normalise( ray_wor );

  return ray_wor;
}

//---------------------------------------------------------------------------
bool OpenGL_Panel_Class::ray_triangle_intersection_test(
		glm::vec3 ray_origin_wor, glm::vec3 ray_direction_wor,
		glm::vec3 v0,glm::vec3 v1,glm::vec3 v2, float* intersection_distance,glm::vec2*uv)
{
	glm::vec3 e1 = v1-v0;
	glm::vec3 e2 = v2-v0;
	glm::vec3 pvec = glm::cross(ray_direction_wor, e2);
	glm::vec3 tvec = ray_origin_wor -  v0;
	glm::vec3 qvec;
	uv[0][0] = glm::dot(tvec, pvec);

	float det = glm::dot(e1, pvec);
	if (det > 0) {
		if (uv[0][0] < 0 || uv[0][0] > det)
			return false;
		qvec = glm::cross(tvec, e1);
		uv[0][1] = glm::dot(ray_direction_wor, qvec);
		if (uv[0][1] < 0 || uv[0][0] + uv[0][1] > det)
			return false;
	} else if (det < 0) {
		if (uv[0][0] > 0 || uv[0][0] < det)
			return false;
		qvec = glm::cross(tvec, e1);
		uv[0][1] = glm::dot(ray_direction_wor, qvec);
		if (uv[0][1] > 0 || uv[0][0] + uv[0][1] < det)
			return false;
	} else {
		return false;
	}
	float inv_det = 1 / det;
	*intersection_distance = glm::dot(e2, qvec) * inv_det;
	uv[0][0] *= inv_det;
	uv[0][1] *= inv_det;
	return *intersection_distance >= OpenGL_Panel_Display_Parameters.zNear &&
		*intersection_distance <= OpenGL_Panel_Display_Parameters.zFar;
}
//-----------------------------------------------------------------------

bool OpenGL_Panel_Class::ray_sphere_intersection_test( vec3 ray_origin_wor, vec3 ray_direction_wor,
	vec3 sphere_centre_wor, float sphere_radius, float* intersection_distance )
{
  // work out components of quadratic
  vec3 dist_to_sphere     = ray_origin_wor - sphere_centre_wor;
  float b                 = dot( ray_direction_wor, dist_to_sphere );
  float c                 = dot( dist_to_sphere, dist_to_sphere ) - sphere_radius * sphere_radius;
  float b_squared_minus_c = b * b - c;
  // check for "imaginary" answer. == ray completely misses sphere
  if ( b_squared_minus_c < 0.0f ) { return false; }
  // check for ray hitting twice (in and out of the sphere)
  if ( b_squared_minus_c > 0.0f ) {
	// get the 2 intersection distances along ray
	float t_a              = -b + sqrt( b_squared_minus_c );
	float t_b              = -b - sqrt( b_squared_minus_c );
	*intersection_distance = t_b;
	// if behind viewer, throw one or both away
	if ( t_a < 0.0 ) {
	  if ( t_b < 0.0 ) { return false; }
	} else if ( t_b < 0.0 ) {
	  *intersection_distance = t_a;
	}

	return true;
  }
  // check for ray hitting once (skimming the surface)
  if ( 0.0f == b_squared_minus_c ) {
	// if behind viewer, throw away
	float t = -b + sqrt( b_squared_minus_c );
	if ( t < 0.0f ) { return false; }
	*intersection_distance = t;
	return true;
  }
  // note: could also check if ray origin is inside sphere radius
  return false;
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::MouseFeedBack(int X, int Y,bool Segmenting)
{
	vec3 dpxyz;
	float closest_intersection,t_dist;

	closest_dp_clicked=-1;
	closest_Surface_clicked_dp=-1;
	closest_Surface_clicked_label=-1;
	closest_label_clicked=-1;
	closest_triangle_clicked=-1;
	closest_Surface_clicked_tt=-1;

	// work out ray
	World_Ray = get_ray_from_mouse( (float)X, (float)Y );

	// -------------------------------------------------------------------
	// check ray against all data points
	// -------------------------------------------------------------------
	if( !Segmenting )
	{

	closest_dp_clicked = -1;
	closest_Surface_clicked_dp = -1;
	closest_intersection = 0.0f;
	t_dist = 0.0f;

	if( OpenGL_Panel_Display_Parameters.Display_Data_Points )
	for(long S=0;S<STUDY->Surfaces_List.size();S++)
	if( STUDY->Surfaces_List[S].Display_Whole_Dataset_Flag )
	if( STUDY->Surfaces_List[S].Display_Geometry_Flag )
	{
		int dset = STUDY->Surfaces_List[S].Current_Data_Point_Set_Ptr;

		if( STUDY->Surfaces_List[S].data_points_set_ptr_in_range() )

		for(long dp=0;dp<STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points.size();dp++)
		{

		dpxyz = vec3( STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[dp].x,
					  STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[dp].y,
					  STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[dp].z );

		if ( ray_sphere_intersection_test( Camera_Position, World_Ray, dpxyz,
				OpenGL_Panel_Display_Parameters.Data_Point_Size, &t_dist ) )
		{
			// if more than one sphere is in path of ray, only use the closest one
			if ( -1 == closest_dp_clicked || t_dist < closest_intersection )
			{
			  closest_dp_clicked = dp;
			  closest_intersection = t_dist;
			  closest_Surface_clicked_dp = S;
			}
		}

		} // through all dps
	} // thru all surfaces
	} // if not segmenting mode

	// -------------------------------------------------------------------
	// check ray against all Labels
	// -------------------------------------------------------------------
	if( !Segmenting )
	{

	closest_label_clicked = -1;
	closest_Surface_clicked_label = -1;
	closest_intersection = 0.0f;
	t_dist = 0.0f;
	double LabelSphereSize = OpenGL_Panel_Display_Parameters.Label_Point_Size_With_Respect_To_DPSize*
					  OpenGL_Panel_Display_Parameters.Data_Point_Size;

	if( OpenGL_Panel_Display_Parameters.Display_Labels )
	for(long S=0;S<STUDY->Surfaces_List.size();S++)
	if( STUDY->Surfaces_List[S].Display_Whole_Dataset_Flag )
	if( STUDY->Surfaces_List[S].Display_Geometry_Flag )
	{
		for(long dp=0;dp<STUDY->Surfaces_List[S].Labels.size();dp++)
		{

		dpxyz = vec3( STUDY->Surfaces_List[S].Labels[dp].x,
					  STUDY->Surfaces_List[S].Labels[dp].y,
					  STUDY->Surfaces_List[S].Labels[dp].z );

		if ( ray_sphere_intersection_test( Camera_Position, World_Ray, dpxyz,
											LabelSphereSize, &t_dist ) )
		{
			// if more than one sphere is in path of ray, only use the closest one
			if ( -1 == closest_label_clicked || t_dist < closest_intersection )
			{
			  closest_label_clicked = dp;
			  closest_intersection = t_dist;
			  closest_Surface_clicked_label = S;
			}
		}

		} // through all labels
	} // thru all surfaces
	} // if not segmenting mode

	// -------------------------------------------------------------------
	// check ray against all triangles of geometries
	// -------------------------------------------------------------------
	closest_triangle_clicked = -1;
	closest_Surface_clicked_tt = -1;
	closest_intersection = 0.0f;
	t_dist = 0.0f;
	long n0,n1,n2;
	glm::vec3 vt1,vt2,vt3;
	glm::vec2 baryPosition;
	glm::vec3 glmCamera_Position = glm::vec3(
		Camera_Position.v[0], Camera_Position.v[1], Camera_Position.v[2]);
	glm::vec3 glmWorld_Ray = glm::vec3(
		World_Ray.v[0], World_Ray.v[1], World_Ray.v[2]);

	for(long S=0;S<STUDY->Surfaces_List.size();S++)
	if( STUDY->Surfaces_List[S].Display_Whole_Dataset_Flag )
	if( STUDY->Surfaces_List[S].Display_Geometry_Flag )
	{
		for(long tt=0;tt<STUDY->Surfaces_List[S].Surface_Triangle_Set.size();tt++)
		{

		n0 = STUDY->Surfaces_List[S].Surface_Triangle_Set[tt].Nodes[0];
		n1 = STUDY->Surfaces_List[S].Surface_Triangle_Set[tt].Nodes[1];
		n2 = STUDY->Surfaces_List[S].Surface_Triangle_Set[tt].Nodes[2];

		vt1 = glm::vec3( STUDY->Surfaces_List[S].Surface_Node_Set[n0].x,
						 STUDY->Surfaces_List[S].Surface_Node_Set[n0].y,
						 STUDY->Surfaces_List[S].Surface_Node_Set[n0].z );

		vt2 = glm::vec3( STUDY->Surfaces_List[S].Surface_Node_Set[n1].x,
						 STUDY->Surfaces_List[S].Surface_Node_Set[n1].y,
						 STUDY->Surfaces_List[S].Surface_Node_Set[n1].z );

		vt3 = glm::vec3( STUDY->Surfaces_List[S].Surface_Node_Set[n2].x,
						 STUDY->Surfaces_List[S].Surface_Node_Set[n2].y,
						 STUDY->Surfaces_List[S].Surface_Node_Set[n2].z );

		if ( ray_triangle_intersection_test( glmCamera_Position, glmWorld_Ray,
				vt1,vt2,vt3, &t_dist, &baryPosition ) )
		{
			// if more than one sphere is in path of ray, only use the closest one
			if ( -1 == closest_triangle_clicked || t_dist < closest_intersection )
			{
			  closest_triangle_clicked = tt;
			  closest_intersection = t_dist;
			  closest_Surface_clicked_tt = S;
			}
		}

		} // through all dps
	}

	//------------------------------------------------------------------
	// final selection
	//------------------------------------------------------------------
	if( !Segmenting )
	{

	if( closest_label_clicked >= 0 && closest_Surface_clicked_label >= 0 )
	{
		STUDY->Current_Surface = closest_Surface_clicked_label;
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Label = closest_label_clicked;
	}
	else
	if( closest_dp_clicked >= 0 && closest_Surface_clicked_dp >= 0 )
	{
		STUDY->Current_Surface = closest_Surface_clicked_dp;
		STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr = closest_dp_clicked;
	}


	}
	if( closest_triangle_clicked >= 0 && closest_Surface_clicked_tt >= 0 )
	{
		STUDY->Current_Surface = closest_Surface_clicked_tt;
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle = closest_triangle_clicked;
	}

}

//---------------------------------------------------------------------------

// this function to be called after every action modyfing displayed values

void OpenGL_Panel_Class::prepare_colors_for_display()
{
	// calculates RGB color value for each vertex of each geometry
	// accoring to current data point set and current value
	double Value,CV;
	double Min_Value, Max_Value,DP_Val;
	long Closest_DP_Ptr;

	for(int S=0;S<(signed)STUDY->Surfaces_List.size();S++)
	if( STUDY->Surfaces_List[S].Display_Whole_Dataset_Flag )
	{

	bool Reversed_Palette = STUDY->Surfaces_List[S].Map_Values.get_current_value_inverted_flag();

	int dset = STUDY->Surfaces_List[S].Current_Data_Point_Set_Ptr;
	int Current_Value_Ptr = STUDY->Surfaces_List[S].Map_Values.get_current_value_ptr();
	AnsiString Current_Value_Name = STUDY->Surfaces_List[S].Map_Values.Current_Map_Value_Name;

	if( Current_Value_Name.Length() > 0 &&
		dset >= 0 && dset < STUDY->Surfaces_List[S].Data_Point_Set.size())
	{

	Min_Value = STUDY->Min_Value_On_All_Maps;
	Max_Value = STUDY->Max_Value_On_All_Maps;

	//---------------------------------------
	// Computing colors of geometry vertices
	//---------------------------------------
	 for(long n=0; n<(signed)STUDY->Surfaces_List[S].Surface_Node_Set.size();n++)
	 {
		Value = STUDY->Surfaces_List[S].Surface_Node_Set[n].get_value(dset, Current_Value_Ptr);

		Closest_DP_Ptr = STUDY->Surfaces_List[S].Surface_Node_Set[n].Closest_Data_Point_Id;
		if( Closest_DP_Ptr >= 0 )
		DP_Val = STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[Closest_DP_Ptr].
					get_value(Current_Value_Name,STUDY->Surfaces_List[S].Map_Values.get_values_table_ref() );

		if(    Value != NOT_POSSIBLE_TO_CALCULATE_VALUE )
		{
			if( Max_Value - Min_Value != 0 && fabs(Max_Value - Min_Value) > 0.00000001 )
			CV = 256.0 - 256.0 * ( Value - Min_Value ) / ( Max_Value - Min_Value );
			else
			CV = 0;

			if( CV < 0 ) CV = 0;
			if( CV > 255 ) CV = 255;

			if( Reversed_Palette )
			CV = 255 - CV;

			STUDY->Surfaces_List[S].Surface_Node_Set[n].R =
				OpenGL_Panel_Display_Parameters.Palette[(int)CV][0]/255.0;
			STUDY->Surfaces_List[S].Surface_Node_Set[n].G =
				OpenGL_Panel_Display_Parameters.Palette[(int)CV][1]/255.0;
			STUDY->Surfaces_List[S].Surface_Node_Set[n].B =
				OpenGL_Panel_Display_Parameters.Palette[(int)CV][2]/255.0;
		}
		else
		{
			STUDY->Surfaces_List[S].Surface_Node_Set[n].R = 0.5;
			STUDY->Surfaces_List[S].Surface_Node_Set[n].G = 0.5;
			STUDY->Surfaces_List[S].Surface_Node_Set[n].B = 0.5;
		}
	 } // through all nodes

	//------------------------------------
	// Computing data points colors
	//------------------------------------
	long CN = STUDY->Surfaces_List[S].Current_Data_Point_Ptr;
	long Perpendicular_DP_1=-1,Perpendicular_DP_2=-1;

	 for(long n=0; n<(signed)STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points.size();n++)
	 {
		Value = STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].
			get_value(Current_Value_Name,
					  STUDY->Surfaces_List[S].Map_Values.get_values_table_ref() );

		if( STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].
				is_data_point_value_valid(Current_Value_Name, STUDY->Surfaces_List[S].Map_Values.get_values_table_ref())
		   )
		{
			if( Max_Value - Min_Value != 0 && fabs(Max_Value - Min_Value) > 0.00000001 )
			CV = 256.0 - 256.0 * ( Value - Min_Value ) / ( Max_Value - Min_Value );
			else
			CV = 0;

			if( CV < 0 ) CV = 0;
			if( CV > 255 ) CV = 255;

			if( Reversed_Palette )
			CV = 255 - CV;

			STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].R =
				OpenGL_Panel_Display_Parameters.Palette[(int)CV][0]/255.0;
			STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].G =
				OpenGL_Panel_Display_Parameters.Palette[(int)CV][1]/255.0;
			STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].B =
				OpenGL_Panel_Display_Parameters.Palette[(int)CV][2]/255.0;
		}
		else
		{
			STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].R = 0.5;
			STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].G = 0.5;
			STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].B = 0.5;
		}

		if( n==Perpendicular_DP_1 || n==Perpendicular_DP_2)
		{
			STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].R = 0.1;
			STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].G = 0.1;
			STUDY->Surfaces_List[S].Data_Point_Set[dset].Data_Points[n].B = 0.1;
		}

	 } // through all dps

		//-----------------------------------
		// Generate contours
		//-----------------------------------
		if( STUDY->Surfaces_List[S].Map_Values.get_current_value_contours_display_flag() )
		{

		double Interval = STUDY->Surfaces_List[S].Map_Values.get_current_value_contours_interval();
		Interval = fabs(Interval);
		STUDY->Surfaces_List[S].Contours_Set.clear();

		if( STUDY->Surfaces_List[S].Map_Values.Current_Map_Value_Name !=
			SEGMENTATION_VALUE_NAME )
		if((Max_Value - Min_Value )/ Interval < 200 )
			STUDY->Surfaces_List[S].generate_contours(Interval);
		}

	STUDY->Surfaces_List[S].Volume = STUDY->Surfaces_List[S].get_volume();
	STUDY->Surfaces_List[S].Area = STUDY->Surfaces_List[S].get_total_area();

	double av,min,max,SD,CovC,median;
	STUDY->Surfaces_List[S].calculate_statistics_for_Values(0,Current_Value_Ptr,
			&av,&median,&min,&max,&SD,&CovC,dset,false); // ,bool Exclude_Zero_Values);
	STUDY->Surfaces_List[S].Mean_Map_Value = av;
	STUDY->Surfaces_List[S].SD_of_Map_Value = SD;

	} // if Value_Name > 0
	} // through all surfaces
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::paint_surface(int Surface_Ptr,int DP_Set,bool For_Mouse_Feedback)
{
	if( Surface_Ptr >=0 && Surface_Ptr < STUDY->Surfaces_List.size() )
	{

	int ptr1,ptr2,ptr3,Segment_Id;
	double x1,y1,z1,x2,y2,z2,c1,c2,c3,x3,y3,z3;
	double x,y,z,c,Area_Color,R,G,B,v;
	int Special_Color; // flag whether triangle has special color

	AnsiString Current_Value_Name =
		STUDY->Surfaces_List[Surface_Ptr].Map_Values.Current_Map_Value_Name;

   if( Surface_Ptr >=0 && Surface_Ptr < STUDY->Surfaces_List.size() )
   {

	//---------------------------------------------------------------------------
	// painting normals (paint normals)
	//---------------------------------------------------------------------------
	if( OpenGL_Panel_Display_Parameters.Display_Normals )
	{
	double n0,n1,n2;
	double A = 20*OpenGL_Panel_Display_Parameters.Data_Point_Size;
	glLineWidth(1.0);
	glBegin(GL_LINES);
	for( long i=0; i <(signed) STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set.size(); i++)
	if( STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Removed_Flag != ON )
	{
		ptr1 = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Nodes[0];
		x1 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].x;
		y1 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].y;
		z1 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].z;

		ptr2 = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Nodes[1];
		x2 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].x;
		y2 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].y;
		z2 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].z;

		ptr3 = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Nodes[2];
		x3 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].x;
		y3 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].y;
		z3 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].z;

		n0 = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].normal[0];
		n1 = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].normal[1];
		n2 = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].normal[2];

		glVertex3f( (x1+x2+x3)/3., (y1+y2+y3)/3., (z1+z2+z3)/3. );
		glVertex3f(
			 (x1+x2+x3)/3.+A*STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].normal[0],
			 (y1+y2+y3)/3.+A*STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].normal[1],
			 (z1+z2+z3)/3.+A*STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].normal[2]);
	}
	glEnd();
	}// painitng normals

	if( OpenGL_Panel_Display_Parameters.Display_Geo_Edges )
	{
	double x2,y2,z2;
	long nptr;
	glLineWidth(1.5);
	glColor4f(0.1,0.1,0.1,OpenGL_Panel_Display_Parameters.Transparency_Level);
	glBegin(GL_LINES);
	for(long i=0; i <(signed) STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set.size(); i++)
	for(long j=0; j <(signed) STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[i].Neighbors.size(); j++)
	{
		x = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[i].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[i].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[i].z;

		nptr = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[i].Neighbors[j];

		x2 = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[nptr].x;
		y2 = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[nptr].y;
		z2 = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[nptr].z;

		glVertex3f(x,y,z);
		glVertex3f(x2,y2,z2);
	}

	glEnd();

	}

	//---------------------------------------------------------------------------
	//---------------------------------------------------------------------------
	// paint triangles ttt
	//---------------------------------------------------------------------------
	//---------------------------------------------------------------------------
	if( STUDY->Surfaces_List[Surface_Ptr].Display_Geometry_Flag )
	{

	glBegin(GL_TRIANGLES);

	for( long i=0; i <(signed) STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set.size(); i++)
	if( STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Removed_Flag != ON )
	{
		ptr1 = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Nodes[0];
		x1 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].x;
		y1 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].y;
		z1 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].z;

		ptr2 = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Nodes[1];
		x2 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].x;
		y2 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].y;
		z2 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].z;

		ptr3 = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Nodes[2];
		x3 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].x;
		y3 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].y;
		z3 =  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].z;

		if( !OpenGL_Panel_Display_Parameters.Limit_Images_Display_Bottom &&
			!OpenGL_Panel_Display_Parameters.Limit_Images_Display_Top
			 ||
			 OpenGL_Panel_Display_Parameters.Limit_Images_Display_Bottom && y1 >
			 OpenGL_Panel_Display_Parameters.Y_Cut ||
			 OpenGL_Panel_Display_Parameters.Limit_Images_Display_Top && y1 <
			 OpenGL_Panel_Display_Parameters.Y_Cut )
		{

		// setting colors
		Special_Color = 0;

		//---------------------------------------------------------------------------
		// PAINTING SEGMENTS segmentsgogh
		//---------------------------------------------------------------------------
		if( Current_Value_Name == SEGMENTATION_VALUE_NAME )
		if(STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Segment_Id > 0)
		if( STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Segment_Id <
				   Segments_Info->Segments.size() )
		{

		Segment_Id = STUDY->Surfaces_List[Surface_Ptr].Surface_Triangle_Set[i].Segment_Id;

		R = Segments_Info->Segments[Segment_Id].Color_R/255.;
		G = Segments_Info->Segments[Segment_Id].Color_G/255.;
		B = Segments_Info->Segments[Segment_Id].Color_B/255.;

		glColor4f(R,G,B,OpenGL_Panel_Display_Parameters.Transparency_Level);

		Special_Color = 1;

		glNormal3fv(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].normal);
		glVertex3f( x1, y1, z1 );
		glNormal3fv(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].normal);
		glVertex3f( x2, y2, z2 );
		glNormal3fv(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].normal);
		glVertex3f( x3, y3, z3 );

		} // painting segments

		//--------------------------------
		// nothing special, paint according to value
		//--------------------------------
		if( Special_Color == 0 )
		{

		// case when data set ptr >=0
		if(STUDY->Surfaces_List[Surface_Ptr].data_points_set_ptr_in_range())
		{

		glColor4f(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].R,
				  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].G,
				  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].B,
				  OpenGL_Panel_Display_Parameters.Transparency_Level);
		glNormal3fv(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].normal);
		glVertex3f( x1, y1, z1 );

		glColor4f(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].R,
				  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].G,
				  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].B,
				  OpenGL_Panel_Display_Parameters.Transparency_Level);
		glNormal3fv(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].normal);
		glVertex3f( x2, y2, z2 );

		glColor4f(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].R,
				  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].G,
				  STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].B,
				  OpenGL_Panel_Display_Parameters.Transparency_Level);
		glNormal3fv(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].normal);
		glVertex3f( x3, y3, z3 );

		} // case where data set is not -1

		//---------------------------------------------------------------------------
		// case where data set is == -1 (just gray surface)
		//---------------------------------------------------------------------------
		if(!STUDY->Surfaces_List[Surface_Ptr].data_points_set_ptr_in_range())
		{

		glColor4f( 0.5,0.5,0.5,OpenGL_Panel_Display_Parameters.Transparency_Level);

		glNormal3fv(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr1].normal);
		glVertex3f( x1, y1, z1 );
		glNormal3fv(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr2].normal);
		glVertex3f( x2, y2, z2 );
		glNormal3fv(STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[ptr3].normal);
		glVertex3f( x3, y3, z3 );

		} // case where data set is == -1


		} // paintint according to value

		} // if triangle within cut conditions

	   } // through all triangles

	   glEnd();

	} // if paint triangles

	//------------------------------------------------------------------------
	// SEGMENTS NAMES goghseg
	//------------------------------------------------------------------------
	AnsiString Seg_Name;
	if( Display_Options_Form_1->Display_Segments_CheckBox->State == cbChecked )
	if( !For_Mouse_Feedback )
	{

	glColor4f( 0.0f,0.0f,0.0f,OpenGL_Panel_Display_Parameters.Transparency_Level);

	// through all segments
	double DistsN = OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo;
	double sx,sy,sz;
	double nx,ny,nz;
	for(int S=0;S<STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_X.size();S++)
	if( STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_X[S] != 0 ||
		STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_Y[S] != 0 ||
		STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_Z[S] != 0 )
	{
		 Seg_Name = Segments_Info->get_segment_name(S);

		 sx = STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_X[S];
		 sy = STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_Y[S];
		 sz = STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_Z[S];

		 nx = STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_X_normal[S];
		 ny = STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_Y_normal[S];
		 nz = STUDY->Surfaces_List[Surface_Ptr].Segments_Centers_Z_normal[S];

		 renderBitmapString(sx+DistsN*nx,sy+DistsN*ny,sz+DistsN*nz,
			(void*)OpenGL_Panel_Display_Parameters.Font_Type, Seg_Name.c_str() );
	}

	}

	//------------------------------------------------------------------------
	// CONTOURS
	//------------------------------------------------------------------------
	glDisable(GL_LIGHTING);
	double spacing = 0.1;

	if( STUDY->Surfaces_List[Surface_Ptr].Map_Values.get_current_value_contours_display_flag() )
	{
			glColor4f( 0,0,0,OpenGL_Panel_Display_Parameters.Transparency_Level );
			glLineWidth(OpenGL_Panel_Display_Parameters.Contour_Line_Thickness);
			glBegin(GL_LINES);

			glNormal3f(0,1,0);

			for( long i=0; i<(signed)STUDY->Surfaces_List[Surface_Ptr].Contours_Set.size(); i++ )
			{
				  x = STUDY->Surfaces_List[Surface_Ptr].Contours_Set[i].x1;
				  y = STUDY->Surfaces_List[Surface_Ptr].Contours_Set[i].y1;
				  z = STUDY->Surfaces_List[Surface_Ptr].Contours_Set[i].z1;
				  glVertex3f( x, y, z );

				  x = STUDY->Surfaces_List[Surface_Ptr].Contours_Set[i].x2;
				  y = STUDY->Surfaces_List[Surface_Ptr].Contours_Set[i].y2;
				  z = STUDY->Surfaces_List[Surface_Ptr].Contours_Set[i].z2;
				  glVertex3f( x, y, z );
			} // for

	glEnd();

	}

	//------------------------------------------------
	// paint path points
	//------------------------------------------------
	// gogh path
	if( 1 )
	{

	double Marker_Sphere_Size=1.5*OpenGL_Panel_Display_Parameters.Data_Point_Size,Grad;
	long Node_Pt=-1;
	AnsiString Marker_Name;

	for(long M=0;M<3;M++)
	{

	if( M==0 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Path_Start;
		Marker_Name = "Start";
	}
	if( M==1 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Path_Intermediate;
		Marker_Name = "Intermediate";
	}

	if( M==2 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Path_End;
		Marker_Name = "End ";
		double d = STUDY->Surfaces_List[Surface_Ptr].get_path_length(
			STUDY->Surfaces_List[Surface_Ptr].Path_Start,
			STUDY->Surfaces_List[Surface_Ptr].Path_End );
		if( d > 0 )
		{
			Marker_Name += " (length=" + FloatToStrF(d,ffGeneral,3,2 ) + " mm)";

			Grad = STUDY->Surfaces_List[Surface_Ptr].get_path_value_difference(
				STUDY->Surfaces_List[Surface_Ptr].Path_Start,
				STUDY->Surfaces_List[Surface_Ptr].Path_End );
			if( Grad != 0 )
			Marker_Name += " (1/gradient=" + FloatToStrF(d/Grad,ffGeneral,3,2 ) + " mm/[])";
		}
	}

	if( Node_Pt >= 0 && Node_Pt < STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set.size() )
	{
		x = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].z;

		double nx = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[
				Node_Pt].normal[0];
		double ny = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[
				Node_Pt].normal[1];
		double nz = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[
				Node_Pt].normal[2];

		glColor4f( 0.2f,0.2f,0.2f,OpenGL_Panel_Display_Parameters.Transparency_Level);

		glPushMatrix();
		glTranslatef(x, y, z);
		paint_sphere(Marker_Sphere_Size);
		glPopMatrix();

		renderBitmapString(
			x + OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*nx,
			y + OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*ny,
			z + OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*nz,
			(void*)OpenGL_Panel_Display_Parameters.Font_Type,
			Marker_Name.c_str());
	}

	} // for M
	} // if path marker display

	//------------------------------------------------
	// paint path
	//------------------------------------------------
	if( 1 )
	{

	double Path_Sphere_Size=2.5*OpenGL_Panel_Display_Parameters.Data_Point_Size;
	vector <long> *Path;

	Path = &STUDY->Surfaces_List[Surface_Ptr].Path_Vector;

	for(long pn=0;pn<Path[0].size();pn++)
	{
	int t1=Path[0].size();
	int t2=Path[0][0];

		x = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Path[0][pn]].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Path[0][pn]].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Path[0][pn]].z;

		glColor4f( 1.0f,1.0f,1.0f,OpenGL_Panel_Display_Parameters.Transparency_Level);
		glPushMatrix();
		glTranslatef(x, y, z);
		paint_sphere(Path_Sphere_Size);
		glPopMatrix();
	} // for

	} // painting paths


	glEnable(GL_LIGHTING);

   } // if paint surface flag on

	//--------------------------------------------------------------------------------
	// Paint registration points on the 3D surface
	//--------------------------------------------------------------------------------
	double Sphere_Size = 3*OpenGL_Panel_Display_Parameters.Data_Point_Size;

	if( OpenGL_Panel_Display_Parameters.Paint_Registration_Points )
	{
		if( STUDY->Surfaces_List[Surface_Ptr].Point_A_Located )
		{
			glColor4f(1,0,0,OpenGL_Panel_Display_Parameters.Transparency_Level);
			glPushMatrix();
			glTranslatef(STUDY->Surfaces_List[Surface_Ptr].Registration_Point_A.x,
						 STUDY->Surfaces_List[Surface_Ptr].Registration_Point_A.y,
						 STUDY->Surfaces_List[Surface_Ptr].Registration_Point_A.z );
			paint_sphere(Sphere_Size);
			glPopMatrix();

			renderBitmapString(STUDY->Surfaces_List[Surface_Ptr].Registration_Point_A.x +
								OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo
							   *STUDY->Surfaces_List[Surface_Ptr].Registration_Point_A_normal.x,
								STUDY->Surfaces_List[Surface_Ptr].Registration_Point_A.y +
								OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*
								STUDY->Surfaces_List[Surface_Ptr].Registration_Point_A_normal.y,
								STUDY->Surfaces_List[Surface_Ptr].Registration_Point_A.z +
								OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo
								*STUDY->Surfaces_List[Surface_Ptr].Registration_Point_A_normal.z,
							   (void*)OpenGL_Panel_Display_Parameters.Font_Type,
							   "Geo Registration Point A");
		}

		if( STUDY->Surfaces_List[Surface_Ptr].Point_B_Located )
		{
			glColor4f(0,1,0,OpenGL_Panel_Display_Parameters.Transparency_Level);
			glPushMatrix();
			glTranslatef(STUDY->Surfaces_List[Surface_Ptr].Registration_Point_B.x,
						 STUDY->Surfaces_List[Surface_Ptr].Registration_Point_B.y,
						 STUDY->Surfaces_List[Surface_Ptr].Registration_Point_B.z );
			paint_sphere(Sphere_Size);
			glPopMatrix();

			renderBitmapString(STUDY->Surfaces_List[Surface_Ptr].Registration_Point_B.x +
				 OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo
				 *STUDY->Surfaces_List[Surface_Ptr].Registration_Point_B_normal.x,
							   STUDY->Surfaces_List[Surface_Ptr].Registration_Point_B.y +
				 OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo
				 *STUDY->Surfaces_List[Surface_Ptr].Registration_Point_B_normal.y,
							   STUDY->Surfaces_List[Surface_Ptr].Registration_Point_B.z +
				 OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo
				 *STUDY->Surfaces_List[Surface_Ptr].Registration_Point_B_normal.z,
							   (void*)OpenGL_Panel_Display_Parameters.Font_Type,
							   "Geo Registration Point B");
		}

		if( STUDY->Surfaces_List[Surface_Ptr].Point_C_Located )
		{
			glColor4f(0,0,1,OpenGL_Panel_Display_Parameters.Transparency_Level);
			glPushMatrix();
			glTranslatef(STUDY->Surfaces_List[Surface_Ptr].Registration_Point_C.x,
						 STUDY->Surfaces_List[Surface_Ptr].Registration_Point_C.y,
						 STUDY->Surfaces_List[Surface_Ptr].Registration_Point_C.z );
			paint_sphere(Sphere_Size);
			glPopMatrix();

			renderBitmapString(STUDY->Surfaces_List[Surface_Ptr].Registration_Point_C.x +
				 OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*
				 STUDY->Surfaces_List[Surface_Ptr].Registration_Point_C_normal.x,
							   STUDY->Surfaces_List[Surface_Ptr].Registration_Point_C.y +
				 OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*
				 STUDY->Surfaces_List[Surface_Ptr].Registration_Point_C_normal.y,
							   STUDY->Surfaces_List[Surface_Ptr].Registration_Point_C.z +
				 OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*
				 STUDY->Surfaces_List[Surface_Ptr].Registration_Point_C_normal.z,
							   (void*)OpenGL_Panel_Display_Parameters.Font_Type,
							   "Geo Registration Point C");
		}
	}

   } // if surface ptr in range
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::paint_data_points(int Surface_Ptr,int Data_Point_Set_Ptr)
{
	if( Surface_Ptr >=0 && Surface_Ptr < STUDY->Surfaces_List.size() )
	if( OpenGL_Panel_Display_Parameters.Display_Data_Points )
	if( STUDY->Surfaces_List[Surface_Ptr].data_points_set_ptr_in_range() )
	{

	double x,y,z,c;
	int DP_Set = STUDY->Surfaces_List[Surface_Ptr].Current_Data_Point_Set_Ptr;
	double nx,ny,nz;
	double DistsN = OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo; // distance between geometry and text
	long CLD;

	// goghdp
	//--------------------------------------------------
	// data cloud display
	//--------------------------------------------------
	int display_type = 0; // Display_Options_Form_1->DP_Disp_Type_RadioGroup->ItemIndex;

	for( int i=0; i <(signed) STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points.size();i++)
	{
		x = STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[i].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[i].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[i].z;

		glColor4f(
			STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[i].R,
			STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[i].G,
			STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[i].B,
			OpenGL_Panel_Display_Parameters.Transparency_Level  );

		glPushMatrix();

		glTranslatef(x, y, z);
		if( i == STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].First_DP_Pointed_Ptr ||
			i == STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Second_DP_Pointed_Ptr )
		paint_sphere(1.5*OpenGL_Panel_Display_Parameters.Data_Point_Size);
		else
		paint_sphere(OpenGL_Panel_Display_Parameters.Data_Point_Size);

		glPopMatrix();
	//xxx
		// name
		if( OpenGL_Panel_Display_Parameters.Display_DP_Name )
		{

		CLD = STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[i].Closest_Node_Id;

		nx = DistsN*STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[CLD].normal[0];
		ny = DistsN*STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[CLD].normal[1];
		nz = DistsN*STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[CLD].normal[2];

		glColor4f( 0.0f,0.0f,0.0f,OpenGL_Panel_Display_Parameters.Transparency_Level);
		renderBitmapString(x+DistsN*nx,y+DistsN*ny,z+DistsN*nz,
			(void*)OpenGL_Panel_Display_Parameters.Font_Type,
				STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[i].Roving_Signal.Name.c_str() );
		}

	}


	//--------------------------------------------------------------------------------
	// Paint cross of currently selected data point
	//--------------------------------------------------------------------------------
	double Cross_Size = 10*OpenGL_Panel_Display_Parameters.Data_Point_Size;
	int DP = STUDY->Surfaces_List[Surface_Ptr].Current_Data_Point_Ptr;
	if( OpenGL_Panel_Display_Parameters.Display_Pointing_Cross )
	if( STUDY->Surfaces_List[Surface_Ptr].data_point_ptr_in_range() )
	{

	x = STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[DP].x;
	y = STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[DP].y;
	z = STUDY->Surfaces_List[Surface_Ptr].Data_Point_Set[DP_Set].Data_Points[DP].z;

	glColor4f(0.3,0.9,0.5,OpenGL_Panel_Display_Parameters.Transparency_Level);
	glLineWidth(4.0f);

	glBegin(GL_LINES);

	glVertex3f(x-Cross_Size,y,z);
	glVertex3f(x+Cross_Size,y,z);

	glVertex3f(x,y-Cross_Size,z);
	glVertex3f(x,y+Cross_Size,z);

	glVertex3f(x,y,z-Cross_Size);
	glVertex3f(x,y,z+Cross_Size);

	glEnd();
	}

	glFinish();

	} // if everything in range
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::paint_sphere(double Radius)
{
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, Radius, 4,4);
	gluDeleteQuadric(quadric);
}

//-------------------------------------------------------------------------

void OpenGL_Panel_Class::renderBitmapString(float x,float y,float z,void *font,char *string)
{
	if( OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color == 0 )
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if( OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color == 1 )
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glDisable(GL_LIGHTING);

	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);

	glEnable(GL_LIGHTING);
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::autoscale_data_point_size()
{
	if( OpenGL_Panel_Display_Parameters.Autoscale_Data_Point_Size )
	if( STUDY->Surfaces_List.size() > 0 )
	{
	double x,y,z,dmax;

	for(int S=0;S<STUDY->Surfaces_List.size();S++)
	STUDY->Surfaces_List[S].get_surface_dimensions(&x,&y,&z);
	dmax = (x+y+z)/3.0;

	double v = 3*OpenGL_Panel_Display_Parameters.DP_Size_Scaling_Ratio/100. *
				(dmax)/250.;

	if( v > 0 && v < 20 )
		OpenGL_Panel_Display_Parameters.Data_Point_Size = v;

	}
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::set_initial_zoom_in_3D_panel()
{
	if( STUDY->Surfaces_List.size() > 0 )
	{
	double x,y,z,dmax=-1;

	for(int S=0;S<STUDY->Surfaces_List.size();S++)
	STUDY->Surfaces_List[S].get_surface_dimensions(&x,&y,&z);
	dmax = std::max( std::max(x,y),z);

	if( dmax > 0 )
	{
		OpenGL_Panel_Display_Parameters.ZoomFactor = -584.*dmax/465;
	}
	}
}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::draw_fluoro_images_3D(int Surface_Ptr)
{
// goghfluoro

	double Sphere_Size;
	double x,y,z,c,t;
	double x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;
	double LAORAO_in_radians;
	double Translation_X,Translation_Y;
	double R,G,B,R1;
	double Distance_of_Image_From_Patient_Centre;
	double scaling_coef;
	double Y_Position=0;
	double Multiplier = 1.5;

	Image_2D_Class* Fluoro_Image;
	int Current_Image_Ptr = STUDY->Surfaces_List[Surface_Ptr].Images_Set.Current_Fluoro_Image;

	//---------------------------------------------------------------------
	// paint registration points
	//---------------------------------------------------------------------
	Sphere_Size = 2*OpenGL_Panel_Display_Parameters.Data_Point_Size;

	for(int image_no=0;image_no<STUDY->Surfaces_List[Surface_Ptr].Images_Set.Images_2D_List.size();image_no++)
	{

	Fluoro_Image = &STUDY->Surfaces_List[Surface_Ptr].Images_Set.Images_2D_List[image_no];

	if(Fluoro_Image->Point_A_Located == true )
	{
		glColor4f(1,0,0,OpenGL_Panel_Display_Parameters.Transparency_Level);

		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			Fluoro_Image->Registration_Point_A_ij.x,
			Fluoro_Image->Registration_Point_A_ij.y,
			image_no,&x,&y,&z);

			glPushMatrix();
			glTranslatef(x,y,z);
			paint_sphere(Sphere_Size);
			glPopMatrix();

			renderBitmapString(x+OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo,y,z,(void*)OpenGL_Panel_Display_Parameters.Font_Type,
							   "Image Registration Point A");
	}

	if( Fluoro_Image->Point_B_Located == true )
	{
		glColor4f(0,1,0,OpenGL_Panel_Display_Parameters.Transparency_Level);
		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			Fluoro_Image->Registration_Point_B_ij.x,
			Fluoro_Image->Registration_Point_B_ij.y,
			image_no,&x,&y,&z);

		glPushMatrix();
		glTranslatef(x,y,z);
		paint_sphere(Sphere_Size);
		glPopMatrix();

		renderBitmapString(x+OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo,y,z,(void*)OpenGL_Panel_Display_Parameters.Font_Type,
							   "Image Registration Point B");
	}

	if( Fluoro_Image->Point_C_Located == true )
	{
		glColor4f(0,0,1,OpenGL_Panel_Display_Parameters.Transparency_Level);
		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			Fluoro_Image->Registration_Point_C_ij.x,
			Fluoro_Image->Registration_Point_C_ij.y,
			image_no,&x,&y,&z);

		glPushMatrix();
		glTranslatef(x,y,z);
		paint_sphere(Sphere_Size);
		glPopMatrix();

		renderBitmapString(x+OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo,y,z,(void*)OpenGL_Panel_Display_Parameters.Font_Type,
							   "Image Registration Point C");
	}
	}


	if( OpenGL_Panel_Display_Parameters.Display_Images_In_3D_Panel )
	{

	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	for(int image_no=0;image_no<STUDY->Surfaces_List[Surface_Ptr].Images_Set.Images_2D_List.size();image_no++)
	if( STUDY->Surfaces_List[Surface_Ptr].Images_Set.Images_2D_List[image_no].Displayed_Flag == true )
	if( !OpenGL_Panel_Display_Parameters.Limit_Images_Display_Bottom ||
		 OpenGL_Panel_Display_Parameters.Limit_Images_Display_Bottom &&
		 image_no >= Current_Image_Ptr )
	if( !OpenGL_Panel_Display_Parameters.Limit_Images_Display_Top ||
		 OpenGL_Panel_Display_Parameters.Limit_Images_Display_Top &&
		 image_no <= Current_Image_Ptr )
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	{

	Y_Position += STUDY->Surfaces_List[Surface_Ptr].Images_Set.
		Images_2D_List[image_no].Slice_Thickness;

	Fluoro_Image = &STUDY->Surfaces_List[Surface_Ptr].Images_Set.Images_2D_List[image_no];

	Distance_of_Image_From_Patient_Centre = 0.5*Fluoro_Image->Distance_from_Emiter;

	int FWidth = Fluoro_Image->Fluoro_Image_Width_Small;
	int FHeight = Fluoro_Image->Fluoro_Image_Height_Small;

	scaling_coef = (double)Fluoro_Image->Fluoro_Image_Height/
				   (double)Fluoro_Image->Fluoro_Image_Width;

	double scaling_coef_x = (double)Fluoro_Image->Fluoro_Image_Width/
							(double)FWidth;

	double scaling_coef_y = (double)Fluoro_Image->Fluoro_Image_Height/
							(double)FHeight;
	int iptr,jptr;



	//---------------------------------------------------------------------
	// paint image
	//---------------------------------------------------------------------
	LAORAO_in_radians = 2.0*M_PI*Fluoro_Image->LAO_RAO/360.0;
	Translation_X = Distance_of_Image_From_Patient_Centre*sin(LAORAO_in_radians);
	Translation_Y = -Distance_of_Image_From_Patient_Centre*cos(LAORAO_in_radians);

	glPushMatrix();

	// perform transformations:
	glRotatef(Fluoro_Image->LAO_RAO,0,0,1);
	glTranslatef(0,0,0 );

	glDisable(GL_LIGHTING); // Turn off lighting (to avoid effects from the light source position)

	// painting

	//-----------------------------------------------------------------------
	if( !OpenGL_Panel_Display_Parameters.Remove_Blue_Images_Background &&
		!OpenGL_Panel_Display_Parameters.Remove_Green_Images_Background )
	//-----------------------------------------------------------------------
	for(int j=0;j<FHeight;j++)
	{
	glBegin(GL_QUAD_STRIP);
	for(int i=0;i<FWidth;i++)
	if( 3*(j*FWidth+i)+2 < Fluoro_Image->Fluoro_Image_Data_Small_vector.size() )
	{
		R = Fluoro_Image->Fluoro_Image_Data_Small_vector[3*(j*FWidth+i)+0];
		G = Fluoro_Image->Fluoro_Image_Data_Small_vector[3*(j*FWidth+i)+1];
		B = Fluoro_Image->Fluoro_Image_Data_Small_vector[3*(j*FWidth+i)+2];

	   glColor4f( R/255.0,G/255.0,B/255.0,1); // no transparency!!!

	   STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			i*scaling_coef_x,j*scaling_coef_y,image_no,&x,&y,&z);
	   glVertex3f( x, y, z );

	   STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			i*scaling_coef_x,(j+1)*scaling_coef_y,image_no,&x,&y,&z);
	   glVertex3f( x, y, z );

	} // for: width
	glEnd();
	} // for: height

	//-----------------------------------------------------------------------
	if( OpenGL_Panel_Display_Parameters.Remove_Blue_Images_Background )
	//-----------------------------------------------------------------------
	for(int j=0;j<FHeight;j++)
	{
	glBegin(GL_TRIANGLES);
	for(int i=0;i<FWidth;i++)
	if( 3*(j*FWidth+i)+2 < Fluoro_Image->Fluoro_Image_Data_Small_vector.size() )
	{
		R = Fluoro_Image->Fluoro_Image_Data_Small_vector[3*(j*FWidth+i)+0];
		G = Fluoro_Image->Fluoro_Image_Data_Small_vector[3*(j*FWidth+i)+1];
		B = Fluoro_Image->Fluoro_Image_Data_Small_vector[3*(j*FWidth+i)+2];

		glColor4f( R/255.0,G/255.0,B/255.0,1); // no transparency!!!

		if( !(B > Multiplier*(R+G) )  )
		{

		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			i*scaling_coef_x,j*scaling_coef_y,image_no,&x1,&y1,&z1);
		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			i*scaling_coef_x,(j+1)*scaling_coef_y,image_no,&x2,&y2,&z2);

		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			(i+1)*scaling_coef_x,j*scaling_coef_y,image_no,&x3,&y3,&z3);
		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			(i+1)*scaling_coef_x,(j+1)*scaling_coef_y,image_no,&x4,&y4,&z4);

		glVertex3f( x1, y1, z1 );
		glVertex3f( x2, y2, z2 );
		glVertex3f( x3, y3, z3 );

		glVertex3f( x2, y2, z2 );
		glVertex3f( x3, y3, z3 );
		glVertex3f( x4, y4, z4 );

		}

	} // for: width
	glEnd();
	} // for: height

	//-----------------------------------------------------------------------
	if( OpenGL_Panel_Display_Parameters.Remove_Green_Images_Background )
	//-----------------------------------------------------------------------
	for(int j=0;j<FHeight;j++)
	{
	glBegin(GL_TRIANGLES);
	for(int i=0;i<FWidth;i++)
	if( 3*(j*FWidth+i)+2 < Fluoro_Image->Fluoro_Image_Data_Small_vector.size() )
	{
		R = Fluoro_Image->Fluoro_Image_Data_Small_vector[3*(j*FWidth+i)+0];
		G = Fluoro_Image->Fluoro_Image_Data_Small_vector[3*(j*FWidth+i)+1];
		B = Fluoro_Image->Fluoro_Image_Data_Small_vector[3*(j*FWidth+i)+2];

		glColor4f( R/255.0,G/255.0,B/255.0,1); // no transparency!!!

		if( !(G > Multiplier*(R+B))  )
		{

		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			i*scaling_coef_x,j*scaling_coef_y,image_no,&x1,&y1,&z1);
		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			i*scaling_coef_x,(j+1)*scaling_coef_y,image_no,&x2,&y2,&z2);

		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			(i+1)*scaling_coef_x,j*scaling_coef_y,image_no,&x3,&y3,&z3);
		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			(i+1)*scaling_coef_x,(j+1)*scaling_coef_y,image_no,&x4,&y4,&z4);

		glVertex3f( x1, y1, z1 );
		glVertex3f( x2, y2, z2 );
		glVertex3f( x3, y3, z3 );

		glVertex3f( x2, y2, z2 );
		glVertex3f( x3, y3, z3 );
		glVertex3f( x4, y4, z4 );

		}
		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			i*scaling_coef_x,j*scaling_coef_y,image_no,&x,&y,&z);
		glVertex3f( x, y, z );

		STUDY->Surfaces_List[Surface_Ptr].Images_Set.ijk_to_xyz(
			i*scaling_coef_x,(j+1)*scaling_coef_y,image_no,&x,&y,&z);
		glVertex3f( x, y, z );

	} // for: width
	glEnd();
	} // for: height

	glPopMatrix();

	} // if image is to be displayed

	glEnable(GL_LIGHTING);
}

}

//---------------------------------------------------------------------------

void OpenGL_Panel_Class::paint_17_LV_segmentation_markers(int Surface_Ptr)
{
	double x,y,z;

 if( OpenGL_Panel_Display_Parameters.Display_17_LV_Markers )
 {
	//------------------------------------------------
	// paint anatomical markers MAIN POINTS
	//------------------------------------------------
	double Marker_Sphere_Size=2*OpenGL_Panel_Display_Parameters.Data_Point_Size;
	long Node_Pt=-1;
	AnsiString Marker_Name;

	for(long M=0;M<6;M++)
	{

	if( M==0 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Apex_Node_Ptr;
		Marker_Name = "APEX";
	}
	if( M==1 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Septal_Anterior_Node_Ptr;
		Marker_Name = "Septal_Anterior";
	}
	if( M==2 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Septal_Lateral_Node_Ptr;
		Marker_Name = "Septal_Lateral";
	}
	if( M==3 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Anterior_Lateral_Node_Ptr;
		Marker_Name = "Anterior_Lateral";
	}
	if( M==4 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Path_Start;
		Marker_Name = "Start";
	}
	if( M==5 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Path_End;
		Marker_Name = "End";
	}

	if( Node_Pt >= 0 && Node_Pt < STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set.size() )
	{
		x = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].z;

		double nx = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[
				Node_Pt].normal[0];
		double ny = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[
				Node_Pt].normal[1];
		double nz = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[
				Node_Pt].normal[2];

		glColor4f( 0.1f,0.1f,0.1f,OpenGL_Panel_Display_Parameters.Transparency_Level);
		glPushMatrix();
		glTranslatef(x, y, z);
		paint_sphere(Marker_Sphere_Size);
		glPopMatrix();

		renderBitmapString(
			x + OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*nx,
			y + OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*ny,
			z + OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*nz,
			(void*)OpenGL_Panel_Display_Parameters.Font_Type,
			Marker_Name.c_str());
	}

	} // for M

	//------------------------------------------------
	// paint anatomical markers INTERMEDIATES
	//------------------------------------------------
	Node_Pt=-1;

	for(long M=0;M<7;M++)
	{

	if( M==0 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Ap_SA_Intermediate;
		Marker_Name = "Ap_SA_Intermediate";
	}
	if( M==1 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Ap_SL_Intermediate;
		Marker_Name = "Ap_SL_Intermediate";
	}
	if( M==2 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Ap_AL_Intermediate;
		Marker_Name = "Ap_AL_Intermediate";
	}
	if( M==3 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Path_Intermediate;
		Marker_Name = "Intermediate";
	}

	if( M==4 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Lat_Intermediate;
		Marker_Name = "Lat_Base_Intermediate";
	}
	if( M==5 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Sept_Intermediate;
		Marker_Name = "Sept_Base_Intermediate";
	}

	if( M==6 )
	{
		Node_Pt = STUDY->Surfaces_List[Surface_Ptr].Ant_Intermediate;
		Marker_Name = "Ant_Base_Intermediate";
	}

	if( Node_Pt >= 0 )
	{
		x = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].z;

		double nx = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].normal[0];
		double ny = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].normal[1];
		double nz = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Node_Pt].normal[2];

		glColor4f( 0.1f,0.1f,0.1f,OpenGL_Panel_Display_Parameters.Transparency_Level);
		glPushMatrix();
		glTranslatef(x, y, z);
		paint_sphere( Marker_Sphere_Size );
		glPopMatrix();

		renderBitmapString(
			x + OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*nx,
			y + OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*ny,
			z + OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo*nz,
			(void*)OpenGL_Panel_Display_Parameters.Font_Type,
			Marker_Name.c_str());
	}

	} // for M

	//------------------------------------------------
	// paint paths anatomical markers
	//------------------------------------------------
	vector <long> *Path;

	for(long M=0;M<7;M++)
	{

	if( M==0 )
	Path = &STUDY->Surfaces_List[Surface_Ptr].Ap_SA_Path;
	if( M==1 )
	Path = &STUDY->Surfaces_List[Surface_Ptr].Ap_SL_Path;
	if( M==2 )
	Path = &STUDY->Surfaces_List[Surface_Ptr].Ap_AL_Path;

	if( M==3 )
	Path = &STUDY->Surfaces_List[Surface_Ptr].Septal_Base_Path;
	if( M==4 )
	Path = &STUDY->Surfaces_List[Surface_Ptr].Anterior_Base_Path;
	if( M==5 )
	Path = &STUDY->Surfaces_List[Surface_Ptr].Lateral_Base_Path;

	if( M==6 )
	Path = &STUDY->Surfaces_List[Surface_Ptr].Path_Vector;

	for(long pn=0;pn<Path[0].size();pn++)
	{
		x = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Path[0][pn]].x;
		y = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Path[0][pn]].y;
		z = STUDY->Surfaces_List[Surface_Ptr].Surface_Node_Set[Path[0][pn]].z;

		glColor4f( 1.0f,1.0f,1.0f,OpenGL_Panel_Display_Parameters.Transparency_Level);
		glPushMatrix();
		glTranslatef(x, y, z);
		paint_sphere( 0.9*OpenGL_Panel_Display_Parameters.Data_Point_Size);
		glPopMatrix();
	} // for

	} // painting paths


 }
}
//---------------------------------------------------------------------------
void OpenGL_Panel_Class::draw_Arrow(double x1,double y1,double z1,
	double x2,double y2,double z2,double D) // D is arrow thickness
{
	double x=x2-x1;
	double y=y2-y1;
	double z=z2-z1;
	double L=sqrt(x*x+y*y+z*z);
	const double RADPERDEG = 0.0174533;
	GLUquadricObj *quadObj;

	glPushMatrix ();

	glTranslated(x1,y1,z1);

	if((x!=0.)||(y!=0.))
	{
		glRotated(atan2(y,x)/RADPERDEG,0.,0.,1.);
		glRotated(atan2(sqrt(x*x+y*y),z)/RADPERDEG,0.,1.,0.);
	}
	else if (z<0)
	{
		glRotated(180,1.,0.,0.);
	}

	  glTranslatef(0,0,L-4*D);
/*
void gluCylinder(	GLUquadric* quad,
	GLdouble base,
	GLdouble top,
	GLdouble height,
	GLint slices,
	GLint stacks);

void gluDisk(	GLUquadric* quad,
	GLdouble inner,
	GLdouble outer,
	GLint slices,
	GLint loops);
*/
	double M = 3;

	  // arrow
	  quadObj = gluNewQuadric ();
	  gluQuadricDrawStyle (quadObj, GLU_FILL);
	  gluQuadricNormals (quadObj, GLU_SMOOTH);
	  gluCylinder(quadObj, M*D, 0.0, 2*M*D, 32, 1);
	  gluDeleteQuadric(quadObj);

	  quadObj = gluNewQuadric ();
	  gluQuadricDrawStyle (quadObj, GLU_FILL);
	  gluQuadricNormals (quadObj, GLU_SMOOTH);
	  gluDisk(quadObj, 0.0, M*D, 32, 1);
	  gluDeleteQuadric(quadObj);

	  glTranslatef(0,0,-L+2*M*D);

	  // shaft
	  quadObj = gluNewQuadric ();
	  gluQuadricDrawStyle (quadObj, GLU_FILL);
	  gluQuadricNormals (quadObj, GLU_SMOOTH);
	  gluCylinder(quadObj, D, D, L-2*M*D, 32, 1);
	  gluDeleteQuadric(quadObj);

	  quadObj = gluNewQuadric ();
	  gluQuadricDrawStyle (quadObj, GLU_FILL);
	  gluQuadricNormals (quadObj, GLU_SMOOTH);
	  gluDisk(quadObj, 0.0, D, 32, 1);
	  gluDeleteQuadric(quadObj);

	glPopMatrix ();
}

//---------------------------------------------------------------------------

