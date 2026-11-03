#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>



// Nombre Routeurs réseau Max
#define LES_ROUTEURS_MAX 8 // Ne pas toucher -Javascript connais pas
#define HOSTNAME "PV-RouterS3-"

//Sources de puissance
#define NB_SOURCES_DEVICE_MAX 4
#define NB_CANAUX_PAR_SOURCE 3
#define NB_CANAUX_MAX 12 // 3 Canaux de puissance par Device, pour traiter les triphasés indépendants

// Nombre Blocs pour la page Actions
#define BLOCS_ACTION_MAX 10
#define BLOCS_CAPTEUR_MAX 30
#define BLOCS_FILTRE_MAX 40
#define BLOCS_LOGIQUE_MAX 40
#define BLOCS_ADD_SUB_MAX 10
#define BLOCS_TOTAUX_MAX 120

#define CANAUX_TEMPERATURE 4

//Timing
#define OVER_PRODUCTION_INTERVALLE 200 // Intervalle en millisecondes pour la gestion de la surproduction
#define DONNEES_ACTIONS_INTERVALLE 500 // Intervalle en millisecondes pour l'envoi par websoket des actions'
#define DONNEES_LENTES_INTERVALLE 51000 // Intervalle en millisecondes pour l'envoi des données à évolution lente


extern String Version;
extern String hostname;

extern String ssid;
extern String password;
extern String CleAcces;
extern String CleAccesRef;
extern byte dhcpOn;
extern unsigned long localIP;


extern byte ModePara;   // 0 = Minimal, 1= Expert
extern byte ModeReseau; // 0 = Internet, 1= LAN only, 2 =AP pas de réseau

extern String nomRouteur;
extern String Theme;

// Compatibilité Ancien routeur


extern String Source, Source_data; // Source referente du routeur, LaSource[0].Type

extern String ES, FS, GS, RS, US; // Unit Separator



extern int8_t tabPulseSinusOn[101];
extern int8_t tabPulseSinusTotal[101];

extern volatile int16_t testPulse;
extern volatile int16_t testTrame;

// Triac
extern bool erreurTriac;
extern byte pTriac; // Choix Pin Triac
extern int8_t pulseTriac, zeroCross;
extern int8_t PulseT[];
extern int8_t ZeroT[];
extern int8_t NumBlocTriac;



// Heure et Date
#define MAX_SIZE_T 80
extern const char *ntpServer1;
extern const char *ntpServer2;
extern String DATE;
extern String DateAMJ;

extern int16_t HeureCouranteDeci;

// Internal Timers
extern unsigned long startMillis;
extern unsigned long previousStatusMillis;
extern unsigned long previousDonneesLentesMillis;
extern unsigned long previousWsMillis;
extern unsigned long previousWiMillis;

extern unsigned long previousTimer2sMillis;
extern unsigned long previousOverProdMillis;
extern unsigned long previousLEDsMillis;
extern unsigned long previousActionMillis;
extern unsigned long previousTempMillis;
extern unsigned long previousRevueBlocMillis;
extern unsigned long previousLoop;
extern unsigned long previousDonneesActionsMillis;
extern unsigned long T_On_seconde;
extern float previousLoopMin;
extern float previousLoopMax;
extern float previousLoopMoy;
extern unsigned long previousTimeRMS;
extern float previousTimeRMSMin;
extern float previousTimeRMSMax;
extern float previousTimeRMSMoy;



extern unsigned long LastRMS_Millis[NB_SOURCES_DEVICE_MAX];
extern unsigned long PeriodeProgMillis[NB_SOURCES_DEVICE_MAX];





extern EXT_RAM_BSS_ATTR int16_t tabTemperature_5mn[4 * 600];
extern EXT_RAM_BSS_ATTR int8_t tab_histo_ouverture[BLOCS_ACTION_MAX* 600];

extern EXT_RAM_BSS_ATTR size_t Mem_SRAM[3]; // Total,dispo,min dispo
extern EXT_RAM_BSS_ATTR size_t Mem_PSRAM[3];
extern EXT_RAM_BSS_ATTR size_t Mem_Fichier[3];
extern EXT_RAM_BSS_ATTR String ConfigActions;
extern EXT_RAM_BSS_ATTR JsonDocument LesActions;
extern EXT_RAM_BSS_ATTR JsonDocument LesZones;


extern float RetardF[BLOCS_ACTION_MAX]; // Floating value of retard
extern float LastErrorPw[BLOCS_ACTION_MAX];    //Floating value of previous Pw error
extern float IntegrErrorPw[BLOCS_ACTION_MAX];  //Integral de l'erreur
extern float Propor[BLOCS_ACTION_MAX];         // correction proportionnelle
extern float DeriveF[BLOCS_ACTION_MAX];        // Derive erreur filtrée


// Variables in RAM for interruptions
extern volatile unsigned long lastIT;
extern volatile int16_t IT10ms;    // Interruption avant deglitch
extern volatile int16_t IT10ms_in; // Interruption apres deglitch
extern volatile int16_t ITmode;    // IT exerne Triac ou interne
extern volatile bool Phase230V;
extern volatile int NbActions;
extern volatile int Retard[BLOCS_ACTION_MAX];
extern volatile int PulseOn[BLOCS_ACTION_MAX];
extern volatile int PulseTotal[BLOCS_ACTION_MAX];
extern volatile int PulseComptage[BLOCS_ACTION_MAX];
extern volatile int Gpio[BLOCS_ACTION_MAX];
extern volatile int OutOn[BLOCS_ACTION_MAX];
extern volatile int OutOff[BLOCS_ACTION_MAX];

//=== Localisation ===
extern float latitude; // Paris par defaut
extern float longitude;
//=== ensoleillement ===
extern int SoleilDemain;
extern int SoleilActuel;
extern int SoleilJour;
extern int SunRiseJour;
extern int SunSetJour;
extern int SunRiseDemain;
extern int SunSetDemain;
extern int HnextSoleil;
extern EXT_RAM_BSS_ATTR int16_t Wm2[48];

extern byte ReacCACSI;
extern bool EstimCACSI;
extern unsigned int FreqPWM; // Frequence signaux PWM en Hz

extern int16_t EthernetBug;



extern String EnphaseSerial; // Sert égalemnet au Shelly comme numéro de voie

//RTE
extern String LTARF; // Option tarifaire RTE
