
//======= Paramétrage des Blocs =======

const ListeMQTTbin = ["toto", "titi", "tata"];
const ListeMQTTnum = ["Ntoto", "Ntiti", "Ntata"];
function dispPara(bl) {
    const type = bloc[bl].type;
    const Value = bloc[bl].Value;
    let s = "";
    let coulFond = "var(--act-coulDef)";
    s += "<h4>Référence</h4>";
    //=========================
    s += `NumBloc : ${bl}<br>`;
    s += `Type : ${bloc[bl].type}<br>Valeur : ${bloc[bl].Value}<br>`;
    if (bloc[bl].LesEntrees.length > 0) {
        s += "<h4>Connexions</h4>";
        //=========================
        for (let i = 0; i < bloc[bl].LesEntrees.length; i++) {
            if (bloc[bl].LesEntrees[i].nom != "" && bloc[bl].LesEntrees[i].nom != null) {
                s += `Entrée  ${bloc[bl].LesEntrees[i].nom} : `;
            } else {
                s += `Entrée ${i} : `;
            }
            s += `${bloc[bl].LesEntrees[i].cable ? "connectée" : "libre"}<br>`;
        }
    }
    if (type == "puissance" || type == "heure" || type == "temperature" || Value == "moyenne" || Value == "mqttnum" || type == "addsub") {
        coulFond = "var(--act-coulNum)";
    }
    if (type == "ActExt_Ouvre" || type == "ActExt_Hequiv") {
        coulFond = "var(--act-coulAct)";
    }
    if (type == "bin") {
        coulFond = "var(--act-coulBin)";
    }
    let LesPara = "";
    switch (type) {
        case "filtre":
            let Seuil = bloc[bl].Para.Seuil;
            let UnitP = "";
            let typeInput = "number";
            LesPara += "<label>Seuil : </label>";
            if (bloc[bl].LesEntrees.length > 0) {
                if (bloc[bl].LesEntrees[0].type) {
                    let typeEntree = bloc[bl].LesEntrees[0].type;
                    if (typeEntree == "heure" && Value!="moyenne") {
                        typeInput = "time";
                        Seuil = heureDecimaleVersHM(Seuil);
                        UnitP="H:mn";
                        coulFond = "var(--act-coulNum)";
                    }
                    if (typeEntree == "temperature") {
                        UnitP = "°C";
                    }
                }
            }
            
            if (Value == "retard" || Value == "moyenne" || Value == "timer") {
            UnitP = "s";           
            }

        
            LesPara += `<input type="${typeInput}"  value="${Seuil}" onclick="NewSeuil(event,this,${bl});" onchange="NewSeuil(event,this,${bl});">${UnitP} `;
            LesPara += "<br>";
            break;
        case "commande":
            let LaListe = [];

            switch (bloc[bl].Value) {
                case "gpio":
                    LesPara += "<label>GPIO : </label>";
                    LaListe = ListeGPIO.slice();
                    break;
                case "mqttbin":
                    LesPara += "<label>MQTT binaire : </label>";
                    LaListe = ListeMQTTbin;
                    break;
                case "mqttnum":
                    LesPara += "<label>MQTT numérique : </label>";
                    LaListe = ListeMQTTnum;
                    break;
            }
            LesPara += `<select id='gpioSelect' onclick='event.stopPropagation();NewCommande(event,this,${bl});'>`;

            for (let i = 0; i < LaListe.length; i++) {
                let value = LaListe[i];
                let selected = (value == bloc[bl].Para.Commande) ? "selected" : "";
                LesPara += `<option value="${value}" ${selected}>${value}</option>`;
            }
            LesPara += "</select><br>";
            break;
        case "action":
            coulFond = "var(--act-coulAct)";
            //==Nom Action==
            LesPara += "<div class='paraB'><div class='tcL'>Nom : </div>";
            let nom = bloc[bl].Para.Nom ? bloc[bl].Para.Nom : "";
            LesPara += `<div class='tcR'><input type="text" value="${nom}" onclick="UpdatePara('Nom',event,this,${bl});" onchange="UpdatePara('Nom',event,this,${bl});"></div><div></div></div>`;
            //==Mode Regulation  Action==
            let LesModes = [];
            const ListeModes = ["Inactif", "On/Off", "Découpe-sinus", "Demi-sinus", "Multi-sinus", "Train de sinus", "PWM"];
            switch (bloc[bl].Value) {
                case "ssr":
                    LesModes = [Regulation.Inactif, Regulation.OnOff, Regulation.DemiSinus, Regulation.MultiSinus, Regulation.TrainSinus, Regulation.PWM];
                    break;
                case "triac":
                    LesModes = [Regulation.Inactif, Regulation.DecoupeSinus, Regulation.DemiSinus, Regulation.MultiSinus, Regulation.TrainSinus];
                    break;
            }
            LesPara += "<div class='paraB'><div class='tcL'>Regulation : </div>";
            LesPara += `<div class='tcR'><select id='modeRegul' onclick="UpdatePara('Regulation',event,this,${bl});" onchange="UpdatePara('Regulation',event,this,${bl});">`;
            for (let i = 0; i < LesModes.length; i++) {
                let value = LesModes[i];
                let selected = (value == bloc[bl].Para.Regulation) ? "selected" : "";
                LesPara += `<option   value="${value}" ${selected}>${ListeModes[value]}</option>`;
            }
            LesPara += "</select></div><div></div></div>";
            //==GPIO Action==

            let ListeGPIO_ext = ListeGPIO.slice();
            if (bloc[bl].Para.Regulation == Regulation.OnOff) ListeGPIO_ext.push(99); //GPIO externe pour les SSR en mode On/Off
            LesPara += "<div id='paraGPIO'><div class='tcL'>GPIO : </div>";
            LesPara += `<div class='tcR'><select id='gpioSelect' onclick="UpdatePara('GPIO',event,this,${bl});" onchange="UpdatePara('GPIO',event,this,${bl});">`;
            for (let i = 0; i < ListeGPIO_ext.length; i++) {
                let value = ListeGPIO_ext[i];
                let V = value;
                if (value == 99) V = "Externe";
                let selected = (value == bloc[bl].Para.GPIO) ? "selected" : "";
                LesPara += `<option value="${value}" ${selected}>${V}</option>`;
            }
            LesPara += "</select></div><div></div></div>";

            //==VoltON Action==
            const LesVoltON = [0, 3.3];
            LesPara += "<div id='paraVolt'><div class='tcL'>Sortie ON : </div>";
            LesPara += `<div class='tcR'><select id='voltSelect' onclick="UpdatePara('VoltON',event,this,${bl});" onchange="UpdatePara('VoltON',event,this,${bl});">`;
            for (let i = 0; i < LesVoltON.length; i++) {
                let value = LesVoltON[i];
                let selected = (value == bloc[bl].Para.VoltON) ? "selected" : "";
                LesPara += `<option value="${i}" ${selected}>${value}</option>`;
            }
            LesPara += "</select></div><div class='tcL'>V</div></div>";
            //==Temporisation Action==
            LesPara += "<div id='paraTempo'><div class='tcL'>Temporisation : </div>";
            LesPara += `<div class='tcR'><input type="number"  step="1" value="${bloc[bl].Para.Temporisation}" onclick="UpdatePara('Temporisation',event,this,${bl});" onchange="UpdatePara('Temporisation',event,this,${bl});"></div><div class='tcL'>s</div></div>`;
            //== Ouvre Max ==
            LesPara += "<div id='paraMax'><div class='tcL'>Ouvre Max : </div>";
            LesPara += `<div class='tcR'><input type="number"  step="1" value="${bloc[bl].Para.OuvreMax}" onclick="UpdatePara('OuvreMax',event,this,${bl});" onchange="UpdatePara('OuvreMax',event,this,${bl});"></div><div class='tcL'>%</div></div>`;
            //== Ouvre Force ==
            LesPara += "<div id='paraForce'><div class='tcL'>Ouvre Force : </div>";
            LesPara += `<div class='tcR'><input type="number"  step="1" value="${bloc[bl].Para.OuvreForce}" onclick="UpdatePara('OuvreForce',event,this,${bl});" onchange="UpdatePara('OuvreForce',event,this,${bl});"></div><div class='tcL'>%</div></div>`;
            //== Seuil PW On ==
            LesPara += "<div id='paraSeuilPW_On'><div class='tcL'>Seuil PW On : </div>";
            LesPara += `<div class='tcR'><input type="number" value="${bloc[bl].Para.SeuilPW_On}" onclick="UpdatePara('SeuilPW_On',event,this,${bl});" onchange="UpdatePara('SeuilPW_On',event,this,${bl});"></div><div class='tcL'>W</div></div>`;
            //Seuil PW Off
            LesPara += "<div id='paraSeuilPW_Off'><div class='tcL'>Seuil PW Off : </div>";
            LesPara += `<div class='tcR'><input type="number" value="${bloc[bl].Para.SeuilPW_Off}" onclick="UpdatePara('SeuilPW_Off',event,this,${bl});" onchange="UpdatePara('SeuilPW_Off',event,this,${bl});"></div><div class='tcL'>W</div></div>`;
            //== PID ON ==
            LesPara += "<div id='paraPID'><div class='tcL'>PID On : </div>";
            let checked = bloc[bl].Para.PID_On ? "checked" : "";
            LesPara += `<div><input type="checkbox" ${checked} id="PID_On_${bl}" onclick="UpdatePara('PID_On',event,this,${bl});" onchange="UpdatePara('PID_On',event,this,${bl});"></div><div></div></div>`;
            //== P ==
            LesPara += "<div id='paraP'><div class='tcL'>P<span class='fsize10'>(Proportionnel)</span> : </div>";
            LesPara += `<div class='tcR'><input type="number" step="1" value="${bloc[bl].Para.P}" onclick="UpdatePara('P',event,this,${bl});" onchange="UpdatePara('P',event,this,${bl});"></div><div></div></div>`;
            //== I ==
            LesPara += "<div id='paraI'><div class='tcL'>I<span class='fsize10'>(Intégral ou réactivité)</span> : </div>";
            LesPara += `<div class='tcR'><input type="number"  step="1" value="${bloc[bl].Para.I}" onclick="UpdatePara('I',event,this,${bl});" onchange="UpdatePara('I',event,this,${bl});"></div><div style="cursor:pointer;" onClick="AffichePID(event,${bl});">📈</div></div>`;
            //== D ==
            LesPara += "<div id='paraD'><div  class='tcL'>D<span class='fsize10'>(Dérivé)</span> : </div>";
            LesPara += `<div class='tcR'><input type="number" step="1" value="${bloc[bl].Para.D}" onclick="UpdatePara('D',event,this,${bl});" onchange="UpdatePara('D',event,this,${bl});"></div><div></div></div>`;
            //==Host==
            LesPara += "<div id='paraHost'><div class='tcL'>Host : </div>";
            LesPara += `<div class='tcR'><input type="text" value="${bloc[bl].Para.Host}" onclick="UpdatePara('Host',event,this,${bl});" onchange="UpdatePara('Host',event,this,${bl});"></div><div></div></div>`;
            //==Port==
            LesPara += "<div id='paraPort'><div class='tcL'>Port : </div>";
            LesPara += `<div class='tcR'><input type="number"  step="1" value="${bloc[bl].Para.Port}" onclick="UpdatePara('Port',event,this,${bl});" onchange="UpdatePara('Port',event,this,${bl});"></div><div></div></div>`;
            //==GetPost==
            const LesGP = ["GET", "POST"];
            LesPara += "<div class='paraB' id='paraGetPost'><div class='tcL'>Méthode : </div>";
            LesPara += `<div class='tcR'><select id='methodSelect' onclick='UpdatePara('GetPost',event,this,${bl});' onchange='UpdatePara('GetPost',event,this,${bl});'>`;
            for (let i = 0; i < LesGP.length; i++) {
                let value = LesGP[i];
                let selected = (value == bloc[bl].Para.GetPost) ? "selected" : "";
                LesPara += `<option value="${i}" ${selected}>${value}</option>`;
            }
            LesPara += "</select></div><div></div></div>";
            //==Ordre On==
            LesPara += "<div id='paraOrdreOn'><div class='tcL'>Ordre On : </div>";
            LesPara += `<div class='tcR'><input type="text" value="${bloc[bl].Para.OrdreOn}" onclick="UpdatePara('OrdreOn',event,this,${bl});" onchange="UpdatePara('OrdreOn',event,this,${bl});"></div><div></div></div>`;
            //==Ordre Off==
            LesPara += "<div id='paraOrdreOff'><div class='tcL'>Ordre Off : </div>";
            LesPara += `<div class='tcR'><input type="text" value="${bloc[bl].Para.OrdreOff}" onclick="UpdatePara('OrdreOff',event,this,${bl});" onchange="UpdatePara('OrdreOff',event,this,${bl});"></div><div></div></div>`;
            //==Repetition==
            LesPara += "<div id='paraRepetition'><div class='tcL'>Repetition : </div>";
            LesPara += `<div class='tcR'><input type="number"  step="1" value="${bloc[bl].Para.Repetition}" onclick="UpdatePara('Repetition',event,this,${bl});" onchange="UpdatePara('Repetition',event,this,${bl});"></div><div class='tcL'>s</div></div>`;



            break;
    }
    if (LesPara != "") {
        s += "<h4>Paramètres</h4>";
        //=========================
        s += LesPara;
    }
    //value="gpio" data-type="commande"
    s += `<span class="poubelle" onclick="RemoveBloc(${bl})">&#128465</span> Cliquer pour supprimer le bloc`;
    info.style.background = coulFond;

    return s;
}
function NewSeuil(e, t, db) {
    e.stopPropagation();
    let seuil = t.value;
    if (seuil.indexOf(":") > 0) {// C'est une heure
        let val = seuil.split(":");
        seuil = parseInt(val[0]) + parseInt(val[1]) / 60; //Heure en décimal
    }
    bloc[db].Para.Seuil = seuil;
    layout_bloc(db);
}
function NewCommande(e, t, db) {
    e.stopPropagation();
    let commande = t.value;
    bloc[db].Para.Commande = commande;
    layout_bloc(db);
}
function UpdatePara(variable, e, t, bl) {
    e.preventDefault();
    e.stopPropagation();
    info_Deplace = true;

    switch (variable) {
        case 'Nom':
            bloc[bl].Para.Nom = t.value;
            break;
        case 'Regulation':
            bloc[bl].Para.Regulation = t.value;
            break;
        case 'GPIO':
            bloc[bl].Para.GPIO = Math.abs(t.value);
            break;
        case 'VoltON':
            bloc[bl].Para.VoltON = t.value;
            break;
        case 'Temporisation':
            bloc[bl].Para.Temporisation = Math.abs(t.value);
            break;
        case 'OuvreMax':
            bloc[bl].Para.OuvreMax = clamp(t.value, 0, 100);
            break;
        case 'OuvreForce':
            bloc[bl].Para.OuvreForce = clamp(t.value, 0, 100);
            break;
        case 'P':
            bloc[bl].Para.P = clamp(t.value, 0, 100);
            if (e.type == 'change') Update_PID();  //Envoi pour regler en temps réel. On evite les clicks
            break;
        case 'I':
            bloc[bl].Para.I = clamp(t.value, 0, 100);
            if (e.type == 'change') Update_PID()
            break;
        case 'D':
            bloc[bl].Para.D = clamp(t.value, 0, 100);
            Update_PID();
            break;
        case 'PID_On':
            bloc[bl].Para.PID_On = !bloc[bl].Para.PID_On;
            setTimeout(function () { //Pb avec le click. On retarde
                GID(`PID_On_` + bl).checked = bloc[bl].Para.PID_On;
            }, 100);
            break;
        case 'OrdreOn':
            bloc[bl].Para.OrdreOn = t.value;
            break;
        case 'OrdreOff':
            bloc[bl].Para.OrdreOff = t.value;
            break;
        case 'Repetition':
            bloc[bl].Para.Repetition = Math.abs(t.value);
            break;
        case 'SeuilPW_On':
            bloc[bl].Para.SeuilPW_On = t.value;
            break;
        case 'SeuilPW_Off':
            bloc[bl].Para.SeuilPW_Off = t.value;
            break;
        case 'Host':
            bloc[bl].Para.Host = t.value;
            break;
        case 'Port':
            bloc[bl].Para.Port = Math.abs(t.value);
            break;
        case 'GetPost':
            bloc[bl].Para.GetPost = t.value;
            break;
    }



    checkParaDisable(bl);
    layout_bloc(bl);

}
function checkParaDisable(bl) {

    if (bloc[bl].type == "action") {
        let disp = (bloc[bl].Para.Regulation != Regulation.Inactif) ? "table-row" : "none";
        const lesChamps = ["paraGPIO", "paraVolt", "paraTempo", "paraMax", "paraForce", "paraSeuilPW_On", "paraSeuilPW_Off", "paraPID", "paraP", "paraI", "paraD", "paraHost", "paraPort", "paraGetPost", "paraOrdreOn", "paraOrdreOff", "paraRepetition"];
        for (let i = 0; i < lesChamps.length; i++) {
            GID(lesChamps[i]).style.display = disp;
        }
        if (bloc[bl].Value == "triac") GID("paraGPIO").style.display = "none";
        if (disp == "table-row") {
            let dispHost = "none";
            if (bloc[bl].Para.GPIO == 99) {
                bloc[bl].Para.Regulation = Regulation.OnOff;
                dispHost = "table-row";
                GID("modeRegul").value = Regulation.OnOff;
            }
            if (bloc[bl].Para.Regulation == Regulation.OnOff) bloc[bl].Para.PID_On = false;
            GID("paraHost").style.display = dispHost;
            GID("paraPort").style.display = dispHost;
            GID("paraGetPost").style.display = dispHost;
            GID("paraOrdreOn").style.display = dispHost;
            GID("paraOrdreOff").style.display = dispHost;
            GID("paraRepetition").style.display = dispHost;
            GID("paraTempo").style.display = (bloc[bl].Para.Regulation == Regulation.OnOff) ? "table-row" : "none";
            GID("paraMax").style.display = (bloc[bl].Para.Regulation != Regulation.OnOff) ? "table-row" : "none";
            GID("paraForce").style.display = (bloc[bl].Para.Regulation != Regulation.OnOff) ? "table-row" : "none";
            GID("paraPID").style.display = (bloc[bl].Para.Regulation != Regulation.OnOff) ? "table-row" : "none";
            GID("paraSeuilPW_Off").style.display = (bloc[bl].Para.Regulation == Regulation.OnOff) ? "table-row" : "none";
            GID("paraP").style.display = (bloc[bl].Para.PID_On) ? "table-row" : "none";
            GID("paraI").style.display = "table-row";
            GID("paraD").style.display = (bloc[bl].Para.PID_On) ? "table-row" : "none";
            const optionGPIO = document.querySelector('#gpioSelect option[value="99"]');
            if (optionGPIO) {
                optionGPIO.disabled = (bloc[bl].Para.Regulation != Regulation.OnOff);
            }


        }
    }
    checkParaCommun();
}

