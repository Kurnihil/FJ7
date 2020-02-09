/*------------------------------------------------------------------------------
 * Title : OEM_VICMV.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_VICMV_H
#define	OEM_VICMV_H


//------------------------------------------------------------------------------
// Common Definition
//------------------------------------------------------------------------------
// leox20150616 Modified for 65W adapter for FH9C requested by power team Vincent Weng
//#define	VICMV65WHiEnThroLimit	0x01A5	// 1.236V	// leox20150716 0x01A4 -> 0x01A5 (3.09A) requested by Vincent Weng on 2015/07/16
#define	VICMV65WHiEnThroLimit		0x01C9	// 1.340V	// leox20150902 0x01A5 -> 0x01C9 (3.35A) requested by Vincent Weng on 2015/09/02
#define	VICMV65WLoDisThroLimit		0x0199	// 1.200V
#define	VICMV65WHiEnHybridLimit		0x01C7	// 1.334V	// leox20150622 0x01C2 -> 0x01C7 requested by Vincent Weng on 2015/06/18
#define	VICMV65WLoDisHybridLimit	0x01BB	// 1.300V

// leox20150616 Modified for 90W adapter for FH9C requested by power team Vincent Weng
#define	VICMV90WHiEnThroLimit		0x0245	// 1.704V
#define	VICMV90WLoDisThroLimit		0x0236	// 1.660V
#define	VICMV90WHiEnHybridLimit		0x026E	// 1.824V
#define	VICMV90WLoDisHybridLimit	0x0264	// 1.796V

#define	VICMVHiEnThroLimitDebounce	100		// 2 sec (timebase: 20ms)	// leox20150916 Change time base to 20ms
#define	VICMVHiDisThroLimitDebounce	250		// 5 sec (timebase: 20ms)	// leox20150916 Change time base to 20ms


// leox20150911 A/D volatage trigger/release point +++
typedef struct ADV_Point
{
	WORD	Trigger;
	WORD	Release;
} sADV_Point;
// leox20150911 A/D volatage trigger/release point ---


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void CheckVICMVThrottlingFUN(void);
extern void InitVICMVHi(void);
extern void Get_VICMVoltage(void);
extern void CheckIfEnableVICMVThrottly(void);
extern void CheckIfDisableVICMVThrottly(void);
extern void LoadVICM_Limit(void);
extern void CheckIfEnDCHGVThrottly(void);		// leox20150826 BAT_DCHG_PROTECT
extern void CheckIfDsDCHGVThrottly(void);		// leox20150826 BAT_DCHG_PROTECT
extern void LoadDCHG_Limit(void);				// leox20150911 BAT_DCHG_PROTECT


#endif	/* OEM_VICMV_H */
