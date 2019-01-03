//---------------------------------------------------------------------------

#ifndef ShiftH
#define ShiftH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include "ShiftSQL.h"
#include "HeadersBuilder.h"
#include <Graphics.hpp>
#include <Mask.hpp>
#include <Menus.hpp>
#include <vector>

using namespace std;
//---------------------------------------------------------------------------

class TFShift : public TForm
{
__published:	// IDE-managed Components
        TPanel *ConditionPanel;
        TPanel *ShiftPanel;
        TPanel *BtnPanel;
        TPanel *ScrPanel;
        TDrawGrid *ShiftDrawGrid;
        TBitBtn *RefreshBtn;
        TRadioGroup *StanRadioGroup;
        TBitBtn *CloseShiftBtn;
        TBitBtn *CheckBtn;
        TBitBtn *DocumentsBtn;
        TBitBtn *PrnBtn;
        TPanel *Panel6;
   TBevel *Bevel1;
   TLabel *Label3;
   TMaskEdit *BegDate;
   TLabel *Label4;
   TImage *Image2;
   TImage *BegDateImg;
   TMaskEdit *EndDate;
   TLabel *Label5;
   TEdit *DepName;
   TImage *DepImage;
        TBitBtn *BitBtn1;
        TPopupMenu *AdminPopupMenu;
        TMenuItem *NChangeStan;
        TMenuItem *N2;
        TMenuItem *NDeleteShift;
        TMenuItem *NDuplicate;
        void __fastcall ShiftDrawGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall RefreshBtnClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall ConditionsChange(TObject *Sender);
        void __fastcall CloseShiftBtnClick(TObject *Sender);
        void __fastcall CheckBtnClick(TObject *Sender);
        void __fastcall ShiftDrawGridSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall DocumentsBtnClick(TObject *Sender);
        void __fastcall PrnBtnClick(TObject *Sender);
        void __fastcall ShiftDrawGridMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall FormShow(TObject *Sender);
   void __fastcall DepImageClick(TObject *Sender);
   void __fastcall DepNameKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall Image2Click(TObject *Sender);
   void __fastcall BegDateImgClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall ShiftDrawGridKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall NChangeStanClick(TObject *Sender);
        void __fastcall NDeleteShiftClick(TObject *Sender);
        void __fastcall NDuplicateClick(TObject *Sender);
private:	// User declarations
        vector <SHIFT> shiftList;
        //
        THeadersBuilder *HeadTitle;
        //
        int dep_ID;
        vector <int> depList;
public:		// User declarations
        __fastcall TFShift(TComponent* Owner);
        //
        void TFShift::SetTitleCaption( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TFShift *FShift;
//---------------------------------------------------------------------------
#endif
