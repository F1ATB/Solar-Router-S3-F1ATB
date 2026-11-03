// ********************
// Gestion des Actions
// ********************
#include "BlocActions.h"
#include "Communications/comSortie.h"
#include "Ecrans/Ecran.h"
#include "Heure.h"
#include "RMS_Externes.h"
#include "Temperatures/Temperature.h"
#include "config.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>

Bloc_Action BlocsActions[BLOCS_ACTION_MAX];    // Liste des Blocs des actions
Bloc_Capteur BlocsCapteurs[BLOCS_CAPTEUR_MAX]; // Liste des Blocs capteurs
Bloc_Filtre BlocsFiltres[BLOCS_FILTRE_MAX];    // Liste des Blocs des filtres
Bloc_Logique BlocsLogiques[BLOCS_LOGIQUE_MAX]; // Liste des Blocs logiques
Bloc_AddSub BlocsAddSub[BLOCS_ADD_SUB_MAX];    // Liste des Blocs des additionneurs de puissance

bool SortieBin[BLOCS_TOTAUX_MAX];   // Etat des sorties si binaire
float SortieNum0[BLOCS_TOTAUX_MAX]; // Etat des sorties si analogique
float SortieNum1[BLOCS_TOTAUX_MAX];
int NbBlocs = 0; // Nombre de Blocs dans la configuration

void Init_Blocs() {
  for (int i = 0; i < BLOCS_ACTION_MAX; i++) {
    BlocsActions[i] = Bloc_Action(); // Creation objets
  }
  for (int i = 0; i < BLOCS_CAPTEUR_MAX; i++) {
    BlocsCapteurs[i] = Bloc_Capteur(); // Creation objets
  }
  for (int i = 0; i < BLOCS_FILTRE_MAX; i++) {
    BlocsFiltres[i] = Bloc_Filtre(); // Creation objets
  }
  for (int i = 0; i < BLOCS_LOGIQUE_MAX; i++) {
    BlocsLogiques[i] = Bloc_Logique(); // Creation objets
  }
  for (int i = 0; i < BLOCS_TOTAUX_MAX; i++) {
    SortieBin[i] = false; // Etat des sorties si binaire
    SortieNum0[i] = 0;    // Etat des sorties si analogique
    SortieNum1[i] = 0;    // Etat des sorties si analogique (2 sorties max par bloc)
  }
  for (int i = 0; i < 48; i++) {
    Wm2[i] = 0; // Tableau des ensoleillements
  }
  for (int i = 0; i < BLOCS_ADD_SUB_MAX; i++) {
    BlocsAddSub[i] = Bloc_AddSub(); // Creation objets
  }
}
//=== Class Bloc_Action ===

Bloc_Action::Bloc_Action() {
  value = "";
  NumBloc = 255;
  Gpio = -1; // si le n° de pin n'est pas valid, on ne fait rien
  Regulation = RegulationType::INACTIF;
  Port = 80;
  Repetition = 0;
  Temporisation = 0;
  T_LastAction = int(millis() / 1000);
  Nom = "";
  Host = "";
  OrdreOn = "";
  OrdreOff = "";
  tOnOff = 0;
  OutOn = 1;
  OutOff = 0;
  PID_On = false;
  P = 0;
  I = 0;
  D = 0;
  GetPost = false; // 0=GET,1=POST
  SeuilPW_On = 0;
  SeuilPW_Off = 0;
  VoltON = true;    // 0=0V, 1=3.3V
  OuvreForce = 100; // Pourcent ouverture forcée
  OuvreMax = 100;   // Pourcent ouverture max
  On = false;
  Hequiv = 0;
  for (int i = 0; i < 4; i++) {
    EntreeCable[i] = false; // 0=Pas de câble, 1= Câble présent
    EntreeNum[i] = false;   // 1 =Entree numérique pour les capteurs et non binaire
    BlocSenEntree[i] = 255; // Bloc de sortie connecté en entrée
    PinSenEntree[i] = 255;  // Bloc de sortie connecté en entrée
  }
}
void Bloc_Action::InitGpio(int FreqPWM) { // Initialise les sorties GPIO pour des relais
  EcranPrintln("Initialisation GPIO pour le bloc " + String(NumBloc));

  if (Gpio > 0) {
    EcranPrintln("Configuration GPIO: " + String(Gpio));
    if (VoltON) {
      OutOn = 1;
      OutOff = 0;
    } else {
      OutOn = 0;
      OutOff = 1;
    }
    T_LastAction = int(millis() / 1000);
    if (Gpio < 99) // Si GPIO interne
    {
      if (Regulation == RegulationType::PWM) { // PWM
        ledcAttach(Gpio, FreqPWM, 8);          //  Frequence, 8 bits de résolution
      } else {
        EcranPrintln("Configuration GPIO en mode digital");
        pinMode(Gpio, OUTPUT);
        digitalWrite(Gpio, OutOff); // Off
      }
    }
  }
}

