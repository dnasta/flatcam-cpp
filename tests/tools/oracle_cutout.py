#!/usr/bin/env python3
"""Oráculo del CutOut: ejecuta `CutOut.on_freeform_cutout` ORIGINAL (FlatCAM 2024.4) con una interfaz simulada.

Usa el contorno `Edge_Cuts.gm1` del usuario (parseado con el parser original) y varias geometrías sintéticas, con
distintos puentes, márgenes, tipos de objeto, puentes finos y "mouse bites". Guarda en `tests/golden_cutout.h` el
contorno, los tramos de los puentes finos y los agujeros, y el código C++ que arma las mismas entradas.

También genera casos del puente manual (`subtract_geo` + `linemerge`).

Requisitos (solo para regenerar el archivo): Shapely 1.8.4 (el del original) y numpy.
Uso:  python3 tests/tools/oracle_cutout.py
"""
import os
import sys
import types
from copy import deepcopy
from unittest import mock

from shapely import wkt
from shapely.geometry import LineString, MultiLineString, Point, Polygon, box
from shapely.ops import linemerge

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load, make_stub  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_cutout.h")
EDGE = "@EDGE_CUTS"

TWO_RECTS = ["POLYGON ((0 0, 20 0, 20 10, 0 10, 0 0))", "POLYGON ((30 2, 45 2, 45 12, 30 12, 30 2))"]
L_SHAPE = "POLYGON ((0 0, 30 0, 30 10, 10 10, 10 25, 0 25, 0 0))"
RING_PLATE = "POLYGON ((0 0, 40 0, 40 30, 0 30, 0 0), (10 8, 30 8, 30 22, 10 22, 10 8))"
SQUARE = "POLYGON ((0 0, 25 0, 25 18, 0 18, 0 0))"

# nombre, entrada (WKT o EDGE), es Gerber, opciones (nombres de CutoutOptions), tipo de objeto
SCENARIOS = [
    ("edge_cuts_tb_user", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="TB", gapsize=0.4)),
    ("edge_cuts_four", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="4", gapsize=0.4)),
    ("edge_cuts_none", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="None", gapsize=0.4)),
    ("edge_cuts_lr", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="LR", gapsize=0.4)),
    ("edge_cuts_two_lr", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="2LR", gapsize=0.4)),
    ("edge_cuts_two_tb", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="2TB", gapsize=0.4)),
    ("edge_cuts_eight", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="8", gapsize=0.4)),
    ("edge_cuts_positive_margin", [EDGE], True, dict(dia=1.2, margin=0.5, gaps="TB", gapsize=0.4)),
    ("single_two_rects_bbox", TWO_RECTS, True, dict(dia=2.0, margin=0.3, gaps="4", gapsize=2.0)),
    ("single_convex_l_shape", [L_SHAPE], True, dict(dia=2.0, margin=0.3, gaps="4", gapsize=2.0, convex=True)),
    ("single_negative_margin", [SQUARE], True, dict(dia=2.0, margin=-0.2, gaps="LR", gapsize=2.0)),
    ("multi_two_rects", TWO_RECTS, True, dict(dia=2.0, margin=0.3, gaps="TB", gapsize=2.0, kind="multi")),
    ("multi_negative_margin_hole", [RING_PLATE], True, dict(dia=2.0, margin=-0.5, gaps="4", gapsize=2.0, kind="multi")),
    ("multi_convex", TWO_RECTS, True, dict(dia=2.0, margin=0.3, gaps="4", gapsize=2.0, kind="multi", convex=True)),
    ("thin_bridges_single", [SQUARE], True, dict(dia=2.0, margin=0.3, gaps="4", gapsize=2.0, gaptype="bt", thin=0.4)),
    ("thin_bridges_multi", TWO_RECTS, True, dict(dia=2.0, margin=0.3, gaps="TB", gapsize=2.0, gaptype="bt", thin=0.4,
                                                 kind="multi")),
    ("mouse_bites_single", [SQUARE], True, dict(dia=2.0, margin=0.3, gaps="4", gapsize=4.0, gaptype="mb", mbdia=0.5,
                                                mbspacing=0.3)),
    ("mouse_bites_multi", TWO_RECTS, True, dict(dia=2.0, margin=0.3, gaps="TB", gapsize=4.0, gaptype="mb", mbdia=0.5,
                                                mbspacing=0.3, kind="multi")),
    ("geometry_single", [SQUARE], False, dict(dia=2.0, margin=0.0, gaps="4", gapsize=2.0)),
    ("geometry_multi", TWO_RECTS, False, dict(dia=2.0, margin=0.0, gaps="TB", gapsize=2.0, kind="multi")),
]

