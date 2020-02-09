@echo OFF

::******************************************************************************
:: Setting
::******************************************************************************
rem path=C:\Keil\C51\BIN;
path=C:\Keil\C51\BIN;
set COREInclude=CORE\INCLUDE\*.H
set OEMInclude=OEM\INCLUDE\*.H
set CDirectives=LA WL(1) CD OT(9, size) NOAREGS OR INCDIR(.\CORE\INCLUDE\;.\OEM\INCLUDE\)
set CDirectives1=LA WL(1) CD OT(9, size) NOAREGS OR SRC INCDIR(.\CORE\INCLUDE\;.\OEM\INCLUDE\)
set CDirectives2=LA WL(1) CD OT(8, size) NOAREGS OR INCDIR(.\CORE\INCLUDE\;.\OEM\INCLUDE\)
set ADirectives=SET (LA) DEBUG EP
set CC=C51
set AS=A51
set LINKER=BL51

::******************************************************************************
:: Parameter
::******************************************************************************
if "%1" == ""       goto OPTIONS
if "%1" == "?"      goto OPTIONS
if "%1" == "/?"     goto OPTIONS
if "%1" == "CLEAR"  goto CLEAR
if "%1" == "clear"  goto CLEAR

::******************************************************************************
:: Build
::******************************************************************************
if not exist .\OEM\%1 goto NO_EXIST

del /Q .\KeilMisc\LST\*.*
del /Q .\KeilMisc\MAP\*.*
del /Q .\KeilMisc\OBJ\*.*
del /Q .\ROM\*.*

cd ROM
copy ..\Tools\FU.EXE
copy ..\Tools\Hex2bin.EXE
cd ..

cd OEM
md INCLUDE
cd INCLUDE
copy ..\%1\INCLUDE\*.H
cd ..
copy .\%1\OEM_BANK0\*.C
copy .\%1\OEM_BANK1\*.C
copy .\%1\OEM_BANK2\*.C
copy .\%1\OEM_BANK3\*.C
cd ..

cd CORE
copy .\CORE_BANK0\*.C
copy .\CORE_COMMON\*.C
copy .\CORE_COMMON\*.A51
copy .\CORE_COMMON\*.SRC
cd ..

::------------------------------------------------------------------------------
:: Compile CORE common file
::------------------------------------------------------------------------------
%CC% CORE\CORE_IRQ.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_BITADDR.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_ACPI.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_MEMORY.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_HOSTIF.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_MAIN.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_PORT6064.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_SMBUS.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_TIMERS.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_CIR.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_CHIPREGS.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_HSPI.C %CDirectives1%
%AS% CORE\CORE_HSPI.SRC %ADirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_PECI.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_ITESTRING.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_SPI.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_LPC.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%AS% CORE\L51_BANK.A51 %ADirectives%
if errorlevel 1 goto MAKE_ERR
%AS% CORE\STARTUP.A51 %ADirectives%
if errorlevel 1 goto MAKE_ERR

::------------------------------------------------------------------------------
:: Compile CORE bank0 file
::------------------------------------------------------------------------------
%CC% CORE\CORE_FLASH.C %CDirectives2%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_XLT.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_SCAN.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% CORE\CORE_PS2.C %CDirectives%
if errorlevel 1 goto MAKE_ERR

::------------------------------------------------------------------------------
:: Compile OEM bank0 file
::------------------------------------------------------------------------------
%CC% OEM\OEM_GPIO.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_LED.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_MAIN.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_SCANTABS.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_FAN.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_BATTERY.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_ACPI.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_CIR.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_FLASH.C %CDirectives2%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_INIT.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_HOSTIF.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_6064.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_MEMORY.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_VER.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_NECP.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_PORT686C.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_PECI.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_PM3.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_SPI.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_LPC.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_THERM.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_IPOD.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_TP.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_MOTIONLED.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_VICMV.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_DCMI.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_VICMVACDC.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_DIMM.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_S4S5WDG.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_CALPELLA.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_EMI_SETTING.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_IT7230_SETTING.C %CDirectives%
if errorlevel 1 goto MAKE_ERR
%CC% OEM\OEM_IT7230.C %CDirectives%
if errorlevel 1 goto MAKE_ERR

::------------------------------------------------------------------------------
:: Compile OEM bank1 file
::------------------------------------------------------------------------------
%CC% OEM\OEM_BANK1_FUNC.C %CDirectives%
if errorlevel 1 goto MAKE_ERR

::------------------------------------------------------------------------------
:: Compile OEM bank2 file
::------------------------------------------------------------------------------
%CC% OEM\OEM_BANK2_FUNC.C %CDirectives%
if errorlevel 1 goto MAKE_ERR

::------------------------------------------------------------------------------
:: Compile OEM bank3 file
::------------------------------------------------------------------------------
%CC% OEM\OEM_BANK3_FUNC.C %CDirectives%
if errorlevel 1 goto MAKE_ERR

::------------------------------------------------------------------------------
:: Link object files
::------------------------------------------------------------------------------
move .\OEM\*.LST .\KeilMisc\LST\
move .\OEM\*.OBJ .\KeilMisc\OBJ\
move .\CORE\*.LST .\KeilMisc\LST\
move .\CORE\*.OBJ .\KeilMisc\OBJ\

%LINKER% @ITEEC.lin
if errorlevel 1 goto MAKE_END
move ITEEC.M51 .\KeilMisc\MAP\
move ITEEC.ABS .\KeilMisc\OBJ\

