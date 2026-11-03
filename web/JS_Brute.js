
function ImplantationConfig() {
    SetHautBas();
    GH("nom_R", C.nomRouteur);
    setTimeout(LoadAppelDonneesImmediat, 100); // Ensure data are visible  after initial call
}





function LaDate(d) {
    return d.substr(0, 1) + ' ' + d.substr(5, 2) + '/' + d.substr(3, 2) + '/' + d.substr(1, 2) + ' ' + d.substr(7, 2) + 'h ' + d.substr(9, 2) + 'mn ' + d.substr(11, 2) + 's';
}
function LaVal(d) {
    d = parseInt(d);
    d = '           ' + d.toString();
    return d.substr(-9, 3) + ' ' + d.substr(-6, 3) + ' ' + d.substr(-3, 3);
}


//=============== Websocket =======================
let LePort = window.location.port;
if (LePort == "") LePort = "80";
let wsBrute;
if (location.hostname == "localhost") {
    wsBrute = new WebSocket("ws://" + RemoteIP + "/wsBrute");
} else {
    wsBrute = new WebSocket("ws://" + window.location.hostname + ":" + LePort + "/wsBrute");
}
wsBrute.onmessage = (event) => {

    let msg = JSON.parse(event.data);
    msg.HeureCourante = msg.HeureCourante.replace("T", " ");
    GH("date", msg.HeureCourante);
    switch (msg.MessageType) {
        case "BruteMessages":
            SortieMessagesEvenements(msg);
            break;
        case "BruteSources":
            SortieBruteSources(msg);
            break;
        case "BruteESP32":
            SortieBruteESP32(msg);
            break;

        default:
            console.warn("Unknown message", msg);
    }
};

