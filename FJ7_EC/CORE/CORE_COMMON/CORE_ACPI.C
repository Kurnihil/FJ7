/*------------------------------------------------------------------------------
 * Title: CORE_ACPI.C - ACPI Handler
 *
 * SCI event generation.
 * ACPI SMBus transfer initiated by ACPI command.
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

//----------------------------------------------------------------------------
// Process Command/Data received from System via the 2nd Host interface
//----------------------------------------------------------------------------
void service_pci2(void)
{
	if (IS_MASK_CLEAR(PM1STS, P_IBF))
	{
		return;
	}
	SetTotalBurstTime();	// Set Burst mode total time (2ms)
	service_pci2f();		// Process Command/Data
}

//----------------------------------------------------------------------------
// Process Command/Data received from System via the 2nd Host interface
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code Port66_table[16] =
{
	ACPI_Cmd_0X,			// Process ACPI command 0x
	ACPI_Cmd_1X,			// Process ACPI command 1x
	ACPI_Cmd_2X,			// Process ACPI command 2x
	ACPI_Cmd_3X,			// Process ACPI command 3x
	ACPI_Cmd_4X,			// Process ACPI command 4x
	ACPI_Cmd_5X,			// Process ACPI command 5x
	ACPI_Cmd_6X,			// Process ACPI command 6x
	ACPI_Cmd_7X,			// Process ACPI command 7x
	ACPI_Cmd_8X,			// Process ACPI command 8x
	ACPI_Cmd_9X,			// Process ACPI command 9x
	ACPI_Cmd_AX,			// Process ACPI command Ax
	ACPI_Cmd_BX,			// Process ACPI command Bx
	ACPI_Cmd_CX,			// Process ACPI command Cx
	ACPI_Cmd_DX,			// Process ACPI command Dx
	ACPI_Cmd_EX,			// Process ACPI command Ex
	ACPI_Cmd_FX,			// Process ACPI command Fx
};

const FUNCT_PTR_V_V code Port62_table[16] =
{
	ACPI_Cmd_0XData,		// Process ACPI data 0x
	ACPI_Cmd_1XData,		// Process ACPI data 1x
	ACPI_Cmd_2XData,		// Process ACPI data 2x
	ACPI_Cmd_3XData,		// Process ACPI data 3x
	ACPI_Cmd_4XData,		// Process ACPI data 4x
	ACPI_Cmd_5XData,		// Process ACPI data 5x
	ACPI_Cmd_6XData,		// Process ACPI data 6x
	ACPI_Cmd_7XData,		// Process ACPI data 7x
	ACPI_Cmd_8XData,		// Process ACPI data 8x
	ACPI_Cmd_9XData,		// Process ACPI data 9x
	ACPI_Cmd_AXData,		// Process ACPI data Ax
	ACPI_Cmd_BXData,		// Process ACPI data Bx
	ACPI_Cmd_CXData,		// Process ACPI data Cx
	ACPI_Cmd_DXData,		// Process ACPI data Dx
	ACPI_Cmd_EXData,		// Process ACPI data Ex
	ACPI_Cmd_FXData,		// Process ACPI data Fx
};

void service_pci2f(void)
{
	do
	{
		if (PM1STS & P_C_D)				// Command port
		{
			PM1Cmd  = PM1DI;			// Load command
			PM1Step = 0x00;
			Hook_66PortDebug();
			(Port66_table[PM1Cmd >> 4])();	// Handle command
		}
		else							// Data port
		{
			PM1Data = PM1DI;			// Load data
			Hook_62PortDebug();
			if (PM1Step != 0x00)
			{
				(Port62_table[PM1Cmd >> 4])();	// Handle command data
				PM1Step--;
			}
		}
	}
	while ((IS_MASK_SET(PM1STS, BURST) || ECCheckBurstMode) && (CheckBurstMode() == 1));

	TR1 = 0;			// disable timer1
	TF1 = 0;			// clear overflow flag
	ET1 = 1;			// Enable timer1 interrupt
	EnableAllInterrupt();
	EnablePMCIBFInt();
	#if	1	// Disable BIOS access battery by send protocol command (66/62) Ken Lee 2012/3/23 PM 08:09:42 [PATCH]
	if (IsFlag0(POWER_FLAG, ACPI_OS)) {Hook_DOSCommand();}
	#else	// (original)
	Hook_ACPICommand();
	#endif
}

//----------------------------------------------------------------------------
// Handle ACPI commands.
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Handle ACPI command 0x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd0X_table[16] =
{
	ACPI_Cmd_00,			// Process ACPI command 00
	ACPI_Cmd_01,			// Process ACPI command 01
	ACPI_Cmd_02,			// Process ACPI command 02
	ACPI_Cmd_03,			// Process ACPI command 03
	ACPI_Cmd_04,			// Process ACPI command 04
	ACPI_Cmd_05,			// Process ACPI command 05
	ACPI_Cmd_06,			// Process ACPI command 06
	ACPI_Cmd_07,			// Process ACPI command 07
	ACPI_Cmd_08,			// Process ACPI command 08
	ACPI_Cmd_09,			// Process ACPI command 09
	ACPI_Cmd_0A,			// Process ACPI command 0A
	ACPI_Cmd_0B,			// Process ACPI command 0B
	ACPI_Cmd_0C,			// Process ACPI command 0C
	ACPI_Cmd_0D,			// Process ACPI command 0D
	ACPI_Cmd_0E,			// Process ACPI command 0E
	ACPI_Cmd_0F				// Process ACPI command 0F
};

void ACPI_Cmd_0X(void)
{
	(ACPICmd0X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command 1x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd1X_table[16] =
{
	ACPI_Cmd_10,			// Process ACPI command 10
	ACPI_Cmd_11,			// Process ACPI command 11
	ACPI_Cmd_12,			// Process ACPI command 12
	ACPI_Cmd_13,			// Process ACPI command 13
	ACPI_Cmd_14,			// Process ACPI command 14
	ACPI_Cmd_15,			// Process ACPI command 15
	ACPI_Cmd_16,			// Process ACPI command 16
	ACPI_Cmd_17,			// Process ACPI command 17
	ACPI_Cmd_18,			// Process ACPI command 18
	ACPI_Cmd_19,			// Process ACPI command 19
	ACPI_Cmd_1A,			// Process ACPI command 1A
	ACPI_Cmd_1B,			// Process ACPI command 1B
	ACPI_Cmd_1C,			// Process ACPI command 1C
	ACPI_Cmd_1D,			// Process ACPI command 1D
	ACPI_Cmd_1E,			// Process ACPI command 1E
	ACPI_Cmd_1F				// Process ACPI command 1F
};

void ACPI_Cmd_1X(void)
{
	(ACPICmd1X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command 2x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd2X_table[16] =
{
	ACPI_Cmd_20,			// Process ACPI command 20
	ACPI_Cmd_21,			// Process ACPI command 21
	ACPI_Cmd_22,			// Process ACPI command 22
	ACPI_Cmd_23,			// Process ACPI command 23
	ACPI_Cmd_24,			// Process ACPI command 24
	ACPI_Cmd_25,			// Process ACPI command 25
	ACPI_Cmd_26,			// Process ACPI command 26
	ACPI_Cmd_27,			// Process ACPI command 27
	ACPI_Cmd_28,			// Process ACPI command 28
	ACPI_Cmd_29,			// Process ACPI command 29
	ACPI_Cmd_2A,			// Process ACPI command 2A
	ACPI_Cmd_2B,			// Process ACPI command 2B
	ACPI_Cmd_2C,			// Process ACPI command 2C
	ACPI_Cmd_2D,			// Process ACPI command 2D
	ACPI_Cmd_2E,			// Process ACPI command 2E
	ACPI_Cmd_2F				// Process ACPI command 2F
};

void ACPI_Cmd_2X(void)
{
	(ACPICmd2X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command 3x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd3X_table[16] =
{
	ACPI_Cmd_30,			// Process ACPI command 30
	ACPI_Cmd_31,			// Process ACPI command 31
	ACPI_Cmd_32,			// Process ACPI command 32
	ACPI_Cmd_33,			// Process ACPI command 33
	ACPI_Cmd_34,			// Process ACPI command 34
	ACPI_Cmd_35,			// Process ACPI command 35
	ACPI_Cmd_36,			// Process ACPI command 36
	ACPI_Cmd_37,			// Process ACPI command 37
	ACPI_Cmd_38,			// Process ACPI command 38
	ACPI_Cmd_39,			// Process ACPI command 39
	ACPI_Cmd_3A,			// Process ACPI command 3A
	ACPI_Cmd_3B,			// Process ACPI command 3B
	ACPI_Cmd_3C,			// Process ACPI command 3C
	ACPI_Cmd_3D,			// Process ACPI command 3D
	ACPI_Cmd_3E,			// Process ACPI command 3E
	ACPI_Cmd_3F				// Process ACPI command 3F
};

void ACPI_Cmd_3X(void)
{
	(ACPICmd3X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command 4x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd4X_table[16] =
{
	ACPI_Cmd_40,			// Process ACPI command 40
	ACPI_Cmd_41,			// Process ACPI command 41
	ACPI_Cmd_42,			// Process ACPI command 42
	ACPI_Cmd_43,			// Process ACPI command 43
	ACPI_Cmd_44,			// Process ACPI command 44
	ACPI_Cmd_45,			// Process ACPI command 45
	ACPI_Cmd_46,			// Process ACPI command 46
	ACPI_Cmd_47,			// Process ACPI command 47
	ACPI_Cmd_48,			// Process ACPI command 48
	ACPI_Cmd_49,			// Process ACPI command 49
	ACPI_Cmd_4A,			// Process ACPI command 4A
	ACPI_Cmd_4B,			// Process ACPI command 4B
	ACPI_Cmd_4C,			// Process ACPI command 4C
	ACPI_Cmd_4D,			// Process ACPI command 4D
	ACPI_Cmd_4E,			// Process ACPI command 4E
	ACPI_Cmd_4F				// Process ACPI command 4F
};

void ACPI_Cmd_4X(void)
{
	(ACPICmd4X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command 5x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd5X_table[16] =
{
	ACPI_Cmd_50,			// Process ACPI command 50
	ACPI_Cmd_51,			// Process ACPI command 51
	ACPI_Cmd_52,			// Process ACPI command 52
	ACPI_Cmd_53,			// Process ACPI command 53
	ACPI_Cmd_54,			// Process ACPI command 54
	ACPI_Cmd_55,			// Process ACPI command 55
	ACPI_Cmd_56,			// Process ACPI command 56
	ACPI_Cmd_57,			// Process ACPI command 57
	ACPI_Cmd_58,			// Process ACPI command 58
	ACPI_Cmd_59,			// Process ACPI command 59
	ACPI_Cmd_5A,			// Process ACPI command 5A
	ACPI_Cmd_5B,			// Process ACPI command 5B
	ACPI_Cmd_5C,			// Process ACPI command 5C
	ACPI_Cmd_5D,			// Process ACPI command 5D
	ACPI_Cmd_5E,			// Process ACPI command 5E
	ACPI_Cmd_5F				// Process ACPI command 5F
};

void ACPI_Cmd_5X(void)
{
	(ACPICmd5X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command 6x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd6X_table[16] =
{
	ACPI_Cmd_60,			// Process ACPI command 60
	ACPI_Cmd_61,			// Process ACPI command 61
	ACPI_Cmd_62,			// Process ACPI command 62
	ACPI_Cmd_63,			// Process ACPI command 63
	ACPI_Cmd_64,			// Process ACPI command 64
	ACPI_Cmd_65,			// Process ACPI command 65
	ACPI_Cmd_66,			// Process ACPI command 66
	ACPI_Cmd_67,			// Process ACPI command 67
	ACPI_Cmd_68,			// Process ACPI command 68
	ACPI_Cmd_69,			// Process ACPI command 69
	ACPI_Cmd_6A,			// Process ACPI command 6A
	ACPI_Cmd_6B,			// Process ACPI command 6B
	ACPI_Cmd_6C,			// Process ACPI command 6C
	ACPI_Cmd_6D,			// Process ACPI command 6D
	ACPI_Cmd_6E,			// Process ACPI command 6E
	ACPI_Cmd_6F				// Process ACPI command 6F
};

void ACPI_Cmd_6X(void)
{
	(ACPICmd6X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command 7x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd7X_table[16] =
{
	ACPI_Cmd_70,			// Process ACPI command 70
	ACPI_Cmd_71,			// Process ACPI command 71
	ACPI_Cmd_72,			// Process ACPI command 72
	ACPI_Cmd_73,			// Process ACPI command 73
	ACPI_Cmd_74,			// Process ACPI command 74
	ACPI_Cmd_75,			// Process ACPI command 75
	ACPI_Cmd_76,			// Process ACPI command 76
	ACPI_Cmd_77,			// Process ACPI command 77
	ACPI_Cmd_78,			// Process ACPI command 78
	ACPI_Cmd_79,			// Process ACPI command 79
	ACPI_Cmd_7A,			// Process ACPI command 7A
	ACPI_Cmd_7B,			// Process ACPI command 7B
	ACPI_Cmd_7C,			// Process ACPI command 7C
	ACPI_Cmd_7D,			// Process ACPI command 7D
	ACPI_Cmd_7E,			// Process ACPI command 7E
	ACPI_Cmd_7F				// Process ACPI command 7F
};

void ACPI_Cmd_7X(void)
{
	(ACPICmd7X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command 8x.
//----------------------------------------------------------------------------
void ACPI_Cmd_80(void)
// Read Embedded Controller.
{
	ACPI_Gen_Int(ACPICmdSCI);
	PM1Step = 0x01;
	if (IS_MASK_SET(POWER_FLAG, ACPI_OS))	// ACPI mode
	{
		if (IS_MASK_CLEAR(PM1STS, BURST))	// Non-burst mode
		{
			ECCheckBurstMode = 1;
		}
	}
}

void ACPI_Cmd_81(void)
// Write Embedded Controller.
{
	ACPI_Gen_Int(ACPICmdSCI);
	PM1Step = 0x02;
	if (IS_MASK_SET(POWER_FLAG, ACPI_OS))	// ACPI mode
	{
		if (IS_MASK_CLEAR(PM1STS, BURST))	// Non-burst mode
		{
			ECCheckBurstMode = 1;
		}
	}
}

void ACPI_Cmd_82(void)
// Burst Enable Embedded Controller.
{
	SET_MASK(PM1STS, BURST);	// PM1STS.4 Set Burst mode flag
	SendFromAcpi(0x90);		// Get Burst Acknowledge byte to send to Host.
	ECCheckBurstMode = 1;
}

void ACPI_Cmd_83(void)
// Burst Disable Embedded Controller.
{
	CLEAR_MASK(PM1STS, BURST);	// PM1STS.4 Set Burst mode flag
	ACPI_Gen_Int(ACPICmdSCI);	// Clear Burst bit in secondary Host interface status register.
	ECCheckBurstMode = 0;
}

void ACPI_Cmd_84(void)
// Query Embedded Controller.
{
	ITempB05 = 0x00;
	if (SCI_Event_Out_Index != SCI_Event_In_Index)
	{
		ITempB05 = SCI_Event_Buffer[SCI_Event_Out_Index];
		SCI_Event_Out_Index++;
		if (SCI_Event_Out_Index >= EVENT_BUFFER_SIZE)
		{
			SCI_Event_Out_Index = 0;
		}
		if (SCI_Event_Out_Index == SCI_Event_In_Index)
		{
			CLEAR_MASK(PM1STS, SCIEVT);
		}
		RdSCIevent_backup = ITempB05;	// leox_20120620 [PATCH] Requested by Ken Lee
		SendFromAcpi(ITempB05);
	}
	else
	{
		CLEAR_MASK(PM1STS, SCIEVT);
	}
}

const FUNCT_PTR_V_V code ACPICmd8X_table[16] =
{
	ACPI_Cmd_80,			// Process ACPI command 80
	ACPI_Cmd_81,			// Process ACPI command 81
	ACPI_Cmd_82,			// Process ACPI command 82
	ACPI_Cmd_83,			// Process ACPI command 83
	ACPI_Cmd_84,			// Process ACPI command 84
	ACPI_Cmd_85,			// Process ACPI command 85
	ACPI_Cmd_86,			// Process ACPI command 86
	ACPI_Cmd_87,			// Process ACPI command 87
	ACPI_Cmd_88,			// Process ACPI command 88
	ACPI_Cmd_89,			// Process ACPI command 89
	ACPI_Cmd_8A,			// Process ACPI command 8A
	ACPI_Cmd_8B,			// Process ACPI command 8B
	ACPI_Cmd_8C,			// Process ACPI command 8C
	ACPI_Cmd_8D,			// Process ACPI command 8D
	ACPI_Cmd_8E,			// Process ACPI command 8E
	ACPI_Cmd_8F				// Process ACPI command 8F
};
void ACPI_Cmd_8X(void)
{
	(ACPICmd8X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command 9x.
//----------------------------------------------------------------------------
void ACPI_Cmd_92(void)		// Oem function of reading EC external ram

{
	PM1Step = 0x02;
}

void ACPI_Cmd_93(void)		// Oem function of writing EC external ram

{
	PM1Step = 0x03;
}

const FUNCT_PTR_V_V code ACPICmd9X_table[16] =
{
	ACPI_Cmd_90,			// Process ACPI command 90
	ACPI_Cmd_91,			// Process ACPI command 91
	ACPI_Cmd_92,			// Process ACPI command 92
	ACPI_Cmd_93,			// Process ACPI command 93
	ACPI_Cmd_94,			// Process ACPI command 94
	ACPI_Cmd_95,			// Process ACPI command 95
	ACPI_Cmd_96,			// Process ACPI command 96
	ACPI_Cmd_97,			// Process ACPI command 97
	ACPI_Cmd_99,			// Process ACPI command 99
	ACPI_Cmd_99,			// Process ACPI command 99
	ACPI_Cmd_9A,			// Process ACPI command 9A
	ACPI_Cmd_9B,			// Process ACPI command 9B
	ACPI_Cmd_9C,			// Process ACPI command 9C
	ACPI_Cmd_9D,			// Process ACPI command 9D
	ACPI_Cmd_9E,			// Process ACPI command 9E
	ACPI_Cmd_9F				// Process ACPI command 9F
};
void ACPI_Cmd_9X(void)
{
	(ACPICmd9X_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command Ax.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmdAX_table[16] =
{
	ACPI_Cmd_A0,			// Process ACPI command A0
	ACPI_Cmd_A1,			// Process ACPI command A1
	ACPI_Cmd_A2,			// Process ACPI command A2
	ACPI_Cmd_A3,			// Process ACPI command A3
	ACPI_Cmd_A4,			// Process ACPI command A4
	ACPI_Cmd_A5,			// Process ACPI command A5
	ACPI_Cmd_A6,			// Process ACPI command A6
	ACPI_Cmd_A7,			// Process ACPI command A7
	ACPI_Cmd_A8,			// Process ACPI command A8
	ACPI_Cmd_A9,			// Process ACPI command A9
	ACPI_Cmd_AA,			// Process ACPI command AA
	ACPI_Cmd_AB,			// Process ACPI command AB
	ACPI_Cmd_AC,			// Process ACPI command AC
	ACPI_Cmd_AD,			// Process ACPI command AD
	ACPI_Cmd_AE,			// Process ACPI command AE
	ACPI_Cmd_AF				// Process ACPI command AF
};

void ACPI_Cmd_AX(void)
{
	(ACPICmdAX_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command Bx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmdBX_table[16] =
{
	ACPI_Cmd_B0,			// Process ACPI command B0
	ACPI_Cmd_B1,			// Process ACPI command B1
	ACPI_Cmd_B2,			// Process ACPI command B2
	ACPI_Cmd_B3,			// Process ACPI command B3
	ACPI_Cmd_B4,			// Process ACPI command B4
	ACPI_Cmd_B5,			// Process ACPI command B5
	ACPI_Cmd_B6,			// Process ACPI command B6
	ACPI_Cmd_B7,			// Process ACPI command B7
	ACPI_Cmd_B8,			// Process ACPI command B8
	ACPI_Cmd_B9,			// Process ACPI command B9
	ACPI_Cmd_BA,			// Process ACPI command BA
	ACPI_Cmd_BB,			// Process ACPI command BB
	ACPI_Cmd_BC,			// Process ACPI command BC
	ACPI_Cmd_BD,			// Process ACPI command BD
	ACPI_Cmd_BE,			// Process ACPI command BE
	ACPI_Cmd_BF				// Process ACPI command BF
};

void ACPI_Cmd_BX(void)
{
	(ACPICmdBX_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command Cx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmdCX_table[16] =
{
	ACPI_Cmd_C0,			// Process ACPI command C0
	ACPI_Cmd_C1,			// Process ACPI command C1
	ACPI_Cmd_C2,			// Process ACPI command C2
	ACPI_Cmd_C3,			// Process ACPI command C3
	ACPI_Cmd_C4,			// Process ACPI command C4
	ACPI_Cmd_C5,			// Process ACPI command C5
	ACPI_Cmd_C6,			// Process ACPI command C6
	ACPI_Cmd_C7,			// Process ACPI command C7
	ACPI_Cmd_C8,			// Process ACPI command C8
	ACPI_Cmd_C9,			// Process ACPI command C9
	ACPI_Cmd_CA,			// Process ACPI command CA
	ACPI_Cmd_CB,			// Process ACPI command CB
	ACPI_Cmd_CC,			// Process ACPI command CC
	ACPI_Cmd_CD,			// Process ACPI command CD
	ACPI_Cmd_CE,			// Process ACPI command CE
	ACPI_Cmd_CF				// Process ACPI command CF
};

void ACPI_Cmd_CX(void)
{
	(ACPICmdCX_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command Dx.
//----------------------------------------------------------------------------
void ACPI_Cmd_DC(void)
{
	ITE_Flash_Utility();	// for 8500
}

const FUNCT_PTR_V_V code ACPICmdDX_table[16] =
{
	ACPI_Cmd_D0,			// Process ACPI command D0
	ACPI_Cmd_D1,			// Process ACPI command D1
	ACPI_Cmd_D2,			// Process ACPI command D2
	ACPI_Cmd_D3,			// Process ACPI command D3
	ACPI_Cmd_D4,			// Process ACPI command D4
	ACPI_Cmd_D5,			// Process ACPI command D5
	ACPI_Cmd_D6,			// Process ACPI command D6
	ACPI_Cmd_D7,			// Process ACPI command D7
	ACPI_Cmd_D8,			// Process ACPI command D8
	ACPI_Cmd_D9,			// Process ACPI command D9
	ACPI_Cmd_DA,			// Process ACPI command DA
	ACPI_Cmd_DB,			// Process ACPI command DB
	ACPI_Cmd_DC,			// Process ACPI command DC
	ACPI_Cmd_DD,			// Process ACPI command DD
	ACPI_Cmd_DE,			// Process ACPI command DE
	ACPI_Cmd_DF				// Process ACPI command DF
};

void ACPI_Cmd_DX(void)
{
	(ACPICmdDX_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command Ex.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmdEX_table[16] =
{
	ACPI_Cmd_E0,			// Process ACPI command E0
	ACPI_Cmd_E1,			// Process ACPI command E1
	ACPI_Cmd_E2,			// Process ACPI command E2
	ACPI_Cmd_E3,			// Process ACPI command E3
	ACPI_Cmd_E4,			// Process ACPI command E4
	ACPI_Cmd_E5,			// Process ACPI command E5
	ACPI_Cmd_E6,			// Process ACPI command E6
	ACPI_Cmd_E7,			// Process ACPI command E7
	ACPI_Cmd_E8,			// Process ACPI command E8
	ACPI_Cmd_E9,			// Process ACPI command E9
	ACPI_Cmd_EA,			// Process ACPI command EA
	ACPI_Cmd_EB,			// Process ACPI command EB
	ACPI_Cmd_EC,			// Process ACPI command EC
	ACPI_Cmd_ED,			// Process ACPI command ED
	ACPI_Cmd_EE,			// Process ACPI command EE
	ACPI_Cmd_EF				// Process ACPI command EF
};

void ACPI_Cmd_EX(void)
{
	(ACPICmdEX_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command Fx.
//----------------------------------------------------------------------------
void ACPI_Cmd_F0(void)

{
	KeyScanACK = KU_ACK_Start;
}

void ACPI_Cmd_F1(void)

{

}

void ACPI_Cmd_F2(void)

{

}

void ACPI_Cmd_F3(void)

{

}

void ACPI_Cmd_F4(void)

{

}

void ACPI_Cmd_F5(void)

{

}

void ACPI_Cmd_F6(void)

{

}

void ACPI_Cmd_F7(void)

{

}

void ACPI_Cmd_F8(void)

{

}

void ACPI_Cmd_F9(void)

{

}

void ACPI_Cmd_FA(void)

{

}

void ACPI_Cmd_FB(void)

{

}

void ACPI_Cmd_FC(void)

{

}

void ACPI_Cmd_FD(void)

{

}

void ACPI_Cmd_FE(void)

{

}

void ACPI_Cmd_FF(void)

{

}

const FUNCT_PTR_V_V code ACPICmdFX_table[16] =
{
	ACPI_Cmd_F0,			// Process ACPI command F0
	ACPI_Cmd_F1,			// Process ACPI command F1
	ACPI_Cmd_F2,			// Process ACPI command F2
	ACPI_Cmd_F3,			// Process ACPI command F3
	ACPI_Cmd_F4,			// Process ACPI command F4
	ACPI_Cmd_F5,			// Process ACPI command F5
	ACPI_Cmd_F6,			// Process ACPI command F6
	ACPI_Cmd_F7,			// Process ACPI command F7
	ACPI_Cmd_F8,			// Process ACPI command F8
	ACPI_Cmd_F9,			// Process ACPI command F9
	ACPI_Cmd_FA,			// Process ACPI command FA
	ACPI_Cmd_FB,			// Process ACPI command FB
	ACPI_Cmd_FC,			// Process ACPI command FC
	ACPI_Cmd_FD,			// Process ACPI command FD
	ACPI_Cmd_FE,			// Process ACPI command FE
	ACPI_Cmd_FF				// Process ACPI command FF
};

void ACPI_Cmd_FX(void)
{
	(ACPICmdFX_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI command data.
//----------------------------------------------------------------------------
void SendFromAcpi(BYTE ecdata)
{
	PM1DO = ecdata;
	ACPI_Gen_Int(ACPICmdSCI);
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 0x
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd0XData_table[16] =
{
	ACPI_Cmd_00Data,		// Process ACPI command data 00
	ACPI_Cmd_01Data,		// Process ACPI command data 01
	ACPI_Cmd_02Data,		// Process ACPI command data 02
	ACPI_Cmd_03Data,		// Process ACPI command data 03
	ACPI_Cmd_04Data,		// Process ACPI command data 04
	ACPI_Cmd_05Data,		// Process ACPI command data 05
	ACPI_Cmd_06Data,		// Process ACPI command data 06
	ACPI_Cmd_07Data,		// Process ACPI command data 07
	ACPI_Cmd_08Data,		// Process ACPI command data 08
	ACPI_Cmd_09Data,		// Process ACPI command data 09
	ACPI_Cmd_0AData,		// Process ACPI command data 0A
	ACPI_Cmd_0BData,		// Process ACPI command data 0B
	ACPI_Cmd_0CData,		// Process ACPI command data 0C
	ACPI_Cmd_0DData,		// Process ACPI command data 0D
	ACPI_Cmd_0EData,		// Process ACPI command data 0E
	ACPI_Cmd_0FData			// Process ACPI command data 0F
};

void ACPI_Cmd_0XData(void)
{
	(ACPICmd0XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 1x
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd1XData_table[16] =
{
	ACPI_Cmd_10Data,		// Process ACPI command data 10
	ACPI_Cmd_11Data,		// Process ACPI command data 11
	ACPI_Cmd_12Data,		// Process ACPI command data 12
	ACPI_Cmd_13Data,		// Process ACPI command data 13
	ACPI_Cmd_14Data,		// Process ACPI command data 14
	ACPI_Cmd_15Data,		// Process ACPI command data 15
	ACPI_Cmd_16Data,		// Process ACPI command data 16
	ACPI_Cmd_17Data,		// Process ACPI command data 17
	ACPI_Cmd_18Data,		// Process ACPI command data 18
	ACPI_Cmd_19Data,		// Process ACPI command data 19
	ACPI_Cmd_1AData,		// Process ACPI command data 1A
	ACPI_Cmd_1BData,		// Process ACPI command data 1B
	ACPI_Cmd_1CData,		// Process ACPI command data 1C
	ACPI_Cmd_1DData,		// Process ACPI command data 1D
	ACPI_Cmd_1EData,		// Process ACPI command data 1E
	ACPI_Cmd_1FData			// Process ACPI command data 1F
};

void ACPI_Cmd_1XData(void)
{
	(ACPICmd1XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 2x
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd2XData_table[16] =
{
	ACPI_Cmd_20Data,		// Process ACPI command data 20
	ACPI_Cmd_21Data,		// Process ACPI command data 21
	ACPI_Cmd_22Data,		// Process ACPI command data 22
	ACPI_Cmd_23Data,		// Process ACPI command data 23
	ACPI_Cmd_24Data,		// Process ACPI command data 24
	ACPI_Cmd_25Data,		// Process ACPI command data 25
	ACPI_Cmd_26Data,		// Process ACPI command data 26
	ACPI_Cmd_27Data,		// Process ACPI command data 27
	ACPI_Cmd_28Data,		// Process ACPI command data 28
	ACPI_Cmd_29Data,		// Process ACPI command data 29
	ACPI_Cmd_2AData,		// Process ACPI command data 2A
	ACPI_Cmd_2BData,		// Process ACPI command data 2B
	ACPI_Cmd_2CData,		// Process ACPI command data 2C
	ACPI_Cmd_2DData,		// Process ACPI command data 2D
	ACPI_Cmd_2EData,		// Process ACPI command data 2E
	ACPI_Cmd_2FData			// Process ACPI command data 2F
};

void ACPI_Cmd_2XData(void)
{
	(ACPICmd2XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 3x
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd3XData_table[16] =
{
	ACPI_Cmd_30Data,		// Process ACPI command data 30
	ACPI_Cmd_31Data,		// Process ACPI command data 31
	ACPI_Cmd_32Data,		// Process ACPI command data 32
	ACPI_Cmd_33Data,		// Process ACPI command data 33
	ACPI_Cmd_34Data,		// Process ACPI command data 34
	ACPI_Cmd_35Data,		// Process ACPI command data 35
	ACPI_Cmd_36Data,		// Process ACPI command data 36
	ACPI_Cmd_37Data,		// Process ACPI command data 37
	ACPI_Cmd_38Data,		// Process ACPI command data 38
	ACPI_Cmd_39Data,		// Process ACPI command data 39
	ACPI_Cmd_3AData,		// Process ACPI command data 3A
	ACPI_Cmd_3BData,		// Process ACPI command data 3B
	ACPI_Cmd_3CData,		// Process ACPI command data 3C
	ACPI_Cmd_3DData,		// Process ACPI command data 3D
	ACPI_Cmd_3EData,		// Process ACPI command data 3E
	ACPI_Cmd_3FData			// Process ACPI command data 3F
};

void ACPI_Cmd_3XData(void)
{
	(ACPICmd3XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 4x
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd4XData_table[16] =
{
	ACPI_Cmd_40Data,		// Process ACPI command data 40
	ACPI_Cmd_41Data,		// Process ACPI command data 41
	ACPI_Cmd_42Data,		// Process ACPI command data 42
	ACPI_Cmd_43Data,		// Process ACPI command data 43
	ACPI_Cmd_44Data,		// Process ACPI command data 44
	ACPI_Cmd_45Data,		// Process ACPI command data 45
	ACPI_Cmd_46Data,		// Process ACPI command data 46
	ACPI_Cmd_47Data,		// Process ACPI command data 47
	ACPI_Cmd_48Data,		// Process ACPI command data 48
	ACPI_Cmd_49Data,		// Process ACPI command data 49
	ACPI_Cmd_4AData,		// Process ACPI command data 4A
	ACPI_Cmd_4BData,		// Process ACPI command data 4B
	ACPI_Cmd_4CData,		// Process ACPI command data 4C
	ACPI_Cmd_4DData,		// Process ACPI command data 4D
	ACPI_Cmd_4EData,		// Process ACPI command data 4E
	ACPI_Cmd_4FData			// Process ACPI command data 4F
};


void ACPI_Cmd_4XData(void)
{
	(ACPICmd4XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 5x
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd5XData_table[16] =
{
	ACPI_Cmd_50Data,		// Process ACPI command data 50
	ACPI_Cmd_51Data,		// Process ACPI command data 51
	ACPI_Cmd_52Data,		// Process ACPI command data 52
	ACPI_Cmd_53Data,		// Process ACPI command data 53
	ACPI_Cmd_54Data,		// Process ACPI command data 54
	ACPI_Cmd_55Data,		// Process ACPI command data 55
	ACPI_Cmd_56Data,		// Process ACPI command data 56
	ACPI_Cmd_57Data,		// Process ACPI command data 57
	ACPI_Cmd_58Data,		// Process ACPI command data 58
	ACPI_Cmd_59Data,		// Process ACPI command data 59
	ACPI_Cmd_5AData,		// Process ACPI command data 5A
	ACPI_Cmd_5BData,		// Process ACPI command data 5B
	ACPI_Cmd_5CData,		// Process ACPI command data 5C
	ACPI_Cmd_5DData,		// Process ACPI command data 5D
	ACPI_Cmd_5EData,		// Process ACPI command data 5E
	ACPI_Cmd_5FData			// Process ACPI command data 5F
};

void ACPI_Cmd_5XData(void)
{
	(ACPICmd5XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 6x
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd6XData_table[16] =
{
	ACPI_Cmd_60Data,		// Process ACPI command data 60
	ACPI_Cmd_61Data,		// Process ACPI command data 61
	ACPI_Cmd_62Data,		// Process ACPI command data 62
	ACPI_Cmd_63Data,		// Process ACPI command data 63
	ACPI_Cmd_64Data,		// Process ACPI command data 64
	ACPI_Cmd_65Data,		// Process ACPI command data 65
	ACPI_Cmd_66Data,		// Process ACPI command data 66
	ACPI_Cmd_67Data,		// Process ACPI command data 67
	ACPI_Cmd_68Data,		// Process ACPI command data 68
	ACPI_Cmd_69Data,		// Process ACPI command data 69
	ACPI_Cmd_6AData,		// Process ACPI command data 6A
	ACPI_Cmd_6BData,		// Process ACPI command data 6B
	ACPI_Cmd_6CData,		// Process ACPI command data 6C
	ACPI_Cmd_6DData,		// Process ACPI command data 6D
	ACPI_Cmd_6EData,		// Process ACPI command data 6E
	ACPI_Cmd_6FData			// Process ACPI command data 6F
};

void ACPI_Cmd_6XData(void)
{
	(ACPICmd6XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 7x
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmd7XData_table[16] =
{
	ACPI_Cmd_70Data,		// Process ACPI command data 70
	ACPI_Cmd_71Data,		// Process ACPI command data 71
	ACPI_Cmd_72Data,		// Process ACPI command data 72
	ACPI_Cmd_73Data,		// Process ACPI command data 73
	ACPI_Cmd_74Data,		// Process ACPI command data 74
	ACPI_Cmd_75Data,		// Process ACPI command data 75
	ACPI_Cmd_76Data,		// Process ACPI command data 76
	ACPI_Cmd_77Data,		// Process ACPI command data 77
	ACPI_Cmd_78Data,		// Process ACPI command data 78
	ACPI_Cmd_79Data,		// Process ACPI command data 79
	ACPI_Cmd_7AData,		// Process ACPI command data 7A
	ACPI_Cmd_7BData,		// Process ACPI command data 7B
	ACPI_Cmd_7CData,		// Process ACPI command data 7C
	ACPI_Cmd_7DData,		// Process ACPI command data 7D
	ACPI_Cmd_7EData,		// Process ACPI command data 7E
	ACPI_Cmd_7FData			// Process ACPI command data 7F
};

void ACPI_Cmd_7XData(void)
{
	(ACPICmd7XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 8x
//----------------------------------------------------------------------------
void ACPI_Cmd_80Data(void)

{
	if (PM1Step == 1)
	{
		SendFromAcpi(Hook_ReadMapECSpace(PM1Data));
		ECCheckBurstMode = 0;
	}
}

void ACPI_Cmd_81Data(void)

{
	ACPI_Gen_Int(ACPICmdSCI);
	if (PM1Step == 2)
	{
		PM1Data1 = PM1Data;
	}
	else if (PM1Step == 1)
	{
		Hook_WriteMapECSpace(PM1Data1, PM1Data);
		ECCheckBurstMode = 0;
	}
}

void ACPI_Cmd_82Data(void)

{

}

void ACPI_Cmd_83Data(void)

{

}

void ACPI_Cmd_84Data(void)

{

}

const FUNCT_PTR_V_V code ACPICmd8XData_table[16] =
{
	ACPI_Cmd_80Data,		// Process ACPI command data 80
	ACPI_Cmd_81Data,		// Process ACPI command data 81
	ACPI_Cmd_82Data,		// Process ACPI command data 82
	ACPI_Cmd_83Data,		// Process ACPI command data 83
	ACPI_Cmd_84Data,		// Process ACPI command data 84
	ACPI_Cmd_85Data,		// Process ACPI command data 85
	ACPI_Cmd_86Data,		// Process ACPI command data 86
	ACPI_Cmd_87Data,		// Process ACPI command data 87
	ACPI_Cmd_88Data,		// Process ACPI command data 88
	ACPI_Cmd_89Data,		// Process ACPI command data 89
	ACPI_Cmd_8AData,		// Process ACPI command data 8A
	ACPI_Cmd_8BData,		// Process ACPI command data 8B
	ACPI_Cmd_8CData,		// Process ACPI command data 8C
	ACPI_Cmd_8DData,		// Process ACPI command data 8D
	ACPI_Cmd_8EData,		// Process ACPI command data 8E
	ACPI_Cmd_8FData			// Process ACPI command data 8F
};
void ACPI_Cmd_8XData(void)
{
	(ACPICmd8XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command 9x
//----------------------------------------------------------------------------
void ACPI_Cmd_92Data(void)

{
	switch (PM1Step)
	{
	case 2:
		PM1Data1 = PM1Data;		// address high
		break;

	case 1:
		Read_External_Ram();
		break;

	default:
		break;
	}
}

void ACPI_Cmd_93Data(void)

{
	if (PM1Step == 0x03)
	{
		PM1Data2 = PM1Data;	// address high
	}
	else if (PM1Step == 0x02)
	{
		PM1Data1 = PM1Data;	// address low
	}
	else if (PM1Step == 0x01)
	{
		Write_External_Ram();
	}
}

const FUNCT_PTR_V_V code ACPICmd9XData_table[16] =
{
	ACPI_Cmd_90Data,		// Process ACPI command data 90
	ACPI_Cmd_91Data,		// Process ACPI command data 91
	ACPI_Cmd_92Data,		// Process ACPI command data 92
	ACPI_Cmd_93Data,		// Process ACPI command data 93
	ACPI_Cmd_94Data,		// Process ACPI command data 94
	ACPI_Cmd_95Data,		// Process ACPI command data 95
	ACPI_Cmd_96Data,		// Process ACPI command data 96
	ACPI_Cmd_97Data,		// Process ACPI command data 97
	ACPI_Cmd_99Data,		// Process ACPI command data 99
	ACPI_Cmd_99Data,		// Process ACPI command data 99
	ACPI_Cmd_9AData,		// Process ACPI command data 9A
	ACPI_Cmd_9BData,		// Process ACPI command data 9B
	ACPI_Cmd_9CData,		// Process ACPI command data 9C
	ACPI_Cmd_9DData,		// Process ACPI command data 9D
	ACPI_Cmd_9EData,		// Process ACPI command data 9E
	ACPI_Cmd_9FData			// Process ACPI command data 9F
};
void ACPI_Cmd_9XData(void)
{
	(ACPICmd9XData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command Ax
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmdAXData_table[16] =
{
	ACPI_Cmd_A0Data,		// Process ACPI command data A0
	ACPI_Cmd_A1Data,		// Process ACPI command data A1
	ACPI_Cmd_A2Data,		// Process ACPI command data A2
	ACPI_Cmd_A3Data,		// Process ACPI command data A3
	ACPI_Cmd_A4Data,		// Process ACPI command data A4
	ACPI_Cmd_A5Data,		// Process ACPI command data A5
	ACPI_Cmd_A6Data,		// Process ACPI command data A6
	ACPI_Cmd_A7Data,		// Process ACPI command data A7
	ACPI_Cmd_A8Data,		// Process ACPI command data A8
	ACPI_Cmd_A9Data,		// Process ACPI command data A9
	ACPI_Cmd_AAData,		// Process ACPI command data AA
	ACPI_Cmd_ABData,		// Process ACPI command data AB
	ACPI_Cmd_ACData,		// Process ACPI command data AC
	ACPI_Cmd_ADData,		// Process ACPI command data AD
	ACPI_Cmd_AEData,		// Process ACPI command data AE
	ACPI_Cmd_AFData			// Process ACPI command data AF
};

void ACPI_Cmd_AXData(void)
{
	(ACPICmdAXData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command Bx
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmdBXData_table[16] =
{
	ACPI_Cmd_B0Data,		// Process ACPI command data B0
	ACPI_Cmd_B1Data,		// Process ACPI command data B1
	ACPI_Cmd_B2Data,		// Process ACPI command data B2
	ACPI_Cmd_B3Data,		// Process ACPI command data B3
	ACPI_Cmd_B4Data,		// Process ACPI command data B4
	ACPI_Cmd_B5Data,		// Process ACPI command data B5
	ACPI_Cmd_B6Data,		// Process ACPI command data B6
	ACPI_Cmd_B7Data,		// Process ACPI command data B7
	ACPI_Cmd_B8Data,		// Process ACPI command data B8
	ACPI_Cmd_B9Data,		// Process ACPI command data B9
	ACPI_Cmd_BAData,		// Process ACPI command data BA
	ACPI_Cmd_BBData,		// Process ACPI command data BB
	ACPI_Cmd_BCData,		// Process ACPI command data BC
	ACPI_Cmd_BDData,		// Process ACPI command data BD
	ACPI_Cmd_BEData,		// Process ACPI command data BE
	ACPI_Cmd_BFData			// Process ACPI command data BF
};

void ACPI_Cmd_BXData(void)
{
	(ACPICmdBXData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command Cx
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmdCXData_table[16] =
{
	ACPI_Cmd_C0Data,		// Process ACPI command data C0
	ACPI_Cmd_C1Data,		// Process ACPI command data C1
	ACPI_Cmd_C2Data,		// Process ACPI command data C2
	ACPI_Cmd_C3Data,		// Process ACPI command data C3
	ACPI_Cmd_C4Data,		// Process ACPI command data C4
	ACPI_Cmd_C5Data,		// Process ACPI command data C5
	ACPI_Cmd_C6Data,		// Process ACPI command data C6
	ACPI_Cmd_C7Data,		// Process ACPI command data C7
	ACPI_Cmd_C8Data,		// Process ACPI command data C8
	ACPI_Cmd_C9Data,		// Process ACPI command data C9
	ACPI_Cmd_CAData,		// Process ACPI command data CA
	ACPI_Cmd_CBData,		// Process ACPI command data CB
	ACPI_Cmd_CCData,		// Process ACPI command data CC
	ACPI_Cmd_CDData,		// Process ACPI command data CD
	ACPI_Cmd_CEData,		// Process ACPI command data CE
	ACPI_Cmd_CFData			// Process ACPI command data CF
};

void ACPI_Cmd_CXData(void)
{
	(ACPICmdCXData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command Dx
//----------------------------------------------------------------------------
void ACPI_Cmd_D8Data(void)
{

}

void ACPI_Cmd_DCData(void)
{

}

void ACPI_Cmd_DDData(void)
{

}


const FUNCT_PTR_V_V code ACPICmdDXData_table[16] =
{
	ACPI_Cmd_D0Data,		// Process ACPI command data D0
	ACPI_Cmd_D1Data,		// Process ACPI command data D1
	ACPI_Cmd_D2Data,		// Process ACPI command data D2
	ACPI_Cmd_D3Data,		// Process ACPI command data D3
	ACPI_Cmd_D4Data,		// Process ACPI command data D4
	ACPI_Cmd_D5Data,		// Process ACPI command data D5
	ACPI_Cmd_D6Data,		// Process ACPI command data D6
	ACPI_Cmd_D7Data,		// Process ACPI command data D7
	ACPI_Cmd_D8Data,		// Process ACPI command data D8
	ACPI_Cmd_D9Data,		// Process ACPI command data D9
	ACPI_Cmd_DAData,		// Process ACPI command data DA
	ACPI_Cmd_DBData,		// Process ACPI command data DB
	ACPI_Cmd_DCData,		// Process ACPI command data DC
	ACPI_Cmd_DDData,		// Process ACPI command data DD
	ACPI_Cmd_DEData,		// Process ACPI command data DE
	ACPI_Cmd_DFData			// Process ACPI command data DF
};

void ACPI_Cmd_DXData(void)
{
	(ACPICmdDXData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command Ex
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPICmdEXData_table[16] =
{
	ACPI_Cmd_E0Data,		// Process ACPI command data E0
	ACPI_Cmd_E1Data,		// Process ACPI command data E1
	ACPI_Cmd_E2Data,		// Process ACPI command data E2
	ACPI_Cmd_E3Data,		// Process ACPI command data E3
	ACPI_Cmd_E4Data,		// Process ACPI command data E4
	ACPI_Cmd_E5Data,		// Process ACPI command data E5
	ACPI_Cmd_E6Data,		// Process ACPI command data E6
	ACPI_Cmd_E7Data,		// Process ACPI command data E7
	ACPI_Cmd_E8Data,		// Process ACPI command data E8
	ACPI_Cmd_E9Data,		// Process ACPI command data E9
	ACPI_Cmd_EAData,		// Process ACPI command data EA
	ACPI_Cmd_EBData,		// Process ACPI command data EB
	ACPI_Cmd_ECData,		// Process ACPI command data EC
	ACPI_Cmd_EDData,		// Process ACPI command data ED
	ACPI_Cmd_EEData,		// Process ACPI command data EE
	ACPI_Cmd_EFData			// Process ACPI command data EF
};


void ACPI_Cmd_EXData(void)
{
	(ACPICmdEXData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle the data of ACPI command Fx
//----------------------------------------------------------------------------
void ACPI_Cmd_F0Data(void)

{

}

void ACPI_Cmd_F1Data(void)

{

}

void ACPI_Cmd_F2Data(void)

{

}

void ACPI_Cmd_F3Data(void)

{

}

void ACPI_Cmd_F4Data(void)

{

}

void ACPI_Cmd_F5Data(void)

{

}

void ACPI_Cmd_F6Data(void)

{

}

void ACPI_Cmd_F7Data(void)

{

}

void ACPI_Cmd_F8Data(void)

{

}

void ACPI_Cmd_F9Data(void)

{

}

void ACPI_Cmd_FAData(void)

{

}

void ACPI_Cmd_FBData(void)

{

}

void ACPI_Cmd_FCData(void)

{

}

void ACPI_Cmd_FDData(void)

{

}

void ACPI_Cmd_FEData(void)

{

}

void ACPI_Cmd_FFData(void)

{

}

const FUNCT_PTR_V_V code ACPICmdFXData_table[16] =
{
	ACPI_Cmd_F0Data,		// Process ACPI command data F0
	ACPI_Cmd_F1Data,		// Process ACPI command data F1
	ACPI_Cmd_F2Data,		// Process ACPI command data F2
	ACPI_Cmd_F3Data,		// Process ACPI command data F3
	ACPI_Cmd_F4Data,		// Process ACPI command data F4
	ACPI_Cmd_F5Data,		// Process ACPI command data F5
	ACPI_Cmd_F6Data,		// Process ACPI command data F6
	ACPI_Cmd_F7Data,		// Process ACPI command data F7
	ACPI_Cmd_F8Data,		// Process ACPI command data F8
	ACPI_Cmd_F9Data,		// Process ACPI command data F9
	ACPI_Cmd_FAData,		// Process ACPI command data FA
	ACPI_Cmd_FBData,		// Process ACPI command data FB
	ACPI_Cmd_FCData,		// Process ACPI command data FC
	ACPI_Cmd_FDData,		// Process ACPI command data FD
	ACPI_Cmd_FEData,		// Process ACPI command data FE
	ACPI_Cmd_FFData			// Process ACPI command data FF
};

void ACPI_Cmd_FXData(void)
{
	(ACPICmdFXData_table[PM1Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Read external ram Space
//----------------------------------------------------------------------------
void Read_External_Ram()
{
	Tmp_XPntr = ((PM1Data1 << 8) + PM1Data);
	PM1DO = *Tmp_XPntr;
}

//----------------------------------------------------------------------------
// Write external ram Space
//----------------------------------------------------------------------------
void Write_External_Ram()
{
	Tmp_XPntr = ((PM1Data2 << 8) + PM1Data1);
	*Tmp_XPntr = PM1Data;
}

//----------------------------------------------------------------------------
// Burst mode or normal mode
//----------------------------------------------------------------------------
BYTE CheckBurstMode(void)
{
	while (IS_MASK_CLEAR(PM1STS, P_IBF))	// wait input buffer full
	{
		if (TF1)						// time-out
		{
			TR1 = 0;
			TF1 = 0;
			CLEAR_MASK(PM1STS, BURST);
			ACPI_Gen_Int(ACPICmdSCI);
			ECCheckBurstMode = 0;
			return(0);
		}
	}
	return(1);
}

//----------------------------------------------------------------------------
// Set Burst mode total time (1ms)
//----------------------------------------------------------------------------
void SetTotalBurstTime(void)
{
	DisableAllInterrupt();
	TR1 = 0;						// disable timer1
	ET1 = 0;						// Disable timer1 interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	TH1 = T_ACPI_BurstTime >> 8;	// Set timer1 counter 2ms
	TL1 = T_ACPI_BurstTime;			// Set timer1 counter 2ms
	TF1 = 0;						// clear overflow flag
	TR1 = 1;						// enable timer1
}

//----------------------------------------------------------------------------
// Process Command/Data received from System via 68/6C interface
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code Port6C_table[16] =
{
	PM2_6CCmd_0X,			// Process ACPI 6C command 0x
	PM2_6CCmd_1X,			// Process ACPI 6C command 1x
	PM2_6CCmd_2X,			// Process ACPI 6C command 2x
	PM2_6CCmd_3X,			// Process ACPI 6C command 3x
	PM2_6CCmd_4X,			// Process ACPI 6C command 4x
	PM2_6CCmd_5X,			// Process ACPI 6C command 5x
	PM2_6CCmd_6X,			// Process ACPI 6C command 6x
	PM2_6CCmd_7X,			// Process ACPI 6C command 7x
	PM2_6CCmd_8X,			// Process ACPI 6C command 8x
	PM2_6CCmd_9X,			// Process ACPI 6C command 9x
	PM2_6CCmd_AX,			// Process ACPI 6C command Ax
	PM2_6CCmd_BX,			// Process ACPI 6C command Bx
	PM2_6CCmd_CX,			// Process ACPI 6C command Cx
	PM2_6CCmd_DX,			// Process ACPI 6C command Dx
	PM2_6CCmd_EX,			// Process ACPI 6C command Ex
	PM2_6CCmd_FX			// Process ACPI 6C command Fx
};

const FUNCT_PTR_V_V code Port68_table[16] =
{
	PM2_6CCmd_0XData,		// Process ACPI 68 data 0x
	PM2_6CCmd_1XData,		// Process ACPI 68 data 1x
	PM2_6CCmd_2XData,		// Process ACPI 68 data 2x
	PM2_6CCmd_3XData,		// Process ACPI 68 data 3x
	PM2_6CCmd_4XData,		// Process ACPI 68 data 4x
	PM2_6CCmd_5XData,		// Process ACPI 68 data 5x
	PM2_6CCmd_6XData,		// Process ACPI 68 data 6x
	PM2_6CCmd_7XData,		// Process ACPI 68 data 7x
	PM2_6CCmd_8XData,		// Process ACPI 68 data 8x
	PM2_6CCmd_9XData,		// Process ACPI 68 data 9x
	PM2_6CCmd_AXData,		// Process ACPI 68 data Ax
	PM2_6CCmd_BXData,		// Process ACPI 68 data Bx
	PM2_6CCmd_CXData,		// Process ACPI 68 data Cx
	PM2_6CCmd_DXData,		// Process ACPI 68 data Dx
	PM2_6CCmd_EXData,		// Process ACPI 68 data Ex
	PM2_6CCmd_FXData		// Process ACPI 68 data Fx
};
void service_pci3(void)
{
	if (IS_MASK_CLEAR(PM2STS, P_IBF))
	{
		return;
	}

	if (PM2STS & P_C_D)	// Command port
	{
		PM2DataCount = 0;
		PM2Cmd  = PM2DI;	// Load command
		EnablePMCIBFInt();
		Hook_6CPortDebug();
		(Port6C_table[PM2Cmd >> 4])();	// Handle command
	}
	else					// Data port
	{
		PM2Data = PM2DI;
		EnablePMCIBFInt();
		Hook_68PortDebug();
		if (PM2DataCount != 0x00)
		{
			(Port68_table[PM2Cmd >> 4])();	// Handle command data
			PM2DataCount--;
		}
	}
	#if	1	// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42 [PATCH]
	if (IsFlag1(POWER_FLAG, ACPI_OS)) {Hook_ACPICommand();}
	#endif
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command 0x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd0X_table[16] =
{
	PM2_6CCmd_00,			// Process 6C command 00
	PM2_6CCmd_01,			// Process 6C command 01
	PM2_6CCmd_02,			// Process 6C command 02
	PM2_6CCmd_03,			// Process 6C command 03
	PM2_6CCmd_04,			// Process 6C command 04
	PM2_6CCmd_05,			// Process 6C command 05
	PM2_6CCmd_06,			// Process 6C command 06
	PM2_6CCmd_07,			// Process 6C command 07
	PM2_6CCmd_08,			// Process 6C command 08
	PM2_6CCmd_09,			// Process 6C command 09
	PM2_6CCmd_0A,			// Process 6C command 0A
	PM2_6CCmd_0B,			// Process 6C command 0B
	PM2_6CCmd_0C,			// Process 6C command 0C
	PM2_6CCmd_0D,			// Process 6C command 0D
	PM2_6CCmd_0E,			// Process 6C command 0E
	PM2_6CCmd_0F			// Process 6C command 0F
};

void PM2_6CCmd_0X(void)

{
	(ACPI6CCmd0X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command 1x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd1X_table[16] =
{
	PM2_6CCmd_10,			// Process 6C command 10
	PM2_6CCmd_11,			// Process 6C command 11
	PM2_6CCmd_12,			// Process 6C command 12
	PM2_6CCmd_13,			// Process 6C command 13
	PM2_6CCmd_14,			// Process 6C command 14
	PM2_6CCmd_15,			// Process 6C command 15
	PM2_6CCmd_16,			// Process 6C command 16
	PM2_6CCmd_17,			// Process 6C command 17
	PM2_6CCmd_18,			// Process 6C command 18
	PM2_6CCmd_19,			// Process 6C command 19
	PM2_6CCmd_1A,			// Process 6C command 1A
	PM2_6CCmd_1B,			// Process 6C command 1B
	PM2_6CCmd_1C,			// Process 6C command 1C
	PM2_6CCmd_1D,			// Process 6C command 1D
	PM2_6CCmd_1E,			// Process 6C command 1E
	PM2_6CCmd_1F			// Process 6C command 1F
};

void PM2_6CCmd_1X(void)

{
	(ACPI6CCmd1X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command 2x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd2X_table[16] =
{
	PM2_6CCmd_20,			// Process 6C command 20
	PM2_6CCmd_21,			// Process 6C command 21
	PM2_6CCmd_22,			// Process 6C command 22
	PM2_6CCmd_23,			// Process 6C command 23
	PM2_6CCmd_24,			// Process 6C command 24
	PM2_6CCmd_25,			// Process 6C command 25
	PM2_6CCmd_26,			// Process 6C command 26
	PM2_6CCmd_27,			// Process 6C command 27
	PM2_6CCmd_28,			// Process 6C command 28
	PM2_6CCmd_29,			// Process 6C command 29
	PM2_6CCmd_2A,			// Process 6C command 2A
	PM2_6CCmd_2B,			// Process 6C command 2B
	PM2_6CCmd_2C,			// Process 6C command 2C
	PM2_6CCmd_2D,			// Process 6C command 2D
	PM2_6CCmd_2E,			// Process 6C command 2E
	PM2_6CCmd_2F			// Process 6C command 2F
};

void PM2_6CCmd_2X(void)

{
	(ACPI6CCmd2X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command 3x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd3X_table[16] =
{
	PM2_6CCmd_30,			// Process 6C command 30
	PM2_6CCmd_31,			// Process 6C command 31
	PM2_6CCmd_32,			// Process 6C command 32
	PM2_6CCmd_33,			// Process 6C command 33
	PM2_6CCmd_34,			// Process 6C command 34
	PM2_6CCmd_35,			// Process 6C command 35
	PM2_6CCmd_36,			// Process 6C command 36
	PM2_6CCmd_37,			// Process 6C command 37
	PM2_6CCmd_38,			// Process 6C command 38
	PM2_6CCmd_39,			// Process 6C command 39
	PM2_6CCmd_3A,			// Process 6C command 3A
	PM2_6CCmd_3B,			// Process 6C command 3B
	PM2_6CCmd_3C,			// Process 6C command 3C
	PM2_6CCmd_3D,			// Process 6C command 3D
	PM2_6CCmd_3E,			// Process 6C command 3E
	PM2_6CCmd_3F			// Process 6C command 3F
};

void PM2_6CCmd_3X(void)

{
	(ACPI6CCmd3X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command 4x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd4X_table[16] =
{
	PM2_6CCmd_40,			// Process 6C command 40
	PM2_6CCmd_41,			// Process 6C command 41
	PM2_6CCmd_42,			// Process 6C command 42
	PM2_6CCmd_43,			// Process 6C command 43
	PM2_6CCmd_44,			// Process 6C command 44
	PM2_6CCmd_45,			// Process 6C command 45
	PM2_6CCmd_46,			// Process 6C command 46
	PM2_6CCmd_47,			// Process 6C command 47
	PM2_6CCmd_48,			// Process 6C command 48
	PM2_6CCmd_49,			// Process 6C command 49
	PM2_6CCmd_4A,			// Process 6C command 4A
	PM2_6CCmd_4B,			// Process 6C command 4B
	PM2_6CCmd_4C,			// Process 6C command 4C
	PM2_6CCmd_4D,			// Process 6C command 4D
	PM2_6CCmd_4E,			// Process 6C command 4E
	PM2_6CCmd_4F			// Process 6C command 4F
};

void PM2_6CCmd_4X(void)

{
	(ACPI6CCmd4X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle 6C 6C port command 5x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd5X_table[16] =
{
	PM2_6CCmd_50,			// Process 6C command 50
	PM2_6CCmd_51,			// Process 6C command 51
	PM2_6CCmd_52,			// Process 6C command 52
	PM2_6CCmd_53,			// Process 6C command 53
	PM2_6CCmd_54,			// Process 6C command 54
	PM2_6CCmd_55,			// Process 6C command 55
	PM2_6CCmd_56,			// Process 6C command 56
	PM2_6CCmd_57,			// Process 6C command 57
	PM2_6CCmd_58,			// Process 6C command 58
	PM2_6CCmd_59,			// Process 6C command 59
	PM2_6CCmd_5A,			// Process 6C command 5A
	PM2_6CCmd_5B,			// Process 6C command 5B
	PM2_6CCmd_5C,			// Process 6C command 5C
	PM2_6CCmd_5D,			// Process 6C command 5D
	PM2_6CCmd_5E,			// Process 6C command 5E
	PM2_6CCmd_5F			// Process 6C command 5F
};


void PM2_6CCmd_5X(void)

{
	(ACPI6CCmd5X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle 6C 6C port command 6x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd6X_table[16] =
{
	PM2_6CCmd_60,			// Process 6C command 60
	PM2_6CCmd_61,			// Process 6C command 61
	PM2_6CCmd_62,			// Process 6C command 62
	PM2_6CCmd_63,			// Process 6C command 63
	PM2_6CCmd_64,			// Process 6C command 64
	PM2_6CCmd_65,			// Process 6C command 65
	PM2_6CCmd_66,			// Process 6C command 66
	PM2_6CCmd_67,			// Process 6C command 67
	PM2_6CCmd_68,			// Process 6C command 68
	PM2_6CCmd_69,			// Process 6C command 69
	PM2_6CCmd_6A,			// Process 6C command 6A
	PM2_6CCmd_6B,			// Process 6C command 6B
	PM2_6CCmd_6C,			// Process 6C command 6C
	PM2_6CCmd_6D,			// Process 6C command 6D
	PM2_6CCmd_6E,			// Process 6C command 6E
	PM2_6CCmd_6F			// Process 6C command 6F
};

void PM2_6CCmd_6X(void)

{
	(ACPI6CCmd6X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command 7x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd7X_table[16] =
{
	PM2_6CCmd_70,			// Process 6C command 70
	PM2_6CCmd_71,			// Process 6C command 71
	PM2_6CCmd_72,			// Process 6C command 72
	PM2_6CCmd_73,			// Process 6C command 73
	PM2_6CCmd_74,			// Process 6C command 74
	PM2_6CCmd_75,			// Process 6C command 75
	PM2_6CCmd_76,			// Process 6C command 76
	PM2_6CCmd_77,			// Process 6C command 77
	PM2_6CCmd_78,			// Process 6C command 78
	PM2_6CCmd_79,			// Process 6C command 79
	PM2_6CCmd_7A,			// Process 6C command 7A
	PM2_6CCmd_7B,			// Process 6C command 7B
	PM2_6CCmd_7C,			// Process 6C command 7C
	PM2_6CCmd_7D,			// Process 6C command 7D
	PM2_6CCmd_7E,			// Process 6C command 7E
	PM2_6CCmd_7F			// Process 6C command 7F
};

void PM2_6CCmd_7X(void)

{
	(ACPI6CCmd7X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command 8x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd8X_table[16] =
{
	PM2_6CCmd_80,			// Process 6C command 80
	PM2_6CCmd_81,			// Process 6C command 81
	PM2_6CCmd_82,			// Process 6C command 82
	PM2_6CCmd_83,			// Process 6C command 83
	PM2_6CCmd_84,			// Process 6C command 84
	PM2_6CCmd_85,			// Process 6C command 85
	PM2_6CCmd_86,			// Process 6C command 86
	PM2_6CCmd_87,			// Process 6C command 87
	PM2_6CCmd_88,			// Process 6C command 88
	PM2_6CCmd_89,			// Process 6C command 89
	PM2_6CCmd_8A,			// Process 6C command 8A
	PM2_6CCmd_8B,			// Process 6C command 8B
	PM2_6CCmd_8C,			// Process 6C command 8C
	PM2_6CCmd_8D,			// Process 6C command 8D
	PM2_6CCmd_8E,			// Process 6C command 8E
	PM2_6CCmd_8F			// Process 6C command 8F
};


void PM2_6CCmd_8X(void)

{
	(ACPI6CCmd8X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command 9x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd9X_table[16] =
{
	PM2_6CCmd_90,			// Process 6C command 90
	PM2_6CCmd_91,			// Process 6C command 91
	PM2_6CCmd_92,			// Process 6C command 92
	PM2_6CCmd_93,			// Process 6C command 93
	PM2_6CCmd_94,			// Process 6C command 94
	PM2_6CCmd_95,			// Process 6C command 95
	PM2_6CCmd_96,			// Process 6C command 96
	PM2_6CCmd_97,			// Process 6C command 97
	PM2_6CCmd_98,			// Process 6C command 98
	PM2_6CCmd_99,			// Process 6C command 99
	PM2_6CCmd_9A,			// Process 6C command 9A
	PM2_6CCmd_9B,			// Process 6C command 9B
	PM2_6CCmd_9C,			// Process 6C command 9C
	PM2_6CCmd_9D,			// Process 6C command 9D
	PM2_6CCmd_9E,			// Process 6C command 9E
	PM2_6CCmd_9F			// Process 6C command 9F
};

void PM2_6CCmd_9X(void)

{
	(ACPI6CCmd9X_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command Ax.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdAX_table[16] =
{
	PM2_6CCmd_A0,			// Process 6C command A0
	PM2_6CCmd_A1,			// Process 6C command A1
	PM2_6CCmd_A2,			// Process 6C command A2
	PM2_6CCmd_A3,			// Process 6C command A3
	PM2_6CCmd_A4,			// Process 6C command A4
	PM2_6CCmd_A5,			// Process 6C command A5
	PM2_6CCmd_A6,			// Process 6C command A6
	PM2_6CCmd_A7,			// Process 6C command A7
	PM2_6CCmd_A8,			// Process 6C command A8
	PM2_6CCmd_A9,			// Process 6C command A9
	PM2_6CCmd_AA,			// Process 6C command AA
	PM2_6CCmd_AB,			// Process 6C command AB
	PM2_6CCmd_AC,			// Process 6C command AC
	PM2_6CCmd_AD,			// Process 6C command AD
	PM2_6CCmd_AE,			// Process 6C command AE
	PM2_6CCmd_AF			// Process 6C command AF
};
void PM2_6CCmd_AX(void)

{
	(ACPI6CCmdAX_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command Bx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdBX_table[16] =
{
	PM2_6CCmd_B0,			// Process 6C command B0
	PM2_6CCmd_B1,			// Process 6C command B1
	PM2_6CCmd_B2,			// Process 6C command B2
	PM2_6CCmd_B3,			// Process 6C command B3
	PM2_6CCmd_B4,			// Process 6C command B4
	PM2_6CCmd_B5,			// Process 6C command B5
	PM2_6CCmd_B6,			// Process 6C command B6
	PM2_6CCmd_B7,			// Process 6C command B7
	PM2_6CCmd_B8,			// Process 6C command B8
	PM2_6CCmd_B9,			// Process 6C command B9
	PM2_6CCmd_BA,			// Process 6C command BA
	PM2_6CCmd_BB,			// Process 6C command BB
	PM2_6CCmd_BC,			// Process 6C command BC
	PM2_6CCmd_BD,			// Process 6C command BD
	PM2_6CCmd_BE,			// Process 6C command BE
	PM2_6CCmd_BF			// Process 6C command BF
};

void PM2_6CCmd_BX(void)

{
	(ACPI6CCmdBX_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command Cx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdCX_table[16] =
{
	PM2_6CCmd_C0,			// Process 6C command C0
	PM2_6CCmd_C1,			// Process 6C command C1
	PM2_6CCmd_C2,			// Process 6C command C2
	PM2_6CCmd_C3,			// Process 6C command C3
	PM2_6CCmd_C4,			// Process 6C command C4
	PM2_6CCmd_C5,			// Process 6C command C5
	PM2_6CCmd_C6,			// Process 6C command C6
	PM2_6CCmd_C7,			// Process 6C command C7
	PM2_6CCmd_C8,			// Process 6C command C8
	PM2_6CCmd_C9,			// Process 6C command C9
	PM2_6CCmd_CA,			// Process 6C command CA
	PM2_6CCmd_CB,			// Process 6C command CB
	PM2_6CCmd_CC,			// Process 6C command CC
	PM2_6CCmd_CD,			// Process 6C command CD
	PM2_6CCmd_CE,			// Process 6C command CE
	PM2_6CCmd_CF			// Process 6C command CF
};

void PM2_6CCmd_CX(void)
{
	(ACPI6CCmdCX_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command Dx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdDX_table[16] =
{
	PM2_6CCmd_D0,			// Process 6C command D0
	PM2_6CCmd_D1,			// Process 6C command D1
	PM2_6CCmd_D2,			// Process 6C command D2
	PM2_6CCmd_D3,			// Process 6C command D3
	PM2_6CCmd_D4,			// Process 6C command D4
	PM2_6CCmd_D5,			// Process 6C command D5
	PM2_6CCmd_D6,			// Process 6C command D6
	PM2_6CCmd_D7,			// Process 6C command D7
	PM2_6CCmd_D8,			// Process 6C command D8
	PM2_6CCmd_D9,			// Process 6C command D9
	PM2_6CCmd_DA,			// Process 6C command DA
	PM2_6CCmd_DB,			// Process 6C command DB
	PM2_6CCmd_DC,			// Process 6C command DC
	PM2_6CCmd_DD,			// Process 6C command DD
	PM2_6CCmd_DE,			// Process 6C command DE
	PM2_6CCmd_DF			// Process 6C command DF
};

void PM2_6CCmd_DX(void)
{
	(ACPI6CCmdDX_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command Ex.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdEX_table[16] =
{
	PM2_6CCmd_E0,			// Process 6C command E0
	PM2_6CCmd_E1,			// Process 6C command E1
	PM2_6CCmd_E2,			// Process 6C command E2
	PM2_6CCmd_E3,			// Process 6C command E3
	PM2_6CCmd_E4,			// Process 6C command E4
	PM2_6CCmd_E5,			// Process 6C command E5
	PM2_6CCmd_E6,			// Process 6C command E6
	PM2_6CCmd_E7,			// Process 6C command E7
	PM2_6CCmd_E8,			// Process 6C command E8
	PM2_6CCmd_E9,			// Process 6C command E9
	PM2_6CCmd_EA,			// Process 6C command EA
	PM2_6CCmd_EB,			// Process 6C command EB
	PM2_6CCmd_EC,			// Process 6C command EC
	PM2_6CCmd_ED,			// Process 6C command ED
	PM2_6CCmd_EE,			// Process 6C command EE
	PM2_6CCmd_EF			// Process 6C command EF
};

void PM2_6CCmd_EX(void)

{
	(ACPI6CCmdEX_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 6C port command Fx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdFX_table[16] =
{
	PM2_6CCmd_F0,			// Process 6C command E0
	PM2_6CCmd_F1,			// Process 6C command E1
	PM2_6CCmd_F2,			// Process 6C command E2
	PM2_6CCmd_F3,			// Process 6C command E3
	PM2_6CCmd_F4,			// Process 6C command E4
	PM2_6CCmd_F5,			// Process 6C command E5
	PM2_6CCmd_F6,			// Process 6C command E6
	PM2_6CCmd_F7,			// Process 6C command E7
	PM2_6CCmd_F8,			// Process 6C command E8
	PM2_6CCmd_F9,			// Process 6C command E9
	PM2_6CCmd_FA,			// Process 6C command EA
	PM2_6CCmd_FB,			// Process 6C command EB
	PM2_6CCmd_FC,			// Process 6C command EC
	PM2_6CCmd_FD,			// Process 6C command ED
	PM2_6CCmd_FE,			// Process 6C command EE
	PM2_6CCmd_FF			// Process 6C command EF
};

void PM2_6CCmd_FX(void)

{
	(ACPI6CCmdFX_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 0x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd0XData_table[16] =
{
	PM2_6CCmd_00Data,		// Process 6C command data 00
	PM2_6CCmd_01Data,		// Process 6C command data 01
	PM2_6CCmd_02Data,		// Process 6C command data 02
	PM2_6CCmd_03Data,		// Process 6C command data 03
	PM2_6CCmd_04Data,		// Process 6C command data 04
	PM2_6CCmd_05Data,		// Process 6C command data 05
	PM2_6CCmd_06Data,		// Process 6C command data 06
	PM2_6CCmd_07Data,		// Process 6C command data 07
	PM2_6CCmd_08Data,		// Process 6C command data 08
	PM2_6CCmd_09Data,		// Process 6C command data 09
	PM2_6CCmd_0AData,		// Process 6C command data 0A
	PM2_6CCmd_0BData,		// Process 6C command data 0B
	PM2_6CCmd_0CData,		// Process 6C command data 0C
	PM2_6CCmd_0DData,		// Process 6C command data 0D
	PM2_6CCmd_0EData,		// Process 6C command data 0E
	PM2_6CCmd_0FData		// Process 6C command data 0F
};

void PM2_6CCmd_0XData(void)
{
	(ACPI6CCmd0XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 1x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd1XData_table[16] =
{
	PM2_6CCmd_10Data,		// Process 6C command data 10
	PM2_6CCmd_11Data,		// Process 6C command data 11
	PM2_6CCmd_12Data,		// Process 6C command data 12
	PM2_6CCmd_13Data,		// Process 6C command data 13
	PM2_6CCmd_14Data,		// Process 6C command data 14
	PM2_6CCmd_15Data,		// Process 6C command data 15
	PM2_6CCmd_16Data,		// Process 6C command data 16
	PM2_6CCmd_17Data,		// Process 6C command data 17
	PM2_6CCmd_18Data,		// Process 6C command data 18
	PM2_6CCmd_19Data,		// Process 6C command data 19
	PM2_6CCmd_1AData,		// Process 6C command data 1A
	PM2_6CCmd_1BData,		// Process 6C command data 1B
	PM2_6CCmd_1CData,		// Process 6C command data 1C
	PM2_6CCmd_1DData,		// Process 6C command data 1D
	PM2_6CCmd_1EData,		// Process 6C command data 1E
	PM2_6CCmd_1FData		// Process 6C command data 1F
};

void PM2_6CCmd_1XData(void)
{
	(ACPI6CCmd1XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 2x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd2XData_table[16] =
{
	PM2_6CCmd_20Data,		// Process 6C command data 20
	PM2_6CCmd_21Data,		// Process 6C command data 21
	PM2_6CCmd_22Data,		// Process 6C command data 22
	PM2_6CCmd_23Data,		// Process 6C command data 23
	PM2_6CCmd_24Data,		// Process 6C command data 24
	PM2_6CCmd_25Data,		// Process 6C command data 25
	PM2_6CCmd_26Data,		// Process 6C command data 26
	PM2_6CCmd_27Data,		// Process 6C command data 27
	PM2_6CCmd_28Data,		// Process 6C command data 28
	PM2_6CCmd_29Data,		// Process 6C command data 29
	PM2_6CCmd_2AData,		// Process 6C command data 2A
	PM2_6CCmd_2BData,		// Process 6C command data 2B
	PM2_6CCmd_2CData,		// Process 6C command data 2C
	PM2_6CCmd_2DData,		// Process 6C command data 2D
	PM2_6CCmd_2EData,		// Process 6C command data 2E
	PM2_6CCmd_2FData		// Process 6C command data 2F
};

void PM2_6CCmd_2XData(void)
{
	(ACPI6CCmd2XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 3x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd3XData_table[16] =
{
	PM2_6CCmd_30Data,		// Process 6C command data 30
	PM2_6CCmd_31Data,		// Process 6C command data 31
	PM2_6CCmd_32Data,		// Process 6C command data 32
	PM2_6CCmd_33Data,		// Process 6C command data 33
	PM2_6CCmd_34Data,		// Process 6C command data 34
	PM2_6CCmd_35Data,		// Process 6C command data 35
	PM2_6CCmd_36Data,		// Process 6C command data 36
	PM2_6CCmd_37Data,		// Process 6C command data 37
	PM2_6CCmd_38Data,		// Process 6C command data 38
	PM2_6CCmd_39Data,		// Process 6C command data 39
	PM2_6CCmd_3AData,		// Process 6C command data 3A
	PM2_6CCmd_3BData,		// Process 6C command data 3B
	PM2_6CCmd_3CData,		// Process 6C command data 3C
	PM2_6CCmd_3DData,		// Process 6C command data 3D
	PM2_6CCmd_3EData,		// Process 6C command data 3E
	PM2_6CCmd_3FData		// Process 6C command data 3F
};

void PM2_6CCmd_3XData(void)
{
	(ACPI6CCmd3XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 4x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd4XData_table[16] =
{
	PM2_6CCmd_40Data,		// Process 6C command data 40
	PM2_6CCmd_41Data,		// Process 6C command data 41
	PM2_6CCmd_42Data,		// Process 6C command data 42
	PM2_6CCmd_43Data,		// Process 6C command data 43
	PM2_6CCmd_44Data,		// Process 6C command data 44
	PM2_6CCmd_45Data,		// Process 6C command data 45
	PM2_6CCmd_46Data,		// Process 6C command data 46
	PM2_6CCmd_47Data,		// Process 6C command data 47
	PM2_6CCmd_48Data,		// Process 6C command data 48
	PM2_6CCmd_49Data,		// Process 6C command data 49
	PM2_6CCmd_4AData,		// Process 6C command data 4A
	PM2_6CCmd_4BData,		// Process 6C command data 4B
	PM2_6CCmd_4CData,		// Process 6C command data 4C
	PM2_6CCmd_4DData,		// Process 6C command data 4D
	PM2_6CCmd_4EData,		// Process 6C command data 4E
	PM2_6CCmd_4FData		// Process 6C command data 4F
};

void PM2_6CCmd_4XData(void)
{
	(ACPI6CCmd4XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 5x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd5XData_table[16] =
{
	PM2_6CCmd_50Data,		// Process 6C command data 50
	PM2_6CCmd_51Data,		// Process 6C command data 51
	PM2_6CCmd_52Data,		// Process 6C command data 52
	PM2_6CCmd_53Data,		// Process 6C command data 53
	PM2_6CCmd_54Data,		// Process 6C command data 54
	PM2_6CCmd_55Data,		// Process 6C command data 55
	PM2_6CCmd_56Data,		// Process 6C command data 56
	PM2_6CCmd_57Data,		// Process 6C command data 57
	PM2_6CCmd_58Data,		// Process 6C command data 58
	PM2_6CCmd_59Data,		// Process 6C command data 59
	PM2_6CCmd_5AData,		// Process 6C command data 5A
	PM2_6CCmd_5BData,		// Process 6C command data 5B
	PM2_6CCmd_5CData,		// Process 6C command data 5C
	PM2_6CCmd_5DData,		// Process 6C command data 5D
	PM2_6CCmd_5EData,		// Process 6C command data 5E
	PM2_6CCmd_5FData		// Process 6C command data 5F
};

void PM2_6CCmd_5XData(void)
{
	(ACPI6CCmd5XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 6x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd6XData_table[16] =
{
	PM2_6CCmd_60Data,		// Process 6C command data 60
	PM2_6CCmd_61Data,		// Process 6C command data 61
	PM2_6CCmd_62Data,		// Process 6C command data 62
	PM2_6CCmd_63Data,		// Process 6C command data 63
	PM2_6CCmd_64Data,		// Process 6C command data 64
	PM2_6CCmd_65Data,		// Process 6C command data 65
	PM2_6CCmd_66Data,		// Process 6C command data 66
	PM2_6CCmd_67Data,		// Process 6C command data 67
	PM2_6CCmd_68Data,		// Process 6C command data 68
	PM2_6CCmd_69Data,		// Process 6C command data 69
	PM2_6CCmd_6AData,		// Process 6C command data 6A
	PM2_6CCmd_6BData,		// Process 6C command data 6B
	PM2_6CCmd_6CData,		// Process 6C command data 6C
	PM2_6CCmd_6DData,		// Process 6C command data 6D
	PM2_6CCmd_6EData,		// Process 6C command data 6E
	PM2_6CCmd_6FData		// Process 6C command data 6F
};


void PM2_6CCmd_6XData(void)
{
	(ACPI6CCmd6XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 7x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd7XData_table[16] =
{
	PM2_6CCmd_70Data,		// Process 6C command data 70
	PM2_6CCmd_71Data,		// Process 6C command data 71
	PM2_6CCmd_72Data,		// Process 6C command data 72
	PM2_6CCmd_73Data,		// Process 6C command data 73
	PM2_6CCmd_74Data,		// Process 6C command data 74
	PM2_6CCmd_75Data,		// Process 6C command data 75
	PM2_6CCmd_76Data,		// Process 6C command data 76
	PM2_6CCmd_77Data,		// Process 6C command data 77
	PM2_6CCmd_78Data,		// Process 6C command data 78
	PM2_6CCmd_79Data,		// Process 6C command data 79
	PM2_6CCmd_7AData,		// Process 6C command data 7A
	PM2_6CCmd_7BData,		// Process 6C command data 7B
	PM2_6CCmd_7CData,		// Process 6C command data 7C
	PM2_6CCmd_7DData,		// Process 6C command data 7D
	PM2_6CCmd_7EData,		// Process 6C command data 7E
	PM2_6CCmd_7FData		// Process 6C command data 7F
};

void PM2_6CCmd_7XData(void)
{
	(ACPI6CCmd7XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 8x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd8XData_table[16] =
{
	PM2_6CCmd_80Data,		// Process 6C command data 80
	PM2_6CCmd_81Data,		// Process 6C command data 81
	PM2_6CCmd_82Data,		// Process 6C command data 82
	PM2_6CCmd_83Data,		// Process 6C command data 83
	PM2_6CCmd_84Data,		// Process 6C command data 84
	PM2_6CCmd_85Data,		// Process 6C command data 85
	PM2_6CCmd_86Data,		// Process 6C command data 86
	PM2_6CCmd_87Data,		// Process 6C command data 87
	PM2_6CCmd_88Data,		// Process 6C command data 88
	PM2_6CCmd_89Data,		// Process 6C command data 89
	PM2_6CCmd_8AData,		// Process 6C command data 8A
	PM2_6CCmd_8BData,		// Process 6C command data 8B
	PM2_6CCmd_8CData,		// Process 6C command data 8C
	PM2_6CCmd_8DData,		// Process 6C command data 8D
	PM2_6CCmd_8EData,		// Process 6C command data 8E
	PM2_6CCmd_8FData		// Process 6C command data 8F
};

void PM2_6CCmd_8XData(void)
{
	(ACPI6CCmd8XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port 9x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmd9XData_table[16] =
{
	PM2_6CCmd_90Data,		// Process 6C command data 90
	PM2_6CCmd_91Data,		// Process 6C command data 91
	PM2_6CCmd_92Data,		// Process 6C command data 92
	PM2_6CCmd_93Data,		// Process 6C command data 93
	PM2_6CCmd_94Data,		// Process 6C command data 94
	PM2_6CCmd_95Data,		// Process 6C command data 95
	PM2_6CCmd_96Data,		// Process 6C command data 96
	PM2_6CCmd_97Data,		// Process 6C command data 97
	PM2_6CCmd_98Data,		// Process 6C command data 98
	PM2_6CCmd_99Data,		// Process 6C command data 99
	PM2_6CCmd_9AData,		// Process 6C command data 9A
	PM2_6CCmd_9BData,		// Process 6C command data 9B
	PM2_6CCmd_9CData,		// Process 6C command data 9C
	PM2_6CCmd_9DData,		// Process 6C command data 9D
	PM2_6CCmd_9EData,		// Process 6C command data 9E
	PM2_6CCmd_9FData		// Process 6C command data 9F
};


void PM2_6CCmd_9XData(void)
{
	(ACPI6CCmd9XData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port Ax.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdAXData_table[16] =
{
	PM2_6CCmd_A0Data,		// Process 6C command data A0
	PM2_6CCmd_A1Data,		// Process 6C command data A1
	PM2_6CCmd_A2Data,		// Process 6C command data A2
	PM2_6CCmd_A3Data,		// Process 6C command data A3
	PM2_6CCmd_A4Data,		// Process 6C command data A4
	PM2_6CCmd_A5Data,		// Process 6C command data A5
	PM2_6CCmd_A6Data,		// Process 6C command data A6
	PM2_6CCmd_A7Data,		// Process 6C command data A7
	PM2_6CCmd_A8Data,		// Process 6C command data A8
	PM2_6CCmd_A9Data,		// Process 6C command data A9
	PM2_6CCmd_AAData,		// Process 6C command data AA
	PM2_6CCmd_ABData,		// Process 6C command data AB
	PM2_6CCmd_ACData,		// Process 6C command data AC
	PM2_6CCmd_ADData,		// Process 6C command data AD
	PM2_6CCmd_AEData,		// Process 6C command data AE
	PM2_6CCmd_AFData		// Process 6C command data AF
};

void PM2_6CCmd_AXData(void)
{
	(ACPI6CCmdAXData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port Bx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdBXData_table[16] =
{
	PM2_6CCmd_B0Data,		// Process 6C command data B0
	PM2_6CCmd_B1Data,		// Process 6C command data B1
	PM2_6CCmd_B2Data,		// Process 6C command data B2
	PM2_6CCmd_B3Data,		// Process 6C command data B3
	PM2_6CCmd_B4Data,		// Process 6C command data B4
	PM2_6CCmd_B5Data,		// Process 6C command data B5
	PM2_6CCmd_B6Data,		// Process 6C command data B6
	PM2_6CCmd_B7Data,		// Process 6C command data B7
	PM2_6CCmd_B8Data,		// Process 6C command data B8
	PM2_6CCmd_B9Data,		// Process 6C command data B9
	PM2_6CCmd_BAData,		// Process 6C command data BA
	PM2_6CCmd_BBData,		// Process 6C command data BB
	PM2_6CCmd_BCData,		// Process 6C command data BC
	PM2_6CCmd_BDData,		// Process 6C command data BD
	PM2_6CCmd_BEData,		// Process 6C command data BE
	PM2_6CCmd_BFData		// Process 6C command data BF
};

void PM2_6CCmd_BXData(void)
{
	(ACPI6CCmdBXData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port Cx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdCXData_table[16] =
{
	PM2_6CCmd_C0Data,		// Process 6C command data C0
	PM2_6CCmd_C1Data,		// Process 6C command data C1
	PM2_6CCmd_C2Data,		// Process 6C command data C2
	PM2_6CCmd_C3Data,		// Process 6C command data C3
	PM2_6CCmd_C4Data,		// Process 6C command data C4
	PM2_6CCmd_C5Data,		// Process 6C command data C5
	PM2_6CCmd_C6Data,		// Process 6C command data C6
	PM2_6CCmd_C7Data,		// Process 6C command data C7
	PM2_6CCmd_C8Data,		// Process 6C command data C8
	PM2_6CCmd_C9Data,		// Process 6C command data C9
	PM2_6CCmd_CAData,		// Process 6C command data CA
	PM2_6CCmd_CBData,		// Process 6C command data CB
	PM2_6CCmd_CCData,		// Process 6C command data CC
	PM2_6CCmd_CDData,		// Process 6C command data CD
	PM2_6CCmd_CEData,		// Process 6C command data CE
	PM2_6CCmd_CFData		// Process 6C command data CF
};

void PM2_6CCmd_CXData(void)
{
	(ACPI6CCmdCXData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port Dx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdDXData_table[16] =
{
	PM2_6CCmd_D0Data,		// Process 6C command data D0
	PM2_6CCmd_D1Data,		// Process 6C command data D1
	PM2_6CCmd_D2Data,		// Process 6C command data D2
	PM2_6CCmd_D3Data,		// Process 6C command data D3
	PM2_6CCmd_D4Data,		// Process 6C command data D4
	PM2_6CCmd_D5Data,		// Process 6C command data D5
	PM2_6CCmd_D6Data,		// Process 6C command data D6
	PM2_6CCmd_D7Data,		// Process 6C command data D7
	PM2_6CCmd_D8Data,		// Process 6C command data D8
	PM2_6CCmd_D9Data,		// Process 6C command data D9
	PM2_6CCmd_DAData,		// Process 6C command data DA
	PM2_6CCmd_DBData,		// Process 6C command data DB
	PM2_6CCmd_DCData,		// Process 6C command data DC
	PM2_6CCmd_DDData,		// Process 6C command data DD
	PM2_6CCmd_DEData,		// Process 6C command data DE
	PM2_6CCmd_DFData		// Process 6C command data DF
};

void PM2_6CCmd_DXData(void)
{
	(ACPI6CCmdDXData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port Ex.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdEXData_table[16] =
{
	PM2_6CCmd_E0Data,		// Process 6C command data E0
	PM2_6CCmd_E1Data,		// Process 6C command data E1
	PM2_6CCmd_E2Data,		// Process 6C command data E2
	PM2_6CCmd_E3Data,		// Process 6C command data E3
	PM2_6CCmd_E4Data,		// Process 6C command data E4
	PM2_6CCmd_E5Data,		// Process 6C command data E5
	PM2_6CCmd_E6Data,		// Process 6C command data E6
	PM2_6CCmd_E7Data,		// Process 6C command data E7
	PM2_6CCmd_E8Data,		// Process 6C command data E8
	PM2_6CCmd_E9Data,		// Process 6C command data E9
	PM2_6CCmd_EAData,		// Process 6C command data EA
	PM2_6CCmd_EBData,		// Process 6C command data EB
	PM2_6CCmd_ECData,		// Process 6C command data EC
	PM2_6CCmd_EDData,		// Process 6C command data ED
	PM2_6CCmd_EEData,		// Process 6C command data EE
	PM2_6CCmd_EFData		// Process 6C command data EF
};

void PM2_6CCmd_EXData(void)
{
	(ACPI6CCmdEXData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle ACPI 68 port Fx.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code ACPI6CCmdFXData_table[16] =
{
	PM2_6CCmd_F0Data,		// Process 6C command data F0
	PM2_6CCmd_F1Data,		// Process 6C command data F1
	PM2_6CCmd_F2Data,		// Process 6C command data F2
	PM2_6CCmd_F3Data,		// Process 6C command data F3
	PM2_6CCmd_F4Data,		// Process 6C command data F4
	PM2_6CCmd_F5Data,		// Process 6C command data F5
	PM2_6CCmd_F6Data,		// Process 6C command data F6
	PM2_6CCmd_F7Data,		// Process 6C command data F7
	PM2_6CCmd_F8Data,		// Process 6C command data F8
	PM2_6CCmd_F9Data,		// Process 6C command data F9
	PM2_6CCmd_FAData,		// Process 6C command data FA
	PM2_6CCmd_FBData,		// Process 6C command data FB
	PM2_6CCmd_FCData,		// Process 6C command data FC
	PM2_6CCmd_FDData,		// Process 6C command data FD
	PM2_6CCmd_FEData,		// Process 6C command data FE
	PM2_6CCmd_FFData		// Process 6C command data FF
};

void PM2_6CCmd_FXData(void)
{
	(ACPI6CCmdFXData_table[PM2Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Process Command/Data received from System via PMC3 interface
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code PortPMC3Cmd_table[16] =
{
	PMC3_Cmd_0X,			// Process PMC3 command 0x
	PMC3_Cmd_1X,			// Process PMC3 command 1x
	PMC3_Cmd_2X,			// Process PMC3 command 2x
	PMC3_Cmd_3X,			// Process PMC3 command 3x
	PMC3_Cmd_4X,			// Process PMC3 command 4x
	PMC3_Cmd_5X,			// Process PMC3 command 5x
	PMC3_Cmd_6X,			// Process PMC3 command 6x
	PMC3_Cmd_7X,			// Process PMC3 command 7x
	PMC3_Cmd_8X,			// Process PMC3 command 8x
	PMC3_Cmd_9X,			// Process PMC3 command 9x
	PMC3_Cmd_AX,			// Process PMC3 command Ax
	PMC3_Cmd_BX,			// Process PMC3 command Bx
	PMC3_Cmd_CX,			// Process PMC3 command Cx
	PMC3_Cmd_DX,			// Process PMC3 command Dx
	PMC3_Cmd_EX,			// Process PMC3 command Ex
	PMC3_Cmd_FX				// Process PMC3 command Fx
};

const FUNCT_PTR_V_V code PortPMC3Data_table[16] =
{
	PMC3_Cmd_0XData,		// Process PMC3 data 0x
	PMC3_Cmd_1XData,		// Process PMC3 data 1x
	PMC3_Cmd_2XData,		// Process PMC3 data 2x
	PMC3_Cmd_3XData,		// Process PMC3 data 3x
	PMC3_Cmd_4XData,		// Process PMC3 data 4x
	PMC3_Cmd_5XData,		// Process PMC3 data 5x
	PMC3_Cmd_6XData,		// Process PMC3 data 6x
	PMC3_Cmd_7XData,		// Process PMC3 data 7x
	PMC3_Cmd_8XData,		// Process PMC3 data 8x
	PMC3_Cmd_9XData,		// Process PMC3 data 9x
	PMC3_Cmd_AXData,		// Process PMC3 data Ax
	PMC3_Cmd_BXData,		// Process PMC3 data Bx
	PMC3_Cmd_CXData,		// Process PMC3 data Cx
	PMC3_Cmd_DXData,		// Process PMC3 data Dx
	PMC3_Cmd_EXData,		// Process PMC3 data Ex
	PMC3_Cmd_FXData			// Process PMC3 data Fx
};
void service_pci4(void)
{
	if (IS_MASK_CLEAR(PM3STS, P_IBF))
	{
		return;
	}

	if (PM3STS & P_C_D)	// Command port
	{
		PM3Cmd  = PM3DI;	// Load command
		EnablePMC3IBFInt();
		PMC3SetDataCounter(0x00);
		(PortPMC3Cmd_table[PM3Cmd >> 4])();	// Handle command
	}
	else					// Data port
	{
		if (PM3DataCount == 0x00)
		{
			PM3Data = PM3DI;
			EnablePMC3IBFInt();
		}
		else
		{
			PM3Data = PM3DI;
			EnablePMC3IBFInt();
			if (PM3DataCount != 0x00)
			{
				(PortPMC3Data_table[PM3Cmd >> 4])();	// Handle command data
				PM3DataCount--;
			}
		}
	}
}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 0x.
//----------------------------------------------------------------------------
void PMC3_Cmd_0X(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 1x.
//----------------------------------------------------------------------------
void PMC3_Cmd_1X(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 2x.
//----------------------------------------------------------------------------
void PMC3_Cmd_2X(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 3x.
//----------------------------------------------------------------------------
void PMC3_Cmd_3X(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 4x.
//----------------------------------------------------------------------------
void PMC3_Cmd_4X(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 5x.
//----------------------------------------------------------------------------
void PMC3_Cmd_5X(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 command 6x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code PMC3Cmd6X_table[16] =
{
	PMC3_Cmd_60,			// Process PMC3 command 60
	PMC3_Cmd_61,			// Process PMC3 command 61
	PMC3_Cmd_62,			// Process PMC3 command 62
	PMC3_Cmd_63,			// Process PMC3 command 63
	PMC3_Cmd_64,			// Process PMC3 command 64
	PMC3_Cmd_65,			// Process PMC3 command 65
	PMC3_Cmd_66,			// Process PMC3 command 66
	PMC3_Cmd_67,			// Process PMC3 command 67
	PMC3_Cmd_68,			// Process PMC3 command 68
	PMC3_Cmd_69,			// Process PMC3 command 69
	PMC3_Cmd_6A,			// Process PMC3 command 6A
	PMC3_Cmd_6B,			// Process PMC3 command 6B
	PMC3_Cmd_6C,			// Process PMC3 command 6C
	PMC3_Cmd_6D,			// Process PMC3 command 6D
	PMC3_Cmd_6E,			// Process PMC3 command 6E
	PMC3_Cmd_6F				// Process PMC3 command 6F
};

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 6x.
//----------------------------------------------------------------------------
void PMC3_Cmd_6X(void)
{
	(PMC3Cmd6X_table[PM3Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 7x.
//----------------------------------------------------------------------------
void PMC3_Cmd_7X(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 8x.
//----------------------------------------------------------------------------
void PMC3_Cmd_8X(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port 9x.
//----------------------------------------------------------------------------
void PMC3_Cmd_9X(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port Ax.
//----------------------------------------------------------------------------
void PMC3_Cmd_AX(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port Bx.
//----------------------------------------------------------------------------
void PMC3_Cmd_BX(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port Cx.
//----------------------------------------------------------------------------
void PMC3_Cmd_CX(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port Dx.
//----------------------------------------------------------------------------
void PMC3_Cmd_DX(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port Ex.
//----------------------------------------------------------------------------
void PMC3_Cmd_EX(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 cmd port Fx.
//----------------------------------------------------------------------------
void PMC3_Cmd_FX(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port 0x.
//----------------------------------------------------------------------------
void PMC3_Cmd_0XData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port 1x.
//----------------------------------------------------------------------------
void PMC3_Cmd_1XData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port 2x.
//----------------------------------------------------------------------------
void PMC3_Cmd_2XData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port 3x.
//----------------------------------------------------------------------------
void PMC3_Cmd_3XData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port 4x.
//----------------------------------------------------------------------------
void PMC3_Cmd_4XData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port 5x.
//----------------------------------------------------------------------------
void PMC3_Cmd_5XData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data 6x.
//----------------------------------------------------------------------------
const FUNCT_PTR_V_V code PMC3Data6X_table[16] =
{
	PMC3_Data_60,			// Process PMC3 Data 60
	PMC3_Data_61,			// Process PMC3 Data 61
	PMC3_Data_62,			// Process PMC3 Data 62
	PMC3_Data_63,			// Process PMC3 Data 63
	PMC3_Data_64,			// Process PMC3 Data 64
	PMC3_Data_65,			// Process PMC3 Data 65
	PMC3_Data_66,			// Process PMC3 Data 66
	PMC3_Data_67,			// Process PMC3 Data 67
	PMC3_Data_68,			// Process PMC3 Data 68
	PMC3_Data_69,			// Process PMC3 Data 69
	PMC3_Data_6A,			// Process PMC3 Data 6A
	PMC3_Data_6B,			// Process PMC3 Data 6B
	PMC3_Data_6C,			// Process PMC3 Data 6C
	PMC3_Data_6D,			// Process PMC3 Data 6D
	PMC3_Data_6E,			// Process PMC3 Data 6E
	PMC3_Data_6F			// Process PMC3 Data 6F
};
//----------------------------------------------------------------------------
// Handle PMC3 data port 6x.
//----------------------------------------------------------------------------
void PMC3_Cmd_6XData(void)
{
	(PMC3Data6X_table[PM3Cmd & 0x0F])();
}

//----------------------------------------------------------------------------
// Handle PMC3 data port 7x.
//----------------------------------------------------------------------------
void PMC3_Cmd_7XData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port 8x.
//----------------------------------------------------------------------------
void PMC3_Cmd_8XData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port 9x.
//----------------------------------------------------------------------------
void PMC3_Cmd_9XData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port Ax.
//----------------------------------------------------------------------------
void PMC3_Cmd_AXData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port Bx.
//----------------------------------------------------------------------------
void PMC3_Cmd_BXData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port Cx.
//----------------------------------------------------------------------------
void PMC3_Cmd_CXData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port Dx.
//----------------------------------------------------------------------------
void PMC3_Cmd_DXData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port Ex.
//----------------------------------------------------------------------------
void PMC3_Cmd_EXData(void)
{

}

//----------------------------------------------------------------------------
// Handle PMC3 data port Fx.
//----------------------------------------------------------------------------
void PMC3_Cmd_FXData(void)
{

}

