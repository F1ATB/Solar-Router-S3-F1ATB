#include "Communications/MQTT.h"
#include "BlocActions.h"
#include "Cartes/carte.h"
#include "Communications/Internet.h"
#include "Communications/TempoRTE.h"
#include "Communications/comSortie.h"
#include "Ecrans/Ecran.h"
#include "Puissance.h"
#include "Sources/Source_Linky.h"
#include "Temperatures/Temperature.h"
#include "commonFX.h"
#include "config.h"
#include <ETH.h>
#include <PubSubClient.h> //Librairie pour la gestion Mqtt
#include <WiFi.h>
#include <WiFiClientSecure.h>

// Paramètres pour puissance via MQTT A TRANSFERER Dans source mqtt ????????
String P_MQTT_Brute = "";
float PwMQTT = 0;
float PvaMQTT = 0;
float PfMQTT = 1;

unsigned int MQTTRepet = 0;
unsigned long MQTTIP = 0;
unsigned int MQTTPort = 1883;
String MQTTUser = "User";
String MQTTPwd = "password";
String MQTTPrefix = "homeassistant"; // prefix obligatoire pour l'auto-discovery entre HA et Core-Mosquitto (par défaut c'est homeassistant)
String MQTTPrefixEtat = "homeassistant";
String MQTTdeviceName = "routeur_rms";
String TopicP = "PuissanceMaison";
byte subMQTT = 0;
int8_t CompteurEnvoiMQTT = 0;

unsigned long previousMQTTenvoiMillis;
unsigned long previousMQTTMillis;
unsigned long LastPwMQTTMillis = 0;
unsigned long PeriodeMQTTMillis = 500;

static char DEVICE[300];
static char ESP_ID[15];
static char mdl[30];
static char StateTopic[50];
static char StateTopicJsonS[50];
static char StateTopicJsonA[50];
static char PrefixMQTT[25];
static char PrefixMQTTEtat[25];
static char AvailableTopic[60];

// Types de composants reconnus par HA et obligatoires pour l'Auto-Discovery.
static const char *SSR = "sensor";
static const char *SLCT = "select";
static const char *NB = "number";
static const char *BINS = "binary_sensor";
static const char *SWTC = "switch";
static const char *TXT = "text";

const char *const REGULATION_TYPE_NAMES[] = {"INACTIF", "ONOFF", "DECOUPE_SINUS", "DEMI_SINUS", "MULTI_SINUS", "TRAIN_DE_SINUS", "PWM"};

bool MQTT_Discovered = false;

//*************************************************************************
//*          CONFIG OF DISCOVERY MESSAGE FOR HOME ASSISTANT  / DOMOTICZ             *
//*************************************************************************

static WiFiClient MqttClient;
static PubSubClient clientMQTT(MqttClient);

void sendMQTTDiscoveryMsg_global();
void SourceToDiscover(String VarName, String TitleName, String Unit, String Class, String Round, int8_t NumSource, int NumCanal);
void ActionToDiscover(String VarName, String TitleName, String Unit, String Class, String Round, int NumAction);
void DeviceToDiscover(String VarName, String TitleName, String Unit, String Class, String Round);
void DeviceToDiscoverWithoutUnit(String VarName, String TitleName, String Round);
void DeviceToDiscoverWithoutClass(String VarName, String TitleName, String Unit, String Round);
void DeviceBin2Discover(String VarName, String TitleName);
void DeviceTextToDiscover(String VarName, String TitleName);
bool testMQTTconnected();
void envoiVersMQTT();
void callback(char *topic, byte *payload, unsigned int length);
void SendDataToHomeAssistant();

void GestionMQTT() {
  bool ActiviteMQTT = false;
  if (ModeReseau < 2 && WiFi.status() == WL_CONNECTED) // A VERIFIER CAS ETHERNET ??????????????
  {
    for (int C = 0; C < CANAUX_TEMPERATURE; C++) {
      if (Source_Temp[C] == "tempMqtt") {
        ActiviteMQTT = true;
        break; // On a trouvé une source tempMqtt, pas besoin de continuer la boucle
      }
    }
    for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++) {
      if (LaSource[source].Type == "Pmqtt") {
        ActiviteMQTT = true;
        break; // On a trouvé une source Pmqtt, pas besoin de continuer la boucle
      }
    }
    if (MQTTRepet > 0 || ActiviteMQTT || subMQTT == 1) { // Si une activité par Mqtt est demandée
      if (testMQTTconnected()) {
        clientMQTT.loop();
        envoiVersMQTT();
      }
    }
  }
}

