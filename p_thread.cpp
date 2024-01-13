//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <registry.hpp>

#include "p_thread.h"
#include "dx6\dplay.h"
#include "dx6\dplobby.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TServerPollThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }

_GUID GUID_JK=StringToGUID("{BF0613C0-DE79-11d0-99C9-00A02476AD4B}");
_GUID GUID_MOTS=StringToGUID("{A4FF3960-3B4F-11d1-96CB-444553540000}");
_GUID GUID_OL=StringToGUID("{BAD0DE20-A4FF-11CF-867C-444553540000}");


#define SWAPWORD(x) ((x >>8) & 0xFF)+((x & 0xFF)<<8)

//typedef struct TServData {
// bool polled;
// timeb start;
// bool responded;
// TListItem * li;
//} TServData, *PServData;


void CheckHR(int hr)
{
 if (hr)
 {
  hr=0;
 }
}

void copySD2CBD(const DPSESSIONDESC2 * lpThisSD,TCallBackData * pcbdata)
{
 pcbdata->guidInst=lpThisSD->guidInstance;
 pcbdata->guidGame=lpThisSD->guidApplication;
 pcbdata->nplayers=lpThisSD->dwCurrentPlayers;
 pcbdata->maxplayers=lpThisSD->dwMaxPlayers;
 pcbdata->user2=lpThisSD->dwUser2;
 pcbdata->user3=lpThisSD->dwUser3;

 pcbdata->user3=lpThisSD->dwUser3;

 if ((lpThisSD->guidApplication==GUID_JK) || (lpThisSD->guidApplication==GUID_MOTS))
 {
  char * p, *e;
  p=lpThisSD->lpszSessionNameA;
  e=StrScan(p,':');
  if (e) { pcbdata->gname=p; pcbdata->gname.SetLength(e-p); }
  p=e+1;
  if (e) pcbdata->level=p;
 } else pcbdata->gname=lpThisSD->lpszSessionNameA;
}

void GetOLData(TCallBackData * cd,char * buf)
{
// "XESD\x06LOCATION\n   Dry Gulch\nDeath Match\nPlayers: (1 / 8)\nStarted By:\n   Chip\nGame Time: 00:00:11"
 char *p,*pe;
 cd->guidGame=GUID_OL;

 p=buf;
 AnsiString sbuf=p;
 pe=StrScan(p,'\n');
 if (!pe) return;
 p=pe+1;

 pe=StrScan(p,'\n');
 if (!pe) pe=StrEnd(p);
 cd->level=Trim(sbuf.SubString(p-buf+1,pe-p));
 if (!*pe) return;
 p=pe+1;

 pe=StrScan(p,'\n');
 if (!pe) pe=StrEnd(p);
 cd->gname=sbuf.SubString(p-buf+1,pe-p);
 if (!*pe) return;
 p=pe+1;

 // Players
 pe=StrScan(p,'\n');
 if (!pe) pe=StrEnd(p);
 sscanf(p,"Players: (%d / %d)",&cd->nplayers,&cd->maxplayers);
 //
 if (!*p) return;
 p=pe+1;

 pe=StrScan(p,'\n');
 if (!pe) return;
 p=pe+1;

 // Started by ?
 pe=StrScan(p,'\n');
 if (!pe) pe=StrEnd(p);
 cd->gname.Insert(Trim(sbuf.SubString(p-buf+1,pe-p))+", ",1);
 if (!*p) return;
 p=pe+1;

 // Game time
 int h,m,s;
 pe=StrScan(p,'\n');
 if (!pe) pe=StrEnd(p);
 sscanf(p,"Game Time: %d:%d:%d",&h,&m,&s);
 cd->user1=h*3600+m*60+s;
 ///

}

BOOL FAR PASCAL EnumPlayers(
    DPID            dpId,
    DWORD           dwPlayerType,
    LPCDPNAME       lpName,
    DWORD           dwFlags,
    LPVOID          lpContext )
{
 PCallBackData pcbdata=(PCallBackData)lpContext;

 pcbdata->players->Add(lpName->lpszShortNameA);
 return true;
}

