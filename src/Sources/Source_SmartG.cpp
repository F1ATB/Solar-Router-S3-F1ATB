// ******************************
// * Client d'un Smart Gateways *
// ******************************

#include "Sources/Source_SmartG.h"
#include <Arduino.h>
#include "config.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "commonFX.h"
#include "Communications/comSortie.h"
#include <WiFi.h>
#include "Communications/Internet.h"



void LectureSmartG(int8_t source) {
  String S = "";
  String SmartG_Data = "";
  String Gr[4];
  String data_[20];
  int8_t C0=source * 3; //Canal de mesure
  PVA_Valide[C0] = false; // ??????????? A FAIRE dans les IHM, retirer si nécessaire l'affichage des VA
  // Use WiFiClient class to create TCP connections
  WiFiClient clientESP_RMS;
  String host = IP2String(LaSource[source].IP);
  if (!clientESP_RMS.connect(host.c_str(), 82, 3000)) {  // PORT 82 pour Smlart Gateways
    clientESP_RMS.stop();
    delay(500);
    if (!clientESP_RMS.connect(host.c_str(), 82, 3000)) {
      StockMessage("connection to SmartGateways failed : " + host);
      delay(100);
      return;
    }
  }
  String url = "/smartmeter/api/read";
  clientESP_RMS.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (clientESP_RMS.available() == 0) {
    if (millis() - timeout > 5000) {
      StockMessage(">>> client SmartGateways Timeout ! : " + host);
      clientESP_RMS.stop();
      return;
    }
  }
  timeout = millis();
  // Lecture des données brutes distantes
  while (clientESP_RMS.available() && (millis() - timeout < 5000)) {
    SmartG_Data += clientESP_RMS.readStringUntil('\r');
  }
  int p = SmartG_Data.indexOf("{");
  SmartG_Data = SmartG_Data.substring(p + 1);
  p = SmartG_Data.indexOf("}");
  SmartG_Data = SmartG_Data.substring(0, p);
  
  PuissanceS_inst[0] = PfloatMax(ValJsonSG("PowerDelivered_total", SmartG_Data));
  PuissanceI_inst[0] = PfloatMax(ValJsonSG("PowerReturned_total", SmartG_Data));
  long EnergyDeliveredTariff1 = int(1000 * ValJsonSG("EnergyDeliveredTariff1", SmartG_Data));
  long EnergyDeliveredTariff2 = int(1000 * ValJsonSG("EnergyDeliveredTariff2", SmartG_Data));
  Energie_Soutiree[0] = EnergyDeliveredTariff1 + EnergyDeliveredTariff2;
  long EnergyReturnedTariff1 = int(1000 * ValJsonSG("EnergyReturnedTariff1", SmartG_Data));
  long EnergyReturnedTariff2 = int(1000 * ValJsonSG("EnergyReturnedTariff2", SmartG_Data));
  Energie_Injectee[0] = EnergyReturnedTariff1 + EnergyReturnedTariff2;
  LaSourceVar[source].DataBrute = SmartG_Data;
  Filtre_Puissance(true,C0);
  AffectationPuissanceAuxActions(C0);
  Puissance_Max(C0);
  CalculEnergieJour(C0, true);
  EnergieActiveValide[C0+1] = false;
  EnergieActiveValide[C0+2] = false;
}


