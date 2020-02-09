/*------------------------------------------------------------------------------
 * Title : OEM_ACPI.C
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//------------------------------------------------------------------------------
// Internal Function Declaration
//------------------------------------------------------------------------------
void SMI_Gen_Int(void);
#if	1	// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42
void vECSelectorComd_DOS(void);
void vECSelectorComd_ACPI(void);
#else	// (original)
void vECSelectorComd(void);
#endif
void Force_Set_SCI_Cause(BYTE sci_number);	// @CP_24EC04WWa, for Q-event(20) only
void DummyFunction(void);
#if	1	// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42
void OSSMbusComd_DOS(void);
void OSSMbusComd_ACPI(void);
#else	// (original)
void OSSMbusComd(void);
#endif
BYTE vOSSMbusWBlcok(void);
BYTE vOSSMbusRBlcok(void);
BYTE vOSSMbusRByte(void);
BYTE vOSSMbusRWord(void);
BYTE vOSSMbusWBYTE(void);
BYTE vOSSMbusWWord(void);
void DoTurnOffPower_668C(void);


//******************************************************************************
// For kernel code using, don't modify these function -->
//******************************************************************************
//------------------------------------------------------------------------------
// Debug for port 62h
//------------------------------------------------------------------------------
void Hook_62PortDebug(void)
{
	#if	Port62Debug
	RamDebug(0x62); RamDebug(PM1Data);
	#endif
}


//------------------------------------------------------------------------------
// Debug for port 66h
//------------------------------------------------------------------------------
void Hook_66PortDebug(void)
{
	#if	Port66Debug
	RamDebug(0x66); RamDebug(PM1Cmd);
	#endif
}


//------------------------------------------------------------------------------
// Debug for port 68h
//------------------------------------------------------------------------------
void Hook_68PortDebug(void)
{
	#if	Port68Debug
	RamDebug(0x68); RamDebug(PM2Data);
	#endif
}


//------------------------------------------------------------------------------
// Debug for port 6Ch
//------------------------------------------------------------------------------
void Hook_6CPortDebug(void)
{
	#if	Port6CDebug
	RamDebug(0x6C); RamDebug(PM2Cmd);
	#endif
}
//******************************************************************************
// For kernel code using, don't modify these function <--
//******************************************************************************


//------------------------------------------------------------------------------
// Hook function of ACPI command
//------------------------------------------------------------------------------
#if	1	// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42 ++
void Hook_DOSCommand(void)
{
	OSSMbusComd_DOS();		// ACPI SMBus command
	vECSelectorComd_DOS();	// Selector command
}


void Hook_ACPICommand(void)
{
	OSSMbusComd_ACPI();		// ACPI SMBus command
	vECSelectorComd_ACPI();	// Selector command
}


#else	// (original)
void Hook_ACPICommand(void)
{
	#if	ACPI_Smbus
	OSSMbusComd();		// ACPI SMBus command
	#endif
	vECSelectorComd();	// Selector command
}
#endif	// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42 --


//------------------------------------------------------------------------------
// Function: ACPI_Gen_Int - Generate an SCI interrupt.
//
// For a pulsed SCI (edge triggered), the SCI signal is pulsed.
//
// For a level triggered SCI, the SCI signal is set or cleared.  The Host will
// send a command to read the cause of the SCI. The signal will be deactivated
// when no more causes exist.
//------------------------------------------------------------------------------
void ACPI_Gen_Int(BYTE Qevent)
{
	#if	ACPIFlagCtrlQevent
	if (IsFlag0(POWER_FLAG, ACPI_OS)) {return;}
	#endif
	if (Qevent == QeventSCI) {DisableAllInterrupt();}
	SCI_on();
	WNCKR = 0x00;	// Delay 15.26us
	SCI_off();
	if (Qevent == QeventSCI) {EnableAllInterrupt();}
}


//------------------------------------------------------------------------------
// Function: SMI_Gen_Int - Generate an SMI interrupt.
//------------------------------------------------------------------------------
void SMI_Gen_Int(void)
{
	/* fox
	#if	ACPIFlagCtrlQevent
	if (IsFlag1(POWER_FLAG, ACPI_OS)) {return;}
	#endif
	*/
	if (IS_MASK_CLR(PM1STS, P_IBF) && IS_MASK_CLR(KBHISR, IBF))
	{
		KBSMI_on();
		KBSMI_off();
	}
}


void Force_Set_SCI_Cause(BYTE sci_number)
{
	ECQEvent(PowerManagement, SCIMode_Force);
}


//------------------------------------------------------------------------------
// Function: ECQEvent -Setup cause flag for an SCI and start the ACPI_Timer.
//
// sci_number = 1 through 255.  The Host uses this number to determine the
// cause of the SCI.
//------------------------------------------------------------------------------
void ECQEvent(BYTE sci_number, BYTE sci_mode)
{
	// fox
	if (sci_mode != SCIMode_Force)
	{
		if (IsFlag1(POWER_FLAG2, DisQevent)) {return;}
		#if	ACPIFlagCtrlQevent
		if (IsFlag0(POWER_FLAG, ACPI_OS)) {return;}
		#endif
	}
	WrSCIevent_backup = sci_number;
	// fox

	#if	ACPI_QEventPending
	if ((sci_mode == SCIMode_Normal) || (sci_mode == SCIMode_Pending))
	{
		if (sci_mode == SCIMode_Normal)
		{
			// fox
			if (SCI_Event_In_Index != SCI_Event_Out_Index)
			{
				PendingECQEvent(sci_number);
				return;
			}
			//if (PD_SCI_Event_In_Index != PD_SCI_Event_Out_Index)
			//{
			//	PendingECQEvent(sci_number);
			//	return;
			//}
			// fox
		}

		if (IS_MASK_SET(PM1STS, P_IBF) || IS_MASK_SET(KBHISR, IBF))
		{
			if (sci_mode == SCIMode_Pending)
			{
				if (PD_SCI_Event_Out_Index == 0x00)
				{
					PD_SCI_Event_Out_Index = PENDING_BUFFER - 1;
				}
				else
				{
					PD_SCI_Event_Out_Index--;
				}
			}
			else
			{
				PendingECQEvent(sci_number);
			}
			return;
		}
	}
	#endif

	ITempB05 = SCI_Event_In_Index;				// Get the input index
	SCI_Event_Buffer[ITempB05] = sci_number;	// Put the SCI number in the buffer
	ITempB05++;									// Increment the index
	if (ITempB05 >= EVENT_BUFFER_SIZE) {ITempB05 = 0;}
	// If the buffer is not full, update the input index.
	if (ITempB05 != SCI_Event_Out_Index) {SCI_Event_In_Index = ITempB05;}
	SET_MASK(PM1STS, SCIEVT);

	#if	ACPIFlagCtrlQevent
	if (sci_mode == SCIMode_Force)
	{
		if (IsFlag0(POWER_FLAG, ACPI_OS)) {return;}
	}
	#endif

	ACPI_Gen_Int(QeventSCI);
}


void PendingECQEvent(BYTE sci_number)
{
	#if	ACPI_QEventPending
	BYTE index;
	index = PD_SCI_Event_In_Index;				// Get the input index
	PD_SCI_Event_Buffer[index] = sci_number;	// Put the SCI number in the buffer
	index++;									// Increment the index
	if (index >= PENDING_BUFFER) {index = 0;}
	// If the buffer is not full, update the input index.
	if (index != PD_SCI_Event_Out_Index) {PD_SCI_Event_In_Index = index;}
	#endif
}


void Get_PendingECQEvent(void)
{
	#if	ACPI_QEventPending
	BYTE sci_number;
	if (IsFlag1(POWER_FLAG, ACPI_OS))
	{
		// fox
		if (SCI_Event_Out_Index != SCI_Event_In_Index)
		{
			ACPI_Gen_Int(QeventSCI);	// Resend sci pulse
			return;
		}
		// fox
		if (PD_SCI_Event_Out_Index != PD_SCI_Event_In_Index)
		{
			sci_number = PD_SCI_Event_Buffer[PD_SCI_Event_Out_Index];
			PD_SCI_Event_Out_Index++;
			if (PD_SCI_Event_Out_Index >= PENDING_BUFFER) {PD_SCI_Event_Out_Index = 0;}
			ECQEvent(sci_number, SCIMode_Pending);
			return;	// fox
		}
	}
	// fox
	if (IsFlag1(SYS_MISC2, postpone_SMI)) {SMI_Gen_Int();}	// Resend SMI
	// fox
	#endif
}


//------------------------------------------------------------------------------
// Read EC RAM space
//------------------------------------------------------------------------------
BYTE Hook_ReadMapECSpace(BYTE MapIndex)
{
	Tmp_XPntr = (0x0300 | MapIndex);
	return (*Tmp_XPntr);
}


//------------------------------------------------------------------------------
// Write EC RAM space
//------------------------------------------------------------------------------
void Hook_WriteMapECSpace(BYTE MapIndex, BYTE data1)
{
	Tmp_XPntr = (0x0300 | MapIndex);
	*Tmp_XPntr = data1;
	#if	SUPPORT_OEM_APP	// leox_20111014
	OemAppHookPort66Cmd81();
	#endif
}


