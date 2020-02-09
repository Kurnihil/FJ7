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


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


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
XBYTE	SPIBuffer[256]			_at_ 0x0200;	// (byte) 256 bytes SPI read/write buffer


//******************************************************************************
// 0x300-0x3FF  OEM RAM 3
//******************************************************************************
//XBYTE							_at_ 0x0300;	// (byte)
//XBYTE							_at_ 0x0301;	// (byte)
//XBYTE							_at_ 0x0302;	// (byte)
//XBYTE							_at_ 0x0303;	// (byte)
//XBYTE							_at_ 0x0304;	// (byte)
//XBYTE							_at_ 0x0305;	// (byte)
//XBYTE							_at_ 0x0306;	// (byte)
//XBYTE							_at_ 0x0307;	// (byte)
//XBYTE							_at_ 0x0308;	// (byte)
XBYTE	BIOS_QE					_at_ 0x0309;	// (byte)
//XBYTE							_at_ 0x030A;	// (byte)
//XBYTE							_at_ 0x030B;	// (byte)
//XBYTE							_at_ 0x030C;	// (byte)
//XBYTE							_at_ 0x030D;	// (byte)
//XBYTE							_at_ 0x030E;	// (byte)
XBYTE	VerMJ					_at_ 0x030F;	// (byte)
XBYTE	VerMN					_at_ 0x0310;	// (byte)
XBYTE	VerFR					_at_ 0x0311;	// (byte)

XWORD	ChkEcRamValid			_at_ 0x0312;	// (word) Check if EC RAM valid	// leox20160317 Check EC RAM

XBYTE	WaitS5ToDoWDT			_at_ 0x0314;	// (byte)						// leox20151117 From 0x2207

// For ITE WinFlash Tool ++		// ken 2012/06/29
XBYTE	FLHB					_at_ 0x0315;	// (byte) Begin counter
XBYTE	FLHE					_at_ 0x0316;	// (byte) End counter
XBYTE	FLHH					_at_ 0x0317;	// (byte) Handshaking
// For ITE WinFlash Tool --

XBYTE	SMB_PRTC				_at_ 0x0318;	// (byte) EC SMB1 Protocol register
XBYTE	SMB_STS					_at_ 0x0319;	// (byte) EC SMB1 Status register
XBYTE	SMB_ADDR				_at_ 0x031A;	// (byte) EC SMB1 Address register
XBYTE	SMB_CMD					_at_ 0x031B;	// (byte) EC SMB1 Command register
XBYTE	SMB_DATA				_at_ 0x031C;	// (byte) EC SMB1 Data register array (32 bytes)
//XBYTE	SMB_DATA1[31]			_at_ 0x031D;	// (byte)
XBYTE	SMB_DATA1				_at_ 0x031D;	// (byte)
XBYTE	SMB_BCNT				_at_ 0x033C;	// (byte) EC SMB1 Block Count register
XBYTE	SMB_ALRA				_at_ 0x033D;	// (byte) EC SMB1 Alarm Address register
XBYTE	SMB_ALRD0				_at_ 0x033E;	// (byte) EC SMB1 Alarm Data register 0
XBYTE	SMB_ALRD1				_at_ 0x033F;	// (byte) EC SMB1 Alarm Data register 1

XWORD	Fan_Rpm2				_at_ 0x0340;	// (word) Fan 2 RPM				// leox_20120322 Support_FAN2
XBYTE	JUMPERSETTING			_at_ 0x0342;	// (byte)
XBYTE	BANKDEFINE				_at_ 0x0343;	// (byte)
XBYTE	POWER_STAT_CTRL			_at_ 0x0344;	// (byte)
XBYTE	KETBOARD_STAT_CTRL		_at_ 0x0345;	// (byte)

XWORD	BT1I_BIOS				_at_ 0x0346;	// (word) Batt1_Rate (mA)
XWORD	BT1C_BIOS				_at_ 0x0348;	// (word) Batt1_Remain_Cap (mAh)
XWORD	BT1V_BIOS				_at_ 0x034A;	// (word) Batt1_Volt (mV)
XWORD	BT2I_BIOS				_at_ 0x034C;	// (word) Batt2_Rate (mA)
XWORD	BT2C_BIOS				_at_ 0x034E;	// (word) Batt2_Remain_Cap (mAh)
XWORD	BT2V_BIOS				_at_ 0x0350;	// (word) Batt2_Volt (mV)

XBYTE	Device_STAT_CTRL		_at_ 0x0352;	// (byte)

XBYTE	ChkReadWriteBattFirstUSEDate	_at_ 0x0353;	// (byte) FA/FB Read/Write BattFirstUSEDate
XBYTE	WriteBattFirstUSEDate			_at_ 0x0354;	// (byte)
XBYTE	WriteBattFirstUSEDatehighbyte	_at_ 0x0355;	// (byte)
XWORD	ReadBattFirstUSEDate			_at_ 0x0356;	// (word)

XBYTE	CPU_Remote_Temp			_at_ 0x0358;	// (byte) CPU Remote Temperature
XBYTE	Device_STAT_CTRL1		_at_ 0x0359;	// (byte)	// CHECK_FAN_FAULT	//David_20120613 add for fan error check
XBYTE	POWER_STAT_CTRL1		_at_ 0x035A;	// (byte)
XBYTE	Critical_Wake_Level		_at_ 0x035B;	// (byte) Critical battery threshold for wake up from S3 (Set by BIOS)
XBYTE	ChargeLimit_CTRL		_at_ 0x035C;	// (byte) Charge limit control	//David add for charge limit function 2014/07/02
XBYTE	CPUTjMaxOEM				_at_ 0x035D;	// (byte) CPU TjMax by OEM		// leox_20120301 CPU_TYPE_BY_TJMAX
XBYTE	ThrottlingTemp			_at_ 0x035E;	// (byte) Throttling point		// leox_20120229 UPDATE_THERM_POINT
XBYTE	OSShutdownTemp			_at_ 0x035F;	// (byte) OS shutdown point		// leox_20120229 UPDATE_THERM_POINT
XBYTE	USB_CutOffpower			_at_ 0x0360;	// (byte)
XBYTE	USBCHARGEDISCHARG		_at_ 0x0361;	// (byte)						// OEM_IPOD
XBYTE	InitUSBChargePortCNT	_at_ 0x0362;	// (byte) InitUSBChargePort Counter	// OEM_IPOD
XBYTE	SELECTOR_STATE1			_at_ 0x0363;	// (byte)
XBYTE	SELECTOR_STATE2			_at_ 0x0364;	// (byte)
XBYTE	CPU_TYPE				_at_ 0x0365;	// (byte) For BIOS set/clear
XWORD	Fan_Rpm					_at_ 0x0366;	// (word) Fan 1 feedback counter
XBYTE	DummySCI_CNT			_at_ 0x0368;	// (byte) Send dummy SCI counter every 1 sec
XBYTE	Device_Setup_Ctrl		_at_ 0x0369;	// (byte) Setup menu control device

XWORD	BIOS_DESIGN_CAPACITY		_at_ 0x036A;	// (word) Battery design capacity	// leox_20120402 Change offset
XWORD	BIOS_DESIGN_FULL_CAPACITY	_at_ 0x036C;	// (word) Battery full charge capacity

XBYTE	NECBattRefreshPSTS		_at_ 0x036E;	// (byte)						// NECP_BattRefresh_Support
XBYTE	NECBattRefreshPSTS01	_at_ 0x036F;	// (byte)						// NECP_BattRefresh_Support

XBYTE	BIOS_TouchpadSWFnFlag	_at_ 0x0370;	// (byte)
XBYTE	BIOS_ABAT_STATUS		_at_ 0x0371;	// (byte)
XBYTE	BIOS_ACHARGE_STATUS		_at_ 0x0372;	// (byte)
XBYTE	BIOS_ABAT_MISC			_at_ 0x0373;	// (byte)
//David add for FJ request change battery CV 2016/11/08
XBYTE	Battery_Status					_at_ 0x0374;	// (byte)
//David add for FJ request change battery CV 2016/11/08
XWORD	BIOS_DESIGN_VOLTAGE		_at_ 0x0375;	// (word) Battery design voltage
XBYTE	BIOS_ABAT_CAP			_at_ 0x0377;	// (byte) ABAT batt capacity (RSOC %)
XWORD	BIOS_BateryTempSBMus	_at_ 0x0378;	// (word) Battery temperature from SMBus

XBYTE	BrightnessValue			_at_ 0x037A;	// (byte) Brightness value
XBYTE	BrightnessLevel			_at_ 0x037B;	// (byte) Brightness level		// leox_20120111

