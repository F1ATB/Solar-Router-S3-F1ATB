#pragma once
#include <Arduino.h>

//Paramètres for Enphase-Envoy-Smetered
extern String TokenEnphase;
extern String EnphaseUser;
extern String EnphasePwd;
extern String Session_id;

#define TIMEOUT_READ_PAYLOAD 100
#define TIMEOUT_WAITING_ANSWER 1500
#define TIMEOUT_CONNECT 4000
#define TOKEN_MIN_LENGTH 200
#define ENPHASE_READING_PERIOD 2000

void Setup_Enphase(bool NewToken ,int8_t source);
uint32_t LectureEnphase(int8_t source);
