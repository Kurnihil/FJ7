#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>

#include <MATH.H>
#include <stdlib.h>

#ifdef	IT_7230MMB_SUPPORT

// Read/write IT7230 registers
void CapS_Write_Reg(BYTE page, BYTE addr_byte, WORD data_word)
{
#if	(0x10 == (BUS_TYPE&0xF0))			// SPI bus
	BYTE addr_High;
	BYTE addr_Low;

	if (0 == page)
	{
		addr_High = 0x00;
	}
	else if (1 == page)
	{
		addr_High = 0x20;
	}
	else if (2 == page)
	{
		addr_High = 0x40;
	}

	addr_Low	= addr_byte;

	// Write Progress
	SPICTRL2	= 0x00;					// Set 8-bit transmission and send write cycle
	SPIDATA		= addr_High;			// Transmit the first byte

#if	(0x01 == (BUS_TYPE&0x0F))
	SPISTS = 0x08;						// Use channel 1
#else
	SPISTS = 0x10;						// Use channel 0
#endif

	while(IsFlag1(SPISTS, BIT2));		// Check if data is in progress

	SPIDATA		= addr_Low;				// Transmit the second byte

#if	(0x01 == (BUS_TYPE&0x0F))
	SPISTS = 0x08;						// Use channel 1
#else
	SPISTS = 0x10;						// Use channel 0
#endif

	while(IsFlag1(SPISTS, BIT2));		// Check if data is in progress

	// Write Progress
	SPIDATA = (BYTE)(data_word >> 8);	// Transmit the third byte

#if	(0x01 == (BUS_TYPE&0x0F))
	SPISTS = 0x08;						// Use channel 1
#else
	SPISTS = 0x10;						// Use channel 0
#endif

	while(IsFlag1(SPISTS, BIT2));		// Check if data is in progress

	SPIDATA = (BYTE)data_word;		// Transmit the fourth byte

#if	(0x01 == (BUS_TYPE&0x0F))
	SPISTS = 0x08;						// Use channel 1
#else
	SPISTS = 0x10;						// Use channel 0
#endif

	while(IsFlag1(SPISTS, BIT2));		// Check if data is in progress

	// End SPI transmission
	SPISTS = 0x20;					// To end
	SPISTS = 0x02;					// Set transfer end flag
#endif	// SPI bus

#if	(0x20 == (BUS_TYPE&0xF0))		// SM bus

	BYTE Data_Array[2];

	if (page != current_page)		// Switch page
	{
		Data_Array[0] = page;		// Low byte
		Data_Array[1] = 0x00;		// High byte

		bRWSMBus((BUS_TYPE&0x0F), SMbusWW, I2C_DEVICE_ADDR, 0x00, &Data_Array , 0);
		current_page = page;
	}

	Data_Array[0] = (BYTE)data_word;			// Low byte
	Data_Array[1] = (BYTE)(data_word >> 8);		// High byte
	
	bRWSMBus((BUS_TYPE&0x0F), SMbusWW, I2C_DEVICE_ADDR, addr_byte, &Data_Array , 0);

#endif	// SM bus
}

WORD CapS_Read_Reg(BYTE page, BYTE addr_byte)
{
	WORD data_word = 0x0000;

#if	(0x10 == (BUS_TYPE&0xF0))			// SPI bus
	BYTE addr_High;
	BYTE addr_Low;

	if (0 == page)
	{
		addr_High = 0x10;
	}
	else if (1 == page)
	{
		addr_High = 0x30;
	}
	else if (2 == page)
	{
		addr_High = 0x50;
	}

	addr_Low = addr_byte;

	// Write Progress
	SPICTRL2 = 0x00;					// Set 8-bit transmission and send write cycle
	SPIDATA = addr_High;				// Transmit the first byte
	
#if	(0x01 == (BUS_TYPE&0x0F))
	SPISTS = 0x08;						// Use channel 1
#else
	SPISTS = 0x10;						// Use channel 0
#endif

	while(IsFlag1(SPISTS, BIT2));		// Check if data is in progress

	SPIDATA = addr_Low;					// Transmit the second byte

#if	(0x01 == (BUS_TYPE&0x0F))
	SPISTS = 0x08;						// Use channel 1
#else
	SPISTS = 0x10;						// Use channel 0
#endif

	while(IsFlag1(SPISTS, BIT2));		// Check if data is in progress

	// Read Progress
	SPICTRL2 = 0x04;					// Set 8-bit transmission and send read cycle

#if	(0x01 == (BUS_TYPE&0x0F))
	SPISTS = 0x08;						// Use channel 1
#else
	SPISTS = 0x10;						// Use channel 0
#endif

	while(IsFlag1(SPISTS, BIT2));		// Check if data is in progress

	data_word = ((WORD)SPIDATA) << 8;	// Return Hi-Byte

#if	(0x01 == (BUS_TYPE&0x0F))
	SPISTS = 0x08;						// Use channel 1
#else
	SPISTS = 0x10;						// Use channel 0
#endif

	while(IsFlag1(SPISTS, BIT2));		// Check if data is in progress

	data_word = data_word | (WORD)SPIDATA;	// Return Low-byte

	// End SPI transmission
	SPISTS = 0x20;						// To end
	SPISTS = 0x02;						// Set transfer end flag

#endif	// SPI bus

#if	(0x20 == (BUS_TYPE&0xF0))			// SM bus

	WORD temp_w = 0x0000;
	BYTE Data_Array[2];

	if (page != current_page)			// Switch page
	{
		Data_Array[0]	= page;		// Low byte
		Data_Array[1]	= 0x00;		// High byte

		bRWSMBus((BUS_TYPE&0x0F), SMbusWW, I2C_DEVICE_ADDR, 0x00, &Data_Array, 0);

		current_page = page;
	}

	// Smart add
	if (bRWSMBus((BUS_TYPE&0x0F), SMbusRW, I2C_DEVICE_ADDR, addr_byte, &temp_w, 0))
	{
		data_word |= temp_w >> 8;
		data_word |= temp_w << 8;
	}

#endif	// SM bus
	return data_word;
}

