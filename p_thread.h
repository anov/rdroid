//---------------------------------------------------------------------------
#ifndef p_threadH
#define p_threadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "dx6\dplay.h"
#include <NMUDP.hpp>
#include <ScktComp.hpp>
#include "utils.h"
#include "u_dns.h"
#include <sys\timeb.h>
//---------------------------------------------------------------------------

#pragma pack(push,1)

typedef struct dpmsg
{
 short size;
 long long1; // 0x2FAB0
 short port; // 0xF04 1304 - port, inverted byte order
 long longs[2]; // 0,0
 short short2; // 0x8019
 short short3; // ? 0x8a00
 char play[4]; // "play"
 short type; // 2 - EnumSess request, 1 - EnumSess response?
 short short4; // 7 or 8?
} TDPMSG, *PDPMSG;

#pragma pack(pop)


typedef class TCallBackData {
 public:
 bool active;
 AnsiString ip;
 int ping;
 LPDIRECTPLAY3A idp;
 char * comment;
 TListItem * li;
 int nplayers;
 int maxplayers;
 AnsiString gname;
 AnsiString level;
 TStringList * players;
 _GUID guidInst;
 _GUID guidGame;
 int user1,user2,user3;
 TThread * thread;
 TCallBackData() {memset(this,0,sizeof(*this)); players=new TStringList;};
 ~TCallBackData() {players->Free();};
} TCallBackData, *PCallBackData;


typedef class TPollData:public TObject
{
 public:
 AnsiString addr;
 TListItem * li;
 AnsiString comment;
 bool responded;
 timeb stime;
 TPollData() {comment="";};
} *PPollData;


class TServerPollThread : public TThread
{
private:
   bool PortsActive;
   TServerSocket * ServSock;
   TNMUDP * DPUDP;
   TNMUDP * OLUDP;
   TTimer * timer;

   TDNS * dns;

   TListView * LView;
//   TProgressBar * PBar;

   TStringList * squeue;
   TStringList * iplist;

   int CurEntry;

   TCallBackData cbdata;

   // Synchronized function parameters
   TCallBackData * pdata;

   void __fastcall OLUDP_DataReceived(TComponent *Sender,
      int NumberBytes, AnsiString FromIP);
   void __fastcall ServerSocketClientRead(TObject *Sender,
      TCustomWinSocket *Socket);
   void __fastcall TimerTimer(TObject *Sender);

protected:
   int counter;
   void __fastcall Execute();
   void __fastcall UpdateLViewSync();
   void __fastcall UpdateLView(TCallBackData * pdata);
   void __fastcall RemoveCurIP();
   void __fastcall UpdatePBar();
   void __fastcall ShowCurServ();
   void ResolveHandler(const AnsiString& ip,void * data);
   void SendDPRequest(const AnsiString& ip);
   void SendOLRequest(const AnsiString& ip);
   void EnableListenPorts();
   void SetTimer();
public:
   __fastcall TServerPollThread(TListView * lv, TServerSocket * ssock,TNMUDP * dpudp,TNMUDP * oludp,TTimer * Timer);
   void CallBack(TCallBackData * pcbdata);
   void __fastcall PollServer(const AnsiString & ip,const AnsiString & comment, const AnsiString & mod);
   void __fastcall UpdateServer(const AnsiString & ip, TListItem * li);

   void __fastcall PollIPX();
   void __fastcall CancelPolling();
   void __fastcall ClearPBar();
   int FindActiveServer(char * ip, const _GUID guid);
   void ClearPollQueue();
   void RemoveOldIPs();
   void DisableListenPorts();
};
//---------------------------------------------------------------------------

void HostOLGame(bool winsock);

void HostGame(int game);
void JoinGame(char * ip,TActiveServerData * sdata);
AnsiString GetGameName(const _GUID * guid);
int WhatGame(const _GUID * guid);

#endif

