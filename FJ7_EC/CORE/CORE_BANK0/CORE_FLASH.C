/*------------------------------------------------------------------------------
 * Title: CORE_FLASH.C
 *
 * Copyright (c) ITE INC. All Rights Reserved.
 *----------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//-----------------------------------------------------------------------------
// The function start address always at 0xFE00 (refer to link file)
//-----------------------------------------------------------------------------
void FuncAt_0xFE00(void)
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	FlashECCode();
}

//-----------------------------------------------------------------------------
// The function of EC flash
//-----------------------------------------------------------------------------
void FlashECCode(void)
{
	RamcodeCmd = 0x00;
	RamcodeSend = 0x00;

	PM1DO = 0x33;		// ACK

	while (1)
	{
		if (IS_MASK_SET(KBHISR, IBF))
		{
			RamcodeCmd = KBHIDIR;
			continue;
		}
		if (IS_MASK_CLEAR(PM1STS, P_IBF)) { continue; }
		if (IS_MASK_CLEAR(PM1STS, P_C_D)) { continue; }
		RamcodeCmd = PM1DI;

		if (RamcodeSend == 1)
		{
			ECINDDR = RamcodeCmd;
			RamcodeSend = 0x00;
			continue;
		}

		if (RamcodeCmd == 0x01)
		{
			ECINDAR3 = 0x0F;		// Enter follow mode
			ECINDAR2 = 0xFF;
			ECINDAR1 = 0xFE;
			ECINDAR0 = 0x00;		// FFFFExx = 0xFF
			ECINDDR = 0x00;			// SCE# high level
		}
		else if (RamcodeCmd == 0x02)
		{
			// Send SPI command
			ECINDAR1 = 0xFD;
			RamcodeSend = 1;
		}
		else if (RamcodeCmd == 0x03)
		{
			// write byte to spi
			RamcodeSend = 1;
		}
		else if (RamcodeCmd == 0x04)
		{
			//while(IS_MASK_SET(PM1STS,P_OBF));
			PM1DO = ECINDDR;		// Read byte from spi
		}
		else if (RamcodeCmd == 0x05)
		{
			ECINDAR3 = 0x00;		// Exit follow mode
			ECINDAR2 = 0x00;
		}
		else if (RamcodeCmd == 0xFC)
		{
			#if	RESET_EC_LATER		// leox_20111206 [PATCH]
			RstLaterMark0 = 4;
			RstLaterMark1 = 3;
			#endif
			WinFlashMark = 0x33;
			break;
		}
		#if	0	// leox_20111206 [PATCH] Disable
		else if (RamcodeCmd == 0xFD)
		{
			WDTRST = 1;				// Reset watch dog timer
			WDTEB = 1;				// Enable watch dog
			while (1);				// Wait for watch dog time-out
		}
		#endif
		else if (RamcodeCmd == 0xFE)
		{
			//reboot for IFU.exe tool ++
			BootMask_Bram_00 = 0x55;
			BootMask_Bram_01 = 0xAA;
			BootMask_Bram_02 = 0xBB;
			BootMask_Bram_03 = 0xCC;
			//reboot for IFU.exe tool --
			WDTRST = 1;				// Reset watch dog timer
			WDTEB = 1;				// Enable watch dog
			while (1);				// Wait for watch dog time-out
		}
	}

	main();
}

//-----------------------------------------------------------------------------
// The function of reading SPI status ( command 0x05)
//  After this function, spi status will save to SPIReadStatus
//-----------------------------------------------------------------------------
void SPI_Read_Status(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read status command

	SPIReadStatus = ECINDDR;		// Save status Register to SPIReadStatus

	ECINDAR3 = 0x00;				//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of Write SPI status ( command 0x01)
//  After this function, the value of SPIWriteStatus will write to spi status
//-----------------------------------------------------------------------------
void SPI_Write_Status(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WREN;			// Write enable

	if (SPIID == SSTID)
	{
		ECINDAR1 = 0xFE;
		ECINDDR = 0xFF;				// SCE# high level
		ECINDAR1 = 0xFD;
		ECINDDR = SPICmd_EWSR;		// Enable Write Status Register
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WRSR;			// Write status command
	ECINDDR = SPIWriteStatus;		// Write SPIWriteStatus to spi status register

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR3 = 0x00;				//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of reading SPI ID (command 0x9F)
//  After this function, spi id will save to array SPIIDBuf[]
//-----------------------------------------------------------------------------
void SPI_Read_ID(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_DeviceID;		// Manufacture ID command
	for (SPIIndex = 0x00; SPIIndex < 4; SPIIndex++)
	{
		SPIIDBuf[SPIIndex] = ECINDDR;
	}
	SPIID = SPIIDBuf[0];

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register

	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}
	ECINDAR3 = 0x00;		//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of reading SPI ID (command 0xAB)
//  After this function, spi id will save to array SPIIDBuf[]
//-----------------------------------------------------------------------------
void SPI_Read_ID_CmdAB(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_RDID;			// Read ID command
	ECINDDR = 0x00;					// Read ID command addr2
	ECINDDR = 0x00;					// Read ID command addr1
	ECINDDR = 0x00;					// Read ID command addr0

	for (SPIIndex = 0x00; SPIIndex < 4; SPIIndex++)
	{
		SPIIDBuf[SPIIndex] = ECINDDR;
	}
	SPIID = SPIIDBuf[0];

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register

	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}
	ECINDAR3 = 0x00;		//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of Erasing SPI
//-----------------------------------------------------------------------------
void SPI_Erase(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & (SPIStatus_BUSY + SPIStatus_WEL)) == SPIStatus_WriteEnable)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPIAEraseCmd;			// Send erase command
	for (SPIIndex = 0x00; SPIIndex < 3; SPIIndex++)
	{
		ECINDDR = SPIAddrCycle[SPIIndex];
	}								// Send address cycle

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level

	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register

	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}
	ECINDAR3 = 0x00;				//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of SPI write enable
//-----------------------------------------------------------------------------
void SPI_Write_Enable(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WREN;			// Write enable

	if (SPIID == SSTID)
	{
		ECINDAR1 = 0xFE;
		ECINDDR = 0xFF;				// SCE# high level
		ECINDAR1 = 0xFD;
		ECINDDR = SPICmd_EWSR;		// Enable Write Status Register
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & (SPIStatus_WEL + SPIStatus_BUSY)) == 0x02)	// Check write enable and spi not busy
		{
			break;
		}
	}

	ECINDAR3 = 0x00;				//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of SPI write disable
//-----------------------------------------------------------------------------
void SPI_Write_Disable(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WRDI;			// Write disable

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & (SPIStatus_WEL + SPIStatus_BUSY)) == 0x00)	// Check write disable and spi not busy
		{
			break;
		}
	}

	ECINDAR3 = 0x00;				//Exit follow mode
	ECINDAR2 = 0x00;
}

//-----------------------------------------------------------------------------
// The function of other SPI write 256 bytes
//-----------------------------------------------------------------------------
void Other_SPI_Write_256Bytes(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_BYTEProgram;	// Send Program One Data Byte command

	ECINDDR = SPIAddrCycle[0];		// For 256 bytes function limite
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];

	//SPIIndex = 0x00;
	SPIIndex = 0x00 - SPIRdWrSize;	// leox20150625 Access eFlash [PATCH]
	do
	{
		ECINDDR = *SPIDataPointer;
		SPIIndex++;
		SPIDataPointerLowByte++;
	}
	while (SPIIndex != 0x00);		// Send 256 bytes

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR3 = 0x00;				// Exit follow mode
	ECINDAR2 = 0x00;				// For 256 bytes function limite
}

//-----------------------------------------------------------------------------
// The function of SST SPI write 256 bytes (for Word AAI command)
//-----------------------------------------------------------------------------
void SST_SPI_Write_256Bytes(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_AAIWordProgram;	// Auto Address Increment Programming

	ECINDDR = SPIAddrCycle[0];		// For 256 bytes function limite
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];

	//SPIIndex = 0x00;
	SPIIndex = 0x00 - SPIRdWrSize;	// leox20150625 Access eFlash [PATCH]
	do
	{
		ECINDDR = *SPIDataPointer;
		SPIIndex++;
		SPIDataPointerLowByte++;

		if ((SPIIndex % 2) == 0x00)
		{
			ECINDAR1 = 0xFE;
			ECINDDR = 0xFF;					// SCE# high level
			ECINDAR1 = 0xFD;
			ECINDDR = SPICmd_ReadStatus;	// Read Status Register
			while (1)						// waiting spi free
			{
				if ((ECINDDR & SPIStatus_BUSY) == 0x00)
				{
					break;
				}
			}

			if (SPIIndex != 0x00)
			{
				ECINDAR1 = 0xFE;
				ECINDDR = 0xFF;							// SCE# high level
				ECINDAR1 = 0xFD;
				ECINDDR = SPICmd_AAIWordProgram;		// Auto Address Increment Programming
			}
		}
	}
	while (SPIIndex != 0x00);				// Send 256 bytes

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WRDI;			// Write disable

	WNCKR = 0x00;					// Delay 15.26 us
	WNCKR = 0x00;					// Delay 15.26 us
	WNCKR = 0x00;					// Delay 15.26 us
	WNCKR = 0x00;					// Delay 15.26 us

	ECINDAR3 = 0x00;				// Exit follow mode
	ECINDAR2 = 0x00;				// For 256 bytes function limite
}

//-----------------------------------------------------------------------------
// The function of old SST SPI write 256 bytes
//-----------------------------------------------------------------------------
void Old_SST_SPI_Write_256Bytes(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_AAIProgram;	// Auto Address Increment Programming

	ECINDDR = SPIAddrCycle[0];		// For 256 bytes function limite
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];

	//SPIIndex = 0x00;
	SPIIndex = 0x00 - SPIRdWrSize;	// leox20150625 Access eFlash [PATCH]
	do
	{
		ECINDDR = *SPIDataPointer;
		SPIIndex++;
		SPIDataPointerLowByte++;

		ECINDAR1 = 0xFE;
		ECINDDR = 0xFF;					// SCE# high level
		ECINDAR1 = 0xFD;
		ECINDDR = SPICmd_ReadStatus;	// Read Status Register
		while (1)						// waiting spi free
		{
			if ((ECINDDR & SPIStatus_BUSY) == 0x00)
			{
				break;
			}
		}

		if (SPIIndex != 0x00)
		{
			ECINDAR1 = 0xFE;
			ECINDDR = 0xFF;				// SCE# high level
			ECINDAR1 = 0xFD;
			ECINDDR = SPICmd_AAIProgram;	// Auto Address Increment Programming
		}
	}
	while (SPIIndex != 0x00);		// Send 256 bytes

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_WRDI;			// Write disable


	WNCKR = 0x00;					// Delay 15.26 us
	WNCKR = 0x00;					// Delay 15.26 us
	WNCKR = 0x00;					// Delay 15.26 us
	WNCKR = 0x00;					// Delay 15.26 us
	//ECINDAR1 = 0xFE;
	//ECINDDR = 0xFF;				// SCE# high level
	//ECINDAR1 = 0xFD;
	//ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	//while(1)						// waiting spi free
	//{
	//	if((ECINDDR&(SPIStatus_BUSY+SSTSPIStatus_AAI))==0x00)
	//	{
	//		break;
	//	}
	//}

	ECINDAR3 = 0x00;				// Exit follow mode
	ECINDAR2 = 0x00;				// For 256 bytes function limite
}

//-----------------------------------------------------------------------------
// The function of SPI write 256 bytes
//-----------------------------------------------------------------------------
void SPI_Write_256Bytes(void)
{
	if (SPIID == SSTID)
	{
		if (SSTDeviceID == SSTID_Old)
		{
			LoadSPIFucnToRam(Old_SST_SPI_Write_256Bytes);	// Load function to ram
		}
		else
		{
			LoadSPIFucnToRam(SST_SPI_Write_256Bytes);	// Load function to ram
		}
	}
	else
	{
		//+Jay20121122 add_eflash_protect
		if (SPIID == ITE8587)
		{
			LoadSPIFucnToRam(SST_SPI_Write_256Bytes);	// Load function to ram
		}
		else
		{
			LoadSPIFucnToRam(Other_SPI_Write_256Bytes);	// Load function to ram
		}
		//-Jay20121122 add_eflash_protect
	}
}


//-----------------------------------------------------------------------------
// The function of SPI read 256 bytes
//-----------------------------------------------------------------------------
void SPI_Read_256Bytes(void)
{
	//+Jay20151125 [PATCH]
	//ECINDAR3 = 0x0F;
	//if (IS_MASK_CLR(FLHCTRL3R, SIFE)) {ECINDAR3 = 0x4F;}	//@Jay20121122 add_eflash_protect [PATCH]
	ECINDAR3 = EC_Indirect_Selection;
	//-Jay20151125
	ECINDAR2 = 0xFF;
	ECINDAR0 = 0x00;				// FFFFExx = 0xFF

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;						// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_HighSpeedRead;		// High-Speed Read

	ECINDDR = SPIAddrCycle[0];			// For 256 bytes function limite
	ECINDDR = SPIAddrCycle[1];
	ECINDDR = SPIAddrCycle[2];
	ECINDDR = 0x00;						// dummy byte

	//SPIIndex = 0x00;
	SPIIndex = 0x00 - SPIRdWrSize;		// leox20150625 Access eFlash [PATCH]
	do
	{
		*SPIDataPointer = ECINDDR;;
		SPIIndex++;
		SPIDataPointerLowByte++;
	}
	while (SPIIndex != 0x00);			// Send 256 bytes

	ECINDAR1 = 0xFE;
	ECINDDR = 0xFF;					// SCE# high level
	ECINDAR1 = 0xFD;
	ECINDDR = SPICmd_ReadStatus;	// Read Status Register
	while (1)						// waiting spi free
	{
		if ((ECINDDR & SPIStatus_BUSY) == 0x00)
		{
			break;
		}
	}

	ECINDAR3 = 0x00;				// Exit follow mode
	ECINDAR2 = 0x00;				// For 256 bytes function limite
}

//-----------------------------------------------------------------------------
// EC indirect fast read
//-----------------------------------------------------------------------------
// leox20150706 Refer to "ZRZ_BOI_20150630.7z" provided by ITE Jay Wu
void ECIndirectFastRead(BYTE Adr2, BYTE Adr1, BYTE Adr0, BYTE *Data, BYTE Size)
{
	ECINDAR3 = EC_Indirect_Selection & 0xF0;	//@Jay20151125	// @EDWU20151207 [PATCH]
	ECINDAR2 = Adr2;		// Start address
	ECINDAR1 = Adr1;
	ECINDAR0 = Adr0;
	while (Size)
	{
		ECINDAR2 = Adr2;
		ECINDAR1 = Adr1;
		ECINDAR0 = Adr0;
		*Data = ECINDDR;
		if ((++Adr0 == 0x00) && (++Adr1 == 0x00)) {Adr2++;}
		Data++;
		Size--;
	}
}

//-----------------------------------------------------------------------------
// The function of loading function to external ram 0x600 ~ 0x6FF
//-----------------------------------------------------------------------------
void LoadSPIFucnToRam(FUNCT_PTR_V_V funcpoint)
{
	#ifdef SPI_DMA_MODE	// leox20150706 [PATCH]
	CacheDma(2, *funcpoint);
	funcpoint();
	#else	// (original)
	Tmp_XPntr = 0x500;
	Tmp_code_pointer = funcpoint;
	ITempB01 = 0x00;
	do
	{
		*Tmp_XPntr = *Tmp_code_pointer;
		Tmp_XPntr++;
		Tmp_code_pointer++;
		ITempB01++;
	}
	while (ITempB01 != 0x00);
	#endif
}

//-----------------------------------------------------------------------------
// The function for ITE flash utility
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void ITE_Flash_Utility(void)
{
	DisableAllInterrupt();				// Disable all interrupt
	LoadSPIFucnToRam(FlashECCode);		// Load function to ram
	SET_MASK(FBCFG, SSMC);				// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
}

//*****************************************************************************
// OEM FUNCTION CALL
//*****************************************************************************
//-----------------------------------------------------------------------------
// The function for oem code write spi status
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Write_Status(BYTE statusvalue)
{
	SPIWriteStatus = statusvalue;
	#ifdef SPI_DMA_MODE					// leox20150706 [PATCH]
	LoadSPIFucnToRam(SPI_Write_Status);	// Load function to ram
	#else	// (original)
//	DisableAllInterrupt();				// Disable all interrupt	//@EDWU20150720 [PATCH] Disable
	LoadSPIFucnToRam(SPI_Write_Status);	// Load function to ram
	SET_MASK(FBCFG, SSMC);				// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	CLEAR_MASK(FBCFG, SSMC);			// disable scatch ROM
//	EnableAllInterrupt();				// Enable all interrupt		//@EDWU20150720 [PATCH] Disable
	#endif
}

//-----------------------------------------------------------------------------
// The function for oem code read spi status
//      Note : SPI status always save to SPIReadStatus
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Read_Status(void)
{
	#ifdef SPI_DMA_MODE					// leox20150706 [PATCH]
	LoadSPIFucnToRam(SPI_Read_Status);	// Load function to ram
	#else	// (original)
//	DisableAllInterrupt();				// Disable all interrupt	//@EDWU20150720 [PATCH] Disable
	LoadSPIFucnToRam(SPI_Read_Status);	// Load function to ram
	SET_MASK(FBCFG, SSMC);				// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	CLEAR_MASK(FBCFG, SSMC);			// disable scatch ROM
//	EnableAllInterrupt();				// Enable all interrupt		//@EDWU20150720 [PATCH] Disable
	#endif
}

//-----------------------------------------------------------------------------
// The function for oem code read spi ID (command 0x9F)
//      Note : SPI ID always save to array SPIIDBuf[]
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Read_ID(void)
{
	#ifdef SPI_DMA_MODE				// leox20150706 [PATCH]
	LoadSPIFucnToRam(SPI_Read_ID);	// Load function to ram
	#else	// (original)
//	DisableAllInterrupt();			// Disable all interrupt	//@EDWU20150720 [PATCH] Disable
	LoadSPIFucnToRam(SPI_Read_ID);	// Load function to ram
	SET_MASK(FBCFG, SSMC);			// enable scatch ROM
	FuncAt_0xFE00();				// do function in extern ram 0x600
	CLEAR_MASK(FBCFG, SSMC);		// disable scatch ROM
//	EnableAllInterrupt();			// Enable all interrupt		//@EDWU20150720 [PATCH] Disable
	#endif
}

//-----------------------------------------------------------------------------
// The function for oem code read spi ID (command 0xAB)
//      Note : SPI ID always save to array SPIIDBuf[]
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Read_ID_CmdAB(void)
{
	#ifdef SPI_DMA_MODE				// leox20150706 [PATCH]
	LoadSPIFucnToRam(SPI_Read_ID_CmdAB);	// Load function to ram
	#else	// (original)
//	DisableAllInterrupt();			// Disable all interrupt	//@EDWU20150720 [PATCH] Disable
	LoadSPIFucnToRam(SPI_Read_ID_CmdAB);	// Load function to ram
	SET_MASK(FBCFG, SSMC);			// enable scatch ROM
	FuncAt_0xFE00();				// do function in extern ram 0x600
	CLEAR_MASK(FBCFG, SSMC);		// disable scatch ROM
//	EnableAllInterrupt();			// Enable all interrupt		//@EDWU20150720 [PATCH] Disable
	#endif
}

//-----------------------------------------------------------------------------
// The function for oem code erase spi
//      Input : 1 EraseCmd --> Only support SPICmd_Erase4KByte,
//                              SPICmd_Erase32KByte, and SPICmd_Erase64KByte.
//              2 Addr2    --> A23 ~ A16
//              3 Addr1    --> A15 ~ A8
//              4 Addr0    --> A7  ~ A0
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Erase(BYTE EraseCmd, BYTE Addr2, BYTE Addr1, BYTE Addr0)
{
	SPIAEraseCmd = EraseCmd;
	SPIAddrCycle[0] = Addr2;
	SPIAddrCycle[1] = Addr1;
	SPIAddrCycle[2] = Addr0;

	#ifdef SPI_DMA_MODE				// leox20150706 [PATCH]
	LoadSPIFucnToRam(SPI_Erase);	// Load function to ram
	#else	// (original)
//	DisableAllInterrupt();			// Disable all interrupt	//@EDWU20150720 [PATCH] Disable
	LoadSPIFucnToRam(SPI_Erase);	// Load function to ram
	SET_MASK(FBCFG, SSMC);			// enable scatch ROM
	FuncAt_0xFE00();				// do function in extern ram 0x600
	CLEAR_MASK(FBCFG, SSMC);		// disable scatch ROM
//	EnableAllInterrupt();			// Enable all interrupt		//@EDWU20150720 [PATCH] Disable
	#endif
}

//-----------------------------------------------------------------------------
// The function for oem code write spi 256 bytes
//      Input : 1 DataPointer --> Pointe to external memory
//              2 Addr2    --> A23 ~ A16
//              3 Addr1    --> A15 ~ A8
//              4 Addr0    --> A7  ~ A0
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Write_256Bytes(XBYTE *DataPointer, BYTE Addr2, BYTE Addr1, BYTE Addr0)
{
	SPIDataPointer = DataPointer;
	SPIAddrCycle[0] = Addr2;
	SPIAddrCycle[1] = Addr1;
	SPIAddrCycle[2] = Addr0;

	#ifdef SPI_DMA_MODE					// leox20150706 [PATCH]
	SPI_Write_256Bytes();				// Load function to ram
	#else	// (original)
//	DisableAllInterrupt();				// Disable all interrupt	//@EDWU20150720 [PATCH] Disable
	SPI_Write_256Bytes();				// Load function to ram
	SET_MASK(FBCFG, SSMC);				// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	CLEAR_MASK(FBCFG, SSMC);			// disable scatch ROM
//	EnableAllInterrupt();				// Enable all interrupt		//@EDWU20150720 [PATCH] Disable
	#endif
}

//-----------------------------------------------------------------------------
// The function for oem code read spi 256 bytes to external memory
//      Input : 1 DataPointer --> Pointe to external memory
//              2 Addr2    --> A23 ~ A16
//              3 Addr1    --> A15 ~ A8
//              4 Addr0    --> A7  ~ A0
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Read_256Bytes(XBYTE *DataPointer, BYTE Addr2, BYTE Addr1, BYTE Addr0)
{
	SPIDataPointer = DataPointer;
	SPIAddrCycle[0] = Addr2;
	SPIAddrCycle[1] = Addr1;
	SPIAddrCycle[2] = Addr0;

	#ifdef SPI_DMA_MODE					// leox20150706 [PATCH]
	LoadSPIFucnToRam(SPI_Read_256Bytes);	// Load function to ram
	#else	// (original)
//	DisableAllInterrupt();				// Disable all interrupt	//@EDWU20150720 [PATCH] Disable
	LoadSPIFucnToRam(SPI_Read_256Bytes);	// Load function to ram
	SET_MASK(FBCFG, SSMC);				// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	CLEAR_MASK(FBCFG, SSMC);			// disable scatch ROM
//	EnableAllInterrupt();				// Enable all interrupt		//@EDWU20150720 [PATCH] Disable
	#endif
}

//-----------------------------------------------------------------------------
// The function for oem code spi write enable
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Write_Enable(void)
{
	#ifdef SPI_DMA_MODE					// leox20150706 [PATCH]
	LoadSPIFucnToRam(SPI_Write_Enable);	// Load function to ram
	#else	// (original)
//	DisableAllInterrupt();				// Disable all interrupt	//@EDWU20150720 [PATCH] Disable
	LoadSPIFucnToRam(SPI_Write_Enable);	// Load function to ram
	SET_MASK(FBCFG, SSMC);				// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	CLEAR_MASK(FBCFG, SSMC);			// disable scatch ROM
//	EnableAllInterrupt();				// Enable all interrupt		//@EDWU20150720 [PATCH] Disable
	#endif
}

//-----------------------------------------------------------------------------
// The function for oem code spi write disable
//-----------------------------------------------------------------------------
#pragma ot(7, SIZE)
void Do_SPI_Write_Disable(void)
{
	#ifdef SPI_DMA_MODE					// leox20150706 [PATCH]
	LoadSPIFucnToRam(SPI_Write_Disable);// Load function to ram
	#else	// (original)
//	DisableAllInterrupt();				// Disable all interrupt	//@EDWU20150720 [PATCH] Disable
	LoadSPIFucnToRam(SPI_Write_Disable);// Load function to ram
	SET_MASK(FBCFG, SSMC);				// enable scatch ROM
	FuncAt_0xFE00();					// do function in extern ram 0x600
	CLEAR_MASK(FBCFG, SSMC);			// disable scatch ROM
//	EnableAllInterrupt();				// Enable all interrupt		//@EDWU20150720 [PATCH] Disable
	#endif
}

//+Jay20121130 add_oem_mirror_code
#pragma ot(7, SIZE)
void Code_Mirror(void)
{
	DisableAllInterrupt();				// Disable all interrupt
	/*	//+Jay20131206 CHANGE_AFTER_WDT_MIRROR
	LoadSPIFucnToRam(OEM_Mirror);
	SET_MASK(FBCFG, SSMC);				// enable scatch ROM
	//+Jay20130219 FIX_IU_CANNOT_FLASH
	//FuncAt_0xFE00();					// do function in extern ram 0x600
	FuncAt_Scratch(6);
	//-Jay20130219 FIX_IU_CANNOT_FLASH
	*/	//-Jay20131206 CHANGE_AFTER_WDT_MIRROR
	//+Jay20131206 CHANGE_AFTER_WDT_MIRROR
	RSMRST_on();
	DelayXms(200);		// leox20150721 Wait PCH off
	STBON_on();
	#if WAIT_PCH_STABLE_MIRROR	// leox20150807 [PATCH]
	WaitS5ToDoWDT = 0x99;		// leox20150721 Wait PCH chip stable after mirror code
	#else	// (original)
	WaitS5ToDoWDT = 0;
	#endif
	FLHCTRL3R = 0x08;
	FLHCTRL3R = 0x18;	// BIT3 enable SPI I/F
	CheckResetLater();
	WDTRST = 1;			// Reset watchdog timer
	WDTEB = 1;			// Enable watchdog
	while(1);
	//-Jay20131206 CHANGE_AFTER_WDT_MIRROR
}
//-Jay20121130 add_oem_mirror_code

