#!/usr/bin/env python3
"""Oráculo del editor de Geometría: toma con `ast` de `appEditors/AppGeoEditor.py` las herramientas de dibujo
(`FCCircle`, `FCArc`, `FCRectangle`, `FCPolygon`, `FCPath`, `FCMove`, `FCCopy`, `FCExplode`, `FCEraser`),
`DrawToolShape` (sus transformaciones) y los métodos de `AppGeoEditor` que trabajan sobre las figuras (`flatten`,
`on_shape_complete`, `union`, `intersection`, `subtract`, `cutpath`, `paint`, `delete_selected`, `update_fcgeometry`) y
los ejecuta en secuencia sobre un almacén real (`FlatCAMRTreeStorage` con `rtree`). Después de cada paso guarda las
figuras del almacén (WKT) y cuáles están elegidas, en `tests/golden_geo_editor.h`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_geo_editor.py
"""
import ast
import logging
import os
import sys
from copy import deepcopy
from unittest import mock

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_geo_editor.h")
# Lo que se abre en el editor: un cuadrado con un agujero (horario), una línea, un polígono horario y un anillo.
INPUT = [
    "POLYGON ((0 0, 0 10, 10 10, 10 0, 0 0), (2 2, 8 2, 8 8, 2 8, 2 2))",
    "LINESTRING (12 0, 20 5, 25 0)",
    "POLYGON ((30 0, 30 6, 36 6, 36 0, 30 0))",
    "LINEARRING (40 0, 44 0, 44 4, 40 4, 40 0)",
]
OPS = [
    ("circle", [50, 50, 53, 54], ""),
    ("arc", [0, 20, 5, 20, 0, 25], "c12 cw"),
    ("arc", [0, 30, 10, 30, 5, 35], "132 cw"),
    ("arc", [20, 30, 30, 30, 25, 33], "12c ccw"),
    ("arc", [20, 40, 30, 40, 25, 37], "12c cw"),
    ("rect", [60, 0, 65, 8], ""),
    ("polygon", [70, 0, 75, 0, 75, 5, 72, 8], ""),
    ("path", [80, 0, 85, 5, 90, 0, 95, 5], ""),
    ("click", [9.9, 9.8, 0], ""),            # el exterior del cuadrado
    ("click", [44.1, 4.2, 1], ""),           # suma el anillo
    ("click", [44.1, 4.2, 1], ""),           # y lo saca
    ("area", [-1, -1, 37, 11, 1, 0], ""),    # de izquierda a derecha: contenidas
    ("area", [37, 11, 29, 5, 0, 1], ""),     # de derecha a izquierda: las que toca (suma)
    ("select", [0, 3], ""),
    ("move", [0, 0, 100, 0], ""),            # anillos: salen como polígonos invertidos
    ("select", [1, 2], ""),
    ("copy", [0, 0, 0, 50], ""),             # con una línea: la lista queda igual
    ("select", [7, 8], ""),
    ("union", [], ""),
    ("select", [6, 9], ""),
    ("intersection", [], ""),
    ("select", [2, 5], ""),
    ("subtract", [], ""),
    ("rect", [102, -2, 108, 12], ""),
    ("select", [12, 13], ""),
    ("cutpath", [], ""),
    ("select", [5], ""),
    ("explode", [], ""),
    ("circle", [0, -20, 0, -15], ""),
    ("select", [18], ""),
    ("erase", [0, -20, 2, -20], ""),
    ("rect", [0, -40, 10, -30], ""),
    ("select", [19], ""),
    ("paint", [0.5, 0.2, 0.0, 1, 1], "Seed"),
    ("paint", [0.5, 0.4, 0.2, 1, 0], "Standard"),
    ("paint", [0.8, 0.3, 0.0, 0, 1], "Lines"),
    ("select", [19, 4], ""),
    ("rotate", [30, 5, -35], ""),
    ("mirror", [5, -35], "X"),
    ("mirror", [5, -35], "Y"),
    ("skew", [10, 5, 0, 0], ""),
    ("scale", [2, 0.5, 1, 1], ""),
    ("offset", [3, -4], ""),
    ("buffer", [0.5, 1, 0], ""),
    ("buffer", [0.5, 2, 0], ""),
    ("buffer", [1.2, 1, 1], ""),
    ("milling", [], "cv"),
    ("circle", [-50, -50, -47, -50], ""),
    ("select", [23], ""),
    ("move", [0, 0, -10, 0], ""),
    ("milling", [], "cl"),
    ("select", [0], ""),
    ("explode", [], ""),                     # tiene agujeros: falla
    ("select", [3, 20], ""),
    ("delete", [], ""),
    ("result", [], ""),
]
CLASSES = ("DrawToolShape", "DrawToolUtilityShape", "DrawTool", "FCShapeTool", "FCCircle", "FCArc", "FCRectangle",
           "FCPolygon", "FCPath", "FCMove", "FCCopy", "FCExplode", "FCEraser")
