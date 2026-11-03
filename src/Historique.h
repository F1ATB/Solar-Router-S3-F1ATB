#pragma once
#include <Arduino.h>
#include "config.h"
#include "Puissance.h"
#include <ArduinoJson.h>
#include <Temperatures/Temperature.h>
#include "BlocActions.h"

extern EXT_RAM_BSS_ATTR int16_t tabPw_2s[NB_CANAUX_MAX][300];  // Puissance Active: toutes les 2s sur 10mn
extern EXT_RAM_BSS_ATTR int16_t tabPva_2s[NB_CANAUX_MAX][300];   // Puissance Apparente
extern EXT_RAM_BSS_ATTR int16_t tabTemp_2s[CANAUX_TEMPERATURE][300];   // Température *10
extern EXT_RAM_BSS_ATTR int8_t tabOuvre_2s[BLOCS_ACTION_MAX][300];   // Ouverture


extern EXT_RAM_BSS_ATTR int16_t tabPw_5mn[NB_CANAUX_MAX][600];  // Puissance Active: toutes les 5mn sur 48h
extern EXT_RAM_BSS_ATTR int16_t tabPva_5mn[NB_CANAUX_MAX][600];   // Puissance Apparente
extern EXT_RAM_BSS_ATTR int16_t tabTemp_5mn[CANAUX_TEMPERATURE][600];   // Température *10
extern EXT_RAM_BSS_ATTR int8_t tabOuvre_5mn[BLOCS_ACTION_MAX][600];   // Ouverture


void Init_Historique();
void SauvegardeHistorique();
String SerialHistorique10mn();
String SerialHistorique48h();
void RAZ_Histo_48h();
