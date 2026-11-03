//*******************************************************
//* Source EnPhase Envoy V5 ou V7   (Unique par Routeur *
//*******************************************************

#include "Sources/Source_EnphaseEnvoy.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include "config.h"
#include "Communications/comSortie.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "commonFX.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>
#include "Ecrans/Ecran.h"
#include "UrlEncode.h"
#include "Communications/Internet.h"
#include "FS.h"
#include "LittleFS.h"

// Paramètres for Enphase-Envoy-Smetered
String TokenEnphase = "";
String EnphaseUser = "";
String EnphasePwd = "";
String Session_id = "";

static String JsonToken = "";


WiFiClientSecure clientSecu;

void Setup_Enphase(bool NewToken , int8_t source);

uint32_t ipToInt(IPAddress ip)
{                                                                                                //
  return uint32_t(ip[0] << 24) | uint32_t(ip[1] << 16) | uint32_t(ip[2] << 8) | uint32_t(ip[3]); //
}
void SaveTokenEnphase()
{
  File f = LittleFS.open("/tokenenphase.json", FILE_WRITE);
  if (!f)
    return;
  String json = "{\"token\":\"" + TokenEnphase + "\"}";
  f.print(json);
  f.close();
}
void LoadTokenEnphase()
{
  File f = LittleFS.open("/tokenenphase.json", FILE_READ);
  if (!f)
    return;
  String json = f.readString();
  f.close();
  TokenEnphase = StringJson("token", json);
  // StockMessage("Token chargé (" + String(TokenEnphase.length()) + ")");
}
void Setup_Enphase(bool NewToken , int8_t source)
{
  //

  const char *host = "envoy"; //
  IPAddress envoyIP;
  if (LaSource[source].IP == 0)
  { // On a pas l'adresse. Recherche par mDNS                                                                          //
    if (!MDNS.begin(hostname))
    {                                                         // Init mDNS                                                                              //
      EcranPrintln("Erreur : impossible d'initialiser mDNS"); //
      return;                                                 //
    }
    else
    {                                                         //
      envoyIP = MDNS.queryHost(host, ENPHASE_READING_PERIOD); //
    } //
    if (envoyIP.toString() != "0.0.0.0")
    {                                                                                               //
      StockMessage("IP Enphase : http://" + String(host) + ".local" + " -> " + envoyIP.toString()); //
      LaSource[source].IP = ipToInt(envoyIP);                                                       // IP -> uint32                                                                         //
    }
    else
    {                                                              //
      StockMessage("Échec! passerelle Enphase envoy déconnectée"); //
      return;                                                      //
    }
  }
  // Chargement du token sauvegardé
  if (NewToken == false)
  {
    LoadTokenEnphase(); // ta fonction
    // Si le token existe encore et n'est pas expiré,
    // inutile d'aller contacter Enlighten.
    if (TokenEnphase.length() > TOKEN_MIN_LENGTH)
    {
      StockMessage("Token chargé depuis la mémoire (" + String(TokenEnphase.length()) + ")");
      Session_id = "Token chargé"; // Si vide affichage en page brute "Not Received from Enphase"
      return;
    }
  }
  // Obtention Session ID
  //********************
  const char *server1Enphase = "enlighten.enphaseenergy.com";
  String Host = String(server1Enphase);
  String adrEnphase = "https://" + Host + "/login/login.json";
  String requestBody = "user[email]=" + EnphaseUser + "&user[password]=" + urlEncode(EnphasePwd);

  if (EnphaseUser != "" && EnphasePwd != "" && LaSource[source].IP > 0)
  { // test envoyIP si perte de connexion //SR19
    EcranPrintln("Essai connexion  Enlighten server 1 pour obtention session_id!");
    clientSecu.setInsecure(); // skip verification
    if (!clientSecu.connect(server1Enphase, 443, TIMEOUT_CONNECT))
    {
      StockMessage("Connection failed to Enlighten server :" + Host);
      clientSecu.stop(); // MC002 à tester
      return;            // MC002 à tester
    }
    else
    {
      EcranPrintln("Connected to Enlighten server:" + Host);
      StockMessage("Connected to Enlighten server :" + Host);
      clientSecu.println("POST " + adrEnphase + "?" + requestBody + " HTTP/1.0");
      clientSecu.println("Host: " + Host);
      clientSecu.println("Connection: close");
      clientSecu.println();
      String line = "";
      while (clientSecu.connected())
      {
        line = clientSecu.readStringUntil('\n');
        if (line == "\r")
        {
          EcranPrintln("headers 1 Enlighten received");
          JsonToken = "";
        }

        JsonToken += line;
      }
      // if there are incoming bytes available
      // from the server, read them and print them:
      while (clientSecu.available())
      {
        char c = clientSecu.read();
        Serial.write(c);
      }
      clientSecu.stop();
    }
    Session_id = StringJson("session_id", JsonToken);
    EcranPrintln("session_id :" + Session_id);
    StockMessage("session_id :" + Session_id);
  }

  // Obtention Token
  //********************
  if (Session_id != "" && EnphaseSerial != "" && EnphaseUser != "")
  {
    const char *server2Enphase = "entrez.enphaseenergy.com";
    Host = String(server2Enphase);
    adrEnphase = "https://" + Host + "/tokens";
    requestBody = "{\"session_id\":\"" + Session_id + "\", \"serial_num\":" + EnphaseSerial + ", \"username\":\"" + EnphaseUser + "\"}";
    EcranPrintln("Essai connexion  Enlighten server 2 pour obtention token!");
    clientSecu.setInsecure(); // skip verification
    if (!clientSecu.connect(server2Enphase, 443, TIMEOUT_CONNECT))
    {
      StockMessage("Connection failed to :" + Host);
      clientSecu.stop(); // MC002 à tester
      return;            // MC002 à tester
    }
    else
    {
      EcranPrintln("Connected to :" + Host);
      StockMessage("Connected to :" + Host);
      clientSecu.println("POST " + adrEnphase + " HTTP/1.0");
      clientSecu.println("Host: " + Host);
      clientSecu.println("Content-Type: application/json");
      clientSecu.println("Content-Length:" + String(requestBody.length()));
      clientSecu.println("Connection: close");
      clientSecu.println();
      clientSecu.println(requestBody);
      clientSecu.println();
      EcranPrintln("Attente user est connecté");
      String line = "";
      JsonToken = "";
      while (clientSecu.connected())
      {
        line = clientSecu.readStringUntil('\n');
        if (line == "\r")
        {
          EcranPrintln("headers 2 enlighten received");
          JsonToken = "";
        }

        JsonToken += line;
      }
      // if there are incoming bytes available
      // from the server, read them and print them:
      while (clientSecu.available())
      {
        char c = clientSecu.read();
        Serial.write(c);
      }
      clientSecu.stop();
      JsonToken.trim();
      EcranPrintln("Token :" + JsonToken);
      // StockMessage("Token :" + JsonToken);
      if (JsonToken.length() > 50)
      {
        TokenEnphase = JsonToken;
        // MC001 Sauvegarde token
        SaveTokenEnphase();
        StockMessage("Nouveau token Enphase sauvegardé");
        // MC001
        previousTimeRMSMin = ENPHASE_READING_PERIOD;
        previousTimeRMSMax = 0;
        previousTimeRMSMoy = ENPHASE_READING_PERIOD;
        previousTimeRMS = millis();
        LastRMS_Millis[source] = millis();
        PeriodeProgMillis[source] = ENPHASE_READING_PERIOD;
      }
    }
  }
}

