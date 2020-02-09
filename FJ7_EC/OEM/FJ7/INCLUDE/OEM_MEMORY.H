/*------------------------------------------------------------------------------
 * Filename: OEM_MEMORY.C   For Chipset: ITE.IT85XX
 *
 * Function: Memory definition for extern all code base reference
 *
 * [Memory Map Description]
 *
 * Chip Internal RAM: 0x00-0xFF For Kernel and Chip Level use
 *
 * Chip External RAM: 0x000-0xFFF
 *
 * 0x000-0x0FF  For Kernel Core/Module
 * 0x100-0x1FF  OEM RAM 1
 * 0x200-0x2FF  For SPI buffer array
 * 0x300-0x3FF  OEM RAM 3
 * 0x400-0x4FF  OEM RAM 4
 * 0x500-0x5FF  For ITE flash utility, EC SPI rom read write function, and/or Ramdebug function.
 * 0x600-0x6FF  For Keil C Complier and ram code space (for SPI programming)
 * 0x700-0x7FF  OEM RAM 7
 * 0x800-0x8FF  For Ramdebug function
 * 0x900-0x9FF  OEM RAM 9
 * 0xA00-0xAFF  OEM RAM A
 * 0xB00-0xBFF  OEM RAM B
 * 0xC00-0xCFF  OEM RAM C
 * 0xD00-0xDFF  OEM RAM D
 * 0xE00-0xEFF  OEM RAM E
 * 0xF00-0xFFF  For HSPI ram code function
 *
 * Copyright (c) 2006-2009, ITE Tech. Inc. All Rights Reserved.
 *----------------------------------------------------------------------------*/


#ifndef OEM_MEMORY_H
#define OEM_MEMORY_H


//******************************************************************************
// 0x000-0x0FF  OEM RAM 0
//******************************************************************************
// For Kernel Core/Module


//******************************************************************************
// 0x100-0x1FF  OEM RAM 1
//******************************************************************************
// 0x100-0x1FF  OEM RAM 1 --> All not use


//******************************************************************************
// 0x200-0x2FF  OEM RAM 2
//******************************************************************************
extern XBYTE	SPIBuffer[256];	// _at_ 0x0200	(byte) 256 bytes SPI read/write buffer


//******************************************************************************
// 0x300-0x3FF  OEM RAM 3
//******************************************************************************
//extern XBYTE					// _at_ 0x0300	(byte)
//extern XBYTE					// _at_ 0x0301	(byte)
//extern XBYTE					// _at_ 0x0302	(byte)
//extern XBYTE					// _at_ 0x0303	(byte)
//extern XBYTE					// _at_ 0x0304	(byte)
//extern XBYTE					// _at_ 0x0305	(byte)
//extern XBYTE					// _at_ 0x0306	(byte)
//extern XBYTE					// _at_ 0x0307	(byte)
//extern XBYTE					// _at_ 0x0308	(byte)
//extern XBYTE					// _at_ 0x0309	(byte)
//extern XBYTE					// _at_ 0x030A	(byte)
//extern XBYTE					// _at_ 0x030B	(byte)
//extern XBYTE					// _at_ 0x030C	(byte)
//extern XBYTE					// _at_ 0x030D	(byte)
//extern XBYTE					// _at_ 0x030E	(byte)
extern XBYTE	VerMJ;			// _at_ 0x030F	(byte)
extern XBYTE	VerMN;			// _at_ 0x0310	(byte)
extern XBYTE	VerFR;			// _at_ 0x0311	(byte)

extern XWORD	ChkEcRamValid;	// _at_ 0x0312	(word) Check if EC RAM valid	// leox20160317 Check EC RAM

extern XBYTE	WaitS5ToDoWDT;	// _at_ 0x0314	(byte)							// leox20151117 From 0x2207

// For ITE WinFlash Tool ++		// ken 2012/06/29
extern XBYTE	FLHB;			// _at_ 0x0315	(byte) Begin counter			// Set by WinFlash (default 10 sec), by EC counter down, for BEFORE WinFlash
extern XBYTE	FLHE;			// _at_ 0x0316	(byte) End counter				// Set by WinFlash (default 10 sec), by EC counter down, for AFTER  WinFlash

extern XBYTE	FLHH;			// _at_ 0x0317	(byte) Handshaking
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 0xA0: BIOS set ECOK already disable,      for EC check
// 0xFA: BIOS set ECOK already disable,      for WinFlash check, then begin WinFlash
// 0xFE: BIOS set ECOK already disable fail, for WinFlash check, then exit WinFlash, not begin WinFlash
// 0xF0: BIOS set ECOK already re-enable,    for EC check
// 0xFC: BIOS set ECOK already re-enable,    for WinFlash check, then exit WinFlash
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// For ITE WinFlash Tool --

extern XBYTE	SMB_PRTC;		// _at_ 0x0318	(byte) EC SMB1 Protocol register
extern XBYTE	SMB_STS;		// _at_ 0x0319	(byte) EC SMB1 Status register
extern XBYTE	SMB_ADDR;		// _at_ 0x031A	(byte) EC SMB1 Address register
extern XBYTE	SMB_CMD;		// _at_ 0x031B	(byte) EC SMB1 Command register
extern XBYTE	SMB_DATA;		// _at_ 0x031C	(byte) EC SMB1 Data register array (32 bytes)
//extern XBYTE	SMB_DATA1[31];	// _at_ 0x031D	(byte)
extern XBYTE	SMB_DATA1;		// _at_ 0x031D	(byte)
extern XBYTE	SMB_BCNT;		// _at_ 0x033C	(byte) EC SMB1 Block Count register
extern XBYTE	SMB_ALRA;		// _at_ 0x033D	(byte) EC SMB1 Alarm Address register
extern XBYTE	SMB_ALRD0;		// _at_ 0x033E	(byte) EC SMB1 Alarm Data register 0
extern XBYTE	SMB_ALRD1;		// _at_ 0x033F	(byte) EC SMB1 Alarm Data register 1

extern XWORD	Fan_Rpm2;		// _at_ 0x0340	(word) Fan 2 feedback counter	// leox_20120322 Support_FAN2

extern XBYTE	JUMPERSETTING;	// _at_ 0x0342	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define KBD_ID0			BIT0	// 0/1 High/Low:DIPSW(1) Off/On
#define KBD_ID1			BIT1	// 0/1 High/Low:DIPSW(2) Off/On
#define KBD_ID			BIT2	// 0/1 High/Low:DIPSW(3) Off/On
#define Model_ID		BIT3	// 0/1 High/Low:Mode select High/Low level
#define Watt_ID			BIT4	// 0/1 High/Low:Watt select 15W/28W
#define LSSupportFlag	BIT5	// 1 Light Sensor Support
#define LANWP_Disable	BIT6	// 1 LAN Write Protect Disable by BIOS or Tool Set/Clear	// LAN_WRITE_PROTECT_Function
#define MEFW_OVERRIDE	BIT7	// 1/0 MEFW_OVERRIDE enable/disable, this bit by utility or BIOS or user control

extern XBYTE	BANKDEFINE;		// _at_ 0x0343	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define				BIT0
//#define				BIT1
//#define				BIT2
//#define				BIT3
#define EC_bank0		BIT4	// EC's RAM bank selector 0
#define EC_bank1		BIT5	// EC's RAM bank selector 1
#define EC_bank2		BIT6	// EC's RAM bank selector 2
#define EC_bank3		BIT7	// EC's RAM bank selector 3

extern XBYTE	POWER_STAT_CTRL;// _at_ 0x0344	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define adapter_in		BIT0	// 1 AC adapter exist
#define WakeOnRTC		BIT1	// 1 Wake On RTC Support
#define WakeOnLan		BIT2	// 1 Wake On Lan Support
#define SWILIDWakeFlag	BIT3	// EC setting for BIOS use and by BIOS clear
#define LIDS4WakeFlag	BIT4	// EC setting for BIOS use and by BIOS clear
#define enter_S4		BIT5	// 1 enter S4
#define SysPwrSourceAC	BIT6	// 1 System power source is AC, 0 System power source is DC
#define DIMM_OFF_Flag	BIT7	// 1 DIMM Off/Leds on, 0 DIMM On/Leds off		// OEM_DIMM

extern XBYTE	KETBOARD_STAT_CTRL;	// _at_ 0x0345	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define WakeByCritical	BIT0	// 1 Wake up by critical battery threshold from S3 (Set by EC)
#define LCtrlFn_RCtrl	BIT1	// 1 Support LeftCtrl plus Fn equal RightCtrl (By BIOS Cmos setting)
#define NotReadPECI		BIT2	// 1/0 Not read PECI/Normal read PECI, state change (CommonStateChange) clear by EC/set or clear by BIOS during S0	// OEM_Calpella
#define CRISIS_ON		BIT3	// 1 Crisis flash enabled
#define SKIPMATRIX		BIT4	// 1 Skip Polling Keyboard Matrix
#define Critical_Wake	BIT5	// 1 Support Critical battery wake up from S3 (Set/Clear by BIOS)
#define OS_Type			BIT6	// 1/0 Win8/Win7
#define WakeOnUMTS		BIT7	// 1 Enable wake on UMTS

extern XWORD	BT1I_BIOS;		// _at_ 0x0346	(word) Batt1_Rate (mA)
extern XWORD	BT1C_BIOS;		// _at_ 0x0348	(word) Batt1_Remain_Cap (mAh)
extern XWORD	BT1V_BIOS;		// _at_ 0x034A	(word) Batt1_Volt (mV)
extern XWORD	BT2I_BIOS;		// _at_ 0x034C	(word) Batt2_Rate (mA)
extern XWORD	BT2C_BIOS;		// _at_ 0x034E	(word) Batt2_Remain_Cap (mAh)
extern XWORD	BT2V_BIOS;		// _at_ 0x0350	(word) Batt2_Volt (mV)

extern XBYTE	Device_STAT_CTRL;	// _at_ 0x0352	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ECO_LED_ON_BLUE			BIT0	// 1 ECO LED BLUE on
#define ECO_LED_ON_GREEN		BIT1	// 1 ECO LED GREEN on
#define ECO_LED_ON_LightBLUE	BIT2	// 1 ECO LED Light BLUE on
#define Lan_Status				BIT3	// 1 RF on, 0 off
#define BT_Status				BIT4	// 1 BlueTooth on, 0 off
#define evt_lid					BIT5	// Lid switch
#define PowerButtonPowerOn		BIT6	// 1 Indicate Power Button power on by EC setting, BIOS clearing
#define DISPON_Status			BIT7	// 1/0 DISPLAY on/off					// DISPON_EC_Support

extern XBYTE	ChkReadWriteBattFirstUSEDate;	// _at_ 0x0353	(byte) FA/FB Read/Write BattFirstUSEDate
extern XBYTE	WriteBattFirstUSEDate;			// _at_ 0x0354	(byte)
extern XBYTE	WriteBattFirstUSEDatehighbyte;	// _at_ 0x0355	(byte)

extern XWORD	ReadBattFirstUSEDate;			// _at_ 0x0356	(word)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BIT0~BIT4 ---> day
// BIT5~BIT8 ---> month
// BIT9~BIT14 --> year + 1980
//      BIT15 --> Sing bit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	CPU_Remote_Temp;	// _at_ 0x0358	(byte) CPU Remote Temperature

// David_20120613 add for fan error check	// CHECK_FAN_FAULT
extern XBYTE	Device_STAT_CTRL1;	// _at_ 0x0359	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ThreeG_Status	BIT0	// 1 3G on, 0 off
#define CCD_Status		BIT1	// 1 CCD on, 0 off
#define VGA_Status		BIT2	// 1 VGA exist, 0 No VGA exist
#define Fan1_Fault_On	BIT3	// 1 Fan1 fault, 0 Fan OK
#define CCD_Kill_En		BIT4	// 1 CCD kill Enable, 0 Disable
#define CCD_Kill_Fn_En	BIT5	// 1 CCD kill hotkey Enable, 0 Disable
#define Fan2_Fault_On	BIT6	// 1 Fan2 fault, 0 Fan OK
#define Thrott_3cell	BIT7	// 1 3cell throttling enable
// David_20120613 add for fan error check

extern XBYTE	POWER_STAT_CTRL1;	// _at_ 0x035A	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define S5_Reboot		BIT0	// 1 S5_On not off, 0 S5_On off
#define AC_CUT			BIT1	// 1 AC off Enable, 0 Disable
#define CHG_STP			BIT2	// 1 Charge stop Enable, 0 Disable
#define ECO_Mode		BIT3	// 1 ECO mode Enable, 0 Disable
#define AC_Status		BIT4	// 1 Keep in AC mode, 0 Keep in DC mode
#define CHG_Status		BIT5	// 1 can process Charge; 0 not process charge
#define lowpower_CHG	BIT6	// 1 low power start charge till "100%/AC out/power status" change clear
#define WakeOnWLan		BIT7	// 1 Wake on WLan enable

extern XBYTE	Critical_Wake_Level;	// _at_ 0x035B	(byte) Critical battery threshold for wake up from S3 (Set by BIOS)

extern XBYTE	ChargeLimit_CTRL;	// _at_ 0x035C	(byte) Charge limit control	//David_20141222 add for charge limit function
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Charge_Limit_En	BIT0	// 1 Enable charge limit, 0 Disable charge limit

extern XBYTE	CPUTjMaxOEM;	// _at_ 0x035D	(byte) CPU TjMax by OEM			// leox_20120301 CPU_TYPE_BY_TJMAX

