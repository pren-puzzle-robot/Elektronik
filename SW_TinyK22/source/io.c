//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

/**
 * Initialize digital IOs
 */
#include <io.h>
#include "platform.h"

#if !SOLUTION


void delay_ms(uint32_t ms)
{
    volatile uint32_t i;

    while (ms--)
    {
        for (i = 0; i < 21; i++)
        {
            __NOP();
        }
    }
}

void setSolenoid(bool state)
{
    if (state)
        GPIOD->PSOR |= 1 << 7;
    else
        GPIOD->PCOR |= 1 << 7;
}

void setPump(bool state)
{
    if (state)
        GPIOD->PSOR |= 1 << 3;
    else
        GPIOD->PCOR |= 1 << 3;
}

void setValve(bool state)
{
    if (state)
        GPIOD->PSOR |= 1 << 6;
    else
        GPIOD->PCOR |= 1 << 6;
}

bool getSwX0(void)
{
    return ((GPIOC->PDIR & (1u << 9)) == 0);   // true when pressed
}

bool getSwY0(void)
{
    return ((GPIOC->PDIR & (1u << 10)) == 0);
}

bool getSwXEnd(void)
{
    return ((GPIOB->PDIR & (1u << 0)) == 0);
}

bool getSwYEnd(void)
{
    return ((GPIOB->PDIR & (1u << 1)) == 0);
}

bool btnNegFlank(void)
{
    static bool lastState = true;   //(Pull-Up → idle = HIGH)
    bool currentState;
    bool flank = false;

    currentState = (GPIOC->PDIR & (1 << 8)) != 0;
    // neg flank
    if (lastState && !currentState){
        flank = true;
        delay_ms(300);
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
  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

  //output
  PORTD->PCR[3] = PORT_PCR_MUX(1); // pump
  //PORTD->PCR[6] = PORT_PCR_MUX(1); // valve
  PORTD->PCR[7] = PORT_PCR_MUX(1); // solenoid

  GPIOC->PDDR |= (1 << 2);
  GPIOD->PDDR |= (1 << 3) | (1 << 6) | (1 << 7);

  //input
  PORTC->PCR[8]  |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // button
  PORTC->PCR[9]   = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xA); // swX0 falling edge
  PORTC->PCR[10]  = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xA); // swY0 falling edge
  PORTB->PCR[0]   = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xA); // swXEnd falling edge
  PORTB->PCR[1]   = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xA);  // swYEnd falling edge
  PORTB->PCR[2]   = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK | PORT_PCR_IRQC(0x9); // V_24V rising edge

  GPIOC->PDDR &= ~(1 << 8);
  GPIOC->PDDR &= ~(1 << 9);
  GPIOC->PDDR &= ~(1 << 10);
  GPIOC->PDDR &= ~(1 << 11);

  PORTC->ISFR = (1u << 9) | (1u << 10);
  NVIC_ClearPendingIRQ(PORTC_IRQn);
  NVIC_EnableIRQ(PORTC_IRQn);

  PORTB->ISFR = (1u << 0) | (1u << 1) | (1u << 2);
  NVIC_ClearPendingIRQ(PORTB_IRQn);
  NVIC_EnableIRQ(PORTB_IRQn);
}
#endif
