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


#ifndef MainFormH
#define MainFormH

//---------------------------------------------------------------------------
#include <vcl.h>
#include <System.Classes.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Buttons.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "PaintBox_Form.h"
#include "PaintBox_6_Form.h"

#include "FileCtrl.hpp"

#include "predefined_parameters.h"
#include "Signals_Display_Parameters_Class.h"

#include "Progress_Bar_Form.h"
#include "Display_Options_Form.h"
#include "StringGridForm.h"
#include "Annotation_Setup_Form.h"
#include "HelpForm.h"
#include "About_Window_Form.h"
#include "Ask_For_Single_Item_Form.h"
#include "Ask_For_StartStopStep_Form.h"
#include "NL_analysis_form_file.h"
#include "Application_Settings_Form.h"
#include "Geoemtric_Transformation_Unit.h"
#include "RichEdit_Form.h"
#include "SinglePlot_Form.h"
#include "Two_Plot_Form.h"
#include "Bar_Plot_Form.h"
#include "HistogramForm.h"

#include "Numerical_Library.h"
#include "Utils.h"

#include "Symbolic_Dynamics.h"
#include "Recurrence_Diagram_Class.h"
#include "data_io_class.h"
#include "OpenglPanel.h"
#include "STUDY_Class.h"

#include "Segments_List.h"

#include "Data_Point.h"

#include "Surface.h"

