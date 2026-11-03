
let bloc = [];
let ligne = [];
let ligneGrosse = [];
let pointconnexion = [];
let LigneCourante = -1;
const NS = "http://www.w3.org/2000/svg";
const dXY = 10; // Delta en pixels svg
const dXYmin = 5; // Delta minimum en pixels svg
const dh=dXY-2; // hauteurs au debut et fin des lignes
let coordDown = [0, 0];
let coordUp = [0, 0];
let nbTriac = 0;
const info = GID("info");


const svg = GID('svg-canvas');
const Regulation = {
    Inactif: 0,
    OnOff: 1,
    DecoupeSinus: 2,
    DemiSinus: 3,
    MultiSinus: 4,
    TrainSinus: 5,
    PWM: 6
};


//OMBRE POUR LES BLOCS SVG
let defs = document.createElementNS(NS, "defs");

svg.appendChild(defs);
let filter = document.createElementNS(NS, "filter");
filter.setAttribute("id", "ombre");

let shadow = document.createElementNS(NS, "feDropShadow");
shadow.setAttribute("dx", "0");
shadow.setAttribute("dy", "0");
shadow.setAttribute("stdDeviation", "1");
shadow.setAttribute("flood-color", "#fff");
shadow.setAttribute("flood-opacity", "0.8");

filter.appendChild(shadow);
defs.appendChild(filter);



let drag_bloc = -1;
let click_ES = -1;
let offsetX = 0, offsetY = 0;


function Init() {

    LoadConfig();

}
let ListeGPIO = [0];
function ImplantationConfig() {
    SetHautBas();
    GH("nom_R", C.nomRouteur);
    //Liste des GPIOs de la carte
    if (C.ListeGPIO.indexOf(",") >= 0) ListeGPIO = C.ListeGPIO.split(",").map(Number);
    //Les Sources de Puissance
    let S = "";
    for (let i = 0; i < C.LaSource.length; i++) {

        for (let j = 0; j < C.LaSource[i].CanalNom.length; j++) {
            if (C.LaSource[i].CanalNom[j] != "") {
                const NumSourceCanal = i * 3 + j; //3 canaux par source
                S += `<option value="P${NumSourceCanal}" data-type="puissance">⚡${C.LaSource[i].CanalNom[j]}</option>`;
                GID("groupePuissance").style.display = "block";
            }
        }
        GH("groupePuissance", S);
    }
    //Les Températures
    S = "";
    for (let c = 0; c < 4; c++) {
        if (C.LesTemperatures[c].Source_Temp != "tempNo") {
            S += `<option value="T${c}" data-type="temperature">🌡️${C.LesTemperatures[c].nomTemperature}</option>`;
            GID("groupeTemperature").style.display = "block";
        }
    }
    GH("groupeTemperature", S);

    //Les Actions externes
    S = "";
    for (let j = 0; j < C.RouteurExt.length; j++) {
        for (let k = 0; k < C.RouteurExt[j].ActionNom.length; k++) {
            if (C.RouteurExt[j].ActionNom[k] != "") {
                S += `<option value="${j}_${k}" data-type="ActExt_Ouvre">🚰${C.RouteurExt[j].ActionNom[k]} / Ouvre</option>`;
                S += `<option value="${j}_${k}" data-type="ActExt_Hequiv">&#x1F553;${C.RouteurExt[j].ActionNom[k]} / Hequiv</option>`;
                GID("groupeActionsExternes").style.display = "block";
            }
        }
    }
    GH("groupeActionsExternes", S);

    //Definition hauteur du SVG nécessaire pour loger les blocs
    let H = 50;
    let JSD = C.LesActions;
    for (let i = 0; i < JSD.blocs.length; i++) {
        H = Math.max(H, JSD.blocs[i].y);
    }
    svg.setAttribute("viewBox", "0 0 500 " + (H + 120));

    //======== Tracage des blocs actions ========

    for (let i = 0; i < JSD.blocs.length; i++) {
        CreationBloc(JSD.blocs[i]);

    }
    //Gestion unités des sorties
    for (let bl = 0; bl < bloc.length; bl++) {

        if (bloc[bl]) {
            Transit_Unite_Bloc(bl);
        }
    }

    for (let L = 0; L < JSD.lignes.length; L++) {
        ligne[L] = document.createElementNS(NS, "polyline");
        ligne[L].setAttribute("data-ligne", L);
        ligne[L].setAttribute("data-pinEntree", JSD.lignes[L].pinEntree);
        ligne[L].setAttribute("data-blocEntree", JSD.lignes[L].blocEntree);
        bloc[JSD.lignes[L].blocEntree].LesEntrees[JSD.lignes[L].pinEntree].cable = true;
        const [X1, Y1] = coord_Entree(JSD.lignes[L].blocEntree, JSD.lignes[L].pinEntree);
        ligne[L].setAttribute("data-pinSortie", JSD.lignes[L].pinSortie);
        ligne[L].setAttribute("data-blocSortie", JSD.lignes[L].blocSortie);
        const [X2, Y2] = coord_Sortie(JSD.lignes[L].blocSortie, JSD.lignes[L].pinSortie);
        Trace_Ligne(L, X1, Y1, X2, Y2);
        ligne[L].setAttribute("stroke", bloc[JSD.lignes[L].blocSortie].SortieNum ? "var(--act-lignenum)" : "var(--act-lignebin)"); //Bonne couleur de la ligne
        Calcul_Trajet(L, [X1, Y1], [X2, Y2]);
        LigneCourante = L;
        Transit_Unite_Ligne(L);
    }

    points_branchements();

    for (let bl = 0; bl < bloc.length; bl++) {

        if (bloc[bl]) {
            layout_bloc(bl);
        }
    }
    //Para bas de page
    GID("CACSI" + C.ReacCACSI).checked = true;
    GID("Fpwm" + C.FreqPWM).checked = true;

    checkParaCommun()
}
function checkParaCommun() {
    GID("commun").style.display = C.ModePara ? "block" : "none";
    let disp = "none";
    for (let bl = 0; bl < bloc.length; bl++) {
        if (bloc[bl]) {
            if (bloc[bl].Para.Regulation == Regulation.PWM) disp = "block";
        }
    }
    GID("Freq_PWM").style.display = disp;
    GID("CACSI").style.display = C.EstimCACSI ? "none" : "block";

}
function FinParaRouteur() {

};
function AdaptationSource() {

}
function Reset() {
    GID("attente").style = "visibility: visible;";
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            GID("attente").style = "visibility: hidden;";
            setTimeout(location.reload(), 1000);
            setTimeout(location.reload(), 10000);
        }
    };
    xhttp.open('GET', 'restart', true);
    xhttp.send();
}