bool testMQTTconnected() {
  bool connecte = true;
  if (!clientMQTT.connected()) { // si le mqtt n'est pas connecté (utile aussi lors de la 1ere connexion)
    EcranPrintln("Connexion au serveur MQTT ...");
    String host = IP2String(MQTTIP);
    int8_t IdxM = Idx_Machine(MQTTIP);
    if (Machine[IdxM].NbCx < 100) {
      Machine[IdxM].NbCx++; // + 1 tentative de connexion
    } else {
      Machine[IdxM].NbCx = 2; // remet les compteurs au debut
      Machine[IdxM].Note = 1;
    }
    String S = "";
    if (MQTTPrefix != "")
      S = MQTTPrefix + "/";
    sprintf(PrefixMQTT, "%s", S.c_str());
    S = "";
    if (MQTTPrefixEtat != "")
      S = MQTTPrefixEtat + "/";
    sprintf(PrefixMQTTEtat, "%s", S.c_str());
    String TopicA = "Available";
    sprintf(AvailableTopic, "%s%s/%s", PrefixMQTTEtat, MQTTdeviceName.c_str(), TopicA.c_str());
    clientMQTT.setServer(host.c_str(), MQTTPort);
    clientMQTT.setCallback(callback); // Déclaration de la fonction de souscription
    if (clientMQTT.connect(MQTTdeviceName.c_str(), MQTTUser.c_str(), MQTTPwd.c_str(), AvailableTopic, 2, true,
                           "offline")) { // si l'utilisateur est connecté au mqtt
      StockMessage(MQTTdeviceName + " connecté au broker MQTT");
      if (Machine[IdxM].Note < 100)
        Machine[IdxM].Note++;
      clientMQTT.publish(AvailableTopic, "online", true);
      // Souscription aux topics de température reçues par Mqtt
      for (int C = 0; C < CANAUX_TEMPERATURE; C++) {
        if (Source_Temp[C] == "tempMqtt") {
          char TopicV[50];
          sprintf(TopicV, "%s", TopicT[C].c_str());
          clientMQTT.subscribe(TopicV);
        }
      }
      // Souscription aux topics de puissance reçues par Mqtt
      for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++) {
        if (LaSource[source].Type == "Pmqtt") {
          char Topicp[50];
          sprintf(Topicp, "%s", LaSource[source].TopicP.c_str());
          clientMQTT.subscribe(Topicp);
        }
      }
      if (subMQTT == 1) {
        char TopicAct[60];
        for (int i = 0; i < NbActions; i++) {
          if (BlocsActions[i].Nom.length() > 0) {
            sprintf(TopicAct, "%s/%s", MQTTdeviceName.c_str(), BlocsActions[i].Nom.c_str());
            clientMQTT.subscribe(TopicAct);
          }
        }
      }
      sprintf(StateTopic, "%s%s_state", PrefixMQTTEtat, MQTTdeviceName.c_str());
      sprintf(StateTopicJsonS, "%s%s_Sources_state", PrefixMQTTEtat, MQTTdeviceName.c_str()); // Source de puissance
      sprintf(StateTopicJsonA, "%s%s_Actions_state", PrefixMQTTEtat, MQTTdeviceName.c_str()); // Les Actions
      byte mac[6];                                                                            // the MAC address of your Wifi shield
      String cu = "http://" + WiFi.localIP().toString();
      if (ESP32_Type < 10) {
        WiFi.macAddress(mac);
      } else {
        // ETH.macAddress(); //?????????????????
        cu = "http://" + ETH.localIP().toString();
      }
      sprintf(ESP_ID, "%02x%02x%02x%02x%02x", mac[4], mac[3], mac[2], mac[1], mac[0]); // ID de l'entité pour HA
      sprintf(mdl, "%s%s", "ESP32 - ", ESP_ID);                                        // ID de l'entité pour HA
      String mf = "F1ATB - https://f1atb.fr";
      String hw = String(ESP.getChipModel()) + " rev." + String(ESP.getChipRevision());
      String sw = Version;
      sprintf(DEVICE, "{\"ids\":\"%s\",\"name\":\"%s\",\"mdl\":\"%s\",\"mf\":\"%s\",\"hw\":\"%s\",\"sw\":\"%s\",\"cu\":\"%s\"}", ESP_ID,
              nomRouteur.c_str(), mdl, mf.c_str(), hw.c_str(), sw.c_str(), cu.c_str());
      PeriodeMQTTMillis = 500;
    } else { // si utilisateur pas connecté au mqtt
      StockMessage("Echec connexion MQTT : " + host);
      if (Machine[IdxM].Note > 0)
        Machine[IdxM].Note--;
      connecte = false;
      delay(1);
      PeriodeMQTTMillis = 30000; // Penalisé 30s
      previousMQTTMillis = millis();
    }
  }
  return connecte;
}
void envoiVersMQTT() {
  unsigned long tps = millis();                                                     // utilisé pour l'envoie de l'état On/Off des actions.
  if (int((tps - previousMQTTenvoiMillis) / 1000) >= MQTTRepet && MQTTRepet != 0) { // Si Service MQTT activé avec période sup à 0
    previousMQTTenvoiMillis = tps;
    if (!MQTT_Discovered) { //(uniquement au démarrage discovery = 0 et toute les 5mn si HA redemarre) ??? A VERIFIER
      sendMQTTDiscoveryMsg_global();
    }
    SendDataToHomeAssistant();
    clientMQTT.loop();
  }
}
// Callback  après souscription à un topic et  réaliser une action
void callback(char *topic, byte *payload, unsigned int length) {}

