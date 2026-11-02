#!/usr/bin/env python3
"""Oráculo del método "Líneas láser" de Paint: ejecuta `ToolPaint.paint_polygon_worker` ORIGINAL (método 3).

No transcribe nada: toma `paint_polygon_worker` del fuente con `ast` y lo ejecuta sobre una `camlib.Geometry` (que trae
`fill_with_lines`, `clear_polygon2`, `clear_polygon3` y `paint_connect` originales) con Gerbers leídos por el parser
original. Los polígonos son los de `paint_poly_all` (la geometría sólida aplanada, achicada el margen 0) y el resultado
es la concatenación de lo que pinta cada uno, como `final_solid_geometry`. Necesita Shapely 1.8.4 y `rtree`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_laser_lines.py
"""
import ast
import logging
import os
import sys
import tempfile

from shapely.geometry import LinearRing, LineString, MultiPolygon, Point, Polygon
from shapely.ops import linemerge, unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs, num  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_laser_lines.h")

HEAD = "%FSLAX46Y46*%\n%MOMM*%\n%LPD*%\n"
# Trazos que se cruzan, destellos C, R y O sobre ellos, una región (apertura de tamaño 0) y un trazo suelto.
MIXED = HEAD + """%ADD10C,0.400000*%
%ADD11R,1.200000X0.800000*%
%ADD12O,1.000000X1.600000*%
%ADD13C,1.000000*%
%ADD14C,0.300000*%
D10*
X0Y0D02*
X5000000Y0D01*
X5000000Y3000000D01*
D11*
X0Y0D03*
D13*
X5000000Y3000000D03*
D12*
X10000000Y0D03*
D10*
X10000000Y0D02*
X10000000Y4000000D01*
X7000000Y4000000D01*
D14*
X0Y6000000D02*
X3000000Y8000000D01*
X6000000Y6000000D01*
G36*
X12000000Y0D02*
X14000000Y0D01*
X14000000Y2000000D01*
X12000000Y2000000D01*
X12000000Y0D01*
G37*
M02*
"""
# Una macro (sin "size" en el original: KeyError, ningún polígono se pinta).
MACRO = HEAD + """%AMBOX*
21,1,1.0,0.6,0,0,0*%
%ADD10C,0.400000*%
%ADD20BOX*%
D10*
X0Y0D02*
X5000000Y0D01*
D20*
X5000000Y0D03*
M02*
"""
# Un único trazo recto: las líneas se unen en una sola y `linemerge` no la acepta.
SINGLE = HEAD + """%ADD10C,0.300000*%
D10*
X0Y0D02*
X4000000Y0D01*
M02*
"""

# nombre, contenido (o ruta), diámetro, solape, unir, contorno
SCENARIOS = [
    ("mixed", MIXED, 0.1, 0.2, True, True),
    ("mixed_no_connect", MIXED, 0.1, 0.2, False, True),
    ("mixed_no_contour", MIXED, 0.08, 0.5, True, False),
    ("macro_fails", MACRO, 0.1, 0.2, True, True),
    ("single_trace", SINGLE, 0.1, 0.2, True, True),
    ("single_trace_no_connect", SINGLE, 0.1, 0.2, False, False),
    ("user_logo", os.path.join(DATA, "luz_de_noche", "luz de noche Pascual-Logo.gbr"), 0.1, 0.2, True, True),
]


def load_worker():
    path = os.path.join(REF, "appTools", "ToolPaint.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolPaint")
    body = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "paint_polygon_worker"]
    module = ast.Module(body=body, type_ignores=[])
    ast.fix_missing_locations(module)
    return module, path


def flatten(geometry):
    """`recurse` de `paint_poly_all`: polígonos simples; los anillos pasan a polígonos."""
    out = []

    def rec(g):
        if isinstance(g, (list, tuple)):
            for x in g:
                rec(x)
        elif hasattr(g, "geoms"):
            for x in g.geoms:
                if not x.is_empty and x.is_valid:
                    rec(x)
        elif isinstance(g, LinearRing):
            out.append(Polygon(g))
        elif not g.is_empty and g.is_valid:
            out.append(g)
    rec(geometry)
    return out


