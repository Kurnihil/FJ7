/*------------------------------------------------------------------------------
 * Title : OEM_IPOD.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_IPOD
// leo_20150917 USB charging +++
void USBChg_SDP(void)
{
	USB_CHARGE_CTL1_HI();
	USB_CHARGE_ILIM_LO();
	USBChg_Status = 1;
}


void USBChg_CDP(void)
{
	USB_CHARGE_CTL1_HI();
	USB_CHARGE_ILIM_HI();
	USBChg_Status = 2;
}


void USBChg_DCP(void)
{
	USB_CHARGE_CTL1_LO();
	USB_CHARGE_ILIM_HI();
	USBChg_Status = 3;
}
// leo_20150917 USB charging ---


// leox20150707 For USB charger SLG55544 of FH9C
// leo_20150917 Rewrite function	// leo_20150917 USB charging
// leox20150918 Rename USB4_CHARGE_EnDis() to USBChg_SDP_CDP()
void USBChg_SDP_CDP(void)
{
	if (IsFlag0(USBCHARGEDISCHARG_BRM, USB4_CHARG_FastMode_BRAM))
	{	// SDP for Normal charge mode	(Normal mode)
		USBChg_SDP();
	}
	else
	{	// CDP for Boost charge mode	(Fast mode)
		USBChg_CDP();
	}
}


void EnableUSBCHARGPORTFun(void)
{
	SetFlag(USBCHARGFLAG, USBCHARGPORTENABLE);
}


void DisableUSBCHARGPORTFun(void)
{
	ClrFlag(USBCHARGFLAG, USBCHARGPORTENABLE);
}


void CheckUSBchargingEnDis(void)
{
	if (IsFlag1(USBCHARGFLAG, USBCHARGPORTENABLE))
	{
		if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
		{
			#ifdef	OEM_IPOD_ACDC
			if (AC_IN_read())
			{
				USBDischargingS0ToS4S5Init();
			}
			else
			{
				if (IsFlag0(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_ACBAT_BRAM))
				{
					USBChargeFunctionOnlyS4S5();
				}
				else
				{
					#ifdef	OEM_BATTERY
					// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
					SMB_ADDR_OEM = _SMB_BatteryAddr;
					SMB_CMD_OEM = _CMD_RelativeStateOfCharge;
					// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
					if (vOSSMbusRByte_OEM())
					{
						Round_Off_RSOC();
						if (SMB_DATA_OEM >= USBCHARGEDThrldofcapacity)
						{
							USBDischargingS0ToS4S5Init();
						}
						else
						{
							USBChargeFunctionOnlyS4S5();
						}
					}
					#endif	//OEM_BATTERY
				}
			}
			#else	//OEM_IPOD_ACDC
			if (AC_IN_read())
			{
				USBDischargingS0ToS4S5Init();
			}
			else
			{
				USBChargeFunctionOnlyS4S5();
			}
			#endif	//OEM_IPOD_ACDC
		}
		else
		{
			USBChargeFunctionOnlyS4S5();
		}
	}
}


void USBDischargingS0ToS4S5Init(void)
{
	if (IsFlag0(ToSBsignals00, SB_PowerSource))
	{
		STBON_on();
	}
	
	USBWorkingCNT = 50;
	USB_CB0_CNT = 50;
	USBChg_SDP_CDP();		// leox20150707 For USB charger SLG55544	// leox20150918
//	USB_CHARGE_CTL1_HI();	// CB0 high	// leo_20150917 USB charging

	DelayXms(2);
	USB4_CHARGE_on();		// USB_CHARGE# low

	DelayXms(2);
	USB4S5_DISCHARGE_on();	// USB_DISCHARGE high
}


void USBChargeFunctionOnlyS4S5(void)
{
	if ((IsFlag0(POWER_FLAG, enter_SUS)) || (IsFlag1(USB_CutOffpower, CutOffUSBP_IPOD)) ||
		(IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM)))
	{
		USBDischargingS0ToS4S5InitDis();
	}
}


void USBDischargingS0ToS4S5InitDis(void)
{
	if ((AC_IN_read()) || (IsFlag0(POWER_FLAG, enter_SUS)) || (IsFlag1(USB_CutOffpower, CutOffUSBP_IPOD)))
	{
		USBDischargingS0ToS4S5InitDis0();
	}
	else
	{
		#ifdef	OEM_IPOD_ACDC
		if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_ACBAT_BRAM))
		{
			USBDischargingS0ToS4S5InitDis1();
		}
		else
		{
			if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
			{
				InitUSBChargePort();
			}
			else
			{
				USBDischargingS0ToS4S5InitDis0();
			}
		}
		#else	//OEM_IPOD_ACDC
		if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
		{
			InitUSBChargePort();
		}
		else
		{
			USBDischargingS0ToS4S5InitDis0();
		}
		#endif	//OEM_IPOD_ACDC
	}
}


void USBDischargingS0ToS4S5InitDis0(void)
{
	InitUSBChargePortCNT = 0;
	USBDischargingS0ToS4S5InitDis2();
}


void USBDischargingS0ToS4S5InitDis1(void)
{
	#ifdef	OEM_BATTERY
	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_RelativeStateOfCharge;
	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
	if (vOSSMbusRByte_OEM())
	{
		Round_Off_RSOC();
		if (SMB_DATA_OEM >= USBCHARGEDThrldofcapacity)
		{
			USBDischargingS0ToS4S5InitDis0();
		}
		else
		{
			InitUSBChargePort();
		}
	}
	#endif	//OEM_BATTERY
}


// leo_20150917 Rewrite function		// leo_20150917 USB charging
void USBDischargingS0ToS4S5InitDis2(void)
{
	if (IsFlag0(POWER_FLAG, enter_SUS))	// leo_20150715 support SLG55544 enable DCP S3 wake when AC mode enable
	{
		USB4_CHARGE_off();	// USB_CHARGE# high
		USBChg_DCP();
	}
	else
	{
//		if (SMB_DATA_OEM < USBCHARGEDThrldofcapacity)
		if (SEL_BAT_CAP < USBCHARGEDThrldofcapacity)	// leox20150918
		{
			if (IsFlag0(USBCHARGEDISCHARG_BRM, USB4_CHARG_FastMode_BRAM))
			{
				USBChg_SDP();
			}
			else
			{
				USB4_CHARGE_off();
			}
		}
		else
		{
			USBChg_SDP();
		}
	}
	USB4S5_DISCHARGE_on();	// USB_DISCHARGE high
	USBWorkingCNT = 0;
}

void InitUSBChargePort(void)
{
	if (InitUSBChargePortCNT >= 3)
	{
		USB4_CHARGE_on();
		USB4S5_DISCHARGE_off();
		USBChg_SDP_CDP();		// leox20150707 For USB charger SLG55544		// leox20150918
//		USB_CHARGE_CTL1_HI();	// CB0 high	// leo_20150917 USB charging
		USBWorkingCNT = 0;
	}
	else
	{
		InitUSBChargePortCNT++;
		USBDischargingS0ToS4S5InitDis2();
	}
}


void PowerOnInitUSBChargePort(void)
{
	USB4_CHARGE_on();		// USB_CHARGE# low
	USB4S5_DISCHARGE_off();	// USB_DISCHARGE low
	USBChg_SDP_CDP();		// leox20150707 For USB charger SLG55544	// leox20150918
//	USB_CHARGE_CTL1_HI();	// CB0 high	// leo_20150917 USB charging
	USBWorkingCNT = 0;
	USB_CB0_CNT = 0;
	USB_CHARGE_CNT = 0;
	if (SYSTEMSTATE == 0x31) {USBDischargingS4S5ToS0Init();}
}


void USBDischargingS4S5ToS0Init(void)
{
	if ((IsFlag1(USBCHARGFLAG, USBCHARGPORTENABLE)) && (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM)))
	{	// S4/S5 to S0 USB Charging/Discharging function initial
		USBWorkingCNT = 145;
		USB4_CHARGE_on();		// USB_CHARGE# low
		DelayXms(2);
		USB4S5_DISCHARGE_on();	// USB_DISCHARGE high
		DelayXms(2);
		USBChg_SDP_CDP();		// leox20150707 For USB charger SLG55544		// leox20150918
//		USB_CHARGE_CTL1_HI();	// CB0 high	// leo_20150917 USB charging
	}
}


void CheckUSBWorkingCNT(void)
{
	if (IsFlag1(USBCHARGFLAG, USBCHARGPORTENABLE))
	{
		if (USBWorkingCNT != 0)
		{
			USBWorkingCNT--;
			if (USBWorkingCNT == 0)
			{
				USB4S5_DISCHARGE_off();	// USB_DISCHARGE low
				DelayXms(2);
				USB4_CHARGE_on();		// USB_CHARGE# low
			}
		}

		if (USB_CB0_CNT != 0)
		{
			USB_CB0_CNT--;
			if (USB_CB0_CNT == 0)
			{
				//USB_CHARGE_ILIM_LO();	// CB1 low
				//USB_CHARGE_CTL1_LO();	// CB0 low
				USBChg_DCP();			// leo_20150917 USB charging
				if (IsFlag0(POWER_FLAG, enter_SUS))
				{
					USB_CHARGE_CNT = 50;
				}
			}
		}

		if (USB_CHARGE_CNT != 0)
		{
			USB_CHARGE_CNT--;
			if (USB_CHARGE_CNT == 0)
			{
				USB4_CHARGE_off();
				USBWorkingCNT = 150;
			}
		}

		if (IsFlag1(POWER_FLAG, enter_SUS))
		{
			// leo_20150918 USB charging +++
			if ((SEL_BAT_CAP < USBCHARGEDThrldofcapacity) &&
				IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_FastMode_BRAM) &&
				IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
			{
				return;
			}
			// leo_20150918 USB charging ---
			if (CEN_L_Low())
			{
				USB4_CHARGE_on();
			}
			else
			{
				USB4_CHARGE_off();
			}
		}
	}
}


void USBHookInit(void)
{
	DelayChkIdleCNT = 5;
	SetFlag(USBCHARGFLAG, USBCHARGPORTENABLE);
	if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
	{
		#ifdef	OEM_IPOD_ACDC
		if (AC_IN_read())
		{
			DefaultUSBCharging();
		}
		else
		{
			if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_ACBAT_BRAM))
			{
				#ifdef	OEM_BATTERY
				// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
				SMB_ADDR_OEM = _SMB_BatteryAddr;
				SMB_CMD_OEM = _CMD_RelativeStateOfCharge;
				// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
				if (vOSSMbusRByte_OEM())
				{
					Round_Off_RSOC();
					if (SMB_DATA_OEM >= USBCHARGEDThrldofcapacity) {DefaultUSBCharging();}
				}
				#endif	//OEM_BATTERY
			}
		}
		#else	//OEM_IPOD_ACDC
		if (AC_IN_read()) {DefaultUSBCharging();}
		#endif	//OEM_IPOD_ACDC
	}
}


void DefaultUSBCharging(void)
{
	if (IsFlag0(ToSBsignals00, SB_PowerSource))
	{
		STBON_on();
	}
	//USB_CHARGE_ILIM_LO();	// CB1 low
	//USB_CHARGE_CTL1_LO();	// CB0 low
	USBChg_DCP();			// leo_20150917 USB charging
	USB4_CHARGE_on();		// USB_CHARGE# low
	USB4S5_DISCHARGE_off();	// USB_DISCHARGE low
}


BYTE HookChkIdle_OEM_IPOD(void)
{
	if (IsFlag0(USBCHARGFLAG, ChkUSBchgFunBeforeIdle))
	{
		SetFlag(USBCHARGFLAG, ChkUSBchgFunBeforeIdle);
		CheckUSBchargingEnDis();
	}

	if (DelayChkIdleCNT != 0)
	{
		DelayChkIdleCNT--;
		return 1;
	}

	#ifdef	OEM_IPOD_ACDC
	if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_ACBAT_BRAM))
	{
		#ifdef	OEM_BATTERY
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
		SMB_ADDR_OEM = _SMB_BatteryAddr;
		SMB_CMD_OEM = _CMD_RelativeStateOfCharge;
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
		if (vOSSMbusRByte_OEM())
		{
			Round_Off_RSOC();
			if (SMB_DATA_OEM >= USBCHARGEDThrldofcapacity - 1)
			{
				return 1;
			}
			else
			{
				if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
				{
					if (AC_IN_read())
					{
						return (1);
					}
					else
					{
						USBDischargingS0ToS4S5InitDis();
					}
				}
			}
		}
		#endif	//OEM_BATTERY
	}
	else
	{
		if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
		{
			if (AC_IN_read())
			{
				return (1);
			}
			else
			{
				USBDischargingS0ToS4S5InitDis();
			}
		}
	}
	#else	//OEM_IPOD_ACDC
	if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
	{
		if (AC_IN_read())
		{
			return (1);
		}
		else
		{
			USBDischargingS0ToS4S5InitDis();
		}
	}
	#endif	//OEM_IPOD_ACDC

	return 0;
}


void ConfigACIn_OEM_IPOD(void)
{
	InitUSBChargePortCNT = 0;
	if (IsFlag0(POWER_FLAG, power_on)) {CheckUSBchargingEnDis();}
}


void ConfigACOut_OEM_IPOD(void)
{
	if (IsFlag0(POWER_FLAG, power_on)) {CheckUSBchargingEnDis();}
}


void S3S4S5toS0_OEM_IPOD(void)
{
	if (SYSTEMSTATE != 0x31)
	{
		USB4_CHARGE_off();		// USB_CHARGE# high
		DelayXms(2);
		USB4S5_DISCHARGE_on();	// USB_DISCHARGE high
		DelayXms(2);
	}
	else
	{
		if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
		{
			USB4_CHARGE_off();		// USB_CHARGE# high
			DelayXms(2);
			USB4S5_DISCHARGE_on();	// USB_DISCHARGE high
			DelayXms(2);
		}
	}
}


// leox20150914 For USB charger SLG55544 to set SDP/CDP in S0 when BIOS set
// leox20150918 Rewrite function refer to "USB-Charge_Rev04.xlsx"
void USBChargeFunctionS0(void)
{
	if (IsFlag0(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_BRAM))
	{		// Anytime USB Charge: Disabled
		USBChg_SDP();
	}
	else
	{
		if (IsFlag1(USBCHARGEDISCHARG_BRM, USB4_CHARG_Enable_ACBAT_BRAM))
		{	// Anytime USB Charge: AC + Battery
			if (IsFlag1(POWER_STAT_CTRL, adapter_in))
			{		// AC only or AC+DC
				USBChg_SDP_CDP();		// leox20150918
			}
			else
			{		// DC only
				if (SEL_BAT_CAP >= USBCHARGEDThrldofcapacity)
				{		// Battery RSOC >= 13%
					USBChg_SDP_CDP();	// leox20150918
				}
				else
				{		// Battery RSOC <= 12%
					USBChg_SDP();
				}
			}
		}
		else
		{	// Anytime USB Charge: AC Only
			if (IsFlag1(POWER_STAT_CTRL, adapter_in))
			{		// AC only or AC+DC
				USBChg_SDP_CDP();		// leox20150918
			}
			else
			{		// DC only
				USBChg_SDP();
			}
		}
	}
}
#endif	//OEM_IPOD
