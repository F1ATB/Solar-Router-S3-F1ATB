// ***************
// *  WEB SERVER *
// ***************
#include "config.h"
#include "Communications/Server.h"
#include "Communications/WebSocket.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Cartes/carte.h"
#include <ETH.h>

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
#include "Communications/OpenMeteo.h"
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

void onWsEvent(AsyncWebSocket *server,
               AsyncWebSocketClient *client,
               AwsEventType type,
               void *arg,
               uint8_t *data,
               size_t len)
{

    if (type == WS_EVT_DATA)
    {
    }
}
void onWsEventAct(AsyncWebSocket *server,
                  AsyncWebSocketClient *client,
                  AwsEventType type,
                  void *arg,
                  uint8_t *data,
                  size_t len)
{

    if (type == WS_EVT_DATA)
    {
    }
}
void onWsEventBrute(AsyncWebSocket *server,
                    AsyncWebSocketClient *client,
                    AwsEventType type,
                    void *arg,
                    uint8_t *data,
                    size_t len)
{

    if (type == WS_EVT_DATA)
    {
    }
}
//===PAGE MAIN===
void envoyerPuissanceSources()
{
    if (ws.count() == 0)
        return; // Si aucun client connecté, on ne fait rien

    JsonDocument val;
    val["MessageType"] = "PuissanceSources";
    val["HeureCourante"] = HeureCourante();
    val["localIP"] = localIP;
    JsonArray arrSource = val["source"].to<JsonArray>();
    JsonArray arrCanal = val["canal"].to<JsonArray>();
    JsonArray arrCanalVoie = val["canalVoie"].to<JsonArray>();
    JsonArray arrNbC = val["nbCanal"].to<JsonArray>();
    JsonArray arrPwS = val["PwS"].to<JsonArray>();
    JsonArray arrPwI = val["PwI"].to<JsonArray>();
    JsonArray arrPVAS = val["PVAS"].to<JsonArray>();
    JsonArray arrPVAI = val["PVAI"].to<JsonArray>();
    JsonArray arrPwS_Max = val["PwS_Max"].to<JsonArray>();
    JsonArray arrPwI_Max = val["PwI_Max"].to<JsonArray>();
    JsonArray arrEnJS = val["EnJS"].to<JsonArray>(); // Energie Jour
    JsonArray arrEnJI = val["EnJI"].to<JsonArray>();
    JsonArray arrEnTS = val["EnTS"].to<JsonArray>(); // Energie totale
    JsonArray arrEnTI = val["EnTI"].to<JsonArray>();
    for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
    {
        int8_t nbCanal = 0;
        for (int8_t canalNum = 0; canalNum < NB_CANAUX_PAR_SOURCE; canalNum++)
        {
            if (LaSource[source].CanalVoie[canalNum] >= 0)
            {
                arrSource.add(source);
                arrCanal.add(canalNum);
                arrCanalVoie.add(LaSource[source].CanalVoie[canalNum]);
                nbCanal++;
                int8_t canal = source * 3 + canalNum;
                arrPwS.add(PuissanceS[canal]);
                arrPwI.add(PuissanceI[canal]);
                arrPVAS.add(PVAS[canal]);
                arrPVAI.add(PVAI[canal]);
                arrPwS_Max.add(PuissanceS_Max[canal]);
                arrPwI_Max.add(PuissanceI_Max[canal]);
                arrEnJS.add(EnergieJour_Soutiree[canal]); // Energie Jour
                arrEnJI.add(EnergieJour_Injectee[canal]);
                arrEnTS.add(Energie_Soutiree[canal]); // Energie totale
                arrEnTI.add(Energie_Injectee[canal]);
            }
        }
        if (nbCanal > 0)
        {
            arrNbC.add(nbCanal);
        }
    }

    // On rajoute état des Actions
    JsonObject valActions = val["lesActions"].to<JsonObject>();
    JsonArray arrOuverture = valActions["ouverture"].to<JsonArray>();
    JsonArray arrHequiv = valActions["Hequiv"].to<JsonArray>();
    JsonArray arrNom = valActions["nom"].to<JsonArray>();
    JsonArray arrOnOff = valActions["tOnOff"].to<JsonArray>();
    for (int i = 0; i < NbActions; i++)
    {
        arrOuverture.add(100 - Retard[i]);
        arrHequiv.add(BlocsActions[i].Hequiv);
        arrNom.add(BlocsActions[i].Nom);
        arrOnOff.add(BlocsActions[i].tOnOff);
    }

    String json;
    serializeJson(val, json);
    ws.textAll(json);
}

