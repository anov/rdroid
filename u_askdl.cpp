//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>

#include "u_askdl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAskDLForm *AskDLForm;
//---------------------------------------------------------------------------
__fastcall TAskDLForm::TAskDLForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAskDLForm::FormCreate(TObject *Sender)
{
 ClientWidth=BNCancel->Left+BNCancel->Width+LBSites->Left;    
 ClientHeight=BNCancel->Top+BNCancel->Height+LBSites->Top;    
}
//---------------------------------------------------------------------------
void __fastcall TAskDLForm::LBSitesClick(TObject *Sender)
{
 if (LBSites->ItemIndex<0) LBSites->Hint="";
 else LBSites->Hint=LBSites->Items->Strings[LBSites->ItemIndex];
}
//---------------------------------------------------------------------------

// LBSites->Items->Add("http://209.1.245.15/review/files/cardiapack.zip");

void LoadSiteList(TStringList * files,TStringList * urls)
{
 FILE *fp;
 char buf[1024],*p;
 int n;
 AnsiString s;

 fp=fopen("sites.lst","rt");
 if (!fp) return;

 files->Clear();
 files->Sorted=true;
 files->Duplicates=dupIgnore;

 int no=0;

 while (!feof(fp))
 {
  fgets(buf,sizeof(buf),fp);
  p=StrScan(buf,'#');
  if (p) *p='\0';

  if (buf[0]=='"')
  {
   p=StrScan(buf+1,'"');
   if (!p) continue;
  *p='\0';
   s=Trim(buf+1);
  } else
  {
  p=StrScan(buf,' ');
  if (!p) p=StrScan(buf,'\t');
  if (!p) continue;
  *p='\0';
  s=Trim(buf);
  }

  if (s=="") continue;
  n=files->IndexOf(s);
  if (n<0) { n=files->AddObject(s,(TObject*)no); no++; };
  urls->AddObject(Trim(p+1),files->Objects[n]);
 }
}

void GetFileSites(const AnsiString &file,TStringList * files,TStringList * urls,TStrings * sites)
{
 sites->Clear();
 int n=files->IndexOf(file);
 int i;

 if (n<0) return;
 TObject * no=files->Objects[n];

 for (i=0;i<urls->Count;i++)
 {
  if (no==urls->Objects[i])
   sites->Add(urls->Strings[i]);
 }
}

void GetSites(TStrings * sl,TActiveServerData * asd)
{
 TStringList * files=new TStringList;
 TStringList * urls=new TStringList;

 LoadSiteList(files,urls);
 GetFileSites(GetGOBName(asd),files,urls,sl);

 files->Free();
 urls->Free();


 char * s=asd->comment.c_str(),*p,*p1;
 p=StrPos(s,"http://");

 if (p)
 {
  p1=StrScan(p,' ');
  AnsiString s=p;
  if (p1) s.SetLength(p1-p);
  sl->Add(s);
 }

 p=StrPos(s,"ftp://");

 if (p)
 {
  p1=StrScan(p,' ');
  AnsiString s=p;
  if (p1) s.SetLength(p1-p);
  sl->Add(s);
 }

}

int TAskDLForm::AskDownload(TActiveServerData * asd)
{
 GetSites(LBSites->Items,asd);

 BNDownload->Enabled=LBSites->Items->Count;
 LBSites->ItemIndex=0;
 LBSitesClick(LBSites);

 switch (ShowModal())
 {
  case mrOk: {
              url="";
              if (LBSites->ItemIndex>=0) url=LBSites->Items->Strings[LBSites->ItemIndex];
              return dl_Download;
             }
  case mrIgnore: return dl_Ignore;
  case mrCancel: return dl_Cancel;
 }
 return dl_Ignore;
}
