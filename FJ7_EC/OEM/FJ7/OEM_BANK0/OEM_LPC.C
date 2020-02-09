/*------------------------------------------------------------------------------
 * Title : OEM_LPC.C
 * Author: Dino
 * Note  : These functions are for reference only.
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//------------------------------------------------------------------------------
// The example of writeing protect 0x180000 ~ 0x1FFFFF
//  bit 23  P1BA0R.bit3
//  bit 22  P1BA0R.bit2
//  bit 21  P1BA0R.bit1
//  bit 20  P1BA0R.bit0
//  bit 19  P1BA1R.bit7
//  bit 18  P1BA1R.bit6
//  bit 17  P1BA1R.bit5
//  bit 16  P1BA1R.bit4
//  bit 15  P1BA1R.bit3
//  bit 14  P1BA1R.bit2
//  bit 13  P1BA1R.bit1
//  bit 12  P1BA1R.bit0
//------------------------------------------------------------------------------
void HLPC_WriteProtect0(void)
{
	P0BA0R = BIT0;
	P0BA1R = BIT7;
	HLPC_Protect0Mode(ProtectMode_WriteProtection);
	HLPC_Protect0Size(ProtectSize_512K);
	HLPC_LockProtectRelatedRegisters();
}


void HLPC_WriteProtectEnable0(void)
{
	P0BA0R = 0x00;
	P0BA1R = 0x00;
	P0ZR = 0x00;
}