XBYTE	VolumeValue				_at_ 0x037C;	// (byte) Volume value			// leox_20111014 SUPPORT_OEM_APP
XBYTE	VolumeLevel				_at_ 0x037D;	// (byte) Volume level			// leox_20120111

XBYTE	BIOS_ShutDnCause		_at_ 0x037E;	// (byte) System shutdown cause	// leox20150922

//David_20120327 modify for battery information	// leox_20120402 Change offset
XBYTE	BarcodeBCNT				_at_ 0x037F;	// (byte)
XBYTE	Barcode32B				_at_ 0x0380;	// (byte) Range: 0x0380~0x039F
//David_20120327 modify for battery information

// leox_20111014 Support OEM application +++	// SUPPORT_OEM_APP
XBYTE	AppReady				_at_ 0x03A0;	// (byte) OEM application status
XBYTE	AppGetVolCnt			_at_ 0x03A1;	// (byte) Counter for volume get	// leox_20111017
XBYTE	AppCommand				_at_ 0x03A2;	// (byte) Command from application	// leox_20111109
XBYTE	AppMuteCnt				_at_ 0x03A3;	// (byte) Counter for volume mute	// leox_20120410
XBYTE	AppVol0Cnt				_at_ 0x03A4;	// (byte) Counter for volume down
XBYTE	AppVol1Cnt				_at_ 0x03A5;	// (byte) Counter for volume up
XBYTE	AppTaskFlag				_at_ 0x03A6;	// (byte) Task flag for application
XBYTE	AppSetVolume			_at_ 0x03A7;	// (byte) Set volume via application using Q54_SCI
XBYTE	AppGetVolume			_at_ 0x03A8;	// (byte) Get volume via application using Q53_SCI
XBYTE	RW_EC_AdrL				_at_ 0x03A9;	// (byte) Address for R/W EC set by BIOS (Low  byte)
XBYTE	RW_EC_AdrH				_at_ 0x03AA;	// (byte) Address for R/W EC set by BIOS (High byte)
XBYTE	RW_EC_Data				_at_ 0x03AB;	// (byte) Data for R/W EC set by BIOS
XBYTE	AppGetMute				_at_ 0x03AC;	// (byte) Get mute state via application using Q51_SCI	// leox_20120411
//XBYTE							_at_ 0x03AD;	// (byte)
//XBYTE							_at_ 0x03AE;	// (byte)
//XBYTE							_at_ 0x03AF;	// (byte)
// leox_20111014 Support OEM application ---

XWORD	REGISTER_ADDR			_at_ 0x03B0;	// (word) Physical 591 register address
XWORD	REGISTER_DATA			_at_ 0x03B2;	// (word) Content of 591 register

XBYTE	WatchDOGS4S5_CNT		_at_ 0x03B4;	// (byte)
//XBYTE							_at_ 0x03B5;	// (byte)

XWORD	VICM_V					_at_ 0x03B6;	// (word)
XBYTE	VICMVHiEnThroLimitCnt	_at_ 0x03B8;	// (byte) Enabled CPU speed down by VICMV debounce counter
XBYTE	VICMVHiDisThroLimitCnt	_at_ 0x03B9;	// (byte) Disable CPU speed down by VICMV debounce counter
XBYTE	VICMVHiFlag				_at_ 0x03BA;	// (byte) VICMV Flag

XWORD	DCMI_V					_at_ 0x03BB;	// (word)
XBYTE	DCMIHiEnThroLimitCnt	_at_ 0x03BD;	// (byte) Enabled CPU speed down by DCMI debounce counter
XBYTE	DCMIHiDisThroLimitCnt	_at_ 0x03BE;	// (byte) Disable CPU speed down by DCMI debounce counter
XBYTE	DCMIHiFlag				_at_ 0x03BF;	// (byte) DCMI Flag

XWORD	VICM_VACDC					_at_ 0x03C0;	// (word)
XBYTE	VICMVHiEnThroLimitCntACDC	_at_ 0x03C2;	// (byte) Enabled CPU speed down by VICMV debounce counter
XBYTE	VICMVHiDisThroLimitCntACDC	_at_ 0x03C3;	// (byte) Disable CPU speed down by VICMV debounce counter
XBYTE	VICMVHiFlagACDC				_at_ 0x03C4;	// (byte) VICMV Flag

XWORD	CmpEnVICM_VACDC_Limit	_at_ 0x03C5;	// (word)
XWORD	CmpDisVICM_VACDC_Limit	_at_ 0x03C7;	// (word)

XBYTE	TPTYPE4BIOS				_at_ 0x03C9;	// (byte) Touchpad type for BIOS	// ken add for touchpad compare	// Multiple_TP
XBYTE	FanControlByBIOS		_at_ 0x03CA;	// (byte)
XBYTE	BatteryCellNum			_at_ 0x03CB;	// (byte)
XBYTE	KCMISCBIT0CNT			_at_ 0x03CC;	// (byte)
XBYTE	KCMISCBIT1CNT			_at_ 0x03CD;	// (byte)
XWORD	BatterySerialNo			_at_ 0x03CE;	// (word)

//David_20120327 modify for battery information	// leox_20120403 Change range
XBYTE	DeviceName15B			_at_ 0x03D0;	// (byte) 0x03D0~0x03DE
XBYTE	DeviceNameBCNT			_at_ 0x03DF;	// (byte)
//David_20120327 modify for battery information

XWORD	BatteryManuFactureDate	_at_ 0x03E0;	// (word)
XWORD	BIOS_BTCyCle			_at_ 0x03E2;	// (word) Batt Cycle Count		//David add for read battery cycle count 2014/06/11

XWORD	CmpEnHybrid_Limit		_at_ 0x03E4;	// (word)
XWORD	CmpDisHybrid_Limit		_at_ 0x03E6;	// (word)
XWORD	CmpEnVICM_Limit			_at_ 0x03E8;	// (word)
XWORD	CmpDisVICM_Limit		_at_ 0x03EA;	// (word)

XBYTE	Factory_KBTest_status	_at_ 0x03EC;	// (byte)	//David add for factory test 2014/07/24

//David modify for fit MMB button layout 2014/07/09	//David add for MMB wake up support 2014/07/02
XBYTE	MMB_WakeUp_S3			_at_ 0x03ED;	// (byte)
XBYTE	MMB_WakeUp_S4			_at_ 0x03EE;	// (byte)
XBYTE	MMB_WakeUp_S5			_at_ 0x03EF;	// (byte)
//David modify for fit MMB button layout 2014/07/09	//David add for MMB wake up support 2014/07/02

//David_20120327 modify for battery information	// leox_20120402 Change offset
XBYTE	Manu_NameBCNT			_at_ 0x03F0;	// (byte)
XBYTE	Manu_Name11B			_at_ 0x03F1;	// (byte) Range: 0x03F1~0x03FB
//David_20120327 modify for battery information

XBYTE	MMB_Status_OEM			_at_ 0x03FC;	// (byte)
XBYTE	CrisisKeyStatus			_at_ 0x03FD;	// (byte) Crisis key flag
XBYTE	Compare_LowByte			_at_ 0x03FE;	// (byte)
XBYTE	Compare_HighByte		_at_ 0x03FF;	// (byte)


//******************************************************************************
// 0x400-0x4FF  OEM RAM 4
//******************************************************************************
#if CHARGER_BQ24780S_DBG						// leox20150617 Add for charger IC debug			// leox20150810
XWORD	ChgIcRegR[13]			_at_ 0x0400;	// (byte) Register state (26 bytes)(0x0400-0x0419)
XBYTE	ChgIcCnt				_at_ 0x041A;	// (byte) Counter for polling registers
XBYTE	ChgIcTmp				_at_ 0x041B;	// (byte) Counter for hook function
XWORD	CmpEnHybrid_LimitD		_at_ 0x041C;	// (word) Hybrid power trigger point for debug		// leox20150716 Move up
XWORD	CmpDsHybrid_LimitD		_at_ 0x041E;	// (word) Hybrid power release point for debug		// leox20150716 Move up
XWORD	ChgIcRegW[11]			_at_ 0x0420;	// (byte) Register debug (22 bytes)(0x0420-0x0435)	// leox20160317 Charger debug
//XBYTE							_at_ 0x0436;	// (byte)
//XBYTE							_at_ 0x0437;	// (byte)
XWORD	ChgIcRegF				_at_ 0x0438;	// (word) Register debug write flag					// leox20160317 Charger debug
#endif

