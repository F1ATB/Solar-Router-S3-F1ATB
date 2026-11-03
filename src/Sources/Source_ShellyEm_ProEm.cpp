// ****************************************************
// * Client d'un Shelly Em sur voie 0 ou 1 ou triphasé*
// * Pour la version Pro voir plus bas                *
// ****************************************************

#include "Sources/Source_ShellyEm_ProEm.h"
#include <Arduino.h>
#include "config.h"
#include "Communications/comSortie.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "commonFX.h"
#include <WiFi.h>
#include "Communications/Internet.h"

// Paramètres for Shelly Em

void LectureShellyEm(int8_t source, int8_t canalV)
{
  String S = "";
  String Shelly_Data = "";
  float Pw = 0;
  float voltage = 0;
  float pf = 0;
  float Pva;
  int p = 0;
  int8_t voie = LaSource[source].CanalVoie[canalV];
  int8_t C0 = source * 3 + canalV;

  String url = "/emeter/" + String(voie);
  if (voie == 3)
    url = "/status"; // Triphasé Shelly 3Em
  if (voie >= 30)
    url = "/rpc/Shelly.GetStatus";

  String host = IP2String(LaSource[source].IP);
  int8_t IdxM = Idx_Machine(LaSource[source].IP);

  url = "http://" + host + url;
  Shelly_Data = httpGetPayload(url, IdxM);
  EnergieActiveValide[C0] = false;
  if (Shelly_Data == "")
    return;

  if (voie == 3)
  { // Triphasé
    if (Shelly_Data.indexOf("fs_mounted") >= 10)
      Shelly_Data = Shelly_Data.substring(0, Shelly_Data.indexOf("fs_mounted") - 10);
    LaSourceVar[source].DataBrute = "<strong>Triphasé</strong>" +GS + Shelly_Data;
    p = Shelly_Data.indexOf("emeters");
    Shelly_Data = Shelly_Data.substring(p + 10);
    Pw = PfloatMax(ValJson("power", Shelly_Data)); // Phase 1
    pf = ValJson("pf", Shelly_Data);
    pf = abs(pf);
    float total_Pw = Pw;
    float total_Pva = 0;
    if (pf > 0)
    {
      total_Pva = abs(Pw) / pf;
    }
    float total_E_soutire = ValJson("total", Shelly_Data);
    float total_E_injecte = ValJson("total_returned", Shelly_Data);
    p = Shelly_Data.indexOf("}");
    Shelly_Data = Shelly_Data.substring(p + 1);
    Pw = PfloatMax(ValJson("power", Shelly_Data)); // Phase 2
    pf = ValJson("pf", Shelly_Data);
    pf = abs(pf);
    total_Pw += Pw;
    if (pf > 0)
    {
      total_Pva += abs(Pw) / pf;
    }
    total_E_soutire += ValJson("total", Shelly_Data);
    total_E_injecte += ValJson("total_returned", Shelly_Data);
    p = Shelly_Data.indexOf("}");
    Shelly_Data = Shelly_Data.substring(p + 1);
    Pw = PfloatMax(ValJson("power", Shelly_Data)); // Phase 3
    pf = ValJson("pf", Shelly_Data);
    pf = abs(pf);
    total_Pw += Pw;
    if (pf > 0)
    {
      total_Pva += abs(Pw) / pf;
    }
    total_E_soutire += ValJson("total", Shelly_Data);
    total_E_injecte += ValJson("total_returned", Shelly_Data);
    Energie_Soutiree[C0] = int(total_E_soutire);
    Energie_Injectee[C0] = int(total_E_injecte);
    if (total_Pw == 0)
    {
      total_Pva = 0;
    }
    if (total_Pw > 0)
    {
      PuissanceS_inst[C0] = total_Pw;
      PuissanceI_inst[C0] = 0;
      PVAS_inst[C0] = total_Pva;
      PVAI_inst[C0] = 0;
    }
    else
    {
      PuissanceS_inst[C0] = 0;
      PuissanceI_inst[C0] = -total_Pw;
      PVAI_inst[C0] = total_Pva;
      PVAS_inst[C0] = 0;
    }

    Filtre_Puissance(true, C0);
    AffectationPuissanceAuxActions(C0);
    Puissance_Max(C0);
    CalculEnergieJour(C0, true);
  }
  else if (voie < 3)
  { // Monophasé Shelly Em de base
    LaSourceVar[source].DataBrute = "<strong>Voie : " + String(voie) + "</strong>" + "|*|" + Shelly_Data;
    Shelly_Data = Shelly_Data + ",";
    if (Shelly_Data.indexOf("true") > 0)
    { // Donnée valide
      Pw = PfloatMax(ValJson("power", Shelly_Data));
      voltage = ValJson("voltage", Shelly_Data);
      pf = ValJson("pf", Shelly_Data);
      pf = abs(pf);
      if (pf > 1)
        pf = 1;

      if (Pw >= 0)
      {
        PuissanceS_inst[C0] = Pw;
        PuissanceI_inst[C0] = 0;
        if (pf > 0.01)
        {
          PVAS_inst[C0] = PfloatMax(Pw / pf);
        }
        else
        {
          PVAS_inst[C0] = 0;
        }
        PVAI_inst[C0] = 0;
      }
      else
      {
        PuissanceS_inst[C0] = 0;
        PuissanceI_inst[C0] = -Pw;
        if (pf > 0.01)
        {
          PVAI_inst[C0] = PfloatMax(-Pw / pf);
        }
        else
        {
          PVAI_inst[C0] = 0;
        }
        PVAS_inst[C0] = 0;
      }
      Energie_Soutiree[C0] = int(ValJson("total", Shelly_Data));
      Energie_Injectee[C0] = int(ValJson("total_returned", Shelly_Data));
      PowerFactor[C0] = pf;
      Tension[C0] = voltage;
      PVA_Valide[C0] = true;
    }

    Filtre_Puissance(true, C0);
    AffectationPuissanceAuxActions(C0);
    Puissance_Max(C0);
    CalculEnergieJour(C0, true);
  }
  else
  { // Shelly Em Gen3  . On recupere les 2 voies à chaque message

    LaSourceVar[source].DataBrute = "<strong>Shelly Em Gen3 sur 2 voies</strong>"  + String("|*|") + Shelly_Data;
    if (Shelly_Data.indexOf("ssid") > 0)
    { // Donnée valide
      Shelly_Data = SubJson("em1:0", "}", Shelly_Data);
      Pw = PfloatMax(ValJson("act_power", Shelly_Data));
      pf = ValJson("pf", Shelly_Data);
      voltage = ValJson("voltage", Shelly_Data);
      if (Pw >= 0)
      {
        PuissanceS_inst[C0] = Pw;
        PuissanceI_inst[C0] = 0;
        if (pf > 0.01)
        {
          PVAS_inst[C0] = PfloatMax(Pw / pf);
        }
        else
        {
          PVAS_inst[C0] = 0;
        }
        PVAI_inst[C0] = 0;
      }
      else
      {
        PuissanceS_inst[C0] = 0;
        PuissanceI_inst[C0] = -Pw;
        if (pf > 0.01)
        {
          PVAI_inst[C0] = PfloatMax(-Pw / pf);
        }
        else
        {
          PVAI_inst[C0] = 0;
        }
        PVAS_inst[C0] = 0;
      }
      Shelly_Data = SubJson("em1data:0", "}", LaSourceVar[source].DataBrute);
      Energie_Soutiree[C0] = int(ValJson("total_act_energy", Shelly_Data));
      Energie_Injectee[C0] = int(ValJson("total_act_ret_energy", Shelly_Data));
      PowerFactor[C0] = pf;
      Tension[C0] = voltage;
      Filtre_Puissance(true, C0);
      AffectationPuissanceAuxActions(C0);
      Puissance_Max(C0);
      PVA_Valide[C0] = true;
      CalculEnergieJour(C0, true);

      C0++;
      Shelly_Data = SubJson("em1:1", "}", LaSourceVar[source].DataBrute);
      Pw = PfloatMax(ValJson("act_power", Shelly_Data));
      pf = ValJson("pf", Shelly_Data);
      if (Pw >= 0)
      {
        PuissanceS_inst[C0] = Pw;
        PuissanceI_inst[C0] = 0;
        if (pf > 0.01)
        {
          PVAS_inst[C0] = PfloatMax(Pw / pf);
        }
        else
        {
          PVAS_inst[C0] = 0;
        }
        PVAI_inst[C0] = 0;
      }
      else
      {
        PuissanceS_inst[C0] = 0;
        PuissanceI_inst[C0] = -Pw;
        if (pf > 0.01)
        {
          PVAI_inst[C0] = PfloatMax(-Pw / pf);
        }
        else
        {
          PVAI_inst[C0] = 0;
        }
        PVAS_inst[C0] = 0;
      }
      Shelly_Data = SubJson("em1data:1", "}", LaSourceVar[source].DataBrute);
      Energie_Soutiree[C0] = int(ValJson("total_act_energy", Shelly_Data));
      Energie_Injectee[C0] = int(ValJson("total_act_ret_energy", Shelly_Data));
      PowerFactor[C0] = pf;
      Tension[C0] = voltage;
      Filtre_Puissance(true, C0);
      AffectationPuissanceAuxActions(C0);
      Puissance_Max(C0);
      CalculEnergieJour(C0, true);
    }
  }
}

