/*------------------------------------------------------------------------------
 * Title : OEM_BATTERY.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_BATTERY
// leox20150722 Battery configuration +++
// leox20150813 Change power off timer ratio from 2 to 1 (from 4 to 8 hours)
// leox20150821 Change back power off timer ratio to 2 (4 hours) (Customer or PM not approved yet)
//  0: [BYTE] Battery P/N (8 bytes) (Ex: "CP671398")
//  1: (BYTE) Cell type (0: LiIon, 1: NiMH)
//  2: (BYTE) Cell configuration (Ex: 0x32 means 3S2P)
//  3: (WORD) Pre-charge timeout (base: 1 sec) (costant) (Ex: 3600 means 60 minutes)
//  4: (BYTE) Pre-charge current (costant) (Ex: 15 means 300mA)
//  5: (BYTE) Fast charge current (Ex: 63 means 1400mA)
//  6: (BYTE) Power on timer ratio (base: 1 sec) (costant)
//  7: (BYTE) Power off timer ratio (base: 1 sec) (costant)
//  8: (WORD) Charge timeout (base: 1 sec) (costant) (Ex: 28880 means 8/4 hours for power on/off)
//  9: (WORD) Over voltage threshold (Ex: 13270 means 13.27V)
// 10: (WORD) Pre-dead charge threshold (Ex: 9000 means 9.0V)
const sBatteryConfig code BatCfgTbl[] =
{   //        0, 1,    2,    3,  4,  5, 6, 7,     8,     9,   10    // Model
    {"        ", 0, 0x00,    0,  0,  0, 0, 0,     0,     0,    0},  //  0       // No matched
    {"********", 0, 0x32,  3600, 256, 63, 1, 2, 28880, 12900, 9000},  //  1     // Always matched   // leox20150811 SUPPORT_UNKNOWN_BATT_PN
    {"CP671398", 0, 0x32,  3600, 256, 63, 1, 2, 28880, 12900, 9000},  //  2
    {"CP656340", 0, 0x32, 14400, 256, 63, 1, 2, 28880, 13270, 9000},  //  3		// FH9U
    {"CP671396", 0, 0x32,  3600, 256, 63, 1, 2, 28880, 12900, 9000},  //  4
    {"CP651529", 0, 0x32, 14400, 256, 63, 1, 2, 28880, 12900, 9000},  //  5		// FH9U
    {"CP656728", 0, 0x31,  3600, 256, 63, 1, 2, 28880, 12900, 9000},  //  6
    {"CP656352", 0, 0x31,  3600, 256, 63, 1, 2, 28880, 12900, 9000},  //  7
    {"CP652728", 0, 0x31,  3600, 256, 63, 1, 2, 28880, 12900, 9000},  //  8
    {"CP671395", 0, 0x31,  3600, 256, 63, 1, 2, 28880, 12900, 9000},  //  9     // leo_20150612 add for FB5 new bat source
    {"CP700280", 0, 0x32,  7200, 128, 63, 1, 2, 28880, 12900, 9000},  // 10     // FH9U				// leo_20150612 add for FB5 new bat source
    {"CP700278", 0, 0x31,  3600, 256, 63, 1, 2, 28880, 12900, 9000},  // 11     // leox20151208 add for FH9C requested by Fujitsu Ishii Shohei
	{"CP700282", 0, 0x32,  7200, 576, 63, 1, 2, 28880, 12900, 9000},  // 12     // FH9U				// leo_20160510 add for FB9U new bat source
	{"CP708752", 0, 0x32, 14400, 256, 63, 1, 2, 28880, 13270, 9000},  // 13		// FH9U				// leo_20160726 rename from CP656340
};  // CHECK BatCfgTbl[], BQ24780SSetTbl[], CmpDCHGTbl[] and CmpMBATVTbl[] also // leox20151211 Bug fixed by Leo Liu

void SetBatConfig(BYTE Model)
{
	if (Model >= GetArrayNum(BatCfgTbl)) {return;}

	if (BatCfgTbl[Model].Type)
	{
		SetFlag(SEL_CHARGE_STATUS, bat_NiMH);
	}
	else
	{
		ClrFlag(SEL_CHARGE_STATUS, bat_NiMH);
	}

	if (IsFlag1(SYS_FLAG, main_sel))
	{
		ABAT_MODEL = Model;
		ABAT_CELLS = BatCfgTbl[Model].Cell;
		if (Model != 0) {AFAIL_CAUSE = 0;}
	}
	else
	{
		BBAT_MODEL = Model;
		BBAT_CELLS = BatCfgTbl[Model].Cell;
		if (Model != 0) {BFAIL_CAUSE = 0;}
	}
	SEL_BAT_MODEL = Model;		// leox20150907
	LoadMBATV_Threshold(Model);	// leox20150916 BAT_MBATV_PROTECT	// leo_20150916 add power protect

	Bat_PreChargeTimeout       = BatCfgTbl[Model].PreChargeTimeout;
	Bat_PreChargeCURT          = BatCfgTbl[Model].PreChargeCurrent;
	Bat_FastChargeCURT         = BatCfgTbl[Model].FastChargeCurrent;
	Bat_PowerOnTimerRatio      = BatCfgTbl[Model].PowerOnTimerRatio;
	Bat_PowerOffTimerRatio     = BatCfgTbl[Model].PowerOffTimerRatio;
	Bat_ChargeTimeout          = BatCfgTbl[Model].ChargeTimeout;
	Bat_OverVoltThreshold      = BatCfgTbl[Model].OverVoltageThreshold;
	Bat_PreDeadChargeThreshold = BatCfgTbl[Model].DeadChargeThreshold;
	//David add for FJ request change battery CV 2016/11/08
	CheckBatteryEEPROM();
	//David add for FJ request change battery CV 2016/11/08
}


// leox20150722 Rewrite function for battery configuration
void GetBatteryModel(void)
{
	BYTE i, j;

	CopyBatteryStatus();
	ClrFlag(SEL_CHARGE_STATUS, bat_wakeup);	// Assume gauge not work

	// David_20120106 modify for vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_DeviceName;
	// David_20120106 modify for vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine
	if (vOSSMbusRBlcok_OEM())
	{
		for (i = 0; i < GetArrayNum(BatCfgTbl); i++)
		{
			for (j = 0; j < 8; j++)
			{
				if (BatCfgTbl[i].Name[j] != *(&SMB_DATA_OEM + j)) {break;}	// No matched
			}
			if (j >= 8) {break;}	// Matched
		}
		#if SUPPORT_UNKNOWN_BATT_PN	// leox20150811
		if ((i == 0) || (i >= GetArrayNum(BatCfgTbl))) {i = 1;}
		#endif

		if (i < GetArrayNum(BatCfgTbl))
		{	// Matched
			SetBatConfig(i);
			// leox20150722 From MatchBatName() +++
			// leox_20120403 Check return value before set flag bat_wakeup (Suggested by Fox Zhu)
			#if	1	// David_20120327 add for read battery Barcode No and Manufacture Name
			if (SaveBatteryDeviceName() || SaveBatterySerialNo() || SaveBatteryManufacuteDate() || SaveBatteryBarcode())
			#else	// (original)
			if (SaveBatteryDeviceName() || SaveBatterySerialNo() || SaveBatteryManufacuteDate())
			#endif
			{
				#if SUPPORT_UNKNOWN_BATT_PN	// leox20150811
				SeveReadBattFirstUSEDate();	// This function is not supported by all gauge IC
				SetFlag(SEL_CHARGE_STATUS, bat_wakeup);
				Hybrid_Status &= 0x01;
				InitialBattery();
				#else	// (original)
				SetBatConfig(0);
				#endif
			}
			else
			{
				SeveReadBattFirstUSEDate();	// This function is not supported by all gauge IC
				SetFlag(SEL_CHARGE_STATUS, bat_wakeup);
				Hybrid_Status &= 0x01;
				InitialBattery();
			}
			// leox20150722 From MatchBatName() ---
		}
		else
		{	// No matched
			SetBatConfig(0);
		}
	}
	UpdateBatteryStatus();
}
// leox20150722 Battery configuration ---


void RetryGetBatteryModel(void)
{
	SetFlag(SYS_FLAG, main_sel);		// Select ABAT
	GetBatteryModelAgain();
	if (IsFlag1(POWER_FLAG2, func_DUAL))
	{
		ClrFlag(SYS_FLAG, main_sel);	// Select BBAT
		GetBatteryModelAgain();
	}
}


void GetBatteryModelAgain(void)
{
	CopyBatteryStatus();
	if ((IsFlag1(SEL_BAT_STATUS, bat_in)) && (IsFlag0(SEL_BAT_STATUS, bat_destroy)))
	{
		if (IsFlag1(SEL_CHARGE_STATUS, bat_wakeup))		// Gauge already work
		{
			ChkBattWDG();
		}
		else
		{
			if (IsFlag1(POWER_STAT_CTRL, adapter_in))	// AC exist
			{
				RetryModelWithAC();
			}
			else
			{
				GetBatteryModel();
				if (IsFlag1(SYS_FLAG, main_sel))
				{
					ABAT_FAIL_TRY++;
				}
				else
				{
					BBAT_FAIL_TRY++;
				}
			}
		}
	}
}


void RetryModelWithAC(void)
{
	if (IsFlag1(SEL_CHARGE_STATUS, bat_WakeChg))
	{
		GetBatteryModel();
		UpdateBatteryStatus();
	}
}


void CopyBatteryStatus(void)
{
	if (IsFlag1(SYS_FLAG, main_sel))
	{
		SEL_BAT_VOLT = BT1V;
		SEL_BAT_TEMP = ABAT_TEMP;
		SEL_BAT_CAP = ABAT_CAP;
		SEL_BAT_STATUS = ABAT_STATUS;
		SEL_CHARGE_STATUS = ACHARGE_STATUS;
		SEL_BAT_MISC = ABAT_MISC;
		SEL_BAT_ALARM = ABAT_ALARM;
		SEL_BAT_MISC2 = ABAT_MISC2;
		SEL_BAT_CURRENT = BT1I;
	}
	else
	{
		SEL_BAT_VOLT = BT2V;
		SEL_BAT_TEMP = BBAT_TEMP;
		SEL_BAT_CAP = BBAT_CAP;
		SEL_BAT_STATUS = BBAT_STATUS;
		SEL_CHARGE_STATUS = BCHARGE_STATUS;
		SEL_BAT_MISC = BBAT_MISC;
		SEL_BAT_ALARM = BBAT_ALARM;
		SEL_BAT_MISC2 = BBAT_MISC2;
		SEL_BAT_CURRENT = BT2I;
	}
}


BYTE SaveBatteryDeviceName(void)	// leox_20120403 Add return value
{
	// David_20120106 modify for vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_DeviceName;
	// David_20120106 modify for vOSSMbusWBlcok/vOSSMbusWBlcok_OEM routine
	if (vOSSMbusRBlcok_OEM())
	{
		BYTE i;
		*(&SMB_DATA_OEM + SMB_BCNT) = 0;
		DeviceNameBCNT = SMB_BCNT;
		//David_20120327 modify for read Device name
		for (i = 0; i < SMB_BCNT; i++)
		{
			*(&DeviceName15B + i) = *(&SMB_DATA_OEM + i);	// leox_20120403
		}
		//David_20120327 modify for read Device name
		return 0;
	}
	else
	{
		return 1;
	}
}


//David_20120327 add for read battery Barcode No and Manufacture Name
BYTE SaveBatteryBarcode(void)	// leox_20120403 Add return value
{
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_ManufacturerName;
	if (vOSSMbusRBlcok_OEM())
	{
		BYTE i;
		BYTE ErrorBatNameFlag;

		ErrorBatNameFlag = 0;
		Manu_NameBCNT = SMB_BCNT;
		for (i = 0; i < SMB_BCNT; i++)
		{
			//David_20120613 modify for compare manufacture name different
			//David modify for no need to read manufacture name 2014/05/28
			/*if (i < 3)
			{
				if (ManuName1[i] != *(&SMB_DATA_OEM + i))
				{
					SetFlag(ErrorBatNameFlag, BIT0);
				}
				if (ManuName2[i] != *(&SMB_DATA_OEM + i))
				{
					SetFlag(ErrorBatNameFlag, BIT1);
				}
				if (ManuName3[i] != *(&SMB_DATA_OEM + i))
				{
					SetFlag(ErrorBatNameFlag, BIT2);
				}
			}*/
			//David modify for no need to read manufacture name 2014/05/28
			//David_20120613 modify for compare manufacture name different
			(*(&Manu_Name11B + i)) = (*(&SMB_DATA_OEM + i));
		}

		//David modify for FJ battery command 2014/05/28
		SMB_ADDR_OEM = _SMB_BatteryAddr;
		SMB_CMD_OEM = _CMD_Barcode_Fujitsu;
		if (vOSSMbusRBlcok_OEM())
		{
			BYTE i;
			*(&SMB_DATA_OEM + SMB_BCNT) = 0;
			BarcodeBCNT = SMB_BCNT;
			for (i = 0; i < SMB_BCNT; i++)
			{
				(*(&Barcode32B + i)) = (*(&SMB_DATA_OEM + i));
			}
			return 0;
		}
		else
		{
			return 1;
		}