//---------------------------------------------------------------------------
class TMain_Application_Window : public TForm
{
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	__published:	// IDE-managed Components
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	TPanel *Panel_1;
	TMainMenu *MainMenuResearch;
	TMenuItem *File1;
	TMenuItem *Opitons1;
	TMenuItem *SignalProcessing1;
	TMenuItem *Geometry1;
	TMenuItem *DataPoints1;
	TMenuItem *Help1;
	TMenuItem *Exit1;
	TToolBar *ToolBar1;
	TPaintBox *Annotation_Window_PaintBox;
	TPageControl *Left_PageControl;
	TTabSheet *TabSheet_Main;
	TComboBox *Geometry_Selection_ComboBox;
	TPopupMenu *OpenGL_Panel_PopupMenu;
	TMenuItem *Rotate1;
	TMenuItem *Zoom1;
	TMenuItem *Select1;
	TMenuItem *Pan1;
	TLabel *Label_p1;
	TMenuItem *ImportAbbottNavXdata1;
	TFileListBox *Data_FileListBox;
	TMenuItem *N1;
	TMenuItem *Displayoptions1;
	TMenuItem *N2;
	TMenuItem *Loadstudy1;
	TMenuItem *Saveworkspace1;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TLabel *DPSet_Label;
	TComboBox *Data_Point_Set_Selection_ComboBox;
	TComboBox *Displayed_Value_Selection_ComboBox_1;
	TLabel *Label2;
	TStringGrid *Data_Points_List_StringGrid;
	TMenuItem *Centergeometry1;
	TMenuItem *Datapoints2;
	TMenuItem *Deletepoints1;
	TMenuItem *Deletepointswithoutxyzcoordinates1;
	TCheckBox *Display_Geo_CheckBox;
	TPageControl *Right_PageControl;
	TTabSheet *GroupedEGMs;
	TPaintBox *All_EGMs_PaintBox;
	TButton *All_Signals_Display_Control_Button;
	TPopupMenu *All_Signal_Tracings_PopupMenu;
	TMenuItem *AllSigs_Increase_Scale;
	TMenuItem *AllSigsDecreaseScale;
	TScrollBar *Signals_ScrollBar;
	TMenuItem *Getstatsofcurrentmapvalue1;
	TMenuItem *GetstatsofcurrentmapvaluePERSEGMENT1;
	TMenuItem *About1;
	TMenuItem *Help2;
	TMenuItem *N3;
	TMenuItem *Centerallgeometries1;
	TMenuItem *ImportCARTOdata1;
	TMenuItem *ImportRhythmiadata1;
	TMenuItem *ImportCARTOexportfolder1;
	TMenuItem *N4;
	TMenuItem *ools1;
	TMenuItem *Downsamplemesh1;
	TMenuItem *N5;
	TMenuItem *Showlogwindow1;
	TTabSheet *TabSheet2;
	TCheckBox *Paint_Segments_CheckBox;
	TLabel *Label3;
	TComboBox *Segment_No_ComboBox;
	TRadioGroup *Segmentation_Brush_Size_RadioGroup;
	TButton *Clear_Segments_Button;
	TButton *ClearAll_segs_Button;
	TButton *Undo_Segmentation_Step_Button;
	TButton *Grow_Current_Segment_Button;
	TCheckBox *Block_Existing_Segments_CheckBox;
	TButton *Shrink_Current_Segment_Button;
	TPopupMenu *Data_Poins_List_PopupMenu;
	TMenuItem *Deletedatapoint1;
	TMenuItem *N6;
	TMenuItem *DisplaylocalsignalinSignalAnalysiswindow1;
	TMenuItem *Sh1;
	TMenuItem *Otherimports1;
	TMenuItem *Importvtkmeshfile1;
	TMenuItem *Applicationsettings1;
	TMenuItem *Annotationsignalprocessinparameters1;
	TMenuItem *N7;
	TMenuItem *N8;
	TMenuItem *N9;
	TMenuItem *Forcedatapointsontosurface1;
	TMenuItem *N10;
	TMenuItem *CalculateWavefrontPropagationVelocityWPVmap1;
	TButton *Annotation_Popup_Button;
	TPopupMenu *Annotation_Window_PopupMenu;
	TMenuItem *MenuItem1;
	TMenuItem *MenuItem2;
	TButton *AP_View_Button;
	TButton *PA_View_Button;
	TMenuItem *N11;
	TMenuItem *Pathanalsis1;
	TMenuItem *Markpathbeginnng1;
	TMenuItem *Markintermediarypoint1;
	TMenuItem *Markpathend1;
	TMenuItem *N12;
	TMenuItem *Displayvaluesalongthepath1;
	TMenuItem *N13;
	TMenuItem *Closeworkspace1;
	TMenuItem *Deletedatapointswithcurrentvaluebelowgiventhreshold1;
	TMenuItem *Getareaandareaofvalueaboveandbeyondspecifiedthreshold1;
	TMenuItem *GetareaandareaofcurrentvalueaboveandbeyondspecifiedthresholdPERSEGMENT1;
	TMenuItem *N14;
	TMenuItem *N15;
	TMenuItem *UpdateReferenceBarpositionwithrespecttocurrentlyselectedVwave1;
	TMenuItem *N16;
	TMenuItem *DisplayrefegminSignalAnalysiswindow1;
	TMenuItem *DisplayECGegminSignalAnalysiswindow1;
	TMenuItem *Varia1;
	TMenuItem *Deletepointswithspecificvalue1;
	TPopupMenu *DataPointsSet_PopupMenu;
	TMenuItem *Changenameofdatapointset1;
	TMenuItem *Clonecurrentdatapointset1;
	TMenuItem *Deletecurrentdatapointset1;
	TMenuItem *N17;
	TMenuItem *N18;
	TPopupMenu *Geometry_PopupMenu;
	TMenuItem *Changecurrentgeometryname1;
	TMenuItem *N19;
	TMenuItem *Clonecurrentsurface1;
	TMenuItem *Restoreoriginallocationsofdatapoints1;
	TMenuItem *N20;
	TMenuItem *Smoothingfunctions1;
	TMenuItem *Averagefilter1;
	TMenuItem *Deletepath1;
	TMenuItem *N21;
	TMenuItem *CalculateEGMdurationmap1;
	TMenuItem *BasedonslidingwindowSD1;
	TMenuItem *BasedonslidingwindowIntegral1;
	TMenuItem *Combined1;
	TMenuItem *DeletedatapointswithcurrentvalueABOVEgiventhreshold1;
	TMenuItem *N22;
	TMenuItem *Undeletealldatapoints1;
	TTabSheet *ECGChannels;
	TPaintBox *ECG_Channels_PaintBox;
	TButton *ECG_Signals_Display_Control_Button;
	TPopupMenu *ECG_Signal_Tracings_PopupMenu;
	TMenuItem *MenuItem3;
	TMenuItem *MenuItem4;
	TTabSheet *ExtraChannels;
	TPaintBox *Extra_Channels_PaintBox;
	TButton *Extra_Channels_Button;
	TPopupMenu *Extra_Signal_Tracings_PopupMenu;
	TMenuItem *MenuItem5;
	TMenuItem *MenuItem6;
	TTabSheet *ImagesPage;
	TStringGrid *Images_StringGrid;
	TOpenDialog *OpenDialogMultiSelect;
	TMenuItem *Importimagesofslicedhearttissue1;
	TPaintBox *Fluoro_Image_Paint_Box;
	TPopupMenu *Fluoro_Image_2D_PopupMenu;
	TMenuItem *Markfirstpointforimagescaling1;
	TMenuItem *Marksecondpointforimagescaling1;
	TMenuItem *Scaleimage1;
	TMenuItem *MenuItem7;
	TMenuItem *Markreferencepoint11;
	TMenuItem *Markreferencepoint21;
	TMenuItem *Markreferencepoint31;
	TMenuItem *ClearRegistrationPoints1;
	TMenuItem *N23;
	TMenuItem *Setregistrationpoint1;
	TMenuItem *A1;
	TMenuItem *B1;
	TMenuItem *C1;
	TMenuItem *N24;
	TMenuItem *Spatialregistration1;
	TMenuItem *Registercurrentgeometrywithsetofhistologyslices1;
	TMenuItem *ranslatecurrentgeometry1;
	TMenuItem *Addlabel1;
	TMenuItem *N25;
	TMenuItem *Deletelabel1;
	TStringGrid *Deleted_Data_Points_List_StringGrid;
	TLabel *Deleted_Data_Points_Label;
	TPopupMenu *Deleted_Data_Poins_List_PopupMenu;
	TMenuItem *MenuItem8;
	TMenuItem *MenuItem9;
	TMenuItem *MenuItem10;
	TMenuItem *MenuItem11;
	TMenuItem *MenuItem13;
	TMenuItem *MenuItem14;
	TMenuItem *MenuItem15;
	TMenuItem *N26;
	TMenuItem *Deletedatapoint2;
	TMenuItem *LVsegmentation1;
	TMenuItem *Addgeometrylandmarkpoint1;
	TMenuItem *Markregions1;
	TMenuItem *Anterior1;
	TMenuItem *Lateral1;
	TMenuItem *Septal1;
	TMenuItem *Apes1;
	TMenuItem *SeptalAnteriorjunctionbase1;
	TMenuItem *SeptalLateraljunctionbase1;
	TMenuItem *AnteriorLateraljunctionbase1;
	TMenuItem *Addintermediatemarkers1;
	TMenuItem *ApextoSeptalAnteriorintermediatepoint1;
	TMenuItem *ApextoSeptalLateralintermediatepoint1;
	TMenuItem *ApextoAnteriorLateralintermediatepoint1;
	TMenuItem *N27;
	TMenuItem *Anteriorbaselineintermediate1;
	TMenuItem *Septalbaselineintermediate1;
	TMenuItem *Lateralbaselineintermediate1;
	TMenuItem *N28;
	TMenuItem *Perform17nuclearsegmentation1;
	TMenuItem *Addwheelgeometry1;
	TPopupMenu *Images_List_PopupMenu;
	TMenuItem *Setslicethickness1;
	TMenuItem *Setthicknessofallslices1;
	TCheckBox *Display_Images_In_3D_Panel_CheckBox;
	TCheckBox *Limit_Image_Display_Bottom_CheckBox;
	TCheckBox *Limit_Image_Display_Top_CheckBox;
	TCheckBox *Remove_blue_background_CheckBox;
	TCheckBox *Remove_green_background_CheckBox;
	TCheckBox *Display_Surface_CheckBox;
	TMenuItem *N29;
	TMenuItem *Recalculateannotation1;
	TMenuItem *N30;
	TMenuItem *Export1;
	TMenuItem *ExportcurrentmapasSTLfileegfor3Dprinting1;
	TMenuItem *N31;
	TMenuItem *Varioustools1;
	TMenuItem *Createtentingpoint1;
	TMenuItem *Createconichole1;
	TMenuItem *Create1;
	TMenuItem *Createuniformelevation1;
	TMenuItem *Upsamplemesh1;
	TMenuItem *Addsquaresample1;
	TPopupMenu *Values_List_PopupMenu;
	TMenuItem *MenuItem12;
	TMenuItem *MenuItem16;
	TMenuItem *MenuItem17;
	TMenuItem *MenuItem18;
	TMenuItem *MenuItem19;
	TMenuItem *Changedatapointtype1;
	TMenuItem *Selecteddatapoint1;
	TMenuItem *Changedatapointtype2;
	TMenuItem *N33;
	TMenuItem *CalculateAFCLmapfrom180msSinusoidalRecomposition1;
	TMenuItem *Importgeometrysegmentation1;
	TMenuItem *Exportgeometrysegmentation1;
	TMenuItem *N36;
	TMenuItem *CenterALLgeometriessimultaneously1;
	TMenuItem *Centeringtools1;
	TMenuItem *N38;
	TMenuItem *Undocentering1;
	TImage *Zoom_Slider_Image;
	TLabel *Zoom_Label;
	TMenuItem *Leftmousebuttonaction1;
	TMenuItem *N40;
	TMenuItem *Geometrystatistics1;
	TMenuItem *ImportALLmapsfromCARTOexportfolder1;
	TMenuItem *Exportvaluesatdatapoints1;
	TButton *Remove_Surface_Segment_Button;
	TLabel *Label1;
	TMenuItem *N42;
	TMenuItem *Restoreremovedsurface1;
	TMenuItem *N43;
	TMenuItem *Smoothmapgeometry1stepofaveragereplacement1;
	TMenuItem *Smoothmapgeometrymultiplestepswithvolumecorrection1;
	TMenuItem *Restoreoriginalgeometry1;
	TMenuItem *Sortvaluesascending1;
	TMenuItem *Sortvaluesdescending1;
	TMenuItem *N47;
	TMenuItem *Sort1;
	TMenuItem *Getdvdtslope1;
	TTabSheet *TabSheet1;
	TCheckBox *Phase_Movie_Mode_CheckBox;
	TLabel *Label4;
	TEdit *SampleValue_Edit;
	TButton *Set_Sample_Button;
	TLabel *Label5;
	TEdit *Movie_Speed_Edit;
	TButton *Movie_Start_Button;
	TButton *Movie_Stop_Button;
	TDirectoryListBox *DirectoryListBox2;
	TMenuItem *Removedatapointsabovetop1;
	TMenuItem *Removedatapointsbelowtop1;
	TMenuItem *Getmicrofractionation1;
	TMenuItem *Displayrovingsignal1;
	TMenuItem *Exportgeometryasvtkfile1;
	TMenuItem *N57;
	TMenuItem *N58;
	TMenuItem *Histograms1;
	TMenuItem *Showhistogramofcurrentvaluespatialgradient1;
	TMenuItem *Smoothnormalswithinspecifiedrange1;
	TMenuItem *Getsegmentwithmaxvalue1;
	TMenuItem *PRESETS1;
	TMenuItem *GetareaandareaofcurrentvaluewithinrangePERSEGMENT1;
	TMenuItem *Showdatasetinfo1;
	TMenuItem *N53;
	TMenuItem *N61;
	TMenuItem *N62;
	TMenuItem *Leavegeometryrelatedwithonlycurrentlypointedsegment1;
	TMenuItem *RemovedatapointsnotonmapsurfaceHARDdelete1;
	TMenuItem *ShowhistogramofspatialgradientofcurrentvalueOFCURRENTSEGMENT1;
	TMenuItem *Reclaculatenormals1;
	TMenuItem *Leaveonly1stmapvisible1;
	TMenuItem *ImportablationpointslistLydiacsvformatv21;
	TMenuItem *RegisterABCABCgeometries1;
	TMenuItem *ABCtoABCfitting1;
	TMenuItem *ABCtoABCfitting2;
	TMenuItem *CenterALLEXCPEPTFIRSTgeometriessimultaneously1;
	TMenuItem *Exportvaluesatgeometrynodes1;
	TMenuItem *N66;
	TMenuItem *N67;
	TMenuItem *Exportcurrentdatapointegms1;
	TMenuItem *ImportDxLmap1;
	TMenuItem *ImportSegment1;
	TMenuItem *Importmatlabadvancedfile1;

	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	// METHODS
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall Panel_1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall Panel_1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Panel_1MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y);
	void __fastcall Select1Click(TObject *Sender);
	void __fastcall Rotate1Click(TObject *Sender);
	void __fastcall Zoom1Click(TObject *Sender);
	void __fastcall Pan1Click(TObject *Sender);
	void __fastcall Displayoptions1Click(TObject *Sender);
	void __fastcall FormAfterMonitorDpiChanged(TObject *Sender, int OldDPI, int NewDPI);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall Loadstudy1Click(TObject *Sender);
	void __fastcall Saveworkspace1Click(TObject *Sender);
	void __fastcall Data_Point_Set_Selection_ComboBoxChange(TObject *Sender);
	void __fastcall Displayed_Value_Selection_ComboBox_1Change(TObject *Sender);
	void __fastcall Centergeometry1Click(TObject *Sender);
	void __fastcall Deletepointswithoutxyzcoordinates1Click(TObject *Sender);
	void __fastcall Display_Geo_CheckBoxClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall All_EGMs_PaintBoxPaint(TObject *Sender);
	void __fastcall All_Signals_Display_Control_ButtonClick(TObject *Sender);
	void __fastcall AllSigs_Increase_ScaleClic(TObject *Sender);
	void __fastcall AllSigsDecreaseScaleClick(TObject *Sender);
	void __fastcall All_EGMs_PaintBoxMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall All_EGMs_PaintBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall All_EGMs_PaintBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall About1Click(TObject *Sender);
	void __fastcall Getstatsofcurrentmapvalue1Click(TObject *Sender);
	void __fastcall ImportCARTOexportfolder1Click(TObject *Sender);
	void __fastcall Signals_ScrollBarChange(TObject *Sender);
	void __fastcall Annotation_Window_PaintBoxPaint(TObject *Sender);
	void __fastcall Annotation_Window_PaintBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Annotation_Window_PaintBoxMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall Annotation_Window_PaintBoxMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Help2Click(TObject *Sender);
	void __fastcall Data_Points_List_StringGridClick(TObject *Sender);
	void __fastcall Downsamplemesh1Click(TObject *Sender);
	void __fastcall Paint_Segments_CheckBoxClick(TObject *Sender);
	void __fastcall Clear_Segments_ButtonClick(TObject *Sender);
	void __fastcall ClearAll_segs_ButtonClick(TObject *Sender);
	void __fastcall Undo_Segmentation_Step_ButtonClick(TObject *Sender);
	void __fastcall Grow_Current_Segment_ButtonClick(TObject *Sender);
	void __fastcall Sh1Click(TObject *Sender);
	void __fastcall Shrink_Current_Segment_ButtonClick(TObject *Sender);
	void __fastcall GetstatsofcurrentmapvaluePERSEGMENT1Click(TObject *Sender);
	void __fastcall Importvtkmeshfile1Click(TObject *Sender);
	void __fastcall Applicationsettings1Click(TObject *Sender);
	void __fastcall Annotationsignalprocessinparameters1Click(TObject *Sender);
	void __fastcall Geometry_Selection_ComboBoxChange(TObject *Sender);
	void __fastcall Forcedatapointsontosurface1Click(TObject *Sender);
	void __fastcall CalculateWavefrontPropagationVelocityWPVmap1Click(TObject *Sender);
	void __fastcall Annotation_Popup_ButtonClick(TObject *Sender);
	void __fastcall MenuItem1Click(TObject *Sender);
	void __fastcall MenuItem2Click(TObject *Sender);
	void __fastcall Showlogwindow1Click(TObject *Sender);
	void __fastcall Deletedatapoint1Click(TObject *Sender);
	void __fastcall Undeletedatapoint1Click(TObject *Sender);
	void __fastcall AP_View_ButtonClick(TObject *Sender);
	void __fastcall PA_View_ButtonClick(TObject *Sender);
	void __fastcall Centerallgeometries1Click(TObject *Sender);
	void __fastcall Markpathbeginnng1Click(TObject *Sender);
	void __fastcall Markintermediarypoint1Click(TObject *Sender);
	void __fastcall Markpathend1Click(TObject *Sender);
	void __fastcall Displayvaluesalongthepath1Click(TObject *Sender);
	void __fastcall Closeworkspace1Click(TObject *Sender);
	void __fastcall Deletedatapointswithcurrentvaluebelowgiventhreshold1Click(TObject *Sender);
	void __fastcall DisplaylocalsignalinSignalAnalysiswindow1Click(TObject *Sender);
	void __fastcall Getareaandareaofvalueaboveandbeyondspecifiedthreshold1Click(TObject *Sender);
	void __fastcall GetareaandareaofcurrentvalueaboveandbeyondspecifiedthresholdPERSEGMENT1Click(TObject *Sender);
	void __fastcall UpdateReferenceBarpositionwithrespecttocurrentlyselectedVwave1Click(TObject *Sender);
	void __fastcall DisplayrefegminSignalAnalysiswindow1Click(TObject *Sender);
	void __fastcall DisplayECGegminSignalAnalysiswindow1Click(TObject *Sender);
	void __fastcall Deletepointswithspecificvalue1Click(TObject *Sender);
	void __fastcall Changenameofdatapointset1Click(TObject *Sender);
	void __fastcall Clonecurrentdatapointset1Click(TObject *Sender);
	void __fastcall Deletecurrentdatapointset1Click(TObject *Sender);
	void __fastcall Changecurrentgeometryname1Click(TObject *Sender);
	void __fastcall Clonecurrentsurface1Click(TObject *Sender);
	void __fastcall Restoreoriginallocationsofdatapoints1Click(TObject *Sender);
	void __fastcall Averagefilter1Click(TObject *Sender);
	void __fastcall Deletepath1Click(TObject *Sender);
	void __fastcall BasedonslidingwindowSD1Click(TObject *Sender);
	void __fastcall BasedonslidingwindowIntegral1Click(TObject *Sender);
	void __fastcall Combined1Click(TObject *Sender);
	void __fastcall DeletedatapointswithcurrentvalueABOVEgiventhreshold1Click(TObject *Sender);
	void __fastcall Undeletealldatapoints1Click(TObject *Sender);
	void __fastcall ECG_Channels_PaintBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ECG_Channels_PaintBoxMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall ECG_Channels_PaintBoxMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ECG_Channels_PaintBoxPaint(TObject *Sender);
	void __fastcall MenuItem3Click(TObject *Sender);
	void __fastcall MenuItem4Click(TObject *Sender);
	void __fastcall MenuItem5Click(TObject *Sender);
	void __fastcall MenuItem6Click(TObject *Sender);
	void __fastcall Extra_Channels_PaintBoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Extra_Channels_PaintBoxMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall Extra_Channels_PaintBoxMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Extra_Channels_PaintBoxPaint(TObject *Sender);
	void __fastcall ECG_Signals_Display_Control_ButtonClick(TObject *Sender);
	void __fastcall Extra_Channels_ButtonClick(TObject *Sender);
	void __fastcall Importimagesofslicedhearttissue1Click(TObject *Sender);
	void __fastcall Fluoro_Image_Paint_BoxPaint(TObject *Sender);
	void __fastcall Fluoro_Image_Paint_BoxMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Fluoro_Image_Paint_BoxMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall Fluoro_Image_Paint_BoxMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Markfirstpointforimagescaling1Click(TObject *Sender);
	void __fastcall Marksecondpointforimagescaling1Click(TObject *Sender);
	void __fastcall Scaleimage1Click(TObject *Sender);
	void __fastcall Markreferencepoint11Click(TObject *Sender);
	void __fastcall Markreferencepoint21Click(TObject *Sender);
	void __fastcall Markreferencepoint31Click(TObject *Sender);
	void __fastcall ClearRegistrationPoints1Click(TObject *Sender);
	void __fastcall Images_StringGridClick(TObject *Sender);
	void __fastcall Images_StringGridDblClick(TObject *Sender);
	void __fastcall A1Click(TObject *Sender);
	void __fastcall B1Click(TObject *Sender);
	void __fastcall C1Click(TObject *Sender);
	void __fastcall Registercurrentgeometrywithsetofhistologyslices1Click(TObject *Sender);
	void __fastcall ranslatecurrentgeometry1Click(TObject *Sender);
	void __fastcall Addlabel1Click(TObject *Sender);
	void __fastcall Deletelabel1Click(TObject *Sender);
	void __fastcall MenuItem8Click(TObject *Sender);
	void __fastcall MenuItem9Click(TObject *Sender);
	void __fastcall MenuItem10Click(TObject *Sender);
	void __fastcall Deletedatapoint2Click(TObject *Sender);
	void __fastcall Anterior1Click(TObject *Sender);
	void __fastcall Lateral1Click(TObject *Sender);
	void __fastcall Septal1Click(TObject *Sender);
	void __fastcall Apes1Click(TObject *Sender);
	void __fastcall SeptalAnteriorjunctionbase1Click(TObject *Sender);
	void __fastcall SeptalLateraljunctionbase1Click(TObject *Sender);
	void __fastcall AnteriorLateraljunctionbase1Click(TObject *Sender);
	void __fastcall ApextoSeptalAnteriorintermediatepoint1Click(TObject *Sender);
	void __fastcall ApextoSeptalLateralintermediatepoint1Click(TObject *Sender);
	void __fastcall ApextoAnteriorLateralintermediatepoint1Click(TObject *Sender);
	void __fastcall Anteriorbaselineintermediate1Click(TObject *Sender);
	void __fastcall Septalbaselineintermediate1Click(TObject *Sender);
	void __fastcall Lateralbaselineintermediate1Click(TObject *Sender);
	void __fastcall Perform17nuclearsegmentation1Click(TObject *Sender);
	void __fastcall Addwheelgeometry1Click(TObject *Sender);
	void __fastcall Setslicethickness1Click(TObject *Sender);
	void __fastcall Images_StringGridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Data_Points_List_StringGridDblClick(TObject *Sender);
	void __fastcall Setthicknessofallslices1Click(TObject *Sender);
	void __fastcall Display_Images_In_3D_Panel_CheckBoxClick(TObject *Sender);
	void __fastcall Limit_Image_Display_Bottom_CheckBoxClick(TObject *Sender);
	void __fastcall Limit_Image_Display_Top_CheckBoxClick(TObject *Sender);
	void __fastcall Remove_blue_background_CheckBoxClick(TObject *Sender);
	void __fastcall Remove_green_background_CheckBoxClick(TObject *Sender);
	void __fastcall Display_Surface_CheckBoxClick(TObject *Sender);
	void __fastcall Recalculateannotation1Click(TObject *Sender);
	void __fastcall ExportcurrentmapasSTLfileegfor3Dprinting1Click(TObject *Sender);
	void __fastcall Createtentingpoint1Click(TObject *Sender);
	void __fastcall Createconichole1Click(TObject *Sender);
	void __fastcall Create1Click(TObject *Sender);
	void __fastcall Createuniformelevation1Click(TObject *Sender);
	void __fastcall Upsamplemesh1Click(TObject *Sender);
	void __fastcall Addsquaresample1Click(TObject *Sender);
	void __fastcall MenuItem19Click(TObject *Sender);
	void __fastcall MenuItem12Click(TObject *Sender);
	void __fastcall MenuItem17Click(TObject *Sender);
	void __fastcall Changedatapointtype1Click(TObject *Sender);
	void __fastcall Changedatapointtype2Click(TObject *Sender);
	void __fastcall CalculateAFCLmapfrom180msSinusoidalRecomposition1Click(TObject *Sender);
	void __fastcall Importgeometrysegmentation1Click(TObject *Sender);
	void __fastcall Exportgeometrysegmentation1Click(TObject *Sender);
	void __fastcall CenterALLgeometriessimultaneously1Click(TObject *Sender);
	void __fastcall Undocentering1Click(TObject *Sender);
	void __fastcall Zoom_Slider_ImageMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Zoom_Slider_ImageMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall Zoom_Slider_ImageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Geometrystatistics1Click(TObject *Sender);
	void __fastcall ImportALLmapsfromCARTOexportfolder1Click(TObject *Sender);
	void __fastcall Exportvaluesatdatapoints1Click(TObject *Sender);
	void __fastcall Remove_Surface_Segment_ButtonClick(TObject *Sender);
	void __fastcall Restoreremovedsurface1Click(TObject *Sender);
	void __fastcall Smoothmapgeometry1stepofaveragereplacement1Click(TObject *Sender);
	void __fastcall Smoothmapgeometrymultiplestepswithvolumecorrection1Click(TObject *Sender);
	void __fastcall Restoreoriginalgeometry1Click(TObject *Sender);
	void __fastcall Createnewmap1Click(TObject *Sender);
	void __fastcall Sortvaluesascending1Click(TObject *Sender);
	void __fastcall Sortvaluesdescending1Click(TObject *Sender);
	void __fastcall Getdvdtslope1Click(TObject *Sender);
	void __fastcall Set_Sample_ButtonClick(TObject *Sender);
	void __fastcall Phase_Movie_Mode_CheckBoxClick(TObject *Sender);
	void __fastcall Movie_Start_ButtonClick(TObject *Sender);
	void __fastcall Movie_Stop_ButtonClick(TObject *Sender);
	void __fastcall Removedatapointsbelowtop1Click(TObject *Sender);
	void __fastcall Getmicrofractionation1Click(TObject *Sender);
	void __fastcall Displayrovingsignal1Click(TObject *Sender);
	void __fastcall Exportgeometryasvtkfile1Click(TObject *Sender);
	void __fastcall N58Click(TObject *Sender);
	void __fastcall Showhistogramofcurrentvaluespatialgradient1Click(TObject *Sender);
	void __fastcall Getsegmentwithmaxvalue1Click(TObject *Sender);
	void __fastcall GetareaandareaofcurrentvaluewithinrangePERSEGMENT1Click(TObject *Sender);
	void __fastcall Showdatasetinfo1Click(TObject *Sender);
	void __fastcall N61Click(TObject *Sender);
	void __fastcall Leavegeometryrelatedwithonlycurrentlypointedsegment1Click(TObject *Sender);
	void __fastcall RemovedatapointsnotonmapsurfaceHARDdelete1Click(TObject *Sender);
	void __fastcall ShowhistogramofspatialgradientofcurrentvalueOFCURRENTSEGMENT1Click(TObject *Sender);
	void __fastcall Smoothnormalswithinspecifiedrange1Click(TObject *Sender);
	void __fastcall Reclaculatenormals1Click(TObject *Sender);
	void __fastcall Leaveonly1stmapvisible1Click(TObject *Sender);
	void __fastcall ImportablationpointslistLydiacsvformatv21Click(TObject *Sender);
	void __fastcall ABCtoABCfitting1Click(TObject *Sender);
	void __fastcall CenterALLEXCPEPTFIRSTgeometriessimultaneously1Click(TObject *Sender);
	void __fastcall Exportvaluesatgeometrynodes1Click(TObject *Sender);
	void __fastcall N67Click(TObject *Sender);
	void __fastcall Exportcurrentdatapointegms1Click(TObject *Sender);
	void __fastcall ImportDxLmap1Click(TObject *Sender);
	void __fastcall Importmatlabadvancedfile1Click(TObject *Sender);

	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	private:	// User declarations
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------

	AnsiString SourceFileName = ": MainForm";

	//----------------------------------------------------------------------
	//----------------------------------------------------------------------
	public:		// User declarations
	//----------------------------------------------------------------------
	//----------------------------------------------------------------------

	__fastcall TMain_Application_Window(TComponent* Owner);

	void update_controls_state();
	void save_state_of_GUI_controls();
	void clear_controls(); // images usuwanie
	void fill_data_points_table(int Which_Data_Points);

	//-------------------------------------------------------------------------
	// images support
	//-------------------------------------------------------------------------
	void update_images_table();
	void draw_fluoro_image_2D();
	Graphics::TBitmap* Fluoro_Image_Bimap;
	void set_registration_point_on_fluoro_image(int Which_Point);

	void repaint_all_controls();

	int Which_Panel_Pointed; // potrzebne???

	void set_registration_point_on_3D_geometry(int Which_Point);

	Graphics::TBitmap *All_EGMs_PaintBox_Bitmap;
	Graphics::TBitmap *Annotation_Window_PaintBox_Bitmap;
	Graphics::TBitmap *ECG_Channels_Box_Bitmap;
	Graphics::TBitmap *Extra_Channels_Box_Bitmap;

	//----------------------------------------------------------------------
	// OPENGL VARIABLES and SUPPORT
	//----------------------------------------------------------------------
	OpenGL_Panel_Class OpenGL_Panel_1;

	void map_display_panel_mouse_down(TMouseButton Button, TShiftState Shift,int X, int Y, OpenGL_Panel_Class* Current_OpenGL_Panel);
	void map_dislplay_panel_mouse_move(int X, int Y, OpenGL_Panel_Class* Current_OpenGL_Panel);

	void repaint_3D_panels();

	bool perform_pre_import_queries();
	void post_import_initialization(int Surface_Ptr,double Max_Distance_Th_mm,bool DownSample_Mesh_Flag);

	bool is_Map_Panel_PopupMenu_item_tag_checked(int tag);
	void set_Map_Panel_PopupMenu_item_checked_state(int tag,bool checked);

	//-------------------------------------------------------------------------
	// MAIN DATA STRUCTURE
	//-------------------------------------------------------------------------
	STUDY_Class* STUDY;

	//-------------------------------------------------------------------------
	// SUPPORT OF DATA INPUT/OUTPUT
	//-------------------------------------------------------------------------
	Data_IO_Class Data_IO_Object;

	void post_study_load_processing();

	// call this to re-calculate "closest" ptrs of all nodes and dp's
	void calculate_closest_nodes_and_data_points_pointers();

	Utils_Tools Utils;
	Numerical_Library PNUM;

	bool Echo;

	AnsiString Application_Directory;

	Signals_Display_Parameters All_Signals_Box_Parameters;
	Signals_Display_Parameters Annotation_Box;
	Signals_Display_Parameters ECG_Channels_Box_Parameters;
	Signals_Display_Parameters Extra_Channels_Box_Parameters;
	Signals_Display_Parameters FImage_Box_Parameters;

	void read_app_version();

	AnsiString EPLAB_App_Version;

	//-----------------------------------------------------------
	// segmentation support
	//-----------------------------------------------------------
	Segments_List_Class Segments_Info;
	void __fastcall mark_segment(int Triangle_Ptr,int Area_Id,bool Locked_Flag);
	int Previously_Selected_Value_ptr;
	int get_current_segment_id();
	AnsiString get_current_segment_name();

	//-----------------------------------------------------------
	// labels support
	//-----------------------------------------------------------
	void read_labels_list(AnsiString FileName);
	void save_labels_list(AnsiString FileName);
	std::vector <AnsiString> Labels_Names;


	void FULL_MAP_VALUES_UPDATE(int Surface_Ptr);
	void update_LAT_annotation_of_current_map();
	void update_display_of_LAT_Bip_maps();


	bool Application_Closing_Flag;

	void close_workspace();

	//----------------------------------------------------------------------
	// ZOOM MOUSE SUPPORT
	//----------------------------------------------------------------------
	int Zoom_MouseStatus;
	TMouseButton Zoom_MouseButton;
	int Zoom_Mouse_X,Zoom_Mouse_Y;

	long Global_Movie_TimePoint_ptr;
	bool Phase_Movie_Flag;

	// various support features
	AnsiString get_value_name_from_selection_form();

	AnsiString Root_Directory_of_Folders_List;

	// tmp
	void create_new_dpset(AnsiString Name);
	std::vector<double> TVecDouble;

};


//---------------------------------------------------------------------------
extern PACKAGE TMain_Application_Window *Main_Application_Window;
//---------------------------------------------------------------------------
#endif
