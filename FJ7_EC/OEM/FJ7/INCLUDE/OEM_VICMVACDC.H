/*------------------------------------------------------------------------------
 * Title : OEM_VICMVACDC.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_VICMVACDC_H
#define	OEM_VICMVACDC_H


//------------------------------------------------------------------------------
// Common Definition
//------------------------------------------------------------------------------
#define	VICMVHiEnThroLimitAC90W			36		// 0.41818182V
#define	VICMVLoDisThroLimitAC90W		32		// 0.38181818V
#define	VICMVHiEnThroLimitAC120W		48		// 0.56363636V
#define	VICMVLoDisThroLimitAC120W		44		// 0.51818182V
#define	VICMVHiEnThroLimitDC8Cell		58		// 0.68181818V
#define	VICMVLoDisThroLimitDC8Cell		50		// 0.59090909V
#define	VICMVHiEnThroLimitDC4Cell		41		// 0.48181818V
#define	VICMVLoDisThroLimitDC4Cell		33		// 0.39090909V

// leo_20150916 add power protect +++			// BAT_MBATV_PROTECT
#if 1	// leox20150917 For FH9C				// Refer to pin I_DCHG
#define	W_66000mW						0x32190	// 205200	// 66.0W (100%)
#define	W_59400mW						0x2D168	// 184680	// 59.4W ( 90%)
#define	W_56700mW						0x2A954	// 174420	// 56.1W ( 85%)
#define	W_52800mW						0x28140	// 164160	// 52.8W ( 80%)
#define	W_51030mW						0x26934	// 158004	// 50.8W ( 77%)
#define	W_45400mW						0x22110	// 139536	// 44.9W ( 68%)

#define	I_10000mA						0x221	// 545
#define	I_09000mA						0x1EB	// 491
#define	I_08100mA						0x1BA	// 442
#define	I_07000mA						0x17E	// 382
#define	I_06600mA						0x168	// 360
#define	I_06300mA						0x157	// 343
#define	I_06000mA						0x147	// 327

#define	VICMV65WHiEnThroLimit_ACDC		0x01D2	// 1.367V
#define	VICMV65WLoDisThroLimit_ACDC		0x01A4	// 1.232V
#define	VICMV90WHiEnThroLimit_ACDC		0x0286	// 1.894V
#define	VICMV90WLoDisThroLimit_ACDC		0x0245	// 1.704V

#define	AC65W_OverPeakCurrentThreshold	0x0265	// 1.798V
#define	AC65W_UnderPeakCurrentThreshold	0x01D2	// 1.367V

#else	// (original) (For FB5)					// Refer to pin I_ADP
#define	W_66000mW						0x3EB2F	// 256815	// 66.0W (100%)
#define	W_59400mW						0x38834	// 231476	// 59.4W ( 90%)
#define	W_56700mW						0x35D66	// 220518	// 56.1W ( 85%)
#define	W_52800mW						0x3228C	// 205452	// 52.8W ( 80%)
#define	W_51030mW						0x307CB	// 198603	// 50.8W ( 77%)
#define	W_45400mW						0x2B231	// 176689	// 44.9W ( 68%)

#define	I_10000mA						0x2AA	// 682
#define	I_09000mA						0x266	// 614
#define	I_08100mA						0x228	// 552
#define	I_07000mA						0x1DD	// 477
#define	I_06600mA						0x1C2	// 450
#define	I_06300mA						0x1AE	// 430
#define	I_06000mA						0x199	// 409

#define	VICMV65WHiEnThroLimit_ACDC		0x00E9	// 0.68V
#define	VICMV65WLoDisThroLimit_ACDC		0x00D2	// 0.61V
#define	VICMV90WHiEnThroLimit_ACDC		0x0124	// 0.86V
#define	VICMV90WLoDisThroLimit_ACDC		0x0117	// 0.82V

#define	AC65W_OverPeakCurrentThreshold	0x0133
#define	AC65W_UnderPeakCurrentThreshold	0x00E9
#endif

#define	VICMVHiEnThroLimitDebounceACDC	100		// 1 sec (timebase: 10ms)
#define	VICMVHiDisThroLimitDebounceACDC	200		// 2 sec (timebase: 10ms)
#define	ACDC_OC_OW_Shutdown_Debounce	3		// 30ms  (timebase: 10ms)
#define	AC_OC_En_Debounce				6000	// 60sec (timebase: 10ms)
#define	AC_OC_Dis_Debounce				1		// 10ms  (timebase: 10ms)
#define	DC_OW_En_Debounce				6000	// 60sec (timebase: 10ms)
#define	DC_OW_Dis_Debounce				1		// 10ms  (timebase: 10ms)
#define	AC_OCP_En_Debounce				6		// 60ms  (timebase: 10ms)
#define	AC_OCP_Dis_Debounce				1		// 10ms  (timebase: 10ms)
#define	DC_OCP_En_Debounce				5		// 50ms  (timebase: 10ms)
#define	DC_OCP_Dis_Debounce				1		// 10ms  (timebase: 10ms)
// leo_20150916 add power protect ---


// leox20150916 MBATV over/under current threshold +++	// BAT_MBATV_PROTECT
typedef struct MBATV_Threshold			// leo_20150916 add power protect
{
	UWORD	OverCurrentThreshold;		// Over  Watt threshold
	UWORD	UnderCurrentThreshold;		// Under Watt threshold
	WORD	OverPeakCurrentThreshold;	// Over  peak current threshold
	WORD	UnderPeakCurrentThreshold;	// Under peak current threshold
} sMBATV_Threshold;
// leox20150916 MBATV over/under current threshold ---


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void CheckVICMVThrottlingFUNACDC(void);
extern void InitVICMVHiACDC(void);
extern void LoadVICM_VACDC_Limit(void);
extern void Get_VICMVoltageACDC(void);
extern void ChkIfEnableVICMVThrottly(void);
extern void ChkIfDisableVICMVThrottly(void);

// leo_20150916 add power protect +++			// BAT_MBATV_PROTECT
extern void ClrDCtoACData(void);
extern void AC_OCP_En_Check(void);
extern void AC_OCP_Dis_Check(void);
extern void ClrACtoDCData(void);
extern void Get_DC_Watt_Value(void);
extern void DC_OW_En_Check(void);
extern void DC_OW_Dis_Check(void);
extern void DC_OCP_En_Check(void);
extern void DC_OCP_Dis_Check(void);
extern void LoadMBATV_Threshold(BYTE Model);	// leox20150916
// leo_20150916 add power protect ---


#endif	/* OEM_VICMVACDC_H */
