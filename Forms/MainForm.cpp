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
#include "MainForm.h"

#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TMain_Application_Window *Main_Application_Window;

//---------------------------------------------------------------------------
__fastcall TMain_Application_Window::TMain_Application_Window(TComponent* Owner)
	: TForm(Owner)
{
	STUDY = new STUDY_Class;

	OpenGL_Panel_1.STUDY = STUDY;

	// Set lowest priority
	SetPriorityClass(GetCurrentProcess(),IDLE_PRIORITY_CLASS);

	// Find application dierctory and store it
	Application_Directory = ExtractFilePath(ParamStr(0));
	Data_IO_Object.Application_Directory = Application_Directory;

	All_EGMs_PaintBox_Bitmap = new Graphics::TBitmap;
	Annotation_Window_PaintBox_Bitmap = new Graphics::TBitmap;
	ECG_Channels_Box_Bitmap = new Graphics::TBitmap;
	Extra_Channels_Box_Bitmap = new Graphics::TBitmap;
	Fluoro_Image_Bimap = new Graphics::TBitmap;

	Segments_Info.FilePath = Application_Directory+"Segments_Codes.txt";
	Segments_Info.load_object();

	read_labels_list("Labels_List.txt");

	read_app_version();

	OpenGL_Panel_1.Segmentation_Painting_Happened_Flag = false;

	Application_Closing_Flag = false;

	Echo = true;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::FormCreate(TObject *Sender)
{
	// to make sure decimal separator is always a dot
	Application->UpdateFormatSettings = false;
	FormatSettings.DecimalSeparator = '.';

	Segment_No_ComboBox->Items[0].Clear();
	for(long i=0;i<(signed)Segments_Info.Segments.size();i++)
	Segment_No_ComboBox->Items[0].Add(Segments_Info.Segments[i].Name);

	Segment_No_ComboBox->DropDownCount = Segments_Info.Segments.size()-1;

	System::Set8087CW(0x133f); // Disable all fpu exceptions.
    SetExceptionMask (exAllArithmeticExceptions);

	OpenGL_Panel_1.Segments_Info = &Segments_Info;

	OpenGL_Panel_1.OpenGL_Panel = Panel_1;
	OpenGL_Panel_1.initialize();

	OpenGL_Panel_1.set_NavX_AP_view();
	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Read_Palette(Application_Directory);

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::FormDestroy(TObject *Sender)
{
	OpenGL_Panel_1.delete_contexts();
	delete All_EGMs_PaintBox_Bitmap;
	delete Annotation_Window_PaintBox_Bitmap;
	delete ECG_Channels_Box_Bitmap;
	delete Extra_Channels_Box_Bitmap;
	delete Fluoro_Image_Bimap;

	delete STUDY;

	Application_Closing_Flag = true;
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::FormResize(TObject *Sender)
{
	if (!Application_Closing_Flag)
	{

	int W = Main_Application_Window->Width;
	int H = Main_Application_Window->Height - ToolBar1->Height - 4;
	int Spacing = 5;

	double page_ratio = 0.2;
	double opengl_panel_height_ratio = 0.6;

	//---------------------------
	// MAIN PAGE CONTROL
	//---------------------------

	Left_PageControl->Left = Spacing;
	Left_PageControl->Top = ToolBar1->Top + ToolBar1->Height + Spacing;
	Left_PageControl->Width = W*page_ratio-Spacing*3;
	Left_PageControl->Height = H - Left_PageControl->Top;

	Geometry_Selection_ComboBox->Left = 5;
	Geometry_Selection_ComboBox->Top = Label_p1->Top + Label_p1->Height + 5;
	// Geometry_Selection_ComboBox->Width = 0.75*Left_PageControl->Width;

	Display_Geo_CheckBox->Left = Geometry_Selection_ComboBox->Left +
								   Geometry_Selection_ComboBox->Width+4;
	Display_Geo_CheckBox->Top = Geometry_Selection_ComboBox->Top;


	Data_Points_List_StringGrid->Top = Displayed_Value_Selection_ComboBox_1->Top +
									   Displayed_Value_Selection_ComboBox_1->Height+10;
	Data_Points_List_StringGrid->Left = 5;
	Data_Points_List_StringGrid->Width = Left_PageControl->Width - 20;
	Data_Points_List_StringGrid->Height = 0.5*(Left_PageControl->Height -
							Displayed_Value_Selection_ComboBox_1->Top-50 );
	Data_Points_List_StringGrid->ColCount = 7;
	Data_Points_List_StringGrid->ColWidths[0]= (double)Data_Points_List_StringGrid->Width/(double)(Data_Points_List_StringGrid->ColCount)-3;
	Data_Points_List_StringGrid->ColWidths[1]= Data_Points_List_StringGrid->ColWidths[0];
	Data_Points_List_StringGrid->ColWidths[2]= Data_Points_List_StringGrid->ColWidths[0];
	Data_Points_List_StringGrid->ColWidths[3]= Data_Points_List_StringGrid->ColWidths[0];
	Data_Points_List_StringGrid->ColWidths[4]= Data_Points_List_StringGrid->ColWidths[0];
	Data_Points_List_StringGrid->ColWidths[5]= Data_Points_List_StringGrid->ColWidths[0];
	Data_Points_List_StringGrid->ColWidths[6]= Data_Points_List_StringGrid->ColWidths[0];

	Deleted_Data_Points_Label->Top = Data_Points_List_StringGrid->Top +
											   Data_Points_List_StringGrid->Height + 10;

	Deleted_Data_Points_List_StringGrid->Top = Deleted_Data_Points_Label->Top +
											   Deleted_Data_Points_Label->Height + 10;
	Deleted_Data_Points_List_StringGrid->Left = Data_Points_List_StringGrid->Left;
	Deleted_Data_Points_List_StringGrid->Width = Data_Points_List_StringGrid->Width;
	Deleted_Data_Points_List_StringGrid->Height = Left_PageControl->Height -
												  Deleted_Data_Points_Label->Top -
												  Deleted_Data_Points_Label->Height - 60;
	Deleted_Data_Points_List_StringGrid->ColCount = 7;
	Deleted_Data_Points_List_StringGrid->ColWidths[0]= (double)Deleted_Data_Points_List_StringGrid->Width/(double)(Deleted_Data_Points_List_StringGrid->ColCount)-3;
	Deleted_Data_Points_List_StringGrid->ColWidths[1]= Deleted_Data_Points_List_StringGrid->ColWidths[0];
	Deleted_Data_Points_List_StringGrid->ColWidths[2]= Deleted_Data_Points_List_StringGrid->ColWidths[0];
	Deleted_Data_Points_List_StringGrid->ColWidths[3]= Deleted_Data_Points_List_StringGrid->ColWidths[0];
	Deleted_Data_Points_List_StringGrid->ColWidths[4]= Deleted_Data_Points_List_StringGrid->ColWidths[0];
	Deleted_Data_Points_List_StringGrid->ColWidths[5]= Deleted_Data_Points_List_StringGrid->ColWidths[0];
	Deleted_Data_Points_List_StringGrid->ColWidths[6]= Deleted_Data_Points_List_StringGrid->ColWidths[0];

	Panel_1->Left = Left_PageControl->Left + Left_PageControl->Width + Spacing;
	Panel_1->Top = ToolBar1->Height;
	Panel_1->Width = 0.5*(W - W*page_ratio-5*Spacing);
	Panel_1->Height = H*opengl_panel_height_ratio;

	Zoom_Label->Left = Left_PageControl->Width - Zoom_Label->Width - 20;
	Zoom_Label->Top = 10;
	Zoom_Slider_Image->Left = Zoom_Label->Left;
	Zoom_Slider_Image->Top = Zoom_Label->Top + Zoom_Label->Height + 10;

	Right_PageControl->Left = Panel_1->Left + Panel_1->Width + Spacing;
	Right_PageControl->Top = Panel_1->Top;
	Right_PageControl->Width = W - Left_PageControl->Width - Panel_1->Width - Spacing;
	Right_PageControl->Height = Panel_1->Height;

	All_EGMs_PaintBox->Left = 4;
	All_EGMs_PaintBox->Top = 4;
	All_EGMs_PaintBox->Width = Right_PageControl->Width - 40;
	All_EGMs_PaintBox->Height = Right_PageControl->Height - 50;
	All_EGMs_PaintBox_Bitmap->Width = All_EGMs_PaintBox->Width;
	All_EGMs_PaintBox_Bitmap->Height = All_EGMs_PaintBox->Height;

	ECG_Channels_PaintBox->Left = 4;
	ECG_Channels_PaintBox->Top = 4;
	ECG_Channels_PaintBox->Width = Right_PageControl->Width - 40;
	ECG_Channels_PaintBox->Height = Right_PageControl->Height - 50;
	ECG_Channels_Box_Bitmap->Width = All_EGMs_PaintBox->Width;
	ECG_Channels_Box_Bitmap->Height = All_EGMs_PaintBox->Height;

	Extra_Channels_PaintBox->Left = 4;
	Extra_Channels_PaintBox->Top = 4;
	Extra_Channels_PaintBox->Width = Right_PageControl->Width - 40;
	Extra_Channels_PaintBox->Height = Right_PageControl->Height - 50;
	Extra_Channels_Box_Bitmap->Width = All_EGMs_PaintBox->Width;
	Extra_Channels_Box_Bitmap->Height = All_EGMs_PaintBox->Height;

	Images_StringGrid->Left = 4;
	Images_StringGrid->Top = Limit_Image_Display_Top_CheckBox->Top +
							 Limit_Image_Display_Top_CheckBox->Height + 5;
	Images_StringGrid->Width = Right_PageControl->Width - 50;
	Images_StringGrid->Height = 0.5*Right_PageControl->Height-Images_StringGrid->Top;

	Fluoro_Image_Paint_Box->Left = 4;
	Fluoro_Image_Paint_Box->Top = Images_StringGrid->Top + Images_StringGrid->Height + 4;
	Fluoro_Image_Paint_Box->Width = std::min((double)(Right_PageControl->Height-
													  Images_StringGrid->Top-
													  Images_StringGrid->Height-5),
											 (double)Right_PageControl->Width - 8);
	Fluoro_Image_Paint_Box->Height = Fluoro_Image_Paint_Box->Width;

	Fluoro_Image_Bimap->Width = Fluoro_Image_Paint_Box->Width;
	Fluoro_Image_Bimap->Height = Fluoro_Image_Paint_Box->Height;

	Annotation_Window_PaintBox->Left = Panel_1->Left;
	Annotation_Window_PaintBox->Top = Panel_1->Top + Panel_1->Height + 10;
	Annotation_Window_PaintBox->Width = Panel_1->Width + Right_PageControl->Width - 20;
	Annotation_Window_PaintBox->Height = Left_PageControl->Height - Panel_1->Height - Signals_ScrollBar->Height - 35;
	Annotation_Window_PaintBox_Bitmap->Width = Annotation_Window_PaintBox->Width;
	Annotation_Window_PaintBox_Bitmap->Height = Annotation_Window_PaintBox->Height;

	Annotation_Popup_Button->Left = Annotation_Window_PaintBox->Left;
	Annotation_Popup_Button->Top = Annotation_Window_PaintBox->Top;

	Signals_ScrollBar->Left = Annotation_Window_PaintBox->Left;
	Signals_ScrollBar->Top = Annotation_Window_PaintBox->Top + Annotation_Window_PaintBox->Height + 5;
	Signals_ScrollBar->Width = Annotation_Window_PaintBox->Width - 8;

	OpenGL_Panel_1.opengl_panel_resized();

	//-----------------------------------------------------------------
	// set position of various dialog forms/boxes
	//-----------------------------------------------------------------

	Progress_Form->Left = 0.5*W - 0.5*Progress_Form->Width;
	Progress_Form->Top = 100;

	double marg_v = 0.2;
	double marg_h = 0.1;
	StringGrid_Form->Left = Main_Application_Window->Left + W*marg_h;
	StringGrid_Form->Top =  Main_Application_Window->Top +  H*marg_v;
	StringGrid_Form->Width = W*(1-2*marg_h);
	StringGrid_Form->Height = H*(1-2*marg_v);

	Ask_For_Single_Item_Form1->Left = Progress_Form->Left;
	Ask_For_Single_Item_Form1->Top = Progress_Form->Top;

	AskFor_StartStopStep_Form->Left = Progress_Form->Left;
	AskFor_StartStopStep_Form->Top = Progress_Form->Top;

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Exit1Click(TObject *Sender)
{
	TModalResult Res = MessageDlg("Application is closing. Save current study?",
	mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

	if( Res == mrYes)
	Saveworkspace1Click(this);

	StringGrid_Form->Close();
	StringGrid_Form->Hide();

	Application_Closing_Flag = true;

	if( Res != mrCancel )
	   Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Panel_1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	map_display_panel_mouse_down(Button,Shift,X,Y,&OpenGL_Panel_1);
}
//---------------------------------------------------------------------------

void TMain_Application_Window::map_display_panel_mouse_down(TMouseButton Button, TShiftState Shift,
		int X, int Y,OpenGL_Panel_Class* Current_OpenGL_Panel)
{
	if( STUDY->is_current_surface_in_range() )
	{
	OpenGL_Panel_1.MouseButton = Button;
	OpenGL_Panel_1.MouseStatus = MB_DOWN;

	// for arcball rotation
	OpenGL_Panel_1.MousePt.s.X = X;
	OpenGL_Panel_1.MousePt.s.Y = Y;

	STUDY->Previous_Current_Surface = STUDY->Current_Surface;
	STUDY->Surfaces_List[STUDY->Current_Surface].Previous_Current_Data_Point_Ptr =
			STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	OpenGL_Panel_1.MouseFeedBack(X, Y,Paint_Segments_CheckBox->State);

	// if data point clicked
	if( OpenGL_Panel_1.closest_dp_clicked >= 0 )
	{
		OpenGL_Panel_1.prepare_colors_for_display();

		if(STUDY->Current_Surface != STUDY->Previous_Current_Surface )
		{
			update_controls_state();
		}
		else
		{
		Data_Points_List_StringGrid->Row =
			STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr+1;
		repaint_all_controls();
		}

	}
	else
	//------------------------------------
	// --- ARC BALL ROTATION SECTION
	//----------------------------------------
//	if( STUDY->is_current_surface_in_range() )
//	if(is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_ROTATE) )
	if( OpenGL_Panel_1.MouseButton == mbLeft &&
		!OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag )
	{
		OpenGL_Panel_1.LastRot = OpenGL_Panel_1.ThisRot;
		OpenGL_Panel_1.ArcBall.click(&OpenGL_Panel_1.MousePt);
	}

	}
}
//---------------------------------------------------------------------------

void TMain_Application_Window::update_images_table()
{
	if( STUDY->is_current_surface_in_range() )
	{

	// columns: Id Label
	Images_StringGrid->EditorMode = true; // ??????????????

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size() > 0 )
	Images_StringGrid->RowCount = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size()+1;
	Images_StringGrid->ColCount = 5;

	Images_StringGrid->Cells[0][0] = "Id";
	Images_StringGrid->Cells[1][0] = "Filename";
	Images_StringGrid->Cells[2][0] = "Width[mm]";
	Images_StringGrid->Cells[3][0] = "Height[mm]";
	Images_StringGrid->Cells[4][0] = "Thickness[mm]";

	for(int k=0;k<Images_StringGrid->ColCount;k++)
	Images_StringGrid->ColWidths[k] = Images_StringGrid->Width/Images_StringGrid->ColCount-5;

	for(int i=0;i<STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size();i++)
	{

	Images_StringGrid->Cells[0][1+i] = IntToStr(STUDY->Surfaces_List[STUDY->Current_Surface].
		Images_Set.Images_2D_List[i].Identificator);

	Images_StringGrid->Cells[1][1+i] =
		Utils.get_string_after_last_occurence_of_specified_string(
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[i].File_Name,"\\");

	Images_StringGrid->Cells[2][1+i] = FloatToStrF(
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Fluoro_Pixel_Size_In_mm *
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[i].Fluoro_Image_Width ,ffGeneral,3,2);

	Images_StringGrid->Cells[3][1+i] = FloatToStrF(
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Fluoro_Pixel_Size_In_mm *
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[i].Fluoro_Image_Height ,ffGeneral,3,2);

	Images_StringGrid->Cells[4][1+i] =	FloatToStrF(STUDY->Surfaces_List[STUDY->Current_Surface].
		Images_Set.Images_2D_List[i].Slice_Thickness,ffGeneral,3,2);

	} // for

	}
}

//---------------------------------------------------------------------------

void TMain_Application_Window::update_controls_state()
{
	int row;

	if( STUDY->is_current_surface_in_range() )
	{

	//--------------------------------------------
	// update imported images table
	//--------------------------------------------
	update_images_table();

	//--------------------------------------------
	// update geometry display status
	//--------------------------------------------
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Display_Whole_Dataset_Flag == true )
	Display_Geo_CheckBox->Checked = cbChecked;
	else
	Display_Geo_CheckBox->Checked = cbUnchecked;

	//--------------------------------------------
	// go to last data point set in case there is nothing selected
	//--------------------------------------------
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr < 0
		&& STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size() > 0 )
	STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr =
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size()-1;

	//--------------------------------------------
	// go to last data point set in case dset is out of range
	//--------------------------------------------
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr >=
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size() )
	STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr =
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size()-1;

	//--------------------------------------------
	// FILLING SURFACE SELECTION COMBOBOX
	//--------------------------------------------
	Geometry_Selection_ComboBox->Clear();
//	Surfaces_CheckListBox->Clear();
	for(long i=0;i<STUDY->Surfaces_List.size();i++)
	{
	Geometry_Selection_ComboBox->AddItem(STUDY->Surfaces_List[i].Name,NULL);
//	Surfaces_CheckListBox->AddItem(STUDY->Surfaces_List[i].Name,NULL);
	}

	Geometry_Selection_ComboBox->ItemIndex = STUDY->Current_Surface;
//	Surfaces_CheckListBox->ItemIndex = STUDY->Current_Surface;

	//--------------------------------------------
	// FILLING DATA POINT SETS SELECTION COMBOBOX
	//--------------------------------------------
	Data_Point_Set_Selection_ComboBox->Clear();
	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size();i++)
	Data_Point_Set_Selection_ComboBox->AddItem(STUDY->
		Surfaces_List[STUDY->Current_Surface].Data_Point_Set[i].Name,NULL);

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	Data_Point_Set_Selection_ComboBox->ItemIndex = DS;

	fill_data_points_table(NOT_DELETED_DATA_POINTS);
	fill_data_points_table(DELETED_DATA_POINTS);

/*
	//------------------------------------------------------------------
	// Paths
	//------------------------------------------------------------------
	double CV;

	// go to 1st path in case there is nothing selected ???? why its here?
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() == ON )
	{

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Current_Path < 0 &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Paths.size() > 0 )
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Current_Path = 0;

	Paths_StringGrid->RowCount = 2;
	Paths_StringGrid->ColCount = 3;
	Paths_StringGrid->FixedRows = 1;

	Paths_StringGrid->Cells[0][0] = "Distance[mm]";
	Paths_StringGrid->Cells[1][0] = "LAT diff[ms]";
	Paths_StringGrid->Cells[2][0] = "CV[m/s]";

	int size = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Paths.size();
	Paths_StringGrid->RowCount = max(2,size+1);

	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Paths.size();i++)
	{

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Paths[i].LAT_Difference != 0 )
	CV = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Paths[i].Distance/
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Paths[i].LAT_Difference;
	else
	CV = -1000;

	Paths_StringGrid->Cells[0][i+1] =
		FloatToStrF(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Paths[i].Distance,ffGeneral,3,2 );
	Paths_StringGrid->Cells[1][i+1] =
		FloatToStrF(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Paths[i].LAT_Difference,ffGeneral,3,2 );
	Paths_StringGrid->Cells[2][i+1] =
		FloatToStrF(fabs(CV),ffGeneral,3,2 );
	}

	row = max(1,STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Current_Path+1);
	if( row < Paths_StringGrid->RowCount )
		Paths_StringGrid->Row = row;

*/

	// postprosessing
	row = max( (int)1, (int)STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr+1 );
	if( row < Data_Points_List_StringGrid->RowCount )
		Data_Points_List_StringGrid->Row = row;

	// GetDistanceLATdifference();

	Signals_ScrollBarChange(this);

	Displayed_Value_Selection_ComboBox_1->ItemIndex =
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_ptr();

	OpenGL_Panel_1.prepare_colors_for_display();

	} // if geo in range
}

//---------------------------------------------------------------------------

void TMain_Application_Window::fill_data_points_table(int Which_Data_Points)
{
	TStringGrid *Selected_Data_Points_List_StringGrid;
	vector<Data_Point> *Data_Points_List;
	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	double val;

	if( Which_Data_Points == NOT_DELETED_DATA_POINTS )
	{
	Selected_Data_Points_List_StringGrid = Data_Points_List_StringGrid;
	Data_Points_List = &STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Data_Points;
	}

	if( Which_Data_Points == DELETED_DATA_POINTS )
	{
	Selected_Data_Points_List_StringGrid = Deleted_Data_Points_List_StringGrid;
	Data_Points_List = &STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Deleted_Data_Points;
	}

	//--------------------------------------------
	// clear and fill headers of the string grid  (this is displayed when no data is loaded)
	//--------------------------------------------
	Selected_Data_Points_List_StringGrid->RowCount = 2;
	Selected_Data_Points_List_StringGrid->ColCount = 9;
	Selected_Data_Points_List_StringGrid->FixedRows = 1;
	Selected_Data_Points_List_StringGrid->ColWidths[0]= (double)Selected_Data_Points_List_StringGrid->Width/(double)(Selected_Data_Points_List_StringGrid->ColCount)-3;
	Selected_Data_Points_List_StringGrid->ColWidths[1]= Selected_Data_Points_List_StringGrid->ColWidths[0];
	Selected_Data_Points_List_StringGrid->ColWidths[2]= Selected_Data_Points_List_StringGrid->ColWidths[0];
	Selected_Data_Points_List_StringGrid->ColWidths[3]= Selected_Data_Points_List_StringGrid->ColWidths[0];
	Selected_Data_Points_List_StringGrid->ColWidths[4]= Selected_Data_Points_List_StringGrid->ColWidths[0];
	Selected_Data_Points_List_StringGrid->ColWidths[5]= Selected_Data_Points_List_StringGrid->ColWidths[0];
	Selected_Data_Points_List_StringGrid->ColWidths[6]= Selected_Data_Points_List_StringGrid->ColWidths[0];
	Selected_Data_Points_List_StringGrid->ColWidths[7]= Selected_Data_Points_List_StringGrid->ColWidths[0];

	Selected_Data_Points_List_StringGrid->Cells[0][0] = "Id";
	Selected_Data_Points_List_StringGrid->Cells[0][1] = "Type";
	Selected_Data_Points_List_StringGrid->Cells[1][1] = "";
	Selected_Data_Points_List_StringGrid->Cells[2][1] = "";
	Selected_Data_Points_List_StringGrid->Cells[3][1] = "";
	Selected_Data_Points_List_StringGrid->Cells[4][1] = "";
	Selected_Data_Points_List_StringGrid->Cells[5][1] = "";
	Selected_Data_Points_List_StringGrid->Cells[6][1] = "";
	Selected_Data_Points_List_StringGrid->Cells[7][1] = "";

	for(int k=0;k<Selected_Data_Points_List_StringGrid->ColCount;k++)
	Selected_Data_Points_List_StringGrid->ColWidths[k] =
			Selected_Data_Points_List_StringGrid->Width/
			Selected_Data_Points_List_StringGrid->ColCount-5;

	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	if( !STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	{
		Data_Point_Set_Selection_ComboBox->Enabled = false;
	}
	else // there are data points in current set
	{

	Data_Point_Set_Selection_ComboBox->Enabled = true;

	//--------------------------------------------
	// filling table with data points
	//--------------------------------------------

	int Number_of_displayed_values_in_columns = 8;

	int Values_Number = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_number();

	Selected_Data_Points_List_StringGrid->ColCount = std::max(Number_of_displayed_values_in_columns,Values_Number);

	//--------------------------------------------------
	// fill values selection combobox
	//--------------------------------------------------
	Displayed_Value_Selection_ComboBox_1->Items[0].Clear();
	for(int k=0;k<Values_Number;k++)
	Displayed_Value_Selection_ComboBox_1->Items[0].Add(
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(k) );
	Displayed_Value_Selection_ComboBox_1->DropDownCount = Values_Number;

	//--------------------------------------------------
	// set names of columns
	//--------------------------------------------------
	Selected_Data_Points_List_StringGrid->Cells[0][0] = "Id";
	Selected_Data_Points_List_StringGrid->Cells[1][0] = "Type";

	for(int k=0;k<Number_of_displayed_values_in_columns-2;k++)
	if( k < Values_Number )
	Selected_Data_Points_List_StringGrid->Cells[k+2][0] =
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(k);

	//--------------------------------------------------
	// fill rows with values
	//--------------------------------------------------
	Selected_Data_Points_List_StringGrid->RowCount = std::max((int)2,(int)Data_Points_List->size()+1);

	for(long i=0;i<(signed)Data_Points_List->size();i++)
	{

	Selected_Data_Points_List_StringGrid->Cells[0][i+1] = IntToStr( Data_Points_List[0][i].Identifier );
	Selected_Data_Points_List_StringGrid->Cells[1][i+1] = Data_Points_List[0][i].data_point_type(
		STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source);

	for(int k=0;k<Number_of_displayed_values_in_columns-1;k++)
	if( k < Values_Number )
	{

	val = Data_Points_List[0][i].get_value(
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(k),
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

	if( val != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	Selected_Data_Points_List_StringGrid->Cells[k+2][i+1] = FloatToStrF(val,ffGeneral,3,2);
	else
	Selected_Data_Points_List_StringGrid->Cells[k+2][i+1] = "";

	// use segment name as text, not Id
	if( STUDY->Surfaces_List[STUDY->Current_Surface].
		Map_Values.get_value_name_according_to_ptr(k) == SEGMENTATION_VALUE_NAME )
	{
		if( val > 0 )
		Selected_Data_Points_List_StringGrid->Cells[k+2][i+1] = Segments_Info.get_segment_name(val);
		else
		Selected_Data_Points_List_StringGrid->Cells[k+2][i+1] = "";
	}

	}

	}

	// in case set is empty
	if(Data_Points_List[0].size()==0)
	{
		Selected_Data_Points_List_StringGrid->Cells[0][1] = "";
		Selected_Data_Points_List_StringGrid->Cells[1][1] = "";
		Selected_Data_Points_List_StringGrid->Cells[2][1] = "";
		Selected_Data_Points_List_StringGrid->Cells[3][1] = "";
		Selected_Data_Points_List_StringGrid->Cells[4][1] = "";
		Selected_Data_Points_List_StringGrid->Cells[5][1] = "";
		Selected_Data_Points_List_StringGrid->Cells[6][1] = "";
		Selected_Data_Points_List_StringGrid->Cells[7][1] = "";
	}

	} // dpset in range
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Panel_1MouseUp(TObject *Sender,
	TMouseButton Button,TShiftState Shift, int X, int Y)
{
	OpenGL_Panel_1.MouseButton = Button;
	OpenGL_Panel_1.MouseStatus = MB_UP;
	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag = false;

	if( OpenGL_Panel_1.Segmentation_Painting_Happened_Flag )
	{
		OpenGL_Panel_1.Segmentation_Painting_Happened_Flag = false;
		STUDY->Surfaces_List[STUDY->Current_Surface].save_segmentation_state();
		STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(
			Segments_Info.Segments.size());
	}
	repaint_all_controls();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Panel_1MouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y)
{
	map_dislplay_panel_mouse_move(X,Y,&OpenGL_Panel_1);
}
//---------------------------------------------------------------------------

void TMain_Application_Window::map_dislplay_panel_mouse_move(int X, int Y,
			OpenGL_Panel_Class* Current_OpenGL_Panel)
{
	//------------------------------------
	// --- ARC BALL ROTATION SECTION
	//----------------------------------------
	if(is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_ROTATE) )
	if(OpenGL_Panel_1.MouseButton == mbLeft &&
	   OpenGL_Panel_1.MouseStatus == MB_DOWN )
	{

	if( !OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag )
	{
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag = true;

		OpenGL_Panel_1.LastRot = OpenGL_Panel_1.ThisRot;			// Set Last Static Rotation To Last Dynamic One
		OpenGL_Panel_1.ArcBall.click(&OpenGL_Panel_1.MousePt);	// Update Start Vector And Prepare For Dragging

	} // dragging == OFF

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag )
	{
		OpenGL_Panel_1.MousePt.s.X = X;
		OpenGL_Panel_1.MousePt.s.Y = Y;

		// Core of rotation
		OpenGL_Panel_1.rotate_opengl_panels_world();

		OpenGL_Panel_1.map_display_panels_paint();

	} // dragging

	} // arc ball rotation

	//------------------------------------
	// --- ZOOM
	//----------------------------------------
	if(is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_ZOOM) )
	if(OpenGL_Panel_1.MouseButton == mbLeft &&
	   OpenGL_Panel_1.MouseStatus == MB_DOWN )
	{
		if (X > OpenGL_Panel_1.MousePt.s.X )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor += (GLdouble)(X-OpenGL_Panel_1.MousePt.s.X) * (GLdouble) 1.0;
		if (X < OpenGL_Panel_1.MousePt.s.X )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor -= (GLdouble)(OpenGL_Panel_1.MousePt.s.X-X) * (GLdouble) 1.0;
		if (X > OpenGL_Panel_1.MousePt.s.Y )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor += (GLdouble)(Y-OpenGL_Panel_1.MousePt.s.Y) * (GLdouble) 1.0;
		if (X < OpenGL_Panel_1.MousePt.s.Y )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor -= (GLdouble)(OpenGL_Panel_1.MousePt.s.Y-Y) * (GLdouble) 1.0;

		OpenGL_Panel_1.MousePt.s.X = X;
		OpenGL_Panel_1.MousePt.s.Y = Y;

		OpenGL_Panel_1.map_display_panels_paint();

	} // zoom

	//------------------------------------
	// PAN
	//------------------------------------
	if(is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_PAN) )
	if (OpenGL_Panel_1.MouseButton == mbLeft &&
		OpenGL_Panel_1.MouseStatus == MB_DOWN )
	{
		if (X > OpenGL_Panel_1.MousePt.s.X )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Translation_X +=
				(X-OpenGL_Panel_1.MousePt.s.X)/8.0;
		if (X < OpenGL_Panel_1.MousePt.s.X )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Translation_X -=
				(OpenGL_Panel_1.MousePt.s.X-X)/8.0;
		if (Y > OpenGL_Panel_1.MousePt.s.Y )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Translation_Y -=
				(Y-OpenGL_Panel_1.MousePt.s.Y)/8.0;
		if (Y < OpenGL_Panel_1.MousePt.s.Y )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Translation_Y +=
				(OpenGL_Panel_1.MousePt.s.Y-Y)/8.0;

		OpenGL_Panel_1.MousePt.s.X = X;
		OpenGL_Panel_1.MousePt.s.Y = Y;

		OpenGL_Panel_1.map_display_panels_paint();

	} // pan

/*
	//--------------------------------------
	// "Encircling area" selected
	//--------------------------------------
	if( is_Map_Panel_PopupMenu_item_tag_checked(MAP_POPUP_ITEM_ENCIRCLING) )
	if (MouseButton == mbLeft && MouseStatus == MB_DOWN )
	{
		OpenGL_Panel_Display_Parameters.OpenGL_Panel_Dragging_Flag[OpenGL_Panels.Which_Panel_Pointed] = true;
		map_display_panels_paint();
	}
*/

	//-------------------------------------------------------------
	// Marking segments
	//-------------------------------------------------------------
	if(Paint_Segments_CheckBox->State == cbChecked )
	if(OpenGL_Panel_1.MouseButton == mbRight &&
	   OpenGL_Panel_1.MouseStatus ==  MB_DOWN)
	{

	OpenGL_Panel_1.MouseFeedBack(X,Y,Paint_Segments_CheckBox->State);

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{

	int Selected_Segment = get_current_segment_id();

	int tt = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Segment_Id;

	mark_segment(STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle,
		Selected_Segment,Block_Existing_Segments_CheckBox->State);

	if( Segmentation_Brush_Size_RadioGroup->ItemIndex > 0 )
	if( Selected_Segment <= 0 ||
		!Block_Existing_Segments_CheckBox->State ||
		Block_Existing_Segments_CheckBox->State &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Segment_Id <= 0 )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Segment_Id = 10000;

		for(int s=0;s<Segmentation_Brush_Size_RadioGroup->ItemIndex;s++)
		STUDY->Surfaces_List[STUDY->Current_Surface].
			segmentation_expand_segment_one_step(10000, Block_Existing_Segments_CheckBox->State );

	   for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();i++)
	   if( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[i].Segment_Id == 10000 )
	   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[i].Segment_Id = Selected_Segment;
	}

	OpenGL_Panel_1.Segmentation_Painting_Happened_Flag = true;
	repaint_3D_panels();

	}

	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Select1Click(TObject *Sender)
{
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_SELECT_MODE,cbChecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ROTATE,cbUnchecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ZOOM,cbUnchecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_PAN,cbUnchecked);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Rotate1Click(TObject *Sender)
{
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_SELECT_MODE,cbUnchecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ROTATE,cbChecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ZOOM,cbUnchecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_PAN,cbUnchecked);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Zoom1Click(TObject *Sender)
{
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_SELECT_MODE,cbUnchecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ROTATE,cbUnchecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ZOOM,cbChecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_PAN,cbUnchecked);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Pan1Click(TObject *Sender)
{
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_SELECT_MODE,cbUnchecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ROTATE,cbUnchecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_ZOOM,cbUnchecked);
	set_Map_Panel_PopupMenu_item_checked_state(MAP_POPUP_ITEM_PAN,cbChecked);
}
//---------------------------------------------------------------------------

bool TMain_Application_Window::is_Map_Panel_PopupMenu_item_tag_checked(int tag)
{
	TMenuItem *ItemToEdit;
	ItemToEdit = OpenGL_Panel_PopupMenu->Items->Items[0]->Items[tag];
	if(ItemToEdit->Checked == cbChecked)
	return true;
	else
	return false;
}

//---------------------------------------------------------------------------
	//-----------------------------------------------------------
	// Function sets state of the Map_Panel_PopupMenu item (as pointed by int tag)
	//-----------------------------------------------------------

