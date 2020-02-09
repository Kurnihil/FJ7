/*------------------------------------------------------------------------------
 * Title : OEM_6064.H
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#ifndef	OEM_6064_H
#define	OEM_6064_H


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void Hook_60PortDebug(void);
extern void Hook_64PortDebug(void);

extern void Hook_Port64CmdFX(void);	// leox_20110926

extern void Cmd_00(void);
extern void Cmd_01(void);
extern void Cmd_02(void);
extern void Cmd_03(void);
extern void Cmd_04(void);
extern void Cmd_05(void);
extern void Cmd_06(void);
extern void Cmd_07(void);
extern void Cmd_08(void);
extern void Cmd_09(void);
extern void Cmd_0A(void);
extern void Cmd_0B(void);
extern void Cmd_0C(void);
extern void Cmd_0D(void);
extern void Cmd_0E(void);
extern void Cmd_0F(void);

extern void Cmd_10(void);
extern void Cmd_11(void);
extern void Cmd_12(void);
extern void Cmd_13(void);
extern void Cmd_14(void);
extern void Cmd_15(void);
extern void Cmd_16(void);
extern void Cmd_17(void);
extern void Cmd_18(void);
extern void Cmd_19(void);
extern void Cmd_1A(void);
extern void Cmd_1B(void);
extern void Cmd_1C(void);
extern void Cmd_1D(void);
extern void Cmd_1E(void);
extern void Cmd_1F(void);

extern void Cmd_40(void);
extern void Cmd_41(void);
extern void Cmd_42(void);
extern void Cmd_43(void);
extern void Cmd_44(void);
extern void Cmd_45(void);
extern void Cmd_46(void);
extern void Cmd_47(void);
extern void Cmd_48(void);
extern void Cmd_49(void);
extern void Cmd_4A(void);
extern void Cmd_4B(void);
extern void Cmd_4C(void);
extern void Cmd_4D(void);
extern void Cmd_4E(void);
extern void Cmd_4F(void);

extern void Cmd_50(void);
extern void Cmd_51(void);
extern void Cmd_52(void);
extern void Cmd_53(void);
extern void Cmd_54(void);
extern void Cmd_55(void);
extern void Cmd_56(void);
extern void Cmd_57(void);
extern void Cmd_58(void);
extern void Cmd_59(void);
extern void Cmd_5A(void);
extern void Cmd_5B(void);
extern void Cmd_5C(void);
extern void Cmd_5D(void);
extern void Cmd_5E(void);
extern void Cmd_5F(void);

extern void Cmd_80(void);
extern void Cmd_81(void);
extern void Cmd_82(void);
extern void Cmd_83(void);
extern void Cmd_84(void);
extern void Cmd_85(void);
extern void Cmd_86(void);
extern void Cmd_87(void);
extern void Cmd_88(void);
extern void Cmd_89(void);
extern void Cmd_8A(void);
extern void Cmd_8B(void);
extern void Cmd_8C(void);
extern void Cmd_8D(void);
extern void Cmd_8E(void);
extern void Cmd_8F(void);

extern void Cmd_94(void);
extern void Cmd_95(void);
extern void Cmd_96(void);
extern void Cmd_97(void);
extern void Cmd_98(void);
extern void Cmd_99(void);
extern void Cmd_9A(void);
extern void Cmd_9B(void);
extern void Cmd_9C(void);
extern void Cmd_9D(void);
extern void Cmd_9E(void);
extern void Cmd_9F(void);

extern void Cmd_B0(void);
extern void Cmd_B1(void);
extern void Cmd_B2(void);
extern void Cmd_B3(void);
extern void Cmd_B4(void);
extern void Cmd_B5(void);
extern void Cmd_B6(void);
extern void Cmd_B7(void);
extern void Cmd_B8(void);
extern void Cmd_B9(void);
extern void Cmd_BA(void);
extern void Cmd_BB(void);
extern void Cmd_BC(void);
extern void Cmd_BD(void);
extern void Cmd_BE(void);
extern void Cmd_BF(void);

extern void Cmd_C8(void);
extern void Cmd_C9(void);
extern void Cmd_CA(void);
extern void Cmd_CB(void);
extern void Cmd_CC(void);
extern void Cmd_CD(void);
extern void Cmd_CE(void);
extern void Cmd_CF(void);

extern void Cmd_D8(void);
extern void Cmd_D9(void);
extern void Cmd_DA(void);
extern void Cmd_DB(void);
extern void Cmd_DC(void);
extern void Cmd_DD(void);
extern void Cmd_DE(void);
extern void Cmd_DF(void);

extern void Cmd_E2(void);
extern void Cmd_E3(void);
extern void Cmd_E4(void);
extern void Cmd_E5(void);
extern void Cmd_E6(void);
extern void Cmd_E7(void);
extern void Cmd_E8(void);
extern void Cmd_E9(void);
extern void Cmd_EA(void);
extern void Cmd_EB(void);
extern void Cmd_EC(void);
extern void Cmd_ED(void);
extern void Cmd_EE(void);
extern void Cmd_EF(void);

extern void Cmd_00Data(void);
extern void Cmd_01Data(void);
extern void Cmd_02Data(void);
extern void Cmd_03Data(void);
extern void Cmd_04Data(void);
extern void Cmd_05Data(void);
extern void Cmd_06Data(void);
extern void Cmd_07Data(void);
extern void Cmd_08Data(void);
extern void Cmd_09Data(void);
extern void Cmd_0AData(void);
extern void Cmd_0BData(void);
extern void Cmd_0CData(void);
extern void Cmd_0DData(void);
extern void Cmd_0EData(void);
extern void Cmd_0FData(void);

extern void Cmd_10Data(void);
extern void Cmd_11Data(void);
extern void Cmd_12Data(void);
extern void Cmd_13Data(void);
extern void Cmd_14Data(void);
extern void Cmd_15Data(void);
extern void Cmd_16Data(void);
extern void Cmd_17Data(void);
extern void Cmd_18Data(void);
extern void Cmd_19Data(void);
extern void Cmd_1AData(void);
extern void Cmd_1BData(void);
extern void Cmd_1CData(void);
extern void Cmd_1DData(void);
extern void Cmd_1EData(void);
extern void Cmd_1FData(void);

extern void Cmd_40Data(void);
extern void Cmd_41Data(void);
extern void Cmd_42Data(void);
extern void Cmd_43Data(void);
extern void Cmd_44Data(void);
extern void Cmd_45Data(void);
extern void Cmd_46Data(void);
extern void Cmd_47Data(void);
extern void Cmd_48Data(void);
extern void Cmd_49Data(void);
extern void Cmd_4AData(void);
extern void Cmd_4BData(void);
extern void Cmd_4CData(void);
extern void Cmd_4DData(void);
extern void Cmd_4EData(void);
extern void Cmd_4FData(void);

extern void Cmd_50Data(void);
extern void Cmd_51Data(void);
extern void Cmd_52Data(void);
extern void Cmd_53Data(void);
extern void Cmd_54Data(void);
extern void Cmd_55Data(void);
extern void Cmd_56Data(void);
extern void Cmd_57Data(void);
extern void Cmd_58Data(void);
extern void Cmd_59Data(void);
extern void Cmd_5AData(void);
extern void Cmd_5BData(void);
extern void Cmd_5CData(void);
extern void Cmd_5DData(void);
extern void Cmd_5EData(void);
extern void Cmd_5FData(void);

extern void Cmd_80Data(void);
extern void Cmd_81Data(void);
extern void Cmd_82Data(void);
extern void Cmd_83Data(void);
extern void Cmd_84Data(void);
extern void Cmd_85Data(void);
extern void Cmd_86Data(void);
extern void Cmd_87Data(void);
extern void Cmd_88Data(void);
extern void Cmd_89Data(void);
extern void Cmd_8AData(void);
extern void Cmd_8BData(void);
extern void Cmd_8CData(void);
extern void Cmd_8DData(void);
extern void Cmd_8EData(void);
extern void Cmd_8FData(void);

extern void Cmd_94Data(void);
extern void Cmd_95Data(void);
extern void Cmd_96Data(void);
extern void Cmd_97Data(void);
extern void Cmd_98Data(void);
extern void Cmd_99Data(void);
extern void Cmd_9AData(void);
extern void Cmd_9BData(void);
extern void Cmd_9CData(void);
extern void Cmd_9DData(void);
extern void Cmd_9EData(void);
extern void Cmd_9FData(void);

extern void Cmd_B0Data(void);
extern void Cmd_B1Data(void);
extern void Cmd_B2Data(void);
extern void Cmd_B3Data(void);
extern void Cmd_B4Data(void);
extern void Cmd_B5Data(void);
extern void Cmd_B6Data(void);
extern void Cmd_B7Data(void);
extern void Cmd_B8Data(void);
extern void Cmd_B9Data(void);
extern void Cmd_BAData(void);
extern void Cmd_BBData(void);
extern void Cmd_BCData(void);
extern void Cmd_BDData(void);
extern void Cmd_BEData(void);
extern void Cmd_BFData(void);

extern void Cmd_C8Data(void);
extern void Cmd_C9Data(void);
extern void Cmd_CAData(void);
extern void Cmd_CBData(void);
extern void Cmd_CCData(void);
extern void Cmd_CDData(void);
extern void Cmd_CEData(void);
extern void Cmd_CFData(void);

extern void Cmd_D8Data(void);
extern void Cmd_D9Data(void);
extern void Cmd_DAData(void);
extern void Cmd_DBData(void);
extern void Cmd_DCData(void);
extern void Cmd_DDData(void);
extern void Cmd_DEData(void);
extern void Cmd_DFData(void);

extern void Cmd_E2Data(void);
extern void Cmd_E3Data(void);
extern void Cmd_E4Data(void);
extern void Cmd_E5Data(void);
extern void Cmd_E6Data(void);
extern void Cmd_E7Data(void);
extern void Cmd_E8Data(void);
extern void Cmd_E9Data(void);
extern void Cmd_EAData(void);
extern void Cmd_EBData(void);
extern void Cmd_ECData(void);
extern void Cmd_EDData(void);
extern void Cmd_EEData(void);
extern void Cmd_EFData(void);


#endif	/* OEM_6064_H */