void envoyerDonneesLentes()
{

    if (ws.count() == 0)
        return; // Si aucun client connecté, on ne fait rien

    JsonDocument val;
    val["MessageType"] = "DonnéesLentes";
    val["HeureCourante"] = HeureCourante();
    val["localIP"] = localIP;
    JsonArray arrTemperature = val["Temperatures"].to<JsonArray>();
    for (int c = 0; c < 4; c++)
    {
        arrTemperature.add(temperature[c]);
    }
    val["LTARF"] = LTARF;
    String Tlendemain = "";
    if (STGEt == "4")
        Tlendemain = "BLEU";
    else if (STGEt == "8")
        Tlendemain = "BLANC";
    else if (STGEt == "C")
        Tlendemain = "ROUGE";
    val["TempoLendemain"] = Tlendemain;
    val["Soleil"] = JsonSoleil();
    String json;
    serializeJson(val, json);
    ws.textAll(json);
}

// ===PAGES ACTION===
void envoyerSortiesBlocs()
{
    if (wsAct.count() == 0)
        return; // Si aucun client connecté, on ne fait rien

    JsonDocument val;
    val["MessageType"] = "SortieBlocs";
    val["HeureCourante"] = HeureCourante();
    val["NbBlocs"] = NbBlocs;
    JsonArray arrSortieBin = val["SortieBin"].to<JsonArray>();
    JsonArray arrSortieNum0 = val["SortieNum0"].to<JsonArray>();
    JsonArray arrSortieNum1 = val["SortieNum1"].to<JsonArray>();
    for (int c = 0; c < NbBlocs; c++)
    {
        arrSortieBin.add(SortieBin[c]);
        arrSortieNum0.add(SortieNum0[c]);
        arrSortieNum1.add(SortieNum1[c]);
    }
    JsonArray arrP = val["P"].to<JsonArray>();
    JsonArray arrI = val["I"].to<JsonArray>();
    JsonArray arrD = val["D"].to<JsonArray>();
    JsonArray arrNumBloc = val["NumBloc"].to<JsonArray>();
    JsonArray arrLastErrorPw = val["LastErrorPw"].to<JsonArray>();

    for (int i = 0; i < NbActions; i++)
    {
        arrP.add(BlocsActions[i].P);
        arrI.add(BlocsActions[i].I);
        arrD.add(BlocsActions[i].D);
        arrLastErrorPw.add(LastErrorPw[i]);
        arrNumBloc.add(BlocsActions[i].NumBloc);
    }
    String json;
    serializeJson(val, json);
    wsAct.textAll(json);
}

