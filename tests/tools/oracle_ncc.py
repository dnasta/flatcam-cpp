#!/usr/bin/env python3
"""Oráculo del NCC (limpieza de cobre): ejecuta el algoritmo ORIGINAL de ToolNCC sobre casos sintéticos.

Transcribe las líneas esenciales de `ToolNCC.clear_copper` (`calculate_bounding_box`, `apply_margin_to_bounding_box`,
`get_tool_empty_area` y `get_ncc_empty_area`) para un Gerber y la selección "Él mismo", y llama a la
`Geometry.clear_polygon` original con el paquete `rtree` real. Genera `tests/golden_ncc.h`.

Entorno:  micromamba create -n old -c conda-forge python=3.10 shapely=1.8.4 numpy=1.22 rtree
Uso:      python tests/tools/oracle_ncc.py
"""
import os
import sys

from shapely.geometry import MultiPolygon, Point, Polygon, base
from shapely.ops import unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs, num  # noqa: E402
from oracle_paint import cpp_lines, cpp_poly  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_ncc.h")


def rect(x0, y0, x1, y1):
    return Polygon([(x0, y0), (x1, y0), (x1, y1), (x0, y1)])


PADS = [rect(2, 2, 4, 3), rect(6, 5, 9, 6.5), Point(5, 2.5).buffer(0.8, 16)]
ISLAND = [Polygon([(2, 2), (10, 2), (10, 8), (2, 8)], [[(4, 4), (8, 4), (8, 6), (4, 6)]]), rect(5, 4.8, 7, 5.2)]
SINGLE = [Polygon([(0, 0), (6, 0), (6, 2), (2, 2), (2, 5), (0, 5)])]
TRACKS = [rect(1, 1, 1.4, 9), rect(3, 1, 3.4, 9), rect(5, 1, 5.4, 9), rect(7, 1, 7.4, 9), rect(1, 9, 7.4, 9.4)]

# nombre, cobre, herramienta, solape, conectar, margen, desplazar, valor del desplazamiento
SCENARIOS = [
    ("three_pads", PADS, 0.3, 0.4, True, 0.0, False, 0.0),
    ("three_pads_no_connect", PADS, 0.3, 0.4, False, 0.0, False, 0.0),
    ("with_margin", PADS, 0.3, 0.4, True, 0.8, False, 0.0),
    ("with_offset", PADS, 0.2, 0.5, True, 0.5, True, 0.2),
    ("island_with_hole", ISLAND, 0.4, 0.3, True, 0.5, False, 0.0),
    ("single_polygon_uses_union", SINGLE, 0.3, 0.5, True, 0.4, False, 0.0),
    ("parallel_tracks", TRACKS, 0.25, 0.5, True, 0.3, False, 0.0),
    ("tool_too_big", PADS, 30.0, 0.4, True, 0.0, False, 0.0),
]


def ncc(geo, copper, dia, over, conn, margin, has_offset, offset, steps=64):
    """Región a limpiar y trayectorias, como `ToolNCC.clear_copper` (una herramienta, Gerber, "Él mismo")."""
    if isinstance(copper, MultiPolygon):
        env = copper.convex_hull
    elif isinstance(copper, list) and len(copper) == 1 and isinstance(copper[0], Polygon):
        env = unary_union(copper)
    else:
        env = unary_union(copper).convex_hull
    bbox = env.buffer(distance=margin, join_style=base.JOIN_STYLE.mitre)
    sol_geo = MultiPolygon(copper) if isinstance(copper, list) else copper
    if has_offset:
        sol_geo = sol_geo.buffer(distance=offset)
    empty = bbox.difference(sol_geo)
    if empty.is_empty:
        return [], []
    if type(empty) is Polygon:
        empty = MultiPolygon([empty])
    region = list(empty.geoms)
    cleared = []
    for p in region:
        p = p.buffer(0)
        if p is not None and p.is_valid:
            try:
                pols = list(p)
            except TypeError:
                pols = [p]
            for pol in pols:
                if isinstance(pol, Polygon):
                    cp = geo.clear_polygon(pol, dia, steps, overlap=over, contour=True, connect=conn)
                    if cp and cp.objects:
                        cleared += list(cp.get_objects())
    return region, cleared


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    app = FakeApp()
    app.defaults["geometry_circle_steps"] = 64
    camlib.Geometry.app = app
    geo = camlib.Geometry(geo_steps_per_circle=64)

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_ncc.py - no editar a mano.",
           "// Referencia: ToolNCC.clear_copper y Geometry.clear_polygon originales (FlatCAM 2024.4), Shapely %s." %
           __import__("shapely").__version__,
           "#pragma once", "#include <vector>", '#include "tools/Ncc.h"', "namespace golden_ncc {",
           "using Lines = std::vector<std::vector<flatcam::Point2D>>;"]
    entries = []
    for idx, (name, copper, dia, over, conn, margin, has_off, off) in enumerate(SCENARIOS):
        region, lines = ncc(geo, copper, dia, over, conn, margin, has_off, off)
        lines = [l for l in lines if l and not l.is_empty]
        print("%-28s región=%d polígonos  trazados=%d puntos=%d" % (name, len(region), len(lines),
                                                                  sum(len(l.coords) for l in lines)), flush=True)
        out.append("inline flatcam::ShapeList copper%d() {  // %s" % (idx, name))
        out.append("    return {%s};" % ", ".join(cpp_poly(p) for p in copper))
        out.append("}")
        out.append("inline flatcam::ShapeList region%d() {" % idx)
        out.append("    return {%s};" % ", ".join(cpp_poly(p) for p in region))
        out.append("}")
        entries.append((name, dia, over, conn, margin, has_off, off, lines))
    out.append("struct Scenario { const char* name; flatcam::ShapeList (*copper)(); flatcam::ShapeList (*region)(); double dia;"
               " double overlap; bool connect; double margin; bool hasOffset; double offset; bool expectEmpty; Lines lines; };")
    out.append("inline const std::vector<Scenario> kScenarios = {")
    for idx, (name, dia, over, conn, margin, has_off, off, lines) in enumerate(entries):
        out.append("{%s, copper%d, region%d, %s, %s, %s, %s, %s, %s, %s, %s}," % (
            cs(name), idx, idx, num(dia), num(over), str(conn).lower(), num(margin), str(has_off).lower(), num(off),
            str(not lines).lower(), cpp_lines(lines)))
    out.append("};")
    out.append("}  // namespace golden_ncc")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.1f MB)" % (os.path.getsize(OUT) / 1e6))


if __name__ == "__main__":
    main()