#if	ACPI_Smbus
//------------------------------------------------------------------------------
// Process OS SMBus command
//------------------------------------------------------------------------------
// ACPI response to write to SMBus protocol register
const FUNCT_PTR_B_V code vOSSMbusComd_table[16] =
{	// The low nibble of SMB_PRTC
	DummyFunction,		// 0x00:
	DummyFunction,		// 0x01:
	DummyFunction,		// 0x02: Write Quick Command
	DummyFunction,		// 0x03: Read Quick Command
	DummyFunction,		// 0x04: Send Byte
	DummyFunction,		// 0x05: Receive Byte
	vOSSMbusWBYTE,		// 0x06: Write Byte
	vOSSMbusRByte,		// 0x07: Read Byte
	vOSSMbusWWord,		// 0x08: Write Word
	vOSSMbusRWord,		// 0x09: Read Word
	vOSSMbusWBlcok,		// 0x0A: Write Block
	vOSSMbusRBlcok,		// 0x0B: Read Block
	DummyFunction,		// 0x0C: Process Call
	DummyFunction,		// 0x0D: Write Block-Read Block Process Call
	DummyFunction,		// 0x0E:
	DummyFunction,		// 0x0F:
};


#if	1	// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42 ++
void OSSMbusComd_DOS(void)
{
	if ((SMB_PRTC != 0x00) && (SMB_ADDR != 0x14))
	{
		SMB_STS = 0x00;
		SMB_PRTC &= 0x0F;
		if ((vOSSMbusComd_table[SMB_PRTC])())
		{
			SMB_STS = 0x80;
		}
		else
		{
			SMB_STS = 0x1A;
		}
		SMB_PRTC = 0x00;
		if (IS_MASK_SET(POWER_FLAG, ACPI_OS))
		{
			ECQEvent(PowerManagement, SCIMode_SMBus);
		}
		else
		{
			ECQEvent(PowerManagement, SCIMode_Force);
		}
	}
}


void OSSMbusComd_ACPI(void)
{
	if ((SMB_PRTC != 0x00) && (SMB_ADDR != 0x14))
	{
		SMB_STS = 0x00;
		SMB_PRTC &= 0x0F;
		if ((vOSSMbusComd_table[SMB_PRTC])())
		{
			SMB_STS = 0x80;
		}
		else
		{
			SMB_STS = 0x1A;
		}
		SMB_PRTC = 0x00;
	}
}


#else	// (original)
void OSSMbusComd(void)
{
	//if ((SMB_PRTC != 0x00) && (SMB_ADDR == 0x16))
	if ((SMB_PRTC != 0x00) && (SMB_ADDR != 0x14))
	{
		SMB_STS = 0x00;
		SMB_PRTC &= 0x0F;
		if ((vOSSMbusComd_table[SMB_PRTC])())
		{
			SMB_STS = 0x80;
		}
		else
		{
			SMB_STS = 0x1A;
		}
		SMB_PRTC = 0x00;

		if (IsFlag1(POWER_FLAG, ACPI_OS))
		{
			ECQEvent(PowerManagement, SCIMode_SMBus);	// leox_20111017
		}
		else
		{
			ECQEvent(PowerManagement, SCIMode_Force);
		}
	}
}
#endif	// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42 --


BYTE vOSSMbusWBlcok(void)
{
	// Ken_20111028 fix Coding bug about vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine ++
	//return (bRSMBusBlock(SMbusCh1, SMbusRBK, SMB_ADDR, SMB_CMD, &SMB_DATA));
	//return (bRSMBusBlock(SMbusCh1, SMbusWBK, SMB_ADDR, SMB_CMD, &SMB_DATA));
	return (bWSMBusBlock(SMbusCh1, SMbusWBK, SMB_ADDR, SMB_CMD, &SMB_DATA, SMB_BCNT, SMBus_NoPEC));	// leox_20120711 Add for SMBus block write
	// Ken_20111028 fix Coding bug about vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine --
}


BYTE vOSSMbusRBlcok(void)
{
	return (bRSMBusBlock(SMbusCh1, SMbusRBK, SMB_ADDR, SMB_CMD, &SMB_DATA));
}


BYTE vOSSMbusRByte(void)
{
	return (bRWSMBus(SMbusCh1, SMbusRB, SMB_ADDR, SMB_CMD, &SMB_DATA, SMBus_NoPEC));
}


BYTE vOSSMbusRWord(void)
{
	return (bRWSMBus(SMbusCh1, SMbusRW, SMB_ADDR, SMB_CMD, &SMB_DATA, SMBus_NoPEC));
}


BYTE vOSSMbusWBYTE(void)
{
	return (bRWSMBus(SMbusCh1, SMbusWB, SMB_ADDR, SMB_CMD, &SMB_DATA, SMBus_NoPEC));
}


BYTE vOSSMbusWWord(void)
{
	return (bRWSMBus(SMbusCh1, SMbusWW, SMB_ADDR, SMB_CMD, &SMB_DATA, SMBus_NoPEC));
}


BYTE vOSSMbusWBlcok_OEM(void)
{
	if (IsFlag1(ABAT_MISC2, Shipmode))
		return (0);
	// Ken_20111028 fix Coding bug about vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine ++
	//return (bRSMBusBlock(SMbusCh1, SMbusRBK, SMB_ADDR, SMB_CMD, &SMB_DATA_OEM));
	// David_20120106 modify for vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine
	//return (bRSMBusBlock(SMbusCh1, SMbusWBK, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM));
	return (bWSMBusBlock(SMbusCh1, SMbusWBK, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM, SMB_BCNT_OEM, SMBus_NoPEC));	// leox_20120711 Add for SMBus block write
	// David_20120106 modify for vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine
	// Ken_20111028 fix Coding bug about vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine --
}


BYTE vOSSMbusRBlcok_OEM(void)
{
	// David_20120106 modify for vOSSMbusRBlcok/vOSSMbusRBlcok_OEM routine
	if (IsFlag1(ABAT_MISC2, Shipmode))
		return (0);
	return (bRSMBusBlock(SMbusCh1, SMbusRBK, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM));
	// David_20120106 modify for vOSSMbusRBlcok/vOSSMbusRBlcok_OEM routine
}


BYTE vOSSMbusRByte_OEM(void)
{
	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
	if (IsFlag1(ABAT_MISC2, Shipmode))
		return (0);
	return (bRWSMBus(SMbusCh1, SMbusRB, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM, SMBus_NoPEC));
	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
}


BYTE vOSSMbusRWord_OEM(void)
{
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (IsFlag1(ABAT_MISC2, Shipmode))
		return (0);
	return (bRWSMBus(SMbusCh1, SMbusRW, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM, SMBus_NoPEC));
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
}


BYTE vOSSMbusWBYTE_OEM(void)
{
	// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
	if (IsFlag1(ABAT_MISC2, Shipmode))
		return (0);
	return (bRWSMBus(SMbusCh1, SMbusWB, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM, SMBus_NoPEC));
	// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
}


BYTE vOSSMbusWWord_OEM(void)
{
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
	if (IsFlag1(ABAT_MISC2, Shipmode))
		return (0);
	#ifdef SUPPORT_BQ24780S		// leox20150907
	Hook_vOSSMbusWWord_OEM();	// leox20150623 Add for wirte charger IC
	#endif
	return (bRWSMBus(SMbusCh1, SMbusWW, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM, SMBus_NoPEC));
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
}

BYTE vOSSMbusRBlcok_OEM_SMB2(void)
{
	return (bRSMBusBlock(SMbusCh2, SMbusRBK, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM));
}

BYTE vOSSMbusRByte_OEM_SMB2(void)
{
	return (bRWSMBus(SMbusCh2, SMbusRB, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM, SMBus_NoPEC));
}

BYTE vOSSMbusWBYTE_OEM_SMB2(void)
{
	return (bRWSMBus(SMbusCh2, SMbusWB, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM, SMBus_NoPEC));
}

BYTE vOSSMbusRBlcok_OEM_SMB3(void)
{
	// David_20120106 modify for vOSSMbusRBlcok/vOSSMbusRBlcok_OEM_SMB3 routine
	return (bRSMBusBlock(SMbusCh3, SMbusRBK, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM));
	// David_20120106 modify for vOSSMbusRBlcok/vOSSMbusRBlcok_OEM_SMB3 routine
}


//David add for thermal sensor use SMB channel 2 2011/09/15
BYTE vOSSMbusRByte_OEM_SMB3(void)
{
	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
	return (bRWSMBus(SMbusCh3, SMbusRB, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM, SMBus_NoPEC));	// leox_20111024 SMbusRW -> SMbusRB
	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
}


BYTE vOSSMbusWBYTE_OEM_SMB3(void)
{
	// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM_SMB3 routine
	return (bRWSMBus(SMbusCh3, SMbusWB, SMB_ADDR_OEM, SMB_CMD_OEM, &SMB_DATA_OEM, SMBus_NoPEC));
	// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM_SMB3 routine
}
//David add for thermal sensor use SMB channel 2 2011/09/15
#endif	//ACPI_Smbus


