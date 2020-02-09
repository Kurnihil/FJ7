/*------------------------------------------------------------------------------
 * Title : OEM_VICMVACDC.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_VICMVACDC
	#if BAT_MBATV_PROTECT	// leo_20150916 add power protect +++	// leox20150916
void CheckVICMVThrottlingFUNACDC(void)
{
	if (IsFlag0(POWER_FLAG, ACPI_OS) || IsFlag1(Hybrid_Status, Hybrid_State))
	{
		InitVICMVHiACDC();
	}
	else
	{
		if (IsFlag1(POWER_STAT_CTRL, adapter_in))
		{
			ClrDCtoACData();
			LoadVICM_VACDC_Limit();
			Get_VICMVoltageACDC();
			ChkIfEnableVICMVThrottly();
			ChkIfDisableVICMVThrottly();
			AC_OCP_En_Check();
			AC_OCP_Dis_Check();
		}
		else
		{
			ClrACtoDCData();
			Get_DC_Watt_Value();
			DC_OW_En_Check();
			DC_OW_Dis_Check();
			DC_OCP_En_Check();
			DC_OCP_Dis_Check();
		}
	}
}


void ClrDCtoACData(void)
{
	if (IsFlag1(VICMVHiFlagACDC, DC_OW_Prochot) || IsFlag1(VICMVHiFlagACDC, DC_OCP_Prochot))
	{
		ClrFlag(VICMVHiFlagACDC, DC_OW_Prochot);
		ClrFlag(VICMVHiFlagACDC, DC_OCP_Prochot);
		Check_PROCHOT_Disable();	// leox20150916
	}
	DC_OW_En_ProchotCnt = 0;
	DC_OW_Dis_ProchotCnt = 0;
	DC_OCP_En_ProchotCnt = 0;
	DC_OCP_Dis_ProchotCnt = 0;
}


void ClrACtoDCData(void)
{
	if (IsFlag1(VICMVHiFlagACDC, Q42Q43_SCIACDC))
	{
		ClrFlag(VICMVHiFlagACDC, Q42Q43_SCIACDC);
//		ECQEvent(Q43_SCIACDC, SCIMode_Normal);	// leox20150916 Disabled		// CHECK
	}

	if (IsFlag1(VICMVHiFlagACDC, AC_OC_Prochot) || IsFlag1(VICMVHiFlagACDC, AC_OCP_Prochot))
	{
		ClrFlag(VICMVHiFlagACDC, AC_OC_Prochot);
		ClrFlag(VICMVHiFlagACDC, AC_OCP_Prochot);
		Check_PROCHOT_Disable();	// leox20150916
	}
	VICMVHiEnThroLimitCntACDC = 0;
	VICMVHiDisThroLimitCntACDC = 0;
	AC_OC_En_ProchotCnt = 0;
	AC_OC_Dis_ProchotCnt = 0;
	AC_OCP_En_ProchotCnt = 0;
	AC_OCP_Dis_ProchotCnt = 0;
}


void InitVICMVHiACDC(void)
{
	if (IsFlag1(VICMVHiFlagACDC, Q42Q43_SCIACDC))
	{
		ClrFlag(VICMVHiFlagACDC, Q42Q43_SCIACDC);
//		ECQEvent(Q43_SCIACDC, SCIMode_Normal);	// leox20150916 Disabled		// CHECK
	}

	if (IsFlag1(VICMVHiFlagACDC, AC_OC_Prochot) || IsFlag1(VICMVHiFlagACDC, AC_OCP_Prochot))
	{
		ClrFlag(VICMVHiFlagACDC, AC_OC_Prochot);
		ClrFlag(VICMVHiFlagACDC, AC_OCP_Prochot);
		Check_PROCHOT_Disable();	// leox20150916
	}
	VICM_VACDC = 0;
	VICMVHiEnThroLimitCntACDC = 0;
	VICMVHiDisThroLimitCntACDC = 0;
	AC_OC_En_ProchotCnt = 0;
	AC_OC_Dis_ProchotCnt = 0;
	AC_OCP_En_ProchotCnt = 0;
	AC_OCP_Dis_ProchotCnt = 0;

	if (IsFlag1(VICMVHiFlagACDC, DC_OW_Prochot) || IsFlag1(VICMVHiFlagACDC, DC_OCP_Prochot))
	{
		ClrFlag(VICMVHiFlagACDC, DC_OW_Prochot);
		ClrFlag(VICMVHiFlagACDC, DC_OCP_Prochot);
		Check_PROCHOT_Disable();	// leox20150916
	}
	DC_OW_En_ProchotCnt = 0;
	DC_OW_Dis_ProchotCnt = 0;
	DC_OCP_En_ProchotCnt = 0;
	DC_OCP_Dis_ProchotCnt = 0;
}


void Get_DC_Watt_Value(void)
{
	VICM_VACDC = ADV_DAT1;	// leox20150917
	L_VMBATV_V = ADV_DAT5;	// Voltage of pin MBATV
	L_VICM_V   = ADV_DAT2;	// leox20150917 ADV_DAT1 -> ADV_DAT2 read pin I_DCHG for FH9C
	L_DC_W     = L_VMBATV_V * L_VICM_V;
}


void LoadVICM_VACDC_Limit(void)
{
	CmpEnVICM_VACDC_Limit = VICMV65WHiEnThroLimit_ACDC;
	CmpDisVICM_VACDC_Limit = VICMV65WLoDisThroLimit_ACDC;
	if (IsFlag1(Device_STAT_CTRL1, VGA_Status))
	{
		CmpEnVICM_VACDC_Limit = VICMV90WHiEnThroLimit_ACDC;
		CmpDisVICM_VACDC_Limit = VICMV90WLoDisThroLimit_ACDC;
	}
	else
	{
		CmpEnVICM_VACDC_Limit = VICMV65WHiEnThroLimit_ACDC;
		CmpDisVICM_VACDC_Limit = VICMV65WLoDisThroLimit_ACDC;
	}
}


void Get_VICMVoltageACDC(void)
{
	VICM_VACDC = ADV_DAT1;
}


//------------------------------------------------------------------------------
// Check AC is over current or not
//------------------------------------------------------------------------------
void ChkIfEnableVICMVThrottly(void)
{
	static BYTE ShutdownCNT = 0;

	if (VICM_VACDC < CmpEnVICM_VACDC_Limit)
	{
		VICMVHiEnThroLimitCntACDC = 0;
		AC_OC_En_ProchotCnt = 0;
		ShutdownCNT = 0;
		return;
	}
	VICMVHiDisThroLimitCntACDC = 0;
	AC_OC_Dis_ProchotCnt = 0;

	if (IsFlag1(VICMVHiFlagACDC, AC_OC_Prochot))
	{
		if (ShutdownCNT >= ACDC_OC_OW_Shutdown_Debounce)
		{
			ShutdownCNT = 0;
			ShutDnCause = 0xFD;
			ForceOffPower();
		}
		else
		{
			ShutdownCNT++;
		}
	}

	if (IsFlag1(VICMVHiFlagACDC, Q42Q43_SCIACDC))
	{
		if (AC_OC_En_ProchotCnt >= (AC_OC_En_Debounce - VICMVHiEnThroLimitDebounceACDC))
		{
			SetFlag(VICMVHiFlagACDC, AC_OC_Prochot);
			h_prochot_enable();
		}
		else
		{
			AC_OC_En_ProchotCnt++;
		}
	}

	if (IsFlag0(VICMVHiFlagACDC, Q42Q43_SCIACDC))
	{
		if (VICMVHiEnThroLimitCntACDC >= VICMVHiEnThroLimitDebounceACDC)
		{
			SetFlag(VICMVHiFlagACDC, Q42Q43_SCIACDC);
//			ECQEvent(Q42_SCIACDC, SCIMode_Normal);	// leox20150916 Disabled	// CHECK
		}
		else
		{
			VICMVHiEnThroLimitCntACDC++;
		}
	}
}


void ChkIfDisableVICMVThrottly(void)
{
	if (VICM_VACDC >= CmpDisVICM_VACDC_Limit)
	{
		VICMVHiDisThroLimitCntACDC = 0;
		AC_OC_Dis_ProchotCnt = 0;
		return;
	}
	VICMVHiEnThroLimitCntACDC = 0;
	AC_OC_En_ProchotCnt = 0;

	if (IsFlag1(VICMVHiFlagACDC, AC_OC_Prochot))
	{
		if (AC_OC_Dis_ProchotCnt >= AC_OC_Dis_Debounce)
		{
			ClrFlag(VICMVHiFlagACDC, AC_OC_Prochot);
			Check_PROCHOT_Disable();	// leox20150916
		}
		else
		{
			AC_OC_Dis_ProchotCnt++;
		}
	}

	if (IsFlag1(VICMVHiFlagACDC, Q42Q43_SCIACDC))
	{
		if (VICMVHiDisThroLimitCntACDC >= VICMVHiDisThroLimitDebounceACDC)
		{
			ClrFlag(VICMVHiFlagACDC, Q42Q43_SCIACDC);
//			ECQEvent(Q43_SCIACDC, SCIMode_Normal);	// leox20150916 Disabled	// CHECK
		}
		else
		{
			VICMVHiDisThroLimitCntACDC++;
		}
	}
}


//------------------------------------------------------------------------------
// Check DC is over Watt or not
//------------------------------------------------------------------------------
void DC_OW_En_Check(void)
{
	static BYTE ShutdownCNT = 0;

	if (L_DC_W < Bat_OverWattThreshold)
	{
		DC_OW_En_ProchotCnt = 0;
		ShutdownCNT = 0;
		return;
	}
	DC_OW_Dis_ProchotCnt = 0;

	if (IsFlag1(VICMVHiFlagACDC, DC_OW_Prochot))
	{
		if (ShutdownCNT >= ACDC_OC_OW_Shutdown_Debounce)
		{
			ShutdownCNT = 0;
			ShutDnCause = 0xFD;
			ForceOffPower();
		}
		else
		{
			ShutdownCNT++;
		}
	}

	if (IsFlag0(VICMVHiFlagACDC, DC_OW_Prochot))
	{
		if (DC_OW_En_ProchotCnt >= DC_OW_En_Debounce)
		{
			SetFlag(VICMVHiFlagACDC, DC_OW_Prochot);
			h_prochot_enable();
		}
		else
		{
			DC_OW_En_ProchotCnt++;
		}
	}
}


void DC_OW_Dis_Check(void)
{
	if (L_DC_W >= Bat_UnderWattThreshold)
	{
		DC_OW_Dis_ProchotCnt = 0;
		return;
	}
	DC_OW_En_ProchotCnt = 0;

	if (IsFlag1(VICMVHiFlagACDC, DC_OW_Prochot))
	{
		if (DC_OW_Dis_ProchotCnt >= DC_OW_Dis_Debounce)
		{
			ClrFlag(VICMVHiFlagACDC, DC_OW_Prochot);
			Check_PROCHOT_Disable();	// leox20150916
		}
		else
		{
			DC_OW_Dis_ProchotCnt++;
		}
	}
}


//------------------------------------------------------------------------------
// Check AC is over current peak or not
//------------------------------------------------------------------------------
void AC_OCP_En_Check(void)
{
	static BYTE ShutdownCNT = 0;

	if (VICM_VACDC < AC65W_OverPeakCurrentThreshold)
	{
		AC_OCP_En_ProchotCnt = 0;
		ShutdownCNT = 0;
		return;
	}
	AC_OCP_Dis_ProchotCnt = 0;

	if (IsFlag1(VICMVHiFlagACDC, AC_OCP_Prochot))
	{
		if (ShutdownCNT >= ACDC_OC_OW_Shutdown_Debounce)
		{
			ShutdownCNT = 0;
			ShutDnCause = 0xFE;
			ForceOffPower();
		}
		else
		{
			ShutdownCNT++;
		}
	}

	if (IsFlag0(VICMVHiFlagACDC, AC_OCP_Prochot))
	{
		if (AC_OCP_En_ProchotCnt >= AC_OCP_En_Debounce)
		{
			SetFlag(VICMVHiFlagACDC, AC_OCP_Prochot);
			h_prochot_enable();
		}
		else
		{
			AC_OCP_En_ProchotCnt++;
		}
	}
}


void AC_OCP_Dis_Check(void)
{
	if (VICM_VACDC >= AC65W_UnderPeakCurrentThreshold)
	{
		AC_OCP_Dis_ProchotCnt = 0;
		return;
	}
	AC_OCP_En_ProchotCnt = 0;

	if (IsFlag1(VICMVHiFlagACDC, AC_OCP_Prochot))
	{
		if (AC_OCP_Dis_ProchotCnt >= AC_OCP_Dis_Debounce)
		{
			ClrFlag(VICMVHiFlagACDC, AC_OCP_Prochot);
			Check_PROCHOT_Disable();	// leox20150916
		}
		else
		{
			AC_OCP_Dis_ProchotCnt++;
		}
	}
}


//------------------------------------------------------------------------------
// Check DC is over current peak or not
//------------------------------------------------------------------------------
void DC_OCP_En_Check(void)
{
	static BYTE ShutdownCNT = 0;

	if (L_VICM_V < Bat_OverPeakCurrentThreshold)
	{
		DC_OCP_En_ProchotCnt = 0;
		ShutdownCNT = 0;
		return;
	}
	DC_OCP_Dis_ProchotCnt = 0;

	if (IsFlag1(VICMVHiFlagACDC, DC_OCP_Prochot))
	{
		if (ShutdownCNT >= ACDC_OC_OW_Shutdown_Debounce)
		{
			ShutdownCNT = 0;
			ShutDnCause = 0xFE;
			ForceOffPower();
		}
		else
		{
			ShutdownCNT++;
		}
	}

	if (IsFlag0(VICMVHiFlagACDC, DC_OCP_Prochot))
	{
		if (DC_OCP_En_ProchotCnt >= DC_OCP_En_Debounce)
		{
			SetFlag(VICMVHiFlagACDC, DC_OCP_Prochot);
			h_prochot_enable();
		}
		else
		{
			DC_OCP_En_ProchotCnt++;
		}
	}
}


void DC_OCP_Dis_Check(void)
{
	if (L_VICM_V >= Bat_UnderPeakCurrentThreshold)
	{
		DC_OCP_Dis_ProchotCnt = 0;
		return;
	}
	DC_OCP_En_ProchotCnt = 0;

	if (IsFlag1(VICMVHiFlagACDC, DC_OCP_Prochot))
	{
		if (DC_OCP_Dis_ProchotCnt >= DC_OCP_Dis_Debounce)
		{
			ClrFlag(VICMVHiFlagACDC, DC_OCP_Prochot);
			Check_PROCHOT_Disable();	// leox20150916
		}
		else
		{
			DC_OCP_Dis_ProchotCnt++;
		}
	}
}


// leox20150916 Load MBATV over/under current threshold according to battery PN
const sMBATV_Threshold code CmpMBATVTbl[] =	// The order the same as BatCfgTbl[]
{//  Over/Under Watt     , Over/Under peak current 	//   #  Battery_PN
	{        0,         0,         0,        0},	//   0  "        "
	{W_66000mW, W_59400mW, I_10000mA, I_09000mA},	//   1  "********"
	{W_66000mW, W_59400mW, I_10000mA, I_09000mA},	//   2  "CP671398"
	{W_66000mW, W_59400mW, I_09000mA, I_08100mA},	//   3  "CP656340"
	{W_66000mW, W_59400mW, I_10000mA, I_09000mA},	//   4  "CP671396"
	{W_66000mW, W_59400mW, I_09000mA, I_08100mA},	//   5  "CP651529"
	{W_56700mW, W_51030mW, I_07000mA, I_06300mA},	//   6  "CP656728"
	{W_56700mW, W_51030mW, I_07000mA, I_06300mA},	//   7  "CP656352"
	{W_56700mW, W_51030mW, I_10000mA, I_09000mA},	//   8  "CP652728"
	{W_56700mW, W_51030mW, I_06600mA, I_06000mA},	//   9  "CP671395"
	{W_66000mW, W_59400mW, I_10000mA, I_09000mA},	//  10  "CP700280"
	{W_56700mW, W_51030mW, I_06600mA, I_06000mA},	//  11  "CP700278"			// leox20151211 Bug fixed by Leo Liu
	{W_66000mW, W_59400mW, I_10000mA, I_09000mA},	//  12  "CP700282"
	{W_66000mW, W_59400mW, I_09000mA, I_08100mA},	//  13  "CP708752"
};

void LoadMBATV_Threshold(BYTE Model)
{
	if (Model >= GetArrayNum(CmpMBATVTbl)) {return;}
	Bat_OverWattThreshold         = CmpMBATVTbl[Model].OverCurrentThreshold;
	Bat_UnderWattThreshold        = CmpMBATVTbl[Model].UnderCurrentThreshold;
	Bat_OverPeakCurrentThreshold  = CmpMBATVTbl[Model].OverPeakCurrentThreshold;
	Bat_UnderPeakCurrentThreshold = CmpMBATVTbl[Model].UnderPeakCurrentThreshold;
}


	#else	// (original)	// leo_20150916 add power protect ----
void CheckVICMVThrottlingFUNACDC(void)
{
	if (IsFlag0(POWER_FLAG, ACPI_OS))
	{
		InitVICMVHiACDC();
	}
	else
	{
		LoadVICM_VACDC_Limit();
		Get_VICMVoltageACDC();
		ChkIfEnableVICMVThrottly();
		ChkIfDisableVICMVThrottly();
	}
}


void InitVICMVHiACDC(void)
{
	if (IsFlag1(VICMVHiFlagACDC, Q42Q43_SCIACDC))
	{
		ClrFlag(VICMVHiFlagACDC, Q42Q43_SCIACDC);
		ECQEvent(Q43_SCIACDC, SCIMode_Normal);
	}
	VICM_VACDC = 0;
	VICMVHiEnThroLimitCntACDC = 0;
	VICMVHiDisThroLimitCntACDC = 0;
}


void LoadVICM_VACDC_Limit(void)
{
	CmpEnVICM_VACDC_Limit = VICMVHiEnThroLimitAC90W;
	CmpDisVICM_VACDC_Limit = VICMVLoDisThroLimitAC90W;
	BatteryCellNum = (((ABAT_CELLS >> 4) & 0x0F) * (ABAT_CELLS & 0x0F));
	if (IsFlag0(POWER_STAT_CTRL, SysPwrSourceAC))
	{
		if (BatteryCellNum == 4)
		{
			CmpEnVICM_VACDC_Limit = VICMVHiEnThroLimitDC4Cell;
			CmpDisVICM_VACDC_Limit = VICMVLoDisThroLimitDC4Cell;
		}
		if (BatteryCellNum == 8)
		{
			CmpEnVICM_VACDC_Limit = VICMVHiEnThroLimitDC8Cell;
			CmpDisVICM_VACDC_Limit = VICMVLoDisThroLimitDC8Cell;
		}
	}
}


void Get_VICMVoltageACDC(void)
{
	VICM_VACDC = ADV_DAT1;
}


void ChkIfEnableVICMVThrottly(void)
{
	if (VICM_VACDC < CmpEnVICM_VACDC_Limit)
	{
		VICMVHiEnThroLimitCntACDC = 0;
		return;
	}
	VICMVHiDisThroLimitCntACDC = 0;

	if (IsFlag0(VICMVHiFlagACDC, Q42Q43_SCIACDC))
	{
		if (VICMVHiEnThroLimitCntACDC >= VICMVHiEnThroLimitDebounceACDC)
		{
			SetFlag(VICMVHiFlagACDC, Q42Q43_SCIACDC);
			ECQEvent(Q42_SCIACDC, SCIMode_Normal);
		}
		else
		{
			VICMVHiEnThroLimitCntACDC++;
		}
	}
}


void ChkIfDisableVICMVThrottly(void)
{
	if (VICM_VACDC >= CmpDisVICM_VACDC_Limit)
	{
		VICMVHiDisThroLimitCntACDC = 0;
		return;
	}
	VICMVHiEnThroLimitCntACDC = 0;

	if (IsFlag1(VICMVHiFlagACDC, Q42Q43_SCIACDC))
	{
		if (VICMVHiDisThroLimitCntACDC >= VICMVHiDisThroLimitDebounceACDC)
		{
			ClrFlag(VICMVHiFlagACDC, Q42Q43_SCIACDC);
			ECQEvent(Q43_SCIACDC, SCIMode_Normal);
		}
		else
		{
			VICMVHiDisThroLimitCntACDC++;
		}
	}
}
	#endif	// BAT_MBATV_PROTECT
#endif	//OEM_VICMVACDC