void Bloc_Action::SortieOnOff(bool onoff) {

  if (Gpio > 0) {
    if (Regulation != RegulationType::ONOFF)
      Temporisation = 0;

    unsigned int Tseconde = int(millis() / 1000);
    if ((Tseconde - T_LastAction) >= Temporisation) {
      if (Gpio == 99) // GPIO externe
      {
        if (On != onoff || ((Tseconde - T_LastAction) > Repetition && Repetition != 0)) {
          CallExterne(Host, onoff ? OrdreOn : OrdreOff, Port, GetPost);
          T_LastAction = Tseconde;
          On = onoff;
        }
      } else { // GPIO interne

        T_LastAction = Tseconde;
        On = onoff;
        if (Regulation == RegulationType::PWM) {
          ledcWrite(Gpio, onoff ? OutOn * 255 : OutOff * 255); // On for PWM
        } else {
          digitalWrite(Gpio, onoff ? OutOn : OutOff); // On for digital
        }
      }
    }
  }
}

void Bloc_Action::CallExterne(String host, String url, int port, bool getPost) {
  HTTPClient http;

  // Construction de l'adresse complète
  String adresse = "http://" + host;

  // Ajout du port uniquement s'il n'est pas le port HTTP standard
  if (port != 80) {
    adresse += ":" + String(port);
  }

  adresse += url;

  Serial.println("Appel HTTP : " + adresse);

  // Connexion au serveur
  http.begin(adresse);

  int httpCode;

  if (getPost == false) {
    // Requête GET
    httpCode = http.GET();
  } else {
    // Requête POST
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // POST vide
    httpCode = http.POST("");
  }

  // Vérification du résultat
  if (httpCode > 0) {
    Serial.printf("Code HTTP : %d\n", httpCode);

    String payload = http.getString();

    Serial.println("Réponse serveur :");
    Serial.println(payload);
  } else {
    Serial.printf("Erreur HTTP : %s\n", http.errorToString(httpCode).c_str());
  }

  // Libération de la connexion
  http.end();
}
//=== Class Bloc_Capteur ===
Bloc_Capteur::Bloc_Capteur() {
  type = "";
  value = "";
  NumBloc = 255;
  Commande = "";
  Idx_RMS = 0; // Index du RMS action externe
  Idx_Act = 0;
  SortieNum = false; // Sortie numérique pour les capteurs
}
//=== Class Bloc_Filtre ===
Bloc_Filtre::Bloc_Filtre() {
  value = "";
  NumBloc = 255;
  SeuilOuSeconde = 0.0;
  Memoire = 0.0;
  EntreeCable = false; // 0=Pas de câble, 1= Câble présent
  EntreeNum = false;   // Entree numérique pour les capteurs et non binaire
  BlocSenEntree = 255; // Bloc de sortie connecté en entrée
  PinSenEntree = 255;
  SortieNum = false;
  OldEntree = false; // Ancienne valeur d'entrée pour le filtre timer
}
//=== Class Bloc_Logique ===
Bloc_Logique::Bloc_Logique() {
  value = "";
  NumBloc = 255;
  for (int i = 0; i < 10; i++) {
    EntreeCable[i] = false; // 0=Pas de câble, 1= Câble présent
    EntreeNum[i] = false;   // 1 =Entree numérique pour les capteurs et non binaire
    BlocSenEntree[i] = 255; // Bloc de sortie connecté en entrée
    PinSenEntree[i] = 255;  // Bloc de sortie connecté en entrée
  }
  for (int i = 0; i < 3; i++) {
    OldEntree[i] = false; // Anciennes valeur d'entrée pour la bascule
  }
}
//=== Class Bloc_AddSub , Additionneurs de puissance===
Bloc_AddSub::Bloc_AddSub() {
  value = "";
  NumBloc = 255;
  for (int i = 0; i < 2; i++) {
    EntreeCable[i] = false; // 0=Pas de câble, 1= Câble présent. Entrées numérique
    BlocSenEntree[i] = 255; // Bloc de sortie connecté en entrée
    PinSenEntree[i] = 255;  // Bloc de sortie connecté en entrée
  }
}
void forceAction(int NumAction, int Force) {
  if (Force != 0 && NumAction < NbActions) {
    if (Force > 0) {
      if (BlocsActions[NumAction].tOnOff < 0) {
        BlocsActions[NumAction].tOnOff = 0;
      } else {
        BlocsActions[NumAction].tOnOff += 30;
      }
    } else {
      if (BlocsActions[NumAction].tOnOff > 0) {
        BlocsActions[NumAction].tOnOff = 0;
      } else {
        BlocsActions[NumAction].tOnOff -= 30;
      }
    }
  }
}

