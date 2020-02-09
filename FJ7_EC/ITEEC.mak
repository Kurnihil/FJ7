COREInclude=CORE\INCLUDE\*.H
OEMInclude=OEM\INCLUDE\*.H
CDirectives=LA WL(1) CD OT(9, size) NOAREGS OR INCDIR(.\CORE\INCLUDE\;.\OEM\INCLUDE\)
CDirectives1=LA WL(1) CD OT(9, size) NOAREGS OR SRC INCDIR(.\CORE\INCLUDE\;.\OEM\INCLUDE\)
CDirectives2=LA WL(1) CD OT(8, size) NOAREGS OR INCDIR(.\CORE\INCLUDE\;.\OEM\INCLUDE\)
ADirectives=SET (LA) DEBUG EP
CC=C51
AS=A51
LINKER=BL51

ALL:ITEEC.b0
#-------------------------------------------------------------------------------
# Hex file to bin file
# Syntax :
#	Hex2bin -s 0x0000 -e b0 ITEEC.H00	# common + bank0 (0x0000 ~ 0xFFFF)
#	Hex2bin -s 0x8000 -e b1 ITEEC.H01	# bank1 (0x10000 ~ 0x17FFF)
#	Hex2bin -s 0x8000 -e b2 ITEEC.H02	# bank2 (0x18000 ~ 0x1FFFF)
#	Hex2bin -s 0x8000 -e b3 ITEEC.H03	# bank3 (0x20000 ~ 0x27FFF)

#	OH51 ITEEC.B00 HEXFILE(ITEEC.H00)	# common + bank0 (0x0000 ~ 0xFFFF)
#	OH51 ITEEC.B01 HEXFILE(ITEEC.H01)	# bank1 (0x10000 ~ 0x17FFF)
#	OH51 ITEEC.B02 HEXFILE(ITEEC.H02)	# bank2 (0x18000 ~ 0x1FFFF)
#	OH51 ITEEC.B03 HEXFILE(ITEEC.H03)	# bank3 (0x20000 ~ 0x27FFF)
#=------------------------------------------------------------------------------
ITEEC.b0:ITEEC.H00
	Hex2bin -s 0x0000 -e b0 ITEEC.H00

ITEEC.H00:ITEEC.B00
	OH51 ITEEC.B00 HEXFILE(ITEEC.H00)

ITEEC.B00:ITEEC.ABS
	OC51 ITEEC.ABS