def fix_zero_offset():
    """GEOS 3.11.0 (la del oráculo) devuelve vacío el desplazamiento 0 de una línea de más de dos puntos (con dos puntos
    devuelve la línea); desde GEOS 3.12 devuelve la línea. Se emula la GEOS corregida: si no, `fill_with_lines` pierde la
    pasada central de cada trazo con esquinas."""
    orig = LineString.parallel_offset

    def parallel_offset(self, distance, side="right", resolution=16, join_style=1, mitre_limit=5.0):
        if distance == 0:
            return LineString(self.coords)
        return orig(self, distance, side, resolution, join_style, mitre_limit)
    LineString.parallel_offset = parallel_offset


def main():
    fix_zero_offset()
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    parse_gerber = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.defaults.update({"geometry_circle_steps": 64})
    app.log = logging.getLogger("oracle")
    camlib.Geometry.app = app
    parse_gerber.Gerber.app = app

    module, path = load_worker()

    class Grace(Exception):
        pass
    ns = dict(Point=Point, FlatCAMRTreeStorage=camlib.FlatCAMRTreeStorage, linemerge=linemerge,
              unary_union=unary_union, log=logging.getLogger("paint"), grace=Grace, _=lambda s: s)
    exec(compile(module, path, "exec"), ns)

    class Worker(camlib.Geometry):
        paint_polygon_worker = ns["paint_polygon_worker"]

    worker = Worker(geo_steps_per_circle=64)
    worker.app = app
    worker.circle_steps = 64

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_laser_lines.py - no editar a mano.",
           "// Referencia: ToolPaint.paint_polygon_worker original (método Líneas láser) y Geometry.fill_with_lines.",
           "#pragma once", "#include <vector>", '#include "core/Shape.h"', "namespace golden_laser_lines {",
           "using Lines = std::vector<std::vector<flatcam::Point2D>>;",
           "struct Scenario { const char* name; const char* gerber; bool isPath; double dia; double overlap;"
           " bool connect; bool contour; int polygons; int painted; int nLines; int nPoints; double length;"
           " bool hasLines; Lines lines; };",
           "inline const std::vector<Scenario> kScenarios = {"]
    for name, src, dia, over, conn, cont in SCENARIOS:
        is_path = os.path.exists(src)
        if is_path:
            gpath = src
        else:
            fd, gpath = tempfile.mkstemp(suffix=".gbr")
            with os.fdopen(fd, "w") as f:
                f.write(src)
        g = parse_gerber.Gerber(steps_per_circle=64)
        g.parse_file(gpath)
        if not is_path:
            os.remove(gpath)
        polys = [p.buffer(-0.0) for p in flatten(g.solid_geometry)]
        lines, painted = [], 0
        for pp in polys:
            res = worker.paint_polygon_worker(pp, tooldiameter=dia, paint_method=3, over=over, conn=conn, cont=cont,
                                              prog_plot=False, obj=g)
            if res:
                painted += 1
                lines += [x for x in res.get_objects() if x and not x.is_empty]
        print("%-24s %3d polígonos, %3d pintados, %4d trazados, %6d puntos" % (
            name, len(polys), painted, len(lines), sum(len(l.coords) for l in lines)), flush=True)
        # Con muchos trazados (el Logo del usuario) solo se guardan los totales.
        full = len(lines) <= 2000
        ls = "{" + ", ".join("{" + ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in l.coords) + "}"
                             for l in lines) + "}" if full else "{}"
        gerber = os.path.relpath(src, DATA) if is_path else src
        out.append('{"%s", %s, %s, %s, %s, %s, %s, %d, %d, %d, %d, %s, %s, %s},' % (
            name, cs(gerber), str(is_path).lower(), num(dia), num(over), str(conn).lower(), str(cont).lower(),
            len(polys), painted, len(lines), sum(len(l.coords) for l in lines), num(sum(l.length for l in lines)),
            str(full).lower(), ls))
    out += ["};", "}  // namespace golden_laser_lines"]
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
