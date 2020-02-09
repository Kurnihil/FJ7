/*------------------------------------------------------------------------------
 * Title : OEM_THERM.C
 * Author: Ken Lee
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#ifdef	OEM_THERM
void InitionThermalSensor(void)
{
	if (IsFlag0(FanControl_Mode00, InitThermalSenFinish))
	{
		ClearALERTTSS();
		switch (FanControl_Mode00 & (CPU_TYPE_TJ85 | CPU_TYPE_TJ90 | CPU_TYPE_TJ100 | CPU_TYPE_TJ105))	// leox_20111201
		{
		#if	Support_CPU_TYPE	// leox_20111205
		case CPU_TYPE_TJ85 :
			if (InitThermalSensor(CPUTSAr, Con_Rate_1Hz, RemoteLocalALT, RemoteLocalALT, 105, 105))
			{
				if (InitThermalSensor(VGATSAr, Con_Rate_1Hz, 110, 110, 110, 110))
				{
					SetFlag(FanControl_Mode00, InitThermalSenFinish);
				}
			}
			break;

		case CPU_TYPE_TJ90 :
			if (InitThermalSensor(CPUTSAr, Con_Rate_1Hz, RemoteLocalALT, RemoteLocalALT, 110, 110))
			{
				if (InitThermalSensor(VGATSAr, Con_Rate_1Hz, 110, 110, 110, 110))
				{
					SetFlag(FanControl_Mode00, InitThermalSenFinish);
				}
			}
			break;

		case CPU_TYPE_TJ100 :
			if (InitThermalSensor(CPUTSAr, Con_Rate_1Hz, RemoteLocalALT, RemoteLocalALT, 120, 120))
			{
				if (InitThermalSensor(VGATSAr, Con_Rate_1Hz, 110, 110, 110, 110))
				{
					SetFlag(FanControl_Mode00, InitThermalSenFinish);
				}
			}
			break;

		case CPU_TYPE_TJ105 :
			if (InitThermalSensor(CPUTSAr, Con_Rate_1Hz, RemoteLocalALT, RemoteLocalALT, 125, 125))
			{
				if (InitThermalSensor(VGATSAr, Con_Rate_1Hz, 110, 110, 110, 110))
				{
					SetFlag(FanControl_Mode00, InitThermalSenFinish);
				}
			}
			break;
		#endif	// Support_CPU_TYPE

		default :
			if (InitThermalSensor(CPUTSAr, Con_Rate_1Hz, RemoteLocalALT, RemoteLocalALT, 105, 125))
			{
				if (InitThermalSensor(VGATSAr, Con_Rate_1Hz, RemoteLocalALT, RemoteLocalALT, 125, 125))
				{
					SetFlag(FanControl_Mode00, InitThermalSenFinish);
				}
			}
			break;
		}
		ClearALERTTSS();
	}
}


BYTE InitThermalSensor(int ThermalSensorAddr, int Sensor_CON_R_w_data, int Remote_ALT_w_data, \
						int Local_ALT_w_data, int Remote_OVER_rw_data, int Local_OVER_rw_data)
{
	BYTE resutl = FALSE;

	if (ThermalSensorAddr != DumTSAr)
	{
		// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
		SMB_ADDR_OEM = ThermalSensorAddr;
		SMB_CMD_OEM = Sensor_CON_R_w;
		// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
		SMB_DATA_OEM = Sensor_CON_R_w_data;
		if (vOSSMbusWBYTE_OEM())
		{
			// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
			SMB_CMD_OEM = Remote_ALT_w;
			// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
			SMB_DATA_OEM = Remote_ALT_w_data;
			if (vOSSMbusWBYTE_OEM())
			{
				// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
				SMB_CMD_OEM = Local_ALT_w;
				// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
				SMB_DATA_OEM = Local_ALT_w_data;
				if (vOSSMbusWBYTE_OEM())
				{
					// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
					SMB_CMD_OEM = Remote_OVER_rw;
					// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
					SMB_DATA_OEM = Remote_OVER_rw_data;
					if (vOSSMbusWBYTE_OEM())
					{
						// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
						SMB_CMD_OEM = Local_OVER_rw;
						// David_20120106 modify for vOSSMbusWBYTE/vOSSMbusWBYTE_OEM routine
						SMB_DATA_OEM = Local_OVER_rw_data;
						if (vOSSMbusWBYTE_OEM()) {resutl = TRUE;}
					}
				}
			}
		}
	}
	else
	{
		resutl = TRUE;
	}

	return resutl;
}


void ClearALERTTSS(void)
{
	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
	SMB_ADDR_OEM = ClearALERTAdd;
	SMB_CMD_OEM = ClearALERTCmd;
	// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM routine
	if (vOSSMbusRByte_OEM()) {}
}


// David modify for Thermal sensor/PECI/Calpella read temperature 2011/09/15
#ifdef	OEM_Calpella
void ReadCalpellaTemperature(void)
{
	if (IsFlag1(KETBOARD_STAT_CTRL, NotReadPECI)) {return;}
	if (IsFlag1(JUMPERSETTING, MEFW_OVERRIDE)) {return;}

	// David_20120106 modify for vOSSMbusRBlcok/vOSSMbusRBlcok_OEM_SMB3 routine
	SMB_ADDR_OEM = PECI_4BAddr;
	SMB_CMD_OEM = PECI_ReadCmd;
	// David_20120106 modify for vOSSMbusRBlcok/vOSSMbusRBlcok_OEM_SMB3 routine
	if (vOSSMbusRBlcok_OEM_SMB3())
	{
		PECI_RemoteErrorCnt = 0;
		ClrFlag(MISC_FLAG, PECI_Sensor_SMB_error);
		if ((*(&SMB_DATA_OEM + 9)) == PECI_ME_Sequence_Num_BK)
		{
			if (PECI_ME_HandUp_ErrorCnt < 30) {PECI_ME_HandUp_ErrorCnt++;}
		}
		else
		{
			PECI_ME_HandUp_ErrorCnt = 0;
			PECI_ME_Sequence_Num_BK = *(&SMB_DATA_OEM + 9);
		}
		VGA_Local_Temp = SMB_BCNT;	// PECIBCNT

		if ((*(&SMB_DATA_OEM + 1)) == 0xFF)
		{
			PCH_Thernmal_Sensor_Error();
		}
		else
		{
			PCH_RemoteErrorCnt = 0;
			CPU_Local_Temp = *(&SMB_DATA_OEM + 1);	// PECIPCH
		}

		if (IsFlag1((*(&SMB_DATA_OEM + 2)), BIT0))
		{
			CPU_Thernmal_Sensor_Error();
		}
		else
		{
			CPU_RemoteErrorCnt = 0;
			ClrFlag(SYS_FLAG, CPU_Sensor_SMB_error);
			CPU_Remote_Temp = *(&SMB_DATA_OEM + 3);	// PECICPU
		}

		if ((*(&SMB_DATA_OEM + 4)) == 0xFF)
		{
			VGA_Thernmal_Sensor_Error();
		}
		else
		{
			VGA_RemoteErrorCnt = 0;
			ClrFlag(SYS_FLAG, VGA_Sensor_SMB_error);
			VGA_Remote_Temp = *(&SMB_DATA_OEM + 4);	// PECIMCH
		}
	}
	else
	{
		PECI_Thernmal_Sensor_Error();
	}
}


#else	//OEM_Calpella
void ReadCPUThermalTemperature(void)
{
	if (CPUTSAr != DumTSAr)
	{
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
		SMB_ADDR_OEM = CPUTSAr;
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine

		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
		SMB_CMD_OEM = Read_Remote;
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
		if (vOSSMbusRByte_OEM_SMB3())
		{
			CPU_RemoteErrorCnt = 0;
			ClrFlag(SYS_FLAG, CPU_Sensor_SMB_error);
			CPU_Remote_Temp = SMB_DATA_OEM;
		}
		else
		{
			CPU_Thernmal_Sensor_Error();
		}

		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
		SMB_CMD_OEM = Read_Local;
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
		if (vOSSMbusRByte_OEM_SMB3())
		{
			CPU_RemoteErrorCnt = 0;
			ClrFlag(SYS_FLAG, CPU_Sensor_SMB_error);
			CPU_Local_Temp = SMB_DATA_OEM;
		}
		else
		{
			CPU_Thernmal_Sensor_Error();
		}
	}
}
#endif	//OEM_Calpella

void ReadPCHTemperature(void)
{
	if (IsFlag1(KETBOARD_STAT_CTRL, NotReadPECI))
	{
		return;
	}
	if (IsFlag1(JUMPERSETTING, MEFW_OVERRIDE))
	{
		return;
	}
	SMB_ADDR_OEM = PECI_4BAddr;
	SMB_CMD_OEM = PECI_ReadCmd;
	if (vOSSMbusRBlcok_OEM_SMB2())
	{
		if ((*(&SMB_DATA_OEM)) == 0xFF)
		{
		}
		else
		{
			PCH_RemoteErrorCnt = 0;
			CPU_Local_Temp = *(&SMB_DATA_OEM);	//PECIPCH
		}
	}
	else
	{
	}
}

#ifdef	OEM_VGA_Support
void ReadVGATemperature(void)
{
	// David_20140408 modify for NVdia and ATI different function
	#ifdef	NVdia_Support
	if ((IsFlag1(FromMBsignals00, GFXPG)) && (IsFlag1(FromMBsignals00, PLTRST_DIS)))
	{
		if (VGATSAr != DumTSAr)
		{
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
			SMB_ADDR_OEM = VGATSAr;
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
	
			#if	0
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
			SMB_CMD_OEM = Read_Remote;
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
			if (vOSSMbusRByte_OEM_SMB3())
			{
				VGA_RemoteErrorCnt = 0;
				ClrFlag(SYS_FLAG, VGA_Sensor_SMB_error);
				VGA_Remote_Temp = SMB_DATA_OEM;
			}
			else
			{
				VGA_Thernmal_Sensor_Error();
			}
			#endif
	
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
			SMB_CMD_OEM = Read_Local;
		// David_20120106 modify for vOSSMbusRByte/vOSSMbusRByte_OEM_SMB3 routine
			if (vOSSMbusRByte_OEM_SMB3())
			{
				VGA_RemoteErrorCnt = 0;
				ClrFlag(SYS_FLAG, VGA_Sensor_SMB_error);
				VGA_Local_Temp = SMB_DATA_OEM;
			}
			else
			{
				VGA_Thernmal_Sensor_Error();
			}
		}
	}
	#endif	//NVdia_Support
	#ifdef	ATI_Support
	if (GPU_PWRGD_High())
	{
	//David modify for read GPU temp 2014/05/28
	//David add for read GPU temperature 2014/05/21
		SMB_ADDR_OEM = AMD_GPU_Addr;
		SMB_CMD_OEM = SMB_CMD_LD_ADDR;
		(*(&SMB_DATA_OEM + 0)) = 0x0F;
		(*(&SMB_DATA_OEM + 1)) = 0x00;
		(*(&SMB_DATA_OEM + 2)) = 0x00;
		(*(&SMB_DATA_OEM + 3)) = 0x86;									//change to channel 2 in Ver b
		if(bWSMBusBlock(SMbusCh2,SMbusWBK,SMB_ADDR_OEM,SMB_CMD_OEM,&SMB_DATA_OEM,4,SMBus_NoPEC))
		{
			SMB_ADDR_OEM = AMD_GPU_Addr;
			SMB_CMD_OEM = SMB_CMD_WR_DATA;
			(*(&SMB_DATA_OEM + 0)) = 0xC0;
			(*(&SMB_DATA_OEM + 1)) = 0x30;
			(*(&SMB_DATA_OEM + 2)) = 0x00;
			(*(&SMB_DATA_OEM + 3)) = 0x14;									//change to channel 2 in Ver b
			if(bWSMBusBlock(SMbusCh2,SMbusWBK,SMB_ADDR_OEM,SMB_CMD_OEM,&SMB_DATA_OEM,4,SMBus_NoPEC))
			{
				SMB_ADDR_OEM = AMD_GPU_Addr;
				SMB_CMD_OEM = SMB_CMD_LD_ADDR;
				(*(&SMB_DATA_OEM + 0)) = 0x0F;
				(*(&SMB_DATA_OEM + 1)) = 0x00;
 				(*(&SMB_DATA_OEM + 2)) = 0x00;
				(*(&SMB_DATA_OEM + 3)) = 0x87;									//change to channel 2 in Ver b
				if(bWSMBusBlock(SMbusCh2,SMbusWBK,SMB_ADDR_OEM,SMB_CMD_OEM,&SMB_DATA_OEM,4,SMBus_NoPEC))
				{
					SMB_ADDR_OEM = AMD_GPU_Addr;
					SMB_CMD_OEM = SMB_CMD_RD_DATA;
					if(bRSMBusBlock(SMbusCh2,SMbusRBK,SMB_ADDR_OEM,SMB_CMD_OEM,&SMB_DATA_OEM))
					{
						int temperature;
						temperature = (((*(&SMB_DATA_OEM + 1) << 8) + (*(&SMB_DATA_OEM + 2))) / 2); 
						VGA_Remote_Temp = temperature;
						VGA_RemoteErrorCnt = 0;
					}
					else
					{
//David modify for A+ SMB error 2014/06/11
						//VGA_Thernmal_Sensor_Error();
					}
				}
				else
				{
					//VGA_Thernmal_Sensor_Error();
				}
			}
			else
			{
				//VGA_Thernmal_Sensor_Error();
			}
		}
		else
		{
			//VGA_Thernmal_Sensor_Error();
//David modify for A+ SMB error 2014/06/11
		}
	}
	else
	{
		VGA_Remote_Temp=0;			// No GPU or power saving clean VGA_tmp to 0
		VGA_RemoteErrorCnt = 0;		// No GPU or power saving clean ErrorCnt to 0
	}
	//David add for read GPU temperature 2014/05/21
	//David modify for read GPU temp 2014/05/28
	#endif
	// David_20140408 modify for NVdia and ATI different function
}
#endif	//OEM_VGA_Support


void ReadThermalSensor(void)
{
	if (IsFlag1(MISC_FLAG, EnTempDebug))
	{
		VGA_Remote_Temp = DBG_VGA_Remote_Temp;
		VGA_Local_Temp = DBG_VGA_Local_Temp;
		CPU_Remote_Temp = DBG_CPU_Remote_Temp;
		CPU_Local_Temp = DBG_CPU_Local_Temp;
	}
	else
	{
		#ifdef	PECI_Support	// leox_20111004
			#if	ReadCPUTemperature
		PECI_ReadCPUTemp();
				#if PCHTemperature
		ReadPCHTemperature();
				#endif
			#endif
		#else	// (original)
			#ifdef	OEM_Calpella
		ReadCalpellaTemperature();
			#else	//OEM_Calpella
				#if	Support_AMD_SB_TSI	// leox_20140120
		ReadCPUTempAMD();
				#else	// (original)
		ReadCPUThermalTemperature();
				#endif	//Support_AMD_SB_TSI
			#endif	//OEM_Calpella
		#endif	//PECI_Support

		#ifdef	OEM_VGA_Support
		if (IsFlag1(Device_STAT_CTRL1, VGA_Status))
		{
			ReadVGATemperature();
		}
		#endif	//OEM_VGA_Support
	}
}
// David modify for Thermal sensor/PECI/Calpella read temperature 2011/09/15


#if	Support_AMD_SB_TSI	// leox_20140120
void ReadCPUTempAMD(void)
{
	if (CPUTSAr == DumTSAr) {return;}

	SMB_ADDR_OEM = CPUTSAr;
	SMB_CMD_OEM = Read_Remote;
	if (bSMBusSendRecvByte(SMbusCh2, SMbusSBC, SMB_ADDR_OEM, &SMB_CMD_OEM, SMBus_NoPEC) &&
		bSMBusSendRecvByte(SMbusCh2, SMbusRBC, SMB_ADDR_OEM, &SMB_DATA_OEM, SMBus_NoPEC))
	{
		CPU_RemoteErrorCnt = 0;
		ClrFlag(SYS_FLAG, CPU_Sensor_SMB_error);
		CPU_Remote_Temp = SMB_DATA_OEM;
	}
	else
	{
		CPU_Thernmal_Sensor_Error();
	}
}
#endif	//Support_AMD_SB_TSI


//#ifdef	OEM_Calpella
void PCH_Thernmal_Sensor_Error(void)
{
	PCH_RemoteErrorCnt++;
	if (PCH_RemoteErrorCnt >= 60)
	{
		PCH_RemoteErrorCnt = 0;
		SetFlag(MISC_FLAG, PCH_Sensor_SMB_error);
		ShutDnCause = 0x1F;
		ForceOffPower();
	}
}


void PECI_Thernmal_Sensor_Error(void)
{
	PECI_RemoteErrorCnt++;
	if (PECI_RemoteErrorCnt >= 30)
	{
		PECI_RemoteErrorCnt = 0;
		SetFlag(MISC_FLAG, PECI_Sensor_SMB_error);
		ShutDnCause = 0x1E;
		ForceOffPower();
	}
}
//#endif	//OEM_Calpella


void CPU_Thernmal_Sensor_Error(void)
{
	CPU_RemoteErrorCnt++;
	if (CPU_RemoteErrorCnt >= 60)
	{
		CPU_RemoteErrorCnt = 0;
		SetFlag(SYS_FLAG, CPU_Sensor_SMB_error);
		ShutDnCause = 0x0F;
		ForceOffPower();
	}
}


void VGA_Thernmal_Sensor_Error(void)
{
	VGA_RemoteErrorCnt++;
	if (VGA_RemoteErrorCnt >= 60)
	{
		VGA_RemoteErrorCnt = 0;
		SetFlag(SYS_FLAG, VGA_Sensor_SMB_error);
		ShutDnCause = 0x0E;
		ForceOffPower();
	}
}
#endif	//OEM_THERM

