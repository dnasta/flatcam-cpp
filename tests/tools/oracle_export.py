#!/usr/bin/env python3
"""Oráculo de exportar Gerber y Excellon: ejecuta `MenuFileHandlers.export_gerber` / `export_excellon` (app_Main.py) y
`GerberObject.export_gerber` / `ExcellonObject.export_excellon` ORIGINALES, tomados del fuente con `ast`, sobre archivos
leídos con los parsers originales. Escribe el texto (salvo la línea "Created on", que tiene la fecha) en
`tests/golden_export.h`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_export.py
"""
import ast
import logging
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_export.h")
GERBERS = ["luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr", "luz_de_noche/luz de noche Pascual-Logo.gbr",
           "PCB principal-B_Cu.gbl", "PCB principal-F_Silkscreen.gto", "PCB principal-B_Mask.gbs"]
EXCELLONS = ["luz_de_noche/luz de noche Pascual-PTH.drl", "PCB principal-Drill.drl"]
# (unidades, enteros, decimales, ceros) y (unidades, enteros, decimales, ceros, formato)
GERBER_FORMATS = [("IN", 2, 4, "L"), ("MM", 3, 5, "T"), ("MM", 4, 6, "L")]
EXCELLON_FORMATS = [("INCH", 2, 4, "LZ", "dec"), ("METRIC", 3, 3, "LZ", "ndec"), ("METRIC", 3, 4, "TZ", "ndec"),
                    ("INCH", 2, 4, "TZ", "ndec")]


def methods(path, clsname, names):
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == clsname)
    return [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in names]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.defaults.update({"gerber_buffering": "full", "gerber_circle_steps": 64, "excellon_circle_steps": 64,
                         "global_last_save_folder": "/tmp", "global_last_folder": "/tmp"})
    app.log = logging.getLogger("oracle")
    app.version, app.version_date = "2024.4", "2024/04/15"
    pg.Gerber.app = app
    pe.Excellon.app = app
    camlib.Geometry.app = app
    from shapely.geometry import LineString, MultiLineString, Point, Polygon

    ns = dict(LineString=LineString, MultiLineString=MultiLineString, Point=Point, Polygon=Polygon,
              log=logging.getLogger("x"))
    grb_m = ast.Module(body=methods(os.path.join(REF, "appObjects", "FlatCAMGerber.py"), "GerberObject",
                                    {"export_gerber"}), type_ignores=[])
    exc_m = ast.Module(body=methods(os.path.join(REF, "appObjects", "FlatCAMExcellon.py"), "ExcellonObject",
                                    {"export_excellon"}), type_ignores=[])
    io_m = ast.Module(body=methods(os.path.join(REF, "app_Main.py"), "MenuFileHandlers",
                                   {"export_gerber", "export_excellon"}), type_ignores=[])
    for m in (grb_m, exc_m, io_m):
        ast.fix_missing_locations(m)
    g_ns, e_ns, io_ns = dict(ns), dict(ns), dict(ns)
    exec(compile(grb_m, "FlatCAMGerber.py", "exec"), g_ns)
    exec(compile(exc_m, "FlatCAMExcellon.py", "exec"), e_ns)

    class ExcObj(pe.Excellon):
        export_excellon = e_ns["export_excellon"]

    class GrbObj(pg.Gerber):
        export_gerber = g_ns["export_gerber"]
    from datetime import datetime
    io_ns.update(datetime=datetime, ExcellonObject=ExcObj, _=lambda s: s)
    exec(compile(io_m, "app_Main.py", "exec"), io_ns)

    class IO:
        export_gerber = io_ns["export_gerber"]
        export_excellon = io_ns["export_excellon"]

        def __init__(self):
            self.app = app
            self.defaults = app.defaults
            self.inform = app.inform
    io = IO()

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_export.py - no editar a mano.",
           "// Referencia: export_gerber / export_excellon originales (app_Main.py, FlatCAMGerber.py, FlatCAMExcellon.py).",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_export {",
           "struct GerberCase { const char* file; const char* units; int whole; int fract; const char* zeros;"
           " const char* text; };",
           "struct ExcellonCase { const char* file; const char* units; int whole; int fract; const char* zeros;"
           " const char* format; const char* text; };",
           "inline const std::vector<GerberCase> kGerber = {"]

    def strip(text):
        return "\n".join(l for l in text.split("\n") if "Created on" not in l)
    for f in GERBERS:
        g = GrbObj(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        for u, w, fr, z in GERBER_FORMATS:
            app.defaults.update({"gerber_exp_units": u, "gerber_exp_integer": w, "gerber_exp_decimals": fr,
                                 "gerber_exp_zeros": z, "units": "MM"})
            text = io.export_gerber("obj", None, local_use=g, use_thread=False)
            out.append('{%s, "%s", %d, %d, "%s", %s},' % (cs(f), u, w, fr, z, cs(strip(text))))
            print(f, u, w, fr, z, text.count("\n"), "líneas", flush=True)
    out += ["};", "inline const std::vector<ExcellonCase> kExcellon = {"]
    for f in EXCELLONS:
        e = ExcObj(geo_steps_per_circle=64)
        e.parse_file(os.path.join(DATA, f))
        e.create_geometry()
        for u, w, fr, z, form in EXCELLON_FORMATS:
            app.defaults.update({"excellon_exp_units": u, "excellon_exp_integer": w, "excellon_exp_decimals": fr,
                                 "excellon_exp_zeros": z, "excellon_exp_format": form,
                                 "excellon_exp_slot_type": "routing", "units": "MM"})
            text = io.export_excellon("obj", None, local_use=e, use_thread=False)
            out.append('{%s, "%s", %d, %d, "%s", "%s", %s},' % (cs(f), u, w, fr, z, form, cs(strip(text))))
            print(f, u, w, fr, z, form, text.count("\n"), "líneas", flush=True)
    out += ["};", "}  // namespace golden_export"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
