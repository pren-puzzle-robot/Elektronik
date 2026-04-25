//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------



#ifndef SOURCES_CMD_H_
#define SOURCES_CMD_H_

#include <stdbool.h>

#define on 1
#define off 0
#define up 0
#define down 1

void cmdInit(void);
bool cmdStart(void);
bool processCmd(void);
bool getCmd(void);
bool resetSystem(void);

#endif /* SOURCES_CMD_H_ */