enum ReadStatus
{
  READ_OK,
  READ_INVALID_ARGUMENT,
  READ_TIMEOUT,
  READ_DISCONNECTED,
  READ_TOO_LONG
};

const char *ReadStatusToString(int status)
{
  switch (status)
  {
  case READ_OK:
    return "OK";
  case READ_INVALID_ARGUMENT:
    return "INVALID_ARGUMENT";
  case READ_TIMEOUT:
    return "TIMEOUT";
  case READ_DISCONNECTED:
    return "DISCONNECTED";
  case READ_TOO_LONG:
    return "TOO_LONG";
  default:
    return "UNKNOWN";
  }
}

int ReadBufferUntilChar( // Précedemment appelé JSONReadingEnphase
    NetworkClient &stream,
    char *out,
    size_t maxSize,
    size_t &outLen,
    char untilChar,
    unsigned long timeoutMs)
{
  if (maxSize == 0)
    return READ_INVALID_ARGUMENT;

  outLen = 0;
  out[0] = '\0';

  const size_t maxLen = maxSize - 1;
  unsigned long lastActivity = millis();

  while (true)
  {
    while (stream.available() > 0)
    {
      int c = stream.read();
      if (c < 0)
        break;

      lastActivity = millis();

      if ((char)c == untilChar)
      {
        out[outLen] = '\0';
        return READ_OK;
      }

      if (outLen >= maxLen)
      {
        out[outLen] = '\0';
        return READ_TOO_LONG;
      }

      out[outLen++] = (char)c;
    }

    if (!stream.connected() && stream.available() == 0)
    {
      out[outLen] = '\0';
      return READ_DISCONNECTED;
    }

    if ((unsigned long)(millis() - lastActivity) >= timeoutMs)
    {
      out[outLen] = '\0';
      return READ_TIMEOUT;
    }

    yield();
  }
}

