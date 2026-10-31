#!/usr/bin/env python3
"""Oráculo del aislamiento: ejecuta el código ORIGINAL de ToolIsolation sobre casos sintéticos y sobre `B_Cu`.

No transcribe nada: toma del fuente original (con `ast`) los métodos `isolate`, `combined_normal`, `combined_rest`,
`generate_envelope`, `generate_rest_geometry`, `area_subtraction`, `area_intersection` y `poly2rings`, y los ejecuta
con una interfaz simulada (tabla de herramientas, casillas y `new_object`). `isolation_geometry` es el de `camlib`.
Genera `tests/golden_isolation.h`.

Entorno:  micromamba create -n old -c conda-forge python=3.10 shapely=1.8.4 numpy=1.22 rtree
Uso:      micromamba run -n old python tests/tools/oracle_isolation.py
"""
import ast
import logging
import math
import os
import sys
from copy import deepcopy
from unittest import mock

from shapely.geometry import (LinearRing, LineString, MultiLineString, MultiPoint, MultiPolygon, Point, Polygon,
                              box)
from shapely.ops import unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs, num  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_isolation.h")
METHODS = {"isolate", "combined_normal", "combined_rest", "generate_envelope", "generate_rest_geometry",
           "area_subtraction", "area_intersection", "poly2rings"}


def load_iso_core():
    """Clase con los métodos originales de ToolIsolation (el resto de la clase depende de Qt)."""
    path = os.path.join(REF, "appTools", "ToolIsolation.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolIsolation")
    body = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in METHODS]
    assert {f.name for f in body} == METHODS
    module = ast.Module(body=[ast.ClassDef(name="IsoCore", bases=[], keywords=[], body=body, decorator_list=[])],
                        type_ignores=[])
    ast.fix_missing_locations(module)
    ns = dict(Polygon=Polygon, MultiPolygon=MultiPolygon, LinearRing=LinearRing, LineString=LineString,
              MultiLineString=MultiLineString, Point=Point, unary_union=unary_union, deepcopy=deepcopy,
              log=logging.getLogger("iso"), _=lambda s: s)
    exec(compile(module, path, "exec"), ns)
    return ns["IsoCore"]


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v

    def currentText(self):
        return self.v


class Cell:
    def __init__(self, r, t):
        self.r, self.t = r, t

    def row(self):
        return self.r

    def text(self):
        return self.t


class Table:
    def __init__(self, tools):
        self.tools = tools  # [(id, dia)]

    def selectedItems(self):
        return [Cell(r, "") for r in range(len(self.tools))]

    def item(self, row, col):
        tid, dia = self.tools[row]
        return Cell(row, str(tid) if col == 3 else "%.4f" % dia)


def run(IsoCore, camlib, copper, follow, tools, combine=True, rest=False, forced=True, order="rev", except_geo=None,
        limited=None, geometry=None, negative=None):
    """Ejecuta `isolate` original; devuelve los objetos creados: (nombre, [(id herramienta, dia, geometría)])."""
    app = FakeApp()
    app.defaults["tools_iso_plotting"] = "normal"
    app.inform_shell = mock.MagicMock()
    app.ui = mock.MagicMock()
    created = []

    def new_object(kind, name, init, plot=True):
        geo = camlib.Geometry(geo_steps_per_circle=64)
        geo.options = {"name": name}
        geo.multigeo = False
        if init(geo, app) == "fail":
            return "fail"
        created.append((name, [(tid, t["tooldia"], list(t["solid_geometry"])) for tid, t in geo.tools.items()]))

    app.app_obj = mock.MagicMock()
    app.app_obj.new_object = new_object
    exc_obj = mock.MagicMock()
    exc_obj.solid_geometry = except_geo
    app.collection = mock.MagicMock()
    app.collection.get_by_name = lambda n: exc_obj
    camlib.Geometry.app = app

    grb = camlib.Geometry(geo_steps_per_circle=64)
    grb.solid_geometry = copper
    grb.follow_geometry = follow
    grb.options = {"name": "PCB"}

    iso = IsoCore()
    iso.app, iso.decimals, iso.grb_obj = app, 4, grb
    iso.temp_shapes, iso.plot_temp_shapes = mock.MagicMock(), mock.MagicMock()
    iso.iso_tools = {}
    for i, t in enumerate(tools, start=1):
        iso.iso_tools[i] = {"tooldia": t["dia"], "offset": "Path", "offset_value": 0.0, "type": "Rough",
                            "tool_type": "C1", "data": {
                                "tools_iso_passes": t.get("passes", 1), "tools_iso_overlap": t.get("overlap", 10),
                                "tools_iso_milling_type": t.get("mill", "cl"), "tools_iso_follow": t.get("follow", False),
                                "tools_iso_isotype": t.get("type", "full")}}
    ui = mock.MagicMock()
    ui.combine_passes_cb, ui.rest_cb, ui.except_cb = Val(combine), Val(rest), Val(except_geo is not None)
    ui.select_combo, ui.area_shape_radio, ui.forced_rest_iso_cb = Val(0), Val("square"), Val(forced)
    ui.order_radio, ui.exc_obj_combo = Val(order), Val("exc")
    ui.tools_table = Table([(i, t["dia"]) for i, t in enumerate(tools, start=1)])
    iso.ui = ui
    iso.isolate(isolated_obj=grb, geometry=geometry, limited_area=limited, negative_dia=negative, plot=False)
    return created


