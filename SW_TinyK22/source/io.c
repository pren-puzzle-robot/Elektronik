
//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

/**
 * Initialize digital IOs
 */
#include <io.h>
#include "platform.h"
#include "motor.h"

#if !SOLUTION

void PORTC_IRQHandler(void)
{
    if (PORTC->ISFR & (1u << 9))
    {
        PORTC->ISFR = (1u << 9);
        motorStop(MOTOR_X);
    }

    if (PORTC->ISFR & (1u << 10))
    {
        PORTC->ISFR = (1u << 10);
        motorStop(MOTOR_Y);
    }
}

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

void setValve(bool state)
{
    if (state)
        GPIOD->PSOR |= 1 << 6;
    else
        GPIOD->PCOR |= 1 << 6;
}

bool btnPosFlank(void)
{
    static bool lastStableState = false;
    static bool lastRawState = false;
    static uint8_t debounceCnt = 0;

    bool rawState;
    bool flank = false;

    rawState = (GPIOC->PDIR & (1 << 8)) != 0;

    if (rawState != lastRawState)
    {
        debounceCnt = 5;          // Entprellzeit (Anzahl Aufrufe)
        lastRawState = rawState;
    }
    else if (debounceCnt > 0)
    {
        debounceCnt--;
        if (debounceCnt == 0)
        {
            if (!lastStableState && rawState)
            {
                flank = true;     // positive Flanke erkannt
            }
            lastStableState = rawState;
        }
    }

    return flank;
}


/**
 * Initializes digital IOs
 */
void ioInit(void)
{
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

  PORTC->PCR[8] |=PORT_PCR_MUX(1)| PORT_PCR_PE_MASK;	//button
  PORTC->PCR[9]  = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x9); //swX  (bei pull down: interner pull up | PORT_PCR_PS_MASK | und 0xA falling edge)
  PORTC->PCR[10] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x9); //swY
  PORTC->PCR[11] |= PORT_PCR_MUX(1)| PORT_PCR_PE_MASK;	//V_24V

  PORTD->PCR[3] = PORT_PCR_MUX(1);	//pump
  PORTD->PCR[6] = PORT_PCR_MUX(1);	//valve
  PORTD->PCR[7] = PORT_PCR_MUX(1);	//solenoid

  GPIOC->PDDR |= (1<<2);            // port direction as output
  GPIOD->PDDR |= (1 << 3) | (1 << 6) | (1 << 7);

  GPIOC->PDDR &= ~(1 << 8);			// port direction as input
  GPIOC->PDDR &= ~(1 << 9);
  GPIOC->PDDR &= ~(1 << 10);
  GPIOC->PDDR &= ~(1 << 11);

  PORTC->ISFR = (1u << 9) | (1u << 10);
  NVIC_ClearPendingIRQ(PORTC_IRQn);
  NVIC_EnableIRQ(PORTC_IRQn);
}
#endif
