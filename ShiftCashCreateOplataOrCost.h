//---------------------------------------------------------------------------

#ifndef ShiftCashCreateOplataOrCostH
#define ShiftCashCreateOplataOrCostH
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

class TFShiftCashCreateOplataOrCost : public TForm
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
        TLabel *DepLabel;
        TLabel *PointOfMoneyLabel;
        void __fastcall ExitBtnClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall DocTovDrawGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall DocTovDrawGridSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
private:	// User declarations
        int    shift_ID;
        int    dep_ID, point_of_money_ID;
        //
        vector <SHIFT_CASH> docList;
public:		// User declarations
        __fastcall TFShiftCashCreateOplataOrCost(int tovar_id, TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFShiftCashCreateOplataOrCost *FShiftCashCreateOplataOrCost;
//---------------------------------------------------------------------------
#endif