void TMain_Application_Window::set_Map_Panel_PopupMenu_item_checked_state
		(int tag,bool checked)
{
	TMenuItem *ItemToEdit;
	ItemToEdit = OpenGL_Panel_PopupMenu->Items->Items[0]->Items[tag];
	ItemToEdit->Checked = checked;
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Displayoptions1Click(TObject *Sender)
{
	double ScalingFactor = 4;
	//-------------------------------------------------------------------
	// data points
	//-------------------------------------------------------------------
//	ShowMessage(FloatToStr(OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Data_Point_Size));

	Display_Options_Form_1->DTS_Edit->Text = FloatToStr(
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo);

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Arrows_Flag )
	Display_Options_Form_1->Display_Arrows_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Display_Arrows_CheckBox->State = cbUnchecked;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Pointing_Cross )
	Display_Options_Form_1->Pointing_Cross_Display_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Pointing_Cross_Display_CheckBox->State = cbUnchecked;

	Display_Options_Form_1->AT_Edit->Text = FloatToStr(
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Arrow_Thickness);

	Display_Options_Form_1->AS_Edit->Text = FloatToStr(
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Arrow_Lenght_Scaling_Factor);

	Display_Options_Form_1->PS_Edit->Text = FloatToStr(
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Arrow_Dist_From_Surface);


	Display_Options_Form_1->DP_Size_ComboBox->ItemIndex =
		ScalingFactor*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Data_Point_Size-1;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Font_Type == (int)ownGLUT_BITMAP_TIMES_ROMAN_10 )
		Display_Options_Form_1->Font_RadioGroup->ItemIndex = 0;
	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Font_Type == (int)ownGLUT_BITMAP_HELVETICA_12 )
		Display_Options_Form_1->Font_RadioGroup->ItemIndex = 1;
	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Font_Type == (int)ownGLUT_BITMAP_HELVETICA_18 )
		Display_Options_Form_1->Font_RadioGroup->ItemIndex = 2;
	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Font_Type == (int)ownGLUT_BITMAP_TIMES_ROMAN_24 )
		Display_Options_Form_1->Font_RadioGroup->ItemIndex = 3;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color == 0 )
	Display_Options_Form_1->Background_Color_RadioGroup->ItemIndex = 0;
	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color == 1 )
	Display_Options_Form_1->Background_Color_RadioGroup->ItemIndex = 1;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers )
	Display_Options_Form_1->Display_17_LV_markersCheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Display_17_LV_markersCheckBox->State = cbUnchecked;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Data_Points )
	Display_Options_Form_1->Draw_Data_Points_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Draw_Data_Points_CheckBox->State = cbUnchecked;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_DP_Name )
	Display_Options_Form_1->DP_Name_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->DP_Name_CheckBox->State = cbUnchecked;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Autoscale_Data_Point_Size )
	Display_Options_Form_1->Auto_DP_Size_Scaling_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Auto_DP_Size_Scaling_CheckBox->State = cbUnchecked;

	double ff = OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.DP_Size_Scaling_Ratio;

	Display_Options_Form_1->DPSize_Scaling_Factor_Edit->Text = IntToStr(
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.DP_Size_Scaling_Ratio);

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Geo_Nodes )
	Display_Options_Form_1->Display_Geo_Nodes_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Display_Geo_Nodes_CheckBox->State = cbUnchecked;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Geo_Edges )
	Display_Options_Form_1->Display_Edges_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Display_Edges_CheckBox->State = cbUnchecked;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_3D_Points_Cloud )
	Display_Options_Form_1->Display_3D_Point_Cloud_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Display_3D_Point_Cloud_CheckBox->State = cbUnchecked;


	//-------------------------------------------------------------------
	// ablation points
	//-------------------------------------------------------------------
	Display_Options_Form_1->AblPointSizeComboBox->ItemIndex =
		ScalingFactor*OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Abl_Point_Size;

	Display_Options_Form_1->Transparency_ScrollBar->Position =
	Display_Options_Form_1->Transparency_ScrollBar->Max -
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Transparency_Level*100;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Normals )
	Display_Options_Form_1->Display_Normals_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Display_Normals_CheckBox->State = cbUnchecked;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Ablation_Points )
		Display_Options_Form_1->DAP_CheckBox->State = cbChecked;
	else
		Display_Options_Form_1->DAP_CheckBox->State = cbUnchecked;

	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{
		if( STUDY->Surfaces_List[STUDY->Current_Surface].Display_Geometry_Flag )
		Display_Options_Form_1->Display_Triangles_CheckBox->State = cbChecked;
		else
		Display_Options_Form_1->Display_Triangles_CheckBox->State = cbUnchecked;

		// contours setup
		if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
				get_current_value_contours_display_flag() )
		Display_Options_Form_1->Isovalues_CheckBox->State = cbChecked;
		else
		Display_Options_Form_1->Isovalues_CheckBox->State = cbUnchecked;

		Display_Options_Form_1->Palette_Coarsening_LeveL_Edit->Text = FloatToStr(
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
				get_current_value_contours_interval() );

		Display_Options_Form_1->Line_Thickness_ComboBox->ItemIndex =
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Contour_Line_Thickness-1;

		// palette
		Display_Options_Form_1->Palette_Type_RadioGroup->ItemIndex =
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Palette_Type;

		// fixed palette support
		if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
				get_current_value_fixed_palette_flag() )
		Display_Options_Form_1->Fixed_Palette_Range_CheckBox->State = cbChecked;
		else
		Display_Options_Form_1->Fixed_Palette_Range_CheckBox->State = cbUnchecked;

		Display_Options_Form_1->Fixed_Palette_Range_Min_Edit->Text = FloatToStrF(STUDY->Min_Value_On_All_Maps,ffGeneral,3,2 );
		Display_Options_Form_1->Fixed_Palette_Range_Max_Edit->Text = FloatToStrF(STUDY->Max_Value_On_All_Maps,ffGeneral,3,2 );

		if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
				get_current_value_inverted_flag() )
		Display_Options_Form_1->Invert_Palette_CheckBox->State = cbChecked;
		else
		Display_Options_Form_1->Invert_Palette_CheckBox->State = cbUnchecked;
	}

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Paths )
	Display_Options_Form_1->Display_Paths_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->Display_Paths_CheckBox->State = cbUnchecked;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Paint_Registration_Points )
	Display_Options_Form_1->PaintRegPoints_CheckBox->State = cbChecked;
	else
	Display_Options_Form_1->PaintRegPoints_CheckBox->State = cbUnchecked;

	//------------------------------------------------------------------
	//------------------------------------------------------------------
	if( Display_Options_Form_1->ShowModal() )
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	{
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Text_Dist_From_Geo = Display_Options_Form_1->DTS_Edit->Text.ToDouble();

		if( Display_Options_Form_1->Display_Arrows_CheckBox->State == cbChecked )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Arrows_Flag = true;
		else
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Arrows_Flag = false;

		if( Display_Options_Form_1->Pointing_Cross_Display_CheckBox->State == cbChecked )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Pointing_Cross = true;
		else
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Pointing_Cross = false;

		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Arrow_Thickness = Display_Options_Form_1->AT_Edit->Text.ToDouble();
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Arrow_Lenght_Scaling_Factor = Display_Options_Form_1->AS_Edit->Text.ToDouble();
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Arrow_Dist_From_Surface = Display_Options_Form_1->PS_Edit->Text.ToDouble();

		if( Display_Options_Form_1->Font_RadioGroup->ItemIndex == 0 )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Font_Type = (int)ownGLUT_BITMAP_TIMES_ROMAN_10;

		if( Display_Options_Form_1->Font_RadioGroup->ItemIndex == 1 )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Font_Type = (int)ownGLUT_BITMAP_HELVETICA_12;

		if( Display_Options_Form_1->Font_RadioGroup->ItemIndex == 2 )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Font_Type = (int)ownGLUT_BITMAP_HELVETICA_18;

		if( Display_Options_Form_1->Font_RadioGroup->ItemIndex == 3 )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Font_Type = (int)ownGLUT_BITMAP_TIMES_ROMAN_24;


		if( Display_Options_Form_1->Display_Geo_Nodes_CheckBox->State == cbChecked )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Geo_Nodes = true;
		else
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Geo_Nodes = false;

		if( Display_Options_Form_1->Display_Edges_CheckBox->State == cbChecked )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Geo_Edges = true;
		else
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Geo_Edges = false;

		if( Display_Options_Form_1->Display_3D_Point_Cloud_CheckBox->State == cbChecked )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_3D_Points_Cloud = true;
		else
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_3D_Points_Cloud = false;

		if( Display_Options_Form_1->Display_17_LV_markersCheckBox->State == cbChecked )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers = true;
		else
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers = false;

		if( Display_Options_Form_1->Background_Color_RadioGroup->ItemIndex == 0 )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color = 0;
		if( Display_Options_Form_1->Background_Color_RadioGroup->ItemIndex == 1 )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Panel_3D_Background_Color = 1;

		//-------------------------------------------------------------------
		// data points
		//-------------------------------------------------------------------
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Data_Point_Size =
			(Display_Options_Form_1->DP_Size_ComboBox->ItemIndex+1)/ScalingFactor;

		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Data_Points =
			Display_Options_Form_1->Draw_Data_Points_CheckBox->State;

		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.DP_Size_Scaling_Ratio =
			Display_Options_Form_1->DPSize_Scaling_Factor_Edit->Text.ToInt();

		if( Display_Options_Form_1->Auto_DP_Size_Scaling_CheckBox->State == cbChecked )
		{
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Autoscale_Data_Point_Size = true;
			OpenGL_Panel_1.autoscale_data_point_size();
		}
		else
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Autoscale_Data_Point_Size = false;

		if( Display_Options_Form_1->DP_Name_CheckBox->State == cbChecked )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_DP_Name = true;
		else
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_DP_Name = false;


		//-------------------------------------------------------------------
		// ablation points
		//-------------------------------------------------------------------
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Ablation_Points = Display_Options_Form_1->DAP_CheckBox->State;

		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Abl_Point_Size =
			Display_Options_Form_1->AblPointSizeComboBox->ItemIndex/ScalingFactor;

		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Transparency_Level = (double)(
			Display_Options_Form_1->Transparency_ScrollBar->Max
			-Display_Options_Form_1->Transparency_ScrollBar->Position)/100.;

		if( STUDY->is_current_surface_in_range() )
		{

		if( Display_Options_Form_1->Display_Triangles_CheckBox->State == cbChecked )
		STUDY->Surfaces_List[STUDY->Current_Surface].Display_Geometry_Flag = true;
		else
		STUDY->Surfaces_List[STUDY->Current_Surface].Display_Geometry_Flag = false;

		}

		int Counter=0;

		if( Display_Options_Form_1->Display_Local_Signal_CheckBox->State == cbChecked )
		{ Annotation_Box.LC=true; Counter++; }
		else
		Annotation_Box.LC=false;

		if( Display_Options_Form_1->Display_Ref1_Signal_CheckBox->State == cbChecked )
		{ Annotation_Box.REF=true; Counter++; }
		else
		Annotation_Box.REF=false;

		if( Display_Options_Form_1->Display_ECG_Signal_CheckBox->State == cbChecked )
		{ Annotation_Box.ECG=true; Counter++; }
		else
		Annotation_Box.ECG=false;

/*		//
		if( Annotation_Box.LC && Annotation_Box.REF_1 && Annotation_Box.REF_2 && Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0.2; Annotation_Box.REF_1_Pos = 0.4; Annotation_Box.REF_2_Pos = 0.6; Annotation_Box.ECG_Pos = 0.8; }

		if( Annotation_Box.LC && Annotation_Box.REF_1 && Annotation_Box.REF_2 && !Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0.25; Annotation_Box.REF_1_Pos = 0.5; Annotation_Box.REF_2_Pos = 0.75; Annotation_Box.ECG_Pos = 0; }
		if( Annotation_Box.LC && Annotation_Box.REF_1 && !Annotation_Box.REF_2 && Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0.25; Annotation_Box.REF_1_Pos = 0.5; Annotation_Box.REF_2_Pos = 0; Annotation_Box.ECG_Pos = 0.75; }
		if( Annotation_Box.LC && !Annotation_Box.REF_1 && Annotation_Box.REF_2 && Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0.25; Annotation_Box.REF_1_Pos = 0; Annotation_Box.REF_2_Pos = 0.5; Annotation_Box.ECG_Pos = 0.75; }
		if( !Annotation_Box.LC && Annotation_Box.REF_1 && Annotation_Box.REF_2 && Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0; Annotation_Box.REF_1_Pos = 0.25; Annotation_Box.REF_2_Pos = 0.5; Annotation_Box.ECG_Pos = 0.75; }

		if( Annotation_Box.LC && Annotation_Box.REF_1 && !Annotation_Box.REF_2 && !Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0.33; Annotation_Box.REF_1_Pos = 0.66; Annotation_Box.REF_2_Pos = 0; Annotation_Box.ECG_Pos = 0; }
		if( Annotation_Box.LC && !Annotation_Box.REF_1 && Annotation_Box.REF_2 && !Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0.33; Annotation_Box.REF_1_Pos = 0.; Annotation_Box.REF_2_Pos = 0.66; Annotation_Box.ECG_Pos = 0; }
		if( Annotation_Box.LC && !Annotation_Box.REF_1 && !Annotation_Box.REF_2 && Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0.33; Annotation_Box.REF_1_Pos = 0; Annotation_Box.REF_2_Pos = 0; Annotation_Box.ECG_Pos = 0.66; }

		if( !Annotation_Box.LC && Annotation_Box.REF_1 && Annotation_Box.REF_2 && !Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0; Annotation_Box.REF_1_Pos = 0.33; Annotation_Box.REF_2_Pos = 0.66; Annotation_Box.ECG_Pos = 0; }
		if( !Annotation_Box.LC && Annotation_Box.REF_1 && !Annotation_Box.REF_2 && Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0; Annotation_Box.REF_1_Pos = 0.33; Annotation_Box.REF_2_Pos = 0; Annotation_Box.ECG_Pos = 0.66; }
		if( !Annotation_Box.LC && !Annotation_Box.REF_1 && Annotation_Box.REF_2 && Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0; Annotation_Box.REF_1_Pos = 0; Annotation_Box.REF_2_Pos = 0.33; Annotation_Box.ECG_Pos = 0.66; }

		if( !Annotation_Box.LC && !Annotation_Box.REF_1 && !Annotation_Box.REF_2 && Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0; Annotation_Box.REF_1_Pos = 0; Annotation_Box.REF_2_Pos = 0; Annotation_Box.ECG_Pos = 0.5; }
		if( !Annotation_Box.LC && !Annotation_Box.REF_1 && Annotation_Box.REF_2 && !Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0; Annotation_Box.REF_1_Pos = 0; Annotation_Box.REF_2_Pos = 0.5; Annotation_Box.ECG_Pos = 0; }
		if( !Annotation_Box.LC && Annotation_Box.REF_1 && Annotation_Box.REF_2 && !Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0; Annotation_Box.REF_1_Pos = 0.5; Annotation_Box.REF_2_Pos = 0; Annotation_Box.ECG_Pos = 0; }
		if( Annotation_Box.LC && !Annotation_Box.REF_1 && !Annotation_Box.REF_2 && !Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0.5; Annotation_Box.REF_1_Pos = 0; Annotation_Box.REF_2_Pos = 0; Annotation_Box.ECG_Pos = 0; }

		if( !Annotation_Box.LC && !Annotation_Box.REF_1 && !Annotation_Box.REF_2 && !Annotation_Box.ECG )
		{ Annotation_Box.LC_Pos = 0.0; Annotation_Box.REF_1_Pos = 0; Annotation_Box.REF_2_Pos = 0; Annotation_Box.ECG_Pos = 0; }
*/



		if( Annotation_Box.LC && !Annotation_Box.REF && !Annotation_Box.ECG )
		Annotation_Box.LC_Pos = 0.5;

		if( !Annotation_Box.LC && Annotation_Box.REF && !Annotation_Box.ECG)
		Annotation_Box.REF_Pos = 0.5;

		if( !Annotation_Box.LC && !Annotation_Box.REF && Annotation_Box.ECG)
		Annotation_Box.ECG_Pos = 0.5;

		if( Annotation_Box.LC && Annotation_Box.REF && !Annotation_Box.ECG)
		{  Annotation_Box.LC_Pos = 0.33;
			Annotation_Box.REF_Pos = 0.66; }

		if( Annotation_Box.LC && !Annotation_Box.REF && Annotation_Box.ECG)
		{  Annotation_Box.LC_Pos = 0.33;
		Annotation_Box.ECG_Pos = 0.66; }

		if( !Annotation_Box.LC && Annotation_Box.REF && Annotation_Box.ECG)
		{  Annotation_Box.REF_Pos = 0.33;
		Annotation_Box.ECG_Pos = 0.66; }

		if( Annotation_Box.LC && Annotation_Box.REF && Annotation_Box.ECG)
		{  Annotation_Box.LC_Pos = 0.2;
		Annotation_Box.REF_Pos = 0.5;
		Annotation_Box.ECG_Pos = 0.8; }

		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Normals =
			Display_Options_Form_1->Display_Normals_CheckBox->State;

		// palette
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Palette_Type =
		Display_Options_Form_1->Palette_Type_RadioGroup->ItemIndex;
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.set_palette();

	// fixed palette support
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{
		// contours setup
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
				set_current_value_contours_display_flag(
					Display_Options_Form_1->Isovalues_CheckBox->State);

		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
				set_current_value_contours_interval(
					Display_Options_Form_1->Palette_Coarsening_LeveL_Edit->Text.ToDouble() );

		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Contour_Line_Thickness =
			Display_Options_Form_1->Line_Thickness_ComboBox->ItemIndex + 1;


		// fixed palette support
		if( Display_Options_Form_1->Fixed_Palette_Range_CheckBox->State == cbChecked )
		{
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
				set_current_value_fixed_palette_flag(true);

		STUDY->Min_Value_On_All_Maps = Display_Options_Form_1->Fixed_Palette_Range_Min_Edit->Text.ToDouble();
		STUDY->Max_Value_On_All_Maps = Display_Options_Form_1->Fixed_Palette_Range_Max_Edit->Text.ToDouble();
		}
		else
		{
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.set_current_value_fixed_palette_flag(false);
		STUDY->compute_min_max_values();
		}

		if( Display_Options_Form_1->Invert_Palette_CheckBox->State == cbChecked )
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.set_current_value_inverted_flag(true);
		else
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.set_current_value_inverted_flag(false);
	}

	if( Display_Options_Form_1->Display_Paths_CheckBox->State == cbChecked )
	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Paths = true;
	else
	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Paths = false;

	if( Display_Options_Form_1->PaintRegPoints_CheckBox->State == cbChecked )
	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Paint_Registration_Points = true;
	else
	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Paint_Registration_Points = false;

	//*********
	OpenGL_Panel_1.prepare_colors_for_display();
	repaint_3D_panels();
	//*********

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::FormAfterMonitorDpiChanged(TObject *Sender,
		  int OldDPI, int NewDPI)
{
	FormResize(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::FormShow(TObject *Sender)
{
	Segment_No_ComboBox->ItemIndex = 1;

	if( OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Images_In_3D_Panel )
	Display_Images_In_3D_Panel_CheckBox->State = cbChecked;
	else
	Display_Images_In_3D_Panel_CheckBox->State = cbUnchecked;

	About_Window_Form1->Application_Directory = Application_Directory;

	App_Settings_Form->Application_Directory = Application_Directory;
	App_Settings_Form->initialize();

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::FormPaint(TObject *Sender)
{
	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void TMain_Application_Window::repaint_3D_panels()
{
	OpenGL_Panel_1.map_display_panels_paint();
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Loadstudy1Click(TObject *Sender)
{
	OpenDialog->Title = "Load workspace";
	OpenDialog->FilterIndex = 4;

	if( STUDY->Surfaces_List.size() > 0 )
	Closeworkspace1Click(this);

	if( OpenDialog->Execute() )
	{

	Progress_Form->clear();
	Progress_Form->add_text("File selected: " + OpenDialog->FileName);
	Progress_Form->Show();
	Application->ProcessMessages();

	Main_Application_Window->Cursor = crHourGlass;

	//-------------------------------------------------------------------
	int Result = STUDY->load_study(OpenDialog->FileName,Progress_Form);
	//-------------------------------------------------------------------

	if( Result > 0 )
	{
		Progress_Form->add_text("Initialization...");
		Progress_Form->Show();
		Application->ProcessMessages();

		post_study_load_processing();

		// set caption of main window
		Main_Application_Window->Caption = OpenDialog->FileName;

		Main_Application_Window->Cursor = crArrow;

		Progress_Form->add_text("Load complete. Press Close to continue.");
		Progress_Form->Show();
		Application->ProcessMessages();

		if(Echo)
		ShowMessage("Loading done");
	}
	else
	{
		Progress_Form->Hide();

		if(Result == -1)
		ShowMessage("Unable to open study file.");

		if(Result == -2)
		ShowMessage("Corrupted or not valid study file.");

		if(Result == -3)
		ShowMessage("Unsupported version of the workspace file (mainform).");
	}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Saveworkspace1Click(TObject *Sender)
{
	SaveDialog->Title = "Save workspace";
	SaveDialog->FilterIndex = 4;

	SaveDialog->FileName = STUDY->Study_Current_Path_And_File_Name;

	if( SaveDialog->Execute() )
	{

	Progress_Form->clear();
	Progress_Form->add_text("File name used: " + SaveDialog->FileName);
	Progress_Form->Show();
	Application->ProcessMessages();

	SaveDialog->FileName = Utils.add_file_extension(SaveDialog->FileName,".eplab");

	Main_Application_Window->Cursor = crHourGlass;

	//-------------------------------------------------------------------
	STUDY->save_study(SaveDialog->FileName,Progress_Form);
	//-------------------------------------------------------------------

	Progress_Form->add_text("File saved. Press CLOSE to continue." );
	Progress_Form->Show();
	Application->ProcessMessages();

	Main_Application_Window->Cursor = crArrow;

	Main_Application_Window->Caption = SaveDialog->FileName;

	}
}

//---------------------------------------------------------------------------

void TMain_Application_Window::post_study_load_processing()
{
	Progress_Form->add_text("Post study load processing...");
	Progress_Form->Show();
	Application->ProcessMessages();

	STUDY->post_study_load_processing();

	// set length of signal boxes so that all egm fits (for current geometry)
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{
		int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
		int dp = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

		Annotation_Box.Displayed_Segment_Length_ptr =
			STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			Data_Points[dp].Roving_Signal.Voltage_Values.size()+100;
		ECG_Channels_Box_Parameters.Displayed_Segment_Length_ptr =
			Annotation_Box.Displayed_Segment_Length_ptr;
		Extra_Channels_Box_Parameters.Displayed_Segment_Length_ptr =
			Annotation_Box.Displayed_Segment_Length_ptr;
	}

	Progress_Form->add_text("Finishing post study load processing...");
	Progress_Form->Show();
	Application->ProcessMessages();

	update_controls_state();
	update_images_table();

	OpenGL_Panel_1.autoscale_data_point_size();
	OpenGL_Panel_1.set_initial_zoom_in_3D_panel();
	OpenGL_Panel_1.prepare_colors_for_display();

	repaint_all_controls();
}
//---------------------------------------------------------------------------

void TMain_Application_Window::calculate_closest_nodes_and_data_points_pointers()
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	Progress_Form->add_text("Calculating closest nodes to data points..." );
	Progress_Form->Show();
	Application->ProcessMessages();

	STUDY->Surfaces_List[STUDY->Current_Surface].calculate_closest_nodes_and_data_points_ALL(dset);

	Progress_Form->add_text("Calculating data points neighbouroods..." );
	Progress_Form->Show();
	Application->ProcessMessages();

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].calculate_neighboring_data_points(
				STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm);

	STUDY->Surfaces_List[STUDY->Current_Surface].interpolate_all_values(0,dset,Progress_Form);

	OpenGL_Panel_1.prepare_colors_for_display();

	Progress_Form->Hide();

	}
}

//---------------------------------------------------------------------------
void __fastcall TMain_Application_Window::Geometry_Selection_ComboBoxChange(TObject *Sender)
{
	//***********************************************
	STUDY->Current_Surface = Geometry_Selection_ComboBox->ItemIndex;
	//***********************************************

	Data_Point_Set_Selection_ComboBoxChange(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Data_Point_Set_Selection_ComboBoxChange(TObject *Sender)
{
	//***********************************************
	save_state_of_GUI_controls();

	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() == OFF )
		STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr = 0;
//	reload_signals_from_data_points_to_whole_view();

//	update_geo_for_pentaray_display();
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	STUDY->Surfaces_List[STUDY->Current_Surface].calculate_closest_nodes_and_data_points_ALL(dset);

	STUDY->compute_min_max_values();

	update_controls_state();

	OpenGL_Panel_1.prepare_colors_for_display();

	repaint_3D_panels();

	Progress_Form->Hide();

	//***********************************************
}
//---------------------------------------------------------------------------

void TMain_Application_Window::save_state_of_GUI_controls()
{
	if( STUDY->is_current_surface_in_range() )
	{

	STUDY->Current_Surface = Geometry_Selection_ComboBox->ItemIndex;
//	STUDY->Current_Surface = Surfaces_CheckListBox->ItemIndex;

	STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr =
		Data_Point_Set_Selection_ComboBox->ItemIndex;

	STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr =
		Data_Points_List_StringGrid->Row - 1;

//	int Selected_col = Data_Points_List_StringGrid->Col;
//	ShowMessage(IntToStr(Selected_col));

	STUDY->Surfaces_List[STUDY->Current_Surface].Current_Deleted_Data_Point_Ptr =
		Deleted_Data_Points_List_StringGrid->Row - 1;

	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image =
		Images_StringGrid->Row-1;
	}
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Displayed_Value_Selection_ComboBox_1Change(TObject *Sender)

{
/*
	for(int S=0;S<STUDY->Surfaces_List.size();S++)
	{
		STUDY->Surfaces_List[S].Map_Values.Current_Map_Value_Name =
			Displayed_Value_Selection_ComboBox_1->Text;

		STUDY->compute_min_max_values();
	}
*/
	if( STUDY->is_current_surface_in_range() )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name =
			Displayed_Value_Selection_ComboBox_1->Text;
		STUDY->compute_min_max_values();
	}

	OpenGL_Panel_1.prepare_colors_for_display();
	repaint_3D_panels();

/*
	// to make sure to turn off segmentation mode if value changed (unless user would like to paint over e.g. bipolar map?)
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name !=
		SEGMENTATION_VALUE_NAME )
	Paint_Segments_CheckBox->State = cbUnchecked;
*/


}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Centergeometry1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].center_geometry_and_data_points();
		repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Deletepointswithoutxyzcoordinates1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	long D;

	Progress_Form->add_text("Removing not utilized or displayed data points... ");
	Progress_Form->Show();
	Application->ProcessMessages();

	D = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		delete_not_utlized_and_displayed_points();

	Progress_Form->add_text(IntToStr((int)D) + " data points removed...");
	Progress_Form->Show();
	Application->ProcessMessages();

	if( D > 0 )
	calculate_closest_nodes_and_data_points_pointers();

	Data_Point_Set_Selection_ComboBoxChange(this);
	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Display_Geo_CheckBoxClick(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{

	if( Display_Geo_CheckBox->State == cbChecked )
	STUDY->Surfaces_List[STUDY->Current_Surface].Display_Whole_Dataset_Flag = true;
	else
	STUDY->Surfaces_List[STUDY->Current_Surface].Display_Whole_Dataset_Flag = false;

	STUDY->compute_min_max_values();

	OpenGL_Panel_1.prepare_colors_for_display();

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	TModalResult Res = MessageDlg("Application is closing. Save current study?",
	mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

	if( Res == mrYes)
	Saveworkspace1Click(this);

	if( Res == mrCancel )
	   CanClose = false;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::All_EGMs_PaintBoxPaint(TObject *Sender)
{
	long ptr,ptr1,ptr2,i,j,DPPtr;
	double x1,y1,x2,y2,x3;
	int pos;
	double Vertical_Spacing;
	TRect T;

	// clear signals box
	All_EGMs_PaintBox_Bitmap->Canvas->Brush->Color = (TColor)RGB(230,230,230);
	All_EGMs_PaintBox_Bitmap->Canvas->FillRect(All_EGMs_PaintBox->ClientRect );

	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	long dpptr = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	vector <long> GroupList;

	for(long dp=0;dp<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points.size();dp++)
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Identifier ==
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dpptr].Identifier )
	GroupList.push_back(dp);

	double Start = All_Signals_Box_Parameters.Annotation_Display_Start;
	double Stop = Start + All_Signals_Box_Parameters.Displayed_Segment_Length_ptr;


	if( GroupList.size() < 50 ) // to avoid painting when it is too many (e.g. all have the same id)
	{

	//----------------------------------------------------------------
	// ELECTRICAL SILENCE PAINT
	//----------------------------------------------------------------
	All_EGMs_PaintBox_Bitmap->Canvas->Brush->Color=(TColor)( RGB(255,182,193) );
	DPPtr = GroupList[0]; // get 1st data poin from the group (all dps should have the same silence vector)
	for(long j=0;j<All_EGMs_PaintBox->Width;j++)
	{

	ptr1 = Start+(double)j/(double)All_EGMs_PaintBox->Width*(Stop-Start);
	ptr2 = Start+(double)(j+1)/(double)All_EGMs_PaintBox->Width*(Stop-Start);

	if( ptr1 >= 0 && ptr1 < STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[DPPtr].Roving_Signal.Silence_Vector.size() )
	{
		if(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			Data_Points[DPPtr].Roving_Signal.Silence_Vector[ptr1] == 0 )
		All_EGMs_PaintBox_Bitmap->Canvas->FillRect(Rect( j,0,j+1,All_EGMs_PaintBox_Bitmap->Height ) );
	}

	} // for, thoguth all pixels

	//------------------------------
	//------------------------------
	// SIGNALS PAINT
	//------------------------------
	//------------------------------
	double Bottom_Offset = 0; // BottomOffset_Edit->Text.ToDouble();
	long DPPtr;
	std::vector<double> *Data_Vec1;

	// current signals display
	for(int i=0;i<(signed)GroupList.size();i++)
	{

	DPPtr = GroupList[i];

	Data_Vec1 = &STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[DPPtr].Roving_Signal.Voltage_Values;

	Vertical_Spacing = (All_EGMs_PaintBox->Height-Bottom_Offset)*(double)(i+1)/(double)(GroupList.size()+1);
/*
	// LOCAL ACTIVATIONS
	All_EGMs_PaintBox_Bitmap->Canvas->Brush->Color =  (TColor)RGB(255,0,0); int Size=2;
	for(long box=0;box<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[DPPtr].Roving_Signal.Local_Activations.Local_Activations_vec.size();box++)
	{

		x1 = (STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			  Data_Points[DPPtr].Roving_Signal.Local_Activations.Local_Activations_vec[box].
				Timing_In_EGM_ptr - Start )
				 * (double)All_EGMs_PaintBox_Bitmap->Width/
				   (double)All_Signals_Box_Parameters.Displayed_Segment_Length_ptr;
		y1 = Vertical_Spacing;

		if( x1 > 0 && x1 < All_EGMs_PaintBox_Bitmap->Width )
		{
			T.Top = y1-15;
			T.Left = x1-1;
			T.Right = x1;
			T.Bottom = y1+15;
			All_EGMs_PaintBox_Bitmap->Canvas->FillRect(T);
		}
	}
*/

	// set line width
	if( DPPtr == STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr )
	All_EGMs_PaintBox_Bitmap->Canvas->Pen->Width = 2;
	else
	All_EGMs_PaintBox_Bitmap->Canvas->Pen->Width = 1;

	// set color
	All_EGMs_PaintBox_Bitmap->Canvas->Pen->Color = clBlack;
	All_EGMs_PaintBox_Bitmap->Canvas->Font->Color = clBlack;
	All_EGMs_PaintBox_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;

	for(long j=0;j<All_EGMs_PaintBox->Width;j++)
	{

	ptr1 = Start+(double)j/(double)All_EGMs_PaintBox->Width*(Stop-Start);
	ptr2 = Start+(double)(j+1)/(double)All_EGMs_PaintBox->Width*(Stop-Start);

	// SIGNAL CURVE PAINT
	All_EGMs_PaintBox_Bitmap->Canvas->Pen->Color = clBlack;
	if( ptr1 <(signed)Data_Vec1[0].size() &&  ptr2 < (signed)Data_Vec1[0].size() )
	{
	x1 = j;
	y1 = Vertical_Spacing-Data_Vec1[0][ptr1]*All_Signals_Box_Parameters.Signals_Amplitude_Amplify;

	x2 = j+1;
	y2 = Vertical_Spacing-Data_Vec1[0][ptr2]*All_Signals_Box_Parameters.Signals_Amplitude_Amplify;

	All_EGMs_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	All_EGMs_PaintBox_Bitmap->Canvas->LineTo( x2, y2 );
	}

	} // for all pixels


	//-------------------------------------
	// display name
	//-------------------------------------
	All_EGMs_PaintBox_Bitmap->Canvas->Brush->Color=(TColor)( RGB(200,200,200) );
	All_EGMs_PaintBox_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;
	All_EGMs_PaintBox_Bitmap->Canvas->TextFlags = ETO_OPAQUE;
	All_EGMs_PaintBox_Bitmap->Canvas->TextOutW(5,
		All_EGMs_PaintBox->Height*(double)(i+1)/(double)(GroupList.size()+1)-5,
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[DPPtr].Roving_Signal.Name);

/*
	//-----------------------------
	// % of CL occupied
	//-----------------------------
	double Block_size = 0.5*PercOcc_Block_Edit->Text.ToDouble()/
			Data_Items[0].Time_Step;
	All_EGMs_PaintBox_Bitmap->Canvas->Brush->Color=(TColor)( RGB(255,182,193) );

	if( MCP_CheckBox->State == cbChecked )
	for(long i=0;i<(signed)Data_Items.size();i++)
	for(long p=0;p<(signed)Data_Items[i].Local_Deflections_Positions.size();p++)
	{

	x1 = (Data_Items[i].Local_Deflections_Positions[p]-Block_size-Start)/(Stop-Start)*
			(double)All_EGMs_PaintBox->Width;
	x2 = (Data_Items[i].Local_Deflections_Positions[p]+Block_size-Start)/(Stop-Start)*
			(double)All_EGMs_PaintBox->Width;

	All_EGMs_PaintBox_Bitmap->Canvas->FillRect(
		Rect( x1,0,x2,All_EGMs_PaintBox->Height ) );

	} // for all peaks
*/
	//-------------------------------------
	// LOCAL DEFLECTIONS
	//-------------------------------------
/*
	int BS = 3;
	for(int act=0;act<Data_Items[i].Local_Deflections_Positions.size();act++)
	if( Data_Items[i].Local_Deflections_Positions[act] > 0 )
	if( Data_Items[i].Local_Deflections_Positions[act] < Data_Vec1.size() )
	{
		Vertical_Spacing = All_EGMs_PaintBox->Height*(double)(i+1)/(double)(Data_Items.size()+1);

		x1 = (Data_Items[i].Local_Deflections_Positions[act]-Start)/(Stop-Start)*
				(double)All_EGMs_PaintBox->Width;
//		y1 = Vertical_Spacing-Data_Vec1[Data_Items[i].Local_Deflections_Positions[act]]*Signals_Amplitude_Amplify;
		y1 = Vertical_Spacing;

		All_EGMs_PaintBox_Bitmap->Canvas->Brush->Color= clRed;
		All_EGMs_PaintBox_Bitmap->Canvas->FillRect( Rect( x1-BS,y1-BS,x1+BS,y1+BS) );
	}

	} // through signals
*/

/*

	//-------------------------------------
	// CONDUCTION RIPPLES
	//-------------------------------------
	int BS = MS_Edit->Text.ToDouble();

	for(long r=0;r<(signed)Conduction_Ripples.size();r++)
	for(long p=0;p<(signed)Conduction_Ripples[r].Activation_Ids.size();p++)
	{
	long DP = Conduction_Ripples[r].Data_Point_Ids[p];
	long act = Conduction_Ripples[r].Activation_Ids[p];

	if( DP >= 0 && DP < (signed)Data_Items.size() )
	if( act >= 0 && act < (signed)Data_Items[DP].Local_Deflections_Positions.size() )
	{

	Vertical_Spacing = All_EGMs_PaintBox->Height*(double)(DP+1)/(double)(Data_Items.size()+1);

	x1 = (Data_Items[DP].Local_Deflections_Positions[act]-Start)/(Stop-Start)*
			(double)All_EGMs_PaintBox->Width;
	y1 = Vertical_Spacing;

	// generate random color
	double R = 255*((int)(100*(long)r)%3) / 2.0;
	double G = 255*((int)(100*(long)r)%7) / 6.0;
	double B = 255*((int)(100*(long)r)%5) / 4.0;

	if( Black_Markers_CheckBox->State == cbChecked )
	{
		R = 10; G = 10; B = 10;
	}

	All_EGMs_PaintBox_Bitmap->Canvas->Brush->Color=(TColor)RGB(R,G,B);
	// All_EGMs_PaintBox_Bitmap->Canvas->Brush->Color= clRed;

	All_EGMs_PaintBox_Bitmap->Canvas->FillRect( Rect( x1-BS,y1-BS,x1+BS,y1+BS) );
	}
	}
*/

	} // thru all dps

	} // if group < 50

	} // if all in range

	All_EGMs_PaintBox_Bitmap->Canvas->Pen->Width = 1;
	All_EGMs_PaintBox->Canvas->Draw(0, 0, All_EGMs_PaintBox_Bitmap);

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::All_Signals_Display_Control_ButtonClick(TObject *Sender)
{
   All_Signal_Tracings_PopupMenu->Popup(Right_PageControl->Left,
	Right_PageControl->Top+50);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::AllSigs_Increase_ScaleClic(TObject *Sender)
{
	All_Signals_Box_Parameters.Displayed_Segment_Length_ptr *= 2;
	All_EGMs_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::AllSigsDecreaseScaleClick(TObject *Sender)
{
	All_Signals_Box_Parameters.Displayed_Segment_Length_ptr /= 2;
	All_EGMs_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::All_EGMs_PaintBoxMouseMove(TObject *Sender,
		  TShiftState Shift, int X, int Y)
{
	//----------------------------------------------------------
	// RIHGT BUTTON
	//----------------------------------------------------------
	if( All_Signals_Box_Parameters.MouseButton == mbRight &&
		All_Signals_Box_Parameters.MouseStatus ==  MB_DOWN)
	{
		if (Y > All_Signals_Box_Parameters.Mouse_Position_Y )
			All_Signals_Box_Parameters.Signals_Amplitude_Amplify *= 1.5;
		if (Y < All_Signals_Box_Parameters.Mouse_Position_Y )
			All_Signals_Box_Parameters.Signals_Amplitude_Amplify /= 1.5;

		All_EGMs_PaintBoxPaint(this);

	All_Signals_Box_Parameters.Mouse_Position_X = X;
	All_Signals_Box_Parameters.Mouse_Position_Y = Y;

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::All_EGMs_PaintBoxMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y)
{
   All_Signals_Box_Parameters.MouseButton = Button;
   All_Signals_Box_Parameters.MouseStatus = MB_DOWN;

   All_Signals_Box_Parameters.Mouse_Position_X = X;
   All_Signals_Box_Parameters.Mouse_Position_Y = Y;

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::All_EGMs_PaintBoxMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   All_Signals_Box_Parameters.MouseButton = Button;
   All_Signals_Box_Parameters.MouseStatus = MB_UP;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::About1Click(TObject *Sender)
{
	ShowMessage("EPLab Works. Version v.2.0.0. (c) Pawel Kuklik. /*.");
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Getstatsofcurrentmapvalue1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
   {

   AnsiString FileName = Utils.get_file_name_from_full_path(STUDY->Study_Current_Path_And_File_Name);
   int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
   double min,max,mean,SD,median,CovC;
   AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;


   StringGrid_Form->StringGrid1->ColCount = 12; // stats paramenters no + value name + filename + geo + dsetname
   StringGrid_Form->StringGrid1->RowCount =
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_number() - 1 + 1; // -1 because there is segmentation value, +1 for header

   StringGrid_Form->StringGrid1->Cells[0][0] = "FileName";
   StringGrid_Form->StringGrid1->Cells[1][0] = "Geometry";
   StringGrid_Form->StringGrid1->Cells[2][0] = "DataPointsSetName";
   StringGrid_Form->StringGrid1->Cells[3][0] = "DataPointsNumber";

   StringGrid_Form->StringGrid1->Cells[4][0] = "Value";
   StringGrid_Form->StringGrid1->Cells[5][0] = "Min";
   StringGrid_Form->StringGrid1->Cells[6][0] = "Max";
   StringGrid_Form->StringGrid1->Cells[7][0] = "Median";
   StringGrid_Form->StringGrid1->Cells[8][0] = "Mean";
   StringGrid_Form->StringGrid1->Cells[9][0] = "SD";
   StringGrid_Form->StringGrid1->Cells[10][0] = "CovC";

   StringGrid_Form->StringGrid1->Cells[11][0] = "Area[cm2]";

   int Counter=0;
   for(int val=0;val<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_number();val++)
   if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(val) != SEGMENTATION_VALUE_NAME )
   {

   STUDY->Surfaces_List[STUDY->Current_Surface].calculate_statistics_for_Values(
		0,val,&mean,&median,&min,&max,&SD,&CovC,dset,false);
//    				int Seg_No,int Val_Id,
//				double* av,double* median,double* min,double* max,double* sd,double* covc,
//				int DP_Set,bool Exclude_Zero_Values);
	double area = STUDY->Surfaces_List[STUDY->Current_Surface].get_total_area();
	long DPValid_No = STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[dset].get_valid_data_points_number(Current_Value_Name,
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref() );

   Counter++;
   StringGrid_Form->StringGrid1->Cells[0][Counter] = FileName;
   StringGrid_Form->StringGrid1->Cells[1][Counter] = STUDY->Surfaces_List[STUDY->Current_Surface].Name;
   StringGrid_Form->StringGrid1->Cells[2][Counter] = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Name;
   StringGrid_Form->StringGrid1->Cells[3][Counter] = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(val);
   StringGrid_Form->StringGrid1->Cells[4][Counter] = FloatToStrF(DPValid_No,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[5][Counter] = FloatToStrF(min,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[6][Counter] = FloatToStrF(max,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[7][Counter] = FloatToStrF(median,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[8][Counter] = FloatToStrF(mean,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[9][Counter] = FloatToStrF(SD,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[10][Counter] = FloatToStrF(CovC,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[11][Counter] = FloatToStrF(area,ffGeneral,3,2);

   }

   StringGrid_Form->ShowModal();

   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::GetstatsofcurrentmapvaluePERSEGMENT1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
   {

   AnsiString FileName = Utils.get_file_name_from_full_path(STUDY->Study_Current_Path_And_File_Name);
   int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
   double min,max,mean,SD,median,CovC;
   //AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;
   AnsiString Value_Name;

   int Segments_Present_Number = 1;

   StringGrid_Form->StringGrid1->ColCount = 13;

   StringGrid_Form->StringGrid1->Cells[0][0] = "FileName";
   StringGrid_Form->StringGrid1->Cells[1][0] = "Geometry";
   StringGrid_Form->StringGrid1->Cells[2][0] = "DataPointsSetName";
   StringGrid_Form->StringGrid1->Cells[3][0] = "SegmentName";
   StringGrid_Form->StringGrid1->Cells[4][0] = "DataPointsNumber";
   StringGrid_Form->StringGrid1->Cells[5][0] = "Value";

   StringGrid_Form->StringGrid1->Cells[6][0] = "Min";
   StringGrid_Form->StringGrid1->Cells[7][0] = "Max";
   StringGrid_Form->StringGrid1->Cells[8][0] = "Median";
   StringGrid_Form->StringGrid1->Cells[9][0] = "Mean";
   StringGrid_Form->StringGrid1->Cells[10][0] = "SD";
   StringGrid_Form->StringGrid1->Cells[11][0] = "CovC";

   StringGrid_Form->StringGrid1->Cells[12][0] = "Area[cm2]";

   int Counter=0;
   int Segment_Id;
   double area;
   long DPValid_No;

   for(int Seg_Ptr=1;Seg_Ptr<Segments_Info.Segments.size();Seg_Ptr++)
   for(int val=0;val<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
		get_values_number();val++)
   if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
		get_value_name_according_to_ptr(val) != SEGMENTATION_VALUE_NAME )
   {

   Segment_Id = Segments_Info.Segments[Seg_Ptr].Code;

   area = STUDY->Surfaces_List[STUDY->Current_Surface].get_area_of_given_segment_cm2(Segment_Id);

   Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.
						get_value_name_according_to_ptr(val);

   DPValid_No = STUDY->Surfaces_List[STUDY->Current_Surface].
		get_number_of_data_points_in_segment(dset,Segment_Id,Value_Name,
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref() );

   if( area > 0 )
   {

   STUDY->Surfaces_List[STUDY->Current_Surface].calculate_statistics_for_Values(
		Segment_Id,val,&mean,&median,&min,&max,&SD,&CovC,dset,false);

   Counter++;
   StringGrid_Form->StringGrid1->Cells[0][Counter] = FileName;
   StringGrid_Form->StringGrid1->Cells[1][Counter] = STUDY->Surfaces_List[STUDY->Current_Surface].Name;
   StringGrid_Form->StringGrid1->Cells[2][Counter] = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Name;
   StringGrid_Form->StringGrid1->Cells[3][Counter] = Segments_Info.Segments[Seg_Ptr].Name;
   StringGrid_Form->StringGrid1->Cells[4][Counter] = FloatToStrF(DPValid_No,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[5][Counter] = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(val);

   StringGrid_Form->StringGrid1->Cells[6][Counter] = FloatToStrF(min,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[7][Counter] = FloatToStrF(max,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[8][Counter] = FloatToStrF(median,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[9][Counter] = FloatToStrF(mean,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[10][Counter] = FloatToStrF(SD,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[11][Counter] = FloatToStrF(CovC,ffGeneral,3,2);

   StringGrid_Form->StringGrid1->Cells[12][Counter] = FloatToStrF(area,ffGeneral,3,2);

   }

   }

   StringGrid_Form->StringGrid1->RowCount = Counter+1;
   StringGrid_Form->ShowModal();

   }

}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ImportCARTOexportfolder1Click(TObject *Sender)
{
   bool Append = perform_pre_import_queries();
   bool Geoff_Export_Flag = false;

   ShowMessage("Select folder with *.mesh, *.car and electrogram files");

   if( SelectDirectory("Select data directory", "", Data_IO_Object.Data_Files_Path) )
   {
	Progress_Form->clear();
	Progress_Form->add_text("Folder selected: " + Data_IO_Object.Data_Files_Path);
	Progress_Form->add_text("Parsing file names...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Data_FileListBox->Clear();
	Data_FileListBox->ApplyFilePath( Data_IO_Object.Data_Files_Path );

	if( Data_IO_Object.Data_Files_Path.Length() > 0 )
	{
		AnsiString Result = Data_IO_Object.
			import_CARTO_folder(Data_FileListBox,STUDY,Progress_Form,true,false,false,false,Append,Geoff_Export_Flag);

		if( Result == "Import completed" )
		{

		STUDY->Current_Surface = 0;

		Progress_Form->add_text("Initialization...");
		Application->ProcessMessages();
		Progress_Form->Show();
		post_import_initialization(STUDY->Surfaces_List.size()-1,STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm,true);

		update_controls_state();

		if( MessageDlg("Center map?",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
		if( STUDY->is_current_surface_in_range() )
			STUDY->Surfaces_List[STUDY->Current_Surface].center_geometry_and_data_points();

		OpenGL_Panel_1.set_initial_zoom_in_3D_panel();

		repaint_3D_panels();

		Progress_Form->add_text("Import completed. Press CLOSE to continue." );
		Application->ProcessMessages();
		Progress_Form->Show();

		}
		else
		{
			Progress_Form->Hide();
			ShowMessage(Result);
		}

   } // folder path size > 0
   else
   ShowMessage("Invalid folder selected");

   }
}
//---------------------------------------------------------------------------

bool TMain_Application_Window::perform_pre_import_queries()
{
	//--------------------------------
	// Ask whether to start new study or append to existing one
	//--------------------------------
	bool Append;

	if( STUDY->Surfaces_List.size() > 0 )
	{
	TModalResult Result_1 = MessageDlg("Start new study? (click No if you want to append a new map to existing study)",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);
	if( Result_1 == mrYes )
	{
		Append = false;
		close_workspace();
	}
	else
		Append = true;

	//--------------------------------
	// Ask whether to save current workspace
	//--------------------------------
	if( Append == false)
	{
		TModalResult Result_2 = MessageDlg("Save current study workspace?",
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

		if( Result_2 == mrYes)
		Saveworkspace1Click(this);
	}

	}
	else
	Append = false;

	return Append;
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Annotation_Window_PaintBoxPaint(TObject *Sender)
{
	// goghann

	// light gray background
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color = (TColor)RGB(230,230,230);
	Annotation_Window_PaintBox_Bitmap->Canvas->FillRect(Annotation_Window_PaintBox->ClientRect );

	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int pos;
	float x1,x2,y1,y2;
	long ptr1,ptr2;
	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;
	double Catheter_Sampling = STUDY->Surfaces_List[STUDY->Current_Surface].
			Data_Point_Set[DS].Data_Points[DP].Roving_Signal.Time_Step_ms;

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
			Data_Points[DP].Roving_Signal.Voltage_Values.size() > 0 )
	{

	// anotacja

	//-----------------------------
	// rov LAT detection ROI
	//-----------------------------
	int left, right;
	left = STUDY->Comp_Module.ROV_LAT_Annotation_Left_Edge_ms;
	right = STUDY->Comp_Module.ROV_LAT_Annotation_Right_Edge_ms;

	TRect T;
	T.Top = 0;
	pos =
		(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Ref_Signal_Activation_ptr
		- Annotation_Box.Annotation_Display_Start +
		left/Catheter_Sampling )
		*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;
	T.Left = pos;
	pos =
		(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Ref_Signal_Activation_ptr
		- Annotation_Box.Annotation_Display_Start +
		right/Catheter_Sampling )
		*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;
	T.Right = pos;
	T.Bottom = Annotation_Window_PaintBox_Bitmap->Height;
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color = (TColor)RGB(200,200,200);
	Annotation_Window_PaintBox_Bitmap->Canvas->FillRect(T);

	// go back to lighter background (for fonts and dashed lines)
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color = (TColor)RGB(230,230,230);


	//-------------------------------------------------
	// 1st: Rov catheter
	//-------------------------------------------------
	if( Annotation_Box.LC )
	{

	// isoelectric line
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Color =  (TColor)RGB(50,50,50);
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Width = 1;
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Style = psDot;

	for(ptr1=0;ptr1<Annotation_Box.Displayed_Segment_Length_ptr-1;ptr1=ptr1+3)
	{

	x1 =     ptr1*(double)Annotation_Window_PaintBox_Bitmap->Width/(double)Annotation_Box.Displayed_Segment_Length_ptr;
	x2 = (ptr1+1)*(double)Annotation_Window_PaintBox_Bitmap->Width/(double)Annotation_Box.Displayed_Segment_Length_ptr;
	y1 = Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height;

	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x2, y1 );

	}

	// signal
	Annotation_Window_PaintBox_Bitmap->Canvas->Font->Color = (TColor)RGB(150,40,40);
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Color =  (TColor)RGB(150,40,40);
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Width = 1;
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Style = psSolid;

	for(ptr1=0;ptr1<Annotation_Box.Displayed_Segment_Length_ptr-1;ptr1++)
	if(Annotation_Box.Annotation_Display_Start+ptr1+1<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.Voltage_Values.size()
		&& Annotation_Box.Annotation_Display_Start+ptr1 >= 0)
	{

	x1 =     ptr1*(double)Annotation_Window_PaintBox_Bitmap->Width/(double)Annotation_Box.Displayed_Segment_Length_ptr;
	x2 = (ptr1+1)*(double)Annotation_Window_PaintBox_Bitmap->Width/(double)Annotation_Box.Displayed_Segment_Length_ptr;

	y1 = Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set
		[DS].
		Data_Points[STUDY->Surfaces_List[STUDY->Current_Surface].
		Current_Data_Point_Ptr].Roving_Signal.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1]
		*Annotation_Box.Local_Signal_Zoom;

	y2 = Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set
		[DS].
		Data_Points[DP].Roving_Signal.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1+1]
		*Annotation_Box.Local_Signal_Zoom;

	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x2, y2 );

	}

	int Size=3;
/*
	// LOCAL ACTIVATIONS
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color =  (TColor)RGB(0,0,0);
	for(long box=0;box<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.Local_Activations.Local_Activations_vec.size();box++)
	{
		x1 = (STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
			  Data_Points[DP].Roving_Signal.
			  Local_Activations.Local_Activations_vec[box].Timing_In_EGM_ptr - Annotation_Box.Annotation_Display_Start )
				 * (double)Annotation_Window_PaintBox_Bitmap->Width/
				   (double)Annotation_Box.Displayed_Segment_Length_ptr;
		if( x1 > 0 && x1 < Annotation_Window_PaintBox_Bitmap->Width )
		{
		T.Top = Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height-Size;
		T.Left = x1-Size;
		T.Right = x1+Size;
		T.Bottom = Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height+Size;
		Annotation_Window_PaintBox_Bitmap->Canvas->FillRect(T);
		}
	}
*/

	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Style = bsClear;

	// Rov channel LAT line
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Color = (TColor)RGB(250,40,90);
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Width = 1;
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Style = psDash;

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Rov_Signal_Activation_ptr != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	{
	pos =
		(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Rov_Signal_Activation_ptr
		- Annotation_Box.Annotation_Display_Start)
		*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;
	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( pos, 0 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( pos,Annotation_Window_PaintBox_Bitmap->Height );
	}

	// Ref channel LAT line
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Color = (TColor)RGB(0,210,0);
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Width = 1;
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Style = psDash;
	pos =
		(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Ref_Signal_Activation_ptr
		- Annotation_Box.Annotation_Display_Start)
		*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;
	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( pos, 0 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( pos,Annotation_Window_PaintBox_Bitmap->Height );

	// display name
	if(Annotation_Box.Display_Channels_Names)
	{
	Annotation_Window_PaintBox_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextFlags = ETO_OPAQUE;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextOutW(5,
		Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height+10,
		"Local: "+STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
			Data_Points[DP].Roving_Signal.Name);
	}

	// peak annotation boxes
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color =  (TColor)RGB(0,0,0);
	for(long p=0;p<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.vPeak_Positions.size();p++)
	{
		x1 = (STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.vPeak_Positions[p] - Annotation_Box.Annotation_Display_Start )
				 * (double)Annotation_Window_PaintBox_Bitmap->Width/
				   (double)Annotation_Box.Displayed_Segment_Length_ptr;
		if( x1 > 0 && x1 < Annotation_Window_PaintBox_Bitmap->Width )
		{
			T.Top = Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height-Size;
			T.Left = x1-Size;
			T.Right = x1+Size;
			T.Bottom = Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height+Size;
			Annotation_Window_PaintBox_Bitmap->Canvas->FillRect(T);
		}
	}

	// electrogram duration annotation boxes (rather rectangles)
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color =  (TColor)RGB(10,10,10);
	for(long p=0;p<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.vED_Positions_In_Signal.size();p++)
	{
		x1 = (STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.vED_Positions_In_Signal[p] - Annotation_Box.Annotation_Display_Start )
				 * (double)Annotation_Window_PaintBox_Bitmap->Width/
				   (double)Annotation_Box.Displayed_Segment_Length_ptr;
		if( x1 > 0 && x1 < Annotation_Window_PaintBox_Bitmap->Width )
		{
			T.Top = Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height-10;
			T.Left = x1-1;
			T.Right = x1+1;
			T.Bottom = Annotation_Box.LC_Pos*Annotation_Window_PaintBox_Bitmap->Height+10;

		Annotation_Window_PaintBox_Bitmap->Canvas->FillRect(T);
		}

	}

/*
	// LAVA duration annotation boxes (rather rectangles)
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color =  (TColor)RGB(180,80,10);
	for(long p=0;p<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.LAVA_StartStop_In_Signal.size();p++)
	{
		x1 = (STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.LAVA_StartStop_In_Signal[p] - Annotation_Box.Annotation_Display_Start )
				 * (double)Annotation_Window_PaintBox_Bitmap->Width/
				   (double)Displayed_Segment_Length_ptr;
		if( x1 > 0 && x1 < Annotation_Window_PaintBox_Bitmap->Width )
		{
			T.Top = LC_Pos*Annotation_Window_PaintBox_Bitmap->Height-10;
			T.Left = x1-1;
			T.Right = x1+1;
			T.Bottom = LC_Pos*Annotation_Window_PaintBox_Bitmap->Height+10;

		Annotation_Window_PaintBox_Bitmap->Canvas->FillRect(T);
		}

	}
*/
/*
	// LAVA marker
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.LAVA_Peak_Position > 0 )
	{
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color =  (TColor)RGB(20,20,170);
		x1 = (STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Roving_Signal.LAVA_Peak_Position - Annotation_Box.Annotation_Display_Start )
				 * (double)Annotation_Window_PaintBox_Bitmap->Width/
				   (double)Displayed_Segment_Length_ptr;
	if( x1 > 0 && x1 < Annotation_Window_PaintBox_Bitmap->Width )
	{
			T.Top = LC_Pos*Annotation_Window_PaintBox_Bitmap->Height-10;
			T.Left = x1-1;
			T.Right = x1+1;
			T.Bottom = LC_Pos*Annotation_Window_PaintBox_Bitmap->Height+10;
			Annotation_Window_PaintBox_Bitmap->Canvas->FillRect(T);
	}
	} // lava marker > 0
*/

	} // if rov is displayed

	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color = (TColor)RGB(230,230,230);

	//-------------------------------------------------
	// 2nd: Ref catheter 1
	//-------------------------------------------------
	if( Annotation_Box.REF  )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Reference_Signal.Voltage_Values.size() > 0 )
	{

	double shift_factor = 0.1;

	// set color
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Color = clGreen;
	Annotation_Window_PaintBox_Bitmap->Canvas->Font->Color = clGreen;
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Width = 1;
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Style = psSolid;

	// 1st reference signal
	for(ptr1=0;ptr1<Annotation_Box.Displayed_Segment_Length_ptr-1;ptr1++)
	if(Annotation_Box.Annotation_Display_Start+ptr1+1<
		(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
				Data_Points[DP].Reference_Signal.Voltage_Values.size()
		&& Annotation_Box.Annotation_Display_Start+ptr1 >= 0)
	{

	x1 =     ptr1*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;
	x2 = (ptr1+1)*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;

	y1 = (1-shift_factor)*Annotation_Box.REF_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Reference_Signal.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1]*Annotation_Box.Reference_Signal_Zoom;

	y2 = (1-shift_factor)*Annotation_Box.REF_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Reference_Signal.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1+1]*Annotation_Box.Reference_Signal_Zoom;

	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x2, y2 );

	}

	// display name
	if(Annotation_Box.Display_Channels_Names)
	{
	Annotation_Window_PaintBox_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextFlags = ETO_OPAQUE;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextOutW(5,
		(1-shift_factor)*Annotation_Box.REF_Pos*Annotation_Window_PaintBox_Bitmap->Height+10,
		"Ref1: "+STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
			Data_Points[DP].Reference_Signal.Name);
	}

	// 2nd reference signal
	for(ptr1=0;ptr1<Annotation_Box.Displayed_Segment_Length_ptr-1;ptr1++)
	if(Annotation_Box.Annotation_Display_Start+ptr1+1<
		(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Reference_Signal_2.Voltage_Values.size()
		&& Annotation_Box.Annotation_Display_Start+ptr1 >= 0)
	{

	x1 =     ptr1*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;
	x2 = (ptr1+1)*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;

	y1 = (1+shift_factor)*Annotation_Box.REF_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Reference_Signal_2.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1]*Annotation_Box.Reference_Signal_Zoom;

	y2 = (1+shift_factor)*Annotation_Box.REF_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Reference_Signal_2.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1+1]*Annotation_Box.Reference_Signal_Zoom;

	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x2, y2 );

	}

	// display name
	if(Annotation_Box.Display_Channels_Names)
	{
	Annotation_Window_PaintBox_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextFlags = ETO_OPAQUE;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextOutW(5,
		(1+shift_factor)*Annotation_Box.REF_Pos*Annotation_Window_PaintBox_Bitmap->Height+10,
		"Ref2: "+STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
			Data_Points[DP].Reference_Signal_2.Name);
	}

	} // ref display


	//-------------------------------------------------
	// 4th: Normalized voltage or ECG channel
	//-------------------------------------------------
	if( Annotation_Box.ECG  )
	{

	// set color
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Color = clGreen;
	Annotation_Window_PaintBox_Bitmap->Canvas->Font->Color = clGreen;
/*
	if( Movie_Mode_CheckBox->State == cbChecked )
	{

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
			Data_Points[DP].Normalized_Voltage.Voltage_Values.size() > 0 )
	{

	// dislpay normalized voltage signal
	for(ptr1=0;ptr1<Annotation_Box.Displayed_Segment_Length_ptr-1;ptr1++)
	if(Annotation_Box.Annotation_Display_Start+ptr1+1 < (signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[STUDY->Surfaces_List[STUDY->Current_Surface].
		Current_Data_Point_Set_Ptr].Data_Points[DP].
		Normalized_Voltage.Voltage_Values.size() &&
		Annotation_Box.Annotation_Display_Start+ptr1 >= 0)
	{

	x1 =     ptr1*Annotation_Window_PaintBox_Bitmap->Width/Displayed_Segment_Length_ptr;
	x2 = (ptr1+1)*Annotation_Window_PaintBox_Bitmap->Width/Displayed_Segment_Length_ptr;

	y1 = ECG_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Normalized_Voltage.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1]*ECG_Signal_Zoom;

	y2 = ECG_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].Normalized_Voltage.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1+1]*ECG_Signal_Zoom;

	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x2, y2 );

	}

	// Movie slider line
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Color = (TColor)RGB(150,140,0);
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Width = 2;
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Style = psDash;
	pos = (Movie_Slider_Activation_ptr
		- Annotation_Box.Annotation_Display_Start)
		*Annotation_Window_PaintBox_Bitmap->Width/Displayed_Segment_Length_ptr;
	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( pos, 0 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( pos,Annotation_Window_PaintBox_Bitmap->Height );

	// display name
	if(Display_Channels_Names)
	{
	Annotation_Window_PaintBox_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextFlags = ETO_OPAQUE;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextOutW(5,
		ECG_Pos*Annotation_Window_PaintBox_Bitmap->Height+10,"Normalized Rov signal");

	}

	}
	} // movie mode checked
	else
	{
*/
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
			Data_Points[DP].ECG_Signal.Voltage_Values.size() > 0 )
	for(ptr1=0;ptr1<Annotation_Box.Displayed_Segment_Length_ptr-1;ptr1++)
	if(Annotation_Box.Annotation_Display_Start+ptr1+1 <(signed)
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Current_Data_Point_Set_Ptr].Data_Points[DP].
		ECG_Signal.Voltage_Values.size() &&
		Annotation_Box.Annotation_Display_Start+ptr1 >= 0)
	{

	x1 =     ptr1*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;
	x2 = (ptr1+1)*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;

	y1 = Annotation_Box.ECG_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].ECG_Signal.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1]*Annotation_Box.ECG_Signal_Zoom;

	y2 = Annotation_Box.ECG_Pos*Annotation_Window_PaintBox_Bitmap->Height-
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].ECG_Signal.Voltage_Values
		[Annotation_Box.Annotation_Display_Start+ptr1+1]*Annotation_Box.ECG_Signal_Zoom;

	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x2, y2 );

	}

	// display name
	if(Annotation_Box.Display_Channels_Names)
	{
	Annotation_Window_PaintBox_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextFlags = ETO_OPAQUE;
	Annotation_Window_PaintBox_Bitmap->Canvas->TextOutW(5,
		Annotation_Box.ECG_Pos*Annotation_Window_PaintBox_Bitmap->Height+10,
		"ECG: "+STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
			Data_Points[DP].ECG_Signal.Name);
	}

	// peak annotation boxes
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color =  (TColor)RGB(80,80,0);
	TRect T;
	for(long p=0;p<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].ECG_Signal.vPeak_Positions.size();p++)
	{
		x1 = (STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].
		Data_Points[DP].ECG_Signal.vPeak_Positions[p] - Annotation_Box.Annotation_Display_Start )
				 * (double)Annotation_Window_PaintBox_Bitmap->Width/
				   (double)Annotation_Box.Displayed_Segment_Length_ptr;
		if( x1 > 0 && x1 < Annotation_Window_PaintBox_Bitmap->Width )

	T.Top = Annotation_Box.ECG_Pos*Annotation_Window_PaintBox_Bitmap->Height-3;
	T.Left = x1-3;
	T.Right = x1+3;
	T.Bottom = Annotation_Box.ECG_Pos*Annotation_Window_PaintBox_Bitmap->Height+3;

	Annotation_Window_PaintBox_Bitmap->Canvas->FillRect(T);
	}
	Annotation_Window_PaintBox_Bitmap->Canvas->Brush->Color = (TColor)RGB(230,230,230);


	// }

	} // ECG display

	//---------------------------------------------------------------
	// paint timescale
	//---------------------------------------------------------------
	y1 = Annotation_Window_PaintBox_Bitmap->Height;
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Color = clGreen;
	Annotation_Window_PaintBox_Bitmap->Canvas->Font->Color = clGreen;
	double Catheter_Sampling = STUDY->Surfaces_List[STUDY->Current_Surface].
			Data_Point_Set[DS].Data_Points[DP].Roving_Signal.Time_Step_ms;
	double CumulativeTime_minor=0,CumulativeTime_major=0,Current_Time;

	int Major_Tick_ms = std::pow( 10,
		(int)log10( Annotation_Box.Displayed_Segment_Length_ptr*Catheter_Sampling ))/10;
	int Minor_Tick_ms=Major_Tick_ms/10;

	for(ptr1=0;ptr1<Annotation_Box.Displayed_Segment_Length_ptr-1;ptr1++)
	if( Annotation_Box.Annotation_Display_Start+ptr1+1<
		(signed)STUDY->Surfaces_List[STUDY->Current_Surface].
				Data_Point_Set[DS].Data_Points[DP].Roving_Signal.Voltage_Values.size()
		&& Annotation_Box.Annotation_Display_Start+ptr1 >= 0)
	{

	x1 = (double)ptr1*(double)Annotation_Window_PaintBox_Bitmap->Width/
		 (double)Annotation_Box.Displayed_Segment_Length_ptr;

	CumulativeTime_minor += Catheter_Sampling;
	CumulativeTime_major += Catheter_Sampling;

	if( CumulativeTime_minor > Minor_Tick_ms )
	{
		Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Width = 1;
		y2 = Annotation_Window_PaintBox_Bitmap->Height-8;
		Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
		Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x1, y2 );
		CumulativeTime_minor -= Minor_Tick_ms;
	}

	if( CumulativeTime_major > Major_Tick_ms )
	{
		Current_Time = (int)(

			(double)((Annotation_Box.Annotation_Display_Start + ptr1 + 1)*Catheter_Sampling)
			/
			(double)Major_Tick_ms ) * Major_Tick_ms;

		Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Width = 2;
		y2 = Annotation_Window_PaintBox_Bitmap->Height-10;
		Annotation_Window_PaintBox_Bitmap->Canvas->TextOutW(x1,y2-20,IntToStr((int)Current_Time));
		Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
		Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x1, y2 );
		CumulativeTime_major -= Major_Tick_ms;
	}

	} // timescale painting


	//----------------------------------------
	// paint calippers
	//----------------------------------------
	Annotation_Window_PaintBox_Bitmap->Canvas->Pen->Color = clRed;
//	double V = Annotation_Box.Pixels_Per_mm*
//			   Annotation_Box.Time_Scale/1000.0;
	double V = (double)Annotation_Window_PaintBox_Bitmap->Width/(double)Annotation_Box.Displayed_Segment_Length_ptr;

	// Left edge
	for(int m=0;m<Annotation_Box.Left_Caliper_Edges.size();m++)
	if( Annotation_Box.Left_Caliper_Edges[m] > 0 && Annotation_Box.Right_Caliper_Edges[m] > 0 )
	{

	x1 = V*(Annotation_Box.Left_Caliper_Edges[m]-Annotation_Box.Annotation_Display_Start);

	y1 = 0;
	y2 = Annotation_Window_PaintBox_Bitmap->Height;

	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x1, y2 );

	Annotation_Window_PaintBox_Bitmap->Canvas->TextOutW(x1+2,
		y1+10,IntToStr((int)Annotation_Box.Left_Caliper_Edges[m])); // unit ???????????????
	}

	// Right edge
	for(int m=0;m<Annotation_Box.Left_Caliper_Edges.size();m++)
	if( Annotation_Box.Left_Caliper_Edges[m] > 0 && Annotation_Box.Right_Caliper_Edges[m] > 0 )
	{

	x1 = V*(Annotation_Box.Right_Caliper_Edges[m]-
		Annotation_Box.Annotation_Display_Start);

	y1 = 0;
	y2 = Annotation_Window_PaintBox_Bitmap->Height;

	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x1, y2 );

	Annotation_Window_PaintBox_Bitmap->Canvas->TextOutW(x1+2,
		y1+10,IntToStr((int)Annotation_Box.Right_Caliper_Edges[m]));
	}

	// Horizontal bar
	for(int m=0;m<Annotation_Box.Left_Caliper_Edges.size();m++)
	if( Annotation_Box.Left_Caliper_Edges[m] > 0 && Annotation_Box.Right_Caliper_Edges[m] > 0 )
	{

	x1 = V*(Annotation_Box.Left_Caliper_Edges[m]-
		Annotation_Box.Annotation_Display_Start );

	x2 = V*(Annotation_Box.Right_Caliper_Edges[m]-
		Annotation_Box.Annotation_Display_Start );

	y1 = Annotation_Window_PaintBox_Bitmap->Height-10;

	Annotation_Window_PaintBox_Bitmap->Canvas->MoveTo( x1, y1 );
	Annotation_Window_PaintBox_Bitmap->Canvas->LineTo( x2, y1 );

	Annotation_Window_PaintBox_Bitmap->Canvas->TextOutW(0.5*(x1+x2),
		y1-10,IntToStr((int)(Annotation_Box.Right_Caliper_Edges[m]-Annotation_Box.Left_Caliper_Edges[m])));

	} // horizontal bar paint

	} // if signal is not empty

	} // ptrs are ok

	Annotation_Window_PaintBox->Canvas->Draw(0, 0,Annotation_Window_PaintBox_Bitmap);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Annotation_Window_PaintBoxMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int pos;
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int cdp = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

   Annotation_Box.MouseButton = Button;
   Annotation_Box.MouseStatus = MB_DOWN;

   Annotation_Box.Mouse_Position_X = X;
   Annotation_Box.Mouse_Position_Y = Y;

	if( Annotation_Box.MouseButton == mbLeft &&
		Annotation_Box.MouseStatus ==  MB_DOWN)
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() == ON )
	{

	if(Annotation_Box.Displayed_Segment_Length_ptr!=0)
	{

	// Reference cath
	pos =
		(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[cdp].Ref_Signal_Activation_ptr
		- Annotation_Box.Annotation_Display_Start)
		*Annotation_Window_PaintBox_Bitmap->Width/
		 Annotation_Box.Displayed_Segment_Length_ptr;
	if(X > pos-5 && X < pos+5 )
		Annotation_Box.Reference_Annotation_Dragged = true;

	// Roving cath
	pos =
		(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[cdp].Rov_Signal_Activation_ptr
		- Annotation_Box.Annotation_Display_Start)
		*Annotation_Window_PaintBox_Bitmap->Width/
		Annotation_Box.Displayed_Segment_Length_ptr;
	if(X > pos-5 && X < pos+5 )
	{
		Annotation_Box.Local_Signal_Annotation_Dragged = true;

	double New_Ptr =
		Annotation_Box.Annotation_Display_Start +
		(Annotation_Box.Displayed_Segment_Length_ptr)*
		(double)(X)/(double)Annotation_Window_PaintBox_Bitmap->Width;

	}
/*
	// Movie slider
	pos =
		(Movie_Slider_Activation_ptr - Annotation_Display_Start)
		*Annotation_Window_PaintBox_Bitmap->Width/Annotation_Box.Displayed_Segment_Length_ptr;
	if(X > pos-5 && X < pos+5 )
	{
		Movie_Playing = true;
		Movie_Mode_CheckBox->State = cbChecked;
		Movie_Mode_CheckBoxClick(this);
		for(long m=0;m<4;m++)
			STUDY->Surfaces_List[m].Current_Map_Value = NORMALIZED_VOLTAGE_VALUE;

//		repaint_main_screen();

		Movie_Line_Dragged_Flag = ON;
	}
*/

	if(Annotation_Box.Reference_Annotation_Dragged &&
	   Annotation_Box.Local_Signal_Annotation_Dragged  )
	Annotation_Box.Reference_Annotation_Dragged = false;
/*
	if(Movie_Line_Dragged_Flag == ON && Local_Signal_Annotation_Dragged == ON )
		Local_Signal_Annotation_Dragged = OFF;

	if(Movie_Line_Dragged_Flag == ON && Reference_Annotation_Dragged == ON )
		Reference_Annotation_Dragged = OFF;
*/

	}

	}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Annotation_Window_PaintBoxMouseMove(TObject *Sender,
		  TShiftState Shift, int X, int Y)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int cdp = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	//-----------------------------------------------------
	// Setting cursor type for rov and ref lines
	//-----------------------------------------------------
	Annotation_Window_PaintBox->Cursor =(TCursor) 0; // default
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() == ON )
	{

	int pos;

	if(Annotation_Box.Displayed_Segment_Length_ptr!=0)
	{

	// ref vertical line
	pos =
		(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[cdp].Ref_Signal_Activation_ptr
		- Annotation_Box.Annotation_Display_Start)
		*Annotation_Window_PaintBox_Bitmap->Width/
		Annotation_Box.Displayed_Segment_Length_ptr;
	if(X > pos-5 && X < pos+5 )
		Annotation_Window_PaintBox->Cursor =(TCursor) -14;

	// rov vertical line
	pos =
		(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[cdp].Rov_Signal_Activation_ptr
		- Annotation_Box.Annotation_Display_Start)
		*Annotation_Window_PaintBox_Bitmap->Width/
		Annotation_Box.Displayed_Segment_Length_ptr;
	if(X > pos-5 && X < pos+5 )
		Annotation_Window_PaintBox->Cursor =(TCursor) -14;

	//-----------------------------------------------------
	// Setting cursor type for calipers
	//-----------------------------------------------------
	double V = (double)Annotation_Window_PaintBox_Bitmap->Width/(double)Annotation_Box.Displayed_Segment_Length_ptr;
//	double V = Annotation_Box.Pixels_Per_mm*
//		Annotation_Box.Time_Scale/1000.0;
	long Hitted_Timepoint_Ptr = (double)X/V;
	double dt = 4.0/V;

		// check if left marker is hitted
			for(int m=0;m<Annotation_Box.Left_Caliper_Edges.size();m++)
		if(Hitted_Timepoint_Ptr > Annotation_Box.Left_Caliper_Edges[m]-dt)
        if(Hitted_Timepoint_Ptr < Annotation_Box.Left_Caliper_Edges[m]+dt)
            Annotation_Window_PaintBox->Cursor = (TCursor)(-14);

        // check if right marker is hitted
        	for(int m=0;m<Annotation_Box.Left_Caliper_Edges.size();m++)
        if(Hitted_Timepoint_Ptr > Annotation_Box.Right_Caliper_Edges[m]-dt )
        if(Hitted_Timepoint_Ptr < Annotation_Box.Right_Caliper_Edges[m]+dt )
            Annotation_Window_PaintBox->Cursor = (TCursor)(-14);

        // whole calipper dragging
        if( X > 60 && Y < Annotation_Window_PaintBox->Height &&
                      Y > Annotation_Window_PaintBox->Height-30 )
        	for(int m=0;m<Annotation_Box.Left_Caliper_Edges.size();m++)
        if(Hitted_Timepoint_Ptr > Annotation_Box.Left_Caliper_Edges[m] )
        if(Hitted_Timepoint_Ptr < Annotation_Box.Right_Caliper_Edges[m] )
            Annotation_Window_PaintBox->Cursor = (TCursor)(-14);
/*
		// check if signal name is hitted
		for(int m=0; m<(signed)Annotation_Box.Signals.size();m++)
		if( X > 5 && X < 5+10*Annotation_Box.Signals[m].Catheter_Name.Length() )
		if( Y > Annotation_Box.Signals[m].Position_In_Signal_Box
				*Annotation_Window_PaintBox->Height-10 &&
			Y < Annotation_Box.Signals[m].Position_In_Signal_Box
				*Annotation_Window_PaintBox->Height+5 )
			Annotation_Window_PaintBox->Cursor = (TCursor)(-7);
*/
	// Deleting calipper
	if( X > 60 && Y < Annotation_Window_PaintBox->Height &&
				  Y > Annotation_Window_PaintBox->Height-30 )
    Annotation_Window_PaintBox->Cursor = (TCursor)(-14);

	}

	}
           // end of calipers so far

	//-----------------------------------------------------
	// RIHGT BUTTON: VERTICAL ZOOM
	//-----------------------------------------------------
	if( Annotation_Box.MouseButton == mbRight &&
		Annotation_Box.MouseStatus ==  MB_DOWN )
	{



	if( Annotation_Box.LC &&
		!Annotation_Box.REF &&
		!Annotation_Box.ECG )
	{
		if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Local_Signal_Zoom *= 1.5;
		if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Local_Signal_Zoom /= 1.5;
	}

	if( !Annotation_Box.LC &&
		Annotation_Box.REF &&
		!Annotation_Box.ECG )
	{
		if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Reference_Signal_Zoom *= 1.5;
		if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Reference_Signal_Zoom /= 1.5;
	}

	if( !Annotation_Box.LC &&
		!Annotation_Box.REF &&
		Annotation_Box.ECG )
	{
		if (Y > Annotation_Box.Mouse_Position_Y )
		Annotation_Box.ECG_Signal_Zoom *= 1.5;
		if (Y < Annotation_Box.Mouse_Position_Y )
		Annotation_Box.ECG_Signal_Zoom /= 1.5;
	}

	if( Annotation_Box.LC &&
		Annotation_Box.REF &&
		!Annotation_Box.ECG )
	{
		if( Y < Annotation_Window_PaintBox->Height*0.5)
		{
			if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Local_Signal_Zoom *= 1.5;
			if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Local_Signal_Zoom /= 1.5;
		}

		if( Y > Annotation_Window_PaintBox->Height*0.5)
		{
			if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Reference_Signal_Zoom *= 1.5;
			if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Reference_Signal_Zoom /= 1.5;
		}
	}

	if( Annotation_Box.LC &&
	   !Annotation_Box.REF &&
	   Annotation_Box.ECG )
	{
		if( Y < Annotation_Window_PaintBox->Height*0.5)
		{
			if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Local_Signal_Zoom *= 1.5;
			if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Local_Signal_Zoom /= 1.5;
		}

		if( Y > Annotation_Window_PaintBox->Height*0.5)
		{
			if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.ECG_Signal_Zoom *= 1.5;
			if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.ECG_Signal_Zoom /= 1.5;
		}
	}

	if( !Annotation_Box.LC &&
		Annotation_Box.REF &&
		Annotation_Box.ECG)
	{
		if( Y < Annotation_Window_PaintBox->Height*0.5)
		{
			if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Reference_Signal_Zoom *= 1.5;
			if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Reference_Signal_Zoom /= 1.5;
		}

		if( Y > Annotation_Window_PaintBox->Height*0.5)
		{
			if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.ECG_Signal_Zoom *= 1.5;
			if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.ECG_Signal_Zoom /= 1.5;
		}
	}

	if( Annotation_Box.LC &&
		Annotation_Box.REF &&
		Annotation_Box.ECG )
	{
		if( Y > Annotation_Window_PaintBox->Height*0.6)
		{
			if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.ECG_Signal_Zoom *= 1.5;
			if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.ECG_Signal_Zoom /= 1.5;
		}

		if( Y > Annotation_Window_PaintBox->Height*0.3)
		if( Y < Annotation_Window_PaintBox->Height*0.6)
		{
			if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Reference_Signal_Zoom *= 1.5;
			if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Reference_Signal_Zoom /= 1.5;
		}

		if( Y < Annotation_Window_PaintBox->Height*0.3)
		{
			if (Y > Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Local_Signal_Zoom *= 1.5;
			if (Y < Annotation_Box.Mouse_Position_Y )
			Annotation_Box.Local_Signal_Zoom /= 1.5;
		}
	}


	Annotation_Box.Mouse_Position_X = X;
	Annotation_Box.Mouse_Position_Y = Y;

	Annotation_Window_PaintBoxPaint(this);

	}

	//----------------------------------------------------
	// LEFT BUTTON: MOVING LAT LINES
	//-----------------------------------------------------
	if(Annotation_Box.MouseButton == mbLeft &&
	   Annotation_Box.MouseStatus ==  MB_DOWN)
	{

	// Dragging Ref LAT line
	if(Annotation_Box.Reference_Annotation_Dragged )
	{

	double New_Ptr = Annotation_Box.Annotation_Display_Start + (
	Annotation_Box.Displayed_Segment_Length_ptr)*
		(double)(X)/(double)Annotation_Window_PaintBox_Bitmap->Width;

	//------------------------------------
	// sliding ref in all DPs
	//------------------------------------
	if( !STUDY->Comp_Module.Individual_Reference_Channel_Annotation )
	if( New_Ptr >= 0 && New_Ptr < STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[dset].Data_Points[cdp].Roving_Signal.Voltage_Values.size() )
	for(long s=0;s<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points.size();s++)
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[s].Ref_Signal_Activation_ptr = New_Ptr;

	// find activation in rov channel (only if auto analysis TRUE )
	if( STUDY->Comp_Module.Deflection_Detection_Alg != ONSET_OF_DEFLECTION_ALG &&
		STUDY->Comp_Module.Deflection_Detection_Alg != OFFSET_OF_DEFLECTION_ALG ) // skip if onset/offset is calculated (too slow for live change)
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[s].find_roving_catheter_LAT_in_data_point(&STUDY->Comp_Module);
	}

	//------------------------------------
	// individual ref sliding
	//------------------------------------
	if( STUDY->Comp_Module.Individual_Reference_Channel_Annotation )
	if( New_Ptr >= 0 && New_Ptr < STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[dset].Data_Points[cdp].Roving_Signal.Voltage_Values.size() )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[cdp].Ref_Signal_Activation_ptr = New_Ptr;

	// find activation in rov channel (only if auto analysis TRUE )
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[cdp].find_roving_catheter_LAT_in_data_point(&STUDY->Comp_Module);
	}

	Annotation_Window_PaintBoxPaint(this);

	} // dragging Ref line

	//-----------------------------------------------------------------------
	// Dragging Rov LAT line
	//-----------------------------------------------------------------------
	if( Annotation_Box.Local_Signal_Annotation_Dragged )
	{

	double New_Ptr =
		 Annotation_Box.Annotation_Display_Start +
		(Annotation_Box.Displayed_Segment_Length_ptr)*
		(double)(X)/(double)Annotation_Window_PaintBox_Bitmap->Width;

	// Check whether pointer is within the signal
	if( New_Ptr >= 0 && New_Ptr < STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[dset].Data_Points[cdp].Roving_Signal.Voltage_Values.size() )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			Data_Points[cdp].Rov_Signal_Activation_ptr = New_Ptr;
	}

	Annotation_Window_PaintBoxPaint(this);

	}

	//-----------------------------------------------------------------------
	// Dragging Movie slider
	//-----------------------------------------------------------------------
/*
	if(Movie_Line_Dragged_Flag == ON)
	{

	double New_Ptr =
		Annotation_Display_Start + (Annotation_Box.Displayed_Segment_Length_ptr)*
		(double)(X)/(double)Annotation_Window_PaintBox_Bitmap->Width;

	// Check whether pointer is within the signal
	if( New_Ptr >= 0 && New_Ptr < STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[dset].
		Data_Points[cdp].Normalized_Voltage.Voltage_Values.size())
	for(long m=0;m<STUDY->Surfaces_List.size();m++)
	if( STUDY->Surfaces_List[m].Display_Flag )
	if( STUDY->Surfaces_List[m].data_points_set_ptr_in_range() == ON )
	if( STUDY->Surfaces_List[m].data_point_ptr_in_range() == ON )
	{

	int DS = STUDY->Surfaces_List[m].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[m].Current_Data_Point_Ptr;

	STUDY->Surfaces_List[m].Current_Map_Value =
			NORMALIZED_VOLTAGE_VALUE;
	STUDY->Surfaces_List[m].Current_Map_Value =
			ROVING_VOLTAGE_VALUE;

	Movie_Slider_Activation_ptr = New_Ptr;

	update_PS_detection();

	}

	map_display_panels_paint();
	Annotation_Window_PaintBoxPaint(this);
	Application->ProcessMessages();

	}
*/

	Annotation_Box.Mouse_Position_X = X;
	Annotation_Box.Mouse_Position_Y = Y;
	Annotation_Window_PaintBoxPaint(this);

	}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Annotation_Window_PaintBoxMouseUp(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int cdp = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	Annotation_Box.MouseButton = Button;
	Annotation_Box.MouseStatus = MB_UP;

	// update data point values if LAT modified
	if( Annotation_Box.Local_Signal_Annotation_Dragged  )
	{

		Progress_Form->add_text("Updating map...");
		Progress_Form->Show();
		Application->ProcessMessages();

		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			Data_Points[cdp].calculate_values_in_data_point(
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref(),&STUDY->Comp_Module);
	}

	// update all data point values if ref dragged and "affects all" flag is true
	if( Annotation_Box.Reference_Annotation_Dragged ) // &&
//		STUDY->Comp_Module.Reference_Reannotation_Affects_All_Channels_Flag )
	{

		Progress_Form->add_text("Updating map...");
		Progress_Form->Show();
		Application->ProcessMessages();

		update_LAT_annotation_of_current_map();
	}

	// update map
	if( Annotation_Box.Reference_Annotation_Dragged ||
		Annotation_Box.Local_Signal_Annotation_Dragged  )
	{
		update_display_of_LAT_Bip_maps();
	}

	Annotation_Box.Reference_Annotation_Dragged = false;
	Annotation_Box.Local_Signal_Annotation_Dragged = false;
	// Annotation_Box.Movie_Line_Dragged_Flag = false;

	Progress_Form->Hide();

	}
}

//---------------------------------------------------------------------------

void TMain_Application_Window::update_LAT_annotation_of_current_map()
{
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	for(long dp=0;dp<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points.size();dp++)
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[dp].find_roving_catheter_LAT_in_data_point(&STUDY->Comp_Module);

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[dp].calculate_values_in_data_point(
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref(),&STUDY->Comp_Module);
	}
}

//---------------------------------------------------------------------------

void TMain_Application_Window::update_display_of_LAT_Bip_maps()
{
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	STUDY->compute_min_max_values();

	STUDY->Surfaces_List[STUDY->Current_Surface].interpolate_LAT_value(0,dset,Progress_Form);

	STUDY->Surfaces_List[STUDY->Current_Surface].interpolate_Bipolar_amplitude_value(0,dset,Progress_Form);

	OpenGL_Panel_1.prepare_colors_for_display();

	update_controls_state();
	save_state_of_GUI_controls();

	Progress_Form->Hide();

	repaint_all_controls();
}

//---------------------------------------------------------------------------

void TMain_Application_Window::repaint_all_controls()
{
	// this always paint
	Annotation_Window_PaintBoxPaint(this);
	repaint_3D_panels();

	if( Right_PageControl->ActivePage->Name == "GroupedEGMs" )
		All_EGMs_PaintBoxPaint(this);

	if( Right_PageControl->ActivePage->Name == "ECGChannels" )
		ECG_Channels_PaintBoxPaint(this);

	if( Right_PageControl->ActivePage->Name == "ExtraChannels" )
		Extra_Channels_PaintBoxPaint(this);

	if( Right_PageControl->ActivePage->Name == "ImagesPage" )
		draw_fluoro_image_2D();
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Help2Click(TObject *Sender)
{
	Help_Form->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Data_Points_List_StringGridClick(TObject *Sender)

{
	//***********************************************
	save_state_of_GUI_controls();
	//***********************************************

	repaint_all_controls();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Downsamplemesh1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	long Old_T_Number = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();

	if( Old_T_Number > 0 )
	{

	ShowMessage("Current surface is composed of " + IntToStr( (int)Old_T_Number ) + " triangles. In next window, put desired number of triangles.");

	Ask_For_Single_Item_Form1->Caption = "Target number of surface triangles: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = IntToStr( (int)(0.5* Old_T_Number));

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{
		long New_T_Number = Ask_For_Single_Item_Form1->Edit_Box->Text.ToInt();

		double Ratio = (double)New_T_Number/(double)Old_T_Number;

		STUDY->Surfaces_List[STUDY->Current_Surface].downsample_mesh(
			Application_Directory,Ratio,Progress_Form);

		New_T_Number = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();

		calculate_closest_nodes_and_data_points_pointers();

		STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(Segments_Info.Segments.size());

		if(Echo)
		ShowMessage("Number of triangles dropped from: " +FloatToStr((double)Old_T_Number) + " to " + FloatToStr((double)New_T_Number) + ".");

		repaint_3D_panels();
	}
	}
	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Paint_Segments_CheckBoxClick(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{
	if(Paint_Segments_CheckBox->State == cbChecked)
	{
		Previously_Selected_Value_ptr = Displayed_Value_Selection_ComboBox_1->ItemIndex;

		Displayed_Value_Selection_ComboBox_1->ItemIndex =
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_ptr(SEGMENTATION_VALUE_NAME);

		for(int S=0;S<STUDY->Surfaces_List.size();S++)
		STUDY->Surfaces_List[S].Map_Values.set_current_value_according_to_name(SEGMENTATION_VALUE_NAME);

		Displayed_Value_Selection_ComboBox_1Change(this);

//		ShowMessage("Displayed map value changed to "+ (AnsiString)SEGMENTATION_VALUE_NAME + ". Paint by dragging mouse cursor with right button pressed.");

		OpenGL_Panel_PopupMenu->AutoPopup = false;
	}
	else
	{
		Displayed_Value_Selection_ComboBox_1->ItemIndex = Previously_Selected_Value_ptr;

		for(int S=0;S<STUDY->Surfaces_List.size();S++)
		STUDY->Surfaces_List[S].Map_Values.set_current_value_according_to_ptr(Previously_Selected_Value_ptr);

		Displayed_Value_Selection_ComboBox_1Change(this);

//		ShowMessage("Changed back to previously selected Displayed value");

		OpenGL_Panel_PopupMenu->AutoPopup = true;
	}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Clear_Segments_ButtonClick(TObject *Sender)
{
   int Seg_Id = get_current_segment_id();

   if( STUDY->is_current_surface_in_range() )
   {

   STUDY->Surfaces_List[STUDY->Current_Surface].save_segmentation_state();

   for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();i++)
   if( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[i].Segment_Id == Seg_Id )
	   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[i].Segment_Id = -1;

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(Segments_Info.Segments.size());
   }

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ClearAll_segs_ButtonClick(TObject *Sender)

{
   if( STUDY->is_current_surface_in_range() )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].save_segmentation_state();

	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();i++)
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[i].Segment_Id = -1;

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(Segments_Info.Segments.size());
	}

	repaint_3D_panels();

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Undo_Segmentation_Step_ButtonClick(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   {
	STUDY->Surfaces_List[STUDY->Current_Surface].restore_previous_segmentation_state();
	STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(Segments_Info.Segments.size());
	repaint_3D_panels();
   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Grow_Current_Segment_ButtonClick(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].save_segmentation_state();

		STUDY->Surfaces_List[STUDY->Current_Surface].
			segmentation_expand_segment_one_step( get_current_segment_id(),
				Block_Existing_Segments_CheckBox->State );
		STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(Segments_Info.Segments.size());
		repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void TMain_Application_Window::read_app_version()
{
	ifstream dfile((Application_Directory+"version.txt").c_str());

	if( dfile == NULL )
		ShowMessage("\n Error reading versioning file." );
	else
	{

	char string[2000];

	dfile >> string;

	EPLAB_App_Version = (AnsiString)string;

	dfile.close();
	}

}

//---------------------------------------------------------------------------

void TMain_Application_Window::read_labels_list(AnsiString FileName)
{
	ifstream dfile((Application_Directory+FileName).c_str());
	AnsiString AS;

	if( dfile == NULL )
	{
			ShowMessage("\n Error : can't open labels list file." );
	}
	else
	{

	char string[2000];

	dfile >> string;

	if( strcmp("Labels_Names_File_v_1",string) )
	ShowMessage("Labels file corrupted. Contact support.");
	else
	{

	Labels_Names.clear();

	while( !dfile.eof() )
	{
		dfile.getline(string,2000);
		AS = (AnsiString)string;
		if( AS.Length() > 0 )
		Labels_Names.push_back(AS);
	}

	}

	dfile.close();

	}
}
//---------------------------------------------------------------------------

void TMain_Application_Window::save_labels_list(AnsiString FileName)
{
	ofstream dfile((Application_Directory+FileName).c_str());
	AnsiString AS;

	if( dfile == NULL )
	{
			ShowMessage("\n Error : can't save updated labels list file." );
	}
	else
	{
		char string[2000];

		dfile << "Labels_Names_File_v_1" << endl;

		for(int i=0;i<Labels_Names.size();i++)
			dfile << Labels_Names[i].c_str() << endl;

		dfile.close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Sh1Click(TObject *Sender)
{
	NL_Analysis_Form->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::mark_segment(int Triangle_Ptr,int Area_Id,bool Locked_Flag)
{
	if( Triangle_Ptr >= 0 )
	{

	int i,n0,n1,n2,area_id;

	n0 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[Triangle_Ptr].Nodes[0];
	n1 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[Triangle_Ptr].Nodes[1];
	n2 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[Triangle_Ptr].Nodes[2];

	for(i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].Neig_Triangles.size();i++)
	if( Area_Id <= 0 || !Locked_Flag ||
		Locked_Flag &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set
			[STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].Neig_Triangles[i]].Segment_Id == -1)
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].Neig_Triangles[i]].Segment_Id = Area_Id;

	for(i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].Neig_Triangles.size();i++)
	if( Area_Id <= 0 || !Locked_Flag ||
		Locked_Flag &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set
			[STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].Neig_Triangles[i]].Segment_Id == -1)
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].Neig_Triangles[i]].Segment_Id = Area_Id;

	for(i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].Neig_Triangles.size();i++)
	if( Area_Id <= 0 || !Locked_Flag ||
		Locked_Flag &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set
			[STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].Neig_Triangles[i]].Segment_Id == -1)
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].Neig_Triangles[i]].Segment_Id = Area_Id;
	}
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Shrink_Current_Segment_ButtonClick(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   {
		STUDY->Surfaces_List[STUDY->Current_Surface].save_segmentation_state();

		STUDY->Surfaces_List[STUDY->Current_Surface].
			segmentation_shrink_segment_one_step( get_current_segment_id(),
				Block_Existing_Segments_CheckBox->State );
		STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(Segments_Info.Segments.size());
		repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Importvtkmeshfile1Click(TObject *Sender)
{
	OpenDialog->Title = "Select vtk file";
	OpenDialog->FilterIndex = 2;

	if( OpenDialog->Execute() )
	{

	Progress_Form->clear();
	Progress_Form->add_text("File selected: " + OpenDialog->FileName);
	Progress_Form->Show();
	Application->ProcessMessages();

	bool Downsample_Flag;
	if( MessageDlg("Downsample mesh?",
	mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	Downsample_Flag = true;
	else
	Downsample_Flag = false;

	Main_Application_Window->Cursor = crHourGlass;

	bool Append = true; // I don't think replacing will be an option ever...

	AnsiString Result = Data_IO_Object.import_vtk_file(OpenDialog->FileName,STUDY,Progress_Form,Append);

	if( Result == "Import completed" )
	{
		STUDY->Current_Surface = 0;

		Progress_Form->add_text("Initialization...");
		Progress_Form->Show();
		Application->ProcessMessages();
		post_import_initialization(STUDY->Surfaces_List.size()-1,STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm,Downsample_Flag);

		update_controls_state();

		STUDY->Surfaces_List[STUDY->Current_Surface].calculate_normals();

		// set caption of main window
		Main_Application_Window->Caption = OpenDialog->FileName;

		if( MessageDlg("Center map?",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
		if( STUDY->is_current_surface_in_range() )
			STUDY->Surfaces_List[STUDY->Current_Surface].center_geometry_and_data_points();

		Progress_Form->add_text("Import completed. Press CLOSE to continue." );
		Progress_Form->Show();
		Application->ProcessMessages();

		repaint_3D_panels();
	}
	else
	{
		Progress_Form->Hide();
		ShowMessage(Result);
	}

   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Applicationsettings1Click(TObject *Sender)

{
	App_Settings_Form->MaxGeoNodes_Number_Edit->Text =
		IntToStr((int)App_Settings_Form->Application_Settings.Max_Number_of_Geometry_Nodes);

	//------------------------------------------------------------------
	//------------------------------------------------------------------
	if( App_Settings_Form->ShowModal() )
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	{

	App_Settings_Form->Application_Settings.Max_Number_of_Geometry_Nodes =
		App_Settings_Form->MaxGeoNodes_Number_Edit->Text.ToInt();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Annotationsignalprocessinparameters1Click(TObject *Sender)
{
	int result;
	bool DP_update_required = false;

	Annotation_Parameters_Form->Left = 100;
	Annotation_Parameters_Form->Top = 100;

	// ANNOTATION PARAMETERS
	Annotation_Parameters_Form->Annotation_Algorithm_RadioGroup->ItemIndex =
		STUDY->Comp_Module.Deflection_Detection_Alg;

	Annotation_Parameters_Form->ROV_Left_Edge_Edit->Text =
		FloatToStr(STUDY->Comp_Module.ROV_LAT_Annotation_Left_Edge_ms);
	Annotation_Parameters_Form->ROV_Right_Edge_Edit->Text =
		FloatToStr(STUDY->Comp_Module.ROV_LAT_Annotation_Right_Edge_ms);

	double prev_Data_Points_Filling_Threshold_mm = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm;
	double prev_Max_Distance_Data_Points_Projection_mm = STUDY->Comp_Module.Max_Distance_Data_Points_Projection_mm;
	Annotation_Parameters_Form->DataPoints_space_Filling_Th_Edit->Text =
		FloatToStr(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm);
	Annotation_Parameters_Form->Max_Dist_Corr_Nodes_Edit->Text =
		FloatToStr(STUDY->Comp_Module.Max_Distance_Data_Points_Projection_mm);

	Annotation_Parameters_Form->MinVolt_LAT_Annotaiton_Edit ->Text =
		FloatToStr(STUDY->Comp_Module.Min_Voltage_Threshold_for_LAT_annotation_mV);

	if(STUDY->Comp_Module.Automatic_Annotation_Flag )
	Annotation_Parameters_Form->Automatic_Annotation_Checkbox->State = cbChecked;
	else
	Annotation_Parameters_Form->Automatic_Annotation_Checkbox->State = cbUnchecked;

	if( STUDY->Comp_Module.Individual_Reference_Channel_Annotation )
	Annotation_Parameters_Form->Individual_Ref_Annotation_CheckBox->State = cbChecked;
	else
	Annotation_Parameters_Form->Individual_Ref_Annotation_CheckBox->State = cbUnchecked;

	Annotation_Parameters_Form->EOS_SD_WL_Edit->Text = FloatToStr(STUDY->Comp_Module.Deflection_SD_WL_ms);
	Annotation_Parameters_Form->EOS_Th_Edit->Text = FloatToStr(STUDY->Comp_Module.Deflection_SD_Th);

	// SCAR SETTINGS
	Annotation_Parameters_Form->Scar_Threshold_Edit->Text =  FloatToStr(App_Settings_Form->Application_Settings.Scar_BZ_Voltage_Threshold);
	Annotation_Parameters_Form->BZ_Threshold_Edit->Text =  FloatToStr(App_Settings_Form->Application_Settings.BZ_Healthy_Voltage_Threshold);

	Annotation_Parameters_Form->Conduction_Block_Th_Edit->Text =  FloatToStr(App_Settings_Form->Application_Settings.Conduction_Block_CV_Threshold);
	Annotation_Parameters_Form->Slow_Conduction_Th_Edit->Text =   FloatToStr(App_Settings_Form->Application_Settings.Slow_Conduction_Threshold);

/*

	if( Push_Part_of_Signal )
	Annotation_Parameters_Form->ShE_Just_Portion_CheckBox->State = cbChecked;
	else
	Annotation_Parameters_Form->ShE_Just_Portion_CheckBox->State = cbUnchecked;

*/
/*
	if( NL_Analysis_Form->Signal_1.Percentage_Range_for_Shannon == true )
	Annotation_Parameters_Form->ShE_Just_Portion_CheckBox->State = cbChecked;
	else
	Annotation_Parameters_Form->ShE_Just_Portion_CheckBox->State = cbUnchecked;

	Annotation_Parameters_Form->ShEn_Begin_Edit->Text = IntToStr(NL_Analysis_Form->Signal_1.ShE_Begining);
	Annotation_Parameters_Form->ShEn_End_Edit->Text = IntToStr(NL_Analysis_Form->Signal_1.ShE_End);
*/
	//--------------------------------------------
	//--------------------------------------------
	result = Annotation_Parameters_Form->ShowModal();
	//--------------------------------------------
	//--------------------------------------------

	if(result == mrOk )
	{
		// ANNOTATION PARAMETERS
	STUDY->Comp_Module.Deflection_Detection_Alg = Annotation_Parameters_Form->Annotation_Algorithm_RadioGroup->ItemIndex;

	STUDY->Comp_Module.ROV_LAT_Annotation_Left_Edge_ms =
		Annotation_Parameters_Form->ROV_Left_Edge_Edit->Text.ToDouble();

	STUDY->Comp_Module.ROV_LAT_Annotation_Right_Edge_ms =
		Annotation_Parameters_Form->ROV_Right_Edge_Edit->Text.ToDouble();

	STUDY->Comp_Module.Min_Voltage_Threshold_for_LAT_annotation_mV =
		Annotation_Parameters_Form->MinVolt_LAT_Annotaiton_Edit->Text.ToDouble();

	if( Annotation_Parameters_Form->Automatic_Annotation_Checkbox->State == cbChecked )
		STUDY->Comp_Module.Automatic_Annotation_Flag = true;
	else
		STUDY->Comp_Module.Automatic_Annotation_Flag = true;

	if( Annotation_Parameters_Form->Individual_Ref_Annotation_CheckBox->State == cbChecked )
		STUDY->Comp_Module.Individual_Reference_Channel_Annotation = true;
	else
		STUDY->Comp_Module.Individual_Reference_Channel_Annotation = false;

	STUDY->Comp_Module.Deflection_SD_WL_ms = Annotation_Parameters_Form->EOS_SD_WL_Edit->Text.ToDouble();
	STUDY->Comp_Module.Deflection_SD_Th = Annotation_Parameters_Form->EOS_Th_Edit->Text.ToDouble();

	// SCAR SETTINGS
	App_Settings_Form->Application_Settings.Scar_BZ_Voltage_Threshold = Annotation_Parameters_Form->Scar_Threshold_Edit->Text.ToDouble();
	App_Settings_Form->Application_Settings.BZ_Healthy_Voltage_Threshold = Annotation_Parameters_Form->BZ_Threshold_Edit->Text.ToDouble();

	App_Settings_Form->Application_Settings.Conduction_Block_CV_Threshold = Annotation_Parameters_Form->Conduction_Block_Th_Edit->Text.ToDouble();
	App_Settings_Form->Application_Settings.Slow_Conduction_Threshold = Annotation_Parameters_Form->Slow_Conduction_Th_Edit->Text.ToDouble();

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm = Annotation_Parameters_Form->DataPoints_space_Filling_Th_Edit->Text.ToDouble();
	STUDY->Comp_Module.Max_Distance_Data_Points_Projection_mm = Annotation_Parameters_Form->Max_Dist_Corr_Nodes_Edit->Text.ToDouble();

	// update closest dps/nodes in case parameters changed.
	DP_update_required = false;
	if( prev_Data_Points_Filling_Threshold_mm != STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm )
	DP_update_required = true;
	if( DP_update_required )
		FULL_MAP_VALUES_UPDATE(STUDY->Current_Surface);
		//calculate_closest_nodes_and_data_points_pointers();

	if( STUDY->Comp_Module.Automatic_Annotation_Flag )
	if( STUDY->Surfaces_List.size() > 0 )
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.is_current_value_Voltage_Amplitude_value() ||
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.is_current_value_LAT_value() )
	{
		TModalResult Res = MessageDlg("Re-annotate current map?",
			mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
		if( Res == mrYes)
		{
			Progress_Form->add_text("Updating map...");
			Progress_Form->Show();
			Application->ProcessMessages();

			Annotation_Box.Reference_Annotation_Dragged = true;
			update_LAT_annotation_of_current_map();
			update_display_of_LAT_Bip_maps();
			Annotation_Box.Reference_Annotation_Dragged = false;

			Progress_Form->Hide();
		}
	}

	}

	repaint_all_controls();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Forcedatapointsontosurface1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	Progress_Form->add_text("Forcing data points onto 3D surface...");
	Progress_Form->Show();
	Application->ProcessMessages();

	STUDY->Surfaces_List[STUDY->Current_Surface].
		force_data_points_to_surface(
			STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr);
/*
	// force ablation points to surface
	double x,y,z;
	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[DS].Ablation_Points_List.size();i++)
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].get_coordinates_of_nearest_surface_point(
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Ablation_Points_List[i].x,
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Ablation_Points_List[i].y,
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Ablation_Points_List[i].z,
		&x,&y,&z);
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Ablation_Points_List[i].x = x;
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Ablation_Points_List[i].y = y;
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Ablation_Points_List[i].z = z;
	}
*/
	Progress_Form->add_text("Updating map...");
	Progress_Form->Show();
	Application->ProcessMessages();

	calculate_closest_nodes_and_data_points_pointers();

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::CalculateWavefrontPropagationVelocityWPVmap1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int Current_Value_Ptr = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_ptr();
	AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

	AnsiString Msg;
	Msg = "Algorithm will use currently selected map value as an input (at the moment it is ";
	Msg += Current_Value_Name + "). Are you happy with this selection or you want to stop and change ";
	Msg += "the current value? (LAT map should be used as basis for CV)";
	TModalResult Res = MessageDlg(Msg,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

	if( Res == mrYes)
	{

	Rich_Edit_Form->clear_text();
	Rich_Edit_Form->add_text("Calculation of propagation velocity is very challenging.");
	Rich_Edit_Form->add_text("As it is based on local activation time (LAT) map, it is as good as LAT map.");
	Rich_Edit_Form->add_text("Each misannotated point will introduce distortion in CV map.");
	Rich_Edit_Form->add_text("Best practice is to make sure, before exporting map from mapping system, that all points are proplerly annotated.");
	Rich_Edit_Form->add_text("");
	Rich_Edit_Form->add_text("This algorithm is based on work by P. Kojodjojo et al.");
	Rich_Edit_Form->add_text(" KOJODJOJO, P., KANAGARATNAM, P., MARKIDES, V., DAVIES, D. W. & PETERS, N. 2006. Age-related changes in human left and right atrial conduction. J Cardiovasc Electrophysiol, 17, 120-7.");
	Rich_Edit_Form->add_text("with geometry triangles used as a basis for calculation of velocity.");
	Rich_Edit_Form->ShowModal();

	AnsiString New_Map_Name = STUDY->Surfaces_List[STUDY->Current_Surface].
		calculate_conduction_velocity(Current_Value_Ptr);

	// set current value to CV map
	STUDY->Surfaces_List[STUDY->Current_Surface].
		Map_Values.set_current_value_according_to_name(New_Map_Name);

	STUDY->compute_min_max_values();

	OpenGL_Panel_1.prepare_colors_for_display();

	update_controls_state();

	repaint_3D_panels();

	} // aborted

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Annotation_Popup_ButtonClick(TObject *Sender)
{
    Annotation_Window_PopupMenu->Popup(Annotation_Window_PaintBox->Left,
	Annotation_Window_PaintBox->Top+50);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem1Click(TObject *Sender)
{
	double tt = Annotation_Box.Displayed_Segment_Length_ptr;

	Annotation_Box.Displayed_Segment_Length_ptr *= 2.;

//	save_state_of_GUI_controls(); // to recenter the display

	Annotation_Window_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem2Click(TObject *Sender)
{
	Annotation_Box.Displayed_Segment_Length_ptr /= 2.;

//	save_state_of_GUI_controls(); // to recenter the display

	Annotation_Window_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void TMain_Application_Window::post_import_initialization(int Surface_Ptr,double Max_Distance_Th_mm,bool DownSample_Mesh_Flag)
{
	int S = Surface_Ptr;
	STUDY->Surfaces_List[S].Current_Data_Point_Set_Ptr = 0;
	STUDY->Surfaces_List[S].Current_Data_Point_Ptr = 0;
	int dset = 0;
	int dp = 0;

	Value_Description_Class VDC;
	VDC.Value_Name = SEGMENTATION_VALUE_NAME;
	VDC.Interpolation_Type = INTERPOLATION_TYPE_FLAG_NO_INTERPOLATION;
	VDC.Unit = "";
	VDC.Displayed_In_Table = false;
	STUDY->Surfaces_List[S].Map_Values.add_value(VDC);

	// check for geo nodes number, downsample if necessary
	if( DownSample_Mesh_Flag )
	if( STUDY->Surfaces_List[S].Surface_Node_Set.size() >
			App_Settings_Form->Application_Settings.Max_Number_of_Geometry_Nodes )
	{
		double Ratio = (double)App_Settings_Form->Application_Settings.Max_Number_of_Geometry_Nodes/
			(double)STUDY->Surfaces_List[S].Surface_Node_Set.size();

		STUDY->Surfaces_List[S].downsample_mesh(Application_Directory,Ratio,Progress_Form);
	}

	//-------------------------------------------------------------------
	// initialize surface
	//-------------------------------------------------------------------
	STUDY->Surfaces_List[S].initialize_surface(Progress_Form);

	if( Display_Options_Form_1->Auto_DP_Size_Scaling_CheckBox->State == cbChecked )
	{
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Autoscale_Data_Point_Size = true;
		OpenGL_Panel_1.autoscale_data_point_size();
	}
	else
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Autoscale_Data_Point_Size = false;

	Main_Application_Window->Caption = Data_IO_Object.Data_Files_Path;

	// set length of signal boxes so that all egm fits
	if( STUDY->Surfaces_List[S].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[S].data_point_ptr_in_range() )
	{
		Annotation_Box.Displayed_Segment_Length_ptr =
			STUDY->Surfaces_List[S].Data_Point_Set[dset].
			Data_Points[dp].Roving_Signal.Voltage_Values.size()+100;
		ECG_Channels_Box_Parameters.Displayed_Segment_Length_ptr =
			Annotation_Box.Displayed_Segment_Length_ptr;
		Extra_Channels_Box_Parameters.Displayed_Segment_Length_ptr =
			Annotation_Box.Displayed_Segment_Length_ptr;

	//------------------------------------------------
	// Mapping system specific setup/initalization
	//------------------------------------------------
	if( STUDY->Surfaces_List[S].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_NAVX )
	{
		// remove not utilized data points
		STUDY->Surfaces_List[S].
			Data_Point_Set[dset].delete_not_utlized_and_displayed_points();
	}

	if( STUDY->Surfaces_List[S].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_CARTO )
	{
		// create label for each "SCAR" data point (to show their location)
		STUDY->Surfaces_List[S].create_labels_for_data_points_with_specified_type(
				CARTO_POINT_TYPE_SCAR, dset, "SC");

		// remove all data points with type != normal (so all Scar data points will be erased as well)
		STUDY->Surfaces_List[S].Data_Point_Set[dset].
			hard_delete_data_points_except_specified_type(DP_POINT_TYPE_NORMAL);
	}

	if( STUDY->Surfaces_List[S].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_RHYTHMIA )
	{
		// anything to remove from rhythmia import?
//		Annotation_Box.Local_Signal_Zoom = 0.004;
//		Annotation_Box.Reference_Signal_Zoom = 0.004;
//		Annotation_Box.ECG_Signal_Zoom = 0.004;
	}

	}
}

//-----------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Showlogwindow1Click(TObject *Sender)
{
	Progress_Form->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Deletedatapoint1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	double x,y,z;
	int Top = Data_Points_List_StringGrid->Selection.Top-1;
	int Bottom = Data_Points_List_StringGrid->Selection.Bottom-1;

	for(int i=Top;i<=Bottom;i++)
	{

	x = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Data_Points[Top].x;
	y = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Data_Points[Top].y;
	z = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Data_Points[Top].z;

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].delete_data_point(Top);

//	STUDY->Surfaces_List[STUDY->Current_Surface].update_closest_nodes_and_dps_around_xyz(x,y,z,DS);
	STUDY->Surfaces_List[STUDY->Current_Surface].update_closest_nodes_and_dps_after_dp_removal(DS,Top);

	}

	STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr =
			Data_Points_List_StringGrid->Row-1;

	STUDY->Surfaces_List[STUDY->Current_Surface].
		interpolate_all_values(0,DS,Progress_Form);

	OpenGL_Panel_1.prepare_colors_for_display();

	update_controls_state();
	repaint_all_controls();
	Progress_Form->Hide();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Undeletedatapoint1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].deleted_data_point_ptr_in_range() )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	double x,y,z;
	int Top = Deleted_Data_Points_List_StringGrid->Selection.Top-1;
	int Bottom = Deleted_Data_Points_List_StringGrid->Selection.Bottom-1;

	for(int i=Top;i<=Bottom;i++)
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].undelete_data_point(Top);

	STUDY->Surfaces_List[STUDY->Current_Surface].update_closest_nodes_and_dps_after_dp_undeleted(DS,
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Data_Points.size()-1);

	}

	STUDY->Surfaces_List[STUDY->Current_Surface].Current_Deleted_Data_Point_Ptr =
		Deleted_Data_Points_List_StringGrid->Row-1;

	STUDY->Surfaces_List[STUDY->Current_Surface].
		interpolate_all_values(0,DS,Progress_Form);

	OpenGL_Panel_1.prepare_colors_for_display();

	update_controls_state();
	repaint_all_controls();
	Progress_Form->Hide();
	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::AP_View_ButtonClick(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{

	// default
	OpenGL_Panel_1.set_NavX_AP_view();

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_CARTO )
	OpenGL_Panel_1.set_CARTO_AP_view();

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source == 0
		|| STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_NAVX )
	OpenGL_Panel_1.set_NavX_AP_view();

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_RHYTHMIA )
	OpenGL_Panel_1.set_Rhythmia_AP_view();

	repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::PA_View_ButtonClick(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{

	// default
	OpenGL_Panel_1.set_NavX_PA_view();

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_CARTO )
	OpenGL_Panel_1.set_CARTO_PA_view();

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source == 0
		|| STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_NAVX )
	OpenGL_Panel_1.set_NavX_PA_view();

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_RHYTHMIA )
	OpenGL_Panel_1.set_Rhythmia_PA_view();

	repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Centerallgeometries1Click(TObject *Sender)
{
	for(int S=0;S<STUDY->Surfaces_List.size();S++)
		STUDY->Surfaces_List[S].center_geometry_and_data_points();

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Markpathbeginnng1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{
	long N1 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		 STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle ].Nodes[0];
	STUDY->Surfaces_List[STUDY->Current_Surface].Path_Start = N1;
	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_between_start_and_end();
	repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Markintermediarypoint1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{
	long N1 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		 STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle ].Nodes[0];
	STUDY->Surfaces_List[STUDY->Current_Surface].Path_Intermediate = N1;
	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_between_start_and_end();
	repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Markpathend1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{
	long N1 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		 STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle ].Nodes[0];
	STUDY->Surfaces_List[STUDY->Current_Surface].Path_End = N1;
	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_between_start_and_end();
	repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Displayvaluesalongthepath1Click(TObject *Sender)
{
	long Nodeptr;
	double x=0,v;

	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	int Current_Value_Ptr = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_ptr();
	AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;
	AnsiString Current_Value_Unit = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_unit();

	Single_Plot_Form->Chart1->Title->Caption = Current_Value_Name + " values along the path";
	Single_Plot_Form->Chart1->LeftAxis->Title->Caption = Current_Value_Unit;
	Single_Plot_Form->Chart1->BottomAxis->Title->Caption = "mm";

	Single_Plot_Form->Chart1->Series[0]->Clear();

	for(long i=0; i <STUDY->Surfaces_List[STUDY->Current_Surface].Path_Vector.size(); i++)
	{

	Nodeptr = STUDY->Surfaces_List[STUDY->Current_Surface].Path_Vector[i];

	if( Nodeptr >= 0 && Nodeptr < STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size() )
	{

	if(i>0)
	x += STUDY->Surfaces_List[STUDY->Current_Surface].get_euclidean_distance_between_nodes(
			STUDY->Surfaces_List[STUDY->Current_Surface].Path_Vector[i],
			STUDY->Surfaces_List[STUDY->Current_Surface].Path_Vector[i-1]);
	v = STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Node_Set[Nodeptr].get_value(dset, Current_Value_Ptr);

	if( v != NOT_POSSIBLE_TO_CALCULATE_VALUE )
	Single_Plot_Form->Chart1->Series[0]->AddXY(x,v);

	} // if nodeptr valid

	} // through nodes along the path

	Single_Plot_Form->ShowModal();

	}
}
//---------------------------------------------------------------------------




void __fastcall TMain_Application_Window::Closeworkspace1Click(TObject *Sender)
{
	TModalResult Res;
	if( STUDY->Surfaces_List.size() > 0 )
	Res = MessageDlg("Save current study workspace?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

	if( Res == mrYes)
	Saveworkspace1Click(this);

	if( Res != mrCancel )
	close_workspace();
}
//---------------------------------------------------------------------------

void TMain_Application_Window::close_workspace()
{
	STUDY->Current_Surface = -1;
	STUDY->Study_Source_Path = ""; // path to file/folder used as a source - MODIFIED JUST ONCE
	STUDY->Study_Current_Path_And_File_Name = ""; // path to current path and filename (updated every time save is performed)
	STUDY->Surfaces_List.clear();
	clear_controls();
	repaint_all_controls();
	repaint_3D_panels();
	draw_fluoro_image_2D();
	Main_Application_Window->Caption = "EPLab Research Works";
}

//---------------------------------------------------------------------------

void TMain_Application_Window::clear_controls()
{
	Geometry_Selection_ComboBox->Clear();
//	Surfaces_CheckListBox->Clear();
	STUDY->Current_Surface = -1;
	Data_Point_Set_Selection_ComboBox->Clear();
	Displayed_Value_Selection_ComboBox_1->Clear();
	Data_Points_List_StringGrid->RowCount = 1;
	Deleted_Data_Points_List_StringGrid->RowCount = 1;
	Images_StringGrid->RowCount = 1;
	Annotation_Window_PaintBoxPaint(this);
	All_EGMs_PaintBoxPaint(this);
	ECG_Channels_PaintBoxPaint(this);
	Extra_Channels_PaintBoxPaint(this);
	Main_Application_Window->Caption = "";
	Limit_Image_Display_Bottom_CheckBox->State = cbUnchecked;
	Limit_Image_Display_Top_CheckBox->State = cbUnchecked;
	Display_Images_In_3D_Panel_CheckBox->State = cbChecked;
	Display_Surface_CheckBox->State = cbChecked;

	repaint_3D_panels();
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Deletedatapointswithcurrentvaluebelowgiventhreshold1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if(STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	Ask_For_Single_Item_Form1->Caption = "Threshold: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "1";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Value = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	long D;

	Progress_Form->add_text("Removing data points with value below given threshold... ");
	Progress_Form->Show();
	Application->ProcessMessages();

	AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

	D = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		delete_data_points_with_value_lower_than(Current_Value_Name,
			Value,
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

	Progress_Form->add_text(IntToStr((int)D) + " data points removed...");
	Progress_Form->Show();
	Application->ProcessMessages();

	if( D>0 )
	calculate_closest_nodes_and_data_points_pointers();

	Data_Point_Set_Selection_ComboBoxChange(this);
	Displayed_Value_Selection_ComboBox_1Change(this);

	}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Signals_ScrollBarChange(TObject *Sender)
{
	if( Phase_Movie_Mode_CheckBox->State == cbChecked )
	{
		double Size;
		int DS;

		for(int S=0;S<STUDY->Surfaces_List.size();S++)
		{
		DS = STUDY->Surfaces_List[S].Current_Data_Point_Set_Ptr;
		Size = STUDY->Surfaces_List[S].Data_Point_Set[DS].Data_Points[0].Roving_Signal.Phase.size();
		Global_Movie_TimePoint_ptr = ((double)Signals_ScrollBar->Position /
			(double)Signals_ScrollBar->Max )*Size;
		STUDY->Surfaces_List[S].set_phase_value_of_map_to_specific_time_point(Global_Movie_TimePoint_ptr);
		}

		SampleValue_Edit->Text = FloatToStr(Global_Movie_TimePoint_ptr);

		OpenGL_Panel_1.prepare_colors_for_display();
		repaint_3D_panels();
	}
	else
	{
	// sliding all signals box
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() == ON )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr].
			Data_Points[0].Roving_Signal.Voltage_Values.size() > 0  )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

   // sliding annotation box
	double Size = STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[DS].Data_Points[DP].Roving_Signal.Voltage_Values.size();
	double v;

	if( Size > 0 )
	{
	v = ((double)Signals_ScrollBar->Position /
		(double)Signals_ScrollBar->Max )*Size;
	Annotation_Box.Annotation_Display_Start = v;
	All_Signals_Box_Parameters.Annotation_Display_Start = v;
	ECG_Channels_Box_Parameters.Annotation_Display_Start = v;
	Extra_Channels_Box_Parameters.Annotation_Display_Start = v;
	}
	else
	{
	Annotation_Box.Annotation_Display_Start = 0;
	All_Signals_Box_Parameters.Annotation_Display_Start = 0;
	ECG_Channels_Box_Parameters.Annotation_Display_Start = 0;
	Extra_Channels_Box_Parameters.Annotation_Display_Start = 0;
	}

	Annotation_Window_PaintBoxPaint(this);
	All_EGMs_PaintBoxPaint(this);
	ECG_Channels_PaintBoxPaint(this);
	Extra_Channels_PaintBoxPaint(this);

	}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::DisplaylocalsignalinSignalAnalysiswindow1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	NL_Analysis_Form->Data_Items.clear();
	NL_Analysis_Form->Data_Items.push_back(&STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[DS].Data_Points[DP].Roving_Signal);
	NL_Analysis_Form->reset_and_clear_displays();
	NL_Analysis_Form->ShowModal();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Getareaandareaofvalueaboveandbeyondspecifiedthreshold1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
   {

   Ask_For_Single_Item_Form1->Caption = "Threshold: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "1";
   if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
   {

   StringGrid_Form->Caption = "Area and %area analysis";

   double Threshold = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
   AnsiString FileName = Utils.get_file_name_from_full_path(STUDY->Study_Current_Path_And_File_Name);
   int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
   double Area_Above,Area_Below,Total_Area;
   int Current_Value_Ptr = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_ptr();
   AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

   StringGrid_Form->StringGrid1->ColCount = 8;
   StringGrid_Form->StringGrid1->RowCount = 2;

   StringGrid_Form->StringGrid1->Cells[0][0] = "FileName";
   StringGrid_Form->StringGrid1->Cells[1][0] = "Geometry";
   StringGrid_Form->StringGrid1->Cells[2][0] = "DataPointsSetName";
   StringGrid_Form->StringGrid1->Cells[3][0] = "Value";
   StringGrid_Form->StringGrid1->Cells[4][0] = "Area_below_threshold[cm^2]";
   StringGrid_Form->StringGrid1->Cells[5][0] = "Area_above_threshold[cm^2]";
   StringGrid_Form->StringGrid1->Cells[6][0] = "%Area_below_threshold";
   StringGrid_Form->StringGrid1->Cells[7][0] = "%Area_above_threshold";

   STUDY->Surfaces_List[STUDY->Current_Surface].calculate_area_taken_by_Value(
		0,Current_Value_Ptr,Threshold,1,&Area_Above,&Total_Area,dset);
   STUDY->Surfaces_List[STUDY->Current_Surface].calculate_area_taken_by_Value(
		0,Current_Value_Ptr,Threshold,-1,&Area_Below,&Total_Area,dset);
/*
   Area_Below /= 100.;
   Area_Above /= 100.;
   Total_Area /= 100.;
*/
   if( Total_Area != 0 )
   {

   StringGrid_Form->StringGrid1->Cells[0][1] = FileName;
   StringGrid_Form->StringGrid1->Cells[1][1] = STUDY->Surfaces_List[STUDY->Current_Surface].Name;
   StringGrid_Form->StringGrid1->Cells[2][1] = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Name;
   StringGrid_Form->StringGrid1->Cells[3][1] = Current_Value_Name;
   StringGrid_Form->StringGrid1->Cells[4][1] = FloatToStrF(Area_Below,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[5][1] = FloatToStrF(Area_Above,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[6][1] = FloatToStrF(100.0*Area_Below/Total_Area,ffGeneral,3,2);
   StringGrid_Form->StringGrid1->Cells[7][1] = FloatToStrF(100.0*Area_Above/Total_Area,ffGeneral,3,2);

   StringGrid_Form->ShowModal();

   }

   }
   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::GetareaandareaofcurrentvalueaboveandbeyondspecifiedthresholdPERSEGMENT1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
   {

   Ask_For_Single_Item_Form1->Caption = "Threshold: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "1";

   int Current_Value_Ptr = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_ptr();
   AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

   if( Current_Value_Name == SEGMENTATION_VALUE_NAME )
   ShowMessage("Select other map than segmentation for quantitative analysis");
   else
   if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
   {

   StringGrid_Form->Caption = "Area and %area analysis per segment";

   double Threshold = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
   AnsiString FileName = Utils.get_file_name_from_full_path(STUDY->Study_Current_Path_And_File_Name);
   int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
   double Area_Above,Area_Below,Total_Area;

   StringGrid_Form->StringGrid1->ColCount = 10;

   StringGrid_Form->StringGrid1->Cells[0][0] = "FileName";
   StringGrid_Form->StringGrid1->Cells[1][0] = "Geometry";
   StringGrid_Form->StringGrid1->Cells[2][0] = "DataPointsSetName";
   StringGrid_Form->StringGrid1->Cells[3][0] = "Segment_Name";
   StringGrid_Form->StringGrid1->Cells[4][0] = "Value";
   StringGrid_Form->StringGrid1->Cells[5][0] = "Threshold";
   StringGrid_Form->StringGrid1->Cells[6][0] = "Area_below_threshold[cm^2]";
   StringGrid_Form->StringGrid1->Cells[7][0] = "Area_above_threshold[cm^2]";
   StringGrid_Form->StringGrid1->Cells[8][0] = "%Area_below_threshold";
   StringGrid_Form->StringGrid1->Cells[9][0] = "%Area_above_threshold";

   int Segment_Id,Column = 0;

   for(int Seg_Ptr=1;Seg_Ptr<Segments_Info.Segments.size();Seg_Ptr++)
   {

   Segment_Id = Segments_Info.Segments[Seg_Ptr].Code;

   STUDY->Surfaces_List[STUDY->Current_Surface].calculate_area_taken_by_Value(
		Segment_Id,Current_Value_Ptr,Threshold,1,&Area_Above,&Total_Area,dset);
   STUDY->Surfaces_List[STUDY->Current_Surface].calculate_area_taken_by_Value(
		Segment_Id,Current_Value_Ptr,Threshold,-1,&Area_Below,&Total_Area,dset);
/*
   Area_Below /= 100.;
   Area_Above /= 100.;
   Total_Area /= 100.;
*/

   if( Total_Area != 0 )
   {
	   Column++;
	   StringGrid_Form->StringGrid1->RowCount = 1 + Column;

	   StringGrid_Form->StringGrid1->Cells[0][Column] = FileName;
	   StringGrid_Form->StringGrid1->Cells[1][Column] = STUDY->Surfaces_List[STUDY->Current_Surface].Name;
	   StringGrid_Form->StringGrid1->Cells[2][Column] = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Name;
	   StringGrid_Form->StringGrid1->Cells[3][Column] = Segments_Info.Segments[Seg_Ptr].Name;
	   StringGrid_Form->StringGrid1->Cells[4][Column] = Current_Value_Name;
	   StringGrid_Form->StringGrid1->Cells[5][Column] = FloatToStrF(Threshold,ffGeneral,3,2);
	   StringGrid_Form->StringGrid1->Cells[6][Column] = FloatToStrF(Area_Below,ffGeneral,3,2);
	   StringGrid_Form->StringGrid1->Cells[7][Column] = FloatToStrF(Area_Above,ffGeneral,3,2);
	   StringGrid_Form->StringGrid1->Cells[8][Column] = FloatToStrF(100.0*Area_Below/Total_Area,ffGeneral,3,2);
	   StringGrid_Form->StringGrid1->Cells[9][Column] = FloatToStrF(100.0*Area_Above/Total_Area,ffGeneral,3,2);
   }

   } // THROUGH segments

   StringGrid_Form->ShowModal();

   }
   }
}

//---------------------------------------------------------------------------



void __fastcall TMain_Application_Window::DisplayrefegminSignalAnalysiswindow1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	NL_Analysis_Form->Data_Items.clear();
	NL_Analysis_Form->Data_Items.push_back(&STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[DS].Data_Points[DP].Reference_Signal);
	NL_Analysis_Form->reset_and_clear_displays();
	NL_Analysis_Form->ShowModal();
	NL_Analysis_Form->V_Wave_Source = 1;

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::DisplayECGegminSignalAnalysiswindow1Click(TObject *Sender)
{
	// sliding all signals box
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() == ON )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	NL_Analysis_Form->Data_Items.clear();
	NL_Analysis_Form->Data_Items.push_back(&STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[DS].Data_Points[DP].ECG_Signal);
	NL_Analysis_Form->reset_and_clear_displays();
	NL_Analysis_Form->ShowModal();
	NL_Analysis_Form->V_Wave_Source = 2;

	}

}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::UpdateReferenceBarpositionwithrespecttocurrentlyselectedVwave1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	vector <double> PSimilarity_Vector;
	double Min,Max;
	long Min_Ptr,Max_Ptr,Start,Stop;
	int Peak_Range_Ptr;


	// 1. Check if V wave is selected
	if( NL_Analysis_Form->V_Wave.size() == 0 )
	ShowMessage("Select V wave first. See Help section 'Floating V wave' based annotation.");
	else
	{

	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[dset].Data_Points.size();i++)
	{
		if( i%10 == 0 )
		{
		Progress_Form->replace_last_line_with_text("Processing point:" +
			IntToStr((int)i) + "/" + IntToStr((int)STUDY->Surfaces_List[STUDY->Current_Surface].
			Data_Point_Set[dset].Data_Points.size()));
		Progress_Form->Show();
		Application->ProcessMessages();
		}

		// get correlation vector
		if( NL_Analysis_Form->V_Wave_Source == 1 )
		PSimilarity_Vector = PNUM.get_correlation_vector(
					&STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
					Data_Points[i].Reference_Signal.Voltage_Values,
					&NL_Analysis_Form->V_Wave,
					1./STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
					Data_Points[i].Reference_Signal.Time_Step_ms );
		if( NL_Analysis_Form->V_Wave_Source == 2 )
		PSimilarity_Vector = PNUM.get_correlation_vector(
					&STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
					Data_Points[i].ECG_Signal.Voltage_Values,
					&NL_Analysis_Form->V_Wave,
					1./STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
					Data_Points[i].ECG_Signal.Time_Step_ms );

		// find max
//		PNUM.find_min_max(&PSimilarity_Vector,&Min,&Max,&Min_Ptr,&Max_Ptr);
		Peak_Range_Ptr = 10 / STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
					Data_Points[i].ECG_Signal.Time_Step_ms;
		Start = 0.1*PSimilarity_Vector.size();
		Stop = 0.9*PSimilarity_Vector.size();
		PNUM.find_max_peak_with_max_slopes(&PSimilarity_Vector,Start,Stop,&Max,&Max_Ptr,Peak_Range_Ptr);

		// reassign ref
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
					Data_Points[i].Ref_Signal_Activation_ptr = Max_Ptr;
	}

	// update annotation of current map
	Annotation_Box.Reference_Annotation_Dragged = true;
	update_LAT_annotation_of_current_map();
	update_display_of_LAT_Bip_maps();
	Annotation_Box.Reference_Annotation_Dragged = false;

	STUDY->compute_min_max_values();

	// set individual ref annotaiton flag
	STUDY->Comp_Module.Individual_Reference_Channel_Annotation = true;
	ShowMessage("Individual positioning of reference bar mode enabled. Go to Options->Annotation options to modify.");

	Progress_Form->Hide();
	repaint_3D_panels();

	} // if V wave size > 0

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Deletepointswithspecificvalue1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() )
	if(STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	Ask_For_Single_Item_Form1->Caption = "Value: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "-10000";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Value = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	long D;

	Progress_Form->add_text("Removing data points with specific value... ");
	Progress_Form->Show();
	Application->ProcessMessages();

	AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

	D = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		delete_data_points_with_specified_value(Current_Value_Name,
			Value,
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

	Progress_Form->add_text(IntToStr((int)D) + " data points removed...");
	Progress_Form->Show();
	Application->ProcessMessages();

	if( D > 0 )
	calculate_closest_nodes_and_data_points_pointers();

	Data_Point_Set_Selection_ComboBoxChange(this);
	Displayed_Value_Selection_ComboBox_1Change(this);

	}

	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Changenameofdatapointset1Click(TObject *Sender)
{
	if(STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() == ON)
	{

	Ask_For_Single_Item_Form1->Caption = "Change data points set name";
	Ask_For_Single_Item_Form1->Edit_Box->Visible = true;
	Ask_For_Single_Item_Form1->Edit_Box->Text =
			STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr].Name;

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr].Name =
		Ask_For_Single_Item_Form1->Edit_Box->Text;

	Data_Point_Set_Selection_ComboBoxChange(this);

	}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Clonecurrentdatapointset1Click(TObject *Sender)

{
//    save_backup_study();

	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int ptr0 = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int ptr1 = ptr0 + 1;

	std::vector<Data_Point_Set_Class>::iterator theIterator = STUDY->
			Surfaces_List[STUDY->Current_Surface].Data_Point_Set.begin();

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.insert(
	theIterator+ptr0, 1, STUDY->Surfaces_List[STUDY->Current_Surface].
	Data_Point_Set[ptr0] );

	AnsiString Name = STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[ptr0].Name;

	// case of 1 digit
	if( Name.SubString(Name.Length()-1,1) == ".")
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[ptr1].Name =
	Name.SubString(0,Name.Length()-2)+"."+
	IntToStr( Name.SubString(Name.Length()-0,1).ToInt()+1);
	else
	// case of 2 digits
	if( Name.SubString(Name.Length()-2,1) == ".")
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[ptr1].Name =
	Name.SubString(0,Name.Length()-3)+"."+
	IntToStr( Name.SubString(Name.Length()-1,2).ToInt()+1);
	else
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[ptr1].Name = Name+".1";


	// copy nodes values
	double v;
				  // get_value(dset, Current_Value_Ptr);
				  // set_value(Data_Point_Set_Ptr,Value_Id,v

	// shift existing values
	for(long Set=STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size()-1; Set>ptr1; Set--)
	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();i++)
	for(int Val=0;Val<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_number();Val++)
	{
	v = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].get_value(Set-1,Val);
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].set_value(Set,Val,v);
	}

	// copy added values to new set
	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();i++)
	for(int Val=0;Val<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_number();Val++)
	{
	v = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].get_value(ptr0,Val);
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].set_value(ptr1,Val,v);
	}

	update_controls_state();

	ShowMessage("Name of cloned data point set: '" + STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[ptr1].Name +"'");

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Deletecurrentdatapointset1Click(TObject *Sender)
{
//    save_backup_study();

	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( MessageDlg("Are you sure you want to delete this data point set?",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	{

	int ptr0 = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.erase(
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.begin() + ptr0 );

	// copy nodes values
	double v;

	// shift existing values
	for(long Set=ptr0;Set<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size(); Set++)
	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();i++)
	for(int Val=0;Val<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_number();Val++)
	{
	v = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].get_value(Set+1,Val);
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].set_value(Set,Val,v);
	}

	}

	update_controls_state();
}
//---------------------------------------------------------------------------





void __fastcall TMain_Application_Window::Changecurrentgeometryname1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{

	Ask_For_Single_Item_Form1->Caption = "Change geometry name";
	Ask_For_Single_Item_Form1->Edit_Box->Visible = true;
	Ask_For_Single_Item_Form1->Edit_Box->Text =
			STUDY->Surfaces_List[STUDY->Current_Surface].Name;

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Name = Ask_For_Single_Item_Form1->Edit_Box->Text;

	Geometry_Selection_ComboBoxChange(this);

	}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Clonecurrentsurface1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() )
	{

	std::vector<Surface_Class>::iterator theIterator =
		STUDY->Surfaces_List.end();

	STUDY->Surfaces_List.insert(theIterator, 1, STUDY->Surfaces_List[STUDY->Current_Surface] );

	STUDY->Current_Surface = STUDY->Surfaces_List.size()-1;


	AnsiString Name = STUDY->Surfaces_List[STUDY->Current_Surface].Name;

	// case of 1 digit
	if( Name.SubString(Name.Length()-1,1) == ".")
	STUDY->Surfaces_List[STUDY->Current_Surface].Name =
	Name.SubString(0,Name.Length()-2)+"."+
	IntToStr( Name.SubString(Name.Length()-0,1).ToInt()+1);
	else
	// case of 2 digits
	if( Name.SubString(Name.Length()-2,1) == ".")
	STUDY->Surfaces_List[STUDY->Current_Surface].Name =
	Name.SubString(0,Name.Length()-3)+"."+
	IntToStr( Name.SubString(Name.Length()-1,2).ToInt()+1);
	else
	STUDY->Surfaces_List[STUDY->Current_Surface].Name = Name+".1";

//	STUDY->Surfaces_List[STUDY->Current_Surface].Name = "Cloned Geo";
/*
	//--------------------------------------------
	// FILLING OF SURFACE SELECTION COMBOBOX
	//--------------------------------------------
	Geometry_Selection_ComboBox->Clear();
	for(long i=0;i<(signed)STUDY->Surfaces_List.size();i++)
	Geometry_Selection_ComboBox->AddItem(STUDY->Surfaces_List[i].Name,NULL);
*/
	Geometry_Selection_ComboBox->ItemIndex = STUDY->Current_Surface;
//	Surfaces_CheckListBox->ItemIndex = STUDY->Current_Surface;

	update_controls_state();

	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Restoreoriginallocationsofdatapoints1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	Progress_Form->add_text("Restoring original locations of data poins...");
	Progress_Form->Show();
	Application->ProcessMessages();

	STUDY->Surfaces_List[STUDY->Current_Surface].
		restore_original_data_points_locations(
			STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr);

	Progress_Form->add_text("Updating map...");
	Progress_Form->Show();
	Application->ProcessMessages();

	calculate_closest_nodes_and_data_points_pointers();

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Averagefilter1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int Current_Value_Ptr = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_ptr();
	AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].
		Map_Values.Current_Map_Value_Name;

	Ask_For_Single_Item_Form1->Caption = "Range for spatial filtering [mm]:";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "10";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Range = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	Progress_Form->add_text("Average spatial filtering of map: " + Current_Value_Name);
	Progress_Form->Show();
	Application->ProcessMessages();

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		apply_average_spatial_filter(
				Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble(),
				Current_Value_Name,
				STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

	STUDY->Surfaces_List[STUDY->Current_Surface].interpolate_specific_value(
		0,dset,Current_Value_Ptr,Progress_Form);

	STUDY->compute_min_max_values();

	OpenGL_Panel_1.prepare_colors_for_display();

	update_controls_state();

	repaint_all_controls();

	repaint_3D_panels();

	Progress_Form->Hide();

	}
	}
}
//---------------------------------------------------------------------------



