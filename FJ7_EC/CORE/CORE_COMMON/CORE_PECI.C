/*------------------------------------------------------------------------------
 * Title: CORE_PECI.C - PECI function for KBC firmware.
 *
 * Copyright (c) 2010 - , ITE INC. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of ITE INC. .
 *----------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

#ifdef	PECI_Support

//-----------------------------------------------------------------------
// Init Enable PECI interface function
//-----------------------------------------------------------------------
void Init_PECI(void)
{
	GPCRF6 = ALT;
	GCR2 |= PECIE;		// Enable PECI
	HOCTL2R = (HOCTL2R & 0xF8) | PECI_TRANSFER_RATE;    // leox_20131129 [PATCH]
	#if Support_IT8987	// leox20150729 PECI setting for IT8987 [PATCH]
	PADCTLR = (PADCTLR & 0xFC) | PECI_VTT_SETTING;
	#endif
}

//-----------------------------------------------------------------------
// Init Enable PECI interface function
//-----------------------------------------------------------------------
void PECI_Enable(void)
{
	//GPCRF6 = ALT;
}

//-----------------------------------------------------------------------
// Init Disable PECI interface function
//-----------------------------------------------------------------------
void PECI_Disable(void)
{
	//GPCRF6 = INPUT;
}

//-----------------------------------------------------------------------
// Init timer1 for interface time-out
//-----------------------------------------------------------------------
void PECI_InitTimeOutTimer(void)
{
	TR1 = 0;					// Disable timer1
	ET1 = 0;					// Disable timer1 interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	TH1 = Timer_30ms >> 8;		// Set timer1 counter 30ms
	TL1 = Timer_30ms;			// Set timer1 counter 30ms
	TF1 = 0;					// Clear overflow flag
	TR1 = 1;					// Enable timer1
}

//-----------------------------------------------------------------------
// CRC-8 polynomial
//-----------------------------------------------------------------------
void PECI_CalcCRC8(BYTE sourcebyte)
{
	#if	PECI_Softwave_AWFCS
	BYTE temp;
	PECI_CRC8 ^= sourcebyte;
	temp = PECI_CRC8;

	if (IS_MASK_CLEAR(temp, BIT7))
	{
		temp = temp << 1;
		PECI_CRC8 ^= temp;
	}
	else
	{
		temp = temp << 1;
		PECI_CRC8 ^= 0x09;
		PECI_CRC8 ^= temp;
	}

	if (IS_MASK_CLEAR(temp, BIT7))
	{
		temp = temp << 1;
		PECI_CRC8 ^= temp;
	}
	else
	{
		temp = temp << 1;
		PECI_CRC8 ^= 0x07;
		PECI_CRC8 ^= temp;
	}
	#endif
}

//-----------------------------------------------------------------------
// Enables the PECI host controller.
//-----------------------------------------------------------------------
void PECI_HostEnable(void)
{
	HOCTLR |= (FIFOCLR + FCSERR_ABT + PECIHEN + CONCTRL);
}

//-----------------------------------------------------------------------
// Disable the PECI host controller.
//-----------------------------------------------------------------------
void PECI_HostDisable(void)
{
	//ben test 20131115 ++ [PATCH]
	//HOCTLR = 0x00;	// Disable will cause the PECI bus to be "floating"
	HOCTLR = 0x08;		// Keep "Enable" for PECI host (EC) to control PECI bus
	//ben test 20131115 --
}

//-----------------------------------------------------------------------
// The PECI host controller will perform the desired transaction.
//-----------------------------------------------------------------------
void PECI_HostControl(BYTE control)
{
	HOCTLR |= control;
}

//-----------------------------------------------------------------------
// Check PECI interface busy or not
// return :
//          0 : 30 ms time out
//          1 : OK
//-----------------------------------------------------------------------
BYTE PECI_CheckHostBusy(void)
{
	PECI_InitTimeOutTimer();

	while (!TF1)
	{
		if (IS_MASK_CLEAR(HOSTAR, HOBY))
		{
			TR1 = 0;
			TF1 = 0;
			ET1 = 1;
			break;
		}
	}

	if (TF1)
	{
		TR1 = 0;
		TF1 = 0;
		ET1 = 1;
		return(0);
	}
	return(1);
}

//-----------------------------------------------------------------------
// Check PECI interface finish or not
// return :
//          0 : time-out or error
//          1 : finish
//-----------------------------------------------------------------------
BYTE PECI_CheckHostFinish(void)
{
	BYTE error;
	BYTE status, result;
	error = 0x04;			// Pre-set error

	PECI_InitTimeOutTimer();
	while (!TF1)
	{
		status = HOSTAR;

		if (status != 0x00)
		{
			if (IS_MASK_SET(status, FINISH))
			{
				error = 0x00;
				break;
			}
			else if (IS_MASK_SET(status, RD_FCS_ERR))
			{
				error = 0x01;
				break;
			}
			else if (IS_MASK_SET(status, WR_FCS_ERR))
			{
				error = 0x01;
				break;
			}
			else if (IS_MASK_SET(status, EXTERR))
			{
				SET_MASK(RSTC4, RPECI);	// Reset PECI interface
				error = 0x02;
				break;
			}
			else if (IS_MASK_SET(status, BUSERR))
			{
				SET_MASK(RSTC4, RPECI);	// Reset PECI interface
				error = 0x02;
				break;
			}
			else if (IS_MASK_SET(status, TEMPERR))
			{
				error = 0x03;
				break;
			}
		}
	}

	if (error != 0x00)
	{
		result = 0x00;	// error
	}
	else
	{
		result = 0x01;	// finish
	}

	TR1 = 0;
	TF1 = 0;
	ET1 = 1;
	return(result);
}

//-----------------------------------------------------------------------
// The function of clearing Host Status Register
//-----------------------------------------------------------------------
void ResetPECIStatus(void)
{
	HOSTAR = 0xFE;
}

//-----------------------------------------------------------------------
// PECI Ping function
//  return :
//              1 : The targeted address of processor is able to respond
//              0 : no respond
//  Input :
//              (1) addr : The address of processor
//              (2) ReadLen read length always 0
//              (3) WriteLen Write length always 0
//-----------------------------------------------------------------------
BYTE PECI_Ping(BYTE addr, BYTE ReadLen, BYTE WriteLen)
{
	BYTE done;
	done = 0x00;

	PECI_HostEnable();
	HOTRADDR = addr;
	HOWRLR = WriteLen;
	HORDLR = ReadLen;

	if (PECI_CheckHostBusy())
	{
		PECI_HostControl(START);
		if (PECI_CheckHostFinish())
		{
			done = 0x01;
		}
	}

	PECI_HostDisable();
	ResetPECIStatus();
	return(done);
}

//-----------------------------------------------------------------------
// PECI get temperature command
//  return :
//              1 : done
//              0 : error
//  Input :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) ReadLen read length always 8
//              (4) WriteLen Write length always 1
//-----------------------------------------------------------------------
BYTE PECI_GetDIB(BYTE addr, XBYTE *ReadData, BYTE ReadLen, BYTE WriteLen)
{
	BYTE done, index;
	done = 0x00;

	PECI_HostEnable();
	HOTRADDR = addr;
	HOWRLR = WriteLen;
	HORDLR = ReadLen;
	HOCMDR = PECI_CMD_GetDIB;

	if (PECI_CheckHostBusy())
	{
		PECI_HostControl(START);
		if (PECI_CheckHostFinish())
		{
			for (index = 0x00; index < HORDLR; index++)
			{
				*(ReadData + index) = HORDDR;
			}
			done = 0x01;
		}
	}

	PECI_HostDisable();
	ResetPECIStatus();
	return(done);
}

//-----------------------------------------------------------------------
// PECI get temperature command
//  return :
//              1 : done
//              0 : error
//  Input :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1
//                           0 : Domain 0, 1 : Domain 1
//              (4) ReadLen read length always 2
//              (5) WriteLen Write length always 1
//-----------------------------------------------------------------------
BYTE PECI_GetTemp(BYTE addr, XBYTE *ReadData, BYTE Domain, BYTE ReadLen, BYTE WriteLen)
{
	BYTE done;
	done = 0x00;

	PECI_HostEnable();
	HOTRADDR = addr;
	HOWRLR = WriteLen;
	HORDLR = ReadLen;
	if (Domain < 2)
	{
		HOCMDR = PECI_CMD_GetTemp + Domain;
	}
	else
	{
		HOCMDR = PECI_CMD_GetTemp;
	}

	if (PECI_CheckHostBusy())
	{
		PECI_HostControl(START);
		if (PECI_CheckHostFinish())
		{
			*ReadData = HORDDR;
			*(++ReadData) = HORDDR;
			done = 0x01;
		}
	}

	PECI_HostDisable();
	ResetPECIStatus();
	return(done);
}

//-----------------------------------------------------------------------
// Read to the package configuration space (PCS) within the processor
//  return :
//              1 : done
//              0 : error
//  Input :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1
//                           0 : Domain 0, 1 : Domain 1
//              (4) Retry   0 or 1
//              (5) Index
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 2 or 3 or 5
//              (9) WriteLen write length 5
//-----------------------------------------------------------------------
BYTE PECI_RdPkgConfig(BYTE addr, XBYTE *ReadData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
	BYTE done, cunt;
	done = 0x00;

	PECI_HostEnable();
	HOTRADDR = addr;
	HOWRLR = WriteLen;
	HORDLR = ReadLen;
	if (Domain < 2)
	{
		HOCMDR = PECI_CMD_RdPkgConfig + Domain;
	}
	else
	{
		HOCMDR = PECI_CMD_RdPkgConfig;
	}

	if (Retry < 2)
	{
		HOWRDR = (PECI_HostID << 1) + Retry;
	}
	else
	{
		HOWRDR = (PECI_HostID << 1);
	}

	HOWRDR = Index;
	HOWRDR = LSB;
	HOWRDR = MSB;

	if (PECI_CheckHostBusy())
	{
		PECI_HostControl(START);
		if (PECI_CheckHostFinish())
		{
			if (HORDLR != 0x00)
			{
				for (cunt = 0x00; cunt < HORDLR; cunt++)
				{
					if (cunt == 0x00)
					{
						PECI_CompletionCode = HORDDR;
					}
					else
					{
						*(ReadData + cunt - 1) = HORDDR;
					}
				}

				if (PECI_CompletionCode == PECI_CC_Valid)
				{
					done = 0x01;
				}
				else
				{
					done = 0x00;
				}
			}
			else
			{
				done = 0x00;
			}
		}
	}

	PECI_HostDisable();
	ResetPECIStatus();
	return(done);
}

//-----------------------------------------------------------------------
// Write to the package configuration space (PCS) within the processor
//  return :
//              1 : done
//              0 : error
//  Input :
//              (1) addr : The address of processor
//              (2) *WriteData : the start address of variable to wirte data
//              (3) Domain : 0 or 1
//                           0 : Domain 0, 1 : Domain 1
//              (4) Retry   0 or 1
//              (5) Index
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 1
//              (9) WriteLen write length 0x07 or 0x08 or 0x0A
//-----------------------------------------------------------------------
BYTE PECI_WrPkgConfig(BYTE addr, XBYTE *WriteData, BYTE Domain, BYTE Retry, BYTE Index, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
	BYTE done, cunt;
	done = 0x00;

	PECI_CompletionCode = 0x00;
	PECI_HostEnable();

	#if	PECI_Softwave_AWFCS
	PECI_CRC8 = 0x00;
	#else
	PECI_HostControl(AWFCS_EN);
	#endif

	HOTRADDR = addr;
	PECI_CalcCRC8(addr);
	HOWRLR = WriteLen;
	PECI_CalcCRC8(WriteLen);
	HORDLR = ReadLen;
	PECI_CalcCRC8(ReadLen);

	if (Domain < 2)
	{
		HOCMDR = PECI_CMD_WrPkgConfig + Domain;
		PECI_CalcCRC8(PECI_CMD_WrPkgConfig + Domain);
	}
	else
	{
		HOCMDR = PECI_CMD_WrPkgConfig;
		PECI_CalcCRC8(PECI_CMD_WrPkgConfig);
	}

	if (Retry < 2)
	{
		HOWRDR = (PECI_HostID << 1) + Retry;
		PECI_CalcCRC8((PECI_HostID << 1) + Retry);
	}
	else
	{
		HOWRDR = (PECI_HostID << 1);
		PECI_CalcCRC8(PECI_HostID << 1);
	}

	HOWRDR = Index;
	PECI_CalcCRC8(Index);
	HOWRDR = LSB;
	PECI_CalcCRC8(LSB);
	HOWRDR = MSB;
	PECI_CalcCRC8(MSB);

	for (cunt = 0x00; cunt < (WriteLen - 6); cunt++)
	{
		HOWRDR = *(WriteData + cunt);
		PECI_CalcCRC8(*(WriteData + cunt));
	}

	#if	PECI_Softwave_AWFCS
	PECI_CRC8 ^= 0x80;			// Inverted MSb of preliminary FCS reslut
	HOWRDR = PECI_CRC8;
	#endif

	if (PECI_CheckHostBusy())
	{
		PECI_HostControl(START);
		if (PECI_CheckHostFinish())
		{
			PECI_CompletionCode = HORDDR;
			if (PECI_CompletionCode == PECI_CC_Valid)
			{
				done = 0x01;
			}
			else
			{
				done = 0x00;
			}
		}
	}

	PECI_HostDisable();
	ResetPECIStatus();
	return(done);
}

//-----------------------------------------------------------------------
// Read to Model Specific Registers function
//  return :
//              1 : done
//              0 : error
//  Input :
//              (1) addr : The address of processor
//              (2) *ReadData : the start address of variable to save data
//              (3) Domain : 0 or 1
//                           0 : Domain 0, 1 : Domain 1
//              (4) Retry   0 or 1
//              (5) ProcessorID
//              (6) LSB of parameter
//              (7) MSB of parameter
//              (8) ReadLen read length 0x02 or 0x03 or 0x05 or 0x09
//              (9) WriteLen write length 0x05
//-----------------------------------------------------------------------
BYTE PECI_RdIAMSR(BYTE addr, XBYTE *ReadData, BYTE Domain, BYTE Retry, BYTE ProcessorID, BYTE LSB, BYTE MSB, BYTE ReadLen, BYTE WriteLen)
{
	BYTE done, cunt;
	done = 0x00;

	PECI_HostEnable();
	HOTRADDR = addr;
	HOWRLR = WriteLen;
	HORDLR = ReadLen;
	if (Domain < 2)
	{
		HOCMDR = PECI_CMD_RdIAMSR + Domain;
	}
	else
	{
		HOCMDR = PECI_CMD_RdIAMSR;
	}

	if (Retry < 2)
	{
		HOWRDR = (PECI_HostID << 1) + Retry;
	}
	else
	{
		HOWRDR = (PECI_HostID << 1);
	}

	HOWRDR = ProcessorID;
	HOWRDR = LSB;
	HOWRDR = MSB;

	if (PECI_CheckHostBusy())
	{
		PECI_HostControl(START);
		if (PECI_CheckHostFinish())
		{
			for (cunt = 0x00; cunt < HORDLR; cunt++)
			{
				if (cunt == 0x00)
				{
					PECI_CompletionCode = HORDDR;
				}
				else
				{
					*(ReadData + cunt - 1) = HORDDR;
				}
			}

			if (PECI_CompletionCode == PECI_CC_Valid)
			{
				done = 0x01;
			}
			else
			{
				done = 0x00;
			}
		}
	}

	PECI_HostDisable();
	ResetPECIStatus();
	return(done);
}

#endif	//PECI_Support

