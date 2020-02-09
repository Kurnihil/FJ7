/*------------------------------------------------------------------------------
 * Title: OEM_MAIN.C - OEM specific code
 *
 * Some "HOOKS" from CHIPHOOK.C are copied into this file in order to replace
 * (or augment) core functions.
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


//------------------------------------------------------------------------------
// Internal Function Declaration
//------------------------------------------------------------------------------
#ifdef	SUPPORT_Shipmode
extern BYTE vOSSMbusWWord(void);
#endif

BYTE vDebounce(BYTE device_id);
void DebouncePwrBtn(BYTE dev_event);	// leox_20130219
void DebounceAcIn(BYTE dev_event);		// leox_20130219
void DebounceBatA(BYTE dev_event);		// leox_20130219
void DebounceLid(BYTE dev_event);		// leox_20130219

#if	SUPPORT_OEM_APP
void OemAppGetSetVol(void);		// leox_20120507
void OemAppCntTask(void);			// leox_20120507
void OemAppCmd(void);				// leox_20120507
#endif	// SUPPORT_OEM_APP


//------------------------------------------------------------------------------
// Device insert/remove debounce routine
//  Input : device_id
//  Output: 0: Status not changed
//          1: Device connected
//          2: Device removed
//          3: During debounce
//          4: Error device ID
//------------------------------------------------------------------------------
// leox_20130219 Rewrite function
const struct sDebounce code Debounce_TBL[] =
{
	{&POWER_FLAG		, wait_PSW_off	, &PSW_BOUNCE	, T_PSW_DEBOUNCE	, DebouncePwrBtn},
	{&POWER_STAT_CTRL	, adapter_in	, &AC_BOUNCE	, T_AC_DEBOUNCE		, DebounceAcIn	},
	{&ABAT_STATUS		, abat_in		, &ABAT_BOUNCE	, T_BAT_DEBOUNCE	, DebounceBatA	},
	{&Device_STAT_CTRL	, evt_lid		, &EXTEVT_BOUNCE, T_EXTEVT_DEBOUNCE	, DebounceLid	},
};


BYTE vDebounce(BYTE device_id)
{
	BYTE new_state, old_state;
	BYTE do_nochange_func = 0;
	BYTE device_fg;

	switch (device_id)
	{
	case CheckPowerSwitch:
		#ifdef	OEM_BATTERY
		if (IsFlag0(Factory_Test_Status, LID_Dis))
			new_state = NBSWON_Status_Low() & (!LIDSW_read());
		else
			new_state = NBSWON_Status_Low();
		#else	//OEM_BATTERY
		new_state = NBSWON_Status_Low();
		#endif	//OEM_BATTERY
		do_nochange_func = new_state;
		break;

	case CheckACIn:
		new_state = AC_IN_read();
		break;

	#ifdef	OEM_BATTERY
	case CheckBatteryIn:
		new_state = BATT_IN_read();
		break;

	case CheckLIDIn:
		if (IsFlag0(Factory_Test_Status, LID_Dis))
			new_state = LIDSW_read();
		else
			new_state = 0;
		break;
	#endif	// OEM_BATTERY

	default:
		return 4;
	}

	Tmp_XPntr  = Debounce_TBL[device_id].Reg;
	Tmp_XPntr1 = Debounce_TBL[device_id].Counter;
	device_fg  = Debounce_TBL[device_id].Flag;

	old_state = ((*Tmp_XPntr & device_fg) != 0);
	if (new_state != old_state)
	{
		if (*Tmp_XPntr1 == 0)
		{
			*Tmp_XPntr1 = Debounce_TBL[device_id].Time;	// Set debounce counter
		}
		else
		{
			if (--(*Tmp_XPntr1) == 0)
			{
				if (new_state)
				{	// Device connected
					(Debounce_TBL[device_id].Func)(MAKE_EVENT);
					*Tmp_XPntr |= device_fg;
					return 1;
				}
				else
				{	// Device removed
					(Debounce_TBL[device_id].Func)(BREAK_EVENT);
					*Tmp_XPntr &= (~device_fg);
					return 2;
				}
			}
			else
			{
				return 3;
			}
		}
	}
	else
	{
		*Tmp_XPntr1 = 0;
	}

	if (do_nochange_func) {(Debounce_TBL[device_id].Func)(REPEAT_EVENT);}

	return 0;
}


//------------------------------------------------------------------------------
// Device debounce routine - Power button
//  dev_event: MAKE_EVENT, BREAK_EVENT or REPEAT_EVENT
//------------------------------------------------------------------------------
// leox_20130219 Code from PSWPressed(), PSWReleased() and ChkMechanicalOff()
void DebouncePwrBtn(BYTE dev_event)
{
	if (dev_event == MAKE_EVENT)
	{	// PSWPressed()
		SetFlag(POWER_FLAG, wait_PSW_off);
		//David add for notify BIOS 2014/08/06
		ECQEvent(Q8B_SMISWISCI, SCIMode_Normal);
		//David add for notify BIOS 2014/08/06
		#ifdef	DeepS3_SUPPORT	// 20120104 Chris add for support Deep S3
		if (IsFlag1(POWER_FLAG, Deep_S3)) {ClrFlag(POWER_FLAG, Deep_S3);}
		#endif
		if (IsFlag0(POWER_FLAG, power_on))
		{
			CheckPower();
			if (IsFlag0(MISC_FLAG, F_CriLow))
			{
				SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
				PSWOnPower();
			}
		}
		else
		{
			// leo_20160715 add for press power button shutdown when set menu to turn off S5_ON power in some conditions, for let S5 power comsumption lower. +++
			if (IsFlag0(POWER_FLAG, ACPI_OS))
			{
				ClrFlag(POWER_STAT_CTRL1, S5_Reboot);
			}
			// leo_20160715 add for press power button shutdown when set menu to turn off S5_ON power in some conditions, for let S5 power comsumption lower. ---
			PSWOffPower();
		}
	}
	else if (dev_event == BREAK_EVENT)
	{	// PSWReleased()
		ClrFlag(POWER_FLAG, wait_PSW_off);
		PSW_COUNTER = 0;
		SetFlag(MISC_FLAG, F_DelayPSW);
		PSW_TIMER = T_PSW_DELAY;
	}
	else if (dev_event == REPEAT_EVENT)
	{	// ChkMechanicalOff()
		if (IsFlag1(POWER_FLAG, wait_PSW_off))
		{
			if ((PSW_COUNTER > 0) && (--PSW_COUNTER == 0))
			{
				ShutDnCause = 0x03;
				ForceOffPower();
			}
		}
	}
}

//------------------------------------------------------------------------------
// Device debounce routine - AC insert
//  dev_event: MAKE_EVENT, BREAK_EVENT or REPEAT_EVENT
//------------------------------------------------------------------------------
// leox_20130219 Code from ACInserted(), ACRemoved() and ACNoChange()
void DebounceAcIn(BYTE dev_event)
{
	if (dev_event == MAKE_EVENT)
	{	// ACInserted()
		#ifdef	NECP_BattRefresh_Support
		NECBattRefreshPSTS = 3;
		#endif	// NECP_BattRefresh_Support
		ConfigACIn();
	}
	else if (dev_event == BREAK_EVENT)
	{	// ACRemoved()
		#ifdef	OEM_BATTERY
			#ifdef	NECP_BattRefresh_Support
		NECBattRefreshPSTS = 3;
		InitBattToNormalCondition();
			#endif	// NECP_BattRefresh_Support
		#endif	// OEM_BATTERY
		ConfigACOut();
	}
	else if (dev_event == REPEAT_EVENT)
	{	// ACNoChange()
		// Do nothing
	}
}


//------------------------------------------------------------------------------
// Device debounce routine - Battery A insert
//  dev_event: MAKE_EVENT, BREAK_EVENT or REPEAT_EVENT
//------------------------------------------------------------------------------
// leox_20130219 Create function
void DebounceBatA(BYTE dev_event)
{
	#ifdef	OEM_BATTERY
	if (dev_event == MAKE_EVENT)
	{
		ABATInserted();
	}
	else if (dev_event == BREAK_EVENT)
	{
		ABATRemoved();
	}
	else if (dev_event == REPEAT_EVENT)
	{
		ABATNoChange();
	}
	#endif	// OEM_BATTERY
}


//------------------------------------------------------------------------------
// Device debounce routine - Lid
//  dev_event: MAKE_EVENT, BREAK_EVENT or REPEAT_EVENT
//------------------------------------------------------------------------------
// leox_20130219 Create function
void DebounceLid(BYTE dev_event)
{
	if (IsFlag1(Factory_Test_Status, LID_Dis))
		return;
	#ifdef	OEM_BATTERY
	if (dev_event == MAKE_EVENT)
	{
		LIDInserted();
	}
	else if (dev_event == BREAK_EVENT)
	{
		LIDRemoved();
	}
	else if (dev_event == REPEAT_EVENT)
	{
		LIDNoChange();
	}
	#endif	// OEM_BATTERY
}


void Delay10plus3XuS(BYTE Timer)
{
	for (; Timer != 0; Timer--) {}
}


void ConfigACIn(void)
{
	#ifdef	OEM_IPOD
	ConfigACIn_OEM_IPOD();
	#endif	//OEM_IPOD

	#ifdef	OEM_Calpella
		#ifdef	Calpella_iAMT_Support
	TurnOnSBPowerSource();
		#endif	//Calpella_iAMT_Support
	#endif	//OEM_Calpella

	SetFlag(POWER_STAT_CTRL, adapter_in);
	SetFlag(Hybrid_Status, Hybrid_Check);
	ClrFlag(Hybrid_Status, Hybrid_Recharge);
	ClrFlag(Hybrid_Status, Hybrid_State);
	ClrFlag(MISC_FLAG, F_CriLow);
	ClrFlag(Shipmode_status, Wait_AC);
	#if SUPPORT_EFLASH_FUNC	// leox20160322
	eFlashSave(&Shipmode_status, Shipmode_status);
	#endif

	#ifdef SUPPORT_BQ24780S	// leox20150824 AC in when system in S3/S4/S5
	if (IsFlag0(BAT_MISC3, BQ24780S_Init_OK)) {BQ24780S_Init();}
	#endif

	#ifdef	OEM_BATTERY
	if ((IsFlag0(ABAT_MISC, abat_AutoLearn)) && (IsFlag0(BBAT_MISC, bbat_AutoLearn)))
	{
		ECOSleepCheck();
		if (IsFlag0(POWER_STAT_CTRL1, AC_Status))
		{
			#ifdef SUPPORT_BQ24780S	// leox20150921 Fix incorrect status of ECO mode
			DCMODE();
			#endif
			return;
		}
		else
		{
			ClrFlag(ABAT_MISC, force_discA);
			ClrFlag(BBAT_MISC, force_discB);
			TurnOffADISCHG();
			TurnOffBDISCHG();
			ACMODE();
		}
	}
	#if BAT_DCHG_PROTECT	// leox20150826 Battery DCHG protection
	if (IsFlag1(BAT_MISC3, DCHG_Throttling))
	{
		ClrFlag(BAT_MISC3, DCHG_Throttling);
		Check_PROCHOT_Disable();
	}
	#endif	// BAT_DCHG_PROTECT
	#endif	//OEM_BATTERY
}


void ConfigACOut(void)
{
	#ifdef	OEM_IPOD
	ConfigACOut_OEM_IPOD();
	if ((IsFlag0(POWER_FLAG, power_on)) && (IsFlag0(POWER_FLAG, enter_SUS))) {CheckTurnOffSBLANPowerSource();}
	#endif	//OEM_IPOD

	#ifdef	OEM_BATTERY
	OffChargeABT();
	OffChargeBBT();
	if ((IsFlag0(ABAT_STATUS, abat_dischg)) && (IsFlag0(BBAT_STATUS, bbat_dischg)))
	{
		SetCHG_High();
		//David add for charge IC ISL88732 2014/05/21
		#ifdef	SUPPORT_BQ24780S	// leox20150607
		SetChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
		#else	// (original)
		ClearChargeControlCHGIC(Charge_Enable);
		#endif
		//David add for charge IC ISL88732 2014/05/21
		SetChargeCurrent0mA();
		DCMODE();
	}
	#endif	//OEM_BATTERY

	ClrFlag(ABAT_STATUS, abat_full);
	ClrFlag(BBAT_STATUS, bbat_full);
	ClrFlag(POWER_STAT_CTRL, adapter_in);
	ClrFlag(POWER_STAT_CTRL1, lowpower_CHG);
	ClrFlag(Hybrid_Status, Hybrid_Recharge);
	ClrFlag(Hybrid_Status, Hybrid_State);

	#ifdef SUPPORT_BQ24780S	// leox20150824 AC out when system in S3/S4/S5
	if (IsFlag0(POWER_FLAG, power_on)) {ClrFlag(BAT_MISC3, BQ24780S_Init_OK);}
	#endif

	if (IsFlag0(ABAT_MISC, abat_AutoLearn))
	{
		ClrFlag(ABAT_MISC, force_chgA);
		if (RSOC_FULL_BASE<=SEL_BAT_CAP)			// if battery RSOC bigger then recharge RSOC base
			SetFlag(ABAT_MISC, abat_ValidFull);		// set validfull flag for not recharge
	}
	if (IsFlag0(BBAT_MISC, bbat_AutoLearn)) {ClrFlag(BBAT_MISC, force_chgB);}

	if (IsFlag0(BBAT_MISC, force_discB))
	{
		if (IsFlag1(ABAT_MISC, abat_next))
		{
			SetFlag(ABAT_MISC, force_discA);			// Discharge ABAT first by request
		}
		else
		{
			if (IsFlag0(ABAT_MISC, force_discA))
			{
				if (IsFlag1(BBAT_MISC, bbat_next))
				{
					SetFlag(BBAT_MISC, force_discB);	// Discharge BBAT first by request
				}
			}
		}
	}
	//David modify for battery shutdown mode 2014/07/02
	if ((IsFlag1(ABAT_MISC2, Shipmode)) && (IsFlag0(POWER_FLAG, power_on)))
		LED_FLASH_CNT = 0x0001;
	//David modify for battery shutdown mode 2014/07/02
}


void LIDInserted(void)
{
	SetFlag(Device_STAT_CTRL, evt_lid);
	SendLIDQeventForBIOS();
}


void LIDRemoved(void)
{
	ClrFlag(Device_STAT_CTRL, evt_lid);
	SendLIDQeventForBIOS();
}


void LIDNoChange(void)
{

}


void SendLIDQeventForBIOS(void)
{
	if ((IsFlag1(POWER_FLAG, enter_SUS)) || (IsFlag1(POWER_FLAG, system_on)))
	{
		if (IsFlag1(POWER_FLAG, enter_SUS))
		{
			CheckPower();
			if ((IsFlag0(MISC_FLAG, F_CriLow)) && (IsFlag0(Device_STAT_CTRL, evt_lid)))
			{
				SendQLIDSWIForBIOS();
			}
		}
		else
		{
			if (IsFlag1(POWER_FLAG, ACPI_OS))
			{
				SendQLIDSCIForBIOS();
			}
			else
			{
				SendQLIDSMIForBIOS();
			}
		}
	}
}


void SendQLIDSWIForBIOS(void)
{
	#if 0	// leox20150626 Assert pin DNBSWON# instead of SWI# to workaround hardware issue for FH9C	// CHECK	// leox20150821 SWI for PVT
	// Pin SIO_EXT_SWI# (aka SWI#) needs to set to INPUT of "OEM_GPIO.H" also
	SetFlag(POWER_FLAG, wait_PSW_off);
	PSWOnPower();
	#else	// (original)
	//WriteSWI_QueryValue(Q8A_SMISWISCI);
	SWI_on();
	Delay10plus3XuS(66);	// Delay 100us	//David modify for LID S3 resume 2014/06/04
	SWI_off();
	#endif
	SetFlag(POWER_STAT_CTRL, SWILIDWakeFlag);
}


void SendQLIDSCIForBIOS(void)
{
	ECQEvent(Q8A_SMISWISCI, SCIMode_Normal);
}


void SendQLIDSMIForBIOS(void)
{
	ECSMIEvent(Q8A_SMISWISCI);
}


// Add One Touch Buttons function 2011/08/19 by Fox Zhu
//------------------------------------------------------------------------------
// Button function takes a BYTE for a parameter
//  event: MAKE_EVENT, BREAK_EVENT or REPEAT_EVENT
//------------------------------------------------------------------------------
void OneTouchButt0_Func(BYTE event)
{

}


void OneTouchButt1_Func(BYTE event)
{

}


void OneTouchButt2_Func(BYTE event)
{	// Volume up
	if (event == MAKE_EVENT || event == REPEAT_EVENT)
	{
		e0_prefix_code(0x32, MAKE_EVENT);
	}
	if (event == BREAK_EVENT)
	{
		e0_prefix_code(0x32, BREAK_EVENT);
	}
}


void OneTouchButt3_Func(BYTE event)
{	// Volume down
	if (event == MAKE_EVENT || event == REPEAT_EVENT)
	{
		e0_prefix_code(0x21, MAKE_EVENT);
	}
	if (event == BREAK_EVENT)
	{
		e0_prefix_code(0x21, BREAK_EVENT);
	}
}


void OneTouchButt4_Func(BYTE event)
{

}


void OneTouchButt5_Func(BYTE event)
{

}


void OneTouchButt6_Func(BYTE event)
{

}


void OneTouchButt7_Func(BYTE event)
{

}


//------------------------------------------------------------------------------
// Polling One Touch buttons
//------------------------------------------------------------------------------
const struct sButtons code OneTouchButt_TBL[] =
{
	{BIT0, OneTouchButt0_Func},
	{BIT1, OneTouchButt1_Func},
	{BIT2, OneTouchButt2_Func},
	{BIT3, OneTouchButt3_Func},
	{BIT4, OneTouchButt4_Func},
	{BIT5, OneTouchButt5_Func},
	{BIT6, OneTouchButt6_Func},
	{BIT7, OneTouchButt7_Func},
};


void PollOneTouchButtons(void)
{
	BYTE new_state, old_state;
	BYTE button_flag, button_id;

	button_id = (OneTouchPollStep & 0x07);
	switch (button_id)
	{
	case 0:
		new_state = OneTouchButt0_read();
		break;

	case 1:
		new_state = OneTouchButt1_read();
		break;

	case 2:
		new_state = OneTouchButt2_read();
		break;

	case 3:
		new_state = OneTouchButt3_read();
		break;

	case 4:
		new_state = OneTouchButt4_read();
		break;

	case 5:
		new_state = OneTouchButt5_read();
		break;

	case 6:
		new_state = OneTouchButt6_read();
		break;

	case 7:
		new_state = OneTouchButt7_read();
		break;
	}

	button_flag = OneTouchButt_TBL[button_id].Flag;
	old_state = ((OneTouchButtStatus & button_flag) != 0x00);

	if (new_state != old_state)
	{
		if (new_state)
		{
			(OneTouchButt_TBL[button_id].Func)(MAKE_EVENT);
			OneTouchButtStatus |= button_flag;
			OneTouchButtStatus2 = 0;
			SetFlag(OneTouchButtStatus2, OneTouchButt_TBL[button_id].Flag);
			if (bTMdelay < 5)
			{
				OneTouchButtonDelay = 1;
			}
			else
			{
				OneTouchButtonDelay = bTMdelay / 5;
			}
		}
		else
		{
			(OneTouchButt_TBL[button_id].Func)(BREAK_EVENT);
			OneTouchButtStatus &= (~button_flag);
		}
	}

	if (new_state)
	{
		if (IsFlag1(OneTouchButtStatus2, OneTouchButt_TBL[button_id].Flag))
		{
			if (OneTouchButtonDelay == 0)
			{
				if (bTMrepeat < 3)
				{
					OneTouchButtonDelay = 1;
				}
				else
				{
					OneTouchButtonDelay = bTMrepeat / 3;
				}
			}
			OneTouchButtonDelay--;
			if (OneTouchButtonDelay == 0)
			{
				(OneTouchButt_TBL[button_id].Func)(REPEAT_EVENT);
			}
		}
	}

	OneTouchPollStep += 1;
}
// Add One Touch Buttons function 2011/08/19 by Fox Zhu


void ScanAnalogInput(void)
{
	if (IsFlag1(VCH1CTL, BIT7))
	{
		SetFlag(VCH1CTL, BIT7);
		#if BAT_MBATV_PROTECT	// leo_20150916 add power protect	// leox20150916
		ADV_DAT5 = ((VCH1DATM << 8) + VCH1DATL);
		#else	// (original)
		ADV_DAT0 = ((VCH1DATM << 8) + VCH1DATL);
		#endif
	}

	if (IsFlag1(VCH2CTL, BIT7))
	{
		SetFlag(VCH2CTL, BIT7);
		ADV_DAT1 = ((VCH2DATM << 8) + VCH2DATL);
	}

	if (IsFlag1(VCH3CTL, BIT7))
	{
		SetFlag(VCH3CTL, BIT7);
		ADV_DAT2 = ((VCH3DATM << 8) + VCH3DATL);
	}
}


// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function ++
void IERx_BackUp(void)
{
	IER0_BK = IER0;
	IER1_BK = IER1;
	IER2_BK = IER2;
	IER3_BK = IER3;
}


void IERx_ReLoad(void)
{
	IER0 = IER0_BK;
	IER1 = IER1_BK;
	IER2 = IER2_BK;
	IER3 = IER3_BK;
}
// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function --


void HookChkIdle(void)
{
	if (IsFlag1(ToSBsignals00, SB_PowerSource) || IsFlag1(SYS_MISC2, InRestart)) {return;}
	if (IsFlag1(POWER_FLAG, power_on) || IsFlag1(POWER_FLAG, wait_PSW_off)) {return;}
	if (IsFlag1(POWER_FLAG, system_on) || IsFlag1(POWER_FLAG, enter_SUS)) {return;}

	#if	ENTER_SLEEP_MODE_AC			// leox_20110921
	//if (IsFlag0(B_PwrCtrl, EC_SLP_MODE_AC)) {return;}	// leox_20120203
	if (PCUHOLD_High()) {return;}	// leox_20111121
	if (IsFlag1(ACHARGE_STATUS, abat_InCharge)) {return;}
	#else	// (original)
	if (AC_IN_read() || IsFlag1(POWER_STAT_CTRL, adapter_in)) {return;}
	#endif

	#ifdef	OEM_IPOD
	if (HookChkIdle_OEM_IPOD() == 1) {return;}
	#endif	//OEM_IPOD

	#ifdef	DeepS3_SUPPORT	// 20120104 Chris add for support Deep S3
	if (SLP_SUS_Low()) {return;}
	#endif

	//David modify for battery shutdown mode 2014/07/02
	#ifdef	SUPPORT_Shipmode
	if (IsFlag1(ABAT_MISC2, Shipmode)) {return;}
	#endif	//SUPPORT_Shipmode
	//David modify for battery shutdown mode 2014/07/02

	#ifdef	SUPPORT_BQ24780S	// leox20150915
	if ((LearnModeSetCnt > 0) || (LearnModeClrCnt > 0)) {return;}
	#endif

	if (IsFlag1(POWER_STAT_CTRL1, ECO_Mode)) {return;}

	if (IsFlag1(CMOSReset_Status, ClearCMOS)) {return;}

	EnterIdleMode();
}


void EnterIdleMode(void)
{
	CAPSLED_on();			// Pull LED pins to low
	SCROLLLED_on();
	NUMLED_on();

	VCH1CTL = 0x1F;
	VCH2CTL = 0x1F;
	VCH3CTL = 0x1F;

	CLR_BIT(ADCCFG, 0);		// Disable ADC module
	SET_BIT(DACCTRL, 4);	// Power down DAC module

	#ifdef	 OEM_BATTERY	// leox_20110921
	//DISCHG_ON_off();
	#endif	//OEM_BATTERY

	ForceProcessLED();
	CheckTurnOffSBLANPowerSource();

	#if	ENTER_SLEEP_MODE_AC	// leox_20110921
	// Do not set ACIN to alternate pin
//	#else	// (original)
//	GPCRI6 = ALT;	// ACIN to alternate pin		// Keep INPUT
	#endif

	GPCRE4 = ALT;	// PWRSW to alternate pin

//	GPCRJ0 = ALT;	// MMB_INT# to alternate pin	// Keep INPUT

	//#if	ENTER_SLEEP_MODE_AC	// leox_20110921
	// Do not set wake-up enable for ACIN
    //	#else	// (original)
	//SET_BIT(WUENR2, 1);	// Wake-up enable WU21 for ACIN
	IsFlag1(POWER_STAT_CTRL, adapter_in) ? SET_BIT(WUEMR7, 6) : CLR_BIT(WUEMR7, 6);	// Wake-up edge WU30 for ACIN falling/rising
    WUESR7 = BIT6;      // Clear Sense Register
	//#endif

	//SET_BIT(WUENR2, 5);	// Wake-up enable WU25 for PWRSW
	SET_BIT(WUEMR2, 5);	// Wake-up edge WU25 for PWRSW falling
    WUESR2 = BIT5;      // Clear Sense Register

	#ifdef IT_7230MMB_SUPPORT
	//SET_BIT(WUENR2, 3);	// Wake-up enable WU23 for MMB_INT#
	SET_BIT(WUEMR14, 0);	// Wake-up edge WU23 for MMB_INT# falling
    WUESR14 = BIT0;         // Clear Sense Register
	#endif

    IsFlag1(ABAT_STATUS, abat_in) ? CLR_BIT(WUEMR14, 2) : SET_BIT(WUEMR14, 2);		// Wake-up edge WU31 for BATIN rising/falling
    WUESR14 = BIT2;      // Clear Sense Register

	WUENR3 = 0xFF;				// Wake-up enable WU30-37 for KB
    WUESR3 = 0xFF;              // Clear Sense Register

	// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function ++
	IERx_BackUp();
	// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function --

	IER0 = 0x00;
	IER1 = 0x00;
	IER2 = 0x00;
	IER3 = 0x00;

	#if	ENTER_SLEEP_MODE_AC	// leox_20110921
	// Do not enable interrupt for ACIN
//	#else	// (original)
    // ACIN
    CLR_BIT(IER9, 6);   // Disable interrupt register
	SET_BIT(IELMR9, 6); // Interrupt edge trigger
	ISR9 = BIT6;	    // Clear interrupt status register
    SET_BIT(IER9, 6);	// Disable interrupt register
	#endif

    // BATT_IN (aka BAT_PRS#)
    CLR_BIT(IER16, 2);   // Disable interrupt register
    SET_BIT(IELMR16, 2); // Interrupt edge trigger
    ISR16 = BIT2;        // Clear interrupt status register
    SET_BIT(IER16, 2);   // Disable interrupt register

    // PWRSW
	CLR_BIT(IER1, 6);   // Disable interrupt register
	SET_BIT(IELMR1, 6); // Interrupt edge trigger
    ISR1 = BIT6;	    // Clear interrupt status register
    SET_BIT(IER1, 6);	// Disable interrupt register

	#ifdef IT_7230MMB_SUPPORT
    // MMB_INT#
	CLR_BIT(IER16, 0);   // Disable interrupt register
	SET_BIT(IELMR16, 0); // Interrupt edge trigger
    ISR16 = BIT0;	    // Clear interrupt status register
    SET_BIT(IER16, 0);	// Disable interrupt register
	#endif

    // KB
    CLR_BIT(IER1, 5);   // Disable interrupt register
    SET_BIT(IELMR1, 5); // Interrupt edge trigger
    ISR1 = BIT5;	    // Clear interrupt status register
    SET_BIT(IER1, 5);	// Disable interrupt register

	#if	CIRFuncSupport	// leox_20110921
    // CIR
    CLR_BIT(IER1, 7);   // Disable interrupt register
    SET_BIT(IELMR1, 7); // Interrupt edge trigger
    ISR1 = BIT7;	    // Clear interrupt status register
    SET_BIT(IER1, 7);	// Disable interrupt register
	#endif

	//WUESR1 = 0xFF;			// Clear wake-up status
	//WUESR2 = 0xFF;
	//WUESR3 = 0xFF;
	//WUESR4 = 0xFF;

	EnableAllInterrupt();	// Enable global interrupt

	PLLCTRL = 0x01;			// 0x00:Doze mode, 0x01:Sleep mode
	_nop_();				// Reserved delay	// leox_20120203
	PCON = 0x02;			// 0x01:Idle mode, 0x02:Doze/Sleep mode
	_nop_();				// Repeat "nop" eight times immediately for internal bus turn-around
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();				// Entered sleep mode and waiting wake-up event...

	DisableAllInterrupt();	// Wake-up from sleep

	//WUESR1 = 0xFF;			// Clear wake-up status
	//WUESR2 = 0xFF;
	//WUESR3 = 0xFF;
	//WUESR4 = 0xFF;

//	GPCRJ0 = INPUT;			// MMB_INT# to input pin

	GPCRE4 = INPUT;			// PWRSW to input pin

	#if	ENTER_SLEEP_MODE_AC	// leox_20110921
	// Do nothing
//	#else	// (original)
//	GPCRI6 = INPUT;			// ACIN to input pin
	#endif
/*
    // ACIN
    WUESR7 = BIT6;      // Clear Sense Register

    // PWRSW
    WUESR2 = BIT5;      // Clear Sense Register

    //MMB_INT#
    WUESR14 = BIT0;     // Clear Sense Register

    // KB
	WUENR3 = 0x00;				// Wake-up Disable WU30-37 for KB
    WUESR3 = 0xFF;              // Clear Sense Register
*/

	#if	ENTER_SLEEP_MODE_AC	// leox_20110921
	// Do nothing
