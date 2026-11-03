const EnergieMax = 300000; //300kWh max pour enlever des chiffres déconnants
function createCarteGraphEnergie(z) {
    let contenu = '<div class="hom-boxEdit hom-graphEn-T edit">';
    contenu += "<div class='hom-graphEn-T' >Energie (Wh)</div>";
    contenu += '<div class="hom-graphEn-2T">';
    contenu += '<div id="GraphEn_Wh' + z + '"></div>';
    contenu += '<div><div class="hom-graphEn-J">';//Debut jours'
    contenu += '<div >Jours : </div>';
    contenu += `<input type="radio" id="j10_${z}" name="jour_${z}" value="10" onclick="chgJour(this)">`;
    contenu += `<label for="j10_${z}">10</label>`;
    contenu += `<input type="radio" id="j30_${z}" name="jour_${z}" value="30" onclick="chgJour(this)" checked>`;
    contenu += `<label for="j30_${z}">30</label>`;
    contenu += `<input type="radio" id="j90_${z}" name="jour_${z}" value="90" onclick="chgJour(this)">`;
    contenu += `<label for="j90_${z}">90</label>`;
    contenu += `<input type="radio" id="j370_${z}" name="jour_${z}" value="370" onclick="chgJour(this)">`;
    contenu += `<label for="j370_${z}">370</label>`;
    contenu += `<input type="radio" id="j450_${z}" name="jour_${z}" value="450" onclick="chgJour(this)">`;
    contenu += `<label for="j450_${z}">450</label>`;
    contenu += '</div></div>'; //Fin des jours
    contenu += '</div>'; //Fin hom-graphEn-2T
    contenu += '</div>'; //Fin hom-boxEdit
    contenu += "<svg id='GraphEn_SVG_" + z + "' viewBox='0 0 1400 400' class='hom-graphEnSVG'></svg>";
    if (ListeZones[z].para == null) {
        let para = {
            lesGraph: [], // Initialisation du tableau des graphes
            jour: 30 // Valeur par défaut pour le nombre de jours
        };
        ListeZones[z].para = para;
    }
    ListeZones[z].debutEdit = function (z) { afficher_MenuEn(z); };
    return contenu;
    
}
function afficher_MenuEn(z) {
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
    //==Energie ====
    let GPEn = '<table class="hom-graphEn-table">';
    GPEn += '<tr class="fsize10"><td>Afficher</td><td>Canal</td><td>Soutiré</td><td>Injecté</td></tr>';
    for (let s = 0; s < C.LaSource.length; s++) {
        for (let canal = 0; canal < C.LaSource[s].CanalNom.length; canal++) {
            if (C.LaSource[s].CanalNom[canal] != "") {
                numGraph = ReferenceDuGrapheEner(z, "En", s, canal);
                chk = ListeZones[z].para.lesGraph[numGraph].dispGraph ? "checked" : "";
                couleurS = ListeZones[z].para.lesGraph[numGraph].colorS;
                couleurI = ListeZones[z].para.lesGraph[numGraph].colorI;
                GPEn += '<tr><td><input  type="checkbox" id="GrChk_' + z + '_' + numGraph + '"  onclick="setGraphE(this)" ' + chk + '></td>';
                GPEn += '<td class="L">' + C.LaSource[s].CanalNom[canal] + ' </td>';
                GPEn += '<td><input class="hom-graphEn-color" type="color" id="GrColorS_' + z + '_' + numGraph + '"  value="' + couleurS + '" onchange="setColorE(this)"></td>';
                GPEn += '<td><input class="hom-graphEn-color" type="color" id="GrColorI_' + z + '_' + numGraph + '"  value="' + couleurI + '" onchange="setColorE(this)"></td>';
                GPEn += ' </tr>';
            }
        }
    }
    GPEn += '</table>';
    GH("GraphEn_Wh" + z, GPEn);
    
}
function setGraphE(checkbox) {
    const parts = checkbox.id.split('_');
    const indiceGraph = parseInt(parts[2], 10);
    const z = parseInt(parts[1], 10);

    ListeZones[z].para.lesGraph[indiceGraph].dispGraph = checkbox.checked;
    TraceEnergie(z);
}
function setColorE(element) {
    let zone = element.id.split('_')[1];
    if (element.id.startsWith("GrColorS_")) {
        ListeZones[zone].para.lesGraph[element.id.split('_')[2]].colorS = element.value;
    } else if (element.id.startsWith("GrColorI_")) {
        ListeZones[zone].para.lesGraph[element.id.split('_')[2]].colorI = element.value;
    }
    TraceEnergie(zone);
}
function chgJour(checkbox) {
    const parts = checkbox.id.split('_');
    const z = parseInt(parts[1], 10);
    ListeZones[z].para.jour = parseInt(checkbox.value, 10);
    TraceEnergie(z);
}
function ReferenceDuGrapheEner(z, type_graph, ref1, ref2) {
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
    let couleurS = DefCouleur(hue, 40);
    let couleurI = DefCouleur((hue + 20) % 360, 40);
    ListeZones[z].para.lesGraph.push({ dispGraph: true, type_graph: type_graph, ref1: ref1, ref2: ref2, colorS: couleurS, colorI: couleurI, GraphUtilise: true });
    const newIndex = ListeZones[z].para.lesGraph.length - 1;
    return newIndex;
}