BOOL FAR PASCAL EnumSessions(
    LPCDPSESSIONDESC2   lpThisSD,
    LPDWORD             lpdwTimeOut,
    DWORD               dwFlags,
    LPVOID              lpContext )
{
 if (dwFlags & DPESC_TIMEDOUT)
 {
   return false;
 }

 PCallBackData pcbdata=(PCallBackData)lpContext;

 pcbdata->active=true;
 copySD2CBD(lpThisSD,pcbdata);
 pcbdata->players->Clear();

 CheckHR(
 pcbdata->idp->EnumPlayers((_GUID *)&lpThisSD->guidInstance,EnumPlayers,pcbdata,DPENUMPLAYERS_SESSION)
 );

 ((TServerPollThread*)pcbdata->thread)->CallBack(pcbdata);
 return true;
}




LPDIRECTPLAYLOBBY2A CreateDPLobby()
{
 LPDIRECTPLAYLOBBY idpl1=NULL;
 LPDIRECTPLAYLOBBY2A idpl=NULL;
 int hr;

 hr=DirectPlayLobbyCreate(NULL,&idpl1,NULL,NULL,0);

 CheckHR(hr);

 hr=idpl1->QueryInterface(IID_IDirectPlayLobby2A,(LPVOID *)&idpl);
 idpl1->Release();
 CheckHR(hr);

 if (hr) ShowMessage("Couldn't initialize DirectPlay");

 return idpl;
}

void * CreateDPAddress(LPDIRECTPLAYLOBBY2A idpl,char * ip,ULONG* size)
{
 int hr,nel;
 void * adr=&hr;
 DPCOMPOUNDADDRESSELEMENT    ad[2];


if (strcmp(ip,"ipx:")==0)
{
 ad[0].guidDataType=DPAID_ServiceProvider;
 ad[0].dwDataSize=sizeof(TGUID);
 ad[0].lpData=(void *)&DPSPGUID_IPX;
 nel=1;
} else
{
 ad[0].guidDataType=DPAID_ServiceProvider;
 ad[0].dwDataSize=sizeof(TGUID);
 ad[0].lpData=(void *)&DPSPGUID_TCPIP;

 ad[1].guidDataType=DPAID_INet;
 ad[1].dwDataSize=strlen(ip)+1;
 ad[1].lpData=ip;
 nel=2;
}

 *size=0;
 hr=idpl->CreateCompoundAddress(&ad[0],nel,adr,size);

// hr=idpl->CreateAddress(DPSPGUID_TCPIP,DPAID_INet,ip,strlen(ip)+1,adr,size);

 if (hr!=DPERR_BUFFERTOOSMALL) CheckHR(hr);

 adr=malloc(*size);

// hr=idpl->CreateAddress(DPSPGUID_TCPIP,DPAID_INet,ip,strlen(ip)+1,adr,size);

hr=idpl->CreateCompoundAddress(&ad[0],nel,adr,size);
 CheckHR(hr);

 return adr;
}

int dplay_PollIPX(TCallBackData * pdata)
{
 LPDIRECTPLAY3A idp=NULL;
 LPDIRECTPLAYLOBBY2A idpl=NULL;

 LPDIRECTPLAY idp1=NULL;

 int hr;
 ULONG size;
 DPSESSIONDESC2 ds;
 void * adr;


 hr=DirectPlayCreate((_GUID *)&GUID_NULL,&idp1,NULL);
 CheckHR(hr);

 hr=idp1->QueryInterface(IID_IDirectPlay3A,(LPVOID *)&idp);
 CheckHR(hr);

 idp1->Release();

 idpl=CreateDPLobby();


// hr = CoCreateInstance(CLSID_DirectPlay, NULL, CLSCTX_INPROC_SERVER,
//						  IID_IDirectPlay3A, (LPVOID *) &idp);
// hr = CoCreateInstance(CLSID_DirectPlayLobby, NULL, CLSCTX_INPROC_SERVER,
//						  IID_IDirectPlayLobby2A, (LPVOID *) &idpl);

 adr=CreateDPAddress(idpl,"ipx:",&size);

 hr=idp->InitializeConnection(adr,0);
 CheckHR(hr);

 memset(&ds,0,sizeof(ds));
 ds.dwSize=sizeof(ds);
 ds.guidApplication=GUID_NULL;

 pdata->idp=idp;

 hr=idp->EnumSessions(&ds,0,EnumSessions,pdata,DPENUMSESSIONS_AVAILABLE);
 CheckHR(hr);

 hr=idp->Release();
 hr=idpl->Release();

 return 0;//curPing;

}