#if ADAPTER_PROTECT_DBG							// leox20150716 Adapter protection debug			// leox20150810
XBYTE	VICMVEnThrottleDebD		_at_ 0x043A;	// (byte) Enable  CPU throttling de-bounce time		// leox20150717 Adapter protection debug
XBYTE	VICMVDsThrottleDebD		_at_ 0x043B;	// (byte) Disable CPU throttling de-bounce time		// leox20150717 Adapter protection debug
XWORD	CmpEnVICM_LimitD		_at_ 0x043C;	// (word) Adapter protect trigger point for debug	// leox20150716 Adapter protection debug
XWORD	CmpDsVICM_LimitD		_at_ 0x043E;	// (word) Adapter protect release point for debug	// leox20150716 Adapter protection debug
#endif

XBYTE	ShutDnCause				_at_ 0x0440;	// (byte) System shut down cause code
XBYTE	DA_CCSET_BackUp			_at_ 0x0441;	// (byte) DA_CCSET value Back Up when CHG# is high
XWORD	FCC99P_CAPACITY			_at_ 0x0442;	// (word) 99% of FCC value
XWORD	FCCxP_CAPACITY			_at_ 0x0444;	// (word) (ForceDecPercent/2)% of FCC value
XWORD	DESIGN_FULL_CAPACITY	_at_ 0x0446;	// (word) Battery full charge capacity
XWORD	DESIGN_CAPACITY			_at_ 0x0448;	// (word) Battery design capacity
XWORD	DESIGN_VOLTAGE			_at_ 0x044A;	// (word) Battery design voltage
XBYTE	RSOC_FULL_BASE			_at_ 0x044C;	// (byte) RSOC-base detector threshold
XBYTE	FAN_LIMIT2				_at_ 0x044D;	// (byte) Desire Fan2 speed		// leox_20120322 Support_FAN2
XBYTE	CPU_Local_Temp			_at_ 0x044E;	// (byte) CPU Local temperature
XBYTE	VGA_Remote_Temp			_at_ 0x044F;	// (byte) VGA Remote Temperature
XBYTE	VGA_Local_Temp			_at_ 0x0450;	// (byte) VGA Local temperature
XBYTE	DBG_CPU_Remote_Temp		_at_ 0x0451;	// (byte) Debug CPU Remote Temp
XBYTE	DBG_CPU_Local_Temp		_at_ 0x0452;	// (byte) Debug CPU Local Temp
XBYTE	DBG_VGA_Remote_Temp		_at_ 0x0453;	// (byte) Debug VGA Remote Temp
XBYTE	DBG_VGA_Local_Temp		_at_ 0x0454;	// (byte) Debug VGA Local Temp
XBYTE	FAN_LIMIT_CPU			_at_ 0x0455;	// (byte) CPU Fan Limit
XBYTE	FAN_LIMIT_VGA			_at_ 0x0456;	// (byte) VGA Fan Limit
XBYTE	FAN_LIMIT				_at_ 0x0457;	// (byte) Desire fan speed
XBYTE	RdSCIevent_backup		_at_ 0x0458;	// (byte) Read SCI Event Back Up
XBYTE	WrSCIevent_backup		_at_ 0x0459;	// (byte) Write SCI Event Back Up
XBYTE	RdSMIevent_backup		_at_ 0x045A;	// (byte) Read SMI Event Back Up
XBYTE	WrSMIevent_backup		_at_ 0x045B;	// (byte) Write SCI Event Back Up
XWORD	FLED_FLASH_CNT			_at_ 0x045C;	// (word) Counter for LED flash control
XBYTE	POWER_FLAG				_at_ 0x045E;	// (byte)
XBYTE	POWER_FLAG2				_at_ 0x045F;	// (byte)
XBYTE	SYS_FLAG				_at_ 0x0460;	// (byte)
XBYTE	MISC_FLAG				_at_ 0x0461;	// (byte) System misc flags
XBYTE	NECOverChgCurrent		_at_ 0x0462;	// (byte) For NEC check Over charging current Ram
XBYTE	MotionLedsFunctionCNT	_at_ 0x0463;	// (byte)						// OEM_MotionLed
XBYTE	MotionLedsFunctionFlag	_at_ 0x0464;	// (byte)						// OEM_MotionLed
XWORD	BT1I					_at_ 0x0465;	// (word) Batt1_Rate (mA)
XWORD	BT1C					_at_ 0x0467;	// (word) Batt1_Remain_Cap (mAh)
XWORD	BT1V					_at_ 0x0469;	// (word) Batt1_Volt (mV)
XWORD	BT2I					_at_ 0x046B;	// (word) Batt2_Rate (mA)
XWORD	BT2C					_at_ 0x046D;	// (word) Batt2_Remain_Cap (mAh)
XWORD	BT2V					_at_ 0x046F;	// (word) Batt2_Volt (mV)
XWORD	BTCyCle					_at_ 0x0471;	// (word) Batt Cycle Count		//David add for read battery cycle count 2014/06/11
//XBYTE							_at_ 0x0473;	// (byte)

#if BAT_DCHG_PROTECT							// leox20150826 Battery DCHG protection
XWORD	DCHG_V					_at_ 0x0474;	// (word)
XWORD	CmpEnDCHG_Limit			_at_ 0x0476;	// (word)
XWORD	CmpDsDCHG_Limit			_at_ 0x0478;	// (word)
XBYTE	DCHGVHiEnThroLimitCnt	_at_ 0x047A;	// (byte) Enabled CPU speed down by DCHGV debounce counter
XBYTE	DCHGVHiDsThroLimitCnt	_at_ 0x047B;	// (byte) Disable CPU speed down by DCHGV debounce counter
XBYTE	DCHGVHiEnThroLimitCntD	_at_ 0x047C;	// (byte)
XBYTE	DCHGVHiDsThroLimitCntD	_at_ 0x047D;	// (byte)
#endif

XBYTE	Device_STAT_CTRL2			_at_ 0x047E;	// (byte)
XBYTE	Hybrid_Status			_at_ 0x047F;	// (byte)

//------------------------------------------------------------------------------
// Power On/Off Sequence Input Signals (0x0480 ~ 0x049D)
//------------------------------------------------------------------------------
XBYTE	FromSBsignals00			_at_ 0x0480;	// (byte)
XBYTE	FromSBsignals01			_at_ 0x0481;	// (byte)
XBYTE	FromSBsignals02			_at_ 0x0482;	// (byte)
XBYTE	FromMBsignals00			_at_ 0x0483;	// (byte)
//XBYTE							_at_ 0x0484;	// (byte)
//XBYTE							_at_ 0x0485;	// (byte)
XBYTE	FromSBsignals00_BK0		_at_ 0x0486;	// (byte)
XBYTE	FromSBsignals00_BK1		_at_ 0x0487;	// (byte)
XBYTE	FromSBsignals00_BK2		_at_ 0x0488;	// (byte)
XBYTE	FromMBsignals00_BK0		_at_ 0x0489;	// (byte)
XBYTE	FromMBsignals00_BK1		_at_ 0x048A;	// (byte)
XBYTE	FromMBsignals00_BK2		_at_ 0x048B;	// (byte)
XBYTE	FromSBsignals01_BK0		_at_ 0x048C;	// (byte)
XBYTE	FromSBsignals01_BK1		_at_ 0x048D;	// (byte)
XBYTE	FromSBsignals01_BK2		_at_ 0x048E;	// (byte)
//XBYTE							_at_ 0x048F;	// (byte)
//XBYTE							_at_ 0x0490;	// (byte)
//XBYTE							_at_ 0x0491;	// (byte)
XBYTE	FromSBsignals02_BK0		_at_ 0x0492;	// (byte)
XBYTE	FromSBsignals02_BK1		_at_ 0x0493;	// (byte)
XBYTE	FromSBsignals02_BK2		_at_ 0x0494;	// (byte)
//XBYTE							_at_ 0x0495;	// (byte)
XBYTE	PECI_ME_Sequence_Num_BK	_at_ 0x0496;	// (byte) PECI ME Sequence Number Back Up
XBYTE	PECI_ME_HandUp_ErrorCnt	_at_ 0x0497;	// (byte) PECI ME HandUp ErrorCnt
XBYTE	FromSBsignals00_CNT		_at_ 0x0498;	// (byte)
XBYTE	FromSBsignals01_CNT		_at_ 0x0499;	// (byte)
XBYTE	FromSBsignals02_CNT		_at_ 0x049A;	// (byte)
XBYTE	FromMBsignals00_CNT		_at_ 0x049B;	// (byte)
XBYTE	PCH_RemoteErrorCnt		_at_ 0x049C;	// (byte) CPU Remote Error Counter
XBYTE	PECI_RemoteErrorCnt		_at_ 0x049D;	// (byte) VGA Remote Error Counter

