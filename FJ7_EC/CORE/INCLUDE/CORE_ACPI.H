/*------------------------------------------------------------------------------
 * Title: CORE_ACPI.H
 *
 * Copyright (c) 1983-2006, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/

#ifndef	CORE_ACPI_H
#define	CORE_ACPI_H

void service_pci2(void);
void service_pci2f(void);
void service_pci3(void);
void service_pci4(void);

void ACPI_Cmd_0X(void);
void ACPI_Cmd_1X(void);
void ACPI_Cmd_2X(void);
void ACPI_Cmd_3X(void);
void ACPI_Cmd_4X(void);
void ACPI_Cmd_5X(void);
void ACPI_Cmd_6X(void);
void ACPI_Cmd_7X(void);
void ACPI_Cmd_8X(void);
void ACPI_Cmd_9X(void);
void ACPI_Cmd_AX(void);
void ACPI_Cmd_BX(void);
void ACPI_Cmd_CX(void);
void ACPI_Cmd_DX(void);
void ACPI_Cmd_EX(void);
void ACPI_Cmd_FX(void);

void ACPI_Cmd_0XData(void);
void ACPI_Cmd_1XData(void);
void ACPI_Cmd_2XData(void);
void ACPI_Cmd_3XData(void);
void ACPI_Cmd_4XData(void);
void ACPI_Cmd_5XData(void);
void ACPI_Cmd_6XData(void);
void ACPI_Cmd_7XData(void);
void ACPI_Cmd_8XData(void);
void ACPI_Cmd_9XData(void);
void ACPI_Cmd_AXData(void);
void ACPI_Cmd_BXData(void);
void ACPI_Cmd_CXData(void);
void ACPI_Cmd_DXData(void);
void ACPI_Cmd_EXData(void);
void ACPI_Cmd_FXData(void);

void Read_External_Ram(void);
void Write_External_Ram(void);

void SendFromAcpi(BYTE ecdata);
BYTE CheckBurstMode(void);
void SetTotalBurstTime(void);

void PM2_6CCmd_0X(void);
void PM2_6CCmd_1X(void);
void PM2_6CCmd_2X(void);
void PM2_6CCmd_3X(void);
void PM2_6CCmd_4X(void);
void PM2_6CCmd_5X(void);
void PM2_6CCmd_6X(void);
void PM2_6CCmd_7X(void);
void PM2_6CCmd_8X(void);
void PM2_6CCmd_9X(void);
void PM2_6CCmd_AX(void);
void PM2_6CCmd_BX(void);
void PM2_6CCmd_CX(void);
void PM2_6CCmd_DX(void);
void PM2_6CCmd_EX(void);
void PM2_6CCmd_FX(void);

void PM2_6CCmd_0XData(void);
void PM2_6CCmd_1XData(void);
void PM2_6CCmd_2XData(void);
void PM2_6CCmd_3XData(void);
void PM2_6CCmd_4XData(void);
void PM2_6CCmd_5XData(void);
void PM2_6CCmd_6XData(void);
void PM2_6CCmd_7XData(void);
void PM2_6CCmd_8XData(void);
void PM2_6CCmd_9XData(void);
void PM2_6CCmd_AXData(void);
void PM2_6CCmd_BXData(void);
void PM2_6CCmd_CXData(void);
void PM2_6CCmd_DXData(void);
void PM2_6CCmd_EXData(void);
void PM2_6CCmd_FXData(void);

void PMC3_Cmd_0X(void);
void PMC3_Cmd_1X(void);
void PMC3_Cmd_2X(void);
void PMC3_Cmd_3X(void);
void PMC3_Cmd_4X(void);
void PMC3_Cmd_5X(void);
void PMC3_Cmd_6X(void);
void PMC3_Cmd_7X(void);
void PMC3_Cmd_8X(void);
void PMC3_Cmd_9X(void);
void PMC3_Cmd_AX(void);
void PMC3_Cmd_BX(void);
void PMC3_Cmd_CX(void);
void PMC3_Cmd_DX(void);
void PMC3_Cmd_EX(void);
void PMC3_Cmd_FX(void);

void PMC3_Cmd_0XData(void);
void PMC3_Cmd_1XData(void);
void PMC3_Cmd_2XData(void);
void PMC3_Cmd_3XData(void);
void PMC3_Cmd_4XData(void);
void PMC3_Cmd_5XData(void);
void PMC3_Cmd_6XData(void);
void PMC3_Cmd_7XData(void);
void PMC3_Cmd_8XData(void);
void PMC3_Cmd_9XData(void);
void PMC3_Cmd_AXData(void);
void PMC3_Cmd_BXData(void);
void PMC3_Cmd_CXData(void);
void PMC3_Cmd_DXData(void);
void PMC3_Cmd_EXData(void);
void PMC3_Cmd_FXData(void);

#endif	/* CORE_ACPI_H */

