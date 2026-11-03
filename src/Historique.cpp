#include "Historique.h"

EXT_RAM_BSS_ATTR int16_t tabPw_2s[NB_CANAUX_MAX][300];        // Puissance Active: toutes les 2s sur 10mn
EXT_RAM_BSS_ATTR int16_t tabPva_2s[NB_CANAUX_MAX][300];       // Puissance Apparente
EXT_RAM_BSS_ATTR int16_t tabTemp_2s[CANAUX_TEMPERATURE][300]; // Température *10
EXT_RAM_BSS_ATTR int8_t tabOuvre_2s[BLOCS_ACTION_MAX][300];   // Ouverture
int16_t IdxStock2s;

EXT_RAM_BSS_ATTR int16_t tabPw_5mn[NB_CANAUX_MAX][600];        // Puissance Active: toutes les 5mn sur 48h
EXT_RAM_BSS_ATTR int16_t tabPva_5mn[NB_CANAUX_MAX][600];       // Puissance Apparente
EXT_RAM_BSS_ATTR int16_t tabTemp_5mn[CANAUX_TEMPERATURE][600]; // Température *10
EXT_RAM_BSS_ATTR int8_t tabOuvre_5mn[BLOCS_ACTION_MAX][600];   // Ouverture
int16_t IdxStock5mn;

void Init_Historique()
{
    IdxStock2s = 0;
    for (int8_t canal = 0; canal < NB_CANAUX_MAX; canal++)
    {
        for (int16_t i = 0; i < 300; i++)
        {
            tabPw_2s[canal][i] = 0;
            tabPva_2s[canal][i] = 0;
        }
    }
    for (int8_t canal = 0; canal < CANAUX_TEMPERATURE; canal++)
    {
        for (int16_t i = 0; i < 300; i++)
        {
            tabTemp_2s[canal][i] = -2731; //Dixieme de degrés 0°K
        }
    }
    for (int8_t canal = 0; canal < BLOCS_ACTION_MAX; canal++)
    {
        for (int16_t i = 0; i < 300; i++)
        {
            tabOuvre_2s[canal][i] = 0; 
        }
    }
    RAZ_Histo_48h();
}

void SauvegardeHistorique()
{
    for (int8_t canal = 0; canal < NB_CANAUX_MAX; canal++)
    {
        tabPw_2s[canal][IdxStock2s] = PuissanceS[canal] - PuissanceI[canal];
        tabPva_2s[canal][IdxStock2s] = PVAS[canal] - PVAI[canal];
    }
    for (int8_t canal = 0; canal < CANAUX_TEMPERATURE; canal++)
    {
        tabTemp_2s[canal][IdxStock2s] = int(10.0f * temperature[canal]); // Temperature en dixième de degré
    }
    for (int8_t canal = 0; canal < BLOCS_ACTION_MAX; canal++)
    {
        tabOuvre_2s[canal][IdxStock2s] = 100 - Retard[canal]; //Ouverture 0 à 100%;
    }
    if (IdxStock2s == 0 || IdxStock2s == 150) // Sauvegarde toutes les 5mn
    {
        for (int8_t canal = 0; canal < NB_CANAUX_MAX; canal++)
        {
            tabPw_5mn[canal][IdxStock5mn] = tabPw_2s[canal][IdxStock2s];
            tabPva_5mn[canal][IdxStock5mn] = tabPva_2s[canal][IdxStock2s];
        }
        for (int8_t canal = 0; canal < CANAUX_TEMPERATURE; canal++)
        {
            tabTemp_5mn[canal][IdxStock5mn] = tabTemp_2s[canal][IdxStock2s]; // Temperature en dixième de degré
        }
        for (int8_t canal = 0; canal < BLOCS_ACTION_MAX; canal++)
        {
            tabOuvre_5mn[canal][IdxStock5mn] = tabOuvre_2s[canal][IdxStock2s];
        }
        IdxStock5mn = (IdxStock5mn + 1) % 600;
    }
    IdxStock2s = (IdxStock2s + 1) % 300;
}