void DummyFunction(void)
{

}


//==============================================================================
// Handle ACPI command 0x
//==============================================================================
void ACPI_Cmd_00(void)
{

}


void ACPI_Cmd_01(void)
{

}


void ACPI_Cmd_02(void)
{

}


void ACPI_Cmd_03(void)
{

}


void ACPI_Cmd_04(void)
{

}


void ACPI_Cmd_05(void)
{

}


void ACPI_Cmd_06(void)
{

}


void ACPI_Cmd_07(void)
{

}


void ACPI_Cmd_08(void)
{

}


void ACPI_Cmd_09(void)
{

}


void ACPI_Cmd_0A(void)
{

}


void ACPI_Cmd_0B(void)
{

}


void ACPI_Cmd_0C(void)
{

}


void ACPI_Cmd_0D(void)
{

}


void ACPI_Cmd_0E(void)
{

}


void ACPI_Cmd_0F(void)
{

}


//==============================================================================
// Handle ACPI command 1x
//==============================================================================
void ACPI_Cmd_10(void)
{

}


void ACPI_Cmd_11(void)
{

}


void ACPI_Cmd_12(void)
{

}


void ACPI_Cmd_13(void)
{

}


void ACPI_Cmd_14(void)
{

}


void ACPI_Cmd_15(void)
{

}


void ACPI_Cmd_16(void)
{

}


void ACPI_Cmd_17(void)
{

}


void ACPI_Cmd_18(void)
{

}


void ACPI_Cmd_19(void)
{

}


void ACPI_Cmd_1A(void)
{

}


void ACPI_Cmd_1B(void)
{

}


void ACPI_Cmd_1C(void)
{

}


void ACPI_Cmd_1D(void)
{

}


void ACPI_Cmd_1E(void)
{

}


void ACPI_Cmd_1F(void)
{

}


//==============================================================================
// Handle ACPI command 2x
//==============================================================================
void ACPI_Cmd_20(void)
{

}


void ACPI_Cmd_21(void)
{

}


void ACPI_Cmd_22(void)
{

}


void ACPI_Cmd_23(void)
{

}


void ACPI_Cmd_24(void)
{

}


void ACPI_Cmd_25(void)
{

}


void ACPI_Cmd_26(void)
{

}


void ACPI_Cmd_27(void)
{

}


void ACPI_Cmd_28(void)
{

}


void ACPI_Cmd_29(void)
{

}


void ACPI_Cmd_2A(void)
{

}


void ACPI_Cmd_2B(void)
{

}


void ACPI_Cmd_2C(void)
{

}


void ACPI_Cmd_2D(void)
{

}


void ACPI_Cmd_2E(void)
{

}


void ACPI_Cmd_2F(void)
{

}


//==============================================================================
// Handle ACPI command 3x
//==============================================================================
void ACPI_Cmd_30(void)
{

}


void ACPI_Cmd_31(void)
{

}


void ACPI_Cmd_32(void)
{

}


void ACPI_Cmd_33(void)
{

}


void ACPI_Cmd_34(void)
{

}


void ACPI_Cmd_35(void)
{

}


void ACPI_Cmd_36(void)
{

}


void ACPI_Cmd_37(void)
{

}


void ACPI_Cmd_38(void)
{

}


void ACPI_Cmd_39(void)
{

}


void ACPI_Cmd_3A(void)
{

}


void ACPI_Cmd_3B(void)
{

}


void ACPI_Cmd_3C(void)
{

}


void ACPI_Cmd_3D(void)
{

}


void ACPI_Cmd_3E(void)
{

}


void ACPI_Cmd_3F(void)
{

}


//==============================================================================
// Handle ACPI command 4x
//==============================================================================
void ACPI_Cmd_40(void)
{

}


void ACPI_Cmd_41(void)
{

}


void ACPI_Cmd_42(void)
{

}


void ACPI_Cmd_43(void)
{

}


void ACPI_Cmd_44(void)
{

}


void ACPI_Cmd_45(void)
{

}


void ACPI_Cmd_46(void)
{

}


void ACPI_Cmd_47(void)
{

}


void ACPI_Cmd_48(void)
{

}


void ACPI_Cmd_49(void)
{

}


void ACPI_Cmd_4A(void)
{

}


void ACPI_Cmd_4B(void)
{

}


void ACPI_Cmd_4C(void)
{

}


void ACPI_Cmd_4D(void)
{

}


void ACPI_Cmd_4E(void)
{

}


void ACPI_Cmd_4F(void)
{

}


//==============================================================================
// Handle ACPI command 5x
//==============================================================================
void ACPI_Cmd_50(void)
{

}


void ACPI_Cmd_51(void)
{

}


void ACPI_Cmd_52(void)
{

}


void ACPI_Cmd_53(void)
{

}


void ACPI_Cmd_54(void)
{

}


void ACPI_Cmd_55(void)
{

}


void ACPI_Cmd_56(void)
{

}


void ACPI_Cmd_57(void)
{

}


void ACPI_Cmd_58(void)
{

}


void ACPI_Cmd_59(void)
{

}


void ACPI_Cmd_5A(void)
{

}


void ACPI_Cmd_5B(void)
{

}


void ACPI_Cmd_5C(void)
{

}


void ACPI_Cmd_5D(void)
{

}


void ACPI_Cmd_5E(void)
{

}


void ACPI_Cmd_5F(void)
{

}


//==============================================================================
// Handle ACPI command 6x
//==============================================================================
void ACPI_Cmd_60(void)
{

}


void ACPI_Cmd_61(void)
{

}


void ACPI_Cmd_62(void)
{

}


void ACPI_Cmd_63(void)
{

}


void ACPI_Cmd_64(void)
{

}


void ACPI_Cmd_65(void)
{

}


void ACPI_Cmd_66(void)
{

}


void ACPI_Cmd_67(void)
{

}


void ACPI_Cmd_68(void)
{

}


void ACPI_Cmd_69(void)
{

}


void ACPI_Cmd_6A(void)
{

}


void ACPI_Cmd_6B(void)
{

}


void ACPI_Cmd_6C(void)
{

}


void ACPI_Cmd_6D(void)
{

}


void ACPI_Cmd_6E(void)
{

}


void ACPI_Cmd_6F(void)
{

}


//==============================================================================
// Handle ACPI command 7x
//==============================================================================
void ACPI_Cmd_70(void)
{

}


void ACPI_Cmd_71(void)
{

}


void ACPI_Cmd_72(void)
{

}


void ACPI_Cmd_73(void)
{

}


void ACPI_Cmd_74(void)
{

}


void ACPI_Cmd_75(void)
{

}


void ACPI_Cmd_76(void)
{

}


void ACPI_Cmd_77(void)
{

}


void ACPI_Cmd_78(void)
{

}


void ACPI_Cmd_79(void)
{

}


void ACPI_Cmd_7A(void)
{

}


void ACPI_Cmd_7B(void)
{

}


void ACPI_Cmd_7C(void)
{

}


void ACPI_Cmd_7D(void)
{

}


void ACPI_Cmd_7E(void)
{

}


void ACPI_Cmd_7F(void)
{

}


//==============================================================================
// Handle ACPI command 8x
//==============================================================================
void ACPI_Cmd_85(void)
{
	PM1Step = 0x01;
}


void ACPI_Cmd_86(void)
{
	Hook_Port66Cmd86();	// leox_20120611
}


void ACPI_Cmd_87(void)
{
	Hook_Port66Cmd87();	// leox_20120611
}


void ACPI_Cmd_88(void)
{
	Read591register();
}


void ACPI_Cmd_89(void)
{
	Write591register();
}


//------------------------------------------------------------------------------
// Handle ACPI command 8A - Set fan speed
//------------------------------------------------------------------------------
void ACPI_Cmd_8A(void)
{
	PM1Step = 0x01;
}


void ACPI_Cmd_8B(void)
{
	#ifdef	OEM_IPOD
	DisableUSBCHARGPORTFun();
	#endif	//OEM_IPOD
	SetFlag(SYS_MISC2, InRestart);
	SetFlag(POWER_FLAG, wait_HWPG);
	ClrFlag(POWER_FLAG, ACPI_OS);
	ClrFlag(KETBOARD_STAT_CTRL, OS_Type);
	HWPG_TIMER = T_SWPG;
	ECPWROK_off();
	ClrFlag(ToSBsignals00, ECPWROK);
}


void ACPI_Cmd_8C(void)
{
	if ((AC_IN_read()) || (IsFlag0(MISC_FLAG, F_CriLow)))
	{
		SetFlag(SYS_MISC2, InRestart);
		#ifdef	OEM_IPOD
		DisableUSBCHARGPORTFun();
		#endif	//OEM_IPOD
		ShutDnCause = 0xF5;	// leox_20121123 0xF6 -> 0xF5
		S0S3toS4S5();
		DelayXms(100);	// Delay 100ms
		SetFlag(POWER_FLAG, wait_PSW_off);
		PSWOnPower();
	}
	else
	{
		DoTurnOffPower_668C();
	}
}


