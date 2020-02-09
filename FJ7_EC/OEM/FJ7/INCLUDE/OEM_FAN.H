/*------------------------------------------------------------------------------
 * Title : OEM_FAN.H
 * Author: Ken Lee / Leox Lin
 *----------------------------------------------------------------------------*/


#ifndef	OEM_FAN_H
#define	OEM_FAN_H


//------------------------------------------------------------------------------
// Fan RPM & EC Tachometer Mapping Table
//------------------------------------------------------------------------------
#define	RPM0000	65535
#define	RPM1800	1198
#define	RPM1850	1166
#define	RPM1900	1135
#define	RPM1950	1106
#define	RPM2000	1078
#define	RPM2050	1052
#define	RPM2100	1027
#define	RPM2150	1003
#define	RPM2200	980
#define	RPM2250	958
#define	RPM2300	938
#define	RPM2350	918
#define	RPM2400	898
#define	RPM2450	880
#define	RPM2500	863
#define	RPM2550	846
#define	RPM2600	829
#define	RPM2650	814
#define	RPM2700	799
#define	RPM2750	784
#define	RPM2800	770
#define	RPM2850	757
#define	RPM2900	744
#define	RPM2950	731
#define	RPM3000	719
#define	RPM3050	707
#define	RPM3100	696
#define	RPM3150	685
#define	RPM3200	674
#define	RPM3250	663
#define	RPM3300	653
#define	RPM3350	644
#define	RPM3400	634
#define	RPM3450	625
#define	RPM3500	616
#define	RPM3550	607
#define	RPM3600	599
#define	RPM3650	591
#define	RPM3700	583
#define	RPM3750	575
#define	RPM3800	567
#define	RPM3850	560
#define	RPM3900	553
#define	RPM3950	546
#define	RPM4000	539
#define	RPM4050	532
#define	RPM4100	526
#define	RPM4150	520
#define	RPM4200	513
#define	RPM4250	507
#define	RPM4300	501
#define	RPM4350	496
#define	RPM4400	490
#define	RPM4450	485
#define	RPM4500	479
#define	RPM4550	474
#define	RPM4600	469
#define	RPM4650	464
#define	RPM4700	459
#define	RPM4750	454
#define	RPM4800	449
#define	RPM4850	445
#define	RPM4900	440
#define	RPM4950	436
#define	RPM5000	431
#define	RPM5050	427
#define	RPM5100	423
#define	RPM5150	419
#define	RPM5200	415
#define	RPM5250	411
#define	RPM5300	407
#define	RPM5350	403
#define	RPM5400	399
#define	RPM5450	396
#define	RPM5500	392
#define	RPM5550	389
#define	RPM5600	385
#define	RPM5650	382
#define	RPM5700	378
#define	RPM5750	375
#define	RPM5800	372
#define	RPM5850	369
#define	RPM5900	365
#define	RPM5950	362
#define	RPM6000	359
#define	RPM6050	356
#define	RPM6100	353
#define	RPM6150	351
#define	RPM6200	348
#define	RPM6250	345
#define	RPM6300	342
#define	RPM6350	340
#define	RPM6400	337
#define	RPM6450	334
#define	RPM6500	332
#define	RPM6550	329
#define	RPM6600	327
#define	RPM6650	324
#define	RPM6700	322
#define	RPM6750	319
#define	RPM6800	317
#define	RPM6850	315
#define	RPM6900	313
#define	RPM6950	310
#define	RPM7000	308
#define	RPM7050	306
#define	RPM7100	304
#define	RPM7150	302
#define	RPM7200	299
#define	RPM7250	297
#define	RPM7300	295
#define	RPM7350	293
#define	RPM7400	291
#define	RPM7450	289
#define	RPM7500	288
#define	RPM7550	286
#define	RPM7600	284
#define	RPM7650	282
#define	RPM7700	280
#define	RPM7750	278
#define	RPM7800	276
#define	RPM7850	275
#define	RPM7900	273
#define	RPM7950	271
#define	RPM8000	270
#define	RPM9999	127		// leox_20111025 Let fan run in full speed


//------------------------------------------------------------------------------
// Structure Declaration
//------------------------------------------------------------------------------
typedef struct sFanCtrl	// leox_20111129 For fan control table
{
	BYTE	FanOn;
	WORD	RPMOn;
	BYTE	FanOff;
	WORD	RPMOff;
} asFanCtrl;

typedef struct sThermPoint	// leox_20120229 For thermal point
{
	BYTE	Remote;			// Thermal point (Remote)
	BYTE	Local;			// Thermal point (Local )
} asThermPoint;

#ifdef	Support_Intel_CPU
typedef struct sThermUpDown
{
	BYTE	TurboOnD;		// Turbo on disable
	BYTE	ThermUpH;		// Thermal up (High bound)
	BYTE	ThermUpL;		// Thermal up (Low  bound)
	BYTE	ProcHotE;		// PROCHOT# enable
	BYTE	TurboOnE;		// Turbo on enable
	BYTE	ThermDownH;		// Thermal down (High bound)
	BYTE	ThermDownL;		// Thermal down (Low  bound)
	BYTE	ProcHotD;		// PROCHOT# disable
	BYTE	Throttling;		// Throttling point
	BYTE	OSShutdown;		// OS shutdown point
} asThermUpDown;
#else
typedef struct sThermUpDown
{
	BYTE	ThermUpH;		// Thermal up (High bound)
	BYTE	ThermUpL;		// Thermal up (Low  bound)
	BYTE	ThermDownH;		// Thermal down (High bound)
	BYTE	ThermDownL;		// Thermal down (Low  bound)
	BYTE	Throttling;		// Throttling point
	BYTE	OSShutdown;		// OS shutdown point
} asThermUpDown;
#endif


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void FanCtrlDACPWMRPM(void);
extern void UpDateFanControl_Mode(void);
extern void ChkFanCtrlDACPWMRPM(void);
extern void LoadFanCtrlDefault(void);
extern void GetFan1RPM(void);
extern void GetFan2RPM(void);									// leox_20120322 Support_FAN2
extern void SetCPUTypeByTjMax(void);							// leox_20120229 CPU_TYPE_BY_TJMAX
extern void CheckFan1Fault(void);								// David_20120613 add for fan error check	// CHECK_FAN_FAULT


#endif	/* OEM_FAN_H */
