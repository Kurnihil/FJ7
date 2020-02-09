/*------------------------------------------------------------------------------
 * Title: OEM_SCANTABS.C - RC Table(s) for Scan Matix
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


// Scanner Table 1  // SF2 Keyboard
const unsigned char code Rc_ROM_Tables_US_UK[] =
{
	// Scanner Table 1
	//SF2 Keyboard
	0x00, 0x4B, 0x4E, 0x52, 0x2B, 0x3B, 0x33, 0x21, 0xBC, 0x1D, 0x8E, 0x00, 0x00, 0x00, 0x8C, 0x69,		// leo_20150820 add Fn + D P80 debug, 0x23 -> 0xBC
	0x00, 0x00, 0x4D, 0x55, 0x46, 0x44, 0x3D, 0x2E, 0x26, 0x16, 0x58, 0x88, 0x00, 0x00, 0x00, 0xED,
	0x00, 0xE1, 0x66, 0xE9, 0xB7, 0xB5, 0x36, 0xB2, 0xB0, 0x06, 0x76, 0x89, 0x00, 0x00, 0x00, 0xBB,
	0x9A, 0x00, 0x84, 0x5A, 0x41, 0x3A, 0x31, 0x2A, 0x22, 0x1A, 0x61, 0x00, 0x00, 0x8B, 0x00, 0x00,
	0x99, 0x00, 0x4A, 0x5D, 0x49, 0x32, 0x34, 0x1B, 0x00, 0x29, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x70,
	0x00, 0xEA, 0xB8, 0xE8, 0xB6, 0xB4, 0xB3, 0xB1, 0xAF, 0x05, 0x0E, 0x00, 0x00, 0x00, 0x8D, 0x94,
	0x9B, 0x00, 0x54, 0x00, 0x45, 0x3C, 0x42, 0x2D, 0x25, 0x1E, 0xBA, 0x00, 0x82, 0x00, 0x00, 0x6B,
	0x98, 0x00, 0x4C, 0x5B, 0x43, 0x3E, 0x35, 0x2C, 0x24, 0x15, 0x00, 0x00, 0x00, 0x8A, 0x00, 0x6C,
	//SF2 Keyboard
};

const unsigned char code EtKey_ROM_Tables_US_UK[] =
{
	//16   17   18      // 19X8 Scan Table
	//SF2 Keyboard
	0x72, 0x7A, 0x00,
	0x7C, 0x7B, 0x00,
	0x00, 0x97, 0x00,
	0x00, 0x81, 0x00,
	0x71, 0x79, 0x00,
	0x95, 0x96, 0x00,
	0x73, 0x74, 0x00,
	0x75, 0x7D, 0x00,
	//SF2 Keyboard
};

const unsigned char code Rc_ROM_Tables_JP[] =
{
	// Scanner Table 1
	//SF2 Keyboard
	0x00, 0x4B, 0x4E, 0x52, 0x2B, 0x3B, 0x33, 0x21, 0xBC, 0x1D, 0x8E, 0x00, 0x00, 0x00, 0x8C, 0x69,		// leo_20150820 add Fn + D P80 debug, 0x23 -> 0xBC
	0x00, 0x00, 0x4D, 0x55, 0x46, 0x44, 0x3D, 0x2E, 0x26, 0x16, 0x58, 0x88, 0x00, 0x00, 0x00, 0xED,
	0x00, 0xE2, 0x66, 0xE1, 0xB7, 0xB5, 0x36, 0xB2, 0xB0, 0x06, 0x76, 0x89, 0x00, 0x00, 0x00, 0xBB,
	0xE6, 0x13, 0x84, 0x5A, 0x41, 0x3A, 0x31, 0x2A, 0x22, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xE5, 0x64, 0x4A, 0x5D, 0x49, 0x32, 0x34, 0x1B, 0x00, 0x29, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x70,
	0x00, 0xE3, 0xB8, 0xE0, 0xB6, 0xB4, 0xB3, 0xB1, 0xAF, 0x05, 0x0E, 0x00, 0x00, 0x00, 0x8D, 0x94,
	0xE7, 0x00, 0x54, 0x6A, 0x45, 0x3C, 0x42, 0x2D, 0x25, 0x1E, 0xBA, 0x00, 0x82, 0x00, 0x00, 0x6B,
	0xE4, 0x51, 0x4C, 0x5B, 0x43, 0x3E, 0x35, 0x2C, 0x24, 0x15, 0x67, 0x00, 0x00, 0x8A, 0x00, 0x6C,
	//SF2 Keyboard
};

const unsigned char code EtKey_ROM_Tables_JP[] =
{
	//16   17   18      // 19X8 Scan Table
	//SF2 Keyboard
	0x72, 0x7A, 0x00,
	0x7C, 0x7B, 0x00,
	0x00, 0x97, 0x00,
	0xB9, 0x81, 0x00,
	0x71, 0x79, 0x00,
	0x95, 0x96, 0x00,
	0x73, 0x74, 0x00,
	0x75, 0x7D, 0x00,
	//SF2 Keyboard
};


const unsigned char code Rc_ROM_Tables_JP2[] =
{
	0x94, 0x00, 0x00, 0x00, 0x89, 0x00, 0xE2, 0xE1, 0xB8, 0xB6, 0xB5, 0xB2, 0xB1, 0xB0, 0x06, 0x76,
	0xED, 0x00, 0x00, 0x00, 0x88, 0x00, 0xE3, 0xE0, 0xB7, 0x46, 0xB4, 0xB3, 0x2E, 0xAF, 0x1E, 0x05,
	0xBB, 0xE4, 0x00, 0x00, 0x00, 0x00, 0x6A, 0x55, 0x4E, 0x45, 0x3E, 0x3D, 0x36, 0x25, 0x26, 0x16,
	0x6C, 0xE5, 0x00, 0x82, 0x00, 0x00, 0x66, 0x5B, 0x54, 0x44, 0x00, 0x3C, 0x2C, 0x24, 0x15, 0x0E,
	0x6B, 0xE6, 0x00, 0x00, 0x00, 0x00, 0x5A, 0x52, 0x4C, 0x4D, 0x43, 0x35, 0x34, 0x2D, 0x1D, 0xBA,
	0x69, 0xE7, 0x8A, 0x00, 0x00, 0x00, 0x5D, 0x4B, 0x42, 0x3B, 0x33, 0x2B, 0xBC, 0x1B, 0x1C, 0x58,		// leo_20150820 add Fn + D P80 debug, 0x23 -> 0xBC
	0x70, 0x00, 0x00, 0x00, 0x00, 0x8D, 0x51, 0x49, 0x41, 0x3A, 0x00, 0x00, 0x2A, 0x21, 0x22, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0x4A, 0x84, 0x13, 0x64, 0x29, 0x31, 0x32, 0x67, 0x1A, 0x8E,
};
const unsigned char code EtKey_ROM_Tables_JP2[] =
{
	//16   17   18      // 19X8 Scan Table
	0x95, 0x96, 0x00,
	0x7C, 0x97, 0x00,
	0x00, 0x7B, 0x00,
	0x75, 0x7D, 0x00,
	0x73, 0x74, 0x00,
	0x72, 0x7A, 0x00,
	0x71, 0x79, 0x00,
	0xB9, 0x81, 0x00,
};

	//SF2 Keyboard// Scanner Table 1
const unsigned char code KB_S3WakeUP_Tables[] =
{   //00    01    02    03    04    05    06    07    08    09    10    11    12    13    14    15
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
	0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
};

const unsigned char code EtKey_S3WakeUP_Tables[] =
{   //16    17    18	// 19x8 Scan Table
	0x55, 0x55, 0x55,
	0x55, 0x55, 0x55,
	0x55, 0x55, 0x55,
	0x55, 0x55, 0x55,
	0x55, 0x55, 0x55,
	0x55, 0x55, 0x55,
	0x55, 0x55, 0x55,
	0x55, 0x55, 0x55,
};


//------------------------------------------------------------------------------
// Check boot hotkey Dino 1024
//------------------------------------------------------------------------------
#define	Crisiskey01	0x8A		// LALT
#define Crisiskey02 0x88		// LShift
#define	Crisiskey03	0x21		// c
void CheckCrisis(void)
{
	if (IsFlag1(CrisisKeyStatus, CrisisKey0) &&
		IsFlag1(CrisisKeyStatus, CrisisKey1) &&
		IsFlag1(CrisisKeyStatus, CrisisKey2))
	{
		SetFlag(KETBOARD_STAT_CTRL, CRISIS_ON);
	}
}


void CrisisHotkey01(BYTE event)
{
	if (event == BREAK_EVENT)
	{
		ClrFlag(CrisisKeyStatus, CrisisKey0);
	}
	else if ((event == MAKE_EVENT) || (event == REPEAT_EVENT))
	{
		SetFlag(CrisisKeyStatus, CrisisKey0);
	}
}


void CrisisHotkey02(BYTE event)
{
	if (event == BREAK_EVENT)
	{
		ClrFlag(CrisisKeyStatus, CrisisKey1);
	}
	else if ((event == MAKE_EVENT) || (event == REPEAT_EVENT))
	{
		SetFlag(CrisisKeyStatus, CrisisKey1);
	}
}


void CrisisHotkey03(BYTE event)
{
	if (event == BREAK_EVENT)
	{
		ClrFlag(CrisisKeyStatus, CrisisKey2);
	}
	else if ((event == MAKE_EVENT) || (event == REPEAT_EVENT))
	{
		SetFlag(CrisisKeyStatus, CrisisKey2);
	}
}


const sBootHotKeyStruct code asBootHotKeyStruct[] =
{
	{Crisiskey01, CrisisHotkey01},
	{Crisiskey02, CrisisHotkey02},
	{Crisiskey03, CrisisHotkey03},
};


void CheckBootHotKey(matrix, event)
{
	BYTE index;
	for (index = 0x00; index < (sizeof(asBootHotKeyStruct) / sizeof(sBootHotKeyStruct)); index++)
	{
		if (matrix == asBootHotKeyStruct[index].matrix)
		{
			(asBootHotKeyStruct[index].pfunction)(event);
		}
	}
}


void OEM_Hook_Send_Key(BYTE table_entry, BYTE event)
{
	CheckBootHotKey(table_entry, event);
	if (IsFlag1(SYS_MISC2, CrisisCheck)) {CheckCrisis();}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F1
//------------------------------------------------------------------------------
void HotKey_Fn_F1(BYTE event)
{

}


//------------------------------------------------------------------------------
// Hotkey Fn + F2
//------------------------------------------------------------------------------
void HotKey_Fn_F2(BYTE event)
{

}


//------------------------------------------------------------------------------
// Hotkey Fn + F3
//------------------------------------------------------------------------------
void HotKey_Fn_F3(BYTE event)		// Mute
{
	if (event == MAKE_EVENT)
	{
		e0_prefix_code(0x23, MAKE_EVENT);
	}
	if (event == BREAK_EVENT)
	{
		e0_prefix_code(0x23, BREAK_EVENT);
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F4
//------------------------------------------------------------------------------
void HotKey_Fn_F4(BYTE event)		// TouchPad
{
	if (IsFlag1(POWER_FLAG, ACPI_OS))
	{
		if(IsFlag1(TouchpadSWFnFlag, OemTouchpadEnable))
		{
			if (event == MAKE_EVENT)
			{
				Set_Touchpad_Disable();
				e0_prefix_code(0x19, MAKE_EVENT);
				ECQEvent(Q0D_SCI, SCIMode_Normal);
			}
			if (event == BREAK_EVENT)
			{
				e0_prefix_code(0x19, BREAK_EVENT);
//				ECQEvent(Q0D_SCI, SCIMode_Normal);
			}
		}
		else
		{
			if (event == MAKE_EVENT)
			{
				Set_Touchpad_Enable();
				e0_prefix_code(0x39, MAKE_EVENT);
				ECQEvent(Q0D_SCI, SCIMode_Normal);
			}
			if (event == BREAK_EVENT)
			{
				e0_prefix_code(0x39, BREAK_EVENT);
//				ECQEvent(Q0D_SCI, SCIMode_Normal);
			}
		}
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F5
//------------------------------------------------------------------------------
void HotKey_Fn_F5(BYTE event)		// Wireless Lan Switch
{
	if (IsFlag1(POWER_FLAG, ACPI_OS))
	{
		if (event == MAKE_EVENT)
		{
			if (IsFlag1(Device_STAT_CTRL, Lan_Status))
			{
				ECQEvent(Q0E_SCI, SCIMode_Normal);
				e0_prefix_code(0x0B, MAKE_EVENT);
			}
			else
			{
				ECQEvent(Q0E_SCI, SCIMode_Normal);
				e0_prefix_code(0x0B, MAKE_EVENT);
			}
/*			if (IsFlag0(KETBOARD_STAT_CTRL, OS_Type))
			{
				if((IsFlag1(Device_STAT_CTRL, Lan_Status)) && (IsFlag1(Device_STAT_CTRL, BT_Status)) && (IsFlag1(Device_STAT_CTRL1, ThreeG_Status)))
				{
					ClrFlag(Device_STAT_CTRL, Lan_Status);
					ClrFlag(Device_STAT_CTRL, BT_Status);
					ClrFlag(Device_STAT_CTRL1, ThreeG_Status);
					e0_prefix_code(0x0B, MAKE_EVENT);
				}
				else
				{
					SetFlag(Device_STAT_CTRL, Lan_Status);
					SetFlag(Device_STAT_CTRL, BT_Status);
					SetFlag(Device_STAT_CTRL1, ThreeG_Status);
					e0_prefix_code(0x0B, MAKE_EVENT);
				}
			}
			else
			{
				if (IsFlag1(Device_STAT_CTRL, Lan_Status))
				{
					ECQEvent(Q0E_SCI, SCIMode_Normal);
					e0_prefix_code(0x0B, MAKE_EVENT);
				}
				else
				{
					ECQEvent(Q0E_SCI, SCIMode_Normal);
					e0_prefix_code(0x0B, MAKE_EVENT);
				}
			}*/
		}
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F6
//------------------------------------------------------------------------------
void HotKey_Fn_F6(BYTE event)		// Brightness Down
{
	if (IsFlag1(POWER_FLAG, ACPI_OS))
	{
		if ((event == MAKE_EVENT))
		{
			ECQEvent(Q0F_SMISCI, SCIMode_Normal);
		}
	}
	else
	{
		if ((event == MAKE_EVENT))
		{
			ECSMIEvent(Q0F_SMISCI);
		}
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F7
//------------------------------------------------------------------------------
void HotKey_Fn_F7(BYTE event)		// Brightness Up
{
	if (IsFlag1(POWER_FLAG, ACPI_OS))
	{
		if ((event == MAKE_EVENT))
		{
			ECQEvent(Q10_SMISCI, SCIMode_Normal);
		}
	}
	else
	{
		if ((event == MAKE_EVENT))
		{
			ECSMIEvent(Q10_SMISCI);
		}
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F8
//------------------------------------------------------------------------------
void HotKey_Fn_F8(BYTE event)		// Volumn Down
{
	if (event == MAKE_EVENT || event == REPEAT_EVENT)
	{
		e0_prefix_code(0x21, MAKE_EVENT);
	}
	if (event == BREAK_EVENT)
	{
		e0_prefix_code(0x21, BREAK_EVENT);
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F9
//------------------------------------------------------------------------------
void HotKey_Fn_F9(BYTE event)		// Volumn Up
{
	if (event == MAKE_EVENT || event == REPEAT_EVENT)
	{
		e0_prefix_code(0x32, MAKE_EVENT);
	}
	if (event == BREAK_EVENT)
	{
		e0_prefix_code(0x32, BREAK_EVENT);
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F10
//------------------------------------------------------------------------------
void HotKey_Fn_F10(BYTE event)		// Display Switch
{
	if (IsFlag1(POWER_FLAG, ACPI_OS))
	{
		//David modify for send WIN+P scancode 2014/06/18
		if ((event == BREAK_EVENT))
		{
			//ECQEvent(Q13_SMISCI, SCIMode_Normal);
			e0_prefix_code(0x1F, MAKE_EVENT);
			simple_code(0x4D, MAKE_EVENT);
			simple_code(0x4D, BREAK_EVENT);
			e0_prefix_code(0x1F, BREAK_EVENT);
		//David modify for send WIN+P scancode 2014/06/18
		}
	}
	else
	{
		if ((event == MAKE_EVENT))
		{
			ECSMIEvent(Q13_SMISCI);
		}
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F11
//------------------------------------------------------------------------------
void HotKey_Fn_F11(BYTE event)		// ECO mode
{
	if (IsFlag1(POWER_FLAG, ACPI_OS))
	{
		if ((event == MAKE_EVENT))
		{
			ECQEvent(Q14_SMISCI, SCIMode_Normal);
		}
	}
	else
	{
		if ((event == MAKE_EVENT))
		{
			ECSMIEvent(Q14_SMISCI);
		}
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + F12
//------------------------------------------------------------------------------
void HotKey_Fn_F12(BYTE event)		// CCD Power Switch
{
	if (IsFlag1(POWER_FLAG, ACPI_OS))
	{
		if (event == MAKE_EVENT)
		{
			if ((IsFlag1(Device_STAT_CTRL1, CCD_Kill_En)) && (IsFlag1(Device_STAT_CTRL1, CCD_Kill_Fn_En)))
			{
				if (IsFlag1(Device_STAT_CTRL1, CCD_Status))
				{
					ClrFlag(Device_STAT_CTRL1, CCD_Status);
				}
				else
				{
					SetFlag(Device_STAT_CTRL1, CCD_Status);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + ESC
//------------------------------------------------------------------------------
void HotKey_00(BYTE event)		// 00
{
	if (IS_MASK_CLEAR(Device_STAT_CTRL2, NumLock)) {return;}
	if ((event == MAKE_EVENT) || (event == REPEAT_EVENT))
	{
		simple_code(0x70, MAKE_EVENT);
		simple_code(0x70, BREAK_EVENT);
		simple_code(0x70, MAKE_EVENT);
		simple_code(0x70, BREAK_EVENT);
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + up arrow
//------------------------------------------------------------------------------
void HotKey_LTAB(BYTE event)
{
	if((event == MAKE_EVENT) || (event == REPEAT_EVENT))
	{
		SetFlag(Factory_KBTest_status, LTab);
		ClrFlag(Factory_KBTest_status, RTab);
		simple_code(0x0D, MAKE_EVENT);
	}
	else if(event == BREAK_EVENT)
	{
		simple_code(0x0D, BREAK_EVENT);
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + down arrow
//------------------------------------------------------------------------------
void HotKey_RTAB(BYTE event)
{
	if((event == MAKE_EVENT) || (event == REPEAT_EVENT))
	{
		SetFlag(Factory_KBTest_status, RTab);
		ClrFlag(Factory_KBTest_status, LTab);
		simple_code(0x0D, MAKE_EVENT);
	}
	else if(event == BREAK_EVENT)
	{
		simple_code(0x0D, BREAK_EVENT);
	}
}


//------------------------------------------------------------------------------
// Hotkey Fn + left arrow
//------------------------------------------------------------------------------
void HotKey_Fn_LEFT(BYTE event)
{

}


//------------------------------------------------------------------------------
// Hotkey Fn + right arrow
//------------------------------------------------------------------------------
void HotKey_Fn_RIGHT(BYTE event)
{

}


//------------------------------------------------------------------------------
// Hotkey Fn + D (P80 debug)	// leo_20150820 add Fn + D P80 debug
//------------------------------------------------------------------------------
void HotKey_Fn_D(BYTE event)
{
	BaseP80[0xFD] = P80LB_BRAM;
}


//------------------------------------------------------------------------------
// Hotkey Fn + Ins (Scr Lk)
//------------------------------------------------------------------------------
void HotKey_Fn_Ins(BYTE event)
{

}


//------------------------------------------------------------------------------
// Hotkey Fn + Del (NumLk)
//------------------------------------------------------------------------------
void HotKey_Fn_Del(BYTE event)
{

}


const FUNCT_PTR_V_B code HotKey_Fn_Fx[] =
{
	HotKey_Fn_F1,		// Fn + F1
	HotKey_Fn_F2,		// Fn + F2
	HotKey_Fn_F3,		// Fn + F3
	HotKey_Fn_F4,		// Fn + F4
	HotKey_Fn_F5,		// Fn + F5
	HotKey_Fn_F6,		// Fn + F6
	HotKey_Fn_F7,		// Fn + F7
	HotKey_Fn_F8,		// Fn + F8
	HotKey_Fn_F9,		// Fn + F9
	HotKey_Fn_F10,		// Fn + F10
	HotKey_Fn_F11,		// Fn + F11
	HotKey_Fn_F12,		// Fn + F12
	HotKey_00,			// 00
	HotKey_LTAB,		// Left Tab
	HotKey_RTAB,		// Right Tab
	HotKey_Fn_LEFT,		// Fn + left arrow
	HotKey_Fn_RIGHT,	// Fn + right arrow
	HotKey_Fn_D,		// Fn + D			// leo_20150820 add Fn + D P80 debug
//	HotKey_Fn_Ins,		// Fn + Ins (Scr Lk)
//	HotKey_Fn_Del,		// Fn + Del (NumLk)
};


/*------------------------------------------------------------------------------
 * Function: sskey2_A2_table
 *
 * The A2_TABLE structure contains the following.
 *
 * BYTE comb; Bit   Setting Combination Key
 *            ---   -----------------------
 *             7    reserved
 *             6    reserved
 *             5    Fn
 *             4    Numlock
 *             3    Ctrl
 *             2    Alt
 *             1    Shift
 *             0    reserved
 *
 * BYTE *pntr; Pointer to array for key.
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
//  Numpad 7 Home   [B0h]
//  Numpad 8 UP     [B1h]
//  Numpad 9 PGUp   [B2h]
//  Numpad *        [B3h]
//  Numpad 4 Left   [B4h]
//  Numpad 5        [B5h]
//  Numpad 6 Right  [B6h]
//  Numpad -        [B7h]
//  Numpad 1 End    [B8h]
//  Numpad 2 Down   [B9h]
//  Numpad 3 PGDn   [BAh]
//  Numpad +        [BBh]
//  Numpad 0 Ins    [BCh]
//  Numpad . Del    [BDh]
//  Numpad /        [BEh]
//------------------------------------------------------------------------------
const BYTE code ss2_9C[] =  // Numpad 7
{
	0x3D,	// Normal		// 7
	0xB0,	// Numlock
	0xB0,	// Fn
	0x3D,	// Fn+Numlock
};

const BYTE code ss2_9D[] =  // Numpad 8
{
	0x3E,	// Normal		// 8
	0xB1,	// Numlock
	0xB1,	// Fn
	0x3E,	// Fn+Numlock
};

const BYTE code ss2_9E[] =  // Numpad 9
{
	0x46,	// Normal		// 9
	0xB2,	// Numlock
	0xB2,	// Fn
	0x46,	// Fn+Numlock
};

const BYTE code ss2_9F[] =  // Numpad -_
{
	0x4E,	// Normal       // -_
	0xB7,	// Numlock
	0xB7,	// Fn
	0x4E,	// Fn+Numlock
};

const BYTE code ss2_A0[] =  // Numpad U
{
	0x3C,	// Normal		// U
	0xB4,	// Numlock
	0xB4,	// Fn
	0x3C,	// Fn+Numlock
};

const BYTE code ss2_A1[] =  // Numpad I
{
	0x43,	// Normal		// I
	0xB5,	// Numlock
	0xB5,	// Fn
	0x43,	// Fn+Numlock
};

const BYTE code ss2_A2[] =  // Numpad O
{
	0x44,	// Normal		// O
	0xB6,	// Numlock
	0xB6,	// Fn
	0x44,	// Fn+Numlock
};

const BYTE code ss2_A3[] =  // Numpad [{
{
	0x54,	// Normal       //[{
	0x81,	// Numlock
	0x81,	// Fn
	0x54,	// Fn+Numlock
};

const BYTE code ss2_A4[] =  // Numpad J
{
	0x3B,	// Normal		// J
	0xB8,	// Numlock
	0xB8,	// Fn
	0x3B,	// Fn+Numlock
};

const BYTE code ss2_A5[] =  // Numpad K
{
	0x42,	// Normal		// K
	0xB9,	// Numlock
	0xB9,	// Fn
	0x42,	// Fn+Numlock
};

const BYTE code ss2_A6[] =  // Numpad L
{
	0x4B,	// Normal		// L
	0xBA,	// Numlock
	0xBA,	// Fn
	0x4B,	// Fn+Numlock
};

const BYTE code ss2_A7[] =  // Numpad ;:
{
	0x4C,	// Normal		// ;:
	0xBB,	// Numlock
	0xBB,	// Fn
	0x4C,	// Fn+Numlock
};

const BYTE code ss2_A8[] =  // Numpad M
{
	0x3A,	// Normal		// M
	0xBC,	// Numlock
	0xBC,	// Fn
	0x3A,	// Fn+Numlock
};

const BYTE code ss2_A9[] =  // Numpad '"
{
	0x52,	// Normal       // '"
	0xB3,	// Numlock
	0xB3,	// Fn
	0x52,	// Fn+Numlock
};

const BYTE code ss2_AA[] =  // Numpad .>
{
	0x49,	// Normal		// .>
	0xBD,	// Numlock
	0xBD,	// Fn
	0x49,	// Fn+Numlock
};

const BYTE code ss2_AB[] =  // Numpad /?
{
	0x4A,	// Normal		// /?
	0xBE,	// Numlock
	0xBE,	// Fn
	0x4A,	// Fn+Numlock
};

const BYTE code ss2_AC[] =
{
	0x5A,	// Normal
	0x81,	// Fn
};


//------------------------------------------------------------------------------
// Function key F1 ~ F12
//------------------------------------------------------------------------------
const BYTE code ss2_AD[] =  // F1
{
	0x05,	// Normal
	0xD0,	// Fn
};

const BYTE code ss2_AE[] =  // F2
{
	0x06,	// Normal
	0xD1,	// Fn
};

const BYTE code ss2_AF[] =  // F3
{
	0x04,	// Normal
	0xD2,	// Fn
};

const BYTE code ss2_B0[] =  // F4
{
	0x0C,	// Normal
	0xD3,	// Fn
};

const BYTE code ss2_B1[] =  // F5
{
	0x03,	// Normal
	0xD4,	// Fn
};

const BYTE code ss2_B2[] =  // F6
{
	0x0B,	// Normal
	0xD5,	// Fn
};

const BYTE code ss2_B3[] =  // F7
{
	0x80,	// Normal
	0xD6,	// Fn
};

const BYTE code ss2_B4[] =  // F8
{
	0x0A,	// Normal
	0xD7,	// Fn
};

const BYTE code ss2_B5[] =  // F9
{
	0x01,	// Normal
	0xD8,	// Fn
//	0x01,	// Normal
};

const BYTE code ss2_B6[] =  // F10
{
	0x09,	// Normal
//	0x09,	// Fn
	0xD9,	// Fn
//	0x09,	// Normal
};

const BYTE code ss2_B7[] =  // F11
{
	0x78,	// Normal
	0xDA,	// Fn
//	0x78,	// Normal
};

const BYTE code ss2_B8[] =  // F12
{
	0x07,	// Normal
	0xDB,	// Fn
//	0x07,	//
};

const BYTE code ss2_B9[] =  // 00
{
	0xDC,	// Normal
	0xDC,	// Fn
};

const BYTE code ss2_BA[] =  // L-Tab
{
	0xDD,	// Normal
	0xDD,	// Fn
};

const BYTE code ss2_BB[] =  // R-Tab
{
	0xDE,	// Normal
	0xDE,	// Fn
};

const BYTE code ss2_BC[] =  // D	// leo_20150820 add Fn + D P80 debug
{
	0x23,	// Normal
	0xE1,	// Fn			// leox20150821 0xC6 -> 0xE1
};

/*------------------------------------------------------------------------------
 * Function: sskey2_A2_table
 *
 * The A2_TABLE structure contains the following.
 *
 * BYTE comb; Bit   Setting Combination Key
 *            ---   -----------------------
 *             7    reserved
 *             6    reserved
 *             5    Fn
 *             4    Numlock
 *             3    Ctrl
 *             2    Alt
 *             1    Shift
 *             0    reserved
 *
 * BYTE *pntr; Pointer to array for key.
 *----------------------------------------------------------------------------*/
const A2_TABLE sskey2_A2_table[] =
{	// Keyboard matrix index
	// Keypad -->
	{0x30, ss2_9C},	// index = 00h  // 0x9C
	{0x30, ss2_9D},	// index = 01h  // 0x9D
	{0x30, ss2_9E},	// index = 02h  // 0x9E
	{0x30, ss2_9F},	// index = 03h  // 0x9F
	{0x30, ss2_A0},	// index = 04h  // 0xA0
	{0x30, ss2_A1},	// index = 05h  // 0xA1
	{0x30, ss2_A2},	// index = 06h  // 0xA2
	{0x30, ss2_A3},	// index = 07h  // 0xA3
	{0x30, ss2_A4},	// index = 08h  // 0xA4
	{0x30, ss2_A5},	// index = 09h  // 0xA5
	{0x30, ss2_A6},	// index = 0Ah  // 0xA6
	{0x30, ss2_A7},	// index = 0Bh  // 0xA7
	{0x30, ss2_A8},	// index = 0Ch  // 0xA8
	{0x30, ss2_A9},	// index = 0Dh  // 0xA9
	{0x30, ss2_AA},	// index = 0Eh  // 0xAA
	{0x30, ss2_AB},	// index = 0Fh  // 0xAB
	{0x20, ss2_AC},	// index = 10h  // 0xAC
	// Keypad <--

	// F1~F12 hotkeys -->
	{0x20, ss2_AD},	// index = 11h  // 0xAD
	{0x20, ss2_AE},	// index = 12h  // 0xAE
	{0x20, ss2_AF},	// index = 13h  // 0xAF
	{0x20, ss2_B0},	// index = 14h  // 0xB0
	{0x20, ss2_B1},	// index = 15h  // 0xB1
	{0x20, ss2_B2},	// index = 16h  // 0xB2
	{0x20, ss2_B3},	// index = 17h  // 0xB3
	{0x20, ss2_B4},	// index = 18h  // 0xB4
	{0x20, ss2_B5},	// index = 19h  // 0xB5
	{0x20, ss2_B6},	// index = 1Ah  // 0xB6
	{0x20, ss2_B7},	// index = 1Bh  // 0xB7
	{0x20, ss2_B8}, // index = 1Ch  // 0xB8
	{0x20, ss2_B9}, // index = 1Dh  // 0xB9
	{0x20, ss2_BA}, // index = 1Dh  // 0xBA
	{0x20, ss2_BB}, // index = 1Dh  // 0xBB
	{0x20, ss2_BC}, // index = 1Dh  // 0xBC	// leo_20150820 add Fn + D P80 debug
	// F1~F12 hotkeys <--
};
// Reserved 0xB9 ~ 0xDF


/*------------------------------------------------------------------------------
 * Function: sskey2_overlay_table
 *----------------------------------------------------------------------------*/
const BYTE code sskey2_overlay_table[] =
{				//  Normal		Fn											// Keyboard matrix index
	0xC3, 0xC1,	//  Print Scr	SysRq										// 0xE0
	0x91, 0x92,	// Pause		Break				// 0xE1
	0xC0, 0x7E,	// Del			Scr LK				// 0xE2
	0xC2, 0x77,	// Insert		Num Lk				// 0xE3

	0x98, 0x96,	// Up arrow		Function key up arrow    (HotKey_Fn_Up)		// 0xE4
	0x99, 0x97,	// Down arrow	Function key down arrow  (HotKey_Fn_Down)	// 0xE5
	0x9A, 0x94,	// Left arrow	Function key Left arrow  (HotKey_Fn_LEFT)	// 0xE6
	0x9B, 0x95,	// Right arrow	Function key Right arrow (HotKey_Fn_RIGHT)	// 0xE7

	0x77, 0x7E,	// Num Lk		Scr LK				// 0xE8
	0xC2, 0xC3,	// Insert		Print Scr			// 0xE9
	0xC0, 0xC1,	// Del			SysRq				// 0xEA
	0x95, 0x9C,	//  End			Next Track event							// 0xEB

	0x76, 0xDC,	// ESC			ESC					// 0xEC
	0xBE, 0xBE,	//  KeyPad_/	KeyPad_/									// 0xED

	0xC4, 0xC4,	// Euro Sign	Euro Sign			// 0xEE
	0xC5, 0xC5,	// Dollar Sign	Dollar Sign			// 0xEF
};
// Reserved 0xEE ~ 0xFF


/*------------------------------------------------------------------------------
 * Insyde Software Key Number 2
 *
 * 00h         Null
 * 01h - 7Fh   Same as IBM Scan Code, Set2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * C0h - DFh   Pre-Index for PPK function
 * C0h - DFh   Pre-Index for SMI function
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Function: sskey3_80_table - Insyde Software Key Number 2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 *----------------------------------------------------------------------------*/
const BYTE code sskey3_80_table[] =
{
	//--------------------------------------------------------------------------
	// Index from keyboard matrix
	//--------------------------------------------------------------------------
	// Keyboard matrix index
	0x83,	DO_SIMPLE_CODE,			// [80h] F7 (118)
	0x5A,	DO_E0_PREFIX_CODE,		// [81h] Numpad Enter (108)
	0x1F,	DO_E0_PREFIX_CODE,		// [82h] Windows Left
	0x27,	DO_E0_PREFIX_CODE,		// [83h] Windows Right
	0x2F,	DO_E0_PREFIX_CODE,		// [84h] Application
	0x37,	DO_E0_PREFIX_CODE,		// [85h] Power event
	0x3F,	DO_E0_PREFIX_CODE,		// [86h] Sleep event
	0x5E,	DO_E0_PREFIX_CODE,		// [87h] Wake event
	LShift,	DO_CONTROL_EFFECT,		// [88h] 12 Shift(L)
	RShift,	DO_CONTROL_EFFECT,		// [89h] 59 Shift(R)
	LAlt,	DO_CONTROL_EFFECT,		// [8Ah] 11 Alt(L)
	RAlt,	DO_CONTROL_EFFECT_E0,	// [8Bh] E0 11 Alt(R)
	LCtrl,	DO_CONTROL_EFFECT,		// [8Ch] 14 Crtl(L)
	RCtrl,	DO_CONTROL_EFFECT_E0,	// [8Dh] E0 14 Ctrl(R)
	FN,		DO_CONTROL_EFFECT,		// [8Eh] Fn
	OVLAY,	DO_CONTROL_EFFECT,		// [8Fh] Overlay
	0x00,	DO_SIMPLE_CODE,			// [90h] Reserved
	0x00,	DO_SPECIAL_CODE,		// [91h] Pause (126)
	0x01,	DO_SPECIAL_CODE,		// [92h] Break
	0x84,	DO_SIMPLE_CODE,			// [93h] (124) Alt-Case (SysRq)
	0x6C,	DO_E0_PREFIX_CODE,		// [94h] Home
	0x69,	DO_E0_PREFIX_CODE,		// [95h] End
	0x7D,	DO_E0_PREFIX_CODE,		// [96h] Page up
	0x7A,	DO_E0_PREFIX_CODE,		// [97h] Page down
	0x75,	DO_E0_PREFIX_CODE,		// [98h] UP arrow
	0x72,	DO_E0_PREFIX_CODE,		// [99h] Down arrow
	0x6B,	DO_E0_PREFIX_CODE,		// [9Ah] Left arrow
	0x74,	DO_E0_PREFIX_CODE,		// [9Bh] Right arrow

	//--------------------------------------------------------------------------
	// For Microsoft enhance keyboard feature
	//--------------------------------------------------------------------------
	0x4D,	DO_E0_PREFIX_CODE,		// [9Ch] Next Track event
	0x15,	DO_E0_PREFIX_CODE,		// [9Dh] Prev Track event
	0x3B,	DO_E0_PREFIX_CODE,		// [9Eh] Stop event
	0x34,	DO_E0_PREFIX_CODE,		// [9Fh] Play/Pause event
	0x23,	DO_E0_PREFIX_CODE,		// [A0h] Mute event
	0x32,	DO_E0_PREFIX_CODE,		// [A1h] Volume Up event
	0x21,	DO_E0_PREFIX_CODE,		// [A2h] Volume Down event
	0x48,	DO_E0_PREFIX_CODE,		// [A3h] Mail event
	0x10,	DO_E0_PREFIX_CODE,		// [A4h] Search event
	0x3A,	DO_E0_PREFIX_CODE,		// [A5h] Web/Home event
	0x38,	DO_E0_PREFIX_CODE,		// [A6h] Back event
	0x30,	DO_E0_PREFIX_CODE,		// [A7h] Forward event
	0x28,	DO_E0_PREFIX_CODE,		// [A8h] Stop event
	0x20,	DO_E0_PREFIX_CODE,		// [A9h] Refresh event
	0x18,	DO_E0_PREFIX_CODE,		// [AAh] Favorites event
	0x2B,	DO_E0_PREFIX_CODE,		// [ABh] Caluator event
	0x40,	DO_E0_PREFIX_CODE,		// [ACh] My Computer event
	0x50,	DO_E0_PREFIX_CODE,		// [ADh] Media event
	0x00,	DO_SIMPLE_CODE,			// [AEh] Reserved
	0x00,	DO_SIMPLE_CODE,			// [AFh] Reserved

	//--------------------------------------------------------------------------
	// Index from sskey2_A2_table
	//--------------------------------------------------------------------------
	0x6C,	DO_SIMPLE_CODE,			// Numpad 7 Home	[B0h]
	0x75,	DO_SIMPLE_CODE,			// Numpad 8 UP		[B1h]
	0x7D,	DO_SIMPLE_CODE,			// Numpad 9 PGUp	[B2h]
	0x7C,	DO_SIMPLE_CODE,			// Numpad *			[B3h]
	0x6B,	DO_SIMPLE_CODE,			// Numpad 4 Left	[B4h]
	0x73,	DO_SIMPLE_CODE,			// Numpad 5			[B5h]
	0x74,	DO_SIMPLE_CODE,			// Numpad 6 Right	[B6h]
	0x7B,	DO_SIMPLE_CODE,			// Numpad -			[B7h]
	0x69,	DO_SIMPLE_CODE,			// Numpad 1 End		[B8h]
	0x72,	DO_SIMPLE_CODE,			// Numpad 2 Down	[B9h]
	0x7A,	DO_SIMPLE_CODE,			// Numpad 3 PGDn	[BAh]
	0x79,	DO_SIMPLE_CODE,			// Numpad +			[BBh]
	0x70,	DO_SIMPLE_CODE,			// Numpad 0 Ins		[BCh]
	0x71,	DO_SIMPLE_CODE,			// Numpad . Del		[BDh]
	0x4A,	DO_E0_PREFIX_CODE,		// Numpad /			[BEh]

	0x00,	DO_SIMPLE_CODE,			// Reserved			[BFh]

	0x71,	DO_E0_PREFIX_CODE,		// Delete			[C0h]
	0x84,	DO_SIMPLE_CODE,			// SysRq			[C1h]
	0x70,	DO_E0_PREFIX_CODE,		// Insert			[C2h]
//	0x7C,	DO_E0_PREFIX_CODE,		// Print Screen		[C3h]	// fox_20140123
	0x02,	DO_SPECIAL_CODE,		// Print Screen		[C3h]	// fox_20140123	//@JayForSpecialKey
	0x41,	DO_E0_PREFIX_CODE,		// Euro Sign		[C4h]
	0x49,	DO_E0_PREFIX_CODE,		// Dollar Sign		[C5h]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[C6h]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[C7h]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[C8h]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[C9h]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[CAh]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[CBh]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[CCh]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[CDh]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[CEh]
	0x00,	DO_SIMPLE_CODE,			// Reserved			[CFh]

	// Function key F1 ~ F12
	0   ,	DO_COSTOMER_FUNCTION,		// Function key F1			[D0h]
	1   ,	DO_COSTOMER_FUNCTION,		// Function key F2			[D1h]
	2   ,	DO_COSTOMER_FUNCTION,		// Function key F3			[D2h]
	3   ,	DO_COSTOMER_FUNCTION,		// Function key F4			[D3h]
	4   ,	DO_COSTOMER_FUNCTION,		// Function key F5			[D4h]
	5   ,	DO_COSTOMER_FUNCTION,		// Function key F6			[D5h]
	6   ,	DO_COSTOMER_FUNCTION,		// Function key F7			[D6h]
	7   ,	DO_COSTOMER_FUNCTION,		// Function key F8			[D7h]
	8   ,	DO_COSTOMER_FUNCTION,		// Function key F9			[D8h]
	9   ,	DO_COSTOMER_FUNCTION,		// Function key F10			[D9h]
	10  ,	DO_COSTOMER_FUNCTION,		// Function key F11			[DAh]
	11  ,	DO_COSTOMER_FUNCTION,		// Function key F12			[DBh]
	12  ,	DO_COSTOMER_FUNCTION,		// Function key Esc			[DCh]
	13  ,	DO_COSTOMER_FUNCTION,		// Function key Up arrow	[DDh]
	14  ,	DO_COSTOMER_FUNCTION,		// Function key Down arrow	[DEh]
	15  ,	DO_COSTOMER_FUNCTION,		// Function key Left arrow	[DFh]
	16  ,	DO_COSTOMER_FUNCTION,		// Function key Right arrow	[E0h]
	17  ,	DO_COSTOMER_FUNCTION,		// Function key D			[E1h]		// leo_20150820 add Fn + D P80 debug	// leox20150821
};
// Reserved 0xE2 ~ 0xFF


//------------------------------------------------------------------------------
// Keyboard wake up system from S3
//------------------------------------------------------------------------------
void CheckKBS3Wake(BYTE BKSI, BYTE BKSO)
{
	BYTE wuv;

	wuv = ((BKSI << 4) | BKSO);
	Tmp_code_pointer = &KB_S3WakeUP_Tables;
	if (*(Tmp_code_pointer + wuv) == 0x55)
	{
		//PulsePwrBtnToSB(32);	// leox_20121115 Set delay time by your project
	}
}


void Et_CheckKBS3Wake(BYTE BKSI, BYTE BKSO)
{
	BYTE wuv;

	wuv = ((BKSI * 3) + BKSO);
	Tmp_code_pointer = &EtKey_S3WakeUP_Tables;
	if (*(Tmp_code_pointer + wuv) == 0x55)
	{
		//PulsePwrBtnToSB(32);	// leox_20121115 Set delay time by your project
	}
}


//------------------------------------------------------------------------------
// Keyboard hook function
//------------------------------------------------------------------------------
void Hook_keyboard(BYTE KSIValus, BYTE KSOValus)
{
	#if	S3KeyboardWakeUp
	if (IsFlag1(POWER_FLAG, enter_SUS)) {CheckKBS3Wake(KSIValus, KSOValus);}
	#endif
}


//------------------------------------------------------------------------------
// Keyboard hook function
//------------------------------------------------------------------------------
void Et_Hook_keyboard(BYTE KSIValus, BYTE KSOValus)
{
	#if	S3KeyboardWakeUp
	if (IsFlag1(POWER_FLAG, enter_SUS)) {Et_CheckKBS3Wake(KSIValus, KSOValus);}
	#endif
}


//------------------------------------------------------------------------------
// Hook_Setup_Scanner_Pntr - To define the possible scanner tables
//      0   Rc_RAM_Table
//      1   ROM Table 1
//      ...
//------------------------------------------------------------------------------
void Hook_Setup_Scanner_Pntr(void)
{
	GetJumpper();
	//David modify for change matrix check ID 2014/06/18
	if (((IsFlag0(JUMPERSETTING,KBD_ID0)) && (IsFlag1(JUMPERSETTING,KBD_ID1))) || ((IsFlag1(JUMPERSETTING,KBD_ID0)) && (IsFlag0(JUMPERSETTING,KBD_ID1))))
	{
		Scanner_Table_Pntr = &Rc_ROM_Tables_US_UK;
		Extendkey_Table_Pntr = &EtKey_ROM_Tables_US_UK;
	}
	else if ((IsFlag1(JUMPERSETTING,KBD_ID0)) && (IsFlag1(JUMPERSETTING,KBD_ID1)))
	{
		Scanner_Table_Pntr = &Rc_ROM_Tables_JP;
		Extendkey_Table_Pntr = &EtKey_ROM_Tables_JP;
	}
	else
	{
		Scanner_Table_Pntr = &Rc_ROM_Tables_JP2;
		Extendkey_Table_Pntr = &EtKey_ROM_Tables_JP2;
	//David modify for change matrix check ID 2014/06/18
	}
}


//------------------------------------------------------------------------------
// Set GPIO scan pin to high
//------------------------------------------------------------------------------
void Hook_SetGPIOScanPinH(void)
{
	#if	ExternMatrixGPO	// leox_20111129
	GPOKeyPin1H();		// leox_20111214
	#endif
}


//------------------------------------------------------------------------------
// Set GPIO scan pin to low
//------------------------------------------------------------------------------
void Hook_SetGPIOScanPinL(void)
{
	#if	ExternMatrixGPO	// leox_20111129
	GPOKeyPin1L();		// leox_20111214
	#endif
}


//------------------------------------------------------------------------------
// Set GPIO scan pin output
//------------------------------------------------------------------------------
void Hook_SetGPIOScanPinCtrl(void)
{
	#if	ExternMatrixGPO		// leox_20111129
	GPOKeyPin1Ctl = OUTPUT;	// leox_20111108 0x40 -> OUTPUT
	#endif
}
