// Fichier généré automatiquement. Ne pas modifier.
#pragma once
#include <Arduino.h>

static const char pageHtmlActions[] PROGMEM = R"WEB0(<!doctype html>
<html>

<head>
  <meta charset="UTF-8">
  <script src="/JS_Commun.js"></script>
  <link rel="stylesheet" href="/CommunCSS.css">

  <style>
    :root {
      

      /* Couleurs Actions */
      --act-Bact: 8px inset azure;
      --act-svg1: #0b0b0c;
      --act-svg2: #20272c;
      --act-svg3: #0a0a0c;
      --act-svgborder: inset 4px rgb(164, 165, 165);
      --act-svgRadius: 10px;
      --act-boxborder: 1px solid #ccc;
      --act-boxborRad: 6px;
      --act-boxBackGrd: rgb(27, 27, 80);
      --act-infobackground: rgb(129, 128, 128);
      --act-infoborder: 4px inset rgb(117, 117, 117);
      --act-inforadius: 10px;
      --act-infoshadow: 0 2px 10px rgba(0, 0, 0, 0.3);
      --act-infocolor:black;
      --act-lignenum:rgb(65, 138, 247);
      --act-lignebin:rgb(208, 224, 136);
      --act-svgtxt:black;
      --act-svgtxtOut:white;
      --act-coulDef:#2dadc4;
      --act-coulBin:rgb(71, 199, 71);
      --act-coulAct:#cca;
      --act-coulNum:rgb(190, 192, 43);
      --act-coulLig:#eee;
      --act-CoulLab:#3fe6d8;
      --act-CoulNum:rgb(203, 231, 75);
      --act-CoulAct:rgb(226, 148, 46);
      --act-CoulBor:grey;
      --act-CoulTxt:black;
      --act-CoulAcT:#ddb;
      --act-CoulNon:#dfd;
      --act-CoulLin:rgb(88, 207, 88);
      --act-CoulStr:grey;
      --act-CoulBrd:black;
      --act-coulSel:white;
      --act-coulSeb:#112;
      --act-bordRad:3px;
      --act-modeRad:3px;
      --act-PID-W: #f00;
      --act-PID-Ouvre: #0f0;
      --act-PID-Retard: rgb(72, 117, 241);
      --act-PID-T: #288;
      --act-PID-P: rgb(67, 192, 201);
      --act-PID-I: rgb(93, 127, 161);
      --act-PID-D: rgb(41, 126, 86);
    }
    body {width:100%;max-width:none;}
    .Bactions {
      border: 8px inset azure;
    }

    h4 {
      padding: 2px;
      margin: 0px;
    }

    h5 {
      text-align: left;
      padding: 2px;
      margin: 0px;
    }

    .act_cadre {
      width: 100%;
      max-width: 1200px;
      margin: auto;
    }

    .box {
      margin: 10px;
      padding: 10px;
      border: var(--act-boxborder);
      border-radius: var(--act-boxborRad);
      background: var(--act-boxBackGrd);
    }

    .flex-container {
      display: flex;
      justify-content: space-around;
    }

    svg {
      border: var(--act-svgborder);
      border-radius: var(--act-svgRadius);
      margin: 10px;
      touch-action: none;
      background:linear-gradient(to bottom, var(--act-svg1), var(--act-svg2), var(--act-svg3));

    }

    #info {
      position: absolute;
      display: none;
      min-width: 180px;
      padding: 10px;
      background: var(--act-infobackground);
      border:  var(--act-infoborder);
      border-radius: var(--act-inforadius);
      box-shadow: var(--act-infoshadow);
      color: var(--act-infocolor);
      pointer-events: auto;
      text-wrap: nowrap;
    }
     .pauseSVG {
            position: absolute; bottom: 18px; right: 70px;
            cursor: pointer; border: 2px outset grey;
            background-color: white; width: 25px; height: 30px;
            text-align: center;
            color: black;
        }


    .poubelle {
      cursor: pointer;
      font-size: 30px;
    }

    .paraB,
    #paraGPIO,
    #paraVolt,
    #paraTempo,
    #paraMax,
    #paraForce,
    #paraSeuilPW_On,
    #paraSeuilPW_Off,
    #paraPID,
    #paraP,
    #paraI,
    #paraD,
    #paraHost,
    #paraPort,
    #paraGetPost,
    #paraOrdreOn,
    #paraOrdreOff,
    #paraRepetition {
      display: table-row;

    }

    .tcL {
      display: table-cell;
      text-align: left;
      margin: 2px;

    }

    .tcR {
      display: table-cell;
      text-align: right;
      margin: 2px;
    }

    .tcR input,
    .tcR select {
      width: 100px;
      text-align: right;
      margin: 2px;
    }

    select,
    input {
      color: var(--act-coulSel);
      background: var(--act-coulSeb);
    }
    #groupePuissance,#groupeTemperature {
      display: none;
    }
    .act_Pwm {
      display:flex; justify-content:space-around;
      font-size:20px;  padding:3px;
    }
    #paraCourbesPID{
      position:absolute;
      top:200px;
       left:0px;
      display: block;
    }
    
    #CACSI, #Freq_PWM {
      width: 100%; position: relative; margin: 4px; margin-bottom: 10px;
      padding: 2px; border: 2px inset grey;
      background-color:rgba(30,30,30,0.8);
      color: white; border-radius: 10px;
      display:none;
    }
    #commun { display: none; }
  </style>
  <link rel="stylesheet" href="/ThemeCSS.css"> 
  
  <title>Actions</title>
  
</head>

<body onLoad="Init();" onmouseup="mouseClick=false;">
  <div class="act_cadre">
    <div id="lesOnglets"></div>
    <h4>Logigramme des Actions </h4>
  </div>
  <div id="message"></div><br>


 <div class="flex-container">
    <div class="box">
      <label for="Capteur">Capteur : </label>
      <select id="Capteur" onchange="CreationBloc(this)">
        <option value=""></option>
        <optgroup label="Puissance" id="groupePuissance">
          <option value="puissance1" data-type="puissance">⚡Puissance 1</option>
          <option value="puissance2" data-type="puissance">⚡Puissance 2</option>
        </optgroup>
        <optgroup label="Heure">
          <option value="heure" data-type="heure">🕒Heure</option>
        </optgroup>
        <optgroup label="Ensoleillement">
          <option value="actuel" data-type="soleil">🌤️Actuel</option>
          <option value="jourJ" data-type="soleil">🌤️Jour J</option>
          <option value="demain" data-type="soleil">🌤️Jour J+1</option>
        </optgroup>
        <optgroup label="Température" id="groupeTemperature">
          <option value="temperature1" data-type="temperature">🌡️Temperature 1</option>
          <option value="temperature2" data-type="temperature">🌡️Temperature 2</option>
        </optgroup>
        <optgroup label="Tempo">
          <option value="tempo_bleu" data-type="tempo">🔵Bleu</option>
          <option value="tempo_blanc" data-type="tempo">⚪Blanc</option>
          <option value="tempo_rouge" data-type="tempo">🔴Rouge</option>
        </optgroup>
        <optgroup label="HP / HC">
          <option value="HP" data-type="hphc">☀️HP</option>
          <option value="HC" data-type="hphc">🌙HC</option>
        </optgroup>
        <optgroup label="Actions Routeurs Externes" id="groupeActionsExternes" style="display:none;">
          
        </optgroup>
        <optgroup label="Commandes Externes">
          <option value="gpio" data-type="commande">📥Entrée GPIO</option>
          <option value="mqttbin" data-type="commande">📥Entrée MQTT bin</option>
          <option value="mqttnum" data-type="commande">📥Entrée MQTT num.</option>
        </optgroup>
        <optgroup label="Jour">
          <option value="lundi" data-type="jour">📅Lundi</option>
          <option value="mardi" data-type="jour">📅Mardi</option>
          <option value="mercredi" data-type="jour">📅Mercredi</option>
          <option value="jeudi" data-type="jour">📅Jeudi</option>
          <option value="vendredi" data-type="jour">📅Vendredi</option>
          <option value="samedi" data-type="jour">📅Samedi</option>
          <option value="dimanche" data-type="jour">📅Dimanche</option>
        </optgroup>
      </select>
    </div>

    <div class="box">
      <label for="Filtre">Filtre : </label>
      <select id="Filtre" onchange="CreationBloc(this)">
        <option value=""></option>
        <optgroup label="Comparateurs">
          <option value="sup" data-type="filtre">&gt;</option>
          <option value="sup_egal" data-type="filtre">≥</option>
          <option value="inf" data-type="filtre">&lt;</option>
          <option value="inf_egal" data-type="filtre">≤</option>
        </optgroup>
        <optgroup label="Filtres">
          <option value="retard" data-type="filtre">⏳Retard</option>
          <option value="moyenne" data-type="filtre">📈Moyenne</option>
          <option value="timer" data-type="filtre">⏱️Timer</option>
        </optgroup>
        <optgroup label="Additionneurs">
          <option value="addition" data-type="addsub">➕Addition</option>
          <option value="soustraction" data-type="addsub">➖Soustraction</option>
        </optgroup>
      </select>
    </div>

    <div class="box">
      <label for="Logique">Logique : </label>
      <select id="Logique" onchange="CreationBloc(this)">
        <option value=""></option>
        <option value="et" data-type="bin">ET</option>
        <option value="ou" data-type="bin">OU</option>
        <option value="ou_ex" data-type="bin">OU exclusif</option>
        <option value="non" data-type="bin">NON</option>
        <option value="bascule" data-type="bin">Bascule Flip-Flop</option>
      </select>
    </div>
    <div class="box">
      <label for="Actionneur">Actionneur : </label>
      <select id="Actionneur" onchange="CreationBloc(this)">
        <option value=""></option>
        <option value="triac" data-type="action">Triac</option>
        <option value="ssr" data-type="action">SSR</option>
      </select>
    </div>
  </div>
  <div id="zone">
    <svg id="svg-canvas" viewBox="0 0 500 150">

    </svg>
    


  </div>
  <div id="commun" class="act_cadre">
            <br>
            <h4>Paramètres communs aux Actions</h4>

            <div id="CACSI">
                <div>
                    Réactivité (I) si CACSI
                    <span class="fsize10">
                        Augmentée pour les valeurs de puissance inférieures au seuil Pw.  
                        Pour tous les SSR ou Triac.
                    </span>
                </div>
                    <div class="act_Pwm">
                        <div><input type="radio" name="ReacCACSI" id="CACSI1" value="1" checked>Pas d'augmentation</div>
                        <div><input type="radio" name="ReacCACSI" id="CACSI2" value="2">Réactivité x 2</div>
                        <div><input type="radio" name="ReacCACSI" id="CACSI4" value="4">Réactivité x 4</div>
                        <div><input type="radio" name="ReacCACSI" id="CACSI8" value="8">Réactivité x 8</div>
                    </div>                
            </div>

            <div id="Freq_PWM">
                <div>
                    Fréquence signaux PWM 
                    <span class="fsize10">Nécessite un Restart de l'ESP32</span>
                </div>
                    <div class="act_Pwm">
                        <div><input type="radio" name="Fpwm" id="Fpwm5" value="5">5 Hz</div> 
                        <div><input type="radio" name="Fpwm" id="Fpwm50" value="50">50 Hz</div>                              
                        <div><input type="radio" name="Fpwm" id="Fpwm500" value="500" checked>500 Hz</div>                     
                        <div><input type="radio" name="Fpwm" id="Fpwm5000" value="5000">5000 Hz</div>                                        
                        <div><input type="radio" name="Fpwm" id="Fpwm50000" value="50000">50000 Hz</div>                       
                    </div>
            </div>
        </div>
  
  <div class="act_cadre">
    <div id="BoutonsBas">
      <input class="bouton" type="button" onclick="SendValues();" value="Sauvegarder">
      <div class="lds-dual-ring" id="attente"></div>
      <input class="bouton" type="button" onclick="Reset();" value="ESP32 Reset">
    </div>
  </div>
  
  <div id="pied"></div>
  <div id="info"></div>
  <div id="paraCourbesPID"></div>
 
  <script src="/JS_Actions/JS_Action.js"></script>
  <script src="/JS_Actions/JS_boolean.js"></script>
  <script src="/JS_Actions/JS_paraBloc.js"></script>
 
</body>

</html>)WEB0";

static const char pageHtmlBrute[] PROGMEM = R"WEB0(<!doctype html>
<html>

<head>
  <meta charset="UTF-8">
  <script src="/JS_Commun.js"></script>
  <link rel="stylesheet" href="/CommunCSS.css">
  <style>
    :root {
      --brute-axes: white;
      --brute-V: rgb(241, 9, 9);
      --brute-I: rgb(9, 241, 113);
    }

    .ri {
      text-align: right;
    }

    .phi {
      background-color: #ffd;
    }


    .VA {
      background-color: cyan;
    }

    .Hz,
    .Enph {
      background-color: #eeb;
      ;
    }

    .titre {
      color: white;
      text-align: center;
      font-weight: bold;
      text-shadow: 0 1px 3px rgba(0, 0, 0, 0.7);
    }

    .dataIn {
      text-align: left;
      overflow: hidden;
    }

    td {
      text-align: left;
      padding: 4px;
    }



    .Bbrut {
      border: 8px inset azure;
    }

    .dispT {
      display: none;
    }

    .ce {
      text-align: center;
      position: relative;
    }

    svg {
      border: 10px inset azure;
    }

    #infoUxIx2,
    #infoUxIx3,
    #infoUxI,
    #infoNotDef,
    #infoLinky,
    #infoEnphase,
    #infoSmartG,
    #infoHomeW,
    #infoShellyEm,
    #infoPmqtt {
      display: none;
    }


    #DataESP32 a {
      color: #300;
      text-decoration: none;
    }

    .wb100 {
      width: 100%;
      word-wrap: break-word;
      text-align: left;
    }

    .wm550 {
      min-width: 550px;
    }

    .gris {
      background-color: rgba(128, 128, 128, 0.3);
    }

    .ligne0 {
      background-color: rgba(128, 128, 128, 0.3);
    }

    .ligne1 {
      background-color: rgba(128, 128, 128, 0.1);
    }

    .brut-graphSVG {
      border: 1px solid grey;
      border-radius: 4px;
      margin: 0px;
      touch-action: none;
      background: linear-gradient(to bottom, #222222 0%, #444444 50%, #222222 100%);
    }

    .json-viewer {
      font-family: monospace;
      white-space: pre;
      background-color: #f0f0f0;
      padding: 8px;
      border: 1px solid #ccc;
      border-radius: 4px;
    }
  </style>
  <link rel="stylesheet" href="/ThemeCSS.css">
  <title>Brute</title>
</head>

<body onload="LoadConfig();">

  <div id="lesOnglets"></div>
  <div id="date">Date</div><br><br>




  <div class="titre">Donn&eacute;es ESP32</div>
  <div>
    <div id="DataESP32" class="tableau w100"></div>
  </div><br>
  <div class="titre">Evènements</div>
  <div>
    <div id="BruteMessages" class="tableau w100"></div>
  </div><br>
  <div class="titre">Données Brutes des Sources</div>
  <div id="BruteSources"></div>
  <div id="pied"></div>

  <!-- <script src="/ParaRouteurJS"></script> -->
  <script src="JS_Brute.js"></script>



  <br>
</body>

</html>)WEB0";

static const char pageHtmlWifi[] PROGMEM = R"WEB0(<!--  
// **********************************************************
//  Page de connexion 'Acces Point' pour définir réseau WIFI
// **********************************************************
-->
<!doctype html>
<html>

