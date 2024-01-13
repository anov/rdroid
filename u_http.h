//---------------------------------------------------------------------------
#ifndef u_httpH
#define u_httpH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <stdio.h>
#include <HttpProt.hpp>
#include <ComCtrls.hpp>
#include "Utils.h"
#include "Ftpcli.hpp"
//---------------------------------------------------------------------------
class THttpForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TButton *BNCancel;
    TProgressBar *ProgressBar;
    TLabel *LBMsg;
    THttpCli *Http;
    void __fastcall FormShow(TObject *Sender);
    
    
    
    void __fastcall HttpRequestDone(TObject *Sender, THttpRequest RqType,
          WORD Error);
    void __fastcall HttpSessionConnected(TObject *Sender);
    
    void __fastcall HttpDocData(TObject *Sender, Pointer Buffer, int Len);
    void __fastcall BNCancelClick(TObject *Sender);
    
    
private:	// User declarations
    char * pdata;
    int ppos;
    FILE *fp;
    void __fastcall HttpDLData(TObject *Sender, Pointer Buffer, int Len);
public:		// User declarations
    __fastcall THttpForm(TComponent* Owner);
    char * GetURLData(const AnsiString & url);
    bool DownloadURL(const AnsiString & url,const AnsiString& file);

    void CGIPost(char * url,char * params);
    void CGIGet(char * url);
/*    void PostToDarkJedi(int game, char *comment,int duration);
    void PostToJKNET(int game,char * ip,char *comment,char * level,char * mod, int rank, int players); */
};
//---------------------------------------------------------------------------
extern PACKAGE THttpForm *HttpForm;
//---------------------------------------------------------------------------


#endif