#-------------------------------------------------------------------------------
# Link all obj fils
#=------------------------------------------------------------------------------
ITEEC.ABS:KeilMisc\OBJ\CORE_IRQ.OBJ\
	KeilMisc\OBJ\CORE_BITADDR.OBJ\
	KeilMisc\OBJ\CORE_ACPI.OBJ\
	KeilMisc\OBJ\CORE_MEMORY.OBJ\
	KeilMisc\OBJ\CORE_HOSTIF.OBJ\
	KeilMisc\OBJ\CORE_MAIN.OBJ\
	KeilMisc\OBJ\CORE_PORT6064.OBJ\
	KeilMisc\OBJ\CORE_PS2.OBJ\
	KeilMisc\OBJ\CORE_SCAN.OBJ\
	KeilMisc\OBJ\CORE_SMBUS.OBJ\
	KeilMisc\OBJ\CORE_TIMERS.OBJ\
	KeilMisc\OBJ\CORE_XLT.OBJ\
	KeilMisc\OBJ\CORE_CIR.OBJ\
	KeilMisc\OBJ\CORE_HSPI.OBJ\
	KeilMisc\OBJ\CORE_FLASH.OBJ\
	KeilMisc\OBJ\CORE_PECI.OBJ\
	KeilMisc\OBJ\CORE_CHIPREGS.OBJ\
	KeilMisc\OBJ\CORE_ITESTRING.OBJ\
	KeilMisc\OBJ\L51_BANK.OBJ\
	KeilMisc\OBJ\STARTUP.OBJ\
	KeilMisc\OBJ\OEM_GPIO.OBJ\
	KeilMisc\OBJ\OEM_LED.OBJ\
	KeilMisc\OBJ\OEM_MAIN.OBJ\
	KeilMisc\OBJ\OEM_SCANTABS.OBJ\
	KeilMisc\OBJ\OEM_FAN.OBJ\
	KeilMisc\OBJ\OEM_BATTERY.OBJ\
	KeilMisc\OBJ\OEM_ACPI.OBJ\
	KeilMisc\OBJ\OEM_CIR.OBJ\
	KeilMisc\OBJ\OEM_FLASH.OBJ\
	KeilMisc\OBJ\OEM_PECI.OBJ\
	KeilMisc\OBJ\OEM_INIT.OBJ\
	KeilMisc\OBJ\OEM_HOSTIF.OBJ\
	KeilMisc\OBJ\OEM_6064.OBJ\
	KeilMisc\OBJ\OEM_MEMORY.OBJ\
	KeilMisc\OBJ\OEM_NECP.OBJ\
	KeilMisc\OBJ\OEM_THERM.OBJ\
	KeilMisc\OBJ\OEM_IPOD.OBJ\
	KeilMisc\OBJ\OEM_TP.OBJ\
	KeilMisc\OBJ\OEM_MOTIONLED.OBJ\
	KeilMisc\OBJ\OEM_VICMV.OBJ\
	KeilMisc\OBJ\OEM_DCMI.OBJ\
	KeilMisc\OBJ\OEM_VICMVACDC.OBJ\
	KeilMisc\OBJ\OEM_DIMM.OBJ\
	KeilMisc\OBJ\OEM_S4S5WDG.OBJ\
	KeilMisc\OBJ\OEM_CALPELLA.OBJ\
	KeilMisc\OBJ\OEM_VER.OBJ\
	KeilMisc\OBJ\OEM_PORT686C.OBJ\
	KeilMisc\OBJ\OEM_PM3.OBJ\
	KeilMisc\OBJ\Oem_it7230_setting.OBJ\
	KeilMisc\OBJ\OEM_IT7230.OBJ\
	KeilMisc\OBJ\OEM_BANK1_FUNC.OBJ\
	KeilMisc\OBJ\OEM_BANK2_FUNC.OBJ\
	KeilMisc\OBJ\OEM_BANK3_FUNC.OBJ
	$(LINKER) @ITEEC.lin

#-------------------------------------------------------------------------------
# Compile chip file
#=------------------------------------------------------------------------------
KeilMisc\OBJ\CORE_CHIPREGS.OBJ:CORE\CORE_CHIPREGS.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_CHIPREGS.C $(CDirectives)
	move CORE\CORE_CHIPREGS.OBJ KeilMisc\OBJ
	move CORE\CORE_CHIPREGS.LST KeilMisc\LST

KeilMisc\OBJ\CORE_ITESTRING.OBJ:CORE\CORE_ITESTRING.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_ITESTRING.C $(CDirectives)
	move CORE\CORE_ITESTRING.OBJ KeilMisc\OBJ
	move CORE\CORE_ITESTRING.LST KeilMisc\LST

#-------------------------------------------------------------------------------
# Compile CORE file
#=------------------------------------------------------------------------------
KeilMisc\OBJ\CORE_IRQ.OBJ:CORE\CORE_IRQ.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_IRQ.C $(CDirectives)
	move CORE\CORE_IRQ.OBJ KeilMisc\OBJ
	move CORE\CORE_IRQ.LST KeilMisc\LST

KeilMisc\OBJ\CORE_BITADDR.OBJ:CORE\CORE_BITADDR.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_BITADDR.C $(CDirectives)
	move CORE\CORE_BITADDR.OBJ KeilMisc\OBJ
	move CORE\CORE_BITADDR.LST KeilMisc\LST