/*		switch (ErrorBatNameFlag)
		{
		case 0x06:	// Match ManuName1
		case 0x03:	// Match ManuName3
			SMB_ADDR_OEM = _SMB_BatteryAddr;
			SMB_CMD_OEM = _CMD_Barcode_Simplo;
			if (vOSSMbusRBlcok_OEM())
			{
				BYTE i;
				*(&SMB_DATA_OEM + SMB_BCNT) = 0;
				BarcodeBCNT = SMB_BCNT;
				for (i = 0; i < SMB_BCNT; i++)
				{
					(*(&Barcode32B + i)) = (*(&SMB_DATA_OEM + i));
				}
			}
			break;

		case 0x05:	// Match ManuName2
			SMB_ADDR_OEM = _SMB_BatteryAddr;
			SMB_CMD_OEM = _CMD_Barcode_Sanyo;
			if (vOSSMbusRBlcok_OEM())
			{
				BYTE i;
				*(&SMB_DATA_OEM + SMB_BCNT) = 0;
				BarcodeBCNT = SMB_BCNT;
				for (i = 0; i < SMB_BCNT; i++)
				{
					(*(&Barcode32B + i)) = (*(&SMB_DATA_OEM + i));
				}
			}
			break;

		default:	// Not match
			break;
		}
		return 0;*/
		//David modify for FJ battery command 2014/05/28
	}
	else
	{
		return 1;
	}
}
//David_20120327 add for read battery Barcode No and Manufacture Name


BYTE SaveBatterySerialNo(void)	// leox_20120403 Add return value
{
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_SerialNumber;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		BatterySerialNo = ((SMB_DATA_OEM << 8) + SMB_DATA1_OEM);
		return 0;
	}
	else
	{
		return 1;
	}
}


BYTE SaveBatteryManufacuteDate(void)	// leox_20120403 Add return value
{
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_ManufactureDate;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		BatteryManuFactureDate = ((SMB_DATA_OEM << 8) + SMB_DATA1_OEM);
		return 0;
	}
	else
	{
		return 1;
	}
}

//David add for FJ request change battery CV 2016/11/08
void CheckBatteryEEPROM(void)
{
	BYTE FormatR,SaftyFunction;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_EEPROM_Fujitsu;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRBlcok_OEM())
	{
		FormatR = (*(&SMB_DATA_OEM + 0x01));
		SaftyFunction = (*(&SMB_DATA_OEM + 0x0d));
		if ((FormatR < 0x02) && (SaftyFunction == 0))
			SetFlag(Battery_Status, Battery_FW_Old);
		else
			ClrFlag(Battery_Status, Battery_FW_Old);
	}
}
//David add for FJ request change battery CV 2016/11/08

void ABATRemoved(void)
{
	#ifdef	NECP_BattRefresh_Support
	NECBattRefreshPSTS = 3;
	InitBattToNormalCondition();
	#endif	//NECP_BattRefresh_Support
	MainBatteryOut();
	SetCHG_High();
	//David modify for charge IC ISL88732 2014/05/21
	#ifdef	SUPPORT_BQ24780S	// leox20150607
	SetChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
	#else	// (original)
	ClearChargeControlCHGIC(Charge_Enable);
	//SetChargeCurrent0mA();
	#endif
	//David modify for charge IC ISL88732 2014/05/21
	if (IsFlag1(POWER_STAT_CTRL, adapter_in) && IsFlag0(BBAT_STATUS, bbat_in)) {ACMODE();}	// leox20150828 Switch to AC mode when battery removed
}


void MainBatteryOut(void)
{
	OffChargeABT();
	TurnOffADISCHG();
	ClrFlag(ABAT_MISC, abat_AutoLearn);
	SetFlag(SYS_FLAG, main_sel);
	SEL_BAT_MODEL = 0;	// leox20150907
	ACHARGE_STATUS = 0;
	ABAT_STATUS = 0;
	ABAT_MODEL = 0;
	ABAT_FAIL_TRY = 0;
	ABAT_MISC = 0;
	ABAT_ALARM = 0;
	AAUTO_STEP = 0;
	ABAT_VOLT = 0;
	ABAT_CELLS = 0;
	BT1I = 0;
	BT1V = 0;
	BT1C = 0;
	ABAT_CAP = 0;
	//David add for read battery cycle count 2014/06/11
	BTCyCle = 0;
	BIOS_BTCyCle = 0;
	//David add for read battery cycle count 2014/06/11
	Bat_PreChargeTimeout = 0;
	Bat_PreChargeCURT = 0;
	Bat_FastChargeCURT = 0;
	Bat_PowerOnTimerRatio = 0;
	Bat_PowerOffTimerRatio = 0;
	Bat_ChargeTimeout = 0;
	Bat_OverVoltThreshold = 0;
	Bat_PreDeadChargeThreshold = 0;
	//David modify for charge IC ISL88732 2014/05/21
	#ifdef	SUPPORT_BQ24780S	// leox20150607
	ClrFlag(BAT_MISC3, BQ24780S_Set_Reg);	// leox20150901
	SetChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
	#else	// (original)
	ClearChargeControlCHGIC(Charge_Enable);
	//SetChargeCurrent0mA();
	#endif
	//David modify for charge IC ISL88732 2014/05/21
	//David_20120327 add for read battery Barcode No and Manufacture Name
	ResetBatteryBarcode();
	ResetManufactureName();
	//David_20120327 add for read battery Barcode No and Manufacture Name
	ResetBatteryDeviceName();
	ResetBatterySerialNo();
	ResetBatteryManufactureDate();
	ResetReadWriteBattFirstDate();
	#ifdef	Battery_Protection_NEC
	ResetBatteryProtectionFunNEC();
	#endif	//Battery_Protection_NEC
	Hybrid_Status &= 0x01;
}


//David_20120327 add for read battery Barcode No and Manufacture Name
void ResetManufactureName(void)
{
	BYTE i;
	Manu_NameBCNT = 0;
	for (i = 0; i <= 10; i++) {(*(&Manu_Name11B + i)) = 0;}
}


void ResetBatteryBarcode(void)
{
	BYTE i;
	BarcodeBCNT = 0;
	for (i = 0; i <= 31; i++) {(*(&Barcode32B + i)) = 0;}
}
//David_20120327 add for read battery Barcode No and Manufacture Name


void ResetBatteryDeviceName(void)
{
	BYTE i;
	DeviceNameBCNT = 0;
	//David_20120327 modify for reset Device name
	for (i = 0; i < 15; i++) {*(&DeviceName15B + i) = 0;}	// leox_20120403
	//David_20120327 modify for reset Device name
}


void ResetBatterySerialNo(void)
{
	BatterySerialNo = 0;
}


void ResetBatteryManufactureDate(void)
{
	BatteryManuFactureDate = 0;
}


void ResetReadWriteBattFirstDate(void)
{
	ChkReadWriteBattFirstUSEDate = 0;
	WriteBattFirstUSEDate = 0;
	WriteBattFirstUSEDatehighbyte = 0;
	ReadBattFirstUSEDate = 0;
}


BYTE SeveReadBattFirstUSEDate(void)	// leox_20120403 Add return value
{
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_First_USE_Date;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		ReadBattFirstUSEDate = ((SMB_DATA_OEM << 8) + SMB_DATA1_OEM);
		return 0;
	}
	else
	{
		return 1;
	}
}


void DoWriteBattFirstUSEDate(void)
{
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_First_USE_Date;
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
	SMB_DATA_OEM = WriteBattFirstUSEDatehighbyte;
	SMB_DATA1_OEM = WriteBattFirstUSEDate;
	vOSSMbusWWord_OEM();
}


void InitialBattery(void)
{
	DoPollingJobs();
	AddBATVAR();
}


void ABATInserted(void)
{
	SetFlag(SYS_FLAG, main_sel);
	ConfigBatteryIn();
	#ifdef	NECP_BattRefresh_Support
	NECBattRefreshPSTS = 3;
	#endif	//NECP_BattRefresh_Support
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_RelativeStateOfCharge;
	if (vOSSMbusRByte_OEM())
	{
		Round_Off_RSOC();
		ABAT_CAP = SMB_DATA_OEM;
		if (ABAT_CAP < RSOC_FULL_BASE) {ClrFlag(ABAT_MISC, abat_ValidFull);}	// leo_20150806 fix S5 ECO mode battery status error
		ABAT_FAIL_TRY = 0;	// reset SMB safety watchdog timer
	}
	ClrFlag(POWER_STAT_CTRL1, lowpower_CHG);
	ECOSleepCheck();
}


void ABATNoChange(void)
{

}


void ConfigBatteryIn(void)
{
	POLLING_STEP = 4;		// Next time PollingBatteryData() executed,
	AddBATVAR();			// Read special/important battery data
	if (IsFlag1(SYS_FLAG, main_sel))
	{
		SetFlag(ABAT_STATUS, abat_in);
		SetFlag(ABAT_MISC, abat_ValidFull);		// assume battery RSOC is bigger then recharge RSOC base
		AFAIL_CAUSE = 7;	// Assume gauge won't work
	}
	else
	{
		SetFlag(BBAT_STATUS, bbat_in);
		BFAIL_CAUSE = 7;	// Assume gauge won't work
	}
	GetBatteryModel();
	#ifdef	SUPPORT_BQ24780S	// leox20150901
	SetFlag(BAT_MISC3, BQ24780S_Set_Reg);
	#endif
}


void AddBATVAR(void)
{
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_DesignCapacity;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		DESIGN_CAPACITY = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
	}

	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_DesignVoltage;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		DESIGN_VOLTAGE = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
	}

	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_FullChargeCapacity;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		DESIGN_FULL_CAPACITY = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		Get1P();
	}
}


void Get1P(void)
{
	FCCxP_CAPACITY = ((DESIGN_FULL_CAPACITY / 200) * ForceDecPercent);
	FCC99P_CAPACITY = (DESIGN_FULL_CAPACITY - (DESIGN_FULL_CAPACITY / 200) - 1);
}


void DoPollingJobs(void)
{
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_RemainingCapacity;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		ModifyRMValue();
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			BT1C = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
		else
		{
			BT2C = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
	}

	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_RelativeStateOfCharge;
	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
	if (vOSSMbusRByte_OEM())
	{
		ModifyRSOCvalue();
		Round_Off_RSOC();
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			ABAT_CAP = SMB_DATA_OEM;
			ABAT_FAIL_TRY = 0;	// Reset SMB safety watchdog timer
		}
		else
		{
			BBAT_CAP = SMB_DATA_OEM;
			BBAT_FAIL_TRY = 0;	// Reset SMB safety watchdog timer
		}
	}

/*	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_RemainingCapacity;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		ModifyRMValue();
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			BT1C = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
		else
		{
			BT2C = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
	}*/

	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_Voltage;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			BT1V = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
		else
		{
			BT2V = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
	}

	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_AverageCurrent;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			BT1I = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
		else
		{
			BT2I = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
	}

	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_BatteryStatus;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			ABAT_ALARM = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
		else
		{
			BBAT_ALARM = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
	}

	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_Temperature;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			BateryTempSBMus = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
		else
		{
			BateryTempSBMus = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		}
	}
}


void ModifyRSOCvalue(void)
{
	if (IsFlag1(SEL_BAT_STATUS, bat_full))
	{
		SMB_DATA_OEM = 100;
	}
	else
	{
		if (SMB_DATA_OEM >= 100)
		{
			SMB_DATA_OEM = 99;
		}
	}
}


