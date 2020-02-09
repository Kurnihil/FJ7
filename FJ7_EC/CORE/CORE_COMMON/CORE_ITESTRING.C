/*------------------------------------------------------------------------------
 * Title: CORE_ITESTRING.C
 *
 * Copyright (c) ITE INC. All Rights Reserved.
 *----------------------------------------------------------------------------*/

#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

const unsigned char code ITEString[] =		// For 8518 crystal free
{
	0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, 0xA5, EC_Signature_Flag,
	0x85, 0x12, 0x5A, 0x5A, 0xAA, EFlash_Size, 0x55, 0x55,
};

const unsigned char code VersionString[] =  "ITE EC-V13.6  ";
