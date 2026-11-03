// *********************************************************************************************
// * Client d'autres ESP32 en charge de mesurer les températures ou donner l'état des routages *
// *********************************************************************************************

#include "config.h"
#include "RMS_Externes.h"
#include <Arduino.h>
#include <WiFi.h>
#include "commonFX.h"
#include "Temperatures/Temperature.h"
#include "Communications/comSortie.h"
#include "Communications/Internet.h"
#include "BlocActions.h"
#include "Ecrans/Ecran.h"
#include "Puissance.h"

// Routeurs du réseau
static int RMS_Noms_idx = 0;

RouteurExterne RouteurExt[LES_ROUTEURS_MAX];
RouteurComExterne RouteurComExt[LES_ROUTEURS_MAX];

String RMS_NomEtatLocal; // Definition pour routeur classique

void NomEtatLocal();
void receptionWebSocket(
    WStype_t type,
    uint8_t *payload,
    size_t length);

void Init_ListeRouteurs()
{
  for (int i = 0; i < LES_ROUTEURS_MAX; i++)
  {

    RouteurExt[i].IP = 0;
    RouteurExt[i].Data = "";
    RouteurExt[i].NomRouteur = "";
    RouteurExt[i].NomEtat = ""; // Ancien routeur classique
    RouteurExt[i].S3 = false;   //   Initialisation de la source externe
    for (int j = 0; j < BLOCS_ACTION_MAX; j++)
    {
      RouteurExt[i].NomActionExterne[j] = "";
      RouteurExt[i].OuvertureActionExterne[j] = 0;
      RouteurExt[i].HequivActionExterne[j] = 0.0;
    }

    for (int s = 0; s < NB_SOURCES_DEVICE_MAX; s++)
    {
      for (int j = 0; j < NB_CANAUX_PAR_SOURCE; j++)
      {
        RouteurExt[i].CanalNom[s][j] = "";
      }
    }
    for (int j = 0; j < CANAUX_TEMPERATURE; j++)
    {
      RouteurExt[i].temperature[j] = -12000;
      RouteurExt[i].TemperNom[j] = "";
    }
    RouteurComExt[i].IP = 0;
  }
}

void initClientWebsocket()
{ // On verifie l'initialisation des clients websocket
  
  unsigned long IPduRMS;

  for (int routeur = 0; routeur < LES_ROUTEURS_MAX; routeur++)
  {
    IPduRMS = RouteurExt[routeur].IP;
    bool trouve = false;
    for (int i = 0; i < LES_ROUTEURS_MAX; i++)
    {
      if (RouteurComExt[i].IP == IPduRMS)
      {
        trouve = true;
        break;
      }
    }
    if (!trouve)
    {
      for (int i = 0; i < LES_ROUTEURS_MAX; i++)
      {
        if (RouteurComExt[i].IP == 0)
        {
          EcranPrintln("Initialisation WebSocket pour " + IP2String(IPduRMS));

          RouteurComExt[i].webSocket.begin(IP2String(IPduRMS), 80, "/ws");
          RouteurComExt[i].webSocket.onEvent(receptionWebSocket);
          RouteurComExt[i].IP = IPduRMS;
          break;
        }
      }
    }
  }
}
void Liste_des_Noms()
{ // Les noms des routeurs partenaires
  for (int i = 0; i < LES_ROUTEURS_MAX; i++)
  {
    if (RouteurExt[i].IP > 0)
    {
      Liste_NomsEtats(i);
    }
  }
}

void RefreshNomsEtats()
{
  for (int i = 0; i < LES_ROUTEURS_MAX; i++)
  {
    NomEtatLocal(); // Canal interne au format du routeur classique
    RMS_Noms_idx = (RMS_Noms_idx + 1) % LES_ROUTEURS_MAX;
    if (RouteurExt[RMS_Noms_idx].IP > 0)
    {
      Liste_NomsEtats(RMS_Noms_idx);
      break;
    }
  }
}

