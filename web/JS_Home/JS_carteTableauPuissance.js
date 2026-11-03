function createCarteTableauPuissance(z) {
    let contenu = "<div id='Tableau_Puissance_" + z + "'class='hom-LeTableau'><span class='hom-card-title'>Attente données de puissance</span></div>";
    if (ListeZones[z].para == null) {

        let para = {
            ligne: [true, true, true, true, true],
            colonne: [true, true, true, true, true, true, true, true, true, true, true, true],
            nbColonneMax: 1
        };
        ListeZones[z].para = para;
    }
    ListeZones[z].finEdit = function (z) { afficherLC_Tableau(z); };
    ListeZones[z].debutEdit = function (z) { afficherLC_Tableau(z); };

    return contenu;

}

function traceLesTableauxPuissance(msg) {
    for (let zone = 0; zone < ListeZones.length; zone++) {
        if (ListeZones[zone].type == "tableauPuissance") MessageSources_TableauPuissance(msg, zone);
    }
}

function MessageSources_TableauPuissance(ValSources, z) {
    let para = ListeZones[z].para;
    if (!ListeZones[z].init) {


        ListeZones[z].para.nbColonneMax = ValSources.canal.length;
        FormatColonneTableau(z, ValSources.canal.length);
        let colTitre = '<div class="grid-contColTitre">';
        colTitre += '<div class="grid-Titre1">&nbsp;<div class="hom-tempo" id="couleurTarif_jour_' + z + '"></div></div><div class="grid-Titre1">&nbsp;<div class="hom-tempo" id="couleurTarif_lendemain_' + z + '"></div></div>';
        colTitre += `<div class="W L0" ><input type="checkbox" class="showInline edit" id="chkL_0_${z}" onchange="setLigne(this);">Puissance Active</div>`;
        colTitre += `<div class="VA L1"><input type="checkbox" class="showInline edit" id="chkL_1_${z}" onchange="setLigne(this);">Puissance Apparente</div>`;
        colTitre += `<div class="W L2"><input type="checkbox" class="showInline edit" id="chkL_2_${z}" onchange="setLigne(this);">Puissance Active Max du jour</div>`;
        colTitre += `<div class="Wh L3"><input type="checkbox" class="showInline edit" id="chkL_3_${z}" onchange="setLigne(this);">Energie Active du jour</div>`;
        colTitre += `<div class="Wh L4"><input type="checkbox" class="showInline edit" id="chkL_4_${z}" onchange="setLigne(this);">Energie Active Totale</div>`;
        colTitre += '</div>';


        let colSource = "";

        for (let i = 0; i < ValSources.canal.length; i++) {
            let SI = 0;
            let s = ValSources.source[i];
            let c = ValSources.canal[i];
            let CanalNom = C.LaSource[s].CanalNom[c];
            let NomSoutir = C.LaSource[s].NomSoutir[c];
            let NomInject = C.LaSource[s].NomInject[c];
            if (NomSoutir != "") SI++;
            if (NomInject != "") SI++;

            if (SI == 1) {
                colSource += `<div class="grid-contColCanal grid-Canal_SouI  C${i}">`; // colonne soutir ou Inject
                colSource += `<div  class="grid-NomCanal-1 C${i}"><input type="checkbox" class="showInline edit" id="chkC_${i}_${z}" onchange="setColonne(this);">${CanalNom}</div>`;
                colSource += `<div  class="grid-NomCanal-1 C${i}">` + NomSoutir + NomInject + `</div>`;
            } else {
                colSource += `<div class="grid-contColCanal grid-Canal_SetI  C${i}">`; // colonne soutir et Inject
                colSource += `<div  class="grid-NomCanal-2 C${i}"><input type="checkbox" class="showInline edit" id="chkC_${i}_${z}" onchange="setColonne(this);">${CanalNom}</div>`;
                colSource += `<div  class="grid-NomCanal-1 C${i}">` + NomSoutir + `</div>`; //Peut être un pb d'ordre ????
                colSource += `<div  class="grid-NomCanal-1 C${i}">` + NomInject + `</div>`;
            }

            let sc = String(s) + String(c);

            colSource += `<div class="W R L0 C${i}" id="z${z}Pw${sc}C1">0</div>`;
            if (SI == 2) colSource += `<div class="W R L0 C${i}" id="z${z}Pw${sc}C2">0</div>`;
            colSource += `<div class="VA R L1 C${i}" id="z${z}Pva${sc}C1">0</div>`;
            if (SI == 2) colSource += `<div class="VA R L1 C${i}" id="z${z}Pva${sc}C2">0</div>`;
            colSource += `<div class="W R L2 C${i}" id="z${z}Pwm${sc}C1">0</div>`;
            if (SI == 2) colSource += `<div class="W R L2 C${i}" id="z${z}Pwm${sc}C2">0</div>`;
            colSource += `<div class="Wh R L3 C${i}" id="z${z}EnJ${sc}C1">0</div>`;
            if (SI == 2) colSource += `<div class="Wh R L3 C${i}" id="z${z}EnJ${sc}C2">0</div>`;
            colSource += `<div class="Wh R L4 C${i}" id="z${z}EnT${sc}C1">0</div>`;
            if (SI == 2) colSource += `<div class="Wh R L4 C${i}" id="z${z}EnT${sc}C2">0</div>`;
            colSource += '</div>';

        }

        let colUnite = '<div class="grid-contColUnite">';
        colUnite += '<div >&nbsp;</div><div >&nbsp;</div>';//A completer   
        colUnite += `<div class="W L0">W</div>`;
        colUnite += `<div class="VA L1">VA</div>`;
        colUnite += `<div class="W L2">W</div>`;
        colUnite += `<div class="Wh L3">Wh</div>`;
        colUnite += `<div class="Wh L4">Wh</div>`;
        colUnite += '</div>';
        for (let i = ValSources.canal.length; i < para.colonne.length; i++) {
            para.colonne[i] = false; // Masquer les colonnes supplémentaires par défaut
        }

        GH("Tableau_Puissance_" + z, colTitre + colSource + colUnite);

        afficherLC_Tableau(z)
        ListeZones[z].init = true;
    }


    for (let i = 0; i < ValSources.canal.length; i++) {
        let SI = 0;
        let s = ValSources.source[i];
        let c = ValSources.canal[i];
        let sc = String(s) + String(c);
        let NomSoutir = C.LaSource[s].NomSoutir[c];
        let NomInject = C.LaSource[s].NomInject[c];
        if (NomSoutir != "") SI++;
        if (NomInject != "") SI++;

        if (SI == 1) {
            if (NomInject != "") { //On a que des valeurs colonne Inject
                GH("z" + z + "Pw" + sc + "C1", LaVal(ValSources.PwI[i])); // en première colonne on affiche injecté
                GH("z" + z + "Pva" + sc + "C1", LaVal(ValSources.PVAI[i]));
                GH("z" + z + "Pwm" + sc + "C1", LaVal(ValSources.PwI_Max[i]));
                GH("z" + z + "EnJ" + sc + "C1", LaVal(ValSources.EnJI[i]));
                GH("z" + z + "EnT" + sc + "C1", LaVal(ValSources.EnTI[i]));

            } else { //On a que des valeurs colonne soutiré
                GH("z" + z + "Pw" + sc + "C1", LaVal(ValSources.PwS[i])); // en première colonne on affiche  soutiré 
                GH("z" + z + "Pva" + sc + "C1", LaVal(ValSources.PVAS[i]));
                GH("z" + z + "Pwm" + sc + "C1", LaVal(ValSources.PwS_Max[i]));
                GH("z" + z + "EnJ" + sc + "C1", LaVal(ValSources.EnJS[i]));
                GH("z" + z + "EnT" + sc + "C1", LaVal(ValSources.EnTS[i]));
            }

        } else {

            GH("z" + z + "Pw" + sc + "C1", LaVal(ValSources.PwS[i])); // en première colonne on affiche  soutiré 
            GH("z" + z + "Pva" + sc + "C1", LaVal(ValSources.PVAS[i]));
            GH("z" + z + "Pwm" + sc + "C1", LaVal(ValSources.PwS_Max[i]));
            GH("z" + z + "EnJ" + sc + "C1", LaVal(ValSources.EnJS[i]));
            GH("z" + z + "EnT" + sc + "C1", LaVal(ValSources.EnTS[i]));

            GH("z" + z + "Pw" + sc + "C2", LaVal(ValSources.PwI[i])); // en première colonne on affiche injecté
            GH("z" + z + "Pva" + sc + "C2", LaVal(ValSources.PVAI[i]));
            GH("z" + z + "Pwm" + sc + "C2", LaVal(ValSources.PwI_Max[i]));
            GH("z" + z + "EnJ" + sc + "C2", LaVal(ValSources.EnJI[i]));
            GH("z" + z + "EnT" + sc + "C2", LaVal(ValSources.EnTI[i]));
        }
    }



}
function FormatColonneTableau(z, nbColonnes) {
    let template = "auto ";
    for (let i = 0; i < nbColonnes; i++) {
        template += "auto ";
    }
    template += "80px";
    GID("Tableau_Puissance_" + z).style.gridTemplateColumns = template;
    let fs = 120;
    if (nbColonnes > 2) {
        fs = 60;
    }
    if (nbColonnes > 4) {
        fs = 50;
    }
    const card = ListeZones[z].carte;
    let span = parseInt(card.dataset.span, 10);
    if (span == 1) fs = Math.min(fs, 60);
    GID("Tableau_Puissance_" + z).style.fontSize = fs + "%"; //A ajuster si besoin en fonction du nombre de canaux et de la taille de l'écran
}
function afficherLC_Tableau(z) { //Affichage Ligne et colonnes
    let para = ListeZones[z].para;

    let nbColVisible = 0;
    for (let indiceColonne = 0; indiceColonne < para.colonne.length; indiceColonne++) {
        document.querySelectorAll(
            `#Tableau_Puissance_${z} .C${indiceColonne}`
        ).forEach(cellule => {
            if (ModeEdit) {
                cellule.style.filter = para.colonne[indiceColonne] ? 'opacity(1)' : 'opacity(0.4)';
                cellule.style.display = '';
            } else {
                cellule.style.display = para.colonne[indiceColonne] ? '' : 'none';
            }
        });
        if (GID("chkC_" + indiceColonne + "_" + z)) {
            GID("chkC_" + indiceColonne + "_" + z).checked = para.colonne[indiceColonne];
        }
        if (para.colonne[indiceColonne]) {
            nbColVisible++;
        }
    }

    for (let indiceLigne = 0; indiceLigne < para.ligne.length; indiceLigne++) {
        document.querySelectorAll(
            `#Tableau_Puissance_${z} .L${indiceLigne}`
        ).forEach(cellule => {
            if (ModeEdit) {
                cellule.style.filter = para.ligne[indiceLigne] ? 'opacity(1)' : 'opacity(0.4)';
                cellule.style.display = '';
            } else {
                cellule.style.display = para.ligne[indiceLigne] ? '' : 'none';
            }
        });

        if (GID("chkL_" + indiceLigne + "_" + z)) {
            GID("chkL_" + indiceLigne + "_" + z).checked = para.ligne[indiceLigne];
        }
    }
    let nb_ColonneEnCours = ModeEdit ? ListeZones[z].para.nbColonneMax : nbColVisible;
    FormatColonneTableau(z, nb_ColonneEnCours);
    const elements = document.querySelectorAll(`#Tableau_Puissance_${z} .edit`);
    elements.forEach(el => {
        el.style.display = ModeEdit ? '' : 'none';
    });

}
function setLigne(checkbox) {
    const parts = checkbox.id.split('_');
    const indiceLigne = parseInt(parts[1], 10);
    const z = parseInt(parts[2], 10);
    ListeZones[z].para.ligne[indiceLigne] = checkbox.checked;
    afficherLC_Tableau(z);
}
function setColonne(checkbox) {
    const parts = checkbox.id.split('_');
    const indiceColonne = parseInt(parts[1], 10);
    const z = parseInt(parts[2], 10);
    ListeZones[z].para.colonne[indiceColonne] = checkbox.checked;
    afficherLC_Tableau(z);
}
function traceDonneesLentesTableauPuissance(msg) {
    for (let zone = 0; zone < ListeZones.length; zone++) {
        if (ListeZones[zone].type == "tableauPuissance") MessageDonneesLentes_TableauPuissance(msg, zone);
    }
}
function MessageDonneesLentes_TableauPuissance(msg, z) {
    if (ListeZones[z].init) {
        let Tarif = ["NON_DEFINI", "PLEINE", "CREUSE", "BLEU", "BLANC", "ROUGE"];
        let couleur = ["#444", "#f00", "#0f0", "#00bfff", "#fff", "#f00"];
        let tarif = ["", "H.<br>pleine", "H.<br>creuse", "Tempo<br>Bleu", "Tempo<br>Blanc", "Tempo<br>Rouge"];
        let idx = -1;


        for (let i = 0; i < 6; i++) {
            if (msg.LTARF.indexOf(Tarif[i]) >= 0) { //LTARF dans Linky et couleur Tempo si applicable
                idx = i;
                break;
            }
        }
        if(idx>=0) {
            GID('couleurTarif_jour_' + z).style.backgroundColor = couleur[idx];
            GID('couleurTarif_jour_' + z).innerHTML = "<br>" + tarif[idx];
        }

        idx = -1;
        
        for (let i = 3; i < 6; i++) {
            if (msg.TempoLendemain.indexOf(Tarif[i]) >= 0) { // couleur Tempo si applicable
                idx = i;
                break;
            }
        }
        if(idx>=0) {
            GID('couleurTarif_lendemain_' + z).style.backgroundColor = couleur[idx];
            GID('couleurTarif_lendemain_' + z).innerHTML = "<br>Tempo<br>J+1";
        }
    }
}