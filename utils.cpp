//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <winsock.h>
#include <registry.hpp>

// #include <SysUtils.hpp>


#pragma hdrstop

#include "utils.h"
#include "u_http.h"
#include "mainform.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


char * ScanForSpace(char *str)
{
 char *tmp;
 tmp=str;
 while (!(*tmp=='\0'))
 {
  if ((*tmp==' ') || (*tmp==(char)9)) break;
  tmp++;
 }
 return tmp;
}

char * ScanForNonSpace(char *str)
{
 char *tmp;
 tmp=str;
 while ((*tmp==' ') || (*tmp==(char)9)) tmp++;
 return tmp;
}


void LoadGroupFile(char * fname, TTreeNodes * nodes)
{
 FILE *fp;
 int i;
 char tmp[1024];
 char *p,*w,*v,c;
 TTreeNode * node;
 TStack nstack;
 TNodeData * nd;

 for (i=0;i<nodes->Count;i++)
 {
   delete nodes->Item[i]->Data;
 }

 nodes->Clear();

 node=NULL;


 fp=fopen(fname,"rt");
 if (!fp)
  {
   AnsiString s="Couldn't open group file - ";
   s=s+fname;
   ShowMessage(s);
   return;
  }

 while (!feof(fp))
 {
  fgets(tmp,sizeof(tmp),fp);
  p=StrScan(tmp,'\n');
  if (p) *p='\0';

  p=StrScan(tmp,'#');
  if (p) *p='\0';

  p=ScanForNonSpace(tmp);
  if (!*p) continue;
  w=p;
  p=ScanForSpace(w);
  c=*p;
  *p='\0'; // w is the word

  if (strcmpi(w,"GROUP")==0)
  {
   *p=c;
   w=ScanForNonSpace(p);
   nstack.Push(node); node=nodes->AddChild(node,w);
   nd=new TNodeData;
   node->Data=nd;

   continue;
  }

  if (strcmpi(w,"ENDGROUP")==0)
  {
   node=(TTreeNode*)nstack.Pop();
   continue;
  }

  if (!node) continue;

  *p=c;
  p=StrScan(tmp,'=');
  if (!p) continue;
  c=*p;
  *p='\0';

  v=ScanForNonSpace(p+1);

  // trim trailing spaces/tabs

  p=StrEnd(v);
  while (p>w)
  {
   if ((*(p-1)!=' ') && (*(p-1)!=9)) break;
   *(--p)='\0';
  }

  nd=(TNodeData *)node->Data;

//  if (strcmpi(w,"game")==0)
//  {
//   strncpy(nd->game,v,10);
//  }
//

  if (strcmpi(w,"format")==0)
  {
   strncpy(nd->format,v,10);
  }

  if (strcmpi(w,"url")==0)
  {
   nd->url=v;
  }


 }


};


char * LoadURLData(TNodeData * nd)
{
 char tmp[10]="";
 char *buf;
 int size;
 FILE *fp;

 strncpy(tmp,nd->url.c_str(),5);

 try
 {

  if (strcmpi(tmp,"http:")==0)
  {
   buf=HttpForm->GetURLData(nd->url);
   return buf;
  }
  else
  {
   fp=fopen(nd->url.c_str(),"rb");
   if (!fp) return NULL;
   fseek(fp,0,SEEK_END);
   size=ftell(fp);
   fseek(fp,0,SEEK_SET);
   buf=(char *)malloc(size+1);
   fread(buf,size,1,fp);
   *(buf+size)='\0';
   fclose(fp);
    return buf;
  };

 } catch (Exception &exception) {
  AnsiString s;
  s="Error retrieving ";
  s=s+nd->url;
  s=s+": "+exception.Message;
  Application->MessageBox(s.c_str(),"Error",MB_OK);
  return NULL;
 }
};