void sendMQTTDiscoveryMsg_global() {
  clientMQTT.setBufferSize(1700);
  DeviceToDiscover("ESP32_On", "ESP32 On", "h", "duration", "2");

  for (int canal = 0; canal < 4; canal++) {
    if (Source_Temp[canal] != "tempNo")
      DeviceToDiscover("Temperature_" + String(canal), nomTemperature[canal], "°C", "temperature", "1");
  }
  String Prefix;
  for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++) {

    for (int8_t canalNum = 0; canalNum < NB_CANAUX_PAR_SOURCE; canalNum++) {
      if (LaSource[source].CanalVoie[canalNum] >= 0) {
        Prefix = "s" + String(source) + String(canalNum);
        SourceToDiscover("NomSource", Prefix + " @_Nom", "", "enum", "0", source, canalNum);
        SourceToDiscover("Pw", Prefix + " Puissance active", "W", "power", "0", source, canalNum);
        SourceToDiscover("PVA", Prefix + " Puissance apparente", "VA", "apparent_power", "0", source, canalNum);
        SourceToDiscover("EnJ", Prefix + " Energie Journalière", "Wh", "energy", "0", source, canalNum);
        SourceToDiscover("EnT", Prefix + " Energie Totale", "Wh", "energy", "0", source, canalNum);
      }
    }
  }

  for (int8_t bloc = 0; bloc < NbActions; bloc++) {
    Prefix = "a" + String(bloc);
    ActionToDiscover("NomAction", Prefix + " @_Nom", "", "enum", "0", bloc);
    ActionToDiscover("Ouvre", Prefix + " Ouverture", "%", "", "0", bloc);
    ActionToDiscover("Hequiv", Prefix + " Hequiv", "h", "", "2", bloc);
    ActionToDiscover("tOnOff", Prefix + " Force On/Off", "min", "", "0", bloc);
    ActionToDiscover("Regul", Prefix + " Régulation", "", "enum", "0", bloc);
  }

  EcranPrintln("Paramètres Auto-Discovery publiés !");
  MQTT_Discovered = true;
}

//****************************************
//* ENVOIE DES DATAS VERS HOME ASSISTANT *
//****************************************

