#include "Cartes/gestion_IT.h"
#include "BlocActions.h"
#include "Ecrans/Ecran.h"
#include "config.h"
#include <Arduino.h>

hw_timer_t *timer = NULL;
hw_timer_t *timer10ms = NULL;

// prototypes
void GestionIT_10ms();

// Interruptions, Current Zero Crossing from Triac device and Internal Timer
//*************************************************************************
void IRAM_ATTR onTimer10ms() { // Interruption interne toutes 10ms
  ITmode = ITmode - 1;
  if (ITmode < -5)
    ITmode = -5;
  if (ITmode < 0)
    GestionIT_10ms(); // IT non synchrone avec le secteur . Horloge interne
}

// Interruption du Triac Signal Zc, toutes les 10ms si Triac, toutes les 20ms si systeme redressement secteur
void IRAM_ATTR currentNull() {
  IT10ms = IT10ms + 1;

  if ((millis() - lastIT) > 2) { // to avoid glitch detection during 2ms
    ITmode = ITmode + 3;
    if (ITmode > 5)
      ITmode = 5;
    IT10ms_in = IT10ms_in + 1;
    lastIT = millis();
    if (ITmode > 0)
      GestionIT_10ms(); // IT synchrone avec le secteur signal Zc toutes les 10ms
  }
}

void GestionIT_10ms() {

  for (int i = 0; i < NbActions; i++) {
    if (Gpio[i] > 0 and Gpio[i] < 99) {
      switch (BlocsActions[i].Regulation) { // valeur en RAM
      case Bloc_Action::INACTIF:            // Inactif

        break;
      case Bloc_Action::DECOUPE_SINUS: // Decoupe Sinus uniquement pour Triac
        if (i == NumBlocTriac) {
          PulseComptage[NumBlocTriac] = 0;
          if (pulseTriac > 0)
            digitalWrite(pulseTriac, LOW); // Stop Découpe Triac
        }
        break;
      case Bloc_Action::PWM: // PWM ne depend pas IT 10ms

        break;
      case Bloc_Action::DEMI_SINUS:
        Phase230V = !Phase230V;                                                                                     // Demi-Sinus
        PulseComptage[i] = PulseComptage[i] + PulseOn[i];                                                           // Augmente la phase
        if (((Phase230V && PulseTotal[i] == 0) || (!Phase230V && PulseTotal[i] == 1)) && PulseComptage[i] >= 100) { // Phase differente
          PulseTotal[i] = 0;
          if (Phase230V)
            PulseTotal[i] = 1; // Enregistrement de la phase positive ou negative du ON
          digitalWrite(Gpio[i], OutOn[i]);
          PulseComptage[i] = PulseComptage[i] - 100;
        } else {
          digitalWrite(Gpio[i], OutOff[i]); // Stop
        }
        break;
      default: // Multi Sinus ou Train de sinus
               // Gpio valide

        if (PulseComptage[i] < PulseOn[i]) {
          digitalWrite(Gpio[i], OutOn[i]);
        } else {
          digitalWrite(Gpio[i], OutOff[i]); // Stop
        }
        PulseComptage[i] = PulseComptage[i] + 1;
        if (PulseComptage[i] >= PulseTotal[i]) {
          PulseComptage[i] = 0;
        }

        break;
      }
    }
  }
}

// Interruption Timer interne toutes les 100 micro secondes
void IRAM_ATTR onTimer() { // Interruption every 100 micro second
  if (NumBlocTriac >= 0) {
    if (BlocsActions[NumBlocTriac].Regulation == Bloc_Action::DECOUPE_SINUS) {

      PulseComptage[NumBlocTriac] = PulseComptage[NumBlocTriac] + 1;
      if (PulseComptage[NumBlocTriac] > Retard[NumBlocTriac] && Retard[NumBlocTriac] < 98 && ITmode > 0) { // 100 steps in 10 ms
        if (pulseTriac > 0)
          digitalWrite(pulseTriac, HIGH); // Activate Triac
      } else {
        if (pulseTriac > 0)
          digitalWrite(pulseTriac, LOW); // Stop Triac
      }
    }
  }
}

void InitTimersIT() {
  if (pTriac > 0) {
    // Interruptions du Triac et Timer interne
    attachInterrupt(zeroCross, currentNull, RISING);

    // Hardware timer 100uS
    timer = timerBegin(1000000);
    timerAttachInterrupt(timer, &onTimer);
    // 100 ticks = 100 µs
    timerAlarm(timer, 100, true, 0);
  }
  // Hardware timer 10ms
  timer10ms = timerBegin(1000000);
  timerAttachInterrupt(timer10ms, &onTimer10ms); // Attache la fonction de callback
  timerAlarm(timer10ms, 10000, true, 0);         // Interrupt every 10ms
}