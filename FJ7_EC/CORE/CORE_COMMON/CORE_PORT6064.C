/*------------------------------------------------------------------------------
 * Title: CORE_PORT6064.C - Standard 8042 Keyboard Controller Commands
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

//-----------------------------------------------------------------------
// Process Command/Data received from System via the KBC interface
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Port64_table[16] =
{
	Cmd_0X,				// Process command 0x
	Cmd_1X,				// Process command 1x
	Cmd_2X,				// Process command 2x
	Cmd_3X,				// Process command 3x
	Cmd_4X,				// Process command 4x
	Cmd_5X,				// Process command 5x
	Cmd_6X,				// Process command 6x
	Cmd_7X,				// Process command 7x
	Cmd_8X,				// Process command 8x
	Cmd_9X,				// Process command 9x
	Cmd_AX,				// Process command Ax
	Cmd_BX,				// Process command Bx
	Cmd_CX,				// Process command Cx
	Cmd_DX,				// Process command Dx
	Cmd_EX,				// Process command Ex
	Cmd_FX,				// Process command Fx
};

const FUNCT_PTR_V_V code Port60_table[16] =
{
	Cmd_0XData,			// Process 64 command data 0x
	Cmd_1XData,			// Process 64 command data 1x
	Cmd_2XData,			// Process 64 command data 2x
	Cmd_3XData,			// Process 64 command data 3x
	Cmd_4XData,			// Process 64 command data 4x
	Cmd_5XData,			// Process 64 command data 5x
	Cmd_6XData,			// Process 64 command data 6x
	Cmd_7XData,			// Process 64 command data 7x
	Cmd_8XData,			// Process 64 command data 8x
	Cmd_9XData,			// Process 64 command data 9x
	Cmd_AXData,			// Process 64 command data Ax
	Cmd_BXData,			// Process 64 command data Bx
	Cmd_CXData,			// Process 64 command data Cx
	Cmd_DXData,			// Process 64 command data Dx
	Cmd_EXData,			// Process 64 command data Ex
	Cmd_FXData,			// Process 64 command data Fx
};

void service_pci1(void)
{
	if (IS_MASK_CLEAR(KBHISR, IBF))
	{
		return;
	}

	if (KBHISR & C_D)				// command
	{
		KBHIStep = 0;				// command start
		KbdNeedResponseFlag = 0;

		KBHICmd  = KBHIDIR;
		Hook_64PortDebug();

		if ((KBHICmd == 0xD2) || (KBHICmd == 0xD3))
		{
			if (KBHICmd == 0xD3)
			{
				Cmd_D3();
			}
			else
			{
				Cmd_D2();
			}
		}
		else
		{
			(Port64_table[(KBHICmd >> 4)])();
			if (KbdNeedResponseFlag)
			{
				KbdNeedResponseFlag = 0;
				KBC_DataToHost(KBHIReponse);
			}
		}
	}
	else							// data
	{
		KBHIData  = KBHIDIR;
		Hook_60PortDebug();
		if (FastA20)
		{
			if ((KBHIData & 0x02) != 0x00)
			{
				GA20_on();
			}
			else
			{
				GA20_off();
			}
			FastA20 = 0;
			KBHIStep = 0;
		}
		else
		{
			if (KBHIStep != 0x00)		// if need data
			{
				(Port60_table[(KBHICmd >> 4)])();
				KBHIStep --;
				if (KbdNeedResponseFlag)
				{
					KbdNeedResponseFlag = 0;
					KBC_DataToHost(KBHIReponse);
				}
			}
			else
			{
				Cmd_AE();					// Enable auxiliary keyboard.

				#if	PS2_KBCMD
					#ifdef	UART_Debug
				printf("\nKB CMD : 0X%bX\n", KBHIData);
					#else
				RamDebug(0x60);
				RamDebug(KBHIData);
					#endif
				#endif

				KeyboardCmd(KBHIData);		// Keyboard command
			}
		}
	}

	EnableKBCIBFInt();			// enable interrupt again
}

//-----------------------------------------------------------------------
// Handle 64 port command 0x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd0X_table[16] =
{
	Cmd_00,				// Process 64 port command 00
	Cmd_01,				// Process 64 port command 01
	Cmd_02,				// Process 64 port command 02
	Cmd_03,				// Process 64 port command 03
	Cmd_04,				// Process 64 port command 04
	Cmd_05,				// Process 64 port command 05
	Cmd_06,				// Process 64 port command 06
	Cmd_07,				// Process 64 port command 07
	Cmd_08,				// Process 64 port command 08
	Cmd_09,				// Process 64 port command 09
	Cmd_0A,				// Process 64 port command 0A
	Cmd_0B,				// Process 64 port command 0B
	Cmd_0C,				// Process 64 port command 0C
	Cmd_0D,				// Process 64 port command 0D
	Cmd_0E,				// Process 64 port command 0E
	Cmd_0F				// Process 64 port command 0F
};

void Cmd_0X(void)
{
	(Cmd0X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command 1x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd1X_table[16] =
{
	Cmd_10,				// Process 64 port command 10
	Cmd_11,				// Process 64 port command 11
	Cmd_12,				// Process 64 port command 12
	Cmd_13,				// Process 64 port command 13
	Cmd_14,				// Process 64 port command 14
	Cmd_15,				// Process 64 port command 15
	Cmd_16,				// Process 64 port command 16
	Cmd_17,				// Process 64 port command 17
	Cmd_18,				// Process 64 port command 18
	Cmd_19,				// Process 64 port command 19
	Cmd_1A,				// Process 64 port command 1A
	Cmd_1B,				// Process 64 port command 1B
	Cmd_1C,				// Process 64 port command 1C
	Cmd_1D,				// Process 64 port command 1D
	Cmd_1E,				// Process 64 port command 1E
	Cmd_1F				// Process 64 port command 1F
};


void Cmd_1X(void)
{
	(Cmd1X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command 2x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Handle command 20 - Read command byte
//-----------------------------------------------------------------------
void Cmd_20(void)

{
	ResponseKBData(Ccb42);
}

//-----------------------------------------------------------------------
// Handle command 21 -
//-----------------------------------------------------------------------
void Cmd_21(void)

{

}

//-----------------------------------------------------------------------
// Handle command 22 -
//-----------------------------------------------------------------------
void Cmd_22(void)

{

}

//-----------------------------------------------------------------------
// Handle command 23 -
//-----------------------------------------------------------------------
void Cmd_23(void)

{

}

//-----------------------------------------------------------------------
// Handle command 24 -
//-----------------------------------------------------------------------
void Cmd_24(void)

{

}

//-----------------------------------------------------------------------
// Handle command 25 -
//-----------------------------------------------------------------------
void Cmd_25(void)

{

}

//-----------------------------------------------------------------------
// Handle command 26 -
//-----------------------------------------------------------------------
void Cmd_26(void)

{

}

//-----------------------------------------------------------------------
// Handle command 27 -
//-----------------------------------------------------------------------
void Cmd_27(void)

{

}

//-----------------------------------------------------------------------
// Handle command 28 -
//-----------------------------------------------------------------------
void Cmd_28(void)

{

}

//-----------------------------------------------------------------------
// Handle command 29 -
//-----------------------------------------------------------------------
void Cmd_29(void)

{

}

//-----------------------------------------------------------------------
// Handle command 2A -
//-----------------------------------------------------------------------
void Cmd_2A(void)

{

}

//-----------------------------------------------------------------------
// Handle command 2B -
//-----------------------------------------------------------------------
void Cmd_2B(void)

{

}

//-----------------------------------------------------------------------
// Handle command 2C -
//-----------------------------------------------------------------------
void Cmd_2C(void)

{

}

//-----------------------------------------------------------------------
// Handle command 2D -
//-----------------------------------------------------------------------
void Cmd_2D(void)

{

}

//-----------------------------------------------------------------------
// Handle command 2E -
//-----------------------------------------------------------------------
void Cmd_2E(void)

{

}

//-----------------------------------------------------------------------
// Handle command 2F -
//-----------------------------------------------------------------------
void Cmd_2F(void)

{

}

const FUNCT_PTR_V_V code Cmd2X_table[16] =
{
	Cmd_20,				// Process 64 port command 20
	Cmd_21,				// Process 64 port command 21
	Cmd_22,				// Process 64 port command 22
	Cmd_23,				// Process 64 port command 23
	Cmd_24,				// Process 64 port command 24
	Cmd_25,				// Process 64 port command 25
	Cmd_26,				// Process 64 port command 26
	Cmd_27,				// Process 64 port command 27
	Cmd_28,				// Process 64 port command 28
	Cmd_29,				// Process 64 port command 29
	Cmd_2A,				// Process 64 port command 2A
	Cmd_2B,				// Process 64 port command 2B
	Cmd_2C,				// Process 64 port command 2C
	Cmd_2D,				// Process 64 port command 2D
	Cmd_2E,				// Process 64 port command 2E
	Cmd_2F				// Process 64 port command 2F
};

void Cmd_2X(void)
{
	(Cmd2X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command 3x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Handle command 30 -
//-----------------------------------------------------------------------
void Cmd_30(void)

{

}

//-----------------------------------------------------------------------
// Handle command 31 -
//-----------------------------------------------------------------------
void Cmd_31(void)

{

}

//-----------------------------------------------------------------------
// Handle command 32 -
//-----------------------------------------------------------------------
void Cmd_32(void)

{

}

//-----------------------------------------------------------------------
// Handle command 33 - Send Security Code On byte to host
//-----------------------------------------------------------------------
void Cmd_33(void)

{
	ResponseKBData(Pass_On);
}

//-----------------------------------------------------------------------
// Handle command 34 - Send Security Code Off byte to host
//-----------------------------------------------------------------------
void Cmd_34(void)

{
	ResponseKBData(Pass_Off);
}

//-----------------------------------------------------------------------
// Handle command 35 -
//-----------------------------------------------------------------------
void Cmd_35(void)

{

}

//-----------------------------------------------------------------------
// Handle command 36 - Send Reject make code 1 to host
//-----------------------------------------------------------------------
void Cmd_36(void)

{
	ResponseKBData(Pass_Make1);
}

//-----------------------------------------------------------------------
// Handle command 37 - Send Reject make code 2 to host
//-----------------------------------------------------------------------
void Cmd_37(void)

{
	ResponseKBData(Pass_Make2);
}

//-----------------------------------------------------------------------
// Handle command 38 -
//-----------------------------------------------------------------------
void Cmd_38(void)

{

}

//-----------------------------------------------------------------------
// Handle command 39 -
//-----------------------------------------------------------------------
void Cmd_39(void)

{

}

//-----------------------------------------------------------------------
// Handle command 3A -
//-----------------------------------------------------------------------
void Cmd_3A(void)

{

}

//-----------------------------------------------------------------------
// Handle command 3B -
//-----------------------------------------------------------------------
void Cmd_3B(void)

{

}

//-----------------------------------------------------------------------
// Handle command 3C -
//-----------------------------------------------------------------------
void Cmd_3C(void)

{

}

//-----------------------------------------------------------------------
// Handle command 3D -
//-----------------------------------------------------------------------
void Cmd_3D(void)

{

}

//-----------------------------------------------------------------------
// Handle command 3E -
//-----------------------------------------------------------------------
void Cmd_3E(void)

{

}

//-----------------------------------------------------------------------
// Handle command 3F -
//-----------------------------------------------------------------------
void Cmd_3F(void)

{

}

const FUNCT_PTR_V_V code Cmd3X_table[16] =
{
	Cmd_30,				// Process 64 port command 30
	Cmd_31,				// Process 64 port command 31
	Cmd_32,				// Process 64 port command 32
	Cmd_33,				// Process 64 port command 33
	Cmd_34,				// Process 64 port command 34
	Cmd_35,				// Process 64 port command 35
	Cmd_36,				// Process 64 port command 36
	Cmd_37,				// Process 64 port command 37
	Cmd_38,				// Process 64 port command 38
	Cmd_39,				// Process 64 port command 39
	Cmd_3A,				// Process 64 port command 3A
	Cmd_3B,				// Process 64 port command 3B
	Cmd_3C,				// Process 64 port command 3C
	Cmd_3D,				// Process 64 port command 3D
	Cmd_3E,				// Process 64 port command 3E
	Cmd_3F				// Process 64 port command 3F
};

void Cmd_3X(void)
{
	(Cmd3X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command 4x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd4X_table[16] =
{
	Cmd_40,				// Process 64 port command 40
	Cmd_41,				// Process 64 port command 41
	Cmd_42,				// Process 64 port command 42
	Cmd_43,				// Process 64 port command 43
	Cmd_44,				// Process 64 port command 44
	Cmd_45,				// Process 64 port command 45
	Cmd_46,				// Process 64 port command 46
	Cmd_47,				// Process 64 port command 47
	Cmd_48,				// Process 64 port command 48
	Cmd_49,				// Process 64 port command 49
	Cmd_4A,				// Process 64 port command 4A
	Cmd_4B,				// Process 64 port command 4B
	Cmd_4C,				// Process 64 port command 4C
	Cmd_4D,				// Process 64 port command 4D
	Cmd_4E,				// Process 64 port command 4E
	Cmd_4F				// Process 64 port command 4F
};

void Cmd_4X(void)
{
	(Cmd4X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command 5x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd5X_table[16] =
{
	Cmd_50,				// Process 64 port command 50
	Cmd_51,				// Process 64 port command 51
	Cmd_52,				// Process 64 port command 52
	Cmd_53,				// Process 64 port command 53
	Cmd_54,				// Process 64 port command 54
	Cmd_55,				// Process 64 port command 55
	Cmd_56,				// Process 64 port command 56
	Cmd_57,				// Process 64 port command 57
	Cmd_58,				// Process 64 port command 58
	Cmd_59,				// Process 64 port command 59
	Cmd_5A,				// Process 64 port command 5A
	Cmd_5B,				// Process 64 port command 5B
	Cmd_5C,				// Process 64 port command 5C
	Cmd_5D,				// Process 64 port command 5D
	Cmd_5E,				// Process 64 port command 5E
	Cmd_5F				// Process 64 port command 5F
};

void Cmd_5X(void)
{
	(Cmd5X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command 6x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Handle command 60 - Write command byte to KBC
//-----------------------------------------------------------------------
void Cmd_60(void)

{
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command 61 -
//-----------------------------------------------------------------------
void Cmd_61(void)

{

}

//-----------------------------------------------------------------------
// Handle command 62 -
//-----------------------------------------------------------------------
void Cmd_62(void)

{

}

//-----------------------------------------------------------------------
// Handle command 63 -
//-----------------------------------------------------------------------
void Cmd_63(void)

{

}

//-----------------------------------------------------------------------
// Handle command 64 -
//-----------------------------------------------------------------------
void Cmd_64(void)

{

}

//-----------------------------------------------------------------------
// Handle command 65 -
//-----------------------------------------------------------------------
void Cmd_65(void)

{

}

//-----------------------------------------------------------------------
// Handle command 66 -
//-----------------------------------------------------------------------
void Cmd_66(void)

{

}

//-----------------------------------------------------------------------
// Handle command 67 -
//-----------------------------------------------------------------------
void Cmd_67(void)

{

}

//-----------------------------------------------------------------------
// Handle command 68 -
//-----------------------------------------------------------------------
void Cmd_68(void)

{

}

//-----------------------------------------------------------------------
// Handle command 69 -
//-----------------------------------------------------------------------
void Cmd_69(void)

{

}

//-----------------------------------------------------------------------
// Handle command 6A -
//-----------------------------------------------------------------------
void Cmd_6A(void)

{

}

//-----------------------------------------------------------------------
// Handle command 6B -
//-----------------------------------------------------------------------
void Cmd_6B(void)

{

}

//-----------------------------------------------------------------------
// Handle command 6C -
//-----------------------------------------------------------------------
void Cmd_6C(void)

{

}

//-----------------------------------------------------------------------
// Handle command 6D -
//-----------------------------------------------------------------------
void Cmd_6D(void)

{

}

//-----------------------------------------------------------------------
// Handle command 6E -
//-----------------------------------------------------------------------
void Cmd_6E(void)

{

}

//-----------------------------------------------------------------------
// Handle command 6F -
//-----------------------------------------------------------------------
void Cmd_6F(void)

{

}

const FUNCT_PTR_V_V code Cmd6X_table[16] =
{
	Cmd_60,				// Process 64 port command 60
	Cmd_61,				// Process 64 port command 61
	Cmd_62,				// Process 64 port command 62
	Cmd_63,				// Process 64 port command 63
	Cmd_64,				// Process 64 port command 64
	Cmd_65,				// Process 64 port command 65
	Cmd_66,				// Process 64 port command 66
	Cmd_67,				// Process 64 port command 67
	Cmd_68,				// Process 64 port command 68
	Cmd_69,				// Process 64 port command 69
	Cmd_6A,				// Process 64 port command 6A
	Cmd_6B,				// Process 64 port command 6B
	Cmd_6C,				// Process 64 port command 6C
	Cmd_6D,				// Process 64 port command 6D
	Cmd_6E,				// Process 64 port command 6E
	Cmd_6F				// Process 64 port command 6F
};

void Cmd_6X(void)
{
	(Cmd6X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command 7x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Handle command 70 -
//-----------------------------------------------------------------------
void Cmd_70(void)

{

}

//-----------------------------------------------------------------------
// Handle command 71 -
//-----------------------------------------------------------------------
void Cmd_71(void)

{

}

//-----------------------------------------------------------------------
// Handle command 72 -
//-----------------------------------------------------------------------
void Cmd_72(void)

{

}

//-----------------------------------------------------------------------
// Handle command 73 - Write security Code On byte.
//-----------------------------------------------------------------------
void Cmd_73(void)

{
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command 74 - Write security Code Off byte.
//-----------------------------------------------------------------------
void Cmd_74(void)

{
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command 75 -
//-----------------------------------------------------------------------
void Cmd_75(void)

{

}

//-----------------------------------------------------------------------
// Handle command 76 - Write Password Code Discard 1
//-----------------------------------------------------------------------
void Cmd_76(void)

{
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command 77 - Write Password Code Discard 2
//-----------------------------------------------------------------------
void Cmd_77(void)

{
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command 78 -
//-----------------------------------------------------------------------
void Cmd_78(void)

{

}

//-----------------------------------------------------------------------
// Handle command 79 -
//-----------------------------------------------------------------------
void Cmd_79(void)

{

}

//-----------------------------------------------------------------------
// Handle command 7A -
//-----------------------------------------------------------------------
void Cmd_7A(void)

{

}

//-----------------------------------------------------------------------
// Handle command 7B -
//-----------------------------------------------------------------------
void Cmd_7B(void)

{

}

//-----------------------------------------------------------------------
// Handle command 7C -
//-----------------------------------------------------------------------
void Cmd_7C(void)

{

}

//-----------------------------------------------------------------------
// Handle command 7D -
//-----------------------------------------------------------------------
void Cmd_7D(void)

{

}

//-----------------------------------------------------------------------
// Handle command 7E -
//-----------------------------------------------------------------------
void Cmd_7E(void)

{

}

//-----------------------------------------------------------------------
// Handle command 7F -
//-----------------------------------------------------------------------
void Cmd_7F(void)

{

}

const FUNCT_PTR_V_V code Cmd7X_table[16] =
{
	Cmd_70,				// Process 64 port command 70
	Cmd_71,				// Process 64 port command 71
	Cmd_72,				// Process 64 port command 72
	Cmd_73,				// Process 64 port command 73
	Cmd_74,				// Process 64 port command 74
	Cmd_75,				// Process 64 port command 75
	Cmd_76,				// Process 64 port command 76
	Cmd_77,				// Process 64 port command 77
	Cmd_78,				// Process 64 port command 78
	Cmd_79,				// Process 64 port command 79
	Cmd_7A,				// Process 64 port command 7A
	Cmd_7B,				// Process 64 port command 7B
	Cmd_7C,				// Process 64 port command 7C
	Cmd_7D,				// Process 64 port command 7D
	Cmd_7E,				// Process 64 port command 7E
	Cmd_7F				// Process 64 port command 7F
};

void Cmd_7X(void)
{
	(Cmd7X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command 8x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd8X_table[16] =
{
	Cmd_80,				// Process 64 port command 80
	Cmd_81,				// Process 64 port command 81
	Cmd_82,				// Process 64 port command 82
	Cmd_83,				// Process 64 port command 83
	Cmd_84,				// Process 64 port command 84
	Cmd_85,				// Process 64 port command 85
	Cmd_86,				// Process 64 port command 86
	Cmd_87,				// Process 64 port command 87
	Cmd_88,				// Process 64 port command 88
	Cmd_89,				// Process 64 port command 89
	Cmd_8A,				// Process 64 port command 8A
	Cmd_8B,				// Process 64 port command 8B
	Cmd_8C,				// Process 64 port command 8C
	Cmd_8D,				// Process 64 port command 8D
	Cmd_8E,				// Process 64 port command 8E
	Cmd_8F				// Process 64 port command 8F
};

void Cmd_8X(void)
{
	(Cmd8X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command 9x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Handle command 90 -
//-----------------------------------------------------------------------
void Cmd_90(void)

{
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command 91 -
//-----------------------------------------------------------------------
void Cmd_91(void)

{
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command 92 -
//-----------------------------------------------------------------------
void Cmd_92(void)

{
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command 93 -
//-----------------------------------------------------------------------
void Cmd_93(void)

{
	SetHandleCmdData(1);
}

const FUNCT_PTR_V_V code Cmd9X_table[16] =
{
	Cmd_90,				// Process 64 port command 90
	Cmd_91,				// Process 64 port command 91
	Cmd_92,				// Process 64 port command 92
	Cmd_93,				// Process 64 port command 93
	Cmd_94,				// Process 64 port command 94
	Cmd_95,				// Process 64 port command 95
	Cmd_96,				// Process 64 port command 96
	Cmd_97,				// Process 64 port command 97
	Cmd_98,				// Process 64 port command 98
	Cmd_99,				// Process 64 port command 99
	Cmd_9A,				// Process 64 port command 9A
	Cmd_9B,				// Process 64 port command 9B
	Cmd_9C,				// Process 64 port command 9C
	Cmd_9D,				// Process 64 port command 9D
	Cmd_9E,				// Process 64 port command 9E
	Cmd_9F				// Process 64 port command 9F
};

void Cmd_9X(void)
{
	(Cmd9X_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command Ax.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Handle command A0 -
//-----------------------------------------------------------------------
void Cmd_A0(void)

{

}

//-----------------------------------------------------------------------
// Handle command A1 -
//-----------------------------------------------------------------------
void Cmd_A1(void)

{

}

//-----------------------------------------------------------------------
// Handle command A2 -
//-----------------------------------------------------------------------
void Cmd_A2(void)

{

}

//-----------------------------------------------------------------------
// Handle command A3 -
//-----------------------------------------------------------------------
void Cmd_A3(void)

{

}

//-----------------------------------------------------------------------
// Handle command A4 - Test Password Installed
// Return: data to send to HOST (FA or F1)
//-----------------------------------------------------------------------
void Cmd_A4(void)

{
	if (Flag.PASS_READY)
	{
		ResponseKBData(0xFA);	// Return FA if password is loaded
	}
	else
	{
		ResponseKBData(0xF1);	// Return F1 if password not loaded
	}
}

//-----------------------------------------------------------------------
// Handle command A5 - Load Password
//-----------------------------------------------------------------------
void Cmd_A5(void)

{
	Tmp_Load = 0x00;
	SetHandleCmdData(8);
}

//-----------------------------------------------------------------------
// Handle command A6 - Enable Password
//-----------------------------------------------------------------------
void Cmd_A6(void)

{
	if (Flag.PASS_READY)			// At this point, a password is loaded.
	{
		// Enable inhibit switch
		Gen_Info_PASS_ENABLE = 1;	// Enable password
		Pass_Buff_Idx = 0;			// Clear password buffer index
		Flag.SCAN_INH = 0;			// Clear internal keyboard inhibit
		ResponseKBData(Pass_On);
	}
	else
	{
		// At this point, a password is not loaded.
		ResponseKBData(0x00);
	}
}

//-----------------------------------------------------------------------
// Handle command A7 - Disable Aux Device Interface
//-----------------------------------------------------------------------
void Cmd_A7(void)

{
	TouchpadSWFnDebounce = 20;
	Ccb42_DISAB_AUX = 1;	// Disable auxiliary device (mouse)
}

//-----------------------------------------------------------------------
// Handle command A8 - Enable Auxiliary Device Interface
//-----------------------------------------------------------------------
void Cmd_A8(void)

{
	TouchpadSWFnDebounce = 20;
	Ccb42_DISAB_AUX = 0;	// Enable aux device (mouse)
}

//-----------------------------------------------------------------------
// Handle command A9 - Test Aux Device Interface
// Returns: test error code:
//             0 = no error
//             1 = Clock line stuck low
//             2 = Clock line stuck high
//             3 = Data line stuck low
//             4 = Data line stuck high
//-----------------------------------------------------------------------
void Cmd_A9(void)

{
	MULPX_Multiplex = 0;
	ResponseKBData(0x00);
}

//-----------------------------------------------------------------------
// Handle command AA - Self Test
// Returns: 0x55 to signify that the test passed
//-----------------------------------------------------------------------
void Cmd_AA(void)

{
	#if	TouchPad_only
	ScanAUXDevice(ScanMouseChannel);	// Scan Mouse channel
	#else
	ScanAUXDevice(ScanAllPS2Channel);	// Scan all PS2 channels
	#endif
	ClrFlag(SYS_MISC2, CrisisCheck);	// leox_20110930 [PATCH] ClearFlag() -> ClrFlag()
	MULPX_Multiplex = 0;		// Dino MUX ps2
	Data_To_Host(0x55);
	//KBC_DataToHost(0x55);
}

//-----------------------------------------------------------------------
// Handle command AB - Test Keyboard Interface
// Returns: test error code:
//             0 = no error
//             1 = Clock line stuck low
//             2 = Clock line stuck high
//             3 = Data line stuck low
//             4 = Data line stuck high
//-----------------------------------------------------------------------
void Cmd_AB(void)

{
	ResponseKBData(0x00);
}

//-----------------------------------------------------------------------
// Handle command AC - Diagnostic Dump
//-----------------------------------------------------------------------
void Cmd_AC(void)

{
	//Kbd_Response = respCMD_AC;// Send multibyte sequence.
	//Tmp_Byte[0] = 0;			// Tmp_Byte[0] will be used as the index for data.
}

//-----------------------------------------------------------------------
// Handle command AD - Disable Keyboard Interface
//-----------------------------------------------------------------------
void Cmd_AD(void)

{
	Ccb42_DISAB_KEY = 1;		// Disable auxiliary keyboard.
	Flag.SCAN_INH = 1;			// Inhibit scanner (internal keyboard).
	Lock_Scan();				// Lock scanner.
	//Load_Timer_B();			// Start inhibit delay timer.
}

//-----------------------------------------------------------------------
// Handle command AE - Enable Keyboard Interface
//-----------------------------------------------------------------------
void Cmd_AE(void)

{
	Ccb42_DISAB_KEY = 0;		// Enable auxiliary keyboard.
	Flag.SCAN_INH = 0;			// Enable scanner (internal keyboard).
}

//-----------------------------------------------------------------------
// Handle command AF -
//-----------------------------------------------------------------------
void Cmd_AF(void)

{

}

const FUNCT_PTR_V_V code CmdAX_table[16] =
{
	Cmd_A0,				// Process 64 port command A0
	Cmd_A1,				// Process 64 port command A1
	Cmd_A2,				// Process 64 port command A2
	Cmd_A3,				// Process 64 port command A3
	Cmd_A4,				// Process 64 port command A4
	Cmd_A5,				// Process 64 port command A5
	Cmd_A6,				// Process 64 port command A6
	Cmd_A7,				// Process 64 port command A7
	Cmd_A8,				// Process 64 port command A8
	Cmd_A9,				// Process 64 port command A9
	Cmd_AA,				// Process 64 port command AA
	Cmd_AB,				// Process 64 port command AB
	Cmd_AC,				// Process 64 port command AC
	Cmd_AD,				// Process 64 port command AD
	Cmd_AE,				// Process 64 port command AE
	Cmd_AF				// Process 64 port command AF
};

void Cmd_AX(void)
{
	(CmdAX_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command Bx.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code CmdBX_table[16] =
{
	Cmd_B0,				// Process 64 port command B0
	Cmd_B1,				// Process 64 port command B1
	Cmd_B2,				// Process 64 port command B2
	Cmd_B3,				// Process 64 port command B3
	Cmd_B4,				// Process 64 port command B4
	Cmd_B5,				// Process 64 port command B5
	Cmd_B6,				// Process 64 port command B6
	Cmd_B7,				// Process 64 port command B7
	Cmd_B8,				// Process 64 port command B8
	Cmd_B9,				// Process 64 port command B9
	Cmd_BA,				// Process 64 port command BA
	Cmd_BB,				// Process 64 port command BB
	Cmd_BC,				// Process 64 port command BC
	Cmd_BD,				// Process 64 port command BD
	Cmd_BE,				// Process 64 port command BE
	Cmd_BF				// Process 64 port command BF
};

void Cmd_BX(void)
{
	(CmdBX_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command Cx.
// Return: Byte from input port.
//     Bit 7 = Keyboard not inhibited via switch.
//     Bit 6 = Monochrome adapter (0 = Color/Graphics adapter).
//     Bit 5 = Manufacturing jumper not installed.
//     Bit 4 = Enable 2nd 256K of system board RAM.
//     Bit 3 =
//     Bit 2 =
//     Bit 1 = Auxiliary data in line (PS/2 only).
//     Bit 0 = Keyboard data in line (PS/2 only).
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Handle command C0 - Emulate reading the 8042 Input port and send data
//                      to the system
//-----------------------------------------------------------------------
void Cmd_C0(void)

{
	ResponseKBData(0xBF);	//  Just return the compatibility value for now. //return (PCIN | 0x1F);
}

//-----------------------------------------------------------------------
// Handle command C1 -
//-----------------------------------------------------------------------
void Cmd_C1(void)

{

}

//-----------------------------------------------------------------------
// Handle command C2 -
//-----------------------------------------------------------------------
void Cmd_C2(void)

{

}

//-----------------------------------------------------------------------
// Handle command C3 -
//-----------------------------------------------------------------------
void Cmd_C3(void)

{

}

//-----------------------------------------------------------------------
// Handle command C4 -
//-----------------------------------------------------------------------
void Cmd_C4(void)

{

}

//-----------------------------------------------------------------------
// Handle command C5 -
//-----------------------------------------------------------------------
void Cmd_C5(void)

{

}

//-----------------------------------------------------------------------
// Handle command C6 -
//-----------------------------------------------------------------------
void Cmd_C6(void)

{

}

//-----------------------------------------------------------------------
// Handle command C7 -
//-----------------------------------------------------------------------
void Cmd_C7(void)

{

}

const FUNCT_PTR_V_V code CmdCX_table[16] =
{
	Cmd_C0,				// Process 64 port command C0
	Cmd_C1,				// Process 64 port command C1
	Cmd_C2,				// Process 64 port command C2
	Cmd_C3,				// Process 64 port command C3
	Cmd_C4,				// Process 64 port command C4
	Cmd_C5,				// Process 64 port command C5
	Cmd_C6,				// Process 64 port command C6
	Cmd_C7,				// Process 64 port command C7
	Cmd_C8,				// Process 64 port command C8
	Cmd_C9,				// Process 64 port command C9
	Cmd_CA,				// Process 64 port command CA
	Cmd_CB,				// Process 64 port command CB
	Cmd_CC,				// Process 64 port command CC
	Cmd_CD,				// Process 64 port command CD
	Cmd_CE,				// Process 64 port command CE
	Cmd_CF				// Process 64 port command CF
};

void Cmd_CX(void)
{
	(CmdCX_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command Dx.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Handle command D0 - Send 8042 Output port value to the system
//						(emulates data since there¡¦s no real Output port)
//-----------------------------------------------------------------------
void Cmd_D0(void)

{
	ResponseKBData(Read_Output_Port_2());
}

//-----------------------------------------------------------------------
// Handle command D1 - Only set/reset GateA20 line based on the System Data bit1.
//-----------------------------------------------------------------------
void Cmd_D1(void)

{
	#if	1	// leox_20110920 [PATCH]
	SetHandleCmdData(1);
	#else	// (original)
	FastA20 = 1;
	#endif
}

//-----------------------------------------------------------------------
// Handle command D2 - Send data to the system as if it came from the keyboard.
//-----------------------------------------------------------------------
void Cmd_D2(void)

{
	TR1 = 0;					// Disable timer1
	ET1 = 0;					// Disable timer1 interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	TH1 = Timer_20ms >> 8;		// Set timer1 counter 20ms
	TL1 = Timer_20ms;			// Set timer1 counter 20ms
	TF1 = 0;					// Clear overflow flag
	TR1 = 1;					// Enable timer1

	while (!TF1)
	{
		if (IS_MASK_SET(KBHISR, IBF))
		{
			if (IS_MASK_CLEAR(KBHISR, C_D))	// Data Port
			{
				KBHIData = KBHIDIR;
				Data_To_Host(KBHIData);
			}
			TR1 = 0;			// Disable timer 1
			TF1 = 0;			// clear overflow flag
			ET1 = 1;			// Enable timer1 interrupt
			return;
		}
	}

	TR1 = 0;					// Disable timer 1
	TF1 = 0;					// clear overflow flag
	ET1 = 1;					// Enable timer1 interrupt
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command D3 - Send data to the system as if it came from the auxiliary device.
//-----------------------------------------------------------------------
void Cmd_D3(void)

{
	TR1 = 0;					// Disable timer1
	ET1 = 0;					// Disable timer1 interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	TH1 = Timer_20ms >> 8;		// Set timer1 counter 20ms
	TL1 = Timer_20ms;			// Set timer1 counter 20ms
	TF1 = 0;					// Clear overflow flag
	TR1 = 1;					// Enable timer1

	while (!TF1)
	{
		if (IS_MASK_SET(KBHISR, IBF))
		{
			if (IS_MASK_CLEAR(KBHISR, C_D))	// Data Port
			{
				KBHIData = KBHIDIR;
				#if	MultiplexedModes
				Aux_Data_To_Host(CheckAuxMux());
				#else
				Aux_Data_To_Host(KBHIData);
				#endif
			}
			TR1 = 0;			// Disable timer 1
			TF1 = 0;			// clear overflow flag
			ET1 = 1;			// Enable timer1 interrupt
			return;
		}
	}

	TR1 = 0;					// Disable timer 1
	TF1 = 0;					// clear overflow flag
	ET1 = 1;					// Enable timer1 interrupt
	SetHandleCmdData(1);
}

//-----------------------------------------------------------------------
// Handle command D4 - Output next received byte of data from system to auxiliary device.
//-----------------------------------------------------------------------
void Cmd_D4(void)

{
	SetHandleCmdData(1);
	MULPX_Multiplex = 0;		// Dino MUX ps2
}

//-----------------------------------------------------------------------
// Handle command D5 -
//-----------------------------------------------------------------------
void Cmd_D5(void)

{

}

//-----------------------------------------------------------------------
// Handle command D6 -
//-----------------------------------------------------------------------
void Cmd_D6(void)

{

}

//-----------------------------------------------------------------------
// Handle command D7 -
//-----------------------------------------------------------------------
void Cmd_D7(void)

{

}

const FUNCT_PTR_V_V code CmdDX_table[16] =
{
	Cmd_D0,				// Process 64 port command D0
	Cmd_D1,				// Process 64 port command D1
	Cmd_D2,				// Process 64 port command D2
	Cmd_D3,				// Process 64 port command D3
	Cmd_D4,				// Process 64 port command D4
	Cmd_D5,				// Process 64 port command D5
	Cmd_D6,				// Process 64 port command D6
	Cmd_D7,				// Process 64 port command D7
	Cmd_D8,				// Process 64 port command D8
	Cmd_D9,				// Process 64 port command D9
	Cmd_DA,				// Process 64 port command DA
	Cmd_DB,				// Process 64 port command DB
	Cmd_DC,				// Process 64 port command DC
	Cmd_DD,				// Process 64 port command DD
	Cmd_DE,				// Process 64 port command DE
	Cmd_DF				// Process 64 port command DF
};

void Cmd_DX(void)
{
	(CmdDX_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command Ex.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Handle command E0 - Reports the state of the test inputs
//-----------------------------------------------------------------------
void Cmd_E0(void)

{
	ResponseKBData(0x00);
}

//-----------------------------------------------------------------------
// Handle command E1 -
//-----------------------------------------------------------------------
void Cmd_E1(void)

{

}

const FUNCT_PTR_V_V code CmdEX_table[16] =
{
	Cmd_E0,				// Process 64 port command E0
	Cmd_E1,				// Process 64 port command E1
	Cmd_E2,				// Process 64 port command E2
	Cmd_E3,				// Process 64 port command E3
	Cmd_E4,				// Process 64 port command E4
	Cmd_E5,				// Process 64 port command E5
	Cmd_E6,				// Process 64 port command E6
	Cmd_E7,				// Process 64 port command E7
	Cmd_E8,				// Process 64 port command E8
	Cmd_E9,				// Process 64 port command E9
	Cmd_EA,				// Process 64 port command EA
	Cmd_EB,				// Process 64 port command EB
	Cmd_EC,				// Process 64 port command EC
	Cmd_ED,				// Process 64 port command ED
	Cmd_EE,				// Process 64 port command EE
	Cmd_EF				// Process 64 port command EF
};

void Cmd_EX(void)
{
	(CmdEX_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 64 port command Fx.
//-----------------------------------------------------------------------
void Cmd_FX(void)
{
	if ((KBHICmd % 2) == 0x00)	// Even command
	{
		Hook_Port64CmdFX();		// leox_20110926 [PATCH]
		//CLEAR_MASK(KBHISR,SYSF);
		GA20_on();
		RCIN_on();
		//Microsecond_Delay(64);	// Delay.
		WNCKR = 0x00;			// Delay 15.26 us
		WNCKR = 0x00;			// Delay 15.26 us
		WNCKR = 0x00;			// Delay 15.26 us
		WNCKR = 0x00;			// Delay 15.26 us
		WNCKR = 0x00;			// Delay 15.26 us
		RCIN_off();
	}							// Odd command do no thing
}

//-----------------------------------------------------------------------
// Handle 60 port data 0x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd0XData_table[16] =
{
	Cmd_00Data,				// Process 64 command data 00
	Cmd_01Data,				// Process 64 command data 01
	Cmd_02Data,				// Process 64 command data 02
	Cmd_03Data,				// Process 64 command data 03
	Cmd_04Data,				// Process 64 command data 04
	Cmd_05Data,				// Process 64 command data 05
	Cmd_06Data,				// Process 64 command data 06
	Cmd_07Data,				// Process 64 command data 07
	Cmd_08Data,				// Process 64 command data 08
	Cmd_09Data,				// Process 64 command data 09
	Cmd_0AData,				// Process 64 command data 0A
	Cmd_0BData,				// Process 64 command data 0B
	Cmd_0CData,				// Process 64 command data 0C
	Cmd_0DData,				// Process 64 command data 0D
	Cmd_0EData,				// Process 64 command data 0E
	Cmd_0FData				// Process 64 command data 0F
};

void Cmd_0XData(void)
{
	(Cmd0XData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data 1x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd1XData_table[16] =
{
	Cmd_10Data,				// Process 64 command data 10
	Cmd_11Data,				// Process 64 command data 11
	Cmd_12Data,				// Process 64 command data 12
	Cmd_13Data,				// Process 64 command data 13
	Cmd_14Data,				// Process 64 command data 14
	Cmd_15Data,				// Process 64 command data 15
	Cmd_16Data,				// Process 64 command data 16
	Cmd_17Data,				// Process 64 command data 17
	Cmd_18Data,				// Process 64 command data 18
	Cmd_19Data,				// Process 64 command data 19
	Cmd_1AData,				// Process 64 command data 1A
	Cmd_1BData,				// Process 64 command data 1B
	Cmd_1CData,				// Process 64 command data 1C
	Cmd_1DData,				// Process 64 command data 1D
	Cmd_1EData,				// Process 64 command data 1E
	Cmd_1FData				// Process 64 command data 1F
};

void Cmd_1XData(void)
{
	(Cmd1XData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data 2x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Process command data 20 -
//-----------------------------------------------------------------------
void Cmd_20Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 21 -
//-----------------------------------------------------------------------
void Cmd_21Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 22 -
//-----------------------------------------------------------------------
void Cmd_22Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 23 -
//-----------------------------------------------------------------------
void Cmd_23Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 24 -
//-----------------------------------------------------------------------
void Cmd_24Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 25 -
//-----------------------------------------------------------------------
void Cmd_25Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 26 -
//-----------------------------------------------------------------------
void Cmd_26Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 27 -
//-----------------------------------------------------------------------
void Cmd_27Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 28 -
//-----------------------------------------------------------------------
void Cmd_28Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 29 -
//-----------------------------------------------------------------------
void Cmd_29Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 2A -
//-----------------------------------------------------------------------
void Cmd_2AData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 2B -
//-----------------------------------------------------------------------
void Cmd_2BData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 2C -
//-----------------------------------------------------------------------
void Cmd_2CData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 2D -
//-----------------------------------------------------------------------
void Cmd_2DData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 2E -
//-----------------------------------------------------------------------
void Cmd_2EData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 2F -
//-----------------------------------------------------------------------
void Cmd_2FData(void)
{

}

const FUNCT_PTR_V_V code Cmd2XData_table[16] =
{
	Cmd_20Data,				// Process 64 command data 20
	Cmd_21Data,				// Process 64 command data 21
	Cmd_22Data,				// Process 64 command data 22
	Cmd_23Data,				// Process 64 command data 23
	Cmd_24Data,				// Process 64 command data 24
	Cmd_25Data,				// Process 64 command data 25
	Cmd_26Data,				// Process 64 command data 26
	Cmd_27Data,				// Process 64 command data 27
	Cmd_28Data,				// Process 64 command data 28
	Cmd_29Data,				// Process 64 command data 29
	Cmd_2AData,				// Process 64 command data 2A
	Cmd_2BData,				// Process 64 command data 2B
	Cmd_2CData,				// Process 64 command data 2C
	Cmd_2DData,				// Process 64 command data 2D
	Cmd_2EData,				// Process 64 command data 2E
	Cmd_2FData				// Process 64 command data 2F
};

void Cmd_2XData(void)
{
	(Cmd2XData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data 3x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Process command data 30 -
//-----------------------------------------------------------------------
void Cmd_30Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 31 -
//-----------------------------------------------------------------------
void Cmd_31Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 32 -
//-----------------------------------------------------------------------
void Cmd_32Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 33 -
//-----------------------------------------------------------------------
void Cmd_33Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 34 -
//-----------------------------------------------------------------------
void Cmd_34Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 35 -
//-----------------------------------------------------------------------
void Cmd_35Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 36 -
//-----------------------------------------------------------------------
void Cmd_36Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 37 -
//-----------------------------------------------------------------------
void Cmd_37Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 38 -
//-----------------------------------------------------------------------
void Cmd_38Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 39 -
//-----------------------------------------------------------------------
void Cmd_39Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 3A -
//-----------------------------------------------------------------------
void Cmd_3AData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 3B -
//-----------------------------------------------------------------------
void Cmd_3BData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 3C -
//-----------------------------------------------------------------------
void Cmd_3CData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 3D -
//-----------------------------------------------------------------------
void Cmd_3DData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 3E -
//-----------------------------------------------------------------------
void Cmd_3EData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 3F -
//-----------------------------------------------------------------------
void Cmd_3FData(void)
{

}

const FUNCT_PTR_V_V code Cmd3XData_table[16] =
{
	Cmd_30Data,				// Process 64 command data 30
	Cmd_31Data,				// Process 64 command data 31
	Cmd_32Data,				// Process 64 command data 32
	Cmd_33Data,				// Process 64 command data 33
	Cmd_34Data,				// Process 64 command data 34
	Cmd_35Data,				// Process 64 command data 35
	Cmd_36Data,				// Process 64 command data 36
	Cmd_37Data,				// Process 64 command data 37
	Cmd_38Data,				// Process 64 command data 38
	Cmd_39Data,				// Process 64 command data 39
	Cmd_3AData,				// Process 64 command data 3A
	Cmd_3BData,				// Process 64 command data 3B
	Cmd_3CData,				// Process 64 command data 3C
	Cmd_3DData,				// Process 64 command data 3D
	Cmd_3EData,				// Process 64 command data 3E
	Cmd_3FData				// Process 64 command data 3F
};

void Cmd_3XData(void)
{
	(Cmd3XData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data 4x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd4XData_table[16] =
{
	Cmd_40Data,				// Process 64 command data 40
	Cmd_41Data,				// Process 64 command data 41
	Cmd_42Data,				// Process 64 command data 42
	Cmd_43Data,				// Process 64 command data 43
	Cmd_44Data,				// Process 64 command data 44
	Cmd_45Data,				// Process 64 command data 45
	Cmd_46Data,				// Process 64 command data 46
	Cmd_47Data,				// Process 64 command data 47
	Cmd_48Data,				// Process 64 command data 48
	Cmd_49Data,				// Process 64 command data 49
	Cmd_4AData,				// Process 64 command data 4A
	Cmd_4BData,				// Process 64 command data 4B
	Cmd_4CData,				// Process 64 command data 4C
	Cmd_4DData,				// Process 64 command data 4D
	Cmd_4EData,				// Process 64 command data 4E
	Cmd_4FData				// Process 64 command data 4F
};

void Cmd_4XData(void)
{
	(Cmd4XData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data 5x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd5XData_table[16] =
{
	Cmd_50Data,				// Process 64 command data 50
	Cmd_51Data,				// Process 64 command data 51
	Cmd_52Data,				// Process 64 command data 52
	Cmd_53Data,				// Process 64 command data 53
	Cmd_54Data,				// Process 64 command data 54
	Cmd_55Data,				// Process 64 command data 55
	Cmd_56Data,				// Process 64 command data 56
	Cmd_57Data,				// Process 64 command data 57
	Cmd_58Data,				// Process 64 command data 58
	Cmd_59Data,				// Process 64 command data 59
	Cmd_5AData,				// Process 64 command data 5A
	Cmd_5BData,				// Process 64 command data 5B
	Cmd_5CData,				// Process 64 command data 5C
	Cmd_5DData,				// Process 64 command data 5D
	Cmd_5EData,				// Process 64 command data 5E
	Cmd_5FData				// Process 64 command data 5F
};

void Cmd_5XData(void)
{
	(Cmd5XData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data 6x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Process command data 60 - Write command byte
//-----------------------------------------------------------------------
void Cmd_60Data(void)
{
	Ccb42 = KBHIData;	// Write the data.
	//if (Ext_Cb0_PS2_AT)	// Reconfigure variables/registers that are dependent on Command Byte value.
	//{
	//	if (Ccb42_DISAB_AUX)	// For PS/2 style 8042 interface.
	//	{
	//		Cmd_A7();	// Disable aux device interface.
	//	}
	//	else
	//	{
	//		Cmd_A8();	// Enable Auxiliary Device Interface.
	//	}
	//}

	if (Ccb42_DISAB_KEY)
	{
		Cmd_AD();		// Disable keyboard interface.
	}
	else
	{
		Cmd_AE();		// Enable keyboard interface.
	}


	if (Ccb42_SYS_FLAG)	// Put system flag bit in Status Reg.
	{
		SET_MASK(KBHISR, SYSF);
	}
	else
	{
		CLEAR_MASK(KBHISR, SYSF);
	}
}

//-----------------------------------------------------------------------
// Process command data 61 -
//-----------------------------------------------------------------------
void Cmd_61Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 62 -
//-----------------------------------------------------------------------
void Cmd_62Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 63 -
//-----------------------------------------------------------------------
void Cmd_63Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 64 -
//-----------------------------------------------------------------------
void Cmd_64Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 65 -
//-----------------------------------------------------------------------
void Cmd_65Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 66 -
//-----------------------------------------------------------------------
void Cmd_66Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 67 -
//-----------------------------------------------------------------------
void Cmd_67Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 68 -
//-----------------------------------------------------------------------
void Cmd_68Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 69 -
//-----------------------------------------------------------------------
void Cmd_69Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 6A -
//-----------------------------------------------------------------------
void Cmd_6AData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 6B -
//-----------------------------------------------------------------------
void Cmd_6BData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 6C -
//-----------------------------------------------------------------------
void Cmd_6CData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 6D -
//-----------------------------------------------------------------------
void Cmd_6DData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 6E -
//-----------------------------------------------------------------------
void Cmd_6EData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 6F -
//-----------------------------------------------------------------------
void Cmd_6FData(void)
{

}

const FUNCT_PTR_V_V code Cmd6XData_table[16] =
{
	Cmd_60Data,				// Process 64 command data 60
	Cmd_61Data,				// Process 64 command data 61
	Cmd_62Data,				// Process 64 command data 62
	Cmd_63Data,				// Process 64 command data 63
	Cmd_64Data,				// Process 64 command data 64
	Cmd_65Data,				// Process 64 command data 65
	Cmd_66Data,				// Process 64 command data 66
	Cmd_67Data,				// Process 64 command data 67
	Cmd_68Data,				// Process 64 command data 68
	Cmd_69Data,				// Process 64 command data 69
	Cmd_6AData,				// Process 64 command data 6A
	Cmd_6BData,				// Process 64 command data 6B
	Cmd_6CData,				// Process 64 command data 6C
	Cmd_6DData,				// Process 64 command data 6D
	Cmd_6EData,				// Process 64 command data 6E
	Cmd_6FData				// Process 64 command data 6F
};

void Cmd_6XData(void)
{
	(Cmd6XData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data 7x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Process command data 70 -
//-----------------------------------------------------------------------
void Cmd_70Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 71 -
//-----------------------------------------------------------------------
void Cmd_71Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 72 -
//-----------------------------------------------------------------------
void Cmd_72Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 71 - Write Security Code On byte
//-----------------------------------------------------------------------
void Cmd_73Data(void)
{
	Pass_On = KBHIData;		// Write the data.
}

//-----------------------------------------------------------------------
// Process command data 74 - Write Security Code Off byte
//-----------------------------------------------------------------------
void Cmd_74Data(void)
{
	Pass_Off = KBHIData;	// Write the data.
}

//-----------------------------------------------------------------------
// Process command data 75 -
//-----------------------------------------------------------------------
void Cmd_75Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 76 - Reject make code 1
//-----------------------------------------------------------------------
void Cmd_76Data(void)
{
	Pass_Make1 = KBHIData;	// Write the data.
}

//-----------------------------------------------------------------------
// Process command data 77 - Reject make code 2
//-----------------------------------------------------------------------
void Cmd_77Data(void)
{
	Pass_Make2 = KBHIData;	// Write the data.
}

//-----------------------------------------------------------------------
// Process command data 78 -
//-----------------------------------------------------------------------
void Cmd_78Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 79 -
//-----------------------------------------------------------------------
void Cmd_79Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data 7A -
//-----------------------------------------------------------------------
void Cmd_7AData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 7B -
//-----------------------------------------------------------------------
void Cmd_7BData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 7C -
//-----------------------------------------------------------------------
void Cmd_7CData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 7D -
//-----------------------------------------------------------------------
void Cmd_7DData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 7E -
//-----------------------------------------------------------------------
void Cmd_7EData(void)
{

}

//-----------------------------------------------------------------------
// Process command data 7F -
//-----------------------------------------------------------------------
void Cmd_7FData(void)
{

}

const FUNCT_PTR_V_V code Cmd7XData_table[16] =
{
	Cmd_70Data,				// Process 64 command data 70
	Cmd_71Data,				// Process 64 command data 71
	Cmd_72Data,				// Process 64 command data 72
	Cmd_73Data,				// Process 64 command data 73
	Cmd_74Data,				// Process 64 command data 74
	Cmd_75Data,				// Process 64 command data 75
	Cmd_76Data,				// Process 64 command data 76
	Cmd_77Data,				// Process 64 command data 77
	Cmd_78Data,				// Process 64 command data 78
	Cmd_79Data,				// Process 64 command data 79
	Cmd_7AData,				// Process 64 command data 7A
	Cmd_7BData,				// Process 64 command data 7B
	Cmd_7CData,				// Process 64 command data 7C
	Cmd_7DData,				// Process 64 command data 7D
	Cmd_7EData,				// Process 64 command data 7E
	Cmd_7FData				// Process 64 command data 7F
};


void Cmd_7XData(void)
{
	(Cmd7XData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data 8x.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code Cmd8XData_table[16] =
{
	Cmd_80Data,				// Process 64 command data 80
	Cmd_81Data,				// Process 64 command data 81
	Cmd_82Data,				// Process 64 command data 82
	Cmd_83Data,				// Process 64 command data 83
	Cmd_84Data,				// Process 64 command data 84
	Cmd_85Data,				// Process 64 command data 85
	Cmd_86Data,				// Process 64 command data 86
	Cmd_87Data,				// Process 64 command data 87
	Cmd_88Data,				// Process 64 command data 88
	Cmd_89Data,				// Process 64 command data 89
	Cmd_8AData,				// Process 64 command data 8A
	Cmd_8BData,				// Process 64 command data 8B
	Cmd_8CData,				// Process 64 command data 8C
	Cmd_8DData,				// Process 64 command data 8D
	Cmd_8EData,				// Process 64 command data 8E
	Cmd_8FData				// Process 64 command data 8F
};

void Cmd_8XData(void)
{
	(Cmd8XData_table[KBHICmd & 0x0F])();
}

// Dino MUX ps2 -->
//-----------------------------------------------------------------------
// Handle 60 port data 9x.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Process command data 90 - MUX AUX port #0
//-----------------------------------------------------------------------
void Cmd_90Data(void)			// always send timeout error to system
{
	#if	PS2_MSCMD
		#ifdef	UART_Debug
		printf("\nMS 90Port CMD : 0X%bX\n", KBHIData);
		#else
		RamDebug(0x90);
		RamDebug(KBHIData);
		#endif
	#endif

	#if	(!TouchPad_only)
	if (ECSendtoAUXFlag)
	{
		if (WaitECSend2PortACK(AUX_SCAN_INDEX) == 0x01)
		{
			AUXDevicePlugOutClearVariables();
		}
	}

	SetPS2SkipScanTime(100);	// 1Sec
	#endif

	if (KBHIData == 0xFF)
	{
		ScanAUXDevice(ScanAllPS2Channel);
	}

	KBHISR = 0x24;					// set error bit and AUX bit, source bits is 00
	KBHICR &= 0xFC;
	if (Ccb42_INTR_AUX)				// If AUX IRQ bit of command is present
	{
		SET_MASK(KBHICR, OBFMIE);	// Enable IRQ
	}

	DelayXms(5);					// Emulate transmission delay times
	KBHIMDOR = 0xFC;				// timeout error
}

//-----------------------------------------------------------------------
// Process command data 91 - MUX AUX port #1 internal mouse
//-----------------------------------------------------------------------
void Cmd_91Data(void)
{
	#if	PS2_MSCMD
		#ifdef	UART_Debug
		printf("\nMS 91Port CMD : 0X%bX\n", KBHIData);
		#else
		RamDebug(0x91);
		RamDebug(KBHIData);
		#endif
	#endif

	if (IS_MASK_SET(AuxFlags[0], DEVICE_IS_MOUSE))
	{
		SendCmdtoMUX(0);			// Send command to channel 0 mouse
	}
	else
	{
		KBHISR = 0x40;
		DelayXms(5);				// Emulate transmission delay times
		KBHIMDOR = 0xFC;			// timeout error
	}
}

//-----------------------------------------------------------------------
// Process command data 92 - MUX AUX port #2
//-----------------------------------------------------------------------
void Cmd_92Data(void)
{
	#if	PS2_MSCMD
		#ifdef	UART_Debug
		printf("\nMS 92Port CMD : 0X%bX\n", KBHIData);
		#else
		RamDebug(0x92);
		RamDebug(KBHIData);
		#endif
	#endif

	if (IS_MASK_SET(AuxFlags[1], DEVICE_IS_MOUSE))
	{
		SendCmdtoMUX(1);			// Send command to channel 0 mouse
	}
	else
	{
		KBHISR = 0x80;
		DelayXms(5);				// Emulate transmission delay times
		KBHIMDOR = 0xFC;			// timeout error
	}
}

//-----------------------------------------------------------------------
// Process command data 93 - MUX AUX port #3 external mouse
//-----------------------------------------------------------------------
void Cmd_93Data(void)
{
	#if	PS2_MSCMD
		#ifdef	UART_Debug
		printf("\nMS 93Port CMD : 0X%bX\n", KBHIData);
		#else
		RamDebug(0x93);
		RamDebug(KBHIData);
		#endif
	#endif

	if (IS_MASK_SET(AuxFlags[2], DEVICE_IS_MOUSE))
	{
		SendCmdtoMUX(2);			// Send command to channel 2 mouse
	}
	else
	{
		KBHISR = 0xC0;				// source bits is 11
		DelayXms(5);				// Emulate transmission delay times
		KBHIMDOR = 0xFC;
	}
}
// Dino MUX ps2 <--

const FUNCT_PTR_V_V code Cmd9XData_table[16] =
{
	Cmd_90Data,				// Process 64 command data 90
	Cmd_91Data,				// Process 64 command data 91
	Cmd_92Data,				// Process 64 command data 92
	Cmd_93Data,				// Process 64 command data 93
	Cmd_94Data,				// Process 64 command data 94
	Cmd_95Data,				// Process 64 command data 95
	Cmd_96Data,				// Process 64 command data 96
	Cmd_97Data,				// Process 64 command data 97
	Cmd_98Data,				// Process 64 command data 98
	Cmd_99Data,				// Process 64 command data 99
	Cmd_9AData,				// Process 64 command data 9A
	Cmd_9BData,				// Process 64 command data 9B
	Cmd_9CData,				// Process 64 command data 9C
	Cmd_9DData,				// Process 64 command data 9D
	Cmd_9EData,				// Process 64 command data 9E
	Cmd_9FData				// Process 64 command data 9F
};

void Cmd_9XData(void)
{
	(Cmd9XData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data Ax.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Process command data A0 -
//-----------------------------------------------------------------------
void Cmd_A0Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data A1 -
//-----------------------------------------------------------------------
void Cmd_A1Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data A2 -
//-----------------------------------------------------------------------
void Cmd_A2Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data A3 -
//-----------------------------------------------------------------------
void Cmd_A3Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data A4 -
//-----------------------------------------------------------------------
void Cmd_A4Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data A5 - Load Password
//-----------------------------------------------------------------------
void Cmd_A5Data(void)
{
	if ((Tmp_Load == 0) && (KBHIData == 0))	// The first byte is NULL
	{
		Flag.PASS_READY = 0;	// clear password ready flag and exit
		// Cmd_Byte has already been set to 0.  So the code will
		// NOT wait for next byte
	}
	else
	{
		if (Tmp_Load < 8)		// PASS_SIZE = 8
		{
			// If there is room in the buffer
			Pass_Buff[Tmp_Load] = KBHIData;	// Store scan code
			Tmp_Load++;			// Increment password buffer index
		}
		//Cmd_Byte = 0xA5;		// Set to keep waiting for next byte
		if (KBHIData == 0)		// The last byte (null terminated string) has been stored
		{
			//Cmd_Byte = 0;		// Clear out the command byte.
			Flag.PASS_READY = 1;	// Set password ready bit.
		}
	}
}

//-----------------------------------------------------------------------
// Process command data A6 -
//-----------------------------------------------------------------------
void Cmd_A6Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data A7 -
//-----------------------------------------------------------------------
void Cmd_A7Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data A8 -
//-----------------------------------------------------------------------
void Cmd_A8Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data A9 -
//-----------------------------------------------------------------------
void Cmd_A9Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data AA -
//-----------------------------------------------------------------------
void Cmd_AAData(void)
{

}

//-----------------------------------------------------------------------
// Process command data AB -
//-----------------------------------------------------------------------
void Cmd_ABData(void)
{

}

//-----------------------------------------------------------------------
// Process command data AC -
//-----------------------------------------------------------------------
void Cmd_ACData(void)
{

}

//-----------------------------------------------------------------------
// Process command data AD -
//-----------------------------------------------------------------------
void Cmd_ADData(void)
{

}

//-----------------------------------------------------------------------
// Process command data AE -
//-----------------------------------------------------------------------
void Cmd_AEData(void)
{

}

//-----------------------------------------------------------------------
// Process command data AF -
//-----------------------------------------------------------------------
void Cmd_AFData(void)
{

}

const FUNCT_PTR_V_V code CmdAXData_table[16] =
{
	Cmd_A0Data,				// Process 64 command data A0
	Cmd_A1Data,				// Process 64 command data A1
	Cmd_A2Data,				// Process 64 command data A2
	Cmd_A3Data,				// Process 64 command data A3
	Cmd_A4Data,				// Process 64 command data A4
	Cmd_A5Data,				// Process 64 command data A5
	Cmd_A6Data,				// Process 64 command data A6
	Cmd_A7Data,				// Process 64 command data A7
	Cmd_A8Data,				// Process 64 command data A8
	Cmd_A9Data,				// Process 64 command data A9
	Cmd_AAData,				// Process 64 command data AA
	Cmd_ABData,				// Process 64 command data AB
	Cmd_ACData,				// Process 64 command data AC
	Cmd_ADData,				// Process 64 command data AD
	Cmd_AEData,				// Process 64 command data AE
	Cmd_AFData				// Process 64 command data AF
};

void Cmd_AXData(void)
{
	(CmdAXData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data Bx.
//-----------------------------------------------------------------------
const FUNCT_PTR_V_V code CmdBXData_table[16] =
{
	Cmd_B0Data,				// Process 64 command data B0
	Cmd_B1Data,				// Process 64 command data B1
	Cmd_B2Data,				// Process 64 command data B2
	Cmd_B3Data,				// Process 64 command data B3
	Cmd_B4Data,				// Process 64 command data B4
	Cmd_B5Data,				// Process 64 command data B5
	Cmd_B6Data,				// Process 64 command data B6
	Cmd_B7Data,				// Process 64 command data B7
	Cmd_B8Data,				// Process 64 command data B8
	Cmd_B9Data,				// Process 64 command data B9
	Cmd_BAData,				// Process 64 command data BA
	Cmd_BBData,				// Process 64 command data BB
	Cmd_BCData,				// Process 64 command data BC
	Cmd_BDData,				// Process 64 command data BD
	Cmd_BEData,				// Process 64 command data BE
	Cmd_BFData				// Process 64 command data BF
};

void Cmd_BXData(void)
{
	(CmdBXData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data Cx.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Process command data C0 -
//-----------------------------------------------------------------------
void Cmd_C0Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data C1 -
//-----------------------------------------------------------------------
void Cmd_C1Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data C2 -
//-----------------------------------------------------------------------
void Cmd_C2Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data C3 -
//-----------------------------------------------------------------------
void Cmd_C3Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data C4 -
//-----------------------------------------------------------------------
void Cmd_C4Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data C5 -
//-----------------------------------------------------------------------
void Cmd_C5Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data C6 -
//-----------------------------------------------------------------------
void Cmd_C6Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data C7 -
//-----------------------------------------------------------------------
void Cmd_C7Data(void)
{

}

const FUNCT_PTR_V_V code CmdCXData_table[16] =
{
	Cmd_C0Data,				// Process 64 command data C0
	Cmd_C1Data,				// Process 64 command data C1
	Cmd_C2Data,				// Process 64 command data C2
	Cmd_C3Data,				// Process 64 command data C3
	Cmd_C4Data,				// Process 64 command data C4
	Cmd_C5Data,				// Process 64 command data C5
	Cmd_C6Data,				// Process 64 command data C6
	Cmd_C7Data,				// Process 64 command data C7
	Cmd_C8Data,				// Process 64 command data C8
	Cmd_C9Data,				// Process 64 command data C9
	Cmd_CAData,				// Process 64 command data CA
	Cmd_CBData,				// Process 64 command data CB
	Cmd_CCData,				// Process 64 command data CC
	Cmd_CDData,				// Process 64 command data CD
	Cmd_CEData,				// Process 64 command data CE
	Cmd_CFData				// Process 64 command data CF
};

void Cmd_CXData(void)
{
	(CmdCXData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data Dx.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Process command data D0 -
//-----------------------------------------------------------------------
void Cmd_D0Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data D1 - set/reset GateA20 line based on the System Data bit1.
//-----------------------------------------------------------------------
void Cmd_D1Data(void)
{
	if ((KBHIData & 0x02) != 0x00)
	{
		GA20_on();
	}
	else
	{
		GA20_off();
	}
}

//-----------------------------------------------------------------------
// Process command data D2 - Send data back to the system as if it came from the keyboard.
//-----------------------------------------------------------------------
void Cmd_D2Data(void)
{
	Data_To_Host(KBHIData);
}

//-----------------------------------------------------------------------
// Process command data D3 - Send data back to the system as if it came from the auxiliary device.
//-----------------------------------------------------------------------
void Cmd_D3Data(void)				// Dino MUX ps2
{
	#if	MultiplexedModes
	Aux_Data_To_Host(CheckAuxMux());
	#else
	Aux_Data_To_Host(KBHIData);
	#endif
}

//-----------------------------------------------------------------------
// Process command data D4 - Output next received byte of data from system to auxiliary device.
//-----------------------------------------------------------------------
void Cmd_D4Data(void)
{
	#if	PS2_MSCMD
		#ifdef	UART_Debug
		printf("\nMS CMD : 0X%bX\n", KBHIData);
		#else
		RamDebug(0xD4);
		RamDebug(KBHIData);
		#endif
	#endif

	if (KBHIData == 0xFF)					// if is reset command
	{
		ScanAUXDevice(ScanMouseChannel);	// Scan mouse channel
	}

	if (Main_MOUSE_CHN != 0x00)				// mouse device is attached
	{
		CLEAR_MASK(AuxFlags[(Main_MOUSE_CHN - 1)], DEVICE_NEED_INIT);
		CLEAR_MASK(AuxFlags[(Main_MOUSE_CHN - 1)], DEVICE_NEED_CONFIG);
		SendCmdtoMouse((Main_MOUSE_CHN - 1));
	}
	else
	{
		DelayXms(20);					// Emulate transmission delay times
		SendFromAux(0xFC);
	}
}

//-----------------------------------------------------------------------
// Process command data D5 -
//-----------------------------------------------------------------------
void Cmd_D5Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data D6 -
//-----------------------------------------------------------------------
void Cmd_D6Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data D7 -
//-----------------------------------------------------------------------
void Cmd_D7Data(void)
{

}

const FUNCT_PTR_V_V code CmdDXData_table[16] =
{
	Cmd_D0Data,				// Process 64 command data D0
	Cmd_D1Data,				// Process 64 command data D1
	Cmd_D2Data,				// Process 64 command data D2
	Cmd_D3Data,				// Process 64 command data D3
	Cmd_D4Data,				// Process 64 command data D4
	Cmd_D5Data,				// Process 64 command data D5
	Cmd_D6Data,				// Process 64 command data D6
	Cmd_D7Data,				// Process 64 command data D7
	Cmd_D8Data,				// Process 64 command data D8
	Cmd_D9Data,				// Process 64 command data D9
	Cmd_DAData,				// Process 64 command data DA
	Cmd_DBData,				// Process 64 command data DB
	Cmd_DCData,				// Process 64 command data DC
	Cmd_DDData,				// Process 64 command data DD
	Cmd_DEData,				// Process 64 command data DE
	Cmd_DFData				// Process 64 command data DF
};

void Cmd_DXData(void)
{
	(CmdDXData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data Ex.
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Process command data E0 -
//-----------------------------------------------------------------------
void Cmd_E0Data(void)
{

}

//-----------------------------------------------------------------------
// Process command data E1 -
//-----------------------------------------------------------------------
void Cmd_E1Data(void)
{

}

const FUNCT_PTR_V_V code CmdEXData_table[16] =
{
	Cmd_E0Data,				// Process 64 command data E0
	Cmd_E1Data,				// Process 64 command data E1
	Cmd_E2Data,				// Process 64 command data E2
	Cmd_E3Data,				// Process 64 command data E3
	Cmd_E4Data,				// Process 64 command data E4
	Cmd_E5Data,				// Process 64 command data E5
	Cmd_E6Data,				// Process 64 command data E6
	Cmd_E7Data,				// Process 64 command data E7
	Cmd_E8Data,				// Process 64 command data E8
	Cmd_E9Data,				// Process 64 command data E9
	Cmd_EAData,				// Process 64 command data EA
	Cmd_EBData,				// Process 64 command data EB
	Cmd_ECData,				// Process 64 command data EC
	Cmd_EDData,				// Process 64 command data ED
	Cmd_EEData,				// Process 64 command data EE
	Cmd_EFData				// Process 64 command data EF
};

void Cmd_EXData(void)
{
	(CmdEXData_table[KBHICmd & 0x0F])();
}

//-----------------------------------------------------------------------
// Handle 60 port data Fx.
//-----------------------------------------------------------------------
void Cmd_FXData(void)
{

}

//-----------------------------------------------------------------------
// Handle 60 port data Fx.
//-----------------------------------------------------------------------
void ResponseKBData(BYTE RData)
{
	KBHIReponse = RData;
	KbdNeedResponseFlag = 1;
}

//-----------------------------------------------------------------------
// Handle 60 port data Fx.
//-----------------------------------------------------------------------
void SetHandleCmdData(BYTE step)
{
	KBHIStep = step;
}

/* ----------------------------------------------------------------------------
 * FUNCTION: vKeyboardCmd
 *
 * Passes data sent from the Host (port 60 write) to the keyboard.
 *
 * Input: data = data byte to send
 * ------------------------------------------------------------------------- */
