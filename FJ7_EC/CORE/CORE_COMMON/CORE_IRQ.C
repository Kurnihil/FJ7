/*------------------------------------------------------------------------------
 * Title: CORE_IRQ.C - Interrupt Service Routines (ISR).
 *
 * The Interrupt Service Routines (ISR).  The actual interrupt functions are
 * implemented in the "CHIP_IRQ.C".
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//#pragma NOAREGS

//----------------------------------------------------------------------------
// Input full irq lock scan
//----------------------------------------------------------------------------
void IrqLock_scan(void)	//using 2
{
	Int_Var.Scan_Lock = 1;
	//Timer_B.fbit.SEND_ENABLE = 0;	// Lock scanner.
}

//-----------------------------------------------------------------------------
// AUX port1 interrupt routine
//-----------------------------------------------------------------------------
void Irq_Aux_Port1(void)	//using 2
{
	if (IS_MASK_SET(PSSTS1, TDS))	// Transaction done interrupt
	{
		CLEAR_MASK(IER2, Int_PS2_0);	// Disable PS2 interrupt 0
		ISR2 = Int_PS2_0;			// Write to clear pending interrupt

		if (SendtoAUXFlag)
		{
			for (PS2DataPinStatusCounter = 0x00; PS2DataPinStatusCounter < 5; PS2DataPinStatusCounter++)
			{
				if (IS_MASK_SET(PSSTS1, DLS))	// DATA Line Status is high
				{
					break;
				}
				else
				{
					WNCKR = 0x00;	// Delay 15.26 us
				}
			}
		}
		else
		{
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
		}

		PSCTL1 = PS2_InhibitMode;	// Inhibit clock pin1
		PSCTL2 = PS2_InhibitMode;	// Inhibit clock pin2
		PSCTL3 = PS2_InhibitMode;	// Inhibit clock pin3

		PS2_IRQ_Channel = 0;		// Set PS2 0 service flag
		F_Service_PS2 = 1;			// Set main service flag
		PS2StartBit = 0;			// clear start bit flag
		PS2_SSIRQ_Channel = 0xFF;	//
		PS2PortxData[0] = PSDAT1;
	}
	else							// Start bit interrupt
	{
		PSCTL2 = PS2_InhibitMode;	// Inhibit clock pin2
		PSCTL3 = PS2_InhibitMode;	// Inhibit clock pin3
		IER2 &= ~(Int_PS2_0 + Int_PS2_1 + Int_PS2_2);	// Disable all PS2 interrupt
		ISR2 = (Int_PS2_0 + Int_PS2_1 + Int_PS2_2);	// Write to clear all PS2 pending interrupt
		PS2StartBit = 1;			// Set start bit flag
		PS2_SSIRQ_Channel = 0;		//
		SET_MASK(IER2, Int_PS2_0);	// Enable PS2 interrupt 0
	}
}

//----------------------------------------------------------------------------
// AUX port2 interrupt routine
//----------------------------------------------------------------------------
void Irq_Aux_Port2(void)	//using 2
{
	if (IS_MASK_SET(PSSTS2, TDS))	// Transaction done interrupt
	{
		CLEAR_MASK(IER2, Int_PS2_1);	// Disable PS2 interrupt 1
		ISR2 = Int_PS2_1;			// Write to clear pending interrupt

		if (SendtoAUXFlag)
		{
			for (PS2DataPinStatusCounter = 0x00; PS2DataPinStatusCounter < 5; PS2DataPinStatusCounter++)
			{
				if (IS_MASK_SET(PSSTS2, DLS))	// DATA Line Status is high
				{
					break;
				}
				else
				{
					WNCKR = 0x00;	// Delay 15.26 us
				}
			}
		}
		else
		{
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
		}

		PSCTL1 = PS2_InhibitMode;	// Inhibit clock pin1
		PSCTL2 = PS2_InhibitMode;	// Inhibit clock pin2
		PSCTL3 = PS2_InhibitMode;	// Inhibit clock pin3

		PS2_IRQ_Channel = 1;		// Set PS2 1 service flag
		F_Service_PS2 = 1;			// Set main service flag
		PS2StartBit = 0;			// clear start bit flag
		PS2_SSIRQ_Channel = 0xFF;	//
		PS2PortxData[1] = PSDAT2;
	}
	else							// Start bit interrupt
	{
		PSCTL1 = PS2_InhibitMode;	// Inhibit clock pin1
		PSCTL3 = PS2_InhibitMode;	// Inhibit clock pin3
		IER2 &= ~(Int_PS2_0 + Int_PS2_1 + Int_PS2_2);	// Disable all PS2 interrupt
		ISR2 = (Int_PS2_0 + Int_PS2_1 + Int_PS2_2);	// Write to clear all PS2 pending interrupt
		PS2StartBit = 1;			// Set start bit flag
		PS2_SSIRQ_Channel = 1;		//
		SET_MASK(IER2, Int_PS2_1);	// Enable PS2 interrupt 1
	}
}

//----------------------------------------------------------------------------
// AUX port3 interrupt routine
//----------------------------------------------------------------------------
void Irq_Aux_Port3(void)	//using 2
{
	if (IS_MASK_SET(PSSTS3, TDS))	// Transaction done interrupt
	{
		CLEAR_MASK(IER2, Int_PS2_2);	// Disable PS2 interrupt 2
		ISR2 = Int_PS2_2;			// Write to clear pending interrupt

		if (SendtoAUXFlag)
		{
			for (PS2DataPinStatusCounter = 0x00; PS2DataPinStatusCounter < 5; PS2DataPinStatusCounter++)
			{
				if (IS_MASK_SET(PSSTS3, DLS))	// DATA Line Status is high
				{
					break;
				}
				else
				{
					WNCKR = 0x00;	// Delay 15.26 us
				}
			}
		}
		else
		{
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
			WNCKR = 0x00;			// Delay 15.26 us
		}

		PSCTL1 = PS2_InhibitMode;	// Inhibit clock pin1
		PSCTL2 = PS2_InhibitMode;	// Inhibit clock pin2
		PSCTL3 = PS2_InhibitMode;	// Inhibit clock pin3

		PS2_IRQ_Channel = 2;		// Set PS2 2 service flag
		F_Service_PS2 = 1;			// Set main service flag
		PS2StartBit = 0;			// clear start bit flag
		PS2_SSIRQ_Channel = 0xFF;	//
		PS2PortxData[2] = PSDAT3;
	}
	else							// Start bit interrupt
	{
		PSCTL1 = PS2_InhibitMode;	// Inhibit clock pin1
		PSCTL2 = PS2_InhibitMode;	// Inhibit clock pin2
		IER2 &= ~(Int_PS2_0 + Int_PS2_1 + Int_PS2_2);	// Disable all PS2 interrupt
		ISR2 = (Int_PS2_0 + Int_PS2_1 + Int_PS2_2);	// Write to clear all PS2 pending interrupt
		PS2StartBit = 1;			// Set start bit flag
		PS2_SSIRQ_Channel = 2;		//
		SET_MASK(IER2, Int_PS2_2);	// Enable PS2 interrupt 2
	}
}

//----------------------------------------------------------------------------
// Port 60/64 input buffer full interrupt routine
//----------------------------------------------------------------------------
void Irq_Ibf1(void)	//using 2
{
	IrqLock_scan();
	CLEAR_MASK(IER3, Int_KBCIBF);
	ISR3 = Int_KBCIBF;
	F_Service_PCI = 1;
}

//----------------------------------------------------------------------------
// Port 62/66, 68/6C input buffer full interrupt routine
//----------------------------------------------------------------------------
void Irq_Ibf2(void)	//using 2
{
	if (IS_MASK_SET(PM2STS, P_IBF))	// 68/6C input buffer full
	{
		F_Service_PCI3 = 1;
	}
	else						// 62/66 input buffer full
	{
		F_Service_PCI2 = 1;
	}
	CLEAR_MASK(IER3, Int_PMCIBF);
	ISR3 = Int_PMCIBF;
}

//----------------------------------------------------------------------------
// ADC interrupt routine
//----------------------------------------------------------------------------
void Irq_ADC(void)	//using 2
{

}

//----------------------------------------------------------------------------
// SMBus0 interrupt routine
//----------------------------------------------------------------------------
void Irq_SMB0(void)	//using 2
{
}

//----------------------------------------------------------------------------
// SMBus1 interrupt routine
//----------------------------------------------------------------------------
void Irq_SMB1(void)	//using 2
{
}

//----------------------------------------------------------------------------
// SMBus1 interrupt routine
//----------------------------------------------------------------------------
void Irq_SMB2(void)	//using 2
{
}

//----------------------------------------------------------------------------
// CIR interrupt routine
//----------------------------------------------------------------------------
void Irq_CIR(void)	//using 2
{
	CLEAR_MASK(IER1, Int_CIR);	// Disable INT15 CIR Interrupt
	ISR1 = Int_CIR;				// Clear INT15 pending Status
	F_Service_CIR = 1;
}

//----------------------------------------------------------------------------
// Null function
//----------------------------------------------------------------------------
void IrqNull(void)	//using 2
{

}

//----------------------------------------------------------------------------
// Irq_Anykey - Direct Keyboard Scan key press interrupt handler.
// Disable further key interrupts and post key service request.
//----------------------------------------------------------------------------
void Irq_Anykey(void)	//using 2
{
	CLEAR_MASK(IER1, Int_KB);
	ISR1 = Int_KB;
	F_Service_KEY = 1;		// Post service request to scan internal keyboard.
}

//----------------------------------------------------------------------------
// FUNCTION:   Irq_HPSI
// ISR for INT59 WUC Interrupt  for HSPI Process
//----------------------------------------------------------------------------
void Irq_HSPI(void)	//using 2
{
	#ifdef	HSPI_DefferingMode
	BackUpTR1 = TR1;			// store TR1 status
	TR1 = 0;					// Disable timer1
	CLEAR_MASK(IER7, Int_DeferredSPI);	// Disable HSPI interrup
	HSPI_RamCode();
	SET_MASK(IER7, Int_DeferredSPI);	// Enable HSPI interrupt
	TR1 = BackUpTR1;			// Re store TR1 status
	#endif
}

//----------------------------------------------------------------------------
// PMC3 input buffer full interrupt routine
//----------------------------------------------------------------------------
void Irq_Ibf3(void)	//using 2
{
	CLEAR_MASK(IER8, Int_PMC3IBF);
	ISR8 = Int_PMC3IBF;
	F_Service_PCI4 = 1;
}

//----------------------------------------------------------------------------
// FUNCTION:   Irq_SlpS4
// ISR for INT1
//----------------------------------------------------------------------------
void Irq_SlpS4(void)		// leo_20160425 add for cost down by EE Jackson wu
{
	CLR_BIT(IER0, 1);
	WUESR2 = BIT0;
	ISR0 = BIT1;
	if (!SLP_S4_High())
	{
		SUSON_EC_off();
	}
	SET_BIT(IER0, 1);		// Enable INT1
}

//----------------------------------------------------------------------------
// FUNCTION:   Irq_PCHLVDS
// ISR for INT104
//----------------------------------------------------------------------------
void Irq_PCHLVDS(void)		// leo_20160420 add for cost down by EE Jackson wu
{
	CLR_BIT(IER13, 0);
	WUESR10 = BIT3;
	ISR13 = BIT0;
	if (APU_BLON_read())
	{
		if (IsFlag1(Device_STAT_CTRL2, Lid_open))
		{
			DLID_High();
		}
		SET_BIT(WUEMR10, 3);	// set falling-edge trigger
	}
	else
	{
		DLID_Low();
		CLR_BIT(WUEMR10, 3);	// set rising-edge trigger
	}
	SET_BIT(IER13, 0);		// Enable INT121 for RSMRST#_PWR
}

//----------------------------------------------------------------------------
// FUNCTION:   Irq_SlpS3
// ISR for INT123
//----------------------------------------------------------------------------
void Irq_SlpS3(void)		// leo_20160425 add for cost down by EE Jackson wu
{
	CLR_BIT(IER15, 3);
	WUESR12 = BIT6;
	ISR15 = BIT3;
	if (!SLP_S3_High())
	{
		VRON_off();
		MAINON_off();
	}
	SET_BIT(IER15, 3);		// Enable INT123
}

//----------------------------------------------------------------------------
// FUNCTION:   Isr_Int0
// ISR for INT0 (Key Scan Interrupt).
//----------------------------------------------------------------------------
void Isr_Int0(void) interrupt 0 using 2
{

}

//----------------------------------------------------------------------------
// Isr_Tmr0
// ISR for TF0 (Timer 0 Interrupt).  Timer 0 is the 1mS Timer (Timer A).
//----------------------------------------------------------------------------
/* ----------------------------------------------------------------------------
 * FUNCTION: Irq_Timer_A - Handle Timer A interrupts.
 *
 * Timer A has a time base of 1 mS.
 *
 * Input: On entry, "TIMER Timer_A" has the event(s) to handle. Valid Events:
 *     TMR_SCAN   -
 *     TMR_GENTMO - This timer is a timer that is used for timeout detection
 *                  of serial data transmissions to/from the auxiliary
 *                  keyboard/pointing devices.
 * ------------------------------------------------------------------------- */
