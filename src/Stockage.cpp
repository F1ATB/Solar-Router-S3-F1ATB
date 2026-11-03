// ***************************
// Stockage des données en ROM/LittleFS
// ***************************

#include "config.h"
#include "Stockage.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"
#include "LittleFS.h"
#include "BlocActions.h"
#include "commonFX.h"
#include "Temperatures/Temperature.h"
#include "Communications/comSortie.h"
#include "Puissance.h"
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
#include "Communications/TempoRTE.h"
#include "Cartes/carte.h"

void DeserializeConfiguration(String json);
String SerializeConfiguration();
void Creation_Fichier_JM(String nom);
void Ecrit_Fichier(String Fichier, String data);
String Record_Conf = "";
bool LastRecordData = false;

void Record_Data(String Date_AMJ, int16_t Heure_, int16_t Minute_) // On rentre ici à la date du jour ou de la veille
{
  if (HeureValide && LastRecordData)
  {
    String New_Record_Conf = "Date";
    String Nom, NomSoutir, NomInject;

    int8_t canal = 0;
    for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
    {
      for (int8_t canalV = 0; canalV < NB_CANAUX_PAR_SOURCE; canalV++)
      {
        canal = source * NB_CANAUX_PAR_SOURCE + canalV;

        if (LaSource[source].CanalVoie[canalV] >= 0)
        {

          Nom = LaSource[source].CanalNom[canalV];
          Nom.replace(",", ".");
          Nom.replace("#", "=");
          NomSoutir = LaSource[source].NomSoutir[canalV];
          NomSoutir.replace(",", ".");
          NomSoutir.replace("#", "=");
          NomInject = LaSource[source].NomInject[canalV];
          NomInject.replace(",", ".");
          NomInject.replace("#", "=");
          if (NomSoutir != "")
          {
            New_Record_Conf += "," + Nom + " (" + LaSource[source].Type + "#" + String(source) + String(canalV) + "S) Wh " + NomSoutir;
          }

          if (NomInject != "")
          {
            New_Record_Conf += "," + Nom + " (" + LaSource[source].Type + "#" + String(source) + String(canalV) + "I) Wh " + NomInject;
          }
        }
      }
    }
    String Fichier_Jour = "/Jour_Wh_" + Date_AMJ + ".csv";
    Creation_Fichier_JM(Fichier_Jour);
    String Fichier_Mois = "/Mois_Wh_" + Date_AMJ.substring(0, 6) + ".csv";
    Creation_Fichier_JM(Fichier_Mois);
    if (New_Record_Conf != Record_Conf)
    { // Nouvelle configuration
      Record_Conf = New_Record_Conf;
      Ecrit_Fichier(Fichier_Jour, Record_Conf + "\r\n");
      Ecrit_Fichier(Fichier_Mois, Record_Conf + "\r\n");
    }
    if (Date_AMJ != DateAMJ) // On a eu un changement de jour depuis le dernier enregistrement
    {
      int mois = DateAMJ.substring(4, 6).toInt();
      int jour = DateAMJ.substring(6, 8).toInt();
      int an = DateAMJ.substring(0, 4).toInt();
      String Fichier_JourR, JourR, MoisR;
      for (int i = 0; i < 20; i++) // Suppression des fichiers jour de plus de 13 jours
      {
        jour--;
        if (jour < 1)
        {
          mois--;
          jour = 31;
          if (mois < 1)
          {
            mois = 12;
            an--;
          }
        }
        if (i > 13)
        {
          JourR = String(jour);
          if (JourR.length() == 1)
            JourR = "0" + JourR;
          MoisR = String(mois);
          if (MoisR.length() == 1)
            MoisR = "0" + MoisR;
          String Fichier_JourR = "/Jour_Wh_" + String(an) + MoisR + JourR + ".csv";
          LittleFS.remove(Fichier_JourR);
        }
      }
      for (int i = 0; i < 20; i++) // Suppression des fichiers mois de plus de 15 mois
      {

        mois--;

        if (mois < 1)
        {
          mois = 12;
          an--;
        }

        if (i > 15)
        {

          MoisR = String(mois);
          if (MoisR.length() == 1)
            MoisR = "0" + MoisR;
          String Fichier_MoisR = "/Mois_Wh_" + String(an) + MoisR + ".csv";
          LittleFS.remove(Fichier_MoisR);
        }
      }
      String Fichier_Jour1 = "/Jour_Wh_" + DateAMJ + ".csv"; // Jour suivant
      Creation_Fichier_JM(Fichier_Jour1);
      // On stock les données de la veille au soir dans le fichier mois
      String Fichier_Mois_v = "/Mois_Wh_" + Date_AMJ.substring(0, 6) + ".csv"; // Mois veille
      Creation_Fichier_JM(Fichier_Mois_v);
      String contenuF = Date_AMJ.substring(6, 8) + "/" + Date_AMJ.substring(4, 6) + "/" + Date_AMJ.substring(0, 4); // Format date Excel
      for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
      {
        for (int8_t canalV = 0; canalV < NB_CANAUX_PAR_SOURCE; canalV++)
        {
          canal = source * NB_CANAUX_PAR_SOURCE + canalV;
          if (LaSource[source].CanalVoie[canalV] >= 0)
          {
            if (LaSource[source].NomSoutir[canalV] != "")
            {
              contenuF = contenuF + "," + String(EnergieJour_Soutiree[canal]);
            }
            if (LaSource[source].NomInject[canalV] != "")
            {
              contenuF = contenuF + "," + String(EnergieJour_Injectee[canal]);
            }
          }
        }

        //  file_v.close();
      }
      Ecrit_Fichier(Fichier_Mois_v, contenuF + "\r\n");
    }
    String H = String(Heure_);
    if (H.length() == 1)
      H = "0" + H;
    String M = String(Minute_);
    if (M.length() == 1)
      M = "0" + M;
    String contenuF = Date_AMJ.substring(6, 8) + "/" + Date_AMJ.substring(4, 6) + "/" + Date_AMJ.substring(0, 4) + " " + H + ":" + M; // Format date Excel
    for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
    {
      for (int8_t canalV = 0; canalV < NB_CANAUX_PAR_SOURCE; canalV++)
      {
        canal = source * NB_CANAUX_PAR_SOURCE + canalV;

        if (LaSource[source].CanalVoie[canalV] >= 0)
        {
          if (LaSource[source].NomSoutir[canalV] != "")
          {
            contenuF = contenuF + "," + String(EnergieJour_Soutiree[canal]);
          }
          if (LaSource[source].NomInject[canalV] != "")
          {
            contenuF = contenuF + "," + String(EnergieJour_Injectee[canal]);
          }
        }
      }
    }
    Ecrit_Fichier(Fichier_Jour, contenuF + "\r\n");
  }
}
void Creation_Fichier_JM(String nom)
{
  if (!LittleFS.exists(nom))
  {
    File file = LittleFS.open(nom, FILE_WRITE);
    file.print(Record_Conf + "\r\n");
    file.close();
  }
}
void Ecrit_Fichier(String Fichier, String data)
{
  File file = LittleFS.open(Fichier, FILE_APPEND);
  file.print(data);
  file.close();
}
void LitLastRecord_Data() // Lire le dernier enregistrement journalier pour récupérer les valeurs de EnergieJour_Soutiree et EnergieJour_Injectee
{
  String S = "", ligne = "";
  LastRecordData = true;
  String Fichier = "/Jour_Wh_" + DateAMJ + ".csv";
  EcranPrintln("Relecture:" + Fichier);
  if (LittleFS.exists(Fichier))
  {
    delay(1000);
    File file = LittleFS.open(Fichier);
    delay(1000);
    while (file.available())
    {
      char c = file.read();
      if (c == '\n')
      {
        if (ligne.length() > 10)
          S = ligne;
        ligne = "";
      }
      else if (c == '\r')
      {
        if (S.indexOf("Date,") == 0)
        {
          Record_Conf = S;
        }
      }
      else
      {
        ligne += String(c);
      }
    }
    file.close();
  }
  String Str = Record_Conf;
  String BeforeT, BeforeD, After;
  ligne = S + ",";
  while (Str.indexOf(",") >= 0)
  {
    SplitS(Str, BeforeT, ",", After); // les titres avec numéro de canal
    SplitS(After, BeforeT, "#", Str);
    SplitS(Str, BeforeT, ")", Str);
    int8_t canal = BeforeT.substring(0, 1).toInt() * NB_CANAUX_PAR_SOURCE + BeforeT.substring(1, 2).toInt();
    SplitS(ligne, BeforeD, ",", After); // Les anciennes données
    SplitS(After, BeforeD, ",", After);
    ligne = "," + After;
    if (BeforeT.indexOf("S") >= 0)
    {
      EnergieJour_Soutiree[canal] = BeforeD.toInt();
    }
    if (BeforeT.indexOf("I") >= 0)
    {
      EnergieJour_Injectee[canal] = BeforeD.toInt();
    }
  }
}

