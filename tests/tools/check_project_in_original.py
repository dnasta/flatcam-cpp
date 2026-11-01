#!/usr/bin/env python3
"""Verifica que el FlatCAM 2024.4 ORIGINAL abre un proyecto guardado por el port.

Hace lo mismo que `open_project`: lee el JSON (con o sin XZ) con `camlib.dict2obj`, crea cada objeto con su clase
original, copia las opciones de la aplicación (`new_object`), aplica `from_dict`, calcula `bounds()`, le pone su panel
de Propiedades (`set_ui` + `build_ui`, que leen las opciones y las herramientas) y, en los CNC Job, arma el texto que
exportaría (`export_gcode`). Termina con error si algo falla.

Uso:
  FLATCAM_SAVE_COPY=/tmp/p.FlatPrj build/tests/gui_tests --gtest_filter=GuiProject.SavesObjectsCreatedInThePort
  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
      python tests/tools/check_project_in_original.py /tmp/p.FlatPrj
"""
import json
import lzma
import os
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "..", "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402

CLASSES = {"gerber": ("appObjects.FlatCAMGerber", "GerberObject", "GerberObjectUI"),
           "excellon": ("appObjects.FlatCAMExcellon", "ExcellonObject", "ExcellonObjectUI"),
           "geometry": ("appObjects.FlatCAMGeometry", "GeometryObject", "GeometryObjectUI"),
           "cncjob": ("appObjects.FlatCAMCNCJob", "CNCJobObject", "CNCObjectUI")}


def main(path):
    qapp, fa, gui = fr.build_app()
    camlib = fr.import_original("camlib")
    ou = fr.import_original("appGUI.ObjectUI")
    fr.import_original("appObjects.FlatCAMObj").FlatCAMObj.app = fa
    pp = fr.import_original("appPreProcessor")
    fa.preprocessors = pp.load_preprocessors()
    with open(path, "rb") as f:
        magic = f.read(6)
    opener = lzma.open if magic == b"\xfd7zXZ\x00" else open
    with opener(path, "rb") as f:
        d = json.loads(f.read().decode("utf-8"), object_hook=camlib.dict2obj)
    fa.options.update(d["options"])
    errors = 0
    for od in d["objs"]:
        kind, name = od["kind"], od["options"]["name"]
        module, cls, ui = CLASSES[kind]
        try:
            obj = getattr(fr.import_original(module), cls)(name)
            for option in fa.options:  # new_object
                if option.find(kind + "_") == 0:
                    obj.options[option[len(kind) + 1:]] = fa.options[option]
            obj.from_dict(od)
            b = obj.bounds()
            obj.set_ui(getattr(ou, ui)(app=fa))
            obj.build_ui()
            extra = ""
            if kind == "cncjob":
                out = os.path.join(tempfile.mkdtemp(), "x.nc")
                obj.app.defaults = fa.defaults
                text = obj.export_gcode(filename=None, to_file=False)
                extra = " G-code: %s" % (type(text).__name__)
            print("OK  %-9s %-50s bounds=(%.3f, %.3f, %.3f, %.3f)%s" % (kind, name, *b, extra))
        except Exception as e:  # noqa: BLE001
            import traceback
            traceback.print_exc()
            print("ERR %-9s %-50s %s: %s" % (kind, name, type(e).__name__, e))
            errors += 1
    sys.exit(1 if errors else 0)


if __name__ == "__main__":
    main(sys.argv[1])