void SendDataToHomeAssistant() {
  char value[1200];
  bool published ;
  CompteurEnvoiMQTT = (CompteurEnvoiMQTT + 1) % 10; // Compteur sur 10 envois pour les messages lents
  if (CompteurEnvoiMQTT == 2) {                     // Données que l'on publie 10 fois moins rapidement
    float H = float(T_On_seconde) / 3600.0;
    sprintf(value, "{\"ESP32_On\":%f", H);

    for (int canal = 0; canal < 4; canal++) {
      if (temperature[canal] > -100 && Source_Temp[canal] != "tempNo") {
        sprintf(value, "%s,\"Temperature_%s\": %.1f", value, String(canal).c_str(), temperature[canal]);
      }
    }

    sprintf(value, "%s}", value);
    published = clientMQTT.publish(StateTopic, value);
  }
  JsonDocument doc;
  String Prefix;
  for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++) {
    for (int8_t canalNum = 0; canalNum < NB_CANAUX_PAR_SOURCE; canalNum++) {
      if (LaSource[source].CanalVoie[canalNum] >= 0) {
        int idx = source * NB_CANAUX_PAR_SOURCE + canalNum;
        Prefix = "s" + String(source) + String(canalNum);
        doc[Prefix]["NomSource"] = LaSource[source].CanalNom[canalNum];
        doc[Prefix]["Pw"] = PuissanceS[idx] - PuissanceI[idx];
        doc[Prefix]["PVA"] = PVAS[idx] - PVAI[idx];
        doc[Prefix]["EnJ"] = EnergieJour_Soutiree[idx] - EnergieJour_Injectee[idx];
        doc[Prefix]["EnT"] = Energie_Soutiree[idx] - Energie_Injectee[idx];
      }
    }
  }
  String JSON;
  serializeJson(doc, JSON);
  published = clientMQTT.publish(StateTopicJsonS, JSON.c_str());

  JsonDocument docA;
  for (int8_t bloc = 0; bloc < NbActions; bloc++) {
    Prefix = "a" + String(bloc);
    docA[Prefix]["NomAction"] = BlocsActions[bloc].Nom;
    docA[Prefix]["Ouvre"] = 100 - Retard[bloc];
    docA[Prefix]["Hequiv"] = BlocsActions[bloc].Hequiv;
    docA[Prefix]["tOnOff"] = BlocsActions[bloc].tOnOff;
    docA[Prefix]["Regul"] = String(REGULATION_TYPE_NAMES[BlocsActions[bloc].Regulation]);
  }

  String JSONA;
  serializeJson(docA, JSONA);
  published = clientMQTT.publish(StateTopicJsonA, JSONA.c_str());
}

void SourceToDiscover(String VarName, String TitleName, String Unit, String Class, String Round, int8_t NumSource, int NumCanal) {
  char value[700];
  char DiscoveryTopic[150];
  char UniqueID[80];
  char ValTpl[120];
  char unit_state_class[60];

  sprintf(DiscoveryTopic, "%s%s/%s_%s_%d_%d/config", PrefixMQTT, SSR, MQTTdeviceName.c_str(), VarName.c_str(), NumSource, NumCanal);
  sprintf(UniqueID, "%s_%s_%d_%d", MQTTdeviceName.c_str(), VarName.c_str(), NumSource, NumCanal);

  sprintf(unit_state_class, "%s", "");

  if (Unit == "Wh" || Unit == "kWh")
    sprintf(unit_state_class, "\"unit_of_meas\":\"%s\",\"state_class\":\"total_increasing\",", Unit.c_str());
  if (Unit == "W" || Unit == "VA")
    sprintf(unit_state_class, "\"unit_of_meas\":\"%s\",\"state_class\":\"measurement\",", Unit.c_str());

  if (Unit == "") {
    sprintf(ValTpl, "{{ value_json.s%d%d.%s }}", NumSource, NumCanal, VarName.c_str());
  } else {
    sprintf(ValTpl, "{{ value_json.s%d%d.%s|default(0)|round(%s) }}", NumSource, NumCanal, VarName.c_str(), Round.c_str());
  }

  sprintf(value,
          "{\"name\":\"%s\",\"uniq_id\":\"%s\","
          "\"stat_t\":\"%s\","
          "\"device_class\":\"%s\",%s"
          "\"val_tpl\":\"%s\","
          "\"device\":%s,"
          "\"availability_topic\":\"%s\"}",
          TitleName.c_str(), UniqueID, StateTopicJsonS, Class.c_str(), unit_state_class, ValTpl, DEVICE, AvailableTopic);
  clientMQTT.publish(DiscoveryTopic, value);
}

