//         __  ___   ______   ______   __  __    _   __
//        /  |/  /  / ____/  / ____/  / / / /   / | / /
//       / /|_/ /  / /      / /_     / / / /   /  |/ /
//      / /  / /  / /___   / __/    / /_/ /   / /|  /
//     /_/  /_/   \____/  /_/       \____/   /_/ |_/
//     (c) Hochschule Luzern T&A  ==== www.hslu.ch ====
//
//     \brief   terminal program
//     \author  Christian Jost, christian.jost@hslu.ch
//     \date    25.02.2025
//     ------------------------------------------------

#ifndef SOURCES_TERM_H_
#define SOURCES_TERM_H_

typedef tError (*cmdHandler)(const char *cmd);

typedef struct commandLineHandler
{
  char cmd[16];
  char cmdDesc[32];
  cmdHandler cmdHandler;
  struct commandLineHandler *next;
} tCommandLineHandler;

void termRegisterCommandLineHandler(tCommandLineHandler *clh, char* cmd, char *cmdDesc, cmdHandler h);

void termWrite(const char *str);
void termWriteNum32s(int32_t value);
void termWriteNum16s(int16_t value);

void termWriteLine(const char *str);
void termDoWork(void);
void termInit(uint16_t baudrate);


#endif /* SOURCES_TERM_H_ */