KeilMisc\OBJ\CORE_ACPI.OBJ:CORE\CORE_ACPI.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_ACPI.C $(CDirectives)
	move CORE\CORE_ACPI.OBJ KeilMisc\OBJ
	move CORE\CORE_ACPI.LST KeilMisc\LST

KeilMisc\OBJ\CORE_MEMORY.OBJ:CORE\CORE_MEMORY.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_MEMORY.C $(CDirectives)
	move CORE\CORE_MEMORY.OBJ KeilMisc\OBJ
	move CORE\CORE_MEMORY.LST KeilMisc\LST

KeilMisc\OBJ\CORE_HOSTIF.OBJ:CORE\CORE_HOSTIF.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_HOSTIF.C $(CDirectives)
	move CORE\CORE_HOSTIF.OBJ KeilMisc\OBJ
	move CORE\CORE_HOSTIF.LST KeilMisc\LST

KeilMisc\OBJ\CORE_MAIN.OBJ:CORE\CORE_MAIN.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_MAIN.C $(CDirectives)
	move CORE\CORE_MAIN.OBJ KeilMisc\OBJ
	move CORE\CORE_MAIN.LST KeilMisc\LST

KeilMisc\OBJ\CORE_PORT6064.OBJ:CORE\CORE_PORT6064.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_PORT6064.C $(CDirectives)
	move CORE\CORE_PORT6064.OBJ KeilMisc\OBJ
	move CORE\CORE_PORT6064.LST KeilMisc\LST

KeilMisc\OBJ\CORE_PS2.OBJ:CORE\CORE_PS2.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_PS2.C $(CDirectives)
	move CORE\CORE_PS2.OBJ KeilMisc\OBJ
	move CORE\CORE_PS2.LST KeilMisc\LST

KeilMisc\OBJ\CORE_SCAN.OBJ:CORE\CORE_SCAN.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_SCAN.C $(CDirectives)
	move CORE\CORE_SCAN.OBJ KeilMisc\OBJ
	move CORE\CORE_SCAN.LST KeilMisc\LST

KeilMisc\OBJ\CORE_SMBUS.OBJ:CORE\CORE_SMBUS.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_SMBUS.C $(CDirectives)
	move CORE\CORE_SMBUS.OBJ KeilMisc\OBJ
	move CORE\CORE_SMBUS.LST KeilMisc\LST

KeilMisc\OBJ\CORE_CIR.OBJ:CORE\CORE_CIR.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_CIR.C $(CDirectives)
	move CORE\CORE_CIR.OBJ KeilMisc\OBJ
	move CORE\CORE_CIR.LST KeilMisc\LST

KeilMisc\OBJ\CORE_HSPI.OBJ:CORE\CORE_HSPI.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_HSPI.C $(CDirectives1)
	$(AS) CORE\CORE_HSPI.SRC $(ADirectives)
	move CORE\CORE_HSPI.OBJ KeilMisc\OBJ
	move CORE\CORE_HSPI.LST KeilMisc\LST

KeilMisc\OBJ\CORE_TIMERS.OBJ:CORE\CORE_TIMERS.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_TIMERS.C $(CDirectives)
	move CORE\CORE_TIMERS.OBJ KeilMisc\OBJ
	move CORE\CORE_TIMERS.LST KeilMisc\LST

KeilMisc\OBJ\CORE_XLT.OBJ:CORE\CORE_XLT.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_XLT.C $(CDirectives)
	move CORE\CORE_XLT.OBJ KeilMisc\OBJ
	move CORE\CORE_XLT.LST KeilMisc\LST

KeilMisc\OBJ\CORE_FLASH.OBJ:CORE\CORE_FLASH.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_FLASH.C $(CDirectives2)
	move CORE\CORE_FLASH.OBJ KeilMisc\OBJ
	move CORE\CORE_FLASH.LST KeilMisc\LST