<head>
  <meta charset="UTF-8">
  <link rel="stylesheet" href="/CommunCSS.css">
  <script src="/JS_Commun.js"></script>
  <style>
    #form-passe {
      display: none;
      padding: 10px;
      text-align: center;
      margin: auto;
      width: 100%;
    }

    label,
    input,
    .dB {
      display: table-cell;
      padding-left: 10px;
      padding-right: 5px;
      text-align: left;
      font-size: 120%;
    }

    .l0 {
      display: table-row;
      margin: auto;
      background-color: #333;
      padding: 2px;
    }

    .l1 {
      display: table-row;
      margin: auto;
      background-color: #666;
      padding: 2px;
    }

    #ListeWifi {
      display: inline-block;
      margin: auto;
    }

    .tableWifi {
      display: table;
      margin: auto;
    }

    #envoyer {
      padding-top: 20px;
      display: none;
    }

    #attente2,
    #lesOnglets,
    #pied {
      display: none;
    }

    .Bparametres {
      border: inset 10px azure;
    }

    .Bwifi {
      border: inset 4px azure;
    }
  </style>
  <title>Access Point F1ATB</title>
  <link rel="stylesheet" href="/ThemeCSS.css">
</head>

<body onload="init();">
  <div id='lesOnglets'></div>
  <h1 id="h1T">Routeur Solaire - RMS</h1>
  <h2>Connexion au r&eacute;seau WIFI local</h2>
  <div id="ListeWifi"></div><br><br>
  <div id="scanReseau">
    <input class='bouton' type='button' onclick="ScanWIFI();" value='Scan réseaux WIFI'>
  </div>
  <br>
  <div id='form-passe'>
    <div>Entrez le mot de passe du r&eacute;seau : <span id='nom_reseau'></span></div>
    <input type='password' name='passe' id='passe'>
  </div>
  <div id="envoyer">
    <input class='bouton' type='button' onclick="Envoyer();" value='Envoyer'>
  </div>
  <br>
  <div id="attente2">Attendez l'adresse IP attribuée à l'ESP 32</div>
  <br>
  <div id="pied"></div>

  <script>

    function ScanWIFI() {
      GH("ListeWifi", "Patientez 2s");
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          var LesWifi = this.responseText;
          var Wifi = LesWifi.split(GS);
          var S = "Sélectionnez un réseau Wifi<br><span class='fsize12'>Une nouvelle adresse IP sera probablement attribuée par la box gérant le réseau</span>";
          S += "<div class='tableWifi'>";
          S += "<div class='l1' style='font-weight: bold;'><div class='dB'>SSID</div><div class='dB'>Niveau</div><div class='dB'> Adresse MAC</div><div class='dB'> Canal</div><div class='dB'> </div></div>";
          for (var i = 0; i < Wifi.length - 1; i++) {
            var wifi = Wifi[i].split(RS);
            var j = i % 2;
            S += "<div class='l" + j + "'>";
            S += "<label for='W" + i + "'>" + wifi[0] + " </label>";
            S += "<div class='dB'>" + wifi[1] + " dBm </div><div class='dB'> " + wifi[2] + "</div><div class='dB'> " + wifi[3] + "</div> <input type='radio' name='Wifi' value='" + wifi[0] + "' onclick='ChoixWifi(this.value);'>";
            S += "</div>";
          }
          S += "</div>";
          S += "<div><span class='fsize12'>Ne sont visibles que les réseaux scannés à la mise sous tension de l'ESP32 ou après un reset</span></div>";
          S += "<div><span class='fsize12'>Attention, l'adresse IP attribuée à l'ESP32 peut changer</span></div>";
          GH("ListeWifi", S);
        }
      };
      let adr = adrHttpRemote + '/AP_ScanWifi';
      xhttp.open('GET', adr, true);
      xhttp.send();
      GID("form-passe").style.display = "none";
      GID("envoyer").style.display = "none";
      GID("attente2").style.display = "none";
    }
    function Envoyer() {
     GID("scanReseau").style.display = "none";

      const NewWifi = {
        ssid: ssid,
        password: GID("passe").value
      };


      const jsonData = JSON.stringify(NewWifi);
      let adr = adrHttpRemote + "/AP_SetWifi";

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
          const T = JSON.parse(resultat);
          let Txt = T.new_Wifi.split("||");
          GH("ListeWifi", Txt[1]);
          GID("attente2").style.display = "none";
          GID("form-passe").style.display = "none";
          GID("envoyer").style.display = "none";
          if (Txt[0] == "Ok") {
            GID("scanReseau").style.display = "none";
          }

        })
        .catch(error => {
          console.error("Erreur d'envoi :", error);
        });



      GID("form-passe").style.display = "none";
      GID("envoyer").style.display = "none";
      if (window.location.href.indexOf("192.168.4.1") == -1) {
        GH("attente2", "Adresse IP reconduite ou pas suivant box internet");
      }
      GID("attente2").style.display = "block";
    }
    ssid = "";
    function ChoixWifi(V) {
      ssid = V;
      GH("nom_reseau", ssid);
      GID("envoyer").style.display = "inline-block";
      GID("form-passe").style.display = "inline-block";
    }
    function init() {
      LoadConfig();
    }
    function ImplantationConfig() {
      SetHautBas();


      GID("lesOnglets").style.display = "block";
      GID("onglets2").style.display = "block";
      GID("pied").style.display = "flex";
      GID("h1T").style.display = "none";
    }
    function AdaptationSource() { }
    function FinParaRouteur() {
      GID("Bwifi").style.display = (C.ESP32_Type < 10) ? "inline-block" : "none";
    }
  </script>


</body>

</html>)WEB0";

static const char pageHtmlExport[] PROGMEM = R"WEB0(<!--  
//********************************************************
// Page HTML et Javascript Import / Export configuration *
//********************************************************
-->


  <!doctype html>
  <html><head><meta charset="UTF-8">
  <script src="/JS_Commun.js"></script>
  <link rel="stylesheet" href="/CommunCSS.css">
  <style>
    .Zone{width:100%;border:1px solid grey;border-radius:10px;margin-top:10px;background-color:rgba(30,30,30,0.8);} 

    .boldT{text-align:left;font-weight:bold;padding:10px;}
    .pad10{padding:10px;}
    .form {margin:auto;padding:10px;display: table;text-align:left;width:100%;}
    .ligne {display: table-row;padding:10px;}
    label{display: table-cell;margin: 5px;text-align:left;font-size:20px;height:25px;width:70%;}
    input,.cell{display: table-cell;margin: 5px;text-align:left;font-size:20px;height:25px;}
    input[type=file]{display:block;height:32px;border:none;}
    .liste{display:flex;justify-content:center;text-align:left;} 
    #onglets2{display:block;}
    .Bparametres{border:inset 10px azure;}
    .Bexport{border:inset 4px azure;}
  </style>
  <link rel="stylesheet" href="/ThemeCSS.css">
  </head>
  <body onload="Init();">
    <div id='lesOnglets'></div>
    <h2 >Import / Export des paramètres et données</h2>
    <div class="Zone">
        <div class="boldT">Liste des fichiers <br><span class='fsize12'>(Permet la sauvegarde des paramètres et données sur votre PC)</span></div>
        <div class="form" id="liste_file" ></div>
    </div>
   
    <div class="Zone">
         <div class="boldT">Import de fichiers éditables et modifiables avec un éditeur de texte : </div>
        <div class="fs16 L pad10">- les paramètres du routeur au format .json,<br>
             - les fichiers de données mensuelles au format .csv,<br>
             - les fichiers de styles au format .css
            </div>
        <form method='POST' onsubmit="submit_para(event);" action="#" enctype='multipart/form-data' id='upload_form'>
          <div class="form">
            <div class='ligne'>
              <div class='cell'><input type='file' name='fichier_para_' id="fichier_para_"  class='bouton' accept='.json,.css'></div>
              <input type='submit' value='Mettre à jour'  class='bouton'>
            </div>
            <div class="lds-dual-ring" id="attente"></div>
          </div>
        </form>
        <span class='fsize12'>Après un Import de paramètres, faites un Reset  pour redémarrer avec les nouveaux.</span>
    </div>
    <input  class='bouton' type='button' onclick='Reset();' value='ESP32 Reset' >
    <script>
       
        function Init(){
          LoadConfig();          
        }
        function ImplantationConfig(){
          SetHautBas();
          LoadListeFile();
        }
        function submit_para(event){
          GID("attente").style="visibility: visible;";
          event.preventDefault(); //Pour Firefox
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() { 
            if (this.readyState == 4 && this.status == 200) {
              var retour=this.responseText;
              console.log(retour);              
              GID("attente").style="visibility: hidden;";
              setTimeout(location.reload(),1000);
            }         
          };
          const fileInput = GID("fichier_para_");
          var file = fileInput.files[0];
          var data = new FormData();
          data.append("file", file);
          xhttp.open('POST', adrHttpRemote + '/import', true);
          xhttp.send(data);
        }
        
        
        function LoadListeFile(){
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() { 
            if (this.readyState == 4 && this.status == 200) {
              var files = this.responseText.split(GS);
              var S="";
              for (var i=0;i<files.length-1;i++){
                var F=files[i].split(RS);
                S +=`<div class='ligne'><div class='cell'>${F[0]} </div><div class='cell'>${F[1]} octets </div>`;
                S +=`<div class='cell'><a href='${adrHttpRemote}/export_file?Fichier=${F[0]}&download=${F[0]}' ><button class='bouton'>Télécharger</button></a></div>`;
                S +=`<div class='cell'><button class='bouton' onclick='valideEfface("${F[0]}");'>Effacer</button></div></div>`;
              }
              GH("liste_file", S);
            }         
          };
          let adr =adrHttpRemote + "/ListeFile";
          xhttp.open('GET', adr, true);
          xhttp.send();
        }
        function valideEfface(F){
          var Conf="Confirmez l'effacement";
          if (confirm(Conf)==true){
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() { 
              if (this.readyState == 4 && this.status == 200) {
                location.reload();
              }         
            };
         
            let adr =adrHttpRemote + "/export_file?Fichier=" + F + "&Delete=";
            xhttp.open('GET', adr, true);
            xhttp.send();
          }
        }
        function AdaptationSource(){};
        function FinParaRouteur(){
         
        };
        function Reset(){
          GID("attente").style="visibility: visible;";
            var xhttp = new XMLHttpRequest();
              xhttp.onreadystatechange = function() { 
                if (this.readyState == 4 && this.status == 200) {
                  GID('BoutonsBas').innerHTML=this.responseText;
                  GID("attente").style="visibility: hidden;";
                  setTimeout(location.reload(),1000);
                  setTimeout(location.reload(),10000);
                }         
              };
              xhttp.open('GET', adrHttpRemote + '/restart', true);
              xhttp.send();
        }
    </script>
    <br>
    <div id='pied'></div>
    <br>
    
    
</body></html>
 
)WEB0";

static const char pageHtmlHeure[] PROGMEM = R"WEB0(<!--  
//********************************************************
// Page HTML et Javascript Mise à l'heure horloge interne *
//********************************************************
-->
<!doctype html>
<html>

<head>
  <meta charset="UTF-8">
  <script src="/JS_Commun.js"></script>
  <link rel="stylesheet" href="/CommunCSS.css">
  <style>
    .Zone {
      width: 100%;
      border: 1px solid grey;
      border-radius: 10px;
      margin-top: 10px;
      background-color: rgba(30, 30, 30, 0.8);
    }

    .form {
      margin: auto;
      padding: 10px;
      display: table;
      text-align: left;
      width: 100%;
    }

    .ligne {
      display: table-row;
      padding: 10px;
    }

    label,
    input {
      display: inline-block;
      margin: 5px;
      text-align: right;
      font-size: 20px;
      height: 25px;
    }

    #onglets2 {
      display: block;
    }

    .Bparametres {
      border: inset 10px azure;
    }

    .Bheure {
      border: inset 4px azure;
    }

    .bouton {
      width: auto;
    }

    .boldT {
      text-align: left;
      font-weight: bold;
      padding: 10px;
    }

    .space {
      display: flex;
      justify-content: center;
    }

    select {
      margin: 5px;
      text-align: right;
      font-size: 20px;
      height: 25px;
    }

    .Freset {
      margin: 5px;
      text-align: left;
      font-size: 10px;
      height: 25px;
      padding-top: 15px;
    }

    .spaceAR {
      display: flex;
      justify-content: space-around;
    }
  </style>
  <title>Set hours F1ATB</title>
  <link rel="stylesheet" href="/ThemeCSS.css">
</head>

<body onload="Init();">
  <div id='lesOnglets'></div>
  <h3>Mise à l'heure horloge interne</h3>
<br>
  <h4>Heure actuelle : <span id='date'>--------</span></h4>
  <h4>Nous sommes un : <span id='Jour'>--------</span></h4>

  <div class="Zone">
    <div class="boldT">Horloge du Routeur</div>
    <div class="spaceAR">
      <div><label for='Hor0' id='Hor0L'>Internet</label><input type='radio' name='Horlo' id='Hor0' value="0" checked
          onclick="checkDisabled();"></div>
      <div><label for='Hor1'>Linky</label><input type='radio' name='Horlo' id='Hor1' value="1"
          onclick="checkDisabled();"></div>
     
      
    </div>
  </div>
  <br>
  <div class="Zone" id="ZoneNew_H">
    <div class="form">
      <div class='ligne'>
        <label for='New_J'>Année/Mois/Jour <small>AAAA-MM-JJ</small></label><input type='text' id='New_J' value="">
        <label for='New_H'>Heure hh:mn</label><input type='text' id='New_H' value="">
      </div>
    </div>
  </div>
  <br>
  <div class="Zone" id="ZoneFuseau">
    <div class='spaceAR'>
      <div>
        <label for='Fuseau'>Fuseau horaire : </label>
        <select id='Fuseau'>
          <option value='0' selected>Europe centrale</option>
          <option value='1'>Guadeloupe / Martinique</option>
          <option value='2'>Guyane</option>
          <option value='3'>Réunion</option>
          <option value='4'>Mayotte</option>
          <option value='5'>Nouvelle Calédonie</option>
          <option value='6'>Wallis et Futuna</option>
        </select>

      </div>
      <div>
        <label for='ntpServer'>Serveur NTP (Option)<br><small>ex: mafreebox.free.fr </small></label><input type='text'
          id='ntpServer' value="" title='Laissez vide par defaut'>
      </div>
    </div>
    <div class='Freset'>* Nécessite une sauvegarde puis un Reset pour prise en compte</div>
  </div>

  <div id="BoutonsBas">
    <input class='bouton' type='button' onclick="SendHour();" value='Envoyer'>
    <div class="lds-dual-ring" id="attente"></div>
    <input class='bouton' type='button' onclick='Reset();' value='ESP32 Reset'>
  </div>

  <script>

 // A FAIRE Horloge = 2,3,4 ou 5 ?????????????????
    function Init() {
      LoadConfig();
 
    }
    function ImplantationConfig() {
      SetHautBas();


      GID("Hor" + C.Horloge).checked = true;
      GID("Fuseau").value = C.idxFuseau;
      GID("ntpServer").value = C.ntpServer;

      checkDisabled();
    }
    function AdaptationSource() { };
    
    
    function SendHour() {

      GID("attente").style = "visibility: visible;";
      let xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
          let retour = this.responseText;
          GID("attente").style = "visibility: hidden;";
          location.reload();
        }
      };
      let New_H = GID("New_H").value.trim();
      let New_J = GID("New_J").value.trim();
      C.idxFuseau = GID("Fuseau").value;
      C.Horloge = document.querySelector('input[name="Horlo"]:checked').value;
      C.ntpServer=GID("ntpServer").value.trim();
      if (C.Horloge < 2 || (New_H.indexOf(":") > 0 && New_J.length == 10 && New_J.indexOf("-") > 0)) {
        let adr = adrHttpRemote;
        adr +='/HourUpdate?New_H=' + encodeURIComponent(New_H) + '&New_J=' + New_J + '&idxFuseau=' + C.idxFuseau + '&ntpServer=' + encodeURIComponent(C.ntpServer) + '&Horloge=' + C.Horloge
        xhttp.open('GET', adr, true);
        xhttp.send();
      }
    }

    function checkDisabled() {
      if (C.ModeReseau > 0 && C.Horloge == 0) { GID("Hor2").checked = true; C.Horloge = 2; }
      let Horloge = document.querySelector('input[name="Horlo"]:checked').value;
      GID("ZoneNew_H").style.display = (Horloge < 2) ? "none" : "block";
      GID("ZoneFuseau").style.display = (Horloge == 0) ? "block" : "none";
    }
    function Reset() {
      GID("attente").style = "visibility: visible;";
      let xhttp = new XMLHttpRequest();
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
      GH("date", msg.HeureCourante);

      if (msg.HeureCourante.indexOf("T") > 0) {
        const Ladate = msg.HeureCourante.substring(0, msg.HeureCourante.indexOf("T"));
        const dateSpecifique = new Date(Ladate);
        const NomJ = ["Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"];
        GID('Jour').innerHTML = NomJ[dateSpecifique.getDay()];
      }
      msg.HeureCourante = msg.HeureCourante.replace("T", " ");
      GID('date').innerHTML = msg.HeureCourante;

    };
  </script>
  <br>
  <div id='pied'></div>
  <br>