void __fastcall TMain_Application_Window::Deletepath1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Path_Start = -1;
	STUDY->Surfaces_List[STUDY->Current_Surface].Path_Intermediate = -1;
	STUDY->Surfaces_List[STUDY->Current_Surface].Path_End = -1;
	STUDY->Surfaces_List[STUDY->Current_Surface].Path_Vector.clear();
	repaint_3D_panels();
	}

}
//---------------------------------------------------------------------------



void __fastcall TMain_Application_Window::BasedonslidingwindowSD1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	AnsiString Msg;
	Msg = "Algorithm will use sliding window approach with parameters defined in 'Annotation parameters' window,' ";
	Msg += "fields: 'Onset/offset local deflection','SD window size' and 'Cutoff threshold'.";
	Msg += "Are you happy with values set there?";
	TModalResult Res = MessageDlg(Msg,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

	if( Res == mrYes)
	{

	Progress_Form->add_text("Calculating electrogram duration map... ");
	Progress_Form->Show();
	Application->ProcessMessages();

	STUDY->Comp_Module.Sliding_Window_Algorithm = EGM_DURATION_ALGORITHM_SD;

	//---------------------
	AnsiString New_Map_Name = STUDY->Surfaces_List[STUDY->Current_Surface].
		calculate_egm_duration_map(dset,&STUDY->Comp_Module);
	//---------------------

	// set current value to EGMd map
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.set_current_value_according_to_name(New_Map_Name);

	STUDY->compute_min_max_values();

	OpenGL_Panel_1.prepare_colors_for_display();

	update_controls_state();

	Progress_Form->Hide();

	repaint_3D_panels();

	} // aborted

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::BasedonslidingwindowIntegral1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	AnsiString Msg;
	Msg = "Algorithm will use sliding window of integral approach with parameters defined in 'Annotation parameters' window,' ";
	Msg += "fields: 'Onset/offset local deflection','SD window size' and 'Cutoff threshold'.";
	Msg += "Are you happy with values set there?";
	TModalResult Res = MessageDlg(Msg,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

	if( Res == mrYes)
	{

	Progress_Form->add_text("Calculating electrogram duration map... ");
	Progress_Form->Show();
	Application->ProcessMessages();

	STUDY->Comp_Module.Sliding_Window_Algorithm = EGM_DURATION_ALGORITHM_INTEGRAL;

	//---------------------
	AnsiString New_Map_Name = STUDY->Surfaces_List[STUDY->Current_Surface].
		calculate_egm_duration_map(dset,&STUDY->Comp_Module);
	//---------------------

	// set current value to EGMd map
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.set_current_value_according_to_name(New_Map_Name);

	STUDY->compute_min_max_values();

	OpenGL_Panel_1.prepare_colors_for_display();

	update_controls_state();

	Progress_Form->Hide();

	repaint_3D_panels();

	} // aborted

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Combined1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	AnsiString Msg;
	Msg = "Algorithm will use sliding window of integral approach with parameters defined in 'Annotation parameters' window,' ";
	Msg += "fields: 'Onset/offset local deflection','SD window size' and 'Cutoff threshold'.";
	Msg += "Are you happy with values set there?";
	TModalResult Res = MessageDlg(Msg,mtConfirmation,TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);

	if( Res == mrYes)
	{

	Progress_Form->add_text("Calculating electrogram duration map... ");
	Progress_Form->Show();
	Application->ProcessMessages();

	STUDY->Comp_Module.Sliding_Window_Algorithm = EGM_DURATION_ALGORITHM_INTEGRAL;

	//---------------------
	AnsiString New_Map_Name = STUDY->Surfaces_List[STUDY->Current_Surface].
		calculate_egm_duration_map(dset,&STUDY->Comp_Module);
	//---------------------

	// set current value to EGMd map
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.set_current_value_according_to_name(New_Map_Name);

	STUDY->compute_min_max_values();

	OpenGL_Panel_1.prepare_colors_for_display();

	update_controls_state();

	Progress_Form->Hide();

	repaint_3D_panels();

	} // aborted

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::DeletedatapointswithcurrentvalueABOVEgiventhreshold1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	Ask_For_Single_Item_Form1->Caption = "Threshold: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "1";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Value = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	long D;

	Progress_Form->add_text("Removing data points with value above given threshold... ");
	Progress_Form->Show();
	Application->ProcessMessages();

	AnsiString Current_Value_Name =
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

	D = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		delete_data_points_with_value_greater_than(Current_Value_Name,
			Value,
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

	Progress_Form->add_text(IntToStr((int)D) + " data points removed...");
	Progress_Form->Show();
	Application->ProcessMessages();

	if( D>0 )
	calculate_closest_nodes_and_data_points_pointers();

	Data_Point_Set_Selection_ComboBoxChange(this);
	Displayed_Value_Selection_ComboBox_1Change(this);

	}

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Undeletealldatapoints1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	long D;

	Progress_Form->add_text("Restoring removed data points...");
	Progress_Form->Show();
	Application->ProcessMessages();

	D = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			restore_removed_data_points();

	if( D > 0 )
	calculate_closest_nodes_and_data_points_pointers();

	Data_Point_Set_Selection_ComboBoxChange(this);
	Displayed_Value_Selection_ComboBox_1Change(this);

	Progress_Form->add_text(IntToStr((int)D) + " data points restored...");
	Progress_Form->Show();
	Application->ProcessMessages();


	}

	}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ECG_Channels_PaintBoxMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   ECG_Channels_Box_Parameters.MouseButton = Button;
   ECG_Channels_Box_Parameters.MouseStatus = MB_DOWN;

   ECG_Channels_Box_Parameters.Mouse_Position_X = X;
   ECG_Channels_Box_Parameters.Mouse_Position_Y = Y;

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ECG_Channels_PaintBoxMouseMove(TObject *Sender,
		  TShiftState Shift, int X, int Y)
{
	//----------------------------------------------------------
	// RIHGT BUTTON
	//----------------------------------------------------------
	if( ECG_Channels_Box_Parameters.MouseButton == mbRight &&
		ECG_Channels_Box_Parameters.MouseStatus ==  MB_DOWN)
	{
		if (Y > ECG_Channels_Box_Parameters.Mouse_Position_Y )
			ECG_Channels_Box_Parameters.Signals_Amplitude_Amplify *= 1.5;
		if (Y < ECG_Channels_Box_Parameters.Mouse_Position_Y )
			ECG_Channels_Box_Parameters.Signals_Amplitude_Amplify /= 1.5;

		ECG_Channels_PaintBoxPaint(this);

	ECG_Channels_Box_Parameters.Mouse_Position_X = X;
	ECG_Channels_Box_Parameters.Mouse_Position_Y = Y;

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ECG_Channels_PaintBoxMouseUp(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   ECG_Channels_Box_Parameters.MouseButton = Button;
   ECG_Channels_Box_Parameters.MouseStatus = MB_UP;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ECG_Channels_PaintBoxPaint(TObject *Sender)

{
	long ptr,ptr1,ptr2,i,j;
	double x1,y1,x2,y2,x3;
	int pos;
	double Vertical_Spacing;

	// clear signals box
	ECG_Channels_Box_Bitmap->Canvas->Brush->Color = (TColor)RGB(230,230,230);
	ECG_Channels_Box_Bitmap->Canvas->FillRect(ECG_Channels_PaintBox->ClientRect );

	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].deleted_data_point_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].
					Current_Data_Point_Set_Ptr;
	long dpptr = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	double Start = ECG_Channels_Box_Parameters.Annotation_Display_Start;
	double Stop = Start + ECG_Channels_Box_Parameters.Displayed_Segment_Length_ptr;

	//------------------------------
	//------------------------------
	// SIGNALS PAINT
	//------------------------------
	//------------------------------
	double Bottom_Offset = 0; // BottomOffset_Edit->Text.ToDouble();
	std::vector<double> *Data_Vec1;

	// current signals display
	for(int i=0;i<12;i++)
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[dpptr].All_ECG_Channels[i].Voltage_Values.size() > 3 )
	{

	Data_Vec1 = &STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[dpptr].All_ECG_Channels[i].Voltage_Values;

	Vertical_Spacing = (ECG_Channels_PaintBox->Height-Bottom_Offset)*
		(double)(i+1)/(double)(12+1);

	// set line width
	ECG_Channels_Box_Bitmap->Canvas->Pen->Width = 1;

	// set color
	ECG_Channels_Box_Bitmap->Canvas->Pen->Color = clBlack;
	ECG_Channels_Box_Bitmap->Canvas->Font->Color = clBlack;
	ECG_Channels_Box_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;

	for(long j=0;j<ECG_Channels_PaintBox->Width;j++)
	{

	ptr1 = Start+(double)j/(double)ECG_Channels_PaintBox->Width*(Stop-Start);
	ptr2 = Start+(double)(j+1)/(double)ECG_Channels_PaintBox->Width*(Stop-Start);

	if( ptr1 <(signed)Data_Vec1[0].size() &&  ptr2 < (signed)Data_Vec1[0].size() )
	{

	x1 = j;
	y1 = Vertical_Spacing-Data_Vec1[0][ptr1]*ECG_Channels_Box_Parameters.Signals_Amplitude_Amplify;

	x2 = j+1;
	y2 = Vertical_Spacing-Data_Vec1[0][ptr2]*ECG_Channels_Box_Parameters.Signals_Amplitude_Amplify;

	ECG_Channels_Box_Bitmap->Canvas->MoveTo( x1, y1 );
	ECG_Channels_Box_Bitmap->Canvas->LineTo( x2, y2 );

	}

	} // for all pixels


	//-------------------------------------
	// display name
	//-------------------------------------
	ECG_Channels_Box_Bitmap->Canvas->Brush->Color=(TColor)( RGB(200,200,200) );
	ECG_Channels_Box_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;
	ECG_Channels_Box_Bitmap->Canvas->TextFlags = ETO_OPAQUE;
	ECG_Channels_Box_Bitmap->Canvas->TextOutW(5,
		ECG_Channels_PaintBox->Height*(double)(i+1)/(double)(12+1)-5,
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[dpptr].All_ECG_Channels[i].Name);

	} // thru all dps

	} // if all in range

	ECG_Channels_Box_Bitmap->Canvas->Pen->Width = 1;
	ECG_Channels_PaintBox->Canvas->Draw(0, 0, ECG_Channels_Box_Bitmap);

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem3Click(TObject *Sender)
{
	ECG_Channels_Box_Parameters.Displayed_Segment_Length_ptr *= 2;
	ECG_Channels_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem4Click(TObject *Sender)
{
	ECG_Channels_Box_Parameters.Displayed_Segment_Length_ptr /= 2;
	ECG_Channels_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem5Click(TObject *Sender)
{
	Extra_Channels_Box_Parameters.Displayed_Segment_Length_ptr *= 2;
	Extra_Channels_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem6Click(TObject *Sender)
{
	Extra_Channels_Box_Parameters.Displayed_Segment_Length_ptr /= 2;
	Extra_Channels_PaintBoxPaint(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Extra_Channels_PaintBoxMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Extra_Channels_Box_Parameters.MouseButton = Button;
   Extra_Channels_Box_Parameters.MouseStatus = MB_DOWN;

   Extra_Channels_Box_Parameters.Mouse_Position_X = X;
   Extra_Channels_Box_Parameters.Mouse_Position_Y = Y;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Extra_Channels_PaintBoxMouseMove(TObject *Sender,
		  TShiftState Shift, int X, int Y)
{
	//----------------------------------------------------------
	// RIHGT BUTTON
	//----------------------------------------------------------
	if( Extra_Channels_Box_Parameters.MouseButton == mbRight &&
		Extra_Channels_Box_Parameters.MouseStatus ==  MB_DOWN)
	{
		if (Y > Extra_Channels_Box_Parameters.Mouse_Position_Y )
			Extra_Channels_Box_Parameters.Signals_Amplitude_Amplify *= 1.5;
		if (Y < Extra_Channels_Box_Parameters.Mouse_Position_Y )
			Extra_Channels_Box_Parameters.Signals_Amplitude_Amplify /= 1.5;

		Extra_Channels_PaintBoxPaint(this);

	Extra_Channels_Box_Parameters.Mouse_Position_X = X;
	Extra_Channels_Box_Parameters.Mouse_Position_Y = Y;

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Extra_Channels_PaintBoxMouseUp(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
   Extra_Channels_Box_Parameters.MouseButton = Button;
   Extra_Channels_Box_Parameters.MouseStatus = MB_UP;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Extra_Channels_PaintBoxPaint(TObject *Sender)

{
	long ptr,ptr1,ptr2,i,j;
	double x1,y1,x2,y2,x3;
	int pos;
	double Vertical_Spacing;

	// clear signals box
	Extra_Channels_Box_Bitmap->Canvas->Brush->Color = (TColor)RGB(230,230,230);
	Extra_Channels_Box_Bitmap->Canvas->FillRect(Extra_Channels_PaintBox->ClientRect );

	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].deleted_data_point_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].
					Current_Data_Point_Set_Ptr;
	long dpptr = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	double Start = Extra_Channels_Box_Parameters.Annotation_Display_Start;
	double Stop = Start + Extra_Channels_Box_Parameters.Displayed_Segment_Length_ptr;

	// SIGNALS PAINT
	double Bottom_Offset = 0; // BottomOffset_Edit->Text.ToDouble();
	std::vector<double> *Data_Vec1;

	//------------------------------
	// current signals display ---------------------------------------------------------
	//------------------------------
	for(int i=0;i<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[dpptr].Additional_Ref_Channels.size();i++)
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[dpptr].Additional_Ref_Channels[i].Voltage_Values.size() > 3 )
	{

	Data_Vec1 = &STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[dpptr].Additional_Ref_Channels[i].Voltage_Values;

	Vertical_Spacing = (Extra_Channels_PaintBox->Height-Bottom_Offset)*
		(double)(i+1)/(double)(12+1);

	// set line width
	Extra_Channels_Box_Bitmap->Canvas->Pen->Width = 1;

	// set color
	Extra_Channels_Box_Bitmap->Canvas->Pen->Color = clBlack;
	Extra_Channels_Box_Bitmap->Canvas->Font->Color = clBlack;
	Extra_Channels_Box_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;

	for(long j=0;j<Extra_Channels_PaintBox->Width;j++)
	{

	ptr1 = Start+(double)j/(double)Extra_Channels_PaintBox->Width*(Stop-Start);
	ptr2 = Start+(double)(j+1)/(double)Extra_Channels_PaintBox->Width*(Stop-Start);

	if( ptr1 <(signed)Data_Vec1[0].size() &&  ptr2 < (signed)Data_Vec1[0].size() )
	{

	x1 = j;
	y1 = Vertical_Spacing-Data_Vec1[0][ptr1]*Extra_Channels_Box_Parameters.Signals_Amplitude_Amplify;

	x2 = j+1;
	y2 = Vertical_Spacing-Data_Vec1[0][ptr2]*Extra_Channels_Box_Parameters.Signals_Amplitude_Amplify;

	Extra_Channels_Box_Bitmap->Canvas->MoveTo( x1, y1 );
	Extra_Channels_Box_Bitmap->Canvas->LineTo( x2, y2 );

	}

	} // for all pixels

	//-------------------------------------
	// display name
	//-------------------------------------
	Extra_Channels_Box_Bitmap->Canvas->Brush->Color=(TColor)( RGB(200,200,200) );
	Extra_Channels_Box_Bitmap->Canvas->Font->Style = TFontStyles()<< fsBold;
	Extra_Channels_Box_Bitmap->Canvas->TextFlags = ETO_OPAQUE;
	Extra_Channels_Box_Bitmap->Canvas->TextOutW(5,
		Extra_Channels_PaintBox->Height*(double)(i+1)/(double)(12+1)-5,
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		Data_Points[dpptr].Additional_Ref_Channels[i].Name);

	} // thru all dps

	} // if all in range

	Extra_Channels_Box_Bitmap->Canvas->Pen->Width = 1;
	Extra_Channels_PaintBox->Canvas->Draw(0, 0, Extra_Channels_Box_Bitmap);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ECG_Signals_Display_Control_ButtonClick(TObject *Sender)
{
   ECG_Signal_Tracings_PopupMenu->Popup(Right_PageControl->Left,
	Right_PageControl->Top+50);

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Extra_Channels_ButtonClick(TObject *Sender)

{
   Extra_Signal_Tracings_PopupMenu->Popup(Right_PageControl->Left,
	Right_PageControl->Top+50);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Importimagesofslicedhearttissue1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   {

	OpenDialog->Title = "Select slices files (bitmap *.bmp format)";
	OpenDialog->FilterIndex = 6;

	Ask_For_Single_Item_Form1->Caption = "Inter-images spacing [mm] (can be modified later)";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "4";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	if( OpenDialogMultiSelect->Execute() )
	{

	AnsiString Result = Data_IO_Object.import_images_files(
			OpenDialogMultiSelect->Files,STUDY,Progress_Form,true);

	if( Result == "Import completed" )
	{
	   	// set initial thickness in each image
		double Th = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

		for(int i=0;i<STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size();i++)
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[i].Slice_Thickness = Th;

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.update_data_set();

		Progress_Form->add_text("Import completed. Press CLOSE to continue." );
		Progress_Form->Show();
		Application->ProcessMessages();

		update_images_table();
		draw_fluoro_image_2D();
		repaint_3D_panels();
	}
	else
	{
		Progress_Form->Hide();
		ShowMessage(Result);
	}
	}

   }
   }
   else
   ShowMessage("Current map is empty. First load EA map, then import images.");
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Fluoro_Image_Paint_BoxPaint(TObject *Sender)
{
	draw_fluoro_image_2D();
}
//---------------------------------------------------------------------------

void TMain_Application_Window::draw_fluoro_image_2D()
{
	// clear
	Fluoro_Image_Bimap->Canvas->Brush->Color = clWhite;
	Fluoro_Image_Bimap->Canvas->FillRect(
						Rect(0,0,Fluoro_Image_Paint_Box->Width,
								 Fluoro_Image_Paint_Box->Height));

   if( STUDY->is_current_surface_in_range() &&
	   STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.current_fluoro_image_valid() )
   {

   // gogh3
	int Image_Id = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image;

	double Distance_of_Image_From_Patient_Centre;
	int R1,R,G,B;
	double ptr,iptr,jptr;
	int X,Y;

	Image_2D_Class* Fluoro_Image;
	Fluoro_Image = &STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id];

   // set size of display window
   double W = Right_PageControl->Width - 8;
   double H = 0.5*Right_PageControl->Height-15;

   double ImageW = Fluoro_Image->Fluoro_Image_Width;
   double ImageH = Fluoro_Image->Fluoro_Image_Height;

	if( ImageW > ImageH )
	{
		Fluoro_Image_Paint_Box->Width = W;
		Fluoro_Image_Paint_Box->Height = ImageH/ImageW * W;
	}
	else
	{
		Fluoro_Image_Paint_Box->Width = ImageW/ImageH * H;
		Fluoro_Image_Paint_Box->Height = H;
	}

	Fluoro_Image_Bimap->Width = Fluoro_Image_Paint_Box->Width;
	Fluoro_Image_Bimap->Height = Fluoro_Image_Paint_Box->Height;



	double scalex = (double)(Fluoro_Image->X_RL-Fluoro_Image->X_LU)/
					(double)Fluoro_Image_Bimap->Width;

	double scaley = (double)(Fluoro_Image->Y_RL-Fluoro_Image->Y_LU)/
					(double)Fluoro_Image_Bimap->Height;

	Distance_of_Image_From_Patient_Centre = 0.5*Fluoro_Image->Distance_from_Emiter;

	if( scalex == 0 || scaley == 0 )
	{
		scalex = 1;
		scaley = 1;
	}

	//----------------------------------------------------------------------
	// Paint fluoro image
	//----------------------------------------------------------------------

//	if( Display_3D_Shadow_on_Fluoro_Image_CheckBox->State == cbUnchecked )
//	{

	for (int i=0; i<Fluoro_Image_Bimap->Width; i++)
	for (int j=0; j<Fluoro_Image_Bimap->Height; j++)
	{
		// get RGB values from fluoro image
		iptr = Fluoro_Image->X_LU+i*scalex;
		jptr = Fluoro_Image->Y_LU+j*scaley;

		ptr = 3.0*((int)(Fluoro_Image->Fluoro_Image_Height-1-(int)jptr)*
						 Fluoro_Image->Fluoro_Image_Width + (int)iptr);

		ptr = (int)ptr - (int)ptr%3;

		if( ptr >= 0 && ptr+2 < Fluoro_Image->Fluoro_Image_Data_vector.size() )
		{
			B = Fluoro_Image->Fluoro_Image_Data_vector[(int)ptr+0]; // was R
			G = Fluoro_Image->Fluoro_Image_Data_vector[(int)ptr+1];
			R = Fluoro_Image->Fluoro_Image_Data_vector[(int)ptr+2]; // was B
		}
		else
		{
			R = 0;
			G = 0;
			B = 0;
		}

		Fluoro_Image_Bimap->Canvas->Pixels[i][j] = (TColor)RGB(B,G,R);
	}
/*
	}
	else
	{

	// set which 3D shadow to take:
	unsigned char* Geo_3D_Image_Data_current;

	if( Fluoro_Image->Registered_Status == 1 )
		Geo_3D_Image_Data_current = Geo_3D_Image_Data_1;

	if( Fluoro_Image->Registered_Status == 2 )
		Geo_3D_Image_Data_current = Geo_3D_Image_Data_2;

	// start painting image
	for (int i=0; i<Fluoro_Image_Bimap->Width; i++)
	for (int j=0; j<Fluoro_Image_Bimap->Height; j++)
	{
		// get RGB values from fluoro image
		iptr = Fluoro_Image->X_LU+i*scalex;
		jptr = Fluoro_Image->Y_LU+j*scaley;

		ptr = 3.0*((int)(Fluoro_Image->Fluoro_Image_Height-1-(int)jptr)*
						 Fluoro_Image->Fluoro_Image_Width + (int)iptr);

		ptr = (int)ptr - (int)ptr%3;

		if( ptr >= 0 && ptr+2 < Fluoro_Image->Fluoro_Image_Data_vector.size() )
		{
			R = Fluoro_Image->Fluoro_Image_Data_vector[(int)ptr+0];
			G = Fluoro_Image->Fluoro_Image_Data_vector[(int)ptr+1];
			B = Fluoro_Image->Fluoro_Image_Data_vector[(int)ptr+2];
		}
		else
		{
			R = 0;
			G = 0;
			B = 0;
		}

		// get RGB values from 3D image from Ortho projection
		iptr = iptr*Geo_3D_Image_Width/Fluoro_Image->Fluoro_Image_Width;
		jptr = jptr*Geo_3D_Image_Height/Fluoro_Image->Fluoro_Image_Height;

		// get red level of 3D object
		if( 3*((int)jptr*Geo_3D_Image_Width+(int)iptr)+0 >= 0 )
		if( 3*((int)jptr*Geo_3D_Image_Width+(int)iptr)+0 <
			Geo_3D_Image_Width*Geo_3D_Image_Height*4 )
		{
		R1 = Geo_3D_Image_Data_current[
			3*((int)jptr*Geo_3D_Image_Width+(int)iptr)+0];
		R1 = G + R1*CT_Geo_Intensity_On_Fluoro_Image;
		}
		else
		R1 = G;

		if( R1 > 255 ) R1 = 255;

		Fluoro_Image_Bimap->Canvas->Pixels[i][j] = (TColor)RGB(R1,G,G); // with red 3D shadow
	}

	} // painting with 3D geo shadow
*/

	//---------------------------------------------------
	// Paint registration points
	//---------------------------------------------------
	double Circle_Size=5;

	if( Fluoro_Image->Point_A_Located == true )
	{
	X = (Fluoro_Image->Registration_Point_A_ij.x-Fluoro_Image->X_LU)/scalex;
	Y = (Fluoro_Image->Registration_Point_A_ij.y-Fluoro_Image->Y_LU)/scaley;
	Fluoro_Image_Bimap->Canvas->Brush->Style = bsSolid;
	Fluoro_Image_Bimap->Canvas->Brush->Color = clRed;
	Fluoro_Image_Bimap->Canvas->Pen->Color = clRed;
	Fluoro_Image_Bimap->Canvas->Ellipse(X-Circle_Size,Y-Circle_Size,
										X+Circle_Size,Y+Circle_Size );
	Fluoro_Image_Bimap->Canvas->Brush->Style = bsClear;
	Fluoro_Image_Bimap->Canvas->Font->Color = clRed;
	Fluoro_Image_Bimap->Canvas->TextOut(X+Circle_Size+2,Y+Circle_Size+2,"Registration Point A");
	}

	if( Fluoro_Image->Point_B_Located == true )
	{
	X = (Fluoro_Image->Registration_Point_B_ij.x-Fluoro_Image->X_LU)/scalex;
	Y = (Fluoro_Image->Registration_Point_B_ij.y-Fluoro_Image->Y_LU)/scaley;
	Fluoro_Image_Bimap->Canvas->Brush->Style = bsSolid;
	Fluoro_Image_Bimap->Canvas->Brush->Color = clGreen;
	Fluoro_Image_Bimap->Canvas->Pen->Color = clGreen;
	Fluoro_Image_Bimap->Canvas->Ellipse(X-Circle_Size,Y-Circle_Size,
										X+Circle_Size,Y+Circle_Size );
	Fluoro_Image_Bimap->Canvas->Brush->Style = bsClear;
	Fluoro_Image_Bimap->Canvas->Font->Color = clGreen;
	Fluoro_Image_Bimap->Canvas->TextOut(X+Circle_Size+2,Y+Circle_Size+2,"Registration Point B");
	}

	if( Fluoro_Image->Point_C_Located == true )
	{
	X = (Fluoro_Image->Registration_Point_C_ij.x-Fluoro_Image->X_LU)/scalex;
	Y = (Fluoro_Image->Registration_Point_C_ij.y-Fluoro_Image->Y_LU)/scaley;
	Fluoro_Image_Bimap->Canvas->Brush->Style = bsSolid;
	Fluoro_Image_Bimap->Canvas->Brush->Color = clBlue;
	Fluoro_Image_Bimap->Canvas->Pen->Color = clBlue;
	Fluoro_Image_Bimap->Canvas->Ellipse(X-Circle_Size,Y-Circle_Size,
										X+Circle_Size,Y+Circle_Size );
	Fluoro_Image_Bimap->Canvas->Brush->Style = bsClear;
	Fluoro_Image_Bimap->Canvas->Font->Color = clBlue;
	Fluoro_Image_Bimap->Canvas->TextOut(X+Circle_Size+2,Y+Circle_Size+2,"Registration Point C");
	}

	if( Fluoro_Image->Point_Rov_Located == true )
	{
	X = (Fluoro_Image->Registration_Point_Rov_ij.x-Fluoro_Image->X_LU)/scalex;
	Y = (Fluoro_Image->Registration_Point_Rov_ij.y-Fluoro_Image->Y_LU)/scaley;
	Fluoro_Image_Bimap->Canvas->Brush->Style = bsSolid;
	Fluoro_Image_Bimap->Canvas->Brush->Color = (TColor)RGB(255,150,0);
	Fluoro_Image_Bimap->Canvas->Pen->Color = (TColor)RGB(255,150,0);
	Fluoro_Image_Bimap->Canvas->Ellipse(X-Circle_Size,Y-Circle_Size,
										X+Circle_Size,Y+Circle_Size );
	Fluoro_Image_Bimap->Canvas->Brush->Style = bsClear;
	Fluoro_Image_Bimap->Canvas->Font->Color = (TColor)RGB(255,150,0);
	Fluoro_Image_Bimap->Canvas->TextOut(X+Circle_Size+2,Y+Circle_Size+2,"Roving point");
	}

	//---------------------------------------------------
	// Paint scaling crosses
	//---------------------------------------------------
	int Cross_Size=7;

	Fluoro_Image_Bimap->Canvas->Pen->Color = clYellow;
	Fluoro_Image_Bimap->Canvas->Brush->Color = clYellow;

	X = (Fluoro_Image->Fluoro_Image_2D_scaling_points_X[0]-Fluoro_Image->X_LU)/scalex;
	Y = (Fluoro_Image->Fluoro_Image_2D_scaling_points_Y[0]-Fluoro_Image->Y_LU)/scaley;
	Fluoro_Image_Bimap->Canvas->MoveTo(X,Y-Cross_Size);
	Fluoro_Image_Bimap->Canvas->LineTo(X,Y+Cross_Size);
	Fluoro_Image_Bimap->Canvas->MoveTo(X-Cross_Size,Y);
	Fluoro_Image_Bimap->Canvas->LineTo(X+Cross_Size,Y);

	X = (Fluoro_Image->Fluoro_Image_2D_scaling_points_X[1]-Fluoro_Image->X_LU)/scalex;
	Y = (Fluoro_Image->Fluoro_Image_2D_scaling_points_Y[1]-Fluoro_Image->Y_LU)/scaley;
	Fluoro_Image_Bimap->Canvas->MoveTo(X,Y-Cross_Size);
	Fluoro_Image_Bimap->Canvas->LineTo(X,Y+Cross_Size);
	Fluoro_Image_Bimap->Canvas->MoveTo(X-Cross_Size,Y);
	Fluoro_Image_Bimap->Canvas->LineTo(X+Cross_Size,Y);

	//---------------------------------------------------
	// Paint labels if located within this slice
	//---------------------------------------------------
	int image_i,image_j,Slice;
	for(int Label=0;Label<STUDY->Surfaces_List[STUDY->Current_Surface].Labels.size();Label++)
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.xyz_to_ijk(
		STUDY->Surfaces_List[STUDY->Current_Surface].Labels[Label].x,
		STUDY->Surfaces_List[STUDY->Current_Surface].Labels[Label].y,
		STUDY->Surfaces_List[STUDY->Current_Surface].Labels[Label].z,&image_i,&image_j,&Slice);

	X = (image_i-Fluoro_Image->X_LU)/scalex;
	Y = (image_j-Fluoro_Image->Y_LU)/scaley;

	if (Slice == Image_Id )
	{

	Fluoro_Image_Bimap->Canvas->Brush->Style = bsSolid;
	Fluoro_Image_Bimap->Canvas->Brush->Color = clWhite; //(TColor)RGB(0,0,0);
	Fluoro_Image_Bimap->Canvas->Pen->Color = clWhite; // (TColor)RGB(0,0,0);
	Fluoro_Image_Bimap->Canvas->Ellipse(X-Circle_Size,Y-Circle_Size,
										X+Circle_Size,Y+Circle_Size );
	Fluoro_Image_Bimap->Canvas->Brush->Style = bsClear;
	Fluoro_Image_Bimap->Canvas->Font->Color = clWhite; // (TColor)RGB(0,0,0);
	Fluoro_Image_Bimap->Canvas->TextOut(X+Circle_Size+2,Y+Circle_Size+2,
	STUDY->Surfaces_List[STUDY->Current_Surface].Labels[Label].Name.c_str());

	}
	}

	Fluoro_Image_Paint_Box->Canvas->Draw(0, 0, Fluoro_Image_Bimap);


	}
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Fluoro_Image_Paint_BoxMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	FImage_Box_Parameters.MouseStatus = MB_DOWN;

	FImage_Box_Parameters.Mouse_Position_X = X;
	FImage_Box_Parameters.Mouse_Position_Y = Y;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Fluoro_Image_Paint_BoxMouseMove(TObject *Sender,
		  TShiftState Shift, int X, int Y)
{
	FImage_Box_Parameters.Mouse_Position_X = X;
	FImage_Box_Parameters.Mouse_Position_Y = Y;

	Main_Application_Window->Cursor = crArrow;

	double Sensitivity = 0.5; // range: 0-1

	if( STUDY->is_current_surface_in_range() )
	if( FImage_Box_Parameters.MouseStatus == MB_DOWN )
	{
		int Image_Id = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image;

		if( Image_Id>=0 && Image_Id < STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size() )
		{

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].X_LU -=
		 Sensitivity*(X-FImage_Box_Parameters.Mouse_Position_X);
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].Y_LU -=
		 Sensitivity*(Y-FImage_Box_Parameters.Mouse_Position_Y);

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].X_RL -=
		 Sensitivity*(X-FImage_Box_Parameters.Mouse_Position_X);
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].Y_RL -=
		 Sensitivity*(Y-FImage_Box_Parameters.Mouse_Position_Y);

		}

		FImage_Box_Parameters.Mouse_Position_X = X;
		FImage_Box_Parameters.Mouse_Position_Y = Y;

		draw_fluoro_image_2D();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Fluoro_Image_Paint_BoxMouseUp(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	FImage_Box_Parameters.MouseStatus = MB_UP;
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Markfirstpointforimagescaling1Click(TObject *Sender)

{
	Image_2D_Class* Fluoro_Image;

	int Image_Id = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image;

	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.current_fluoro_image_valid() )
	{

	Fluoro_Image = &STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id];

	double scalex = (Fluoro_Image->X_RL-Fluoro_Image->X_LU)/
					(double)Fluoro_Image_Bimap->Width;

	double scaley = (Fluoro_Image->Y_RL-Fluoro_Image->Y_LU)/
					(double)Fluoro_Image_Bimap->Height;

	if( scalex == 0 || scaley == 0 )
	{
		scalex = 1;
		scaley = 1;
	}

	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].
		Fluoro_Image_2D_scaling_points_X[0] =
				Fluoro_Image->X_LU+FImage_Box_Parameters.Mouse_Position_X*scalex;
	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].
		Fluoro_Image_2D_scaling_points_Y[0] =
				Fluoro_Image->Y_LU+FImage_Box_Parameters.Mouse_Position_Y*scaley;

	draw_fluoro_image_2D();

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Marksecondpointforimagescaling1Click(TObject *Sender)

{
	Image_2D_Class* Fluoro_Image;

	int Image_Id = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image;

	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.current_fluoro_image_valid() )
	{

	Fluoro_Image = &STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id];

	double scalex = (Fluoro_Image->X_RL-Fluoro_Image->X_LU)/
					(double)Fluoro_Image_Bimap->Width;

	double scaley = (Fluoro_Image->Y_RL-Fluoro_Image->Y_LU)/
					(double)Fluoro_Image_Bimap->Height;

	if( scalex == 0 || scaley == 0 )
	{
		scalex = 1;
		scaley = 1;
	}

	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].
		Fluoro_Image_2D_scaling_points_X[1] =
				Fluoro_Image->X_LU+FImage_Box_Parameters.Mouse_Position_X*scalex;
	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].
		Fluoro_Image_2D_scaling_points_Y[1] =
				Fluoro_Image->Y_LU+FImage_Box_Parameters.Mouse_Position_Y*scaley;

	draw_fluoro_image_2D();

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Scaleimage1Click(TObject *Sender)
{
	int Id = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image;

	Ask_For_Single_Item_Form1->Caption = "Actual distance between marked points [mm]";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "10";

   if( STUDY->is_current_surface_in_range() )
   {
	if( !STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.current_fluoro_image_valid() )
	ShowMessage("Fluoro image not selected");
	else
	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{
		double Pixel_Distance = sqrt(
			std::pow( STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Id].Fluoro_Image_2D_scaling_points_X[1]-
					  STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Id].Fluoro_Image_2D_scaling_points_X[0],2) +
			std::pow( STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Id].Fluoro_Image_2D_scaling_points_Y[1]-
					  STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Id].Fluoro_Image_2D_scaling_points_Y[0],2) );

		double Dist_mm = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

		if( Dist_mm!=0 && Pixel_Distance!=0 )
		{
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Fluoro_Pixel_Size_In_mm = Dist_mm/Pixel_Distance;
		}
		else
		ShowMessage("Distance between marked points must be greater than 0.");