//+Jay20151125 [PATCH]
//-----------------------------------------------------------------------------
// parameter :
// selection : SPI_selection_internal | SPI_selection_external
//
// return :
// none
//-----------------------------------------------------------------------------
void Init_EC_Indirect_Selection(BYTE selection)
{
	if (selection == SPI_selection_internal)
	{	// Internal
		EC_Indirect_Selection = 0x4F;
	}
	else
	{	// External
		EC_Indirect_Selection = 0x0F;
	}
}
//-Jay20151125

#if SUPPORT_EFLASH_FUNC	// leox20150624 Access eFlash +++	// leox20150702 [PATCH]
//-----------------------------------------------------------------------------
// Read data from eFlash
//-----------------------------------------------------------------------------
// leox20150624 Create function
// leox20150706 Use ECIndirectFastRead() instead of SPI_Read_NBytes()
#pragma ot(7, SIZE)
void eFlashRead(BYTE Adr2, BYTE Adr1, BYTE Adr0, XBYTE *Data, BYTE Size)
{
	ECIndirectFastRead(Adr2, Adr1, Adr0, Data, Size);
}

//-----------------------------------------------------------------------------
// Write data to eFlash
//-----------------------------------------------------------------------------
// leox20150624 Create function
#pragma ot(7, SIZE)
void eFlashWrite(BYTE Adr2, BYTE Adr1, BYTE Adr0, XBYTE *Data, BYTE Size)
{
	SPIRdWrSize = Size;	// Set write size
	Do_SPI_Read_ID();
	Do_SPI_Write_Status(SPIStatus_UnlockAll);
	Do_SPI_Write_Enable();
	Do_SPI_Write_256Bytes(Data, Adr2, Adr1, Adr0);
	Do_SPI_Write_Disable();
	SPIRdWrSize = 0;	// Clear after wrote
}

//-----------------------------------------------------------------------------
// Erase data of eFlash
//-----------------------------------------------------------------------------
// leox20150624 Create function
#pragma ot(7, SIZE)
void eFlashErase(BYTE Adr2, BYTE Adr1, BYTE Adr0)
{
	Do_SPI_Read_ID();
	Do_SPI_Write_Status(SPIStatus_UnlockAll);
	Do_SPI_Write_Enable();
	Do_SPI_Erase(SPICmd_Erase1KByte, Adr2, Adr1, Adr0);
	Do_SPI_Write_Disable();
}
#endif	// SUPPORT_EFLASH_FUNC	// leox20150624 Access eFlash ---
