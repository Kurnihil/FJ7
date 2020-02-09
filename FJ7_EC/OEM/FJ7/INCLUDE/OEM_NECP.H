/*------------------------------------------------------------------------------
 * Title : OEM_NECP.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_NECP_H
#define	OEM_NECP_H


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void NECStopCharging(void);
extern void CheckNECP_BattRefresh(void);
extern void CheckNECP_BattRefresh2(void);					// leox_20130312 For new timer routine (500ms)
extern void ChkNECPBattRefreshFun(void);
extern void ChkChgCurfromBattMax(void);
extern void IfReadWriteBattFirstDate(void);
extern void IfReadBattFirstUSEDate(void);
extern void IfWriteBattFirstUSEDate(void);
extern void InitBattToNormalCondition(void);
extern void ChkNECBattRefreshPSTS(void);
extern void InitBattToNormalCondition01(void);
extern void ResetBatteryProtectionFunNEC(void);

#endif	/* OEM_NECP_H */
