/*------------------------------------------------------------------------------
 * Title : OEM_CIR.C
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#if	CIRFuncSupport
	#if	CIR_NEC_IMPROVED	// Ken_20120605 CHECK offset and key codes by your project
#define	CIRCompareOffset 4
const BYTE code RC6PowerKey[] = {0x95, 0x55, 0xA5, 0xF5};	// RC6 power key
	#else	// (original)
#define	CIRCompareOffset 7
const BYTE code RC6PowerKey[] = {0x95, 0x55, 0xA5, 0xF5};	// RC6 power key
	#endif

//------------------------------------------------------------------------------
// CIR wake up system
//------------------------------------------------------------------------------
void Hook_CIRWakeUp(void)
{
	BYTE cirdata;

	if (IsFlag1(POWER_FLAG, power_on) || IsFlag1(SYS_MISC2, InRestart))	// leox_20120521
	{	// S0 or during restart
		if (IsFlag1(POWER_FLAG, power_on))	// leox_20120522
		{	// S0
			// Do something or handled by driver in OS
		}
		else
		{	// During restart
			// Do nothing
		}
		#if	CIR_NEC_IMPROVED	// Ken_20120605
		ClearCIR();
		Init_CIR();
		CIR_NEC_Rst();
		#endif
	}
	else
	{	// S3 or S4/S5		
		CLR_MASK(C0SCK, SCKS);	// CIR unselect slow clock
		for (ITempB01 = 0x00; ITempB01 < CIRCompareOffset; ITempB01++)
		{
			cirdata = C0DR;
			_nop_();
		}

		for (ITempB01 = 0x00; ITempB01 < sizeof(RC6PowerKey); ITempB01++)
		{
			cirdata = C0DR;
			if (cirdata != RC6PowerKey[ITempB01])
			{
				ClearCIR();
				#if	CIR_NEC_IMPROVED	// Ken_20120605
				CIR_NEC_Rst();
				#endif
				return;
			}
		}

		#if	CIR_NEC_IMPROVED	// Ken_20120605
		CIR_NEC_Rst();
		#endif

		if (IsFlag1(POWER_FLAG, enter_SUS))
		{	// S3
			CIRS3WakeUp();
		}
		else
		{	// S4/S5
			CIRS4S5WakeUp();
		}
	}
}


//------------------------------------------------------------------------------
// CIR wake up system from S4/S5
//------------------------------------------------------------------------------
void CIRS4S5WakeUp(void)
{
	SET_MASK(C0MSTCR, CIR_RESET);	// CIR reset
	#if	1	// leox_20110916	// leox_20120521
	SetFlag(POWER_FLAG, wait_PSW_off);
	PSWOnPower();
	#else
	//Oem_TriggerS5S0();
	#endif
}


//------------------------------------------------------------------------------
// CIR wake up system from S3
//------------------------------------------------------------------------------
void CIRS3WakeUp(void)
{
	SET_MASK(C0MSTCR, CIR_RESET);	// CIR reset
	#if	1	// leox_20110916	// leox_20120521
	SetFlag(POWER_FLAG, wait_PSW_off);
	PSWOnPower();
	#else
	//Oem_TriggerS3S0();
	#endif
}


	#if	CIR_NEC_IMPROVED	// Ken_20120605
//------------------------------------------------------------------------------
// CIR reset improved NEC protocol
//------------------------------------------------------------------------------
void CIR_NEC_Rst(void)
{
	GPCRC0 = INPUT;	// CHECK
	CIR_NEC_Act = 0x55;
}


//------------------------------------------------------------------------------
// CIR check improved NEC protocol
//------------------------------------------------------------------------------
void CIR_NEC_Chk(void)
{
	if (CIR_read() && (CIR_NEC_Act == 0x55))
	{
		CIR_NEC_Act = 0xAA;
//		GPCRC0 = ALT + PULL_UP;	// CIR RX (8512 only) Set GPIO to ALT function	// CHECK
		ClearCIR();
		Init_CIR();
	}
}
	#endif	// CIR_NEC_IMPROVED
#endif	//CIRFuncSupport
