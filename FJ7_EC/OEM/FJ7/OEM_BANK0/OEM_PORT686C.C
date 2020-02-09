/*------------------------------------------------------------------------------
 * Title : OEM_PORT686C.C
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//------------------------------------------------------------------------------
// Internal Definition
//------------------------------------------------------------------------------
void DoTurnOffPower_6C8C(void);


void Read591register(void)
{
	if ((REGISTER_ADDR & 0x00F0) == 0x00F0)
	{
		REGISTER_ADDR = REGISTER_ADDR & 0xFF0F;
	}
	Tmp_XPntr = (((REGISTER_ADDR & 0x00FF) << 8) | ((REGISTER_ADDR & 0xFF00) >> 8));
	REGISTER_DATA = (((*Tmp_XPntr) << 8) + (*(Tmp_XPntr + 1)));
}


void Write591register(void)
{
	if ((REGISTER_ADDR & 0x00F0) == 0x00F0)
	{
		REGISTER_ADDR = REGISTER_ADDR & 0xFF0F;
	}
	Tmp_XPntr = (((REGISTER_ADDR & 0x00FF) << 8) | ((REGISTER_ADDR & 0xFF00) >> 8));
	(*Tmp_XPntr) = (REGISTER_DATA >> 8);
	(*(Tmp_XPntr + 1)) = REGISTER_DATA;
}


//==============================================================================
// Handle ACPI 6C port command 0x
//==============================================================================
void PM2_6CCmd_00(void)
{
	PM2DO = (Device_STAT_CTRL & 0x07);	// Load ECO Leds flag
}


void PM2_6CCmd_01(void)
{
	PM2DataCount = 1;
}


void PM2_6CCmd_02(void)
{
	#ifdef	OEM_DIMM
	PM2DO = ((POWER_STAT_CTRL & 0x80) >> 7);	// Load dimmer flag
	#endif	//OEM_DIMM
}


void PM2_6CCmd_03(void)
{
	#ifdef	OEM_DIMM
	PM2DataCount = 1;
	#endif	//OEM_DIMM
}


void PM2_6CCmd_04(void)
{
	PM2DataCount = 1;
}


void PM2_6CCmd_05(void)
{
	PM2DataCount = 2;
}


void PM2_6CCmd_06(void)
{
	PM2DataCount = 2;
}


void PM2_6CCmd_07(void)
{
	PM2DataCount = 3;
}


void PM2_6CCmd_08(void)
{

}


void PM2_6CCmd_09(void)
{

}


void PM2_6CCmd_0A(void)
{

}


void PM2_6CCmd_0B(void)
{

}


void PM2_6CCmd_0C(void)
{

}


void PM2_6CCmd_0D(void)
{

}


void PM2_6CCmd_0E(void)
{

}


void PM2_6CCmd_0F(void)
{
	SetFlag(ABAT_MISC2, Shipmode);
}


//==============================================================================
// Handle ACPI 6C port command 1x
//==============================================================================
void PM2_6CCmd_10(void)
{

}


void PM2_6CCmd_11(void)
{

}


void PM2_6CCmd_12(void)
{

}


void PM2_6CCmd_13(void)
{

}


void PM2_6CCmd_14(void)
{

}


void PM2_6CCmd_15(void)
{

}


void PM2_6CCmd_16(void)
{

}


void PM2_6CCmd_17(void)
{

}


void PM2_6CCmd_18(void)
{

}


void PM2_6CCmd_19(void)
{

}


void PM2_6CCmd_1A(void)
{

}


void PM2_6CCmd_1B(void)
{

}


void PM2_6CCmd_1C(void)
{

}


void PM2_6CCmd_1D(void)
{

}


void PM2_6CCmd_1E(void)
{

}


void PM2_6CCmd_1F(void)
{

}


//==============================================================================
// Handle ACPI 6C port command 2x
//==============================================================================
void PM2_6CCmd_20(void)
{

}


void PM2_6CCmd_21(void)
{

}


void PM2_6CCmd_22(void)
{

}


void PM2_6CCmd_23(void)
{

}


void PM2_6CCmd_24(void)
{

}


void PM2_6CCmd_25(void)
{

}


void PM2_6CCmd_26(void)
{

}


void PM2_6CCmd_27(void)
{

}


void PM2_6CCmd_28(void)
{

}


void PM2_6CCmd_29(void)
{

}


void PM2_6CCmd_2A(void)
{

}


void PM2_6CCmd_2B(void)
{

}


void PM2_6CCmd_2C(void)
{

}


void PM2_6CCmd_2D(void)
{

}


void PM2_6CCmd_2E(void)
{

}


void PM2_6CCmd_2F(void)
{

}


//==============================================================================
// Handle ACPI 6C port command 3x
//==============================================================================
void PM2_6CCmd_30(void)
{

}


void PM2_6CCmd_31(void)
{

}


void PM2_6CCmd_32(void)
{

}


void PM2_6CCmd_33(void)
{

}


void PM2_6CCmd_34(void)
{

}


void PM2_6CCmd_35(void)
{
	#if SIT_BIOS_SETUP_MENU	// leox20150612
	PM2DataCount = 1;
	#endif
}


void PM2_6CCmd_36(void)
{

}


void PM2_6CCmd_37(void)
{

}


void PM2_6CCmd_38(void)
{

}


void PM2_6CCmd_39(void)
{

}


void PM2_6CCmd_3A(void)
{

}


void PM2_6CCmd_3B(void)
{

}


void PM2_6CCmd_3C(void)
{

}


void PM2_6CCmd_3D(void)
{

}


void PM2_6CCmd_3E(void)
{

}


void PM2_6CCmd_3F(void)
{

}


//==============================================================================
// Handle ACPI 6C port command 4x
//==============================================================================
void PM2_6CCmd_40(void)
{

}


void PM2_6CCmd_41(void)
{

}


void PM2_6CCmd_42(void)
{

}


void PM2_6CCmd_43(void)
{

}


void PM2_6CCmd_44(void)
{

}


void PM2_6CCmd_45(void)
{

}


void PM2_6CCmd_46(void)
{

}


void PM2_6CCmd_47(void)
{

}


void PM2_6CCmd_48(void)
{

}


void PM2_6CCmd_49(void)
{

}


void PM2_6CCmd_4A(void)
{

}


void PM2_6CCmd_4B(void)
{

}


void PM2_6CCmd_4C(void)
{

}


void PM2_6CCmd_4D(void)
{

}


void PM2_6CCmd_4E(void)
{

}


void PM2_6CCmd_4F(void)
{

}


//==============================================================================
// Handle ACPI 6C port command 5x
//==============================================================================
void PM2_6CCmd_50(void)
{

}


void PM2_6CCmd_51(void)
{

}


void PM2_6CCmd_52(void)
{

}


void PM2_6CCmd_53(void)
{

}


void PM2_6CCmd_54(void)
{

}


void PM2_6CCmd_55(void)
{

}


void PM2_6CCmd_56(void)
{

}


void PM2_6CCmd_57(void)
{

}


void PM2_6CCmd_58(void)
{

}


void PM2_6CCmd_59(void)
{

}


void PM2_6CCmd_5A(void)
{

}


void PM2_6CCmd_5B(void)
{

}


void PM2_6CCmd_5C(void)
{

}


void PM2_6CCmd_5D(void)
{

}


void PM2_6CCmd_5E(void)
{

}


void PM2_6CCmd_5F(void)
{

}


//==============================================================================
// Handle ACPI 6C port command 6x
//==============================================================================
void PM2_6CCmd_60(void)
{

}


void PM2_6CCmd_61(void)
{

}


void PM2_6CCmd_62(void)
{

}


void PM2_6CCmd_63(void)
{

}


void PM2_6CCmd_64(void)
{

}


void PM2_6CCmd_65(void)
{

}


void PM2_6CCmd_66(void)
{

}


void PM2_6CCmd_67(void)
{

}


void PM2_6CCmd_68(void)
{

}


void PM2_6CCmd_69(void)
{

}


void PM2_6CCmd_6A(void)
{

}


void PM2_6CCmd_6B(void)
{

}


void PM2_6CCmd_6C(void)
{

}


void PM2_6CCmd_6D(void)
{

}


void PM2_6CCmd_6E(void)
{

}


void PM2_6CCmd_6F(void)
{

}


//==============================================================================
// Handle ACPI 6C port command 7x
//==============================================================================
void PM2_6CCmd_70(void)
{

}


void PM2_6CCmd_71(void)
{

}


void PM2_6CCmd_72(void)
{

}


void PM2_6CCmd_73(void)
{

}


void PM2_6CCmd_74(void)
{

}


void PM2_6CCmd_75(void)
{

}


void PM2_6CCmd_76(void)
{

}


void PM2_6CCmd_77(void)
{

}


void PM2_6CCmd_78(void)
{

}


void PM2_6CCmd_79(void)
{

}


void PM2_6CCmd_7A(void)
{

}


void PM2_6CCmd_7B(void)
{

}


void PM2_6CCmd_7C(void)
{

}


void PM2_6CCmd_7D(void)
{

}


void PM2_6CCmd_7E(void)
{

}


void PM2_6CCmd_7F(void)
{

}


//==============================================================================
// Handle ACPI 6C port command 8x
//==============================================================================
void PM2_6CCmd_80(void)
{
	PM2DataCount = 1;
}


void PM2_6CCmd_81(void)
{
	PM2DataCount = 2;
}


void PM2_6CCmd_82(void)
{

}


void PM2_6CCmd_83(void)
{

}


void PM2_6CCmd_84(void)
{

}


void PM2_6CCmd_85(void)
{
	PM2DataCount = 1;
}


void PM2_6CCmd_86(void)
{
	Hook_Port66Cmd86();	// leox_20120611
}


void PM2_6CCmd_87(void)
{
	Hook_Port66Cmd87();	// leox_20120611
}


void PM2_6CCmd_88(void)
{
	Read591register();
}


void PM2_6CCmd_89(void)
{
	Write591register();
}


void PM2_6CCmd_8A(void)
{

}


void PM2_6CCmd_8B(void)
{
	#ifdef	OEM_IPOD
	DisableUSBCHARGPORTFun();
	#endif	//OEM_IPOD
	SetFlag(SYS_MISC2, InRestart);
	SetFlag(POWER_FLAG, wait_HWPG);
	ClrFlag(POWER_FLAG, ACPI_OS);
	HWPG_TIMER = T_SWPG;
	ECPWROK_off();
	ClrFlag(ToSBsignals00, ECPWROK);
}


void PM2_6CCmd_8C(void)
{
	if (AC_IN_read() || IsFlag0(MISC_FLAG, F_CriLow))
	{
		SetFlag(SYS_MISC2, InRestart);
		#ifdef	OEM_IPOD
		DisableUSBCHARGPORTFun();
		#endif	//OEM_IPOD
		ShutDnCause = 0xF6;
		S0S3toS4S5();
		DelayXms(100);	// Delay 100ms
		SetFlag(POWER_FLAG, wait_PSW_off);
		PSWOnPower();
	}
	else
	{
		DoTurnOffPower_6C8C();
	}
}


void PM2_6CCmd_8D(void)
{
	ShutDnCause = 0x19;
	ForceOffPower();
}


//------------------------------------------------------------------------------
// LED debug
//------------------------------------------------------------------------------
// leox_20120406 Refer to ACPI_Cmd_8E()
void PM2_6CCmd_8E(void)
{
	PM2DataCount = 1;
}


//------------------------------------------------------------------------------
// Exit LED debug mode
//------------------------------------------------------------------------------
// leox_20120524 Refer to ACPI_Cmd_8F()
void PM2_6CCmd_8F(void)
{
	ExitDebugLED();
}


//==============================================================================
// Handle ACPI 6C port command 9x
//==============================================================================
void PM2_6CCmd_90(void)
{

}


void PM2_6CCmd_91(void)
{

}


void PM2_6CCmd_92(void)
{

}


void PM2_6CCmd_93(void)
{

}


void PM2_6CCmd_94(void)
{

}


void PM2_6CCmd_95(void)
{

}


void PM2_6CCmd_96(void)
{

}


void PM2_6CCmd_97(void)
{

}


void PM2_6CCmd_98(void)
{

}


void PM2_6CCmd_99(void)
{

}


void PM2_6CCmd_9A(void)
{

}


void PM2_6CCmd_9B(void)
{
	#ifdef	OEM_BATTERY
	DoWriteBattFirstUSEDate();
	#endif	//OEM_BATTERY
}


void PM2_6CCmd_9C(void)
{
	#ifdef	OEM_BATTERY
	SeveReadBattFirstUSEDate();
	#endif	//OEM_BATTERY
}


void PM2_6CCmd_9D(void)
{

}


void PM2_6CCmd_9E(void)
{
	PM2DataCount = 1;	// leox20150721 LED function
}


void PM2_6CCmd_9F(void)
{

}


//==============================================================================
// Handle ACPI 6C port command Ax
//==============================================================================
void PM2_6CCmd_A0(void)
{

}


void PM2_6CCmd_A1(void)
{

}


void PM2_6CCmd_A2(void)
{

}


void PM2_6CCmd_A3(void)
{

}


void PM2_6CCmd_A4(void)
{

}


void PM2_6CCmd_A5(void)
{
	SetFlag(KETBOARD_STAT_CTRL, SKIPMATRIX);
}


void PM2_6CCmd_A6(void)
{
	ClrFlag(KETBOARD_STAT_CTRL, SKIPMATRIX);
}


void PM2_6CCmd_A7(void)
{

}


void PM2_6CCmd_A8(void)
{

}


void PM2_6CCmd_A9(void)
{

}


void PM2_6CCmd_AA(void)
{

}


void PM2_6CCmd_AB(void)
{

}


void PM2_6CCmd_AC(void)
{

}


void PM2_6CCmd_AD(void)
{

}


void PM2_6CCmd_AE(void)
{

}


void PM2_6CCmd_AF(void)
{

}


//==============================================================================
// Handle ACPI 6C port command Bx
//==============================================================================
void PM2_6CCmd_B0(void)
{

}


void PM2_6CCmd_B1(void)
{

}


void PM2_6CCmd_B2(void)
{

}


void PM2_6CCmd_B3(void)
{

}


void PM2_6CCmd_B4(void)
{
	RamProgram(0x6C);
}


void PM2_6CCmd_B5(void)
{

}


void PM2_6CCmd_B6(void)
{

}


void PM2_6CCmd_B7(void)
{

}


void PM2_6CCmd_B8(void)
{

}


void PM2_6CCmd_B9(void)
{

}


void PM2_6CCmd_BA(void)
{

}


void PM2_6CCmd_BB(void)
{

}


void PM2_6CCmd_BC(void)
{

}


void PM2_6CCmd_BD(void)
{

}


void PM2_6CCmd_BE(void)
{

}


void PM2_6CCmd_BF(void)
{

}


//==============================================================================
// Handle ACPI 6C port command Cx
//==============================================================================
void PM2_6CCmd_C0(void)
{

}


void PM2_6CCmd_C1(void)
{

}


//------------------------------------------------------------------------------
// Read EC ROM
//------------------------------------------------------------------------------
void PM2_6CCmd_C2(void)
{
	PM2DataCount = 3;	// leox_20130717 For read EC ROM
}


void PM2_6CCmd_C3(void)
{

}


void PM2_6CCmd_C4(void)
{

}


void PM2_6CCmd_C5(void)
{
	//David add for battery shutdown mode 2014/07/02
	PM2DataCount = 0x01;
	//David add for battery shutdown mode 2014/07/02
}


void PM2_6CCmd_C6(void)
{
	SetFlag(Shipmode_status, Tool_Notify);
	#if SUPPORT_EFLASH_FUNC	// leox20160322
	eFlashSave(&Shipmode_status, Shipmode_status);
	#endif
}


void PM2_6CCmd_C7(void)
{
	Shipmode_status = 0;
	#if SUPPORT_EFLASH_FUNC	// leox20160322
	eFlashSave(&Shipmode_status, Shipmode_status);
	#endif
}


void PM2_6CCmd_C8(void)
{
	WaitS5ToDoWDT = 0x5A;
}


void PM2_6CCmd_C9(void)
{
	//David add for charge limit function 2014/07/02
	RSOC_FULL_BASE = 70;
	//David add for charge limit function 2014/07/02
}


void PM2_6CCmd_CA(void)
{
	//David add for charge limit function 2014/07/02
	RSOC_FULL_BASE = 90;
	//David add for charge limit function 2014/07/02
}


void PM2_6CCmd_CB(void)
{
	PM2DataCount = 0x01;
}


void PM2_6CCmd_CC(void)
{

}


void PM2_6CCmd_CD(void)
{

}


void PM2_6CCmd_CE(void)
{

}


void PM2_6CCmd_CF(void)
{
	#if	RESET_EC_LATER		// leox_20111206
	RstLaterMark0 = 4;
	RstLaterMark1 = 3;
	#endif
}


//==============================================================================
// Handle ACPI 6C port command Dx
//==============================================================================
void PM2_6CCmd_D0(void)
{

}


void PM2_6CCmd_D1(void)
{

}


void PM2_6CCmd_D2(void)
{

}


void PM2_6CCmd_D3(void)
{

}


void PM2_6CCmd_D4(void)
{

}


void PM2_6CCmd_D5(void)
{

}


void PM2_6CCmd_D6(void)
{

}


void PM2_6CCmd_D7(void)
{

}


void PM2_6CCmd_D8(void)
{

}


void PM2_6CCmd_D9(void)
{
	PM2DataCount = 1;
}


void PM2_6CCmd_DA(void)
{

}


void PM2_6CCmd_DB(void)
{

}


void PM2_6CCmd_DC(void)
{

}


void PM2_6CCmd_DD(void)
{

}


void PM2_6CCmd_DE(void)
{

}


void PM2_6CCmd_DF(void)
{

}


//==============================================================================
// Handle ACPI 6C port command Ex
//==============================================================================
void PM2_6CCmd_E0(void)
{

}


void PM2_6CCmd_E1(void)
{

}


void PM2_6CCmd_E2(void)
{

}


void PM2_6CCmd_E3(void)
{

}


void PM2_6CCmd_E4(void)
{

}


void PM2_6CCmd_E5(void)
{

}


void PM2_6CCmd_E6(void)
{

}


void PM2_6CCmd_E7(void)
{

}


void PM2_6CCmd_E8(void)
{

}


void PM2_6CCmd_E9(void)
{

}


void PM2_6CCmd_EA(void)
{

}


void PM2_6CCmd_EB(void)
{

}


void PM2_6CCmd_EC(void)
{

}


void PM2_6CCmd_ED(void)
{

}


void PM2_6CCmd_EE(void)
{

}


void PM2_6CCmd_EF(void)
{

}


//==============================================================================
// Handle ACPI 6C port command Fx
//==============================================================================
void PM2_6CCmd_F0(void)
{

}


void PM2_6CCmd_F1(void)
{

}


void PM2_6CCmd_F2(void)
{

}


void PM2_6CCmd_F3(void)
{

}


void PM2_6CCmd_F4(void)
{

}


void PM2_6CCmd_F5(void)
{

}


void PM2_6CCmd_F6(void)
{

}


void PM2_6CCmd_F7(void)
{

}


void PM2_6CCmd_F8(void)
{

}


void PM2_6CCmd_F9(void)
{

}


void PM2_6CCmd_FA(void)
{

}


void PM2_6CCmd_FB(void)
{

}


void PM2_6CCmd_FC(void)
{

}


void PM2_6CCmd_FD(void)
{

}


void PM2_6CCmd_FE(void)
{

}


void PM2_6CCmd_FF(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 0x
//==============================================================================
void PM2_6CCmd_00Data(void)
{

}


void PM2_6CCmd_01Data(void)
{
	if (PM2DataCount == 1)
	{
		Device_STAT_CTRL = ((Device_STAT_CTRL & 0xF8) | (PM2Data & 0x07));
	}
}


void PM2_6CCmd_02Data(void)
{

}


void PM2_6CCmd_03Data(void)
{
	#ifdef	OEM_DIMM
	if (PM2DataCount == 1)
	{
		ClrFlag(POWER_STAT_CTRL, DIMM_OFF_Flag);	// Initial DIMM_ON_Flag
		POWER_STAT_CTRL = (POWER_STAT_CTRL | (PM2Data << 7));
		VerifyDimmerFunction();
	}
	#endif	//OEM_DIMM
}


void PM2_6CCmd_04Data(void)
{
	if (PM2DataCount == 1)
	{
		Tmp_XPntr = (0x0400 | PM2Data);
		PM2DO = (*Tmp_XPntr);
	}
}


void PM2_6CCmd_05Data(void)
{
	if (PM2DataCount == 2)
	{
		PM2Data1 = PM2Data;	// Save first byte in PM2DataFirst
	}
	if (PM2DataCount == 1)
	{
		Tmp_XPntr = (0x0400 | PM2Data1);
		(*Tmp_XPntr) = PM2Data;
	}
}


void PM2_6CCmd_06Data(void)
{
	if (PM2DataCount == 2)
	{
		PM2Data1 = PM2Data;	// Save first byte in PM2DataFirst
	}
	if (PM2DataCount == 1)
	{
		//Tmp_XPntr = (((PM2Data1 << 8) | (PM2Data)) & 0x0FFF);
		Tmp_XPntr = (PM2Data1 << 8) | PM2Data;	// leox_20130808
		PM2DO = (*Tmp_XPntr);
	}
}


void PM2_6CCmd_07Data(void)
{
	if (PM2DataCount == 3)
	{
		PM2Data1 = PM2Data;	// Save first byte in PM2DataFirst
	}
	if (PM2DataCount == 2)
	{
		PM2Data2 = PM2Data;	// Save second byte in PM2DataSecond
	}
	if (PM2DataCount == 1)
	{
		//Tmp_XPntr = (((PM2Data1 << 8) | (PM2Data2)) & 0x0FFF);
		Tmp_XPntr = (PM2Data1 << 8) | PM2Data2;	// leox_20130808
		(*Tmp_XPntr) = PM2Data;
	}
}


void PM2_6CCmd_08Data(void)
{

}


void PM2_6CCmd_09Data(void)
{

}


void PM2_6CCmd_0AData(void)
{

}


void PM2_6CCmd_0BData(void)
{

}


void PM2_6CCmd_0CData(void)
{

}


void PM2_6CCmd_0DData(void)
{

}


void PM2_6CCmd_0EData(void)
{

}


void PM2_6CCmd_0FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 1x
//==============================================================================
void PM2_6CCmd_10Data(void)
{

}


void PM2_6CCmd_11Data(void)
{

}


void PM2_6CCmd_12Data(void)
{

}


void PM2_6CCmd_13Data(void)
{

}


void PM2_6CCmd_14Data(void)
{

}


void PM2_6CCmd_15Data(void)
{

}


void PM2_6CCmd_16Data(void)
{

}


void PM2_6CCmd_17Data(void)
{

}


void PM2_6CCmd_18Data(void)
{

}


void PM2_6CCmd_19Data(void)
{

}


void PM2_6CCmd_1AData(void)
{

}


void PM2_6CCmd_1BData(void)
{

}


void PM2_6CCmd_1CData(void)
{

}


void PM2_6CCmd_1DData(void)
{

}


void PM2_6CCmd_1EData(void)
{

}


void PM2_6CCmd_1FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 2x
//==============================================================================
void PM2_6CCmd_20Data(void)
{

}


void PM2_6CCmd_21Data(void)
{

}


void PM2_6CCmd_22Data(void)
{

}


void PM2_6CCmd_23Data(void)
{

}


void PM2_6CCmd_24Data(void)
{

}


void PM2_6CCmd_25Data(void)
{

}


void PM2_6CCmd_26Data(void)
{

}


void PM2_6CCmd_27Data(void)
{

}


void PM2_6CCmd_28Data(void)
{

}


void PM2_6CCmd_29Data(void)
{

}


void PM2_6CCmd_2AData(void)
{

}


void PM2_6CCmd_2BData(void)
{

}


void PM2_6CCmd_2CData(void)
{

}


void PM2_6CCmd_2DData(void)
{

}


void PM2_6CCmd_2EData(void)
{

}


void PM2_6CCmd_2FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 3x
//==============================================================================
void PM2_6CCmd_30Data(void)
{

}


void PM2_6CCmd_31Data(void)
{

}


void PM2_6CCmd_32Data(void)
{

}


void PM2_6CCmd_33Data(void)
{

}


void PM2_6CCmd_34Data(void)
{

}


void PM2_6CCmd_35Data(void)
{
	#if SIT_BIOS_SETUP_MENU	// leox20150612
	if (PM2DataCount == 1)
	{
		if (PM2Data == 0x01)
		{	// Set by application
			SetFlag(SYS_MISC2, LaunchSetupMenu);
			PM2DO = 0x10;	// Response ACK to application
		}
		else if (PM2Data == 0x02)
		{	// Query by BIOS
			PM2DO = IsFlag1(SYS_MISC2, LaunchSetupMenu) ? 0x01 : 0x00;
			ClrFlag(SYS_MISC2, LaunchSetupMenu);	// Clear status after read
		}
	}
	#endif
}


void PM2_6CCmd_36Data(void)
{

}


void PM2_6CCmd_37Data(void)
{

}


void PM2_6CCmd_38Data(void)
{

}


void PM2_6CCmd_39Data(void)
{

}


void PM2_6CCmd_3AData(void)
{

}


void PM2_6CCmd_3BData(void)
{

}


void PM2_6CCmd_3CData(void)
{

}


void PM2_6CCmd_3DData(void)
{

}


void PM2_6CCmd_3EData(void)
{

}


void PM2_6CCmd_3FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 4x
//==============================================================================
void PM2_6CCmd_40Data(void)
{

}


void PM2_6CCmd_41Data(void)
{

}


void PM2_6CCmd_42Data(void)
{

}


void PM2_6CCmd_43Data(void)
{

}


void PM2_6CCmd_44Data(void)
{

}


void PM2_6CCmd_45Data(void)
{

}


void PM2_6CCmd_46Data(void)
{

}


void PM2_6CCmd_47Data(void)
{

}


void PM2_6CCmd_48Data(void)
{

}


void PM2_6CCmd_49Data(void)
{

}


void PM2_6CCmd_4AData(void)
{

}


void PM2_6CCmd_4BData(void)
{

}


void PM2_6CCmd_4CData(void)
{

}


void PM2_6CCmd_4DData(void)
{

}


void PM2_6CCmd_4EData(void)
{

}


void PM2_6CCmd_4FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 5x
//==============================================================================
void PM2_6CCmd_50Data(void)
{

}


void PM2_6CCmd_51Data(void)
{

}


void PM2_6CCmd_52Data(void)
{

}


void PM2_6CCmd_53Data(void)
{

}


void PM2_6CCmd_54Data(void)
{

}


void PM2_6CCmd_55Data(void)
{

}


void PM2_6CCmd_56Data(void)
{

}


void PM2_6CCmd_57Data(void)
{

}


void PM2_6CCmd_58Data(void)
{

}


void PM2_6CCmd_59Data(void)
{

}


void PM2_6CCmd_5AData(void)
{

}


void PM2_6CCmd_5BData(void)
{

}


void PM2_6CCmd_5CData(void)
{

}


void PM2_6CCmd_5DData(void)
{

}


void PM2_6CCmd_5EData(void)
{

}


void PM2_6CCmd_5FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 6x
//==============================================================================
void PM2_6CCmd_60Data(void)
{

}


void PM2_6CCmd_61Data(void)
{

}


void PM2_6CCmd_62Data(void)
{

}


void PM2_6CCmd_63Data(void)
{

}


void PM2_6CCmd_64Data(void)
{

}


void PM2_6CCmd_65Data(void)
{

}


void PM2_6CCmd_66Data(void)
{

}


void PM2_6CCmd_67Data(void)
{

}


void PM2_6CCmd_68Data(void)
{

}


void PM2_6CCmd_69Data(void)
{

}


void PM2_6CCmd_6AData(void)
{

}


void PM2_6CCmd_6BData(void)
{

}


void PM2_6CCmd_6CData(void)
{

}


void PM2_6CCmd_6DData(void)
{

}


void PM2_6CCmd_6EData(void)
{

}


void PM2_6CCmd_6FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 7x
//==============================================================================
void PM2_6CCmd_70Data(void)
{

}


void PM2_6CCmd_71Data(void)
{

}


void PM2_6CCmd_72Data(void)
{

}


void PM2_6CCmd_73Data(void)
{

}


void PM2_6CCmd_74Data(void)
{

}


void PM2_6CCmd_75Data(void)
{

}


void PM2_6CCmd_76Data(void)
{

}


void PM2_6CCmd_77Data(void)
{

}


void PM2_6CCmd_78Data(void)
{

}


void PM2_6CCmd_79Data(void)
{

}


void PM2_6CCmd_7AData(void)
{

}


void PM2_6CCmd_7BData(void)
{

}


void PM2_6CCmd_7CData(void)
{

}


void PM2_6CCmd_7DData(void)
{

}


void PM2_6CCmd_7EData(void)
{

}


void PM2_6CCmd_7FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 8x
//==============================================================================
void PM2_6CCmd_80Data(void)
{
	if (PM2DataCount == 1)
	{
		Tmp_XPntr = (0x0300 | PM2Data);
		PM2DO = (*Tmp_XPntr);
	}
}


void PM2_6CCmd_81Data(void)
{
	if (PM2DataCount == 2)
	{
		PM2Data1 = PM2Data;	// Save first byte in PM2DataFirst
	}
	if (PM2DataCount == 1)
	{
		Tmp_XPntr = (0x0300 | PM2Data1);
		(*Tmp_XPntr) = PM2Data;
	}
}


void PM2_6CCmd_82Data(void)
{

}


void PM2_6CCmd_83Data(void)
{

}


void PM2_6CCmd_84Data(void)
{

}


void PM2_6CCmd_85Data(void)
{
	if (PM2DataCount == 1)	// Get PM2Data
	{
		if (PM2Data == 0xFF)
		{
			SetFlag(POWER_FLAG2, DisQevent);		// Disable Q event
		}
		else
		{
			if (PM2Data == 0xFE)
			{
				ClrFlag(POWER_FLAG2, DisQevent);	// Enable Q event
			}
			else
			{
				Q09CNT = PM2Data;
			}
		}
	}
}


void PM2_6CCmd_86Data(void)
{

}


void PM2_6CCmd_87Data(void)
{

}


void PM2_6CCmd_88Data(void)
{

}


void PM2_6CCmd_89Data(void)
{

}


void PM2_6CCmd_8AData(void)
{

}


void PM2_6CCmd_8BData(void)
{

}


void PM2_6CCmd_8CData(void)
{

}


void PM2_6CCmd_8DData(void)
{

}


//------------------------------------------------------------------------------
// LED debug
//------------------------------------------------------------------------------
// leox_20120406 Refer to ACPI_Cmd_8EData()
void PM2_6CCmd_8EData(void)
{
	if (PM2DataCount == 1)
	{
		DebugLED(PM2Data);
	}
}


void PM2_6CCmd_8FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data 9x
//==============================================================================
void PM2_6CCmd_90Data(void)
{

}


void PM2_6CCmd_91Data(void)
{

}


void PM2_6CCmd_92Data(void)
{

}


void PM2_6CCmd_93Data(void)
{

}


void PM2_6CCmd_94Data(void)
{

}


void PM2_6CCmd_95Data(void)
{

}


void PM2_6CCmd_96Data(void)
{

}


void PM2_6CCmd_97Data(void)
{

}


void PM2_6CCmd_98Data(void)
{

}


void PM2_6CCmd_99Data(void)
{

}


void PM2_6CCmd_9AData(void)
{

}


void PM2_6CCmd_9BData(void)
{

}


void PM2_6CCmd_9CData(void)
{

}


void PM2_6CCmd_9DData(void)
{

}


void PM2_6CCmd_9EData(void)
{
	if (PM2DataCount == 1)
	{
		DebugLED2(PM2Data);	// leox20150721 LED function
	}
}


void PM2_6CCmd_9FData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data Ax
//==============================================================================
void PM2_6CCmd_A0Data(void)
{

}


void PM2_6CCmd_A1Data(void)
{

}


void PM2_6CCmd_A2Data(void)
{

}


void PM2_6CCmd_A3Data(void)
{

}


void PM2_6CCmd_A4Data(void)
{

}


void PM2_6CCmd_A5Data(void)
{

}


void PM2_6CCmd_A6Data(void)
{

}


void PM2_6CCmd_A7Data(void)
{

}


void PM2_6CCmd_A8Data(void)
{

}


void PM2_6CCmd_A9Data(void)
{

}


void PM2_6CCmd_AAData(void)
{

}


void PM2_6CCmd_ABData(void)
{

}


void PM2_6CCmd_ACData(void)
{

}


void PM2_6CCmd_ADData(void)
{

}


void PM2_6CCmd_AEData(void)
{

}


void PM2_6CCmd_AFData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data Bx
//==============================================================================
void PM2_6CCmd_B0Data(void)
{

}


void PM2_6CCmd_B1Data(void)
{

}


void PM2_6CCmd_B2Data(void)
{

}


void PM2_6CCmd_B3Data(void)
{

}


void PM2_6CCmd_B4Data(void)
{

}


void PM2_6CCmd_B5Data(void)
{

}


void PM2_6CCmd_B6Data(void)
{

}


void PM2_6CCmd_B7Data(void)
{

}


void PM2_6CCmd_B8Data(void)
{

}


void PM2_6CCmd_B9Data(void)
{

}


void PM2_6CCmd_BAData(void)
{

}


void PM2_6CCmd_BBData(void)
{

}


void PM2_6CCmd_BCData(void)
{

}


void PM2_6CCmd_BDData(void)
{

}


void PM2_6CCmd_BEData(void)
{

}


void PM2_6CCmd_BFData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data Cx
//==============================================================================
void PM2_6CCmd_C0Data(void)
{

}


void PM2_6CCmd_C1Data(void)
{

}


//------------------------------------------------------------------------------
// Read EC ROM
//------------------------------------------------------------------------------
void PM2_6CCmd_C2Data(void)
{
	if (PM2DataCount == 3)		// leox_20130717 For read EC ROM
	{
		PM2Data2 = PM2Data;
	}
	else if (PM2DataCount == 2)
	{
		PM2Data1 = PM2Data;
	}
	else if (PM2DataCount == 1)
	{
		EA = 0;
		ECINDAR3 = 0x00;
		ECINDAR2 = PM2Data2;	// Block (1 block = 64 KB)
		ECINDAR1 = PM2Data1;	// Page  (1 page = 256 bytes)
		ECINDAR0 = PM2Data;		// Offset
		PM2DO = ECINDDR;
		EA = 1;
	}
}


void PM2_6CCmd_C3Data(void)
{

}


void PM2_6CCmd_C4Data(void)
{

}


void PM2_6CCmd_C5Data(void)
{
	if (PM2DataCount == 0x02)
	//David add for battery shutdown mode 2014/07/02
	{/*
		if (PM2Data == 0xFF)
		{
			SetFlag(Shipmode_status, Manufacture);
		}
		else
		{
			ClrFlag(Shipmode_status, Manufacture);
		}
	*/}
	//David add for battery shutdown mode 2014/07/02
	else if (PM2DataCount == 0x01)
	{
		if (PM2Data == 0x01)
		{
			ClrFlag(Shipmode_status, Mode_type);
		}
		else
		{
			SetFlag(Shipmode_status, Mode_type);
		}
		#if SUPPORT_EFLASH_FUNC	// leox20160322
		eFlashSave(&Shipmode_status, Shipmode_status);
		#endif
	}
}