void ParseJKNet(char * s,TStringList * sl)
{
 char *p, *pe, *tp, *cs, *ce, *p1, *pp, c;
 char tmp[1024];
 char ip[256];
 TServerData * sd;
 int ncol;
 AnsiString levurl;

 p=StrPos(s,"<!-- GAME LISTINGS -->");
 if (!p) { PanMessage("Not a JK.Net format - no <!-- GAME LISTINGS --> marker"); return; }
 p=StrPos(p,"<TABLE");
 if (!p) { PanMessage("A table doesn't follow <!-- GAME LISTINGS --> marker"); return; }

 p=StrPos(p,"</TR>");


while (true) // Table
{
 if (!p) break;
 if (*p=='\0') break;
 p++;

 levurl="";

 p=StrScan(p,'<');
 if (!p) break;
 if (strncmpi(p,"</TABLE>",8)==0) break;

 if (strncmpi(p,"<TR",3)==0) // row found
 {
  sd=new TServerData;
  pe=StrPos(p,"</TR>");
  if (!pe) pe=StrEnd(p);


  ncol=0;
  while (true) // while row
  {
   p++;
   cs=StrPos(p,"<TD");
   if ((!cs) || (cs>=pe)) break;
   ce=StrPos(cs,"</TD>");
   if ((!ce) || (ce>pe)) ce=pe;

   tp=tmp;

   while (cs<ce) // while column
   {
    if (*cs=='<')
    {
     if ((ncol==1) && (strnicmp(cs,"<A HREF=\x22http://",15)==0))
     // this is "level" column and URL to level is found
     {
      pp=StrScan(cs+9,'"');
      if (pp) { c=*pp; *pp='\0'; levurl=cs+9; *pp=c; }
     }

     p1=StrScan(cs,'>');
     if ((!p1) || (p1>ce))  break;
     cs=p1+1;
    }
    else if (*cs=='\n') cs++;
    else *tp++=*cs++;

   } // while column

   *tp='\0';
   p=ce+1;

   // tmp contains column content, ncol - its number

   switch (ncol) {
    case 0: {
             tp=tmp;
             while ((*tp!='\0') && ((*tp=='\t') || (*tp==' '))) tp++;

             p1=StrEnd(tp)-1;
             while ((p1>tp) && ((*p1=='\t') || (*p1==' ')))
             *p1--='\0';

             strncpy(ip,tp,sizeof(ip));
            }
            break;
    case 7: { sd->comment=tmp; if (levurl!="") sd->comment=sd->comment+" "+levurl; } break;
   }



   ncol++;

  } // while row

  sd->ip=ip;
  sl->AddObject(ip,(TObject *)sd);

  p=pe+1;

 }


}

}

void ParseText(char * s,TStringList * sl)
{
 char *p,*ps,*pe,*p1,*p2,c;
 TServerData * sd;

 p=s;

 while (true)
 {
  p=StrScan(p,'\r');
  if (!p) break;
  *p=' ';
 }

 p=s;

 while (true)
 {
  pe=StrScan(p,'\n');
  if (!pe) pe=StrEnd(p);
  ps=p;
  c=*pe;
  *pe='\0';
  p1=StrScan(ps,'|');
  if (!p1) p1=pe; else {*p1='\0'; p1++;}

  p2=StrScan(ps,'|');
  if (p2) *p2='\0';
  // ps - first, p1 - second}

  if (Trim(ps)!="")
  {

  sd=new TServerData;

  sd->ip=Trim(ps);
  sd->comment=Trim(p1);
  sl->AddObject(sd->ip,(TObject *)sd);
 }

  if (c=='\0') break;
  p=pe+1;

 }
}