void ModifyRMValue(void)
{
	int RMValueCmp = 0;
	RMValueCmp = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
	if (IsFlag1(SEL_BAT_STATUS, bat_full))
	{
		// David_20120106 modify for remain capacity get wrong full charge capacity
		SMB_DATA_OEM = DESIGN_FULL_CAPACITY;
		SMB_DATA1_OEM = (DESIGN_FULL_CAPACITY >> 8);
	}
	else
	{
		// David_20120106 modify for remain capacity get wrong full charge capacity
		if (RMValueCmp >= FCC99P_CAPACITY)
		{
			SMB_DATA_OEM = FCC99P_CAPACITY;
			SMB_DATA1_OEM = (FCC99P_CAPACITY >> 8);
		}
		// David_20120106 modify for remain capacity get wrong full charge capacity
	}
}


void UpdateBatteryStatus(void)
{
	if (IsFlag1(SYS_FLAG, main_sel))
	{
		ABAT_STATUS = SEL_BAT_STATUS;
		ACHARGE_STATUS = SEL_CHARGE_STATUS;
		ABAT_MISC = SEL_BAT_MISC;
		ABAT_MISC2 = SEL_BAT_MISC2;
	}
	else
	{
		BBAT_STATUS = SEL_BAT_STATUS;
		BCHARGE_STATUS = SEL_CHARGE_STATUS;
		BBAT_MISC = SEL_BAT_MISC;
		BBAT_MISC2 = SEL_BAT_MISC2;
	}
}


void PollingBatteryData(void)
{
	SetFlag(SYS_FLAG, main_sel);		// Select ABAT
	ReadPollingData();
	if (IsFlag1(POWER_FLAG2, func_DUAL))
	{
		ClrFlag(SYS_FLAG, main_sel);	// Select BBAT
		ReadPollingData();
	}
}


void ReadPollingData(void)
{
	CopyBatteryStatus();
	if ((IsFlag0(SEL_BAT_STATUS, bat_in)) || (IsFlag1(SEL_BAT_STATUS, bat_destroy)) || (IsFlag0(SEL_CHARGE_STATUS, bat_wakeup)))
	{
		UpdateBatteryStatus();
		return;
	}
	else
	{
		DoPollingJobs();
		UpdateBatteryStatus();
	}
}


// Charging and Discharging Procedure
void PreCharge(void)
{
	if ((ABAT_MODEL != 0) || (BBAT_MODEL != 0))
	{
		if (SEL_BAT_VOLT <= Bat_PreDeadChargeThreshold)
		{
			BatteryWeak();
		}
		else
		{
			TurnOnNormalCharge();
		}
	}
}


void TurnOnNormalCharge(void)
{
	ClrFlag(SEL_CHARGE_STATUS, bat_prechg);
	SetFlag(SEL_CHARGE_STATUS, bat_overtemp + bat_charge + bat_InCharge);	// Assume overheat
	CHG_TIMER = 0;
	OVERTEMP_COUNT = 0;
	ClrFlag(BAT_MISC1, postponetrickle);
	#ifdef	NECP_BattRefresh_Support
	if ((IsFlag0(NECBattRefreshPSTS, CHGC)) && (IsFlag1(NECBattRefreshPSTS, ADPC))) {return;}
	#endif	//NECP_BattRefresh_Support
	//David add for FJ request change battery CV 2016/11/08
	SetChgICChargingVoltage();
	//David add for FJ request change battery CV 2016/11/08
	SetChargeNormalCurrent();
	TurnOnCharger();
	//David modify for charge IC ISL88732 2014/05/21
	#ifdef	SUPPORT_BQ24780S	// leox20150607
	ClrChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
	#else	// (original)
	SetChargeControlCHGIC(Charge_Enable);
	#endif
	//David modify for charge IC ISL88732 2014/05/21
}


void SetChargeNormalCurrent(void)
{
	#ifdef	BattChg_TBLtoDAC	// 1400mA
	if ((ABAT_MODEL != 0) || (BBAT_MODEL != 0)) {SetChargeCurrentDAC(Bat_FastChargeCURT);}
	#endif	//BattChg_TBLtoDAC

	#ifdef	BattChg_SMBtoTBLtoDAC
	if ((ABAT_MODEL != 0) || (BBAT_MODEL != 0)) {ChkGBCCsetValue();}
	#endif	//BattChg_SMBtoTBLtoDAC

	#ifdef	BattChg_SMBtoCHGIC
	if ((ABAT_MODEL != 0) || (BBAT_MODEL != 0)) {SetChgICChargingCurt();}
	#endif	//BattChg_SMBtoCHGIC
}


void ChkGBCCsetValue(void)
{
	int SMB_BAT_ChargingCurrent = 0;
	int DAC_ChargingCurrent = 0;

	if ((IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_destroy)))
	{
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		SMB_ADDR_OEM = _SMB_BatteryAddr;
		SMB_CMD_OEM = _CMD_ChargingCurrent;
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		if (vOSSMbusRWord_OEM())
		{
			SMB_BAT_ChargingCurrent = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
			switch (SMB_BAT_ChargingCurrent)
			{
			case 1680 :
				DAC_ChargingCurrent = 88;
				break;

			case 1200 :
				DAC_ChargingCurrent = 62;
				break;

			case 840 :
				DAC_ChargingCurrent = 43;
				break;

			case 600 :
				DAC_ChargingCurrent = 29;
				break;

			case 240 :
				DAC_ChargingCurrent = 8;
				break;

			default :
				break;
			}
		}
	}
	SetChargeCurrentDAC(DAC_ChargingCurrent);
}


void SetChgICChargingCurt(void)
{
	int CHGIC_ChargingCurrent_DATA = 0;

	if ((IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_destroy)))
	{
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		SMB_ADDR_OEM = _SMB_BatteryAddr;
		SMB_CMD_OEM = _CMD_ChargingCurrent;
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		if (vOSSMbusRWord_OEM())
		{
			CHGIC_ChargingCurrent_DATA = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
			#ifdef	BattChg_SMB_Limit_ChgCurr
			if (IsFlag1(POWER_FLAG, power_on))
			{
				switch (ABAT_MODEL)
				{
				case 1 :
					if (CHGIC_ChargingCurrent_DATA >= ChgCurrLimitValue_M1)
					{
						CHGIC_ChargingCurrent_DATA = ChgCurrLimitValue_M1;
					}
					break;

				case 2 :
					if (CHGIC_ChargingCurrent_DATA >= ChgCurrLimitValue_M2)
					{
						CHGIC_ChargingCurrent_DATA = ChgCurrLimitValue_M2;
					}
					break;

				default :
					break;
				}
			}
			#endif	//BattChg_SMB_Limit_ChgCurr
			SetChargeCurrentCHGIC(CHGIC_ChargingCurrent_DATA);
		}
	}
}


void BatteryWeak(void)
{
	if (IsFlag0(SEL_CHARGE_STATUS, bat_prechg))
	{
		InitPrechg();	// init: prechg
		return;
	}
	SetPreChargeCurrent();
	CHG_TIMER++;
	if ((ABAT_MODEL != 0) || (BBAT_MODEL != 0))
	{
		if (CHG_TIMER >= Bat_PreChargeTimeout) {BatteryFail(2);}
	}
}


void InitPrechg(void)
{
	CHG_TIMER = 0;
	TurnOnPreCharge();
}


void TurnOnPreCharge(void)
{
	SetFlag(SEL_CHARGE_STATUS, bat_prechg + bat_InCharge);
	//David add for FJ request change battery CV 2016/11/08
	SetChgICChargingVoltage();
	//David add for FJ request change battery CV 2016/11/08
	SetPreChargeCurrent();
	TurnOnCharger();
}


void SetPreChargeCurrent(void)
{
	#ifdef	BattChg_TBLtoDAC		// 300mA
	if ((ABAT_MODEL != 0) || (BBAT_MODEL != 0)) {SetChargeCurrentDAC(Bat_PreChargeCURT);}
	#endif	//BattChg_TBLtoDAC

	#ifdef	BattChg_SMBtoTBLtoDAC	// 300mA
	if ((ABAT_MODEL != 0) || (BBAT_MODEL != 0)) {SetChargeCurrentDAC(Bat_PreChargeCURT);}
	#endif	//BattChg_SMBtoTBLtoDAC

	#ifdef	BattChg_SMBtoCHGIC
	//David modify for charge IC ISL88732 2014/05/21
		#ifdef	SUPPORT_BQ24780S	// leox20150607
	ClrChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
		#else	// (original)
	SetChargeControlCHGIC(Charge_Enable);
		#endif
	//David modify for charge IC ISL88732 2014/05/21
	//SetChargeCurrentCHGIC(256);		// 512mA
	SetChargeCurrentCHGIC(Bat_PreChargeCURT);	// Leo_20160726 precharge current value by battery model by power Jeff.
	#endif	//BattChg_SMBtoCHGIC
}


void SetChargeCurrentDAC(int ChargeCurrentDAC)
{
	DA_CCSET = ChargeCurrentDAC;
	ISL6251ACHICWaitVinCNT = CHICWaitVinCNTValue;
}


void SetChargeCurrentCHGIC(int ChargeCurrentCHGIC)
{
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
	SMB_ADDR_OEM = SMBCHR_A;
	SMB_CMD_OEM = ChgIC_ChargingCurrent;
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
	if (ChargeCurrentCHGIC == 0)
	{
		ChargeCurrentCHGIC = ChargingCurrent_Backup;
	}
	ChargingCurrent_Backup = ChargeCurrentCHGIC;
	SMB_DATA_OEM = ChargeCurrentCHGIC;
	SMB_DATA1_OEM = (ChargeCurrentCHGIC >> 8);
	vOSSMbusWWord_OEM();
	ISL6251ACHICWaitVinCNT = CHICWaitVinCNTValue;
}


void SetChargeVoltageCHGIC(int ChargeVoltageCHGIC)
{
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
	SMB_ADDR_OEM = SMBCHR_A;
	SMB_CMD_OEM = ChgIC_ChargingVoltage;
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
	SMB_DATA_OEM = ChargeVoltageCHGIC;
	SMB_DATA1_OEM = (ChargeVoltageCHGIC >> 8);
	ChargingVoltage_Backup = ChargeVoltageCHGIC;
	vOSSMbusWWord_OEM();
}

//David add for FJ request change battery CV 2016/11/08
void Check_Battery_FCC_Status(void)
{
	LWORD FCC_Value;
	int percentage;

	FCC_Value = DESIGN_FULL_CAPACITY;
	percentage = (FCC_Value * 100) / DESIGN_CAPACITY ;
	if (percentage <= 70)
	{
		SetFlag(Battery_Status, Battery_Old);
	}
	else
	{
		ClrFlag(Battery_Status, Battery_Old);
		if (IsFlag1(Battery_Status, Start_Chg))
			ClrFlag(Battery_Status, Start_Chg);
	}
}
//David add for FJ request change battery CV 2016/11/08

void FCC_UpDate(void)
{
	if (IsFlag1(ABAT_STATUS, abat_in))
	{
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		SMB_ADDR_OEM = _SMB_BatteryAddr;
		SMB_CMD_OEM = _CMD_FullChargeCapacity;
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		if (vOSSMbusRWord_OEM())
		{
			DESIGN_FULL_CAPACITY = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
			Get1P();
		}
		//David add for FJ request change battery CV 2016/11/08
		Check_Battery_FCC_Status();
		//David add for FJ request change battery CV 2016/11/08
	}
}


void ProcessAutoLearn(void)
{
	if (BAUTO_STEP != 0)
	{
		if (AAUTO_STEP == 0)
		{
			ClrFlag(SYS_FLAG, main_sel);	// Select BBAT
			ALStepControl();
		}
	}
	else
	{
		SetFlag(SYS_FLAG, main_sel);		// Select ABAT
		ALStepControl();
		if (AAUTO_STEP == 0)
		{
			ClrFlag(SYS_FLAG, main_sel);	// Select BBAT
			ALStepControl();
		}
	}
}


