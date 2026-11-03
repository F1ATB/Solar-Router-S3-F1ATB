let refTempIP = [];

function Init() {
    let S = "";
    for (let i = 0; i < 4; i++) {
        S += `<div class='form cadre'>
                <div class="ligne" >
                    <label for="srcTemp${i}">Canal :${i}</label>
                    <select id="srcTemp${i}" onchange="checkDisabled();" >
                        <option value="tempNo" selected>Pas de mesure</option>
                        <option value="tempInt">Capteur DS18B20 Interne</option>
                        <option value="tempExt">Capteur DS18B20 ESP Externe</option>
                        <option value="tempMqtt">MQTT</option>
                    </select>
                </div>
            </div>
            <div class='form ligneTemperature' id='ligneTemperature${i}' >
                <div class='ligne' id='ligneNomTemp${i}'>
                    <label for='nomTemperature${i}' >Nom Température : </label>
                    <input type='text' name='nomTemperature${i}' id='nomTemperature${i}' >
                </div>
                <div class='ligne' id='ligneoffsetTemp${i}'>
                    <label for='offsetTemp${i}' >Offset mesure température (°C) :</label>
                    <input type='number' name='offsetTemp${i}' id='offsetTemp${i}' >
                </div>
                <div class='ligne' id='ligneTopicT${i}'>
                    <label for='TopicT${i}' >MQTT Topic Température :</label>
                    <input type='text' name='TopicT${i}' id='TopicT${i}' >
                </div>
                <div class='ligne' id='ligneIPtemp${i}'>
                    <label for='refTempIP${i}' >Ref. ESP-RMS externe :</label>
                    <select  id='refTempIP${i}' onchange="checkDisabled();" ></select>
                </div>
                <div class='ligne' id='canalEXTtemp${i}'>
                    <label for='canalEXT${i}' >Canal Température ESP-RMS externe :</label>
                    <select id='canalEXT${i}' onchange="C.LesTemperatures[${i}].canalTempExterne=this.value;GID('nomTemperature${i}').value = this.options[this.selectedIndex].text;" ></select>
                </div>
            </div>`;

    }
    GH("Sources_Temp", S);
    S = "<div class='ligne'><div class='nomR'><strong>Nom</strong></div><div class='nomR'><strong>Adresse IP</strong></div></div>";
    S += "<div class='ligne' id='Routeur_local'>";
    S += "<div id='RMS_Nom_local'  class='nomR'></div><input type='text' id='RMS_IP_local' readOnly>";
    S += "</div>";
    for (i = 0; i < 8; i++) {
        S += "<div class='ligne' id='Routeur_" + i + "' style='display:none;' >";
        S += "<div id='RMS_Nom" + i + "'  class='nomR'></div><input type='text' id='RMS_IP" + i + "' >";
        S += "</div>";
    }
    GH("Routeurs", S);
    LoadConfig();

}
function ImplantationConfig() {
    //Cookie clé d'accès

    let name = "CleAcces=";
    let decodedCookie = decodeURIComponent(document.cookie);
    let ca = decodedCookie.split(';');
    for (let i = 0; i < ca.length; i++) {
        let c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) GID("CleAcces").value = c.substring(name.length, c.length);
    }
    GH("nom_R", C.nomRouteur);
    GID("dhcp").checked = C.dhcpOn == 1 ? true : false;
    GID("adrIP").value = int2ip(C.localIP);
    GID("RMS_IP_local").value = int2ip(C.localIP);
    GID("gateway").value = int2ip(C.Gateway);
    GID("masque").value = int2ip(C.masque);
    GID("dns").value = int2ip(C.dns);
    GID("ModeP" + C.ModePara).checked = true;
    GID("ModeW").value = C.ModeReseau;
    GID("MQTTRepete").value = C.MQTTRepet;
    GID("MQTTIP").value = int2ip(C.MQTTIP);
    GID("MQTTPort").value = C.MQTTPort;
    GID("MQTTUser").value = C.MQTTUser;
    GID("MQTTpwd").value = C.MQTTPwd;
    GID("MQTTPrefix").value = C.MQTTPrefix;
    GID("MQTTPrefixEtat").value = C.MQTTPrefixEtat
    GID("MQTTdeviceName").value = C.MQTTdeviceName
    GID("subMQTT").checked = C.subMQTT == 1 ? true : false;
    GID("nomRouteur").value = C.nomRouteur;
    GH("RMS_Nom_local", C.nomRouteur);
    GID("CalibU").value = C.CalibU;
    GID("CalibI").value = C.CalibI;
    GID("TempoRTEon").checked = C.TempoRTEon == 1 ? true : false;
    GID("WifiSleep").checked = C.WifiSleep == 1 ? true : false;
    GID("Horlo").value = C.Horloge;
    GID("ComSurv").value = C.ComSurv;
    GID("Serial1").value = C.pSerial1;
    GID("Serial2").value = C.pSerial2;
    GID("pTriac").value = C.pTriac;
    GID("ESP").value = C.ESP32_Type;
    GID("pLED").value = C.LEDgroupe;
    GID("Rot").value = C.rotation;
    GID("DurEcran").value = C.DurEcran;
    GID("pUxI").value = C.pUxI;
    GID("pTemp").value = C.pTemp;
    GID("Horlo").value = C.Horloge;
    GID("hostname").value = C.hostname;
    GID("latitude").value = C.latitude;
    GID("longitude").value = C.longitude;
    GID("EstimCACSI").checked = C.EstimCACSI == 1 ? true : false;
    for (let c = 0; c < 4; c++) {
        GID("nomTemperature" + c).value = C.LesTemperatures[c].nomTemperature;
        GID("srcTemp" + c).value = C.LesTemperatures[c].Source_Temp;
        GID("TopicT" + c).value = C.LesTemperatures[c].TopicT;
        refTempIP[c] = C.LesTemperatures[c].refTempIP;
        GID("canalEXT" + c).value = C.LesTemperatures[c].canalTempExterne;
        GID("offsetTemp" + c).value = C.LesTemperatures[c].offsetTemp / 100;
    }
    //RMS Externes
    let d = 0;
    for (let c = 0; c < C.RouteurExt.length; c++) {
        if (C.RouteurExt[c].IP > 0) {
            GID("RMS_IP" + d).value = int2ip(C.RouteurExt[c].IP);
            GH("RMS_Nom" + d, C.RouteurExt[c].NomRouteur);
            GID("Routeur_" + d).style = "display: table-row";
            d++;
        }
    }
    if (d < 7) {
        GH("RMS_Nom" + d, "<small>Ajoutez une adresse IP de routeur <small>(et sauvegardez)</small> ----></small>");
        GID("Routeur_" + d).style = "display: table-row";
    }

    Liste_des_Sources();
    LoadParaTheme(); //Liste des fichiers de Themes
    SetHautBas();
    GH("nom_R", C.nomRouteur);

    checkDisabled();

}


