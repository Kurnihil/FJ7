/*------------------------------------------------------------------------------
 * Title: OEM_SCANTABS.H - RC Table(s) for Scan Matix
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/


#ifndef	OEM_SCANTABS_H
#define	OEM_SCANTABS_H


//------------------------------------------------------------------------------
// Scanner Table & HotKey
//------------------------------------------------------------------------------
extern const BYTE code Rc_ROM_Tables[];
extern const BYTE code KB_S3WakeUP_Tables[];
extern const BYTE code EtKey_S3WakeUP_Tables[];
extern const FUNCT_PTR_V_B code HotKey_Fn_Fx[];


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void Hook_Setup_Scanner_Pntr(void);
extern void Hook_keyboard(BYTE KSIValus, BYTE KSOValus);
extern void Et_Hook_keyboard(BYTE KSIValus, BYTE KSOValus);
extern void Hook_SetGPIOScanPinH(void);
extern void Hook_SetGPIOScanPinL(void);
extern void Hook_SetGPIOScanPinCtrl(void);
extern void OEM_Hook_Send_Key(BYTE table_entry, BYTE event);


#endif	/* OEM_SCANTABS_H */

