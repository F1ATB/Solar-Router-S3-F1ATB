// ********************
// Gestion des Actions
// ********************
#pragma once
#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>



void Init_Blocs();
void DecodageConfigAction();
void RevueBlocsActions();
void RafraichissementsBlocsCapteurs();





class Bloc_Action
{
public:
  // Listes
  Bloc_Action();
  Bloc_Action(int aIdx);
  String value;
  byte NumBloc; // Numero du Bloc sur la page Actions
  int Gpio;
  
  enum  RegulationType
  {
    INACTIF = 0,
    ONOFF = 1,
    DECOUPE_SINUS = 2,
    DEMI_SINUS = 3,
    MULTI_SINUS = 4,
    TRAIN_DE_SINUS = 5,
    PWM = 6
  };
  RegulationType Regulation;
  int Port;
  unsigned int Repetition;
  unsigned int Temporisation;
  unsigned int T_LastAction;
  String Nom;
  String Host;
  String OrdreOn;
  String OrdreOff;
  int tOnOff = 0;
  int OutOn ;
  int OutOff ;
  bool PID_On;
  byte P;
  byte I;
  byte D;
  bool GetPost;
  int SeuilPW_On;
  int SeuilPW_Off;
  bool VoltON;           // 0=0V, 1=3.3V
  byte OuvreForce;       // Pourcent ouverture forcée
  byte OuvreMax;         // Pourcent ouverture max
  bool On;
  float Hequiv;          // Equivalent temps ouverture 100%
  bool EntreeCable[4];   // 0=Pas de câble, 1= Câble présent
  bool EntreeNum[4];     // Entree numérique pour les capteurs et non binaire
  byte BlocSenEntree[4]; // Bloc de sortie connecté en entrée
  byte PinSenEntree[4];  // Bloc de sortie connecté en entrée

  void InitGpio(int FreqPWM);
  void SortieOnOff(bool onoff);
  void SortieOff();
  void CallExterne(String host, String url, int port,bool getPost);
};

class Bloc_Capteur
{
public:
  Bloc_Capteur();
  String type;
  String value;
  byte NumBloc;
  byte Idx_RMS;
  byte Idx_Act;
  String Commande;
  bool SortieNum;
};

class Bloc_Filtre
{
public:
  Bloc_Filtre();
  String value;
  byte NumBloc;
  float SeuilOuSeconde;
  float Memoire;
  bool EntreeCable;   // 0=Pas de câble, 1= Câble présent
  bool EntreeNum;     // Entree numérique pour les capteurs et non binaire
  bool OldEntree;     // Ancienne valeur d'entrée pour le filtre timer
  byte BlocSenEntree; // Bloc de sortie connecté en entrée
  byte PinSenEntree;  // Bloc de sortie connecté en entrée
  bool SortieNum;
};
class Bloc_Logique
{
public:
  Bloc_Logique();
  String value;
  byte NumBloc;
  bool EntreeCable[10];   // 0=Pas de câble, 1= Câble présent
  bool EntreeNum[10];     // Entree numérique pour les capteurs et non binaire
  bool OldEntree[3];      // Anciennes valeur d'entrée pour la bascule
  byte BlocSenEntree[10]; // Bloc de sortie connecté en entrée
  byte PinSenEntree[10];  // Bloc de sortie connecté en entrée
  byte NbEntree;          // Nombre d'entrées utilisées
};
class Bloc_AddSub
{
public:
  Bloc_AddSub();
  String value;
  byte NumBloc;
  bool EntreeCable[2];   // 0=Pas de câble, 1= Câble présent
  byte BlocSenEntree[2]; // Bloc de sortie connecté en entrée
  byte PinSenEntree[2];  // Bloc de sortie connecté en entrée
};

void GestionForcageActions();
void forceAction(int NumAction, int Force);


extern Bloc_Action BlocsActions[BLOCS_ACTION_MAX]; // Liste des Blocs des actions
extern Bloc_Capteur BlocsCapteurs[BLOCS_CAPTEUR_MAX]; // Liste des Blocs capteurs
extern Bloc_Filtre BlocsFiltres[BLOCS_FILTRE_MAX]; // Liste des Blocs des actions
extern Bloc_Logique BlocsLogiques[BLOCS_LOGIQUE_MAX]; // Liste des Blocs des actions
extern Bloc_AddSub BlocsAddSub[BLOCS_ADD_SUB_MAX]; // Liste des Blocs des additionneurs de puissance
extern bool SortieBin[BLOCS_TOTAUX_MAX]; //Etat des sorties si binaire
extern float SortieNum0[BLOCS_TOTAUX_MAX]; //Etat des sorties si analogique
extern float SortieNum1[BLOCS_TOTAUX_MAX]; //Etat des sorties si analogique (2 sorties max par bloc)
extern int NbBlocs; // Nombre de Blocs dans la configuration


