//---------------------------------------------------------------------------

#ifndef ShiftCheckH
#define ShiftCheckH
//---------------------------------------------------------------------------
#include "ShiftSQL.h"
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <StdCtrls.hpp>
#include "HeadersBuilder.h"
//---------------------------------------------------------------------------
#include <vector>
#include <algorithm>

using namespace std;
//---------------------------------------------------------------------------
static bool   sortDirect;
//---------------------------------------------------------------------------
class TFShiftCheck : public TForm
{
__published:	// IDE-managed Components
        TPanel *BtnPanel;
        TSplitter *Splitter1;
        TSpeedButton *ExitBtn;
        TPanel *PanelLeft;
        TPanel *PanelCheck;
        TDrawGrid *CheckDrawGrid;
        TPanel *PanelSuma;
        TLabel *TotalSuma;
        TImageList *ImageList;
        TPanel *PanelRight;
        TPanel *PanelTovar;
        TDrawGrid *TovDrawGrid;
        TPanel *Panel1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *SumTovLabel;
        TLabel *SpTovLabel;
        TSpeedButton *PrnBtn;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label10;
        TLabel *TotalRepayment;
        TLabel *ZnTovLabel;
        TLabel *Label11;
        TLabel *ShiftNumber;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *DateBeg;
        TLabel *DateEnd;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *CashRegisterName;
        TLabel *UserName;
        TLabel *Label21;
        TLabel *StanShift;
        TSpeedButton *SHBitBtn;
        TLabel *BonusLabel;
        TLabel *Bonus;
        TDrawGrid *ShiftCalculationDrawGrid;
        TLabel *Label1;
        TDrawGrid *ZakazCalculationDrawGrid;
        TPanel *Panel6;
   TPanel *Panel2;
        TLabel *ComentLabel;
   TLabel *Label7;
   TLabel *AgentSuma;
   TLabel *Label9;
   TLabel *RozdribSuma;
   TLabel *Label15;
   TLabel *ViddilSuma;
   TLabel *Label19;
   TLabel *RepaymentViddil;
   TShape *Shape1;
   TShape *Shape2;
   TShape *Shape3;
   TShape *Shape4;
   TLabel *Label8;
   TLabel *CashIn;
   TLabel *Label16;
   TLabel *CashOut;
   TLabel *DiscountCardInfo;
   TShape *Shape5;
   TLabel *Label12;
   TLabel *CashInSafe;
        TLabel *LBonusPlus;
        TLabel *BonusPlus;
        TLabel *LBonusMinus;
        TLabel *BonusMinus;
        TLabel *CheckInfoLabel;
        TLabel *CreditCardCheckLabel;
        void __fastcall TovDrawGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall CheckDrawGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall ExitBtnClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CheckDrawGridClick(TObject *Sender);
        void __fastcall CheckDrawGridMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall PrnBtnClick(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall SHBitBtnClick(TObject *Sender);
        void __fastcall ShiftCalculationDrawGridDrawCell(TObject *Sender,
          int ACol, int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall ZakazCalculationDrawGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall Splitter1Moved(TObject *Sender);
        void __fastcall CheckDrawGridMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall TovDrawGridMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall CashDblClick(TObject *Sender);
private:	// User declarations
        vector <ZAKAZ> zakazList;
        vector <ZAKAZ_CALCULATION> shiftCalculationList, zakazCalculationList;
        vector <ZAKAZ_ROW> rowList;
        void RefreshTov(int check_ID);
        void RecountTotalSum(); // формування підсумкових сум по зміні

        static bool compNumber( ZAKAZ a, ZAKAZ b );
        static bool compTime( ZAKAZ a, ZAKAZ b );
        static bool compName( ZAKAZ a, ZAKAZ b );
        static bool compZn( ZAKAZ a, ZAKAZ b );
        static bool compSuma( ZAKAZ a, ZAKAZ b );

        void SortDocList( void );
        void sortNumber( void ) { sort(zakazList.begin(),zakazList.end(),compNumber); }
        void sortTime( void ) { sort(zakazList.begin(),zakazList.end(),compTime); }
        void sortName( void ) { sort(zakazList.begin(),zakazList.end(),compName); }
        void sortZn( void ) { sort(zakazList.begin(),zakazList.end(),compZn); }
        void sortSuma( void ) { sort(zakazList.begin(),zakazList.end(),compSuma); }
        //
        int    sortOrder;
        //
        int shift_ID;  // Активна зміна
        int BONUS_ON;
        //
        THeadersBuilder *HeadTitleCheck;
        THeadersBuilder *HeadTitleTovar;
        //
        int QUANTITY_SCALE;
        int PRICE_OUT_SCALE;
public:		// User declarations
        __fastcall TFShiftCheck(int shift_id, TComponent* Owner);
        //
        void SetTitleCaptionCheck( void );
        void SetTitleCaptionTovar( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TFShiftCheck *FShiftCheck;
//---------------------------------------------------------------------------
#endif
