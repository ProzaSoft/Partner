//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "StatusHistorySQL.h"
#include "FormatingData.h"
#include "DBService.h"
#include "UniQuery.h"
//
extern DConnect *pConnect;
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

void TStatusHistorySQL::LoadStatuses( vector <STATUSES> &statusList )
{
   if( !pConnect->IsConnecting )
      return;
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   statusList.clear();
   //
   try {
      TmpQuery->Close();
      TmpQuery->SQL->Clear();
      TmpQuery->SQL->Add("SELECT * FROM r_statuses ORDER BY name");
      TmpQuery->Open();
      while( !TmpQuery->Eof ) {
         STATUSES st;
            st.ID   = TmpQuery->FieldByName("status_id")->AsInteger;
            st.name = TmpQuery->FieldByName("name")->AsString;
            st.selected = false;
         statusList.push_back( st );
         TmpQuery->Next();
      }
      TmpQuery->Close();
   }
   catch (Exception &E) {
      MessageDlg( String("Помилка завантаження списку статусів!\n")+E.Message,
                                                             mtError, TMsgDlgButtons() << mbOK, 0);
   }
   delete TmpQuery;
}
//---------------------------------------------------------------------------

void TStatusHistorySQL::LoadActionsByTovarID( vector <STATUS_ACTION> &actionList, vector <STATUSES> &statusList, String BegDate, String EndDate, int tovar_id, int user_id, int dep_id )
{
   // user_id = employee_id логін що записаний в історії викручуємо з r_user_cash_register
   // dep_id - значення з таблиці department cash_register_id - тип точки продажу
   // Якщо doc_id!=0 перевіряємо ненульові значення відділу та точки продажу
   // для цього з zakaz зчитуємо shift а зі зміни значення відділу та точки продажу
   if( !pConnect->IsConnecting )
      return;
   actionList.clear();
   //
   if( !statusList.size() ) return;
   //
   String statuses_str = "";
   for( int i = 0; i < (int)statusList.size(); i++ ) {
      if( !statusList[i].selected ) continue;
      statuses_str += String(statusList[i].ID)+",";
   }
   if( !statuses_str.Length() ) return;
   statuses_str = statuses_str.SubString(1, statuses_str.Length()-1);
   //
   String user_login = TDBService::get_from_ID("r_user_cash_register","login","employee_id",user_id);
   map <String,String> user_map;
   GetUserNameIntoMap( user_map );
   //
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   try {
      TmpQuery->Close();
      TmpQuery->SQL->Clear();
      TmpQuery->SQL->Add( String("")+
         " SELECT s.name as name_st,sh.* "+
         " FROM r_status_history sh "+
         "    LEFT JOIN r_statuses s ON ( sh.status_id = s.status_id ) "+
         "    LEFT JOIN r_zakaz z    ON ( sh.doc_id = z.zakaz_id ) "+
         "    LEFT JOIN r_shift rs   ON ( z.shift_id = rs.shift_id ) "+
         " WHERE date_d>=:beg_date AND date_d<=:end_date AND "+
         "    IF( 0!=:tovar_id,tovar_id=:tovar_id,1) AND "+
         "    IF( 0!=:user_id,sh.user_name=:user_login,1) AND "+
         "    s.status_id IN ("+statuses_str+") AND "+
         "    ( doc_id=0 OR IF( 0!=:dep_id,rs.dep_id=:dep_id,1) ) "+
//         "      IF( 0!=:dep_id,rs.dep_id=:dep_id,1) AND "+
//         "      IF( 0!=:cash_register_id,rs.cash_register_id=:cash_register_id,1) ) "+
         " ORDER BY sh.status_history_id " );
      TmpQuery->ParamByName("beg_date")->AsString    = FIELD::ConvertDatePartnerSql(BegDate)+" 00:00:00";
      TmpQuery->ParamByName("end_date")->AsString    = FIELD::ConvertDatePartnerSql(EndDate)+" 23:59:59";
      TmpQuery->ParamByName("user_id")->AsInteger    = user_id;
      TmpQuery->ParamByName("user_login")->AsString  = user_login;
      TmpQuery->ParamByName("tovar_id")->AsInteger   = tovar_id;
      TmpQuery->ParamByName("dep_id")->AsInteger     = dep_id;
//      TmpQuery->ParamByName("cash_register_id")->AsInteger = cash_register_id;
      TmpQuery->Open();
      while( !TmpQuery->Eof ) {
         STATUS_ACTION ah;
            ah.status_history_ID = TmpQuery->FieldByName("status_history_id")->AsInteger;
            //
            ah.doc_ID    = TmpQuery->FieldByName("doc_id")->AsInteger;
            ah.tovar_ID  = TmpQuery->FieldByName("tovar_id")->AsInteger;
            ah.date_d    = TmpQuery->FieldByName("date_d")->AsString;
            ah.user_name = user_map[TmpQuery->FieldByName("user_name")->AsString];
            ah.comment   = RecodeComment(TmpQuery->FieldByName("name_st")->AsString,
                                         TmpQuery->FieldByName("param_1")->AsString,
                                         TmpQuery->FieldByName("param_2")->AsString,
                                         TmpQuery->FieldByName("param_3")->AsString,
                                         TmpQuery->FieldByName("param_4")->AsString,
                                         TmpQuery->FieldByName("param_5")->AsString );
            ah.quantity = ah.price = ah.suma = 0;
            switch( TmpQuery->FieldByName("status_id")->AsInteger ) {
               case 6: // Добавлення рядка
                  ah.quantity = TFormatingData::StrToDbl(TmpQuery->FieldByName("param_2")->AsString);
                  ah.suma     = TFormatingData::StrToDbl(TmpQuery->FieldByName("param_3")->AsString);
                  if( ah.quantity ) ah.price = ah.suma/ah.quantity;
                  break;
               case 7: // Редагування рядка
                  ah.quantity = TFormatingData::StrToDbl(TmpQuery->FieldByName("param_3")->AsString)-
                                TFormatingData::StrToDbl(TmpQuery->FieldByName("param_2")->AsString);
                  if( TFormatingData::StrToDbl(TmpQuery->FieldByName("param_3")->AsString) )
                     ah.price = TFormatingData::StrToDbl(TmpQuery->FieldByName("param_4")->AsString)/
                                TFormatingData::StrToDbl(TmpQuery->FieldByName("param_3")->AsString);
                  ah.suma = ah.quantity * ah.price;
                  break;
               case 8: // Вилучення рядка
                  ah.quantity = -TFormatingData::StrToDbl(TmpQuery->FieldByName("param_2")->AsString);
                  ah.suma     = -TFormatingData::StrToDbl(TmpQuery->FieldByName("param_3")->AsString);
                  if( ah.quantity ) ah.price = ah.suma/ah.quantity;
                  break;
               case 17: // Добавлення рядка в чек на Повернення
                  ah.quantity = -TFormatingData::StrToDbl(TmpQuery->FieldByName("param_2")->AsString);
                  ah.suma     = -TFormatingData::StrToDbl(TmpQuery->FieldByName("param_3")->AsString);
                  if( ah.quantity ) ah.price = ah.suma/ah.quantity;
                  break;
            }
            ah.selected = false;
         actionList.push_back( ah );
         TmpQuery->Next();
      }
      TmpQuery->Close();
   }
   catch (Exception &E) {
      MessageDlg( String("Помилка завантаження подій по товару!\n")+E.Message,
                                                             mtError, TMsgDlgButtons() << mbOK, 0);
   }
   delete TmpQuery;
}
//---------------------------------------------------------------------------

