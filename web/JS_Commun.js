const RemoteIP = "192.168.0.180";  //Debug sur image locale
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
let US = String.fromCharCode(31); //Unit Separator