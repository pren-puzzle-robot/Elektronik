

//     \author  PREN Team 13
//     \date    12.03.2026
//     ------------------------------------------------

#include "platform.h"
#include "term.h"
#include "cmd.h"

typedef enum {
	    INIT,
	    START,
	    IDLE,
	    PROCESSING,
	    RESET,
	    ERROR
	} state_t;

state_t state = INIT;

int main(void)
{
	while(TRUE)
	{

		switch(state)

		{
			case INIT:	// initialize hardware
				termInit(57600);
				cmdInit();
				state = RESET;
				break;

			case RESET:
				state = START; //will be removed

				// reset and calibrate system
				if (resetSystem())
					state = START;
				break;

			case START: // wait for start

				//state = IDLE;
				if (cmdStart())
				{
					termWriteChar('S');
					state = IDLE;
				}
			    break;

			case IDLE:
				// wait for instruction and send ACK

				if (cmdStart())
				{
					//resetSystem();
				}

				if (termDataAvailable())
				{

					if (getCmd())		//command is valid
					{
						termWriteChar('A');
						state = PROCESSING;
					}
					else
						termWriteChar('N');
				}

				break;

			case PROCESSING:
				// do instruction and send DONE
				if (processCmd())		//command is done
					{
						termWriteChar('D');
						state = IDLE;
					}

				break;

			case ERROR:

				//handleError();
				state = RESET;
				break;
		}
	}

	// Never leave main
	for (;;){}
	return 0;
}
