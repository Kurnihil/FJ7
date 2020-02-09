/*------------------------------------------------------------------------------
 * Title: CORE_LPC.C - LPC function for KBC firmware.
 *
 * Copyright (c) 2011 - , ITE INC. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of ITE INC. .
 *----------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

//----------------------------------------------------------------------------
// The selection of HLPC protect mode use P0ZR
//----------------------------------------------------------------------------
void HLPC_Protect0Mode(BYTE mode)
{
	if (mode == ProtectMode_ReadProtection || mode == ProtectMode_WriteProtection || mode == ProtectMode_RWProtection)
	{
		P0ZR |= mode;
	}
}

//----------------------------------------------------------------------------
// The selection of HLPC protect size use P0ZR
//----------------------------------------------------------------------------
void HLPC_Protect0Size(BYTE size)
{
	P0ZR |= size;
}

//----------------------------------------------------------------------------
// The selection of HLPC protect mode use P1ZR
//----------------------------------------------------------------------------
void HLPC_Protect1Mode(BYTE mode)
{
	if (mode == ProtectMode_ReadProtection || mode == ProtectMode_WriteProtection || mode == ProtectMode_RWProtection)
	{
		P1ZR |= mode;
	}
}

//----------------------------------------------------------------------------
// The selection of HLPC protect size use P1ZR
//----------------------------------------------------------------------------
void HLPC_Protect1Size(BYTE size)
{
	P1ZR |= size;
}

//----------------------------------------------------------------------------
// To lock protect related registers
//----------------------------------------------------------------------------
void HLPC_LockProtectRelatedRegisters(void)
{
	SET_MASK(SMECCS, LKPRR);
}

