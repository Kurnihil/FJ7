/*-----------------------------------------------------------------------------------
 * Filename: IFU.Cpp         For Chipset: ITE.IT85X2
 *
 * Function: EC flash  utility
 *
 * Author  : Dino March 2009 
 * 
 * Copyright (c) 2009 - , ITE Tech. Inc. All Rights Reserved. 
 *
 * You may not present,reproduce,distribute,publish,display,modify,adapt,
 * perform,transmit,broadcast,recite,release,license or otherwise exploit
 * any part of this publication in any form,by any means,without the prior
 * written permission of ITE Tech. Inc.	
 *---------------------------------------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include <dir.h>
#include <alloc.h>
#include <bios.h>
#include <process.h>
#include <time.h>


//---------------------------------------------------------------------------
// Type define
//---------------------------------------------------------------------------
typedef unsigned char   BYTE;
typedef unsigned int   	WORD;
typedef unsigned long   DWORD;

#define Version					"1.1.3"

#define EC_STATUS_port          0x66
#define EC_CMD_port             0x66
#define EC_DATA_port            0x62
#define EC_OBF                  0x01
#define EC_IBF                  0x02

#define KB_STATUS_port          0x64
#define KB_CMD_port             0x64
#define KB_DATA_port            0x60
#define KB_OBF                  0x01
#define KB_IBF                  0x02

#define SPICmd_WRSR				0x01	// Write Status Register
#define SPICmd_BYTEProgram		0x02	// To Program One Data Byte
#define SPICmd_WRDI				0x04	// Write diaable
#define SPICmd_ReadStatus		0x05	// Read Status Register
#define SPICmd_WREN				0x06	// Write Enable
#define SPICmd_HighSpeedRead	0x0B	// High-Speed Read
#define SPICmd_EWSR				0x50	// Enable Write Status Register
#define SPICmd_RDID			    0xAB	// Read ID
#define SPICmd_DeviceID			0x9F	// Manufacture ID command
#define SPICmd_AAIWordProgram	0xAD	// Auto Address Increment Programming (word)
#define SPICmd_AAIProgram		0xAF	// Auto Address Increment Programming (byte)
#define SPICmd_64KByteBE		0xD8	// Erase 64 KByte block of memory array

#define SSTID					0xBF
#define WinbondID				0xEF
#define AtmelID					0x9F
#define STID					0x20
#define SpansionID				0x01
#define MXICID					0xC2
#define AMICID					0x37
#define EONID					0x1C
#define ESMTID					0x8C


WORD counter;
WORD counter2;

BYTE *str1,*str2;
BYTE BlockNum;
BYTE Old_SSTDeviceID;
BYTE Old_DeviceID;

FILE *fi;
WORD len1,len2;
BYTE SPIDeviceID[4];


struct devicevendor
{
	BYTE muid;
	void (* showid) (void);	
};

void vShowSSTID(void)
{
	printf("SPI Vendor       : SST \n");
}

void vShowWinbondID(void)
{
	printf("SPI Vendor       : Winbond \n");
}

void vShowAtmelID(void)
{
	printf("SPI Vendor       : Atmel \n");
}

void vShowSTID(void)
{
	printf("SPI Vendor       : ST \n");
}

void vShowSpansionID(void)
{
	printf("SPI Vendor       : Spansion \n");
}

void vShowMXICID(void)
{
	printf("SPI Vendor       : MXIC \n");
}

void vShowAMICID(void)
{
	printf("SPI Vendor       : AMIC \n");
}

void vShowEONID(void)
{
	printf("SPI Vendor       : EON \n");
}

void vShowESMTID(void)
{
	printf("SPI Vendor       : ESMT \n");
}

struct devicevendor adevicevendor[]=
{
	{	SSTID,			vShowSSTID			},
	{	WinbondID,		vShowWinbondID		},
	{	AtmelID,		vShowAtmelID		},
	{	STID,			vShowSTID			},
	{	SpansionID,		vShowSpansionID		},
	{	MXICID,			vShowMXICID			},
	{	AMICID,			vShowAMICID			},
	{	EONID,			vShowEONID			},
	{	ESMTID,			vShowESMTID			},
};
//------------------------------------
// wait EC output buffer full
//------------------------------------
void wait_EC_OBF (void)
{
  	while (!(inportb(EC_STATUS_port) & EC_OBF));
}

//void wait_EC_OBE (void)
//{
//  	while (inportb(EC_STATUS_port) & EC_OBF);
    //inportb(EC_DATA_port);
//}

//------------------------------------
// wait EC input buffer empty
//------------------------------------
void wait_EC_IBE (void)
{
  	while (inportb(EC_STATUS_port) & EC_IBF);
}


void send_EC_cmd (BYTE EcCmd)
{
  	//wait_EC_OBE();
  	wait_EC_IBE();
  	outportb(EC_CMD_port, EcCmd);
  	wait_EC_IBE();
}

//------------------------------------
// send EC data
//------------------------------------
void send_EC_data(BYTE EcData)
{
  	//wait_EC_OBE();
  	wait_EC_IBE();
  	outportb(EC_DATA_port, EcData);
  	wait_EC_IBE();
}

BYTE Read_EC_data(void)
{
	wait_EC_OBF();
 	return(inportb(0x62));
}

//------------------------------------
// wait KB output buffer full
//------------------------------------
void wait_KB_OBF (void)
{
  	while (!(inportb(KB_STATUS_port) & KB_OBF));
}

void wait_KB_OBE (void)
{
  	while (inportb(KB_STATUS_port) & KB_OBF)
    inportb(KB_DATA_port);
}

//------------------------------------
// wait KB input buffer empty
//------------------------------------
void wait_KB_IBE (void)
{
  	while (inportb(KB_STATUS_port) & KB_IBF);
}


void send_KB_cmd (BYTE KBCmd)
{
  	wait_KB_OBE();
  	wait_KB_IBE();
  	outportb(KB_CMD_port, KBCmd);
  	wait_KB_IBE();
}

//------------------------------------
// send KB data
//------------------------------------
void send_KB_data(BYTE KBData)
{
  	wait_KB_OBE();
  	wait_KB_IBE();
  	outportb(KB_DATA_port, KBData);
  	wait_KB_IBE();
}

BYTE Read_KB_data(void)
{
	wait_KB_OBF();
 	return(inportb(0x62));
}


void vEnterFollowMode(void)
{
	send_EC_cmd(0x01);
}

void vExitFollowMode(void)
{
	send_EC_cmd(0x05);
}

void vSendSPICommand(BYTE cmd)
{
	send_EC_cmd(0x02);
	send_EC_cmd(cmd);
}

void vSendSPIByte(BYTE index)
{
	send_EC_cmd(0x03);
	send_EC_cmd(index);
}

BYTE BReadByteFromSPI(void)
{
	send_EC_cmd(0x04);
	return(Read_EC_data());
}

void vWaitSPIFree(void)
{
	vEnterFollowMode();
	vSendSPICommand(SPICmd_ReadStatus);
	while(1)
	{
		if((BReadByteFromSPI()&0x01)==0x00)
        {
            break;
        } 
	}
	vExitFollowMode();
}

void vWaitWriteEnable(void)
{
	vEnterFollowMode();
	vSendSPICommand(SPICmd_ReadStatus);
	while(1)
	{
		if((BReadByteFromSPI()&0x03)==0x02)
        {
            break;
        } 
	}
}

void vShowVersion(void)
{
	gotoxy(1,1);
	printf(" ITE Flash Utility Version : %s",Version); 
}


void vShowDeviceID(void)
{
	gotoxy(1,3);
	printf("Device ID        : %x %x %x %x ",SPIDeviceID[0],SPIDeviceID[1],SPIDeviceID[2],SPIDeviceID[3]); 
}

void vShowVendorID(void)
{
	BYTE index;
	
	gotoxy(1,4);

	for(index=0x00;index<(sizeof(adevicevendor)/5);index++)
	{
		if(SPIDeviceID[0]==adevicevendor[index].muid)
		{
			(adevicevendor[index].showid)();
		}
	}
}

void vShowBlockNumber(void)
{
	gotoxy(1,5);
	printf("\nBlock Number     : %d \n\n",(BlockNum-1));
}

void vReadSPIDeviceID_CmdAB(void)
{
	BYTE index;
	vWaitSPIFree();
	
	vEnterFollowMode();
	vSendSPICommand(SPICmd_RDID);
	vSendSPIByte(0x00);
	vSendSPIByte(0x00);
	vSendSPIByte(0x00);
	for(index=0x00;index<4;index++)
	{
		SPIDeviceID[index]=BReadByteFromSPI();
	}
	if(SPIDeviceID[0]==SSTID)
	{
		Old_SSTDeviceID = 1;
		Old_DeviceID = 0;
	}
	else
	{
		Old_SSTDeviceID = 0;
		Old_DeviceID = 1;
	}
	vExitFollowMode();
}

void vReadSPIDeviceID(void)
{
	BYTE index;
	
	SPIDeviceID[0]=0x00;
	vWaitSPIFree();
	vEnterFollowMode();
	vSendSPICommand(SPICmd_DeviceID);
	for(index=0x00;index<4;index++)
	{
		SPIDeviceID[index]=BReadByteFromSPI();
	}
	if(SPIDeviceID[0]==0x00)
	{
		vReadSPIDeviceID_CmdAB();
	}

	vExitFollowMode();
}


void vSPIWriteDisable(void)
{
	vWaitSPIFree();
	vEnterFollowMode();
	vSendSPICommand(SPICmd_WRDI);

	vEnterFollowMode();
	vSendSPICommand(SPICmd_ReadStatus);
	while(1)
	{
		if((BReadByteFromSPI()&0x02)==0x00)
        {
            break;
        } 
	}	
	vExitFollowMode();
}

void SPIUnlockAll(void)
{
	vWaitSPIFree();
	vEnterFollowMode();
	vSendSPICommand(SPICmd_WREN);

	vEnterFollowMode();
	vSendSPICommand(SPICmd_ReadStatus);
	while(1)
	{
		if((BReadByteFromSPI()&0x02)!=0x00)
		{
			break;
		} 
	}	
	
	if(SPIDeviceID[0]==SSTID)
	{
		vEnterFollowMode();
		vSendSPICommand(SPICmd_EWSR);
	}

	vEnterFollowMode();
	vSendSPICommand(SPICmd_WRSR);
	vSendSPIByte(0x00);

	vWaitSPIFree();
}

void vSPIWriteEnable(void)
{
	vWaitSPIFree();

	vEnterFollowMode();
	vSendSPICommand(SPICmd_WREN);

	if(SPIDeviceID[0]==SSTID)
	{
		vEnterFollowMode();
		vSendSPICommand(SPICmd_EWSR);
	}

	vEnterFollowMode();
	vSendSPICommand(SPICmd_ReadStatus);
	while(1)
	{
		if((BReadByteFromSPI()&0x03)==0x02)
		{
			break;
		} 
	}	
	vExitFollowMode();
}

BYTE vSPIVerifyErase64KByte(BYTE addr2,BYTE addr1,BYTE addr0)
{
	vSPIWriteDisable();
	
	vWaitSPIFree();
	vEnterFollowMode();
	vSendSPICommand(SPICmd_HighSpeedRead);
	vSendSPIByte(addr2);
	vSendSPIByte(addr1);
	vSendSPIByte(addr0);
	vSendSPIByte(0x00);		// fast read dummy byte


	printf("Erase Verify...  : ");

	for(counter2=0x00;counter2<2;counter2++)
	{
		for(counter=0x0000;counter<0x8000;counter++)
		{
			if(counter2==0x00)
			{
				if(BReadByteFromSPI()!=0xFF)
				{
					vWaitSPIFree();
					printf(" -- Verify Fail. \n"); 
					return(0);
				}
			}
			else
			{
				if(BReadByteFromSPI()!=0xFF)
				{
					vWaitSPIFree();
					printf(" -- Verify Fail. \n"); 
					return(0);
				}
			}
			
			if((counter%3276)==0x00)
			{
				printf("%c",0xDB);
			}
		}
	}


	vWaitSPIFree();

	printf(" -- Verify OK. \n\n"); 
	return(1);
}

void vSPIBlockErase64KByte(BYTE addr2,BYTE addr1,BYTE addr0)
{
	BYTE counter;
	BYTE status;
	//vSPIWriteEnable();

	if(Old_SSTDeviceID==1)
	{
		vSPIWriteEnable();
		vEnterFollowMode();
		vSendSPICommand(0x52);
		vSendSPIByte(addr2);
		vSendSPIByte(addr1);
		vSendSPIByte(addr0);
		vExitFollowMode();
		vWaitSPIFree();

		vSPIWriteEnable();
		vEnterFollowMode();
		vSendSPICommand(0x52);
		vSendSPIByte(addr2);
		vSendSPIByte(addr1+0x80);
		vSendSPIByte(addr0);
		vExitFollowMode();
		vWaitSPIFree();		
	}
	else
	{
		vSPIWriteEnable();
		vEnterFollowMode();
		vSendSPICommand(SPICmd_64KByteBE);
		vSendSPIByte(addr2);
		vSendSPIByte(addr1);
		vSendSPIByte(addr0);
		vExitFollowMode();
		vWaitSPIFree();

		if(Old_DeviceID==0x01)
		{
			vSPIWriteEnable();
			vEnterFollowMode();
			vSendSPICommand(SPICmd_64KByteBE);
			vSendSPIByte(addr2);
			vSendSPIByte(addr1+0x80);
			vSendSPIByte(addr0);
			vExitFollowMode();
			vWaitSPIFree();
		}
	}
	//vExitFollowMode();

	printf("Eraseing...      : ");
	for(counter=0x00;counter<22;counter++)
	{
		printf("%c",0xDB);
	}
	printf(" -- Erase OK. \n\n"); 

}

void vAAIProgram64KByte(BYTE addr2,BYTE addr1,BYTE addr0)
{
	BYTE index;

	vSPIWriteEnable();
	
	vEnterFollowMode();
	vSendSPICommand(SPICmd_AAIProgram);
	vSendSPIByte(addr2);
	vSendSPIByte(addr1);
	vSendSPIByte(addr0);

	printf("Programing...    : ");

	for(counter2=0x00;counter2<2;counter2++)
	{
		for(counter=0x0000;counter<0x8000;counter++)
		{
			if(counter2==0x00)
			{
				vSendSPIByte(str1[counter]);
			}
			else
			{
				vSendSPIByte(str2[counter]);
			}

			vWaitSPIFree();
			vEnterFollowMode();
			vSendSPICommand(SPICmd_AAIProgram);

			if((counter%3276)==0x00)
			{
				printf("%c",0xDB);
			}
		}
	}
	
	vSPIWriteDisable();
	vWaitSPIFree();

	printf(" -- Programing OK. \n\n"); 	
}

void vAAIWordProgram64KByte(BYTE addr2,BYTE addr1,BYTE addr0)
{
	BYTE index;
	index=0x00;

	vSPIWriteEnable();
	
	vEnterFollowMode();
	vSendSPICommand(SPICmd_AAIWordProgram);
	vSendSPIByte(addr2);
	vSendSPIByte(addr1);
	vSendSPIByte(addr0);

	printf("Programing...    : ");

	for(counter2=0x00;counter2<2;counter2++)
	{
		for(counter=0x0000;counter<0x8000;counter++)
		{
			if(counter2==0x00)
			{
				vSendSPIByte(str1[counter]);
			}
			else
			{
				vSendSPIByte(str2[counter]);
			}
			index++;
		
			if(index==0x02)
			{
				index = 0x00;
				vWaitSPIFree();
				vEnterFollowMode();
				vSendSPICommand(SPICmd_AAIWordProgram);
			}

			if((counter%3276)==0x00)
			{
				printf("%c",0xDB);
			}
		}
	}

	vSPIWriteDisable();
	vWaitSPIFree();

	printf(" -- Programing OK. \n\n"); 	
}

void ByteProgram64KByte(BYTE addr2,BYTE addr1,BYTE addr0)
{
	printf("Programing...    : ");

	for(counter2=0x0000;counter2<0x100;counter2++)
	{
		vSPIWriteEnable();
		//vWaitSPIFree();
		vEnterFollowMode();
		vSendSPICommand(SPICmd_BYTEProgram);
		vSendSPIByte(addr2);
		vSendSPIByte(addr1+(BYTE)counter2);
		vSendSPIByte(addr0);
		if(counter2<0x80)
		{
			for(counter=0x0000;counter<0x100;counter++)
			{
				vSendSPIByte(str1[counter+(counter2*256)]);
			}
		}
		else
		{
			for(counter=0x0000;counter<0x100;counter++)
			{
				vSendSPIByte(str2[counter+((counter2-0x80)*256)]);
			}
		}
		vWaitSPIFree();
		//vSPIWriteDisable();
		if((counter2%12)==0x00)
		{
			printf("%c",0xDB);
		}
	}
	vSPIWriteDisable();
	printf(" -- Programing OK. \n\n"); 	
}

BYTE vSPIVerify64KByte(BYTE addr2,BYTE addr1,BYTE addr0)
{
	//BYTE counter2;
	//WORD counter;
	//BYTE temp;

	vSPIWriteDisable();
	
	vWaitSPIFree();
	vEnterFollowMode();
	vSendSPICommand(SPICmd_HighSpeedRead);
	vSendSPIByte(addr2);
	vSendSPIByte(addr1);
	vSendSPIByte(addr0);
	vSendSPIByte(0x00);		// fast read dummy byte


	printf("Verify...        : ");

	for(counter2=0x00;counter2<2;counter2++)
	{
		for(counter=0x0000;counter<0x8000;counter++)
		{
			if(counter2==0x00)
			{
				if(str1[counter]!=BReadByteFromSPI())
				{
					vWaitSPIFree();
					printf(" -- Verify Fail. \n"); 
					return(0);
				}
			}
			else
			{
				if(str2[counter]!=BReadByteFromSPI())
				{
					vWaitSPIFree();
					printf(" -- Verify Fail. \n"); 
					return(0);
				}
			}
			
			if((counter%3276)==0x00)
			{
				printf("%c",0xDB);
			}
		}
	}


	vWaitSPIFree();

	printf(" -- Verify OK. \n"); 
	return(1);
}



BYTE vRead64KBytesToBuf(void)
{
	WORD index;
	
	len1=fread(str1,1,0x8000,fi);
	if(len1>0x00)
	{
		BlockNum++;
		
		if(len1<0x8000)
		{
			for(index=len1;index<0x8000;index++)
			{
				str1[index]=0xFF;
			}
			for(index=0x00;index<0x8000;index++)
			{
				str2[index]=0xFF;
			}
		}
		else
		{
			len2=fread(str2,1,0x8000,fi);
			if(len2<0x8000)
			{
				for(index=len2;index<0x8000;index++)
				{
					str2[index]=0xFF;
				}
			}
		}
		return(1);
	}
	return(0);
}

//----------------------------------------------------------------------------------
// Main function
//----------------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
	BYTE reset;
	char *end;
	long int val;
	BYTE Addroffset;
	reset=0x00;
	BlockNum = 0x00;
	
	send_KB_cmd(0xAD);
	clrscr();

	switch(argc)
	{
		case 1:
			break;

		case 2:
   			if ( (fi=fopen(argv[1],"rb"))!=NULL)
 			{	
 				str1=(BYTE *)malloc(0x8000);
   				str2=(BYTE *)malloc(0x8000);

				send_EC_cmd(0xDC);
				//send_EC_cmd(0xdf);
				if(Read_EC_data()==0x33)
				{
					//gotoxy(1,2);
					//printf("EC ACK OK. \n\n");  
				}
				else
				{
					gotoxy(1,2);
					printf("EC ACK Fail. \n\n");  
					break;
				}
				
				vReadSPIDeviceID();
				
				while(vRead64KBytesToBuf())
				{
					clrscr();
					vShowVersion();
					vShowDeviceID();
					vShowVendorID();
					vShowBlockNumber();

					SPIUnlockAll();
					
					vSPIBlockErase64KByte((BlockNum-1),0x00,0x00);
					
					if(vSPIVerifyErase64KByte((BlockNum-1),0x00,0x00))
					{
						if(SPIDeviceID[0]==SSTID)	// SST flash part
						{
							if(Old_SSTDeviceID==1)
							{
								vAAIProgram64KByte((BlockNum-1),0x00,0x00);
							}
							else
							{
								vAAIWordProgram64KByte((BlockNum-1),0x00,0x00);
							}
						}
						else						// other flash part
						{
							ByteProgram64KByte((BlockNum-1),0x00,0x00);
						}

						if(vSPIVerify64KByte((BlockNum-1),0x00,0x00))
						{
							reset=0x01;
							//send_EC_cmd(0xFE);		// Watch dog reset EC
						}
						else
						{
							reset=0x00;
							send_EC_cmd(0xFC);	// return to dos
						}					
					}
					else
					{
						reset=0x00;
						send_EC_cmd(0xFC);	// return to dos
					}	
				}
				if(reset==0x01)
				{
					send_EC_cmd(0xFE);		// Watch dog reset EC
				}
				free(str1);
				free(str2);
   			}
			else
			{

			}
			break;
		case 3:
			val = strtol(argv[2], &end, 16);
			if (*end || val<0 || val>127) 
			{
				printf("\n Invalid block number !!! \n");
			}
			else
			{
			Addroffset = (BYTE)val;
   			if ( (fi=fopen(argv[1],"rb"))!=NULL)
 			{	
 				str1=(BYTE *)malloc(0x8000);
   				str2=(BYTE *)malloc(0x8000);

				send_EC_cmd(0xDC);
				//send_EC_cmd(0xdf);
				if(Read_EC_data()==0x33)
				{
					//gotoxy(1,2);
					//printf("EC ACK OK. \n\n");  
				}
				else
				{
					gotoxy(1,2);
					printf("EC ACK Fail. \n\n");  
					break;
				}
				
				vReadSPIDeviceID();
				BlockNum += Addroffset;
				while(vRead64KBytesToBuf())
				{
					clrscr();
					vShowVersion();
					vShowDeviceID();
					vShowVendorID();
					vShowBlockNumber();

					SPIUnlockAll();
					
					vSPIBlockErase64KByte((BlockNum-1),0x00,0x00);
					
					if(vSPIVerifyErase64KByte((BlockNum-1),0x00,0x00))
					{
						if(SPIDeviceID[0]==SSTID)	// SST flash part
						{
							if(Old_SSTDeviceID==1)
							{
								vAAIProgram64KByte((BlockNum-1),0x00,0x00);
							}
							else
							{
								vAAIWordProgram64KByte((BlockNum-1),0x00,0x00);
							}
						}
						else						// other flash part
						{
							ByteProgram64KByte((BlockNum-1),0x00,0x00);
						}

						if(vSPIVerify64KByte((BlockNum-1),0x00,0x00))
						{
							reset=0x01;
							//send_EC_cmd(0xFE);		// Watch dog reset EC
						}
						else
						{
							reset=0x00;
							send_EC_cmd(0xFC);	// return to dos
						}					
					}
					else
					{
						reset=0x00;
						send_EC_cmd(0xFC);	// return to dos
					}	
				}
				if(reset==0x01)
				{
					send_EC_cmd(0xFE);		// Watch dog reset EC
				}
				free(str1);
				free(str2);
   			}
			else
			{

			}
			}
			break;
			
	}
	
	send_KB_cmd(0xAE);

	return(0);
}




