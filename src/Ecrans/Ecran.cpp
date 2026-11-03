#include "Ecrans/Ecran.h"
#include <Arduino.h>
#include <U8g2lib.h> 
#include <Arduino_GFX_Library.h>
#include <Wire.h>
#include "config.h"
#include "Puissance.h"
#include "commonFX.h"
#include "Sources/Source_Linky.h"
#include "Communications/comSortie.h"
#include "Cartes/carte.h"

//Paramètres écran
byte rotation = 3;
unsigned long DurEcran = 30000;
unsigned long allumEcranMillis = 0;
int16_t PageOld = 3, PageActu = 3, PageDelta = 0, PageTotal = 4;
EXT_RAM_BSS_ATTR char MessageEcran[8192];



static int16_t EcranW, EcranH, EcranH20, EcranH40;


static int16_t CoulTexte, CoulFond, CoulBouton, CoulBoutFond, CoulBoutBord, CoulW, CoulWh, CoulTabTexte, CoulTabFond, CoulTabBord;
static int16_t CoulSaisieTexte, CoulSaisieFond, CoulSaisieBord, CoulTemp, CoulGrTexte, CoulGrFond, CoulMesFond, CoulMesTexte;
static unsigned long runtime_0 = 0, runtime_1 = 0;
static uint16_t compteurLoopAccueil = 0, compteurLoopMessage = 0, MesIdxDebut = 0, MesIdxFin = 0;

static uint16_t touchX = 0, touchY = 0;
int16_t DeltaTouchX = 0, DeltaTouchY = 0;

//Prototypes
void TouchSetup();
bool getTouchPoint(uint16_t &x, uint16_t &y, int16_t &dX, int16_t &dY);
void AccueiLoop();
void MessageLoop();
void PrintCentre(Arduino_Canvas *canva, const String &S, int16_t X, int16_t Y, uint8_t Sz);
void PrintGauche(Arduino_Canvas *canva, const String &S, int16_t X, int16_t Y, uint8_t Sz) ;
void PrintDroite(Arduino_Canvas *canva, const String &S, int16_t X, int16_t Y, uint8_t Sz) ;
void TraceTarif();
void SetCouleurs();
String ULtoHex(unsigned long x);
unsigned long ConvCouleur(String V);

Arduino_DataBus *bus = new Arduino_ESP32QSPI(45, 47, 21, 48, 40, 39);
Arduino_GFX *g = new Arduino_AXS15231B(bus, GFX_NOT_DEFINED, 0, false, 320, 480);
Arduino_Canvas *CanvaBase = new Arduino_Canvas(320, 480, g, 0, 0, 1);
Arduino_Canvas *CanvaAccueil = nullptr;
Arduino_Canvas *CanvaGr10mn = nullptr;
Arduino_Canvas *CanvaGr48h = nullptr;
Arduino_Canvas *CanvaMessage = nullptr;



