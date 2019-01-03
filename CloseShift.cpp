//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CloseShift.h"
#include "FormatingData.h"
#include "ShiftSQL.h"
#include "WorkFiles.h"
#include "SubMethods.h"
#include "DBService.h"
#include "WorkDocTov.h"
#include "MaterialDoc.h"
#include "ShiftCashCreateOplataOrCost.h"

#include "com_st.h"
#include <time.h>
#include <dos.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFCloseShift *FCloseShift;

//---------------------------------------------------------------------------

__fastcall TFCloseShift::TFCloseShift(int shift_id, TComponent* Owner)
        : TForm(Owner)
{
   shift_ID = shift_id;
   InfoLabel->Caption = "";
}
//---------------------------------------------------------------------------

bool TFCloseShift::CreateRealDocument( void )
{
   //   Читання даних з термінальних чеків
   AddInfo( "Зчитування даних...", "Зчитування даних", 0, 2 );
   // Виділяємо змінні
   String query = String("");
   //
   //
   // 1. Читаємо дані по зміні
   int agent_id; // Відділ продажу
   int agent_to_id; // Покупець
   String date_beg_shift, time_beg_shift;
   String date_end_shift, time_end_shift;
   String date_current;
   bool check_recreate;
   String zzvit_info; // Інформація про Z-звіт
   String oplata_info; // Інформація про оплати по Z-звіту
   int stan_shift; // Стан зміни. Формувати документи дозволити лише для стану 1
   SHIFT shiftE;
   vector <ZAKAZ_CALCULATION> total_calculation;
   //
   TShiftSQL::LoadShift(shiftE,shift_ID);
   //
   agent_id = shiftE.agent_ID;
   if( !agent_id )
      agent_id = TShiftSQL::LoadDefaultStore( );
   //
   if( shiftE.datetime_beg.SubString(3,1)=='.' && shiftE.datetime_beg.SubString(6,1)=='.' )
      date_beg_shift = shiftE.datetime_beg.SubString(1,10);
   else
      date_beg_shift = FIELD::ConvertDateSystemPartner(shiftE.datetime_beg.SubString(1,10));
   time_beg_shift = TimeToStr(StrToTime(shiftE.datetime_beg.SubString(11,8)));
   if( shiftE.datetime_end.SubString(3,1)=='.' && shiftE.datetime_end.SubString(6,1)=='.' )
      date_end_shift = shiftE.datetime_end.SubString(1,10);
   else
      date_end_shift = FIELD::ConvertDateSystemPartner(shiftE.datetime_end.SubString(1,10));
   time_end_shift = TimeToStr(StrToTime(shiftE.datetime_end.SubString(11,8)));
   zzvit_info = String("Зміна № ")+shiftE.shift_ID+
                   "; Z-№"+shiftE.zzvit_number+
                   " "+shiftE.user_name;
   stan_shift = shiftE.stan;

   if ( stan_shift != 1 ) {
      MessageDlg( String("Зміна не знаходиться в стані ~Завершена~"), mtError,
                                            TMsgDlgButtons() << mbOK, 0);
      return false;
   }
   AddInfo( zzvit_info, "", 0, 1 );
   AddInfo( String("Початок зміни : ")+date_beg_shift+"  "+time_beg_shift, "", 0, 1 );
   AddInfo( String("Завершення зміни : ")+date_end_shift+"  "+time_end_shift, "", 1, 2 );
   //
   vector <TERMINAL_TOVAR> tov;
   // 2.Читаємо реалізацію по кожному з типів покупців
   // - Зчитати агентів , що обслуговувалися по терміналу
   //
   vector <int> agentShift;
   TShiftSQL::LoadAgentShift( shift_ID, agentShift );
   // check_recreate - потрібен щоб перевірка на існування документів по даній зміні
   // проводилася лише при першому проходженні і не вилучала щойно створені накладні
   check_recreate = false;
   // - в циклі формуємо накладні по кожному з агентів
   for( int i=0;i<(int)agentShift.size();i++ ) {
      // В date_current заносимо початкову дату та перебираємо всі дні до кінцевої
      date_current = date_beg_shift;
      bool agent_multy_nakl = TShiftSQL::GetMultyNaklAgent(agentShift[i]);
      //
      while( StrToDate(date_current) <= StrToDate(date_end_shift) ) {
         AddInfo( String("Зчитування даних за ")+date_current+" Агент("+TDBService::get_from_ID("agents","name","agent_id",String(agentShift[i]))+") ...", "Зчитування даних", 0, 2 );
         if( !agent_multy_nakl ) { // Одна накладна на агента за дату
            // Реалізація
            if( TShiftSQL::ReadRealToVector( agentShift[i], tov, date_current, shift_ID, total_calculation, 0 ) ) {
               AddInfo( "OK!!!", "", 0, 2 );
               // якщо зчитані дані то формуємо накладну по реалізації в кредит
               AddInfo( String("ФОРМУВАННЯ ДОКУМЕНТУ ЗА ")+date_current, String("Формування документу на РЕАЛІЗАЦІЮ за ")+date_current, 1, 3 );
               // Перевіряємо перший раз чи накладна ще не створена
               String time_doc = "";
               if( check_recreate || RecreateNakl(shift_ID,time_doc) ) {
                  check_recreate = true; // перевірка вже пройшла
                  int doc_tov_id = 0;
                  if( (int) tov.size() ) {
                     if( StrToDate(date_current) == StrToDate(date_end_shift) )
                        time_doc = time_end_shift;
                     else
                        time_doc = "23:59:59";
                     doc_tov_id = TShiftSQL::CreateNakl( tov, date_current, time_doc, agent_id, agentShift[i], zzvit_info, shift_ID );
                     oplata_info = zzvit_info+String(" = ")+GetZakazCalculationInfo(total_calculation);
                     AddInfo( String("Документ від ")+date_current+" на РЕАЛІЗАЦІЮ сформований.", "", 0, 2 );
                  }
                  if( (int) total_calculation.size() )
                     TShiftSQL::CreateOplat(total_calculation, 3, date_current, agent_id, agentShift[i], shift_ID, doc_tov_id );
                  // Корегуємо призначення в документі
                  TShiftSQL::UpdatePryznDoc(doc_tov_id,oplata_info);
//                  TDBService::update_field_ID("document","pryzn",oplata_info,"doc_id",String(doc_tov_id));
               }
            }
            else
               AddInfo( "Дані про реалізацію відсутні!!!", "", 0, 2 );
            //
            // Повернення
            if( TShiftSQL::ReadRealToVector( agentShift[i], tov, date_current, shift_ID, total_calculation, 1 ) ) {
               AddInfo( "OK!!!", "", 0, 2 );
               // якщо зчитані дані то формуємо накладну по реалізації в кредит
               AddInfo( String("ФОРМУВАННЯ ДОКУМЕНТУ НА ПОВЕРНЕННЯ ЗА ")+date_current, String("Формування документу на ПОВЕРНЕННЯ за ")+date_current, 1, 3 );
               // Перевіряємо перший раз чи накладна ще не створена
               String time_doc = "";
               if( check_recreate || RecreateNakl(shift_ID,time_doc) ) {
                  check_recreate = true; // перевірка вже пройшла
                  int doc_tov_id = 0;
                  if( (int) tov.size() ) {
                     if( StrToDate(date_current) == StrToDate(date_end_shift) )
                        time_doc = time_end_shift;
                     else
                        time_doc = "23:59:59";
                     doc_tov_id = TShiftSQL::CreateNakl( tov, date_current, time_doc, agentShift[i], agent_id, zzvit_info, shift_ID );
                     oplata_info = zzvit_info+String(" = ")+GetZakazCalculationInfo(total_calculation);
                     AddInfo( String("Документ від ")+date_current+" на ПОВЕРНЕННЯ сформований.", "", 0, 2 );
                  }
                  if( (int) total_calculation.size() ) {
                     for( int i=0;i<(int)total_calculation.size();i++ ) total_calculation[i].suma = -total_calculation[i].suma;
                     //
                     TShiftSQL::CreateOplat(total_calculation, 3, date_current, agent_id, agentShift[i], shift_ID, doc_tov_id );
                  }
                  // Корегуємо призначення в документі
                  TShiftSQL::UpdatePryznDoc(doc_tov_id,oplata_info);
//                  TDBService::update_field_ID("document","pryzn",oplata_info,"doc_id",String(doc_tov_id));
               }
            }
            else
               AddInfo( "Дані про повернення відсутні!!!", "", 0, 2 );
         }
         else { // Кожен чек на агента за дату як окрема накладна
            vector <int> zakazID;
            zakazID.clear();
            // Читаємо перелік чеків за задане число по заданому агенту
            TShiftSQL::ReadNaklAgentToVector( agentShift[i], zakazID , date_current, shift_ID );
            for( int j=0; j<(int) zakazID.size();j++ ) {
               // Реалізація
               if( TShiftSQL::ReadRealToVector( agentShift[i], tov, date_current, shift_ID, total_calculation, 0, zakazID[j] ) ) {
                  AddInfo( "OK!!!", "", 0, 2 );
                  // якщо зчитані дані то формуємо накладну по реалізації в кредит
                  AddInfo( String("ФОРМУВАННЯ ДОКУМЕНТУ ЗА ")+date_current, String("Формування документу на РЕАЛІЗАЦІЮ за ")+date_current, 1, 3 );
                  // Перевіряємо перший раз чи накладна ще не створена
                  String time_doc = "";
                  if( check_recreate || RecreateNakl(shift_ID,time_doc) ) {
                     check_recreate = true; // перевірка вже пройшла
                     int doc_tov_id = 0;
                     if( (int) tov.size() ) {
                        if( StrToDate(date_current) == StrToDate(date_end_shift) )
                           time_doc = time_end_shift;
                        else
                           time_doc = "23:59:59";
                        doc_tov_id = TShiftSQL::CreateNakl( tov, date_current, time_doc, agent_id, agentShift[i],
                                                           (zzvit_info+String(" ")+IntToStr(zakazID[j])+" "+TDBService::get_from_ID("r_zakaz","coment","zakaz_id",String(zakazID[j]))).Trim(),
                                                            shift_ID );
                        oplata_info = (zzvit_info+String(" ")+IntToStr(zakazID[j])+" "+TDBService::get_from_ID("r_zakaz","coment","zakaz_id",String(zakazID[j]))).Trim()+String(" = ")+GetZakazCalculationInfo(total_calculation);
                        AddInfo( String("Документ від ")+date_current+" на РЕАЛІЗАЦІЮ сформований.", "", 0, 2 );
                     }
                     if( (int) total_calculation.size() )
                        TShiftSQL::CreateOplat(total_calculation, 3, date_current, agent_id, agentShift[i], shift_ID, doc_tov_id );
                     // Корегуємо призначення в документі
                     TShiftSQL::UpdatePryznDoc(doc_tov_id,oplata_info);
//                     TDBService::update_field_ID("document","pryzn",oplata_info,"doc_id",String(doc_tov_id));
                  }
               }
               else
                  AddInfo( "Дані про реалізацію відсутні!!!", "", 0, 2 );
               //
               // Повернення
               if( TShiftSQL::ReadRealToVector( agentShift[i], tov, date_current, shift_ID, total_calculation, 1, zakazID[j] ) ) {
                  AddInfo( "OK!!!", "", 0, 2 );
                  // якщо зчитані дані то формуємо накладну по реалізації в кредит
                  AddInfo( String("ФОРМУВАННЯ ДОКУМЕНТУ НА ПОВЕРНЕННЯ ЗА ")+date_current, String("Формування документу на ПОВЕРНЕННЯ за ")+date_current, 1, 3 );
                  // Перевіряємо перший раз чи накладна ще не створена
                  String time_doc = "";
                  if( check_recreate || RecreateNakl(shift_ID,time_doc) ) {
                     check_recreate = true; // перевірка вже пройшла
                     int doc_tov_id = 0;
                     if( (int) tov.size() ) {
                        if( StrToDate(date_current) == StrToDate(date_end_shift) )
                           time_doc = time_end_shift;
                        else
                           time_doc = "23:59:59";
                        doc_tov_id = TShiftSQL::CreateNakl( tov, date_current, time_doc, agentShift[i], agent_id, zzvit_info+String(" ")+IntToStr(zakazID[j]), shift_ID );
                        oplata_info = zzvit_info+String(" ")+IntToStr(zakazID[j])+String(" = ")+GetZakazCalculationInfo(total_calculation);
                        AddInfo( String("Документ від ")+date_current+" на ПОВЕРНЕННЯ сформований.", "", 0, 2 );
                     }
                     if( (int) total_calculation.size() ) {
                        for( int i=0;i<(int)total_calculation.size();i++ ) total_calculation[i].suma = -total_calculation[i].suma;
                        //
                        TShiftSQL::CreateOplat(total_calculation, 3, date_current, agent_id, agentShift[i], shift_ID, doc_tov_id );
                     }
                     // Корегуємо призначення в документі
                     TShiftSQL::UpdatePryznDoc(doc_tov_id,oplata_info);
//                     TDBService::update_field_ID("document","pryzn",oplata_info,"doc_id",String(doc_tov_id));
                  }
               }
               else
                  AddInfo( "Дані про повернення відсутні!!!", "", 0, 2 );
            }
         }
         //
         date_current = DateToStr(StrToDate(date_current)+1);
      }
   }
   // 6.Записуємо в історію по дисконтах всіпродажі по дисконтах по зміні
   TShiftSQL::InsertDiscountHistoryByShift( shift_ID );
   //
   AddInfo( "ДОКУМЕНТИ сформовані!!!", "", 1, 3 );
   // 7.Закриваємо зміну
   TShiftSQL::SetShiftStan(shift_ID,2);
   //
   tov.clear();
   //
   // 8.Формуємо оплати/витрати по зміні сл.внесення/сл.видача
   if( atoi(TDBService::get_from_ID("const_default","strvalue","name_const","SHIFT_CASH_CLOSE").c_str()) ) {
      TFShiftCashCreateOplataOrCost *FShiftCashCreateOplataOrCost = new TFShiftCashCreateOplataOrCost(shift_ID,NULL);
      FShiftCashCreateOplataOrCost->ShowModal();
      delete FShiftCashCreateOplataOrCost;
   }
   //
   AddInfo( "ЗМІНА ЗАКРИТА!!!", "", 1, 3 );
   //
   return true;
}
//---------------------------------------------------------------------------