//---------------------------------------------------------
// Initialize EC SMBUS or GPIO for MMB control
void EC_Init(void)
{

#if	(0x20 == (BUS_TYPE&0xF0))			// SM bus

	if (0x00 == (BUS_TYPE&0x0F))		// Channel A
	{
		GPCRB3 &= 0x3F;			// Define GPIO group B
		GPCRB4 &= 0x3F;			// byte3(SMCLK0) and byte4(SMDAT0)for SM Bus channel A

		GPCRB3 |= 0x04;			// Internal Pull Up
		GPCRB4 |= 0x04;			// Internal Pull Up
	}
	else if (0x01 == (BUS_TYPE&0x0F))	// Channel B
	{
		GPCRC1 &= 0x3F;			// Define GPIO group C
		GPCRC2 &= 0x3F;			// byte1(SMCLK1)and byte2(SMDAT1) for SM Bus channel B

		GPCRC1 |= 0x04;			// Internal Pull Up
		GPCRC2 |= 0x04;			// Internal Pull Up
	}
	else if (0x02 == (BUS_TYPE&0x0F))	// Channel C
	{
		GPCRF6 &= 0x3F;			// Define GPIO group F
		GPCRF7 &= 0x3F;			// byte6(SMCLK2)and byte7(SMDAT2) for SM Bus channel C

		GPCRF6 |= 0x04;			// Internal Pull Up
		GPCRF7 |= 0x04;			// Internal Pull Up
	}
	else if (0x03 == (BUS_TYPE&0x0F))	// Channel C
	{
		GPCRH1 &= 0x3F;			// Define GPIO group F
		GPCRH2 &= 0x3F;			// byte6(SMCLK2)and byte7(SMDAT2) for SM Bus channel C

		GPCRH1 |= 0x04;			// Internal Pull Up
		GPCRH2 |= 0x04;			// Internal Pull Up
	}
#endif	// SM bus
}
//---------------------------------------------------------
// Initialize some variables
void CapS_Init(void)
{
	current_page  = 0x00;				// Defaule page number is 0
	convn_setting = 0x00;				// The default value of conversion interrupt is disabled
	last_contact  = 0x00;
	EC_Init();
}

void CapS_ISR(void)
{
	BYTE i;
	BYTE convn_enable = 0x00;
	WORD contact;

	contact = CapS_Read_Reg(PAGE_0, CAPS_SXCHSR);
	CapS_Read_Reg(PAGE_0, CAPS_SXCSR);
	CapS_Read_Reg(PAGE_0, CAPS_SIR);

//	RamDebug(0xBB);
//	RamDebug(contact&0xff);
//	RamDebug(contact>>8);

	if (contact)
	{
		// Find the first contact stage out
		i = 0;
		while (1)
		{
			if ((contact>>i)&0x0001)
			{
				break;
			}
			i++;
		}
		if(contact & ~(0x01<<i))
		{//two btn return
			//RamDebug(0xCC);
			return;
		}
		if(last_contact!=i)
		{
			CapS_Run_Button(last_contact,BREAK_EVENT);
		}
		CapS_Run_Button(i,MAKE_EVENT);
		last_contact = i;
	}
	else
	{
		CapS_Run_Button(last_contact,BREAK_EVENT);
		last_contact = 0x00;
	}
}

