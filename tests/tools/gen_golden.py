#!/usr/bin/env python3
"""Genera tests/golden_data.h con valores de referencia calculados con Shapely y numpy.

Solo hace falta ejecutarlo si se agregan casos nuevos:
    python3 -m pip install shapely numpy
    python3 tests/tools/gen_golden.py

Las funciones de arcos son copias fieles de las de FlatCAM 2024.4 (camlib.py), de modo que los
tests de C++ comparan contra el comportamiento del programa original.
"""
import os
import numpy as np
import shapely
from shapely import affinity
from shapely.geometry import LineString, Point, Polygon
from shapely.ops import unary_union
from numpy.linalg import solve

out = []


def num(v):
    return repr(float(v))


def const(name, v):
    out.append(f"constexpr double {name} = {num(v)};")


def pts(name, coords):
    body = ", ".join(f"{{{num(x)}, {num(y)}}}" for x, y in coords)
    out.append(f"inline const std::vector<flatcam::Point2D> {name} = {{{body}}};")


# ---- copias de camlib.py -----------------------------------------------------------------------
def arc(center, radius, start, stop, direction, steps_per_circ):
    da_sign = {"cw": -1.0, "ccw": 1.0}
    points = []
    if direction == "ccw" and stop <= start:
        stop += 2 * np.pi
    if direction == "cw" and stop >= start:
        stop -= 2 * np.pi
    angle = abs(stop - start)
    steps = max([int(np.ceil(angle / (2 * np.pi) * steps_per_circ)), 2])
    delta_angle = da_sign[direction] * angle * 1.0 / steps
    for i in range(steps + 1):
        theta = start + delta_angle * i
        points.append((center[0] + radius * np.cos(theta), center[1] + radius * np.sin(theta)))
    return points


def arc2(p1, p2, center, direction, steps_per_circ):
    r = np.sqrt((center[0] - p1[0]) ** 2 + (center[1] - p1[1]) ** 2)
    start = np.arctan2(p1[1] - center[1], p1[0] - center[0])
    stop = np.arctan2(p2[1] - center[1], p2[0] - center[0])
    return arc(center, r, start, stop, direction, steps_per_circ)


def arc_angle(start, stop, direction):
    if direction == "ccw" and stop <= start:
        stop += 2 * np.pi
    if direction == "cw" and stop >= start:
        stop -= 2 * np.pi
    return abs(stop - start)


def three_point_circle(p1, p2, p3):
    a1 = (p1 + p2) / 2.0
    a2 = (p2 + p3) / 2.0
    b1 = np.dot((p2 - p1), np.array([[0, -1], [1, 0]], dtype=np.float32))
    b2 = np.dot((p3 - p2), np.array([[0, 1], [-1, 0]], dtype=np.float32))
    T = solve(np.transpose(np.array([-b1, b2])), a1 - a2)
    center = a1 + b1 * T[0]
    radius = np.linalg.norm(center - p1)
    return center, radius, T[0]


out.append("// ARCHIVO GENERADO por tests/tools/gen_golden.py - no editar a mano.")
out.append(f"// Referencia: Shapely {shapely.__version__} (GEOS {shapely.geos_version_string}), numpy {np.__version__}.")
out.append("#pragma once")
out.append("#include <vector>")
out.append('#include "core/Types.h"')
out.append("namespace golden {")

# ---- buffer ------------------------------------------------------------------------------------
c = Point(1, 2).buffer(1.5, 32)
const("kCircleArea32", c.area)
for i, v in enumerate(c.bounds):
    const(f"kCircleBounds{i}", v)
const("kCircleArea8", Point(0, 0).buffer(1.0, 8).area)
const("kCircleArea128", Point(0, 0).buffer(1.0, 128).area)

rect = Polygon([(0, 0), (4, 0), (4, 3), (0, 3)])
const("kRectBufferRound", rect.buffer(0.5, 16, join_style=1).area)
const("kRectBufferMitre", rect.buffer(0.5, 16, join_style=2).area)
const("kRectBufferBevel", rect.buffer(0.5, 16, join_style=3).area)
const("kRectBufferNeg", rect.buffer(-0.3, 16).area)

