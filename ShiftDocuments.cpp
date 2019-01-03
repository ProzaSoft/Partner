//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "ShiftDocuments.h"
#include "FormatingData.h"
#include "WorkFiles.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFShiftDocuments *FShiftDocuments;
//---------------------------------------------------------------------------
__fastcall TFShiftDocuments::TFShiftDocuments(TComponent* Owner)
        : TForm(Owner)
{
   shift_ID  = 0;
}
__fastcall TFShiftDocuments::TFShiftDocuments(int shift_id, TComponent* Owner)
        : TForm(Owner)
{
   shift_ID  = shift_id;
   ShiftNumber->Caption = String(shift_ID);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDocuments::ExitBtnClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDocuments::FormShow(TObject *Sender)
{
   //
   TShiftSQL::GetShiftDocs( shift_ID, docList );

   double total_suma = 0;
   for(int i=0;i<(int)docList.size();i++) total_suma += docList[i].suma;
   //
   if ( docList.size() ) {
      DocTovDrawGrid->RowCount = docList.size() + 1;
      DocTovDrawGrid->Row = docList.size();
   }
   else
      DocTovDrawGrid->RowCount = 2;
   //
   SumaTotal->Caption = TFormatingData::DblToStr(total_suma);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDocuments::DocTovDrawGridDrawCell(TObject *Sender,
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
         case 0 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[0]-DocTovDrawGrid->Canvas->TextWidth("№")-LeftPos)/2,Rect.Top+2,"№");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 1 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[1]-DocTovDrawGrid->Canvas->TextWidth("Дата")-LeftPos)/2,Rect.Top+2,"Дата");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 2 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[2]-DocTovDrawGrid->Canvas->TextWidth("Від кого...")-LeftPos)/2,Rect.Top+2,"Від кого...");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 3 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[3]-DocTovDrawGrid->Canvas->TextWidth("Кому...")-LeftPos)/2,Rect.Top+2,"Кому...");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 4 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[4]-DocTovDrawGrid->Canvas->TextWidth("Примітка")-LeftPos)/2,Rect.Top+2,"Примітка");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 5 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[5]-DocTovDrawGrid->Canvas->TextWidth("Сума")-LeftPos)/2,Rect.Top+2,"Сума");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 6 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocTovDrawGrid->ColWidths[5]-DocTovDrawGrid->Canvas->TextWidth("Стан")-LeftPos)/2,Rect.Top+2,"Стан");
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
      }
   }
   else {
      if ( !docList.size() ) return;
      //
      switch( ACol ) {
         case 0 : DocTovDrawGrid->Canvas->TextOut(Rect.Right-2-DocTovDrawGrid->Canvas->TextWidth(docList[ARow-1].number_d),Rect.Top+2,docList[ARow-1].number_d);
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 1 : DocTovDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,docList[ARow-1].date_d);
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 2 : DocTovDrawGrid->Canvas->TextRect(Rect,Rect.Left+1,Rect.Top+2,docList[ARow-1].from_name);
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 3 : DocTovDrawGrid->Canvas->TextRect(Rect,Rect.Left+1,Rect.Top+2,docList[ARow-1].to_name);
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 4 : DocTovDrawGrid->Canvas->TextRect(Rect,Rect.Left+1,Rect.Top+2,docList[ARow-1].coment);
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 5 : DocTovDrawGrid->Canvas->TextOut(Rect.Right-2-DocTovDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(docList[ARow-1].suma)),Rect.Top+2,TFormatingData::DblToStr(docList[ARow-1].suma));
                  DocTovDrawGrid->Canvas->Brush->Color = clBlack;
                  DocTovDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 6 : String stan = "";
               int curColor = clBlack;
               switch( docList[ARow-1].stan ) {
                  case 0  : stan = "  Створений";
                            curColor = clBlue;
                            break;
                  case 1  : stan = "Проведений";
                            curColor = clGreen;
                            break;
                  case 2  : stan = "В стані пров.";
                            curColor = clBlack;
                            break;
                  case 3  : stan = "В стані анул.";
                            curColor = clBlack;
                            break;
                  case 4  : stan = "Резервування";
                            curColor = clRed;
                            break;
                  default : stan = "Невідомий";
                            curColor = clBlack;
                            break;
               }
               if ( DocTovDrawGrid->Row != ARow || DocTovDrawGrid->Col != ACol )
                  DocTovDrawGrid->Canvas->Font->Color = curColor;
               DocTovDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,stan);
               DocTovDrawGrid->Canvas->Brush->Color = clBlack;
               DocTovDrawGrid->Canvas->FrameRect(TheRect);
               break;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDocuments::SendBtnClick(TObject *Sender)
{
   if( !docList.size() ) return;
   //
   vector <TERMINAL_TOVAR> tovarList;
   TShiftSQL::GetDocTovar(docList[DocTovDrawGrid->Row-1].doc_tov_ID, tovarList);

   if( !tovarList.size() ) {
      MessageDlg( String("Звіт порожній!"), mtWarning, TMsgDlgButtons() << mbOK, 0);
      return;
   }
   //
   // Формуємо файл з даними
   char curDir[120];
   GetCurrentDirectory(100,curDir);
   FILE *pf;
   char outbuf[80];
   // Записуємо файл на дискету
   SaveDlg->Title = "Копіювання звітів в TXT-файл";
   SaveDlg->InitialDir = String(curDir);
   SaveDlg->FileName   = String("z_p")+ShiftNumber->Caption+"_"+
                         docList[DocTovDrawGrid->Row-1].date_d.SubString(7,4)+
                         docList[DocTovDrawGrid->Row-1].date_d.SubString(4,2)+
                         docList[DocTovDrawGrid->Row-1].date_d.SubString(1,2)+"_"+
                         docList[DocTovDrawGrid->Row-1].to_name+".txt";
   if ( SaveDlg->Execute() ) {
      if ( (pf = fopen(SaveDlg->FileName.c_str(),"wt")) != NULL ) {
         for( int i=0; i<(int)tovarList.size(); i++ ) {
            fprintf(pf,"%d\t%.3f\t%.2f\n", tovarList[i].kod_ekka,
                                           tovarList[i].quantity_real,
                                           tovarList[i].suma_real);
         }
         fclose(pf);
         ShowMessage("Файл-звіт успішно створено!!!");
      }
      else {
         ShowMessage("Помилка створення файла-звіту!!!");
      }
   }
   //
   SetCurrentDirectory(curDir);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDocuments::SendZnBtnClick(TObject *Sender)
{
   if( !docList.size() ) return;
   //
   vector <TERMINAL_TOVAR> tovarList;
   TShiftSQL::GetDocTovar(docList[DocTovDrawGrid->Row-1].doc_tov_ID, tovarList);

   if( !tovarList.size() ) {
      MessageDlg( String("Звіт порожній!"), mtWarning, TMsgDlgButtons() << mbOK, 0);
      return;
   }
   // Формуємо файл з даними
   char curDir[120];
   GetCurrentDirectory(100,curDir);
   FILE *pf;
   char outbuf[80];
   // Записуємо файл на дискету
   SaveDlg->Title = "Копіювання звітів в TXT-файл";
   SaveDlg->InitialDir = String(curDir);
   SaveDlg->FileName   = String("z_p")+ShiftNumber->Caption+"_"+
                         docList[DocTovDrawGrid->Row-1].date_d.SubString(7,4)+
                         docList[DocTovDrawGrid->Row-1].date_d.SubString(4,2)+
                         docList[DocTovDrawGrid->Row-1].date_d.SubString(1,2)+"_"+
                         docList[DocTovDrawGrid->Row-1].to_name+".txt";
   if ( SaveDlg->Execute() ) {
      if ( (pf = fopen(SaveDlg->FileName.c_str(),"wt")) != NULL ) {
         for( int i=0; i<(int)tovarList.size(); i++ ) {
            fprintf(pf,"%d\t%.3f\t%.2f\t%.2f\n",tovarList[i].kod_ekka,
                                           tovarList[i].quantity_real,
                                           tovarList[i].suma_real,
                                           tovarList[i].suma_no_discount);
         }
         fclose(pf);
         ShowMessage("Файл-звіт успішно створено!!!");
      }
      else {
         ShowMessage("Помилка створення файла-звіту!!!");
      }
   }
   //
   SetCurrentDirectory(curDir);
}
//---------------------------------------------------------------------------

