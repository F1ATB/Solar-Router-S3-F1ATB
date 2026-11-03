#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

void Init_Server();


extern AsyncWebSocket ws;
extern AsyncWebSocket wsAct;
extern AsyncWebSocket wsBrute;


