/*------------------------------------------------------------------------------
 * Title : OEM_DCMI.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_DCMI
void CheckDCMIThrottlingFUN(void)
{
	if ((IsFlag0(POWER_FLAG, ACPI_OS)) || (IsFlag1(POWER_STAT_CTRL, adapter_in)) || (IsFlag0(ABAT_STATUS, abat_in)))
	{
		InitDCMIHi();
	}
	else
	{
		Get_DCMI();
		CheckIfEnableDCMIThrottly();
		CheckIfDisableDCMIThrottly();
	}
}


void InitDCMIHi(void)
{
	if (IsFlag1(DCMIHiFlag, Q44Q45_SCI))
	{
		ClrFlag(DCMIHiFlag, Q44Q45_SCI);
		ECQEvent(Q45_SCI, SCIMode_Normal);
	}
	DCMI_V = 0;
	DCMIHiEnThroLimitCnt = 0;
	DCMIHiDisThroLimitCnt = 0;
}


void Get_DCMI(void)
{
	DCMI_V = (0xFFFF - BT1I);
}


void CheckIfEnableDCMIThrottly(void)
{
	if (DCMI_V < DCMIHiEnThroLimit)
	{
		DCMIHiEnThroLimitCnt = 0;
		return;
	}

	DCMIHiDisThroLimitCnt = 0;

	if (IsFlag0(DCMIHiFlag, Q44Q45_SCI))
	{
		if (DCMIHiEnThroLimitCnt >= DCMIHiEnThroLimitDebounce)
		{
			SetFlag(DCMIHiFlag, Q44Q45_SCI);
			ECQEvent(Q44_SCI, SCIMode_Normal);
		}
		else
		{
			DCMIHiEnThroLimitCnt++;
		}
	}
}


void CheckIfDisableDCMIThrottly(void)
{
	if (DCMI_V >= DCMILoDisThroLimit)
	{
		DCMIHiDisThroLimitCnt = 0;
		return;
	}

	DCMIHiEnThroLimitCnt = 0;

	if (IsFlag1(DCMIHiFlag, Q44Q45_SCI))
	{
		if (DCMIHiDisThroLimitCnt >= DCMIHiDisThroLimitDebounce)
		{
			ClrFlag(DCMIHiFlag, Q44Q45_SCI);
			ECQEvent(Q45_SCI, SCIMode_Normal);
		}
		else
		{
			DCMIHiDisThroLimitCnt++;
		}
	}
}
#endif	//OEM_DCMI

