//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "ShiftCash.h"
#include "FormatingData.h"
#include "WorkFiles.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFShiftCash *FShiftCash;
//---------------------------------------------------------------------------
__fastcall TFShiftCash::TFShiftCash(int shift_id, TComponent* Owner)
        : TForm(Owner)
{
   shift_ID  = shift_id;
   ShiftNumber->Caption = String(shift_ID);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftCash::ExitBtnClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TFShiftCash::FormShow(TObject *Sender)
{
   //
   TShiftSQL::LoadShiftCashList( shift_ID, docList );

   double total_cash_in  = 0;
   double total_cash_out = 0;
   for(int i=0;i<(int)docList.size();i++) {
      total_cash_in  += docList[i].cash_in;
      total_cash_out += docList[i].cash_out;
   }
   //
   if ( docList.size() ) {
      DocTovDrawGrid->RowCount = docList.size() + 1;
      DocTovDrawGrid->Row = docList.size();
   }
   else
      DocTovDrawGrid->RowCount = 2;
   //
   TotalCashIn->Caption  = TFormatingData::DblToStr(total_cash_in);
   TotalCashOut->Caption = TFormatingData::DblToStr(total_cash_out);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftCash::DocTovDrawGridDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
   TRect TheRect;
   TheRect.Left = Rect.Left-1;
   TheRect.Top = Rect.Top-1;
   TheRect.Right = Rect.Right+1;
   TheRect.Bottom = Rect.Bottom+1;
   //
   if ( !ARow ) {
      int LeftPos = 0;
      switch( ACol ) {
         case 0 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[0]-DocTovDrawGrid->Canvas->TextWidth("Дата/час")-LeftPos)/2,Rect.Top+2,"Дата/час");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 1 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[1]-DocTovDrawGrid->Canvas->TextWidth("Сл.внесення")-LeftPos)/2,Rect.Top+2,"Сл.внесення");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 2 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[2]-DocTovDrawGrid->Canvas->TextWidth("Сл.видача")-LeftPos)/2,Rect.Top+2,"Сл.видача");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 3 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[3]-DocTovDrawGrid->Canvas->TextWidth("Коментар")-LeftPos)/2,Rect.Top+2,"Коментар");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
      }
   }
   else {
      if ( !docList.size() ) return;
      //
      switch( ACol ) {
         case 0 : DocTovDrawGrid->Canvas->TextOut(Rect.Right-2-DocTovDrawGrid->Canvas->TextWidth(docList[ARow-1].date_time_input),Rect.Top+2,docList[ARow-1].date_time_input);
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 1 :
                  if( TFormatingData::scaleDbl(docList[ARow-1].cash_in) )
                     DocTovDrawGrid->Canvas->TextOut(Rect.Right-2-DocTovDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(docList[ARow-1].cash_in)),Rect.Top+2,TFormatingData::DblToStr(docList[ARow-1].cash_in));
                  else
                     DocTovDrawGrid->Canvas->TextOut(Rect.Left,Rect.Top+2,"");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 2 :
                  if( TFormatingData::scaleDbl(docList[ARow-1].cash_out) )
                     DocTovDrawGrid->Canvas->TextOut(Rect.Right-2-DocTovDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(docList[ARow-1].cash_out)),Rect.Top+2,TFormatingData::DblToStr(docList[ARow-1].cash_out));
                  else
                     DocTovDrawGrid->Canvas->TextOut(Rect.Left,Rect.Top+2,"");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 3 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,docList[ARow-1].coment);
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
      }
   }
}
//---------------------------------------------------------------------------



