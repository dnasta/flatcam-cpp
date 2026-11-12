#!/usr/bin/env python3
"""Oráculo de Verificación de reglas: ejecuta las comprobaciones ORIGINALES de `appTools/ToolRulesCheck.py`
(`check_traces_size`, `check_inside_gerber_clearance`, `check_gerber_clearance`, `check_holes_size`,
`check_holes_clearance`, `check_gerber_annular_ring`, tomadas con `ast`) sobre los archivos del usuario y escribe en
`tests/golden_rules.h` las violaciones (como conjunto) de cada regla.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_rules.py
"""
import ast
import logging
import os
import sys
from copy import deepcopy

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_rules.h")
CU = "luz_de_noche/luz de noche Pascual-B_Cu.gbr"
EDGE = "luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr"
PTH = "luz_de_noche/luz de noche Pascual-PTH.drl"
DCU = "PCB principal-B_Cu.gbl"
MASK = "PCB principal-B_Mask.gbs"
SILK = "PCB principal-F_Silkscreen.gto"
DRL = "PCB principal-Drill.drl"
# (regla, archivos, tamaño)
CASES = [("traces", [CU], 0.5), ("traces", [DCU], 0.3), ("inside", [CU], 0.3), ("inside", [MASK], 0.2),
         ("inside", [EDGE], 0.1), ("gerber", [CU, EDGE], 1.5), ("gerber", [SILK, MASK], 0.1),
         ("holes_size", [PTH], 1.2), ("holes_clearance", [PTH], 2.0), ("holes_clearance", [DRL], 1.0),
         ("ring", [CU, PTH], 0.5), ("ring", [DCU, DRL], 0.3)]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.defaults.update({"excellon_circle_steps": 64})
    app.log = logging.getLogger("oracle")
    pg.Gerber.app = app
    pe.Excellon.app = app
    from shapely.geometry import MultiPolygon, Polygon
    from shapely.ops import nearest_points
    path = os.path.join(REF, "appTools", "ToolRulesCheck.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "RulesCheck")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name.startswith("check_")]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(MultiPolygon=MultiPolygon, Polygon=Polygon, nearest_points=nearest_points, deepcopy=deepcopy,
              _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    C = ns["Core"]
    cache = {}

    def obj(f):
        if f in cache:
            return cache[f]
        if f.endswith(".drl"):
            e = pe.Excellon(geo_steps_per_circle=64)
            e.default_data = {}
            e.parse_file(os.path.join(DATA, f))
            e.create_geometry()
            d = {"name": os.path.basename(f), "tools": deepcopy(e.tools)}
        else:
            g = pg.Gerber(steps_per_circle=64)
            g.parse_file(os.path.join(DATA, f))
            d = {"name": os.path.basename(f), "apertures": deepcopy(g.apertures)}
        cache[f] = d
        return d
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_rules.py - no editar a mano.",
           "// Referencia: las comprobaciones de appTools/ToolRulesCheck.py originales.", "#pragma once",
           "#include <string>", "#include <vector>", "namespace golden_rules {",
           "struct Case { const char* rule; std::vector<std::string> files; double size; std::vector<std::string> names;"
           " const char* failed; std::vector<double> points; std::vector<int> shapelyPoints; std::vector<double> dias; };",
           "inline const std::vector<Case> kCases = {"]
    for rule, files, size in CASES:
        objs = [obj(f) for f in files]
        if rule == "traces":
            title, v = C.check_traces_size(objs, size)
        elif rule == "inside":
            title, v = C.check_inside_gerber_clearance(objs[0], size, "x")
        elif rule == "gerber":
            title, v = C.check_gerber_clearance(objs, size, "x")
        elif rule == "holes_size":
            title, v = C.check_holes_size(objs, size)
        elif rule == "holes_clearance":
            title, v = C.check_holes_clearance(objs, size)
        else:
            title, v = C.check_gerber_annular_ring(objs, size, "x")
        r = v[0]
        names = r["name"] if isinstance(r["name"], list) else [r["name"]]
        failed = ""
        pts, shp = [], []
        for p in r.get("points", []):
            if isinstance(p, str):
                failed = p
            elif hasattr(p, "x"):
                pts += [p.x, p.y]
                shp.append(1)
            else:
                pts += [float(p[0]), float(p[1])]
                shp.append(0)
        dias = r.get("dia", [])
        out.append("{\"%s\", {%s}, %r, {%s}, %s, {%s}, {%s}, {%s}}," % (
            rule, ", ".join(cs(f) for f in files), size, ", ".join(cs(n) for n in names), cs(failed),
            ", ".join(repr(x) for x in pts), ", ".join(map(str, shp)), ", ".join(repr(x) for x in dias)))
        print(rule, files, size, failed, len(pts) // 2, sum(shp), dias)
    out += ["};", "}  // namespace golden_rules"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