void PM2_6CCmd_C6Data(void)
{
	SetFlag(Hybrid_Status, Hybrid_En);
}


void PM2_6CCmd_C7Data(void)
{
	ClrFlag(Hybrid_Status, Hybrid_En);
}


void PM2_6CCmd_C8Data(void)
{

}


void PM2_6CCmd_C9Data(void)
{

}


void PM2_6CCmd_CAData(void)
{

}


void PM2_6CCmd_CBData(void)
{
	if (PM2DataCount == 0x01)
	{
		if (PM2Data == 0xEE)
		{
			USB4_CHARGE_off();
		}
		else if (PM2Data == 0xFF)
		{
			USB4_CHARGE_on();
		}
	}
}


void PM2_6CCmd_CCData(void)
{

}


void PM2_6CCmd_CDData(void)
{

}


void PM2_6CCmd_CEData(void)
{

}


void PM2_6CCmd_CFData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data Dx
//==============================================================================
void PM2_6CCmd_D0Data(void)
{

}


void PM2_6CCmd_D1Data(void)
{

}


void PM2_6CCmd_D2Data(void)
{

}


void PM2_6CCmd_D3Data(void)
{

}


void PM2_6CCmd_D4Data(void)
{

}


void PM2_6CCmd_D5Data(void)
{

}


