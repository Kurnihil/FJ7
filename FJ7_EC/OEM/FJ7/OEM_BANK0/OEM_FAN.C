/*------------------------------------------------------------------------------
 * Title : OEM_FAN.C
 * Author: Ken Lee / Leox Lin
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//------------------------------------------------------------------------------
// Internal Function Declaration
//------------------------------------------------------------------------------
void ChkECThermalPoint(void);
void ChkIfSendThermoUpDown(void);
void GetMaxExpectRPM(void);
void BackUpCPUVGARemoteLocal(void);
void ChkFanCtrlRPM(void);
void ChkFanCtrlDACPWM(void);
#if	Support_FAN2	// leox_20120322
void ChkFanCtrlRPM2(void);														// leox_20120322
void ChkFanCtrlDACPWM2(void);													// leox_20120322
#endif
WORD GetFanTblRPM(BYTE TmpCur, BYTE TmpPrv, asFanCtrl FanTbl[], BYTE TblLen, WORD RpmCur);	// leox_20111129	// leox_20120727


#ifdef	OEM_FAN
//------------------------------------------------------------------------------
// Fan control table
//------------------------------------------------------------------------------
// _CPUCHGRemoteTBL +++
#ifdef	OEM_BATTERY	// leox_20111205
const asFanCtrl code FanTblRmtChg[] =
//	asFanCtrl code FanTblTJ85RmtChg[] =
//	asFanCtrl code FanTblTJ90RmtChg[] =
//	asFanCtrl code FanTblTJ100RmtChg[] =
//	asFanCtrl code FanTblTJ105RmtChg[] =
//	asFanCtrl code FanTblDefRmtChg[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{55, RPM2500, 50, RPM2000},	// 1
	{65, RPM2750, 56, RPM2250},	// 2
	{70, RPM3000, 66, RPM3000},	// 3
};
#endif	// OEM_BATTERY
// _CPUCHGRemoteTBL ---


// _CPURemoteTBL +++
#if	Support_CPU_TYPE	// leox_20111205
const asFanCtrl code FanTblTJ85Rmt[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{50, RPM2300, 45, RPM2300},	// 1
	{57, RPM2700, 53, RPM2700},	// 2
	{64, RPM3000, 60, RPM3000},	// 3
	{70, RPM3700, 66, RPM3700},	// 4
};


const asFanCtrl code FanTblTJ90Rmt[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{50, RPM2300, 45, RPM2300},	// 1
	{58, RPM2700, 53, RPM2700},	// 2
	{66, RPM3000, 60, RPM3000},	// 3
	{73, RPM3700, 68, RPM3700},	// 4
};

// leo_20160510 Apply "FH9U-Thermal Table-Rev.01.xls" by thermal team Archer Lai
const asFanCtrl code FanTblTJ100Rmt[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{41, RPM2800, 37, RPM2800},	// 1
	{53, RPM3200, 43, RPM3200},	// 2
	{60, RPM3500, 54, RPM3500},	// 3
	{70, RPM4200, 62, RPM4200},	// 4
};

const asFanCtrl code FanTblTJ105Rmt[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{45, RPM2800, 40, RPM2800},	// 1
	{48, RPM3200, 45, RPM3200},	// 2
	{52, RPM3500, 48, RPM3500},	// 3
//	{65, RPM3800, 60, RPM3800},	// 4
	{55, RPM4100, 52, RPM4100},	// 5
};

const asFanCtrl code FanTblTJ85Rmt_Silent[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{55, RPM2300, 50, RPM2300},	// 1
	{61, RPM2700, 57, RPM2700},	// 2
	{67, RPM3000, 63, RPM3000},	// 3
	{73, RPM3700, 69, RPM3700},	// 4
};


const asFanCtrl code FanTblTJ90Rmt_Silent[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{56, RPM2300, 51, RPM2300},	// 1
	{64, RPM2700, 59, RPM2700},	// 2
	{71, RPM3000, 65, RPM3000},	// 3
	{77, RPM3700, 72, RPM3700},	// 4
};

// leo_20160617 Apply "FH9U-Thermal Table-Rev.02.xls" by thermal team Archer Lai
const asFanCtrl code FanTblTJ100Rmt_Silent[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{47, RPM2800, 43, RPM2800},	// 1
	{59, RPM3200, 49, RPM3200},	// 2
	{66, RPM3500, 60, RPM3500},	// 3
	{76, RPM4200, 68, RPM4200},	// 4
};

const asFanCtrl code FanTblTJ105Rmt_Silent[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{50, RPM2800, 45, RPM2800},	// 1
	{60, RPM3200, 55, RPM3200},	// 2
	{70, RPM3500, 65, RPM3500},	// 3
	{75, RPM3800, 70, RPM3800},	// 4
	{80, RPM4100, 75, RPM4100},	// 5
};
#endif	// Support_CPU_TYPE


const asFanCtrl code FanTblDefRmt[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{50, RPM2300, 45, RPM2300},	// 1
	{57, RPM2700, 53, RPM2700},	// 2
	{64, RPM3000, 60, RPM3000},	// 3
	{70, RPM3700, 66, RPM3700},	// 4
};

const asFanCtrl code FanTblDefRmt_Silent[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{55, RPM2300, 50, RPM2300},	// 1
	{61, RPM2700, 57, RPM2700},	// 2
	{67, RPM3000, 63, RPM3000},	// 3
	{73, RPM3700, 69, RPM3700},	// 4
};
// _CPURemoteTBL ---


// _CPUCHGLocalTBL +++
#ifdef	OEM_BATTERY		// leox_20111205
const asFanCtrl code FanTblLocChg[] =
//	asFanCtrl code FanTblTJ85LocChg[] =
//	asFanCtrl code FanTblTJ90LocChg[] =
//	asFanCtrl code FanTblTJ100LocChg[] =
//	asFanCtrl code FanTblTJ105LocChg[] =
//	asFanCtrl code FanTblDefLocChg[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{55, RPM2500, 50, RPM2000},	// 1
	{65, RPM2750, 56, RPM2250},	// 2
	{70, RPM3000, 66, RPM3000},	// 3
};
#endif	// OEM_BATTERY
// _CPUCHGLocalTBL ---


// _CPULocalTBL +++
#if	Support_CPU_TYPE	// leox_20111205
const asFanCtrl code FanTblTJ85Loc[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{55, RPM2500, 50, RPM2000},	// 1
	{65, RPM2750, 56, RPM2250},	// 2
	{70, RPM3000, 66, RPM3000},	// 3
};


const asFanCtrl code FanTblTJ90Loc[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{50, RPM2400, 30, RPM2400},	// 1
	{60, RPM3100, 52, RPM3100},	// 2
	{65, RPM3600, 62, RPM3600},	// 3
};


const asFanCtrl code FanTblTJ100Loc[] =	// leox_20120308 Refer to FanTblTJ85Loc[]
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{50, RPM2400, 30, RPM2400},	// 1
	{60, RPM3100, 52, RPM3100},	// 2
	{74, RPM3600, 62, RPM3600},	// 3
};


const asFanCtrl code FanTblTJ105Loc[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{75, RPM4100, 70, RPM4100},	// 1
};
#endif	// Support_CPU_TYPE


const asFanCtrl code FanTblDefLoc[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{60, RPM2200, 52, RPM2200},	// 1
	{70, RPM2900, 62, RPM2900},	// 2
	{80, RPM3400, 72, RPM3400},	// 3
};
// _CPULocalTBL ---


// Fan table for VGA temperature (All VGA use the same fan table)
const asFanCtrl code FanTblVGARmt[] =	// leox_20120308
//	asFanCtrl code FanTblVGARmtChg[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{55, RPM2500, 50, RPM2000},	// 1
	{65, RPM2750, 56, RPM2250},	// 2
	{70, RPM3000, 66, RPM3000},	// 3
};


const asFanCtrl code FanTblVGALoc[] =	// leox_20120308
//	asFanCtrl code FanTblVGALocChg[] =
{//  On, RPM on ,Off, RPM off	// Level
	{ 0, RPM0000,  0, RPM0000},	// 0
	{48, RPM2300, 45, RPM2300},	// 1
	{54, RPM2700, 50, RPM2700},	// 2
	{62, RPM3000, 58, RPM3000},	// 3
	{68, RPM3700, 64, RPM3700},	// 4
};


//------------------------------------------------------------------------------
// Fan control
//------------------------------------------------------------------------------
// leox_20111129 Rewrite function and implement struct sFanCtrl
void FanCtrlDACPWMRPM(void)
{
	asFanCtrl *FanTblPtr;	// leox_20111130
	BYTE FanTblLen = 0;		// leox_20111201

	// Check temperature for thermal shutdown
	ChkECThermalPoint();

	// Check CPU temperature throttling
	if (CPU_Remote_Temp != CPU_Remote_Temp_BackUp) {ChkIfSendThermoUpDown();}

	// Check CPU remote temperature for fan control	// CPU_Remote_RPM_Value
	if ((CPU_Remote_Temp != CPU_Remote_Temp_BackUp) && IsFlag1(FanControl_Mode01, EnFanCtrlCPURempte))
	{
		#ifdef	OEM_BATTERY  // leox_20111205
		if (IsFlag1(FanControl_Mode01, EnFanCtrlCPURempteCHG) && IsFlag1(ACHARGE_STATUS, abat_InCharge))
		{	// _CPUCHGRemoteTBL (_CPUCHGRemoteTBL use the same table)
			FanTblPtr = FanTblRmtChg;
			FanTblLen = GetArrayNum(FanTblRmtChg);
		}
		else
		#endif	// OEM_BATTERY
		{	// _CPURemoteTBL
			switch (FanControl_Mode00 & (CPU_TYPE_TJ85 | CPU_TYPE_TJ90 | CPU_TYPE_TJ100 | CPU_TYPE_TJ105))
			{
			#if	Support_CPU_TYPE	// leox_20111205
			case CPU_TYPE_TJ85:
				if (IsFlag1(CPU_TYPE, Silent_Mode))
				{
					FanTblPtr = FanTblTJ85Rmt_Silent;
					FanTblLen = GetArrayNum(FanTblTJ85Rmt_Silent);
				}
				else
				{
					FanTblPtr = FanTblTJ85Rmt;
					FanTblLen = GetArrayNum(FanTblTJ85Rmt);
				}
				break;

			case CPU_TYPE_TJ90:
				if (IsFlag1(CPU_TYPE, Silent_Mode))
				{
					FanTblPtr = FanTblTJ90Rmt_Silent;
					FanTblLen = GetArrayNum(FanTblTJ90Rmt_Silent);
				}
				else
				{
					FanTblPtr = FanTblTJ90Rmt;
					FanTblLen = GetArrayNum(FanTblTJ90Rmt);
				}
				break;

			case CPU_TYPE_TJ100:
				if (IsFlag1(CPU_TYPE, Silent_Mode))
				{
					FanTblPtr = FanTblTJ100Rmt_Silent;
					FanTblLen = GetArrayNum(FanTblTJ100Rmt_Silent);
				}
				else
				{
					FanTblPtr = FanTblTJ100Rmt;
					FanTblLen = GetArrayNum(FanTblTJ100Rmt);
				}
				break;

			case CPU_TYPE_TJ105:
				if (IsFlag1(CPU_TYPE, Silent_Mode))
				{
					FanTblPtr = FanTblTJ105Rmt_Silent;
					FanTblLen = GetArrayNum(FanTblTJ105Rmt_Silent);
				}
				else
				{
					FanTblPtr = FanTblTJ105Rmt;
					FanTblLen = GetArrayNum(FanTblTJ105Rmt);
				}
				break;
			#endif	// Support_CPU_TYPE

			default:
				if (IsFlag1(CPU_TYPE, Silent_Mode))
				{
					FanTblPtr = FanTblDefRmt_Silent;
					FanTblLen = GetArrayNum(FanTblDefRmt_Silent);
				}
				else
				{
					FanTblPtr = FanTblDefRmt;
					FanTblLen = GetArrayNum(FanTblDefRmt);
				}
				break;
			}
		}
		CPU_Remote_RPM_Value = GetFanTblRPM(CPU_Remote_Temp, CPU_Remote_Temp_BackUp, FanTblPtr, FanTblLen, CPU_Remote_RPM_Value);	// leox_20120727
	}

	// Check CPU local temperature for fan control	// CPU_Local_RPM_Value
	if ((CPU_Local_Temp != CPU_Local_Temp_BackUp) && IsFlag1(FanControl_Mode01, EnFanCtrlCPULocal))
	{
		#ifdef	OEM_BATTERY		// leox_20111205
		if ((IsFlag1(FanControl_Mode01, EnFanCtrlCPULocalCHG)) && (IsFlag1(ACHARGE_STATUS, abat_InCharge)))
		{	// _CPUCHGLocalTBL (_CPUCHGLocalTBL use the same table)
			FanTblPtr = FanTblLocChg;
			FanTblLen = GetArrayNum(FanTblLocChg);
		}
		else
		#endif	// OEM_BATTERY
		{	// _CPULocalTBL
			switch (FanControl_Mode00 & (CPU_TYPE_TJ85 | CPU_TYPE_TJ90 | CPU_TYPE_TJ100 | CPU_TYPE_TJ105))
			{
			#if	Support_CPU_TYPE	// leox_20111205
			case CPU_TYPE_TJ85:
				FanTblPtr = FanTblTJ85Loc;
				FanTblLen = GetArrayNum(FanTblTJ85Loc);
				break;

			case CPU_TYPE_TJ90:
				FanTblPtr = FanTblTJ90Loc;
				FanTblLen = GetArrayNum(FanTblTJ90Loc);
				break;

			case CPU_TYPE_TJ100:	// leox_20120308
				FanTblPtr = FanTblTJ100Loc;
				FanTblLen = GetArrayNum(FanTblTJ100Loc);
				break;

			case CPU_TYPE_TJ105:
				FanTblPtr = FanTblTJ105Loc;
				FanTblLen = GetArrayNum(FanTblTJ105Loc);
				break;
			#endif	// Support_CPU_TYPE

			default:
				FanTblPtr = FanTblDefLoc;
				FanTblLen = GetArrayNum(FanTblDefLoc);
				break;
			}
		}
		CPU_Local_RPM_Value = GetFanTblRPM(CPU_Local_Temp, CPU_Local_Temp_BackUp, FanTblPtr, FanTblLen, CPU_Local_RPM_Value);		// leox_20120727
	}

	// Check VGA remote temperature for fan control	// VGA_Remote_RPM_Value
	if ((VGA_Remote_Temp != VGA_Remote_Temp_BackUp) && IsFlag1(FanControl_Mode01, EnFanCtrlVGARempte))
	{
		#ifdef	OEM_BATTERY		// leox_20111205
		if ((IsFlag1(FanControl_Mode01, EnFanCtrlVGARempteCHG)) && (IsFlag1(ACHARGE_STATUS, abat_InCharge)))
		{	// _VGACHGRemoteTBL (_VGACHGRemoteTBL use the same table)
			FanTblPtr = FanTblVGARmt;
			FanTblLen = GetArrayNum(FanTblVGARmt);
		}
		else
		#endif	// OEM_BATTERY
		{	// _VGARemoteTBL (_VGARemoteTBL use the same table)
			FanTblPtr = FanTblVGARmt;
			FanTblLen = GetArrayNum(FanTblVGARmt);
		}
		VGA_Remote_RPM_Value = GetFanTblRPM(VGA_Remote_Temp, VGA_Remote_Temp_BackUp, FanTblPtr, FanTblLen, VGA_Remote_RPM_Value);	// leox_20120727
	}

	// Check VGA local temperature for fan control	// VGA_Local_RPM_Value
	if ((VGA_Local_Temp != VGA_Local_Temp_BackUp) && IsFlag1(FanControl_Mode01, EnFanCtrlVGALocal))
	{
		#ifdef	OEM_BATTERY		// leox_20111205
		if ((IsFlag1(FanControl_Mode01, EnFanCtrlVGALocalCHG)) && (IsFlag1(ACHARGE_STATUS, abat_InCharge)))
		{	// _VGACHGLocalTBL (_VGACHGLocalTBL use the same table)
			FanTblPtr = FanTblVGALoc;
			FanTblLen = GetArrayNum(FanTblVGALoc);
		}
		else
		#endif	// OEM_BATTERY
		{	// _VGALocalTBL (_VGALocalTBL use the same table)
			FanTblPtr = FanTblVGALoc;
			FanTblLen = GetArrayNum(FanTblVGALoc);
		}
		VGA_Local_RPM_Value = GetFanTblRPM(VGA_Local_Temp, VGA_Local_Temp_BackUp, FanTblPtr, FanTblLen, VGA_Local_RPM_Value);		// leox_20120727
	}

	#ifdef	FanAlwaysOnForES1
	return;
	#endif

	if (IsFlag1(MISC_FLAG, FAN_TEST))
	{	// FanTestMode
		#ifdef	FanByDAorPWM
		DAPWM_VFAN = FAN_LIMIT;
		#else
		DAPWM_VFAN = (FAN_LIMIT > CTR) ? CTR : FAN_LIMIT;
		#endif
		#if	Support_FAN2	// leox_20120322
			#ifdef	FanByDAorPWM
		DAPWM_VFAN2 = FAN_LIMIT2;
			#else
		DAPWM_VFAN2 = (FAN_LIMIT2 > CTR) ? CTR : FAN_LIMIT2;
			#endif
		#endif
	}
	else
	{	// NotFanTestMode
		GetMaxExpectRPM();
		BackUpCPUVGARemoteLocal();
	}
}


#if	Support_CPU_TYPE
const asThermPoint code ThermPntTJ85  = { 85, 105};
const asThermPoint code ThermPntTJ90  = { 90, 105};
const asThermPoint code ThermPntTJ100 = {100, 105};
const asThermPoint code ThermPntTJ105 = {105, 105};
#endif
const asThermPoint code ThermPntDef   = { 85, 105};


#if	Support_CPU_TYPE
const asThermPoint code ThermPntTJ85VGA  = { 97, 97};
const asThermPoint code ThermPntTJ90VGA  = { 97, 97};
const asThermPoint code ThermPntTJ100VGA = { 97, 97};
const asThermPoint code ThermPntTJ105VGA = { 97, 97};
#endif
const asThermPoint code ThermPntDefVGA   = { 97, 97};


//------------------------------------------------------------------------------
// Check EC thermal poiont
//------------------------------------------------------------------------------
// leox_20111130 Rewrite function
// leox_20120229 Rewrite function and implement struct sThermPoint
void ChkECThermalPoint(void)
{
	asThermPoint *Tmp;			// leox_20120229

#ifndef	PECI_Support
	if (CPUTSAr != DumTSAr)
	{
#endif
		switch (FanControl_Mode00 & (CPU_TYPE_TJ85 | CPU_TYPE_TJ90 | CPU_TYPE_TJ100 | CPU_TYPE_TJ105))
		{
		#if	Support_CPU_TYPE	// leox_20111205
		case CPU_TYPE_TJ85:
			Tmp = &ThermPntTJ85;
			break;

		case CPU_TYPE_TJ90:
			Tmp = &ThermPntTJ90;
			break;

		case CPU_TYPE_TJ100:
			Tmp = &ThermPntTJ100;
			break;

		case CPU_TYPE_TJ105:
			Tmp = &ThermPntTJ105;
			break;
		#endif	// Support_CPU_TYPE

		default:
			Tmp = &ThermPntDef;
			break;
		}
		CPU_VeryHotCnt = (CPU_Remote_Temp >= Tmp->Remote) ? (CPU_VeryHotCnt + 1) : 0;
		CPU_VeryHotCnt_Local = (CPU_Local_Temp >= Tmp->Local) ? (CPU_VeryHotCnt_Local + 1) : 0;
#ifndef	PECI_Support
	}
#endif

	if (VGATSAr != DumTSAr)
	{
		switch (FanControl_Mode00 & (CPU_TYPE_TJ85 | CPU_TYPE_TJ90 | CPU_TYPE_TJ100 | CPU_TYPE_TJ105))
		{
		#if	Support_CPU_TYPE	// leox_20111205
		case CPU_TYPE_TJ85:
			Tmp = &ThermPntTJ85VGA;
			break;

		case CPU_TYPE_TJ90:
			Tmp = &ThermPntTJ90VGA;
			break;

		case CPU_TYPE_TJ100:
			Tmp = &ThermPntTJ100VGA;
			break;

		case CPU_TYPE_TJ105:
			Tmp = &ThermPntTJ105VGA;
			break;
		#endif	// Support_CPU_TYPE

		default:
			Tmp = &ThermPntDefVGA;
			break;
		}
		VGA_VeryHotCnt = (VGA_Remote_Temp >= Tmp->Remote) ? (VGA_VeryHotCnt + 1) : 0;
		VGA_VeryHotCnt_Local = (VGA_Local_Temp >= Tmp->Local) ? (VGA_VeryHotCnt_Local + 1) : 0;
	}

	if (CPU_VeryHotCnt >= 5)
	{
		ShutDnCause = 0xF2;
	}
	else if (CPU_VeryHotCnt_Local >= 5)
	{
		ShutDnCause = 0x27;
	}
	else if (VGA_VeryHotCnt >= 5)
	{
		ShutDnCause = 0xF1;
	}
	else if (VGA_VeryHotCnt_Local >= 5)
	{
		ShutDnCause = 0x17;
	}
	else
	{	// Do nothing
		return;
	}

	ForceOffPower();
}

#ifdef	Support_Intel_CPU
#if	Support_CPU_TYPE	// leox_20120229
//										// 1    2    3    4   5   6   7   8    9   10
const asThermUpDown code ThermChgTJ85  = {75,  85,  81,  83, 74, 78, 73, 78,  81,  84};
const asThermUpDown code ThermChgTJ90  = {79,  90,  86,  88, 77, 83, 78, 83,  86,  89};
const asThermUpDown code ThermChgTJ100 = {92, 100,  95,  95, 88, 93, 88, 93,  90,  98};	// leox20150617 Apply "FH9C-Thermal_Table-Rev.01.xls" by thermal team Archer Lai	// leox20150721
const asThermUpDown code ThermChgTJ105 = {92, 105,  97,  99, 88, 99, 90, 90, 255, 103};
#endif
const asThermUpDown code ThermChgDef   = {75,  85,  81,  83, 74, 78, 73, 78,  81,  84};
// 1: Turbo on disable
// 2: Thermal up (High bond)
// 3: Thermal up (Low  bond)
// 4: PROCHOT# enable
// 5: Turbo on enable
// 6: Thermal down (High bond)
// 7: Thermal down (Low  bond)
// 8: PROCHOT# disable
// 9: Throttling point
//10: OS shutdown point
#else
//										// 1   2   3   4   5   6
const asThermUpDown code ThermChgDef   = {85, 81, 78, 73, 81, 84};
// 1: Thermal up (High bond)
// 2: Thermal up (Low  bond)
// 3: Thermal down (High bond)
// 4: Thermal down (Low  bond)
// 5: Throttling point
// 6: OS shutdown point
#endif

//------------------------------------------------------------------------------
// Check temperature up/down and send Q-Event
//------------------------------------------------------------------------------
// leox_20120229 Rewrite function and implement struct sThermUpDown
void ChkIfSendThermoUpDown(void)
{
	asThermUpDown *Tmp;		// leox_20120229

	switch (FanControl_Mode00 & (CPU_TYPE_TJ85 | CPU_TYPE_TJ90 | CPU_TYPE_TJ100 | CPU_TYPE_TJ105))
	{
	#if	Support_CPU_TYPE	// leox_20111205
	case CPU_TYPE_TJ85:
		Tmp = &ThermChgTJ85;
		break;

	case CPU_TYPE_TJ90:
		Tmp = &ThermChgTJ90;
		break;

	case CPU_TYPE_TJ100:
		Tmp = &ThermChgTJ100;
		break;

	case CPU_TYPE_TJ105:
		Tmp = &ThermChgTJ105;
		break;
	#endif	// Support_CPU_TYPE

	default:
		Tmp = &ThermChgDef;
		break;
	}

	#if	UPDATE_THERM_POINT	// leox_20120229
	ThrottlingTemp = Tmp->Throttling;
	OSShutdownTemp = Tmp->OSShutdown;
	#endif

	if (CPU_Remote_Temp > CPU_Remote_Temp_BackUp)
	{	// Thermal up
		if ((CPU_Remote_Temp <= Tmp->ThermUpH) && (CPU_Remote_Temp >= Tmp->ThermUpL))
		{
			ECQEvent(Q80_SCI, SCIMode_Normal);
		}
		#ifdef	Support_Intel_CPU
		if (CPU_Remote_Temp >= Tmp->TurboOnD)
		{
			if (IsFlag0(MISC_FLAG, Turbo_On))
			{
				SetFlag(MISC_FLAG, Turbo_On);
				//ECQEvent(Q88_SCI, SCIMode_Normal);
			}
		}
		if (CPU_Remote_Temp >= Tmp->ProcHotE)
		{
			SetFlag(FanControl_Mode00, Thrott_CPU_hot);	// leox20150731
			h_prochot_enable();
		}
		#endif
	}
	else
	{	// Thermal down
		if ((CPU_Remote_Temp <= Tmp->ThermDownH) && (CPU_Remote_Temp >= Tmp->ThermDownL))
		{
			ECQEvent(Q81_SCI, SCIMode_Normal);
		}
		#ifdef	Support_Intel_CPU
		if (CPU_Remote_Temp <= Tmp->TurboOnE)
		{
			if (IsFlag1(MISC_FLAG, Turbo_On))
			{
				ClrFlag(MISC_FLAG, Turbo_On);
				//ECQEvent(Q89_SCI, SCIMode_Normal);
			}
		}
		if (CPU_Remote_Temp <= Tmp->ProcHotD)
		{
			ClrFlag(FanControl_Mode00, Thrott_CPU_hot);	// leox20150731
//			h_prochot_disable();
			Check_PROCHOT_Disable();	// leox20150803 Check before disable PROCHOT
		}
		#endif
	}
}


//------------------------------------------------------------------------------
// Get maximal expect RPM
//------------------------------------------------------------------------------
// leox_20111130 SaveSmallerExpectRPMValue() -> GetMaxExpectRPM()
void GetMaxExpectRPM(void)
{
	// If tachometer decreased, RPM will increase, so get the smallest value.
	Expect_RPM_Value = RPM0000;
	if (CPU_Remote_RPM_Value < Expect_RPM_Value) {Expect_RPM_Value = CPU_Remote_RPM_Value;}
	if (CPU_Local_RPM_Value  < Expect_RPM_Value) {Expect_RPM_Value = CPU_Local_RPM_Value;}
	if (VGA_Remote_RPM_Value < Expect_RPM_Value) {Expect_RPM_Value = VGA_Remote_RPM_Value;}
	if (VGA_Local_RPM_Value  < Expect_RPM_Value) {Expect_RPM_Value = VGA_Local_RPM_Value;}
	#if	Support_FAN2	// leox_20120322 Depend on the fan table
	Expect_RPM_Value2 = RPM0000;
//	if (VGA_Remote_RPM_Value < Expect_RPM_Value2) {Expect_RPM_Value2 = VGA_Remote_RPM_Value;}
	Expect_RPM_Value2 = Expect_RPM_Value;	// leox_20120322 Set Fan2 speed as Fan1 for TEST
	#endif
}


//------------------------------------------------------------------------------
// Backup CPU and VGA temperature
//------------------------------------------------------------------------------
void BackUpCPUVGARemoteLocal(void)
{
	CPU_Remote_Temp_BackUp = CPU_Remote_Temp;
	VGA_Remote_Temp_BackUp = VGA_Remote_Temp;
	CPU_Local_Temp_BackUp = CPU_Local_Temp;
	VGA_Local_Temp_BackUp = VGA_Local_Temp;
}


//------------------------------------------------------------------------------
// Update fan control mode
//------------------------------------------------------------------------------
void UpDateFanControl_Mode(void)
{
	#if	CPU_TYPE_BY_TJMAX	// leox_20120229
	SetCPUTypeByTjMax();
	#endif
	if (FanControl_Mode01 != FanControl_Mode01_Bk)
	{
		FanControl_Mode01_Bk = FanControl_Mode01;
	}
	if (FanControl_Mode00 != FanControl_Mode00_Bk)
	{
		FanControl_Mode00_Bk = FanControl_Mode00;
		ClrFlag(FanControl_Mode00, InitThermalSenFinish);
	}
	CLR_MASK(FanControl_Mode00, (CPU_TYPE_TJ85 | CPU_TYPE_TJ90 | CPU_TYPE_TJ100 | CPU_TYPE_TJ105));
	SET_MASK(FanControl_Mode00, (CPU_TYPE & (CPU_TJ85 | CPU_TJ90 | CPU_TJ100 | CPU_TJ105)));
}


//------------------------------------------------------------------------------
// Check fan control
//------------------------------------------------------------------------------
void ChkFanCtrlDACPWMRPM(void)
{
	#ifdef	FanAlwaysOnForES1
	return;
	#endif

	#if	CHECK_FAN_FAULT		// David_20120613 add for fan error check	// leox_20120615
	if ((IsFlag0(MISC_FLAG, FAN_TEST)) && (Fan1_Fault_CNT == 0))
	#else	// (original)
	if (IsFlag0(MISC_FLAG, FAN_TEST))
	#endif
	{
		ChkFanCtrlRPM();
		ChkFanCtrlDACPWM();
		#if	Support_FAN2	// leox_20120322
		ChkFanCtrlRPM2();
		ChkFanCtrlDACPWM2();
		#endif
	}
}


//------------------------------------------------------------------------------
// Check fan control by RPM
//------------------------------------------------------------------------------
void ChkFanCtrlRPM(void)
{
	WORD TmpRPM;

	if (Expect_RPM_Value == RPM0000)	// leox_20110920 0xFFFF -> RPM0000
	{
		FAN_LIMIT = 0;
	}
	else
	{
		if (Fan_Rpm == 0)
		{
			#ifdef	FanByDAorPWM		// Setting Start Fan on minimum voltage 3.1V
			FAN_LIMIT = VFAN_DA_MIN;	// leox_20110920 148 -> VFAN_DA_MIN
			#else						// Setting Start Fan on minimum PWM duty 40%
			FAN_LIMIT = VFAN_PWM_MIN;	// leox_20110920 80 -> VFAN_PWM_MIN
			#endif
			DAPWM_VFAN = FAN_LIMIT;		// leox_20110920
		}
		else
		{
			// RPM increased, tachometer decreased	// leox_20111130
			TmpRPM = ((Fan_Rpm << 8) & 0xFF00) | ((Fan_Rpm >> 8) & 0x00FF);
			if (TmpRPM > Expect_RPM_Value)
			{
				if (FAN_LIMIT < 0xFF) {FAN_LIMIT++;}
			}
			else if (TmpRPM < Expect_RPM_Value)
			{
				if (FAN_LIMIT > 0x00) {FAN_LIMIT--;}
			}
		}
	}
}


//------------------------------------------------------------------------------
// Check fan control by DAC/PWM
//------------------------------------------------------------------------------
void ChkFanCtrlDACPWM(void)
{
	#ifdef	OEM_Calpella
	if (PECI_ME_HandUp_ErrorCnt == 30)
	{
		#ifdef	FanByDAorPWM
		DAPWM_VFAN = VFAN_DA_3V9;	// 3.9V fan full speed run	// leox_20110920 186 -> VFAN_DA_3V9
		#else
		DAPWM_VFAN = VFAN_PWM_35DB;	// 35db fan full speed run	// leox_20110920 166 -> VFAN_PWM_35DB
		#endif
		return;
	}
	#endif

	#ifdef	FanByDAorPWM
	// Do nothing
	#else
	if (FAN_LIMIT > CTR) {FAN_LIMIT = CTR;}
	#endif

	if (FAN_LIMIT != DAPWM_VFAN)
	{
		if (DAPWM_VFAN > FAN_LIMIT)
		{
			DAPWM_VFAN--;
		}
		else
		{
			DAPWM_VFAN++;
		}
	}
}


#if	Support_FAN2	// leox_20120322
//------------------------------------------------------------------------------
// Check Fan2 control by RPM
//------------------------------------------------------------------------------
void ChkFanCtrlRPM2(void)
{
	WORD TmpRPM;

	if (Expect_RPM_Value2 == RPM0000)
	{
		FAN_LIMIT2 = 0;
	}
	else
	{
		if (Fan_Rpm2 == 0)
		{
			#ifdef	FanByDAorPWM	// Setting Start Fan on minimum voltage 3.1V
			FAN_LIMIT2 = VFAN_DA_MIN;
			#else					// Setting Start Fan on minimum PWM duty 40%
			FAN_LIMIT2 = VFAN_PWM_MIN;
			#endif
			DAPWM_VFAN2 = FAN_LIMIT2;
		}
		else
		{	// RPM increased, tachometer decreased	// leox_20111130
			TmpRPM = ((Fan_Rpm2 << 8) & 0xFF00) | ((Fan_Rpm2 >> 8) & 0x00FF);
			if (TmpRPM > Expect_RPM_Value2)
			{
				if (FAN_LIMIT2 < 0xFF) {FAN_LIMIT2++;}
			}
			else if (TmpRPM < Expect_RPM_Value2)
			{
				if (FAN_LIMIT2 > 0x00) {FAN_LIMIT2--;}
			}
		}
	}
}


//------------------------------------------------------------------------------
// Check Fan2 control by DAC/PWM
//------------------------------------------------------------------------------
void ChkFanCtrlDACPWM2(void)
{
	#ifdef	OEM_Calpella
	if (PECI_ME_HandUp_ErrorCnt == 30)
{
		#ifdef	FanByDAorPWM
		DAPWM_VFAN2 = VFAN_DA_3V9;		// 3.9V fan full speed run
		#else
		DAPWM_VFAN2 = VFAN_PWM_35DB;	// 35db fan full speed run
		#endif
		return;
	}
	#endif

	#ifdef	FanByDAorPWM
	// Do nothing
	#else
	if (FAN_LIMIT2 > CTR) {FAN_LIMIT2 = CTR;}
	#endif

	if (FAN_LIMIT2 != DAPWM_VFAN2)
	{
		if (DAPWM_VFAN2 > FAN_LIMIT2)
		{
			DAPWM_VFAN2--;
		}
		else
		{
			DAPWM_VFAN2++;
		}
	}
}
#endif	// Support_FAN2


//------------------------------------------------------------------------------
// Load default value for fan control
//------------------------------------------------------------------------------
void LoadFanCtrlDefault(void)
{
	CPU_VeryHotCnt = 0;
	CPU_VeryHotCnt_Local = 0;
	VGA_VeryHotCnt = 0;
	VGA_VeryHotCnt_Local = 0;
	CPU_Remote_Temp = 0;
	CPU_Local_Temp  = 0;
	VGA_Remote_Temp = 0;
	VGA_Local_Temp  = 0;
	DBG_CPU_Remote_Temp = 55;
	DBG_CPU_Local_Temp  = 55;
	DBG_VGA_Remote_Temp = 55;
	DBG_VGA_Local_Temp  = 55;
	// leox20150612 Fix fan detect fail issue +++
	CPU_Remote_Temp_BackUp = 0;
	VGA_Remote_Temp_BackUp = 0;
	CPU_Local_Temp_BackUp = 0;
	VGA_Local_Temp_BackUp = 0;
	// leox20150612 Fix fan detect fail issue ---
	CPU_Remote_RPM_Value = RPM0000;						// leox_20111130
	CPU_Local_RPM_Value  = RPM0000;
	VGA_Remote_RPM_Value = RPM0000;
	VGA_Local_RPM_Value  = RPM0000;
	Expect_RPM_Value     = RPM0000;
	Expect_RPM_Value2    = RPM0000;						// leox_20120322 Support_FAN2
	ClrFlag(FanControl_Mode00, InitThermalSenFinish);
	SetFlag(FanControl_Mode01, EnFanCtrlCPURempte);
	//SetFlag(FanControl_Mode01, EnFanCtrlCPULocal);		// leox_20111025
}


//------------------------------------------------------------------------------
// Get Fan1 tachometer
//------------------------------------------------------------------------------
void GetFan1RPM(void)
{
	// Note: The high/low bytes of Fan_Rpm are swapped for compatible fan tool
	Fan_Rpm = (F1TLRR * 256) + F1TMRR;
	SetFlag(POWER_FLAG2, FAN_ON);
	if (Fan_Rpm == 0) {ClrFlag(POWER_FLAG2, FAN_ON);}
}


#if	Support_FAN2	// leox_20120322
//------------------------------------------------------------------------------
// Get Fan2 tachometer
//------------------------------------------------------------------------------
void GetFan2RPM(void)
{	// Note: The high/low bytes of Fan_Rpm2 are swapped for compatible fan tool
	Fan_Rpm2 = (F2TLRR * 256) + F2TMRR;
	SetFlag(POWER_FLAG2, FAN2_ON);
	if (Fan_Rpm2 == 0) {ClrFlag(POWER_FLAG2, FAN2_ON);}
}
#endif	// Support_FAN2


//------------------------------------------------------------------------------
// Get RPM from fan table
//  TmpCur: Current temperature
//  TmpPrv: Previous temperature
//  FanTbl: Pointer of fan table
//  TblLen: Size of fan table
//  RpmCur: Current fan RPM
//------------------------------------------------------------------------------
// leox_20111129 Create function
// leox_20120727 Add argument RpmCur for fixing bug of error fan level
WORD GetFanTblRPM(BYTE TmpCur, BYTE TmpPrv, asFanCtrl FanTbl[], BYTE TblLen, WORD RpmCur)
{
	BYTE idx;

	if (TmpCur > TmpPrv)
	{	// Temperature increased
		for (idx = 0; idx < TblLen; idx++)
		{
			if (TmpCur < FanTbl[idx].FanOn) {break;}
		}
		if (idx >= TblLen) {idx--;}
		if ((TmpCur < FanTbl[idx].FanOn) && (idx > 0)) {idx--;}					// leox_20130924
		return (RpmCur < FanTbl[idx].RPMOn) ? RpmCur : FanTbl[idx].RPMOn;		// leox_20120727
	}
	else
	{	// Temperature decreased
		for (idx = 0; idx < TblLen; idx++)
		{
			if (TmpCur <= FanTbl[idx].FanOff) {break;}
		}
		if (idx > 0) {idx--;}
		return (RpmCur > FanTbl[idx].RPMOff) ? RpmCur : FanTbl[idx].RPMOff;	// leox_20120727	// leox_20120803 Fix coding bug
	}
}


#if	CPU_TYPE_BY_TJMAX
//------------------------------------------------------------------------------
// Set CPU type by TjMax
//------------------------------------------------------------------------------
// leox_20120229 Create function
// leox_20120301 Support OEM TjMax set by BIOS or test tool
void SetCPUTypeByTjMax(void)
{
	BYTE Tmp = 0;

	#ifdef	PECI_Support
	Tmp = CPUTjmax;
	#endif
	if (CPUTjMaxOEM != 0) {Tmp = CPUTjMaxOEM;}	// leox_20120301

	CLR_MASK(CPU_TYPE, (CPU_TJ85 | CPU_TJ90 | CPU_TJ100 | CPU_TJ105));
	if (Tmp == 85)
	{
		SetFlag(CPU_TYPE, CPU_TJ85);
	}
	else if (Tmp == 90)
	{
		SetFlag(CPU_TYPE, CPU_TJ90);
	}
	else if (Tmp == 100)
	{
		SetFlag(CPU_TYPE, CPU_TJ100);
	}
	else if (Tmp == 105)
	{
		SetFlag(CPU_TYPE, CPU_TJ105);
	}
}
#endif	// CPU_TYPE_BY_TJMAX


	#if	CHECK_FAN_FAULT	// David_20120613 add for fan error check
//------------------------------------------------------------------------------
// Check fan error
//------------------------------------------------------------------------------
void CheckFan1Fault(void)
{
	Fan_Rpm = (F1TMRR * 256) + F1TLRR;
	Fan1_Fault_CNT--;
	if (Fan1_Fault_CNT == 0)						// Over 750ms fan still not on or <300rpm
	{
		SetFlag(Device_STAT_CTRL1, Fan1_Fault_On);	// Set Fan error
		DAPWM_VFAN = 0x00;							// Reset fan voltage
	}
	else
	{
		//David modify for fan erro check rpm 0 2014/07/02
		if (((Fan_Rpm != 0xFFFF) && (Fan_Rpm != 0)) && (Fan1_Fault_CNT < 30))	// Fan Rpm > 300 Rpm and != 0 Rpm
		//David modify for fan erro check rpm 0 2014/07/02
		{
			DAPWM_VFAN = 0x00;						// Reset fan voltage
			Fan1_Fault_CNT = 0;						// Fan already check
		}
	}
}
	#endif	// CHECK_FAN_FAULT
#endif	// OEM_FAN

