#include <Arduino.h>
#include "OverProd.h"
#include "config.h"
#include "BlocActions.h"
#include "Ecrans/Ecran.h"
void Init_OverProd()
{
    for (int i = 0; i < BLOCS_ACTION_MAX; i++)
    {
        PulseOn[i] = 0; // 1/2 sinus
        PulseTotal[i] = 100;
        PulseComptage[i] = 0;
        Retard[i] = 100;
        RetardF[i] = 100.0f;
        OutOn[i] = 1;
        OutOff[i] = 0;
        Gpio[i] = -1;
        LastErrorPw[i] = 0.0f;
        IntegrErrorPw[i] = 100.0f;
        Propor[i] = 0.0f;
        DeriveF[i] = 0.0f;
    }

    // Tableau Longueur Pulse et Longueur Trame pour Multi-Sinus de 0 à 100%
    float erreur;
    float vrai;
    float target;
    for (int I = 0; I < 101; I++)
    {
        tabPulseSinusTotal[I] = -1;
        tabPulseSinusOn[I] = -1;
        target = float(I) / 100.0;
        for (int T = 20; T < 101; T++)
        {
            for (int N = 0; N <= T; N++)
            {
                if (T % 2 == 1 || N % 2 == 0)
                { // Valeurs impaires du total ou pulses pairs pour éviter courant continu
                    vrai = float(N) / float(T);
                    erreur = abs(vrai - target);
                    if (erreur < 0.004)
                    {
                        tabPulseSinusTotal[I] = T;
                        tabPulseSinusOn[I] = N;
                        N = 101;
                        T = 101;
                    }
                }
            }
        }
    }
}

