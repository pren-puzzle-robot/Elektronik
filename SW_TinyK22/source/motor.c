
//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

#include "motor.h"
#include "platform.h"
#include "ftm0.h"
#include <stdbool.h>
#include <stdint.h>

#define STEP_CHANNEL_X        1
#define STEP_CHANNEL_Y        2
#define STEP_CHANNEL_ROT        0

#define STEP_HIGH_TICKS 50   // 20 µs (250kHz)
#define F_MIN 4   // HZ
#define F_MAX 10000   // HZ

typedef struct
{
    uint8_t channel;
    volatile uint32_t stepTicks;
    volatile uint16_t lowTicks;
    volatile bool busy;
    volatile bool stepIsHigh;
} tMotorState;

static volatile tMotorState motors[3] = {
    { STEP_CHANNEL_X, 0, 0, FALSE, FALSE },
    { STEP_CHANNEL_Y, 0, 0, FALSE, FALSE },
    { STEP_CHANNEL_ROT, 0, 0, FALSE, FALSE }
};

static inline void setStepHigh(tMotor motor)
{
    if (motor == MOTOR_X)
        GPIOA->PSOR = (1u << 4);
    else if (motor == MOTOR_Y)
        GPIOA->PSOR = (1u << 5);
    else
        GPIOB->PSOR = (1u << 3);
}

static inline void setStepLow(tMotor motor)
{
    if (motor == MOTOR_X)
        GPIOA->PCOR = (1u << 4);
    else if (motor == MOTOR_Y)
        GPIOA->PCOR = (1u << 5);
    else
        GPIOB->PCOR = (1u << 3);
}

static inline void setDIR(tMotor motor, bool state)
{
    uint32_t mask;

    if (motor == MOTOR_X)
        mask = (1u << 0);
    else if (motor == MOTOR_Y)
        mask = (1u << 1);
    else
        mask = (1u << 2);

    if (state)
        GPIOD->PSOR = mask;
    else
        GPIOD->PCOR = mask;
}

static void motorIRQ(tMotor motor)
{
    volatile tMotorState *m = &motors[motor];

    FTM0->CONTROLS[m->channel].CnSC &= ~FTM_CnSC_CHF_MASK;

    if (!m->busy)
    {
        FTM0->CONTROLS[m->channel].CnSC = 0;
        setStepLow(motor);
        m->stepIsHigh = FALSE;
        return;
    }

    if (!m->stepIsHigh)
    {
        setStepHigh(motor);
        m->stepIsHigh = TRUE;
        FTM0->CONTROLS[m->channel].CnV = FTM0->CNT + STEP_HIGH_TICKS;
    }
    else
    {
        setStepLow(motor);
        m->stepIsHigh = FALSE;

        if (m->stepTicks > 0)
            m->stepTicks--;

        if (m->stepTicks == 0)
        {
            FTM0->CONTROLS[m->channel].CnSC = 0;
            m->busy = FALSE;
        }
        else
        {
            FTM0->CONTROLS[m->channel].CnV = FTM0->CNT + m->lowTicks;
        }
    }
}

void FTM0CH0_IRQHandler(void)
{
	motorIRQ(MOTOR_ROT);
}

void FTM0CH1_IRQHandler(void)
{
	motorIRQ(MOTOR_X);
}

void FTM0CH2_IRQHandler(void)
{
    motorIRQ(MOTOR_Y);
}

void motorDrive(tMotor motor, uint16_t frequency, uint16_t steps, bool dir)
{
    volatile tMotorState *m = &motors[motor];

    if (m->busy || frequency < F_MIN || frequency > F_MAX || steps == 0)
        return;

    setDIR(motor, dir);

    m->busy = TRUE;
    m->stepIsHigh = FALSE;
    m->stepTicks = steps;
    m->lowTicks = (FTM0_CLOCK / frequency) - STEP_HIGH_TICKS;

    setStepLow(motor);

    FTM0->CONTROLS[m->channel].CnSC = FTM_CnSC_MSA_MASK | FTM_CnSC_CHIE_MASK;
    FTM0->CONTROLS[m->channel].CnV = FTM0->CNT + 1;
}

bool motorBusy(void)
{
    return motors[MOTOR_X].busy || motors[MOTOR_Y].busy || motors[MOTOR_ROT].busy;
}

void motorStop(tMotor motor)
{
    volatile tMotorState *m = &motors[motor];

    m->busy = FALSE;
    m->stepTicks = 0;
    m->stepIsHigh = FALSE;
    setStepLow(motor);
    FTM0->CONTROLS[m->channel].CnSC = 0;
}

/**
 * Initializes digital IOs
 */

void motorInit(void)
{
	ftm0Init();
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

    // DIR -> GPIO
    PORTD->PCR[0] = PORT_PCR_MUX(1);
	PORTD->PCR[1] = PORT_PCR_MUX(1);
	PORTD->PCR[2] = PORT_PCR_MUX(1);
	GPIOD->PDDR |= (1u << 0) | (1u << 1) | (1u << 2);

    // STEP -> FTM0; CH0. CH1, CH2
	PORTB->PCR[3] = PORT_PCR_MUX(1);
	PORTA->PCR[4] = PORT_PCR_MUX(1);
	PORTA->PCR[5] = PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1u << 3);
	GPIOA->PDDR |= (1u << 4) | (1u << 5);

    // Initialzustand
	GPIOD->PCOR = (1u << 0) | (1u << 1) | (1u << 2);
	GPIOA->PCOR = (1u << 4) | (1u << 5);
	GPIOB->PCOR = (1u << 3);

    // Channel zunächst aus
	FTM0->CONTROLS[STEP_CHANNEL_X].CnSC = 0;
	FTM0->CONTROLS[STEP_CHANNEL_X].CnV = 0;
	FTM0->CONTROLS[STEP_CHANNEL_Y].CnSC = 0;
	FTM0->CONTROLS[STEP_CHANNEL_Y].CnV = 0;
	FTM0->CONTROLS[STEP_CHANNEL_ROT].CnSC = 0;
	FTM0->CONTROLS[STEP_CHANNEL_ROT].CnV = 0;
}