void TServerPollThread::EnableListenPorts()
{
 if (PortsActive) return;
 int n=0;
 bool started=false;

 while (n<5 && !started)
 {
 try
 {
  ServSock->Open();
  started=true;
 }
 __except(true)
 {
  n++;
  ServSock->Port=ServSock->Port+20;
 }
 }

 try
 {
 OLUDP->LocalPort= 5310;
 }
 __except(true)
 {
 }
 PortsActive=true;
}

void TServerPollThread::DisableListenPorts()
{
 if (ServSock->Active) ServSock->Close();
 OLUDP->LocalPort=0;
 PortsActive=false;
}

void TServerPollThread::SendOLRequest(const AnsiString& ip)
{
 int n=2;
 OLUDP->RemoteHost=ip;
 OLUDP->SendBuffer("ND",n,n);
}

void TServerPollThread::SendDPRequest(const AnsiString& ip)
{
 TDPMSG req;
 memset(&req,0,sizeof(req));
 req.size=sizeof(req);
 req.long1=0x2FAB0;
 req.port=SWAPWORD(ServSock->Port);
 req.short2=0x8019;
 req.short3=0x8a00;
 StrCopy(req.play,"play");
 req.type=0x4;
 req.short4=0x8;
 int n=sizeof(req);

 DPUDP->RemoteHost=ip;
 DPUDP->SendBuffer((char *)&req,n,n);
}


//---------------------------------------------------------------------------
__fastcall TServerPollThread::TServerPollThread(TListView * lv,TServerSocket * ssock,TNMUDP * dpudp,TNMUDP * oludp,TTimer * Timer):TThread(true)
{
 LView=lv;
// PBar=bar;
 squeue=new TStringList;
 iplist=new TStringList;
 iplist->Sorted=true;
 OLUDP=oludp;
 DPUDP=dpudp;
 timer=Timer;

 timer->Interval=1000;
 timer->Enabled=false;
 timer->OnTimer=TimerTimer;
 
 OLUDP->RemotePort=5310;
 DPUDP->RemotePort=47624;

 dns=new TDNS;
 dns->OnResolve=&ResolveHandler;
 ServSock=ssock;
 OLUDP->OnDataReceived=OLUDP_DataReceived;
 ServSock->OnClientRead=ServerSocketClientRead;
 PortsActive=false;
}

//---------------------------------------------------------------------------

void __fastcall TServerPollThread::ShowCurServ()
{
// pdata is the server's params
 AnsiString s="Polling ";
 s=s+pdata->ip;
 PanMessage(s.c_str());
}

void TServerPollThread::ClearPollQueue()
{
 squeue->Clear();
}

void __fastcall TServerPollThread::Execute()
{
    //---- Place thread code here ----
 while (!Terminated)
 {
  Sleep(10);
 }

}

void __fastcall TServerPollThread::UpdatePBar()
{
// PBar->Max=squeue->Count;
// PBar->Position=CurEntry;
}

void __fastcall TServerPollThread::ClearPBar()
{
// PBar->Max=1;
// PBar->Position=0;
// PanMessage("");
}


//---------------------------------------------------------------------------
int TServerPollThread::FindActiveServer(char * ip,const  _GUID guid)
{
 int n=0;
 TListItem * li;
 TActiveServerData * asd;

 do
 {
  li=LView->FindCaption(n, ip, false, true, false);
  if (!li) break;

  asd=(TActiveServerData *)li->Data;
  if (asd->guidInst==guid) return li->Index;
  n=li->Index+1;
 } while (true);

 return -1;
}


void __fastcall TServerPollThread::RemoveCurIP()
{
// pdata is current server data

 TListItem * li;

 li=NULL;
 if (LView->Items->IndexOf(pdata->li)!=-1) li=pdata->li;

 if (!li) li=LView->FindCaption(0, pdata->ip, false, true, false);

 if (li) li->SubItems->Strings[3]="lost";

}