//==========================================
//======== Création des Blocs ==============
//==========================================
function CreationBloc(t) {

    if (t.value == "") return;

    let bl = bloc.length
    let LesEntrees = [];
    let LesSorties = [];
    let NbEntreeMin = 0;
    let NbEntreeMax = 0;
    let optionB, type, Value, Label;
    let SortieNum = false;  //Sortie Binaire par défaut,
    let ancienBloc = false;


    if (t.Para) { //ancien Bloc
        type = t.type;
        Value = t.value;
        if (type == "puissance") {
            for (let i = 0; i < C.LaSource.length; i++) {
                for (let j = 0; j < C.LaSource[i].CanalNom.length; j++) {
                    if (C.LaSource[i].CanalNom[j] != "") {
                        let NumSourceCanal = i * 3 + j; //3 canaux par source
                        let V = "P" + NumSourceCanal;
                        if (V == Value) {
                            t.Para.Label = C.LaSource[i].CanalNom[j]; // On prend le dernier nom de la source de puissance
                        }
                    }
                }
            }
        }
        Label = t.Para.Label;
        ancienBloc = true;

    } else { //Creation de bloc
        optionB = t.options[t.selectedIndex];
        type = optionB.dataset.type;
        Value = optionB.value;
        Label = optionB.text;
    }

    let X0 = 0.1;
    let W = 50; //Largeur Bloc
    let Para = { Label: Label }; //Parametres associés au bloc

    if (type == "commande") {
        Para = { Label: Label, Commande: 0 };
    }

    if (type == "bin") {
        switch (Value) {
            case "et":
            case "ou":
                NbEntreeMin = 2;
                NbEntreeMax = 10;
                break;
            case "ou_ex":
                NbEntreeMin = 2;
                NbEntreeMax = 2;
                break;
            case "bascule":
                NbEntreeMin = 3;
                NbEntreeMax = 3;
                break;
            case "non":
                NbEntreeMin = 1;
                NbEntreeMax = 1;
                break;
        }
        X0 = 0.6;
        W = 30;
    }

    if (type == "puissance" || type == "heure" || type == "temperature" || type == "soleil" || Value == "moyenne" || Value == "mqttnum" || type == "addsub") {
        SortieNum = true; //Sortie numérique pour certains capteurs

    }
    if (type == "ActExt_Ouvre" || type == "ActExt_Hequiv") { //Action routeur Externe
        SortieNum = true; //Sortie numérique pour les actions externes
        let parts = Value.split("_");
        let Idx_RMS = parseInt(parts[0]);
        let Idx_Act = parseInt(parts[1]);
        Para = { Label: Label, Idx_RMS: Idx_RMS, Idx_Act: Idx_Act };
    }

    if (type == "action") {
        NbEntreeMin = 4;
        NbEntreeMax = 4;
        SortieNum = true; //Sortie % d'actionneur
        X0 = 0.8;
        W = 60;
        if (Value == "triac") {
            nbTriac++;
            if (nbTriac > 1) {
                alert("Vous ne pouvez pas créer plus d'un Triac");
                t.selectedIndex = 0;
                nbTriac = 1;
                return;
            }
        }

        Para = {
            Label: Label, Nom: "", GPIO: -1,
            Regulation: Regulation.OnOff, VoltON: 3.3, Temporisation: 0,
            OuvreForce: 100, OuvreMax: 100,
            PID_On: false, P: 0, I: 5, D: 0,
            SeuilPW_On: 0, SeuilPW_Off: 100,
            Host: "", Port: 80, OrdreOn: "", OrdreOff: "", Repetition: 240, GetPost: 0
        }
    }


    if (type == "filtre") {
        NbEntreeMin = 1;
        NbEntreeMax = 1;
        X0 = 0.3;
        Para = { Label: Label, Seuil: 0 }; // Valeur par défaut du seuil pour les filtres
    }
    if (type == "addsub") {
        NbEntreeMin = 2;
        NbEntreeMax = 2;
        X0 = 0.3;
        Para = { Label: Label }; // Valeur par défaut pour les additionneurs/soustracteurs
    }


    let NbEntree = Math.max(1, NbEntreeMin);
    if (ancienBloc) {
        Para = t.Para;
        NbEntree = Math.max(NbEntree, t.LesEntrees.length);
        NbEntreeMin = Math.max(NbEntree, NbEntreeMin);
        NbEntreeMax = Math.max(NbEntreeMin, NbEntreeMax);
    }

    //=== creation d'un groupe ===
    bloc[bl] = {
        Svg: null,
        LesEntrees: LesEntrees,
        LesSorties: LesSorties,
        rect: null,
        ZoneBloc: null,
        Value: Value,
        SortieNum: SortieNum,
        NbEntreeMin: NbEntreeMin,
        NbEntreeMax: NbEntreeMax,
        type: type,
        Display: null,
        Para: Para
    };
    bloc[bl].Svg = document.createElementNS(NS, "g");
    bloc[bl].Svg.setAttribute("data-bloc", bl);
    //Positionnement initial
    const viewBoxString = svg.getAttribute('viewBox');
    const viewBoxValues = viewBoxString.split(' ').map(Number);
    const [minX, minY, viewBoxWidth, viewBoxHeight] = viewBoxValues;

    let X = X0 * viewBoxWidth;
    let Y = 10 + 0.8 * viewBoxHeight * Math.random();
    X=dXYmin*Math.round(X/dXYmin);
    Y=dXYmin*Math.round(Y/dXYmin);
    if (ancienBloc) {
        X = t.x;
        Y = t.y;
    }
    bloc[bl].Svg.setAttribute("transform", `translate(${X},${Y})`);


    //=== Zone autour du Bloc et lignes===
    const ZoneBloc = document.createElementNS(NS, "rect");
    ZoneBloc.setAttribute("y", - 5);
    ZoneBloc.setAttribute("x", -20);
    ZoneBloc.setAttribute("width", 40 + W);
    ZoneBloc.setAttribute("height", 10 + (1 + NbEntree) * dXY);
    ZoneBloc.setAttribute("fill", "transparent");
    ZoneBloc.setAttribute("style", "cursor: ew-resize");
    ZoneBloc.setAttribute("data-bloc", bl);
    bloc[bl].Svg.appendChild(ZoneBloc);
    bloc[bl].ZoneBloc = ZoneBloc;



    ZoneBloc.addEventListener('pointerdown', (e) => {
        const p = getSVGPoint(e);
        const db = parseInt(ZoneBloc.getAttribute('data-bloc'));

        const coordBloc = coord_Bloc(db);
        const H = ZoneBloc.getAttribute("height");
        let ClickZoneSortie = false;
        if (Math.abs(p.x - coordBloc[0]) > W && Math.abs(p.y - coordBloc[1]) < H) {
            ClickZoneSortie = true;
        }

        if (click_ES == -1) {
            click_ES = db;
            //== creation d'une ligne ===
            LigneCourante++;
            ligne[LigneCourante] = document.createElementNS(NS, "polyline");
            ligne[LigneCourante].setAttribute("data-ligne", LigneCourante);
            let [X, Y] = [0, 0];
            if (!ClickZoneSortie) { // On a cliqué sur une entrée, 
                let pinIdx = Creation_Entree(db, true, false, p, null);
                if (pinIdx == -1) {
                    click_ES = -1;//"Nombre d'entrée maximum atteint pour ce bloc"
                    return;
                }
                layout_bloc(db);
                ligne[LigneCourante].setAttribute("data-pinEntree", pinIdx);
                ligne[LigneCourante].setAttribute("data-blocEntree", db);
                [X, Y] = coord_Entree(db, pinIdx);
            } else {
                const pinSor = LocaliseSortie(db, p)
                ligne[LigneCourante].setAttribute("data-pinSortie", pinSor);
                ligne[LigneCourante].setAttribute("data-blocSortie", db);
                [X, Y] = coord_Sortie(db, pinSor);


            }
            Trace_Ligne(LigneCourante, X, Y, X, Y);
        }

    });



    //=== Hauteur du Bloc ========
    const H =2*dh+ ( NbEntree-1) * dXY;
    //== Affichage au centre du bloc ====
    if (type == "bin") {
        let bool_Trace = boolTrace(bl, W, H);
        bloc[bl].Svg.appendChild(bool_Trace);
        bloc[bl].Display = bool_Trace;
    } else {
        let blocLabel = bloc_Label(bl, W, H)
        bloc[bl].Svg.appendChild(blocLabel);
        bloc[bl].Display = blocLabel;
    }
    //=== Sortie du Bloc ========
    let N = "";
    if (type == "action") N = "Ouvre %";
    Creation_Sortie(bl, W, H, SortieNum, type, N);
    if (type == "action") Creation_Sortie(bl, W, H, SortieNum, type, "Hequiv");

    //=== Prepositionne certaines entrées ===
    // pour les blocs qui en ont besoin dès la création (ex: filtre, moyenne) et les colorie en bleu pour les différencier des autres entrées qui sont libres et en noir.
    let SignalN = false; //Par défaut, les entrées sont binaires
    let nom_ = ""
    if (type == "filtre") {
        if (Value != "timer" && Value != "retard") SignalN = true;
        if (Value == "timer") nom_ = "⬏";
        let pinEntree = Creation_Entree(bl, false, SignalN, null, nom_);

    } else if (type == "addsub") {
        let pinEntree = Creation_Entree(bl, true, true, null, "+");
        let signe = (Value == "soustraction") ? "-" : "+";
        let pinEntree2 = Creation_Entree(bl, false, true, null, signe);
        for (let i = 0; i < bloc[bl].LesEntrees.length; i++) {
            bloc[bl].LesEntrees[i].cable = false; // Libere les entrées 
        }
    } else {
        for (let i = 0; i < NbEntreeMin; i++) {

            if (type == "action") {
                if (i == 3) SignalN = true;
                const noms = ["Force Off", "Force On", "Régulation", "Puissance"];
                nom_ = noms[i];
            }
            if (Value == "bascule") {
                const noms = ["⬏Off", "⬏On", "⬏Basculer"];
                nom_ = noms[i];
            }
            let pinEntree = Creation_Entree(bl, true, SignalN, null, nom_);
        }
        for (let i = 0; i < bloc[bl].LesEntrees.length; i++) {
            bloc[bl].LesEntrees[i].cable = false; // Libere les entrées prépositionnées pour les blocs qui en ont besoin dès la création, afin de pouvoir y connecter d'autres blocs par la suite
        }
    }

    // Rectangle central réactif aux move et clicks
    const rect = document.createElementNS(NS, "rect"); //Enveloppe de l'objet bloc, pour le déplacer et le sélectionner. Non Visible
    rect.setAttribute("y", 0);
    rect.setAttribute("x", 0);
    rect.setAttribute("width", W);
    rect.setAttribute("height", H);
    rect.setAttribute("fill", "transparent");
    rect.setAttribute("style", "cursor: move");
    rect.setAttribute("data-bloc", bl);
    bloc[bl].Svg.appendChild(rect);
    bloc[bl].rect = rect;


    rect.addEventListener('pointerdown', (e) => {
        drag_bloc = parseInt(bloc[bl].Svg.getAttribute('data-bloc'));
        const p = getSVGPoint(e);
        coordDown = [p.x, p.y];
        // Mémorise le décalage entre le curseur et le groupe bloc

        const coordBloc = coord_Bloc(drag_bloc);
        offsetX = p.x - coordBloc[0];
        offsetY = p.y - coordBloc[1];

    });


    rect.addEventListener('click', (e) => {
        const db = parseInt(rect.getAttribute('data-bloc'));
        if (Math.abs(coordDown[0] - coordUp[0]) < 5 && Math.abs(coordDown[1] - coordUp[1]) < 5) {
            // Affiche les informations du bloc dans une infobulle

            const s = dispPara(db);
            AfficheInfo(s, e);
            checkParaDisable(db)

        }
    });



    svg.appendChild(bloc[bl].Svg); //On installe dans le graphique
    t.selectedIndex = 0;

}


