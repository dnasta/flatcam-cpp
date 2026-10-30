#!/usr/bin/env python3
"""Oráculo de las transformaciones de Gerber (scale, offset, mirror, rotate, skew, buffer).

Parsea cada archivo con el parser ORIGINAL de FlatCAM, le aplica la operación original y resume el resultado en
`tests/golden_gerber_ops.h`. Los tests de C++ aplican la misma operación a su propio Gerber y comparan.

Requisitos (solo para regenerar el archivo): python3 -m pip install shapely numpy
Uso:  python3 tests/tools/oracle_gerber_ops.py
"""
import os
import sys

from shapely.ops import unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_gerber import flat_polys  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_gerber_ops.h")
FILES = ["PCB principal-Edge_Cuts.gm1", "PCB principal-B_Logo.gbr", "PCB principal-B_Mask.gbs"]

# (nombre, función que aplica la operación original)
OPS = [
    ("scale", lambda g: g.scale(1.5, 0.8, (1, 2))),
    ("offset", lambda g: g.offset((3, -4))),
    ("mirror_y", lambda g: g.mirror("Y", (0, 0))),
    ("mirror_x", lambda g: g.mirror("X", (1, 1))),
    ("rotate", lambda g: g.rotate(30, (2, 3))),
    ("skew", lambda g: g.skew(10, 20, (0, 0))),
    ("buffer", lambda g: g.buffer(0.2, 1, None)),
    ("buffer_factor", lambda g: g.buffer(1.5, 1, True)),
]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    num = lambda v: repr(float(v))
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_gerber_ops.py - no editar a mano.",
           "// Referencia: transformaciones del parser de Gerber original de FlatCAM.",
           "#pragma once", "#include <vector>", "namespace golden_gerber_ops {",
           "struct Ap { const char* id; double size; double width; double height; double diam; int nSolid; };",
           "struct Case { const char* file; const char* op; int nPolygons; double area; double bounds[4]; std::vector<Ap> apertures; };",
           "inline const std::vector<Case> kCases = {"]
    for name in FILES:
        for op_name, op in OPS:
            app = FakeApp()
            pg.Gerber.app = app
            camlib.Geometry.app = app
            g = pg.Gerber(steps_per_circle=64)
            g.parse_file(os.path.join(DATA, name))
            op(g)
            polys = flat_polys(g.solid_geometry)
            u = unary_union(polys)
            aps = []
            for k, v in sorted(g.apertures.items()):
                n_solid = sum(1 for e in v.get("geometry", []) if "solid" in e)
                aps.append('{"%s", %s, %s, %s, %s, %d}' % (k, num(v.get("size", 0.0)), num(v.get("width", 0.0)),
                                                         num(v.get("height", 0.0)), num(v.get("diam", 0.0)), n_solid))
            b = u.bounds
            out.append('{"%s", "%s", %d, %s, {%s, %s, %s, %s}, {%s}},' % (
                name, op_name, len(polys), num(u.area), num(b[0]), num(b[1]), num(b[2]), num(b[3]), ", ".join(aps)))
            print("%-30s %-14s polígonos=%-4d área=%.6f" % (name, op_name, len(polys), u.area), flush=True)
    out.append("};")
    out.append("}  // namespace golden_gerber_ops")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
