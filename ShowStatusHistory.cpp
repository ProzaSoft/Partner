//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShowStatusHistory.h"
#include "StatusHistoryExellio.h"
#include "FormatingData.h"
#include "Manual.h"
#include "MyCalendar.h"
#include "DBService.h"
#include "WorkFiles.h"
//---------------------------------------------------------------------------
#include "Reports.h"
//---------------------------------------------------------------------------
extern void RegistryForm( void *document, char *title );
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFShowStatusHistory *FShowStatusHistory;
//---------------------------------------------------------------------------

__fastcall TFShowStatusHistory::TFShowStatusHistory(TComponent* Owner)
        : TForm(Owner)
{
   tovar_ID = 0;
   //
   cash_register_ID = 0;
   CashRegisterName->Text = "Всі точки продажу";
   dep_ID = 0;
   DepName->Text = "Всі склади і відділи";
   user_ID = 0;
   UserName->Text = "Всі касири";
}
//---------------------------------------------------------------------------

__fastcall TFShowStatusHistory::TFShowStatusHistory( int tovar_id, int dep_id, String beg_date, String end_date, TComponent* Owner)
        : TForm(Owner)
{
   tovar_ID = tovar_id;
   //
//   String b_date = TWorkFiles::GetDataFromIniFile( "Partner.ini", "audit_history", "beg_date", DateToStr(Now()));
//   if( StrToDate(b_date) > StrToDate(beg_date) )
//      BegDate->Text = FIELD::ConvertDateSystemPartner(beg_date);
//   EndDate->Text = FIELD::ConvertDateSystemPartner(end_date);
   //
   cash_register_ID = 0;
   CashRegisterName->Text = "Всі точки продажу";
   dep_ID = dep_id;
   DepName->Text = "";
   user_ID = 0;
   UserName->Text = "Всі касири";
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::FormShow(TObject *Sender)
{
   BegDate->Text = FIELD::ConvertDateSystemPartner(TWorkFiles::GetDataFromIniFile( "Partner.ini", "audit_history", "beg_date", DateToStr(Now())));
   EndDate->Text = FIELD::ConvertDateSystemPartner(TWorkFiles::GetDataFromIniFile( "Partner.ini", "audit_history", "end_date", DateToStr(Now())));
   //
   if( tovar_ID > 0 ) {
      KodEdit->Text = String(tovar_ID);
      FindTovar();
   }
   if( dep_ID > 0 ) {
      int agent_id = atoi(TDBService::get_from_ID("department","agent_id","dep_id",String(dep_ID)).c_str());
      DepName->Text = TDBService::get_from_ID("agents","name","agent_id",String(agent_id));
   }
   //
   TStatusHistorySQL::LoadStatuses( statusList );
   StatusDrawGrid->RowCount = statusList.size()?statusList.size()+1:2;
   StatusDrawGrid->Refresh();
   //
   RefreshAction();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::DepImageClick(TObject *Sender)
{
   TFManual *pDetail = new TFManual(String("StoreDepartment"),DepName->ClientOrigin.x,DepName->ClientOrigin.y+DepName->Height-1,DepName->Width,String(""),DepName);
   int result = pDetail->ShowModal();
   if ( result == 1 ) {
      DepName->Text = pDetail->name_GET();
      dep_ID = atoi(TDBService::get_from_ID("department","dep_id","agent_id",String(pDetail->kod_GET())).c_str());
   }
   else {
      DepName->Text = "Всі склади і відділи";
      dep_ID = 0;
   }
   delete pDetail;
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::UserImageClick(TObject *Sender)
{
   TFManual *pDetail = new TFManual(String("Performer"),UserName->ClientOrigin.x,UserName->ClientOrigin.y+UserName->Height-1,UserName->Width,String(""),UserName);
   int result = pDetail->ShowModal();
   if ( result == 1 ) {
      UserName->Text = pDetail->name_GET();
      user_ID = atoi(TDBService::get_from_ID("employee","employee_id","agent_id",String(pDetail->kod_GET())).c_str());
   }
   else {
      UserName->Text = "Всі касири";
      user_ID  = 0;
   }
   delete pDetail;
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::CashRegisterImageClick(TObject *Sender)
{
   TFManual *pDetail = new TFManual(String("CashRegister"),CashRegisterName->ClientOrigin.x,CashRegisterName->ClientOrigin.y+CashRegisterName->Height-1,CashRegisterName->Width,String(""),CashRegisterName);
   int result = pDetail->ShowModal();
   if ( result == 1 ) {
      CashRegisterName->Text = pDetail->name_GET();
      cash_register_ID = pDetail->kod_GET();
   }
   else {
      CashRegisterName->Text = "Всі точки продажу";
      cash_register_ID = 0;
   }
   delete pDetail;
}
//---------------------------------------------------------------------------

void TFShowStatusHistory::FindTovar()
{
   if( KodEdit->Text=="" || KodEdit->Text=="Код ЕККА" ){
      tovar_ID = 0;
      KodEdit->Text="Код ЕККА";
      TovEdit->Text = "Всі товари";
      return;
   }
   TovEdit->Text = TTerminalAdminSQL::GetTovarByKodEkka( StrToIntDef(KodEdit->Text,0), tovar_ID );
   if( tovar_ID == 0 ) {
      KodEdit->Text = "Код ЕККА";
      TovEdit->Text = "Всі товари";
   }
}
//-------------------------------------------------------------------------

void TFShowStatusHistory::RefreshAction()
{
   TStatusHistorySQL::LoadActionsByTovarID( actionList, statusList, BegDate->Text, EndDate->Text,tovar_ID, user_ID, dep_ID );
   //
   TWorkFiles::SetDataIntoIniFile( "Partner.ini", "audit_history", "beg_date", BegDate->Text );
   TWorkFiles::SetDataIntoIniFile( "Partner.ini", "audit_history", "end_date", EndDate->Text );
   //
   ActionDrawGrid->RowCount = (!actionList.size()?2:(int)actionList.size()+1);
   //
   StatusDrawGrid->Refresh();
   ActionDrawGrid->Refresh();
   //
   ActCheckBox->Checked = false;
   SelectCheckBox->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::StatusDrawGridDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
   TRect TheRect;
   TheRect.Left = Rect.Left-1;
   TheRect.Top = Rect.Top-1;
   TheRect.Right = Rect.Right+1;
   TheRect.Bottom = Rect.Bottom+1;
   //
   if ( !ARow ) {
      int LeftPos=0;
      switch( ACol ) {
      case 0 : StatusDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(StatusDrawGrid->ColWidths[0]-StatusDrawGrid->Canvas->TextWidth("Статус")-LeftPos)/2,Rect.Top+1,"Статус");
               StatusDrawGrid->Canvas->Brush->Color = clBlack;
               StatusDrawGrid->Canvas->FrameRect(TheRect);
               break;
      }
   }
   else{
      if ( !statusList.size() ) return;
      switch( ACol ) {
         case 0 :
               if( statusList[ARow-1].selected )
                  ImageList->Draw(StatusDrawGrid->Canvas,3,(ARow+1-StatusDrawGrid->TopRow)*(StatusDrawGrid->DefaultRowHeight+1)+2,5,true);
               //
               StatusDrawGrid->Canvas->TextOut(Rect.Left+20,Rect.Top+1,statusList[ARow-1].name);
               break;
     }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::StatusDrawGridDblClick(TObject *Sender)
{
   if(!statusList.size()) return;
   statusList[StatusDrawGrid->Row-1].selected = !statusList[StatusDrawGrid->Row-1].selected;
   StatusDrawGrid->Refresh();
   RefreshAction();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::ActionDrawGridDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
   TRect TheRect;
   TheRect.Left = Rect.Left-1;
   TheRect.Top = Rect.Top-1;
   TheRect.Right = Rect.Right+1;
   TheRect.Bottom = Rect.Bottom+1;
   //
   if ( !ARow ) {
      switch( ACol ) {
         case 0:  ActionDrawGrid->Canvas->TextOut(Rect.Left+(ActionDrawGrid->ColWidths[0]-ActionDrawGrid->Canvas->TextWidth("Друк"))/2,Rect.Top+1,"Друк");
                  ActionDrawGrid->Canvas->Brush->Color = clBlack;
                  ActionDrawGrid->Canvas->FrameRect(TheRect);
                  break;

         case 1 : ActionDrawGrid->Canvas->TextOut(Rect.Left+(ActionDrawGrid->ColWidths[1]-ActionDrawGrid->Canvas->TextWidth("Час"))/2,Rect.Top+1,"Час");
                  ActionDrawGrid->Canvas->Brush->Color = clBlack;
                  ActionDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 2 : ActionDrawGrid->Canvas->TextOut(Rect.Left+(ActionDrawGrid->ColWidths[2]-ActionDrawGrid->Canvas->TextWidth("Дія"))/2,Rect.Top+1,"Дія");
                  ActionDrawGrid->Canvas->Brush->Color = clBlack;
                  ActionDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 3 : ActionDrawGrid->Canvas->TextOut(Rect.Left+(ActionDrawGrid->ColWidths[3]-ActionDrawGrid->Canvas->TextWidth("Кількість"))/2,Rect.Top+1,"Кількість");
                  ActionDrawGrid->Canvas->Brush->Color = clBlack;
                  ActionDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 4 : ActionDrawGrid->Canvas->TextOut(Rect.Left+(ActionDrawGrid->ColWidths[4]-ActionDrawGrid->Canvas->TextWidth("Ціна"))/2,Rect.Top+1,"Ціна");
                  ActionDrawGrid->Canvas->Brush->Color = clBlack;
                  ActionDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 5 : ActionDrawGrid->Canvas->TextOut(Rect.Left+(ActionDrawGrid->ColWidths[5]-ActionDrawGrid->Canvas->TextWidth("Сума"))/2,Rect.Top+1,"Сума");
                  ActionDrawGrid->Canvas->Brush->Color = clBlack;
                  ActionDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 6 : ActionDrawGrid->Canvas->TextOut(Rect.Left+(ActionDrawGrid->ColWidths[6]-ActionDrawGrid->Canvas->TextWidth("№чеку"))/2,Rect.Top+1,"№чеку");
                  ActionDrawGrid->Canvas->Brush->Color = clBlack;
                  ActionDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 7 : ActionDrawGrid->Canvas->TextOut(Rect.Left+(ActionDrawGrid->ColWidths[7]-ActionDrawGrid->Canvas->TextWidth("Користувач"))/2,Rect.Top+1,"Користувач");
                  ActionDrawGrid->Canvas->Brush->Color = clBlack;
                  ActionDrawGrid->Canvas->FrameRect(TheRect);
                  break;
      }
   }
   else {
      if ( !actionList.size() ) return;
      switch( ACol ) {
         case 0:
             if( actionList[ARow-1].selected )
                ImageList->Draw(ActionDrawGrid->Canvas,3,(ARow+1-ActionDrawGrid->TopRow)*(ActionDrawGrid->DefaultRowHeight+1)+2,5,true);
             break;
         case 1:
             ActionDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+1,actionList[ARow-1].date_d);
             break;
         case 2:
             ActionDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+1,actionList[ARow-1].comment);
             break;
         case 3:
             if( actionList[ARow-1].quantity )
                ActionDrawGrid->Canvas->TextOut(Rect.Right-2-ActionDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(actionList[ARow-1].quantity,3)),Rect.Top+1,TFormatingData::DblToStr(actionList[ARow-1].quantity,3));
             else
                ActionDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+1,actionList[ARow-1].quantity);
             break;
         case 4:
             if( actionList[ARow-1].price )
                ActionDrawGrid->Canvas->TextOut(Rect.Right-2-ActionDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(actionList[ARow-1].price)),Rect.Top+1,TFormatingData::DblToStr(actionList[ARow-1].price));
             else
                ActionDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+1,actionList[ARow-1].quantity);
             break;
         case 5:
             if( actionList[ARow-1].suma )
                ActionDrawGrid->Canvas->TextOut(Rect.Right-2-ActionDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(actionList[ARow-1].suma)),Rect.Top+1,TFormatingData::DblToStr(actionList[ARow-1].suma));
             else
                ActionDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+1,actionList[ARow-1].quantity);
             break;
         case 6:
             if(!actionList[ARow-1].doc_ID) return;
             ActionDrawGrid->Canvas->TextOut(Rect.Left+(ActionDrawGrid->ColWidths[3]-ActionDrawGrid->Canvas->TextWidth(actionList[ARow-1].doc_ID))/2,Rect.Top+1,actionList[ARow-1].doc_ID);
             break;
         case 7:
             ActionDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+1,actionList[ARow-1].user_name);
             break;
      }
   }
}
//---------------------------------------------------------------------------

