

#include <Arduino.h>
#include <esp_wps.h>
#include <AsyncTCP.h>
#include <ETH.h>
#include <ESPAsyncWebServer.h>
#include <SPI.h>
#include <WiFi.h>
#include "Communications/Internet.h"
#include "config.h"
#include "Ecrans/Ecran.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_sntp.h"
#include <ESPmDNS.h>
#include "commonFX.h"
#include "Cartes/carte.h"
#include "Stockage.h"
#include "RMS_Externes.h"
#include "Communications/comSortie.h"
#include "Ecrans/LEDs.h"
#include "Cartes/carte.h"
#include <HTTPClient.h>
#include "Heure.h"

// WIFI
int WIFIbug = 0;
int16_t ComSurv = 6; // Timeout sans Wifi par pas de 30s
byte WifiSleep = 1;

String Liste_AP = "";
uint8_t bestBSSID[6]; // Meilleur en dBm adresse MAC
unsigned long Gateway = 0;
unsigned long masque = 4294967040;
unsigned long dns = 0;
// VARIABLES
const char *ap_default_ssid;       // Mode Access point  IP: 192.168.4.1
const char *ap_default_psk = NULL; // Pas de mot de passe en AP,

unsigned long String2IP(String S);
String Format_WiFi(int num, const String &nom, int niveau, const String &MAC, int channel);
bool Liste_WIFI();

/*** WPS Configurations ***/                                         //
#define ESP_WPS_MODE WPS_TYPE_PBC                                    //
esp_wps_config_t wps_config = WPS_CONFIG_INIT_DEFAULT(ESP_WPS_MODE); //
bool isGOT_IP = false;

static bool eth_connected = false; // true si IP reçue                                                            //

void wpsStop()
{                                           //
    esp_err_t err = esp_wifi_wps_disable(); //
    if (err != ESP_OK)
    {                                                                                             //
        SeriePrintln("WPS Disable Failed: " + String(err, HEX) + "h -> " + esp_err_to_name(err)); //
    } //
} //

// Evènements WPS/WiFi                                                                                //
void WiFiEvent(WiFiEvent_t event)
{ //
    switch (event)
    {                                                                                          //
    case ARDUINO_EVENT_WIFI_STA_START:                                                         //
        SeriePrintln("WiFi Démarré en Mode Station. ");                   //
        break;                                                                                 //
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:                                                        //
        SeriePrintln("\rWiFi : " + String(WiFi.SSID()) + " connecté");                  //
        ssid = (WiFi.SSID());                                                                  // Récup ssid                                                             //
        SeriePrintln("Récupération IP de " + hostname + " -> " + (WiFi.localIP().toString())); //
        SeriePrintln("Récupération password -> " + String(WiFi.psk()));                        //
        password = (WiFi.psk());                                                               // Récup password                                                      //
        isGOT_IP = true; /*IP reçue*/                                                          //
        break;                                                                                 //
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:                                                  //
        SeriePrintln("Déconnecté. Redémarrage WiFi...");                                       //
        WiFi.disconnect();                                                                     // Forçage reconnexion normale                                               //
        delay(2000);                                                                             //
        WiFi.reconnect();                                                                      //
        delay(100);                                                                            //
        isGOT_IP = false;    
        WIFIbug++;  
        if (WIFIbug > ComSurv)
          { // Timeout sans WIFI Reset
            EcranPrintln("Redémarrage pour problème WIFI...");
            delay(5000);
            ESP.restart();
          }                                                                // Réinitialiser l'événement si déconnecté                                    //
        break;                                                                                 //
    case ARDUINO_EVENT_WPS_ER_SUCCESS:                                                         //
        SeriePrintln("WPS réussi! Stop WPS et connexion vers: " + String(WiFi.SSID()));        //
        wpsStop();                                                                             // Must disable WPS before connecting                                                //
        WiFi.begin();                                                                          // Connect using credentials from WPS                                             //
        break;                                                                                 //
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:                                                     //
        SeriePrintln("\rWiFi Reconnecté en Mode Station");                                       //
        break;
    case ARDUINO_EVENT_ETH_START:
        SeriePrintln("ETH Started");
        // set eth hostname here
        ETH.setHostname(hostname.c_str()); 
        break;
    case ARDUINO_EVENT_ETH_CONNECTED:
        SeriePrintln("ETH Connected");
        break;
    case ARDUINO_EVENT_ETH_GOT_IP:
        SeriePrint("ETH MAC: ");
        SeriePrint(ETH.macAddress());
        SeriePrint(", IPv4: ");
        SeriePrint(ETH.localIP().toString());
        localIP = String2IP(ETH.localIP().toString());
        if (ETH.fullDuplex())
        {
            SeriePrint(", FULL_DUPLEX");
        }
        SeriePrint(", ");
        SeriePrint(String(ETH.linkSpeed()));
        SeriePrintln("Mbps");
        eth_connected = true;
        break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
        SeriePrintln("ETH Disconnected");
        eth_connected = false;
        break;
    case ARDUINO_EVENT_ETH_STOP:
        SeriePrintln("ETH Stopped");
        eth_connected = false;
        break; //
    default:   //
        break; //
    } //
}

