/*------------------------------------------------------------------------------
 * Title: OEM_LED.H
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/


#ifndef	OEM_LED_H
#define	OEM_LED_H


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void OEM_Write_Leds(BYTE data_byte);
extern void ForceProcessLED(void);
extern void ProcessLED(void);
extern void DebugLED(BYTE val);								// leox_20120406
extern void DimmLEDProcess(void);
extern void ExitDebugLED(void);								// leox_20120406
extern void DebugLED2(BYTE val);							// leox20150721 LED function


#endif	/* OEM_LED_H */