String SerialHistorique10mn()
{
    JsonDocument doc;
    doc["MessageType"] = "Historique 10mn";
    doc["HeureCouranteDeci"] = HeureCouranteDeci;

    //==Les Puissances et PVA des sources actives==
    JsonObject valSources = doc["lesSources"].to<JsonObject>();
    JsonArray arrNumSource = valSources["numSource"].to<JsonArray>();
    JsonArray arrSource = valSources["source"].to<JsonArray>();
    JsonArray arrNbC = valSources["nbCanal"].to<JsonArray>();

    for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
    {
        bool hasCanal = false;
        for (int8_t canalV = 0; canalV < NB_CANAUX_PAR_SOURCE; canalV++)
        {
            if (LaSource[source].CanalVoie[canalV] >= 0)
            {
                hasCanal = true;
                break;
            }
        }
        if (hasCanal)
        {
            int8_t nbCanal = 0;
            arrNumSource.add(source);
            JsonObject sourceObj = arrSource.add<JsonObject>();
            JsonArray arrNumCanaux = sourceObj["Canaux"].to<JsonArray>();

            for (int8_t canalV = 0; canalV < NB_CANAUX_PAR_SOURCE; canalV++)
            {
                if (LaSource[source].CanalVoie[canalV] >= 0)
                {
                    nbCanal++;
                    int8_t canal = source * 3 + canalV;
                    JsonObject canalObj = arrNumCanaux.add<JsonObject>();
                    canalObj["numCanal"] = canalV;
                    JsonArray arrPw = canalObj["Pw"].to<JsonArray>();
                    JsonArray arrPva = canalObj["Pva"].to<JsonArray>();
                    int16_t idx = IdxStock2s;
                    for (int16_t i = 0; i < 300; i++)
                    {
                        arrPw.add(tabPw_2s[canal][idx]);
                        arrPva.add(tabPva_2s[canal][idx]);
                        idx = (idx + 1) % 300;
                    }
                }
            }

            arrNbC.add(nbCanal);
        }
    }


     //Les 4 canaux de température
    JsonObject valTemperature = doc["lesTemperatures"].to<JsonObject>();
    JsonArray arrTempCanal = valTemperature["CanalTemp"].to<JsonArray>();
     for (int8_t canal = 0; canal < CANAUX_TEMPERATURE; canal++)
     {
         JsonObject canalObj = arrTempCanal.add<JsonObject>();
         JsonArray arrTemp = canalObj["Temp"].to<JsonArray>();
         int16_t idx = IdxStock2s;
         for (int16_t i = 0; i < 300; i++)
         {
             arrTemp.add(tabTemp_2s[canal][idx]);
             idx = (idx + 1) % 300;
         }
     }

       //Les Ouvertures des actions actives
    JsonObject valOuvre = doc["lesOuvertures"].to<JsonObject>();
    JsonArray arrOuvreBloc = valOuvre["BlocOuvre"].to<JsonArray>();
    valOuvre["NbActions"] = NbActions;
     for (int8_t bloc = 0; bloc < NbActions; bloc++)
     {
         JsonObject blocObj = arrOuvreBloc.add<JsonObject>();
         blocObj["nomAction"] = BlocsActions[bloc].Nom;
         JsonArray arrOuvre = blocObj["Ouvre"].to<JsonArray>();
         int16_t idx = IdxStock2s;
         for (int16_t i = 0; i < 300; i++)
         {
             arrOuvre.add(tabOuvre_2s[bloc][idx]);
             idx = (idx + 1) % 300;
         }
     }


    String json;
    serializeJson(doc, json);
    return json;
}
void RAZ_Histo_48h()
{
    IdxStock5mn = 0;
    for (int8_t canal = 0; canal < NB_CANAUX_MAX; canal++)
    {
        for (int16_t i = 0; i < 600; i++)
        {
            tabPw_5mn[canal][i] = 0;
            tabPva_5mn[canal][i] = 0;
        }
    }
    for (int8_t canal = 0; canal < CANAUX_TEMPERATURE; canal++)
    {
        for (int16_t i = 0; i < 600; i++)
        {
            tabTemp_5mn[canal][i] = -2731;;
        }
    }
    for (int8_t bloc = 0; bloc < BLOCS_ACTION_MAX; bloc++)
    {
        for (int16_t i = 0; i < 600; i++)
        {
            tabOuvre_5mn[bloc][i] = 0;
        }
    }
}
String SerialHistorique48h(){
    JsonDocument doc;
    doc["MessageType"] = "Historique 48h";
    doc["HeureCouranteDeci"] = HeureCouranteDeci;

    //==Les Puissances et PVA des sources actives==
    JsonObject valSources = doc["lesSources"].to<JsonObject>();
    JsonArray arrNumSource = valSources["numSource"].to<JsonArray>();
    JsonArray arrSource = valSources["source"].to<JsonArray>();
    JsonArray arrNbC = valSources["nbCanal"].to<JsonArray>();

    for (int8_t source = 0; source < NB_SOURCES_DEVICE_MAX; source++)
    {
        bool hasCanal = false;
        for (int8_t canalV = 0; canalV < NB_CANAUX_PAR_SOURCE; canalV++)
        {
            if (LaSource[source].CanalVoie[canalV] >= 0)
            {
                hasCanal = true;
                break;
            }
        }
        if (hasCanal)
        {
            int8_t nbCanal = 0;
            arrNumSource.add(source);
            JsonObject sourceObj = arrSource.add<JsonObject>();
            JsonArray arrNumCanaux = sourceObj["Canaux"].to<JsonArray>();

            for (int8_t canalV = 0; canalV < NB_CANAUX_PAR_SOURCE; canalV++)
            {
                if (LaSource[source].CanalVoie[canalV] >= 0)
                {
                    nbCanal++;
                    int8_t canal = source * 3 + canalV;
                    JsonObject canalObj = arrNumCanaux.add<JsonObject>();
                    canalObj["numCanal"] = canalV;
                    JsonArray arrPw = canalObj["Pw"].to<JsonArray>();
                    JsonArray arrPva = canalObj["Pva"].to<JsonArray>();
                    int16_t idx = IdxStock5mn;
                    for (int16_t i = 0; i < 300; i++)
                    {
                        arrPw.add(tabPw_5mn[canal][idx]);
                        arrPva.add(tabPva_5mn[canal][idx]);
                        idx = (idx + 1) % 300;
                    }
                }
            }

            arrNbC.add(nbCanal);
        }
    }


     //Les 4 canaux de température
    JsonObject valTemperature = doc["lesTemperatures"].to<JsonObject>();
    JsonArray arrTempCanal = valTemperature["CanalTemp"].to<JsonArray>();
     for (int8_t canal = 0; canal < CANAUX_TEMPERATURE; canal++)
     {
         JsonObject canalObj = arrTempCanal.add<JsonObject>();
         JsonArray arrTemp = canalObj["Temp"].to<JsonArray>();
         int16_t idx = IdxStock5mn;
         for (int16_t i = 0; i < 300; i++)
         {
             arrTemp.add(tabTemp_5mn[canal][idx]);
             idx = (idx + 1) % 300;
         }
     }

       //Les Ouvertures des actions actives
    JsonObject valOuvre = doc["lesOuvertures"].to<JsonObject>();
    JsonArray arrOuvreBloc = valOuvre["BlocOuvre"].to<JsonArray>();
    valOuvre["NbActions"] = NbActions;
     for (int8_t bloc = 0; bloc < NbActions; bloc++)
     {
         JsonObject blocObj = arrOuvreBloc.add<JsonObject>();
         blocObj["nomAction"] = BlocsActions[bloc].Nom;
         JsonArray arrOuvre = blocObj["Ouvre"].to<JsonArray>();
         int16_t idx = IdxStock5mn;
         for (int16_t i = 0; i < 300; i++)
         {
             arrOuvre.add(tabOuvre_5mn[bloc][idx]);
             idx = (idx + 1) % 300;
         }
     }


    String json;
    serializeJson(doc, json);
    return json;
}