KeilMisc\OBJ\CORE_PECI.OBJ:CORE\CORE_PECI.C $(COREInclude) $(OEMInclude)
	$(CC) CORE\CORE_PECI.C $(CDirectives)
	move CORE\CORE_PECI.OBJ KeilMisc\OBJ
	move CORE\CORE_PECI.LST KeilMisc\LST

KeilMisc\OBJ\L51_BANK.OBJ:CORE\L51_BANK.A51
	$(AS) CORE\L51_BANK.A51 $(ADirectives)
	move CORE\L51_BANK.OBJ KeilMisc\OBJ
	move CORE\L51_BANK.LST KeilMisc\LST

KeilMisc\OBJ\STARTUP.OBJ:CORE\STARTUP.A51
	$(AS) CORE\STARTUP.A51 $(ADirectives)
	move CORE\STARTUP.OBJ KeilMisc\OBJ
	move CORE\STARTUP.LST KeilMisc\LST

#-------------------------------------------------------------------------------
# Compile OEM bank0 file
#=------------------------------------------------------------------------------
KeilMisc\OBJ\OEM_GPIO.OBJ:OEM\OEM_GPIO.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_GPIO.C $(CDirectives)
	move OEM\OEM_GPIO.OBJ KeilMisc\OBJ
	move OEM\OEM_GPIO.LST KeilMisc\LST

KeilMisc\OBJ\OEM_LED.OBJ:OEM\OEM_LED.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_LED.C $(CDirectives)
	move OEM\OEM_LED.OBJ KeilMisc\OBJ
	move OEM\OEM_LED.LST KeilMisc\LST

KeilMisc\OBJ\OEM_MAIN.OBJ:OEM\OEM_MAIN.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_MAIN.C $(CDirectives)
	move OEM\OEM_MAIN.OBJ KeilMisc\OBJ
	move OEM\OEM_MAIN.LST KeilMisc\LST

KeilMisc\OBJ\OEM_SCANTABS.OBJ:OEM\OEM_SCANTABS.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_SCANTABS.C $(CDirectives)
	move OEM\OEM_SCANTABS.OBJ KeilMisc\OBJ
	move OEM\OEM_SCANTABS.LST KeilMisc\LST

KeilMisc\OBJ\OEM_FAN.OBJ:OEM\OEM_FAN.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_FAN.C $(CDirectives)
	move OEM\OEM_FAN.OBJ KeilMisc\OBJ
	move OEM\OEM_FAN.LST KeilMisc\LST

KeilMisc\OBJ\OEM_BATTERY.OBJ:OEM\OEM_BATTERY.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_BATTERY.C $(CDirectives)
	move OEM\OEM_BATTERY.OBJ KeilMisc\OBJ
	move OEM\OEM_BATTERY.LST KeilMisc\LST

KeilMisc\OBJ\OEM_ACPI.OBJ:OEM\OEM_ACPI.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_ACPI.C $(CDirectives)
	move OEM\OEM_ACPI.OBJ KeilMisc\OBJ
	move OEM\OEM_ACPI.LST KeilMisc\LST

KeilMisc\OBJ\OEM_CIR.OBJ:OEM\OEM_CIR.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_CIR.C $(CDirectives)
	move OEM\OEM_CIR.OBJ KeilMisc\OBJ
	move OEM\OEM_CIR.LST KeilMisc\LST

KeilMisc\OBJ\OEM_FLASH.OBJ:OEM\OEM_FLASH.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_FLASH.C $(CDirectives2)
	move OEM\OEM_FLASH.OBJ KeilMisc\OBJ
	move OEM\OEM_FLASH.LST KeilMisc\LST

KeilMisc\OBJ\OEM_PECI.OBJ:OEM\OEM_PECI.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_PECI.C $(CDirectives2)
	move OEM\OEM_PECI.OBJ KeilMisc\OBJ
	move OEM\OEM_PECI.LST KeilMisc\LST