String HistoriqueEnergie(void)
{
  JsonDocument ener;
  JsonArray AnMois = ener["AnMois"].to<JsonArray>();
  JsonArray Data = ener["Data"].to<JsonArray>();

  File root = LittleFS.open("/");
  File file = root.openNextFile();
  int annee0 = 10000;
  while (file)
  {
    String nom = file.name();

    if (nom.endsWith(".csv") && nom.startsWith("Mois_Wh_"))
    {
      int annee = nom.substring(8, 12).toInt();
      annee0 = min(annee0, annee);
      EcranPrintln(String(annee0));
    }

    file = root.openNextFile();
  }
  int annee1 = DateAMJ.substring(0, 4).toInt();
  for (int annee = annee0; annee <= annee1; annee++)
  {
    for (int mois = 1; mois <= 12; mois++)
    {
      String Fichier_Mois = "/Mois_Wh_" + String(annee) + String(mois < 10 ? "0" : "") + String(mois) + ".csv";
      if (LittleFS.exists(Fichier_Mois))
      {

        File file = LittleFS.open(Fichier_Mois, "r");
        AnMois.add(Fichier_Mois);
        Data.add(file.readString());
      }
      file.close();
      file = File(); // Réinitialiser l'objet file pour éviter de réutiliser un fichier fermé
    }
  }
  String Json;
  serializeJson(ener, Json);
  return Json;
}

