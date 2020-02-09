/*------------------------------------------------------------------------------
 * Title : OEM_NECP.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_NECP
	#ifdef	OEM_BATTERY
void NECStopCharging(void)
{
	ClrFlag(SEL_BAT_MISC, bat_force_chg + bat_force_disc + bat_next + bat_inUSE);
	ClrFlag(SEL_BAT_STATUS, bat_dischg);
	ClrFlag(SEL_CHARGE_STATUS, bat_level3);
	TurnOffCharger();
	UpdateBatteryStatus();
}


#ifdef	BattChg_SMB_OVS
void ChkChgCurfromBattMax(void)
{
	int SMB_BAT_ChargingCurrent = 0;

	ClrFlag(NECOverChgCurrent, StopChgCurOVSbyBatt);
	if ((IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_destroy)) && (ABAT_MODEL != 0))
	{
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		SMB_ADDR_OEM = _SMB_BatteryAddr;
		SMB_CMD_OEM = _CMD_ChargingCurrent;
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		if (vOSSMbusRWord_OEM())
		{
			SMB_BAT_ChargingCurrent = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
			switch (ABAT_MODEL)
			{
			case 1:
				if (SMB_BAT_ChargingCurrent > ChgCurrMaxValue_M1)
				{
					SetFlag(NECOverChgCurrent, StopChgCurOVSbyBatt);
				}
				break;

			case 2:
				if (SMB_BAT_ChargingCurrent > ChgCurrMaxValue_M2)
				{
					SetFlag(NECOverChgCurrent, StopChgCurOVSbyBatt);
				}
				break;

			case 3:
				if (SMB_BAT_ChargingCurrent > ChgCurrMaxValue_M3)
				{
					SetFlag(NECOverChgCurrent, StopChgCurOVSbyBatt);
				}
				break;

			case 4:
				if (SMB_BAT_ChargingCurrent > ChgCurrMaxValue_M4)
				{
					SetFlag(NECOverChgCurrent, StopChgCurOVSbyBatt);
				}
				break;

			default:
				break;
			}
		}
	}
}
#endif	//BattChg_SMB_OVS


void IfReadWriteBattFirstDate(void)
{
	if ((IsFlag0(ABAT_STATUS, abat_in)) || (ABAT_MODEL == 0))
	{
		ChkReadWriteBattFirstUSEDate = 0;
		return;
	}
	else
	{
		IfReadBattFirstUSEDate();
		IfWriteBattFirstUSEDate();
		ChkReadWriteBattFirstUSEDate = 0;
	}
}


void IfReadBattFirstUSEDate(void)
{
    if (ChkReadWriteBattFirstUSEDate == 0xFA) {SeveReadBattFirstUSEDate();}
}


void IfWriteBattFirstUSEDate(void)
{
	if (ChkReadWriteBattFirstUSEDate == 0xFB) {DoWriteBattFirstUSEDate();}
}


#ifdef	NECP_BattRefresh_Support
void InitBattToNormalCondition(void)
{
	ClrFlag(NECBattRefreshPSTS, StopChgDischgDoing);
	ClrFlag(NECBattRefreshPSTS, ForceDischgDoing);
	ClrFlag(ABAT_MISC, abat_StopChg);
	ClrFlag(ABAT_MISC, abat_AutoLearn);
}


void ChkNECBattRefreshPSTS(void)
{
	if ((NECBattRefreshPSTS & 0x03) != 0x03)
	{	// ADPC!='1' or CHGC!='1'
		ClrFlag(NECBattRefreshPSTS01, ToNormalCondition01_OK);
	}

	if (IsFlag0(NECBattRefreshPSTS, ADPC))
	{
		if (IsFlag0(NECBattRefreshPSTS, ForceDischgDoing))
		{	// ADPC='0' ForceDischgDoing='0'
			SetFlag(NECBattRefreshPSTS, ForceDischgDoing);
			ClrFlag(NECBattRefreshPSTS, StopChgDischgDoing);
			ClrFlag(ABAT_MISC, abat_StopChg);
			SetFlag(ABAT_MISC, abat_AutoLearn);
			TurnOnDischarger();
		}
		else
		{	// ADPC='0' ForceDischgDoing='1'
			if (ABAT_CAP <= 3) {SetFlag(NECBattRefreshPSTS, ADPC);}
		}
	}
	else
	{
		if (IsFlag0(NECBattRefreshPSTS, CHGC))
		{	// ADPC='1' CHGC='0'
			if (IsFlag0(NECBattRefreshPSTS, StopChgDischgDoing))
			{	// ADPC='1' CHGC='0' StopChgDischgDoing='0'
				SetFlag(NECBattRefreshPSTS, StopChgDischgDoing);
				ClrFlag(NECBattRefreshPSTS, ForceDischgDoing);
				TurnOffADISCHG();
				SetChargeCurrent0mA();
			}
			else
			{	// ADPC='1' CHGC='0' StopChgDischgDoing='1'
				SetChargeCurrent0mA();
				if (IsFlag0(ABAT_MISC, abat_StopChg)) {SetFlag(ABAT_MISC, abat_StopChg);}
			}
		}
		else
		{	// ADPC='1' CHGC='1'
			InitBattToNormalCondition01();
			if ((AC_IN_read()) && (IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_full)))
			{
				TurnOnCharger();
			}
		}
	}
}


void InitBattToNormalCondition01(void)
{
	if (IsFlag0(NECBattRefreshPSTS01, ToNormalCondition01_OK))
	{
		SetFlag(NECBattRefreshPSTS01, ToNormalCondition01_OK);
		ClrFlag(NECBattRefreshPSTS, StopChgDischgDoing);
		ClrFlag(NECBattRefreshPSTS, ForceDischgDoing);
		ClrFlag(ABAT_MISC, abat_StopChg);
		ClrFlag(ABAT_MISC, abat_AutoLearn);
		TurnOffADISCHG();
	}
}


// leox_20130312 Modify for new timer routine (3 sec)
void CheckNECP_BattRefresh(void)
{
	if (IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_destroy))
	{
		if (((NECBattRefreshPSTS & 0x03) != 0) || (ABAT_CAP >= 7))
		{
			ChkNECPBattRefreshFun();
		}
	}
}

// leox_20130312 Add for new timer routine (500ms)
void CheckNECP_BattRefresh2(void)
{
	if (IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_destroy))
	{
		if (((NECBattRefreshPSTS & 0x03) == 0) && (ABAT_CAP < 7))
		{
				ChkNECPBattRefreshFun();
		}
	}
}


void ChkNECPBattRefreshFun(void)
{
	if (IsFlag1(POWER_FLAG, ACPI_OS)) {ChkNECBattRefreshPSTS();}
}
#endif	//NECP_BattRefresh_Support


#ifdef	Battery_Protection_NEC
void ResetBatteryProtectionFunNEC(void)
{
	ClrFlag(NECOverChgCurrent, C_First_USE_Date_Bit15_Set);
}
#endif	//Battery_Protection_NEC
	#endif	//OEM_BATTERY
#endif	//OEM_NECP
