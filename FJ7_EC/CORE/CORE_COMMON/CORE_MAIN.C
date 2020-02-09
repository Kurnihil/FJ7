/*------------------------------------------------------------------------------
 * Title: CORE_MAIN.C - Main program for KBC firmware.
 *
 * Main processing loop (interrupt driven) for the keyboard controller/scanner.
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

//----------------------------------------------------------------------------
// FUNCTION: cold_reset - Initialization when power goes on
//----------------------------------------------------------------------------
void cold_reset(void)
{
	Ext_Cb2.byte = EXT_CB2_INIT;			/* Update Ext_Cb3 a little further down */
	Save_Typematic = SAVE_TYPEMATIC_INIT;
	Save_Kbd_State = (SAVE_KBD_STATE_INIT & ~maskLEDS) | (0 & maskLEDS);

	Led_Ctrl = LED_CTRL_INIT;
	Led_Data = 0;
	Gen_Info = 0;	/* Clear general control. */

	Int_Var.Ticker_10 = 10;

	Flag.PASS_READY = 0;	/* Password not loaded. */
	Int_Var.Ticker_100 = 50;

	Flag.SCAN_INH = 1;		/* Scanner transmission inhibited. */
	Flag.VALID_SCAN = 0;

	Flag.NEW_PRI_K = 0;
	Flag.NEW_PRI_M = 0;

	Flag.LED_ON = 1;
	Ccb42 = CCB42_INIT;

	Pass_On = 0;		/* Password enable send code. */
	Pass_Off = 0;		/* Password disable send code. */
	Pass_Make1 = 0;	/* Reject make code 1. */
	Pass_Make2 = 0;	/* Reject make code 2. */

	MULPX = 0;

}


//----------------------------------------------------------------------------
// FUNCTION:   general_reset1, general_reset2
// Initialize things common to both Cold/Warm reset.
//----------------------------------------------------------------------------
static void common_reset(void)	/* MCHT, 27/05/04 */ // static void general_reset1(void)
{
	Service  = 0;		/* Clear service requests */
	Service1 = 0;

	Kbd_Response = 0;	/* Clear response codes */
	Timer_A.byte = 0;	/* Clear Timer A events */
	Timer_B.byte = 0;	/* Clear Timer B events */
	Scanner_State = 0;	/* Clear scanner state */

	SCI_Event_In_Index = 0;
	SCI_Event_Out_Index = 0;

	#if	ACPI_QEventPending
	PD_SCI_Event_In_Index = 0;
	PD_SCI_Event_Out_Index = 0;
	#endif
}


//----------------------------------------------------------------------------
// FUNCTION: Init_Mouse - Initialize the Intelligent mouse variables.
//----------------------------------------------------------------------------
void Init_Mouse(void)
{

}

void Core_Initialization(void)
{
	Init_Kbd();
	cold_reset();
	common_reset();
	Init_Mouse();
	InitSMBus();
	Init_Timers();
}

/* ----------------------------------------------------------------------------
 *  FUNCTION: main - Main service loop.
 *
 *  Wait in idle state until an IRQ causes an exit from idle.  If the IRQ
 *  handler posted a service request (via bSERVICE) then dispatch control to
 *  the appropriate service handler.  Otherwise, go back to idle state.  After
 *  all service requests have been handled, return to idle state.
 * ------------------------------------------------------------------------- */
