const cx = 255
const cy = 190
const r = 150
LesNomsJ = [];

function createCarteGraphJauge(z) {
    let contenu = `<div class="hom-boxEdit hom-graphPw-T edit">
                        <div class='hom-graphPw-T' >Jauge</div>
                        <div id="Jauge_Menu_${z}" class= "hom-graphPw-flex"></div>
                    </div>
                    <svg id='Jauge_SVG_${z}' viewBox='0 0 510 200' class='hom-graphPwSVG'>
                           
                    </svg>`;
    if (ListeZones[z].para == null) {

        let para = {
            nbSecteurs: 6,
            Couleurs: ["#2196F3", "#00BCD4", "#4CAF50", "#FFEB3B", "#FF9800", "#F44336"],
            Angles: [-90, -60, -30, 0, 30, 60, 90],
            ValMin: -4000,
            ValMax: 4000,
            Valeur: 0,
            RefJauge: null
        };
        ListeZones[z].para = para;
    }
    ListeZones[z].debutEdit = function (z) { afficher_MenuJauge(z); };
    return contenu;
}

function afficher_MenuJauge(z) {

    let Menu = `<div class="hom-tab-Jauge">
                    <div>
                        <label for="CapteurJ_${z}">Capteur : </label>
                        <select id="CapteurJ_${z}" onchange="ChoixJauge(this)">
                            <option></option>
                            <optgroup label="Puissance" id="groupePuissance_${z}">
                            <option value="puissance1" >⚡Puissance 1</option>
                            </optgroup>
                            
                            <optgroup label="Température" id="groupeTemperature_${z}">
                            <option value="temperature1" >🌡️ Pas de capteur</option>
                            </optgroup>
                            <optgroup label="Ouverture" id="groupeOuverture_${z}">
                            <option value="action1" >🚪Pas d'actionneur</option>          
                            </optgroup>
                            <optgroup label="Ensoleillement" id="groupeEnsoleillement_${z}">
                            <option value="soleil_actuel" >🌤️Actuel</option>
                            <option value="soleil_jourJ" >🌤️Jour J</option>
                            <option value="soleil_demain" >🌤️Jour J+1</option>
                            </optgroup>
                            <optgroup label="Tarif" id="groupeTarif_${z}">
                            <option value="tarif_tempo_J" >Tempo J &nbsp;&nbsp;&nbsp;🔵⚪🔴</option>
                            <option value="tarif_tempo_J1" >Tempo J+1 🔵⚪🔴</option>
                            <option value="tarif_HPHC" >HP☀️ / HC🌙</option>
                            </optgroup>
                            
                        </select>
                    </div>
                    <div id="Jauge_Options_${z}">
                        <div>
                            <label for="Secteurs_${z}">Nombre de secteurs : </label>
                            <select id="Secteurs_${z}" onchange="ChoixNbSecteursJauge(this)">
                                <option value="6">6</option>
                                <option value="5">5</option>
                                <option value="4">4</option>
                                <option value="3">3</option>
                                <option value="2">2</option>
                                <option value="1">1</option>
                            </select>
                        </div>
                        <div>
                            <label for="ValMin_${z}">Valeur Min : </label>
                            <input type="number" id="ValMin_${z}" value="${ListeZones[z].para.ValMin}" onchange="ListeZones[${z}].para.ValMin=this.value;afficher_MenuJauge(${z})">
                        </div> 
                        <div>
                            <label for="ValMax_${z}">Valeur Max : </label>
                            <input type="number" id="ValMax_${z}" value="${ListeZones[z].para.ValMax}" onchange="ListeZones[${z}].para.ValMax=this.value;afficher_MenuJauge(${z})">
                        </div>
                    </div>              
                </div>
                <div id="Jauge_DefSecteurs_${z}" class="hom-tab-Jauge"></div>`;

    GH("Jauge_Menu_" + z, Menu);

    // Liste des sources de puissance
    let LesPuissances = "";
    for (let s = 0; s < C.LaSource.length; s++) {
        for (let canal = 0; canal < C.LaSource[s].CanalNom.length; canal++) {
            if (C.LaSource[s].CanalNom[canal] != "") {
                let Nom = C.LaSource[s].CanalNom[canal];
                LesPuissances += `<option value="puissance_${s}_${canal}" >⚡${Nom}</option>`;
            }
        }
    }
    GH(`groupePuissance_${z}`, LesPuissances);
    // Liste des canaux de température
    let LesTemperatures = "";
    for (let canal = 0; canal < C.LesTemperatures.length; canal++) {
        if (C.LesTemperatures[canal].Source_Temp != "tempNo") {
            let Nom = C.LesTemperatures[canal].nomTemperature;
            LesTemperatures += `<option value="temperature_${canal}" >🌡️ ${Nom}</option>`;
        }
    }
    if (LesTemperatures != "") GH(`groupeTemperature_${z}`, LesTemperatures);
    // Liste des actionneurs.
    let LesActions = "";
    let JSD = C.LesActions;
    let numAction = 0;
    for (i = 0; i < JSD.blocs.length; i++) {
        if (JSD.blocs[i].type == "action") {
            let nom = JSD.blocs[i].Para.Nom;
            LesActions += `<option value="action_${numAction}" >🚪${nom}</option>`;
            numAction++;
        }
    }
    if (LesActions != "") GH(`groupeOuverture_${z}`, LesActions);

    //Les Secteurs de la jauge
    GID("Secteurs_" + z).value = ListeZones[z].para.nbSecteurs;
    let defSecteurs = "<table>";
    defSecteurs += `<tr><td>Couleur </td><td>Largeur °</td><td class="val-seuilJ">${ListeZones[z].para.ValMin}</td></tr>`;
    for (let i = 0; i < ListeZones[z].para.nbSecteurs; i++) {
        let ValSeuil = Math.round(ListeZones[z].para.ValMin + (ListeZones[z].para.ValMax - ListeZones[z].para.ValMin) * (ListeZones[z].para.Angles[i + 1] + 90) / 180);
        defSecteurs += `<tr>
                            <td><input type="color" id="Couleur_${z}_${i}" value="${ListeZones[z].para.Couleurs[i]}" onchange="SetColorJ(this)"></td>
                            <td><input type="number" id="Angle_${z}_${i}" value="${ListeZones[z].para.Angles[i + 1] - ListeZones[z].para.Angles[i]}" onchange="SetAngleJ(this)"></td>
                            <td class="val-seuilJ">${ValSeuil}</td>   
                        </tr>`;
    }
    defSecteurs += "</table>";
    GH("Jauge_DefSecteurs_" + z, defSecteurs);
    GID(`Angle_${z}_${ListeZones[z].para.nbSecteurs - 1}`).disabled = true; //Inactif le dernier input

    if (ListeZones[z].para.RefJauge != null) {
        GID(`CapteurJ_${z}`).value = ListeZones[z].para.RefJauge;
        let LaRef = ListeZones[z].para.RefJauge.split("_")[0];
        if (LaRef == "action" || LaRef == "soleil") {
            GID(`ValMin_${z}`).disabled = true;
            GID(`ValMax_${z}`).disabled = true;
        }
        if (LaRef == "tarif") {
            GID("Jauge_DefSecteurs_" + z).style.display = "none";
            GID(`Jauge_Options_${z}`).style.display = "none";
        }
    }

    TraceJauge(z);
}
function ChoixJauge(selectElement) {
    let RefJauge = selectElement.value;
    if (!RefJauge) return;
    let z = selectElement.id.split("_")[1];
    ListeZones[z].para.RefJauge = RefJauge;
    LesRef = RefJauge.split("_");
    switch (LesRef[0]) {
        case "puissance":
            ListeZones[z].para.ValMin = -4000;
            ListeZones[z].para.ValMax = 4000; // Exemple de valeur maximale pour la température    
            ListeZones[z].para.nbSecteurs = 6; // Exemple de nombre de secteurs pour la température
            ListeZones[z].para.Angles = [-90, -60, -30, 0, 30, 60, 90]; // Exemple d'angles pour 6 secteurs
            ListeZones[z].para.Couleurs = ["#2196F3", "#00BCD4", "#4CAF50", "#FFEB3B", "#FF9800", "#F44336"];
            ListeZones[z].para.Valeur = 0;
            break;
        case "temperature":
            ListeZones[z].para.ValMin = 0;
            ListeZones[z].para.ValMax = 60; // Exemple de valeur maximale pour la température    
            ListeZones[z].para.nbSecteurs = 6; // Exemple de nombre de secteurs pour la température
            ListeZones[z].para.Angles = [-90, -60, -30, 0, 30, 60, 90]; // Exemple d'angles pour 6 secteurs
            ListeZones[z].para.Couleurs = ["#2196F3", "#00BCD4", "#4CAF50", "#FFEB3B", "#FF9800", "#F44336"];
            ListeZones[z].para.Valeur = 0;
            break;
        case "action":
            ListeZones[z].para.ValMin = 0;
            ListeZones[z].para.ValMax = 100; // Exemple de valeur maximale pour l'action
            ListeZones[z].para.nbSecteurs = 4; // Exemple de nombre de secteurs pour l'action
            ListeZones[z].para.Angles = [-90, -45, 0, 45, 90]; // Exemple d'angles pour 4 secteurs
            ListeZones[z].para.Couleurs = ["#2196F3", "#4CAF50", "#FFEB3B", "#FF9800"];
            ListeZones[z].para.Valeur = 0;
            break;
        case "soleil":
            ListeZones[z].para.ValMin = 0;
            ListeZones[z].para.ValMax = 1000; // Valeur Max ensoleillement
            ListeZones[z].para.nbSecteurs = 4; // Exemple de nombre de secteurs pour l'action
            ListeZones[z].para.Angles = [-90, -45, 0, 45, 90]; // Exemple d'angles pour 4 secteurs
            ListeZones[z].para.Couleurs = ["#2196F3", "#4CAF50", "#FFEB3B", "#FF9800"];
            ListeZones[z].para.Valeur = 0;
            break;
        case "tarif":
            switch (LesRef[1]) {
                case "tempo":
                    ListeZones[z].para.ValMin = 0;
                    ListeZones[z].para.ValMax = 1; // Valeur Max pour tempo
                    ListeZones[z].para.nbSecteurs = 3;
                    ListeZones[z].para.Angles = [-90, -30, 30, 90]; // Exemple d'angles pour 3 secteurs
                    ListeZones[z].para.Couleurs = ["#0000ff", "#ffffff", "#FF0000"];
                    ListeZones[z].para.Valeur = -1;//inconnuu
                    break;
                case "HPHC":
                    ListeZones[z].para.ValMin = 0;
                    ListeZones[z].para.ValMax = 1; // Valeur Max ensoleillement
                    ListeZones[z].para.nbSecteurs = 2; // Exemple de nombre de secteurs pour l'action
                    ListeZones[z].para.Angles = [-90, 0, 90]; // Exemple d'angles pour 2 secteurs
                    ListeZones[z].para.Couleurs = ["#00ff00", "#ff0000"];
                    ListeZones[z].para.Valeur = -1; //inconnu
                    break;
            }
    }

    afficher_MenuJauge(z);
    LoadAppelDonneesImmediat(); //Forcer un envoi des données lentes
}
function ChoixNbSecteursJauge(selectElement) {
    let z = selectElement.id.split("_")[1];
    let nbSecteurs = parseInt(selectElement.value);
    ListeZones[z].para.nbSecteurs = nbSecteurs;
    // Mettre à jour les couleurs et les angles en fonction du nombre de secteurs
    ListeZones[z].para.Angles = []; //On vide pour reconstituer dans le menu après
    ListeZones[z].para.Couleurs = [];
    for (let i = 0; i < nbSecteurs; i++) {
         const hue = (600-360*i/nbSecteurs)%360; //Roue des couleurs
         let couleur = DefCouleur(hue, 60);
        ListeZones[z].para.Couleurs.push(couleur);
        ListeZones[z].para.Angles.push(-90 + i * 180 / nbSecteurs);
    }
    ListeZones[z].para.Angles.push(90);
   
    afficher_MenuJauge(z);
}
function SetAngleJ(inputElement) {
    let ids = inputElement.id.split("_");
    let z = parseInt(ids[1]);
    let i = parseInt(ids[2]);
    let newAngle = clamp(parseInt(inputElement.value), 0, 178);
    ListeZones[z].para.Angles[i + 1] = ListeZones[z].para.Angles[i] + newAngle;
    if (i <= ListeZones[z].para.Angles.length - 3) {
        ListeZones[z].para.Angles[i + 1] = Math.min(ListeZones[z].para.Angles[i + 1], ListeZones[z].para.Angles[i + 2] - 2);
    } else if (i == ListeZones[z].para.Angles.length - 2) {
        ListeZones[z].para.Angles[i + 1] = Math.min(ListeZones[z].para.Angles[i + 1], 90); //2 deg minimum
    }
    ListeZones[z].para.Angles[ListeZones[z].para.Angles.length - 1] = 90;
    afficher_MenuJauge(z);
}
function SetColorJ(inputElement) {
    let ids = inputElement.id.split("_");
    let z = parseInt(ids[1]);
    let i = parseInt(ids[2]);
    ListeZones[z].para.Couleurs[i] = inputElement.value;
    TraceJauge(z);
}