void LitConfig()
{

  File file = LittleFS.open("/Configuration.json");
  if (!file || file.isDirectory())
  {
    EcranPrintln("- failed to open file  Configuration.json . On cree le fichier");
    file.close();
    SauveConfig();
    return;
  }

  size_t taille = file.size();
  std::unique_ptr<char[]> buf(new char[taille + 1]);
  file.readBytes(buf.get(), taille);
  buf[taille] = '\0'; // Finir avec zéro
  DeserializeConfiguration(String(buf.get()));
  file.close();
  DecodageConfigAction();
  Calibration();
}
void DeserializeConfiguration(String json)
{

  int16_t Hdeb;
  JsonDocument conf;
  DeserializationError error = deserializeJson(conf, json);

  if (error)
  {
    EcranPrint("Erreur de parsing : ");
    EcranPrintln(error.c_str());
    return;
  }
  String VersionEnregistre = conf["Version"].as<String>();
  ssid = conf["ssid"].as<String>();
  password = conf["password"].as<String>();
  dhcpOn = conf["dhcpOn"].as<unsigned char>();
  Gateway = conf["Gateway"].as<unsigned long>();
  masque = conf["masque"].as<unsigned long>();
  localIP = conf["localIP"].as<unsigned long>();
  dns = conf["dns"].as<unsigned long>();
  CleAccesRef = conf["CleAccesRef"].as<String>();

  int i = 0;
  JsonArray arr = conf["LaSource"];
  for (JsonObject obj : arr)
  {
    if (i >= NB_SOURCES_DEVICE_MAX)
      break; // éviter de dépasser le tableau

    String Ty = LaSource[i].Type;
    unsigned long IP = LaSource[i].IP;
    int8_t Po = LaSource[i].Port;
    int8_t C0 = LaSource[i].CanalVoie[0];
    int8_t C1 = LaSource[i].CanalVoie[1];

    LaSource[i].Type = obj["Type"].as<String>();
    LaSource[i].IP = obj["IP"].as<unsigned long>();
    LaSource[i].Port = obj["Port"].as<signed char>();
    LaSource[i].TopicP=obj["TopicP"].as<String>();
    JsonArray arrN = obj["CanalNom"];
    for (size_t ii = 0; ii < arrN.size(); ii++)
    {
      LaSource[i].CanalNom[ii] = arrN[ii].as<String>();
    }
    JsonArray arrV = obj["CanalVoie"];
    for (size_t ii = 0; ii < arrV.size(); ii++)
    {
      LaSource[i].CanalVoie[ii] = arrV[ii].as<signed char>();
    }
    JsonArray arrS = obj["NomSoutir"];
    for (size_t ii = 0; ii < arrS.size(); ii++)
    {
      LaSource[i].NomSoutir[ii] = arrS[ii].as<String>();
    }
    JsonArray arrI = obj["NomInject"];
    for (size_t ii = 0; ii < arrI.size(); ii++)
    {
      LaSource[i].NomInject[ii] = arrI[ii].as<String>();
    }
    if (LaSource[i].Type != "Ext")
    {
      LaSource[i].Data = LaSource[i].Type;
    }
    else
    {
      LaSource[i].Data = "NotDef";
    }

    if (Ty != LaSource[i].Type || IP != LaSource[i].IP || Po != LaSource[i].Port || C0 != LaSource[i].CanalVoie[0] || C1 != LaSource[i].CanalVoie[1])
      RAZ_Source_Var(i); // RAZ Si changement de type de source
    i++;
  }

  Source = LaSource[0].Type; // Source referente du routeur
  hostname = conf["hostname"].as<String>();
  ModePara = conf["ModePara"].as<unsigned char>();
  ModeReseau = conf["ModeReseau"].as<unsigned char>();
  Horloge = conf["Horloge"].as<unsigned char>();
  idxFuseau = conf["idxFuseau"].as<unsigned char>();
  ntpServer = conf["ntpServer"].as<String>();
  int ESP32_TypeInt = conf["ESP32_Type"].as<unsigned char>();
  ESP32_Type = (CarteType)ESP32_TypeInt;
  ESP32_New_Type = ESP32_Type;
  LEDgroupe = conf["LEDgroupe"].as<unsigned char>();
  rotation = conf["rotation"].as<unsigned char>();
  DurEcran = conf["DurEcran"].as<unsigned long>();
  pUxI = conf["pUxI"].as<unsigned char>();
  pTemp = conf["pTemp"].as<unsigned char>();
  EnphaseUser = conf["EnphaseUser"].as<String>();
  EnphasePwd = conf["EnphasePwd"].as<String>();
  EnphaseSerial = conf["EnphaseSerial"].as<String>();
  MQTTRepet = conf["MQTTRepet"].as<unsigned short>();
  MQTTIP = conf["MQTTIP"].as<unsigned long>();
  MQTTPort = conf["MQTTPort"].as<unsigned short>();
  MQTTUser = conf["MQTTUser"].as<String>();
  MQTTPwd = conf["MQTTPwd"].as<String>();
  MQTTPrefix = conf["MQTTPrefix"].as<String>();
  MQTTPrefixEtat = conf["MQTTPrefixEtat"].as<String>();
  MQTTdeviceName = conf["MQTTdeviceName"].as<String>();
  subMQTT = conf["subMQTT"].as<unsigned short>();
  nomRouteur = conf["nomRouteur"].as<String>();
  Theme = conf["Theme"].as<String>();
  latitude = conf["latitude"].as<float>();
  longitude = conf["longitude"].as<float>();
  for (int i = 0; i < LES_ROUTEURS_MAX; i++)
  {
    RouteurExt[i].IP = 0;
  }

  i = 0;
  JsonArray arrX2 = conf["RouteurExt"];
  for (JsonObject obj : arrX2)
  {
    if (i >= LES_ROUTEURS_MAX)
      break; // éviter de dépasser le tableau

    RouteurExt[i].IP = obj["IP"].as<unsigned long>(); // Seul 'IP est renvoyé de la page parametre
    if (obj["NomRouteur"].is<String>())               // En mémoire Flash
    {
      RouteurExt[i].NomRouteur = obj["NomRouteur"].as<String>(); // On ne recupere pas le S3  afin de ne pas écraser le type détecté automatiquement
      JsonArray arrN = obj["CanalNom"];
      JsonArray arrV = obj["CanalVoie"];
      int k = 0;
      for (int s = 0; s < NB_SOURCES_DEVICE_MAX; s++)
      {
        for (int c = 0; c < NB_CANAUX_PAR_SOURCE; c++)
        {
          if (arrV[k].as<int>() >= 0)
          {
            RouteurExt[i].CanalNom[s][c] = arrN[k].as<String>();
          }
          else
          {
            RouteurExt[i].CanalNom[s][c] = "";
          }
          k++;
        }
      }
      JsonArray arrT = obj["TemperNom"];
      k = 0;

      for (int c = 0; c < CANAUX_TEMPERATURE; c++)
      {
        RouteurExt[i].TemperNom[c] = arrT[k].as<String>();
        k++;
      }
    }
    i++;
  }

  int j = 0;
  // On retire les IP à zéro (compacter le tableau)
  for (int i = 0; i < LES_ROUTEURS_MAX; i++)
  {
    if (RouteurExt[i].IP > 0)
    {
      if (i != j)
        RouteurExt[j] = RouteurExt[i];

      j++;
    }
  }
  for (int k = j; k < LES_ROUTEURS_MAX; k++)
  {
    RouteurExt[k].IP = 0;
    RouteurExt[k].NomRouteur = "";
  }
  initClientWebsocket(); // On verifie
  JsonArray arrT = conf["LesTemperatures"];
  int c = 0;
  for (JsonObject objT : arrT)
  {

    nomTemperature[c] = objT["nomTemperature"].as<String>();
    Source_Temp[c] = objT["Source_Temp"].as<String>();
    refTempIP[c] = objT["refTempIP"].as<unsigned long>();
    TopicT[c] = objT["TopicT"].as<String>();
    canalTempExterne[c] = objT["canalTempExterne"].as<unsigned char>();
    offsetTemp[c] = objT["offsetTemp"].as<signed short>();
    c++;
  }
  CalibU = conf["CalibU"].as<unsigned short>();
  CalibI = conf["CalibI"].as<unsigned short>();
  TempoRTEon = conf["TempoRTEon"].as<unsigned char>();
  WifiSleep = conf["WifiSleep"].as<unsigned char>();
  ComSurv = conf["ComSurv"].as<signed short>();
  pSerial1 = conf["pSerial1"].as<unsigned char>();
  pSerial2 = conf["pSerial2"].as<unsigned char>();
  pTriac = conf["pTriac"].as<unsigned char>();
  // Zone des actions
  EstimCACSI = conf["EstimCACSI"].as<bool>();
  NbActions = conf["NbActions"].as<signed short>();
  if (conf["LesActions"].is<JsonObject>())
  {
    LesActions = conf["LesActions"].as<JsonObject>();
  }
  if (conf["LesZones"].is<JsonObject>())
  {
    LesZones = conf["LesZones"].as<JsonObject>();
  }
}
void SauveConfig()
{
  String Json = SerializeConfiguration();
  File file = LittleFS.open("/Configuration.json", FILE_WRITE);
  file.print(Json);
  file.close();
  EcranPrintln("Fichier Configuration.json enregistré");
  Calibration();
}

