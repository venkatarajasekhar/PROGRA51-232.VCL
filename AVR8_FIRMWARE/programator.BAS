'-----------------Stanis³aw Stasiak "TSoft, where?" 2001-2002--------------
'-------------------------Program PROGRAMATORA-----------------------------
'-------------Autor Stanis³aw Stasiak zastrzega sobie prawo do-------------
'-----------------------firmowania treœci programu-------------------------

'-------------AT90c2313-------------
'            -----__-----
'           |            |
'--(01) RST |            | VCC (20)--
'--(02) PD0 | RxD    SCK | PB7 (19)--
'--(03) PD1 | TxD   MISO | PB6 (18)--
'--(04) Xt1 |       MOSI | PB5 (17)--
'--(05) Xt2 |         SS | PB4 (16)--
'--(06) PD2 | INT0   OCI | PB3 (15)--
'--(07) PD3 | INT1       | PB2 (14)--
'--(08) PD4 | T0    AIN1 | PB1 (13)--
'--(09) PD5 | T1    AIN2 | PB0 (12)--
'--(10) GND |        ICP | PD6 (11)--
'           |            |
'            ------------
'-----------------------------------

Config Rc5 = Pinb.3
Config Spi = Soft , Din = Pinb.6 , Dout = Portb.5 , Ss = Portb.4 , Clock = Portb.7

$regfile = "2313DEF.dat"
$crystal = 4000000 : $baud = 4800

M_ezap Alias &H10 : M_read Alias &H03 : M_write Alias &H07 : M_lock1 Alias &H0F : M_lock2 Alias &H0C : M_lock3 Alias &H0A : M_sign Alias &H00 : M_erase Alias &H0E : M_error Alias &HF0

Config Serialin = Buffered , Size = 32
Config Portd = &B01111110
Config Portb = Input
Enable Interrupts
Enable Serial

Portd_prog Alias Portd.6
Portd_clk_adress Alias Portd.5
Portd_clk_mode Alias Portd.4
Portd_nonev Alias Portd.3
Portd_5v Alias Portd.2

'*************************** zmienne komunikacji **************************

Dim Eezapisane As Eram Long , Zapisane As Long

Dim Rxd_start As Byte
Dim Rxd_mode As Byte
Dim Rxd_adress As Word
Dim Rxd_stack(32) As Byte

'*************************** zmienne "sprzêtowe" **************************

Dim Cfg_mode As Byte
Dim Cfg_adress As Word
Dim Cfg_manufacturer As Byte
Dim Cfg_processor As Byte
Dim Cfg_voltage As Byte

'*************************** definicje funkcji ****************************

Dim X As Word
Declare Sub Set_adress_counter
Declare Sub Set_mode_counter
Declare Sub Zeruj
Declare Sub Programuj
Declare Sub Pulse

'***************************** gowna petla *******************************

Zapisane = Eezapisane

Do_zeruj:
Call Zeruj

Do

Do_pulse:
Call Pulse

Do_kontakt:
Inputbin Rxd_start
If Rxd_start = &H55 Then

   Inputbin Rxd_mode
   If Rxd_mode = M_ezap Then                                'M_EEZAP
      Goto Respond
   End If
   If Rxd_mode = M_sign Then                                'M_SIGN
      Rxd_adress = &H30

      Call Set_mode_counter
      For X = 1 To 3
          Call Set_adress_counter
          Rxd_stack(x) = Pinb
          Incr Rxd_adress
      Next X

'      Cfg_manufacturer = Rxd_stack(1)
'         Cfg_processor = Rxd_stack(2)
      If Rxd_stack(3) = &H05 Then Cfg_voltage = 5 Else Cfg_voltage = 12
      Goto Respond
   End If
   If Rxd_mode = M_read Then                                'M_READ
      Inputbin Rxd_adress

      Call Set_mode_counter
      For X = 1 To 32
          Call Set_adress_counter
          Incr Rxd_adress
          Rxd_stack(x) = Pinb
      Next X
      Goto Respond
   End If
   If Rxd_mode = M_write Then                               'M_WRITE
      Inputbin Rxd_adress
      Inputbin Rxd_stack(1) , 32

      Call Set_mode_counter
      For X = 1 To 32
          Call Set_adress_counter
          Incr Rxd_adress
          Portb = Rxd_stack(x)
          Call Programuj
      Next X
      Goto Respond
   End If
   If Rxd_mode = M_lock1 Or Rxd_mode = M_lock2 Or Rxd_mode = M_lock3 Or Rxd_mode = M_erase Then

      Call Set_mode_counter
      Call Programuj
      Goto Respond
   Else

      Goto Do_kontakt

   End If

End If
'// reset
If Rxd_start = &H00 Then                                    '&h00
   Printbin Rxd_start : Goto Do_zeruj
End If