void Init_Ecran(void) {
  if (ESP32_Type != CARTE_ECRAN) return;
  SetCouleurs();
  // Initialize Display
  CanvaBase->begin();
  CanvaBase->setUTF8Print(true);
  //CanvaMessage->setUTF8Print(true);

  // Initialize touch
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  Wire.setClock(TOUCH_I2C_CLOCK);

  // Configure touch pins
  pinMode(TOUCH_INT_PIN, INPUT_PULLUP);
  pinMode(TOUCH_RST_PIN, OUTPUT);
  digitalWrite(TOUCH_RST_PIN, LOW);
  delay(200);
  digitalWrite(TOUCH_RST_PIN, HIGH);
  delay(200);


  CanvaBase->setRotation(rotation);
  CanvaBase->fillScreen(CoulFond);
  CanvaBase->setTextColor(CoulTexte);
  EcranW = CanvaBase->width();
  EcranH = CanvaBase->height();
  EcranH20 = EcranH - 20;
  EcranH40 = EcranH - 40;

  CanvaAccueil = new Arduino_Canvas(EcranW, EcranH, g, 0, 0, 0);
  CanvaGr10mn = new Arduino_Canvas(EcranW, EcranH20, g, 0, 0, 0);
  CanvaGr48h = new Arduino_Canvas(EcranW, EcranH20, g, 0, 0, 0);
  CanvaMessage = new Arduino_Canvas(EcranW, EcranH, g, 0, 0, 0);

  CanvaAccueil->begin(GFX_SKIP_OUTPUT_BEGIN);
  CanvaGr10mn->begin(GFX_SKIP_OUTPUT_BEGIN);
  CanvaGr48h->begin(GFX_SKIP_OUTPUT_BEGIN);
  CanvaMessage->begin(GFX_SKIP_OUTPUT_BEGIN);

  CanvaAccueil->setTextColor(CoulTexte);
  CanvaAccueil->fillScreen(CoulFond);
  CanvaAccueil->fillRect(0, 0, EcranW, 20, CoulFond);
  CanvaAccueil->setFont(u8g2_font_10x20_tf);
  PrintCentre(CanvaAccueil, "ACCUEIL", CanvaAccueil->width() / 2, 18, 1);


  //Tableau consommations
  int16_t W = EcranW / 4;
  int16_t Hconso = 25;
  int16_t H = EcranH / 8;
  int16_t H1 = Hconso + H / 2 + 5;
  int16_t H2 = H1 + H;

  CanvaAccueil->fillRect(W, Hconso, 2 * W, H1 - Hconso, CoulTabFond);
  CanvaAccueil->setTextColor(CoulTabTexte, CoulTabFond);
  CanvaAccueil->setFont(u8g2_font_10x20_tf);
  PrintCentre(CanvaAccueil, "Soutirée", W + W / 2, Hconso + 20, 1);
  PrintCentre(CanvaAccueil, "Injectée", 2 * W + W / 2, Hconso + 20, 1);
  CanvaAccueil->fillRect(0, H1, 3 * W, H, CoulW);
  CanvaAccueil->fillRect(0, H2, 3 * W, H, CoulWh);
  CanvaAccueil->drawRect(0, H1, 3 * W, H, CoulTabBord);
  CanvaAccueil->drawRect(0, H2, 3 * W, H, CoulTabBord);
  CanvaAccueil->drawRect(W, Hconso, W, H2 + H - Hconso, CoulTabBord);
  CanvaAccueil->drawRect(2 * W, Hconso, W, H2 + H - Hconso, CoulTabBord);
  CanvaAccueil->setTextColor(CoulTabTexte, CoulW);
  CanvaAccueil->setFont(u8g2_font_10x20_tf);
  PrintGauche(CanvaAccueil, "P (W)", -2, H1 + 30, 2);
  CanvaAccueil->setTextColor(CoulTabTexte, CoulWh);
  PrintGauche(CanvaAccueil, "E (Wh)", -2, H2 + 30, 2);



  CanvaGr10mn->fillScreen(CoulGrFond);
  CanvaGr10mn->setTextColor(CoulTexte);
  CanvaGr10mn->fillRect(0, 0, EcranW, 20, CoulFond);
  CanvaGr10mn->setFont(u8g2_font_10x20_tf);
  PrintCentre(CanvaGr10mn, "GRAPHIQUE 10mn", CanvaGr10mn->width() / 2, 18, 1);
  CanvaGr10mn->setTextColor(CoulGrTexte);
  CanvaGr10mn->setTextBound(0, 0, EcranW, EcranH40);
  CanvaGr10mn->setTextWrap(true);
  PrintCentre(CanvaGr10mn, "GRAPHIQUE 10mn", CanvaGr10mn->width() / 2, 50, 3);

  CanvaGr48h->fillScreen(CoulGrFond);
  CanvaGr48h->setTextColor(CoulTexte);
  CanvaGr48h->fillRect(0, 0, EcranW, 20, CoulFond);
  CanvaGr48h->setFont(u8g2_font_10x20_tf);
  PrintCentre(CanvaGr48h, "GRAPHIQUE 48h", CanvaGr48h->width() / 2, 18, 1);
  CanvaGr48h->setTextColor(CoulGrTexte);
  CanvaGr48h->setTextBound(0, 0, EcranW, EcranH40);
  CanvaGr48h->setTextWrap(true);

  PrintCentre(CanvaGr48h, "GRAPHIQUE 48h", CanvaGr48h->width() / 2, 50, 3);

  CanvaMessage->fillScreen(CoulMesFond);
  CanvaMessage->fillRect(0, 0, EcranW, 20, CoulFond);
  CanvaMessage->setFont(u8g2_font_10x20_tf);
  PrintCentre(CanvaMessage, "Routeur Photovoltaïque F1ATB", CanvaMessage->width() / 2, 18, 1);
  CanvaMessage->setCursor(0, 40);
  CanvaMessage->setTextSize(1);
  CanvaMessage->setTextColor(CoulMesTexte);
  CanvaMessage->setTextWrap(true);
  CanvaBase->draw16bitRGBBitmap(0, 0, CanvaMessage->getFramebuffer(), EcranW, EcranH);
  CanvaBase->flush();


  pinMode(GFX_BL, OUTPUT);  //Back Light On
  digitalWrite(GFX_BL, HIGH);
  allumEcranMillis = millis();
}


