#pragma once
#include <Arduino.h>
#include "Communications/Server.h"
#include <ESPAsyncWebServer.h>

void onWsEvent(AsyncWebSocket *server,
               AsyncWebSocketClient *client,
               AwsEventType type,
               void *arg,
               uint8_t *data,
               size_t len);

void onWsEventAct(AsyncWebSocket *server,
               AsyncWebSocketClient *client,
               AwsEventType type,
               void *arg,
               uint8_t *data,
               size_t len);
void onWsEventBrute(AsyncWebSocket *server,
                  AsyncWebSocketClient *client,
                  AwsEventType type,
                  void *arg,
                  uint8_t *data,
                  size_t len);

void envoyerPuissanceSources();
void envoyerDonneesLentes();
void envoyerSortiesBlocs();
void envoyerBruteMessages();
void envoyerBruteSources();
void envoyerBrutesESP32();