void CapS_Run_Button(BYTE index, BYTE event)
{
	if(event == MAKE_EVENT)
	{
		//David modify for LID close not power on 2014/06/18
		//David modify MMB wake up support 2014/07/02
		if (LIDSW_read())
		{
			return;
		//David modify MMB wake up support 2014/07/02
		}
		//David add for notify BIOS when button pressed 2014/06/04
		if (IsFlag0(Factory_MMBTest_status, MMBTest_Start))
			ECQEvent(Q8F_SCI, SCIMode_Normal);
		//David add for notify BIOS when button pressed 2014/06/04
		if (IsFlag1(Factory_MMBTest_status, MMBLED_Start))
			return;
		switch(index)
		{
			case 0:		// SUPPORT
			CapS_Write_Reg(PAGE_1, CAPS_GPIOOR ,(CapS_Read_Reg(PAGE_1, CAPS_GPIOOR)|0x0010));
			SET_MASK(CapButtonStat,MMBBtn0);
			if (IsFlag0(POWER_FLAG, power_on))
			{
				if (IsFlag1(POWER_FLAG, enter_SUS))
				{
					if (IsFlag1(MMB_WakeUp_S3_BRM, MMB_Button1_Wake))
					{
						CheckPower();
						if (IsFlag0(MISC_FLAG, F_CriLow))
						{
							SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
							PSWOnPower();
							if (MMB_Status_OEM == 0)
								SetFlag(MMB_Status_OEM, MMBBtn0);
						}
					}
				}
				else
				{
					if (IsFlag1(POWER_STAT_CTRL, enter_S4))
					{
						if (IsFlag1(MMB_WakeUp_S4_BRM, MMB_Button1_Wake))
						{
							CheckPower();
							if (IsFlag0(MISC_FLAG, F_CriLow))
							{
								SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
								PSWOnPower();
								if (MMB_Status_OEM == 0)
									SetFlag(MMB_Status_OEM, MMBBtn0);
							}
						}
					}
					else
					{
						if (IsFlag1(MMB_WakeUp_S5_BRM, MMB_Button1_Wake))
						{
							CheckPower();
							if (IsFlag0(MISC_FLAG, F_CriLow))
							{
								SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
								PSWOnPower();
								if (MMB_Status_OEM == 0)
									SetFlag(MMB_Status_OEM, MMBBtn0);
							}
						}
					}
				}
			}
			else
			{
				//David add for factory test 2014/07/24
				if (IsFlag1(Factory_MMBTest_status, MMBTest_Start))
					SetFlag(Factory_MMBTest_status, MMBBtn0);
				//David add for factory test 2014/07/24
				if (MMB_Status_OEM == 0)
					SetFlag(MMB_Status_OEM, MMBBtn0);
			}
			break;

			case 1:		// MENU
			CapS_Write_Reg(PAGE_1, CAPS_GPIOOR ,(CapS_Read_Reg(PAGE_1, CAPS_GPIOOR)|0x0008));
			SET_MASK(CapButtonStat,MMBBtn1);
			if (IsFlag0(POWER_FLAG, power_on))
			{
				if (IsFlag1(POWER_FLAG, enter_SUS))
				{
					if (IsFlag1(MMB_WakeUp_S3_BRM, MMB_Button2_Wake))
					{
						CheckPower();
						if (IsFlag0(MISC_FLAG, F_CriLow))
						{
							SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
							PSWOnPower();
							if (MMB_Status_OEM == 0)
								SetFlag(MMB_Status_OEM, MMBBtn1);
						}
					}
				}
				else
				{
					if (IsFlag1(POWER_STAT_CTRL, enter_S4))
					{
						if (IsFlag1(MMB_WakeUp_S4_BRM, MMB_Button2_Wake))
						{
							CheckPower();
							if (IsFlag0(MISC_FLAG, F_CriLow))
							{
								SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
								PSWOnPower();
								if (MMB_Status_OEM == 0)
									SetFlag(MMB_Status_OEM, MMBBtn1);
							}
						}
					}
					else
					{
						if (IsFlag1(MMB_WakeUp_S5_BRM, MMB_Button2_Wake))
						{
							CheckPower();
							if (IsFlag0(MISC_FLAG, F_CriLow))
							{
								SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
								PSWOnPower();
								if (MMB_Status_OEM == 0)
									SetFlag(MMB_Status_OEM, MMBBtn1);
							}
						}
					}
				}
			}
			else
			{
				//David add for factory test 2014/07/24
				if (IsFlag1(Factory_MMBTest_status, MMBTest_Start))
					SetFlag(Factory_MMBTest_status, MMBBtn1);
				//David add for factory test 2014/07/24
				if (MMB_Status_OEM == 0)
					SetFlag(MMB_Status_OEM, MMBBtn1);
			}
			break;

			case 2:		// MY CLOUD
			CapS_Write_Reg(PAGE_1, CAPS_GPIOOR ,(CapS_Read_Reg(PAGE_1, CAPS_GPIOOR)|0x0004));
			SET_MASK(CapButtonStat,MMBBtn2);
			if (IsFlag0(POWER_FLAG, power_on))
			{
				if (IsFlag1(POWER_FLAG, enter_SUS))
				{
					if (IsFlag1(MMB_WakeUp_S3_BRM, MMB_Button3_Wake))
					{
						CheckPower();
						if (IsFlag0(MISC_FLAG, F_CriLow))
						{
							SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
							PSWOnPower();
							if (MMB_Status_OEM == 0)
								SetFlag(MMB_Status_OEM, MMBBtn2);
						}
					}
				}
				else
				{
					if (IsFlag1(POWER_STAT_CTRL, enter_S4))
					{
						if (IsFlag1(MMB_WakeUp_S4_BRM, MMB_Button3_Wake))
						{
							CheckPower();
							if (IsFlag0(MISC_FLAG, F_CriLow))
							{
								SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
								PSWOnPower();
								if (MMB_Status_OEM == 0)
									SetFlag(MMB_Status_OEM, MMBBtn2);
							}
						}
					}
					else
					{
						if (IsFlag1(MMB_WakeUp_S5_BRM, MMB_Button3_Wake))
						{
							CheckPower();
							if (IsFlag0(MISC_FLAG, F_CriLow))
							{
								SetFlag(Device_STAT_CTRL, PowerButtonPowerOn);
								PSWOnPower();
								if (MMB_Status_OEM == 0)
									SetFlag(MMB_Status_OEM, MMBBtn2);
							}
						}
					}
				}
			}
			else
			{
				//David add for factory test 2014/07/24
				if (IsFlag1(Factory_MMBTest_status, MMBTest_Start))
					SetFlag(Factory_MMBTest_status, MMBBtn2);
				//David add for factory test 2014/07/24
				if (MMB_Status_OEM == 0)
					SetFlag(MMB_Status_OEM, MMBBtn2);
			}
			break;
		}
	}else
	{//Break
		if(index==0)
		{
			if(IS_MASK_SET(CapButtonStat,MMBBtn0))
			{
				CapS_Write_Reg(PAGE_1, CAPS_GPIOOR ,(CapS_Read_Reg(PAGE_1, CAPS_GPIOOR)&0xFFEF));
				CLR_MASK(CapButtonStat,MMBBtn0);
			}

		}
		if(index==1)
		{
			if(IS_MASK_SET(CapButtonStat,MMBBtn1))
			{
				CapS_Write_Reg(PAGE_1, CAPS_GPIOOR ,(CapS_Read_Reg(PAGE_1, CAPS_GPIOOR)&0xFFF7));
				CLR_MASK(CapButtonStat,MMBBtn1);
			}
		}
		if(index==2)
		{
			if(IS_MASK_SET(CapButtonStat,MMBBtn2))
			{
				CapS_Write_Reg(PAGE_1, CAPS_GPIOOR ,(CapS_Read_Reg(PAGE_1, CAPS_GPIOOR)&0xFFFB));
				CLR_MASK(CapButtonStat,MMBBtn2);
			}
		}
	}
}