void ALStepControl(void)
{
	CopyBatteryStatus();

	if ((IsFlag0(SEL_BAT_MISC, bat_AutoLearn)) || (IsFlag0(SEL_BAT_STATUS, bat_in)) || (IsFlag1(SEL_BAT_STATUS, bat_destroy)))
	{
		ALStop();
		return;
	}
	else
	{
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			switch (AAUTO_STEP)
			{
			case 0 :
				OffChargeABT();
				OffChargeBBT();
				ClrFlag(ABAT_MISC, force_chgA);
				ClrFlag(BBAT_MISC, force_chgB);
				TurnOffADISCHG();
				TurnOffBDISCHG();
				ClrFlag(SEL_BAT_MISC, bat_force_disc);
				SetFlag(SEL_BAT_MISC, bat_force_chg);
				AAUTO_STEP++;
				EndAutoLearn();
				break;

			case 1 :
				#ifdef	NECP_BattRefresh_Support
				if ((IsFlag0(NECBattRefreshPSTS, ForceDischgDoing)) && (IsFlag0(SEL_BAT_STATUS, bat_full)))
				#else	//NECP_BattRefresh_Support
				if (IsFlag0(SEL_BAT_STATUS, bat_full))
				#endif	//NECP_BattRefresh_Support
				{
					EndAutoLearn();
				}
				else
				{
					ClrFlag(SEL_BAT_MISC, bat_force_chg);
					SetFlag(SEL_BAT_MISC, bat_force_disc);
					AAUTO_STEP++;
					EndAutoLearn();
				}
				break;

			case 2 :
				if (IsFlag1(SEL_BAT_STATUS, bat_dead))
				{
					ClrFlag(SEL_BAT_MISC, bat_force_disc);
					SetFlag(SEL_BAT_MISC, bat_force_chg);
					AAUTO_STEP++;
					EndAutoLearn();
				}
				else
				{
					EndAutoLearn();
				}
				break;

			default :
				if (IsFlag1(SEL_BAT_STATUS, bat_full))
				{
					ClrFlag(SEL_BAT_MISC, bat_AutoLearn);
					AAUTO_STEP = 0;
					EndAutoLearn();
				}
				else
				{
					EndAutoLearn();
				}
				break;
			}
		}
		else
		{
			switch (BAUTO_STEP)
			{
			case 0 :
				OffChargeABT();
				OffChargeBBT();
				ClrFlag(ABAT_MISC, force_chgA);
				ClrFlag(BBAT_MISC, force_chgB);
				TurnOffADISCHG();
				TurnOffBDISCHG();
				ClrFlag(SEL_BAT_MISC, bat_force_disc);
				SetFlag(SEL_BAT_MISC, bat_force_chg);
				BAUTO_STEP++;
				EndAutoLearn();
				break;

			case 1 :
				#ifdef	NECP_BattRefresh_Support
				if ((IsFlag0(NECBattRefreshPSTS, ForceDischgDoing)) && (IsFlag0(SEL_BAT_STATUS, bat_full)))
				#else	//NECP_BattRefresh_Support
				if (IsFlag0(SEL_BAT_STATUS, bat_full))
				#endif	//NECP_BattRefresh_Support
				{
					EndAutoLearn();
				}
				else
				{
					ClrFlag(SEL_BAT_MISC, bat_force_chg);
					SetFlag(SEL_BAT_MISC, bat_force_disc);
					BAUTO_STEP++;
					EndAutoLearn();
				}
				break;

			case 2 :
				if (IsFlag1(SEL_BAT_STATUS, bat_dead))
				{
					ClrFlag(SEL_BAT_MISC, bat_force_disc);
					SetFlag(SEL_BAT_MISC, bat_force_chg);
					BAUTO_STEP++;
					EndAutoLearn();
				}
				else
				{
					EndAutoLearn();
				}
				break;

			default :
				if (IsFlag1(SEL_BAT_STATUS, bat_full))
				{
					ClrFlag(SEL_BAT_MISC, bat_AutoLearn);
					BAUTO_STEP = 0;
					EndAutoLearn();
				}
				else
				{
					EndAutoLearn();
				}
				break;
			}
		}
	}
}


void ALStop(void)
{
	ClrFlag(SEL_BAT_MISC, bat_AutoLearn);
	if (IsFlag1(SYS_FLAG, main_sel))
	{
		AAUTO_STEP = 0;
	}
	else
	{
		BAUTO_STEP = 0;
	}
	EndAutoLearn();
}


void EndAutoLearn(void)
{
	if (IsFlag1(SYS_FLAG, main_sel))
	{
		ABAT_MISC = SEL_BAT_MISC;
	}
	else
	{
		BBAT_MISC = SEL_BAT_MISC;
	}
}


void ProcessDischarge(void)
{
	ForceDischarge();
}


void ForceDischarge(void)
{
	if (IsFlag1(POWER_FLAG, enter_SUS))
	{
		HandleDischarge();
	}
	else
	{
		if (IsFlag1(POWER_FLAG, power_on))
		{
			HandleDischarge();
		}
		else
		{
			EndProcDischg();
		}
	}
}


void HandleDischarge(void)
{
	if (IsFlag1(ABAT_MISC, force_discA))
	{
		DischargeABAT();
	}
	else
	{
		if (IsFlag1(BBAT_MISC, force_discB))
		{
			DischargeBBAT();
		}
		else
		{
			if (IsFlag1(POWER_STAT_CTRL, adapter_in))
			{
				EndProcDischg();
			}
			else
			{
				if (IsFlag1(ABAT_STATUS, abat_dischg))
				{
					DischargeABAT();
				}
				else
				{
					if (IsFlag1(BBAT_STATUS, bbat_dischg))
					{
						DischargeBBAT();
					}
					else
					{
						GetDischgCode();
						EndProcDischg();
					}
				}
			}
		}
	}
}


void GetDischgCode(void)
{
	BYTE BattSta = 0;	// leox_20111201

	if (IsFlag1(BBAT_STATUS, bbat_dead		)) {SetFlag(BattSta, BIT0);}
	if (IsFlag1(BBAT_STATUS, bbat_destroy	)) {SetFlag(BattSta, BIT1);}
	if (IsFlag1(BBAT_STATUS, bbat_in		)) {SetFlag(BattSta, BIT2);}
	if (IsFlag1(ABAT_STATUS, abat_dead		)) {SetFlag(BattSta, BIT3);}
	if (IsFlag1(ABAT_STATUS, abat_destroy	)) {SetFlag(BattSta, BIT4);}
	if (IsFlag1(ABAT_STATUS, abat_in		)) {SetFlag(BattSta, BIT5);}

	switch (BattSta)
	{
	case 4:
	case 12:
	case 20:
	case 28:	// Discharge 2nd Batt first
	case 36:	// Discharge 2nd Batt first
	case 44:
	case 52:
	case 60:
		DischargeBBAT();
		break;

	case 32:
	case 33:
	case 34:
	case 35:	// Discharge 1st Batt first
//	case 36:	// Discharge 1st Batt first
	case 37:
	case 38:
	case 39:
		DischargeABAT();
		break;

//	case 36:	// Discharge BAT with higher cap
//		CheckHighCapacity();
//		break;

	default:	// Discharge BAT with higher cap
		break;
	}
}


void DischargeBBAT(void)
{
	if (IsFlag0(POWER_FLAG2, func_DUAL))
	{
		EndProcDischg();
	}
	else
	{
		if (IsFlag1(ACHARGE_STATUS, abat_WakeChg))
		{
			EndProcDischg();
		}
		else
		{
			ClrFlag(SYS_FLAG, main_sel);
			if (IsFlag1(ABAT_MISC, abat_inUSE))
			{
				DischgBatt();
			}
			else
			{
				SetFlag(BBAT_MISC, bbat_inUSE);
				DischgBatt();
			}
		}
	}
}


void EndProcDischg(void)
{

}


void DischargeABAT(void)
{
	if (IsFlag1(BCHARGE_STATUS, bbat_WakeChg))
	{
		EndProcDischg();
	}
	else
	{
		SetFlag(SYS_FLAG, main_sel);
		if (IsFlag1(BBAT_MISC, bbat_inUSE))
		{
			DischgBatt();
		}
		else
		{
			SetFlag(ABAT_MISC, abat_inUSE);
			DischgBatt();
		}
	}
}


void DischgBatt(void)
{
	CopyBatteryStatus();
	if (IsFlag0(SEL_BAT_STATUS, bat_dischg))
	{
		TurnOnDischarger();
		ChkBATDead();
	}
	else
	{
		ChkBATDead();
	}
}


void ChkBATDead(void)
{
	if (IsFlag0(ACHARGE_STATUS, bat_NiMH))
	{
		ChkBATDeadLi();
	}
	else
	{
		if (SEL_BAT_CAP < Low_CAP_Ni)
		{
			SetFlag(SEL_BAT_STATUS, bat_low);
			SetFlag(POWER_FLAG2, bat_warning);
			CheckCriticalLow();
			CheckBattDead();
		}
		else
		{
			ChkWarningCap();
		}
	}
}


void ChkBATDeadLi(void)
{
	if (SEL_BAT_CAP < Low_CAP_Li)
	{
		SetFlag(SEL_BAT_STATUS, bat_low);
		SetFlag(POWER_FLAG2, bat_warning);
		CheckCriticalLow();
		CheckBattDead();
	}
	else
	{
		ChkWarningCap();
	}
}


void CheckBattDead(void)
{
	if (SEL_BAT_VOLT <= Bat_PreDeadChargeThreshold)
	{
		ConfirmBATDead();
	}
	else
	{
		BAT_DEAD_CNT = 0;
		EndDischgBAT();
	}
}


void EndDischgBAT(void)
{
	UpdateBatteryStatus();
}


void ConfirmBATDead(void)
{
	BAT_DEAD_CNT++;		// Qualify battery dead
	if (BAT_DEAD_CNT >= dead_qualify)
	{
		SETbxATDead();
	}
	else
	{
		EndDischgBAT();	// Confirm?
	}
}


void SETbxATDead(void)
{
	SetFlag(SEL_BAT_STATUS, bat_low + bat_dead);
	ClrFlag(SEL_BAT_MISC, bat_inUSE);
	BATDead1();
}


void BATDead1(void)
{
	TurnOffDischarger();
	FreeDischarge();
	if (IsFlag1(ABAT_MISC, abat_AutoLearn))
	{
		BATDead2();
	}
	else
	{
		if (IsFlag0(ABAT_MISC, force_discA))
		{
			BATDead3();
		}
		else
		{
			ClrFlag(ABAT_MISC, force_discA);
			BATDead2();
		}

	}
}


void BATDead2(void)
{
	ACMODE();
	EndDischgBAT();
}


void BATDead3(void)
{
	SetFlag(MISC_FLAG, F_CriLow);
	ShutDnCause = 0x06;
	ForceOffPower();
	EndDischgBAT();
}


void CheckCriticalLow(void)
{
	if (IsFlag0(ACHARGE_STATUS, bat_NiMH))
	{
		CheckCriticalLowLi();
	}
	else
	{
		if (SEL_BAT_CAP < CriLow_CAP_Li)
		{
			SetFlag(MISC_FLAG, F_CriLow);
			CheckBattDead();
		}
		else
		{
			ChkCriticalLow();
		}
	}
}


void CheckCriticalLowLi(void)
{
	if (SEL_BAT_CAP < CriLow_CAP_Hi)
	{
		SetFlag(MISC_FLAG, F_CriLow);
		ChkCriticalLow();
	}
	else
	{
		ChkCriticalLow();
	}
}


void ChkCriticalLow(void)
{

}


void ChkWarningCap(void)
{
	if (IsFlag0(ACHARGE_STATUS, bat_NiMH))
	{
		ChkWarningCapLi();
	}
	else
	{
		if (SEL_BAT_CAP < Warning_CAP_Li)
		{
			SetFlag(POWER_FLAG2, bat_warning);
			CheckBattDead();
		}
		else
		{
			EndDischgBAT();
		}
	}
}


void ChkWarningCapLi(void)
{
	if (SEL_BAT_CAP < Warning_CAP_Ni)
	{
		SetFlag(POWER_FLAG2, bat_warning);
	}
	else
	{
		CheckBattDead();
	}
}


void TurnOffDischarger(void)
{
	if (IsFlag0(SEL_BAT_STATUS, bat_dischg))
	{
		EndOffDisgr();
	}
	else
	{
		ClrFlag(SEL_BAT_MISC, bat_force_disc);
		ClrFlag(SEL_BAT_STATUS, bat_dischg);
		ClrFlag(SEL_CHARGE_STATUS, bat_level3);
	}
}


void EndOffDisgr(void)
{

}


void FreeDischarge(void)
{

}


void BatteryFullyDischarge(void)
{

}


void CheckHighCapacity(void)
{
	if (IsFlag0(POWER_FLAG2, func_DUAL))
	{
		DischargeABAT();
	}
	else
	{
		if (ABAT_CAP < BBAT_CAP)
		{
			DischargeBBAT();
		}
		else
		{
			DischargeBBAT();
		}
	}
}


