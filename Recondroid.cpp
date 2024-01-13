//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("recondroid.res");
USEFORM("mainform.cpp", JSForm);
USELIB("dxguid.lib");
USEUNIT("utils.cpp");
USEUNIT("p_thread.cpp");
USEFORM("u_host.cpp", HostForm);
USEFORM("u_sprop.cpp", ServPropForm);
USEFORM("u_about.cpp", AboutForm);
USEFORM("u_pollip.cpp", PollIP);
USEFORM("u_aprop.cpp", APropForm);
USEFORM("u_http.cpp", HttpForm);
USEUNIT("u_dns.cpp");
USEFORM("u_askdl.cpp", AskDLForm);
USEFORM("U_unzip.cpp", UnZipForm);
USEUNIT("Unzip.pas");
USEUNIT("Ziptypes.pas");
USEFORM("u_options.cpp", Options);
USEFORM("u_ftp.cpp", FtpForm);
//---------------------------------------------------------------------------
#include "mainform.h"

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
   if (ParamCount())
   {
    HWND wnd=FindWindow("TJSForm",NULL);
    ATOM url;
    int n;
    if (wnd)
    {
     url=GlobalAddAtom(ParamStr(1).c_str());
     n=SendMessage(wnd,WM_TAKEURL,url,0);
     GlobalDeleteAtom(url);

     if (n==URLTAKEN) exit(0);
    }
   }

    try
    {
        Application->Initialize();
        Application->CreateForm(__classid(TJSForm), &JSForm);
        Application->CreateForm(__classid(THostForm), &HostForm);
        Application->CreateForm(__classid(TServPropForm), &ServPropForm);
        Application->CreateForm(__classid(TAboutForm), &AboutForm);
        Application->CreateForm(__classid(TPollIP), &PollIP);
        Application->CreateForm(__classid(TAPropForm), &APropForm);
        Application->CreateForm(__classid(THttpForm), &HttpForm);
        Application->CreateForm(__classid(TAskDLForm), &AskDLForm);
        Application->CreateForm(__classid(TUnZipForm), &UnZipForm);
        Application->CreateForm(__classid(TOptions), &Options);
        Application->CreateForm(__classid(TFtpForm), &FtpForm);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
