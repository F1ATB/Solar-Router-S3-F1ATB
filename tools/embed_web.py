
from pathlib import Path

Import("env")
ROOT = Path(env["PROJECT_DIR"])
WEB_DIR = ROOT / "web"
OUT = ROOT / "src" / "generated" / "WebFiles.h"

FILES = {
    "pageHtmlActions": "pageHtmlActions.html",
    "pageHtmlBrute": "pageHtmlBrute.html",
    "pageHtmlWifi": "pageHtmlWifi.html",
    "pageHtmlExport": "pageHtmlExport.html",
    "pageHtmlHeure": "pageHtmlHeure.html",
    "pageHtmlHome": "pageHtmlHome.html",
    "pageHtmlOTA": "pageHtmlOTA.html",
    "pageHtmlPara": "pageHtmlPara.html",
    "pageHtmlCleAcces": "pageHtmlCleAcces.html",
    "CommunCSS": "CommunCSS.css",
    "HomeCSS": "HomeCSS.css",
    "Favicon": "favicon.ico",
    "JS_Para": "JS_Para.js",
    "JS_Brute": "JS_Brute.js",
    "JS_Commun": "JS_Commun.js",
    "JS_Home": "JS_Home/JS_Home.js",
    "JS_Action": "JS_Actions/JS_Action.js",
    "JS_boolean": "JS_Actions/JS_boolean.js",
    "JS_paraBloc": "JS_Actions/JS_paraBloc.js",
    "JS_carteGraphEnergie": "JS_Home/JS_carteGraphEnergie.js",
    "JS_carteGraphEnsoleillement": "JS_Home/JS_carteGraphEnsoleillement.js",
    "JS_carteGraphHistorique": "JS_Home/JS_carteGraphHistorique.js",
    "JS_carteJauge": "JS_Home/JS_carteJauge.js",
    "JS_carteTableauOuvertures": "JS_Home/JS_carteTableauOuvertures.js",
    "JS_carteTableauPuissance": "JS_Home/JS_carteTableauPuissance.js"
}


def raw_string(content):
    # Choisit un délimiteur absent du contenu
    for i in range(10000):
        delimiter = f"WEB{i}"
        if f'){delimiter}"' not in content:
            return f'R"{delimiter}({content}){delimiter}"'

    raise RuntimeError("Délimiteur C++ introuvable")


OUT.parent.mkdir(parents=True, exist_ok=True)

parts = [
    "// Fichier généré automatiquement. Ne pas modifier.\n",
    "#pragma once\n",
    "#include <Arduino.h>\n\n",
]

for symbol, filename in FILES.items():
    path = WEB_DIR / filename

    if not path.is_file():
        raise FileNotFoundError(f"Fichier manquant : {path}")

    content = path.read_text(encoding="utf-8")

    parts.append(
        f"static const char {symbol}[] PROGMEM = "
        f"{raw_string(content)};\n\n"
    )

OUT.write_text("".join(parts), encoding="utf-8", newline="\n")

print(f"Ressources web intégrées : {OUT}")
