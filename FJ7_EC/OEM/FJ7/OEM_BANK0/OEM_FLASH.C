/*------------------------------------------------------------------------------
 * Title : OEM_FLASH.C
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//------------------------------------------------------------------------------
// RAM code for flashing
//------------------------------------------------------------------------------
void RamCode(void)
{
	RamcodeCmd = 0x00;

	SET_MASK(SMECCS, HOSTWA);	// Host write allow

	if (RamCodePort == 0x64)
	{
		KBHIKDOR = 0xFA;
	}
	else if (RamCodePort == 0x66)
	{
		PM1DO = 0xFA;
	}
	else if (RamCodePort == 0x6C)
	{
		PM2DO = 0xFA;
	}

	while (1)
	{
		if (RamCodePort == 0x64)
		{
			if (IS_MASK_CLR(KBHISR, IBF)) {continue;}
			if (IS_MASK_CLR(KBHISR, C_D)) {continue;}
			RamcodeCmd = KBHIDIR;
		}
		else if (RamCodePort == 0x66)
		{
			if (IS_MASK_CLR(PM1STS, P_IBF)) {continue;}
			if (IS_MASK_CLR(PM1STS, P_C_D)) {continue;}
			RamcodeCmd = PM1DI;
		}
		else if (RamCodePort == 0x6C)
		{
			if (IS_MASK_CLR(PM2STS, P_IBF)) {continue;}
			if (IS_MASK_CLR(PM2STS, P_C_D)) {continue;}
			RamcodeCmd = PM2DI;
		}

		if (RamcodeCmd == 0x55)
		{
			//#if	RESET_EC_LATER		// leox_20111206
			#if	0	// 120731 chris add for fix flash EC by WINIFU then shutdown then boot fail
			RstLaterMark0 = 4;
			RstLaterMark1 = 3;
			#endif
			WinFlashMark = 0x33;
			break;
		}
		else if (RamcodeCmd == 0xAF)
		{
			WDTRST = 1;		// Reset watch dog timer
			WDTEB = 1;		// Enable watch dog
			while (1) {}	// Wait for watch dog time-out
		}
		else if (RamcodeCmd == 0xD7)	// For AMI BIOS
		{
			CLR_MASK(SMECCS, HOSTWA);	// Host write not allow
			_nop_();
			_nop_();
			SET_MASK(SMECCS, HOSTWA);	// Host write allow
		}
		else if (RamcodeCmd == 0xFE)
		{
			REBOOT_MARK  = 165;	// After DOS flash command FE
			REBOOT_MARK2 = 100;
			REBOOT_MARK3 = 200;
			REBOOT_MARK4 = 50;
			break;
		}
	}

	CLR_MASK(SMECCS, HOSTWA);	// Host write not allow
	_nop_();

	main();
}


//------------------------------------------------------------------------------
// Move code to scratch ROM
//------------------------------------------------------------------------------
void RamProgram(BYTE useport)
{
	RamCodePort = useport;	// Set port for RamCode()

	DisableAllInterrupt();

	Tmp_XPntr = 0x600;

	#ifdef	HSPI
		#ifdef	HSPI_DefferingMode
	CLR_MASK(IER7, Int_DeferredSPI);	// Disable HSPI interrupt
		#endif
	Tmp_code_pointer = (void *)HSPI_CmdRamCode;
	#else
	Tmp_code_pointer = (void *)RamCode;
	#endif

	for (ITempB01 = 0; ITempB01 < 255; ITempB01++)
	{
		*Tmp_XPntr = *Tmp_code_pointer;
		Tmp_XPntr++;
		Tmp_code_pointer++;
	}

	SET_MASK(FBCFG, SSMC);	// Enable scatch ROM
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();

	RamCode();
}


//==============================================================================
// SPI read & write example
//==============================================================================
//------------------------------------------------------------------------------
// SPI protect all
//------------------------------------------------------------------------------
void OEM_SPIProtectAll(void)
{
	Do_SPI_Write_Status(SPIStatus_ProtectAll);
	Do_SPI_Write_Disable();
}


void OEM_SPIUnlockAll(void)
{
	Do_SPI_Write_Status(SPIStatus_UnlockAll);
	Do_SPI_Write_Enable();
}


//------------------------------------------------------------------------------
// Write external RAM 0x200 ~ 0x2FF to SPI ROM 0x010000 ~ 0x0100FF
//------------------------------------------------------------------------------
void OEM_Write_SPI_256Bytes(void)
{
	Do_SPI_Write_Status(SPIStatus_UnlockAll);
	Do_SPI_Write_Enable();
	Do_SPI_Erase(SPICmd_Erase4KByte, 0x01, 0x00, 0x00);
	Do_SPI_Write_Enable();
	Do_SPI_Write_256Bytes(&SPIBuffer, 0x01, 0x00, 0x00);
	Do_SPI_Write_Disable();
}


//------------------------------------------------------------------------------
// Read SPI ROM 0x010000 ~ 0x0100FF to external RAM 0x200 ~ 0x2FF
//------------------------------------------------------------------------------
void OEM_Read_SPI_256Bytes(void)
{
	Do_SPI_Read_256Bytes(&SPIBuffer, 0x01, 0x00, 0x00);
}


//------------------------------------------------------------------------------
// Read SPI ID
// Note: SPI ID always save to array SPIIDBuf[]
//------------------------------------------------------------------------------
void OEM_SPI_Read_ID(void)
{
	Do_SPI_Read_ID();
}


//------------------------------------------------------------------------------
// SPI read status
//------------------------------------------------------------------------------
BYTE OEM_SPIReadStatus(void)
{
	Do_SPI_Read_Status();
	return SPIReadStatus;
}


// For ITE WinFlash Tool ++		// ken 2012/06/29
void ForITEWinFlashTool(void)
{
	if (IsFlag0(POWER_FLAG, ACPI_OS)) {return;}

	if (FLHB != 0)	// Before Flash
	{
		FLHB--;
		if (FLHB == 9)
		{
			ClrFlag(POWER_FLAG2, DisQevent);		// Enabled Q event
			ECQEvent(Q8D_SCI, SCIMode_Normal);		// Store(0, ECOK)	// Notify BIOS not access EC
		}
			if (FLHB == 0)
		{
			ClrFlag(POWER_FLAG2, DisQevent);		// Enabled Q event
			FLHH = 0xFE;
		}
		if ((FLHB <= 8) && (FLHB >= 1))
		{
			if (FLHH == 0xA0)
			{
				SetFlag(POWER_FLAG2, DisQevent);	// Disabled Q event
				FLHH = 0xFA;
				FLHB = 0x00;
			}
			else
			{
				ClrFlag(POWER_FLAG2, DisQevent);	// Enabled Q event
				ECQEvent(Q8D_SCI, SCIMode_Normal);	// Store(0, ECOK)	// Notify BIOS not access EC
			}
		}
	}

	if (FLHE != 0)	// After Flash
	{
		FLHE--;
		if (FLHE == 9)
		{
			ClrFlag(POWER_FLAG2, DisQevent);		// Enabled Q event
			ECQEvent(Q8E_SCI, SCIMode_Normal);		// Store(1, ECOK)	// Notify BIOS can access EC
		}
		if (FLHE == 0)
		{
			FLHH = 0xFC;
		}
		if ((FLHE <= 8) && (FLHE >= 1))
		{
			if (FLHH == 0xF0)
			{
				ClrFlag(POWER_FLAG2, DisQevent);	// Enabled Q event
				FLHH = 0xFC;
				FLHE = 0x00;
			}
			else
			{
				ClrFlag(POWER_FLAG2, DisQevent);	// Enabled Q event
				ECQEvent(Q8E_SCI, SCIMode_Normal);	// Store(1, ECOK)	// Notify BIOS can access EC
			}
		}
	}
}
// For ITE WinFlash Tool --

#if SUPPORT_EFLASH_FUNC	// leox20150630 Access eFlash +++	// leox20160322 Code from N83
//------------------------------------------------------------------------------
// eFlash - Debug
//------------------------------------------------------------------------------
// leox20150624 Add debug code to Service_T500m9SEC()
// leox20150630 Move code from Service_T500m9SEC() to eFlashDbgFunc()
// leox20151030 Rewrite function
// leox20151103 Add command 0x93 to save all data
void eFlashDbgFunc(void)
{
	if (eFL_Act == 0x00) {return;}	// Do nothing

	DisableAllInterrupt();	//@EDWU20150720

	if (eFL_Act == 0x80)	//@EDWU20150720
	{	// Read
		eFL_Act = 0x00;
		if ((eFL_Len >= 1) && (eFL_Len <= 4))		// leox20160122 eFlash secure
		{
			eFlashRead(eFL_Adr[1], eFL_Adr[2], eFL_Adr[3], &eFL_Buf[0], eFL_Len);
		}
	}
	else if (eFL_Act == 0x81)
	{	// Write
		eFL_Act = 0x00;
		if ((eFL_Len >= 1) && (eFL_Len <= 4))		// leox20160122 eFlash secure
		{
			eFlashWrite(eFL_Adr[1], eFL_Adr[2], eFL_Adr[3], &eFL_Buf[0], eFL_Len);
		}
	}
	else if (eFL_Act == 0x8E)
	{	// Erase
		eFL_Act = 0x00;
		eFlashErase(eFL_Adr[1], eFL_Adr[2], eFL_Adr[3]);
	}
	else if (eFL_Act == 0x8F)
	{	// Format
		eFL_Act = 0x00;
		if ((eFL_Len >= 96) && (eFL_Len <= 126))	// leox20160122 eFlash secure
		{
			eFlashFormat(eFL_Len);
		}
	}
	else if (eFL_Act == 0x90)
	{	// Load data
		eFL_Act = 0x00;
		eFlashLoad(eFL_Buf[2], eFL_Buf[3]);
	}
	else if (eFL_Act == 0x91)
	{	// Save data
		eFL_Act = 0x00;
		eFlashSave((XBYTE *)(0x2200 + eFL_Buf[2]), eFL_Buf[3]);	// leox20151102
	}
	else if (eFL_Act == 0x92)	// leox20151103
	{	// Load all data
		eFL_Act = 0x00;
//		eFlashLoadAll();		// Reserved
	}
	else if (eFL_Act == 0x93)	// leox20151103
	{	// Save all data
		eFL_Act = 0x00;
		eFlashSaveAll();
	}

	EnableAllInterrupt();	//@EDWU20150720
}

//------------------------------------------------------------------------------
// eFlash - Initial
//------------------------------------------------------------------------------
// leox20150630 Create function
// leox20150709 Add MMB wake-up settings for S3/S4/S5
// leox20150709 Add power status control setting
// leox20151030 Rewrite function
void eFlashInit(void)
{
	WORD Cnt = 0;

	DisableAllInterrupt();	//@EDWU20150720

	// Search active block
	for (eFL_Blk = EFL_R_B; eFL_Blk <= EFL_R_E; eFL_Blk++)
	{
		eFL_AdrH = 0x0001;	// 0x00018000 is the start of 96KB
		eFL_AdrL = 0x8000 + ((eFL_Blk - 96) << 10) + 0;	// (eFL_Blk - 96) * 1024
		eFlashRead((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 4);
		if ((eFL_Buf[0] == 'E') && (eFL_Buf[1] == 'F') && (eFL_Buf[2] == 'L'))
		{
			if ((eFL_Buf[3] & 0xF0) == (EFL_I_VER & 0xF0)) {break;}	// leox20160316 eFlash version
		}
	}

	if (eFL_Blk <= EFL_R_E)
	{	// Find active block --> Load data and update index
		// Find data record page
		eFL_AdrH = 0x0001;
		eFL_AdrL = 0x8000 + ((eFL_Blk - 96) << 10) + 4;	// Offset of page usage is 4
		eFlashRead((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 4);
		for (eFL_Idx = 0; eFL_Idx < 8; eFL_Idx++)
		{	// BIT[7-0] for 8 pages of 1024 bytes, each page is 128 bytes
			if (IS_BIT_SET(eFL_Buf[0], eFL_Idx)) {break;}		// leox20151103
			Cnt++;
		}
		if (Cnt > 0) {Cnt--;}

		// Load data
		eFL_Idx = (Cnt == 0) ? 8 : (Cnt << 7);	// Set to 8 to skip block header of page 0
		do
		{
			eFL_AdrH = 0x0001;
			eFL_AdrL = 0x8000 + ((eFL_Blk - 96) << 10) + eFL_Idx;
			eFlashRead((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 2);
			if ((eFL_Buf[0] == 0xFF) && (eFL_Buf[1] == 0xFF)) {break;}
			eFlashLoad(eFL_Buf[0], eFL_Buf[1]);
			eFL_Idx += 2;
		} while (eFL_Idx < ((Cnt + 1) << 7));	// (Cnt + 1) * 128
	}
	else
	{	// Not find valid block --> Format block
		eFL_Blk = EFL_R_B;
		eFlashFormat(EFL_R_B);
		Init_SetBramDefVal();	// leox20151104
		eFlashSaveAll();		// leox20160315 Save all BRAM data to eFlash
	}

	EnableAllInterrupt();	//@EDWU20150720
}

//------------------------------------------------------------------------------
// eFlash - Format
//------------------------------------------------------------------------------
// leox20150630 Create function
// leox20151030 Rewrite function
void eFlashFormat(WORD BlkNum)
{
	WORD EraseB2 = 0, EraseB0 = 0;

	if ((BlkNum < 96) || (BlkNum >= 127)) {return;}	// leox20160122 eFlash secure
	if ((BlkNum < EFL_R_B) || (BlkNum > EFL_R_E)) {return;}

	// Read block information before erasing
	eFL_AdrH = 0x0001;	// 0x00018000 is the start of 96KB
	eFL_AdrL = 0x8000 + ((BlkNum - 96) << 10);	// (BlkNum - 96) * 1024
	eFlashRead((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 4);
	if ((eFL_Buf[0] == 'E') && (eFL_Buf[1] == 'F') && (eFL_Buf[2] == 'L'))
	{	// Block was used before --> Get erased times
		eFL_AdrL += 4;
		eFlashRead((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 4);
		EraseB2 = eFL_Buf[1];
		EraseB0 = ((eFL_Buf[2] << 8) + eFL_Buf[3]);
	}

	// Erase block
	eFL_AdrH = 0x0001;
	eFL_AdrL = 0x8000 + ((BlkNum - 96) << 10);
	if (eFL_AdrL < 0x8000) {eFL_AdrL |= 0x8000;}	// leox20160122 eFlash secure, make sure range >= 96KB
	eFlashErase((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF));

	// Add erased counter
	if (++EraseB0 == 0) {EraseB2++;}

	// Reset data index
	eFL_Idx = 8;	// Byte 0 ~ 7 for block header

	// Write header byte 0 ~ 3
	eFL_Buf[0] = 'E';
	eFL_Buf[1] = 'F';
	eFL_Buf[2] = 'L';
	eFL_Buf[3] = EFL_I_VER;	// eFlash version, 0x00 means block discarded
	eFL_AdrH = 0x0001;
	eFL_AdrL = 0x8000 + ((BlkNum - 96) << 10) + 0;
	if (eFL_AdrL < 0x8000) {eFL_AdrL |= 0x8000;}	// leox20160122 eFlash secure, make sure range >= 96KB
	eFlashWrite((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 4);

	// Write header byte 4 ~ 7
	eFL_Buf[0] = 0xFE;	// Mark page 0 (BIT0), it is already used by block header
	eFL_Buf[1] = (EraseB2 >> 0) & 0xFF;
	eFL_Buf[2] = (EraseB0 >> 8) & 0xFF;
	eFL_Buf[3] = (EraseB0 >> 0) & 0xFF;
	eFL_AdrH = 0x0001;
	eFL_AdrL = 0x8000 + ((BlkNum - 96) << 10) + 4;
	if (eFL_AdrL < 0x8000) {eFL_AdrL |= 0x8000;}	// leox20160122 eFlash secure, make sure range >= 96KB
	eFlashWrite((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 4);
}

//------------------------------------------------------------------------------
// eFlash - Save BRAM to eFlash
//------------------------------------------------------------------------------
// leox20150701 Create function
// leox20151030 eFlashSet() -> eFlashSave and rewrite function
void eFlashSave(XBYTE *Adr, BYTE Val)
{
	if ((eFL_Blk < EFL_R_B) || (eFL_Blk > EFL_R_E)) {return;}
	if ((((WORD)Adr) < 0x2200) || (((WORD)Adr) > 0x22BF)) {return;}	// BRAM are 192 bytes (0x2200 ~ 0x22BF)

	DisableAllInterrupt();	//@EDWU20150720

	// Block was full, discard current block and format next block
	if (eFL_Idx >= 1024)
	{
		eFL_Buf[0] = 0x00;	// Set version to 0x00 to discard current block
		eFL_AdrH = 0x0001;
		eFL_AdrL = 0x8000 + ((eFL_Blk - 96) << 10) + 3;	// Offset of version is 3
		if (eFL_AdrL < 0x8000) {eFL_AdrL |= 0x8000;}	// leox20160122 eFlash secure, make sure range >= 96KB
		eFlashWrite((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 1);
		if (++eFL_Blk > EFL_R_E) {eFL_Blk = EFL_R_B;}
		eFlashFormat(eFL_Blk);	// Format and move to next block
		SetFlag(eFL_ToDo, BIT0);// Save all BRAM data to eFlash later	// leox20160316
	}

	// Set record page
	if ((eFL_Idx & 0x7F) == 0)	// (eFL_Idx % 128) == 0
	{
		eFL_Buf[0] = 0xFF & (~(BIT(eFL_Idx >> 7)));	// eFL_Idx / 128
		eFL_AdrH = 0x0001;
		eFL_AdrL = 0x8000 + ((eFL_Blk - 96) << 10) + 4;	// Offset of page usage is 4
		if (eFL_AdrL < 0x8000) {eFL_AdrL |= 0x8000;}	// leox20160122 eFlash secure, make sure range >= 96KB
		eFlashWrite((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 1);
		SetFlag(eFL_ToDo, BIT0);// Save all BRAM data to eFlash later	// leox20160316
	}

	// Save data
	eFL_Buf[0] = ((WORD)(Adr)) & 0xFF;
	eFL_Buf[1] = Val;
	eFL_AdrH = 0x0001;
	eFL_AdrL = 0x8000 + ((eFL_Blk - 96) << 10) + eFL_Idx;
	if (eFL_AdrL < 0x8000) {eFL_AdrL |= 0x8000;}		// leox20160122 eFlash secure, make sure range >= 96KB
	eFlashWrite((eFL_AdrH & 0xFF), ((eFL_AdrL >> 8) & 0xFF), (eFL_AdrL & 0xFF), &eFL_Buf[0], 2);
	eFL_Idx += 2;

	EnableAllInterrupt();	//@EDWU20150720
}

//------------------------------------------------------------------------------
// eFlash - Load data to BRAM
//------------------------------------------------------------------------------
// leox20150703 Create function
// leox20151030 eFlashLoadData() -> eFlashLoad and rewrite function
void eFlashLoad(BYTE Adr, BYTE Val)
{
	if (Adr > 0xBF) {return;}	// BRAM are 192 bytes (0x2200 ~ 0x22BF)
	Tmp_XPntr = 0x2200 | Adr;
	*Tmp_XPntr = Val;
}

//------------------------------------------------------------------------------
// eFlash - Save all BRAM to eFlash	// CHECK it by project demand
//------------------------------------------------------------------------------
// leox20151103 Create function
// leox20151104 Rewrite function by saving variables itself
// leox20160322 Add the data need to save of FH9C
void eFlashSaveAll(void)
{
	eFlashSave(&USBCHARGEDISCHARG_BRM, USBCHARGEDISCHARG_BRM);		// OEM_IPOD
	eFlashSave(&B_PwrLoss, B_PwrLoss);
	#if ENTER_SLEEP_MODE_AC		// leox20150710
	eFlashSave(&B_PwrCtrl, B_PwrCtrl);
	#endif
	eFlashSave(&Shipmode_status, Shipmode_status);
	#ifdef IT_7230MMB_SUPPORT	// leox20150709
	eFlashSave(&MMB_WakeUp_S3_BRM, MMB_WakeUp_S3_BRM);
	eFlashSave(&MMB_WakeUp_S4_BRM, MMB_WakeUp_S4_BRM);
	eFlashSave(&MMB_WakeUp_S5_BRM, MMB_WakeUp_S5_BRM);
	#endif
	eFlashSave(&POWER_STAT_CTRL1_BRM, POWER_STAT_CTRL1_BRM);
	#if SUPPORT_OEM_APP			// leox20160322
	eFlashSave(&B_AppSwitchFnFx, B_AppSwitchFnFx);
	#endif
}

//------------------------------------------------------------------------------
// eFlash - To do some actions later
//------------------------------------------------------------------------------
// leox20160316 Create function
void eFlashToDo(void)
{
	if (eFL_ToDo == 0x00) {return;}	// Do nothing

	DisableAllInterrupt();	//@EDWU20150720

	if (IsFlag1(eFL_ToDo, BIT0))
	{	// Save all data
		ClrFlag(eFL_ToDo, BIT0);
		eFlashSaveAll();
	}

	EnableAllInterrupt();	//@EDWU20150720
}
#endif	// SUPPORT_EFLASH_FUNC	// leox20150630 Access eFlash ---
