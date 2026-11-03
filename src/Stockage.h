#pragma once
#include <Arduino.h>

extern String Record_Conf;
extern bool LastRecordData;



String HistoriqueEnergie(void);
void LitConfig();
void SauveConfig();
String BilanMemoire();
void  DeserializeConfiguration(String json );
String SerializeConfiguration();
void Record_Data( String Date_AMJ,int16_t Heure_,int16_t Minute_);
void LitLastRecord_Data();
String SerializeParaTheme();
