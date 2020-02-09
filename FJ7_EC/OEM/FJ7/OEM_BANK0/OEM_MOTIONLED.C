/*------------------------------------------------------------------------------
 * Title : OEM_MOTIONLED.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_MotionLed
void EnableMotionLedsFunction(void)
{
	SetFlag(MotionLedsFunctionFlag, EnableMotionLedsFun);
	MotionLedsFunctionCNT = 0;
}


void MotionLedsFunctionNUMon(void)
{
	NUMLED_on();
	SCROLLLED_off();
	CAPSLED_off();
	MotionLedsFunctionCNT++;
}


void MotionLedsFunctionSCRon(void)
{
	NUMLED_off();
	SCROLLLED_on();
	CAPSLED_off();
	MotionLedsFunctionCNT++;
}


void MotionLedsFunctionCAPSon(void)
{
	NUMLED_off();
	SCROLLLED_off();
	CAPSLED_on();
	MotionLedsFunctionCNT++;
}


void MotionLedsFunctionAllLedson(void)
{
	NUMLED_on();
	SCROLLLED_on();
	CAPSLED_on();
	MotionLedsFunctionCNT++;
}


void MotionLedsFunctionAllLedsoff(void)
{
	NUMLED_off();
	SCROLLLED_off();
	CAPSLED_off();
	MotionLedsFunctionCNT++;
}


void MotionLedsFunctionCAPSonNUMon(void)
{
	NUMLED_on();
	SCROLLLED_off();
	CAPSLED_on();
	MotionLedsFunctionCNT++;
}


void NECP_MotionLedsFunction(void)
{
	if ((IsFlag1(MotionLedsFunctionFlag, EnableMotionLedsFun)) && (MotionLedsFunctionCNT != 10))
	{
		switch (MotionLedsFunctionCNT)
		{
		case 0:
			MotionLedsFunctionCNT++;
			break;

		case 1:
			MotionLedsFunctionCAPSonNUMon();
			break;

		case 2:
			MotionLedsFunctionSCRon();
			break;

		case 3:
			MotionLedsFunctionCAPSonNUMon();
			break;

		case 4:
			MotionLedsFunctionSCRon();
			break;

		case 5:
			MotionLedsFunctionCAPSonNUMon();
			break;

		case 6:
			MotionLedsFunctionSCRon();
			break;

		case 7:
			MotionLedsFunctionCAPSonNUMon();
			break;

		case 8:
			MotionLedsFunctionAllLedsoff();
			break;

		case 9:
			ClrFlag(MotionLedsFunctionFlag, EnableMotionLedsFun);
			Led_Data = LedBackup;
			OEM_Write_Leds(Led_Data);
			MotionLedsFunctionCNT++;
			break;

		default:
			break;
		}
	}
}
#endif	//OEM_MotionLed

