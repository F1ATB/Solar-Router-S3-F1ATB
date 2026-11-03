// ***************************************************************
// * Client d'un autre ESP32 en charge de mesurer les puissances *
// ***************************************************************
#include <Arduino.h>
#include "Sources/Source_Externe.h"
#include "config.h"
#include "RMS_Externes.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "Sources/Source_Linky.h"
#include "commonFX.h"
#include "Communications/TempoRTE.h"
#include "Communications/comSortie.h"
#include <WiFi.h>
#include "Communications/Internet.h"
#include "Ecrans/Ecran.h"



void CallESP32_Externe(int8_t source) {
  String S = "";
  String Gr[4];
  String data_[22];
  int8_t C0=source * NB_CANAUX_PAR_SOURCE; //Canal des données. On en garde que un seul par source, suivant le numero CanalVoie
  int8_t C1=C0+1;
  EnergieActiveValide[C1] = false;
  EnergieActiveValide[C1+1] = false;

  unsigned long IPduRMS=LaSource[source].IP;
  
  //On verifie si toujours dans la liste des routeurs
  bool inList=false;
  bool cestUnS3=false;
  for (int i = 0; i < LES_ROUTEURS_MAX; i++)
  {
    if (RouteurExt[i].IP == IPduRMS) {
      inList = true;
      cestUnS3 = RouteurExt[i].S3;

      break;
    }
  }
  if (!inList) IPduRMS = 0;
  if (IPduRMS == 0) {
    LaSource[source].Type="";
    LaSource[source].IP = 0;
    return;
  }
  if (cestUnS3) return; // Si c'est un S3, on ne continue pas avec ce code. Ce sera un client websocket
  int8_t IdxM=Idx_Machine( IPduRMS);
  String url = "http://" + IP2String(IPduRMS) + "/ajax_data";
  String RMSExtDataB = httpGetPayload(url, IdxM);
  if (RMSExtDataB == "") return;
 
  if (RMSExtDataB.indexOf("Deb") >= 0 && RMSExtDataB.indexOf("Fin") > 0) {  //Trame complète reçue
    RMSExtDataB = RMSExtDataB.substring(RMSExtDataB.indexOf("Deb") + 4);
    RMSExtDataB = RMSExtDataB.substring(0, RMSExtDataB.indexOf("Fin") + 3);
    LaSourceVar[source].DataBrute = RMSExtDataB; //Sauvegarde des données brutes
    String Sval = "";
    int idx = 0;
    while (RMSExtDataB.indexOf(GS) > 0) {
      Sval = RMSExtDataB.substring(0, RMSExtDataB.indexOf(GS));
      RMSExtDataB = RMSExtDataB.substring(RMSExtDataB.indexOf(GS) + 1);
      Gr[idx] = Sval;
      idx++;
    }
    Gr[idx] = RMSExtDataB;
    idx = 0;
    for (int i = 0; i < 3; i++) {
      while (Gr[i].indexOf(RS) >= 0) {
        Sval = Gr[i].substring(0, Gr[i].indexOf(RS));
        Gr[i] = Gr[i].substring(Gr[i].indexOf(RS) + 1);
        data_[idx] = Sval;
        idx++;
      }
      data_[idx] = Gr[i];
      idx++;
    }
    for (int i = 0; i <= idx; i++) {
      switch (i) {

        case 1:
          LaSource[source].Data = data_[i];
          if (source==0) Source_data = data_[i]; //source Reference du routeur
          break;
        case 2:
          if (TempoRTEon == 0) LTARF = data_[i];
          break;
        case 3:
          if (TempoRTEon == 0) STGEt = data_[i];
          break;
        case 4:
          //Temperature non utilisé
          break;
        case 5:
          PVA_Valide[C0] = data_[i].toInt();
          
          break;
        case 6:
          PuissanceS[C0] = PintMax(data_[i].toInt());
          break;
        case 7:
          PuissanceI[C0] = PintMax(data_[i].toInt());
         
          Puissance_Max(C0);

          Filtre_Puissance(false,C0);
          AffectationPuissanceAuxActions(C0);
    
          break;
        case 8:
          PVAS[C0] = PintMax(data_[i].toInt());
          break;
        case 9:
          PVAI[C0] = PintMax(data_[i].toInt());
          break;
        case 10:
          EnergieJour_Soutiree[C0] = data_[i].toInt();
          break;
        case 11:
          EnergieJour_Injectee[C0] = data_[i].toInt();
          break;
        case 12:
          Energie_Soutiree[C0] = data_[i].toInt();
          break;
        case 13:
          Energie_Injectee[C0] = data_[i].toInt();
          CalculEnergieJour(C0, false);
          
          if (LaSource[source].CanalVoie[0]==0) i=idx+1; //On a fini. Sinon c'est le canal suivant qui est choisi dans les cas UxIx2
         
          break;
        case 14:  //CAS UxIx2 avec une deuxieme sonde
          PuissanceS[C0] = data_[i].toInt();
          break;
        case 15:
          PuissanceI[C0] = data_[i].toInt();
          Puissance_Max(C0);
          Filtre_Puissance(false,C0);
          AffectationPuissanceAuxActions(C0);
          break;
        case 16:
          PVAS[C0] = data_[i].toInt();
          break;
        case 17:
          PVAI[C0] = data_[i].toInt();
          break;
        case 18:
          EnergieJour_Soutiree[C0] = data_[i].toInt();
          break;
        case 19:
          EnergieJour_Injectee[C0] = data_[i].toInt();
          break;
        case 20:
          Energie_Soutiree[C0] = data_[i].toInt();
          break;
        case 21:
          Energie_Injectee[C0] = data_[i].toInt();
          CalculEnergieJour(C0, false);
          
          
          break;
      }
    }
    RMSExtDataB = "";
  }
  
}
