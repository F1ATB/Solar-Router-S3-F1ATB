#define VersionS3 "V1.00-beta.11"

/*
  PV Router / Routeur Photovoltaïque
  ****************************************

Les détails sont disponibles sur / Details are available here:
  https://f1atb.fr  Section Domotique / Home Automation

  F1ATB Novembre 2026

  GNU Affero General Public License (AGPL) / AGPL-3.0-or-later

*/

// Librairies
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>   //Modification On The Air
#include <PubSubClient.h> //Librairie pour la gestion Mqtt
#include "esp_system.h"
#include <ETH.h>

#include "UrlEncode.h"
#include <HardwareSerial.h>
#include <Update.h>
#include "FS.h"
#include "LittleFS.h"
#include <ArduinoJson.h>

#include "esp_system.h"
#include "esp_heap_caps.h"
#include <esp_task_wdt.h>
#include "time.h"

// Pour l'écran 3.5 pouces JC3248W535 et OLED
#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>
#include <Wire.h>

// Modules
#include "config.h"
#include "commonFX.h"
#include "Puissance.h"
#include "Historique.h"
#include "Ecrans/Ecran.h"
#include "Ecrans/LEDs.h"
#include "Temperatures/Temperature.h"
#include "RMS_Externes.h"
#include "Communications/TempoRTE.h"
#include "Communications/MQTT.h"
#include "Communications/comSortie.h"
#include "Stockage.h"
#include "Sources/Source_Linky.h"
#include "Sources/Source_UxIx2.h"
#include "Sources/Source_UxIx3.h"
#include "Sources/Source_ShellyEm_ProEm.h"
#include "Sources/Source_EnphaseEnvoy.h"
#include "Sources/Source_NotDef.h"
#include "Sources/Source_HomeWizard.h"
#include "Sources/Source_MQTT.h"
#include "Sources/Source_Externe.h"
#include "Sources/Source_UxI.h"
#include "Sources/Source_SmartG.h"
#include "Sources/Source_PZEM004T.h"
#include "BlocActions.h"
#include "OverProd.h"
#include "Cartes/carte.h"
#include "Communications/Internet.h"
#include "Communications/Server.h"
#include "Communications/WebSocket.h"
#include "Communications/OpenMeteo.h"
#include "Heure.h"
#include "Cartes/gestion_IT.h"
#include "generated/WebFiles.h"

// Watchdog de 180 secondes. Le systeme se Reset si pas de dialoque avec une source de puissance pendant 180s
// Watchdog for 180 seconds. The system resets if no dialogue with a power source for 180s
#define WDT_TIMEOUT_SECONDS 180

// Multicoeur - Processeur 0
TaskHandle_t Task1;
esp_err_t ESP32_ERROR;

int indexTest = 0;

// prototypes

void Task_LectureRMS(void *pvParameters);
void afficherCauseReset();

