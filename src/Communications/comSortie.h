#ifndef COMSORTIE_H
#define COMSORTIE_H
#include <Arduino.h>

#define SER_BUF_SIZE 4096
extern byte pSerial1,pSerial2 ;             //Choix Pin port serie
extern int8_t RXD1 , TXD1 ;  //Port serie 1
extern int8_t RXD2 , TXD2 ;  //Port serie 2
extern HardwareSerial MySerial;

extern EXT_RAM_BSS_ATTR String MessageH[20];
extern int idxMessage ;

void InitGpioSerial();
void LireSerial();
void InfoVersSerie();
void infoSerie();
void InitMessages();

void Init_TelnetServer();
void GestionTelnet();
void SeriePrint(const String& message) ;
void SeriePrintln(const String& message);

void StockMessage(String m);

extern void SauveConfig();
#endif