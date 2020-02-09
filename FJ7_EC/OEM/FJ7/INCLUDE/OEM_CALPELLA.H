/*------------------------------------------------------------------------------
 * Title : OEM_CALPELLA.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_CALPELLA_H
#define	OEM_CALPELLA_H


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void Check_SLP_M(void);
extern void TurnOffSLP_M_On(void);
extern void TurnOnSLP_M_On(void);
extern void Check_AC_PRESENT(void);
extern void TurnOnAC_PRESENT(void);
extern void TurnOffAC_PRESENT(void);
extern void ChkneedTurnOffSBLANPwrFS345(void);
extern void CommonStateChange_Calpella(void);
extern void TurnOffSBPowerSource_Calpella(void);


#endif	/* OEM_CALPELLA_H */

