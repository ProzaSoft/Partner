//---------------------------------------------------------------------------

#ifndef ShiftSQLH
#define ShiftSQLH
//---------------------------------------------------------------------------
#include <vector>
#include <algorithm>

using namespace std;
//---------------------------------------------------------------------------

struct SHIFT {
   int shift_ID;
   //
   int    cash_register_ID;
   String cash_register_name;
   //
   int    employee_ID;
   String user_name;
   //
   int    dep_ID;
   int    agent_ID;
   String agent_name;
   //
   String datetime_beg;
   String datetime_end;
   int    zzvit_number;
   //
   int stan;
};

struct ZAKAZ_ROW {
   int    zakaz_row_ID;
   int    zakaz_ID;
   int    tovar_ID;
   String kod_ekka;
   String name_tovar;
   int    ov_ID;
   String ov_name;
   double price;
   double quantity;
   double suma;
   double suma_discount;
   double bonus;
   double cd_percent;
   int    fiscal_tovar;
   int    type_tovar;
   int    make_flag;
   bool   used;
};

struct ZAKAZ_CALCULATION {
   int    zakaz_calculation_ID;
   int    zakaz_ID;
   int    type_calculation_ID;
   String type_calculation_name;
   double suma;
   String info;
   int    type_oplat;
   int    point_of_money_id;
   //
   double course;
   int    currency_ID;
   double amount_currency;
};

struct ZAKAZ {
   int    zakaz_ID;
   int    employee_id;
   String user_name;
   int    agent_id;
   String agent_name;
   int    type_ag_id;
   int    number;
   String date_z;
   String time_open;
   String time_close;
   int    stan;
   double suma;
   double suma_discount;
   double znyzka;
   double bonus;
   int    repayment;
   int    repayment_check;
   int    shift_id;
   int    discount_card_id;
   int    fiscal_check;
   double fiscal_suma;
   double fiscal_suma_discount;
   //
   String credit_card_check;
   String coment;
   //
   int    oper_id;
   String oper_name;
   int    doc_id;
   int    doc_number;
   //
   vector <ZAKAZ_ROW> rowList;
   vector <ZAKAZ_CALCULATION> calculationList;
};

struct TERMINAL_TOVAR {
   int    tovar_ID;
   int    kod_ekka;
   double price;
   double quantity_real;
   double suma_real;
   double suma_no_discount;
   int    type_tovar;
   int    make_flag;
};

struct SHIFT_DOCS {
   int    doc_tov_ID;
   String date_d;
   String time_d;
   int    number_d;
   String from_name;
   String to_name;
   String coment;
   double suma;
   int    stan;
};

struct DOC_HISTORY {
   int    status_history_ID;
   int    doc_ID;
   String date_d;
   String user_name;
   String comment;
   int    tovar_ID;
   String tovar_name;
};

struct TYPE_CALCULATION {
   int    type_calculation_ID;
   String name;
   int    use_flag;
   int    type_oplat;
   int    point_of_money_id;
   String point_of_money_name;
};

struct SHIFT_CASH {
   int    cash_id;
   String date_time_input;
   double cash_in;
   double cash_out;
   String coment;
   int    cash_type;
   int    cost_id;
   int    oplata_id;
   String doc_info;
};

struct CHECK_BY_NUMBER {
   int    zakaz_id;
   int    number;
   String date_check;
   String time_check;
   String employee_name;
   double suma;
   SHIFT  check_shift;
};
//---------------------------------------------------------------------------

