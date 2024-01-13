//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <registry.hpp>
#pragma hdrstop

#include "mainform.h"
#include "utils.h"
#include "u_sprop.h"
#include "u_host.h"
#include "u_about.h"
#include "u_pollip.h"
#include "u_aprop.h"

#include "u_http.h"
#include "u_ftp.h"
#include "u_askdl.h"
#include "U_unzip.h"
#include "U_options.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "WSocket"
#pragma resource "*.dfm"
TJSForm *JSForm;


void NoServMsg()
{
 ShowMessage("No server selected!");
}

void NoAServMsg()
{
 ShowMessage("No active server selected!");
}

void NoNSel()
{
 ShowMessage("No groups selected");
}

void ReadOnlyMsg()
{
 ShowMessage("This group can't be modified");
}

//---------------------------------------------------------------------------
__fastcall TJSForm::TJSForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TJSForm::Exit1Click(TObject *Sender)
{
 Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::PollserversClick(TObject *Sender)
{
 int i,n;
 TListItem * li;
 n=0;
 for (i=0;i<LVServList->Items->Count;i++)
 {
  li=LVServList->Items->Item[i];
  if ((li->Selected) || (li->Focused))
  {
    pthread->PollServer(li->Caption,((TServerData *)li->Data)->comment,"");
    n++;
  }
 }
 if (n==0) Application->MessageBox("No servers selected in 'Servers' window","Warning",MB_OK);
}
//---------------------------------------------------------------------------

void TJSForm::PollAllServers(TStringList * sl)
{
 int i;
 if (!sl) return;
 for (i=0;i<sl->Count;i++)
  pthread->PollServer(sl->Strings[i],
  ((TServerData *)sl->Objects[i])->comment,"");
};

//---------------------------------------------------------------------------


void __fastcall TJSForm::FormCreate(TObject *Sender)
{

 int i;
 pthread=new TServerPollThread(LVActiveServs,ServerSocket,DPUDP,OLUDP,Timer);

 chdir(ExtractFilePath(ParamStr(0)).c_str());

 LoadGroupFile("lists.dat",TVServTree->Items);
 LoadIPBoards("lists.dat");

 CreateCopyToMenu(CopyServerTo);
 CreateCopyToMenu(CopyServerTo1);
 ASCreateCopyToMenu(CopyServerTo2);
 ASCreateCopyToMenu(CopyServerTo3);

 for (i=0;i<TVServTree->Items->Count;i++)
 {
  if (IsNodeLocal(TVServTree->Items->Item[i]))
     UpdateNodeData((TNodeData *)(TVServTree->Items->Item[i])->Data);
 }

 TVServTree->FullExpand();

 TRegistry * reg;

 reg=new TRegistry;

 reg->RootKey=HKEY_CLASSES_ROOT;
 reg->OpenKey("rdroid",true);
 reg->WriteString("","RD URL");
 reg->WriteString("URL Protocol","");
 reg->CloseKey();

 reg->OpenKey("rdroid\\shell\\open\\command",true);
 reg->WriteString("",ParamStr(0)+" %1");
 reg->CloseKey();

 reg->Free();

 if (ParamCount()>0) HandleCommandLine(ParamStr(1).c_str());

}
//---------------------------------------------------------------------------



void __fastcall TJSForm::TVServTreeChange(TObject *Sender, TTreeNode *Node)
{
 TNodeData * nd=(TNodeData *)Node->Data;
 TServerData * sd;
 int i;
 TListItem * li;

 LVServList->Items->BeginUpdate();
 LVServList->Items->Clear();
 LVServList->Items->EndUpdate();

 MAddServer->Visible=IsNodeEditable(Node);
 DeleteServers->Visible=MAddServer->Visible;
 AddServer1->Visible=MAddServer->Visible;
 DeleteServers1->Visible=MAddServer->Visible;

 if (!nd->servlist) return;

 LVServList->Items->BeginUpdate();
 for (i=0;i<nd->servlist->Count;i++)
 {
  li=LVServList->Items->Add();
  li->Caption=nd->servlist->Strings[i];
  li->Data=nd->servlist->Objects[i];
  sd=(TServerData *)li->Data;
  li->SubItems->Add(sd->comment);
 }
 LVServList->Items->EndUpdate();

}
//---------------------------------------------------------------------------
void TJSForm::UpdateNode(TTreeNode * node)
{
 int i;
 UpdateNodeData((TNodeData *)node->Data);
 for (i=0;i<node->Count;i++)
  UpdateNode(node->Item[i]);
}


void __fastcall TJSForm::UpdateNodesClick(TObject *Sender)
{
 if (TVServTree->Selected)
 {
  UpdateNode(TVServTree->Selected);
  TVServTreeChange(TVServTree,TVServTree->Selected);
 } else NoNSel();
}
//---------------------------------------------------------------------------


void TJSForm::PollNode(TTreeNode * node)
{
 int i;
 PollAllServers(((TNodeData *)node->Data)->servlist);
 for (i=0;i<node->Count;i++)
  PollNode(node->Item[i]);
}

void __fastcall TJSForm::FindactiveserversClick(TObject *Sender)
{
 if (TVServTree->Selected) PollNode(TVServTree->Selected);
 else NoNSel();
}
//---------------------------------------------------------------------------

void TJSForm::UpdateActiveOnes(bool all)
{
 int i;
 TListItem * li;
 for (i=0;i<LVActiveServs->Items->Count;i++)
 {
  li=LVActiveServs->Items->Item[i];
  if (all || (li->Selected) || (li->Focused))
    pthread->UpdateServer(li->Caption.c_str(),li);
 }
}

void TJSForm::DeleteListItem(TListItem * li)
{
 if (!li) return;
 if (li->Data) ((TObject*)li->Data)->Free();
 li->Delete();
}


void __fastcall TJSForm::UpdateAServClick(TObject *Sender)
{
 UpdateActiveOnes(false);
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::Updateall1Click(TObject *Sender)
{
 UpdateActiveOnes(true);
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::RemoveClick(TObject *Sender)
{
 DeleteListItem(LVActiveServs->ItemFocused);
}
//---------------------------------------------------------------------------


void __fastcall TJSForm::LVServListDblClick(TObject *Sender)
{
 Pollservers->Click();
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::LVActiveServsDblClick(TObject *Sender)
{
 UpdateAServ->Click();
}
//---------------------------------------------------------------------------


void __fastcall TJSForm::UpdateFindActiveClick(TObject *Sender)
{
 UpdateNodes->Click();
 Findactiveservers->Click();
}
//---------------------------------------------------------------------------


void __fastcall TJSForm::RemoveAllClick(TObject *Sender)
{
 int i;
 for (i=0;i<LVActiveServs->Items->Count;i++)
  delete LVActiveServs->Items->Item[i]->Data;
 LVActiveServs->Items->Clear();
}
//---------------------------------------------------------------------------

bool TJSForm::IsNodeLocal(TTreeNode * node)
{
 TNodeData * nd=(TNodeData *)node->Data;
 if (nd->url=="ipx:") return true;
 if (strcmpi(nd->format,"text")!=0) return false;
 return IsNodeEditable(node);
}

bool TJSForm::IsNodeEditable(TTreeNode * node)
{
 if (!node) return false;

 TNodeData * nd=(TNodeData *)node->Data;
 if (strncmpi(nd->url.c_str(),"http:",5)==0) return false;
 if (nd->url=="ipx:") return false;
 if (nd->url=="") return false;
 return true;
}

void __fastcall TJSForm::LVActiveServsKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key==VK_DELETE) Remove->Click();
  if (Key==VK_RETURN && Shift.Contains(ssAlt)) PropertiesAServ->Click();
}
//---------------------------------------------------------------------------


void __fastcall TJSForm::TVServTreeDblClick(TObject *Sender)
{
 UpdateFindActive->Click();
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::Properties2Click(TObject *Sender)
{
 TListItem * li;
 li=LVServList->ItemFocused;
 if (!li) { NoServMsg(); return; }
 if (ServPropForm->ServerProperties((TServerData *)li->Data,
                                 IsNodeEditable(TVServTree->Selected)))
 {
  li->Caption=((TServerData *)li->Data)->ip;
  ((TNodeData *)TVServTree->Selected->Data)->servlist->Strings[li->Index]=li->Caption;
  li->SubItems->Strings[0]=((TServerData *)li->Data)->comment;
  SaveNode(TVServTree->Selected);
 }
}
//---------------------------------------------------------------------------

void TJSForm::DeleteServer(int n)
{
 TNodeData * nd;
 TServerData * sd;
 if (!TVServTree->Selected) { NoNSel(); return;} 
 if (!IsNodeEditable(TVServTree->Selected)) { ReadOnlyMsg(); return;}
 nd=(TNodeData *)TVServTree->Selected->Data;
 if (!nd->servlist) return;
 sd=(TServerData *)nd->servlist->Objects[n];
 LVServList->Items->Delete(n);
 nd->servlist->Delete(n);
 delete sd;
 SaveNode(TVServTree->Selected);
}

void TJSForm::AddServer(const AnsiString & ip,const AnsiString & comment)
{
 TNodeData * nd;
 TServerData * sd;
 TListItem * li;
 if (!TVServTree->Selected) { NoNSel(); return;}
 if (!IsNodeEditable(TVServTree->Selected)) { ReadOnlyMsg(); return;}
 nd=(TNodeData *)TVServTree->Selected->Data;
 sd=new TServerData;

 sd->ip=ip;
 sd->comment=comment;

 if (!nd->servlist) nd->servlist=new TStringList;

 nd->servlist->AddObject(ip,(TObject *)sd);

 li=LVServList->Items->Add();
 li->Data=sd;
 li->Caption=ip;
 li->SubItems->Add(comment);
 SaveNode(TVServTree->Selected);
}

void TJSForm::SaveNode(TTreeNode * node)
{
 TNodeData * nd=(TNodeData *)node->Data;
 if (!IsNodeEditable(node)) return;
 chdir(ExtractFilePath(ParamStr(0)).c_str());
 SaveServerList(nd->url,nd->servlist);
}


void __fastcall TJSForm::DeleteServersClick(TObject *Sender)
{
 if (Application->MessageBox(
  "Are you sure you want to delete the selected servers?",
  "Confirmation",MB_YESNO)!=IDYES) return;

 int i;
 TListItem * li;
 for (i=LVServList->Items->Count-1;i>=0;i--)
 {
  li=LVServList->Items->Item[i];
  if (li->Selected) DeleteServer(i);
 }

}
//---------------------------------------------------------------------------

void __fastcall TJSForm::MAddServerClick(TObject *Sender)
{
 TServerData * sd;
 sd=new TServerData;
// memset(sd,0,sizeof(*sd));
 if (ServPropForm->ServerProperties(sd,true))
 {
  AddServer(sd->ip,sd->comment);
 }
 delete sd;
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::PropertiesAServClick(TObject *Sender)
{
 TListItem * li=LVActiveServs->ItemFocused;
 if (!li) { NoAServMsg(); return; }
 TActiveServerData * asd=(TActiveServerData *)li->Data;

 APropForm->ShowProperties(li->Caption.c_str(),asd);
}
//---------------------------------------------------------------------------



void __fastcall TJSForm::Hostgame1Click(TObject *Sender)
{
 HostForm->HostDialog();
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::Cancelpolling2Click(TObject *Sender)
{
 Cancelpolling->Click();    
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::CancelpollingClick(TObject *Sender)
{
 pthread->CancelPolling();    
}
//---------------------------------------------------------------------------

void TJSForm::DisableListenPorts()
{
 pthread->DisableListenPorts();
}


bool IsURLFtp(const AnsiString& url)
{
 return CompareText(url.SubString(1,6),"ftp://")==0;
}

void __fastcall TJSForm::ConnectClick(TObject *Sender)
{
 if (!LVActiveServs->ItemFocused) { NoAServMsg(); return; }

 TActiveServerData* asd=(TActiveServerData *)LVActiveServs->ItemFocused->Data;
 AnsiString ip=LVActiveServs->ItemFocused->Caption;

int LevLocation;
do
{
 switch (WhatGame(&asd->guidGame))
 {
  case game_JK: LevLocation=LocateJKLevel(asd->level); break;
  case game_MOTS: LevLocation=LocateMOTSLevel(asd->level); break;
  case game_OL: LevLocation=LocateOutlawsLevel(asd->level); break;
  default: LevLocation=lv_GameDir;
 }

 if (LevLocation==lv_NotFound)
 switch (AskDLForm->AskDownload(asd))
 {
  case dl_Cancel: return;
  case dl_Ignore: LevLocation=lv_GameDir; break;
  case dl_Download:
  {
   AnsiString file;
   char tmp[256];

   file=AskDLForm->url.c_str();

   int i;
   for (i=1;i<=file.Length();i++)
    if (file[i]=='/') file[i]='\\';

   GetTempPath(sizeof(tmp),tmp);
   file=tmp+ExtractFileName(file);

   AnsiString AddOnDir;

  switch (WhatGame(&asd->guidGame))
  {
   case game_JK: AddOnDir=GetJKAddOnDir(); break;
   case game_MOTS: AddOnDir=GetMOTSAddOnDir(); break;
   case game_OL: AddOnDir=GetOutlawsDir(); break;
   default: LevLocation=lv_GameDir;
  }

   bool dlgood;

   if (IsURLFtp(AskDLForm->url)) dlgood=FtpForm->DownloadURL(AskDLForm->url,file);
   else dlgood=HttpForm->DownloadURL(AskDLForm->url,file);
   if (dlgood)
   {
       if (!UnZipForm->Unzip(file,AddOnDir)) ShowMessage("Error unzipping "+file);
   }
   LevLocation=lv_NotFound; // To check if the file really contains the level

  }
 } // switch
} while (LevLocation==lv_NotFound);

 char * s;
 HGLOBAL hs;
 OpenClipboard(0);
 hs=GlobalAlloc(GMEM_MOVEABLE,ip.Length()+1);
 s=(char*)GlobalLock(hs);
 StrCopy(s,ip.c_str());
 GlobalUnlock(hs);
 SetClipboardData(CF_TEXT,hs);
 CloseClipboard();

  switch (WhatGame(&asd->guidGame))
  {
   case game_JK: SetCmdAddOnDir(false,LevLocation==lv_AddOnDir); break;
   case game_MOTS: SetCmdAddOnDir(true,LevLocation==lv_AddOnDir); break;
  }


 DisableListenPorts();
 JoinGame(ip.c_str(),asd);
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::About1Click(TObject *Sender)
{
 AboutForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::WMTakeURL(TMessage &Message)
{
 char url[1024];
 GlobalGetAtomName((ATOM)Message.WParam,url,sizeof(url));

 HandleCommandLine(url);

 Message.Result=URLTAKEN;
}

void TJSForm::HandleCommandLine(char * cmd)
{
 char *p,*pe;
 if (strncmpi(cmd,"rdroid:",7)!=0) return;
 p=cmd+7;
 if (*p=='/') p++;
 if (*p=='/') p++;
 pe=StrScan(p,'/');
 if (pe) *pe='\0';
 pe=StrScan(p,'?');
 if (pe) *pe='\0';
 pthread->PollServer(p,"","");
}

void __fastcall TJSForm::Joingame1Click(TObject *Sender)
{
  Connect->Click();    
}
//---------------------------------------------------------------------------



void __fastcall TJSForm::SelectAll1Click(TObject *Sender)
{
 int i;
 for (i=0;i<LVServList->Items->Count;i++) LVServList->Items->Item[i]->Selected=true;
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::Updatethislist1Click(TObject *Sender)
{
 UpdateNodes->Click();
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::UpdateAllLists1Click(TObject *Sender)
{
 int i;
 for (i=0;i<TVServTree->Items->Count;i++)
 {
  UpdateNodeData((TNodeData *)TVServTree->Items->Item[i]->Data);
 }
 PollAllLists->Click();
 TVServTreeChange(TVServTree,TVServTree->Selected);
}
//---------------------------------------------------------------------------








void __fastcall TJSForm::PollIP1Click(TObject *Sender)
{
  AnsiString s;
  s=PollIP->GetIP();
  if (s!="") pthread->PollServer(s.c_str(),"","");
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::CopyServerToClick(TObject *Sender)
{
 int n=((TMenuItem *)Sender)->Tag;
 TTreeNode * node=TVServTree->Items->Item[n];
 TNodeData * nd=(TNodeData *)node->Data;
 TListItem * li;
 TServerData * sd,*nsd;
 int i;

 if (!IsNodeEditable(node)) return;

 for (i=0;i<LVServList->Items->Count;i++)
 {
  li=LVServList->Items->Item[i];
  if (li->Selected || li->Focused)
  {
   n=nd->servlist->IndexOf(li->Caption);
   if (n==-1)
   {
    nsd=new TServerData;
    nd->servlist->AddObject(li->Caption,(TObject *)nsd);
   } else nsd=(TServerData *)nd->servlist->Objects[n];

   sd=(TServerData *)li->Data;
   nsd->comment=sd->comment;
  }
 }

 SaveNode(node);

}
//---------------------------------------------------------------------------

void TJSForm::CreateCopyToMenu(TMenuItem * mi)
{
 int i;
 TTreeNode * node;
 TMenuItem * ni;
 for (i=mi->Count-1;i>=0;i--) mi->Delete(i);

 for (i=0;i<TVServTree->Items->Count;i++)
 {
  node=TVServTree->Items->Item[i];
  if (!IsNodeEditable(node)) continue;
  ni=new TMenuItem(mi);
  ni->Caption=node->Text;
  ni->Tag=node->AbsoluteIndex;
  ni->OnClick=CopyServerToClick;
  mi->Add(ni);
 }

}

void __fastcall TJSForm::CopyServerTo2Click(TObject *Sender)
{
 int n=((TMenuItem *)Sender)->Tag;
 TTreeNode * node=TVServTree->Items->Item[n];
 TNodeData * nd=(TNodeData *)node->Data;
 TListItem * li;
 TActiveServerData * asd;
 TServerData *nsd;

 if (!IsNodeEditable(node)) return;

 li=LVActiveServs->ItemFocused;
 if (!li) return;

 n=nd->servlist->IndexOf(li->Caption);
 if (n==-1)
 {
  nsd=new TServerData;
  nd->servlist->AddObject(li->Caption,(TObject *)nsd);
 } else nsd=(TServerData *)nd->servlist->Objects[n];

 asd=(TActiveServerData *)li->Data;
 nsd->comment=asd->comment;
 SaveNode(node);
 TVServTreeChange(TVServTree,TVServTree->Selected);
}
//---------------------------------------------------------------------------

void TJSForm::ASCreateCopyToMenu(TMenuItem * mi)
{
 int i;
 TTreeNode * node;
 TMenuItem * ni;
 for (i=mi->Count-1;i>=0;i--) mi->Delete(i);

 for (i=0;i<TVServTree->Items->Count;i++)
 {
  node=TVServTree->Items->Item[i];
  if (!IsNodeEditable(node)) continue;
  ni=new TMenuItem(mi);
  ni->Caption=node->Text;
  ni->Tag=node->AbsoluteIndex;
  ni->OnClick=CopyServerTo2Click;
  mi->Add(ni);
 }

}

void __fastcall TJSForm::LVServListKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key==VK_DELETE) DeleteServers->Click();
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::Options1Click(TObject *Sender)
{
 Options->SetOptions();    
}
//---------------------------------------------------------------------------

void __fastcall TJSForm::PollAllListsClick(TObject *Sender)
{
 int i;
 for (i=0;i<TVServTree->Items->Count;i++)
 {
  PollAllServers(((TNodeData *)TVServTree->Items->Item[i]->Data)->servlist);
 }
}
//---------------------------------------------------------------------------

