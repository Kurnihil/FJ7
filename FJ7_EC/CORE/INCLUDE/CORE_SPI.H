/*------------------------------------------------------------------------------
 * Title: CORE_SPI.H - SPI function for KBC firmware.
 *
 * Copyright (c) 2011 - , ITE INC. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of ITE INC. .
 *----------------------------------------------------------------------------*/

#ifndef	CORE_SPI_H
#define	CORE_SPI_H

#define	SPI_Channel_0	0x00
#define	SPI_Channel_1	0x01
#define	SPI_Channel_0N1	0x02

#define	SPI_SupportBusy		0x01
#define	SPI_NoSupportBusy	0x00

#define	SSCK			GPCRA6
#define	SMOSI			GPCRC3
#define	SMISO			GPCRC5
#define	SSCE1			GPCRG0
#define	SSCE0			GPCRG2

extern void Init_SPIInterface(BYTE channel, BYTE supportbusy);
extern void Disable_SPIInterface(BYTE channel);
extern void Enable_SPIInterface(void);
extern void SPIWriteByte(BYTE spidata, BYTE channel);
extern void SPIWriteWord(BYTE MSB, BYTE LSB, BYTE channel);
extern BYTE SPIReadByte(BYTE channel);

#endif	/* CORE_SPI_H */

