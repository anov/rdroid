//---------------------------------------------------------------------------
#ifndef u_ftpH
#define u_ftpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Ftpcli.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFtpForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TLabel *LBMsg;
    TButton *BNCancel;
    TProgressBar *ProgressBar;
    TFtpClient *Ftp;
    void __fastcall FtpSessionConnected(TObject *Sender, WORD Error);
    void __fastcall FtpSessionClosed(TObject *Sender, WORD Error);
    void __fastcall FtpProgress(TObject *Sender, int Count,
          bool &Abort);
    void __fastcall FormShow(TObject *Sender);
    
    void __fastcall BNCancelClick(TObject *Sender);
private:	// User declarations
    bool cancelled;
public:		// User declarations
    __fastcall TFtpForm(TComponent* Owner);
    bool DownloadURL(const AnsiString& url,const AnsiString& file);
};
//---------------------------------------------------------------------------
extern PACKAGE TFtpForm *FtpForm;
//---------------------------------------------------------------------------
#endif