let Energies = null;
function LoadEnergies() {
    if (Energies == null) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                Energies = JSON.parse(this.responseText);
                console.log(Energies);
                traceLesEnergies();
            }
        };
        let adr=adrHttpRemote +  "/ajax_histoEnergie";
        
        xhttp.open('GET', adr, true);
        
        xhttp.send();
    } else {
        traceLesEnergies()
    }
}
function traceLesEnergies() { //Plusieurs zones eventuelles avec des historique
    for (let zone = 0; zone < ListeZones.length; zone++) {
        if (ListeZones[zone].type == "graphEnergie") {
            afficher_MenuEn(zone);
            TraceEnergie(zone);
        }
    }
}
function TraceEnergie(z) {
    console.log("TraceEnergie for zone",z, ListeZones[z].para);
    let NBjour = ListeZones[z].para.jour;
    GID("j" + NBjour + "_" + z).checked = true;
    let tableauWh_S = [];
    let tableauWh_I = [];
    let tableauDate = [];
    let tableauNomCanaux = [];
    let tableauNomSoutire = [];
    let tableauNomInjecte = [];
    let tableauDesSources = [];
    let tableauDesCanaux = [];
    let tableauDesCouleursS = [];
    let tableauDesCouleursI = [];
    let LesTitres = [];
    let numGraph;
    const LesGraph = ListeZones[z].para.lesGraph;
    for (let i = 0; i < LesGraph.length; i++) {
        const graph = LesGraph[i];

        if (graph.dispGraph && graph.GraphUtilise) {
            numGraph = tableauNomCanaux.length;
            tableauWh_S[numGraph] = [];
            tableauWh_I[numGraph] = [];
            tableauDate[numGraph] = [];
            tableauNomCanaux[numGraph] = [];
            tableauNomSoutire[numGraph] = [];
            tableauNomInjecte[numGraph] = [];


            const s = graph.ref1; //source
            const canal = graph.ref2;
            tableauDesSources[numGraph] = s;
            tableauDesCanaux[numGraph] = canal;
            tableauDesCouleursS[numGraph] = graph.colorS;
            tableauDesCouleursI[numGraph] = graph.colorI;

            let nomCanal = "", nomSoutire = "", nomInjecte = "";
            let idxSoutire = -1, idxInjecte = -1;

            for (let M = 0; M < Energies.AnMois.length; M++) { //Tableau de chaque mois
                let lignes = Energies.Data[M].split("\r\n");
                for (L = 0; L < lignes.length; L++) {
                    let LaLigne = lignes[L].split(',');
                    if (LaLigne.length > 1) { // On a des données
                        if (LaLigne[0] == 'Date') {
                            idxSoutire = -1; idxInjecte = -1;
                            nomSoutire = "";
                            nomInjecte = "";
                            for (let j = 1; j < LaLigne.length; j++) {
                                let laConf = LaLigne[j];
                                let laConfParts = laConf.split('#');
                                let numSource = laConfParts[1].substring(0, 1);
                                let numCanal = laConfParts[1].substring(1, 2);
                                if (numSource == s && numCanal == canal) {
                                    nomCanal = laConfParts[0].split('(')[0].trim();
                                    if (laConfParts[1].substring(2, 3) == "S") {
                                        nomSoutire = laConfParts[1].split('Wh')[1].trim();
                                        idxSoutire = j;
                                    }
                                    if (laConfParts[1].substring(2, 3) == "I") {
                                        nomInjecte = laConfParts[1].split('Wh')[1].trim();
                                        idxInjecte = j;
                                        break;
                                    }
                                }
                            }
                        } else {
                            let trouve = false;
                            if (LaLigne.length > idxSoutire && idxSoutire != -1) {
                                tableauWh_S[numGraph].push(parseInt(LaLigne[idxSoutire]));
                                trouve = true;
                            }
                            if (LaLigne.length > idxInjecte && idxInjecte != -1) {
                                tableauWh_I[numGraph].push(parseInt(LaLigne[idxInjecte]));
                                trouve = true;
                            }
                            if (trouve) { //Pour chaque couple S et I on a une date, des noms. On rempli les tableaux
                                tableauNomCanaux[numGraph].push(nomCanal);
                                LesTitres[numGraph]=nomCanal; //Dernier nom connu
                                tableauNomSoutire[numGraph].push(nomSoutire);
                                tableauNomInjecte[numGraph].push(nomInjecte);
                                tableauDate[numGraph].push(LaLigne[0]); //A chaque date un soutiré et un injecté
                                if (tableauWh_S[numGraph].length < tableauDate[numGraph].length) {
                                    tableauWh_S[numGraph].push(null);
                                }
                                if (tableauWh_I[numGraph].length < tableauDate[numGraph].length) {
                                    tableauWh_I[numGraph].push(null);
                                }
                            }
                        }
                    }
                }
            }


        }
    }
    
    //Tableau de toutes les dates
    let toutesLesDates = [...new Set(tableauDate.flat())];
    toutesLesDates.sort((a, b) => {
        let [ja, ma, aa] = a.split("/");
        let [jb, mb, ab] = b.split("/");
        return new Date(aa, ma - 1, ja) - new Date(ab, mb - 1, jb);
    });

    toutesLesDates = toutesLesDates.slice(-NBjour); // On ne garde que les NBjour derniers jours
    //On va retirer les valeurs hors dates
    for (let i = 0; i < tableauDate.length; i++) {
        for (let j = 0; j < tableauDate[i].length; j++) {
            if (!toutesLesDates.includes(tableauDate[i][j])) {
                tableauDate[i].splice(j, 1);
                tableauWh_S[i].splice(j, 1);
                tableauWh_I[i].splice(j, 1);
                tableauNomCanaux[i].splice(j, 1);
                tableauNomSoutire[i].splice(j, 1);
                tableauNomInjecte[i].splice(j, 1);
                j--; // Ajuster l'index après la suppression            
            }
        }
    }
    //Recherche minimum et maximum des Wh
    let toutlesesWh_S = [...new Set(tableauWh_S.flat())];
    let toutlesesWh_I = [...new Set(tableauWh_I.flat())];
    const copieS = toutlesesWh_S.filter(valeur => valeur <= EnergieMax); //On rejete les valeurs non valides
    const copieI = toutlesesWh_I.filter(valeur => valeur <= EnergieMax); //On rejete les valeurs non valides
    const maximum = Math.max(...copieS, 10000) / 1000; //En kWh
    const minimum = Math.max(...copieI, 0) / 1000; //Ici les injecté sont positifs

    console.log("maximum", maximum, "minimum", minimum);

    const NS = "http://www.w3.org/2000/svg";
    const EnergieSVG = GID("GraphEn_SVG_" + z);

    EnergieSVG.innerHTML = "";


    //Trace Axes
    let x_Max = 1390;
    let X0 = 35;
    let Yhaut = 14;
    let Ybas = 395;
    let Y0 = EchelleEnergie(0, Yhaut, Ybas, maximum, minimum);

    let axes = document.createElementNS(NS, "polyline");
    axes.setAttribute("points", `${X0},${Yhaut} ${X0},${Ybas} ${X0},${Y0} ${x_Max},${Y0}`);
    axes.setAttribute("stroke", "var(--hom-histaxes)");
    axes.setAttribute("stroke-width", 1);
    axes.setAttribute("fill", "none");
    EnergieSVG.appendChild(axes);


    let step = 10; //10kWh
    if (maximum + minimum > 100) step = 20; //20kWh
    if (maximum + minimum > 200) step = 50; //50kWh

    for (let kWh = step; kWh <= maximum; kWh += step) { //Traits fins
        let y = EchelleEnergie(kWh, Yhaut, Ybas, maximum, minimum);
        let tick = document.createElementNS(NS, "line");
        tick.setAttribute("x1", X0);
        tick.setAttribute("y1", y);
        tick.setAttribute("x2", x_Max);
        tick.setAttribute("y2", y);
        tick.setAttribute("stroke", "var(--hom-histaxes)");
        tick.setAttribute("stroke-width", 0.2);
        EnergieSVG.appendChild(tick);
        let txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0 - 4);
        txt.setAttribute("y", y);
        txt.setAttribute("fill", "var(--hom-histlabel)");
        txt.setAttribute("text-anchor", "end");
        txt.textContent = kWh;
        txt.setAttribute("font-size", "12");
        EnergieSVG.appendChild(txt);
    }
    for (let kWh = -step; kWh >= -minimum; kWh -= step) { //Traits fins
        let y = EchelleEnergie(kWh, Yhaut, Ybas, maximum, minimum);
        let tick = document.createElementNS(NS, "line");
        tick.setAttribute("x1", X0);
        tick.setAttribute("y1", y);
        tick.setAttribute("x2", x_Max);
        tick.setAttribute("y2", y);
        tick.setAttribute("stroke", "var(--hom-histaxes)");
        tick.setAttribute("stroke-width", 0.2);
        EnergieSVG.appendChild(tick);
        let txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0 - 4);
        txt.setAttribute("y", y);
        txt.setAttribute("fill", "var(--hom-histlabel)");
        txt.setAttribute("text-anchor", "end");
        txt.textContent = kWh;
        txt.setAttribute("font-size", "12");
        EnergieSVG.appendChild(txt);
    }
    let txt = document.createElementNS(NS, "text");
    txt.setAttribute("x", X0 + 2);
    txt.setAttribute("y", Yhaut);
    txt.setAttribute("fill", "var(--hom-histlabel)");
    txt.textContent = "kWh";
    txt.setAttribute("font-size", "12");
    EnergieSVG.appendChild(txt);
    // Buildings
    step = (x_Max - X0) / (toutesLesDates.length +0.0001);
    let w = step - 2 - 200 / (tableauNomInjecte.length * toutesLesDates.length);
    let dw = w / tableauNomInjecte.length
    let tableauInfo=[];
    for (let i = 0; i < toutesLesDates.length; i++) {
        let celluleInfo=`<div>${toutesLesDates[i]}</div>`;
        let x = X0 + step * i;
        for (t = 0; t < tableauNomInjecte.length; t++) {

            let index = tableauDate[t].indexOf(toutesLesDates[i]);

            if (index !== -1) {
                let valeur = tableauWh_S[t][index]/1000;
                if (valeur <= EnergieMax && valeur != null) {
                    let rect = document.createElementNS(NS, "rect");
                    rect.setAttribute("x", x);
                    let h = EchelleEnergie(valeur , Yhaut, Ybas, maximum, minimum);
                    rect.setAttribute("y", h);
                    rect.setAttribute("width", dw);
                    rect.setAttribute("height", Y0 - h);
                    rect.setAttribute("fill", tableauDesCouleursS[t]);
                    rect.setAttribute("stroke", "none");
                    EnergieSVG.appendChild(rect);
                    celluleInfo += `<div>${tableauNomCanaux[t][index]} / ${tableauNomSoutire[t][index]} : ${valeur.toFixed(1)} kWh</div>`;
                }
                valeur = tableauWh_I[t][index]/1000;
                if (valeur <= EnergieMax && valeur != null) {
                    let rect = document.createElementNS(NS, "rect");
                    rect.setAttribute("x", x);
                    let h = EchelleEnergie(valeur , Yhaut, Ybas, maximum, minimum);
                    rect.setAttribute("y", Y0);
                    rect.setAttribute("width", dw);
                    rect.setAttribute("height", Y0 - h);
                    rect.setAttribute("fill", tableauDesCouleursI[t]);
                    rect.setAttribute("stroke", "none");
                    EnergieSVG.appendChild(rect);
                    celluleInfo += `<div>${tableauNomCanaux[t][index]} / ${tableauNomInjecte[t][index]} : ${valeur.toFixed(1)} kWh</div>`;
                }
               
            }
            x += dw;
        }
        tableauInfo.push(celluleInfo);
    }
    let deltaDate= 1+ Math.floor(toutesLesDates.length /12);
    for (let i = 0; i < toutesLesDates.length; i += deltaDate) {
        let txt = document.createElementNS(NS, "text");
        let x=X0 + (i+0.5) * step-8;
        let y = Y0 + 65;
        txt.setAttribute("x", x);
        txt.setAttribute("y", y);
        txt.setAttribute("fill", "var(--hom-histlabel)");
        txt.textContent = toutesLesDates[i];
        txt.setAttribute("font-size", "12");
        txt.setAttribute("transform", `rotate(-90 ${x} ${y})`);
        EnergieSVG.appendChild(txt);
    }

    //Les titres
    const largeur = x_Max - X0 - 80;
    txt = document.createElementNS(NS, "text");
    txt.setAttribute("y", 25);
    txt.setAttribute("font-size", 270/ (10 + LesTitres.length));
    LesTitres.forEach((texte, i) => {
        const tspan = document.createElementNS(NS, "tspan");
        const x = i * largeur / (LesTitres.length - 0.9);
        tspan.setAttribute("x", X0 + 40 + x);
        tspan.setAttribute("fill", tableauDesCouleursS[i]);
        if (i === 0)
            tspan.setAttribute("text-anchor", "start");
        else if (i === LesTitres.length - 1)
            tspan.setAttribute("text-anchor", "end");
        else
            tspan.setAttribute("text-anchor", "middle");
        tspan.textContent = texte;
        txt.appendChild(tspan);
    });
    EnergieSVG.appendChild(txt);

    //Position curseur
    let ligneCurseur = document.createElementNS(NS, "line");

    ligneCurseur.setAttribute("y1", Yhaut);
    ligneCurseur.setAttribute("y2", Ybas);   // hauteur du SVG
    ligneCurseur.setAttribute("stroke", "red");
    ligneCurseur.setAttribute("stroke-width", 1);
    ligneCurseur.setAttribute("pointer-events", "none");

    EnergieSVG.appendChild(ligneCurseur);
    EnergieSVG.addEventListener("pointermove", function (event) {
        const pt = EnergieSVG.createSVGPoint();
        pt.x = event.clientX;
        pt.y = event.clientY;
        const p = pt.matrixTransform(
            EnergieSVG.getScreenCTM().inverse()
        );
        let idx = (p.x - X0) /step;
        idx = Math.floor(idx);
        if (idx >= 0 && idx < toutesLesDates.length) {
            GH("infoCurseur", tableauInfo[idx]);
            GID("infoCurseur").style.top = (pt.y +window.scrollY) + "px";
            if (p.x > 700) {
                GID("infoCurseur").style.right = (window.innerWidth - pt.x + 10) + "px";
                GID("infoCurseur").style.left = "auto";
            } else {
                GID("infoCurseur").style.left = (10 + pt.x) + "px";
                GID("infoCurseur").style.right = "auto";
            }
           
            ligneCurseur.setAttribute("x1", p.x);
            ligneCurseur.setAttribute("x2", p.x);

            if (p.x < X0 || p.x > x_Max || p.y < Yhaut || p.y > Ybas) {
                ligneCurseur.style.display = "none";
                GID("infoCurseur").style.display = "none";
            } else {
                ligneCurseur.style.display = "block";
                GID("infoCurseur").style.display = "block";
            }
        }
    });
}
function EchelleEnergie(kWh, Yhaut, Ybas, maximum, minimum) {
    return Yhaut + (maximum - kWh) * (Ybas - Yhaut) / (maximum + minimum);
}