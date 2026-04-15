//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

#ifndef SOURCES_IO_H_
#define SOURCES_IO_H_

#include <stdbool.h>

#define on 1
#define off 0
#define up 0
#define down 1

void setSolenoid(bool state);
void setPump(bool state);
void setValve(bool state);
bool btnPosFlank(void);
void ioInit(void);

#endif /* SOURCES_IO_H_ */