void ACPI_Cmd_8D(void)
{
	ShutDnCause = 0x09;
	ForceOffPower();
}


void ACPI_Cmd_8E(void)
{
	PM1Step = 0x01;
}


void ACPI_Cmd_8F(void)
{
	ExitDebugLED();	// leox_20120524
}


//==============================================================================
// Handle ACPI command 9x
//==============================================================================
void ACPI_Cmd_90(void)
{

}


void ACPI_Cmd_91(void)
{

}


void ACPI_Cmd_94(void)
{

}


void ACPI_Cmd_95(void)
{

}


void ACPI_Cmd_96(void)
{

}


void ACPI_Cmd_97(void)
{

}


void ACPI_Cmd_98(void)
{

}


void ACPI_Cmd_99(void)
{

}


//------------------------------------------------------------------------------
// Handle ACPI command 9A - Set Fan2 speed
//------------------------------------------------------------------------------
void ACPI_Cmd_9A(void)
{
	PM1Step = 0x01;	// leox_20120322 Support_FAN2
}


void ACPI_Cmd_9B(void)
{
	#ifdef	OEM_BATTERY
	DoWriteBattFirstUSEDate();
	#endif	//OEM_BATTERY
}


void ACPI_Cmd_9C(void)
{
	#ifdef	OEM_BATTERY
	SeveReadBattFirstUSEDate();
	#endif	//OEM_BATTERY
}


void ACPI_Cmd_9D(void)
{
	PM1Step = 0x01;
}


void ACPI_Cmd_9E(void)
{
	PM1Step = 0x01;	// leox20150721 LED function
}


void ACPI_Cmd_9F(void)
{

}


//==============================================================================
// Handle ACPI command Ax
//==============================================================================
void ACPI_Cmd_A0(void)
{
	#ifdef	OEM_IPOD
	USBDischargingS4S5ToS0Init();
	#endif	//OEM_IPOD
}


void ACPI_Cmd_A1(void)
{

}


void ACPI_Cmd_A2(void)
{

}


void ACPI_Cmd_A3(void)
{

}


void ACPI_Cmd_A4(void)
{

}


void ACPI_Cmd_A5(void)
{
	SetFlag(KETBOARD_STAT_CTRL, SKIPMATRIX);
}


void ACPI_Cmd_A6(void)
{
	ClrFlag(KETBOARD_STAT_CTRL, SKIPMATRIX);
}


void ACPI_Cmd_A7(void)
{
	SetFlag(Device_STAT_CTRL1, VGA_Status);
}


void ACPI_Cmd_A8(void)
{
	ClrFlag(Device_STAT_CTRL1, VGA_Status);
}


void ACPI_Cmd_A9(void)
{
	SetFlag(POWER_FLAG2, No_Dimm);
	ClrFlag(CMOSReset_Status, WaitPCH);
}


void ACPI_Cmd_AA(void)
{
	ClrFlag(POWER_FLAG2, No_Dimm);
	ClrFlag(CMOSReset_Status, WaitPCH);
}


void ACPI_Cmd_AB(void)
{
	SetFlag(POWER_FLAG2, Quick_S3);
}


void ACPI_Cmd_AC(void)
{
	ClrFlag(POWER_FLAG2, Quick_S3);
}


void ACPI_Cmd_AD(void)
{

}


void ACPI_Cmd_AE(void)
{

}


void ACPI_Cmd_AF(void)
{

}


//==============================================================================
// Handle ACPI command Bx
//==============================================================================
void ACPI_Cmd_B0(void)
{
	SetFlag(NECOverChgCurrent, ForceStopChgOCC);
}


void ACPI_Cmd_B1(void)
{
	ClrFlag(NECOverChgCurrent, ForceStopChgOCC);
}


void ACPI_Cmd_B2(void)
{

}


void ACPI_Cmd_B3(void)
{

}


void ACPI_Cmd_B4(void)
{
	RamProgram(0x66);
}


void ACPI_Cmd_B5(void)
{

}


void ACPI_Cmd_B6(void)
{

}


void ACPI_Cmd_B7(void)
{

}


void ACPI_Cmd_B8(void)
{

}


void ACPI_Cmd_B9(void)
{

}


void ACPI_Cmd_BA(void)
{

}


void ACPI_Cmd_BB(void)
{

}


void ACPI_Cmd_BC(void)
{

}


void ACPI_Cmd_BD(void)
{

}


void ACPI_Cmd_BE(void)
{

}


void ACPI_Cmd_BF(void)
{

}


//==============================================================================
// Handle ACPI command Cx
//==============================================================================
//------------------------------------------------------------------------------
// OEM function for reading EC external RAM
//------------------------------------------------------------------------------
void ACPI_Cmd_C0(void)
{
	PM1Step = 0x02;
}


//------------------------------------------------------------------------------
// OEM function for writing EC external RAM
//------------------------------------------------------------------------------
void ACPI_Cmd_C1(void)
{
	PM1Step = 0x03;
}


//------------------------------------------------------------------------------
// Read EC ROM
//------------------------------------------------------------------------------
void ACPI_Cmd_C2(void)
{
	PM1Step = 3;	// leox_20130717 For read EC ROM
}


void ACPI_Cmd_C3(void)
{

}


void ACPI_Cmd_C4(void)
{

}


void ACPI_Cmd_C5(void)
{

}


void ACPI_Cmd_C6(void)
{

}


void ACPI_Cmd_C7(void)
{

}


void ACPI_Cmd_C8(void)
{

}


void ACPI_Cmd_C9(void)
{

}


void ACPI_Cmd_CA(void)
{

}


void ACPI_Cmd_CB(void)
{

}


void ACPI_Cmd_CC(void)
{

}


void ACPI_Cmd_CD(void)
{

}


void ACPI_Cmd_CE(void)
{

}


void ACPI_Cmd_CF(void)
{

}


//==============================================================================
// Handle ACPI command Dx
//==============================================================================
void ACPI_Cmd_D0(void)
{
	VOLMUTE_off();
}


void ACPI_Cmd_D1(void)
{

}


void ACPI_Cmd_D2(void)
{

}


void ACPI_Cmd_D3(void)
{

}


void ACPI_Cmd_D4(void)
{

}


void ACPI_Cmd_D5(void)
{

}


void ACPI_Cmd_D6(void)
{

}


void ACPI_Cmd_D7(void)
{

}


void ACPI_Cmd_D8(void)
{
	//FanFullOn();
	//RamProgram(0x66);
}


void ACPI_Cmd_D9(void)
{

}


void ACPI_Cmd_DA(void)
{

}


void ACPI_Cmd_DB(void)
{

}


void ACPI_Cmd_DD(void)
{
	//RamProgram(0x66);
}


void ACPI_Cmd_DE(void)
{

}


void ACPI_Cmd_DF(void)
{

}


//==============================================================================
// Handle ACPI command Ex
//==============================================================================
//------------------------------------------------------------------------------
// Set power loss configuration
//------------------------------------------------------------------------------
void ACPI_Cmd_E0(void)
{
	PM1Step = 1;	// leox_20110921 OEM_POWER_LOSS
}


//------------------------------------------------------------------------------
// Set EC sleep mode
//------------------------------------------------------------------------------
void ACPI_Cmd_E1(void)
{
	PM1Step = 1;	// leox_20120203 ENTER_SLEEP_MODE_AC
}


void ACPI_Cmd_E2(void)
{

}


void ACPI_Cmd_E3(void)
{

}


void ACPI_Cmd_E4(void)
{

}


void ACPI_Cmd_E5(void)
{

}


void ACPI_Cmd_E6(void)
{

}


void ACPI_Cmd_E7(void)
{

}


void ACPI_Cmd_E8(void)
{

}


void ACPI_Cmd_E9(void)
{

}


void ACPI_Cmd_EA(void)
{

}


void ACPI_Cmd_EB(void)
{

}


void ACPI_Cmd_EC(void)
{

}


void ACPI_Cmd_ED(void)
{

}


void ACPI_Cmd_EE(void)
{

}


void ACPI_Cmd_EF(void)
{

}


//==============================================================================
// Handle ACPI data 0x
//==============================================================================
void ACPI_Cmd_00Data(void)
{

}


void ACPI_Cmd_01Data(void)
{

}


void ACPI_Cmd_02Data(void)
{

}


void ACPI_Cmd_03Data(void)
{

}


void ACPI_Cmd_04Data(void)
{

}


void ACPI_Cmd_05Data(void)
{

}


void ACPI_Cmd_06Data(void)
{

}


void ACPI_Cmd_07Data(void)
{

}


void ACPI_Cmd_08Data(void)
{

}


void ACPI_Cmd_09Data(void)
{

}


void ACPI_Cmd_0AData(void)
{

}


void ACPI_Cmd_0BData(void)
{

}


void ACPI_Cmd_0CData(void)
{

}


void ACPI_Cmd_0DData(void)
{

}


void ACPI_Cmd_0EData(void)
{

}


void ACPI_Cmd_0FData(void)
{

}


//==============================================================================
// Handle ACPI data 1x
//==============================================================================
void ACPI_Cmd_10Data(void)
{

}


