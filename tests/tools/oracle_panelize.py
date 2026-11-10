#!/usr/bin/env python3
"""Oráculo de Panelizar: ejecuta `Panelize.on_panelize` ORIGINAL (ToolPanelize.py, tomado con `ast`) con una interfaz
simulada sobre un Gerber, Excellon y una Geometría, y escribe en `tests/golden_panelize.h` el objeto que crea: su tipo,
su nombre, las filas y columnas usadas, la geometría (WKT con 17 cifras), las aperturas o los agujeros y ranuras, y el
último mensaje.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_panelize.py
"""
import ast
import contextlib
import logging
import os
import sys
from copy import deepcopy
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_panelize.h")
EDGE = "luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr"
LOGO = "luz_de_noche/luz de noche Pascual-Logo.gbr"
PTH = "luz_de_noche/luz de noche Pascual-PTH.drl"
SLOTS = "excellon_samples/routed_slots.drl"
# (nombre, origen, columnas, filas, espacio X, espacio Y, tipo de panel, optimizar, limitar, ancho, alto)
CASES = [
    ("gerber_2x3", ("gerber", EDGE), 2, 3, 2.0, 1.0, "gerber", True, False, 0, 0),
    ("gerber_as_geometry", ("gerber", EDGE), 2, 1, 0.0, 0.0, "geometry", True, False, 0, 0),
    ("gerber_constrained", ("gerber", EDGE), 4, 4, 2.0, 2.0, "gerber", True, True, 200.0, 120.0),
    ("excellon_2x2", ("excellon", PTH), 2, 2, 5.0, 5.0, "gerber", True, False, 0, 0),
    ("excellon_slots", ("excellon", SLOTS), 2, 1, 1.0, 0.0, "gerber", True, False, 0, 0),
    ("geometry_opt", ("geometry", None), 2, 2, 0.0, 0.0, "geometry", True, False, 0, 0),
    ("geometry_noopt", ("geometry", None), 2, 2, 0.0, 0.0, "geometry", False, False, 0, 0),
    ("geometry_spaced", ("geometry", None), 3, 1, 2.5, 0.0, "geometry", True, False, 0, 0),
]


