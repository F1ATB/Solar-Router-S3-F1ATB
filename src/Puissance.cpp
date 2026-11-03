#include "Puissance.h"
#include "BlocActions.h"
#include "Ecrans/LEDs.h"
#include "config.h"
#include <Arduino.h>

SourcePuissance LaSource[NB_SOURCES_DEVICE_MAX];
SourceVariable LaSourceVar[NB_SOURCES_DEVICE_MAX];
// 3 canaux par device
float Tension[NB_CANAUX_MAX], Intensite[NB_CANAUX_MAX], PowerFactor[NB_CANAUX_MAX], Frequence_[NB_CANAUX_MAX];
long Energie_Soutiree[NB_CANAUX_MAX], Energie_Injectee[NB_CANAUX_MAX];
long EnergieJour_Soutiree[NB_CANAUX_MAX], EnergieJour_Injectee[NB_CANAUX_MAX];
long EnergieJour0_Soutiree[NB_CANAUX_MAX], EnergieJour0_Injectee[NB_CANAUX_MAX];
float PuissanceS_inst[NB_CANAUX_MAX], PuissanceI_inst[NB_CANAUX_MAX], Puissance_moy[NB_CANAUX_MAX];
float PVAS_inst[NB_CANAUX_MAX], PVAI_inst[NB_CANAUX_MAX], PVA_moy[NB_CANAUX_MAX];
int PuissanceS[NB_CANAUX_MAX], PuissanceI[NB_CANAUX_MAX];
int PuissanceS_Max[NB_CANAUX_MAX], PuissanceI_Max[NB_CANAUX_MAX];
int PVAS[NB_CANAUX_MAX], PVAI[NB_CANAUX_MAX];
double EAS_float[NB_CANAUX_MAX], EAI_float[NB_CANAUX_MAX];
bool EnergieActiveValide[NB_CANAUX_MAX], PVA_Valide[NB_CANAUX_MAX];
int PactConso_M, PactProd; // Pour Enphase
long LastwhDlvdCum = 0;    // Dernière valeur cumul Wh Soutire.
long LastwhRcvdCum = 0;

int16_t PuissanceValide = 5;
bool LissageLong = false;
bool PuissanceRecue = false;

static float PmaxReseau = 36000; // Puissance Max pour eviter des débordements

void RAZ_Source_Def(int8_t source);
void RAZ_Source_Var(int8_t source);

void Init_Puissance() {
  for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++) {
    RAZ_Source_Def(source);
    RAZ_Source_Var(source);
  }
  LaSource[0].Type = "NotDef";
  LaSource[0].Data = "NotDef";
  LaSource[0].CanalNom[0] = "Source A Définir";
  LaSource[0].CanalVoie[0] = 0;
  LaSource[0].NomSoutir[0] = "Soutirée";
  LaSource[0].NomInject[0] = "Injectée";
}
void RAZ_Source_Def(int8_t source) // Définition de l'objet
{
  LaSource[source].Type = "";
  LaSource[source].Data = "";   // Format des Data des Sources de puissance externes
  LaSource[source].IP = 0;      // IP des sources externes de puissance
  LaSource[source].Port = 1;    // Port Série pour les UxIx2 etc
  LaSource[source].TopicP = ""; // MQTT Topic Puissance
  for (int i = 0; i < NB_CANAUX_PAR_SOURCE; i++) {
    LaSource[source].CanalNom[i] = "";
    LaSource[source].CanalVoie[i] = -1; //-1=non affecté
    LaSource[source].NomSoutir[i] = "Conso.";
    LaSource[source].NomInject[i] = ""; // Pas d'injection par défaut
  }
}
void RAZ_Source_Var(int8_t source) // Variables de l'objet
{

  LaSourceVar[source].RAZ_JSY = false;
  LaSourceVar[source].DataBrute = "";
  LaSourceVar[source].Temps_precedent = 0;
  LaSourceVar[source].Modele = "";
  LaSourceVar[source].Profile = "";

  int8_t j = source * NB_CANAUX_PAR_SOURCE;
  for (int8_t i = j; i < j + NB_CANAUX_PAR_SOURCE; i++) {
    Puissance_moy[i] = 0;
    PVA_moy[i] = 0;
    PuissanceS_inst[i] = 0;
    PuissanceI_inst[i] = 0;

    PVAS_inst[i] = 0;
    PVAI_inst[i] = 0;
    PVA_moy[i] = 0;
    PuissanceS[i] = 0;
    PuissanceI[i] = 0;
    PuissanceS_Max[i] = 0;
    PuissanceI_Max[i] = 0;
    PVAS[i] = 0;
    PVAI[i] = 0;
    PowerFactor[i] = 1.0;
    EAS_float[i] = 0;
    EAI_float[i] = 0;
    EnergieJour_Soutiree[i] = 0;
    EnergieJour_Injectee[i] = 0;
    EnergieJour0_Soutiree[i] = 0;
    EnergieJour0_Injectee[i] = 0;
    EnergieActiveValide[i] = false;
    PVA_Valide[i] = false;
  }
}

