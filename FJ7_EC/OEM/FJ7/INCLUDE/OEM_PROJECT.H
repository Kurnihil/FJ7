/*------------------------------------------------------------------------------
 * Filename: OEM_PROJECT.H      For Chipset: ITE.IT85XX
 * Function: Project function selection.
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved.
 *----------------------------------------------------------------------------*/


#ifndef	OEM_PROJECT_H
#define	OEM_PROJECT_H


//******************************************************************************
// ITE EC Function Define
//******************************************************************************
//#define	ITE_EVBoard				// Define: For ITE emulation baord
//#define	ITE8500					// Define: Chip is 8500
//#define	HSPI					// Define: Support HSPI function
#define	PECI_Support				// Define: Support PECI function
//#define	PMC3_Support			// Define: For Intel PECI stress tool and other OEM command via PMC3
//#define	SysMemory2ECRam			// Define: Support system memory cycle mapping to ec ram
#define	DisableDcache				// Define: Disable 256 bytes cache			// leox_20130912 EC weekly meeting decided to disable it
//#define	UART_Debug				// Define: Enable Tx debug (GPIO B1)
#define	Support_GPJ6_GPJ7			// Define: Support GPJ6 and GPJ7			// leox_20121213
#define	SPI_DMA_MODE				// Define: Execute SPI function by DMA mode	// leox20150706 Refer to "ZRZ_BOI_20150630.7z" provided by ITE Jay Wu


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ITE EC function setting and selection
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#define	EC_Signature_Flag	0xB5	// For 8518 crystal free
#define	EC_Signature_Flag		0x94	// bit 5: 0 Enable mirror  (internal/ex signature)	// 8587
//#define	EC_Signature_Flag	0xB4	// bit 5: 1 Disable mirror (internal/ex signature)

//#define	EFlash_Size			0xAA	// For 8518 crystal free
//#define	EFlash_Size			0x7F	// 0x7F = 128K size (14th byte size)
#define	EFlash_Size				0x5F	// 0x5F =  96K size (14th byte size)	// leox20150624 Access eFlash

#define PWRSW_WDT_1				TRUE	// TRUE: Enable PWRSW WDT 1				// leox20150724 For IT8987	// FH9C
#define PWRSW_WDT_1_CNT			0x04	// 8 sec (0x00~0x08 for 4~12 sec)		// leox20150724 PWRSW_WDT_1	// leox20150728 0x06 -> 0x04 requested by Fujitsu Iwatsuki san

#define Support_IT8987			TRUE	// TRUE: For IT8987, FALSE: For IT8587	// chris_20140924


//------------------------------------------------------------------------------
// ACPI
//------------------------------------------------------------------------------
#define	ACPI_Smbus				TRUE	// TRUE: Enable ACPI SMBus function
#define	ACPIFlagCtrlQevent		TRUE	// TRUE: BIOS needs to set ACPI_OS flag of EC when system in ACPI mode
#define	ACPI_QEventPending		TRUE	// TRUE: EC will pending q-event, if 62/66 IBF


//------------------------------------------------------------------------------
// Base Address Select
//------------------------------------------------------------------------------
// 0x00: 2E/2F, 0x01: 4E/4F, 0x02: User define (SWCBAHR | SWCBALR)
#define	Init_BADR				0x02	// Base address select
#define	Init_SWCBAHR			0x00	// I/O port high byte
#define	Init_SWCBALR			0x4E	// I/O port low  byte


//------------------------------------------------------------------------------
// Debug
//------------------------------------------------------------------------------
#define	Port60Debug				TRUE	// Port 60h RAM debug
#define	Port64Debug				TRUE	// Port 64h RAM debug
#define	Port62Debug				TRUE	// Port 62h RAM debug
#define	Port66Debug				TRUE	// Port 66h RAM debug
#define	Port68Debug				TRUE	// Port 68h RAM debug
#define	Port6CDebug				TRUE	// Port 6Ch RAM debug

#define	Init_I2EC				0x43	// Enable I2EC R/W and log port 80h

#define	LogP80InECRamDxx
#define	LogShutdownCauseInECRamCxx

#define	LogShutdownCauseInBRAM	TRUE	// TRUE: Log shutdown cause in BRAM		// leox_20111209
#define	SdCauseBRAMAddr			0x22B0
#define	SdCauseBRAMSize			8		// The size MUST large or equal to 2	// leox_20130913

// UART debug interface
// Timer 2 baudrate = Fosc / 32 / (0x10000 - RCAP2)
#ifdef	UART_Debug
// 9600 bps
//#define	R2HV					0xFF
//#define	R2LV					0xE2

