#!/usr/bin/env python3
"""Arma las tablas de traducción de la interfaz: `src/gui/resources/i18n/<idioma>.json`.

Cada tabla tiene:
- `ui`: los textos de la interfaz generada (`gen_cpp.py` los escribe en inglés con `trUi_`), emparejados posición por
  posición entre el volcado de la interfaz original en inglés y el volcado en ese idioma. Así cada texto sale como lo
  muestra el original en ese idioma, también los compuestos ("New Project...\tCtrl+N").
- `msg`: los mensajes de la lógica (`tr_`): el diccionario `translate/<idioma>.bin` del original (en español, con las
  correcciones de `es_corrections.json`) y los textos propios del port (`port_strings.json`).

Los volcados se hacen con la interfaz original (entorno `old`), un directorio por idioma:

    for L in en es de fr it pt_BR ro ru tr; do
      for s in dump_tree.py dump_preferences.py dump_panels.py; do
        FC_LANG=$L FC_DUMP_DIR=<dir>/$L micromamba run -n old python tools/gui_dump/$s
      done
    done
    python3 tools/gui_dump/build_translations.py <dir>

El volcado en inglés es el que se copia a `tools/gui_dump/main_gui.json` y `panels/` (lo que genera el C++).
"""
import collections
import json
import os
import pickle
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", ".."))
REF = os.path.join(ROOT, "reference", "FlatCAM_2024_4", "FlatCAM")
OUT = os.path.join(ROOT, "src", "gui", "resources", "i18n")

# Los idiomas del original (translate/*.bin) con su nombre en ese idioma (el combo de Preferencias).
LANGUAGES = {"de": "Deutsch", "en": "English", "es": "Español", "fr": "Français", "it": "Italiano",
             "pt_BR": "Português (Brasil)", "ro": "Română", "ru": "Русский", "tr": "Türkçe"}


def dump_files(d):
    names = ["main_gui.json"] + [l.strip() + ".json" for l in open(os.path.join(d, "index.txt")) if l.strip()]
    return names


def pair(en, other, pairs):
    """Recorre los dos volcados a la vez y junta cada texto en inglés con el del otro idioma."""
    if type(en) != type(other):
        raise ValueError("los volcados no tienen la misma forma")
    if isinstance(en, dict):
        if "actions" in en and isinstance(en["actions"], dict):  # las acciones van por la dirección de memoria
            a, b = list(en["actions"].values()), list(other["actions"].values())
            if len(a) != len(b):
                raise ValueError("distinta cantidad de acciones")
            for x, y in zip(a, b):
                pair(x, y, pairs)
        for k, v in en.items():
            if k != "actions" and k in other:
                pair(v, other[k], pairs)
    elif isinstance(en, list):
        if len(en) != len(other):
            raise ValueError("listas de distinto largo")
        for x, y in zip(en, other):
            pair(x, y, pairs)
    elif isinstance(en, str) and en != other and not en.isdigit():
        pairs[en][other] += 1


def main():
    dumps = sys.argv[1]
    os.makedirs(OUT, exist_ok=True)
    port = json.load(open(os.path.join(HERE, "port_strings.json"), encoding="utf-8"))
    files = dump_files(os.path.join(dumps, "en"))
    en = {f: json.load(open(os.path.join(dumps, "en", f), encoding="utf-8")) for f in files}
    with open(os.path.join(OUT, "languages.json"), "w", encoding="utf-8") as fh:
        json.dump(LANGUAGES, fh, ensure_ascii=False, indent=1)
    for lang, name in LANGUAGES.items():
        pairs = collections.defaultdict(collections.Counter)
        if lang != "en":
            for f in files:
                pair(en[f], json.load(open(os.path.join(dumps, lang, f), encoding="utf-8")), pairs)
        ui = {}
        for k, c in pairs.items():
            ui[k] = c.most_common(1)[0][0]  # pocos casos con dos traducciones: la más usada
            if len(c) > 1:
                print("%s: %r tiene %d traducciones; se usa %r" % (lang, k[:40], len(c), ui[k][:40]))
        with open(os.path.join(REF, "translate", lang + ".bin"), "rb") as fh:
            msg = {k: v for k, v in pickle.load(fh).items() if isinstance(k, str) and isinstance(v, str) and k != v}
        if lang == "es":
            with open(os.path.join(HERE, "es_corrections.json"), encoding="utf-8") as fh:
                msg.update({k: v for k, v in json.load(fh).items() if not k.startswith("_")})
        for k, tr in port.items():
            if not k.startswith("_") and tr.get(lang) and tr[lang] != k:
                msg[k] = tr[lang]
        # Los mensajes del port con %1, %2... que el núcleo arma con un valor adentro (tr_ los reconoce por la forma).
        patterns = sorted(k for k in port if not k.startswith("_") and "%1" in k and port[k].get("pattern"))
        data = {"code": lang, "name": name, "patterns": patterns, "ui": dict(sorted(ui.items())),
                "msg": dict(sorted(msg.items()))}
        with open(os.path.join(OUT, lang + ".json"), "w", encoding="utf-8") as fh:
            json.dump(data, fh, ensure_ascii=False, indent=0)
        print("%s: %d textos de la interfaz, %d mensajes" % (lang, len(ui), len(msg)))


if __name__ == "__main__":
    main()
