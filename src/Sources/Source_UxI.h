#ifndef SOURCE_UXI_H
#define SOURCE_UXI_H
#include <Arduino.h>
extern byte pUxI;
extern unsigned int CalibU ;  //Calibration Routeur UxI
extern unsigned int CalibI;
extern float voltM[100];  //Voltage Mean value
extern float ampM[100];

void InitGpioAnalogique();
void Setup_UxI();
void LectureUxI(int8_t source);
void Calibration();

#endif