// 19200 bps
//#define	R2HV					0xFF
//#define	R2LV					0xF1

// 57600 bps
#define	R2HV					0xFF
#define	R2LV					0xFB
#endif


//------------------------------------------------------------------------------
// Flash Memory Size Select
//------------------------------------------------------------------------------
// 0x01: 128KB, 0x03: 256KB, 0x07: 512KB, 0x0F: 1MB  , 0x1F: 2MB  , 0x3F: 4MB
// Or
// 0x00: 128KB, 0x02: 256KB, 0x04: 512KB, 0x06: 1MB  , 0x08: 2MB  , 0x0A: 4MB
// 0x0C: 8MB  , 0x0E: 16MB , 0x10: 32MB , 0x12: 64MB , 0x14: 128MB, 0x16: 256MB
#ifdef	HSPI
#define	Init_FMSSR				0x0E	// 16MB share ROM for HSPI
#else
#define	Init_FMSSR				0x1F	// 2MB split ROM
#endif


//------------------------------------------------------------------------------
// HSPI
//------------------------------------------------------------------------------
#define	HSPI_OFF_WITH_VCC		FALSE	// TRUE: Turn off HSPI with VCC power	// leox_20120112
//#define	HSPI_DefferingMode			// Define : HSPI deffering mode

#ifdef	HSPI
// HSPI setting select			// leox_20110930 HSPI_Setting05 -> HSPI_Setting02
//#define	HSPI_Setting01		// Host[All/20MHz],                                           EC[1.READ(03h)/33MHz]
#define	HSPI_Setting02			// Host[FREAD/33MHz, Otherwise/20MHz, DOFR isn't supported],  EC[1.READ(03h)/33MHz, and 2.FREAD(0Bh)/66MHz, and 3.Others/66MHz]
//#define	HSPI_Setting03		// Host[FREAD/33MHz, Otherwise/20MHz, DOFR isn't supported],  EC[1.READ(03h)/33MHz, and 2.Dual(BBh)/33MHz]
//#define	HSPI_Setting04		// Host[DOFR/50MHz,  Otherwise/20MHz, FREAD isn't supported], EC[1.READ(03h)/33MHz, and 2.DIOFR(BBh)/66MHz, and 3.Others/66MHz]
//#define	HSPI_Setting05		// Host[All/20MHz],                                           EC[1.READ(03h)/33MHz, and 2.FREAD(0Bh)/66MHz, and 3.Others/66MHz]
//#define	HSPI_Setting06		// Host[All/20MHz],                                           EC[1.READ(03h)/33MHz, and 2.Dual(BBh)/33MHz]
//#define	HSPI_Setting07		// Host[DOFR/33MHz,  Otherwise/20MHz, FREAD isn't supported], EC[1.READ(03h)/33MHz, and 2.DIOFR(BBh)/66MHz, and 3.Others/66MHz]

#ifdef	HSPI_Setting01
#define	PLLFrequency			PLLFreqSetting03		// 32.3MHz PLL frequency
#define	SPIReadMode				SPIReadMode_0			// Read cycle
#define	HSRSMode				HSPI_RequestSelection_0	// HSPI request Setting 1, 2, 3, and 4
#endif

#ifdef	HSPI_Setting02
#define	PLLFrequency			PLLFreqSetting07		// 64.5MHz PLL frequency
#define	SPIReadMode				SPIReadMode_1			// Fast read cycle
#define	HSRSMode				HSPI_RequestSelection_0	// HSPI request Setting 1, 2, 3, and 4
#endif

#ifdef	HSPI_Setting03
#define	PLLFrequency			PLLFreqSetting03		// 32.3MHz PLL frequency
#define	SPIReadMode				SPIReadMode_3			// Fast read dual input/output
#define	HSRSMode				HSPI_RequestSelection_0	// HSPI request Setting 1, 2, 3, and 4
#endif

#ifdef	HSPI_Setting04
#define	PLLFrequency			PLLFreqSetting07		// 64.5MHz PLL frequency
#define	SPIReadMode				SPIReadMode_3			// Fast read dual input/output
#define	HSRSMode				HSPI_RequestSelection_0	// HSPI request Setting 1, 2, 3, and 4
#endif

#ifdef	HSPI_Setting05
#define	PLLFrequency			PLLFreqSetting07		// 64.5MHz PLL frequency
#define	SPIReadMode				SPIReadMode_1			// Fast read cycle
#define	HSRSMode				HSPI_RequestSelection_2	// HSPI request Setting 5
#endif

