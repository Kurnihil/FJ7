/*------------------------------------------------------------------------------
 * Filename: OEM_VER.H          For Chipset: ITE.IT85XX
 * Function: Project version
 *----------------------------------------------------------------------------*/


#ifndef	OEM_VER_H
#define	OEM_VER_H


//------------------------------------------------------------------------------
// Project Information
//------------------------------------------------------------------------------
extern const BYTE code P_model[];
extern const BYTE code P_date[];
extern const BYTE code P_rev[];
extern const BYTE code P_digit[];


extern void LoadEcVer(void);									// leox20150721 For check EC version


#endif	/* OEM_VER_H */