String SerializeConfiguration() // Configuration paramètres et actions
{
  JsonDocument conf;
  conf["Version"] = Version;
  conf["ssid"] = ssid;
  conf["password"] = password;
  conf["dhcpOn"] = dhcpOn;
  conf["Gateway"] = Gateway;
  conf["masque"] = masque;
  conf["localIP"] = localIP;
  conf["dns"] = dns;
  conf["CleAccesRef"] = CleAccesRef;

  JsonArray arr = conf["LaSource"].to<JsonArray>();

  for (SourcePuissance &c : LaSource)
  {
    JsonObject obj = arr.add<JsonObject>();
    obj["Type"] = c.Type;
    obj["Data"] = c.Data;
    obj["IP"] = c.IP;
    obj["Port"] = c.Port;
    obj["TopicP"] = c.TopicP;

    JsonArray noms = obj["CanalNom"].to<JsonArray>();
    JsonArray voies = obj["CanalVoie"].to<JsonArray>();
    JsonArray soutir = obj["NomSoutir"].to<JsonArray>();
    JsonArray inject = obj["NomInject"].to<JsonArray>();
    for (int i = 0; i < NB_CANAUX_PAR_SOURCE; i++)
    {
      noms.add(c.CanalNom[i]);
      voies.add(c.CanalVoie[i]);
      soutir.add(c.NomSoutir[i]);
      inject.add(c.NomInject[i]);
    }
  }
  conf["hostname"] = hostname;
  conf["ModePara"] = ModePara;
  conf["ModeReseau"] = ModeReseau;
  conf["Horloge"] = Horloge;
  conf["idxFuseau"] = idxFuseau;
  conf["ntpServer"] = ntpServer;
  conf["ESP32_Type"] = (int)ESP32_New_Type;
  conf["LEDgroupe"] = LEDgroupe;
  conf["rotation"] = rotation;
  conf["DurEcran"] = DurEcran;
  conf["pUxI"] = pUxI;
  conf["pTemp"] = pTemp;
  conf["EnphaseUser"] = EnphaseUser;
  conf["EnphasePwd"] = EnphasePwd;
  conf["EnphaseSerial"] = EnphaseSerial;
  if (ModePara == 0)
  {
    MQTTRepet = 0;
    subMQTT = 0;
  }

  conf["MQTTRepet"] = MQTTRepet;
  conf["MQTTIP"] = MQTTIP;
  conf["MQTTPort"] = MQTTPort;
  conf["MQTTUser"] = MQTTUser;
  conf["MQTTPwd"] = MQTTPwd;
  conf["MQTTPrefix"] = MQTTPrefix;
  conf["MQTTPrefixEtat"] = MQTTPrefixEtat;
  conf["MQTTdeviceName"] = MQTTdeviceName;
  conf["subMQTT"] = subMQTT;
  conf["nomRouteur"] = nomRouteur;
  conf["Theme"] = Theme;
  conf["latitude"] = latitude;
  conf["longitude"] = longitude;

  // Sources externes
  JsonArray arrX = conf["RouteurExt"].to<JsonArray>();

  for (RouteurExterne &c : RouteurExt)
  {
    if (c.IP > 0)
    {
      JsonObject objX = arrX.add<JsonObject>();
      objX["Data"] = c.Data;
      objX["IP"] = c.IP;
      objX["NomRouteur"] = c.NomRouteur;
      objX["S3"] = c.S3;

      JsonArray nomc = objX["CanalNom"].to<JsonArray>();

      for (int s = 0; s < NB_SOURCES_DEVICE_MAX; s++)
      {
        for (int i = 0; i < NB_CANAUX_PAR_SOURCE; i++)
        {
          nomc.add(c.CanalNom[s][i]);
        }
      }
      JsonArray temper = objX["TemperNom"].to<JsonArray>();
      for (int i = 0; i < CANAUX_TEMPERATURE; i++)
      {
        temper.add(c.TemperNom[i]);
      }
      JsonArray actionsN = objX["ActionNom"].to<JsonArray>();
      for (int i = 0; i < BLOCS_ACTION_MAX; i++)
      {
        actionsN.add(c.NomActionExterne[i]);
      }
    }
  }

  JsonArray arrT = conf["LesTemperatures"].to<JsonArray>();
  for (int c = 0; c < 4; c++)
  {
    JsonObject objT = arrT.add<JsonObject>();
    objT["nomTemperature"] = nomTemperature[c];
    objT["Source_Temp"] = Source_Temp[c];
    objT["refTempIP"] = refTempIP[c];
    objT["TopicT"] = TopicT[c];
    objT["canalTempExterne"] = canalTempExterne[c];
    objT["offsetTemp"] = offsetTemp[c];
  }
  conf["CalibU"] = CalibU;
  conf["CalibI"] = CalibI;
  conf["TempoRTEon"] = TempoRTEon;
  conf["WifiSleep"] = WifiSleep;
  conf["ComSurv"] = ComSurv;
  conf["pSerial1"] = pSerial1;
  conf["pSerial2"] = pSerial2;
  conf["pTriac"] = pTriac;
  // Enregistrement des Actions
  conf["ReacCACSI"] = ReacCACSI;
  conf["FreqPWM"] = FreqPWM;
  conf["NbActions"] = NbActions;

  conf["LesActions"] = LesActions.as<JsonObject>();
  conf["LesZones"] = LesZones.as<JsonObject>();

  
  conf["ListeGPIO"] = ListeDesGPIOs(ESP32_Type);

  String Json;
  serializeJson(conf, Json);
  return Json;
}