void PM2_6CCmd_D6Data(void)
{

}


void PM2_6CCmd_D7Data(void)
{

}


void PM2_6CCmd_D8Data(void)
{

}

void Data_To_Host6C(BYTE data_byte)
{
	PM2DO = data_byte;
}

void Data_To_Host_nWait6C(BYTE data_byte)
{
	Data_To_Host6C(data_byte);

	TR1 = 0;				// Disable timer1
	ET1 = 0;				// Disable timer1 interrupt
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	TH1 = Timer_26ms >> 8;	// Set timer1 counter 26ms
	TL1 = Timer_26ms;		// Set timer1 counter 26ms
	TF1 = 0;				// Clear overflow flag
	TR1 = 1;				// Enable timer1

	while (!TF1)
	{
		if (IS_MASK_CLR(PM2STS, OBF))
		{
			break;
		}
		if (IS_MASK_SET(PM2STS, IBF))
		{
			break;
		}
	}

	TR1 = 0;				// Disable timer1
	TF1 = 0;				// Clear overflow flag
	ET1 = 1;				// Enable timer1 interrupt
}

void SendVerFunction6C(BYTE code *cpntr)
{
	BYTE index;
	index = 0x00;

	while (1)
	{
		if (*cpntr == '#')
		{
			Data_To_Host6C(*cpntr);
			break;
		}
		else
		{
			Data_To_Host_nWait6C(*cpntr);
		}
		cpntr++;

		index++;
		if (index > 32)
		{
			break;
		}
	}
}