// ***********************************
// INIT INTERNET
// Configure adresses WIFI ou Ethernet
// ***************************
void Init_Internet()
{
    byte mac[6]; // the MAC address of your Wifi shield
    WiFi.macAddress(mac);
    Serial.print("Mac::");
    for (int i = 0; i < 6; i++)
    {
        if (mac[i] < 16)
            Serial.print("0");
        Serial.print(mac[i], HEX);
        if (i < 5)
            Serial.print(":");
    }
    Serial.println();

    IP2String(localIP);
    // Set youRMS_IP[0]c IP address
    IPAddress local_IP(arrIP[3], arrIP[2], arrIP[1], arrIP[0]);
    EcranPrint("Adresse IP en mémoire : ");
    EcranPrintln(local_IP.toString());
    Ecran_Loop();
    // Set your Gateway IP address
    IP2String(Gateway);
    IPAddress gateway(arrIP[3], arrIP[2], arrIP[1], arrIP[0]);
    // Set your masque/subnet IP address
    IP2String(masque);
    IPAddress subnet(arrIP[3], arrIP[2], arrIP[1], arrIP[0]);
    // Set your DNS IP address
    IP2String(dns);
    IPAddress primaryDNS(arrIP[3], arrIP[2], arrIP[1], arrIP[0]); // optional
    IPAddress secondaryDNS(8, 8, 4, 4);
    if (hostname == "")
    {
        hostname = String(HOSTNAME);
        uint32_t chipId = 0;
        for (int i = 0; i < 17; i = i + 8)
        {
            chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
        }
        hostname += String(chipId); // Add chip ID to hostname
    }
    EcranPrintln("Hostname : " + hostname);
    WiFi.hostname(hostname);
    WiFi.onEvent(WiFiEvent);
    bool bestWifi = false;
    if (ESP32_Type == CARTE_ETHERNET)
    { // Ethernet (avant Horloge)
        EcranPrintln("Lancement de la liaison Ethernet");
        Ecran_Loop();
        // if ????????
        delay(100);
        
        if (!ETH.begin(ETH_PHY_W5500, 1, ETH_CS_PIN, ETH_INT_PIN, ETH_RST_PIN,
                       SPI3_HOST,
                       ETH_SCLK_PIN, ETH_MISO_PIN, ETH_MOSI_PIN))
        {
            EcranPrintln("ETH start Failed!");
        }
        EcranPrintln("Starting Ethernet...");
        EcranPrintln("Câble ethernet : " + String(ETH.linkUp() == 1 ? "Connecté" : "Déconnecté"));
        while (!eth_connected)
        {
            EcranPrintln("Wait for network connect ...");
            delay(500);
        }

        EcranPrintln("\nConnected !");
        EcranPrint("IP: ");
        EcranPrintln(ETH.localIP().toString());
        StockMessage("Adresse IP Ethernet fixe : " + ETH.localIP().toString()); 
        localIP = String2IP(ETH.localIP().toString());
        unsigned long Start = millis(); //???????????????????
    }
    else
    { // ESP32 en WIFI
        EcranPrintln("Lancement du Wifi");
        // Liste Wifi à faire avant connexion à un AP. Necessaire depuis biblio ESP32 3.0.1
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL);
        WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);
        bestWifi = Liste_WIFI();
        EcranPrint("Version : ");
        EcranPrintln(Version);
        LireSerial();
        ap_default_ssid = (const char *)hostname.c_str();
        // Check WiFi connection
        // ... check mode
        if (WiFi.getMode() != WIFI_STA)
        {
            WiFi.mode(WIFI_STA);
            delay(10);
        }
    }
    DefFuseauHoraire();

    // WIFI
    if (ESP32_Type != CARTE_ETHERNET)
    {
        if (ModeReseau < 2)
        {
            EcranPrintln("ssid:" + ssid);
            EcranPrintln("password:" + password);
            if (ssid.length() > 0)
            {
                if (dhcpOn == 0)
                {
                    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
                    {
                        EcranPrintln("WIFI STA Failed to configure");
                    }
                }
                StockMessage("Wifi Begin : " + ssid);
                if (bestWifi)
                {
                    WiFi.begin(ssid.c_str(), password.c_str(), 0, bestBSSID); // Connexion forcée au BSSID choisi
                }
                else
                {
                    WiFi.begin(ssid.c_str(), password.c_str());
                }

                WiFi.setSleep(WifiSleep);
                while (WiFi.status() != WL_CONNECTED && (millis() - startMillis < 40000))
                { // Attente connexion au Wifi
                    EcranPrint(".");
                    Ecran_Loop();
                    Gestion_LEDs();
                    EcranPrint(String(WiFi.status()));
                    LireSerial();
                    delay(300);
                }
                EcranPrintln("");
            }
        }
        if (WiFi.status() == WL_CONNECTED && ModeReseau < 2)
        {
            //RMS_IP[0] = String2IP(WiFi.localIP().toString());
            localIP = String2IP(WiFi.localIP().toString());
            Machine[0].IP = localIP;
            StockMessage("🛜 Connecté par WiFi, addresse IP : " + WiFi.localIP().toString() + " or <a href='http://" + hostname + ".local' >" + hostname + ".local</a>");
        }
        else
        {
            /*** WPS SETUP ***/ //
            if (WiFi.scanNetworks() != 0 && WiFi.RSSI(0) > -83)
            {                                 // WPS inutile si aucun signal WiFi > -83dBm                                                                        //
                WiFi.disconnect(false, true); // RAZ config.                                                                                                                             //
                delay(100);                   //
                WiFi.mode(WIFI_AP_STA);       //
                delay(10);                    //
                EcranPrintln("Tentative de connexion via WPS...");
                Ecran_Loop();                     //
                                                  //
                delay(10);                        //
                esp_wifi_wps_enable(&wps_config); //
                esp_wifi_wps_start(0);            //
                startMillis = millis();           //
                while (isGOT_IP != true && (millis() - startMillis < 20000))
                {                   // Attente évènement "GOT_IP" pour récupération: ssid, password et IP                                    //
                    Gestion_LEDs(); //
                    delay(300);     //
                } //
                if (WiFi.status() == WL_CONNECTED && ModeReseau < 2)
                {                   // Remplacement de RMS_IP[0] par localIP
                    localIP = String2IP(WiFi.localIP().toString());     //
                    Machine[0].IP = localIP; //
                    // Go into software AP and STA modes.                                                                                                                                //
                    StockMessage("🛜 Connecté par WiFi et WPS, IP : " + WiFi.localIP().toString() + " nom d'hôte : <a href='http://" + hostname + ".local'>" + hostname + ".local</a>"                                                                     //
                                                                                                                                                                           " Copiez/collez le nom d'hôte dans votre navigateur. ESP32 en mode AP et STA."); //
                    Ecran_Loop();
                    LireSerial();                                 //
                    WiFi.softAP(ap_default_ssid, ap_default_psk); // on entre en mode AP pour enregistrer et lancer le RMS                                                                //
                }
                else
                { //
                    StockMessage("😩 Echec de connexion WiFi via WPS. ESP32 en mode AP et STA.");
                    // Go into software AP and STA modes.                                                                                                                                //
                    LireSerial();                                 //
                    WiFi.softAP(ap_default_ssid, ap_default_psk); //
                    infoSerie();
                    Ecran_Loop(); //
                } //
            }
            else
            {
                StockMessage("😧 Pas de connexion WIFI. ESP32 en mode AP et STA.");
                // Go into software AP and STA modes.
                // WiFi.disconnect();
                delay(100);
                WiFi.mode(WIFI_AP_STA);
                delay(10);
                LireSerial();
                Ecran_Loop();
                WiFi.softAP(ap_default_ssid, ap_default_psk);
                infoSerie();
            }
            WiFi.scanDelete(); //
        }
    }
}