void Liste_NomsEtats(int Idx_RMS) // On cherche si routeur classique ou S3 ainsi que les noms et états des canaux
{
  unsigned long IPduRMS = RouteurExt[Idx_RMS].IP;
  int8_t IdxM = Idx_Machine(IPduRMS); // Index des machines : routeurs et sources externes

  for (int i = 0; i < BLOCS_ACTION_MAX; i++)
  {
    if (RouteurExt[Idx_RMS].ValideActionExterne[i] > 0)
      RouteurExt[Idx_RMS].ValideActionExterne[i]--; // Securité si on ne recoit plus d'information sur ces actions
  }

  String url = "/ajax_Noms"; // Pour les ESP32 classiques ou S3 si on a pas encore identifié le type
  if (RouteurExt[Idx_RMS].S3)
  {
    url = "/ConfigAjax"; // Pour les ESP32-S3
  }
  url = "http://" + IP2String(IPduRMS) + url;
  String RMSExtDataB = httpGetPayload(url, IdxM); // On recupere la réponse HTTP du routeur externe
  if (RMSExtDataB == "")
    return;
  if (RouteurExt[Idx_RMS].S3) // Cas des ESP32-S3 avec le fichier de configuration
  {

    int p = RMSExtDataB.indexOf("{");
    RMSExtDataB = RMSExtDataB.substring(p);
    JsonDocument doc;
    DeserializationError erreur = deserializeJson(doc, RMSExtDataB);

    if (erreur)
    {
      EcranPrintln("ESP32-S3 : Erreur JSON");
      RouteurExt[Idx_RMS].S3 = false; // On revient en classique
    }
    else
    {
      RouteurExt[Idx_RMS].NomRouteur = doc["nomRouteur"].as<String>();
      Machine[IdxM].nom = RouteurExt[Idx_RMS].NomRouteur;
      int s = 0;
      JsonArray arr = doc["LaSource"];
      for (JsonObject obj : arr)
      {
        if (s >= NB_SOURCES_DEVICE_MAX)
          break; // éviter de dépasser le tableau
        JsonArray arrN = obj["CanalNom"];
        JsonArray arrV = obj["CanalVoie"];
        for (int c = 0; c < NB_CANAUX_PAR_SOURCE; c++)
        {
          if (arrV[c].as<int>() >= 0)
          {
            RouteurExt[Idx_RMS].CanalNom[s][c] = arrN[c].as<String>();
          }
          else
          {
            RouteurExt[Idx_RMS].CanalNom[s][c] = "";
          }
        }
        s++;
      }
      JsonArray arrT = doc["LesTemperatures"];
      int c = 0;
      for (JsonObject objT : arrT)
      {
        RouteurExt[Idx_RMS].TemperNom[c] = objT["nomTemperature"].as<String>();
        c++;
      }
    }
  }
  else
  { // Cas des ESP32 à priori classiques
    if (RMSExtDataB.length() > 600)
    {
      RMSExtDataB = "";
    }
    if (RMSExtDataB.indexOf("S3" + GS) >= 0)
    { // On a un ESP32-S3
      RouteurExt[Idx_RMS].S3 = true;
    }
    int p = RMSExtDataB.indexOf(GS);
    RMSExtDataB = RMSExtDataB.substring(p + 1);
    if (RMSExtDataB.indexOf(US) > 0)
    { // Trame semble OK

      RouteurExt[Idx_RMS].NomEtat = RMSExtDataB;
      String nom = "", after, Valeur;
      SplitS(RouteurExt[Idx_RMS].NomEtat, nom, US, after); // Extraction du nom seul
      RouteurExt[Idx_RMS].NomRouteur = nom;
      Machine[IdxM].nom = nom;

      // Extraction des Actions pour ESP32 classiques et S3
      SplitS(after, Valeur, US, after); // Températures decodées dans Temperature.cpp pour les ESP32 classiques
      String ActionsR, ActionR;
      SplitS(after, ActionsR, US, after); // Ensemble desActions
      for (int i = 0; i < BLOCS_ACTION_MAX; i++)
      {
        RouteurExt[Idx_RMS].NomActionExterne[i] = "";
        RouteurExt[Idx_RMS].OuvertureActionExterne[i] = 0;
        RouteurExt[Idx_RMS].HequivActionExterne[i] = 0.0;
        RouteurExt[Idx_RMS].ValideActionExterne[i] = 0;
      }
      while (ActionsR.length() > 1)
      {
        SplitS(ActionsR, ActionR, FS, ActionsR);
        SplitS(ActionR, Valeur, ES, ActionR); // Index
        int NumAction = Valeur.toInt();
        if (NumAction >= 0 && NumAction < BLOCS_ACTION_MAX)
        {
          SplitS(ActionR, RouteurExt[Idx_RMS].NomActionExterne[NumAction], ES, ActionR); // Nom de l'action externe
          SplitS(ActionR, Valeur, ES, ActionR);                                          // Ouverture
          RouteurExt[Idx_RMS].OuvertureActionExterne[NumAction] = Valeur.toInt();
          SplitS(ActionR, Valeur, ES, ActionR); // Hequiv
          RouteurExt[Idx_RMS].HequivActionExterne[NumAction] = Valeur.toFloat() / 100.0;
          RouteurExt[Idx_RMS].ValideActionExterne[NumAction] = 5;
        }
      }

      // Recherche de tous les noms pour routeur classique en lisant le fichier des paramètres de configuration
      if (!RouteurExt[Idx_RMS].S3)
      {
        url = "http://" + IP2String(IPduRMS) + "/ParaFixe";
        RMSExtDataB = httpGetPayload(url, IdxM); // On recupere la réponse HTTP du routeur externe
        if (RMSExtDataB == "")
          return;
        int p = RMSExtDataB.indexOf("{");
        RMSExtDataB = RMSExtDataB.substring(p);
        JsonDocument doc2;
        DeserializationError erreur = deserializeJson(doc2, RMSExtDataB);

        if (erreur)
        {

          EcranPrintln("ESP32 classique : Erreur JSON");
        }
        else
        {
          RouteurExt[Idx_RMS].NomRouteur = doc2["nomRouteur"].as<String>();
          for (int s = 0; s < NB_SOURCES_DEVICE_MAX; s++)
          {
            for (int i = 0; i < NB_CANAUX_PAR_SOURCE; i++)
            {
              RouteurExt[Idx_RMS].CanalNom[s][i] = ""; // RAZ des anciens noms de canaux
            }
          }
          RouteurExt[Idx_RMS].CanalNom[0][0] = doc2["nomSondeMobile"].as<String>();
          RouteurExt[Idx_RMS].CanalNom[0][1] = doc2["nomSondeFixe"].as<String>();
          for (int c = 0; c < CANAUX_TEMPERATURE; c++)
          {
            RouteurExt[Idx_RMS].TemperNom[c] = doc2["nomTemperature" + String(c)].as<String>();
          }
        }
      }
    }
  }
}

