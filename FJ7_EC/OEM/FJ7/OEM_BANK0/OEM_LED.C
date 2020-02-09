/*------------------------------------------------------------------------------
 * Title: OEM_LED.C
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
// Function: OEM_Write_Leds
//
// Write to SCROLL LOCK, NUM LOCK, CAPS LOCK, and any custom LEDs
// (including an "Fn" key LED).
//
// Input: data (LED_ON bit) = Data to write to the LEDs
//         bit 0 - SCROLL, Scroll Lock
//         bit 1 - NUM,    Num Lock
//         bit 2 - CAPS,   Caps Lock
//         bit 3 - OVL,    Fn LED
//         bit 4 - LED4,   Undefined
//         bit 5 - LED5,   Undefined
//         bit 6 - LED6,   Undefined
//         bit 7 - LED7,   Undefined
//
// Updates Led_Data variable and calls send_leds if appropriate.
//------------------------------------------------------------------------------
void OEM_Write_Leds(BYTE data_byte)
{
	Led_Data = data_byte;

	if (data_byte & BIT0)
	{
		SCROLLLED_on();
	}
	else
	{
		SCROLLLED_off();
	}

	if (data_byte & BIT1)
	{
		NUMLED_on();
	}
	else
	{
		NUMLED_off();
	}

	if (data_byte & BIT2)
	{
		CAPSLED_on();
	}
	else
	{
		CAPSLED_off();
	}
}


//------------------------------------------------------------------------------
// LED Pattern
//------------------------------------------------------------------------------
#define	BATLED_LEN	6
#define	PWRLED_LEN	8

const WORD code BattLEDPattern[] =
{
//David modify for battery shutdown mode 2014/07/02
//  0       1       2       3       4       5
//	BAT_FUL BAT_CHG BAT_BAD WARNING SHUTDOWN SHIP		// FH8A     FH9C
	0x0000, 0xFFFF, 0xAAAA, 0x1041, 0x001B, 0xFFFF,		// AMBER    ORANGE
	0xFFFF, 0x0000, 0x5555, 0x0000, 0x001B, 0xFFFF,		// BLUE     WHITE
//David modify for battery shutdown mode 2014/07/02

};

const WORD code PowerLEDPattern[] =
{
//  0       1       2       3       4       5       6       7
//	S0(<8%) S3(<8%) S0(<3%) S3(<3%) S0(>8%) S3(>8%) S0(Dim) S3(Qmd)		// FH8A     FH9C
	0xFFFF, 0x1041, 0xFFFF, 0x1041, 0xFFFF, 0x1041, 0x5555, 0x0000,		// BLUE     WHITE
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF,		// AMBER    ORANGE
	0xFFFF, 0x1041, 0xFFFF, 0x1041, 0xFFFF, 0x1041, 0x5555, 0x0000,		// BLUE2    WHITE2

};

#ifdef	Support_Wake_On_WLan
const WORD code RFLEDPattern[] =
{
	//--------------------------------------------------------------------------
	//  S0      S3/S4		// FH8A     FH9C
	//--------------------------------------------------------------------------
		0xFFFF, 0x1001,		// BLUE2    WHITE
};
#endif

//------------------------------------------------------------------------------
// Turn on/off LEDs according to machine status.
// Executed once per 1 sec
// For example:
//       ->BIT14
//      1010101010101010
//      0101010101010101
//                <-BIT3
//------------------------------------------------------------------------------
void ForceProcessLED(void)
{
	#ifdef	OEM_BATTERY	// leox_20111006
	BYTE bat_index = 0xFF;	// Set battery LED off as default
	#endif
	BYTE pwr_index = 0xFF;	// Set power LED off as default
	#ifdef	Support_Wake_On_WLan
	BYTE lan_index = 0xFF;	// Set Wlan LED off as default
	#endif

	LED_FLASH_CNT <<= 1;
	if (LED_FLASH_CNT == 0x1000) {LED_FLASH_CNT = 0x0001;}

	#ifdef	Support_Wake_On_WLan
	// WLan LED control method
	if (IsFlag1(Device_STAT_CTRL, Lan_Status))
	{
		if (IsFlag1(POWER_FLAG, power_on))
		{
			// System in S0
			lan_index = 0;
		}
		else
		{
			// System in S3/S4
			if (IsFlag1(POWER_STAT_CTRL1, WakeOnWLan))
			{
				if ((IsFlag1(POWER_FLAG, enter_SUS)) || (IsFlag1(POWER_STAT_CTRL, enter_S4)))
					lan_index = 1;
			}
			// System in S5
			else
			{
				//OffRFLED
			}
		}
	}
	else
	{
		//OffRFLED
	}
	if (lan_index != 0xFF)
	{
		if (RFLEDPattern[lan_index] & LED_FLASH_CNT)
		{
			WLAN_LED_on();
		}
		else
		{
			WLAN_LED_off();
		}
	}
	else
	{
		WLAN_LED_off();
	}
	#endif

	// Power LED control method
	#ifdef	OEM_BATTERY	// leox_20111006
	if (IsFlag1(ABAT_STATUS, abat_low))
	{	// BateryBelow3Percent
		if (IsFlag1(POWER_FLAG, enter_SUS))
		{	// BateryBelow3Percent_S3
			if (IsFlag1(POWER_FLAG2, Quick_S3))
			{
				pwr_index = 7;
			}
			else
			{
				pwr_index = 3;
			}
		}
		else
		{
			if (IsFlag1(POWER_FLAG, power_on))
			{	// BateryBelow3Percent_S0
				if (IsFlag1(POWER_FLAG2, No_Dimm))
				{
					pwr_index = 6;
				}
				else
				{
					pwr_index = 2;
				}
			}
			else
			{
				// OffPWRLED
			}
		}
	}
	else
	{
		if (IsFlag1(POWER_FLAG2, bat_warning))
		{	// BateryBelow8Percent
			if (IsFlag1(POWER_FLAG, enter_SUS))
			{	// BateryBelow8Percent_S3
				if (IsFlag1(POWER_FLAG2, Quick_S3))
				{
					pwr_index = 7;
				}
				else
				{
					pwr_index = 1;
				}
			}
			else
			{
				if (IsFlag1(POWER_FLAG, power_on))
				{	// BateryBelow8Percent_S0
					if (IsFlag1(POWER_FLAG2, No_Dimm))
					{
						pwr_index = 6;
					}
					else
					{
						pwr_index = 0;
					}
				}
				else
				{
					// OffPWRLED
				}
			}
		}
		else
		{	// BateryAbove8Percent
			if (IsFlag1(POWER_FLAG, enter_SUS))
			{	// BateryAbove8Percent_S3
				if (IsFlag1(POWER_FLAG2, Quick_S3))
				{
					pwr_index = 7;
				}
				else
				{
					pwr_index = 5;
				}
			}
			else
			{
				if (IsFlag1(POWER_FLAG, power_on))
				{	// BateryAbove8Percent_S0
					if (IsFlag1(POWER_FLAG2, No_Dimm))
					{
						pwr_index = 6;
					}
					else
					{
						pwr_index = 4;
					}
				}
				else
				{
					// OffPWRLED
				}
			}
		}
	}
	#else	// OEM_BATTERY	// leox_20111006
	if (IsFlag1(POWER_FLAG, enter_SUS))
	{
		pwr_index = 5;
	}
	else
	{
		if (IsFlag1(POWER_FLAG, power_on))
		{
			pwr_index = 4;
		}
		else
		{
			// OffPWRLED
		}
	}
	#endif	// OEM_BATTERY

	if (IsFlag0(POWER_FLAG2, No_Dimm))
	{
		if (pwr_index != 0xFF)
		{
			if (PowerLEDPattern[pwr_index] & LED_FLASH_CNT)
			{
				PWRLED0_on();
			}
			else
			{
				PWRLED0_off();
			}
			if (PowerLEDPattern[PWRLED_LEN + pwr_index] & LED_FLASH_CNT)
			{
				PWRLED1_on();
			}
			else
			{
				PWRLED1_off();
			}

			if (PowerLEDPattern[PWRLED_LEN + PWRLED_LEN + pwr_index] & LED_FLASH_CNT)
			{
				PWRLED2_on();
			}
			else
			{
				PWRLED2_off();
			}
		}
		else
		{
			PWRLED0_off();
			PWRLED1_off();
			PWRLED2_off();
		}
	}

	// Battery LED control method
	#ifdef	OEM_BATTERY	// leox_20111006
	if (IsFlag1(ACHARGE_STATUS, abat_InCharge))
	{	// BATT in charge
		ACMODE();
		bat_index = 1;
	}
	else
	{
		if ((IsFlag1(ABAT_STATUS, abat_full) || IsFlag1(ABAT_MISC, abat_ValidFull)) && IsFlag1(POWER_STAT_CTRL, adapter_in) && IsFlag1(POWER_FLAG, power_on))
		{	// battery full
			bat_index = 0;
		}
		else
		{
			#ifdef	 NECP_BattRefresh_Support
			if (IsFlag1(ABAT_MISC, bat_StopChg))
			{
				bat_index = 0;
			}
			else
			{
				if (IsFlag1(ABAT_STATUS, abat_destroy))
				{
					bat_index = 2;
				}
				else
				{
					// OffBATLED
				}
			}
			#else	//NECP_BattRefresh_Support
			if (IsFlag1(ABAT_STATUS, abat_destroy))
			{
				if (IsFlag1(POWER_FLAG, power_on))
				{
					bat_index = 2;
				}
				else
				{
					// OffBATLED
				}
			}
			else
			{
				if (IsFlag1(POWER_FLAG2, bat_warning))
				{
					if ((IsFlag1(POWER_FLAG, enter_SUS)) || (IsFlag1(POWER_FLAG, power_on)))
					{
						bat_index = 3;
					}
				}
				//OffBATLED
			}
			#endif	//NECP_BattRefresh_Support
		}
	}
	//David modify for battery shutdown mode 2014/07/02
	if ((IsFlag0(POWER_FLAG, power_on)) && (IsFlag1(ABAT_MISC2, Shipmode)) && (IsFlag0(POWER_STAT_CTRL, adapter_in)))
	{
		if (IsFlag1(Shipmode_status, Mode_type))	// Shut down mode
			bat_index = 4;
		else										// Ship mode
			bat_index = 5;
	}
	//David modify for battery shutdown mode 2014/07/02

	if (bat_index != 0xFF)
	{
		if (BattLEDPattern[bat_index] & LED_FLASH_CNT)
		{
			BATLED0_on();
		}
		else
		{
			BATLED0_off();
		}
		if (BattLEDPattern[BATLED_LEN + bat_index] & LED_FLASH_CNT)
		{
			BATLED1_on();
		}
		else
		{
			BATLED1_off();
		}
	}
	else
	{
		BATLED0_off();
		BATLED1_off();
	}

	//David modify for battery shutdown mode 2014/07/02
	if ((IsFlag0(POWER_FLAG, power_on)) && (IsFlag1(ABAT_MISC2, Shipmode)) && (IsFlag0(POWER_STAT_CTRL, adapter_in)))
	{
//		if (LED_FLASH_CNT == 0020)
		if (LED_FLASH_CNT == 0x0020)	// leox20150806 Fix LED bug for battery shutdown mode
		{
			SetFlag(Shipmode_status, LED_Fin);
			#if SUPPORT_EFLASH_FUNC		// leox20160322
			eFlashSave(&Shipmode_status, Shipmode_status);
			#endif
		}
	}
	//David modify for battery shutdown mode 2014/07/02
	#endif	// OEM_BATTERY
}


//------------------------------------------------------------------------------
// Process LED
//------------------------------------------------------------------------------
void ProcessLED(void)
{
	if (IsFlag1(POWER_FLAG2, func_LED)) {ForceProcessLED();}
}


//------------------------------------------------------------------------------
// Turn on all LED
//------------------------------------------------------------------------------
void TurnOnAllLED(void)
{
	PWRLED0_on();
	PWRLED1_on();
	BATLED0_on();
	BATLED1_on();
	NUMLED_on();
	CAPSLED_on();
	SCROLLLED_on();
	SATA_LED_BBO_Dis_On();
}


//------------------------------------------------------------------------------
// Turn off all LED
//------------------------------------------------------------------------------
void TurnOffAllLED(void)
{
	PWRLED0_off();
	PWRLED1_off();
	BATLED0_off();
	BATLED1_off();
	NUMLED_off();
	CAPSLED_off();
	SCROLLLED_off();
	SATA_LED_BBO_Dis_Off();
}


//------------------------------------------------------------------------------
// LED debug
//------------------------------------------------------------------------------
// leox_20120406 Code from ACPI_Cmd_8EData() of "OEM_ACPI.C"
void DebugLED(BYTE val)
{
	ClrFlag(POWER_FLAG2, func_LED);	// Switch to debug mode

	// Turn off all LEDs first
	BATLED0_off();		// Blue
	BATLED1_off();		// Amber
	PWRLED0_off();		// Blue
	PWRLED1_off();		// Amber
	PWRLED2_off();		// BTN Blue
	CAPSLED_off();
	NUMLED_off();
	WLAN_LED_off();		// RF LED off
	SATA_LED_BBO_Dis_Off();	// SATA LED

	// Turn on particular LED
	switch (val)
	{
	case 0:				// Reserved for all LEDs off (The same as case 254)
		break;

	case 1:
		BATLED1_on();	// Blue
		break;

	case 2:
		BATLED0_on();	// Amber
		break;

	case 3:
		PWRLED0_on();	// Blue
		break;

	case 4:
		PWRLED1_on();	// Amber
		break;

	case 5:
		//PWRLED2_on();	// BTN Blue
		SATA_LED_BBO_Dis_On();
		break;

	case 6:
		CAPSLED_on();
		break;

	case 7:
		NUMLED_on();
		break;

	case 8:
		WLAN_LED_on();	// RF LED
		break;

	case 9:
		BATLED0_on();	// Blue
		BATLED1_on();	// Amber
		PWRLED0_on();	// Blue
		PWRLED1_on();	// Amber
		PWRLED2_on();	// BTN Blue
		CAPSLED_on();
		NUMLED_on();
		WLAN_LED_on();	// RF LED on
		SATA_LED_BBO_Dis_On();
		break;

	default:
		break;
	}
}


//------------------------------------------------------------------------------
// Exit LED debug mode
//------------------------------------------------------------------------------
// leox_20120524 Code from ACPI_Cmd_8F() of "OEM_ACPI.C"
void ExitDebugLED(void)
{
	ECO_LED0_off();		// ECO LED off
	ECO_LED1_off();		// ECO LED1 off
	SetFlag(POWER_FLAG2, func_LED);
	ForceProcessLED();
	Led_Data = LedBackup;
	OEM_Write_Leds(Led_Data);
	SATA_LED_BBO_En();
}


void DimmLEDProcess(void)
{
	if (IsFlag1(POWER_FLAG2, No_Dimm))
	{
		if (DIMM_LED_Status == 0)
		{
			DIMM_LED_Status = 1;
			PWRLED0_on();
			PWRLED2_on();
		}
		else
		{
			DIMM_LED_Status = 0;
			PWRLED0_off();
			PWRLED2_off();
		}
	}
}


//------------------------------------------------------------------------------
// LED debug 2	// leox20150721 LED function
//------------------------------------------------------------------------------
// leox20150721 Create function
void DebugLED2(BYTE val)
{
	// Switch to debug mode
	ClrFlag(POWER_FLAG2, func_LED);

	// Turn off all LEDs first
	BATLED0_off();		// Blue
	BATLED1_off();		// Amber
	PWRLED0_off();		// Blue
	PWRLED1_off();		// Amber
	PWRLED2_off();		// BTN Blue
	CAPSLED_off();
	NUMLED_off();
	WLAN_LED_off();		// RF LED off
	SATA_LED_BBO_Dis_Off();

	// Turn on specific LEDs
	if (IS_BIT_SET(val, 0)) {BATLED0_on();}		// Blue
	if (IS_BIT_SET(val, 1)) {BATLED1_on();}		// Amber
	if (IS_BIT_SET(val, 2)) {PWRLED0_on();}		// Blue
	if (IS_BIT_SET(val, 3)) {PWRLED1_on();}		// Amber
	//if (IS_BIT_SET(val, 4)) {PWRLED2_on();}		// BTN Blue
	if (IS_BIT_SET(val, 4)) {SATA_LED_BBO_Dis_On();}
	if (IS_BIT_SET(val, 5)) {CAPSLED_on();}
	if (IS_BIT_SET(val, 6)) {NUMLED_on();}
	if (IS_BIT_SET(val, 7)) {WLAN_LED_on();}	// RF LED on
}
