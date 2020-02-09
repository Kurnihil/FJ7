/*------------------------------------------------------------------------------
 * Title : OEM_TP.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_TP
//------------------------------------------------------------------------------
// Disable Aux device
//------------------------------------------------------------------------------
void OEM_CmdA7_DAux(void)
{
	if (~Ccb42_DISAB_AUX) {Ccb42_DISAB_AUX = 1;}	// Disable auxiliary device (mouse)
}


//------------------------------------------------------------------------------
// Enable Aux device
//------------------------------------------------------------------------------
void OEM_CmdA8_EAux(void)
{
	if (Ccb42_DISAB_AUX) {Ccb42_DISAB_AUX = 0;}		// Enable aux device (mouse)
}


//------------------------------------------------------------------------------
// Check OEM touchpad status
//------------------------------------------------------------------------------
void CheckOemTouchpadStatus(void)
{
	if (IsFlag1(POWER_FLAG, ACPI_OS))
	{
		if (TouchpadSWFnDebounce != 0)
		{
			TouchpadSWFnDebounce--;
		}
		else
		{
			if (IsFlag1(TouchpadSWFnFlag, EnableTouchpadSWFn))
			{
				if (IsFlag0(TouchpadSWFnFlag, OemTouchpadEnable))
				{
					//David modify for EC no need disable touchpad
					//OEM_CmdA7_DAux();
				}
				else
				{
					//OEM_CmdA8_EAux();
					//David modify for EC no need disable touchpad
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
// Enable touchpad switch function
//------------------------------------------------------------------------------
void SWTouchpad_Enable_DisableFunOn(void)
{
	SetFlag(TouchpadSWFnFlag, EnableTouchpadSWFn);
	SetFlag(BIOS_TouchpadSWFnFlag, EnableTouchpadSWFn);
}


//------------------------------------------------------------------------------
// Disable touchpad switch function
//------------------------------------------------------------------------------
void SWTouchpad_Enable_DisableFunOff(void)
{
	ClrFlag(TouchpadSWFnFlag, EnableTouchpadSWFn);
	ClrFlag(BIOS_TouchpadSWFnFlag, EnableTouchpadSWFn);
}


//------------------------------------------------------------------------------
// Set OEM touchpad enable
//------------------------------------------------------------------------------
void Set_Touchpad_Enable(void)
{
	SetFlag(TouchpadSWFnFlag, OemTouchpadEnable);
	SetFlag(BIOS_TouchpadSWFnFlag, OemTouchpadEnable);
}


//------------------------------------------------------------------------------
// Set OEM touchpad disable
//------------------------------------------------------------------------------
void Set_Touchpad_Disable(void)
{
	ClrFlag(TouchpadSWFnFlag, OemTouchpadEnable);
	ClrFlag(BIOS_TouchpadSWFnFlag, OemTouchpadEnable);
}
#endif	//OEM_TP


#ifdef	Multiple_TP	// ken add for touchpad compare	// leox_20120316 Code from "OEM_MAIN.C"
void DetectTPID(void)
{
	if (DetectTPID_Ctrl == 0x01)
	{
		if ((!Send2PortNWait(0, 0xF3, 1)) && (!Send2PortNWait(0, 0x66, 1)) && (!Send2PortNWait(0, 0x88, 1)) &&
			(!Send2PortNWait(0, 0xF3, 1)) && (!Send2PortNWait(0, 0x66, 1)) && (!Send2PortNWait(0, 0x00, 1)) &&
			(!Send2PortNWait(0, 0xE9, 4)))
		{
			TPTYPE4BIOS = 0x00;
			if ((LastDetectTPID_Cmd == 0xE9) && (DetectTPID0 == 0xFA) && (DetectTPID1 == 0x00) && (DetectTPID2 == 0xFE) && (DetectTPID3 == 0x01))
			{	// SE: Sentelic
				TPTYPE4BIOS = 0x01;
			}
			if ((LastDetectTPID_Cmd == 0xE9) && (DetectTPID0 == 0xFA) && (DetectTPID1 == 0x00) && (DetectTPID2 == 0x02) && (DetectTPID3 == 0x64))
			{	// EL: Elan
				TPTYPE4BIOS = 0x02;
			}
		}
		DetectTPID_Ctrl = 0x00;
	}
}


void DetectTPID4PS2(void)
{
	if ((DetectTPID_Ctrl == 0x01) && (PS2IFAck == 0xE9))
	{
		LastDetectTPID_Cmd = PS2IFAck;
		DetectTPID_Ctrl = 0x02;
		return;
	}

	if (DetectTPID_Ctrl == 0x02)
	{
		DetectTPID0 = PS2IFAck;
		DetectTPID_Ctrl = 0x03;
		return;
	}

	if (DetectTPID_Ctrl == 0x03)
	{
		DetectTPID1 = PS2IFAck;
		DetectTPID_Ctrl = 0x04;
		return;
	}

	if (DetectTPID_Ctrl == 0x04)
	{
		DetectTPID2 = PS2IFAck;
		DetectTPID_Ctrl = 0x05;
		return;
	}

	if (DetectTPID_Ctrl == 0x05)
	{
		DetectTPID3 = PS2IFAck;
		DetectTPID_Ctrl = 0x06;
		return;
	}
}
#endif	// Multiple_TP