bool Liste_WIFI()
{ // Doit être fait avant toute connection WIFI depuis biblio ESP32 3.0.1
    int bestNetworkDb = -1000;
    bool bestFound = false;
    WIFIbug = 0;
    ResetWatchDog();
    int n = 0;
    // WiFi.disconnect();
    delay(100);
    EcranPrintln("Scan start");
    // WiFi.scanNetworks will return the number of networks found.
    n = WiFi.scanNetworks();
    EcranPrintln("Scan done");
    Liste_AP = "";
    if (n <= 0)
    {
        EcranPrintln("Pas de réseau Wifi trouvé");
    }
    else
    {
        EcranPrint(String(n));
        EcranPrintln(" réseaux trouvés");
        EcranPrintln("|Nr|          SSID              |   RSSI  |       MAC       | Channel |");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            EcranPrintln(Format_WiFi(i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.BSSIDstr(i), WiFi.channel(i)));
            Liste_AP += WiFi.SSID(i).c_str() + RS + String(WiFi.RSSI(i)) + RS + WiFi.BSSIDstr(i).c_str() + RS + String(WiFi.channel(i)) + GS;
            if (WiFi.SSID(i) == ssid)
            {
                if (WiFi.RSSI(i) > bestNetworkDb)
                {
                    bestNetworkDb = WiFi.RSSI(i);
                    memcpy(bestBSSID, WiFi.BSSID(i), 6);
                    bestFound = true;
                }
            }
        }
    }
    WiFi.scanDelete();
    return bestFound;
}
String Format_WiFi(int num, const String &nom, int niveau, const String &MAC, int channel)
{
    char value[100];
    sprintf(value, "|%2d|%-28s|%4d dBm |%-16s|%9d|", num, nom.c_str(), niveau, MAC.c_str(), channel);
    return String(value);
}