RECT = [
    ("rect_edge_cuts_tb_user", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="TB", gapsize=0.4, mode="rect")),
    ("rect_edge_cuts_four", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="4", gapsize=0.4, mode="rect")),
    ("rect_edge_cuts_none_fails", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="None", gapsize=0.4, mode="rect")),
    ("rect_edge_cuts_lr", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="LR", gapsize=0.4, mode="rect")),
    ("rect_edge_cuts_two_lr", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="2LR", gapsize=0.4, mode="rect")),
    ("rect_edge_cuts_two_tb", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="2TB", gapsize=0.4, mode="rect")),
    ("rect_edge_cuts_eight", [EDGE], True, dict(dia=1.2, margin=0.0, gaps="8", gapsize=0.4, mode="rect")),
    ("rect_edge_cuts_margin", [EDGE], True, dict(dia=1.2, margin=0.5, gaps="TB", gapsize=0.4, mode="rect")),
    ("rect_negative_margin_single", [SQUARE], True, dict(dia=2.0, margin=-0.3, gaps="4", gapsize=2.0, mode="rect")),
    ("rect_gerber_two_rects_single", TWO_RECTS, True, dict(dia=2.0, margin=0.3, gaps="4", gapsize=2.0, mode="rect")),
    ("rect_gerber_multi", TWO_RECTS, True, dict(dia=2.0, margin=0.3, gaps="TB", gapsize=2.0, kind="multi", mode="rect")),
    ("rect_gerber_multi_negative_fails", TWO_RECTS, True, dict(dia=2.0, margin=-0.3, gaps="TB", gapsize=2.0, kind="multi", mode="rect")),
    ("rect_geometry_single", [SQUARE], False, dict(dia=2.0, margin=0.0, gaps="4", gapsize=2.0, mode="rect")),
    ("rect_geometry_multi", TWO_RECTS, False, dict(dia=2.0, margin=0.0, gaps="TB", gapsize=2.0, kind="multi", mode="rect")),
    ("rect_thin_bridges_single", [SQUARE], True, dict(dia=2.0, margin=0.3, gaps="4", gapsize=2.0, gaptype="bt", thin=0.4, mode="rect")),
    ("rect_thin_bridges_multi", TWO_RECTS, True, dict(dia=2.0, margin=0.3, gaps="TB", gapsize=2.0, gaptype="bt", thin=0.4, kind="multi", mode="rect")),
    ("rect_mouse_bites_single", [SQUARE], True, dict(dia=2.0, margin=0.3, gaps="4", gapsize=4.0, gaptype="mb", mbdia=0.5, mbspacing=0.3, mode="rect")),
    ("rect_mouse_bites_multi", TWO_RECTS, True, dict(dia=2.0, margin=0.3, gaps="TB", gapsize=4.0, gaptype="mb", mbdia=0.5, mbspacing=0.3, kind="multi", mode="rect")),
]
SCENARIOS += RECT

MANUAL = [
    ("manual_top_side", "POLYGON ((0 0, 20 0, 20 10, 0 10, 0 0))", (10.0, 10.0), 1.2, 0.4),
    ("manual_corner", "POLYGON ((0 0, 20 0, 20 10, 0 10, 0 0))", (20.0, 10.0), 2.0, 1.0),
    ("manual_two_in_a_row", "POLYGON ((0 0, 20 0, 20 10, 0 10, 0 0))", [(10.0, 0.0), (10.0, 10.0)], 1.2, 0.4),
]


class W:
    """Widget simulado: devuelve un valor fijo."""
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v

    def currentText(self):
        return self.v


def install_tool_stubs():
    class AppTool:
        def __init__(self, *a, **k):
            pass
    make_stub("appTool", AppTool=AppTool)
    make_stub("appGUI.GUIElements", **{n: mock.MagicMock() for n in
              ["FCDoubleSpinner", "FCCheckBox", "RadioSet", "FCComboBox", "OptionalInputSection", "FCButton", "FCLabel"]})
    make_stub("matplotlib")
    make_stub("matplotlib.backend_bases", KeyEvent=object)
    import json
    make_stub("simplejson", dumps=json.dumps, loads=json.loads)