void NomEtatLocal()
{ // Génération de l'état local du routeur au format du routeur classique
  String S = nomRouteur + US;
  for (int canal = 0; canal < 4; canal++) // On rajoute des infos sur les 4 canaux de température
  {
    if (Source_Temp[canal] != "tempNo")
    {
      String St = String(temperature[canal]);
      if (St.indexOf(".") > 0)
        St = St.substring(0, St.indexOf(".") + 2);
      S += String(canal) + ES + nomTemperature[canal] + ES + St + ES + Source_Temp[canal] + FS;
    }
  }
  S += US;
  for (int i = 0; i < NbActions; i++) // On rajoute des infos sur les Actions actives
  {
    Bloc_Action::RegulationType RegulationEnCours = BlocsActions[i].Regulation;
    if (RegulationEnCours != Bloc_Action::INACTIF)
    { // Si la régulation n'est pas inactive

      int8_t Ouvre = 100 - Retard[i];
      int16_t Hequiv = int(100 * BlocsActions[i].Hequiv);
      S += String(i) + ES + BlocsActions[i].Nom + ES + String(Ouvre) + ES + String(Hequiv) + ES + BlocsActions[i].tOnOff + FS;
    }
  }
  RMS_NomEtatLocal = S;
}
void InfoActionExterne()
{ // Relevé périodique etat des actions internes et externes
}