// Paramètres Variables
String SerializeParaTheme()
{
  JsonDocument paraVar;

  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while (file)
  {
    String name = file.name();
    if (name.endsWith(".css"))
    {
      paraVar["FichiersCSS"].add(name);
    }
    file = root.openNextFile();
  }
  String Json;
  serializeJson(paraVar, Json);
  return Json;
}
// Bilan Mémoire
String BilanMemoire()
{
  // Mémoire SRAM interne
  Mem_SRAM[0] = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);        // Total
  Mem_SRAM[1] = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);         // disponible
  Mem_SRAM[2] = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL); // le minimum du disponible
  // Mémoire PSRAM externe
  Mem_PSRAM[0] = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);        // Total
  Mem_PSRAM[1] = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);         // disponible
  Mem_PSRAM[2] = heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM); // le minimum du disponible
  // Mémoire FLASH fichiers
  Mem_Fichier[2] = LittleFS.usedBytes();            // utilisé
  Mem_Fichier[0] = LittleFS.totalBytes();           // Total
  Mem_Fichier[1] = Mem_Fichier[0] - Mem_Fichier[2]; // disponible

  char buffer[250];
  snprintf(buffer, sizeof(buffer), "              |          Total|     Disponible|Min depuis boot| \n");
  String S = String(buffer);
  snprintf(buffer, sizeof(buffer), "SRAM Interne  |%15u|%15u|%15u| \n", Mem_SRAM[0], Mem_SRAM[1], Mem_SRAM[2]);
  S += String(buffer);
  snprintf(buffer, sizeof(buffer), "PSRAM Externe |%15u|%15u|%15u| \n", Mem_PSRAM[0], Mem_PSRAM[1], Mem_PSRAM[2]);
  S += String(buffer);
  snprintf(buffer, sizeof(buffer), "Fichiers      |%15u|%15u| \n", Mem_Fichier[0], Mem_Fichier[1]);
  S += String(buffer);

  return "               -------------- Mémoire ESP32-S3 -------------- \n" + S + "               ---------------------------------------------- ";
}