void __fastcall TServerPollThread::UpdateLViewSync()
{
 UpdateLView(pdata);
}


void __fastcall TServerPollThread::UpdateLView(TCallBackData * pdata)
{
// pdata is server's data

 TActiveServerData * asd;
 char tmp[128];
 TListItem * li;
 int i;

 li=pdata->li;

 if (!li)
 {
  i=FindActiveServer(pdata->ip.c_str(),pdata->guidInst);
  if (i==-1)
  {
   li=LView->Items->Add();
   li->Data=new TActiveServerData;
   asd=(TActiveServerData *)li->Data;
  } else li=LView->Items->Item[i];
 }

 li->SubItems->Clear();
 asd=(TActiveServerData *)li->Data;


 asd->comment=pdata->comment;

 // StrLCopy(asd->players,pdata->players,sizeof(asd->players));

 asd->name=pdata->gname;
 asd->level=pdata->level;
 asd->players->Assign(pdata->players);
 asd->guidInst=pdata->guidInst;
 asd->guidGame=pdata->guidGame;
 asd->nplayers=pdata->nplayers;
 asd->maxplayers=pdata->maxplayers;
 asd->user1=pdata->user1;
 asd->user2=pdata->user2;
 asd->user3=pdata->user3;


 switch (WhatGame(&pdata->guidGame))
 {
  case game_JK: li->ImageIndex=1;break;
  case game_MOTS: li->ImageIndex=2;break;
  case game_OL: li->ImageIndex=3;break;
 default: li->ImageIndex=0;
 }

 li->Caption=pdata->ip;
 li->SubItems->Add(asd->name);
 li->SubItems->Add(asd->level);

 sprintf(tmp,"%d/%d",pdata->nplayers,pdata->maxplayers);
 li->SubItems->Add(tmp);

 if (pdata->ping==0) li->SubItems->Add("");
 else
 {
  int ping=pdata->ping;
  if (ping<0) { li->SubItems->Add(">5sec"); ping=5000; }
  else li->SubItems->Add(IntToStr(ping));

  if (!asd->nreplies) asd->avgping=ping;
  else asd->avgping=(asd->avgping*asd->nreplies+ping)/(asd->nreplies+1);
  asd->nreplies++;
 }

 if (asd->avgping==0) li->SubItems->Add("");
 else li->SubItems->Add(IntToStr(asd->avgping));

};



void TServerPollThread::CallBack(TCallBackData * pcbdata)
{
 pdata=pcbdata;
 Synchronize(&UpdateLViewSync);
}

void __fastcall TServerPollThread::UpdateServer(const AnsiString & ip, TListItem * li)
{
 if (ip=="ipx:") { PollIPX(); return; }


 PPollData pd=new TPollData;
 pd->comment=((TActiveServerData *)li->Data)->comment;
 pd->li=li;
 pd->addr=ip;
 dns->Resolve(ip.c_str(),pd);
}

void __fastcall TServerPollThread::CancelPolling()
{
 ClearPollQueue();
}

void TServerPollThread::RemoveOldIPs()
{
// Removes IPs that are older than 5 sec or responded already}
 timeb now;
 int i;
 PPollData pd;

 ftime(&now);
 for (i=iplist->Count-1;i>=0;i--)
 {
  pd=(PPollData)iplist->Objects[i];
  if (pd->responded || (now.time-pd->stime.time>=5))
  {
    if (!pd->responded && pd->li)  pd->li->SubItems->Strings[3]="t/o";
   iplist->Delete(i);
   pd->Free();
  }
 }
}


void TServerPollThread::ResolveHandler(const AnsiString& ip,void * data)
{
 RemoveOldIPs();
 TPollData * pd=(TPollData *)data;
 if (ip=="" || ip=="0.0.0.0") {delete pd; return;}
 int n=iplist->IndexOf(ip);
 if (n!=-1) {delete pd; return;}

 EnableListenPorts();
 SetTimer();

 if (pd->li) pd->li->SubItems->Strings[3]="..."; 

 SendDPRequest(ip);
 SendOLRequest(ip);
 ftime(&pd->stime);
 iplist->AddObject(ip,pd);
}



