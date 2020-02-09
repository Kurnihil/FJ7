/*------------------------------------------------------------------------------
 * Title : OEM_GPIO.C
 * Author: Dino
 * Note  : These functions are for reference only. Please follow your
 *         project software specification to do some modification.
 *----------------------------------------------------------------------------*/


#include <CORE_INCLUDE.H>
#include <OEM_INCLUDE.H>


#if Support_IT8987	// leox20160105
//------------------------------------------------------------------------------
// The Function of Input Voltage Selection
//------------------------------------------------------------------------------
void Set_Input_Voltage(void)
{
	IS_BIT_SET(PortB_VOLT, 5) ? SET_BIT(GCR19, 7) : CLR_BIT(GCR19, 7);
	IS_BIT_SET(PortB_VOLT, 6) ? SET_BIT(GCR19, 6) : CLR_BIT(GCR19, 6);
	IS_BIT_SET(PortC_VOLT, 1) ? SET_BIT(GCR19, 5) : CLR_BIT(GCR19, 5);
	IS_BIT_SET(PortC_VOLT, 2) ? SET_BIT(GCR19, 4) : CLR_BIT(GCR19, 4);
	IS_BIT_SET(PortC_VOLT, 7) ? SET_BIT(GCR19, 3) : CLR_BIT(GCR19, 3);
	IS_BIT_SET(PortD_VOLT, 0) ? SET_BIT(GCR19, 2) : CLR_BIT(GCR19, 2);
	IS_BIT_SET(PortD_VOLT, 1) ? SET_BIT(GCR19, 1) : CLR_BIT(GCR19, 1);
	IS_BIT_SET(PortD_VOLT, 2) ? SET_BIT(GCR19, 0) : CLR_BIT(GCR19, 0);

	IS_BIT_SET(PortD_VOLT, 3) ? SET_BIT(GCR20, 7) : CLR_BIT(GCR20, 7);
	IS_BIT_SET(PortD_VOLT, 4) ? SET_BIT(GCR20, 6) : CLR_BIT(GCR20, 6);
	IS_BIT_SET(PortE_VOLT, 0) ? SET_BIT(GCR20, 5) : CLR_BIT(GCR20, 5);
	IS_BIT_SET(PortE_VOLT, 6) ? SET_BIT(GCR20, 4) : CLR_BIT(GCR20, 4);
	IS_BIT_SET(PortE_VOLT, 7) ? SET_BIT(GCR20, 3) : CLR_BIT(GCR20, 3);
	IS_BIT_SET(PortF_VOLT, 2) ? SET_BIT(GCR20, 2) : CLR_BIT(GCR20, 2);
	IS_BIT_SET(PortF_VOLT, 3) ? SET_BIT(GCR20, 1) : CLR_BIT(GCR20, 1);
	IS_BIT_SET(PortF_VOLT, 4) ? SET_BIT(GCR20, 0) : CLR_BIT(GCR20, 0);

	IS_BIT_SET(PortF_VOLT, 5) ? SET_BIT(GCR21, 7) : CLR_BIT(GCR21, 7);
	IS_BIT_SET(PortF_VOLT, 6) ? SET_BIT(GCR21, 6) : CLR_BIT(GCR21, 6);
	IS_BIT_SET(PortF_VOLT, 7) ? SET_BIT(GCR21, 5) : CLR_BIT(GCR21, 5);
	IS_BIT_SET(PortG_VOLT, 1) ? SET_BIT(GCR21, 4) : CLR_BIT(GCR21, 4);
	IS_BIT_SET(PortG_VOLT, 6) ? SET_BIT(GCR21, 3) : CLR_BIT(GCR21, 3);
	IS_BIT_SET(PortH_VOLT, 0) ? SET_BIT(GCR21, 2) : CLR_BIT(GCR21, 2);
	IS_BIT_SET(PortH_VOLT, 1) ? SET_BIT(GCR21, 1) : CLR_BIT(GCR21, 1);
	IS_BIT_SET(PortH_VOLT, 2) ? SET_BIT(GCR21, 0) : CLR_BIT(GCR21, 0);

	IS_BIT_SET(PortB_VOLT, 3) ? SET_BIT(GCR22, 1) : CLR_BIT(GCR22, 1);
	IS_BIT_SET(PortB_VOLT, 4) ? SET_BIT(GCR22, 0) : CLR_BIT(GCR22, 0);
}
#endif	// Support_IT8987


