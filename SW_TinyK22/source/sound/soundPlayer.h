//         __  ___   ______   ______   __  __    _   __
//        /  |/  /  / ____/  / ____/  / / / /   / | / /
//       / /|_/ /  / /      / /_     / / / /   /  |/ /
//      / /  / /  / /___   / __/    / /_/ /   / /|  /
//     /_/  /_/   \____/  /_/       \____/   /_/ |_/
//     (c) Hochschule Luzern T&A  ==== www.hslu.ch ====
//
//     \brief   Nokia Ringtone player
//     \author  Christian Jost, christian.jost@hslu.ch
//     \date    25.02.2025
//     ------------------------------------------------

#ifndef SOURCES_SOUNDPLAYER_H_
#define SOURCES_SOUNDPLAYER_H_

bool soundIsPlaying(void);
void soundTooglePlayPause(void);
void soundStart(void);
void soundStop(void);
void soundLoad(const char *soundFile);
void soundPlay(const char *soundFile);
void soundPlayerInit(void);

#endif /* SOURCES_SOUNDPLAYER_H_ */
