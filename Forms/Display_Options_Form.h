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

#ifndef Maps_Setup_FormH
#define Maps_Setup_FormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>

//---------------------------------------------------------------------------
class TDisplay_Options_Form_1 : public TForm
{
__published:	// IDE-managed Components
    TButton *OK_Button;
    TLabel *Label6;
    TRadioGroup *Font_RadioGroup;
    TCheckBox *Fixed_Palette_Range_CheckBox;
    TLabel *Label9;
    TLabel *Label10;
    TEdit *Fixed_Palette_Range_Min_Edit;
    TEdit *Fixed_Palette_Range_Max_Edit;
    TBevel *Bevel6;
    TCheckBox *Isovalues_CheckBox;
    TLabel *Label11;
    TEdit *Palette_Coarsening_LeveL_Edit;
    TLabel *Label12;
    TComboBox *Line_Thickness_ComboBox;
    TBevel *Bevel7;
    TCheckBox *DP_Name_CheckBox;
    TBevel *Bevel1;
    TCheckBox *Display_Local_Signal_CheckBox;
	TCheckBox *Display_Ref1_Signal_CheckBox;
	TCheckBox *Display_ECG_Signal_CheckBox;
    TCheckBox *Pointing_Cross_Display_CheckBox;
    TCheckBox *Draw_Data_Points_CheckBox;
    TCheckBox *Display_Geo_Nodes_CheckBox;
    TCheckBox *Display_3D_arrow_CheckBox;
    TCheckBox *Display_All_DP_Sets_CheckBox;
    TRadioGroup *Extremes_Colours_RadioGroup;
    TBevel *Bevel3;
    TCheckBox *Display_Triangles_CheckBox;
    TCheckBox *Invert_Palette_CheckBox;
    TCheckBox *Interpolation_CheckBox;
    TCheckBox *NavX_Patient_Doll_CheckBox;
	TCheckBox *Display_Segments_CheckBox;
	TCheckBox *Display_17_LV_markersCheckBox;
	TCheckBox *DAP_CheckBox;
	TButton *P1_Limits_Button;
	TCheckBox *Disp_Segm_Paths_CheckBox;
	TLabel *Label20;
	TCheckBox *PaintRegPoints_CheckBox;
	TScrollBar *Transparency_ScrollBar;
	TLabel *Label21;
	TCheckBox *Display_Map_Title_CheckBox;
	TCheckBox *Display_Normals_CheckBox;
	TButton *P2_Limits_Button;
	TLabel *Label13;
	TCheckBox *Auto_DP_Size_Scaling_CheckBox;
	TLabel *Label1;
	TLabel *Label3;
	TBevel *Bevel4;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *DPSize_Scaling_Factor_Edit;
	TComboBox *DP_Size_ComboBox;
	TComboBox *AblPointSizeComboBox;
	TButton *Cancel_Button;
	TCheckBox *Display_Paths_CheckBox;
	TRadioGroup *Background_Color_RadioGroup;
	TCheckBox *Display_Edges_CheckBox;
	TCheckBox *Display_3D_Point_Cloud_CheckBox;
	TRadioGroup *Palette_Type_RadioGroup;
	TCheckBox *Min09Max_Range_CheckBox;
	TLabel *Label2;
	TEdit *AT_Edit;
	TEdit *PS_Edit;
	TEdit *AS_Edit;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label14;
	TEdit *DTS_Edit;
	TCheckBox *Display_Arrows_CheckBox;


    void __fastcall OK_ButtonClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
	void __fastcall Fixed_Palette_Range_CheckBoxClick(TObject *Sender);
	void __fastcall P1_Limits_ButtonClick(TObject *Sender);
	void __fastcall P2_Limits_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);

    private:	// User declarations

	public:		// User declarations

	__fastcall TDisplay_Options_Form_1(TComponent* Owner);


};
//---------------------------------------------------------------------------
extern PACKAGE TDisplay_Options_Form_1 *Display_Options_Form_1;
//---------------------------------------------------------------------------
#endif
