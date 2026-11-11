#!/usr/bin/env python3
"""Oráculo de Robo de cobre: ejecuta `on_add_copper_thieving_click`, `copper_thieving`, `on_mouse_release` (los
rectángulos del modo área), `on_add_robber_bar_click`, `on_new_pattern_plating_object` y `flatten` ORIGINALES
(tomados con `ast`) en secuencia sobre una misma herramienta (el relleno, la barra y la lista aplanada pasan de una
operación a la siguiente, como en el original) y escribe `tests/golden_thieving.h` con cada Gerber creado resumido:
polígonos (cantidad, área, vértices y límites), aperturas en orden (elementos, área, vértices), lo agregado al
relleno, el área galvanizada y la geometría de seguimiento.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_thieving.py
"""
import ast
import logging
import os
import sys
from collections.abc import Iterable
from copy import deepcopy
from unittest import mock

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_thieving.h")
CU = "luz_de_noche/luz de noche Pascual-B_Cu.gbr"
EDGE = "luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr"
LOGO = "luz_de_noche/luz de noche Pascual-Logo.gbr"
MASK = "PCB principal-B_Mask.gbs"
BASE = dict(ref="itself", bbox="rect", fill="solid", clearance=0.25, margin=1.0, area=0.1, dot_dia=1.0,
            dot_sp=2.0, sq=1.0, sq_sp=2.0, line=0.25, line_sp=2.0, ref_kind="", ref_file="", clicks=(),
            rb_margin=1.0, rb_thick=1.0, ppm_clear=0.0, ppm_choice="b", src="cu")
OPS = [
    ("thief", {}),
    ("thief", dict(bbox="min")),
    ("thief", dict(fill="dot")),
    ("thief", dict(fill="square", clearance=0.5, margin=0.0, area=1.0)),
    ("thief", dict(fill="line")),
    ("thief", dict(fill="line", line=0.3, line_sp=3.0)),      # la lista aplanada se acumula
    ("thief", dict(ref="area", clicks=(-30.0, -30.0, 0.0, 0.0, 10.0, 10.0, -10.0, -10.0))),
    ("thief", dict(ref="area", fill="dot", margin=0.5, clicks=(5.0, 5.0, 25.0, 20.0))),
    ("thief", dict(ref="box", ref_kind="gerber", ref_file=EDGE, fill="dot", dot_dia=0.5, dot_sp=1.0)),
    ("thief", dict(ref="box", ref_kind="geometry", ref_file=LOGO, fill="square", margin=2.0)),
    ("thief", dict(ref="box", ref_kind="excellon", ref_file=LOGO)),  # no se admite
    ("thief", dict(fill="square", area=5.0)),
    ("thief", dict(src="logo", fill="dot")),                 # el logo deja mucho espacio libre
    ("thief", dict(src="logo", fill="square", sq=0.8, sq_sp=1.5, area=0.5)),
    ("thief", dict(src="logo", bbox="min", fill="line", line=0.2, line_sp=1.5)),  # se suma a lo aplanado antes
    ("thief", dict(src="logo", bbox="min", clearance=0.1, margin=0.5)),
    ("robber", dict(rb_margin=1.0, rb_thick=1.0)),
    ("robber", dict(rb_margin=0.5, rb_thick=2.0)),           # ya hay una C de 2,0
    ("ppm", dict(ppm_clear=0.0, ppm_choice="b")),            # la barra va a la C de 2,0 de la máscara
    ("ppm", dict(ppm_clear=0.1, ppm_choice="t")),
    ("ppm", dict(ppm_clear=-0.05, ppm_choice="b")),          # un relleno achicado en dos partes: falla
    ("ppm", dict(ppm_clear=0.2, ppm_choice="r")),
    ("thief", dict(src="logo", fill="dot", dot_dia=0.6, dot_sp=0.8)),
    ("ppm", dict(ppm_clear=-0.05, ppm_choice="b")),          # achica también la máscara
]
METHODS = ("on_add_copper_thieving_click", "copper_thieving", "on_mouse_release", "on_add_robber_bar_click",
           "on_new_pattern_plating_object", "flatten")


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v

    def set_value(self, v):
        self.v = v

    def currentText(self):
        return self.v

    def currentIndex(self):
        return 0

    def rootModelIndex(self):
        return None


