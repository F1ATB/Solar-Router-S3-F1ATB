#pragma once
#include <Arduino.h>
#include <WebSocketsClient.h>
#include "config.h"



void Liste_des_Noms() ;
void RefreshNomsEtats();
void Liste_NomsEtats(int Idx_RMS) ;
void InfoActionExterne() ;
void Init_ListeRouteurs();
void NomEtatLocal();
void initClientWebsocket();

struct RouteurExterne { 
    String Data; //Format des Data des Sources de puissance externes
    unsigned long IP;//IP des routeurs externes 
    String CanalNom[NB_SOURCES_DEVICE_MAX][NB_CANAUX_PAR_SOURCE];
    String NomEtat; //Représentation pour les ESP32 classiques
    String NomRouteur;
    bool S3;
    String TemperNom[CANAUX_TEMPERATURE];
    float temperature[CANAUX_TEMPERATURE];
    String NomActionExterne[BLOCS_ACTION_MAX];
    byte OuvertureActionExterne[BLOCS_ACTION_MAX];
    float HequivActionExterne[BLOCS_ACTION_MAX];
    byte ValideActionExterne[BLOCS_ACTION_MAX];

};
extern RouteurExterne RouteurExt[LES_ROUTEURS_MAX];

extern String RMS_NomEtatLocal; //Definition pour routeur classique

struct RouteurComExterne { //Structure figée
    WebSocketsClient webSocket;
    unsigned long IP;//IP des routeurs externes
};
extern RouteurComExterne RouteurComExt[LES_ROUTEURS_MAX]; //IP utilise dans les websockets