function SortieMessagesEvenements(msg) {
    let S = "<table class='w100'>";
    let L = 0;
    for (let i = 0; i < msg.Message.length; i++) {
        if (msg.Message[i] != '') S += `<tr class="ligne${L++ % 2}"><td>` + msg.Message[i] + '</td></tr>';
    }
    S += '</table>';
    GH('BruteMessages', S);
}
let TabUxI = null;
function SortieBruteSources(msg) {
    let S = "";
    msg.DataBrute = msg.DataBrute.map(texte =>
        texte
            .replaceAll("\x1C", "<br>")
            .replaceAll("\x1D", "<br>")
            .replaceAll("\x1E", "<br>")
    );
    let UxI = false
    for (let i = 0; i < msg.numSource.length; i++) {
        if (msg.Type[i] == "UxI") UxI = true;


        S += "<div class='w100'>";
        S += `<div class='tableau w100'><table class='w100'>`;
        S += `<tr class="ligne0"><td>Source </td><td class='ri'>Ref : ` + msg.numSource[i] + `</td>`;
        S += `<td class='ri'>Type : ` + msg.Type[i] + `</td>`;
        S += `<td class='ri'>Data : ` + msg.Data[i] + `</td></tr>`;
        if (msg.Type[i] == "UxI") {

            TabUxI = msg.DataBrute[i].split("|");  // Tension, courant, tableau Tension, Tableau Courant
            if (TabUxI.length == 3) {
                if (TabUxI[1].length > 10 && TabUxI[2].length > 10) UxI = true;
            };
            let M = TabUxI[0].split(":");
            let valeurs = M[1].split(",");
            S += `<tr><td  >` + valeurs[0] + `</td><td  class='ri'>` + valeurs[1] + `</td><td  class='ri'>` + valeurs[2] + `</td><td></td></tr>`;
            S += `<tr><td colspan='4' ><svg id='GraphUxI_SVG' viewBox='0 0 510 200' class='brut-graphSVG'></svg></td></tr>`;
        } else if (msg.Type[i].indexOf("Shelly") != -1) {
                let Separ = msg.DataBrute[i].split("|*|");
                let JsonObjet = JSON.parse(Separ[1]);
                S += `<tr><td colspan='4' class='fsize8'><br>` + Separ[0] +`<br><div class='json-viewer'>` + JSON.stringify(JsonObjet, null, 2) + `</div></td></tr>`;

        } else {
            S += `<tr><td colspan='4' class='fsize8'><br>` + msg.DataBrute[i] + `</td></tr>`;
        }
        S += `</table></div>`;
        S += "</div><br>";
    }
    GH('BruteSources', S);
    if (UxI) TraceGraphUxI();
}
function SortieBruteESP32(D) {
    let S = '<table class="w100">';
    let H = parseInt(D.T_On);
    H = H + (parseFloat(D.T_On) - H) * 0.6;
    H = H.toFixed(2);
    H = H.replace(".", "h ") + "mn";
    let LaSource = C.LaSource[0].Type;
    if (LaSource == 'Ext') LaSource = "Externe (" + C.LaSource[0].Data + ")<br>" + int2ip(C.LaSource[0].IP);
    let typeESP32 = ["Non défini", "Wroom seul", "Ecran 480*320", "", "", "", "", "", "", "", "T-ETH-Lite"];
    let L = 0;
    S += `<tr class="ligne${L++ % 2}"><td>ESP32-S3 On depuis :</td><td>` + H + `</td></tr>`;
    S += `<tr class="ligne${L++ % 2}"><td>ESP32-S3 modèle :</td><td>` + typeESP32[D.ESP32_Type] + `</td></tr>`;
    S += `<tr class="ligne${L++ % 2}"><td>Source des mesures :</td><td>` + LaSource + `</td></tr>`;

    if (C.ModeReseau < 2) {
        if (D.ESP32_Type < 10) { //WIFI
            S += `<tr class="ligne${L++ % 2}"><td>Niveau WiFi :</td><td>` + D.RSSI + ' dBm</td></tr>';
            S += `<tr class="ligne${L++ % 2}"><td>Point d'acc&egrave;s WiFi :</td><td>` + D.BSSID + '</td></tr>';
            S += `<tr class="ligne${L++ % 2}"><td>R&eacute;seau WiFi :</td><td>` + D.ssid + '</td></tr>';
        }
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IP .local :</td><td>` + D.hostname + '.local</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse MAC ESP32 :</td><td>` + D.Mac + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IPv4 ESP32 :</td><td>` + D.IPv4 + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IPv6 ESP32 (Global) :</td><td>` + D.IPv6global + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IPv6 ESP32 (Local) :</td><td>` + D.IPv6local + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse passerelle :</td><td>` + D.gateway + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Masque du r&eacute;seau :</td><td>` + D.masque + '</td></tr>';
    } else {
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IP ESP32 (Point d'Accès) :</td><td>192.168.4.1</td></tr>`;
    }
    S += `<tr class="ligne${L++ % 2}"><td>Charge coeur 0 (Lecture Puissances) Min, Moy, Max :</td><td>` + D.coeur0 + ' ms</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Charge coeur 1 (Calcul + Serveur Web) Min, Moy, Max :</td><td>` + D.coeur1 + ' ms</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire RAM interne totale libre :</td><td>` + parseInt(D.RAMfree).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Plus gros bloc RAM interne libre :</td><td>` + parseInt(D.RAMbloc).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire PSRAM totale libre :</td><td>` + parseInt(D.PSRAMfree).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Plus gros bloc PSRAM libre :</td><td>` + parseInt(D.PSRAMbloc).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire stockage fichiers totale :</td><td>` + parseInt(D.FileTotal).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire stockage fichiers utilisée :</td><td>` + parseInt(D.FileUsed).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire stockage fichiers libre :</td><td>` + parseInt(D.FileTotal - D.FileUsed).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Nombre d'interruptions en 15ms du Gradateur (signal Zc) : Filtrés/Brutes :</td><td>` + D.IT_triac + '</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Synchronisation au Secteur ou asynchrone horloge ESP32</td><td>` + D.IT_Mode + '</td></tr>';
    let Stemp = D.Nbr_DS18B20;
    if (parseInt(D.Nbr_DS18B20) > 0) Stemp += '<span class="fsize10">' + D.AllTemp + '</span>';
    S += `<tr class="ligne${L++ % 2}"><td>Nombre de capteurs de température DS18B20 :</td><td>` + Stemp + '</td></tr>';


    if (D.MachineNom) {

        S += `<tr class="ligne${L++ % 2}"><td style="text-align:center;"><strong>Note échanges entre routeurs</strong></td><td></td></tr>`;
        for (let i = 0; i < D.MachineNom.length; i++) {
            S += `<tr class="ligne${L++ % 2}"><td>` + D.MachineNom[i] + '</td><td>' + D.MachineNote[i] + '</td></tr>';
        }
    }
    S += '</table>';

    GH('DataESP32', S);
}

//=============== Fin Websocket =======================

