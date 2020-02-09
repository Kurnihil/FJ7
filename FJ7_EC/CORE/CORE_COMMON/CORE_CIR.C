/*------------------------------------------------------------------------------
 * Title: CORE_CIR.C
 *
 * Copyright (c) ITE INC. All Rights Reserved.
 *----------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// The function of CIR Initialization
//----------------------------------------------------------------------------
void Init_CIR(void)

{
	GPCRC0 = ALT;				// CIR RX (8512 only) Set GPIO to ALT function	// leox_20111108 [PATCH] 0x00 -> ALT
	CLEAR_MASK(C0RCR, CIR_RXEN);	// RX Disable
	C0BDHR = 0x00;				// BaudRate
	C0BDLR = CIR_BAUD_RATE;		// Baud rate (Low byte)							// leox_20120521 [PATCH]
	C0MSTCR = 0x08;				// FIFO Threshold = 17 Bytes
	C0CFR = 0x0B;				// Carrier Freqency = 38KHz

	bConfirmCirRIM();			// Write receiver enter an inactive mode
	bClearCirFIFO();			// Clear FIFO

	C0IER = 0x82;				// CIR Interrupt Enable
	// CIR receive available interrupt
	SET_MASK(IELMR1, Int_CIR);	// CIR Interrupt must use Edge-Trig
	SET_MASK(IER1, Int_CIR);	// Enable INT15 CIR Interrupt
	ISR1 = Int_CIR;				// Clear INT15 pending Status
	SET_MASK(C0RCR, CIR_RXEN);	// RX enable
}

//----------------------------------------------------------------------------
// The function of CIR module reset
//----------------------------------------------------------------------------
void Reset_CIR(void)

{
	SET_MASK(RSTC2, RCIR);		// Reset CIR module
	CLEAR_MASK(IER1, Int_CIR);	// Disable INT15 CIR Interrupt
	ISR1 = Int_CIR;				// Clear INT15 pending Status
}

//----------------------------------------------------------------------------
// The function of clearing CIR FIFO
//----------------------------------------------------------------------------
void bClearCirFIFO(void)
{
	SET_MASK(C0MSTCR, CIR_FIFOCLR);				// Write 1 for Clearing FIFO data
	_nop_();
	while (IS_MASK_SET(C0MSTCR, CIR_FIFOCLR))	// Wait FIFO Clear Done
	{
		;
	}
}

//----------------------------------------------------------------------------
// The function of confirming CIR receiver inactive mode
//----------------------------------------------------------------------------
void bConfirmCirRIM(void)
{
	SET_MASK(C0RCR, CIR_RXACT);				// Write 1 for clearing Receive Active
	_nop_();
	while (IS_MASK_SET(C0RCR, CIR_RXACT))	// Wait CIR receiver enter an inactive mode
	{
		;
	}
}

//----------------------------------------------------------------------------
// The function of clearing CIR
//----------------------------------------------------------------------------
void ClearCIR(void)
{
	CLEAR_MASK(C0RCR, CIR_RXEN);	// RX Disable
	bConfirmCirRIM();			// Write receiver enter an inactive mode
	bClearCirFIFO();			// Clear FIFO
	SET_MASK(C0RCR, CIR_RXEN);	// RX enable
}

//----------------------------------------------------------------------------
// The function of CIR Interrupt service routine
//----------------------------------------------------------------------------
void service_cir(void)
{
	#if	CIRFuncSupport
	Hook_CIRWakeUp();
	#endif
}