// Convertit les coordonnées écran → coordonnées SVG
function getSVGPoint(e) {
    const pt = svg.createSVGPoint();
    pt.x = e.clientX;
    pt.y = e.clientY;
    return pt.matrixTransform(svg.getScreenCTM().inverse());
}


svg.addEventListener('pointermove', (e) => {
    const p = getSVGPoint(e);

    if (drag_bloc !== -1) {// Si un bloc est en cours de déplacement, met à jour sa position

        let X = p.x - offsetX;
        let Y = p.y - offsetY;
        X = dXYmin * Math.round(X / dXYmin);
        Y = dXYmin * Math.round(Y / dXYmin);
        bloc[drag_bloc].Svg.setAttribute('transform', `translate(${X},${Y})`);
        for (let i = 0; i < ligne.length; i++) {
            if (ligne[i] !== null) {
                let db_sortie = ligne[i].getAttribute("data-blocSortie");
                let pin_sortie = ligne[i].getAttribute("data-pinSortie");
                let db_entree = ligne[i].getAttribute("data-blocEntree");
                let pin_entree = ligne[i].getAttribute("data-pinEntree");
                if (db_sortie == drag_bloc) {
                    let coord_entree = coord_Entree(db_entree, pin_entree);
                    let coord_sortie = coord_Sortie(drag_bloc, pin_sortie);
                    Calcul_Trajet(i, coord_entree, coord_sortie);
                    points_branchements();
                } else if (db_entree == drag_bloc) {
                    let coord_sortie = coord_Sortie(db_sortie, pin_sortie);
                    let coord_entree = coord_Entree(drag_bloc, pin_entree);
                    Calcul_Trajet(i, coord_entree, coord_sortie);
                    points_branchements();
                }
            }
        }

    } else {
        // Si une ligne est en cours de création, met à jour la position du point final
        if (click_ES !== -1) {

            let points = ligne[LigneCourante].getAttribute("points").split(" ");
            let ax = parseFloat(points[0].split(",")[0]);
            let ay = parseFloat(points[0].split(",")[1]);
            let coord_deb = [ax, ay];
            let coord_fin = [p.x, p.y];
            Calcul_Trajet(LigneCourante, coord_fin, coord_deb)
        }



    }
});