void ParseDJ(char * s,TStringList * sl)
{
 char *p,*ps,*pe,*p1,c;
 TServerData * sd;
 int n;


 p=s;

 while (true)
 {
  p=StrScan(p,'\r');
  if (!p) break;
  *p=' ';
 }

 p=s;

 while (true)
 {
  pe=StrScan(p,'\n');
  if (!pe) pe=StrEnd(p);
  ps=p;
  c=*pe;
  *pe='\0';

  // Need 4th and 6th columns

  n=0;

  while ((ps) && (*ps!='\0'))
  {
   p1=StrScan(ps,'|');
   if (!p1) p1=pe; else {*p1='\0'; p1++;}

   switch (n)
   {
    case 3: {
             sd=new TServerData;
             sd->comment=Trim(ps);
            };break;
    case 5: if (Trim(ps)!="")
            {
             sd->ip=Trim(ps);
             sl->AddObject(sd->ip,(TObject *)sd);
            };break;
   }
   ps=p1;
   n++;
  }

  if (c=='\0') break;
  p=pe+1;

 }
}


void RipIPs(char * s,TStringList * sl)
{
 char *p,*p1,*p2,*p3,*ps,*pe;
 char ip[256];
 char tmp[4];
 TServerData * sd;
 int i;

 p=s;
 while (true)
 {
  p1=StrScan(p,'.');
  if (!p1) break;

  p=p1+1;

  ps=p1-1;
  if (ps<s) continue;

  if ((*ps<'0') || (*ps>'9')) continue;

  if ((ps>s) && (*(ps-1)>='0') && (*(ps-1)<='9')) ps--;
  if ((ps>s) && (*(ps-1)>='0') && (*(ps-1)<='9')) ps--;

  p2=p1+1; // xxx. found

  for (i=0;i<3;i++)
  {
   p2++;
   if (*p2=='.') break;
  }

  if (*p2!='.') continue;

  p3=p2+1;
  for (i=0;i<3;i++)
  {
   p3++;
   if (*p3=='.') break;
  }

  if (*p3!='.') continue;

  pe=p3+1;

  if ((*pe<'0') || (*pe>'9')) continue;

  if ((*(pe+1)>='0') && (*(pe+1)<='9')) pe++;
  if ((*(pe+1)>='0') && (*(pe+1)<='9')) pe++;

  StrLCopy(tmp,ps,p1-ps);
  if (StrToIntDef(tmp,256)>255) continue;

  StrLCopy(tmp,p1+1,p2-p1-1);
  if (StrToIntDef(tmp,256)>255) continue;

  StrLCopy(tmp,p2+1,p3-p2-1);
  if (StrToIntDef(tmp,256)>255) continue;

  StrLCopy(tmp,p3+1,pe-p3);
  if (StrToIntDef(tmp,256)>255) continue;

  StrLCopy(ip,ps,pe-ps+1);

  if (sl->IndexOf(ip)!=-1) continue;

  sd=new TServerData;

  sd->ip=ip;
  sl->AddObject(ip,(TObject *)sd);
  p=pe+1;
 }

}


void UpdateNodeData(TNodeData * nd)
{
 TStringList * sl;
 char * s;
 TServerData * sd;


 if (nd->url=="") return;

// if (nd->servlist)
//  for (i=0;i<nd->servlist->Count;i++)
//   delete nd->servlist->Objects[i];

 sl=nd->servlist;

 sl->Clear();

 if (nd->url=="ipx:")
 {
  sd=new TServerData;
  sd->ip="ipx:";
  sd->comment="Local IPX Games";
  sl->AddObject("ipx:",(TObject *)sd);
  return;
 }


 s=LoadURLData(nd);

 if (s)
 {
  if (strcmpi(nd->format,"jknet")==0) ParseJKNet(s,sl); else
  if (strcmpi(nd->format,"text")==0) ParseText(s,sl); else
  if (strcmpi(nd->format,"ripips")==0) RipIPs(s,sl); else
  if (strcmpi(nd->format,"darkjedi")==0) ParseDJ(s,sl); else
  if (strcmpi(nd->format,"")==0) RipIPs(s,sl); else
  {
   AnsiString st;
   st="Unknown list format '";
   st=st+nd->format+"'";
   PanMessage(st.c_str());
  }
 free((void *)s);
}

};

void PanMessage(char * msg)
{
 JSForm->PMsg->Caption=msg;
}

