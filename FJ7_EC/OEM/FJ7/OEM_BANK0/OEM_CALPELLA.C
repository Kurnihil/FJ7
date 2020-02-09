/*------------------------------------------------------------------------------
 * Title : OEM_CALPELLA.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_Calpella
void Check_SLP_M(void)
{
	if (IsFlag0(ToSBsignals00, SB_PowerSource)) || IsFlag0(FromSBsignals00, SLP_M))
	{
		TurnOffSLP_M_On();
	}
	else
	{
		TurnOnSLP_M_On();
	}
}


void TurnOffSLP_M_On(void)
{
	if (IsFlag1(ToSBsignals00, SLP_M_On))
	{
		SLP_M_On_off();
		ClrFlag(ToSBsignals00, SLP_M_On);
	}
}


void TurnOnSLP_M_On(void)
{
	if (IsFlag0(ToSBsignals00, SLP_M_On))
	{
		SLP_M_On_on();
		SetFlag(ToSBsignals00, SLP_M_On);
	}
}

void ChkneedTurnOffSBLANPwrFS345(void)
{
	if (IsFlag1(POWER_FLAG, enter_SUS))
	{
		if (IsFlag0(POWER_STAT_CTRL, WakeOnLan)) {TurnOffLanPower();}
		return;
	}

	if (IsFlag1(POWER_FLAG, power_on)) || (IsFlag0(ToSBsignals00, SB_PowerSource))
	{
		ChkTurnOffSBLANPwrFS345CNT = 0;
		return;
	}

	ChkTurnOffSBLANPwrFS345CNT++;
	if (ChkTurnOffSBLANPwrFS345CNT >= 5)
	{
		ChkTurnOffSBLANPwrFS345CNT = 0;
		CheckTurnOffSBLANPowerSource();
	}
}


void CommonStateChange_Calpella(void)
{
	PCH_RemoteErrorCnt = 0;
	PECI_RemoteErrorCnt = 0;
	ClrFlag(MISC_FLAG, PCH_Sensor_SMB_error);
	ClrFlag(MISC_FLAG, PECI_Sensor_SMB_error);
	SetFlag(KETBOARD_STAT_CTRL, NotReadPECI);
}


void TurnOffSBPowerSource_Calpella(void)
{
	#ifdef	DeepS3_SUPPORT	// 20120110 chris modify for support Calpella and Deep S3
	if ((IsFlag1(FromSBsignals00, SUS_PWR_DN_ACK) && IsFlag0(FromSBsignals00, SLP_M) &&
		IsFlag0(FromSBsignals00, SLP_LAN) && IsFlag0(FromSBsignals00, WOL_EN) &&
		IsFlag1(ToSBsignals00, SB_PowerSource)) || SLP_SUS_Low())
	#else
	if (IsFlag1(FromSBsignals00, SUS_PWR_DN_ACK) && IsFlag0(FromSBsignals00, SLP_M) &&
		IsFlag0(FromSBsignals00, SLP_LAN) && IsFlag0(FromSBsignals00, WOL_EN) &&
		IsFlag1(ToSBsignals00, SB_PowerSource))
	#endif
	{
		Delay500us();			// Delay 500us	// leox_20130717
		ClrFlag(POWER_FLAG, PwrOffRVCCOff);
		RSMRST_on();			// leox_20111109
		#ifdef DeepS3_SUPPORT	// leox_20120111
		DelayXms(SLP_SUS_Low() ? RVcc2RSMRST_DS3 : RVcc2RSMRST);
		#else
		DelayXms(RVcc2RSMRST);
		#endif
		USB_CHARGE_ILIM_LO();	// CB1 low	// leox20150626 For USB charger SLG55544 of FH9C
		USB_CHARGE_CTL1_LO();	// CB0 low	// leo_20150917 USB charging
		STBON_off();
		ClrFlag(ToSBsignals00, SB_PowerSource);
		SYSTEMSTATE = 0x05;
		ResetSusBSusC();
		AC_PRESENT_input();
	}
}
#endif	//OEM_Calpella


// leox20150522 Move out of switch define OEM_Calpella for FH9C +++
void Check_AC_PRESENT(void)
{
	if (IsFlag1(ToSBsignals00, SB_PowerSource))
	{
		if (IsFlag1(POWER_STAT_CTRL, adapter_in))
		{
			TurnOnAC_PRESENT();
		}
		else
		{
			TurnOffAC_PRESENT();
		}
	}
}


void TurnOnAC_PRESENT(void)
{
	AC_PRESENT_on();
	SetFlag(ToSBsignals00, AC_Present);
}


void TurnOffAC_PRESENT(void)
{
	AC_PRESENT_off();
	ClrFlag(ToSBsignals00, AC_Present);
}
// leox20150522 Move out of switch define OEM_Calpella for FH9C ---
