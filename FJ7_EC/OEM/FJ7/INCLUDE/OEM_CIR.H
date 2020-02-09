/*------------------------------------------------------------------------------
 * Title : OEM_CIR.H
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#ifndef	OEM_CIR_H
#define	OEM_CIR_H


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void Hook_CIRWakeUp(void);
extern void CIRS4S5WakeUp(void);
extern void CIRS3WakeUp(void);

#if	CIR_NEC_IMPROVED	// Ken_20120605
extern void CIR_NEC_Rst(void);
extern void CIR_NEC_Chk(void);
#endif	// CIR_NEC_IMPROVED


#endif	/* OEM_CIR_H */