void  TFShowStatusHistory::ColVisible()
{
   if( user_ID != 0 ) { // 7 стовч.
      ActionDrawGrid->ColCount=7;
      ActionDrawGrid->ColWidths[2] = (ActionDrawGrid->Width -
                                ActionDrawGrid->ColWidths[0] -
                                ActionDrawGrid->ColWidths[1] -
                                ActionDrawGrid->ColWidths[3] -
                                ActionDrawGrid->ColWidths[4] -
                                ActionDrawGrid->ColWidths[5] -
                                ActionDrawGrid->ColWidths[6] -
                                27);
   }
   else {
      ActionDrawGrid->ColCount=8;
      ActionDrawGrid->ColWidths[2] = (ActionDrawGrid->Width -
                                ActionDrawGrid->ColWidths[0] -
                                ActionDrawGrid->ColWidths[1] -
                                ActionDrawGrid->ColWidths[3] -
                                ActionDrawGrid->ColWidths[4] -
                                ActionDrawGrid->ColWidths[5] -
                                ActionDrawGrid->ColWidths[6] -
                                ActionDrawGrid->ColWidths[7] -
                                28);
   }
   ActionDrawGrid->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::FormResize(TObject *Sender)
{
   ColVisible();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::SelectCheckBoxClick(TObject *Sender)
{
   for( int i =0;i<(int)statusList.size();i++ ) {
      statusList[i].selected = SelectCheckBox->Checked;
   }
   RefreshAction();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::ActionDrawGridDblClick(TObject *Sender)
{
   if( !actionList.size() ) return;
   actionList[ActionDrawGrid->Row-1].selected = !actionList[ActionDrawGrid->Row-1].selected;
   //
   ActionDrawGrid->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::ActCheckBoxClick(TObject *Sender)
{
   for(int i =0;i<(int)actionList.size();i++)
      actionList[i].selected = ActCheckBox->Checked;
   ActionDrawGrid->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::Splitter1Moved(TObject *Sender)
{
   ColVisible();
   StatusDrawGrid->ColWidths[0] = (StatusDrawGrid->Width-20);
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::ActionDrawGridMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
   int index = ActionDrawGrid->TopRow + Y/(ActionDrawGrid->DefaultRowHeight+1);
   if ( index > 1 && index < (int)actionList.size()+2  )
      FullName->Caption = actionList[index-2].comment;
   else
      FullName->Caption = "";
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::KodEditClick(TObject *Sender)
{
   if( KodEdit->Text == "Код ЕККА" )
      KodEdit->Text="";
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::KodEditExit(TObject *Sender)
{
   FindTovar();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::KodEditKeyPress(TObject *Sender, char &Key)
{
   if( Key==VK_RETURN ) FindTovar();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::RefreshBtnClick(TObject *Sender)
{
   RefreshAction();
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::BegDateImgClick(TObject *Sender)
{
   TFMyCalendar *pCalendar = new TFMyCalendar(BegDate,this,1930,80);
   pCalendar->ShowModal();
   delete pCalendar;
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::Image2Click(TObject *Sender)
{
   TFMyCalendar *pCalendar = new TFMyCalendar(EndDate,this,1930,80);
   pCalendar->ShowModal();
   delete pCalendar;
}
//---------------------------------------------------------------------------


void __fastcall TFShowStatusHistory::CheckActionBtnClick(TObject *Sender)
{
   if ( !actionList.size() ) return;
   if ( !actionList[ActionDrawGrid->Row-1].doc_ID ) return;
   //
   TFStatusHistoryExellio *FStatusHistoryExellio = new TFStatusHistoryExellio (actionList[ActionDrawGrid->Row-1].doc_ID,NULL);
   FStatusHistoryExellio->ShowModal();
   delete FStatusHistoryExellio;
}
//---------------------------------------------------------------------------

void __fastcall TFShowStatusHistory::PrnBtnClick(TObject *Sender)
{
   if( !actionList.size() ) return;
   //
   TFReports *Reports=new TFReports(Application);
   Reports->Caption = "Події";
   Reports->frxReports->LoadFromFile(ExtractFileDir(ParamStr(0))+"\\Reports\\TerminalAdminReports\\FR_STATUS_HISTORY.fr3");
   //
   Reports->frxReports->Variables->Items[Reports->frxReports->Variables->IndexOf("BegDate")]->Value  = "\""+BegDate->Text+"\"";
   Reports->frxReports->Variables->Items[Reports->frxReports->Variables->IndexOf("EndDate")]->Value  = "\""+EndDate->Text+"\"";
   Reports->frxReports->Variables->Items[Reports->frxReports->Variables->IndexOf("ActiveParam")]->Value  = "\""+DepName->Text+".  "+UserName->Text+".  "+TovEdit->Text+"."+"\"";
   //
   Reports->ClientDataSet->FieldDefs->Clear();
   Reports->ClientDataSet->FieldDefs->Add("date_time",ftString,255);
   Reports->ClientDataSet->FieldDefs->Add("tovar_id",ftString,255);
   Reports->ClientDataSet->FieldDefs->Add("name",ftString,255);
   Reports->ClientDataSet->FieldDefs->Add("quantity",ftFloat,0);
   Reports->ClientDataSet->FieldDefs->Add("price",ftFloat,0);
   Reports->ClientDataSet->FieldDefs->Add("suma",ftFloat,0);
   Reports->ClientDataSet->FieldDefs->Add("doc_id",ftString,10);
   Reports->ClientDataSet->FieldDefs->Add("user_name",ftString,100);
   Reports->ClientDataSet->CreateDataSet();
   //
   for (int i = 0; i < (int) actionList.size(); i++) {
      if( !actionList[i].selected ) continue;
      Reports->ClientDataSet->Append();
      Reports->ClientDataSet->FieldByName("date_time")->Value = actionList[i].date_d;
      Reports->ClientDataSet->FieldByName("tovar_id")->Value  = actionList[i].tovar_ID;
      Reports->ClientDataSet->FieldByName("name")->Value      = actionList[i].comment;
      Reports->ClientDataSet->FieldByName("quantity")->Value  = actionList[i].quantity;
      Reports->ClientDataSet->FieldByName("price")->Value     = actionList[i].price;
      Reports->ClientDataSet->FieldByName("suma")->Value      = actionList[i].suma;
      Reports->ClientDataSet->FieldByName("doc_id")->Value    = actionList[i].doc_ID;
      Reports->ClientDataSet->FieldByName("user_name")->Value = actionList[i].user_name;
      Reports->ClientDataSet->Post();
   }
   //
   RegistryForm(Reports,"Перегляд");
}
//---------------------------------------------------------------------------