</body>

</html>)WEB0";

static const char pageHtmlHome[] PROGMEM = R"WEB0(<!DOCTYPE html>
<html lang="fr">

<head>
    <meta charset="UTF-8">
   
    <title>Routeur Photovoltaïque S3 - F1ATB</title>
    <link rel="stylesheet" href="/CommunCSS.css">
    <link rel="stylesheet" href="/HomeCSS.css">
    <link rel="stylesheet" href="/ThemeCSS.css">
    <script src="/JS_Commun.js"></script>
    <script src="/JS_Home/JS_carteTableauPuissance.js"></script>
    <script src="/JS_Home/JS_carteTableauOuvertures.js"></script>
    <script src="/JS_Home/JS_carteGraphEnsoleillement.js"></script>
    <script src="/JS_Home/JS_carteGraphHistorique.js"></script>
    <script src="/JS_Home/JS_carteGraphEnergie.js"></script>
    <script src="/JS_Home/JS_carteJauge.js"></script>
</head>

<body onload="init();">
    <div id="lesOnglets"></div>
    <div id="date">DATE</div>
    <div id="hom-zoneBoutonEdit"><span id="annuler" class="showInline edit" onclick="setMode('annuler');">❌</span>
        <span id="valider" class="showInline edit" onclick="setMode('valider');">✅</span>
        <span id="editer" onclick="setMode('editer');">
            <svg width="18" height="18" viewBox="0 0 24 24"
                fill="none" stroke="currentColor"
                stroke-width="2" stroke-linecap="round"
                stroke-linejoin="round"
                style="vertical-align:middle; cursor:pointer">
                <path d="M12 20h9"/>
                <path d="M16.5 3.5a2.12 2.12 0 0 1 3 3L8 18l-4 1 1-4Z"/>
            </svg>
        </span>
    </div>
    <div id="hom-zoneEditPage" class="hom-boxEdit edit" >
        <div>
            <span>
                <label for="carte">Choisir une carte :</label>
                <select name="carte" id="carte">
                    <option value="tableauPuissance">Tableau puissances</option>
                    <option value="tableauOuvertures">Tableau ouvertures</option>
                    <option value="graphHistorique">Graphique historique</option>
                    <option value="graphEnergie">Graphique énergie</option>
                    <option value="graphJauge">Graphique jauge</option>
                    <option value="graphEnsoleillement">Graphique ensoleillement</option>
                    
                </select>
            </span>
            <span>
                <input type="button" value="Ajouter une carte" onclick="addCarte();">
            </span>
        </div>
        <div>
            <label for="nbColumn">Nombre de colonnes dans la page :</label>
            <select name="nbColumn" id="nbColumn" onchange="setNbColonnes(this.value);">
                <option value="1">1</option>
                <option value="2">2</option>
                <option value="3">3</option>
                <option value="4">4</option>
            </select>
        </div>

    </div>
    <div id="hom-zonesContainer" ></div>
    <div id="infoCurseur"></div>
    <br>
    <div id="pied" ></div>
    
    <script src="/JS_Home/JS_Home.js"></script>
    
</body>

</html>)WEB0";

static const char pageHtmlOTA[] PROGMEM = R"WEB0(<!--  
//***************************************************
// Page HTML et Javascript mise à jour du code par OTA
//***************************************************
-->
<!doctype html>
<html>

<head>
  <meta charset="UTF-8">
  <script src="/JS_Commun.js"></script>
  <link rel="stylesheet" href="/CommunCSS.css">
  <style>
    input {
      font-size: 20px;
    }
    .Zone {
      width: 100%;
      border: 1px solid grey;
      border-radius: 10px;
      margin-top: 10px;
      background-color: rgba(30, 30, 30, 0.8);
    }
    .liste {
      text-align: center;
      margin:2px;
    }
    ul{
      margin: 0;
      padding: 10px;
      list-style-position: inside;
      text-align: left;
    }
    li {
      margin: 5px 0;
    }


    #onglets2 {
      display: block;
    }

    .Bparametres {
      border: inset 10px azure;
    }

    .Bota {
      border: inset 4px azure;
    }

    
  </style>
  <link rel="stylesheet" href="/ThemeCSS.css">
  <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>

</head>

<body onload="Init();">
  <div id='lesOnglets'></div>
  <h2>Web OTA</h2>
  <br>
  <h4>Mise à jour du firmware par Wifi ou Ethernet</h4>
  <div class="liste">
    Votre version actuelle du routeur : <span id="Version_actu"></span>
  </div>
  <br>
  <div class="liste">
    Version(s) disponible(s) :
  </div>
  <div class="liste">
    <iframe src="https://f1atb.fr/web_tool_RMS_S3/scan_dir_bin.php" width=50% height=150px></iframe>
  </div>
  <div class="Zone">
    <div class="liste">
      <ul>
        <li>1 - <a href='/pageHtmlExport.html'>Sauvegardez, par sécurité, vos paramètres</a> sur votre ordinateur.</li>
        <li>2 - Téléchargez sur votre ordinateur, la version binaire du logiciel du routeur souhaitée
          <br>(RMS_S3_F1ATB_Vxx.xx.bin) en cliquant dessus</li>
        <li>3 - Cliquez sur "Choisir un fichier" et sélectionnez ce binaire sur votre ordinateur</li>
        <li>4 - Cliquez sur "Mettre à jour"</li>
        <li>5 - <a href='/pageHtmlExport.html'>Importez si besoin vos anciens paramètres</a> du PC </li>
      </ul>
    </div>
    <form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
      <input type='file' name='update' id="fichier">
      <input class='bouton' type='submit' value='Mettre à jour'>
    </form>
    <div id='prg'>progression: 0%</div>
  </div>
  <script>
    $('form').submit(function (e) {
      e.preventDefault();
      var form = $('#upload_form')[0];
      var data = new FormData(form);

      $.ajax({
        url: '/update',
        type: 'POST',
        data: data,
        contentType: false,
        processData: false,
        xhr: function () {
          var xhr = new window.XMLHttpRequest();
          xhr.upload.addEventListener('progress', function (evt) {
            if (evt.lengthComputable) {
              var per = evt.loaded / evt.total;
              $('#prg').html('progression: ' + Math.round(per * 100) + '%');
            }
          }, false);
          return xhr;
        },
        success: function (d, s) {
          console.log('succès!')

        },
        error: function (a, b, c) {
        }
      });
    });
    function Init() {
      LoadConfig();

    }
    function ImplantationConfig() {
      SetHautBas();
      GH("Version_actu",C.Version);

    }
    
    
  </script>
  <br>
  <small>Après une mise à jour un reset de l'ESP32 et un Ctrl+F5 pour vider le cache du navigateur sont
    recommandés.</small>
  <div id='pied'></div>
  <br>


</body>

</html>)WEB0";

static const char pageHtmlPara[] PROGMEM = R"WEB0(<!doctype html>
<html>

<head>
  <meta charset="UTF-8">
  <script src="/JS_Commun.js"></script>
  <link rel="stylesheet" href="/CommunCSS.css">
  <style>
    .form {
      margin: auto;
      padding: 5px;
      display: table;
      text-align: left;
      width: 100%;
    }

    .form2 {
      margin: 2px;
      padding: 2px;
      display: block;
      width: 100%;
    }

    .ligne {
      display: table-row;
      padding: 10px;
    }

    .ligne2 {
      display: none;
      padding: 10px;
    }

    .ligne3 {
      display: flex;
      justify-content: center;
      padding: 0px;
      margin: 0px;
    }

    .ligneIB33 {
      display: inline-block;
      width: 33%;
    }

    .cadre {
      border-top: 1px solid azure;
    }

    .canalT {
      display: table;
    }

    .canalTr {
      display: table-row;
      width: 100%;
    }

    .canalTc {
      display: table-cell;
      padding: 1px;
      text-align: center;
      width: 100%;
      font-size: 12px;
    }

    label,
    .nomR {
      display: table-cell;
      margin: 5px;
      text-align: left;
      font-size: 20px;
      height: 25px;
      width: 60%;
    }

    input {
      display: table-cell;
      margin: 5px;
      text-align: left;
      font-size: 20px;
      height: 25px;
    }

    select {
      display: table-cell;
      margin: 5px;
      text-align: left;
      font-size: 20px;
      height: 25px;
    }

    .source label {
      display: table-cell;
      margin: 5px;
      text-align: left;
      font-size: 12px;
      height: 21px;
      width: initial;
    }

    .boldT {
      text-align: left;
      font-weight: bold;
      padding: 10px;
    }

    .Bparametres {
      border: inset 10px azure;
    }

    .Bgeneraux {
      border: inset 4px azure;
    }

    #BoutonsBas {
      text-align: center;
    }

    #ligneFixe,
    .ligneTemperature,
    #ligneExt,
    #infoIP,
    #ligneTopicP,
    #ligneTopicT {
      display: none;
    }

    .Zone,
    .generaux {
      width: 100%;
      border: 4px inset grey;;
      border-radius: 10px;
      margin-top: 10px;
      background-color:rgba(30,30,30,0.8);
    }

    #onglets2 {
      display: block;
    }

    .don {
      height: 50px;
      display: block;
      text-align: center;
    }

    #donEnv {
      display: inline-block;
      text-align: center;
      border: azure 3px outset;
      border-radius: 18px;
      background-color: white;
    }

    .dispNone {
      display: none;
    }

    .generaux {
      width: 60%;
      text-align: center;
      margin: auto;
    }

    .generaux label {
      width: 30%;
    }

    .fsize150 {
      font-size: 140%;
    }

    .topG {
      display: flex;
      justify-content: center;
    }

    .shem {
      font-size: 12px;
      display: flex;
    }

    .TC {
      display: table-cell;
    }

    .TCc {
      display: table-cell;
      cursor: pointer;
      text-align:left;
    }

    .titreS {
      display: table-cell;
      margin: 5px;
      text-align: left;
      font-size: 20px;
      height: 25px;
      font-weight: bold;
    }

    .enphW100 {
      width: 100%;
      display: flex;
      justify-content: space-between;
    }
    #Routeur_local {
      background-color: #333;
    }
  </style>
  <link rel="stylesheet" href="/ThemeCSS.css">
  <title>Paramètres généraux</title>
</head>