extern XBYTE	ThrottlingTemp;	// _at_ 0x035E	(byte) Throttling point			// leox_20120229 UPDATE_THERM_POINT
extern XBYTE	OSShutdownTemp;	// _at_ 0x035F	(byte) OS shutdown point		// leox_20120229 UPDATE_THERM_POINT

extern XBYTE	USB_CutOffpower;// _at_ 0x0360	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define CutOffUSBP		BIT0	// Cut-Off USBP power,      BIOS set EC clear
#define CutOffUSBP_IPOD	BIT1	// Cut-Off USBP IPOD power, BIOS set EC clear	// OEM_IPOD
#define CutOffUSBP2		BIT2	// Cut-Off USBP2 power,     BIOS set EC clear

extern XBYTE	USBCHARGEDISCHARG;		// _at_ 0x0361	(byte)					// OEM_IPOD
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define						BIT0
//#define						BIT1
//#define						BIT2
#define USB4_CHARG_Enable		BIT3	// BIOS set/clear, default clear
#define USB4_CHARG_Enable_ACBAT	BIT4	// BIOS set/clear, default clear (AC and DC Enable/AC Enable 1/0), when USB4_CHARG_Enable_BRAM set then check this bit	// OEM_IPOD_ACDC
#define USB4_CHARG_FastMode		BIT5	// BIOS set/clear, default clear (1/0 for Fast(CDP)/Normal(SDP) mode)	// leox20150707 For USB charger SLG55544 of FH9C

extern XBYTE	InitUSBChargePortCNT;	// _at_ 0x0362	(byte) InitUSBChargePort Counter	// OEM_IPOD

extern XBYTE	SELECTOR_STATE1;// _at_ 0x0363	(byte)
extern XBYTE	SELECTOR_STATE2;// _at_ 0x0364	(byte)

extern XBYTE	CPU_TYPE;		// _at_ 0x0365	(byte)							// OEM_FAN
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define CPU_TJ85		BIT0	// 1/0: TJ85 CPU,  BIOS set/clear, default clear
#define CPU_TJ90		BIT1	// 1/0: TJ90 CPU,  BIOS set/clear, default clear
#define CPU_TJ100		BIT2	// 1/0: TJ100 CPU, BIOS set/clear, default clear
#define CPU_TJ105		BIT3	// 1/0: TJ105 CPU, BIOS set/clear, default clear
//#define				BIT4
//#define				BIT5
//#define				BIT6
#define Silent_Mode		BIT7	// 1 Silent mode, 0 Normal mode

extern XWORD	Fan_Rpm;		// _at_ 0x0366	(word) Fan 1 feedback counter
extern XBYTE	DummySCI_CNT;	// _at_ 0x0368	(byte) Send dummy SCI counter every 1 sec

extern XBYTE	Device_Setup_Ctrl;	// _at_ 0x0369	(byte) Setup menu control device
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Lan_Setup_Status	BIT0	// Setup menu Lan status
#define BT_Setup_Status		BIT1	// Setup menu BT status

extern XWORD	BIOS_DESIGN_CAPACITY;		// _at_ 0x036A	(word) Battery design capacity	// leox_20120402 Change offset
extern XWORD	BIOS_DESIGN_FULL_CAPACITY;	// _at_ 0x036C	(word) Battery full charge capacity

extern XBYTE	NECBattRefreshPSTS;	// _at_ 0x036E	(byte)						// NECP_BattRefresh_Support
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define CHGC				BIT0
#define ADPC				BIT1
// ADPC  CHGC
// '1'   '1'   By EC Setting (Inition Battery Refresh step0) --> EC Do Inition Battery Refresh Function
// '0'   '0'   By AP Setting (Battery Refresh Step1)         --> BIOS --> EC Do Force Dischg
// '1'    0    By EC Setting (When Battey Low Step2)         --> EC Do Stop Chg and Dischg Done
//  1    '1'   By AP Setting (Battery Refresh Step3)         --> BIOS --> EC Do Force Chg Normal
//#define					BIT2
//#define					BIT3
//#define					BIT4
#define ForceDischgDoing	BIT5	// 1 Force Dischg Doing
#define StopChgDischgDoing	BIT6	// 1 Stop ChgDischg Doing

extern XBYTE	NECBattRefreshPSTS01;	// _at_ 0x036F	(byte)					// NECP_BattRefresh_Support
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ToNormalCondition01_OK	BIT0	// 1 Ever Do ToNormalCondition01

extern XBYTE	BIOS_TouchpadSWFnFlag;	// _at_ 0x0370	(byte)

extern XBYTE	BIOS_ABAT_STATUS;		// _at_ 0x0371	(byte)
extern XBYTE	BIOS_ACHARGE_STATUS;	// _at_ 0x0372	(byte)
extern XBYTE	BIOS_ABAT_MISC;			// _at_ 0x0373	(byte)
//David add for FJ request change battery CV 2016/11/08
extern XBYTE	Battery_Status;		// _at_ 0x0374;	// (byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define	Battery_Old			BIT0		// Battery percentage <= 70%
#define	Battery_FW_Old		BIT1		// 1 Old FW not support battery old
#define	Start_Chg			BIT2		// 1 Start Precharge or normal charge
//David add for FJ request change battery CV 2016/11/08
#define	No_RTC				BIT3		// 1 RTC battery power low or it doesn't exist

extern XWORD	BIOS_DESIGN_VOLTAGE;	// _at_ 0x0375	(word) Battery design voltage
extern XBYTE	BIOS_ABAT_CAP;			// _at_ 0x0377	(byte) ABAT batt capacity (RSOC %)
extern XWORD	BIOS_BateryTempSBMus;	// _at_ 0x0378	(word) Battery temperature from SMBus

extern XBYTE	BrightnessValue;// _at_ 0x037A	(byte) Brightness value
extern XBYTE	BrightnessLevel;// _at_ 0x037B	(byte) Brightness level	// leox_20120111

extern XBYTE	VolumeValue;	// _at_ 0x037C	(byte) Volume value		// leox_20111014 SUPPORT_OEM_APP
extern XBYTE	VolumeLevel;	// _at_ 0x037D	(byte) Volume level		// leox_20120111

extern XBYTE	BIOS_ShutDnCause;		// _at_ 0x037E	(byte) System shutdown cause	// leox20150922

//David_20120327 modify for battery information		// leox_20120402 Change offset
extern XBYTE	BarcodeBCNT;	// _at_ 0x037F	(byte)
extern XBYTE	Barcode32B;		// _at_ 0x0380	(byte) Range: 0x0380~0x039F
//David_20120327 modify for battery information

#if SUPPORT_OEM_APP				// leox_20111014 Support OEM application +++
extern XBYTE	AppReady;		// _at_ 0x03A0	(byte) Application state
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 0xAA: EC set with ACPI_OS
// 0xFA: App is working
// 0xFF: App was closed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	AppGetVolCnt;	// _at_ 0x03A1	(byte) Counter for volume get	// leox_20111017
extern XBYTE	AppCommand;		// _at_ 0x03A2	(byte) Command from application	// leox_20111109

extern XBYTE	AppMuteCnt;		// _at_ 0x03A3	(byte) Counter for volume mute	// leox_20120410
extern XBYTE	AppVol0Cnt;		// _at_ 0x03A4	(byte) Counter for volume down
extern XBYTE	AppVol1Cnt;		// _at_ 0x03A5	(byte) Counter for volume up

extern XBYTE	AppTaskFlag;	// _at_ 0x03A6	(byte) Task flag for application
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SetVol_F		BIT0	// Set volume via application
#define GetVol_F		BIT1	// Get volume via application
#define WaitVol_F		BIT2	// Wait application update volume
#define HandshakeToApp	BIT3	// Send handshaking to application
#define SetMute_F		BIT4	// Set OS mute state via application			// leox_20120411
#define GetMute_F		BIT5	// Get OS mute state via application			// leox_20120410
//#define				BIT6
#define HandshakeMute	BIT7	// Send handshaking to application for mute		// leox_20120410

extern XBYTE	AppSetVolume;	// _at_ 0x03A7	(byte) Set volume via application using Q54_SCI
extern XBYTE	AppGetVolume;	// _at_ 0x03A8	(byte) Get volume via application using Q53_SCI

extern XBYTE	RW_EC_AdrL;		// _at_ 0x03A9	(byte) Address for R/W EC set by BIOS (Low  byte)

extern XBYTE	RW_EC_AdrH;		// _at_ 0x03AA	(byte) Address for R/W EC set by BIOS (High byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BIT [7:6] - EC command indicator
//             00: Do nothing
//             01: Read EC
//             10: Write EC
//             11: Do nothing
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	RW_EC_Data;		// _at_ 0x03AB	(byte) Data for R/W EC set by BIOS

extern XBYTE	AppGetMute;		// _at_ 0x03AC	(byte) Get mute state via application using Q51_SCI	// leox_20120411
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 0xAA: Un-mute
// 0xBB: Mute
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//extern XBYTE					// _at_ 0x03AD	(byte)
//extern XBYTE					// _at_ 0x03AE	(byte)
//extern XBYTE					// _at_ 0x03AF	(byte)
#endif	// SUPPORT_OEM_APP		// leox_20111014 Support OEM application ---

extern XWORD	REGISTER_ADDR;	// _at_ 0x03B0	(word) Physical 591 register address
extern XWORD	REGISTER_DATA;	// _at_ 0x03B2	(word) Content of 591 register

extern XBYTE	WatchDOGS4S5_CNT;	// _at_ 0x03B4	(byte)						// OEM_S4S5WDG

//extern XBYTE					// _at_ 0x03B5	(byte)

#ifdef OEM_VICMV
extern XWORD	VICM_V;					// _at_ 0x03B6	(word)
extern XBYTE	VICMVHiEnThroLimitCnt;	// _at_ 0x03B8	(byte) Enabled CPU speed down by VICMV debounce counter
extern XBYTE	VICMVHiDisThroLimitCnt;	// _at_ 0x03B9	(byte) Disable CPU speed down by VICMV debounce counter

extern XBYTE	VICMVHiFlag;			// _at_ 0x03BA	(byte) VICMV flag
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Q42Q43_SCI		BIT0	// 1/0 VICMV Hi EnThroLimit On/Off
#endif	// OEM_VICMV

#ifdef OEM_DCMI
extern XWORD	DCMI_V;					// _at_ 0x03BB	(word)
extern XBYTE	DCMIHiEnThroLimitCnt;	// _at_ 0x03BD	(byte) Enabled CPU speed down by DCMI debounce counter
extern XBYTE	DCMIHiDisThroLimitCnt;	// _at_ 0x03BE	(byte) Disable CPU speed down by DCMI debounce counter

extern XBYTE	DCMIHiFlag;				// _at_ 0x03BF	(byte) DCMI flag
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Q44Q45_SCI		BIT0	// 1/0 DCMI Hi EnThroLimit On/Off
#endif	// OEM_DCMI

#ifdef OEM_VICMVACDC
extern XWORD	VICM_VACDC;					// _at_ 0x03C0	(word)
extern XBYTE	VICMVHiEnThroLimitCntACDC;	// _at_ 0x03C2	(byte) Enabled CPU speed down by VICMV debounce counter
extern XBYTE	VICMVHiDisThroLimitCntACDC;	// _at_ 0x03C3	(byte) Disable CPU speed down by VICMV debounce counter

extern XBYTE	VICMVHiFlagACDC;			// _at_ 0x03C4	(byte) VICMV flag
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Q42Q43_SCIACDC	BIT0	// 1/0 VICMV Hi EnThroLimit On/Off
// leo_20150916 add power protect +++		// BAT_MBATV_PROTECT
#define DC_OW_Prochot	BIT1
#define DC_OCP_Prochot	BIT2
#define AC_OC_Prochot	BIT3
#define AC_OCP_Prochot	BIT4
// leo_20150916 add power protect ---

extern XWORD	CmpEnVICM_VACDC_Limit;		// _at_ 0x03C5	(word)
extern XWORD	CmpDisVICM_VACDC_Limit;		// _at_ 0x03C7	(word)
#endif	// OEM_VICMVACDC

extern XBYTE	TPTYPE4BIOS;	// _at_ 0x03C9	(byte) Touchpad type for BIOS	// ken add for touchpad compare // Multiple_TP
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 00: Unknown
// 01: SE: Sentelic // E9 FA 00 FE 01
// 02: EL: Elan     // E9 FA 00 02 64
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	FanControlByBIOS;	// _at_ 0x03CA	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ECIDLENotForceTurnOnFan	BIT0	// 1 When EC receive IDLE command not force turn on fan, BIOS setting EC clear

extern XBYTE	BatteryCellNum;	// _at_ 0x03CB	(byte)
extern XBYTE	KCMISCBIT0CNT;	// _at_ 0x03CC	(byte)
extern XBYTE	KCMISCBIT1CNT;	// _at_ 0x03CD	(byte)
extern XWORD	BatterySerialNo;// _at_ 0x03CE	(word)

//David_20120327 modify for battery information			// leox_20120403 Change range
extern XBYTE	DeviceName15B;	// _at_ 0x03D0	(byte) 0x03D0~0x03DE
extern XBYTE	DeviceNameBCNT;	// _at_ 0x03DF	(byte)
//David_20120327 modify for battery information

extern XWORD	BatteryManuFactureDate;	// _at_ 0x03E0	(word)

extern XWORD	BIOS_BTCyCle;	// _at_ 0x03E2	(word)	//David_20141222 add battery cycle count

