//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Shift.h"
#include "ShiftDoc.h"
#include "CloseShift.h"
#include "ShiftCheck.h"
#include "ShiftDocuments.h"
#include "FormatingData.h"
#include "SubMethods.h"
#include "CheckByNumber.h"
//---------------------------------------------------------------------------
#include "Manual.h"
#include "MyCalendar.h"
#include "DBService.h"
#include "WorkFiles.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFShift *FShift;
//---------------------------------------------------------------------------
__fastcall TFShift::TFShift(TComponent* Owner)
        : TForm(Owner)
{
   HeadTitle = new THeadersBuilder(ShiftDrawGrid,5);
   //
   SetTitleCaption();
}
//---------------------------------------------------------------------------
void TFShift::SetTitleCaption( void )
{
   HeadTitle->Clear();
   //
   HeadTitle->Add("№ зміни");
   HeadTitle->Add("Початок зміни");
   HeadTitle->Add("Кінець зміни");
   HeadTitle->Add("     Касир     ");
   HeadTitle->Add("РРО");
   HeadTitle->Add("  № Z-звіту  ");
   HeadTitle->Add("      Стан      ");
   //
   HeadTitle->Activate();
}
//---------------------------------------------------------------------------
void __fastcall TFShift::ShiftDrawGridDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
   TRect TheRect;
   TheRect.Left = Rect.Left-1;
   TheRect.Top = Rect.Top-1;
   TheRect.Right = Rect.Right+1;
   TheRect.Bottom = Rect.Bottom+1;
   //
   if(!ARow){
      HeadTitle->DrawTitle(ACol,Rect,clBtnFace);
   }
   else {
      if( !shiftList.size() ) return;
      switch(ACol) {
         case 0:
            ShiftDrawGrid->Canvas->TextOut(Rect.Right-2-ShiftDrawGrid->Canvas->TextWidth(String(shiftList[ARow-1].shift_ID)),Rect.Top+2,shiftList[ARow-1].shift_ID);
            ShiftDrawGrid->Canvas->Brush->Color = clBtnFace;
            ShiftDrawGrid->Canvas->FrameRect(TheRect);
            break;
         case 1:
            ShiftDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,shiftList[ARow-1].datetime_beg);
            ShiftDrawGrid->Canvas->Brush->Color = clBtnFace;
            ShiftDrawGrid->Canvas->FrameRect(TheRect);
	    break;
         case 2:
            ShiftDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,shiftList[ARow-1].datetime_end);
	    ShiftDrawGrid->Canvas->Brush->Color = clBtnFace;
            ShiftDrawGrid->Canvas->FrameRect(TheRect);
	    break;
         case 3:
            ShiftDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,shiftList[ARow-1].user_name);
	    ShiftDrawGrid->Canvas->Brush->Color = clBtnFace;
            ShiftDrawGrid->Canvas->FrameRect(TheRect);
	    break;
         case 4:
            ShiftDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,String("[")+shiftList[ARow-1].agent_name+"]   "+shiftList[ARow-1].cash_register_ID+" "+shiftList[ARow-1].cash_register_name);
	    ShiftDrawGrid->Canvas->Brush->Color = clBtnFace;
            ShiftDrawGrid->Canvas->FrameRect(TheRect);
	    break;
         case 5:
            ShiftDrawGrid->Canvas->TextOut(Rect.Right-2-ShiftDrawGrid->Canvas->TextWidth(String(shiftList[ARow-1].zzvit_number)),Rect.Top+2,shiftList[ARow-1].zzvit_number);
            ShiftDrawGrid->Canvas->Brush->Color = clBtnFace;
            ShiftDrawGrid->Canvas->FrameRect(TheRect);
            break;
         case 6:
            String stan = "";
            int curColor = clBlack;
            switch( shiftList[ARow-1].stan ) {
               case 0  : stan = " Відкрита";
                         curColor = clBlue;
                         break;
               case 1  : stan = "Завершена";
                         curColor = clGreen;
                         break;
               case 2  : stan = "  Закрита";
                         curColor = clBlack;
                         break;
               default : stan = "Невідомий";
                         curColor = clBlack;
                         break;
            }
            ShiftDrawGrid->Canvas->Font->Color = curColor;
            //
            if ( ShiftDrawGrid->Row == ARow ) {
               ShiftDrawGrid->Canvas->Font->Color = clWhite;
            }
            //
            ShiftDrawGrid->Canvas->TextOut(Rect.Left+2,Rect.Top+2,stan);
            break;
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TFShift::RefreshBtnClick(TObject *Sender)
{
   DocumentsBtn->Visible = false;
   // Обновити список робочих змін
   TShiftSQL::LoadShiftList(shiftList,StanRadioGroup->ItemIndex-1, dep_ID, BegDate->Text, EndDate->Text);
   //
   if ( shiftList.size() ) {
      ShiftDrawGrid->RowCount = shiftList.size() + 1;
      ShiftDrawGrid->Row = 1;
   }
   else
      ShiftDrawGrid->RowCount = 2;
   //
   ShiftDrawGrid->Refresh();
   //
   RefreshBtn->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFShift::FormResize(TObject *Sender)
{
   int width_float_col = ShiftDrawGrid->Width - 18; // 17+1 - ширина полоси прокрутки
   //
   for( int i = 0; i < ShiftDrawGrid->ColCount; i++ ) {
      if ( i == HeadTitle->FloatCol_GET() - 1 ) continue; // колонка зі змінною шириною
      width_float_col -= (ShiftDrawGrid->ColWidths[i] + 1);
   }
   //
   ShiftDrawGrid->ColWidths[HeadTitle->FloatCol_GET() - 1] = width_float_col;
}
//---------------------------------------------------------------------------

void __fastcall TFShift::ConditionsChange(TObject *Sender)
{
   RefreshBtn->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFShift::CloseShiftBtnClick(TObject *Sender)
{
   if( !shiftList.size() ) return;
   //
   if( shiftList[ShiftDrawGrid->Row-1].stan != 1 ) {
      MessageDlg( String("Закривати можна лише ЗАВЕРШЕНІ зміни!"), mtError,
                                         TMsgDlgButtons() << mbOK, 0);
      return;
   }
   //
   TFCloseShift *FCloseShift = new TFCloseShift (shiftList[ShiftDrawGrid->Row-1].shift_ID,NULL);
   FCloseShift->ShowModal();
   delete FCloseShift;
   //
   RefreshBtnClick(Sender);
}
//---------------------------------------------------------------------------

extern void RegistryForm( void *document, char *title );

void __fastcall TFShift::CheckBtnClick(TObject *Sender)
{
   if( !shiftList.size() ) return;
   //
   TFShiftCheck *FShiftCheck = new TFShiftCheck (shiftList[ShiftDrawGrid->Row-1].shift_ID,Application);
   RegistryForm(FShiftCheck,String("Чеки по зміні "+String(shiftList[ShiftDrawGrid->Row-1].shift_ID)).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFShift::ShiftDrawGridSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
   if( !shiftList.size() ) return;
   // Для закритих змін відкриваємо кнопку з сформованими документами
   if( shiftList[ARow-1].stan == 2 )
      DocumentsBtn->Visible = true;
   else
      DocumentsBtn->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFShift::DocumentsBtnClick(TObject *Sender)
{
   if( !shiftList.size() ) return;
   //
   FShiftDocuments = new TFShiftDocuments(shiftList[ShiftDrawGrid->Row-1].shift_ID,Application);
   FShiftDocuments->ShowModal();
   delete FShiftDocuments;
}
//---------------------------------------------------------------------------

void __fastcall TFShift::PrnBtnClick(TObject *Sender)
{
   if( !shiftList.size() ) return;
   //
   TFCheckByNumber *FCheckByNumber = new TFCheckByNumber(Application);
   RegistryForm(FCheckByNumber,String("Пошук по номеру чека").c_str());
}
//---------------------------------------------------------------------------


void __fastcall TFShift::ShiftDrawGridMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFShift::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   TSubMethods::SaveFormParams( "Partner.ini", this, HeadTitle );
}
//---------------------------------------------------------------------------

void __fastcall TFShift::FormShow(TObject *Sender)
{
   TSubMethods::RestoreFormParams( "Partner.ini", this, HeadTitle );
   //
   BegDate->Text = FIELD::ConvertDateSystemPartner(DateToStr(Now()-5));
   EndDate->Text = FIELD::ConvertDateSystemPartner(DateToStr(Now()));
   // Згідно логіна визначаємо чи всі відділи можна переглядати
   String department_name = "Всі склади і відділи";
   dep_ID = TShiftSQL::GetDepartmentInfo(department_name);
   DepName->Text = department_name;
   if( dep_ID ) { // Забороняємо вибір
      DepImage->Visible = false;
      DepName->OnKeyDown = NULL;
   }
   //
   RefreshBtnClick(Sender);
   //
   if( StrToIntDef(TWorkFiles::GetDataFromIniFile("Partner.ini","SHIFT_ADMIN","SHIFT_ADMIN_ON","0"),0) ) {
      ShiftDrawGrid->PopupMenu = AdminPopupMenu;
      NChangeStan->Visible = StrToIntDef(TWorkFiles::GetDataFromIniFile("Partner.ini","SHIFT_ADMIN","CAN_CHANGE_STAN","0"),0);
   }
   else {
      ShiftDrawGrid->PopupMenu = NULL;
      NChangeStan->Visible = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShift::BegDateImgClick(TObject *Sender)
{
   TFMyCalendar *pCalendar = new TFMyCalendar(BegDate,this,1930,80);
   pCalendar->ShowModal();
   delete pCalendar;
}
//---------------------------------------------------------------------------

void __fastcall TFShift::Image2Click(TObject *Sender)
{
   TFMyCalendar *pCalendar = new TFMyCalendar(EndDate,this,1930,80);
   pCalendar->ShowModal();
   delete pCalendar;
}
//---------------------------------------------------------------------------

void __fastcall TFShift::DepImageClick(TObject *Sender)
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

void __fastcall TFShift::DepNameKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if ( Key == 13 ) {
      dep_ID = 0;
      DepName->Text = "Всі склади і відділи";
      //
      Key = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShift::BitBtn1Click(TObject *Sender)
{
   if( !shiftList.size() ) return;
   //
   TFShiftDoc *FShiftDoc = new TFShiftDoc (shiftList[ShiftDrawGrid->Row-1].shift_ID,Application);
   RegistryForm(FShiftDoc,String("Документи по зміні "+String(shiftList[ShiftDrawGrid->Row-1].shift_ID)).c_str());
}
//---------------------------------------------------------------------------


void __fastcall TFShift::ShiftDrawGridKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if ( Key == VK_F5 && Shift.Contains(ssAlt) && Shift.Contains(ssShift) && Shift.Contains(ssCtrl) ) { // Ctrl-Alt-Shift-F5
      if( ShiftDrawGrid->PopupMenu == AdminPopupMenu )
         NDeleteShift->Visible = StrToIntDef(TWorkFiles::GetDataFromIniFile("Partner.ini","SHIFT_ADMIN","CAN_DELETE_SHIFT","0"),0);
      else
         NDeleteShift->Visible = false;
      NDuplicate->Visible = false;
      Key = 0;
   }
   else if ( Key == VK_F6 && Shift.Contains(ssAlt) && Shift.Contains(ssShift) && Shift.Contains(ssCtrl) ) { // Ctrl-Alt-Shift-F6
      if( ShiftDrawGrid->PopupMenu == AdminPopupMenu )
         NDuplicate->Visible = StrToIntDef(TWorkFiles::GetDataFromIniFile("Partner.ini","SHIFT_ADMIN","DUPLICATE","0"),0);
      else
         NDuplicate->Visible = false;
      NDeleteShift->Visible = false;
      Key = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShift::NChangeStanClick(TObject *Sender)
{
   if( !shiftList.size() ) return;
   //
   if( shiftList[ShiftDrawGrid->Row-1].stan != 2 ) {
      MessageDlg( String("Відкривати можна лише ЗАКРИТІ зміни!"), mtError,
                                         TMsgDlgButtons() << mbOK, 0);
      return;
   }
   //
   if ( MessageDlg( String("Повернути зміну ")+shiftList[ShiftDrawGrid->Row-1].shift_ID+" в стан ЗАВЕРШЕНА?", mtConfirmation,
                   TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes ) {
      TShiftSQL::SetShiftStan(shiftList[ShiftDrawGrid->Row-1].shift_ID,1);
      //
      RefreshBtnClick(Sender);
      return;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShift::NDeleteShiftClick(TObject *Sender)
{
   if( !shiftList.size() ) return;
   //
   if ( MessageDlg( String("Вилучити зміну ")+shiftList[ShiftDrawGrid->Row-1].shift_ID+"?", mtConfirmation,
                   TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes ) {
      ShiftDrawGrid->Cursor = crHourGlass;
      TShiftSQL::DeleteShift(shiftList[ShiftDrawGrid->Row-1].shift_ID);
      ShiftDrawGrid->Cursor = crDefault;
      //
      RefreshBtnClick(Sender);
      //
      NDeleteShift->Visible = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFShift::NDuplicateClick(TObject *Sender)
{
   if( !shiftList.size() ) return;
   //
   if ( MessageDlg( String("Перевірити зміну ")+shiftList[ShiftDrawGrid->Row-1].shift_ID+" на дублікати?", mtConfirmation,
                   TMsgDlgButtons() << mbYes << mbNo, 0) != mrYes ) return;
   //
   ShiftDrawGrid->Cursor = crHourGlass;
   TShiftSQL::DeleteShiftDuplicate(shiftList[ShiftDrawGrid->Row-1].shift_ID);
   ShiftDrawGrid->Cursor = crDefault;
   //
   RefreshBtnClick(Sender);
   //
   NDuplicate->Visible = false;
}
//---------------------------------------------------------------------------