def rect(x0, y0, x1, y1):
    return Polygon([(x0, y0), (x1, y0), (x1, y1), (x0, y1)])


PADS = [rect(2, 2, 4, 3), rect(6, 5, 9, 6.5), Point(5, 2.5).buffer(0.8, 16)]
ISLAND = [Polygon([(2, 2), (10, 2), (10, 8), (2, 8)], [[(4, 4), (8, 4), (8, 6), (4, 6)]]), rect(5, 4.8, 7, 5.2)]
# El segundo agujero mide 0,3 mm: el buffer de la fresa de 0,4 lo cierra y el de la de 0,1 no.
HOLES2 = [Polygon([(0, 0), (10, 0), (10, 4), (0, 4)], [[(1, 1), (4, 1), (4, 3), (1, 3)], [(5, 1), (5.3, 1), (5.3, 3),
                                                                                         (5, 3)]]), rect(12, 0, 14, 4)]
# Pistas paralelas a 0,5 mm: la fresa de 0,4 no entra entre las dos primeras (0,3 mm), la de 0,1 sí.
TRACKS = [rect(1, 1, 1.4, 9), rect(1.7, 1, 2.1, 9), rect(4, 1, 4.4, 9), rect(7, 1, 7.4, 9)]
FOLLOW = [LineString([(1.2, 1), (1.2, 9)]), LineString([(4.2, 1), (4.2, 9), (7.2, 9)]), Point(5, 5)]
DRILLS = [Point(3, 2.5).buffer(0.4, 16), Point(7.5, 5.75).buffer(0.5, 16)]

T = lambda dia, **k: dict(dia=dia, **k)  # noqa: E731

# nombre, cobre, seguimiento, herramientas, opciones de `run`
SCENARIOS = [
    ("pads_full_climb", PADS, [], [T(0.2)], {}),
    ("pads_full_conventional", PADS, [], [T(0.2, mill="cv")], {}),
    ("pads_exterior_climb", PADS, [], [T(0.2, type="ext")], {}),
    ("pads_exterior_conventional", PADS, [], [T(0.2, type="ext", mill="cv")], {}),
    ("island_interior_climb", ISLAND, [], [T(0.3, type="int")], {}),
    ("island_interior_conventional", ISLAND, [], [T(0.3, type="int", mill="cv")], {}),
    ("island_full_three_passes", ISLAND, [], [T(0.3, passes=3, overlap=20)], {}),
    ("pads_separate_passes", PADS, [], [T(0.2, passes=2, overlap=10)], dict(combine=False)),
    ("pads_separate_two_tools", PADS, [], [T(0.2), T(0.5, type="ext", mill="cv")], dict(combine=False)),
    ("tracks_two_tools_combined", TRACKS, [], [T(0.1), T(0.4, passes=2)], {}),
    ("pads_except_drills", PADS, [], [T(0.2, passes=2)], dict(except_geo=DRILLS)),
    ("pads_except_separate", PADS, [], [T(0.2)], dict(combine=False, except_geo=DRILLS)),
    ("pads_limited_area", PADS, [], [T(0.2)], dict(limited=unary_union([box(1, 1, 5, 4), box(7, 4.5, 8, 7)]))),
    ("pads_except_and_limited", PADS, [], [T(0.2)], dict(except_geo=DRILLS, limited=box(1, 1, 5, 4))),
    ("tracks_follow", TRACKS, FOLLOW, [T(0.2, follow=True, passes=2)], {}),
    ("tracks_follow_except", TRACKS, FOLLOW, [T(0.2, follow=True)], dict(except_geo=[box(0, 4, 8, 5)])),
    ("tracks_rest_reverse", TRACKS, [], [T(0.1), T(0.4)], dict(rest=True)),
    ("tracks_rest_forward", TRACKS, [], [T(0.1), T(0.4)], dict(rest=True, order="fwd")),
    ("tracks_rest_table_order", TRACKS, [], [T(0.4, passes=2, overlap=30), T(0.1, mill="cv")],
     dict(rest=True, order="no")),
    ("tracks_rest_unisolable", TRACKS, [], [T(0.4)], dict(rest=True)),
    ("holes2_rest_forced", HOLES2, [], [T(0.4), T(0.1)], dict(rest=True, forced=True)),
    ("holes2_rest_not_forced", HOLES2, [], [T(0.4), T(0.1)], dict(rest=True, forced=False)),
    ("holes2_rest_interior", HOLES2, [], [T(0.1, type="int")], dict(rest=True)),
    ("polygon_selection", ISLAND, [], [T(0.2)], dict(geometry=[ISLAND[1]])),
    ("negative_interior", ISLAND, [], [T(0.2)], dict(geometry=[Polygon(ISLAND[0].interiors[0])], negative=True)),
    ("reference_object", PADS, [], [T(0.2)],
     dict(geometry=unary_union(PADS).difference(box(5, 4, 10, 8)))),
    ("reference_object_rest", PADS, [], [T(0.2)], dict(rest=True, geometry=unary_union(PADS).difference(box(5, 4, 10, 8)))),
]


