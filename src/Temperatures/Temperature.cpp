#include "Temperatures/Temperature.h"
#include <Arduino.h>
#include "config.h"
#include "Ecrans/Ecran.h"
#include "commonFX.h"
#include "RMS_Externes.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "Communications/comSortie.h"
#include "Communications/Internet.h"

float temperature[CANAUX_TEMPERATURE]; // 4 canaux max de températurre
String Source_Temp[CANAUX_TEMPERATURE];
String nomTemperature[CANAUX_TEMPERATURE];
String TopicT[CANAUX_TEMPERATURE];
String AllTemp = "";
// Température Capteur DS18B20
byte pTemp = 0;
int offsetTemp[CANAUX_TEMPERATURE]; // erreur *100
int TemperatureValide[CANAUX_TEMPERATURE];
byte canalTempExterne[CANAUX_TEMPERATURE];
unsigned long refTempIP[CANAUX_TEMPERATURE];
int Nbr_DS18B20 = 0;

OneWire oneWire(0); // Numero de pin bidon pour le constructor en attendant affectation reel à placer au debut du setup
DallasTemperature ds18b20(&oneWire);

static byte pinTemp[] = {0, 15,21,27,33};

void InitGpioTemperature()
{
  if (pTemp > 0)
  {
    EcranPrint("InitTemp:");
    EcranPrintln(String(pinTemp[pTemp]));
    oneWire.begin(pinTemp[pTemp]);
    ds18b20.begin();
    Nbr_DS18B20 = ds18b20.getDeviceCount();
  }
}

void Init_Temperature()
{
  Nbr_DS18B20 = 0;
  for (int i = 0; i < CANAUX_TEMPERATURE; i++)
  { // 4 canaux max de température
    Source_Temp[i] = "tempNo";
    temperature[i] = -127;
    offsetTemp[i] = 0;
    TemperatureValide[i] = 0;
    canalTempExterne[i] = 0;
    nomTemperature[i] = "Température canal " + String(i);
    TopicT[i] = "TemperatureMQTT_" + String(i);
    refTempIP[i] = 0;
  }
}

void LectureTemperature()
{
  float temperature_brute = -127;
  float temperDs18B20[4];
  bool tempInterneOK = true;
  AllTemp = "|";
  // On fait les mesures de températures internes
  if (Nbr_DS18B20 > 0)
  {
    ds18b20.requestTemperatures();
    int i = 0;
    for (int canal = 0; canal < CANAUX_TEMPERATURE; canal++)
    {
      temperDs18B20[canal] = -127;
      if (Source_Temp[canal] == "tempInt")
      {
        temperDs18B20[canal] = ds18b20.getTempCByIndex(i) + float(offsetTemp[canal]) / 100.0;
        AllTemp += String(temperDs18B20[canal]) + "°C|";
        i++;
      }
    }
    while (i < Nbr_DS18B20)
    {
      AllTemp += String(ds18b20.getTempCByIndex(i)) + "°C|"; // On rajoute pour l'affichage brute les ds18b20 non utilisés
      i++;
    }
  }

  // On affecte les mesures si valides
  for (int canal = 0; canal < CANAUX_TEMPERATURE; canal++)
  {
    temperature_brute = -127;

    if (Source_Temp[canal] == "tempInt")
    {
      temperature_brute = temperDs18B20[canal];
      if (temperature_brute < -50 || temperature_brute > 150)
      { // Invalide. Pas de capteur ou parfois mauvaise réponse
        if (TemperatureValide[canal] > 0)
        {
          TemperatureValide[canal] = TemperatureValide[canal] - 1; // Perte éventuels de quelques mesures
        }
        else
        {
          StockMessage("Mesure Température invalide ou pas de capteur DS18B20, canal : " + String(canal)); // Trop de pertes
          tempInterneOK = false;
        }
      }
      else
      { // Mesure interne OK
        TemperatureValide[canal] = 5;
      }
    }
    else if (Source_Temp[canal] == "tempExt" && ModeReseau < 2)
    {
      if (refTempIP[canal] > 0)
      {
        unsigned long IPduRMS = refTempIP[canal];
        int idxRMS = -1;
        for (int i = 0; i < LES_ROUTEURS_MAX; i++)
        {
          if (RouteurExt[i].IP == IPduRMS)
          {
            idxRMS = i;
            break;
          }
        }
        int idxM = Idx_Machine(IPduRMS);
        if (TemperatureValide[canal] > 0)
          TemperatureValide[canal] = TemperatureValide[canal] - 1; // Watchdog pour verfier mesures arrivent
        if (Machine[idxM].Note > 0 && idxRMS >= 0)
        {
          // Ici on recupere la température des ESP32 classiques et S3
          String Etat, TempsR, TempR, Valeur;
          SplitS(RouteurExt[idxRMS].NomEtat, Valeur, US, Etat); // Nom
          SplitS(Etat, TempsR, US, Etat);                       // Les Températures
          while (TempsR.length() > 1)
          {
            SplitS(TempsR, TempR, FS, TempsR);
            SplitS(TempR, Valeur, ES, TempR); // Canal Externe
            if (Valeur.toInt() == canalTempExterne[canal])
            {
              SplitS(TempR, Valeur, ES, TempR); // Titre
              SplitS(TempR, Valeur, ES, TempR); // Température
              temperature_brute = Valeur.toFloat();
              TemperatureValide[canal] = 5;
            }
          }
        }
      }
    }
    else if (Source_Temp[canal] == "tempMqtt" && ModeReseau < 2)
    {

      // A FAIRE MQTT ??????????????????????????????????
      if (TemperatureValide[canal] > 0)
      {
        TemperatureValide[canal] = TemperatureValide[canal] - 1; // Watchdog pour verfier mesures arrivent voir MQTT.ino
      }
      else
      {
        temperature[canal] = temperature_brute;
      }
    }
    else
    { // "tempNo"
    }
    temperature[canal] = temperature_brute; // On affecte la température brute à la valeur finale
    if (temperature[canal] > -100)
    { // Valeur indicative d'erreur
      EcranPrintln("Temp" + String(canal) + "=" + String(temperature[canal]) + "°C,");
    }
  } // Fin de la boucle sur les canaux de température

  for (int canal = 0; canal < CANAUX_TEMPERATURE; canal++)
  {
    if (Source_Temp[canal] == "tempInt" && !tempInterneOK)
    {
      TemperatureValide[canal] = 0; // On arrete les mesures de tous les internes car l'ordre des DS18B20 peut être rompu
      temperature[canal] = -126;
    }
  }
}
String LesTemperatures()
{
  String LesTemp = "";
  for (int c = 0; c < CANAUX_TEMPERATURE; c++)
  {
    LesTemp += String(temperature[c]) + "|";
  }
  return LesTemp;
}