def num(v):
    return repr(float(v))


def cs(text):
    return '"' + text.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n") + '"'


def lines_of(g):
    """Lista de líneas (coordenadas) de una geometría o lista de geometrías de Shapely."""
    out = []

    def rec(o):
        if o is None:
            return
        if isinstance(o, (list, tuple)):
            for x in o:
                rec(x)
        elif hasattr(o, "geoms"):
            for x in o.geoms:
                rec(x)
        elif not o.is_empty:
            out.append([tuple(c) for c in o.coords])
    rec(g)
    return out


def cpp_lines(ls):
    return "{" + ", ".join("{" + ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in l) + "}" for l in ls) + "}"


def run_freeform(camlib, cutout_mod, name, shapes, is_gerber, o):
    tool = object.__new__(cutout_mod.CutOut)
    tool.cut_tool_dict = {"data": {}}
    created = {}

    class Collection:
        def get_by_name(self, n):
            return obj

        def promise(self, n):
            pass

    class AppObj:
        def new_object(self, kind, outname, init, *a, **k):
            # `solid_geometry` con algo de contenido: el original calcula los límites del Excellon creado.
            o_ = types.SimpleNamespace(options={"name": outname}, tools={}, solid_geometry=[box(0, 0, 1, 1)],
                                       multigeo=False, create_geometry=lambda: None)
            r = init(o_, app)
            created[kind] = o_
            return r

    app = types.SimpleNamespace(collection=Collection(), inform=mock.MagicMock(), app_obj=AppObj(),
                                worker_task=types.SimpleNamespace(emit=lambda d: d["fcn"](*d["params"])),
                                f_handlers=types.SimpleNamespace(export_excellon=lambda **k: ""),
                                should_we_save=False, proc_container=mock.MagicMock())
    obj = types.SimpleNamespace(kind="gerber" if is_gerber else "geometry", solid_geometry=shapes, multigeo=False,
                                options={"name": name}, tools={})
    tool.app = app
    ui = types.SimpleNamespace(
        obj_combo=W(name), dia=W(o["dia"]), obj_kind_combo=W(o.get("kind", "single")), margin=W(o["margin"]),
        gaps=W(o["gaps"]), gapsize=W(o["gapsize"]), convex_box_cb=W(o.get("convex", False)),
        mb_dia_entry=W(o.get("mbdia", 0.5)), mb_spacing_entry=W(o.get("mbspacing", 0.25)),
        gaptype_radio=W(o.get("gaptype", "b")), thin_depth_entry=W(o.get("thin", 0.0)), cutz_entry=W(-1.0),
        mpass_cb=W(False), maxdepth_entry=W(0.5))
    tool.ui = ui
    try:
        if o.get("mode") == "rect":
            tool.on_rectangular_cutout()
        else:
            tool.on_freeform_cutout()
    except Exception as e:  # el original lanza en un hilo de trabajo y la excepción se pierde: no hay geometría
        print("   (excepción del original: %s)" % type(e).__name__, flush=True)
        return None
    geo = created.get("geometry")
    if geo is None:
        return None
    solid = lines_of(geo.tools[1]["solid_geometry"])
    gaps = lines_of(geo.tools[9999]["solid_geometry"]) if 9999 in geo.tools else []
    holes = []
    if "excellon" in created:
        holes = [(p.x, p.y) for p in created["excellon"].tools[1]["drills"]]
    return solid, gaps, holes