//------------------------------------------------------------------------------
// Power On/Off Sequence Output Signals (0x049E ~ 0x04A6)
//------------------------------------------------------------------------------
XBYTE	ToSBsignals00			_at_ 0x049E;	// (byte)
XBYTE	ToSBsignals01			_at_ 0x049F;	// (byte)
XBYTE	ToSBsignals02			_at_ 0x04A0;	// (byte)
XBYTE	ToSBsignals03			_at_ 0x04A1;	// (byte)
XBYTE	ToMBsignals00			_at_ 0x04A2;	// (byte)
XBYTE	ToMBsignals01			_at_ 0x04A3;	// (byte)
XBYTE	ToMBsignals02			_at_ 0x04A4;	// (byte)
XBYTE	ToMBsignals03			_at_ 0x04A5;	// (byte)
XBYTE	ToMBsignals04			_at_ 0x04A6;	// (byte)

XBYTE	FanControl_Mode00_Bk	_at_ 0x04A7;	// (byte)
XBYTE	CPU_Remote_Temp_BackUp	_at_ 0x04A8;	// (byte)
XBYTE	CPU_Local_Temp_BackUp	_at_ 0x04A9;	// (byte)
XBYTE	VGA_Remote_Temp_BackUp	_at_ 0x04AA;	// (byte)
XBYTE	VGA_Local_Temp_BackUp	_at_ 0x04AB;	// (byte)
//XBYTE							_at_ 0x04AC;	// (byte)
//XBYTE							_at_ 0x04AD;	// (byte)
XWORD	CPU_Remote_RPM_Value	_at_ 0x04AE;	// (word)
XWORD	CPU_Local_RPM_Value		_at_ 0x04B0;	// (word)
XWORD	VGA_Remote_RPM_Value	_at_ 0x04B2;	// (word)
XWORD	VGA_Local_RPM_Value		_at_ 0x04B4;	// (word)
XWORD	Expect_RPM_Value2		_at_ 0x04B6;	// (word)						// leox_20120322 Support_FAN2
XWORD	Expect_RPM_Value		_at_ 0x04B8;	// (word)
XBYTE	CPU_Remote_DAPWM_Value	_at_ 0x04BA;	// (byte)
XBYTE	CPU_Local_DAPWM_Value	_at_ 0x04BB;	// (byte)
XBYTE	VGA_Remote_DAPWM_Value	_at_ 0x04BC;	// (byte)
XBYTE	VGA_Local_DAPWM_Value	_at_ 0x04BD;	// (byte)
//XBYTE							_at_ 0x04BE;	// (byte)
XBYTE	FanControl_Mode00		_at_ 0x04BF;	// (byte)
//XBYTE							_at_ 0x04C0;	// (byte)
//XBYTE							_at_ 0x04C1;	// (byte)
XBYTE	FanControl_Mode01		_at_ 0x04C2;	// (byte)
XBYTE	FanControl_Mode01_Bk	_at_ 0x04C3;	// (byte)
XBYTE	FanSpeedOnSec			_at_ 0x04C4;	// (byte)
XBYTE	FanSpeedOnSec_256		_at_ 0x04C5;	// (byte)
XBYTE	FanSpeedOffSec			_at_ 0x04C6;	// (byte)
XBYTE	FanSpeedOffSec_256		_at_ 0x04C7;	// (byte)

// leox20150121 For check EC version +++		// leox20150522 From HP N61A
XBYTE	VerY					_at_ 0x04C8;	// (byte) EC version: Year
XBYTE	VerM					_at_ 0x04C9;	// (byte) EC version: Month
XBYTE	VerD					_at_ 0x04CA;	// (byte) EC version: Day
XBYTE	VerB					_at_ 0x04CB;	// (byte) EC version: Build
// leox20150121 For check EC version ---

//XBYTE							_at_ 0x04CC;	// (byte)
XBYTE	DIMM_LED_Status			_at_ 0x04CD;	// (byte)
XBYTE	ThreeG_En_CNT			_at_ 0x04CE;	// (byte) Delay from MAINON to 3G_EN Counter
XBYTE	DLIDCNT					_at_ 0x04CF;	// (byte) Delay LID# Counter
XBYTE	Factory_Test_Status		_at_ 0x04D0;	// (byte)
XBYTE	TouchpadSWFnFlag		_at_ 0x04D1;	// (byte) Touchpad Switch Function Flag
XBYTE	TouchpadSWFnDebounce	_at_ 0x04D2;	// (byte) Touchpad Switch Function Debounce
XBYTE	ThermalSnrClrALERTCNT	_at_ 0x04D3;	// (byte) Thermal Sensor Clear ALERT counter
XBYTE	ISL6251ACHICWaitVinCNT	_at_ 0x04D4;	// (byte) For wait Vin ready then Change CHG- signal counter
XBYTE	BatteryOnlyXXPercent	_at_ 0x04D5;	// (byte)
XWORD	ChargingCurrentSMBus	_at_ 0x04D6;	// (word) Battery charging current from SMBus
XBYTE	ChkChg_I_TBL_CNT		_at_ 0x04D8;	// (byte)
//XBYTE							_at_ 0x04D9;	// (byte)
XBYTE	ChkTurnOffSBLANPwrFS345CNT	_at_ 0x04DA;// (byte)
XWORD	ATRICKLE_DUTY_ConstCurrt	_at_ 0x04DB;// (word) First battery trickle charge duty low byte
XBYTE	USBChg_Status			_at_ 0x04DD;	// (byte)						// leo_20150917 USB charging

#ifdef SUPPORT_BQ24780S							// leox20150915 Add for set/clear LEARN mode of charger IC
XBYTE	LearnModeSetCnt			_at_ 0x04DE;	// (byte)
XBYTE	LearnModeClrCnt			_at_ 0x04DF;	// (byte)
#endif

XBYTE	Fan1_Fault_CNT			_at_ 0x04E0;	// (byte)	// CHECK_FAN_FAULT	// David_20120613 add for fan error check
XBYTE	USB_CHARGE_CNT			_at_ 0x04E1;	// (byte)
XBYTE	USB_CB0_CNT				_at_ 0x04E2;	// (byte)
XBYTE	USBWorkingCNT			_at_ 0x04E3;	// (byte) USB Charging/Discharging port Working Count
XBYTE	DelayChkIdleCNT			_at_ 0x04E4;	// (byte) Delay Check IDLE Counter
XBYTE	USBCHARGFLAG			_at_ 0x04E5;	// (byte) USB Charging/Discharging port Function Flags
XBYTE	OverVoltageCNT			_at_ 0x04E6;	// (byte) Debounce for Battery Over Voltage
XBYTE	SMBusAbnormalDATlowCNT	_at_ 0x04E7;	// (byte) SMBus Data Line Abnormal Low Counter
XBYTE	DoPullSMBusCLKlowPulseCNT	_at_ 0x04E8;// (byte) Do PullSMBusCLKlowPulse Counter
XBYTE	CMOSReset_Status		_at_ 0x04E9;	// (byte)
XBYTE	CMOSResetCNT			_at_ 0x04EA;	// (byte)
XBYTE	SYS_MISC2				_at_ 0x04EB;	// (byte) System misc flags
XBYTE	OLD_SELSTATE1			_at_ 0x04EC;	// (byte) Previous selector state
XBYTE	OLD_SELSTATE2			_at_ 0x04ED;	// (byte) Previous selector state
XBYTE	SYSTEMSTATE				_at_ 0x04EE;	// (byte)
XBYTE	LedBackup				_at_ 0x04EF;	// (byte)

//------------------------------------------------------------------------------
// EC Debug Using RAM (0x04F0 ~ 0x04FF)
//------------------------------------------------------------------------------
XBYTE	DBG0					_at_ 0x04F0;	// (byte)	// leox_20110926
XBYTE	DBG1					_at_ 0x04F1;	// (byte)
XBYTE	DBG2					_at_ 0x04F2;	// (byte)
XBYTE	DBG3					_at_ 0x04F3;	// (byte)
XBYTE	DBG4					_at_ 0x04F4;	// (byte)
XBYTE	DBG5					_at_ 0x04F5;	// (byte)
XBYTE	DBG6					_at_ 0x04F6;	// (byte)
XBYTE	DBG7					_at_ 0x04F7;	// (byte)
XBYTE	DBG8					_at_ 0x04F8;	// (byte)
XBYTE	DBG9					_at_ 0x04F9;	// (byte)
XBYTE	DBG10					_at_ 0x04FA;	// (byte)
XBYTE	DBG11					_at_ 0x04FB;	// (byte)
XBYTE	DBG12					_at_ 0x04FC;	// (byte)
XBYTE	DBG13					_at_ 0x04FD;	// (byte)
XBYTE	KeyECScanCode			_at_ 0x04FE;	// (byte)
XBYTE	KeyPosition				_at_ 0x04FF;	// (byte)


