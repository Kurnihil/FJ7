/*------------------------------------------------------------------------------
 * Title : OEM_PECI.H
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#ifndef	OEM_PECI_H
#define	OEM_PECI_H


//------------------------------------------------------------------------------
// PECI 3.0 Power Control Parameter
//------------------------------------------------------------------------------
#define	PowerLimit2Max			PECI_MAXPL2
#define	PowerLimit1TDP			PECI_PL1TDP
#define	PowerLimit1Tau			PECI_PL1Time
#define	ACPowerWatts			65		// Set 65w adaptor
#define	PowerLimit2Max_Value	60		// Set power limit 2 max xx watts
#define	PowerLimit1_TDP			45		// Setting initial TDP value if BIOS doesn't set it (watts)
#define	AlgorithmDelayTime		100		// 500ms (Timer base 5ms)


//------------------------------------------------------------------------------
// PECI 3.0 Domain Parameter Definition
//------------------------------------------------------------------------------
#define	PECI_Domain_0			0x00
#define	PECI_Domain_1			0x01


//------------------------------------------------------------------------------
// Index Definition Of PECI 3.0 Read/Write Pkg Command
//------------------------------------------------------------------------------
#define	PECI_Index_PTR			2		// Package temperature read
#define	PECI_Index_TTR			16		// Read the Maximum Allowed Processor Temperature
#define	PECI_Index_DRCTR		22		// DRAM rank channel temperature read
#define	PECI_Index_PPL1			26		// Power limit 1
#define	PECI_Index_PPL2			27		// Power limit 2
#define	PECI_Index_PPSU			28		// Power SKU unit
#define	PECI_Index_PPSL			29		// Package power and time limit information
#define	PECI_Index_PPSH			30		// Package power and time limit information


//------------------------------------------------------------------------------
// Address Definition Of PECI 3.0 Read MSR
//------------------------------------------------------------------------------
#define	MC0_CTL					0x400


//------------------------------------------------------------------------------
// CPU Power Control Step Via PECI
//------------------------------------------------------------------------------
#define	PECI_Step_Start			0x01
#define	PECI_Step_ReadPSKUUnit	0x02	// Read power SKU unit
#define	PECI_Step_ReadPL1		0x03	// Read power limit 1
#define	PECI_Step_WritePL1		0x04	// Write power limit 1
#define	PECI_Step_Algorithm		0x05
#define	PECI_Step_Stop			0x10
#define	PECI_Step_StopRequest	0x11


//------------------------------------------------------------------------------
// PECI Stress Tool
//------------------------------------------------------------------------------
#define	BufferLength			24		// Buffer is 24 bytes


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void PECI_ClearTCVariables(void);
extern void PECI_ClearPowerSKUUnit(void);
extern BYTE PECI_ReadPowerUnit(void);
extern BYTE PECI_WritePowerLimit1(void);
extern BYTE PECI_ReadPowerLimit1(void);
extern void PECI_ACToDCModeTurboCtrl(void);
extern void PECI_SETPL2Watts(BYTE watts);
extern void PECI_WritePowerLimit2(BYTE watts);
extern void PECI_ReadPowerLimit2(void);
extern void PECI_ReadCPUTemp(void);
extern void PECI_ReadCPUTJMAX(void);
extern void PECI_ReadDRAMTemperature(void);
extern void PECI_ReadPkgTemperature(void);
extern void PECI_ReadMSRExample(void);
extern void PECI_CheckCurrentPlatformPower(void);
extern void PECI_ACTurboControlAlgorithm(void);
extern void ChecknSetAdaptorWatts(void);
extern void PECI_PowerCtrlCenter(void);
extern void SetPECIPowerCtrlStep(BYTE step);
extern void ProcessPSTCmd(void);
extern void PECI_PSTIssueCmd(void);
extern void PECI_PSTEndCmd(void);
extern void PECI_PSTWriteBuffer(void);
extern void PECI_PSTReadBuffer(void);
extern void PECI_PSTReadErrorCount(void);
extern void PECI_PSTReadStatus(void);
extern void PECI_ClearPSTVariables(void);
extern void PECI_SendBufferData(void);


#endif	/* OEM_PECI_H */

