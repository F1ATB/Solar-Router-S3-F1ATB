// ******************************
// * Client d'un HomeWizard     *
// * ESSAI A VALIDER            *
// ******************************

#include "Sources/Source_HomeWizard.h"
#include <Arduino.h>
#include "config.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "commonFX.h"
#include "Communications/comSortie.h"
#include <WiFi.h>
#include "Communications/Internet.h"

void LectureHomeW(int8_t source)
{
  String S = "";
  String HomeW_Data = "";
  String Gr[4];
  String data_[20];
  int8_t C0 = source * 3; // Canal de mesures
  PVA_Valide[C0] = false;
  
  EnergieActiveValide[C0 + 1] = false;
  EnergieActiveValide[C0 + 2] = false;
  String host = IP2String(LaSource[source].IP);
  String url = "http://" + host + "/api/v1/data";
  int8_t IdxM = Idx_Machine(LaSource[source].IP);
  HomeW_Data = httpGetPayload(url, IdxM);
  if (HomeW_Data == "")
    return;

  int p = HomeW_Data.indexOf("{");
  HomeW_Data = HomeW_Data.substring(p + 1);
  p = HomeW_Data.indexOf("}");
  HomeW_Data = HomeW_Data.substring(0, p);

  if (PfloatMax(ValJsonSG("active_power_w", HomeW_Data)) > 0)
  {
    PuissanceS_inst[C0] = PfloatMax(ValJsonSG("active_power_w", HomeW_Data));
    PuissanceI_inst[C0] = 0;
  }
  else
  {
    PuissanceS_inst[C0] = 0;
    PuissanceI_inst[C0] = -PfloatMax(ValJsonSG("active_power_w", HomeW_Data));
  }
  long EnergyDeliveredTariff1 = int(1000 * ValJsonSG("total_power_import_t1_kwh", HomeW_Data));
  long EnergyDeliveredTariff2 = int(1000 * ValJsonSG("total_power_import_t2_kwh", HomeW_Data));
  Energie_Soutiree[C0] = EnergyDeliveredTariff1 + EnergyDeliveredTariff2;
  long EnergyReturnedTariff1 = int(1000 * ValJsonSG("total_power_export_t1_kwh", HomeW_Data));
  long EnergyReturnedTariff2 = int(1000 * ValJsonSG("total_power_export_t2_kwh", HomeW_Data));
  Energie_Injectee[C0] = EnergyReturnedTariff1 + EnergyReturnedTariff2;
  LaSourceVar[source].DataBrute = HomeW_Data;
  Filtre_Puissance(true, C0);
  Puissance_Max(C0);
  AffectationPuissanceAuxActions(C0);
  CalculEnergieJour(C0, true);
 
}
