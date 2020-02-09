/*------------------------------------------------------------------------------
 * Filename: OEM_VER.C          For Chipset: ITE.IT85XX
 * Function: Project version
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


//------------------------------------------------------------------------------
// Project Information
//------------------------------------------------------------------------------
//                           0123456789ABCDEF   // 16th (F) for '\0' of string
const BYTE code P_model[] = "FJ7           #";  // Project information
const BYTE code P_date[]  = "2018/02/22b00 #";  // Latest modified date         // yyyy/MM/ddbXX (bXX for build count)
const BYTE code P_rev[]   = "0.02 #";           // Project revision             // ' ': Formal release, 'T': Test build
const BYTE code P_digit[] = {0, 0};             // Project extended rev


// leox20150121 For check EC version
// leox20150522 From HP N61A
// leox20150721 For check EC version and moved code from Init_OEMVariable()
void LoadEcVer(void)
{
	VerY = ((P_date[2]  - '0') << 4) + (P_date[3]  - '0');
	VerM = ((P_date[5]  - '0') << 4) + (P_date[6]  - '0');
	VerD = ((P_date[8]  - '0') << 4) + (P_date[9]  - '0');
	VerB = ((P_date[11] - '0') << 4) + (P_date[12] - '0');
	VerMJ = P_rev[0] - '0';
	VerMN = ((P_rev[2] - '0') << 4) + (P_rev[3] - '0');
	VerFR = P_rev[4];
}
