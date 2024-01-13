//---------------------------------------------------------------------------
#ifndef u_askdlH
#define u_askdlH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "utils.h"

#define dl_Ignore 0
#define dl_Cancel 1
#define dl_Download 2

//---------------------------------------------------------------------------
class TAskDLForm : public TForm
{
__published:	// IDE-managed Components
    TButton *BNDownload;
    TListBox *LBSites;
    TButton *BNIgnore;
    TLabel *Label1;
    TLabel *Label2;
    TButton *BNCancel;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall LBSitesClick(TObject *Sender);
    
private:	// User declarations
public:		// User declarations
    AnsiString url;
    __fastcall TAskDLForm(TComponent* Owner);
    int AskDownload(TActiveServerData * asd);
};
//---------------------------------------------------------------------------
extern PACKAGE TAskDLForm *AskDLForm;
//---------------------------------------------------------------------------
#endif
