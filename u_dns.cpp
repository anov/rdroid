//---------------------------------------------------------------------------
#include <vcl.h>
#include <winsock.h>
#pragma hdrstop

#include "u_dns.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#define WM_ADDRRESOLVED WM_USER+3

long _stdcall DNSWndProc(HWND wnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
 TDNS * dns;
 if (msg==WM_ADDRRESOLVED)
 {
  dns=(TDNS *)GetWindowLong(wnd, 0);
  if (dns) dns->ResolvedProc(wparam,lparam);
  return 0;
 }
 else return DefWindowProc(wnd, msg, wparam, lparam);
}

void TDNS::ResolvedProc(WPARAM wparam,LPARAM lparam)
{
 if ((HANDLE)wparam!=hreq) return;
 void * data=(void *)reqs->Objects[0];
 if (HIWORD(lparam)) (OnResolve)("",data);
 else
 {
  hostent * he=(hostent *)dnsbuf;
  in_addr **ia=(in_addr **)he->h_addr_list;
  OnResolve(inet_ntoa(**ia),data);
 }
 reqs->Delete(0);
 if (reqs->Count==0) hreq=0; else
 {
  hreq=WSAAsyncGetHostByName(wnd,
                            WM_ADDRRESOLVED,
                            reqs->Strings[0].c_str(),
                            dnsbuf,
                            sizeof(dnsbuf));
 }

}

__fastcall TDNS::TDNS()
{
 WNDCLASSEXA dc;
 dc.cbSize=sizeof(dc);
 dc.style=0;
 dc.lpfnWndProc=DNSWndProc;
 dc.cbClsExtra=0;
 dc.cbWndExtra=sizeof(void *);
 dc.hInstance=0;
 dc.hIcon=0;
 dc.hCursor=0;
 dc.hbrBackground=0;
 dc.lpszMenuName=NULL;
 dc.lpszClassName="RDroidDNSClass";
 dc.hIconSm=0;
 RegisterClassEx(&dc);

 wnd=CreateWindowEx(WS_EX_TOOLWINDOW,
                    dc.lpszClassName,
                    "",       // Window name   }
                    WS_POPUP,  // Window Style  }
                    0, 0,      // X, Y          }
                    0, 0,      // Width, Height }
                    0,         // hWndParent    }
                    0,         // hMenu         }
                    HInstance, // hInstance     }
                    NULL);      // CreateParam   }

 SetWindowLong(wnd, 0, (long)this);
 hreq=0;
 reqs=new TStringList;
}

__fastcall TDNS::~TDNS()
{
 DestroyWindow(wnd);
}


void TDNS::StarResolve(char * addr,void * data)
{
 reqs->AddObject(addr,(TObject *)data);
 if (!hreq) hreq=WSAAsyncGetHostByName(wnd,
                            WM_ADDRRESOLVED,
                            addr,
                            dnsbuf,
                            sizeof(dnsbuf));
}

void TDNS::Resolve(char * addr,void * data)
{
  in_addr IPAddr;
  IPAddr.s_addr = inet_addr(addr);
  if (IPAddr.s_addr!= INADDR_NONE)
  {
   OnResolve(inet_ntoa(IPAddr),data);
  } else StarResolve(addr,data);
}