void ACPI_Cmd_11Data(void)
{

}


void ACPI_Cmd_12Data(void)
{

}


void ACPI_Cmd_13Data(void)
{

}


void ACPI_Cmd_14Data(void)
{

}


void ACPI_Cmd_15Data(void)
{

}


void ACPI_Cmd_16Data(void)
{

}


void ACPI_Cmd_17Data(void)
{

}


void ACPI_Cmd_18Data(void)
{

}


void ACPI_Cmd_19Data(void)
{

}


void ACPI_Cmd_1AData(void)
{

}


void ACPI_Cmd_1BData(void)
{

}


void ACPI_Cmd_1CData(void)
{

}


void ACPI_Cmd_1DData(void)
{

}


void ACPI_Cmd_1EData(void)
{

}


void ACPI_Cmd_1FData(void)
{

}


//==============================================================================
// Handle ACPI data 2x
//==============================================================================
void ACPI_Cmd_20Data(void)
{

}


void ACPI_Cmd_21Data(void)
{

}


void ACPI_Cmd_22Data(void)
{

}


void ACPI_Cmd_23Data(void)
{

}


void ACPI_Cmd_24Data(void)
{

}


void ACPI_Cmd_25Data(void)
{

}


void ACPI_Cmd_26Data(void)
{

}


void ACPI_Cmd_27Data(void)
{

}


void ACPI_Cmd_28Data(void)
{

}


void ACPI_Cmd_29Data(void)
{

}


void ACPI_Cmd_2AData(void)
{

}


void ACPI_Cmd_2BData(void)
{

}


void ACPI_Cmd_2CData(void)
{

}


void ACPI_Cmd_2DData(void)
{

}


void ACPI_Cmd_2EData(void)
{

}


void ACPI_Cmd_2FData(void)
{

}


//==============================================================================
// Handle ACPI data 3x
//==============================================================================
void ACPI_Cmd_30Data(void)
{

}


void ACPI_Cmd_31Data(void)
{

}


void ACPI_Cmd_32Data(void)
{

}


void ACPI_Cmd_33Data(void)
{

}


void ACPI_Cmd_34Data(void)
{

}


void ACPI_Cmd_35Data(void)
{

}


void ACPI_Cmd_36Data(void)
{

}


void ACPI_Cmd_37Data(void)
{

}


void ACPI_Cmd_38Data(void)
{

}


void ACPI_Cmd_39Data(void)
{

}


void ACPI_Cmd_3AData(void)
{

}


void ACPI_Cmd_3BData(void)
{

}


void ACPI_Cmd_3CData(void)
{

}


void ACPI_Cmd_3DData(void)
{

}


void ACPI_Cmd_3EData(void)
{

}


void ACPI_Cmd_3FData(void)
{

}


//==============================================================================
// Handle ACPI data 4x
//==============================================================================
void ACPI_Cmd_40Data(void)
{

}


void ACPI_Cmd_41Data(void)
{

}


void ACPI_Cmd_42Data(void)
{

}


void ACPI_Cmd_43Data(void)
{

}


void ACPI_Cmd_44Data(void)
{

}


void ACPI_Cmd_45Data(void)
{

}


void ACPI_Cmd_46Data(void)
{

}


void ACPI_Cmd_47Data(void)
{

}


void ACPI_Cmd_48Data(void)
{

}


void ACPI_Cmd_49Data(void)
{

}


void ACPI_Cmd_4AData(void)
{

}


void ACPI_Cmd_4BData(void)
{

}


void ACPI_Cmd_4CData(void)
{

}


void ACPI_Cmd_4DData(void)
{

}


void ACPI_Cmd_4EData(void)
{

}


void ACPI_Cmd_4FData(void)
{

}


//==============================================================================
// Handle ACPI data 5x
//==============================================================================
void ACPI_Cmd_50Data(void)
{

}


void ACPI_Cmd_51Data(void)
{

}


void ACPI_Cmd_52Data(void)
{

}


void ACPI_Cmd_53Data(void)
{

}


void ACPI_Cmd_54Data(void)
{

}


void ACPI_Cmd_55Data(void)
{

}


void ACPI_Cmd_56Data(void)
{

}


void ACPI_Cmd_57Data(void)
{

}


void ACPI_Cmd_58Data(void)
{

}


void ACPI_Cmd_59Data(void)
{

}


void ACPI_Cmd_5AData(void)
{

}


void ACPI_Cmd_5BData(void)
{

}


void ACPI_Cmd_5CData(void)
{

}


void ACPI_Cmd_5DData(void)
{

}


void ACPI_Cmd_5EData(void)
{

}


void ACPI_Cmd_5FData(void)
{

}


//==============================================================================
// Handle ACPI data 6x
//==============================================================================
void ACPI_Cmd_60Data(void)
{

}


void ACPI_Cmd_61Data(void)
{

}


void ACPI_Cmd_62Data(void)
{

}


void ACPI_Cmd_63Data(void)
{

}


void ACPI_Cmd_64Data(void)
{

}


void ACPI_Cmd_65Data(void)
{

}


void ACPI_Cmd_66Data(void)
{

}


void ACPI_Cmd_67Data(void)
{

}


void ACPI_Cmd_68Data(void)
{

}


void ACPI_Cmd_69Data(void)
{

}


void ACPI_Cmd_6AData(void)
{

}


void ACPI_Cmd_6BData(void)
{

}


void ACPI_Cmd_6CData(void)
{

}


void ACPI_Cmd_6DData(void)
{

}


void ACPI_Cmd_6EData(void)
{

}


void ACPI_Cmd_6FData(void)
{

}


//==============================================================================
// Handle ACPI data 7x
//==============================================================================
void ACPI_Cmd_70Data(void)
{

}


void ACPI_Cmd_71Data(void)
{

}


void ACPI_Cmd_72Data(void)
{

}


void ACPI_Cmd_73Data(void)
{

}


void ACPI_Cmd_74Data(void)
{

}


void ACPI_Cmd_75Data(void)
{

}


void ACPI_Cmd_76Data(void)
{

}


void ACPI_Cmd_77Data(void)
{

}


void ACPI_Cmd_78Data(void)
{

}


void ACPI_Cmd_79Data(void)
{

}


void ACPI_Cmd_7AData(void)
{

}


void ACPI_Cmd_7BData(void)
{

}


void ACPI_Cmd_7CData(void)
{

}


void ACPI_Cmd_7DData(void)
{

}


void ACPI_Cmd_7EData(void)
{

}


void ACPI_Cmd_7FData(void)
{

}


//==============================================================================
// Handle ACPI data 8x
//==============================================================================
void ACPI_Cmd_85Data(void)
{
	if (PM1Step == 1)	// Get PM1Data
	{
		if (PM1Data == 0xFF)
		{
			SetFlag(POWER_FLAG2, DisQevent);		// Disable Q event
		}
		else
		{
			if (PM1Data == 0xFE)
			{
				ClrFlag(POWER_FLAG2, DisQevent);	// Enable Q event
			}
			else
			{
				Q09CNT = PM1Data;
			}
		}
	}
}


void ACPI_Cmd_86Data(void)
{

}


void ACPI_Cmd_87Data(void)
{

}


void ACPI_Cmd_88Data(void)
{

}


void ACPI_Cmd_89Data(void)
{

}


//------------------------------------------------------------------------------
// Handle ACPI data 8A - Set fan speed
//------------------------------------------------------------------------------
void ACPI_Cmd_8AData(void)
{
	if (PM1Step == 1)
	{
		if ((IsFlag1(MISC_FLAG, FAN_TEST))) {FAN_LIMIT = PM1Data;}
	}
}


void ACPI_Cmd_8BData(void)
{

}


void ACPI_Cmd_8CData(void)
{

}


void ACPI_Cmd_8DData(void)
{

}


void ACPI_Cmd_8EData(void)
{
	if (PM1Step == 1)
	{
		DebugLED(PM1Data);	// leox_20120406
	}
}


void ACPI_Cmd_8FData(void)
{

}


//==============================================================================
// Handle ACPI data 9x
//==============================================================================
void ACPI_Cmd_90Data(void)
{

}


void ACPI_Cmd_91Data(void)
{

}


void ACPI_Cmd_94Data(void)
{

}


void ACPI_Cmd_95Data(void)
{

}


void ACPI_Cmd_96Data(void)
{

}


void ACPI_Cmd_97Data(void)
{

}


void ACPI_Cmd_98Data(void)
{

}


void ACPI_Cmd_99Data(void)
{

}


//------------------------------------------------------------------------------
// Handle ACPI data 9A - Set Fan2 speed
//------------------------------------------------------------------------------
void ACPI_Cmd_9AData(void)
{
	if (PM1Step == 1)	// leox_20120322 Support_FAN2
	{
		if ((IsFlag1(MISC_FLAG, FAN_TEST))) {FAN_LIMIT2 = PM1Data;}
	}
}


void ACPI_Cmd_9BData(void)
{

}


void ACPI_Cmd_9CData(void)
{

}


void ACPI_Cmd_9DData(void)
{
	if (PM1Step == 1)
	{
		#ifdef	OEM_S4S5WDG
		WatchDOGS4S5_CNT = PM1Data;
		#endif	//OEM_S4S5WDG
	}
}


