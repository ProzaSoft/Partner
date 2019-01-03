//---------------------------------------------------------------------------

#include <vcl.h>
#include <winsock.h>
#pragma hdrstop

#include "StatusHistoryExellio.h"
//---------------------------------------------------------------------------
#include "Reports.h"
//---------------------------------------------------------------------------
extern void RegistryForm( void *document, char *title );
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFStatusHistoryExellio *FStatusHistoryExellio;
//---------------------------------------------------------------------------
__fastcall TFStatusHistoryExellio::TFStatusHistoryExellio(TComponent* Owner)
        : TForm(Owner)
{
   doc_ID = 0;
}
//
__fastcall TFStatusHistoryExellio::TFStatusHistoryExellio(int doc_id, TComponent* Owner)
        : TForm(Owner)
{
   doc_ID = doc_id;
}
//---------------------------------------------------------------------------
void __fastcall TFStatusHistoryExellio::StatusDrawGridDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
   if ( !status.size() ) return;
   //
   StatusDrawGrid->Canvas->Brush->Color = clWhite;
   StatusDrawGrid->Canvas->Font->Color = clBlack;
   StatusDrawGrid->Canvas->FillRect(Rect);
   //
   switch( ACol ) {
      case 0 : StatusDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,status[ARow].date_d); break;
      case 1 : StatusDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,status[ARow].user_name); break;
      case 2 : StatusDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,status[ARow].comment); break;
   }
}
//---------------------------------------------------------------------------
void __fastcall TFStatusHistoryExellio::FormShow(TObject *Sender)
{
   TShiftSQL::LoadDocHistoryList(status,doc_ID);
   //
   StatusDrawGrid->RowCount = (status.size() ? status.size() : 1);
}
//---------------------------------------------------------------------------

void __fastcall TFStatusHistoryExellio::FormKeyPress(TObject *Sender, char &Key)
{
   if ( Key == 27 ) Close();
}
//---------------------------------------------------------------------------

void __fastcall TFStatusHistoryExellio::FormResize(TObject *Sender)
{
   StatusDrawGrid->ColWidths[2]=StatusDrawGrid->Width-
                                StatusDrawGrid->ColWidths[0]-
                                StatusDrawGrid->ColWidths[1]-23;
}
//---------------------------------------------------------------------------

void __fastcall TFStatusHistoryExellio::PrnBtnClick(TObject *Sender)
{
   if( !status.size() ) return;
   //
   TFReports *Reports=new TFReports(Application);
   Reports->Caption = "Події по чеку";
   Reports->frxReports->LoadFromFile(ExtractFileDir(ParamStr(0))+"\\Reports\\TerminalAdminReports\\FR_CHECK_STATUS_HISTORY.fr3");
   //
   Reports->ClientDataSet->FieldDefs->Clear();
   Reports->ClientDataSet->FieldDefs->Add("date_time",ftString,255);
   Reports->ClientDataSet->FieldDefs->Add("user_name",ftString,100);
   Reports->ClientDataSet->FieldDefs->Add("comment",ftString,255);
   Reports->ClientDataSet->CreateDataSet();
   //
   for (int i = 0; i < (int) status.size(); i++) {
      Reports->ClientDataSet->Append();
      Reports->ClientDataSet->FieldByName("date_time")->Value = status[i].date_d;
      Reports->ClientDataSet->FieldByName("user_name")->Value = status[i].user_name;
      Reports->ClientDataSet->FieldByName("comment")->Value   = status[i].comment;
      Reports->ClientDataSet->Post();
   }
   //
   RegistryForm(Reports,"Перегляд");
}
//---------------------------------------------------------------------------

void __fastcall TFStatusHistoryExellio::ExitBtnClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