<body onLoad="Init();">
  <div id="lesOnglets"></div>
  <div class="generaux">
    <div class="fsize150">Paramètres généraux</div>
    <div class="topG">
      <label for="ModeP0" style="text-align:right;">Mode standard</label>
      <input type="radio" name="ModeP" id="ModeP0" value="0" checked onclick="checkDisabled();">
      <label for="ModeP1" style="text-align:right;">Mode expert</label>
      <input type="radio" name="ModeP" id="ModeP1" value="1" onclick="checkDisabled();">
    </div>
  </div>
  <div class="Zone">
    <div class="boldT">Routeur</div>
    <div class="form">
      <div class="ligne">
        <label for="nomRouteur">Nom du routeur : </label>
        <input type="text" name="nomRouteur" id="nomRouteur">
      </div>
      <div class="ligne">
        <label for="Horlo">Horloge du routeur</label>
        <select id="Horlo" onclick="checkDisabled();"
          title="Internet de préférence pour executer les actions planifiées.">
          <option value="0" selected>Internet</option>
          <option value="1">Linky</option>
          <option value="2">Interne</option>
          <option value="3">IT 10ms/100Hz (Triac)</option>
          <option value="4">IT 20ms/50Hz</option>
          <option value="5">ESP externe</option>
        </select>
      </div>
      <div class="ligne" id="l_wifi_0">
        <label for="TempoRTEon">Affichage couleur Tempo RTE : </label>
        <input type="checkbox" name="TempoRTEon" id="TempoRTEon" style="width:25px;"><span class="fsize10">Nécessite un
          Reset de l'ESP32</span>
      </div>
      <div class="ligne" id="l_wifi_1">
        <label for="MQTTRepete">Puissances et températures envoyées au serveur MQTT<br>Période (s) répétition (0= pas
          d'envoi) : </label>
        <input type="number" name="MQTTRepete" id="MQTTRepete" onclick="checkDisabled();">
      </div>
      <div class="ligne" id="l_wifi_2">
        <label for="WifiSleep">Wifi Sleep/veille <small>(réduit la consommation mais ralenti la communication)</small>:
        </label>
        <input type="checkbox" name="WifiSleep" id="WifiSleep" style="width:25px;"><span class="fsize10">Nécessite un
          Reset de l'ESP32</span>
      </div>
      <div class="ligne">
        <label for="RAZdata">Remise à zéro historique des mesures</label>
        <input id="RAZdata" class="bouton" type="button" onclick="SendRAZ();" value="RAZ">
      </div>
    </div>
    <div>Configuration matérielle <span class="fsize10">Nécessite un Reset de l'ESP32</span></div>
    <div class="form">
      <div class="ligne">
        <label for="ESP">Modèle carte ESP32-S3</label>
        <select id="ESP" onclick="checkDisabled();" title="Carte à définir pour connaître les GPIOs disponibles.">
          <option value="0" selected>Non défini</option>
          <option value="1">Wroom seul</option>
          <option value="2">Ecran 480*320 pixels</option>
          <option value="10">T-ETH-Lite</option>
        </select>
      </div>
      <div class="ligne" id="rotation">
        <label for="Rot">Format écran</label>
        <select id="Rot" onclick="checkDisabled();">
          <option value="0" selected>Portrait (0°)</option>
          <option value="1">Paysage (90°)</option>
          <option value="2">Portrait (180°)</option>
          <option value="3">Paysage (270°)</option>
        </select>
      </div>
      <div class="ligne" id="dureeOn">
        <label for="DurEcran">Durée écran On</label>
        <select id="DurEcran" onclick="checkDisabled();">
          <option value="10000" selected>10s</option>
          <option value="30000">30s</option>
          <option value="300000">5mn</option>
          <option value="54000000">15h</option>
          <option value="0">Toujours allumé</option>
        </select>
      </div>
      <div class="ligne">
        <label for="Serial1">Port série 1</label>
        <select id="Serial1" onclick="checkDisabled();" title="Port série 1.">
          <option value="0" selected>Non utilisé</option>
          <option value="1">RX=gpio 18, TX=gpio 17</option>
        </select>
      </div>
      <div class="ligne">
        <label for="Serial2">Port série 2</label>
        <select id="Serial2" onclick="checkDisabled();" title="Port série 2.">
          <option value="0" selected>Non utilisé</option>
          <option value="1">RX=gpio 6, TX=gpio 7</option>
        </select>
      </div>

      <div class="ligne" id="Analog">
        <label for="pUxI">GPIOs Entrées analogiques</label>
        <select id="pUxI" onclick="checkDisabled();">
          <option value="0" selected>Pas d'entrée</option>
          <option value="1">Commun(5), U(6) , I(7)</option>
        </select>
      </div>
      <div class="ligne">
        <label for="pTriac">GPIOs Triac</label>
        <select id="pTriac" onclick="checkDisabled();">
          <option value="0" selected>Pas de triac</option>
          <option value="1">Pulse=gpio 9, Zc=gpio 14</option>
        </select>
      </div>
      <div class="ligne" id="p_Temp">
        <label for="pTemp">Capteur(s) de température</label>
        <select id="pTemp" onclick="checkDisabled();">
          <option value="0" selected>Pas de capteur</option>
          <option value="1">DS18B20 gpio 15</option>
          <option value="2">DS18B20 gpio 21</option>
          <option value="3">DS18B20 gpio 27</option>
          <option value="4">DS18B20 gpio 33</option>
        </select>
      </div>
      <div class="ligne">
        <label for="pLED">Affichage LED / OLED</label>
        <select id="pLED" onclick="checkDisabled();">
          <option value="0" selected>Pas d'affichage</option>
          <option value="1">LEDs gpio 15, gpio 16</option>
          <option value="6">RGB LED gpio 48</option>
          <option value="10">SSD1306/1309 sda/scl=gpio 15/16</option>
          <option value="11">SH1106 OLED | sda/scl=gpio 15/16</option>
        </select>
      </div>

    </div>
  </div>
  <div class="Zone">
    <div class="boldT">Accès réseau et adresse IP de l'ESP32 du Routeur</div>
    <div class="form">
      <div class="ligne">
        <label for="ModeW">Accès</label>
        <select id="ModeW" onclick="checkDisabled();">
          <option value="0" selected>Internet</option>
          <option value="1">Réseau local uniquement</option>
          <option value="2">Point isolé</option>
        </select>
      </div>

      <div class="ligne">
        <label for="dhcp">Adresse IP <span id="localIP"></span></label>
        <input type="checkbox" name="dhcp" id="dhcp" style="width:25px;" onclick="checkDisabled();"><span id="ipreset"
          class="fsize10">Nécessite un Reset de l'ESP32</span>
      </div>
    </div>
    <div class="form" id="infoIP">
      <div class="ligne">
        <label for="adrIP">Adresse IP si fixe (ex : 192.168.1.245) <br><span class="fsize10">N&eacute;cessite un Reset
            de l'ESP32</span></label>
        <input type="text" name="adrIP" id="adrIP">
      </div>
      <div class="ligne">
        <label for="gateway">Passerelle / Gateway (ex : 192.168.1.254)<br><span class="fsize10">En g&eacute;n&eacute;ral
            l'adresse de votre box internet</span></label>
        <input type="text" name="gateway" id="gateway">
      </div>
      <div class="ligne">
        <label for="masque">Masque / Subnet (ex : 255.255.255.0)</label>
        <input type="text" name="masque" id="masque">
      </div>
      <div class="ligne">
        <label for="dns">DNS (ex : 192.168.1.254)<br><span class="fsize10">En g&eacute;n&eacute;ral l'adresse de votre
            box internet</span></label>
        <input type="text" name="dns" id="dns">
      </div>
    </div>
    <div class="form" id="Hostname">
      <div class="ligne">
        <label for="hostname">Hostname<br><span class="fsize10"> Caractères (a-z et 0-9 et -)</span></label>
        <input type="text" name="hostname" id="hostname">
      </div>
    </div>
  </div>

  <div class="Zone" id="SurvCom">
    <div class="boldT">Surveillance Communications du Routeur <span class="fsize10"> Reset si coupure de : </span></div>
    <div class="form">
      <div class="ligne">
        <label for="ComSurv">Reset si coupure Wifi ou Ethernet de</label>
        <select id="ComSurv" onclick="checkDisabled();">
          <option value="6" selected="">3mn</option>
          <option value="60" selected="">30mn</option>
          <option value="360" selected>3h</option>
          <option value="1440">12h</option>
          <option value="2880">24h</option>
        </select>
      </div>
    </div>
  </div>
  <div class="Zone" id="listerouteurs">
    <div class="boldT">Liste des Routeurs en réseau </div>
    <div class="form" id="Routeurs"></div>
  </div>
  <div class="Zone">
    <div class="boldT">Sources des mesures de puissance</div>
    <div class="form" id="liste_sources"> </div>
    <div id="CACSI" class="form">
      <div class="ligne">
        <label for="EstimCACSI">Estimateur injection si CACSI: </label>
        <input type="checkbox" name="EstimCACSI" id="EstimCACSI" style="width:25px;" onclick="checkDisabled();">
      </div>
    </div>
  </div>
  <div class="Zone" id="LesSourcesTemp">
    <div class="boldT">Source des mesures de température</div>
    <div class="form">
      <div id="Sources_Temp"></div>
    </div>
  </div>

  <div class="Zone" id="Zmqtt">
    <div class="boldT">Paramètres serveur MQTT <small>(Home Assistant , Domoticz ...)</small></div>
    <div class="form">
      <div class="ligne">
        <label for="MQTTIP">Adresse IP host MQTT (ex : 192.168.1.18) : </label>
        <input class="w100" type="text" name="MQTTIP" id="MQTTIP"><div class="TCc"></div>
      </div>
      <div class="ligne">
        <label for="MQTTPort"> port (ex : 1883) : </label>
        <input class="w100" type="number" name="MQTTPort" id="MQTTPort"><div class="TCc"></div>
      </div>
      <div class="ligne">
        <label for="MQTTUser">MQTT User nom : </label>
        <input class="w100" type="text" name="MQTTUser" id="MQTTUser"><div class="TCc"></div>
      </div>
      <div class="ligne">
        <label for="MQTTpwd">MQTT mot de passe : </label>
        <input class="w100" type="password" name="MQTTpwd" id="MQTTpwd"><div class="TCc" onclick="visupwd(this.id,'MQTTpwd');" id="iconMqttPwd">👁️</div>
      </div>
    
      <div class="ligne">
        <label for="MQTTPrefix">MQTT Préfixe de découverte (1 seul mot ex : homeassistant ) : </label>
        <input class="w100" type="text" name="MQTTPrefix" id="MQTTPrefix"><div class="TCc"></div>
      </div>
      <div class="ligne">
        <label for="MQTTPrefixEtat">MQTT Préfixe de publication (1 seul mot ex : homeassistant ) : </label>
        <input class="w100" type="text" name="MQTTPrefixEtat" id="MQTTPrefixEtat"><div class="TCc"></div>
      </div>
      <div class="ligne">
        <label for="MQTTdeviceName">MQTT Device Name / Nom routeur (1 seul mot ex : routeur_rms ) : </label>
        <input class="w100" type="text" name="MQTTdeviceName" id="MQTTdeviceName"><div class="TCc"></div>
      </div>
      <div class="ligne">
        <label for="subMQTT">Souscrire au forçage des Actions via le serveur MQTT: </label>
        <input class="w100" type="checkbox" name="subMQTT" id="subMQTT" style="width:25px;" onclick="checkDisabled();"><div class="TCc"></div>
      </div>
    </div>
  </div>
  <div class="Zone" id="Zcalib">
    <div class="boldT" id="Tui">Calibration Mesures Ueff et Ieff <small>(UxI)</small></div>
    <div class="form">
      <div class="ligne" id="CUi">
        <label for="CalibU">Coefficient multiplicateur Ueff (typique : 1000) : </label>
        <input type="number" name="CalibU" id="CalibU">
      </div>
      <div class="ligne" id="CuI">
        <label for="CalibI">Coefficient multiplicateur Ieff (typique : 1000) : </label>
        <input type="number" name="CalibI" id="CalibI">
      </div>
    </div>
  </div>
  <div class="Zone">
    <div class="boldT">Mise en page Web</div>
    <div class="form">
      <div class="ligne">
        <label for="Theme">Thème</label>
        <select id="Theme" ></select>
      </div>
    </div>
  </div>
  <div class="Zone" id="Localisation">
    <div class="boldT">Localisation</div>
    <div class="form">
      <div class="ligne">
        <label for="latitude">Latitude (° décimal) :</label>
        <input type="text" name="latitude" id="latitude" value="48.8566">
      </div>
      <div class="ligne">
        <label for="longitude">Longitude (° décimal) :</label>
        <input type="text" name="longitude" id="longitude" value="2.3522">
      </div>
    </div>
  </div>
  <div class="Zone">
    <div class="boldT">Sécurité d'accès aux paramètres et Actions</div>
    <div class="form">
      <div class="ligne">
        <label for="CleAcces">Définissez un mot de passe d'accès <span class="fsize10">Pas de ";" Ne rien mettre si
            accès libre</span> : </label>
        <input type="text" name="CleAcces" id="CleAcces">
      </div>
    </div>
  </div>
  <div id="BoutonsBas">
    <br><input class="bouton" type="button" onclick="sendConfig();" value="Sauvegarder">
    <div class="lds-dual-ring" id="attente"></div>
    <input class="bouton" type="button" onclick="Reset();" value="ESP32 Reset">
  </div>
  <br>
  <div id="pied"></div>
  <br>
  <div id="donEnv">
    <form action="https://www.paypal.com/donate" method="post" target="_top">
      <input type="hidden" name="hosted_button_id" value="Z35E9D5D9N9DN">
      <input class="don" type="image"
        src="https://pics.paypal.com/00/s/MGY1NzdhY2YtYTRkNi00YzIwLWI2YzQtNWI3YjM3ZmFiNWUx/file.PNG" style="border:0px;"
        name="submit" title="PayPal - The safer, easier way to pay online!" alt="Bouton Faites un don avec PayPal">
      <img alt="" class="dispNone" style="border:0px;" src="https://www.paypal.com/fr_FR/i/scr/pixel.gif" width="1"
        height="1" />
    </form>

  </div>
 
  <script src="/JS_Para.js"></script>

</body>

</html>)WEB0";

static const char pageHtmlCleAcces[] PROGMEM = R"WEB0(//*******************************************************************
//Page HTML pour les paramètres de clé d'accès
//*******************************************************************

  <!doctype html>
  <html><head><meta charset="UTF-8">
 
  <link rel="stylesheet" href="/CommunCSS.css">  
  <style>    
    body{color:white;}
    .form {margin:auto;padding:10px;display: table;text-align:left;width:100%;}
    .ligne {display: table-row;padding:10px;}
    .cadre {border-top:1px solid azure;}
    label,.nomR{display: table-cell;margin: 5px;text-align:left;font-size:20px;height:25px;width:70%;}
    input{display: table-cell;margin: 5px;text-align:left;font-size:20px;height:25px;}
    .boldT{text-align:left;font-weight:bold;padding:10px;}   
    .Bparametres{border:inset 10px azure;}
    .Bgeneraux{border:inset 4px azure;}
    #BoutonsBas{text-align:center;}    
    .Zone{width:100%;border: 1px solid grey;border-radius:10px;margin-top:10px;background-color:rgba(30,30,30,0.3);} 
    #onglets2{display:none;} 
  </style>
  <link rel="stylesheet" href="/ThemeCSS">
  <script>
   
    function SendCle(){
        document.cookie="CleAcces=" + GID("CleAcces").value.trim() ;
        
        GID("attente").style="visibility: visible;";
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() { 
          if (this.readyState == 4 && this.status == 200) {
            var retour=this.responseText;
            location.reload();
          }         
        };
        xhttp.open('GET', 'CleUpdate', true);
        xhttp.send(); 
    }
    function init(){
      LoadConfig();      
      document.cookie="CleAcces=" + GID("CleAcces").value.trim() ;
    }
    function   AdaptationSource(){}  ;
    function FinParaRouteur(){};
     function ImplantationConfig(){
          SetHautBas();
          LoadParaRouteur();
         
      }
  </script>
  
  </head>
  <body onload="init();">
    <div id="lesOnglets"></div>
    <h4>Mot de passe d'accès</h4>
    <div class="Zone" >
        <div class="boldT" >Sécurité d'accès aux paramètres et Actions</div>
        <div class="form"  >    
          <div class='ligne'>
            <label for='CleAcces'>Entrez le mot de passe d'accès : </label>
            <input type='text' name='CleAcces' id='CleAcces'>
          </div>
        </div>
    </div>
    <div  id='BoutonsBas'>        
        <br><input  class='bouton' type='button' onclick="SendCle();" value='Envoyer' >
        <div class="lds-dual-ring" id="attente"></div>
    </div>
    <br>
    <div id='pied'></div>
     
    <script src="/ParaRouteurJS"></script>
    
   </body></html>
)WEB0";