void ACPI_Cmd_9EData(void)
{
	if (PM1Step == 1)
	{
		DebugLED2(PM1Data);	// leox20150721 LED function
	}
}


void ACPI_Cmd_9FData(void)
{

}


//==============================================================================
// Handle ACPI data Ax
//==============================================================================
void ACPI_Cmd_A0Data(void)
{

}


void ACPI_Cmd_A1Data(void)
{

}


void ACPI_Cmd_A2Data(void)
{

}


void ACPI_Cmd_A3Data(void)
{

}


void ACPI_Cmd_A4Data(void)
{

}


void ACPI_Cmd_A5Data(void)
{

}


void ACPI_Cmd_A6Data(void)
{

}


void ACPI_Cmd_A7Data(void)
{

}


void ACPI_Cmd_A8Data(void)
{

}


void ACPI_Cmd_A9Data(void)
{

}


void ACPI_Cmd_AAData(void)
{

}


void ACPI_Cmd_ABData(void)
{

}


void ACPI_Cmd_ACData(void)
{

}


void ACPI_Cmd_ADData(void)
{

}


void ACPI_Cmd_AEData(void)
{

}


void ACPI_Cmd_AFData(void)
{

}


//==============================================================================
// Handle ACPI data Bx
//==============================================================================
void ACPI_Cmd_B0Data(void)
{

}


void ACPI_Cmd_B1Data(void)
{

}


void ACPI_Cmd_B2Data(void)
{

}


void ACPI_Cmd_B3Data(void)
{

}


void ACPI_Cmd_B4Data(void)
{

}


void ACPI_Cmd_B5Data(void)
{

}


void ACPI_Cmd_B6Data(void)
{

}


void ACPI_Cmd_B7Data(void)
{

}


void ACPI_Cmd_B8Data(void)
{

}


void ACPI_Cmd_B9Data(void)
{

}


void ACPI_Cmd_BAData(void)
{

}


void ACPI_Cmd_BBData(void)
{

}


void ACPI_Cmd_BCData(void)
{

}


void ACPI_Cmd_BDData(void)
{

}


void ACPI_Cmd_BEData(void)
{

}


void ACPI_Cmd_BFData(void)
{

}


//==============================================================================
// Handle ACPI data Cx
//==============================================================================
void ACPI_Cmd_C0Data(void)
{
	switch (PM1Step)
	{
	case 2:
		PM1Data1 = PM1Data & 0xF0;
		if (PM1Data1 == 0xF0)
		{
			PM1Data1 = PM1Data & 0x0F;
		}
		else
		{
			PM1Data1 = PM1Data;		// Address high byte
		}
		break;

	case 1:
		Read_External_Ram();
		break;

	default:
		break;
	}
}


void ACPI_Cmd_C1Data(void)
{
	if (PM1Step == 0x03)
	{
		PM1Data2 = PM1Data & 0xF0;
		if (PM1Data2 == 0xF0)
		{
			PM1Data2 = PM1Data & 0x0F;
		}
		else
		{
			PM1Data2 = PM1Data;		// Address high byte
		}
	}
	else if (PM1Step == 0x02)
	{
		PM1Data1 = PM1Data;	// Address low byte
	}
	else if (PM1Step == 0x01)
	{
		Write_External_Ram();
	}
}


//------------------------------------------------------------------------------
// Read EC ROM
//------------------------------------------------------------------------------
void ACPI_Cmd_C2Data(void)
{
	if (PM1Step == 3)			// leox_20130717 For read EC ROM
	{
		PM1Data2 = PM1Data;
	}
	else if (PM1Step == 2)
	{
		PM1Data1 = PM1Data;
	}
	else if (PM1Step == 1)
	{
		EA = 0;
		ECINDAR3 = 0x00;
		ECINDAR2 = PM1Data2;	// Block (1 block = 64 KB)
		ECINDAR1 = PM1Data1;	// Page  (1 page = 256 bytes)
		ECINDAR0 = PM1Data;		// Offset
		PM1DO = ECINDDR;
		EA = 1;
	}
}


void ACPI_Cmd_C3Data(void)
{

}


void ACPI_Cmd_C4Data(void)
{

}


void ACPI_Cmd_C5Data(void)
{

}


void ACPI_Cmd_C6Data(void)
{

}


void ACPI_Cmd_C7Data(void)
{

}


void ACPI_Cmd_C8Data(void)
{

}


void ACPI_Cmd_C9Data(void)
{

}


void ACPI_Cmd_CAData(void)
{

}


void ACPI_Cmd_CBData(void)
{

}


void ACPI_Cmd_CCData(void)
{

}


void ACPI_Cmd_CDData(void)
{

}


void ACPI_Cmd_CEData(void)
{

}


void ACPI_Cmd_CFData(void)
{

}


//==============================================================================
// Handle ACPI data Dx
//==============================================================================
void ACPI_Cmd_D0Data(void)
{

}


void ACPI_Cmd_D1Data(void)
{

}


void ACPI_Cmd_D2Data(void)
{

}


void ACPI_Cmd_D3Data(void)
{

}


void ACPI_Cmd_D4Data(void)
{

}


void ACPI_Cmd_D5Data(void)
{

}


void ACPI_Cmd_D6Data(void)
{

}


void ACPI_Cmd_D7Data(void)
{

}


void ACPI_Cmd_D9Data(void)
{

}


void ACPI_Cmd_DAData(void)
{

}


void ACPI_Cmd_DBData(void)
{

}


void ACPI_Cmd_DEData(void)
{

}


void ACPI_Cmd_DFData(void)
{

}


//==============================================================================
// Handle ACPI data Ex
//==============================================================================
//------------------------------------------------------------------------------
// Set power loss configuration
//------------------------------------------------------------------------------
void ACPI_Cmd_E0Data(void)
{
	#if	OEM_POWER_LOSS	// leox_20110921
	if (PM1Step == 1)
	{
		B_PwrLoss = (B_PwrLoss & 0xFC) | (PM1Data & 0x03);
		#if SUPPORT_EFLASH_FUNC	// leox20160322
		eFlashSave(&B_PwrLoss, B_PwrLoss);
		#endif
	}
	#endif
}


//------------------------------------------------------------------------------
// Set EC sleep mode
//------------------------------------------------------------------------------
void ACPI_Cmd_E1Data(void)
{
	#if	ENTER_SLEEP_MODE_AC		// leox_20120203
	if (PM1Step == 1)
	{
		if (PM1Data == 0)
		{
			ClrFlag(B_PwrCtrl, EC_SLP_MODE_AC);
		}
		else
		{
			SetFlag(B_PwrCtrl, EC_SLP_MODE_AC);
		}
		#if SUPPORT_EFLASH_FUNC	// leox20150710 Access eFlash
		eFlashSave(&B_PwrCtrl, B_PwrCtrl);	// leox20160322
		#endif
	}
	#endif
}


void ACPI_Cmd_E2Data(void)
{

}


void ACPI_Cmd_E3Data(void)
{

}


void ACPI_Cmd_E4Data(void)
{

}


void ACPI_Cmd_E5Data(void)
{

}


void ACPI_Cmd_E6Data(void)
{

}


void ACPI_Cmd_E7Data(void)
{

}


void ACPI_Cmd_E8Data(void)
{

}


void ACPI_Cmd_E9Data(void)
{

}


void ACPI_Cmd_EAData(void)
{

}


void ACPI_Cmd_EBData(void)
{

}


void ACPI_Cmd_ECData(void)
{

}


void ACPI_Cmd_EDData(void)
{

}


void ACPI_Cmd_EEData(void)
{

}


void ACPI_Cmd_EFData(void)
{

}


//------------------------------------------------------------------------------
// Function: ECSMIEvent - Notify SMI Event and interrupt to HOST.
//
// smi_number = 1 through 255.  The Host uses this number to determine the
// cause of the SMI.
//------------------------------------------------------------------------------
void ECSMIEvent(BYTE smi_number)
{
	// fox
	if (IsFlag1(SYS_FLAG, enable_SMI))
	{
		SetFlag(SYS_MISC2, postpone_SMI);
		WrSMIevent_backup = smi_number;
		SMI_Event_Data = smi_number;
		SMI_Gen_Int();
	}
	// fox
}


//------------------------------------------------------------------------------
// Process EC selector command
//------------------------------------------------------------------------------
const FUNCT_PTR_V_V code ECSelectorComd_table[16] =
{	// The low nibble of SMB_PRTC
	DummyFunction,		// 0x00:
	DummyFunction,		// 0x01:
	vWritePCUCmd,		// 0x02:
	vReadPCUCmd,		// 0x03:
	vWritePCUCmd,		// 0x04:
	vReadPCUCmd,		// 0x05:
	vWritePCUCmd,		// 0x06:
	vReadPCUCmd,		// 0x07:
	vWritePCUCmd,		// 0x08:
	vReadPCUCmd,		// 0x09:
	vWritePCUCmd,		// 0x0A:
	vReadPCUCmd,		// 0x0B:
	vWritePCUCmd,		// 0x0C:
	vWritePCUCmd,		// 0x0D:
	DummyFunction,		// 0x0E:
	DummyFunction,		// 0x0F:
};