/*
		// recalculate position of registration and roving point in 3D space
		set_registration_point_on_fluoro_image(0);
		set_registration_point_on_fluoro_image(1);
		set_registration_point_on_fluoro_image(2);
		set_registration_point_on_fluoro_image(3);
*/
		// reclaculate position of intersection points
//		STUDY->calculate_ABCRov_intersection_points();

		update_images_table();
		repaint_3D_panels();
	}
   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Markreferencepoint11Click(TObject *Sender)
{
	set_registration_point_on_fluoro_image(0);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Markreferencepoint21Click(TObject *Sender)
{
	set_registration_point_on_fluoro_image(1);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Markreferencepoint31Click(TObject *Sender)
{
	set_registration_point_on_fluoro_image(2);
}
//---------------------------------------------------------------------------

void TMain_Application_Window::set_registration_point_on_fluoro_image(int Which_Point)
{
	Image_2D_Class* Fluoro_Image;
	double Distance_of_Image_From_Patient_Centre;

   if( STUDY->is_current_surface_in_range() )
   {

	int Image_Id = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image;

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.current_fluoro_image_valid() )
	{

	Fluoro_Image = &STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id];

	Distance_of_Image_From_Patient_Centre = Fluoro_Image->Distance_from_Emiter;

	double scalex = (Fluoro_Image->X_RL-Fluoro_Image->X_LU)/
					(double)Fluoro_Image_Bimap->Width;

	double scaley = (Fluoro_Image->Y_RL-Fluoro_Image->Y_LU)/
					(double)Fluoro_Image_Bimap->Height;

	if( scalex == 0 || scaley == 0 )
	{
		scalex = 1;
		scaley = 1;
	}

	if( Which_Point == 0 ) // "A" point
	{
		// clear flags on other images (at the moment it will be sort of "unique" reg point
		for(int i=0;i<STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size();i++)
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[i].Point_A_Located = false;

		Fluoro_Image->Registration_Point_A_ij.x =
				Fluoro_Image->X_LU+FImage_Box_Parameters.Mouse_Position_X*scalex;
		Fluoro_Image->Registration_Point_A_ij.y =
		Fluoro_Image->Y_LU+FImage_Box_Parameters.Mouse_Position_Y*scaley;
		Fluoro_Image->Point_A_Located = true;
	}

	if( Which_Point == 1 ) // "B" point
	{
		// clear flags on other images (at the moment it will be sort of "unique" reg point
		for(int i=0;i<STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size();i++)
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[i].Point_B_Located = false;

		Fluoro_Image->Registration_Point_B_ij.x =
				Fluoro_Image->X_LU+FImage_Box_Parameters.Mouse_Position_X*scalex;
		Fluoro_Image->Registration_Point_B_ij.y =
		Fluoro_Image->Y_LU+FImage_Box_Parameters.Mouse_Position_Y*scaley;
		Fluoro_Image->Point_B_Located = true;
	}

	if( Which_Point == 2 ) // "C" point
	{
		// clear flags on other images (at the moment it will be sort of "unique" reg point
		for(int i=0;i<STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size();i++)
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[i].Point_C_Located = false;

		Fluoro_Image->Registration_Point_C_ij.x =
				Fluoro_Image->X_LU+FImage_Box_Parameters.Mouse_Position_X*scalex;
		Fluoro_Image->Registration_Point_C_ij.y =
		Fluoro_Image->Y_LU+FImage_Box_Parameters.Mouse_Position_Y*scaley;
		Fluoro_Image->Point_C_Located = true;
	}

	draw_fluoro_image_2D();
	repaint_3D_panels();

	}
   }
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ClearRegistrationPoints1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   {

	int Image_Id = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image;

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.current_fluoro_image_valid() )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].Point_A_Located = false;
	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].Point_B_Located = false;
	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].Point_C_Located = false;
	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Image_Id].Point_Rov_Located = false;

	}
	draw_fluoro_image_2D();
	repaint_3D_panels();
   }
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::A1Click(TObject *Sender)
{
	set_registration_point_on_3D_geometry(0);
	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::B1Click(TObject *Sender)
{
	set_registration_point_on_3D_geometry(1);
	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::C1Click(TObject *Sender)
{
	set_registration_point_on_3D_geometry(2);
	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void TMain_Application_Window::set_registration_point_on_3D_geometry(int Which_Point)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{

	// Find (x,y,z) of center of the pointed triangle
	long ptr0,ptr1,ptr2;
	ptr0 = STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];
	ptr1 = STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[1];
	ptr2 = STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[2];

	double x,y,z;
	x = (STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr0].x+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr1].x+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr2].x)/3.0;
	y = (STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr0].y+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr1].y+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr2].y)/3.0;
	z = (STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr0].z+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr1].z+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr2].z)/3.0;

	if( Which_Point==0 )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_A.x = x;
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_A.y = y;
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_A.z = z;
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_A_normal.x =
			STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].normal[0];
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_A_normal.y =
			STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].normal[1];
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_A_normal.z =
			STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].normal[2];
		STUDY->Surfaces_List[STUDY->Current_Surface].Point_A_Located = true;

	}

	if( Which_Point==1 )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_B.x = x;
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_B.y = y;
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_B.z = z;
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_B_normal.x =
			STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].normal[0];
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_B_normal.y =
			STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].normal[1];
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_B_normal.z =
			STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].normal[2];
		STUDY->Surfaces_List[STUDY->Current_Surface].Point_B_Located = true;
	}

	if( Which_Point==2 )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_C.x = x;
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_C.y = y;
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_C.z = z;
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_C_normal.x =
			STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].normal[0];
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_C_normal.y =
			STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].normal[1];
		STUDY->Surfaces_List[STUDY->Current_Surface].Registration_Point_C_normal.z =
			STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].normal[2];
		STUDY->Surfaces_List[STUDY->Current_Surface].Point_C_Located = true;
	}

	repaint_3D_panels();

	}
	else
	ShowMessage("Point precisely location of the point on surface.");
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Registercurrentgeometrywithsetofhistologyslices1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{

	// find xyz of images registration points

	int A_Image,B_Image,C_Image;

	A_Image = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.get_registration_point_image_id(0);
	B_Image = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.get_registration_point_image_id(1);
	C_Image = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.get_registration_point_image_id(2);

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Point_A_Located &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Point_B_Located &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Point_C_Located &&
		A_Image >= 0 && B_Image >= 0 && C_Image >= 0 )
	{
		// transform ij registration points on image to 3D
		Point_3D Registration_Point_A;
		Point_3D Registration_Point_B;
		Point_3D Registration_Point_C;

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.ijk_to_xyz(
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[A_Image].Registration_Point_A_ij.x,
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[A_Image].Registration_Point_A_ij.y,
			A_Image,
			&Registration_Point_A.x,&Registration_Point_A.y,&Registration_Point_A.z);

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.ijk_to_xyz(
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[B_Image].Registration_Point_B_ij.x,
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[B_Image].Registration_Point_B_ij.y,
			B_Image,
			&Registration_Point_B.x,&Registration_Point_B.y,&Registration_Point_B.z);

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.ijk_to_xyz(
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[C_Image].Registration_Point_C_ij.x,
			STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[C_Image].Registration_Point_C_ij.y,
			C_Image,
			&Registration_Point_C.x,&Registration_Point_C.y,&Registration_Point_C.z);

		for(long i=0;i<200;i++)
		{
		STUDY->Surfaces_List[STUDY->Current_Surface].perform_one_step_of_triangles_ABC_fitting(
			Registration_Point_A,Registration_Point_B,Registration_Point_C);
		repaint_3D_panels();
		}
	}
	else
	ShowMessage("Operation requires 3 registration points (A,B,C) on both 3D surface and histology images");

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ranslatecurrentgeometry1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() )
	{

	int res = mrOk;

	Gemetric_Transformation_Form->X_Edit->Text = "0.0";
	Gemetric_Transformation_Form->Y_Edit->Text = "0.0";
	Gemetric_Transformation_Form->Z_Edit->Text = "0.0";

	Gemetric_Transformation_Form->Teta_Edit->Text = "0.0";
	Gemetric_Transformation_Form->Fi_Edit->Text = "0.0";

	Gemetric_Transformation_Form->aX_Edit->Text = "1.0";
	Gemetric_Transformation_Form->aY_Edit->Text = "1.0";
	Gemetric_Transformation_Form->aZ_Edit->Text = "1.0";

	res = Gemetric_Transformation_Form->ShowModal();

	if( res == mrOk )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].make_geometrical_transformation(
    Gemetric_Transformation_Form->X_Edit->Text.ToDouble(),
    Gemetric_Transformation_Form->Y_Edit->Text.ToDouble(),
    Gemetric_Transformation_Form->Z_Edit->Text.ToDouble(),
    Gemetric_Transformation_Form->Teta_Edit->Text.ToDouble(),
    Gemetric_Transformation_Form->Fi_Edit->Text.ToDouble(),
    Gemetric_Transformation_Form->aX_Edit->Text.ToDouble(),
    Gemetric_Transformation_Form->aY_Edit->Text.ToDouble(),
	Gemetric_Transformation_Form->aZ_Edit->Text.ToDouble(),
    Gemetric_Transformation_Form->Seg_No_Edit->Text.ToDouble() );

	repaint_3D_panels();

	}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Addlabel1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{

	// Find (x,y,z) of center of the pointed triangle
	long ptr0,ptr1,ptr2;
	ptr0 = STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];
	ptr1 = STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[1];
	ptr2 = STUDY->Surfaces_List[STUDY->Current_Surface].
			Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[2];

	double x,y,z;
	x = (STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr0].x+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr1].x+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr2].x)/3.0;
	y = (STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr0].y+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr1].y+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr2].y)/3.0;
	z = (STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr0].z+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr1].z+
		 STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr2].z)/3.0;

	Selection_List_Form->Caption = "Select label to locate";
	Selection_List_Form->fill_selection(&Labels_Names);

	Selection_List_Form->Extra_Button->Caption = "Add label";
	Selection_List_Form->Extra_Button->Visible = true;
	Selection_List_Form->Extra_Action_Name = "Add label";

	Selection_List_Form->Extra_Button_2->Caption = "Remove current label";
	Selection_List_Form->Extra_Button_2->Visible = true;
	Selection_List_Form->Extra_Action_2_Name = "Remove label";

	if( Selection_List_Form->ShowModal() )
	{

	save_labels_list("Labels_List.txt");

	if( Selection_List_Form->Selected_Item_Ptr >= 0 )
	{

	Label_Class Label_Class_Item;

	Label_Class_Item.Name = Labels_Names[Selection_List_Form->Selected_Item_Ptr];

	// coordinates
	Label_Class_Item.x = x;
	Label_Class_Item.y = y;
	Label_Class_Item.z = z;

	// normal
	Label_Class_Item.nx = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr0].normal[0];
	Label_Class_Item.ny = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr0].normal[1];
	Label_Class_Item.nz = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[ptr0].normal[2];

	STUDY->Surfaces_List[STUDY->Current_Surface].Labels.push_back(Label_Class_Item);

	repaint_3D_panels();

	//------------------------------------------------------------------------------------
	// if slice images are present, locate slice closest to the pointed location
	//------------------------------------------------------------------------------------
	int image_x,image_y,Slice;

	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.xyz_to_ijk(
				x,y,z,&image_x,&image_y,&Slice);

	if (Slice >= 0 && Slice <
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size() )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image = Slice;
	Images_StringGrid->Row = Slice + 1;

	draw_fluoro_image_2D();

	} // image_z/slice in range

	}
	}

	Selection_List_Form->Extra_Action_Name = "";
	Selection_List_Form->Extra_Button->Visible = false;
	Selection_List_Form->Extra_Action_2_Name = "";
	Selection_List_Form->Extra_Button_2->Visible = false;

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Deletelabel1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Label >= 0 )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Labels.erase(
		STUDY->Surfaces_List[STUDY->Current_Surface].Labels.begin() +
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Label );

	STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Label = -1 ;

	draw_fluoro_image_2D();
	repaint_3D_panels();

	}
	else
	ShowMessage("To select label to remove, hover with mouse cursor over the sphere denoting label and click right mouse button");

}
//---------------------------------------------------------------------------



