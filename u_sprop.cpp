//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "u_sprop.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TServPropForm *ServPropForm;
//---------------------------------------------------------------------------
__fastcall TServPropForm::TServPropForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

bool TServPropForm::ServerProperties(TServerData * sd,bool editable)
{
 EBIP->Text=sd->ip;
 Memo->Text=sd->comment;
 EBIP->ReadOnly=!editable;
 Memo->ReadOnly=!editable;

 EBIP->Color=editable ? clWindow: clBtnFace;
 Memo->Color=editable ? clWindow: clBtnFace;

 if (editable) ActiveControl=EBIP;

 if (ShowModal()!=mrOk) return false;
 if (!editable) return false;
 
 sd->ip=EBIP->Text;
 sd->comment=Memo->Text;
 return true;

}


bool TServPropForm::ActiveServerProperties(char * ip,char * comment)
{
 EBIP->ReadOnly=true;
 Memo->ReadOnly=true;
 EBIP->Color=clBtnFace;
 Memo->Color=clBtnFace;

 EBIP->Text=ip;
 Memo->Text=comment;
 ShowModal();
 return false;
}

void __fastcall TServPropForm::FormCreate(TObject *Sender)
{
 ClientWidth=Memo->Left*2+Memo->Width;    
 ClientHeight=Memo->Left+BNOK->Top+BNOK->Height;    
}
//---------------------------------------------------------------------------

