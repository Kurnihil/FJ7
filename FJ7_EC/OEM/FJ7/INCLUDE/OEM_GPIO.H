/*------------------------------------------------------------------------------
 * Title : OEM_GPIO.H
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#ifndef OEM_GPIO_H
#define OEM_GPIO_H


//------------------------------------------------------------------------------
// GPIO Port Setting
//------------------------------------------------------------------------------
#define ALT			0x00
#define INPUT		0x80
#define OUTPUT		0x40
#define PULL_UP		0x04
#define PULL_DW		0x02
#define MaxD		0x38

#define mA4_6		0x08
#define mA6_9		0x10
#define mA8_12		0x18
#define mA10_15		0x30
#define mA12_18		0x38

#define HighActive	1
#define LowActive	0


//------------------------------------------------------------------------------
// General Control Registers
//------------------------------------------------------------------------------
#define GCR_Init	0x06	// General Control Register (GCR)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BIT7     GPB5 Follow LPCRST# Enable (GFLE)
// BIT6     WUI7 Enabled (WUI7EN)
// BIT5     WUI6 Enabled (WUI6EN)
// BIT[4-3] Reserved
// BIT[2-1] LPC Reset Enable (LPCRSTEN)
//          00: Reserved
//          01: LPCRST# in GPIOB7
//          10: LPCRST# in GPIOD2
//          11: LPCRST# is dsiabled
// BIT0     GFLE Set (GFLES0)
//          1: GPDRB bit 5 will be set if WUI4 is level-low
//          0: GPDRB bit 5 will be set immediately if there is a high-to-low transition on WUI4

#define GCR2_Init	0x20	// General Control 2 Register (GCR2)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BIT7     TACH2 Enable (TACH2E)
// BIT6     CK32K Out Enable (CK32OE)
// BIT5     SMBus Channel 3 Enable (SMB3E)
// BIT4     PECI Enable (PECIE)
// BIT3     TMB1 Enabled (TMB1EN)
// BIT2     TMB0 Enabled (TMB0EN)
// BIT1     TMA1 Enabled (TMA1EN)
// BIT0     TMA0 Enabled (TMA0EN)

#define GCR4_Init	0x00	// General Control 4 Register (GCR4)	// Leo_20160328 GPI7 bypass to GPC7 SATA_LED
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BIT[7-3]	Reserved
// BIT[2-0] R/W 000b Hardware Bypass Enable (HWBPE)
// 			000b: Disable
// 			001b: GPI6 input will be directly bypassed to BAO.
// 			010b: GPI7 input will be directly bypassed to BBO.
// 			011b: GPI6/GPI7 input will be directly bypassed to BAO/BBO.
// 			101b: GPI6 input will be directly bypassed to BAO and BBO.
// 			Others: Reserved.

//------------------------------------------------------------------------------
// GPIO Port A Registers Define
//------------------------------------------------------------------------------
#define PortA_VOLT	0x00			// GPA[---] (Not supported)					// leox20160105
#define PortA_GPOT	0x0E			// GPA[3-0] (0:Push-pull, 1:Open-drain)		// leox20151231
#define PortA_Init	0x0E			// Data of port A	// aka name		//              (FH8A)                  (FH9C)
#define PortA0_Ctrl	INPUT			// RTC_DET#
#define PortA1_Ctrl	OUTPUT			// BATLED1#			// BAT_LED1#	// Battery LED1 (Blue)  High/Low Off/On (White)
#define PortA2_Ctrl	OUTPUT			// PWRLED0#			// SUSLED#		// Power LED0   (Amber) High/Low Off/On (White)
#define PortA3_Ctrl	OUTPUT			// BATLED0#			// BAT_LED0#	// Battery LED0 (Amber) High/Low Off/On (Orange)
#define PortA4_Ctrl	OUTPUT			// MAINON 			// RUN_ON
#define PortA5_Ctrl	INPUT			// SLP_S0#			// leo_20160328 Reserved
#define PortA6_Ctrl	ALT				// PWM_FAN1
#define PortA7_Ctrl	OUTPUT			// CLR_CMOS			// EC_RTC_RST


//------------------------------------------------------------------------------
// GPIO Port B Registers Define
//------------------------------------------------------------------------------
#define PortB_VOLT	0x00			// GPB[6-3] (0:3.3V, 1:1.8V)				// leox20160105
#define PortB_GPOT	0x44			// GPB[7-0] (0:Push-pull, 1:Open-drain)		// leox20151231
#define PortB_Init	0x5C			// Data of port B	// aka name
#define PortB0_Ctrl	OUTPUT			// AMP_MUTE#		//				// Speaker sound, High/Low Enable/Disable
#define PortB1_Ctrl	INPUT+PULL_DW	// ME_WR#
#define PortB2_Ctrl	OUTPUT			// DNBSWON#
#define PortB3_Ctrl	ALT+PULL_UP		// MBCLK0
#define PortB4_Ctrl	ALT+PULL_UP		// MBDATA0
#define PortB5_Ctrl	INPUT			// SSD_DETECT#
#define PortB6_Ctrl	OUTPUT			// RCIN#			// EC_RCIN#
#define PortB7_Ctrl	OUTPUT			// RSMRST#


//------------------------------------------------------------------------------
// GPIO Port C Registers Define
//------------------------------------------------------------------------------
#define PortC_VOLT	0x00			// GPC[7,2,1] (0:3.3V, 1:1.8V)				// leox20160105
#define PortC_GPOT	0x00			// GPC[---] (Not supported)					// leox20151231
#define PortC_Init	0x07			// Data of port C	// aka name
#define PortC0_Ctrl	OUTPUT			// EC_WAKE_ON		//				// AC in high, DC low	// leo_20150429 ????
#define PortC1_Ctrl	ALT+PULL_UP		// MBCLK1
#define PortC2_Ctrl	ALT+PULL_UP		// MBDATA1
#if	Extend_Matrix
#define PortC3_Ctrl	ALT+PULL_UP		// KSO16
#else
#define PortC3_Ctrl	INPUT			// SATA_LED#
#endif
#define PortC4_Ctrl	OUTPUT			// S3_ON			// SUS_ON
#if	Extend_Matrix
#define PortC5_Ctrl	ALT+PULL_UP		// KSO17
#else
#define PortC5_Ctrl	INPUT			// SATA_LED#_D
#endif
#define PortC6_Ctrl	OUTPUT			// CCD_Kill			// CCD Power source, High/Low On/Off
#define PortC7_Ctrl	OUTPUT			// CCD2_KILL


//------------------------------------------------------------------------------
// GPIO Port D Registers Define
//------------------------------------------------------------------------------
#define PortD_VOLT	0x00			// GPD[4-0] (0:3.3V, 1:1.8V)				// leox20160105
#define PortD_GPOT	0x18			// GPD[7-0] (0:Push-pull, 1:Open-drain)		// leox20151231
#define PortD_Init	0x18			// Data of port D	// aka name
#define PortD0_Ctrl	INPUT			// SUSC#			// SLP_S4#
#define PortD1_Ctrl	INPUT			// HWPG
#define PortD2_Ctrl	ALT				// PLTRST#
#define PortD3_Ctrl	OUTPUT			// SCI#				// SIO_EXT_SCI#
#define PortD4_Ctrl	OUTPUT			// KBSMI#			// SIO_EXT_SMI#
#define PortD5_Ctrl	OUTPUT			// ECPWROK			// EC_PWROK
#define PortD6_Ctrl	ALT				// FanSig			// FANSIG1
#define PortD7_Ctrl	OUTPUT			// USB_CHARGE		//				// leox20150605 Change to high active for FH9C


//------------------------------------------------------------------------------
// GPIO Port E Registers Define
//------------------------------------------------------------------------------
#define PortE_VOLT	0x00			// GPE[7,6,0] (0:3.3V, 1:1.8V)				// leox20160105
#define PortE_GPOT	0x80			// GPE[7-0] (0:Push-pull, 1:Open-drain)		// leox20151231
#define PortE_Init	0x81			// Data of port E	// aka name
#define PortE0_Ctrl	OUTPUT			// CHG_DISABLE#
#define PortE1_Ctrl	OUTPUT			// BT_EN			//				// Blue Tooth power source, High/Low On/Off
#define PortE2_Ctrl	OUTPUT			// SUS_ON_EC		// leo_20160425 add for cost down by EE Jackson wu
#define PortE3_Ctrl	OUTPUT			// AC_PRESENT_EC
#define PortE4_Ctrl	INPUT			// NBSWON#
#define PortE5_Ctrl	INPUT			// EC_PCIE_LED#
#define PortE6_Ctrl	OUTPUT			// H_PROCHOT_EC
#define PortE7_Ctrl	OUTPUT			// TP_CLOSE


//------------------------------------------------------------------------------
// GPIO Port F Registers Define
//------------------------------------------------------------------------------
#define PortF_VOLT	0x00			// GPF[7-2] (0:3.3V, 1:1.8V)				// leox20160105
#define PortF_GPOT	0x04			// GPF[7-0] (0:Push-pull, 1:Open-drain)		// leox20151231
#define PortF_Init	0x06			// Data of port F	// aka name
#define PortF0_Ctrl	INPUT			// KB_ID1
#define PortF1_Ctrl	OUTPUT			// DLID#			// EC_FPBACK#
#define PortF2_Ctrl	OUTPUT			// CAPSLED#			//				// CAPS LED, High/Low Off/On (White)
#define PortF3_Ctrl	INPUT			// PCH_BLON			// PCH_LVDS_BLON (aka APU_BLON_C)
#define PortF4_Ctrl	ALT				// TPCLK
#define PortF5_Ctrl	ALT				// TPDATA
#define PortF6_Ctrl	ALT+PULL_UP		// EC_PECI
#define PortF7_Ctrl	INPUT			// LID#


//------------------------------------------------------------------------------
// GPIO Port G Registers Define
//------------------------------------------------------------------------------
#define PortG_VOLT	0x00			// GPG[6,1] (0:3.3V, 1:1.8V)				// leox20160105
#define PortG_GPOT	0x00			// GPG[---] (Not supported)					// leox20151231
#define PortG_Init	0x00			// Data of port G	// aka name
#define PortG0_Ctrl	INPUT			// HYBRID_STAT#		//				// Turbo Boost mode, Low/High Enable/Disable
#define PortG1_Ctrl	OUTPUT			// N/A				// MAINON_2		// leox20150605 Refer to "FH9C_MB_B_20150605A.pdf"
#define PortG2_Ctrl	OUTPUT			// S5_ON			//				// PCH power source, High/Low On/Off
#define PortG3_Ctrl	ALT				// EC_CE#
#define PortG4_Ctrl	ALT				// EC_SI
#define PortG5_Ctrl	ALT				// EC_SO
#ifdef  DeepS3_SUPPORT
#define PortG6_Ctrl	INPUT			// SLP_SUS#
#else
#define PortG6_Ctrl	INPUT			// SUSB#			// SLP_S3#
#endif
#define PortG7_Ctrl	ALT				// EC_SCK


//------------------------------------------------------------------------------
// GPIO Port H Registers Define
//------------------------------------------------------------------------------
#define PortH_VOLT	0x00			// GPH[2-0] (0:3.3V, 1:1.8V)				// leox20160105
#define PortH_GPOT	0x20			// GPH[6-0] (0:Push-pull, 1:Open-drain)		// leox20151231
#define PortH_Init	0xA6			// Data of port H	// aka name
#define PortH0_Ctrl	INPUT			// CLKRUN#
#define PortH1_Ctrl	ALT+PULL_UP		// ISH_EC_SCL
#define PortH2_Ctrl	ALT+PULL_UP		// ISH_EC_SDA
#define PortH3_Ctrl	OUTPUT			// CB0
#define PortH4_Ctrl	OUTPUT			// CB1
#define PortH5_Ctrl	OUTPUT			// SWI#				// SIO_EXT_SWI#	// leox20150821 SWI for PVT
#define PortH6_Ctrl	INPUT			// S5_ON_2			// Leo_20160328 Reserved
#define PortH7_Ctrl	OUTPUT			// TS_CLOSE


//------------------------------------------------------------------------------
// GPIO Port I Registers Define
//------------------------------------------------------------------------------
#define PortI_VOLT	0x00			// GPI[---] (Not supported)					// leox20160105
#define PortI_GPOT	0x00			// GPI[---] (Not supported)					// leox20151231
#define PortI_Init	0x00			// Data of port I	// aka name
#define PortI0_Ctrl	OUTPUT			// WLAN_EN			//				// Wireless LAN, High/Low Enable/Disable
#define PortI1_Ctrl	ALT 			// ADC1 ICM			// I_ADP (aka ISENS_IN)
#define PortI2_Ctrl	ALT				// I_DCHG
#define PortI3_Ctrl	OUTPUT			// VR_ON_EC			// VRON
#define PortI4_Ctrl	OUTPUT			// LAN_ON			// LAN_ON_EC
#define PortI5_Ctrl	ALT				// MBATV			//				// leo_20150429 remerber to open ADC5 detect DC voltage
#define PortI6_Ctrl	INPUT			// ACIN
#define PortI7_Ctrl	INPUT			// VR_READY


//------------------------------------------------------------------------------
// GPIO Port J Registers Define
//------------------------------------------------------------------------------
#define PortJ_VOLT	0x00			// GPJ[---] (Not supported)					// leox20160105
#define PortJ_GPOT	0x18			// GPJ[5-0] (0:Push-pull, 1:Open-drain)		// leox20151231
#define PortJ_Init	0x18			// Data of port J	// aka name
#define PortJ0_Ctrl	INPUT			// SHDN#			// CHECK
#define PortJ1_Ctrl	INPUT			// KB_ID0
#define PortJ2_Ctrl	INPUT			// BAT_PRS#
#define PortJ3_Ctrl	OUTPUT			// LED_WLAN#		//				// RF LED,  High/Low Off/On (WHite)
#define PortJ4_Ctrl	OUTPUT			// NUMLED#			//				// NUM LED, High/Low Off/On (WHite)
#define PortJ5_Ctrl	INPUT			// ISH_Notice EC
#define PortJ6_Ctrl	OUTPUT			// PCH_PWROK		//				// leo_20150429 ????
#define PortJ7_Ctrl	INPUT			// KB_ID2


//------------------------------------------------------------------------------
// GPIO Output
//------------------------------------------------------------------------------
#define BATLED0_on()			CLR_BIT(GPDRA,1)	// BATLED0#		// Amber on		// Orange in FH9C
#define BATLED0_off()			SET_BIT(GPDRA,1)	// BAT_LED0#	// Amber off

#define BATLED1_on()			CLR_BIT(GPDRA,3)	// BATLED1#		// Blue on		// White in FH9C
#define BATLED1_off()			SET_BIT(GPDRA,3)	// BAT_LED1#	// Blue off

#define PWRLED0_on()			CLR_BIT(GPDRA,2)	// PWRLED1#		// Blue on		// White in FH9C
#define PWRLED0_off()			SET_BIT(GPDRA,2)	// SUSLED#		// Blue on		// leox20150610

#define PWRLED1_on()			//CLR_BIT(GPDRA,0)	// PWRLED0#		// Amber on		// Orange in FH9C
#define PWRLED1_off()			//SET_BIT(GPDRA,0)	// PWRLED#		// Amber off	// leox20150610

#define PWRLED2_on()			//CLR_BIT(GPDRE,5)	//				// Button on
#define PWRLED2_off()			//SET_BIT(GPDRE,5)	//				// Button off

#define CAPSLED_on()			CLR_BIT(GPDRF,2)	// CAPSLED#
#define CAPSLED_off()			SET_BIT(GPDRF,2)

#define SCROLLLED_on()			//CLR_BIT(GPDRH,4)
#define SCROLLLED_off()			//SET_BIT(GPDRH,4)

#define NUMLED_on()				CLR_BIT(GPDRJ,4)	// NUMLED#
#define NUMLED_off()			SET_BIT(GPDRJ,4)

#define ABAT_ON_on()			//CLR_BIT(GPDRX,3)
#define ABAT_ON_off()			//SET_BIT(GPDRX,3)

#define DISCHG_ON_on()			//SET_BIT(GPDRH,7)	// BAT/AC#		// DC
#define DISCHG_ON_off()			//CLR_BIT(GPDRH,7)	// DC/-C		// AC

#define ACHG_on()				//CLR_BIT(GPDRH,0)
#define ACHG_off()				//SET_BIT(GPDRH,0)

#define BCHG_on()				//CLR_BIT(GPDRX,0)
#define BCHG_off()				//SET_BIT(GPDRX,0)

#define SWI_on()				CLR_BIT(GPDRH,5)	// SWI#
#define SWI_off()				SET_BIT(GPDRH,5)	// SIO_EXT_SWI#

#define ENBEEP_on()				//CLR_BIT(GPDRX,0)
#define ENBEEP_off()			//SET_BIT(GPDRX,0)

#define TVon_LED()				//CLR_BIT(GPDRX,0)
#define TVoff_LED()				//SET_BIT(GPDRX,0)

#define ODD_LED_on()			//CLR_BIT(GPDRX,0)
#define ODD_LED_off()			//SET_BIT(GPDRX,0)

#define USBP_on()				//CLR_BIT(GPDRX,0)
#define USBP_off()				//SET_BIT(GPDRX,0)

#define USBP_DISCHARGE_on()		//SET_BIT(GPDRX,0)
#define USBP_DISCHARGE_off()	//CLR_BIT(GPDRX,0)

#define USBP2_on()				//CLR_BIT(GPDRX,0)
#define USBP2_off()				//SET_BIT(GPDRX,0)

#define USBP2_DISCHARGE_on()	//SET_BIT(GPDRX,0)
#define USBP2_DISCHARGE_off()	//CLR_BIT(GPDRX,0)

#define ENPCMSPK_on()			//CLR_BIT(GPDRX,0)
#define ENPCMSPK_off()			//SET_BIT(GPDRX,0)

#define WLAN_LED_on()			CLR_BIT(GPDRJ,3)	// LED_WLAN#
#define WLAN_LED_off()			SET_BIT(GPDRJ,3)

#define GPLED_on()				//SET_BIT(GPDRX,4)
#define GPLED_off()				//CLR_BIT(GPDRX,4)

#define ECO_LED0_on()			//CLR_BIT(GPDRX,4)
#define ECO_LED0_off()			//SET_BIT(GPDRX,4)

#define CIR_POWER_on()			//CLR_BIT(GPDRX,4)
#define CIR_POWER_off()			//SET_BIT(GPDRX,4)

#define LED_ON_on()				//SET_BIT(GPDRX,4)
#define LED_ON_off()			//CLR_BIT(GPDRX,4)

#define ECO_LED1_on()			//CLR_BIT(GPDRX,4)
#define ECO_LED1_off()			//SET_BIT(GPDRX,4)

#define RF_LED_on()				//CLR_BIT(GPDRX,4)
#define RF_LED_off()			//SET_BIT(GPDRX,4)

#define WLSW_on()				SET_BIT(GPDRI,0)	// WLAN_EN
#define WLSW_off()				CLR_BIT(GPDRI,0)

#define WLAN_ON_on()			//SET_BIT(GPDRE,5)	// WLAN_ON
#define WLAN_ON_off()			//CLR_BIT(GPDRE,5)

#define MODE_LED_on()			//CLR_BIT(GPDRX,4)
#define MODE_LED_off()			//SET_BIT(GPDRX,4)

#define BTON_EC_on()			SET_BIT(GPDRE,1)	// BT_EN
#define BTON_EC_off()			CLR_BIT(GPDRE,1)

#define ThreeG_on()				//SET_BIT(GPDRX,3)
#define ThreeG_off()			//CLR_BIT(GPDRX,3)

#define CCD_on()				{SET_BIT(GPDRC,6);SET_BIT(GPDRC,7);}	// CCD_Kill
#define CCD_off()				{CLR_BIT(GPDRC,6);CLR_BIT(GPDRC,7);}

#define REFON_on()				//SET_BIT(GPDRX,2)
#define REFON_off()				//CLR_BIT(GPDRX,2)

#define TV_POWERON_on()			//CLR_BIT(GPDRX,4)
#define TV_POWERON_off()		//SET_BIT(GPDRX,4)

#define VOLMUTE_on()			CLR_BIT(GPDRB,0)	// AMP_MUTE#
#define VOLMUTE_off()			SET_BIT(GPDRB,0)

#define BATT_TYPE_Li4S()		//CLR_BIT(GPDRX,4)
#define BATT_TYPE_Li3S()		//SET_BIT(GPDRX,4)

#define KBSMI_on()				CLR_BIT(GPDRD,4)	// KBSMI#
#define KBSMI_off()				SET_BIT(GPDRD,4)	// SIO_EXT_SMI#

#define SCI_on()				CLR_BIT(GPDRD,3)	// SCI#
#define SCI_off()				SET_BIT(GPDRD,3)	// SIO_EXT_SCI#

#define USB_CHARGE_ILIM_HI()	SET_BIT(GPDRH,4)	// CB1
#define USB_CHARGE_ILIM_LO()	CLR_BIT(GPDRH,4)	// leo_20150917 USB charging

#define	USB_CHARGE_CTL1_HI()	SET_BIT(GPDRH,3)	// CB0
#define	USB_CHARGE_CTL1_LO()	CLR_BIT(GPDRH,3)	// leo_20150917 USB charging

#define USB4_CHARGE_on()		SET_BIT(GPDRD,7)	// USB_CHARGE
#define USB4_CHARGE_off()		CLR_BIT(GPDRD,7)	// leox20150605 Change to high active for FH9C

#define USB4S5_DISCHARGE_on()	//SET_BIT(GPDRX,5)
#define USB4S5_DISCHARGE_off()	//CLR_BIT(GPDRX,5)

#define ECOLED_on()				//CLR_BIT(GPDRX,4)
#define ECOLED_off()			//SET_BIT(GPDRX,4)

#define SLP_M_On_on()			//SET_BIT(GPDRX,4)
#define SLP_M_On_off()			//CLR_BIT(GPDRX,4)

#define LAN_WOL_EN_EC_on()		SET_BIT(GPDRI,4)	// LAN_ON
#define LAN_WOL_EN_EC_off()		CLR_BIT(GPDRI,4)	// LAN_ON_EC

#define DLID_High()				SET_BIT(GPDRF,1)	// DLID#
#define DLID_Low()				CLR_BIT(GPDRF,1)	// EC_FPBACK#

#define LCD_BLON_On()			//SET_BIT(GPDRE,7)	// leo_20150518 add panel blon need wait APU_BLON
#define LCD_BLON_Off()			//CLR_BIT(GPDRE,7)	// leo_20150518 add panel blon need wait APU_BLON

#define AC_PRESENT_on()			SET_BIT(GPDRE,3)	// AC_PRESENT_EC
#define AC_PRESENT_off()		CLR_BIT(GPDRE,3)

#define LAN_RESET_on()			//SET_BIT(GPDRX,4)
#define LAN_RESET_off()			//CLR_BIT(GPDRX,4)

#define STBON_on()				SET_BIT(GPDRG,2)	// S5_ON
#define STBON_off()				CLR_BIT(GPDRG,2)

#define RVCC_ON_on()			//SET_BIT(GPDRX,1)
#define RVCC_ON_off()			//CLR_BIT(GPDRX,1)

#define MAINON_on()				SET_BIT(GPDRA,4)	// MAINON
#define MAINON_off()			CLR_BIT(GPDRA,4)

#define MAINON_1_on()			SET_BIT(GPDRG,1)	// MAINON_2
#define MAINON_1_off()			CLR_BIT(GPDRG,1)	// leox20150605 Refer to "FH9C_MB_B_20150605A.pdf"

#define MAINON_2_on()			//SET_BIT(GPDRH,7)	// MAINON_3
#define MAINON_2_off()			//CLR_BIT(GPDRH,7)	// leox20150605 Refer to "FH9C_MB_B_20150605A.pdf"

#define GA20_on()				//SET_BIT(GPDRB,5)
#define GA20_off()				//CLR_BIT(GPDRB,5)

#define RCIN_on()				CLR_BIT(GPDRB,6)	// RCIN#
#define RCIN_off()				SET_BIT(GPDRB,6)	// EC_RCIN#

#define DNBSWON_on()			CLR_BIT(GPDRB,2)	// DNBSWON#
#define DNBSWON_off()			SET_BIT(GPDRB,2)

#define ECPWROK_on()			SET_BIT(GPDRJ,6)	// EC: ECPWROK -> PCH: PCH_PWROK		// Leo_20160329 swap with PCH_PWROK
#define ECPWROK_off()			CLR_BIT(GPDRJ,6)	// EC_PWROK		// Intel:_pPWROK_off (South OK)

#define SUSON_on()				SET_BIT(GPDRC,4)	// S3_ON
#define SUSON_off()				CLR_BIT(GPDRC,4)	// SUS_ON

#define SUSON_EC_on()			SET_BIT(GPDRE,2)	// SUS_EC_ON	// leo_20160425 add for cost down by EE Jackson wu
#define SUSON_EC_off()			CLR_BIT(GPDRE,2)	// SUS_EC_ON

#define VRON_on()				SET_BIT(GPDRI,3)	// VRON
#define VRON_off()				CLR_BIT(GPDRI,3)	// VR_ON

#define RSMRST_on()				CLR_BIT(GPDRB,7)	// RSMRST#		// leox20150721 Fix RSMRST#
#define RSMRST_off()			SET_BIT(GPDRB,7)	//				// leox20150721 Fix RSMRST#

#define SBPWROK_on()			//SET_BIT(GPDRX,4)	// ATI:SBPWROK_on  (South OK)
#define SBPWROK_off()			//CLR_BIT(GPDRX,4)	// ATI:SBPWROK_off (South OK)

#define NBPWROK_on()			//SET_BIT(GPDRX,4)	// ATI:_pPWROK_on  (North OK)
#define NBPWROK_off()			//CLR_BIT(GPDRX,4)	// ATI:_pPWROK_off (North OK)

#define PCHPWROK_on()			SET_BIT(GPDRD,5)	// EC: PCH_PWROK -> PCH: SYS_PWROK		// Leo_20160329 swap with EC_PWROK
#define PCHPWROK_off()			CLR_BIT(GPDRD,5)

#define PCIEVDD_off()			//CLR_BIT(GPDRX,4)	// ATI
#define PCIEVDD_on()			//SET_BIT(GPDRX,4)	// ATI

#define EC_CK_PWG_on()			//SET_BIT(GPDRX,4)
#define EC_CK_PWG_off()			//CLR_BIT(GPDRX,4)

#define LAN_WP_on()				//CLR_BIT(GPDRX,1)
#define LAN_WP_off()			//SET_BIT(GPDRX,1)

#define SATA_LED_BBO_En()		//GPCRC7=ALT	// leo_20160427 sata led control method
#define SATA_LED_BBO_Dis_On()	//{GPCRC7=OUTPUT;CLR_BIT(GPDRC,7);}
#define SATA_LED_BBO_Dis_Off()	//{GPCRC7=OUTPUT;SET_BIT(GPDRC,7);}

#define MEFW_OVERRIDE_En()		{GPCRB1=OUTPUT;SET_BIT(GPDRB,1);}	// ME_WR#
#define MEFW_OVERRIDE_Dis()		GPCRB1=INPUT+PULL_DW

#define DDR3_CORL_EC_High()		//SET_BIT(GPDRX,5)
#define DDR3_CORL_EC_Low()		//CLR_BIT(GPDRX,5)

#define h_prochot_enable()		SET_BIT(GPDRE,6)	// H_PROCHOT_EC
#define h_prochot_disable()		CLR_BIT(GPDRE,6)

#ifdef  PECI_Support
#define PCH_PROCHOT_ON()		//CLR_BIT(GPDRX,1)
#define PCH_PROCHOT_OFF()		//SET_BIT(GPDRX,1)
#endif

#define DGPU_VRON_on()			//SET_BIT(GPDRX,2)
#define DGPU_VRON_off()			//CLR_BIT(GPDRX,2)

#define DGPU_VRON2_on()			//SET_BIT(GPDRX,7)
#define DGPU_VRON2_off()		//CLR_BIT(GPDRX,7)

#define GPU_AC_BATT_on()		//SET_BIT(GPDRJ,1)
#define GPU_AC_BATT_off()		//CLR_BIT(GPDRJ,1)

#define GPU_ON_EC_on()			//SET_BIT(GPDRA,6)
#define GPU_ON_EC_off()			//CLR_BIT(GPDRA,6)

#define GPU_RST_EC_on()			//SET_BIT(GPDRE,7)
#define GPU_RST_EC_off()		//CLR_BIT(GPDRE,7)

#define GPU_PROCHOT_on()		//SET_BIT(GPDRJ,2)
#define GPU_PROCHOT_off()		//CLR_BIT(GPDRJ,2)

#define CLR_CMOS_on()			SET_BIT(GPDRA,7)	// CLR_CMOS		// CMOSprotect
#define CLR_CMOS_off()			CLR_BIT(GPDRA,7)	// EC_RTC_RST	// leox20150612 Enabled for FH9C

#define BAT_SENSE_on()			SET_BIT(GPDRE,0)	// PMU_SCONT	// leox20150604 FH9C
#define BAT_SENSE_off()			CLR_BIT(GPDRE,0)	// CHG_DISABLE#

#define AUD_15_ON_on()			//SET_BIT(GPDRG,0)
#define AUD_15_ON_off()			//CLR_BIT(GPDRG,0)

#ifdef  OEM_Calpella
#define AC_PRESENT_output()		GPCRE3=OUTPUT		// AC_PRESENT_EC
#define AC_PRESENT_input()		GPCRE3=INPUT
#endif


//------------------------------------------------------------------------------
// GPIO Input
//------------------------------------------------------------------------------
#define DISPON()				0//IS_BIT_SET(GPDRX,4)
#define AC_IN_read()			IS_BIT_SET(GPDRI,6)		// ACIN
#define BATT_IN_read()			IS_BIT_CLR(GPDRJ,2)		// BATT_IN	// BAT_PRS#
#define LIDSW_read()			IS_BIT_CLR(GPDRF,7)		// LID#
#define KBD_ID0_read()			IS_BIT_SET(GPDRJ,1)		// KB_ID0
#define KBD_ID1_read()			IS_BIT_SET(GPDRF,0)		// KB_ID1
#define KBD_ID_read()			IS_BIT_SET(GPDRJ,7)		// KB_ID2				// leox20150806 FH9C
#define CIR_SEL_read()			0//IS_BIT_CLR(GPDRX,4)
#define CIR_read()				0//IS_BIT_CLR(GPDRX,0)
#define STATE_GATEA20()			0//IS_BIT_SET(GPDRB,5)

#define PCUHOLD_read()			0//IS_BIT_SET(GPDRX,5)
#define PCUHOLD_High()			0//IS_BIT_SET(GPDRX,5)

#define CRT_SENSE_In()			0//IS_BIT_CLR(GPDRX,4)
#define BT_SENSE_In()			0//IS_BIT_CLR(GPDRX,4)

#define MID1_read()				0//IS_BIT_CLR(GPDRX,5)
#define MID2_read()				0//IS_BIT_CLR(GPDRX,6)

#define RF_EN_read()			0//IS_BIT_CLR(GPDRX,4)
#define LANRST_Low()			0//IS_BIT_CLR(GPDRX,4)
#define SLP_LAN_High()			0//IS_BIT_SET(GPDRX,3)
#define WOL_EN_High()			IS_BIT_SET(GPDRI,4)		// LAN_ON	// LAN_ON_EC

#define SLP_S3_High()			IS_BIT_SET(GPDRG,6)		// SUSB#	// SLP_S3#
#define SLP_S4_High()			IS_BIT_SET(GPDRD,0)		// SUSC#	// SLP_S4#
#define SLP_S5_High()			0//IS_BIT_SET(GPDRX,3)
#define SLP_M_High()			0//IS_BIT_SET(GPDRX,3)

#ifdef  DeepS3_SUPPORT
#define SLP_SUS_Low()			IS_BIT_CLR(GPDRG,6)
#else
#define SLP_SUS_Low()			0//IS_BIT_CLR(GPDRX,6)
#endif

#define S4_STATE_Low()			0//IS_BIT_CLR(GPDRX,4)
#define S4_STATE_High()			0//IS_BIT_SET(GPDRX,5)

#define GFXPG_High()			0//IS_BIT_SET(GPDRX,3)
#define PLTRST_DIS_High()		0//IS_BIT_SET(GPDRX,4)

#define GPU_PWRGD_High()		0//IS_BIT_SET(GPDRD,0)
#define GPU_ON_PCH_High()		0//IS_BIT_SET(GPDRC,7)
#define GPU_RST_PCH_High()		0//IS_BIT_SET(GPDRJ,0)

#define SUS_PWR_DN_ACK_High()	0//IS_BIT_SET(GPDRX,4)
#define HWPG_Status_High()		IS_BIT_SET(GPDRD,1)
#define NBSWON_Status_Low()		IS_BIT_CLR(GPDRE,4)		// NBSWON#

#define CEN_L_Low()				1//IS_BIT_CLR(GPDRH,6)	// CEN_L#				// leox20150605 Actived for FH9C	// CHECK

#define MMB_Detect_Low()		0//IS_BIT_CLR(GPDRE,2)		// MMB_DET#				// David modify for MMB detect 2014/05/21	// Leo_20160328 FH9U didn't support MMB service
#define READ_IT723X_INT()		0//IS_BIT_SET(GPDRJ,0)		// MMB_INT#				// David add for MMB service 2014/05/21		// Leo_20160328 FH9U didn't support MMB service

#define EC_BOARD_ID_read()		0//IS_BIT_SET(GPDRA,4)
#define EC_BOARD_ID2_read()		0//IS_BIT_SET(GPDRB,5)	// leo_20150429 end

#define APU_BLON_read()			IS_BIT_SET(GPDRF,3)		// PCH_BLON	// PCH_LVDS_BLON (aka APU_BLON_C)	// leo_20150518 add panel blon need wait APU_BLON
#define	S5_ON_Read()			IS_BIT_SET(GPDRG,2)
#define SHDN_Read()				IS_BIT_CLR(GPDRJ,0)		// low active
#define RTC_Read()         		IS_BIT_SET(GPDRA,0)		// Hi: No battery or doesn't exist Lo: Exist

//Add One Touch Buttons function 2011/08/19 +++
#define OneTouchButt0_read()	0//IS_BIT_SET(GPDRX,0)
#define OneTouchButt1_read()	0//IS_BIT_SET(GPDRX,1)
#define OneTouchButt2_read()	0//IS_BIT_SET(GPDRX,2)
#define OneTouchButt3_read()	0//IS_BIT_SET(GPDRX,3)
#define OneTouchButt4_read()	0//IS_BIT_SET(GPDRX,4)
#define OneTouchButt5_read()	0//IS_BIT_SET(GPDRX,5)
#define OneTouchButt6_read()	0//IS_BIT_SET(GPDRX,6)
#define OneTouchButt7_read()	0//IS_BIT_SET(GPDRX,7)
//Add One Touch Buttons function 2011/08/19 ---


//------------------------------------------------------------------------------
// Structure Declaration
//------------------------------------------------------------------------------
typedef struct InitGPIOReg
{
	XBYTE	*reg;
	BYTE	value;
} sInitGPIOReg;


//------------------------------------------------------------------------------
// External Function Declaration
//------------------------------------------------------------------------------
extern void Init_GPIO(void);


#endif	/* OEM_GPIO_H */
