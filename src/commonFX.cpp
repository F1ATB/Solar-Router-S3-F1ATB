//*******************
// Fonction communes
//*******************
#include "commonFX.h"
#include <Arduino.h>



String utf8ToLatin15(const String &utf8) {
  String latin15;
  latin15.reserve(utf8.length());  // optimisation mémoire

  for (size_t i = 0; i < utf8.length();) {
    uint8_t c = (uint8_t)utf8[i];

    if (c < 0x80) {
      // ASCII direct
      latin15 += (char)c;
      i++;
    } else if ((c & 0xE0) == 0xC0 && i + 1 < utf8.length()) {
      // Séquence UTF-8 sur 2 octets
      uint8_t c1 = (uint8_t)utf8[i + 1];
      if ((c1 & 0xC0) == 0x80) {
        uint16_t codepoint = ((c & 0x1F) << 6) | (c1 & 0x3F);
        if (codepoint <= 0xFF) {
          latin15 += (char)codepoint;
        } else {
          latin15 += '?';
        }
        i += 2;
      } else {
        latin15 += '?';
        i++;
      }
      
    } else if ((c & 0xF0) == 0xE0 && i + 2 < utf8.length()) {
      // Séquence UTF-8 sur 3 octets (ex: €, Œ…)
      uint8_t c1 = (uint8_t)utf8[i + 1];
      uint8_t c2 = (uint8_t)utf8[i + 2];
      if ((c1 & 0xC0) == 0x80 && (c2 & 0xC0) == 0x80) {
        uint16_t codepoint = ((c & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);

        // Mapping spécial Latin-15
        switch (codepoint) {
          case 0x20AC: latin15 += (char)0xA4; break;  // €
          case 0x0160: latin15 += (char)0xA6; break;  // Š
          case 0x0161: latin15 += (char)0xA8; break;  // š
          case 0x017D: latin15 += (char)0xB4; break;  // Ž
          case 0x017E: latin15 += (char)0xB8; break;  // ž
          case 0x0152: latin15 += (char)0xBC; break;  // Œ
          case 0x0153: latin15 += (char)0xBD; break;  // œ
          case 0x0178: latin15 += (char)0xBE; break;  // Ÿ
          case 0x2705: latin15 += (char)0xBB; break;  //  ✅
          case 0x26A0: latin15 += (char)0xA4; break;  // ⚠️
          case 0xFE0F: latin15 += (char)0x20; break;  //  Selecteur Emoji colore
          default:
            if (codepoint <= 0xFF) {
              latin15 += (char)codepoint;
            } else {
              latin15 += '?';
            }
        }
        i += 3;
      } else {
        latin15 += '?';
        i++;
      }
    } else {
      latin15 += '?';
      i++;
    }
  }

  return latin15;
}

void SplitS(String Str, String& Before, String Separ, String& After) {
  int p = Str.indexOf(Separ);
  Before = Str.substring(0, p);
  After = Str.substring(p + 1);
}

//JSON
//**********
String PrefiltreJson(const String &F1, const String &F2, String Json) {
  int p = Json.indexOf(F1);
  Json = Json.substring(p);
  p = Json.indexOf(F2);
  Json = Json.substring(p);
  return Json;
}
String SubJson(const String &F1, const String &F2, String Json) {
  int p = Json.indexOf(F1);
  Json = Json.substring(p);
  p = Json.indexOf(F2);
  Json = Json.substring(0,p+1);
  return Json;
}

float ValJson(const String &nom, String Json) {
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  int q = Json.indexOf(",");
  p = Json.indexOf("}");
  p = min(p, q);
  float val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    val = Json.toFloat();
  }
  return val;
}
long LongJson(const String &nom, String Json) {  // Pour éviter des problèmes d'overflow
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  int q = Json.indexOf(".");
  p = Json.indexOf("}");
  p = min(p, q);
  long val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    val = Json.toInt();
  }
  return val;
}

long myLongJson(const String &nom, String Json) {  // Alternative a LongJson au dessus pour extraire chez RTE nb jour Tempo  https://particulier.RTE.fr/services/rest/referentiel/getNbTempoDays?TypeAlerte=TEMPO
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  int q = Json.indexOf(",");  //<==== Recherche d'une virgule et non d'un point
  if (q == -1) q = 999;       //  /<==== Ajout de ces 2 lignes pour que la ligne p = min(p, q); ci dessous donne le bon résultat
  p = Json.indexOf("}");
  p = min(p, q);
  long val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    val = Json.toInt();
  }
  return val;
}
unsigned long ULongJson(const String &nom, String Json) {  // Alternative a LongJson au dessus pour extraire chez RTE nb jour Tempo  https://particulier.RTE.fr/services/rest/referentiel/getNbTempoDays?TypeAlerte=TEMPO
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  int q = Json.indexOf(",");  //<==== Recherche d'une virgule et non d'un point
  if (q == -1) q = 999;       //  /<==== Ajout de ces 2 lignes pour que la ligne p = min(p, q); ci dessous donne le bon résultat
  p = Json.indexOf("}");
  p = min(p, q);
  unsigned long val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    Json = "0000" + Json;
    int L = Json.length();
    unsigned long y = (Json.substring(0, L - 5)).toInt();  //Problème des valeurs signées dans un unsigned
    unsigned long z = (Json.substring(L - 5)).toInt();
    val = (y * 100000) + z;
  }
  return val;
}
int IntJson(const String &nom, String Json) {  // Pour éviter des problèmes d'overflow
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  int q = Json.indexOf(",");
  if (q == -1) q = 999;
  p = Json.indexOf("}");
  p = min(p, q);
  int val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    val = Json.toInt();
  }
  return val;
}
byte ByteJson(const String &nom, String Json) {  // Pour éviter des problèmes d'overflow
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  int q = Json.indexOf(",");
  if (q == -1) q = 999;
  p = Json.indexOf("}");
  p = min(p, q);
  byte val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    val = Json.toInt();
  }
  return val;
}
unsigned short UShortJson(const String &nom, String Json) {  // Pour éviter des problèmes d'overflow
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  int q = Json.indexOf(",");
  if (q == -1) q = 999;
  p = Json.indexOf("}");
  p = min(p, q);
  unsigned short val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    val = Json.toInt();
  }
  return val;
}
short ShortJson(const String &nom, String Json) {  // Pour éviter des problèmes d'overflow
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  int q = Json.indexOf(",");
  if (q == -1) q = 999;
  p = Json.indexOf("}");
  p = min(p, q);
  short val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    val = Json.toInt();
  }
  return val;
}


String StringJson(const String &nom, String Json) {
  int p = Json.indexOf(nom + "\":");
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  p = Json.indexOf("\"");
  Json = Json.substring(p + 1);
  p = Json.indexOf("\"");
  Json = Json.substring(0, p);
  return Json;
}
float ValJsonSG(String nom, String Json) {
  int p = Json.indexOf(nom);
  Json = Json.substring(p);
  p = Json.indexOf(":");
  Json = Json.substring(p + 1);
  Json.trim();
  p = Json.indexOf(",");
  float val = 0;
  if (p > 0) {
    Json = Json.substring(0, p);
    Json = Json.substring(1, Json.length()-1); 
    val = Json.toFloat();
  }
  return val;
}

