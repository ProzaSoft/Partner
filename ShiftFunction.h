//---------------------------------------------------------------------------

#ifndef ShiftFunctionH
#define ShiftFunctionH
//---------------------------------------------------------------------------

class TShiftFunction
{
private:
    //
public:
    TShiftFunction();
    static int  TShiftFunction::GetCurrentShiftID( int cash_register_id, int user_id, int agent_id, bool create_new=true );
    static int  TShiftFunction::CreateNewShift( int cash_register_id, int user_id, int agent_id );
    static void TShiftFunction::InputCash( int shift_id, double suma );
    static void TShiftFunction::ZZvitShift( int shift_id, int user_id, int zzvit_number );
    static void TShiftFunction::CloseShift( int shift_id );
    static int  TShiftFunction::GetCashRegisterID( String znumber );
    //
    static void TShiftFunction::CreateNakl( int shift_id );

};
//---------------------------------------------------------------------------
#endif