void Isr_Tmr0(void) interrupt 1 using 2
{
	Load_Timer_A();
	F_Service_MS_1 = 1;	// Request 1 mS timer service.
}


/* ----------------------------------------------------------------------------
 * FUNCTION:   Isr_Int1
 * ISR for INT1 (Interrupt Source A and B Interrupts).
 *
 * Interrupt Source A:
 * ------------------------------------------------------------------------- */
const FUNCT_PTR_V_V code IRQ_Service[] =
{
	IrqNull,			//	INT0   Reserved
	Irq_SlpS4,			//	INT1   WUC interrupt WUI0	// leo_20160425 add for cost down by EE Jackson wu
	IrqNull,			//	INT2   KBC output buffer empty interrupt
	IrqNull,			//	INT3   PMC output buffer empty interrupt
	IrqNull,			//	INT4   SMBus D Interrupt
	IrqNull,			//	INT5   WUC interrupt (WU10 ~ WU15)(WU40 ~ WU47)
	IrqNull,			//	INT6   WUC interrupt WUI3
	IrqNull,			//	INT7   PWM interrupt

	Irq_ADC,			//	INT8   ADC Interrupt
	Irq_SMB0,			//	INT9   SMB0 Interrupt
	Irq_SMB1,			//	INT10  SMB1 Interrupt
	Irq_Anykey,			//	INT11  Key matrix scan Int
	IrqNull,			//	INT12  WUC interrupt SWUC wake up
	IrqNull,			//	INT13  WUC interrupt KSI wake-up (WU30 ~ WU37)
	IrqNull,			//	INT14  WUC interrupt Power switch
	Irq_CIR,			//	INT15  CIR interrupt

	Irq_SMB2,			//	INT16  SMB2 Interrupt
	IrqNull,			//	INT17  WUC Interrupt WUI4
	Irq_Aux_Port3,		//	INT18  PS2 P2 Interrupt
	Irq_Aux_Port2,		//	INT19  PS2 P1 Interrupt
	Irq_Aux_Port1,		//	INT20  PS2 P0 Interrupt
	IrqNull,			//	INT21  WUC Interrupt WUI2
	IrqNull,			//	INT22  SMFI Semaphore Interrupt
	IrqNull,			//	INT23  --

	Irq_Ibf1,			//	INT24  KBC input buffer empty interrupt
	Irq_Ibf2,			//	INT25  PMC input buffer empty interrupt
	IrqNull,			//	INT26  PMC2 Output Buffer Empty Intr.
	IrqNull,			//	INT27  PMC2 Input Buffer Full Intr.
	IrqNull,			//	INT28  GINT from function 1 of GPD5
	IrqNull,			//	INT29  EGPC Interrupt
	IrqNull,			//	INT30  External timer interrupt
	IrqNull,			//	INT31  WUC interrupt WUI1

	IrqNull,			//	INT32  GPINT0
	IrqNull,			//	INT33  GPINT1
	IrqNull,			//	INT34  GPINT2
	IrqNull,			//	INT35  GPINT3
	IrqNull,			//	INT36  CIR GPINT
	IrqNull,			//	INT37  SSPI Interrupt
	IrqNull,			//	INT38  UART1 Interrupt
	IrqNull,			//	INT39  UART2 Interrupt

	IrqNull,			//	INT40  --
	IrqNull,			//	INT41  --
	IrqNull,			//	INT42  --
	IrqNull,			//	INT43  --
	IrqNull,			//	INT44  --
	IrqNull,			//	INT45  --
	IrqNull,			//	INT46  --
	IrqNull,			//	INT47  --

	IrqNull,			//	INT48  WKO[60]
	IrqNull,			//	INT49  WKO[61]
	IrqNull,			//	INT50  WKO[62]
	IrqNull,			//	INT51  WKO[63]
	IrqNull,			//	INT52  WKO[64]
	IrqNull,			//	INT53  WKO[65]
	IrqNull,			//	INT54  WKO[66]
	IrqNull,			//	INT55  WKO[67]

	IrqNull,			//	INT56  --
	IrqNull,			//	INT57  --
	IrqNull,			//	INT58  External Timer 2 Interrupt
	Irq_HSPI,			//	INT59  Deferred SPI Instruction Interrupt
	IrqNull,			//	INT60  TMRINTA0
	IrqNull,			//	INT61  TMRINTA1
	IrqNull,			//	INT62  TMRINTB0
	IrqNull,			//	INT63  TMRINTB1

	IrqNull,			//	INT64  PMC2EX Output Buffer Empty Intr.
	IrqNull,			//	INT65  PMC2EX Input Buffer Full Intr.
	IrqNull,			//	INT66  PMC3 Output Buffer Empty Intr.
	Irq_Ibf3,			//	INT67  PMC3 Input Buffer Full Intr.
	IrqNull,			//	INT68  PMC4 Output Buffer Empty Intr.
	IrqNull,			//	INT69  PMC4 Input Buffer Full Intr.
	IrqNull,			//	INT70  --
	IrqNull,			//	INT71  I2BRAM Interrupt

	IrqNull,			//	INT72  WKO[70]
	IrqNull,			//	INT73  WKO[71]
	IrqNull,			//	INT74  WKO[72]
	IrqNull,			//	INT75  WKO[73]
	IrqNull,			//	INT76  WKO[74]
	IrqNull,			//	INT77  WKO[75]
	IrqNull,			//	INT78  WKO[76]
	IrqNull,			//	INT79  WKO[77]
	IrqNull,			//	INT80 
	IrqNull,			//	INT81 
	IrqNull,			//	INT82 
	IrqNull,			//	INT83 
	IrqNull,			//	INT84 
	IrqNull,			//	INT85 
	IrqNull,			//	INT86 
	IrqNull,			//	INT87 
	IrqNull,			//	INT88 
	IrqNull,			//	INT89 
	IrqNull,			//	INT90 
	IrqNull,			//	INT91 
	IrqNull,			//	INT92 
	IrqNull,			//	INT93 
	IrqNull,			//	INT94 
	IrqNull,			//	INT95 
	IrqNull,			//	INT96 
	IrqNull,			//	INT97 
	IrqNull,			//	INT98 
	IrqNull,			//	INT99 
	IrqNull,			//	INT100
	IrqNull,			//	INT101
	IrqNull,			//	INT102
	IrqNull,			//	INT103
	Irq_PCHLVDS,		//	INT104	// leo_20160420 add for cost down by EE Jackson wu
	IrqNull,			//	INT105
	IrqNull,			//	INT106
	IrqNull,			//	INT107
	IrqNull,			//	INT108
	IrqNull,			//	INT109
	IrqNull,			//	INT110
	IrqNull,			//	INT111
	IrqNull,			//	INT112
	IrqNull,			//	INT113
	IrqNull,			//	INT114
	IrqNull,			//	INT115
	IrqNull,			//	INT116
	IrqNull,			//	INT117
	IrqNull,			//	INT118
	IrqNull,			//	INT119
	IrqNull,			//	INT120
	IrqNull,			//	INT121
	IrqNull,			//	INT122
	Irq_SlpS3,			//	INT123	// leo_20160425 add for cost down by EE Jackson wu
	
};