//David_20141222 add support different adapter
extern XWORD	CmpEnHybrid_Limit;	// _at_ 0x03E4	(word)
extern XWORD	CmpDisHybrid_Limit;	// _at_ 0x03E6	(word)
extern XWORD	CmpEnVICM_Limit;	// _at_ 0x03E8	(word)
extern XWORD	CmpDisVICM_Limit;	// _at_ 0x03EA	(word)
//David_20141222 add support different adapter

extern XBYTE	Factory_KBTest_status;	// _at_ 0x03EC	(byte)	//David_20141222 add for factory test
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define LTab			BIT0
#define RTab			BIT1


//David_20141222 add for MMB wake up support
extern XBYTE	MMB_WakeUp_S3;	// _at_ 0x03ED	(byte)
extern XBYTE	MMB_WakeUp_S4;	// _at_ 0x03EE	(byte)

extern XBYTE	MMB_WakeUp_S5;	// _at_ 0x03EF	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MMB_Button3_Wake	BIT0
#define MMB_Button2_Wake	BIT1
#define MMB_Button1_Wake	BIT2
//David_20141222 add for MMB wake up support

//David_20120327 modify for battery information	// leox_20120402 Change offset
extern XBYTE	Manu_NameBCNT;	// _at_ 0x03F0	(byte)
extern XBYTE	Manu_Name11B;	// _at_ 0x03F1	(byte) Range: 0x03F1~0x03FB
//David_20120327 modify for battery information

extern XBYTE	MMB_Status_OEM;	// _at_ 0x03FC	(byte)	//David_20141222 add MMB button

extern XBYTE	CrisisKeyStatus;// _at_ 0x03FD	(byte) Crisis key flag
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define CrisisKey0		BIT0
#define CrisisKey1		BIT1
#define CrisisKey2		BIT2

extern XBYTE	Compare_LowByte;	// _at_ 0x03FE	(byte)
extern XBYTE	Compare_HighByte;	// _at_ 0x03FF	(byte)


//******************************************************************************
// 0x400-0x4FF  OEM RAM 4
//******************************************************************************
#if CHARGER_BQ24780S_DBG			// leox20150617 Add for charger IC debug	// leox20150810
extern XWORD	ChgIcRegR[13];		// _at_ 0x0400	(byte) Register state (26 bytes)(0x0400-0x0419)
extern XBYTE	ChgIcCnt;			// _at_ 0x041A	(byte) Counter for polling registers
extern XBYTE	ChgIcTmp;			// _at_ 0x041B	(byte) Counter for hook function
extern XWORD	CmpEnHybrid_LimitD;	// _at_ 0x041C	(word) Hybrid power trigger point for debug		// leox20150716 Move up
extern XWORD	CmpDsHybrid_LimitD;	// _at_ 0x041E	(word) Hybrid power release point for debug		// leox20150716 Move up
extern XWORD	ChgIcRegW[11];		// _at_ 0x0420	(byte) Register debug (22 bytes)(0x0420-0x0435)	// leox20160317 Charger debug
//extern XBYTE						// _at_ 0x0436	(byte)
//extern XBYTE						// _at_ 0x0437	(byte)
extern XWORD	ChgIcRegF;			// _at_ 0x0438	(word) Register debug write flag				// leox20160317 Charger debug
#endif

#if ADAPTER_PROTECT_DBG				// leox20150716 Adapter protection debug	// leox20150810
extern XBYTE	VICMVEnThrottleDebD;// _at_ 0x043A	(byte) Enable  CPU throttling de-bounce time	// leox20150717 Adapter protection debug
extern XBYTE	VICMVDsThrottleDebD;// _at_ 0x043B	(byte) Disable CPU throttling de-bounce time	// leox20150717 Adapter protection debug
extern XWORD	CmpEnVICM_LimitD;	// _at_ 0x043C	(word) Adapter protect trigger point for debug	// leox20150716 Adapter protection debug
extern XWORD	CmpDsVICM_LimitD;	// _at_ 0x043E	(word) Adapter protect release point for debug	// leox20150716 Adapter protection debug
#endif

extern XBYTE	ShutDnCause;	// _at_ 0x0440	(byte) System shutdown cause
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 01h: Command 21h turn off host power (WPCmd21_OffPWR)
// 02h: Command 22h reset host (WPCmd22_Reset)
// 03h: Mechanical Off (4 secs overwrite power off)
// 04h: Hardware power not good in system off
// 05h: Hardware power not good in system on
// 06h: Battery were fully discharge
// 07h: VAG Remote
// 07h: VAG Remote (MCH:PECI)													// OEM_Calpella
// 08h: SUSB and SUSC off (S0 to S4S5)
// 09h: 66-8D
// 0Bh: SUSB and SUSC off (S3 to S4S5)
// 0Ah: 64-8C
// 0Eh: VGA Thermal sensor SMBus error
// 0Fh: CPU Thermal sensor SMBus error
// 17h: VGA Local
// 19h: 6C-8D
// 27h: CPU Local
// 0Eh: VGA Thermal sensor SMBus error (MCH SMBus error:PECI)					// OEM_Calpella
// 0Fh: CPU Thermal sensor SMBus error (CPU SMBus error:PECI)					// OEM_Calpella
// 17h: VGA Local (None Shutdown:PECI Byte Count)								// OEM_Calpella
// 1Eh: PECI SMBus error:PECI													// OEM_Calpella
// 1Fh: PCH SMBus error:PECI													// OEM_Calpella
// 27h: CPU Local (PCH:PECI)													// OEM_Calpella
// 8Ch: 66-8C Battery Low
// ACh: 6C-8C Battery Low
// C0h: CMOS reset																// CMOSprotect
// C1h: SHDN reset																// CPU over 125C protect	// leo_20160715 add for EE Jim/Jackson
// C3h: 66-C3 Battery Low
// F0h: VGA NVEDIA Thermal sensor SMBus error
// F1h: VAG NVEDIA Remote
// F2h: CPU Remote
// F2h: CPU Remote (CPU:PECI)													// OEM_Calpella
// F5h: 66-8C No Battery Low
// F6h: 6C-8C No Battery Low
// F7h: 66-8C BIOS Watchdog Reboot No Battery Low
// F8h: 66-8C S4S5 Watchdog Reboot No Battery Low not turn off RVCC and STB
// F9h: 66-8C S4S5 Watchdog Reboot Battery Low
// FAh: EC reset
// FBh: Reboot mark																// leox_20110926
// FCh: Last reset source is PWRSW timeout reset (LRSIPWRSWTR)					// leox20150728 PWRSW_WDT_1
// FDh: AC/DC over current/watt 60 seconds										// leo_20150916 add power protect	// BAT_MBATV_PROTECT
// FEh: AC/DC over current peak													// leo_20150916 add power protect	// BAT_MBATV_PROTECT
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	DA_CCSET_BackUp;		// _at_ 0x0441	(byte) DA_CCSET value Back Up when CHG# is high
extern XWORD	FCC99P_CAPACITY;		// _at_ 0x0442	(word) 99% of FCC value
extern XWORD	FCCxP_CAPACITY;			// _at_ 0x0444	(word) (ForceDecPercent/2)% of FCC value
extern XWORD	DESIGN_FULL_CAPACITY;	// _at_ 0x0446	(word) Battery full charge capacity
extern XWORD	DESIGN_CAPACITY;		// _at_ 0x0448	(word) Battery design capacity
extern XWORD	DESIGN_VOLTAGE;			// _at_ 0x044A	(word) Battery design voltage
extern XBYTE	RSOC_FULL_BASE;			// _at_ 0x044C	(byte) RSOC-base detector threshold
extern XBYTE	FAN_LIMIT2;				// _at_ 0x044D	(byte) Desire Fan2 speed	// leox_20120322 Support_FAN2
extern XBYTE	CPU_Local_Temp;			// _at_ 0x044E	(byte) CPU Local temperature
extern XBYTE	VGA_Remote_Temp;		// _at_ 0x044F	(byte) VGA Remote Temperature
extern XBYTE	VGA_Local_Temp;			// _at_ 0x0450	(byte) VGA Local temperature
extern XBYTE	DBG_CPU_Remote_Temp;	// _at_ 0x0451	(byte) Debug CPU Remote Temp
extern XBYTE	DBG_CPU_Local_Temp;		// _at_ 0x0452	(byte) Debug CPU Local Temp
extern XBYTE	DBG_VGA_Remote_Temp;	// _at_ 0x0453	(byte) Debug VGA Remote Temp
extern XBYTE	DBG_VGA_Local_Temp;		// _at_ 0x0454	(byte) Debug VGA Local Temp
extern XBYTE	FAN_LIMIT_CPU;			// _at_ 0x0455	(byte) CPU Fan Limit
extern XBYTE	FAN_LIMIT_VGA;			// _at_ 0x0456	(byte) VGA Fan Limit
extern XBYTE	FAN_LIMIT;				// _at_ 0x0457	(byte) Desire fan speed
extern XBYTE	RdSCIevent_backup;		// _at_ 0x0458	(byte) Read SCI Event Back Up
extern XBYTE	WrSCIevent_backup;		// _at_ 0x0459	(byte) Write SCI Event Back Up
extern XBYTE	RdSMIevent_backup;		// _at_ 0x045A	(byte) Read SMI Event Back Up
extern XBYTE	WrSMIevent_backup;		// _at_ 0x045B	(byte) Write SCI Event Back Up
extern XWORD	FLED_FLASH_CNT;			// _at_ 0x045C	(word) Counter for LED flash control

extern XBYTE	POWER_FLAG;		// _at_ 0x045E	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ACPI_OS			BIT0	// 1 System in ACPI environment
#define power_on		BIT1	// 1 System power on
#define enter_SUS		BIT2	// 1 Host enter suspend to RAM mode
#define wait_PSW_off	BIT3	// 1 Waiting for PSW released
#define PwrOffRVCCOff	BIT4	// 1 Before power off include turn off RVCC, 0 Before power off include not turn off RVCC
#define system_on		BIT5	// 1 System on (SWPG issued)
#define wait_HWPG		BIT6	// 1 Wait hardware power good
#define Deep_S3			BIT7	// 1 Enter deep S3	// 20120104 Chris add for support Deep S3 DeepS3_SUPPORT

extern XBYTE	POWER_FLAG2;	// _at_ 0x045F	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define No_Dimm			BIT0	// 1 No_Dimm
#define Quick_S3		BIT1	// 1 S3 Quick mode
#define DisQevent		BIT2	// 1 Disable Q-Event
#define bat_warning		BIT3	// 1 Battery warning
#define FAN_ON			BIT4	// 1/0 Detect Fan On/Off
#define FAN2_ON			BIT5	// 1/0 Detect Fan2 On/Off	// leox_20120322 Support_FAN2
#define func_DUAL		BIT6	// 1 Dual battery supported
#define func_LED		BIT7	// 1 LED indicator enable

extern XBYTE	SYS_FLAG;		// _at_ 0x0460	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define BIOS_Flash_Reset		BIT0
#define in_standby				BIT1	// 1 System enter standby mode
#define enable_SMI				BIT2	// 1 Allow SMI
#define VGA_Sensor_SMB_error	BIT3	// 1 The VGA Thermal Sensor SMBus Error
#define CPU_Sensor_SMB_error	BIT4	// 1 The CPU Thermal Sensor SMBus Error
#define main_sel				BIT5	// 1 Main battery selected
#define delay_PCUSMI			BIT6	// 1 Host busy issue PCU SMI later
#define polling_BAT				BIT7	// 1 Battery polling in progress

extern XBYTE	MISC_FLAG;		// _at_ 0x0461	(byte) System misc flags
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define F_DelayPSW				BIT0	// 1 Delay Send DNBSWOn
#define EnTempDebug				BIT1	// 1 Enable Emulator VPU and VGA Temp
#define FAN_TEST				BIT2	// 1 Enabled fan test control
#define PCBeep_Status			BIT3	// 1 PC Beep on, 0 off
#define F_CriLow				BIT4	// 1 Battery critical low
#define PCH_Sensor_SMB_error	BIT5	// 1 PCH Thermal Sensor SMBus Error		// OEM_Calpella
#define PECI_Sensor_SMB_error	BIT6	// 1 PECI Thermal Sensor SMBus Error	// OEM_Calpella
#define Turbo_On				BIT7	// 1 Turbo on, 0 Turbo off

extern XBYTE	NECOverChgCurrent;	// _at_ 0x0462	(byte) For NEC check over charging current Ram
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ForceStopChgOCC				BIT0	// 1 Force stop charging by tool set/clear
#define StopChgCurOVSbyBatt			BIT1	// 1 Charging Current Over Max Spec form Battery by SMBus	// BattChg_SMB_OVS
//#define							BIT2
//#define							BIT3
//#define							BIT4
//#define							BIT5
//#define							BIT6
#define C_First_USE_Date_Bit15_Set	BIT7	// If C_First_USE_Date_Bit15_Set = 0 --> Follow normal chg/dischg condition	// Battery_Protection_NEC
//											// If C_First_USE_Date_Bit15_Set = 1 --> Force not charge even AC and battery exist

extern XBYTE	MotionLedsFunctionCNT;	// _at_ 0x0463	(byte)					// OEM_MotionLed

extern XBYTE	MotionLedsFunctionFlag;	// _at_ 0x0464	(byte)					// OEM_MotionLed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define EnableMotionLedsFun		BIT0	// 1 When SetClearLEDs During execute MotionLedsFunction temporary
//										// Disable display Leds but still save Leds status (LedBackup)

