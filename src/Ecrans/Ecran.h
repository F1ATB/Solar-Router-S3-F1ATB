#pragma once
#include <Arduino.h>

// Pin definitions
#define GFX_BL 1
#define TOUCH_ADDR 0x3B
#define TOUCH_SDA 4
#define TOUCH_SCL 8
#define TOUCH_I2C_CLOCK 400000
#define TOUCH_RST_PIN 12
#define TOUCH_INT_PIN 11
#define AXS_MAX_TOUCH_NUMBER 1

//Paramètres écran
extern byte rotation;
extern unsigned long DurEcran ;
extern int16_t PageOld, PageActu , PageDelta , PageTotal ;
extern EXT_RAM_BSS_ATTR char MessageEcran[8192];




void Init_Ecran(void);
void Ecran_Loop();
void SetCouleurs();
void EcranPrintln(const String &Mm);
void EcranPrint(const String &Mm);