void TrickleCharge(void)
{
	if ((IsFlag1(POWER_STAT_CTRL, adapter_in)) && (IsFlag0(BAT_MISC1, STOP_charge)) && (IsFlag0(ABAT_MISC, force_discA)) && (IsFlag0(BBAT_MISC, force_discB)))
	{
		SetFlag(SYS_FLAG, main_sel);			// Select ABAT
		if (CheckLiReCharge() != 1)
		{
			if (IsFlag1(POWER_FLAG2, func_DUAL))
			{
				ClrFlag(SYS_FLAG, main_sel);	// Select BBAT
				CheckLiReCharge();
			}
		}
	}
}


int CheckLiReCharge(void)
{
	CopyBatteryStatus();
	if (IsFlag0(SEL_BAT_STATUS, bat_in))		// Battery not exist
	{
		UpdateBatteryStatus();
		return(0);
	}
	if (IsFlag0(SEL_BAT_STATUS, bat_full))		// Battery not fully charged
	{
		UpdateBatteryStatus();
		return(1);
	}
	if (IsFlag0(SEL_CHARGE_STATUS, bat_NiMH))	// Type is Li
	{
		if (IsFlag1(SEL_BAT_ALARM, FullyChg))
		{
			UpdateBatteryStatus();
			return(0);
		}
		else
		{
			ClrFlag(SEL_BAT_STATUS, bat_full);
			UpdateBatteryStatus();
			return(1);
		}
	}
	NiTrickleChargeConstCurrt();
	UpdateBatteryStatus();
	return(0);
}


void NiTrickleChargeConstCurrt(void)
{
	switch (ATRICKLE_DUTY_ConstCurrt)
	{
	case 0 :
		TurnOffCHGPin();
		ClrFlag(BAT_MISC1, postponetrickle);
		NiTrickleSetDuty();
		break;

	case 1 :
		if ((IsFlag0(SEL_CHARGE_STATUS, bat_overtemp)) && (IsFlag0(BAT_MISC1, postponetrickle)))
		{	// If not overheat and postpone not request
			TurnOnTrickleChargeConstCurrt();
			SetFlag(BAT_MISC1, postponetrickle);	// Charger turned on
			DecDutyCounterCCC();
		}
		break;

	default :
		DecDutyCounterCCC();
		break;
	}
}


void DecDutyCounterCCC(void)
{
	if (ATRICKLE_DUTY_ConstCurrt != 0) {ATRICKLE_DUTY_ConstCurrt--;}
}


void NiTrickleSetDuty(void)
{
	if ((IsFlag1((BateryTempSBMus >> 8), BIT7)) || (BateryTempSBMus < 2980))
	{	// Battery Temperature is minus or < 25C
		ATRICKLE_DUTY_ConstCurrt = 1200;		// 1/1200(sec) Duty Cycle
	}
	else
	{
		if (BateryTempSBMus < 3080)
		{	// 25C <= Battery Temperature < 35C
			ATRICKLE_DUTY_ConstCurrt = 600;		// 1/600(sec) Duty Cycle
		}
		else
		{
			if (BateryTempSBMus < 3180)
			{	// 35C <= Battery Temperature < 45C
				ATRICKLE_DUTY_ConstCurrt = 225;	// 1/225(sec) Duty Cycle
			}
			else
			{	// 45C <= Battery Temperature
				ATRICKLE_DUTY_ConstCurrt = 150;	// 1/150(sec) Duty Cycle
			}
		}
	}
}


void TurnOnTrickleChargeConstCurrt(void)
{
	SetTrickleChargeCurrent();
	TurnOnCharger();
}


void SetTrickleChargeCurrent(void)
{
	#ifdef	BattChg_TBLtoDAC
	SetChargeCurrentDAC(63);	// Set trickle charge current 1.2A
	#endif	//BattChg_TBLtoDAC

	#ifdef	BattChg_SMBtoTBLtoDAC
	SetChargeCurrentDAC(63);	// Set trickle charge current 1.2A
	#endif	//BattChg_SMBtoTBLtoDAC

	#ifdef	BattChg_SMBtoCHGIC
	//David modify for charge IC ISL88732 2014/05/21
		#ifdef	SUPPORT_BQ24780S	// leox20150607
	ClrChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
		#else	// (original)
	SetChargeControlCHGIC(Charge_Enable);
		#endif
	//David modify for charge IC ISL88732 2014/05/21
	SetChargeCurrentCHGIC(640);	// 1280mA
	#endif	//BattChg_SMBtoCHGIC
}


void OffChargeABT(void)
{
	if (IsFlag1(ACHARGE_STATUS, abat_InCharge))
	{
		ClrFlag(ACHARGE_STATUS, abat_charge + abat_prechg + abat_WakeChg + abat_InCharge + abat_level3);
		//David add for FJ request change battery CV 2016/11/08
		ClrFlag(Battery_Status, Start_Chg);
		//David add for FJ request change battery CV 2016/11/08
		OffAChgr();
	}
}


void OffAChgr(void)
{
	ATRICKLE_DUTY_ConstCurrt = 0;
	off_ABAT_HW();
	EndOffChgr1();
}


void off_ABAT_HW(void)
{
	if (IsFlag0(BCHARGE_STATUS, bbat_level3)) {TurnOffCHGPin();}
}


void EndOffChgr1(void)
{
	ClrFlag(BAT_MISC1, postponetrickle);
}


void TurnOffCHGPin(void)
{
	SetCHG_High();
	//David modify for charge IC ISL88732 2014/05/21
	#ifdef	SUPPORT_BQ24780S	// leox20150607
	SetChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
	#else	// (original)
	ClearChargeControlCHGIC(Charge_Enable);
	//SetChargeCurrent0mA();
	#endif
	//David modify for charge IC ISL88732 2014/05/21
	ClrFlag(BAT_MISC3, charger_on);
}


void SetCHG_High(void)
{
	ISL6251ACHICWaitVinCNT = 0;
	ACHG_off();
	SetFlag(SYS_MISC2, CHGpinHigh);
}


void OffChargeBBT(void)
{
	if (IsFlag1(BCHARGE_STATUS, bbat_InCharge))
	{
		ClrFlag(BCHARGE_STATUS, bbat_charge + bbat_prechg + bbat_WakeChg + bbat_InCharge + bbat_level3);
		OffBChgr();
	}
}


void OffBChgr(void)
{
	BTRICKLE_DUTY = 0;
	off_BBAT_HW();
	EndOffChgr1();
}


void off_BBAT_HW(void)
{
	if (IsFlag0(ACHARGE_STATUS, abat_level3)) {TurnOffCHGPin();}
}


void TurnOnDischarger(void)
{
	BAT_DEAD_CNT = 0;
	SetFlag(SEL_BAT_STATUS, bat_dischg);
	ClrFlag(SEL_BAT_STATUS, bat_full);
	SetFlag(SEL_CHARGE_STATUS, bat_level3);
	if (IsFlag1(SYS_FLAG, main_sel))
	{
		TurnOnADISCHG();
	}
	else
	{
		TurnOnBDISCHG();
	}
}


void TurnOnADISCHG(void)
{
	ABAT_ON_on();
	DCMODE();
}


void TurnOnBDISCHG(void)
{
	ABAT_ON_off();
	DCMODE();
}


void TurnOffADISCHG(void)
{
	if (IsFlag1(ABAT_STATUS, abat_dischg))
	{
		ClrFlag(ABAT_STATUS, abat_dischg);
		ClrFlag(ABAT_MISC, force_discA);
		ClrFlag(ACHARGE_STATUS, abat_level3);
	}
}


void SetChargeCurrent0mA(void)
{
	#ifdef	BattChg_TBLtoDAC
	SetChargeCurrentDAC0mA();
	#endif	//BattChg_TBLtoDAC

	#ifdef	BattChg_SMBtoTBLtoDAC
	SetChargeCurrentDAC0mA();
	#endif	//BattChg_SMBtoTBLtoDAC

	#ifdef	BattChg_SMBtoCHGIC
	SetChargeCurrentCHGIC0mA();
	#endif	//BattChg_SMBtoCHGIC
}


void SetChargeCurrentDAC0mA(void)
{
	DA_CCSET = 0;
}


void SetChargeCurrentCHGIC0mA(void)
{
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
	SMB_ADDR_OEM = SMBCHR_A;
	SMB_CMD_OEM = ChgIC_ChargingCurrent;
	// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
	SMB_DATA_OEM = 0;
	SMB_DATA1_OEM = 0;
	vOSSMbusWWord_OEM();
}


void TurnOnCharger(void)
{
	SEL_BAT_STATUS = SEL_BAT_STATUS & 0x0F;
	SetFlag(BAT_MISC3, charger_on);
	ClrFlag(SEL_BAT_STATUS, bat_low);
	ClrFlag(POWER_FLAG2, bat_warning);
	ClrFlag(MISC_FLAG, F_CriLow);
	if (IsFlag0(SYS_FLAG, main_sel))
	{
		set_BCHG();
	}
	else
	{
		set_ACHG();
	}
}


void set_ACHG(void)
{
	ABAT_ON_on();
	ACMODE();
}


void set_BCHG(void)
{
	ABAT_ON_off();
	ACMODE();
}


void TurnOffBDISCHG(void)
{
	if (IsFlag1(BBAT_STATUS, bbat_dischg))
	{
		ClrFlag(BBAT_STATUS, bbat_dischg);
		ClrFlag(BBAT_MISC, force_discB);
		ClrFlag(BCHARGE_STATUS, bbat_level3);
	}
}


void SetChgICChargingVoltage(void)
{
	if ((IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_destroy)))
	{
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		SMB_ADDR_OEM = _SMB_BatteryAddr;
		SMB_CMD_OEM = _CMD_ChargingVoltage;
		// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
		if (vOSSMbusRWord_OEM())
		{
			if ((SMB_DATA_OEM == 0) && (SMB_DATA1_OEM == 0))
			{
				SMB_DATA_OEM = ChargingVoltage_Backup;
				SMB_DATA1_OEM = (ChargingVoltage_Backup >>8);
			}
			ChargingVoltage_Backup = ((SMB_DATA1_OEM <<8) + SMB_DATA_OEM);
			//David add for FJ request change battery CV 2016/11/08
			ChargingVoltage_Backup = 12400;
			SMB_DATA_OEM = ChargingVoltage_Backup;
			SMB_DATA1_OEM = (ChargingVoltage_Backup >>8);
			/*
			if (IsFlag1(Battery_Status, Battery_Old))
			{
				if (IsFlag1(Battery_Status, Battery_FW_Old))
				{
					if (IsFlag1(Battery_Status, Start_Chg))
					{
						if (ChargingVoltage_Backup == 12600)
						{
							ChargingVoltage_Backup = 12400;
							SMB_DATA_OEM = ChargingVoltage_Backup;
							SMB_DATA1_OEM = (ChargingVoltage_Backup >>8);
						}
						else if(ChargingVoltage_Backup == 13050)
						{
							ChargingVoltage_Backup = 12848;
							SMB_DATA_OEM = ChargingVoltage_Backup;
							SMB_DATA1_OEM = (ChargingVoltage_Backup >>8);
						}
						else
							BatteryFail(9);
					}
					else
					{
						if ((IsFlag1(SEL_CHARGE_STATUS, bat_InCharge)) && CHG_TIMER == 0)
						{
							if (ChargingVoltage_Backup == 12600)
							{
								SetFlag(Battery_Status, Start_Chg);
								ChargingVoltage_Backup = 12400;
								SMB_DATA_OEM = ChargingVoltage_Backup;
								SMB_DATA1_OEM = (ChargingVoltage_Backup >>8);
							}
							else if(ChargingVoltage_Backup == 13050)
							{
								SetFlag(Battery_Status, Start_Chg);
								ChargingVoltage_Backup = 12848;
								SMB_DATA_OEM = ChargingVoltage_Backup;
								SMB_DATA1_OEM = (ChargingVoltage_Backup >>8);
							}
							else
								BatteryFail(9);
						}
					}
				}
			}
			*/
			#if SUPPORT_UNKNOWN_BATT_PN
			if (ABAT_MODEL == 0x01)
			#else
			if (ABAT_MODEL == 0)
			#endif
			{
				if((ChargingVoltage_Backup == 12600) || (ChargingVoltage_Backup == 12400))
					Bat_OverVoltThreshold = OverVoltThreshold_12900mV;
				else if((ChargingVoltage_Backup == 13050) || (ChargingVoltage_Backup == 12848))
					Bat_OverVoltThreshold = OverVoltThreshold_13270mV;
			}
			//David add for FJ request change battery CV 2016/11/08
			// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
			SMB_ADDR_OEM = SMBCHR_A;
			SMB_CMD_OEM = ChgIC_ChargingVoltage;
			// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
			vOSSMbusWWord_OEM();
		}
		else
		{
			#if SUPPORT_UNKNOWN_BATT_PN
			if (ABAT_MODEL == 0x01)
			#else
			if (ABAT_MODEL == 0)
			#endif
			Bat_OverVoltThreshold = OverVoltThreshold_12900mV;
		}
	}
}