void main(void)
{
	//DisableAllInterrupt();
	EA = 0;						// leox_20111206 [PATCH] Expand DisableAllInterrupt()
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SP = 0xD0;					// Setting stack pointer
	//FPCFG = 0x3F;				// Bit7 :0 using P1[0] P1[1] as code banking
	//CheckResetSource();		// Change to startup.a51

	// Disable dynamic cache	// leox_20120517 [PATCH]
	DCache = 0x03;				// Disable before clear it

	// Clear dynamic caches		// leox_20111206 [PATCH] From (WinFlashMark == 0x33)
	CLEAR_MASK(FBCFG, SSMC);	// Disable scatch ROM
	_nop_();
	MPRECF = 0x01;
	_nop_();
	MPRECF = 0x01;
	_nop_();
	MPRECF = 0x01;
	_nop_();
	MPRECF = 0x01;
	_nop_();
	_nop_();

	if (WinFlashMark == 0x33)
	{
		WinFlashMark = 0x00;

		Init_Cache();

		//David_20120816 add for Win flash EC keyboard reset
		Hook_Setup_Scanner_Pntr();
		//David_20120816 add for Win flash EC keyboard reset

		#ifdef	HSPI
			#ifdef	HSPI_DefferingMode
		SET_MASK(IER7, Int_DeferredSPI);	// Enable HSPI interrupt
			#endif
		#endif

		Init_Timers();
		InitSMBus();
		EnableInterrupt();		// enable all interrupt

		#if	Insyde_Bios
		if (RamCodePort == 0x64)	// for insyde bios
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
		#endif
	}
	else
	{
		//+Jay20121130 add_oem_mirror_code
		MirrorDoneAndSPITriState();
		//-Jay20121130 add_oem_mirror_code
		Oem_InitializationL();
		Core_Initialization();
//		InitEnableInterrupt();	//@EDWU20150720 [PATCH] Move down
		Oem_InitializationH();

		#ifdef	UART_Debug
		Init_UART();
		printf("\n************************************");
		printf("\nEC Init OK !!!");
		printf("\n************************************");
		#endif

		InitEnableInterrupt();	//@EDWU20150720 [PATCH] Enable interrupt when all initial routines done
	}

	// Set dynamic cache	// leox_20120517 [PATCH] 0x00: Enable, 0x03: Disable
	#ifdef	DisableDcache
	DCache = 0x03;
	#else
	DCache = 0x00;
	#endif

	while (1)
	{
		main_service();

		EnableInterrupt();
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		if (PS2CheckPendingISR() == 0x00)
		{
			ScanAUXDeviceStep();
		}

		if ((Service == 0x00) && (Service1 == 0x00))
		{
			if (TH0 < (Timer_1ms >> 8)) {TH0 = Timer_1ms >> 8;}	// 20120613 ITE add for fix TH0 sometimes timer lost (< Timer_1ms) [PATCH]
			PCON = 1;			// enter idle mode
		}
	}
}

//----------------------------------------------------------------------------
// The function of enabling interrupts
//----------------------------------------------------------------------------
void InitEnableInterrupt(void)
{
	ISR0 = 0xFF;
	ISR1 = 0xFF;
	ISR2 = 0xFF;
	ISR3 = 0xFF;
	ISR4 = 0xFF;
	ISR5 = 0xFF;
	ISR6 = 0xFF;
	ISR7 = 0xFF;
	ISR8 = 0xFF;
	ISR9 = 0xFF;

	EX1 = 1;				// enable external 1 interrupt
	EnableAllInterrupt();
}

void EnableInterrupt(void)
{
	EnableAllInterrupt();
	EnableKBCIBFInt();	// Enable 60/64 port input buffer full interrup
	EnablePMCIBFInt();	// Enable PMC Input Buffer Full (62 66 68 6C port)Interrupt
	#ifdef	PMC3_Support
	EnablePMC3IBFInt();	// Enable PMC3 input buffer full interrup
	#endif
	Hook_EnableInterrupt();
}

void main_service(void)
{
	while ((Service != 0x00) || (Service1 != 0x00))
	{
		//-----------------------------------
		// Host command/data service
		//-----------------------------------
		if (F_Service_PCI)
		{
			F_Service_PCI = 0;
			service_pci1();
			continue;
		}

		//-----------------------------------
		// Service unlock
		//-----------------------------------
		if (F_Service_UNLOCK)
		{
			F_Service_UNLOCK = 0;
			service_unlock();
			continue;
		}

		//-----------------------------------
		// Send byte from KBC
		//-----------------------------------
		if (F_Service_SEND)
		{
			F_Service_SEND = 0;
			service_send();
			continue;
		}

		//-----------------------------------
		// Send PS2 interface data
		//-----------------------------------
		if (F_Service_Send_PS2)
		{
			F_Service_Send_PS2 = 0;
			service_PS2_data();
			continue;
		}

		//-----------------------------------
		// process PS2 interface data
		//-----------------------------------
		if (F_Service_PS2)
		{
			F_Service_PS2 = 0;
			service_ps2();
			continue;
		}

		//-----------------------------------
		// 1 millisecond elapsed
		//-----------------------------------
		if (F_Service_MS_1)
		{
			F_Service_MS_1 = 0;
			service_1mS();
			continue;
		}

		//-----------------------------------
		// Secondary Host command/data service
		//-----------------------------------
		if (F_Service_PCI2)
		{
			F_Service_PCI2 = 0;
			service_pci2();
			continue;
		}

		//-----------------------------------
		// Keyboard scanner service
		//-----------------------------------
		if (F_Service_KEY)
		{
			F_Service_KEY = 0;
			service_scan();
			continue;
		}

		//-----------------------------------
		// Third Host command/data service
		//-----------------------------------
		if (F_Service_PCI3)
		{
			F_Service_PCI3 = 0;
			service_pci3();
			continue;
		}
		//David_20141222 add for MMB button
		if(F_Service_OEM_1)
		{
			F_Service_OEM_1 = 0;
			#ifdef IT_7230MMB_SUPPORT
			CapS_ISR();
			#endif
			continue;
		}
		//David_20141222 add for MMB button

		//-----------------------------------
		// CIR IRQ
		//-----------------------------------
		if (F_Service_CIR)
		{
			F_Service_CIR = 0;
			service_cir();
			continue;
		}

		//-----------------------------------
		// fourth command/data service
		//-----------------------------------
		if (F_Service_PCI4)
		{
			F_Service_PCI4 = 0;
			service_pci4();
			continue;
		}

		//-----------------------------------
		// Low level event
		//-----------------------------------
		if (F_Service_Low_LV)
		{
			F_Service_Low_LV = 0;
			service_Low_LVEvent();
			continue;
		}
	}
}

