//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "ShiftSQL.h"
//
#include "FormatingData.h"
#include "WorkFiles.h"
#include "UniQuery.h"
#include "SubMethods.h"
#include "DBService.h"
#include "MaterialDoc.h"
#include "WorkDocTov.h"
#include "Group.h"
#include "Common_struct.h"
#include "DSklad.h"
//
extern DConnect *pConnect;

extern String LOGIN;

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

TShiftSQL::TShiftSQL()
{
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadShiftList( vector <SHIFT> &shiftList, int stan, int dep_id, String BegDate, String EndDate )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rs.*, a.agent_id, a.name as agent_name, ae.name as user_name, tcr.name as cash_register_name "+
         " FROM r_shift rs "+
         "    LEFT JOIN department d ON rs.dep_id = d.dep_id "+
         "    LEFT JOIN agents a     ON d.agent_id = a.agent_id "+
         "    LEFT JOIN employee e   ON rs.employee_id  = e.employee_id "+
         "    LEFT JOIN agents ae    ON e.agent_id = ae.agent_id "+
         "    LEFT JOIN r_cash_register cr ON rs.cash_register_id = cr.cash_register_id "+
         "    LEFT JOIN r_type_cash_register tcr ON cr.type_cash_register_id = tcr.type_cash_register_id "+
         " WHERE  if((:stan)>=0,rs.stan=:stan,1) AND if((:dep_id)>0,rs.dep_id=:dep_id,1) "+
         "    AND (Date(datetime_beg)>='"+FIELD::ConvertDatePartnerSql(BegDate)+"'"+
         "    AND Date(datetime_beg)<='"+FIELD::ConvertDatePartnerSql(EndDate)+"'"
         "    OR Date(datetime_end)>='"+FIELD::ConvertDatePartnerSql(BegDate)+"'"+
         "    AND Date(datetime_end)<='"+FIELD::ConvertDatePartnerSql(EndDate)+"')"+
         " ORDER BY shift_id" );
      //
      Query_tmp->ParamByName("stan")->AsInteger   = stan;
      Query_tmp->ParamByName("dep_id")->AsInteger = dep_id;
      Query_tmp->Open();
      shiftList.clear();
      SHIFT shiftE;
      while( !Query_tmp->Eof ){
         ClearShift(shiftE);
         //
         shiftE.shift_ID	   = Query_tmp->FieldByName("shift_id")->AsInteger;
         shiftE.cash_register_ID   = Query_tmp->FieldByName("cash_register_id")->AsInteger;
         shiftE.cash_register_name = Query_tmp->FieldByName("cash_register_name")->AsString;
         shiftE.dep_ID             = Query_tmp->FieldByName("dep_id")->AsInteger;
         shiftE.agent_ID           = Query_tmp->FieldByName("agent_id")->AsInteger;
         shiftE.agent_name         = Query_tmp->FieldByName("agent_name")->AsString;
         shiftE.employee_ID        = Query_tmp->FieldByName("employee_id")->AsInteger;
         shiftE.user_name          = Query_tmp->FieldByName("user_name")->AsString;
         shiftE.zzvit_number       = Query_tmp->FieldByName("zzvit_number")->AsInteger;
         shiftE.datetime_beg       = Query_tmp->FieldByName("datetime_beg")->AsString;
         shiftE.datetime_end       = Query_tmp->FieldByName("datetime_end")->AsString;
         shiftE.stan               = Query_tmp->FieldByName("stan")->AsInteger;
         //
         shiftList.push_back(shiftE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------


void TShiftSQL::LoadShiftList( vector <SHIFT> &shiftList, int stan )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rs.*, a.agent_id, a.name as agent_name, ae.name as user_name, tcr.name as cash_register_name "+
         " FROM r_shift rs "+
         "    LEFT JOIN department d ON rs.dep_id = d.dep_id "+
         "    LEFT JOIN agents a     ON d.agent_id = a.agent_id "+
         "    LEFT JOIN employee e   ON rs.employee_id  = e.employee_id "+
         "    LEFT JOIN agents ae    ON e.agent_id = ae.agent_id "+
         "    LEFT JOIN r_cash_register cr ON rs.cash_register_id = cr.cash_register_id "+
         "    LEFT JOIN r_type_cash_register tcr ON cr.type_cash_register_id = tcr.type_cash_register_id "+
         " WHERE  if((:stan)>=0,rs.stan=:stan,1) ORDER BY shift_id" );
      //
      Query_tmp->ParamByName("stan")->AsInteger = stan;
      Query_tmp->Open();
      shiftList.clear();
      SHIFT shiftE;
      while( !Query_tmp->Eof ){
         ClearShift(shiftE);
         //
         shiftE.shift_ID	   = Query_tmp->FieldByName("shift_id")->AsInteger;
         shiftE.cash_register_ID   = Query_tmp->FieldByName("cash_register_id")->AsInteger;
         shiftE.cash_register_name = Query_tmp->FieldByName("cash_register_name")->AsString;
         shiftE.dep_ID             = Query_tmp->FieldByName("dep_id")->AsInteger;
         shiftE.agent_ID           = Query_tmp->FieldByName("agent_id")->AsInteger;
         shiftE.agent_name         = Query_tmp->FieldByName("agent_name")->AsString;
         shiftE.employee_ID        = Query_tmp->FieldByName("employee_id")->AsInteger;
         shiftE.user_name          = Query_tmp->FieldByName("user_name")->AsString;
         shiftE.zzvit_number       = Query_tmp->FieldByName("zzvit_number")->AsInteger;
         shiftE.datetime_beg       = Query_tmp->FieldByName("datetime_beg")->AsString;
         shiftE.datetime_end       = Query_tmp->FieldByName("datetime_end")->AsString;
         shiftE.stan               = Query_tmp->FieldByName("stan")->AsInteger;
         //
         shiftList.push_back(shiftE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadShift( SHIFT &shiftE, int shift_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rs.*, a.agent_id, a.name as agent_name, ae.name as user_name, tcr.name as cash_register_name "+
         " FROM r_shift rs "+
         "    LEFT JOIN department d ON rs.dep_id = d.dep_id "+
         "    LEFT JOIN agents a     ON d.agent_id = a.agent_id "+
         "    LEFT JOIN employee e   ON rs.employee_id  = e.employee_id "+
         "    LEFT JOIN agents ae    ON e.agent_id = ae.agent_id "+
         "    LEFT JOIN r_cash_register cr ON rs.cash_register_id = cr.cash_register_id "+
         "    LEFT JOIN r_type_cash_register tcr ON cr.type_cash_register_id = tcr.type_cash_register_id "+
         " WHERE  rs.shift_id=:shift_id " );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      ClearShift(shiftE);
      if( !Query_tmp->Eof ){
         //
         shiftE.shift_ID	   = Query_tmp->FieldByName("shift_id")->AsInteger;
         shiftE.cash_register_ID   = Query_tmp->FieldByName("cash_register_id")->AsInteger;
         shiftE.cash_register_name = Query_tmp->FieldByName("cash_register_name")->AsString;
         shiftE.dep_ID             = Query_tmp->FieldByName("dep_id")->AsInteger;
         shiftE.agent_ID           = Query_tmp->FieldByName("agent_id")->AsInteger;
         shiftE.agent_name         = Query_tmp->FieldByName("agent_name")->AsString;
         shiftE.employee_ID        = Query_tmp->FieldByName("employee_id")->AsInteger;
         shiftE.user_name          = Query_tmp->FieldByName("user_name")->AsString;
         shiftE.zzvit_number       = Query_tmp->FieldByName("zzvit_number")->AsInteger;
         shiftE.datetime_beg       = Query_tmp->FieldByName("datetime_beg")->AsString.SubString(9,2)+"."+
                                     Query_tmp->FieldByName("datetime_beg")->AsString.SubString(6,2)+"."+
                                     Query_tmp->FieldByName("datetime_beg")->AsString.SubString(1,4)+" "+
                                     TimeToStr(StrToTime(Query_tmp->FieldByName("datetime_beg")->AsString.SubString(11,8)));
         if ( Query_tmp->FieldByName("datetime_end")->AsString != "" ) {
            shiftE.datetime_end       = Query_tmp->FieldByName("datetime_end")->AsString.SubString(9,2)+"."+
                                        Query_tmp->FieldByName("datetime_end")->AsString.SubString(6,2)+"."+
                                        Query_tmp->FieldByName("datetime_end")->AsString.SubString(1,4)+" "+
                                        TimeToStr(StrToTime(Query_tmp->FieldByName("datetime_end")->AsString.SubString(11,8)));
         }
         else
            shiftE.datetime_end = "";
         shiftE.stan               = Query_tmp->FieldByName("stan")->AsInteger;
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShift()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::ClearShift( SHIFT &shiftE )
{
   shiftE.shift_ID           = 0;
   shiftE.cash_register_ID   = 0;
   shiftE.cash_register_name = "";
   shiftE.dep_ID             = 0;
   shiftE.agent_ID           = 0;
   shiftE.agent_name         = "";
   shiftE.employee_ID        = 0;
   shiftE.user_name          = "";
   shiftE.zzvit_number       = 0;
   shiftE.datetime_beg       = "";
   shiftE.datetime_end       = "";
   shiftE.stan               = 0;
}
//---------------------------------------------------------------------------

void TShiftSQL::DeleteShift( int shift_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *dbQueryOrders = new DUniQuery( pConnect );
   DUniQuery *dbQueryDelete = new DUniQuery( pConnect );
   //
   // 2.Вибір Замовлень
   try {
     dbQueryOrders->Close();
     dbQueryOrders->SQL->Clear();
     dbQueryOrders->SQL->Add( String("")+ "SELECT zakaz_id FROM r_zakaz WHERE shift_id = :shift_id " );
     dbQueryOrders->ParamByName("shift_id")->AsInteger = shift_id;
     if( dbQueryOrders->Open() ) {
        while (!dbQueryOrders->Eof) {
           // 3. Рядки
           dbQueryDelete->Close();
           dbQueryDelete->SQL->Clear();
           dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_zakaz_row WHERE zakaz_id = :zakaz_id ");
           dbQueryDelete->ParamByName("zakaz_id")->AsInteger = dbQueryOrders->FieldByName("zakaz_id")->AsInteger;
           if( !dbQueryDelete->ExecSQL() ) {
              MessageDlg( String("Помилка видалення рядків!"), mtError,
                                               TMsgDlgButtons() << mbOK, 0);
              return;
           }
           //
           // 4. Оплати
           dbQueryDelete->Close();
           dbQueryDelete->SQL->Clear();
           dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_zakaz_calculation WHERE zakaz_id = :zakaz_id ");
           dbQueryDelete->ParamByName("zakaz_id")->AsInteger = dbQueryOrders->FieldByName("zakaz_id")->AsInteger;
           if ( !dbQueryDelete->ExecSQL() ) {
              MessageDlg( String("Помилка видалення оплат!"), mtError,
                                               TMsgDlgButtons() << mbOK, 0);
              return;
           }
           //
           // 5. Історія
           dbQueryDelete->Close();
           dbQueryDelete->SQL->Clear();
           dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_status_history WHERE doc_id = :zakaz_id ");
           dbQueryDelete->ParamByName("zakaz_id")->AsInteger = dbQueryOrders->FieldByName("zakaz_id")->AsInteger;
           if ( !dbQueryDelete->ExecSQL() ) {
              MessageDlg( String("Помилка видалення історії"), mtError,
                                               TMsgDlgButtons() << mbOK, 0);
              return;
           }
           //
           dbQueryOrders->Next();
        }
     }
     // 6. Замовлення
     dbQueryDelete->Close();
     dbQueryDelete->SQL->Clear();
     dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_zakaz WHERE shift_id = :shift_id ");
     dbQueryDelete->ParamByName("shift_id")->AsInteger = shift_id;
     if( !dbQueryDelete->ExecSQL() ) {
        MessageDlg( String("Помилка видалення замовлення"), mtError,
                                               TMsgDlgButtons() << mbOK, 0);
        return;
     }
     //
     // 7. Службові операціїї
     dbQueryDelete->Close();
     dbQueryDelete->SQL->Clear();
     dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_cash WHERE shift_id = :shift_id ");
     dbQueryDelete->ParamByName("shift_id")->AsInteger = shift_id;
     if (!dbQueryDelete->ExecSQL())
     {
        MessageDlg( String("Помилка видалення службових операцій"), mtError,
                                               TMsgDlgButtons() << mbOK, 0);
        return;
     }
     //
     // 8. Зміну
     dbQueryDelete->Close();
     dbQueryDelete->SQL->Clear();
     dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_shift WHERE shift_id = :shift_id ");
     dbQueryDelete->ParamByName("shift_id")->AsInteger = shift_id;
     if (!dbQueryDelete->ExecSQL())
     {
        MessageDlg( String("Помилка видалення зміни"), mtError,
                                               TMsgDlgButtons() << mbOK, 0);
        return;
     }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка вилучення зміни DeleteShift()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   //
   dbQueryDelete->Close();
   dbQueryOrders->Close();
   //
   delete dbQueryDelete;
   delete dbQueryOrders;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadShiftCash( int shift_id, double &cash_in, double &cash_out )
{
   if( !pConnect->IsConnecting )
      return;
   //
   cash_in = cash_out = 0;
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT SUM(if(suma>0,suma,0)) as cash_in, SUM(if(suma<0,suma,0)) as cash_out "+
         " FROM r_cash "+
         " WHERE shift_id=:shift_id GROUP BY shift_id" );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      if( !Query_tmp->Eof ) {
         cash_in	= Query_tmp->FieldByName("cash_in")->AsFloat;
         cash_out	= Query_tmp->FieldByName("cash_out")->AsFloat;
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftCash()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadShiftBonus( int shift_id, double &bonus_plus, double &bonus_minus )
{
   if( !pConnect->IsConnecting )
      return;
   //
   bonus_plus = bonus_minus = 0;
   // Читаємо налаштування
   if( !atoi(TDBService::get_from_ID("const_default","strvalue","name_const","BONUS_ON").c_str()) ) return;
   int bonus_type_calculation_id = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","BONUS_TYPE_CALC").c_str());
   if( !bonus_type_calculation_id ) return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT SUM(if(zc.suma>0,zc.suma,0)) as bonus_plus, SUM(if(zc.suma<0,zc.suma,0)) as bonus_minus "+
         " FROM r_zakaz_calculation zc "+
         "    LEFT JOIN r_zakaz z ON zc.zakaz_id=z.zakaz_id "+
         " WHERE z.shift_id=:shift_id AND zc.type_calculation_id=:bonus_type_calculation_id GROUP BY shift_id" );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->ParamByName("bonus_type_calculation_id")->AsInteger = bonus_type_calculation_id;
      Query_tmp->Open();
      if( !Query_tmp->Eof ) {
         bonus_plus	= Query_tmp->FieldByName("bonus_plus")->AsFloat;
         bonus_minus	= Query_tmp->FieldByName("bonus_minus")->AsFloat;
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftBonus()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadShiftCashList( int shift_id, vector <SHIFT_CASH> &cashList )
{
   if( !pConnect->IsConnecting )
      return;
   //
   cashList.clear();
   SHIFT_CASH eSH;
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT cash_id, time_input, if(suma>0,suma,0) as cash_in, if(suma<0,-suma,0) as cash_out, coment, cost_id, oplata_id, cash_type "+
         " FROM r_cash "+
         " WHERE shift_id=:shift_id ORDER BY time_input" );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      while( !Query_tmp->Eof ) {
         eSH.cash_id	     = Query_tmp->FieldByName("cash_id")->AsInteger;
         eSH.date_time_input = Query_tmp->FieldByName("time_input")->AsString;
         eSH.cash_in	     = Query_tmp->FieldByName("cash_in")->AsFloat;
         eSH.cash_out  	     = Query_tmp->FieldByName("cash_out")->AsFloat;
         eSH.coment          = Query_tmp->FieldByName("coment")->AsString;
         eSH.cash_type       = Query_tmp->FieldByName("cash_type")->AsInteger;
         eSH.cost_id         = Query_tmp->FieldByName("cost_id")->AsInteger;
         eSH.oplata_id       = Query_tmp->FieldByName("oplata_id")->AsInteger;
         eSH.doc_info        = "";
         if( eSH.cost_id )   eSH.doc_info = LoadDocInfoFromCost( eSH.cost_id );
         if( eSH.oplata_id ) eSH.doc_info = LoadDocInfoFromOplata( eSH.oplata_id );
         cashList.push_back(eSH);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftCashList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

String TShiftSQL::LoadDocInfoFromCost( int cost_id )
{
   String result = String("");
   //
   if( !pConnect->IsConnecting )
      return result;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT c.number_d, c.date_d, tc.name as type_cost_name, p.name as point_of_money_name, a.name as dep_name, c.info "+
         " FROM cost c "+
         "    LEFT JOIN type_cost tc     ON c.type_cost_id = tc.type_cost_id "+
         "    LEFT JOIN point_of_money p ON c.point_of_money_id = p.point_of_money_id "+
         "    LEFT JOIN department d     ON c.department_id = d.dep_id "+
         "    LEFT JOIN agents a         ON d.agent_id = a.agent_id "+
         " WHERE c.cost_id=:cost_id " );
      //
      Query_tmp->ParamByName("cost_id")->AsInteger = cost_id;
      Query_tmp->Open();
      if( !Query_tmp->Eof ) {
         result = String("Витрата № ")+Query_tmp->FieldByName("number_d")->AsString+
                  " від "+Query_tmp->FieldByName("date_d")->AsString+
                  " "+Query_tmp->FieldByName("type_cost_name")->AsString+
                  " ( "+Query_tmp->FieldByName("info")->AsString+")"+
                  " Департамент: "+Query_tmp->FieldByName("dep_name")->AsString+
                  " Джерело коштів: "+Query_tmp->FieldByName("point_of_money_name")->AsString;
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadDocInfoFromCost()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   return result;
}
//---------------------------------------------------------------------------

String TShiftSQL::LoadDocInfoFromOplata( int oplata_id )
{
   String result = String("");
   //
   if( !pConnect->IsConnecting )
      return result;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT o.number_d, o.date_d, o.type_opl, p_from.name as from_name, if(o.type_opl=1,a_to.name,p_to.name) as to_name, o.info "+
         " FROM oplata o "+
         "    LEFT JOIN agents a_from         ON o.agent_id = a_from.agent_id "+
         "    LEFT JOIN point_of_money p_from ON o.agent_id = p_from.point_of_money_id "+
         "    LEFT JOIN agents a_to           ON o.agent_to_id = a_from.agent_id "+
         "    LEFT JOIN point_of_money p_to   ON o.agent_to_id = p_from.point_of_money_id "+
         " WHERE oplata_id=:oplata_id " );
      //
      Query_tmp->ParamByName("oplata_id")->AsInteger = oplata_id;
      Query_tmp->Open();
      if( !Query_tmp->Eof ) {
         if( Query_tmp->FieldByName("type_opl")->AsInteger == 1 )
            result = String("Оплата постачальнику");
         else
            result = String("Вн.переміщення");
         result += String(" № ")+ Query_tmp->FieldByName("number_d")->AsString+
                  " від "+Query_tmp->FieldByName("date_d")->AsString+
                  " ( "+Query_tmp->FieldByName("info")->AsString+")"+
                  " "+Query_tmp->FieldByName("from_name")->AsString+
                  " --> "+Query_tmp->FieldByName("to_name")->AsString;
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadDocInfoFromOplata()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   return result;
}
//---------------------------------------------------------------------------

void TShiftSQL::SaveCashTypeInShiftCash( int cash_id, int cash_type )
{
   if( !pConnect->IsConnecting )
      return;
   //
   if( !cash_id ) return; // типи оплат можна лише редагувати

   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(" UPDATE r_cash SET cash_type=:cash_type WHERE cash_id=:cash_id" );
      Query_tmp->ParamByName("cash_type")->AsInteger = cash_type;
      Query_tmp->ParamByName("cash_id")->AsInteger   = cash_id;
      Query_tmp->ExecSQL();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка SaveCashTypeInShiftCash()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::SaveCostIDInShiftCash( int cash_id, int cost_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   if( !cash_id ) return;

   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(" UPDATE r_cash SET cost_id=:cost_id WHERE cash_id=:cash_id" );
      Query_tmp->ParamByName("cost_id")->AsInteger = cost_id;
      Query_tmp->ParamByName("cash_id")->AsInteger = cash_id;
      Query_tmp->ExecSQL();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка SaveCostIDInShiftCash()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::SaveOplataIDInShiftCash( int cash_id, int oplata_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   if( !cash_id ) return;

   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(" UPDATE r_cash SET oplata_id=:oplata_id WHERE cash_id=:cash_id" );
      Query_tmp->ParamByName("oplata_id")->AsInteger = oplata_id;
      Query_tmp->ParamByName("cash_id")->AsInteger   = cash_id;
      Query_tmp->ExecSQL();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка SaveOplataIDInShiftCash()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::GetDepartmentInfoFromShift( int shift_id, int &dep_id, String &department_name, int &point_of_money_id, String &point_of_money_name )
{

   dep_id = 0;
   point_of_money_id   = 0;
   department_name     = "";
   point_of_money_name = "";
   //
   if( !shift_id ) return;
   //
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rs.dep_id, a.name as department_name, d.point_of_money_id, p.name as point_of_money_name "+
         " FROM r_shift rs "+
         "    LEFT JOIN department d     ON rs.dep_id = d.dep_id "+
         "    LEFT JOIN agents a         ON d.agent_id = a.agent_id "+
         "    LEFT JOIN point_of_money p ON d.point_of_money_id = p.point_of_money_id "+
         " WHERE  rs.shift_id=:shift_id " );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      if( !Query_tmp->Eof ){
         dep_id              = Query_tmp->FieldByName("dep_id")->AsInteger;
         department_name     = Query_tmp->FieldByName("department_name")->AsString;
         point_of_money_id   = Query_tmp->FieldByName("point_of_money_id")->AsInteger;
         point_of_money_name = Query_tmp->FieldByName("point_of_money_name")->AsString;
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка GetDepartmentInfoFromShift()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   //
   if( !dep_id ) {
      dep_id = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","DEPARTMENT_DEF").c_str());
      int agent_id = atoi(TDBService::get_from_ID("department","agent_id","dep_id",IntToStr(dep_id)).c_str());
      department_name     = TDBService::get_from_ID("agents","name","agent_id",IntToStr(agent_id));
      point_of_money_id   = atoi(TDBService::get_from_ID("department","point_of_money_id","dep_id",IntToStr(dep_id)).c_str());
      point_of_money_name = TDBService::get_from_ID("point_of_money","name","point_of_money_id",IntToStr(point_of_money_id));
   }
   if( !point_of_money_id ) {
      point_of_money_id = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","KASA_DEF").c_str());
      point_of_money_name = TDBService::get_from_ID("point_of_money","name","point_of_money_id",IntToStr(point_of_money_id));
   }
   return;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadZakazList( vector <ZAKAZ> &zakazList, int shift_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rz.*, a.name as agent_name, if(a.type_ag_id IS NOT NULL,a.type_ag_id,0 ) as type_ag_id, ae.name as user_name "+
         " FROM r_zakaz rz "+
         "    LEFT JOIN agents a     ON rz.agent_id = a.agent_id "+
         "    LEFT JOIN employee e   ON rz.employee_id  = e.employee_id "+
         "    LEFT JOIN agents ae    ON e.agent_id = ae.agent_id "+
         " WHERE  shift_id=:shift_id " );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      zakazList.clear();
      ZAKAZ zakazE;
      while( !Query_tmp->Eof ){
         ClearZakaz(zakazE);
         //
         zakazE.zakaz_ID	          = Query_tmp->FieldByName("zakaz_id")->AsInteger;
         zakazE.employee_id          = Query_tmp->FieldByName("employee_id")->AsInteger;
         zakazE.user_name            = Query_tmp->FieldByName("user_name")->AsString;
         zakazE.agent_id             = Query_tmp->FieldByName("agent_id")->AsInteger;
         zakazE.agent_name           = Query_tmp->FieldByName("agent_name")->AsString;
         zakazE.type_ag_id           = Query_tmp->FieldByName("type_ag_id")->AsInteger;
         zakazE.shift_id             = Query_tmp->FieldByName("shift_id")->AsInteger;
         zakazE.number               = Query_tmp->FieldByName("number")->AsInteger;
         zakazE.date_z               = Query_tmp->FieldByName("date_z")->AsString;
         zakazE.time_open            = Query_tmp->FieldByName("time_open")->AsString;
         zakazE.time_close           = Query_tmp->FieldByName("time_close")->AsString;
         zakazE.stan                 = Query_tmp->FieldByName("stan")->AsInteger;
         zakazE.suma                 = Query_tmp->FieldByName("suma")->AsFloat;
         zakazE.suma_discount        = Query_tmp->FieldByName("suma_discount")->AsFloat;
         zakazE.znyzka               = Query_tmp->FieldByName("znyzka")->AsFloat;
         zakazE.bonus                = Query_tmp->FieldByName("bonus")->AsFloat;
         zakazE.repayment            = Query_tmp->FieldByName("repayment")->AsInteger;
         zakazE.repayment_check      = Query_tmp->FieldByName("repayment_check")->AsInteger;
         zakazE.discount_card_id     = Query_tmp->FieldByName("discount_card_id")->AsInteger;
         zakazE.fiscal_check         = Query_tmp->FieldByName("fiscal_check")->AsInteger;
         zakazE.fiscal_suma          = Query_tmp->FieldByName("fiscal_suma")->AsFloat;
         zakazE.fiscal_suma_discount = Query_tmp->FieldByName("fiscal_suma_discount")->AsFloat;
         zakazE.credit_card_check    = Query_tmp->FieldByName("credit_card_check")->AsString;
         zakazE.coment               = Query_tmp->FieldByName("coment")->AsString;
         //
         zakazList.push_back(zakazE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadZakazList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::ClearZakaz( ZAKAZ &zakazE )
{
   zakazE.zakaz_ID	       = 0;
   zakazE.employee_id          = 0;
   zakazE.user_name            = "";
   zakazE.agent_id             = 0;
   zakazE.agent_name           = "";
   zakazE.type_ag_id           = 0;
   zakazE.shift_id             = 0;
   zakazE.number               = 0;
   zakazE.date_z               = "";
   zakazE.time_open            = "";
   zakazE.time_close           = "";
   zakazE.stan                 = 0;
   zakazE.suma                 = 0;
   zakazE.suma_discount        = 0;
   zakazE.znyzka               = 0;
   zakazE.bonus                = 0;
   zakazE.repayment            = 0;
   zakazE.repayment_check      = 0;
   zakazE.discount_card_id     = 0;
   zakazE.fiscal_check         = 0;
   zakazE.fiscal_suma          = 0;
   zakazE.fiscal_suma_discount = 0;
   zakazE.credit_card_check    = "";
   zakazE.coment               = "";
   //
   zakazE.oper_id              = 0;
   zakazE.oper_name            = "";
   zakazE.doc_id               = 0;
   zakazE.doc_number           = 0;
   //
   zakazE.rowList.clear();
   zakazE.calculationList.clear();
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadZakazRowList( vector <ZAKAZ_ROW> &rowList, int zakaz_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      int nesting_group  = atoi(TDBService::get_from_ID("const_default","strvalue","NAME_CONST","NESTING_GROUP").c_str());
      int show_vyrobnyk  = atoi(TDBService::get_from_ID("const_default","strvalue","NAME_CONST","SHOW_VYROBNYK").c_str());
      Query_tmp->SQL->Add(String("")+
         " SELECT rz.*, t.name as name_tovar, t.group_tov_id, v.name as vyrobnyk_name, t.kod_ekka, o.short_name as ov_name, gt.type_group as type_tovar, t.ov_id "+
         " FROM r_zakaz_row rz "+
         "    LEFT JOIN t_tovar t  ON rz.tovar_id = t.tovar_id "+
         "    LEFT JOIN t_ov o     ON t.ov_id = o.ov_id "+
         "    LEFT JOIN vyrobnyk v ON t.vyrobnyk_id = v.vyrobnyk_id "+
         "    LEFT JOIN t_group_tov gt ON t.group_tov_id = gt.group_tov_id "+
         " WHERE zakaz_id=:zakaz_id " );
      //
      Query_tmp->ParamByName("zakaz_id")->AsInteger = zakaz_id;
      Query_tmp->Open();
      rowList.clear();
      ZAKAZ_ROW rowE;
      while( !Query_tmp->Eof ){
         ClearZakazRow(rowE);
         //
         rowE.zakaz_row_ID         = Query_tmp->FieldByName("zakaz_row_id")->AsInteger;
         rowE.zakaz_ID	           = Query_tmp->FieldByName("zakaz_id")->AsInteger;
         rowE.tovar_ID             = Query_tmp->FieldByName("tovar_id")->AsInteger;
         rowE.name_tovar           = (nesting_group?group.group_GET(Query_tmp->FieldByName("group_tov_id")->AsInteger)+" ":String(""))+
                                     (show_vyrobnyk?Query_tmp->FieldByName("vyrobnyk_name")->AsString+" ":String(""))+
                                     Query_tmp->FieldByName("name_tovar")->AsString;
         rowE.kod_ekka             = Query_tmp->FieldByName("kod_ekka")->AsInteger;
         rowE.ov_ID                = Query_tmp->FieldByName("ov_id")->AsInteger;
         rowE.ov_name              = Query_tmp->FieldByName("ov_name")->AsString;
         rowE.price                = Query_tmp->FieldByName("price")->AsFloat;
         rowE.quantity             = Query_tmp->FieldByName("quantity")->AsFloat;
         rowE.suma                 = Query_tmp->FieldByName("suma")->AsFloat;
         rowE.suma_discount        = Query_tmp->FieldByName("suma_discount")->AsFloat;
         rowE.bonus                = Query_tmp->FieldByName("bonus")->AsFloat;
         rowE.cd_percent           = Query_tmp->FieldByName("cd_percent")->AsFloat;
         rowE.fiscal_tovar         = Query_tmp->FieldByName("fiscal_tovar")->AsInteger;
         rowE.type_tovar           = Query_tmp->FieldByName("type_tovar")->AsInteger;
         //
         rowList.push_back(rowE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadZakazRowList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::ClearZakazRow( ZAKAZ_ROW &rowE )
{
   rowE.zakaz_row_ID         = 0;
   rowE.zakaz_ID	     = 0;
   rowE.tovar_ID             = 0;
   rowE.name_tovar           = "";
   rowE.kod_ekka             = 0;
   rowE.ov_ID                = 0;
   rowE.ov_name              = "";
   rowE.price                = 0;
   rowE.quantity             = 0;
   rowE.suma                 = 0;
   rowE.suma_discount        = 0;
   rowE.bonus                = 0;
   rowE.cd_percent           = 0;
   rowE.fiscal_tovar         = 0;
   rowE.type_tovar           = 0;
   rowE.used                 = false;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadZakazCalculationList( vector <ZAKAZ_CALCULATION> &calculationList, int zakaz_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rz.*, t.name as type_calculation_name, t.type_oplat, t.point_of_money_id, z.repayment "+
         " FROM r_zakaz_calculation rz "+
         "    LEFT JOIN r_type_calculation t ON rz.type_calculation_id = t.type_calculation_id "+
         "    LEFT JOIN r_zakaz z ON rz.zakaz_id = z.zakaz_id "+
         " WHERE rz.zakaz_id=:zakaz_id " );
      //
      Query_tmp->ParamByName("zakaz_id")->AsInteger = zakaz_id;
      Query_tmp->Open();
      calculationList.clear();
      ZAKAZ_CALCULATION calculationE;
      while( !Query_tmp->Eof ){
         ClearZakazCalculation(calculationE);
         //
         calculationE.zakaz_calculation_ID  = Query_tmp->FieldByName("zakaz_calculation_id")->AsInteger;
         calculationE.zakaz_ID	            = Query_tmp->FieldByName("zakaz_id")->AsInteger;
         calculationE.type_calculation_ID   = Query_tmp->FieldByName("type_calculation_id")->AsInteger;
         calculationE.type_calculation_name = Query_tmp->FieldByName("type_calculation_name")->AsString;
         calculationE.suma                  = (Query_tmp->FieldByName("repayment")->AsInteger?-1:1)*Query_tmp->FieldByName("suma")->AsFloat;
         calculationE.info                  = Query_tmp->FieldByName("info")->AsString;
         calculationE.type_oplat            = Query_tmp->FieldByName("type_oplat")->AsInteger;
         calculationE.point_of_money_id     = Query_tmp->FieldByName("point_of_money_id")->AsInteger;
         //
         calculationE.course                = Query_tmp->FieldByName("course")->AsFloat;
         calculationE.currency_ID           = Query_tmp->FieldByName("currency_id")->AsInteger;
         calculationE.amount_currency       = (Query_tmp->FieldByName("repayment")->AsInteger?-1:1)*Query_tmp->FieldByName("amount_currency")->AsFloat;
         //
         if( calculationE.currency_ID &&
             calculationE.currency_ID != atoi(TDBService::get_from_ID("const_default","strvalue","name_const","CURDEFAULT").c_str()) ) {
            calculationE.type_calculation_name = TFormatingData::DblToStr(calculationE.amount_currency)+
                                                 TDBService::get_from_ID("currency","short_name","currency_id",IntToStr(calculationE.currency_ID))+
                                                 "("+TFormatingData::DblToStr( calculationE.course,3 )+")";
         }
         //
         calculationList.push_back(calculationE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadZakazCalculationList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadShiftCalculationList( vector <ZAKAZ_CALCULATION> &calculationList, int shift_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rz.type_calculation_id, t.name as type_calculation_name, t.type_oplat, t.point_of_money_id, SUM(if(z.repayment,-1,1)*rz.suma) as suma, rz.course, rz.currency_id, SUM(if(z.repayment,-1,1)*rz.amount_currency) as amount_currency   "+
         " FROM r_zakaz_calculation rz "+
         "    LEFT JOIN r_type_calculation t ON rz.type_calculation_id = t.type_calculation_id "+
         "    LEFT JOIN r_zakaz z ON rz.zakaz_id = z.zakaz_id "+
         " WHERE z.shift_id=:shift_id "+
         " GROUP BY rz.type_calculation_id, rz.currency_id, rz.course "+
         " ORDER BY t.name " );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      calculationList.clear();
      ZAKAZ_CALCULATION calculationE;
      while( !Query_tmp->Eof ){
         ClearZakazCalculation(calculationE);
         //
         calculationE.type_calculation_ID   = Query_tmp->FieldByName("type_calculation_id")->AsInteger;
         calculationE.type_calculation_name = Query_tmp->FieldByName("type_calculation_name")->AsString;
         calculationE.suma                  = Query_tmp->FieldByName("suma")->AsFloat;
         calculationE.type_oplat            = Query_tmp->FieldByName("type_oplat")->AsInteger;
         calculationE.point_of_money_id     = Query_tmp->FieldByName("point_of_money_id")->AsInteger;
         //
         calculationE.course                = Query_tmp->FieldByName("course")->AsFloat;
         calculationE.currency_ID           = Query_tmp->FieldByName("currency_id")->AsInteger;
         calculationE.amount_currency       = Query_tmp->FieldByName("amount_currency")->AsFloat;
         //
         if( calculationE.currency_ID &&
             calculationE.currency_ID != atoi(TDBService::get_from_ID("const_default","strvalue","name_const","CURDEFAULT").c_str()) ) {
            calculationE.type_calculation_name = TFormatingData::DblToStr(calculationE.amount_currency)+
                                                 TDBService::get_from_ID("currency","short_name","currency_id",IntToStr(calculationE.currency_ID))+
                                                 "("+TFormatingData::DblToStr( calculationE.course,3 )+")";
         }
         calculationList.push_back(calculationE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftCalculationList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::ClearZakazCalculation( ZAKAZ_CALCULATION &calculationE )
{
   calculationE.zakaz_calculation_ID  = 0;
   calculationE.zakaz_ID	      = 0;
   calculationE.type_calculation_ID   = 0;
   calculationE.type_calculation_name = "";
   calculationE.suma                  = 0;
   calculationE.info                  = "";
   calculationE.type_oplat            = 0;
   calculationE.point_of_money_id     = 0;
   calculationE.course                = 0;
   calculationE.currency_ID           = 0;
   calculationE.amount_currency       = 0;
}
//---------------------------------------------------------------------------

String TShiftSQL::GetDiscountCardOwner( int discount_card_id )
{
   String result = String("");
   if( !pConnect->IsConnecting )
      return result;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT number, owner_name  "+
         " FROM r_discount_card "+
         " WHERE discount_card_id=:discount_card_id ");
      //
      Query_tmp->ParamByName("discount_card_id")->AsInteger = discount_card_id;
      Query_tmp->Open();
      if( !Query_tmp->Eof )
         result = Query_tmp->FieldByName("number")->AsString+" "+Query_tmp->FieldByName("owner_name")->AsString;
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних GetDiscountCardOwner()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   return result;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void TShiftSQL::SetShiftStan(int shift_id, int stan)
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " UPDATE r_shift SET stan=:stan WHERE shift_id=:shift_id " );
      //
      Query_tmp->ParamByName("stan")->AsInteger     = stan;
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->ExecSQL();
      //
      // Для реплікованих РРО заповнюємо таблицю r_shift_closed якщо cash_register_shift_id!=0
      Query_tmp->SQL->Clear();
//      Query_tmp->SQL->Add(String("")+
//         " SELECT cash_register_id, cash_register_shift_id, stan FROM r_shift WHERE shift_id=:shift_id " );
      Query_tmp->SQL->Add(String("")+
         " SELECT s.cash_register_id, s.cash_register_shift_id, s.stan "+
         " FROM r_shift s "+
         "    LEFT JOIN r_cash_register c ON s.cash_register_id = c.cash_register_id "+
         " WHERE s.shift_id=:shift_id AND c.type_cash_register_id=1 " ); // Лише для терміналів
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      int cash_register_id=0, cash_register_shift_id=0, stan=0;
      if( !Query_tmp->Eof ) {
         cash_register_id       = Query_tmp->FieldByName("cash_register_id")->AsInteger;
         cash_register_shift_id = Query_tmp->FieldByName("cash_register_shift_id")->AsInteger;
         stan = Query_tmp->FieldByName("stan")->AsInteger;
      }
      Query_tmp->Close();
      //
      if( cash_register_shift_id ) {
         Query_tmp->SQL->Clear();
         Query_tmp->SQL->Add(String("")+
            " INSERT INTO r_shift_closed VALUES (:cash_register_id, :cash_register_shift_id, :stan) "+
            " ON DUPLICATE KEY UPDATE stan=:stan " );
         //
         Query_tmp->ParamByName("stan")->AsInteger                   = stan;
         Query_tmp->ParamByName("cash_register_id")->AsInteger       = cash_register_id;
         Query_tmp->ParamByName("cash_register_shift_id")->AsInteger = cash_register_shift_id;
         Query_tmp->ExecSQL();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка збереження даних SetShiftStan()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadAgentShift( int shift_id, vector <int> &agentShift )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT agent_id "+
         " FROM r_zakaz "+
         " WHERE shift_id=:shift_id GROUP BY agent_id ORDER BY agent_id" );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      //
      agentShift.clear();
      while( !Query_tmp->Eof ){
         int agent_id = Query_tmp->FieldByName("agent_id")->AsInteger;
         if( !agent_id ) // Нульового агента заміняємо на Покупця по замовчуванню
            agent_id = atoi(TDBService::get_from_ID("firms","agent_id","firm_id",TDBService::get_from_ID("const_default","strvalue","name_const","CUSTOMER_DEF")).c_str());
         //
         int i=0;
         for( i=0; i<(int)agentShift.size(); i++ )
            if( agent_id == agentShift[i] ) break;
         //
         if( i==(int)agentShift.size() ) // Заносимо лише унікальний код
            agentShift.push_back( agent_id );
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadAgentShift()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

int TShiftSQL::LoadDefaultStore( )
{
   return atoi(TDBService::get_from_ID("department","agent_id","dep_id",TDBService::get_from_ID("const_default","strvalue","name_const","DEPARTMENT_DEF")).c_str());
}
//---------------------------------------------------------------------------

bool TShiftSQL::ReadRealToVector( int agent_id, vector <TERMINAL_TOVAR> &tov, String date_doc, int shift_id, vector <ZAKAZ_CALCULATION> &total_calculation, int repayment, int zakaz_id )
{
   if( !pConnect->IsConnecting )
      return false;
   //
   int default_agent_id = atoi(TDBService::get_from_ID("firms","agent_id","firm_id",TDBService::get_from_ID("const_default","strvalue","name_const","CUSTOMER_DEF")).c_str());
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT zr.tovar_id, t.kod_ekka, t.make_flag, gt.type_group as type_tovar, SUM(zr.quantity) as quantity_real, SUM(zr.suma_discount) as suma_real, SUM(zr.suma) as suma_no_discount "+
         " FROM r_zakaz_row zr "+
         "    LEFT JOIN r_zakaz z ON (zr.zakaz_id=z.zakaz_id) "+
         "    LEFT JOIN t_tovar t ON (zr.tovar_id=t.tovar_id) "+
         "    LEFT JOIN t_group_tov gt ON (t.group_tov_id=gt.group_tov_id) "+
         " WHERE z.shift_id = :shift_id AND z.stan = 1 " );
      if( agent_id == default_agent_id ) // Для агента по замовчуванню читаємо і чеки з agent_id=0
         Query_tmp->SQL->Add( "   AND ( z.agent_id=:agent_id OR z.agent_id=0) " );
      else
         Query_tmp->SQL->Add( "   AND z.agent_id=:agent_id " );
      if( zakaz_id  ) // Накладна по конкретному чеку
         Query_tmp->SQL->Add( "   AND zr.zakaz_id=:zakaz_id " );
      Query_tmp->SQL->Add(String("")+
         "   AND date_z=:date_z "+
         "   AND repayment=:repayment "+
         " GROUP BY zr.tovar_id "+
         " ORDER BY kod_ekka " );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->ParamByName("agent_id")->AsInteger = agent_id;
      Query_tmp->ParamByName("date_z")->AsDate = StrToDate(date_doc);
      Query_tmp->ParamByName("repayment")->AsInteger = repayment;
      if( zakaz_id  ) // Накладна по конкретному чеку
         Query_tmp->ParamByName("zakaz_id")->AsInteger = zakaz_id;
      Query_tmp->Open();
      //
      tov.clear();
      TERMINAL_TOVAR lt;
      while( !Query_tmp->Eof ){
         lt.tovar_ID         = Query_tmp->FieldByName("tovar_id")->AsInteger;
         lt.kod_ekka         = Query_tmp->FieldByName("kod_ekka")->AsInteger;
         lt.quantity_real    = Query_tmp->FieldByName("quantity_real")->AsFloat;
         lt.suma_real        = Query_tmp->FieldByName("suma_real")->AsFloat;
         lt.suma_no_discount = Query_tmp->FieldByName("suma_no_discount")->AsFloat;
         lt.type_tovar       = Query_tmp->FieldByName("type_tovar")->AsInteger;
         lt.make_flag        = Query_tmp->FieldByName("make_flag")->AsInteger;
         //
         tov.push_back(lt);
         //
         Query_tmp->Next();
      }
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add( String("")+
         " SELECT zc.type_calculation_id, tc.name as type_calculation_name, tc.type_oplat, tc.point_of_money_id, SUM(zc.suma) as suma_calculation,SUM(if(zc.suma>0,zc.suma,0)) as suma_plus,SUM(if(zc.suma<0,zc.suma,0)) as suma_minus, zc.currency_id, zc.course, SUM(zc.amount_currency) as amount_currency "+
         " FROM r_zakaz z  "+
         "    LEFT JOIN r_zakaz_calculation zc ON (z.zakaz_id=zc.zakaz_id) "+
         "    LEFT JOIN r_type_calculation tc ON (zc.type_calculation_id=tc.type_calculation_id) "+
         " WHERE z.shift_id = :shift_id AND z.stan = 1 " );
      if( agent_id == default_agent_id ) // Для агента по замовчуванню читаємо і чеки з agent_id=0
         Query_tmp->SQL->Add( "   AND ( z.agent_id=:agent_id OR z.agent_id=0) " );
      else
         Query_tmp->SQL->Add( "   AND z.agent_id=:agent_id " );
      if( zakaz_id  ) // Накладна по конкретному чеку
         Query_tmp->SQL->Add( "   AND z.zakaz_id=:zakaz_id " );
      Query_tmp->SQL->Add(String("")+
         "   AND date_z = :date_z "+
         "   AND repayment=:repayment "+
         "   AND zc.type_calculation_id is not null "+
         " GROUP BY zc.type_calculation_id, zc.currency_id, zc.course "+
         " ORDER BY zc.type_calculation_id " );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->ParamByName("agent_id")->AsInteger = agent_id;
      Query_tmp->ParamByName("date_z")->AsDate = StrToDate(date_doc);
      Query_tmp->ParamByName("repayment")->AsInteger = repayment;
      if( zakaz_id  ) // Накладна по конкретному чеку
         Query_tmp->ParamByName("zakaz_id")->AsInteger = zakaz_id;
      Query_tmp->Open();
      //
      int bonus_type_calculation_id = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","BONUS_TYPE_CALC").c_str());
      if( !atoi(TDBService::get_from_ID("const_default","strvalue","name_const","BONUS_ON").c_str()) ) bonus_type_calculation_id = 0;
      //
      total_calculation.clear();
      ZAKAZ_CALCULATION zc;
      while( !Query_tmp->Eof ){
         ClearZakazCalculation(zc);
         //
         zc.type_calculation_ID   = Query_tmp->FieldByName("type_calculation_id")->AsInteger;
         zc.type_calculation_name = Query_tmp->FieldByName("type_calculation_name")->AsString;
         zc.suma                  = Query_tmp->FieldByName("suma_calculation")->AsFloat;
         zc.type_oplat            = Query_tmp->FieldByName("type_oplat")->AsInteger;
         zc.point_of_money_id     = Query_tmp->FieldByName("point_of_money_id")->AsInteger;
         //
         zc.course                = Query_tmp->FieldByName("course")->AsFloat;
         zc.currency_ID           = Query_tmp->FieldByName("currency_id")->AsInteger;
         zc.amount_currency       = Query_tmp->FieldByName("amount_currency")->AsFloat;
         //
         if( bonus_type_calculation_id == zc.type_calculation_ID ) {
            zc.suma                  = Query_tmp->FieldByName("suma_plus")->AsFloat;
            total_calculation.push_back(zc);
            //
            zc.type_calculation_name = Query_tmp->FieldByName("type_calculation_name")->AsString+"+";
            zc.suma                  = Query_tmp->FieldByName("suma_minus")->AsFloat;
            total_calculation.push_back(zc);
         }
         else
            total_calculation.push_back(zc);
         //
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних ReadRealToVector()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   //
   return true;
}
//---------------------------------------------------------------------------

int TShiftSQL::CreateNakl( vector <TERMINAL_TOVAR> &tov,
           String date_doc, String time_doc, int agent_id, int agent_to_id, String coment, int shift_id )
{
   if( !pConnect->IsConnecting )
      return 0;
   //
   int doc_tov_ID = 0;
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      int oper_id = 0;
      // Визначаємо тип документа по агентах
      oper_id   = TShiftSQL::GetOperationID( agent_id, agent_to_id );
      // Підрахувати суму по документу
      double suma_doc_real = 0, suma_no_discount = 0;
      for( int n = 0; n < (int)tov.size(); n++ ) {
         suma_doc_real += tov[n].suma_real;
         suma_no_discount += tov[n].suma_no_discount;
      }
      //
      // 1. Записати дані в таблицю document
      //
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " INSERT INTO document ( agent_id, agent_to_id,  pryzn, currency_id, course, number_d, date_d, time_d, suma, suma_add, rest, pdv, stan, price_id, user, author, type_doc_id, oper_id, number_doc ) "+
         " VALUES(:agent_from_id,:agent_to_id,:coment, :currency_id,1, :number_d, :date_d,:time_d,:suma,:suma_add,:suma, 0,0,:price_id,:user,:author,:type_doc_id, :oper_id, :number_doc ) " );
      //
      int number_d = TSubMethods::NewNumberDocMaterial(oper_id,date_doc);
      Query_tmp->ParamByName("agent_from_id")->AsInteger = agent_id;
      Query_tmp->ParamByName("agent_to_id")->AsInteger   = agent_to_id;
      Query_tmp->ParamByName("number_doc")->AsString     = shift_id;
      Query_tmp->ParamByName("coment")->AsString         = coment;
      Query_tmp->ParamByName("number_d")->AsInteger      = number_d;
      Query_tmp->ParamByName("date_d")->AsDate           = StrToDate(date_doc);
      Query_tmp->ParamByName("time_d")->AsString         = time_doc;
      Query_tmp->ParamByName("suma")->AsFloat            = suma_doc_real;
      Query_tmp->ParamByName("suma_add")->AsFloat        = suma_no_discount;
      Query_tmp->ParamByName("price_id")->AsInteger      = StrToIntDef(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "TYPE_PRICE", "0" ),0);
      Query_tmp->ParamByName("user")->AsString           = LOGIN;
      Query_tmp->ParamByName("author")->AsString         = LOGIN;
      Query_tmp->ParamByName("type_doc_id")->AsInteger   = 13;
      Query_tmp->ParamByName("oper_id")->AsInteger       = oper_id;
      Query_tmp->ParamByName("currency_id")->AsInteger   = StrToIntDef(TDBService::get_from_ID("const_default","strvalue","name_const","CURDEFAULT").c_str(),0);
      //
      Query_tmp->ExecSQL();
      //
      doc_tov_ID = Query_tmp->LastInsertID();
      //
      String code_oper = TSubMethods::CodeOper_GET( doc_tov_ID );
      //
      // Зберегти рядки
      //
      double price = 0, price_no_discount = 0, cur_price = 0;
      int QUANTITY_SCALE  = StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "QUANTITYSCALE", "3" ));
      int PRICE_OUT_SCALE = StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "PRICEOUTSCALE", "2" ));
      //
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " INSERT INTO t_ruh_tovar( doc_id, tovar_id, quantity, price, suma, price_add, suma_add, price_rozr, suma_rozr, discount, price_in, correction_price, order_row, koef, quantity_ov, debet_id, kredyt_id) "+
         "                  VALUES(:doc_id,:tovar_id,:quantity,:price,:suma,:price_add,:suma_add,:price_rozr,:suma_rozr,:discount,:price_in,:correction_price,:order_row,    1,           1,:debet_id,:kredyt_id) ");
      for(int i = 0; i < (int)tov.size(); i++) {
         Query_tmp->ParamByName("doc_id")->AsInteger   = doc_tov_ID;
         Query_tmp->ParamByName("tovar_id")->AsInteger = tov[i].tovar_ID;
         Query_tmp->ParamByName("quantity")->AsFloat   = TFormatingData::scaleDbl(tov[i].quantity_real,QUANTITY_SCALE);
         //
         if( tov[i].quantity_real ) {
            price = tov[i].suma_real/tov[i].quantity_real;
            price_no_discount = tov[i].suma_no_discount/tov[i].quantity_real;
         }
         else {
            price = 0;
            price_no_discount = 0;
         }
         //
         if( StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "SUMA_NO_DISCOUNT", "0" )) ){
            Query_tmp->ParamByName("price")->AsFloat    = TFormatingData::scaleDbl(price_no_discount,PRICE_OUT_SCALE);
            Query_tmp->ParamByName("suma")->AsFloat     = TFormatingData::scaleDbl(tov[i].suma_no_discount);
         }
         else {
            cur_price = TWorkDocTov::GetPriceDepCurDate(tov[i].tovar_ID,((code_oper=="BACK_CUSTS")?agent_to_id:agent_id),date_doc,time_doc, true);
            if ( !cur_price )
               cur_price = price_no_discount; // price; 18-06-2014
            Query_tmp->ParamByName("price")->AsFloat    = TFormatingData::scaleDbl(cur_price,PRICE_OUT_SCALE);
            Query_tmp->ParamByName("suma")->AsFloat     = TFormatingData::scaleDbl(cur_price * tov[i].quantity_real,2);
         }
         Query_tmp->ParamByName("price_add")->AsFloat   = TFormatingData::scaleDbl(price,PRICE_OUT_SCALE);
         Query_tmp->ParamByName("suma_add")->AsFloat    = TFormatingData::scaleDbl(tov[i].suma_real);
         //
         Query_tmp->ParamByName("price_rozr")->AsFloat  = TSubMethods::PriceRozr(tov[i].tovar_ID);
         Query_tmp->ParamByName("suma_rozr")->AsFloat   = TFormatingData::scaleDbl(tov[i].quantity_real * TSubMethods::PriceRozr(tov[i].tovar_ID));
         //
//         Query_tmp->ParamByName("discount")->AsFloat    = TFormatingData::scaleDbl(TFormatingData::scaleDbl(cur_price * tov[i].quantity_real) - tov[i].suma_real);
         Query_tmp->ParamByName("discount")->AsFloat    = TFormatingData::scaleDbl(tov[i].suma_no_discount - tov[i].suma_real);
         Query_tmp->ParamByName("correction_price")->AsFloat = TFormatingData::scaleDbl(TFormatingData::scaleDbl(cur_price * tov[i].quantity_real)  - tov[i].suma_no_discount);
         //
         Query_tmp->ParamByName("order_row")->AsInteger = i;
         Query_tmp->ParamByName("price_in")->AsFloat    = atof(TDBService::get_from_ID("t_tovar","price_in","tovar_id",String(tov[i].tovar_ID)).c_str()); // StrToFloatDef(TDBService::get_from_ID("t_tovar","price_in","tovar_id",String(tov[i].tovar_ID)).c_str(),0);
         // РАХУНКИ
         String code_oper = TDBService::get_from_ID("operation","code_oper","oper_id",String(oper_id));
         if ( code_oper == "REAL_STORE" || code_oper == "MOVE_GP" ) {
            Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_id,"REAL_DEBET");
            Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_id,tov[i].tovar_ID);
         }
         else if ( code_oper == "MOVE_VIDDIL" || code_oper == "MOVE_STORE" || code_oper == "PUT_VIDDIL"|| code_oper == "PUT_BACK" ) {
            Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_to_id,tov[i].tovar_ID);
            Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_id,tov[i].tovar_ID);
         }
         else if ( code_oper == "BACK_CUSTS" || code_oper == "BACK_CUSTV" ) {
            Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_to_id,tov[i].tovar_ID);
            Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_to_id,"GET_KREDYT");
         }
         else {
            Query_tmp->ParamByName("debet_id")->AsInteger  = 0;
            Query_tmp->ParamByName("kredyt_id")->AsInteger = 0;
            //
            ShowMessage("Невідома операція '"+code_oper+"'("+String(oper_id)+")");
         }
         //
         Query_tmp->ExecSQL();
         //
         //
         //
         if ( tov[i].type_tovar == 7  && !tov[i].make_flag ) { // Комплект
            TSubMethods::SaveKomplektTovar(doc_tov_ID, code_oper == "BACK_CUSTV" || code_oper == "BACK_CUSTS" ? agent_to_id : agent_id,tov[i].tovar_ID,tov[i].quantity_real,Query_tmp->LastInsertID(),date_doc,time_doc);
         }
      }
      // 3. Записати дані в таблицю r_link_shift_docs
      //
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " INSERT INTO r_link_shift_docs( shift_id, document_id ) "+
         "                        VALUES(:shift_id,:document_id ) ");
      //
      Query_tmp->ParamByName("shift_id")->AsInteger    = shift_id;
      Query_tmp->ParamByName("document_id")->AsInteger = doc_tov_ID;
      Query_tmp->ExecSQL();
      //
      // При потребі зразу ж провести
      int prov_auto = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","PROV_AUTO").c_str());
      if ( prov_auto ) {
         if ( !TSubMethods::IntoCurrentPeriod(date_doc) ) {
            ShowMessage("Дата документу №"+String(number_d)+" відноситься до закритого періоду!!!");
         }
         else {
            String ErrMsg = "";
            TMaterialDoc::ProvDoc(true,doc_tov_ID,(TProgressBar *) NULL,false,ErrMsg);
            if ( ErrMsg != "" )
               ShowMessage(ErrMsg);
         }
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка CreateNakl()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   return doc_tov_ID;
}
//---------------------------------------------------------------------------

int TShiftSQL::GetOperationID( int agent_id, int agent_to_id )
{
   if( !pConnect->IsConnecting )
      return 0;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   int operation_id = 0, type_ag_id = 0, type_ag_to_id = 0;
   try {
      // Від КОГО
      Query_tmp->SQL->Add(String("")+
         " SELECT type_ag_id "+
         " FROM agents "+
         " WHERE agent_id=:agent_id " );
      //
      Query_tmp->ParamByName("agent_id")->AsInteger = agent_id;
      Query_tmp->Open();
      //
      if( !Query_tmp->Eof )
         type_ag_id = Query_tmp->FieldByName("type_ag_id")->AsInteger;
      // КОМУ
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " SELECT type_ag_id "+
         " FROM agents "+
         " WHERE agent_id=:agent_id " );
      //
      Query_tmp->ParamByName("agent_id")->AsInteger = agent_to_id;
      Query_tmp->Open();
      //
      if( !Query_tmp->Eof )
         type_ag_to_id = Query_tmp->FieldByName("type_ag_id")->AsInteger;
      // Slava для покупця/постачальника
      // зі складом працює постачальник
      if( type_ag_id==5 && type_ag_to_id==7 ) type_ag_to_id = 6;
      if( type_ag_to_id==5 && type_ag_id==7 ) type_ag_id = 6;
      // з відділом працює покупець
      if( type_ag_id==2 && type_ag_to_id==7 ) type_ag_to_id = 6;
      if( type_ag_to_id==2 && type_ag_id==7 ) type_ag_id = 6;
      //
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " SELECT oper_id "+
         " FROM operation "+
         " WHERE type_ag_id1=:type_ag_id AND type_ag_id2=:type_ag_to_id" );
      Query_tmp->ParamByName("type_ag_id")->AsInteger    = type_ag_id;
      Query_tmp->ParamByName("type_ag_to_id")->AsInteger = type_ag_to_id;
      //
      Query_tmp->Open();
      //
      if( !Query_tmp->Eof )
         operation_id = Query_tmp->FieldByName("oper_id")->AsInteger;
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка GetOperationID()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   //
   return operation_id;
}
//---------------------------------------------------------------------------

void TShiftSQL::CreateOplat( vector <ZAKAZ_CALCULATION> &total_calculation, int type_opl_id,
           String date_doc, int dep_agent_id, int agent_id, int shift_id, int doc_tov_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   int point_of_money_id = 0;
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      for( int i=0;i<(int)total_calculation.size();i++  ) {
         if( !total_calculation[i].suma ) continue;
         point_of_money_id = 0;
         // 1. По типу розрахунку та агентах визначаємо тип оплати та джерело коштів
         switch( total_calculation[i].type_oplat ) {
            case 0: // Оплату не створюємо
               continue;
            case 1: // Каса береться з агента складу(відділа) або з const_default
               point_of_money_id = atoi(TDBService::get_from_ID("department","point_of_money_id","agent_id",String(dep_agent_id)).c_str());
               if( !point_of_money_id )
                  point_of_money_id = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","KASA_DEF").c_str());
               break;
            case 2: // Фіктивна каса береться з типу розрахунку або з const_default
               point_of_money_id = total_calculation[i].point_of_money_id;
               if( !point_of_money_id )
                  point_of_money_id = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","FKASA_DEF").c_str());
               break;
         }
         if( !point_of_money_id ) continue;
         // 2. Добавляємо оплату
         Query_tmp->Close();
         Query_tmp->SQL->Clear();
         Query_tmp->SQL->Add(String("")+
            " INSERT INTO oplata ( agent_from_id, number_d, date_d, info, suma, stan, type_opl, alive, order_id, checked_flag, agent_to_id, agent_id, department_id, date_changed, changed, sent, location, rest, user, author ) "+
            "        VALUES      (:agent_from_id,:number_d,:date_d,:info,:suma,:stan,:type_opl,     0,        0,            0,:agent_to_id,:agent_id,:department_id,        NOW(),       1,    0,        0,:rest,:user,:author ) " );
         //
         int number_d = TSubMethods::NewNumberDocOplata( type_opl_id, date_doc );
         Query_tmp->ParamByName("agent_from_id")->AsInteger = 0;
         Query_tmp->ParamByName("number_d")->AsInteger      = number_d;
         Query_tmp->ParamByName("date_d")->AsDate           = StrToDate(date_doc);
         Query_tmp->ParamByName("info")->AsString           = String("Накладна №")+TDBService::get_from_ID("document","number_d","doc_id",String(doc_tov_id))+"\n "+
                                                              TDBService::get_from_ID("document","pryzn","doc_id",String(doc_tov_id));
         Query_tmp->ParamByName("suma")->AsFloat            = total_calculation[i].suma;
         Query_tmp->ParamByName("stan")->AsInteger          = 1;
         Query_tmp->ParamByName("type_opl")->AsInteger      = type_opl_id;
         Query_tmp->ParamByName("agent_to_id")->AsInteger   = point_of_money_id;
         Query_tmp->ParamByName("agent_id")->AsInteger      = agent_id;
         Query_tmp->ParamByName("department_id")->AsInteger = atoi(TDBService::get_from_ID("department","dep_id","agent_id",String(dep_agent_id)).c_str());
         Query_tmp->ParamByName("rest")->AsFloat            = fabs(total_calculation[i].suma);
         Query_tmp->ParamByName("user")->AsString           = LOGIN;
         Query_tmp->ParamByName("author")->AsString         = LOGIN;
         //
         Query_tmp->ExecSQL();
         //
         int oplata_id = Query_tmp->LastInsertID();
         // Добавляємо рядки
         if( total_calculation[i].type_calculation_ID == 1 || total_calculation[i].type_calculation_ID == 7 ) { // Готівка
            double suma_total = 0;
            for( int j=0; j<(int)total_calculation.size();j++  ) {
               if( total_calculation[j].type_calculation_ID !=1 && total_calculation[j].type_calculation_ID !=7 ) continue;
               //
               AddOplataRow( oplata_id, total_calculation[j].currency_ID, total_calculation[j].amount_currency, total_calculation[j].course, total_calculation[j].suma );
               suma_total += total_calculation[j].suma;
               total_calculation[j].suma = 0; // щоб наступного проходу пропустити
            }
            // Записуємо підраховану суму в оплату та залишок
            TDBService::update_double_field_ID("oplata","suma",suma_total,"oplata_id",String(oplata_id));
            TDBService::update_double_field_ID("oplata","rest",fabs(suma_total),"oplata_id",String(oplata_id));
         }
         else if( total_calculation[i].type_calculation_ID == atoi(TDBService::get_from_ID("const_default","strvalue","name_const","BONUS_TYPE_CALC").c_str()) ){ // Бонуси
            double suma_total = 0;
            for( int j=0; j<(int)total_calculation.size();j++  ) {
               if( total_calculation[j].type_calculation_ID != total_calculation[i].type_calculation_ID ) continue;
               //
               AddOplataRow( oplata_id, total_calculation[j].currency_ID, total_calculation[j].amount_currency, total_calculation[j].course, total_calculation[j].suma );
               suma_total += total_calculation[j].suma;
               total_calculation[j].suma = 0; // щоб наступного проходу пропустити
            }
            // Записуємо підраховану суму в оплату та залишок
            TDBService::update_double_field_ID("oplata","suma",suma_total,"oplata_id",String(oplata_id));
            TDBService::update_double_field_ID("oplata","rest",fabs(suma_total),"oplata_id",String(oplata_id));
         }
         else { // один рядок
            AddOplataRow( oplata_id, total_calculation[i].currency_ID, total_calculation[i].amount_currency, total_calculation[i].course, total_calculation[i].suma );
         }
         // і привязуємо до документа
         if( oplata_id && doc_tov_id ) {
            double rest_doc_tov = atof(TDBService::get_from_ID("document","rest","doc_id",String(doc_tov_id)).c_str());
            double rest_oplata  = atof(TDBService::get_from_ID("oplata",  "rest","oplata_id",String(oplata_id)).c_str());
            //
            Query_tmp->Close();
            Query_tmp->SQL->Clear();
            Query_tmp->SQL->Add(String("")+
               " INSERT INTO link_doc_opl ( doc_tov_id, oplata_id, suma ) "+
               "        VALUES            (:doc_tov_id,:oplata_id,:suma ) " );
            //
            Query_tmp->ParamByName("doc_tov_id")->AsInteger = doc_tov_id;
            Query_tmp->ParamByName("oplata_id")->AsInteger  = oplata_id;
            Query_tmp->ParamByName("suma")->AsFloat         = (rest_doc_tov>rest_oplata?rest_oplata:rest_doc_tov);
            //
            Query_tmp->ExecSQL();
            //
            TDBService::update_double_field_ID("document","rest",(rest_doc_tov>rest_oplata?rest_doc_tov-rest_oplata:0),"doc_id",String(doc_tov_id));
            TDBService::update_double_field_ID("oplata",  "rest",(rest_doc_tov>rest_oplata?0:rest_oplata-rest_doc_tov),"oplata_id",String(oplata_id));
         }
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка CreateOplat()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   return;
}
//---------------------------------------------------------------------------

void TShiftSQL::AddOplataRow( int oplata_id, int currency_id, double amount_currency, double course, double suma )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      if( !currency_id || !course ) {
         currency_id = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","CURDEFAULT").c_str());
         course = 1;
         amount_currency = suma;
      }
      if( !amount_currency ) amount_currency = suma/course;
      // Добавляємо рядок оплати
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " INSERT INTO oplata_row ( oplata_id, currency_id, quantity, course, suma, head_row ) "+
         "            VALUES      (:oplata_id,:currency_id,:quantity,:course,:suma,        0 ) " );
      //
      Query_tmp->ParamByName("oplata_id")->AsInteger     = oplata_id;
      Query_tmp->ParamByName("currency_id")->AsInteger   = currency_id;
      Query_tmp->ParamByName("quantity")->AsFloat        = amount_currency;
      Query_tmp->ParamByName("course")->AsFloat          = course;
      Query_tmp->ParamByName("suma")->AsFloat            = suma;
      //
      Query_tmp->ExecSQL();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка AddOplataRow()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   return;
}
//---------------------------------------------------------------------------
// Функції роботи по документах по зміні
//---------------------------------------------------------------------------

void TShiftSQL::GetShiftDocs( int shift_id, vector <SHIFT_DOCS> &shiftDocList )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT dt.*, aFrom.name as from_name, aTo.name as to_name, o.code_oper "+
         " FROM r_link_shift_docs l "+
         "    LEFT JOIN document dt ON (l.document_id = dt.doc_id) "+
         "    LEFT JOIN agents aFrom ON (dt.agent_id = aFrom.agent_id) "+
         "    LEFT JOIN agents aTo ON (dt.agent_to_id = aTo.agent_id) "+
         "    LEFT JOIN operation o ON (dt.oper_id = o.oper_id) "+
        " WHERE l.shift_id=:shift_id AND dt.doc_id IS NOT NULL "+
         " ORDER BY dt.doc_id ");
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      shiftDocList.clear();
      SHIFT_DOCS shiftDocE;
      while( !Query_tmp->Eof ){
         ClearShiftDoc(shiftDocE);
         //
         shiftDocE.doc_tov_ID = Query_tmp->FieldByName("doc_id")->AsInteger;
         shiftDocE.number_d   = Query_tmp->FieldByName("number_d")->AsInteger;
         shiftDocE.date_d     = Query_tmp->FieldByName("date_d")->AsString;
         shiftDocE.time_d     = Query_tmp->FieldByName("time_d")->AsString;
         shiftDocE.from_name  = Query_tmp->FieldByName("from_name")->AsString;
         shiftDocE.to_name    = Query_tmp->FieldByName("to_name")->AsString;
         shiftDocE.coment     = Query_tmp->FieldByName("pryzn")->AsString;
         shiftDocE.suma       = (Query_tmp->FieldByName("code_oper")->AsString=="BACK_CUSTV"?-1:1)*Query_tmp->FieldByName("suma")->AsFloat;
         shiftDocE.stan       = Query_tmp->FieldByName("stan")->AsInteger;
         //
         shiftDocList.push_back(shiftDocE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadZakazCalculationList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete  Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::ClearShiftDoc( SHIFT_DOCS &shiftDocE )
{
   shiftDocE.doc_tov_ID = 0;
   shiftDocE.number_d   = 0;
   shiftDocE.date_d     = "";
   shiftDocE.time_d     = "";
   shiftDocE.from_name  = "";
   shiftDocE.to_name    = "";
   shiftDocE.coment     = "";
   shiftDocE.suma       = 0;
   shiftDocE.stan       = 0;
}
//---------------------------------------------------------------------------

void TShiftSQL::DeleteShiftDoc( int shift_id, int doc_tov_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      // Вилучити шапку документа
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add("DELETE FROM document WHERE doc_id=:doc_id");
      Query_tmp->ParamByName("doc_id")->AsInteger = doc_tov_id;
      Query_tmp->ExecSQL();
      // Вилучити рядки документа
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add("DELETE FROM t_ruh_tovar WHERE doc_id=:doc_id");
      Query_tmp->ParamByName("doc_id")->AsInteger = doc_tov_id;
      Query_tmp->ExecSQL();
      // Вилучити лінки документа та зміни
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add("DELETE FROM r_link_shift_docs WHERE document_id=:doc_id AND shift_id=:shift_id" );
      Query_tmp->ParamByName("doc_id")->AsInteger   = doc_tov_id;
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->ExecSQL();
      // Вилучити оплати документа
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add("DELETE FROM oplata USING oplata, link_doc_opl WHERE link_doc_opl.doc_tov_id=:doc_id AND oplata.oplata_id=link_doc_opl.oplata_id" );
      Query_tmp->ParamByName("doc_id")->AsInteger   = doc_tov_id;
      Query_tmp->ExecSQL();
      //
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка DeleteShiftDoc()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::DeleteDocOplat( int doc_tov_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      // Вилучити оплати документа
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add("DELETE FROM oplata USING oplata, link_doc_opl WHERE link_doc_opl.doc_tov_id=:doc_id AND oplata.oplata_id=link_doc_opl.oplata_id" );
      Query_tmp->ParamByName("doc_id")->AsInteger   = doc_tov_id;
      Query_tmp->ExecSQL();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка DeleteDocOplat()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::GetDocTovar( int doc_tov_id, vector <TERMINAL_TOVAR> &tovarList)
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         "SELECT rt.tovar_id, t.kod_ekka, rt.quantity, rt.suma, rt.suma_add "+
         " FROM t_ruh_tovar rt "+
         "    LEFT JOIN t_tovar t ON rt.tovar_id = t.tovar_id "+
         " WHERE doc_id = :doc_tov_id ");
      Query_tmp->ParamByName("doc_tov_id")->AsInteger = doc_tov_id;
      Query_tmp->Open();
      tovarList.clear();
      TERMINAL_TOVAR tovarListE;
      while( !Query_tmp->Eof ){
         ClearTerminalTovar(tovarListE);
         //
         tovarListE.tovar_ID         = Query_tmp->FieldByName("tovar_id")->AsInteger;
         tovarListE.kod_ekka         = Query_tmp->FieldByName("kod_ekka")->AsInteger;
         tovarListE.price            = Query_tmp->FieldByName("price")->AsFloat;
         tovarListE.quantity_real    = Query_tmp->FieldByName("quantity")->AsFloat;
         tovarListE.suma_real        = Query_tmp->FieldByName("suma")->AsFloat;
         tovarListE.suma_no_discount = Query_tmp->FieldByName("suma_add")->AsFloat;
         //
         tovarList.push_back(tovarListE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка GetDocTovar()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete  Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::ClearTerminalTovar( TERMINAL_TOVAR &tovarE )
{
   tovarE.tovar_ID         = 0;
   tovarE.kod_ekka         = 0;
   tovarE.price            = 0;
   tovarE.quantity_real    = 0;
   tovarE.suma_real        = 0;
   tovarE.suma_no_discount = 0;
}
//---------------------------------------------------------------------------

void TShiftSQL::InsertDiscountHistoryByShift( int shift_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   DUniQuery *Query_sel = new DUniQuery( pConnect );
   try {
      //
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add( String("")+
         " INSERT INTO r_discount_card_history ( zakaz_id, discount_card_id, suma_discount, date_z ) "+
         "                              VALUES (:zakaz_id,:discount_card_id,:suma_discount,:date_z ) " );
      //
      Query_sel->Close();
      Query_sel->SQL->Clear();
      Query_sel->SQL->Add( String("")+
         " SELECT zakaz_id, discount_card_id, if(repayment,-suma_discount,suma_discount) as suma_discount, date_z FROM r_zakaz "+
         " WHERE shift_id = :shift_id AND discount_card_id!=0 " );
      Query_sel->ParamByName("shift_id")->AsInteger = shift_id;
      Query_sel->Open();
      while( !Query_sel->Eof ) {
         Query_tmp->ParamByName("zakaz_id")->AsInteger         = Query_sel->FieldByName("zakaz_id")->AsInteger;
         Query_tmp->ParamByName("discount_card_id")->AsInteger = Query_sel->FieldByName("discount_card_id")->AsInteger;
         Query_tmp->ParamByName("suma_discount")->AsFloat      = Query_sel->FieldByName("suma_discount")->AsFloat;
         Query_tmp->ParamByName("date_z")->AsDate              = StrToDate(Query_sel->FieldByName("date_z")->AsString);
         Query_tmp->ExecSQL();
         //
         Query_sel->Next();
      }
      Query_sel->Close();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка InsertDiscountHistoryByShift()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   delete Query_sel;
}
//---------------------------------------------------------------------------

void TShiftSQL::DeleteDiscountHistoryByShift( int shift_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   DUniQuery *Query_sel = new DUniQuery( pConnect );
   try {
      // Вилучити оплати документа
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add( String("")+
         " DELETE FROM r_discount_card_history "+
         " WHERE zakaz_id=:zakaz_id " );
      //
      Query_sel->Close();
      Query_sel->SQL->Clear();
      Query_sel->SQL->Add( String("")+
         " SELECT rh.zakaz_id FROM r_discount_card_history rh "+
         "    LEFT JOIN r_zakaz z ON rh.zakaz_id=z.zakaz_id "+
         " WHERE z.shift_id = :shift_id" );
      Query_sel->ParamByName("shift_id")->AsInteger = shift_id;
      Query_sel->Open();
      while( !Query_sel->Eof ) {
         Query_tmp->ParamByName("zakaz_id")->AsInteger = Query_sel->FieldByName("zakaz_id")->AsInteger;
         Query_tmp->ExecSQL();
         //
         Query_sel->Next();
      }
      Query_sel->Close();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка DeleteDiscountHistoryByShift()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   delete Query_sel;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadDocHistoryList( vector <DOC_HISTORY> &docHistoryList, int doc_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT s_h.*, s.name as status_name, t.name as tovar_name FROM r_status_history s_h "+
         "    LEFT JOIN r_statuses s ON s.status_id = s_h.status_id "+
         "    LEFT JOIN t_tovar t ON s_h.tovar_id = t.tovar_id "+
         " WHERE doc_id=:doc_id "+
         " ORDER BY date_d,status_history_id " );
      //
      Query_tmp->ParamByName("doc_id")->AsInteger = doc_id;
      Query_tmp->Open();
      docHistoryList.clear();
      DOC_HISTORY docHistoryE;
      while( !Query_tmp->Eof ) {
         ClearDocHistory(docHistoryE);
         //
         docHistoryE.status_history_ID = Query_tmp->FieldByName("status_history_id")->AsInteger;
         //
         docHistoryE.doc_ID    = Query_tmp->FieldByName("doc_id")->AsInteger;
         docHistoryE.date_d    = Query_tmp->FieldByName("date_d")->AsString;
         docHistoryE.user_name = Query_tmp->FieldByName("user_name")->AsString;
         docHistoryE.comment   = RecodeComment(Query_tmp->FieldByName("status_name")->AsString,
                                      Query_tmp->FieldByName("param_1")->AsString,
                                      Query_tmp->FieldByName("param_2")->AsString,
                                      Query_tmp->FieldByName("param_3")->AsString,
                                      Query_tmp->FieldByName("param_4")->AsString,
                                      Query_tmp->FieldByName("param_5")->AsString );

         docHistoryE.tovar_ID   = Query_tmp->FieldByName("tovar_id")->AsInteger;
         docHistoryE.tovar_name = Query_tmp->FieldByName("tovar_name")->AsString;
         //
         docHistoryList.push_back(docHistoryE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка LoadDocHistoryList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::ClearDocHistory( DOC_HISTORY &docHistoryE )
{
   docHistoryE.status_history_ID = 0;
   docHistoryE.doc_ID     = 0;
   docHistoryE.date_d     = "";
   docHistoryE.user_name  = "";
   docHistoryE.comment    = "";
   docHistoryE.tovar_ID   = 0;
   docHistoryE.tovar_name = "";
}
//---------------------------------------------------------------------------

String TShiftSQL::RecodeComment(String name, String p1, String p2, String p3, String p4, String p5)
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

int TShiftSQL::GetDepartmentInfo( String &department_name )
{
   int dep_id = 0;
   //
   if ( LOGIN == TDBService::get_from_ID("const_default","strvalue","name_const","SUPER_USER") ) return 0;
   //
   if( !pConnect->IsConnecting )
      return 0;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add( String("")+
         " SELECT d.dep_id, a.name "+
         " FROM users u "+
         "    LEFT JOIN employee e ON (u.employee_id=e.employee_id) "+
         "    LEFT JOIN department d ON (e.dep_id=d.dep_id)"+
         "    LEFT JOIN agents a ON (d.agent_id=a.agent_id)"+
         " WHERE user_login=:pLogin");
      Query_tmp->ParamByName("pLogin")->AsString = LOGIN;
      Query_tmp->Open();
      //
      if( !Query_tmp->Eof ) {
         dep_id = Query_tmp->FieldByName("dep_id")->AsInteger;
         department_name = Query_tmp->FieldByName("name")->AsString;
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка GetDepartmentInfo()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   //
   return dep_id;
}
//---------------------------------------------------------------------------
// Функції роботи з типами оплат (роздріб)
//---------------------------------------------------------------------------

void TShiftSQL::LoadTypeCalculationList( vector <TYPE_CALCULATION> &calculationList )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add( String("")+
         " SELECT tc.*, p.name as point_of_money_name "+
         " FROM r_type_calculation tc "+
         "    LEFT JOIN point_of_money p ON tc.point_of_money_id=p.point_of_money_id "
         " ORDER BY tc.type_calculation_id" );
      Query_tmp->Open();
      calculationList.clear();
      TYPE_CALCULATION calculationE;
      while( !Query_tmp->Eof ){
         ClearTypeCalculation(calculationE);
         //
         calculationE.type_calculation_ID = Query_tmp->FieldByName("type_calculation_id")->AsInteger;
         calculationE.name                = Query_tmp->FieldByName("name")->AsString;
         calculationE.use_flag            = Query_tmp->FieldByName("use_flag")->AsInteger;
         calculationE.type_oplat          = Query_tmp->FieldByName("type_oplat")->AsInteger;
         calculationE.point_of_money_id   = Query_tmp->FieldByName("point_of_money_id")->AsInteger;
         calculationE.point_of_money_name = Query_tmp->FieldByName("point_of_money_name")->AsString;
         //
         calculationList.push_back(calculationE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadTypeCalculationList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::SaveTypeCalculation( TYPE_CALCULATION &calculationE )
{
   if( !pConnect->IsConnecting )
      return;
   //
   if( !calculationE.type_calculation_ID ) return; // типи оплат можна лише редагувати

   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(" UPDATE r_type_calculation SET use_flag=:use_flag, type_oplat=:type_oplat, point_of_money_id=:point_of_money_id WHERE type_calculation_id=:type_calculation_id" );
      Query_tmp->ParamByName("use_flag")->AsInteger            = calculationE.use_flag;
      Query_tmp->ParamByName("type_oplat")->AsInteger          = calculationE.type_oplat;
      Query_tmp->ParamByName("point_of_money_id")->AsInteger   = calculationE.point_of_money_id;
      Query_tmp->ParamByName("type_calculation_id")->AsInteger = calculationE.type_calculation_ID;
      Query_tmp->ExecSQL();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка SaveTypeCalculation()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::ClearTypeCalculation( TYPE_CALCULATION &calculationE )
{
   calculationE.type_calculation_ID   = 0;
   calculationE.name                  = "";
   calculationE.use_flag              = 0;
   calculationE.type_oplat            = 0;
   calculationE.point_of_money_id     = 0;
   calculationE.point_of_money_name   = "";
}
//---------------------------------------------------------------------------

bool TShiftSQL::ReadNaklAgentToVector( int agent_id, vector <int> &zakazID, String date_doc, int shift_id )
{
   if( !pConnect->IsConnecting )
      return false;
   //
   int default_agent_id = atoi(TDBService::get_from_ID("firms","agent_id","firm_id",TDBService::get_from_ID("const_default","strvalue","name_const","CUSTOMER_DEF")).c_str());
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT z.zakaz_id "+
         " FROM r_zakaz z "+
         " WHERE z.shift_id=:shift_id AND z.stan=1 " );
      if( agent_id == default_agent_id ) // Для агента по замовчуванню читаємо і чеки з agent_id=0
         Query_tmp->SQL->Add( "   AND ( z.agent_id=:agent_id OR z.agent_id=0) " );
      else
         Query_tmp->SQL->Add( "   AND z.agent_id=:agent_id " );
      Query_tmp->SQL->Add(String("")+
         "   AND z.date_z=:date_z "+
         " ORDER BY z.zakaz_id " );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->ParamByName("agent_id")->AsInteger = agent_id;
      Query_tmp->ParamByName("date_z")->AsDate = StrToDate(date_doc);
      Query_tmp->Open();
      //
      zakazID.clear();
      while( !Query_tmp->Eof ){
         zakazID.push_back(Query_tmp->FieldByName("zakaz_id")->AsInteger);
         //
         Query_tmp->Next();
      }
      Query_tmp->Close();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних ReadNaklAgentToVector()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   //
   return true;
}
//---------------------------------------------------------------------------

bool TShiftSQL::GetMultyNaklAgent( int agent_id )
{
   if( !pConnect->IsConnecting )
      return false;
   //
   bool result = false;
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(" SELECT multy_nakl FROM firms WHERE agent_id=:agent_id " );
      //
      Query_tmp->ParamByName("agent_id")->AsInteger = agent_id;
      Query_tmp->Open();
      //
      if( !Query_tmp->Eof )
         result = Query_tmp->FieldByName("multy_nakl")->AsInteger;
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка GetMultyNaklAgent()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   //
   return result;
}
//---------------------------------------------------------------------------
// Функції роботи з автоматичними документами
//---------------------------------------------------------------------------

void TShiftSQL::LoadShiftDocList( vector <ZAKAZ> &zakazList, int shift_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rz.*, a.name as agent_name, if(a.type_ag_id IS NOT NULL,a.type_ag_id,0 ) as type_ag_id, "+
         "        ae.name as user_name, o.name as oper_name, d.number_d as doc_number "+
         " FROM r_shift_doc rz "+
         "    LEFT JOIN agents a     ON rz.agent_id = a.agent_id "+
         "    LEFT JOIN employee e   ON rz.employee_id  = e.employee_id "+
         "    LEFT JOIN agents ae    ON e.agent_id = ae.agent_id "+
         "    LEFT JOIN operation o  ON rz.oper_id = o.oper_id "+
         "    LEFT JOIN document d   ON rz.doc_id = d.doc_id "+
         " WHERE  shift_id=:shift_id " );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      zakazList.clear();
      ZAKAZ zakazE;
      while( !Query_tmp->Eof ){
         ClearZakaz(zakazE);
         //
         zakazE.zakaz_ID             = Query_tmp->FieldByName("shift_doc_id")->AsInteger;
         zakazE.employee_id          = Query_tmp->FieldByName("employee_id")->AsInteger;
         zakazE.user_name            = Query_tmp->FieldByName("user_name")->AsString;
         zakazE.agent_id             = Query_tmp->FieldByName("agent_id")->AsInteger;
         zakazE.agent_name           = Query_tmp->FieldByName("agent_name")->AsString;
         zakazE.type_ag_id           = Query_tmp->FieldByName("type_ag_id")->AsInteger;
         zakazE.shift_id             = Query_tmp->FieldByName("shift_id")->AsInteger;
         zakazE.number               = Query_tmp->FieldByName("number_d")->AsInteger;
         zakazE.date_z               = Query_tmp->FieldByName("date_d")->AsString;
         zakazE.time_open            = Query_tmp->FieldByName("time_open")->AsString;
         zakazE.time_close           = Query_tmp->FieldByName("time_close")->AsString;
         zakazE.stan                 = Query_tmp->FieldByName("stan")->AsInteger;
         zakazE.suma                 = Query_tmp->FieldByName("suma")->AsFloat;
         zakazE.suma_discount        = Query_tmp->FieldByName("suma_discount")->AsFloat;
         zakazE.znyzka               = Query_tmp->FieldByName("percent_discount")->AsFloat;
         zakazE.coment               = Query_tmp->FieldByName("coment")->AsString;
         //
         zakazE.oper_id              = Query_tmp->FieldByName("oper_id")->AsInteger;
         zakazE.oper_name            = Query_tmp->FieldByName("oper_name")->AsString;
         zakazE.doc_id               = Query_tmp->FieldByName("doc_id")->AsInteger;
         zakazE.doc_number           = Query_tmp->FieldByName("doc_number")->AsInteger;
         //
         zakazList.push_back(zakazE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftDocList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadShiftDocRowList( vector <ZAKAZ_ROW> &rowList, int shift_doc_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      int nesting_group  = atoi(TDBService::get_from_ID("const_default","strvalue","NAME_CONST","NESTING_GROUP").c_str());
      int show_vyrobnyk  = atoi(TDBService::get_from_ID("const_default","strvalue","NAME_CONST","SHOW_VYROBNYK").c_str());
      Query_tmp->SQL->Add(String("")+
         " SELECT rz.*, t.name as name_tovar, t.group_tov_id, t.make_flag, gt.type_group as type_tovar, v.name as vyrobnyk_name, t.kod_ekka, o.short_name as ov_name, t.ov_id "+
         " FROM r_shift_doc_row rz "+
         "    LEFT JOIN t_tovar t  ON rz.tovar_id = t.tovar_id "+
         "    LEFT JOIN t_ov o     ON t.ov_id = o.ov_id "+
         "    LEFT JOIN t_group_tov gt ON t.group_tov_id = gt.group_tov_id "+
         "    LEFT JOIN vyrobnyk v ON t.vyrobnyk_id = v.vyrobnyk_id "+
         " WHERE shift_doc_id=:shift_doc_id " );
      //
      Query_tmp->ParamByName("shift_doc_id")->AsInteger = shift_doc_id;
      Query_tmp->Open();
      rowList.clear();
      ZAKAZ_ROW rowE;
      while( !Query_tmp->Eof ){
         ClearZakazRow(rowE);
         //
         rowE.zakaz_row_ID         = Query_tmp->FieldByName("shift_doc_row_id")->AsInteger;
         rowE.zakaz_ID	           = Query_tmp->FieldByName("shift_doc_id")->AsInteger;
         rowE.tovar_ID             = Query_tmp->FieldByName("tovar_id")->AsInteger;
         rowE.name_tovar           = (nesting_group?group.group_GET(Query_tmp->FieldByName("group_tov_id")->AsInteger)+" ":String(""))+
                                     (show_vyrobnyk?Query_tmp->FieldByName("vyrobnyk_name")->AsString+" ":String(""))+
                                     Query_tmp->FieldByName("name_tovar")->AsString;
         rowE.kod_ekka             = Query_tmp->FieldByName("kod_ekka")->AsInteger;
         rowE.ov_ID                = Query_tmp->FieldByName("ov_id")->AsInteger;
         rowE.ov_name              = Query_tmp->FieldByName("ov_name")->AsString;
         rowE.price                = Query_tmp->FieldByName("price")->AsFloat;
         rowE.quantity             = Query_tmp->FieldByName("quantity")->AsFloat;
         rowE.suma                 = Query_tmp->FieldByName("suma")->AsFloat;
         rowE.suma_discount        = Query_tmp->FieldByName("suma_discount")->AsFloat;
         //
         rowE.type_tovar           = Query_tmp->FieldByName("type_tovar")->AsInteger;
         rowE.make_flag            = Query_tmp->FieldByName("make_flag")->AsInteger;
         //
         rowList.push_back(rowE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftDocRowList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadShiftDocCalculationList( vector <ZAKAZ_CALCULATION> &calculationList, int shift_doc_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rz.*, t.name as type_calculation_name, t.type_oplat, t.point_of_money_id "+
         " FROM r_shift_doc_calculation rz "+
         "    LEFT JOIN r_type_calculation t ON rz.type_calculation_id = t.type_calculation_id "+
         "    LEFT JOIN r_shift_doc z ON rz.shift_doc_id = z.shift_doc_id "+
         " WHERE rz.shift_doc_id=:shift_doc_id " );
      //
      Query_tmp->ParamByName("shift_doc_id")->AsInteger = shift_doc_id;
      Query_tmp->Open();
      calculationList.clear();
      ZAKAZ_CALCULATION calculationE;
      while( !Query_tmp->Eof ){
         ClearZakazCalculation(calculationE);
         //
         calculationE.zakaz_calculation_ID  = Query_tmp->FieldByName("shift_doc_calculation_id")->AsInteger;
         calculationE.zakaz_ID	            = Query_tmp->FieldByName("shift_doc_id")->AsInteger;
         calculationE.type_calculation_ID   = Query_tmp->FieldByName("type_calculation_id")->AsInteger;
         calculationE.type_calculation_name = Query_tmp->FieldByName("type_calculation_name")->AsString;
         calculationE.suma                  = Query_tmp->FieldByName("suma")->AsFloat;
         calculationE.info                  = Query_tmp->FieldByName("info")->AsString;
         calculationE.type_oplat            = Query_tmp->FieldByName("type_oplat")->AsInteger;
         calculationE.point_of_money_id     = Query_tmp->FieldByName("point_of_money_id")->AsInteger;
         //
         calculationList.push_back(calculationE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftDocCalculationList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadShiftShiftDocCalculationList( vector <ZAKAZ_CALCULATION> &calculationList, int shift_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rz.type_calculation_id, t.name as type_calculation_name, t.type_oplat, t.point_of_money_id, SUM(z.suma) as suma  "+
         " FROM r_shift_doc_calculation rz "+
         "    LEFT JOIN r_type_calculation t ON rz.type_calculation_id = t.type_calculation_id "+
         "    LEFT JOIN r_shift_doc z ON rz.shift_doc_id = z.shift_doc_id "+
         " WHERE z.shift_id=:shift_id "+
         " GROUP BY rz.type_calculation_id "+
         " ORDER BY t.name " );
      //
      Query_tmp->ParamByName("shift_id")->AsInteger = shift_id;
      Query_tmp->Open();
      calculationList.clear();
      ZAKAZ_CALCULATION calculationE;
      while( !Query_tmp->Eof ){
         ClearZakazCalculation(calculationE);
         //
         calculationE.type_calculation_ID   = Query_tmp->FieldByName("type_calculation_id")->AsInteger;
         calculationE.type_calculation_name = Query_tmp->FieldByName("type_calculation_name")->AsString;
         calculationE.suma                  = Query_tmp->FieldByName("suma")->AsFloat;
         calculationE.type_oplat            = Query_tmp->FieldByName("type_oplat")->AsInteger;
         calculationE.point_of_money_id     = Query_tmp->FieldByName("point_of_money_id")->AsInteger;
         //
         calculationList.push_back(calculationE);
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadShiftShiftDocCalculationList()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

int TShiftSQL::CreateDocumentFromShiftDoc( )
{
   if( !pConnect->IsConnecting )
      return 0;
   // 1.Вибираємо всі несформовані документи з незакритих змін
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   int num_doc = 0;
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT rz.*, a.name as agent_name, if(a.type_ag_id IS NOT NULL,a.type_ag_id,0 ) as type_ag_id, ae.name as user_name, d.agent_id as dep_agent_id "+
         " FROM r_shift_doc rz "+
         "    LEFT JOIN agents a     ON rz.agent_id    = a.agent_id "+
         "    LEFT JOIN employee e   ON rz.employee_id = e.employee_id "+
         "    LEFT JOIN agents ae    ON e.agent_id  = ae.agent_id "+
         "    LEFT JOIN r_shift s    ON rz.shift_id = s.shift_id "+
         "    LEFT JOIN department d ON s.dep_id    = d.dep_id "+
         " WHERE  doc_id=0 AND s.stan!=2 AND rz.stan=1 " );
      //
      Query_tmp->Open();
      while( !Query_tmp->Eof ) {
         // 2.Формуємо документи по кожному запису shift_doc
         int document_ID = CreateShiftDocNakl( Query_tmp->FieldByName("shift_doc_id")->AsInteger,
                                               Query_tmp->FieldByName("date_d")->AsString,
                                               Query_tmp->FieldByName("time_close")->AsString,
                                               Query_tmp->FieldByName("oper_id")->AsInteger,
                                               Query_tmp->FieldByName("dep_agent_id")->AsInteger,
                                               Query_tmp->FieldByName("agent_id")->AsInteger,
                                               Query_tmp->FieldByName("coment")->AsString+String(" Документ №")+Query_tmp->FieldByName("number_d")->AsString+
                                               "  Зміна №"+Query_tmp->FieldByName("shift_id")->AsString,
                                               TDBService::get_from_ID("users","user_login","employee_id",Query_tmp->FieldByName("employee_id")->AsString) );
         //
         SEND_MSG(WM_UpdateDocument,0,document_ID);
         //
         num_doc++;
         //
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка CreateDocumentFromShiftDoc()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   //
   delete Query_tmp;
   //
   return num_doc;
}
//---------------------------------------------------------------------------

int TShiftSQL::CreateShiftDocNakl( int shift_doc_id,
           String date_doc, String time_doc, int oper_id, int dep_agent_id, int agent_id, String coment, String user_name )
{
   if( !pConnect->IsConnecting )
      return 0;
   //
   int doc_tov_ID = 0;
   int number_d = 0;
   // Згідно операції визначаємо агентів
   int agent_from_id = 0;
   int agent_to_id   = 0;
   //
   String code_oper = TDBService::get_from_ID("operation","code_oper","oper_id",String(oper_id));
   if ( code_oper == "INVENTAR_OPYS"  )
      return CreateShiftDocNaklInventory( shift_doc_id, date_doc, time_doc, oper_id, dep_agent_id, coment );
   //
   if ( code_oper == "REAL_STORE"  || code_oper == "MOVE_GP" ||
        code_oper == "MOVE_VIDDIL" || code_oper == "MOVE_STORE" ||
        code_oper == "PUT_VIDDIL"  || code_oper == "PUT_BACK" ||
        code_oper == "BACK_POST" ) {
      agent_from_id = dep_agent_id;
      agent_to_id   = agent_id;
   }
   else if ( code_oper == "GET_TOVAR" || code_oper == "BACK_CUSTS" || code_oper == "BACK_CUSTV" ) {
      agent_from_id = agent_id;
      agent_to_id   = dep_agent_id;
   }
   else if ( code_oper == "SPYS_VIDDIL" || code_oper == "SPYS_STORE" || code_oper == "REQUEST" ) {
      agent_from_id = dep_agent_id;
   }
   else {
      ShowMessage("Невідома операція '"+code_oper+"'("+String(oper_id)+")");
   }
   //
   int type_oblik_from = atoi(TDBService::get_from_ID("department","type_oblik","agent_id",String(agent_from_id)).c_str());
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      // Завантажимо рядки документа
      vector <ZAKAZ_ROW> rowList;
      LoadShiftDocRowList( rowList, shift_doc_id );
      if( rowList.size() ) { // Непорожній документ
         // Підрахувати суму по документу
         double suma_doc = 0, suma_discount = 0;
         for( int n = 0; n < (int)rowList.size(); n++ ) {
            suma_doc += rowList[n].suma;
            suma_discount += rowList[n].suma_discount;
         }
         //
         // 1. Записати дані в таблицю document
         //
         Query_tmp->Close();
         Query_tmp->SQL->Clear();
         Query_tmp->SQL->Add(String("")+
            " INSERT INTO document ( agent_id, agent_to_id,  pryzn, currency_id, course, number_d, date_d, time_d, suma, suma_add, rest, pdv, stan, price_id, user, author, type_doc_id, oper_id, number_doc ) "+
            " VALUES(:agent_from_id,:agent_to_id,:coment,1,:currency_id, :number_d, :date_d,:time_d,:suma,:suma_add,:suma, 0,0,:price_id,:user,:author,:type_doc_id, :oper_id, :number_doc ) " );
         //
         number_d = TSubMethods::NewNumberDocMaterial(oper_id,date_doc);
         Query_tmp->ParamByName("agent_from_id")->AsInteger = agent_from_id;
         Query_tmp->ParamByName("agent_to_id")->AsInteger   = agent_to_id;
         Query_tmp->ParamByName("number_doc")->AsString     = shift_doc_id;
         Query_tmp->ParamByName("coment")->AsString         = coment;
         Query_tmp->ParamByName("number_d")->AsInteger      = number_d;
         Query_tmp->ParamByName("date_d")->AsDate           = StrToDate(date_doc);
         Query_tmp->ParamByName("time_d")->AsString         = time_doc;
         Query_tmp->ParamByName("suma")->AsFloat            = suma_doc;
         Query_tmp->ParamByName("suma_add")->AsFloat        = suma_discount;
         Query_tmp->ParamByName("price_id")->AsInteger      = 0;
         Query_tmp->ParamByName("user")->AsString           = (user_name==""?LOGIN:user_name);
         Query_tmp->ParamByName("author")->AsString         = (user_name==""?LOGIN:user_name);
         Query_tmp->ParamByName("type_doc_id")->AsInteger   = 13;
         Query_tmp->ParamByName("oper_id")->AsInteger       = oper_id;
         Query_tmp->ParamByName("currency_id")->AsInteger   = StrToIntDef(TDBService::get_from_ID("const_default","strvalue","name_const","CURDEFAULT").c_str(),0);
         //
         Query_tmp->ExecSQL();
         //
         doc_tov_ID = Query_tmp->LastInsertID();
         //
         // Зберегти рядки
         //
         double price = 0, price_no_discount = 0, cur_price = 0;
         int QUANTITY_SCALE  = StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "QUANTITYSCALE", "3" ));
         int PRICE_OUT_SCALE = StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "PRICEOUTSCALE", "2" ));
         int PRICE_IN_SCALE  = StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "PRICEOUTSCALE", "3" ));
         //
         Query_tmp->Close();
         Query_tmp->SQL->Clear();
         Query_tmp->SQL->Add(String("")+
            " INSERT INTO t_ruh_tovar( doc_id, tovar_id, quantity, price, suma, price_add, suma_add, price_obl, suma_obl, discount, correction_price, order_row, koef, quantity_ov, debet_id, kredyt_id) "+
            "                  VALUES(:doc_id,:tovar_id,:quantity,:price,:suma,:price_add,:suma_add,:price_obl,:suma_obl,:discount,:correction_price,0,    1,:quantity   ,:debet_id,:kredyt_id) ");
         for(int i = 0; i < (int)rowList.size(); i++) {
            Query_tmp->ParamByName("doc_id")->AsInteger   = doc_tov_ID;
            Query_tmp->ParamByName("tovar_id")->AsInteger = rowList[i].tovar_ID;
            Query_tmp->ParamByName("quantity")->AsFloat   = TFormatingData::scaleDbl(rowList[i].quantity,QUANTITY_SCALE);
            Query_tmp->ParamByName("price_obl")->AsFloat  = 0;
            Query_tmp->ParamByName("suma_obl")->AsFloat   = 0;
            //
            if( rowList[i].quantity ) {
               price = rowList[i].suma_discount/rowList[i].quantity;
               price_no_discount = rowList[i].suma/rowList[i].quantity;
            }
            else {
               price = 0;
               price_no_discount = 0;
            }
            cur_price = TWorkDocTov::GetPriceDepCurDate(rowList[i].tovar_ID,dep_agent_id,date_doc,time_doc, true);
            if ( !cur_price )
               cur_price = price_no_discount; // price; 18-06-2014
            //
            // РАХУНКИ ТА ЦІНИ
            if ( code_oper == "GET_TOVAR" ) {
               Query_tmp->ParamByName("price")->AsFloat    = TFormatingData::scaleDbl(cur_price,PRICE_IN_SCALE);
               Query_tmp->ParamByName("suma")->AsFloat     = TFormatingData::scaleDbl(cur_price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("price_obl")->AsFloat = TFormatingData::scaleDbl(price,PRICE_IN_SCALE);
               Query_tmp->ParamByName("suma_obl")->AsFloat  = TFormatingData::scaleDbl(price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("price_add")->AsFloat   = TFormatingData::scaleDbl(price,PRICE_IN_SCALE);
               Query_tmp->ParamByName("suma_add")->AsFloat    = TFormatingData::scaleDbl(price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("discount")->AsFloat    = 0;
               Query_tmp->ParamByName("correction_price")->AsFloat = 0;
               //
               Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_to_id,rowList[i].tovar_ID);
               Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_to_id,"GET_KREDYT");
            }
            else if ( code_oper == "BACK_POST" ) {
               Query_tmp->ParamByName("price")->AsFloat    = TFormatingData::scaleDbl(cur_price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma")->AsFloat     = TFormatingData::scaleDbl(cur_price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("price_add")->AsFloat   = TFormatingData::scaleDbl(price,PRICE_IN_SCALE);
               Query_tmp->ParamByName("suma_add")->AsFloat    = TFormatingData::scaleDbl(rowList[i].suma_discount);
               //
               Query_tmp->ParamByName("discount")->AsFloat    = 0;
               Query_tmp->ParamByName("correction_price")->AsFloat = 0;
               //
               Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_from_id,"GET_KREDYT");
               Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_from_id,rowList[i].tovar_ID);
            }
            else if ( code_oper == "MOVE_VIDDIL" || code_oper == "MOVE_STORE" || code_oper == "PUT_VIDDIL"|| code_oper == "PUT_BACK" ) {
               Query_tmp->ParamByName("price")->AsFloat    = TFormatingData::scaleDbl(cur_price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma")->AsFloat     = TFormatingData::scaleDbl(cur_price * rowList[i].quantity,2);
               //
               cur_price = TWorkDocTov::GetPriceDepCurDate(rowList[i].tovar_ID,agent_to_id,date_doc,time_doc, true);
               Query_tmp->ParamByName("price_add")->AsFloat   = TFormatingData::scaleDbl(cur_price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma_add")->AsFloat    = TFormatingData::scaleDbl(cur_price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("discount")->AsFloat    = 0;
               Query_tmp->ParamByName("correction_price")->AsFloat = 0;
               //
               Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_to_id,rowList[i].tovar_ID);
               Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_from_id,rowList[i].tovar_ID);
            }
            else if ( code_oper == "SPYS_VIDDIL" || code_oper == "SPYS_STORE"  || code_oper == "REQUEST") {
               Query_tmp->ParamByName("price")->AsFloat    = TFormatingData::scaleDbl(cur_price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma")->AsFloat     = TFormatingData::scaleDbl(cur_price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("price_add")->AsFloat   = TFormatingData::scaleDbl(cur_price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma_add")->AsFloat    = TFormatingData::scaleDbl(cur_price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("discount")->AsFloat    = 0;
               Query_tmp->ParamByName("correction_price")->AsFloat = 0;
               //
               Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_from_id,"SPYS");
               Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_from_id,rowList[i].tovar_ID);
            }
            else if ( (code_oper == "REAL_STORE" || code_oper == "MOVE_GP") && type_oblik_from ) {
               Query_tmp->ParamByName("price")->AsFloat    = TFormatingData::scaleDbl(cur_price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma")->AsFloat     = TFormatingData::scaleDbl(cur_price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("price_add")->AsFloat   = TFormatingData::scaleDbl(price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma_add")->AsFloat    = TFormatingData::scaleDbl(rowList[i].suma_discount);
               //
               Query_tmp->ParamByName("discount")->AsFloat    = TFormatingData::scaleDbl(rowList[i].suma - rowList[i].suma_discount);
               Query_tmp->ParamByName("correction_price")->AsFloat = TFormatingData::scaleDbl(TFormatingData::scaleDbl(cur_price * rowList[i].quantity) - rowList[i].suma);
               //
               Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_from_id,"REAL_DEBET");
               Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_from_id,rowList[i].tovar_ID);
            }
            else if ( (code_oper == "REAL_STORE" || code_oper == "MOVE_GP") && !type_oblik_from ) {
               Query_tmp->ParamByName("price")->AsFloat    = TFormatingData::scaleDbl(price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma")->AsFloat     = TFormatingData::scaleDbl(price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("price_add")->AsFloat   = TFormatingData::scaleDbl(price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma_add")->AsFloat    = TFormatingData::scaleDbl(price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("discount")->AsFloat    = 0;
               Query_tmp->ParamByName("correction_price")->AsFloat = 0;
               //
               Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_from_id,"REAL_DEBET");
               Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_from_id,rowList[i].tovar_ID);
            }
            else if ( code_oper == "BACK_CUSTS" || code_oper == "BACK_CUSTV" ) {
               Query_tmp->ParamByName("price")->AsFloat    = TFormatingData::scaleDbl(cur_price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma")->AsFloat     = TFormatingData::scaleDbl(cur_price * rowList[i].quantity,2);
               //
               Query_tmp->ParamByName("price_add")->AsFloat   = TFormatingData::scaleDbl(price,PRICE_OUT_SCALE);
               Query_tmp->ParamByName("suma_add")->AsFloat    = TFormatingData::scaleDbl(rowList[i].suma_discount);
               //
               Query_tmp->ParamByName("discount")->AsFloat    = TFormatingData::scaleDbl(rowList[i].suma - rowList[i].suma_discount);
               Query_tmp->ParamByName("correction_price")->AsFloat = TFormatingData::scaleDbl(TFormatingData::scaleDbl(cur_price * rowList[i].quantity) - rowList[i].suma);
               //
               Query_tmp->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(agent_to_id,rowList[i].tovar_ID);
               Query_tmp->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(agent_to_id,"GET_KREDYT");
            }
            else {
               Query_tmp->ParamByName("price")->AsFloat    = 0;
               Query_tmp->ParamByName("suma")->AsFloat     = 0;
               //
               Query_tmp->ParamByName("price_add")->AsFloat   = 0;
               Query_tmp->ParamByName("suma_add")->AsFloat    = 0;
               //
               Query_tmp->ParamByName("discount")->AsFloat    = 0;
               Query_tmp->ParamByName("correction_price")->AsFloat = 0;
               //
               Query_tmp->ParamByName("debet_id")->AsInteger  = 0;
               Query_tmp->ParamByName("kredyt_id")->AsInteger = 0;
               //
               ShowMessage("Невідома операція '"+code_oper+"'("+String(oper_id)+")");
            }
            //
            Query_tmp->ExecSQL();
            //
            //
            //
            if ( rowList[i].type_tovar == 7 && !rowList[i].make_flag ) { // Комплект
               TSubMethods::SaveKomplektTovar(doc_tov_ID, code_oper == "BACK_CUSTV" || code_oper == "BACK_CUSTS" ? agent_to_id : agent_from_id,rowList[i].tovar_ID,rowList[i].quantity,Query_tmp->LastInsertID(),date_doc,time_doc);
            }
         }
      }
      else {
         doc_tov_ID = -1; // Порожній документ
      }
      rowList.clear();
      // 3. Записати дані в таблицю r_shift_doc
      //
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " UPDATE r_shift_doc SET doc_id=:document_id WHERE shift_doc_id=:shift_doc_id ");
      //
      Query_tmp->ParamByName("document_id")->AsInteger  = doc_tov_ID;
      Query_tmp->ParamByName("shift_doc_id")->AsInteger = shift_doc_id;
      Query_tmp->ExecSQL();
      //
      // При потребі зразу ж провести
      // При CreateDocument=2 документи не проводити
      int type_create_doc = atoi(TWorkFiles::GetDataFromIniFile( "Partner.ini", "General", "CreateDocument", "0" ).c_str());
      int prov_auto = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","PROV_AUTO").c_str());
      if ( prov_auto && type_create_doc!=2 ) {
         if ( !TSubMethods::IntoCurrentPeriod(date_doc) ) {
            ShowMessage("Дата документу №"+String(number_d)+" відноситься до закритого періоду!!!");
         }
         else {
            String ErrMsg = "";
            TMaterialDoc::ProvDoc(true,doc_tov_ID,(TProgressBar *) NULL,false,ErrMsg);
            if ( ErrMsg != "" )
               ShowMessage(ErrMsg);
         }
      }
      // Формуємо оплати по документу
      vector <ZAKAZ_CALCULATION> zakazCalculationList;
      LoadShiftDocCalculationList(zakazCalculationList,shift_doc_id);
      if( zakazCalculationList.size() ) {
         if ( code_oper == "REAL_STORE"  || code_oper == "MOVE_GP" || code_oper == "BACK_POST" ) {
            CreateShiftDocOplat( zakazCalculationList, 3, date_doc, dep_agent_id, agent_id, coment, doc_tov_ID );
         }
         else if ( code_oper == "GET_TOVAR" || code_oper == "BACK_CUSTS" || code_oper == "BACK_CUSTV" ) {
            CreateShiftDocOplat( zakazCalculationList, 1, date_doc, dep_agent_id, agent_id, coment, doc_tov_ID );
         }
      }
      zakazCalculationList.clear();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка CreateShiftDocNakl()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   return doc_tov_ID;
}
//---------------------------------------------------------------------------

void TShiftSQL::CreateShiftDocOplat( vector <ZAKAZ_CALCULATION> &total_calculation, int type_opl_id,
           String date_doc, int dep_agent_id, int agent_id, String coment, int doc_tov_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   int point_of_money_id = 0;
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      for( int i=0;i<(int)total_calculation.size();i++  ) {
         point_of_money_id = 0;
         // 1. По типу розрахунку та агентах визначаємо тип оплати та джерело коштів
         switch( total_calculation[i].type_oplat ) {
            case 0: // Оплату не створюємо
               continue;
            case 1: // Каса береться з агента складу(відділа) або з const_default
               point_of_money_id = atoi(TDBService::get_from_ID("department","point_of_money_id","agent_id",String(dep_agent_id)).c_str());
               if( !point_of_money_id )
                  point_of_money_id = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","KASA_DEF").c_str());
               break;
            case 2: // Фіктивна каса береться з типу розрахунку або з const_default
               point_of_money_id = total_calculation[i].point_of_money_id;
               if( !point_of_money_id )
                  point_of_money_id = atoi(TDBService::get_from_ID("const_default","strvalue","name_const","FKASA_DEF").c_str());
               break;
         }
         if( !point_of_money_id ) continue;
         // 2. Добавляємо оплату
         Query_tmp->Close();
         Query_tmp->SQL->Clear();
         Query_tmp->SQL->Add(String("")+
            " INSERT INTO oplata ( agent_from_id, number_d, date_d, info, suma, stan, type_opl, alive, order_id, checked_flag, agent_to_id, agent_id, department_id, date_changed, changed, sent, location, rest, user, author ) "+
            "        VALUES      (:agent_from_id,:number_d,:date_d,:info,:suma,:stan,:type_opl,     0,        0,            0,:agent_to_id,:agent_id,:department_id,        NOW(),       1,    0,        0,:rest,:user,:author ) " );
         //
         int number_d = TSubMethods::NewNumberDocOplata( type_opl_id, date_doc );
         Query_tmp->ParamByName("agent_from_id")->AsInteger = 0;
         Query_tmp->ParamByName("number_d")->AsInteger      = number_d;
         Query_tmp->ParamByName("date_d")->AsDate           = StrToDate(date_doc);
         Query_tmp->ParamByName("info")->AsString           = coment;
         Query_tmp->ParamByName("suma")->AsFloat            = total_calculation[i].suma;
         Query_tmp->ParamByName("stan")->AsInteger          = 1;
         Query_tmp->ParamByName("type_opl")->AsInteger      = type_opl_id;
         Query_tmp->ParamByName("agent_to_id")->AsInteger   = type_opl_id==1?agent_id:point_of_money_id;
         Query_tmp->ParamByName("agent_id")->AsInteger      = type_opl_id==1?point_of_money_id:agent_id;
         Query_tmp->ParamByName("department_id")->AsInteger = atoi(TDBService::get_from_ID("department","dep_id","agent_id",String(dep_agent_id)).c_str());
         Query_tmp->ParamByName("rest")->AsFloat            = fabs(total_calculation[i].suma);
         Query_tmp->ParamByName("user")->AsString           = LOGIN;
         Query_tmp->ParamByName("author")->AsString         = LOGIN;
         //
         Query_tmp->ExecSQL();
         //
         int oplata_id = Query_tmp->LastInsertID();
         // Добавляємо рядки
         if( total_calculation[i].type_calculation_ID == 1 || total_calculation[i].type_calculation_ID == 7 ) { // Готівка
            double suma_total = 0;
            for( int j=0; j<(int)total_calculation.size();j++  ) {
               if( total_calculation[j].type_calculation_ID !=1 && total_calculation[j].type_calculation_ID !=7 ) continue;
               //
               AddOplataRow( oplata_id, total_calculation[j].currency_ID, total_calculation[j].amount_currency, total_calculation[j].course, total_calculation[j].suma );
               suma_total += total_calculation[j].suma;
               total_calculation[j].suma = 0; // щоб наступного проходу пропустити
            }
            // Записуємо підраховану суму в оплату та залишок
            TDBService::update_double_field_ID("oplata","suma",suma_total,"oplata_id",String(oplata_id));
            TDBService::update_double_field_ID("oplata","rest",fabs(suma_total),"oplata_id",String(oplata_id));
         }
         else if( total_calculation[i].type_calculation_ID == atoi(TDBService::get_from_ID("const_default","strvalue","name_const","BONUS_TYPE_CALC").c_str()) ){ // Бонуси
            double suma_total = 0;
            for( int j=0; j<(int)total_calculation.size();j++  ) {
               if( total_calculation[j].type_calculation_ID != total_calculation[i].type_calculation_ID ) continue;
               //
               AddOplataRow( oplata_id, total_calculation[j].currency_ID, total_calculation[j].amount_currency, total_calculation[j].course, total_calculation[j].suma );
               suma_total += total_calculation[j].suma;
               total_calculation[j].suma = 0; // щоб наступного проходу пропустити
            }
            // Записуємо підраховану суму в оплату та залишок
            TDBService::update_double_field_ID("oplata","suma",suma_total,"oplata_id",String(oplata_id));
            TDBService::update_double_field_ID("oplata","rest",fabs(suma_total),"oplata_id",String(oplata_id));
         }
         else { // один рядок
            AddOplataRow( oplata_id, total_calculation[i].currency_ID, total_calculation[i].amount_currency, total_calculation[i].course, total_calculation[i].suma );
         }
         // і привязуємо до документа
         if( oplata_id && doc_tov_id ) {
            double rest_doc_tov = atof(TDBService::get_from_ID("document","rest","doc_id",String(doc_tov_id)).c_str());
            double rest_oplata  = atof(TDBService::get_from_ID("oplata",  "rest","oplata_id",String(oplata_id)).c_str());
            //
            Query_tmp->Close();
            Query_tmp->SQL->Clear();
            Query_tmp->SQL->Add(String("")+
               " INSERT INTO link_doc_opl ( doc_tov_id, oplata_id, suma ) "+
               "        VALUES            (:doc_tov_id,:oplata_id,:suma ) " );
            //
            Query_tmp->ParamByName("doc_tov_id")->AsInteger = doc_tov_id;
            Query_tmp->ParamByName("oplata_id")->AsInteger  = oplata_id;
            Query_tmp->ParamByName("suma")->AsFloat         = (rest_doc_tov>rest_oplata?rest_oplata:rest_doc_tov);
            //
            Query_tmp->ExecSQL();
            //
            TDBService::update_double_field_ID("document","rest",(rest_doc_tov>rest_oplata?rest_doc_tov-rest_oplata:0),"doc_id",String(doc_tov_id));
            TDBService::update_double_field_ID("oplata",  "rest",(rest_doc_tov>rest_oplata?0:rest_oplata-rest_doc_tov),"oplata_id",String(oplata_id));
         }
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка CreateShiftDocOplat()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   return;
}
//---------------------------------------------------------------------------

int TShiftSQL::CreateShiftDocNaklInventory( int shift_doc_id,
           String date_doc, String time_doc, int oper_id, int dep_agent_id, String coment )
{
   if( !pConnect->IsConnecting )
      return 0;
   //
   int doc_tov_ID = 0;
   int number_d = 0;
   int dep_id   = StrToIntDef(TDBService::get_from_ID("department", "dep_id","agent_id",String(dep_agent_id)).c_str(),0);
   // Завантажимо рядки документа
   vector <ZAKAZ_ROW> rowList;
   LoadShiftDocRowList( rowList, shift_doc_id );
   // Якщо немає рядків то накладну не створюємо
   if( !rowList.size() ) return 0; // Порожній документ
   //
   DUniQuery *Query_tmp   = new DUniQuery( pConnect );
   DUniQuery *Query_audit = new DUniQuery( pConnect );
   try {
      // Вибрати типи товарів по яких зроблена інвентаризація
      vector <int> type_tovar_list;
      for( int n = 0; n < (int)rowList.size(); n++ ) {
         int i = 0;
         for( ; i < (int)type_tovar_list.size(); i++ )
            if( type_tovar_list[i] == rowList[n].type_tovar ) break;
         if( i == (int)type_tovar_list.size() )
            type_tovar_list.push_back(rowList[n].type_tovar);
      }
      // По кожному з типів створюємо накладну
      for( int j=0;j<(int)type_tovar_list.size(); j++ ) {
         // Читаємо обєкт обліку для ревізії
         int bcount_id = GetBcountID( type_tovar_list[j], dep_id );
         // Підрахувати суму по документу
         double suma_doc = 0, suma_discount = 0;
         //
         // 1. Записати дані в таблицю document
         //
         Query_tmp->Close();
         Query_tmp->SQL->Clear();
         Query_tmp->SQL->Add(String("")+
            " INSERT INTO document ( agent_id, agent_to_id,  pryzn, currency_id, course, number_d, date_d, time_d, suma, suma_add, rest, pdv, stan, price_id, user, author, type_doc_id, oper_id, number_doc, type_pdv_id ) "+
            " VALUES(:agent_from_id,:agent_to_id,:coment,:currency_id,1, :number_d, :date_d,:time_d,:suma,:suma_add,:suma, 0,0,:price_id,:user,:author,:type_doc_id, :oper_id, :number_doc, :type_pdv_id ) " );
         //
         number_d = TSubMethods::NewNumberDocMaterial(oper_id,date_doc);
         Query_tmp->ParamByName("agent_from_id")->AsInteger = dep_agent_id;
         Query_tmp->ParamByName("agent_to_id")->AsInteger   = 0;
         Query_tmp->ParamByName("number_doc")->AsString     = shift_doc_id;
         Query_tmp->ParamByName("coment")->AsString         = coment;
         Query_tmp->ParamByName("number_d")->AsInteger      = number_d;
         Query_tmp->ParamByName("date_d")->AsDate           = StrToDate(date_doc);
         Query_tmp->ParamByName("time_d")->AsString         = time_doc;
         Query_tmp->ParamByName("suma")->AsFloat            = suma_doc;
         Query_tmp->ParamByName("suma_add")->AsFloat        = suma_discount;
         Query_tmp->ParamByName("price_id")->AsInteger      = 0;
         Query_tmp->ParamByName("user")->AsString           = LOGIN;
         Query_tmp->ParamByName("author")->AsString         = LOGIN;
         Query_tmp->ParamByName("type_doc_id")->AsInteger   = 13;
         Query_tmp->ParamByName("oper_id")->AsInteger       = oper_id;
         Query_tmp->ParamByName("type_pdv_id")->AsInteger   = bcount_id;
         Query_tmp->ParamByName("currency_id")->AsInteger   = StrToIntDef(TDBService::get_from_ID("const_default","strvalue","name_const","CURDEFAULT").c_str(),0);
         //
         Query_tmp->ExecSQL();
         //
         doc_tov_ID = Query_tmp->LastInsertID();
         //
         // Зберегти рядки документа та audit
         //
         // Читаємо залишки по складу/відділу на час ревізії
         DSklad sk(dep_agent_id,bcount_id,date_doc,date_doc,0,time_doc );
         sk.activate();
         //
         double price = 0, price_no_discount = 0, cur_price = 0;
         int QUANTITY_SCALE  = StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "QUANTITYSCALE", "3" ));
         int PRICE_OUT_SCALE = StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "PRICEOUTSCALE", "2" ));
         int PRICE_IN_SCALE  = StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "close_shift", "PRICEOUTSCALE", "3" ));
         //
         Query_tmp->Close();
         Query_tmp->SQL->Clear();
         Query_tmp->SQL->Add(String("")+
            " INSERT INTO t_ruh_tovar( doc_id, tovar_id, quantity, price, suma, koef, quantity_ov, debet_id, kredyt_id, ov_id) "+
            "                  VALUES(:doc_id,:tovar_id,:quantity,:price,:suma,    1,:quantity   ,:debet_id,:kredyt_id,:ov_id) ");
         //
         Query_audit->SQL->Clear();
         Query_audit->SQL->Add(String("")+
            " INSERT INTO t_audit( doc_id, tovar_id, price,  quantity, quantity_ov, suma, quantity_fact, quantity_fact_ov, suma_fact, koef, debet_id, kredyt_id, ov_id) "+
            "              VALUES(:doc_id,:tovar_id,:price, :quantity,:quantity   ,:suma,:quantity_fact,:quantity_fact   ,:suma_fact,    1,:debet_id,:kredyt_id,:ov_id) ");
         // Рухаємося по залишках і формуємо рядки ревізії та документу
         vector <DTovar>::iterator itTov;
         for(itTov = sk.tovarBegin(); itTov != sk.tovarEnd(); itTov++) {
            // Параметри ревізії
            Query_audit->ParamByName("doc_id")->AsInteger    = doc_tov_ID;
            Query_audit->ParamByName("tovar_id")->AsInteger  = (*itTov).tovarID_GET();
            Query_audit->ParamByName("debet_id")->AsInteger  = TSubMethods::GetBCountOfDep(dep_agent_id,"REGRADING");
            Query_audit->ParamByName("kredyt_id")->AsInteger = TSubMethods::GetBCountOfDep(dep_agent_id,(*itTov).tovarID_GET());
            Query_audit->ParamByName("ov_id")->AsInteger     = (*itTov).tovarOVID_GET();
            Query_audit->ParamByName("price")->AsFloat       = TFormatingData::scaleDbl((*itTov).tovarPRICE_GET(),PRICE_OUT_SCALE);
            Query_audit->ParamByName("quantity")->AsFloat    = TFormatingData::scaleDbl((*itTov).EndSaldoKilk_GET(),QUANTITY_SCALE);
            Query_audit->ParamByName("suma")->AsFloat        = TFormatingData::scaleDbl((*itTov).EndSaldoSuma_GET());
            // Факт шукаємо в накладній
            int i = 0;
            for(i = 0; i < (int)rowList.size(); i++)
               if ( (*itTov).tovarID_GET() == rowList[i].tovar_ID ) break;
            if( i == (int)rowList.size() ) { // Не знайдено фактичного залишку
               Query_audit->ParamByName("quantity_fact")->AsFloat = 0;
               Query_audit->ParamByName("suma_fact")->AsFloat     = 0;
               //
               if ( !(*itTov).EndSaldoKilk_GET() ) continue;
            }
            else { // Знайдено фактичний залишку
               Query_audit->ParamByName("quantity_fact")->AsFloat = TFormatingData::scaleDbl(rowList[i].quantity,QUANTITY_SCALE);
               Query_audit->ParamByName("suma_fact")->AsFloat     = TFormatingData::scaleDbl(rowList[i].quantity*Query_audit->ParamByName("price")->AsFloat);
               rowList[i].used = true;
            }
            // Якщо кількості відрізняються то різницю записуємо в t_ruh_tovar
            if( Query_audit->ParamByName("quantity")->AsFloat != Query_audit->ParamByName("quantity_fact")->AsFloat ) {
               Query_tmp->ParamByName("doc_id")->AsInteger    = Query_audit->ParamByName("doc_id")->AsInteger;
               Query_tmp->ParamByName("tovar_id")->AsInteger  = Query_audit->ParamByName("tovar_id")->AsInteger;
               Query_tmp->ParamByName("debet_id")->AsInteger  = Query_audit->ParamByName("debet_id")->AsInteger;
               Query_tmp->ParamByName("kredyt_id")->AsInteger = Query_audit->ParamByName("kredyt_id")->AsInteger;
               Query_tmp->ParamByName("ov_id")->AsInteger     = Query_audit->ParamByName("ov_id")->AsInteger;
               Query_tmp->ParamByName("price")->AsFloat       = Query_audit->ParamByName("price")->AsFloat;
               Query_tmp->ParamByName("quantity")->AsFloat    = Query_audit->ParamByName("quantity_fact")->AsFloat -
                                                                Query_audit->ParamByName("quantity")->AsFloat;
               Query_tmp->ParamByName("suma")->AsFloat        = Query_audit->ParamByName("suma_fact")->AsFloat-
                                                                Query_audit->ParamByName("suma")->AsFloat;
               Query_tmp->ExecSQL();
            }
            Query_audit->ExecSQL();
            //
            suma_doc += Query_audit->ParamByName("suma_fact")->AsFloat-
                        Query_audit->ParamByName("suma")->AsFloat;
         }
         // Товари що не присутні в залишках але є в ревізії добавляємо в кінці
         for(int i = 0; i < (int)rowList.size(); i++) {
            if( rowList[i].used ) continue; // вже записано товар
            if( type_tovar_list[j] != rowList[i].type_tovar ) continue; // не той тип товару
            //
            // Параметри ревізії
            Query_audit->ParamByName("doc_id")->AsInteger      = doc_tov_ID;
            Query_audit->ParamByName("tovar_id")->AsInteger    = rowList[i].tovar_ID;
            Query_audit->ParamByName("debet_id")->AsInteger    = TSubMethods::GetBCountOfDep(dep_agent_id,"REGRADING");
            Query_audit->ParamByName("kredyt_id")->AsInteger   = TSubMethods::GetBCountOfDep(dep_agent_id,(*itTov).tovarID_GET());
            Query_audit->ParamByName("ov_id")->AsInteger       = rowList[i].ov_ID;
            Query_audit->ParamByName("price")->AsFloat         = TFormatingData::scaleDbl( TWorkDocTov::GetPriceDepCurDate(rowList[i].tovar_ID,dep_agent_id,date_doc,time_doc, true), PRICE_OUT_SCALE );
            Query_audit->ParamByName("quantity")->AsFloat      = 0;
            Query_audit->ParamByName("suma")->AsFloat          = 0;
            Query_audit->ParamByName("quantity_fact")->AsFloat = TFormatingData::scaleDbl(rowList[i].quantity,QUANTITY_SCALE);
            Query_audit->ParamByName("suma_fact")->AsFloat     = TFormatingData::scaleDbl(rowList[i].quantity*Query_audit->ParamByName("price")->AsFloat);
            rowList[i].used = true;
            // Kількості відрізняються то різницю записуємо в t_ruh_tovar
            if( Query_audit->ParamByName("quantity")->AsFloat != Query_audit->ParamByName("quantity_fact")->AsFloat ) {
               Query_tmp->ParamByName("doc_id")->AsInteger    = Query_audit->ParamByName("doc_id")->AsInteger;
               Query_tmp->ParamByName("tovar_id")->AsInteger  = Query_audit->ParamByName("tovar_id")->AsInteger;
               Query_tmp->ParamByName("debet_id")->AsInteger  = Query_audit->ParamByName("debet_id")->AsInteger;
               Query_tmp->ParamByName("kredyt_id")->AsInteger = Query_audit->ParamByName("kredyt_id")->AsInteger;
               Query_tmp->ParamByName("ov_id")->AsInteger     = Query_audit->ParamByName("ov_id")->AsInteger;
               Query_tmp->ParamByName("price")->AsFloat       = Query_audit->ParamByName("price")->AsFloat;
               Query_tmp->ParamByName("quantity")->AsFloat    = Query_audit->ParamByName("quantity_fact")->AsFloat -
                                                                Query_audit->ParamByName("quantity")->AsFloat;
               Query_tmp->ParamByName("suma")->AsFloat        = Query_audit->ParamByName("suma_fact")->AsFloat-
                                                                Query_audit->ParamByName("suma")->AsFloat;
               Query_tmp->ExecSQL();
            }
            Query_audit->ExecSQL();
            //
            suma_doc += Query_audit->ParamByName("suma_fact")->AsFloat-
                        Query_audit->ParamByName("suma")->AsFloat;
         }
         // Змінимо суму по накладній
         Query_tmp->Close();
         Query_tmp->SQL->Clear();
         Query_tmp->SQL->Add(String("")+
            " UPDATE document SET suma=:suma WHERE doc_id=:document_id");
         Query_tmp->ParamByName("suma")->AsFloat           = TFormatingData::scaleDbl(suma_doc);
         Query_tmp->ParamByName("document_id")->AsInteger  = doc_tov_ID;
         Query_tmp->ExecSQL();
      }
      rowList.clear();
      // 3. Записати дані в таблицю r_shift_doc
      //
      Query_tmp->Close();
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " UPDATE r_shift_doc SET doc_id=:document_id WHERE shift_doc_id=:shift_doc_id ");
      //
      Query_tmp->ParamByName("document_id")->AsInteger  = doc_tov_ID;
      Query_tmp->ParamByName("shift_doc_id")->AsInteger = shift_doc_id;
      Query_tmp->ExecSQL();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка CreateShiftDocNaklInventory()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
   return doc_tov_ID;
}
//---------------------------------------------------------------------------

int TShiftSQL::GetBcountID( int type_tovar_id, int dep_id )
{
   if( !pConnect->IsConnecting )
      return 0;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   int bcount_id = 0;
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT bcount_id "+
         " FROM bcounts "+
         " WHERE dep_id=:dep_id AND object_id=:object_id " );
      Query_tmp->ParamByName("dep_id")->AsInteger    = dep_id;
      Query_tmp->ParamByName("object_id")->AsInteger = type_tovar_id;
      //
      Query_tmp->Open();
      if( !Query_tmp->Eof )
         bcount_id = Query_tmp->FieldByName("bcount_id")->AsInteger;
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка GetBcountID()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   //
   delete Query_tmp;
   //
   return bcount_id;
}
//---------------------------------------------------------------------------

void TShiftSQL::LoadChecksByNumber( vector <CHECK_BY_NUMBER> &checkList, int number )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Add(String("")+
         " SELECT z.zakaz_id, z.number, z.`date_z`, z.time_close, z.shift_id, e.`surname`, e.`first_name`, e.`second_name`, z.suma "+
         " FROM r_zakaz z "+
         "    LEFT JOIN employee e ON z.employee_id=e.employee_id "+
         " WHERE z.number = :number "+
         " ORDER BY z.date_z, z.time_close ");
      //
      Query_tmp->ParamByName("number")->AsInteger = number;
      Query_tmp->Open();
      checkList.clear();
      CHECK_BY_NUMBER c;
      while( !Query_tmp->Eof ){
         c.zakaz_id   = Query_tmp->FieldByName("zakaz_id")->AsInteger;
         c.number     = Query_tmp->FieldByName("number_d")->AsInteger;
         c.date_check = Query_tmp->FieldByName("date_z")->AsString;
         c.time_check = Query_tmp->FieldByName("time_close")->AsString;
         c.employee_name = Query_tmp->FieldByName("surname")->AsString+" "+Query_tmp->FieldByName("first_name")->AsString+" "+Query_tmp->FieldByName("second_name")->AsString;
         c.suma       = Query_tmp->FieldByName("suma")->AsFloat;
         //
         TShiftSQL::LoadShift( c.check_shift, Query_tmp->FieldByName("shift_id")->AsInteger );
         //
         checkList.push_back( c );
         Query_tmp->Next();
      }
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка завантаження даних LoadChecksByNumber()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete  Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::UpdatePryznDoc( int doc_id, String oplata_info )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *Query_tmp = new DUniQuery( pConnect );
   try {
      Query_tmp->SQL->Clear();
      Query_tmp->SQL->Add(String("")+
         " UPDATE document SET pryzn=:pryzn WHERE doc_id=:doc_id " );
      //
      Query_tmp->ParamByName("doc_id")->AsInteger = doc_id;
      Query_tmp->ParamByName("pryzn")->AsString   = oplata_info;
      Query_tmp->ExecSQL();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка збереження даних UpdatePryznDoc()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   Query_tmp->Close();
   delete Query_tmp;
}
//---------------------------------------------------------------------------

void TShiftSQL::DeleteShiftDuplicate( int shift_id )
{
   if( !pConnect->IsConnecting )
      return;
   //
   DUniQuery *dbQueryOrders = new DUniQuery( pConnect );
   DUniQuery *dbQueryDelete = new DUniQuery( pConnect );
   //
   try {
     // 1.Перевіряємо наявність дублікатів в зміні якщо є то з number формуємо вектор
     dbQueryOrders->Close();
     dbQueryOrders->SQL->Clear();
     dbQueryOrders->SQL->Add( String("")+
        " SELECT rzd.suma_discount, rzd.shift_id, rzd.date_z, rzd.employee_id, rzd.number    "+
        " FROM r_zakaz rzd   "+
        " WHERE rzd.shift_id=:shift_id "+
        " GROUP BY rzd.number, rzd.date_z, rzd.employee_id, rzd.suma_discount  "+
        " HAVING COUNT(rzd.number) > 1" );
     dbQueryOrders->ParamByName("shift_id")->AsInteger = shift_id;
     //
     vector <String> duplicate_numbers;
     if( dbQueryOrders->Open() ) {
        while (!dbQueryOrders->Eof) {
           duplicate_numbers.push_back( dbQueryOrders->FieldByName("number")->AsString );
           //
           dbQueryOrders->Next();
        }
     }
     dbQueryOrders->Close();
     // 2.Якщо вектор не порожній то по значенню number визначаємо який з чеків вилучати
     if( !duplicate_numbers.size() ) {
        MessageDlg( String("Дублікатів по зміні не знайдено!"), mtInformation, TMsgDlgButtons() << mbOK, 0);
        //
        delete dbQueryDelete;
        delete dbQueryOrders;
        return;
     }
     //
     dbQueryOrders->Close();
     dbQueryOrders->SQL->Clear();
     dbQueryOrders->SQL->Add( String("")+
        " SELECT rzd.zakaz_id, rzd.suma_discount, "+
        "   (SELECT SUM(r.suma_discount) FROM r_zakaz_row r WHERE r.zakaz_id=rzd.zakaz_id) as suma_discount_row "+
        " FROM r_zakaz rzd   "+
        " WHERE rzd.shift_id=:shift_id AND rzd.number=:number " );
     for(int i=0;i<(int)duplicate_numbers.size();i++ ) {
        dbQueryOrders->ParamByName("shift_id")->AsInteger = shift_id;
        dbQueryOrders->ParamByName("number")->AsString    = duplicate_numbers[i];
        int ok_zakaz_id = 0, del_zakaz_id = 0;
        if( dbQueryOrders->Open() ) {
           while (!dbQueryOrders->Eof) {
              if( !ok_zakaz_id &&
                 TFormatingData::scaleDbl( dbQueryOrders->FieldByName("suma_discount")->AsFloat-
                                           dbQueryOrders->FieldByName("suma_discount_row")->AsFloat) == 0 ) {
                 ok_zakaz_id = dbQueryOrders->FieldByName("zakaz_id")->AsInteger;
                 dbQueryOrders->Next();
                 continue;
              }
              del_zakaz_id = dbQueryOrders->FieldByName("zakaz_id")->AsInteger;
              //
              dbQueryOrders->Next();
              //
              if( !ok_zakaz_id && dbQueryOrders->Eof ) break; // Якщо не було жодного коректного чеку то залишаємо останній
              // 3.Вилучаємо чек, рядки, історію, оплати
              // 3. Рядки
              dbQueryDelete->Close();
              dbQueryDelete->SQL->Clear();
              dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_zakaz_row WHERE zakaz_id = :zakaz_id ");
              dbQueryDelete->ParamByName("zakaz_id")->AsInteger = del_zakaz_id;
              if( !dbQueryDelete->ExecSQL() ) {
                 MessageDlg( String("Помилка видалення рядків!"), mtError,
                                                  TMsgDlgButtons() << mbOK, 0);
                 break;
              }
              //
              // 4. Оплати
              dbQueryDelete->Close();
              dbQueryDelete->SQL->Clear();
              dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_zakaz_calculation WHERE zakaz_id = :zakaz_id ");
              dbQueryDelete->ParamByName("zakaz_id")->AsInteger = del_zakaz_id;
              if ( !dbQueryDelete->ExecSQL() ) {
                 MessageDlg( String("Помилка видалення оплат!"), mtError,
                                                  TMsgDlgButtons() << mbOK, 0);
                 break;
              }
              //
              // 5. Історія
              dbQueryDelete->Close();
              dbQueryDelete->SQL->Clear();
              dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_status_history WHERE doc_id = :zakaz_id ");
              dbQueryDelete->ParamByName("zakaz_id")->AsInteger = del_zakaz_id;
              if ( !dbQueryDelete->ExecSQL() ) {
                 MessageDlg( String("Помилка видалення історії"), mtError,
                                                  TMsgDlgButtons() << mbOK, 0);
                 break;
              }
              // 6. чек
              dbQueryDelete->Close();
              dbQueryDelete->SQL->Clear();
              dbQueryDelete->SQL->Add(String("") + " DELETE FROM r_zakaz WHERE zakaz_id = :zakaz_id ");
              dbQueryDelete->ParamByName("zakaz_id")->AsInteger = del_zakaz_id;
              if( !dbQueryDelete->ExecSQL() ) {
                 MessageDlg( String("Помилка видалення чеків!"), mtError,
                                                  TMsgDlgButtons() << mbOK, 0);
                 break;
              }
           }
        }
        dbQueryOrders->Close();
     }
     //
     duplicate_numbers.clear();
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка вилучення дублікатів по зміні DeleteShiftDuplicate()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
   //
   dbQueryDelete->Close();
   dbQueryOrders->Close();
   //
   delete dbQueryDelete;
   delete dbQueryOrders;
   //
   MessageDlg( String("Дублікати по зміні почищено!"), mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------

