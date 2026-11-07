#!/usr/bin/env python3
"""Oráculo de exportar SVG: ejecuta `MenuFileHandlers.export_svg` (app_Main.py, tomado con `ast`) y
`Geometry.export_svg` (camlib) ORIGINALES sobre Gerber y Excellon leídos con los parsers originales, y guarda el archivo
escrito en `tests/golden_svg.h`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_svg.py
"""
import ast
import logging
import os
import sys
import tempfile
from xml.dom.minidom import parseString as parse_xml_string

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_svg.h")
GERBERS = ["luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr", "luz_de_noche/luz de noche Pascual-Logo.gbr",
           "PCB principal-B_Mask.gbs"]
EXCELLONS = ["luz_de_noche/luz de noche Pascual-PTH.drl"]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.defaults.update({"gerber_buffering": "full", "gerber_circle_steps": 64, "excellon_circle_steps": 64,
                         "global_last_save_folder": "/tmp", "global_last_folder": "/tmp", "global_open_style": True})
    app.log = logging.getLogger("oracle")
    for c in (camlib.Geometry, pg.Gerber, pe.Excellon):
        c.app = app
    tree = ast.parse(open(os.path.join(REF, "app_Main.py"), encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "MenuFileHandlers")
    fn = next(f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "export_svg")
    mod = ast.Module(body=[fn], type_ignores=[])
    ast.fix_missing_locations(mod)
    ns = dict(parse_xml_string=parse_xml_string, _=lambda s: s)
    exec(compile(mod, "app_Main.py", "exec"), ns)

    class Collection:
        obj = None

        def get_by_name(self, name):
            return self.obj

    class Proc:
        def new(self, *a):
            import contextlib
            return contextlib.nullcontext()

    class IO:
        export_svg = ns["export_svg"]

        def __init__(self):
            self.app = app
            self.defaults = app.defaults
            self.inform = app.inform
    app.collection = Collection()
    app.proc_container = Proc()
    app.file_saved = app.inform
    io = IO()
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_svg.py - no editar a mano.",
           "// Referencia: export_svg original (app_Main.py y camlib.Geometry.export_svg, Shapely 1.8.4).",
           "#pragma once", "#include <vector>", "namespace golden_svg {",
           "struct Case { const char* file; bool excellon; const char* svg; };", "inline const std::vector<Case> kCases = {"]
    for f in GERBERS + EXCELLONS:
        exc = f in EXCELLONS
        if exc:
            o = pe.Excellon(geo_steps_per_circle=64)
            o.default_data = {}  # como en el objeto de la aplicación; sin él, create_geometry falla
            o.parse_file(os.path.join(DATA, f))
            o.create_geometry()
            o.kind = "excellon"
        else:
            o = pg.Gerber(steps_per_circle=64)
            o.parse_file(os.path.join(DATA, f))
            o.kind = "gerber"
        app.collection.obj = o
        fd, path = tempfile.mkstemp(suffix=".svg")
        os.close(fd)
        io.export_svg("obj", path)
        text = open(path).read()
        os.remove(path)
        out.append("{%s, %s, %s}," % (cs(f), str(exc).lower(), cs(text)))
        print(f, len(text), flush=True)
    out += ["};", "}  // namespace golden_svg"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