'// bezposredni dostep do portu b
If Rxd_start = &H4F Then                                    'O
   Inputbin Portb
   Goto Do_pulse

End If
If Rxd_start = &H45 Then                                    'E
   Config Portb = Output
   Goto Do_pulse

End If
If Rxd_start = &H49 Then                                    'I
   Printbin Pinb
   Goto Do_pulse

End If
If Rxd_start = &H44 Then                                    'D
   Config Portb = Input
   Goto Do_pulse

End If

'// spi
If Rxd_start = &H53 Then                                    'S

   Inputbin Rxd_mode
   If Rxd_mode = &H53 Then                                  'S
      Spiinit
      Goto Do_pulse
   End If
   If Rxd_mode = &H4F Then                                  'O
      Inputbin Rxd_start
      Spiout Rxd_start , 1
      Goto Do_pulse
   End If
   If Rxd_mode = &H49 Then                                  'I
       Spiin Rxd_start , 1
       Printbin Rxd_start
      Goto Do_pulse
   End If

End If
'// rc5
If Rxd_start = &H52 Then                                    'R

   Inputbin Rxd_mode
   If Rxd_mode = &H35 Then                                  '5
      Inputbin Rxd_stack(1) , 3 : Rc5send Rxd_stack(1) , Rxd_stack(2) , Rxd_stack(3)
      Goto Do_pulse
   End If
   If Rxd_mode = &H36 Then                                  '6
      Inputbin Rxd_stack(1) , 3 : Rc6send Rxd_stack(1) , Rxd_stack(2) , Rxd_stack(3)
      Goto Do_pulse
   End If

Else
   Printbin Rxd_start : Goto Do_kontakt
End If

'// ODPOWIEDZ
Respond:

   Waitms 10

If Rxd_mode = M_read Then
   Printbin &H55 ; Rxd_mode ; Rxd_stack(1) ; 32
   Goto Do_kontakt
End If
If Rxd_mode = M_sign Then
   Printbin &H55 ; Rxd_mode ; Rxd_stack(1) ; 3
   Goto Do_kontakt
End If
If Rxd_mode = M_ezap Then
   Printbin &H55 ; Rxd_mode ; Zapisane
   Goto Do_kontakt
End If
If Rxd_mode = M_write Or Rxd_mode = M_erase Then
   Printbin &H55 ; Rxd_mode
   Eezapisane = Zapisane
   Goto Do_kontakt
Else
   Printbin &H55 ; Rxd_mode
   Goto Do_pulse
End If

Loop

'//****************************** Adress **********************************

Sub Set_adress_counter

If Rxd_adress < Cfg_adress Or Rxd_adress = 0 Then
   Set Portd_nonev
   Set Portd_clk_adress
   Waitms 5
   Reset Portd_nonev
   Reset Portd_clk_adress
   Waitms 5
   Cfg_adress = 0
End If
While Cfg_adress < Rxd_adress
       Incr Cfg_adress : Reset Portd_clk_adress : Set Portd_clk_adress : Reset Portd_clk_adress
Wend
End Sub

'//******************************* Mode ***********************************

Sub Set_mode_counter

If Rxd_mode < Cfg_mode Or Rxd_mode = 0 Then
   Set Portd_clk_mode
   Waitms 5
   Reset Portd_clk_mode
   Waitms 5
   Cfg_mode = 0
End If
While Cfg_mode < Rxd_mode
       Incr Cfg_mode : Reset Portd_clk_mode : Set Portd_clk_mode : Reset Portd_clk_mode
Wend
End Sub

'//****************************** Impuls **********************************

Sub Programuj

    Set Portd_prog
     If Cfg_voltage = 5 Then Set Portd_5v Else Reset Portd_5v
    Config Portb = Output

    Waitus 10
      Reset Portd_prog
      If Rxd_mode = M_erase Then Waitms 10 Else Waitus 100
        Set Portd_prog
    Waitus 10

    Set Portd_5v
     If Rxd_mode = M_write Then Waitms 2
    Config Portb = Input

     If Rxd_mode = M_write Or Rxd_mode = M_erase Then Zapisane = Zapisane + 1

End Sub

'//****************************** Do_zeruj **********************************

Sub Zeruj

    Cfg_voltage = 5
    Cfg_processor = 0
    Cfg_manufacturer = 0

    Set Portd_nonev
    Set Portd_5v
    Set Portd_clk_adress
    Set Portd_clk_mode

    Call Pulse

    Reset Portd_nonev
    Reset Portd_clk_adress
    Cfg_adress = &H0000
    Reset Portd_clk_mode
    Cfg_mode = &H00

    Config Portd = &B01111110
    Config Portb = Input

End Sub

'//****************************** Pulse **********************************

Sub Pulse

    Set Portd_prog
    Waitms 10
    Reset Portd_prog
    Waitms 15
    Set Portd_prog

End Sub





Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
Data "sfhgsggggggggggggg"