void __fastcall TServerPollThread::PollServer(const AnsiString & ip,const AnsiString & comment, const AnsiString & mod)
{
  if (ip=="ipx:") { PollIPX(); return; }

  PPollData pd=new TPollData;
  pd->comment=comment;
  pd->li=NULL;
  pd->addr=ip;
  dns->Resolve(ip.c_str(),pd);
// RemoveOldIPs();
// if (iplist->IndexOf(ip)==-1)
// {
//  Resume();
// }
}

void __fastcall TServerPollThread::ServerSocketClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
 timeb now;
 ftime(&now);
 char * buf;
 int n;
 int nbytes=Socket->ReceiveLength();

 buf=(char *)malloc(nbytes);
 Socket->ReceiveBuf(buf,nbytes);
 PDPMSG dpm=(PDPMSG)buf;

 if (dpm->size==nbytes && (strncmp(dpm->play,"play",4)==0))
 {
  TCallBackData cbdata;

  cbdata.ip=Socket->RemoteAddress;
  cbdata.active=true;
  cbdata.ping=-1;


  DPSESSIONDESC2 * sd= (DPSESSIONDESC2 *)(buf+sizeof(TDPMSG)+28);

  AnsiString sn=(wchar_t *)((char *)sd+sd->dwSize);

  sd->lpszSessionNameA=sn.c_str();
  copySD2CBD(sd,&cbdata);

  n=iplist->IndexOf(cbdata.ip);
  if (n!=-1)
  {
   PPollData pd=(PPollData)iplist->Objects[n];
   cbdata.ping=(now.time-pd->stime.time)*1000+(now.millitm-pd->stime.millitm);
   cbdata.comment=pd->comment.c_str();
   cbdata.li=pd->li;
   pd->responded=true;
  }

   char * ptr, *pend;

   pend=buf+nbytes;

   /* Find the beginning of players data */

   ptr=(char *)sd+sd->dwSize+2*sn.Length()+2+20;

   while (ptr<pend)
   {
    if ((*ptr!='\x20') || (*(ptr+1)!='\x2') || (*(ptr+2)!='\0')) break;
    ptr+=53;
   }

   n=0;
   while ((ptr<pend) && (n<cbdata.nplayers))
   {
    sn=(wchar_t *)ptr;
    cbdata.players->Add(sn);
    ptr+=sn.Length()*2+2+53;
    if (sn=="") ptr+=2;
    n++;
   }


  UpdateLView(&cbdata);

 }
 free(buf);
}

void TServerPollThread::SetTimer()
{
 timer->Enabled=false;
 timer->Enabled=true;
 counter=0;
}


void __fastcall TServerPollThread::TimerTimer(TObject *Sender)
{
 counter++;
 PanMessage(("Waiting "+IntToStr(counter)+"...").c_str());
 if (counter<10) return;

 RemoveOldIPs();
 DisableListenPorts();
 timer->Enabled=false;
 PanMessage("");
}


void __fastcall TServerPollThread::OLUDP_DataReceived(TComponent *Sender,
      int NumberBytes, AnsiString FromIP)
{
 timeb now;
 ftime(&now);

 char * buf;
 int nbytes=NumberBytes;
 int n;

 buf=(char *)malloc(nbytes);

 OLUDP->ReadBuffer(buf, nbytes, nbytes);
// "XESD\x06LOCATION\n   Dry Gulch\nDeath Match\nPlayers: (1 / 8)\nStarted By:\n   Chip\nGame Time: 00:00:11"

 if (strncmp(buf,"XESD",4)==0)
 {
  TCallBackData cbdata;
  cbdata.ip=FromIP;
  cbdata.active=true;
  cbdata.ping=-1;
  GetOLData(&cbdata,buf);
  n=iplist->IndexOf(cbdata.ip);
  if (n!=-1)
  {
    PPollData pd=(PPollData)iplist->Objects[n];
    cbdata.ping=(now.time-pd->stime.time)*1000+(now.millitm-pd->stime.millitm);
    cbdata.comment=pd->comment.c_str();
    cbdata.li=pd->li;
    pd->responded=true;
  }

  UpdateLView(&cbdata);

 }
 free(buf);
}

