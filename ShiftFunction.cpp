//---------------------------------------------------------------------------

#include <vcl.h>
//
#include "ShiftFunction.h"
#include "FormatingData.h"
#include "UniQuery.h"
//
extern DConnect *pConnect;
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

TShiftFunction::TShiftFunction()
{
}
//---------------------------------------------------------------------------

int TShiftFunction::GetCurrentShiftID( int cash_register_id, int user_id, int agent_id, bool create_new )
{
   if( !pConnect->IsConnecting )
      return 0;
   //
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   int result = 0;
   // Шукаємо вдкриту зміну по касовому апарату
   TmpQuery->Close();
   TmpQuery->SQL->Clear();
   TmpQuery->SQL->Add("SELECT shift_id FROM shift WHERE cash_register_id=:cash_register_id AND stan = 0");
   TmpQuery->ParamByName("cash_register_id")->AsInteger = cash_register_id;
   TmpQuery->Open();
   if( !TmpQuery->Eof )
      result = TmpQuery->FieldByName("shift_id")->AsInteger;
   delete TmpQuery;
   // Якщо немає відкритої зміни то створюємо її якщо create_new == true
   if( !result && create_new )
      result = CreateNewShift( cash_register_id, user_id, agent_id );
   //
   return result;
}
//---------------------------------------------------------------------------

int TShiftFunction::CreateNewShift( int cash_register_id, int user_id, int agent_id )
{
   if( !pConnect->IsConnecting )
      return 0;
   //
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   int result = 0;
   // Створюємо нову зміну
   TmpQuery->Close();
   TmpQuery->SQL->Clear();
   TmpQuery->SQL->Add( String("")+
      " INSERT INTO shift ( cash_register_id, user_id, agent_id, datetime_beg ) "+
      "            VALUES (:cash_register_id,:user_id,:agent_id,:datetime_beg ) ");
   TmpQuery->ParamByName("cash_register_id")->AsInteger = cash_register_id;
   TmpQuery->ParamByName("user_id")->AsInteger          = user_id;
   TmpQuery->ParamByName("agent_id")->AsInteger         = agent_id;
//   TmpQuery->ParamByName("datetime_beg")->AsDateTime    = Now();
   TmpQuery->ParamByName("datetime_beg")->AsString      = FIELD::ConvertDatePartnerSql(DateToStr(Now()))+" "+TimeToStr(Now());
   TmpQuery->ExecSQL();
   // Зчитуємо новий ID
   TmpQuery->Close();
   TmpQuery->SQL->Clear();
   TmpQuery->SQL->Add("SELECT LAST_INSERT_ID() as max_num");
   TmpQuery->Open();
   if( !TmpQuery->Eof )
      result = TmpQuery->FieldByName("max_num")->AsInteger;
   delete TmpQuery;
   // Якщо немає відкритої зміни то помилка
   if( !result )
      MessageDlg( String("Помилка добавлення нової зміни!"), mtError,
                                            TMsgDlgButtons() << mbOK, 0);
   //
   return result;
}
//---------------------------------------------------------------------------

void TShiftFunction::InputCash( int shift_id, double suma )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   // Створюємо новий рядок по готівці по даній зміні
   TmpQuery->Close();
   TmpQuery->SQL->Clear();
   TmpQuery->SQL->Add( String("")+
      " INSERT INTO cash ( shift_id, suma, time_input ) "+
      "           VALUES (:shift_id,:suma,:time_input ) ");
   TmpQuery->ParamByName("shift_id")->AsInteger = shift_id;
   TmpQuery->ParamByName("suma")->AsFloat       = suma;
   TmpQuery->ParamByName("time_input")->AsString  = TimeToStr(Time());
   TmpQuery->ExecSQL();
   //
   delete TmpQuery;
}
//---------------------------------------------------------------------------