void setup()
{
  startMillis = millis();
  Version = VersionS3;
  Serial.begin(115200);
  Serial0.begin(115200, SERIAL_8N1, 44, 43);
  delay(3000);
  InitMessages();
  while (!Serial && millis() < 5000)
    Serial.println("Booting Routeur F1ATB (Sortie USB/CDC)");
  Serial0.println("Booting Routeur F1ATB (Sortie port Serie 43 ,44)");
  afficherCauseReset();
  // Initialiser LittleFS
  if (!LittleFS.begin(true))
  { // `true` = format si erreur
    EcranPrintln("Erreur de montage fichiers LittleFS");
  }
  else
  {
    EcranPrintln("Système fichiers LittleFS monté");
  }
  delay(1000);
  // Initialisation
  if (psramInit())
  {
    EcranPrintln("PSRAM  correctement initialisée");
  }
  else
  {
    EcranPrintln("La PSRAM ne fonctionne pas");
  }
  delay(100);
  Init_Machines();      // Init table des Machines du réseau
  Init_ListeRouteurs(); // Init table des autre routeurs
  Init_Blocs();         // Création table des Blocs des Actions
  Init_OverProd();      // Initialisation de la gestion de l'overproduction associé aux actions
  Init_Historique();
  Init_Puissance();
  Init_Temperature();
  EcranPrintln("Initialisation avant configuration terminée");

  LitConfig();
  EcranPrintln("Configuration lue");

  Init_GPIOs();
  Init_LED_OLED();
  Init_Ecran(); // A faire immediatement après LitConfig. Cas écriture écran

  EcranPrintln("Booting ESP32S3 " + Modele_Carte());
  Ecran_Loop();

  //=========== Watchdog initialisation ==========
  esp_task_wdt_deinit();
  esp_task_wdt_config_t wdt_cfg = {
      .timeout_ms = WDT_TIMEOUT_SECONDS * 1000UL,
      .idle_core_mask = (1 << portNUM_PROCESSORS) - 1, // Bitmask of all cores
      .trigger_panic = true,
  };
  esp_task_wdt_init(&wdt_cfg); // add current thread to WDT watch
  // Abonner la tâche Arduino loop() au watchdog
  esp_task_wdt_add(NULL);
  ResetWatchDog();

  Ecran_Loop();

  Ecran_Loop();
  Init_Internet();
  Ecran_Loop();

  // Serveur Web
  Init_Server();
  EcranPrintln("Serveur Web actif sur le port 80");
  // Lancer serveur Telnet
  Init_TelnetServer();

  Ecran_Loop();
  NomEtatLocal();
  Liste_des_Noms(); // Nom des autres routeurs du réseau
  // Modification du programme par le Wifi  - OTA(On The Air)
  //***************************************************
  ArduinoOTA.setHostname((const char *)hostname.c_str());
  ArduinoOTA.begin(); // Mandatory

  // Adaptation à la Source
  for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
  {
    EcranPrintln("Source " + String(source) + " : " + LaSource[source].Type);
    if (LaSource[source].Type == "UxI")
    {
      Setup_UxI();
    }

    if (LaSource[source].Type == "Enphase")
    {
      Setup_Enphase(false, source);
    }

    if (LaSource[source].Type == "Pmqtt")
    {
      GestionMQTT();
    }

    // Port Série si besoin
    if (pSerial1 > 0 || pSerial2 > 0) // METTTRE TIMEOUT SI INIT NE SE FAIT PAS
    {

      if (LaSource[source].Type == "UxIx2")
      {
        Setup_UxIx2(source);
      }
      if (LaSource[source].Type == "UxIx3")
      {
        Setup_JSY333(source);     // init port série
        delay(100);               // pour s'assurer que l'init du port série est ok coté module
        PeriodeProgMillis[source] = 1000; // la première lecture aura lieu 1000ms plus tard
        Requete_JSY333(source);   // requête initiale au module. La première lecture aura lieu PeriodeProgMillis =1000ms plus tard.
                                  // et les données seront déjà toutes dans le buffer de réception
      }

      if (LaSource[source].Type == "Linky")
      {
        Setup_Linky(source);
      }
      if (LaSource[source].Type == "PZEM004T")
      {
        Setup_PZEM004T(source);
      }
    }

    if (LaSource[source].Type == "Ext")
    {
      LaSource[source].Data = "NotDef";
    }
    else
    {
      LaSource[source].Data = LaSource[source].Type;
    }
  }

  Ecran_Loop();
  xTaskCreatePinnedToCore( // Préparation Tâche Multi Coeur
      Task_LectureRMS,     /* Task function. */
      "Task_LectureRMS",   /* name of task. */
      10000,               /* Stack size of task */
      NULL,                /* parameter of the task */
      10,                  /* priority of the task */
      &Task1,              /* Task handle to keep track of created task */
      0);                  /* pin task to core 0 */

  InitTimersIT(); // Initialisation des timers et interruptions logiciels
  EcranPrintln("Démarrage des Timers IT : " + String(millis() - startMillis) + " ms");
  // Timers
  previousStatusMillis = millis();
  previousDonneesLentesMillis = millis() - 200000;
  previousTimer2sMillis = millis();
  previousLoop = millis();
  previousTimeRMS = millis();
  previousMQTTenvoiMillis = millis();
  previousMQTTMillis = millis();

  previousOverProdMillis = millis();

  previousActionMillis = millis();
  previousTempMillis = millis() - 110000;
  previousRevueBlocMillis = millis();
  previousDonneesActionsMillis = millis();
  if (Nbr_DS18B20 > 0)
    LectureTemperature();
  for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
  {
    LastRMS_Millis[source] = millis();
  }
  if (ESP32_Type == CARTE_ECRAN)
  {
    PageActu = 0;
    PageDelta = 480;
  }
  ResetWatchDog();
  T_On_seconde = millis() / 1000;
}