//******************************************************************************
// 0x500-0x5FF  OEM RAM 5
//******************************************************************************
// 0x500-0x5FF  For ITE flash utility, EC SPI rom read write function, and/or Ramdebug function.


//******************************************************************************
// 0x600-0x6FF  OEM RAM 6
//******************************************************************************
// 0x600-0x6FF  For Keil C Complier and Ram Code Space (for SPI programming)


//******************************************************************************
// 0x700-0x7FF  OEM RAM 7
//******************************************************************************
// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function ++
XBYTE	IER0_BK					_at_ 0x0700;	// (byte)
XBYTE	IER1_BK					_at_ 0x0701;	// (byte)
XBYTE	IER2_BK					_at_ 0x0702;	// (byte)
XBYTE	IER3_BK					_at_ 0x0703;	// (byte)
// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function --

// leox_20111206 Reset EC later after flash +++	// RESET_EC_LATER
XBYTE	RstLaterMark0			_at_ 0x0704;	// (byte) Reset later mark 0
XBYTE	RstLaterMark1			_at_ 0x0705;	// (byte) Reset later mark 1
// leox_20111206 Reset EC later after flash +++

//XBYTE							_at_ 0x0706;	// (byte)
//XBYTE							_at_ 0x0707;	// (byte)
//XBYTE							_at_ 0x0708;	// (byte)
//XBYTE							_at_ 0x0709;	// (byte)
//XBYTE							_at_ 0x070A;	// (byte)
//XBYTE							_at_ 0x070B;	// (byte)
//XBYTE							_at_ 0x070C;	// (byte)
//XBYTE							_at_ 0x070D;	// (byte)
//XBYTE							_at_ 0x070E;	// (byte)
//XBYTE							_at_ 0x070F;	// (byte)

// leo_20150916 add power protect +++			// BAT_MBATV_PROTECT
XWORD	AC_OC_En_ProchotCnt				_at_ 0x0710;	// (word)
XWORD	AC_OC_Dis_ProchotCnt			_at_ 0x0712;	// (word)
XWORD	AC_OCP_En_ProchotCnt			_at_ 0x0714;	// (word)
XWORD	AC_OCP_Dis_ProchotCnt			_at_ 0x0716;	// (word)
XWORD	DC_OW_En_ProchotCnt				_at_ 0x0718;	// (word)
XWORD	DC_OW_Dis_ProchotCnt			_at_ 0x071A;	// (word)
XWORD	DC_OCP_En_ProchotCnt			_at_ 0x071C;	// (word)
XWORD	DC_OCP_Dis_ProchotCnt			_at_ 0x071E;	// (word)
LWORD	Bat_OverWattThreshold			_at_ 0x0720;	// (dword)
LWORD	Bat_UnderWattThreshold			_at_ 0x0724;	// (dword)
XWORD	Bat_OverPeakCurrentThreshold	_at_ 0x0728;	// (word)
XWORD	Bat_UnderPeakCurrentThreshold	_at_ 0x072A;	// (word)
LWORD	L_DC_W							_at_ 0x072C;	// (dword)
LWORD	L_VMBATV_V						_at_ 0x0730;	// (dword)
LWORD	L_VICM_V						_at_ 0x0734;	// (dword)
//XBYTE							_at_ 0x0730;	// (byte)
//XBYTE							_at_ 0x0731;	// (byte)
//XBYTE							_at_ 0x0732;	// (byte)
//XBYTE							_at_ 0x0733;	// (byte)
//XBYTE							_at_ 0x0734;	// (byte)
//XBYTE							_at_ 0x0735;	// (byte)
//XBYTE							_at_ 0x0736;	// (byte)
//XBYTE							_at_ 0x0737;	// (byte)
// leo_20150916 add power protect ---

//XBYTE							_at_ 0x0738;	// (byte)
//XBYTE							_at_ 0x0739;	// (byte)

#ifdef Multiple_TP	// ken add for touchpad compare
XBYTE	DetectTPID_Ctrl			_at_ 0x073A;	// (byte) Detect touchpad ID command: Control
XBYTE	LastDetectTPID_Cmd		_at_ 0x073B;	// (byte) Detect touchpad ID command: Last command
XBYTE	DetectTPID0				_at_ 0x073C;	// (byte) Detect touchpad ID command: ID0
XBYTE	DetectTPID1				_at_ 0x073D;	// (byte) Detect touchpad ID command: ID1
XBYTE	DetectTPID2				_at_ 0x073E;	// (byte) Detect touchpad ID command: ID2
XBYTE	DetectTPID3				_at_ 0x073F;	// (byte) Detect touchpad ID command: ID3
#endif	// Multiple_TP

//XBYTE							_at_ 0x0740;	// (byte)
//XBYTE							_at_ 0x0741;	// (byte)
//XBYTE							_at_ 0x0742;	// (byte)
//XBYTE							_at_ 0x0743;	// (byte)
//XBYTE							_at_ 0x0744;	// (byte)
//XBYTE							_at_ 0x0745;	// (byte)
//XBYTE							_at_ 0x0746;	// (byte)
//XBYTE							_at_ 0x0747;	// (byte)
//XBYTE							_at_ 0x0748;	// (byte)
//XBYTE							_at_ 0x0749;	// (byte)
//XBYTE							_at_ 0x074A;	// (byte)
//XBYTE							_at_ 0x074B;	// (byte)
//XBYTE							_at_ 0x074C;	// (byte)
//XBYTE							_at_ 0x074D;	// (byte)
//XBYTE							_at_ 0x074E;	// (byte)
//XBYTE							_at_ 0x074F;	// (byte)

//------------------------------------------------------------------------------
// Primary Battery Information Namespaces (0x0750 ~ 0x075F)
//------------------------------------------------------------------------------
XBYTE	ABAT_MODEL				_at_ 0x0750;	// (byte) First battery model
XBYTE	ABAT_STATUS				_at_ 0x0751;	// (byte)
XBYTE	ACHARGE_STATUS			_at_ 0x0752;	// (byte)
XBYTE	ABAT_MISC				_at_ 0x0753;	// (byte)
XBYTE	ABAT_MISC2				_at_ 0x0754;	// (byte)
XWORD	ABAT_ALARM				_at_ 0x0755;	// (word) Smart battery alarm and status
XBYTE	ABAT_CAP				_at_ 0x0757;	// (byte) ABAT batt capacity (RSOC %)
XWORD	ABAT_VOLT				_at_ 0x0758;	// (word) Volatge of aux battery
XWORD	ABAT_AV					_at_ 0x075A;	// (word) ABAT voltage accumulator
XWORD	ABAT_TEMP				_at_ 0x075C;	// (word) Temperature of aux battery
XWORD	ABAT_AT					_at_ 0x075E;	// (word) ABAT temp accumulator

//------------------------------------------------------------------------------
// Secondary Battery Information Namespaces (0x0760 ~ 0x076F)
//------------------------------------------------------------------------------
XBYTE	BBAT_MODEL				_at_ 0x0760;	// (byte) Second battery model
XBYTE	BBAT_STATUS				_at_ 0x0761;	// (byte)
XBYTE	BCHARGE_STATUS			_at_ 0x0762;	// (byte)
XBYTE	BBAT_MISC				_at_ 0x0763;	// (byte)
XBYTE	BBAT_MISC2				_at_ 0x0764;	// (byte)
XWORD	BBAT_ALARM				_at_ 0x0765;	// (word) Smart battery alarm and status
XBYTE	BBAT_CAP				_at_ 0x0767;	// (byte) BBAT batt capacity (RSOC %)
XWORD	BBAT_VOLT				_at_ 0x0768;	// (word) Volatge of aux battery
XWORD	BBAT_AV					_at_ 0x076A;	// (word) BBAT voltage accumulator
XWORD	BBAT_TEMP				_at_ 0x076C;	// (word) Temperature of aux battery
XWORD	BBAT_AT					_at_ 0x076E;	// (word) BBAT temp accumulator