function polar(cx, cy, r, angle) {
    let rad = (angle - 90) * Math.PI / 180
    return {
        x: cx + r * Math.cos(rad),
        y: cy + r * Math.sin(rad)
    }
}

function arc(start, end) {

    let s = polar(cx, cy, r, end)
    let e = polar(cx, cy, r, start)

    return `M ${s.x} ${s.y} A ${r} ${r} 0 0 0 ${e.x} ${e.y}`
}


function traceLesJauges(msg) {
    for (let z = 0; z < ListeZones.length; z++) {
        if (ListeZones[z].type === "graphJauge") {
            let RefJauge = ListeZones[z].para.RefJauge;
            if (RefJauge) {
                let lesRef = ListeZones[z].para.RefJauge.split("_");
                switch (lesRef[0]) {
                    case "puissance":
                        // Ajouter le code pour gérer le cas du capteur
                        if (msg.source && msg.canal) {
                            for (let i = 0; i < msg.canal.length; i++) {
                                let s = msg.source[i];
                                let c = msg.canal[i];
                                if (s == lesRef[1] && c == lesRef[2]) {
                                    LesNomsJ[z] = C.LaSource[s].CanalNom[c];
                                    ListeZones[z].para.Valeur = parseInt(msg.PwS[i]) - parseInt(msg.PwI[i]);
                                }
                            }
                        }
                        break;
                    case "temperature":
                        if (msg.Temperatures) {
                            LesNomsJ[z] = C.LesTemperatures[lesRef[1]].nomTemperature;
                            ListeZones[z].para.Valeur = msg.Temperatures[lesRef[1]];
                        }
                        break;
                    case "action":
                        if (msg.lesActions) {
                            LesNomsJ[z] = msg.lesActions.nom[lesRef[1]];
                            ListeZones[z].para.Valeur = msg.lesActions.ouverture[lesRef[1]];
                        }
                        break;
                    case "soleil":
                        if (Soleil != null) {
                            switch (lesRef[1]) {
                                case "actuel":
                                    LesNomsJ[z] = "Ensoleillement actuel";
                                    ListeZones[z].para.Valeur = Soleil.Wm2[Soleil.Int_Heure];
                                    break;
                                case "jourJ":
                                    LesNomsJ[z] = "Ensoleillement jour J";
                                    ListeZones[z].para.Valeur = Soleil.SoleilJour;
                                    break;
                                case "demain":
                                    LesNomsJ[z] = "Ensoleillement jour J+1";
                                    ListeZones[z].para.Valeur = Soleil.SoleilDemain;
                                    break;
                            }

                        }

                        break;
                    case "tarif":
                        if (msg.LTARF ) {
                            if ( lesRef[1] == "HPHC") {
                                if (msg.LTARF.indexOf("PLEINE") >= 0) ListeZones[z].para.Valeur = 0.75;
                                if (msg.LTARF.indexOf("CREUSE") >= 0 && lesRef[1] == "HPHC") ListeZones[z].para.Valeur = 0.25;
                                LesNomsJ[z] = "Tarif HP/HC";
                            }
                            if ( lesRef[1] == "tempo" && lesRef[2] == "J") {
                                LesNomsJ[z] = "Tarif Tempo jour J";
                                if (msg.LTARF.indexOf("BLEU") >= 0 && lesRef[1] == "tempo") ListeZones[z].para.Valeur = 0.166;
                                if (msg.LTARF.indexOf("BLANC") >= 0 && lesRef[1] == "tempo") ListeZones[z].para.Valeur = 0;
                                if (msg.LTARF.indexOf("ROUGE") >= 0 && lesRef[1] == "tempo") ListeZones[z].para.Valeur = 0.836;
                            }
                        }
                        if(msg.TempoLendemain && lesRef[1] == "tempo" && lesRef[2] == "J1") {
                            LesNomsJ[z] = "Tarif Tempo jour J+1";
                            if (msg.TempoLendemain.indexOf("BLEU") >= 0) ListeZones[z].para.Valeur = 0.166;
                            if (msg.TempoLendemain.indexOf("BLANC") >= 0) ListeZones[z].para.Valeur = 0;
                            if (msg.TempoLendemain.indexOf("ROUGE") >= 0) ListeZones[z].para.Valeur = 0.833;
                        }
                        break;
                    default:
                        break;
                }

            }
            TraceJauge(z);
        }
    }
}