//======= Trace Graphe UxI ============
function TraceGraphUxI() {
    let Vmax = 1;
    let Imax = 1;
    let V = TabUxI[1].split(',');
    V.pop(); // Supprime le dernier élément vide si présent
    let I = TabUxI[2].split(',');
    I.pop(); // Supprime le dernier élément vide si présent
    for (let i = 0; i < V.length; i++) {
        V[i] = parseFloat(V[i]);
        I[i] = parseFloat(I[i]);
        if (Math.abs(V[i]) > Vmax) Vmax = Math.abs(V[i]);
        if (Math.abs(I[i]) > Imax) Imax = Math.abs(I[i]);
    }




    //Positionnement graphe en X
    let X0 = 15;

    let Ymin = 15;
    let Ymax = 195;
    let Y0 = 105; //Valeurs positives uniquement
    let dY = 9;

    const NS = "http://www.w3.org/2000/svg";
    const GraphSvg = GID("GraphUxI_SVG");

    GraphSvg.innerHTML = "";

    //Trace Axes
    let x_Max = 500;
    let NBmax = 100;
    let stepTick = 10;


    let axes = document.createElementNS(NS, "polyline");
    axes.setAttribute("points", `${X0},${Ymax} ${X0},${Ymin} ${X0},${Y0} ${x_Max},${Y0}`);
    axes.setAttribute("stroke", "var(--brute-axes)");
    axes.setAttribute("stroke-width", 1);
    axes.setAttribute("fill", "none");
    GraphSvg.appendChild(axes);




    for (let Nb = 0; Nb <= NBmax; Nb += stepTick) {
        let x, y, h;
        x = X0 + (x_Max - X0) * Nb / NBmax;
        y = Y0;
        h = Nb / 5;
        if (x > X0) {
            let tick = document.createElementNS(NS, "line");
            tick.setAttribute("x1", x);
            tick.setAttribute("y1", y);
            tick.setAttribute("x2", x);
            tick.setAttribute("y2", y + 4);
            tick.setAttribute("stroke", "var(--brute-axes)");
            tick.setAttribute("stroke-width", 1);
            GraphSvg.appendChild(tick);
            let txt = document.createElementNS(NS, "text");
            txt.setAttribute("x", x);
            txt.setAttribute("y", y + 9);
            txt.setAttribute("fill", "var(--brute-axes)");
            txt.textContent = h;
            txt.setAttribute("font-size", "5");
            GraphSvg.appendChild(txt);
        }

    }
    let txtMs = document.createElementNS(NS, "text");
    txtMs.setAttribute("x", 500);
    txtMs.setAttribute("y", 100);
    txtMs.setAttribute("fill", "var(--brute-axes)");
    txtMs.textContent = "ms";
    txtMs.setAttribute("font-size", "5");
    GraphSvg.appendChild(txtMs);


    // === Courbes ===

    let courbe = document.createElementNS(NS, "polyline");
    courbe.setAttribute("stroke", "var(--brute-V)");
    courbe.setAttribute("stroke-width", 1);
    courbe.setAttribute("fill", "none");
    let step = (x_Max - X0) / (V.length - 1);
    let courbePoints = "";
    for (let j = 0; j < V.length; j++) {
        const valeur = V[j] / Vmax;

        let x = X0 + j * step;
        let y = Y0 - valeur * (Y0 - Ymin);
        courbePoints += `${x},${y} `;

    }
    courbe.setAttribute("points", courbePoints);
    GraphSvg.appendChild(courbe);
    let txt = document.createElementNS(NS, "text");
    txt.setAttribute("x", 100);
    txt.setAttribute("y", 12);
    txt.setAttribute("fill", "var(--brute-V)");
    txt.textContent = "Volt";
    txt.setAttribute("font-size", "10");
    GraphSvg.appendChild(txt);

    let courbeI = document.createElementNS(NS, "polyline");
    courbeI.setAttribute("stroke", "var(--brute-I)");
    courbeI.setAttribute("stroke-width", 1);
    courbeI.setAttribute("fill", "none");
    let stepI = (x_Max - X0) / (I.length - 1);
    let courbePointsI = "";
    for (let j = 0; j < I.length; j++) {
        const valeur = I[j] / Imax;

        let x = X0 + j * stepI;
        let y = Y0 - valeur * (Y0 - Ymin);
        courbePointsI += `${x},${y} `;

    }
    courbeI.setAttribute("points", courbePointsI);
    GraphSvg.appendChild(courbeI);
    let txtI = document.createElementNS(NS, "text");
    txtI.setAttribute("x", 400);
    txtI.setAttribute("y", 12);
    txtI.setAttribute("fill", "var(--brute-I)");
    txtI.textContent = "Ampere";
    txtI.setAttribute("font-size", "10");
    GraphSvg.appendChild(txtI);

}