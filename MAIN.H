#ifndef FlashH
#define FlashH

#include <windows.h>
#define ERROR_MESSAGE_EXCEEDS_MAX_SIZE   4336L
#define SCARD_W_CANCELLED_BY_USER        _HRESULT_TYPEDEF_(0x8010006EL)
#pragma hdrstop

class plik_Class
  {
//---------------------------------------------------------------------------
   public:
//---------------------------------------------------------------------------
        __stdcall plik_Class    (void);
//---------------------------------------------------------------------------
char Path[256];
HANDLE Handle; LPDWORD lpdword;
bool    __stdcall Open          (char* PathToSet);
bool    __stdcall Close         (void);
//---------------------------------------------------------------------------
__int8  *Bufor; __int32  Pos, Size;
bool    __stdcall Read          (int PosToGet, void *BuforToGet, int SizeToGet);
bool    __stdcall Load          (void);
bool    __stdcall Post          (int PosToSet, void *BuforToSet, int SizeToSet);
bool    __stdcall Save          (void);
//---------------------------------------------------------------------------
        __stdcall ~plik_Class   (void);
//---------------------------------------------------------------------------
  };
//---------------------------------------------------------------------------

__stdcall plik_Class::plik_Class(void)
{
Bufor = NULL; Pos = Size = 0; Handle = NULL; lpdword = new DWORD;
}
//---------------------------------------------------------------------------

__stdcall plik_Class::~plik_Class(void)
{
delete lpdword;
}
//---------------------------------------------------------------------------