KeilMisc\OBJ\OEM_INIT.OBJ:OEM\OEM_INIT.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_INIT.C $(CDirectives)
	move OEM\OEM_INIT.OBJ KeilMisc\OBJ
	move OEM\OEM_INIT.LST KeilMisc\LST

KeilMisc\OBJ\OEM_HOSTIF.OBJ:OEM\OEM_HOSTIF.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_HOSTIF.C $(CDirectives)
	move OEM\OEM_HOSTIF.OBJ KeilMisc\OBJ
	move OEM\OEM_HOSTIF.LST KeilMisc\LST

KeilMisc\OBJ\OEM_6064.OBJ:OEM\OEM_6064.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_6064.C $(CDirectives)
	move OEM\OEM_6064.OBJ KeilMisc\OBJ
	move OEM\OEM_6064.LST KeilMisc\LST

KeilMisc\OBJ\OEM_MEMORY.OBJ:OEM\OEM_MEMORY.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_MEMORY.C $(CDirectives)
	move OEM\OEM_MEMORY.OBJ KeilMisc\OBJ
	move OEM\OEM_MEMORY.LST KeilMisc\LST

KeilMisc\OBJ\OEM_VER.OBJ:OEM\OEM_VER.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_VER.C $(CDirectives)
	move OEM\OEM_VER.OBJ KeilMisc\OBJ
	move OEM\OEM_VER.LST KeilMisc\LST

KeilMisc\OBJ\OEM_PORT686C.OBJ:OEM\OEM_PORT686C.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_PORT686C.C $(CDirectives)
	move OEM\OEM_PORT686C.OBJ KeilMisc\OBJ
	move OEM\OEM_PORT686C.LST KeilMisc\LST

KeilMisc\OBJ\OEM_PM3.OBJ:OEM\OEM_PM3.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_PM3.C $(CDirectives)
	move OEM\OEM_PM3.OBJ KeilMisc\OBJ
	move OEM\OEM_PM3.LST KeilMisc\LST

KeilMisc\OBJ\OEM_S4S5WDG.OBJ:OEM\OEM_S4S5WDG.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_S4S5WDG.C $(CDirectives)
	move OEM\OEM_S4S5WDG.OBJ KeilMisc\OBJ
	move OEM\OEM_S4S5WDG.LST KeilMisc\LST

KeilMisc\OBJ\OEM_CALPELLA.OBJ:OEM\OEM_CALPELLA.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_CALPELLA.C $(CDirectives)
	move OEM\OEM_CALPELLA.OBJ KeilMisc\OBJ
	move OEM\OEM_CALPELLA.LST KeilMisc\LST

#-------------------------------------------------------------------------------
# Compile OEM bank1 file
#=------------------------------------------------------------------------------
KeilMisc\OBJ\OEM_BANK1_FUNC.OBJ:OEM\OEM_BANK1_FUNC.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_BANK1_FUNC.C $(CDirectives)
	move OEM\OEM_BANK1_FUNC.OBJ KeilMisc\OBJ
	move OEM\OEM_BANK1_FUNC.LST KeilMisc\LST

KeilMisc\OBJ\OEM_THERM.OBJ:OEM\OEM_THERM.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_THERM.C $(CDirectives)
	move OEM\OEM_THERM.OBJ KeilMisc\OBJ
	move OEM\OEM_THERM.LST KeilMisc\LST

KeilMisc\OBJ\OEM_NECP.OBJ:OEM\OEM_NECP.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_NECP.C $(CDirectives)
	move OEM\OEM_NECP.OBJ KeilMisc\OBJ
	move OEM\OEM_NECP.LST KeilMisc\LST

KeilMisc\OBJ\OEM_IPOD.OBJ:OEM\OEM_IPOD.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_IPOD.C $(CDirectives)
	move OEM\OEM_IPOD.OBJ KeilMisc\OBJ
	move OEM\OEM_IPOD.LST KeilMisc\LST

