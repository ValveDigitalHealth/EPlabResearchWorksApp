//---------------------------------------------------------------------------

#ifndef GeosSelection_FormH
#define GeosSelection_FormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TGeos_Selection_Form : public TForm
{
__published:	// IDE-managed Components
	TComboBox *Geometry_Selection_ComboBox1;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *Geometry_Selection_ComboBox2;
	TButton *Cancel_Button;
	TButton *Proceed_Button;
	void __fastcall Proceed_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TGeos_Selection_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGeos_Selection_Form *Geos_Selection_Form;
//---------------------------------------------------------------------------
#endif
