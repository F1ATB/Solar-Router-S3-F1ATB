
// ******************************************************
// * Informations de puissance reçue via un Broker MQTT *
// ******************************************************
#include "Sources/Source_MQTT.h"
#include <Arduino.h>
#include "config.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "Communications/MQTT.h"
#include "Communications/comSortie.h"
#include "Heure.h"

void UpdatePmqtt(int8_t source)
{
  LaSourceVar[source].DataBrute = P_MQTT_Brute; // Sauvegarde des données brutes
  float Pw = PfloatMax(PwMQTT);
  float Pf = 1;
  int8_t C0 = source * 3; // Canal de mesure
  if (P_MQTT_Brute.indexOf("Pf") > 0)
  {
    Pf = abs(PfMQTT);
  }
  if (P_MQTT_Brute.indexOf("Pva") > 0)
  {
    if (PvaMQTT != 0)
    {
      Pf = abs(Pw / PfloatMax(PvaMQTT));
    }
  }
  if (P_MQTT_Brute.indexOf("Pva") > 0 || P_MQTT_Brute.indexOf("Pf") > 0)
  {
    PVA_Valide[C0] = true;
  }
  else
  {
    PVA_Valide[C0] = false;
  }
  if (Pf > 1)
    Pf = 1;
  if (Pw >= 0)
  {
    PuissanceS_inst[C0] = Pw;
    PuissanceI_inst[C0] = 0;
    if (Pf > 0.01)
    {
      PVAS_inst[C0] = PfloatMax(Pw / Pf);
    }
    else
    {
      PVAS_inst[C0] = 0;
    }
    PVAI_inst[C0] = 0;
    EAS_float[C0] += Pw / 6000.0;                 // Watt Hour,Every 600ms. Soutirée
    Energie_Soutiree[C0] = (long)(EAS_float[C0]); // Watt Hour,Every 40ms. Soutirée
  }
  else
  {
    PuissanceS_inst[C0] = 0;
    PuissanceI_inst[C0] = -Pw;
    if (Pf > 0.01)
    {
      PVAI_inst[C0] = PfloatMax(-Pw / Pf);
    }
    else
    {
      PVAI_inst[C0] = 0;
    }
    PVAS_inst[C0] = 0;
    EAI_float[C0] += -Pw / 6000.0;
    Energie_Injectee[C0] = (long)(EAI_float[C0]);
  }

  Filtre_Puissance(true, C0);
  Puissance_Max(C0);
  AffectationPuissanceAuxActions(C0);
  if ((P_MQTT_Brute.indexOf("Pw") > 0) && (millis() - LastPwMQTTMillis < 30000))
    CalculEnergieJour(C0, true);
  EnergieActiveValide[C0 + 1] = false;
  EnergieActiveValide[C0 + 2] = false;
}