static const char CommunCSS[] PROGMEM = R"WEB0(* {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }
   

    body {font-size:120%;text-align:center;width:100%;max-width:1000px;margin:auto;padding:10px;
      background:linear-gradient(#000033, #77b5fe,#000033);
      background-attachment:fixed;
      color:white;
      font-family: Arial;
    }
    a:link {color:rgb(173, 173, 238);text-decoration: none;}
    a:visited {color:rgb(173, 173, 238);text-decoration: none;}
    .onglets{margin-top:4px;left:0px;font-size:130%;}
    #onglets2{margin-top:10px;left:0px;font-size:80%;display:none;}
    .dispNone{display:none;}
    .Bonglet{margin-left:20px;border:4px outset grey;background:#333;border-radius:6px;padding-left:20px;padding-right:20px;display:inline-block;}
    .Bonglet2{margin-left:20px;border:2px outset grey;background:#333;border-radius:4px;padding-left:20px;padding-right:20px;display:inline-block;}
    #pied{display:flex;justify-content:space-between;font-size:14px;}
    .fsize12{font-size:12px;height:16px;}
    .fsize10{font-size:10px;height:14px;}
    .fsize8{font-size:8px;}
    .tableau { background-color:white;display:inline-block;margin:auto;padding:4px;color:black;border:2px inset grey;border-radius:10px;}
    table{border-collapse:collapse;}
    .lds-dual-ring {color: #cccc5b;visibility: hidden;}
    .lds-dual-ring,.lds-dual-ring:after {box-sizing: border-box;}
    .lds-dual-ring {display: inline-block;width: 80px;height: 80px;}
    .lds-dual-ring:after {content: " ";display: block;width: 64px;height: 64px;margin: 8px;border-radius: 50%;border:
    6.4px solid currentColor;border-color: currentColor transparent currentColor transparent;animation: lds-dual-ring
    1.2s linear infinite;}
    @keyframes lds-dual-ring {0% {transform: rotate(0deg);} 100% {transform: rotate(360deg);}}
    .bouton,input[type=file]::file-selector-button{margin: 5px;text-align:left;font-size:20px;height:28px;border:3px outset grey;border-radius:7px;cursor:pointer;}
    .w100{width:100%;}
    .Wh { background-color:yellow; }
    .W { background-color:red; }
    .VA { background-color:cyan; }
    .deg { background-color:magenta; }
    .temper { background-color:green; }
    .A { background-color:lightblue;}
    .V { background-color:lightyellow;}
    #nom_R,#date{
      margin:10px;
    }
    

     #LED {
      position: absolute;
      top: 4px;
      left: 4px;
      width: 0px;
      height: 0px;
      border: 5px solid red;
      border-radius: 5px;
    }

    .L{text-align:left;}
    .C{text-align:center;}
    .R{text-align:right;})WEB0";

static const char HomeCSS[] PROGMEM = R"WEB0(:root {

            /* Couleurs SVGs Soleil*/
            --hom-solaxes: white;
            --hom-solcourbeJ: yellow;
            --hom-solcourbeJ1: orange;
            --hom-sollabel: white;
            --hom-solactuel: lime;

            /* Couleurs SVGs Historique*/
            --hom-histaxes: white;
            --hom-histlabel: white;




        }

        body {
            user-select: none;
            /* Évite la sélection de texte pendant le glissement */
        }

        #infoCurseur {
            position: absolute;
            top: 10px;
            left: 10px;
            color: white;
            background-color: rgba(0, 0, 0, 0.5);
            padding: 5px 10px;
            border-radius: 5px;
            z-index: 1001;
            font-size: 12px;
            text-align: right;
        }

        /*Pour l'affichage conditionnel des éléments d'édition */
        .edit {
            display: none;
        }

        .showBlock {
            display: block;
        }

        .showInline {
            display: inline;
        }

        /* Conteneur principal en grille CSS de nbColonnes colonnes */
        #hom-zonesContainer {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 15px;
            max-width: 1400px;
            margin: 0 auto;
            min-height: 80vh;
            align-content: start;
        }

        /* Style des cartes / zones <div> */
        .hom-card-title {
            color: #ffffff;
            margin-bottom: 10px;
        }

        .hom-card {
            position: relative;
            border-radius: 10px;
            padding: 10px;
            box-shadow: 0 4px 15px rgba(250, 2500, 250, 0.3);
            background-color: rgba(0, 0, 0, 0.8);
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            cursor: grab;
            touch-action:none;
            /* Prévient le défilement de la page lors du glissement sur mobile */
            transition: transform 0.15s ease, box-shadow 0.15s ease, opacity 0.2s ease;
            color: #ffffff;
            text-shadow: 0 1px 3px rgba(0, 0, 0, 0.7);
        }

        .hom-card:active {
            cursor: grabbing;
        }

        /* Largeurs relatives */
        .span-1 {
            grid-column: span 1;
        }

        .span-2 {
            grid-column: span 2;
        }

        .span-3 {
            grid-column: span 3;
        }

        .span-4 {
            grid-column: span 4;
        }

        .span-1,
        .span-2,
        .span-3,
        .span-4 {
            height: fit-content;
        }

        /* Style pendant le déplacement */
        .hom-card.dragging {
            opacity: 0.4;
            transform: scale(0.98);
        }

        .hom-card.drag-over {
            outline: 3px dashed #ffffff;
            outline-offset: -5px;
        }

        /* Style spécifique au clone volant sur tactile */
        .touch-clone {
            position: fixed;
            pointer-events: none;
            z-index: 1000;
            opacity: 0.85;
            transform: scale(0.9);
            box-shadow: 0 10px 25px rgba(0, 0, 0, 0.5);
            border-radius: 12px;
        }

        .hom-card-contenu {
            min-height: 50px;
            min-width: 50px;
            padding: 0px;
            margin: 0px;
        }

        /* Adaptabilité mobile */
        @media (max-width: 768px) {
            body{
                padding:30px;
            }
            #hom-zonesContainer {
                grid-template-columns: 1fr;
            }

            .span-1,
            .span-2,
            .span-3,
            .span-4 {
                grid-column: span 1;
            }
        }

        #hom-zoneBoutonEdit {
            font-size: 1.5rem;
            cursor: pointer;
            position: absolute;
            top: 10px;
            right: 10px;
        }

        #hom-zoneEditPage {
            display: flex;
            justify-content: space-around;

            padding: 20px;
            margin: auto;
            margin-bottom: 20px;
            max-width: 1400px;
        }

        .hom-boxEdit {
            border-radius: 12px;
            border: 2px dotted rgba(255, 255, 255, 0.3);
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.3);
            background-color: rgba(0, 0, 0, 0.3);
            margin-bottom: 10px;
        }

        .hom-boxZone {
            display: flex;
            justify-content: space-between;
            padding: 5px;
        }

        .hom-modifLargeur {
            display: flex;
            justify-content: center;

            font-size: 80%;

        }

        .hom-modifLargeur>div:nth-child(2),
        .hom-modifLargeur>div:nth-child(4) {
            cursor: pointer;
            user-select: none;
            text-align: center;
        }

        .hom-removeZone {
            cursor: pointer;
            text-align: center;
        }


        .hom-LeTableau {
            display: grid;
            grid-template-columns: auto 80px;
            padding: 10px;
            color: black;
            border: 2px inset grey;
            border-radius: 10px;
            background-color: rgba(30, 30, 30, 0.3);
            white-space: nowrap;
        }

        .hom-LeTableau>div {
            margin-top: 2px;
            margin-bottom: 2px;

        }

        .grid-contColTitre {
            display: grid;
            grid-template-columns: auto;
            grid-gap: 2px;
            padding: 0px;

        }

        .grid-contColTitre>div {
            padding: 10px;
            text-align: left;
        }

        .grid-Titre1 {
            position: relative;
        }

        .grid-contColUnite {
            display: grid;
            grid-template-columns: auto;
            grid-gap: 2px;
            padding: 0px;
            margin-left: 4px;
            max-width: 80px;
        }

        .grid-contColUnite>div {
            padding: 10px;
            text-align: Center;
        }

        .hom-tempo {
            width: 100px;
            top: 0px;
            bottom: 0px;
            right: 0px;
            display: block;
            position: absolute;
            font-size: 10px;
            text-align: center;
            line-height: 90%;
        }

        .grid-contColCanal {
            display: grid;
            grid-gap: 2px;
            padding: 0px;
            margin-left: 4px;
        }

        .grid-contColCanal>div {
            padding: 10px;
        }

        .grid-Canal_SouI {
            /* Soutirée ou Injectée */
            grid-template-columns: auto;
        }

        .grid-Canal_SetI {
            /* Soutirée et Injectée */
            grid-template-columns: auto auto;
        }

        .grid-NomCanal-1 {
            background-color: rgba(62, 194, 247, 0.5);
            font-weight: bold;
            text-align: center;
            overflow: hidden;
        }

        .grid-NomCanal-2 {
            background-color: rgba(62, 194, 247, 0.5);
            font-weight: bold;
            text-align: center;
            grid-column: 1 / span 2;
            overflow: hidden;
        }

        /* Style pour le SVG du graphique d'ensoleillement */
        .hom-soleil-edit {
            display: flex;
            justify-content: center;
            gap: 10px;
            padding: 10px;
        }

        .hom-SoleilSVG {
            border: 1px solid grey;
            border-radius: 4px;
            margin: 0px;
            touch-action:none;
            background: linear-gradient(to bottom, #000033 0%, #5555bb 60%, #000099 100%);

        }

        /* Style pour le SVG du graphique historique */
        .hom-graphPw-T {
            display: block;
            text-align: center;
            padding: 10px;
        }

        .hom-graphPw-edit {
            display: flex;
            justify-content: center;
            gap: 10px;
            padding: 10px;
        }

        .hom-graphPw-flex {
            display: flex;
            justify-content: space-around;
            gap: 10px;
            padding: 10px;
        }


        .hom-graphPw-table td {
            border: none;
            padding: 5px;
        }

        .hom-graphPw-color {
            width: 30px;
            height: 20px;
            border: 1px solid #888;
            box-sizing: border-box;
        }

        .hom-graphPwSVG {
            border: 1px solid grey;
            border-radius: 4px;
            margin: 0px;
            touch-action:none;
            background: linear-gradient(to bottom, #222222 0%, #444444 50%, #222222 100%);
        }

        /* Style pour le tableau ouvertures */
        .hom-Tableau_Ouvertures {
            color: white;
            width: 100%;
            background-color: #202020;
            font-size: 80%;
        }

        .hom-Tableau_Ouvertures td,
        .hom-Tableau_Ouvertures th {
            border: 1px solid grey;
            padding: 8px;
        }

        /* Style pour le SVG du graphique Energie */
        .hom-graphEn-T {
            display: block;
            text-align: center;
            padding: 10px;
        }

        .hom-graphEnSVG {
            border: 1px solid grey;
            border-radius: 4px;
            margin: 0px;
            touch-action:none;
            background: linear-gradient(to bottom, #222222 0%, #444444 50%, #222222 100%);
        }

        .hom-graphEn-2T {
            display: flex;
            justify-content: space-around;
            gap: 10px;
            padding: 10px;
        }

        .hom-graphEn-J {
            display: flex;
            justify-content: center;
            padding: 10px;
            gap: 4px;
        }

        .hom-graphEn-J label {
            margin-right: 10px;
        }

        .hom-graphEn-table td {
            border: none;
            padding: 5px;
        }
         /* SVG GAUGE */
        .zoneJauge {
            fill: none;
            stroke-width: 50;
        }
         .aiguilleG {
            fill: white;
        }
        .val-seuilJ {
            text-align: right;
            font-size:10px;
            vertical-align: bottom;
        }
        .hom-tab-Jauge div {
            padding: 4px;
            text-align: right;
        }
        .hom-tab-Jauge  input {
            max-width: 70px;
            text-align: right;
        })WEB0";

static const char Favicon[] PROGMEM = R"WEB0( <svg xmlns = "http://www.w3.org/2000/svg" width = "64" height = "64" viewBox = "15 20 42 40" >
<path d="m16 36h19l16 20h-20z" fill="#92d3f5"></path>
<circle cx="48" cy="32" fill="#fcea2b" r="8"></circle>
<g fill="none" stroke="#000" stroke-linecap="round" stroke-linejoin="round" stroke-width="2">
  <path d="m16 36h19l16 20h-20z"></path><path d="m17 43v13h10"></path>
  <path d="m24 46h19"></path><path d="m26 36 16 20"></path>
  <circle cx="48" cy="32" r="8"></circle>
</g>
</svg >)WEB0";

static const char JS_Para[] PROGMEM = R"WEB0(let refTempIP = [];

function Init() {
    let S = "";
    for (let i = 0; i < 4; i++) {
        S += `<div class='form cadre'>
                <div class="ligne" >
                    <label for="srcTemp${i}">Canal :${i}</label>
                    <select id="srcTemp${i}" onchange="checkDisabled();" >
                        <option value="tempNo" selected>Pas de mesure</option>
                        <option value="tempInt">Capteur DS18B20 Interne</option>
                        <option value="tempExt">Capteur DS18B20 ESP Externe</option>
                        <option value="tempMqtt">MQTT</option>
                    </select>
                </div>
            </div>
            <div class='form ligneTemperature' id='ligneTemperature${i}' >
                <div class='ligne' id='ligneNomTemp${i}'>
                    <label for='nomTemperature${i}' >Nom Température : </label>
                    <input type='text' name='nomTemperature${i}' id='nomTemperature${i}' >
                </div>
                <div class='ligne' id='ligneoffsetTemp${i}'>
                    <label for='offsetTemp${i}' >Offset mesure température (°C) :</label>
                    <input type='number' name='offsetTemp${i}' id='offsetTemp${i}' >
                </div>
                <div class='ligne' id='ligneTopicT${i}'>
                    <label for='TopicT${i}' >MQTT Topic Température :</label>
                    <input type='text' name='TopicT${i}' id='TopicT${i}' >
                </div>
                <div class='ligne' id='ligneIPtemp${i}'>
                    <label for='refTempIP${i}' >Ref. ESP-RMS externe :</label>
                    <select  id='refTempIP${i}' onchange="checkDisabled();" ></select>
                </div>
                <div class='ligne' id='canalEXTtemp${i}'>
                    <label for='canalEXT${i}' >Canal Température ESP-RMS externe :</label>
                    <select id='canalEXT${i}' onchange="C.LesTemperatures[${i}].canalTempExterne=this.value;GID('nomTemperature${i}').value = this.options[this.selectedIndex].text;" ></select>
                </div>
            </div>`;

    }
    GH("Sources_Temp", S);
    S = "<div class='ligne'><div class='nomR'><strong>Nom</strong></div><div class='nomR'><strong>Adresse IP</strong></div></div>";
    S += "<div class='ligne' id='Routeur_local'>";
    S += "<div id='RMS_Nom_local'  class='nomR'></div><input type='text' id='RMS_IP_local' readOnly>";
    S += "</div>";
    for (i = 0; i < 8; i++) {
        S += "<div class='ligne' id='Routeur_" + i + "' style='display:none;' >";
        S += "<div id='RMS_Nom" + i + "'  class='nomR'></div><input type='text' id='RMS_IP" + i + "' >";
        S += "</div>";
    }
    GH("Routeurs", S);
    LoadConfig();

}
function ImplantationConfig() {
    //Cookie clé d'accès

    let name = "CleAcces=";
    let decodedCookie = decodeURIComponent(document.cookie);
    let ca = decodedCookie.split(';');
    for (let i = 0; i < ca.length; i++) {
        let c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) GID("CleAcces").value = c.substring(name.length, c.length);
    }
    GH("nom_R", C.nomRouteur);
    GID("dhcp").checked = C.dhcpOn == 1 ? true : false;
    GID("adrIP").value = int2ip(C.localIP);
    GID("RMS_IP_local").value = int2ip(C.localIP);
    GID("gateway").value = int2ip(C.Gateway);
    GID("masque").value = int2ip(C.masque);
    GID("dns").value = int2ip(C.dns);
    GID("ModeP" + C.ModePara).checked = true;
    GID("ModeW").value = C.ModeReseau;
    GID("MQTTRepete").value = C.MQTTRepet;
    GID("MQTTIP").value = int2ip(C.MQTTIP);
    GID("MQTTPort").value = C.MQTTPort;
    GID("MQTTUser").value = C.MQTTUser;
    GID("MQTTpwd").value = C.MQTTPwd;
    GID("MQTTPrefix").value = C.MQTTPrefix;
    GID("MQTTPrefixEtat").value = C.MQTTPrefixEtat
    GID("MQTTdeviceName").value = C.MQTTdeviceName
    GID("subMQTT").checked = C.subMQTT == 1 ? true : false;
    GID("nomRouteur").value = C.nomRouteur;
    GH("RMS_Nom_local", C.nomRouteur);
    GID("CalibU").value = C.CalibU;
    GID("CalibI").value = C.CalibI;
    GID("TempoRTEon").checked = C.TempoRTEon == 1 ? true : false;
    GID("WifiSleep").checked = C.WifiSleep == 1 ? true : false;
    GID("Horlo").value = C.Horloge;
    GID("ComSurv").value = C.ComSurv;
    GID("Serial1").value = C.pSerial1;
    GID("Serial2").value = C.pSerial2;
    GID("pTriac").value = C.pTriac;
    GID("ESP").value = C.ESP32_Type;
    GID("pLED").value = C.LEDgroupe;
    GID("Rot").value = C.rotation;
    GID("DurEcran").value = C.DurEcran;
    GID("pUxI").value = C.pUxI;
    GID("pTemp").value = C.pTemp;
    GID("Horlo").value = C.Horloge;
    GID("hostname").value = C.hostname;
    GID("latitude").value = C.latitude;
    GID("longitude").value = C.longitude;
    GID("EstimCACSI").checked = C.EstimCACSI == 1 ? true : false;
    for (let c = 0; c < 4; c++) {
        GID("nomTemperature" + c).value = C.LesTemperatures[c].nomTemperature;
        GID("srcTemp" + c).value = C.LesTemperatures[c].Source_Temp;
        GID("TopicT" + c).value = C.LesTemperatures[c].TopicT;
        refTempIP[c] = C.LesTemperatures[c].refTempIP;
        GID("canalEXT" + c).value = C.LesTemperatures[c].canalTempExterne;
        GID("offsetTemp" + c).value = C.LesTemperatures[c].offsetTemp / 100;
    }
    //RMS Externes
    let d = 0;
    for (let c = 0; c < C.RouteurExt.length; c++) {
        if (C.RouteurExt[c].IP > 0) {
            GID("RMS_IP" + d).value = int2ip(C.RouteurExt[c].IP);
            GH("RMS_Nom" + d, C.RouteurExt[c].NomRouteur);
            GID("Routeur_" + d).style = "display: table-row";
            d++;
        }
    }
    if (d < 7) {
        GH("RMS_Nom" + d, "<small>Ajoutez une adresse IP de routeur <small>(et sauvegardez)</small> ----></small>");
        GID("Routeur_" + d).style = "display: table-row";
    }

    Liste_des_Sources();
    LoadParaTheme(); //Liste des fichiers de Themes
    SetHautBas();
    GH("nom_R", C.nomRouteur);

    checkDisabled();

}