void CalculEnergieJour(int8_t canal, bool A_calculer) {
  if (A_calculer) // Quand on est en externe, c'est le maître qui calcul
  {
    if (Energie_Soutiree[canal] > 0 && (Energie_Soutiree[canal] < EnergieJour0_Soutiree[canal] || EnergieJour0_Soutiree[canal] == 0)) {
      EnergieJour0_Soutiree[canal] = Energie_Soutiree[canal];
    }
    if (EnergieJour0_Soutiree[canal] > 0) {
      EnergieJour_Soutiree[canal] += Energie_Soutiree[canal] - EnergieJour0_Soutiree[canal]; // On accumule
      EnergieJour0_Soutiree[canal] = Energie_Soutiree[canal];
    }
    if (Energie_Injectee[canal] > 0 && (Energie_Injectee[canal] < EnergieJour0_Injectee[canal] || EnergieJour0_Injectee[canal] == 0)) {
      EnergieJour0_Injectee[canal] = Energie_Injectee[canal];
    }
    if (EnergieJour0_Injectee[canal]) {
      EnergieJour_Injectee[canal] += Energie_Injectee[canal] - EnergieJour0_Injectee[canal];
      EnergieJour0_Injectee[canal] = Energie_Injectee[canal];
    }
  }
  EnergieActiveValide[canal] = true;
  if (canal < 3) { // Canal de mesure principal servant au routeur
    ValidToLED();
    PuissanceRecue = true; // Reset du Watchdog à chaque trame  reçue
  }
}
void RAZ_Energie_Le_Matin() {
  for (int8_t canal = 0; canal < NB_CANAUX_MAX; canal++) {
    EnergieJour0_Soutiree[canal] = 0;
    EnergieJour0_Injectee[canal] = 0;
    EnergieJour_Soutiree[canal] = 0;
    EnergieJour_Injectee[canal] = 0;
    EnergieActiveValide[canal] = false;
  }
}
void Puissance_Max(int8_t canal) {
  PuissanceS_Max[canal] = max(PuissanceS_Max[canal], PuissanceS[canal]);
  PuissanceI_Max[canal] = max(PuissanceI_Max[canal], PuissanceI[canal]);
}
void Filtre_Puissance(bool Float, int8_t canal) { // Filtre RC

  if (!Float) {
    PuissanceS_inst[canal] = float(PuissanceS[canal]);
    PuissanceI_inst[canal] = float(PuissanceI[canal]);
    PVAS_inst[canal] = float(PVAS[canal]);
    PVAI_inst[canal] = float(PVAI[canal]);
  }

  if (LissageLong) {
    float A = 0.3; // Coef pour un lissage en multi-sinus et train de sinus sur les mesures de puissance courte
    float B = 0.7;
    Puissance_moy[canal] = A * (PuissanceS_inst[canal] - PuissanceI_inst[canal]) + B * Puissance_moy[canal];
    PVA_moy[canal] = A * (PVAS_inst[canal] - PVAI_inst[canal]) + B * PVA_moy[canal]; // Puissance VA affichée en entiers
  } else {
    Puissance_moy[canal] = PuissanceS_inst[canal] - PuissanceI_inst[canal];
    PVA_moy[canal] = (PVAS_inst[canal] - PVAI_inst[canal]);
  }

  if (Puissance_moy[canal] < 0) {
    PuissanceI[canal] = -int(Puissance_moy[canal]); // Puissance Watt affichée en entier
    PuissanceS[canal] = 0;
  } else {
    PuissanceS[canal] = int(Puissance_moy[canal]);
    PuissanceI[canal] = 0;
  }

  if (PVA_moy[canal] < 0) {
    PVAI[canal] = -int(PVA_moy[canal]);
    PVAS[canal] = 0;
  } else {
    PVAS[canal] = int(PVA_moy[canal]);
    PVAI[canal] = 0;
  }
}
//*************
//* Test Pmax *
//*************
float PfloatMax(float Pin) { return constrain(Pin, float(-1.0F * PmaxReseau), float(PmaxReseau)); }

int PintMax(int Pin) { return constrain(Pin, int(-1 * PmaxReseau), int(PmaxReseau)); }

//=== Remplissage tableau des Sorties des blocs de puissance pour les Actions=======
void AffectationPuissanceAuxActions(int8_t canal) {
  for (int i = 0; i < BLOCS_CAPTEUR_MAX; i++) {
    if (BlocsCapteurs[i].NumBloc != 255) {

      if (BlocsCapteurs[i].type == "puissance") {
        int bloc = BlocsCapteurs[i].NumBloc;
        if (BlocsCapteurs[i].value == "P" + String(canal)) {
          SortieNum0[bloc] = Puissance_moy[canal]; // Valeur soutirée - Injectée
        }
      }
    } else {
      break; // On sort si on a plus de bloc capteur
    }
  }
}
