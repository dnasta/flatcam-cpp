#!/usr/bin/env python3
"""Oráculo de Pasta de soldar: ejecuta `SolderPaste.on_create_geo` ORIGINAL (tomado con `ast`) sobre la máscara de
pasta (los pads) de Gerber del usuario y después `CNCjob.generate_gcode_from_solderpaste_geo` original con el
postprocesador Paste_1 real, y escribe en `tests/golden_solderpaste.h` los trazos de cada boquilla y el G-code.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_solderpaste.py
"""
import ast
import logging
import math
import os
import sys
from copy import deepcopy
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_solderpaste.h")
DATA_DEF = {"tools_solderpaste_z_start": 0.05, "tools_solderpaste_z_dispense": 0.1, "tools_solderpaste_z_stop": 0.05,
            "tools_solderpaste_z_travel": 0.1, "tools_solderpaste_z_toolchange": 1.0,
            "tools_solderpaste_xy_toolchange": "0.0, 0.0", "tools_solderpaste_frxy": 150,
            "tools_solderpaste_frz": 150, "tools_solderpaste_frz_dispense": 1.0, "tools_solderpaste_speedfwd": 300,
            "tools_solderpaste_dwellfwd": 1, "tools_solderpaste_speedrev": 200, "tools_solderpaste_dwellrev": 1,
            "tools_solderpaste_pp": "Paste_1"}
# (archivo, boquillas)
CASES = [("PCB principal-B_Mask.gbs", [1.0, 0.3]), ("luz_de_noche/luz de noche Pascual-B_Cu.gbr", [1.0, 0.3]),
         ("PCB principal-B_Mask.gbs", [0.3]), ("PCB principal-B_Mask.gbs", [5.0])]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    import appPreProcessor  # noqa: E402
    for f in sorted(os.listdir(os.path.join(REF, "preprocessors"))):
        if f.endswith(".py") and f != "__init__.py":
            load("pp_" + f[:-3], os.path.join(REF, "preprocessors", f)).math = math
    from shapely import wkt as W
    from shapely.geometry import LineString, MultiPolygon, Polygon
    app = FakeApp()
    app.defaults.update({"cncjob_coords_type": "G90", "units": "MM", "tools_solderpaste_pp": "Paste_1"})
    app.preprocessors = appPreProcessor.preprocessors
    app.log = logging.getLogger("oracle")
    pg.Gerber.app = app
    camlib.Geometry.app = app
    camlib.CNCjob.app = app
    path = os.path.join(REF, "appTools", "ToolSolderPaste.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "SolderPaste")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "on_create_geo"]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(LineString=LineString, MultiPolygon=MultiPolygon, Polygon=Polygon, deepcopy=deepcopy,
              distance=camlib.distance, _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    d = lambda x: W.dumps(x, rounding_precision=17, trim=True)  # noqa: E731
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_solderpaste.py - no editar a mano.",
           "// Referencia: SolderPaste.on_create_geo y CNCjob.generate_gcode_from_solderpaste_geo originales.",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_solderpaste {",
           "struct Tool { int uid; double dia; std::vector<std::string> paths; const char* gcode; };",
           "struct Case { const char* file; std::vector<double> nozzles; bool created; std::vector<double> bounds;"
           " std::vector<Tool> tools; };",
           "inline const std::vector<Case> kCases = {"]
    for f, nozzles in CASES:
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        core = ns["Core"]()
        core.app = app
        core.decimals = 4
        core.units = "MM"
        core.options = {}
        core.tooltable_tools = {i + 1: {"tooldia": dia, "data": dict(DATA_DEF)} for i, dia in enumerate(nozzles)}
        created = []

        def new_object(kind, name, init):
            o = mock.MagicMock()
            o.options = {}
            r = init(o, app)
            if r != 'fail':
                created.append(o)
        app.app_obj = mock.MagicMock()
        app.app_obj.new_object = new_object
        core.on_create_geo("x", g, use_thread=False)
        tools = []
        bounds = []
        if created:
            o = created[0]
            # Los límites del CNC Job son los de la Geometría (new_object los calcula de todas sus herramientas).
            from shapely.ops import unary_union
            allp = unary_union([p for t in o.tools.values() for p in t["solid_geometry"]])
            xmin, ymin, xmax, ymax = allp.bounds
            bounds = [xmin, ymin, xmax, ymax]
            for uid, t in o.tools.items():
                job = camlib.CNCjob(units="MM", kind="geometry", steps_per_circle=64)
                job.coords_decimals = 4
                job.fr_decimals = 2
                job.xy_end = "0.0, 0.0"
                job.tool = uid
                job.options = {"xmin": xmin, "ymin": ymin, "xmax": xmax, "ymax": ymax, "tooldia": t["tooldia"],
                               "tool_dia": t["tooldia"]}
                gc = job.generate_gcode_from_solderpaste_geo(**t)
                tools.append((uid, t["tooldia"], [d(p) for p in t["solid_geometry"]], gc))
        out.append("{%s, {%s}, %s, {%s}, {%s}}," % (cs(f), ", ".join(repr(v) for v in nozzles),
                                             "true" if created else "false", ", ".join(repr(b) for b in bounds),
                                             ", ".join("{%d, %r, {%s}, %s}" % (u, dia, ", ".join(cs(p) for p in ps),
                                                                               cs(gc)) for u, dia, ps, gc in tools)))
        print(f, nozzles, bool(created), [(u, dia, len(ps), gc.count("\n")) for u, dia, ps, gc in tools])
    out += ["};", "}  // namespace golden_solderpaste"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
