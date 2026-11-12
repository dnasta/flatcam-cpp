#!/usr/bin/env python3
"""Oráculo del editor de Gerber: toma con `ast` de `appEditors/AppGerberEditor.py` la carga de `edit_fcgerber`
(`worker_job`: resta los borrados que quedan dentro de cada sólido; `add_apertures`), `PadEditorGrb.util_shape`,
`DiscSemiEditorGrb.make` y `new_edited_gerber` (el `obj_init` que arma el Gerber guardado) y los ejecuta sobre los
cobres de "luz de noche" y del dosificador. Escribe `tests/golden_grb_editor.h`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_grb_editor.py
"""
import ast
import logging
import math
import os
import sys
from copy import deepcopy
from unittest import mock

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_grb_editor.h")
FILES = ["luz_de_noche/luz de noche Pascual-B_Cu.gbr", "PCB principal-B_Cu.gbl"]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    from shapely import wkt as W
    from shapely.geometry import LineString, MultiPolygon, Point, Polygon
    from shapely.ops import unary_union
    app = FakeApp()
    app.defaults.update({"geometry_circle_steps": 64, "gerber_circle_steps": 64, "units": "MM"})
    app.log = logging.getLogger("oracle")
    pg.Gerber.app = app
    path = os.path.join(REF, "appEditors", "AppGerberEditor.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    body = [n for n in tree.body if isinstance(n, ast.ClassDef) and n.name in
            ("DrawToolShape", "DrawToolUtilityShape", "DrawTool", "ShapeToolEditorGrb", "PadEditorGrb",
             "DiscSemiEditorGrb")]
    ed = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "AppGerberEditor")
    edit = next(f for f in ed.body if isinstance(f, ast.FunctionDef) and f.name == "edit_fcgerber")
    # La clase Execute_Edit de edit_fcgerber trae el worker_job.
    exe = next(n for n in ast.walk(edit) if isinstance(n, ast.ClassDef) and n.name == "Execute_Edit")
    worker = next(f for f in exe.body if isinstance(f, ast.FunctionDef) and f.name == "worker_job")
    worker.decorator_list = []
    meths = [f for f in ed.body if isinstance(f, ast.FunctionDef) and f.name in ("add_apertures", "new_edited_gerber")]
    meths.append(worker)
    body.append(ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[]))
    m = ast.Module(body=body, type_ignores=[])
    ast.fix_missing_locations(m)
    qt = mock.MagicMock()
    ns = dict(QtGui=qt, QtCore=qt, QtWidgets=qt, Qt=qt, arc=camlib.arc, distance=camlib.distance,
              three_point_circle=camlib.three_point_circle, math=math, np=np, numpy_norm=np.linalg.norm,
              LineString=LineString, MultiPolygon=MultiPolygon, Point=Point, Polygon=Polygon,
              unary_union=unary_union, deepcopy=deepcopy, _=lambda s: s, log=logging.getLogger("x"),
              traceback=__import__("traceback"))
    exec(compile(m, path, "exec"), ns)
    Core = ns["Core"]
    Core.add_apertures = staticmethod(Core.add_apertures)
    Core.worker_job = staticmethod(Core.worker_job)
    d = lambda x: W.dumps(x, rounding_precision=17, trim=True)  # noqa: E731
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_grb_editor.py - no editar a mano.",
           "// Referencia: appEditors/AppGerberEditor.py original.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_grb_editor {",
           "struct Ap { const char* code; int count; double area; };",
           "struct Case { const char* file; std::vector<Ap> loaded; std::vector<std::string> pads;"
           " int solidCount; double solidArea; int follow; std::vector<std::string> order; };",
           "inline const std::vector<Case> kCases = {"]
    for f in FILES:
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        g.options = {"name": "grb"}
        core = Core()
        core.app = app
        core.gerber_obj = g
        core.gerber_obj_options = {"name": "grb"}
        core.storage_dict = {}
        core.results = []

        class Res:
            def __init__(self, v):
                self.v = v

            def get(self):
                return self.v
        core.pool = mock.MagicMock()
        core.pool.apply_async = lambda fn, args: Res(fn(*args))
        core.mp_finished = mock.MagicMock()
        core.worker_job(core)
        loaded = []
        for code, v in core.storage_dict.items():
            geo = v.get("geometry", [])
            loaded.append((code, len(geo), sum(e.geo["solid"].area for e in geo if "solid" in e.geo)))
        # Un pad de cada apertura C, R y O que haya.
        pads = []
        for code, v in core.storage_dict.items():
            if v["type"] in ("C", "R", "O"):
                t = ns["PadEditorGrb"].__new__(ns["PadEditorGrb"])
                t.draw_app = mock.MagicMock()
                t.draw_app.storage_dict = core.storage_dict
                t.draw_app.last_aperture_selected = code
                t.draw_app.app = app
                e = t.util_shape((1.25, -2.5))
                pads.append("%s|%s" % (code, d(e["solid"])))
        # Guardar.
        created = []

        def new_object(kind, name, init, plot=True):
            o = pg.Gerber(steps_per_circle=64)
            o.options = {}
            init(o, app)
            created.append(o)
        app.app_obj = mock.MagicMock()
        app.app_obj.new_object = new_object
        app.f_handlers = mock.MagicMock()
        app.f_handlers.export_gerber.return_value = ""
        core.deactivate_grb_editor = lambda: None
        core.new_edited_gerber("grb_edit", core.storage_dict)
        o = created[0]
        parts = list(o.solid_geometry) if isinstance(o.solid_geometry, list) else list(o.solid_geometry.geoms)
        out.append("{%s, {%s}, {%s}, %d, %r, %d, {%s}}," % (
            cs(f), ", ".join("{%s, %d, %r}" % (cs(c), n, a) for c, n, a in loaded), ", ".join(cs(p) for p in pads),
            len(parts), sum(p.area for p in parts), len(o.follow_geometry), ", ".join(cs(k) for k in o.apertures)))
        print(f, len(loaded), len(pads), len(parts), round(sum(p.area for p in parts), 4), len(o.follow_geometry))
    out += ["};"]
    # Semidiscos.
    t = ns["DiscSemiEditorGrb"].__new__(ns["DiscSemiEditorGrb"])
    t.draw_app = mock.MagicMock()
    t.buf_val = 0.2
    t.steps_per_circ = 64
    t.storage_obj = []
    semis = []
    for mode, direction in (("c12", "cw"), ("c12", "ccw"), ("132", "cw"), ("12c", "ccw")):
        t.mode, t.direction = mode, direction
        t.points = [(0.0, 0.0), (3.0, 0.0), (0.0, 3.0)] if mode == "c12" else [(0.0, 0.0), (4.0, 0.0), (2.0, 1.5)]
        t.make()
        semis.append((mode, direction, d(t.geometry.geo["solid"])))
    out.append("struct Semi { const char* mode; const char* dir; const char* wkt; };")
    out.append("inline const std::vector<Semi> kSemi = {%s};" % ", ".join(
        "{\"%s\", \"%s\", %s}" % (a, b, cs(w)) for a, b, w in semis))
    out += ["}  // namespace golden_grb_editor"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