void __fastcall TMain_Application_Window::MenuItem8Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].deleted_data_point_ptr_in_range() )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Deleted_Data_Point_Ptr;

	NL_Analysis_Form->Data_Items.clear();
	NL_Analysis_Form->Data_Items.push_back(&STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[DS].Deleted_Data_Points[DP].Roving_Signal);
	NL_Analysis_Form->reset_and_clear_displays();
	NL_Analysis_Form->ShowModal();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem9Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].deleted_data_point_ptr_in_range() )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Deleted_Data_Point_Ptr;

	NL_Analysis_Form->Data_Items.clear();
	NL_Analysis_Form->Data_Items.push_back(&STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[DS].Deleted_Data_Points[DP].Reference_Signal);
	NL_Analysis_Form->reset_and_clear_displays();
	NL_Analysis_Form->ShowModal();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem10Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].deleted_data_point_ptr_in_range() )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Deleted_Data_Point_Ptr;

	NL_Analysis_Form->Data_Items.clear();
	NL_Analysis_Form->Data_Items.push_back(&STUDY->Surfaces_List[STUDY->Current_Surface].
		Data_Point_Set[DS].Deleted_Data_Points[DP].ECG_Signal);
	NL_Analysis_Form->reset_and_clear_displays();
	NL_Analysis_Form->ShowModal();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Deletedatapoint2Click(TObject *Sender)
{
	Deletedatapoint1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Anterior1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].propagate_region(
		Segments_Info.get_segment_id(LV_ANTERIOR_SEGMENT_NAME),
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0]);

	repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Lateral1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].propagate_region(
		Segments_Info.get_segment_id(LV_LATERAL_SEGMENT_NAME),STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0]);

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Septal1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range())
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].propagate_region(
		Segments_Info.get_segment_id(LV_SEPTAL_SEGMENT_NAME),STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0]);

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Apes1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range())
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Apex_Node_Ptr =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();

	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers = true;

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::SeptalAnteriorjunctionbase1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Septal_Anterior_Node_Ptr =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();

	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers = true;

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::SeptalLateraljunctionbase1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range())
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Septal_Lateral_Node_Ptr =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();

	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers = true;

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::AnteriorLateraljunctionbase1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Anterior_Lateral_Node_Ptr =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();

	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers = true;

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ApextoSeptalAnteriorintermediatepoint1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Ap_SA_Intermediate =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();

	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers = true;

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ApextoSeptalLateralintermediatepoint1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Ap_SL_Intermediate =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();

	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers = true;

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ApextoAnteriorLateralintermediatepoint1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range()  )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Ap_AL_Intermediate =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();

	OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_17_LV_Markers = true;

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Anteriorbaselineintermediate1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range()  )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Ant_Intermediate =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();
	repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Septalbaselineintermediate1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range()  )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Sept_Intermediate =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();
	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Lateralbaselineintermediate1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range()  )
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].Lat_Intermediate =
		STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_paths_for_4_point_LV_segmentation();
	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------

