//---------------------------------------------------------------------------
#ifndef utilsH
#define utilsH
//---------------------------------------------------------------------------

class TNodeData:public TObject
{
 public:
 AnsiString url;
 char format[10];
 bool changed;
 TStringList * servlist;
 __fastcall TNodeData()
            {
             changed=false;
             servlist=new TStringList;
            };
 virtual __fastcall ~TNodeData()
            {
             int i;
             for (i=0;i<servlist->Count;i++)
               servlist->Objects[i]->Free();
             servlist->Free();
            };
};


class TServerData:public TObject
{
 public:
 AnsiString ip;
 AnsiString comment;
};

class TActiveServerData:public TObject
{
 public:
 AnsiString name;
 AnsiString level;
 AnsiString comment;
 _GUID guidInst;
 _GUID guidGame;
 int nplayers;
 int maxplayers;
 TStringList * players;
 long user1;
 long user2;
 long user3;
 int avgping;
 int nreplies;
 __fastcall TActiveServerData() { players=new TStringList;};
 virtual __fastcall ~TActiveServerData() { players->Free(); };
};

class TIPBoard:public TObject
{
 public:
 AnsiString name;
 int game;
 AnsiString url;
 bool is_method_post;
 AnsiString params;
 bool checked;
};


#define game_UN 0
#define game_JK 1
#define game_MOTS 2
#define game_OL 3


#define lv_NotFound 0
#define lv_GameDir 1
#define lv_AddOnDir 2


#define Version 0.2

class TStack
{
 int sp;
 void* mem[20];
 public:
 TStack() {sp=0;};
 void Push(void * ptr){if (sp>19) return; mem[sp]=ptr; sp++;};
 void* Pop() {if (sp<1) return NULL; return mem[--sp];};
};

void LoadGroupFile(char * fname, TTreeNodes * nodes);
void LoadIPBoards(char * fname);
TList * GetIPBoards();

void UpdateNodeData(TNodeData * nd);

void PanMessage(char *msg);
bool SaveServerList(const AnsiString & fname,TStringList * sl);


AnsiString GetGOBName(TActiveServerData * asd);
AnsiString GetJKLName(TActiveServerData * asd);


AnsiString GetJKDir();
AnsiString GetMOTSDir();
AnsiString GetOutlawsDir();

AnsiString GetJKAddOnDir();
AnsiString GetMOTSAddOnDir();
void SaveAddOnDirs(const AnsiString& jkdir,const AnsiString& motsdir);

int LocateJKLevel(const AnsiString& level);
int LocateMOTSLevel(const AnsiString& level);
int LocateOutlawsLevel(const AnsiString& level);

bool IsFileInGOB(const AnsiString& gob,const AnsiString& file);

AnsiString GetMyIP();
// bool IsAddrValid(char * adr);
void SetCmdAddOnDir(bool mots,bool addonactive);
AnsiString PathAddSlash(const AnsiString& dir);

#endif
