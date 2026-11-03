#include "Sources/Source_Linky.h"
#include <Arduino.h>
#include "config.h"
#include "Communications/comSortie.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "Communications/TempoRTE.h"
#include "Heure.h"


String STGE = "";  // Status Linky
String STGEt = ""; // Status Tempo uniquement RTE
String NGTF = "";  // Calendrier tarifaire
String JourLinky = "";

long EASF01 = 0;
long EASF02 = 0;
long EASF03 = 0;
long EASF04 = 0;
long EASF05 = 0;
long EASF06 = 0;
long EASF07 = 0;
long EASF08 = 0;
long EASF09 = 0;
long EASF10 = 0;

// Parameters for Linky
volatile int IdxDataRawLinky = 0;
volatile int IdxBufDecodLinky = 0;
EXT_RAM_BSS_ATTR char DataRawLinky[10000]; // Buffer entrée données Linky

static bool LFon = false;
static bool EASTvalid = false;
static bool EAITvalid = false;
static float moyPWS = 0;
static float moyPWI = 0;
static float moyPVAS = 0;
static float moyPVAI = 0;
static float COSphiS = 1;
static float COSphiI = 1;
static long TlastEASTvalide = 0;
static long TlastEAITvalide = 0;
static unsigned long previousETX;

// ****************************
// * Source de Mesures LINKY  *
// ****************************

static float deltaWS = 0;
static float deltaWI = 0;
static int boucle_appel_Linky = 0;

void AnalyseChar(int V, int8_t source);

void Setup_Linky(int8_t source)
{ // On considere qu'un seul Linky par Maison
  delay(20);
  if (LaSource[source].Port == 1)
  {
    Serial1.begin(9600, SERIAL_7E1, RXD1, TXD1); // 7-bit Even parity 1 stop bit pour le Linky
  }
  else
  {
    Serial2.begin(9600, SERIAL_7E1, RXD2, TXD2); // 7-bit Even parity 1 stop bit pour le Linky
  }

  delay(100);
  previousETX = millis();
}
static int pIRMS1, pIRMS2, pIRMS3 = -1, pURMS1, pURMS2, pURMS3, pSINSTS = 0, pSINSTS1 = 0, pSINSTS2, pSINSTS3, pPuissance;
void LectureLinky(int8_t source)
{ // Lecture port série du LINKY .
  int V = 0;

  boucle_appel_Linky++;
  if (boucle_appel_Linky > 4000)
  {
    boucle_appel_Linky = 0;
    if (LaSource[source].Port == 1)
    {
      Serial1.flush();
      Serial1.write("Ok");
    }
    else
    {
      Serial2.flush();
      Serial2.write("Ok");
    }
    StockMessage("Attente Linky 4000 boucles = 8s");
  }
  if (LaSource[source].Port == 1)
  {
    while (Serial1.available() > 0)
    {
      boucle_appel_Linky = 0;
      V = Serial1.read();
      AnalyseChar(V, source);
    }
  }
  else
  {
    while (Serial2.available() > 0)
    {
      boucle_appel_Linky = 0;
      V = Serial2.read();
      AnalyseChar(V, source);
    }
  }
}

