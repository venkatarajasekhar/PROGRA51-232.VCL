#include <vcl.h>
#include "Unit2.h"
#include "Unit4.h"
#include "Unit5.h"
#pragma hdrstop
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------

__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
EditedCellX = 1; EditedCellY = 1;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormCreate(TObject *Sender)
{
StringGrid1->ColWidths[0] = 30;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormShow(TObject *Sender)
{
if (Flash51->plik.Handle==INVALID_HANDLE_VALUE)
   {
    Flash51->OnError(0);
    return;
   }
if (Flash51->port.Handle==INVALID_HANDLE_VALUE)
   {
    Flash51->OnError(0);
    return;
   }
for (int X = 1; X <= 16; X++)
    {
     StringGrid1->Cells[X][0] = String::IntToHex(X-1,2);
    }
for (int Y = 1; Y <= Flash51->FLASH/16 ; Y++)
  {
  UpdateFileToGrid_Line(Y);
  UpdateGridToList_Line(Y);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::StringGrid1SetEditText(TObject *Sender, int ACol,
      int ARow, const AnsiString Value)
{
AnsiString tValue = Value;

if (ACol <= 16)
   {
   if (tValue.Length() > 2)
      {
       StringGrid1->Cells[ACol][ARow] = StringGrid1->Cells[ACol][ARow].Delete(1,1); return;
      }
   if (StrToIntDef("0x" + tValue,0xFF)!=0)
      {
       StringGrid1->Cells[17][ARow] = StringGrid1->Cells[17][ARow].Delete(ACol,1);
       StringGrid1->Cells[17][ARow] = StringGrid1->Cells[17][ARow].Insert((char)StrToIntDef("0x" + tValue,0xFF), ACol);
      }
   else
      {
       StringGrid1->Cells[17][ARow] = StringGrid1->Cells[17][ARow].Delete(ACol,1);
       StringGrid1->Cells[17][ARow] = StringGrid1->Cells[17][ARow].Insert(".", ACol);
      }
   }
else
   {
    for (int i = 1; i <= 16 && i <= tValue.Length() && i <= StringGrid1->Cells[17][ARow].Length(); i++)
        {
//        if ((int)tValue[i] != StrToIntDef("0x" + StringGrid1->Cells[i][ARow],0xFF))
           {
            StringGrid1->Cells[i][ARow] = IntToHex((unsigned short)tValue[i],2);
           }
        }
  return;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::StringGrid1SelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
StringGrid1KeyPress(Sender, 13);

EditedCellX = ACol; EditedCellY = ARow; EditedCellString = StringGrid1->Cells[ACol][ARow];
}
//---------------------------------------------------------------------------

void __fastcall TForm2::StringGrid1KeyPress(TObject *Sender, char &KEY)
{
if (KEY==13)
   {
    if (EditedCellX <= 16)
       {
        if (StringGrid1->Cells[EditedCellX][EditedCellY].Length() > 2)
           {
            StringGrid1->Cells[EditedCellX][EditedCellY] = StringGrid1->Cells[EditedCellX][EditedCellY].SetLength(16);
           }
        StringGrid1->Cells[EditedCellX][EditedCellY] = String::IntToHex(StrToIntDef("0x" + StringGrid1->Cells[EditedCellX][EditedCellY],0xFF) ,2);
       }
    else
       {
        if (StringGrid1->Cells[17][EditedCellY].Length() > 16)
           {
            StringGrid1->Cells[17][EditedCellY] = StringGrid1->Cells[17][EditedCellY].SetLength(16);
           }
        StringGrid1->Cells[17][EditedCellY] = StringGrid1->Cells[17][EditedCellY] + EditedCellString.Delete(1, StringGrid1->Cells[17][EditedCellY].Length());
       }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::StringGrid1Exit(TObject *Sender)
{
StringGrid1KeyPress(Sender, 13);
}
//---------------------------------------------------------------------------

void __fastcall TForm2::UpdateFileToGrid_Line(int Y)
{
StringGrid1->RowCount = Y + 1;

for (int X = 1; X <= 16; X++)
    {
     IntGrid_Calosc[X-1][Y-1] = 0xFF;
    }
for (int X = 1; X <= 16 ? (Y-1)*16 + (X-1) < Flash51->plik.Size : false; X++)
    {
     IntGrid_Calosc[X-1][Y-1] = Flash51->plik.Bufor[(Y-1)*16 + X-1];
    }
/* grid */

StringGrid1->Cells[0][Y] = String::IntToHex(Y-1,3);

for (int X = 1; X <= 16; X++)
    {
     StringGrid1->Cells[X][Y] = String::IntToHex(IntGrid_Calosc[X-1][Y-1],2);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::UpdateGridToFile_Line(int Y)
{
for (int X = 1; X <= 16; X++)
    {
     IntGrid_Calosc[X-1][Y-1] = StrToIntDef("0x" + StringGrid1->Cells[X][Y],0x00);
    }
for (int X = 1; X <= 16; X++)
    {
     Flash51->plik.Bufor[(Y-1)*16 + (X-1)] = IntGrid_Calosc[X-1][Y-1];
    }
Flash51->plik.Save();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::UpdateGridToList_Line(int Y)
{
StringGrid1->Cells[17][Y] = "";

for (int X = 1; X <= 16; X++)
    {
     if (StrToIntDef("0x" + Form2->StringGrid1->Cells[X][Y],0xFF)!=0)
        {
         StringGrid1->Cells[17][Y] = StringGrid1->Cells[17][Y] + (char)StrToIntDef("0x" + Form2->StringGrid1->Cells[X][Y],0xFF);
        }
     else
        {
         StringGrid1->Cells[17][Y] = StringGrid1->Cells[17][Y] + ".";
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Zamknij2Click(TObject *Sender)
{
Form2->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::StringGrid1DblClick(TObject *Sender)
{
StringGrid1->Options << goEditing;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Kopiuj1Click(TObject *Sender)
{
if (Form2->StringGrid1->Selection.Top == 17) return;

SelectionLeft   = Form2->StringGrid1->Selection.Left;
SelectionRight  = Form2->StringGrid1->Selection.Right;
SelectionTop    = Form2->StringGrid1->Selection.Top;
SelectionBottom = Form2->StringGrid1->Selection.Bottom;

for (int Y = SelectionTop; Y <= SelectionBottom; Y++)
    {
     for (int X = SelectionLeft; X <= SelectionRight; X++)
         {
          IntGrid_Selection[X-1][Y-1] = StrToIntDef("0x" + Form2->StringGrid1->Cells[X][Y],0xFF);
         }
    }
StatusBar1->Panels->Items[0]->Text = "Zaznaczenie w buforze = " + String((SelectionRight - SelectionLeft + 1)*(SelectionBottom - SelectionTop + 1)) + " Bajt";

Zastpobszar1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Zastpkopi1Click(TObject *Sender)
{
for (int Y = Form2->StringGrid1->Selection.Top, SelectionY = SelectionTop, X = Form2->StringGrid1->Selection.Left, SelectionX = SelectionLeft; SelectionY <= SelectionBottom; )
    {
     for (; SelectionX <= SelectionRight; SelectionX++)
         {
          Form2->StringGrid1->Cells[X][Y] = String::IntToHex((unsigned __int8)IntGrid_Selection[SelectionX-1][SelectionY-1],2);

          if (X < Form2->StringGrid1->Selection.Right)  { X+= 1; }
                                                  else  { X = Form2->StringGrid1->Selection.Left; UpdateGridToList_Line(Y); Y+= 1; }
          if (Y > Form2->StringGrid1->Selection.Bottom) { return; }
         }
    SelectionX = SelectionLeft; SelectionY+=1;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Przenietu1Click(TObject *Sender)
{
for (int Y = Form2->StringGrid1->Selection.Top, SelectionY = SelectionTop, X = Form2->StringGrid1->Selection.Left, SelectionX = SelectionLeft; SelectionY <= SelectionBottom; )
    {
     for (; SelectionX <= SelectionRight; SelectionX++)
         {
          Form2->StringGrid1->Cells[X][Y] = String::IntToHex((unsigned __int8)IntGrid_Selection[SelectionX-1][SelectionY-1],2);

          if (X < 16) { X+= 1; }
                 else { X = 1; UpdateGridToList_Line(Y); Y+= 1; }

          if (Y > Form2->StringGrid1->RowCount) { return; }
         }
    SelectionX = SelectionLeft; SelectionY+=1;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Usuzaznaczenie1Click(TObject *Sender)
{
for (int Y = Form2->StringGrid1->Selection.Top, DelY = Y, X = Form2->StringGrid1->Selection.Right, DelX = Form2->StringGrid1->Selection.Left; Y <= Form2->StringGrid1->RowCount; )
    {
     for (; X <= 16; X++)
         {
          if (Y <= Form2->StringGrid1->Selection.Bottom ? X < Form2->StringGrid1->Selection.Left || X > Form2->StringGrid1->Selection.Right : true)
             {
              Form2->StringGrid1->Cells[DelX][DelY] = Form2->StringGrid1->Cells[X][Y];

              if (DelX < 16) { DelX++; }
                        else { UpdateGridToList_Line(DelY); DelX = 1; DelY+=1; }
             }
         }
     X = 1; Y++;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Wklej1Click(TObject *Sender)
{
for (int Y = Form2->StringGrid1->Selection.Top, DelY = Y, X = Form2->StringGrid1->Selection.Right, DelX = Form2->StringGrid1->Selection.Left; Y <= Form2->StringGrid1->RowCount; )
    {
     for (; X <= 16; X++)
         {
          if (Y <= Form2->StringGrid1->Selection.Bottom ? X < Form2->StringGrid1->Selection.Left || X > Form2->StringGrid1->Selection.Right : true)
             {
              Form2->StringGrid1->Cells[DelX][DelY] = Form2->StringGrid1->Cells[X][Y];

              if (DelX < 16) { DelX++; }
                        else { UpdateGridToList_Line(DelY); DelX = 1; DelY+=1; }
             }
         }
     X = 1; Y++;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Zapenijwartoci1Click(TObject *Sender)
{
if (Form5->ShowModal()==mrOk)
{
for (int Y = Form2->StringGrid1->Selection.Top; Y <= Form2->StringGrid1->Selection.Bottom; Y++)
    {
     for (int X = Form2->StringGrid1->Selection.Left; X <= Form2->StringGrid1->Selection.Right && X <= 16; X++)
         {
          Form2->StringGrid1->Cells[X][Y] = String::IntToHex(StrToIntDef("0x" + Form5->MaskEdit1->Text,0xFF),2);
         }
     UpdateGridToList_Line(Y);
    }
}
}
//---------------------------------------------------------------------------

void __fastcall TForm2::StringGrid1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Clicked = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::StringGrid1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
if (Clicked==true)
   {
    StatusBar1->Panels->Items[1]->Text = "Zaznaczenie = " + String((Form2->StringGrid1->Selection.Right - Form2->StringGrid1->Selection.Left + 1)*(Form2->StringGrid1->Selection.Bottom - Form2->StringGrid1->Selection.Top + 1)) + " Bajt";
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::StringGrid1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Clicked = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm2::Zamknij1Click(TObject *Sender)
{
for (int Y = 1; Y <= Flash51->FLASH/16 ; Y++)
  {
   UpdateGridToFile_Line(Y);
  }
}
//---------------------------------------------------------------------------