extern XWORD	BT1I;			// _at_ 0x0465	(word) Batt1_Rate (mA)
extern XWORD	BT1C;			// _at_ 0x0467	(word) Batt1_Remain_Cap (mAh)
extern XWORD	BT1V;			// _at_ 0x0469	(word) Batt1_Volt (mV)
extern XWORD	BT2I;			// _at_ 0x046B	(word) Batt2_Rate (mA)
extern XWORD	BT2C;			// _at_ 0x046D	(word) Batt2_Remain_Cap (mAh)
extern XWORD	BT2V;			// _at_ 0x046F	(word) Batt2_Volt (mV)

extern XWORD	BTCyCle;		// _at_ 0x0471	(word) Batt Cycle Count	//David_20141222 add battery cycle count
//extern XBYTE					// _at_ 0x0473	(byte)

#if BAT_DCHG_PROTECT					// leox20150826 Battery DCHG protection
extern XWORD	DCHG_V;					// _at_ 0x0474	(word)
extern XWORD	CmpEnDCHG_Limit;		// _at_ 0x0476	(word)
extern XWORD	CmpDsDCHG_Limit;		// _at_ 0x0478	(word)
extern XBYTE	DCHGVHiEnThroLimitCnt;	// _at_ 0x047A	(byte) Enabled CPU speed down by DCHGV debounce counter
extern XBYTE	DCHGVHiDsThroLimitCnt;	// _at_ 0x047B	(byte) Disable CPU speed down by DCHGV debounce counter
extern XBYTE	DCHGVHiEnThroLimitCntD;	// _at_ 0x047C	(byte)
extern XBYTE	DCHGVHiDsThroLimitCntD;	// _at_ 0x047D	(byte)
#endif

extern XBYTE	Device_STAT_CTRL2;			// _at_ 0x047E	(byte)	// leo_20160420 add for new feature
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Lid_open		BIT0				// 1/0 Lid open/close		// leo_20160420 add for cost down by EE Jackson wu
#define NumLock			BIT1				// 1/0 NumLock on/off		// leo_20160510 add for "00" key no output when NumLock off


extern XBYTE	Hybrid_Status;	// _at_ 0x047F	(byte)	//David_20141222 add support hybrid power
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Hybrid_En		BIT0	// 1 Enable power hybrid mode
#define Hybrid_Recharge	BIT1	// 1 Stop hybrid and recharge to hybrid start RSOC
#define Hybrid_State	BIT2	// 1 Hybrid turbo boost mode
#define Hybrid_Check	BIT3	// 1 AC insert wait for check RSOC to do hybrid or charge

//------------------------------------------------------------------------------
// Power On/Off Sequence Input Signals (0x0480 ~ 0x049D)
//------------------------------------------------------------------------------
extern XBYTE	FromSBsignals00;// _at_ 0x0480	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//															// Normal Cappella Montevina
#define SLP_S3			BIT0	// SLP_S3# 1/0:Hi/Lo		// V      V        V
#define SLP_S4			BIT1	// SLP_S4# 1/0:Hi/Lo		// V      V        V
#define SLP_S5			BIT2	// SLP_S5# 1/0:Hi/Lo		// X      V        V
#define SLP_M			BIT3	// SLP_M# 1/0:Hi/Lo			// X      V        V
#define SUS_PWR_DN_ACK	BIT4	// SUS_PWR_DN_ACK 1/0:Hi/Lo	// X      V        V
#define SLP_LAN			BIT5	// SLP_LAN# 1/0:Hi/Lo		// X      V        X
#define WOL_EN			BIT6	// WOL_EN 1/0:Hi/Lo			// X      X        V
#define S4_STATE		BIT7	// S4_STATE# 1/0:Hi/Lo		// X      X        V

extern XBYTE	FromSBsignals01;// _at_ 0x0481	(byte)
extern XBYTE	FromSBsignals02;// _at_ 0x0482	(byte)

extern XBYTE	FromMBsignals00;// _at_ 0x0483	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//															// Normal Cappella Montevina
#define HWPG			BIT0	// HWPG 1/0:Hi/Lo			// V      V        V
#define GFXPG			BIT1
#define PLTRST_DIS		BIT2

//extern XBYTE					// _at_ 0x0484	(byte)
//extern XBYTE					// _at_ 0x0485	(byte)

extern XBYTE	FromSBsignals00_BK0;	// _at_ 0x0486	(byte)
extern XBYTE	FromSBsignals00_BK1;	// _at_ 0x0487	(byte)
extern XBYTE	FromSBsignals00_BK2;	// _at_ 0x0488	(byte)
extern XBYTE	FromMBsignals00_BK0;	// _at_ 0x0489	(byte)
extern XBYTE	FromMBsignals00_BK1;	// _at_ 0x048A	(byte)
extern XBYTE	FromMBsignals00_BK2;	// _at_ 0x048B	(byte)
extern XBYTE	FromSBsignals01_BK0;	// _at_ 0x048C	(byte)
extern XBYTE	FromSBsignals01_BK1;	// _at_ 0x048D	(byte)
extern XBYTE	FromSBsignals01_BK2;	// _at_ 0x048E	(byte)
//extern XBYTE							// _at_ 0x048F	(byte)
//extern XBYTE							// _at_ 0x0490	(byte)
//extern XBYTE							// _at_ 0x0491	(byte)
extern XBYTE	FromSBsignals02_BK0;	// _at_ 0x0492	(byte)
extern XBYTE	FromSBsignals02_BK1;	// _at_ 0x0493	(byte)
extern XBYTE	FromSBsignals02_BK2;	// _at_ 0x0494	(byte)
//extern XBYTE							// _at_ 0x0495	(byte)
extern XBYTE	PECI_ME_Sequence_Num_BK;// _at_ 0x0496	(byte) PECI ME Sequence Number Back Up	// OEM_Calpella
extern XBYTE	PECI_ME_HandUp_ErrorCnt;// _at_ 0x0497	(byte) PECI ME HandUp ErrorCnt			// OEM_Calpella
extern XBYTE	FromSBsignals00_CNT;	// _at_ 0x0498	(byte)
extern XBYTE	FromSBsignals01_CNT;	// _at_ 0x0499	(byte)
extern XBYTE	FromSBsignals02_CNT;	// _at_ 0x049A	(byte)
extern XBYTE	FromMBsignals00_CNT;	// _at_ 0x049B	(byte)
extern XBYTE	PCH_RemoteErrorCnt;		// _at_ 0x049C	(byte) CPU Remote Error Counter	// OEM_Calpella
extern XBYTE	PECI_RemoteErrorCnt;	// _at_ 0x049D	(byte) VGA Remote Error Counter	// OEM_Calpella

//------------------------------------------------------------------------------
// Power On/Off Sequence Output Signals (0x049E ~ 0x04A6)
//------------------------------------------------------------------------------
extern XBYTE	ToSBsignals00;	// _at_ 0x049E	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//												// Normal Cappella Montevina
#define SB_PowerSource	BIT0	// 1/0:Hi/Lo	// V      V        V
#define ECPWROK			BIT1	// 1/0:Hi/Lo	// V      V        V
#define AC_Present		BIT2	// 1/0:Hi/Lo	// X      V        V
#define SLP_M_On		BIT3	// 1/0:Hi/Lo	// X      V        V
#define InterLANPower	BIT4	// 1/0:Hi/Lo	// V      V        V

extern XBYTE	ToSBsignals01;	// _at_ 0x049F	(byte)
extern XBYTE	ToSBsignals02;	// _at_ 0x04A0	(byte)
extern XBYTE	ToSBsignals03;	// _at_ 0x04A1	(byte)

extern XBYTE	ToMBsignals00;	// _at_ 0x04A2	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//												// Normal Cappella Montevina
#define SUSON			BIT0	// 1/0:Hi/Lo	// V      V        V
#define MAINON			BIT1	// 1/0:Hi/Lo	// V      V        V
#define VRON			BIT2	// 1/0:Hi/Lo	// V      V        V

extern XBYTE	ToMBsignals01;	// _at_ 0x04A3	(byte)
extern XBYTE	ToMBsignals02;	// _at_ 0x04A4	(byte)
extern XBYTE	ToMBsignals03;	// _at_ 0x04A5	(byte)
extern XBYTE	ToMBsignals04;	// _at_ 0x04A6	(byte)

#ifdef OEM_FAN
extern XBYTE	FanControl_Mode00_Bk;	// _at_ 0x04A7	(byte)
extern XBYTE	CPU_Remote_Temp_BackUp;	// _at_ 0x04A8	(byte)
extern XBYTE	CPU_Local_Temp_BackUp;	// _at_ 0x04A9	(byte)
extern XBYTE	VGA_Remote_Temp_BackUp;	// _at_ 0x04AA	(byte)
extern XBYTE	VGA_Local_Temp_BackUp;	// _at_ 0x04AB	(byte)
//extern XBYTE							// _at_ 0x04AC	(byte)
//extern XBYTE							// _at_ 0x04AD	(byte)
extern XWORD	CPU_Remote_RPM_Value;	// _at_ 0x04AE	(word)
extern XWORD	CPU_Local_RPM_Value;	// _at_ 0x04B0	(word)
extern XWORD	VGA_Remote_RPM_Value;	// _at_ 0x04B2	(word)
extern XWORD	VGA_Local_RPM_Value;	// _at_ 0x04B4	(word)
extern XWORD	Expect_RPM_Value2;		// _at_ 0x04B6	(word)					// leox_20120322 Support_FAN2
extern XWORD	Expect_RPM_Value;		// _at_ 0x04B8	(word)
extern XBYTE	CPU_Remote_DAPWM_Value;	// _at_ 0x04BA	(byte)
extern XBYTE	CPU_Local_DAPWM_Value;	// _at_ 0x04BB	(byte)
extern XBYTE	VGA_Remote_DAPWM_Value;	// _at_ 0x04BC	(byte)
extern XBYTE	VGA_Local_DAPWM_Value;	// _at_ 0x04BD	(byte)
#endif	// OEM_FAN	// leox_20111004 Move up

//extern XBYTE							// _at_ 0x04BE	(byte)

extern XBYTE	FanControl_Mode00;		// _at_ 0x04BF	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define CPU_TYPE_TJ85			BIT0	// 1 CPU TYPE is TJ85
#define CPU_TYPE_TJ90			BIT1	// 1 CPU TYPE is TJ90
#define CPU_TYPE_TJ100			BIT2	// 1 CPU TYPE is TJ100
#define CPU_TYPE_TJ105			BIT3	// 1 CPU TYPE is TJ105
//#define						BIT4
//#define						BIT5
#define InitThermalSenFinish	BIT6	// 1/0 Inition Thermal Sensor Finish/Not Finish
#define Thrott_CPU_hot			BIT7	// Throttling enabled due to CPU hot	// leox20150731

//extern XBYTE							// _at_ 0x04C0	(byte)
//extern XBYTE							// _at_ 0x04C1	(byte)

extern XBYTE	FanControl_Mode01;		// _at_ 0x04C2	(byte)					// OEM_FAN
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define EnFanCtrlCPURempte		BIT0	// 1 Enable Fan Contrl by CPU Remote
#define EnFanCtrlVGARempte		BIT1	// 1 Enable Fan Contrl by VGA Remote
#define EnFanCtrlCPULocal		BIT2	// 1 Enable Fan Contrl by CPU Local
#define EnFanCtrlVGALocal		BIT3	// 1 Enable Fan Contrl by VGA Local
#define EnFanCtrlCPURempteCHG	BIT4	// 1 Enable Fan Contrl by CPU Remote Charging mode
#define EnFanCtrlVGARempteCHG	BIT5	// 1 Enable Fan Contrl by VGA Remote Charging mode
#define EnFanCtrlCPULocalCHG	BIT6	// 1 Enable Fan Contrl by CPU Local Charging mode
#define EnFanCtrlVGALocalCHG	BIT7	// 1 Enable Fan Contrl by VGA Local Charging mode

extern XBYTE	FanControl_Mode01_Bk;	// _at_ 0x04C3	(byte)					// OEM_FAN
extern XBYTE	FanSpeedOnSec;			// _at_ 0x04C4	(byte)
extern XBYTE	FanSpeedOnSec_256;		// _at_ 0x04C5	(byte)
extern XBYTE	FanSpeedOffSec;			// _at_ 0x04C6	(byte)
extern XBYTE	FanSpeedOffSec_256;		// _at_ 0x04C7	(byte)

// leox20150121 For check EC version +++	// leox20150522 From HP N61A
extern XBYTE	VerY;			// _at_ 0x04C8	(byte) EC version: Year
extern XBYTE	VerM;			// _at_ 0x04C9	(byte) EC version: Month
extern XBYTE	VerD;			// _at_ 0x04CA	(byte) EC version: Day
extern XBYTE	VerB;			// _at_ 0x04CB	(byte) EC version: Build
// leox20150121 For check EC version ---

//extern XBYTE					// _at_ 0x04CC	(byte)
extern XBYTE	DIMM_LED_Status;// _at_ 0x04CD	(byte)
extern XBYTE	ThreeG_En_CNT;	// _at_ 0x04CE	(byte) Delay from MAINON to 3G_EN Counter
extern XBYTE	DLIDCNT;		// _at_ 0x04CF	(byte) Delay LID# Counter		// DLID_Function


extern XBYTE	Factory_Test_Status;// _at_ 0x04D0	(byte)	//David_20141222 add for factory test
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define LID_Dis				BIT0	// 1/0 LID function disable/enable

extern XBYTE	TouchpadSWFnFlag;	// _at_ 0x04D1	(byte) Touchpad Switch Function Flag	// OEM_TP
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define EnableTouchpadSWFn	BIT0	// 1/0 Enable/Disable Touchpad Switch Function
#define OemTouchpadEnable	BIT1	// 1/0 OEM Touchpad Enable/Disable

