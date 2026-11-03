// *******************************
// * Source de Mesures UI Double *
// *      Capteur JSY-MK-194     *
// *******************************

#include "Sources/Source_UxIx2.h"
#include <Arduino.h>
#include "config.h"
#include "Communications/comSortie.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "Communications/TempoRTE.h"
#include "Ecrans/Ecran.h"  //  ??? A REtirer

void Setup_UxIx2(int8_t source)
{
  if (LaSource[source].Port == 1)
  {
    Serial1.begin(4800, SERIAL_8N1, RXD1, TXD1); // PORT DE CONNEXION AVEC LE CAPTEUR JSY-MK-194
  }
  else
  {
    Serial2.begin(4800, SERIAL_8N1, RXD2, TXD2); // PORT DE CONNEXION AVEC LE CAPTEUR JSY-MK-194
  }
}
void LectureUxIx2(int8_t source)
{ // Ecriture et Lecture port série du JSY-MK-194  .

  int i, j;
  int Sens_1, Sens_2;
  long LesDatas[14];
  byte ByteArray[130];

  if (LaSourceVar[source].RAZ_JSY)
  { // RAZ Message proposé par F6AAM
    byte msg_send[] = {0x01, 0x10, 0x00, 0x0C, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0xF3, 0xFA};
    // Envoi commande raz sur le JSY-MK_194T
    for (i = 0; i < 13; i++)
    {
      if (LaSource[source].Port == 1)
      {
        Serial1.write(msg_send[i]);
      }
      else
      {
        Serial2.write(msg_send[i]);
      }
    }
    LaSourceVar[source].RAZ_JSY = false;
    delay(100);
  }

  byte msg_send[] = {0x01, 0x03, 0x00, 0x48, 0x00, 0x0E, 0x44, 0x18};
  // Demande Info sur le Serial port 2 (Modbus RTU)
  for (i = 0; i < 8; i++)
  {
    if (LaSource[source].Port == 1)
    {
      Serial1.write(msg_send[i]);
    }
    else
    {
      Serial2.write(msg_send[i]);
    }
  }

  // Réponse en général à l'appel précédent (seulement 4800bauds)
  int a = 0;
  if (LaSource[source].Port == 1)
  {
    while (Serial1.available())
    {
      ByteArray[a] = Serial1.read();
      a++;
    }
  }
  else
  {
    while (Serial2.available())
    {
      ByteArray[a] = Serial2.read();
      a++;
    }
  }

  if (a == 61)
  { // Message complet reçu
    
    j = 3;
    for (i = 0; i < 14; i++)
    { // conversion séries de 4 octets en long
      LesDatas[i] = 0;
      LesDatas[i] += ByteArray[j] << 24;
      j += 1;
      LesDatas[i] += ByteArray[j] << 16;
      j += 1;
      LesDatas[i] += ByteArray[j] << 8;
      j += 1;
      LesDatas[i] += ByteArray[j];
      j += 1;
    }
    Sens_1 = ByteArray[27]; // Sens 1
    Sens_2 = ByteArray[28];
    int8_t CM=source * 3;  //Canal sonde mobile maison a priori
    int8_t CT=CM+1; // Canal sonde fixe Triac
    // Données du Triac
    
    Tension[CT] = LesDatas[0] * .0001;
    Intensite[CT] = LesDatas[1] * .0001;
    float Puiss_1 = PfloatMax(LesDatas[2] * .0001);
    Energie_Soutiree[CT] = int(LesDatas[3] * .1);
    PowerFactor[CT] = LesDatas[4] * .001;
    Energie_Injectee[CT] = int(LesDatas[5] * .1);
    Frequence_[CT] = LesDatas[7] * .01;
    float PVA1 = 0;
    if (PowerFactor[CT] > 0)
    {
      PVA1 = Puiss_1 / PowerFactor[CT];
    }
    String Brute="<h4>Sonde Fixe</h4>Tension : " + String(Tension[CT]) + "<br>Intensite : " + String(Intensite[CT]) + "<br>Frequence : " + String(Frequence_[CT]) + "<br>Puissance : " + String(Puiss_1);
    Brute +="<br>Facteur de puissance : " + String(PowerFactor[CT])+ "<br>Energie soutirée : " + String(Energie_Soutiree[CT]) + "<br>Energie injectée : " + String(Energie_Injectee[CT]);
    if (Sens_1 > 0)
    { // Injection sur TRiac. Ne devrait pas arriver
      PuissanceI_inst[CT] = Puiss_1;
      PuissanceS_inst[CT] = 0;
      PVAI_inst[CT] = PVA1;
      PVAS_inst[CT] = 0;
    }

    else
    {
      PuissanceI_inst[CT] = 0;
      PuissanceS_inst[CT] = Puiss_1;
      PVAI_inst[CT] = 0;
      PVAS_inst[CT] = PVA1;
    }
    
    
    // Données générale de la Maison
    Tension[CM] = LesDatas[8] * .0001;
    Intensite[CM] = LesDatas[9] * .0001;
    float Puiss_2 = PfloatMax(LesDatas[10] * .0001);
    Energie_Soutiree[CM] = int(LesDatas[11] * .1);
    PowerFactor[CM] = LesDatas[12] * .001;
    Energie_Injectee[CM] = int(LesDatas[13] * .1);
    float PVA2 = 0;
    if (PowerFactor[CM] > 0)
    {
      PVA2 = Puiss_2 / PowerFactor[CM];
    }
    Brute +="<h4>Sonde Mobile</h4>Tension : " + String(Tension[CM]) + "<br>Intensite : " + String(Intensite[CM]) + "<br>Frequence : " + String(Frequence_[CM]) + "<br>Puissance : " + String(Puiss_2);
    Brute +="<br>Facteur de puissance : " + String(PowerFactor[CM])+ "<br>Energie soutirée : " + String(Energie_Soutiree[CM]) + "<br>Energie injectée : " + String(Energie_Injectee[CM]);
    if (Sens_2 > 0)
    { // Injection en entrée de Maison
      PuissanceI_inst[CM] = Puiss_2;
      PuissanceS_inst[CM] = 0;
      PVAI_inst[CM] = PVA2;
      PVAS_inst[CM] = 0;
    }
    else
    {
      PuissanceI_inst[CM] = 0;
      PuissanceS_inst[CM] = Puiss_2;
      PVAI_inst[CM] = 0;
      PVAS_inst[CM] = PVA2;
    }
    Frequence_[CM] =Frequence_[CT];
    LaSourceVar[source].DataBrute = Brute;
    Filtre_Puissance(true,CM);
    Filtre_Puissance(true,CT);
    AffectationPuissanceAuxActions(CM);
    AffectationPuissanceAuxActions(CT);
    Puissance_Max(CM);
    Puissance_Max(CT);
    CalculEnergieJour(CM, true);
    CalculEnergieJour(CT, true);
    PVA_Valide[CM] = true;
    PVA_Valide[CT] = true;
    
  }
}