line = LineString([(0, 0), (10, 0), (10, 10)])
const("kLineBufferRoundRound", line.buffer(1, 16, cap_style=1, join_style=1).area)
const("kLineBufferFlatMitre", line.buffer(1, 16, cap_style=2, join_style=2).area)
const("kLineBufferSquareBevel", line.buffer(1, 16, cap_style=3, join_style=3).area)

# ---- booleanas ---------------------------------------------------------------------------------
a = Point(0, 0).buffer(2, 64)
b = Point(1.5, 0).buffer(2, 64)
const("kUnionArea", a.union(b).area)
const("kDiffArea", a.difference(b).area)
const("kInterArea", a.intersection(b).area)
const("kSymDiffArea", a.symmetric_difference(b).area)
const("kUnaryUnionArea", unary_union([a, b, Point(10, 10).buffer(1, 64)]).area)

# ---- afines ------------------------------------------------------------------------------------
poly = Polygon([(0, 0), (4, 0), (4, 3), (1, 5), (0, 3)])
pts("kPolyRot30", list(affinity.rotate(poly, 30, origin=(1, 1)).exterior.coords))
pts("kPolyRot90", list(affinity.rotate(poly, 90, origin=(2, 2)).exterior.coords))
pts("kPolySkew", list(affinity.skew(poly, 10, 20, origin=(1, 1)).exterior.coords))
pts("kPolyScale", list(affinity.scale(poly, 2, 0.5, origin=(1, 1)).exterior.coords))
pts("kPolyMirrorX", list(affinity.scale(poly, 1.0, -1.0, origin=(0, 1)).exterior.coords))
pts("kPolyScaleCenter", list(affinity.scale(poly, 1.5, 1.5, origin="center").exterior.coords))

# ---- arcos -------------------------------------------------------------------------------------
pts("kArcCcw", arc((1, 2), 3, 0.3, 2.0, "ccw", 64))
pts("kArcCw", arc((0, 0), 2, 2.0, 0.3, "cw", 50))
pts("kArcWrapCcw", arc((0, 0), 1, 3.0, 1.0, "ccw", 40))
pts("kArcWrapCw", arc((0, 0), 1, 1.0, 3.0, "cw", 40))
pts("kArc2", arc2((3, 0), (0, 3), (0, 0), "ccw", 32))
const("kArcAngleCcw", arc_angle(0.3, 2.0, "ccw"))
const("kArcAngleCw", arc_angle(2.0, 0.3, "cw"))
const("kArcAngleWrap", arc_angle(3.0, 1.0, "ccw"))
cen, rad, t = three_point_circle(np.array([0.0, 0.0]), np.array([2.0, 0.0]), np.array([1.0, 1.0]))
const("kCircle3Cx", cen[0]); const("kCircle3Cy", cen[1]); const("kCircle3R", rad); const("kCircle3T", t)
cen, rad, t = three_point_circle(np.array([1.0, 2.0]), np.array([4.0, 6.0]), np.array([7.0, 1.0]))
const("kCircle3bCx", cen[0]); const("kCircle3bCy", cen[1]); const("kCircle3bR", rad); const("kCircle3bT", t)

# ---- Geometry (contenedor) ---------------------------------------------------------------------
g_rect = Polygon([(0, 0), (10, 0), (10, 5), (0, 5)])
g_circ = Point(20, 2.5).buffer(2, 32)
allg = [g_rect, g_circ]
for i, v in enumerate(unary_union(allg).bounds):
    const(f"kGeomBounds{i}", v)
mir = [affinity.scale(g, -1.0, 1.0, origin=(5, 0)) for g in allg]
for i, v in enumerate(unary_union(mir).bounds):
    const(f"kGeomMirrorYBounds{i}", v)
rot = [affinity.rotate(g, 90, origin=(0, 0)) for g in allg]
for i, v in enumerate(unary_union(rot).bounds):
    const(f"kGeomRot90Bounds{i}", v)
buf = [g.buffer(0.25, 32, join_style=1) for g in allg]
const("kGeomBufferAreaRect", buf[0].area)
const("kGeomBufferAreaCirc", buf[1].area)
fac = [affinity.scale(g, 1.2, 1.2, origin="center") for g in allg]
const("kGeomFactorAreaRect", fac[0].area)
const("kGeomFactorAreaCirc", fac[1].area)

out.append("}  // namespace golden")
path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "golden_data.h")
open(path, "w").write("\n".join(out) + "\n")
print("escrito", os.path.normpath(path), len(out), "líneas")