extern XBYTE	TouchpadSWFnDebounce;	// _at_ 0x04D2	(byte) Touchpad Switch Function Debounce	// OEM_TP
extern XBYTE	ThermalSnrClrALERTCNT;	// _at_ 0x04D3	(byte) Thermal Sensor Clear ALERT counter
extern XBYTE	ISL6251ACHICWaitVinCNT;	// _at_ 0x04D4	(byte) For wait Vin ready then Change CHG- signal counter

extern XBYTE	BatteryOnlyXXPercent;	// _at_ 0x04D5	(byte)					// BatteryOnlyXXPThrollFUN
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Q40Q41_SCI			BIT0	// 1/0 Battery Only below 20% On/Off

extern XWORD	ChargingCurrentSMBus;		// _at_ 0x04D6	(word) Battery charging current from SMBus	// BattChg_SMBtoTBLtoDAC
extern XBYTE	ChkChg_I_TBL_CNT;			// _at_ 0x04D8	(byte)				// BattChg_SMBtoTBLtoDAC
//extern XBYTE								// _at_ 0x04D9	(byte)
extern XBYTE	ChkTurnOffSBLANPwrFS345CNT;	// _at_ 0x04DA	(byte)				// OEM_Calpella
extern XWORD	ATRICKLE_DUTY_ConstCurrt;	// _at_ 0x04DB	(word) First battery trickle charge duty low byte
extern XBYTE	USBChg_Status;				// _at_ 0x04DD	(byte)				// leo_20150917 USB charging

#ifdef SUPPORT_BQ24780S						// leox20150915 Add for set/clear LEARN mode of charger IC
extern XBYTE	LearnModeSetCnt;			// _at_ 0x04DE	(byte)
extern XBYTE	LearnModeClrCnt;			// _at_ 0x04DF	(byte)
#endif

extern XBYTE	Fan1_Fault_CNT;				// _at_ 0x04E0	(byte)	// David_20120613 add for fan error check	// CHECK_FAN_FAULT

#ifdef OEM_IPOD
extern XBYTE	USB_CHARGE_CNT;	// _at_ 0x04E1	(byte)
extern XBYTE	USB_CB0_CNT;	// _at_ 0x04E2	(byte)
extern XBYTE	USBWorkingCNT;	// _at_ 0x04E3	(byte) USB Charging/Discharging port Working Count
extern XBYTE	DelayChkIdleCNT;// _at_ 0x04E4	(byte) Delay Check IDLE Counter

extern XBYTE	USBCHARGFLAG;	// _at_ 0x04E5	(byte) USB Charging/Discharging port Function Flags
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USBCHARGPORTENABLE		BIT0	// USB Charging/Discharging port Enable Flag
#define ChkUSBchgFunBeforeIdle	BIT1	// 1 Already check USB Charging Function Before Enter Idle mode
#endif	// OEM_IPOD						// 0 Need check USB Charging Function Before Enter Idle mode

extern XBYTE	OverVoltageCNT;	// _at_ 0x04E6	(byte) Debounce for Battery Over Voltage

extern XBYTE	SMBusAbnormalDATlowCNT;		// _at_ 0x04E7	(byte) SMBus Data Line Abnormal Low Counter
extern XBYTE	DoPullSMBusCLKlowPulseCNT;	// _at_ 0x04E8	(byte) Do PullSMBusCLKlowPulse Counter

extern XBYTE	CMOSReset_Status;	// _at_ 0x04E9	(byte)	//David_20141222 add for CMOS clear function
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define WaitSUSBSUSC	BIT0	// 1 Wait SUSB/SUSC on
#define WaitPCH			BIT1	// 1 Wait PCH on
#define ClearCMOS		BIT2	// 1 Need clear CMOS

extern XBYTE	CMOSResetCNT;		// _at_ 0x04EA	(byte)	//David_20141222 add for CMOS clear function

extern XBYTE	SYS_MISC2;		// _at_ 0x04EB	(byte) System misc flags
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define postpone_SCI	BIT0	// 1 Issue SCI later
#define postpone_SMI	BIT1	// 1 Issue SMI later
#define InRestart		BIT2	// 1 EC is restarting power on sequence
//#define				BIT3
#define CrisisCheck		BIT4	// 1 Crisis key check enable
#define AcMode			BIT5	// 1 AC mode, 0 DC mode
#define CHGpinHigh		BIT6	// 1/0 CHG# Status Hi/Lo
#define LaunchSetupMenu	BIT7	// 1 Launch BIOS setup menu		// leox20150612 SIT_BIOS_SETUP_MENU

extern XBYTE	OLD_SELSTATE1;	// _at_ 0x04EC	(byte) Previous selector state
extern XBYTE	OLD_SELSTATE2;	// _at_ 0x04ED	(byte) Previous selector state

extern XBYTE	SYSTEMSTATE;	// _at_ 0x04EE	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 00h: First power up
// 02h: S0
// 03h: S3
// 04h: S4/S5 and have S.B. power source
// 05h: S4/S5 and no S.B. power source
// 13h: Deep S3		// 20120104 Chris add for support Deep S3 DeepS3_SUPPORT
// 31h: S3 to S0 but not yet send PowerOK to S.B.
// 41h: S4/S5 to S0 but not yet send PowerOK to S.B.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	LedBackup;		// _at_ 0x04EF	(byte)

//------------------------------------------------------------------------------
// EC Debug Using RAM (0x04F0 ~ 0x04FF)
//------------------------------------------------------------------------------
extern XBYTE	DBG0;			// _at_ 0x04F0	(byte)	// leox_20110926
extern XBYTE	DBG1;			// _at_ 0x04F1	(byte)
extern XBYTE	DBG2;			// _at_ 0x04F2	(byte)
extern XBYTE	DBG3;			// _at_ 0x04F3	(byte)
extern XBYTE	DBG4;			// _at_ 0x04F4	(byte)
extern XBYTE	DBG5;			// _at_ 0x04F5	(byte)
extern XBYTE	DBG6;			// _at_ 0x04F6	(byte)
extern XBYTE	DBG7;			// _at_ 0x04F7	(byte)
extern XBYTE	DBG8;			// _at_ 0x04F8	(byte)
extern XBYTE	DBG9;			// _at_ 0x04F9	(byte)
extern XBYTE	DBG10;			// _at_ 0x04FA	(byte)
extern XBYTE	DBG11;			// _at_ 0x04FB	(byte)
extern XBYTE	DBG12;			// _at_ 0x04FC	(byte)
extern XBYTE	DBG13;			// _at_ 0x04FD	(byte)
extern XBYTE	KeyECScanCode;	// _at_ 0x04FE	(byte)
extern XBYTE	KeyPosition;	// _at_ 0x04FF	(byte)


//******************************************************************************
// 0x500-0x5FF  OEM RAM 5
//******************************************************************************
// For ITE flash utility, EC SPI rom read write function, and/or Ramdebug function.


//******************************************************************************
// 0x600-0x6FF  OEM RAM 6
//******************************************************************************
// For Keil C complier and ram code space (For SPI programming)


//******************************************************************************
// 0x700-0x7FF  OEM RAM 7
//******************************************************************************
// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function ++
extern XBYTE	IER0_BK;		// _at_ 0x0700	(byte)
extern XBYTE	IER1_BK;		// _at_ 0x0701	(byte)
extern XBYTE	IER2_BK;		// _at_ 0x0702	(byte)
extern XBYTE	IER3_BK;		// _at_ 0x0703	(byte)
// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function --

// leox_20111206 Reset EC later after flash +++		// RESET_EC_LATER
extern XBYTE	RstLaterMark0;	// _at_ 0x0704	(byte) Reset later mark 0
extern XBYTE	RstLaterMark1;	// _at_ 0x0705	(byte) Reset later mark 1
// leox_20111206 Reset EC later after flash ---

//extern XBYTE					// _at_ 0x0706	(byte)
//extern XBYTE					// _at_ 0x0707	(byte)
//extern XBYTE					// _at_ 0x0708	(byte)
//extern XBYTE					// _at_ 0x0709	(byte)
//extern XBYTE					// _at_ 0x070A	(byte)
//extern XBYTE					// _at_ 0x070B	(byte)
//extern XBYTE					// _at_ 0x070C	(byte)
//extern XBYTE					// _at_ 0x070D	(byte)
//extern XBYTE					// _at_ 0x070E	(byte)
//extern XBYTE					// _at_ 0x070F	(byte)

// leo_20150916 add power protect +++			// BAT_MBATV_PROTECT
extern XWORD	AC_OC_En_ProchotCnt;			// _at_ 0x0710	(word)
extern XWORD	AC_OC_Dis_ProchotCnt;			// _at_ 0x0712	(word)
extern XWORD	AC_OCP_En_ProchotCnt;			// _at_ 0x0714	(word)
extern XWORD	AC_OCP_Dis_ProchotCnt;			// _at_ 0x0716	(word)
extern XWORD	DC_OW_En_ProchotCnt;			// _at_ 0x0718	(word)
extern XWORD	DC_OW_Dis_ProchotCnt;			// _at_ 0x071A	(word)
extern XWORD	DC_OCP_En_ProchotCnt;			// _at_ 0x071C	(word)
extern XWORD	DC_OCP_Dis_ProchotCnt;			// _at_ 0x071E	(word)
extern LWORD	Bat_OverWattThreshold;			// _at_ 0x0720	(dword)
extern LWORD	Bat_UnderWattThreshold;			// _at_ 0x0724	(dword)
extern XWORD	Bat_OverPeakCurrentThreshold;	// _at_ 0x0728	(word)
extern XWORD	Bat_UnderPeakCurrentThreshold;	// _at_ 0x072A	(word)
extern LWORD	L_DC_W;							// _at_ 0x072C	(dword)
extern LWORD	L_VMBATV_V;						// _at_ 0x0730	(dword)
extern LWORD	L_VICM_V;						// _at_ 0x0734	(dword)
//extern XBYTE					// _at_ 0x0730	(byte)
//extern XBYTE					// _at_ 0x0731	(byte)
//extern XBYTE					// _at_ 0x0732	(byte)
//extern XBYTE					// _at_ 0x0733	(byte)
//extern XBYTE					// _at_ 0x0734	(byte)
//extern XBYTE					// _at_ 0x0735	(byte)
//extern XBYTE					// _at_ 0x0736	(byte)
//extern XBYTE					// _at_ 0x0737	(byte)
// leo_20150916 add power protect ---

//extern XBYTE					// _at_ 0x0738	(byte)
//extern XBYTE					// _at_ 0x0739	(byte)

#ifdef Multiple_TP	// ken add for touchpad compare
extern XBYTE	DetectTPID_Ctrl;	// _at_ 0x073A	(byte) Detect touchpad ID command: Control
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 00: Disable detect touchpad ID function
// 01: Enable  detect touchpad ID function
// 02: Get LastDetectTPID_Cmd ready
// 03: Get DetectTPID0 ready
// 04: Get DetectTPID1 ready
// 05: Get DetectTPID2 ready
// 06: Get DetectTPID3 ready
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	LastDetectTPID_Cmd;	// _at_ 0x073B	(byte) Detect touchpad ID command: Last command
extern XBYTE	DetectTPID0;		// _at_ 0x073C	(byte) Detect touchpad ID command: ID0
extern XBYTE	DetectTPID1;		// _at_ 0x073D	(byte) Detect touchpad ID command: ID1
extern XBYTE	DetectTPID2;		// _at_ 0x073E	(byte) Detect touchpad ID command: ID2
extern XBYTE	DetectTPID3;		// _at_ 0x073F	(byte) Detect touchpad ID command: ID3
#endif	// Multiple_TP

//extern XBYTE					// _at_ 0x0740	(byte)
//extern XBYTE					// _at_ 0x0741	(byte)
//extern XBYTE					// _at_ 0x0742	(byte)
//extern XBYTE					// _at_ 0x0743	(byte)
//extern XBYTE					// _at_ 0x0744	(byte)
//extern XBYTE					// _at_ 0x0745	(byte)
//extern XBYTE					// _at_ 0x0746	(byte)
//extern XBYTE					// _at_ 0x0747	(byte)
//extern XBYTE					// _at_ 0x0748	(byte)
//extern XBYTE					// _at_ 0x0749	(byte)
//extern XBYTE					// _at_ 0x074A	(byte)
//extern XBYTE					// _at_ 0x074B	(byte)
//extern XBYTE					// _at_ 0x074C	(byte)
//extern XBYTE					// _at_ 0x074D	(byte)
//extern XBYTE					// _at_ 0x074E	(byte)
//extern XBYTE					// _at_ 0x074F	(byte)

//------------------------------------------------------------------------------
// Primary Battery Information Namespaces (0x0750 ~ 0x075F)
//------------------------------------------------------------------------------
extern XBYTE	ABAT_MODEL;		// _at_ 0x0750	(byte) First battery model

extern XBYTE	ABAT_STATUS;	// _at_ 0x0751	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define abat_in			BIT0	// 1 First battery exist
#define abat_full		BIT1	// 1 First battery fully charged
#define abat_destroy	BIT2	// 1 First battery mulfunction
#define abat_trickle	BIT3	// 1 Process first battery trickle charge
#define abat_RTalarm	BIT4	// 1 Remaining time alarm
#define abat_low		BIT5	// 1 Battery low
#define abat_dischg		BIT6	// 1 First battery in discharging
#define abat_dead		BIT7	// 1 First battery fully discharged