window.addEventListener('pointerup', (e) => {
    const p = getSVGPoint(e);
    coordUp = [p.x, p.y];
    drag_bloc = -1;
    if (click_ES !== -1 && LigneCourante !== -1) {
        let coord_entree = [0, 0];
        let coord_sortie = [0, 0];
        let db_sortie = ligne[LigneCourante].getAttribute("data-blocSortie");
        let db_entree = ligne[LigneCourante].getAttribute("data-blocEntree");
        let pin_entree = 0;
        let pin_Sortie = 0;

        let Lien_ES_trouve = false;
        if (db_sortie !== null) { //On a une sortie, on cherche l'entrée
            pin_Sortie = ligne[LigneCourante].getAttribute("data-pinSortie");
            coord_sortie = coord_Sortie(db_sortie, pin_Sortie);

            for (i = 0; i < bloc.length; i++) {
                if (bloc[i] != null && i !== parseInt(db_sortie)) {
                    let coordBloc = bloc[i].Svg.getAttribute("transform").match(/translate\(([^,]+),([^)]+)\)/);
                    let x = parseFloat(coordBloc[1]);
                    let y = parseFloat(coordBloc[2]);
                    let H = parseFloat(bloc[i].ZoneBloc.getAttribute("height"));

                    if (Math.abs(x - p.x) <= 40 && Math.abs(p.y - y) <= H) {
                        pin_entree = Creation_Entree(i, true, false, p, null);
                        if (pin_entree == -1) {
                            click_ES = -1;//"Nombre d'entrée maximum atteint pour ce bloc"
                            break;
                        }
                        Lien_ES_trouve = true; //Entree trouvée
                        layout_bloc(i);
                        coord_entree = coord_Entree(i, pin_entree);
                        ligne[LigneCourante].setAttribute("data-blocEntree", i);
                        ligne[LigneCourante].setAttribute("data-pinEntree", pin_entree);
                        break;
                    }
                }
            }
        }

        if (db_entree !== null) { //On a une entrée, on cherche la sortie
            pin_entree = ligne[LigneCourante].getAttribute("data-pinEntree");
            coord_entree = coord_Entree(db_entree, pin_entree);
            for (i = 0; i < bloc.length; i++) {
                if (bloc[i] != null && i !== parseInt(db_entree)) {
                    const pinProche = LocaliseSortie(i, p);

                    coord_sortie = coord_Sortie(i, pinProche);
                    if (Math.abs(p.x - coord_sortie[0]) <= 40 && Math.abs(p.y - coord_sortie[1]) <= 20) {
                        Lien_ES_trouve = true;
                        ligne[LigneCourante].setAttribute("data-blocSortie", i);
                        ligne[LigneCourante].setAttribute("data-pinSortie", pinProche);
                        pin_Sortie = pinProche;
                        db_sortie = i;
                        break;
                    }

                }

            }
        }
        let Signal = null; //Les sortie d'um même bloc sont de même type binaire ou numérique

        if (Lien_ES_trouve) {
            Signal = bloc[db_sortie].SortieNum; //Les sortie d'um même bloc sont de même type binaire ou numérique
            Calcul_Trajet(LigneCourante, coord_entree, coord_sortie);
            ligneGrosse[LigneCourante].setAttribute("style", "cursor: help");
            db_sortie = ligne[LigneCourante].getAttribute("data-blocSortie");
            db_entree = ligne[LigneCourante].getAttribute("data-blocEntree");
            pin_entree = ligne[LigneCourante].getAttribute("data-pinEntree");

           
            let SignalEntree = bloc[db_entree].LesEntrees[pin_entree].EntreeNum;
            if (SignalEntree !== Signal) { // Pas le meme type de signal en sortie et en entrée
                Lien_ES_trouve = false;
            }
           
            if (bloc[db_entree].LesEntrees[pin_entree].nom == "Puissance") { //Cas du Triac ou SSR
                if (bloc[db_sortie].type != "puissance" && bloc[db_sortie].Value != "moyenne") Lien_ES_trouve = false;
                if (bloc[db_sortie].LesSorties[pin_Sortie].Unit != "W" && bloc[db_sortie].Value == "moyenne") Lien_ES_trouve = false;
            }
            
            if (bloc[db_entree].type == "addsub") { //Cas des additions ou soustraction de puissance
                
                if (bloc[db_sortie].LesSorties[pin_Sortie].type != "puissance") Lien_ES_trouve = false;
            }
            
        }
        if (Lien_ES_trouve) {
            const SortieNum = bloc[db_sortie].LesSorties[pin_Sortie].SortieNum;
            bloc[db_entree].LesEntrees[pin_entree].type = bloc[db_sortie].type; // Transmet le type de signal de la sortie du bloc source à l'entrée du bloc cible
            bloc[db_entree].LesEntrees[pin_entree].Unit = bloc[db_sortie].LesSorties[pin_Sortie].Unit;
            bloc[db_entree].LesEntrees[pin_entree].Svg.setAttribute("fill", bloc[db_entree].LesEntrees[pin_entree].EntreeNum ? "var(--act-lignenum)" : "var(--act-lignebin)"); // Met à jour la couleur de l'entrée en fonction du signal de sortie reçu
            bloc[db_entree].LesEntrees[pin_entree].cable = true; // Marque l'entrée comme câblée
            ligne[LigneCourante].setAttribute("stroke", SortieNum ? "var(--act-lignenum)" : "var(--act-lignebin)"); // Met à jour la couleur de la ligne en fonction du signal
            layout_bloc(db_entree);
            points_branchements();

        } else {
            RemoveLigne(LigneCourante);
        }
        click_ES = -1;
    }

});
function Creation_Entree(bl, cable, SignalNum, pSouris, nom) {
    let pinIdx = -1;
    const type = bloc[bl].type;
    if (type !== "puissance" && type !== "heure" && type !== "temperature" && type !== "soleil" && type !== "commande" && type !== "jour" && type !== "tempo" && type !== "hphc" && type !== "ActExt_Ouvre" && type !== "ActExt_Hequiv") {


        //On cherche si l'entrée déjà dessinée
        let distance = 9999;
        for (let pin = 0; pin < bloc[bl].LesEntrees.length; pin++) { //Les entrees existantes
            if (bloc[bl].LesEntrees[pin].cable == false) {

                if (pSouris) {
                    const coordEntree = coord_Entree(bl, pin);
                    const dist = Math.sqrt(Math.pow(coordEntree[0] - pSouris.x, 2) + Math.pow(coordEntree[1] - pSouris.y, 2));
                    if (dist < distance) {
                        distance = dist;
                        pinIdx = pin;
                        SignalNum = bloc[bl].LesEntrees[pin].EntreeNum; //On reprend ancien  signal
                    }
                } else {
                    pinIdx = pin;
                    SignalNum = bloc[bl].LesEntrees[pin].EntreeNum; //On reprend ancien  signal
                }
            }
        }

        if (bloc[bl].LesEntrees.length >= bloc[bl].NbEntreeMax && pinIdx == -1) {
            return -1; //"Nombre d'entrée maximum atteint pour ce bloc" On ne peut pas creer
        }
        if (pinIdx == -1) { //On a pas trouvé de pin de libre, on en crée un nouveau
            bloc[bl].LesEntrees.push({ Svg: null, cable: cable, nom: "", Unit: null, EntreeNum: SignalNum, type: null });
            pinIdx = bloc[bl].LesEntrees.length - 1;

        }

        UneEntree = document.createElementNS(NS, "circle");
        const pt_entree = svg.createSVGPoint();
        pt_entree.x = -2;
        pt_entree.y =dh+  pinIdx * dXY;

        UneEntree.setAttribute("cx", pt_entree.x);
        UneEntree.setAttribute("cy", pt_entree.y);
        UneEntree.setAttribute("r", 2);
        UneEntree.setAttribute("fill", SignalNum ? "var(--act-lignenum)" : "var(--act-lignebin)");
        UneEntree.setAttribute("data-bloc", bl);
        bloc[bl].LesEntrees[pinIdx].Svg = UneEntree;
        bloc[bl].Svg.appendChild(bloc[bl].LesEntrees[pinIdx].Svg);
        if (nom != "") {
            const Texte = document.createElementNS(NS, "text");
            Texte.setAttribute("x", pt_entree.x + 5);
            Texte.setAttribute("y", pt_entree.y + 1);
            Texte.textContent = nom;
            Texte.setAttribute("font-size", "3");
            Texte.setAttribute("fill", "var(--act-svgtxt)");

            bloc[bl].Svg.appendChild(Texte);
            bloc[bl].LesEntrees[pinIdx].nom = nom;
        }
        let Unit = null;
        switch (bloc[bl].type) {
            case "bin":
            case "filtre":
                Unit = "bin";
                if (bloc[bl].Value == "moyenne") Unit = null;
                break;

            case "action":
                Unit = SignalNum ? "W" : "bin";
                break;
        }
        bloc[bl].LesEntrees[pinIdx].Unit = Unit;
    }
    return pinIdx;
}

