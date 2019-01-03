//---------------------------------------------------------------------------

#include "vcl.h"
#pragma hdrstop

#include "CheckByNumber.h"
//
#include "SubMethods.h"
#include "FormatingData.h"
#include "Config.h"
#include "StatusHistoryExellio.h"
#include "DBService.h"
//---------------------------------------------------------------------------
#include "Reports.h"
//---------------------------------------------------------------------------

extern DConnect *pConnect;

extern void RegistryForm( void *document, char *title );

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFCheckByNumber *FCheckByNumber;
//---------------------------------------------------------------------------
__fastcall TFCheckByNumber::TFCheckByNumber(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TFCheckByNumber::ExitBtnClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TFCheckByNumber::DocDrawGridDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
   TRect TheRect;
   TheRect.Left = Rect.Left-1;
   TheRect.Top = Rect.Top-1;
   TheRect.Right = Rect.Right+1;
   TheRect.Bottom = Rect.Bottom+1;
   //
   if ( !ARow ) {
      int LeftPos = 0;
      switch( ACol ) {
         case 0 : DocDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocDrawGrid->ColWidths[0]-DocDrawGrid->Canvas->TextWidth("ID")-LeftPos)/2,Rect.Top+1,"ID");
                  DocDrawGrid->Canvas->Brush->Color = clBlack;
                  DocDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 1 : DocDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocDrawGrid->ColWidths[1]-DocDrawGrid->Canvas->TextWidth("Дата")-LeftPos)/2,Rect.Top+1,"Дата");
                  DocDrawGrid->Canvas->Brush->Color = clBlack;
                  DocDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 2 : DocDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocDrawGrid->ColWidths[2]-DocDrawGrid->Canvas->TextWidth("Час")-LeftPos)/2,Rect.Top+1,"Час");
                  DocDrawGrid->Canvas->Brush->Color = clBlack;
                  DocDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 3 : DocDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocDrawGrid->ColWidths[3]-DocDrawGrid->Canvas->TextWidth("Працівник")-LeftPos)/2,Rect.Top+1,"Працівник");
                  DocDrawGrid->Canvas->Brush->Color = clBlack;
                  DocDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 4 : DocDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocDrawGrid->ColWidths[4]-DocDrawGrid->Canvas->TextWidth("Сума")-LeftPos)/2,Rect.Top+1,"Сума");
                  DocDrawGrid->Canvas->Brush->Color = clBlack;
                  DocDrawGrid->Canvas->FrameRect(TheRect);
                  break;
         case 5 : DocDrawGrid->Canvas->TextOut(Rect.Left+LeftPos+(DocDrawGrid->ColWidths[5]-DocDrawGrid->Canvas->TextWidth("Зміна")-LeftPos)/2,Rect.Top+1,"Зміна");
                  DocDrawGrid->Canvas->Brush->Color = clBlack;
                  DocDrawGrid->Canvas->FrameRect(TheRect);
                  break;
      }
   }
   else {
      if ( !docList.size() ) return;
      //
      if ( DocDrawGrid->Row == ARow && DocDrawGrid->Col == ACol ) {
         DocDrawGrid->Canvas->Brush->Color = clNavy;
         DocDrawGrid->Canvas->Font->Color = clWhite;
      }
      DocDrawGrid->Canvas->FillRect(Rect);
      //
      switch( ACol ) {
         case 0 : DocDrawGrid->Canvas->TextOut(Rect.Right-2-DocDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(docList[ARow-1].zakaz_id,0)),Rect.Top+2,TFormatingData::DblToStr(docList[ARow-1].zakaz_id,0 ));
                  break;
         case 1 : DocDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,String("")+docList[ARow-1].date_check );
                  break;
         case 2 : DocDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,String("")+docList[ARow-1].time_check );
                  break;
         case 3 : DocDrawGrid->Canvas->TextOut(Rect.Left+1,Rect.Top+2,docList[ARow-1].employee_name);
                  break;
         case 4 : DocDrawGrid->Canvas->TextOut(Rect.Right-2-DocDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(docList[ARow-1].suma)),Rect.Top+2,TFormatingData::DblToStr(docList[ARow-1].suma ));
                  break;
         case 5 : DocDrawGrid->Canvas->TextOut(Rect.Right-2-DocDrawGrid->Canvas->TextWidth(TFormatingData::DblToStr(docList[ARow-1].check_shift.shift_ID,0)),Rect.Top+2,TFormatingData::DblToStr(docList[ARow-1].check_shift.shift_ID,0 ));
                  break;
     }
   }
}
//---------------------------------------------------------------------------