bool SaveServerList(const AnsiString & fname,TStringList * sl)
{
 FILE *fp;
 int i;
 TServerData * sd;
 AnsiString s;

 mkdir(ExtractFilePath(fname).c_str());

 fp=fopen(fname.c_str(),"wt");
 if (!fp)
 {
  PanMessage(("Couldn't create "+fname).c_str());
  return false;
 }

 for (i=0;i<sl->Count;i++)
 {
  sd=(TServerData *)sl->Objects[i];
  fprintf(fp,"%s | %s\n",sl->Strings[i].c_str(),sd->comment.c_str());
 }

 return (fclose(fp)==0);
}

AnsiString GetMyIP()
{
//   type
//   PInAddr TaPInAddr = array [0..10] of PInAddr;
//    PaPInAddr = ^TaPInAddr;

    PHostEnt phe;
    PInAddr *pptr;
    char Buffer[64];
    int i;
    WSADATA GInitData;
    AnsiString ip;

    WSAStartup(0x101, &GInitData);
    gethostname(Buffer, sizeof(Buffer));
    phe = gethostbyname(Buffer);
    if (!phe) return ip;
    pptr = (PInAddr *)phe->h_addr_list;
    i = 0;
    while (*pptr)
    {
      ip=inet_ntoa(**pptr);
      pptr++;
      i++;
    }
    WSACleanup();
 return ip;
}


bool Confirm(char * msg)
{
 return Application->MessageBox(msg,"Confirm",MB_YESNO)==IDOK;
}

AnsiString GetOutlawsDir()
{
 TRegistry * reg;
 AnsiString dir;

 reg=new TRegistry;
 reg->RootKey=HKEY_LOCAL_MACHINE;
 if (reg->OpenKey("SOFTWARE\\Microsoft\\DirectPlay\\Applications\\Outlaws",false))
  dir=reg->ReadString("Path");
 reg->CloseKey();

 if (dir=="")
 {
  if (reg->OpenKey("SOFTWARE\\LucasArts Entertainment Company\\Outlaws\\v1.0",false))
   dir=ExtractFilePath(reg->ReadString("Network Driver"));
 }
 return PathAddSlash(dir);
}


AnsiString GetJKDir()
{
 AnsiString dir;
 TRegistry * reg;

 reg=new TRegistry;

 reg->RootKey=HKEY_LOCAL_MACHINE;

 if (reg->OpenKey("SOFTWARE\\LucasArts Entertainment Company\\JediKnight\\v1.0",false))
 {
  dir=reg->ReadString("Install Path");
 }
 reg->Free();
 return PathAddSlash(dir);
}

AnsiString GetMOTSDir()
{
 AnsiString dir;
 TRegistry * reg;

 reg=new TRegistry;
 reg->RootKey=HKEY_LOCAL_MACHINE;

 if (reg->OpenKey("SOFTWARE\\LucasArts Entertainment Company LLC\\Mysteries of the Sith\\v1.0",false))
 {
  dir=reg->ReadString("Install Path");
 }
 reg->Free();
 return PathAddSlash(dir);
}

AnsiString GetJKAddOnDir()
{
 AnsiString dir;
 TRegistry * reg;

 reg=new TRegistry;
 reg->RootKey=HKEY_CURRENT_USER;

 if (reg->OpenKey("Software\\Code Alliance\\ReconDroid",false))
 {
  dir=reg->ReadString("JKAddOnDir");
 }

 reg->Free();

 if (dir=="") dir=GetJKDir()+"Episode\\";
 return PathAddSlash(dir);
}

AnsiString GetMOTSAddOnDir()
{
 AnsiString dir;
 TRegistry * reg;

 reg=new TRegistry;
 reg->RootKey=HKEY_CURRENT_USER;

 if (reg->OpenKey("Software\\Code Alliance\\ReconDroid",false))
 {
  dir=reg->ReadString("MOTSAddOnDir");
 }

 reg->Free();
 if (dir=="") dir=GetMOTSDir()+"Episode\\";
 return PathAddSlash(dir);
}

