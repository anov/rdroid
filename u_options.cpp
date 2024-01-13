//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "u_options.h"
#include "utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptions *Options;
//---------------------------------------------------------------------------
__fastcall TOptions::TOptions(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptions::FormCreate(TObject *Sender)
{
 ClientWidth=BNCancel->Left+BNCancel->Width+EBJK->Left;
 ClientHeight=BNCancel->Top+BNCancel->Height+EBJK->Left;
}
//---------------------------------------------------------------------------


void TOptions::SetOptions()
{

 EBJK->Text=GetJKAddOnDir();
 EBMOTS->Text=GetMOTSAddOnDir();

 if (ShowModal()==mrOk)
 {
  SaveAddOnDirs(PathAddSlash(EBJK->Text),PathAddSlash(EBMOTS->Text));
 }

}

void __fastcall TOptions::BNJKClick(TObject *Sender)
{
 JKDialog->InitialDir=EBJK->Text;
 JKDialog->FileName="";
 if (JKDialog->Execute())
 {
  EBJK->Text=ExtractFilePath(JKDialog->FileName);
 }
}
//---------------------------------------------------------------------------

void __fastcall TOptions::BNMOTSClick(TObject *Sender)
{
 MOTSDialog->InitialDir=EBMOTS->Text;
 MOTSDialog->FileName="";
 if (MOTSDialog->Execute())
 {
  EBMOTS->Text=ExtractFilePath(MOTSDialog->FileName);
 }

}
//---------------------------------------------------------------------------