#if	1	// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42 ++
void vECSelectorComd_DOS(void)
{
	if ((SMB_PRTC != 0x00) && (SMB_ADDR == 0x14))
	{
		SMB_STS &= 0x00;
		SMB_PRTC &= 0x0F;
		(ECSelectorComd_table[SMB_PRTC])();
		SMB_STS |= 0x80;
		SMB_PRTC = 0x00;
		Force_Set_SCI_Cause(0x20);
	}
}


void vECSelectorComd_ACPI(void)
{
	if ((SMB_PRTC != 0x00) && (SMB_ADDR == 0x14))
	{
		SMB_STS &= 0x00;
		SMB_PRTC &= 0x0F;
		(ECSelectorComd_table[SMB_PRTC])();
		SMB_STS |= 0x80;
		SMB_PRTC = 0x00;
	}
}


#else	// (original)
void vECSelectorComd(void)
{
	if ((SMB_PRTC != 0x00) && (SMB_ADDR == 0x14))
	{
		//RamDebug(0x5A);
		SMB_STS &= 0x00;
		SMB_PRTC &= 0x0F;
		(ECSelectorComd_table[SMB_PRTC])();
		SMB_STS |= 0x80;
		SMB_PRTC = 0x00;
		Force_Set_SCI_Cause(0x20);	// @CP_24EC04WWa, for Q-event(20) only
	}
}
#endif	// Support Win battery tool (6C/68) Ken Lee 2012/3/23 PM 08:09:42 --


//------------------------------------------------------------------------------
// Process reading PCU command
//------------------------------------------------------------------------------
const FUNCT_PTR_V_V code ReadPCUComd_table[6] =
{
	DummyFunction,		// 0x00:
	vReadPCUCmd01_GSSS,	// 0x01: Get smart selector state
	DummyFunction,		// 0x02: Get selector preset data
	DummyFunction,		// 0x03:
	DummyFunction,		// 0x04: Get selector information
	DummyFunction,		// 0x05:
};


void vReadPCUCmd(void)
{
	if (SMB_CMD < 0x06) {(ReadPCUComd_table[SMB_CMD])();}
}


void vReadPCUCmd01_GSSS(void)
{
	vGetSelectorState(1);	// @CP_24EC06WWx, ITE update core code for fix albios battery auto learning
	SMB_BCNT = 0x02;
	SMB_DATA = SELECTOR_STATE1;
	SMB_DATA1 = SELECTOR_STATE2;
}


//------------------------------------------------------------------------------
// Report selector state
//		SELECTOR_STATE:
//		BIT0: First battery exist
//		BIT1: Second battery exist
//		BIT2:
//		BIT3:
//		BIT4: Charge first battery by host request
//		BIT5: Charge second battery by host request
//		BIT6: (11XX) AC adapter plugged in
//		BIT7: (00XX) AC adapter remove
//		BIT8: First battery in discharging
//		BIT9: Second battery in discharging
//		BITA: (11XX) Charging battery
//		BITB: (00XX) stop charging battery
//		BITC: First battery connected to SMBus
//		BITD: Second battery connected to SMBus
//		BITE:
//		BITF:
//------------------------------------------------------------------------------
const sGetSelState code asGetSelState[] =
{
	{BIT4,	&SELECTOR_STATE2,	abat_inUSE,		&ABAT_MISC		},
	{BIT5,	&SELECTOR_STATE2,	bbat_inUSE,		&BBAT_MISC		},
	{BIT0,	&SELECTOR_STATE2,	abat_dischg,	&ABAT_STATUS	},
	{BIT1,	&SELECTOR_STATE2,	bbat_dischg,	&BBAT_STATUS	},
//	{0x0C,	&SELECTOR_STATE2,	abat_InCharge,	&ACHARGE_STATUS},
	{BIT4,	&SELECTOR_STATE1,	force_chgA,		&ABAT_MISC		},
	{BIT5,	&SELECTOR_STATE1,	force_chgB,		&BBAT_MISC		},
	{BIT0,	&SELECTOR_STATE1,	abat_in,		&ABAT_STATUS	},
//	{0xC0,	&SELECTOR_STATE1,	adapter_in,		&POWER_FLAG1	},
	{BIT1,	&SELECTOR_STATE1,	bbat_in,		&BBAT_STATUS	},
};


void vGetSelectorState(BYTE getnow)	// @CP_24EC06WWx, ITE ADD
{
	//RamDebug(0x51);
	SELECTOR_STATE1 = 0x00;
	SELECTOR_STATE2 = 0x00;
	ITempB01 = 0x00;
	while (ITempB01 < (sizeof(asGetSelState) / 6))
	{
		if (IsFlag1(*asGetSelState[ITempB01].ConditionVar, asGetSelState[ITempB01].Conditionflag))
		{
			SetFlag(*asGetSelState[ITempB01].Var, asGetSelState[ITempB01].flag);
		}
		else
		{
			ClrFlag(*asGetSelState[ITempB01].Var, asGetSelState[ITempB01].flag);
		}
		ITempB01++;
	}
	if (IsFlag1(ACHARGE_STATUS, abat_InCharge))
	{
		SELECTOR_STATE2 ^= 0x0F;
		//RamDebug(0x52);
	}
	if (IsFlag1(POWER_STAT_CTRL, adapter_in))
	{
		SELECTOR_STATE1 ^= 0xF0;
		//RamDebug(0x53);
	}

	// fox
	if (getnow == 0x00)
	{
		if (IsFlag1(POWER_FLAG2, DisQevent)) {return;}
		if ((SELECTOR_STATE1 ^ OLD_SELSTATE1) != 0x00 || (SELECTOR_STATE2 ^ OLD_SELSTATE2) != 0x00)
		{
			SMB_ALRA = 0x14;		// For Quanta selester
			SMB_ALRD0 = SELECTOR_STATE1;
			SMB_ALRD1 = SELECTOR_STATE2;
			// @CP_24EC09WWj, solve battery auto learning hang up issue
			//if (getnow == 0x00)	// Event return 0
			SetFlag(SMB_STS, BIT6);	// For Quanta selester
			//RamDebug(0x54);
			if (IsFlag1(POWER_FLAG, ACPI_OS))
			{
				//Force_Set_SCI_Cause(0x20);	// @CP_24EC04WWa, for Q-event(20) only
				ECQEvent(0x20, SCIMode_Normal);	// fox
				//RamDebug(0x55);
			}
			OLD_SELSTATE1 = SELECTOR_STATE1;
			OLD_SELSTATE2 = SELECTOR_STATE2;
		}
	}
	// fox
	//RamDebug(0x56);
}


// @CP_24EC06WWx, ITE ADD for albios support ++
void WPCmd22_Reset(void)
{
	vRetWSelectorACK();
}


void WPCmd2E_AutoA(void)
{
	if (SMB_DATA == 0x01)
	{
		SetFlag(SEL_BAT_MISC, bat_AutoLearn);
		SetFlag(ABAT_MISC, abat_AutoLearn);
	}
	else
	{
		ClrFlag(SEL_BAT_MISC, bat_AutoLearn);
		ClrFlag(ABAT_MISC, abat_AutoLearn);
	}
	vRetWSelectorACK();
}


void WPCmd2F_AutoB(void)
{
	if (SMB_DATA == 0x01)
	{
		SetFlag(SEL_BAT_MISC, bat_AutoLearn);
		SetFlag(BBAT_MISC, bbat_AutoLearn);
	}
	else
	{
		ClrFlag(SEL_BAT_MISC, bat_AutoLearn);
		ClrFlag(BBAT_MISC, bbat_AutoLearn);
	}
	vRetWSelectorACK();
}
// @CP_24EC06WWx, ITE ADD  for albios support --


//------------------------------------------------------------------------------
// Process writing PCU command
//------------------------------------------------------------------------------
const FUNCT_PTR_V_V code WritePCUComd_table[16] =
{
	DummyFunction,		// 0x20: Enter/exit STANDBY status
	DummyFunction,		// 0x21: Turn off host power
	WPCmd22_Reset,		// 0x22: Reset host //@CP_24EC06WWx,ITE ADD
	vWritePCUCmd23_SMI,	// 0x23: Enable/disable SMI
	DummyFunction,		// 0x24: Enable/disable deep discharge
	DummyFunction,		// 0x25:
	DummyFunction,		// 0x26:
	DummyFunction,		// 0x27: Enable/disable Ni-MH slow charge
	DummyFunction,		// 0x28:
	DummyFunction,		// 0x29:
	DummyFunction,		// 0x2A: Set the threshold of S2RAM -> S2D
	DummyFunction,		// 0x2B:
	DummyFunction,		// 0x2C:
	DummyFunction,		// 0x2D:
	WPCmd2E_AutoA,		// 0x2E: Do a learning cycle of ABAT	// @CP_24EC06WWx, ITE ADD
	WPCmd2F_AutoB,		// 0x2F: Do a learning cycle of BBAT	// @CP_24EC06WWx, ITE ADD
};


