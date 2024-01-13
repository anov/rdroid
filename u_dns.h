//---------------------------------------------------------------------------
#ifndef u_dnsH
#define u_dnsH
//---------------------------------------------------------------------------

typedef void (__closure *TResolveEvent)(const AnsiString & ip,void * data);

class TDNS
{
 HWND wnd;
 HANDLE hreq;
 TStringList * reqs;
 char dnsbuf[MAXGETHOSTSTRUCT];
 void StarResolve(char * addr,void * data);
public:
 TResolveEvent OnResolve;
 __fastcall TDNS();
 __fastcall ~TDNS();
 void Resolve(char * addr,void * data);
 void ResolvedProc(WPARAM wparam,LPARAM lparam);
};


#endif