void Isr_Int1(void) interrupt 2 using 2
{
	(IRQ_Service[(IVECT - 0x10) & 0x7F])();	// Dispatch to service handler.
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Isr_Tmr1
 * ISR for TF1 (Timer 1 Interrupt).  Timer 1 is the 2ms Timer (Timer B).
 * ------------------------------------------------------------------------- */
void Isr_Tmr1(void) interrupt 3 using 2
{
	Stop_Timer_B();
	if (Timer_B.fbit.SEND_ENABLE)
	{
		F_Service_SEND = 1;				// Post service request.
		Timer_B.fbit.SEND_ENABLE = 0;
		Load_Timer_B();
	}
}

/* ----------------------------------------------------------------------------
 * FUNCTION:   Isr_UART
 * ISR for TI or RI (UART Interrupt).
 * ------------------------------------------------------------------------- */
void Isr_UART(void) interrupt 4 using 2
{

}


/* ----------------------------------------------------------------------------
 * FUNCTION:   Isr_Tmr2
 * ISR for TF2 or EXF2 (Timer 2 Interrupt).
 * ------------------------------------------------------------------------- */
void Isr_Tmr2(void) interrupt 5 using 2
{

}

//------------------------------------------------------------------------------
// The function of disable all interrupts
//------------------------------------------------------------------------------
void DisableAllInterrupt(void)
{
	EA = 0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

//------------------------------------------------------------------------------
// The function of enable all interrupts
//------------------------------------------------------------------------------
void EnableAllInterrupt(void)
{
	EA = 1;
}

//------------------------------------------------------------------------------
// The function of enabling KBC Input Buffer Full (60 64 port)Interrupt
//------------------------------------------------------------------------------
void EnableKBCIBFInt(void)
{
	SET_MASK(IER3, Int_KBCIBF);
}

//------------------------------------------------------------------------------
// The function of enabling PMC Input Buffer Full (62 66 68 6C port)Interrupt
//------------------------------------------------------------------------------
void EnablePMCIBFInt(void)
{
	SET_MASK(IER3, Int_PMCIBF);
}

//------------------------------------------------------------------------------
// The function of enabling PMC3 Input Buffer Full Interrupt
//------------------------------------------------------------------------------
void EnablePMC3IBFInt(void)
{
	SET_MASK(IER8, Int_PMC3IBF);
}

//#pragma AREGS

