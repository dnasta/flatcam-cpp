#!/usr/bin/env python3
"""Vuelca la pestaña Preferencias original (`MainGUI.preferences_tab` con los 8 formularios de `appGUI/preferences`) a
`tools/gui_dump/panels/PreferencesTabUI.json`, con los valores de fábrica cargados como `show_preferences_gui` +
`defaults_write_form`, y la tabla `defaults_form_fields` (clave de preferencia -> miembro del panel generado) a
`src/gui/resources/preferences_fields.json`.

Los nombres de los miembros llevan el grupo adelante para que no se repitan: `general_app_group.units_radio` ->
`general_app_group_units_radio`.

Uso (desde tools/gui_dump):  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \
      python dump_preferences.py
"""
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fc_runtime as fr  # noqa: E402
from dump_tree import Dumper  # noqa: E402

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "panels", "PreferencesTabUI.json")
FIELDS = os.path.join(fr.ROOT, "src", "gui", "resources", "preferences_fields.json")
FORMS = ["general_defaults_form", "gerber_defaults_form", "excellon_defaults_form", "geometry_defaults_form",
         "cncjob_defaults_form", "tools_defaults_form", "tools2_defaults_form", "util_defaults_form"]


def main():
    from PyQt5 import QtCore
    qapp, fa, gui = fr.build_app()
    pm_mod = fr.import_original("appGUI.preferences.PreferencesUIManager")
    pm = pm_mod.PreferencesUIManager(defaults=fa.defaults, data_path=fr.ROOT, ui=gui, inform=fa.inform)
    pm.show_preferences_gui()
    pm.defaults_write_form()
    # Nombres: los atributos de la ventana (pref_save_button...), de cada formulario (sus grupos) y de cada grupo (con el
    # grupo adelante).
    names = {}
    for k, v in vars(gui).items():
        if isinstance(v, QtCore.QObject):
            names.setdefault(id(v), k)
    for form in FORMS:
        f = getattr(gui, form)
        names[id(f)] = form
        for gk, g in vars(f).items():
            if not isinstance(g, QtCore.QObject):
                continue
            names[id(g)] = gk
            for wk, w in vars(g).items():
                if isinstance(w, QtCore.QObject) and id(w) not in names:
                    names[id(w)] = "%s_%s" % (gk, wk)
    tab = gui.preferences_tab
    names.pop(id(tab), None)
    d = Dumper(names)
    data = {"root": d.widget(tab), "actions": d.actions, "class": "PreferencesTabUI"}
    with open(OUT, "w", encoding="utf-8") as fh:
        json.dump(data, fh, ensure_ascii=False, indent=1)
    fields = {}
    for key, w in pm.defaults_form_fields.items():
        fields[key] = {"member": names.get(id(w)), "py": type(w).__name__}
    with open(FIELDS, "w", encoding="utf-8") as fh:
        json.dump(fields, fh, ensure_ascii=False, indent=1)
    missing = [k for k, v in fields.items() if not v["member"]]
    print("Escrito", OUT, "y", FIELDS, "-", len(fields), "claves;", "sin nombre:", missing)
    print("Clases:", sorted({v["py"] for v in fields.values()}))


if __name__ == "__main__":
    main()
