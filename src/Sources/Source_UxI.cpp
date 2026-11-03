// ****************************
// * Source de Mesures U et I *
// *          UXI             *
// ****************************
#include "Sources/Source_UxI.h"
#include <Arduino.h>
#include "config.h"
#include "Puissance.h"
#include "Ecrans/LEDs.h"
#include "commonFX.h"


//Parameters for UxI (1 seul UxI par routeur)
byte pUxI = 0;
unsigned int CalibU = 1000;  //Calibration Routeur UxI
unsigned int CalibI = 1000;
float voltM[100];   //Voltage Mean value
float ampM[100];

static byte AnalogIn0 = 5;
static byte AnalogIn1 = 6;
static byte AnalogIn2 = 7;


static byte Analog0[] = { 0, 5 };
static byte Analog1[] = { 0, 6 };
static byte Analog2[] = { 0, 7 };
static int value0;
static int volt[100];
static int amp[100];
static float KV = 0.2083;  //Calibration coefficient for the voltage. Value for CalibU=1000 at startup
static float KI = 0.0642;  //Calibration coefficient for the current. Value for CalibI=1000 at startup
static float kV = 0.2083;  //Calibration coefficient for the voltage. Corrected value
static float kI = 0.0642;  //Calibration coefficient for the current. Corrected value

void MeasurePower();
void ComputePower(int8_t source);

void InitGpioAnalogique(){
  //Entree Analogique UxI
  if (Source == "UxI" && pUxI == 0) pUxI = 1;
  AnalogIn0 = Analog0[pUxI];
  AnalogIn1 = Analog1[pUxI];
  AnalogIn2 = Analog2[pUxI];
}
void Setup_UxI() {
  for (int i = 0; i < 100; i++) {  //Reset table measurements
    voltM[i] = 0;
    ampM[i] = 0;
  }
}
void LectureUxI(int8_t source) {
  MeasurePower();
  ComputePower(source);
}
void MeasurePower() {  //Lecture Tension et courants pendant 20ms
  int iStore;
  value0 = analogRead(AnalogIn0);  //Mean value. Should be at 3.3v/2 
  static unsigned long OverflowOffset = 0;
  static unsigned long PrevMicros = 0;
  unsigned long NowMicros;
  unsigned long MeasureMillis = millis();
  while (millis() - MeasureMillis < 21) {  //Read values in continuous during 20ms. One loop is around 150 micro seconds
    NowMicros = micros();
    if (NowMicros < PrevMicros) {
      OverflowOffset = (OverflowOffset+ 7296)%20000;
    }
    iStore = ((NowMicros%20000 + OverflowOffset) % 20000) / 200;  //We have more results that we need during 20ms to fill the tables of 100 samples
    volt[iStore] = analogRead(AnalogIn1) - value0;//32
    amp[iStore] = analogRead(AnalogIn2) - value0;//33
    PrevMicros = NowMicros;
  }
}

void ComputePower(int8_t source) {
  float PWcal = 0;  //Computation Power in Watt
  float V;
  float I;
  float Uef2 = 0;
  float Ief2 = 0;
  int idx0 = 0;
  int j = 0;
  int8_t C0=source * 3; //Canal de mesure
  for (int i = 0; i < 100; i++) {
    voltM[i] = (19.0 * voltM[i] + float(volt[i])) / 20.0;  //Mean value. First Order Filter. Short Integration
    V = kV * voltM[i];
    Uef2 += sq(V);
    ampM[i] = (19.0 * ampM[i] + float(amp[i])) / 20.0;  //Mean value. First Order Filter
    I = kI * ampM[i];
    Ief2 += sq(I);
    PWcal += V * I;
  }
  
  Uef2 = Uef2 / 100.0;       //square of voltage
  Tension[C0] = sqrt(Uef2);    //RMS voltage
  Ief2 = Ief2 / 100.0;       //square of current
  Intensite[C0] = sqrt(Ief2);  // RMS current
  PWcal = PfloatMax(PWcal / 100.0);
  float PVA = PfloatMax(floor(Tension[C0] * Intensite[C0]));
  float PowerFactor_ = 0;
  if (PVA > 0) {
    PowerFactor_ = floor(100.0 * PWcal / PVA) / 100.0;
  }
  PowerFactor[C0] = PowerFactor_;
  if (PWcal >= 0) {
    EAS_float[C0] += PWcal / 90000.0;         // Watt Hour,Every 40ms. Soutirée
    Energie_Soutiree[C0] = (long)(EAS_float[C0]);  
    PuissanceS_inst[C0] = PWcal;
    PuissanceI_inst[C0] = 0;
    PVAS_inst[C0] = PVA;
    PVAI_inst[C0] = 0;
  } else {
    EAI_float[C0] += -PWcal / 90000.0;
    Energie_Injectee[C0] = (long)(EAI_float[C0]);
    PuissanceS_inst[C0] = 0;
    PuissanceI_inst[C0] = -PWcal;
    PVAS_inst[C0] = 0;
    PVAI_inst[C0] = PVA;
  }
  
  PVA_Valide[C0] = true;
  Filtre_Puissance(true,C0);
  AffectationPuissanceAuxActions(C0);
  Puissance_Max(C0);
  CalculEnergieJour(C0, true);
  EnergieActiveValide[C0+1] = false;
    
  LaSourceVar[source].DataBrute = "UxI Mesure: U=" + String(Tension[C0],0) + " V, I=" + String(Intensite[C0],0) + " A, P=" + String(PWcal,0) + " W"; //Sauvegarde des données brutes
  String S="";
  int i0 = 0;
      int i1 = 0;
      for (int i = 0; i < 100; i++) {
        i1 = (i + 1) % 100;
        if (voltM[i] <= 0 && voltM[i1] > 0) {
          i0 = i1;  // Point de départ tableau . Phase positive
          i = 100;
        }
      }
      for (int i = 0; i < 100; i++) {
        i1 = (i + i0) % 100;
        S += String(int(10 * voltM[i1])) + ",";  // Voltages*10. Increase dynamic
      }
      S += "V|" ;
      for (int i = 0; i < 100; i++) {
        i1 = (i + i0) % 100;
        S += String(int(10 * ampM[i1])) + ",";  // Currents*10
      }
      LaSourceVar[source].DataBrute += "|" + S+ "A";
}
void Calibration() {
  kV = KV * float(CalibU) / 1000.0;  //Calibration coefficient to be applied
  kI = KI * float(CalibI) / 1000.0;
}
