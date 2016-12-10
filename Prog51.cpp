//---------------------------------------------------------------------------
#include <vcl.h>
#include "Unit2.h"
#pragma hdrstop
USERES("Prog51.res");
USEFORM("Unit1.cpp", Form1);
USEUNIT("Unit2.cpp");
USEFORM("Unit3.cpp", Form2);
USEFORM("Unit4.cpp", Form4);
USEFORM("Unit5.cpp", Form5);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR lpszCmdLine, int)
{
        try
        {
        int com = Flash51->port.Open(" \\\\.\\COM1:");
            Application->Initialize();
            Application->Title = "Programator AT89Cx051 / sstsoft.pl ";
		Application->CreateForm(__classid(TForm1), &Form1);
            Application->CreateForm(__classid(TForm2), &Form2);
            Application->CreateForm(__classid(TForm4), &Form4);
            Application->CreateForm(__classid(TForm5), &Form5);
            Application->Run();
        if (com) {            Flash51->plik.Close(); Flash51->port.Close(); }
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
return 0;
}
//---------------------------------------------------------------------------