void SaveAddOnDirs(const AnsiString& jkdir,const AnsiString& motsdir)
{
 TRegistry * reg;
 reg=new TRegistry;
 reg->RootKey=HKEY_CURRENT_USER;
 reg->OpenKey("Software\\Code Alliance\\ReconDroid",true);
 reg->WriteString("JKAddOnDir",jkdir);
 reg->WriteString("MOTSAddOnDir",motsdir);
 reg->CloseKey();
 reg->Free();
}


typedef struct TGOB2Header
{
 char Magic[4]; // ="GOB\x20"
 long long1,long2; //  0x14, 0xC}
 long NEntries;
}TGOB2Header;

typedef struct TGOB2Entry
{
 long pos,size;
 char name[128];
}TGOB2Entry;




bool IsFileInGOB(const AnsiString& gob,const AnsiString& file)
{
 FILE *fp;
 TGOB2Header gh;
 TGOB2Entry ge;
 int i;

 fp=fopen(gob.c_str(),"rb");
 if (!fp) return false;
 fread(&gh,sizeof(gh),1,fp);
 if (strncmp(gh.Magic,"GOB ",4)!=0) { fclose(fp); return false; }

 for (i=0;i<gh.NEntries;i++)
 {
  fread(&ge,sizeof(ge),1,fp);
  if (file.AnsiCompareIC(ge.name)==0) { fclose(fp); return true; }
 }
 fclose(fp);
 return false;
}



int LocateJKLevel(const AnsiString& level)
{
 AnsiString gob,jkl;

 int p;
 p=level.Pos(':');
 if (!p) return lv_NotFound;
 gob=level;
 jkl=gob.SubString(p+1,gob.Length());
 gob.SetLength(p-1);

 if (CompareText(gob,"JK1")==0) return lv_GameDir;
 if (CompareText(gob,"JK1CTF")==0) return lv_GameDir;
 if (CompareText(gob,"JK1MP")==0) return lv_GameDir;

 gob=gob+".gob";
 if (IsFileInGOB(GetJKDir()+"Episode\\"+gob,"jkl\\"+jkl)) return lv_GameDir;
 if (IsFileInGOB(GetJKAddOnDir()+gob,"jkl\\"+jkl)) return lv_AddOnDir;
 return lv_NotFound;
}


int LocateMOTSLevel(const AnsiString& level)
{
 AnsiString goo,jkl;

 int p;
 p=level.Pos(':');
 if (!p) return lv_NotFound;
 goo=level;
 jkl=goo.SubString(p+1,goo.Length());
 goo.SetLength(p-1);

 if (CompareText(goo,"JKM")==0) return lv_GameDir;
 if (CompareText(goo,"Jkm_kfy")==0) return lv_GameDir;
 if (CompareText(goo,"Jkm_mp")==0) return lv_GameDir;
 if (CompareText(goo,"JKM_SABER")==0) return lv_GameDir;

 goo=goo+".goo";
 if (IsFileInGOB(GetMOTSDir()+"Episode\\"+goo,"jkl\\"+jkl)) return lv_GameDir;
 if (IsFileInGOB(GetMOTSAddOnDir()+goo,"jkl\\"+jkl)) return lv_AddOnDir;
 return lv_NotFound;
}

int LocateOutlawsLevel(const AnsiString& level)
{
 return lv_GameDir;
}


AnsiString GetJKLName(TActiveServerData * asd)
{
 int p=asd->level.Pos(":");
 if (!p) return "";
 return asd->level.SubString(p+1,asd->level.Length());
}

AnsiString GetGOBName(TActiveServerData * asd)
{
 int game=WhatGame(&asd->guidGame);
 if ((game!=game_JK) && (game!=game_MOTS)) return "";
 AnsiString gob=asd->level;
 int p=gob.Pos(":");
 if (p) gob.SetLength(p-1);
 if (game==game_JK) gob=gob+".gob";
 else gob=gob+".goo";
 return gob;
}


