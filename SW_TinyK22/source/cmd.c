
//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

#include "cmd.h"
#include "io.h"
#include "motor.h"
#include "uart.h"
#include "platform.h"

static char cmd;
static uint16_t coord_x;
static uint16_t coord_y;
static uint16_t rot_z;

bool getCmd(void){

	cmd = uart0ReadChar();
	if (cmd == 'M' || cmd == 'H' || cmd == 'h' || cmd == 'L' || cmd == 'l' || cmd == 'R')
		return TRUE;
	else
		return FALSE;
}

bool processCmd(void){
	if (cmd == 'M')
		motorStep10();
	if (cmd == 'H')
		setPump(on);
	if (cmd == 'h')
		setPump(off);
	if (cmd == 'L')
		setSolenoid(up);
	if (cmd == 'l')
		setSolenoid(down);
	if (cmd == 'R')
		;
	return TRUE;
}

bool cmdStart(void){
	btnPosFlank();
}
void cmdInit(void){
	ioInit();
	motorInit();
}
