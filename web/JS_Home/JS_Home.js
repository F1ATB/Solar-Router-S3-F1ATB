
let ModeEdit = false;
const zoneBoutonEdit = document.getElementById('hom-zoneBoutonEdit');
const editer = document.getElementById('editer');
const zonesContainer = document.getElementById('hom-zonesContainer');
ListeZones = [];
let nbColonnes = 3; //dans la page

function init() {

    // Initialisation
    LoadConfig();

}
function ImplantationConfig() {
    SetHautBas();
    GH("nom_R", C.nomRouteur);
    setMode('annuler');
    //LesZones = JSON.parse(localStorage.getItem("LesZones") || "{}");
    let LesZones = {};
    if (C.LesZones) {
        LesZones = C.LesZones;
    }
    if (LesZones.nbColonnes) { //On a une configuration
        nbColonnes = LesZones.nbColonnes;
        for (let i = 0; i < LesZones.zone.length; i++) {
            createZone(LesZones.zone[i].type, LesZones.zone[i].para, LesZones.zone[i].span);
        }
    }
    setNbColonnes(nbColonnes);
    if (ListeZones.length == 0) { //On crée une zone tableauPuissance
        createZone("tableauPuissance", null, nbColonnes);
    }

    setMode('annuler');




    //On est pret à charger les données 
    LoadAppelDonneesImmediat();
}
//======= Gestion des Cartes =========
function setMode(mode) {
    ModeEdit = false;
    if (mode === 'editer') ModeEdit = true;
    editer.style.display = ModeEdit ? 'none' : 'inline';

    const elements = document.querySelectorAll('.edit');
    elements.forEach(el => {
        el.style.display = ModeEdit ? '' : 'none';
    });
    const elements2 = document.querySelectorAll('.hom-card');
    elements2.forEach(el => {
        el.style.cursor = ModeEdit ? 'grab' : 'auto';
        el.style.touchAction = ModeEdit ? 'none' : 'auto';
    });
    const elements3 = document.querySelectorAll('.hom-SoleilSVG');
    elements3.forEach(el => {
        el.style.touchAction = ModeEdit ? 'none' : 'auto';
    });
    const elements4 = document.querySelectorAll('.hom-graphEnSVG');
    elements4.forEach(el => {
        el.style.touchAction = ModeEdit ? 'none' : 'auto';
    });
    const elements5 = document.querySelectorAll('.hom-graphPwSVG');
    elements5.forEach(el => {
        el.style.touchAction = ModeEdit ? 'none' : 'auto';
    });


    for (let z = 0; z < ListeZones.length; z++) {
        if (ListeZones[z].finEdit != null && ListeZones[z].type != null && !ModeEdit) {
            ListeZones[z].finEdit(z);
        }
        if (ListeZones[z].debutEdit != null && ListeZones[z].type != null && ModeEdit) {
            ListeZones[z].debutEdit(z);
        }
    }
    if (mode === 'valider') {
        const LesZones = { zone: [], nbColonnes: nbColonnes };
        //On enregitre les cartes valides dans l'ordre à l'écran
        const cards = Array.from(zonesContainer.children);
        cards.forEach(card => {
            const zOrigine = parseInt(card.dataset.zOrigine, 10);
            const span = parseInt(card.dataset.span, 10);
            LesZones.zone.push({ type: ListeZones[zOrigine].type, para: ListeZones[zOrigine].para, span: span });
        });


        const jsonData = JSON.stringify(LesZones);
        let adr = adrHttpRemote + "/ajaxNewConfHome";

        fetch(adr, {
            method: "POST",                     // méthode HTTP
            headers: {
                "Content-Type": "application/json" // indique qu'on envoie du JSON
            },

            body: jsonData          // conversion de l'objet JS en texte JSON

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
function setNbColonnes(nb) {
    nbColonnes = nb;
    document.querySelector('#nbColumn').value = nbColonnes;
    const el = document.querySelector('#hom-zonesContainer');
    el.style.gridTemplateColumns = `repeat(${nbColonnes}, 1fr)`;
    for (let i = 0; i < ListeZones.length; i++) {
        if (ListeZones[i].type != null) setLargeur(i, 0); // Ajuste la largeur sans changer le delta
    }

}
function setLargeur(index, delta) {
    const zone = ListeZones[index];
    if (!zone) return;
    const card = zone.carte;
    let span = parseInt(card.dataset.span, 10);
    span += delta;
    if (span < 1) span = 1;
    if (span > nbColonnes) span = nbColonnes;
    card.dataset.span = span;
    card.className = `hom-card span-${span}`;
    const largeurCarte = document.getElementById(`largeurCarte${index}`);
    if (largeurCarte) {
        largeurCarte.textContent = `${span}/${nbColonnes}`;
    }
}
function addCarte() {
    const typeDeCarte = GID("carte").value;
    createZone(typeDeCarte, null, nbColonnes);
    setMode('editer');//Pour rafraîchir l'affichage des contrôles de largeur
}


// Génération d'une couleur HSL variée retur en Hexa pour les inputs de couleur
function DefCouleur(h, l) {
    l = l / 100;
    const a = Math.min(l, 1 - l);
    const f = n => {
        const k = (n + h / 30) % 12;
        const c = l - a * Math.max(-1, Math.min(k - 3, 9 - k, 1));
        return Math.round(c * 255).toString(16).padStart(2, "0");
    };
    return `#${f(0)}${f(8)}${f(4)}`;
}



function createZone(typeDeCarte, para, span) {
    const card = document.createElement('div');
    const z = ListeZones.length;
    ListeZones.push({ carte: card, index: z, type: typeDeCarte, init: false, para: para, finEdit: null, debutEdit: null });
    card.className = `hom-card span-${span}`;
    card.dataset.span = span;
    card.dataset.zOrigine = z;
    card.draggable = true;
    switch (typeDeCarte) {
        case "tableauPuissance":
            contenuCarte = createCarteTableauPuissance(z);
            break;
        case "tableauOuvertures":
            contenuCarte = createCarteTableauOuvertures(z);
            break;
        case "graphHistorique":
            contenuCarte = createCarteGraphHistorique(z);
            break;
        case "graphEnergie":
            contenuCarte = createCarteGraphEnergie(z);
            break;
        case "graphEnsoleillement":
            contenuCarte = createCarteGraphEnsoleillement(z);
            break;
        case "graphJauge":
            contenuCarte = createCarteGraphJauge(z);
            break;
        default:
            contenuCarte = `<p>Contenu du type de carte : ${typeDeCarte}</p>`;
    }

    card.innerHTML = `
            <div class="hom-boxEdit hom-boxZone  edit">   
                    <div>Zone :  #${z}  </div>
                    <div class="hom-modifLargeur ">
                        <div>Largeur : <span id="largeurCarte${z}">${span}/${nbColonnes}</span></div>
                        <div onclick="setLargeur(${z},-1)">➖</div>
                        <div>↔️</div>
                        <div onclick="setLargeur(${z},1)">➕</div>
                    </div>
                    <div onclick="removeZone(${z})" class="hom-removeZone">X</div>
            </div>
            <div class="hom-card-contenu">
                ${contenuCarte}
            
            </div>
        `;

    addDragAndDropEvents(card);
    zonesContainer.appendChild(card);
    setLargeur(z, 0); // Initialize the width of the new zone

    switch (typeDeCarte) {
        case "tableauPuissance":

            break;
        case "tableauOuvertures":

            break;
        case "graphHistorique":
            LoadHistoriquePuissance();
            break;
        case "graphEnergie":
            LoadEnergies();
            break;
        case "graphEnsoleillement":
            TraceSoleil(z);
            break;
        case "graphJauge":
            afficher_MenuJauge(z);
            break;
        default:

    }
}
function removeZone(z) {
    const card = ListeZones[z].carte;
    if (card && card.parentNode) {
        card.parentNode.removeChild(card);
    }
    ListeZones[z].type = null;
}
// --- LOGIQUE D'INSERTION ET D'ÉCHANGE ---

// Calcule la somme des 'span' de chaque ligne actuelle dans la grille
function getGridRowsState() {
    const cards = Array.from(zonesContainer.children);
    const rows = [];
    let currentRow = [];
    let currentSpanSum = 0;

    cards.forEach(card => {
        const span = parseInt(card.dataset.span, 10);
        if (currentSpanSum + span > nbColonnes) {
            rows.push(currentRow);
            currentRow = [card];
            currentSpanSum = span;
        } else {
            currentRow.push(card);
            currentSpanSum += span;
        }
    });
    if (currentRow.length > 0) {
        rows.push(currentRow);
    }
    return rows;
}

// Gère le placement : Insertion dans l'espace disponible ou Échange
function handleDrop(dragged, target) {
    if (!dragged || !target || dragged === target) return;

    const draggedSpan = parseInt(dragged.dataset.span, 10);
    const rows = getGridRowsState();

    // Trouver la ligne contenant la carte cible
    const targetRow = rows.find(row => row.includes(target));
    if (!targetRow) return;

    // Calculer la largeur occupée sur cette ligne (sans le bloc déplacé s'il était déjà dedans)
    const targetRowSpanSum = targetRow.reduce((sum, card) => {
        return card === dragged ? sum : sum + parseInt(card.dataset.span, 10);
    }, 0);

    // S'il y a de la place sur la ligne cible pour insérer le bloc déplacé :
    if (targetRowSpanSum + draggedSpan <= nbColonnes) {
        // Déterminer si on insère avant ou après la carte cible selon la position du curseur
        target.parentNode.insertBefore(dragged, target.nextSibling);
    } else {
        // Sinon (ligne pleine), procéder à un échange classique
        swapElements(dragged, target);
    }
}

function swapElements(elem1, elem2) {
    const parent = elem1.parentNode;
    const next1 = elem1.nextSibling === elem2 ? elem1 : elem1.nextSibling;
    elem2.parentNode.insertBefore(elem1, elem2);
    parent.insertBefore(elem2, next1);
}

// --- ÉVÉNEMENTS DRAG & DROP (SOURIS & TACTILE) ---
let draggedItem = null;

function addDragAndDropEvents(card) {
    // Souris
    card.addEventListener('dragstart', (e) => {

        if (!ModeEdit) {
            e.preventDefault();
            return;
        }

        draggedItem = card;
        setTimeout(() => card.classList.add('dragging'), 0);
    });

    card.addEventListener('dragend', () => {
        draggedItem = null;
        card.classList.remove('dragging');
        document.querySelectorAll('.card').forEach(c => c.classList.remove('drag-over'));
    });

    card.addEventListener('dragover', (e) => {
        e.preventDefault();
    });

    card.addEventListener('dragenter', (e) => {
        e.preventDefault();
        if (card !== draggedItem) {
            card.classList.add('drag-over');
        }
    });

    card.addEventListener('dragleave', () => {
        card.classList.remove('drag-over');
    });

    card.addEventListener('drop', (e) => {
        e.preventDefault();
        card.classList.remove('drag-over');
        if (draggedItem) {
            handleDrop(draggedItem, card);
        }
    });

    // Tactile (Smartphone)
    card.addEventListener('touchstart', handleTouchStart, { passive: false });
    card.addEventListener('touchmove', handleTouchMove, { passive: false });
    card.addEventListener('touchend', handleTouchEnd);
}

let touchItem = null;
let touchClone = null;
let touchOffsetX = 0;
let touchOffsetY = 0;

function handleTouchStart(e) {
    console.log("Touch start");
    if (!ModeEdit) {
        // e.preventDefault();
        return;
    }
    touchItem = this;
    const touch = e.touches[0];
    const rect = touchItem.getBoundingClientRect();

    touchOffsetX = touch.clientX - rect.left;
    touchOffsetY = touch.clientY - rect.top;

    touchClone = touchItem.cloneNode(true);
    touchClone.classList.add('touch-clone');
    touchClone.style.width = `${rect.width}px`;
    touchClone.style.height = `${rect.height}px`;
    touchClone.style.left = `${touch.clientX - touchOffsetX}px`;
    touchClone.style.top = `${touch.clientY - touchOffsetY}px`;
    document.body.appendChild(touchClone);

    touchItem.classList.add('dragging');
}

function handleTouchMove(e) {
    console.log("Touch move");
    if (!touchItem || !touchClone) return;
    e.preventDefault();

    const touch = e.touches[0];
    touchClone.style.left = `${touch.clientX - touchOffsetX}px`;
    touchClone.style.top = `${touch.clientY - touchOffsetY}px`;

    touchClone.style.display = 'none';
    const elementBelow = document.elementFromPoint(touch.clientX, touch.clientY);
    touchClone.style.display = 'flex';

    document.querySelectorAll('.card').forEach(c => c.classList.remove('drag-over'));

    if (elementBelow) {
        const targetCard = elementBelow.closest('.card');
        if (targetCard && targetCard !== touchItem) {
            targetCard.classList.add('drag-over');
        }
    }
}

function handleTouchEnd(e) {
    if (!touchItem) return;

    if (touchClone) {
        const touch = e.changedTouches[0];
        touchClone.style.display = 'none';
        const elementBelow = document.elementFromPoint(touch.clientX, touch.clientY);

        if (elementBelow) {
            const targetCard = elementBelow.closest('.card');
            if (targetCard && targetCard !== touchItem) {
                handleDrop(touchItem, targetCard);
            }
        }
        touchClone.remove();
        touchClone = null;
    }

    touchItem.classList.remove('dragging');
    document.querySelectorAll('.card').forEach(c => c.classList.remove('drag-over'));
    touchItem = null;
}

//=============== Websocket =======================
let LastTemperatures = [-1270, -1270, -1270, -1270];
let LePort = window.location.port;
if (LePort == "") LePort = "80";

let ws;
if (location.hostname == "localhost") {
    ws = new WebSocket("ws://" + RemoteIP + "/ws");
} else {
    ws = new WebSocket("ws://" + window.location.hostname + ":" + LePort + "/ws");
}
ws.onmessage = (event) => {

    let msg = JSON.parse(event.data);
    msg.HeureCourante = msg.HeureCourante.replace("T", " ");
    GH("date", msg.HeureCourante);
    
    switch (msg.MessageType) {
        case "PuissanceSources": //Arrive exactement toutes les 2s
            for (let i = 0; i < msg.canal.length; i++) {
                let s = msg.source[i];
                let c = msg.canal[i];
                if (Histo10mnPw.length > s) {
                    if (Histo10mnPw[s].length > c) {
                        Histo10mnPw[s][c].push(msg.PwS[i] - msg.PwI[i]); //On rajoute derniere mesure à l'historique
                        Histo10mnPw[s][c].shift();
                        Histo10mnPva[s][c].push(msg.PVAS[i] - msg.PVAI[i]); //On rajoute derniere mesure à l'historique
                        Histo10mnPva[s][c].shift();
                    }

                }

            }
            if (Histo10mnTemp.length == 4) {
                for (let i = 0; i < 4; i++) { // Toutes les 2s on rafarichi le tableau historique des températures
                    Histo10mnTemp[i].push(LastTemperatures[i]);
                    Histo10mnTemp[i].shift();
                }
            }

            for (let i = 0; i < msg.lesActions.ouverture.length; i++) {
                if (Histo10mnOuverture.length > i) {
                    Histo10mnOuverture[i].push(msg.lesActions.ouverture[i]);
                    Histo10mnOuverture[i].shift();
                }
            }
            traceLesHistoriques();
            traceLesTableauxPuissance(msg);
            traceLesTableauxOuvertures(msg);
            traceLesJauges(msg);
            break;

        case "DonnéesLentes":
            //  DonneesLentes(msg);
            for (let i = 0; i < msg.Temperatures.length; i++) {
                LastTemperatures[i] = msg.Temperatures[i] * 10; //Stocké en dixième de degrés
            }
            traceDonneesLentesTableauPuissance(msg);
            TraceLesSoleils(msg);
            traceLesJauges(msg);
            break;

        case "error":
            console.error(msg.message);
            break;

        default:
            console.warn("Unknown message", msg);
    }
};