void PollingIT723XInt(void)
{
	if (MMB_Detect_Low())
	{
		if(!READ_IT723X_INT())
		{
			F_Service_OEM_1 = 1;
		}
	}
}

void ITE_Check_ESD(void)
{
	WORD usESD_PCR_status = 0x0006;

	if ( usESD_PCR_status != (CapS_Read_Reg(PAGE_1, CAPS_PCR)&0x0006)) 
	{
		resetIT7230();
		return;
	}
}

void MMBLEDTest(void)
{
	if (IsFlag1(Factory_MMBTest_status, MMBLED_Start))
	{
		if (IsFlag0(Factory_MMBTest_status, MMBLED_Fin))
		{
			CapS_Write_Reg(PAGE_1, CAPS_GPIOOR ,(CapS_Read_Reg(PAGE_1, CAPS_GPIOOR)|0x001C));
			SetFlag(Factory_MMBTest_status, MMBLED_Fin);
		}
	}
	else
	{
		if (IsFlag0(Factory_MMBTest_status, MMBLED_Fin))
		{
			CapS_Write_Reg(PAGE_1, CAPS_GPIOOR ,(CapS_Read_Reg(PAGE_1, CAPS_GPIOOR)&0xFFE3));
			SetFlag(Factory_MMBTest_status, MMBLED_Fin);
		}
	}
}

#endif