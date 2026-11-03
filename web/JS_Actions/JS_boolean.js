
// === Traçage des blocs booléen === 
function boolTrace(bl, w, h) {
    const Value = bloc[bl].Value;
    const B = document.createElementNS(NS, "g");
    let line1, line2, nom;

    line1 = document.createElementNS(NS, "polygon");
    if (Value == "non") {
        line1.setAttribute("points", `0,0 ${w - 6},${h / 2} 0,${h}`);

        line2 = document.createElementNS(NS, "circle"); //Cercle de droite
        line2.setAttribute("cy", `${h / 2}`);
        line2.setAttribute("fill", "var(--act-CoulNon)");

        line2.setAttribute("cx", w - 3);
        line2.setAttribute("r", 3);
        nom = "NON";
    } else if (Value == "bascule") {
        line1.setAttribute("points", `0,0 ${w},0 ${w},${h} 0,${h} 0,0`);
        nom = "Bascule";
    } else {
        let dX = [0, 2, 3.5, 4.7, 5.7, 6, 5.7, 4.7, 3.5, 2, 0];
        let R = w - 6;
        let P = `0,0 ${R},0 `;
        const N = dX.length - 1;
        for (let i = 0; i <= N; i++) {
            P += `${dX[i] + R},${i * h / N} `;
        }
        P += `${R},${h} 0,${h}`;


        if (Value == "et") {

            nom = "ET";
        } else { //ou et ou_ex
            for (let i = 0; i <= N; i++) {
                P += ` ${dX[i] - 5},${(N - i) * h / N}`;
            }
            nom = "OU";
            if (Value == "ou_ex") {
                let P2 = "";
                line2 = document.createElementNS(NS, "polyline");
                for (let i = 0; i <= N; i++) {
                    P2 += ` ${dX[i]},${i * h / N}`;
                }
                line2.setAttribute("points", P2);
                line2.setAttribute("fill", "none");
                nom = "OU-EX";
            }
        }
        line1.setAttribute("points", P);

    }
    line1.setAttribute("fill", "var(--act-CoulLin)");
    line1.setAttribute("stroke", "var(--act-CoulStr)");
    line1.setAttribute("stroke-width", 0.5);
    line1.setAttribute("filter", "url(#ombre)");



    const Texte = document.createElementNS(NS, "text");
    Texte.setAttribute("x", '10');
    Texte.setAttribute("y", `${h / 2 + 2}`);
    Texte.textContent = nom;
    Texte.setAttribute("font-size", "5");
    Texte.setAttribute("fill", "var(--act-CoulTxt)");

    // 3. Assembler dans le bon ordre 


    B.appendChild(line1);
    if (line2) {
        line2.setAttribute("stroke", "var(--act-CoulBrd)");
        line2.setAttribute("stroke-width", 1);
        B.appendChild(line2);
    }

    B.appendChild(Texte);

    return B;
}

