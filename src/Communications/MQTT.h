#ifndef MQTT_H
#define MQTT_H
#include <Arduino.h>




extern unsigned int MQTTRepet ;
extern unsigned long MQTTIP ;
extern unsigned int MQTTPort ;
extern String MQTTUser ;
extern String MQTTPwd ;
extern String MQTTPrefix ;  // prefix obligatoire pour l'auto-discovery entre HA et Core-Mosquitto (par défaut c'est homeassistant)
extern String MQTTPrefixEtat ;
extern String MQTTdeviceName ;
extern String TopicP ;
extern byte subMQTT ;

extern unsigned long previousMQTTenvoiMillis;
extern unsigned long previousMQTTMillis;
extern unsigned long LastPwMQTTMillis ;
extern unsigned long PeriodeMQTTMillis ;

//Paramètres pour puissance via MQTT A TRANSFERER
extern String P_MQTT_Brute ;
extern float PwMQTT;
extern float PvaMQTT ;
extern float PfMQTT ;

extern bool MQTT_Discovered ;

void GestionMQTT();

#endif