def wkt(g):
    def seq(cs_):
        return "(" + ", ".join("%r %r" % (float(x), float(y)) for x, y in cs_) + ")"
    t = g.geom_type
    if t == "Point":
        return "POINT (%r %r)" % (g.x, g.y)
    if t in ("LineString", "LinearRing"):
        return "%s %s" % (t.upper(), seq(g.coords))
    if t == "Polygon":
        return "POLYGON (%s)" % ", ".join(seq(r.coords) for r in [g.exterior] + list(g.interiors))
    if t == "MultiLineString":
        return "MULTILINESTRING (%s)" % ", ".join(seq(l.coords) for l in g.geoms)
    if t == "MultiPoint":
        return "MULTIPOINT (%s)" % ", ".join("(%r %r)" % (p.x, p.y) for p in g.geoms)
    raise ValueError(t)


def cpp_shapes(gs):
    return "{" + ", ".join('flatcam::Shape::fromWkt("%s")' % wkt(g) for g in gs) + "}"


def cpp_list(gs):
    if isinstance(gs, (Polygon, MultiPolygon)):
        gs = list(gs.geoms) if isinstance(gs, MultiPolygon) else [gs]
    return cpp_shapes(gs)


def tool_cpp(t):
    return "{%s, %d, %s, %s, %s, %s}" % (
        num(t["dia"]), t.get("passes", 1), num(t.get("overlap", 10) / 100.0),
        "flatcam::MillingDirection::Conventional" if t.get("mill", "cl") == "cv" else "flatcam::MillingDirection::Climb",
        {"ext": "flatcam::IsolationType::Exterior", "int": "flatcam::IsolationType::Interior",
         "full": "flatcam::IsolationType::Full"}[t.get("type", "full")], str(t.get("follow", False)).lower())