//*********************************************************************
// Variante Shelly Pro Em proposé par Raphael591 (Juillet 2024)       *
//  + Correction Octobre 2024  et Janvier 2025                        *
// ********************************************************************
// dash (février 2025) - Adaptation à l'utilisation du Pro 3EM en     *
// monophasé tout en ayant conservé le profile du matériel en triphasé*
// ********************************************************************
// * Paramétrage de la voie pour le shelly pro 3 em :                 *
// *   - 3 sur le triphasé.                                           *
// *   - 0 ou 1 ou 2 sur le monophasé fonction de la pince à utiliser.*
// ********************************************************************

void LectureShellyProEm(int8_t source, int8_t canalV)
{
  String S = "";
  String Shelly_Data = "";
  bool Shelly_Triphase_As_Monophase = false;
  String Shelly_Phase = "";
  float Pw = 0;
  float voltage = 0;
  float pf = 0;

  // ADD PERSO : AJOUT VARIABLE JSON pour facilité la lecture des infos EM PRO
  String tmp; // ADD PERSO

  int8_t voie = LaSource[source].CanalVoie[canalV];
  int8_t C0 = source * 3 + canalV;

  int p;

  int8_t IdxM = Idx_Machine(LaSource[source].IP);
  // Connaître modèle du shelly *******************************************
  if (LaSourceVar[source].Modele == "")
  {
    String url = "http://" + IP2String(LaSource[source].IP) + "/rpc/Shelly.GetDeviceInfo";

    Shelly_Data = httpGetPayload(url, IdxM);
    if (Shelly_Data == "")
      return;
    LaSourceVar[source].Modele = StringJson("id", Shelly_Data);
    p = LaSourceVar[source].Modele.indexOf("-");
    LaSourceVar[source].Modele = LaSourceVar[source].Modele.substring(0, p);
    LaSourceVar[source].Profile = StringJson("profile", Shelly_Data);
    Shelly_Data = "";
  }
  // Modèle shelly FIN ******

  // Protocole monophasé ou triphasé DÉBUT
  if (LaSourceVar[source].Profile == "triphase" && voie != 3)
  {
    Shelly_Triphase_As_Monophase = true;
  }
  // Protocole monophasé ou triphasé FIN
  String url = "http://" + IP2String(LaSource[source].IP) + "/rpc/Shelly.GetStatus";
  Shelly_Data = httpGetPayload(url, IdxM);
  EnergieActiveValide[C0] = false;
  if (Shelly_Data == "")
    return;
  if (LaSourceVar[source].Modele == "shellypro3em" && voie == 3)
  {
    // 3 em Triphasé
    LaSourceVar[source].DataBrute = "<strong>" + LaSourceVar[source].Modele + "</strong>" + "|*|" + Shelly_Data;

    float pf1, pf2, pf3;
    float volt1, volt2, volt3;

    tmp = PrefiltreJson("em:0", ":", Shelly_Data);

    Pw = ValJson("total_act_power", tmp);

    volt1 = ValJson("a_voltage", tmp);
    pf1 = ValJson("a_pf", tmp);

    volt2 = ValJson("b_voltage", tmp);
    pf2 = ValJson("b_pf", tmp);

    volt3 = ValJson("c_voltage", tmp);
    pf3 = ValJson("c_pf", tmp);

    voltage = (volt1 + volt2 + volt3) / 3;
    pf = abs((pf1 + pf2 + pf3) / 3);
    if (pf > 1)
      pf = 1;
    if (Pw >= 0)
    {
      PuissanceS_inst[C0] = Pw;
      PuissanceI_inst[C0] = 0;
      if (pf > 0.01)
      {
        PVAS_inst[C0] = PfloatMax(Pw / pf);
      }
      else
      {
        PVAS_inst[C0] = 0;
      }
      PVAI_inst[C0] = 0;
    }
    else
    {
      PuissanceS_inst[C0] = 0;
      PuissanceI_inst[C0] = -Pw;
      if (pf > 0.01)
      {
        PVAI_inst[C0] = PfloatMax(-Pw / pf);
      }
      else
      {
        PVAI_inst[C0] = 0;
      }
      PVAS_inst[C0] = 0;
    }
    tmp = PrefiltreJson("emdata:0", ":", Shelly_Data);       // ADD PERSO
    Energie_Soutiree[C0] = myLongJson("total_act", tmp);     // ADD PERSO
    Energie_Injectee[C0] = myLongJson("total_act_ret", tmp); // ADD PERSO
    PowerFactor[C0] = pf;
    Tension[C0] = voltage;
    PVA_Valide[C0] = true;

    CalculEnergieJour(C0, true);
  }
  else if (LaSourceVar[source].Modele == "shellypro3em" && Shelly_Triphase_As_Monophase)
  {
    // on utilise le code ASCII de a (97) pour obtenir le prefix de la phase. Voie 0=a 1=b 2=c
    Shelly_Phase = char(voie + 97);

    // 3 em Monophasé : Voie != 3 et profile Triphase actif
    LaSourceVar[source].DataBrute = "<strong>" + LaSourceVar[source].Modele + " " + "Phase " + Shelly_Phase + "</strong>" + "|*|" + Shelly_Data;

    tmp = PrefiltreJson("em:0", ":", Shelly_Data);

    Pw = ValJson(Shelly_Phase + "_act_power", tmp);
    voltage = ValJson(Shelly_Phase + "_voltage", tmp);
    pf = abs(ValJson(Shelly_Phase + "_pf", tmp));

    if (pf > 1)
      pf = 1;
    if (Pw >= 0)
    {
      PuissanceS_inst[C0] = Pw;
      PuissanceI_inst[C0] = 0;
      if (pf > 0.01)
      {
        PVAS_inst[C0] = PfloatMax(Pw / pf);
      }
      else
      {
        PVAS_inst[C0] = 0;
      }
      PVAI_inst[C0] = 0;
    }
    else
    {
      PuissanceS_inst[C0] = 0;
      PuissanceI_inst[C0] = -Pw;
      if (pf > 0.01)
      {
        PVAI_inst[C0] = PfloatMax(-Pw / pf);
      }
      else
      {
        PVAI_inst[C0] = 0;
      }
      PVAS_inst[C0] = 0;
    }
    tmp = PrefiltreJson("emdata:0", ":", Shelly_Data);                              // ADD PERSO
    Energie_Soutiree[C0] = myLongJson(Shelly_Phase + "_total_act_energy", tmp);     // ADD PERSO
    Energie_Injectee[C0] = myLongJson(Shelly_Phase + "_total_act_ret_energy", tmp); // ADD PERSO
    PowerFactor[C0] = pf;
    Tension[C0] = voltage;
    PVA_Valide[C0] = true;
    CalculEnergieJour(C0, true);
  }
  else if (LaSourceVar[source].Modele == "shellypro3em")
  {
    // 3 em Monophasé : Voie != 3
    LaSourceVar[source].DataBrute = "<strong>" + LaSourceVar[source].Modele + "</strong>" + "|*|" + Shelly_Data;

    tmp = PrefiltreJson("em1:" + String(voie), ":", Shelly_Data); // Voie mono
    Pw = ValJson("act_power", tmp);
    voltage = ValJson("voltage", tmp);
    pf = ValJson("pf", tmp);
    pf = abs(pf);
    if (pf > 1)
      pf = 1;
    if (Pw >= 0)
    {
      PuissanceS_inst[C0] = Pw;
      PuissanceI_inst[C0] = 0;
      if (pf > 0.01)
      {
        PVAS_inst[C0] = PfloatMax(Pw / pf);
      }
      else
      {
        PVAS_inst[C0] = 0;
      }
      PVAI_inst[C0] = 0;
    }
    else
    {
      PuissanceS_inst[C0] = 0;
      PuissanceI_inst[C0] = -Pw;
      if (pf > 0.01)
      {
        PVAI_inst[C0] = PfloatMax(-Pw / pf);
      }
      else
      {
        PVAI_inst[C0] = 0;
      }
      PVAS_inst[C0] = 0;
    }
    tmp = PrefiltreJson("em1data:" + String(voie), ":", Shelly_Data); // ADD PERSO
    Energie_Soutiree[C0] = myLongJson("total_act_energy", tmp);       // ADD PERSO
    Energie_Injectee[C0] = myLongJson("total_act_ret_energy", tmp);   // ADD PERSO
    PowerFactor[C0] = pf;
    Tension[C0] = voltage;
    PVA_Valide[C0] = true;
    CalculEnergieJour(C0, true);
  }
  else if (LaSourceVar[source].Modele == "shellyproem50")
  { // Monophasé pro EM

    // redifinition proEm50 2 voie (0 ou 1)

    LaSourceVar[source].DataBrute = "<strong>" + LaSourceVar[source].Modele + "</strong>" + "|*|" + Shelly_Data;
    Shelly_Data = Shelly_Data + ",";
    if (Shelly_Data.indexOf("act_power") > 0)
    {                                                               // Donnée valide
      tmp = PrefiltreJson("em1:" + String(voie), ":", Shelly_Data); // ADD PERSO
      Pw = ValJson("act_power", tmp);                               // ADD PERSO
      voltage = ValJson("voltage", tmp);                            // ADD PERSO
      pf = ValJson("pf", tmp);                                      // ADD PERSO
      pf = abs(pf);
      if (pf > 1)
        pf = 1;

      if (Pw >= 0)
      {
        PuissanceS_inst[C0] = Pw;
        PuissanceI_inst[C0] = 0;
        if (pf > 0.01)
        {
          PVAS_inst[C0] = PfloatMax(Pw / pf);
        }
        else
        {
          PVAS_inst[C0] = 0;
        }
        PVAI_inst[C0] = 0;
      }
      else
      {
        PuissanceS_inst[C0] = 0;
        PuissanceI_inst[C0] = -Pw;
        if (pf > 0.01)
        {
          PVAI_inst[C0] = PfloatMax(-Pw / pf);
        }
        else
        {
          PVAI_inst[C0] = 0;
        }
        PVAS_inst[C0] = 0;
      }
      tmp = PrefiltreJson("em1data:" + String(voie), ":", Shelly_Data); // ADD PERSO
      Energie_Soutiree[C0] = myLongJson("total_act_energy", tmp);       // ADD PERSO
      Energie_Injectee[C0] = myLongJson("total_act_ret_energy", tmp);   // ADD PERSO
      PowerFactor[C0] = pf;
      Tension[C0] = voltage;
      PVA_Valide[C0] = true;
      CalculEnergieJour(C0, true);
    }
  }

  Filtre_Puissance(true, C0);

  AffectationPuissanceAuxActions(C0);
  Puissance_Max(C0);
}
