//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "u_ftp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Ftpcli"
#pragma resource "*.dfm"
TFtpForm *FtpForm;
//---------------------------------------------------------------------------
__fastcall TFtpForm::TFtpForm(TComponent* Owner)
    : TForm(Owner)
{
}

bool TFtpForm::DownloadURL(const AnsiString& url,const AnsiString& file)
{
 char * p, *p1, *purl;
 AnsiString s=url;
 purl=s.c_str();
 int i;

 p=purl+6;
 p1=StrScan(p,'/');

 Ftp->HostName=s.SubString(p-purl+1,p1-p);

 s=p1;
 for (i=1;i<=s.Length();i++)
 {
  if (s[i]=='/') s[i]='\\';
 }
 Ftp->HostFileName=ExtractFileName(s);

 s=ExtractFilePath(s);

 for (i=1;i<=s.Length();i++)
 {
  if (s[i]=='\\') s[i]='/';
 }

 Ftp->HostDirName=s;
 Ftp->LocalFileName=file;

 LBMsg->Caption="Connecting";
 Caption=url;

 ShowModal();
 return !cancelled;

}

//---------------------------------------------------------------------------
void __fastcall TFtpForm::FtpSessionConnected(TObject *Sender,
      WORD Error)
{
 if (!Error) LBMsg->Caption="Connected";
 else {LBMsg->Caption="Error"; cancelled=true; Close(); }
}
//---------------------------------------------------------------------------
void __fastcall TFtpForm::FtpSessionClosed(TObject *Sender,
      WORD Error)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TFtpForm::FtpProgress(TObject *Sender, int Count,
      bool &Abort)
{
 LBMsg->Caption="Receiving "+IntToStr(Count)+" bytes";
 Abort=cancelled;    
}
//---------------------------------------------------------------------------
void __fastcall TFtpForm::FormShow(TObject *Sender)
{
 cancelled=false;
 Ftp->ReceiveAsync();
}
//---------------------------------------------------------------------------
void __fastcall TFtpForm::BNCancelClick(TObject *Sender)
{
 cancelled=true;    
}
//---------------------------------------------------------------------------
