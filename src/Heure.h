#pragma once
#include <Arduino.h>

// Variables globales
extern byte Horloge; //0=Internet, 1=Linky, 2=Interne, 3=IT 10ms/triac, 4=IT 20ms
extern byte idxFuseau; //Fuseau Horaire
extern int16_t Int_Heure, Int_Minute;
extern bool HeureValide;
extern int8_t Jour; //-1=inconnu,0=dimanche,1=lundi...
extern String codeTZ[];
extern String ntpServer;

// Fonctions
void FormatteHeureDate();
void JourHeureChange();
void DefFuseauHoraire(void);
String HeureCourante();

