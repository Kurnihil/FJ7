/*------------------------------------------------------------------------------
 * Title : OEM_FLASH.H
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#ifndef	OEM_FLASH_H
#define	OEM_FLASH_H


//------------------------------------------------------------------------------
// Common Definition
//------------------------------------------------------------------------------
// leox20151030 Access eFlash +++	// SUPPORT_EFLASH_FUNC	// leox20160322 Code from N83
#define EFL_I_VER	0x24	// eFlash function version		// BIT[7-4]:Major, BIT[3-0]:Minor	// leox20151104	// leox20160316 eFlash version
#define EFL_R_B		100		// eFlash record range Begin	// Range: 96 ~ 126 (KB), do NOT use block 127
#define EFL_R_E		107		// eFlash record range End		// Block 127 reserved for ITE eFlash protect table
// leox20151030 Access eFlash ---


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void RamProgram(BYTE useport);
extern void RamCode(void);
extern void OEM_SPIProtectAll(void);
extern void OEM_SPIUnlockAll(void);
extern void OEM_Write_SPI_256Bytes(void);
extern void OEM_Read_SPI_256Bytes(void);
extern void OEM_SPI_Read_ID(void);
extern BYTE OEM_SPIReadStatus(void);
extern void ForITEWinFlashTool(void);			// For ITE WinFlash Tool		// ken 2012/06/29

// leox20150630 Access eFlash +++				// leox20160322 Code from N83	// SUPPORT_EFLASH_FUNC
extern void eFlashDbgFunc(void);				// leox20150624	// leox20150630	// leox20151030
extern void eFlashInit(void);					// leox20150630	// leox20151030
extern void eFlashFormat(WORD BlkNum);			// leox20150630	// leox20151030
extern void eFlashSave(XBYTE *Adr, BYTE Val);	// leox20150701	// leox20151030
extern void eFlashLoad(BYTE Adr, BYTE Val);		// leox20150703	// leox20151030
extern void eFlashSaveAll(void);				// leox20151103
extern void eFlashToDo(void);					// leox20160316
// leox20150630 Access eFlash ---


#endif	/* OEM_FLASH_H */
