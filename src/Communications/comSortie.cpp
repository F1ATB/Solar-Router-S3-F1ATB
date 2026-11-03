#include "Communications/comSortie.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include "config.h"
#include "Ecrans/Ecran.h"
#include "Ecrans/LEDs.h"
#include "Puissance.h"
#include "Cartes/carte.h"
#include "commonFX.h"




byte pSerial1 = 0,pSerial2=0;             //Choix Pin port serie
int8_t RXD1 = -1, TXD1 = -1;  //Port serie1
int8_t RXD2 = -1, TXD2 = -1;  //Port serie2
static int8_t RX1_[] = { -1, 18 };
static int8_t TX1_[] = { -1, 17 };
static int8_t RX2_[] = { -1, 6 };
static int8_t TX2_[] = { -1, 7 };
static String SerialIn = "";


static bool dispPw = false;   //Affiche  Power sur serial et Telnet
static bool dispAct = false;  //Affiche  Ouverture Actions sur serial et Telnet


//Messages
EXT_RAM_BSS_ATTR String  MessageH[20];
int idxMessage = 0;

//Prototypes
void DecodeSerial();
void MessageCommandes() ;

// === Serveur Telnet ===
WiFiServer telnetServer(23);  //Port Telnet 23
WiFiClient telnetClient;

void InitGpioSerial(){
if (pSerial1 > 0) {
    RXD1 = RX1_[pSerial1];  //Port serie 1
    TXD1 = TX1_[pSerial1];
    Serial1.setRxBufferSize(SER_BUF_SIZE);
  }
  if (pSerial2 > 0) {
    RXD2 = RX2_[pSerial2];  //Port serie 2
    TXD2 = TX2_[pSerial2];
    Serial2.setRxBufferSize(SER_BUF_SIZE);
  }
}

void InfoVersSerie(){//Sortie vers port Série ou Telnet
  if (dispPw || dispAct) {
    String dispOut = "???????";
    
    SeriePrintln(dispOut);
  }
}
void infoSerie() {
  EcranPrintln("Access Point Mode : " + hostname);
  EcranPrint("IP address: ");
  EcranPrintln(WiFi.softAPIP().toString());
  EcranPrintln("\nPar le port série vous pouvez définir le WIFI à utiliser par l'ESP32 en tapant les 3 commandes ci-dessous en remplaçant xxx par la bonne valeur :");
  EcranPrintln("ssid:xxx");
  EcranPrintln("password:xxx");
  EcranPrintln("restart");
}


// PORT SERIE ou TELNET
void LireSerial(void) {
  int inbyte;
  //Port Serie USB/CDC
  while (Serial.available() > 0) {
    inbyte = Serial.read();
    if ((inbyte == 10) || (inbyte == 13)) {
      DecodeSerial();
    } else {
      Serial.print(char(inbyte));
      SerialIn += String(char(inbyte));
    }
  }
  //Port Serie0
  while (Serial0.available() > 0) {
    inbyte = Serial0.read();
    if ((inbyte == 10) || (inbyte == 13)) {
      DecodeSerial();
    } else {
      Serial0.print(char(inbyte));
      SerialIn += String(char(inbyte));
    }
  }
  //Telnet
  if (telnetClient && telnetClient.connected() && telnetClient.available()) {
    char c = telnetClient.read();  // Lire caractère
    if (c == '\n' || c == '\r') {  // Fin de ligne → commande complète
      DecodeSerial();
    } else {
      SerialIn += c;  // Ajouter caractère au buffer
    }
  }
}
void DecodeSerial() {
  String sw;
  String valeur = "";
  int p;
  SerialIn.trim();
  SeriePrintln(SerialIn);
  p = SerialIn.indexOf(":");
  if (p > 0) {
    sw = SerialIn.substring(0, p+1);
    valeur = SerialIn.substring(p + 1);
    sw.trim();
    valeur.trim();
  } else {
    sw = SerialIn;
  }

  if (sw.indexOf("restart") >= 0) {
    SeriePrintln(">>" + SerialIn);
    delay(200);
    ESP.restart();
  }
  if (sw.indexOf("ssid:") >= 0) {
    ssid = valeur;
    ModeReseau = 0;          // A priori
    dhcpOn = 1;              //Au cas ou l'on change de mapping des adresses LAN
    if (ESP32_Type >= CARTE_ETHERNET) {  //Carte Ethernet
     // ESP32_Type = 0; ?????????????????????
     SeriePrintln(">>" + SerialIn);
    }
    SauveConfig();
  }
  if (sw.indexOf("password:") >= 0) {
    password = valeur;
    SeriePrintln(">>" + SerialIn);
    SauveConfig();
  }
  if (sw.indexOf("T-ETH") >= 0) {
    ESP32_Type = CARTE_ETHERNET;
    SeriePrintln(">>" + SerialIn);
    SauveConfig();
  }
  if (sw.indexOf("dispPw") >= 0) {
    SeriePrintln(">>" + SerialIn);
    dispPw = !dispPw;
  }
  if (sw.indexOf("dispAct") >= 0) {
    SeriePrintln(">>" + SerialIn);
    dispAct = !dispAct;
  }
  if (sw.indexOf("P:") >= 0 || sw.indexOf("T:") >= 0) {
    if (sw.indexOf("T:") >= 0) {
      testTrame = valeur.toInt();
    }
    if (sw.indexOf("P:") >= 0) {
      testPulse = valeur.toInt();
    }
    if (testPulse > testTrame) testPulse = testTrame;
    SeriePrintln("Rapport cyclique Pulse " + String(testPulse * 10) + " ms / Trame " + String(testTrame * 10) + " ms  : P=" + String(testPulse) + "  T=" + String(testTrame));
  }
  if ((sw.indexOf("H")>=0 || sw.indexOf("?")>=0 ) && p==-1){
    MessageCommandes();
  }
  SerialIn = "";
}
void InitMessages() {
  for (int i = 0; i < 20; i++) {
    MessageH[i] = "";
  }
  idxMessage = 0;
}

