/*------------------------------------------------------------------------------
 * Title : OEM_INIT.C
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//------------------------------------------------------------------------------
// Internal Function Declaration
//------------------------------------------------------------------------------
void Init_Regs(void);
void Init_PS2Port(void);
void HookInit(void);
void OemInitGPIO(void);	// leox_20110926 From "OEM_MAIN.H"
void Init_OEMVariable(void);
void CheckRebootMark(void);
void CheckPowerLoss(void);	// leox_20110921 OEM_POWER_LOSS	// leox_20110926 From "OEM_MAIN.H"


//------------------------------------------------------------------------------
// Initialize the registers and data variables before kernel initial function
//------------------------------------------------------------------------------
void Oem_InitializationL(void)
{
	// leox_20110926 From main() of "CORE_MAIN.C"	// leox_20111206 From Oem_InitializationH() +++
	if ((REBOOT_MARK  == 165) && (REBOOT_MARK2 == 100) &&
		(REBOOT_MARK3 == 200) && (REBOOT_MARK4 == 50))
	{
		VOLMUTE_on();		// leox_20110926 Refer to ASM code
		ShutDnCause = 0xFB;	// leox_20110926
		ForceOffPower();
		DelayXms(0xFF);		// leox_20110926 Refer to ASM code
	}
	// leox_20110926 From main() of "CORE_MAIN.C"	// leox_20111206 From Oem_InitializationH() ---

//	Init_ClearRam();	// It had been done in "STARTUP.A51"
	ChkEcRamValid = 0x55AA;	// leox20160317 Check EC RAM

	Init_ClearBram();	// leox_20110926 From OemSetDefaultVariables()

	Init_GPIO();

	#if PWRSW_WDT_1		// leox20150724 FH9C
	ShutDnCause = 0x00;	// leox20150728
	if (IsFlag1(SPECTRL4, LRSIPWRSWTR))
	{
		SetFlag(SPECTRL4, LRSIPWRSWTR);	// Write 1 to clear this bit
		ShutDnCause = 0xFC;				// leox20150728 It will be saved of Init_OEMVariable()
		while (NBSWON_Status_Low()) {}	// Wait power button release
	}
	#endif

	#ifdef	HSPI
	GPIO_HSPI_INIT();
		#if	HSPI_OFF_WITH_VCC	// leox_20120112
	Enable_HSPI();
		#endif
	#endif

	#ifdef	SPIReadMode
	ChangeSPIFlashReadMode(SPIReadMode);
	#endif

	#ifdef	HSRSMode
	ChangeHSRSMode(HSRSMode);
	#endif

	CLR_BIT(FLHCTRL2R, 3);

	Init_EC_Indirect_Selection(SPI_selection_internal);	//@Jay20151125

	#ifdef	PLLFrequency
	ChangePLLFrequency(PLLFrequency);
	#endif

	#ifdef	PECI_Support
	Init_PECI();
	#endif

	Init_Regs();

	#ifdef	SMBusChannel3Support	// leox_20131218
	InitSMBusChannel3();			// leox_20131218
	#endif
	Init_SMBus_Regs();
	//SET_MASK(SLVISELR, OVRSMDBG);	// 120614 chris add for not into debug mode for SMBus error	// leox_20120810

	#if	CIRFuncSupport
	Init_CIR();
	#endif

	Init_Cache();

	Init_PS2Port();

	#ifdef	SysMemory2ECRam
	InitSysMemory2ECRam();
	#endif

	Init_ADC();	// leox_20110926 From OemSetDefaultVariables()

	HookInit();	// leox_20110926 From Init_OEMVariable()
}


//------------------------------------------------------------------------------
// Initialize the registers and data variables after kernel initial function
//------------------------------------------------------------------------------
void Oem_InitializationH(void)
{
	#if WAIT_PCH_STABLE_MIRROR	// leox20150807
	if (WaitS5ToDoWDT == 0x99)	// leox20150721 Wait PCH chip stable after mirror code
	{
		WaitS5ToDoWDT = 0;
		for (ITempB06 = 0; ITempB06 < 20; ITempB06++) {DelayXms(100);}	// 2 sec (timebase: 100ms)
	}
	#endif

	#if SUPPORT_EFLASH_FUNC		// leox20150701 Access eFlash	// leox20150702
	eFlashInit();
	#endif

	// leox_20110926 From OemSetDefaultVariables() +++
	OemInitGPIO();
	#ifdef	OEM_IPOD
	USBHookInit();
	#endif	//OEM_IPOD
	// leox_20110926 From OemSetDefaultVariables() ---

	Init_OEMVariable();	// leox_20110926 From Oem_InitializationL()

	CheckRebootMark();	// leox_20110926

	// leox_20110926 From OemSetDefaultVariables() +++
	#if	OEM_POWER_LOSS	// leox_20110921
	CheckPowerLoss();
	#endif

	//David add for MMB service 2014/05/21
	#ifdef IT_7230MMB_SUPPORT
	resetIT7230();
	#endif
	//David add for MMB service 2014/05/21

	#ifdef SUPPORT_BQ24780S	// leox20150605
	BQ24780S_Init();
	#endif
	// leox_20110926 From OemSetDefaultVariables() ---
}


//------------------------------------------------------------------------------
// Initial registers
//------------------------------------------------------------------------------
const sREG_INIT_DEF code reg_init_table[] =
{
	#ifdef	DisableDcache
	// 256 bytes cache
	{&DCache	, 0x03			},
	#endif

	// Flash memory size select register
	{&FMSSR		, Init_FMSSR	},	// Share ROM size

	// HOST interface
	{&SPCTRL1	, Init_I2EC		},	// Enable I2EC R/W
	{&BADRSEL	, Init_BADR		},	// Base address select, 0b00:2E/2F, 0b01:4E/4F, 0b10:User define
	{&SWCBALR	, Init_SWCBALR	},
	{&SWCBAHR	, Init_SWCBAHR	},
    #if Support_IT8987              // Leo_20141111 8987 RSTS no support bit7-6 = 0 1
    {&RSTS      , 0x07          },  // leo_20141112 BIT0-1 = 1 1 check with Jay
    #else   // (original)
    {&RSTS      , 0x84          },
    #endif

	// PWRSW WDT
	#if PWRSW_WDT_1						// leox20150724 FH9C
	{&GCR11		, PWRSW_WDT_1_CNT},		// Set counter first
	{&GCR9		, PWRSW1EN2},			// PWRSW WDT 1 Enable 2
	#endif

	// KBC and PM interface
	{&KBIRQR	, 0x00			},		// Disable KBC IRQ
	{&KBHICR	, IBFCIE + PM1ICIE},	// KBC port control IBFCIE+PMICIE
	{&PM1CTL	, IBFIE + SCINP	},		// EC port control  IBFCIE+SCI low active
	{&PM2CTL	, IBFIE + SCINP	},		// EC port2 control IBFCIE+SCI low active
	#ifdef	PMC3_Support
	{&PM3CTL	, IBFIE			},		// PMC3 input buffer full interrupt enable
	#endif

	// PS2
	{&PSCTL1	, PS2_InhibitMode},	// Enable Debounce, Receive mode, Inhibit CLK
	{&PSCTL2	, PS2_InhibitMode},	// Enable Debounce, Receive mode, Inhibit CLK
	{&PSCTL3	, PS2_InhibitMode},	// Enable Debounce, Receive mode, Inhibit CLK
	{&PSINT1	, 0x06			},	// Transation Done, Start Clock Interrupt
	{&PSINT2	, 0x06			},	// Transation Done, Start Clock Interrupt
	{&PSINT3	, 0x06			},	// Transation Done, Start Clock Interrupt

	// Key Scan
	{&KSOCTRL	, KSOOD + KSOPU	},
	{&KSICTRL	, KSIPU			},

	// ADC
	{&ADCSTS	, AINITB		},	// Enable Analog accuracy initialization
//	{&ADCSTS	, 0				},	// Stop adc accuracy initialization
	{&ADCSTS	, 0x80			},	// Stop adc accuracy initialization
	{&ADCCFG	, 0				},	//
	{&KDCTL		, AHCE			},	// Enable Hardware Callibration
	{&ADCCFG	, DFILEN		},	// Enable Digital Filter enable,
	{&ADCCTL	, 0x15			},

	// DAC
	{&DACPWRDN	, 0x00			},

	// PWM
	{&ZTIER		, 0x00			},	// Disable
	{&CTR		, 0xC8			},	// Set CTR = 200 Duty Cycle output = (DCRi)/(CTR+1)
//	{&C0CPRS	, 0x2D			},	// Beep F =1KHz
	{&C4CPRS	, 0xE3			},	//
	{&C4MCPRS	, 0x00			},	//
	{&C6CPRS	, 0x01			},	// F =23KHz
	{&C6MCPRS	, 0x00			},	//
	{&C7CPRS	, 0xE3			},	// Brightness = 200Hz
	{&C7MCPRS	, 0x00			},	//
	{&PCFSR		, 0x8F			},	// Bit7=1: Disable CR256 channel 1 output. Bit0-bit3: 0 => select 32.768KHz, 1=> select EC clock frequency
	{&PCSSGL	, 0x00			},	// Select channel 0-3 Group
	{&PCSSGH	, 0x20			},	// Select channel 4-7 Group
	{&DCR2		, 0x00			},	// Default
	{&DCR3		, 0x00			},	// Default
//	{&DCR7		, 0x00			},	// Default
	{&ZTIER		, 0x02			},	// Enable
};


void Init_Regs(void)
{
	BYTE index = 0x00;
	while (index < (sizeof(reg_init_table) / sizeof(sREG_INIT_DEF)))
	{
		Tmp_XPntr = reg_init_table[index].address;
		*Tmp_XPntr = reg_init_table[index].initdata;
		index++;
	}
}


//------------------------------------------------------------------------------
// Initial registers - SMBus
//------------------------------------------------------------------------------
const sREG_INIT_DEF code Init_SMBus_table[] =
{
	{&SMB4P7USL		, 0x28},	// 100K
	{&SMB4P0USH		, 0x25},
	{&SMB300NS		, 0x03},
	{&SMB250NS		, 0x02},
	{&SMB25MS		, 0x19},
	{&SMB45P3USL	, 0xA5},
	{&SMB45P3USH	, 0x01},
	{&SMB4P7A4P0H	, 0x00},

	{&HOCTL2_A	, 0x01},
	{&HOCTL_A	, 0x03},
	{&HOCTL_A	, 0x01},
	{&HOSTA_A	, 0xFF},

	{&HOCTL2_B	, 0x01},
	{&HOCTL_B	, 0x03},
	{&HOCTL_B	, 0x01},
	{&HOSTA_B	, 0xFF},

	{&HOCTL2_C	, 0x01},
	{&HOCTL_C	, 0x03},
	{&HOCTL_C	, 0x01},
	{&HOSTA_C	, 0xFF},

    {&SCLKTS_A  , 0x10},
    {&SCLKTS_B  , 0x10},
    {&SCLKTS_C  , 0x10},
    {&SCLKTS_D  , 0x10},

	#ifdef	SMBusChannel3Support	// leox_20131218
	{&HOCTL2_D	, 0x01},
	{&HOCTL_D	, 0x03},
	{&HOCTL_D	, 0x01},
	{&HOSTA_D	, 0xFF},
	#endif
};


void Init_SMBus_Regs(void)
{
	BYTE index = 0x00;
	while (index < (sizeof(Init_SMBus_table) / sizeof(sREG_INIT_DEF)))
	{
		Tmp_XPntr = Init_SMBus_table[index].address;
		*Tmp_XPntr = Init_SMBus_table[index].initdata;
		index++;
	}
}


//------------------------------------------------------------------------------
// Clear internal and external RAM
//------------------------------------------------------------------------------
void Init_ClearRam(void)
{
	IIBYTE *IdataIndex;
	PORT_BYTE_PNTR byte_register_pntr;

	byte_register_pntr = 0;
	while (byte_register_pntr < 0x1000)	// Clear external RAM (0x000~0xFFF)
	{
		*byte_register_pntr = 0;
		byte_register_pntr++;
	}

	IdataIndex = 0x20;
	while (IdataIndex < 0xD0)			// Clear internal RAM (0x20~0xCF)
	//while (IdataIndex < 0x100)		// Clear internal RAM (0x20~0xFF)
	{
		*IdataIndex = 0;
		IdataIndex++;
	}
}


//------------------------------------------------------------------------------
// Clear Battery-backed SRAM (BRAM)
//------------------------------------------------------------------------------
// leox_20110926 From "OEM_MAIN.C"
void Init_ClearBram(void)
{
	PORT_BYTE_PNTR byte_register_pntr;

	if ((CheckBramMask01 != 0x33) || (CheckBramMask02 != 0x33))
	{	// Backup battery is first power-up (Reset all values of BRAM)
		CheckBramMask01 = 0x33;
		CheckBramMask02 = 0x33;
		byte_register_pntr = 0x2203;
		#ifdef	chip_8512_Bram
		while (byte_register_pntr < 0x2240)	// Clear bram ram (0x2203~0x223F)	// 8502/8512
		#else	//chip_8512_Bram
		while (byte_register_pntr < 0x22C0)	// Clear bram ram (0x2203~0x22BF)	// 8518/8519
		#endif	//chip_8512_Bram
		{
			*byte_register_pntr = 0;
			byte_register_pntr++;
		}
		#if (!SUPPORT_EFLASH_FUNC)			// leox20160322
		Init_SetBramDefVal();
		#endif
	}
	else
	{	// Backup battery is not first power-up
		// Do nothing
	}
}


//----------------------------------------------------------------------------
// Set BRAM default value	// CHECK it by project demand
//----------------------------------------------------------------------------
// leox20151104 Create function and refer the code of OemSetDefaultVariables()
// leox20160322 Code from N83
void Init_SetBramDefVal(void)
{
}


//------------------------------------------------------------------------------
// DMA for Scratch SRAM
//------------------------------------------------------------------------------
const sDMAScratchSRAM code asDMAScratchSRAM[] = // For 8500 Dino
{
	{&SCRA1H, &SCRA1M, &SCRA1L},	// 1024 bytes (Externl RAM 0x800 ~ 0xBFF)
	{&SCRA2H, &SCRA2M, &SCRA2L},	//  512 bytes (Externl RAM 0xC00 ~ 0xDFF)
	{&SCRA3H, &SCRA3M, &SCRA3L},	//  256 bytes (Externl RAM 0xE00 ~ 0xEFF)
	{&SCRA4H, &SCRA4M, &SCRA4L},	//  256 bytes (Externl RAM 0xF00 ~ 0xFFF)
};


void CacheDma(BYTE sramnum, WORD addr)
{
	*asDMAScratchSRAM[sramnum].scarh = 0x80;
	*asDMAScratchSRAM[sramnum].scarm = (WORD)(addr >> 8);	// High byte of function address
	*asDMAScratchSRAM[sramnum].scarl = (WORD)(addr & 0xFF);	// Low byte of function address
	*asDMAScratchSRAM[sramnum].scarh = 0x00;				// Start cache DMA
}


//------------------------------------------------------------------------------
// Initial DMA for Scratch SRAM
//------------------------------------------------------------------------------
void Init_Cache(void)
{
	#ifdef	ITE8500				// Dino for 8500 no cache RAM 0x800 ~ 0xFFF
	// Do nothing
	#else
		#ifdef	HSPI
			#ifdef	HSPI_DefferingMode
	CacheDma(3, HSPI_RamCode);	// Cache to memory 0xF00 ~ 0xFFF
			#endif
		#endif
	#endif
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}


//------------------------------------------------------------------------------
// Select touch pad, mouse, and keyboard port number
//------------------------------------------------------------------------------
void Init_PS2Port(void)
{

}


//------------------------------------------------------------------------------
// Initial ADC
//------------------------------------------------------------------------------
// leox_20110926 From "OEM_MAIN.C"
void Init_ADC(void)
{
	#if BAT_MBATV_PROTECT	// leo_20150916 add power protect	// leox20150916
	VCH1CTL = 5;	// ADV_DAT5 dw  rADC1 + 5	// ADC5 MBATV
	#else	// (original)
	VCH1CTL = 0;	// ADV_DAT0 dw  rADC1 + 0
	#endif
	VCH2CTL = 1;	// ADV_DAT1 dw  rADC1 + 1	// ADC1 ICM (aka I_ADP)
	VCH3CTL = 2;	// ADV_DAT2 dw  rADC1 + 2	// ADC2 I_DCHG

	SetFlag(ADCSTS, BIT3);	// Enable analog accuracy initialization
	SetFlag(KDCTL, BIT7);
	SetFlag(ADCCFG, BIT5);
	ClrFlag(ADCSTS, BIT3);

	ADCCFG = 0;				// ADC module disable
	SetFlag(ADCCFG, BIT0);	// ADC module enable
}


void HookInit(void)
{
	// leox_20110926 From Oem_InitializationH() +++
	if (ExtendScanPin == 0x02)	// KSO16 and KSO17 are used
	{
		#if	ExternMatrixGPO	// leox_20111129
		ExtendScanPin++;
		#endif
	}

	#ifdef	ITE_EVBoard
	CLR_MASK(KBHISR, SYSF);
	Ccb42_SYS_FLAG = 0;
	SET_BIT(GPDRD, 3);		// PD.3 SCI
	SET_BIT(GPDRB, 5);		// PB.5 A20
	SET_BIT(GPDRB, 6);		// PB.6 KB_RST
	InitSio();

	GPCRF0 = ALT;
	GPCRF1 = ALT;
	GPCRF2 = ALT;
	GPCRF3 = ALT;
	GPCRF4 = ALT;
	GPCRF5 = ALT;			// Enable all ps2 interface

	GPCRB3 = ALT + PULL_UP;
	GPCRB4 = ALT + PULL_UP;
	GPCRC1 = ALT + PULL_UP;
	GPCRC2 = ALT + PULL_UP;
	GPCRF6 = ALT + PULL_UP;
	GPCRF7 = ALT + PULL_UP;	// Enable SMBus channel A ~ C
	#endif	// ITE_EVBoard
	// leox_20110926 From Oem_InitializationH() ---
	
	WUESR10 = BIT3;
	ISR13 = BIT0;
	SET_BIT(IER13, 0);		// Enable INT104 for PCH_LVDS	// leo_20160420 add for cost down by EE Jackson wu
	
	
	WUESR12 = BIT6;
	ISR15 = BIT3;
	SET_BIT(WUEMR12, 6);	// set falling-edge trigger
	SET_BIT(IER15, 3);		// Enable INT123 for SLP_S3#	// leo_20160425 add for cost down by EE Jackson wu
	
	WUESR2 = BIT0;
	ISR0 = BIT1;
	SET_BIT(WUEMR2, 0);		// set falling-edge trigger
	SET_BIT(IER0, 1);		// Enable INT1 for SLP_S4#		// leo_20160425 add for cost down by EE Jackson wu
}


//------------------------------------------------------------------------------
// OEM initial GPIO
//------------------------------------------------------------------------------
// leox_20110926 From "OEM_MAIN.C"
void OemInitGPIO(void)
{
	#ifdef	UART1_SUPPORT
	SET_BIT(GCR1, 0);	// leox_20110926 BIT0 -> 0
	SET_BIT(GCR1, 1);	// leox_20110926 BIT1 -> 1
	#endif	//UART1_SUPPORT

	#ifdef	UART2_SUPPORT
	SET_BIT(GCR1, 2);	// leox_20110926 BIT2 -> 2
	SET_BIT(GCR1, 3);	// leox_20110926 BIT3 -> 3
	#endif	//UART2_SUPPORT

	if (AC_IN_read())	// leox20150707
	{
		DISCHG_ON_off();	// AC
	}
	else
	{
		DISCHG_ON_on();		// DC
	}
}


//------------------------------------------------------------------------------
// Reset OEM variables
//------------------------------------------------------------------------------
// leox_20110926 From OemSetDefaultVariables()
void Init_OEMVariable(void)
{
	#if PWRSW_WDT_1		// leox20150728
	if (ShutDnCause != 0xFC) {ShutDnCause = 0xFA;}
	#else	// (original)
	ShutDnCause = 0xFA;
	#endif
	LogShutdownCause();

	GetJumpper();
	LoadEcVer();	// leox20150721 For check EC version

	VOLMUTE_on();

	Led_Data = 0x07;
	OEM_Write_Leds(Led_Data);

	LED_FLASH_CNT = 0x0001;	// For LED control
	SetFlag(POWER_FLAG2, func_LED);

	SetFlag(Device_STAT_CTRL,Lan_Status);		// Default Setting WL LED on
	SetFlag(Device_STAT_CTRL,BT_Status);		// Default Setting Bluetooth on
	SetFlag(Device_Setup_Ctrl, Lan_Setup_Status);	// Default Setting Bluetooth on
	SetFlag(Device_Setup_Ctrl, BT_Setup_Status);	// Default Setting Bluetooth on
	SetFlag(Device_STAT_CTRL1,ThreeG_Status);	// Default Setting 3G on
	SetFlag(Device_STAT_CTRL1, CCD_Status);		// Dafault Setting CCD enable
	SetFlag(Device_STAT_CTRL1, CCD_Kill_En);	// Dafault Setting CCD enable

	SetFlag(POWER_FLAG, PwrOffRVCCOff);
	ClrFlag(POWER_FLAG, ACPI_OS);


	//David add for MMB wake up default enable 2014/07/02
	MMB_WakeUp_S5 = 0x07;
	MMB_WakeUp_S4 = 0x07;
	MMB_WakeUp_S3 = 0x07;
	//David add for MMB wake up default enable 2014/07/02

	RSOC_FULL_BASE = 90;
	ChargingVoltage_Backup = 12400;
	ChargingCurrent_Backup = 1000;
	SetFlag(Hybrid_Status, Hybrid_En);
	BrightnessLevel = MaxLevel;
	BrightnessValue = CTR;	// leox_20110917
	POWER_STAT_CTRL1 |= (POWER_STAT_CTRL1_BRM & ECO_Mode);	// leox20150709		// leox20160322

	#if ADAPTER_PROTECT_DBG	// leox20150717 Adapter protection debug	// leox20150810	// leox20150916 Change time base to 20ms
	VICMVEnThrottleDebD = VICMVHiEnThroLimitDebounce;
	VICMVDsThrottleDebD = VICMVHiDisThroLimitDebounce;
	#endif

	#if BAT_DCHG_PROTECT	// leox20150826 Battery DCHG protection		// leox20150911 Changed default value
	DCHGVHiEnThroLimitCntD =  1;	// 100ms (timebase: 100ms)
	DCHGVHiDsThroLimitCntD = 30;	// 3 sec (timebase: 100ms)
	#endif

	#ifdef	OEM_FAN
	LoadFanCtrlDefault();
	#endif	//OEM_FAN

	#ifdef	OEM_TP
	SWTouchpad_Enable_DisableFunOn();
	Set_Touchpad_Enable();
	#endif	//OEM_TP

	#ifdef	OEM_DIMM
	SetFlag(POWER_STAT_CTRL, DIMM_OFF_Flag);
	#endif	//OEM_DIMM

	REFON_on();

	#if	FIX_USB_POWER_ISSUE	// leox_20110922
	DelayXms(250);
	DelayXms(130);
	#endif

	#ifdef	OEM_Calpella
	AC_PRESENT_input();
	#endif	//OEM_Calpella
}


//------------------------------------------------------------------------------
// Check reboot mark of BRAM and external RAM
//------------------------------------------------------------------------------
// leox_20110926 From Init_OEMVariable() and OemSetDefaultVariables()
void CheckRebootMark(void)
{
	//reboot for IFU.exe tool ++
	if ((BootMask_Bram_00 == 0x55) && (BootMask_Bram_01 == 0xAA) &&
		(BootMask_Bram_02 == 0xBB) && (BootMask_Bram_03 == 0xCC))
	{
		SetFlag(POWER_FLAG, wait_PSW_off);
		TurnOnSBPowerSource();
		DelayXms(120);
		DNBSWON_on();
		PSW_COUNTER = T_PSWOFF;
		BootMask_Bram_00 = 0;
		BootMask_Bram_01 = 0;
		BootMask_Bram_02 = 0;
		BootMask_Bram_03 = 0;
	}
	//reboot for IFU.exe tool --

	if ((REBOOT_MARK  == 165) && (REBOOT_MARK2 == 100) &&
		(REBOOT_MARK3 == 200) && (REBOOT_MARK4 == 50))
	{
		SetFlag(POWER_FLAG, wait_PSW_off);
		TurnOnSBPowerSource();
		DelayXms(120);
		DNBSWON_on();
		PSW_COUNTER = T_PSWOFF;
		REBOOT_MARK  = 0;
		REBOOT_MARK2 = 0;
		REBOOT_MARK3 = 0;
		REBOOT_MARK4 = 0;
	}

	// 120731 chris add for fix flash EC by WINIFU then shutdown then boot fail +++
	if ((BIOS_FLASH_Reset00 == 0xAA) && (BIOS_FLASH_Reset01 == 0xCC) &&
		(BIOS_FLASH_Reset02 == 0xBB) && (BIOS_FLASH_Reset03 == 0xDD))
	{
		SetFlag(POWER_FLAG, wait_PSW_off);
		TurnOnSBPowerSource();
		DelayXms(120);
		DNBSWON_on();
		PSW_COUNTER = T_PSWOFF;
		BIOS_FLASH_Reset00 = 0;
		BIOS_FLASH_Reset01 = 0;
		BIOS_FLASH_Reset02 = 0;
		BIOS_FLASH_Reset03 = 0;
	}
	// 120731 chris add for fix flash EC by WINIFU then shutdown then boot fail ---
}


#if	OEM_POWER_LOSS	// leox_20110921
//------------------------------------------------------------------------------
// Check power loss
//------------------------------------------------------------------------------
// leox_20110926 From "OEM_MAIN.C"
void CheckPowerLoss(void)
{
	switch (B_PwrLoss & 0x03)
	{
	case 0:		// Depend on last state
		if ((B_PwrLoss & 0xFC) == 0xA8)
		{
			SetFlag(POWER_FLAG, wait_PSW_off);
			PSWOnPower();
		}
		break;

	case 1:		// Power on system
		SetFlag(POWER_FLAG, wait_PSW_off);
		PSWOnPower();
		break;

	default:	// Keep power off (2 or other value)
		break;
	}
}
#endif


#if	RESET_EC_LATER
//------------------------------------------------------------------------------
// Check EC reset later mark
//------------------------------------------------------------------------------
// leox_20111206 Create function
void CheckResetLater(void)
{
	if ((RstLaterMark0 == 4) && (RstLaterMark1 == 3))
	{
		#if	1	// 120731 chris add for fix flash EC by WINIFU then shutdown then boot fail
		RstLaterMark0 = 0;
		RstLaterMark1 = 0;
		BIOS_FLASH_Reset00 = 0xAA;
		BIOS_FLASH_Reset01 = 0xCC;
		BIOS_FLASH_Reset02 = 0xBB;
		BIOS_FLASH_Reset03 = 0xDD;
		WinFlashMark = 0;
		WDTRST = 1;		// Reset watch dog timer
		WDTEB = 1;		// Enable watch dog
		while (1) {}	// Wait for watch dog time-out
		#else	// (original)
		RstLaterMark0 = 0;
		RstLaterMark1 = 0;
		REBOOT_MARK  = 165;
		REBOOT_MARK2 = 100;
		REBOOT_MARK3 = 200;
		REBOOT_MARK4 = 50;
		WinFlashMark = 0;
		main();
		#endif
	}
}
#endif