extern XBYTE	ACHARGE_STATUS;	// _at_ 0x0752	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define abat_charge		BIT0	// 1 First battery in charge
#define abat_prechg		BIT1	// 1 First battery in precharge
#define abat_overtemp	BIT2	// 1 First battery over heat
#define abat_NiMH		BIT3	// 1 Ni-MH, 0 LI-Ion
#define abat_wakeup		BIT4	// 1 Battery gauge working
#define abat_InCharge	BIT5	// 1 Battery in any charge phase
#define abat_WakeChg	BIT6	// 1 Battery in wakeup charge phase
#define abat_level3		BIT7	// 1 ABAT connect to chgr/discghr

extern XBYTE	ABAT_MISC;		// _at_ 0x0753	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define force_chgA		BIT0	// 1 Charge first battery by host request
#define force_discA		BIT1	// 1 Discharge first battery by host request
#define abat_StopChg	BIT2	// 1 Stop charging								// NECP_BattRefresh_Support
#define abat_next		BIT3	// 1 Next use first battery
#define abat_inUSE		BIT4	// 1 First battery connected to SMB
#define abat_AutoLearn	BIT5	// 1 Auto learning in progress
#define abat_ValidFull	BIT6	// 1 Battery real full
#define abat_LiHi		BIT7	// 1 High capacity Li-Ion battery

extern XBYTE	ABAT_MISC2;		// _at_ 0x0754	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define bat_HiVolt		BIT0	// 1 require high charge voltage (4 Cells Li-Ion battery)
#define bat_IntCC		BIT1	// 1 charge control inside battery
#define MBATModeChg		BIT2	// MBAT mode change success FENNY NE2P1A41 001215
#define ForceMBATDischg	BIT3	// Force MBAT discharge flag FENNY NE2P1A31 001207
#define ForceMBATCharge	BIT4	// Force MABT charge flag FENNY NE2P1A31 001207
#define abat_test		BIT5	// for battery charge/discharge command test
#define Shipmode		BIT6	// 1 Prepare enter ship mode
#define Bat_Fcc_test	BIT7	// For BIOS setup menu battery status test

extern XWORD	ABAT_ALARM;		// _at_ 0x0755	(word) Smart battery alarm and status
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SMBerror0		BIT0	// Error code bit0
#define SMBerror1		BIT1	// Error code bit1
#define SMBerror2		BIT2	// Error code bit2
#define SMBerror3		BIT3	// Error code bit3
#define FullyDsg		BIT4	// Statusfully discharged
#define FullyChg		BIT5	// Statusfully charged
#define Dsg				BIT6	// Statusdischarging
#define Initialized		BIT7	// Statusgauge initialized
#define RemTime_alarm	BIT8	// Alarmremaining time
#define RemCap_alarm	BIT9	// Alarmremaining capacity
//#define				BITA
#define TermDsg_alarm	BITB	// Alarmterminate discharge
#define OverTemp_alarm	BITC	// Alarmover temperature
//#define				BITD
#define TermChg_alarm	BITE	// Alarmterminate charge
#define OverChg_alarm	BITF	// Alarmover charge

extern XBYTE	ABAT_CAP;		// _at_ 0x0757	(byte) ABAT batt capacity (RSOC %)
extern XWORD	ABAT_VOLT;		// _at_ 0x0758	(word) Volatge of aux battery
extern XWORD	ABAT_AV;		// _at_ 0x075A	(word) ABAT voltage accumulator
extern XWORD	ABAT_TEMP;		// _at_ 0x075C	(word) Temperature of aux battery
extern XWORD	ABAT_AT;		// _at_ 0x075E	(word) ABAT temp accumulator

//------------------------------------------------------------------------------
// Secondary Battery Information Namespaces (0x0760 ~ 0x076F)
//------------------------------------------------------------------------------
extern XBYTE	BBAT_MODEL;		// _at_ 0x0760	(byte) Second battery model

extern XBYTE	BBAT_STATUS;	// _at_ 0x0761	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define bbat_in			BIT0	// 1 Second battery exist
#define bbat_full		BIT1	// 1 Second battery fully charged
#define bbat_destroy	BIT2	// 1 Second battery mulfunction
#define bbat_trickle	BIT3	// 1 Process second battery trickle charge
#define bbat_RTalarm	BIT4	// 1 Remaining time alarm
#define bbat_low		BIT5	// 1 Battery low
#define bbat_dischg		BIT6	// 1 Second battery in discharging
#define bbat_dead		BIT7	// 1 Second battery fully discharged

extern XBYTE	BCHARGE_STATUS;	// _at_ 0x0762	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define bbat_charge		BIT0	// 1 Second battery in charge
#define bbat_prechg		BIT1	// 1 Second battery in precharge
#define bbat_overtemp	BIT2	// 1 Second battery over heat
#define bbat_NiMH		BIT3	// 1 Ni-MH 0 LI-Ion
#define bbat_wakeup		BIT4	// 1 Battery gauge working
#define bbat_InCharge	BIT5	// 1 Battery in any charge phase
#define bbat_WakeChg	BIT6	// 1 Battery in wakeup charge phase
#define bbat_level3		BIT7	// 1 BBAT connect to chgr/discghr

extern XBYTE	BBAT_MISC;		// _at_ 0x0763	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define force_chgB		BIT0	// 1 Charge second battery by host request
#define force_discB		BIT1	// 1 Discharge second battery by host request
//#define				BIT2
#define bbat_next		BIT3	// 1 Next use second battery
#define bbat_inUSE		BIT4	// 1 Second battery connected to SMB
#define bbat_AutoLearn	BIT5	// 1 Auto learning in progress
#define bbat_ValidFull	BIT6	// 1 Battery real full
#define bbat_LiHi		BIT7	// 1 High capacity Li-Ion battery

extern XBYTE	BBAT_MISC2;		// _at_ 0x0764	(byte)
extern XWORD	BBAT_ALARM;		// _at_ 0x0765	(word) Smart battery alarm and status
extern XBYTE	BBAT_CAP;		// _at_ 0x0767	(byte) BBAT batt capacity (RSOC %)
extern XWORD	BBAT_VOLT;		// _at_ 0x0768	(word) Volatge of aux battery
extern XWORD	BBAT_AV;		// _at_ 0x076A	(word) BBAT voltage accumulator
extern XWORD	BBAT_TEMP;		// _at_ 0x076C	(word) Temperature of aux battery
extern XWORD	BBAT_AT;		// _at_ 0x076E	(word) BBAT temp accumulator

//------------------------------------------------------------------------------
// ADC Data Value RAM (0x0770 ~ 0x077F)
//------------------------------------------------------------------------------
extern XWORD	ADV_DAT0;		// _at_ 0x0770	(word) Result of A/D channel 0
extern XWORD	ADV_DAT1;		// _at_ 0x0772	(word) Result of A/D channel 1
extern XWORD	ADV_DAT2;		// _at_ 0x0774	(word) Result of A/D channel 2
extern XWORD	ADV_DAT3;		// _at_ 0x0776	(word) Result of A/D channel 3
extern XWORD	ADV_DAT4;		// _at_ 0x0778	(word) Result of A/D channel 4
extern XWORD	ADV_DAT5;		// _at_ 0x077A	(word) Result of A/D channel 5
extern XWORD	ADV_DAT6;		// _at_ 0x077C	(word) Result of A/D channel 6
extern XWORD	ADV_DAT7;		// _at_ 0x077E	(word) Result of A/D channel 7

//------------------------------------------------------------------------------
// Ext Vector Bounce (0x0780 ~ 0x078F)
//------------------------------------------------------------------------------
extern XBYTE	EXTEVT_BOUNCE;	// _at_ 0x0780	(byte) Ext event debounce counter

//------------------------------------------------------------------------------
// ABAT Shadow Information (0x0790 ~ 0x079A)
//------------------------------------------------------------------------------
extern XBYTE	AAUTO_STEP;		// _at_ 0x0790	(byte) ABAT auto learning step control

extern XBYTE	AAUTO_STS;		// _at_ 0x0791	(byte) ABAT auto learning status
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 0: No error
// 1: Battery not exist
// 2: Battery abnormal
// 3: System power off
// 4: SelectorState() command in
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	AFAIL_CAUSE;	// _at_ 0x0792	(byte) ABAT failure cause code
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 0; Normal
// 1; Unable to get model
// 2; Precharge fail
// 4; Gauge malfunction
// 6; Over voltage
// 8; TimerProtect
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	AAVERAGE_STEP;	// _at_ 0x0793	(byte) ADC average control of ABAT
//extern XBYTE					// _at_ 0x0794	(byte)
extern XBYTE	ABAT_FAIL_TRY;	// _at_ 0x0795	(byte) Counter for retry get model
extern XWORD	ABLED1_PAT;		// _at_ 0x0796	(word) ABAT LED1 flash pattern
extern XWORD	ABLED2_PAT;		// _at_ 0x0798	(word) ABAT LED2 flash pattern
extern XBYTE	ABAT_BOUNCE;	// _at_ 0x079A	(byte) Debounce timer for first battery

//------------------------------------------------------------------------------
// BBAT Shadow Information (0x079B ~ 0x07A5)
//------------------------------------------------------------------------------
extern XBYTE	BAUTO_STEP;		// _at_ 0x079B	(byte) BBAT auto learning step control
extern XBYTE	BAUTO_STS;		// _at_ 0x079C	(byte) BBAT auto learning status
extern XBYTE	BFAIL_CAUSE;	// _at_ 0x079D	(byte) BBAT failure cause code
extern XBYTE	BAVERAGE_STEP;	// _at_ 0x079E	(byte) ADC average control of BBAT
extern XBYTE	BTRICKLE_DUTY;	// _at_ 0x079F	(byte) Second battery trickle charge duty
extern XBYTE	BBAT_FAIL_TRY;	// _at_ 0x07A0	(byte) Counter for retry get model
extern XWORD	BBLED1_PAT;		// _at_ 0x07A1	(word) BBAT LED1 flash pattern
extern XWORD	BBLED2_PAT;		// _at_ 0x07A3	(word) BBAT LED2 flash pattern
extern XBYTE	BBAT_BOUNCE;	// _at_ 0x07A5	(byte) Debounce timer for second battery

//------------------------------------------------------------------------------
// Battery Protect Information (0x07A6 ~ 0x07AF)
//------------------------------------------------------------------------------
extern XBYTE	OVERTEMP_COUNT;		// _at_ 0x07A6	(byte) Counter for battery over temperature
extern XBYTE	Bat_FastChargeCURT;	// _at_ 0x07A7	(byte)
extern XWORD	CHG_TIMER;			// _at_ 0x07A8	(word) Wakeup charge timer

//extern XWORD	WAKECHG_TIMER;	// _at_ 0x07A8	(word) Wakeup charge timer
//extern XWORD	PRECHG_TIMER;	// _at_ 0x07A8	(word) Timer for battery trickle charge
//extern XWORD	CHARGE_TIMER	// _at_ 0x07A8	(word) Timer for battery normal charge

extern XBYTE	BAT_DEAD_CNT;	// _at_ 0x07AA	(byte) Counter for BAT dead qualify
extern XBYTE	FASTCHG_MASK;	// _at_ 0x07AB	(byte) Detection mask of fast charge

extern XBYTE	SLOWCHG_MASK;	// _at_ 0x07AC	(byte) Detection mask of slow charge(1 detection enable)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define StopBy_timer	BIT0	// Timer protect
#define StopBy_overV	BIT1	// Over voltage protect
#define StopBy_overT	BIT2	// Over temperature protect
#define StopBy_Imin		BIT3	// I minimum detect
#define StopBy_DTdt		BIT4	// DT/dt detect
#define StopBy_peak		BIT5	// Peak voltage detect
#define StopBy_MDV		BIT6	// -DV detect
#define StopBy_cap		BIT7	// Capacity full

extern XBYTE	CUTOFF_CAUSE;	// _at_ 0x07AD	(byte) Charger cut-off cause
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  3: Bat_overtemp
//  4: Li OverChg_alarm or FullyChg
//  8: Full RSOC
// 10: FullyChg
// 12: Ni OverChg alarm or FullyChg

extern XBYTE	POLLING_JOB;	// _at_ 0x07AE	(byte) SMB read buffer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define step_RSOC				BIT0	// 1 Read RSOC
#define step_current			BIT1	// 1 Read current
#define step_ChargingCurrent	BIT2	// 1 Read charging current
#define step_ChargingVoltage	BIT3	// 1 Read charging voltage
#define step_BatStatus			BIT4	// 1 Read status

extern XBYTE	BAT_MISC3;		// _at_ 0x07AF	(byte) Flags for battery manage
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define NoProcess_bat		BIT0	// 1 Don't handle battery
#define charger_on			BIT1	// 1 Charger turned on
#define DCHG_Throttling		BIT2	// 1 Throttling for battery protect			// leox20150826 BAT_DCHG_PROTECT
//#define					BIT3
#define BQ24780S_Set_Reg	BIT4	// 1 BQ24780S need to modify register by PN	// leox20150907 SUPPORT_BQ24780S
#define VGA_WriSensorError	BIT5	// 1 VGA Sensor Error 0 normal
#define CPU_WriSensorError	BIT6	// 1 CPU Sensor Error 0 normal
#define BQ24780S_Init_OK	BIT7	// 1 BQ24780S initial ok					// leox20150824 SUPPORT_BQ24780S