// IP
byte arrIP[4];
String IP2String(unsigned long IP)
{
    byte arr[4];
    arr[0] = IP & 0xFF;         // 0x78
    arr[1] = (IP >> 8) & 0xFF;  // 0x56
    arr[2] = (IP >> 16) & 0xFF; // 0x34
    arr[3] = (IP >> 24) & 0xFF; // 0x12
    for (int i = 0; i < 4; i++)
    {                      // Pour eviter corruption données entre coeur 0 et coeur1 (arr en variable local)
        arrIP[i] = arr[i]; // Pour le WIFI au setup
    }
    return String(arr[3]) + "." + String(arr[2]) + "." + String(arr[1]) + "." + String(arr[0]);
}
unsigned long String2IP(String S)
{
    unsigned long IP = 0;
    for (int i = 0; i < 3; i++)
    {
        int p = S.indexOf(".");
        String s = S.substring(0, p);
        S = S.substring(p + 1);
        IP += s.toInt();
        IP = IP * 256;
    }
    IP += S.toInt();
    return IP;
}
// *************************
// * Appel Serveur Externe *
//**************************
// retourne le contenu d'une interrogation htpp // chaine vide si problem
// pour les cas https et POST [Enphase & RTE], c'est geré localement avec l'ajout de headers necessaire
// pour les cas sans utilisation du retour c'est geré en local
// url commence par http
// IdxM = x pour la mesure de qualité échange entre ESP ou autre IP , -1 si pas de notation [incremente de +1 RMS_NbCx lors de la tentatative, increment de +1 RMS_Note si réussite ou -1 si echec / limite le compteur a 100 ]
String httpGetPayload(String url, int IdxM = -1)
{
    if (IdxM > 0)
    {
        if (Machine[IdxM].NbCx < 100)
        {
            Machine[IdxM].NbCx++; // + 1 tentative de connexion
        }
        else
        {
            Machine[IdxM].NbCx = 2; // remet les compteurs au debut
            Machine[IdxM].Note = 1;
        }
    }
    String reponse = "";
    
    WiFiClient client;
    HTTPClient http;
    if (http.begin(client, url.c_str()))
    {
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK)
        { // negatif si echec
            reponse = http.getString();
        }
        else
        { // echec
            if (IdxM > 0 && Machine[IdxM].Note > 0)
                Machine[IdxM].Note--; // echec, on décremente la note
            
            reponse = "";
        }
    }
    else
    { // echec
        StockMessage("Connection failed to :" + url);
        if (IdxM > 0 && Machine[IdxM].Note > 0)
            Machine[IdxM].Note--; // echec, on décremente la note
        // log_i("http begin for %s fail", url.c_str());
        reponse = "";
    }
    client.stop(); // on arret même si echec
    http.end();    // on termine même si echec
    // reponse.trim();  // supprime espaces et caracteres de controle en debut et fin
    // log_i("Result for %s\n|%s|", url.c_str(), reponse.c_str());
    if (reponse != "" && IdxM > 0 && Machine[IdxM].Note < 100)
        Machine[IdxM].Note++; // + 1 point pour la note
    return reponse;
}

Reseau Machine[NB_MACHINE_MAX];

void Init_Machines()
{
    for (int8_t i = 0; i < NB_MACHINE_MAX; i++)
    {
         Machine[i].nom = "";
         Machine[i].IP = 0;
         Machine[i].NbCx = 0;
         Machine[i].Note = 0;
         Machine[i].RMS = false;
    }
    
}
int8_t Idx_Machine(unsigned long ip)
{
    for (int8_t i = 0; i < NB_MACHINE_MAX; i++)
    {
        if (Machine[i].IP == ip)
        {
            return i;
        }
        if (Machine[i].IP == 0)
        {
            Machine[i].IP = ip;
            return i;
        }
    }
    return -1; // N'arrive jamais
}