bool __stdcall plik_Class::Open(char* PathToSet)
{
Close();

Handle = CreateFile(strcpy(this->Path, PathToSet),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

if (Handle==INVALID_HANDLE_VALUE)
   {return false;
   }
if (strstr(this->Path," \\\\.\\COM")!=NULL)
   {
    DCB CommDCB;
    COMMTIMEOUTS TimeOuts;

    TimeOuts.ReadIntervalTimeout         = 1000;
    TimeOuts.ReadTotalTimeoutMultiplier  = 1;
    TimeOuts.ReadTotalTimeoutConstant    = 1000;
    TimeOuts.WriteTotalTimeoutMultiplier = 1;
    TimeOuts.WriteTotalTimeoutConstant   = 10;

    SetCommTimeouts(Handle,&TimeOuts);
    SetupComm(Handle,40,40);

    GetCommState(Handle,&CommDCB);
    PurgeComm(Handle,PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_RXABORT|PURGE_TXABORT);

    CommDCB.BaudRate = CBR_4800;
    CommDCB.ByteSize = 8;
    CommDCB.Parity = NOPARITY;
    CommDCB.StopBits = ONESTOPBIT;

    CommDCB.fBinary = 1;
    CommDCB.fParity = 0;

    CommDCB.fInX = 0;
    CommDCB.fTXContinueOnXoff = 1;
    CommDCB.XonChar = 0x11;
    CommDCB.XonLim = 512;
    CommDCB.XoffChar = 0x13;
    CommDCB.XoffLim = 512;
    CommDCB.fOutX = 0;

    CommDCB.EofChar = 0x1A;
    CommDCB.ErrorChar = 0xFF;
    CommDCB.EvtChar = 0;
    CommDCB.fErrorChar = 0;
    CommDCB.fAbortOnError = true;
    CommDCB.fNull = 0;

    CommDCB.fDtrControl = DTR_CONTROL_HANDSHAKE;
    CommDCB.fOutxDsrFlow = true;
    CommDCB.fRtsControl = RTS_CONTROL_HANDSHAKE;
    CommDCB.fOutxCtsFlow = true;

    SetCommState(Handle, &CommDCB);
    }
Bufor = new __int8[65536];
Pos = 0; Size = GetFileSize(Handle, 0);
return Load();
}
//---------------------------------------------------------------------------

bool __stdcall plik_Class::Read(int PosToGet, void *BuforToGet, int SizeToGet)
{
SetFilePointer(Handle, PosToGet, NULL, FILE_BEGIN);
if (ReadFile(Handle, BuforToGet, SizeToGet, lpdword, NULL))
   {this->Pos = PosToGet + SizeToGet;
    return true;
   }
return false;
}
//---------------------------------------------------------------------------

bool __stdcall plik_Class::Post(int PosToSet, void *BuforToSet, int SizeToSet)
{
SetFilePointer(Handle, PosToSet, NULL, FILE_BEGIN); SetEndOfFile(Handle);
if (WriteFile(Handle, BuforToSet, SizeToSet, lpdword, NULL))
   {this->Size = this->Pos = PosToSet + SizeToSet;
    return true;
   }
return false;
}
//---------------------------------------------------------------------------

bool __stdcall plik_Class::Load(void)
{
return Read(0, Bufor, Size);
}
//---------------------------------------------------------------------------

bool __stdcall plik_Class::Save(void)
{
return Post(0, Bufor, Size);
}
//---------------------------------------------------------------------------

bool __stdcall plik_Class::Close(void)
{
if (Handle!=INVALID_HANDLE_VALUE)
   {
    CloseHandle(Handle);
    Handle = INVALID_HANDLE_VALUE;
    Path[0] = '\0';
    delete Bufor;
    Pos = 0, Size = 0;
    return true;
   }
else
    return false;
}
//---------------------------------------------------------------------------

 class TFlashAT89Cx051
{
public:
//---------------------------------------------------------------------------
bool Stop; short LOCKBIT, FLASH;
//---------------------------------------------------------------------------
plik_Class plik, port;
//---------------------------------------------------------------------------
        __stdcall TFlashAT89Cx051            (void);
//---------------------------------------------------------------------------
void    __stdcall Set_Bit                    (__int8 *Info, __int8 Bit);
void    __stdcall Clr_Bit                    (__int8 *Info, __int8 Bit);
void    __stdcall XOr_Bit                    (__int8 *Info, __int8 Bit);
//---------------------------------------------------------------------------
unsigned __int8 *__stdcall Sign              (void);
void             __stdcall Lock              (void);
unsigned __int8 *__stdcall Read              (void);
void             __stdcall Prog              (void);
void             __stdcall Erase             (void);
void             __stdcall Reset             (void);
        __int32 *__stdcall Eezap             (void);
//---------------------------------------------------------------------------
__int32 __stdcall ( __closure *OnBreak )     ( __int32 brParam );
__int32 __stdcall ( __closure *OnError )     ( __int32 erParam );
//---------------------------------------------------------------------------
__int32 __stdcall DoBreak	             ( __int32 brParam );
__int32 __stdcall DoError             	     ( __int32 erParam );
//---------------------------------------------------------------------------
        __stdcall ~TFlashAT89Cx051           (void);
};
//---------------------------------------------------------------------------

#define AT89C1024 (int)1024
#define AT89C2048 (int)2048
#define AT89C4096 (int)4096
//---------------------------------------------------------------------------

__stdcall TFlashAT89Cx051::TFlashAT89Cx051(void)
{
Stop = false;

FLASH = 2048; LOCKBIT = 0;

OnBreak = DoBreak; OnError = DoError;
}

//---------------------------------------------------------------------------

#define M_READ  0x03
#define M_WRITE 0x07
#define M_LOCK1 0x0F
#define M_LOCK2 0x0C
#define M_LOCK3 0x0A
#define M_SIGN  0x00
#define M_ERASE 0x0E
#define M_EEZAP 0x10

//---------------------------------------------------------------------------

unsigned __int8 *__stdcall TFlashAT89Cx051::Sign(void)
{
static unsigned __int8 sentdata[2], received[5];

sentdata[0] = 0x55;                             // start
sentdata[1] = M_SIGN;                           // mode
port.Post(0, &sentdata, 2);

port.Read(0, &received, 5);
return &received[2];
}
//---------------------------------------------------------------------------

__int32 *__stdcall TFlashAT89Cx051::Eezap(void)
{
static unsigned __int8 sentdata[2], received[6];

sentdata[0] = 0x55;                             // start
sentdata[1] = M_EEZAP;                          // mode
port.Post(0, &sentdata, 2);

port.Read(0, &received, 6);
return (__int32*)&received[2];
}
//---------------------------------------------------------------------------

unsigned __int8 *__stdcall TFlashAT89Cx051::Read(void)
{
unsigned __int8 sentdata[40], received[40];

for (plik.Pos = 0; plik.Pos < 2048 && Stop==false; OnBreak(0))
    {
       sentdata[0] = 0x55;                              // start
       sentdata[1] = M_READ;                            // mode
       sentdata[2] = (unsigned __int8)plik.Pos;         // adress
       sentdata[3] = (unsigned __int8)plik.Pos >> 8 ;   // adress

       port.Post(0, &sentdata, 4);
       port.Read(0, &received, 34);

       for (int x = 2; x < 2+32; x++)
       plik.Bufor[plik.Pos++] = received[x];

       if (received[0]!=0x55 || received[1]!=M_READ)
           Stop = true;
    }
for (int adress = plik.Pos; adress < 8192; adress++) plik.Bufor[adress] = 0xFF;

if (Stop==true)
   {::SetLastError(SCARD_W_CANCELLED_BY_USER); OnError(0);
    Stop = false;
   }
return &plik.Bufor[0];
}
//---------------------------------------------------------------------------

void __stdcall TFlashAT89Cx051::Prog()
{
unsigned __int8 sentdata[40], received[40];

Sign(); Erase();

for (plik.Pos = 0; plik.Pos < plik.Size && Stop==false; OnBreak(0))
    {
       sentdata[0] = 0x55;                              // start
       sentdata[1] = M_WRITE;                           // mode
       sentdata[2] = (unsigned __int8)plik.Pos;         // adress
       sentdata[3] = (unsigned __int8)plik.Pos >> 8 ;   // adress
       port.Post(0, &sentdata, 4);

       for (int x = 0; x < 32; x++)
       sentdata[x] = plik.Bufor[plik.Pos++];
       port.Post(0, &sentdata, 32);

       port.Read(0, &received, 2);
       if (received[0]!=0x55 || received[1]!=M_WRITE)
           Stop = true;
    }
Lock();

if (Stop ==true)
   {::SetLastError(SCARD_W_CANCELLED_BY_USER); OnError(0);
    Stop = false;
   }
}
//---------------------------------------------------------------------------

void __stdcall TFlashAT89Cx051::Erase(void)
{
unsigned __int8 sentdata[2], received[2];

sentdata[0] = 0x55;                             // start
sentdata[1] = M_ERASE;                          // mode
port.Post(0, &sentdata, 2);

port.Read(0, &received, 2);
if (received[0]!=0x55 || received[1]!=M_ERASE)
    Stop = true;

if (Stop ==true)
   {::SetLastError(SCARD_W_CANCELLED_BY_USER); OnError(0);
    Stop = false;
   }
}
//---------------------------------------------------------------------------

void __stdcall TFlashAT89Cx051::Lock(void)
{
unsigned __int8 sentdata[5], received[5];

sentdata[0] = 0x55;                     // start
if (LOCKBIT==0)
    return;
if (LOCKBIT==1)
    sentdata[1] = M_LOCK1;  // mode
if (LOCKBIT==2)
    sentdata[1] = M_LOCK2;  // mode
if (LOCKBIT==3)
    sentdata[1] = M_LOCK3;  // mode
port.Post(0, &sentdata, 2);

port.Read(0, &received, 2);
if (received[0]!=0x55)
    Stop = true;
if (LOCKBIT==1 && received[1]!=M_LOCK1)
    Stop = true;
if (LOCKBIT==2 && received[1]!=M_LOCK2)
    Stop = true;
if (LOCKBIT==3 && received[1]!=M_LOCK3)
    Stop = true;

if (Stop ==true)
   {::SetLastError(SCARD_W_CANCELLED_BY_USER); OnError(0);
    Stop = false;
   }
}
//---------------------------------------------------------------------------

void __stdcall TFlashAT89Cx051::Reset()
{
unsigned __int8 sentdata[5], received[5];

for (int x = 0; x < 1; x++)
    { sentdata[x] = 0x00;
      port.Post(0, &sentdata, 1);
      port.Read(0, &received, 1);
    }
PurgeComm(port.Handle,PURGE_RXCLEAR);
}
//---------------------------------------------------------------------------

__int32 __stdcall TFlashAT89Cx051::DoBreak(__int32 brParam)
{
Application->ProcessMessages();
return brParam;
}
//---------------------------------------------------------------------------

__int32 __stdcall TFlashAT89Cx051::DoError(__int32 erParam)
{
char lpMsgBuf[1024];
FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
              NULL,GetLastError(),
              MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
              &lpMsgBuf[0],sizeof(lpMsgBuf),NULL);
return MessageBox(NULL,lpMsgBuf,"TSoft \"Programator AT89Cx051\"",
                  MB_OK|MB_ICONERROR|MB_APPLMODAL);
}
//---------------------------------------------------------------------------