int TMain_Application_Window::get_current_segment_id()
{
	int ma_id = Segment_No_ComboBox->ItemIndex;
	int ptr = 0;

	for(long i=0;i<(signed)Segments_Info.Segments.size();i++)
	if( Segments_Info.Segments[i].Name == Segment_No_ComboBox->Items[0][ma_id] )
	ptr = i;

	if( ptr < (signed)Segments_Info.Segments.size() )
	return Segments_Info.Segments[ptr].Code;
	else
	return -1;
}

//---------------------------------------------------------------------------

AnsiString TMain_Application_Window::get_current_segment_name()
{
	return Segment_No_ComboBox->Items[0][Segment_No_ComboBox->ItemIndex];
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Perform17nuclearsegmentation1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range()  )
	{
		STUDY->perform_17_nuclear_segmentation(Progress_Form,STUDY->Current_Surface,&Segments_Info);

		update_controls_state();
		repaint_3D_panels();

		Progress_Form->add_text("Marking 17 segments completed...");
		Progress_Form->Hide();
		Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Images_StringGridClick(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   {

	save_state_of_GUI_controls();

	if( Limit_Image_Display_Bottom_CheckBox->State == cbChecked )
	{
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Limit_Images_Display_Bottom = true;
		double x,y,z;
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.ijk_to_xyz(
			1,1,STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image-1,
			&x,&y,&z);
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Y_Cut = y;
	}

	if( Limit_Image_Display_Top_CheckBox->State == cbChecked )
	{
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Limit_Images_Display_Top = true;
		double x,y,z;
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.ijk_to_xyz(
			1,1,STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image+1,
			&x,&y,&z);
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Y_Cut = y;
	}

	draw_fluoro_image_2D();
	repaint_3D_panels();
   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Images_StringGridDblClick(TObject *Sender)
{
	Images_StringGridClick(this);
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Images_StringGridMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Images_StringGridClick(this);
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Setslicethickness1Click(TObject *Sender)

{
   if( STUDY->is_current_surface_in_range() )
   {
	Ask_For_Single_Item_Form1->Caption = "Slice thickness [mm]: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "4";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{
		double Th = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

		long Im = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image;

		if( Im >= 0 && Im < STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size())
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Im].Slice_Thickness = Th;

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.update_data_set();

		update_images_table();

		repaint_3D_panels();
	}
   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Data_Points_List_StringGridDblClick(TObject *Sender)

{
	Data_Points_List_StringGridClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Setthicknessofallslices1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   {
	Ask_For_Single_Item_Form1->Caption = "Slice thickness [mm]: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "2";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{
		double Th = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

		long Im = STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image;

		for(int Im=0;Im<STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List.size();Im++)
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Images_2D_List[Im].Slice_Thickness = Th;

		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.update_data_set();

		update_images_table();

		repaint_3D_panels();
	}
   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Display_Images_In_3D_Panel_CheckBoxClick(TObject *Sender)
{
	if( Display_Images_In_3D_Panel_CheckBox->State == cbChecked )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Images_In_3D_Panel = true;
	else
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Images_In_3D_Panel = false;

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Limit_Image_Display_Bottom_CheckBoxClick(TObject *Sender)
{
	if( Limit_Image_Display_Bottom_CheckBox->State == cbChecked )
	{
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Limit_Images_Display_Bottom = true;
		double x,y,z;
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.ijk_to_xyz(
			1,1,STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image-1,
			&x,&y,&z);
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Y_Cut = y;
	}
	else
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Limit_Images_Display_Bottom = false;

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Limit_Image_Display_Top_CheckBoxClick(TObject *Sender)

{
	if( Limit_Image_Display_Top_CheckBox->State == cbChecked )
	{
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Limit_Images_Display_Top = true;
		double x,y,z;
		STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.ijk_to_xyz(
			1,1,STUDY->Surfaces_List[STUDY->Current_Surface].Images_Set.Current_Fluoro_Image+1,
			&x,&y,&z);
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Y_Cut = y;
	}
	else
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Limit_Images_Display_Top = false;

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Remove_blue_background_CheckBoxClick(TObject *Sender)
{
	if( Remove_blue_background_CheckBox->State == cbChecked )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Remove_Blue_Images_Background = true;
	else
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Remove_Blue_Images_Background = false;

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Remove_green_background_CheckBoxClick(TObject *Sender)
{
	if( Remove_green_background_CheckBox->State == cbChecked )
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Remove_Green_Images_Background = true;
	else
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Remove_Green_Images_Background = false;

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Display_Surface_CheckBoxClick(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range()  )
	{
	if( Display_Surface_CheckBox->State == cbChecked )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Display_Geometry_Flag = true;
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Data_Points = true;
	}
	else
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Display_Geometry_Flag = false;
		OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.Display_Data_Points = false;
	}

	repaint_3D_panels();
	}

}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Recalculateannotation1Click(TObject *Sender)
{
	Progress_Form->add_text("Updating map...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Annotation_Box.Reference_Annotation_Dragged = true;
	update_LAT_annotation_of_current_map();
	Application->ProcessMessages();

	update_display_of_LAT_Bip_maps();
	Application->ProcessMessages();

	Annotation_Box.Reference_Annotation_Dragged = false;

	Progress_Form->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ExportcurrentmapasSTLfileegfor3Dprinting1Click(TObject *Sender)
{
	SaveDialog->Title = "Select file name";
	SaveDialog->FilterIndex = 5;

	if( STUDY->is_current_surface_in_range() )
	if( SaveDialog->Execute() )
	{

	Progress_Form->clear();
	Progress_Form->add_text("File selected for 'stl' export: " + SaveDialog->FileName);
	Progress_Form->Show();
	Application->ProcessMessages();

	bool Result = Data_IO_Object.export_geo_as_stl_file(SaveDialog->FileName,
						&STUDY->Surfaces_List[STUDY->Current_Surface],0,&Segments_Info);
	if( Result )
	{
		Progress_Form->add_text("'stl' file saved.");
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	repaint_3D_panels();
	Progress_Form->Hide();

   }
}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Createtentingpoint1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{

	Ask_For_Single_Item_Form1->Caption = "Tenging point height? [mm]";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "5";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{
		double H = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

		long n0 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].x +=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].normal[0]*H;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].y +=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].normal[1]*H;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].z +=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].normal[2]*H;

		STUDY->Surfaces_List[STUDY->Current_Surface].calculate_normals();
		repaint_3D_panels();
	}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Createconichole1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{

	Ask_For_Single_Item_Form1->Caption = "Hole depth? [mm]";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "5";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{
		double H = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

		long n0 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];
		long n1 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];
		long n2 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];

		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].x -=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].normal[0]*H;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].y -=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].normal[1]*H;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].z -=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].normal[2]*H;

		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].x -=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].normal[0]*H;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].y -=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].normal[1]*H;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].z -=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].normal[2]*H;

		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].x -=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].normal[0]*H;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].y -=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].normal[1]*H;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].z -=
			STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].normal[2]*H;

		STUDY->Surfaces_List[STUDY->Current_Surface].calculate_normals();
		repaint_3D_panels();
	}

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Create1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{

	Ask_For_Single_Item_Form1->Caption = "Depression depth? [mm]";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "5";
	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Depth = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	double cx,cy,cz;

	Ask_For_Single_Item_Form1->Caption = "Depression radius? [mm]";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "5";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Radius = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	long n0 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];
	long n1 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[1];
	long n2 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[2];

	cx = ( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].x +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].x +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].x ) / 3.0;
	cy = ( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].y +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].y +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].y ) / 3.0;
	cz = ( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].z +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].z +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].z ) / 3.0;

	STUDY->Surfaces_List[STUDY->Current_Surface].create_deflection_in_surface(
		cx,cy,cz,Radius,Depth,1); // 1: uniform depression

	repaint_3D_panels();

	}

	}
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Createuniformelevation1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{

	Ask_For_Single_Item_Form1->Caption = "Elevation height? [mm]";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "5";
	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Depth = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	double cx,cy,cz;

	Ask_For_Single_Item_Form1->Caption = "Elevation radius? [mm]";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "5";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Radius = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	long n0 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[0];
	long n1 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[1];
	long n2 = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
			STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Nodes[2];

	cx = ( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].x +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].x +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].x ) / 3.0;
	cy = ( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].y +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].y +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].y ) / 3.0;
	cz = ( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n0].z +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n1].z +
		   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[n2].z ) / 3.0;

	STUDY->Surfaces_List[STUDY->Current_Surface].create_deflection_in_surface(
		cx,cy,cz,Radius,Depth,2); // 2: uniform elevation

	repaint_3D_panels();

	}

	}
	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Upsamplemesh1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{
		int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

		STUDY->Surfaces_List[STUDY->Current_Surface].upsample_mesh();

		calculate_closest_nodes_and_data_points_pointers();

		OpenGL_Panel_1.prepare_colors_for_display();

		repaint_3D_panels();
	}

}
//---------------------------------------------------------------------------
void __fastcall TMain_Application_Window::Addwheelgeometry1Click(TObject *Sender)
{
	Ask_For_Single_Item_Form1->Caption = "Radius [mesh nodes/spacing = 1]";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "50";
	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Radius = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	Surface_Class Suface_Class_Item;
	STUDY->Surfaces_List.push_back(Suface_Class_Item);
	STUDY->Current_Surface = STUDY->Surfaces_List.size()-1; // *********************
	STUDY->Surfaces_List[STUDY->Current_Surface].generate_wheel_sample(Progress_Form,Radius,1);

	update_controls_state();
	repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Addsquaresample1Click(TObject *Sender)
{
	Ask_For_Single_Item_Form1->Caption = "Size? [mesh nodes/spacing = 1]";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "50";
	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Size = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	Surface_Class Suface_Class_Item;
	STUDY->Surfaces_List.push_back(Suface_Class_Item);
	STUDY->Current_Surface = STUDY->Surfaces_List.size()-1; // *********************
	STUDY->Surfaces_List[STUDY->Current_Surface].generate_square_sample(Progress_Form,Size,Size,1);

	update_controls_state();
	repaint_3D_panels();
	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem19Click(TObject *Sender)
{
	ShowMessage("Feature in development");

//    save_backup_study();
/*
	// delete current value

	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name.Length() > 0 )
	if( MessageDlg("Are you sure you want to delete this value?",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.remove_value(
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name );

	// tu update nodes i data points zrob

	update_controls_state();

	}
*/
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem12Click(TObject *Sender)
{
	// change name of current value

	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name.Length() > 0 )
	{

	Ask_For_Single_Item_Form1->Caption = "Change value name";
	Ask_For_Single_Item_Form1->Edit_Box->Visible = true;
	Ask_For_Single_Item_Form1->Edit_Box->Text =
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.change_value_name(
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name,
		Ask_For_Single_Item_Form1->Edit_Box->Text);

	Displayed_Value_Selection_ComboBox_1Change(this);

	}

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::MenuItem17Click(TObject *Sender)
{
//    save_backup_study();

	// clone current value

	if( STUDY->is_current_surface_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name.Length() > 0 )
	{

	int ptr0 = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_ptr();
	int ptr1 = ptr0 + 1;

	std::vector<Value_Description_Class>::iterator theIterator =
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table.begin();

	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table.insert(
	theIterator+ptr0, 1, STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table[ptr0] );

	AnsiString Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

	// case of 1 digit
	if( Name.SubString(Name.Length()-1,1) == ".")
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table[ptr1].Value_Name =
		Name.SubString(0,Name.Length()-2)+"."+
		IntToStr( Name.SubString(Name.Length()-0,1).ToInt()+1);
	else
	// case of 2 digits
	if( Name.SubString(Name.Length()-2,1) == ".")
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table[ptr1].Value_Name =
		Name.SubString(0,Name.Length()-3)+"."+
		IntToStr( Name.SubString(Name.Length()-1,2).ToInt()+1);
	else
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table[ptr1].Value_Name = Name+".1";

	// copy nodes values
	double v;

	// shift existing values
	for(long Set=0; Set<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size(); Set++)
	for(long i=0; i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size(); i++)
	for(int Val=STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_number()-1; Val>ptr1; Val--)
	{
	v = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].get_value(Set,Val-1);
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].set_value(Set,Val,v);
	}

	// copy added values to new set
	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();i++)
	for(long Set=0; Set<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size(); Set++)
//	for(int Val=0;Val<STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_number();Val++)
	{
	v = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].get_value(Set,ptr0);
	STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].set_value(Set,ptr1,v);
	}

	update_controls_state();

	ShowMessage("Name of cloned value: '" + STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Values_Table[ptr1].Value_Name +"'");

	}

}
//---------------------------------------------------------------------------

