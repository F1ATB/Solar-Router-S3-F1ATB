let Puissance10mn = null;
let Puissance48h = null;
let Histo10mnPw = [];
let Histo10mnPva = [];
let Histo10mnTemp = [];
let Histo10mnOuverture = [];
let Histo48hPw = [];
let Histo48hPva = [];
let Histo48hTemp = [];
let Histo48hOuverture = [];
const PuissanceMaximum = 32000; // 32 kW ou KVA maxi
function createCarteGraphHistorique(z) {
    let contenu = '<div class="hom-boxEdit hom-graphPw-T edit">';
    contenu += "<div class='hom-graphPw-T' >Historique</div>";
    contenu += '<div class="hom-graphPw-edit">';
    contenu += '<div>Echelle du temps :</div>';
    contenu += `<input type="radio" id="T48h_${z}" name="graphPw_${z}" value="T48h" onclick="chgGraphPw(this)">`;
    contenu += `<label for="T48h_${z}">48h</label>`;
    contenu += `<input type="radio" id="T10mn_${z}" name="graphPw_${z}" value="T10mn" onclick="chgGraphPw(this)">`;
    contenu += `<label for="T10mn_${z}">10mn</label>`;

    contenu += '</div>';
    contenu += '<div class="hom-graphPw-T">';
    contenu += '<div class="hom-graphPw-flex">';
    contenu += '<div>';
    contenu += '<div>Puissance Active (W)</div>';
    contenu += '<div id="GraphPw_Puissance_W' + z + '"></div>';
    contenu += '</div><div>';
    contenu += '<div>Puissance Apparente (VA)</div>';
    contenu += '<div id="GraphPw_Puissance_VA' + z + '"></div>';
    contenu += '</div><div>';
    contenu += '<div>Température</div>';
    contenu += '<div id="GraphPw_Temperature_' + z + '"></div>';
    contenu += '</div><div>';
    contenu += '<div>Ouverture</div>';
    contenu += '<div id="GraphPw_Ouverture_' + z + '"></div>';
    contenu += '</div>';



    contenu += '</div>';
    contenu += '</div>';
    contenu += '</div>'; //Fin hom-boxEdit
    contenu += "<svg id='GraphHisto_SVG_" + z + "' viewBox='0 0 510 200' class='hom-graphPwSVG'></svg>";
    if (ListeZones[z].para == null) {

        let para = {
            dureeGraph: "T10mn",
            lesGraph: [] // Initialisation du tableau des graphes
        };
        ListeZones[z].para = para;

    }
    ListeZones[z].debutEdit = function (z) { afficher_MenuPTO(z); };
    return contenu;
}


