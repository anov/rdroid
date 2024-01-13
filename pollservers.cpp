//---------------------------------------------------------------------------
#include <vcl.h>
#include "dx6\dplay.h"
#include "dx6\dplobby.h"
#pragma hdrstop

#include "pollservers.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)



int PollServer(char * ip)
{
 LPDIRECTPLAY3A idp=NULL;
 LPDIRECTPLAYLOBBY2A idpl=NULL;

 LPDIRECTPLAY idp1=NULL;
 LPDIRECTPLAYLOBBY idpl1=NULL;

 int hr;
 ULONG size;
 void * adr=NULL;
 DPSESSIONDESC2 ds;
 DPCOMPOUNDADDRESSELEMENT    ad[2];


 hr=DirectPlayCreate((_GUID *)&GUID_NULL,&idp1,NULL);
 hr=idp1->QueryInterface(IID_IDirectPlay3A,(LPVOID *)&idp);
 idp1->Release();

 hr=DirectPlayLobbyCreate(NULL,&idpl1,NULL,NULL,0);

 hr=idpl1->QueryInterface(IID_IDirectPlayLobby2A,(LPVOID *)&idpl);
 idpl1->Release();

// hr = CoCreateInstance(CLSID_DirectPlay, NULL, CLSCTX_INPROC_SERVER,
//						  IID_IDirectPlay3A, (LPVOID *) &idp);
// hr = CoCreateInstance(CLSID_DirectPlayLobby, NULL, CLSCTX_INPROC_SERVER,
//						  IID_IDirectPlayLobby2A, (LPVOID *) &idpl);

 ad[0].guidDataType=DPAID_ServiceProvider;
 ad[0].dwDataSize=sizeof(TGUID);
 ad[0].lpData=(void *)&DPSPGUID_TCPIP;

 ad[1].guidDataType=DPAID_INet;
 ad[1].dwDataSize=strlen(ip)+1;
 ad[1].lpData=ip;

 size=0;
 hr=idpl->CreateCompoundAddress(&ad[0],2,adr,&size);

 adr=malloc(size);

 hr=idpl->CreateCompoundAddress(&ad[0],2,adr,&size);

 hr=idp->InitializeConnection(adr,0);

 memset(&ds,0,sizeof(ds));
 ds.dwSize=sizeof(ds);
 ds.guidApplication=GUID_NULL;

 hr=idp->EnumSessions(&ds,0,EnumSessions,proc,DPENUMSESSIONS_AVAILABLE);


 return 1;
}

