#include "Communications/OpenMeteo.h"
#include "Communications/comSortie.h"
#include "Heure.h"
#include "commonFX.h"
#include "config.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

// =========================================================
//  RÉCUPÉRATION DE L'ENSOLEILLEMENT (GTI) VIA OPEN-METEO (exemple Lolo69)
// =========================================================

// --- 1. L'URL DE L'API ---------------------------------------------------
// On interroge Open-Meteo (gratuit, sans clé). Le paramètre clé est
// "global_tilted_irradiance" placé dans le bloc "hourly" : c'est lui qui
// renvoie l'ensoleillement (W/m²) heure par heure, pour un panneau incliné.
// forecast_days=2 => on récupère aujourd'hui ET demain.

int ExtraiHeureSun(const String &timeStr);

void recupererEnsoleillement() {

  if (WiFi.status() != WL_CONNECTED || (latitude == 0.0 && longitude == 0.0) || ModePara == 0 || HeureValide == false) {
    SoleilDemain = 0;
    SoleilActuel = 0;
    SoleilJour = 0;
    HnextSoleil = 0;
    return;
  }
  SoleilActuel = Wm2[Int_Heure]; // Valeur à l'heure courante
  if (HnextSoleil <= Int_Heure && Int_Heure < 20) {
    HnextSoleil = Int_Heure + 4; // Retour dans 4h
    if (HnextSoleil >= 20) {
      HnextSoleil = 3;
    }
    SeriePrintln("Open-Meteo : Appel");
    String METEO_URL = "http://api.open-meteo.com/v1/forecast";
    METEO_URL += "?latitude=" + String(latitude) + "&longitude=" + String(longitude);
    METEO_URL += "&current_weather=true&hourly=global_tilted_irradiance&timezone=Europe%2FParis&forecast_days=2";
    METEO_URL += "&daily=sunrise&daily=sunset";

    // --- 2. REQUÊTE HTTP -------------------------------------------------

    WiFiClient client;
    HTTPClient http;
    http.setTimeout(5000);
    client.setTimeout(5);
    http.begin(client, METEO_URL.c_str());
    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) { // 200 attendu
      http.end();
      return;
    }
    String payload = http.getString(); // le JSON renvoyé par l'API
    http.end();

    // --- 3. PARSING DU JSON ----------------------------------------------

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      SeriePrint("Erreur de parsing OpenMeteo: ");
      SeriePrintln(error.c_str());
      return;
    }

    // L'API renvoie deux tableaux parallèles dans "hourly" :
    //   "time" : ["2026-06-23T00:00", "2026-06-23T01:00", ...]
    //   "global_tilted_irradiance" : [0, 0, ..., 850, 920, ...]
    // JsonArray hourlyTime = doc["hourly"]["time"];
    JsonArray hourlyGti = doc["hourly"]["global_tilted_irradiance"];
    for (int h = 0; h < 48; h++) {
      if (h < (int)hourlyGti.size()) {
        Wm2[h] = int(hourlyGti[h].as<float>());
      } else {
        Wm2[h] = 0;
      }
    }

    int gti = 0;
    int n = 0;
    for (int h = 0; h < 24; h++) {
      if (Wm2[h] > 0) {
        gti += Wm2[h];
        n++;
      }
    }
    if (n > 0)
      gti = gti / n;
    SoleilJour = gti;
    gti = 0;
    n = 0;
    for (int h = 24; h < 48; h++) {
      if (Wm2[h] > 0) {
        gti += Wm2[h];
        n++;
      }
    }
    if (n > 0)
      gti = gti / n;
    SoleilDemain = gti;
    SoleilActuel = Wm2[Int_Heure];

    // --- 4. RÉSULTAT ENSOLEILLEMENT-----------------------------------------------------
    StockMessage("Ensoleillement actuel = " + String(SoleilActuel) + " W/m² , Moyen J = " + String(SoleilJour) +
                 " W/m² , Moyen J+1 = " + String(SoleilDemain) + " W/m²");

    // SUNRISE SUNSET
    JsonArray sunRise = doc["daily"]["sunrise"];
    JsonArray sunSet = doc["daily"]["sunset"];
    SunRiseJour = ExtraiHeureSun(sunRise[0].as<String>());
    SunSetJour = ExtraiHeureSun(sunSet[0].as<String>());
    SunRiseDemain = ExtraiHeureSun(sunRise[1].as<String>());
    SunSetDemain = ExtraiHeureSun(sunSet[1].as<String>());
    StockMessage("Lever du soleil aujourd'hui = " + String(SunRiseJour) + " , Coucher du soleil aujourd'hui = " + String(SunSetJour) +
                 " , Lever du soleil demain = " + String(SunRiseDemain) + " , Coucher du soleil demain = " + String(SunSetDemain));
  }
}
JsonDocument JsonSoleil() {
  JsonDocument SoleilDoc;
  SoleilDoc["Int_Heure"] = Int_Heure;
  SoleilDoc["SoleilJour"] = SoleilJour;
  SoleilDoc["SoleilDemain"] = SoleilDemain;
  SoleilDoc["SunRiseJour"] = SunRiseJour;
  SoleilDoc["SunSetJour"] = SunSetJour;
  SoleilDoc["SunRiseDemain"] = SunRiseDemain;
  SoleilDoc["SunSetDemain"] = SunSetDemain;

  for (int h = 0; h < 48; h++) {
    SoleilDoc["Wm2"][h] = Wm2[h];
  }
  return SoleilDoc;
}
int ExtraiHeureSun(const String &timeStr) {
  // timeStr format: "2026-06-23T00:00"
  int hourMn = 0;
  if (timeStr.length() >= 13) {
    hourMn = 100 * timeStr.substring(11, 13).toInt() + timeStr.substring(14, 16).toInt();
  }
  return hourMn;
}
