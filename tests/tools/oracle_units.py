#!/usr/bin/env python3
"""Oráculo del cambio de unidades: ejecuta `App.on_toggle_units` y `App.scale_defaults` ORIGINALES (tomados de
app_Main.py con `ast`) con la pestaña Preferencias real (PyQt5) y un `LoudDict` cuyo aviso escribe el campo, como la
aplicación. Guarda en `tests/data/preferences/units.json` las opciones después de pasar de mm a pulgadas desde la
pestaña (`to_inch`), de volver a mm (`back_to_mm`) y de la tecla Q (`no_pref`, sin escalar las preferencias).

Uso: MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
     python tests/tools/oracle_units.py
"""
import ast
import json
import os
import sys
from unittest import mock

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402

OUT = os.path.join(ROOT, "tests", "data", "preferences", "units.json")


class FakeBox:
    """QMessageBox que contesta con el primer botón (Ok)."""
    Question = AcceptRole = RejectRole = 0

    def __init__(self, *a, **k):
        self.buttons = []

    def addButton(self, *a):
        b = object()
        self.buttons.append(b)
        return b

    def clickedButton(self):
        return self.buttons[0]

    def __getattr__(self, name):
        return lambda *a, **k: None


def load_methods():
    from PyQt5 import QtGui, QtWidgets
    path = os.path.join(fr.REF if hasattr(fr, "REF") else os.path.join(ROOT, "reference", "FlatCAM_2024_4", "FlatCAM"),
                        "app_Main.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "App")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef)
             and f.name in ("on_toggle_units", "scale_defaults", "dec_format", "on_toggle_units_click")]
    module = ast.Module(body=[ast.ClassDef(name="UnitsCore", bases=[], keywords=[], body=meths, decorator_list=[])],
                        type_ignores=[])
    ast.fix_missing_locations(module)
    widgets = mock.MagicMock(wraps=QtWidgets)
    widgets.QMessageBox = FakeBox
    ns = dict(QtWidgets=widgets, QtGui=QtGui, _=lambda s: s, GeometryObject=type("G", (), {}))
    exec(compile(module, path, "exec"), ns)
    return ns["UnitsCore"]


def main():
    qapp, fa, gui = fr.build_app()
    common = fr.import_original("appCommon.Common")
    pm_mod = fr.import_original("appGUI.preferences.PreferencesUIManager")
    defaults = common.LoudDict(fa.defaults)
    defaults.report_usage = lambda *a: None
    fa.defaults = defaults
    pm = pm_mod.PreferencesUIManager(defaults=defaults, data_path=ROOT, ui=gui, inform=fa.inform)
    pm.show_preferences_gui()
    pm.defaults_write_form()
    defaults.set_change_callback(lambda key: pm.defaults_write_form_field(field=key))
    Core = load_methods()

    class App(Core):
        pass

    app = App()
    app.defaults = defaults
    app.ui = gui
    app.preferencesUiManager = pm
    app.log = mock.MagicMock()
    app.toggle_units_ignore = False
    app.decimals = 4
    app.collection = mock.MagicMock()
    app.collection.get_list.return_value = []
    app.collection.get_active.return_value = None
    app.options = {}
    app.on_options_app2project = lambda: (pm.defaults_read_form(), app.options.update(defaults))
    for name in ("app_obj", "plotcanvas", "inform", "set_screen_units", "plot_all"):
        setattr(app, name, mock.MagicMock())
    out = {"startup": dict(defaults)}
    radio = gui.general_defaults_form.general_app_group.units_radio
    radio.set_value("IN")
    app.on_toggle_units(no_pref=False)
    out["to_inch"] = dict(defaults)
    radio.set_value("MM")
    app.on_toggle_units(no_pref=False)
    out["back_to_mm"] = dict(defaults)
    app.on_toggle_units_click()
    out["no_pref"] = dict(defaults)
    with open(OUT, "w", encoding="utf-8") as fh:
        json.dump(out, fh, ensure_ascii=False, indent=1, sort_keys=True, default=list)
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