void StockMessage(String m) {
  if (DATE!="") m = DATE + " : " + m;
  EcranPrintln(m);
  MessageH[idxMessage] = m;
  idxMessage = (idxMessage + 1) % 20;
}



//Telnet et Serial
//****************
// commandes disponibles par port serie ou Telnet
const char *helpSerial = R"====(
**************
commandes pour configuration par port série ou Telnet (respect majuscules, ponctuation et terminer par touche 'Enter'):

ssid:xxx     | Pour définir le nom xxx du Wifi à utiliser
password:yyy | Pour définir le mot de passe yyy du Wifi
restart      | Pour redémarrer l'ESP32 sans appui sur le bouton EN

H ou ?       | pour avoir cette aide
**************
)====";
//? A REFAIRE ???????????????
//dispPw       | Pour afficher les mesures de puissance Pw
//dispAct      | Pour afficher les ouvertures des Actions
//ETH01        | Bascule sur la config ethernet avec bus RMII et LAN8720
//T:xxx        | En mode Train de Sinus force la longeur de Trame. 
//             | T:xxx = xxx*10ms, T:0 retourne en Train de sinus normal
//P:yyy        | En mode Train de Sinus force la longueur des Pulses. 
//             | P:yyy = yyy*10ms. P<=T . Evitez P impaire et Trame paire. 
void MessageCommandes() {
  String M = helpSerial;
  String Before;
  while (M.indexOf("\n") >= 0) { //Decoupe pour Telnet
    SplitS(M, Before, "\n", M);
    SeriePrintln(Before);
  }
}
void Init_TelnetServer(){
  telnetServer.begin();
  telnetServer.setNoDelay(true);
  EcranPrintln("Serveur Telnet actif sur le port 23");
}
void GestionTelnet(){
  // Vérifie si un nouveau client Telnet
  if (telnetServer.hasClient()) {
    if (telnetClient && telnetClient.connected()) {
      telnetClient.stop();  // Déconnecter ancien client
    }
    telnetClient = telnetServer.accept();
    SeriePrintln("Nouveau client Telnet : " + nomRouteur + " connecté !");
    telnetClient.println("Bienvenue !");
    MessageCommandes();
  }
}
// --- Fonction de sortie partagée ---
void SeriePrint(const String& message) {
  Serial.print(message);  // Sortie sur port série USB/CDC
  Serial0.print(message); // Sortie sur port série classique
  if (telnetClient && telnetClient.connected()) {
    telnetClient.print(message);  // Sortie sur Telnet
  }
  OledPrint(message);
}
void SeriePrintln(const String& message) {
  Serial.println(message);
  Serial0.println(message);
  if (telnetClient && telnetClient.connected()) {
    telnetClient.println(message);
  }
    OledPrintln(message);
}


