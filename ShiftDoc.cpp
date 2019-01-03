//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShiftDoc.h"
#include "FormatingData.h"
#include "WorkFiles.h"
#include "SubMethods.h"
#include "StatusHistoryExellio.h"
#include "ShiftCash.h"

//---------------------------------------------------------------------------
#include "Reports.h"
//---------------------------------------------------------------------------

extern DConnect *pConnect;

extern void RegistryForm(  void *document, char *title  );

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFShiftDoc *FShiftDoc;
//---------------------------------------------------------------------------
__fastcall TFShiftDoc::TFShiftDoc(int shift_id, TComponent* Owner)
        : TForm(Owner)
{
   shift_ID = shift_id;
   sortOrder  = 0;
   sortDirectShiftDoc = false;
   //
   HeadTitleCheck = new THeadersBuilder(CheckDrawGrid,3);
   SetTitleCaptionCheck();
   //
   HeadTitleTovar = new THeadersBuilder(TovDrawGrid,2);
   SetTitleCaptionTovar();
}
//---------------------------------------------------------------------------
void TFShiftDoc::SetTitleCaptionCheck( void )
{
   HeadTitleCheck->Clear();
   //
   HeadTitleCheck->Add("    №    ");
   HeadTitleCheck->Add("   Час   ");
   HeadTitleCheck->Add("  Агент  ");
   HeadTitleCheck->Add("   Зн%   ");
   HeadTitleCheck->Add("  Сума   ");
   HeadTitleCheck->Add("  Док    ");
   //
   HeadTitleCheck->Activate();
}
//---------------------------------------------------------------------------
void TFShiftDoc::SetTitleCaptionTovar( void )
{
   HeadTitleTovar->Clear();
   //
   HeadTitleTovar->Add("Код");
   HeadTitleTovar->Add("Товар");
   HeadTitleTovar->Add("ОВ");
   HeadTitleTovar->Add("Кільк.");
   HeadTitleTovar->Add("Ціна");
   HeadTitleTovar->Add("Сума");
   //
   HeadTitleTovar->Activate();
}
//---------------------------------------------------------------------------

