//---------------------------------------------------------------------------

#ifndef StatusHistorySQLH
#define StatusHistorySQLH
//---------------------------------------------------------------------------
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
//---------------------------------------------------------------------------
struct STATUS_ACTION {
   int    status_history_ID;
   int    doc_ID;
   int    tovar_ID;
   String date_d;
   String user_name;
   String comment;
   double quantity, price, suma;
   bool   selected;
};
struct STATUSES{
   int    ID;
   String name;
   bool   selected;
};
//---------------------------------------------------------------------------

class TStatusHistorySQL
{
   private:
   public:
      TStatusHistorySQL();
      //
      static void   LoadStatuses( vector <STATUSES> &statusList );
      static void   LoadActionsByTovarID( vector <STATUS_ACTION> &actionList, vector <STATUSES> &statusList, String BegDate, String EndDate, int tovar_id, int user_id, int dep_id );
      static String RecodeComment(String name, String p1, String p2, String p3, String p4, String p5);
      static String GetUserNameFromLogin( String user_name );
      static void   GetUserNameIntoMap( map <String,String> &user_map );
};
//---------------------------------------------------------------------------
#endif
