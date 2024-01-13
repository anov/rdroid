//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "U_unzip.h"
#include "Unzip.hpp"
#include "ziptypes.hpp"

#define id_progress 1000

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TUnZipForm *UnZipForm;
//---------------------------------------------------------------------------
__fastcall TUnZipForm::TUnZipForm(TComponent* Owner)
    : TForm(Owner)
{
}

void __fastcall TUnZipForm::WndProc(TMessage &Message)
{
 TForm::WndProc(Message);
 if ((Message.Msg==WM_COMMAND) && (Message.WParam==id_progress))
 {
  short * ppercent=(short *)Message.LParam;
  if (ppercent)
  {
   ProgressBar->Position=(CFile*100+*ppercent)/NFiles;
   if (cancelled) *ppercent=0xFFFF;
  }
 }
}


//---------------------------------------------------------------------------

bool TUnZipForm::Unzip(const AnsiString& zip,const AnsiString& dir)
{
 Zip=zip;
 Dir=dir;
 Show();
 bool res=UnzipIt();
 Hide();
 return res;
}

bool TUnZipForm::UnzipIt()
{
 int r;
 TZipRec zr;

 Caption="Unzipping "+Zip;

 cancelled=false;
 NFiles=0;

 r=GetFirstInZip(Zip.c_str(),zr);
 if (r)
 {
  ShowMessage("Error opening .ZIP file: "+Zip);
  return false;
 }

 TStringList * files=new TStringList;

 while (!r)
 {
  NFiles++;
  files->AddObject(zr.FileName,(TObject *)zr.headeroffset);
  r=GetNextInZip (zr);
 }

 bool res=true;

 for (CFile=0;CFile<NFiles;CFile++)
 {
  r=UnzipFile (Zip.c_str(),(Dir+ExtractFileName(files->Strings[CFile])).c_str(),
               (int)files->Objects[CFile], (int)Handle, id_progress );
  res=res && (r==0);
  if (cancelled) break;
 }

 CloseZipFile (zr);
 return res;
}
//---------------------------------------------------------------------------

void __fastcall TUnZipForm::BNCancelClick(TObject *Sender)
{
 cancelled=true;    
}
//---------------------------------------------------------------------------

