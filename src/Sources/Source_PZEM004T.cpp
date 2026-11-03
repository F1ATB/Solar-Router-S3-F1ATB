#include "Sources/Source_PZEM004T.h"
#include <Arduino.h>
#include "config.h"
#include "Communications/comSortie.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "Communications/TempoRTE.h"
#include "Heure.h"
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem1(Serial1, RXD1, TXD1);
PZEM004Tv30 pzem2(Serial2, RXD2, TXD2);

void Setup_PZEM004T(int8_t source)
{
    delay(20);
    if (LaSource[source].Port == 1)
    {
        Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
        delay(500);
    }
    else
    {
        Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
        delay(500);
    }

    delay(100);
};
void LecturePZEM004T(int8_t source)
{

    float tension = 0.0;          // Volts
    float courant = 0.0;          // Ampères
    float puissance = 0.0;        // Watts
    float energie = 0.0;          // kWh
    float frequence = 0.0;        // Hz
    float facteurPuissance = 0.0; // 0.00 – 1.00
    int8_t C0 = source * 3;       // Canal de mesure

    if (LaSource[source].Port == 1)
    {
        tension = pzem1.voltage();     // Volts
        courant = pzem1.current();     // Ampères
        puissance = pzem1.power();     // Watts
        energie = pzem1.energy();      // kWh
        frequence = pzem1.frequency(); // Hz
        facteurPuissance = pzem1.pf(); // 0.00 – 1.00
    }
    else
    {

        tension = pzem2.voltage();     // Volts
        courant = pzem2.current();     // Ampères
        puissance = pzem2.power();     // Watts
        energie = pzem2.energy();      // kWh
        frequence = pzem2.frequency(); // Hz
        facteurPuissance = pzem2.pf(); // 0.00 – 1.00
    }
    Energie_Soutiree[C0] = long(energie * 1000); // Conversion en Wh
    Energie_Injectee[C0] = 0;                    // Le PZEM004T ne mesure pas l'énergie injectée
    if (facteurPuissance > 0)
    {
        PVAS[C0] = int(puissance / facteurPuissance); // Puissance apparente en VA
    }
    else
    {
        PVAS[C0] = 0;
    }
    PVAI[C0] = 0; // Le PZEM004T ne mesure pas la puissance apparente injectée
    PuissanceS[C0] = int(puissance); // Puissance active soutirée en W
    PuissanceI[C0] = 0;              // Le PZEM004T
    Tension[C0] = tension;
    Intensite[C0] = courant;
    PowerFactor[C0] = facteurPuissance;
    Frequence_[C0] = frequence;
    LaSourceVar[source].DataBrute = "PZEM-004T Mesure: U=" + String(tension,1) + " V, I=" + String(courant,2) + " A, P=" + String(puissance,1) + " W"; //Sauvegarde des données brutes
    LaSourceVar[source].DataBrute += ", E=" + String(energie,3) + " kWh, F=" + String(frequence,1) + " Hz, PF=" + String(facteurPuissance,2); //Sauvegarde des données brutes   
    Puissance_Max(C0);
    Filtre_Puissance(false,C0);
    AffectationPuissanceAuxActions(C0);
    CalculEnergieJour(C0, true);
    EnergieActiveValide[C0+1] = false;
    EnergieActiveValide[C0+2] = false;
}