void Ecran_Loop() {
  if (ESP32_Type != CARTE_ECRAN) return;
  if (DurEcran == 0) {
    digitalWrite(GFX_BL, HIGH);  //Toujours allumé
  } else {
    if (millis() - allumEcranMillis > DurEcran) {  //Duree allumage
      digitalWrite(GFX_BL, LOW);
    }
  }
  if ((millis() - runtime_0) > 20) {
    runtime_0 = millis();
    if (getTouchPoint(touchX, touchY, DeltaTouchX, DeltaTouchY)) {
        digitalWrite(GFX_BL, HIGH);
        allumEcranMillis = millis();
      if (DeltaTouchX < -30) {
        PageActu = (PageOld + 1) % PageTotal;
        PageDelta = 480;  //Delta en pixels
      }
      if (DeltaTouchX > 30) {
        PageActu = (PageTotal + PageOld - 1) % PageTotal;
        PageDelta = -480;  //Delta en pixels
      }
      if (PageActu == 3 && abs(DeltaTouchY) > 30) {  //Messages
        int16_t deltaLigne = abs(int(DeltaTouchY / 10));
        while (MesIdxDebut != MesIdxFin && deltaLigne != 0) {
          if (MessageEcran[MesIdxDebut] == '\n') deltaLigne--;
          if (DeltaTouchY < 0) {
            MesIdxDebut = (MesIdxDebut + 1) & 0x1FFF;
          } else {
            MesIdxDebut = (MesIdxDebut - 1) & 0x1FFF;
          }
        }
        MessageLoop();
        compteurLoopMessage = 2;  //pour attendre le defilement
      }
    }
    if (PageDelta != 0 || (millis() - runtime_1) > 300) {
      runtime_1 = millis();
      compteurLoopAccueil = (compteurLoopAccueil + 1) & 0x1F;
      compteurLoopMessage = (compteurLoopMessage + 1) & 0x1F;
      if (compteurLoopMessage == 1) MessageLoop();
      if (PageDelta > 0) {
        PageDelta = PageDelta - 80;
        if (PageDelta <= 0) {
          PageDelta = 0;
          PageOld = PageActu;
        }
      } else {
        PageDelta = PageDelta + 80;
        if (PageDelta >= 0) {
          PageDelta = 0;
          PageOld = PageActu;
        }
      }

      switch (PageActu) {
        case 0:
          if (compteurLoopAccueil % 4 == 0) AccueiLoop();
          CanvaBase->draw16bitRGBBitmap(PageDelta, 0, CanvaAccueil->getFramebuffer(), EcranW, EcranH);
          break;
        case 1:
          CanvaBase->draw16bitRGBBitmap(PageDelta, 0, CanvaGr10mn->getFramebuffer(), EcranW, EcranH20);
          break;
        case 2:
          CanvaBase->draw16bitRGBBitmap(PageDelta, 0, CanvaGr48h->getFramebuffer(), EcranW, EcranH20);
          break;
        case 3:
          CanvaBase->draw16bitRGBBitmap(PageDelta, 0, CanvaMessage->getFramebuffer(), EcranW, EcranH);
          break;
      }
      if (PageDelta > 0) CanvaBase->writeFastVLine(PageDelta, 0, EcranH20, RGB565_DARKGREY);
      if (PageDelta < 0) CanvaBase->writeFastVLine(PageDelta + EcranW, 0, EcranH20, RGB565_DARKGREY);
      CanvaBase->flush();
    }
  }
}
void AccueiLoop() {
  //Tableau consommations
  int16_t W = EcranW / 4;
  int16_t Hconso = 25;
  int16_t H = EcranH / 8;
  int16_t H1 = Hconso + H / 2 + 5;
  int16_t H2 = H1 + H;
  CanvaAccueil->setFont(u8g2_font_10x20_tf);
  CanvaAccueil->setTextColor(CoulTexte, CoulFond);
  PrintGauche(CanvaAccueil, DATE, 0, EcranH - 4, 1);
  TraceTarif();
  CanvaAccueil->fillRect(W + 1, H1 + 1, W - 2, H - 2, CoulW);
  CanvaAccueil->fillRect(2 * W + 1, H1 + 1, W - 2, H - 2, CoulW);
  CanvaAccueil->setTextColor(CoulTabTexte, CoulW);
  CanvaAccueil->setFont(u8g2_font_10x20_tf);
  PrintDroite(CanvaAccueil, String(PuissanceS[0]), 2 * W - 2, H1 + 30, 2);
  PrintDroite(CanvaAccueil, String(PuissanceI[0]), 3 * W - 2, H1 + 30, 2);
  CanvaAccueil->fillRect(W + 1, H2 + 1, W - 2, H - 2, CoulWh);
  CanvaAccueil->fillRect(2 * W + 1, H2 + 1, W - 2, H - 2, CoulWh);
  CanvaAccueil->setTextColor(CoulTabTexte, CoulWh);
  PrintDroite(CanvaAccueil, String(EnergieJour_Soutiree[0]), 2 * W - 2, H2 + 30, 2);
  PrintDroite(CanvaAccueil, String(EnergieJour_Injectee[0]), 3 * W - 2, H2 + 30, 2);
  uint16_t C = RGB565_RED;
  float Angle = 0;
  int Puissance = PuissanceS[0]-PuissanceI[0];
  C = (Puissance >= 0) ? RGB565_RED : RGB565_GREEN;
  Angle=360*float(Puissance)/8000.0;
  Angle = min(float(180), Angle);
  Angle = max(float(-180), Angle);
  Angle = Angle - 90;
  CanvaAccueil->fillCircle(3.5 * W, H2 - 5, H + 1, RGB565_WHITE);
  CanvaAccueil->fillArc(3.5 * W, H2 - 5, H - 1, 0, -90, Angle, C);
}
void EcranPrintln(const String &Mm) {
  SeriePrintln(Mm);
  if (ESP32_Type != CARTE_ECRAN) return;

  String M_Latin = utf8ToLatin15(Mm);
  for (int i = 0; i < M_Latin.length(); i++) {
    MessageEcran[MesIdxFin] = M_Latin.charAt(i);
    MesIdxFin++;
    MesIdxFin = MesIdxFin & 0x1FFF;
  }
  MessageEcran[MesIdxFin] = '\n';
  MesIdxFin++;
  MesIdxFin = MesIdxFin & 0x1FFF;
  compteurLoopMessage = 0;
}
void EcranPrint(const String &m) {
  SeriePrint(m);
  if (ESP32_Type != CARTE_ECRAN) return;
  String M_Latin = utf8ToLatin15(m);
  for (int i = 0; i < M_Latin.length(); i++) {
    MessageEcran[MesIdxFin] = M_Latin.charAt(i);
    MesIdxFin++;
    MesIdxFin = MesIdxFin & 0x1FFF;
  }
}
void MessageLoop() {
  CanvaMessage->setFont(u8g2_font_6x10_tf);
  int16_t p = -1;
  CanvaMessage->fillRect(0, 20, EcranW, EcranH20, CoulMesFond);
  CanvaMessage->setCursor(0, 40);
  uint16_t idx = MesIdxDebut;
  while (idx != MesIdxFin && CanvaMessage->getCursorY() < EcranH20) {
    CanvaMessage->print(MessageEcran[idx]);
    if (p == -1 && MessageEcran[idx] == '\n') p = idx;
    idx++;
    idx = idx & 0x1FFF;
  }
  if (CanvaMessage->getCursorY() >= EcranH20) {
    p++;
    p = p & 0x1FFF;
    MesIdxDebut = p;  //Ligne d'après
    CanvaMessage->fillRect(0, 0, EcranW, 20, CoulFond);
    CanvaMessage->setFont(u8g2_font_10x20_tf);
    PrintCentre(CanvaMessage, "MESSAGES", CanvaMessage->width() / 2, 18, 1);
    compteurLoopMessage = 0;
  }
}