function LoadHistoriquePuissance() {
    if (Puissance10mn == null) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                Puissance10mn = JSON.parse(this.responseText);

                //On passe le JSON dans des tableaux pour une mise à jour plus facile toutes les 2s
                for (let s = 0; s < Puissance10mn.lesSources.source.length; s++) {
                    Histo10mnPw[s] ??= [];
                    Histo10mnPva[s] ??= [];
                    for (let canal = 0; canal < Puissance10mn.lesSources.source[s].Canaux.length; canal++) {
                        Histo10mnPw[s][canal] ??= [];
                        Puissance10mn.lesSources.source[s].Canaux[canal].Pw.forEach(val => Histo10mnPw[s][canal].push(val));
                        Histo10mnPva[s][canal] ??= [];
                        Puissance10mn.lesSources.source[s].Canaux[canal].Pva.forEach(val => Histo10mnPva[s][canal].push(val));
                    }
                }

                for (let canal = 0; canal < Puissance10mn.lesTemperatures.CanalTemp.length; canal++) {
                    Histo10mnTemp[canal] ??= [];
                    Puissance10mn.lesTemperatures.CanalTemp[canal].Temp.forEach(val => Histo10mnTemp[canal].push(val));
                }

                for (let action = 0; action < Puissance10mn.lesOuvertures.BlocOuvre.length; action++) {
                    Histo10mnOuverture[action] ??= [];
                    Puissance10mn.lesOuvertures.BlocOuvre[action].Ouvre.forEach(val => Histo10mnOuverture[action].push(val));
                }


                traceLesHistoriques();
            }
        };
        let adr = adrHttpRemote + "/ajax_histo10mn";
        xhttp.open('GET', adr, true);

        xhttp.send();
    } else {
        traceLesHistoriques();
    }
    if (Puissance48h == null) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                Puissance48h = JSON.parse(this.responseText);
                //On passe le JSON dans des tableaux pour une compatibilité avec 10mn
                for (let s = 0; s < Puissance48h.lesSources.source.length; s++) {
                    Histo48hPw[s] ??= [];
                    Histo48hPva[s] ??= [];
                    for (let canal = 0; canal < Puissance48h.lesSources.source[s].Canaux.length; canal++) {
                        Histo48hPw[s][canal] ??= [];
                        Puissance48h.lesSources.source[s].Canaux[canal].Pw.forEach(val => Histo48hPw[s][canal].push(val));
                        Histo48hPva[s][canal] ??= [];
                        Puissance48h.lesSources.source[s].Canaux[canal].Pva.forEach(val => Histo48hPva[s][canal].push(val));
                    }
                }

                for (let canal = 0; canal < Puissance48h.lesTemperatures.CanalTemp.length; canal++) {
                    Histo48hTemp[canal] ??= [];
                    Puissance48h.lesTemperatures.CanalTemp[canal].Temp.forEach(val => Histo48hTemp[canal].push(val));
                }

                for (let action = 0; action < Puissance48h.lesOuvertures.BlocOuvre.length; action++) {
                    Histo48hOuverture[action] ??= [];
                    Puissance48h.lesOuvertures.BlocOuvre[action].Ouvre.forEach(val => Histo48hOuverture[action].push(val));
                }

                traceLesHistoriques();
            }
        };

        let adr = adrHttpRemote + "/ajax_histo48h";
        xhttp.open('GET', adr, true);
        xhttp.send();



    } else {
        traceLesHistoriques();
    }
}
function traceLesHistoriques() { //Plusieurs zones eventuelles avec des historique

    for (let zone = 0; zone < ListeZones.length; zone++) {
        if (ListeZones[zone].type == "graphHistorique") TraceHistorique(zone);
    }
}
function chgGraphPw(element) {
    let zone = element.name.split('_')[1];
    ListeZones[zone].para.dureeGraph = element.value;
    TraceHistorique(zone);
}