void ActionToDiscover(String VarName, String TitleName, String Unit, String DeviceClass, String Round, int NumAction) {
  char value[700];
  char DiscoveryTopic[150];
  char UniqueID[80];
  char ValTpl[120];
  char unit_state_class[160];

  sprintf(DiscoveryTopic, "%s%s/%s_%s_%d/config", PrefixMQTT, SSR, MQTTdeviceName.c_str(), VarName.c_str(), NumAction);
  sprintf(UniqueID, "%s_%s_%d", MQTTdeviceName.c_str(), VarName.c_str(), NumAction);
  sprintf(unit_state_class, "%s", "");

  if (Unit == "h") {
    sprintf(unit_state_class, "\"unit_of_meas\":\"h\",\"device_class\":\"duration\",");
  } else if (Unit == "%") {
    sprintf(unit_state_class, "\"unit_of_meas\":\"%s\",\"unit_class\":\"%s\",", Unit.c_str(), Unit.c_str());
  } else if (Unit == "min") {
    sprintf(unit_state_class, "\"unit_of_meas\":\"min\",");
  } else if (DeviceClass != "") {
    sprintf(unit_state_class, "\"device_class\":\"%s\",", DeviceClass.c_str());
  }
  if (Unit == "") {
    sprintf(ValTpl, "{{ value_json.a%d.%s }}", NumAction, VarName.c_str());
  } else {
    sprintf(ValTpl, "{{ value_json.a%d.%s|default(0)|round(%s) }}", NumAction, VarName.c_str(), Round.c_str());
  }
  sprintf(value,
          "{\"name\":\"%s\",\"uniq_id\":\"%s\","
          "\"stat_t\":\"%s\",%s"
          "\"val_tpl\":\"%s\","
          "\"device\":%s,"
          "\"availability_topic\":\"%s\"}",
          TitleName.c_str(), UniqueID, StateTopicJsonA, unit_state_class, ValTpl, DEVICE, AvailableTopic);
  clientMQTT.publish(DiscoveryTopic, value);
}

