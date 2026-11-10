#!/usr/bin/env python3
"""Oráculo de Alinear objetos: ejecuta `AlignObjects.on_mouse_click_release`, `check_points`, `align_translate` y
`align_rotate` ORIGINALES (tomados con `ast`) y escribe `tests/golden_align.h`:
  - qué puntos agrega un clic sobre el cobre de "luz de noche" (solo los pads) o sobre su taladro;
  - el desplazamiento y el giro que resultan de los puntos elegidos (también cuando el original falla).

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_align.py
"""
import ast
import logging
import math
import os
import sys
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_align.h")
GBR = "luz_de_noche/luz de noche Pascual-B_Cu.gbr"
EXC = "luz_de_noche/luz de noche Pascual-PTH.drl"
# Puntos elegidos a mano (dos o cuatro) para probar solo el desplazamiento y el giro.
TRANSFORMS = [
    ("sp", [[0, 0], [10, 5]]),
    ("dp", [[0, 0], [10, 5], [10, 0], [20, 10]]),          # gira 45 grados alrededor de (10, 5)
    ("dp", [[1, 1], [2, 3], [5, 1], [6, 4]]),
    ("dp", [[-3.5, 2.25], [1.125, -0.5], [4, 6], [-2, 8]]),
    ("dp", [[0, 0], [0, 0], [3, 4], [3, 7]]),             # el inicio y el destino tienen la misma X: no gira
    ("dp", [[0, 0], [0, 0], [3, 4], [5, 4]]),             # la misma Y: no gira (el original usa "o")
    ("dp", [[0, 0], [1, 1], [4, 5], [1, 9]]),             # destino con la X del centro: ZeroDivisionError
    ("dp", [[0, 0], [1, 1], [1, 5], [6, 9]]),             # gira
    ("dp", [[0, 0], [0, 0], [2, 1], [1, 2]]),             # gira 36,87 grados
    ("dp", [[0, 0], [1, 1], [0, 4], [3, 2]]),             # inicio con la X del centro: ZeroDivisionError
]


def methods(path, clsname, names):
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == clsname)
    return [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in names]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.defaults.update({"excellon_circle_steps": 64})
    app.log = logging.getLogger("oracle")
    app.inform = mock.MagicMock()
    app.is_legacy = False
    app.plotcanvas = mock.MagicMock()
    app.selection_type = None
    app.event_is_dragging = False
    pg.Gerber.app = app
    pe.Excellon.app = app
    from shapely.affinity import translate
    from shapely.geometry import LineString, Point
    path = os.path.join(REF, "appTools", "ToolAlignObjects.py")
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], decorator_list=[], body=methods(
        path, "AlignObjects", {"on_mouse_click_release", "check_points", "align_translate", "align_rotate"}))],
        type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(Point=Point, translate=translate, math=math, _=lambda s: s)
    exec(compile(m, path, "exec"), ns)

    g = pg.Gerber(steps_per_circle=64)
    g.parse_file(os.path.join(DATA, GBR))
    g.kind = "gerber"
    e = pe.Excellon(geo_steps_per_circle=64)
    e.default_data = {}
    e.parse_file(os.path.join(DATA, EXC))
    e.create_geometry()
    e.kind = "excellon"
    for o in (g, e):
        o.options = {}
        o.shapes = mock.MagicMock()

    def tool(points, aligned, aligner, kind="dp"):
        c = ns["Core"]()
        c.app = app
        c.decimals = 4
        c.canvas = mock.MagicMock()
        c.canvas.translate_coords = lambda p: p
        c.clicked_points = [list(p) for p in points]
        c.align_type = kind
        c.aligned_obj, c.aligner_obj, c.target_obj = aligned, aligner, aligned
        c.set_color = c.reset_color = c.disconnect_cal_events = lambda: None
        c.calls = []
        aligned.offset = lambda v: c.calls.append(("offset", v[0], v[1]))
        aligned.rotate = lambda angle, point: c.calls.append(("rotate", angle, point[0], point[1]))
        app.plot_all = lambda: None
        return c

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_align.py - no editar a mano.",
           "// Referencia: AlignObjects.on_mouse_click_release / check_points / align_translate / align_rotate.",
           "#pragma once", "#include <vector>", "namespace golden_align {",
           "struct Pick { const char* target; double x; double y; std::vector<double> points; };",
           "struct Transform { const char* type; std::vector<double> points; bool fails; double dx; double dy;"
           " bool rotates; double angle; double ox; double oy; };"]
    # Clics: en un pad, apenas corrido, en un trazo, afuera; en un agujero y apenas corrido.
    clicks = []
    pads = [el["follow"] for ap in g.apertures.values() for el in ap.get("geometry", [])
            if isinstance(el.get("follow"), Point)]
    lines = [el["follow"] for ap in g.apertures.values() for el in ap.get("geometry", [])
             if isinstance(el.get("follow"), LineString)]
    for p in pads[:4] + pads[40:42]:
        clicks += [("gerber", p.x, p.y), ("gerber", p.x + 0.05, p.y - 0.03)]
    mid = lines[3].interpolate(0.5, normalized=True)
    clicks += [("gerber", mid.x, mid.y), ("gerber", 1000.0, 1000.0)]
    for t in list(e.tools.values())[:4]:
        d = t["drills"][0]
        clicks += [("excellon", d.x, d.y), ("excellon", d.x + 0.1, d.y + 0.1)]
    clicks += [("excellon", 1000.0, 1000.0)]
    out.append("inline const std::vector<Pick> kPicks = {")
    for kind, x, y in clicks:
        obj = g if kind == "gerber" else e
        c = tool([], obj, obj)
        c.check_points = lambda: None
        ev = mock.MagicMock()
        ev.pos, ev.button = (x, y), 1
        c.on_mouse_click_release(ev)
        pts = [v for p in c.clicked_points for v in p]
        out.append("{\"%s\", %r, %r, {%s}}," % (kind, x, y, ", ".join(repr(v) for v in pts)))
    out.append("};")
    out.append("inline const std::vector<Transform> kTransforms = {")
    for kind, pts in TRANSFORMS:
        c = tool(pts, g, g, kind)
        fails = False
        try:
            c.align_translate()
            if kind == "dp":
                c.align_rotate()
        except ZeroDivisionError:
            fails = True
        off = next(x for x in c.calls if x[0] == "offset")
        rot = next((x for x in c.calls if x[0] == "rotate"), None)
        out.append("{\"%s\", {%s}, %s, %r, %r, %s, %r, %r, %r}," % (
            kind, ", ".join(repr(float(v)) for p in pts for v in p), "true" if fails else "false", off[1], off[2],
            "true" if rot else "false", rot[1] if rot else 0.0, float(rot[2]) if rot else 0.0,
            float(rot[3]) if rot else 0.0))
        print(kind, pts, fails, c.calls)
    out += ["};", "}  // namespace golden_align"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
