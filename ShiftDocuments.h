//---------------------------------------------------------------------------

#ifndef ShiftDocumentsH
#define ShiftDocumentsH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
#include "ShiftSQL.h"
//---------------------------------------------------------------------------
#include <vector>
#include <algorithm>

using namespace std;
//---------------------------------------------------------------------------

class TFShiftDocuments : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel2;
        TBitBtn *ExitBtn;
        TLabel *Label11;
        TLabel *ShiftNumber;
        TPanel *Panel1;
        TPanel *Panel3;
        TDrawGrid *DocTovDrawGrid;
        TLabel *SumaTotal;
        TLabel *Label1;
        TBitBtn *SendBtn;
        TSaveDialog *SaveDlg;
        TBitBtn *SendZnBtn;
        void __fastcall ExitBtnClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall DocTovDrawGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall SendBtnClick(TObject *Sender);
        void __fastcall SendZnBtnClick(TObject *Sender);
private:	// User declarations
        int    shift_ID;
        //
        vector <SHIFT_DOCS> docList;
public:		// User declarations
        __fastcall TFShiftDocuments(TComponent* Owner);
        __fastcall TFShiftDocuments(int tovar_id, TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFShiftDocuments *FShiftDocuments;
//---------------------------------------------------------------------------
#endif
