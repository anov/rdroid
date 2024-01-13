//---------------------------------------------------------------------------
#ifndef u_spropH
#define u_spropH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "utils.h"
//---------------------------------------------------------------------------
class TServPropForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TButton *BNOK;
    TButton *BNCancel;
    TEdit *EBIP;
    TLabel *Label1;
    TMemo *Memo;
    TLabel *LBComment;
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TServPropForm(TComponent* Owner);
    bool ServerProperties(TServerData * sd,bool editable); 
    bool ActiveServerProperties(char * ip,char * comment); 
};
//---------------------------------------------------------------------------
extern PACKAGE TServPropForm *ServPropForm;
//---------------------------------------------------------------------------
#endif
