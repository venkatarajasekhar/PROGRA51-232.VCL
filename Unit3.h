//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TButton *Button1;
        TMainMenu *MainMenu1;
        TMenuItem *Plik1;
        TMenuItem *Zamknij1;
        TMenuItem *Zamknij2;
        TMenuItem *hghh1;
        TMenuItem *Doczplik1;
        TStringGrid *StringGrid1;
        TPopupMenu *PopupMenu1;
        TMenuItem *Kopiuj1;
        TMenuItem *Wklej1;
        TMenuItem *Zastpkopi1;
        TMenuItem *N1;
        TMenuItem *Przenietu1;
        TMenuItem *N2;
        TMenuItem *Zapenijwartoci1;
        TMenuItem *Usuzaznaczenie1;
        TStatusBar *StatusBar1;
        TMenuItem *Zastpobszar1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall StringGrid1SetEditText(TObject *Sender, int ACol,
          int ARow, const AnsiString Value);
        void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall StringGrid1KeyPress(TObject *Sender, char &KEY);
        void __fastcall StringGrid1Exit(TObject *Sender);
        void __fastcall Zamknij2Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall StringGrid1DblClick(TObject *Sender);
        void __fastcall Kopiuj1Click(TObject *Sender);
        void __fastcall Zastpkopi1Click(TObject *Sender);
        void __fastcall Usuzaznaczenie1Click(TObject *Sender);
        void __fastcall Wklej1Click(TObject *Sender);
        void __fastcall Przenietu1Click(TObject *Sender);
        void __fastcall Zapenijwartoci1Click(TObject *Sender);
        void __fastcall StringGrid1MouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall StringGrid1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall StringGrid1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Zamknij1Click(TObject *Sender);
private:	// User declarations
unsigned char IntGrid_Calosc[18][1024], IntGrid_Selection[18][1024]; int SelectionLeft, SelectionRight, SelectionTop, SelectionBottom, EditedCellX, EditedCellY; String EditedCellString;

bool Clicked;

void __fastcall UpdateGridToFile_Line(int Y);
void __fastcall UpdateFileToGrid_Line(int Y);
void __fastcall UpdateGridToList_Line(int Y);
void __fastcall UpdateListToGrid_Line(int Y);

public:		// User declarations
        __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