bool getTouchPoint(uint16_t &x, uint16_t &y, int16_t &dX, int16_t &dY) {
  bool valide = true;
  uint16_t X0 = x;
  uint16_t Y0 = y;
  uint8_t data[AXS_MAX_TOUCH_NUMBER * 6 + 2] = { 0 };

  // Define the read command array properly
  const uint8_t read_cmd[11] = {
    0xb5, 0xab, 0xa5, 0x5a, 0x00, 0x00,
    (uint8_t)((AXS_MAX_TOUCH_NUMBER * 6 + 2) >> 8),
    (uint8_t)((AXS_MAX_TOUCH_NUMBER * 6 + 2) & 0xff),
    0x00, 0x00, 0x00
  };

  Wire.beginTransmission(TOUCH_ADDR);
  Wire.write(read_cmd, 11);
  if (Wire.endTransmission() != 0) {
    valide = false;
  } else {

    if (Wire.requestFrom(TOUCH_ADDR, sizeof(data)) != sizeof(data)) {
      valide = false;
    } else {
      for (int i = 0; i < sizeof(data); i++) {
        data[i] = Wire.read();
      }

      if (data[1] > 0 && data[1] <= AXS_MAX_TOUCH_NUMBER) {
        uint16_t rawX = ((data[2] & 0x0F) << 8) | data[3];
        uint16_t rawY = ((data[4] & 0x0F) << 8) | data[5];
        if (rawX > 500 || rawY > 500) valide = false;
        switch (rotation) {
          case 0:
            x = rawX;
            y = rawY;
            break;
          case 1:
            y = map(rawX, 0, 320, 320, 0);
            x = rawY;
            break;
          case 2:
            x = map(rawX, 0, 320, 320, 0);
            y = map(rawY, 0, 480, 480, 0);
            break;
          case 3:
            y = rawX;
            x = map(rawY, 0, 480, 480, 0);
            break;
        }

      } else {
        valide = false;
      }
    }
  }
  if (valide) {
    if (dX == 1000) {  //ancien pas valide
      dX = 0;
      dY = 0;
    } else {
      dX = x - X0;
      dY = y - Y0;
    }
  } else {
    dX = 1000;  //Pas valide
  }
  return valide;
}