void GestionOverproduction()
{
    bool valeurEntreeB = false;
    float PuissanceEnEntree;
    float GainCACSI = float(ReacCACSI);
    float Kp, Ki, Kd;
    float SeuilPW_On_f, SeuilPW_Off_f, ErrorPw, Derive;
    int pos = 0;
    int Vout = 0;

    for (int i = 0; i < NbActions; i++)
    {
        Bloc_Action::RegulationType RegulationEnCours = BlocsActions[i].Regulation;
        if (RegulationEnCours != Bloc_Action::INACTIF) // Si la régulation n'est pas inactive
        {

            if (BlocsActions[i].EntreeCable[2]) // Entree Regulation cablé
            {
                valeurEntreeB = SortieBin[BlocsActions[i].BlocSenEntree[2]];
                if (valeurEntreeB)
                {                                     // Entree regulation activée
                    RetardF[i] = RetardF[i] + 0.0001; // On ferme très légèrement si pas de message reçu
                    SeuilPW_On_f = float(BlocsActions[i].SeuilPW_On);
                    SeuilPW_Off_f = float(BlocsActions[i].SeuilPW_Off);
                    PuissanceEnEntree = SortieNum0[BlocsActions[i].BlocSenEntree[3]];
                    if (BlocsActions[i].PinSenEntree[3] == 1)
                    {
                        PuissanceEnEntree = SortieNum1[BlocsActions[i].BlocSenEntree[3]];
                    }
                    if (RegulationEnCours == Bloc_Action::ONOFF)
                    {
                        if (PuissanceEnEntree < SeuilPW_On_f)
                            RetardF[i] = 0;
                        if (PuissanceEnEntree > SeuilPW_Off_f)
                            RetardF[i] = 100;
                    }
                    else
                    {                                          // Regul autres que ONOFF
                        Ki = float(BlocsActions[i].I) / 10000.0f; // Reactivité
                        if (PuissanceEnEntree < SeuilPW_On_f && ReacCACSI > 1)
                            Ki = Ki * GainCACSI;
                        ErrorPw = PuissanceEnEntree - SeuilPW_On_f;
                        IntegrErrorPw[i] += 0.0001; // On ferme très légèrement si pas de message reçu. Sécurité
                        IntegrErrorPw[i] += ErrorPw * Ki;
                        IntegrErrorPw[i] = constrain(IntegrErrorPw[i], 0.0, 100.0); // Ne pas accumuler des valeurs enormes
                        if (BlocsActions[i].PID_On && ModePara == 1)
                        {
                            Kp = float(BlocsActions[i].P) / 1000.0f; // Coef proportionnel
                            Kd = float(BlocsActions[i].D) / 1000.0f; // Coef/derivé
                            Propor[i] = Kp * ErrorPw;
                            if (Ki == 0.0f && Kp > 0)
                                IntegrErrorPw[i] = 50.0; // Cas particulier avec régulation uniquement de P
                            Derive = Kd * (ErrorPw - LastErrorPw[i]);
                            DeriveF[i] = 0.2 * Derive + 0.8 * DeriveF[i];           // Filtrage car manque de mesure donne Derive=0
                            RetardF[i] = Propor[i] + IntegrErrorPw[i] + DeriveF[i]; // Mode PID
                        }
                        else
                        {
                            // le Triac ou les relais en sinus
                            RetardF[i] = IntegrErrorPw[i]; // Gain de boucle de l'asservissement en mode Integral only
                            Propor[i] = 0;
                            DeriveF[i] = 0;
                        }
                        LastErrorPw[i] = ErrorPw;
                    }
                }
            }
            // Traitement des pins de forcage
            if (BlocsActions[i].EntreeCable[1]) // Entree Forcé On cablé
            {
                valeurEntreeB = SortieBin[BlocsActions[i].BlocSenEntree[1]];
                if (valeurEntreeB)
                    RetardF[i] = 0.f;
            }

            if (BlocsActions[i].EntreeCable[0]) // Entree Forcé Off cablé
            {
                valeurEntreeB = SortieBin[BlocsActions[i].BlocSenEntree[0]];
                if (valeurEntreeB)
                    RetardF[i] = 100.f;
            }
            // Test ouvre Max
            if (Gpio[i]==99) { //GPIO externe
                BlocsActions[i].OuvreMax=100;
                BlocsActions[i].OuvreForce=100;
            }
            float OuvreMaxf = float(BlocsActions[i].OuvreMax);
            if (100.f - RetardF[i] > OuvreMaxf)
                RetardF[i] = 100.f - OuvreMaxf;
            // Traitement des Forcages par bouton (Le plus prioritaire)
            if (BlocsActions[i].tOnOff > 0) // Demande On
                RetardF[i] = 100.f - float(BlocsActions[i].OuvreForce);
            if (BlocsActions[i].tOnOff < 0) // Demande Off
                RetardF[i] = 100.f;
        }
        else
        {
            RetardF[i] = 100.f; // Si la régulation est inactive, on force le retard à 100%
        }
        // Verif validité du retard
        RetardF[i] = constrain(RetardF[i], 0.0, 100.0);
        if (ITmode < 0 && BlocsActions[i].value == "triac")
            RetardF[i] = 100.f; // Triac pas possible sur synchro interne

        Retard[i] = round(RetardF[i]);
        Retard[i] = constrain(Retard[i], 0, 100);
        SortieNum0[BlocsActions[i].NumBloc] = float(100 - Retard[i]);
        if (Retard[i] == 100)
        { // Force en cas d'arret des IT
            BlocsActions[i].SortieOnOff(false);
            PulseOn[i] = 0; // Stop Triac ou relais
        }
        else
        {
            switch (RegulationEnCours)
            {
            case Bloc_Action::ONOFF:
                BlocsActions[i].SortieOnOff(true);

                break;
            case Bloc_Action::DECOUPE_SINUS:
                // Handle DECOUPE_SINUS regulation ?????????????????????? A FAIRE
                break;
            case Bloc_Action::DEMI_SINUS:
                PulseOn[i] = 100 - Retard[i]; // Avance de phase

                if (PulseTotal[i] > 1)
                    PulseTotal[i] = 0; // 0 ou 1 pour mémoriser phase230V dernier pulse
                break;
            case Bloc_Action::MULTI_SINUS:
                PulseOn[i] = tabPulseSinusOn[100 - Retard[i]];
                PulseTotal[i] = tabPulseSinusTotal[100 - Retard[i]];

                pos = PulseComptage[i];
                if (pos >= PulseTotal[i])
                {
                    PulseComptage[i] = 0;
                }
                break;
            case Bloc_Action::TRAIN_DE_SINUS:
                PulseOn[i] = 100 - Retard[i];
                PulseTotal[i] = 99; // Nombre impair pour éviter courant continu

                if (testTrame > 0)
                {
                    PulseOn[i] = testPulse;    // mode Test mesure de Puissance
                    PulseTotal[i] = testTrame; //
                }
                break;
            case Bloc_Action::PWM:
                Vout = int(RetardF[i] * 2.55);
                Vout = constrain(Vout, 0, 255);
                if (OutOn[i] == 1)
                    Vout = 255 - Vout;
                ledcWrite(Gpio[i], Vout);
                break;
            default:
                // Handle default case
                break;
            }
        }
    }
}

// ***********************************
// * Energie quotidienne equivalente * 
// ***********************************

void H_Ouvre_Equivalent(unsigned long dt)
{
  float Dheure = float(dt) / 3600000.0f;
  for (int i = 0; i < NbActions; i++)
  {
    Bloc_Action::RegulationType RegulationEnCours = BlocsActions[i].Regulation;
    if (RegulationEnCours != Bloc_Action::INACTIF) // Si la régulation n'est pas inactive

    { // valeur en RAM du Mode de regulation
      if (RegulationEnCours == Bloc_Action::DECOUPE_SINUS)
      {                                                      // Decoupe pour Triac
        float teta = 6.28318 * (100.0 - RetardF[i]) / 100.0; // 2*PI integral sin²
        BlocsActions[i].Hequiv += Dheure * (teta - sin(2.0 * teta) / 2.0) / 6.28318;
      }
      else
      {
        BlocsActions[i].Hequiv += Dheure * (100 - RetardF[i]) / 100.0;
      }
    }
    SortieNum1[BlocsActions[i].NumBloc] = BlocsActions[i].Hequiv; //Temps equivalent en heures
  }
}