function sendConfig() {
    GID("attente").style = "visibility: visible;";
    C.dhcpOn = GID("dhcp").checked ? 1 : 0;
    C.TempoRTEon = GID("TempoRTEon").checked ? 1 : 0;
    C.ComSurv = GID("ComSurv").value;
    C.subMQTT = GID("subMQTT").checked ? 1 : 0;
    C.WifiSleep = GID("WifiSleep").checked ? 1 : 0;
    C.hostname = GID("hostname").value;
    C.Horloge = GID("Horlo").value;
    C.pSerial1 = GID("Serial1").value;
    C.pSerial2 = GID("Serial2").value;
    C.pTriac = GID("pTriac").value;
    C.ModePara = document.querySelector('input[name="ModeP"]:checked').value;
    C.ModeReseau = GID("ModeW").value;
    C.ESP32_Type = GID("ESP").value;
    C.LEDgroupe = GID("pLED").value;
    C.rotation = GID("Rot").value;
    C.DurEcran = GID("DurEcran").value;
    C.pUxI = GID("pUxI").value;
    C.pTemp = GID("pTemp").value;
    C.EstimCACSI = GID("EstimCACSI").checked ? 1 : 0;

    if (C.ModePara == 0) { //Non Expert
        C.subMQTT = 0; C.WifiSleep = 1;
    }
    C.localIP = ip2int(GID("adrIP").value);
    C.Gateway = ip2int(GID("gateway").value);

    C.masque = ip2int(GID("masque").value);
    C.dns = ip2int(GID("dns").value);



    C.MQTTRepet = GID("MQTTRepete").value;
    C.MQTTIP = ip2int(GID("MQTTIP").value);
    C.MQTTPort = GID("MQTTPort").value;
    C.MQTTUser = GID("MQTTUser").value.trim();
    C.MQTTPwd = GID("MQTTpwd").value.trim();
    C.MQTTPrefix = GID("MQTTPrefix").value.trim();
    C.MQTTPrefixEtat = GID("MQTTPrefixEtat").value.trim();
    C.MQTTdeviceName = GID("MQTTdeviceName").value.trim();
    C.nomRouteur = GID("nomRouteur").value.trim();
    C.Theme = GID("Theme").value.trim();
    C.latitude = parseFloat(GID("latitude").value);
    C.longitude = parseFloat(GID("longitude").value);
    C.Horloge = GID("Horlo").value;
    C.CalibU = GID("CalibU").value;
    C.CalibI = GID("CalibI").value;

    for (let c = 0; c < 4; c++) {
        let QS = GID("srcTemp" + c).value;
        C.LesTemperatures[c].Source_Temp = (C.ModePara == 0) ? "tempNo" : QS;
        C.LesTemperatures[c].nomTemperature = GID("nomTemperature" + c).value.trim();
        C.LesTemperatures[c].TopicT = GID("TopicT" + c).value.trim()
        C.LesTemperatures[c].refTempIP = GID("refTempIP" + c).value;
        C.LesTemperatures[c].canalTempExterne = GID("canalEXT" + c).value;
        C.LesTemperatures[c].offsetTemp = GID("offsetTemp" + c).value * 100;
    }
    C.RouteurExt = []
    for (let i = 0; i < 8; i++) {
        let Vip = (C.ModePara == 0) ? 0 : ip2int(GID("RMS_IP" + i).value.trim());
        C.RouteurExt.push({ IP: Vip }); // On ne recupere que l'IP
    }

    j = 0;
    if (GID("TypeSource0").value.trim() == "") {
        GID("TypeSource0").value = "NotDef"; //On valide au minimum
        GID("CanalNom00").value = "Source Simulée 0";
        GID("CanalVoie00").value = 0;
        GID("CanalVoie01").value = -1;
        GID("CanalVoie02").value = -1;
    }
    for (let i = 0; i < 4; i++) { //4 sources maximum
        C.LaSource[i].Type = "";
        for (let k = 0; k < C.LaSource[i].CanalNom.length; k++) { // On efface anciennes valeurs
            C.LaSource[i].CanalNom[k] = "";
            C.LaSource[i].CanalVoie[k] = -1;
            C.LaSource[i].NomSoutir[k] = "Soutirée";
            C.LaSource[i].NomInject[k] = "Injectée";
        }
        let typeSource = GID("TypeSource" + i).value.trim();
        if (typeSource != "") { //On valide les sources ayant un type 
            if (typeSource != "Ext") {
                if (GID("CanalNom" + i + "0").value.trim() != "") { //On valide les sources interne avec un nom canal 0
                    C.LaSource[j].Type = typeSource;
                    C.LaSource[j].IP = ip2int(GID("SourceRMS_IP" + i).value);
                    C.LaSource[j].Port = GID("SourcePort" + i).value;
                    for (let k = 0; k < C.LaSource[j].CanalNom.length; k++) {
                        let ik = i.toString() + k.toString();
                        C.LaSource[j].CanalNom[k] = GID("CanalNom" + ik).value;
                        C.LaSource[j].CanalVoie[k] = GID("CanalVoie" + ik).value;
                        if (C.LaSource[j].CanalNom[k] != "" && C.LaSource[j].CanalVoie[k] < 0) C.LaSource[j].CanalVoie[k] = 0;//On valide
                        if (k == 1 && C.LaSource[j].Type != "Ext" && C.LaSource[j].Type != "UxIx2" && C.LaSource[j].Type != "ShellyEm" && C.LaSource[j].Type != "UxIx3") C.LaSource[j].CanalVoie[k] = -1;// on invalde le deuxième canal
                        if (k == 2 && C.LaSource[j].Type != "Ext" && C.LaSource[j].Type != "UxIx3") C.LaSource[j].CanalVoie[k] = -1;// on invalde le troisième canal
                        C.LaSource[j].NomSoutir[k] = GID("NomSoutir" + ik).value.trim();
                        C.LaSource[j].NomInject[k] = GID("NomInject" + ik).value.trim();
                    }

                    j++;
                } // Fin de la validation des sources interne ayant  un nom canal 0
            } else {// Fin de la validation des sources internes. Puis source externes
                if (GID("CanalNomExt" + i)) { //Dans certain cas peut ne pas exister
                    if (GID("CanalNomExt" + i).value.trim() != "") { //On valide les sources Externe avec un nom canal 0
                        C.LaSource[j].Type = typeSource;
                        C.LaSource[j].IP = GID("SourceExtIP" + i).value;
                        C.LaSource[j].Port = 80;
                        for (let k = 0; k < C.LaSource[j].CanalNom.length; k++) {
                            let ik = i.toString() + k.toString();
                            C.LaSource[j].CanalNom[k] = "";
                            C.LaSource[j].CanalVoie[k] = -1;
                        }
                        let select = GID("CanalNomExt" + i);
                        C.LaSource[j].CanalNom[0] = select.options[select.selectedIndex].text;
                        C.LaSource[j].CanalVoie[0] = select.value;

                        C.LaSource[j].NomSoutir[0] = GID("NomSoutirExt" + i).value.trim();
                        C.LaSource[j].NomInject[0] = GID("NomInjectExt" + i).value.trim();


                        j++;
                    } // Fin de la validation des sources externes ayant  un nom canal 0
                }
            }
        }


    }


    document.cookie = "CleAcces=" + GID("CleAcces").value.trim();
    if ((GID("dhcp").checked || checkIP("adrIP") && checkIP("gateway")) && (!GID("MQTTRepete").checked || checkIP("MQTTIP"))) {
        let adr = adrHttpRemote + "/ajaxNewConfig";

        fetch(adr, {
            method: "POST",                     // méthode HTTP
            headers: {
                "Content-Type": "application/json" // indique qu'on envoie du JSON
            },

            body: JSON.stringify(C)          // conversion de l'objet JS en texte JSON

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

function LoadParaTheme() {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            const T = JSON.parse(this.responseText);
            let D = `<option value="default" selected>Défaut</option>`;
            let S = "";
            if (T && T.FichiersCSS) {
                for (let i = 0; i < T.FichiersCSS.length; i++) {
                    if (T.FichiersCSS[i] == C.Theme) {
                        S += `<option value="${T.FichiersCSS[i]}" selected>${T.FichiersCSS[i]}</option>`;
                        D = `<option value="default" >Défaut</option>`;
                    } else {
                        S += `<option value="${T.FichiersCSS[i]}">${T.FichiersCSS[i]}</option>`;
                    }
                }
            }
            GID("Theme").innerHTML = D + S;
        }
    };
    let adr = adrHttpRemote + "/ParaThemeAjax";

    xhttp.open('GET', adr, true);
    xhttp.send();
}
function SendRAZ() {
    if (confirm("Confirmez la remise à zéro des historiques")) {
        let xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                let retour = this.responseText;
            }
        };
        let adr = adrHttpRemote + "/ajaxRAZhisto";
        xhttp.open('GET', adr, true);
        xhttp.send();
    }
}
function checkDisabled() {
    C.ModeReseau = GID("ModeW").value;
    C.ModePara = document.querySelector('input[name="ModeP"]:checked').value;
    let pESP = GID("ESP").value;
    GID("Bwifi").style.display = (C.ESP32_Type < 10) ? "inline-block" : "none";
    GID("infoIP").style.display = (GID("dhcp").checked || C.ModeReseau == 2) ? "none" : "table";
    GID("dhcp").style.visibility = (C.ModeReseau == 2) ? "hidden" : "visible";
    GID("ipreset").style.display = (C.ModeReseau == 2) ? "none" : "inherit";
    GID("rotation").style.display = (pESP == 2) ? "table-row" : "none";
    GID("dureeOn").style.display = (pESP == 2) ? "table-row" : "none";
    GID("l_wifi_0").style.display = (C.ModeReseau == 2) ? "none" : "table-row";
    GID("l_wifi_1").style.display = (C.ModeReseau == 2 || C.ModePara == 0) ? "none" : "table-row";
    GID("l_wifi_2").style.display = (C.ModeReseau == 2 || C.ModePara == 0 || C.ESP32_Type >= 10) ? "none" : "table-row";
    GID("listerouteurs").style.display = (C.ModeReseau == 2 || C.ModePara == 0) ? "none" : "block";



    for (let i = 0; i < 4; i++) {
        let QS = '#srcTemp' + i + ' option[value="tempExt"]';
        document.querySelector(QS).disabled = (C.ModeReseau == 2) ? true : false;
        QS = '#srcTemp' + i + ' option[value="tempMqtt"]';
        document.querySelector(QS).disabled = (C.ModeReseau == 2) ? true : false;
    }
    GID("donEnv").style.display = (C.ModeReseau == 2) ? "none" : "inline-block;";
    if (C.ModeReseau == 2) {
        GH("localIP", " : 192.168.4.1");
    } else {
        GH("localIP", "auto (DHCP) : " + int2ip(C.localIP));
    }


    GID("Zmqtt").style.display = ((GID("MQTTRepete").value != 0 || GID("subMQTT").checked) && C.ModePara > 0) ? "block" : "none";
    GID("LesSourcesTemp").style.display = (C.ModePara == 0) ? "none" : "block";
    GID("p_Temp").style.display = (C.ModePara == 0) ? "none" : "table-row";
    GID("Hostname").style.display = (C.ModePara == 0) ? "none" : "table";
    GID("SurvCom").style.display = (C.ModePara == 0) ? "none" : "block";
    GID("Localisation").style.display = (C.ModePara == 0) ? "none" : "block";
    GID("SourceExt").style.display = (C.ModePara == 0) ? "none" : "block";
    //Les températures
    for (let c = 0; c < 4; c++) {
        GID('ligneTemperature' + c).style.display = (GID("srcTemp" + c).value == "tempNo") ? "none" : "table";
        GID('ligneNomTemp' + c).style.display = (GID("srcTemp" + c).value == "tempExt") ? "none" : "table-row";
        GID('ligneTopicT' + c).style.display = (GID("srcTemp" + c).value == "tempMqtt") ? "table-row" : "none";
        GID('ligneIPtemp' + c).style.display = (GID("srcTemp" + c).value == "tempExt") ? "table-row" : "none";
        GID('canalEXTtemp' + c).style.display = (GID("srcTemp" + c).value == "tempExt") ? "table-row" : "none";
        GID('ligneoffsetTemp' + c).style.display = (GID("srcTemp" + c).value == "tempInt") ? "table-row" : "none";
        if (GID("srcTemp" + c).value == "tempMqtt") GID("Zmqtt").style.display = "table";

        if (GID("srcTemp" + c).value == "tempExt") {
            let IPext = GID("refTempIP" + c).value;
            let idxRMS = -1;

            for (let i = 0; i < C.RouteurExt.length; i++) {
                if (C.RouteurExt[i].IP == IPext) {
                    idxRMS = i;
                    break;
                }
            }
            if (idxRMS >= 0) {
                let Options = "";
                for (let j = 0; j < C.RouteurExt[idxRMS].TemperNom.length; j++) {
                    Options += "<option value='" + j + "'> " + C.RouteurExt[idxRMS].TemperNom[j] + "</option>";
                }
                GID("canalEXT" + c).innerHTML = Options;
                GID("canalEXT" + c).value = C.LesTemperatures[c].canalTempExterne;
                GID("nomTemperature" + c).value = C.RouteurExt[idxRMS].TemperNom[GID("canalEXT" + c).value];

            }
        }
    }
    GID("Horlo").options[0].disabled = (C.ModeReseau > 0) ? true : false;
    GID("Horlo").options[5].disabled = (C.LaSource[0].Type == 'Ext' && C.LaSource[0].IP != "") ? false : true;
    if (C.ModeReseau > 0 && C.Horloge == 0) { GID("Horlo").value = 2; C.Horloge = 2; }
    
    GID("SourcePlus").style.display = "none";
    GID("LabelVoie").style.visibility = "hidden";
    GID('Zcalib').style.display = "none";
    GID('Analog').style.display = "none";
    for (let i = 0; i < 4; i++) {
        C.LaSource[i].Type = GID("TypeSource" + i).value;
        let S = C.LaSource[i].Type;
        GID("LaSource" + i).style.display = (S != "") ? "table-row" : "none";
        if (S == "") GID("SourcePlus").style.display = "block";
        GID("SourceRMS_IP" + i).style.display = (S == "Enphase" || S == "SmartG" || S == "HomeW" || S == "ShellyEm" || S == "ShellyPro") ? "block" : "none";
        GID("SourcePort" + i).style.display = (S == "Linky" || S == "UxIx2" || S == "UxIx3" || S == "PZEM004T") ? "block" : "none";
        GID("SourceExtIP" + i).style.display = (S == "Ext") ? "block" : "none";
        for (let j = 0; j < C.LaSource[i].CanalNom.length; j++) {
            let ij = i.toString() + j.toString();
            GID("CanalVoie" + ij).style.display = (S == "ShellyEm" || S == "ShellyPro" || (S == "UxIx3" && j == 0)) ? "table-cell" : "none";
            let voieUxIx3mono = (S == "UxIx3" && GID("CanalVoie" + i + "0").value == 0);
            GID("S_Canal" + ij).style.display = (j == 0 || (j == 1 && (S == "ShellyEm" || voieUxIx3mono || S == "UxIx2")) || (j == 2 && voieUxIx3mono)) ? "table-row" : "none";
            if (S == "UxIx3" && j == 0) {
                if (voieUxIx3mono) { // trois Monophasé
                    GID("CanalVoie" + i + "1").value = 1;
                    GID("CanalVoie" + i + "2").value = 2;
                } else { //Triphasé
                    GID("CanalVoie" + i + "1").value = -1;
                    GID("CanalVoie" + i + "2").value = -1;
                }
            }
        }
        let IPext = GID("SourceExtIP" + i).value; // Récupère l'adresse IP externe sélectionnée 
        let idx = C.RouteurExt.findIndex(r => r.IP == IPext);
        if (idx >= 0) {
            let NomsCanauxExt = `<div class='ligne'>
                                <select class='TC' id='CanalNomExt${i}'>`;
            for (let j = 0; j < C.RouteurExt[idx].CanalNom.length; j++) {
                if (C.RouteurExt[idx].CanalNom[j] != "") NomsCanauxExt += "<option value='" + j + "'>" + C.RouteurExt[idx].CanalNom[j] + "</option>";
            }
            NomsCanauxExt += "</select>";
            NomsCanauxExt += `<div class='TC' style='width:40%;'>
                                    <div><input type='text'  id='NomSoutirExt${i}' title='Nom Puissance soutirée' value='${C.LaSource[i].NomSoutir[0]}'></div>
                                    <div><input type='text'  id='NomInjectExt${i}' title='Nom Puissance injectée' value='${C.LaSource[i].NomInject[0]}'></div>
                               </div>
                           </div>`;
            GID("S_CanalExt" + i).innerHTML = NomsCanauxExt;
            GID("CanalNomExt" + i).value = C.LaSource[i].CanalVoie[0]; // La voie donne l'indice du nom du canal externe
        }
        GID("S_CanalExt" + i).style.display = (S == "Ext") ? "table" : "none";
        GID("S_CanalInt" + i).style.display = (S == "Ext") ? "none" : "table";



        GH("S_complement" + i, "");
        if (S == "Pmqtt") {
            let T = `<div class='ligne'><label for='TopicP_${i}' >MQTT Topic Puissance :</label>
            <input type='text' name='TopicP_${i}' id='TopicP_${i}' value='${C.LaSource[i].TopicP}' onchange='C.LaSource[${i}].TopicP=this.value.trim();'></div>`;
            GH("S_complement" + i, T);
        }
        if (S == "Enphase") {
            let T = "<div class='ligne' >";
            T += "<label for='EnphaseUser' class='fsize12'>Enphase Envoy-S metered User : </label>";
            T += "<input type='text' name='EnphaseUser' id='EnphaseUser' value='" + C.EnphaseUser + "' onchange='C.EnphaseUser=this.value.trim();'  >";
            T += "</div>";
            T += "<div class='ligne' >";
            T += "<label for='EnphasePwd' class='fsize12'>Enphase Envoy-S metered Password : </label>";
            T += "<input type='password' name='EnphasePwd' id='EnphasePwd' value='" + C.EnphasePwd + "' onchange='C.EnphasePwd=this.value.trim()' ><div class='TCc' onclick='visupwd(this.id,\"EnphasePwd\");' id='iconPwd'>👁️</div>";
            T += "</div>"
            T += "<div class='ligne' >";
            T += "<label for='EnphaseSerial' class='fsize12'>Numéro série passerelle IQ Enphase : </label>";
            T += "<input type='text' name='EnphaseSerial' id='EnphaseSerial' value='" + C.EnphaseSerial + "' onchange='C.EnphaseSerial=this.value.trim();' >";
            T += "</div>";
            GH("S_complement" + i, T);
        }
        if (S == 'ShellyEm' || S == 'ShellyPro') {
            let T = "<div class='shem'><strong>Shelly (Pro) Em</strong><br>";
            T += "Monophasé : Courant maison mettre voie = 0,1 ou 2<br>Triphasé : mettre voie 3 ";
            T += "</div><div class='shem'><Strong>Shelly Em Gen3</strong><br>";
            T += "Courant maison sur voie 0 = 30, voie 1 = 31</div>";
            T += "<div class='shem'><Strong>Non utilisé</strong><br>";
            T += "Mettre voie  = -1</div>";
            GH("S_complement" + i, T);
            GID("LabelVoie").style.visibility = "visible";
        }
        if (S == 'UxI') {
            GID('Zcalib').style.display = "table";
            GID('Analog').style.display = "table-row";
        }
        if (S == 'UxIx3') {
            let T = "<div class='shem'><strong>UxIx3 (Triphasé ou 3 Monophasés)</strong><br>";
            T += "3 Monophasés :  voie = 0<br>Triphasé : voie = 3 ";
            T += "</div>";
            GH("S_complement" + i, T);
            GID("LabelVoie").style.visibility = "visible";
        }


    }




}
function visupwd(id, pwdId) {
    if (GID(pwdId).type === "password") {
        GID(pwdId).type = "text";
        GID(id).textContent = "🙈";
    } else {
        GID(pwdId).type = "password";
        GID(id).textContent = "👁️";
    }
}
function AdaptationSource() { };
function checkIP(id) {
    let S = GID(id).value;
    let Table = S.split(".");
    let valide = true;
    if (Table.length != 4) {
        valide = false;
    } else {
        for (let i = 0; i < Table.length; i++) {
            if (Table[i] > 255 || Table[i] < 0) valide = false;
        }
    }
    if (valide) {
        GID(id).style.color = "black";
    } else {
        GID(id).style.color = "red";
    }
    return valide;
}