void receptionWebSocket(
    WStype_t type,
    uint8_t *payload,
    size_t length)
{
  // Un message texte vient d'arriver
  if (type == WStype_TEXT)
  {

    // Décodage du JSON reçu
    JsonDocument doc;

    DeserializationError erreur =
        deserializeJson(doc, payload, length);

    if (erreur)
    {
      Serial.println("Erreur de décodage JSON");
      return;
    }

    // Lecture des valeurs

    String MessageType = doc["MessageType"] | "";
    if (MessageType == "PuissanceSources")
    {

      unsigned long localIPr = doc["localIP"].as<unsigned long>(); // IP de l'ESP reçu dans le message
      JsonArray arrS = doc["source"];
      JsonArray arrC = doc["canal"];

      // Il faut voir si ce message correspond à une source externe demandée par ce routeur
      for (int s = 0; s < NB_SOURCES_DEVICE_MAX; s++)
      {
        if (LaSource[s].Type == "Ext" && LaSource[s].IP == localIPr)
        {
          int NumVoieRecherche = LaSource[s].CanalVoie[0];
          for (size_t idx = 0; idx < arrS.size(); idx++)
          {
            int source = arrS[idx].as<int>();
            int canal = arrC[idx].as<int>();
            int NumVoie = source * NB_CANAUX_PAR_SOURCE + canal;
            if (NumVoieRecherche == NumVoie)
            {
              JsonArray arrPwS = doc["PwS"];
              JsonArray arrPwI = doc["PwI"];
              JsonArray arrPwS_Max = doc["PwS_Max"];
              JsonArray arrPwI_Max = doc["PwI_Max"];
              JsonArray arrPVAS = doc["PVAS"];
              JsonArray arrPVAI = doc["PVAI"];
              JsonArray arrEnJS = doc["EnJS"];
              JsonArray arrEnJI = doc["EnJI"];
              JsonArray arrEnTS = doc["EnTS"];
              JsonArray arrEnTI = doc["EnTI"];

              int8_t C0 = s * NB_CANAUX_PAR_SOURCE;
              PuissanceS[C0] = arrPwS[idx].as<int>();
              PuissanceI[C0] = arrPwI[idx].as<int>();
              PuissanceS_Max[C0] = arrPwS_Max[idx].as<int>();
              PuissanceI_Max[C0] = arrPwI_Max[idx].as<int>();
              PVAS[C0] = arrPVAS[idx].as<int>();
              PVAI[C0] = arrPVAI[idx].as<int>();
              EnergieJour_Soutiree[C0] = arrEnJS[idx].as<int>();
              EnergieJour_Injectee[C0] = arrEnJI[idx].as<int>();
              Energie_Soutiree[C0] = arrEnTS[idx].as<int>();
              Energie_Injectee[C0] = arrEnTI[idx].as<int>();

              String json;
              json.reserve(length);
              json.concat((const char *)payload, length);
              LaSourceVar[s].DataBrute = json;
            }
          }
        }
      }
      // Action externes
      for (int Idx_RMS = 0; Idx_RMS < LES_ROUTEURS_MAX; Idx_RMS++)
      {
        if (RouteurExt[Idx_RMS].IP == localIPr)
        {
         
          for (int i = 0; i < BLOCS_ACTION_MAX; i++) // ON RAZ les anciennes valeurs sur toutes les actions
          {
            RouteurExt[Idx_RMS].NomActionExterne[i] = "";
            RouteurExt[Idx_RMS].OuvertureActionExterne[i] = 0;
            RouteurExt[Idx_RMS].HequivActionExterne[i] = 0.0;
            RouteurExt[Idx_RMS].ValideActionExterne[i] = 0;
          }
          JsonObject objX = doc["lesActions"].as<JsonObject>();
          int NumAction = 0;
          JsonArray arrNom = objX["nom"];
          JsonArray arrHequiv = objX["Hequiv"];
          JsonArray arrOuverture = objX["ouverture"];
          for (size_t NumAction = 0; NumAction < arrNom.size(); NumAction++)
            if (NumAction < BLOCS_ACTION_MAX)
            {
              RouteurExt[Idx_RMS].NomActionExterne[NumAction] = arrNom[NumAction].as<String>();
              RouteurExt[Idx_RMS].HequivActionExterne[NumAction] = arrHequiv[NumAction].as<float>();
              RouteurExt[Idx_RMS].OuvertureActionExterne[NumAction] = arrOuverture[NumAction].as<int>();
              RouteurExt[Idx_RMS].ValideActionExterne[NumAction] = 5;
            }
        }
      }
    }
  }

  else if (type == WStype_CONNECTED)
  {
    EcranPrintln("WebSocket connecté");
  }

  else if (type == WStype_DISCONNECTED)
  {
    // Serial.println("WebSocket déconnecté");
  }
}