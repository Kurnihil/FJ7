/*------------------------------------------------------------------------------
 * Title: CORE_IRQ.H - Interrupt Service Routines (ISR).
 *
 * The Interrupt Service Routines (ISR).  The actual interrupt functions are
 * implemented in the "CHIP_IRQ.C".
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/

#ifndef	CORE_IRQ_H
#define	CORE_IRQ_H

extern void DisableAllInterrupt(void);
extern void EnableAllInterrupt(void);
extern void EnableKBCIBFInt(void);
extern void EnablePMCIBFInt(void);
extern void EnablePMC3IBFInt(void);
extern void Irq_Aux_Port3(void);
extern void Irq_Aux_Port2(void);
extern void Irq_Aux_Port1(void);

#endif	/* CORE_IRQ_H */