//	#else	// (original)
    // ACIN
    WUESR7 = BIT6;      // Clear Sense Register
	CLR_BIT(IER9, 6);	// Disable INT78 for ACIN
    ISR9 = BIT6;	    // Clear interrupt status register
	#endif

    // BATT_IN (aka BAT_PRS#)
    WUESR14 = BIT2;      // Clear Sense Register
    CLR_BIT(IER16, 2);   // Disable INT79 for BATT_IN
    ISR16 = BIT2;        // Clear interrupt status register

    // KB
	WUENR3 = 0x00;	    // Wake-up Disable WU30-37 for KB
    WUESR3 = 0xFF;      // Clear Sense Register
	CLR_BIT(IER1, 5);	// Disable INT13 for KB
    ISR1 = BIT5;	    // Clear interrupt status register

    // PWRSW
    WUESR2 = BIT5;      // Clear Sense Register
	CLR_BIT(IER1, 6);	// Disable INT14 for PWRSW
    ISR1 = BIT6;	    // Clear interrupt status register

	#ifdef IT_7230MMB_SUPPORT
    //MMB_INT#
    WUESR14 = BIT0;     // Clear Sense Register
	CLR_BIT(IER16, 0);	// Disable INT128 for MMB_INT#
    ISR16 = BIT0;	    // Clear interrupt status register
	#endif
	
    #if	CIRFuncSupport	// leox_20110921
    CLR_BIT(IER1, 7);	// Disable INT15 for CIR
    ISR1 = BIT7;	    // Clear interrupt status register
    #endif
	// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function ++
	IERx_ReLoad();
	// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function --

	SET_BIT(ADCCFG, 0);		// Enable ADC module
	Init_ADC();
	CLR_BIT(DACCTRL, 4);	// Power down DAC module

	#ifdef	OEM_BATTERY
	FreeDischarge();
	#endif	//OEM_BATTERY

	#ifdef	OEM_IPOD
	ClrFlag(USBCHARGFLAG, ChkUSBchgFunBeforeIdle);	// Initial ChkUSBchgFunBeforeIdle flag
	#endif	//OEM_IPOD
}


//------------------------------------------------------------------------------
// Update delayed event timer
//------------------------------------------------------------------------------
void DelayTimer(void)
{
	if (IsFlag1(POWER_FLAG, ACPI_OS) && (Q09CNT != 0))
	{
		Q09CNT--;
		if (Q09CNT == 0) {ECQEvent(Q09_SCI, SCIMode_Normal);}
	}
}


#ifdef	DISPON_EC_Support
void ChkDISPON_Signal(void)
{
	if (DISPON())
	{	// DISPON_on
		SetFlag(Device_STAT_CTRL, DISPON_Status);
	}
	else
	{	// DISPON_off
		ClrFlag(Device_STAT_CTRL, DISPON_Status);
		DAPWM_VADJ = 0;
	}
}
#endif	//DISPON_EC_Support


#ifdef	DLID_Function
void ChkDLIDbyLID(void)
{
	static BYTE DLID_Status = 0;
	if (IsFlag0(Factory_Test_Status, LID_Dis))
	{
		if (LIDSW_read())	// leo_20160420 add for cost down by EE Jackson wu
		{	// Pannel close
			//DLID_Low();
			ClrFlag(Device_STAT_CTRL2, Lid_open);
			DLIDCNT = 0;
		}
		else
		{	// Pannel open
			if (DLIDCNT != 5)
			{
				DLIDCNT++;
				if (DLIDCNT == 5) 
				{
					SetFlag(Device_STAT_CTRL2, Lid_open);
					//DLID_High();
				}
			}
		}
	}
	else
	{
		//DLID_High();			// leo_20160420 add for cost down by EE Jackson wu
		SetFlag(Device_STAT_CTRL2, Lid_open);
	}
	
	if ((IsFlag1(Device_STAT_CTRL2, Lid_open)) && (APU_BLON_read())) 
	{
		DLID_High();
	}
	else
	{
		DLID_Low();
	}
}

// leo_20150518 add panel blon need wait APU_BLON +++
void CheckAPU_BLON(void)
{
    static BYTE LCD_status = 0;
    if (APU_BLON_read())
    {
        if (LCD_status == 0)
        {
            LCD_BLON_On();
            LCD_status = 1;
        }
    }
    else
    {
        if (LCD_status == 1)
        {
            LCD_BLON_Off();
            LCD_status = 0;
        }
    }
}
// leo_20150518 add panel blon need wait APU_BLON ---
#endif	//DLID_Function


#ifdef	LAN_WRITE_PROTECT_Function
void CheckLAN_WP(void)
{
	if (IsFlag1(JUMPERSETTING, LANWP_Disable))
	{	// Lan write protect disable
		LAN_WP_off();
	}
	else
	{	// Lan write protect enable
		LAN_WP_on();
	}
}
#endif	//LAN_WRITE_PROTECT_Function