extern XWORD	Bat_PreChargeTimeout;		// _at_ 0x07B0	(word)
//extern XBYTE	Bat_PreChargeCURT;			// _at_ 0x07B2	(byte)
extern XBYTE	Bat_PowerOnTimerRatio;		// _at_ 0x07B3	(byte)
extern XBYTE	Bat_PowerOffTimerRatio;		// _at_ 0x07B4	(byte)
extern XWORD	Bat_ChargeTimeout;			// _at_ 0x07B5	(word)
extern XWORD	Bat_OverVoltThreshold;		// _at_ 0x07B7	(word)
extern XWORD	Bat_PreDeadChargeThreshold;	// _at_ 0x07B9	(word)

//David_20141222 add for charge IC ISL88732 charge data backup
extern XWORD	ChargingVoltage_Backup;		// _at_ 0x07BB	(word)
extern XWORD	ChargingCurrent_Backup;		// _at_ 0x07BD	(word)
//David_20141222 add for charge IC ISL88732 charge data backup

//extern XBYTE					// _at_ 0x07BF	(byte)

//------------------------------------------------------------------------------
// Reboot Mark for Command 64-B4 (0x07C0 ~ 0x07C4)
//------------------------------------------------------------------------------
extern XBYTE	REBOOT_MARK;	// _at_ 0x07C0	(byte)
extern XBYTE	REBOOT_MARK2;	// _at_ 0x07C1	(byte)
extern XBYTE	REBOOT_MARK3;	// _at_ 0x07C2	(byte)
extern XBYTE	REBOOT_MARK4;	// _at_ 0x07C3	(byte)
//extern XBYTE					// _at_ 0x07C4	(byte)

//------------------------------------------------------------------------------
// Normal Debounce (0x07C5 ~ 0x07C9)
//------------------------------------------------------------------------------
extern XBYTE	POLLING_STEP;	// _at_ 0x07C5	(byte) Counter for polling sequence contorl
extern XBYTE	PSW_BOUNCE;		// _at_ 0x07C6	(byte) Counter for PSW debounce
extern XBYTE	AC_BOUNCE;		// _at_ 0x07C7	(byte) Counter for AC debounce
extern XBYTE	OneButtDebounce;// _at_ 0x07C8	(byte) Counter for one touched button

extern XBYTE	BAT_MISC1;		// _at_ 0x07C9	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define				BIT0
//#define				BIT1
//#define				BIT2
//#define				BIT3
#define STOP_charge		BIT4	// 1 Do NOT charge battery
#define postponetrickle	BIT5	// 1 Trickle charge signal already init

//------------------------------------------------------------------------------
// Thermal Protect Information (0x07CA ~ 0x07CF)
//------------------------------------------------------------------------------
extern XBYTE	CPU_VeryHotCnt;			// _at_ 0x07CA	(byte) CPU Remote too hot counter
extern XBYTE	CPU_VeryHotCnt_Local;	// _at_ 0x07CB	(byte) CPU Remote too hot counter
extern XBYTE	VGA_VeryHotCnt;			// _at_ 0x07CC	(byte) VGA Remote too hot counter
extern XBYTE	VGA_VeryHotCnt_Local;	// _at_ 0x07CD	(byte) VGA Remote too hot counter
extern XBYTE	ABAT_CELLS;				// _at_ 0x07CE	(byte) Battery cells
extern XBYTE	BBAT_CELLS;				// _at_ 0x07CF	(byte) Battery cells

//------------------------------------------------------------------------------
// Battery Shadow RAM (0x07D0 ~ 0x07DF)
//------------------------------------------------------------------------------
extern XBYTE	SEL_BAT_MODEL;	// _at_ 0x07D0	(byte) selected battery model

extern XBYTE	SEL_BAT_STATUS;	// _at_ 0x07D1	(byte) selected battery status
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define bat_in			BIT0	// 1 Battery exist
#define bat_full		BIT1	// 1 Battery fully charged
#define bat_destroy		BIT2	// 1 Battery mulfunction
#define bat_trickle		BIT3	// 1 Process battery trickle charge
#define bat_RTalarm		BIT4	// 1 Remaining time alarm
#define bat_low			BIT5	// 1 Battery low
#define bat_dischg		BIT6	// 1 Battery in discharging
#define bat_dead		BIT7	// 1 Battery fully discharged

extern XBYTE	SEL_CHARGE_STATUS;	// _at_ 0x07D2	(byte) Battery charge status
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define bat_charge		BIT0	// 1 Battery in charge
#define bat_prechg		BIT1	// 1 Battery in precharge
#define bat_overtemp	BIT2	// 1 Battery over heat
#define bat_NiMH		BIT3	// 1 Ni-MH 0 LI-Ion
#define bat_wakeup		BIT4	// 1 Battery gauge working
#define bat_InCharge	BIT5	// 1 Battery in any charge phase
#define bat_WakeChg		BIT6	// 1 Battery in wakeup charge phase
#define bat_level3		BIT7	// 1 Batt connect to chgr/discghr

extern XBYTE	SEL_BAT_MISC;	// _at_ 0x07D3	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define bat_force_chg	BIT0	// 1 Charge battery by host request
#define bat_force_disc	BIT1	// 1 Discharge battery by host request
#define bat_StopChg		BIT2	// 1 Stop charging								// NECP_BattRefresh_Support
#define bat_next		BIT3	// 1 Next use battery
#define bat_inUSE		BIT4	// 1 Battery connected to SMB
#define bat_AutoLearn	BIT5	// 1 Auto learning in progress
#define bat_ValidFull	BIT6	// 1 Battery real full

extern XBYTE	SEL_BAT_MISC2;	// _at_ 0x07D4	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define bat_HiVolt		BIT0	// 1 Require high charge voltage
#define bat_IntCC		BIT1	// 1 Charge control inside battery

extern XWORD	SEL_BAT_ALARM;	// _at_ 0x07D5	(word) Smart battery alarm and status
extern XBYTE	SEL_BAT_CAP;	// _at_ 0x07D7	(byte) RSOC of selected battery
extern XWORD	SEL_BAT_VOLT;	// _at_ 0x07D8	(word) Volatge of selected battery
extern XWORD	SEL_BAT_CURRENT;// _at_ 0x07DA	(word) ABAT batt current (mA)
extern XWORD	SEL_BAT_TEMP;	// _at_ 0x07DC	(word) Temperature of selected battery
extern XWORD	BateryTempSBMus;// _at_ 0x07DE	(word) Battery temperature from SMBus

//------------------------------------------------------------------------------
// Counter Information (0x07E0 ~ 0x07E6)
//------------------------------------------------------------------------------
//extern XBYTE					// _at_ 0x07E0	(byte)
//extern XBYTE					// _at_ 0x07E1	(byte)
//extern XBYTE					// _at_ 0x07E2	(byte)
//extern XBYTE					// _at_ 0x07E3	(byte)
//extern XBYTE					// _at_ 0x07E4	(byte)
//extern XBYTE					// _at_ 0x07E5	(byte)
//extern XBYTE					// _at_ 0x07E6	(byte)

//------------------------------------------------------------------------------
// Normal Timer Information (0x07E7 ~ 0x07FF)
//------------------------------------------------------------------------------
extern XBYTE	PSW_TIMER;		// _at_ 0x07E7	(byte) Power switch timer counter
extern XBYTE	HWPG_TIMER;		// _at_ 0x07E8	(byte) Timer for waiting HWPG
extern XWORD	PSW_COUNTER;	// _at_ 0x07E9	(word) PSW override counter
extern XBYTE	RETRY_COUNTER;	// _at_ 0x07EB	(byte) Counter for SMB retry
extern XWORD	LED_FLASH_CNT;	// _at_ 0x07EC	(word) Counter for LED flash control
extern XBYTE	Q09CNT;			// _at_ 0x07EE	(byte) Delay Send Q-09 Counter
extern XBYTE	Volume_CNT;		// _at_ 0x07EF	(byte) For POPO noise
//extern XBYTE					// _at_ 0x07F0	(byte)

// Add One Touch Buttons function 2011/08/19 +++
extern XBYTE	OneTouchPollStep;	// _at_ 0x07F1	(byte) One Touch Buttons Polling Step
extern XBYTE	OneTouchButtStatus;	// _at_ 0x07F2	(byte) One Touch Buttons Status
extern XBYTE	OneTouchButtStatus2;// _at_ 0x07F3	(byte) One Touch Buttons Repeat Status
extern XBYTE	OneTouchButtonDelay;// _at_ 0x07F4	(byte) One Touch Buttons Counter
// Add One Touch Buttons function 2011/08/19 ---

extern XWORD	Bat_PreChargeCURT;	// _at_ 0x07F5	(word)	// leo_20160726 move from 0x07B2
//extern XBYTE						// _at_ 0x07F7	(byte)
//extern XBYTE						// _at_ 0x07F8	(byte)
//extern XBYTE						// _at_ 0x07F9	(byte)
extern XBYTE	CPU_RemoteErrorCnt;	// _at_ 0x07FA	(byte) CPU Remote Error Counter
extern XBYTE	VGA_RemoteErrorCnt;	// _at_ 0x07FB	(byte) VGA Remote Error Counter
//extern XBYTE						// _at_ 0x07FC	(byte)
//extern XBYTE						// _at_ 0x07FD	(byte)
//extern XBYTE						// _at_ 0x07FE	(byte)
//extern XBYTE						// _at_ 0x07FF	(byte)


//******************************************************************************
// 0x800-0x8FF  OEM RAM 8
//******************************************************************************
// 0x800-0x8FF  Ramdebug function


//******************************************************************************
// 0x900-0x9FF  OEM RAM 9
//******************************************************************************
extern XBYTE	SMB_DATA_OEM;	// _at_ 0x0900	(byte) EC SMB1 Data register array (32 bytes)
extern XBYTE	SMB_DATA1_OEM;	// _at_ 0x0901	(byte)
// Reserved offset 0x0902 ~ 0x091F

extern XBYTE	SPI_Mouse_Buf[3];	// _at_ 0x0920	(byte)
extern XBYTE	SPI_Mouse_DeltaX;	// _at_ 0x0923	(byte)
extern XBYTE	SPI_Mouse_DeltaY;	// _at_ 0x0924	(byte)

extern XBYTE	SPI_Mouse_BYTE1;	// _at_ 0x0925	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define LeftBtn			BIT0
#define RightBtn		BIT1
#define MiddleBtn		BIT2
#define Always1			BIT3
#define X_Signbit		BIT4
#define Y_Signbit		BIT5
#define X_Overflow		BIT6
#define Y_Overflow		BIT7

extern XBYTE	SPI_Mouse_XMovement;// _at_ 0x0926	(byte)
extern XBYTE	SPI_Mouse_YMovement;// _at_ 0x0927	(byte)
extern XBYTE	SPI_Mouse_Buf_Index;// _at_ 0x0928	(byte)

extern XBYTE	SPI_Mouse_Misc;		// _at_ 0x0929	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SPI_Mouse_Need_Break	BIT0

extern XBYTE	SPI_Mouse_ID;		// _at_ 0x092A	(byte)
// Reserved offset 0x092B ~ 0x092F

#ifdef PECI_Support
extern XBYTE	TDIM0_temp;				// _at_ 0x0938	(byte) DRAM 0 temperature
extern XBYTE	TDIM1_temp;				// _at_ 0x0939	(byte) DRAM 1 temperature
extern XBYTE	TPkg_temp;				// _at_ 0x093A	(byte)
extern XBYTE	TVGA_temp;				// _at_ 0x093B	(byte) VGA Temperature
extern XBYTE	TPCH_temp;				// _at_ 0x093C	(byte) PCH Temperature
extern XBYTE	TSys_temp;				// _at_ 0x093D	(byte) system Temperature
extern XBYTE	TCPU_temp;				// _at_ 0x093E	(byte) CPU Temperature
extern XBYTE	PECI_CPU_temp;			// _at_ 0x093F	(byte) Use PECI interface to read CPU temperature
extern XBYTE	PECIReadBuffer[];		// _at_ 0x0940	(byte)
extern XBYTE	PECIWriteBuffer[];		// _at_ 0x0950	(byte)
extern XBYTE	PECI_CRC8;				// _at_ 0x0960	(byte)
extern XBYTE	PECI_CompletionCode;	// _at_ 0x0961	(byte)
extern XBYTE	PECI_PCIConfigAddr[];	// _at_ 0x0962	(byte)
extern XBYTE	PECI_PowerCtrlStep;		// _at_ 0x0970	(byte)
extern XBYTE	PECI_PowerUnit;			// _at_ 0x0971	(byte)
extern XBYTE	PECI_EnergyUnit;		// _at_ 0x0972	(byte)
extern XBYTE	PECI_TimeUnit;			// _at_ 0x0973	(byte)
extern XBYTE	PECI_PL2Watts;			// _at_ 0x0974	(byte)
extern XBYTE	PECI_MAXPL2;			// _at_ 0x0975	(byte)
extern XBYTE	PECI_PL1TDP;			// _at_ 0x0976	(byte)
extern XBYTE	PECI_PL1Time;			// _at_ 0x0977	(byte)

extern XBYTE	PECI_Ctrl;				// _at_ 0x0978	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SetPL2Request	BIT0

extern XBYTE	PECI_AdaptorWatts;		// _at_ 0x0979	(byte)
extern XBYTE	PECI_SystemWatts;		// _at_ 0x097A	(byte)
extern XBYTE	PECI_Algorithm_Delay;	// _at_ 0x097B	(byte)
extern LWORD	PECI_ErrorCount;		// _at_ 0x097C	(dword)

