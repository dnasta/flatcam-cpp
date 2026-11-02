#!/usr/bin/env python3
"""Oráculo del mecanizado de resto de NCC y de Paint, con los `clear_polygon*` ORIGINALES.

Transcribe `gen_clear_area_rest` (ToolNCC) y `job_rest_clear` (ToolPaint, dentro de `paint_geo`) y escribe
`tests/golden_rest_machining.h` con las trayectorias de cada herramienta (de la más grande a la más chica).
Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_rest_machining.py
"""
import os
import sys

from shapely import geometry as base
from shapely.geometry import MultiPolygon, Point, Polygon
from shapely.ops import unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import num  # noqa: E402
from oracle_paint import cpp_lines, cpp_poly, flatten_input  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_rest_machining.h")


def rect(x0, y0, x1, y1):
    return Polygon([(x0, y0), (x1, y0), (x1, y1), (x0, y1)])


PADS = [rect(2, 2, 4, 3), rect(6, 5, 9, 6.5), Point(5, 2.5).buffer(0.8, 16)]
TRACKS = [rect(1, 1, 1.4, 9), rect(3, 1, 3.4, 9), rect(5, 1, 5.4, 9), rect(7, 1, 7.4, 9), rect(1, 9, 7.4, 9.4)]
SQUARE_HOLE = Polygon([(0, 0), (8, 0), (8, 6), (0, 6)], [[(2.5, 1.5), (5.5, 1.5), (5.5, 4.5), (2.5, 4.5)]])
L_SHAPE = Polygon([(0, 0), (6, 0), (6, 2), (2, 2), (2, 6), (0, 6)])
NARROW = Polygon([(0, 0), (10, 0), (10, 1.2), (4, 1.2), (4, 6), (3.3, 6), (3.3, 1.2), (0, 1.2)])

# kind, nombre, figuras, herramientas [(diámetro, solape, método)], opciones comunes
SCENARIOS = [
    ("ncc", "pads_two_tools", PADS, [(0.8, 0.4, 0), (0.3, 0.4, 0)], dict(margin=0.5, conn=True, cont=True)),
    ("ncc", "tracks_seed_then_lines", TRACKS, [(0.25, 0.5, 2), (1.0, 0.4, 1)], dict(margin=0.3, conn=True, cont=True)),
    ("ncc", "tracks_combo", TRACKS, [(0.6, 0.3, 3), (0.2, 0.3, 0)], dict(margin=0.0, conn=False, cont=True)),
    ("paint", "l_shape_two_tools", [L_SHAPE], [(1.5, 0.4, 0), (0.4, 0.4, 0)], dict(offset=0.0, method=0)),
    ("paint", "narrow_lines", [NARROW], [(2.0, 0.3, 0), (0.5, 0.3, 0)], dict(offset=0.0, method=2)),
    ("paint", "square_hole_seed_offset", [SQUARE_HOLE], [(1.0, 0.5, 0), (0.3, 0.5, 0)], dict(offset=0.1, method=1)),
]


def worker(geo, pol, dia, method, over, conn, cont, steps=64):
    """clear_polygon_worker (NCC) / paint_polygon_worker (Paint) con los métodos 0..3 (3 = combinado)."""
    cp = None
    try:
        if method == 0:
            cp = geo.clear_polygon(pol, dia, steps_per_circle=steps, overlap=over, contour=cont, connect=conn)
        elif method == 1:
            cp = geo.clear_polygon2(pol, dia, steps_per_circle=steps, overlap=over, contour=cont, connect=conn)
        elif method == 2:
            cp = geo.clear_polygon3(pol, dia, steps_per_circle=steps, overlap=over, contour=cont, connect=conn)
        else:
            cp = geo.clear_polygon3(pol, dia, steps_per_circle=steps, overlap=over, contour=cont, connect=conn)
            if not (cp and cp.objects):
                cp = geo.clear_polygon2(pol, dia, steps_per_circle=steps, overlap=over, contour=cont, connect=conn)
                if not (cp and cp.objects):
                    cp = geo.clear_polygon(pol, dia, steps_per_circle=steps, overlap=over, contour=cont, connect=conn)
    except Exception:
        cp = None
    return cp if (cp and cp.objects) else None


