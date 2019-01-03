//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "ShiftCashCreateOplataOrCost.h"
#include "FormatingData.h"
#include "WorkFiles.h"
#include "Cost_new.h"
#include "Oplata_new.h"
#include "com_st.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFShiftCashCreateOplataOrCost *FShiftCashCreateOplataOrCost;
//---------------------------------------------------------------------------

__fastcall TFShiftCashCreateOplataOrCost::TFShiftCashCreateOplataOrCost(int shift_id, TComponent* Owner)
        : TForm(Owner)
{
   shift_ID  = shift_id;
   ShiftNumber->Caption = String(shift_ID);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftCashCreateOplataOrCost::ExitBtnClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TFShiftCashCreateOplataOrCost::FormShow(TObject *Sender)
{
   dep_ID = point_of_money_ID = 0;
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
   //
   String department_name, point_of_money_name;
   TShiftSQL::GetDepartmentInfoFromShift( shift_ID, dep_ID, department_name, point_of_money_ID, point_of_money_name );
   DepLabel->Caption = String("Департамент: ")+department_name;
   PointOfMoneyLabel->Caption = String("Джерело коштів: ")+point_of_money_name;
}
//---------------------------------------------------------------------------

void __fastcall TFShiftCashCreateOplataOrCost::DocTovDrawGridDrawCell(TObject *Sender,
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
         case 4 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[4]-DocTovDrawGrid->Canvas->TextWidth("Тип")-LeftPos)/2,Rect.Top+2,"Тип");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 5 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[5]-DocTovDrawGrid->Canvas->TextWidth("Інформація по документу")-LeftPos)/2,Rect.Top+2,"Інформація по документу");
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
         case 4 : if( docList[ARow-1].cash_type ) {
                     switch( docList[ARow-1].cash_type ) {
                        case 1:  DocTovDrawGrid->Canvas->Brush->Color = clYellow;
                                 DocTovDrawGrid->Canvas->FillRect(TheRect);
                                 DocTovDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,"Витрата");
                                 break;
                        case 2:  DocTovDrawGrid->Canvas->Brush->Color = clGreen;
                                 DocTovDrawGrid->Canvas->FillRect(TheRect);
                                 DocTovDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,"Оплата");
                                 break;
                        case 3:  DocTovDrawGrid->Canvas->Brush->Color = clBlue;
                                 DocTovDrawGrid->Canvas->FillRect(TheRect);
                                 DocTovDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,"Вн.оплата");
                                 break;
                        default: DocTovDrawGrid->Canvas->Brush->Color = clWindow;
                                 DocTovDrawGrid->Canvas->FillRect(TheRect);
                                 break;
                     }
                  }
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 5 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,docList[ARow-1].doc_info);
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShiftCashCreateOplataOrCost::DocTovDrawGridSelectCell(
      TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
   if( (int)!docList.size() ) return;
   //
   if( ACol==4 ) {
      if( docList[ARow-1].cost_id || docList[ARow-1].oplata_id ) return;
      docList[ARow-1].cash_type++;
      if( docList[ARow-1].cash_type > 3 ) docList[ARow-1].cash_type = 0;
      TShiftSQL::SaveCashTypeInShiftCash( docList[ARow-1].cash_id, docList[ARow-1].cash_type );
      DocTovDrawGrid->Refresh();
   }
   if( ACol==5 ) {
      if( docList[ARow-1].cash_type == 1 ) { // Витрата
         if( !docList[ARow-1].cost_id ) {
            FCost_new = new TFCost_new(0,point_of_money_ID,dep_ID,0,0,FIELD::ConvertDateSqlPartner(docList[ARow-1].date_time_input.SubString(1,10)),docList[ARow-1].cash_out,docList[ARow-1].coment.c_str(),Application);
            FCost_new->ShowModal();
            docList[ARow-1].cost_id = FCost_new->GetDocID();
            TShiftSQL::SaveCostIDInShiftCash( docList[ARow-1].cash_id, docList[ARow-1].cost_id );
            docList[ARow-1].doc_info = TShiftSQL::LoadDocInfoFromCost( docList[ARow-1].cost_id );
            delete FCost_new;
         }
         else {
            FCost_new = new TFCost_new(0,0,docList[ARow-1].cost_id,0,Application);
            FCost_new->ShowModal();
            delete FCost_new;
         }
         DocTovDrawGrid->Refresh();
         ACol=1;
      }
      if( docList[ARow-1].cash_type == 2 ) { // Оплата на постачальника
         if( !docList[ARow-1].oplata_id ) {
            FOplata_new = new TFOplata_new(point_of_money_ID,0,0,1,docList[ARow-1].cash_out,docList[ARow-1].coment.c_str(),0,Application);
            FOplata_new->ShowModal();
            docList[ARow-1].oplata_id = FOplata_new->GetDocID();
            TShiftSQL::SaveOplataIDInShiftCash( docList[ARow-1].cash_id, docList[ARow-1].oplata_id );
            docList[ARow-1].doc_info = TShiftSQL::LoadDocInfoFromOplata( docList[ARow-1].oplata_id );
            delete FOplata_new;
         }
         else {
            FOplata_new = new TFOplata_new(0,0,docList[ARow-1].oplata_id,1,Application);
            FOplata_new->ShowModal();
            delete FOplata_new;
         }
         DocTovDrawGrid->Refresh();
         ACol=1;
      }
      if( docList[ARow-1].cash_type == 3 ) { // Оплата внутрішнє преміщення
         if( !docList[ARow-1].oplata_id ) {
            if( docList[ARow-1].cash_out )
               FOplata_new = new TFOplata_new(point_of_money_ID,0,0,2,docList[ARow-1].cash_out,docList[ARow-1].coment.c_str(),0,Application);
            else
               FOplata_new = new TFOplata_new(0,point_of_money_ID,0,2,docList[ARow-1].cash_in,docList[ARow-1].coment.c_str(),0,Application);
            FOplata_new->ShowModal();
            docList[ARow-1].oplata_id = FOplata_new->GetDocID();
            TShiftSQL::SaveOplataIDInShiftCash( docList[ARow-1].cash_id, docList[ARow-1].oplata_id );
            docList[ARow-1].doc_info = TShiftSQL::LoadDocInfoFromOplata( docList[ARow-1].oplata_id );
            delete FOplata_new;
         }
         else {
            FOplata_new = new TFOplata_new(0,0,docList[ARow-1].oplata_id,2,Application);
            FOplata_new->ShowModal();
            delete FOplata_new;
         }
         DocTovDrawGrid->Refresh();
         ACol=1;
      }
   }
}
//---------------------------------------------------------------------------

