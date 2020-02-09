/*------------------------------------------------------------------------------
 * Title : OEM_DCMI.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_DCMI_H
#define	OEM_DCMI_H


//------------------------------------------------------------------------------
// Common Definition
//------------------------------------------------------------------------------
#define	DCMIHiEnThroLimit			9000	// - 9000mA
#define	DCMILoDisThroLimit			7000	// - 7000mA

#define	DCMIHiEnThroLimitDebounce	2		// Time base 1 sec
#define	DCMIHiDisThroLimitDebounce	2		// Time base 1 sec


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void CheckDCMIThrottlingFUN(void);
extern void InitDCMIHi(void);
extern void Get_DCMI(void);
extern void CheckIfEnableDCMIThrottly(void);
extern void CheckIfDisableDCMIThrottly(void);


#endif	/* OEM_DCMI_H */