void PM2_6CCmd_D9Data(void)
{
	BYTE index;
	index = 0x00;

	if (PM2DataCount == 1)
	{
		switch (PM2DI)
		{
		case 1:			// 1: revision desciption
			SendVerFunction6C(P_rev);
			break;

		case 2:			// 2: platform information
			SendVerFunction6C(P_model);
			break;

		case 3:			// 3: last updated date
			SendVerFunction6C(P_date);
			break;

		case 4:			// 4: internal revision
			Data_To_Host_nWait6C(P_digit[0]);
			Data_To_Host6C(P_digit[1]);
			break;

		default:
			break;
		}
	}
}


void PM2_6CCmd_DAData(void)
{

}


void PM2_6CCmd_DBData(void)
{

}


void PM2_6CCmd_DCData(void)
{

}


void PM2_6CCmd_DDData(void)
{

}


void PM2_6CCmd_DEData(void)
{

}


void PM2_6CCmd_DFData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data Ex
//==============================================================================
void PM2_6CCmd_E0Data(void)
{

}


void PM2_6CCmd_E1Data(void)
{

}


void PM2_6CCmd_E2Data(void)
{

}


void PM2_6CCmd_E3Data(void)
{

}


void PM2_6CCmd_E4Data(void)
{

}


void PM2_6CCmd_E5Data(void)
{

}


