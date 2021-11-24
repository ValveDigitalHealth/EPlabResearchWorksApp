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

#ifndef Annotation_Setup_FormH
#define Annotation_Setup_FormH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TAnnotation_Parameters_Form : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label3;
    TLabel *Label4;
    TEdit *ROV_Left_Edge_Edit;
    TEdit *ROV_Right_Edge_Edit;
    TButton *OK_Button;
    TButton *Cancel_Button;
	TCheckBox *Automatic_Annotation_Checkbox;
	TCheckBox *Individual_Ref_Annotation_CheckBox;
    TRadioGroup *Annotation_Algorithm_RadioGroup;
    TLabel *Label1;
    TLabel *Label8;
    TLabel *Label9;
    TEdit *Scar_Threshold_Edit;
    TEdit *Conduction_Block_Th_Edit;
    TEdit *Slow_Conduction_Th_Edit;
    TLabel *Label7;
    TLabel *Label11;
    TLabel *Label12;
    TRadioGroup *Shannon_Source_RadioGroup;
    TLabel *Label14;
    TEdit *Bin_Size_Edit;
    TCheckBox *ShannonSDCheckBox;
    TLabel *Label15;
    TEdit *WL_Seg_Detr_Edit;
    TCheckBox *Calc_Shannon_Each_Interp_CheckBox;
    TCheckBox *ShE_Just_Portion_CheckBox;
    TLabel *Label18;
    TLabel *Label19;
    TEdit *ShEn_Begin_Edit;
    TEdit *ShEn_End_Edit;
	TLabel *Label20;
	TEdit *BZ_Threshold_Edit;
	TCheckBox *ShenOnDerivativeCheckBox;
	TLabel *Label40;
	TEdit *QRS_Duration_Edit;
	TLabel *Label41;
	TLabel *Label42;
	TEdit *PDT_For_ED_Edit;
	TLabel *Label43;
	TEdit *SD_Size_forED_Edit;
	TLabel *Label47;
	TEdit *LocalThreshold_Edit;
	TLabel *Label50;
	TEdit *Max_Deflection_Duration_Edit;
	TLabel *Label51;
	TEdit *DataPoints_space_Filling_Th_Edit;
	TLabel *Label52;
	TEdit *Max_Dist_Corr_Nodes_Edit;
	TLabel *Label2;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label10;
	TEdit *EOS_SD_WL_Edit;
	TEdit *EOS_Th_Edit;
	TLabel *Label13;
	TEdit *MinVolt_LAT_Annotaiton_Edit;
    void __fastcall OK_ButtonClick(TObject *Sender);
    void __fastcall Cancel_ButtonClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TAnnotation_Parameters_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAnnotation_Parameters_Form *Annotation_Parameters_Form;
//---------------------------------------------------------------------------
#endif
