/*------------------------------------------------------------------------------
 * Title : OEM_THERM.H
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#ifndef	OEM_THERM_H
#define	OEM_THERM_H


//------------------------------------------------------------------------------
// Thermal Sensor Device Address
//------------------------------------------------------------------------------
// GMT Thermal Sensor ++
#define	G780_Addr			0x98
#define	G780_1_Addr			0x9A
#define	G781_Addr			0x98
#define	G781_1_Addr			0x9A
#define	G784_Addr			0x98
#define	G784_1_Addr			0x9A
#define	G791_Addr			0x7A
#define	G799_Addr			0x98
// GMT Thermal Sensor --

// AMD Thermal Sensor ++
#define	AMD1032_Addr		0x98
// AMD Thermal Sensor --

// NS Thermal Sensor ++
#define	LM89_Addr			0x98
#define	LM89_1_Addr			0x9A
#define	LM95245_A0H_Addr	0x98
#define	LM95245_A0M_Addr	0x50
#define	LM95245_A0L_Addr	0x30
#define	LM95245_1_A0H_Addr	0x9A
#define	LM95245_1_A0M_Addr	0x52
#define	LM95245_1_A0L_Addr	0x32
// NS Thermal Sensor --

// MAXIM Thermal Sensor ++
#define	MAX6648_Addr		0x98
#define	MAX6657_Addr		0x98
#define	MAX6658_Addr		0x98
#define	MAX6692_Addr		0x98
#define	MAX6659_Pin5G_Addr	0x98
#define	MAX6659_Pin5F_Addr	0x9A
#define	MAX6659_Pin5V_Addr	0x9C
// MAXIM Thermal Sensor --

//NVdia GPU thermal addr --
#define	NV_N13P_Addr		0x9E
//NVdia GPU thermal addr --

//David add for read GPU temperature 2014/05/21
//AMD GPU thermal addr --
#define	AMD_GPU_Addr		0x82
#define	SMB_CMD_LD_ADDR		0x01
#define	SMB_CMD_WR_DATA		0x02
#define	SMB_CMD_RD_DATA		0x03
//AMD GPU thermal addr --
//David add for read GPU temperature 2014/05/21

//#ifdef	OEM_Calpella
#define	PECI_4BAddr			0x96
#define	PECI_2BAddr			0x56
#define	PECI_ReadCmd		0x40
//#endif	//OEM_Calpella

// Define Thermal Sensor ++
#define	DumTSAr				0xFF

// David modify for separate read temperature from Calpella/CPU or VGA thermal sensor 2011/09/15
#ifdef	OEM_Calpella
#define	CPUTSAr				PECI_4BAddr
#else	//OEM_Calpella
#define	CPUTSAr				AMD1032_Addr
#endif	//OEM_Calpella

#ifdef	OEM_VGA_Support
	// David_20140408 modify for NVdia and ATI different function
	#ifdef	NVdia_Support
#define	VGATSAr				NV_N13P_Addr
	#endif	//NVdia_Support
	#ifdef	ATI_Support
#define	VGATSAr				DumTSAr
	#endif	//ATI_Support
	// David_20140408 modify for NVdia and ATI different function
#else	//OEM_VGA_Support
#define	VGATSAr				DumTSAr
#endif	//OEM_VGA_Support
// David modify for separate read temperature from Calpella/CPU or VGA thermal sensor 2011/09/15
// Define Thermal Sensor --


//------------------------------------------------------------------------------
// Common Definition
//------------------------------------------------------------------------------
// Thermal Sensor Standard Command ++
#define	Read_Local			0x00
#define	Read_Remote			0x01
#define	Sensor_CON_R_r		0x04
#define	Sensor_CON_R_w		0x0A
#define	Con_Rate_1Hz		4
#define	Remote_ALT_r		0x07
#define	Remote_ALT_w		0x0D
#define	Local_ALT_r			0x05
#define	Local_ALT_w			0x0B
#define	RemoteLocalALT		127
#define	Remote_OVER_rw		0x19
#define	Local_OVER_rw		0x20
// Thermal Sensor Standard Command --

// Thermal Sensor Clear ALERT status ++
#define	ClearALERTAdd		0x18
#define	ClearALERTCmd		0x00
// Thermal Sensor Clear ALERT status --


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void InitionThermalSensor(void);
extern void ClearALERTTSS(void);
extern void ReadThermalSensor(void);

//#ifdef	OEM_Calpella
extern void PCH_Thernmal_Sensor_Error(void);
extern void PECI_Thernmal_Sensor_Error(void);
//#endif	//OEM_Calpella

extern void CPU_Thernmal_Sensor_Error(void);
extern void VGA_Thernmal_Sensor_Error(void);

extern BYTE InitThermalSensor(int ThermalSensorAddr, int Sensor_CON_R_w_data, int Remote_ALT_w_data, \
                              int Local_ALT_w_data, int Remote_OVER_rw_data, int Local_OVER_rw_data);

//David add for separate read temperature from Calpella/CPU or VGA thermal sensor 2011/09/15
#ifdef	OEM_Calpella
extern void ReadCalpellaTemperature(void);
#else	//OEM_Calpella
#if PCHTemperature
extern void ReadPCHTemperature(void);
#endif
extern void ReadCPUThermalTemperature(void);
#endif	//OEM_Calpella

#ifdef	OEM_VGA_Support
extern void ReadVGATemperature(void);
#endif	//OEM_VGA_Support
//David add for separate read temperature from Calpella/CPU or VGA thermal sensor 2011/09/15

extern void ReadCPUTempAMD(void);				// leox_20140120 Support_AMD_SB_TSI

#endif	/* OEM_THERM_H */
