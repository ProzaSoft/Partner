//---------------------------------------------------------------------------

#ifndef StatusHistoryExellioH
#define StatusHistoryExellioH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <Grids.hpp>

#include "ShiftSQL.h"
#include "StatusHistorySQL.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
#include <vector>

using namespace std;
//---------------------------------------------------------------------------
class TFStatusHistoryExellio : public TForm
{
__published:	// IDE-managed Components
        TPanel *BtnPanel;
        TPanel *Panel1;
        TDrawGrid *StatusDrawGrid;
        TSpeedButton *PrnBtn;
        TSpeedButton *ExitBtn;
        void __fastcall StatusDrawGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormKeyPress(TObject *Sender, char &Key);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall PrnBtnClick(TObject *Sender);
        void __fastcall ExitBtnClick(TObject *Sender);
private:	// User declarations
        int doc_ID;
        //
        vector <DOC_HISTORY> status;
public:		// User declarations
        __fastcall TFStatusHistoryExellio(TComponent* Owner);
        __fastcall TFStatusHistoryExellio(int doc_id, TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFStatusHistoryExellio *FStatusHistoryExellio;
//---------------------------------------------------------------------------
#endif