void DeviceToDiscover(String VarName, String TitleName, String Unit, String Class, String Round) {
  char value[700];
  char DiscoveryTopic[120];
  char UniqueID[50];
  char ValTpl[60];
  char state_class[60];
  sprintf(DiscoveryTopic, "%s%s/%s_%s/%s", PrefixMQTT, SSR, MQTTdeviceName.c_str(), VarName.c_str(), "config");
  sprintf(UniqueID, "%s_%s", MQTTdeviceName.c_str(), VarName.c_str());
  sprintf(ValTpl, "{{ value_json.%s|default(0)|round(%s)}}", VarName.c_str(), Round.c_str());
  sprintf(state_class, "%s", "");
  if (Unit == "Wh" || Unit == "kWh") {
    sprintf(state_class, "\"state_class\":\"total_increasing\"%s,", state_class);
  }
  sprintf(value,
          "{\"name\": \"%s\",\"uniq_id\": \"%s\",\"stat_t\": \"%s\",\"device_class\": \"%s\",\"unit_of_meas\": \"%s\",%s\"val_tpl\": "
          "\"%s\",\"device\": %s, \"availability_topic\": \"%s\"}",
          TitleName.c_str(), UniqueID, StateTopic, Class.c_str(), Unit.c_str(), state_class, ValTpl, DEVICE, AvailableTopic);
  clientMQTT.publish(DiscoveryTopic, value);
}
void DeviceToDiscoverWithoutUnit(String VarName, String TitleName, String Round) {
  char value[700];
  char DiscoveryTopic[120];
  char UniqueID[50];
  char ValTpl[60];
  sprintf(DiscoveryTopic, "%s%s/%s_%s/%s", PrefixMQTT, SSR, MQTTdeviceName.c_str(), VarName.c_str(), "config");
  sprintf(UniqueID, "%s_%s", MQTTdeviceName.c_str(), VarName.c_str());
  sprintf(ValTpl, "{{ value_json.%s|default(0)|round(%s)}}", VarName.c_str(), Round.c_str());
  sprintf(value, "{\"name\": \"%s\",\"uniq_id\": \"%s\",\"stat_t\": \"%s\",\"val_tpl\": \"%s\",\"device\": %s, \"availability_topic\": \"%s\"}",
          TitleName.c_str(), UniqueID, StateTopic, ValTpl, DEVICE, AvailableTopic);
  clientMQTT.publish(DiscoveryTopic, value);
}
void DeviceToDiscoverWithoutClass(String VarName, String TitleName, String Unit, String Round) {
  char value[700];
  char DiscoveryTopic[120];
  char UniqueID[50];
  char ValTpl[60];
  sprintf(DiscoveryTopic, "%s%s/%s_%s/%s", PrefixMQTT, SSR, MQTTdeviceName.c_str(), VarName.c_str(), "config");
  sprintf(UniqueID, "%s_%s", MQTTdeviceName.c_str(), VarName.c_str());
  sprintf(ValTpl, "{{ value_json.%s|default(0)|round(%s)}}", VarName.c_str(), Round.c_str());
  sprintf(value,
          "{\"name\": \"%s\",\"uniq_id\": \"%s\",\"stat_t\": \"%s\",\"unit_of_meas\": \"%s\",\"val_tpl\": \"%s\",\"device\": %s, "
          "\"availability_topic\": \"%s\"}",
          TitleName.c_str(), UniqueID, StateTopic, Unit.c_str(), ValTpl, DEVICE, AvailableTopic);
  clientMQTT.publish(DiscoveryTopic, value);
}

void DeviceBin2Discover(String VarName, String TitleName) {
  char value[700];
  char DiscoveryTopic[120];
  char UniqueID[50];
  char ValTpl[60];
  int init = 0; // default value
  String ic = "mdi:electric-switch";
  sprintf(DiscoveryTopic, "%s%s/%s_%s/%s", PrefixMQTT, BINS, MQTTdeviceName.c_str(), VarName.c_str(), "config");
  sprintf(UniqueID, "%s_%s", MQTTdeviceName.c_str(), VarName.c_str());
  sprintf(ValTpl, "{{ value_json.%s}}", VarName.c_str());
  sprintf(value,
          "{\"name\": \"%s\",\"uniq_id\": \"%s\",\"stat_t\": \"%s\",\"init\": %d,\"ic\": "
          "\"%s\",\"payload_off\":\"0\",\"payload_on\":\"1\",\"val_tpl\": \"%s\",\"device\": %s, \"availability_topic\": \"%s\"}",
          TitleName.c_str(), UniqueID, StateTopic, init, ic.c_str(), ValTpl, DEVICE, AvailableTopic);
  clientMQTT.publish(DiscoveryTopic, value);
}

void DeviceTextToDiscover(String VarName, String TitleName) {
  char value[600];
  char DiscoveryTopic[120];
  char UniqueID[50];
  char ValTpl[50];
  sprintf(DiscoveryTopic, "%s%s/%s_%s/%s", PrefixMQTT, SSR, MQTTdeviceName.c_str(), VarName.c_str(), "config");
  sprintf(UniqueID, "%s_%s", MQTTdeviceName.c_str(), VarName.c_str());
  sprintf(ValTpl, "{{ value_json.%s }}", VarName.c_str());
  sprintf(value,
          "{\"name\": \"%s\",\"uniq_id\": \"%s\",\"stat_t\": \"%s\",\"device_class\": \"%s\",\"val_tpl\": \"%s\",\"device\": %s, "
          "\"availability_topic\": \"%s\"}",
          TitleName.c_str(), UniqueID, StateTopic, "enum", ValTpl, DEVICE, AvailableTopic);
  clientMQTT.publish(DiscoveryTopic, value);
}