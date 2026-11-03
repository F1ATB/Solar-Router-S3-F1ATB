// ***************
// *  WEB SERVER *
// ***************
#include "config.h"
#include "Communications/Server.h"
#include "Communications/WebSocket.h"
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "Cartes/carte.h"

#include "LittleFS.h"
#include "BlocActions.h"
#include "commonFX.h"
#include "Temperatures/Temperature.h"
#include "Communications/comSortie.h"
#include "Puissance.h"
#include "Historique.h"
#include "Ecrans/Ecran.h"
#include "Ecrans/LEDs.h"
#include "Communications/MQTT.h"
#include "Communications/Internet.h"
#include "Heure.h"
#include "RMS_Externes.h"
#include "Sources/Source_Linky.h"
#include "Sources/Source_EnphaseEnvoy.h"
#include "Sources/Source_Externe.h"
#include "Sources/Source_HomeWizard.h"
#include "Sources/Source_MQTT.h"
#include "Sources/Source_NotDef.h"
#include "Sources/Source_ShellyEm_ProEm.h"
#include "Sources/Source_SmartG.h"
#include "Sources/Source_UxI.h"
#include "Sources/Source_UxIx2.h"
#include "Sources/Source_UxIx3.h"
#include "Stockage.h"
#include "Communications/TempoRTE.h"
#include "Communications/OpenMeteo.h"
#include "generated/WebFiles.h" //Pages web incluses automatiquement par tools/embed_web.py

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h> //Modification On The Air

// Serveur Web
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncWebSocket wsAct("/wsAct");
AsyncWebSocket wsBrute("/wsBrute");
static const char *PARAM_MESSAGE = "message";

String postData = "";

// Prototypes
void notFound(AsyncWebServerRequest *request);
void handleDoUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);

void lectureCookie(AsyncWebServerRequest *request, const String &S);