void TMain_Application_Window::FULL_MAP_VALUES_UPDATE(int Surface_Ptr)
{
	// call to recalculate all map values

	if( Surface_Ptr >= 0 && Surface_Ptr < STUDY->Surfaces_List.size() )
	if( STUDY->Surfaces_List[Surface_Ptr].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[Surface_Ptr].Current_Data_Point_Set_Ptr;
/*
	STUDY->Surfaces_List[Surface_Ptr].calculate_closest_data_point_to_each_node(
		dset,STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm);

	STUDY->Surfaces_List[Surface_Ptr].calculate_closest_node_of_each_data_point(
		dset,STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm);
*/
	STUDY->Surfaces_List[Surface_Ptr].calculate_closest_nodes_and_data_points_ALL(dset);

	STUDY->Surfaces_List[Surface_Ptr].interpolate_all_values(0,dset,Progress_Form);

	OpenGL_Panel_1.prepare_colors_for_display();

	}

}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Changedatapointtype1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int DP = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

/*
#define CARTO_POINT_TYPE_NORMAL 0
#define CARTO_POINT_TYPE_LOCATION_ONLY 1
#define CARTO_POINT_TYPE_SCAR 2
#define CARTO_POINT_TYPE_FLOATING 3
#define CARTO_POINT_TYPE_TE 4
*/
	vector <AnsiString> Types_Names;
	Types_Names.push_back("Normal");
	Types_Names.push_back("Location only");
	Types_Names.push_back("Scar");
	Types_Names.push_back("Floating");
	Types_Names.push_back("Transient Event");

	Selection_List_Form->Caption = "Select data point type";
	Selection_List_Form->fill_selection(&Types_Names);

	Selection_List_Form->Extra_Button->Visible = false;
	Selection_List_Form->Extra_Button_2->Visible = false;

	if( Selection_List_Form->ShowModal() )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[DS].Data_Points[DP].Type =
		Selection_List_Form->Selected_Item_Ptr;

	FULL_MAP_VALUES_UPDATE(STUDY->Current_Surface);

	update_controls_state();
	repaint_all_controls();
	}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Changedatapointtype2Click(TObject *Sender)

{
	Changedatapointtype1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::CalculateAFCLmapfrom180msSinusoidalRecomposition1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{

	Ask_For_Single_Item_Form1->Caption = "Base cycle length [ms]";
	Ask_For_Single_Item_Form1->Edit_Box->Visible = true;
	Ask_For_Single_Item_Form1->Edit_Box->Text = "180";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Base_AFCL_ms = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	int DS = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	//-----------------------------------------------------------------------------------------
	AnsiString New_Map_Name = STUDY->Surfaces_List[STUDY->Current_Surface].
		calculate_AFCL_map(DS,Base_AFCL_ms,&STUDY->Comp_Module);
	//-----------------------------------------------------------------------------------------

	// set current value to AFCL
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.set_current_value_according_to_name(New_Map_Name);

	STUDY->compute_min_max_values();

	OpenGL_Panel_1.prepare_colors_for_display();
	update_controls_state();
	Progress_Form->Hide();
	repaint_3D_panels();

	} // if ok

	} // if in range
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Importgeometrysegmentation1Click(TObject *Sender)
{
	OpenDialog->Title = "Import segmentation";
	OpenDialog->FilterIndex = 0;
	OpenDialog->FileName = STUDY->Patient_Data.Patient_FirstName+
		STUDY->Patient_Data.Patient_LastName+"_segmentation.csv";
	int Segment_Id;

	if( OpenDialog->Execute() )
	{

	ifstream df;
	df.open( OpenDialog->FileName.c_str());


	for(long t=0;t<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();t++)
	{
	df >> Segment_Id;

	STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set[t].Segment_Id = Segment_Id;
	}

	df.close();

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(
			Segments_Info.Segments.size());

	}
	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Exportgeometrysegmentation1Click(TObject *Sender)

{
	SaveDialog->Title = "Export segmentation";
	SaveDialog->FilterIndex = 0;
	SaveDialog->FileName = STUDY->Patient_Data.Patient_FirstName+
        STUDY->Patient_Data.Patient_LastName+"_segmentation.csv";

	if( SaveDialog->Execute() )
	{

    ofstream df;
    df.open( SaveDialog->FileName.c_str());

	for(long t=0;t<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].
		Surface_Triangle_Set.size();t++)
	df << endl << STUDY->Surfaces_List[STUDY->Current_Surface].
        Surface_Triangle_Set[t].Segment_Id;

	df.close();

	}
	repaint_3D_panels();

}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::CenterALLgeometriessimultaneously1Click(TObject *Sender)
{
	STUDY->center_geometries(0,STUDY->Surfaces_List.size()-1);
	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Undocentering1Click(TObject *Sender)
{
	if( STUDY->Surfaces_List.size() > 0 )
	{

	double mx = STUDY->Surfaces_List[0].Translation_Vector_X;
	double my = STUDY->Surfaces_List[0].Translation_Vector_Y;
	double mz = STUDY->Surfaces_List[0].Translation_Vector_Z;

	// Translate ALL geos
	for(long S=0;S<STUDY->Surfaces_List.size();S++)
	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();i++)
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].x -= mx;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].y -= my;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[i].z -= mz;
	}

	// translate all data points in all sets
	for(long S=0;S<STUDY->Surfaces_List.size();S++)
	for(long dset=0;dset<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points.size();i++)
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[i].x -= mx;
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[i].y -= my;
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[i].z -= mz;
	}

	for(long S=0;S<STUDY->Surfaces_List.size();S++)
	for(long dset=0;dset<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size();dset++)
	for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Ablation_Points_List.size();i++)
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Ablation_Points_List[i].x -= mx;
		STUDY->Surfaces_List[STUDY->Current_Surface].Ablation_Points_List[i].y -= my;
		STUDY->Surfaces_List[STUDY->Current_Surface].Ablation_Points_List[i].z -= mz;
	}

	// center ABC spatial registration points
	for(long S=0;S<STUDY->Surfaces_List.size();S++)
	{
	STUDY->Surfaces_List[STUDY->Current_Surface].translate_ABC_spatial_registration_markers(mx,my,mz);
	STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(Segments_Info.Segments.size());
	}

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------





void __fastcall TMain_Application_Window::Zoom_Slider_ImageMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	// Init Mouse Position On Button Click
	Zoom_Mouse_X = X;
	Zoom_Mouse_Y = Y;

   Zoom_MouseButton = Button;
   Zoom_MouseStatus = MB_DOWN;

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Zoom_Slider_ImageMouseMove(TObject *Sender,
		  TShiftState Shift, int X, int Y)
{
	if( Zoom_MouseStatus == MB_DOWN )
	{
		if (Y > Zoom_Mouse_Y )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor +=
				(GLdouble)(Y-Zoom_Mouse_Y) * (GLdouble) 1.0;
		if (Y < Zoom_Mouse_Y )
			OpenGL_Panel_1.OpenGL_Panel_Display_Parameters.ZoomFactor -=
				(GLdouble)(Zoom_Mouse_Y-Y) * (GLdouble) 1.0;

		Zoom_Mouse_X = X;
		Zoom_Mouse_Y = Y;

		OpenGL_Panel_1.map_display_panels_paint();

	} // zoom
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Zoom_Slider_ImageMouseUp(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Zoom_MouseButton = Button;
	Zoom_MouseStatus = MB_UP;

}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::Geometrystatistics1Click(TObject *Sender)
{
	ShowMessage("Number of nodes= " +
		IntToStr((int)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size())+
		"   Number of trinagles= " +
		IntToStr((int)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size())+
		"   Area[cm2]= " +
		FloatToStrF(STUDY->Surfaces_List[STUDY->Current_Surface].get_total_area(),ffGeneral,3,2 )+
		"   Volume[cm3]= " +
		FloatToStrF(STUDY->Surfaces_List[STUDY->Current_Surface].get_volume(),ffGeneral,3,2 ) );
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ImportALLmapsfromCARTOexportfolder1Click(TObject *Sender)

{
   bool Append = perform_pre_import_queries();
   bool Geoff_Export_Flag = false;
//   ShowMessage("Select folder with *.mesh, *.car and electrogram files");

   if( SelectDirectory("Select data directory", "", Data_IO_Object.Data_Files_Path) )
   {
	Progress_Form->clear();
	Progress_Form->add_text("Folder selected: " + Data_IO_Object.Data_Files_Path);
	Progress_Form->add_text("Parsing file names...");
	Progress_Form->Show();
	Application->ProcessMessages();

	Data_FileListBox->Clear();
	Data_FileListBox->ApplyFilePath( Data_IO_Object.Data_Files_Path );

	if( Data_IO_Object.Data_Files_Path.Length() > 0 )
	{
		AnsiString Result = Data_IO_Object.
			import_CARTO_folder(Data_FileListBox,STUDY,Progress_Form,true,false,false,true,Append,Geoff_Export_Flag);

		if( Result == "Import completed" )
		{

		STUDY->Current_Surface = 0;

		Progress_Form->add_text("Initialization...");
		Application->ProcessMessages();
		Progress_Form->Show();
		post_import_initialization(STUDY->Surfaces_List.size()-1,STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm,true);

		update_controls_state();

		if( MessageDlg("Center maps?",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
		CenterALLgeometriessimultaneously1Click(this);

		Progress_Form->add_text("Import completed. Press CLOSE to continue." );
		Application->ProcessMessages();
		Progress_Form->ShowModal();

		OpenGL_Panel_1.set_initial_zoom_in_3D_panel();

		repaint_3D_panels();
		}
		else
		{
			Progress_Form->Hide();
			ShowMessage(Result);
		}

   } // folder path size > 0
   else
   ShowMessage("Invalid folder selected");

   }

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Exportvaluesatdatapoints1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( SaveDialog->Execute() )
	{

	AnsiString AS = Utils.remove_substring_from_string(SaveDialog->FileName,".csv") + ".csv";
	ofstream df( AS.c_str());

	Data_IO_Object.save_values_at_data_points(AS,STUDY);

	ShowMessage("Done");

   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Remove_Surface_Segment_ButtonClick(TObject *Sender)
{
   int Seg_Id = get_current_segment_id();

   if( STUDY->is_current_surface_in_range() )
   {
   for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();i++)
   if( STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[i].Segment_Id == Seg_Id )
   {
	   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[i].Segment_Id = -1;
	   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[i].Removed_Flag = ON;
   }

	STUDY->Surfaces_List[STUDY->Current_Surface].compute_segments_centers(Segments_Info.Segments.size());
   }

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Restoreremovedsurface1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   {
   for(long i=0;i<(signed)STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set.size();i++)
	   STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[i].Removed_Flag = OFF;
   }

	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Smoothmapgeometry1stepofaveragereplacement1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range()  )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].smooth_map_geometry();
		STUDY->Surfaces_List[STUDY->Current_Surface].calculate_normals();

		repaint_3D_panels();
	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Smoothmapgeometrymultiplestepswithvolumecorrection1Click(TObject *Sender)

{
	double Volume,Volume2,Volume3;
	if( STUDY->is_current_surface_in_range() )
	{

	Ask_For_Single_Item_Form1->Caption = "How many steps of smoothing? ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "20";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	int Steps_No = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

	double Initial_Volume = STUDY->Surfaces_List[STUDY->Current_Surface].get_volume();

	for(int step=0;step<Steps_No;step++)
	{
		Volume = STUDY->Surfaces_List[STUDY->Current_Surface].get_volume();

		STUDY->Surfaces_List[STUDY->Current_Surface].smooth_map_geometry();

		Volume2 = STUDY->Surfaces_List[STUDY->Current_Surface].get_volume();

		STUDY->Surfaces_List[STUDY->Current_Surface].rescale_geometry_to_reach_specified_volume(Initial_Volume);

		Volume3 = STUDY->Surfaces_List[STUDY->Current_Surface].get_volume();

		STUDY->Surfaces_List[STUDY->Current_Surface].calculate_normals();

		repaint_3D_panels();

		Application->ProcessMessages();
	}

	ShowMessage("Smoothing done.");

	if( MessageDlg("Force data points position onto the map surface?",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	Forcedatapointsontosurface1Click(this);

	}

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Restoreoriginalgeometry1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range()  )
	{

	for(unsigned long j=0;j<STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set.size();j++)
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[j].x = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[j].Original_x;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[j].y = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[j].Original_y;
		STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[j].z = STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Node_Set[j].Original_z;
	}


	STUDY->Surfaces_List[STUDY->Current_Surface].calculate_normals();

	repaint_3D_panels();

	}
}
//---------------------------------------------------------------------------




void __fastcall TMain_Application_Window::Createnewmap1Click(TObject *Sender)
{
	Ask_For_Single_Item_Form1->Caption = "Map name: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "New Map";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{
		// 1. Add new surface object
		STUDY->add_empty_surface( Ask_For_Single_Item_Form1->Edit_Box->Text );
		STUDY->Current_Surface = STUDY->Surfaces_List.size() - 1;

		/*
		Point_3D P3;
		randomize();
		for(int i=0;i<40000;i++)
		{
			P3.x = rand()%100;
			P3.y = rand()%100;
			P3.z = rand()%100;

			STUDY->Surfaces_List[STUDY->Current_Surface].Geo_3D_Point_Cloud.push_back(P3);
		}
		*/

		update_controls_state();
		repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------




void __fastcall TMain_Application_Window::Sortvaluesascending1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range()  )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	AnsiString Current_Value_Name = get_value_name_from_selection_form();

	if( Current_Value_Name != "" )
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].sort_data_points(
		Current_Value_Name,
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref(),false);

	update_controls_state();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Sortvaluesdescending1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range()  )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;

	AnsiString Current_Value_Name = get_value_name_from_selection_form();

	if( Current_Value_Name != "" )
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].sort_data_points(
		Current_Value_Name,
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref(),true);

	update_controls_state();

	}
}
//---------------------------------------------------------------------------

AnsiString TMain_Application_Window::get_value_name_from_selection_form()
{
	if( STUDY->is_current_surface_in_range()  )
	{

	Selection_List_Form->Caption = "Select value to use";
	Selection_List_Form->Extra_Button->Visible = false;
	Selection_List_Form->Extra_Button_2->Visible = false;

	Labels_Names.clear();
	int Values_Number = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_number();
	for(int k=0;k<Values_Number;k++)
	Labels_Names.push_back(
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_name_according_to_ptr(k) );

	Selection_List_Form->fill_selection(&Labels_Names);

	if( Selection_List_Form->ShowModal() )
	{

	save_labels_list("Labels_List.txt");

	if( Selection_List_Form->Selected_Item_Ptr >= 0 )
	return Labels_Names[Selection_List_Form->Selected_Item_Ptr];
	else
	return "";

	}
	else
	return "";
	}
}

//---------------------------------------------------------------------------



