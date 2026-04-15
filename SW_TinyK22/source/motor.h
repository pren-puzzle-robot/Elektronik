//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

#ifndef SOURCES_MOTOR_H_
#define SOURCES_MOTOR_H_

#include <stdbool.h>
#include <stdint.h>

#define F_NOM 3000
#define ACC 10

typedef enum
{
    MOTOR_X = 0,
    MOTOR_Y = 1,
    MOTOR_ROT = 2
} tMotor;

void motorInit(void);
void motorDrive(tMotor motor, uint16_t frequency, uint16_t steps, bool dir);
bool motorBusy(void);
void motorStop(tMotor motor);

#endif /* SOURCES_MOTOR_H_ */
