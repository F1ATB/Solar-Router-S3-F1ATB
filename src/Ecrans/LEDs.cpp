//***********************
//* Gestion LEDs et OLED*
//***********************
#include "Ecrans/LEDs.h"
#include "Ecrans/Ecran.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "config.h"
#include "Puissance.h"
#include <WiFi.h>
#include "Cartes/carte.h"
#include "BlocActions.h"

// Définition pour SH1106 (I2C 128x64)
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2_sh1106(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2_ssd1306(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
// Pointeur générique vers l’écran actif
U8G2 u8g2;

byte LEDgroupe = 0;  
bool LEDinit=false;                                               // 0:pas de LED,
static byte LEDyellow[] = {0, 15, 0, 0, 0, 0, 48, 0, 0, 0, 15, 15}; // Ou SDA pour OLED
static byte LEDgreen[] = {0, 16, 0, 0, 0, 0, 48, 0, 0, 0, 16, 16};  // ou SCL pour OLED
static char OledBuffer[5][64];
static char Oled_sans_cr[64];
static int cptLEDyellow = 0;
static int cptLEDgreen = 0;

// Prototypes
void OledDisp();

void Init_LED_OLED(void)
{
  LEDinit = false;
  EcranPrintln("Initialisation LED et OLED du groupe :" + String(LEDgroupe));
  // LEDgroupe = 0;   //0:pas de LED,1à 9 LEDs, 10 et 11 écran OLED SSD1306  et   OLED SH1106
  if (LEDgroupe > 0 && LEDgroupe < 6)
  { // Simples LEDs
    pinMode(LEDyellow[LEDgroupe], OUTPUT);
    pinMode(LEDgreen[LEDgroupe], OUTPUT);
  }
  if (LEDgroupe >= 10)
  {
    if (LEDgroupe == 10)
      u8g2 = u8g2_ssd1306;
    if (LEDgroupe == 11)
      u8g2 = u8g2_sh1106;

    Wire.begin(LEDyellow[LEDgroupe], LEDgreen[LEDgroupe]); // SDA et SCL
    u8g2.begin();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_9x15B_tr);
    char P[10];
    strcpy(P, "F1ATB");
    int W = u8g2.getUTF8Width(P);
    u8g2.drawUTF8(64 - W / 2, 14, P);
    u8g2.sendBuffer();
    delay(300);
  }
  Oled_sans_cr[0] = '\0';
  LEDinit = true;
}

void Gestion_LEDs()
{

  int retard_min = 100;
  int retardI;
  cptLEDyellow++;
  if ((WiFi.status() != WL_CONNECTED && ESP32_Type < CARTE_ETHERNET) || (EthernetBug > 0 && ESP32_Type >= CARTE_ETHERNET))
  { // Attente connexion au Wifi ou ethernet
    if (WiFi.getMode() == WIFI_STA)
    { // en  Station mode
      cptLEDyellow = (cptLEDyellow + 6) % 10;
      cptLEDgreen = cptLEDyellow;
    }
    else
    { // AP Mode
      cptLEDyellow = cptLEDyellow % 10;
      cptLEDgreen = (cptLEDyellow + 5) % 10;
    }
  }
  else
  {
    for (int i = 0; i < NbActions; i++)
    {
      retardI = Retard[i];
      retard_min = min(retard_min, retardI);
    }
    if (retard_min < 100)
    {
      cptLEDgreen = int((cptLEDgreen + 1 + 8 / (1 + retard_min / 10))) % 10;
    }
    else
    {
      cptLEDgreen = 10;
    }
  }
  if (!LEDinit)
    return;
  if (LEDgroupe > 0 && LEDgroupe < 6)
  {
    int L = 0, H = 1; // LED classique

    if (cptLEDyellow > 5)
    {
      digitalWrite(LEDyellow[LEDgroupe], L);
    }
    else
    {
      digitalWrite(LEDyellow[LEDgroupe], H);
    }
    if (cptLEDgreen > 5)
    {
      digitalWrite(LEDgreen[LEDgroupe], L);
    }
    else
    {
      digitalWrite(LEDgreen[LEDgroupe], H);
    }
  }
  if (LEDgroupe == 6)
  { // RGB LED
    if (WiFi.status() != WL_CONNECTED)
    {
      if (cptLEDgreen > 5)
      {
        rgbLedWrite(LEDgreen[LEDgroupe], 0, 0, 0); // Off
      }
      else
      {
        rgbLedWrite(LEDgreen[LEDgroupe], 0, 0, 100); // Blue
      }
    }
    else
    {
      if (cptLEDyellow <= 5)
      {
        rgbLedWrite(LEDyellow[LEDgroupe], 100, 0, 0); //  Red
      }
      else
      {
        if (cptLEDgreen <= 5)
        {
          rgbLedWrite(LEDgreen[LEDgroupe], 0, 100, 0); // vert
        }
        else
        {
          rgbLedWrite(LEDgreen[LEDgroupe], 0, 0, 0); // Off
        }
      }
    }
  }
  if (LEDgroupe >= 10)
  {
    // Début du buffer
  }
}
void ValidToLED()
{
  if (cptLEDyellow > 30)
  {
    cptLEDyellow = 3;
  }
}

void OledPrint(const String &S)
{
  
  if (LEDgroupe >= 10 && LEDinit)
  {
    strncat(Oled_sans_cr, S.c_str(), sizeof(Oled_sans_cr) - strlen(Oled_sans_cr) - 1);
  }
}
void OledPrintln(const String &S)
{
  if (LEDgroupe >= 10 && LEDinit )
  {
    u8g2.setFont(u8g2_font_6x10_tf);
    int maxWidth = 128;

    char buffer[400];
    char temp[400];

    int i;
    int nbligne = 5;

    strcpy(temp, Oled_sans_cr);
    Oled_sans_cr[0] = '\0';
    strncat(temp, S.c_str(), sizeof(temp) - strlen(temp) - 1);
    // const char* p = S.c_str();  // pointeur d’écriture
    const char *p = temp; // pointeur d’écriture

    while (*p)
    {
      int len = 0;
      int w = 0;
      const char *lastSpace = nullptr;
      // Construire une ligne qui ne dépasse pas maxWidth
      while (*p && w < maxWidth)
      {
        if (*p == '\r' || *p == '\n')
        {
          buffer[len] = ' ';
        }
        else
        {
          buffer[len] = *p;
        }
        buffer[len + 1] = '\0';
        int newW = u8g2.getUTF8Width(buffer);
        if (newW > maxWidth)
          break; // dépassement largeur
        if (*p == ' ')
          lastSpace = p; // mémorise dernier espace
        w = newW;
        len++;
        p++;
      }

      // Si on coupe au milieu d’un mot, revenir au dernier espace
      if (*p && lastSpace)
      {
        int cutLen = lastSpace - (p - len) + 1;
        buffer[cutLen] = '\0';
        p = lastSpace + 1;
      }
      else
      {
        buffer[len] = '\0';
      }

      // Affiche la ligne

      for (i = 0; i < nbligne - 1; i++)
      {
        strcpy(OledBuffer[i], OledBuffer[i + 1]);
      }
      strcpy(OledBuffer[nbligne - 1], buffer);
    }
    OledDisp();
  }
}
void OledDisp()
{
  if (LEDgroupe >= 10 && LEDinit )
  {
    int nbligne = 5;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_9x15B_tr);
    int Pw = PuissanceS[0] - PuissanceI[0]; // Puissance Source 0
    char P[20];
    sprintf(P, "%d W", Pw); // Puissance
    int W = u8g2.getUTF8Width(P);
    u8g2.drawUTF8(64 - W / 2, 15, P);
    u8g2.setFont(u8g2_font_6x10_tf);
    int retard_min = 100;
    int retardI;
    for (int i = 0; i < NbActions; i++)
    {
      retardI = Retard[i];
      retard_min = min(retard_min, retardI);
    }
    sprintf(P, "%d %%", 100 - retard_min); // Ouverture
    W = u8g2.getUTF8Width(P);
    u8g2.drawUTF8(128 - W, 15, P);
    int lineHeight = 10;
    int y = 2 * lineHeight + 4;
    for (int i = 0; i < nbligne; i++)
    {
      u8g2.drawUTF8(0, y, OledBuffer[i]);
      y += lineHeight;
    }
    u8g2.sendBuffer();
  }
}
