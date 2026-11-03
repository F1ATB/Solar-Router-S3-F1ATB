function createCarteGraphEnsoleillement(z) {
    let contenu = "<div  >Ensoleillement</div>";
    contenu += '<div class="hom-soleil-edit hom-boxEdit edit">';
    contenu += '<div>Graphique :</div>';
    contenu += `<input type="radio" id="h48_${z}" name="soleil_${z}" value="h48" onclick="chgSoleil(this)">`;
    contenu += `<label for="h48_${z}">48h</label><br>`;
    contenu += `<input type="radio" id="h24_${z}" name="soleil_${z}" value="h24" onclick="chgSoleil(this)">`;
    contenu += `<label for="h24_${z}">24h</label><br>`;
    contenu += `<input type="radio" id="j2_${z}" name="soleil_${z}" value="j2" onclick="chgSoleil(this)">`;
    contenu += `<label for="j2_${z}">2 Jours</label>`;
    contenu += '</div>';
    contenu += "<svg id='Soleil_SVG_" + z + "' viewBox='0 0 510 100' class='hom-SoleilSVG'></svg>";
    if (ListeZones[z].para == null) {

        let para = {
            type_graph: "h48"
        };
        ListeZones[z].para = para;
    }
    return contenu;
}
Soleil = null;// Valeurs ensoleillement