void __stdcall TFlashAT89Cx051::Set_Bit(__int8 *Info, __int8 Bit)
{
switch (Bit) {
case 0: *Info = *Info | 0x01; break;
case 1: *Info = *Info | 0x02; break;
case 2: *Info = *Info | 0x04; break;
case 3: *Info = *Info | 0x08; break;
case 4: *Info = *Info | 0x10; break;
case 5: *Info = *Info | 0x20; break;
case 6: *Info = *Info | 0x40; break;
case 7: *Info = *Info | 0x80; break;
}
}
//---------------------------------------------------------------------------

void __stdcall TFlashAT89Cx051::Clr_Bit(__int8 *Info, __int8 Bit)
{
switch (Bit)
{
case 0: *Info = *Info & 0xFE; break;
case 1: *Info = *Info & 0xFD; break;
case 2: *Info = *Info & 0xFB; break;
case 3: *Info = *Info & 0xF7; break;
case 4: *Info = *Info & 0xEF; break;
case 5: *Info = *Info & 0xDF; break;
case 6: *Info = *Info & 0xBF; break;
case 7: *Info = *Info & 0x7F; break;
}
}
//---------------------------------------------------------------------------

void __stdcall TFlashAT89Cx051::XOr_Bit(__int8 *Info, __int8 Bit)
{
switch (Bit)
{
case 0: *Info = *Info ^ 0x01; break;
case 1: *Info = *Info ^ 0x02; break;
case 2: *Info = *Info ^ 0x04; break;
case 3: *Info = *Info ^ 0x08; break;
case 4: *Info = *Info ^ 0x10; break;
case 5: *Info = *Info ^ 0x20; break;
case 6: *Info = *Info ^ 0x40; break;
case 7: *Info = *Info ^ 0x80; break;
}
}
//---------------------------------------------------------------------------

__stdcall TFlashAT89Cx051::~TFlashAT89Cx051(void)
{
}
//---------------------------------------------------------------------------

#endif FlashH