cd KeilMisc
cd OBJ
OC51 ITEEC.ABS

OH51 ITEEC.B00 HEXFILE(ITEEC.H00)

if not exist *.B01 goto OH51_OK
OH51 ITEEC.B01 HEXFILE(ITEEC.H01)

if not exist *.B02 goto OH51_OK
OH51 ITEEC.B02 HEXFILE(ITEEC.H02)

if not exist *.B01 goto OH51_OK
OH51 ITEEC.B03 HEXFILE(ITEEC.H03)

:OH51_OK
move *.H* ..\..\ROM\
del /Q *.B0*
cd ..
cd ..

cd ROM
FU /DISP

if not exist *.H01 goto CB0
if not exist *.H02 goto CB0B1
if not exist *.H03 goto CB0B1B2

::------------------------------------------------------------------------------
:: Common + Bank0 + Bank1 + Bank2 + Bank3 [160KB]
::------------------------------------------------------------------------------
Hex2bin -s 0x0000 -e b00 *.H00
FU /SIZE 64 ITEEC.b00 BANK0.BIN FF

Hex2bin -s 0x8000 -e b01 *.H01
FU /SIZE 32 ITEEC.b01 BANK1.BIN FF

Hex2bin -s 0x8000 -e b02 *.H02
FU /SIZE 32 ITEEC.b02 BANK2.BIN FF

Hex2bin -s 0x8000 -e b03 *.H03
FU /SIZE 32 ITEEC.b03 BANK3.BIN FF

copy /B BANK0.BIN+BANK1.BIN+BANK2.BIN+BANK3.BIN ITEEC.BIN
goto MAKE_OK

::------------------------------------------------------------------------------
:: Common + Bank0 [64KB]
::------------------------------------------------------------------------------
:CB0
Hex2bin -s 0x0000 -e b00 *.H00
FU /SIZE 64 ITEEC.b00 BANK0.BIN FF
copy /B BANK0.BIN ITEEC.BIN
goto MAKE_OK

::------------------------------------------------------------------------------
:: Common + Bank0 + Bank1 [96KB]
::------------------------------------------------------------------------------
:CB0B1
Hex2bin -s 0x0000 -e b00 *.H00
FU /SIZE 64 ITEEC.b00 BANK0.BIN FF
Hex2bin -s 0x8000 -e b01 *.H01
FU /SIZE 32 ITEEC.b01 BANK1.BIN FF
copy /B BANK0.BIN+BANK1.BIN ITEEC.BIN
goto MAKE_OK

::------------------------------------------------------------------------------
:: Common + Bank0 + Bank1 + Bank2 [128KB]
::------------------------------------------------------------------------------
:CB0B1B2
Hex2bin -s 0x0000 -e b00 *.H00
FU /SIZE 64 ITEEC.b00 BANK0.BIN FF
Hex2bin -s 0x8000 -e b01 *.H01
FU /SIZE 32 ITEEC.b01 BANK1.BIN FF
Hex2bin -s 0x8000 -e b02 *.H02
FU /SIZE 32 ITEEC.b02 BANK2.BIN FF
copy /B BANK0.BIN+BANK1.BIN+BANK2.BIN ITEEC.BIN
goto MAKE_OK

::------------------------------------------------------------------------------
:: Build OK
::------------------------------------------------------------------------------
:MAKE_OK
copy ITEEC.BIN %1_%2.BIN
FU /SIZE 128 ITEEC.BIN %1_EC.BIN FF
del /Q *.H*
del /Q *.B0*
del /Q BANK?.*
cd ..

cls
echo    ********************************************************************
echo    *    ITE Embedded Controller Firmware Build Process                *
echo    *    Copyright (c) 2006-2010, ITE Tech. Inc. All Rights Reserved.  *
echo    ********************************************************************
echo.
echo    Making EC bin file successfully !!!
echo.
if exist ROM\%1_EC.BIN Tools\CK32 ROM\%1_EC.BIN
goto MAKE_END

:NO_EXIST
echo.
echo    ********************************************************************
echo    *    ITE Embedded Controller Firmware Build Process                *
echo    *    Copyright (c) 2006-2010, ITE Tech. Inc. All Rights Reserved.  *
echo    ********************************************************************
echo.
echo    Project folder is not exist.
goto END

:OPTIONS
echo.
echo    ********************************************************************
echo    *    ITE Embedded Controller Firmware Build Process                *
echo    *    Copyright (c) 2006-2010, ITE Tech. Inc. All Rights Reserved.  *
echo    ********************************************************************
echo.
echo    USAGE: build [P1] [P2]
echo                P1 = Project name of OEM folder or [CLEAR] for clear
echo                P2 = Code version which will attach to bin file name
goto END

:CLEAR
del /Q .\KeilMisc\LST\*.*
del /Q .\KeilMisc\MAP\*.*
del /Q .\KeilMisc\OBJ\*.*
del /Q .\ROM\*.*
goto END

:MAKE_ERR
del /Q .\OEM\*.LST
del /Q .\OEM\*.OBJ
del /Q .\CORE\*.LST
del /Q .\CORE\*.OBJ

:MAKE_END
rd /S /Q .\OEM\INCLUDE
del /Q .\OEM\*.C
del /Q .\CORE\*.C
del /Q .\CORE\*.A51
del /Q .\CORE\*.SRC
del /Q .\ROM\*.EXE

::------------------------------------------------------------------------------
:: End
::------------------------------------------------------------------------------
:END
rem echo.
rem pause
@echo ON