def load_on_panelize(ns):
    path = os.path.join(REF, "appTools", "ToolPanelize.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "Panelize")
    meth = next(f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "on_panelize")
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=[meth], decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    exec(compile(m, path, "exec"), ns)
    return ns["Core"]


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v

    def isChecked(self):
        return bool(self.v)

    def set_value(self, v):
        self.v = v

    def currentText(self):
        return self.v


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.defaults.update({"excellon_circle_steps": 64})
    app.log = logging.getLogger("oracle")
    pg.Gerber.app = app
    pe.Excellon.app = app
    camlib.Geometry.app = app
    import numpy as np
    import shapely.affinity as affinity
    from shapely import wkt as W
    from shapely.geometry import LineString, MultiLineString
    from shapely.ops import linemerge, snap, unary_union
    ns = dict(deepcopy=deepcopy, np=np, affinity=affinity, unary_union=unary_union, linemerge=linemerge, snap=snap,
              LineString=LineString, MultiLineString=MultiLineString, log=logging.getLogger("x"), _=lambda s: s,
              grace=Exception)
    Core = load_on_panelize(ns)
    messages = []
    app.inform.emit = lambda m: messages.append(m)
    app.proc_container.new = lambda *a, **k: contextlib.nullcontext()
    app.delete_selection_shape = lambda: None
    app.export = mock.MagicMock()
    app.export.export_excellon.return_value = ""
    app.f_handlers = mock.MagicMock()
    app.f_handlers.export_gerber.return_value = ""
    app.f_handlers.export_dxf.return_value = ""
    app.worker_task = mock.MagicMock()
    app.worker_task.emit = lambda d: d["fcn"](*d["params"])
    created = []

    def new_object(kind, name, init, plot=True, autoselected=True):
        if kind == "excellon":
            o = pe.Excellon(geo_steps_per_circle=64)
            o.default_data = {}
        elif kind == "gerber":
            o = pg.Gerber(steps_per_circle=64)
        else:
            o = camlib.Geometry(geo_steps_per_circle=64)
            o.tools = {}
            o.multigeo = False
        o.kind = kind
        o.options = {"name": name}
        init(o, app)
        created.append((kind, name, o))
    app.app_obj = mock.MagicMock()
    app.app_obj.new_object = new_object
    app.app_obj.inform = app.inform
    app.app_obj.abort_flag = False
    app.app_obj.proc_container = app.proc_container
    objects = {}
    app.collection = mock.MagicMock()
    app.collection.get_by_name = lambda n: objects.get(n)

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_panelize.py - no editar a mano.",
           "// Referencia: Panelize.on_panelize original (ToolPanelize.py).",
           "#pragma once", "#include <map>", "#include <string>", "#include <vector>", "namespace golden_panelize {",
           "struct ApGeo { const char* solid; const char* follow; const char* clear; };",
           "struct PanelCase { const char* name; const char* kind; const char* file; int columns; int rows;"
           " double spacingX; double spacingY; const char* panelType; bool optimize; bool constrain; double cx;"
           " double cy; const char* outKind; const char* outName; const char* message;"
           " std::vector<std::string> solid; std::map<std::string, std::vector<ApGeo>> apertures;"
           " std::map<int, std::vector<std::string>> tools; };",
           "inline const std::vector<PanelCase> kCases = {"]
    for (name, (kind, f), cols, rows, sx, sy, ptype, opt, constr, cx, cy) in CASES:
        if kind == "gerber":
            src = pg.Gerber(steps_per_circle=64)
            src.parse_file(os.path.join(DATA, f))
        elif kind == "excellon":
            src = pe.Excellon(geo_steps_per_circle=64)
            src.default_data = {}  # create_geometry lo copia en cada herramienta (sin él se corta el bucle)
            src.parse_file(os.path.join(DATA, f))
            src.create_geometry()
        else:
            # Un corte de 10 x 10 con dos puentes (como el de CutOut): tres tramos por herramienta, y un punto.
            src = camlib.Geometry(geo_steps_per_circle=64)
            src.multigeo = True
            lines = [LineString([(4, 0), (10, 0), (10, 10), (6, 10)]), LineString([(4, 10), (0, 10), (0, 0), (2, 0)]),
                     LineString([(2, 0), (4, 0)])]
            src.tools = {1: {"tooldia": 1.2, "solid_geometry": lines},
                         9999: {"tooldia": 1.2, "solid_geometry": [LineString([(4, 10), (6, 10)])]}}
            src.solid_geometry = lines + [LineString([(4, 10), (6, 10)])]
        src.kind = kind
        src.options = {"name": name}
        objects[name] = src
        core = Core()
        core.app = app
        core.constrain_flag = False
        core.ui = mock.MagicMock()
        core.ui.object_combo = Val(name)
        core.ui.box_combo = Val("")
        core.ui.reference_radio = Val("bbox")
        core.ui.spacing_columns = Val(sx)
        core.ui.spacing_rows = Val(sy)
        core.ui.rows = Val(rows)
        core.ui.columns = Val(cols)
        core.ui.x_width_entry = Val(cx)
        core.ui.y_height_entry = Val(cy)
        core.ui.panel_type_radio = Val(ptype)
        core.ui.constrain_cb = Val(constr)
        core.ui.optimization_cb = Val(opt)
        messages.clear()
        created.clear()
        core.on_panelize()
        okind, oname, o = created[0]
        dump = lambda g: W.dumps(g, rounding_precision=17, trim=True)  # noqa: E731
        sg = o.solid_geometry if isinstance(o.solid_geometry, list) else [o.solid_geometry]
        solid = [] if okind == "excellon" else [dump(g) for g in sg]  # del Excellon se comparan agujeros y ranuras
        aps = {}
        if okind == "gerber" and getattr(o, "apertures", None):
            for ap, v in o.apertures.items():
                aps[ap] = [(dump(e["solid"]) if "solid" in e else "", dump(e["follow"]) if "follow" in e else "",
                            dump(e["clear"]) if "clear" in e else "") for e in v.get("geometry", [])]
        tools = {}
        if okind == "excellon":
            for t, v in o.tools.items():
                tools[t] = [dump(d) for d in v["drills"]] + ["SLOT " + dump(LineString([a, b]))
                                                              for a, b in v.get("slots", [])]
        elif okind == "geometry" and o.tools:
            for t, v in o.tools.items():
                tools[t] = [dump(g) for g in v["solid_geometry"]]
        msg = messages[-1] if messages else ""
        ap_txt = ", ".join("{%s, {%s}}" % (cs(k), ", ".join("{%s, %s, %s}" % (cs(a), cs(b), cs(c)) for a, b, c in v))
                           for k, v in aps.items())
        tool_txt = ", ".join("{%d, {%s}}" % (k, ", ".join(cs(x) for x in v)) for k, v in tools.items())
        out.append("{%s, \"%s\", %s, %d, %d, %r, %r, \"%s\", %s, %s, %r, %r, \"%s\", %s, %s, {%s}, {%s}, {%s}}," % (
            cs(name), kind, cs(f or ""), cols, rows, sx, sy, ptype, "true" if opt else "false",
            "true" if constr else "false", float(cx), float(cy), okind, cs(oname), cs(msg),
            ", ".join(cs(s) for s in solid), ap_txt, tool_txt))
        print(name, okind, oname, len(solid), len(aps), {k: len(v) for k, v in tools.items()}, msg,
              [m for m in messages if "constrain" in m.lower() or "Final" in m])
    out += ["};", "}  // namespace golden_panelize"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
