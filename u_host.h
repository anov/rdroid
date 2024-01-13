//---------------------------------------------------------------------------
#ifndef u_hostH
#define u_hostH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <checklst.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class THostForm : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TButton *BNHost;
    TButton *BNCancel;
    TPageControl *PGGames;
    TTabSheet *TBJK;
    TTabSheet *TBMOTS;
    TLabel *Label1;
    TButton *BNJKDefaults;
    TComboBox *CBJKTime;
    TLabel *Label3;
    TMemo *JKMemo;
    TLabel *Label2;
    TLabel *Label4;
    TComboBox *CBJKRank;
    TLabel *Label7;
    TComboBox *CBJKLevel;
    TLabel *Label6;
    TComboBox *CBJKPlayers;
    TComboBox *CBJKMod;
    TLabel *Label8;
    TLabel *Label5;
    TButton *BNMDefaults;
    TLabel *Label9;
    TComboBox *CBMTime;
    TMemo *MMemo;
    TLabel *Label10;
    TComboBox *CBMRank;
    TLabel *Label11;
    TLabel *Label12;
    TComboBox *CBMPlayers;
    TLabel *Label13;
    TComboBox *CBMLevel;
    TLabel *Label14;
    TComboBox *CBMMod;
    TTabSheet *TBOL;
    TLabel *Label15;
    TButton *BNOLDefaults;
    TComboBox *CBOLTime;
    TMemo *OLMemo;
    TLabel *Label16;
    TLabel *Label17;
    TRadioGroup *RGOLNDriver;
    TCheckBox *CBJKAddon;
    TCheckBox *CBMOTSAddon;
    TCheckListBox *LBPostMOTS;
    TCheckListBox *LBPostJK;
    TCheckListBox *LBPostOL;
    void __fastcall BNJKDefaultsClick(TObject *Sender);
    void __fastcall BNMDefaultsClick(TObject *Sender);
    
    
    
    
    
    void __fastcall BNOLDefaultsClick(TObject *Sender);
    
    
private:	// User declarations
    void PostToBoards(TCheckListBox * cbl);
    void LoadIPBoards();
    AnsiString GetString(int what);
public:		// User declarations
    __fastcall THostForm(TComponent* Owner);
    void HostDialog();
};
//---------------------------------------------------------------------------
extern PACKAGE THostForm *HostForm;
//---------------------------------------------------------------------------
#endif
