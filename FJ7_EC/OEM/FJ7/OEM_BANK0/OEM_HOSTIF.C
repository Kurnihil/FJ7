/*------------------------------------------------------------------------------
 * Title : OEM_HOSTIF.C
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


const BYTE code initsio_table[] =
{
	// Configure logical device 06h (KBD)
	0x07, 0x06,	// Select logical device 06h (KBD)
	0x70, 0x01,	// Set IRQ=01h for logical device 06h (KBD)
	0x30, 0x01,	// Enable logical device 06h (Mouse)

	// Configure logical device 05h (Mouse)
	0x07, 0x05,	// Select logical device 05h (Mouse)
	0x70, 0x0C,	// Set IRQ=0Ch for logical device 05h (Mouse)
	0x30, 0x01,	// Enable logical device 05h (Mouse)

	// Configure logical device 11h (PM1)
	0x07, 0x11,	// Select logical device 11h (PM1)
	0x70, 0x00,	// Clear IRQ=0 for logical device 11h (PM1)
	0x30, 0x01,	// Enable logical device 11h (PM1)

	// Configure logical device 12h (PM2)
	0x07, 0x12,	// Select logical device 12h (PM2)
	0x70, 0x00,	// Clear IRQ=0 for logical device 12h (PM2)
	0x30, 0x01,	// Enable logical device 12h (PM2)

	// Configure logical device 04h (MSWC)
	0x07, 0x04,	// Select logical device 04h (MSWC)
	0x30, 0x00,	// Enable logical device 04h (MSWC)

	#if	CIRFuncSupport
	// Configure logical device 0Ah (CIR)
	0x07, 0x0A,	// Select logical device 0Ah (CIR)
	0x60, 0x03,	// IO Port 300
	0x61, 0x00,	//
	0x70, 0x0A,	// Clear IRQ=10 for CIR
	0x30, 0x01,	// Enable logical device 0Ah (CIR)
	#endif

	// Configure logical device 10h (BRAM)
	0x07, 0x10,	// Select logical device 10h (BRAM)
	0x62, 0x03,	//
	0x63, 0x80,	// BRAMLD address is 0x380 (index) and 0x381 (data)
	0x30, 0x01,	// Enable logical device 10h (BRAM)

	// Configure logical device 0Fh (Shared Memory)
	0x07, 0x0F,	// Select logical device 0Fh (Shared Memory)
//  0xF4, 0x09,	// F0h ~ F9h for device specific logical device configuration
	#ifdef	SysMemory2ECRam	// Define EC internal RAM base address on LPC memory space
	0xF5, 0xC0,	// H2RAM-HLPC base address bits [15:12] (HLPCRAMBA[15:12]) high nibble
	0xF6, 0xFF,	// H2RAM-HLPC base address bits [23:16] (HLPCRAMBA[23:16])
	#endif
	0x30, 0x01,	// Enable logical device 0Fh (Shared Memory)

	#ifdef	PMC3_Support
	// Configure logical device 17h (PM3)
	0x07, 0x17,	// Select logical device 17h (PM3)
	0x62, 0x06,	// IO Port 6A4
	0x63, 0xA4,	//
	0x60, 0x06,	// IO Port 6A0
	0x61, 0xA0,	//
	0x70, 0x01,	// Clear IRQ=1 for logical device 13h (PM3)
	0x30, 0x01,	// Enable logical device 17h (PM3)
	#endif

	#ifdef	UART1_SUPPORT
	// Configure logical device 01 (UART1)
	0x07, 0x01,	// Select logical device 01 (UART1)
	0x30, 0x01,	// Enable logical device 01 (UART1)
	#endif

	#ifdef	UART2_SUPPORT
	// Configure logical device 02 (UART2)
	0x07, 0x02,	// Select logical device 02 (UART2)
	0x30, 0x01,	// Enable logical device 02 (UART2)
	#endif

	#ifdef	LogP80InECRamDxx
	// Configure logical device 10h (RTC)
	0x07, 0x10,	// Select logical device 10h (RTC)
	0xF3, 0x00,	// P80LB begin index
	0xF4, 0x00,	// P80LE end index
	0xF5, 0x00,	// P80LC current index
	#endif
};


//------------------------------------------------------------------------------
// Initial Super I/O
//------------------------------------------------------------------------------
void InitSio(void)
{
	BYTE code *data_pntr;
	BYTE cnt;

	SET_MASK(LSIOHA, LKCFG);
	SET_MASK(IBMAE, CFGAE);
	SET_MASK(IBCTL, CSAE);

	cnt = 0;
	data_pntr = initsio_table;
	while (cnt < (sizeof(initsio_table) / 2))
	{
		IHIOA = 0;	// Set indirect Host I/O Address
		IHD = *data_pntr;
		while (IS_MASK_SET(IBCTL, CWIB));
		data_pntr++;
		IHIOA = 1;	// Set indirect Host I/O Address
		IHD = *data_pntr;
		while (IS_MASK_SET(IBCTL, CWIB));
		data_pntr++;
		cnt++;
	}

	CLR_MASK(LSIOHA, LKCFG);
	CLR_MASK(IBMAE, CFGAE);
	CLR_MASK(IBCTL, CSAE);
}


//------------------------------------------------------------------------------
// Initial system memory cycle to EC external RAM
//------------------------------------------------------------------------------
void InitSysMemory2ECRam(void)
{
	HRAMW0BA = 0xA0;					// Define RAM window 0 base address 0xA00
	HRAMW0AAS |= HostRamSize256Byte;	// Host RAM Window 0 Size 256 bytes
	//HRAMWC |= Window0En;
}