function afficher_MenuPTO(z) { //Menu choix des graphes
    let numGraph = null, chk = null, couleur = null;
    let idxPasUtilise = 0;
    //Nettoyage ancien graphe non utilisé
    while (idxPasUtilise >= 0) {
        idxPasUtilise = -1; //Sortie de la boucle pour l'instant
        for (let gr = 0; gr < ListeZones[z].para.lesGraph.length; gr++) {
            if (!ListeZones[z].para.lesGraph[gr].GraphUtilise) {
                idxPasUtilise = gr;
                ListeZones[z].para.lesGraph.splice(gr, 1); //Supprime le graph non utilisé
                break;
            }
        }
    }
    for (let gr = 0; gr < ListeZones[z].para.lesGraph.length; gr++) {
        ListeZones[z].para.lesGraph[gr].GraphUtilise = false; //Va t'on utilisé ce graph déjà defini en cas de changement de configuration
    }
    //==colonnes puissance ====
    let GPw = '<table class="hom-graphPw-table">';
    let GPva = '<table class="hom-graphPw-table">';
    GPw += '<tr class="fsize10"><td>Afficher</td><td>Canal</td><td>Couleur</td></tr>';
    GPva += '<tr class="fsize10"><td>Afficher</td><td>Canal</td><td>Couleur</td></tr>';
    for (let s = 0; s < C.LaSource.length; s++) {
        for (let canal = 0; canal < C.LaSource[s].CanalNom.length; canal++) {
            if (C.LaSource[s].CanalNom[canal] != "") {
                numGraph = ReferenceDuGrapheHisto(z, "Pw", s, canal);
                chk = ListeZones[z].para.lesGraph[numGraph].dispGraph ? "checked" : "";
                couleur = ListeZones[z].para.lesGraph[numGraph].color;
                GPw += '<tr><td><input  type="checkbox" id="GrChk_' + z + '_' + numGraph + '"  onclick="setGraphH(this)" ' + chk + '></td>';
                GPw += '<td class="L">' + C.LaSource[s].CanalNom[canal] + ' </td>';
                GPw += '<td><input class="hom-graphPw-color" type="color" id="GrColor_' + z + '_' + numGraph + '"  value="' + couleur + '" onchange="setColorH(this)"></td>';
                GPw += ' </tr>';

                numGraph = ReferenceDuGrapheHisto(z, "Pva", s, canal);
                chk = ListeZones[z].para.lesGraph[numGraph].dispGraph ? "checked" : "";
                couleur = ListeZones[z].para.lesGraph[numGraph].color;
                GPva += '<tr><td><input  type="checkbox" id="GrChk_' + z + '_' + numGraph + '"  onclick="setGraphH(this)" ' + chk + '></td>';
                GPva += '<td class="L">' + C.LaSource[s].CanalNom[canal] + ' </td>';
                GPva += '<td><input class="hom-graphPw-color" type="color" id="GrColor_' + z + '_' + numGraph + '"  value="' + couleur + '" onchange="setColorH(this)"></td>';
                GPva += ' </tr>';
            }
        }
    }
    GPw += '</table>';
    GPva += '</table>';
    GH("GraphPw_Puissance_W" + z, GPw);
    GH("GraphPw_Puissance_VA" + z, GPva);
    //===colonne temperature ===
    let GTemp = '<table class="hom-graphPw-table">';
    GTemp += '<tr class="fsize10"><td>Afficher</td><td>Canal</td><td>Couleur</td></tr>';
    for (let canal = 0; canal < C.LesTemperatures.length; canal++) {
        if (C.LesTemperatures[canal].Source_Temp != "tempNo") {
            numGraph = ReferenceDuGrapheHisto(z, "Temp", canal, 0);
            chk = ListeZones[z].para.lesGraph[numGraph].dispGraph ? "checked" : "";
            couleur = ListeZones[z].para.lesGraph[numGraph].color;
            GTemp += '<tr><td><input  type="checkbox" id="GrChk_' + z + '_' + numGraph + '"  onclick="setGraphH(this)" ' + chk + '></td>';
            GTemp += '<td class="L">' + C.LesTemperatures[canal].nomTemperature + ' </td>';
            GTemp += '<td><input class="hom-graphPw-color" type="color" id="GrColor_' + z + '_' + numGraph + '"  value="' + couleur + '" onchange="setColorH(this)"></td>';
            GTemp += ' </tr>';
        }
    }
    GTemp += '</table>';
    GH("GraphPw_Temperature_" + z, GTemp);

    //===colonne ouverture ===
    let JSD = C.LesActions;

    let GOuv = '<table class="hom-graphPw-table">';
    GOuv += '<tr class="fsize10"><td>Afficher</td><td>Canal</td><td>Couleur</td></tr>';
    let iAction = 0;
    for (let i = 0; i < JSD.blocs.length; i++) {
        if (JSD.blocs[i].type == "action") {
            let nom = JSD.blocs[i].Para.Nom;
            numGraph = ReferenceDuGrapheHisto(z, "Ouv", iAction, 0);
            chk = ListeZones[z].para.lesGraph[numGraph].dispGraph ? "checked" : "";
            couleur = ListeZones[z].para.lesGraph[numGraph].color;
            GOuv += '<tr><td><input  type="checkbox" id="GrChk_' + z + '_' + numGraph + '"  onclick="setGraphH(this)" ' + chk + '></td>';
            GOuv += '<td class="L">' + nom + ' </td>';
            GOuv += '<td><input class="hom-graphPw-color" type="color" id="GrColor_' + z + '_' + numGraph + '"  value="' + couleur + '" onchange="setColorH(this)"></td>';
            GOuv += ' </tr>';
            iAction++;
        }
    }
    GOuv += '</table>';
    GH("GraphPw_Ouverture_" + z, GOuv);
}
function ReferenceDuGrapheHisto(z, type_graph, ref1, ref2) {
    // Obtention deses caractéristiques du graphe
    let trouve = false;
    for (let gr = 0; gr < ListeZones[z].para.lesGraph.length; gr++) {
        if (ListeZones[z].para.lesGraph[gr].type_graph == type_graph) {
            if (ListeZones[z].para.lesGraph[gr].ref1 == ref1) {
                if (ListeZones[z].para.lesGraph[gr].ref2 == ref2) {
                    ListeZones[z].para.lesGraph[gr].GraphUtilise = true;
                    return gr;
                }

            }
        }
    }
    //N'existe pas, on crée une nouvelle entrée 
    const hue = ListeZones[z].para.lesGraph.length * 133; //Roue des couleurs
    let couleur = DefCouleur(hue, 60);
    ListeZones[z].para.lesGraph.push({ dispGraph: true, type_graph: type_graph, ref1: ref1, ref2: ref2, color: couleur, GraphUtilise: true });
    const newIndex = ListeZones[z].para.lesGraph.length - 1;
    return newIndex;
}