String TStatusHistorySQL::RecodeComment(String name, String p1, String p2, String p3, String p4, String p5)
{
   String new_name = "";
   for(int i=1; i<=name.Length(); i++ ) {
      if( name.SubString(i,1) == '#' ) {
         i++;
         switch( StrToInt(name.SubString(i,1)) ) {
          case 1: new_name += p1; break;
          case 2: new_name += p2; break;
          case 3: new_name += p3; break;
          case 4: new_name += p4; break;
          case 5: new_name += p5; break;
         }
      }
      else
        new_name += name.SubString(i,1);
   }
   return new_name;
}
//---------------------------------------------------------------------------

String TStatusHistorySQL::GetUserNameFromLogin( String user_name )
{
   String result = user_name;
   if( !pConnect->IsConnecting )
      return result;
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   try {
      TmpQuery->Close();
      TmpQuery->SQL->Clear();
      TmpQuery->SQL->Add("SELECT CONCAT(e.surname,' ',e.first_name,' ',e.second_name) as user_name FROM r_user_cash_register r LEFT JOIN employee e ON r.employee_id=e.employee_id WHERE r.login=:user_login LIMIT 1");
      TmpQuery->ParamByName("user_login")->AsString = user_name;
      TmpQuery->Open();
      if( !TmpQuery->Eof ) {
         result = TmpQuery->FieldByName("user_name")->AsString;
      }
      TmpQuery->Close();
   }
   catch (Exception &E) {
      MessageDlg( String("Помилка GetUserNameFromLogin!\n")+E.Message,
                                                             mtError, TMsgDlgButtons() << mbOK, 0);
   }
   delete TmpQuery;
   return result;
}
//---------------------------------------------------------------------------

void TStatusHistorySQL::GetUserNameIntoMap( map <String,String> &user_map )
{
   if( !pConnect->IsConnecting )
      return;
   DUniQuery *TmpQuery = new DUniQuery( pConnect );
   //
   user_map.clear();
   try {
      TmpQuery->Close();
      TmpQuery->SQL->Clear();
      TmpQuery->SQL->Add("SELECT DISTINCT login, CONCAT(e.surname,' ',e.first_name) as user_name FROM r_user_cash_register r LEFT JOIN employee e ON r.employee_id=e.employee_id");
      TmpQuery->Open();
      while( !TmpQuery->Eof ) {
         user_map[TmpQuery->FieldByName("login")->AsString] = TmpQuery->FieldByName("user_name")->AsString;
         TmpQuery->Next();
      }
      TmpQuery->Close();
   }
   catch (Exception &E) {
      MessageDlg( String("Помилка GetUserNameIntoMap!\n")+E.Message,
                                                            mtError, TMsgDlgButtons() << mbOK, 0);
   }
   delete TmpQuery;
}