function chgSoleil(t) {
    let z = parseInt(t.id.split("_")[1]);
    ListeZones[z].para.type_graph = t.value;
    TraceSoleil(z);
}
function TraceLesSoleils(msg) {
    Soleil = msg.Soleil;
    for (let zone = 0; zone < ListeZones.length; zone++) {
        if (ListeZones[zone].type == "graphEnsoleillement") TraceSoleil(zone);
    }
}
function TraceSoleil(z) {
    if (Soleil != null) {
        let type_graph = ListeZones[z].para.type_graph;
        GID(type_graph + "_" + z).checked = true;

        const NS = "http://www.w3.org/2000/svg";
        const SolSvg = GID("Soleil_SVG_" + z);
        const X0Sol = 20;
        const Y0Sol = 60;
        let x_Max = 480;
        let hMin = 0;
        let hMax = 48;
        let Wb = 6; //largeur building
        let xJ1 = 250;
        let dy = 8;
        let fsT = 9;
        let fs = 7;
        SolSvg.innerHTML = "";
        let viewBox = '0 0 510 100';
        if (type_graph == "h24") {
            x_Max = 180;
            hMin = 5;
            hMax = 23;
            xJ1 = 130;
            fsT=7;
            fs = 5;
            dy=7;
            viewBox = '0 0 210 100';
        }
        if (type_graph == "j2") {
            x_Max = 120;
            xJ1 = 70;
            viewBox = '0 0 150 75';
            fsT=5;
            fs = 4;
            dy=6;
        }
        SolSvg.setAttribute("viewBox", viewBox);

        //Trace Axes
        let axes = document.createElementNS(NS, "polyline");
        axes.setAttribute("points", `${X0Sol},${Y0Sol - 50} ${X0Sol},${Y0Sol} ${X0Sol + x_Max},${Y0Sol}`);
        axes.setAttribute("stroke", "var(--hom-solaxes)");
        axes.setAttribute("stroke-width", 1);
        axes.setAttribute("fill", "none");
        SolSvg.appendChild(axes);
        if (type_graph != "j2") {
            for (let h = hMin; h < hMax; h++) {
                let x = X0Sol + (h - hMin) * 10;
                let y = Y0Sol;
                let tick = document.createElementNS(NS, "line");
                tick.setAttribute("x1", x);
                tick.setAttribute("y1", y);
                tick.setAttribute("x2", x);
                tick.setAttribute("y2", y + 4);
                tick.setAttribute("stroke", "var(--hom-solaxes)");
                tick.setAttribute("stroke-width", 1);
                SolSvg.appendChild(tick);
                let txt = document.createElementNS(NS, "text");
                txt.setAttribute("x", x);
                txt.setAttribute("y", y + 9);
                txt.setAttribute("fill", "var(--hom-sollabel)");
                txt.textContent = h % 24;
                txt.setAttribute("font-size", "5");
                SolSvg.appendChild(txt);
            }
        }

        for (let P = 0; P <= 1000; P = P + 200) {
            let x = X0Sol;
            let y = Y0Sol - P / 20;
            let tick = document.createElementNS(NS, "line");
            tick.setAttribute("x1", x);
            tick.setAttribute("y1", y);
            tick.setAttribute("x2", x - 4);
            tick.setAttribute("y2", y);
            tick.setAttribute("stroke", "var(--hom-solaxes)");
            tick.setAttribute("stroke-width", 1);
            SolSvg.appendChild(tick);
            let txt = document.createElementNS(NS, "text");
            txt.setAttribute("x", x - 5);
            txt.setAttribute("y", y);
            txt.setAttribute("text-anchor", "end");
            txt.setAttribute("fill", "var(--hom-sollabel)");
            txt.textContent = P;
            txt.setAttribute("font-size", "5");
            SolSvg.appendChild(txt);
        }
        let txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol - 10);
        txt.setAttribute("y", Y0Sol - 55);
        txt.setAttribute("fill", "var(--hom-sollabel)");
        txt.textContent = "W/m²";
        txt.setAttribute("font-size", "5");
        SolSvg.appendChild(txt);

        //Trace courbe
        if (type_graph == "j2") {
            let x = X0Sol + 10;
            let H = Soleil.SoleilJour / 20;;
            let y = Y0Sol - H;
            let rect = document.createElementNS(NS, "rect");
            rect.setAttribute("x", x);
            rect.setAttribute("y", y);
            rect.setAttribute("width", 40);
            rect.setAttribute("height", H);
            rect.setAttribute("fill", "var(--hom-solcourbeJ)");
            SolSvg.appendChild(rect);

            x = X0Sol + 70;
            H = Soleil.SoleilDemain / 20;;
            y = Y0Sol - H;
            rect = document.createElementNS(NS, "rect");
            rect.setAttribute("x", x);
            rect.setAttribute("y", y);
            rect.setAttribute("width", 40);
            rect.setAttribute("height", H);
            rect.setAttribute("fill", "var(--hom-solcourbeJ1)");
            SolSvg.appendChild(rect);

        } else { //h48 et h24
            for (let h = 0; h < 48; h++) {
                let hX = h * 10;
                if (type_graph == "h24") {
                    hX = (h % 24) * 10;
                    Wb = 4;
                    if (h > 23) {
                        hX += 5;
                        Wb = 2;
                    }
                    hX = hX - (hMin * 10);
                }
                if (hX >= 0 && hX <= x_Max) {
                    let x = X0Sol + hX + 2;
                    let H = Soleil.Wm2[h] / 20;;
                    let y = Y0Sol - H;

                    let rect = document.createElementNS(NS, "rect");
                    rect.setAttribute("x", x);
                    rect.setAttribute("y", y);
                    rect.setAttribute("width", Wb);
                    rect.setAttribute("height", H);
                    rect.setAttribute("fill", h > 23 ? "var(--hom-solcourbeJ1)" : "var(--hom-solcourbeJ)");
                    SolSvg.appendChild(rect);
                }
            }



        }

        //Jour J
        let yJ = 10;
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol + 10);
        txt.setAttribute("y", yJ);
        txt.setAttribute("fill", "var(--hom-solcourbeJ)");
        txt.textContent = "Jour J";
        txt.setAttribute("font-size", fsT);
        SolSvg.appendChild(txt);

        yJ += dy;
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol + 10);
        txt.setAttribute("y", yJ);
        txt.setAttribute("fill", "var(--hom-solcourbeJ)");
        txt.textContent = "Moyen = " + Soleil.SoleilJour + " W/m²";
        txt.setAttribute("font-size", fs);
        SolSvg.appendChild(txt);

        let Max = 0;
        let Energie = 0;
        for (let h = 0; h < 24; h++) {
            if (Soleil.Wm2[h] > Max) Max = Soleil.Wm2[h];
            Energie += Soleil.Wm2[h];
        }
        yJ += dy;
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol + 10);
        txt.setAttribute("y", yJ);
        txt.setAttribute("fill", "var(--hom-solcourbeJ)");
        txt.textContent = "Maximum = " + Max + " W/m²";
        txt.setAttribute("font-size", fs);
        SolSvg.appendChild(txt);
        yJ += dy;
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol + 10);
        txt.setAttribute("y", yJ);
        txt.setAttribute("fill", "var(--hom-solcourbeJ)");
        txt.textContent = "Energie = " + Energie + " Wh/m²";
        txt.setAttribute("font-size", fs);
        SolSvg.appendChild(txt);
        yJ += dy;
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol + 10);
        txt.setAttribute("y", yJ);
        txt.setAttribute("fill", "var(--hom-solactuel)");
        txt.textContent = "Actuel = " + Soleil.Wm2[Soleil.Int_Heure] + " W/m²";
        txt.setAttribute("font-size", fs);
        SolSvg.appendChild(txt);

        //Jour J+1
        yJ = 10;
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol + xJ1);
        txt.setAttribute("y", yJ);
        txt.setAttribute("fill", "var(--hom-solcourbeJ1)");
        txt.textContent = "Jour J+1";
        txt.setAttribute("font-size", fsT);
        SolSvg.appendChild(txt);
        yJ += dy;
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol + xJ1);
        txt.setAttribute("y", yJ);
        txt.setAttribute("fill", "var(--hom-solcourbeJ1)");
        txt.textContent = "Moyen = " + Soleil.SoleilDemain + " W/m²";
        txt.setAttribute("font-size", fs);
        SolSvg.appendChild(txt);

        Max = 0;
        Energie = 0;
        for (let h = 24; h < 48; h++) {
            if (Soleil.Wm2[h] > Max) Max = Soleil.Wm2[h];
            Energie += Soleil.Wm2[h];
        }
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol + xJ1);
        yJ += dy;
        txt.setAttribute("y", yJ);
        txt.setAttribute("fill", "var(--hom-solcourbeJ1)");
        txt.textContent = "Maximum = " + Max + " W/m²";
        txt.setAttribute("font-size", fs);
        SolSvg.appendChild(txt);

        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", X0Sol + xJ1);
        yJ += dy;
        txt.setAttribute("y", yJ);
        txt.setAttribute("fill", "var(--hom-solcourbeJ1)");
        txt.textContent = "Energie = " + Energie + " Wh/m²";
        txt.setAttribute("font-size", fs);
        SolSvg.appendChild(txt);


        let x = X0Sol + (Soleil.Int_Heure - hMin) * 10;
        wb = 10;
        if (type_graph == "j2") {
            x = X0Sol + 1.2 * (Soleil.Int_Heure) + 10;
            wb = 5;
        }
        let rect = document.createElementNS(NS, "rect");
        rect.setAttribute("x", x);
        rect.setAttribute("y", Y0Sol - Soleil.Wm2[Soleil.Int_Heure] / 20);
        rect.setAttribute("width", wb);
        rect.setAttribute("height", Soleil.Wm2[Soleil.Int_Heure] / 20);
        rect.setAttribute("fill", "var(--hom-solactuel)");
        SolSvg.appendChild(rect);



        //Lever coucher du soleil actuel
        let xR = X0Sol + Soleil.SunRiseJour / 10 - hMin * 10 - 8;
        let xS = X0Sol + Soleil.SunSetJour / 10 - hMin * 15 - 8;
        let y = Y0Sol + 30;
        let fS = "15";
        if (type_graph == "j2") {
            xR = X0Sol + 10;
            xS = X0Sol + 70;
            y = Y0Sol + 10;
            fS = "7";
        }
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", xR);
        txt.setAttribute("y", y);
        txt.setAttribute("fill", "var(--hom-solcourbeJ)");
        txt.textContent = "🌅" + formatHeureSun(Soleil.SunRiseJour);
        txt.setAttribute("font-size", fS);
        SolSvg.appendChild(txt);
        //Lever coucher du soleil demain
        txt = document.createElementNS(NS, "text");
        txt.setAttribute("x", xS);
        txt.setAttribute("y", y);
        txt.setAttribute("fill", "var(--hom-solcourbeJ)");
        txt.textContent = "🌇" + formatHeureSun(Soleil.SunSetJour);
        txt.setAttribute("font-size", fS);
        SolSvg.appendChild(txt);

        //Lever coucher du soleil demain
        if (type_graph == "h48") {
            txt = document.createElementNS(NS, "text");
            txt.setAttribute("x", X0Sol + Soleil.SunRiseDemain / 10 - 8 + 240);
            txt.setAttribute("y", Y0Sol + 30);
            txt.setAttribute("fill", "var(--hom-solcourbeJ1)");
            txt.textContent = "🌅" + formatHeureSun(Soleil.SunRiseDemain);
            txt.setAttribute("font-size", "15");
            SolSvg.appendChild(txt);

            //Lever coucher du soleil demain
            txt = document.createElementNS(NS, "text");
            let xMax = Math.min(X0Sol + Soleil.SunSetDemain / 10 - 8 + 240, X0Sol + 440);
            txt.setAttribute("x", xMax);
            txt.setAttribute("y", Y0Sol + 30);
            txt.setAttribute("fill", "var(--hom-solcourbeJ1)");
            txt.textContent = "🌇" + formatHeureSun(Soleil.SunSetDemain);
            txt.setAttribute("font-size", "15");
            SolSvg.appendChild(txt);
        }

    }
}
function formatHeureSun(heure) {
    let Heure = Math.floor(heure / 100);
    let minute = Math.floor(heure % 100);
    return Heure.toString().padStart(2, "0") + ":" + minute.toString().padStart(2, "0");
}