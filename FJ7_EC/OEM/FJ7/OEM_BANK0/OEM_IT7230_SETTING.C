#include "CORE\INCLUDE\CORE_INCLUDE.H"
#include "OEM\INCLUDE\OEM_INCLUDE.H"

/////////////////////////////////////////////////////////////////////////////////
//-----------------------------IT7230 init table-------------------------------//
/////////////////////////////////////////////////////////////////////////////////
#ifdef IT_7230MMB_SUPPORT
const sInitCapSReg code asInitCapSReg[] = {
{ PAGE_1,  CAPS_PCR      ,0x0001},
{ PAGE_1,  CAPS_PSR      ,0x0001},
{ PAGE_1,  CAPS_PMR      ,0x0008},
{ PAGE_1,  CAPS_RTR      ,0x007F},
{ PAGE_1,  CAPS_CTR      ,0x003F},
{ PAGE_1,  CAPS_CRMR     ,0x0020},
{ PAGE_1,  CAPS_PDR      ,0x0200},
{ PAGE_1,  CAPS_DR       ,0x0050},
{ PAGE_1,  CAPS_S0CR     ,0x8034},
{ PAGE_1,  CAPS_S1CR     ,0x8012},
{ PAGE_1,  CAPS_S2CR     ,0x8079},
{ PAGE_1,  CAPS_C1COR    ,0x4080},
{ PAGE_1,  CAPS_C2COR    ,0x0BC0},
{ PAGE_1,  CAPS_C3COR    ,0x4080},
{ PAGE_1,  CAPS_C4COR    ,0x0BC0},
{ PAGE_1,  CAPS_C7COR    ,0x4080},
{ PAGE_1,  CAPS_C9COR    ,0x0BC0},
{ PAGE_1,  CAPS_ICR0     ,0xFEEF},
{ PAGE_1,  CAPS_ICR1     ,0x0FFB},
{ PAGE_1,  CAPS_COER0    ,0xFFFF},
{ PAGE_1,  CAPS_COER1    ,0x03FF},
{ PAGE_1,  CAPS_CGCR     ,0x0001},

{ PAGE_1,  CAPS_LEDBR    ,0x0000},
{ PAGE_1,  CAPS_GPIODR   ,0x0000},
//{ PAGE_1,  CAPS_GPIODR ,0x001C},		//LED 234 is out put pin
{ PAGE_1,  CAPS_GPIOOR   ,0x0000},
{ PAGE_1,  CAPS_GPIOMR   ,0x0000},
{ PAGE_1,  CAPS_GPIOLR   ,0x001C},		//LED GPIO high active
{ PAGE_1,  CAPS_GPIOER   ,0x0000},

//{ PAGE_1,  CAPS_LEDCMR0,0x12DD},
//{ PAGE_1,  CAPS_LEDCMR1,0xDDD0},
{ PAGE_1,  CAPS_LEDCMR0  ,0xFFDD},		//LEDx mapping to CDC stage
{ PAGE_1,  CAPS_LEDCMR1  ,0xDDDF},		//LEDx mapping to CDC stage
{ PAGE_1,  CAPS_LEDCMR2  ,0xDDDD},		//LEDx mapping to CDC stage
{ PAGE_1,  CAPS_LEDCMR3  ,0x0DDD},		//LEDx mapping to CDC stage

{ PAGE_1,  CAPS_LEDRPR   ,0x3030},
{ PAGE_1,  CAPS_LEDBR    ,0x001F},		// LED off = 0mA ,LED on = 7 mA
{ PAGE_1,  CAPS_LEDCGCR  ,0x001C},		// driver current double (15mA)
{ PAGE_1,  CAPS_LEDPR0   ,0x2233},
{ PAGE_1,  CAPS_LEDPR1   ,0x3332},
{ PAGE_1,  CAPS_LEDPR2   ,0x3333},
{ PAGE_1,  CAPS_LEDPR3   ,0x0333},
{ PAGE_1,  CAPS_GPIOMSR  ,0x001C},		//Led Drivering mode , 0 : I/O mode, 1: LED driving mode
//{ PAGE_1,  CAPS_GPIOMSR,0x0000},		//change LED 234 to GPIO mode

{ PAGE_0,  CAPS_S0DLR    ,0x8000},
{ PAGE_0,  CAPS_S0OHCR   ,0x0600},
{ PAGE_0,  CAPS_S0OLCR   ,0x7000},
{ PAGE_0,  CAPS_S0SR     ,0xCF8F},
{ PAGE_0,  CAPS_S1DLR    ,0x8000},
{ PAGE_0,  CAPS_S1OHCR   ,0x0600},
{ PAGE_0,  CAPS_S1OLCR   ,0x7000},
{ PAGE_0,  CAPS_S1SR     ,0xCF8F},
{ PAGE_0,  CAPS_S2DLR    ,0x8000},
{ PAGE_0,  CAPS_S2OHCR   ,0x0600},
{ PAGE_0,  CAPS_S2OLCR   ,0x7000},
{ PAGE_0,  CAPS_S2SR     ,0xCF8F},
{ PAGE_0,  CAPS_SXCHAIER ,0x0007},
{ PAGE_0,  CAPS_SXCHRIER ,0x0007},
{ PAGE_0,  CAPS_SXCLAIER ,0x0007},
{ PAGE_0,  CAPS_SXCLRIER ,0x0007},
{ PAGE_1,  CAPS_GPIONPCR ,0x1FFF},
{ PAGE_1,  CAPS_CFER     ,0xC000},
{ PAGE_1,  CAPS_PCR      ,0x2406}
};

void CapS_PowerOn(void)
{
	BYTE temp = 0x00;
	while(temp < (sizeof(asInitCapSReg)/sizeof(sInitCapSReg)))
	{
		CapS_Write_Reg(asInitCapSReg[temp].page, asInitCapSReg[temp].reg, asInitCapSReg[temp].value);
		temp++;
		if(1 == temp)
		{
			DelayXms(0x01);
		}
	}
}

void resetIT7230(void)
{
	if (MMB_Detect_Low())
	{
		CapS_Init();
		CapS_PowerOn();
		CapS_Write_Reg(PAGE_1, CAPS_RTR, 0x0000);
		CapS_Write_Reg(PAGE_1, CAPS_CTR, 0x0000);
		CapS_Write_Reg(PAGE_1, CAPS_CFER, 0x4000);
		//David modify for reset timeing 2014/07/24
		DelayXms(250);
		DelayXms(250);
		DelayXms(50);
		//David modify for reset timeing 2014/07/24
		CapS_Write_Reg(PAGE_1, CAPS_RTR, 0x007F);
		CapS_Write_Reg(PAGE_1, CAPS_CTR, 0x003F);
		CapS_Write_Reg(PAGE_1, CAPS_CFER, 0xC000);
		CapS_Read_Reg(PAGE_0, CAPS_SIR);
	}
}
#endif	// IT_7230MMB_SUPPORT
