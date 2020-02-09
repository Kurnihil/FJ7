/*------------------------------------------------------------------------------
 * Title : OEM_TP.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_TP_H
#define	OEM_TP_H


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void CheckOemTouchpadStatus(void);
extern void SWTouchpad_Enable_DisableFunOn(void);
extern void SWTouchpad_Enable_DisableFunOff(void);
extern void OEM_CmdA8_EAux(void);
extern void OEM_CmdA7_DAux(void);
extern void Set_Touchpad_Enable(void);
extern void Set_Touchpad_Disable(void);

#ifdef	Multiple_TP	// ken add for touchpad compare	// leox_20120316 Code from "OEM_MAIN.H"
extern void DetectTPID(void);
extern void DetectTPID4PS2(void);
#endif	// Multiple_TP


#endif	/* OEM_TP_H */