def ncc_rest(geo, copper, tools, o, steps=64):
    if isinstance(copper, list) and len(copper) == 1 and isinstance(copper[0], Polygon):
        env = unary_union(copper)
    else:
        env = unary_union(copper).convex_hull
    bbox = env.buffer(distance=o["margin"], join_style=base.JOIN_STYLE.mitre)
    area = bbox.difference(MultiPolygon(copper))
    if isinstance(area, Polygon):
        area = MultiPolygon([area])
    out = []
    for dia, over, method in sorted(tools, key=lambda t: -t[0]):
        cleared = []
        for p in area.geoms:
            if p is not None and p.is_valid and not p.is_empty:
                if p.buffer(-dia / 2, steps).is_empty:
                    continue
                if isinstance(p, Polygon):
                    res = worker(geo, p, dia, method, over, o["conn"], o["cont"])
                    if res is not None:
                        cleared += list(res.get_objects())
        out.append((dia, cleared))
        new_area = MultiPolygon([line.buffer(dia / 1.9999999) for line in cleared]).buffer(0.0000001)
        area = area.difference(new_area)
        if isinstance(area, Polygon):
            area = MultiPolygon([area])
        area = MultiPolygon([pol for pol in area.geoms if pol.is_valid and not pol.is_empty])
        if not area or area.is_empty:
            break
    return [(d, c) for d, c in out if c]


def paint_rest(geo, geoms, tools, o, steps=64):
    poly_buf = []
    for pol in flatten_input(geoms):
        b = pol.buffer(-o["offset"])
        if b and not b.is_empty:
            try:
                for x in b:
                    poly_buf.append(x)
            except TypeError:
                poly_buf.append(b)
    poly_buf = unary_union(poly_buf)
    out = []
    for dia, over, _m in sorted(tools, key=lambda t: -t[0]):
        rest_list = []
        cleared = []
        try:
            pieces = list(poly_buf) if not isinstance(poly_buf, Polygon) else [poly_buf]
            for pp in pieces:
                if pp.buffer(-dia / 2.0).is_empty:
                    continue
                res = worker(geo, pp, dia, o["method"], over, True, True)
                elems = list(res.get_objects())  # None -> excepción: se saltea la herramienta
                pp_cleared = unary_union(elems).buffer(dia / 2.0)
                rest = pp.difference(pp_cleared)
                if rest and not rest.is_empty:
                    try:
                        for r in rest:
                            if r.is_valid:
                                rest_list.append(r)
                    except TypeError:
                        if rest.is_valid:
                            rest_list.append(rest)
                cleared += elems
        except Exception:
            continue
        if cleared:
            out.append((dia, cleared))
        poly_buf = poly_buf.difference(unary_union(cleared).buffer(dia / 2.0))
        tmp = []
        try:
            for p in poly_buf:
                if p.is_valid:
                    tmp.append(p)
        except TypeError:
            if poly_buf.is_valid:
                tmp.append(poly_buf)
        tmp += rest_list
        poly_buf = MultiPolygon(tmp)
        if not poly_buf.is_valid:
            poly_buf = unary_union(tmp)
        if not poly_buf or poly_buf.is_empty or not poly_buf.is_valid:
            break
    return out


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    app = FakeApp()
    app.defaults.update({"geometry_circle_steps": 64})
    camlib.Geometry.app = app
    geo = camlib.Geometry(geo_steps_per_circle=64)
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_rest_machining.py - no editar a mano.",
           "// Referencia: gen_clear_area_rest (NCC) y job_rest_clear (Paint) con los clear_polygon* originales.",
           "#pragma once", "#include <vector>", '#include "core/Shape.h"', "namespace golden_rest_machining {",
           "using Lines = std::vector<std::vector<flatcam::Point2D>>;",
           "struct Tool { double dia; double overlap; int method; };",
           "struct PerTool { double dia; Lines lines; };",
           "struct Scenario { const char* kind; const char* name; flatcam::ShapeList (*shapes)(); std::vector<Tool> tools;"
           " double margin; bool connect; bool contour; double offset; int method; std::vector<PerTool> expected; };"]
    rows = []
    for idx, (kind, name, shapes, tools, o) in enumerate(SCENARIOS):
        res = ncc_rest(geo, shapes, tools, o) if kind == "ncc" else paint_rest(geo, shapes, tools, o)
        print("%-6s %-26s %s" % (kind, name, ", ".join("%.2f: %d trazados" % (d, len(c)) for d, c in res)), flush=True)
        out.append("inline flatcam::ShapeList shapes%d() { return {%s}; }" % (idx, ", ".join(cpp_poly(s) for s in shapes)))
        exp = ", ".join("{%s, %s}" % (num(d), cpp_lines(c)) for d, c in res)
        tl = ", ".join("{%s, %s, %d}" % (num(d), num(ov), m) for d, ov, m in tools)
        rows.append('{"%s", "%s", shapes%d, {%s}, %s, %s, %s, %s, %d, {%s}},' % (
            kind, name, idx, tl, num(o.get("margin", 0.0)), str(o.get("conn", True)).lower(),
            str(o.get("cont", True)).lower(), num(o.get("offset", 0.0)), o.get("method", 0), exp))
    out.append("inline const std::vector<Scenario> kScenarios = {")
    out += rows
    out += ["};", "}  // namespace golden_rest_machining"]
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