//------------------------------------------------------------------------------
// GetSBMBsignals4PowerSequence
//
// FromSBsignals00  EQU 0480h   ;(byte)                     ;Normal ;Cappella ;Montevina
// SLP_S3           EQU BIT0    ; SLP_S3# 1/0:Hi/Lo         ; V     ; V       ; V
// SLP_S4           EQU BIT1    ; SLP_S4# 1/0:Hi/Lo         ; V     ; V       ; V
// SLP_S5           EQU BIT2    ; SLP_S5# 1/0:Hi/Lo         ; X     ; V       ; V
// SLP_M            EQU BIT3    ; SLP_M# 1/0:Hi/Lo          ; X     ; V       ; V
// SUS_PWR_DN_ACK   EQU BIT4    ; SUS_PWR_DN_ACK 1/0:Hi/Lo  ; X     ; V       ; V
// SLP_LAN          EQU BIT5    ; SLP_LAN# 1/0:Hi/Lo        ; X     ; V       ; X
// WOL_EN           EQU BIT6    ; WOL_EN 1/0:Hi/Lo          ; X     ; X       ; V
// S4_STATE         EQU BIT7    ; S4_STATE# 1/0:Hi/Lo       ; X     ; X       ; V
//
// FromMBsignals00  EQU 0483h   ;(byte)                     ;Normal ;Cappella ;Montevina
// HWPG             EQU BIT0    ; HWPG 1/0:Hi/Lo            ; V     ; V       ; V
//------------------------------------------------------------------------------
void GetSBMBsignals4PowerSequence(void)
{
	if (IsFlag1(ToSBsignals00, SB_PowerSource))
	{
		BounceFromSBsignals00();
		BounceFromMBsignals00();
	}
}


void BounceFromSBsignals00(void)
{
	switch (FromSBsignals00_CNT)
	{
	case 0:
		FromSBsignals00_BK0 = ReadFromSBsignals00();
		FromSBsignals00_CNT++;
		CheckUpdateFromSBsignals00();
		break;

	case 1:
		FromSBsignals00_BK1 = ReadFromSBsignals00();
		#ifdef	OEM_Calpella
		FromSBsignals00_CNT = 0;
		#else	//OEM_Calpella
		FromSBsignals00_CNT++;
		#endif	//OEM_Calpella
		CheckUpdateFromSBsignals00();
		break;

	case 2:
		FromSBsignals00_BK2 = ReadFromSBsignals00();
		FromSBsignals00_CNT = 0;
		CheckUpdateFromSBsignals00();
		break;

	default:
		break;
	}
}


int ReadFromSBsignals00(void)
{
	int SBsignals00 = 0;

	if (SLP_S3_High()) {SetFlag(SBsignals00, SLP_S3);}
	if (SLP_S4_High()) {SetFlag(SBsignals00, SLP_S4);}
	if (SLP_S5_High()) {SetFlag(SBsignals00, SLP_S5);}
	if (SLP_M_High()) {SetFlag(SBsignals00, SLP_M);}
	if (SUS_PWR_DN_ACK_High()) {SetFlag(SBsignals00, SUS_PWR_DN_ACK);}
	if (SLP_LAN_High()) {SetFlag(SBsignals00, SLP_LAN);}
	if (WOL_EN_High()) {SetFlag(SBsignals00, WOL_EN);}
	if (S4_STATE_High()) {SetFlag(SBsignals00, S4_STATE);}

	return SBsignals00;
}


void CheckUpdateFromSBsignals00(void)
{
	#ifdef	OEM_Calpella
	if (FromSBsignals00_BK0 == FromSBsignals00_BK1)
	#else	//OEM_Calpella
	if ((FromSBsignals00_BK0 == FromSBsignals00_BK1) && (FromSBsignals00_BK1 == FromSBsignals00_BK2))
	#endif	//OEM_Calpella
	{
		FromSBsignals00 = FromSBsignals00_BK0;
	}
}


void BounceFromMBsignals00(void)
{
	switch (FromMBsignals00_CNT)
	{
	case 0:
		FromMBsignals00_BK0 = ReadFromMBsignals00();
		FromMBsignals00_CNT++;
		CheckUpdateFromMBsignals00();
		break;

	case 1:
		FromMBsignals00_BK1 = ReadFromMBsignals00();
		#ifdef	OEM_Calpella
		FromMBsignals00_CNT = 0;
		#else	//OEM_Calpella
		FromMBsignals00_CNT++;
		#endif	//OEM_Calpella
		CheckUpdateFromMBsignals00();
		break;

	case 2:
		FromMBsignals00_BK2 = ReadFromMBsignals00();
		FromMBsignals00_CNT = 0;
		CheckUpdateFromMBsignals00();
		break;

	default:
		break;
	}
}


int ReadFromMBsignals00(void)
{
	int MBsignals00 = 0;
	if (HWPG_Status_High()) {SetFlag(MBsignals00, HWPG);}
	return MBsignals00;
}


void CheckUpdateFromMBsignals00(void)
{
	#ifdef	OEM_Calpella
	if (FromMBsignals00_BK0 == FromMBsignals00_BK1)
	#else	//OEM_Calpella
	if ((FromMBsignals00_BK0 == FromMBsignals00_BK1) && (FromMBsignals00_BK1 == FromMBsignals00_BK2))
	#endif	//OEM_Calpella
	{
		FromMBsignals00 = FromMBsignals00_BK0;
	}
}


void ChkSBMBsignals4PowerSequence(void)
{
	if (IsFlag1(ToSBsignals00, SB_PowerSource))
	{
		//MonitorHWPG();
		ChkSBsignals4PowerSequence();	// Power on/off sequence
		MonitorHWPG();
	}
}


void MonitorHWPG(void)
{
	if (IsFlag0(POWER_FLAG, power_on)) {return;}	// Jump if not power on

	if (IsFlag1(POWER_FLAG, system_on))
	{	// Check HWPG drop down or not
		if (IsFlag0(FromMBsignals00, HWPG))
		{
			ShutDnCause = 0x05;
			ForceOffPower();
		}
		return;
	}

	if (IsFlag0(POWER_FLAG, wait_HWPG))
	{	// Delay a short time for SWPG
		PreparePWROK();
		return;
	}

	HWPG_TIMER--;
	if (HWPG_TIMER == 0)
	{
		ShutDnCause = 0x04;
		ForceOffPower();
		return;
	}

	if (IsFlag0(FromMBsignals00, HWPG)) {return;}

	ClrFlag(POWER_FLAG, wait_HWPG);
	HWPG_TIMER = T_SWPG;

	VRON_on();
	SetFlag(ToMBsignals00, VRON);
	DelayXms(10);
	PCHPWROK_on();

	Led_Data = 0x07;
	OEM_Write_Leds(Led_Data);

	SetFlag(SYS_MISC2, CrisisCheck);
}


void PreparePWROK(void)
{
	HWPG_TIMER--;
	if (HWPG_TIMER != 0) {return;}

	#ifdef	LogP80InECRamDxx
	P80LB_BRAM = 0xFF;
	LogP80();
	#endif	//LogP80InECRamDxx

	ConfigSysOnIO();

	#ifdef	 Multiple_TP	// ken add for touchpad compare
	DetectTPID_Ctrl = 0x01;	// Enable detect touchpad ID function
	#endif

	ECPWROK_on();
	SetFlag(ToSBsignals00, ECPWROK);

	InitSIOCnfg();	// Enable SuperIO devices
	DelayXms(100);

	OemPreparePWROK();
	DelayXms(10);

	InitialSysOnVariables();
	DNBSWON_off();

	Led_Data = 0x00;
	OEM_Write_Leds(Led_Data);

	#ifdef	OEM_MotionLed
	EnableMotionLedsFunction();
	#endif	//OEM_MotionLed

	ClrFlag(SYS_MISC2, InRestart);
	SYSTEMSTATE = 0x02;

	#if	OEM_POWER_LOSS	// leox_20110921
	B_PwrLoss |= 0xA8;	// Last power state: On
		#if SUPPORT_EFLASH_FUNC	// leox20160322
	eFlashSave(&B_PwrLoss, B_PwrLoss);
		#endif
	#endif
}


#ifdef	LogP80InECRamDxx
void LogP80(void)
{
	if (P80LB_BRAM != LastP80)
	{
		ClrFlag(SPCTRL1, P80LEN);
		LastP80 = P80LB_BRAM;
		BaseP80[NextP80Index] = P80LB_BRAM;
		if (++NextP80Index >= 0xFD) {NextP80Index = 0;}	// leox_20110922		// leo_20150820 add Fn + D P80 debug, 0xFE -> 0xFD
	}
	SetFlag(SPCTRL1, P80LEN);
}
#endif	//LogP80InECRamDxx


void ConfigSysOnIO(void)
{
	GA20_on();
	RCIN_off();
	#if	CHECK_FAN_FAULT		// David_20120613 add for fan error check
		#ifdef FanByDAorPWM	// leox20151104 Check fan fault
	DAPWM_VFAN = VFAN_DA_3V9;	// 3.9V fan full speed run
		#else
	DAPWM_VFAN = VFAN_PWM_35DB;	// Use 35db to turn on fan
		#endif
	Fan1_Fault_CNT = 76;	// Fan check timing for 750ms
	ClrFlag(Device_STAT_CTRL1, Fan1_Fault_On);
	#endif
	
	#if	CHECK_RTC_battery
	ClrFlag(Battery_Status, No_RTC);
	if (RTC_Read()) {SetFlag(Battery_Status, No_RTC);}
	#endif
}


void InitSIOCnfg(void)
{
	InitSio();
	#ifdef	LogP80InECRamDxx
	SetFlag(SPCTRL1, P80LEN);
	#endif	//LogP80InECRamDxx
}


void OemPreparePWROK(void)
{
	OemInitPWM();

	#ifdef	OEM_IPOD
	PowerOnInitUSBChargePort();
	#endif	//OEM_IPOD
	DelayXms(10);

	OemInitialSysOnVariables();
}


void OemInitPWM(void)
{
	PCFSR = 0x8F;	// Set C0CPRS, C4CPRS, C6CPRS, C7CPRS base PWM clock are use 9.2MHz
	CTR = 200;		// Set cycle time maxmum is 200
	C0CPRS = 227;	// Set PWMC0 clock is 200Hz (9.2M/(201*228))
	C4CPRS = 151;	// Set PWMC4 clock is 301Hz (9.2M/(201*152))
	C4MCPRS = 0;
	C6CPRS = 1;		// Set PWMC6 clock is 22.885KHz (9.2M/(201*2))
	C6MCPRS = 0;
	C7CPRS = 0;		// Set PWMC7 clock is 45.771KHz (9.2M/(201*1))
	C7MCPRS = 0;
	PCSSGL = 0;		// Set PWM3/PWM2/PWM1/PWM0 use PWMC0
	PCSSGH = 0x20;	// Set PWM7/PWM5/PWM4 use PWMC0, PWM6 use PWMC6
	ZTIER = 2;		// Enable PWM clock
}


void OemInitialSysOnVariables(void)
{
	GetJumpper();
	Volume_CNT = 3;
	#ifdef	UpDate_Brighness_Support
	UpDate_Brighness();
	#endif	//UpDate_Brighness_Support
}


void GetJumpper(void)
{
	ClrFlag(JUMPERSETTING, KBD_ID0);
	if (KBD_ID0_read()) {SetFlag(JUMPERSETTING, KBD_ID0);}

	ClrFlag(JUMPERSETTING, KBD_ID1);
	if (KBD_ID1_read()) {SetFlag(JUMPERSETTING, KBD_ID1);}

	ClrFlag(JUMPERSETTING, KBD_ID);
	if (KBD_ID_read()) {SetFlag(JUMPERSETTING, KBD_ID);}

	ClrFlag(JUMPERSETTING, Model_ID);
	if (EC_BOARD_ID_read()) {SetFlag(JUMPERSETTING, Model_ID);}

	ClrFlag(JUMPERSETTING, Watt_ID);
	if (EC_BOARD_ID2_read()) {SetFlag(JUMPERSETTING, Watt_ID);}
}


#ifdef	UpDate_Brighness_Support
const unsigned char code BrightnessTBL[] =
{
	0x20,0x2F,0x3E,0x4E,0x5D,0x6C,0x7C,0x8B,0x9A,0xAA,0xB9,0xC8
};
void UpDate_Brighness(void)
{
	if (IsFlag0(POWER_FLAG, ACPI_OS))
	{
		if (BrightnessLevel >= MaxLevel)
		{
			BrightnessLevel = MaxLevel;
		}
		if (BrightnessLevel <= 0)
		{
			BrightnessLevel = 0;
		}
		BrightnessValue = BrightnessTBL[BrightnessLevel];
	}
	#ifdef	LCDPannelByDAorPWM
	#else	//LCDPannelByDAorPWM
	if (BrightnessValue >= CTR) {BrightnessValue = CTR;}
	#endif	//LCDPannelByDAorPWM

	#ifdef	DISPON_EC_Support
	if (IsFlag0(Device_STAT_CTRL, DISPON_Status)) {BrightnessValue = 0;}
	#endif	//DISPON_EC_Support

	DAPWM_VADJ = BrightnessValue;
}
#endif	//UpDate_Brighness_Support


void InitialSysOnVariables(void)
{
	GA20_on();
	SetFlag(POWER_FLAG, system_on);		// Set flag: SWPG issued
	SetFlag(SYS_FLAG, enable_SMI);		// Power on default: Enable SMI
	SetFlag(POWER_FLAG2, func_LED);
	ClrFlag(MISC_FLAG, EnTempDebug);
	//David modify for fan change to DAC 2014/05/21
	//David modify for fan default full on for SMT 2014/04/28
	ClrFlag(MISC_FLAG, FAN_TEST);
	//David modify for fan default full on for SMT 2014/04/28
	//David modify for fan change to DAC 2014/05/21
	BANKDEFINE = 0x60;
	ClrFlag(POWER_FLAG, ACPI_OS);
	ClrFlag(KETBOARD_STAT_CTRL, OS_Type);
}


void ChkSBsignals4PowerSequence(void)
{
	if (IsFlag0(POWER_FLAG, power_on))
	{
		ChkTurnOnSystem();
		return;
	}

	#ifdef	DeepS3_SUPPORT	// 20120104 Chris add for support Deep S3
	if (SLP_SUS_Low())
	{
		S0toDPS3();
		return;
	}
	#endif

	if (IsFlag1(FromSBsignals00, SLP_S3))
	{
		return;
	}

	if (IsFlag1(FromSBsignals00, SLP_S4))
	{
		S0toS3();
		return;
	}

	S0toS4S5();
}


void ChkTurnOnSystem(void)
{
	if ((IsFlag0(FromSBsignals00, SLP_S3)) && (IsFlag1(FromSBsignals00, SLP_S4)))
	{
		#ifdef	DeepS3_SUPPORT	// 20120104 Chris add for support Deep S3
		if (SLP_SUS_Low()) {S3toDPS3();}
		#endif
		return;
	}

	if ((IsFlag0(FromSBsignals00, SLP_S3)) && (IsFlag0(FromSBsignals00, SLP_S4)) && (IsFlag0(POWER_FLAG, enter_SUS)))
	{
		return;
	}

	if ((IsFlag1(FromSBsignals00, SLP_S3)) && (IsFlag0(FromSBsignals00, SLP_S4)))
	{
		return;
	}

	if ((IsFlag1(FromSBsignals00, SLP_S3)) && (IsFlag1(FromSBsignals00, SLP_S4)) && (IsFlag0(POWER_FLAG, enter_SUS)))
	{
		S4S5toS0();
		return;
	}

	if ((IsFlag1(FromSBsignals00, SLP_S3)) && (IsFlag1(FromSBsignals00, SLP_S4)) && (IsFlag1(POWER_FLAG, enter_SUS)))
	{
		S3toS0();
		return;
	}

	if ((IsFlag0(FromSBsignals00, SLP_S3)) && (IsFlag0(FromSBsignals00, SLP_S4)) && (IsFlag1(POWER_FLAG, enter_SUS)))
	{
		S3toS4S5();
		return;
	}
}


void S0toS3(void)
{
	#ifdef	OEM_IPOD
	InitUSBChargePortCNT = 0;	// Disable USB wake function when USB charge enable and DC enter S3
	#endif	//OEM_IPOD

	SYSTEMSTATE = 0x03;
	CommonStateChange();
	SetFlag(POWER_FLAG, enter_SUS);

	S0S3toS3S4S5OffPower();
	CheckUSB_Power_Need_CutOff();
	ECOSleepCheck();
	PollingCriticalWake();
	if (IsFlag0(POWER_STAT_CTRL, WakeOnLan)) {TurnOffLanPower();}
}


#ifdef	DeepS3_SUPPORT	// 20120104 Chris add for support Deep S3
void S0toDPS3(void)
{
	#ifdef	OEM_IPOD
	InitUSBChargePortCNT = 0;	// Disable usb wake function when usb charge enable and DC enter S3
	#endif	//OEM_IPOD

	SYSTEMSTATE = 0x13;	// 20120110 chris modify for enter Deep S3 status
	CommonStateChange();
	SetFlag(POWER_FLAG, Deep_S3);

	S0S3toDPS3S4S5OffPower();
	SYSTEMSTATE = 0x13;	// leox_20120111 S0S3toDPS3S4S5OffPower() will call TurnOffSBPowerSource()
}


