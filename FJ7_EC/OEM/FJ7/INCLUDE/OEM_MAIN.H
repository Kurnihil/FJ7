/*------------------------------------------------------------------------------
 * Title : OEM_MAIN.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_MAIN_H
#define	OEM_MAIN_H


//------------------------------------------------------------------------------
// Debounce Time Setting
//------------------------------------------------------------------------------
#define	T_PSW_DEBOUNCE		10	// 100 ms (timebase: 10ms)
#define	T_AC_DEBOUNCE		4	// 80 ms (timebase: 20ms)
#define	T_BAT_DEBOUNCE		3	// 150 ms (timebase: 50ms)
#define	T_EXTEVT_DEBOUNCE	6	// 300 ms (timebase: 50ms)

#define	CheckPowerSwitch	0	// Debounce ID
#define	CheckACIn			1	// Debounce ID
#define	CheckBatteryIn		2	// Debounce ID
#define	CheckLIDIn			3	// Debounce ID


//------------------------------------------------------------------------------
// Common Definition
//------------------------------------------------------------------------------
#define	T_PSW_DELAY		3			// 30ms (timebase: 10ms)
#define	T_PSWOFF		(3800/10)	// 4sec (timebase: 4sec)
#define	RVcc2RSMRST		20			// 20ms (timebase: 1ms)
#define	T_MAINON_VRON	10			// 10ms
#define	T_MAINON_3G		3			// 30ms (timebase: 10ms)

#ifdef	DeepS3_SUPPORT				// 20120104 Chris add for support Deep S3
#define	RVcc2RSMRST_DS3	30			// 30ms (timebase: 1ms)		// leox_20120111 RVcc2RSMRST_DPS3 -> RVcc2RSMRST_DS3
#endif

#ifdef	OEM_Calpella
#define	T_HWPG			200			// 1000ms (timebase: 5ms)
#define	T_SWPG			20			// 100ms  (timebase: 5ms)
#else	//OEM_Calpella
#define	T_HWPG			100			// 100ms (timebase: 1ms)
#define	T_SWPG			10			// 10ms  (timebase: 1ms)
#endif	//OEM_Calpella

#define	T_APP_TASK_CNT	3			// 60ms (timebase: 20ms)

// Defines for volume control		// leox_20111014 SUPPORT_OEM_APP
#define	VOL_MAX			100			// Max volume value
#define	VOL_MIN			0			// Min volume value
#define	VOL_LVL			50			// Max volume level
#define	VOL_STEP		2			// ((VOL_MAX-VOL_MIN)/VOL_LVL)

#define	BAT_IN_H					341		// 3.15V
#define	USBCHARGEDThrldofcapacity	13		// leox20150709 11 -> 13 refer to "ApsalusV(FH9A)_FJ_check(20141222).xlsx"
#define	ForceDecPercent				6		// 3 percent

#ifdef	SUPPORT_BQ24780S			// leox20150607
#define	InputCurrentValue65W		0x0C00	// Input Current Limit Setting = 3.42A * 90% = 3.08A	// leo_20150916 add power protect
#define	InputCurrentValue90W		0x1000	// Input Current Limit Setting = X.XXA * XX% = X.XXA	// CHECK
#else	// (original)
#define	InputCurrentValue65W		3100/2	// 3072 mA by power team setting 65W
#define	InputCurrentValue90W		4200/2	// 4096 mA by power team setting 90W
#endif

#ifdef	LogP80InECRamDxx
#define	P80LEN			BIT7		// 1/0 : Enable/Disable latch P80 value
#endif	//LogP80InECRamDxx

#define	Delay500us()	Delay10plus3XuS(163)	// leox_20130717

#define	ECO_SLEEP_CHG	0			// ECO sleep charge point	// leox20150805 2 -> 0 for FH9C and FB5

//------------------------------------------------------------------------------
// DAC/PWM Output Setting
//------------------------------------------------------------------------------
#define	DA_CCSET		DACDAT0	// DAC Output Data 0	r/w b

#ifdef	FanByDAorPWM
#define	DAPWM_VFAN		DACDAT5	// DAC Output Data 5	r/w b	// leox20150522 FH9C
#define	DAPWM_VFAN2		DACDAT2	// DAC Output Data 3	r/w b	// leox_20120315 Support_FAN2
#else	//FanByDAorPWM
#define	DAPWM_VFAN		DCR6	// PWM6
#define	DAPWM_VFAN2		DCR7	// PWM7							// leox_20120315 Support_FAN2
#endif	//FanByDAorPWM

#ifdef	LCDPannelByDAorPWM
#define	DAPWM_VADJ		DACDAT3	// DAC Output Data 3	r/w b
#else	//LCDPannelByDAorPWM
#define	DAPWM_VADJ		DCR1	// PWM1
#endif	//LCDPannelByDAorPWM

// leox_20110920 Fan control value definitions +++
#define	VFAN_DA_MAX		255		// Maximum voltage
#define	VFAN_DA_MIN		90		// Minimum voltage 3.1V
#define	VFAN_DA_3V9		186		// 3.9V fan full speed run

#define	VFAN_PWM_MAX	CTR		// Maximum PWM duty (100%)
#define	VFAN_PWM_MIN	80		// Minimum PWM duty (40%)
#define	VFAN_PWM_35DB	166		// 35db fan full speed run
// leox_20110920 Fan control value definitions ---

#define	MaxLevel	11

//------------------------------------------------------------------------------
// Structure Declaration
//------------------------------------------------------------------------------
struct sDebounce
{
	XBYTE	*Reg;
	BYTE	Flag;
	XBYTE	*Counter;
	BYTE	Time;
	FUNCT_PTR_V_B	Func;	// leox_20130219
};

struct sButtons	// Add One Touch Buttons function 2011/08/19
{
	BYTE	Flag;
	FUNCT_PTR_V_B	Func;
};


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void Hook_EnableInterrupt(void);

extern void Hook_Timer1msEvent(void);							// leox_20110920 Remove parameter "EventId"
extern void Hook_Timer5msEventA(void);						// leox_20130311 Hook_Timer5msEvent() -> Hook_Timer5msEventA()
extern void Hook_Timer5msEventB(void);						// leox_20130311
extern void Hook_Timer5msEventC(void);						// leox_20130311
extern void Hook_Timer5msEventD(void);						// leox_20130311
extern void Hook_Timer5msEventE(void);						// leox_20130311

extern void Service_T5mSEC0(void);							// leox_20130311 Service_T5mSEC() -> Service_T5mSEC0()
extern void Service_T5mSEC1(void);							// leox_20130311
extern void Service_T5mSEC2(void);							// leox_20130311
extern void Service_T5mSEC3(void);							// leox_20130311
extern void Service_T5mSEC4(void);							// leox_20130311
extern void Service_T1SEC0(void);								// leox_20130311
extern void Service_T1SEC1(void);
extern void Service_T1SEC2(void);
extern void Service_T1SEC3(void);
extern void Service_T1SEC4(void);
extern void Service_T1SEC5(void);
extern void Service_T1SEC6(void);
extern void Service_T1SEC7(void);
extern void Service_T1SEC8(void);
extern void Service_T1SEC9(void);
extern void Service_T1SEC10(void);
extern void Service_T1SEC11(void);
extern void Service_T1SEC12(void);
extern void Service_T1SEC13(void);
extern void Service_T1SEC14(void);
extern void Service_T1SEC15(void);
extern void Service_T1SEC16(void);
extern void Service_T1SEC17(void);
extern void Service_T1SEC18(void);
extern void Service_T1SEC19(void);
extern void Service_T1SEC20(void);
extern void Service_T1SEC21(void);
extern void Service_T1SEC22(void);
extern void Service_T1SEC23(void);
extern void Service_T1SEC24(void);
extern void Service_T2SEC(void);
extern void Service_T3SEC(void);
extern void Service_T5SEC(void);
extern void Service_T10SEC(void);
extern void Service_T1Min(void);
extern void Service_T10mSEC(void);
extern void Service_T20mSEC0(void);							// leox_20130311 Service_T20mSEC() -> Service_T20mSEC0()
extern void Service_T20mSEC1(void);							// leox_20130311
extern void Service_T50mSEC(void);
extern void Service_T100mSEC0(void);							// leox_20130311 Service_T100mSEC() -> Service_T100mSEC0()
extern void Service_T100mSEC1(void);							// leox_20130311
extern void Service_T100mSEC2(void);							// leox_20130311
extern void Service_T500m0SEC(void);							// leox_20130311
extern void Service_T500m1SEC(void);
extern void Service_T500m2SEC(void);
extern void Service_T500m3SEC(void);
extern void Service_T500m4SEC(void);
extern void Service_T500m5SEC(void);
extern void Service_T500m6SEC(void);
extern void Service_T500m7SEC(void);
extern void Service_T500m8SEC(void);
extern void Service_T500m9SEC(void);							// leox_20130311

extern void RamDebug(unsigned char dbgcode);

extern void Delay10plus3XuS(BYTE Timer);
extern void CheckPSWDelay(void);
extern void ConfigACIn(void);
extern void ConfigACOut(void);
extern void ForceOffPower(void);
extern void LogShutdownCause(void);
extern void CommonStateChange(void);
extern void DCMODE(void);
extern void ACMODE(void);
extern void S0S3toS3S4S5OffPower(void);
extern void S0S3toS4S5OffPower(void);
#ifdef	DeepS3_SUPPORT	// 20120104 Chris add for support Deep S3
extern void S0S3toDPS3S4S5OffPower(void);
extern void S0toDPS3(void);
extern void S3toDPS3(void);
extern void DPS3toS3(void);
#endif	// DeepS3_SUPPORT
extern void DisablePS2Interrupt(void);
extern void EnablePS2Interrupt(void);
extern void InitQ20(void);
extern void CheckTurnOffSBLANPowerSource(void);
extern void TurnOffLanPower(void);
extern void TurnOffSBPowerSource(void);
extern void ResetSusBSusC(void);
extern void CheckPower(void);
extern void PSWOnPower(void);
extern void TurnOnSBPowerSource(void);
extern void TurnOnLanPower(void);
extern void PSWOffPower(void);
extern void LIDInserted(void);
extern void LIDRemoved(void);
extern void LIDNoChange(void);
extern void SendLIDQeventForBIOS(void);
extern void SendQLIDSWIForBIOS(void);
extern void SendQLIDSCIForBIOS(void);
extern void SendQLIDSMIForBIOS(void);
extern void ScanAnalogInput(void);
// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function ++
extern void IERx_BackUp(void);
extern void IERx_ReLoad(void);
// Ken_20111021 Add IERx_BackUp and IERx_ReLoad call-back function --
extern void HookChkIdle(void);
extern void EnterIdleMode(void);
extern void DelayTimer(void);
extern void ChkDISPON_Signal(void);
extern void ChkDLIDbyLID(void);
extern void CheckAPU_BLON(void);    // leo_20150518 add panel blon need wait APU_BLON
extern void CheckLAN_WP(void);
extern void GetSBMBsignals4PowerSequence(void);
extern void ChkSBMBsignals4PowerSequence(void);
extern void MonitorHWPG(void);
extern void PreparePWROK(void);
extern void LogP80(void);
extern void ConfigSysOnIO(void);
extern void InitSIOCnfg(void);
extern void OemPreparePWROK(void);
extern void OemInitPWM(void);
extern void OemInitialSysOnVariables(void);
extern void GetJumpper(void);
extern void UpDate_Brighness(void);
extern void InitialSysOnVariables(void);
extern void ChkSBsignals4PowerSequence(void);
extern void ChkTurnOnSystem(void);
extern void S0toS3(void);
extern void CheckUSB_Power_Need_CutOff(void);
extern void EnableUSBPort1(void);
extern void DisableUSBPort1(void);
extern void EnableUSBPort2(void);
extern void DisableUSBPort2(void);
extern void S0toS4S5(void);
extern void S4S5toS0(void);
extern void S3toS0(void);
extern void S3toS4S5(void);
extern void S0S3toS4S5(void);
extern void S3S4S5toS0(void);
extern void CopyRAM2OEMBIOSaccess(void);
extern void Setting_MAX8742ETI(void);
extern void SendDummySCI(void);
extern void PollAVFUNCTIONLED(void);
extern void CheckWLBTStatus(void);
extern void ChkECOLED(void);
extern void Check_Volume_CNT(void);
extern void CopyECRAM2BRAM(void);
extern void ChkBatteryOnlyXXPThroll(void);
extern void InitChkBatteryOnlyXXPThroll(void);
extern void BounceFromSBsignals00(void);
extern int  ReadFromSBsignals00(void);
extern void CheckUpdateFromSBsignals00(void);
extern void BounceFromMBsignals00(void);
extern int  ReadFromMBsignals00(void);
extern void CheckUpdateFromMBsignals00(void);
extern void wSwapHi_Low_Byte_for_copy(WORD *valueSorce, WORD *valueDest);
extern void PollOneTouchButtons(void);	// Add One Touch Buttons function 2011/08/19 by Fox Zhu
#if	SUPPORT_OEM_APP							// leox_20111014 Support OEM application
extern void OemAppFuncService(void);
extern void OemAppHookPort66Cmd81(void);
extern void SetVolumePre(BYTE act);
#endif	// SUPPORT_OEM_APP
extern void ECOSleepCheck(void);
extern void Throttling_3cell_Battery(void);
extern void EnterShipMode(void);
extern void PollingCriticalWake(void);
extern void PulsePwrBtnToSB(BYTE bMS);		// leox_20121115
//David_20141222 add for CMOS clear function
extern void CheckCMOSReset(void);
extern void ClearCMOSProgress(void);
//David_20141222 add for CMOS clear function
extern void ChkMEFW_OVERRIDE(void);
extern void Check_PROCHOT_Disable(void);	// leox20150803 Check before disable PROCHOT


#endif	/* OEM_MAIN_H */