static void KeyboardCmd(BYTE nKB60DAT)
{
	BYTE ack, ack1, ack2, index, cmdbk;

	KBCmdAckByteCunt(0x00);			// New keyboard command, clear ack counter
	ack = 0x00;
	ack1 = 0x00;
	ack2 = 0x00;

	//-------------------------------------------------------------------------
	//Keyboard Command Received Data Process
	//-------------------------------------------------------------------------
	if (KB_Command)
	{
		cmdbk = KB_Command;

		switch (KB_Command)
		{
		case 0xED:	/* Update LEDs command. */
			Led_Data = nKB60DAT;
			// leo_20160510 add for "00" key no output when NumLock off +++
			if (IS_MASK_SET(nKB60DAT, BIT1))	
			{
				SetFlag(Device_STAT_CTRL2, NumLock);
			}
			else
			{
				ClrFlag(Device_STAT_CTRL2, NumLock);
			}
			// leo_20160510 add for "00" key no output when NumLock off ---
			ack = 0xFA;
			KB_Command = 0x00;
			OEM_Write_Leds(Led_Data);	//Hook Oem On-board LED control

			/* Update scanner numlock state. */
			Scanner_State_NUM_LOCK = Led_Data_NUM;
			break;
		case 0xF0:	/* Set alternate scan codes. */
			KB_CodeSet = nKB60DAT;
			ack = 0xFA;
			KB_Command = 0x00;
			break;
		case 0xF3:	/* Set typematic rate/delay. */
			PS2KB_Typematic = nKB60DAT;
			Set_Typematic(PS2KB_Typematic);
			ack = 0xFA;
			KB_Command = 0x00;
			break;
		default:	//Unknown command request system resend
			ack = 0xFE;			//Resend
			//KBCUnProcessCnt++;
			break;
		}

		if ((cmdbk == 0xF3) || (cmdbk == 0xED))
		{
			KeyboardDriverIn = 1;
		}

		if (Main_KB_CHN == 0x00)			// no any external keyboard
		{
			if (ack != 0x00)				// if need send ack to host
			{
				KBC_DataToHost(ack);		// send to host
				#if	PS2_KBCMD
					#ifdef	UART_Debug
				printf("EC KB ACK : 0X%bX\n", ack);
					#else
				RamDebug(ack);
					#endif
				#endif
			}
		}
		else								// Send command to external keyboard
		{

			CLEAR_MASK(AuxFlags[(Main_KB_CHN - 1)], DEVICE_NEED_INIT);
			CLEAR_MASK(AuxFlags[(Main_KB_CHN - 1)], DEVICE_NEED_CONFIG);

			KBCmdAckByteCunt(0x01);			// set ec need send one byte to host

			if (cmdbk == 0xED)
			{
				Send2Port((Main_KB_CHN - 1), (nKB60DAT & 0x07), SendCmdToKB);
			}
			else
			{
				Send2Port((Main_KB_CHN - 1), nKB60DAT, SendCmdToKB);
			}

			if ((cmdbk == 0xF3) || (cmdbk == 0xED))
			{
				SetOtherKBNeedUpdataFlag(cmdbk);
			}
		}
		return;
	}

	//-------------------------------------------------------------------------
	//Keyboard Command Process
	//-------------------------------------------------------------------------
	switch (nKB60DAT)
	{
		KB_Command = 0x00;
	case 0xED:	/* Update LEDs command. */
		ack = 0xFA;
		KB_Command = 0xED;
		break;
	case 0xEC:
		ack = 0xFA;
		break;

	case 0xEE:	/* ECHO command. */
		ack = 0xEE;
		break;

	case 0xF0:	/* Set alternate scan codes. */
		ack = 0xFA;
		KB_Command = 0xF0;
		break;

	case 0xF2:	/* Read manufacturers ID */
		ack = 0xFA;
		ack1 = 0xAB;
		if (Ccb42_XLATE_PC)
		{
			ack2 = 0x83;
		}
		else
		{
			ack2 = 0x41;
		}
		break;

	case 0xF3:	/* Set typematic rate/delay. */
		ack = 0xFA;
		KB_Command = 0xF3;
		break;

	case 0xF4:	/* Enable scanning. */
		KeyboardDriverIn = 1;
		Clear_Key();
		ack = 0xFA;
		break;

	case 0xF5:	/* Default disable. */
		KeyboardDriverIn = 0;
		Clear_Key();
		Clear_Typematic();
		ack = 0xFA;
		break;

	case 0xF6:	/* Set defaults. */
		Clear_Key();
		Clear_Typematic();
		ack = 0xFA;
		break;
	case 0xF7:
	case 0xF8:
	case 0xF9:
	case 0xFA:
	case 0xFB:
		ack = 0xFA;
		break;

	case 0xFF:	/* Reset keyboard. */
		#ifdef	ITE_EVBoard
		//InitShareMemory();
		InitSio();
		#endif
		KeyboardDriverIn = 0;
		ScanAUXDevice(ScanKeyboardChannel);	// Scan keyboard channel
		Clear_Key();
		Clear_Typematic();
		Scanner_State = 0;
		Clear_Fn_Keys();
		ack = 0xFA;
		ack1 = 0xAA;

		for (index = 0x00; index < 3; index++)
		{
			if ((Main_KB_CHN - 1) != index)
			{
				if (IS_MASK_SET(AuxFlags[index], DEVICE_IS_KEYBOARD))
				{
					AuxFlags[index] = 0x00;
				}
			}
		}
		//+JayFn [PATCH]
		#if KB_FnStickKey
		FnStickKey = 0;
		#endif
		//-JayFn [PATCH]
		break;

	default:	//Unknown command request system resend
		ack = 0xFE;
		//KBCUnProcessCnt++;
		//KBCUnProcessRec = nKB60DAT;
		break;

	}

	if (Main_KB_CHN == 0x00)			// no any external keyboard
	{
		KBCmdAckByteCunt(0x00);

		if (ack != 0x00)
		{
			KBC_DataToHost(ack);

			#if	PS2_KBCMD
				#ifdef	UART_Debug
			printf("EC KB ACK : 0X%bX\n", ack);
				#else
			RamDebug(ack);
				#endif
			#endif

			if (ack1 != 0x00)
			{
				KBC_DataPending(ack1);

				#if	PS2_KBCMD
					#ifdef	UART_Debug
				printf("EC KB ACK : 0X%bX\n", ack1);
					#else
				RamDebug(ack1);
					#endif
				#endif

				if (ack2 != 0x00)
				{
					KBC_DataPending(ack2);

					#if	PS2_KBCMD
						#ifdef	UART_Debug
					printf("EC KB ACK : 0X%bX\n", ack2);
						#else
					RamDebug(ack2);
						#endif
					#endif
				}
			}
		}
	}
	else									// Send command to external keyboard
	{
		CLEAR_MASK(AuxFlags[(Main_KB_CHN - 1)], DEVICE_NEED_INIT);
		CLEAR_MASK(AuxFlags[(Main_KB_CHN - 1)], DEVICE_NEED_CONFIG);

		if (nKB60DAT == 0xFF)				// reset command
		{
			KBCmdAckByteCunt(0x02);
		}
		else
		{
			if (nKB60DAT == 0xF2)			// read keyboard ID command
			{
				KBCmdAckByteCunt(0x03);
			}
			else if ((nKB60DAT == 0xF3) || (nKB60DAT == 0xED))
			{
				KBCmdAckByteCunt(0x02);
			}
			else							// other
			{
				KBCmdAckByteCunt(0x01);
			}
		}

		Send2Port((Main_KB_CHN - 1), nKB60DAT, SendCmdToKB);

		if ((nKB60DAT == 0xF4) || (nKB60DAT == 0xF5))
		{
			SetOtherKBNeedUpdataFlag(nKB60DAT);
		}
	}
}