// Websocket page Brute
void envoyerBruteMessages()
{

    if (wsBrute.count() == 0)
        return; // Si aucun client connecté, on ne fait rien
    JsonDocument val;
    val["MessageType"] = "BruteMessages";
    val["HeureCourante"] = HeureCourante();
    int j = idxMessage;
    for (int i = 0; i < 20; i++)
    {
        val["Message"][i] = MessageH[j];
        j = (j + 1) % 20;
    }
    String json;
    serializeJson(val, json);
    wsBrute.textAll(json);
}
void envoyerBruteSources()
{
    JsonDocument doc;
    doc["MessageType"] = "BruteSources";
    doc["HeureCourante"] = HeureCourante();
    JsonArray numSource = doc["numSource"].to<JsonArray>();
    JsonArray arrType = doc["Type"].to<JsonArray>();
    JsonArray arrData = doc["Data"].to<JsonArray>();
    JsonArray arrDataBrute = doc["DataBrute"].to<JsonArray>();

    for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
    {
        if (LaSource[source].CanalVoie[0] >= 0)
        {
            numSource.add(source);
            arrType.add(LaSource[source].Type);
            arrData.add(LaSource[source].Data);
            arrDataBrute.add(LaSourceVar[source].DataBrute);
        }
    }
    String S;
    serializeJson(doc, S);
    S.replace("\x1C", "\\u001c"); // FS : ASCII 28
    S.replace("\x1D", "\\u001d"); // GS : ASCII 29
    S.replace("\x1E", "\\u001e"); // RS : ASCII 30

    wsBrute.textAll(S);
}

void envoyerBrutesESP32()
{
    IT10ms = 0;
    IT10ms_in = 0;

    JsonDocument D;
    D["MessageType"] = "BruteESP32";
    D["HeureCourante"] = HeureCourante();
    float H = float(T_On_seconde) / 3600.0;
    D["Heure"] = HeureCourante();
    D["T_On"] = H;
    D["ESP32_Type"] = ESP32_Type;
    D["hostname"] = hostname;

    if (ESP32_Type >= CARTE_ETHERNET)
    {
        D["IPv4"] = ETH.localIP().toString();
        D["Mac"] = ETH.macAddress();
        D["gateway"] = ETH.gatewayIP().toString();
        D["masque"] = ETH.subnetMask().toString();
        D["IPv6local"] = ETH.linkLocalIPv6().toString();
        D["IPv6global"] = ETH.globalIPv6().toString();
    }
    else
    {
        D["RSSI"] = WiFi.RSSI();
        D["BSSID"] = WiFi.BSSIDstr();
        D["Mac"] = WiFi.macAddress();
        D["IPv4"] = WiFi.localIP().toString();
        D["IPv6global"] = WiFi.globalIPv6().toString();
        D["IPv6local"] = WiFi.linkLocalIPv6().toString();
        D["gateway"] = WiFi.gatewayIP().toString();
        D["masque"] = WiFi.subnetMask().toString();
        D["ssid"] = ssid;
    }

    D["coeur0"] = String(int(previousTimeRMSMin)) + ", " + String(int(previousTimeRMSMoy)) + ", " + String(int(previousTimeRMSMax));
    D["coeur1"] = String(int(previousLoopMin)) + ", " + String(int(previousLoopMoy)) + ", " + String(int(previousLoopMax));
    D["RAMfree"] = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    D["RAMbloc"] = heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);
    D["PSRAMfree"] = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    D["PSRAMbloc"] = heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
    // Mémoire Flash/LittleFS

    D["FileTotal"] = LittleFS.totalBytes(); // Espace total
    D["FileUsed"] = LittleFS.usedBytes();   // Espace utilisé
    delay(15);                              // Comptage interruptions
    if (IT10ms_in > 0)
    {
        D["IT_triac"] = String(IT10ms_in) + "/" + String(IT10ms);
    }
    else
    {
        D["IT_triac"] = "Pas de Triac";
    }
    if (ITmode > 0)
    {
        D["IT_Mode"] = "Secteur";
    }
    else
    {
        D["IT_Mode"] = "Horloge ESP";
    }
    D["Nbr_DS18B20"] = Nbr_DS18B20;
    D["AllTemp"] = AllTemp;

    int j = 0;
    for (int i = 0; i < NB_MACHINE_MAX; i++)
    {
        if (Machine[i].IP > 0 && Machine[i].IP != localIP)
        {
            D["MachineNom"][j] = Machine[i].nom + " (" + IP2String(Machine[i].IP) + ") ";
            D["MachineNote"][j] = String(Machine[i].Note) + "/" + String(Machine[i].NbCx);
            j++;
        }
    }
    String Json;
    serializeJson(D, Json);
    wsBrute.textAll(Json);
}
