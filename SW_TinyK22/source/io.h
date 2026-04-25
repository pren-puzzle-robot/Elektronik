//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

#ifndef SOURCES_IO_H_
#define SOURCES_IO_H_

#include <stdbool.h>

void setSolenoid(bool state);
void setPump(bool state);
void setValve(bool state);
bool getSwX0(void);
bool getSwY0(void);
bool getSwXEnd(void);
bool getSwYEnd(void);
bool btnNegFlank(void);
void ioInit(void);

#endif /* SOURCES_IO_H_ */