/* **********************
 * ****************** *
 * * Tâches Coeur 0 * *
 * ****************** *
 **********************
 */
void Task_LectureRMS(void *pvParameters)
{

  for (;;)
  {
    unsigned long tps = millis();
    float deltaT = float(tps - previousTimeRMS);
    previousTimeRMS = tps;
    previousTimeRMSMin = min(previousTimeRMSMin, deltaT);
    previousTimeRMSMin = previousTimeRMSMin + 0.002;
    previousTimeRMSMax = max(previousTimeRMSMax, deltaT);
    previousTimeRMSMax = previousTimeRMSMax * 0.999;
    previousTimeRMSMoy = deltaT * 0.01 + previousTimeRMSMoy * 0.99;
    previousTimeRMSMin = min(previousTimeRMSMin, previousTimeRMSMoy);
    previousTimeRMSMax = max(previousTimeRMSMax, previousTimeRMSMoy);

    // Recupération des données RMS
    //******************************

    for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
    {
      tps = millis();
      if (tps - LastRMS_Millis[source] > PeriodeProgMillis[source])
      { // Attention delicat pour eviter pb overflow
        LastRMS_Millis[source] = tps;
        if (LaSource[source].Type == "NotDef")
        {
          LectureNotDef(source);
          PeriodeProgMillis[source] = 600;
        }

        if (LaSource[source].Type == "UxI")
        {
          LectureUxI(source);
          PeriodeProgMillis[source] = 40;
        }
        if (pSerial1 > 0 || pSerial2 > 0)
        {

          if (LaSource[source].Type == "UxIx2")
          {
            LectureUxIx2(source);
            PeriodeProgMillis[source] = 400;
          }

          if (LaSource[source].Type == "UxIx3")
          {
            Lecture_JSY333(source);
            PeriodeProgMillis[source] = 1000;
          }
          if (LaSource[source].Type == "Linky")
          {
            LectureLinky(source);
            PeriodeProgMillis[source] = 2;
          }
          if (LaSource[source].Type == "PZEM004T")
          {
            LecturePZEM004T(source);
            PeriodeProgMillis[source] = 1000; //  On peut interroger lentement le PZEM004T pour avoir une idée de la consommation d'un appareil ou d'une ligne, mais pas pour faire de la régulation.
          }
        }
        if (LaSource[source].Type == "Enphase")
        {
          uint32_t nReadingDuration = LectureEnphase(source);
          if (nReadingDuration > ENPHASE_READING_PERIOD) // si la lecture de l'Enphase a durée plus de temps que la période de lecture //On s'adapte à la vitesse réponse Envoy-S metered
            PeriodeProgMillis[source] = nReadingDuration + 500;
          else
            PeriodeProgMillis[source] = ENPHASE_READING_PERIOD;
        }

        if (LaSource[source].Type == "SmartG")
        {
          LectureSmartG(source);
          LastRMS_Millis[source] = millis();
          PeriodeProgMillis[source] = 200; // On s'adapte à la vitesse réponse SmartGateways
        }
        if (LaSource[source].Type == "HomeW")
        {
          LectureHomeW(source);
          LastRMS_Millis[source] = millis();
          PeriodeProgMillis[source] = 200; // On s'adapte à la vitesse réponse HomeWizard
        }
        if (LaSource[source].Type == "ShellyEm")
        {
          if (LaSource[source].CanalVoie[0] >= 0)
            LectureShellyEm(source, 0); // 0,1,30,31
          delay(1);
          if (LaSource[source].CanalVoie[1] >= 0 )
            LectureShellyEm(source, 1);
          LastRMS_Millis[source] = millis();
          PeriodeProgMillis[source] = 150; // On s'adapte à la vitesse réponse ShellyEm.
        }
        if (LaSource[source].Type == "ShellyPro")
        {
          if (LaSource[source].CanalVoie[0] >= 0) // 0,1,30,31
            LectureShellyProEm(source, 0);
          delay(1);
          if (LaSource[source].CanalVoie[1] >= 0)
            LectureShellyProEm(source, 1);
          LastRMS_Millis[source] = millis();
          PeriodeProgMillis[source] = 100; // On adapte à la vitesse réponse ShellyProEm
        }

        if (LaSource[source].Type == "Ext")
        {
          CallESP32_Externe(source);
          LastRMS_Millis[source] = millis();
          PeriodeProgMillis[source] = 100; // Après pour ne pas surchargé Wifi
        }

        if (LaSource[source].Type == "Pmqtt")
        {
          PeriodeProgMillis[source] = 600;
          LastRMS_Millis[source] = millis();
          UpdatePmqtt(source);
        }
      }
    }

    // Client Websocket pour les routeurs externes
    for (int i = 0; i < LES_ROUTEURS_MAX; i++)
    {
      if (RouteurComExt[i].IP > 0)
      {
        RouteurComExt[i].webSocket.loop();
      }
    }

    delay(2);
  }
}

