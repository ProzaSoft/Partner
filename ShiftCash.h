//---------------------------------------------------------------------------

#ifndef ShiftCashH
#define ShiftCashH
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

class TFShiftCash : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel2;
        TBitBtn *ExitBtn;
        TLabel *Label11;
        TLabel *ShiftNumber;
        TPanel *Panel1;
        TPanel *Panel3;
        TDrawGrid *DocTovDrawGrid;
        TLabel *TotalCashOut;
        TLabel *Label1;
        TLabel *TotalCashIn;
        void __fastcall ExitBtnClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall DocTovDrawGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
private:	// User declarations
        int    shift_ID;
        //
        vector <SHIFT_CASH> docList;
public:		// User declarations
        __fastcall TFShiftCash(int tovar_id, TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFShiftCash *FShiftCash;
//---------------------------------------------------------------------------
#endif
