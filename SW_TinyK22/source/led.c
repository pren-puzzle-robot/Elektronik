//         __  ___   ______   ______   __  __    _   __
//        /  |/  /  / ____/  / ____/  / / / /   / | / /
//       / /|_/ /  / /      / /_     / / / /   /  |/ /
//      / /  / /  / /___   / __/    / /_/ /   / /|  /
//     /_/  /_/   \____/  /_/       \____/   /_/ |_/
//     (c) Hochschule Luzern T&A  ==== www.hslu.ch ====
//
//     \brief   driver for the
//              - blue user led (PTC2) on the tinyK22 or
//              - red rear left led (PTA17) on the MC-Car
//     \author  Christian Jost, christian.jost@hslu.ch
//     \date    24.03.2025
//     ------------------------------------------------
#include <string.h>
#include "platform.h"
#include "term.h"
#include "led.h"

#if !SOLUTION

/**
 * Enables the blue led on the tinyk22 or
 * the red rear left led on the MC-Car
 */
void ledSetOn(void)
{
  // todo #06.06 enable the led on PTC2 (TinyK22) or PTA17 (MC-Car)
	GPIOC->PCOR |= 1<<2;
}

/**
 * Disables the blue led on the tinyk22 or
 * the red rear left led on the MC-Car
 */
void ledSetOff(void)
{
  // todo #06.07 disable the blue led on PTC2 (TinyK22) or PTA17 (MC-Car)
	GPIOC->PSOR |= 1<<2;
}


/**
 * Returns the information if the blue led on the tinyk22
 * or the red read left led on the MC-Car is on or off.
 *
 * @returns
 *   TRUE, if the led is on, FALSE otherweise
 */
bool ledIsOn(void)
{
  bool ledTiny = !(GPIOC->PDOR & (1<<2));
  return ledTiny;
}


/**
 * This function parses one command line, executes the command and returns the status
 *
 * @param[in] cmd
 *   the null terminated string to parse
 * @returns
 *   EC_SUCCESS if there was a valid command
 *   EC_INVALID_CMD if the command was unknown or invalid
 */
tError ledParseCommand(const char *cmd)
{
  if (strcmp(cmd, "help") == 0)
  {
    termWriteLine("led commands:");
    termWriteLine("  help");
    termWriteLine("  status");
    termWriteLine("  set [1|0]");
    return EC_SUCCESS;
  }
  // todo #06.08 implement the command "status"

  else if (strcmp(cmd, "status") == 0)
    {
      termWrite("led status: ");
      if (ledIsOn()) termWriteLine("1");
      else termWriteLine("0");
      return EC_SUCCESS;
    }

  else if (strncmp(cmd, "set ", sizeof("set")) == 0)
  {
    cmd += sizeof("set ") - 1;
    if (*cmd == '1')
    {
      ledSetOn();
      return EC_SUCCESS;
    }
    else if (*cmd == '0')
    {
      ledSetOff();
      return EC_SUCCESS;
    }
  }
  return EC_INVALID_CMD;
}


/**
 * Initializes the tiny led driver for the blue led on PTC2.
 */
void ledInit(void)
{
  static tCommandLineHandler clh;   // terminal command line handler
  PORTC->PCR[2] = PORT_PCR_MUX(1);  // MUX PTC2 as GPIO pin (tinyK22 led blue)
  GPIOC->PDDR |= (1<<2);            // port direction as output

  ledSetOn();                      // set led off

  // register terminal command line handler
  termRegisterCommandLineHandler(&clh, "led", "(Led TinyK22/MC-Car)", ledParseCommand);
}
#endif