function setGraphH(checkbox) {
    const parts = checkbox.id.split('_');
    const indiceGraph = parseInt(parts[2], 10);
    const z = parseInt(parts[1], 10);

    ListeZones[z].para.lesGraph[indiceGraph].dispGraph = checkbox.checked;
    TraceHistorique(z);
}
function setColorH(element) {
    let zone = element.id.split('_')[1];
    ListeZones[zone].para.lesGraph[element.id.split('_')[2]].color = element.value;
    TraceHistorique(zone);
}

function TraceHistorique(z) {
    const dureeGraph = ListeZones[z].para.dureeGraph;
    GID(dureeGraph + "_" + z).checked = true;  //Checkbox durée
    const Historique = dureeGraph == "T48h" ? Puissance48h : Puissance10mn;
    if (Historique == null) return;

    let typePw = false;
    let typePva = false;
    let typeTemp = false;
    let typeOuv = false;
    let PwvaMin = 0;
    let PwvaMax = 100;
    let TempMin = 0;
    let TempMax = 10;
    let cadrageMaxPwva = 1;
    let cadrageMaxTemp = 1;
    const LesGraph = ListeZones[z].para.lesGraph;
    for (let i = 0; i < LesGraph.length; i++) {
        const graph = LesGraph[i];

        if (graph.dispGraph && graph.GraphUtilise) {

            if (graph.type_graph == "Pw" || graph.type_graph == "Pva") {
                if (graph.type_graph == "Pw") typePw = true;
                if (graph.type_graph == "Pva") typePva = true;
                const s = graph.ref1; //source
                const canal = graph.ref2;
                let tableau;
                if (dureeGraph == "T10mn") {
                    tableau = graph.type_graph == "Pw" ? [...Histo10mnPw[s][canal]] : [...Histo10mnPva[s][canal]];
                } else {
                    tableau = graph.type_graph == "Pw" ? [...Histo48hPw[s][canal]] : [...Histo48hPva[s][canal]];
                }
                const copie = tableau.filter(valeur => valeur <= PuissanceMaximum && valeur >= -PuissanceMaximum); //On rejete les valeurs non valides
                const maximum = Math.max(...copie);
                const minimum = Math.min(...copie);
                PwvaMin = Math.min(PwvaMin, minimum);
                PwvaMax = Math.max(PwvaMax, maximum);
            };

            if (graph.type_graph == "Temp") {
                typeTemp = true;
                const canal = graph.ref1;
                const tableau = dureeGraph == "T10mn" ? [...Histo10mnTemp[canal]] : [...Histo48hTemp[canal]];
                const copie = tableau.filter(valeur => valeur >= -1000); //On rejete les valeurs non valides
                const maximum = Math.max(...copie);
                const minimum = Math.min(...copie);
                TempMin = Math.min(TempMin, minimum); //dixième de degré
                TempMax = Math.max(TempMax, maximum);

            }
            if (graph.type_graph == "Ouv") typeOuv = true;
        }
    }
    TempMin = TempMin / 10; // En degré
    TempMax = TempMax / 10; // En degré


    //Positionnement graphe en X
    let X0 = 0;
    if (typePw || typePva) X0 += 15;
    if (typeTemp) X0 += 15;
    if (typeOuv) X0 += 15;
    let Ymin = 0;
    let Ymax = 10;
    let Y0 = 105; //Valeurs positives uniquement
    let dY = 9;
    let YmaxGraph = Y0;
    let viewBox = '0 0 510 120';
    let axeY = '';
    if (PwvaMin < 0 || TempMin < 0) { //Valeurs positives et négatives
        Ymin = -10;
        Y0 = 105;
        viewBox = '0 0 510 200'
        YmaxGraph = 195;
        axeY = `${X0},${YmaxGraph} `;

    }

    const NS = "http://www.w3.org/2000/svg";
    const HistSvg = GID("GraphHisto_SVG_" + z);
    HistSvg.setAttribute("viewBox", viewBox);
    HistSvg.innerHTML = "";


    //Trace Axes
    let x_Max = 500;
    let NBmax = 600;
    let stepTick = 12;
    const HeureCouranteDeci = Historique.HeureCouranteDeci / 100;

    if (dureeGraph == "T10mn") {
        NBmax = 300;
        stepTick = 30;
    }

    let axes = document.createElementNS(NS, "polyline");
    axes.setAttribute("points", `${axeY}${X0},${15} ${X0},${Y0} ${x_Max},${Y0}`);
    axes.setAttribute("stroke", "var(--hom-histaxes)");
    axes.setAttribute("stroke-width", 1);
    axes.setAttribute("fill", "none");
    HistSvg.appendChild(axes);


    let step = (x_Max - X0) / NBmax
    let xDepart = (HeureCouranteDeci - Math.floor(HeureCouranteDeci)) * step * stepTick;

    for (let Nb = 0; Nb <= NBmax; Nb += stepTick) {
        let x, y, h;
        if (dureeGraph == "T48h") {
            h = (Math.floor(HeureCouranteDeci) - Nb / stepTick + 72) % 24;
            x = x_Max - xDepart - step * Nb;
            y = Y0;
        } else { // T10mn
            x = x_Max - step * Nb;
            y = Y0;
            h = - Nb / stepTick;
        }
        if (x > X0) {
            let tick = document.createElementNS(NS, "line");
            tick.setAttribute("x1", x);
            tick.setAttribute("y1", y);
            tick.setAttribute("x2", x);
            tick.setAttribute("y2", y + 4);
            tick.setAttribute("stroke", "var(--hom-histaxes)");
            tick.setAttribute("stroke-width", 1);
            HistSvg.appendChild(tick);
            let txt = document.createElementNS(NS, "text");
            txt.setAttribute("x", x);
            txt.setAttribute("y", y + 9);
            txt.setAttribute("fill", "var(--hom-histlabel)");
            txt.textContent = h;
            txt.setAttribute("font-size", "5");
            HistSvg.appendChild(txt);
        }
    }
    let X00 = X0
    if (typePw || typePva) {
        let unit = "";
        if (typePw) unit = "W";
        if (typePva) unit += "  VA";
        cadrageMaxPwva = EchelleY(HistSvg, NS, X00, Y0, Ymin, Ymax, dY, PwvaMin, PwvaMax, unit);
        X00 -= 15;
    }
    if (typeTemp) {
        cadrageMaxTemp = EchelleY(HistSvg, NS, X00, Y0, 0, Ymax, dY, TempMin, TempMax, "°C");
        X00 -= 15;
    }
    if (typeOuv) {
        EchelleY(HistSvg, NS, X00, Y0, 0, Ymax, dY, 0, 100, "%");
    }

    for (let Y = Ymin; Y <= Ymax; Y += 2) { //Pointillés
        let y = Y0 - Y * dY;
        let tick = document.createElementNS(NS, "line");
        tick.setAttribute("x1", X0);
        tick.setAttribute("y1", y);
        tick.setAttribute("x2", x_Max);
        tick.setAttribute("y2", y);
        tick.setAttribute("stroke", "var(--hom-histaxes)");
        tick.setAttribute("stroke-width", 0.1);
        HistSvg.appendChild(tick);
    }

    // === Courbes ===
    let LesTitres = [];
    let ColorTitres = [];
    let LesValeurs = [];
    let LesTypes = [];
    for (let i = 0; i < LesGraph.length; i++) {
        const graph = LesGraph[i];

        if (graph.dispGraph && graph.GraphUtilise) {

            if (graph.type_graph == "Pw" || graph.type_graph == "Pva") {
                const s = graph.ref1; //source
                const canal = graph.ref2;
                if (C.LaSource[s].CanalNom[canal]) {
                    LesTitres.push(C.LaSource[s].CanalNom[canal]);
                    ColorTitres.push(graph.color);
                    LesTypes.push(graph.type_graph);


                    let tableau;
                    if (dureeGraph == "T10mn") {
                        tableau = graph.type_graph == "Pw" ? [...Histo10mnPw[s][canal]] : [...Histo10mnPva[s][canal]];
                    } else {
                        tableau = graph.type_graph == "Pw" ? [...Histo48hPw[s][canal]] : [...Histo48hPva[s][canal]];
                    }
                    LesValeurs.push(tableau);
                    let courbe = document.createElementNS(NS, "polyline");
                    courbe.setAttribute("stroke", graph.color);
                    courbe.setAttribute("stroke-width", 1);
                    courbe.setAttribute("fill", "none");
                    let step = (x_Max - X0) / (tableau.length - 1);
                    let courbePoints = "";
                    for (let j = 0; j < tableau.length; j++) {
                        const valeur = tableau[j];
                        let x = X0 + j * step;
                        let y = Y0 - valeur * 10 * dY / cadrageMaxPwva;
                        courbePoints += `${x},${y} `;
                    }
                    courbe.setAttribute("points", courbePoints);
                    HistSvg.appendChild(courbe);
                }
            };
            if (graph.type_graph == "Temp") {

                const canal = graph.ref1;
                if (C.LesTemperatures[canal].nomTemperature) {
                    LesTitres.push(C.LesTemperatures[canal].nomTemperature);
                    ColorTitres.push(graph.color);
                    LesTypes.push(graph.type_graph);

                    const tableau = dureeGraph == "T10mn" ? [...Histo10mnTemp[canal]] : [...Histo48hTemp[canal]];
                    LesValeurs.push(tableau);
                    let courbe = document.createElementNS(NS, "polyline");
                    courbe.setAttribute("stroke", graph.color);
                    courbe.setAttribute("stroke-width", 1);
                    courbe.setAttribute("fill", "none");
                    let step = (x_Max - X0) / (tableau.length - 1);
                    let courbePoints = "";
                    for (let j = 0; j < tableau.length; j++) {
                        const valeur = tableau[j] / 10;
                        if (valeur > -100) {
                            let x = X0 + j * step;
                            let y = Y0 - valeur * 10 * dY / cadrageMaxTemp;
                            courbePoints += `${x},${y} `;
                        }
                    }
                    courbe.setAttribute("points", courbePoints);
                    HistSvg.appendChild(courbe);
                }
            }
            if (graph.type_graph == "Ouv") {
                const action = parseInt(graph.ref1);
                if (Historique.lesOuvertures.BlocOuvre[action]) {
                    LesTitres.push(Historique.lesOuvertures.BlocOuvre[action].nomAction);
                    ColorTitres.push(graph.color);
                    LesTypes.push(graph.type_graph);
                    const tableau = dureeGraph == "T10mn" ? [...Histo10mnOuverture[action]] : [...Histo48hOuverture[action]];
                    LesValeurs.push(tableau);
                    let courbe = document.createElementNS(NS, "polyline");
                    courbe.setAttribute("stroke", graph.color);
                    courbe.setAttribute("stroke-width", 1);
                    courbe.setAttribute("fill", "none");
                    let step = (x_Max - X0) / (tableau.length - 1);
                    let courbePoints = "";
                    for (let j = 0; j < tableau.length; j++) {
                        const valeur = tableau[j];
                        let x = X0 + j * step;
                        let y = Y0 - valeur * 10 * dY / 100;
                        courbePoints += `${x},${y} `;
                    }
                    courbe.setAttribute("points", courbePoints);
                    HistSvg.appendChild(courbe);
                }
            }
        }
    }
    const largeur = x_Max - X0 - 20;
    let txt = document.createElementNS(NS, "text");

    txt.setAttribute("y", 12);
    txt.setAttribute("font-size", 140 / (10 + LesTitres.length));
    LesTitres.forEach((texte, i) => {
        const tspan = document.createElementNS(NS, "tspan");
        const x = i * largeur / (LesTitres.length - 0.9);
        tspan.setAttribute("x", X0 + 10 + x);
        tspan.setAttribute("fill", ColorTitres[i]);
        if (i === 0)
            tspan.setAttribute("text-anchor", "start");
        else if (i === LesTitres.length - 1)
            tspan.setAttribute("text-anchor", "end");
        else
            tspan.setAttribute("text-anchor", "middle");
        tspan.textContent = texte;
        txt.appendChild(tspan);
    });

    HistSvg.appendChild(txt);

    //Position curseur


    let ligneCurseur = document.createElementNS(NS, "line");

    ligneCurseur.setAttribute("y1", 15);
    ligneCurseur.setAttribute("y2", YmaxGraph);   // hauteur du SVG
    ligneCurseur.setAttribute("stroke", "red");
    ligneCurseur.setAttribute("stroke-width", 1);
    ligneCurseur.setAttribute("pointer-events", "none");

    HistSvg.appendChild(ligneCurseur);
    HistSvg.addEventListener("pointermove", function (event) {
        const pt = HistSvg.createSVGPoint();
        pt.x = event.clientX;
        pt.y = event.clientY;
        const p = pt.matrixTransform(
            HistSvg.getScreenCTM().inverse()
        );
        let info = "";
        for (let i = 0; i < LesValeurs.length; i++) {
            let tableau = LesValeurs[i];
            let idx = (p.x - X0) / ((x_Max - X0) / (tableau.length - 1));
            idx = Math.round(idx);
            let valeur = null;
            if (idx >= 0 && idx < tableau.length) {
                let unit = "W";
                if (LesTypes[i] === "Temp") {
                    unit = "°C";
                    if (tableau[idx] > -1000) {
                        valeur = tableau[idx] / 10; // Temperature valide
                        valeur = valeur.toFixed(1);

                    }
                } else {
                    if (LesTypes[i] === "Pva") unit = "VA";
                    if (LesTypes[i] === "Ouv") unit = "%";
                    valeur = tableau[idx];
                }
                if (valeur != null) {
                    info += `<div style="color:${ColorTitres[i]};"> ${valeur} ${unit}</div>`;
                }

            }
        }
        GH("infoCurseur", info);
        GID("infoCurseur").style.top = (pt.y + window.scrollY) + "px";
        if (p.x > 250) {
            GID("infoCurseur").style.right = (window.innerWidth - pt.x - 10) + "px";
            GID("infoCurseur").style.left = "auto";
        } else {
            GID("infoCurseur").style.left = (10 + pt.x) + "px";
            GID("infoCurseur").style.right = "auto";
        }
        ligneCurseur.setAttribute("x1", p.x);
        ligneCurseur.setAttribute("x2", p.x);
        if (p.x < X0 || p.x > x_Max || p.y < 15 || p.y > YmaxGraph) {
            ligneCurseur.style.display = "none";
            GID("infoCurseur").style.display = "none";
        } else {
            ligneCurseur.style.display = "block";
            GID("infoCurseur").style.display = "block";
        }
    });



}