void Init_Server()
{
  // WebSocket page principale
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  // WebSocket page Actions
  wsAct.onEvent(onWsEventAct);
  server.addHandler(&wsAct);
  // WebSocket page Brute
  wsBrute.onEvent(onWsEventBrute);
  server.addHandler(&wsBrute);
  // Main Page
  //*********
  DefaultHeaders::Instance().addHeader(
      "Access-Control-Allow-Origin", "*"); // Add CORS header
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", pageHtmlHome); });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "image/svg+xml", Favicon); });
  server.on("/ConfigAjax", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "application/json", SerializeConfiguration()); }); // On force envoi derriere
  server.on("/ajax_histo10mn", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/json", SerialHistorique10mn()); });
  server.on("/ajax_histo48h", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/json", SerialHistorique48h()); });

  server.on("/ajax_histoEnergie", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/json", HistoriqueEnergie()); });
  server.on("/AppelDonneesImmediat", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send(200, "text/html", "OK Immediat");
              envoyerPuissanceSources();
              envoyerDonneesLentes();
              envoyerBruteMessages();
              envoyerBrutesESP32();
              envoyerBruteSources(); }); // Envoi par websocket dans 0.3s
  server.on("/ajax_data", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    
    String S = LesTemperatures();
    S = "Deb" + RS + HeureCourante() + RS + LaSource[0].Data + RS + LTARF + RS + STGEt + RS + S + RS + String(PVA_Valide[0]);
    S += GS + String(PuissanceS[0]) + RS + String(PuissanceI[0]) + RS + String(PVAS[0]) + RS + String(PVAI[0]);
    S += RS + String(EnergieJour_Soutiree[0]) + RS + String(EnergieJour_Injectee[0]) + RS + String(Energie_Soutiree[0]) + RS + String(Energie_Injectee[0]);
    if (LaSource[0].Data == "UxIx2" || ((LaSource[0].Data == "ShellyEm" || LaSource[0].Data == "ShellyPro") && LaSource[0].CanalVoie[1]!= 3)) {  //UxIx2 ou Shelly monophasé avec 2 sondes
      S += GS + String(PuissanceS[1]) + RS + String(PuissanceI[1]) + RS + String(PVAS[1]) + RS + String(PVAI[1]);
      S += RS + String(EnergieJour_Soutiree[1]) + RS + String(EnergieJour_Injectee[1]) + RS + String(Energie_Soutiree[1]) + RS + String(Energie_Injectee[1]);
    } else {
    S +=GS +"Fake";
  }
    // --- NOUVEAU GROUPE --- MC gestion du triphasé
	S += GS +FS
    + String(Tension[0]) + RS + String(Intensite[0])
    + RS + String(Tension[0]) + RS + String(Intensite[0])
    + RS + String(Tension[1]) + RS + String(Intensite[1])
    + RS + String(Tension[2]) + RS + String(Intensite[2]); //Compatibilité avec routeur classique à verifier
	if (Source_data == "UxIx2" || ((Source_data == "ShellyEm" || Source_data == "ShellyPro") && EnphaseSerial.toInt() != 3)) {  // UxIx2 ou Shelly monophasé avec 2 sondes
    S += RS + String(Tension[1]) + RS + String(Intensite[1]) ;
	}
// --- FIN ---
  S += FS + "Fin\r"; 
    request->send(200, "text/html", S); });

  server.on("/ajax_forceAction", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    int Force = 0;
    int NumAction = 0;
    if (request->hasParam("Force")) {
      Force = request->getParam("Force")->value().toInt();
    }
    if (request->hasParam("NumAction")) {
      NumAction = request->getParam("NumAction")->value().toInt();
    }
    forceAction(NumAction, Force);
    request->send(200, "text/html", "ok force"); });

  // Home
  server.on("/HomeCSS.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/css", HomeCSS); });
  server.on("/JS_Home/JS_carteGraphEnergie.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_carteGraphEnergie); });
  server.on("/JS_Home/JS_Home.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_Home); });
  server.on("/JS_Home/JS_carteGraphEnsoleillement.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_carteGraphEnsoleillement); });
  server.on("/JS_Home/JS_carteGraphHistorique.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_carteGraphHistorique); });
  server.on("/JS_Home/JS_carteJauge.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_carteJauge); });
  server.on("/JS_Home/JS_carteTableauOuvertures.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_carteTableauOuvertures); });
  server.on("/JS_Home/JS_carteTableauPuissance.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_carteTableauPuissance); });
  server.on(
      "/ajaxNewConfHome",
      HTTP_OPTIONS,
      [](AsyncWebServerRequest *request)
      {
        AsyncWebServerResponse *response =
            request->beginResponse(204);

        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");

        request->send(response);
      });

  server.on(
      "/ajaxNewConfHome",
      HTTP_POST,
      [](AsyncWebServerRequest *request)
      {
        lectureCookie(request, "{\"new_configActions\":\"ok\"}");
        DeserializationError error = deserializeJson(LesZones, postData);
        postData = "";

        if (error)
        {
          EcranPrintln("Failed to parse LesZones");
        }

        SauveConfig();
        LitConfig();
      },
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        for (size_t i = 0; i < len; i++)
        {
          postData += (char)data[i];
        }
      });

  // Ecran
  //*****
  server.on("/ForceAction", HTTP_GET, [](AsyncWebServerRequest *request) { // BESOIN A CONFIRMER ???????????????
                                                                           // int Force = server.arg("Force").toInt();
                                                                           // int NumAction = server.arg("NumAction").toInt();
                                                                           // if (NumAction < NbActions) {
                                                                           //   LesActions[NumAction].tOnOff = Force;
                                                                           // }
    request->send(200, "text/html", "Force");
  });

  // ============== Page Brute ===============
  // =========================================
  server.on("/pageHtmlBrute.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", pageHtmlBrute); });
  server.on("/JS_Brute.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_Brute); });

  // ======== Pages paramètres ==============
  // =========================================
  server.on("/pageHtmlPara.html", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    lectureCookie(request, pageHtmlPara);
    previousTempMillis = millis() - 120000; });

  server.on("/JS_Para.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_Para); });
  server.on("/ConfigAjax", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "application/json", SerializeConfiguration()); });

  server.on("/ParaThemeAjax", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "application/json", SerializeParaTheme()); });

  server.on(
      "/ajaxNewConfig",
      HTTP_OPTIONS,
      [](AsyncWebServerRequest *request)
      {
        AsyncWebServerResponse *response =
            request->beginResponse(204);

        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");

        request->send(response);
      });
  server.on(
      "/ajaxNewConfig",
      HTTP_POST,
      [](AsyncWebServerRequest *request)
      {
        lectureCookie(request, "{\"new_config\":\"ok\"}");
        CleAccesRef = CleAcces; // Nouvelle cle d'accès ou mise à jour
        byte ESP32_Previous_Type = ESP32_Type;
        LEDinit = false;
        DeserializeConfiguration(postData);
        postData = "";
        
        SauveConfig();
        EcranPrintln("Configuration sauvée");
        if (ESP32_Type != ESP32_Previous_Type)
        {
          delay(100);
          ESP.restart();
        }

        Init_GPIOs(); 
        Init_LED_OLED();
        if (Source != "Ext")
        {
          Source_data = Source; // Source = referente routeur
        }
        previousTempMillis = millis() - 60000;
        LastHeureRTE = -1;
        if (ESP32_Type == CARTE_ECRAN)
        {

          // GoPage(NumPage); ??????????
        }
      },
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        for (size_t i = 0; i < len; i++)
        {
          postData += (char)data[i];
        }
      });
  // ========== Pages actions ==========
  // =========================================
  server.on( // Option pour le debug avec le serveur image sur PC
      "/ajaxNewConfAction",
      HTTP_OPTIONS,
      [](AsyncWebServerRequest *request)
      {
        AsyncWebServerResponse *response =
            request->beginResponse(204);

        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");

        request->send(response);
      });
  server.on(
      "/ajaxNewConfAction",
      HTTP_POST,
      [](AsyncWebServerRequest *request)
      {
        lectureCookie(request, "{\"new_configActions\":\"ok\"}");
        DeserializationError error = deserializeJson(LesActions, postData);
        postData = "";

        if (error)
        {
          EcranPrintln("Failed to parse LesActions");
        }

        SauveConfig();
        LitConfig();
        EcranPrintln("Configuration GPIO après mise à jour des actions");
        Init_GPIOs(); 
      },
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        for (size_t i = 0; i < len; i++)
        {
          postData += (char)data[i];
        }
      });

  server.on( // Option pour le debug avec le serveur image sur PC
      "/ajaxNewPID",
      HTTP_OPTIONS,
      [](AsyncWebServerRequest *request)
      {
        AsyncWebServerResponse *response =
            request->beginResponse(204);

        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");

        request->send(response);
      });
  server.on(
      "/ajaxNewPID",
      HTTP_POST,
      [](AsyncWebServerRequest *request)
      {
        JsonDocument LesNewPID; // Paramètres durant les reglages PID
        lectureCookie(request, "{\"new_configPID\":\"ok\"}");
        DeserializationError error = deserializeJson(LesNewPID, postData);
        int NumAction = LesNewPID["numAction"].as<byte>();
        BlocsActions[NumAction].P = LesNewPID["P"].as<byte>();
        BlocsActions[NumAction].I = LesNewPID["I"].as<byte>();
        BlocsActions[NumAction].D = LesNewPID["D"].as<byte>();
        BlocsActions[NumAction].PID_On = LesNewPID["PID_On"].as<bool>();
        postData = "";

        if (error)
        {
          EcranPrintln("Failed to parse PID");
        }
      },
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        for (size_t i = 0; i < len; i++)
        {
          postData += (char)data[i];
        }
      });

  server.on("/CleUpdate", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    lectureCookie(request, "");
    request->send(200, "text/html", "OKcle"); });
  server.on("/ajaxRAZhisto", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    //??? A REDEFINIR

    
    for (int8_t i=0;i <NB_SOURCES_DEVICE_MAX;i++) {;
        LaSourceVar[i].RAZ_JSY = true;
    }
    request->send(200, "text/html", "OK"); });
  server.on("/ajax_Noms", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String S = "S3" + GS + RMS_NomEtatLocal;  //S3 en premier puis suivant definition pour les routeurs classiques
    request->send(200, "text/html", S); });
  server.on("/pageHtmlExport.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { lectureCookie(request, pageHtmlExport); });
  
server.on("/export_file", HTTP_GET, [](AsyncWebServerRequest *request)
{
    String Fichier = "/", S = "";

    if (request->hasParam("Fichier")) {
        Fichier += request->getParam("Fichier")->value();

        if (request->hasParam("Delete")) {
            LittleFS.remove(Fichier);
            request->send(200, "text/plain", "Fichier supprime");
        } else {
            File file = LittleFS.open(Fichier);

            if (file) {
                while (file.available()) {
                    S += (char)file.read();
                }
                file.close();

                AsyncWebServerResponse *response =
                    request->beginResponse(200, "text/csv", S);

                String NomFichier = Fichier.substring(
                    Fichier.lastIndexOf('/') + 1);

                response->addHeader(
                    "Content-Disposition",
                    "attachment; filename=\"" + NomFichier + "\"");

                request->send(response);
            } else {
                request->send(404, "text/plain", "Fichier introuvable");
            }
        }
    } else {
        request->send(400, "text/plain", "Parametre Fichier absent");
    }
});


  server.on(
      "/import", HTTP_POST, [](AsyncWebServerRequest *request)
      { request->send(200, "text/plain", "Fichier reçu avec succès !"); },
      [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
      {
        int nbOK = 0;
        if (!index)
        {
          EcranPrintln("Upload de fichier : " + String(filename));
          if (LittleFS.exists("/Temp_File.txt"))
          {
            LittleFS.remove("/Temp_File.txt");
          }
        }
        File file = LittleFS.open("/Temp_File.txt", FILE_APPEND);
        if (file)
        {
          file.write(data, len);
          file.close();
        }
        if (final)
        {
          EcranPrintln("Upload terminé");
          if (filename == "Configuration.json")
          {
            // Verification bon fichier
            file = LittleFS.open("/Temp_File.txt", "r");

            while (file.available())
            {
              String line = file.readStringUntil('\n');
              if (line.indexOf("Version") != -1)
                nbOK += 1;
              if (line.indexOf("Theme") != -1)
                nbOK += 1;
              if (line.indexOf("Source") != -1)
                nbOK += 1;
              if (line.indexOf("NbActions") != -1)
                nbOK += 1;
            }
            file.close();
          }
          else
          {
            nbOK = 4;
          }
          // Transfert dans le fichier de configuration
          if (nbOK > 3)
          {
            if (LittleFS.exists("/" + filename))
            {
              LittleFS.remove("/" + filename);
            }
            File file = LittleFS.open("/Temp_File.txt", "r");
            File newFile = LittleFS.open("/" + filename, "w");
            // Copier les données
            while (file.available())
            {
              newFile.write(file.read());
            }
            file.close();
            newFile.close();
            if (LittleFS.exists("/Temp_File.txt"))
            {
              LittleFS.remove("/Temp_File.txt");
            }
            EcranPrintln("Fichier " + filename + " mis à jour. Faites un reset");
          }
          else
          {
            EcranPrintln("Erreur. Ce n'est pas un fichier de configuration.");
          }
        }
      });

  server.on("/ListeFile", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String S = "";
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file) {
      S += file.name() + RS + String(file.size()) + GS;
      file = root.openNextFile();
    }
    request->send(200, "text/javascript", S); });
  // Actions
  // *******
  server.on("/pageHtmlActions.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { lectureCookie(request, pageHtmlActions); });
  server.on("/JS_Actions/JS_Action.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_Action); });
  server.on("/JS_Actions/JS_boolean.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_boolean); });
  server.on("/JS_Actions/JS_paraBloc.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_paraBloc); });
  
  server.on("/ActionsAjax", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String S = LesTemperatures() + RS;
    for (int c = 0; c < 4; c++) {
      S += nomTemperature[c] + US;
    }
    if (ReacCACSI < 1) ReacCACSI = 1;
    if (FreqPWM < 5) FreqPWM = 500;
    S = S + RS + String(LTARFbin) + RS + String(pTriac) + RS + String(ReacCACSI) + RS + String(FreqPWM) + RS + String(ModePara) + GS;
    for (int i = 0; i < NbActions; i++) {
     // S += LesActions[i].Lire();
    }
    request->send(200, "text/html", S); });
  server.on("/SetGPIO", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    int gpio = 0;
    int out = 0;
    if (request->hasParam("gpio")) {
      gpio = request->getParam("gpio")->value().toInt();
      out = request->getParam("out")->value().toInt();
    }
    String S = "Refut : gpio =" + String(gpio) + " out =" + String(out);
    if (gpio >= 4 && gpio <= 20 && out >= 0 && out <= 1) {
      pinMode(gpio, OUTPUT);
      digitalWrite(gpio, out);
      S = "OK : gpio =" + String(gpio) + " out =" + String(out);
    }
    request->send(200, "text/html", S); });

  // Commun
  //*******
  server.on("/ThemeCSS.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String  S = "";
              if (Theme.indexOf(".css") > 0) {
                File file = LittleFS.open("/"+Theme);
                while (file.available()) {
                  char c = file.read();
                  S += String(c);
                }
                file.close();
              }
              request->send(200, "text/css", S); });
  server.on("/CommunCSS.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/css", CommunCSS); });

  server.on("/JS_Commun.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/javascript", JS_Commun); });

  // DIVERS
  //******
  server.on("/pageHtmlWifi.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { lectureCookie(request, pageHtmlWifi); });
  server.on("/AP_ScanWifi", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", Liste_AP); });
  server.on("/AP_SetWifi_old", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    ResetWatchDog();
    EcranPrintln("Set Wifi");
    String NewSsid = "";
    String NewPassword = "";
    if (request->hasParam("ssid")) {
      NewSsid = request->getParam("ssid")->value();
      NewPassword = request->getParam("passe")->value();
    }
    NewSsid.trim();
    NewPassword.trim();
    EcranPrintln(NewSsid);
    EcranPrintln(NewPassword);
    ssid = NewSsid;
    password = NewPassword;
    ModeReseau = 0;
    dhcpOn = 1;
    SauveConfig();
    StockMessage("Wifi Begin : " + ssid);
    WiFi.begin(ssid.c_str(), password.c_str());
    unsigned long newstartMillis = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - newstartMillis < 20000)) {  // Attente connexion au Wifi
      Serial.write('!');
      Gestion_LEDs();
      EcranPrint(String(WiFi.status()));
      delay(300);
    }
    EcranPrintln("");
    String S = "";
    if (WiFi.status() == WL_CONNECTED) {
      EcranPrint("IP address: ");
      String IP = WiFi.localIP().toString();
      EcranPrintln(IP);
      S = "Ok" + RS;
      S += "ESP 32 connecté avec succès au wifi : " + ssid + " avec l'adresse IP : " + IP;
      S += "<br><br> Connectez vous au wifi : " + ssid;
      S += "<br><br> Cliquez sur l'adresse : <a href='http://" + IP + "' >http://" + IP + "</a>";
    } else {
      S = "No" + RS + "ESP32 non connecté à :" + ssid + "<br>";
    }
    request->send(200, "text/html", S);
    delay(1000);
    ESP.restart(); });

  server.on( // Option pour le debug avec le serveur image sur PC
      "/AP_SetWifi",
      HTTP_OPTIONS,
      [](AsyncWebServerRequest *request)
      {
        AsyncWebServerResponse *response =
            request->beginResponse(204);

        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");

        request->send(response);
      });
  server.on(
      "/AP_SetWifi",
      HTTP_POST,
      [](AsyncWebServerRequest *request)
      {
        JsonDocument NewWifi; // Paramètres durant les reglages PID
        
        DeserializationError error = deserializeJson(NewWifi, postData);
        String NewSsid = NewWifi["ssid"].as<String>();
        String NewPassword = NewWifi["password"].as<String>();
        postData = "";
        NewSsid.trim();
        NewPassword.trim();
        EcranPrintln(NewSsid);
        EcranPrintln(NewPassword);
        ssid = NewSsid;
        password = NewPassword;
        ModeReseau = 0;
        dhcpOn = 1;
        SauveConfig();
        delay(500);
        StockMessage("Wifi Begin : " + ssid);
        WiFi.begin(ssid.c_str(), password.c_str());
        unsigned long newstartMillis = millis();
        while (WiFi.status() != WL_CONNECTED && (millis() - newstartMillis < 20000))
        { // Attente connexion au Wifi
          Serial.write('!');
          Gestion_LEDs();
          EcranPrint(String(WiFi.status()));
          delay(300);
        }
        EcranPrintln("");
        String S = "";
        if (WiFi.status() == WL_CONNECTED)
        {
          EcranPrint("IP address: ");
          String IP = WiFi.localIP().toString();
          EcranPrintln(IP);
          S += "Ok||" ;
          S += "ESP 32 connecté avec succès au wifi : " + ssid + " avec l'adresse IP : " + IP;
          S += "<br><br> Connectez vous au wifi : " + ssid;
          S += "<br><br> Cliquez sur l'adresse : <a href='http://" + IP + "' >http://" + IP + "</a>";
          delay(2000);
          ESP.restart();
        }
        else
        {
          S += "No||" + String("ESP32 non connecté à :") + ssid + "<br>";
        }
        lectureCookie(request, "{\"new_Wifi\":\"" + S + "\"}");
        if (error)
        {
          EcranPrintln("Failed to parse Wifi");
        }
      },
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {
        for (size_t i = 0; i < len; i++)
        {
          postData += (char)data[i];
        }
      });
  server.on("/pageHtmlHeure.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { lectureCookie(request, pageHtmlHeure); });
  server.on("/HourUpdate", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      String New_H = "";
      String New_J = "";
      int H_=0,Mn=0;
      if (request->hasParam("Horloge")) {
        Horloge = request->getParam("Horloge")->value().toInt();    
      }
      if (Horloge==0) {            
            if (request->hasParam("idxFuseau")) {
              idxFuseau = request->getParam("idxFuseau")->value().toInt();  
              ntpServer = request->getParam("ntpServer")->value();         
            }
      } 
      SauveConfig();  
      if (Horloge==2) {            
            if (request->hasParam("New_H")) {
              New_H = request->getParam("New_H")->value();
            }
            int p = New_H.indexOf(":");
            if (p > 0) {
              H_ = (New_H.substring(0, p).toInt()) % 24;
              Mn = (New_H.substring(p + 1).toInt()) % 60;
            }
            if (request->hasParam("New_J")) {
              New_J = request->getParam("New_J")->value();
            }

          struct tm t = {0};                             // toujours initialiser à 0
          t.tm_year = New_J.substring(0, 4).toInt() -1900; // années depuis 1900
          t.tm_mon = New_J.substring(5, 7).toInt() - 1;    // mois 0–11 (octobre = 9)
          t.tm_mday = New_J.substring(8, 10).toInt();       // jour du mois
          t.tm_hour = H_;
          t.tm_min = Mn;
          t.tm_sec = 0;

             time_t now = mktime(&t);

              // 🔹 Création d’une structure timeval pour settimeofday()
            struct timeval tv = { .tv_sec = now, .tv_usec = 0 };
            settimeofday(&tv, nullptr);   // mise à l'heure de l’ESP32
            FormatteHeureDate();

      }
      request->send(200, "text/plain", "OKheure"); });
  server.on("/pageHtmlOTA.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { lectureCookie(request, pageHtmlOTA); });
  server.on(
      "/update", HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      [](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data,
         size_t len, bool final)
      {
        handleDoUpdate(request, filename, index, data, len, final);
      });

  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    request->send(200, "text/plain", "OK Reset. Attendez.");
    Record_Data(DateAMJ, Int_Heure, Int_Minute); //Enregistre dernières données
    delay(1000);
    ESP.restart(); });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam(PARAM_MESSAGE)) {
      message = request->getParam(PARAM_MESSAGE)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, GET: " + message); });

  // Send a POST request to <IP>/post with a form field message set to <message>
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    String message;
    if (request->hasParam(PARAM_MESSAGE, true)) {
      message = request->getParam(PARAM_MESSAGE, true)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, POST: " + message); });

  server.onNotFound(notFound);

  server.begin();
}

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}
void handleDoUpdate(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{ // Mise à jour par OTA
  char progress[30];
  if (!index)
  {
    EcranPrintln("Update");
    // content_len = request->contentLength();
    if (!Update.begin(UPDATE_SIZE_UNKNOWN))
    {
      Update.printError(Serial);
    }
  }

  if (Update.write(data, len) != len)
  {
    Update.printError(Serial);
    sprintf(progress, "Progress: %d%%\n", (Update.progress() * 100) / Update.size());
    EcranPrintln(String(progress));
  }

  if (final)
  {
    AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Please wait while the device reboots");
    response->addHeader("Refresh", "20");
    response->addHeader("Location", "/");
    request->send(response);
    if (!Update.end(true))
    {
      Update.printError(Serial);
    }
    else
    {
      EcranPrintln("Update complete");
      Serial.flush();
      ESP.restart();
    }
  }
}

void lectureCookie(AsyncWebServerRequest *request, const String &S)
{
  CleAcces = "";
  if (request->hasHeader("Cookie"))
  {
    const AsyncWebHeader *h = request->getHeader("Cookie");
    String cookies = h->value();
    int start = cookies.indexOf("CleAcces=");
    if (start > -1)
    {
      start += +9;
      int end = cookies.indexOf(";", start);
      if (end == -1)
        end = cookies.length();
      CleAcces = cookies.substring(start, end);
      CleAcces.trim();
    }
  }
  Serial.print("Clé accès reçue :" + CleAcces); // Pas de Telnet pour ne pas mettre le mot de passe sur le réseau
  Serial.println("  Attendue :" + CleAccesRef);
  if (S != "")
  {
    AsyncWebServerResponse *response;
    if (CleAccesRef == CleAcces)
    {
      response = request->beginResponse(200, "text/html", S);
    }
    else
    {
      response = request->beginResponse(200, "text/html", pageHtmlCleAcces);
    }
    // CORS
    response->addHeader("Access-Control-Allow-Origin", "*");

    request->send(response);
  }
}
