//---------------------------------------------------------------------------
#ifndef u_pollipH
#define u_pollipH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TPollIP : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TComboBox *CBIP;
    TLabel *Label1;
    TButton *BNPoll;
    TButton *BNCancel;
private:	// User declarations
public:		// User declarations
    __fastcall TPollIP(TComponent* Owner);
    AnsiString __fastcall GetIP();
};
//---------------------------------------------------------------------------
extern PACKAGE TPollIP *PollIP;
//---------------------------------------------------------------------------
#endif
