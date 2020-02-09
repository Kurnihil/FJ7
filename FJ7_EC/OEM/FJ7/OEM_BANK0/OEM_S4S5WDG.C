/*------------------------------------------------------------------------------
 * Title : OEM_S4S5WDG.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_S4S5WDG
void WatchDOGS4S5_Resume(void)
{
	if (IsFlag1(POWER_FLAG, power_on))
	{
		if (WatchDOGS4S5_CNT != 0)
		{
			WatchDOGS4S5_CNT--;
			if (WatchDOGS4S5_CNT == 0)
			{
				ResetSysOff_ECcmd_S4S5WDG();
				WatchDOGS4S5_CNT = 5;
			}
		}
	}
}


void ResetSysOff_ECcmd_S4S5WDG(void)
{
	if ((AC_IN_read()) || (IsFlag0(MISC_FLAG, F_CriLow)))
	{
		ResetSysOff_ECcmd_S4S5WDG1();
	}
	else
	{
		DoTurnOffPower_668C_S4S5WDG();
	}
}


void ResetSysOff_ECcmd_S4S5WDG1(void)
{
	SetFlag(SYS_MISC2, InRestart);
	#ifdef	OEM_IPOD
	DisableUSBCHARGPORTFun();
	#endif	//OEM_IPOD
	ShutDnCause = 0xF8;
	S0S3toS4S5();
	TurnOffLanPower();
	TurnOffSBPowerSource();
	DelayXms(100);
	ClrFlag(POWER_FLAG, wait_PSW_off);
	PSWOnPower();
}


void DoTurnOffPower_668C_S4S5WDG(void)
{
	ShutDnCause = 0xF9;
	ForceOffPower();
}
#endif	//OEM_S4S5WDG