function coord_Entree(Bloc, pin) {
    const Cx = parseFloat(bloc[Bloc].LesEntrees[pin].Svg.getAttribute("cx"));
    const Cy = parseFloat(bloc[Bloc].LesEntrees[pin].Svg.getAttribute("cy"));
    const coordBloc = coord_Bloc(Bloc);
    return [Cx + coordBloc[0], Cy + coordBloc[1]];
}
function Creation_Sortie(bl, W, H, SortieNum, type, nom) {
    if (type == "addsub") type = "puissance";
    bloc[bl].LesSorties.push({ Svg: null, SvgT: null, SvgV: null, type: type, nom: nom, Unit: null, SortieNum: SortieNum });
    let pinSor = bloc[bl].LesSorties.length - 1;
    const UneSortie = document.createElementNS(NS, "polyline");
    let Y = H / 2;
    if (pinSor == 1) Y = 3 * H / 4;
    UneSortie.setAttribute("points", `${W},${Y - 3} ${W + 6},${Y} ${W},${Y + 3}`);
    UneSortie.setAttribute("fill", SortieNum ? "var(--act-lignenum)" : "var(--act-lignebin)");
    UneSortie.setAttribute("data-bloc", bl);
    bloc[bl].LesSorties[pinSor].Svg = UneSortie;
    bloc[bl].Svg.appendChild(bloc[bl].LesSorties[pinSor].Svg);
    if (nom != "") {
        const Texte = document.createElementNS(NS, "text"); //Nom eventuel de la pin
        Texte.setAttribute("x", W - 2 - 1.5 * nom.length);
        Texte.setAttribute("y", Y + 1);
        Texte.textContent = nom;
        Texte.setAttribute("font-size", "3");
        Texte.setAttribute("fill", "var(--act-svgtxt)");
        bloc[bl].Svg.appendChild(Texte);
        bloc[bl].LesSorties[pinSor].SvgT = Texte;
    }
    const TexteV = document.createElementNS(NS, "text");  //Valeur en sortie
    TexteV.setAttribute("x", W + 2);
    TexteV.setAttribute("y", Y - 2);
    TexteV.textContent = "";
    TexteV.setAttribute("font-size", "4");
    TexteV.setAttribute("fill", "var(--act-svgtxtOut)");
    bloc[bl].Svg.appendChild(TexteV);
    bloc[bl].LesSorties[pinSor].SvgV = TexteV;

    let Unit = null;
    switch (type) {
        case "bin":
        case "tempo":
        case "jour":
        case "filtre":
        case "hphc":
            Unit = "bin";
            if (bloc[bl].Value == "moyenne") Unit = null;
            break;
        case "puissance":
            Unit = "W";
            break;
        case "heure":
        case "ActExt_Hequiv":
            Unit = "Hmn";
            break;
        case "ActExt_Ouvre":
            Unit = "%";
            break;
        case "temperature":
            Unit = "°C";
            break;
        case "soleil":
            Unit = "W/m²";
            break;
        case "action":
            if (pinSor == 0) Unit = "%";
            if (pinSor == 1) Unit = "Hmn";
            break;
    }
    bloc[bl].LesSorties[pinSor].Unit = Unit;

    return pinSor;
}
function coord_Sortie(Bloc, pinSor) {
    const points = bloc[Bloc].LesSorties[pinSor].Svg.getAttribute("points")
    let Points = points.split(" ");
    let lePoint = Points[1].split(",");
    const X = parseFloat(lePoint[0]);
    const Y = parseFloat(lePoint[1]);
    const coordBloc = coord_Bloc(Bloc);
    return [X + coordBloc[0], Y + coordBloc[1]];
}
function LocaliseSortie(bl, pSouris) { //Trouve sortie près de la souris
    let pinSor = 0;

    let distance = 9999;
    for (let pin = 0; pin < bloc[bl].LesSorties.length; pin++) { //Les entrees existantes
        const coordSortie = coord_Sortie(bl, pin);
        const dist = Math.sqrt(Math.pow(coordSortie[0] - pSouris.x, 2) + Math.pow(coordSortie[1] - pSouris.y, 2));
        if (dist < distance) {
            distance = dist;
            pinSor = pin;
        }
    }
    return pinSor;
}
function coord_Bloc(Bloc) {
    const Cx = parseFloat(bloc[Bloc].Svg.getAttribute("transform").match(/translate\(([^,]+),([^)]+)\)/)[1]);
    const Cy = parseFloat(bloc[Bloc].Svg.getAttribute("transform").match(/translate\(([^,]+),([^)]+)\)/)[2]);
    return [Cx, Cy];
}