function sendConfig() {
    GID("attente").style = "visibility: visible;";
    C.dhcpOn = GID("dhcp").checked ? 1 : 0;
    C.TempoRTEon = GID("TempoRTEon").checked ? 1 : 0;
    C.ComSurv = GID("ComSurv").value;
    C.subMQTT = GID("subMQTT").checked ? 1 : 0;
    C.WifiSleep = GID("WifiSleep").checked ? 1 : 0;
    C.hostname = GID("hostname").value;
    C.Horloge = GID("Horlo").value;
    C.pSerial1 = GID("Serial1").value;
    C.pSerial2 = GID("Serial2").value;
    C.pTriac = GID("pTriac").value;
    C.ModePara = document.querySelector('input[name="ModeP"]:checked').value;
    C.ModeReseau = GID("ModeW").value;
    C.ESP32_Type = GID("ESP").value;
    C.LEDgroupe = GID("pLED").value;
    C.rotation = GID("Rot").value;
    C.DurEcran = GID("DurEcran").value;
    C.pUxI = GID("pUxI").value;
    C.pTemp = GID("pTemp").value;
    C.EstimCACSI = GID("EstimCACSI").checked ? 1 : 0;

    if (C.ModePara == 0) { //Non Expert
        C.subMQTT = 0; C.WifiSleep = 1;
    }
    C.localIP = ip2int(GID("adrIP").value);
    C.Gateway = ip2int(GID("gateway").value);

    C.masque = ip2int(GID("masque").value);
    C.dns = ip2int(GID("dns").value);



    C.MQTTRepet = GID("MQTTRepete").value;
    C.MQTTIP = ip2int(GID("MQTTIP").value);
    C.MQTTPort = GID("MQTTPort").value;
    C.MQTTUser = GID("MQTTUser").value.trim();
    C.MQTTPwd = GID("MQTTpwd").value.trim();
    C.MQTTPrefix = GID("MQTTPrefix").value.trim();
    C.MQTTPrefixEtat = GID("MQTTPrefixEtat").value.trim();
    C.MQTTdeviceName = GID("MQTTdeviceName").value.trim();
    C.nomRouteur = GID("nomRouteur").value.trim();
    C.Theme = GID("Theme").value.trim();
    C.latitude = parseFloat(GID("latitude").value);
    C.longitude = parseFloat(GID("longitude").value);
    C.Horloge = GID("Horlo").value;
    C.CalibU = GID("CalibU").value;
    C.CalibI = GID("CalibI").value;

    for (let c = 0; c < 4; c++) {
        let QS = GID("srcTemp" + c).value;
        C.LesTemperatures[c].Source_Temp = (C.ModePara == 0) ? "tempNo" : QS;
        C.LesTemperatures[c].nomTemperature = GID("nomTemperature" + c).value.trim();
        C.LesTemperatures[c].TopicT = GID("TopicT" + c).value.trim()
        C.LesTemperatures[c].refTempIP = GID("refTempIP" + c).value;
        C.LesTemperatures[c].canalTempExterne = GID("canalEXT" + c).value;
        C.LesTemperatures[c].offsetTemp = GID("offsetTemp" + c).value * 100;
    }
    C.RouteurExt = []
    for (let i = 0; i < 8; i++) {
        let Vip = (C.ModePara == 0) ? 0 : ip2int(GID("RMS_IP" + i).value.trim());
        C.RouteurExt.push({ IP: Vip }); // On ne recupere que l'IP
    }

    j = 0;
    if (GID("TypeSource0").value.trim() == "") {
        GID("TypeSource0").value = "NotDef"; //On valide au minimum
        GID("CanalNom00").value = "Source Simulée 0";
        GID("CanalVoie00").value = 0;
        GID("CanalVoie01").value = -1;
        GID("CanalVoie02").value = -1;
    }
    for (let i = 0; i < 4; i++) { //4 sources maximum
        C.LaSource[i].Type = "";
        for (let k = 0; k < C.LaSource[i].CanalNom.length; k++) { // On efface anciennes valeurs
            C.LaSource[i].CanalNom[k] = "";
            C.LaSource[i].CanalVoie[k] = -1;
            C.LaSource[i].NomSoutir[k] = "Soutirée";
            C.LaSource[i].NomInject[k] = "Injectée";
        }
        let typeSource = GID("TypeSource" + i).value.trim();
        if (typeSource != "") { //On valide les sources ayant un type 
            if (typeSource != "Ext") {
                if (GID("CanalNom" + i + "0").value.trim() != "") { //On valide les sources interne avec un nom canal 0
                    C.LaSource[j].Type = typeSource;
                    C.LaSource[j].IP = ip2int(GID("SourceRMS_IP" + i).value);
                    C.LaSource[j].Port = GID("SourcePort" + i).value;
                    for (let k = 0; k < C.LaSource[j].CanalNom.length; k++) {
                        let ik = i.toString() + k.toString();
                        C.LaSource[j].CanalNom[k] = GID("CanalNom" + ik).value;
                        C.LaSource[j].CanalVoie[k] = GID("CanalVoie" + ik).value;
                        if (C.LaSource[j].CanalNom[k] != "" && C.LaSource[j].CanalVoie[k] < 0) C.LaSource[j].CanalVoie[k] = 0;//On valide
                        if (k == 1 && C.LaSource[j].Type != "Ext" && C.LaSource[j].Type != "UxIx2" && C.LaSource[j].Type != "ShellyEm" && C.LaSource[j].Type != "UxIx3") C.LaSource[j].CanalVoie[k] = -1;// on invalde le deuxième canal
                        if (k == 2 && C.LaSource[j].Type != "Ext" && C.LaSource[j].Type != "UxIx3") C.LaSource[j].CanalVoie[k] = -1;// on invalde le troisième canal
                        C.LaSource[j].NomSoutir[k] = GID("NomSoutir" + ik).value.trim();
                        C.LaSource[j].NomInject[k] = GID("NomInject" + ik).value.trim();
                    }

                    j++;
                } // Fin de la validation des sources interne ayant  un nom canal 0
            } else {// Fin de la validation des sources internes. Puis source externes
                if (GID("CanalNomExt" + i)) { //Dans certain cas peut ne pas exister
                    if (GID("CanalNomExt" + i).value.trim() != "") { //On valide les sources Externe avec un nom canal 0
                        C.LaSource[j].Type = typeSource;
                        C.LaSource[j].IP = GID("SourceExtIP" + i).value;
                        C.LaSource[j].Port = 80;
                        for (let k = 0; k < C.LaSource[j].CanalNom.length; k++) {
                            let ik = i.toString() + k.toString();
                            C.LaSource[j].CanalNom[k] = "";
                            C.LaSource[j].CanalVoie[k] = -1;
                        }
                        let select = GID("CanalNomExt" + i);
                        C.LaSource[j].CanalNom[0] = select.options[select.selectedIndex].text;
                        C.LaSource[j].CanalVoie[0] = select.value;

                        C.LaSource[j].NomSoutir[0] = GID("NomSoutirExt" + i).value.trim();
                        C.LaSource[j].NomInject[0] = GID("NomInjectExt" + i).value.trim();


                        j++;
                    } // Fin de la validation des sources externes ayant  un nom canal 0
                }
            }
        }


    }


    document.cookie = "CleAcces=" + GID("CleAcces").value.trim();
    if ((GID("dhcp").checked || checkIP("adrIP") && checkIP("gateway")) && (!GID("MQTTRepete").checked || checkIP("MQTTIP"))) {
        let adr = adrHttpRemote + "/ajaxNewConfig";

        fetch(adr, {
            method: "POST",                     // méthode HTTP
            headers: {
                "Content-Type": "application/json" // indique qu'on envoie du JSON
            },

            body: JSON.stringify(C)          // conversion de l'objet JS en texte JSON

        })
            .then(response => {
                if (!response.ok) throw new Error("Erreur HTTP " + response.status);
                return response.json();             // transforme la réponse JSON en objet JS
            })
            .then(resultat => {
                console.log("Réponse du serveur :", resultat);
                location.reload();
            })
            .catch(error => {
                console.error("Erreur d'envoi :", error);
            });
    }
}