#ifdef	HSPI_Setting06
#define	PLLFrequency			PLLFreqSetting03		// 32.3MHz PLL frequency
#define	SPIReadMode				SPIReadMode_3			// Fast read dual input/output
#define	HSRSMode				HSPI_RequestSelection_1	// HSPI request Setting 6 and 7
#endif

#ifdef	HSPI_Setting07
#define	PLLFrequency			PLLFreqSetting07		// 64.5MHz PLL frequency
#define	SPIReadMode				SPIReadMode_3			// Fast read dual input/output
#define	HSRSMode				HSPI_RequestSelection_1	// HSPI request Setting 6 and 7
#endif
#endif	// HSPI


//------------------------------------------------------------------------------
// Keyboard Scan
//------------------------------------------------------------------------------
#define	ExternMatrixGPO			FALSE	// TRUE: Using GPO pin scan keyboard (Note: Just support one GPIO pin now!!!)
#define	GPOKeyPin1Ctl			GPCRF0	// For extend GPIO scan keys
#define	GPOKeyPin1H()			SET_BIT(GPDRF,0)	// leox_20111214 GPOKeyPin1H -> GPOKeyPin1H()
#define	GPOKeyPin1L()			CLR_BIT(GPDRF,0)	// leox_20111214 GPOKeyPin1L -> GPOKeyPin1L()

#define	Extend_Matrix			FALSE	// TRUE: Support KSO16/KSO17 extend matrix	// David Huang 2011/11/28
#define	KB_FnStickKey			FALSE	// TRUE: For Fn stick key function			//@JayFn


//------------------------------------------------------------------------------
// LPC
//------------------------------------------------------------------------------
#ifndef	HSPI
// EC SPI [Fast read, 64.5MHz]
//#define	PLLFrequency		PLLFreqSetting07		// 64.5MHz PLL frequency
//#define	SPIReadMode			SPIReadMode_1			// Fast read cycle

// EC SPI [Read,      46.0MHz]
//#define	PLLFrequency		PLLFreqSetting05		// 46.0MHz PLL frequency
//#define	SPIReadMode			SPIReadMode_0			// Read cycle

// EC SPI [Read,      32.3MHz]
#define	PLLFrequency			PLLFreqSetting03		// 32.3MHz PLL frequency
#define	SPIReadMode				SPIReadMode_1			// Fast read cycle
#endif	//HSPI


//------------------------------------------------------------------------------
// PS2
//------------------------------------------------------------------------------
#ifdef	ITE_EVBoard
#define	TouchPad_only			FALSE	// For ITE EV board only
#else
#define	TouchPad_only			TRUE	// TRUE: Only touch pad device in this project
#endif
// Note: If TouchPad_only is set, remember to call PowerOffClearTPPendingData() in power off sequence (S3, S4, and S5)

#if	TouchPad_only
#define	MultiplexedModes		FALSE
#else
#define	MultiplexedModes		TRUE	// TRUE: Support multiplexed modes
#endif


//------------------------------------------------------------------------------
// SMBus
//------------------------------------------------------------------------------
//#define	SMBusChannel3Support			// Define: GPH1/GPH2 will select SMBus function	// leox_20131218 SMBusChannel4Support -> SMBusChannel3Support	// David modify for MMB service 2014/05/21		// Leo_20160328 FH9U didn't support MMB service
#define	SMBUS_RESET_SUPPORT				// Define: SMBus reset follow spec reset timing	// David_20120327 add for SMBUS reset timing follow SMBUS spec

#define	Support_SMBus_I2C		FALSE	// TRUE: Support I2C-compatible command		// leox_20120725
#define	SMBus_I2C_BIG_BLOCK		FALSE	// TRUE: Support big block transmission		// leox_20120725


//******************************************************************************
// OEM Function Select
//******************************************************************************
#define	S3KeyboardWakeUp		FALSE	// TRUE: Support S3 wake up from internal keyboard
#define	Insyde_Bios				FALSE	// TRUE: Support Insyde BIOS


//------------------------------------------------------------------------------
// Battery
//------------------------------------------------------------------------------
#define	OEM_BATTERY
//#define	Setting_MAX8742ETI_Support

