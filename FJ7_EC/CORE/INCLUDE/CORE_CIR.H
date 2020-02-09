/*------------------------------------------------------------------------------
 * Title: CORE_CIR.C
 *
 * Copyright (c) ITE INC. All Rights Reserved.
 *----------------------------------------------------------------------------*/

#ifndef	CORE_CIR_H
#define	CORE_CIR_H

extern void Init_CIR(void);
extern void Reset_CIR(void);
extern void bClearCirFIFO(void);
extern void bConfirmCirRIM(void);
extern void service_cir(void);
extern void ClearCIR(void);

#endif	/* CORE_CIR_H */

