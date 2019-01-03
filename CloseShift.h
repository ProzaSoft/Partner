//---------------------------------------------------------------------------

#ifndef CloseShiftH
#define CloseShiftH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "ShiftSQL.h"
#include <map>
#include <vector>
#include <algorithm>

using namespace std;
//---------------------------------------------------------------------------
class TFCloseShift : public TForm
{
__published:	// IDE-managed Components
        TTimer *ProgressTimer;
        TImageList *StatusBarImageList;
        TPanel *Panel1;
        TLabel *InfoLabel;
        TListBox *InfoListBox;
        TStatusBar *StatusBar;
   TPanel *Panel2;
   TButton *StartBtn;
        void __fastcall ProgressTimerTimer(TObject *Sender);
        void __fastcall StatusBarDrawPanel(TStatusBar *StatusBar,
          TStatusPanel *Panel, const TRect &Rect);
        void __fastcall StartBtnClick(TObject *Sender);
private:	// User declarations
     map<int,TERMINAL_TOVAR> Tovar;
     //
     int dd, mm, rrrr, p_index;
     int shift_ID;
     //
     // Створити документ на реалізацію товарів
     bool   CreateRealDocument( void );
     void   AddInfo( String list_text, String label_text, int empty_row, int pause_sec );
     String GetZakazCalculationInfo( vector <ZAKAZ_CALCULATION> total_calculation );
     bool   RecreateNakl( int shift_id, String &time_doc );
     //
     void  TimeOut( int sec );
     void  StopTimer( void );
     void  StartTimer( String info );
     TRect ProgressRect;
     int   LeftPos;
     int   NumImage;
     //
public:		// User declarations
     __fastcall TFCloseShift(int shift_id, TComponent* Owner);
     //
};
//---------------------------------------------------------------------------
extern PACKAGE TFCloseShift *FCloseShift;
//---------------------------------------------------------------------------
#endif