static void service_OEM_1(void)
{
	;
}

static void service_OEM_2(void)
{
	;
}

static void service_OEM_3(void)
{
	;
}

static void service_OEM_4(void)
{
	;
}

static void service_Low_LVEvent(void)
{
	if ((KBPendingTXCount != KBPendingRXCount) || (scan.kbf_head != scan.kbf_tail))
	{
		SetServiceSendFlag();
	}

	if (IS_MASK_SET(KBHISR, P_IBF))
	{
		F_Service_PCI = 1;
	}

	if (IS_MASK_SET(PM1STS, P_IBF))
	{
		F_Service_PCI2 = 1;
	}
}

//----------------------------------------------------------------------------
// FUNCTION: service_unlock
// Unlock aux devices and re-enable Host interface IRQ if it is ok to do so.
//----------------------------------------------------------------------------
static void service_unlock(void)
{
	Unlock_Scan();
}

//------------------------------------------------------------
// Polling events
//------------------------------------------------------------
void service_1mS(void)
{
	Timer1msEvent();
	Timer1msCnt++;
	switch (Timer1msCnt)
	{
	case 1:
		Timer5msEventA();
		break;

	case 2:
		Timer5msEventB();
		break;

	case 3:
		Timer5msEventC();
		break;

	case 4:
		Timer5msEventD();
		break;

//	case 5:
	default:
		Timer5msEventE();
		//ken test timer 20130314 ++
		if (!(Timer5msCnt & 0x01)) {SetStartScanAUXFlag();}	// For compatible kernel code of Timer10msEvent()
		//ken test timer 20130314 ++
		//ken test timer 20130315 --++
		//if (Timer5msCnt >= 200) {Timer5msCnt = 0;}
		if (Timer5msCnt >= 200)
		{
			Timer5msCnt = 0;
			if (Timer1SecCnt >= 60) {Timer1SecCnt = 0;}
			Timer1SecCnt++;
		}
		//ken test timer 20130315 --++
		Timer5msCnt++;
		Timer1msCnt = 0;
		break;
	}
}

//------------------------------------------------------------
// 1ms events
//------------------------------------------------------------
void Timer1msEvent(void)
{
	#if CIR_NEC_IMPROVED	// Ken_20120605 [PATCH]
	CIR_NEC_Chk();
	#endif
	ReSendPS2PendingData();
	Hook_Timer1msEvent();	// leox_20110920 [PATCH] Remove argument "Timer1msCnt"
}

//------------------------------------------------------------
// 5ms events
//------------------------------------------------------------
void Timer5msEventA(void)
{
	F_Service_Low_LV = 1;
	Hook_Timer5msEventA();
}

void Timer5msEventB(void)
{
	if (Timer_A.fbit.TMR_SCAN) {F_Service_KEY = 1;}		// Request scanner service
	Hook_Timer5msEventB();
}

void Timer5msEventC(void)
{
	Hook_Timer5msEventC();
}

void Timer5msEventD(void)
{
	Hook_Timer5msEventD();
}

void Timer5msEventE(void)
{
	Hook_Timer5msEventE();
}

void MirrorDoneAndSPITriState(void)
{
	CLR_MASK(FLHCTRL3R, SIFE);			// Clear flag SIFE after mirror code	//@EDWU20150720 [PATCH]
	if (FLHCTRL3R & 0x80)
	{
		SET_MASK(FLHCTRL3R, BIT(7));	// clear mirror code finish flag
	}
	SET_MASK(HINSTC1, SCECB);			// reload signature
	SET_MASK(FLHCTRL3R, SPITRI);		// Force tri state.
}
