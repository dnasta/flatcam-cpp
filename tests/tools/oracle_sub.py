#!/usr/bin/env python3
"""Oráculo de Restar: ejecuta `ToolSub.aperture_intersection` y `new_gerber_object` (Gerber menos Gerber) y
`toolgeo_intersection` (Geometría menos Geometría) ORIGINALES, tomados con `ast`, y escribe en `tests/golden_sub.h` el
resultado resumido: por apertura, la cantidad de elementos y sus áreas; la geometría sólida final (cantidad y áreas); y
las figuras de la resta de geometrías (WKT).

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_sub.py
"""
import ast
import logging
import os
import sys
from copy import deepcopy
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_sub.h")
GERBER_CASES = [("PCB principal-B_Cu.gbl", "PCB principal-B_Mask.gbs"),
                ("luz_de_noche/luz de noche Pascual-B_Cu.gbr", "luz_de_noche/luz de noche Pascual-Logo.gbr"),
                ("PCB principal-B_Mask.gbs", "PCB principal-B_Cu.gbl")]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.log = logging.getLogger("oracle")
    pg.Gerber.app = app
    from shapely import wkt as W
    from shapely.geometry import LineString, MultiLineString, MultiPolygon, Polygon
    from shapely.ops import unary_union
    path = os.path.join(REF, "appTools", "ToolSub.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolSub")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef)
             and f.name in ("aperture_intersection", "new_gerber_object", "toolgeo_intersection", "poly2rings")]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(unary_union=unary_union, MultiPolygon=MultiPolygon, Polygon=Polygon, LineString=LineString,
              MultiLineString=MultiLineString, deepcopy=deepcopy, _=lambda s: s, log=logging.getLogger("x"),
              time=mock.MagicMock())
    exec(compile(m, path, "exec"), ns)
    Core = ns["Core"]
    created = []

    def new_object(kind, name, init, autoselected=False):
        o = mock.MagicMock()
        init(o, app)
        created.append(o)
    app.app_obj = mock.MagicMock()
    app.app_obj.new_object = new_object
    app.f_handlers = mock.MagicMock()
    app.f_handlers.export_gerber.return_value = ""
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_sub.py - no editar a mano.",
           "// Referencia: ToolSub.aperture_intersection / new_gerber_object / toolgeo_intersection originales.",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_sub {",
           "struct ApSummary { const char* id; std::vector<double> solidAreas; };",
           "struct GerberCase { const char* target; const char* sub; std::vector<ApSummary> apertures; int polygons;"
           " std::vector<double> areas; };",
           "inline const std::vector<GerberCase> kGerber = {"]
    for tf, sf in GERBER_CASES:
        t = pg.Gerber(steps_per_circle=64)
        t.parse_file(os.path.join(DATA, tf))
        s = pg.Gerber(steps_per_circle=64)
        s.parse_file(os.path.join(DATA, sf))
        core = Core()
        core.app = app
        core.ui = mock.MagicMock()
        core.ui.delete_sources_cb.get_value.return_value = False
        core.new_apertures = {}
        core.new_solid_geometry = []
        for apid in t.apertures:
            core.new_apertures[apid] = {}
            for key in t.apertures[apid]:
                core.new_apertures[apid][key] = [] if key == "geometry" else t.apertures[apid][key]
        sub_geometry = {"solid": [], "clear": []}
        for s_apid in s.apertures:
            for s_el in s.apertures[s_apid]["geometry"]:
                if "solid" in s_el:
                    sub_geometry["solid"].append(s_el["solid"])
                if "clear" in s_el:
                    sub_geometry["clear"].append(s_el["clear"])
        output = []
        for ap_id in t.apertures:
            target_geo = deepcopy([geo for geo in t.apertures[ap_id]["geometry"]])  # apply_async: copias
            output.append(Core.aperture_intersection(ap_id, target_geo, sub_geometry))
        created.clear()
        core.new_gerber_object(os.path.basename(tf) + "_sub", output)
        o = created[0]
        aps = []
        for ap, v in o.apertures.items():
            aps.append((ap, [round(e["solid"].area, 9) if "solid" in e else -1.0 for e in v["geometry"]]))
        sg = o.solid_geometry
        parts = list(sg.geoms) if hasattr(sg, "geoms") else [sg]
        areas = sorted(round(p.area, 9) for p in parts)
        ap_txt = ", ".join("{%s, {%s}}" % (cs(a), ", ".join(repr(x) for x in v)) for a, v in aps)
        out.append("{%s, %s, {%s}, %d, {%s}}," % (cs(tf), cs(sf), ap_txt, len(parts), ", ".join(repr(a) for a in areas)))
        print(tf, sf, [(a, len(v)) for a, v in aps], len(parts), round(sum(areas), 4))
    out += ["};"]
    # Geometría menos Geometría (toolgeo_intersection), con y sin "Cerrar trayectos".
    sub = [Polygon([(2, 2), (6, 2), (6, 6), (2, 6)]), Polygon([(7, 7), (9, 7), (9, 12), (7, 12)])]
    geos = [Polygon([(0, 0), (8, 0), (8, 8), (0, 8)], [[(3, 3), (5, 3), (5, 5), (3, 5)]]),
            LineString([(-1, 4), (12, 4)]), MultiLineString([[(1, 9), (10, 9)], [(8, 1), (8, 11)]]),
            MultiPolygon([Polygon([(10, 0), (12, 0), (12, 2), (10, 2)])])]
    out += ["struct GeoCase { bool close; std::vector<std::string> result; };", "inline const std::vector<GeoCase> kGeo = {"]
    for close in (False, True):
        core = Core()
        core.app = app
        core.ui = mock.MagicMock()
        core.ui.close_paths_cb.isChecked.return_value = close
        core.sub_union = unary_union(sub)
        core.new_solid_geometry = []
        core.promises = []
        core.toolgeo_intersection("single", geos)
        res = [W.dumps(g, rounding_precision=17, trim=True) for g in core.new_solid_geometry]
        out.append("{%s, {%s}}," % ("true" if close else "false", ", ".join(cs(r) for r in res)))
        print("geo close", close, len(res))
    out += ["};", "}  // namespace golden_sub"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
