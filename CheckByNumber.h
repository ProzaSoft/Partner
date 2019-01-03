//---------------------------------------------------------------------------

#ifndef CheckByNumberH
#define CheckByNumberH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <vector>
//---------------------------------------------------------------------------
#include "ShiftSQL.h"
//---------------------------------------------------------------------------

using namespace std;

class TFCheckByNumber : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel2;
        TPanel *Panel1;
        TDrawGrid *DocDrawGrid;
        TPanel *Panel3;
        TBitBtn *ExitBtn;
        TMemo *MemoInfo;
        TLabel *NumberLabel;
        TEdit *Number;
        TSpeedButton *SHBitBtn;
        TSpeedButton *PrnBtn;
        void __fastcall ExitBtnClick(TObject *Sender);
        void __fastcall DocDrawGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall NumberKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall PrnBtnClick(TObject *Sender);
        void __fastcall SHBitBtnClick(TObject *Sender);
private:	// User declarations
        vector <CHECK_BY_NUMBER> docList;
public:		// User declarations
        __fastcall TFCheckByNumber(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFCheckByNumber *FCheckByNumber;
//---------------------------------------------------------------------------
#endif
