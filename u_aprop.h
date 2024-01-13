//---------------------------------------------------------------------------
#ifndef u_apropH
#define u_apropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "utils.h"
//---------------------------------------------------------------------------
class TAPropForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TLabel *Label1;
    TEdit *EBIP;
    TMemo *MDetails;
    TLabel *LBPlayers;
    TMemo *MComment;
    TMemo *MPlayers;
    TLabel *Label4;
    TLabel *Label5;
    TButton *BNClose;
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TAPropForm(TComponent* Owner);
    void ShowProperties(char * ip,TActiveServerData * asd); 
};
//---------------------------------------------------------------------------
extern PACKAGE TAPropForm *APropForm;
//---------------------------------------------------------------------------
#endif