void __fastcall TFCheckByNumber::FormResize(TObject *Sender)
{
   int colWidth = 0;
   for( int i=0; i<DocDrawGrid->ColCount; i++ )
      if( i!=3 ) colWidth += DocDrawGrid->ColWidths[i]+1;
   DocDrawGrid->ColWidths[3] = DocDrawGrid->Width-colWidth-21;
}
//---------------------------------------------------------------------------

void __fastcall TFCheckByNumber::NumberKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   int number_check = atoi(Number->Text.c_str());
   if ( Key == VK_RETURN && number_check ) {
      TShiftSQL::LoadChecksByNumber( docList, number_check );
      //
      if ( docList.size() ) {
         DocDrawGrid->RowCount = docList.size()+1;
         DocDrawGrid->Row = docList.size();
      }
      else
         DocDrawGrid->RowCount = 2;
      DocDrawGrid->Refresh();
   }
   else if ( Key == VK_ESCAPE ) {
      Number->SelectAll();
   }
}
//---------------------------------------------------------------------------

void __fastcall TFCheckByNumber::PrnBtnClick(TObject *Sender)
{
   char curDir[120];
   GetCurrentDirectory(100,curDir);
   //
   TFReports *Reports=new TFReports(Application);
   //
   try {
      String query = String("");
      Reports->frxReports->LoadFromFile(String(curDir)+"\\Reports\\TerminalAdminReports\\FR_CheckPrn.fr3");
      //
      Reports->Caption = String("Друк чеку №")+Number->Text;
      //
      Reports->frxMSQuery->SetConnected(pConnect);
      query = String("")+
         " SELECT z.number, z.date_z, z.time_close, z.discount_card_id, "+
         "        ae.name as user_name, dc.number as discount_card_number, dc.owner_name as discount_card_info, "+
         "        zr.quantity, zr.price, zr.suma, zr.suma_discount, z.bonus, "+
         "        if( (t.vyrobnyk_id !=0 AND v.name IS NOT NULL) ,CONCAT(v.name,' ',t.name),t.name) as tovar_name, "+
         "        o.short_name as ov_name, t.kod_ekka, t.price_in  "+
         " FROM r_zakaz_row zr "+
         "    LEFT JOIN r_zakaz z ON  (zr.zakaz_id = z.zakaz_id) "+
         "    LEFT JOIN t_tovar t ON  (zr.tovar_id = t.tovar_id) "+
         "    LEFT JOIN t_group_tov gr ON  (t.group_tov_id = gr.group_tov_id) "+
         "    LEFT JOIN vyrobnyk v ON  (t.vyrobnyk_id = v.vyrobnyk_id) "+
         "    LEFT JOIN t_ov o ON  (t.ov_id = o.ov_id) "+
         "    LEFT JOIN employee e   ON z.employee_id  = e.employee_id "+
         "    LEFT JOIN agents ae    ON e.agent_id = ae.agent_id "+
         "    LEFT JOIN r_discount_card dc ON z.discount_card_id=dc.discount_card_id "
         " WHERE zr.zakaz_id="+docList[DocDrawGrid->Row-1].zakaz_id+" "+
         " ORDER BY zr.zakaz_row_id ";
      //
      Reports->frxMSQuery->SetSQL(query);
      Reports->frxMSQuery->QueryOpen=true;
      //
      Reports->frxMSQuery1->SetConnected(pConnect);
      query = String("")+
         " SELECT t.name as type_calculation_name, zc.suma "+
         " FROM r_zakaz_calculation zc "+
         "    LEFT JOIN r_type_calculation t ON (zc.type_calculation_id = t.type_calculation_id) "+
         " WHERE zc.zakaz_id="+docList[DocDrawGrid->Row-1].zakaz_id+" "+
         " ORDER BY zc.zakaz_calculation_id ";
      //
      Reports->frxMSQuery1->SetSQL(query);
      Reports->frxMSQuery1->QueryOpen=true;
      //
      Reports->frxReports->Variables->Items[Reports->frxReports->Variables->IndexOf("bonus_on")]->Value =
         atoi(TDBService::get_from_ID("const_default","strvalue","name_const","BONUS_ON").c_str());
      //
      RegistryForm(Reports,"Перегляд");
   }
   catch( Exception &E )
   {
      MessageDlg( String("Помилка Reports()!\n")+String(E.ClassName())+"\n"+ E.Message, mtError, TMsgDlgButtons() << mbOK, 0);
   }
}
//---------------------------------------------------------------------------

void __fastcall TFCheckByNumber::SHBitBtnClick(TObject *Sender)
{
   if( !docList.size() ) return;
   //
   TFStatusHistoryExellio *FStatusHistoryExellio = new TFStatusHistoryExellio (docList[DocDrawGrid->Row-1].zakaz_id,Application);
   FStatusHistoryExellio->ShowModal();
   delete FStatusHistoryExellio;
}
//---------------------------------------------------------------------------