void TShiftFunction::ZZvitShift( int shift_id, int user_id, int zzvit_number )
{
   // ZZvit по зміні -
   // 1. записуємо дату та час закінчення зміни
   // 2. перезаписуємо користувача оскільки відповідальний буде той хто
   //    робить ZZvit
   // 3. стан встановлюємо в 1 - завершена
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   TmpQuery->Close();
   TmpQuery->SQL->Clear();
   TmpQuery->SQL->Add( String("")+
      " UPDATE shift "+
      "    SET user_id      = :user_id, "+
      "        datetime_end = :datetime_end, "+
      "        zzvit_number = :zzvit_number, "+
      "        stan         = 1 "+
      " WHERE shift_id = :shift_id ");
   TmpQuery->ParamByName("shift_id")->AsInteger      = shift_id;
   TmpQuery->ParamByName("user_id")->AsInteger       = user_id;
   TmpQuery->ParamByName("zzvit_number")->AsInteger  = zzvit_number;
//   TmpQuery->ParamByName("datetime_end")->AsDateTime = Now();
   TmpQuery->ParamByName("datetime_end")->AsString = FIELD::ConvertDatePartnerSql(DateToStr(Now()))+" "+TimeToStr(Now());
   TmpQuery->ExecSQL();
   //
   delete TmpQuery;
}
//---------------------------------------------------------------------------

void TShiftFunction::CloseShift( int shift_id )
{
   // Закриття зміни
   // 1. стан встановлюємо в 2 - закрита
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   TmpQuery->Close();
   TmpQuery->SQL->Clear();
   TmpQuery->SQL->Add( String("")+
      " UPDATE shift "+
      "    SET stan = 2 "+
      " WHERE shift_id = :shift_id ");
   TmpQuery->ParamByName("shift_id")->AsInteger     = shift_id;
   TmpQuery->ExecSQL();
   //
   delete TmpQuery;
}
//---------------------------------------------------------------------------

int TShiftFunction::GetCashRegisterID( String znumber )
{
   if( !pConnect->IsConnecting )
      return 0;
   //
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   int result = 0;
   // Шукаємо касовий апарат по заводському номеру
   TmpQuery->Close();
   TmpQuery->SQL->Clear();
   TmpQuery->SQL->Add(" SELECT cash_register_id FROM cash_register WHERE znumber=:znumber ");
   TmpQuery->ParamByName("znumber")->AsString = znumber;
   TmpQuery->Open();
   if( !TmpQuery->Eof )
      result = TmpQuery->FieldByName("cash_register_id")->AsInteger;
   delete TmpQuery;
   // Якщо немає знайденого касового апарата
   if( !result )
      MessageDlg( String("Касовий апарат з номером ")+znumber+" не знайдено в довіднику!", mtError,
                                            TMsgDlgButtons() << mbOK, 0);
   //
   return result;
}
//---------------------------------------------------------------------------

void TShiftFunction::CreateNakl( int shift_id )
{
/*   // Формування накладних по зміні
   TQuery *TmpQuery = new TQuery(Application);
   TmpQuery->DatabaseName = "GeneralDB";
   //
   // 1.
   int result = 0;
   // Шукаємо касовий апарат по заводському номеру
   TmpQuery->Close();
   TmpQuery->SQL->Clear();
   TmpQuery->SQL->Add(" SELECT cash_register_id FROM cash_register WHERE znumber=:znumber ");
   TmpQuery->ParamByName("znumber")->AsString = znumber;
   TmpQuery->Open();
   if( !TmpQuery->Eof )
      result = TmpQuery->FieldByName("cash_register_id")->AsInteger;
   delete TmpQuery;
   // Якщо немає знайденого касового апарата
   if( !result )
      MessageDlg( String("Касовий апарат з номером ")+znumber+" не знайдено в довіднику!", mtError,
                                            TMsgDlgButtons() << mbOK, 0);
   //
   return result;
*/   
}
//---------------------------------------------------------------------------