class Ev:
    def __init__(self, x, y, button):
        self.pos = (x, y)
        self.button = button


def nverts(g):
    if hasattr(g, "geoms"):
        return sum(nverts(x) for x in g.geoms)
    if g.is_empty:
        return 0
    if g.geom_type == "Polygon":
        return len(g.exterior.coords) + sum(len(i.coords) for i in g.interiors)
    return len(g.coords)


def parts_of(sg):
    if isinstance(sg, list):
        out = []
        for x in sg:
            out += parts_of(x)
        return out
    return list(sg.geoms) if hasattr(sg, "geoms") else [sg]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.log = logging.getLogger("oracle")
    app.inform_shell = mock.MagicMock()
    app.plotcanvas = mock.MagicMock()
    app.plotcanvas.translate_coords = lambda p: p
    app.is_legacy = False
    app.grid_status = lambda: False
    app.worker_task = mock.MagicMock()
    app.worker_task.emit = lambda d: d["fcn"](*d["params"])
    app.file_opened = mock.MagicMock()
    app.defaults.update({"tools_copper_thieving_clearance": 0.25, "tools_copper_thieving_margin": 1.0})
    pg.Gerber.app = app
    import shapely.affinity as affinity
    import shapely.geometry.base as base
    from shapely.geometry import LineString, MultiPolygon, Point, Polygon, box
    from shapely.ops import unary_union
    path = os.path.join(REF, "appTools", "ToolCopperThieving.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolCopperThieving")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in METHODS]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(LineString=LineString, MultiPolygon=MultiPolygon, Point=Point, Polygon=Polygon, box=box,
              unary_union=unary_union, deepcopy=deepcopy, affinity=affinity, base=base, np=np, Iterable=Iterable,
              grace=Exception, QtWidgets=mock.MagicMock(), _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    created = []

    def new_object(kind, name, init, plot=True, autoselected=True):
        o = pg.Gerber(steps_per_circle=64)
        o.options = {}
        init(o, app)
        created.append((name, o))
        return o
    app.app_obj = mock.MagicMock()
    app.app_obj.new_object = new_object
    app.f_handlers = mock.MagicMock()
    app.f_handlers.export_gerber.return_value = ""

    def gerber(f, name):
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        g.options = {"name": name}
        g.follow = False
        return g
    cu = gerber(CU, "cu.gbr")
    logo = gerber(LOGO, "logo.gbr")
    mask = gerber(MASK, "mask.gbs")
    refs = {EDGE: gerber(EDGE, "edge"), LOGO: gerber(LOGO, "logo")}

    core = ns["Core"]()
    core.app = app
    core.geo_steps_per_circle = 64
    core.flat_geometry = []
    core.thief_solid_geometry = []
    core.robber_geo = core.robber_line = None
    core.rb_thickness = None
    core.sel_rect = []
    core.first_click = False
    core.mouse_is_dragging = False
    core.cursor_pos = (0, 0)
    noop = lambda *a, **k: None  # noqa: E731
    core.on_exit = core.connect_event_handlers = core.disconnect_event_handlers = noop
    core.draw_tool_selection_shape = core.delete_tool_selection_shape = noop
    app.delete_selection_shape = noop
    core.ui = mock.MagicMock()
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_thieving.py - no editar a mano.",
           "// Referencia: ToolCopperThieving (copper_thieving, robber bar, pattern plating mask) original.",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_thieving {",
           "struct ApOut { const char* id; const char* type; double size; int count; double area; int vertices; };",
           "struct Op { const char* kind; const char* src; const char* ref; const char* bbox; const char* fill; double clearance;"
           " double margin; double minArea; double dotDia; double dotSpacing; double squareSize; double squareSpacing;"
           " double lineSize; double lineSpacing; const char* refKind; const char* refFile; std::vector<double> clicks;"
           " double rbMargin; double rbThickness; double ppmClearance; const char* ppmChoice;"
           " bool created; const char* name; int polygons; double area; int vertices; std::vector<double> bounds;"
           " std::vector<ApOut> apertures; int thiefCount; double thiefArea; int thiefVertices; const char* platedArea;"
           " int follow; };",
           "inline const std::vector<Op> kOps = {"]
    for kind, over in OPS:
        p = dict(BASE, **over)
        ui = dict(clearance_entry=p["clearance"], margin_entry=p["margin"], reference_radio=p["ref"],
                  bbox_type_radio=p["bbox"], fill_type_radio=p["fill"], area_entry=p["area"],
                  dot_dia_entry=p["dot_dia"], dot_spacing_entry=p["dot_sp"], square_size_entry=p["sq"],
                  squares_spacing_entry=p["sq_sp"], line_size_entry=p["line"], lines_spacing_entry=p["line_sp"],
                  rb_margin_entry=p["rb_margin"], rb_thickness_entry=p["rb_thick"],
                  clearance_ppm_entry=p["ppm_clear"], ppm_choice_radio=p["ppm_choice"], plated_area_entry=None,
                  grb_object_combo=p["src"] + ".gbr", ref_combo=p["ref_file"], sm_object_combo="mask.gbs")
        for k, v in ui.items():
            setattr(core.ui, k, Val(v))
        src = mask if kind == "ppm" else (logo if p["src"] == "logo" else cu)
        idx = mock.MagicMock()
        idx.internalPointer.return_value.obj = src
        app.collection = mock.MagicMock()
        app.collection.index.return_value = idx

        def by_name(n):
            r = refs[n]
            r.kind = p["ref_kind"]
            return r
        app.collection.get_by_name = lambda n: {"cu.gbr": cu, "logo.gbr": logo}.get(n) or by_name(n)
        created.clear()
        try:
            if kind == "thief":
                core.on_add_copper_thieving_click()
                if p["ref"] == "area":
                    c = p["clicks"]
                    for i in range(0, len(c), 2):
                        core.on_mouse_release(Ev(c[i], c[i + 1], 1))
                    core.on_mouse_release(Ev(0.0, 0.0, 2))
            elif kind == "robber":
                core.on_add_robber_bar_click()
            else:
                core.on_new_pattern_plating_object()
        except Exception as e:  # el original falla en su hilo: no crea nada
            print("  falla:", type(e).__name__, e)
        core.sel_rect = []
        thief = core.thief_solid_geometry
        tcount, tarea, tverts = len(thief), sum(t.area for t in thief), sum(nverts(t) for t in thief)
        plated = core.ui.plated_area_entry.v
        name, npoly, area, verts, bounds, aps, follow = "", 0, 0.0, 0, [], [], 0
        if created:
            name, o = created[-1]
            parts = parts_of(o.solid_geometry)
            npoly, area, verts = len(parts), sum(x.area for x in parts), sum(nverts(x) for x in parts)
            bounds = list(MultiPolygon(parts).bounds) if parts else []
            for ap, v in o.apertures.items():
                geo = v.get("geometry", [])
                sol = [e["solid"] for e in geo if "solid" in e and not isinstance(e["solid"], list)]
                aps.append((ap, v["type"], float(v.get("size", -1)), len(geo), sum(s.area for s in sol),
                            sum(nverts(s) for s in sol)))
            follow = len(o.follow_geometry)
        ap_txt = ", ".join("{%s, %s, %r, %d, %r, %d}" % (cs(a), cs(t), s, n, ar, vv) for a, t, s, n, ar, vv in aps)
        out.append("{\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", %r, %r, %r, %r, %r, %r, %r, %r, %r, \"%s\", %s, {%s}, %r, %r, %r,"
                   " \"%s\", %s, %s, %d, %r, %d, {%s}, {%s}, %d, %r, %d, %s, %d}," % (
                       kind, p["src"], p["ref"], p["bbox"], p["fill"], p["clearance"], p["margin"], p["area"], p["dot_dia"],
                       p["dot_sp"], p["sq"], p["sq_sp"], p["line"], p["line_sp"], p["ref_kind"], cs(p["ref_file"]),
                       ", ".join(repr(v) for v in p["clicks"]), p["rb_margin"], p["rb_thick"], p["ppm_clear"],
                       p["ppm_choice"], "true" if created else "false", cs(name), npoly, area, verts,
                       ", ".join(repr(b) for b in bounds), ap_txt, tcount, tarea, tverts,
                       cs(repr(plated)) if plated is not None else "nullptr", follow))
        print(kind, over, bool(created), name, npoly, round(area, 4), verts, tcount, round(tarea, 4), plated,
              [(a[0], a[3]) for a in aps][-3:])
    out += ["};", "}  // namespace golden_thieving"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
