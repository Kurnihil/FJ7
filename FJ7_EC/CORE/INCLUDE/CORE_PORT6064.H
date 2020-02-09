/*------------------------------------------------------------------------------
 * Title: CORE_PORT6064.H - Prototypes for CORE_PORT6064.C.
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/

#ifndef	CORE_PORT6064_H
#define	CORE_PORT6064_H

void service_pci1(void);

void Cmd_0X(void);
void Cmd_1X(void);
void Cmd_2X(void);
void Cmd_3X(void);
void Cmd_4X(void);
void Cmd_5X(void);
void Cmd_6X(void);
void Cmd_7X(void);
void Cmd_8X(void);
void Cmd_9X(void);
void Cmd_AX(void);
void Cmd_A8(void);
void Cmd_AE(void);
void Cmd_BX(void);
void Cmd_CX(void);
void Cmd_DX(void);
void Cmd_EX(void);
void Cmd_FX(void);

void Cmd_0XData(void);
void Cmd_1XData(void);
void Cmd_2XData(void);
void Cmd_3XData(void);
void Cmd_4XData(void);
void Cmd_5XData(void);
void Cmd_6XData(void);
void Cmd_7XData(void);
void Cmd_8XData(void);
void Cmd_9XData(void);
void Cmd_AXData(void);
void Cmd_BXData(void);
void Cmd_CXData(void);
void Cmd_DXData(void);
void Cmd_EXData(void);
void Cmd_FXData(void);

void Cmd_A7(void);

void Cmd_A8(void);
void Cmd_D2(void);
void Cmd_D3(void);

void ResponseKBData(BYTE RData);
void SetHandleCmdData(BYTE step);

void KeyboardCmd(BYTE data_byte);
BBYTE Set_Overlay_Mask(BBYTE data_byte, FLAG bit_state);
//BYTE Read_Input_Port_1(void);
BYTE Read_Output_Port_2(void);

#endif	/* CORE_PORT6064_H */