uint32_t LectureEnphase(int8_t source)
{ // Lecture des consommations

  float Tension_M1, Tension_M2, Tension_M3; // Pour le triphasé
  float Intensite_M1, Intensite_M2, Intensite_M3;

  int8_t IdxM = Idx_Machine(LaSource[source].IP);
  int8_t C0 = source * 3; // Canal des données (Mono et Tri)
  int8_t C1 = C0 + 1;     // Canal des données (Tri)
  int8_t C2 = C0 + 2;     // Canal des données (Tri)

  // init variable
  static unsigned long g_nLastGoodReading = millis();
  uint32_t nTickReadingStart = millis();
  // unsigned long nLastTick;
  bool bJsonLoadingFinished = false;
  bool bTimeout = false;

  float PactReseau = 0.0f;
  float PvaReseau = 0.0f;
  long whDlvdCum = 0L; // on perd les decimals après la virgule avec un type long
  long whRcvdCum = 0L;
  if (TokenEnphase.length() <= TOKEN_MIN_LENGTH)
  { // MC001 Controle longueur du Token
    StockMessage("Token Enphase absent");
    Setup_Enphase(true,source);
    if (TokenEnphase.length() <= TOKEN_MIN_LENGTH)
      return millis() - nTickReadingStart; // retry on next loop with a new connection
  }

  String host = IP2String(LaSource[source].IP);
  String baseRequest;
  baseRequest = "/ivp/meters/readings HTTP/1.1\r\nHost: " + host + "\r\nAccept: application/json\r\nConnection: keep-alive\r\n";
  PowerFactor[C0] = 0.0f;

  if (TokenEnphase.length() > 50 && EnphaseUser != "")
  {
    // nLastTick = millis();
    if (!clientSecu.connected())
    {                           // établi la connexion
      clientSecu.stop();        // MC002 toujours repartir proprement
      delay(5);                 // MC002 toujours repartir proprement
      clientSecu.setInsecure(); // skip verification
      clientSecu.setTimeout(TIMEOUT_CONNECT);
      if (!clientSecu.connect(host.c_str(), 443))
      {
        // StockMessage("Connection failed to Envoy-S server! : https://" + String(host) + "timout=" + String(millis()-nLastTick));
        return millis() - nTickReadingStart; // on sort, pas de comm avec le server enphase
      }
      // StockMessage("Connected to Envoy-S server HTTPS!"+ String(host) + "timout=" + String(millis()-nLastTick));
    }

    clientSecu.println("GET " + baseRequest + "Authorization: Bearer " + TokenEnphase + "\r\n\r\n");

    static char receivedDataBuf[800]; // 'static' pour en faire une variable globale et eviter de saturer la pile
    size_t statusLineLen = 0;
    int result = 0;

    do
    {
      // on consomme le buffer jusqu'à la premiere ligne d'une nouvelle reponse HTTP (contenant HTTP/),
      // au cas où il restait du buffer non consommé de la precedente requete, recu entre-temps).
      result = ReadBufferUntilChar(clientSecu, receivedDataBuf, sizeof(receivedDataBuf), statusLineLen, '\n', TIMEOUT_WAITING_ANSWER);
    } while (!strstr(receivedDataBuf, "HTTP/") && result == READ_OK);
    // Cette ligne devrait alors contenir le HTTP response code (200, 401...)
    if (!strstr(receivedDataBuf, "HTTP/"))
    {
      if (result == READ_DISCONNECTED)
      {
        StockMessage("Envoy connection closed before sending any HTTP response. Retrying new connection...");
      }
      // else
      // {
      // StockMessage(String("Envoy error while reading HTTP response status, status= ") + ReadStatusToString(result) + ", partialLen=" + statusLineLen);
      // }
      clientSecu.stop();
      return millis() - nTickReadingStart; // retry on next loop with a new connection
    }
    if (!strstr(receivedDataBuf, "200"))
    {
      if (strstr(receivedDataBuf, "401"))
      { // MC001 pour gérer un nouveau Token si plus correct ou dépassé
        StockMessage("Bearer Token expired");
        clientSecu.stop();
        Setup_Enphase(true,source);                 // récupère un nouveau token
        return millis() - nTickReadingStart; // retry on next LectureEnphase() call;
      }
      // toute autre erreur n'est pas récupérable, on sort de la fonction
      StockMessage(String("Envoy refused request: receivedDataBuf=[") + receivedDataBuf + "]");
      clientSecu.stop();
      return millis() - nTickReadingStart; // retry on next LectureEnphase() call
    }

    int nGlobalIndex = 0;
    int nPhaseIndex = 0;
    bool bMonoPhase = true;
    size_t jsonPayloadLength = 0;
    int status;

    // Saute L'entete d'ouverture de la trame JSON.
    status = ReadBufferUntilChar(clientSecu, receivedDataBuf, sizeof(receivedDataBuf), jsonPayloadLength, '[', TIMEOUT_READ_PAYLOAD);
    if (status != READ_OK)
    {
      // StockMessage(String("Envoy JSON Reading 1 failed, status= ") + ReadStatusToString(status) + ", partialLen=" + jsonPayloadLength);
      clientSecu.stop();
      return millis() - nTickReadingStart;
    }

    for (nGlobalIndex = 0; (nGlobalIndex < 8) && !bJsonLoadingFinished && !bTimeout; nGlobalIndex++)
    {
      // Read Global Topic
      status = ReadBufferUntilChar(clientSecu, receivedDataBuf, sizeof(receivedDataBuf), jsonPayloadLength, '[', TIMEOUT_READ_PAYLOAD);
      if (status != READ_OK)
      {
        // StockMessage(String("Envoy JSON Reading 2 failed, status= ") + ReadStatusToString(status) + ", partialLen=" + jsonPayloadLength);
        clientSecu.stop();
        bTimeout = true;
        continue;
      }
      // delay(1);

      if (nGlobalIndex == 0)
      {
        // StockMessage(receivedDataBuf);
        float tension = ValJson("voltage", receivedDataBuf);
        long eid = LongJson("eid", receivedDataBuf);

        // StockMessage("Tension Global0 ="+String(tension));

        if (tension > 280.0f)
          bMonoPhase = false;

        // StockMessage("bMonoPhase ="+String(bMonoPhase));

        if (!bMonoPhase)
        {
          PactProd = ValJson("activePower", receivedDataBuf);
          Tension[C0] = ValJson("voltage", receivedDataBuf);
          Intensite[C0] = ValJson("current", receivedDataBuf);
        }
      }
      else if (nGlobalIndex == 1)
      {
        if (!bMonoPhase)
        {
          PactReseau = ValJson("activePower", receivedDataBuf);
          PactConso_M = PactReseau + PactProd; // dans l'hypothese qu'il n'y a pas de l'énergie fournit par une batterie !
          PvaReseau = ValJson("apparentPower", receivedDataBuf);
          whDlvdCum = ValJson("actEnergyDlvd", receivedDataBuf);
          whRcvdCum = ValJson("actEnergyRcvd", receivedDataBuf);
          Frequence_[C0] = ValJson("freq", receivedDataBuf);
        }
      }

      for (nPhaseIndex = 0; (nPhaseIndex < 3) && !bJsonLoadingFinished && !bTimeout; nPhaseIndex++)
      {
        // Read Phase
        status = ReadBufferUntilChar(clientSecu, receivedDataBuf, sizeof(receivedDataBuf), jsonPayloadLength, '}', TIMEOUT_READ_PAYLOAD);
        if (status != READ_OK)
        {
          if (nPhaseIndex == 2) // MC003
            StockMessage(String("Envoy JSON Reading 3 failed, status= ") + ReadStatusToString(status) + ", partialLen=" + jsonPayloadLength);
          clientSecu.stop();
          bTimeout = true;
          continue;
        }
        receivedDataBuf[jsonPayloadLength - 1] = '}';
        // delay(1);

        if ((nGlobalIndex == 0) && (nPhaseIndex == 0))
        {
          if (bMonoPhase)
          {
            PactProd = ValJson("activePower", receivedDataBuf);
          }
        }
        else if ((nGlobalIndex == 1) && (nPhaseIndex == 0))
        {
          Tension_M1 = ValJson("voltage", receivedDataBuf);
          Intensite_M1 = ValJson("current", receivedDataBuf);

          if (bMonoPhase)
          {
            // StockMessage(receivedDataBuf);
            PactReseau = ValJson("activePower", receivedDataBuf);
            PactConso_M = PactReseau + PactProd; // dans l'hypothese qu'il n'y a pas de l'énergie fournit par une batterie !
            PvaReseau = ValJson("apparentPower", receivedDataBuf);
            whDlvdCum = ValJson("actEnergyDlvd", receivedDataBuf);
            whRcvdCum = ValJson("actEnergyRcvd", receivedDataBuf);
            Frequence_[C0] = ValJson("freq", receivedDataBuf);
          }
        }
        else if ((nGlobalIndex == 1) && (nPhaseIndex == 1))
        {
          Tension_M2 = ValJson("voltage", receivedDataBuf);
          Intensite_M2 = ValJson("current", receivedDataBuf);
        }
        else if ((nGlobalIndex == 1) && (nPhaseIndex == 2))
        {
          Tension_M3 = ValJson("voltage", receivedDataBuf);
          Intensite_M3 = ValJson("current", receivedDataBuf);

          bJsonLoadingFinished = true;

          g_nLastGoodReading = millis();
        }
      }
    }
  }
  clientSecu.stop();

  if (!bJsonLoadingFinished)
  {
    // Protection contre les mauvaises lectures qui perdureraient plus de 10s !!!
    if ((millis() - g_nLastGoodReading) > 10000)
    {
      PactProd = 0.0f;
      PactConso_M = 0;
      PactReseau = 0.0f;
      PactConso_M = 0.0f;
      Frequence_[C0] = 0.0f;
      Tension_M1 = 0.0f;
      Tension_M2 = 0.0f;
      Tension_M3 = 0.0f;
      Intensite_M1 = 0.0f;
      Intensite_M2 = 0.0f;
      Intensite_M3 = 0.0f;
    }
    Tension[C0] = Tension_M1;
    Tension[C1] = Tension_M2;
    Tension[C2] = Tension_M3;
    Intensite[C0] = Intensite_M1;
    Intensite[C1] = Intensite_M2;
    Intensite[C2] = Intensite_M3;
    // TelnetPrintln("JSON Loading failed");
    // StockMessage("JSON Loading failed");
    return millis() - nTickReadingStart;
  }

  PactReseau = PfloatMax(PactReseau);
  if (PactReseau < 0)
  {
    PuissanceS_inst[C0] = 0;
    PuissanceI_inst[C0] = int(-PactReseau);
  }
  else
  {
    PuissanceI_inst[C0] = 0;
    PuissanceS_inst[C0] = int(PactReseau);
  }
  PvaReseau = PfloatMax(PvaReseau);
  if (PactReseau < 0)
  {
    PVAS_inst[C0] = 0;
    PVAI_inst[C0] = int(PvaReseau);
  }
  else
  {
    PVAI_inst[C0] = 0;
    PVAS_inst[C0] = int(PvaReseau);
  }
  PVA_Valide[C0] = true;
  Filtre_Puissance(true, C0);

  if ((PVA_moy[C0]) != 0)
  {
    PowerFactor[C0] = floor(100.0f * fabsf(Puissance_moy[C0]) / PVA_moy[C0]) / 100.0f;
    PowerFactor[C0] = min(PowerFactor[C0], 1.0f);
  }

  if (whDlvdCum != 0)
  {
    if (LastwhDlvdCum == 0)
      LastwhDlvdCum = whDlvdCum;
    long DeltaWhSoutire = whDlvdCum - LastwhDlvdCum;
    LastwhDlvdCum = whDlvdCum;
    if (DeltaWhSoutire > 0)
    {
      Energie_Soutiree[C0] += DeltaWhSoutire;
    }
  }

  if (whRcvdCum != 0)
  {
    if (LastwhRcvdCum == 0)
      LastwhRcvdCum = whRcvdCum;
    long DeltaWhInjecte = whRcvdCum - LastwhRcvdCum;
    LastwhRcvdCum = whRcvdCum;
    if (DeltaWhInjecte > 0)
    {
      Energie_Injectee[C0] += DeltaWhInjecte;
    }
  }

  CalculEnergieJour(C0, true);

  return millis() - nTickReadingStart;
}
