#ifndef	OEM_EMI_SETTING_H
#define	OEM_EMI_SETTING_H


#ifdef	EMI_Update_Support

typedef struct InitConverterReg
{
	BYTE	reg;
	BYTE	value;
} sInitConverterReg;

#define	EMI_ADDR		0x6A
#define	T1_Value		0x14
#define	T2_Value		0x37
#define	T3_Value		0x0E
#define	T4_Value		0x02
#define	T5_Value		0x37
#define	T6_Value		0x14
#define	T7_Value		0x82
#define	SSCG_CFG0		0xAB
#define	SSCG_CFG1		0x08		// 0x08 -> 0.5% ; 0x10 -> 1.0% ; 0x18 -> 1.5%
#define	LVDS_MISC		0x04

extern void Converter_Config_Start(void);
extern void Converter_FW_Stop(void);
extern BYTE Converter_Write_Reg(BYTE Cmd, BYTE Data);

#endif
#endif
