#ifndef OEM_IT7230_SETTING_H
#define OEM_IT7230_SETTING_H


#ifdef IT_7230MMB_SUPPORT


typedef struct InitCapSReg
{
    BYTE   page;    
    BYTE   reg;     
    WORD   value;   
} sInitCapSReg;     


void CapS_PowerOn(void);
extern void resetIT7230(void);

#define BUS_TYPE 0x23
#define I2C_DEVICE_ADDR 0x8C 

#endif
#endif
