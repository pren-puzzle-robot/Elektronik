//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

#ifndef SOURCES_MOTOR_H_
#define SOURCES_MOTOR_H_

#include <stdbool.h>

void motorStep(bool state);
void setDIR(bool state);
void motorInit(void);
void motorStep10(void);


#endif /* SOURCES_MOTOR_H_ */