void __fastcall TMain_Application_Window::Getdvdtslope1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range()  )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	if( STUDY->Surfaces_List[STUDY->Current_Surface].data_point_ptr_in_range() )
	{
		int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
		int dp = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

		double v,Catheter_Sampling = STUDY->Surfaces_List[STUDY->Current_Surface].
			Data_Point_Set[dset].Data_Points[dp].Roving_Signal.Time_Step_ms;

		long Begin = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			Data_Points[dp].Ref_Signal_Activation_ptr +
			STUDY->Comp_Module.ROV_LAT_Annotation_Left_Edge_ms/Catheter_Sampling;

		long End = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			Data_Points[dp].Ref_Signal_Activation_ptr +
			STUDY->Comp_Module.ROV_LAT_Annotation_Left_Edge_ms/Catheter_Sampling;

		double Max_slope,Min_slope;

		Rich_Edit_Form->clear_text();
		Rich_Edit_Form->add_text("Roving egm: ");

		Rich_Edit_Form->add_text("Within annotation gray box: ");
		PNUM.calculate_max_min_mean_slope_vec_ranged(
				&STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Voltage_Values,
				Begin,End,&Max_slope, &Min_slope);
		Rich_Edit_Form->add_text("Max +dv/dt = " + FloatToStrF(Max_slope/Catheter_Sampling,ffGeneral,3,2));
		Rich_Edit_Form->add_text("Max -dv/dt = " + FloatToStrF(Min_slope/Catheter_Sampling,ffGeneral,3,2));

		Rich_Edit_Form->add_text("Within whole egm duration: ");
		PNUM.calculate_max_min_mean_slope_vec_ranged(
				&STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Voltage_Values,
				0,STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Roving_Signal.Voltage_Values.size(),
				&Max_slope, &Min_slope);
		Rich_Edit_Form->add_text("Max +dv/dt = " + FloatToStrF(Max_slope/Catheter_Sampling,ffGeneral,3,2));
		Rich_Edit_Form->add_text("Max -dv/dt = " + FloatToStrF(Min_slope/Catheter_Sampling,ffGeneral,3,2));

		Rich_Edit_Form->add_text("");
		Rich_Edit_Form->add_text("Reference egm: ");

		Rich_Edit_Form->add_text("Within annotation gray box: ");
		PNUM.calculate_max_min_mean_slope_vec_ranged(
				&STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Voltage_Values,
				Begin,End,&Max_slope, &Min_slope);
		Rich_Edit_Form->add_text("Max +dv/dt = " + FloatToStrF(Max_slope/Catheter_Sampling,ffGeneral,3,2));
		Rich_Edit_Form->add_text("Max -dv/dt = " + FloatToStrF(Min_slope/Catheter_Sampling,ffGeneral,3,2));

		Rich_Edit_Form->add_text("Within whole egm duration: ");
		PNUM.calculate_max_min_mean_slope_vec_ranged(
				&STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Voltage_Values,
				0,STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
				Data_Points[dp].Reference_Signal.Voltage_Values.size(),
				&Max_slope, &Min_slope);
		Rich_Edit_Form->add_text("Max +dv/dt = " + FloatToStrF(Max_slope/Catheter_Sampling,ffGeneral,3,2));
		Rich_Edit_Form->add_text("Max -dv/dt = " + FloatToStrF(Min_slope/Catheter_Sampling,ffGeneral,3,2));

		Rich_Edit_Form->ShowModal();
	}
}

void __fastcall TMain_Application_Window::Set_Sample_ButtonClick(TObject *Sender)
{
	for(int S=0;S<STUDY->Surfaces_List.size();S++)
	{
		STUDY->Surfaces_List[S].Map_Values.Current_Map_Value_Name = "Instantaneous phase";
		Global_Movie_TimePoint_ptr = SampleValue_Edit->Text.ToDouble();
		STUDY->Surfaces_List[S].set_phase_value_of_map_to_specific_time_point(Global_Movie_TimePoint_ptr);
	}

	update_controls_state();
	STUDY->compute_min_max_values();
	OpenGL_Panel_1.prepare_colors_for_display();
	repaint_3D_panels();
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Phase_Movie_Mode_CheckBoxClick(TObject *Sender)
{
	if( Phase_Movie_Mode_CheckBox->State == cbChecked &&
		STUDY->Surfaces_List[0].Map_Values.get_value_ptr("Instantaneous phase") < 0 )
	{

	for(int S=0;S<STUDY->Surfaces_List.size();S++)
	{
	// just in case its not present, add value to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS;
	Value_Desc.Displayed_In_Table = true;
	Value_Desc.Value_Name = "Instantaneous phase";
	Value_Desc.Interpolation_Type = 0; // no interpolation
	Value_Desc.Unit = "";
	Value_Desc.Fixed_Palette = true;
	Value_Desc.Min_Value_On_Map = -3.14;
	Value_Desc.Max_Value_On_Map =  3.14;

	STUDY->Surfaces_List[S].Map_Values.add_value(Value_Desc);

	STUDY->Surfaces_List[S].Map_Values.Current_Map_Value_Name = "Instantaneous phase";
	STUDY->compute_min_max_values();

	}

	update_controls_state();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Movie_Start_ButtonClick(TObject *Sender)
{
	Phase_Movie_Flag = true;

	while( Phase_Movie_Flag )
	{

	for(int S=0;S<STUDY->Surfaces_List.size();S++)
	{

	Global_Movie_TimePoint_ptr += Movie_Speed_Edit->Text.ToInt();
	int DS = STUDY->Surfaces_List[S].Current_Data_Point_Set_Ptr;
	double Size = STUDY->Surfaces_List[S].Data_Point_Set[0].Data_Points[0].Roving_Signal.Phase.size();
	Signals_ScrollBar->Position = Global_Movie_TimePoint_ptr / Size *
		Signals_ScrollBar->Max;

	STUDY->Surfaces_List[S].
		set_phase_value_of_map_to_specific_time_point(Global_Movie_TimePoint_ptr);

	STUDY->Surfaces_List[S].Map_Values.Current_Map_Value_Name = "Instantaneous phase";

	}

	STUDY->compute_min_max_values();
	OpenGL_Panel_1.prepare_colors_for_display();
	repaint_3D_panels();
	Application->ProcessMessages();

	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Movie_Stop_ButtonClick(TObject *Sender)
{
	Phase_Movie_Flag = false;
}
//---------------------------------------------------------------------------







void __fastcall TMain_Application_Window::Removedatapointsbelowtop1Click(TObject *Sender)

{
	if( STUDY->is_current_surface_in_range() )
	if(STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	Ask_For_Single_Item_Form1->Caption = "% thresohld: (e.g. to leave top 10% put 90)";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "90";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Threshold_Perc = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	long D;
	AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

	Progress_Form->add_text("Removing data points with value below % threshold... ");
	Progress_Form->Show();
	Application->ProcessMessages();

	// find min max value
	double minv,maxv;
	STUDY->compute_min_max_values();
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_minmax(&minv,&maxv);
	double Threshold_Value = minv + Threshold_Perc/100.*(maxv-minv);

	D = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
		delete_data_points_with_value_lower_than(Current_Value_Name,Threshold_Value,
			STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_values_table_ref());

	Progress_Form->add_text(IntToStr((int)D) + " data points removed...");
	Progress_Form->Show();
	Application->ProcessMessages();

	if( D>0 )
	calculate_closest_nodes_and_data_points_pointers();

	Data_Point_Set_Selection_ComboBoxChange(this);
	Displayed_Value_Selection_ComboBox_1Change(this);

	}

	}
}
//---------------------------------------------------------------------------








void __fastcall TMain_Application_Window::Getmicrofractionation1Click(TObject *Sender)

{
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int dp = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	ShowMessage(FloatToStr(
	PNUM.get_micro_fractionation(
		&STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Data_Points[dp].
		Roving_Signal.Voltage_Values,
		STUDY->Comp_Module.Microfractionation_Algorithm,
		STUDY->Comp_Module.Microfractionation_percentile_threshold,
		50./0.5 ) ));
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Displayrovingsignal1Click(TObject *Sender)

{
    DisplaylocalsignalinSignalAnalysiswindow1Click(this);
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Exportgeometryasvtkfile1Click(TObject *Sender)
{
	SaveDialog->Title = "vtk file location and name:";
	SaveDialog->FilterIndex = 2;

	if( STUDY->is_current_surface_in_range() && SaveDialog->Execute() )
	{

	Progress_Form->clear();
	Progress_Form->add_text("Exporting vtk file...");
	Progress_Form->Show();
	Application->ProcessMessages();

	bool Result = Data_IO_Object.export_geo_as_vtk_file(SaveDialog->FileName,
						&STUDY->Surfaces_List[STUDY->Current_Surface],false);
	if( Result )
	{
		Progress_Form->add_text("'vtk' file saved.");
		Progress_Form->Show();
		Application->ProcessMessages();
	}

	repaint_3D_panels();
	Progress_Form->Hide();

   }

}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::N58Click(TObject *Sender)
{
	ShowMessage( get_current_segment_name() + " area= " + FloatToStrF(
		STUDY->Surfaces_List[STUDY->Current_Surface].
		get_area_of_given_segment_cm2(get_current_segment_id()),ffGeneral,3,2 ) + " cm2" );
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Showhistogramofcurrentvaluespatialgradient1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() &&
	   STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
   {

   Ask_For_Single_Item_Form1->Caption = "Neighborhood range [mm]: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "5";
   Ask_For_Single_Item_Form1->ShowModal();
   double Range_mm = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

   Ask_For_Single_Item_Form1->Caption = "Min value cutoff: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "1";
   Ask_For_Single_Item_Form1->ShowModal();
   double Min_Cutoff = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

   Ask_For_Single_Item_Form1->Caption = "Max value cutoff: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "100";

   if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
   {

   double Max_Cutoff = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

   AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;
   int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
   double Min,Box_Size;

   ShowMessage("Histogram will be based on data points! ");

   std::vector <long> Histogram = STUDY->Surfaces_List[STUDY->Current_Surface].
			get_histogram_of_value_gradient(dset,0,Range_mm,Current_Value_Name,
				&Min,&Box_Size,false,0);

   long DPs_Number_No_Cutoffs = PNUM.get_sum_of_vector(&Histogram);

   // take values within min-max range
   std::vector <long> Histogram2;
   for(int i=0;i<Histogram.size();i++)
   if( Min + i*Box_Size > Min_Cutoff && Min + i*Box_Size < Max_Cutoff )
   Histogram2.push_back(Histogram[i]);

   long DPs_Number_With_Cutoffs = PNUM.get_sum_of_vector(&Histogram2);

   Min = Min_Cutoff;

   // calculate CHI
	double p5 = PNUM.get_percentile_from_histogram(&Histogram2,5,Min,Box_Size);
	double p50 = PNUM.get_percentile_from_histogram(&Histogram2,50,Min,Box_Size);
	double p95 = PNUM.get_percentile_from_histogram(&Histogram2,95,Min,Box_Size);
	double CHI;
	if(p50!=0)
	CHI = (p95-p5)/p50;
	else CHI = -1;

   // plot histogram
	Histogram_Form->Chart1->Series[0]->Clear();
	for(int i=0;i<Histogram2.size();i++)
	Histogram_Form->Chart1->Series[0]->AddXY(Min + i*Box_Size,Histogram2[i]);

	Histogram_Form->RichEdit1->Clear();
	Histogram_Form->RichEdit1->Lines[0].Add("5th percentile= " + FloatToStrF(p5,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("50th percentile= " + FloatToStrF(p50,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("95th percentile= " + FloatToStrF(p95,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("CHI: (p95-p5)/p50 = " + FloatToStrF(CHI,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("Number of data points (before cutoff) = " + FloatToStrF(DPs_Number_No_Cutoffs,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("Number of data points (with cutoff) = " + FloatToStrF(DPs_Number_With_Cutoffs,ffGeneral,3,2));

	Histogram_Form->Chart1->BottomAxis->Title->Text = "LAT range within " + FloatToStr(Range_mm) + " mm radius neighbourhood [ms]";
	Histogram_Form->Chart1->LeftAxis->Title->Text = "Count #";

	Histogram_Form->ShowModal();

   }

   }
}
//---------------------------------------------------------------------------
void __fastcall TMain_Application_Window::ShowhistogramofspatialgradientofcurrentvalueOFCURRENTSEGMENT1Click(TObject *Sender)

{
   if( STUDY->is_current_surface_in_range() &&
	   STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
   {

   Ask_For_Single_Item_Form1->Caption = "Neighborhood range (radius) [mm]: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "5";
   Ask_For_Single_Item_Form1->ShowModal();
   double Range_mm = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

   Ask_For_Single_Item_Form1->Caption = "Min value cutoff: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "1";
   Ask_For_Single_Item_Form1->ShowModal();
   double Min_Cutoff = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

   Ask_For_Single_Item_Form1->Caption = "Max value cutoff: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "100";

   if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
   {

   double Max_Cutoff = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

   AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;
   int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
   double Min,Box_Size;
   int Current_Segment_Id = get_current_segment_id();

   ShowMessage("Histogram will be based on data points! ");

   std::vector <long> Histogram = STUDY->Surfaces_List[STUDY->Current_Surface].
			get_histogram_of_value_gradient(dset,Current_Segment_Id,Range_mm,Current_Value_Name,
				&Min,&Box_Size,false,0);

   long DPs_Number_No_Cutoffs = PNUM.get_sum_of_vector(&Histogram);

   // take values within min-max range
   std::vector <long> Histogram2;
   for(int i=0;i<Histogram.size();i++)
   if( Min + i*Box_Size > Min_Cutoff && Min + i*Box_Size < Max_Cutoff )
   Histogram2.push_back(Histogram[i]);

   long DPs_Number_With_Cutoffs = PNUM.get_sum_of_vector(&Histogram2);

   Min = Min_Cutoff;

   // calculate CHI
	double p5 = PNUM.get_percentile_from_histogram(&Histogram2,5,Min,Box_Size);
	double p50 = PNUM.get_percentile_from_histogram(&Histogram2,50,Min,Box_Size);
	double p95 = PNUM.get_percentile_from_histogram(&Histogram2,95,Min,Box_Size);
	double CHI;
	if(p50!=0)
	CHI = (p95-p5)/p50;
	else CHI = -1;

   // plot histogram
	Histogram_Form->Chart1->Series[0]->Clear();
	for(int i=0;i<Histogram2.size();i++)
	Histogram_Form->Chart1->Series[0]->AddXY(Min + i*Box_Size,Histogram2[i]);

	Histogram_Form->RichEdit1->Clear();
	Histogram_Form->RichEdit1->Lines[0].Add("5th percentile= " + FloatToStrF(p5,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("50th percentile= " + FloatToStrF(p50,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("95th percentile= " + FloatToStrF(p95,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("CHI: (p95-p5)/p50 = " + FloatToStrF(CHI,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("Number of data points (before cutoff) = " + FloatToStrF(DPs_Number_No_Cutoffs,ffGeneral,3,2));
	Histogram_Form->RichEdit1->Lines[0].Add("Number of data points (with cutoff) = " + FloatToStrF(DPs_Number_With_Cutoffs,ffGeneral,3,2));

	Histogram_Form->Chart1->BottomAxis->Title->Text = "LAT range within " + FloatToStr(Range_mm) + " mm radius neighbourhood [ms]";
	Histogram_Form->Chart1->LeftAxis->Title->Text = "Count #";
	Histogram_Form->Chart1->Title->Caption = "Segment: " + get_current_segment_name() + " Radius[mm]=" + FloatToStr(Range_mm);
	Histogram_Form->ShowModal();

   }

   }
}
//---------------------------------------------------------------------------






void __fastcall TMain_Application_Window::Getsegmentwithmaxvalue1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   {
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int Val_Ptr = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_ptr();

	int Min_Segment_Id, Max_Segment_Id;

	STUDY->Surfaces_List[STUDY->Current_Surface].get_segment_id_with_minmax_value(
		dset, Val_Ptr, &Min_Segment_Id, &Max_Segment_Id );

	ShowMessage(Segments_Info.get_segment_name(Max_Segment_Id));

   }
}
//---------------------------------------------------------------------------




void __fastcall TMain_Application_Window::GetareaandareaofcurrentvaluewithinrangePERSEGMENT1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   if( STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
   {

   Ask_For_Single_Item_Form1->Caption = "Threshold: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "0.5";
   Ask_For_Single_Item_Form1->ShowModal();
   double Threshold_Low = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

   Ask_For_Single_Item_Form1->Caption = "Threshold: ";
   Ask_For_Single_Item_Form1->Edit_Box->Text = "5";

   int Current_Value_Ptr = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_current_value_ptr();
   AnsiString Current_Value_Name = STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.Current_Map_Value_Name;

   if( Current_Value_Name == SEGMENTATION_VALUE_NAME )
   ShowMessage("Select other map than segmentation for quantitative analysis");
   else
   if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
   {

   double Threshold_High = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();

   StringGrid_Form->Caption = "Area and %area analysis per segment";

   AnsiString FileName = Utils.get_file_name_from_full_path(STUDY->Study_Current_Path_And_File_Name);
   int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
   double Area,Total_Area;

   StringGrid_Form->StringGrid1->ColCount = 10;

   StringGrid_Form->StringGrid1->Cells[0][0] = "FileName";
   StringGrid_Form->StringGrid1->Cells[1][0] = "Geometry";
   StringGrid_Form->StringGrid1->Cells[2][0] = "DataPointsSetName";
   StringGrid_Form->StringGrid1->Cells[3][0] = "Segment_Name";
   StringGrid_Form->StringGrid1->Cells[4][0] = "Value";
   StringGrid_Form->StringGrid1->Cells[5][0] = "Threshold_Low";
   StringGrid_Form->StringGrid1->Cells[6][0] = "Threshold_High";
   StringGrid_Form->StringGrid1->Cells[7][0] = "Area_within_range[cm^2]";
   StringGrid_Form->StringGrid1->Cells[8][0] = "%Area_within_range_threshold";

   int Segment_Id,Column = 0;

   for(int Seg_Ptr=1;Seg_Ptr<Segments_Info.Segments.size();Seg_Ptr++)
   {

   Segment_Id = Segments_Info.Segments[Seg_Ptr].Code;

   STUDY->Surfaces_List[STUDY->Current_Surface].calculate_area_taken_by_Value_within_range(
		Segment_Id,Current_Value_Ptr,Threshold_Low,Threshold_High,&Area,&Total_Area,dset);

   if( Total_Area != 0 )
   {
	   Column++;
	   StringGrid_Form->StringGrid1->RowCount = 1 + Column;

	   StringGrid_Form->StringGrid1->Cells[0][Column] = FileName;
	   StringGrid_Form->StringGrid1->Cells[1][Column] = STUDY->Surfaces_List[STUDY->Current_Surface].Name;
	   StringGrid_Form->StringGrid1->Cells[2][Column] = STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].Name;
	   StringGrid_Form->StringGrid1->Cells[3][Column] = Segments_Info.Segments[Seg_Ptr].Name;
	   StringGrid_Form->StringGrid1->Cells[4][Column] = Current_Value_Name;
	   StringGrid_Form->StringGrid1->Cells[5][Column] = FloatToStrF(Threshold_Low,ffGeneral,3,2);
	   StringGrid_Form->StringGrid1->Cells[6][Column] = FloatToStrF(Threshold_High,ffGeneral,3,2);
	   StringGrid_Form->StringGrid1->Cells[7][Column] = FloatToStrF(Area,ffGeneral,3,2);
	   StringGrid_Form->StringGrid1->Cells[8][Column] = FloatToStrF(100.0*Area/Total_Area,ffGeneral,3,2);
   }

   } // THROUGH segments

   StringGrid_Form->ShowModal();

   }
   }
}
//---------------------------------------------------------------------------







void __fastcall TMain_Application_Window::Showdatasetinfo1Click(TObject *Sender)
{
	Rich_Edit_Form->clear_text();

	for(int S=0;S<STUDY->Surfaces_List.size();S++)
	{
	Rich_Edit_Form->add_text("\n**************************************************************");
	Rich_Edit_Form->add_text("\n\nSurface geometry: " + STUDY->Surfaces_List[S].Name );

	if( STUDY->Surfaces_List[S].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_UNDEFINED )
	Rich_Edit_Form->add_text("\nOrigin: UNDEFINED");
	if( STUDY->Surfaces_List[S].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_NAVX )
	Rich_Edit_Form->add_text("\nOrigin: NavX");
	if( STUDY->Surfaces_List[S].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_CARTO )
	Rich_Edit_Form->add_text("\nOrigin: CARTO");
	if( STUDY->Surfaces_List[S].Mapping_System_Source == MAPPING_SYSTEM_ORIGIN_RHYTHMIA )
	Rich_Edit_Form->add_text("\nOrigin: RHYTHMIA");

	Rich_Edit_Form->add_text("\nData_Points_Filling_Threshold_mm: " + FloatToStr(
		STUDY->Surfaces_List[S].Data_Points_Filling_Threshold_mm));

	}

	Rich_Edit_Form->add_text("\n\nGENERAL SETTINGS: ");


	Rich_Edit_Form->ShowModal();
}
//---------------------------------------------------------------------------



void __fastcall TMain_Application_Window::N61Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( MessageDlg("Operation will remove existing segmentation, proceed?",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].segment_disjonted_elements();

	// set display value to segmentation
	Displayed_Value_Selection_ComboBox_1->ItemIndex =
		STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.get_value_ptr(SEGMENTATION_VALUE_NAME);
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.set_current_value_according_to_name(SEGMENTATION_VALUE_NAME);

	repaint_3D_panels();

	}
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Leavegeometryrelatedwithonlycurrentlypointedsegment1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle >= 0 )
	{

	// get currently pointed segment id
	int Segment_Id =STUDY->Surfaces_List[STUDY->Current_Surface].Surface_Triangle_Set[
		STUDY->Surfaces_List[STUDY->Current_Surface].Pointed_Triangle].Segment_Id;

	if( Segment_Id > 0 )
	{

	STUDY->Surfaces_List[STUDY->Current_Surface].remove_surface_related_with_specific_segment(Segment_Id);

	// initialize new surface (to recalculate neigs etc.)
	STUDY->Surfaces_List[STUDY->Current_Surface].initialize_surface(Progress_Form);

	repaint_3D_panels();
	}
	}
}

//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::RemovedatapointsnotonmapsurfaceHARDdelete1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if(STUDY->Surfaces_List[STUDY->Current_Surface].data_points_set_ptr_in_range() )
	{

	Ask_For_Single_Item_Form1->Caption = "Max distance from geo node: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "0.1";

	if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
	{

	double Th = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	long D;

	Progress_Form->add_text("Removing 'floating' data points... ");
	Progress_Form->Show();
	Application->ProcessMessages();

	D = STUDY->Surfaces_List[STUDY->Current_Surface].delete_floating_data_points(dset,Th,true);

	Progress_Form->add_text(IntToStr((int)D) + " data points removed...");
	Progress_Form->Show();
	Application->ProcessMessages();

	if( D > 0 )
	calculate_closest_nodes_and_data_points_pointers();

	Data_Point_Set_Selection_ComboBoxChange(this);
	Displayed_Value_Selection_ComboBox_1Change(this);

	Progress_Form->Hide();

	}

	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Smoothnormalswithinspecifiedrange1Click(TObject *Sender)
{
	Ask_For_Single_Item_Form1->Caption = "Range [mm]: ";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "15";
	int Res;

	if( Echo)
	Res = Ask_For_Single_Item_Form1->ShowModal();

	if( Res == mrOk || !Echo )
	{
		double Range = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
		STUDY->Surfaces_List[STUDY->Current_Surface].smooth_normals(Range);
		repaint_3D_panels();
	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Reclaculatenormals1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range()  )
	{
		STUDY->Surfaces_List[STUDY->Current_Surface].calculate_normals();
		repaint_3D_panels();
	}

}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Leaveonly1stmapvisible1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range()  )
	{
	STUDY->Surfaces_List[0].Display_Whole_Dataset_Flag = true;

	for(long i=1;i<STUDY->Surfaces_List.size();i++)
	STUDY->Surfaces_List[i].Display_Whole_Dataset_Flag = false;

	repaint_3D_panels();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ImportablationpointslistLydiacsvformatv21Click(TObject *Sender)
{
	ShowMessage("Assumptions: 'RFSessionNumber' columm B, 'SequenceNumber' column D, 'FTI' column M, xyz columns N,O,P");

	double v;
	int dset;
	char string[2000];
	long TInt;
	Ablation_Point_Class ABL_Point;
	AnsiString AS;
	long Counter;
	ABL_Point.Identifier = -1;

	OpenDialog->Title = "Load file";
	OpenDialog->FilterIndex = 0;
	ifstream df,df1,df2;

	if( OpenDialog->Execute() )
	{

	vector<Ablation_Point_Class> Ablation_Points_List;
	std::vector <Row_vec> Table;

	Table = Utils.load_rectangular_csv_file_to_grid(OpenDialog->FileName);

	for(long row=1;row<(signed)Table.size();row++)
	{
		ABL_Point.Identifier_ID =   Table[row].Elements[1].Trim().ToInt();
		ABL_Point.Identifier_Hash = Table[row].Elements[3].Trim().ToInt();

		ABL_Point.DurationTime = Table[row].Elements[6].Trim().ToDouble();

		if( Table[row].Elements[7].Trim() != "" ) ABL_Point.GeneratorMedianPower_W = Table[row].Elements[7].Trim().ToDouble();
		if( Table[row].Elements[8].Trim() != "" ) ABL_Point.AverageForce = Table[row].Elements[8].Trim().ToDouble();
		if( Table[row].Elements[9].Trim() != "" ) ABL_Point.DirectSenseImpedanceBase_Ohm = Table[row].Elements[9].Trim().ToDouble();
		if( Table[row].Elements[10].Trim() != "" ) ABL_Point.DirectSenseImpedanceMin_Ohm = Table[row].Elements[10].Trim().ToDouble();
		if( Table[row].Elements[11].Trim() != "" ) ABL_Point.DirectSenseImpedanceDrop_Ohm = Table[row].Elements[11].Trim().ToDouble();

		ABL_Point.FTI = Table[row].Elements[12].Trim().ToDouble();

		ABL_Point.x = Table[row].Elements[13].Trim().ToDouble();
		ABL_Point.y = Table[row].Elements[14].Trim().ToDouble();
		ABL_Point.z = Table[row].Elements[15].Trim().ToDouble();

		ABL_Point.Original_x = ABL_Point.x;
		ABL_Point.Original_y = ABL_Point.y;
		ABL_Point.Original_z = ABL_Point.z;

		Ablation_Points_List.push_back(ABL_Point);
	}

	STUDY->Surfaces_List[STUDY->Current_Surface].Ablation_Points_List = Ablation_Points_List;

	if(Echo)
	ShowMessage(IntToStr( (int)Ablation_Points_List.size() ) + " points loaded.");

	} // if dialog execute


}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::ABCtoABCfitting1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	{

	// find xyz of images registration points

	if( STUDY->Surfaces_List[STUDY->Current_Surface].Point_A_Located &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Point_B_Located &&
		STUDY->Surfaces_List[STUDY->Current_Surface].Point_C_Located )
	{
		int Surface_2_ptr = 0;
		Ask_For_Single_Item_Form1->Caption = "Base surface to which fuse (1,2,...): ";
		Ask_For_Single_Item_Form1->Edit_Box->Text = "1";

		if( Ask_For_Single_Item_Form1->ShowModal() == mrOk )
		{

		Surface_2_ptr = Ask_For_Single_Item_Form1->Edit_Box->Text.ToInt()-1;

		if( !STUDY->Surfaces_List[Surface_2_ptr].Point_A_Located ||
			!STUDY->Surfaces_List[Surface_2_ptr].Point_B_Located ||
			!STUDY->Surfaces_List[Surface_2_ptr].Point_C_Located )
		ShowMessage("Operation requires 3 registration points (A,B,C) on two separate geometries");
		else
		{

		for(long i=0;i<200;i++)
		{
		STUDY->perform_one_step_of_triangles_fitting(Surface_2_ptr,STUDY->Current_Surface,-1,false,-1,false);
		repaint_3D_panels();
		}

		} // 2 ABC sets present


		double mean,SD,Median;
		STUDY->get_mean_distance_between_geometries(Surface_2_ptr,STUDY->Current_Surface,&mean,&SD,&Median,
			Annotation_Parameters_Form->Max_Dist_Corr_Nodes_Edit->Text.ToDouble());
		ShowMessage("200 steps calculated. If more fitting is required, just repeat. Mean dist between surfaces= " +
			FloatToStrF(mean,ffGeneral,3,2) + " Median= " + FloatToStrF(Median,ffGeneral,3,2) + ". SD= " + FloatToStrF(SD,ffGeneral,3,2)+ " mm" );

		}
	}
	else
	ShowMessage("Operation requires 3 registration points (A,B,C) on two separate geometries");

	}

}
//---------------------------------------------------------------------------


void __fastcall TMain_Application_Window::CenterALLEXCPEPTFIRSTgeometriessimultaneously1Click(TObject *Sender)
{
	STUDY->center_geometries(1,STUDY->Surfaces_List.size()-1);
	repaint_3D_panels();
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Exportvaluesatgeometrynodes1Click(TObject *Sender)
{
   if( STUDY->is_current_surface_in_range() )
   if( SaveDialog->Execute() )
   {

	AnsiString AS = Utils.remove_substring_from_string(SaveDialog->FileName,".csv") + ".csv";
	ofstream df( AS.c_str());

	Data_IO_Object.save_values_at_geometry_nodes(AS,STUDY);

	ShowMessage("Done");
   }
}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::N67Click(TObject *Sender)
{
	int Source_Geometry = STUDY->Current_Surface;
	int Target_Geometry = 0;

	if(Source_Geometry > 0 )
	{

	double Threshold = 10;

	if(Echo)
	{
	Ask_For_Single_Item_Form1->Caption = "Tolerance distance [mm]:";
	Ask_For_Single_Item_Form1->Edit_Box->Text = "20";
	Ask_For_Single_Item_Form1->ShowModal();
	Threshold = Ask_For_Single_Item_Form1->Edit_Box->Text.ToDouble();
	}

	int dset = STUDY->Surfaces_List[Target_Geometry].Current_Data_Point_Set_Ptr;

	// this one uses glm intersection
	AnsiString New_Map_Name = STUDY->project_surface_onto_other_surface_ray_intersection_as_value(
		Source_Geometry,Target_Geometry,Threshold,dset,"Geometric_Projection",Progress_Form,true);

	// set current value to CV map
	STUDY->Surfaces_List[STUDY->Current_Surface].
		Map_Values.set_current_value_according_to_name(New_Map_Name);

	STUDY->compute_min_max_values();

	OpenGL_Panel_1.prepare_colors_for_display();

	update_controls_state();

	repaint_3D_panels();

	if(Echo)
	ShowMessage("Done");

	}
	else
	ShowMessage("Select geometry other than 1st one to do projection");


}
//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Exportcurrentdatapointegms1Click(TObject *Sender)
{
	if( STUDY->is_current_surface_in_range() )
	if( SaveDialog->Execute() )
	{

	int dset = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr;
	int dp = STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Ptr;

	AnsiString AS = Utils.remove_substring_from_string(SaveDialog->FileName,".csv") + "_raw_egm.csv";
	ofstream df( AS.c_str());

	for(long t=0;t<STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			Data_Points[dp].Roving_Signal.Voltage_Values.size();t++)
	df << STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set[dset].
			Data_Points[dp].Roving_Signal.Voltage_Values[t] << endl;

	df.close();

	ShowMessage("Done");

   }

}
//---------------------------------------------------------------------------



void __fastcall TMain_Application_Window::ImportDxLmap1Click(TObject *Sender)
{
   bool Append = perform_pre_import_queries();

   ShowMessage("Select folder with DxL_1.csv, DxL_2.csv,... and DxLandmarkGeo.xml files");

   if( SelectDirectory("Select data directory", "", Data_IO_Object.Data_Files_Path) )
   {
	Data_FileListBox->Clear();
	Data_FileListBox->Update();
	Data_FileListBox->ApplyFilePath( Data_IO_Object.Data_Files_Path );

	Progress_Form->clear();
	Progress_Form->add_text("Folder selected: " + Data_IO_Object.Data_Files_Path);
	Progress_Form->Show();
	Application->ProcessMessages();

	if( Data_IO_Object.Data_Files_Path.Length() > 0 )
	{
		AnsiString Result = Data_IO_Object.import_NavX_DxL_folder(
			Data_FileListBox,STUDY,Progress_Form,Append);

		if( Result == "Import completed" )
		{
		STUDY->Current_Surface = 0;

		Progress_Form->add_text("Initialization...");
		Progress_Form->Show();
		Application->ProcessMessages();
		post_import_initialization(STUDY->Surfaces_List.size()-1,STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm,true);

		update_controls_state();

		if( MessageDlg("Center map?",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
		if( STUDY->is_current_surface_in_range() )
			STUDY->Surfaces_List[STUDY->Current_Surface].center_geometry_and_data_points();

		OpenGL_Panel_1.set_initial_zoom_in_3D_panel();
		repaint_3D_panels();

		Progress_Form->add_text("Import completed. Press CLOSE to continue." );
		Application->ProcessMessages();
		Progress_Form->Show();

		}
		else
		{
			Progress_Form->Hide();
			ShowMessage(Result);
		}

   } // folder path size > 0
   else
   ShowMessage("Invalid folder selected");

   }

}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void TMain_Application_Window::create_new_dpset(AnsiString Name)
{

	// create empty datapoint set
	Data_Point_Set_Class DPSet;
	DPSet.Name = Name; // "Pointset "+FloatToStr(STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size());
	STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.push_back(DPSet);
	STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr =
		STUDY->Surfaces_List[STUDY->Current_Surface].Data_Point_Set.size()-1;

	// add values to list
	Value_Description_Class Value_Desc;
	Value_Desc.Type = VALUE_BASED_ON_DATA_POINTS;
	Value_Desc.Displayed_In_Table = true;

	Value_Desc.Value_Name = "Bipolar amplitude";
	Value_Desc.Unit = "mV";
	Value_Desc.Fixed_Palette = false;
	Value_Desc.Inverted_Palette = false;
	Value_Desc.Voltage_Amplitude_Value = true;
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);
	Value_Desc.Voltage_Amplitude_Value = false;

	Value_Desc.Value_Name = "rov LAT";
	Value_Desc.Unit = "ms";
	Value_Desc.Inverted_Palette = false;
	Value_Desc.LAT_Value = true;
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);
	Value_Desc.LAT_Value = false;

	Value_Desc.Value_Name = "mean AT";
	Value_Desc.Unit = "ms";
	Value_Desc.Inverted_Palette = false;
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);

	Value_Desc.Value_Name = "AT dispersion";
	Value_Desc.Unit = "ms";
	Value_Desc.Inverted_Palette = true;
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);

	Value_Desc.Value_Name = "Shannon entropy";
	Value_Desc.Unit = "";
	STUDY->Surfaces_List[STUDY->Current_Surface].Map_Values.add_value(Value_Desc);

	update_controls_state();
}

//---------------------------------------------------------------------------

void __fastcall TMain_Application_Window::Importmatlabadvancedfile1Click(TObject *Sender)
{
/*
   bool Append = perform_pre_import_queries();

	OpenDialog->Title = "Select matlab file";
	OpenDialog->FilterIndex = 0;

	if( OpenDialog->Execute() )
	{

	Progress_Form->clear();
	Progress_Form->add_text("File selected: " + OpenDialog->FileName);
	Progress_Form->Show();
	Application->ProcessMessages();

	AnsiString Path = Utils.get_string_before_last_occurence_of_specified_string(OpenDialog->FileName,"\\");

	//------------------------------------------------------
	// 1. Convert to intermediate file
	//------------------------------------------------------
	AnsiString quote = "\"";

	AnsiString Command = "cmd /C export_afcam.exe " + quote + OpenDialog->FileName + quote;
	ShowMessage(Command);
	system( (Command).c_str() );

	//FileName

	//------------------------------------------------------
	// 2. Open intermediate file
	//------------------------------------------------------
	AnsiString IntermediateFileName = GetCurrentDir() + "\\A.intermediate_file_for_EPLabWorks.txt";
	AnsiString Result = Data_IO_Object.import_rhythmia_file(IntermediateFileName,STUDY,Progress_Form,Append);

	if( Result == "Import completed" )
	{
		STUDY->Current_Surface = 0;

		Progress_Form->add_text("Initialization...");
		Progress_Form->Show();
		Application->ProcessMessages();

		STUDY->Current_Surface = STUDY->Surfaces_List.size()-1;
		STUDY->Surfaces_List[STUDY->Current_Surface].Current_Data_Point_Set_Ptr =0;
		int dset = 0;

		bool Downsample_Flag  = false;
		post_import_initialization(STUDY->Current_Surface,
			STUDY->Surfaces_List[STUDY->Current_Surface].Data_Points_Filling_Threshold_mm,Downsample_Flag);

		STUDY->Surfaces_List[STUDY->Current_Surface].interpolate_all_values(0,dset,Progress_Form);

		update_controls_state();

		Main_Application_Window->Cursor = crArrow;

		if( MessageDlg("Center map?",
		mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
		if( STUDY->is_current_surface_in_range() )
			STUDY->Surfaces_List[STUDY->Current_Surface].center_geometry_and_data_points();

		Progress_Form->add_text("Import completed. Press CLOSE to continue." );
		Progress_Form->Show();
		Application->ProcessMessages();

		repaint_3D_panels();
	}
	else
	{
		Progress_Form->Hide();
		ShowMessage(Result);
	}

   }
*/
}
//---------------------------------------------------------------------------