void vWritePCUCmd(void)
{
	BYTE ABAT_MISCBK;	// @CP add mark (add for PCU)
	BYTE BBAT_MISCBK;

	if ((SMB_CMD >= 0x20) && (SMB_CMD <= 0x2F))
	{
		(WritePCUComd_table[(SMB_CMD - 0x20)])();
	}
	else
	{
		if (SMB_CMD == 0x01)
		{
			if ((SMB_DATA1 == 0xF0) && (SMB_DATA == 0x1F))	// Charge M battery
			{
				if ((IsFlag1(ABAT_STATUS, abat_in)) &&
					(IsFlag0(ABAT_STATUS, abat_destroy)) &&
					(IsFlag1(ACHARGE_STATUS, abat_wakeup)) &&
					(IsFlag1(POWER_STAT_CTRL, adapter_in)))
				{
					#ifdef	OEM_BATTERY
					SetFlag(ABAT_MISC, force_chgA);
					ClrFlag(BBAT_MISC, force_chgB);
					OffChargeABT();
					ACMODE();
					SetChargeNormalCurrent();
					ClrFlag(ABAT_MISC, force_discA);
					ClrFlag(BBAT_MISC, force_discB);
					TurnOffADISCHG();
					TurnOffBDISCHG();
					ClrFlag(ABAT_MISC, abat_AutoLearn);
					ClrFlag(BBAT_MISC, bbat_AutoLearn);
					ClrFlag(ABAT_MISC, abat_inUSE);
					SetFlag(ABAT_MISC, abat_inUSE);
					SetFlag(ABAT_MISC2, abat_test);
					#endif	//OEM_BATTERY
				}
			}
			if ((SMB_DATA1 == 0xF0) && (SMB_DATA == 0x2F))	// Charge A battery
			{
				if ((IsFlag1(BBAT_STATUS, bbat_in)) &&
					(IsFlag0(BBAT_STATUS, bbat_destroy)) &&
					(IsFlag1(BCHARGE_STATUS, bbat_wakeup)) &&
					(IsFlag1(POWER_STAT_CTRL, adapter_in)))
				{
					#ifdef	OEM_BATTERY
					ClrFlag(ABAT_MISC, force_chgA);
					SetFlag(BBAT_MISC, force_chgB);
					OffChargeBBT();
					ACMODE();
					SetChargeNormalCurrent();
					ClrFlag(ABAT_MISC, abat_AutoLearn);
					ClrFlag(BBAT_MISC, bbat_AutoLearn);
					ClrFlag(ABAT_MISC, force_discA);
					ClrFlag(BBAT_MISC, force_discB);
					TurnOffADISCHG();
					TurnOffBDISCHG();
					ClrFlag(BBAT_MISC, bbat_inUSE);
					SetFlag(ABAT_MISC, abat_inUSE);
					#endif	//OEM_BATTERY
				}
			}
			if ((SMB_DATA1 == 0x11) && (SMB_DATA == 0xFF))	// Discharge M battery
			{
				if ((IsFlag1(ABAT_STATUS, abat_in)) &&
					(IsFlag0(ABAT_STATUS, abat_destroy)) &&
					(IsFlag1(ACHARGE_STATUS, abat_wakeup)))
				{
					#ifdef	OEM_BATTERY
					//David_20120816 modify for battery discharge test not change to charge
					#if	0
					ClrFlag(BBAT_MISC, force_chgB);
					SetFlag(ABAT_MISC, force_chgA);
					OffChargeABT();
					ACMODE();
					SetChargeNormalCurrent();
					#endif
					//David_20120816 modify for battery discharge test not change to charge
					ClrFlag(ABAT_MISC, abat_AutoLearn);
					ClrFlag(BBAT_MISC, bbat_AutoLearn);
					ClrFlag(BBAT_MISC, force_discB);
					SetFlag(ABAT_MISC, force_discA);
					SetCHG_High();
					//David modify for charge IC ISL88732 2014/05/21
					#ifdef	SUPPORT_BQ24780S	// leox20150607
					SetChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
					#else	// (original)
					ClearChargeControlCHGIC(Charge_Enable);
					//SetChargeCurrent0mA();
					#endif
					//David modify for charge IC ISL88732 2014/05/21
					DCMODE();
					TurnOffBDISCHG();
					OffChargeABT();
					OffChargeBBT();
					ForceDischarge();
					ClrFlag(BBAT_MISC, bbat_inUSE);
					SetFlag(ABAT_MISC, abat_inUSE);
					SetFlag(ABAT_MISC2, abat_test);
					#endif	//OEM_BATTERY
				}
			}
			if ((SMB_DATA1 == 0x22) && (SMB_DATA == 0xFF))	// discharge A battery
			{
				if ((IsFlag1(BBAT_STATUS, bbat_in)) &&
					(IsFlag0(BBAT_STATUS, bbat_destroy)) &&
					(IsFlag1(BCHARGE_STATUS, bbat_wakeup)))
				{
					#ifdef	OEM_BATTERY
					//David_20120816 modify for battery discharge test not change to charge
					#if	0
					ClrFlag(BBAT_MISC, force_chgB);
					SetFlag(ABAT_MISC, force_chgA);
					OffChargeABT();
					ACMODE();
					SetChargeNormalCurrent();
					#endif
					//David_20120816 modify for battery discharge test not change to charge
					ClrFlag(ABAT_MISC, abat_AutoLearn);
					ClrFlag(BBAT_MISC, bbat_AutoLearn);
					ClrFlag(ABAT_MISC, force_discA);
					SetFlag(BBAT_MISC, force_discB);
					SetCHG_High();
					//David modify for charge IC ISL88732 2014/05/21
					#ifdef	SUPPORT_BQ24780S	// leox20150607
					SetChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
					#else	// (original)
					ClearChargeControlCHGIC(Charge_Enable);
					//SetChargeCurrent0mA();
					#endif
					//David modify for charge IC ISL88732 2014/05/21
					DCMODE();
					TurnOffADISCHG();
					OffChargeABT();
					OffChargeBBT();
					ForceDischarge();
					ClrFlag(ABAT_MISC, abat_inUSE);
					SetFlag(BBAT_MISC, bbat_inUSE);
					#endif	//OEM_BATTERY
				}
			}
			vRetWSelectorACK();
		}
		if (SMB_CMD == 0x02)
		{
			// Do something
		}
	}
}


void vWritePCUCmd23_SMI(void)
{
	// fox
	if ((SMB_DATA & 0x01) == 0)
	{
		// David_20120110 modify for fix battery charge/discharge test finish can't do SCI function
		ClrFlag(SYS_FLAG, enable_SMI);
		// David_20120110 modify for fix battery charge/discharge test finish can't do SCI function
	}
	else
	{
		// David_20120110 modify for fix battery charge/discharge test finish can't do SCI function
		SetFlag(SYS_FLAG, enable_SMI);
		// David_20120110 modify for fix battery charge/discharge test finish can't do SCI function
	}
	// fox
	vRetWSelectorACK();
}


void vRetWSelectorACK(void)
{
	SMB_DATA = 0xFA;
	SMB_BCNT = 0x01;
	SMB_PRTC = 0x00;
}


//------------------------------------------------------------------------------
// Turn off power for command 8Ch
//------------------------------------------------------------------------------
void DoTurnOffPower_668C(void)
{
	ShutDnCause = 0x8C;
	ForceOffPower();
}


//------------------------------------------------------------------------------
// Hook port 66h command 86h
//------------------------------------------------------------------------------
// leox_20120611 From ACPI_Cmd_86() of "OEM_ACPI.C"
void Hook_Port66Cmd86(void)
{
	SetFlag(KBHISR, BIT2);
	SetFlag(POWER_FLAG, ACPI_OS);
	SetFlag(POWER_FLAG2, func_LED);
	ClrFlag(KETBOARD_STAT_CTRL, SKIPMATRIX);
	ClrFlag(POWER_STAT_CTRL, enter_S4);
	ClrFlag(SMB_STS, BIT6);
//	ClrFlag(SMB2_STS, BIT6);
//	ClrFlag(SMB3_STS, BIT6);
	ClrFlag(MISC_FLAG, FAN_TEST);
	//David modify battery shutdown mode 2014/07/02
	ClrFlag(ABAT_MISC2, Shipmode);
	//David modify battery shutdown mode 2014/07/02
	ECQEvent(Q20_SCI, SCIMode_Normal);
	#if	SUPPORT_OEM_APP	// leox_20111014
	AppReady = 0xAA;
	AppGetVolume = 0xFF;
	AppGetMute = 0xFF;	// leox_20120424
	AppCommand = 0x00;	// leox_20111109
	#endif
}


//------------------------------------------------------------------------------
// Hook port 66h command 87h
//------------------------------------------------------------------------------
// leox_20120611 From ACPI_Cmd_87() of "OEM_ACPI.C"
void Hook_Port66Cmd87(void)
{
	ClrFlag(POWER_FLAG, ACPI_OS);
//	CmdE3_MuteOn();
	ClrFlag(KETBOARD_STAT_CTRL, OS_Type);
}