void SetCmdAddOnDir(bool mots,bool addonactive)
{
 TRegistry * reg;
 AnsiString cmd;
 char spath[256];
 bool res;

 reg=new TRegistry;
 reg->RootKey=HKEY_LOCAL_MACHINE;
 switch (mots)
 {
  case true:
  {
   res=reg->OpenKey("SOFTWARE\\Microsoft\\DirectPlay\\Applications\\Mysteries of the Sith 1.0",false);
   cmd=GetMOTSAddOnDir();
  };break;
  case false:
  {
   res=reg->OpenKey("SOFTWARE\\Microsoft\\DirectPlay\\Applications\\Jedi Knight 1.0",false);
   cmd=GetJKAddOnDir();
  };break;
 }

 if (!res) { reg->Free(); return; }

 if (!addonactive) cmd="";
 else
 {
  if (GetShortPathName(cmd.c_str(),spath,sizeof(spath))) cmd=spath;
  cmd="-path "+cmd;
 }

 reg->WriteString("CommandLine",cmd);
 reg->CloseKey();
 reg->Free();
}

AnsiString PathAddSlash(const AnsiString& dir)
{
 if (dir!="")
 {
  if (!dir.IsPathDelimiter(dir.Length()))
   return dir+"\\"; else return dir;
 }
 return "";
}


TList * ipboards=NULL;

TList * GetIPBoards() { return ipboards;}

void LoadIPBoards(char * fname)
{
 FILE *fp;
 int i;
 char tmp[1024];
 char *p,*w,*v,c;
 TIPBoard * board;

 if (!ipboards) ipboards=new TList;
 else
 {
  for (i=0;i<ipboards->Count-1;i++)
   ((TIPBoard*)ipboards->Items[i])->Free();
  ipboards->Clear();
 }

 board=NULL;


 fp=fopen(fname,"rt");
 if (!fp)
  {
   AnsiString s="Couldn't open group file - ";
   s=s+fname;
   ShowMessage(s);
   return;
  }

 while (!feof(fp))
 {
  fgets(tmp,sizeof(tmp),fp);
  p=StrScan(tmp,'\n');
  if (p) *p='\0';

  p=StrScan(tmp,'#');
  if (p) *p='\0';

  p=ScanForNonSpace(tmp);
  if (!*p) continue;
  w=p;
  p=ScanForSpace(w);
  c=*p;
  *p='\0'; // w is the word

  if (strcmpi(w,"IPBOARD")==0)
  {
   *p=c;
   w=ScanForNonSpace(p);
   board=new TIPBoard;
   ipboards->Add(board);
   board->name=w;
   continue;
  }

  if (strcmpi(w,"ENDIPBOARD")==0)
  {
   board=NULL;
   continue;
  }

  if (!board) continue;

  *p=c;
  p=StrScan(tmp,'=');
  if (!p) continue;
  c=*p;
  *p='\0';

  v=ScanForNonSpace(p+1);

  // trim trailing spaces/tabs

  p=StrEnd(v);
  while (p>w)
  {
   if ((*(p-1)!=' ') && (*(p-1)!=9)) break;
   *(--p)='\0';
  }

//  if (strcmpi(w,"game")==0)
//  {
//   strncpy(nd->game,v,10);
//  }
//

  if (strcmpi(w,"game")==0)
  {
   if (strcmpi(v,"jk")==0) board->game=game_JK;
   if (strcmpi(v,"mots")==0) board->game=game_MOTS;
   if (strcmpi(v,"Outlaws")==0) board->game=game_OL;
  }

  if (strcmpi(w,"url")==0)
  {
   board->url=v;
  }

  if (strcmpi(w,"method")==0)
  {
   board->is_method_post=strcmpi(v,"get");
  }

  if (strcmpi(w,"params")==0)
  {
   board->params=v;
  }

  if (strcmpi(w,"default")==0)
  {
   board->checked=!strcmpi(v,"checked");
  }

 }


};