/* ----------------------------------------------------------------------------
 * FUNCTION: Set_Overlay_Mask
 *
 * Set the overlay state in the scanner_state variable and
 * set the led numlock and/or overlay bits for the LED data.
 *
 * Input: data = current LED data
 *        bit_state = 1 to set overlay/numlock bits
 *                  = 0 to clear overlay/numlock bits
 *
 * Return: updated LED data (num lock and overlay state updated)
 * ------------------------------------------------------------------------- */
BBYTE Set_Overlay_Mask(BBYTE data_byte, FLAG bit_state)
{
	BITS_8 temp;
	temp.byte = data_byte;

	if (Led_Ctrl_NUM)
	{
		temp.fbit.NUM = bit_state;
	}

	if (Led_Ctrl_OVL)
	{
		temp.fbit.OVL = bit_state;
	}

	data_byte = temp.byte;
	Scanner_State_NUM_LOCK = bit_state;
	//Ext_Cb3_OVL_STATE = bit_state;

	return(data_byte);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: Read_Input_Port_1
 *
 * Read 8042 Input Port 1 and return its contents.
 *
 * Return: Byte from input port.
 *     Bit 7 = Keyboard not inhibited via switch.
 *     Bit 6 = Monochrome adapter (0 = Color/Graphics adapter).
 *     Bit 5 = Manufacturing jumper not installed.
 *     Bit 4 = Enable 2nd 256K of system board RAM.
 *     Bit 3 =
 *     Bit 2 =
 *     Bit 1 = Auxiliary data in line (PS/2 only).
 *     Bit 0 = Keyboard data in line (PS/2 only).
 * ------------------------------------------------------------------------- */
//BYTE Read_Input_Port_1(void)
//{
//	return(0xBF);	// Just return the compatibility value for now. //return (PCIN | 0x1F);
//}


/* ----------------------------------------------------------------------------
 * FUNCTION: Read_Output_Port_2
 *
 * Read 8042 output port 2.
 *
 * Return: Byte from output port.
 *     Bit 7 = Keyboard data output line (inverted on PS/2).
 *     Bit 6 = Keyboard clock output line.
 *     Bit 5 = Input buffer empty (auxiliary interrupt IRQ12 on PS/2).
 *     Bit 4 = Output buffer full (Generates IRQ1).
 *     Bit 3 = Reserved (inverted auxiliary clock output line on PS/2).
 *     Bit 2 = Reserved (inverted auxiliary data output line on PS/2).
 *     Bit 1 = Gate A20.
 *     Bit 0 = System reset.
 * ------------------------------------------------------------------------- */
BYTE Read_Output_Port_2(void)
{
	#if	1
	BYTE p2byte;

	/* "data" will hold bits read from different inputs.
		Start with bit 0 set and set bits 2 and 3 for AT mode.
		If PS2 mode is being used, bits 2 and 3 will be changed. */
	p2byte = (1 << 3) | (1 << 2) | (1 << 0);

	//if (Ext_Cb0_PS2_AT)
	//{	/* If PS2 mode is enabled */
	/* Invert auxiliary keyboard data line (bit 7) and clear bits 2 and 3.
		(Since bits 2 and 3 were set before, they can be cleared by inverting.) */
	//	p2byte^= 0x8C;
	//}

	return (p2byte);
	#else

	BYTE data_byte;

	/* "data" will hold bits read from different inputs.
		Start with bit 0 set and set bits 2 and 3 for AT mode.
		If PS2 mode is being used, bits 2 and 3 will be changed. */
	data_byte = (1 << 3) | (1 << 2) | (1 << 0);

	/* Put Gate A20 bit value into bit 1. */

	if (Ext_Cb0_PS2_AT)
	{
		/* PS/2 mode is enabled 8/
		/* Invert aux keyboard data line (bit 7) and clear bit[3:2]. (Since
			bits 2 and 3 were set before, they can be cleared by inverting.) */
		data_byte ^= 0x8C;

		/* Put inverted auxiliary device (mouse) clock line in bit 3. */

		/* Put inverted auxiliary device (mouse) data line in bit 2. */
	}	/* if (Ext_Cb0_PS2_AT) */

	return (data_byte);
	#endif
}

