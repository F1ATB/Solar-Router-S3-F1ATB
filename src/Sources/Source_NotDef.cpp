//****************************************
// Source de puissance non définie       *
//  Simulation de données                *
// ***************************************
#include "Sources/Source_NotDef.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"

void LectureNotDef(int8_t source) {
  int modulo=2000+source*100;
  float Pw = float(int(millis() / 30) % modulo - 900); //Simulation
  LaSourceVar[source].DataBrute = "Puissance simulée: " + String(Pw) + " W"; //Sauvegarde des données brutes
  int8_t C0=source * 3;  //Canal sonde 
  if (Pw >= 0) {
    PuissanceS_inst[C0] = Pw;
    PuissanceI_inst[C0] = 0;
    PVAS_inst[C0] = Pw*1.3;
    PVAI_inst[C0] = 0;
    EAS_float[C0] += Pw / 6000.0;             // Watt Hour,Every 600ms. Soutirée
    Energie_Soutiree[C0] = (long)(EAS_float[C0]);  //
  } else {
    PuissanceS_inst[C0] = 0;
    PuissanceI_inst[C0] = -Pw;
    PVAI_inst[C0] = -Pw*1.3;
    PVAS_inst[C0] = 0;
    EAI_float[C0] += -Pw / 6000.0;
    Energie_Injectee[C0] = (long)(EAI_float[C0]);
  }

  Filtre_Puissance(true,C0);
  Puissance_Max(C0);
  CalculEnergieJour(C0, true);
  EnergieActiveValide[C0+1] = false;
  EnergieActiveValide[C0+2] = false;
  AffectationPuissanceAuxActions(C0);
}