void PrintCentre(Arduino_Canvas *canva, const String &S, int16_t X, int16_t Y, uint8_t Sz) {
  int16_t x1, y1;
  uint16_t w, h;
  canva->setTextSize(Sz);
  canva->getTextBounds(utf8ToLatin15(S), 0, 0, &x1, &y1, &w, &h);
  canva->setCursor(X - w / 2, Y);
  canva->print(utf8ToLatin15(S));
}
void PrintGauche(Arduino_Canvas *canva, const String &S, int16_t X, int16_t Y, uint8_t Sz) {
  canva->setTextSize(Sz);
  canva->setCursor(X + 4, Y + 3);
  canva->print(utf8ToLatin15(S));
}
void PrintDroite(Arduino_Canvas *canva, const String &S, int16_t X, int16_t Y, uint8_t Sz) {
  if (X < 0) X = canva->width() / 2;
  canva->setTextSize(Sz);
  uint16_t w, h;
  int16_t x1, y1;
  canva->getTextBounds(utf8ToLatin15(S), 0, 0, &x1, &y1, &w, &h);
  canva->setCursor(X - w - 4, Y + 3);
  canva->print(utf8ToLatin15(S));
}
void TraceTarif() {
  String Tarif[5] = { "PLEINE", "CREUSE", "BLEU", "BLANC", "ROUGE" };
  uint16_t couleur[5] = { RGB565_RED, RGB565_GREEN, RGB565_BLUE, RGB565_WHITE, RGB565_RED };
  int16_t i;
  int16_t W, H;
  W = 40;  //+ 20 * float(rotation % 2);?????????
  H = 20;  //- 10 * float(rotation % 2);
  if (LTARF != "") {
    CanvaAccueil->drawRect(EcranW - W * 3, EcranH - H, 3 * W - 1, H - 1, CoulSaisieBord);
    for (i = 0; i < 5; i++) {
      if (LTARF.indexOf(Tarif[i]) >= 0) {
        CanvaAccueil->fillRect(EcranW - W * 3 + 1, EcranH - H + 1, 3 * W - 2, H - 2, couleur[i]);
      }
    }
  }

  long lendemain = int(strtol(STGEt.c_str(), NULL, 16)) / 4;
  if (lendemain > 0) {
    CanvaAccueil->drawRect(EcranW - W, EcranH - H, W - 1, H - 1, CoulSaisieBord);
    CanvaAccueil->fillRect(EcranW - W + 1, EcranH - H + 1, W - 2, H - 2, couleur[lendemain + 1]);
  }
}
void SetCouleurs() { // A VERIFIER PB 24bits/16 bits
  
    CoulTexte = RGB565_WHITE;
    CoulFond = RGB565_NAVY;
    CoulBouton = RGB565_WHITE;
    CoulBoutFond = RGB565_LIGHTGREY;
    CoulBoutBord = RGB565_DARKGREY;
    CoulW = RGB565_RED;
    CoulWh = RGB565_YELLOW;
    CoulTabTexte = RGB565_BLACK;
    CoulTabFond = RGB565_LIGHTGREY;
    CoulTabBord = RGB565_DARKGREY;
    CoulSaisieTexte = RGB565_WHITE,
    CoulSaisieFond = RGB565_DARKGREY;
    CoulSaisieBord = RGB565_BLACK;
    CoulTemp = RGB565_GREEN;
    CoulGrTexte = RGB565_WHITE;
    CoulGrFond = RGB565_BLACK;
    CoulMesFond = RGB565_BLACK;
    CoulMesTexte = RGB565_WHITE;
  
}
//Gestion couleurs
String ULtoHex(unsigned long x) {
  char buffer[15];
  ltoa(x, buffer, 16);
  String S = "000000" + String(buffer);
  int p = S.length();
  S = "#" + S.substring(p - 6);  //Format pour HTML color
  return S;
}
unsigned long ConvCouleur(String V) {  //Notation CSS en UL
  return strtoul(V.c_str(), NULL, 16);
}
