/*------------------------------------------------------------------------------
 * Title : OEM_ACPI.H
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#ifndef	OEM_ACPI_H
#define	OEM_ACPI_H


//------------------------------------------------------------------------------
// Common Definition
//------------------------------------------------------------------------------
#define	ACPICmdSCI		0x00
#define	QeventSCI		0x01


//------------------------------------------------------------------------------
// SCI Mode
//------------------------------------------------------------------------------
#define	SCIMode_Normal	0x00	// For OEM SCI event
#define	SCIMode_Pending	0x01
#define	SCIMode_Force	0x02
#define	SCIMode_SMBus	0x03	// leox_20111017 SCIMode_ACPI_SMBus -> SCIMode_SMBus


//------------------------------------------------------------------------------
// Event Code
//------------------------------------------------------------------------------
#define	Q09_SCI			0x09	// Delay timer

#define	Q0C_SMISCI		0x0C	// Q0C Event
#define	Q0D_SCI			0x0D	// Q0D Event
#define	Q0E_SCI			0x0E	// Q0E Event
#define	Q0F_SMISCI		0x0F	// Q0F Event
#define	Q10_SMISCI		0x10	// Q10 Event
#define	Q11_SMISCI		0x11	// Q11 Event
#define	Q12_SMISCI		0x12	// Q12 Event
#define	Q13_SMISCI		0x13	// Q13 Event
#define	Q14_SMISCI		0x14	// Q14 Event
#define	Q15_SCI			0x15	// Q15 Event

#define	Q20_SCI			0x20	// AC/Battrey change
#define	PowerManagement	0x20	// Power management

#ifdef	BatteryOnlyXXPThrollFUN
#define	Q40_SCI			0x40	// Battery only and below 20%
#define	Q41_SCI			0x41	// Not Battery only and below 20%
#endif	// BatteryOnlyXXPThrollFUN

#ifdef	OEM_VICMV
#define	Q42_SCI			0x42	// Enabled VICMVThrottling
#define	Q43_SCI			0x43	// Disabled VICMVThrottling
#endif	// OEM_VICMV

#ifdef	OEM_VICMVACDC
#define	Q42_SCIACDC		0x42	// Enabled VICMVThrottling
#define	Q43_SCIACDC		0x43	// Disabled VICMVThrottling
#endif	// OEM_VICMVACDC

#ifdef	OEM_DCMI
#define	Q44_SCI			0x44	// Enabled DCMIThrottling
#define	Q45_SCI			0x45	// Disabled DCMIThrottling
#endif	// OEM_DCMI

#if	SUPPORT_OEM_APP				// leox_20111014 Support OEM application
#define	Q51_SCI			0x51	// Get OS mute state via application			// leox_20120411
#define	Q52_SCI			0x52	// Set OS mute state via application			// leox_20120411
#define	Q53_SCI			0x53	// Get OS volume value via application
#define	Q54_SCI			0x54	// Set OS volume value via application
#define	Q55_SCI			0x55	// Brightness/Volume function switch
#define	Q56_SCI			0x56	// Brightness/Volume down
#define	Q57_SCI			0x57	// Brightness/Volume up
#define	Q58_SCI			0x58	// ODD eject by application						// leox_20120312 Support_ODD_EJECT
#endif	// SUPPORT_OEM_APP

#define	Q80_SCI			0x80	// CPU temperature up
#define	Q81_SCI			0x81	// CPU temperature down

#define	Q8A_SMISWISCI	0x8A	// LID event
#define	Q8B_SMISWISCI	0x8B	// PSW event
#define	Q88_SCI			0x88	// Turbo on disable event
#define	Q89_SCI			0x89	// Turbo on enable event

// For ITE WinFlash Tool ++		// ken 2012/06/29
#define	Q8D_SCI			0x8D	// Notify BIOS not access EC
#define	Q8E_SCI			0x8E	// Notify BIOS can access EC
// For ITE WinFlash Tool --

//David_20141222 add for MMB button
#define	Q8F_SCI			0x8F	// MMB button pressed event
//David_20141222 add for MMB button

//------------------------------------------------------------------------------
// Structure Declaration
//------------------------------------------------------------------------------
typedef struct GetSelState
{
	BYTE	flag;
	XBYTE	*Var;
	BYTE	Conditionflag;
	XBYTE	*ConditionVar;
} sGetSelState;


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void Hook_62PortDebug(void);
extern void Hook_66PortDebug(void);
extern void Hook_68PortDebug(void);
extern void Hook_6CPortDebug(void);

extern void Hook_ACPICommand(void);
extern void Hook_DOSCommand(void);			// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42

extern void Hook_Port66Cmd86(void);			// leox_20120611
extern void Hook_Port66Cmd87(void);			// leox_20120611

extern void ACPI_Gen_Int(BYTE Qevent);
extern void ECQEvent(BYTE sci_number, BYTE sci_mode);
extern void ECSMIEvent(BYTE smi_number);
extern void PendingECQEvent(BYTE sci_number);
extern void Get_PendingECQEvent(void);

extern BYTE Hook_ReadMapECSpace(BYTE MapIndex);
extern void Hook_WriteMapECSpace(BYTE MapIndex, BYTE data1);

extern void vReadPCUCmd(void);
extern void vReadPCUCmd01_GSSS(void);
extern void vWritePCUCmd(void);
extern void vWritePCUCmd23_SMI(void);
extern void vRetWSelectorACK(void);
extern void vGetSelectorState(BYTE getnow);	// @CP_24EC06WWx, ITE modify

extern BYTE vOSSMbusWBlcok_OEM(void);
extern BYTE vOSSMbusRBlcok_OEM(void);
extern BYTE vOSSMbusRByte_OEM(void);
extern BYTE vOSSMbusRWord_OEM(void);
extern BYTE vOSSMbusWBYTE_OEM(void);
extern BYTE vOSSMbusWWord_OEM(void);
extern BYTE vOSSMbusRBlcok_OEM_SMB2(void);
extern BYTE vOSSMbusWBYTE_OEM_SMB2(void);
extern BYTE vOSSMbusRByte_OEM_SMB2(void);
extern BYTE vOSSMbusRBlcok_OEM_SMB3(void);
//David add for thermal sensor use SMB channel 2 2011/09/15
extern BYTE vOSSMbusWBYTE_OEM_SMB3(void);
extern BYTE vOSSMbusRByte_OEM_SMB3(void);
//David add for thermal sensor use SMB channel 2 2011/09/15

extern void ACPI_Cmd_00(void);
extern void ACPI_Cmd_01(void);
extern void ACPI_Cmd_02(void);
extern void ACPI_Cmd_03(void);
extern void ACPI_Cmd_04(void);
extern void ACPI_Cmd_05(void);
extern void ACPI_Cmd_06(void);
extern void ACPI_Cmd_07(void);
extern void ACPI_Cmd_08(void);
extern void ACPI_Cmd_09(void);
extern void ACPI_Cmd_0A(void);
extern void ACPI_Cmd_0B(void);
extern void ACPI_Cmd_0C(void);
extern void ACPI_Cmd_0D(void);
extern void ACPI_Cmd_0E(void);
extern void ACPI_Cmd_0F(void);

extern void ACPI_Cmd_10(void);
extern void ACPI_Cmd_11(void);
extern void ACPI_Cmd_12(void);
extern void ACPI_Cmd_13(void);
extern void ACPI_Cmd_14(void);
extern void ACPI_Cmd_15(void);
extern void ACPI_Cmd_16(void);
extern void ACPI_Cmd_17(void);
extern void ACPI_Cmd_18(void);
extern void ACPI_Cmd_19(void);
extern void ACPI_Cmd_1A(void);
extern void ACPI_Cmd_1B(void);
extern void ACPI_Cmd_1C(void);
extern void ACPI_Cmd_1D(void);
extern void ACPI_Cmd_1E(void);
extern void ACPI_Cmd_1F(void);

extern void ACPI_Cmd_20(void);
extern void ACPI_Cmd_21(void);
extern void ACPI_Cmd_22(void);
extern void ACPI_Cmd_23(void);
extern void ACPI_Cmd_24(void);
extern void ACPI_Cmd_25(void);
extern void ACPI_Cmd_26(void);
extern void ACPI_Cmd_27(void);
extern void ACPI_Cmd_28(void);
extern void ACPI_Cmd_29(void);
extern void ACPI_Cmd_2A(void);
extern void ACPI_Cmd_2B(void);
extern void ACPI_Cmd_2C(void);
extern void ACPI_Cmd_2D(void);
extern void ACPI_Cmd_2E(void);
extern void ACPI_Cmd_2F(void);

extern void ACPI_Cmd_30(void);
extern void ACPI_Cmd_31(void);
extern void ACPI_Cmd_32(void);
extern void ACPI_Cmd_33(void);
extern void ACPI_Cmd_34(void);
extern void ACPI_Cmd_35(void);
extern void ACPI_Cmd_36(void);
extern void ACPI_Cmd_37(void);
extern void ACPI_Cmd_38(void);
extern void ACPI_Cmd_39(void);
extern void ACPI_Cmd_3A(void);
extern void ACPI_Cmd_3B(void);
extern void ACPI_Cmd_3C(void);
extern void ACPI_Cmd_3D(void);
extern void ACPI_Cmd_3E(void);
extern void ACPI_Cmd_3F(void);

extern void ACPI_Cmd_40(void);
extern void ACPI_Cmd_41(void);
extern void ACPI_Cmd_42(void);
extern void ACPI_Cmd_43(void);
extern void ACPI_Cmd_44(void);
extern void ACPI_Cmd_45(void);
extern void ACPI_Cmd_46(void);
extern void ACPI_Cmd_47(void);
extern void ACPI_Cmd_48(void);
extern void ACPI_Cmd_49(void);
extern void ACPI_Cmd_4A(void);
extern void ACPI_Cmd_4B(void);
extern void ACPI_Cmd_4C(void);
extern void ACPI_Cmd_4D(void);
extern void ACPI_Cmd_4E(void);
extern void ACPI_Cmd_4F(void);

extern void ACPI_Cmd_50(void);
extern void ACPI_Cmd_51(void);
extern void ACPI_Cmd_52(void);
extern void ACPI_Cmd_53(void);
extern void ACPI_Cmd_54(void);
extern void ACPI_Cmd_55(void);
extern void ACPI_Cmd_56(void);
extern void ACPI_Cmd_57(void);
extern void ACPI_Cmd_58(void);
extern void ACPI_Cmd_59(void);
extern void ACPI_Cmd_5A(void);
extern void ACPI_Cmd_5B(void);
extern void ACPI_Cmd_5C(void);
extern void ACPI_Cmd_5D(void);
extern void ACPI_Cmd_5E(void);
extern void ACPI_Cmd_5F(void);

extern void ACPI_Cmd_60(void);
extern void ACPI_Cmd_61(void);
extern void ACPI_Cmd_62(void);
extern void ACPI_Cmd_63(void);
extern void ACPI_Cmd_64(void);
extern void ACPI_Cmd_65(void);
extern void ACPI_Cmd_66(void);
extern void ACPI_Cmd_67(void);
extern void ACPI_Cmd_68(void);
extern void ACPI_Cmd_69(void);
extern void ACPI_Cmd_6A(void);
extern void ACPI_Cmd_6B(void);
extern void ACPI_Cmd_6C(void);
extern void ACPI_Cmd_6D(void);
extern void ACPI_Cmd_6E(void);
extern void ACPI_Cmd_6F(void);

extern void ACPI_Cmd_70(void);
extern void ACPI_Cmd_71(void);
extern void ACPI_Cmd_72(void);
extern void ACPI_Cmd_73(void);
extern void ACPI_Cmd_74(void);
extern void ACPI_Cmd_75(void);
extern void ACPI_Cmd_76(void);
extern void ACPI_Cmd_77(void);
extern void ACPI_Cmd_78(void);
extern void ACPI_Cmd_79(void);
extern void ACPI_Cmd_7A(void);
extern void ACPI_Cmd_7B(void);
extern void ACPI_Cmd_7C(void);
extern void ACPI_Cmd_7D(void);
extern void ACPI_Cmd_7E(void);
extern void ACPI_Cmd_7F(void);

extern void ACPI_Cmd_85(void);
extern void ACPI_Cmd_86(void);
extern void ACPI_Cmd_87(void);
extern void ACPI_Cmd_88(void);
extern void ACPI_Cmd_89(void);
extern void ACPI_Cmd_8A(void);
extern void ACPI_Cmd_8B(void);
extern void ACPI_Cmd_8C(void);
extern void ACPI_Cmd_8D(void);
extern void ACPI_Cmd_8E(void);
extern void ACPI_Cmd_8F(void);

extern void ACPI_Cmd_90(void);
extern void ACPI_Cmd_91(void);
extern void ACPI_Cmd_94(void);
extern void ACPI_Cmd_95(void);
extern void ACPI_Cmd_96(void);
extern void ACPI_Cmd_97(void);
extern void ACPI_Cmd_98(void);
extern void ACPI_Cmd_99(void);
extern void ACPI_Cmd_9A(void);
extern void ACPI_Cmd_9B(void);
extern void ACPI_Cmd_9C(void);
extern void ACPI_Cmd_9D(void);
extern void ACPI_Cmd_9E(void);
extern void ACPI_Cmd_9F(void);

extern void ACPI_Cmd_A0(void);
extern void ACPI_Cmd_A1(void);
extern void ACPI_Cmd_A2(void);
extern void ACPI_Cmd_A3(void);
extern void ACPI_Cmd_A4(void);
extern void ACPI_Cmd_A5(void);
extern void ACPI_Cmd_A6(void);
extern void ACPI_Cmd_A7(void);
extern void ACPI_Cmd_A8(void);
extern void ACPI_Cmd_A9(void);
extern void ACPI_Cmd_AA(void);
extern void ACPI_Cmd_AB(void);
extern void ACPI_Cmd_AC(void);
extern void ACPI_Cmd_AD(void);
extern void ACPI_Cmd_AE(void);
extern void ACPI_Cmd_AF(void);

extern void ACPI_Cmd_B0(void);
extern void ACPI_Cmd_B1(void);
extern void ACPI_Cmd_B2(void);
extern void ACPI_Cmd_B3(void);
extern void ACPI_Cmd_B4(void);
extern void ACPI_Cmd_B5(void);
extern void ACPI_Cmd_B6(void);
extern void ACPI_Cmd_B7(void);
extern void ACPI_Cmd_B8(void);
extern void ACPI_Cmd_B9(void);
extern void ACPI_Cmd_BA(void);
extern void ACPI_Cmd_BB(void);
extern void ACPI_Cmd_BC(void);
extern void ACPI_Cmd_BD(void);
extern void ACPI_Cmd_BE(void);
extern void ACPI_Cmd_BF(void);

extern void ACPI_Cmd_C0(void);
extern void ACPI_Cmd_C1(void);
extern void ACPI_Cmd_C2(void);
extern void ACPI_Cmd_C3(void);
extern void ACPI_Cmd_C4(void);
extern void ACPI_Cmd_C5(void);
extern void ACPI_Cmd_C6(void);
extern void ACPI_Cmd_C7(void);
extern void ACPI_Cmd_C8(void);
extern void ACPI_Cmd_C9(void);
extern void ACPI_Cmd_CA(void);
extern void ACPI_Cmd_CB(void);
extern void ACPI_Cmd_CC(void);
extern void ACPI_Cmd_CD(void);
extern void ACPI_Cmd_CE(void);
extern void ACPI_Cmd_CF(void);

extern void ACPI_Cmd_D0(void);
extern void ACPI_Cmd_D1(void);
extern void ACPI_Cmd_D2(void);
extern void ACPI_Cmd_D3(void);
extern void ACPI_Cmd_D4(void);
extern void ACPI_Cmd_D5(void);
extern void ACPI_Cmd_D6(void);
extern void ACPI_Cmd_D7(void);
extern void ACPI_Cmd_D8(void);
extern void ACPI_Cmd_D9(void);
extern void ACPI_Cmd_DA(void);
extern void ACPI_Cmd_DB(void);
extern void ACPI_Cmd_DD(void);
extern void ACPI_Cmd_DE(void);
extern void ACPI_Cmd_DF(void);

extern void ACPI_Cmd_E0(void);
extern void ACPI_Cmd_E1(void);
extern void ACPI_Cmd_E2(void);
extern void ACPI_Cmd_E3(void);
extern void ACPI_Cmd_E4(void);
extern void ACPI_Cmd_E5(void);
extern void ACPI_Cmd_E6(void);
extern void ACPI_Cmd_E7(void);
extern void ACPI_Cmd_E8(void);
extern void ACPI_Cmd_E9(void);
extern void ACPI_Cmd_EA(void);
extern void ACPI_Cmd_EB(void);
extern void ACPI_Cmd_EC(void);
extern void ACPI_Cmd_ED(void);
extern void ACPI_Cmd_EE(void);
extern void ACPI_Cmd_EF(void);

extern void ACPI_Cmd_00Data(void);
extern void ACPI_Cmd_01Data(void);
extern void ACPI_Cmd_02Data(void);
extern void ACPI_Cmd_03Data(void);
extern void ACPI_Cmd_04Data(void);
extern void ACPI_Cmd_05Data(void);
extern void ACPI_Cmd_06Data(void);
extern void ACPI_Cmd_07Data(void);
extern void ACPI_Cmd_08Data(void);
extern void ACPI_Cmd_09Data(void);
extern void ACPI_Cmd_0AData(void);
extern void ACPI_Cmd_0BData(void);
extern void ACPI_Cmd_0CData(void);
extern void ACPI_Cmd_0DData(void);
extern void ACPI_Cmd_0EData(void);
extern void ACPI_Cmd_0FData(void);

extern void ACPI_Cmd_10Data(void);
extern void ACPI_Cmd_11Data(void);
extern void ACPI_Cmd_12Data(void);
extern void ACPI_Cmd_13Data(void);
extern void ACPI_Cmd_14Data(void);
extern void ACPI_Cmd_15Data(void);
extern void ACPI_Cmd_16Data(void);
extern void ACPI_Cmd_17Data(void);
extern void ACPI_Cmd_18Data(void);
extern void ACPI_Cmd_19Data(void);
extern void ACPI_Cmd_1AData(void);
extern void ACPI_Cmd_1BData(void);
extern void ACPI_Cmd_1CData(void);
extern void ACPI_Cmd_1DData(void);
extern void ACPI_Cmd_1EData(void);
extern void ACPI_Cmd_1FData(void);

extern void ACPI_Cmd_20Data(void);
extern void ACPI_Cmd_21Data(void);
extern void ACPI_Cmd_22Data(void);
extern void ACPI_Cmd_23Data(void);
extern void ACPI_Cmd_24Data(void);
extern void ACPI_Cmd_25Data(void);
extern void ACPI_Cmd_26Data(void);
extern void ACPI_Cmd_27Data(void);
extern void ACPI_Cmd_28Data(void);
extern void ACPI_Cmd_29Data(void);
extern void ACPI_Cmd_2AData(void);
extern void ACPI_Cmd_2BData(void);
extern void ACPI_Cmd_2CData(void);
extern void ACPI_Cmd_2DData(void);
extern void ACPI_Cmd_2EData(void);
extern void ACPI_Cmd_2FData(void);

extern void ACPI_Cmd_30Data(void);
extern void ACPI_Cmd_31Data(void);
extern void ACPI_Cmd_32Data(void);
extern void ACPI_Cmd_33Data(void);
extern void ACPI_Cmd_34Data(void);
extern void ACPI_Cmd_35Data(void);
extern void ACPI_Cmd_36Data(void);
extern void ACPI_Cmd_37Data(void);
extern void ACPI_Cmd_38Data(void);
extern void ACPI_Cmd_39Data(void);
extern void ACPI_Cmd_3AData(void);
extern void ACPI_Cmd_3BData(void);
extern void ACPI_Cmd_3CData(void);
extern void ACPI_Cmd_3DData(void);
extern void ACPI_Cmd_3EData(void);
extern void ACPI_Cmd_3FData(void);

extern void ACPI_Cmd_40Data(void);
extern void ACPI_Cmd_41Data(void);
extern void ACPI_Cmd_42Data(void);
extern void ACPI_Cmd_43Data(void);
extern void ACPI_Cmd_44Data(void);
extern void ACPI_Cmd_45Data(void);
extern void ACPI_Cmd_46Data(void);
extern void ACPI_Cmd_47Data(void);
extern void ACPI_Cmd_48Data(void);
extern void ACPI_Cmd_49Data(void);
extern void ACPI_Cmd_4AData(void);
extern void ACPI_Cmd_4BData(void);
extern void ACPI_Cmd_4CData(void);
extern void ACPI_Cmd_4DData(void);
extern void ACPI_Cmd_4EData(void);
extern void ACPI_Cmd_4FData(void);

extern void ACPI_Cmd_50Data(void);
extern void ACPI_Cmd_51Data(void);
extern void ACPI_Cmd_52Data(void);
extern void ACPI_Cmd_53Data(void);
extern void ACPI_Cmd_54Data(void);
extern void ACPI_Cmd_55Data(void);
extern void ACPI_Cmd_56Data(void);
extern void ACPI_Cmd_57Data(void);
extern void ACPI_Cmd_58Data(void);
extern void ACPI_Cmd_59Data(void);
extern void ACPI_Cmd_5AData(void);
extern void ACPI_Cmd_5BData(void);
extern void ACPI_Cmd_5CData(void);
extern void ACPI_Cmd_5DData(void);
extern void ACPI_Cmd_5EData(void);
extern void ACPI_Cmd_5FData(void);

extern void ACPI_Cmd_60Data(void);
extern void ACPI_Cmd_61Data(void);
extern void ACPI_Cmd_62Data(void);
extern void ACPI_Cmd_63Data(void);
extern void ACPI_Cmd_64Data(void);
extern void ACPI_Cmd_65Data(void);
extern void ACPI_Cmd_66Data(void);
extern void ACPI_Cmd_67Data(void);
extern void ACPI_Cmd_68Data(void);
extern void ACPI_Cmd_69Data(void);
extern void ACPI_Cmd_6AData(void);
extern void ACPI_Cmd_6BData(void);
extern void ACPI_Cmd_6CData(void);
extern void ACPI_Cmd_6DData(void);
extern void ACPI_Cmd_6EData(void);
extern void ACPI_Cmd_6FData(void);

extern void ACPI_Cmd_70Data(void);
extern void ACPI_Cmd_71Data(void);
extern void ACPI_Cmd_72Data(void);
extern void ACPI_Cmd_73Data(void);
extern void ACPI_Cmd_74Data(void);
extern void ACPI_Cmd_75Data(void);
extern void ACPI_Cmd_76Data(void);
extern void ACPI_Cmd_77Data(void);
extern void ACPI_Cmd_78Data(void);
extern void ACPI_Cmd_79Data(void);
extern void ACPI_Cmd_7AData(void);
extern void ACPI_Cmd_7BData(void);
extern void ACPI_Cmd_7CData(void);
extern void ACPI_Cmd_7DData(void);
extern void ACPI_Cmd_7EData(void);
extern void ACPI_Cmd_7FData(void);

extern void ACPI_Cmd_85Data(void);
extern void ACPI_Cmd_86Data(void);
extern void ACPI_Cmd_87Data(void);
extern void ACPI_Cmd_88Data(void);
extern void ACPI_Cmd_89Data(void);
extern void ACPI_Cmd_8AData(void);
extern void ACPI_Cmd_8BData(void);
extern void ACPI_Cmd_8CData(void);
extern void ACPI_Cmd_8DData(void);
extern void ACPI_Cmd_8EData(void);
extern void ACPI_Cmd_8FData(void);

extern void ACPI_Cmd_90Data(void);
extern void ACPI_Cmd_91Data(void);
extern void ACPI_Cmd_94Data(void);
extern void ACPI_Cmd_95Data(void);
extern void ACPI_Cmd_96Data(void);
extern void ACPI_Cmd_97Data(void);
extern void ACPI_Cmd_98Data(void);
extern void ACPI_Cmd_99Data(void);
extern void ACPI_Cmd_9AData(void);
extern void ACPI_Cmd_9BData(void);
extern void ACPI_Cmd_9CData(void);
extern void ACPI_Cmd_9DData(void);
extern void ACPI_Cmd_9EData(void);
extern void ACPI_Cmd_9FData(void);

extern void ACPI_Cmd_A0Data(void);
extern void ACPI_Cmd_A1Data(void);
extern void ACPI_Cmd_A2Data(void);
extern void ACPI_Cmd_A3Data(void);
extern void ACPI_Cmd_A4Data(void);
extern void ACPI_Cmd_A5Data(void);
extern void ACPI_Cmd_A6Data(void);
extern void ACPI_Cmd_A7Data(void);
extern void ACPI_Cmd_A8Data(void);
extern void ACPI_Cmd_A9Data(void);
extern void ACPI_Cmd_AAData(void);
extern void ACPI_Cmd_ABData(void);
extern void ACPI_Cmd_ACData(void);
extern void ACPI_Cmd_ADData(void);
extern void ACPI_Cmd_AEData(void);
extern void ACPI_Cmd_AFData(void);

extern void ACPI_Cmd_B0Data(void);
extern void ACPI_Cmd_B1Data(void);
extern void ACPI_Cmd_B2Data(void);
extern void ACPI_Cmd_B3Data(void);
extern void ACPI_Cmd_B4Data(void);
extern void ACPI_Cmd_B5Data(void);
extern void ACPI_Cmd_B6Data(void);
extern void ACPI_Cmd_B7Data(void);
extern void ACPI_Cmd_B8Data(void);
extern void ACPI_Cmd_B9Data(void);
extern void ACPI_Cmd_BAData(void);
extern void ACPI_Cmd_BBData(void);
extern void ACPI_Cmd_BCData(void);
extern void ACPI_Cmd_BDData(void);
extern void ACPI_Cmd_BEData(void);
extern void ACPI_Cmd_BFData(void);

extern void ACPI_Cmd_C0Data(void);
extern void ACPI_Cmd_C1Data(void);
extern void ACPI_Cmd_C2Data(void);
extern void ACPI_Cmd_C3Data(void);
extern void ACPI_Cmd_C4Data(void);
extern void ACPI_Cmd_C5Data(void);
extern void ACPI_Cmd_C6Data(void);
extern void ACPI_Cmd_C7Data(void);
extern void ACPI_Cmd_C8Data(void);
extern void ACPI_Cmd_C9Data(void);
extern void ACPI_Cmd_CAData(void);
extern void ACPI_Cmd_CBData(void);
extern void ACPI_Cmd_CCData(void);
extern void ACPI_Cmd_CDData(void);
extern void ACPI_Cmd_CEData(void);
extern void ACPI_Cmd_CFData(void);

extern void ACPI_Cmd_D0Data(void);
extern void ACPI_Cmd_D1Data(void);
extern void ACPI_Cmd_D2Data(void);
extern void ACPI_Cmd_D3Data(void);
extern void ACPI_Cmd_D4Data(void);
extern void ACPI_Cmd_D5Data(void);
extern void ACPI_Cmd_D6Data(void);
extern void ACPI_Cmd_D7Data(void);
extern void ACPI_Cmd_D9Data(void);
extern void ACPI_Cmd_DAData(void);
extern void ACPI_Cmd_DBData(void);
extern void ACPI_Cmd_DEData(void);
extern void ACPI_Cmd_DFData(void);

extern void ACPI_Cmd_E0Data(void);
extern void ACPI_Cmd_E1Data(void);
extern void ACPI_Cmd_E2Data(void);
extern void ACPI_Cmd_E3Data(void);
extern void ACPI_Cmd_E4Data(void);
extern void ACPI_Cmd_E5Data(void);
extern void ACPI_Cmd_E6Data(void);
extern void ACPI_Cmd_E7Data(void);
extern void ACPI_Cmd_E8Data(void);
extern void ACPI_Cmd_E9Data(void);
extern void ACPI_Cmd_EAData(void);
extern void ACPI_Cmd_EBData(void);
extern void ACPI_Cmd_ECData(void);
extern void ACPI_Cmd_EDData(void);
extern void ACPI_Cmd_EEData(void);
extern void ACPI_Cmd_EFData(void);


#endif	/* OEM_ACPI_H */

