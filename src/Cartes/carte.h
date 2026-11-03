#pragma once
#include <Arduino.h>


enum CarteType {    
    CARTE_INCONNU = 0,
    CARTE_WROOMS3= 1,
    CARTE_ECRAN = 2,
    CARTE_ETHERNET = 10
};
extern CarteType ESP32_Type;  
extern CarteType ESP32_New_Type;

//GPIOs pour le module W5500 de la carte T-ET-LITE ESP32-S3
#define ETH_MISO_PIN                    11
#define ETH_MOSI_PIN                    12
#define ETH_SCLK_PIN                    10
#define ETH_CS_PIN                      9
#define ETH_INT_PIN                     13
#define ETH_RST_PIN                     14
#define ETH_ADDR                        1
#define SD_MISO_PIN                     5
#define SD_MOSI_PIN                     6
#define SD_SCLK_PIN                     7
#define SD_CS_PIN                       42




String Modele_Carte();
void ResetWatchDog();
void Init_GPIOs();
String ListeDesGPIOs(CarteType type);
