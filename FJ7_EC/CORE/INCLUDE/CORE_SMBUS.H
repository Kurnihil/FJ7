/*------------------------------------------------------------------------------
 * Title : CORE_SMBUS.H
 * Author:
 *----------------------------------------------------------------------------*/

#ifndef	CORE_SMBUS_H
#define	CORE_SMBUS_H

#define	SMbusQ			0x00	// Quick command
#define	SMbusSRB		0x04	// Send Byte command and Receive Byte command
#define	SMbusRWB		0x08	// Read byte command Write byte command
#define	SMbusRWW		0x0C	// Read word command Write word command
#define	SMbusRWBB		0x14	// Read block command Write block command

#define	SMbusCh1		0x00	// SMbus channel A index
#define	SMbusCh2		0x01	// SMbus channel B index
#define	SMbusCh3		0x02	// SMbus channel C index
#define	SMbusCh4		0x03	// SMbus channel D index

#define	Cmd_Quick		0x00
#define	Cmd_SendByte	0x01
#define	Cmd_RecvByte	0x02
#define	Cmd_ReadByte	0x03
#define	Cmd_WriteByte	0x04
#define	Cmd_ReadWord	0x05
#define	Cmd_WriteWord	0x06
#define	Cmd_ReadBlock	0x07
#define	Cmd_WriteBlock	0x08

#define	SMbusWQ			0x80	// Write Quick command
#define	SMbusRQ			0x00	// Read Quick command
#define	SMbusSBC		0x84	// Send Byte command
#define	SMbusRBC		0x04	// Receive Byte command
#define	SMbusWB			0x88	// Write byte command
#define	SMbusRB			0x08	// Read byte command
#define	SMbusWW			0x8C	// Write word command
#define	SMbusRW			0x0C	// Read  word command
#define	SMbusWBK		0x94	// Write block command
#define	SMbusRBK		0x14	// Read block command
#define SMbusWEXT		0x9C	// Write extend command	// leox_20120712 [PATCH] Support_SMBus_I2C
#define SMbusREXT		0x1C	// Read extend command	// leox_20120712 [PATCH] Support_SMBus_I2C

#define	SMBus_Busy		0x01	// SMBus clock pin or data pin is low
#define	SMBus_Free		0x00	// SMBus clock pin and data pin are both high

#define	SMBusPin_GPIO	0x00	// SMBus interface is GPIO function
#define	SMBusPin_ALT	0x01	// SMBus interface is ALT function

#define	SMBus_CanUse	0x01	// SMBus interface can be used.
#define	SMBus_CanNotUse	0x00	// SMBus interface can't be used.

#define	SMBus_AccessType_Fucn	0x01
#define	SMBus_AccessType_Table	0x00

#define	SMBus_BusyRecover	30	// Reset SMBus interface if bus clock pin or data pin is low greater than SMBus_BusyRecover times.

#define	SMBus_NeedService	0x01
#define	SMBus_ClearService	0x00

#define	SMBus_NeedPEC	0x01
#define	SMBus_NoPEC		0x00

extern void ResetSMBus(BYTE Channel);
extern void InitSMBus(void);
extern BYTE bRWSMBus(BYTE Channel, BYTE Protocol, BYTE Addr, BYTE Comd, XBYTE *Var, BYTE PECSupport);
extern BYTE bRSMBusBlock(BYTE Channel, BYTE Protocol, BYTE Addr, BYTE Comd, XBYTE *Var);
extern BYTE bWSMBusBlock(BYTE Channel, BYTE Protocol, BYTE Addr, BYTE Comd, XBYTE *Var, BYTE ByteCont, BYTE PECsupport);	// leox_20120711 [PATCH] Add for SMBus block write
extern void CalcCRC8(BYTE sourcebyte);
extern void InitSMBusChannel3(void);
extern BYTE CheckSMBusFree(BYTE channel);
extern BYTE CheckSMBusInterfaceIsALT(BYTE channel);
extern BYTE CheckSMBusInterfaceCanbeUse(BYTE channel, BYTE accesstype);
//David_20120327 add for SMBUS reset timing follow SMBUS spec
extern void CNTResetSMBus(void);
//David_20120327 add for SMBUS reset timing follow SMBUS spec
extern BYTE bRSMBusExt(BYTE Channel, BYTE Protocol, BYTE Addr, XBYTE *Var, BYTE BCnt);	// leox_20120712 [PATCH]	// Support_SMBus_I2C
extern BYTE bWSMBusExt(BYTE Channel, BYTE Protocol, BYTE Addr, XBYTE *Var, BYTE BCnt);	// leox_20120712 [PATCH]	// Support_SMBus_I2C
extern BYTE bSMBusSendRecvByte(BYTE Channel, BYTE Protocol, BYTE Addr, XBYTE *Var, BYTE PECSupport);	// leox_20140115 [PATCH] For SMBus send/receive byte command

typedef struct SMBusStruct
{
	XBYTE	*SMBusCTL;
	XBYTE	*SMBusADR;
	XBYTE	*SMBusCMD;
	XBYTE	*SMBusSTA;
	XBYTE	*SMBusData0;
	XBYTE	*SMBusData1;
	XBYTE	*SMBusBData;
	XBYTE	*SMBusIER;
	XBYTE	*SMBusISR;
	BYTE	IERFlag;
	XBYTE	*SMBusPEC;
} sSMBus;

typedef struct ResetSMBusS
{
	XBYTE	*SMBusSTA;
	XBYTE	*SMBusCTL2;
	XBYTE	*SMBusPinCTL;
	XBYTE	*SMBusPin0;
	XBYTE	*SMBusPin1;
	XBYTE	*GPIOReg;
	BYTE	GPIOData;
	XBYTE	*recoverCunt;
} sResetSMBusS;

//David_20120327 add for SMBUS reset timing follow SMBUS spec
typedef struct AResetSMBusS
{
	XBYTE	*SMBusSTA;
	XBYTE	*SMBusCTL2;
	XBYTE	*SMBusPinCTL;
	XBYTE	*SMBusPin0;
	XBYTE	*SMBusPin1;
	XBYTE	*GPIOReg;
	BYTE	GPIOData;
	BYTE	CLKGPIO;
	BYTE	DataGPIO;
	XBYTE	*recoverCunt;
	XBYTE	*SMBCunt;
} sAResetSMBusS;
//David_20120327 add for SMBUS reset timing follow SMBUS spec

typedef struct SCS
{
	BYTE	protocol;
	BYTE	address;
	BYTE	cmd;
	XBYTE	*smbdata;
	BYTE	Conditionflag;
	XBYTE	*ConditionVar;
	FUNCT_PTR_V_V	hookfunc;
	FUNCT_PTR_V_V	errorfunc;
	BYTE	Bcunt;
	//BYTE	data1;
	//BYTE	data2;
} sSCS;

typedef struct SSC
{
	XBYTE	*index;
	XBYTE	*timeout;
	XBYTE	*bytecount;
	XBYTE	*inusing;
	XBYTE	*tablesize;
	XBYTE	*serviceflag;
	FUNCT_PTR_V_V	servicefunc;
} sSSC;

extern sSMBus code asSMBus[];

#define	ACK_Done			0x33
#define	ACK_Error			0x44
#define	ACK_Start			0xFF

#endif	/* CORE_SMBUS_H */