void SetCghICInputCurrent(void)
{
	if ((IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_destroy)))
	{
		// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
		SMB_ADDR_OEM = SMBCHR_A;
		SMB_CMD_OEM = ChgIC_InputCurrent;
		// David_20120106 modify for vOSSMbusWWord/vOSSMbusWWord_OEM routine
		if (IsFlag1(Device_STAT_CTRL1, VGA_Status))
		{
			SMB_DATA_OEM = InputCurrentValue90W;
			SMB_DATA1_OEM = (InputCurrentValue90W >> 8);
		}
		else
		{
			SMB_DATA_OEM = InputCurrentValue65W;
			SMB_DATA1_OEM = (InputCurrentValue65W >> 8);
		}
		vOSSMbusWWord_OEM();
	}
}


void ChkBattWDG(void)
{
	if (IsFlag1(SYS_FLAG, main_sel))
	{
		ABAT_FAIL_TRY++;
		if (ABAT_FAIL_TRY >= 50) {SET_MASK(SLVISELR, OVRSMDBG);}	// 120614 chris add for not into debug mode for SMBus error	// leox_20120810
		if (ABAT_FAIL_TRY >= BattWDG_Timeout)
		{
			BatteryFail(4);
			UpdateBatteryStatus();
		}
	}
	else
	{
		BBAT_FAIL_TRY++;
		if (BBAT_FAIL_TRY >= 50) {SET_MASK(SLVISELR, OVRSMDBG);}	// 120614 chris add for not into debug mode for SMBus error	// leox_20120810
		if (BBAT_FAIL_TRY >= BattWDG_Timeout)
		{
			BatteryFail(4);
			UpdateBatteryStatus();
		}
	}
}


void BatteryFail(int BattFailCause)
{
	if (IsFlag1(SYS_FLAG, main_sel))
	{
		AFAIL_CAUSE = BattFailCause;
	}
	else
	{
		BFAIL_CAUSE = BattFailCause;
	}
	ClrFlag(SEL_BAT_MISC, bat_force_chg + bat_force_disc + bat_next + bat_inUSE);
	SetFlag(SEL_BAT_STATUS, bat_destroy);
	ClrFlag(SEL_BAT_STATUS, bat_dischg);
	ClrFlag(SEL_CHARGE_STATUS, bat_level3);
	ClrFlag(SEL_BAT_MISC, bat_ValidFull);
	TurnOffCharger();
}


void TurnOffCharger(void)
{
	if (IsFlag1(SEL_CHARGE_STATUS, bat_InCharge))
	{
		ClrFlag(SEL_CHARGE_STATUS, bat_charge + bat_prechg + bat_WakeChg + bat_InCharge + bat_level3);
		//David add for FJ request change battery CV 2016/11/08
		ClrFlag(Battery_Status, Start_Chg);
		//David add for FJ request change battery CV 2016/11/08
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			OffAChgr();
		}
		else
		{
			OffBChgr();
		}
	}
}


void ProcessCharge(void)
{
	SetFlag(SYS_FLAG, main_sel);		// Select ABAT
	CheckBatteryTemperature();
	if (IsFlag1(POWER_FLAG2, func_DUAL))
	{
		ClrFlag(SYS_FLAG, main_sel);	// Select BBAT
		CheckBatteryTemperature();
	}

	ChooseBatteryToCharge();	// Decide which battery to be charge
	if (IsFlag1(BAT_MISC3, NoProcess_bat)) {return;}	// Need handle battery?

	WakeUpCharge();				// Try to recover gauge power
	if (IsFlag0(SEL_CHARGE_STATUS, bat_wakeup))
	{
		UpdateBatteryStatus();
		return;
	}
	if ((IsFlag0(POWER_STAT_CTRL1, CHG_Status)) || (IsFlag0(POWER_STAT_CTRL1, AC_Status)))
	{
		StopCharging();
		return;
	}

	#ifdef	NECP_BattRefresh_Support
	if (IsFlag1(SEL_BAT_MISC, bat_StopChg))
	{
		StopCharging();
		return;
	}
	if (IsFlag1(NECBattRefreshPSTS, ForceDischgDoing))
	{
		UpdateBatteryStatus();
		return;
	}
	#endif	//NECP_BattRefresh_Support

	#ifdef	BattChg_SMB_OVS
	#ifdef	BattChg_SMBtoTBLtoDAC
	#ifdef	OEM_NECP
	if (IsFlag1(NECOverChgCurrent, StopChgCurOVSbyBatt))
	{
		NECStopCharging();
		return;
	}
	#endif	//OEM_NECP
	#endif	//BattChg_SMBtoTBLtoDAC

	#ifdef	BattChg_SMBtoCHGIC
	#ifdef	OEM_NECP
	if (IsFlag1(NECOverChgCurrent, StopChgCurOVSbyBatt))
	{
		NECStopCharging();
		return;
	}
	#endif	//OEM_NECP
	#endif	//BattChg_SMBtoCHGIC
	#endif	//BattChg_SMB_OVS

	#ifdef	Battery_Protection_NEC
	#ifdef	OEM_NECP
	if (IsFlag1(NECOverChgCurrent, C_First_USE_Date_Bit15_Set))
	{
		NECStopCharging();
		return;
	}
	#endif	//OEM_NECP
	#endif	//Battery_Protection_NEC

	#ifdef	OEM_NECP
	if (IsFlag1(NECOverChgCurrent, ForceStopChgOCC))
	{
		NECStopCharging();
		return;
	}
	#endif	//OEM_NECP

	if (IsFlag1(SEL_CHARGE_STATUS, bat_charge))	// Battery in charge
	{
		MonitorNormalChg();
		return;
	}
	PreCharge();
	UpdateBatteryStatus();
}


void CheckBatteryTemperature(void)
{
	CopyBatteryStatus();
	if (IsFlag1(SEL_BAT_STATUS, bat_in))
	{
		ClrFlag(SEL_CHARGE_STATUS, bat_overtemp);			// Assume temp: in range
		if (IsFlag1((SEL_BAT_ALARM >> 8), OverTemp_alarm))	// temp: in range
		{
			SetFlag(SEL_CHARGE_STATUS, bat_overtemp);		// Overheat
		}
		UpdateBatteryStatus();
	}
}


void ChooseBatteryToCharge(void)
{
	ClrFlag(BAT_MISC3, NoProcess_bat);	// Reset flag

	if ((IsFlag0(POWER_STAT_CTRL, adapter_in)) || (IsFlag1(BAT_MISC1, STOP_charge)))
	{	// AC not exist or charger disable
		NoChargeBAT();
		return;
	}

	if ((IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_destroy)) && (IsFlag0(ACHARGE_STATUS, abat_wakeup)))
	{	// ABAT exist and battery no fail and gauge no working
		ChooseABAT();
		return;
	}

	if ((IsFlag1(BBAT_STATUS, bbat_in)) && (IsFlag0(BBAT_STATUS, bbat_destroy)) && (IsFlag0(BCHARGE_STATUS, bbat_wakeup)))
	{	// BBAT exist and battery no fail and gauge no working
		ChooseBBAT();
		return;
	}

	if ((IsFlag1(ABAT_MISC, force_discA)) || (IsFlag1(BBAT_MISC, force_discB)))	// any discharge request?
	{
		NoChargeBAT();
		return;
	}

	if ((IsFlag0(BBAT_MISC, force_chgB)) && (IsFlag1(ABAT_STATUS, abat_in)) && (IsFlag0(ABAT_STATUS, abat_destroy)) && (IsFlag0(ABAT_STATUS, abat_full)))
	{	// BBAT charge not requested and ABAT exist and ABAT not abnormal and ABAT not full
		if (IsFlag0(ABAT_MISC, abat_ValidFull))		// if ValidFull not set
		{
			if (IsFlag1(POWER_STAT_CTRL1, AC_Status))
			{
				SetFlag(ABAT_MISC, force_chgA);			// set charge
				ClrFlag(BBAT_MISC, force_chgB);
				ChooseABAT();
			}
			return;
		}
		else
		{
			if (RSOC_FULL_BASE>ABAT_CAP)			// if Valid Full set check if RSOC < RSOC_FULL_BASE
			{
				if (IsFlag1(POWER_STAT_CTRL1, AC_Status))
				{
					SetFlag(ABAT_MISC, force_chgA);		// set charge
					ClrFlag(BBAT_MISC, force_chgB);
					ChooseABAT();
				}
				return;
			}
		}
	}

	if ((IsFlag0(BBAT_STATUS, bbat_in)) || (IsFlag1(BBAT_STATUS, bbat_destroy)) || (IsFlag1(BBAT_STATUS, bbat_full)))
	{	// BBAT not exist or BBAT abnormal or BBAT fully charged
		NoCharge1();
		return;
	}

	SetFlag(BBAT_MISC, force_chgB);
	ClrFlag(ABAT_MISC, force_chgA);
	ChooseBBAT();
}


void NoChargeBAT(void)
{
	//David modify for charge IC ISL88732 2014/05/21
	#ifdef	SUPPORT_BQ24780S	// leox20150607
	SetChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
	#else	// (original)
	ClearChargeControlCHGIC(Charge_Enable);
	//SetChargeCurrent0mA();
	#endif
	//David modify for charge IC ISL88732 2014/05/21
	SetFlag(BAT_MISC3, NoProcess_bat);
}


void ChooseABAT(void)
{
	ClrFlag(BBAT_STATUS, bbat_dischg);
	ClrFlag(BCHARGE_STATUS, bbat_charge + bbat_prechg + bbat_WakeChg + bbat_InCharge + bbat_level3);
	SetFlag(SYS_FLAG, main_sel);
	CopyBatteryStatus();
}


void ChooseBBAT(void)
{
	ClrFlag(ABAT_STATUS, abat_dischg);
	ClrFlag(ACHARGE_STATUS, abat_charge + abat_prechg + abat_WakeChg + abat_InCharge + abat_level3);
	ClrFlag(SYS_FLAG, main_sel);
	CopyBatteryStatus();
}


void NoCharge1(void)
{
	ClrFlag(ABAT_MISC, force_chgA);
	ClrFlag(BBAT_MISC, force_chgB);
	NoChargeBAT();
}


void WakeUpCharge(void)
{
	if (IsFlag1(SEL_CHARGE_STATUS, bat_wakeup))
	{	// Gauge already working
		ClrFlag(SEL_CHARGE_STATUS, bat_WakeChg);	// Stop wakeup charge
		return;
	}

	if (IsFlag0(SEL_CHARGE_STATUS, bat_WakeChg))
	{
		CHG_TIMER = 0;	// Initial wakeup charge
		TurnOnWakeUpCharge();
		return;
	}

	CHG_TIMER++;
	SetWakeUpChargeCurrent();
	if (CHG_TIMER >= WAKECHG_TIMEOUT) {BatteryFail(1);}
}


void TurnOnWakeUpCharge(void)
{
	SetFlag(SEL_CHARGE_STATUS, bat_WakeChg + bat_InCharge);
	SetWakeUpChargeCurrent();
	TurnOnCharger();
}


void SetWakeUpChargeCurrent(void)
{
	#ifdef	BattChg_TBLtoDAC
	SetChargeCurrentDAC(WAKEUP_CURRENT);	// 300mA
	#endif	//BattChg_TBLtoDAC

	#ifdef	BattChg_SMBtoTBLtoDAC
	SetChargeCurrentDAC(WAKEUP_CURRENT);	// 300mA
	#endif	//BattChg_SMBtoTBLtoDAC

	#ifdef	BattChg_SMBtoCHGIC
	//David modify for battery charge voltage 2014/05/21
	//David add for FJ request change battery CV 2016/11/08
	SetChargeVoltageCHGIC(12400);			// 12480mV
	//David add for FJ request change battery CV 2016/11/08
	//David modify for battery charge voltage 2014/05/21
		#ifdef	SUPPORT_BQ24780S	// leox20150607
	ClrChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
		#else	// (original)
	SetChargeControlCHGIC(Charge_Enable);
		#endif
	SetChargeCurrentCHGIC(256);				// 512mA
	#endif	//BattChg_SMBtoCHGIC
}