function Trace_Ligne(LigneCourante, X1, Y1, X2, Y2) {
    ligne[LigneCourante].setAttribute("points", `${X1},${Y1} ${X2},${Y2}`);
    ligne[LigneCourante].setAttribute("fill", "none");
    ligne[LigneCourante].setAttribute("stroke", "grey");
    ligne[LigneCourante].setAttribute("stroke-width", 1);

    ligneGrosse[LigneCourante] = document.createElementNS(NS, "polyline");
    ligneGrosse[LigneCourante].setAttribute("fill", "none");
    ligneGrosse[LigneCourante].setAttribute("stroke-width", 9);
    ligneGrosse[LigneCourante].setAttribute("stroke", "transparent");
    ligneGrosse[LigneCourante].setAttribute("points", `${X1},${Y1} ${X2},${Y2}`);
    ligneGrosse[LigneCourante].setAttribute("data-ligne", LigneCourante);
    ligneGrosse[LigneCourante].setAttribute("style", "cursor: move");
    const Ligne = ligne[LigneCourante];

    const LigneG = ligneGrosse[LigneCourante];

    svg.appendChild(Ligne);
    svg.appendChild(LigneG);
    LigneG.addEventListener('click', (e) => {
        const LigneC = parseInt(LigneG.getAttribute('data-ligne'));

        const p = getSVGPoint(e);
        coordDown = [p.x, p.y];
        let s = "";
        for (const [cle, valeur] of Object.entries(ligne[LigneC].dataset)) {
            s += `${cle} = ${valeur}<br>`;
        }
        s += `<span class="poubelle" onclick="RemoveLigne(${LigneC})">&#128465</span> Cliquer pour supprimer la ligne`;
        AfficheInfo(s, e);
        info.style.background = "var(--act-coulLig)";

    });
}
function Calcul_Trajet(LigneC, coord_E, coord_S) {
    let Sx = coord_S[0];
    let Sy = coord_S[1];
    let Ex = coord_E[0];
    let Ey = coord_E[1];
    let mx = Math.round((Sx + Ex) / 2); // milieu en X
    let my = Math.round((Sy + Ey) / 2); // milieu en Y

    mx = dXYmin*Math.round(mx/dXYmin);
    my = dXYmin*Math.round(my/dXYmin);

    let points = [`${Sx},${Sy}`, `${mx},${Sy}`, `${mx},${Ey}`, `${Ex},${Ey}`]; // Recalcule les points de la ligne pour qu'eLigneCourantee soit bien droite entre les deux blocs 
    if (mx < Sx + 20 && mx > Ex - 20) { // La sortie et l'entréé coupe les blocs
        points = [`${Sx},${Sy}`, `${Sx + 20},${Sy}`, `${Sx + 20},${my}`, `${Ex - 20},${my}`, `${Ex - 20},${Ey}`, `${Ex},${Ey}`];
    } else if (mx < Sx + 20) { // La sortie coupe le bloc par la gauche
        points[1] = `${Sx + 20},${Sy}`;
        points[2] = `${Sx + 20},${Ey}`;
    } else if (mx > Ex - 20) { // L'entrée coupe le bloc par la gauche
        points[1] = `${Ex - 20},${Sy}`;
        points[2] = `${Ex - 20},${Ey}`;
    }
    ligne[LigneC].setAttribute("points", points.join(" "));
    ligneGrosse[LigneC].setAttribute("points", points.join(" "));

}
function layout_bloc(bl) {

    let NbEntree = 1;
    if (bloc[bl].LesEntrees) {
        for (let i = 0; i < bloc[bl].LesEntrees.length; i++) {
            if (bloc[bl].LesEntrees[i].Svg !== null) NbEntree = i + 1;
        }
    }
    NbEntree = Math.max(NbEntree, bloc[bl].NbEntreeMin);
    let H = 2*dh+ ( NbEntree-1) * dXY;
    bloc[bl].rect.setAttribute("height", H);
    bloc[bl].ZoneBloc.setAttribute("height", 10 + H);
    const W = parseFloat(bloc[bl].rect.getAttribute("width"));


    for (let pinSor = 0; pinSor < bloc[bl].LesSorties.length; pinSor++) {
        let Y = H / 2;
        if (pinSor == 1) Y = 3 * H / 4;
        bloc[bl].LesSorties[pinSor].Svg.setAttribute("points", `${W},${Y - 3} ${W + 6},${Y} ${W},${Y + 3}`);
        if (bloc[bl].LesSorties[pinSor].SvgT) bloc[bl].LesSorties[pinSor].SvgT.setAttribute("y", `${Y + 1}`);
    }

    if (bloc[bl].type == "bin") {
        let bool_Trace = boolTrace(bl, W, H);
        bloc[bl].Svg.replaceChild(bool_Trace, bloc[bl].Display);
        bloc[bl].Display = bool_Trace;
    } else {
        let blocLabel = bloc_Label(bl, W, H)
        bloc[bl].Svg.replaceChild(blocLabel, bloc[bl].Display);
        bloc[bl].Display = blocLabel;
    }
    Transit_Unite_Bloc(bl);
    for (let i = 0; i < ligne.length; i++) {

        if (ligne[i] !== null) {

            let db_sortie = ligne[i].getAttribute("data-blocSortie");
            let pin_Sortie = ligne[i].getAttribute("data-pinSortie");
            let db_entree = ligne[i].getAttribute("data-blocEntree");
            let pin_entree = ligne[i].getAttribute("data-pinEntree");


            if (db_sortie == drag_bloc) {
                let coord_entree = coord_Entree(db_entree, pin_entree);
                let coord_sortie = coord_Sortie(db_sortie, pin_Sortie);
                Calcul_Trajet(i, coord_entree, coord_sortie);
                points_branchements();
            }
            Transit_Unite_Ligne(i);
        }
    }


}
function points_branchements() { //Plusieurs ligne sur même sortie.
    for (let i = 0; i < pointconnexion.length; i++) {
        if (pointconnexion[i] !== null) {
            svg.removeChild(pointconnexion[i]);
        }
    }
    pointconnexion.splice(0, pointconnexion.length);
    for (let i = 0; i < ligne.length; i++) {
        if (ligne[i] !== null) {
            let db_sortieI = ligne[i].getAttribute("data-blocSortie");
            let pin_sortieI = ligne[i].getAttribute("data-pinSortie");
            for (let j = i + 1; j < ligne.length; j++) {
                if (ligne[j] !== null) {
                    let db_sortieJ = ligne[j].getAttribute("data-blocSortie");
                    let pin_sortieJ = ligne[j].getAttribute("data-pinSortie");
                    if (db_sortieI === db_sortieJ && pin_sortieI === pin_sortieJ && db_sortieI !== null) {
                        const SortieNum = bloc[db_sortieI].LesSorties[pin_sortieI].SortieNum;
                        // Gérer le cas des lignes qui partagent la même sortie
                        let points_i = ligne[i].getAttribute("points").split(" ");
                        let points_j = ligne[j].getAttribute("points").split(" ");
                        let x_i = parseFloat(points_i[1].split(",")[0]);
                        let x_j = parseFloat(points_j[1].split(",")[0]);
                        let Xmin = Math.min(x_i, x_j);
                        pointconnexion.push(document.createElementNS(NS, "circle"));
                        const pt_connexion = svg.createSVGPoint();
                        pt_connexion.x = Xmin;
                        pt_connexion.y = parseFloat(points_i[1].split(",")[1]);
                        pt_connexion.x = dXYmin*Math.round(pt_connexion.x/dXYmin);
                       // pt_connexion.y = dXYmin*Math.round(pt_connexion.y/dXYmin);
                        let CoulSignal = ligne[j].getAttribute("stroke");
                        pointconnexion[pointconnexion.length - 1].setAttribute("cx", pt_connexion.x);
                        pointconnexion[pointconnexion.length - 1].setAttribute("cy", pt_connexion.y);
                        pointconnexion[pointconnexion.length - 1].setAttribute("r", 2);
                        pointconnexion[pointconnexion.length - 1].setAttribute("fill", SortieNum ? "var(--act-lignenum)" : "var(--act-lignebin)");
                        svg.appendChild(pointconnexion[pointconnexion.length - 1]);
                    }
                }
            }

        }
    }
}
function Transit_Unite_Bloc(bl) { //Transit unité entrée bloc vers sortie          
    if (bloc[bl].Value == "moyenne" && bloc[bl].LesEntrees[0].Unit != null) {
        bloc[bl].LesSorties[0].Unit = bloc[bl].LesEntrees[0].Unit; //On passe l'unité dans la moyenne
        bloc[bl].LesSorties[0].type = bloc[bl].LesEntrees[0].type; //On passe le type dans la moyenne
    }
}
function Transit_Unite_Ligne(i) { //Transit unité sortie bloc vers entrée blocfiltre
    let db_sortie = ligne[i].getAttribute("data-blocSortie");
    let pin_Sortie = ligne[i].getAttribute("data-pinSortie");
    let db_entree = ligne[i].getAttribute("data-blocEntree");
    let pin_entree = ligne[i].getAttribute("data-pinEntree");

    if (db_sortie !== null && db_entree !== null && pin_entree !== null && pin_Sortie !== null) {
        bloc[db_entree].LesEntrees[pin_entree].Unit = bloc[db_sortie].LesSorties[pin_Sortie].Unit;
        bloc[db_entree].LesEntrees[pin_entree].type = bloc[db_sortie].LesSorties[pin_Sortie].type;
    }
}
function RemoveLigne(LigneC) {
    let ibE = ligne[LigneC].getAttribute("data-blocEntree");
    let pinE = ligne[LigneC].getAttribute("data-pinEntree");
    if (pinE !== null) {
        bloc[ibE].LesEntrees[pinE].cable = false;
        layout_bloc(ibE);
    }
    svg.removeChild(ligne[LigneC]);
    ligne[LigneC] = null;
    svg.removeChild(ligneGrosse[LigneC]);
    ligneGrosse[LigneC] = null;
    points_branchements();
}