void PM2_6CCmd_E6Data(void)
{

}


void PM2_6CCmd_E7Data(void)
{

}


void PM2_6CCmd_E8Data(void)
{

}


void PM2_6CCmd_E9Data(void)
{

}


void PM2_6CCmd_EAData(void)
{

}


void PM2_6CCmd_EBData(void)
{

}


void PM2_6CCmd_ECData(void)
{

}


void PM2_6CCmd_EDData(void)
{

}


void PM2_6CCmd_EEData(void)
{

}


void PM2_6CCmd_EFData(void)
{

}


//==============================================================================
// Handle ACPI 6C port data Fx
//==============================================================================
void PM2_6CCmd_F0Data(void)
{

}


void PM2_6CCmd_F1Data(void)
{

}


void PM2_6CCmd_F2Data(void)
{

}


void PM2_6CCmd_F3Data(void)
{

}


void PM2_6CCmd_F4Data(void)
{

}


void PM2_6CCmd_F5Data(void)
{

}


void PM2_6CCmd_F6Data(void)
{

}


void PM2_6CCmd_F7Data(void)
{

}


void PM2_6CCmd_F8Data(void)
{

}


void PM2_6CCmd_F9Data(void)
{

}


void PM2_6CCmd_FAData(void)
{

}


void PM2_6CCmd_FBData(void)
{

}


void PM2_6CCmd_FCData(void)
{

}


void PM2_6CCmd_FDData(void)
{

}


void PM2_6CCmd_FEData(void)
{

}


void PM2_6CCmd_FFData(void)
{

}


//------------------------------------------------------------------------------
// Turn off power for command 8Ch
//------------------------------------------------------------------------------
void DoTurnOffPower_6C8C(void)
{
	ShutDnCause = 0xAC;
	ForceOffPower();
}
