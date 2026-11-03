#pragma once
#include <Arduino.h>

//WIFI
extern int WIFIbug ;
extern int16_t ComSurv ;  //Timeout sans Wifi par pas de 30s
extern byte WifiSleep ;

extern String Liste_AP ;
extern uint8_t bestBSSID[6];  //Meilleur en dBm adresse MAC
extern unsigned long Gateway ;
extern unsigned long masque;
extern unsigned long dns ;



#define NB_MACHINE_MAX 16
struct Reseau{  //Mesurer qualité liaison
    String nom;
    unsigned long IP;
    int8_t NbCx;
    int8_t Note;
    bool RMS;
};
extern Reseau Machine[NB_MACHINE_MAX];


extern byte arrIP[4];
String IP2String(unsigned long IP);

void Init_Internet();
void Init_Machines();
int8_t Idx_Machine(unsigned long ip);
String httpGetPayload(String url, int Idx_RMS ) ;