#ifdef	OEM_BATTERY
//#define	BattChg_TBLtoDAC
//#define	BattChg_SMBtoTBLtoDAC
#define	BattChg_SMBtoCHGIC
//#define	BattChg_SMB_OVS
//#define	BattChg_SMB_Limit_ChgCurr
//#define	BatteryOnlyXXPThrollFUN
#define	DLID_Function
//#define	Battery_3Cell_Throttling	// leox20150915 Disabled for FH9C formal release requested by Vincent Weng
#define	Hybrid_Turbo_Boost
#define	SUPPORT_BQ24780S				// leox20150605 Add for FH9C
#define CHARGER_BQ24780S_DBG	TRUE	// TRUE: Enable charger BQ24780S debug	// leox20150810 FH9C	// DEBUG
#define SUPPORT_UNKNOWN_BATT_PN	TRUE	// TRUE: Support unknown battery PN		// leox20150811
#define BAT_DCHG_PROTECT		TRUE	// TRUE: Enable battery DCHG protect	// leox20150826 FH9C	// leox20150907 Requested by Vincent Weng on 2015/09/04
#define BAT_MBATV_PROTECT		TRUE	// TRUE: Enable battery MBATV protect (switch define OEM_VICMVACDC must enable)	// leo_20150916 add power protect	// leox20150916 Code from FB5
#endif

#define	OEM_NECP
#ifdef	OEM_NECP
//#define	NECP_BattRefresh_Support
//#define	Battery_Protection_NEC
#endif

#define	SUPPORT_Shipmode


//------------------------------------------------------------------------------
// Brightness & Display
//------------------------------------------------------------------------------
#define	UpDate_Brighness_Support
//#define	LCDPannelByDAorPWM	// Define: LCD panel controlled by DA, else by PWM
//#define	SetBrightnessForES1
//#define	DISPON_EC_Support
#define	EMI_Update_Support


//------------------------------------------------------------------------------
// Chipset
//------------------------------------------------------------------------------
//#define	Intel_Internal_Lan_Support

//#define	OEM_Calpella
#ifdef	OEM_Calpella
//#define	Calpella_iAMT_Support
#endif


//------------------------------------------------------------------------------
// CIR
//------------------------------------------------------------------------------
#define	CIRFuncSupport			FALSE	// TRUE: Support CIR function
#define	CIR_BAUD_RATE			0x30	// 0x30: 2400bps for RC5, 0x40: 1800bps for NEC	// leox_20120521
#define	CIR_NEC_IMPROVED		FALSE	// TRUE: Use improved NEC protocol				// Ken_20120605


//------------------------------------------------------------------------------
// Device
//------------------------------------------------------------------------------
#define	OEM_TP					// Define: Enable touchpad
//#define	Multiple_TP			// Define: Support multiple touchpad		// ken add for touchpad compare

#define	OEM_IPOD				// Define: Support USB charging for iPod
#ifdef	OEM_IPOD
#define	OEM_IPOD_ACDC
#endif

//#define IT_7230MMB_SUPPORT		// David add for MMB service 2014/05/21		// Leo_20160328 FH9U didn't support MMB service


//------------------------------------------------------------------------------
// Fan & Thermal
//------------------------------------------------------------------------------
#define	OEM_FAN
//#define	FanByDAorPWM					// Define: Fan controlled by DC, else by PWM	// leox20150522 FH9C
//#define	FanAlwaysOnForES1				// Leo_20160329 bring up
//#define	ForceFanOffDuringACPImode
#define	Support_FAN2			FALSE	// TRUE: Support Fan2					// leox_20120315
#define	CHECK_FAN_FAULT			TRUE	// TRUE: Check fan fault				// David_20120613 add for fan error check	// leox_20120615	// Leo_20160329 bring up

#define	OEM_THERM

#define PCHTemperature		FALSE	// TRUE: Get PCH temperature via SMBUS
//#define	OEM_VGA_Support				// TRUE: Support GPU chip				// David add for VGA read thermal sensor 2011/09/15
#ifdef	OEM_VGA_Support
//#define	NVdia_Support				// TRUE: Support Nvida GPU				// David_20140408 modify for NVdia and ATI different function
#define	ATI_Support						// TRUE: Support ATI GPU				// David_20140408 modify for NVdia and ATI different function
#endif

#define	Support_Intel_CPU				// David modify for support Intel CPU 2014/07/02	// leox20150604 FH9C

#ifdef	Support_Intel_CPU
#define	Support_CPU_TYPE		TRUE	// TRUE: Support thermal/fan control by CPU type	// leox_20111205
#else
#define	Support_CPU_TYPE		FALSE	// TRUE: Support thermal/fan control by CPU type	// leox_20111205
#endif

#define	CPU_TYPE_BY_TJMAX		TRUE	// TRUE: Set CPU type by TjMax			// leox_20120229	// leox20150604 FH9C
#define	UPDATE_THERM_POINT		TRUE	// TRUE: Update thermal points for BIOS	// leox_20120229	// leox20150604 FH9C
#define	Support_AMD_SB_TSI		FALSE	// TRUE: Support SB-TSI to read AMD CPU temperature	// leox_20140120

