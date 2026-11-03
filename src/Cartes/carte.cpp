#include "Cartes/carte.h"
#include "BlocActions.h"
#include "Communications/comSortie.h"
#include "Ecrans/Ecran.h"
#include "Sources/Source_UxI.h"
#include "Temperatures/Temperature.h"
#include "config.h"
#include <Arduino.h>
#include <esp_task_wdt.h>

CarteType ESP32_Type = CARTE_INCONNU; // 0=Inconnu,1=Wroom S3 seul,2=Ecran 480*320,10=Ethernet T-ETH
CarteType ESP32_New_Type = CARTE_INCONNU;

String Modele_Carte() {
  String ESP32_T[] = {"Non Défini", "Wroom seul", "Ecran 480*320 pixels", "", "", "", "", "", "", "", "Ethernet T-ETH"};
  return ESP32_T[(int)ESP32_Type];
}
void ResetWatchDog() {
  esp_task_wdt_reset();
  delay(1); // VERY VERY IMPORTANT for Watchdog Reset
}

void Init_GPIOs() {
  EcranPrintln("Initialisation des GPIOs pour la carte " + Modele_Carte());
  if (ESP32_Type > CARTE_INCONNU) {
    for (int i = 0; i < NbActions; i++) {

      EcranPrintln("Initialisation GPIO pour l'action " + String(i) + " sur la pin " + String(BlocsActions[i].Gpio));
      BlocsActions[i].InitGpio(FreqPWM);
      Gpio[i] = BlocsActions[i].Gpio;
      OutOn[i] = BlocsActions[i].OutOn;
      OutOff[i] = BlocsActions[i].OutOff;
      EcranPrintln("GPIO:" + String(Gpio[i]) + " OutOn:" + String(OutOn[i]) + " OutOff:" + String(OutOff[i]));
    }
  }
  // Triac init
  if (pTriac > 0) {
    pulseTriac = PulseT[pTriac];
    zeroCross = ZeroT[pTriac];
    pinMode(zeroCross, INPUT_PULLUP);
    pinMode(pulseTriac, OUTPUT);
    digitalWrite(pulseTriac, LOW); // Stop Triac
  }

  // LEDs traités à part
  InitGpioSerial();

  InitGpioTemperature();

  InitGpioAnalogique();
}

String ListeDesGPIOs(CarteType type) {
  String ListeGPIO = "0";
  if (type == CARTE_WROOMS3)
    ListeGPIO = "4,5,6,7,8,9,10,11,12,13,14,21"; // Wroom seul
  if (type == CARTE_ECRAN)
    ListeGPIO = "5,6,7,9,14,15,16"; // Ecran 480*320
  if (type == CARTE_ETHERNET)
    ListeGPIO = "4,5,6,7,8,15,16,20,21"; // Ethernet
  return ListeGPIO;
}