extern XBYTE	PECI_StressToolBuf[];	// _at_ 0x0980	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define PSTB_RepeatCycle0			0
#define PSTB_RepeatCycle1			1
#define PSTB_RepeatCycle2			2
#define PSTB_RepeatCycle3			3
#define PSTB_RepeatInterval			4	// 10ms timer base
#define PSTB_StopOnError			5
#define PSTB_ClientAddress			6
#define PSTB_WriteLength			7
#define PSTB_ReadLength				8
#define PSTB_CommandSpecificData	9
#define PSTB_CmdCode				PSTB_CommandSpecificData
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	PSTB_Timer;				// _at_ 0x09A0	(byte)

extern XBYTE	PSTB_CmdStatus;			// _at_ 0x09A1	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ServivePECIStressTool	BIT0

extern XBYTE	PSTB_RepeatIntervalCount;	// _at_ 0x09A2	(byte)
extern LWORD	PSTB_RepeatCycle;			// _at_ 0x09A3	(dword)
extern XBYTE	CPUTjmax;					// _at_ 0x09A7	(byte)
extern XBYTE	ReadCPUTjmaxCUNT;			// _at_ 0x09A8	(byte)
extern XBYTE	StressToolReadBufIndex;		// _at_ 0x09A9	(byte)
// Reserved offset 0x09AA ~ 0x09AF
#endif	// PECI_Support

// David_20120106 modify for vOSSMbus/vOSSMbus_OEM routine
extern XBYTE	SMB_ADDR_OEM;	// _at_ 0x09B0	(byte)
extern XBYTE	SMB_CMD_OEM;	// _at_ 0x09B1	(byte)
extern XBYTE	SMB_BCNT_OEM;	// _at_ 0x09B2	(byte)	// leox_20120711 Add for SMBus block write
// David_20120106 modify for vOSSMbus/vOSSMbus_OEM routine

extern XBYTE	CIR_NEC_Act;	// _at_ 0x09B3	(byte)							// Ken_20120605 CIR_NEC_IMPROVED

extern XBYTE	Converter_Config_Status;	// _at_ 0x09B4	(byte) Converter configuartion status	//David_20141222 add LVDS converter FW update	// leox20150630 0x09E0 -> 0x09B4
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Fisrt_Delay_Fin		BIT0
#define Stop_FW_Fin			BIT1
#define Stop_FW_Delay_Fin	BIT2
#define Config_Fin			BIT3
#define Error_Over			BIT4

// Reserved offset 0x09B5 ~ 0x09BF

// leox20150630 Access eFlash +++		// leox20151030 SUPPORT_EFLASH_FUNC		// leox20160322 Code from N83
extern XBYTE eFL_Buf[4];				// _at_ 0x09C0	(byte) eFlash buffer
extern XBYTE eFL_Adr[4];				// _at_ 0x09C4	(byte) eFlash address
#define eFL_AdrH (*(XWORD*)(0x09C4))	// _at_ 0x09C4	(word) eFlash offset (high word)
#define eFL_AdrL (*(XWORD*)(0x09C6))	// _at_ 0x09C6	(word) eFlash offset (low  word)
extern XBYTE eFL_Act;					// _at_ 0x09C8	(byte) eFlash debug action
extern XBYTE eFL_Len;					// _at_ 0x09C9	(byte) eFlash access size
extern XWORD eFL_Blk;					// _at_ 0x09CA	(word) eFlash block number
extern XWORD eFL_Idx;					// _at_ 0x09CC	(word) eFlash block index
extern XBYTE eFL_ToDo;					// _at_ 0x09CE	(byte) eFlash to do		// leox20160316
//extern XBYTE							// _at_ 0x09CF	(byte)
// leox20150630 Access eFlash ---
// Reserved offset 0x09D0 ~ 0x09F7

//David_20141222 add for MMB service	// leox20160321 Move from 0x09C0-0x09C5 to 0x09F8-0x09FD
extern XBYTE	current_page;	// _at_ 0x09F8	(byte)
extern XBYTE	convn_setting;	// _at_ 0x09F9	(byte)
extern XBYTE	SLD_level[2];	// _at_ 0x09FA	(byte)
extern XBYTE	last_contact;	// _at_ 0x09FC	(byte)
extern XBYTE	CapButtonStat;	// _at_ 0x09FD	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MMBBtn0		BIT0
#define MMBBtn1		BIT1
#define MMBBtn2		BIT2
//David_20141222 add for MMB service

//extern XBYTE					// _at_ 0x09FE	(byte)

//David_20141222 add for factory test
extern XBYTE	Factory_MMBTest_status;		// _at_ 0x09FF	(byte)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define MMBBtn0		BIT0	// Reserved for tool
//#define MMBBtn1		BIT1	// Reserved for tool
//#define MMBBtn2		BIT2	// Reserved for tool
//#define				BIT3
//#define				BIT4
#define MMBLED_Start	BIT5
#define MMBLED_Fin		BIT6
#define MMBTest_Start	BIT7
//David_20141222 add for factory test


//******************************************************************************
// 0xA00-0xAFF  OEM RAM A
//******************************************************************************
// 0xA00-0xAFF  OEM RAM A --> All not use


//******************************************************************************
// 0xB00-0xBFF  OEM RAM B
//******************************************************************************
// 0xB00-0xBFF  OEM RAM B --> All not use


//******************************************************************************
// 0xC00-0xCFF  OEM RAM C
//******************************************************************************
// 0xC00-0xCFF  Log Shutdown Cause to RAM for LogShutdownCause()
#ifdef LogShutdownCauseInECRamCxx
extern XBYTE	BaseShutdownCause[254];	// _at_ 0x0C00	(byte)
extern XBYTE	LastShutdownCause;		// _at_ 0x0CFE	(byte)
extern XBYTE	NextShutdownCauseIndex;	// _at_ 0x0CFF	(byte)
#endif


//******************************************************************************
// 0xD00-0xDFF  OEM RAM D
//******************************************************************************
// 0xD00-0xDFF  Log Port 80h to RAM for LogP80()
#ifdef LogP80InECRamDxx
extern XBYTE	BaseP80[254];	// _at_ 0x0D00	(byte)
extern XBYTE	LastP80;		// _at_ 0x0DFE	(byte)
extern XBYTE	NextP80Index;	// _at_ 0x0DFF	(byte)
#endif


//******************************************************************************
// 0xE00-0xEFF  OEM RAM E
//------------------------------------------------------------------------------
// 0xE00-0xEFF  OEM RAM E --> All not use


//******************************************************************************
// 0xF00-0xFFF  OEM RAM F
//******************************************************************************
// 0xF00-0xFFF  For HSPI ram code function


//******************************************************************************
// Battery-backed SRAM (BRAM)
//******************************************************************************
// Chip_8512: (2200h~223Fh) = (2280h~22BFh) = ( 64 bytes)
// Chip_8518: (2200h~227Fh) + (2280h~22BFh) = (192 bytes)

//------------------------------------------------------------------------------
// BRAM Bank 0 (0x2200 ~ 0x227F) (128 bytes)
//------------------------------------------------------------------------------
//extern XBYTE	P80LB_BRAM;		// _at_ 0x2200	(byte) Reserve for P80		// LogP80InECRamDxx
extern XBYTE	CheckBramMask01;// _at_ 0x2201	(byte) For check if first backup battery power-up mask00
extern XBYTE	CheckBramMask02;// _at_ 0x2202	(byte) For check if first backup battery power-up mask01

extern XBYTE	USBCHARGEDISCHARG_BRM;	// _at_ 0x2203	(byte) For project special function	// OEM_IPOD
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define								BIT0
//#define								BIT1
//#define								BIT2
#define USB4_CHARG_Enable_BRAM			BIT3	// BIOS set/clear, default clear
#define USB4_CHARG_Enable_ACBAT_BRAM	BIT4	// When USB4_CHARG_Enable_BRAM set then check this bit, BIOS set/clear, default clear (AC and DC Enable/AC Enable 1/0)	// OEM_IPOD_ACDC
#define USB4_CHARG_FastMode_BRAM		BIT5	// BIOS set/clear, default clear (1/0 for Fast(CDP)/Normal(SDP) mode)	// leox20150707 For USB charger SLG55544 of FH9C

extern XBYTE	B_PwrLoss;		// _at_ 0x2204	(byte) For power loss function	// leox_20110921 OEM_POWER_LOSS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BIT [7:2] - Last power state
//             000000: System off
//             101010: System on
// BIT [1:0] - After power loss
//             00: Last state
//             01: On
//             10: Off
//             11: Reserved
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

extern XBYTE	B_PwrCtrl;		// _at_ 0x2205	(byte) For power control		// leox_20120203
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define EC_SLP_MODE_AC	BIT0	// 1 EC enter sleep mode even AC in				// ENTER_SLEEP_MODE_AC

extern XBYTE	Shipmode_status;// _at_ 0x2206	(byte) For ship mode function	//David modify for battery shutdown mode 2014/07/02
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define Manufacture	BIT0	// 1 Simplo, 0 Sanyo
#define Mode_type		BIT1	// 1 Shutdown mode; 0 ship mode
#define Tool_Notify		BIT2	// 1 Tool already notify do ship mode
#define EC_Ready		BIT3	// 1 EC ready to do ship mode
#define EC_Send			BIT4	// 1 EC send ship mode command success
#define EC_Send1		BIT5	// 1 EC send ship mode command success
#define Wait_AC			BIT6	// 1 Wait AC wake up, 0 AC wake up
#define LED_Fin			BIT7	// 1 LED finish can send command

//extern XBYTE					// _at_ 0x2207	(byte)

//David add for support G3 keep status 2014/07/24								// IT_7230MMB_SUPPORT
extern XBYTE	MMB_WakeUp_S3_BRM;		// _at_ 0x2208	(byte)
extern XBYTE	MMB_WakeUp_S4_BRM;		// _at_ 0x2209	(byte)
extern XBYTE	MMB_WakeUp_S5_BRM;		// _at_ 0x220A	(byte)
extern XBYTE	POWER_STAT_CTRL1_BRM;	// _at_ 0x220B	(byte)
//David add for support G3 keep status 2014/07/24

//extern XBYTE					// _at_ 0x220C	(byte)
//extern XBYTE					// _at_ 0x220D	(byte)

extern XBYTE	B_AppSwitchFnFx;// _at_ 0x220E	(byte) For switch Fn+Fx function set by application	// leox_20120314 SUPPORT_OEM_APP
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 0x00: Initial value --> First backup battery power up or never run application
// 0x66: Normal  mode  --> Pressed Fn+Fx (Hotkey keycode) and pressed Fx (Fx keycode)
// 0x77: Special mode  --> Pressed Fn+Fx (Fx keycode) and pressed Fx (Fn+Fx keycode)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//reboot for IFU.exe tool ++
//extern XBYTE	BootMask_Bram_00;	// _at_ 0x223C	(byte)
//extern XBYTE	BootMask_Bram_01;	// _at_ 0x223D	(byte)
//extern XBYTE	BootMask_Bram_02;	// _at_ 0x223E	(byte)
//extern XBYTE	BootMask_Bram_03;	// _at_ 0x223F	(byte)
//reboot for IFU.exe tool --

//------------------------------------------------------------------------------
// BRAM Bank 1 (0x2280 ~ 0x22BF) (64 bytes)
//------------------------------------------------------------------------------
extern XBYTE	P80LB_BRAM;			// _at_ 0x2280	(byte)						// LogP80InECRamDxx
//extern XBYTE	CheckBramMask01;	// _at_ 0x2281	(byte) For check if first backup battery power-up mask00
//extern XBYTE	CheckBramMask02;	// _at_ 0x2282	(byte) For check if first backup battery power-up mask01
//extern XBYTE						// _at_ 0x2283	(byte)
//extern XBYTE						// _at_ 0x2284	(byte)
//extern XBYTE						// _at_ 0x2285	(byte)
//extern XBYTE						// _at_ 0x2286	(byte)
//extern XBYTE						// _at_ 0x2287	(byte)
//extern XBYTE						// _at_ 0x2288	(byte)
//extern XBYTE						// _at_ 0x2289	(byte)
//extern XBYTE						// _at_ 0x228A	(byte)
//extern XBYTE						// _at_ 0x228B	(byte)
//extern XBYTE						// _at_ 0x228C	(byte)
//extern XBYTE						// _at_ 0x228D	(byte)
//extern XBYTE						// _at_ 0x228E	(byte)
// 0x2290 ~ 0x22AB not used yet

// 120731 chris add for fix flash EC by WINIFU then shutdown then boot fail +++
extern XBYTE	BIOS_FLASH_Reset00;	// _at_ 0x22AC	(byte)
extern XBYTE	BIOS_FLASH_Reset01;	// _at_ 0x22AD	(byte)
extern XBYTE	BIOS_FLASH_Reset02;	// _at_ 0x22AE	(byte)
extern XBYTE	BIOS_FLASH_Reset03;	// _at_ 0x22AF	(byte)
// 120731 chris add for fix flash EC by WINIFU then shutdown then boot fail ---

extern XBYTE	B_SdCause[SdCauseBRAMSize];	// _at_ 0x22B0	(byte) For log shutdown code	// leox_20111209 LogShutdownCauseInBRAM
#define B_SdCauseIdx	B_SdCause[SdCauseBRAMSize-1]	// (byte) Index for log shutdown code

//reboot for IFU.exe tool ++
extern XBYTE	BootMask_Bram_00;	// _at_ 0x22BC	(byte)
extern XBYTE	BootMask_Bram_01;	// _at_ 0x22BD	(byte)
extern XBYTE	BootMask_Bram_02;	// _at_ 0x22BE	(byte)
extern XBYTE	BootMask_Bram_03;	// _at_ 0x22BF	(byte)
//reboot for IFU.exe tool --


#endif	/* OEM_MEMORY_H */