//------------------------------------------------------------------------------
// ADC Data Value RAM (0x0770 ~ 0x077F)
//------------------------------------------------------------------------------
XWORD	ADV_DAT0				_at_ 0x0770;	// (word) Result of A/D channel 0
XWORD	ADV_DAT1				_at_ 0x0772;	// (word) Result of A/D channel 1
XWORD	ADV_DAT2				_at_ 0x0774;	// (word) Result of A/D channel 2
XWORD	ADV_DAT3				_at_ 0x0776;	// (word) Result of A/D channel 3
XWORD	ADV_DAT4				_at_ 0x0778;	// (word) Result of A/D channel 4
XWORD	ADV_DAT5				_at_ 0x077A;	// (word) Result of A/D channel 5
XWORD	ADV_DAT6				_at_ 0x077C;	// (word) Result of A/D channel 6
XWORD	ADV_DAT7				_at_ 0x077E;	// (word) Result of A/D channel 7

//------------------------------------------------------------------------------
// Ext Vector Bounce (0x0780 ~ 0x078F)
//------------------------------------------------------------------------------
XBYTE	EXTEVT_BOUNCE			_at_ 0x0780;	// (byte) Ext event debounce counter

//------------------------------------------------------------------------------
// ABAT Shadow Information (0x0790 ~ 0x079A)
//------------------------------------------------------------------------------
XBYTE	AAUTO_STEP				_at_ 0x0790;	// (byte) ABAT auto learning step control
XBYTE	AAUTO_STS				_at_ 0x0791;	// (byte) ABAT auto learning status
XBYTE	AFAIL_CAUSE				_at_ 0x0792;	// (byte) ABAT failure cause code
XBYTE	AAVERAGE_STEP			_at_ 0x0793;	// (byte) ADC average control of ABAT
//XBYTE							_at_ 0x0794;	// (byte)
XBYTE	ABAT_FAIL_TRY			_at_ 0x0795;	// (byte) Counter for retry get model
XWORD	ABLED1_PAT				_at_ 0x0796;	// (word) ABAT LED1 flash pattern
XWORD	ABLED2_PAT				_at_ 0x0798;	// (word) ABAT LED2 flash pattern
XBYTE	ABAT_BOUNCE				_at_ 0x079A;	// (byte) Debounce timer for first battery

//------------------------------------------------------------------------------
// BBAT Shadow Information (0x079B ~ 0x07A5)
//------------------------------------------------------------------------------
XBYTE	BAUTO_STEP				_at_ 0x079B;	// (byte) BBAT auto learning step control
XBYTE	BAUTO_STS				_at_ 0x079C;	// (byte) BBAT auto learning status
XBYTE	BFAIL_CAUSE				_at_ 0x079D;	// (byte) BBAT failure cause code
XBYTE	BAVERAGE_STEP			_at_ 0x079E;	// (byte) ADC average control of BBAT
XBYTE	BTRICKLE_DUTY			_at_ 0x079F;	// (byte) Second battery trickle charge duty
XBYTE	BBAT_FAIL_TRY			_at_ 0x07A0;	// (byte) Counter for retry get model
XWORD	BBLED1_PAT				_at_ 0x07A1;	// (word) BBAT LED1 flash pattern
XWORD	BBLED2_PAT				_at_ 0x07A3;	// (word) BBAT LED2 flash pattern
XBYTE	BBAT_BOUNCE				_at_ 0x07A5;	// (byte) Debounce timer for second battery

//------------------------------------------------------------------------------
// Battery Protect Information (0x07A6 ~ 0x07AF)
//------------------------------------------------------------------------------
XBYTE	OVERTEMP_COUNT			_at_ 0x07A6;	// (byte) Counter for battery over temperature
XBYTE	Bat_FastChargeCURT		_at_ 0x07A7;	// (byte)
XWORD	CHG_TIMER				_at_ 0x07A8;	// (word) Wakeup charge timer
//XWORD	WAKECHG_TIMER			_at_ 0x07A8;	// (word) Wakeup charge timer
//XWORD	PRECHG_TIMER			_at_ 0x07A8;	// (word) Timer for battery trickle charge
//XWORD	CHARGE_TIMER			_at_ 0x07A8;	// (word) Timer for battery normal charge
XBYTE	BAT_DEAD_CNT			_at_ 0x07AA;	// (byte) Counter for BAT dead qualify
XBYTE	FASTCHG_MASK			_at_ 0x07AB;	// (byte) Detection mask of fast charge
XBYTE	SLOWCHG_MASK			_at_ 0x07AC;	// (byte) Detection mask of slow charge(1 detection enable)
XBYTE	CUTOFF_CAUSE			_at_ 0x07AD;	// (byte) Charger cut-off cause
XBYTE	POLLING_JOB				_at_ 0x07AE;	// (byte) SMB read buffer
XBYTE	BAT_MISC3				_at_ 0x07AF;	// (byte) Flags for battery manage
XWORD	Bat_PreChargeTimeout	_at_ 0x07B0;	// (word)
//XBYTE	Bat_PreChargeCURT		_at_ 0x07B2;	// (byte)
XBYTE	Bat_PowerOnTimerRatio	_at_ 0x07B3;	// (byte)
XBYTE	Bat_PowerOffTimerRatio	_at_ 0x07B4;	// (byte)
XWORD	Bat_ChargeTimeout		_at_ 0x07B5;	// (word)
XWORD	Bat_OverVoltThreshold	_at_ 0x07B7;	// (word)
XWORD	Bat_PreDeadChargeThreshold	_at_ 0x07B9;// (word)
XWORD	ChargingVoltage_Backup	_at_ 0x07BB;	// (word)
XWORD	ChargingCurrent_Backup	_at_ 0x07BD;	// (word)
//XBYTE							_at_ 0x07BF;	// (byte)

//------------------------------------------------------------------------------
// Reboot Mark for Command 64-B4 (0x07C0 ~ 0x07C4)
//------------------------------------------------------------------------------
XBYTE	REBOOT_MARK				_at_ 0x07C0;	// (byte)
XBYTE	REBOOT_MARK2			_at_ 0x07C1;	// (byte)
XBYTE	REBOOT_MARK3			_at_ 0x07C2;	// (byte)
XBYTE	REBOOT_MARK4			_at_ 0x07C3;	// (byte)
//XBYTE							_at_ 0x07C4;	// (byte)

//------------------------------------------------------------------------------
// Normal Debounce (0x07C5 ~ 0x07C9)
//------------------------------------------------------------------------------
XBYTE	POLLING_STEP			_at_ 0x07C5;	// (byte) Counter for polling sequence contorl
XBYTE	PSW_BOUNCE				_at_ 0x07C6;	// (byte) Counter for PSW debounce
XBYTE	AC_BOUNCE				_at_ 0x07C7;	// (byte) Counter for AC debounce
XBYTE	OneButtDebounce			_at_ 0x07C8;	// (byte) Counter for one touched button
XBYTE	BAT_MISC1				_at_ 0x07C9;	// (byte)

//------------------------------------------------------------------------------
// Thermal Protect Information (0x07CA ~ 0x07CF)
//------------------------------------------------------------------------------
XBYTE	CPU_VeryHotCnt			_at_ 0x07CA;	// (byte) CPU Remote too hot counter
XBYTE	CPU_VeryHotCnt_Local	_at_ 0x07CB;	// (byte) CPU Remote too hot counter
XBYTE	VGA_VeryHotCnt			_at_ 0x07CC;	// (byte) VGA Remote too hot counter
XBYTE	VGA_VeryHotCnt_Local	_at_ 0x07CD;	// (byte) VGA Remote too hot counter
XBYTE	ABAT_CELLS				_at_ 0x07CE;	// (byte) Battery cells
XBYTE	BBAT_CELLS				_at_ 0x07CF;	// (byte) Battery cells

//------------------------------------------------------------------------------
// Battery Shadow RAM (0x07D0 ~ 0x07DF)
//------------------------------------------------------------------------------
XBYTE	SEL_BAT_MODEL			_at_ 0x07D0;	// (byte) Selected battery model
XBYTE	SEL_BAT_STATUS			_at_ 0x07D1;	// (byte) Selected battery status
XBYTE	SEL_CHARGE_STATUS		_at_ 0x07D2;	// (byte) Battery charge status
XBYTE	SEL_BAT_MISC			_at_ 0x07D3;	// (byte)
XBYTE	SEL_BAT_MISC2			_at_ 0x07D4;	// (byte)
XWORD	SEL_BAT_ALARM			_at_ 0x07D5;	// (word) Smart battery alarm and status
XBYTE	SEL_BAT_CAP				_at_ 0x07D7;	// (byte) RSOC of selected battery
XWORD	SEL_BAT_VOLT			_at_ 0x07D8;	// (word) Volatge of selected battery
XWORD	SEL_BAT_CURRENT			_at_ 0x07DA;	// (word) ABAT batt current (mA)
XWORD	SEL_BAT_TEMP			_at_ 0x07DC;	// (word) Temperature of selected battery
XWORD	BateryTempSBMus			_at_ 0x07DE;	// (word) Battery temperature from SMBus

