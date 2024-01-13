//---------------------------------------------------------------------------
#ifndef mainformH
#define mainformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "p_thread.h"
#include <NMUDP.hpp>
#include <WSocket.hpp>
//---------------------------------------------------------------------------


#define WM_TAKEURL WM_USER+100
#define URLTAKEN 0x33

class TJSForm : public TForm
{
__published:	// IDE-managed Components
    TTreeView *TVServTree;
    TListView *LVServList;
    TListView *LVActiveServs;
    TPanel *PMsg;
    TPanel *Panel2;
    TLabel *Label2;
    TPanel *Panel3;
    TLabel *Label3;
    TLabel *Label4;
    TMainMenu *MainMenu;
    TMenuItem *File1;
    TMenuItem *Exit1;
    TPopupMenu *GroupMenu;
    TMenuItem *UpdateNodes;
    TMenuItem *Findactiveservers;
    TMenuItem *Properties1;
    TPopupMenu *ListMenu;
    TMenuItem *Updatethislist1;
    TMenuItem *Pollservers;
    TMenuItem *Properties2;
    TPopupMenu *Servmenu;
    TMenuItem *Connect;
    TMenuItem *UpdateAServ;
    TMenuItem *Updateall1;
    TMenuItem *UpdateFindActive;
    TMenuItem *PropertiesAServ;
    TMenuItem *Remove;
    TMenuItem *RemoveAll;
    TMenuItem *DeleteServers;
    TMenuItem *MAddServer;
    TMenuItem *Updategroup1;
    TMenuItem *FindActiveServers1;
    TMenuItem *UpdateandFindServers1;
    TMenuItem *List;
    TMenuItem *SelectAll1;
    TMenuItem *Host1;
    TMenuItem *Joingame1;
    TMenuItem *Hostgame1;
    TMenuItem *PollServers1;
    TMenuItem *Properties3;
    TMenuItem *AddServer1;
    TMenuItem *DeleteServers1;
    TMenuItem *Servers1;
    TMenuItem *Connect2;
    TMenuItem *Update1;
    TMenuItem *UpdateAll2;
    TMenuItem *Remove1;
    TMenuItem *RemoveAll1;
    TMenuItem *Properties4;
    TMenuItem *Help;
    TMenuItem *About1;
    TMenuItem *Cancelpolling;
    TMenuItem *Cancelpolling2;
    TImageList *ImageList;
    TMenuItem *UpdateAllLists1;
    TMenuItem *PollIP1;
    TMenuItem *CopyServerTo1;
    TMenuItem *CopyServerTo;
    TMenuItem *CopyServerTo2;
    TMenuItem *CopyServerTo3;
    TServerSocket *ServerSocket;
    TTimer *Timer;
    TNMUDP *DPUDP;
    TNMUDP *OLUDP;
    TMenuItem *Options1;
    TMenuItem *PollAllLists;
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall PollserversClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);



    void __fastcall TVServTreeChange(TObject *Sender, TTreeNode *Node);
    void __fastcall UpdateNodesClick(TObject *Sender);

    void __fastcall FindactiveserversClick(TObject *Sender);

    void __fastcall UpdateAServClick(TObject *Sender);
    void __fastcall Updateall1Click(TObject *Sender);
    void __fastcall LVServListDblClick(TObject *Sender);
    void __fastcall LVActiveServsDblClick(TObject *Sender);
    void __fastcall UpdateFindActiveClick(TObject *Sender);


    void __fastcall RemoveClick(TObject *Sender);
    void __fastcall RemoveAllClick(TObject *Sender);
    void __fastcall LVActiveServsKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);

    void __fastcall TVServTreeDblClick(TObject *Sender);
    void __fastcall Properties2Click(TObject *Sender);
    void __fastcall DeleteServersClick(TObject *Sender);
    void __fastcall MAddServerClick(TObject *Sender);
    void __fastcall PropertiesAServClick(TObject *Sender);

    void __fastcall Hostgame1Click(TObject *Sender);
    void __fastcall Cancelpolling2Click(TObject *Sender);
    void __fastcall CancelpollingClick(TObject *Sender);
    void __fastcall ConnectClick(TObject *Sender);
    void __fastcall About1Click(TObject *Sender);
    void __fastcall Joingame1Click(TObject *Sender);
    
    void __fastcall SelectAll1Click(TObject *Sender);
    void __fastcall Updatethislist1Click(TObject *Sender);
    void __fastcall UpdateAllLists1Click(TObject *Sender);
    
    
    
    
    
    
    
    void __fastcall PollIP1Click(TObject *Sender);
    
    void __fastcall CopyServerToClick(TObject *Sender);
    void __fastcall CopyServerTo2Click(TObject *Sender);
    
    void __fastcall LVServListKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);

    
    void __fastcall Options1Click(TObject *Sender);
    void __fastcall PollAllListsClick(TObject *Sender);
private:	// User declarations
    TServerPollThread * pthread;
public:		// User declarations
    __fastcall TJSForm(TComponent* Owner);
    void DisableListenPorts();
    void PollAllServers(TStringList * sl);
    void PollNode(TTreeNode * node);
    void UpdateNode(TTreeNode * node);
    void UpdateActiveOnes(bool all);
    void DeleteListItem(TListItem * li);
    bool IsNodeEditable(TTreeNode * node);
    bool IsNodeLocal(TTreeNode * node);
    void DeleteServer(int n);
    void AddServer(const AnsiString & ip,const AnsiString & comment);
    void SaveNode(TTreeNode * node);
    void PostToDarkJedi(int game, char *comment,int duration);
    void PostToJKNET(int game, char * ip,char *comment,char * level,char * mod, int rank, int players);
    void HandleCommandLine(char * cmd);
    void CreateCopyToMenu(TMenuItem * mi);
    void ASCreateCopyToMenu(TMenuItem * mi);
protected:
  void __fastcall WMTakeURL(TMessage &Message);
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_TAKEURL, TMessage, WMTakeURL)
END_MESSAGE_MAP(TComponent)

};
//---------------------------------------------------------------------------
extern PACKAGE TJSForm *JSForm;
//---------------------------------------------------------------------------
#endif
