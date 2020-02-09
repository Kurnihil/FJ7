/*------------------------------------------------------------------------------
 * Title: OEM_INIT.H
 *
 * OEM defined constants for initializing variables to known values on reset.
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/


#ifndef	OEM_INI_H
#define	OEM_INI_H


//------------------------------------------------------------------------------
// Common Definition
//------------------------------------------------------------------------------
#define	LED_CTRL_INIT		(MASK(SCROLL) | MASK(NUM) | MASK(CAPS) | MASK(OVL))
#define	SAVE_KBD_STATE_INIT	(maskAUXENAB + (2 << shiftCODESET))
#define	CCB42_INIT			DISAB_KEY
#define	EXT_CB2_INIT		0x22
#define	SAVE_TYPEMATIC_INIT	0x2A


//------------------------------------------------------------------------------
// Structure Declaration
//------------------------------------------------------------------------------
typedef struct REG_INIT_DEF
{
	XBYTE	*address;
	BYTE	initdata;
} sREG_INIT_DEF;

typedef struct DMAScratchSRAM
{
	XBYTE	*scarh;
	XBYTE	*scarm;
	XBYTE	*scarl;
} sDMAScratchSRAM;


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void Oem_InitializationL(void);
extern void Oem_InitializationH(void);
extern void Init_ClearRam(void);
extern void Init_ClearBram(void);		// leox_20110926 From "OEM_MAIN.H"
extern void Init_SetBramDefVal(void);	// leox20151104	// leox20160322
extern void Init_SMBus_Regs(void);
extern void Init_Cache(void);
extern void Init_ADC(void);			// leox_20110926 From "OEM_MAIN.H"
extern void CheckResetLater(void);	// leox_20111206 RESET_EC_LATER
extern void CacheDma(BYTE sramnum, WORD addr);	// leox20150706 SPI_DMA_MODE


#endif	/* OEM_INI_H */
