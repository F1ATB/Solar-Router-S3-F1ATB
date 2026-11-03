#pragma once
#include <Arduino.h>

extern byte LEDgroupe ;
extern bool LEDinit;
void Init_LED_OLED(void);
void Gestion_LEDs() ;
void OledPrint(const String& S) ;
void OledPrintln(const String& S) ;
void ValidToLED();