void S3toDPS3(void)
{
	SYSTEMSTATE = 0x13;	// 20120110 chris modify for enter Deep S3 status
	CommonStateChange();

	ClrFlag(POWER_FLAG, enter_SUS);
	SetFlag(POWER_FLAG, Deep_S3);

	ClrFlag(ABAT_MISC, force_discA + abat_inUSE + abat_next);
	ClrFlag(BBAT_MISC, force_discB + bbat_inUSE + bbat_next);
	ClrFlag(ABAT_STATUS, abat_dischg);
	ClrFlag(BBAT_STATUS, bbat_dischg);
	ClrFlag(BAT_MISC1, STOP_charge);
	ClrFlag(ABAT_MISC, abat_AutoLearn);
	ClrFlag(BBAT_MISC, bbat_AutoLearn);

	SetFlag(POWER_FLAG2, func_LED);
	ClrFlag(KETBOARD_STAT_CTRL, LCtrlFn_RCtrl);

	PWRLED0_off();
	PWRLED1_off();

//	SUSON_off();
//	ClrFlag(ToMBsignals00, SUSON);
	DelayXms(1);

	DisableUSBPort1();
	DisableUSBPort2();

	ClrFlag(POWER_FLAG, PwrOffRVCCOff);
	RSMRST_on();	// 20120110 chris modify for enter Deep S3 turn off RSMRST	// leox20150721 Fix RSMRST#
	DelayXms(RVcc2RSMRST_DS3);
	STBON_off();
	ClrFlag(ToSBsignals00, SB_PowerSource);

	ResetSusBSusC();
}


void DPS3toS3(void)
{
	STBON_on();
	DelayXms(RVcc2RSMRST_DS3);
	RSMRST_off();	// 20120110 chris modify for enter Deep S3 turn off RSMRST	// leox20150721 Fix RSMRST#
	SetFlag(ToSBsignals00, SB_PowerSource);
	ClrFlag(POWER_FLAG, Deep_S3);
	SetFlag(POWER_FLAG, enter_SUS);	// leox_20120111
	SYSTEMSTATE = 0x03;				// leox_20120111
}
#endif	// DeepS3_SUPPORT


void CheckUSB_Power_Need_CutOff(void)
{
	if (IsFlag1(USB_CutOffpower, CutOffUSBP)) {DisableUSBPort1();}
	if (IsFlag1(USB_CutOffpower, CutOffUSBP2)) {DisableUSBPort2();}
}


void S0toS4S5(void)
{
	ShutDnCause = 0x08;
	S0S3toS4S5();
}


void S4S5toS0(void)
{
	#if	RESET_EC_LATER	// leox_20111206
	CheckResetLater();
	#endif
	CommonStateChange();
	//TurnOnLanPower(); // leo_20150430 remove lan turn on delay
	DelayXms(5);
	SUSON_on();
	DelayXms(2);
	SUSON_EC_on();		// leo_20160425 add for cost down by EE Jackson wu
	SetFlag(ToMBsignals00, SUSON);
	S3S4S5toS0();
	#ifdef	OEM_S4S5WDG
	WatchDOGS4S5_CNT = 10;
	#endif	//OEM_S4S5WDG
	ClrFlag(POWER_STAT_CTRL1, lowpower_CHG);
	ECOSleepCheck();
}


void S3toS0(void)
{
	SYSTEMSTATE = 0x31;
	CommonStateChange();
	TurnOnLanPower();
	S3S4S5toS0();
	ClrFlag(POWER_STAT_CTRL1, lowpower_CHG);
	ECOSleepCheck();
}


void S3toS4S5(void)
{
	ShutDnCause = 0x0B;
	S0S3toS4S5();
}


void S0S3toS4S5(void)
{
	LogShutdownCause();
	SYSTEMSTATE = 0x04;
	CommonStateChange();
	S0S3toS3S4S5OffPower();
	S0S3toS4S5OffPower();
	ECOSleepCheck();
	CheckTurnOffSBLANPowerSource();
	//David modify for battery shutdown mode 2014/07/02
	if ((IsFlag1(ABAT_MISC2, Shipmode)) && (IsFlag0(POWER_STAT_CTRL, adapter_in)))
		LED_FLASH_CNT = 0x0001;
	//David modify for battery shutdown mode 2014/07/02
}


void S3S4S5toS0(void)
{
	Hook_Setup_Scanner_Pntr();	// leo_20160809 FH9U KB detect wrong ID workarround.
	#if	CIR_NEC_IMPROVED	// Ken_20120605
	CIR_NEC_Rst();
	#endif
	CLR_MASK(KBHISR, SYSF);
	Ccb42_SYS_FLAG = 0;
	DDR3_CORL_EC_High();
	ClrFlag(USB_CutOffpower, CutOffUSBP + CutOffUSBP2);
	#ifdef	OEM_IPOD
	ClrFlag(USB_CutOffpower, CutOffUSBP_IPOD);
	#endif	//OEM_IPOD
	EnableUSBPort1();
	EnableUSBPort2();
	ClrFlag(MISC_FLAG, PCBeep_Status);
	InitQ20();
	#ifdef	OEM_FAN
	LoadFanCtrlDefault();
	#endif	//OEM_FAN
	SetFlag(POWER_FLAG, power_on + wait_HWPG);
	SetFlag(POWER_STAT_CTRL1, S5_Reboot);
	ClrFlag(POWER_FLAG, enter_SUS);
	ClrFlag(POWER_FLAG2, Quick_S3);
	ClrFlag(POWER_STAT_CTRL, WakeOnLan + WakeOnRTC);
	ClrFlag(POWER_STAT_CTRL1, WakeOnWLan);
	ClrFlag(KETBOARD_STAT_CTRL, WakeOnUMTS);
	ClrFlag(CMOSReset_Status, WaitSUSBSUSC);
	ClrFlag(CMOSReset_Status, ClearCMOS);
	Converter_Config_Status = 0;
	HWPG_TIMER = T_HWPG;
	ForceProcessLED();
	EnablePS2Interrupt();
	#ifdef	OEM_IPOD
	S3S4S5toS0_OEM_IPOD();
	#endif	//OEM_IPOD
	#ifdef	OEM_DIMM
	S3S4S5toS0_OEM_DIMM();
	#endif	//OEM_DIMM
	VOLMUTE_on();
	SATA_LED_BBO_En();
	DelayXms(3);
	MAINON_on();
    #if Support_IT8987  // Leo_20141111 8987 RSTS no support bit7-6 = 0 1
    SET_MASK(RSTS, BIT6);
    #endif
	WLAN_ON_on();
	WLSW_on();
    LAN_WOL_EN_EC_on();
	//David add for HW request 2014/07/09
//	Delay500us();       // leo_20150430 add for power sequence.
	DelayXms(5);		// leox20150605 Refer to "FH9C_MB_B_20150605A.pdf"
	MAINON_1_on();
	DelayXms(4);		// leox20150605 Refer to "FH9C_MB_B_20150605A.pdf"
	MAINON_2_on();
	//David add for HW request 2014/07/09
	// David_20140408 modify for NVdia and ATI different function
	#ifdef	NVdia_Support
	DelayXms(1);
	DGPU_VRON_on();
	DelayXms(2);
	DGPU_VRON2_on();
	#endif	//NVdia_Support
	#ifdef	ATI_Support
	GPU_ON_EC_on();
	#endif
	// David_20140408 modify for NVdia and ATI different function
	SetFlag(ToMBsignals00, MAINON);
	ThreeG_En_CNT = T_MAINON_3G;
	DelayXms(T_MAINON_VRON);
	#ifdef	OEM_TP
	SWTouchpad_Enable_DisableFunOn();
	Set_Touchpad_Enable();
	#endif	//OEM_TP
	#ifdef	PECI_Support				// ben_20131120
	SET_MASK(HOCTLR, PECIHEN);	// After CPU VCC on, set PECI Host Enable by EC
	#endif	//PECI_Support
	#ifdef SUPPORT_BQ24780S	// leox20150824 Power on with DC mode
	if (IsFlag0(BAT_MISC3, BQ24780S_Init_OK)) {BQ24780S_Init();}
	#endif
	ENBEEP_on();
}


void CopyRAM2OEMBIOSaccess(void)
{
	if (IsFlag0(ABAT_MISC2, Bat_Fcc_test)) {wSwapHi_Low_Byte_for_copy(&DESIGN_FULL_CAPACITY, &BIOS_DESIGN_FULL_CAPACITY);}
	BIOS_ACHARGE_STATUS = ACHARGE_STATUS;
	BIOS_ABAT_MISC = ABAT_MISC;
	wSwapHi_Low_Byte_for_copy(&DESIGN_CAPACITY, &BIOS_DESIGN_CAPACITY);
	BIOS_ABAT_STATUS = ABAT_STATUS;
	BIOS_ABAT_CAP = ABAT_CAP;
	wSwapHi_Low_Byte_for_copy(&BateryTempSBMus, &BIOS_BateryTempSBMus);
	wSwapHi_Low_Byte_for_copy(&DESIGN_VOLTAGE, &BIOS_DESIGN_VOLTAGE);
	wSwapHi_Low_Byte_for_copy(&BT1I, &BT1I_BIOS);
	wSwapHi_Low_Byte_for_copy(&BT1C, &BT1C_BIOS);
	wSwapHi_Low_Byte_for_copy(&BT1V, &BT1V_BIOS);
	wSwapHi_Low_Byte_for_copy(&BT2I, &BT2I_BIOS);
	wSwapHi_Low_Byte_for_copy(&BT2C, &BT2C_BIOS);
	wSwapHi_Low_Byte_for_copy(&BT2V, &BT2V_BIOS);
	//David add for read battery cycle count 2014/06/11
	wSwapHi_Low_Byte_for_copy(&BTCyCle, &BIOS_BTCyCle);
	//David add for BIOS read battery cell number 2014/06/18
	BatteryCellNum = ABAT_CELLS;
	//David add for BIOS read battery cell number 2014/06/18
	//David add for read battery cycle count 2014/06/11
}


#ifdef	Setting_MAX8742ETI_Support
void Setting_MAX8742ETI(void)
{
	#ifdef	OEM_BATTERY
	if (IsFlag0(ABAT_STATUS, abat_in))
	{
		BATT_TYPE_Li4S();
		return;
	}

	if ((IsFlag1(ACHARGE_STATUS, abat_charge)) && (ABAT_MODEL != 0))
	{
		if ((ABAT_CELLS & 0xF0) == 0x30) {BATT_TYPE_Li3S();}	// 3 series
		if ((ABAT_CELLS & 0xF0) == 0x40) {BATT_TYPE_Li4S();}	// 4 series
	}
	#endif	//OEM_BATTERY
}
#endif	//Setting_MAX8742ETI_Support


void SendDummySCI(void)
{
	if (DummySCI_CNT != 0)
	{
		SCI_on();
		DelayXms(1);
		SCI_off();
		DummySCI_CNT--;
	}
}


void PollAVFUNCTIONLED(void)
{
	CheckWLBTStatus();
	ChkECOLED();
}


void CheckWLBTStatus(void)
{
	if (IsFlag0(POWER_FLAG, power_on))
		return;
	if (IsFlag1(POWER_FLAG2, func_LED))
	{
		if (IsFlag0(Device_Setup_Ctrl, Lan_Setup_Status))
		{
			ClrFlag(Device_STAT_CTRL, Lan_Status);
			WLSW_off();				// WL Disable
			#ifndef	Support_Wake_On_WLan
			WLAN_LED_off();			// WL LED off
			#endif
		}
		else
		{
			if (IsFlag1(Device_STAT_CTRL, Lan_Status))
			{
				WLSW_on();			// WL enable
				#ifndef	Support_Wake_On_WLan
				WLAN_LED_on();		// WL LED on
				#endif
			}
			else
			{
				/*if (IsFlag0(KETBOARD_STAT_CTRL, OS_Type))
					WLSW_off();		// WL Disable
				else
					WLSW_on();		// WL Enable*/
				WLSW_on();		// WL Enable
				#ifndef	Support_Wake_On_WLan
				WLAN_LED_off();		// WL LED off
				#endif
			}
		}

		if (IsFlag0(Device_Setup_Ctrl, BT_Setup_Status))
		{
			ClrFlag(Device_STAT_CTRL, BT_Status);
			BTON_EC_off();			// Bluetooth Disable
		}
		else
		{
			if (IsFlag1(Device_STAT_CTRL, BT_Status))
			{
				BTON_EC_on();		// Bluetooth Enable
			}
			else
			{
				if (IsFlag0(KETBOARD_STAT_CTRL, OS_Type))
					BTON_EC_off();	// Bluetooth Disable
				else
					BTON_EC_on();	// Bluetooth Enable
			}
		}

		if (IsFlag1(Device_STAT_CTRL1, CCD_Kill_En))
		{
			if (IsFlag1(Device_STAT_CTRL1, CCD_Status))
			{
				CCD_on();		// CCD Power on
			}
			else
			{
				CCD_off();		// CCD Power off
			}
		}
		else
		{
			CCD_off();			// CCD Power off
		}
	}
}

void Check3GStatus(void)
{
	if (IsFlag1(ToMBsignals00, MAINON))
	{
		if (IsFlag1(Device_STAT_CTRL, Lan_Status) && (ThreeG_En_CNT == 0))
		{
			SetFlag(Device_STAT_CTRL1, ThreeG_Status);
			ThreeG_on();	// 3G Enable
		}
		else
		{
			ClrFlag(Device_STAT_CTRL1, ThreeG_Status);
			ThreeG_off();	// 3G Disable
		}
		if (ThreeG_En_CNT != 0)
		{
			ThreeG_En_CNT--;
		}
	}
	else
	{
		ClrFlag(Device_STAT_CTRL1, ThreeG_Status);
		ThreeG_off();		// 3G Disable
	}
}

void ChkECOLED(void)
{
	if (IsFlag1(POWER_FLAG2, func_LED))
	{
		if (IsFlag1(Device_STAT_CTRL, ECO_LED_ON_LightBLUE))
		{
			ECO_LED0_on();	// Turn on ECO LED GREEN
			ECO_LED1_on();	// Turn on ECO LED BLUE
			return;
		}

		if (IsFlag1(Device_STAT_CTRL, ECO_LED_ON_BLUE))
		{
			ECO_LED0_off();	// Turn off ECO LED GREEN
			ECO_LED1_on();	// Turn on ECO LED BLUE
			return;
		}

		if (IsFlag1(Device_STAT_CTRL, ECO_LED_ON_GREEN))
		{
			ECO_LED0_on();	// Turn on ECO LED GREEN
			ECO_LED1_off();	// Turn off ECO LED BLUE
			return;
		}

		ECO_LED0_off();		// Turn off ECO LED GREEN
		ECO_LED1_off();		// Turn off ECO LED BLUE
	}
}


void Check_Volume_CNT(void)
{
	if (Volume_CNT != 0)
	{
		Volume_CNT--;
		if (Volume_CNT == 0) {VOLMUTE_off();}
	}
}


void CopyECRAM2BRAM(void)
{
	if (ChkEcRamValid != 0x55AA) {return;}	// leox20160317 Check EC RAM
	#if SUPPORT_EFLASH_FUNC	// leox20150703 Access eFlash	// leox20160322
	if (USBCHARGEDISCHARG_BRM != USBCHARGEDISCHARG) {eFlashSave(&USBCHARGEDISCHARG_BRM, USBCHARGEDISCHARG);}	// OEM_IPOD
	if (MMB_WakeUp_S3_BRM != MMB_WakeUp_S3) {eFlashSave(&MMB_WakeUp_S3_BRM, MMB_WakeUp_S3);}					// IT_7230MMB_SUPPORT
	if (MMB_WakeUp_S4_BRM != MMB_WakeUp_S4) {eFlashSave(&MMB_WakeUp_S4_BRM, MMB_WakeUp_S4);}
	if (MMB_WakeUp_S5_BRM != MMB_WakeUp_S5) {eFlashSave(&MMB_WakeUp_S5_BRM, MMB_WakeUp_S5);}
	if ((POWER_STAT_CTRL1_BRM & ECO_Mode) != (POWER_STAT_CTRL1 & ECO_Mode)) {eFlashSave(&POWER_STAT_CTRL1_BRM, POWER_STAT_CTRL1);}
	#endif
	#ifdef	OEM_IPOD
	USBCHARGEDISCHARG_BRM = USBCHARGEDISCHARG;
//	USBChargeFunctionS0();	// leox20150914 For USB charger SLG55544 to set SDP/CDP in S0 when BIOS set	// leox20150918 Move to Service_T1SEC14()
	#endif	//OEM_IPOD
	//David add for G3 keep status 2014/07/24	// IT_7230MMB_SUPPORT
	MMB_WakeUp_S3_BRM = MMB_WakeUp_S3;
	MMB_WakeUp_S4_BRM = MMB_WakeUp_S4;
	MMB_WakeUp_S5_BRM = MMB_WakeUp_S5;
	POWER_STAT_CTRL1_BRM = (POWER_STAT_CTRL1 & ECO_Mode);
	//David add for G3 keep status 2014/07/24
}