function LoadParaTheme() {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            const T = JSON.parse(this.responseText);
            let D = `<option value="default" selected>Défaut</option>`;
            let S = "";
            if (T && T.FichiersCSS) {
                for (let i = 0; i < T.FichiersCSS.length; i++) {
                    if (T.FichiersCSS[i] == C.Theme) {
                        S += `<option value="${T.FichiersCSS[i]}" selected>${T.FichiersCSS[i]}</option>`;
                        D = `<option value="default" >Défaut</option>`;
                    } else {
                        S += `<option value="${T.FichiersCSS[i]}">${T.FichiersCSS[i]}</option>`;
                    }
                }
            }
            GID("Theme").innerHTML = D + S;
        }
    };
    let adr = adrHttpRemote + "/ParaThemeAjax";

    xhttp.open('GET', adr, true);
    xhttp.send();
}
function SendRAZ() {
    if (confirm("Confirmez la remise à zéro des historiques")) {
        let xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                let retour = this.responseText;
            }
        };
        let adr = adrHttpRemote + "/ajaxRAZhisto";
        xhttp.open('GET', adr, true);
        xhttp.send();
    }
}
function checkDisabled() {
    C.ModeReseau = GID("ModeW").value;
    C.ModePara = document.querySelector('input[name="ModeP"]:checked').value;
    let pESP = GID("ESP").value;
    GID("Bwifi").style.display = (C.ESP32_Type < 10) ? "inline-block" : "none";
    GID("infoIP").style.display = (GID("dhcp").checked || C.ModeReseau == 2) ? "none" : "table";
    GID("dhcp").style.visibility = (C.ModeReseau == 2) ? "hidden" : "visible";
    GID("ipreset").style.display = (C.ModeReseau == 2) ? "none" : "inherit";
    GID("rotation").style.display = (pESP == 2) ? "table-row" : "none";
    GID("dureeOn").style.display = (pESP == 2) ? "table-row" : "none";
    GID("l_wifi_0").style.display = (C.ModeReseau == 2) ? "none" : "table-row";
    GID("l_wifi_1").style.display = (C.ModeReseau == 2 || C.ModePara == 0) ? "none" : "table-row";
    GID("l_wifi_2").style.display = (C.ModeReseau == 2 || C.ModePara == 0 || C.ESP32_Type >= 10) ? "none" : "table-row";
    GID("listerouteurs").style.display = (C.ModeReseau == 2 || C.ModePara == 0) ? "none" : "block";



    for (let i = 0; i < 4; i++) {
        let QS = '#srcTemp' + i + ' option[value="tempExt"]';
        document.querySelector(QS).disabled = (C.ModeReseau == 2) ? true : false;
        QS = '#srcTemp' + i + ' option[value="tempMqtt"]';
        document.querySelector(QS).disabled = (C.ModeReseau == 2) ? true : false;
    }
    GID("donEnv").style.display = (C.ModeReseau == 2) ? "none" : "inline-block;";
    if (C.ModeReseau == 2) {
        GH("localIP", " : 192.168.4.1");
    } else {
        GH("localIP", "auto (DHCP) : " + int2ip(C.localIP));
    }


    GID("Zmqtt").style.display = ((GID("MQTTRepete").value != 0 || GID("subMQTT").checked) && C.ModePara > 0) ? "block" : "none";
    GID("LesSourcesTemp").style.display = (C.ModePara == 0) ? "none" : "block";
    GID("p_Temp").style.display = (C.ModePara == 0) ? "none" : "table-row";
    GID("Hostname").style.display = (C.ModePara == 0) ? "none" : "table";
    GID("SurvCom").style.display = (C.ModePara == 0) ? "none" : "block";
    GID("Localisation").style.display = (C.ModePara == 0) ? "none" : "block";
    GID("SourceExt").style.display = (C.ModePara == 0) ? "none" : "block";
    //Les températures
    for (let c = 0; c < 4; c++) {
        GID('ligneTemperature' + c).style.display = (GID("srcTemp" + c).value == "tempNo") ? "none" : "table";
        GID('ligneNomTemp' + c).style.display = (GID("srcTemp" + c).value == "tempExt") ? "none" : "table-row";
        GID('ligneTopicT' + c).style.display = (GID("srcTemp" + c).value == "tempMqtt") ? "table-row" : "none";
        GID('ligneIPtemp' + c).style.display = (GID("srcTemp" + c).value == "tempExt") ? "table-row" : "none";
        GID('canalEXTtemp' + c).style.display = (GID("srcTemp" + c).value == "tempExt") ? "table-row" : "none";
        GID('ligneoffsetTemp' + c).style.display = (GID("srcTemp" + c).value == "tempInt") ? "table-row" : "none";
        if (GID("srcTemp" + c).value == "tempMqtt") GID("Zmqtt").style.display = "table";

        if (GID("srcTemp" + c).value == "tempExt") {
            let IPext = GID("refTempIP" + c).value;
            let idxRMS = -1;

            for (let i = 0; i < C.RouteurExt.length; i++) {
                if (C.RouteurExt[i].IP == IPext) {
                    idxRMS = i;
                    break;
                }
            }
            if (idxRMS >= 0) {
                let Options = "";
                for (let j = 0; j < C.RouteurExt[idxRMS].TemperNom.length; j++) {
                    Options += "<option value='" + j + "'> " + C.RouteurExt[idxRMS].TemperNom[j] + "</option>";
                }
                GID("canalEXT" + c).innerHTML = Options;
                GID("canalEXT" + c).value = C.LesTemperatures[c].canalTempExterne;
                GID("nomTemperature" + c).value = C.RouteurExt[idxRMS].TemperNom[GID("canalEXT" + c).value];

            }
        }
    }
    GID("Horlo").options[0].disabled = (C.ModeReseau > 0) ? true : false;
    GID("Horlo").options[5].disabled = (C.LaSource[0].Type == 'Ext' && C.LaSource[0].IP != "") ? false : true;
    if (C.ModeReseau > 0 && C.Horloge == 0) { GID("Horlo").value = 2; C.Horloge = 2; }
    
    GID("SourcePlus").style.display = "none";
    GID("LabelVoie").style.visibility = "hidden";
    GID('Zcalib').style.display = "none";
    GID('Analog').style.display = "none";
    for (let i = 0; i < 4; i++) {
        C.LaSource[i].Type = GID("TypeSource" + i).value;
        let S = C.LaSource[i].Type;
        GID("LaSource" + i).style.display = (S != "") ? "table-row" : "none";
        if (S == "") GID("SourcePlus").style.display = "block";
        GID("SourceRMS_IP" + i).style.display = (S == "Enphase" || S == "SmartG" || S == "HomeW" || S == "ShellyEm" || S == "ShellyPro") ? "block" : "none";
        GID("SourcePort" + i).style.display = (S == "Linky" || S == "UxIx2" || S == "UxIx3" || S == "PZEM004T") ? "block" : "none";
        GID("SourceExtIP" + i).style.display = (S == "Ext") ? "block" : "none";
        for (let j = 0; j < C.LaSource[i].CanalNom.length; j++) {
            let ij = i.toString() + j.toString();
            GID("CanalVoie" + ij).style.display = (S == "ShellyEm" || S == "ShellyPro" || (S == "UxIx3" && j == 0)) ? "table-cell" : "none";
            let voieUxIx3mono = (S == "UxIx3" && GID("CanalVoie" + i + "0").value == 0);
            GID("S_Canal" + ij).style.display = (j == 0 || (j == 1 && (S == "ShellyEm" || voieUxIx3mono || S == "UxIx2")) || (j == 2 && voieUxIx3mono)) ? "table-row" : "none";
            if (S == "UxIx3" && j == 0) {
                if (voieUxIx3mono) { // trois Monophasé
                    GID("CanalVoie" + i + "1").value = 1;
                    GID("CanalVoie" + i + "2").value = 2;
                } else { //Triphasé
                    GID("CanalVoie" + i + "1").value = -1;
                    GID("CanalVoie" + i + "2").value = -1;
                }
            }
        }
        let IPext = GID("SourceExtIP" + i).value; // Récupère l'adresse IP externe sélectionnée 
        let idx = C.RouteurExt.findIndex(r => r.IP == IPext);
        if (idx >= 0) {
            let NomsCanauxExt = `<div class='ligne'>
                                <select class='TC' id='CanalNomExt${i}'>`;
            for (let j = 0; j < C.RouteurExt[idx].CanalNom.length; j++) {
                if (C.RouteurExt[idx].CanalNom[j] != "") NomsCanauxExt += "<option value='" + j + "'>" + C.RouteurExt[idx].CanalNom[j] + "</option>";
            }
            NomsCanauxExt += "</select>";
            NomsCanauxExt += `<div class='TC' style='width:40%;'>
                                    <div><input type='text'  id='NomSoutirExt${i}' title='Nom Puissance soutirée' value='${C.LaSource[i].NomSoutir[0]}'></div>
                                    <div><input type='text'  id='NomInjectExt${i}' title='Nom Puissance injectée' value='${C.LaSource[i].NomInject[0]}'></div>
                               </div>
                           </div>`;
            GID("S_CanalExt" + i).innerHTML = NomsCanauxExt;
            GID("CanalNomExt" + i).value = C.LaSource[i].CanalVoie[0]; // La voie donne l'indice du nom du canal externe
        }
        GID("S_CanalExt" + i).style.display = (S == "Ext") ? "table" : "none";
        GID("S_CanalInt" + i).style.display = (S == "Ext") ? "none" : "table";



        GH("S_complement" + i, "");
        if (S == "Pmqtt") {
            let T = `<div class='ligne'><label for='TopicP_${i}' >MQTT Topic Puissance :</label>
            <input type='text' name='TopicP_${i}' id='TopicP_${i}' value='${C.LaSource[i].TopicP}' onchange='C.LaSource[${i}].TopicP=this.value.trim();'></div>`;
            GH("S_complement" + i, T);
        }
        if (S == "Enphase") {
            let T = "<div class='ligne' >";
            T += "<label for='EnphaseUser' class='fsize12'>Enphase Envoy-S metered User : </label>";
            T += "<input type='text' name='EnphaseUser' id='EnphaseUser' value='" + C.EnphaseUser + "' onchange='C.EnphaseUser=this.value.trim();'  >";
            T += "</div>";
            T += "<div class='ligne' >";
            T += "<label for='EnphasePwd' class='fsize12'>Enphase Envoy-S metered Password : </label>";
            T += "<input type='password' name='EnphasePwd' id='EnphasePwd' value='" + C.EnphasePwd + "' onchange='C.EnphasePwd=this.value.trim()' ><div class='TCc' onclick='visupwd(this.id,\"EnphasePwd\");' id='iconPwd'>👁️</div>";
            T += "</div>"
            T += "<div class='ligne' >";
            T += "<label for='EnphaseSerial' class='fsize12'>Numéro série passerelle IQ Enphase : </label>";
            T += "<input type='text' name='EnphaseSerial' id='EnphaseSerial' value='" + C.EnphaseSerial + "' onchange='C.EnphaseSerial=this.value.trim();' >";
            T += "</div>";
            GH("S_complement" + i, T);
        }
        if (S == 'ShellyEm' || S == 'ShellyPro') {
            let T = "<div class='shem'><strong>Shelly (Pro) Em</strong><br>";
            T += "Monophasé : Courant maison mettre voie = 0,1 ou 2<br>Triphasé : mettre voie 3 ";
            T += "</div><div class='shem'><Strong>Shelly Em Gen3</strong><br>";
            T += "Courant maison sur voie 0 = 30, voie 1 = 31</div>";
            T += "<div class='shem'><Strong>Non utilisé</strong><br>";
            T += "Mettre voie  = -1</div>";
            GH("S_complement" + i, T);
            GID("LabelVoie").style.visibility = "visible";
        }
        if (S == 'UxI') {
            GID('Zcalib').style.display = "table";
            GID('Analog').style.display = "table-row";
        }
        if (S == 'UxIx3') {
            let T = "<div class='shem'><strong>UxIx3 (Triphasé ou 3 Monophasés)</strong><br>";
            T += "3 Monophasés :  voie = 0<br>Triphasé : voie = 3 ";
            T += "</div>";
            GH("S_complement" + i, T);
            GID("LabelVoie").style.visibility = "visible";
        }


    }




}
function visupwd(id, pwdId) {
    if (GID(pwdId).type === "password") {
        GID(pwdId).type = "text";
        GID(id).textContent = "🙈";
    } else {
        GID(pwdId).type = "password";
        GID(id).textContent = "👁️";
    }
}
function AdaptationSource() { };
function checkIP(id) {
    let S = GID(id).value;
    let Table = S.split(".");
    let valide = true;
    if (Table.length != 4) {
        valide = false;
    } else {
        for (let i = 0; i < Table.length; i++) {
            if (Table[i] > 255 || Table[i] < 0) valide = false;
        }
    }
    if (valide) {
        GID(id).style.color = "black";
    } else {
        GID(id).style.color = "red";
    }
    return valide;
}


