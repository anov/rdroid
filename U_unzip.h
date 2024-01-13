//---------------------------------------------------------------------------
#ifndef U_unzipH
#define U_unzipH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TUnZipForm : public TForm
{
__published:	// IDE-managed Components
    TProgressBar *ProgressBar;
    TButton *BNCancel;
    void __fastcall BNCancelClick(TObject *Sender);
private:	// User declarations
    AnsiString Zip;
    AnsiString Dir;
    bool cancelled;
    int NFiles;
    int CFile;
    bool UnzipIt();
public:		// User declarations
    __fastcall TUnZipForm(TComponent* Owner);
 bool Unzip(const AnsiString& zip,const AnsiString& dir);
protected:
 virtual void __fastcall WndProc(TMessage &Message);
};
//---------------------------------------------------------------------------
extern PACKAGE TUnZipForm *UnZipForm;
//---------------------------------------------------------------------------
#endif
