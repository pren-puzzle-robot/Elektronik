
//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

#include "cmd.h"
#include "io.h"
#include "motor.h"
#include "uart.h"
#include "platform.h"
#include "stdio.h"

#define MAX_X 10000 		// *10 steps, MS1 = 0, MS2 = 0, 360° -> 200 steps, mm?
#define MAX_Y 10000			// *10 steps, MS1 = 0, MS2 = 0, 360° -> 200 steps, mm?
#define MAX_ROT 1600		//MS1 = 1, MS2 = 1, 360° -> 1600 steps

static char cmd;

//current position coordinates (absolute) 0-65535
static uint16_t x_abs = 0;
static uint16_t y_abs = 0;
static int rot_abs = 0;
//relative coordinates to be moved
static int x_rel;
static int y_rel;
static int rot_rel;
//ordered coordinates from Raspi (absolute)
static uint16_t x_new;
static uint16_t y_new;
static int rot_new;

static uint16_t abs16(int value) //absolute value of int
{
    if (value < 0)
        return (uint16_t)(-value);
    return (uint16_t)value;
}

bool getCmd(void){

	if (PLATFORM == RASPY)
		cmd = uart0ReadChar();
	else
		cmd = uart1ReadChar();

	if (cmd == 'H' || cmd == 'h' || cmd == 'L' || cmd == 'l' || cmd == 'R')
		return TRUE;
	else if (cmd == 'M'){

		if (PLATFORM == RASPY){
			x_new = uart0ReadInfo();
			y_new = uart0ReadInfo();
			rot_new = uart0ReadInfo();		//0-360°
		}
		else {
			x_new = uart1ReadInfo();
			y_new = uart1ReadInfo();
			rot_new = uart1ReadInfo();		//0-360°
		}


		if (rot_new > 800)				//-180° bis 180°
			rot_new = rot_new -1600;

		x_rel = x_new - x_abs;
		y_rel = y_new - y_abs;
		rot_rel = rot_new - rot_abs;

		return TRUE;
	}
	else
		return FALSE;
}

bool processCmd(void){
	char str[50]; //will be removed
	if (cmd == 'M' && !motorBusy()){
		motorDrive(MOTOR_Y, F_NOM, (abs16(y_rel)*10), (y_rel < 0));
		motorDrive(MOTOR_X, F_NOM, (abs16(x_rel)*10), (x_rel < 0));
		motorDrive(MOTOR_ROT, F_NOM, (abs16(rot_rel)), (rot_rel < 0));

		snprintf(str, sizeof(str), "x_rel: %d y_rel: %d z_rel: %d\r\n", x_rel, y_rel, rot_rel); //will be removed
		uart1Write(str); //will be removed

		x_abs = x_new;
		y_abs = y_new;
		rot_abs = rot_new;
	}
	if (cmd == 'H'){
		setPump(on);
		setValve(off);
	}
	if (cmd == 'h'){
		setPump(off);
		setValve(on);
	}
	if (cmd == 'L')
		setSolenoid(up);
	if (cmd == 'l')
		setSolenoid(down);
	if (cmd == 'R')
		; //resetSystem();

	if (motorBusy())
		return FALSE;
	else
		return TRUE;
}

bool cmdStart(void){
	return btnNegFlank();
}

void cmdInit(void){
	ioInit();
	motorInit();
}

bool resetSystem(void){
	if (getSwX0() && getSwY0())
		return TRUE;
	else
		return FALSE;

	motorDrive(MOTOR_Y, F_NOM, (MAX_X + 100), (1));
	motorDrive(MOTOR_X, F_NOM, (MAX_X + 100), (1));
	rot_new = 0;
	rot_rel = rot_new - rot_abs;
	motorDrive(MOTOR_ROT, F_NOM, (abs16(rot_rel)), (rot_rel < 0));
	rot_abs=0;

}

void PORTC_IRQHandler(void)
{
    if (PORTC->ISFR & (1u << 9))
    {
        PORTC->ISFR = (1u << 9);
        motorStop(MOTOR_X);
        x_abs = 0;
        x_new = 0;
        x_rel = 0;
    }

    if (PORTC->ISFR & (1u << 10))
    {
        PORTC->ISFR = (1u << 10);
        motorStop(MOTOR_Y);
        y_abs = 0;
		y_new = 0;
		y_rel = 0;
    }
}

void PORTB_IRQHandler(void)
{
    if (PORTB->ISFR & (1u << 0))
    {
        PORTB->ISFR = (1u << 0);
        motorStop(MOTOR_X);
    }

    if (PORTB->ISFR & (1u << 1))
    {
        PORTB->ISFR = (1u << 1);
        motorStop(MOTOR_Y);
    }
}
