//         __  ___   ______   ______   __  __    _   __
//        /  |/  /  / ____/  / ____/  / / / /   / | / /
//       / /|_/ /  / /      / /_     / / / /   /  |/ /
//      / /  / /  / /___   / __/    / /_/ /   / /|  /
//     /_/  /_/   \____/  /_/       \____/   /_/ |_/
//     (c) Hochschule Luzern T&A  ==== www.hslu.ch ====
//
//     \brief   Exercise 07.3 - Buzzer with timer output compare
//     \author  Christian Jost, christian.jost@hslu.ch
//     \date    25.02.2025
//     ------------------------------------------------

#include "platform.h"
#include "ftm0.h"
#include "term.h"
#include "sound.h"
#include "soundPlayer.h"


void init_led_blink_ftm3_oc(void) {
    // Clock für Port C aktivieren
   // SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

    // Clock für FTM3 aktivieren
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;

    // PTC8 als FTM3_CH4 konfigurieren (ALT3)
   // PORTC->PCR[8] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[8] |= PORT_PCR_MUX(3);  // ALT3 = FTM3_CH4

    // Write protection deaktivieren
  //  FTM3->MODE |= FTM_MODE_WPDIS_MASK;

    // Zähler stoppen
 //   FTM3->SC = 0;

    // Modulo setzen – 375000 Ticks für 1 s (mit 48 MHz / 128 Prescaler)
    FTM3->MOD = 0xFFFF;

    // Compare-Wert in die Mitte setzen (z. B. nach 0.5 s → toggeln)
    //FTM3->CONTROLS[4].CnV = 187500;


    // Channel konfigurieren: Output Compare + Toggle
    FTM3->CONTROLS[4].CnSC =
        FTM_CnSC_MSA_MASK |   // Output Compare Modus aktivieren
        FTM_CnSC_ELSA_MASK;   // Toggle on match

    // Prescaler /128, System Clock wählen
    FTM3->SC = FTM_SC_CLKS(2) | FTM_SC_PS(3);  // CLKS=01, PS=111 (div 128)
    int tmrFrequencyTicks = ((250000/8)/(2*0.5));
    FTM3->CONTROLS[4].CnV = tmrFrequencyTicks;
}

/**
 * Buzzer test program with terminal
 */
int main(void)
{
  EnableDebugLeds();
  ftm0Init();
  termInit(57600);
  soundInit();
  soundPlayerInit();
  soundBeep(4000, 100);
  init_led_blink_ftm3_oc();
  int tmrFrequencyTicks = ((250000/8)/(2*1));
  while(TRUE)
  {
    termDoWork();


    if (FTM3->CONTROLS[4].CnSC & FTM_CnSC_CHF_MASK) // wait until TOF flag is set
    			 {
    				 FTM3->CONTROLS[4].CnSC &= ~FTM_CnSC_CHF_MASK;
    				 FTM3->CONTROLS[4].CnV = (FTM3->CNT + tmrFrequencyTicks) & 0xFFFF;
    			 }
  }

  // Never leave main
  for (;;){}
  return 0;
}
