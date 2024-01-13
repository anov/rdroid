//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "u_http.h"
#include <stdio.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HttpProt"
#pragma link "Ftpcli"
#pragma resource "*.dfm"
THttpForm *HttpForm;
//---------------------------------------------------------------------------
__fastcall THttpForm::THttpForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

bool THttpForm::DownloadURL(const AnsiString & url,const AnsiString& file)
{
 fp=fopen(file.c_str(),"wb");
 if (!fp) return false;

 HttpForm->Caption=url;
 Http->URL=url;
 Http->RcvdStream = NULL;
 Http->ModifiedSince = 0;
 Http->OnDocData=HttpDLData;
 ProgressBar->Position=0;
 ShowModal();

 return !fclose(fp);
}


char * THttpForm::GetURLData(const AnsiString & url)
{
 HttpForm->Caption=url;
 Http->URL=url;
 Http->RcvdStream = NULL;
 Http->ModifiedSince = 0;
 Http->OnDocData=HttpDocData;
 ProgressBar->Position=0;
 ShowModal();
 return pdata;
}

void __fastcall THttpForm::FormShow(TObject *Sender)
{
 LBMsg->Caption="Connecting";
 pdata=NULL;
 try {
  Http->GetASync();
  } __except (TRUE) {
   Close();
 }
}
//---------------------------------------------------------------------------


void __fastcall THttpForm::HttpRequestDone(TObject *Sender,
      THttpRequest RqType, WORD Error)
{
 if (Error) LBMsg->Caption="Error occured";
 Close();
}
//---------------------------------------------------------------------------



void __fastcall THttpForm::HttpSessionConnected(TObject *Sender)
{
 LBMsg->Caption="Connected";
}
//---------------------------------------------------------------------------

void __fastcall THttpForm::HttpDLData(TObject *Sender, Pointer Buffer, int Len)
{
 long fpos;
 fgetpos(fp,&fpos);
 LBMsg->Caption="Retrieving: "+IntToStr(fpos)+" bytes...";
 fwrite(Buffer,Len,1,fp);

 if (Http->ContentLength)
 {
   ProgressBar->Position=fpos*100/Http->ContentLength;
 }

}

void __fastcall THttpForm::HttpDocData(TObject *Sender, Pointer Buffer,
      int Len)
{
 LBMsg->Caption="Retrieving: "+IntToStr(ppos)+" bytes...";
 if (!pdata)
 {
  pdata=(char *)malloc(Len+1);
  ppos=0;
 } else
 {
  pdata=(char *)realloc(pdata,ppos+Len+1);
 }
 memcpy((pdata+ppos),Buffer,Len);
 ppos+=Len;
 *(pdata+ppos)='\0';

}
//---------------------------------------------------------------------------


void __fastcall THttpForm::BNCancelClick(TObject *Sender)
{
 Http->Abort();
 Close();    
}
//---------------------------------------------------------------------------

void THttpForm::CGIPost(char * url,char * params)
{
 TMemoryStream * ms;
 ms=new TMemoryStream;
 ms->WriteBuffer(params,strlen(params));
 ms->Position=0;
 Http->OnDocData=NULL;
 Http->URL=url;
 Http->SendStream = ms;
 Http->RcvdStream = NULL;

  try {
        Http->Post();
   } __except (TRUE) {
  }
  delete ms;
  Http->OnDocData=HttpDocData;
}

void THttpForm::CGIGet(char * url)
{
   Http->OnDocData=NULL;
   Http->URL=url;
   Http->SendStream = NULL;
   Http->RcvdStream = NULL;

    try {
        Http->Get();
    } __except (TRUE) {
   }
   Http->OnDocData=HttpDocData;
}