// PECI
#ifdef	PECI_Support
#define	ReadCPUTemperature		TRUE	// TRUE: Get CPU temperature via PECI interface
#define	ReadPKGTemperature		FALSE	// TRUE: Get package temperature via PECI interface
#define	ReadDRAMTemperature		FALSE	// TRUE: Get DRAM temperature via PECI interface
#define	ACTurboControlAlgorithm	FALSE

// PECI host's optimal transfer rate	// leox_20131129
#if Support_IT8987						// leox20150729 PECI setting for IT8987
#define	HOPTTRS_1p8MHz			0x00	// 1.8MHz (default)
#define	HOPTTRS_1p0MHz			0x01	// 1.0MHz
#define	HOPTTRS_1p5MHz			0x04	// 1.5MHz
#define	PECI_TRANSFER_RATE		HOPTTRS_1p0MHz
#else	// For old chip
#define	HOPTTRS_2p0MHz			0x00	// 2.0MHz (default)
#define	HOPTTRS_1p0MHz			0x01	// 1.0MHz
#define	HOPTTRS_1p6MHz			0x04	// 1.6MHz
#define	PECI_TRANSFER_RATE		HOPTTRS_2p0MHz
#endif

// PECI Vtt level						// leox20150729 PECI setting for IT8987
#define	HOVTTS_1p10V			0x00	// 1.10V (default)
#define	HOVTTS_1p05V			0x01	// 1.05V
#define	HOVTTS_1p00V			0x02	// 1.00V
#define	PECI_VTT_SETTING		HOVTTS_1p00V
#endif	// PECI


//------------------------------------------------------------------------------
// LED
//------------------------------------------------------------------------------
//#define	OEM_MotionLed

#define	OEM_DIMM
#ifdef	OEM_DIMM
#define	SW_OEM_DIMM
#endif


//------------------------------------------------------------------------------
// Misc
//------------------------------------------------------------------------------
//#define	chip_8512_Bram				// Define: Chip is 8502/8512 (BRAM size: 64KB), else chip is 8518/8519 (BRAM size: 192KB)
//#define	VistaOSTAT_Support
//#define	LAN_WRITE_PROTECT_Function
#define	SUPPORT_OEM_APP			FALSE	// TRUE: Support OEM application		// leox_20111014
#define	OEM_APP_HOTKEY			FALSE	// TRUE: Support hotkeys by application	// leox_20120508
#define	SIT_BIOS_SETUP_MENU		TRUE	// TRUE: Support launch BIOS setup menu by application	// leox20150612
#define	Support_Wake_On_WLan			// Define: Support Win8 wake on Wlan
#define	SUPPORT_EFLASH_FUNC		TRUE	// TRUE: Support eFlash access function	// leox20150702 Access eFlash
#define	WAIT_PCH_STABLE_MIRROR	FALSE	// TRUE: Wait PCH chip stable after mirror code			// leox20150807 CHECK


//------------------------------------------------------------------------------
// Power
//------------------------------------------------------------------------------
//#define	OEM_S4S5WDG			// Define: Enable S4/S5 software watchdog
//#define	DeepS3_SUPPORT		// 20120104 Chris add for support Deep S3
#define	RESET_EC_LATER			TRUE	// TRUE: Reset EC later after flash		// leox_20111206
#define	OEM_POWER_LOSS			FALSE	// TRUE: Enable power loss function		// leox_20110921
#define	ENTER_SLEEP_MODE_AC		TRUE	// TRUE: Enter sleep mode even AC exist	// leox_20110921
#define	FIX_USB_POWER_ISSUE		TRUE	// TRUE: Fix USB power issue for HW request of Kernel52	// leox_20110922
#define	CMOSprotect				TRUE	// TRUE: Enable CMOS reset to do PCH protect function	// leox20150612 Enabled for FH9C
#define	CHECK_RTC_battery		TRUE


//#define	OEM_VICMV
//#define	OEM_VICMV_PROCHOT			// Define: Throttling by PROCHOT pin	// leox20150731 FH9C
//#define	OEM_DCMI
#define	OEM_VICMVACDC			// (Check switch define BAT_MBATV_PROTECT, if necessary)		// leo_20150916 add power protect

#define ADAPTER_PROTECT_DBG		TRUE	// TRUE: Enable adapter protect debug	// leox20150810 FH9C	// DEBUG


//------------------------------------------------------------------------------
// UART
//------------------------------------------------------------------------------
//#define	UART_SUPPORT
#ifdef	UART_SUPPORT
#define	UART1_SUPPORT
//#define	UART2_SUPPORT
#endif

#endif	/* OEM_PROJECT_H */