void StopCharging(void)
{
	ClrFlag(SEL_BAT_MISC, bat_force_chg + bat_force_disc + bat_next + bat_inUSE);
	ClrFlag(SEL_BAT_STATUS, bat_dischg);
	ClrFlag(SEL_CHARGE_STATUS, bat_level3);
	TurnOffCharger();
	UpdateBatteryStatus();
}


void MonitorNormalChg(void)
{
	int Normal_Charge_PASS;

	#ifdef	NECP_BattRefresh_Support
	if ((IsFlag0(NECBattRefreshPSTS, CHGC)) && (IsFlag1(NECBattRefreshPSTS, ADPC)))
	{
		UpdateBatteryStatus();
		return;
	}
	#endif	//NECP_BattRefresh_Support

	Normal_Charge_PASS = NormalCharge();
	if (Normal_Charge_PASS == 0)
	{
		//David modify for charge IC ISL88732 2014/06/18
		#ifdef	SUPPORT_BQ24780S	// leox20150607
		ClrChargeOptionsCHGIC(ChgIC_ChargeOptions0, ChgOpt0_CHRG_INHIBIT);
		#else	// (original)
		SetChargeControlCHGIC(Charge_Enable);
		#endif
		//David modify for charge IC ISL88732 2014/06/18
		SetChargeNormalCurrent();
	}
	UpdateBatteryStatus();
}


int NormalCharge(void)
{
	if (OverHeatProtect())	// Battery over temperature?
	{
		return(1);
	}
	else
	{
		if (TimerProtect())	// Timeout?
		{
			return(1);
		}
		else
		{
			if (OverVoltageProtect())		// Over voltage?
			{
				return(1);
			}
			else
			{
				if (FullCapacityDetector())	// Capacity = 100%?
				{
					return(1);
				}
				else
				{
					if (IsFlag1(SEL_CHARGE_STATUS, bat_NiMH))
					{	// Type is NiMH
						return(CheckNiFull());
					}
					else
					{	// Type is Li-Ion
						if (RSOC_FULL_BASE <= SEL_BAT_CAP)
						{
							return(CheckLiFull());
						}
						else
						{
							return(0);
						}
					}
				}
			}
		}
	}
}


int OverHeatProtect(void)
{
	int TrueState = 0;

	if (IsFlag0(SEL_CHARGE_STATUS, bat_overtemp))
	{
		if (IsFlag0(BAT_MISC3, charger_on)) {TurnOnCharger();}
	}
	else
	{
		if (IsFlag0(BAT_MISC3, charger_on))
		{
			TrueState = 1;
		}
		else
		{
			TurnOffCHGPin();
			if (IsFlag0(SEL_CHARGE_STATUS, bat_NiMH))
			{
				TrueState = 1;
			}
			else
			{
				OVERTEMP_COUNT++;
				if (RSOC_FULL_BASE <= SEL_BAT_CAP)
				{
					if (ChkChargeFullBit())
					{
						TrueState = 1;
					}
					else
					{
						if (OVERTEMP_COUNT >= MaxOverHeatTimes)
						{
							TrueState = BatteryFull(3);
						}
						else
						{
							TrueState = 1;
						}
					}
				}
				else
				{
					TrueState = 1;
				}
			}
		}
	}

	return(TrueState);
}


int TimerProtect(void)
{
	int TrueState = 0;

	if ((ABAT_MODEL != 0) || (BBAT_MODEL != 0))
	{
		CHG_TIMER++;
		if (IsFlag0(POWER_FLAG, power_on))
		{
			if (CHG_TIMER >= (Bat_ChargeTimeout / Bat_PowerOffTimerRatio)) {BatteryFail(8);}
		}
		else
		{
			if (CHG_TIMER >= (Bat_ChargeTimeout / Bat_PowerOnTimerRatio)) {BatteryFail(8);}
		}
	}

	return(TrueState);
}


int OverVoltageProtect(void)
{
	int TrueState = 0;

	if ((ABAT_MODEL != 0) || (BBAT_MODEL != 0))
	{
		if (SEL_BAT_VOLT >= Bat_OverVoltThreshold)
		{
			OverVoltageCNT++;
			if (OverVoltageCNT == 25) {BatteryFail(6);}
		}
		else
		{
			OverVoltageCNT = 0;
		}
	}

	return(TrueState);
}


int FullCapacityDetector(void)
{
	int TrueState = 0;

	if (IsFlag1(SEL_BAT_MISC, bat_ValidFull))
	{
		if (SEL_BAT_CAP >= RSOC_FULL_BASE)
		{
			TrueState = BatteryFull(8);
		}
		else
		{
			ClrFlag(SEL_BAT_MISC, bat_ValidFull);
		}
	}

	return(TrueState);
}


int CheckNiFull(void)
{
	if (RSOC_FULL_BASE <= SEL_BAT_CAP)
	{
		return(ChkBattFullFlag());
	}
	else
	{
		return(0);
	}
}


int ChkBattFullFlag(void)
{
	//David modify charge limit function 2014/07/02
		if (IsFlag0(ChargeLimit_CTRL, Charge_Limit_En))
	{
		if ((IsFlag1((SEL_BAT_ALARM >> 8), OverChg_alarm)) || (IsFlag1(SEL_BAT_ALARM, FullyChg)))
		{
			return(BatteryFull(12));
		}
		else
		{
			return(0);
		}
	}
	else
	{
		if (SEL_BAT_CAP >= 80)
		{
			return(BatteryFull(12));
		}
		else
		{
			return(0);
		}
	//David modify charge limit function 2014/07/02
	}
}


int CheckLiFull(void)
{
	//David modify charge limit function 2014/07/02
	if (IsFlag0(ChargeLimit_CTRL, Charge_Limit_En))
	{
		if ((IsFlag1((SEL_BAT_ALARM >> 8), OverChg_alarm)) || (IsFlag1(SEL_BAT_ALARM, FullyChg)))
		{
			return(BatteryFull(4));
		}
		else
		{
			return(0);
		}
	}
	else
	{
		if (SEL_BAT_CAP >= 80)
		{
			return(BatteryFull(12));
		}
		else
		{
			return(0);
		}
	//David modify charge limit function 2014/07/02
	}
}


int ChkChargeFullBit(void)
{
	int BatFullalarm = 0;
	//David modify charge limit function 2014/07/02
	if (IsFlag0(ChargeLimit_CTRL, Charge_Limit_En))
	{
		if (IsFlag1(SEL_BAT_ALARM, FullyChg)) {BatFullalarm = BatteryFull(10);}
	}
	else
	{
		if (SEL_BAT_CAP >= 80) {BatFullalarm = BatteryFull(10);}
	}
	//David modify charge limit function 2014/07/02
	return(BatFullalarm);
}


int BatteryFull(int BattCutoffCause)
{
	CUTOFF_CAUSE = BattCutoffCause;
	TurnOffCharger();
	SetFlag(SEL_BAT_STATUS, bat_full);
	ClrFlag(SEL_BAT_MISC, bat_force_chg);
	SetFlag(SEL_BAT_MISC, bat_ValidFull);
	ClrFlag(POWER_STAT_CTRL1, lowpower_CHG);
	Set100Percent();
	return(1);
}


void Set100Percent(void)
{
	//David modify charge limit function 2014/07/02
	if (IsFlag0(ChargeLimit_CTRL, Charge_Limit_En))
		return;
	//David modify charge limit function 2014/07/02
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_FullChargeCapacity;
	// David_20120106 modify for vOSSMbusRWord/vOSSMbusRWord_OEM routine
	if (vOSSMbusRWord_OEM())
	{
		DESIGN_FULL_CAPACITY = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
		Get1P();
		if (IsFlag1(SYS_FLAG, main_sel))
		{
			BT1C = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
			ABAT_CAP = 100;
		}
		else
		{
			BT2C = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
			BBAT_CAP = 100;
		}
	}
}


void ChkISL6251ACHICWaitVinCNT(void)
{
	if (IsFlag0(SYS_MISC2, CHGpinHigh))
	{
		ISL6251ACHICWaitVinCNT = 0;
	}
	if (ISL6251ACHICWaitVinCNT != 0)
	{
		ISL6251ACHICWaitVinCNT--;
		if (ISL6251ACHICWaitVinCNT == 0) {SetCHG_Low();}
	}
}


void SetCHG_Low(void)
{
	ACHG_on();
	ClrFlag(SYS_MISC2, CHGpinHigh);
}


#ifdef SUPPORT_BQ24780S	// leox20150605
void SetChargeOptionsCHGIC(BYTE Reg, WORD Val)
{
	XBYTE TmpH, TmpL;
	SMB_ADDR_OEM = SMBCHR_A;
	SMB_CMD_OEM  = Reg;
	if (vOSSMbusRWord_OEM())
	{
		TmpH = (Val >> 8) & 0xFF;
		TmpL = Val & 0xFF;
		if (((SMB_DATA1_OEM & TmpH) != TmpH) || ((SMB_DATA_OEM & TmpL) != TmpL))
		{
			SMB_DATA1_OEM |= TmpH;
			SMB_DATA_OEM  |= TmpL;
			vOSSMbusWWord_OEM();
		}
	}
	if (Val & ChgOpt0_EN_LEARN) {LearnModeClrCnt = 0;}	// leox20150915
}


void ClrChargeOptionsCHGIC(BYTE Reg, WORD Val)
{
	XBYTE TmpH, TmpL;
	SMB_ADDR_OEM = SMBCHR_A;
	SMB_CMD_OEM  = Reg;
	if (vOSSMbusRWord_OEM())
	{
		TmpH = (Val >> 8) & 0xFF;
		TmpL = Val & 0xFF;
		if (((SMB_DATA1_OEM & TmpH) != 0x00) || ((SMB_DATA_OEM & TmpL) != 0x00))
		{
			SMB_DATA1_OEM &= ~TmpH;
			SMB_DATA_OEM  &= ~TmpL;
			vOSSMbusWWord_OEM();
		}
	}
	if (Val & ChgOpt0_EN_LEARN) {LearnModeSetCnt = 0;}	// leox20150915
}


#else	// (original)
//David add for charge IC ISL88732 2014/05/21
void SetChargeControlCHGIC(int ChargeControlCHGIC)
{
	SMB_ADDR_OEM = SMBCHR_A;
	SMB_CMD_OEM = ChgIC_Control;
	if (vOSSMbusRWord_OEM())
	{
		SMB_DATA_OEM = 0;
		if (IsFlag0(SMB_DATA1_OEM, ChargeControlCHGIC))
		{
			SetFlag(SMB_DATA1_OEM, ChargeControlCHGIC);
			vOSSMbusWWord_OEM();
		}
	}
}

void ClearChargeControlCHGIC(int ChargeControlCHGIC)
{
	SMB_ADDR_OEM = SMBCHR_A;
	SMB_CMD_OEM = ChgIC_Control;
	if (vOSSMbusRWord_OEM())
	{
		SMB_DATA_OEM = 0;
		if (IsFlag1(SMB_DATA1_OEM, ChargeControlCHGIC))
		{
			ClrFlag(SMB_DATA1_OEM, ChargeControlCHGIC);
			vOSSMbusWWord_OEM();
		}
	}
}
//David add for charge IC ISL88732 2014/05/21
#endif	// SUPPORT_BQ24780S


//David add for read battery cycle count 2014/06/11
void PollingCycle(void)
{
	SMB_ADDR_OEM = _SMB_BatteryAddr;
	SMB_CMD_OEM = _CMD_CycleCount;
	if (vOSSMbusRWord_OEM())
	{
		BTCyCle = ((SMB_DATA1_OEM << 8) + SMB_DATA_OEM);
	}
}
//David add for read battery cycle count 2014/06/11

#ifdef	Hybrid_Turbo_Boost
void LoadHybrid_Limit(void)
{
	CmpEnHybrid_Limit = VICMV65WHiEnHybridLimit;
	CmpDisHybrid_Limit = VICMV65WLoDisHybridLimit;
	if (IsFlag1(Device_STAT_CTRL1, VGA_Status))
	{
		CmpEnHybrid_Limit = VICMV90WHiEnHybridLimit;
		CmpDisHybrid_Limit = VICMV90WLoDisHybridLimit;
	}
	else
	{
		CmpEnHybrid_Limit = VICMV65WHiEnHybridLimit;
		CmpDisHybrid_Limit = VICMV65WLoDisHybridLimit;
	}
	#if CHARGER_BQ24780S_DBG	// leox20150617 Add for charger IC debug		// leox20150810
	if (CmpEnHybrid_LimitD != 0) {CmpEnHybrid_Limit  = CmpEnHybrid_LimitD;}
	if (CmpDsHybrid_LimitD != 0) {CmpDisHybrid_Limit = CmpDsHybrid_LimitD;}
	#endif
}

