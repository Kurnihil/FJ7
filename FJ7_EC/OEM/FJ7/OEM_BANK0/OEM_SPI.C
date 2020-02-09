/*------------------------------------------------------------------------------
 * Title : OEM_SPI.C
 * Author: Dino
 * Note  : These functions are for reference only.
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


BYTE spi_read_sensor(unsigned char addr)
{
	BYTE temp;
	SPIWriteByte(addr, SPI_Channel);
	WNCKR = 0x00;	// Delay 15.26us
	temp = SPIReadByte(SPI_Channel);
	WNCKR = 0x00;	// Delay 15.26us
	return temp;
}


void spi_write_sensor(unsigned char addr, unsigned char data_in)
{
	if (SPI_Mouse_ID == SPI_Mouse_Sensor_ID_Avago) {addr |= 0x80;}
	SPIWriteWord(addr, data_in, SPI_Channel);
	WNCKR = 0x00;	// Delay 15.26us
	WNCKR = 0x00;	// Delay 15.26us
}


void Power_Up_Sequence(void)
{
	/*
	BYTE spi_temp;

	spi_write_sensor(ADBM_A320_SOFTRESET_ADDR, 0x5A);
	#ifdef MOTION_BURST
	spi_write_sensor(ADBM_A320_IO_MODE_ADDR, ADBM_A320_BURST);
	#endif
	spi_write_sensor(ADBM_A320_OFN_ENGINE_ADDR, 0xE4);	// Write 0xA0 to address 0x60

	// Speed switching thresholds
	spi_write_sensor(ADBM_A320_OFN_RESOLUTION_ADDR, 0x12);		// Wakeup 500DPI
	spi_write_sensor(ADBM_A320_OFN_SPEED_CONTROL_ADDR, 0x0E);	// 16ms, low DPI
	spi_write_sensor(ADBM_A320_OFN_SPEED_ST12_ADDR, 0x08);
	spi_write_sensor(ADBM_A320_OFN_SPEED_ST21_ADDR, 0x06);
	spi_write_sensor(ADBM_A320_OFN_SPEED_ST23_ADDR, 0x40);
	spi_write_sensor(ADBM_A320_OFN_SPEED_ST32_ADDR, 0x08);
	spi_write_sensor(ADBM_A320_OFN_SPEED_ST34_ADDR, 0x48);
	spi_write_sensor(ADBM_A320_OFN_SPEED_ST43_ADDR, 0x0A);
	spi_write_sensor(ADBM_A320_OFN_SPEED_ST45_ADDR, 0x50);
	spi_write_sensor(ADBM_A320_OFN_SPEED_ST54_ADDR, 0x48);

	// Assert/Deassert thresholds
	spi_write_sensor(ADBM_A320_OFN_AD_ATH_HIGH_ADDR, 0x34);
	spi_write_sensor(ADBM_A320_OFN_AD_DTH_HIGH_ADDR, 0x3C);
	spi_write_sensor(ADBM_A320_OFN_AD_ATH_LOW_ADDR, 0x18);
	spi_write_sensor(ADBM_A320_OFN_AD_DTH_LOW_ADDR, 0x20);

	// FPD
	spi_write_sensor(ADBM_A320_OFN_FPD_CTRL_ADDR, 0x50);
	spi_write_sensor(0x76, 0x0F);

	// XYQ
	spi_write_sensor(ADBM_A320_OFN_QUANTIZE_CTRL_ADDR, 0x99);
	spi_write_sensor(ADBM_A320_OFN_XYQ_THRESH_ADDR, 0x02);

	// Read from registers 0x02, 0x03 and 0x04
	spi_temp = spi_read_sensor(ADBM_A320_MOTION_ADDR);
	spi_temp = spi_read_sensor(ADBM_A320_DELTAX_ADDR);
	spi_temp = spi_read_sensor(ADBM_A320_DELTAY_ADDR);

	// Lengthen Rest1 to Rest2 downshift time
	spi_write_sensor(0x29, 0xC1);
	spi_write_sensor(0x15, 0xFF);
	spi_write_sensor(0x29, 0xA1);
	*/
}


void Init_Avago(void)
{
	/*
	Power_Up_Sequence();
	spi_write_sensor(ADBM_A320_CONFIGURATIONBITS_ADDR, 0x80);	// Configuration register data (Normal 800CPI)
	*/
}


void Init_PIXART(void)
{
	/*
	spi_write_sensor(PIXART_PAW3002OL_Write_Protect, 0x5A);
	spi_write_sensor(PIXART_PAW3002OL_Operation_Mode, 0x09);
	spi_write_sensor(PIXART_PAW3002OL_Configuration, 0x07);
	spi_write_sensor(PIXART_PAW3002OL_Operation_State, 0x70);
	spi_write_sensor(PIXART_PAW3002OL_Navimode1, 0x30);
	spi_write_sensor(PIXART_PAW3002OL_DeflickerSwitch, 0x0D);
	spi_write_sensor(PIXART_PAW3002OL_Write_Protect, 0x00);
	*/
}