/* **********************
 * ****************** *
 * * Tâches Coeur 1 * *
 * ****************** *
 **********************
 */

void loop()
{

  // Estimation charge coeur
  unsigned long tps = millis();
  float deltaT = float(tps - previousLoop);
  previousLoop = tps;
  previousLoopMin = min(previousLoopMin, deltaT);
  previousLoopMin = previousLoopMin + 0.002;
  previousLoopMax = max(previousLoopMax, deltaT);
  previousLoopMax = previousLoopMax * 0.999;
  previousLoopMoy = deltaT * 0.01 + previousLoopMoy * 0.99;
  previousLoopMin = min(previousLoopMin, previousLoopMoy);
  previousLoopMax = max(previousLoopMax, previousLoopMoy);

  // WebSocket
  ws.cleanupClients();
  wsAct.cleanupClients();
  wsBrute.cleanupClients();

  // Gestion du serveur OTA
  //********************
  ArduinoOTA.handle();
  GestionTelnet();

  // Archivage et envois des mesures périodiquement
  //**********************************************
  if (tps - previousRevueBlocMillis >= 1000)
  { // Revue des blocs toutes les secondes
    previousRevueBlocMillis = tps;
    RevueBlocsActions();
    envoyerPuissanceSources(); // Websocket pour les ESP32-S3
  }
  if (tps - previousTimer2sMillis >= 2000)
  {
    unsigned long dt = 2000;
    previousTimer2sMillis += 2000; // Pour avoir un intervalle régulier de 2 secondes
    T_On_seconde = T_On_seconde + 2;
    JourHeureChange();
    if (HeureValide && !LastRecordData)
      LitLastRecord_Data();
    if (EnergieActiveValide[0])
    {
    }
    SauvegardeHistorique();

    H_Ouvre_Equivalent(dt);
    // envois par WebSocket
  }

  //??if (EnergieActiveValide[0])
  //????{
  if (tps - previousDonneesActionsMillis >= DONNEES_ACTIONS_INTERVALLE)
  {
    previousDonneesActionsMillis = tps;
    envoyerSortiesBlocs(); // Envoi des sorties des blocs actions périodiquement
  }
  if (tps - previousOverProdMillis >= OVER_PRODUCTION_INTERVALLE)
  {
    previousOverProdMillis = tps;
    GestionOverproduction();
  }
  if (tps - previousDonneesLentesMillis >= DONNEES_LENTES_INTERVALLE)
  { // Envoi données lentes , temperature, ensoleillement
    envoyerDonneesLentes();
    envoyerBruteMessages();
    envoyerBrutesESP32();
    envoyerBruteSources();
    previousDonneesLentesMillis = tps;
  }
  if (tps - previousMQTTMillis > PeriodeMQTTMillis)
  {

    previousMQTTMillis = tps;
    GestionMQTT();
  }
  if (tps - previousLEDsMillis >= 50)
  {
    previousLEDsMillis = tps;
    Gestion_LEDs();
  }
  // Actions forcées et température
  if (tps - previousActionMillis > 60000)
  {
    previousActionMillis += 60000;
    GestionForcageActions();
    // Discovery message pour MQTT (if HA restart)
    MQTT_Discovered = false; // ???? A decaler toutes les 5mn
  }
  if (tps - previousTempMillis > 15001)
  {
    previousTempMillis = tps;
    // Temperature
    LectureTemperature();
    // Rafraichissement des noms et des états des actions externes
    RefreshNomsEtats();
    InfoActionExterne();
  }
  // Vérification du WIFI et de la puissance etc
  //********************

  if (tps - previousStatusMillis > 3000)
  { // Test périodique different toutes les 3s, cycle de 30s (indexTest de 0 à 9)

    previousStatusMillis = tps;
    envoyerBruteSources();

    // Lot de tests périodiques
    indexTest = (indexTest + 1) % 10; // 1 test ou mise à jour toute les 3s
    float DureeOn;
    String OK;
    int Ltarf;
    switch (indexTest)
    {
    case 0:
      DureeOn = float(T_On_seconde) / 3600.0;
      EcranPrintln("ESP32 ON depuis : " + String(DureeOn) + " heures");
      EcranPrintln("✅ Test boucle contrôle ESP32 OK");
      envoyerBrutesESP32();
      break;
    case 1: // Verification puissance reçue
      OK = "Non ⚠️";
      if (PuissanceRecue)
      {
        OK = "Oui ✅";
        PuissanceValide = 4;
        PuissanceRecue = false;
        ResetWatchDog();
      }
      EcranPrintln("Puissance reçue source 0 : " + OK);
      if (PuissanceValide > 0)
      {
        PuissanceValide = PuissanceValide - 1;
      }
      else
      {
        delay(500);
        ESP.restart();
      }
      RafraichissementsBlocsCapteurs();
      break;
    case 2:
      if (pSerial1 == 0 && pSerial2 == 0 && (Source == "UxIx2" || Source == "UxIx3" || Source == "Linky" || Source == "PZEM004T"))
        StockMessage("Attention, port série non défini dans les paramètres !");
      envoyerBrutesESP32();
      break;
    case 3:
      BilanMemoire();
      EcranPrintln("Mém RAM dispo : " + String(Mem_SRAM[1]) + " Bytes");
      break;
    case 4:
      JourHeureChange();
      EcranPrintln("Date : " + DATE);
      // RTE
      if (ModeReseau == 0)
      { // Valable pour Ethernet également
        Call_RTE_data();
        Ltarf = 0; // Code binaire Tarif
        if (LTARF.indexOf("PLEINE") >= 0)
          Ltarf += 1;
        if (LTARF.indexOf("CREUSE") >= 0)
          Ltarf += 2;
        if (LTARF.indexOf("BLEU") >= 0)
          Ltarf += 4;
        if (LTARF.indexOf("BLANC") >= 0)
          Ltarf += 8;
        if (LTARF.indexOf("ROUGE") >= 0)
          Ltarf += 16;
        LTARFbin = Ltarf;
        if (LTARF != "")
          EcranPrintln(LTARF);
      }
      break;
    case 5:
      if (ESP32_Type == CARTE_INCONNU)
        StockMessage("Attention, modèle carte ESP32-S3 non défini dans les paramètres !");
      envoyerBrutesESP32();
      break;
    case 6:
      if (ESP32_Type < CARTE_ETHERNET)
      { // ESP32 en WIFI
        if (WiFi.getMode() == WIFI_STA || WiFi.getMode() == WIFI_AP_STA)
        {
          if (WiFi.waitForConnectResult(10000) != WL_CONNECTED)
          {
            StockMessage("WIFI Connection Failed! #" + String(WIFIbug));
            WIFIbug++;
          }
          else
          {
            WIFIbug = 0;
          }
          if (WiFi.getMode() == WIFI_STA)
          {
            EcranPrint("Niveau Signal WIFI :");
            EcranPrintln(String(WiFi.RSSI()) + " dBm");
            EcranPrint("IP address : ");
            EcranPrintln(WiFi.localIP().toString());
          }
          else
          {
            infoSerie();
          }
          EcranPrint("WIFIbug : #");
          EcranPrintln(String(WIFIbug));
          if (WIFIbug > ComSurv)
          { // Timeout sans WIFI Reset
            EcranPrintln("Redémarrage pour problème WIFI...");
            delay(5000);
            ESP.restart();
          }
        }
        else
        {
          if (ModeReseau < 2)
          { // Normalement connecté au réseau
            WIFIbug++;
            if (WIFIbug > ComSurv)
            { // Timeout en mode AP Reset
              ESP.restart();
            }
          }
          EcranPrintln("Access Point Mode : " + hostname);
          EcranPrint("IP address : ");
          EcranPrintln(WiFi.softAPIP().toString());
          EcranPrintln("Par le port série vous pouvez définir le WIFI à utiliser par l'ESP32 en tapant les 3 commandes ci-dessous en remplaçant xxx par la bonne valeur :");
          EcranPrintln("ssid:xxx");
          EcranPrintln("password:xxx");
          EcranPrintln("restart");
        }
      }
      else
      {

        EcranPrintln("IP :" + ETH.localIP().toString());

        // if (Ethernet.linkStatus() == LinkOFF) {
        //    EcranPrintln("Câble Ethernet non connecté.");
        //    EthernetBug++;
        //  } else {
        //    EthernetBug = 0;
        //  }
        //  if (EthernetBug > ComSurv) {  // TimeOut sans réseau
        //    ESP.restart();
        //  }
      }
      RafraichissementsBlocsCapteurs();
      break;
    case 7:
      // Test pulse Zc Triac
      if (ITmode < 0 && pTriac > 0)
      {
        if (!erreurTriac)
          StockMessage("Erreur : pas de signal Zc du gradateur/Triac"); // Pour ne pas répéter sans cesse
        erreurTriac = true;
      }
      else
      {
        erreurTriac = false;
      }
      break;

    case 8:
      recupererEnsoleillement();
      envoyerBrutesESP32();
      break;
    case 9:
      RafraichissementsBlocsCapteurs();

      break;
    }
  }

  Ecran_Loop();
  LireSerial();

  delay(1);
}

// ========== Bilan Dernier Reset  ==========

void afficherCauseReset()
{
  esp_reset_reason_t reason = esp_reset_reason();

  String Message = "Cause du dernier reset : ";
  switch (reason)
  {
  case ESP_RST_POWERON:
    Message += "Mise sous tension";
    break;

  case ESP_RST_EXT:
    Message += "Reset externe";
    break;
  case ESP_RST_SW:
    Message += "Reset logiciel";
    break;

  case ESP_RST_PANIC:
    Message += "Exception / PANIC";
    break;

  case ESP_RST_INT_WDT:
    Message += "Watchdog interruption";
    break;

  case ESP_RST_TASK_WDT:
    Message += "Watchdog tâche";
    break;

  case ESP_RST_WDT:
    Message += "Watchdog";
    break;

  case ESP_RST_BROWNOUT:
    Message += "Brownout (tension trop faible)";
    break;

  case ESP_RST_DEEPSLEEP:
    Message += "Réveil après Deep Sleep";
    break;

  default:
    Message += "Inconnue (" + String(reason) + ")";
    break;
  }
  StockMessage(Message);
}