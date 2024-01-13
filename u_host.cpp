//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "u_host.h"
#include "utils.h"
#include "mainform.h"
#include "u_http.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
THostForm *HostForm;
//---------------------------------------------------------------------------
__fastcall THostForm::THostForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void THostForm::LoadIPBoards()
{
 int i;
 TList * ipboards=GetIPBoards();
 TIPBoard * board;
 TCheckListBox * cbl;

 LBPostJK->Items->Clear();
 LBPostMOTS->Items->Clear();
 LBPostOL->Items->Clear();

 if (ipboards)
 for (i=0;i<ipboards->Count;i++)
 {
  board=(TIPBoard *)ipboards->Items[i];
  switch (board->game)
  {
   case game_JK: cbl=LBPostJK; break;
   case game_MOTS: cbl=LBPostMOTS; break;
   case game_OL: cbl=LBPostOL; break;
  default: continue;
  }

  cbl->Items->AddObject(board->name,board);
  cbl->Checked[cbl->Items->Count-1]=board->checked;
 }
}

#define st_comment 0
#define st_duration 1
#define st_rank 2
#define st_nplayers 3
#define st_level 4
#define st_mod 5


AnsiString THostForm::GetString(int what)
{
 if (PGGames->ActivePage==TBJK) // JK
 switch (what)
 {
  case st_comment: {
                    int i;
                    AnsiString s;
                    for (i=0;i<JKMemo->Lines->Count;i++)
                     if (i==0) s=JKMemo->Lines->Strings[0];
                     else s=s+" "+JKMemo->Lines->Strings[i];
                    return s;
                   }
  case st_duration: return CBJKTime->Text;
  case st_rank: return CBJKRank->Text;
  case st_nplayers: return CBJKPlayers->Text;
  case st_level: return CBJKLevel->Text;
  case st_mod: return CBJKMod->Text;
 }

 if (PGGames->ActivePage==TBMOTS) // MOTS
 switch (what)
 {
  case st_comment: return MMemo->Text;
  case st_duration: return CBMTime->Text;
  case st_rank: return CBMRank->Text;
  case st_nplayers: return CBMPlayers->Text;
  case st_level: return CBMLevel->Text;
  case st_mod: return CBMMod->Text;
 }

 if (PGGames->ActivePage==TBOL) // Outlaws
 switch (what)
 {
  case st_comment: return OLMemo->Text;
  case st_duration: return CBOLTime->Text;
 }


 return "";
}

AnsiString validate(AnsiString& str)
{
  AnsiString s;
  char c;
  int i;
  s="";
  for (i=1;i<=str.Length();i++)
  {
   c=str[i];
   if ((c>='0') && (c<='9') ||
       (c>='a') && (c<='z') ||
       (c>='A') && (c<='Z') ||
       (c=='.')) s=s+c;
   else if (c==' ') s=s+'+';
   else s=s+'%'+IntToHex(c,2);
  }
  return s;
}



void THostForm::PostToBoards(TCheckListBox * cbl)
{
 int i;
 AnsiString params,pstr,pvar;
 TIPBoard * board;
 int p;

 for (i=0;i<cbl->Items->Count;i++)
 if (cbl->Checked[i])
 {
  board=(TIPBoard*)cbl->Items->Objects[i];
  params="";
  pstr=board->params;
  // generate parameter string
  while (pstr.Length())
  {
   p=pstr.Pos("$");
   if (p==0) { params=params+pstr; break; } else
   params=params+pstr.SubString(1,p-1);
   pvar=pstr.SubString(p,3);
   if (pvar=="$ip") params=params+GetMyIP();
   else if (pvar=="$lv") params=params+validate(GetString(st_level));
   else if (pvar=="$md") params=params+validate(GetString(st_mod));
   else if (pvar=="$rk") params=params+validate(GetString(st_rank));
   else if (pvar=="$np") params=params+validate(GetString(st_nplayers));
   else if (pvar=="$cm") params=params+validate(GetString(st_comment));
   else if (pvar=="$tm") params=params+validate(GetString(st_duration));
   pstr.Delete(1,p+2);
  }

  if (board->is_method_post) HttpForm->CGIPost(board->url.c_str(),params.c_str());
  else HttpForm->CGIGet((board->url+"?"+params).c_str());
 }
}

void THostForm::HostDialog()
{
 if (CBJKTime->ItemIndex==-1) CBJKTime->ItemIndex=1;
 if (CBMTime->ItemIndex==-1) CBMTime->ItemIndex=1;
 if (CBOLTime->ItemIndex==-1) CBOLTime->ItemIndex=1;

 LoadIPBoards();

 if (ShowModal()!=mrOk) return;

 if (PGGames->ActivePage==TBJK) // JK
 {
  PostToBoards(LBPostJK);

  JSForm->DisableListenPorts();
  SetCmdAddOnDir(true,false); //CBJKAddon->Checked
  HostGame(game_JK);
 }

 if (PGGames->ActivePage==TBMOTS) // MOTS
 {
  PostToBoards(LBPostMOTS);
  JSForm->DisableListenPorts();
  SetCmdAddOnDir(true,false); //CBMOTSAddon->Checked
  HostGame(game_MOTS);
 }

 if (PGGames->ActivePage==TBOL) // Outlaws
 {
  PostToBoards(LBPostOL);
  JSForm->DisableListenPorts();
   HostOLGame(RGOLNDriver->ItemIndex==0);
 }


}


void __fastcall THostForm::BNJKDefaultsClick(TObject *Sender)
{
 LoadIPBoards();
 CBJKTime->ItemIndex=1;
 CBJKRank->Text=8;
 CBJKPlayers->Text=4;
 CBJKLevel->Text="Rotating Levels";
 CBJKMod->Text="none";
}
//---------------------------------------------------------------------------

void __fastcall THostForm::BNMDefaultsClick(TObject *Sender)
{
 LoadIPBoards();
 CBMTime->ItemIndex=1;
 CBMRank->Text=8;
 CBMPlayers->Text=4;
 CBMLevel->Text="Rotating Levels";
 CBMMod->Text="none";
}
//---------------------------------------------------------------------------


void __fastcall THostForm::BNOLDefaultsClick(TObject *Sender)
{
 LoadIPBoards();
 RGOLNDriver->ItemIndex=0;
 CBOLTime->Text="2";
}
//---------------------------------------------------------------------------