function Reset() {
    GID("attente").style = "visibility: visible;";
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            GID('BoutonsBas').innerHTML = this.responseText;
            GID("attente").style = "visibility: hidden;";
            setTimeout(location.reload(), 1000);
            setTimeout(location.reload(), 10000);
        }
    };
    xhttp.open('GET', 'restart', true);
    xhttp.send();
}
function Liste_des_Sources() {
    let option = "<option value='NotDef'>Source non définie</option>";
    option += "<option value='UxI'>UxI</option>";
    option += "<option value='UxIx2'>UxIx2</option>";
    option += "<option value='UxIx3'>UxIx3</option>";
    option += "<option value='Linky'>Linky</option>";
    option += "<option value='Enphase'>Enphase-Envoy</option>";
    option += "<option value='SmartG'>SmartGateways</option>";
    option += "<option value='HomeW'>HomeWizard</option>";
    option += "<option value='ShellyEm'>Shelly Em</option>";
    option += "<option value='ShellyPro'>Shelly Pro Em</option>";
    option += "<option value='PZEM004T'>PZEM-004T</option>";
    option += "<option value='Ext' id='SourceExt'>ESP Externe</option>";
    option += "<option value='Pmqtt'>MQTT</option>";
    let optionPort = "<option value='1'>Port Série 1</option>";
    optionPort += "<option value='2'>Port Série 2</option>";

    let optionExt = "";
    for (let c = 0; c < C.RouteurExt.length; c++) {
        if (C.RouteurExt[c].IP > 0) {
            optionExt += "<option value='" + C.RouteurExt[c].IP + "' title='" + int2ip(C.RouteurExt[c].IP) + "'>" + C.RouteurExt[c].NomRouteur + "</option>";
        }
    }

    let S = `<div class='ligne'><div class='titreS'>Type</div><div class='titreS'>Ref. RMS <small>ou</small> Port Série</div><div class='titreS'><div class='ligneIB33'>Nom Puissance </div><div class='ligneIB33' id='LabelVoie'> Voie</div><div class='ligneIB33' > Sout./Injec.</div></div></div>`;
    for (let i = 0; i < 4; i++) {
        let a = 0.1 * (i % 2);
        S += `<div class='ligne2' id='LaSource${i}' style='background-color: rgba(128,128,128,${a});'>
                <select id='TypeSource${i}' onchange='checkDisabled();' >${option}</select>
                <div class='TC'>
                    <input type='text'  id='SourceRMS_IP${i}'  >
                    <select id='SourcePort${i}'>${optionPort}</select>
                    <select id='SourceExtIP${i}' onchange='checkDisabled();'>${optionExt}</select>
                </div>
                <div class='TC'>
                    <div  class='canalT' id='S_CanalInt${i}'>`;
        for (let j = 0; j < C.LaSource[i].CanalNom.length; j++) {
            let ij = i.toString() + j.toString();
            S += `<div  class='ligne' id = 'S_Canal${ij}'>
                                <input type='text'   style='width:40%' id='CanalNom${ij}' title='Nom de la source de puissance'>
                                <input type='number' title='Numéro de voie' id='CanalVoie${ij}' style='width:20%' onchange='checkDisabled();'>
                                <div class='TC' style='width:40%;'>
                                    <div ><input type='text'  id='NomSoutir${ij}' title='Nom Puissance soutirée'></div>
                                    <div><input type='text'  id='NomInject${ij}' title='Nom Puissance injectée'></div>
                                </div>
                              </div>`;
        }
        S += `</div>
                    <div id='S_complement${i}' class='canalT'></div>
                    <div id='S_CanalExt${i}' class='canalT'></div>
                </div>
             </div>`;
    }
    S += `<div id='SourcePlus' onclick='AddSource();' class='dispNone emoji' >➕️</div>`;
    GH("liste_sources", S);
    for (let i = 0; i < 4; i++) {
        GID("TypeSource" + i).value = C.LaSource[i].Type;
        GID("SourceRMS_IP" + i).value = int2ip(C.LaSource[i].IP);
        GID("SourcePort" + i).value = C.LaSource[i].Port;
        GID("SourceExtIP" + i).value = C.LaSource[i].IP;

        for (let j = 0; j < C.LaSource[i].CanalNom.length; j++) {
            let ij = i.toString() + j.toString();
            GID("CanalNom" + ij).value = C.LaSource[i].CanalNom[j];
            GID("CanalVoie" + ij).value = C.LaSource[i].CanalVoie[j];
            GID("NomSoutir" + ij).value = C.LaSource[i].NomSoutir[j];
            GID("NomInject" + ij).value = C.LaSource[i].NomInject[j];
        }
    }

    //Sources temperatures exterieurs
    for (let i = 0; i < 4; i++) {
        GH("refTempIP" + i, optionExt);
        GID("refTempIP" + i).value = refTempIP[i];
    }


}
function AddSource() {
    for (let i = 0; i < 4; i++) {
        if (C.LaSource[i].Type == "") {
            C.LaSource[i].Type = "NotDef";
            GID("TypeSource" + i).value = C.LaSource[i].Type;
            i = 4;
        }
    }
    checkDisabled();
}