bool TFShiftDoc::compNumber( ZAKAZ a, ZAKAZ b ) {
   if ( !sortDirectShiftDoc ) {
      if ( a.zakaz_ID < b.zakaz_ID ) return true;
      return false;
   }
   else {
      if ( b.zakaz_ID < a.zakaz_ID ) return true;
      else return false;
   }
}
bool TFShiftDoc::compTime( ZAKAZ a, ZAKAZ b ) {

   if ( !sortDirectShiftDoc ) {
      if(a.time_close=="") return true;
      if(b.time_close=="") return true;
      if ( (StrToTime(a.time_close) < StrToTime(b.time_close)) ) return true;
      if ( (StrToTime(a.time_close) == StrToTime(b.time_close)) && compNumber(a,b) ) return true;
      return false;
   }
   else {
      if(a.time_close=="") return true;
      if(b.time_close=="") return true;
      if ( (StrToTime(b.time_close) < StrToTime(a.time_close)) ) return true;
      if ( (StrToTime(b.time_close) == StrToTime(a.time_close)) && compNumber(b,a) ) return true;
            return false;
   }
}
bool TFShiftDoc::compName( ZAKAZ a, ZAKAZ b ) {
   if ( !sortDirectShiftDoc ) {
      if ( a.user_name.AnsiCompare(b.user_name) < 0 ) return true;
      else
         if ( !a.user_name.AnsiCompare(b.user_name) && compTime(a,b) ) return true;
      return false;
   }
   else {
      if ( b.user_name.AnsiCompare(a.user_name) < 0 ) return true;
      else
         if ( !b.user_name.AnsiCompare(a.user_name) && compTime(b,a) ) return true;
      return false;
   }
}
bool TFShiftDoc::compZn( ZAKAZ a, ZAKAZ b ) {
   if ( !sortDirectShiftDoc ) {
      if ( a.znyzka < b.znyzka ) return true;
      return false;
   }
   else {
      if ( b.znyzka < a.znyzka ) return true;
      return false;
   }
}
bool TFShiftDoc::compSuma( ZAKAZ a, ZAKAZ b ) {
   if ( !sortDirectShiftDoc ) {
      if ( a.suma_discount < b.suma_discount ) return true;
      return false;
   }
   else {
      if ( b.suma_discount < a.suma_discount ) return true;
      return false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::CheckDrawGridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
   TRect TheRect;
   TheRect.Left = Rect.Left-1;
   TheRect.Top = Rect.Top-1;
   TheRect.Right = Rect.Right+1;
   TheRect.Bottom = Rect.Bottom+1;
   if( !ARow ){
      HeadTitleCheck->DrawTitle(ACol,Rect,sortOrder == ACol?clInfoBk:clBtnFace);
   }
   else {
      if(!zakazList.size()) return;
      if ( zakazList[ARow-1].repayment )
         CheckDrawGrid->Canvas->Font->Color = clRed;
      else if( CheckDrawGrid->Row == ARow )
         CheckDrawGrid->Canvas->Font->Color = clWhite;
      else
         CheckDrawGrid->Canvas->Font->Color = clBlack;
      switch(ACol) {
         case 0:
//            CheckDrawGrid->Canvas->TextOut(Rect.Right-2-CheckDrawGrid->Canvas->TextWidth(zakazList[ARow-1].zakaz_ID),Rect.Top+2,zakazList[ARow-1].zakaz_ID);
            CheckDrawGrid->Canvas->TextOut(Rect.Right-2-CheckDrawGrid->Canvas->TextWidth(zakazList[ARow-1].number),Rect.Top+2,zakazList[ARow-1].number);
            CheckDrawGrid->Canvas->Brush->Color = clBtnFace;
            CheckDrawGrid->Canvas->FrameRect(TheRect);
            break;
         case 1:
            CheckDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,zakazList[ARow-1].time_close);
            CheckDrawGrid->Canvas->Brush->Color = clBtnFace;
            CheckDrawGrid->Canvas->FrameRect(TheRect);
            break;
         case 2:
            CheckDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,(zakazList[ARow-1].agent_id?zakazList[ARow-1].agent_name:zakazList[ARow-1].oper_name));
            CheckDrawGrid->Canvas->Brush->Color = clBtnFace;
            CheckDrawGrid->Canvas->FrameRect(TheRect);
            break;
         case 3: if ( zakazList[ARow-1].znyzka )
                    CheckDrawGrid->Canvas->TextOut(Rect.Right-2-CheckDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(zakazList[ARow-1].znyzka)),Rect.Top+2,TFormatingData::DblToStr(zakazList[ARow-1].znyzka));
                 else
                    CheckDrawGrid->Canvas->TextOut(Rect.Left,Rect.Top+2,"");
            CheckDrawGrid->Canvas->Brush->Color = clBtnFace;
            CheckDrawGrid->Canvas->FrameRect(TheRect);
            break;
         case 4:
            CheckDrawGrid->Canvas->TextOut(Rect.Right-2-CheckDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(zakazList[ARow-1].suma_discount)),Rect.Top+2,TFormatingData::DblToStr(zakazList[ARow-1].suma_discount));
            CheckDrawGrid->Canvas->Brush->Color = clBtnFace;
            CheckDrawGrid->Canvas->FrameRect(TheRect);
            break;
         case 5:
            CheckDrawGrid->Canvas->TextOut(Rect.Right-2-CheckDrawGrid->Canvas->TextWidth(String("")+zakazList[ARow-1].doc_number+"/"+zakazList[ARow-1].doc_id),Rect.Top+2,String("")+zakazList[ARow-1].doc_number+"/"+zakazList[ARow-1].doc_id);
            CheckDrawGrid->Canvas->Brush->Color = clBtnFace;
            CheckDrawGrid->Canvas->FrameRect(TheRect);
            break;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::TovDrawGridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
   TRect TheRect;
   TheRect.Left = Rect.Left-1;
   TheRect.Top = Rect.Top-1;
   TheRect.Right = Rect.Right+1;
   TheRect.Bottom = Rect.Bottom+1;
   if(!ARow){
      HeadTitleTovar->DrawTitle(ACol,Rect,clBtnFace);
   }
   else {
      if( !rowList.size() ) return;
      //
      if ( rowList[ARow-1].fiscal_tovar )
         TovDrawGrid->Canvas->Brush->Color = 0xfdfde1;
      else
         TovDrawGrid->Canvas->Brush->Color = clWhite;
      //
      if ( TovDrawGrid->Row == ARow ){
//         TovDrawGrid->Canvas->Brush->Color = clNavy;
         TovDrawGrid->Canvas->Font->Color = clBlack;
      }
      TovDrawGrid->Canvas->FillRect(Rect);
      //
      switch(ACol){
         case 0:
            TovDrawGrid->Canvas->TextOut(Rect.Right-2-TovDrawGrid->Canvas->TextWidth(rowList[ARow-1].kod_ekka),Rect.Top+2,rowList[ARow-1].kod_ekka);
            break;
         case 1:
            TovDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,rowList[ARow-1].name_tovar);
            break;
         case 2:
            TovDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,rowList[ARow-1].ov_name);
	    break;
         case 3:
            TovDrawGrid->Canvas->TextOut(Rect.Right-2-TovDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(rowList[ARow-1].quantity,3)),Rect.Top+2,TFormatingData::DblToStr(rowList[ARow-1].quantity,3));
            break;
         case 4:
            TovDrawGrid->Canvas->TextOut(Rect.Right-2-TovDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(rowList[ARow-1].price)),Rect.Top+2,TFormatingData::DblToStr(rowList[ARow-1].price));
            break;
         case 5:
            TovDrawGrid->Canvas->TextOut(Rect.Right-2-TovDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(rowList[ARow-1].suma)),Rect.Top+2,TFormatingData::DblToStr(rowList[ARow-1].suma));
            break;
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TFShiftDoc::ExitBtnClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::FormResize(TObject *Sender)
{
   int width_float_col = CheckDrawGrid->Width - 18; // 17+1 - ширина полоси прокрутки
   //
   for( int i = 0; i < CheckDrawGrid->ColCount; i++ ) {
      if ( i == HeadTitleCheck->FloatCol_GET() - 1 ) continue; // колонка зі змінною шириною
      width_float_col -= (CheckDrawGrid->ColWidths[i] + 1);
   }
   //
   CheckDrawGrid->ColWidths[HeadTitleCheck->FloatCol_GET() - 1] = width_float_col;
   //
   //
   //
   width_float_col = TovDrawGrid->Width - 18; // 17+1 - ширина полоси прокрутки
   //
   for( int i = 0; i < TovDrawGrid->ColCount; i++ ) {
      if ( i == HeadTitleTovar->FloatCol_GET() - 1 ) continue; // колонка зі змінною шириною
      width_float_col -= (TovDrawGrid->ColWidths[i] + 1);
   }
   //
   TovDrawGrid->ColWidths[HeadTitleTovar->FloatCol_GET() - 1] = width_float_col;
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::FormShow(TObject *Sender)
{
   vector <THeadersBuilder *> HeadTitles;
                              HeadTitles.push_back(HeadTitleCheck);
                              HeadTitles.push_back(HeadTitleTovar);
   TSubMethods::RestoreFormParams( "Partner.ini", this, HeadTitles );
   //
   BONUS_ON = 0;
//   PrnBtn->Visible = StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "check", "CHECK_TO_PRINTER", "0" ));
   //
   // Читаємо інформацію про зміну
   SHIFT shiftE;
   TShiftSQL::LoadShift(shiftE,shift_ID);
   //
   ShiftNumber->Caption      = shiftE.shift_ID;
   DateBeg->Caption          = shiftE.datetime_beg;
   DateEnd->Caption          = shiftE.datetime_end;
   CashRegisterName->Caption = shiftE.cash_register_name;
   UserName->Caption         = shiftE.user_name;
   switch( shiftE.stan ) {
      case 0: StanShift->Caption = "Відкрита";  break;
      case 1: StanShift->Caption = "Завершена"; break;
      case 2: StanShift->Caption = "Закрита";   break;
      default: StanShift->Caption = "????????"; break;
   }
   //
   TShiftSQL::LoadShiftDocList(zakazList,shift_ID);
   //
   if ( zakazList.size() ) {
      CheckDrawGrid->RowCount = zakazList.size() + 1;
      CheckDrawGrid->Row = 1;
   }
   else {
      CheckDrawGrid->RowCount = 2;
   }
   //
   TShiftSQL::LoadShiftShiftDocCalculationList(shiftCalculationList,shift_ID);
   //
   if ( shiftCalculationList.size() ) {
      ShiftCalculationDrawGrid->RowCount = shiftCalculationList.size();
      ShiftCalculationDrawGrid->Row = 0;
   }
   else
      ShiftCalculationDrawGrid->RowCount = 1;

   if ( zakazList.size() ) {
      RefreshTov(zakazList[CheckDrawGrid->Row-1].zakaz_ID);
   }
   else {
      RefreshTov( 0 );
   }
   sortOrder = 1;
   SortDocList();
   RecountTotalSum();
   CheckDrawGrid->Refresh();
   ShiftCalculationDrawGrid->Refresh();
}
//---------------------------------------------------------------------------

void TFShiftDoc::RecountTotalSum() // формування підсумкових сум по зміні
{
   double suma_total, suma_no_discount, repayment, suma_cash, suma_agent, suma_viddil, repayment_viddil, cash_in, cash_out;
   suma_total = suma_no_discount = repayment = 0;
   suma_cash = suma_agent = suma_viddil = repayment_viddil = 0;
   cash_in = cash_out = 0;
   //
   for(int i=0;i<(int)zakazList.size();i++ ) {
      if( !zakazList[i].zakaz_ID )
         continue;
      if( !zakazList[i].type_ag_id || zakazList[i].type_ag_id==6 ){
         suma_total       += zakazList[i].suma_discount;
         suma_no_discount += zakazList[i].suma;
         if( !zakazList[i].type_ag_id ) // роздріб
            suma_cash += zakazList[i].suma_discount;
         else // покупець
            suma_agent += zakazList[i].suma_discount;
      }
      else {
         suma_viddil += zakazList[i].suma_discount;
      }
   }
   TotalSuma->Caption       = TFormatingData::DblToStr(suma_total);
   RozdribSuma->Caption     = TFormatingData::DblToStr(suma_cash);
   AgentSuma->Caption       = TFormatingData::DblToStr(suma_agent);
   TotalRepayment->Caption  = TFormatingData::DblToStr(repayment);
   ViddilSuma->Caption      = TFormatingData::DblToStr(suma_viddil);
   RepaymentViddil->Caption = TFormatingData::DblToStr(repayment_viddil);
   //
   TShiftSQL::LoadShiftCash(shift_ID, cash_in, cash_out);
   CashIn->Caption  = TFormatingData::DblToStr(cash_in);
   CashOut->Caption = TFormatingData::DblToStr(cash_out);
   //
   double cash_oborot = 0;
   for(int i=0;i<(int)shiftCalculationList.size();i++ )
     if( shiftCalculationList[i].type_calculation_ID ==
         StrToInt(TWorkFiles::GetDataFromIniFile( "PTerminalAdmin.ini", "shift", "CASH_TYPE_CALCULATION_ID", "1" )) ) {
         cash_oborot = shiftCalculationList[i].suma;
     }
   CashInSafe->Caption = TFormatingData::DblToStr(cash_in+cash_out+cash_oborot);
}
//---------------------------------------------------------------------------

void  TFShiftDoc::RefreshTov( int shift_doc_ID )
{
   CheckInfoLabel->Caption = "";
   ComentLabel->Caption    = "";
   rowList.clear();
   TovDrawGrid->RowCount = 2;
   TovDrawGrid->Row      = 1;
   TovDrawGrid->Refresh();
   zakazCalculationList.clear();
   ZakazCalculationDrawGrid->RowCount = 2;
   ZakazCalculationDrawGrid->Row      = 1;
   ZakazCalculationDrawGrid->Refresh();
   //
   SumTovLabel->Caption  = "";
   ZnTovLabel->Caption   = "";
   SpTovLabel->Caption   = "";
   Bonus->Caption        = "";
   //
   if ( !zakazList.size() ) return;
   // Інформація про чек
   CheckInfoLabel->Caption = String("")+(zakazList[CheckDrawGrid->Row-1].agent_name!=""?String("")+zakazList[CheckDrawGrid->Row-1].agent_name+"    ":String(""))+
      String("Документ №")+zakazList[CheckDrawGrid->Row-1].number+"    "+
                      zakazList[CheckDrawGrid->Row-1].date_z+" "+
                      zakazList[CheckDrawGrid->Row-1].time_close+"     "+
                      zakazList[CheckDrawGrid->Row-1].oper_name;
   ComentLabel->Caption = zakazList[CheckDrawGrid->Row-1].coment;
   //
   TShiftSQL::LoadShiftDocRowList(rowList,shift_doc_ID);
   //
   if( rowList.size() ){
      TovDrawGrid->RowCount = rowList.size() + 1;
      TovDrawGrid->Row = 1;
   }
   else{
      TovDrawGrid->RowCount = 2;
   }
   TovDrawGrid->Refresh();
   //
   TShiftSQL::LoadShiftDocCalculationList(zakazCalculationList,shift_doc_ID);
   //
   if ( zakazCalculationList.size() ) {
      ZakazCalculationDrawGrid->RowCount = zakazCalculationList.size();
      ZakazCalculationDrawGrid->Row = 0;
   }
   else
      ZakazCalculationDrawGrid->RowCount = 1;
   //
   ZakazCalculationDrawGrid->Refresh();
   //
   double suma_total=0, suma_discount=0;
   for(int i=0;i<(int)rowList.size();i++) {
      suma_total    += rowList[i].suma;
      suma_discount += rowList[i].suma_discount;
   }
   SumTovLabel->Caption  = TFormatingData::DblToStr(suma_total);
   ZnTovLabel->Caption   = TFormatingData::DblToStr(suma_total-suma_discount);
   SpTovLabel->Caption   = TFormatingData::DblToStr(suma_discount);
   //
   if( zakazList[CheckDrawGrid->Row-1].discount_card_id ) {
      DiscountCardInfo->Caption = TShiftSQL::GetDiscountCardOwner(zakazList[CheckDrawGrid->Row-1].discount_card_id);
      DiscountCardInfo->Visible = true;
   }
   else {
      DiscountCardInfo->Caption = "";
      DiscountCardInfo->Visible = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::CheckDrawGridClick(TObject *Sender)
{
   if( !CheckDrawGrid->Row )
      return;
   if ( zakazList.size() )
      RefreshTov(zakazList[CheckDrawGrid->Row-1].zakaz_ID);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::CheckDrawGridMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   int Column, Row;
   CheckDrawGrid->MouseToCell(X, Y, Column, Row);
   if ( !Row ) {
      if ( Column == 5 ) return;
      if ( sortOrder == Column ) sortDirectShiftDoc = !sortDirectShiftDoc;
      else sortOrder = Column;
      SortDocList();
      CheckDrawGrid->Refresh();
      //
      if ( zakazList.size() )
         RefreshTov(zakazList[CheckDrawGrid->Row-1].zakaz_ID);
   }
}
//---------------------------------------------------------------------------

void TFShiftDoc::SortDocList( void )
{
   Cursor = crHourGlass;
   //
   switch( sortOrder ) {
      case 0 : sortNumber(); break;
      case 1 : sortTime();   break;
      case 2 : sortName();   break;
      case 3 : sortZn();     break;
      case 4 : sortSuma();   break;
   }
   //
   CheckDrawGrid->Refresh();
   CheckDrawGrid->SetFocus();
   //
   Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::FormClose(TObject *Sender, TCloseAction &Action)
{
   Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::PrnBtnClick(TObject *Sender)
{
/*   if( !zakazList.size() ) return;
   if( !zakazList[CheckDrawGrid->Row-1].stan ) {
      ShowMessage("Чек не проведений!");
      return;//непроведені чеки забороняємо друкувати
   }
   //
   char curDir[120];
   GetCurrentDirectory(100,curDir);
   //
   TFReports *Reports=new TFReports(this);
   //
   try {
      String query = String("");
      Reports->frxReports->LoadFromFile(String(curDir)+"\\Reports\\TerminalAdminReports\\FR_CheckPrn.fr3");
      //
      Reports->Caption = String("Друк документа №")+zakazList[CheckDrawGrid->Row-1].number;
      //
      Reports->frxMSQuery->SetConnected(pConnect);
      query = String("")+
         " SELECT z.number, z.date_z, z.time_close, z.discount_card_id, "+
         "        ae.name as user_name, dc.number as discount_card_number, dc.owner_name as discount_card_info, "+
         "        zr.quantity, zr.price, zr.suma, zr.suma_discount, zr.bonus, "+
         "        if(t.vyrobnyk_id,CONCAT(v.name,' ',t.name),t.name) as tovar_name, "+
         "        o.short_name as ov_name, t.kod_ekka "+
         " FROM r_zakaz_row zr "+
         "    LEFT JOIN r_zakaz z ON  (zr.zakaz_id = z.zakaz_id) "+
         "    LEFT JOIN t_tovar t ON  (zr.tovar_id = t.tovar_id) "+
         "    LEFT JOIN t_group_tov gr ON  (t.group_tov_id = gr.group_tov_id) "+
         "    LEFT JOIN vyrobnyk v ON  (t.vyrobnyk_id = v.vyrobnyk_id) "+
         "    LEFT JOIN t_ov o ON  (t.ov_id = o.ov_id) "+
         "    LEFT JOIN employee e   ON z.employee_id  = e.employee_id "+
         "    LEFT JOIN agents ae    ON e.agent_id = ae.agent_id "+
         "    LEFT JOIN r_discount_card dc ON z.discount_card_id=dc.discount_card_id "
         " WHERE zr.zakaz_id="+zakazList[CheckDrawGrid->Row-1].zakaz_ID+" "+
         " ORDER BY zr.zakaz_row_id ";
      //
      Reports->frxMSQuery->SetSQL(query);
      Reports->frxMSQuery->QueryOpen=true;
      //
      RegistryForm(Reports,"Перегляд");
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка Reports()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }  **/
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if( Key == VK_F10 ) {
      Key=0;
      ExitBtnClick(Sender);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::SHBitBtnClick(TObject *Sender)
{
/*   if( !zakazList.size() ) return;
   //
   TFStatusHistoryExellio *FStatusHistoryExellio = new TFStatusHistoryExellio (zakazList[CheckDrawGrid->Row-1].zakaz_ID,Application);
   FStatusHistoryExellio->ShowModal();
   delete FStatusHistoryExellio;*/
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::ShiftCalculationDrawGridDrawCell(
      TObject *Sender, int ACol, int ARow, TRect &Rect,
      TGridDrawState State)
{
   TRect TheRect;
   TheRect.Left = Rect.Left-1;
   TheRect.Top = Rect.Top-1;
   TheRect.Right = Rect.Right+1;
   TheRect.Bottom = Rect.Bottom+1;

   if(!shiftCalculationList.size()) return;
   switch(ACol) {
      case 0:
         ShiftCalculationDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,shiftCalculationList[ARow].type_calculation_name);
         ShiftCalculationDrawGrid->Canvas->Brush->Color = clBtnFace;
         ShiftCalculationDrawGrid->Canvas->FrameRect(TheRect);
         break;
      case 1:
         ShiftCalculationDrawGrid->Canvas->TextOut(Rect.Right-2-ShiftCalculationDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(shiftCalculationList[ARow].suma)),Rect.Top+2,TFormatingData::DblToStr(shiftCalculationList[ARow].suma));
         ShiftCalculationDrawGrid->Canvas->Brush->Color = clBtnFace;
         ShiftCalculationDrawGrid->Canvas->FrameRect(TheRect);
         break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::ZakazCalculationDrawGridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
   TRect TheRect;
   TheRect.Left = Rect.Left-1;
   TheRect.Top = Rect.Top-1;
   TheRect.Right = Rect.Right+1;
   TheRect.Bottom = Rect.Bottom+1;

   if( !zakazCalculationList.size() ) return;
   switch(ACol) {
      case 0:
         ZakazCalculationDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,zakazCalculationList[ARow].type_calculation_name);
         ZakazCalculationDrawGrid->Canvas->Brush->Color = clBtnFace;
         ZakazCalculationDrawGrid->Canvas->FrameRect(TheRect);
         break;
      case 1:
         ZakazCalculationDrawGrid->Canvas->TextOut(Rect.Right-2-ZakazCalculationDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(zakazCalculationList[ARow].suma)),Rect.Top+2,TFormatingData::DblToStr(zakazCalculationList[ARow].suma));
         ZakazCalculationDrawGrid->Canvas->Brush->Color = clBtnFace;
         ZakazCalculationDrawGrid->Canvas->FrameRect(TheRect);
         break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
   vector <THeadersBuilder *> HeadTitles;
                              HeadTitles.push_back(HeadTitleCheck);
                              HeadTitles.push_back(HeadTitleTovar);
   TSubMethods::SaveFormParams( "Partner.ini", this, HeadTitles );
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::Splitter1Moved(TObject *Sender)
{
   FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::CheckDrawGridMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::TovDrawGridMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::CashDblClick(TObject *Sender)
{
   // Внесення/видача готівки по зміні
   TFShiftCash *FShiftCash = new TFShiftCash (shift_ID,NULL);
   FShiftCash->ShowModal();
   delete FShiftCash;
}
//---------------------------------------------------------------------------

void __fastcall TFShiftDoc::StanShiftDblClick(TObject *Sender)
{
   TShiftSQL::CreateDocumentFromShiftDoc();        
}
//---------------------------------------------------------------------------

