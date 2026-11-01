#!/usr/bin/env python3
"""Vuelca a JSON los paneles ORIGINALES de FlatCAM que necesita la primera versión del port.

- Pestaña Propiedades de cada tipo de objeto: `GerberObjectUI`, `ExcellonObjectUI`, `GeometryObjectUI`, `CNCObjectUI`,
  después del `set_ui` del objeto original (nivel Básico y valores de fábrica en el formulario).
- Pestaña Herramienta: NCC, Paint, Perforación y Corte, después de su `run()` original (que carga los valores de fábrica).

Escribe `tools/gui_dump/panels/<Clase>.json`; `gen_cpp.py --panels` genera las clases C++.
Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tools/gui_dump/dump_panels.py
"""
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fc_runtime as fr  # noqa: E402
from dump_tree import dump_widget  # noqa: E402

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "panels")

# clase del panel -> (tipo de objeto, módulo, clase del objeto)
OBJECT_UIS = {"GerberObjectUI": ("gerber", "appObjects.FlatCAMGerber", "GerberObject"),
              "ExcellonObjectUI": ("excellon", "appObjects.FlatCAMExcellon", "ExcellonObject"),
              "GeometryObjectUI": ("geometry", "appObjects.FlatCAMGeometry", "GeometryObject"),
              "CNCObjectUI": ("cncjob", "appObjects.FlatCAMCNCJob", "CNCJobObject")}
# opciones de herramientas que AppObject.new_object copia a cada tipo de objeto
EXTRA_OPTIONS = {"gerber": ("tools_iso_",), "excellon": ("tools_drill_", "tools_mill_"), "geometry": ("tools_mill_",),
                 "cncjob": ()}


def object_with_ui(fa, ou, cls, kind, module, objcls):
    """Crea el objeto original como `AppObject.new_object` (opciones de fábrica) y le aplica su `set_ui`, que esconde
    lo que no corresponde al nivel Básico y carga los valores en el formulario."""
    obj = getattr(fr.import_original(module), objcls)("x")
    for option, value in fa.options.items():
        if option.startswith(kind + "_"):
            obj.options[option[len(kind) + 1:]] = value
        elif option.startswith(EXTRA_OPTIONS[kind]):
            obj.options[option] = value
    obj.set_ui(getattr(ou, cls)(app=fa))
    return obj.ui
# atributo de la aplicación -> nombre de la clase C++ del panel
TOOLS = {"ncclear_tool": "NccToolUI", "paint_tool": "PaintToolUI", "drilling_tool": "DrillingToolUI",
         "cutout_tool": "CutOutToolUI", "dblsidedtool": "DblSidedToolUI"}


def main():
    qapp, fa, gui = fr.build_app()
    os.makedirs(OUT, exist_ok=True)
    ou = fr.import_original("appGUI.ObjectUI")
    written = []
    fr.import_original("appObjects.FlatCAMObj").FlatCAMObj.app = fa
    for cls, spec in OBJECT_UIS.items():
        ui = object_with_ui(fa, ou, cls, *spec)
        # Como en FlatCAMObj.build_ui: el panel vive dentro del área de Propiedades (cambia márgenes y fondo).
        fa.ui.properties_scroll_area.takeWidget()
        fa.ui.properties_scroll_area.setWidget(ui)
        data = dump_widget(ui, ui)
        data["class"] = cls
        written.append(write(cls, data))
    for attr, cls in TOOLS.items():
        tool = getattr(fa, attr)
        tool.run(toggle=False)
        ui = getattr(tool, "ui", None) or getattr(tool, "t_ui", None)
        data = dump_widget(tool, *(x for x in (ui, tool) if x is not None))
        data["class"] = cls
        written.append(write(cls, data))
    with open(os.path.join(OUT, "index.txt"), "w") as f:
        f.write("\n".join(written) + "\n")


def write(cls, data):
    path = os.path.join(OUT, cls + ".json")
    with open(path, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=1)
    return cls


if __name__ == "__main__":
    main()
