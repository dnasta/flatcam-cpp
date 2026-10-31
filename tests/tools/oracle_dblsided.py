#!/usr/bin/env python3
"""Oráculo de los agujeros de alineación de "PCB a 2 caras": transcribe `ToolDblSided.on_create_alignment_holes`.

Usa `shapely.affinity.scale` con origen en el punto del eje, igual que el original, así que los reflejos coinciden bit a bit.
Genera `tests/golden_dblsided.h`.   Uso: python tests/tools/oracle_dblsided.py
"""
import os
import sys

from shapely import affinity
from shapely.geometry import Point

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import ROOT  # noqa: E402
from oracle_geometry_modes import cs, num  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_dblsided.h")

# nombre, eje de alineación, punto del eje, agujeros pedidos
SCENARIOS = [
    ("user_alignment_axis_x_origin", "X", (0.0, 0.0), [(-38.0, 39.5), (38.0, 39.5)]),
    ("axis_y_origin", "Y", (0.0, 0.0), [(-38.0, 39.5), (38.0, 39.5)]),
    ("axis_x_offset_point", "X", (3.3, -1.7), [(0.1, 0.3), (5.5, 7.7), (-12.345, 4.0)]),
    ("axis_y_offset_point", "Y", (-2.25, 8.125), [(1.0, 2.0), (30.7, -4.4)]),
    ("single_hole", "X", (10.0, 10.0), [(12.5, 14.5)]),
    ("hole_on_the_axis", "Y", (5.0, 5.0), [(5.0, 9.0)]),
]


def alignment(axis, pivot, holes):
    xscale, yscale = {"X": (1.0, -1.0), "Y": (-1.0, 1.0)}[axis]
    drills = []
    for hole in holes:
        point = Point(hole)
        mirror = affinity.scale(point, xscale, yscale, origin=pivot)
        drills += [point, mirror]
    return [(p.x, p.y) for p in drills]


def main():
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_dblsided.py - no editar a mano.",
           "// Referencia: ToolDblSided.on_create_alignment_holes (FlatCAM 2024.4), Shapely %s." % __import__("shapely").__version__,
           "#pragma once", "#include <vector>", '#include "core/Types.h"', "namespace golden_dblsided {",
           "struct Scenario { const char* name; flatcam::Axis axis; flatcam::Point2D pivot;"
           " std::vector<flatcam::Point2D> holes; std::vector<flatcam::Point2D> drills; };",
           "inline const std::vector<Scenario> kScenarios = {"]
    for name, axis, pivot, holes in SCENARIOS:
        drills = alignment(axis, pivot, holes)
        pts = lambda ps: "{" + ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in ps) + "}"
        out.append("{%s, flatcam::Axis::%s, {%s, %s}, %s, %s}," % (cs(name), axis, num(pivot[0]), num(pivot[1]), pts(holes), pts(drills)))
        print("%-30s %s" % (name, drills))
    out += ["};", "}  // namespace golden_dblsided"]
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