void Hybrid_Status_Control(void)
{
	BYTE HybridStateFlag;

	if ((IsFlag0(Hybrid_Status, Hybrid_En)) || (IsFlag0(POWER_FLAG, power_on)) || (IsFlag0(POWER_STAT_CTRL, adapter_in)) || (IsFlag0(SEL_CHARGE_STATUS, bat_wakeup)))
	{
//		ClearChargeControlCHGIC(Boost_Enable);	// leox20150618 Disabled because it will be executed in BoostModeControl()
		Hybrid_Status &= 0x01;
		return;
	}
	else
	{
		LoadHybrid_Limit();
		// leo_20150916 add power protect +++
		#ifdef OEM_VICMV
		Get_VICMVoltage();
		if (VICM_V < CmpDisHybrid_Limit)
		#else
		Get_VICMVoltageACDC();
		if (VICM_VACDC < CmpDisHybrid_Limit)
		#endif
		// leo_20150916 add power protect ---
		{
//			ClearChargeControlCHGIC(Boost_Enable);	// leox20150618 Disabled because it will be executed in BoostModeControl()
			// leox20150618 Modify hybrid status control +++
			//Hybrid_Status &= 0x01;
			ClrFlag(Hybrid_Status, Hybrid_State);
			//return;
			// leox20150618 Modify hybrid status control ---
		}

		HybridStateFlag = Hybrid_Status & 0xFE;
		switch(HybridStateFlag)
		{
			case 0x00:		// Check if need turn on hybrid
				// leo_20150916 add power protect +++
				#ifdef OEM_VICMV
				if ((VICM_V > CmpEnHybrid_Limit) && (BIOS_ABAT_CAP >= 70))
				#else
				if ((VICM_VACDC > CmpEnHybrid_Limit) && (BIOS_ABAT_CAP >= 70))
				#endif
				// leo_20150916 add power protect ---
					SetFlag(Hybrid_Status, Hybrid_State);
				break;
			case 0x02:		// Check if need to stop recharge
				if (BIOS_ABAT_CAP >= 70)
				{
					ClrFlag(Hybrid_Status, Hybrid_Recharge);
				}
				break;
			case 0x04:		// Check if need to start recharge
				if (BIOS_ABAT_CAP <= 15)
				{
					SetFlag(Hybrid_Status, Hybrid_Recharge);
					ClrFlag(Hybrid_Status, Hybrid_State);
				}
				break;
			case 0x08:		// Check if need do recharge when AC insert
				if (BIOS_ABAT_CAP < 70)
				{
					Hybrid_Status &= 0x01;
					SetFlag(Hybrid_Status, Hybrid_Recharge);
				}
				else
				{
					Hybrid_Status &= 0x01;
					SetFlag(Hybrid_Status, Hybrid_State);
				}
				break;
			default:
				Hybrid_Status &= 0x01;
				break;
		}
	}
}

void BoostModeControl(void)
{
	if(IsFlag1(Hybrid_Status, Hybrid_State))
	{
		#ifdef	SUPPORT_BQ24780S	// leox20150607
		SetChargeOptionsCHGIC(ChgIC_ChargeOptions3, ChgOpt3_EN_BOOST);
		#else	// (original)
		SetChargeControlCHGIC(Boost_Enable);
		#endif
		SetChargeCurrentCHGIC(ChargingCurrent_Backup);
	}
	else
	{
		#ifdef	SUPPORT_BQ24780S	// leox20150607
		ClrChargeOptionsCHGIC(ChgIC_ChargeOptions3, ChgOpt3_EN_BOOST);
		#else	// (original)
		ClearChargeControlCHGIC(Boost_Enable);
		#endif
	}
}

#endif	//Hybrid_Turbo_Boost

void Round_Off_RSOC(void)
{
	BYTE RSOC;
	LWORD RM,FCC;

	RM = BT1C;
	FCC = DESIGN_FULL_CAPACITY;

	if ((FCC != 0) && (RM != 0) && (RM <= FCC))
	{
		RSOC = (RM * 100) / FCC;
		if((RM * 100 - FCC * RSOC) >= (FCC / 2))
			RSOC = RSOC+1;
		SMB_DATA_OEM = RSOC;
	}
}


#ifdef SUPPORT_BQ24780S	// leox20150605
const sInitChargerReg code BQ24780SInitTbl[] =
{
	{0x12, 0x6118},	// ChargeOptions0 Register
	{0x3B, 0x4E80},	// ChargeOptions1 Register
	{0x38, 0x0080},	// ChargeOptions2 Register									// leox20150608 0x0384 -> 0x0080
	{0x37, 0x8A40},	// ChargeOptions3 Register		// With adapter
	{0x3C, 0x2A56},	// ProchotOption0 Register		// For 65W adapter
	{0x3D, 0x4638},	// ProchotOption1 Register
//	{0x3A, 0xXXXX},	// ProchotStatus  Register		// (Read only)
//	{0x14, 0x0000},	// Charge Current Register
//	{0x15, 0x0000},	// Charge Voltage Register
	{0x3F, 0x0C00},	// Input Current  Register		// For 65W adapter			// leox20150608 0xEC00 -> 0x0C00
	{0x39, 0x2200},	// Discharge Current Register	// For Samsung battery
};

void BQ24780S_Init(void)
{
	BYTE i = 0;

	while (i < (sizeof(BQ24780SInitTbl) / sizeof(sInitChargerReg)))
	{
		SMB_ADDR_OEM  = SMBCHR_A;
		SMB_CMD_OEM   = BQ24780SInitTbl[i].Reg;
		SMB_DATA_OEM  = (BQ24780SInitTbl[i].Val) & 0xFF;
		SMB_DATA1_OEM = (BQ24780SInitTbl[i].Val >> 8) & 0xFF;
		if (vOSSMbusWWord_OEM() == FALSE) {return;}
		i++;
	}

	SetFlag(BAT_MISC3, BQ24780S_Init_OK);	// leox20150824
	SetFlag(BAT_MISC3, BQ24780S_Set_Reg);	// leox20150901
}


#if CHARGER_BQ24780S_DBG		// leox20150617 Add for charger IC debug		// leox20150810
//                               0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12
const WORD code ChgIcAdr[] = {0x12, 0x3B, 0x38, 0x37, 0x3C, 0x3D, 0x3A, 0x14, 0x15, 0x3F, 0x39, 0xFE, 0xFF};
// Read and save to 0x040X    0400, 0402, 0404, 0406, 0408, 040A, 040C, 040E, 0410, 0412, 0414, 0416, 0418
// Write data using 0x042X    0420, 0422, 0424, 0426, 0428, 042A, 042C, 042E, 0430, 0432, 0434, 0436, 0438
#endif
void ChgICDbgFunc(void)
{	// leox20150623 From Service_T100mSEC1()
	#if CHARGER_BQ24780S_DBG	// leox20150617 Add for charger IC debug +++	// leox20150810
	// Read/Write all registers
	if (ChgIcCnt != 0xFF)
	{
		if (ChgIcCnt > 12) {ChgIcCnt = 0;}
		bRWSMBus(SMbusCh1, SMbusRW, SMBCHR_A, ChgIcAdr[ChgIcCnt], &SMB_DATA_OEM, SMBus_NoPEC);
		ChgIcRegR[ChgIcCnt] = (SMB_DATA1_OEM << 8) + SMB_DATA_OEM;
		#if 1	// leox20150623 Add for wirte charger IC +++
		if ((ChgIcCnt <= 10) && ((ChgIcRegF & (1 << ChgIcCnt)) != 0) && (ChgIcRegW[ChgIcCnt] != ChgIcRegR[ChgIcCnt]))	// leox20160317 Charger debug
		{
			if (ChgIcAdr[ChgIcCnt] != 0x3A)			// leox20160317 Skip read only registers
			{
				SMB_DATA1_OEM = (ChgIcRegW[ChgIcCnt] >> 8) & 0xFF;
				SMB_DATA_OEM  = (ChgIcRegW[ChgIcCnt]) & 0xFF;
				bRWSMBus(SMbusCh1, SMbusWW, SMBCHR_A, ChgIcAdr[ChgIcCnt], &SMB_DATA_OEM, SMBus_NoPEC);
			}
		}
		#endif	// leox20150623 Add for wirte charger IC ---
		ChgIcCnt++;
	}
	#endif	// leox20150617 Add for charger IC debug ---
}


// leox20160317 Fix bug	// leox20160317 Skip read only registers	// leox20160317 Charger debug
// leox20160317 Rewrite function to fix bug, skip read only registers and charger debug
void Hook_vOSSMbusWWord_OEM(void)
{
	#if CHARGER_BQ24780S_DBG	// leox20150623 Add for wirte charger IC +++	// leox20150810
	if (SMB_ADDR_OEM != SMBCHR_A) {return;}
	for (ChgIcTmp = 0; ChgIcTmp <= 10; ChgIcTmp++)
	{
		if ((SMB_CMD_OEM == ChgIcAdr[ChgIcTmp]) && ((ChgIcRegF & (1 << ChgIcTmp)) != 0))
		{
			if (ChgIcAdr[ChgIcTmp] != 0x3A)
			{
				SMB_DATA1_OEM = (ChgIcRegW[ChgIcTmp] >> 8) & 0xFF;
				SMB_DATA_OEM  = (ChgIcRegW[ChgIcTmp]) & 0xFF;
			}
			break;
		}
	}
	#endif	// leox20150623 Add for wirte charger IC ---
}


// leox20150901 To set register according to full charged capacity
// leox20150907 Rewrite function to modify register by battery product number
// leox20150907 BQ24780S_SetRegByFullCap() -> BQ24780S_SetRegByPN()
// leox20150911 Rewrite function with structure SetChargerReg
const sSetChargerReg code BQ24780SSetTbl[] =
{	// 0x3D,   0x39  	//  #   Battery_PN	// The order the same as BatCfgTbl[]
	{0x0000, 0x0000},	//  0   "        "
	{0x3E38, 0x2400},	//  1   "********"
	{0x4E38, 0x2600},	//  2   "CP671398"
	{0x4638, 0x2200},	//  3   "CP656340"
	{0x4E38, 0x2600},	//  4   "CP671396"
	{0x4638, 0x2200},	//  5   "CP651529"
	{0x3638, 0x1A00},	//  6   "CP656728"
	{0x3638, 0x1A00},	//  7   "CP656352"
	{0x3638, 0x1800},	//  8   "CP652728"
	{0x3638, 0x1800},	//  9   "CP671395"
	{0x4E38, 0x2600},	// 10   "CP700280"
	{0x3638, 0x1800},	// 11   "CP700278"	// leox20151211 Bug fixed by Leo Liu
	{0x4E38, 0x2600},	// 12   "CP700282"
	{0x4638, 0x2200},	// 13   "CP708752"
};

void BQ24780S_SetRegByPN(void)
{
	if (IsFlag1(BAT_MISC3, BQ24780S_Set_Reg) && IsFlag1(BAT_MISC3, BQ24780S_Init_OK) &&
		IsFlag1(SEL_BAT_STATUS, bat_in) && (SEL_BAT_MODEL != 0) &&
		(IsFlag1(POWER_STAT_CTRL, adapter_in) || IsFlag1(POWER_FLAG, power_on)))
	{
		if (SEL_BAT_MODEL >= GetArrayNum(BQ24780SSetTbl)) {return;}

		SMB_ADDR_OEM  = SMBCHR_A;
		SMB_CMD_OEM   = 0x3D;
		SMB_DATA1_OEM = (BQ24780SSetTbl[SEL_BAT_MODEL].Reg3D >> 8) & 0xFF;
		SMB_DATA_OEM  = (BQ24780SSetTbl[SEL_BAT_MODEL].Reg3D) & 0xFF;
		if (vOSSMbusWWord_OEM() == FALSE) {return;}

		SMB_ADDR_OEM  = SMBCHR_A;
		SMB_CMD_OEM   = 0x39;
		SMB_DATA1_OEM = (BQ24780SSetTbl[SEL_BAT_MODEL].Reg39 >> 8) & 0xFF;
		SMB_DATA_OEM  = (BQ24780SSetTbl[SEL_BAT_MODEL].Reg39) & 0xFF;
		if (vOSSMbusWWord_OEM() == FALSE) {return;}

		ClrFlag(BAT_MISC3, BQ24780S_Set_Reg);
	}
}
#endif	// SUPPORT_BQ24780S


#endif	//OEM_BATTERY
