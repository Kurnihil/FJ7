/*------------------------------------------------------------------------------
 * Title : CORE_SMBUS.C - The system management SMBus protocol drivers.
 * Author:
 *----------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// Read/write SMbus byte/word function
//----------------------------------------------------------------------------
const sSMBus code asSMBus[] =
{
	{ &HOCTL_A, &TRASLA_A, &HOCMD_A, &HOSTA_A, &D0REG_A, &D1REG_A, &HOBDB_A, &IER1, &ISR1, Int_SMBUS0, &PECERC_A},
	{ &HOCTL_B, &TRASLA_B, &HOCMD_B, &HOSTA_B, &D0REG_B, &D1REG_B, &HOBDB_B, &IER1, &ISR1, Int_SMBUS1, &PECERC_B},
	{ &HOCTL_C, &TRASLA_C, &HOCMD_C, &HOSTA_C, &D0REG_C, &D1REG_C, &HOBDB_C, &IER2, &ISR2, Int_SMBUS2, &PECERC_C},
	{ &HOCTL_D, &TRASLA_D, &HOCMD_D, &HOSTA_D, &D0REG_D, &D1REG_D, &HOBDB_D, &IER0, &ISR0, Int_SMBUS3, &PECERC_D}
};

//David_20120327 add for SMBUS reset timing follow SMBUS spec
#ifdef	SMBUS_RESET_SUPPORT
const sAResetSMBusS code asResetSMBusS[]=
{
	{ &HOSTA_A, &HOCTL2_A, &SMBPCTL_A, &GPCRB3, &GPCRB4, &GPDRB, (BIT3+BIT4), BIT3, BIT4, &SMBus1RecoverCunt, &SMBus1Cunt},
	{ &HOSTA_B, &HOCTL2_B, &SMBPCTL_B, &GPCRC1, &GPCRC2, &GPDRC, (BIT1+BIT2), BIT1, BIT2, &SMBus2RecoverCunt, &SMBus2Cunt},
	{ &HOSTA_C, &HOCTL2_C, &SMBPCTL_C, &GPCRF6, &GPCRF7, &GPDRF, (BIT6+BIT7), BIT6, BIT7, &SMBus3RecoverCunt, &SMBus3Cunt},
	{ &HOSTA_D, &HOCTL2_D, &SMBPCTL_D, &GPCRH1, &GPCRH2, &GPDRH, (BIT1+BIT2), BIT1, BIT2, &SMBus4RecoverCunt, &SMBus4Cunt}
};
#else
const sResetSMBusS code asResetSMBusS[] =
{
	{ &HOSTA_A, &HOCTL2_A, &SMBPCTL_A, &GPCRB3, &GPCRB4, &GPDRB, (BIT3 + BIT4), &SMBus1RecoverCunt },
	{ &HOSTA_B, &HOCTL2_B, &SMBPCTL_B, &GPCRC1, &GPCRC2, &GPDRC, (BIT1 + BIT2), &SMBus2RecoverCunt },
	{ &HOSTA_C, &HOCTL2_C, &SMBPCTL_C, &GPCRF6, &GPCRF7, &GPDRF, (BIT6 + BIT7), &SMBus3RecoverCunt },
	{ &HOSTA_D, &HOCTL2_D, &SMBPCTL_D, &GPCRH1, &GPCRH2, &GPDRH, (BIT1 + BIT2), &SMBus4RecoverCunt }
};
#endif
//David_20120327 add for SMBUS reset timing follow SMBUS spec

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void InitSMBus(void)
{
	DB_SMBusAddr = 0x00;
	DB_SMBusFlag1 = 0x00;
	DB_SMBusFlag2 = 0x00;
	DB_SMBusFlag3 = 0x00;

	DB_SMBusAck1 = 0x00;
	DB_SMBusACDID = 0x00;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void DelayInact(void)
{
	BYTE index;
	BYTE counter;
	for (index = 0; index < 200; index++)
	{
		counter++;
	}
}

//----------------------------------------------------------------------------
// The function of reseting SMbus
//----------------------------------------------------------------------------
//David_20120327 add for SMBUS reset timing follow SMBUS spec
#ifdef	SMBUS_RESET_SUPPORT
void CNTResetSMBus(void)
{
	if(*asResetSMBusS[SMbusCh1].SMBCunt != 0)
	{
		(*asResetSMBusS[SMbusCh1].SMBCunt)--;
		if(*asResetSMBusS[SMbusCh1].SMBCunt == 0)
		{
			*asResetSMBusS[SMbusCh1].SMBusPin0 = ALT;
			*asResetSMBusS[SMbusCh1].SMBusPin1 = ALT;
		}
	}

	if(*asResetSMBusS[SMbusCh2].SMBCunt != 0)
	{
		(*asResetSMBusS[SMbusCh2].SMBCunt)--;
		if(*asResetSMBusS[SMbusCh2].SMBCunt == 0)
		{
			*asResetSMBusS[SMbusCh2].SMBusPin0 = ALT;
			*asResetSMBusS[SMbusCh2].SMBusPin1 = ALT;
		}
	}

	if(*asResetSMBusS[SMbusCh3].SMBCunt != 0)
	{
		(*asResetSMBusS[SMbusCh3].SMBCunt)--;
		if(*asResetSMBusS[SMbusCh3].SMBCunt == 0)
		{
			*asResetSMBusS[SMbusCh3].SMBusPin0 = ALT;
			*asResetSMBusS[SMbusCh3].SMBusPin1 = ALT;
		}
	}

	if(*asResetSMBusS[SMbusCh4].SMBCunt != 0)
	{
		(*asResetSMBusS[SMbusCh4].SMBCunt)--;
		if(*asResetSMBusS[SMbusCh4].SMBCunt == 0)
		{
			*asResetSMBusS[SMbusCh4].SMBusPin0 = ALT;
			*asResetSMBusS[SMbusCh4].SMBusPin1 = ALT;
		}
	}
}
#endif
//David_20120327 add for SMBUS reset timing follow SMBUS spec

void ResetSMBus(BYTE Channel)
{
	#ifdef	PECI_Support
	if (Channel == SMbusCh3)
	{
		return;
	}
	#endif

	SET_MASK(*asSMBus[Channel].SMBusCTL, HOCTL_KILL);
	CLEAR_MASK(*asSMBus[Channel].SMBusCTL, HOCTL_KILL);
	//RSTC3=0x0F;								// Reset all SMBus module
	*asResetSMBusS[Channel].SMBusSTA = 0xFE;	// clear bits
	//David_20120327 add for SMBUS reset timing follow SMBUS spec
	#ifdef	SMBUS_RESET_SUPPORT
	*asResetSMBusS[Channel].SMBusPin0 = OUTPUT;
	*asResetSMBusS[Channel].SMBusPin1 = OUTPUT;
	DelayInact();
	*asResetSMBusS[Channel].GPIOReg |= asResetSMBusS[Channel].GPIOData;			// Output High
	DelayInact();
	//*asResetSMBusS[Channel].SMBusPin0 = ALT;
	//*asResetSMBusS[Channel].SMBusPin1 = ALT;
	//*asResetSMBusS[Channel].SMBusCTL2 = 0x02;			// I2C Enable support I2C-compatible cycle
	#if	0	// Fox_20120613 modify for fix always reset SMBus clock [PATCH]
	if ((*asResetSMBusS[Channel].GPIOReg & asResetSMBusS[Channel].DataGPIO) != 1)
	#endif
	{
		//*asResetSMBusS[Channel].SMBusCTL2 = 0x02;		// I2C Enable support I2C-compatible cycle
		//*asResetSMBusS[Channel].SMBusPinCTL = 0x1B;	// SMDAT0/1/2 pin will not be deiven low & SMCLK0/1/2 pin will be deiven low
		//DelayXms(35);
		//*asResetSMBusS[Channel].SMBusPinCTL = 0x1F;	// SMDAT0/1/2 & SMCLK0/1/2 pin will not be deiven low
		//*asResetSMBusS[Channel].SMBusCTL2 = 0x01;		// SMBUS Host Enable
	*asResetSMBusS[Channel].GPIOReg &= (~asResetSMBusS[Channel].CLKGPIO);		// SMCLK0/1/2 output Low
	*asResetSMBusS[Channel].SMBCunt = 50;				// 35ms delay
	}
	//*asResetSMBusS[Channel].SMBusCTL2 = 0x01;			// SMBUS Host Enable
	#else
	*asResetSMBusS[Channel].SMBusPin0 = OUTPUT;
	*asResetSMBusS[Channel].SMBusPin1 = OUTPUT;
	DelayInact();
	*asResetSMBusS[Channel].GPIOReg |= asResetSMBusS[Channel].GPIOData;			// Output High
	DelayInact();
	*asResetSMBusS[Channel].SMBusPin0 = ALT;
	*asResetSMBusS[Channel].SMBusPin1 = ALT;
	*asResetSMBusS[Channel].SMBusCTL2 = 0x02;			// I2C Enable support I2C-compatible cycle
	*asResetSMBusS[Channel].SMBusPinCTL = 0x03;			// SMDAT0/1/2 & SMCLK0/1/2 pin will be deiven low
	DelayInact();
	*asResetSMBusS[Channel].SMBusPinCTL = 0x07;			// SMDAT0/1/2 pin will not be deiven low & SMCLK0/1/2 pin will be deiven low
	*asResetSMBusS[Channel].SMBusCTL2 = 0x01;			// SMBUS Host Enable
	#endif
	//David_20120327 add for SMBUS reset timing follow SMBUS spec

	//*asSSC[Channel].inusing=0x00;
	CLEAR_MASK(*asSMBus[Channel].SMBusIER, asSMBus[Channel].IERFlag);
	*asSMBus[Channel].SMBusISR = asSMBus[Channel].IERFlag;
	//*asSSC[Channel].serviceflag=0x00;
	//Init_SMBus_Regs();

	if (DB_SMBusAck1 == ACK_Start)	// For SMBus utility
	{
		DB_SMBusAddr = 0x00;
		if (Channel == 0x00)
		{
			DB_SMBusFlag1 = 0x00;
		}
		else if (Channel == 0x01)
		{
			DB_SMBusFlag2 = 0x00;
		}
		else if (Channel == 0x02)
		{
			DB_SMBusFlag3 = 0x00;
		}

		DB_SMBusACDID = 0x00;
		DB_SMBusAck1 = ACK_Error;	// For SMBus utility error ACK.
	}
}

//----------------------------------------------------------------------------
// SMbus read byte/word and write byte/word function
//----------------------------------------------------------------------------
BYTE bRWSMBus(BYTE Channel, BYTE Protocol, BYTE Addr, BYTE Comd, XBYTE *Var, BYTE PECSupport)
{
	BYTE error;
	BYTE status;
	BYTE resutl;

	if (CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn) == SMBus_CanNotUse)
	{
		resutl = FALSE;								// SMBus interface can't be used.
	}
	else
	{
		error = 0xEE;								// Pre-set error
		resutl = FALSE;								// Pre-set result is fail

		SMBCRC8_A = 0x00;							// Clear CRC variable

		if (Protocol & 0x80)						// Write byte function
		{
			if (PECSupport)
			{
				CalcCRC8(Addr);						// To calculate crc8
			}
			*asSMBus[Channel].SMBusADR = Addr;		// set address with writing bit
			if (PECSupport)
			{
				CalcCRC8(Comd);						// To calculate crc8
			}
			*asSMBus[Channel].SMBusCMD = Comd;		// Set command
			Protocol &= (~0x80);
			if (PECSupport)
			{
				CalcCRC8(*Var);						// To calculate crc8
			}
			*asSMBus[Channel].SMBusData0 = *Var;	// set data1
			if (Protocol == SMbusRW)				// write word function
			{
				if (PECSupport)
				{
					CalcCRC8(*(Var + 0x01));		// To calculate crc8
				}
				*asSMBus[Channel].SMBusData1 = *(Var + 0x01);
			}										// set data2
		}
		else										// Read function
		{
			*asSMBus[Channel].SMBusADR = Addr | 0x01;
			*asSMBus[Channel].SMBusCMD = Comd;		// Set command
		}											// set address with reading bit

		*asSMBus[Channel].SMBusSTA = 0xFE;			// clear bits
		if (PECSupport)
		{
			*asSMBus[Channel].SMBusPEC = SMBCRC8_A;
			*asSMBus[Channel].SMBusCTL = (Protocol | HOCTL_SRT | HOCTL_PEC_EN);	// Start transaction
		}
		else
		{
			*asSMBus[Channel].SMBusCTL = (Protocol | HOCTL_SRT);	// Start transaction
		}

		TR1 = 0;									// disable timer1
		ET1 = 0;									// Disable timer1 interrupt
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		TH1 = Timer_26ms >> 8;						// Set timer1 counter 26ms
		TL1 = Timer_26ms;							// Set timer1 counter 26ms
		TF1 = 0;									// Clear overflow flag
		TR1 = 1;									// Enable timer1

		while (!TF1)
		{
			status = *asSMBus[Channel].SMBusSTA;	// Read SMBus Host Status
			if (IS_MASK_SET(status, (HOSTA_FINTR + HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
			{
				TR1 = 0;
				TF1 = 0;
				ET1 = 1;							// Enable timer1 interrupt
				break;
			}
		}

		if (TF1)									// 26ms time-out and no any status bit is set.
		{
			TR1 = 0;
			TF1 = 0;
			ET1 = 1;								// Enable timer1 interrupt
			ResetSMBus(Channel);
			error = 0xEE;
		}
		else
		{
			if (IS_MASK_SET(status, (HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
			{
				if (IS_MASK_SET(status, (HOSTA_BSER + HOSTA_FAIL)))
				{
					ResetSMBus(Channel);
				}
				error = 0xEE;
			}
			else									// Only Finish Interrupt bit is set.
			{
				error = 0x00;
			}
		}

		if (error == 0xEE)							// Fail
		{
			resutl = FALSE;
		}
		else										// OK
		{
			if ((*asSMBus[Channel].SMBusADR & 0x01 != 0x00))
			{
				*Var = *asSMBus[Channel].SMBusData0;	// read data1
				if (Protocol == SMbusRW)
				{
					*(Var + 0x01) = *asSMBus[Channel].SMBusData1;
				}										// read data2
			}
			resutl = TRUE;
		}

		*asSMBus[Channel].SMBusSTA = 0xFE;				// clear bits
	}

	return(resutl);
}

//----------------------------------------------------------------------------
// Read SMbus block function
//----------------------------------------------------------------------------
BYTE bRSMBusBlock(BYTE Channel, BYTE Protocol, BYTE Addr, BYTE Comd, XBYTE *Var)
{
	BYTE ack;
	BYTE bcount;
	BYTE status;

	if (CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn) == SMBus_CanNotUse)
	{
		ack = FALSE;							// SMBus interface can't be used.
	}
	else
	{
		ack = FALSE;							// pr-set flag
		bcount = 0x00;							// clear byte counter

		*asSMBus[Channel].SMBusADR = Addr | 0x01;	// set address with reading bit
		*asSMBus[Channel].SMBusCMD = Comd;		// Set command
		*asSMBus[Channel].SMBusSTA = 0xFE;		// clear bits
		*asSMBus[Channel].SMBusCTL = (Protocol | HOCTL_SRT);	// Start transaction

		TR1 = 0;								// disable timer1
		ET1 = 0;								// Disable timer1 interrupt
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		TH1 = Timer_26ms >> 8;					// Set timer1 counter 26ms
		TL1 = Timer_26ms;						// Set timer1 counter 26ms
		TF1 = 0;								// Clear overflow flag
		TR1 = 1;								// Enable timer1

		while (!TF1)							// Not time out
		{
			status = *asSMBus[Channel].SMBusSTA;	// Read SMBus Host Status
			if (IS_MASK_SET(status, (HOSTA_BDS + HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
			{
				if (IS_MASK_SET(status, (HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
				{
					if (IS_MASK_SET(status, (HOSTA_BSER + HOSTA_FAIL)))
					{
						ResetSMBus(Channel);
					}
					TR1 = 0;					// disable timer1
					TF1 = 0;					// clear overflow flag
					ET1 = 1;					// Enable timer1 interrupt
					ack = FALSE;
					break;
				}
				else							// Only Byte Done Status bit is set.
				{
					*Var = *asSMBus[Channel].SMBusBData;		// get first data
					if (bcount == 0x00)
					{
						bcount = *asSMBus[Channel].SMBusData0;	// get the data of byte count
						SMB_BCNT = bcount;		// For ACPI SMBus EC SMB1 Block Count register
						// fox 2011/11/04 fix read block over 32 byte
						if(SMB_BCNT > 32)
						{
							TR1 = 0;			// disable timer1
							TF1 = 0;			// clear overflow flag
							ET1 = 1;			// Enable timer1 interrupt
							ResetSMBus(Channel);
							ack = FALSE;
							break;
						}
						// fox 2011/11/04 fix read block over 32 byte
					}

					bcount--;
					Var++;

					if (bcount == 0x01)
					{
						// set last byte flag
						SET_MASK(*asSMBus[Channel].SMBusCTL, HOCTL_LABY);
					}

					*asSMBus[Channel].SMBusSTA = 0xFE;	// clear bits

					if (bcount == 0x00)
					{
						TR1 = 0;				// disable timer1
						TF1 = 0;				// clear overflow flag
						ET1 = 1;				// Enable timer1 interrupt
						ack = TRUE;
						break;					// SMBus read block done.
					}
				}
			}
		}

		if (TF1)								// time-out
		{
			TR1 = 0;							// disable timer1
			TF1 = 0;							// clear overflow flag
			ET1 = 1;							// Enable timer1 interrupt
			ResetSMBus(Channel);
			ack = FALSE;
		}

		CLEAR_MASK(*asSMBus[Channel].SMBusCTL, HOCTL_LABY);	// clear last byte flag
		*asSMBus[Channel].SMBusSTA = 0xFE;		// clear bits
	}

	return(ack);
}

//------------------------------------------------------------------------------
// Write SMbus block function
//------------------------------------------------------------------------------
BYTE bWSMBusBlock(BYTE Channel, BYTE Protocol, BYTE Addr, BYTE Comd, XBYTE *Var, BYTE ByteCont, BYTE PECsupport)
{	// leox_20120711 [PATCH] Add for SMBus block write (Code provided by Fox Zhu)
	BYTE ack;
	BYTE BCTemp;
	BYTE status;

	if (CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn) == SMBus_CanNotUse)
	{
		ack = FALSE;	// SMBus interface can't be used
	}
	else
	{
		ack = FALSE;	// Preset flag
		BCTemp = 0;		// Clear byte count

		if ((ByteCont < 2) || (ByteCont > 32)) {return ack;}	// fox

		if (PECsupport)
		{
			SMBCRC8_A = 0x00;
			CalcCRC8(Addr);
			CalcCRC8(Comd);
			CalcCRC8(ByteCont);
			CalcCRC8(*Var);
		}

		Protocol &= (~0x80);
		*asSMBus[Channel].SMBusADR = Addr;			// Set address with writing flag
		*asSMBus[Channel].SMBusCMD = Comd;			// Set command
		*asSMBus[Channel].SMBusData0 = ByteCont;	// Set byte count
		BCTemp = ByteCont;							// Sync byte count
		*asSMBus[Channel].SMBusBData = *Var;		// Set first data
		*asSMBus[Channel].SMBusSTA = 0xFE;			// Clear bits
		if (PECsupport)								// Start transaction
		{
			*asSMBus[Channel].SMBusCTL = (Protocol | HOCTL_SRT | HOCTL_PEC_EN);
		}
		else
		{
			*asSMBus[Channel].SMBusCTL = (Protocol | HOCTL_SRT);
		}

		TR1 = 0;				// Disable timer1
		ET1 = 0;				// Disable timer1 interrupt
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		TH1 = Timer_26ms >> 8;	// Set timer1 counter 26ms
		TL1 = Timer_26ms;		// Set timer1 counter 26ms
		TF1 = 0;				// Clear overflow flag
		TR1 = 1;				// Enable timer1

		while (!TF1)			// Not time-out
		{
			status = *asSMBus[Channel].SMBusSTA;	// Read SMBus host status
			if (IS_MASK_SET(status, (HOSTA_BDS + HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
			{
				if (IS_MASK_SET(status, (HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
				{
					if (IS_MASK_SET(status, (HOSTA_BSER + HOSTA_FAIL))) {ResetSMBus(Channel);}
					TR1 = 0;	// Disable timer1
					TF1 = 0;	// Clear overflow flag
					ET1 = 1;	// Enable timer1 interrupt
					ack = FALSE;
					break;
				}
				else			// Only Byte Done Status bit is set
				{
					Var++;		// Point to next address of variable
					BCTemp--;
					if (BCTemp != 0)
					{
						if (PECsupport)
						{
							*asSMBus[Channel].SMBusBData = *Var;
							CalcCRC8(*Var);
							if (BCTemp == 1) {*asSMBus[Channel].SMBusPEC = SMBCRC8_A;}	// Last byte of byte count
						}
						else
						{
							*asSMBus[Channel].SMBusBData = *Var;
						}
						*asSMBus[Channel].SMBusSTA = 0xFE;	// Clear bits
					}
					else			// Write block transaction done
					{
						TR1 = 0;	// Disable timer1
						TF1 = 0;	// Clear overflow flag
						ET1 = 1;	// Enable timer1 interrupt
						ack = TRUE;
						break;
					}
				}
			}
		}

		if (TF1)				// Time-out
		{
			TR1 = 0;			// Disable timer1
			TF1 = 0;			// Clear overflow flag
			ET1 = 1;			// Enable timer1 interrupt
			ResetSMBus(Channel);
			ack = FALSE;
		}
		*asSMBus[Channel].SMBusSTA = 0xFE;	// Clear bits
	}

	return ack;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CalcCRC8(BYTE sourcebyte)
{
	BYTE temp;
	SMBCRC8_A ^= sourcebyte;
	temp = SMBCRC8_A;

	if (IS_MASK_CLEAR(temp, BIT7))
	{
		temp = temp << 1;
		SMBCRC8_A ^= temp;
	}
	else
	{
		temp = temp << 1;
		SMBCRC8_A ^= 0x09;
		SMBCRC8_A ^= temp;
	}

	if (IS_MASK_CLEAR(temp, BIT7))
	{
		temp = temp << 1;
		SMBCRC8_A ^= temp;
	}
	else
	{
		temp = temp << 1;
		SMBCRC8_A ^= 0x07;
		SMBCRC8_A ^= temp;
	}
}

#ifdef	SMBusChannel3Support	// leox_20131218 [PATCH]
//----------------------------------------------------------------
//Function: Enable SMBus channel 3
//----------------------------------------------------------------
void InitSMBusChannel3(void)	// leox_20131218 [PATCH]
{
	GPCRH1 = ALT;		// SMBCLK3
	GPCRH2 = ALT;		// SMBDAT3
	GCR2 |= SMB3E;		// SMBus Channel 3 Enable.  // leox_20131218 [PATCH]
}
#endif

//----------------------------------------------------------------------------
// The function of Checking SMbus clock and data pins are both high
//----------------------------------------------------------------------------
BYTE CheckSMBusFree(BYTE channel)
{
	BYTE pinstatus;

	pinstatus = *asResetSMBusS[channel].SMBusPinCTL;

	if ((pinstatus &= 0x03) == 0x03)	// Check BIT0 and BIT1
	{
		return(SMBus_Free);
	}
	else
	{
		return(SMBus_Busy);
	}
}

//----------------------------------------------------------------------------
// The function of Checking SMbus pins are all alt
//----------------------------------------------------------------------------
BYTE CheckSMBusInterfaceIsALT(BYTE channel)
{
	if (((*asResetSMBusS[channel].SMBusPin0 & 0xFB) != 0x00) || ((*asResetSMBusS[channel].SMBusPin1 & 0xFB) != 0x00))
	{
		return(SMBusPin_GPIO);
	}
	else
	{
		return(SMBusPin_ALT);
	}
}

//----------------------------------------------------------------------------
// The function of Checking SMbus pins are all alt
//----------------------------------------------------------------------------
BYTE CheckSMBusInterfaceCanbeUse(BYTE channel, BYTE accesstype)
{
	BYTE checksmbus;

	checksmbus = SMBus_CanUse;					// Pre-set smbus interface can be used

	#ifdef	PECI_Support
	if (channel == SMbusCh3)					// If PECI function enable, return.
	{
		checksmbus = SMBus_CanNotUse;			// Interface can't be used
		return(checksmbus);						// return result
	}
	#endif

	if (CheckSMBusInterfaceIsALT(channel) == SMBusPin_GPIO)	// SMBus interface is GPIO function
	{
		checksmbus = SMBus_CanNotUse;			// Interface can't be used
	}
	else										// SMBus interface is ALT function
	{
		if (accesstype == SMBus_AccessType_Fucn)
		{
			//WatiSMBusCHxFree(channel);			// Check smbus is in used or not.
		}

		if (CheckSMBusFree(channel) == SMBus_Busy)
		{
			checksmbus = SMBus_CanNotUse;		// Interface can't be used
			if ((*asResetSMBusS[channel].recoverCunt)++ > SMBus_BusyRecover)
			{
				ResetSMBus(channel);
			}
		}
		else
		{
			*asResetSMBusS[channel].recoverCunt = 0x00;
			checksmbus = SMBus_CanUse;			// smbus interface can be used
		}
	}

	return(checksmbus);							// return result
}


#if	Support_SMBus_I2C	// leox_20120725 [PATCH] For I2C-compatible command
//------------------------------------------------------------------------------
// I2C-compatible read function
//------------------------------------------------------------------------------
// leox_20120712 Create function for I2C-compatible command
BYTE bRSMBusExt(BYTE Channel, BYTE Protocol, BYTE Addr, XBYTE *Var, BYTE BCnt)
{
	BYTE ack = FALSE;
	BYTE bcount = BCnt;
	BYTE status;

	if (CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn) == SMBus_CanNotUse) {return ack;}
	#if	SMBus_I2C_BIG_BLOCK
	if (BCnt < 2) {return ack;}
	#else
	if ((BCnt < 2) || (BCnt > 32)) {return ack;}
	#endif

	SET_MASK(*asResetSMBusS[Channel].SMBusCTL2, I2C_EN);
	*asSMBus[Channel].SMBusADR = Addr | 1;	// Set address with reading bit
	*asSMBus[Channel].SMBusSTA = 0xFE;		// Clear bits
	*asSMBus[Channel].SMBusCTL = (Protocol | HOCTL_SRT);	// Start transaction

	TR1 = 0;				// Disable timer1
	ET1 = 0;				// Disable timer1 interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	#if	SMBus_I2C_BIG_BLOCK
	TH1 = Timer_85ms >> 8;	// Set timer1 counter 85ms
	TL1 = Timer_85ms;		// Set timer1 counter 85ms
	#else
	TH1 = Timer_26ms >> 8;	// Set timer1 counter 26ms
	TL1 = Timer_26ms;		// Set timer1 counter 26ms
	#endif
	TF1 = 0;				// Clear overflow flag
	TR1 = 1;				// Enable timer1

	while (!TF1)			// Not time-out
	{
		status = *asSMBus[Channel].SMBusSTA;	// Read SMBus Host Status
		if (IS_MASK_SET(status, (HOSTA_BDS + HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
		{
			if (IS_MASK_SET(status, (HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
			{
				if (IS_MASK_SET(status, (HOSTA_BSER + HOSTA_FAIL))) {ResetSMBus(Channel);}
				TR1 = 0;	// Disable timer1
				TF1 = 0;	// Clear overflow flag
				ET1 = 1;	// Enable timer1 interrupt
				ack = FALSE;
				break;
			}
			else			// Only Byte Done Status bit is set
			{
				bcount--;
				if (bcount == 1) {SET_MASK(*asSMBus[Channel].SMBusCTL, HOCTL_LABY);}	// Set last byte flag
				*Var++ = *asSMBus[Channel].SMBusBData;	// Get received data
				*asSMBus[Channel].SMBusSTA = 0xFE;		// Clear bits
				if (bcount == 0)
				{
					TR1 = 0;	// Disable timer1
					TF1 = 0;	// Clear overflow flag
					ET1 = 1;	// Enable timer1 interrupt
					ack = TRUE;
					break;
				}
			}
		}
	}

	if (TF1)				// Time-out
	{
		TR1 = 0;			// Disable timer1
		TF1 = 0;			// Clear overflow flag
		ET1 = 1;			// Enable timer1 interrupt
		ResetSMBus(Channel);
		ack = FALSE;
	}

	CLR_MASK(*asSMBus[Channel].SMBusCTL, HOCTL_LABY);	// Clear last byte flag
	CLR_MASK(*asResetSMBusS[Channel].SMBusCTL2, I2C_EN);
	*asSMBus[Channel].SMBusSTA = 0xFE;	// Clear bits

	return ack;
}


//------------------------------------------------------------------------------
// I2C-compatible write function
//------------------------------------------------------------------------------
// leox_20120712 Create function for I2C-compatible command
BYTE bWSMBusExt(BYTE Channel, BYTE Protocol, BYTE Addr, XBYTE *Var, BYTE BCnt)
{
	BYTE ack = FALSE;
	BYTE bcount = BCnt;
	BYTE status;

	if (CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn) == SMBus_CanNotUse) {return ack;}
	#if	SMBus_I2C_BIG_BLOCK
	if (BCnt < 2) {return ack;}
	#else
	if ((BCnt < 2) || (BCnt > 32)) {return ack;}
	#endif

	Protocol &= (~0x80);
	SET_MASK(*asResetSMBusS[Channel].SMBusCTL2, I2C_EN);
	*asSMBus[Channel].SMBusADR = Addr;		// Set address with writing flag
	*asSMBus[Channel].SMBusBData = *Var;	// Set first data
	*asSMBus[Channel].SMBusSTA = 0xFE;		// Clear bits
	*asSMBus[Channel].SMBusCTL = (Protocol | HOCTL_SRT);	// Start transaction

	TR1 = 0;				// Disable timer1
	ET1 = 0;				// Disable timer1 interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	#if	SMBus_I2C_BIG_BLOCK
	TH1 = Timer_85ms >> 8;	// Set timer1 counter 85ms
	TL1 = Timer_85ms;		// Set timer1 counter 85ms
	#else
	TH1 = Timer_26ms >> 8;	// Set timer1 counter 26ms
	TL1 = Timer_26ms;		// Set timer1 counter 26ms
	#endif
	TF1 = 0;				// Clear overflow flag
	TR1 = 1;				// Enable timer1

	while (!TF1)			// Not time-out
	{
		status = *asSMBus[Channel].SMBusSTA;	// Read SMBus host status
		if (IS_MASK_SET(status, (HOSTA_BDS + HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
		{
			if (IS_MASK_SET(status, (HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
			{
				if (IS_MASK_SET(status, (HOSTA_BSER + HOSTA_FAIL))) {ResetSMBus(Channel);}
				TR1 = 0;	// Disable timer1
				TF1 = 0;	// Clear overflow flag
				ET1 = 1;	// Enable timer1 interrupt
				ack = FALSE;
				break;
			}
			else			// Only Byte Done Status bit is set
			{
				Var++;		// Point to next address of variable
				bcount--;
				if (bcount != 0)
				{
					*asSMBus[Channel].SMBusBData = *Var;
					*asSMBus[Channel].SMBusSTA = 0xFE;	// Clear bits
				}
				else			// Write block transaction done
				{
					TR1 = 0;	// Disable timer1
					TF1 = 0;	// Clear overflow flag
					ET1 = 1;	// Enable timer1 interrupt
					ack = TRUE;
					break;
				}
			}
		}
	}

	if (TF1)				// Time-out
	{
		TR1 = 0;			// Disable timer1
		TF1 = 0;			// Clear overflow flag
		ET1 = 1;			// Enable timer1 interrupt
		ResetSMBus(Channel);
		ack = FALSE;
	}

	CLR_MASK(*asResetSMBusS[Channel].SMBusCTL2, I2C_EN);
	*asSMBus[Channel].SMBusSTA = 0xFE;	// Clear bits

	return ack;
}
#endif	// Support_SMBus_I2C

//------------------------------------------------------------------------------
// SMBus send/receive byte function
//------------------------------------------------------------------------------
// leox_20140115 [PATCH] Add for SMBus send/receive byte command
BYTE bSMBusSendRecvByte(BYTE Channel, BYTE Protocol, BYTE Addr, XBYTE *Var, BYTE PECSupport)
{
	BYTE Result = FALSE;	// Preset result is fail
	BYTE Status;

	// Check is SMBus interface available
	if (CheckSMBusInterfaceCanbeUse(Channel, SMBus_AccessType_Fucn) == SMBus_CanNotUse) {return FALSE;}

	SMBCRC8_A = 0x00;						// Clear CRC variable
	if (Protocol & 0x80)					// Send byte command
	{
		Protocol &= (~0x80);
		CLR_MASK(Addr, BIT0);				// Set address with writing bit
		if (PECSupport) {CalcCRC8(*Var);}	// Calculate CRC8
		*asSMBus[Channel].SMBusCMD = *Var;	// Set data
	}
	else									// Receive byte command
	{
		SET_MASK(Addr, BIT0);				// Set address with reading bit
	}
	if (PECSupport) {CalcCRC8(Addr);}		// Calculate CRC8
	*asSMBus[Channel].SMBusADR = Addr;		// Set address
	*asSMBus[Channel].SMBusSTA = 0xFE;		// Clear bits

	if (PECSupport)
	{
		*asSMBus[Channel].SMBusPEC = SMBCRC8_A;
		Protocol |= HOCTL_PEC_EN;
	}
	*asSMBus[Channel].SMBusCTL = (Protocol | HOCTL_SRT);	// Start transaction

	TR1 = 0;				// Disable timer1
	ET1 = 0;				// Disable timer1 interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	TH1 = Timer_26ms >> 8;	// Set timer1 counter 26ms
	TL1 = Timer_26ms;		// Set timer1 counter 26ms
	TF1 = 0;				// Clear overflow flag
	TR1 = 1;				// Enable timer1

	while (!TF1)
	{
		Status = *asSMBus[Channel].SMBusSTA;	// Read SMBus Host Status
		if (IS_MASK_SET(Status, (HOSTA_FINTR + HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE)))
		{
			TR1 = 0;
			TF1 = 0;
			ET1 = 1;		// Enable timer1 interrupt
			break;
		}
	}

	if (TF1)	// Time-out and no any status bit is set
	{
		TR1 = 0;
		TF1 = 0;
		ET1 = 1;
		ResetSMBus(Channel);
		Result = FALSE;
	}
	else
	{
		if (IS_MASK_SET(Status, HOSTA_DVER + HOSTA_BSER + HOSTA_FAIL + HOSTA_NACK + HOSTA_TMOE))
		{
			if (IS_MASK_SET(Status, HOSTA_BSER + HOSTA_FAIL)) {ResetSMBus(Channel);}
			Result = FALSE;
		}
		else	// Only Finish Interrupt bit is set
		{
			if (IS_MASK_SET(Addr, BIT0)) {*Var = *asSMBus[Channel].SMBusData0;}	// Receive byte
			Result = TRUE;
		}
	}

	*asSMBus[Channel].SMBusSTA = 0xFE;	// Clear bits

	return Result;
}
