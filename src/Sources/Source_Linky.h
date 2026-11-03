#pragma once
#include <Arduino.h>

extern String STGE;   //Status Linky
extern String STGEt;  //Status Tempo uniquement RTE
extern String NGTF;   //Calendrier tarifaire
extern String JourLinky;
extern long EASF01, EASF02, EASF03, EASF04, EASF05, EASF06, EASF07, EASF08, EASF09, EASF10;

//Parameters for Linky
extern volatile int IdxDataRawLinky;
extern volatile int IdxBufDecodLinky ;
extern EXT_RAM_BSS_ATTR char DataRawLinky[10000];  //Buffer entrée données Linky



void Setup_Linky(int8_t source);
void LectureLinky(int8_t source);
