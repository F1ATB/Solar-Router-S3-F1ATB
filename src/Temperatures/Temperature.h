#pragma once
#include <Arduino.h>
#include "../config.h"

extern float temperature[CANAUX_TEMPERATURE];  // 4 canaux max de températurre
extern String Source_Temp[CANAUX_TEMPERATURE];
extern String nomTemperature[CANAUX_TEMPERATURE];
extern String TopicT[CANAUX_TEMPERATURE];
extern String AllTemp ;
extern byte pTemp ;
extern int offsetTemp[CANAUX_TEMPERATURE];  //erreur *100
extern int TemperatureValide[CANAUX_TEMPERATURE];
extern byte canalTempExterne[CANAUX_TEMPERATURE];
extern unsigned long refTempIP[CANAUX_TEMPERATURE];
extern int Nbr_DS18B20 ;

void InitGpioTemperature();
void Init_Temperature() ;
void LectureTemperature();
String LesTemperatures() ;