//------------------------------------------------------------------------------
// Counter Information (0x07E0 ~ 0x07E6)
//------------------------------------------------------------------------------
//XBYTE							_at_ 0x07E0;	// (byte)
//XBYTE							_at_ 0x07E1;	// (byte)
//XBYTE							_at_ 0x07E2;	// (byte)
//XBYTE							_at_ 0x07E3;	// (byte)
//XBYTE							_at_ 0x07E4;	// (byte)
//XBYTE							_at_ 0x07E5;	// (byte)
//XBYTE							_at_ 0x07E6;	// (byte)

//------------------------------------------------------------------------------
// Normal Timer Information (0x07E7 ~ 0x07FF)
//------------------------------------------------------------------------------
XBYTE	PSW_TIMER				_at_ 0x07E7;	// (byte) Power switch timer counter
XBYTE	HWPG_TIMER				_at_ 0x07E8;	// (byte) Timer for waiting HWPG
XWORD	PSW_COUNTER				_at_ 0x07E9;	// (word) PSW override counter
XBYTE	RETRY_COUNTER			_at_ 0x07EB;	// (byte) Counter for SMB retry
XWORD	LED_FLASH_CNT			_at_ 0x07EC;	// (word) Counter for LED flash control
XBYTE	Q09CNT					_at_ 0x07EE;	// (byte) Delay Send Q-09 Counter
XBYTE	Volume_CNT				_at_ 0x07EF;	// (byte) For POPO noise
//XBYTE							_at_ 0x07F0;	// (byte)

// Add One Touch Buttons function 2011/08/19 +++
XBYTE	OneTouchPollStep		_at_ 0x07F1;	// (byte) One Touch Buttons Polling Step
XBYTE	OneTouchButtStatus		_at_ 0x07F2;	// (byte) One Touch Buttons Status
XBYTE	OneTouchButtStatus2		_at_ 0x07F3;	// (byte) One Touch Buttons Repeat Status
XBYTE	OneTouchButtonDelay		_at_ 0x07F4;	// (byte) One Touch Buttons Counter
// Add One Touch Buttons function 2011/08/19 ---

XWORD	Bat_PreChargeCURT		_at_ 0x07F5;	// (word)	// leo_20160726 move from 0x07B2
//XBYTE							_at_ 0x07F7;	// (byte)
//XBYTE							_at_ 0x07F8;	// (byte)
//XBYTE							_at_ 0x07F9;	// (byte)
XBYTE	CPU_RemoteErrorCnt		_at_ 0x07FA;	// (byte) CPU Remote Error Counter
XBYTE	VGA_RemoteErrorCnt		_at_ 0x07FB;	// (byte) VGA Remote Error Counter
//XBYTE							_at_ 0x07FC;	// (byte)
//XBYTE							_at_ 0x07FD;	// (byte)
//XBYTE							_at_ 0x07FE;	// (byte)
//XBYTE							_at_ 0x07FF;	// (byte)


//******************************************************************************
// 0x800-0x8FF  OEM RAM 8
//******************************************************************************
// 0x800-0x8FF  Ramdebug function


//******************************************************************************
// 0x900-0x9FF  OEM RAM 9
//******************************************************************************
XBYTE	SMB_DATA_OEM			_at_ 0x0900;	// (byte) EC SMB1 Data register array (32 bytes)
XBYTE	SMB_DATA1_OEM			_at_ 0x0901;	// (byte)
// Reserved offset 0x0902 ~ 0x091F for EC SMB1 data buffer

XBYTE	SPI_Mouse_Buf[3]		_at_ 0x0920;	// (byte)
XBYTE	SPI_Mouse_DeltaX		_at_ 0x0923;	// (byte)
XBYTE	SPI_Mouse_DeltaY		_at_ 0x0924;	// (byte)
XBYTE	SPI_Mouse_BYTE1			_at_ 0x0925;	// (byte)
XBYTE	SPI_Mouse_XMovement		_at_ 0x0926;	// (byte)
XBYTE	SPI_Mouse_YMovement		_at_ 0x0927;	// (byte)
XBYTE	SPI_Mouse_Buf_Index		_at_ 0x0928;	// (byte)
XBYTE	SPI_Mouse_Misc			_at_ 0x0929;	// (byte)
XBYTE	SPI_Mouse_ID			_at_ 0x092A;	// (byte)
// Reserved offset 0x092B ~ 0x092F

#ifdef PECI_Support
XBYTE	TDIM0_temp				_at_ 0x0938;	// (byte) DRAM 0 temperature
XBYTE	TDIM1_temp				_at_ 0x0939;	// (byte) DRAM 1 temperature
XBYTE	TPkg_temp				_at_ 0x093A;	// (byte)
XBYTE	TVGA_temp				_at_ 0x093B;	// (byte) VGA Temperature
XBYTE	TPCH_temp				_at_ 0x093C;	// (byte) PCH Temperature
XBYTE	TSys_temp				_at_ 0x093D;	// (byte) System Temperature
XBYTE	TCPU_temp				_at_ 0x093E;	// (byte) CPU Temperature
XBYTE	PECI_CPU_temp			_at_ 0x093F;	// (byte) Use PECI interface to read CPU temperature
XBYTE	PECIReadBuffer[16]		_at_ 0x0940;	// (byte)
XBYTE	PECIWriteBuffer[16]		_at_ 0x0950;	// (byte)
XBYTE	PECI_CRC8				_at_ 0x0960;	// (byte)
XBYTE	PECI_CompletionCode		_at_ 0x0961;	// (byte)
XBYTE	PECI_PCIConfigAddr[4]	_at_ 0x0962;	// (byte)
XBYTE	PECI_PowerCtrlStep		_at_ 0x0970;	// (byte)
XBYTE	PECI_PowerUnit			_at_ 0x0971;	// (byte)
XBYTE	PECI_EnergyUnit			_at_ 0x0972;	// (byte)
XBYTE	PECI_TimeUnit			_at_ 0x0973;	// (byte)
XBYTE	PECI_PL2Watts			_at_ 0x0974;	// (byte)
XBYTE	PECI_MAXPL2				_at_ 0x0975;	// (byte)
XBYTE	PECI_PL1TDP				_at_ 0x0976;	// (byte)
XBYTE	PECI_PL1Time			_at_ 0x0977;	// (byte)
XBYTE	PECI_Ctrl				_at_ 0x0978;	// (byte)
XBYTE	PECI_AdaptorWatts		_at_ 0x0979;	// (byte)
XBYTE	PECI_SystemWatts		_at_ 0x097A;	// (byte)
XBYTE	PECI_Algorithm_Delay	_at_ 0x097B;	// (byte)
LWORD	PECI_ErrorCount			_at_ 0x097C;	// (dword)
XBYTE	PECI_StressToolBuf[32]	_at_ 0x0980;	// (byte)
XBYTE	PSTB_Timer				_at_ 0x09A0;	// (byte)
XBYTE	PSTB_CmdStatus			_at_ 0x09A1;	// (byte)
XBYTE	PSTB_RepeatIntervalCount	_at_ 0x09A2;// (byte)
LWORD	PSTB_RepeatCycle		_at_ 0x09A3;	// (dword)
XBYTE	CPUTjmax				_at_ 0x09A7;	// (byte)
XBYTE	ReadCPUTjmaxCUNT		_at_ 0x09A8;	// (byte)
XBYTE	StressToolReadBufIndex	_at_ 0x09A9;	// (byte)
// Reserved offset 0x09AA ~ 0x09AF
#endif	//PECI_Support

// David_20120106 modify for vOSSMbus/vOSSMbus_OEM routine
XBYTE	SMB_ADDR_OEM			_at_ 0x09B0;	// (byte)
XBYTE	SMB_CMD_OEM				_at_ 0x09B1;	// (byte)
XBYTE	SMB_BCNT_OEM			_at_ 0x09B2;	// (byte)	// leox_20120711 Add for SMBus block write
// David_20120106 modify for vOSSMbus/vOSSMbus_OEM routine

