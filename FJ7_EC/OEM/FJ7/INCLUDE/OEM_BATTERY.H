/*------------------------------------------------------------------------------
 * Title : OEM_BATTERY.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_BATTERY_H
#define	OEM_BATTERY_H


//------------------------------------------------------------------------------
// Common Definition
//------------------------------------------------------------------------------
#define	_SMB_BatteryAddr			0x16
#define	_CMD_ManufacturerAccess		0x00
#define	_CMD_RemainingCapacityAlarm	0x01
#define	_CMD_RemainingTimeAlarm		0x02
#define	_CMD_BatteryMode			0x03
#define	_CMD_AtRate					0x04
#define	_CMD_AtRateTimeToFull		0x05
#define	_CMD_AtRateTimeToEmpty		0x06
#define	_CMD_AtRateOK				0x07
#define	_CMD_Temperature			0x08
#define	_CMD_Voltage				0x09
#define	_CMD_Current				0x0A
#define	_CMD_AverageCurrent			0x0B
#define	_CMD_MaxError				0x0C
#define	_CMD_RelativeStateOfCharge	0x0D
#define	_CMD_AbsoluteStateOfCharge	0x0E
#define	_CMD_RemainingCapacity		0x0F
#define	_CMD_FullChargeCapacity		0x10
#define	_CMD_RunTimeToEmpty			0x11
#define	_CMD_AverageTimeToEmpty		0x12
#define	_CMD_AverageTimeToFull		0x13
#define	_CMD_ChargingCurrent		0x14
#define	_CMD_ChargingVoltage		0x15
#define	_CMD_BatteryStatus			0x16
#define	_CMD_CycleCount				0x17
#define	_CMD_DesignCapacity			0x18
#define	_CMD_DesignVoltage			0x19
#define	_CMD_SpecificationInfo		0x1A
#define	_CMD_ManufactureDate		0x1B
#define	_CMD_SerialNumber			0x1C
#define	_CMD_ManufacturerName		0x20
#define	_CMD_DeviceName				0x21
#define	_CMD_DeviceChemistry		0x22
#define	_CMD_ManufacturerData		0x23
#define	_CMD_PackStatus				0x2F
//David_20120327 add for read Barcode No
#define	_CMD_Barcode_Sanyo			0x32
#define	_CMD_Barcode_Simplo			0x70
//David_20141222 add for FJ battery command
#define	_CMD_Barcode_Fujitsu		0x31
//David_20141222 add for FJ battery command
//David_20120327 add for read Barcode No
//David add for FJ request change battery CV 2016/11/08
#define	_CMD_EEPROM_Fujitsu			0x33
//David add for FJ request change battery CV 2016/11/08
#define	_CMD_First_USE_Date			0x36
#define	_CMD_VCELL4					0x3C
#define	_CMD_VCELL3					0x3D
#define	_CMD_VCELL2					0x3E
#define	_CMD_VCELL1					0x3F
#define	_CMD_VPACK					0x45
#define	_CMD_AFEData				0x46

#define	SMBCHR_A					0x12
#define	ChgIC_ChargingCurrent		0x14
#define	ChgIC_ChargingVoltage		0x15
#define	ChgIC_InputCurrent			0x3F

//David_20141222 add for charge IC ISL88732
#define	ChgIC_Control				0x44
#define	Quick_Boost					BIT1	// 1 Disable quick boost mode change; 0 Enable quick boost mode change
#define	Current_Monitor				BIT2	// 0 Adapter current monitor; 1 Battery current monitor
#define	Charge_Enable				BIT5	// 1 Enable; 0 Disable
#define	Boost_Enable				BIT6	// 1 Enable; 0 Disable
//David_20141222 add for charge IC ISL88732

#ifdef	SUPPORT_BQ24780S					// leox20150607
#define	ChgIC_ChargeOptions0		0x12
#define	ChgOpt0_EN_LEARN			0x0020	// BIT[5]: 0:Disable LEARN mode, 1:Enable
#define	ChgOpt0_CHRG_INHIBIT		0x0001	// BIT[0]: 0:Enable charge, 1:Disable

#define	ChgIC_ChargeOptions3		0x37
#define	ChgOpt3_EN_BOOST			0x0004	// BIT[2]: 0: Disable hybrid power boost mode, 1: Enable
#endif	// SUPPORT_BQ24780S

#define	dead_qualify				6		// 6 times (time base 1 sec)
#define	Warning_CAP_Li				12		// Issue battery warning
#define	Low_CAP_Li					6		// Issue battery low
#define	Warning_CAP_Ni				12		// Issue battery warning
#define	Low_CAP_Ni					6		// Issue battery low
#define	CriLow_CAP_Li				6		// Issue battery critical low
#define	CriLow_CAP_Hi				6		// Issue battery critical low

#define	BattWDG_Timeout				210		// Battery watchdog timeout (time base 1 sec)
#define	CHICWaitVinCNTValue			9		// 45 ms (time base 5ms)
#define	WAKECHG_TIMEOUT				1200	// 20 min (time base 1 sec)
#define	WAKEUP_CURRENT				15		// 300 mA
#define	MaxOverHeatTimes			3		// If overheat over 3 times battery full (time base 1 sec)

#define	ChgCurrLimitValue_M1		2176
#define	ChgCurrLimitValue_M2		2176
#define	ChgCurrMaxValue_M1			3072
#define	ChgCurrMaxValue_M2			3072
#define	ChgCurrMaxValue_M3			1536
#define	ChgCurrMaxValue_M4			1536

#define	OverVoltThreshold_13270mV	13270
#define	OverVoltThreshold_12900mV	12900

// leox20150605 Add for charge IC BQ24780S +++	// SUPPORT_BQ24780S
typedef struct InitChargerReg
{
	BYTE	Reg;	// Register	// Command
	WORD	Val;	// Value	// Data
} sInitChargerReg;
// leox20150605 Add for charge IC BQ24780S ---

// leox20150911 For set charge IC BQ24780S +++	// SUPPORT_BQ24780S
typedef struct SetChargerReg
{
	WORD	Reg3D;
	WORD	Reg39;
} sSetChargerReg;
// leox20150911 For set charge IC BQ24780S ---

// leox20150722 Battery configuration +++
typedef struct BatteryConfig
{
	BYTE	Name[8];				// Battery P/N (Ex: "CP671398")
	BYTE	Type;					// Cell type (0: LiIon, 1: NiMH)
	BYTE	Cell;					// Cell configuration (Ex: 0x32 means 3S2P)
	WORD	PreChargeTimeout;		// Pre-charge timeout (base: 1 sec) (costant) (Ex: 3600 means 60 minutes)
	WORD	PreChargeCurrent;		// Pre-charge current (costant) (Ex: 15 means 300mA)
	BYTE	FastChargeCurrent;		// Fast charge current (Ex: 63 means 1400mA)
	BYTE	PowerOnTimerRatio;		// Power on timer ratio (base: 1 sec) (costant)
	BYTE	PowerOffTimerRatio;		// Power off timer ratio (base: 1 sec) (costant)
	WORD	ChargeTimeout;			// Charge timeout (base: 1 sec) (costant) (Ex: 28880 means 8/4 hours for power on/off)
	WORD	OverVoltageThreshold;	// Over voltage threshold (Ex: 13270 means 13.27V)
	WORD	DeadChargeThreshold;	// Dead charge threshold (Ex: 9000 means 9.0V)
} sBatteryConfig;
// leox20150722 Battery configuration ---


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void GetBatteryModel(void);
extern void SetBatConfig(BYTE Model);			// leox20150722 Battery configuration
extern void RetryGetBatteryModel(void);
extern void GetBatteryModelAgain(void);
extern void RetryModelWithAC(void);
extern void CopyBatteryStatus(void);
extern BYTE SaveBatteryDeviceName(void);		// leox_20120403 Add return value
//David_20120327 add for read battery Barcode No and Manufacture Name
extern BYTE SaveBatteryBarcode(void);			// leox_20120403 Add return value
//David_20120327 add for read battery Barcode No and Manufacture Name
extern BYTE SaveBatterySerialNo(void);			// leox_20120403 Add return value
extern BYTE SaveBatteryManufacuteDate(void);	// leox_20120403 Add return value
//David add for FJ request change battery CV 2016/11/08
extern void CheckBatteryEEPROM(void);
//David add for FJ request change battery CV 2016/11/08
extern void ABATRemoved(void);
extern void MainBatteryOut(void);
extern void ResetBatteryDeviceName(void);
//David_20120327 add for read battery Barcode No and Manufacture Name
extern void ResetManufactureName(void);
extern void ResetBatteryBarcode(void);
//David_20120327 add for read battery Barcode No and Manufacture Name
extern void ResetBatterySerialNo(void);
extern void ResetBatteryManufactureDate(void);
extern void ResetReadWriteBattFirstDate(void);
extern BYTE SeveReadBattFirstUSEDate(void);	// leox_20120403 Add return value
extern void DoWriteBattFirstUSEDate(void);
extern void InitialBattery(void);
extern void ABATInserted(void);
extern void ABATNoChange(void);
extern void ConfigBatteryIn(void);
extern void AddBATVAR(void);
extern void Get1P(void);
extern void DoPollingJobs(void);
extern void ModifyRSOCvalue(void);
extern void ModifyRMValue(void);
extern void UpdateBatteryStatus(void);
extern void PollingBatteryData(void);
extern void ReadPollingData(void);

// Charging and discharging procedure
extern void PreCharge(void);
extern void TurnOnNormalCharge(void);
extern void SetChargeNormalCurrent(void);
extern void ChkGBCCsetValue(void);
extern void SetChgICChargingCurt(void);
extern void BatteryWeak(void);
extern void InitPrechg(void);
extern void TurnOnPreCharge(void);
extern void SetPreChargeCurrent(void);
extern void SetChargeCurrentDAC(int ChargeCurrentDAC);
extern void SetChargeCurrentCHGIC(int ChargeCurrentCHGIC);
extern void SetChargeVoltageCHGIC(int ChargeVoltageCHGIC);
extern void FCC_UpDate(void);
extern void ProcessAutoLearn(void);
extern void ALStepControl(void);
extern void ALStop(void);
extern void EndAutoLearn(void);
extern void ProcessDischarge(void);
extern void ForceDischarge(void);
extern void HandleDischarge(void);
extern void GetDischgCode(void);
extern void DischargeBBAT(void);
extern void EndProcDischg(void);
extern void DischargeABAT(void);
extern void DischgBatt(void);
extern void ChkBATDead(void);
extern void ChkBATDeadLi(void);
extern void CheckBattDead(void);
extern void EndDischgBAT(void);
extern void ConfirmBATDead(void);
extern void SETbxATDead(void);
extern void BATDead1(void);
extern void BATDead2(void);
extern void BATDead3(void);
extern void CheckCriticalLow(void);
extern void CheckCriticalLowLi(void);
extern void ChkCriticalLow(void);
extern void ChkWarningCap(void);
extern void ChkWarningCapLi(void);
extern void TurnOffDischarger(void);
extern void EndOffDisgr(void);
extern void FreeDischarge(void);
extern void BatteryFullyDischarge(void);
extern void CheckHighCapacity(void);
extern void TrickleCharge(void);
extern int  CheckLiReCharge(void);
extern void NiTrickleChargeConstCurrt(void);
extern void DecDutyCounterCCC(void);
extern void NiTrickleSetDuty(void);
extern void TurnOnTrickleChargeConstCurrt(void);
extern void SetTrickleChargeCurrent(void);
extern void OffChargeABT(void);
extern void OffAChgr(void);
extern void off_ABAT_HW(void);
extern void EndOffChgr1(void);
extern void TurnOffCHGPin(void);
extern void SetCHG_High(void);
extern void OffChargeBBT(void);
extern void OffBChgr(void);
extern void off_BBAT_HW(void);
extern void TurnOnDischarger(void);
extern void TurnOnADISCHG(void);
extern void TurnOnBDISCHG(void);
extern void TurnOffADISCHG(void);
extern void SetChargeCurrent0mA(void);
extern void SetChargeCurrentDAC0mA(void);
extern void SetChargeCurrentCHGIC0mA(void);
extern void TurnOnCharger(void);
extern void set_ACHG(void);
extern void set_BCHG(void);
extern void TurnOffBDISCHG(void);
extern void SetChgICChargingVoltage(void);
extern void SetCghICInputCurrent(void);
extern void ChkBattWDG(void);
extern void BatteryFail(int BattFailCause);
extern void TurnOffCharger(void);
extern void ProcessCharge(void);
extern void CheckBatteryTemperature(void);
extern void ChooseBatteryToCharge(void);
extern void NoChargeBAT(void);
extern void ChooseABAT(void);
extern void ChooseBBAT(void);
extern void NoCharge1(void);
extern void WakeUpCharge(void);
extern void TurnOnWakeUpCharge(void);
extern void SetWakeUpChargeCurrent(void);
extern void StopCharging(void);
extern void MonitorNormalChg(void);
extern int  NormalCharge(void);
extern int  OverHeatProtect(void);
extern int  TimerProtect(void);
extern int  OverVoltageProtect(void);
extern int  FullCapacityDetector(void);
extern int  CheckNiFull(void);
extern int  ChkBattFullFlag(void);
extern int  CheckLiFull(void);
extern int  ChkChargeFullBit(void);
extern int  BatteryFull(int BattCutoffCause);
extern void Set100Percent(void);
extern void ChkISL6251ACHICWaitVinCNT(void);
extern void SetCHG_Low(void);

//David_20141222 add for charge IC ISL88732
extern void SetChargeControlCHGIC(int ChargeControlCHGIC);
extern void ClearChargeControlCHGIC(int ChargeControlCHGIC);
extern void Hybrid_Status_Control(void);
extern void BoostModeControl(void);
//David_20141222 add for charge IC ISL88732

//David_20141222 add for polling battery cycle count
extern void PollingCycle(void);
//David_20141222 add for polling battery cycle count

//David_20141222 add for round off RSOC
extern void Round_Off_RSOC(void);
//David_20141222 add for round off RSOC

//David_20141222 add for support two adapter
extern void LoadHybrid_Limit(void);
//David_20141222 add for support two adapter

// leox20150607 Add for charge IC BQ24780S +++	// SUPPORT_BQ24780S
extern void BQ24780S_Init(void);
extern void BQ24780S_SetRegByPN(void);			// leox20150901	// leox20150907 BQ24780S_SetRegByFullCap() -> BQ24780S_SetRegByPN()
extern void SetChargeOptionsCHGIC(BYTE Reg, WORD Val);
extern void ClrChargeOptionsCHGIC(BYTE Reg, WORD Val);
extern void ChgICDbgFunc(void);					// leox20150623 CHARGER_BQ24780S_DBG
extern void Hook_vOSSMbusWWord_OEM(void);		// leox20150623 CHARGER_BQ24780S_DBG
// leox20150607 Add for charge IC BQ24780S ---

#endif	/* OEM_BATTERY_H */