function TraceJauge(z) { //Valeur 
    //Le SVG
    let J_SVG = "";
    for (let i = 1; i <= ListeZones[z].para.nbSecteurs; i++) {
        J_SVG += `<path id="z${i}_${z}" class="zoneJauge" stroke="blue" />`;
    }
    J_SVG += `<defs>
                <filter id="dropShadow" x="-50%" y="-50%" width="200%" height="200%">
                    <feDropShadow dx="0" dy="0" stdDeviation="3" flood-color="black" flood-opacity="0.7"/>
                </filter>
              </defs>`;

    J_SVG += `<polygon id="aiguilleG_${z}" class="aiguilleG" filter="url(#dropShadow)" />
              <text id="titreJ_${z}" x="5" y="20" font-size="18" text-anchor="left" fill="white" style="text-shadow: 0 1px 3px rgba(0, 0, 0, 0.7);"></text>
              <text id="valeurJ_${z}" x="${cx}" y="${cy}" font-size="54" text-anchor="middle" fill="white" style="text-shadow: 0 1px 3px rgba(0, 0, 0, 0.7);">---</text>`;

    GH("Jauge_SVG_" + z, J_SVG);
    let x1, y1, x2, y2, x3, y3;
    for (i = 1; i <= ListeZones[z].para.nbSecteurs; i++) {
        GID(`z${i}_${z}`).setAttribute("d", arc(ListeZones[z].para.Angles[i - 1], ListeZones[z].para.Angles[i]));
        GID(`z${i}_${z}`).setAttribute("stroke", ListeZones[z].para.Couleurs[i - 1]);
    }
    GID(`titreJ_${z}`).textContent = LesNomsJ[z];
    let ValeurClampe = clamp(ListeZones[z].para.Valeur, ListeZones[z].para.ValMin, ListeZones[z].para.ValMax);
    let angle = -90 + (ValeurClampe - ListeZones[z].para.ValMin) / (ListeZones[z].para.ValMax - ListeZones[z].para.ValMin) * 180;
    let p = polar(cx, cy, 80, angle - 5);
    x1 = p.x;
    y1 = p.y;
    p = polar(cx, cy, 150, angle);
    x2 = p.x;
    y2 = p.y;
    p = polar(cx, cy, 80, angle + 5);
    x3 = p.x;
    y3 = p.y;
    let aig = GID(`aiguilleG_${z}`);
    aig.setAttribute("points", `${x1},${y1} ${x2},${y2} ${x3},${y3}`);


    let displayValue = "---";
    let Unit = "";
    let RefJauge = ListeZones[z].para.RefJauge;
    if (RefJauge) {
        let lesRef = ListeZones[z].para.RefJauge.split("_");
        switch (lesRef[0]) {
            case "puissance":
                displayValue = Math.round(ListeZones[z].para.Valeur);
                Unit = " W";
                break;
            case "temperature":
                displayValue = ListeZones[z].para.Valeur.toFixed(1);
                Unit = " °C";
                break;
            case "action":
                displayValue = ListeZones[z].para.Valeur;
                Unit = " %";
                break;
            case "soleil":
                displayValue = ListeZones[z].para.Valeur;
                Unit = " W/m²";
                break;
            case "tarif":
                displayValue = "---";
                if (ListeZones[z].para.Valeur==0.166) displayValue="🔵";
                if (ListeZones[z].para.Valeur==0.25) displayValue="🟢";
                if (ListeZones[z].para.Valeur==0) displayValue="⚪";
                if (ListeZones[z].para.Valeur>=0.75) displayValue="🔴";
                Unit = "";
                break;
        }
    }
    GID(`valeurJ_${z}`).textContent = displayValue + Unit;
}