def stats(gs):
    """Cantidad de figuras, de puntos y longitud total (para el caso grande)."""
    pts, length = 0, 0.0
    for g in gs:
        lines = []
        if g.geom_type == "Polygon":
            lines = [g.exterior] + list(g.interiors)
        elif g.geom_type.startswith("Multi"):
            lines = list(g.geoms)
        else:
            lines = [g]
        for l in lines:
            pts += len(l.coords)
            length += l.length
    return len(gs), pts, length


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    IsoCore = load_iso_core()

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_isolation.py - no editar a mano.",
           "// Referencia: ToolIsolation.isolate / combined_normal / combined_rest originales (FlatCAM 2024.4), Shapely %s." %
           __import__("shapely").__version__,
           "#pragma once", "#include <string>", "#include <vector>", '#include "tools/Isolation.h"',
           "namespace golden_isolation {",
           "struct Object { std::string name; std::vector<std::size_t> toolIndex; std::vector<flatcam::ShapeList> geometry; };"]
    entries = []
    for idx, (name, copper, follow, tools, opts) in enumerate(SCENARIOS):
        created = run(IsoCore, camlib, copper, follow, tools, **opts)
        print("%-30s objetos=%d figuras=%s" % (name, len(created),
                                               [sum(len(g) for _, _, g in t) for _, t in created]), flush=True)
        out.append("inline flatcam::ShapeList copper%d() { return %s; }  // %s" % (idx, cpp_list(copper), name))
        out.append("inline flatcam::ShapeList follow%d() { return %s; }" % (idx, cpp_shapes(follow)))
        for key in ("except_geo", "limited", "geometry"):
            if opts.get(key) is not None:
                out.append("inline flatcam::ShapeList %s%d() { return %s; }" % (key, idx, cpp_list(opts[key])))
        objs = []
        # Con pasadas separadas el objeto guarda la herramienta con la clave '1': se identifica por el diámetro.
        tindex = lambda tid, dia: tid - 1 if isinstance(tid, int) else [t["dia"] for t in tools].index(dia)  # noqa: E731
        for oname, per_tool in created:
            objs.append("{%s, {%s}, {%s}}" % (cs(oname), ", ".join(str(tindex(tid, d)) for tid, d, _ in per_tool),
                                              ", ".join(cpp_shapes(g) for _, _, g in per_tool)))
        out.append("inline std::vector<Object> objects%d() { return {%s}; }" % (idx, ",\n    ".join(objs)))
        entries.append((idx, name, tools, opts))

    out.append("struct Scenario { const char* name; flatcam::ShapeList (*copper)(); flatcam::ShapeList (*follow)(); "
               "std::vector<flatcam::IsolationTool> tools; bool combine; bool rest; bool forced; "
               "flatcam::IsolationToolOrder order; flatcam::ShapeList (*exceptGeo)(); flatcam::ShapeList (*limited)(); "
               "flatcam::ShapeList (*geometry)(); bool negative; std::vector<Object> (*objects)(); };")
    out.append("inline const std::vector<Scenario> kScenarios = {")
    for idx, name, tools, o in entries:
        order = {"rev": "Reverse", "fwd": "Forward", "no": "Default"}[o.get("order", "rev")]
        fn = lambda key: ("%s%d" % (key, idx)) if o.get(key) is not None else "nullptr"  # noqa: E731
        out.append("{%s, copper%d, follow%d, {%s}, %s, %s, %s, flatcam::IsolationToolOrder::%s, %s, %s, %s, %s, objects%d}," % (
            cs(name), idx, idx, ", ".join(tool_cpp(t) for t in tools), str(o.get("combine", True)).lower(),
            str(o.get("rest", False)).lower(), str(o.get("forced", True)).lower(), order, fn("except_geo"),
            fn("limited"), fn("geometry"), str(bool(o.get("negative"))).lower(), idx))
    out.append("};")

    # Caso real: el cobre inferior del usuario con el parser original y los valores por defecto de la herramienta.
    gerber_mod = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    gerber_mod.Gerber.app = app
    camlib.Geometry.app = app
    g = gerber_mod.Gerber(steps_per_circle=64)
    g.parse_file(os.path.join(DATA, "PCB principal-B_Cu.gbl"))
    copper = g.solid_geometry
    real = []
    for label, tools, opts in (("default_0.1", [T(0.1)], {}),
                               ("rest_0.4_0.2_0.1", [T(0.1), T(0.2), T(0.4)], dict(rest=True))):
        created = run(IsoCore, camlib, copper, g.follow_geometry, tools, **opts)
        per_tool = created[0][1] if created else []
        summary = [(tid - 1,) + stats(gs) for tid, _, gs in per_tool]  # combinado: claves enteras
        print("B_Cu %-18s %s" % (label, summary), flush=True)
        real.append((label, tools, opts, summary))
    out.append("struct RealTool { std::size_t toolIndex; std::size_t shapes; std::size_t points; double length; };")
    out.append("struct RealCase { const char* name; std::vector<flatcam::IsolationTool> tools; bool rest; "
               "std::vector<RealTool> result; };")
    out.append("inline const std::vector<RealCase> kBottomCopper = {")
    for label, tools, opts, summary in real:
        out.append("{%s, {%s}, %s, {%s}}," % (cs(label), ", ".join(tool_cpp(t) for t in tools),
                                              str(opts.get("rest", False)).lower(),
                                              ", ".join("{%d, %d, %d, %s}" % (i, n, p, num(l)) for i, n, p, l in summary)))
    out.append("};")
    out.append("}  // namespace golden_isolation")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.1f MB)" % (os.path.getsize(OUT) / 1e6))


if __name__ == "__main__":
    main()