//------------------------------------------------------------------------------
// External RAM GPIO Registers
//------------------------------------------------------------------------------
const sInitGPIOReg code asInitGPIOReg[] =
{
	{&GCR,    GCR_Init   },		// General Control Register
	{&GCR2,   GCR2_Init  },		// General Control 2 Register
	{&GCR4,   GCR4_Init  },		// General Control 4 Register
	{&GPDRA,  PortA_Init },		// GPIO data register	// leox20160323 Move down
	{&GPDRB,  PortB_Init },
	{&GPDRC,  PortC_Init },
	{&GPDRD,  PortD_Init },
	{&GPDRE,  PortE_Init },
	{&GPDRF,  PortF_Init },
	{&GPDRG,  PortG_Init },
	{&GPDRH,  PortH_Init },
	{&GPDRI,  PortI_Init },
	{&GPDRJ,  PortJ_Init },
	
	{&GPOTA,  PortA_GPOT },		// Output Type Register	// leox20151231
	{&GPOTB,  PortB_GPOT },
	{&GPOTD,  PortD_GPOT },
	{&GPOTE,  PortE_GPOT },
	{&GPOTF,  PortF_GPOT },
	{&GPOTH,  PortH_GPOT },
	{&GPOTJ,  PortJ_GPOT },

	{&GPCRA0, PortA0_Ctrl},		// GPIO control register
	{&GPCRA1, PortA1_Ctrl},
	{&GPCRA2, PortA2_Ctrl},
	{&GPCRA3, PortA3_Ctrl},
	{&GPCRA4, PortA4_Ctrl},
	{&GPCRA5, PortA5_Ctrl},
	{&GPCRA6, PortA6_Ctrl},
	{&GPCRA7, PortA7_Ctrl},

	{&GPCRB0, PortB0_Ctrl},
	{&GPCRB1, PortB1_Ctrl},
	{&GPCRB2, PortB2_Ctrl},
	{&GPCRB3, PortB3_Ctrl},
	{&GPCRB4, PortB4_Ctrl},
	{&GPCRB5, PortB5_Ctrl},
	{&GPCRB6, PortB6_Ctrl},
	{&GPCRB7, PortB7_Ctrl},

	{&GPCRC0, PortC0_Ctrl},
	{&GPCRC1, PortC1_Ctrl},
	{&GPCRC2, PortC2_Ctrl},
	{&GPCRC3, PortC3_Ctrl},
	{&GPCRC4, PortC4_Ctrl},
	{&GPCRC5, PortC5_Ctrl},
	{&GPCRC6, PortC6_Ctrl},
	{&GPCRC7, PortC7_Ctrl},

	{&GPCRD0, PortD0_Ctrl},
	{&GPCRD1, PortD1_Ctrl},
	{&GPCRD2, PortD2_Ctrl},
	{&GPCRD3, PortD3_Ctrl},
	{&GPCRD4, PortD4_Ctrl},
	{&GPCRD5, PortD5_Ctrl},
	{&GPCRD6, PortD6_Ctrl},
	{&GPCRD7, PortD7_Ctrl},

	{&GPCRE0, PortE0_Ctrl},
	{&GPCRE1, PortE1_Ctrl},
	{&GPCRE2, PortE2_Ctrl},
	{&GPCRE3, PortE3_Ctrl},
	{&GPCRE4, PortE4_Ctrl},
	{&GPCRE5, PortE5_Ctrl},
	{&GPCRE6, PortE6_Ctrl},
	{&GPCRE7, PortE7_Ctrl},

	{&GPCRF0, PortF0_Ctrl},
	{&GPCRF1, PortF1_Ctrl},
	{&GPCRF2, PortF2_Ctrl},
	{&GPCRF3, PortF3_Ctrl},
	{&GPCRF4, PortF4_Ctrl},
	{&GPCRF5, PortF5_Ctrl},
	{&GPCRF6, PortF6_Ctrl},
	{&GPCRF7, PortF7_Ctrl},

	{&GPCRG0, PortG0_Ctrl},
	{&GPCRG1, PortG1_Ctrl},
	{&GPCRG2, PortG2_Ctrl},
	{&GPCRG3, PortG3_Ctrl},
	{&GPCRG4, PortG4_Ctrl},
	{&GPCRG5, PortG5_Ctrl},
	{&GPCRG6, PortG6_Ctrl},
	{&GPCRG7, PortG7_Ctrl},

	{&GPCRH0, PortH0_Ctrl},
	{&GPCRH1, PortH1_Ctrl},
	{&GPCRH2, PortH2_Ctrl},
	{&GPCRH3, PortH3_Ctrl},
	{&GPCRH4, PortH4_Ctrl},
	{&GPCRH5, PortH5_Ctrl},
	{&GPCRH6, PortH6_Ctrl},
	#if Support_IT8987			// leox20150923 For IT8987 or later
	{&GPCRH7, PortH7_Ctrl},		// leox_20141016 For IT8987
	#endif

	{&GPCRI0, PortI0_Ctrl},
	{&GPCRI1, PortI1_Ctrl},
	{&GPCRI2, PortI2_Ctrl},
	{&GPCRI3, PortI3_Ctrl},
	{&GPCRI4, PortI4_Ctrl},
	{&GPCRI5, PortI5_Ctrl},
	{&GPCRI6, PortI6_Ctrl},
	{&GPCRI7, PortI7_Ctrl},

	{&GPCRJ0, PortJ0_Ctrl},
	{&GPCRJ1, PortJ1_Ctrl},
	{&GPCRJ2, PortJ2_Ctrl},
	{&GPCRJ3, PortJ3_Ctrl},
	{&GPCRJ4, PortJ4_Ctrl},
	{&GPCRJ5, PortJ5_Ctrl},
	#ifdef Support_GPJ6_GPJ7	// leox_20121213
	{&GPCRJ6, PortJ6_Ctrl},
	{&GPCRJ7, PortJ7_Ctrl},
	#endif

	/*
	{&GPDRA,  PortA_Init },		// GPIO data register	// leox20160323 Move down
	{&GPDRB,  PortB_Init },
	{&GPDRC,  PortC_Init },
	{&GPDRD,  PortD_Init },
	{&GPDRE,  PortE_Init },
	{&GPDRF,  PortF_Init },
	{&GPDRG,  PortG_Init },
	{&GPDRH,  PortH_Init },
	{&GPDRI,  PortI_Init },
	{&GPDRJ,  PortJ_Init },
	*/
};


//------------------------------------------------------------------------------
// Initial GPIO Registers
//------------------------------------------------------------------------------
void Init_GPIO(void)
{
	BYTE index = 0;

	#if Support_IT8987		// chris_20140924	// leox20160323 Move up to initial Input Voltage Selection first
	Set_Input_Voltage();	// leox20160105
	#endif

	while (index < (sizeof(asInitGPIOReg) / sizeof(sInitGPIOReg)))
	{
		*asInitGPIOReg[index].reg = asInitGPIOReg[index].value;
		index++;
	}
}
