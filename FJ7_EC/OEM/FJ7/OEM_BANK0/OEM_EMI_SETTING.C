#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>
/////////////////////////////////////////////////////////////////////////////////
//-----------------------------RT213XN init table-------------------------------//
/////////////////////////////////////////////////////////////////////////////////
#ifdef	EMI_Update_Support
extern BYTE vOSSMbusWBYTE(void);

const sInitConverterReg code asInitConverterReg[] = {
// T1 ~ T7 setting(Power sequence)
{ 0x01 , 0x32},
{ 0x00 , T1_Value},
{ 0x01 , 0x33},
{ 0x00 , T2_Value},
{ 0x01 , 0x34},
{ 0x00 , T3_Value},
{ 0x01 , 0x35},
{ 0x00 , T4_Value},
{ 0x01 , 0x36},
{ 0x00 , T5_Value},
{ 0x01 , 0x37},
{ 0x00 , T6_Value},
{ 0x01 , 0x38},
{ 0x00 , T7_Value},

// SSCG
{ 0x01 , 0x39},
{ 0x00 , SSCG_CFG0},
{ 0x01 , 0x3A},
{ 0x00 , SSCG_CFG1},

// LVDS swap
{ 0x01 , 0x3B},
{ 0x00 , LVDS_MISC},

// Default setting
{ 0x01 , 0x3C},
{ 0x00 , 0x06},
{ 0x01 , 0x3D},
{ 0x00 , 0x38},
{ 0x01 , 0x3E},
{ 0x00 , 0x73},
{ 0x01 , 0x3F},
{ 0x00 , 0x33},
{ 0x01 , 0x06},
{ 0x00 , 0x90},
{ 0x01 , 0x06},
{ 0x00 , 0xB0},
{ 0x01 , 0x06},
{ 0x00 , 0x80},

// Last command of configuration
{ 0x80 , 0x00},
};

void Converter_Config_Start(void)
{
	BYTE temp = 0x00;
	int ErrorCNT = 0;

	if ((IsFlag0(Converter_Config_Status, Stop_FW_Fin)) || (IsFlag1(Converter_Config_Status, Config_Fin)))	// Check Stop FW command not send or config finish
		return;
	if (IsFlag0(Converter_Config_Status, Stop_FW_Delay_Fin))	// Check stop FW command delay
	{
		SetFlag(Converter_Config_Status, Stop_FW_Delay_Fin);	// Do stop FW command delay
		return;
	}
	while(temp < (sizeof(asInitConverterReg)/sizeof(sInitConverterReg)))
	{
		while (Converter_Write_Reg( asInitConverterReg[temp].reg, asInitConverterReg[temp].value))
		{
			//if fail rewrite again
			ErrorCNT++;
			if (ErrorCNT > 10)
			{
				SetFlag(Converter_Config_Status, Error_Over);
				ClrFlag(ABAT_MISC2, Shipmode);
				return;
			}
		}
		ErrorCNT = 0;
		temp++;
	}
	SetFlag(Converter_Config_Status, Config_Fin);	// Config finish
	ClrFlag(ABAT_MISC2, Shipmode);
}

void Converter_FW_Stop(void)
{
	int ErrorCNT = 0;
	if (IsFlag1(Converter_Config_Status, Stop_FW_Fin))		// Check Stop FW command sended finish
		return;
	if (IsFlag0(Converter_Config_Status, Fisrt_Delay_Fin))	// Check first delay
	{
		SetFlag(Converter_Config_Status, Fisrt_Delay_Fin);	// Do first delay
		return;
	}
	SetFlag(ABAT_MISC2, Shipmode);
	while (Converter_Write_Reg(0x80, 0x01))
	{
		//if fail rewrite again
		ErrorCNT++;
		if (ErrorCNT > 10)
		{
			SetFlag(Converter_Config_Status, Error_Over);
			ClrFlag(ABAT_MISC2, Shipmode);
			return;
		}
	}
	SetFlag(Converter_Config_Status, Stop_FW_Fin);			// Send stop FW command finish
}

BYTE Converter_Write_Reg(BYTE Cmd, BYTE Data)
{
	SMB_ADDR = EMI_ADDR;
	SMB_CMD = Cmd;
	SMB_DATA = Data;
	if (vOSSMbusWBYTE())
		return 0;
	else
		return 1;
}

#endif