class TShiftSQL
{
private:
    //
public:
    TShiftSQL();
    // Функції роботи зі змінами
    static void LoadShiftList( vector <SHIFT> &shiftList, int stan, int dep_id, String BegDate, String EndDate );
    static void LoadShiftList( vector <SHIFT> &shiftList, int stan );
    static void LoadShift( SHIFT &shiftE, int shift_id );
    static void ClearShift( SHIFT &shiftE );
    static void LoadShiftCash( int shift_id, double &cash_in, double &cash_out );
    static void TShiftSQL::LoadShiftBonus( int shift_id, double &bonus_plus, double &bonus_minus );
    static int  GetDepartmentInfo( String &department_name );
    static void LoadShiftCashList( int shift_id, vector <SHIFT_CASH> &cashList);
    static void DeleteShift( int shift_id );
    static void DeleteShiftDuplicate( int shift_id );
    // Функції роботи з чеками
    static void LoadZakazList( vector <ZAKAZ> &zakazList, int shift_id );
    static void ClearZakaz( ZAKAZ &zakazE );
    static void LoadZakazRowList( vector <ZAKAZ_ROW> &rowList, int zakaz_id );
    static void ClearZakazRow( ZAKAZ_ROW &rowE );
    static void LoadZakazCalculationList( vector <ZAKAZ_CALCULATION> &calculationList, int zakaz_id );
    static void LoadShiftCalculationList( vector <ZAKAZ_CALCULATION> &calculationList, int shift_id );
    static void ClearZakazCalculation( ZAKAZ_CALCULATION &calculationE );
    static String GetDiscountCardOwner( int discount_card_id );
    // Функції роботи з автоматичними документами
    static void LoadShiftDocList( vector <ZAKAZ> &zakazList, int shift_id );
    static void LoadShiftDocRowList( vector <ZAKAZ_ROW> &rowList, int shift_doc_id );
    static void LoadShiftDocCalculationList( vector <ZAKAZ_CALCULATION> &calculationList, int shift_doc_id );
    static void LoadShiftShiftDocCalculationList( vector <ZAKAZ_CALCULATION> &calculationList, int shift_id );
    static int  CreateDocumentFromShiftDoc( );
    static int  CreateShiftDocNakl( int shift_doc_id,
           String date_doc, String time_doc, int oper_id, int dep_agent_id, int agent_id, String coment, String user_name );
    static void CreateShiftDocOplat( vector <ZAKAZ_CALCULATION> &total_calculation, int type_opl_id,
           String date_doc, int agent_id, int agent_to_id, String coment, int doc_tov_id );
    static int  CreateShiftDocNaklInventory( int shift_doc_id,
           String date_doc, String time_doc, int oper_id, int dep_agent_id, String coment );
    static int  GetBcountID( int type_tovar_id, int dep_id );
    // Функції роботи по закриттю зміни
    static void SetShiftStan(int shift_id, int stan);
    static void LoadAgentShift( int shift_id, vector <int> &agentShift );
    static int  LoadDefaultStore( );
    static bool ReadRealToVector( int agent_id, vector <TERMINAL_TOVAR> &tov, String date_doc, int shift_id, vector <ZAKAZ_CALCULATION> &total_calculation, int repayment, int zakaz_id=0 );
    static int CreateNakl( vector <TERMINAL_TOVAR> &tov,
           String date_doc, String time_doc, int agent_id, int agent_to_id, String coment, int shift_id );
    static int  GetOperationID( int agent_id, int agent_to_id  );
    static void CreateOplat( vector <ZAKAZ_CALCULATION> &total_calculation, int type_opl_id,
           String date_doc, int agent_id, int agent_to_id, int shift_id, int doc_tov_id );
    static void AddOplataRow( int oplata_id, int currency_id, double amount_currency, double course, double suma );
    //
    static String LoadDocInfoFromCost( int cost_id );
    static String LoadDocInfoFromOplata( int oplata_id );
    static void SaveCashTypeInShiftCash( int cash_id, int cash_type );
    static void SaveCostIDInShiftCash( int cash_id, int cost_id );
    static void SaveOplataIDInShiftCash( int cash_id, int oplata_id );
    static void GetDepartmentInfoFromShift( int shift_id, int &dep_id, String &department_name, int &point_of_money_id, String &point_of_money_name );
    // Функції роботи по документах по зміні
    static void GetShiftDocs( int shift_id, vector <SHIFT_DOCS> &shiftDocList );
    static void ClearShiftDoc( SHIFT_DOCS &shiftDocE );
    static void DeleteShiftDoc( int shift_id, int doc_tov_id );
    static void DeleteDocOplat( int doc_tov_id );
    static void GetDocTovar( int doc_tov_id, vector <TERMINAL_TOVAR> &tovarList);
    static void ClearTerminalTovar( TERMINAL_TOVAR &tovarE );
    // Функції роботи по історії використання дисконтів для накопичувальної системи з реплікаціями
    static void InsertDiscountHistoryByShift( int shift_id );
    static void DeleteDiscountHistoryByShift( int shift_id );
    // Функції роботи по історії
    static void LoadDocHistoryList( vector <DOC_HISTORY> &docHistoryList, int doc_id );
    static void ClearDocHistory( DOC_HISTORY &docHistoryE );
    static String RecodeComment(String name, String p1, String p2, String p3, String p4, String p5);
    // Функції роботи з типами оплат (роздріб)
    static void LoadTypeCalculationList( vector <TYPE_CALCULATION> &calculationList );
    static void SaveTypeCalculation( TYPE_CALCULATION &calculationE );
    static void ClearTypeCalculation( TYPE_CALCULATION &calculationE );
    // Функції для створення окремих накладних по агенту по кожному чеку
    static bool ReadNaklAgentToVector( int agent_id, vector <int> &zakazID, String date_doc, int shift_id );
    static bool GetMultyNaklAgent( int agent_id );
    //
    static void LoadChecksByNumber( vector <CHECK_BY_NUMBER> &checkList, int number );
    static void UpdatePryznDoc( int doc_id, String oplata_info );
};
//---------------------------------------------------------------------------
#endif
