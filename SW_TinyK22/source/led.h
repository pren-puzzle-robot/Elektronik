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
#ifndef SOURCES_LED_H_
#define SOURCES_LED_H_


void ledSetOn(void);
void ledSetOff(void);
bool ledIsOn(void);
tError ledParseCommand(const char *cmd);
void ledInit(void);


#endif /* SOURCES_TINYLED_H_ */