String TFCloseShift::GetZakazCalculationInfo( vector <ZAKAZ_CALCULATION> total_calculation )
{
   String result = String("");
   for( int i=0; i<(int)total_calculation.size();i++ ) {
      result += total_calculation[i].type_calculation_name+": "+TFormatingData::DblToStr(total_calculation[i].suma)+(total_calculation[i].course!=0&&total_calculation[i].course!=1?String(" (")+total_calculation[i].amount_currency+" "+TDBService::get_from_ID("currency","short_name","currency_id",String(total_calculation[i].currency_ID))+"x"+total_calculation[i].course+") = ":String(" = "));
   }
   return result;
}
//---------------------------------------------------------------------------

bool TFCloseShift::RecreateNakl( int shift_id, String &time_doc )
{
   //
   // Перевірити чи по даному протоколу вже є сформовані документи
   //
   vector <SHIFT_DOCS> shiftDocList;
   TShiftSQL::GetShiftDocs( shift_id, shiftDocList );

   if ( shiftDocList.size() ) {
      if ( MessageDlg( String("По даній зміні документи вже сформовані!\nПереформувати документи - ?"), mtConfirmation,
                      TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo ) {
         return false;
      }
      else {
         Refresh();
         for(int i=0; i<(int)shiftDocList.size(); i++) {
            if ( shiftDocList[i].stan == 1 ) { // Анулювати документ
               time_doc = shiftDocList[i].time_d;
               AddInfo( String("Документ №")+shiftDocList[i].number_d+" вже проведено!", "", 0, 2 );
               AddInfo( String("Анульовуємо для переформування!"), "", 1, 2 );
               // Анулювати документ
               if ( !TSubMethods::IntoCurrentPeriod(shiftDocList[i].date_d) ) {
                  AddInfo( String("Дата документу відноситься до закритого періоду!!!"), "", 1, 2 );
                  continue;
               }
               String ErrMsg = "";
               if ( !TMaterialDoc::ProvDoc(false,shiftDocList[i].doc_tov_ID,NULL,false,ErrMsg) ) {
                  if ( ErrMsg != "" )
                     AddInfo( ErrMsg, "", 1, 2 );
                  continue;
               }
               //
               // Вилучаємо оплати по документу
               TShiftSQL::DeleteDocOplat( shiftDocList[i].doc_tov_ID );
               //
               // Вилучаємо звязки з оплатами (Анулювання)
               //
               TWorkDocTov::NakladnaKillingProcedure(shiftDocList[i].doc_tov_ID);
            }
            AddInfo( String("Вилучаємо попередньо створений документ №")+shiftDocList[i].number_d, "", 0, 2 );
            //
            TShiftSQL::DeleteShiftDoc( shift_id, shiftDocList[i].doc_tov_ID );
         }
         TShiftSQL::DeleteDiscountHistoryByShift( shift_id );
      }
   }
   return true;
}
//---------------------------------------------------------------------------

void TFCloseShift::AddInfo( String list_text, String label_text, int empty_row, int pause_sec )
{
   //
   if( label_text != "" ) {
      InfoLabel->Caption = label_text;
      Refresh();
   }
   if( list_text != "" ) {
      InfoListBox->Items->Add(list_text);
      InfoListBox->ItemIndex ++;
   }
   for(int i=0;i<empty_row;i++){
      InfoListBox->Items->Add("");
      InfoListBox->ItemIndex ++;
   }
   InfoListBox->Refresh();
   TimeOut(pause_sec);
}
//---------------------------------------------------------------------------

void TFCloseShift::TimeOut( int sec )
{
  time_t beg_t, end_t;
  beg_t = time(NULL);
  for( int i=0; end_t-beg_t < sec; ) {
    end_t = time(NULL);
    if( end_t-i-beg_t < 1 ) continue;
    i++;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFCloseShift::ProgressTimerTimer(TObject *Sender)
{
   StatusBar->Canvas->Brush->Color = clBtnFace;
   StatusBar->Canvas->FillRect(ProgressRect);
   StatusBarImageList->Draw(StatusBar->Canvas,LeftPos,5,NumImage,true);
   //
   if ( NumImage ) { // ?oo ae?ai
      if ( LeftPos > 2 ) LeftPos -= 1;
      else NumImage = 0;
   }
   else { // ?oo ai?aai
      if ( LeftPos < 390 ) LeftPos += 1;
      else NumImage = 1;
   }
}
//---------------------------------------------------------------------------

void TFCloseShift::StopTimer( void )
{
   StatusBar->Canvas->FillRect(ProgressRect);
   StatusBar->Canvas->Brush->Color = clBtnFace;
   ProgressTimer->Enabled = false;
}
//---------------------------------------------------------------------------

void TFCloseShift::StartTimer( String info )
{
   if ( ProgressTimer->Enabled )
      StopTimer();
   //
   LeftPos = 2;
   NumImage = 0;
//   TimerMsg = info;
   ProgressTimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFCloseShift::StatusBarDrawPanel(TStatusBar *StatusBar,
      TStatusPanel *Panel, const TRect &Rect)
{
   if ( Panel->Text == "Find" )
      ProgressRect = Rect;
}
//---------------------------------------------------------------------------

void __fastcall TFCloseShift::StartBtnClick(TObject *Sender)
{
   StartBtn->Enabled = false;
   CreateRealDocument();
}
//---------------------------------------------------------------------------


