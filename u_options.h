//---------------------------------------------------------------------------
#ifndef u_optionsH
#define u_optionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TOptions : public TForm
{
__published:	// IDE-managed Components
    TEdit *EBJK;
    TButton *BNJK;
    TLabel *Label1;
    TEdit *EBMOTS;
    TLabel *Label2;
    TButton *BNMOTS;
    TButton *BNSave;
    TButton *BNCancel;
    TOpenDialog *JKDialog;
    TOpenDialog *MOTSDialog;
    void __fastcall FormCreate(TObject *Sender);
    
    void __fastcall BNJKClick(TObject *Sender);
    
    void __fastcall BNMOTSClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TOptions(TComponent* Owner);
    void SetOptions();
};
//---------------------------------------------------------------------------
extern PACKAGE TOptions *Options;
//---------------------------------------------------------------------------
#endif
