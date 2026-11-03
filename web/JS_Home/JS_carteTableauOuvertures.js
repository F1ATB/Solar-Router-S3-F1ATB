function createCarteTableauOuvertures(z) {
    let contenu = "<div id='Tableau_Ouvertures_" + z + "' ><span class='hom-card-title'>Attente données des ouvertures</span></div>";


    return contenu;

}

function traceLesTableauxOuvertures(msg) {
    for (let zone = 0; zone < ListeZones.length; zone++) {
        if (ListeZones[zone].type == "tableauOuvertures") MessageSources_TableauOuvertures(msg, zone);
    }
}

function MessageSources_TableauOuvertures(msg, z) {

    const para_ = ListeZones[z].para;
    if (!ListeZones[z].init) {
        if (para_ == null) {
            visible = [];
            for (let i = 0; i < msg.lesActions.ouverture.length; i++) {
                visible.push(true);
            }
            let para = {
                LigneVisible: visible
            };
            ListeZones[z].para = para;
        }
        let para = ListeZones[z].para;

        let LesAct = '<table class="hom-Tableau_Ouvertures">';
        LesAct += '<tr><th colspan="2">Etat Action(s)</th><th>H<div class="fsize10">ouverture équivalente</div></th><th colspan="3">Forçage</th></tr>';
        for (let i = 0; i < msg.lesActions.ouverture.length; i++) {
            let chk = para.LigneVisible[i] ? "checked" : "";
            LesAct += '<tr><td class="L"><input  type="checkbox" class="edit" id="TabOuv_' + z + '_' + i + '"  onclick="setTabOuv(this)" ' + chk + '>';
            LesAct += ' ' + msg.lesActions.nom[i] + ' </td>';
            LesAct += '<td class="R" style="width:20%" id="TabOuv_' + z + '_' + i + 'ouv"> </td>';
            LesAct += '<td class="C" id="TabOuv_' + z + '_' + i + 'H"></td>';
            LesAct += '<td class="C"><input type="button" value="ON" onclick="ForceAction(1, ' + i + ')"></td>';
            LesAct += '<td class="C" ><span id="TabOuv_' + z + '_' + i + 'T"></span></td>';
            LesAct += '<td class="C"><input type="button" value="OFF" onclick="ForceAction(-1, ' + i + ')"></td>';
            LesAct += ' </tr>';
        }
        LesAct += '</table>';
        GH("Tableau_Ouvertures_" + z, LesAct);

    }

    ListeZones[z].init = true;
    for (let i = 0; i < msg.lesActions.ouverture.length; i++) {
        GH("TabOuv_" + z + "_" + i + "ouv", msg.lesActions.ouverture[i] + '%');
        GH("TabOuv_" + z + "_" + i + "H", Hdeci2Hmn(100 * msg.lesActions.Hequiv[i]));
        const T = msg.lesActions.tOnOff[i];
        if (T != 0) {
            GH("TabOuv_" + z + "_" + i + "T", T + " mn");
        } else {
            GH("TabOuv_" + z + "_" + i + "T", "");
        }
    }
}

// A COMPLETER POUR LE MASQUAGE
function ouvreTabAffiche(z) { //AFAIRE
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
}
function setTabOuv(checkbox) {
    const parts = checkbox.id.split('_');
    const indiceLigne = parseInt(parts[2], 10);
    const z = parseInt(parts[1], 10);
    ListeZones[z].para.LigneVisible[indiceLigne] = checkbox.checked;
}
function ForceAction(Force, NumAction) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var retour = this.responseText;
        }

    };

    let adr = adrHttpRemote + "/ajax_forceAction?Force=" + Force + "&NumAction=" + NumAction;

    xhttp.open('GET', adr, true);

    xhttp.send();
}