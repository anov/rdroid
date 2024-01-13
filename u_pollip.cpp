//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "u_pollip.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPollIP *PollIP;
//---------------------------------------------------------------------------
__fastcall TPollIP::TPollIP(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

AnsiString __fastcall TPollIP::GetIP()
{
 if (ShowModal()!=mrOk) return "";
 if (CBIP->Items->IndexOf(CBIP->Text)==-1)
    CBIP->Items->Add(CBIP->Text);
 return CBIP->Text.c_str();
}