void __fastcall TServerPollThread::PollIPX()
{
 int n=iplist->IndexOf("ipx:");
 PPollData pd;

 if (n==-1)
 {
  pd=new TPollData;
  PPollData pd=new TPollData;
  pd->comment="Local Game";
  pd->li=NULL;
  iplist->AddObject("ipx:",pd);
 } else
 {
  pd=(PPollData)iplist->Objects[n];
 }

 squeue->AddObject("ipx:",pd);
 Resume();
}

void HostGame(int game)
{
 LPDIRECTPLAYLOBBY2A idpl=NULL;
 void * adr;
 int hr;
 ULONG mf,id,size;
 DPSESSIONDESC2 ds;
 DPNAME pn;
 DPLCONNECTION con;

 memset(&ds,0,sizeof(ds));
 ds.dwSize=sizeof(ds);
 ds.guidInstance=GUID_NULL;
 ds.lpszSessionNameA="Aye! Karamba!";
 ds.dwMaxPlayers=6;

  switch (game)
 {
  case game_JK: ds.guidApplication=GUID_JK; break;
  case game_MOTS: ds.guidApplication=GUID_MOTS; break;
 default: return;
 }


 idpl=CreateDPLobby();
// adr=CreateDPAddress(idpl,"1.1.1.1",&size);


 memset(&pn,0,sizeof(pn));
 pn.dwSize=sizeof(pn);
 pn.dwFlags=0;
 pn.lpszShortNameA="Bort";
 pn.lpszLongNameA="Bort";

 adr=NULL;

 memset(&con,0,sizeof(con));
 con.dwSize=sizeof(con);
 con.dwFlags=DPLCONNECTION_CREATESESSION;
 con.lpSessionDesc=&ds;
 con.lpPlayerName=&pn;
 con.guidSP=DPSPGUID_TCPIP;
 con.lpAddress=adr;
 con.dwAddressSize=0;

 hr=idpl->RunApplication(0,&id,&con,0);
 CheckHR(hr);

 if (hr) Application->MessageBox("Couldn't start the game. Make sure it's installed properly","Error",MB_OK);
 else
 {
  do
  {
   hr=idpl->ReceiveLobbyMessage(0,id,&mf,NULL,&size);
   Sleep(5);
  }
  while (hr==DPERR_NOMESSAGES);
 }
 idpl->Release();
}


void SetOLNetDriver(const AnsiString& oldir,bool winsock,const AnsiString& ip)
{
 TRegistry * reg;

 if (winsock && FileExists(oldir+"LECWSOCK.DLL"))
 {
  reg=new TRegistry;
  reg->RootKey=HKEY_LOCAL_MACHINE;
  if (reg->OpenKey("SOFTWARE\\LucasArts Entertainment Company\\Outlaws\\v1.0",false))
   reg->WriteString("Network Driver",oldir+"LECWSOCK.DLL");

  reg->CloseKey();
  if (reg->OpenKey("SOFTWARE\\LucasArts Entertainment Company\\Outlaws\\Users\\Default User\\WinSock\\IP",false))
  {
   reg->WriteString("Server Address",ip);
  }

  reg->Free();
 }

 if (!winsock && FileExists(oldir+"Lecdp3.dll")) // DirectPlay
 {
  reg=new TRegistry;
  reg->RootKey=HKEY_LOCAL_MACHINE;
  if (reg->OpenKey("SOFTWARE\\LucasArts Entertainment Company\\Outlaws\\v1.0",false))
   reg->WriteString("Network Driver",oldir+"Lecdp3.dll");
  reg->CloseKey();

  if (reg->OpenKey("SOFTWARE\\LucasArts Entertainment Company\\Outlaws\\v1.0\\DirectPlay 3",false))
  {
   if (ip=="ipx:") reg->WriteString("Provider","IPX (LAN)");
   else reg->WriteString("Provider","TCP/IP (Internet/LAN)");
  }
  reg->Free();
 }
}