//Traçage des blocs textuels au centre
function bloc_Label(bl, w, h) {
 
    let CoulFond = "var(--act-CoulLab)"; //Fond éléments gris par defaut
    let Label_ = bloc[bl].Para.Label;
    let Nom_ = bloc[bl].Para.Nom;
    let type = bloc[bl].type;
    let Value = bloc[bl].Value;
    let Label2;
    if (type == "puissance" || type == "heure" || type == "temperature" || Value == "moyenne" || Value == "mqttnum"|| type == "addsub" ) {
        CoulFond =  "var(--act-CoulNum)";
    }
    if ( type == "ActExt_Ouvre" || type == "ActExt_Hequiv") {
        CoulFond = "var(--act-CoulAct)";
       let parts = Label_.split("/");
       Label_ = parts[0];
       Label2 = parts[1] ? parts[1] : null;
    }

    if (type == "filtre") {
        let leSeuil = bloc[bl].Para.Seuil;
        let Unit = "";
      
        if (bloc[bl].LesEntrees.length > 0) {
            if (bloc[bl].LesEntrees[0].type) {
                let typeEntree = bloc[bl].LesEntrees[0].type;
                Unit = "W";
                if (typeEntree == "temperature") Unit = "°C";
                if (typeEntree == "heure" || typeEntree == "ActExt_Hequiv") {
                    Unit = "";
                    leSeuil = heureDecimaleVersHM(parseFloat(leSeuil));
                }
            }
        }
        if (Value == "retard" || Value == "moyenne" || Value == "timer") {
            Unit = "s";
            leSeuil = parseFloat(leSeuil);
            if (leSeuil < 0) {
                leSeuil = 0;
                bloc[bl].Para.Seuil = leSeuil;
            }

        }
        Label_ = bloc[bl].Para.Label + " " + leSeuil + Unit;
    }
    const B = document.createElementNS(NS, "g");
    if (type == "action") {
        CoulFond = "var(--act-CoulAct)";
        Label_ = Value == "ssr" ? "SSR" : "Triac";
    }


    const rect = document.createElementNS(NS, "rect");
    rect.setAttribute("y", 0);
    rect.setAttribute("x", 0);
    rect.setAttribute("width", w);
    rect.setAttribute("height", h);
    rect.setAttribute("fill", CoulFond);
    rect.setAttribute("rx", "var(--act-bordRad)");
    rect.setAttribute("ry", "var(--act-bordRad)");
    rect.setAttribute("stroke", "var(--act-CoulBor)");
    rect.setAttribute("stroke-width", 0.5);
    rect.setAttribute("filter", "url(#ombre)");
    B.appendChild(rect);


    const TexteLargeur = document.createElementNS(NS, "text"); //non visible
    TexteLargeur.setAttribute("font-size", "5");
    TexteLargeur.textContent = Label_;
    TexteLargeur.setAttribute("visibility", "hidden");
    svg.appendChild(TexteLargeur);
    let largeur = TexteLargeur.getComputedTextLength();
    const Texte = document.createElementNS(NS, "text");
    Texte.setAttribute("x", `${(w - largeur) / 2}`); //Centrage
    Texte.setAttribute("y", `${h / 2 + 2}`);
    Texte.textContent = Label_;
    Texte.setAttribute("font-size", "5");
    Texte.setAttribute("fill", "var(--act-CoulTxt)");
    B.appendChild(Texte);
    if (Label2) {
        const Texte2 = document.createElementNS(NS, "text");
        Texte2.setAttribute("x", `${7 + (w - largeur) / 2}`);
        Texte2.setAttribute("y", `${h / 2 + 8}`);
        Texte2.setAttribute("font-size", "5");
        Texte2.textContent = Label2;
        Texte2.setAttribute("fill", "var(--act-CoulTxt)");
        B.appendChild(Texte2);
    }

    if (type == "action") {
        // Zone Mode
        const rectM = document.createElementNS(NS, "rect");
        rectM.setAttribute("y", 2);
        rectM.setAttribute("x", 2);
        rectM.setAttribute("width", w / 3);
        rectM.setAttribute("height", h * 0.66);
        rectM.setAttribute("fill", "var(--act-CoulAcT)");
        rectM.setAttribute("rx", "var(--act-modeRad)");
        rectM.setAttribute("ry", "var(--act-modeRad)");
        B.appendChild(rectM);
        const TexteM = document.createElementNS(NS, "text");
        TexteM.setAttribute("x", 6); //Centrage
        TexteM.setAttribute("y", 6);
        TexteM.textContent = "Mode"
        TexteM.setAttribute("font-size", "4");
        TexteM.setAttribute("fill", "var(--act-CoulTxt)");
        B.appendChild(TexteM);

        //Nom donné par l'utilisateur
        if (Value == "ssr" && Nom_ == "") Nom_ = "SSR " + bl;
        if (Value == "triac" && Nom_ == "") Nom_ = "Triac ";

    }
    if (Nom_ != "") {
        TexteLargeur.textContent = Nom_;
        largeur = TexteLargeur.getComputedTextLength();
        const TexteNom = document.createElementNS(NS, "text");
        TexteNom.setAttribute("x", `${(w - largeur) / 2}`);
        TexteNom.setAttribute("y", `${h - 2}`);
        TexteNom.setAttribute("font-size", "5");
        TexteNom.textContent = Nom_;
        TexteNom.setAttribute("fill", "var(--act-CoulTxt)");
        B.appendChild(TexteNom);
    }
    bloc[bl].Para.Nom = Nom_;

    return B;
}

function heureDecimaleVersHM(decimal) {
    if (decimal == null || isNaN(decimal)) return "";
    const heures = Math.floor(decimal);
    const minutes = Math.round((decimal - heures) * 60);
    return String(heures).padStart(2, 0) + ":" + String(minutes).padStart(2, "0");
}