function Reset() {
    GID("attente").style = "visibility: visible;";
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            GID('BoutonsBas').innerHTML = this.responseText;
            GID("attente").style = "visibility: hidden;";
            setTimeout(location.reload(), 1000);
            setTimeout(location.reload(), 10000);
        }
    };
    xhttp.open('GET', 'restart', true);
    xhttp.send();
}
function Liste_des_Sources() {
    let option = "<option value='NotDef'>Source non définie</option>";
    option += "<option value='UxI'>UxI</option>";
    option += "<option value='UxIx2'>UxIx2</option>";
    option += "<option value='UxIx3'>UxIx3</option>";
    option += "<option value='Linky'>Linky</option>";
    option += "<option value='Enphase'>Enphase-Envoy</option>";
    option += "<option value='SmartG'>SmartGateways</option>";
    option += "<option value='HomeW'>HomeWizard</option>";
    option += "<option value='ShellyEm'>Shelly Em</option>";
    option += "<option value='ShellyPro'>Shelly Pro Em</option>";
    option += "<option value='PZEM004T'>PZEM-004T</option>";
    option += "<option value='Ext' id='SourceExt'>ESP Externe</option>";
    option += "<option value='Pmqtt'>MQTT</option>";
    let optionPort = "<option value='1'>Port Série 1</option>";
    optionPort += "<option value='2'>Port Série 2</option>";

    let optionExt = "";
    for (let c = 0; c < C.RouteurExt.length; c++) {
        if (C.RouteurExt[c].IP > 0) {
            optionExt += "<option value='" + C.RouteurExt[c].IP + "' title='" + int2ip(C.RouteurExt[c].IP) + "'>" + C.RouteurExt[c].NomRouteur + "</option>";
        }
    }

    let S = `<div class='ligne'><div class='titreS'>Type</div><div class='titreS'>Ref. RMS <small>ou</small> Port Série</div><div class='titreS'><div class='ligneIB33'>Nom Puissance </div><div class='ligneIB33' id='LabelVoie'> Voie</div><div class='ligneIB33' > Sout./Injec.</div></div></div>`;
    for (let i = 0; i < 4; i++) {
        let a = 0.1 * (i % 2);
        S += `<div class='ligne2' id='LaSource${i}' style='background-color: rgba(128,128,128,${a});'>
                <select id='TypeSource${i}' onchange='checkDisabled();' >${option}</select>
                <div class='TC'>
                    <input type='text'  id='SourceRMS_IP${i}'  >
                    <select id='SourcePort${i}'>${optionPort}</select>
                    <select id='SourceExtIP${i}' onchange='checkDisabled();'>${optionExt}</select>
                </div>
                <div class='TC'>
                    <div  class='canalT' id='S_CanalInt${i}'>`;
        for (let j = 0; j < C.LaSource[i].CanalNom.length; j++) {
            let ij = i.toString() + j.toString();
            S += `<div  class='ligne' id = 'S_Canal${ij}'>
                                <input type='text'   style='width:40%' id='CanalNom${ij}' title='Nom de la source de puissance'>
                                <input type='number' title='Numéro de voie' id='CanalVoie${ij}' style='width:20%' onchange='checkDisabled();'>
                                <div class='TC' style='width:40%;'>
                                    <div ><input type='text'  id='NomSoutir${ij}' title='Nom Puissance soutirée'></div>
                                    <div><input type='text'  id='NomInject${ij}' title='Nom Puissance injectée'></div>
                                </div>
                              </div>`;
        }
        S += `</div>
                    <div id='S_complement${i}' class='canalT'></div>
                    <div id='S_CanalExt${i}' class='canalT'></div>
                </div>
             </div>`;
    }
    S += `<div id='SourcePlus' onclick='AddSource();' class='dispNone emoji' >➕️</div>`;
    GH("liste_sources", S);
    for (let i = 0; i < 4; i++) {
        GID("TypeSource" + i).value = C.LaSource[i].Type;
        GID("SourceRMS_IP" + i).value = int2ip(C.LaSource[i].IP);
        GID("SourcePort" + i).value = C.LaSource[i].Port;
        GID("SourceExtIP" + i).value = C.LaSource[i].IP;

        for (let j = 0; j < C.LaSource[i].CanalNom.length; j++) {
            let ij = i.toString() + j.toString();
            GID("CanalNom" + ij).value = C.LaSource[i].CanalNom[j];
            GID("CanalVoie" + ij).value = C.LaSource[i].CanalVoie[j];
            GID("NomSoutir" + ij).value = C.LaSource[i].NomSoutir[j];
            GID("NomInject" + ij).value = C.LaSource[i].NomInject[j];
        }
    }

    //Sources temperatures exterieurs
    for (let i = 0; i < 4; i++) {
        GH("refTempIP" + i, optionExt);
        GID("refTempIP" + i).value = refTempIP[i];
    }


}
function AddSource() {
    for (let i = 0; i < 4; i++) {
        if (C.LaSource[i].Type == "") {
            C.LaSource[i].Type = "NotDef";
            GID("TypeSource" + i).value = C.LaSource[i].Type;
            i = 4;
        }
    }
    checkDisabled();
}

)WEB0";

static const char JS_Brute[] PROGMEM = R"WEB0(
function ImplantationConfig() {
    SetHautBas();
    GH("nom_R", C.nomRouteur);
    setTimeout(LoadAppelDonneesImmediat, 100); // Ensure data are visible  after initial call
}





function LaDate(d) {
    return d.substr(0, 1) + ' ' + d.substr(5, 2) + '/' + d.substr(3, 2) + '/' + d.substr(1, 2) + ' ' + d.substr(7, 2) + 'h ' + d.substr(9, 2) + 'mn ' + d.substr(11, 2) + 's';
}
function LaVal(d) {
    d = parseInt(d);
    d = '           ' + d.toString();
    return d.substr(-9, 3) + ' ' + d.substr(-6, 3) + ' ' + d.substr(-3, 3);
}


//=============== Websocket =======================
let LePort = window.location.port;
if (LePort == "") LePort = "80";
let wsBrute;
if (location.hostname == "localhost") {
    wsBrute = new WebSocket("ws://" + RemoteIP + "/wsBrute");
} else {
    wsBrute = new WebSocket("ws://" + window.location.hostname + ":" + LePort + "/wsBrute");
}
wsBrute.onmessage = (event) => {

    let msg = JSON.parse(event.data);
    msg.HeureCourante = msg.HeureCourante.replace("T", " ");
    GH("date", msg.HeureCourante);
    switch (msg.MessageType) {
        case "BruteMessages":
            SortieMessagesEvenements(msg);
            break;
        case "BruteSources":
            SortieBruteSources(msg);
            break;
        case "BruteESP32":
            SortieBruteESP32(msg);
            break;

        default:
            console.warn("Unknown message", msg);
    }
};

