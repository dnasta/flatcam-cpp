#!/usr/bin/env python3
"""Oráculo de Marcas de esquina: ejecuta `ToolCorners.add_markers` / `add_corners_geo` / `on_create_drill_object`
ORIGINALES (tomados con `ast`) con una interfaz simulada sobre Gerber leídos con el parser original, y escribe en
`tests/golden_corners.h` el Gerber nuevo (geometría sólida, de seguimiento agregada y aperturas) o los taladros.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_corners.py
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

OUT = os.path.join(ROOT, "tests", "golden_corners.h")
EDGE = "luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr"
EDGE2 = "PCB principal-Edge_Cuts.gm1"
# (archivo, operación, tl, tr, bl, br, tipo, grosor, largo, margen, diámetro del taladro)
CASES = [
    (EDGE, "markers", True, True, True, True, "s", 0.1, 3.0, 0.0, 0.5),    # apertura nueva (bisel)
    (EDGE, "markers", True, False, True, False, "c", 0.05, 4.0, 1.0, 0.5),  # apertura existente (inglete)
    (EDGE2, "markers", False, True, False, True, "s", 0.2, 2.0, 0.5, 0.5),
    (EDGE, "markers", False, False, False, False, "s", 0.1, 3.0, 0.0, 0.5),  # sin esquinas: falla
    (EDGE, "drills", True, True, True, True, "s", 0.1, 3.0, 1.0, 0.8),
    (EDGE2, "drills", True, False, False, True, "s", 0.3, 3.0, 0.0, 1.0),
]


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.log = logging.getLogger("oracle")
    app.defaults.update({"excellon_circle_steps": 64})
    pg.Gerber.app = app
    pe.Excellon.app = app
    from shapely import wkt as W
    from shapely.geometry import LineString, MultiPolygon, Point
    from shapely.ops import unary_union
    path = os.path.join(REF, "appTools", "ToolCorners.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolCorners")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef)
             and f.name in ("add_markers", "add_corners_geo", "on_create_drill_object")]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(LineString=LineString, MultiPolygon=MultiPolygon, Point=Point, unary_union=unary_union,
              deepcopy=deepcopy, _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    messages = []
    app.inform.emit = lambda s: messages.append(s)
    created = []

    def new_object(kind, name, init, plot=True):
        o = pg.Gerber(steps_per_circle=64) if kind == "gerber" else pe.Excellon(geo_steps_per_circle=64)
        o.default_data = {}
        o.options = {}
        init(o, app)
        created.append((kind, name, o))
        return o
    app.app_obj = mock.MagicMock()
    app.app_obj.new_object = new_object
    app.f_handlers = mock.MagicMock()
    app.f_handlers.export_gerber.return_value = ""
    app.f_handlers.export_excellon.return_value = ""
    d = lambda x: W.dumps(x, rounding_precision=17, trim=True)  # noqa: E731
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_corners.py - no editar a mano.",
           "// Referencia: ToolCorners.add_markers / add_corners_geo / on_create_drill_object originales.",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_corners {",
           "struct ApOut { const char* id; const char* type; double size; std::vector<std::string> solid;"
           " std::vector<std::string> follow; };",
           "struct CornersCase { const char* file; const char* op; bool tl; bool tr; bool bl; bool br; const char* type;"
           " double thickness; double length; double margin; double drillDia; bool ok; const char* name;"
           " std::vector<std::string> solid; std::vector<std::string> followAdded; std::vector<ApOut> apertures;"
           " std::vector<std::string> drills; };",
           "inline const std::vector<CornersCase> kCases = {"]
    for f, op, tl, tr, bl, br, typ, th, ln, mg, dd in CASES:
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        g.options = {"name": os.path.basename(f)}
        g.follow = False
        nfollow = len(g.follow_geometry)
        idx = mock.MagicMock()
        idx.internalPointer.return_value.obj = g
        app.collection = mock.MagicMock()
        app.collection.index.return_value = idx
        core = ns["Core"]()
        core.app = app
        core.grb_steps_per_circle = 64
        core.on_exit = lambda *a, **k: None
        core.ui = mock.MagicMock()
        for k, v in dict(tl_cb=tl, tr_cb=tr, bl_cb=bl, br_cb=br, type_radio=typ, thick_entry=th, l_entry=ln,
                         margin_entry=mg, drill_dia_entry=dd).items():
            setattr(core.ui, k, Val(v))
        created.clear()
        messages.clear()
        if op == "markers":
            core.add_markers()
        else:
            core.on_create_drill_object()
        ok = bool(created)
        name, solid, follow, aps, drills = "", [], [], [], []
        if created:
            kind, name, o = created[0]
            if kind == "gerber":
                sg = o.solid_geometry
                solid = [d(p) for p in (sg.geoms if hasattr(sg, "geoms") else [sg])]
                follow = [d(x) for x in o.follow_geometry[nfollow:]]
                for ap, v in o.apertures.items():
                    aps.append((ap, v["type"], v.get("size", 0.0) or 0.0, [d(e["solid"]) for e in v.get("geometry", [])
                                                                          if "solid" in e],
                                [d(e["follow"]) for e in v.get("geometry", []) if "follow" in e]))
            else:
                drills = [d(p) for p in o.tools[1]["drills"]]
        ap_txt = ", ".join("{%s, %s, %r, {%s}, {%s}}" % (cs(a), cs(t), float(s), ", ".join(cs(x) for x in so),
                                                         ", ".join(cs(x) for x in fo)) for a, t, s, so, fo in aps)
        out.append("{%s, \"%s\", %s, %s, %s, %s, \"%s\", %r, %r, %r, %r, %s, %s, {%s}, {%s}, {%s}, {%s}}," % (
            cs(f), op, *["true" if x else "false" for x in (tl, tr, bl, br)], typ, th, ln, mg, dd,
            "true" if ok else "false", cs(name), ", ".join(cs(x) for x in solid), ", ".join(cs(x) for x in follow),
            ap_txt, ", ".join(cs(x) for x in drills)))
        print(f, op, typ, th, ok, name, len(solid), len(follow), [a[0] for a in aps], len(drills), messages[-1:])
    out += ["};", "}  // namespace golden_corners"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
