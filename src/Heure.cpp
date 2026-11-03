#include "Heure.h"
#include "config.h"
#include "Stockage.h"
#include <Arduino.h>
#include "FS.h"
#include "LittleFS.h"
#include "commonFX.h"
#include "Puissance.h"
#include "Communications/Internet.h"
#include "Sources/Source_Linky.h"
#include "esp_system.h"
#include "time.h"
#include "esp_sntp.h"
#include "Communications/comSortie.h"
#include "Communications/OpenMeteo.h"
#include "Ecrans/Ecran.h"
#include <WiFi.h>
#include "BlocActions.h"

byte Horloge = 0;                                                                                       // 0=Internet, 1=Linky, 2=Interne, 3=IT 10ms/triac, 4=IT 20ms
byte idxFuseau = 0;                                                                                     // Europe Centrale                                                                                  //-1=inconnu,0=dimache,1=lundi...
String codeTZ[] = {"CET-1CEST,M3.5.0,M10.5.0/3", "AST4", "GFT3", "RET-3", "EAT-3", "NCT-11", "WFT-12"}; // Europe centrale, Guadeloupe / Martinique, Guyane, Réunion, Mayotte,Nouvelle Calédonie, Wallis et Futuna
int16_t Int_Heure, Int_Minute;
bool HeureValide = false;
int8_t Jour = -1; //-1=inconnu,0=dimanche,1=lundi...
String ntpServer = "";

static bool EstFuseauHoraireValide(byte fuseau)
{
  return fuseau < (sizeof(codeTZ) / sizeof(codeTZ[0]));
}

void FormatteHeureDate()
{
  // Formatte la date et l'heure en String
  char buffer[MAX_SIZE_T];
  time_t now = time(nullptr);
  struct tm timeinfo;

  if (now == (time_t)-1 || localtime_r(&now, &timeinfo) == nullptr)
  {
    HeureValide = false;
    DATE = "";
    DateAMJ = "";
    Int_Heure = 0;
    Int_Minute = 0;
    Jour = -1;
    return;
  }

  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  DATE = String(buffer);
  strftime(buffer, sizeof(buffer), "%Y%m%d", &timeinfo);
  DateAMJ = String(buffer);
  Int_Heure = timeinfo.tm_hour;
  Int_Minute = timeinfo.tm_min;
  Jour = timeinfo.tm_wday; //-1=inconnu,0=dimanche,1=lundi...
  HeureValide = true;
}

void JourHeureChange()
{
  if (!HeureValide)
  {
    return;
  }

  const int16_t old_Heure = Int_Heure;
  const int16_t old_Minute = Int_Minute;
  const String oldDateAMJ = DateAMJ;

  FormatteHeureDate();

  HeureCouranteDeci = Int_Heure * 100 + Int_Minute * 10 / 6;

  for (int i = 0; i < BLOCS_CAPTEUR_MAX; i++)
  {
    if (BlocsCapteurs[i].NumBloc == 255)
    {
      break; // On sort si on a plus de bloc capteur
    }

    if (BlocsCapteurs[i].type == "heure")
    {
      SortieNum0[BlocsCapteurs[i].NumBloc] = float(HeureCouranteDeci) / 100.0;
    }
  }

  if (HeureCouranteDeci == 600) // RAZ ouverture actions à 6h00
  {
    for (int i = 0; i < BLOCS_ACTION_MAX; i++)
    {
      BlocsActions[i].Hequiv = 0; // RAZ temps equivalent ouverture à 6h du matin
    }
  }

  if (old_Heure == 23 && Int_Heure == 0) // Changement de Jour
  {
    erreurTriac = false;
    Record_Data(oldDateAMJ, old_Heure, old_Minute);
    RAZ_Energie_Le_Matin();
    for (int h = 0; h < 24; h++)
    {
      Wm2[h] = Wm2[h + 24]; // Décalage des valeurs d'ensoleillement du lendemain à ce jour
    }
    SoleilJour = SoleilDemain; // Valeur du lendemain devient valeur du jour
  }
}

// **************
// * Heure DATE * -
// **************
void time_sync_notification(struct timeval *tv)
{
  EcranPrintln("\nNotification de l'heure ( time synchronization event ) ");
  EcranPrint("Sync time in ms : ");
  EcranPrintln(String(sntp_get_sync_interval()));
  FormatteHeureDate();
  JourHeureChange();
  StockMessage("Réception de l'heure Internet");
}
void DefFuseauHoraire(void)
{
  if (Horloge != 0)
  {
    return;
  }

  // Heure / Hour . A Mettre en priorité avant WIFI (exemple ESP32 Simple Time)
  // External timer to obtain the Hour and reset Watt Hour every day at 0h
  sntp_set_sync_interval(10800000); // Synchro toutes les 3h
  sntp_set_time_sync_notification_cb(time_sync_notification);
  // sntp_servermode_dhcp(1);   Déprecié
  esp_sntp_servermode_dhcp(true); // Option

  const byte fuseauActif = EstFuseauHoraireValide(idxFuseau) ? idxFuseau : 0;

  if (ntpServer != "")
  {
    char buffer[30];
    ntpServer.toCharArray(buffer, sizeof(buffer));
    const char *NTP = buffer;
    configTzTime(codeTZ[idxFuseau].c_str(), NTP, ntpServer1, ntpServer2); // Voir Time-Zone:
  }
  else
  {
    configTzTime(codeTZ[fuseauActif].c_str(), ntpServer1, ntpServer2); // Voir Time-Zone:
  }
  EcranPrintln("idxFuseau: " + String(idxFuseau));
  EcranPrintln("CODE: " + codeTZ[idxFuseau]);
}

String HeureCourante()
{
  String DateLast = "Attente d'une mise à l'heure par internet";
  if (Horloge == 1)
    DateLast = "Attente d'une mise à l'heure par le Linky";
  if (ModeReseau == 0 && WiFi.getMode() != WIFI_STA)
    DateLast = "Sélectionnez un réseau <a href='/pageHtmlWifi.html'>Wifi</a>";
  if (Horloge == 2)
    DateLast = "Attente d'une mise à l'heure  <a href='/pageHtmlHeure.html' >manuellement</a> ";
  if (Horloge == 5)
    DateLast = "Attente d'une mise à l'heure par un ESP externe (maître) ";
  if (HeureValide)
  {
    DateLast = DATE;
  }
  return DateLast;
}