void Init_SPIMouseSensor(void)
{
	BYTE index;

	Enable_SPIInterface();

	SPI_Mouse_ID = 0x00;	// Clear device ID
	for (index = 0x00; index < 3; index++)
	{
		SPI_Mouse_ID = spi_read_sensor(ADBM_A320_PRODUCTID_ADDR);
		if (SPI_Mouse_ID == SPI_Mouse_Sensor_ID_Avago || SPI_Mouse_ID == SPI_Mouse_Sensor_ID_PIXART) {break;}
	}

	if (SPI_Mouse_ID == SPI_Mouse_Sensor_ID_Avago)
	{
		Init_Avago();
	}
	else if (SPI_Mouse_ID == SPI_Mouse_Sensor_ID_PIXART)
	{
		Init_PIXART();
	}

	ClearSPIMouseSensorBuf();
}


void CheckMotionPin(void)
{
	/*
	BYTE spi_temp;
	XWORD XMovement_Templ;
	XWORD YMovement_Templ;
	BYTE temp;

	if (MouseDriverIn)
	{
		if (Read_Motion())
		{
			if (SPI_Mouse_Buf_Index != 0x00) {return;}

			if (SPI_Mouse_ID == SPI_Mouse_Sensor_ID_PIXART)
			{
				temp = spi_read_sensor(PIXART_PAW3002OL_Motion_Status);
			}

			if (SPI_Mouse_ID == SPI_Mouse_Sensor_ID_Avago)
			{
				SPI_Mouse_DeltaX = spi_read_sensor(ADBM_A320_DELTAX_ADDR);
			}
			else if (SPI_Mouse_ID == SPI_Mouse_Sensor_ID_PIXART)
			{
				SPI_Mouse_DeltaX = spi_read_sensor(PIXART_PAW3002OL_EnhancedDelta_X);
			}

			if (SPI_Mouse_DeltaX < 0x80)
			{
				if (IsFlag1(SPI_Mouse_BYTE1, X_Signbit)) {SPI_Mouse_XMovement = 0x00;}

				SPI_Mouse_BYTE1 &= ~X_Signbit;

				XMovement_Templ = SPI_Mouse_XMovement;
				XMovement_Templ += SPI_Mouse_DeltaX;
				if (XMovement_Templ > 0xFF)
				{
					SPI_Mouse_XMovement = 0xFF;
					SetFlag(SPI_Mouse_BYTE1, X_Overflow);
				}
				else
				{
					SPI_Mouse_XMovement = (BYTE)XMovement_Templ;
					ClrFlag(SPI_Mouse_BYTE1, X_Overflow);
				}
			}
			else
			{
				if (IsFlag0(SPI_Mouse_BYTE1, X_Signbit)) {SPI_Mouse_XMovement = 0x00;}

				SPI_Mouse_BYTE1 |= X_Signbit;
				SPI_Mouse_DeltaX = (~SPI_Mouse_DeltaX)+1;

				XMovement_Templ = SPI_Mouse_XMovement;
				XMovement_Templ += SPI_Mouse_DeltaX;
				if (XMovement_Templ > 0xFF)
				{
					SPI_Mouse_XMovement = 0x01;
					SetFlag(SPI_Mouse_BYTE1, X_Overflow);
				}
				else
				{
					SPI_Mouse_XMovement = (BYTE)XMovement_Templ;
					SPI_Mouse_XMovement = (~SPI_Mouse_XMovement) + 1;
					ClrFlag(SPI_Mouse_BYTE1, X_Overflow);
				}
			}

			if (SPI_Mouse_ID == SPI_Mouse_Sensor_ID_Avago)
			{
				SPI_Mouse_DeltaY = spi_read_sensor(ADBM_A320_DELTAY_ADDR);
			}
			else if(SPI_Mouse_ID == SPI_Mouse_Sensor_ID_PIXART)
			{
				SPI_Mouse_DeltaY = spi_read_sensor(PIXART_PAW3002OL_EnhancedDelta_Y);
			}

			if (SPI_Mouse_DeltaY < 0x80)
			{
				if (IsFlag1(SPI_Mouse_BYTE1, Y_Signbit)) {SPI_Mouse_YMovement = 0x00;}

				SPI_Mouse_BYTE1 &= ~Y_Signbit;

				YMovement_Templ = SPI_Mouse_YMovement;
				YMovement_Templ += SPI_Mouse_DeltaY;
				if (YMovement_Templ > 0xFF)
				{
					SPI_Mouse_YMovement = 0xFF;
					SetFlag(SPI_Mouse_BYTE1, Y_Overflow);
				}
				else
				{
					SPI_Mouse_YMovement = (BYTE)YMovement_Templ;
					ClrFlag(SPI_Mouse_BYTE1, Y_Overflow);
				}
			}
			else
			{
				if (IsFlag0(SPI_Mouse_BYTE1, Y_Signbit)) {SPI_Mouse_YMovement=0x00;}

				SPI_Mouse_BYTE1 |= Y_Signbit;
				SPI_Mouse_DeltaY= (~SPI_Mouse_DeltaY)+1;

				YMovement_Templ = SPI_Mouse_YMovement;
				YMovement_Templ += SPI_Mouse_DeltaY;
				if (YMovement_Templ > 0xFF)
				{
					SPI_Mouse_YMovement = 0x01;
					SetFlag(SPI_Mouse_BYTE1, Y_Overflow);
				}
				else
				{
					SPI_Mouse_YMovement = (BYTE)YMovement_Templ;
					SPI_Mouse_YMovement = (~SPI_Mouse_YMovement)+1;
					ClrFlag(SPI_Mouse_BYTE1, Y_Overflow);
				}
			}

			if (Read_Left_Button())
			{
				SetFlag(SPI_Mouse_BYTE1, LeftBtn);
			}
			else
			{
				ClrFlag(SPI_Mouse_BYTE1, LeftBtn);
			}
			//ClrFlag(SPI_Mouse_Misc, SPI_Mouse_Need_Break);

			SPI_Mouse_Buf[2] = SPI_Mouse_BYTE1 |= Always1;
			SPI_Mouse_Buf[1] = SPI_Mouse_XMovement;
			SPI_Mouse_Buf[0] = SPI_Mouse_YMovement;

			if (SPI_Mouse_Buf_Index == 0x00) {SPI_Mouse_Buf_Index = 0x03;}
		}
		else
		{
			SPI_Mouse_XMovement = 0x00;
			SPI_Mouse_YMovement = 0x00;
		}
	}
	*/
}


