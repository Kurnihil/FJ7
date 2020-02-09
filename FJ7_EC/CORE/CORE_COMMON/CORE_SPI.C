/*------------------------------------------------------------------------------
 * Title: CORE_SPI.C - SPI function for KBC firmware.
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

//-----------------------------------------------------------------------
// Init SPI interface function
//-----------------------------------------------------------------------
void Init_SPIInterface(BYTE channel, BYTE supportbusy)
{
	BYTE settingOK;

	settingOK = 0x01;	// Pr-set OK

	if (channel == SPI_Channel_0)
	{
		GCR1 |= SPICTRL_0;
		SSCE0 = ALT;
	}
	else if (channel == SPI_Channel_1)
	{
		GCR1 |= SPICTRL_1;
		SSCE1 = ALT;
	}
	else if (channel == SPI_Channel_0N1)
	{
		GCR1 |= (SPICTRL_0 + SPICTRL_1);
		SSCE0 = ALT;
		SSCE1 = ALT;
	}
	else
	{
		GCR1 &= ~(SPICTRL_0 + SPICTRL_1);
		settingOK = 0x00;
	}

	if (settingOK == 0x01)
	{
		if (supportbusy == SPI_SupportBusy)
		{
			GCR1 |= SSSPIBP;
		}

		SSCK = ALT;
		SMOSI = ALT;
		SMISO = ALT;

		//SPICTRL1 |= (SCKFREQ2+NTREN);			// 100b: 1/10 FreqEC, Interrupt Enable, 4-wire
		SPICTRL1 |= SCKFREQ2 + CLPOL + CLPHS;	// 100b: 1/10 FreqEC, Interrupt Disable, 4-wir, mode 3

	}
}

//-----------------------------------------------------------------------
// Disable SPI interface function
//-----------------------------------------------------------------------
void Disable_SPIInterface(BYTE channel)
{
	GCR1 &= ~(SPICTRL_0 + SPICTRL_1 + SSSPIBP);

	if (channel == SPI_Channel_0)
	{
		SSCE0 = INPUT;
	}
	else if (channel == SPI_Channel_1)
	{
		SSCE1 = INPUT;
	}
	else if (channel == SPI_Channel_0N1)
	{
		SSCE0 = INPUT;
		SSCE1 = INPUT;
	}

	SSCK = INPUT;
	SMOSI = INPUT;
	SMISO = INPUT;
}

//-----------------------------------------------------------------------
// Enable SPI interface function
//-----------------------------------------------------------------------
void Enable_SPIInterface(void)
{
	Init_SPIInterface(SPI_Channel_0, SPI_NoSupportBusy);
}

//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
void SPIWriteByte(BYTE spidata, BYTE channel)
{
	BYTE settingok;
	settingok = 0x00;

	SPIDATA = spidata;

	SET_MASK(SPICTRL2, BLKSEL);	// Blocking selection.
	CLEAR_MASK(SPICTRL2, CHRW);	// Write cycle.

	if (channel == SPI_Channel_0)
	{
		SET_MASK(SPISTS, CH0START);
		settingok = 0x01;
	}
	else if (channel == SPI_Channel_1)
	{
		SET_MASK(SPISTS, CH1START);
		settingok = 0x01;
	}
}

//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
void SPIWriteWord(BYTE MSB, BYTE LSB, BYTE channel)
{
	BYTE settingok;
	settingok = 0x00;

	SPIDATA = MSB;

	SET_MASK(SPICTRL2, BLKSEL);	// Blocking selection.
	CLEAR_MASK(SPICTRL2, CHRW);	// Write cycle.

	if (channel == SPI_Channel_0)
	{
		SET_MASK(SPISTS, CH0START);
		SPIDATA = LSB;
		SET_MASK(SPISTS, CH0START);
		settingok = 0x01;
	}
	else if (channel == SPI_Channel_1)
	{
		SET_MASK(SPISTS, CH1START);
		SPIDATA = LSB;
		SET_MASK(SPISTS, CH1START);
		settingok = 0x01;
	}
}

//-----------------------------------------------------------------------
//
//-----------------------------------------------------------------------
BYTE SPIReadByte(BYTE channel)
{
	BYTE settingok;
	settingok = 0x00;

	SET_MASK(SPICTRL2, BLKSEL);	// Blocking selection.
	SET_MASK(SPICTRL2, CHRW);		// Read cycle.

	if (channel == SPI_Channel_0)
	{
		SET_MASK(SPISTS, CH0START);
		settingok = 0x01;
	}
	else if (channel == SPI_Channel_1)
	{
		SET_MASK(SPISTS, CH1START);
		settingok = 0x01;
	}

	return(SPIDATA);
}

