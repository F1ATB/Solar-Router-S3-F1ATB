#ifndef COMMONFX_H
#define COMMONFX_H
#include <Arduino.h>
String utf8ToLatin15(const String &utf8);
void SplitS(String Str, String& Before, String Separ, String& After);

//JSON
//*******
String PrefiltreJson(const String &F1, const String &F2, String Json);
String SubJson(const String &F1, const String &F2, String Json) ;
float ValJson(const String &nom, String Json) ;
long LongJson(const String &nom, String Json) ;
long myLongJson(const String &nom, String Json) ;
unsigned long ULongJson(const String &nom, String Json) ;
int IntJson(const String &nom, String Json) ;
byte ByteJson(const String &nom, String Json);
unsigned short UShortJson(const String &nom, String Json) ;
short ShortJson(const String &nom, String Json) ;
String StringJson(const String &nom, String Json);
float ValJsonSG(String nom, String Json);

#endif