void GestionForcageActions() {
  for (int i = 0; i < NbActions; i++) {
    if (BlocsActions[i].tOnOff > 0)
      BlocsActions[i].tOnOff -= 1;
    if (BlocsActions[i].tOnOff < 0)
      BlocsActions[i].tOnOff += 1;
  }
}

void DecodageConfigAction() {
  byte typeDeBloc[BLOCS_TOTAUX_MAX]; // 0=Pas de bloc,1=Action,2=Capteur,3=Filtre,4=Logique
  int refBloc[BLOCS_TOTAUX_MAX];     // Référence du bloc dans le tableau correspondant (BlocsActions, BlocsCapteurs, BlocsFiltres, BlocsLogiques)
  for (int i = 0; i < BLOCS_TOTAUX_MAX; i++) {
    typeDeBloc[i] = 0;
  }
  if (LesActions.isNull())
    return; // Si LesActions est vide, on quitte la fonction

  int iBloc = 0;
  int iAction = 0;
  int iCapteur = 0;
  int iFiltre = 0;
  int iLogique = 0;
  int iAddSub = 0;

  ReacCACSI = LesActions["ReacCACSI"].as<byte>();
  FreqPWM = LesActions["FreqPWM"].as<unsigned int>();
  NumBlocTriac = -1;

  //==== Affectation des Blocs ====

  JsonArray arr = LesActions["blocs"].as<JsonArray>();
  for (JsonObject obj : arr) {
    if (iBloc >= BLOCS_TOTAUX_MAX || iAction >= BLOCS_ACTION_MAX || iCapteur >= BLOCS_CAPTEUR_MAX || iFiltre >= BLOCS_FILTRE_MAX ||
        iLogique >= BLOCS_LOGIQUE_MAX || iAddSub >= BLOCS_ADD_SUB_MAX)
      break; // éviter de dépasser les tableaux

    String type = obj["type"].as<String>();
    String value = obj["value"].as<String>();
    JsonArray arrEntree = obj["LesEntrees"].as<JsonArray>();
    int iEntree = 0;
    JsonArray arrSortie = obj["LesSorties"].as<JsonArray>();
    int iSortie = 0;
    if (type == "action") // Les 2 sorties sont numériques pour les blocs actions
    {
      BlocsActions[iAction] = Bloc_Action();
      BlocsActions[iAction].value = value;
      BlocsActions[iAction].NumBloc = iBloc;
      BlocsActions[iAction].Nom = obj["Para"]["Nom"].as<String>();
      int RegulationInt = obj["Para"]["Regulation"].as<int>() | 0; // Valeur par défaut si la clé "Regulation" n'existe pas
      BlocsActions[iAction].Regulation = (Bloc_Action::RegulationType)RegulationInt;
      BlocsActions[iAction].Gpio = obj["Para"]["GPIO"].as<String>().toInt(); // Il faut passer par les Strings . Etrange
      if (pTriac > 0)
        BlocsActions[iAction].Gpio = PulseT[pTriac];
      BlocsActions[iAction].Port = obj["Para"]["Port"].as<String>().toInt();
      BlocsActions[iAction].Temporisation = obj["Para"]["Temporisation"].as<String>().toInt();
      BlocsActions[iAction].Repetition = obj["Para"]["Repetition"].as<String>().toInt();
      BlocsActions[iAction].Host = obj["Para"]["Host"].as<String>();
      BlocsActions[iAction].OrdreOn = obj["Para"]["OrdreOn"].as<String>();
      BlocsActions[iAction].OrdreOff = obj["Para"]["OrdreOff"].as<String>();
      BlocsActions[iAction].PID_On = obj["Para"]["PID_On"].as<bool>();
      BlocsActions[iAction].P = obj["Para"]["P"].as<String>().toInt();
      BlocsActions[iAction].I = obj["Para"]["I"].as<String>().toInt();
      BlocsActions[iAction].D = obj["Para"]["D"].as<String>().toInt();
      BlocsActions[iAction].GetPost = obj["Para"]["GetPost"].as<bool>(); // 0=GET,1=POST
      BlocsActions[iAction].SeuilPW_On = obj["Para"]["SeuilPW_On"].as<String>().toInt();
      BlocsActions[iAction].SeuilPW_Off = obj["Para"]["SeuilPW_Off"].as<String>().toInt();
      BlocsActions[iAction].VoltON = obj["Para"]["VoltON"].as<bool>();                   // 0=0V, 1=3.3V
      BlocsActions[iAction].OuvreForce = obj["Para"]["OuvreForce"].as<String>().toInt(); // Pourcent ouverture forcée
      BlocsActions[iAction].OuvreMax = obj["Para"]["OuvreMax"].as<String>().toInt();     // Pourcent ouverture max
      if (value == "triac")
        NumBlocTriac = iAction;              // Un seul bloc triac est attendu
      for (JsonObject objEntree : arrEntree) // 4 entrée par action
      {
        BlocsActions[iAction].EntreeCable[iEntree] = objEntree["cable"].as<bool>(); // 0=Pas de câble, 1= Câble présent
        BlocsActions[iAction].EntreeNum[iEntree] = objEntree["EntreeNum"].as<bool>();
        iEntree++;
      }

      refBloc[iBloc] = iAction;
      iAction++;
      typeDeBloc[iBloc] = 1; // Action
    } else if (type == "puissance" || type == "heure" || type == "soleil" || type == "temperature" || type == "tempo" || type == "hphc" ||
               type == "commande" || type == "jour" || type == "ActExt_Ouvre" || type == "ActExt_Hequiv") {
      BlocsCapteurs[iCapteur] = Bloc_Capteur();
      BlocsCapteurs[iCapteur].value = value;
      BlocsCapteurs[iCapteur].NumBloc = iBloc;
      BlocsCapteurs[iCapteur].type = type;
      BlocsCapteurs[iCapteur].Commande = obj["Para"]["Commande"].as<String>();
      BlocsCapteurs[iCapteur].Idx_RMS = obj["Para"]["Idx_RMS"].as<String>().toInt();
      BlocsCapteurs[iCapteur].Idx_Act = obj["Para"]["Idx_Act"].as<String>().toInt();
      for (JsonObject objSortie : arrSortie) // 1 sortie par capteur
      {
        BlocsCapteurs[iCapteur].SortieNum = objSortie["SortieNum"].as<bool>();
      }
      refBloc[iBloc] = iCapteur;
      iCapteur++;
      typeDeBloc[iBloc] = 2; // Capteur
    } else if (type == "filtre") {
      BlocsFiltres[iFiltre] = Bloc_Filtre();
      BlocsFiltres[iFiltre].value = value;
      BlocsFiltres[iFiltre].NumBloc = iBloc;
      BlocsFiltres[iFiltre].SeuilOuSeconde = obj["Para"]["Seuil"].as<float>();
      for (JsonObject objEntree : arrEntree) // 1 entrée par filtre
      {
        BlocsFiltres[iFiltre].EntreeCable = objEntree["cable"].as<bool>(); // 0=Pas de câble, 1= Câble présent
        BlocsFiltres[iFiltre].EntreeNum = objEntree["EntreeNum"].as<bool>();
      }
      for (JsonObject objSortie : arrSortie) // 1 sortie par filtre
      {
        BlocsFiltres[iFiltre].SortieNum = objSortie["SortieNum"].as<bool>();
      }
      BlocsFiltres[iFiltre].SeuilOuSeconde = obj["Para"]["Seuil"].as<float>();

      refBloc[iBloc] = iFiltre;
      iFiltre++;
      typeDeBloc[iBloc] = 3;  // Filtre
    } else if (type == "bin") // Les sorties sont binaires pour les blocs logiques
    {
      BlocsLogiques[iLogique] = Bloc_Logique();
      BlocsLogiques[iLogique].value = value;
      BlocsLogiques[iLogique].NumBloc = iBloc;
      for (JsonObject objEntree : arrEntree) {
        BlocsLogiques[iLogique].EntreeCable[iEntree] = objEntree["cable"].as<bool>(); // 0=Pas de câble, 1= Câble présent
        BlocsLogiques[iLogique].EntreeNum[iEntree] = objEntree["EntreeNum"].as<bool>();
        iEntree++;
      }
      BlocsLogiques[iLogique].NbEntree = iEntree; // Nombre d'entrées câblées ou non
      for (int i = 0; i < 3; i++) {
        BlocsLogiques[iLogique].OldEntree[i] = false; // Anciennes valeur d'entrée pour la bascule
      }
      refBloc[iBloc] = iLogique;
      iLogique++;
      typeDeBloc[iBloc] = 4;     // Logique
    } else if (type == "addsub") // Les sorties sont analogiques
    {
      BlocsAddSub[iAddSub] = Bloc_AddSub();
      BlocsAddSub[iAddSub].value = value;
      BlocsAddSub[iAddSub].NumBloc = iBloc;
      for (JsonObject objEntree : arrEntree) {
        BlocsAddSub[iAddSub].EntreeCable[iEntree] = objEntree["cable"].as<bool>(); // 0=Pas de câble, 1= Câble présent
        iEntree++;
      }
      refBloc[iBloc] = iAddSub;
      iAddSub++;
      typeDeBloc[iBloc] = 5; // AddSub
    }
    SortieBin[iBloc] = false; // On stop les sorties précédentes
    SortieNum0[iBloc] = 0;
    SortieNum1[iBloc] = 0;
    iBloc++;
  }
  NbBlocs = iBloc;     // Nombre totaux de blocs
  NbActions = iAction; // Nombre total de blocs d'actions Triac + SSR
  for (int j = iAction; j < BLOCS_ACTION_MAX; j++) {
    BlocsActions[j].NumBloc = 255; // Marque les blocs non utilisés
  }
  for (int j = iCapteur; j < BLOCS_CAPTEUR_MAX; j++) {
    BlocsCapteurs[j].NumBloc = 255; // Marque les blocs non utilisés
  }
  for (int j = iFiltre; j < BLOCS_FILTRE_MAX; j++) {
    BlocsFiltres[j].NumBloc = 255; // Marque les blocs non utilisés
  }
  for (int j = iLogique; j < BLOCS_LOGIQUE_MAX; j++) {
    BlocsLogiques[j].NumBloc = 255; // Marque les blocs non utilisés
  }
  for (int j = iAddSub; j < BLOCS_ADD_SUB_MAX; j++) {
    BlocsAddSub[j].NumBloc = 255; // Marque les blocs non utilisés
  }
  //==== Affectation des lignes de connexion ====
  // On a la reference dans les tables de sortie (SortieBin,SortieNum0 et SortieNum1) des valeurs à prendre en entrée suite à connexion entre les
  // blocs.
  JsonArray arrL = LesActions["lignes"].as<JsonArray>();
  for (JsonObject obj : arrL) {
    byte blocEntree = obj["blocEntree"].as<byte>();
    byte pinEntree = obj["pinEntree"].as<byte>();
    byte blocSortie = obj["blocSortie"].as<byte>();
    byte pinSortie = obj["pinSortie"].as<byte>();
    switch (typeDeBloc[blocEntree]) { // Les blocs capteurs n'ont pas de connexion en entrée, donc pas de case 2
    case 1:                           // Action
      BlocsActions[refBloc[blocEntree]].BlocSenEntree[pinEntree] = blocSortie;
      BlocsActions[refBloc[blocEntree]].PinSenEntree[pinEntree] = pinSortie;
      break;
    case 3: // Filtre
      BlocsFiltres[refBloc[blocEntree]].BlocSenEntree = blocSortie;
      BlocsFiltres[refBloc[blocEntree]].PinSenEntree = pinSortie;
      break;
    case 4: // Logique
      BlocsLogiques[refBloc[blocEntree]].BlocSenEntree[pinEntree] = blocSortie;
      BlocsLogiques[refBloc[blocEntree]].PinSenEntree[pinEntree] = pinSortie;
      break;
    case 5: // AddSub
      BlocsAddSub[refBloc[blocEntree]].BlocSenEntree[pinEntree] = blocSortie;
      BlocsAddSub[refBloc[blocEntree]].PinSenEntree[pinEntree] = pinSortie;
      break;
    }
  }
}
void RevueBlocsActions() {
  //==== Implémentation de la revue des Filtres ====
  float valeurEntree = 0;
  bool valeurEntreeB = false;
  String valeurStr;
  for (int i = 0; i < BLOCS_FILTRE_MAX; i++) {
    if (BlocsFiltres[i].NumBloc != 255) {
      if (BlocsFiltres[i].EntreeCable) // L'entree est cablée
      {
        if (BlocsFiltres[i].EntreeNum) {
          valeurEntree = SortieNum0[BlocsFiltres[i].BlocSenEntree];
          if (BlocsFiltres[i].PinSenEntree == 1) { // Cas des Hequ des SSR ou Triac
            valeurEntree = SortieNum1[BlocsFiltres[i].BlocSenEntree];
          }
        } else {
          valeurEntreeB = SortieBin[BlocsFiltres[i].BlocSenEntree];
        }
        float SeuilOuSeconde = BlocsFiltres[i].SeuilOuSeconde;
        valeurStr = BlocsFiltres[i].value;
        bool OutBin = false;
        float Memoire = BlocsFiltres[i].Memoire;

        if (valeurStr == "sup") {
          if (valeurEntree > SeuilOuSeconde) {
            OutBin = true;
          }
        } else if (valeurStr == "inf") {
          if (valeurEntree < SeuilOuSeconde) {
            OutBin = true;
          }
        } else if (valeurStr == "sup_egal") {
          if (valeurEntree >= SeuilOuSeconde) {
            OutBin = true;
          }
        } else if (valeurStr == "inf_egal") {
          if (valeurEntree <= SeuilOuSeconde) {
            OutBin = true;
          }
        } else if (valeurStr == "retard") {
          if (SeuilOuSeconde != 0) {
            if (valeurEntreeB) {                      // Entree binaire
              Memoire = Memoire + 1 / SeuilOuSeconde; // Incrémenter la mémoire de 1 unité (ajuster selon le besoin)
            } else {
              Memoire = Memoire - 1 / SeuilOuSeconde;
            }
            OutBin = SortieBin[BlocsFiltres[i].NumBloc]; // Ancien état du filtre
            if (Memoire >= 1) {
              OutBin = true;
              Memoire = 1; // Limiter la mémoire à 1
            } else if (Memoire <= 0) {
              OutBin = false;
              Memoire = 0; // Limiter la mémoire à 0
            }
          } else {
            OutBin = valeurEntreeB;
          }
        } else if (valeurStr == "moyenne") {
          if (SeuilOuSeconde != 0) {
            float alpha = 1 / SeuilOuSeconde;
            Memoire = alpha * valeurEntree + (1 - alpha) * Memoire;
          } else {
            Memoire = valeurEntree;
          }
          SortieNum0[BlocsFiltres[i].NumBloc] = Memoire;
        } else if (valeurStr == "timer") { // Declenche sur front montant
          Memoire = Memoire + 1;
          OutBin = SortieBin[BlocsFiltres[i].NumBloc]; // Ancien état du filtre
          if (valeurEntreeB && !BlocsFiltres[i].OldEntree) {
            OutBin = true;
            Memoire = 0;
          }
          if (Memoire >= SeuilOuSeconde) {
            OutBin = false;
            Memoire = 0;
          }
          BlocsFiltres[i].OldEntree = valeurEntreeB;
        }
        SortieBin[BlocsFiltres[i].NumBloc] = OutBin;
        BlocsFiltres[i].Memoire = Memoire;
      }
    } else {
      break; // On sort de la boucle si on rencontre un bloc non utilisé
    }
  }

  //==== Implémentation de la revue des Operateurs Logiques ====
  for (int i = 0; i < BLOCS_LOGIQUE_MAX; i++) {
    if (BlocsLogiques[i].NumBloc != 255) {
      bool SiET = true;
      bool SiOU = false;
      bool SiOUex = false;
      bool SiNON = false;
      bool SortieLogique = SortieBin[BlocsLogiques[i].NumBloc]; // Ancienne valeur de sortie du bloc logique
      String valeurStr = BlocsLogiques[i].value;

      for (int j = 0; j < BlocsLogiques[i].NbEntree; j++) {
        if (BlocsLogiques[i].EntreeCable[j]) // L'entree est cablée
        {

          valeurEntreeB = SortieBin[BlocsLogiques[i].BlocSenEntree[j]];
          SiNON = !valeurEntreeB; // Inversion de l'entrée si nécessaire
          if (valeurEntreeB) {
            SiOU = true;
            SiOUex = !SiOUex; // Ou Exclusif
          } else {
            SiET = false;
          }

          if (valeurStr == "bascule") {

            switch (j) {
            case 0:
              if (valeurEntreeB && !BlocsLogiques[i].OldEntree[0]) {
                SortieLogique = false; // Pulse  sur le front montant en entrée Off
              }
              BlocsLogiques[i].OldEntree[0] = valeurEntreeB;
              break;
            case 1:
              if (valeurEntreeB && !BlocsLogiques[i].OldEntree[1]) {
                SortieLogique = true; // Pulse  sur front montant en entrée On
              }
              BlocsLogiques[i].OldEntree[1] = valeurEntreeB;
              break;
            case 2:
              if (valeurEntreeB && !BlocsLogiques[i].OldEntree[2]) {
                SortieLogique = !SortieLogique; // Inverser la sortie sur front montant
              }
              BlocsLogiques[i].OldEntree[2] = valeurEntreeB;
              break;
            }
          }
        }
      }
      if (valeurStr == "et") {
        SortieLogique = SiET; // Pour ET, la sortie est vraie si toutes les entrées sont vraies
      } else if (valeurStr == "ou") {
        SortieLogique = SiOU; // Pour OU, la sortie est vraie si au moins une entrée est vraie
      } else if (valeurStr == "ou_ex") {
        SortieLogique = SiOUex; // Pour OU exclusif, la sortie est vraie si un nombre impair d'entrées est vrai
      } else if (valeurStr == "non") {
        SortieLogique = SiNON; // Pour NON, la sortie est l'inverse de l'entrée
      }
      SortieBin[BlocsLogiques[i].NumBloc] = SortieLogique; // Sortie du bloc logique
    } else {
      break; // On sort de la boucle si on rencontre un bloc non utilisé
    }
  }
  //==== Implémentation de la revue des Additionneurs AddSub ====
  for (int i = 0; i < BLOCS_ADD_SUB_MAX; i++) {
    if (BlocsAddSub[i].NumBloc != 255) {
      // Implémentation spécifique pour les additionneurs AddSub
      // Par exemple, on peut calculer la somme ou la différence des entrées câblées
      float somme = 0;
      for (int j = 0; j < 2; j++) {
        if (BlocsAddSub[i].BlocSenEntree[j] != 255) {
          if (j == 1 && BlocsAddSub[i].value == "soustraction")
            somme -= SortieNum0[BlocsAddSub[i].BlocSenEntree[j]];
          else
            somme += SortieNum0[BlocsAddSub[i].BlocSenEntree[j]];
        }
      }
      SortieNum0[BlocsAddSub[i].NumBloc] = somme;
    } else {
      break; // On sort de la boucle si on rencontre un bloc non utilisé
    }
  }
}
void RafraichissementsBlocsCapteurs() {
  float sortie = 0;
  bool sortieBin = false;
  String valueStr;
  String typeStr;
  for (int i = 0; i < BLOCS_CAPTEUR_MAX; i++) {
    if (BlocsCapteurs[i].NumBloc != 255) {
      typeStr = BlocsCapteurs[i].type;
      valueStr = BlocsCapteurs[i].value;
      if (typeStr == "soleil") {

        if (valueStr == "actuel") {
          sortie = float(SoleilActuel);
        } else if (valueStr == "jourJ") {
          sortie = float(SoleilJour);
        } else if (valueStr == "demain") {
          sortie = SoleilDemain;
        }

        SortieNum0[BlocsCapteurs[i].NumBloc] = sortie;
      } else if (typeStr == "temperature") {
        for (int canal = 0; canal < 4; canal++) {
          String CanalTempStr = "T" + String(canal);
          if (valueStr == CanalTempStr) {
            if (Source_Temp[canal] != "tempNo") {
              sortie = float(temperature[canal]);
            } else {
              sortie = -100.0; // Valeur par défaut si le canal n'est pas trouvé
            }
            SortieNum0[BlocsCapteurs[i].NumBloc] = sortie;
          }
        }
      } else if (typeStr == "tempo") {
        if (valueStr == "tempo_bleu" && LTARF.indexOf("BLEU") >= 0)
          sortieBin = true;
        else if (valueStr == "tempo_blanc" && LTARF.indexOf("BLANC") >= 0)
          sortieBin = true;
        else if (valueStr == "tempo_rouge" && LTARF.indexOf("ROUGE") >= 0)
          sortieBin = true;
        else
          sortieBin = false;
        SortieBin[BlocsCapteurs[i].NumBloc] = sortieBin;
      } else if (typeStr == "hphc") {
        if (valueStr == "HP" && LTARF.indexOf("PLEINE") >= 0)
          sortieBin = true;
        else if (valueStr == "HC" && LTARF.indexOf("CREUSE") >= 0)
          sortieBin = true;
        else
          sortieBin = false;
        SortieBin[BlocsCapteurs[i].NumBloc] = sortieBin;
      } else if (typeStr == "ActExt_Ouvre") {
        if (RouteurExt[BlocsCapteurs[i].Idx_RMS].ValideActionExterne[BlocsCapteurs[i].Idx_Act] > 0) {
          SortieNum0[BlocsCapteurs[i].NumBloc] = RouteurExt[BlocsCapteurs[i].Idx_RMS].OuvertureActionExterne[BlocsCapteurs[i].Idx_Act];
        } else {
          SortieNum0[BlocsCapteurs[i].NumBloc] = 0;
        }
      } else if (typeStr == "ActExt_Hequiv") {
        if (RouteurExt[BlocsCapteurs[i].Idx_RMS].ValideActionExterne[BlocsCapteurs[i].Idx_Act] > 0) {
          SortieNum0[BlocsCapteurs[i].NumBloc] = RouteurExt[BlocsCapteurs[i].Idx_RMS].HequivActionExterne[BlocsCapteurs[i].Idx_Act];
        } else {
          SortieNum0[BlocsCapteurs[i].NumBloc] = 0;
        }
      } else if (BlocsCapteurs[i].type == "commande") {
        // A FAIRE ?????????????????????????
      } else if (BlocsCapteurs[i].type == "jour") {
        if (valueStr == "dimanche" && Jour == 0)
          sortieBin = true;
        else if (valueStr == "lundi" && Jour == 1)
          sortieBin = true;
        else if (valueStr == "mardi" && Jour == 2)
          sortieBin = true;
        else if (valueStr == "mercredi" && Jour == 3)
          sortieBin = true;
        else if (valueStr == "jeudi" && Jour == 4)
          sortieBin = true;
        else if (valueStr == "vendredi" && Jour == 5)
          sortieBin = true;
        else if (valueStr == "samedi" && Jour == 6)
          sortieBin = true;
        else
          sortieBin = false;
        SortieBin[BlocsCapteurs[i].NumBloc] = sortieBin;
      }
    } else {
      break; // On sort de la boucle si on rencontre un bloc non utilisé
    }
  }
}