KeilMisc\OBJ\OEM_TP.OBJ:OEM\OEM_TP.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_TP.C $(CDirectives)
	move OEM\OEM_TP.OBJ KeilMisc\OBJ
	move OEM\OEM_TP.LST KeilMisc\LST

KeilMisc\OBJ\OEM_MOTIONLED.OBJ:OEM\OEM_MOTIONLED.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_MOTIONLED.C $(CDirectives)
	move OEM\OEM_MOTIONLED.OBJ KeilMisc\OBJ
	move OEM\OEM_MOTIONLED.LST KeilMisc\LST

KeilMisc\OBJ\OEM_DIMM.OBJ:OEM\OEM_DIMM.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_DIMM.C $(CDirectives)
	move OEM\OEM_DIMM.OBJ KeilMisc\OBJ
	move OEM\OEM_DIMM.LST KeilMisc\LST

KeilMisc\OBJ\OEM_VICMV.OBJ:OEM\OEM_VICMV.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_VICMV.C $(CDirectives)
	move OEM\OEM_VICMV.OBJ KeilMisc\OBJ
	move OEM\OEM_VICMV.LST KeilMisc\LST

KeilMisc\OBJ\OEM_DCMI.OBJ:OEM\OEM_DCMI.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_DCMI.C $(CDirectives)
	move OEM\OEM_DCMI.OBJ KeilMisc\OBJ
	move OEM\OEM_DCMI.LST KeilMisc\LST

KeilMisc\OBJ\OEM_VICMVACDC.OBJ:OEM\OEM_VICMVACDC.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_VICMVACDC.C $(CDirectives)
	move OEM\OEM_VICMVACDC.OBJ KeilMisc\OBJ
	move OEM\OEM_VICMVACDC.LST KeilMisc\LST

KeilMisc\OBJ\OEM_EMI_SETTING.OBJ:OEM\OEM_EMI_SETTING.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_EMI_SETTING.C $(CDirectives)
	move OEM\OEM_EMI_SETTING.OBJ KeilMisc\OBJ
	move OEM\OEM_EMI_SETTING.LST KeilMisc\LST

KeilMisc\OBJ\OEM_IT7230_SETTING.OBJ:OEM\OEM_IT7230_SETTING.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_IT7230_SETTING.C $(CDirectives)
	move OEM\OEM_IT7230_SETTING.OBJ KeilMisc\OBJ
	move OEM\OEM_IT7230_SETTING.LST KeilMisc\LST

KeilMisc\OBJ\OEM_IT7230.OBJ:OEM\OEM_IT7230.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_IT7230.C $(CDirectives)
	move OEM\OEM_IT7230.OBJ KeilMisc\OBJ
	move OEM\OEM_IT7230.LST KeilMisc\LST

#-------------------------------------------------------------------------------
# Compile OEM bank2 file
#=------------------------------------------------------------------------------
KeilMisc\OBJ\OEM_BANK2_FUNC.OBJ:OEM\OEM_BANK2_FUNC.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_BANK2_FUNC.C $(CDirectives)
	move OEM\OEM_BANK2_FUNC.OBJ KeilMisc\OBJ
	move OEM\OEM_BANK2_FUNC.LST KeilMisc\LST

#-------------------------------------------------------------------------------
# Compile OEM bank2 file
#=------------------------------------------------------------------------------
KeilMisc\OBJ\OEM_BANK3_FUNC.OBJ:OEM\OEM_BANK3_FUNC.C $(COREInclude) $(OEMInclude)
	$(CC) OEM\OEM_BANK3_FUNC.C $(CDirectives)
	move OEM\OEM_BANK3_FUNC.OBJ KeilMisc\OBJ
	move OEM\OEM_BANK3_FUNC.LST KeilMisc\LST
