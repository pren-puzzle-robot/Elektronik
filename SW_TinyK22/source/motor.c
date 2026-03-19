
//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------


#include <motor.h>
#include "platform.h"

void setDIR(bool state)
{
	if (state)
		GPIOD->PSOR |= 1;
	else
		GPIOD->PCOR |= 1;
}
void motorStep(bool state)
{
	if (state)
		GPIOA->PSOR |= 1<<4;
	else
		GPIOA->PCOR |= 1<<4;
}

void motorStep10(void)
{
    static bool dir = false;   // remembers direction between calls
    dir = !dir;                // toggle direction each time function runs

    setDIR(dir);

    for (int i = 0; i < 10; i++)
    {
        motorStep(true);       // STEP high
        for (volatile int d = 0; d < 500000; d++);  // small delay

        motorStep(false);      // STEP low
        for (volatile int d = 0; d < 500000; d++);  // small delay
    }
}


/**
 * Initializes digital IOs
 */
void motorInit(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

  PORTD->PCR[0] = PORT_PCR_MUX(1);
  PORTA->PCR[4] = PORT_PCR_MUX(1);

  GPIOA->PDDR |= (1<<4);            // port direction as output
  GPIOD->PDDR |= (1);       // port direction as output
}
