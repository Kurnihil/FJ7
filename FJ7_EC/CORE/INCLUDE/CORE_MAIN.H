/*------------------------------------------------------------------------------
 * Title: CORE_MAIN.H
 *
 * Copyright (c) 1983-2007, Insyde Software Corporation. All Rights Reserved.
 *
 * You may not reproduce, distribute, publish, display, perform, modify, adapt,
 * transmit, broadcast, present, recite, release, license or otherwise exploit
 * any part of this publication in any form, by any means, without the prior
 * written permission of Insyde Software Corporation.
 *----------------------------------------------------------------------------*/

#ifndef CORE_MAIN_H
#define CORE_MAIN_H

static void main(void);
extern void main_service(void);

static void InitEnableInterrupt(void);
static void EnableInterrupt(void);

static void service_unlock(void);
static void service_Low_LVEvent(void);
static void service_OEM_1(void);
static void service_OEM_2(void);
static void service_OEM_3(void);
static void service_OEM_4(void);
static void service_1mS(void);

static void Timer1msEvent(void);
static void Timer5msEventA(void);
static void Timer5msEventB(void);
static void Timer5msEventC(void);
static void Timer5msEventD(void);
static void Timer5msEventE(void);

static void MirrorDoneAndSPITriState(void);

#endif  /* CORE_MAIN_H */
