/*------------------------------------------------------------------------------
 * Title : OEM_DIMM.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_DIMM
void S3S4S5toS0_OEM_DIMM(void)
{
	LED_ON_on();
	#ifdef	SW_OEM_DIMM
	Led_Data = LedBackup;
	OEM_Write_Leds(Led_Data);
	#endif	//SW_OEM_DIMM
}


void VerifyDimmerFunction(void)
{
	if (IsFlag1(POWER_STAT_CTRL, DIMM_OFF_Flag))
	{
		LED_ON_on();
		#ifdef	SW_OEM_DIMM
		Led_Data = LedBackup;
		OEM_Write_Leds(Led_Data);
		#endif	//SW_OEM_DIMM
	}
	else
	{
		LED_ON_off();
		#ifdef	SW_OEM_DIMM
		SW_DIMM_Function_AllLedsoff();
		#endif	//SW_OEM_DIMM
	}
}


void SW_DIMM_Function_AllLedsoff(void)
{
	NUMLED_off();
	SCROLLLED_off();
	CAPSLED_off();
}
#endif	//OEM_DIMM