def main():
    install_stubs()
    install_tool_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    parse_gerber = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    cutout_mod = load("ToolCutOut", os.path.join(REF, "appTools", "ToolCutOut.py"))
    app = FakeApp()
    parse_gerber.Gerber.app = app
    camlib.Geometry.app = app
    g = parse_gerber.Gerber(steps_per_circle=64)
    g.parse_file(os.path.join(DATA, "PCB principal-Edge_Cuts.gm1"))
    edge_solid = list(g.solid_geometry) if isinstance(g.solid_geometry, (list, tuple)) else [g.solid_geometry]

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_cutout.py - no editar a mano.",
           "// Referencia: CutOut.on_freeform_cutout original (FlatCAM 2024.4) con Shapely %s." % __import__("shapely").__version__,
           "#pragma once", "#include <string>", "#include <vector>", '#include "tools/CutOut.h"',
           "namespace golden_cutout {",
           "using Lines = std::vector<std::vector<flatcam::Point2D>>;",
           "struct Scenario { const char* name; bool useEdgeCuts; bool isGerber; bool rectangular; bool expectFail; std::vector<std::string> wkt;",
           "                  flatcam::CutoutOptions options; Lines solid; Lines gaps; std::vector<flatcam::Point2D> holes; };",
           "inline const std::vector<Scenario> kScenarios = {"]
    for name, inputs, is_gerber, o in SCENARIOS:
        use_edge = inputs == [EDGE]
        shapes = deepcopy(edge_solid) if use_edge else [wkt.loads(w) for w in inputs]
        res = run_freeform(camlib, cutout_mod, name, shapes, is_gerber, o)
        expect_fail = res is None
        if expect_fail:
            print("%-30s el original NO genera geometría (se espera fallo)" % name, flush=True)
            solid, gaps, holes = [], [], []
        else:
            solid, gaps, holes = res
        if not expect_fail:
            print("%-30s líneas=%d  puentes finos=%d  agujeros=%d" % (name, len(solid), len(gaps), len(holes)), flush=True)
        gm = {"None": "None", "LR": "LR", "TB": "TB", "2LR": "TwoLR", "2TB": "TwoTB", "4": "Four", "8": "Eight"}[o["gaps"]]
        gaptype = o.get("gaptype", "b")
        opts = ["{", "  flatcam::CutoutOptions c;", "  c.toolDia = %s;" % num(o["dia"]), "  c.margin = %s;" % num(o["margin"]),
                "  c.gaps = flatcam::CutoutGaps::%s;" % gm, "  c.gapSize = %s;" % num(o["gapsize"]),
                "  c.convexBox = %s;" % str(bool(o.get("convex", False))).lower(),
                "  c.kind = flatcam::CutoutKind::%s;" % ("Multi" if o.get("kind") == "multi" else "Single"),
                "  c.thinBridges = %s;" % str(gaptype == "bt").lower(), "  c.thinDepth = %s;" % num(o.get("thin", 0.0)),
                "  c.mouseBites = %s;" % str(gaptype == "mb").lower(), "  c.mbDia = %s;" % num(o.get("mbdia", 0.5)),
                "  c.mbSpacing = %s;" % num(o.get("mbspacing", 0.25)), "  return c; }()"]
        out.append('{%s, %s, %s, %s, %s, {%s},' % (cs(name), str(use_edge).lower(), str(is_gerber).lower(),
                                                    str(o.get("mode") == "rect").lower(), str(expect_fail).lower(),
                                                    ", ".join(cs(w) for w in ([] if use_edge else inputs))))
        out.append("[]() " + "\n".join(opts) + ",")
        out.append("%s, %s, {%s}}," % (cpp_lines(solid), cpp_lines(gaps), ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in holes)))
    out.append("};")

    # ---- puente manual: restar la caja y unir las líneas
    out.append("struct Manual { const char* name; const char* wkt; std::vector<flatcam::Point2D> clicks; double dia; double gap; Lines expected; };")
    out.append("inline const std::vector<Manual> kManual = {")
    for name, w, clicks, dia, gap in MANUAL:
        poly = wkt.loads(w)
        solid = linemerge([poly.exterior])
        clicks_l = clicks if isinstance(clicks, list) else [clicks]
        for pos in clicks_l:
            offset = dia / 2 + gap / 2
            cut = box(pos[0] - offset, pos[1] - offset, pos[0] + offset, pos[1] + offset)
            new_solid = cutout_mod.CutOut.subtract_geo(solid, cut)
            solid = linemerge(new_solid)
        print("%-30s líneas=%d" % (name, len(lines_of(solid))), flush=True)
        out.append('{%s, %s, {%s}, %s, %s, %s},' % (cs(name), cs(w), ", ".join("{%s, %s}" % (num(p[0]), num(p[1])) for p in clicks_l),
                                                   num(dia), num(gap), cpp_lines(lines_of(solid))))
    out.append("};")
    out.append("}  // namespace golden_cutout")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
