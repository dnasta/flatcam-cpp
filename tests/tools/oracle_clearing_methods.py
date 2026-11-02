#!/usr/bin/env python3
"""Oráculo de los métodos de Semilla y Líneas: ejecuta `Geometry.clear_polygon2` y `clear_polygon3` ORIGINALES.

Necesita Shapely 1.8.4 y el paquete `rtree` real (`paint_connect` lo usa). Escribe `tests/golden_clearing_methods.h` con
las trayectorias de cada caso (o "sin resultado").
Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_clearing_methods.py
"""
import os
import sys

from shapely.geometry import Polygon

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import num  # noqa: E402
from oracle_paint import cpp_poly  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_clearing_methods.h")

SQUARE_HOLE = Polygon([(0, 0), (8, 0), (8, 6), (0, 6)], [[(2.5, 1.5), (5.5, 1.5), (5.5, 4.5), (2.5, 4.5)]])
L_SHAPE = Polygon([(0, 0), (6, 0), (6, 2), (2, 2), (2, 6), (0, 6)])
TALL = Polygon([(0, 0), (3, 0), (3, 9), (0, 9)])
ROUND = Polygon([(5 + 4 * __import__("math").cos(a / 20 * 6.283185307179586),
                  3 + 3 * __import__("math").sin(a / 20 * 6.283185307179586)) for a in range(20)])
SMALL = Polygon([(0, 0), (0.1, 0), (0.1, 0.1), (0, 0.1)])

# nombre, polígono, método (2 = semilla, 3 = líneas), opciones
SCENARIOS = []
for name, pol in [("square_with_hole", SQUARE_HOLE), ("l_shape", L_SHAPE), ("tall", TALL), ("ellipse", ROUND),
                  ("too_small", SMALL)]:
    for method in (2, 3):
        SCENARIOS.append((name, pol, method, dict(dia=0.5, over=0.4, conn=True, cont=True)))
SCENARIOS += [("square_with_hole", SQUARE_HOLE, 2, dict(dia=0.5, over=0.4, conn=False, cont=True)),
              ("square_with_hole", SQUARE_HOLE, 3, dict(dia=0.5, over=0.4, conn=False, cont=False)),
              ("l_shape", L_SHAPE, 2, dict(dia=0.3, over=0.8, conn=True, cont=False)),
              ("l_shape", L_SHAPE, 3, dict(dia=0.3, over=0.8, conn=True, cont=True))]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    app = FakeApp()
    app.defaults.update({"geometry_circle_steps": 64})
    camlib.Geometry.app = app
    geo = camlib.Geometry(geo_steps_per_circle=64)
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_clearing_methods.py - no editar a mano.",
           "// Referencia: Geometry.clear_polygon2 (semilla) y clear_polygon3 (líneas) originales (FlatCAM 2024.4).",
           "#pragma once", "#include <vector>", '#include "core/Shape.h"', "namespace golden_clearing_methods {",
           "using Lines = std::vector<std::vector<flatcam::Point2D>>;",
           "struct Scenario { const char* name; flatcam::Shape (*polygon)(); int method; double dia; double overlap;"
           " bool connect; bool contour; bool hasResult; Lines lines; };"]
    rows = []
    for idx, (name, pol, method, o) in enumerate(SCENARIOS):
        fn = geo.clear_polygon2 if method == 2 else geo.clear_polygon3
        res = fn(pol, tooldia=o["dia"], steps_per_circle=64, overlap=o["over"], connect=o["conn"], contour=o["cont"])
        lines = [g for g in res.get_objects()] if res else None
        print("%-18s m=%d %s" % (name, method, "sin resultado" if lines is None else
                                  "%d trazados %d puntos" % (len(lines), sum(len(l.coords) for l in lines))), flush=True)
        out.append("inline flatcam::Shape polygon%d() { return %s; }" % (idx, cpp_poly(pol)))
        ls = "{}" if not lines else "{" + ", ".join(
            "{" + ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in l.coords) + "}" for l in lines) + "}"
        rows.append('{"%s", polygon%d, %d, %s, %s, %s, %s, %s, %s},' % (
            name, idx, method, num(o["dia"]), num(o["over"]), str(o["conn"]).lower(), str(o["cont"]).lower(),
            "false" if lines is None else "true", ls))
    out.append("inline const std::vector<Scenario> kScenarios = {")
    out += rows
    out += ["};", "}  // namespace golden_clearing_methods"]
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
