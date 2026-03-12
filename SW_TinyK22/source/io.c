
//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

/**
 * Initialize digital IOs
 */
#include <io.h>
#include "platform.h"

#if !SOLUTION

void setSolenoid(bool state)
{
	if (state)
		GPIOD->PSOR |= 1<<7;
	else
		GPIOD->PCOR |= 1<<7;
}
void setPump(bool state)
{
	if (state)
		GPIOD->PSOR |= 1<<3;
	else
		GPIOD->PCOR |= 1<<3;
}

bool btnPosFlank(void)
{
    static bool lastState = false;
    bool currentState;
    bool flank = false;

    currentState = (GPIOC->PDIR & (1 << 8)) != 0;

    if (!lastState && currentState)
    {
        flank = true;
    }
    lastState = currentState;

    return flank;
}


/**
 * Initializes digital IOs
 */
void ioInit(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
  PORTC->PCR[2] = PORT_PCR_MUX(1);  // MUX PTC2 as GPIO pin (tinyK22 led blue)
  PORTC->PCR[8] |=PORT_PCR_MUX(1)| PORT_PCR_PE_MASK;	//button
  PORTC->PCR[9] = PORT_PCR_MUX(1)| PORT_PCR_PE_MASK;	//swX
  PORTC->PCR[10] = PORT_PCR_MUX(1)| PORT_PCR_PE_MASK;	//swY
  PORTC->PCR[11] = PORT_PCR_MUX(1)| PORT_PCR_PE_MASK;	//V_24V

  PORTD->PCR[3] = PORT_PCR_MUX(1);	//pump
  PORTD->PCR[6] = PORT_PCR_MUX(1);	//ventil
  PORTD->PCR[7] = PORT_PCR_MUX(1);	//solenoid

  GPIOC->PDDR |= (1<<2);            // port direction as output
  GPIOD->PDDR |= 0b11001000;        // port direction as output

  GPIOC->PDDR &= ~(1 << 8);
  GPIOC->PDDR &= ~(1 << 9);
  GPIOC->PDDR &= ~(1 << 10);
  GPIOC->PDDR &= ~(1 << 11);
}
#endif
