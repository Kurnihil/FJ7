/*------------------------------------------------------------------------------
 * Title : OEM_IPOD.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_IPOD_H
#define	OEM_IPOD_H


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void EnableUSBCHARGPORTFun(void);
extern void DisableUSBCHARGPORTFun(void);
extern void CheckUSBchargingEnDis(void);
extern void USBDischargingS0ToS4S5Init(void);
extern void USBChargeFunctionOnlyS4S5(void);
extern void USBDischargingS0ToS4S5InitDis(void);
extern void USBDischargingS0ToS4S5InitDis0(void);
extern void USBDischargingS0ToS4S5InitDis1(void);
extern void USBDischargingS0ToS4S5InitDis2(void);
extern void InitUSBChargePort(void);
extern void PowerOnInitUSBChargePort(void);
extern void USBDischargingS4S5ToS0Init(void);
extern void CheckUSBWorkingCNT(void);
extern void USBHookInit(void);
extern void DefaultUSBCharging(void);
extern BYTE HookChkIdle_OEM_IPOD(void);
extern void ConfigACIn_OEM_IPOD(void);
extern void ConfigACOut_OEM_IPOD(void);
extern void S3S4S5toS0_OEM_IPOD(void);
extern void USBChargeFunctionS0(void);							// leox20150914 For USB charger SLG55544 to set SDP/CDP in S0 when BIOS set


#endif	/* OEM_IPOD_H */