function RemoveBloc(bl) {
    for (let i = 0; i < ligne.length; i++) {
        if (ligne[i] !== null) {
            let db_sortie = ligne[i].getAttribute("data-blocSortie");
            let db_entree = ligne[i].getAttribute("data-blocEntree");
            if (db_sortie == bl || db_entree == bl) RemoveLigne(i);
        }
    }
    if (bloc[bl].Value == "triac") {
        if (nbTriac > 0) nbTriac--;
    }
    svg.removeChild(bloc[bl].Svg);
    bloc[bl] = null;
    points_branchements();
    info.style.display = "none";
}




function AfficheInfo(I, e) {
    // position du conteneur
    const p = getSVGPoint(e);
    const x = e.clientX;
    const y = e.clientY;
    const zoneRect = GID("zone").getBoundingClientRect();

    // placement du popup
    let X = x - zoneRect.left + 15;
    if (x - zoneRect.left > zoneRect.width / 2) X = x - zoneRect.left - 300;
    info.style.left = X + "px";
    info.style.top = (y - zoneRect.top - 10) + "px";

    info.style.display = "block";

    info.innerHTML = I;

}


let decalageX_Info = 0;
let decalageY_Info = 0;
let deplacement_Info = false;
let info_Deplace = false;
let debutX_Info = 0;
let debutY_Info = 0;

