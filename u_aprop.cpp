//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "u_aprop.h"
#include "p_thread.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAPropForm *APropForm;
//---------------------------------------------------------------------------
__fastcall TAPropForm::TAPropForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TAPropForm::ShowProperties(char * ip,TActiveServerData * asd)
{
 TStrings * sl;
 AnsiString s;

 EBIP->Text=ip;

 sl=MDetails->Lines;

 sl->Clear();

 s="Game: "+GetGameName(&asd->guidGame);
 sl->Add(s);
 s="Name: ";
 s=s+asd->name;
 sl->Add(s);
 s="Level: ";
 s=s+asd->level;
 sl->Add(s);

 switch (WhatGame(&asd->guidGame))
 {
  case game_JK:
   {
     s="Max Rank: "+IntToStr(HIWORD(asd->user3));
     sl->Add(s);
     if ((asd->user2 & 0x80)==0) sl->Add(" Rotating Levels");
     if (asd->user2 & 0x103) sl->Add(" Team Play");
     if (asd->user2 & 0x8) sl->Add(" Time Limit");
     if (asd->user2 & 0x10) sl->Add(" Score Limit");
   }; break;
  case game_MOTS:
  {
     if (asd->user2 & 0x1000) sl->Add("Max Rank: "+IntToStr(HIWORD(asd->user3)));
     else sl->Add(" Personalities");
     if ((asd->user2 & 0x80)==0) sl->Add(" Rotating Levels");
     if (asd->user2 & 0x103) sl->Add(" Team Play");
     if (asd->user2 & 0x8) sl->Add(" Time Limit");
     if (asd->user2 & 0x10) sl->Add(" Score Limit");
  };break;
  case game_OL:
  {
   bool winsock=asd->guidInst==GUID_NULL;
   if (!winsock) sl->Add(" Protocol: DirectPlay");
   else
   {
    int a,h,m,s;
    h=asd->user1/3600;
    a=asd->user1-h*3600;
    m=a/60;
    s=a-m*60;
    sl->Add(" Protocol: Winsock");
    sl->Add(" Playing for "+IntToStr(h)+":"+IntToStr(m)+":"+IntToStr(s));
   } 
  }; break;
  default:;
 }

 LBPlayers->Caption="Players: ("+IntToStr(asd->nplayers)+"/"+IntToStr(asd->maxplayers)+")";

 sl=MPlayers->Lines;
 sl->Assign(asd->players);
// sl->Text=asd->players;

 MComment->Lines->Text=asd->comment;

 ShowModal();

}
void __fastcall TAPropForm::FormCreate(TObject *Sender)
{
 ClientWidth=MComment->Left*2+MComment->Width;    
 ClientHeight=BNClose->Top+BNClose->Height+MComment->Left;    
}
//---------------------------------------------------------------------------