function SortieMessagesEvenements(msg) {
    let S = "<table class='w100'>";
    let L = 0;
    for (let i = 0; i < msg.Message.length; i++) {
        if (msg.Message[i] != '') S += `<tr class="ligne${L++ % 2}"><td>` + msg.Message[i] + '</td></tr>';
    }
    S += '</table>';
    GH('BruteMessages', S);
}
let TabUxI = null;
function SortieBruteSources(msg) {
    let S = "";
    msg.DataBrute = msg.DataBrute.map(texte =>
        texte
            .replaceAll("\x1C", "<br>")
            .replaceAll("\x1D", "<br>")
            .replaceAll("\x1E", "<br>")
    );
    let UxI = false
    for (let i = 0; i < msg.numSource.length; i++) {
        if (msg.Type[i] == "UxI") UxI = true;


        S += "<div class='w100'>";
        S += `<div class='tableau w100'><table class='w100'>`;
        S += `<tr class="ligne0"><td>Source </td><td class='ri'>Ref : ` + msg.numSource[i] + `</td>`;
        S += `<td class='ri'>Type : ` + msg.Type[i] + `</td>`;
        S += `<td class='ri'>Data : ` + msg.Data[i] + `</td></tr>`;
        if (msg.Type[i] == "UxI") {

            TabUxI = msg.DataBrute[i].split("|");  // Tension, courant, tableau Tension, Tableau Courant
            if (TabUxI.length == 3) {
                if (TabUxI[1].length > 10 && TabUxI[2].length > 10) UxI = true;
            };
            let M = TabUxI[0].split(":");
            let valeurs = M[1].split(",");
            S += `<tr><td  >` + valeurs[0] + `</td><td  class='ri'>` + valeurs[1] + `</td><td  class='ri'>` + valeurs[2] + `</td><td></td></tr>`;
            S += `<tr><td colspan='4' ><svg id='GraphUxI_SVG' viewBox='0 0 510 200' class='brut-graphSVG'></svg></td></tr>`;
        } else if (msg.Type[i].indexOf("Shelly") != -1) {
                let Separ = msg.DataBrute[i].split("|*|");
                let JsonObjet = JSON.parse(Separ[1]);
                S += `<tr><td colspan='4' class='fsize8'><br>` + Separ[0] +`<br><div class='json-viewer'>` + JSON.stringify(JsonObjet, null, 2) + `</div></td></tr>`;

        } else {
            S += `<tr><td colspan='4' class='fsize8'><br>` + msg.DataBrute[i] + `</td></tr>`;
        }
        S += `</table></div>`;
        S += "</div><br>";
    }
    GH('BruteSources', S);
    if (UxI) TraceGraphUxI();
}
function SortieBruteESP32(D) {
    let S = '<table class="w100">';
    let H = parseInt(D.T_On);
    H = H + (parseFloat(D.T_On) - H) * 0.6;
    H = H.toFixed(2);
    H = H.replace(".", "h ") + "mn";
    let LaSource = C.LaSource[0].Type;
    if (LaSource == 'Ext') LaSource = "Externe (" + C.LaSource[0].Data + ")<br>" + int2ip(C.LaSource[0].IP);
    let typeESP32 = ["Non défini", "Wroom seul", "Ecran 480*320", "", "", "", "", "", "", "", "T-ETH-Lite"];
    let L = 0;
    S += `<tr class="ligne${L++ % 2}"><td>ESP32-S3 On depuis :</td><td>` + H + `</td></tr>`;
    S += `<tr class="ligne${L++ % 2}"><td>ESP32-S3 modèle :</td><td>` + typeESP32[D.ESP32_Type] + `</td></tr>`;
    S += `<tr class="ligne${L++ % 2}"><td>Source des mesures :</td><td>` + LaSource + `</td></tr>`;

    if (C.ModeReseau < 2) {
        if (D.ESP32_Type < 10) { //WIFI
            S += `<tr class="ligne${L++ % 2}"><td>Niveau WiFi :</td><td>` + D.RSSI + ' dBm</td></tr>';
            S += `<tr class="ligne${L++ % 2}"><td>Point d'acc&egrave;s WiFi :</td><td>` + D.BSSID + '</td></tr>';
            S += `<tr class="ligne${L++ % 2}"><td>R&eacute;seau WiFi :</td><td>` + D.ssid + '</td></tr>';
        }
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IP .local :</td><td>` + D.hostname + '.local</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse MAC ESP32 :</td><td>` + D.Mac + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IPv4 ESP32 :</td><td>` + D.IPv4 + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IPv6 ESP32 (Global) :</td><td>` + D.IPv6global + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IPv6 ESP32 (Local) :</td><td>` + D.IPv6local + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Adresse passerelle :</td><td>` + D.gateway + '</td></tr>';
        S += `<tr class="ligne${L++ % 2}"><td>Masque du r&eacute;seau :</td><td>` + D.masque + '</td></tr>';
    } else {
        S += `<tr class="ligne${L++ % 2}"><td>Adresse IP ESP32 (Point d'Accès) :</td><td>192.168.4.1</td></tr>`;
    }
    S += `<tr class="ligne${L++ % 2}"><td>Charge coeur 0 (Lecture Puissances) Min, Moy, Max :</td><td>` + D.coeur0 + ' ms</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Charge coeur 1 (Calcul + Serveur Web) Min, Moy, Max :</td><td>` + D.coeur1 + ' ms</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire RAM interne totale libre :</td><td>` + parseInt(D.RAMfree).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Plus gros bloc RAM interne libre :</td><td>` + parseInt(D.RAMbloc).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire PSRAM totale libre :</td><td>` + parseInt(D.PSRAMfree).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Plus gros bloc PSRAM libre :</td><td>` + parseInt(D.PSRAMbloc).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire stockage fichiers totale :</td><td>` + parseInt(D.FileTotal).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire stockage fichiers utilisée :</td><td>` + parseInt(D.FileUsed).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Mémoire stockage fichiers libre :</td><td>` + parseInt(D.FileTotal - D.FileUsed).toLocaleString('fr-FR') + ' octets</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Nombre d'interruptions en 15ms du Gradateur (signal Zc) : Filtrés/Brutes :</td><td>` + D.IT_triac + '</td></tr>';
    S += `<tr class="ligne${L++ % 2}"><td>Synchronisation au Secteur ou asynchrone horloge ESP32</td><td>` + D.IT_Mode + '</td></tr>';
    let Stemp = D.Nbr_DS18B20;
    if (parseInt(D.Nbr_DS18B20) > 0) Stemp += '<span class="fsize10">' + D.AllTemp + '</span>';
    S += `<tr class="ligne${L++ % 2}"><td>Nombre de capteurs de température DS18B20 :</td><td>` + Stemp + '</td></tr>';


    if (D.MachineNom) {

        S += `<tr class="ligne${L++ % 2}"><td style="text-align:center;"><strong>Note échanges entre routeurs</strong></td><td></td></tr>`;
        for (let i = 0; i < D.MachineNom.length; i++) {
            S += `<tr class="ligne${L++ % 2}"><td>` + D.MachineNom[i] + '</td><td>' + D.MachineNote[i] + '</td></tr>';
        }
    }
    S += '</table>';

    GH('DataESP32', S);
}

//=============== Fin Websocket =======================

//======= Trace Graphe UxI ============
function TraceGraphUxI() {
    let Vmax = 1;
    let Imax = 1;
    let V = TabUxI[1].split(',');
    V.pop(); // Supprime le dernier élément vide si présent
    let I = TabUxI[2].split(',');
    I.pop(); // Supprime le dernier élément vide si présent
    for (let i = 0; i < V.length; i++) {
        V[i] = parseFloat(V[i]);
        I[i] = parseFloat(I[i]);
        if (Math.abs(V[i]) > Vmax) Vmax = Math.abs(V[i]);
        if (Math.abs(I[i]) > Imax) Imax = Math.abs(I[i]);
    }




    //Positionnement graphe en X
    let X0 = 15;

    let Ymin = 15;
    let Ymax = 195;
    let Y0 = 105; //Valeurs positives uniquement
    let dY = 9;

    const NS = "http://www.w3.org/2000/svg";
    const GraphSvg = GID("GraphUxI_SVG");

    GraphSvg.innerHTML = "";

    //Trace Axes
    let x_Max = 500;
    let NBmax = 100;
    let stepTick = 10;


    let axes = document.createElementNS(NS, "polyline");
    axes.setAttribute("points", `${X0},${Ymax} ${X0},${Ymin} ${X0},${Y0} ${x_Max},${Y0}`);
    axes.setAttribute("stroke", "var(--brute-axes)");
    axes.setAttribute("stroke-width", 1);
    axes.setAttribute("fill", "none");
    GraphSvg.appendChild(axes);




    for (let Nb = 0; Nb <= NBmax; Nb += stepTick) {
        let x, y, h;
        x = X0 + (x_Max - X0) * Nb / NBmax;
        y = Y0;
        h = Nb / 5;
        if (x > X0) {
            let tick = document.createElementNS(NS, "line");
            tick.setAttribute("x1", x);
            tick.setAttribute("y1", y);
            tick.setAttribute("x2", x);
            tick.setAttribute("y2", y + 4);
            tick.setAttribute("stroke", "var(--brute-axes)");
            tick.setAttribute("stroke-width", 1);
            GraphSvg.appendChild(tick);
            let txt = document.createElementNS(NS, "text");
            txt.setAttribute("x", x);
            txt.setAttribute("y", y + 9);
            txt.setAttribute("fill", "var(--brute-axes)");
            txt.textContent = h;
            txt.setAttribute("font-size", "5");
            GraphSvg.appendChild(txt);
        }

    }
    let txtMs = document.createElementNS(NS, "text");
    txtMs.setAttribute("x", 500);
    txtMs.setAttribute("y", 100);
    txtMs.setAttribute("fill", "var(--brute-axes)");
    txtMs.textContent = "ms";
    txtMs.setAttribute("font-size", "5");
    GraphSvg.appendChild(txtMs);


    // === Courbes ===

    let courbe = document.createElementNS(NS, "polyline");
    courbe.setAttribute("stroke", "var(--brute-V)");
    courbe.setAttribute("stroke-width", 1);
    courbe.setAttribute("fill", "none");
    let step = (x_Max - X0) / (V.length - 1);
    let courbePoints = "";
    for (let j = 0; j < V.length; j++) {
        const valeur = V[j] / Vmax;

        let x = X0 + j * step;
        let y = Y0 - valeur * (Y0 - Ymin);
        courbePoints += `${x},${y} `;

    }
    courbe.setAttribute("points", courbePoints);
    GraphSvg.appendChild(courbe);
    let txt = document.createElementNS(NS, "text");
    txt.setAttribute("x", 100);
    txt.setAttribute("y", 12);
    txt.setAttribute("fill", "var(--brute-V)");
    txt.textContent = "Volt";
    txt.setAttribute("font-size", "10");
    GraphSvg.appendChild(txt);

    let courbeI = document.createElementNS(NS, "polyline");
    courbeI.setAttribute("stroke", "var(--brute-I)");
    courbeI.setAttribute("stroke-width", 1);
    courbeI.setAttribute("fill", "none");
    let stepI = (x_Max - X0) / (I.length - 1);
    let courbePointsI = "";
    for (let j = 0; j < I.length; j++) {
        const valeur = I[j] / Imax;

        let x = X0 + j * stepI;
        let y = Y0 - valeur * (Y0 - Ymin);
        courbePointsI += `${x},${y} `;

    }
    courbeI.setAttribute("points", courbePointsI);
    GraphSvg.appendChild(courbeI);
    let txtI = document.createElementNS(NS, "text");
    txtI.setAttribute("x", 400);
    txtI.setAttribute("y", 12);
    txtI.setAttribute("fill", "var(--brute-I)");
    txtI.textContent = "Ampere";
    txtI.setAttribute("font-size", "10");
    GraphSvg.appendChild(txtI);

})WEB0";

static const char JS_Commun[] PROGMEM = R"WEB0(const RemoteIP = "192.168.0.180";  //Debug sur image locale
let adrHttpRemote = '';
if (location.hostname == "localhost") { //Test sur serveur local sur PC
    adrHttpRemote = 'http://' + RemoteIP;
}
function GID(id) { return document.getElementById(id); }
function GH(id, T) {
    if (GID(id)) {
        GID(id).innerHTML = T;
    }
}
function GV(id, T) { GID(id).value = T; }
let C; //Objet Configuration
function LoadConfig() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            C = JSON.parse(this.responseText);
            ImplantationConfig();
        }
    };
    let adr = adrHttpRemote + "/ConfigAjax";

    xhttp.open('GET', adr, true);

    xhttp.send();
}
function LoadAppelDonneesImmediat() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            let retour = this.responseText;

        }
    };
    let adr = adrHttpRemote + "/AppelDonneesImmediat";
    xhttp.open('GET', adr, true);
    xhttp.send();

}
function LaVal(d) {
    d = parseInt(d);
    return d.toLocaleString('fr-FR');
}
function Hdeci2Hmn(H) {
    var HI = parseInt(H);
    return Math.floor(HI / 100) + ":" + ("0" + Math.floor(0.6 * (HI + 0.4 - 100 * Math.floor(HI / 100)))).substr(-2, 2);
}
function SetHautBas() {
    var S = `<div class='onglets'>
                    <div class='Bonglet Baccueil'><a href='/'>Accueil</a></div>
                    <div class='Bonglet Bbrut'><a href='/pageHtmlBrute.html'>Donn&eacute;es brutes</a></div>
                    <div class='Bonglet Bparametres'><a href='/pageHtmlPara.html'>Param&egrave;tres</a></div>
                    <div class='Bonglet Bactions'><a href='/pageHtmlActions.html'>Actions</a></div>
            </div>
            <div id='onglets2'>
                <div class='Bonglet2 Bgeneraux'><a href='/pageHtmlPara.html'>Généraux</a></div>
                <div class='Bonglet2 Bexport'><a href='/pageHtmlExport.html'>Import / Export</a></div>
                <div class='Bonglet2 Bota'><a href='/pageHtmlOTA.html'>Mise à jour par OTA</a></div>
                <div id='Bwifi' class='Bonglet2 Bwifi'><a href='/pageHtmlWifi.html'>WIFI</a></div>
                <div class='Bonglet2 Bheure' id='Bheure'><a href='/pageHtmlHeure.html'>Jour / Heure</a></div>
            </div>
            <h2 id='nom_R'>Routeur Photovoltaïque RMS-S3</h2>`;
    GH("lesOnglets", S);
    GH("pied", `<div>Routeur Photovoltaïque Multi-Sources RMS-S3 Version : <span id='version'>${C.Version}</span></div><div><a href='https://F1ATB.fr/fr' >F1ATB.fr</a></div>`);
}
const clamp = (val, min, max) => Math.min(Math.max(val, min), max);
function int2ip(V) {
    var ipInt = parseInt(V);
    return ((ipInt >>> 24) + '.' + (ipInt >> 16 & 255) + '.' + (ipInt >> 8 & 255) + '.' + (ipInt & 255));
}
function ip2int(ip) {
    ip = ip.trim();
    return ip.split('.').reduce(function (ipInt, octet) { return (ipInt << 8) + parseInt(octet, 10); }, 0) >>> 0;
}
let ES = String.fromCharCode(27); //Escape Separator
let FS = String.fromCharCode(28); //File Separator
let GS = String.fromCharCode(29); //Group Separator
let RS = String.fromCharCode(30); //Record Separator
let US = String.fromCharCode(31); //Unit Separator)WEB0";

static const char JS_Home[] PROGMEM = R"WEB0(
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
};)WEB0";

static const char JS_Action[] PROGMEM = R"WEB0(
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

)WEB0";

static const char JS_boolean[] PROGMEM = R"WEB0(
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
                if (typeEntree == "heure" ) {
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

)WEB0";

static const char JS_paraBloc[] PROGMEM = R"WEB0(
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
)WEB0";

static const char JS_carteGraphEnergie[] PROGMEM = R"WEB0(const EnergieMax = 300000; //300kWh max pour enlever des chiffres déconnants
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
})WEB0";

static const char JS_carteGraphEnsoleillement[] PROGMEM = R"WEB0(function createCarteGraphEnsoleillement(z) {
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
})WEB0";

static const char JS_carteGraphHistorique[] PROGMEM = R"WEB0(let Puissance10mn = null;
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
    let iAction=0;
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
})WEB0";

static const char JS_carteJauge[] PROGMEM = R"WEB0(const cx = 255
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

)WEB0";

static const char JS_carteTableauOuvertures[] PROGMEM = R"WEB0(function createCarteTableauOuvertures(z) {
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
})WEB0";

static const char JS_carteTableauPuissance[] PROGMEM = R"WEB0(function createCarteTableauPuissance(z) {
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
})WEB0";

