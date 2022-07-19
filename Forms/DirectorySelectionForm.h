//---------------------------------------------------------------------------

#ifndef DirectorySelectionFormH
#define DirectorySelectionFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.FileCtrl.hpp>
//---------------------------------------------------------------------------
class TDirectory_Selection_Form : public TForm
{
__published:	// IDE-managed Components

	TDirectoryListBox *DirectoryListBox;
	TButton *Close_Button;
	TButton *Cancel_Button;

	void __fastcall Close_ButtonClick(TObject *Sender);
	void __fastcall Cancel_ButtonClick(TObject *Sender);

	private:	// User declarations

	public:		// User declarations

	__fastcall TDirectory_Selection_Form(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDirectory_Selection_Form *Directory_Selection_Form;
//---------------------------------------------------------------------------
#endif