void EmulationSendMouseData(void)
{
	/*
	if (MouseDriverIn)
	{
		if (SPI_Mouse_Buf_Index != 0x00)
		{
			if (IS_MASK_SET(KBHISR, OBF) || IS_MASK_SET(KBHISR, IBF) || Ccb42_DISAB_AUX)
			{

			}
			else
			{
				SPI_Mouse_Buf_Index--;
				SendFromAux(SPI_Mouse_Buf[SPI_Mouse_Buf_Index]);
				if (SPI_Mouse_Buf_Index == 0x02)
				{
					ClrFlag(SPI_Mouse_BYTE1, Y_Overflow);
					ClrFlag(SPI_Mouse_BYTE1, X_Overflow);
				}
				else if (SPI_Mouse_Buf_Index == 0x01)
				{
					SPI_Mouse_XMovement = 0x00;
				}
				else if (SPI_Mouse_Buf_Index == 0x00)
				{
					SPI_Mouse_YMovement = 0x00;
					if (IsFlag1(SPI_Mouse_Misc, SPI_Mouse_Need_Break))
					{
						ClrFlag(SPI_Mouse_Misc, SPI_Mouse_Need_Break);
						SPI_Mouse_Buf_Index = 0x03;
						SPI_Mouse_BYTE1 = Always1;
						SPI_Mouse_Buf[2]=SPI_Mouse_BYTE1;
						SPI_Mouse_Buf[1]=0x00;
						SPI_Mouse_Buf[0]=0x00;
					}
				}
			}
		}
	}
	*/
}


void ProcessLeftBtnPress(void)
{
	/*
	if (MouseDriverIn)
	{
		if (SPI_Mouse_Buf_Index == 0x00)
		{
			SPI_Mouse_Buf_Index = 0x03;
			SPI_Mouse_BYTE1 = Always1+LeftBtn;
			SPI_Mouse_Buf[2] = SPI_Mouse_BYTE1;
			SPI_Mouse_Buf[1] = 0x00;
			SPI_Mouse_Buf[0] = 0x00;
			ClrFlag(SPI_Mouse_Misc, SPI_Mouse_Need_Break);
		}
		else
		{

		}
	}
	*/
}


void ProcessLeftBtnRelease(void)
{
	/*
	if (MouseDriverIn)
	{
		if (SPI_Mouse_Buf_Index == 0x00)
		{
			SPI_Mouse_Buf_Index = 0x03;
			SPI_Mouse_BYTE1 = Always1;
			SPI_Mouse_Buf[2] = SPI_Mouse_BYTE1;
			SPI_Mouse_Buf[1] = 0x00;
			SPI_Mouse_Buf[0] = 0x00;
		}
		else
		{
			SetFlag(SPI_Mouse_Misc, SPI_Mouse_Need_Break);
		}
	}
	*/
}


void ClearSPIMouseSensorBuf(void)
{
	SPI_Mouse_Buf[2] = 0x00;
	SPI_Mouse_Buf[1] = 0x00;
	SPI_Mouse_Buf[0] = 0x00;
	SPI_Mouse_DeltaX = 0x00;
	SPI_Mouse_DeltaY = 0x00;
	SPI_Mouse_BYTE1 = 0x00;
	SPI_Mouse_XMovement = 0x00;
	SPI_Mouse_YMovement = 0x00;
	SPI_Mouse_Buf_Index = 0x00;
	SPI_Mouse_Misc = 0x00;
}