#ifdef	BatteryOnlyXXPThrollFUN
void ChkBatteryOnlyXXPThroll(void)
{
	if ((IsFlag0(POWER_FLAG, ACPI_OS)) || (IsFlag0(ABAT_STATUS, abat_in)) ||
		(IsFlag1(ACHARGE_STATUS, abat_charge)) || (BIOS_ABAT_CAP >= 21))
	{
		InitChkBatteryOnlyXXPThroll();
	}
	else
	{
		if (IsFlag0(BatteryOnlyXXPercent, Q40Q41_SCI))
		{
			SetFlag(BatteryOnlyXXPercent, Q40Q41_SCI);
			ECQEvent(Q40_SCI, SCIMode_Normal);
		}
	}
}


void InitChkBatteryOnlyXXPThroll(void)
{
	if (IsFlag1(BatteryOnlyXXPercent, Q40Q41_SCI))
	{
		ClrFlag(BatteryOnlyXXPercent, Q40Q41_SCI);
		ECQEvent(Q41_SCI, SCIMode_Normal);
	}
}
#endif	//BatteryOnlyXXPThrollFUN


void ForceOffPower(void)
{
	LogShutdownCause();
	CommonStateChange();
	S0S3toS3S4S5OffPower();
	S0S3toS4S5OffPower();
	ECOSleepCheck();
//	ClrFlag(Device_STAT_CTRL, Lan_Status);
//	ClrFlag(Device_STAT_CTRL, BT_Status);
	ClrFlag(POWER_STAT_CTRL, WakeOnLan);
	ClrFlag(POWER_STAT_CTRL, WakeOnRTC);
	ClrFlag(POWER_STAT_CTRL1, WakeOnWLan);
	ClrFlag(KETBOARD_STAT_CTRL, WakeOnUMTS);
	ClrFlag(POWER_STAT_CTRL1, S5_Reboot);
	USBCHARGEDISCHARG_BRM = 0;
	#if SUPPORT_EFLASH_FUNC	// leox20150703 Access eFlash
	eFlashSave(&USBCHARGEDISCHARG_BRM, USBCHARGEDISCHARG_BRM);	// leox20160322
	#endif
	CheckTurnOffSBLANPowerSource();
}


void LogShutdownCause(void)
{
	BIOS_ShutDnCause = ShutDnCause;		// leox20150922
	#ifdef	LogShutdownCauseInECRamCxx	// leox_20130913 Move switch define into function
	LastShutdownCause = ShutDnCause;
	BaseShutdownCause[NextShutdownCauseIndex] = ShutDnCause;
	if (++NextShutdownCauseIndex >= 0xFE) {NextShutdownCauseIndex = 0;}	// leox_20110922
	#endif	//LogShutdownCauseInECRamCxx
	#if	LogShutdownCauseInBRAM			// leox_20111209	// leox_20130913
	B_SdCause[B_SdCauseIdx] = ShutDnCause;
	if (++B_SdCauseIdx >= (SdCauseBRAMSize - 1)) {B_SdCauseIdx = 0;}
	#endif	//LogShutdownCauseInBRAM
}

void CommonStateChange(void)
{
	#ifdef	OEM_Calpella
	CommonStateChange_Calpella();
	#endif	//OEM_Calpella
	PCH_RemoteErrorCnt = 0;
	PECI_RemoteErrorCnt = 0;
	ClrFlag(MISC_FLAG, PCH_Sensor_SMB_error);
	ClrFlag(MISC_FLAG, PECI_Sensor_SMB_error);
//	SetFlag(KETBOARD_STAT_CTRL, NotReadPECI);
	CPU_RemoteErrorCnt = 0;
	VGA_RemoteErrorCnt = 0;

	ClrFlag(SYS_FLAG, CPU_Sensor_SMB_error);
	ClrFlag(SYS_FLAG, VGA_Sensor_SMB_error);

	ClrFlag(POWER_FLAG2, DisQevent);	// Enabled Q event
	ClrFlag(KETBOARD_STAT_CTRL, SKIPMATRIX);
	Q09CNT = 0;

	#ifdef	OEM_BATTERY
		#ifdef	NECP_BattRefresh_Support
	NECBattRefreshPSTS = 3;
	InitBattToNormalCondition();
	ChkNECBattRefreshPSTS();
		#endif	//NECP_BattRefresh_Support
	#endif	//OEM_BATTERY

	#if	CIRFuncSupport	// leox_20110916
	Init_CIR();
	#endif

	VOLMUTE_on();
	GPLED_off();
}


void DCMODE(void)
{
	DISCHG_ON_on();
	ClrFlag(SYS_MISC2, AcMode);
	ClrFlag(POWER_STAT_CTRL, SysPwrSourceAC);
	#ifdef	SUPPORT_BQ24780S	// leox20150607
	LearnModeSetCnt = 4;		// leox20150915
	if (IsFlag0(ABAT_MISC2, Shipmode)) {SetChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_EN_LEARN);}	// Enable LEARN mode
	#endif
}


void ACMODE(void)
{
	DISCHG_ON_off();
	SetFlag(SYS_MISC2, AcMode);
	SetFlag(POWER_STAT_CTRL, SysPwrSourceAC);
	#ifdef	SUPPORT_BQ24780S	// leox20150607
	LearnModeClrCnt = 4;		// leox20150915
	if (IsFlag0(ABAT_MISC2, Shipmode)) {ClrChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_EN_LEARN);}	// Disable LEARN mode
	#endif
}


void S0S3toS3S4S5OffPower(void)
{
	DDR3_CORL_EC_Low();

	#ifdef	OEM_DIMM
	SetFlag(POWER_STAT_CTRL, DIMM_OFF_Flag);
	#endif	//OEM_DIMM

	SetFlag(MISC_FLAG, PCBeep_Status);
	ClrFlag(Device_STAT_CTRL, ECO_LED_ON_BLUE + ECO_LED_ON_GREEN + ECO_LED_ON_LightBLUE);
	ClrFlag(KETBOARD_STAT_CTRL, CRISIS_ON);
	ClrFlag(POWER_FLAG, wait_HWPG + system_on + power_on);
	ClrFlag(SYS_MISC2, postpone_SCI + postpone_SMI);
	ClrFlag(POWER_FLAG2, No_Dimm);
	HWPG_TIMER = 0;
	ClrFlag(POWER_FLAG, ACPI_OS);
	ClrFlag(KETBOARD_STAT_CTRL, OS_Type);
	DisablePS2Interrupt();
	InitQ20();

	// leox20150731 Disable PROCHOT when enter S3/S4/S5 +++
	ClrFlag(FanControl_Mode00, Thrott_CPU_hot);	// Support_Intel_CPU
	ClrFlag(Device_STAT_CTRL1, Thrott_3cell);	// Battery_3Cell_Throttling
	#ifdef OEM_VICMV
	ClrFlag(VICMVHiFlag, Q42Q43_SCI);			// OEM_VICMV_PROCHOT
	#endif // OEM_VICMV
	ClrFlag(BAT_MISC3, DCHG_Throttling);		// BAT_DCHG_PROTECT				// leox20150826
	h_prochot_disable();
	// leox20150731 Disable PROCHOT when enter S3/S4/S5 ---

	Led_Data = 0x00;
	OEM_Write_Leds(Led_Data);

	DAPWM_VADJ = 0;
	#ifdef	OEM_FAN			// leox_20110916 FanByDAorPWM -> OEM_FAN
	DAPWM_VFAN = 0;
		#if	Support_FAN2	// leox_20120315
	DAPWM_VFAN2 = 0;
		#endif
	#endif	//OEM_FAN

	#ifdef	OEM_IPOD
	EnableUSBCHARGPORTFun();
	#endif	//OEM_IPOD

//	ClrFlag(Device_STAT_CTRL, Lan_Status);	// Default set WL LED off
//	ClrFlag(Device_STAT_CTRL, BT_Status);	// Default set Bluetooth off

	#ifdef	OEM_BATTERY
	TurnOffADISCHG();
	TurnOffBDISCHG();
	#endif	//OEM_BATTERY

	#ifdef	OEM_IPOD
	CheckUSBchargingEnDis();
	#endif	//OEM_IPOD

	ENBEEP_off();		// ENBEEP off
	RCIN_on();
	GA20_off();
	WLAN_LED_off();	// WL LED off

	ECPWROK_off();
	ClrFlag(ToSBsignals00, ECPWROK);

	PCHPWROK_off();
//	DelayXms(6);	// leox20150611 Disabled for Intel SKL-H of FH9C
	VRON_off();
	ClrFlag(ToMBsignals00, VRON);

	#ifdef	PECI_Support		// ben_20131120
	CLR_MASK(HOCTLR, PECIHEN);	// After CPU VCC off, clear PECI Host Enable by EC
	#endif	//PECI_Support

	// David_20140408 modify for NVdia and ATI different function
	#ifdef	NVdia_Support
	DGPU_VRON2_off();
	DGPU_VRON_off();
	#endif	//NVdia_Support
	#ifdef	ATI_Support
	GPU_ON_EC_off();
	#endif
	// David_20140408 modify for NVdia and ATI different function
	//David add for HW request 2014/07/09
	MAINON_2_off();
	MAINON_1_off();
	#ifdef	Support_Wake_On_WLan
	if (IsFlag0(POWER_STAT_CTRL1, WakeOnWLan))
		WLSW_off();
	else if(IsFlag0(Device_STAT_CTRL,Lan_Status))
		WLSW_off();
	else if((IsFlag0(POWER_FLAG, enter_SUS)) && (IsFlag0(POWER_STAT_CTRL, enter_S4)))
		WLSW_off();
	#else
	WLSW_off();
	#endif
//	DelayXms(25);	// leox20150612 Disabled for Intel SKL-H of FH9C
	//David add for HW request 2014/07/09
	MAINON_off();
	SATA_LED_BBO_Dis_Off();
	#if Support_IT8987  // Leo_20141111 8987 RSTS no support bit7-6 = 0 1
	CLR_MASK(RSTS, BIT6);
	#endif
	ClrFlag(ToMBsignals00, MAINON);
	#ifdef SUPPORT_BQ24780S	// leox20150824 System enter S3/S4/S5 with DC mode
	if (IsFlag0(POWER_STAT_CTRL, adapter_in)) {ClrFlag(BAT_MISC3, BQ24780S_Init_OK);}
	#endif
	DelayXms(5);

	CAPSLED_on();
	NUMLED_on();
	SCROLLLED_on();

	EnablePS2Interrupt();
}


#ifdef	DeepS3_SUPPORT	// 20120104 Chris add for support Deep S3
void S0S3toDPS3S4S5OffPower(void)
{
	DDR3_CORL_EC_Low();

	#ifdef	OEM_DIMM
	SetFlag(POWER_STAT_CTRL, DIMM_OFF_Flag);
	#endif	//OEM_DIMM

	SetFlag(MISC_FLAG, PCBeep_Status);
	ClrFlag(Device_STAT_CTRL, ECO_LED_ON_BLUE + ECO_LED_ON_GREEN + ECO_LED_ON_LightBLUE);
	ClrFlag(KETBOARD_STAT_CTRL, CRISIS_ON);
	ClrFlag(POWER_FLAG, wait_HWPG + system_on + power_on);
	ClrFlag(SYS_MISC2, postpone_SCI + postpone_SMI);
	HWPG_TIMER = 0;
	ClrFlag(POWER_FLAG, ACPI_OS);
	DisablePS2Interrupt();
	InitQ20();

	Led_Data = 0x00;
	OEM_Write_Leds(Led_Data);

	DAPWM_VADJ = 0;
	#ifdef	 OEM_FAN	// leox_20110916 FanByDAorPWM -> OEM_FAN
	DAPWM_VFAN = 0;
	#endif	//OEM_FAN

	#ifdef	OEM_IPOD
	EnableUSBCHARGPORTFun();
	#endif	//OEM_IPOD

	ClrFlag(Device_STAT_CTRL, Lan_Status);	// Default set WL LED off
	ClrFlag(Device_STAT_CTRL, BT_Status);	// Default set Bluetooth off

	#ifdef	OEM_BATTERY
	TurnOffADISCHG();
	TurnOffBDISCHG();
	#endif	//OEM_BATTERY

	#ifdef	OEM_IPOD
	CheckUSBchargingEnDis();
	#endif	//OEM_IPOD

	ENBEEP_off();		// ENBEEP off
	RCIN_on();
	GA20_off();
	WLAN_LED_off();	// WL LED off

	ECPWROK_off();
	ClrFlag(ToSBsignals00, ECPWROK);

	PCHPWROK_off();
	DelayXms(6);
	VRON_off();
	ClrFlag(ToMBsignals00, VRON);

	#ifdef	PECI_Support		// ben_20131120
	CLR_MASK(HOCTLR, PECIHEN);	// After CPU VCC off, clear PECI Host Enable by EC
	#endif	//PECI_Support

	#ifdef	HSPI	// leox_20110930	// leox_20120110 From SUSON_off()
	Disable_HSPI();
	#endif

	MAINON_2_off();	// leox20150605
	MAINON_1_off();
	MAINON_off();
	SATA_LED_BBO_Dis_Off();
    #if Support_IT8987  // Leo_20141111 8987 RSTS no support bit7-6 = 0 1
    CLR_MASK(RSTS, BIT6);
    #endif
	ClrFlag(ToMBsignals00, MAINON);
	#ifdef SUPPORT_BQ24780S	// leox20150824 System enter S3/S4/S5 with DC mode
	if (IsFlag0(POWER_STAT_CTRL, adapter_in)) {ClrFlag(BAT_MISC3, BQ24780S_Init_OK);}
	#endif
	DelayXms(5);

	CAPSLED_on();
	NUMLED_on();
	SCROLLLED_on();

	EnablePS2Interrupt();

	ClrFlag(ABAT_MISC, force_discA + abat_inUSE + abat_next);
	ClrFlag(BBAT_MISC, force_discB + bbat_inUSE + bbat_next);
	ClrFlag(ABAT_STATUS, abat_dischg);
	ClrFlag(BBAT_STATUS, bbat_dischg);
	ClrFlag(BAT_MISC1, STOP_charge);
	ClrFlag(ABAT_MISC, abat_AutoLearn);
	ClrFlag(BBAT_MISC, bbat_AutoLearn);

	ClrFlag(POWER_FLAG, enter_SUS);
	if (IsFlag0(ABAT_MISC2, Shipmode))
		SetFlag(POWER_FLAG2, func_LED);
	ClrFlag(KETBOARD_STAT_CTRL, LCtrlFn_RCtrl);

	PWRLED0_off();
	PWRLED1_off();

//	SUSON_off();
//	ClrFlag(ToMBsignals00, SUSON);
	DelayXms(1);

	DisableUSBPort1();
	DisableUSBPort2();

	ClrFlag(POWER_FLAG, PwrOffRVCCOff);
	TurnOffSBPowerSource();
}
#endif	// DeepS3_SUPPORT


void DisablePS2Interrupt(void)
{
	CLR_BIT(IELMR2, 2);
	CLR_BIT(IELMR2, 3);
	CLR_BIT(IELMR2, 4);
}


void EnablePS2Interrupt(void)
{
	SET_BIT(IELMR2, 2);
	SET_BIT(IELMR2, 3);
	SET_BIT(IELMR2, 4);
}


void InitQ20(void)
{
	ClrFlag(SMB_STS, BIT6);
	//ClrFlag(SMB2_STS, BIT6);
	//ClrFlag(SMB3_STS, BIT6);
}


void S0S3toS4S5OffPower(void)
{
//	ClrFlag(ABAT_MISC, force_discA + abat_inUSE + abat_next);
//	ClrFlag(BBAT_MISC, force_discB + bbat_inUSE + bbat_next);
//	ClrFlag(ABAT_STATUS, abat_dischg);
//	ClrFlag(BBAT_STATUS, bbat_dischg);
//	ClrFlag(BAT_MISC1, STOP_charge);
	ClrFlag(ABAT_MISC, abat_AutoLearn);
	ClrFlag(BBAT_MISC, bbat_AutoLearn);
	ClrFlag(ABAT_MISC2, abat_test);

	ClrFlag(POWER_FLAG, enter_SUS);
	if (IsFlag0(ABAT_MISC2, Shipmode))
		SetFlag(POWER_FLAG2, func_LED);
	ClrFlag(KETBOARD_STAT_CTRL, LCtrlFn_RCtrl);

	PWRLED0_off();
	PWRLED1_off();

	#ifdef	PECI_Support	// leox_20111004
	CPUTjmax = 0;
	PECI_CPU_temp = 0;
	#endif

	DelayXms(22);	// leox20150612 Add for Intel SKL-H of FH9C (DDR4 power off timing)
	SUSON_off();
	ClrFlag(ToMBsignals00, SUSON);
	DelayXms(1);

	DisableUSBPort1();
	DisableUSBPort2();

	#if	OEM_POWER_LOSS	// leox_20110921
	B_PwrLoss &= 0x03;	// Last power state: Off
		#if SUPPORT_EFLASH_FUNC	// leox20160322
	eFlashSave(&B_PwrLoss, B_PwrLoss);
		#endif
	#endif
}


void EnableUSBPort1(void)
{
	USBP_on();
	USBP_DISCHARGE_off();
}


void DisableUSBPort1(void)
{
	USBP_off();
	USBP_DISCHARGE_on();
}


void EnableUSBPort2(void)
{
	USBP2_on();
	USBP2_DISCHARGE_off();
}


void DisableUSBPort2(void)
{
	USBP2_off();
	USBP2_DISCHARGE_on();
}