let LastEcart = 0;
let LastProp = 0;
let LastInteg = 0;
let LastDeriv = 0;
let LastOuvert = 0;
let LastRetard = 100;
let Ecart = [];
let Prop = [];
let Integ = [];
let Deriv = [];
let Retard = [];
Ouvert = [];
for (let i = 0; i < 150; i++) {
    Ecart[i] = 0;
    Prop[i] = 0;
    Integ[i] = 0;
    Deriv[i] = 0;
    Retard[i] = 100;
    Ouvert[i] = 0;
}


let TabIdx = -1;
let Current_Bl = -1;
let Current_Action = 0;
const paraCourbesPID = GID("paraCourbesPID");
function AffichePID(event, bl) { // Courbes détaillées PID

    event.stopPropagation();
    if (Current_Bl == bl) {
        paraCourbesPID.style.display = "none";
        Current_Bl = -1;


    } else {
        paraCourbesPID.style.display = "block";
        paraCourbesPID.style.top = info.style.top;
        let left = Math.max(parseInt(info.style.left) - 1000, 0) + "px";
        paraCourbesPID.style.left = left;
        Current_Bl = bl;
        PlotPID();
    }

}
function Message_PID(msg) {

    if (Current_Bl != -1) {
        let NumBloc = msg.NumBloc;
        let idx = -1;
        for (let i = 0; i < NumBloc.length; i++) {
            if (NumBloc[i] == Current_Bl) {
                idx = i;
                break;
            }
        }
        if (idx != -1) {
            LastEcart = msg.LastErrorPw[idx];
            LastProp = msg.P[idx];
            LastInteg = msg.I[idx];
            LastDeriv = msg.D[idx];
            LastOuvert = msg.SortieNum0[Current_Bl];
            LastRetard = 100 - LastOuvert;
            Current_Action = idx;
        }

    }
}
let Pause = false;
function PlotPID() {

    if (!Pause) {
        TabIdx = (TabIdx + 1) % 150;
        Ecart[TabIdx] = LastEcart;
        Prop[TabIdx] = LastProp;
        Integ[TabIdx] = LastInteg;
        Deriv[TabIdx] = LastDeriv;
        Retard[TabIdx] = LastRetard;
        Ouvert[TabIdx] = LastOuvert;



        let cW = "var(--act-PID-W)";
        let cOuvre = "var(--act-PID-Ouvre)";
        let cRetard = "var(--act-PID-Retard)";
        let cT = "var(--act-PID-T)";
        let cP = "var(--act-PID-P)";
        let cI = "var(--act-PID-I)";
        let cD = "var(--act-PID-D)";


        let style = 'background:linear-gradient(#111,#222,#000);border-color:#333;';
        let S = "<svg viewbox='0 0 970 240' style='" + style + "' width='970'  >"; //   
        S += "<line x1='50' y1='20' x2='50' y2='220' style='stroke:" + cW + ";stroke-width:2' />";
        S += "<line x1='800' y1='20' x2='800' y2='220' style='stroke:" + cOuvre + ";stroke-width:2' />";
        S += "<line x1='50' y1='220' x2='800' y2='220' style='stroke:" + cT + ";stroke-width:2' />";
        for (let t = 0; t < 75; t = t + 5) {
            let x = 800 - t * 10;
            S += "<text x='" + x + "' y='237' style='font-size:16px;fill:" + cT + ";'>" + t + "</text>";
            S += "<line x1='" + x + "' y1='220' x2='" + x + "' y2='224' style='stroke:" + cT + ";stroke-width:2' />";
        }
        S += "<text x='790' y='218' style='font-size:12px;fill:" + cT + ";'>s</text>";
        S += "<text x='804' y='220' style='font-size:12px;fill:" + cOuvre + ";'>0%</text>";
        S += "<text x='804' y='20' style='font-size:12px;fill:" + cOuvre + ";'>100%</text>";
        S += "<line x1='50' y1='20' x2='800' y2='20' style='stroke:" + cOuvre + ";stroke-width:2' stroke-dasharray='1 4' />";
        S += "<line x1='50' y1='120' x2='800' y2='120' style='stroke:" + cW + ";stroke-width:2' stroke-dasharray='1 4' />";
        S += "<text x='20' y='120' style='font-size:12px;fill:" + cW + ";'>0 W</text>";
        S += "<text x='5' y='20' style='font-size:12px;fill:" + cW + ";'>100 W</text>";
        S += "<text x='1' y='220' style='font-size:12px;fill:" + cW + ";'>-100 W</text>";


        S += "<text x='830' y='40' style='font-size:16px;fill:" + cW + ";'>Ecart..........." + Ecart[TabIdx] + " W</text>";
        S += "<text x='830' y='60' style='font-size:16px;fill:" + cOuvre + ";'>Ouverture....." + Ouvert[TabIdx] + " %</text>";
        S += "<text x='860' y='76' style='font-size:12px;fill:" + cT + ";'>= 100 - Retard</text>";
        S += "<text x='830' y='96' style='font-size:16px;fill:" + cRetard + ";'>Retard........." + Retard[TabIdx] + " %</text>";
        S += "<text x='830' y='168' style='font-size:16px;fill:" + cI + ";'>Intégral....." + Integ[TabIdx] + "</text>";

        if (C.ModePara == 1 && bloc[Current_Bl].Para.PID_On) {
            S += "<text x='860' y='112' style='font-size:12px;fill:" + cT + ";'>=</text>";
            S += "<text x='830' y='132' style='font-size:16px;fill:" + cP + ";'>Proportion.." + Prop[TabIdx] + "</text>";
            S += "<text x='860' y='148' style='font-size:12px;fill:" + cT + ";'>+</text>";
            S += "<text x='860' y='184' style='font-size:12px;fill:" + cT + ";'>+</text>";
            S += "<text x='830' y='204' style='font-size:16px;fill:" + cD + ";'>Dérivée........" + Deriv[TabIdx] + "</text>";
        }

        let SE = "<polyline points='";
        let SO = "<polyline points='";
        let SR = "<polyline points='";
        let SP = "<polyline points='";
        let SI = "<polyline points='";
        let SD = "<polyline points='";
        for (let i = 0; i < 150; i++) { //toutes les 0.5s
            let j = (TabIdx - i + 150) % 150;
            let x = 800 - i * 5;
            let y = 120 - Ecart[j];
            y = Math.min(y, 220); y = Math.max(y, 20);
            SE += x + "," + y + " ";
            y = 220 - 2 * Ouvert[j];
            SO += x + "," + y + " ";
            y = 220 - 2 * Retard[j];
            SR += x + "," + y + " ";
            y = 120 - Prop[j];
            y = Math.min(y, 220); y = Math.max(y, 20);
            SP += x + "," + y + " ";
            y = 220 - 2 * Integ[j];
            SI += x + "," + y + " ";
            y = 120 - Deriv[j];
            y = Math.min(y, 220); y = Math.max(y, 20);
            SD += x + "," + y + " ";
        }
        SE += "' style='fill:none;stroke:" + cW + ";stroke-width:2' />";
        SO += "' style='fill:none;stroke:" + cOuvre + ";stroke-width:2' />";
        SR += "' style='fill:none;stroke:" + cRetard + ";stroke-width:2' />";
        SP += "' style='fill:none;stroke:" + cP + ";stroke-width:2' />";
        SI += "' style='fill:none;stroke:" + cI + ";stroke-width:2' />";
        SD += "' style='fill:none;stroke:" + cD + ";stroke-width:2' />";
        if (C.ModePara == 1 && bloc[Current_Bl].Para.PID_On) {
            S += SD + SP;
        }
        S += SI + SR + SO + SE + "</svg>";
        S += "<div class='pauseSVG' onclick='Pause = !Pause;'>⏯</div>";
        paraCourbesPID.innerHTML = S;

    }

    if (Current_Bl !== -1) {
        setTimeout(() => {
            PlotPID();
        }, 500);
    }

}
let decalageX_PID = 0;
let decalageY_PID = 0;
let deplacement_PID = false;
paraCourbesPID.addEventListener("pointerdown", function (e) {
    deplacement_PID = true;
    decalageX_PID = e.clientX - paraCourbesPID.getBoundingClientRect().left;
    decalageY_PID = e.clientY - paraCourbesPID.getBoundingClientRect().top;
    e.preventDefault();
});

document.addEventListener("pointermove", function (e) {
    if (!deplacement_PID) return;

    paraCourbesPID.style.left = (e.clientX - decalageX_PID + window.scrollX) + "px";
    paraCourbesPID.style.top = (e.clientY - decalageY_PID + window.scrollY) + "px";
});

document.addEventListener("pointerup", function () {
    deplacement_PID = false;
});

function Update_PID() { //Envoi des 3 coeff durant les réglages. Ne sont pas enregistrés définitivement 
    let bl = Current_Bl;
    if (bl === -1) return;
    const dataPID = {
        numAction: Current_Action,
        P: bloc[bl].Para.P,
        I: bloc[bl].Para.I,
        D: bloc[bl].Para.D,
        PID_On: bloc[bl].Para.PID_On
    };


    const jsonData = JSON.stringify(dataPID);
    let adr = adrHttpRemote + "/ajaxNewPID";

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

        })
        .catch(error => {
            console.error("Erreur d'envoi :", error);
        });


}