METHODS = ("flatten", "on_shape_complete", "union", "intersection", "subtract", "cutpath", "paint", "add_shape",
           "delete_shape", "delete_selected", "delete_utility_geometry", "get_selected", "update_fcgeometry")


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    from shapely import wkt as W
    import shapely.affinity as affinity
    from shapely.geometry import LinearRing, LineString, MultiLineString, MultiPolygon, Point, Polygon
    from shapely.geometry.polygon import orient
    from shapely.ops import linemerge, unary_union
    path = os.path.join(REF, "appEditors", "AppGeoEditor.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    body = []
    for n in tree.body:
        if isinstance(n, ast.ClassDef) and n.name in CLASSES:
            body.append(n)
        elif isinstance(n, ast.FunctionDef) and n.name in ("distance", "mag", "poly2rings", "get_shapely_list_bounds"):
            body.append(n)
    ed = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "AppGeoEditor")
    meths = [f for f in ed.body if isinstance(f, ast.FunctionDef) and f.name in METHODS]
    body.append(ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[]))
    m = ast.Module(body=body, type_ignores=[])
    ast.fix_missing_locations(m)
    qt = mock.MagicMock()
    ns = dict(QtGui=qt, QtCore=qt, QtWidgets=qt, Qt=qt, arc=camlib.arc, three_point_circle=camlib.three_point_circle,
              Geometry=camlib.Geometry, FlatCAMRTreeStorage=camlib.FlatCAMRTreeStorage, LineString=LineString,
              LinearRing=LinearRing, MultiLineString=MultiLineString, Polygon=Polygon, MultiPolygon=MultiPolygon,
              Point=Point, unary_union=unary_union, linemerge=linemerge, affinity=affinity, orient=orient, np=np,
              numpy_norm=np.linalg.norm, deepcopy=deepcopy, _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    Core = ns["Core"]
    Core.AppGeoEditor = Core
    ns["AppGeoEditor"] = Core
    Core.draw_shape_idx = -1
    app = FakeApp()
    app.defaults.update({"geometry_circle_steps": 64, "geometry_editor_milling_type": "cl",
                         "geometry_editor_sel_limit": 30, "global_mselect_key": "Control"})
    app.log = logging.getLogger("oracle")
    app.jump_signal = mock.MagicMock()
    app.ui = mock.MagicMock()
    core = Core()
    core.launched_from_shortcuts = False
    core.app = app
    core.storage = camlib.FlatCAMRTreeStorage()
    core.storage.get_points = ns["DrawToolShape"].get_pts
    core.selected, core.utility, core.flat_geo = [], [], []
    core.tool_shape = mock.MagicMock()
    core.build_ui = core.replot = core.plot_all = lambda *a, **k: None
    core.select_tool = lambda *a, **k: None
    core.multigeo_tool = None
    DTS = ns["DrawToolShape"]
    DTS.tolerance = None

    # edit_fcgeometry: flatten con orient 1 (husillo CW y fresado por ascenso), polígonos partidos en anillos.
    geo = [W.loads(w) for w in INPUT]
    for shape in core.flatten(geometry=geo, orient_val=1):
        if type(shape) == Polygon:
            core.add_shape(DTS(shape.exterior))
            for inter in shape.interiors:
                core.add_shape(DTS(inter))
        else:
            core.add_shape(DTS(shape))

    d = lambda x: W.dumps(x, rounding_precision=17, trim=True)  # noqa: E731
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_geo_editor.py - no editar a mano.",
           "// Referencia: appEditors/AppGeoEditor.py (herramientas de dibujo y operaciones) original.",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_geo_editor {",
           "inline const std::vector<std::string> kInput = {%s};" % ", ".join(cs(w) for w in INPUT),
           "struct Step { const char* op; std::vector<double> args; const char* text; bool ok;"
           " std::vector<std::string> shapes; std::vector<int> selected; std::vector<std::string> result; };",
           "inline const std::vector<Step> kSteps = {"]

    def objs():
        return list(core.storage.get_objects())

    def run_tool(cls, points):
        t = ns[cls](core)
        t.points = [tuple(p) for p in points]
        t.make()
        core.active_tool = t
        core.on_shape_complete()

    for op, args, text in OPS:
        ok, result = True, []
        try:
            pts = [(args[i], args[i + 1]) for i in range(0, len(args) - 1, 2)]
            if op == "circle":
                run_tool("FCCircle", pts)
            elif op == "arc":
                t = ns["FCArc"](core)
                t.mode, t.direction = text.split()
                t.points = pts
                t.make()
                core.active_tool = t
                core.on_shape_complete()
            elif op == "rect":
                run_tool("FCRectangle", pts)
            elif op == "polygon":
                run_tool("FCPolygon", pts)
            elif op == "path":
                run_tool("FCPath", pts)
            elif op == "select":
                o = objs()
                core.selected = [o[i] for i in args]
            elif op == "click":
                qt.QtWidgets.QApplication.keyboardModifiers.return_value = qt.Qt.ControlModifier if args[2] else 0
                ns["QtWidgets"].QApplication.keyboardModifiers.return_value = \
                    ns["QtCore"].Qt.ControlModifier if args[2] else 0
                # FCSelect.click_release (lo que hace con el más cercano; recorre el almacén entero).
                over = [core.storage.nearest(tuple(args[:2]))[1] for _ in core.storage.get_objects()]
                if not over:
                    core.selected = []
                else:
                    obj = over[0]
                    if args[2]:
                        if obj in core.selected:
                            core.selected.remove(obj)
                        else:
                            core.selected.append(obj)
                    else:
                        core.selected = [obj]
            elif op == "area":
                a, b = (args[0], args[1]), (args[2], args[3])
                poly = Polygon([a, (b[0], a[1]), b, (a[0], b[1])])
                lst = [o for o in objs() if (args[4] and poly.contains(o.geo)) or (not args[4] and poly.intersects(o.geo))]
                if args[5]:
                    for o in lst:
                        if o in core.selected:
                            core.selected.remove(o)
                        else:
                            core.selected.append(o)
                else:
                    core.selected = lst
            elif op in ("move", "copy"):
                t = ns["FCMove" if op == "move" else "FCCopy"](core)
                t.origin, t.destination = pts[0], pts[1]
                t.make()
                core.active_tool = t
                core.on_shape_complete()
            elif op == "erase":
                t = ns["FCEraser"](core)
                t.origin, t.destination = pts[0], pts[1]
                t.make()
            elif op == "explode":
                ns["FCExplode"](core)
            elif op == "paint":
                core.paint(args[0], args[1], args[2], connect=bool(args[3]), contour=bool(args[4]), method=text)
            elif op == "rotate":
                for s in core.selected:
                    s.rotate(args[0], point=(args[1], args[2]))
            elif op == "mirror":
                for s in core.selected:
                    s.mirror(text, (args[0], args[1]))
            elif op == "skew":
                for s in core.selected:
                    s.skew(args[0], args[1], point=(args[2], args[3]))
            elif op == "scale":
                for s in core.selected:
                    s.scale(args[0], args[1], point=(args[2], args[3]))
            elif op == "offset":
                for s in core.selected:
                    s.offset((args[0], args[1]))
            elif op == "buffer":
                for s in core.selected:
                    s.buffer(args[0], int(args[1]), bool(args[2]))
            elif op == "milling":
                app.defaults["geometry_editor_milling_type"] = text
            elif op == "delete":
                core.delete_selected()
            elif op == "result":
                fc = mock.MagicMock()
                fc.solid_geometry = []
                core.deactivate = lambda: None
                core.update_fcgeometry(fc)
                result = [d(g) for g in fc.solid_geometry]
            else:
                getattr(core, op)()
        except Exception as e:  # noqa: BLE001  (el original falla: solo queda en el registro)
            ok = False
            print("  falla:", op, type(e).__name__, e)
        o = objs()
        sel = [o.index(s) for s in core.selected if s in o]
        out.append("{\"%s\", {%s}, \"%s\", %s, {%s}, {%s}, {%s}}," % (
            op, ", ".join(repr(float(a)) for a in args), text, "true" if ok else "false",
            ", ".join(cs(d(s.geo)) for s in o), ", ".join(str(i) for i in sel), ", ".join(cs(r) for r in result)))
        print(op, text, ok, len(o), sel, [s.geo.geom_type[:4] for s in o])
    out += ["};", "}  // namespace golden_geo_editor"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
