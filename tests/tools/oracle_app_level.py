#!/usr/bin/env python3
"""Oráculo del nivel de la aplicación (`global_app_level`: 'b' Básico, 'a' Avanzado).

Con la interfaz PyQt5 real arma, con cada nivel, los paneles que cambian: Propiedades de Gerber, Excellon, Geometría,
CNC Job, Script y Documento (`set_ui` del objeto original, como `tools/gui_dump/dump_panels.py`) y las herramientas
Isolation y Perforación (`run`). De cada widget con nombre de atributo guarda si está escondido (`isHidden`), de las
tablas las columnas escondidas y, de los controles, su valor (casillas, radios, combos y el texto de los rótulos).
Además, `ToolDrilling.process_slot_as_drills` (Taladrar ranuras) sobre 8 ranuras.

Escribe `tests/data/app_level/oracle.json`.
Uso: MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
     python tests/tools/oracle_app_level.py
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402
import dump_panels as dp  # noqa: E402

OUT = os.path.join(ROOT, "tests", "data", "app_level", "oracle.json")
PANELS = ("GerberObjectUI", "ExcellonObjectUI", "GeometryObjectUI", "CNCObjectUI", "ScriptObjectUI",
          "DocumentObjectUI")
TOOLS = {"isolation_tool": "IsolationToolUI", "drilling_tool": "DrillingToolUI"}


def state(*sources):
    from PyQt5 import QtWidgets
    out = {}
    for src in sources:
        for name, w in vars(src).items():
            if name.startswith("_") or name in out or not isinstance(w, QtWidgets.QWidget):
                continue
            d = {"hidden": w.isHidden()}
            if isinstance(w, QtWidgets.QTableWidget):
                d["hiddenCols"] = [c for c in range(w.columnCount()) if w.isColumnHidden(c)]
            if isinstance(w, QtWidgets.QCheckBox):
                d["value"] = w.isChecked()
            elif isinstance(w, QtWidgets.QComboBox):
                d["value"] = w.currentText()
            elif hasattr(w, "get_value") and type(w).__name__ == "RadioSet":
                d["value"] = w.get_value()
            elif isinstance(w, QtWidgets.QLabel) and name == "level":
                d["value"] = w.text()
            out[name] = d
    return out


def slot_cases():
    """`ToolDrilling.process_slot_as_drills` original (tomado con `ast`) con la Shapely real: hay que correrlo antes de
    `build_app`, que reemplaza módulos y rompe a Shapely. Con Shapely 1.8, `within` corta la serie cuando el punto
    interpolado no cae exacto sobre la ranura."""
    import ast
    from shapely.geometry import LineString, Point
    src = open(os.path.join(fr.REF, "appTools", "ToolDrilling.py"), encoding="utf-8").read()
    fn = next(n for n in ast.walk(ast.parse(src)) if isinstance(n, ast.FunctionDef) and n.name == "process_slot_as_drills")
    fn.decorator_list = []
    ns = {"LineString": LineString}
    exec(compile(ast.Module(body=[fn], type_ignores=[]), "ToolDrilling.py", "exec"), ns)
    slots = []
    for (x0, y0, x1, y1), overlap, last in (((0, 0, 10, 0), 1.0, False), ((0, 0, 10, 0), 1.0, True),
                                           ((0, 0, 10.3, 0), 1.0, True), ((0, 0, 0, -7.5), 0.4, False),
                                           ((1, 2, 4, 6), 0.5, False), ((1, 2, 4, 6), 0.5, True),
                                           ((0.1, 0.2, 3.7, 1.9), 0.25, True), ((0, 0, 2, 0), 0.0, True)):
        pts = ns["process_slot_as_drills"]((Point(x0, y0), Point(x1, y1)), overlap, add_last_pt=last)
        slots.append({"slot": [x0, y0, x1, y1], "overlap": overlap, "last": last,
                      "drills": [[p.x, p.y] for p in pts]})
    return slots


def main():
    slots = slot_cases()
    qapp, fa, gui = fr.build_app()
    ou = fr.import_original("appGUI.ObjectUI")
    fr.import_original("appObjects.FlatCAMObj").FlatCAMObj.app = fa
    fa.myKeywords = []
    out = {}
    for level in ("b", "a"):
        fa.defaults["global_app_level"] = level
        res = {}
        for cls in PANELS:
            ui = dp.object_with_ui(fa, ou, cls, *dp.OBJECT_UIS[cls])
            res[cls] = state(ui)
        for attr, cls in TOOLS.items():
            tool = getattr(fa, attr)
            tool.run(toggle=False)
            ui = getattr(tool, "ui", None) or getattr(tool, "t_ui", None)
            res[cls] = state(ui)
        out[level] = res
    out["slots"] = slots
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=1, sort_keys=True)
    # Lo que cambia entre los dos niveles.
    for cls in out["b"]:  # noqa
        for name in sorted(out["b"][cls]):
            b, a = out["b"][cls][name], out["a"][cls].get(name)
            if a != b:
                print(cls, name, b, "->", a)


if __name__ == "__main__":
    main()