XBYTE	CIR_NEC_Act				_at_ 0x09B3;	// (byte)						// Ken_20120605 CIR_NEC_IMPROVED
XBYTE	Converter_Config_Status	_at_ 0x09B4;	// (byte)	// Converter configuartion status	// leox20150630 0x09E0 -> 0x09B4
// Reserved offset 0x09B5 ~ 0x09BF

// leox20150630 Access eFlash +++		// leox20151030 SUPPORT_EFLASH_FUNC		// leox20160322 Code from N83
XBYTE eFL_Buf[4]				_at_ 0x09C0;	// (byte) eFlash buffer
XBYTE eFL_Adr[4]				_at_ 0x09C4;	// (byte) eFlash address
XBYTE eFL_Act					_at_ 0x09C8;	// (byte) eFlash action
XBYTE eFL_Len					_at_ 0x09C9;	// (byte) eFlash access size
XWORD eFL_Blk					_at_ 0x09CA;	// (word) eFlash block number
XWORD eFL_Idx					_at_ 0x09CC;	// (word) eFlash block index
XBYTE eFL_ToDo					_at_ 0x09CE;	// (byte) eFlash to do			// leox20160316
//XBYTE							_at_ 0x09CF;	// (byte)
// leox20150630 Access eFlash ---
// Reserved offset 0x09D0 ~ 0x09F7

//David add for MMB service 2014/05/21			// leox20160321 Move from 0x09C0-0x09C5 to 0x09F8-0x09FD
XBYTE current_page				_at_ 0x09F8;	// (byte)
XBYTE convn_setting				_at_ 0x09F9;	// (byte)
XBYTE SLD_level[2]				_at_ 0x09FA;	// (byte)
XBYTE last_contact				_at_ 0x09FC;	// (byte)
XBYTE CapButtonStat				_at_ 0x09FD;	// (byte)
//David add for MMB service 2014/05/21

//XBYTE							_at_ 0x09FE;	// (byte)

XBYTE	Factory_MMBTest_status	_at_ 0x09FF;	// (byte)	//David add for factory test 2014/07/24


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
XBYTE	BaseShutdownCause[254]	_at_ 0x0C00;	// (byte)
XBYTE	LastShutdownCause		_at_ 0x0CFE;	// (byte)
XBYTE	NextShutdownCauseIndex	_at_ 0x0CFF;	// (byte)


//******************************************************************************
// 0xD00-0xDFF  OEM RAM D
//******************************************************************************
// 0xD00-0xDFF  Log Port 80h to RAM for LogP80()
XBYTE	BaseP80[254]			_at_ 0x0D00;	// (byte)
XBYTE	LastP80					_at_ 0x0DFE;	// (byte)
XBYTE	NextP80Index			_at_ 0x0DFF;	// (byte)


//******************************************************************************
// 0xE00-0xEFF  OEM RAM E
//------------------------------------------------------------------------------
// 0xE00-0xEFF  OEM RAM E --> All not use


//******************************************************************************
// 0xF00-0xFFF  OEM RAM F
//******************************************************************************
// 0xF00-0xFFF  For HSPI ram code function


//******************************************************************************
// Battery-backed SRAM (BRAM) (0x2200 ~ 0x22BF)
//******************************************************************************
// Chip_8512: (0x2200 ~ 0x223F) = (0x2280 ~ 0x22BF) = ( 64 bytes)
// Chip_8518: (0x2200 ~ 0x227F) + (0x2280 ~ 0x22BF) = (192 bytes)

//------------------------------------------------------------------------------
// BRAM Bank 0 (0x2200 ~ 0x227F) (128 bytes)
//------------------------------------------------------------------------------
//XBYTE P80LB_BRAM				_at_ 0x2200;	// (byte) Reserved for P80
XBYTE	CheckBramMask01			_at_ 0x2201;	// (byte) For check if first backup battery power-up mask00
XBYTE	CheckBramMask02			_at_ 0x2202;	// (byte) For check if first backup battery power-up mask01
XBYTE	USBCHARGEDISCHARG_BRM	_at_ 0x2203;	// (byte) For project special function
XBYTE	B_PwrLoss				_at_ 0x2204;	// (byte) For power loss function	// leox_20110921 OEM_POWER_LOSS
XBYTE	B_PwrCtrl				_at_ 0x2205;	// (byte) For power control			// leox_20120203 ENTER_SLEEP_MODE_AC
XBYTE	Shipmode_status			_at_ 0x2206;	// (byte) For ship mode function
//XBYTE							_at_ 0x2207;	// (byte)

//David add for G3 keep status 2014/07/24		// IT_7230MMB_SUPPORT
XBYTE	MMB_WakeUp_S3_BRM		_at_ 0x2208;	// (byte)
XBYTE	MMB_WakeUp_S4_BRM		_at_ 0x2209;	// (byte)
XBYTE	MMB_WakeUp_S5_BRM		_at_ 0x220A;	// (byte)
XBYTE	POWER_STAT_CTRL1_BRM	_at_ 0x220B;	// (byte)
//David add for G3 keep status 2014/07/24

//XBYTE							_at_ 0x220C;	// (byte)
//XBYTE							_at_ 0x220D;	// (byte)
XBYTE	B_AppSwitchFnFx			_at_ 0x220E;	// (byte) For switch Fn+Fx function set by application	// leox_20120314 SUPPORT_OEM_APP
// 0x2210 ~ 0x223B not used yet

//reboot for IFU.exe tool ++
//XBYTE	BootMask_Bram_00		_at_ 0x223C;	// (byte)
//XBYTE	BootMask_Bram_01		_at_ 0x223D;	// (byte)
//XBYTE	BootMask_Bram_02		_at_ 0x223E;	// (byte)
//XBYTE	BootMask_Bram_03		_at_ 0x223F;	// (byte)
//reboot for IFU.exe tool --

//------------------------------------------------------------------------------
// BRAM Bank 1 (0x2280 ~ 0x22BF) (64 bytes)
//------------------------------------------------------------------------------
XBYTE	P80LB_BRAM				_at_ 0x2280;	// (byte)
//XBYTE	CheckBramMask01			_at_ 0x2281;	// (byte) For check if first backup battery power-up mask00
//XBYTE	CheckBramMask02			_at_ 0x2282;	// (byte) For check if first backup battery power-up mask01
//XBYTE							_at_ 0x2283;	// (byte)
//XBYTE							_at_ 0x2284;	// (byte)
//XBYTE							_at_ 0x2285;	// (byte)
//XBYTE							_at_ 0x2286;	// (byte)
//XBYTE							_at_ 0x2287;	// (byte)
//XBYTE							_at_ 0x2288;	// (byte)
//XBYTE							_at_ 0x2289;	// (byte)
//XBYTE							_at_ 0x228A;	// (byte)
//XBYTE							_at_ 0x228B;	// (byte)
//XBYTE							_at_ 0x228C;	// (byte)
//XBYTE							_at_ 0x228D;	// (byte)
//XBYTE							_at_ 0x228E;	// (byte)
// 0x2290 ~ 0x22AB not used yet

// 120731 chris add for fix flash EC by WINIFU then shutdown then boot fail +++
XBYTE	BIOS_FLASH_Reset00		_at_ 0x22AC;	// (byte)
XBYTE	BIOS_FLASH_Reset01		_at_ 0x22AD;	// (byte)
XBYTE	BIOS_FLASH_Reset02		_at_ 0x22AE;	// (byte)
XBYTE	BIOS_FLASH_Reset03		_at_ 0x22AF;	// (byte)
// 120731 chris add for fix flash EC by WINIFU then shutdown then boot fail ---

XBYTE	B_SdCause[SdCauseBRAMSize]	_at_ 0x22B0;// (byte) Log shutdown code		// leox_20111209 LogShutdownCauseInBRAM

//XBYTE							_at_ 0x22B8;	// (byte)
//XBYTE							_at_ 0x22B9;	// (byte)
//XBYTE							_at_ 0x22BA;	// (byte)
//XBYTE							_at_ 0x22BB;	// (byte)

//reboot for IFU.exe tool ++
XBYTE	BootMask_Bram_00		_at_ 0x22BC;	// (byte)
XBYTE	BootMask_Bram_01		_at_ 0x22BD;	// (byte)
XBYTE	BootMask_Bram_02		_at_ 0x22BE;	// (byte)
XBYTE	BootMask_Bram_03		_at_ 0x22BF;	// (byte)
//reboot for IFU.exe tool --