void JoinOLGame(char * ip,TActiveServerData * sdata)
{
 AnsiString oldir=GetOutlawsDir();
 if (oldir=="") { ShowMessage("Outlaws doesn't seem to be installed"); return; }
 SetOLNetDriver(oldir,sdata->guidInst==GUID_NULL,ip);
 STARTUPINFO si;
 PROCESS_INFORMATION pi;

 memset(&si,0,sizeof(si));
 si.cb=sizeof(si);


 int r=CreateProcess((oldir+"olwin.exe").c_str(), // app name
               "", // cmd line
               NULL, // process attributes
               NULL, // thread attributes
               false, // inherits handles
               0, // process flags
               NULL, // environment
               oldir.c_str(), // Directory
               &si, // starupinfo
               &pi); // process info

 if (!r) ShowMessage(SysErrorMessage(GetLastError()));

}


void HostOLGame(bool winsock)
{
 AnsiString oldir=GetOutlawsDir();
 if (oldir=="") { ShowMessage("Outlaws doesn't seem to be installed"); return; }
 SetOLNetDriver(oldir,winsock,"");

 STARTUPINFO si;
 PROCESS_INFORMATION pi;

 memset(&si,0,sizeof(si));
 si.cb=sizeof(si);


 int r=CreateProcess((oldir+"olwin.exe").c_str(), // app name
               "", // cmd line
               NULL, // process attributes
               NULL, // thread attributes
               false, // inherits handles
               0, // process flags
               NULL, // environment
               oldir.c_str(), // Directory
               &si, // starupinfo
               &pi); // process info

 if (!r) ShowMessage(SysErrorMessage(GetLastError()));

}



void JoinGame(char * ip,TActiveServerData * sdata)
{
 LPDIRECTPLAYLOBBY2A idpl=NULL;
 void * adr;
 int hr;
 ULONG size,id,mf;
 DPSESSIONDESC2 ds;
 DPNAME pn;
 DPLCONNECTION con;

 switch (WhatGame(&sdata->guidGame))
 {
  case game_JK: break;
  case game_MOTS: break;
  case game_OL: JoinOLGame(ip,sdata); return;
 }

 idpl=CreateDPLobby();
 adr=CreateDPAddress(idpl,ip,&size);

 memset(&ds,0,sizeof(ds));
 ds.dwSize=sizeof(ds);
 ds.guidInstance=sdata->guidInst;
 ds.guidApplication=sdata->guidGame;
 ds.lpszSessionNameA="Aye, Karamba!";
 ds.dwMaxPlayers=6;

 memset(&pn,0,sizeof(pn));
 pn.dwSize=sizeof(pn);
 pn.dwFlags=0;
 pn.lpszShortNameA="Bort";
 pn.lpszLongNameA="Bort";

 memset(&con,0,sizeof(con));
 con.dwSize=sizeof(con);
 con.dwFlags=DPLCONNECTION_JOINSESSION;
 con.lpSessionDesc=&ds;
 con.lpPlayerName=&pn;

 if (strcmp(ip,"ipx:")==0) con.guidSP=DPSPGUID_IPX;
 else con.guidSP=DPSPGUID_TCPIP;

 con.lpAddress=adr;
 con.dwAddressSize=size;

 hr=idpl->RunApplication(0,&id,&con,0);
 CheckHR(hr);

 if (hr) Application->MessageBox("Couldn't start the game. Make sure it's installed properly","Error",MB_OK);
 else
 {
  do
  hr=idpl->ReceiveLobbyMessage(0,id,&mf,NULL,&size);
  while (hr==DPERR_NOMESSAGES);
 }
 idpl->Release();

}

AnsiString GetGameName(const _GUID * guid)
{
 AnsiString s="Unknown";
 if (*guid==GUID_JK) s="Jedi Knight";
 if (*guid==GUID_MOTS) s="MOTS";
 if (*guid==GUID_OL) s="Outlaws";
 return s;
}


int WhatGame(const _GUID * guid)
{
 if (*guid==GUID_JK) return game_JK;
 if (*guid==GUID_MOTS) return game_MOTS;
 if (*guid==GUID_OL) return game_OL;
 return game_UN;
}

