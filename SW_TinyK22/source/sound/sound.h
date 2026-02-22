//         __  ___   ______   ______   __  __    _   __
//        /  |/  /  / ____/  / ____/  / / / /   / | / /
//       / /|_/ /  / /      / /_     / / / /   /  |/ /
//      / /  / /  / /___   / __/    / /_/ /   / /|  /
//     /_/  /_/   \____/  /_/       \____/   /_/ |_/
//     (c) Hochschule Luzern T&A  ==== www.hslu.ch ====
//
//     \brief   Buzzer driver to generate different frequencies
//     \author  Christian Jost, christian.jost@hslu.ch
//     \date    25.02.2025
//     ------------------------------------------------

#ifndef SOURCES_SOUND_H_
#define SOURCES_SOUND_H_


typedef void (*soundFinishedHandler)(void);

void soundSetCallbackHandler(soundFinishedHandler handler);
void soundBeep(uint16_t frequency, uint16_t timeMS);
void soundInit(void);


#endif /* SOURCES_SOUND_H_ */