void AnalyseChar(int V, int8_t source)
{
  long OldWh = 0;
  float deltaWh = 0;
  float Pmax = 0;
  float Pmin = 0;
  unsigned long Tm = 0;
  float deltaT = 0;
  int8_t C0 = source * 3; // Canal de mesure
  DataRawLinky[IdxDataRawLinky] = char(V);
  IdxDataRawLinky = (IdxDataRawLinky + 1) % 10000;
  switch (V)
  {
  case 2: // STX (Start Text)
    break;
  case 3: // ETX (End Text)
    previousETX = millis();
    LFon = false;
    break;
  case 10: // Line Feed. Debut Groupe
    LFon = true;
    IdxBufDecodLinky = IdxDataRawLinky;
    break;
  case 13: // Line Feed. Debut Groupe
    if (LFon)
    { // Debut groupe OK
      LFon = false;
      int nb_tab = 0;
      String code = "";
      String val = "";
      int checksum = 0;
      int checkLinky = -1;
      LaSourceVar[source].DataBrute = ""; // Efface les données brutes avant décodage
      while (IdxBufDecodLinky != IdxDataRawLinky)
      {
        LaSourceVar[source].DataBrute += DataRawLinky[IdxBufDecodLinky];
        if (DataRawLinky[IdxBufDecodLinky] == char(9))
        { // Tabulation
          nb_tab++;
        }
        else
        {
          if (nb_tab == 0)
          {
            code += DataRawLinky[IdxBufDecodLinky];
          }
          if (nb_tab == 1)
          {
            val += DataRawLinky[IdxBufDecodLinky];
          }
          if (nb_tab <= 1)
          {
            checksum += (int)DataRawLinky[IdxBufDecodLinky];
          }
        }
        IdxBufDecodLinky = (IdxBufDecodLinky + 1) % 10000;
        if (checkLinky == -1 && nb_tab == 2)
        {
          checkLinky = (int)DataRawLinky[IdxBufDecodLinky];
          checksum += 18;           // 2 tabulations
          checksum = checksum & 63; // 0x3F
          checksum = checksum + 32; // 0x20
        }
      }
      if (code.indexOf("EAST") == 0 || code.indexOf("EAIT") == 0 || code == "SINSTS" || code.indexOf("SINSTI") == 0)
      {
        if (checksum != checkLinky)
        {
          StockMessage("Erreur checksum code : " + code + " " + String(checksum) + "," + String(checkLinky));
        }
        else
        {
          if (code.indexOf("EAST") == 0)
          {

            OldWh = Energie_Soutiree[C0];
            if (OldWh == 0)
            {
              OldWh = val.toInt();
            }
            Energie_Soutiree[C0] = val.toInt();
            Tm = millis();
            deltaT = float(Tm - TlastEASTvalide);
            deltaT = deltaT / float(3600000);
            if (Energie_Soutiree[C0] == OldWh)
            { // Pas de resultat en Wh
              Pmax = 1.3 / deltaT;
              moyPWS = min(moyPWS, Pmax);
            }
            else
            {
              TlastEASTvalide = Tm;
              deltaWh = float(Energie_Soutiree[C0] - OldWh);
              deltaWS = deltaWh / deltaT;
              Pmin = (deltaWh - 1) / deltaT;
              moyPWS = max(moyPWS, Pmin); // saut à la montée en puissance
            }
            moyPWS = 0.05 * deltaWS + 0.95 * moyPWS;
            EASTvalid = true;
            if (!EAITvalid && Tm > 8000)
            { // Cas des CACSI ou EAIT n'est jamais positionné
              EAITvalid = true;
            }
          }
          if (code.indexOf("EAIT") == 0)
          {
            OldWh = Energie_Injectee[C0];
            if (OldWh == 0)
            {
              OldWh = val.toInt();
            }
            Energie_Injectee[C0] = val.toInt();
            Tm = millis();
            deltaT = float(Tm - TlastEAITvalide);
            deltaT = deltaT / float(3600000);
            if (Energie_Injectee[C0] == OldWh)
            { // Pas de resultat en Wh
              Pmax = 1.3 / deltaT;
              moyPWI = min(moyPWI, Pmax);
            }
            else
            {
              TlastEAITvalide = Tm;
              deltaWh = float(Energie_Injectee[C0] - OldWh);
              deltaWI = deltaWh / deltaT;
              Pmin = (deltaWh - 1) / deltaT;
              moyPWI = max(moyPWI, Pmin); // saut à la montée en puissance
            }
            moyPWI = 0.05 * deltaWI + 0.95 * moyPWI;
            EAITvalid = true;
          }

          if (code == "SINSTS")
          { // Puissance apparente soutirée. Egalité pour ne pas confondre avec SINSTS1 (triphasé)
            PVAS[C0] = PintMax(val.toInt());
            moyPVAS = 0.05 * float(PVAS[C0]) + 0.95 * moyPVAS;
            moyPWS = min(moyPWS, moyPVAS);
            if (moyPVAS > 0)
            {
              COSphiS = moyPWS / moyPVAS;
              COSphiS = min(float(1.0), COSphiS);
              PowerFactor[C0] = COSphiS;
            }
            PuissanceS[C0] = PintMax(int(COSphiS * float(PVAS[C0])));
            PVA_Valide[C0] = true;
          }
          if (code.indexOf("SINSTI") == 0)
          { // Puissance apparente injectée
            if (ReacCACSI != 100)
            {
              //???      PVAI_M = PintMax(val.toInt());
              //???     moyPVAI = 0.05 * float(PVAI_M) + 0.95 * moyPVAI;
              //???    moyPWI = min(moyPWI, moyPVAI);
              //???     if (moyPVAI > 0)
              //???    {
              //???      COSphiI = moyPWI / moyPVAI;
              //???      COSphiI = min(float(1.0), COSphiI);
              //???      PowerFactor[C0] = COSphiI;
              //???    }
              PuissanceI[C0] = PintMax(int(COSphiI * float(PVAI[C0])));
              PVA_Valide[C0] = true;
            }
            if (EASTvalid && EAITvalid)
            {
              Puissance_Max(C0);
              Filtre_Puissance(false, C0);
              AffectationPuissanceAuxActions(C0);
              CalculEnergieJour(C0, true);
              EnergieActiveValide[C0 + 1] = false;
              EnergieActiveValide[C0 + 2] = false;
            }
          }
        }
      }
      if (code.indexOf("DATE") == 0)
      {

        if (Horloge == 1)
        {

          struct tm t = {0};                             // toujours initialiser à 0
          t.tm_year = val.substring(1, 3).toInt() + 100; // années depuis 1900
          t.tm_mon = val.substring(3, 5).toInt() - 1;    // mois 0–11 (octobre = 9)
          t.tm_mday = val.substring(5, 7).toInt();       // jour du mois
          t.tm_hour = val.substring(7, 9).toInt();
          t.tm_min = val.substring(9, 11).toInt();
          t.tm_sec = val.substring(11, 13).toInt();

          time_t now = mktime(&t);

          // 🔹 Création d’une structure timeval pour settimeofday()
          struct timeval tv = {.tv_sec = now, .tv_usec = 0};
          settimeofday(&tv, nullptr); // mise à l'heure de l’ESP32

          FormatteHeureDate();
        }
      }

      if (ReacCACSI == 100)
      { // Estimateur ON
        if (code.indexOf("IRMS1") == 0)
        {
          pIRMS1 = val.toInt();
        }
        if (code.indexOf("IRMS2") == 0)
        {
          pIRMS2 = val.toInt();
        }
        if (code.indexOf("IRMS3") == 0)
        {
          pIRMS3 = val.toInt();
        }
        if (code.indexOf("URMS1") == 0)
        {
          pURMS1 = val.toInt();
        }
        if (code.indexOf("URMS2") == 0)
        {
          pURMS2 = val.toInt();
        }
        if (code.indexOf("URMS3") == 0)
        {
          pURMS3 = val.toInt();
        }
        if (code.indexOf("SINSTS1") == 0)
        {
          pSINSTS1 = val.toInt();
        }
        if (code.indexOf("SINSTS2") == 0)
        {
          pSINSTS2 = val.toInt();
        }
        if (code.indexOf("SINSTS3") == 0)
        {
          pSINSTS3 = val.toInt();
        }
        if (code == "SMAXSN")
        {

          PuissanceI[C0] = 0;
          if (PuissanceS[C0] == 0)
          { // estimation de la puissance d'injection si PuissanceS_M==0
            if (pIRMS3 != -1)
            {                                                                // triphasé
              pPuissance = 150 + (pSINSTS1 == 0 ? -1 : 1) * pURMS1 * pIRMS1; // marge de 150W, en mono SINSTS1==0 si PuissanceS_M==0
              pPuissance += (pSINSTS2 == 0 ? -1 : 1) * pURMS2 * pIRMS2;
              pPuissance += (pSINSTS3 == 0 ? -1 : 1) * pURMS3 * pIRMS3;
            }
            else
            {
              pPuissance = 150 + (pSINSTS == 0 ? -1 : 1) * pURMS1 * pIRMS1; // marge de 150W, en mono SINSTS==0 si PuissanceS_M==0
            }
            if (pPuissance < 0)
            { // estimation si l'écart est supérieur à 150W

              PuissanceI[C0] = -pPuissance; // "-" car on donne la valeur injectée
              PVAI[C0] = PuissanceI[C0];    // On egalise Pw et PVA
            }
          }
        }
      }
      // LJ STOP

      if (code.indexOf("URMS1") == 0)
      {
        Tension[C0] = val.toFloat(); // phase 1 uniquement
      }
      if (code.indexOf("IRMS1") == 0)
      {
        Intensite[C0] = val.toFloat(); // Phase 1 uniquement
      }
      if (code.indexOf("STGE") == 0)
      {
        STGE = val; // Status
        STGE.trim();
      }
      if (TempoRTEon == 0)
      { // On prend tarif sur Linky
        if (code.indexOf("LTARF") == 0)
        {
          LTARF = val; // Option Tarifaire
          LTARF.trim();
        }
        if (code.indexOf("STGE") == 0)
        {
          STGEt = STGE.substring(1, 2); // Tempo lendemain et jour sur 1 octet
        }
      }
      if (code.indexOf("NGTF") == 0)
      {
        NGTF = val; // Calendrier Tarifaire
        NGTF.trim();
      }
      if (code.indexOf("EASF01") == 0)
        EASF01 = val.toInt();
      if (code.indexOf("EASF02") == 0)
        EASF02 = val.toInt();
      if (code.indexOf("EASF03") == 0)
        EASF03 = val.toInt();
      if (code.indexOf("EASF04") == 0)
        EASF04 = val.toInt();
      if (code.indexOf("EASF05") == 0)
        EASF05 = val.toInt();
      if (code.indexOf("EASF06") == 0)
        EASF06 = val.toInt();
      if (code.indexOf("EASF07") == 0)
        EASF07 = val.toInt();
      if (code.indexOf("EASF08") == 0)
        EASF08 = val.toInt();
      if (code.indexOf("EASF09") == 0)
        EASF09 = val.toInt();
      if (code.indexOf("EASF10") == 0)
        EASF10 = val.toInt();
    }
    break;
  default:
    break;
  }
}