function EchelleY(HistSvg, NS, X0, Y0, Ymin, Ymax, dY, Min, Max, unit) {
    Max = Math.max(Max, -Min)
    let cadrageMax = 1;
    let cadrage1 = 1000000;
    let cadrage2 = [10, 8, 5, 4, 2, 1];
    for (let m = 0; m < 7; m++) {
        for (let i = 0; i < cadrage2.length; i++) {
            let X = cadrage1 * cadrage2[i];
            if ((Max) <= X) cadrageMax = X;
        }
        cadrage1 = cadrage1 / 10;
    }
    for (let Y = Ymin; Y <= Ymax; Y += 2) {
        let x = X0;
        let y = Y0 - Y * dY;
        let tick = document.createElementNS(NS, "line");
        tick.setAttribute("x1", x);
        tick.setAttribute("y1", y);
        tick.setAttribute("x2", x - 15);
        tick.setAttribute("y2", y);
        tick.setAttribute("stroke", "var(--hom-histaxes)");
        tick.setAttribute("stroke-width", 1);
        HistSvg.appendChild(tick);
        let txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", x - 2);
        txt.setAttribute("y", y - 2);
        txt.setAttribute("text-anchor", "end");
        txt.setAttribute("fill", "var(--hom-histlabel)");
        txt.textContent = Y * cadrageMax / 10;
        txt.setAttribute("font-size", "5");
        HistSvg.appendChild(txt);
    }

    let txt = document.createElementNS(NS, "text");
    txt.setAttribute("x", X0);
    txt.setAttribute("y", 8);
    txt.setAttribute("fill", "var(--hom-histlabel)");
    txt.setAttribute("text-anchor", "end");
    txt.textContent = unit;
    txt.setAttribute("font-size", "5");
    HistSvg.appendChild(txt);
    return cadrageMax;
}