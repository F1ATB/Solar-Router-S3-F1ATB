#include "config.h"
#include <Arduino.h>
#include "BlocActions.h"


String Version="";
String hostname="";

String ssid = "";
String password = "";
String CleAcces = "";
String CleAccesRef = "";
byte dhcpOn = 1;
unsigned long localIP = 0;

byte ModePara = 0;    //0 = Minimal, 1= Expert
byte ModeReseau = 0;  //0 = Internet, 1= LAN only, 2 =AP pas de réseau


String nomRouteur = "Routeur F1ATB-S3";
String Theme = "default";

//Compatibilité Ancien routeur

String Source = "NotDef";
String Source_data = "NotDef";

String ES = String((char)27);  //ESC Separator
String FS = String((char)28);  //File Separator
String GS = String((char)29);  //Group Separator
String RS = String((char)30);  //Record Separator
String US = String((char)31);  //Unit Separator




int8_t tabPulseSinusOn[101];
int8_t tabPulseSinusTotal[101];

volatile int16_t testPulse=0;
volatile int16_t testTrame=0;

//Triac
bool erreurTriac = false;
byte pTriac = 0;  //Choix Pin Triac
int8_t pulseTriac = 0, zeroCross = -1;
int8_t PulseT[] = { 0, 9 };
int8_t ZeroT[] = { -1, 14 };
int8_t NumBlocTriac = -1;


// Heure et Date

const char *ntpServer1 = "fr.pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
String DATE = "";
String DateAMJ = "";  
int16_t HeureCouranteDeci = 0;


//Internal Timers
unsigned long startMillis;
unsigned long previousStatusMillis;
unsigned long previousDonneesLentesMillis;
unsigned long previousWsMillis;
unsigned long previousWiMillis;

unsigned long previousTimer2sMillis;
unsigned long previousOverProdMillis;
unsigned long previousLEDsMillis=0;
unsigned long previousActionMillis;
unsigned long previousTempMillis;
unsigned long previousRevueBlocMillis;
unsigned long previousLoop;
unsigned long previousDonneesActionsMillis;


unsigned long T_On_seconde = 0;
float previousLoopMin = 1000;
float previousLoopMax = 0;
float previousLoopMoy = 0;
unsigned long previousTimeRMS;
float previousTimeRMSMin = 1000;
float previousTimeRMSMax = 0;
float previousTimeRMSMoy = 0;


unsigned long LastRMS_Millis[NB_SOURCES_DEVICE_MAX];
unsigned long PeriodeProgMillis[NB_SOURCES_DEVICE_MAX] = {1000};





EXT_RAM_BSS_ATTR int16_t tabTemperature_5mn[4 * 600];
EXT_RAM_BSS_ATTR int8_t tab_histo_ouverture[BLOCS_ACTION_MAX * 600];

EXT_RAM_BSS_ATTR size_t Mem_SRAM[3];  //Total,dispo,min dispo
EXT_RAM_BSS_ATTR size_t Mem_PSRAM[3];
EXT_RAM_BSS_ATTR size_t Mem_Fichier[3];
EXT_RAM_BSS_ATTR JsonDocument LesActions;
EXT_RAM_BSS_ATTR JsonDocument LesZones;

//Actions et Triac(action 0)
float RetardF[BLOCS_ACTION_MAX]= {100.0};  //Floating value of retard
float LastErrorPw[BLOCS_ACTION_MAX]= {0};    //Floating value of previous Pw error
float IntegrErrorPw[BLOCS_ACTION_MAX]= {0};  //Integral de l'erreur
float Propor[BLOCS_ACTION_MAX]= {0};         // correction proportionnelle
float DeriveF[BLOCS_ACTION_MAX]= {0};        // Derive erreur filtrée

//Variables in RAM for interruptions
volatile unsigned long lastIT = 0;
volatile int16_t IT10ms = 0;        //Interruption avant deglitch
volatile int16_t IT10ms_in = 0;     //Interruption apres deglitch
volatile int16_t ITmode = 0;        //IT exerne Triac ou interne
volatile bool Phase230V = false;
volatile int NbActions = 0;
volatile int Retard[BLOCS_ACTION_MAX] = {0};
volatile int PulseOn[BLOCS_ACTION_MAX] = {0};
volatile int PulseTotal[BLOCS_ACTION_MAX] = {0};
volatile int PulseComptage[BLOCS_ACTION_MAX] = {0};
volatile int Gpio[BLOCS_ACTION_MAX] = {-1};
volatile int OutOn[BLOCS_ACTION_MAX] = {1};
volatile int OutOff[BLOCS_ACTION_MAX] = {0};

//=== Localisation ===
float latitude=48.8566; //Paris par defaut
float longitude=2.3522;
//=== ensoleillement ===
int SoleilDemain=0;
int SoleilActuel=0;
int SoleilJour=0;
int SunRiseJour=0;
int SunSetJour=0;
int SunRiseDemain=0;
int SunSetDemain=0;
int HnextSoleil=0;
EXT_RAM_BSS_ATTR int16_t Wm2[48];

byte ReacCACSI = 1;
bool EstimCACSI = false;
unsigned int FreqPWM = 500;  // Frequence signaux PWM en Hz

int16_t EthernetBug = 0;




String EnphaseSerial = "0";  //Sert égalemnet au Shelly comme numéro de voie

//RTE
String LTARF = ""; // Option tarifaire RTE