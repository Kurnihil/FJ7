/*------------------------------------------------------------------------------
 * Title : OEM_VICMV.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_VICMV
void CheckVICMVThrottlingFUN(void)
{
	if ((IsFlag0(POWER_FLAG, ACPI_OS)) || (IsFlag0(POWER_STAT_CTRL, adapter_in)) || (IsFlag1(Hybrid_Status, Hybrid_State)))
	{
		InitVICMVHi();
	}
	else
	{
		LoadVICM_Limit();
		Get_VICMVoltage();
		CheckIfEnableVICMVThrottly();
		CheckIfDisableVICMVThrottly();
	}
	#if BAT_DCHG_PROTECT	// leox20150826 Battery DCHG protection
	if (IsFlag1(POWER_FLAG, system_on) && IsFlag0(POWER_STAT_CTRL, adapter_in))
	{	// DC mode in S0
		LoadDCHG_Limit();	// leox20150911
		DCHG_V = ADV_DAT2;	// Get_DCHGVoltage()
		CheckIfEnDCHGVThrottly();
		CheckIfDsDCHGVThrottly();
	}
	else
	{	// InitDCHGVHi()
		DCHG_V = 0;
		DCHGVHiEnThroLimitCnt = 0;
		DCHGVHiDsThroLimitCnt = 0;
	}
	#endif
}


void InitVICMVHi(void)
{
	if (IsFlag1(VICMVHiFlag, Q42Q43_SCI))
	{
		ClrFlag(VICMVHiFlag, Q42Q43_SCI);
		ECQEvent(Q43_SCI, SCIMode_Normal);
	}
	VICM_V = 0;
	VICMVHiEnThroLimitCnt = 0;
	VICMVHiDisThroLimitCnt = 0;
}


void LoadVICM_Limit(void)
{
	CmpEnVICM_Limit = VICMV65WHiEnThroLimit;
	CmpDisVICM_Limit = VICMV65WLoDisThroLimit;
	if (IsFlag1(Device_STAT_CTRL1, VGA_Status))
	{
		CmpEnVICM_Limit = VICMV90WHiEnThroLimit;
		CmpDisVICM_Limit = VICMV90WLoDisThroLimit;
	}
	else
	{
		CmpEnVICM_Limit = VICMV65WHiEnThroLimit;
		CmpDisVICM_Limit = VICMV65WLoDisThroLimit;
	}
	#if ADAPTER_PROTECT_DBG	// leox20150716 Adapter protection debug	// leox20150810
	if (CmpEnVICM_LimitD != 0) {CmpEnVICM_Limit  = CmpEnVICM_LimitD;}
	if (CmpDsVICM_LimitD != 0) {CmpDisVICM_Limit = CmpDsVICM_LimitD;}
	#endif
}


void Get_VICMVoltage(void)
{
	VICM_V = ADV_DAT1;
}


void CheckIfEnableVICMVThrottly(void)
{
	if (VICM_V < CmpEnVICM_Limit)
	{
		VICMVHiEnThroLimitCnt = 0;
		return;
	}
	VICMVHiDisThroLimitCnt = 0;

	if (IsFlag0(VICMVHiFlag, Q42Q43_SCI))
	{
		#if ADAPTER_PROTECT_DBG	// leox20150717 Adapter protection debug		// leox20150810
		if (VICMVHiEnThroLimitCnt >= VICMVEnThrottleDebD)
		#else	// (original)
		if (VICMVHiEnThroLimitCnt >= VICMVHiEnThroLimitDebounce)
		#endif
		{
			SetFlag(VICMVHiFlag, Q42Q43_SCI);
			#ifdef OEM_VICMV_PROCHOT	// leox20150731
			h_prochot_enable();
			#else	// (original)
			ECQEvent(Q42_SCI, SCIMode_Normal);
			#endif
		}
		else
		{
			VICMVHiEnThroLimitCnt++;
		}
	}
}


void CheckIfDisableVICMVThrottly(void)
{
	if (VICM_V >= CmpDisVICM_Limit)
	{
		VICMVHiDisThroLimitCnt = 0;
		return;
	}
	VICMVHiEnThroLimitCnt = 0;

	if (IsFlag1(VICMVHiFlag, Q42Q43_SCI))
	{
		#if ADAPTER_PROTECT_DBG	// leox20150717 Adapter protection debug		// leox20150810
		if (VICMVHiDisThroLimitCnt >= VICMVDsThrottleDebD)
		#else	// (original)
		if (VICMVHiDisThroLimitCnt >= VICMVHiDisThroLimitDebounce)
		#endif
		{
			ClrFlag(VICMVHiFlag, Q42Q43_SCI);
			#ifdef OEM_VICMV_PROCHOT	// leox20150731
			Check_PROCHOT_Disable();	// leox20150803 Check before disable PROCHOT
			#else	// (original)
			ECQEvent(Q43_SCI, SCIMode_Normal);
			#endif
		}
		else
		{
			VICMVHiDisThroLimitCnt++;
		}
	}
}


#if BAT_DCHG_PROTECT	// leox20150826 Battery DCHG protection
void CheckIfEnDCHGVThrottly(void)
{
	if (DCHG_V < CmpEnDCHG_Limit)
	{
		DCHGVHiEnThroLimitCnt = 0;
		return;
	}
	DCHGVHiDsThroLimitCnt = 0;

	if (IsFlag0(BAT_MISC3, DCHG_Throttling))
	{
		if (DCHGVHiEnThroLimitCnt >= DCHGVHiEnThroLimitCntD)
		{
			SetFlag(BAT_MISC3, DCHG_Throttling);
			h_prochot_enable();
		}
		else
		{
			DCHGVHiEnThroLimitCnt++;
		}
	}
}


void CheckIfDsDCHGVThrottly(void)
{
	if (DCHG_V >= CmpDsDCHG_Limit)
	{
		DCHGVHiDsThroLimitCnt = 0;
		return;
	}
	DCHGVHiEnThroLimitCnt = 0;

	if (IsFlag1(BAT_MISC3, DCHG_Throttling))
	{
		if (DCHGVHiDsThroLimitCnt >= DCHGVHiDsThroLimitCntD)
		{
			ClrFlag(BAT_MISC3, DCHG_Throttling);
			Check_PROCHOT_Disable();
		}
		else
		{
			DCHGVHiDsThroLimitCnt++;
		}
	}
}


// leox20150911 Load DCHG limit according to battery PN
const sADV_Point code CmpDCHGTbl[] =	// The order the same as BatCfgTbl[]
{//  Trigger Release,	// Trigger        Release       //   #  Battery_PN
	{0x0000, 0x0000},	// 0.000V (0.0A), 0.000V (0.0A) //   0  "        "
	{0x017E, 0x00AF},	// 1.120V (7.0A), 0.513V (3.2A) //   1  "********"
	{0x017E, 0x00AF},	// 1.120V (7.0A), 0.513V (3.2A) //   2  "CP671398"
	{0x017E, 0x00AF},	// 1.120V (7.0A), 0.513V (3.2A) //   3  "CP656340"
	{0x0162, 0x00AF},	// 1.038V (6.5A), 0.513V (3.2A) //   4  "CP671396"
	{0x0162, 0x00AF},	// 1.038V (6.5A), 0.513V (3.2A) //   5  "CP651529"
	{0x0147, 0x00BF},	// 0.959V (6.0A), 0.560V (3.5A) //   6  "CP656728"
	{0x012C, 0x00BF},	// 0.880V (5.5A), 0.560V (3.5A) //   7  "CP656352"
	{0x0147, 0x00BF},	// 0.959V (6.0A), 0.560V (3.5A) //   8  "CP652728"
	{0x0147, 0x00BF},	// 0.959V (6.0A), 0.560V (3.5A) //   9  "CP671395"
	{0x0162, 0x00AF},	// 1.038V (6.5A), 0.513V (3.2A) //  10  "CP700280"
	{0x0147, 0x00BF},	// 0.959V (6.0A), 0.560V (3.5A) //  11  "CP700278"		// leox20151211 Bug fixed by Leo Liu
	{0x017E, 0x00AF},	// 1.120V (7.0A), 0.513V (3.2A) //  12  "CP700282"
	{0x017E, 0x00AF},	// 1.120V (7.0A), 0.513V (3.2A) //  13  "CP708752"
};

void LoadDCHG_Limit(void)
{
	if (SEL_BAT_MODEL >= GetArrayNum(CmpDCHGTbl)) {return;}
	CmpEnDCHG_Limit = CmpDCHGTbl[SEL_BAT_MODEL].Trigger;
	CmpDsDCHG_Limit = CmpDCHGTbl[SEL_BAT_MODEL].Release;
}
#endif	// BAT_DCHG_PROTECT
#endif	// OEM_VICMV
