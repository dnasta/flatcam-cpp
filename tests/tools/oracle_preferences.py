#!/usr/bin/env python3
"""Oráculo de las Preferencias: ejecuta el `PreferencesUIManager` ORIGINAL (con la interfaz PyQt5 real, como
`tools/gui_dump/dump_preferences.py`) y guarda en `tests/data/preferences/oracle.json`:

- `factory_read`: las opciones que da `defaults_read_form()` después de `defaults_write_form()` con las de fábrica
  (el tipo de cada valor: entero, real, texto, booleano, tupla, None);
- `factory_text`: el texto de cada campo de texto después de escribirlo (los formatos de `set_value`);
- `roundtrip_read`: lo mismo partiendo de `factory_read` pasado por JSON (como queda al guardarlo y volver a cargarlo:
  las tuplas pasan a ser listas);
- `typed_read`: el valor de algunos campos con textos escritos a mano (fórmulas, tuplas, comas).

Uso: MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
     python tests/tools/oracle_preferences.py
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402

OUT = os.path.join(ROOT, "tests", "data", "preferences", "oracle.json")

TYPED = {
    "tools_ncc_tools": ["1, 0.5", "(0.1, 0.2)", "[1, 2.5]", "0.2", "0.1,", "1/4, 3*2", "", "1 2"],
    "geometry_toolchangexy": ["10, -5", "(1.5,2)"],
    "geometry_startz": ["3/2", "1,5", "", "2"],
    "tools_drill_startz": ["10", "0.5"],
}


def text_of(w):
    if hasattr(w, "toPlainText"):
        return w.toPlainText()
    if hasattr(w, "entry"):
        return w.entry.text()
    if hasattr(w, "text") and callable(w.text) and type(w).__name__ not in ("FCCheckBox",):
        try:
            return w.text()
        except TypeError:
            return None
    return None


def main():
    qapp, fa, gui = fr.build_app()
    pm_mod = fr.import_original("appGUI.preferences.PreferencesUIManager")
    defaults = fa.defaults
    pm = pm_mod.PreferencesUIManager(defaults=defaults, data_path=ROOT, ui=gui, inform=fa.inform)
    pm.show_preferences_gui()
    factory = dict(type(defaults).factory_defaults) if hasattr(type(defaults), "factory_defaults") else dict(defaults)
    pm.defaults_write_form(source_dict=factory)
    fields = pm.defaults_form_fields
    texts = {}
    for k, w in fields.items():
        if type(w).__name__ in ("FCEntry", "NumericalEvalEntry", "NumericalEvalTupleEntry", "FCTextArea", "FCColorEntry"):
            texts[k] = text_of(w)
    read1 = {k: w.get_value() for k, w in fields.items()}
    loaded = json.loads(json.dumps(read1))
    pm.defaults_write_form(source_dict=loaded)
    read2 = {k: w.get_value() for k, w in fields.items()}
    typed = {}
    for k, values in TYPED.items():
        typed[k] = []
        for t in values:
            fields[k].setText(t)
            typed[k].append([t, fields[k].get_value()])
    with open(OUT, "w", encoding="utf-8") as fh:
        json.dump({"factory_read": read1, "factory_text": texts, "roundtrip_read": read2, "typed_read": typed}, fh,
                  ensure_ascii=False, indent=1, sort_keys=True)
    print("Escrito", OUT, len(read1), "claves")


if __name__ == "__main__":
    main()