void CheckTurnOffSBLANPowerSource(void)
{
	ClrFlag(POWER_FLAG, PwrOffRVCCOff);
	if (IsFlag0(SYS_MISC2, InRestart) && IsFlag0(POWER_STAT_CTRL, WakeOnLan))
	{
		if (IsFlag0(POWER_STAT_CTRL, WakeOnLan)) {TurnOffLanPower();}
		#ifdef	Support_Wake_On_WLan
		if (IsFlag0(POWER_STAT_CTRL1, S5_Reboot))
		{
			if (IsFlag0(POWER_STAT_CTRL, WakeOnRTC))
			{
				if (!((IsFlag1(POWER_STAT_CTRL1, WakeOnWLan)) && (IsFlag1(POWER_STAT_CTRL, enter_S4))))
				{
					if (IsFlag0(KETBOARD_STAT_CTRL, WakeOnUMTS))
					{
						if (IsFlag0(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
						{
							if (IsFlag0(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_ACBAT_BRAM))
								TurnOffSBPowerSource();
						}
						else
						{
							if (IsFlag0(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_ACBAT_BRAM))
							{
								if (!(AC_IN_read()))
									TurnOffSBPowerSource();
							}
							else
							{
								if (!(AC_IN_read()))
								{
									if (BIOS_ABAT_CAP <= USBCHARGEDThrldofcapacity)
										TurnOffSBPowerSource();
								}
							}
						}
					}
				}
			}
		}
		#else
		if ((IsFlag0(POWER_STAT_CTRL, WakeOnRTC)) && (IsFlag0(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM)) && (IsFlag0(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_ACBAT_BRAM))
			 && (IsFlag0(POWER_STAT_CTRL1, S5_Reboot)))
		{
			TurnOffSBPowerSource();
		}
		#endif
	}
	if (WaitS5ToDoWDT == 0x5A)
	{
		TurnOffSBPowerSource();
	}
}


void TurnOffLanPower(void)
{
	#ifdef	OEM_Calpella
	if ((IsFlag0(FromSBsignals00, SLP_LAN)) && (IsFlag0(FromSBsignals00, WOL_EN)) && (IsFlag1(ToSBsignals00, InterLANPower)))
	#else	//OEM_Calpella
	if (IsFlag1(ToSBsignals00, InterLANPower))
	#endif	//OEM_Calpella
	{
		ClrFlag(ToSBsignals00, InterLANPower);
		DelayXms(20);
		LAN_RESET_off();
		DelayXms(2);
		LAN_WOL_EN_EC_off();
	}
}


void TurnOffSBPowerSource(void)
{
	#ifdef	OEM_Calpella
	TurnOffSBPowerSource_Calpella();
	#else	//OEM_Calpella
	if (IsFlag1(ToSBsignals00, SB_PowerSource))
	{
		#ifdef	HSPI
			#if	(!HSPI_OFF_WITH_VCC)
		Disable_HSPI();
			#endif
		#endif
		ClrFlag(POWER_FLAG, PwrOffRVCCOff);
		RSMRST_on();			// leox_20111109	// leox20150721 Fix RSMRST#
		#ifdef	DeepS3_SUPPORT	// leox_20120111
		DelayXms(SLP_SUS_Low() ? RVcc2RSMRST_DS3 : RVcc2RSMRST);
		#else
		DelayXms(RVcc2RSMRST);
		#endif
		TurnOffAC_PRESENT();	// leox20150605 Add for FH9C
		USB_CHARGE_ILIM_LO();	// CB1 low	// leox20150626 For USB charger SLG55544 of FH9C
		USB_CHARGE_CTL1_LO();	// CB0 low	// leo_20150917 USB charging
		STBON_off();
		ClrFlag(ToSBsignals00, SB_PowerSource);
		SYSTEMSTATE = 0x05;
		ResetSusBSusC();
	}
	else if(S5_ON_Read())
	{
		STBON_off();
	}
	
	if (WaitS5ToDoWDT == 0x5A)
	{
		DisableAllInterrupt();		//@Jay20130222 FIX_SAVE_EEPROM_EC_HANGUP	// leox20150718 Refer to project 0H3
		SET_MASK(HINSTC1, SCECB);	// Rescan 16 byte signature					// leox20150718 Refer to project 0H3
		//+Jay20121130 add_oem_mirror_code
		#if Support_IT8987			// chris_20140924
		ChangePLLFrequency(0x01);	// 20140827 Jay suggest before mirror code
		#endif
		Code_Mirror();
		//-Jay20121130 add_oem_mirror_code
	}
	#endif	//OEM_Calpella
}


void ResetSusBSusC(void)
{
	FromSBsignals00 = 0;
	FromSBsignals00_BK0 = 0;
	FromSBsignals00_BK1 = 0;
	FromSBsignals00_BK2 = 0;
}


void CheckPower(void)
{	// leox_20130208 Rewrite function
	#ifdef	OEM_BATTERY
	BYTE i;
	#endif	//OEM_BATTERY

	if (AC_IN_read())
	{
		if ((ABAT_MODEL == 0) && (IsFlag1(ABAT_STATUS, abat_in)))
		{
			SetFlag(MISC_FLAG, F_CriLow);
			return;
		}
		else
		{
			ClrFlag(MISC_FLAG, F_CriLow);
		}
	}
	else
	{
		#ifdef	OEM_BATTERY
		for (i = 0; i < 3; i++)
		{
			SMB_ADDR_OEM = _SMB_BatteryAddr;			// David_20130207 Fix coding bug
			SMB_CMD_OEM = _CMD_RelativeStateOfCharge;	// David_20130207 Fix coding bug
			if (vOSSMbusRByte_OEM())
			{
				Round_Off_RSOC();
				if (SMB_DATA_OEM < 6) {SetFlag(MISC_FLAG, F_CriLow);}
				break;
			}
			DelayXms(10);
		}
		#endif	//OEM_BATTERY
	}
}


void PSWOnPower(void)
{
	SetFlag(CMOSReset_Status, WaitSUSBSUSC);
	SetFlag(CMOSReset_Status, WaitPCH);
	ClrFlag(CMOSReset_Status, ClearCMOS);
	TurnOnSBPowerSource();
	PSWOffPower();
}


void PSWOffPower(void)
{
	DNBSWON_on();
	PSW_COUNTER = T_PSWOFF;
}


void TurnOnSBPowerSource(void)
{
	if (IsFlag0(ToSBsignals00, SB_PowerSource))
	{
		STBON_on();
		#ifdef	 DeepS3_SUPPORT	// 20120104 Chris add for support Deep S3
		DelayXms(SLP_SUS_Low() ? RVcc2RSMRST_DS3 : RVcc2RSMRST);	// leox_20120110
		#else
		DelayXms(RVcc2RSMRST);
		#endif
		RSMRST_off();			// leox_20111109	// leox20150721 Fix RSMRST#
		SetFlag(ToSBsignals00, SB_PowerSource);
		#ifdef	Intel_Internal_Lan_Support
		DelayXms(5);
		TurnOnLanPower();
		#endif	//Intel_Internal_Lan_Support
		#ifdef	OEM_Calpella
		DelayXms(200);
		AC_PRESENT_output();
		#else	// leox20150522 FH9C
		DelayXms(50);
		AC_IN_read() ? TurnOnAC_PRESENT() : TurnOffAC_PRESENT();	// leox20150605 For FH9C
		DelayXms(50);
		#endif	//OEM_Calpella
		#ifdef	HSPI
			#if	(!HSPI_OFF_WITH_VCC)
		Enable_HSPI();
			#endif
		#endif
	}
}


void TurnOnLanPower(void)
{
	if (IsFlag0(ToSBsignals00, InterLANPower))
	{
		SetFlag(ToSBsignals00, InterLANPower);
		LAN_WOL_EN_EC_on();
		DelayXms(2);
		LAN_RESET_on();
		DelayXms(20);
	}
}

void SHDN_Protect(void)
{
	if(SHDN_Read())
	{
		if (IsFlag1(POWER_FLAG, system_on))
		{	
			ShutDnCause = 0xC1;
			ForceOffPower();
		}
	}
}



//------------------------------------------------------------------------------
// Hook enable interrupt
//------------------------------------------------------------------------------
void Hook_EnableInterrupt(void)
{
	#if	CIRFuncSupport
	if (IsFlag0(POWER_FLAG, power_on)) {SET_MASK(IER1, Int_CIR);}
	#endif
}


//------------------------------------------------------------------------------
// Debug function
//------------------------------------------------------------------------------
#define	DebugRamAddr	0x800
#define	DebugRamRange	0xFF
void RamDebug(unsigned char dbgcode)
{
	PORT_BYTE_PNTR ClearIndex;
	PORT_BYTE_PNTR byte_register_pntr;
	BYTE index;
	BYTE i;

	byte_register_pntr = DebugRamAddr + DebugRamRange;
	index = *byte_register_pntr;
	*byte_register_pntr += 1;

	if (*byte_register_pntr == DebugRamRange)
	{
		*byte_register_pntr = 0;
		ClearIndex = DebugRamAddr;
		for (i = 0x00; i < DebugRamRange; i++)
		{
			*ClearIndex = 0x00;
			ClearIndex++;
		}
	}

	byte_register_pntr = DebugRamAddr + index;
	*byte_register_pntr = dbgcode;
}


//------------------------------------------------------------------------------
// Hook 1ms events
//------------------------------------------------------------------------------
// leox_20110920 Remove parameter "EventId"
void Hook_Timer1msEvent(void)
{
	//David_20120327 add for SMBUS reset timing follow SMBUS spec
	#ifdef	SMBUS_RESET_SUPPORT
	CNTResetSMBus();
	#endif
	//David_20120327 add for SMBUS reset timing follow SMBUS spec

	#ifndef OEM_Calpella	// leox20150611 From Service_T10mSEC() for Intel SKL-H of FH9C
	GetSBMBsignals4PowerSequence();
	ChkSBMBsignals4PowerSequence();
	#endif
	SHDN_Protect();
}


//------------------------------------------------------------------------------
// Hook 5ms events
//------------------------------------------------------------------------------
void Hook_Timer5msEventA(void)
{
	Service_T5mSEC0();

	switch (Timer5msCnt)		// 1, 2, 3, 5, 10 sec and 1 min events
	{
	case 1:
		Service_T1SEC0();
		break;

	case 11:
		Service_T1SEC1();
		break;

	case 21:
		Service_T1SEC2();
		break;

	case 31:
		Service_T1SEC3();
		break;

	case 41:
		Service_T1SEC4();
		break;

	case 51:
		Service_T1SEC5();
		break;

	case 61:
		Service_T1SEC6();
		break;

	case 71:
		Service_T1SEC7();
		break;

	case 81:
		Service_T1SEC8();
		break;

	case 91:
		Service_T1SEC9();
		break;

	case 101:
		Service_T1SEC10();
		break;

	case 111:
		Service_T1SEC11();
		break;

	case 121:
		Service_T1SEC12();
		break;

	case 131:
		Service_T1SEC13();
		break;

	case 141:
		Service_T1SEC14();
		break;

	case 151:
		Service_T1SEC15();
		break;

	case 161:
		Service_T1SEC16();
		break;

	case 171:
		Service_T1SEC17();
		break;

	case 181:
		Service_T1SEC18();
		break;

	case 191:
		Service_T1SEC19();
		break;

	case 19:
		Service_T1SEC20();
		break;

	case 39:
		Service_T1SEC21();
		break;

	case 59:
		Service_T1SEC22();
		break;

	case 79:
		Service_T1SEC23();
		break;

	case 99:
		//ken test timer 20130315 ++
		Service_T1SEC24();
		//ken test timer 20130315 ++
		break;

	case 119:
		if (!(Timer1SecCnt % 2)) {Service_T2SEC();}
		break;

	case 139:
		if (!(Timer1SecCnt % 3)) {Service_T3SEC();}
		break;

	case 159:
		if (!(Timer1SecCnt % 5)) {Service_T5SEC();}
		break;

	case 179:
		if (!(Timer1SecCnt % 10)) {Service_T10SEC();}
		break;

	case 199:
		if (!(Timer1SecCnt % 60)) {Service_T1Min();}
		//ken test timer 20130315 --
		//if (Timer1SecCnt >= 60) {Timer1SecCnt = 0;}
		//Timer1SecCnt++;
		//ken test timer 20130315 --
		break;
	}
}


void Hook_Timer5msEventB(void)
{
	Service_T5mSEC1();

	switch (Timer5msCnt)		// 500ms events
	{
	case   7:
	case 107:
		Service_T500m0SEC();
		break;

	case  17:
	case 117:
		Service_T500m1SEC();
		break;

	case  27:
	case 127:
		Service_T500m2SEC();
		break;

	case  37:
	case 137:
		Service_T500m3SEC();
		break;

	case  47:
	case 147:
		Service_T500m4SEC();
		break;

	case  57:
	case 157:
		Service_T500m5SEC();
		break;

	case  67:
	case 167:
		Service_T500m6SEC();
		break;

	case  77:
	case 177:
		Service_T500m7SEC();
		break;

	case  87:
	case 187:
		Service_T500m8SEC();
		break;

	case  97:
	case 197:
		Service_T500m9SEC();
		break;
	}
}


void Hook_Timer5msEventC(void)
{
	Service_T5mSEC2();

	switch (Timer5msCnt)		// 100ms events
	{
	case   3:
	case  23:
	case  43:
	case  63:
	case  83:
	case 103:
	case 123:
	case 143:
	case 163:
	case 183:
		Service_T100mSEC0();
		break;

	case   9:
	case  29:
	case  49:
	case  69:
	case  89:
	case 109:
	case 129:
	case 149:
	case 169:
	case 189:
		Service_T100mSEC1();
		break;

	case  13:
	case  33:
	case  53:
	case  73:
	case  93:
	case 113:
	case 133:
	case 153:
	case 173:
	case 193:
		Service_T100mSEC2();
		break;
	}
}


void Hook_Timer5msEventD(void)
{
	Service_T5mSEC3();

	if (Timer5msCnt & 0x01)	 // 50ms event
	{
		if (!(Timer5msCnt % 5)) {Service_T50mSEC();}
	}
}


void Hook_Timer5msEventE(void)
{
	Service_T5mSEC4();

	if (!(Timer5msCnt & 0x01))  // 10ms and 20ms events
	{
		Service_T10mSEC();
		(Timer5msCnt & 0x02) ? Service_T20mSEC0() : Service_T20mSEC1();
	}
}

//------------------------------------------------------------------------------
// OEM service routine
//------------------------------------------------------------------------------
void Service_T5mSEC0(void)
{
	#ifdef	PECI_Support
	PECI_PowerCtrlCenter();
	#endif	//PECI_Support
	#ifdef	DISPON_EC_Support
	ChkDISPON_Signal();
	#endif	//DISPON_EC_Support
	//David add for MMB service 2014/05/21
	#ifdef IT_7230MMB_SUPPORT
	PollingIT723XInt();
	#endif
	//David add for MMB service 2014/05/21

//	if (IsFlag1(POWER_FLAG, system_on)) {}
}

void Service_T5mSEC1(void)
{
	#ifdef	DLID_Function
	ChkDLIDbyLID();
//	CheckAPU_BLON();    // leo_20150518 add panel blon need wait APU_BLON		// leox20150522 Disabled for FH9C	// CHECK
	#endif	//DLID_Function
	#if SUPPORT_EFLASH_FUNC		// leox20150630 Access eFlash	// leox20160322
	eFlashToDo();		// leox20160316
	eFlashDbgFunc();	// leox20160322 From Service_T500m5SEC()
	#endif
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T5mSEC2(void)
{
	#if	ACPI_QEventPending
	Get_PendingECQEvent();
	#endif	//ACPI_QEventPending
	#ifdef	LAN_WRITE_PROTECT_Function
	CheckLAN_WP();
	#endif	//LAN_WRITE_PROTECT_Function
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T5mSEC3(void)
{
	#ifdef	OEM_Calpella
	GetSBMBsignals4PowerSequence();
	ChkSBMBsignals4PowerSequence();
	#endif	//OEM_Calpella
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T5mSEC4(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	LogP80InECRamDxx
		LogP80();
		#endif	//LogP80InECRamDxx
		#ifdef	OEM_OneTouchButtons
		PollOneTouchButtons();	// Add One Touch Buttons function 2011/08/19
		#endif	//OEM_OneTouchButtons
	}
}


void Service_T1SEC0(void)
{	// leox_20130311 Code from Service_T1SEC24()
	if (IsFlag1(POWER_FLAG, system_on))
	{
		ForITEWinFlashTool();	// For ITE WinFlash Tool	// ken 2012/06/29
	}
}


void Service_T1SEC1(void)
{
	#ifdef	OEM_BATTERY
		#ifdef	BattChg_SMBtoCHGIC
	if (IsFlag0(ABAT_MISC2, Shipmode))
		SetChgICChargingVoltage();
		#endif	//BattChg_SMBtoCHGIC
	#endif	//OEM_BATTERY
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC2(void)
{
	#ifdef	OEM_BATTERY
		#ifdef	BattChg_SMBtoCHGIC
	if (IsFlag0(ABAT_MISC2, Shipmode))
		SetCghICInputCurrent();
		#endif	//BattChg_SMBtoCHGIC
	#endif	//OEM_BATTERY
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC3(void)
{
	#ifdef	OEM_BATTERY
	if (IsFlag0(ABAT_MISC2, Shipmode))
		RetryGetBatteryModel();	// timebase: 1 sec
	#endif	//OEM_BATTERY
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC4(void)
{
	#ifdef	OEM_BATTERY
	if (IsFlag0(ABAT_MISC2, Shipmode))
		ProcessCharge();		// timebase: 1 sec:
	#endif	//OEM_BATTERY
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC5(void)
{
	#ifdef	OEM_BATTERY
	if (IsFlag0(ABAT_MISC2, Shipmode))
		FCC_UpDate();
	HookChkIdle();
	#else	// leox_20110921
		#if	ENTER_SLEEP_MODE_AC	// leox_20110921
	HookChkIdle();
		#endif
	#endif	//OEM_BATTERY
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC6(void)
{
	#ifdef	OEM_BATTERY
	if (IsFlag0(ABAT_MISC2, Shipmode))
		ProcessAutoLearn();		// Check battery learning
	#endif	//OEM_BATTERY
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC7(void)
{
	#ifdef	OEM_BATTERY
	if (IsFlag0(ABAT_MISC2, Shipmode))
	{
		ProcessDischarge();
		TrickleCharge();		// Check Li recharge and Ni-MH trickle charge
	}
	#endif	//OEM_BATTERY
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC8(void)
{
	#ifdef	IT_7230MMB_SUPPORT
	ITE_Check_ESD();
	//David modify for battery shutdown mode 2014/07/02
	//ProcessLED();
	//David modify for battery shutdown mode 2014/07/02
	if (IsFlag1(POWER_FLAG, system_on))
	{
		MMBLEDTest();
	}
	#endif
}


void Service_T1SEC9(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		DelayTimer();
	}
}


void Service_T1SEC10(void)
{
	#ifdef	OEM_BATTERY
	if (IsFlag0(ABAT_MISC2, Shipmode))
	{
		#ifdef	BattChg_SMB_OVS
			#ifdef	BattChg_SMBtoTBLtoDAC
		ChkChgCurfromBattMax();
			#endif	//BattChg_SMBtoTBLtoDAC
			#ifdef	BattChg_SMBtoCHGIC
		ChkChgCurfromBattMax();
			#endif	//BattChg_SMBtoCHGIC
		#endif	//BattChg_SMB_OVS
	}
	#endif	//OEM_BATTERY
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC11(void)
{
	#ifdef	OEM_NECP
		#ifdef	OEM_BATTERY
			#ifdef	OEM_VICMV
//	CheckVICMVThrottlingFUN();		// leox20150717 Move to Service_T100mSEC2()	// leox20150916 Change time base to 20ms and move to Service_T20mSEC0()
			#endif	//OEM_VICMV
			#ifdef	OEM_DCMI
	CheckDCMIThrottlingFUN();
			#endif	//OEM_DCMI
			#ifdef	OEM_VICMVACDC
//	CheckVICMVThrottlingFUNACDC();	// leox20150916 Move to Service_T10mSEC()	// leo_20150916 add power protect
			#endif	//OEM_VICMVACDC
		#endif	//OEM_BATTERY
	#endif	//OEM_NECP
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC12(void)
{
	Check_Volume_CNT();
	SendDummySCI();
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC13(void)
{
	PollAVFUNCTIONLED();
	#ifdef	OEM_S4S5WDG
	WatchDOGS4S5_Resume();
	#endif	//OEM_S4S5WDG
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC14(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		CopyECRAM2BRAM();
		#ifdef	OEM_IPOD		// leox20150918 From CopyECRAM2BRAM()
		USBChargeFunctionS0();	// leox20150914 For USB charger SLG55544 to set SDP/CDP in S0 when BIOS set
		#endif	//OEM_IPOD
		#ifdef	VistaOSTAT_Support
		if (IsFlag1(POWER_FLAG, ACPI_OS)) {ECQEvent(Q80_SCI, SCIMode_Normal);}
		#endif	//VistaOSTAT_Support
	}
}


void Service_T1SEC15(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	OEM_THERM
			#ifndef OEM_Calpella
		InitionThermalSensor();
			#endif	//OEM_Calpella
			#ifdef	 PECI_Support	// leox_20111004
				#if	ReadDRAMTemperature
		PECI_ReadDRAMTemperature();
				#endif
			#endif	// PECI_Support
		#endif	//OEM_THERM
	}
}


void Service_T1SEC16(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	OEM_THERM
		ReadThermalSensor();
		#endif	//OEM_THERM
	}
}


void Service_T1SEC17(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	 OEM_THERM	// leox_20111004
			#ifdef	PECI_Support
				#if	ReadPKGTemperature
		PECI_ReadPkgTemperature();
				#endif
			#endif	// PECI_Support
		#endif	//OEM_THERM
	}
}


void Service_T1SEC18(void)
{
	#ifdef	OEM_Calpella
	ChkneedTurnOffSBLANPwrFS345();
	#endif	//OEM_Calpella
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC19(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	OEM_FAN
		FanCtrlDACPWMRPM();
		UpDateFanControl_Mode();
		#endif	//OEM_FAN
	}
}


void Service_T1SEC20(void)
{
	//David add for read battery cycle count 2014/06/11
	if (IsFlag1(POWER_FLAG, system_on))
	{
		if (IsFlag0(ABAT_MISC2, Shipmode))
			PollingCycle();
	}
	//David add for read battery cycle count 2014/06/11
}


void Service_T1SEC21(void)
{
	PollingCriticalWake();
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1SEC22(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	Battery_3Cell_Throttling
		Throttling_3cell_Battery();
		#endif
	}
}


void Service_T1SEC23(void)
{
//	if (IsFlag1(POWER_FLAG, system_on)) {}
	#ifdef SUPPORT_BQ24780S	// leox20150915
	if (IsFlag0(ABAT_MISC2, Shipmode))
	{
		if (LearnModeSetCnt > 0)
		{
			LearnModeSetCnt--;
			SetChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_EN_LEARN);	// Enable LEARN mode
		}
		if (LearnModeClrCnt > 0)
		{
			LearnModeClrCnt--;
			ClrChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_EN_LEARN);	// Disable LEARN mode
		}
	}
	#endif
}


void Service_T1SEC24(void)
{
    #if CMOSprotect
	CheckCMOSReset();     // leo_20150515 test
	ClearCMOSProgress();  // leo_20150515 test
    #endif
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T2SEC(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	ForceFanOffDuringACPImode
		if (IsFlag1(POWER_FLAG, ACPI_OS))
		{
			SetFlag(MISC_FLAG, FAN_TEST);	// Fan test function enable
			FAN_LIMIT = 0;
			FAN_LIMIT2 = 0;	// leox_20120322 Support_FAN2
		}
		#endif	//ForceFanOffDuringACPImode
	}
}


void Service_T3SEC(void)
{
	GetJumpper();

	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	OEM_BATTERY
			#ifdef	NECP_BattRefresh_Support		// leox_20130312 For new timer routine
		CheckNECP_BattRefresh();
			#endif	//NECP_BattRefresh_Support
			#ifdef	BatteryOnlyXXPThrollFUN
		if (IsFlag0(ABAT_MISC2, Shipmode))
			ChkBatteryOnlyXXPThroll();
			#endif	//BatteryOnlyXXPThrollFUN
		#endif	//OEM_BATTERY
	}
}


void Service_T5SEC(void)
{
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T10SEC(void)
{
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T1Min(void)
{
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T10mSEC(void)
{
	vDebounce(CheckPowerSwitch);
	CheckPSWDelay();		// add for power switch delay
	#ifdef	OEM_Calpella
	Check_SLP_M();
	Check_AC_PRESENT();
	#endif	//OEM_Calpella
	Check_AC_PRESENT();		// leox20150522 Add for FH9C	// CHECK
	ChkMEFW_OVERRIDE();
	Check3GStatus();
	#if 0	//#ifndef OEM_Calpella	// leox20150611 Move to Hook_Timer1msEvent() for Intel SKL-H of FH9C
	GetSBMBsignals4PowerSequence();
	ChkSBMBsignals4PowerSequence();
	#endif	//OEM_Calpella

	#ifdef	 DeepS3_SUPPORT	// 20120104 Chris add for support Deep S3
	if (IsFlag1(POWER_FLAG, Deep_S3) && (!SLP_SUS_Low())) {DPS3toS3();}
	#endif

	#ifdef	OEM_NECP
		#ifdef	OEM_BATTERY
			#ifdef	OEM_VICMVACDC
	ScanAnalogInput();
	CheckVICMVThrottlingFUNACDC();	// leox20150717 From Service_T1SEC11()		// leo_20150916 add power protect
			#endif
		#endif
	#endif

	if (IsFlag1(POWER_FLAG, system_on))
	{
		#if	CHECK_FAN_FAULT	// David_20120613 add for fan error check
		if (Fan1_Fault_CNT != 0) {CheckFan1Fault();}
		#endif
	}
}


void Service_T20mSEC0(void)
{
	#ifdef	 Multiple_TP	// ken add for touchpad compare
	DetectTPID();
	#endif

	vDebounce(CheckACIn);

	#ifdef	OEM_NECP
		#ifdef	OEM_BATTERY
			#ifdef	OEM_VICMV
	ScanAnalogInput();
	CheckVICMVThrottlingFUN();	// leox20150717 From Service_T1SEC11()			// leox20150916 Change time base to 20ms
			#endif
		#endif
	#endif

	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	UpDate_Brighness_Support
		UpDate_Brighness();
		#endif	//UpDate_Brighness_Support
	}
}


void Service_T20mSEC1(void)
{
	#ifdef	OEM_BATTERY
	ChkISL6251ACHICWaitVinCNT();
	#endif	//OEM_BATTERY

	#ifdef	OEM_IPOD
	CheckUSBWorkingCNT();
	#endif	//OEM_IPOD

	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	 OEM_FAN		// leox_20111011
		GetFan1RPM();
			#if	Support_FAN2	// leox_20120322
		GetFan2RPM();
			#endif
		#endif	// OEM_FAN

		#if	SUPPORT_OEM_APP		// leox_20111014
		OemAppFuncService();
		#endif
	}
}


void Service_T50mSEC(void)
{
	#ifdef	OEM_BATTERY
	vDebounce(CheckBatteryIn);
		#ifdef	OEM_NECP
	if (IsFlag0(ABAT_MISC2, Shipmode))
		IfReadWriteBattFirstDate();
		#endif	//OEM_NECP
	vDebounce(CheckLIDIn);
	#endif	//OEM_BATTERY

	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	 FanAlwaysOnForES1	// leox_20120507
			#ifdef	FanByDAorPWM
		FAN_LIMIT = 255;
			#else	//FanByDAorPWM
		FAN_LIMIT = CTR;
			#endif	//FanByDAorPWM
		DAPWM_VFAN = FAN_LIMIT;
			#if	Support_FAN2		// leox_20120322
				#ifdef	FanByDAorPWM
		FAN_LIMIT2 = 255;
				#else
		FAN_LIMIT2 = CTR;
				#endif
		DAPWM_VFAN2 = FAN_LIMIT2;
			#endif
		#endif	//FanAlwaysOnForES1

		#ifdef	SetBrightnessForES1
			#ifdef	UpDate_Brighness_Support
		BrightnessValue = CTR;	// leox_20110917 200 -> CTR
			#endif	//UpDate_Brighness_Support
		#endif	//SetBrightnessForES1
		DimmLEDProcess();
	}
}


void Service_T100mSEC0(void)
{
	CopyRAM2OEMBIOSaccess();

	#ifdef	Setting_MAX8742ETI_Support
	Setting_MAX8742ETI();
	#endif	//Setting_MAX8742ETI_Support

	if (IsFlag1(POWER_FLAG, power_on))
	{
		if (IsFlag0(Converter_Config_Status, Error_Over))
		{
			Converter_FW_Stop();
			Converter_Config_Start();
		}
	}
}


void Service_T100mSEC1(void)
{
	ChgICDbgFunc();	// leox20150623 Add for charger IC debug

	#ifdef	OEM_MotionLed
	NECP_MotionLedsFunction();
	#endif	//OEM_MotionLed

	#ifdef	OEM_TP
	CheckOemTouchpadStatus();
	#endif	//OEM_TP

//	if (IsFlag1(POWER_FLAG, system_on)) {}
}

void Service_T100mSEC2(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		Hybrid_Status_Control();
		BoostModeControl();
		#ifdef	OEM_FAN
		ChkFanCtrlDACPWMRPM();
		#endif	//OEM_FAN
	}
}


void Service_T500m0SEC(void)
{
	if (IsFlag1(POWER_FLAG, system_on))
	{
		#ifdef	OEM_BATTERY		// leox_20130312 For new timer routine
			#ifdef	NECP_BattRefresh_Support
		CheckNECP_BattRefresh2();
			#endif	//NECP_BattRefresh_Support
		#endif	//OEM_BATTERY
	}
}


void Service_T500m1SEC(void)
{
	#ifdef	OEM_BATTERY
	if (IsFlag0(ABAT_MISC2, Shipmode))
		PollingBatteryData();	// Polling battery information
		#ifdef	SUPPORT_BQ24780S	// leox20150901
		BQ24780S_SetRegByPN();		// leox20150907
		#endif
	#endif	//OEM_BATTERY
	ECOSleepCheck();
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T500m2SEC(void)
{
	if (IsFlag1(POWER_FLAG, power_on)) {vGetSelectorState(0);}	// @CP_24EC06WWx, ITE ADD
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T500m3SEC(void)
{
	//David modify for battery shutdown mode 2014/07/02
	ProcessLED();
	#ifdef	SUPPORT_Shipmode
	EnterShipMode();
	#endif	//SUPPORT_Shipmode
	//David modify for battery shutdown mode 2014/07/02
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T500m4SEC(void)
{
	ScanAnalogInput();
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T500m5SEC(void)
{
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T500m6SEC(void)
{
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T500m7SEC(void)
{
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T500m8SEC(void)
{
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void Service_T500m9SEC(void)
{
//	if (IsFlag1(POWER_FLAG, system_on)) {}
}


void CheckPSWDelay(void)
{
	if (IsFlag1(MISC_FLAG, F_DelayPSW))
	{
		PSW_TIMER--;
		if (PSW_TIMER == 0xFF)
		{
			DNBSWON_off();
			ClrFlag(MISC_FLAG, F_DelayPSW);
		}
	}
}


void wSwapHi_Low_Byte_for_copy(WORD *valueSorce, WORD *valueDest)
{
	*valueDest = ((((BYTE)(*valueSorce)) * 256) + ((*valueSorce) >> 8));
}


#if	SUPPORT_OEM_APP // leox_20111014
//------------------------------------------------------------------------------
// Service routine for OEM application
//------------------------------------------------------------------------------
// leox_20111014 Create function
// leox_20111017 Add counter for volume get
// leox_20111109 Execute special command
// leox_20120507 Move code to OemAppGetSetVol(), OemAppCntTask() and OemAppCmd()
void OemAppFuncService(void)
{
	if (IsFlag0(POWER_FLAG, ACPI_OS)) {return;}
	OemAppGetSetVol();	// Set/Get/Handshaking for volume and mute state
	OemAppCntTask();	// Execute tasks by counters
	OemAppCmd();		// Execute command requested by application
}


//------------------------------------------------------------------------------
// Get and set volume value or mute state via application
//------------------------------------------------------------------------------
// leox_20120507 Code from OemAppFuncService()
// leox_20120508 Support get/set mute state
void OemAppGetSetVol(void)
{
	// Set volume to OS (Set VolumeValue and then set flag SetVol_F)
	if ((AppReady == 0xFA) && IsFlag1(AppTaskFlag, SetVol_F))
	{	// Issue Q-Event to application for volume set
		AppSetVolume = (VolumeValue < VOL_MAX) ? VolumeValue : VOL_MAX;			// leox_20120424
		ClrFlag(AppTaskFlag, SetVol_F);
		ECQEvent(Q54_SCI, SCIMode_Normal);
	}

	// Set mute state to OS (Set AppGetMute and then set flag SetMute_F)
	if ((AppReady == 0xFA) && IsFlag1(AppTaskFlag, SetMute_F))
	{	// Issue Q-Event to application for set mute state
		ClrFlag(AppTaskFlag, SetMute_F);
		ECQEvent(Q52_SCI, SCIMode_Normal);
	}

	// Get volume/mute of OS (Set flag GetVol_F/GetMute_F and wait updating)
	if ((AppReady == 0xFA) && IsFlag1(AppTaskFlag, GetVol_F + GetMute_F))		// leox_20120417
	{
		if (IsFlag0(AppTaskFlag, WaitVol_F))
		{	// Issue Q-Event to application for get volume value or mute state
			SetFlag(AppTaskFlag, WaitVol_F);
			AppGetVolCnt = T_APP_TASK_CNT;	// leox_20111017
			AppGetVolume = AppGetMute = 0xFF;
			ECQEvent(IsFlag1(AppTaskFlag, GetVol_F) ? Q53_SCI : Q51_SCI, SCIMode_Normal);
		}
		else
		{	// Wait volume/mute updated by application or time-out
			if ((AppGetVolume != 0xFF) || (AppGetMute != 0xFF) || (--AppGetVolCnt == 0))
			{
				if (AppGetVolCnt != 0) {VolumeValue = AppGetVolume;}
				AppGetVolCnt = 0;
				ClrFlag(AppTaskFlag, GetVol_F + WaitVol_F + GetMute_F);
			}
		}
	}

	// Wait handshaking from application (Check is volume/mute updated)
	if ((AppReady == 0xAA) && ((AppGetVolume != 0xFF) || (AppGetMute != 0xFF)))
	{	// If volume was updated, it means that application is exist.
		AppReady = 0xFA;
		VolumeValue = AppGetVolume;
	}

	// Send handshaking to application (Set flag HandshakeToApp/HandshakeMute)
	if ((AppReady == 0xAA) && IsFlag1(AppTaskFlag, HandshakeToApp + HandshakeMute))	// leox_20120417
	{	// Issue Q-Event to application for handshaking by volume get or mute state
		AppGetVolume = AppGetMute = 0xFF;
		ECQEvent(IsFlag1(AppTaskFlag, HandshakeToApp) ? Q53_SCI : Q51_SCI, SCIMode_Normal);
		ClrFlag(AppTaskFlag, HandshakeToApp + HandshakeMute);
	}
}


//------------------------------------------------------------------------------
// Execute tasks by counters
//------------------------------------------------------------------------------
// leox_20120507 Code from OemAppFuncService()
// leox_20120508 Support task for set mute state
void OemAppCntTask(void)
{
	// Volume -
	if ((AppVol0Cnt > 0) && (--AppVol0Cnt == 0))
	{	// Decrease volume by counter without issuing scan code or showing OSD	// leox_20120109
		VolumeValue = (VolumeValue > (VOL_MIN + VOL_STEP)) ? (VolumeValue - VOL_STEP) : VOL_MIN;
		SetFlag(AppTaskFlag, SetVol_F);
	}

	// Volume +
	if ((AppVol1Cnt > 0) && (--AppVol1Cnt == 0))
	{	// Increase volume by counter without issuing scan code or showing OSD	// leox_20120109
		VolumeValue = (VolumeValue < (VOL_MAX - VOL_STEP)) ? (VolumeValue + VOL_STEP) : VOL_MAX;
		SetFlag(AppTaskFlag, SetVol_F);
	}

	// Set mute state
	if ((AppMuteCnt > 0) && (--AppMuteCnt == 0))	// leox_20120410
	{	// Set mute state by counter without issuing scan code or showing OSD	// leox_20120508
		SetFlag(AppTaskFlag, SetMute_F);
		if (AppGetMute == 0xAA)
		{	// Un-mute -> Mute
			AppGetMute = 0xBB;
		}
		else if (AppGetMute == 0xBB)
		{	// Mute -> Un-mute
			AppGetMute = 0xAA;
		}
		else
		{	// Do nothing if mute state unknown
			ClrFlag(AppTaskFlag, SetMute_F);
		}
	}
}


//------------------------------------------------------------------------------
// Execute command requested by application
//------------------------------------------------------------------------------
// leox_20120507 Code from OemAppFuncService()
// leox_20120508 Add command 0Ah-15h for hotkeys issued by application
void OemAppCmd(void)
{
	if (AppCommand == 0x00) {return;}	// leox_20120507

	switch (AppCommand)
	{
	case 0x02:	// ODD LED off
		ODD_LED_off();	// leox_20120817
		break;

	case 0x03:	// ODD LED on
		ODD_LED_on();	// leox_20120817
		break;

	#if	OEM_APP_HOTKEY	// leox_20120508
	case 0x0A:	// Fn + F1
		break;

	case 0x0B:	// Fn + F2
		break;

	case 0x0C:	// Fn + F3
		break;

	case 0x0D:	// Fn + F4
		break;

	case 0x0E:	// Fn + F5
		break;

	case 0x0F:	// Fn + F6
		break;

	case 0x10:	// Fn + F7
		break;

	case 0x11:	// Fn + F8
		break;

	case 0x12:	// Fn + F9
		break;

	case 0x13:	// Fn + F10
		break;

	case 0x14:	// Fn + F11
		break;

	case 0x15:	// Fn + F12
		break;
	#endif	// OEM_APP_HOTKEY

	default:
		break;
	}
	AppCommand = 0x00;
}


//------------------------------------------------------------------------------
// Hook port 66h command 81h for OEM application
//------------------------------------------------------------------------------
// leox_20111014 Create function
void OemAppHookPort66Cmd81(void)
{
	if ((RW_EC_AdrH & 0xC0) == 0x00) {return;}
	Tmp_XPntr = ((RW_EC_AdrH & 0x3F) << 8) | RW_EC_AdrL;
	if ((RW_EC_AdrH & 0xC0) == 0x80) {*Tmp_XPntr = RW_EC_Data;}	// Write EC
	if ((RW_EC_AdrH & 0xC0) == 0x40) {RW_EC_Data = *Tmp_XPntr;}	// Read  EC
	RW_EC_AdrH &= (~0xC0);	// Clear command indicator			// leox_20111017
}


//------------------------------------------------------------------------------
// Pre-procedure for setting volume
//  act: 0: Decrease, 1: Increase, 2: Mute
//------------------------------------------------------------------------------
// leox_20111014 Create function
// leox_20120410 Support volume mute
void SetVolumePre(BYTE act)
{
	if (IsFlag0(POWER_FLAG, system_on)) {return;}
	if (IsFlag0(POWER_FLAG, ACPI_OS)) {return;}
	if ((AppVol0Cnt > 0) || (AppVol1Cnt > 0) || (AppMuteCnt > 0)) {return;}		// leox_20120105	// leox_20120410

	if (act == 0)
	{	// Decrease
		AppVol0Cnt = T_APP_TASK_CNT;
		SetFlag(AppTaskFlag, (AppReady == 0xFA) ? GetVol_F : HandshakeToApp);
	}
	else if (act == 1)
	{	// Increase
		AppVol1Cnt = T_APP_TASK_CNT;
		SetFlag(AppTaskFlag, (AppReady == 0xFA) ? GetVol_F : HandshakeToApp);
	}
	else if (act == 2)
	{	// Set mute state
		AppMuteCnt = T_APP_TASK_CNT;
		SetFlag(AppTaskFlag, (AppReady == 0xFA) ? GetMute_F : HandshakeMute);
	}
}
#endif	// SUPPORT_OEM_APP


//------------------------------------------------------------------------------
// Pulse power button (EC to SB/PCH/FCH)
//------------------------------------------------------------------------------
// leox_20121115 Create function
void PulsePwrBtnToSB(BYTE bMS)
{
	DNBSWON_on();
	DelayXms(bMS);
	DNBSWON_off();
}

void ChkMEFW_OVERRIDE(void)
{
	if (IsFlag1(JUMPERSETTING, MEFW_OVERRIDE))
	{
		MEFW_OVERRIDE_En();
	}
	else
	{
		MEFW_OVERRIDE_Dis();
	}
}

void ECOSleepCheck(void)
{
	if ((IsFlag1(ABAT_MISC2, abat_test)) || (IsFlag1(ABAT_MISC2, Shipmode)))
		return;
	if (IsFlag1(POWER_STAT_CTRL, adapter_in))
	{	// AC in
		if (IsFlag1(POWER_FLAG, power_on))
		{	// S0
			if (IsFlag1(POWER_STAT_CTRL1, AC_CUT))
			{
				if (ABAT_CAP > ECO_SLEEP_CHG)	// leox20150805
				{
					if (IsFlag0(POWER_STAT_CTRL1, lowpower_CHG))
					{
						if (IsFlag1(POWER_STAT_CTRL1, AC_Status))
						{
							ECQEvent(Q20_SCI, SCIMode_Normal);
						}
						ClrFlag(POWER_STAT_CTRL1, AC_Status);
						ClrFlag(ABAT_MISC, abat_ValidFull);
						ClrFlag(ABAT_MISC, force_chgA);
						SetFlag(ABAT_MISC, force_discA);
						OffChargeABT();
						DischargeABAT();
					}
				}
				else
				{
					if (IsFlag0(POWER_STAT_CTRL1, AC_Status))
					{
						ECQEvent(Q20_SCI, SCIMode_Normal);
					}
					SetFlag(POWER_STAT_CTRL1, AC_Status);
					SetFlag(POWER_STAT_CTRL1, lowpower_CHG);
					ClrFlag(ABAT_MISC, force_discA);
				}
			}
			else
			{
				if (IsFlag0(POWER_STAT_CTRL1, AC_Status))
				{
					ECQEvent(Q20_SCI, SCIMode_Normal);
					if (RSOC_FULL_BASE<=SEL_BAT_CAP)			// if battery RSOC bigger then recharge RSOC base
						SetFlag(ABAT_MISC, abat_ValidFull);		// set validfull flag for not recharge
				}
				SetFlag(POWER_STAT_CTRL1, AC_Status);
				ClrFlag(ABAT_MISC, force_discA);
				TurnOffADISCHG();
				ACMODE();
			}
			if (IsFlag1(POWER_STAT_CTRL1, CHG_STP))
			{
				if (ABAT_CAP > ECO_SLEEP_CHG)	// leox20150805
				{
					if (IsFlag0(POWER_STAT_CTRL1, lowpower_CHG))
					{
						ClrFlag(POWER_STAT_CTRL1, CHG_Status);
						ClrFlag(ABAT_MISC, force_chgA);
					}
				}
				else
				{
					SetFlag(POWER_STAT_CTRL1, CHG_Status);
					SetFlag(POWER_STAT_CTRL1, lowpower_CHG);
				}
			}
			else
			{
				if (IsFlag0(POWER_STAT_CTRL1, AC_Status))
				{
					ClrFlag(POWER_STAT_CTRL1, CHG_Status);
				}
				else
				{
					SetFlag(POWER_STAT_CTRL1, CHG_Status);
				}
			}
		}
		else
		{	// S3 or S4/S5
			if((IsFlag0(POWER_FLAG, enter_SUS)))
			{	// S4/S5
//				if ((IsFlag1(POWER_STAT_CTRL1_BRM, ECO_Mode)) && (IsFlag1(ABAT_MISC, abat_ValidFull)) && (USBCHARGEDISCHARG != 0x08))
				if ((IsFlag1(POWER_STAT_CTRL1_BRM, ECO_Mode)) && (IsFlag1(ABAT_MISC, abat_ValidFull)) && (!(IsFlag1(USBCHARGEDISCHARG, USB4_CHARG_Enable) && IsFlag0(USBCHARGEDISCHARG, USB4_CHARG_Enable_ACBAT))))	// leo_20150805 fix coding bug
				{
					ClrFlag(POWER_STAT_CTRL1, AC_Status);
					SetFlag(POWER_STAT_CTRL1, CHG_Status);
					ClrFlag(ABAT_MISC, force_chgA);
					SetFlag(ABAT_MISC, force_discA);
					OffChargeABT();
					DischargeABAT();
				}
				else
				{
					SetFlag(POWER_STAT_CTRL1, AC_Status);
					SetFlag(POWER_STAT_CTRL1, CHG_Status);
					ClrFlag(ABAT_MISC, force_discA);
				}
			}
			else
			{	// S3
				SetFlag(POWER_STAT_CTRL1, AC_Status);
				SetFlag(POWER_STAT_CTRL1, CHG_Status);
				ClrFlag(ABAT_MISC, force_discA);
			}
		}
	}
	else
	{	// AC out
		ClrFlag(POWER_STAT_CTRL1, AC_Status);
		ClrFlag(POWER_STAT_CTRL1, CHG_Status);
		ClrFlag(ABAT_MISC, force_discA);
	}
}

#ifdef	Battery_3Cell_Throttling
void Throttling_3cell_Battery(void)
{
//	if ((ABAT_MODEL == 1) && (IsFlag0(POWER_STAT_CTRL, adapter_in)))
	if ((ABAT_CELLS == 0x31) && (IsFlag0(POWER_STAT_CTRL, adapter_in)))	// leox20150731 24Wh battery is 3S1P
	{
		if (IsFlag0(Device_STAT_CTRL1, Thrott_3cell))
		{
			SetFlag(Device_STAT_CTRL1, Thrott_3cell);
			h_prochot_enable();
		}
	}
	else
	{
		if (IsFlag1(Device_STAT_CTRL1, Thrott_3cell))
		{
			ClrFlag(Device_STAT_CTRL1, Thrott_3cell);
			Check_PROCHOT_Disable();	// leox20150803 Check before disable PROCHOT
		}
	}
}
#endif	//Battery_3Cell_Throttling

#ifdef	SUPPORT_Shipmode
void EnterShipMode(void)
{
	//David modify for battery shutdown mode 2014/07/02
	if ((IsFlag1(ABAT_MISC2, Shipmode)) && (IsFlag0(Shipmode_status, EC_Send)) && (IsFlag0(POWER_STAT_CTRL, adapter_in)))
	{
		SMB_ADDR = 0x16;
		if (IsFlag0(Shipmode_status, Mode_type))	// Ship mode
		{
			SMB_CMD = 0x3A;
			SMB_DATA = 0x74;
			SMB_DATA1 = 0xC5;
			if (IsFlag0(POWER_FLAG, power_on))
			{
				if (IsFlag0(Shipmode_status, LED_Fin))
					return;
				ClrFlag(Shipmode_status, LED_Fin);
				SetFlag(Shipmode_status, EC_Ready);
				if (vOSSMbusWWord())
				{
					SetFlag(Shipmode_status, EC_Send);
					SetFlag(Shipmode_status, Wait_AC);
				}
				#if SUPPORT_EFLASH_FUNC	// leox20150710 Access eFlash	// leox20160322
				eFlashSave(&Shipmode_status, Shipmode_status);
				#endif
			}
		}
		else										// Shut down mode
		{
			SMB_CMD = 0x3B;
			SMB_DATA = 0x78;
			SMB_DATA1 = 0x56;
			if (IsFlag0(POWER_FLAG, power_on))
			{
				if (IsFlag0(Shipmode_status, LED_Fin))
					return;
				ClrFlag(Shipmode_status, LED_Fin);
				SetFlag(Shipmode_status, EC_Ready);
				if (vOSSMbusWWord())
				{
					SetFlag(Shipmode_status, EC_Send);
					SetFlag(Shipmode_status, Wait_AC);
				}
				#if SUPPORT_EFLASH_FUNC	// leox20150710 Access eFlash	// leox20160322
				eFlashSave(&Shipmode_status, Shipmode_status);
				#endif
			}
		}
	}
	//David modify for battery shutdown mode 2014/07/02
}
#endif	//SUPPORT_Shipmode

void PollingCriticalWake(void)
{
	if ((IsFlag0(KETBOARD_STAT_CTRL, Critical_Wake)) || (IsFlag1(POWER_STAT_CTRL, adapter_in)) || (IsFlag0(POWER_FLAG, enter_SUS)))
		return;
	if (ABAT_CAP <= Critical_Wake_Level)
	{
		if (IsFlag0(KETBOARD_STAT_CTRL, WakeByCritical))
		{
			SetFlag(POWER_FLAG, wait_PSW_off);
			SetFlag(KETBOARD_STAT_CTRL, WakeByCritical);
			PSWOnPower();
		}
	}
}

#if CMOSprotect	// leox20150612 Add switch define
void CheckCMOSReset(void)
{
	if ((IsFlag0(CMOSReset_Status, WaitPCH)) && (IsFlag0(CMOSReset_Status, WaitSUSBSUSC)))
	{
		CMOSResetCNT = 0;
		return;
	}
	else if (IsFlag1(CMOSReset_Status, ClearCMOS))
	{
		return;
	}
	CMOSResetCNT++;
	if (CMOSResetCNT > 20)
	{
		ShutDnCause = 0xC0;
		ForceOffPower();
		CLR_CMOS_on();
		SetFlag(CMOSReset_Status, ClearCMOS);
		CMOSResetCNT = 0;
	}
}

void ClearCMOSProgress(void)
{
	if (IsFlag1(CMOSReset_Status, ClearCMOS))
	{
		CMOSResetCNT++;
		if (CMOSResetCNT > 2)
		{
			CheckPower();
			if (IsFlag0(MISC_FLAG, F_CriLow))
			{
				CMOSResetCNT = 0;
				ClrFlag(CMOSReset_Status, WaitSUSBSUSC);
				ClrFlag(CMOSReset_Status, WaitPCH);
				ClrFlag(CMOSReset_Status, ClearCMOS);
				CLR_CMOS_off();
				SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
				PSWOnPower();
			}
		}
	}
}
#endif	// CMOSprotect

// leox20150803 Check before disable PROCHOT
void Check_PROCHOT_Disable(void)
{
	#ifdef	Support_Intel_CPU
	if (IsFlag1(FanControl_Mode00, Thrott_CPU_hot)) {return;}
	#endif
	#ifdef	Battery_3Cell_Throttling
	if (IsFlag1(Device_STAT_CTRL1, Thrott_3cell)) {return;}
	#endif
	#ifdef	OEM_VICMV_PROCHOT	// leox20150731
	if (IsFlag1(VICMVHiFlag, Q42Q43_SCI)) {return;}
	#endif
	#if BAT_DCHG_PROTECT	// leox20150826
	if (IsFlag1(BAT_MISC3, DCHG_Throttling)) {return;}
	#endif
	#if BAT_MBATV_PROTECT	// leo_20150916 add power protect	// leox20150916
	if (IsFlag1(VICMVHiFlagACDC, AC_OC_Prochot | DC_OW_Prochot | AC_OCP_Prochot | DC_OCP_Prochot)) {return;}
	#endif
	h_prochot_disable();
}
