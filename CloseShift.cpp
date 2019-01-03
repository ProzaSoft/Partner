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
   //   ������� ����� � ����������� ����
   AddInfo( "���������� �����...", "���������� �����", 0, 2 );
   // �������� ����
   String query = String("");
   //
   //
   // 1. ������ ��� �� ���
   int agent_id; // ³��� �������
   int agent_to_id; // ��������
   String date_beg_shift, time_beg_shift;
   String date_end_shift, time_end_shift;
   String date_current;
   bool check_recreate;
   String zzvit_info; // ���������� ��� Z-���
   String oplata_info; // ���������� ��� ������ �� Z-����
   int stan_shift; // ���� ����. ��������� ��������� ��������� ���� ��� ����� 1
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
   zzvit_info = String("���� � ")+shiftE.shift_ID+
                   "; Z-�"+shiftE.zzvit_number+
                   " "+shiftE.user_name;
   stan_shift = shiftE.stan;

   if ( stan_shift != 1 ) {
      MessageDlg( String("���� �� ����������� � ���� ~���������~"), mtError,
                                            TMsgDlgButtons() << mbOK, 0);
      return false;
   }
   AddInfo( zzvit_info, "", 0, 1 );
   AddInfo( String("������� ���� : ")+date_beg_shift+"  "+time_beg_shift, "", 0, 1 );
   AddInfo( String("���������� ���� : ")+date_end_shift+"  "+time_end_shift, "", 1, 2 );
   //
   vector <TERMINAL_TOVAR> tov;
   // 2.������ ��������� �� ������� � ���� ��������
   // - ������� ������ , �� ��������������� �� ��������
   //
   vector <int> agentShift;
   TShiftSQL::LoadAgentShift( shift_ID, agentShift );
   // check_recreate - ������� ��� �������� �� ��������� ��������� �� ���� ���
   // ����������� ���� ��� ������� ���������� � �� �������� ����� ������� �������
   check_recreate = false;
   // - � ���� ������� ������� �� ������� � ������
   for( int i=0;i<(int)agentShift.size();i++ ) {
      // � date_current �������� ��������� ���� �� ���������� �� �� �� ������
      date_current = date_beg_shift;
      bool agent_multy_nakl = TShiftSQL::GetMultyNaklAgent(agentShift[i]);
      //
      while( StrToDate(date_current) <= StrToDate(date_end_shift) ) {
         AddInfo( String("���������� ����� �� ")+date_current+" �����("+TDBService::get_from_ID("agents","name","agent_id",String(agentShift[i]))+") ...", "���������� �����", 0, 2 );
         if( !agent_multy_nakl ) { // ���� �������� �� ������ �� ����
            // ���������
            if( TShiftSQL::ReadRealToVector( agentShift[i], tov, date_current, shift_ID, total_calculation, 0 ) ) {
               AddInfo( "OK!!!", "", 0, 2 );
               // ���� ������ ��� �� ������� �������� �� ��������� � ������
               AddInfo( String("���������� ��������� �� ")+date_current, String("���������� ��������� �� ���˲��ֲ� �� ")+date_current, 1, 3 );
               // ���������� ������ ��� �� �������� �� �� ��������
               String time_doc = "";
               if( check_recreate || RecreateNakl(shift_ID,time_doc) ) {
                  check_recreate = true; // �������� ��� �������
                  int doc_tov_id = 0;
                  if( (int) tov.size() ) {
                     if( StrToDate(date_current) == StrToDate(date_end_shift) )
                        time_doc = time_end_shift;
                     else
                        time_doc = "23:59:59";
                     doc_tov_id = TShiftSQL::CreateNakl( tov, date_current, time_doc, agent_id, agentShift[i], zzvit_info, shift_ID );
                     oplata_info = zzvit_info+String(" = ")+GetZakazCalculationInfo(total_calculation);
                     AddInfo( String("�������� �� ")+date_current+" �� ���˲��ֲ� �����������.", "", 0, 2 );
                  }
                  if( (int) total_calculation.size() )
                     TShiftSQL::CreateOplat(total_calculation, 3, date_current, agent_id, agentShift[i], shift_ID, doc_tov_id );
                  // �������� ����������� � ��������
                  TShiftSQL::UpdatePryznDoc(doc_tov_id,oplata_info);
//                  TDBService::update_field_ID("document","pryzn",oplata_info,"doc_id",String(doc_tov_id));
               }
            }
            else
               AddInfo( "��� ��� ��������� ������!!!", "", 0, 2 );
            //
            // ����������
            if( TShiftSQL::ReadRealToVector( agentShift[i], tov, date_current, shift_ID, total_calculation, 1 ) ) {
               AddInfo( "OK!!!", "", 0, 2 );
               // ���� ������ ��� �� ������� �������� �� ��������� � ������
               AddInfo( String("���������� ��������� �� ���������� �� ")+date_current, String("���������� ��������� �� ���������� �� ")+date_current, 1, 3 );
               // ���������� ������ ��� �� �������� �� �� ��������
               String time_doc = "";
               if( check_recreate || RecreateNakl(shift_ID,time_doc) ) {
                  check_recreate = true; // �������� ��� �������
                  int doc_tov_id = 0;
                  if( (int) tov.size() ) {
                     if( StrToDate(date_current) == StrToDate(date_end_shift) )
                        time_doc = time_end_shift;
                     else
                        time_doc = "23:59:59";
                     doc_tov_id = TShiftSQL::CreateNakl( tov, date_current, time_doc, agentShift[i], agent_id, zzvit_info, shift_ID );
                     oplata_info = zzvit_info+String(" = ")+GetZakazCalculationInfo(total_calculation);
                     AddInfo( String("�������� �� ")+date_current+" �� ���������� �����������.", "", 0, 2 );
                  }
                  if( (int) total_calculation.size() ) {
                     for( int i=0;i<(int)total_calculation.size();i++ ) total_calculation[i].suma = -total_calculation[i].suma;
                     //
                     TShiftSQL::CreateOplat(total_calculation, 3, date_current, agent_id, agentShift[i], shift_ID, doc_tov_id );
                  }
                  // �������� ����������� � ��������
                  TShiftSQL::UpdatePryznDoc(doc_tov_id,oplata_info);
//                  TDBService::update_field_ID("document","pryzn",oplata_info,"doc_id",String(doc_tov_id));
               }
            }
            else
               AddInfo( "��� ��� ���������� ������!!!", "", 0, 2 );
         }
         else { // ����� ��� �� ������ �� ���� �� ������ ��������
            vector <int> zakazID;
            zakazID.clear();
            // ������ ������ ���� �� ������ ����� �� �������� ������
            TShiftSQL::ReadNaklAgentToVector( agentShift[i], zakazID , date_current, shift_ID );
            for( int j=0; j<(int) zakazID.size();j++ ) {
               // ���������
               if( TShiftSQL::ReadRealToVector( agentShift[i], tov, date_current, shift_ID, total_calculation, 0, zakazID[j] ) ) {
                  AddInfo( "OK!!!", "", 0, 2 );
                  // ���� ������ ��� �� ������� �������� �� ��������� � ������
                  AddInfo( String("���������� ��������� �� ")+date_current, String("���������� ��������� �� ���˲��ֲ� �� ")+date_current, 1, 3 );
                  // ���������� ������ ��� �� �������� �� �� ��������
                  String time_doc = "";
                  if( check_recreate || RecreateNakl(shift_ID,time_doc) ) {
                     check_recreate = true; // �������� ��� �������
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
                        AddInfo( String("�������� �� ")+date_current+" �� ���˲��ֲ� �����������.", "", 0, 2 );
                     }
                     if( (int) total_calculation.size() )
                        TShiftSQL::CreateOplat(total_calculation, 3, date_current, agent_id, agentShift[i], shift_ID, doc_tov_id );
                     // �������� ����������� � ��������
                     TShiftSQL::UpdatePryznDoc(doc_tov_id,oplata_info);
//                     TDBService::update_field_ID("document","pryzn",oplata_info,"doc_id",String(doc_tov_id));
                  }
               }
               else
                  AddInfo( "��� ��� ��������� ������!!!", "", 0, 2 );
               //
               // ����������
               if( TShiftSQL::ReadRealToVector( agentShift[i], tov, date_current, shift_ID, total_calculation, 1, zakazID[j] ) ) {
                  AddInfo( "OK!!!", "", 0, 2 );
                  // ���� ������ ��� �� ������� �������� �� ��������� � ������
                  AddInfo( String("���������� ��������� �� ���������� �� ")+date_current, String("���������� ��������� �� ���������� �� ")+date_current, 1, 3 );
                  // ���������� ������ ��� �� �������� �� �� ��������
                  String time_doc = "";
                  if( check_recreate || RecreateNakl(shift_ID,time_doc) ) {
                     check_recreate = true; // �������� ��� �������
                     int doc_tov_id = 0;
                     if( (int) tov.size() ) {
                        if( StrToDate(date_current) == StrToDate(date_end_shift) )
                           time_doc = time_end_shift;
                        else
                           time_doc = "23:59:59";
                        doc_tov_id = TShiftSQL::CreateNakl( tov, date_current, time_doc, agentShift[i], agent_id, zzvit_info+String(" ")+IntToStr(zakazID[j]), shift_ID );
                        oplata_info = zzvit_info+String(" ")+IntToStr(zakazID[j])+String(" = ")+GetZakazCalculationInfo(total_calculation);
                        AddInfo( String("�������� �� ")+date_current+" �� ���������� �����������.", "", 0, 2 );
                     }
                     if( (int) total_calculation.size() ) {
                        for( int i=0;i<(int)total_calculation.size();i++ ) total_calculation[i].suma = -total_calculation[i].suma;
                        //
                        TShiftSQL::CreateOplat(total_calculation, 3, date_current, agent_id, agentShift[i], shift_ID, doc_tov_id );
                     }
                     // �������� ����������� � ��������
                     TShiftSQL::UpdatePryznDoc(doc_tov_id,oplata_info);
//                     TDBService::update_field_ID("document","pryzn",oplata_info,"doc_id",String(doc_tov_id));
                  }
               }
               else
                  AddInfo( "��� ��� ���������� ������!!!", "", 0, 2 );
            }
         }
         //
         date_current = DateToStr(StrToDate(date_current)+1);
      }
   }
   // 6.�������� � ������ �� ��������� �������� �� ��������� �� ���
   TShiftSQL::InsertDiscountHistoryByShift( shift_ID );
   //
   AddInfo( "��������� ���������!!!", "", 1, 3 );
   // 7.��������� ����
   TShiftSQL::SetShiftStan(shift_ID,2);
   //
   tov.clear();
   //
   // 8.������� ������/������� �� ��� ��.��������/��.������
   if( atoi(TDBService::get_from_ID("const_default","strvalue","name_const","SHIFT_CASH_CLOSE").c_str()) ) {
      TFShiftCashCreateOplataOrCost *FShiftCashCreateOplataOrCost = new TFShiftCashCreateOplataOrCost(shift_ID,NULL);
      FShiftCashCreateOplataOrCost->ShowModal();
      delete FShiftCashCreateOplataOrCost;
   }
   //
   AddInfo( "�̲�� �������!!!", "", 1, 3 );
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
   // ��������� �� �� ������ ��������� ��� � ��������� ���������
   //
   vector <SHIFT_DOCS> shiftDocList;
   TShiftSQL::GetShiftDocs( shift_id, shiftDocList );

   if ( shiftDocList.size() ) {
      if ( MessageDlg( String("�� ���� ��� ��������� ��� ���������!\n������������� ��������� - ?"), mtConfirmation,
                      TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo ) {
         return false;
      }
      else {
         Refresh();
         for(int i=0; i<(int)shiftDocList.size(); i++) {
            if ( shiftDocList[i].stan == 1 ) { // ��������� ��������
               time_doc = shiftDocList[i].time_d;
               AddInfo( String("�������� �")+shiftDocList[i].number_d+" ��� ���������!", "", 0, 2 );
               AddInfo( String("���������� ��� ��������������!"), "", 1, 2 );
               // ��������� ��������
               if ( !TSubMethods::IntoCurrentPeriod(shiftDocList[i].date_d) ) {
                  AddInfo( String("���� ��������� ���������� �� ��������� ������!!!"), "", 1, 2 );
                  continue;
               }
               String ErrMsg = "";
               if ( !TMaterialDoc::ProvDoc(false,shiftDocList[i].doc_tov_ID,NULL,false,ErrMsg) ) {
                  if ( ErrMsg != "" )
                     AddInfo( ErrMsg, "", 1, 2 );
                  continue;
               }
               //
               // �������� ������ �� ���������
               TShiftSQL::DeleteDocOplat( shiftDocList[i].doc_tov_ID );
               //
               // �������� ������ � �������� (����������)
               //
               TWorkDocTov::NakladnaKillingProcedure(shiftDocList[i].doc_tov_ID);
            }
            AddInfo( String("�������� ���������� ��������� �������� �")+shiftDocList[i].number_d, "", 0, 2 );
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


