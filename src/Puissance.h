
#pragma once
#include <Arduino.h>
#include "config.h"





 

struct SourcePuissance { 
    String Type; //Sources de puissance
    String Data; //Format des Data des Sources de puissance externes
    unsigned long IP;//IP des sources externes de puissance
    int8_t Port; //Port Série pour les UxIx2 etc
    String CanalNom[NB_CANAUX_PAR_SOURCE];
    int8_t CanalVoie[NB_CANAUX_PAR_SOURCE];
    String NomSoutir[NB_CANAUX_PAR_SOURCE];
    String NomInject[NB_CANAUX_PAR_SOURCE];
    String TopicP; //Topic MQTT pour la source de puissance externe
};
extern SourcePuissance LaSource[NB_SOURCES_DEVICE_MAX];

struct SourceVariable { //Variables attachées à des sources
    String DataBrute; //Sources de puissance
    bool RAZ_JSY; //JSY 2 et 3
    unsigned long Temps_precedent; //UxIx3
    String Modele;
    String Profile; 

};
extern SourceVariable LaSourceVar[NB_SOURCES_DEVICE_MAX];

//Note, les sources MQTT ou Enphase sont considérés unique (max 1 par maison) ????? A REVOIR POUR ENPHASE QUI PEUT AVOIR PLUSIEURS ONDULEURS


//Paramètres électriques

extern float Tension[NB_CANAUX_MAX], Intensite[NB_CANAUX_MAX], PowerFactor[NB_CANAUX_MAX], Frequence_[NB_CANAUX_MAX];
extern long Energie_Soutiree[NB_CANAUX_MAX], Energie_Injectee[NB_CANAUX_MAX];
extern long EnergieJour_Soutiree[NB_CANAUX_MAX], EnergieJour_Injectee[NB_CANAUX_MAX];
extern long EnergieJour0_Soutiree[NB_CANAUX_MAX],  EnergieJour0_Injectee[NB_CANAUX_MAX];
extern float PuissanceS_inst[NB_CANAUX_MAX], PuissanceI_inst[NB_CANAUX_MAX],Puissance_moy[NB_CANAUX_MAX];
extern float PVAS_inst[NB_CANAUX_MAX],  PVAI_inst[NB_CANAUX_MAX], PVA_moy[NB_CANAUX_MAX];
extern int PuissanceS[NB_CANAUX_MAX],  PuissanceI[NB_CANAUX_MAX];
extern int PuissanceS_Max[NB_CANAUX_MAX], PuissanceI_Max[NB_CANAUX_MAX];
extern int PVAS[NB_CANAUX_MAX],  PVAI[NB_CANAUX_MAX];
extern double EAS_float[NB_CANAUX_MAX] , EAI_float[NB_CANAUX_MAX] ;
extern bool EnergieActiveValide[NB_CANAUX_MAX],PVA_Valide[NB_CANAUX_MAX];
extern int PactConso_M, PactProd; //Pour Enphase ??? A TRAITER pour plusieurs enphase cela existe
extern long LastwhDlvdCum,LastwhRcvdCum ;   


extern int16_t PuissanceValide;
extern bool LissageLong;
extern bool PuissanceRecue;


void Init_Puissance() ;
void Filtre_Puissance(bool Float,int8_t canal);
float PfloatMax(float Pin);
int PintMax(int Pin);
void RAZ_Source_Var(int8_t source);
void CalculEnergieJour(int8_t canal, bool A_calculer);
void Puissance_Max(int8_t canal);
void RAZ_Energie_Le_Matin();
void AffectationPuissanceAuxActions(int8_t canal);




