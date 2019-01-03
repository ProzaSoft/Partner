//---------------------------------------------------------------------------
#ifndef ShowStatusHistoryH
#define ShowStatusHistoryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
#include "StatusHistorySQL.h"
#include "TerminalAdminSQL.h"
#include <Graphics.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------

class TFShowStatusHistory : public TForm
{
__published:	// IDE-managed Components
        TPanel *BtnPanel;
        TPanel *PanelLeft;
        TPanel *PanelRigth;
        TDrawGrid *ActionDrawGrid;
        TDrawGrid *StatusDrawGrid;
        TSplitter *Splitter1;
        TImageList *ImageList;
        TGroupBox *GroupBox1;
        TCheckBox *SelectCheckBox;
        TGroupBox *GroupBox2;
        TCheckBox *ActCheckBox;
        TLabel *FullName;
        TBevel *Bevel1;
        TPanel *Panel6;
        TBevel *Bevel2;
        TLabel *Label3;
        TImage *BegDateImg;
        TLabel *Label4;
        TImage *Image2;
        TBevel *Bevel3;
        TLabel *Label2;
        TLabel *Label5;
        TEdit *CashRegisterName;
        TEdit *UserName;
        TImage *UserImage;
        TImage *CashRegisterImage;
        TMaskEdit *BegDate;
        TMaskEdit *EndDate;
        TLabel *Label1;
        TEdit *KodEdit;
        TEdit *TovEdit;
        TSpeedButton *RefreshBtn;
        TSpeedButton *PrnBtn;
        TLabel *Label6;
        TEdit *DepName;
        TImage *DepImage;
        TSpeedButton *CheckActionBtn;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall StatusDrawGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall StatusDrawGridDblClick(TObject *Sender);
        void __fastcall ActionDrawGridDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall SelectCheckBoxClick(TObject *Sender);
        void __fastcall ActionDrawGridDblClick(TObject *Sender);
        void __fastcall ActCheckBoxClick(TObject *Sender);
        void __fastcall Splitter1Moved(TObject *Sender);
        void __fastcall ActionDrawGridMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall UserImageClick(TObject *Sender);
        void __fastcall CashRegisterImageClick(TObject *Sender);
        void __fastcall KodEditClick(TObject *Sender);
        void __fastcall KodEditExit(TObject *Sender);
        void __fastcall KodEditKeyPress(TObject *Sender, char &Key);
        void __fastcall RefreshBtnClick(TObject *Sender);
        void __fastcall BegDateImgClick(TObject *Sender);
        void __fastcall Image2Click(TObject *Sender);
        void __fastcall DepImageClick(TObject *Sender);
        void __fastcall CheckActionBtnClick(TObject *Sender);
        void __fastcall PrnBtnClick(TObject *Sender);
private:	// User declarations
        int tovar_ID, user_ID, cash_register_ID, dep_ID;
        //
        vector <STATUSES>      statusList;
        vector <STATUS_ACTION> actionList;
        //
        void RefreshAction();
        void ColVisible();
        void FindTovar();
public:		// User declarations
        __fastcall TFShowStatusHistory(TComponent* Owner);
        __fastcall TFShowStatusHistory(int tovar_id, int dep_id, String beg_date, String end_date, TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFShowStatusHistory *FShowStatusHistory;
//---------------------------------------------------------------------------
#endif
