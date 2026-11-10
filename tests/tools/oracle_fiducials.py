#!/usr/bin/env python3
"""Oráculo de Fiduciales: ejecuta `ToolFiducials.add_fiducials`, `add_fiducials_geo` y `add_soldermask_opening`
ORIGINALES (tomados con `ast`) sobre el cobre de "luz de noche" y escribe `tests/golden_fiducials.h`: los puntos, la
geometría sólida (cantidad y área), las aperturas (con los elementos agregados) y la geometría de seguimiento agregada.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_fiducials.py
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

OUT = os.path.join(ROOT, "tests", "golden_fiducials.h")
GBR = "luz_de_noche/luz de noche Pascual-B_Cu.gbr"
# (operación, tipo, posición del segundo, tamaño, margen, grosor de línea, puntos a mano)
CASES = [
    ("auto", "circular", "up", 1.0, 1.0, 0.25, None),
    ("auto", "circular", "down", 2.0, 1.0, 0.25, None),     # ya hay una apertura C de 2,0
    ("auto", "circular", "no", 1.0, 0.0, 0.25, None),
    ("auto", "circular", "up", 1.0, -2.0, 0.25, None),
    ("auto", "cross", "up", 1.0, 1.0, 0.25, None),
    ("auto", "cross", "down", 1.5, 1.0, 0.4, None),          # ya hay una apertura C de 0,4
    ("auto", "chess", "up", 1.0, 1.0, 0.25, None),
    ("auto", "chess", "no", 2.0, 1.0, 0.25, None),           # ya hay una R de 2 x 2 (diagonal igual)
    ("geo", "cross", "no", 1.0, 1.0, 0.25, [(0.0, 0.0), (0.1, 0.0)]),   # cruces superpuestas: falla
    ("geo", "chess", "no", 1.0, 1.0, 0.25, [(1.23456, -2.5), (3.0, 4.0)]),
    ("sm", "circular", "up", 1.0, 1.0, 0.25, None),          # agujero en la máscara (tamaño x 2)
]


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v

    def set_value(self, v):
        self.v = v


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.log = logging.getLogger("oracle")
    app.plotcanvas = mock.MagicMock()
    app.is_legacy = False
    pg.Gerber.app = app
    from shapely import wkt as W
    from shapely.geometry import LineString, MultiPolygon, Point, Polygon, box
    from shapely.ops import unary_union
    path = os.path.join(REF, "appTools", "ToolFiducials.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolFiducials")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef)
             and f.name in ("add_fiducials", "add_fiducials_geo", "add_soldermask_opening")]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(LineString=LineString, MultiPolygon=MultiPolygon, Point=Point, Polygon=Polygon, box=box, math=math,
              unary_union=unary_union, deepcopy=deepcopy, _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    created = []

    def new_object(kind, name, init, plot=True):
        o = pg.Gerber(steps_per_circle=64)
        o.options = {}
        init(o, app)
        created.append((name, o))
        return o
    app.app_obj = mock.MagicMock()
    app.app_obj.new_object = new_object
    app.f_handlers = mock.MagicMock()
    app.f_handlers.export_gerber.return_value = ""
    d = lambda x: W.dumps(x, rounding_precision=17, trim=True)  # noqa: E731
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_fiducials.py - no editar a mano.",
           "// Referencia: ToolFiducials.add_fiducials / add_fiducials_geo / add_soldermask_opening originales.",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_fiducials {",
           "struct ApOut { const char* id; const char* type; double size; double width; double height; int count;"
           " std::vector<std::string> solid; std::vector<std::string> follow; };",
           "struct FidCase { const char* op; const char* type; const char* second; double size; double margin;"
           " double line; std::vector<double> manual; bool ok; std::vector<double> points; std::vector<std::string> texts;"
           " const char* name; int polygons; double area; std::vector<ApOut> apertures;"
           " std::vector<std::string> followAdded; };",
           "inline const std::vector<FidCase> kCases = {"]
    for op, typ, sec, size, margin, line, manual in CASES:
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, GBR))
        g.options = {"name": "cu"}
        g.follow = False
        orig = {k: len(v.get("geometry", [])) for k, v in g.apertures.items()}
        nfollow = len(g.follow_geometry)
        idx = mock.MagicMock()
        idx.internalPointer.return_value.obj = g
        app.collection = mock.MagicMock()
        app.collection.index.return_value = idx
        core = ns["Core"]()
        core.app = app
        core.decimals = 4
        core.grb_steps_per_circle = 64
        core.copper_obj_set, core.sm_obj_set = set(), set()
        core.on_exit = core.disconnect_event_handlers = core.connect_event_handlers = lambda *a, **k: None
        core.ui = mock.MagicMock()
        for k, v in dict(mode_radio="auto", margin_entry=margin, pos_radio=sec, fid_type_radio=typ,
                         fid_size_entry=size, line_thickness_entry=line, bottom_left_coords_entry="",
                         top_right_coords_entry="", sec_points_coords_entry="").items():
            setattr(core.ui, k, Val(v))
        created.clear()
        ok = True
        try:
            if op == "geo":
                core.click_points = list(manual)
                core.add_fiducials_geo(core.click_points, g_obj=g, fid_type=typ)
            else:
                core.add_fiducials()
                if op == "sm":
                    created.clear()
                    core.add_soldermask_opening()
        except TypeError:
            ok = False
        pts = [v for p in core.click_points for v in p]
        texts = [core.ui.bottom_left_coords_entry.v, core.ui.top_right_coords_entry.v, core.ui.sec_points_coords_entry.v]
        name, npoly, area, aps, fol = "", 0, 0.0, [], []
        if ok and created:
            name, o = created[-1]
            sg = o.solid_geometry
            parts = list(sg.geoms) if hasattr(sg, "geoms") else [sg]
            npoly, area = len(parts), sum(p.area for p in parts)
            for ap, v in o.apertures.items():
                geo = v.get("geometry", [])
                added = geo[orig.get(ap, 0):]
                aps.append((ap, v["type"], float(v.get("size", -1)), float(v.get("width", -1)),
                            float(v.get("height", -1)), len(geo), [d(e["solid"]) for e in added],
                            [d(e["follow"]) for e in added]))
            flat = []
            for x in o.follow_geometry[nfollow:]:
                flat += x if isinstance(x, list) else [x]
            fol = [d(x) for x in flat]
        ap_txt = ", ".join("{%s, %s, %r, %r, %r, %d, {%s}, {%s}}" % (
            cs(a), cs(t), s, w, h, n, ", ".join(cs(x) for x in so), ", ".join(cs(x) for x in fo))
            for a, t, s, w, h, n, so, fo in aps)
        out.append("{\"%s\", \"%s\", \"%s\", %r, %r, %r, {%s}, %s, {%s}, {%s}, %s, %d, %r, {%s}, {%s}}," % (
            op, typ, sec, size, margin, line, ", ".join(repr(v) for p in (manual or []) for v in p),
            "true" if ok else "false", ", ".join(repr(float(v)) for v in pts), ", ".join(cs(str(t)) for t in texts),
            cs(name), npoly, area, ap_txt, ", ".join(cs(x) for x in fol)))
        print(op, typ, sec, size, margin, ok, name, npoly, round(area, 4), pts, texts,
              [(a[0], a[5], len(a[6])) for a in aps if a[6]])
    out += ["};", "}  // namespace golden_fiducials"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