info.addEventListener("pointerdown", function (e) {
    deplacement_Info = true;
    info_Deplace = false;

    debutX_Info = e.clientX;
    debutY_Info = e.clientY;

    decalageX_Info = e.clientX - info.getBoundingClientRect().left;
    decalageY_Info = e.clientY - info.getBoundingClientRect().top;

    // e.preventDefault();
});

document.addEventListener("pointermove", function (e) {
    if (!deplacement_Info) return;

    // Détecte un déplacement réel de plus de 5 pixels
    if (Math.abs(e.clientX - debutX_Info) > 5 ||
        Math.abs(e.clientY - debutY_Info) > 5) {
        info_Deplace = true;
    }

    info.style.left = (e.clientX - decalageX_Info + window.scrollX) + "px";
    info.style.top = (e.clientY - decalageY_Info + window.scrollY) + "px";
});

document.addEventListener("pointerup", function () {
    deplacement_Info = false;
});

info.addEventListener("click", function (e) {
    if (info_Deplace) {
        e.preventDefault();
        e.stopPropagation();
        info_Deplace = false;
        return;
    }

    info.style.display = "none";
    paraCourbesPID.style.display = "none";
    Current_Bl = -1;
});
function SendValues() {

    const dataActions = {
        blocs: [],
        lignes: [],
        FreqPWM: document.querySelector('input[name="Fpwm"]:checked').value,
        ReacCACSI: document.querySelector('input[name="ReacCACSI"]:checked').value
    };
    let RenumBlocs = []; //Renumérotation des blocs pour éviter les trous dans la numérotation
    let idxBloc = 0;
    for (let bl = 0; bl < bloc.length; bl++) {
        if (bloc[bl]) {
            const coordBloc = coord_Bloc(bl);
            let LesEntrees = []; //On n'enregistre que certains paramètres des entrées et sorties pour éviter de surcharger le fichier JSON
            for (let pin = 0; pin < bloc[bl].LesEntrees.length; pin++) {
                LesEntrees.push({
                    nom: bloc[bl].LesEntrees[pin].nom,
                    Unit: bloc[bl].LesEntrees[pin].Unit,
                    EntreeNum: bloc[bl].LesEntrees[pin].EntreeNum,
                    cable: bloc[bl].LesEntrees[pin].cable,
                    type: bloc[bl].LesEntrees[pin].type
                });
            }

            let LesSorties = [];
            for (let pinSor = 0; pinSor < bloc[bl].LesSorties.length; pinSor++) {
                LesSorties.push({
                    nom: bloc[bl].LesSorties[pinSor].nom,
                    Unit: bloc[bl].LesSorties[pinSor].Unit,
                    SortieNum: bloc[bl].LesSorties[pinSor].SortieNum,
                    type: bloc[bl].LesSorties[pinSor].type
                });
            }
            dataActions.blocs.push({
                type: bloc[bl].type,
                value: bloc[bl].Value,
                LesEntrees: LesEntrees,
                LesSorties: LesSorties,
                Para: bloc[bl].Para,
                x: Math.round(coordBloc[0]),
                y: Math.round(coordBloc[1])
            });
            RenumBlocs[bl] = idxBloc;
            idxBloc++;
        }
    }


    for (let i = 0; i < ligne.length; i++) {

        if (ligne[i]) {
            dataActions.lignes.push({
                blocEntree: RenumBlocs[ligne[i].getAttribute("data-blocEntree")],
                pinEntree: ligne[i].getAttribute("data-pinEntree"),
                blocSortie: RenumBlocs[ligne[i].getAttribute("data-blocSortie")],
                pinSortie: ligne[i].getAttribute("data-pinSortie")
            });
        }
    }

    const jsonData = JSON.stringify(dataActions);
    let adr = adrHttpRemote + "/ajaxNewConfAction";

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

//=============== Websocket =======================
let LePort = window.location.port;
if (LePort == "") LePort = "80";
let wsAct;
if (location.hostname == "localhost") {
    wsAct = new WebSocket("ws://" + RemoteIP + "/wsAct");
} else {
    wsAct = new WebSocket("ws://" + window.location.hostname + ":" + LePort + "/wsAct");
}
wsAct.onmessage = (event) => {

    let msg = JSON.parse(event.data);

    switch (msg.MessageType) {
        case "SortieBlocs":
            MessageSortieBlocs(msg);
            Message_PID(msg);
            break;


        default:
            console.warn("Unknown message", msg);
    }
};

function MessageSortieBlocs(msg) {
    let NbBlocs = msg.NbBlocs;
    let SortieBin = msg.SortieBin;
    let SortieNum0 = msg.SortieNum0;
    let SortieNum1 = msg.SortieNum1;
    for (let bl = 0; bl < bloc.length; bl++) {
        if (bloc[bl]) {
            Transit_Unite_Bloc(bl);
            for (let pinSor = 0; pinSor < bloc[bl].LesSorties.length; pinSor++) {
                let Unit = bloc[bl].LesSorties[pinSor].Unit;
                let SortieNum = bloc[bl].LesSorties[pinSor].SortieNum;
                let Sortie = "";
                if (SortieNum) {
                    if (pinSor == 1) {
                        Sortie = SortieNum1[bl];
                    } else {
                        Sortie = SortieNum0[bl];
                    }
                    if (Unit == "Hmn") Sortie = heureDecimaleVersHM(Sortie);
                } else {
                    Sortie = (SortieBin[bl] ? "1" : "0");
                }
                if (Sortie == null) Sortie = "";
                if (Unit == null) Unit = "";
                bloc[bl].LesSorties[pinSor].SvgV.textContent = Sortie + " " + Unit;
            }
        }
    }
}



//=============== Fin Websocket =======================

