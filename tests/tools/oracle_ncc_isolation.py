#!/usr/bin/env python3
"""Oráculo de la operación "Aislamiento" de NCC: ejecuta `get_tool_empty_area` ORIGINAL con `isotooldia`.

No transcribe nada: toma de `ToolNCC` con `ast` los métodos `get_tool_empty_area`, `generate_envelope`, `poly2rings`,
`get_ncc_empty_area`, `calculate_bounding_box` y `apply_margin_to_bounding_box`, y los ejecuta sobre un objeto que deriva
del `Gerber` original (ToolNCC deriva de Gerber: su `isolation_geometry` es la de camlib). Para cada caso guarda el
recorrido de cada herramienta de aislamiento y el área que queda para limpiar, en WKT.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_ncc_isolation.py
"""
import ast
import logging
import os
import sys
import tempfile
from copy import deepcopy

import numpy as np
from shapely.geometry import LinearRing, LineString, MultiLineString, MultiPolygon, Point, Polygon, base
from shapely.ops import unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs, num  # noqa: E402
from oracle_laser_lines import MIXED  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_ncc_isolation.h")
METHODS = {"get_tool_empty_area", "generate_envelope", "poly2rings", "get_ncc_empty_area", "calculate_bounding_box",
           "apply_margin_to_bounding_box"}
LOGO = os.path.join(DATA, "luz_de_noche", "luz de noche Pascual-Logo.gbr")

# nombre, Gerber (texto o ruta), herramientas de aislamiento, fresado ('cl' o 'cv'), margen, agrandar, cuánto
SCENARIOS = [
    ("mixed_one_tool", MIXED, [0.2], "cl", 1.0, False, 0.0),
    ("mixed_conventional", MIXED, [0.2], "cv", 1.0, False, 0.0),
    ("mixed_two_tools", MIXED, [0.1, 0.3], "cl", 0.5, False, 0.0),
    ("mixed_offset", MIXED, [0.2], "cl", 1.0, True, 0.15),
    ("mixed_small_margin", MIXED, [0.6], "cl", 0.2, False, 0.0),
    ("user_logo", LOGO, [0.1], "cl", 1.0, False, 0.0),
]


def load_core(parse_gerber):
    path = os.path.join(REF, "appTools", "ToolNCC.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "NonCopperClear")
    body = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in METHODS]
    assert {f.name for f in body} == METHODS
    module = ast.Module(body=[ast.ClassDef(name="NccCore", bases=[ast.Name("GerberBase", ast.Load())], keywords=[],
                                           body=body, decorator_list=[])], type_ignores=[])
    ast.fix_missing_locations(module)

    class QApp:
        @staticmethod
        def processEvents():
            pass

    class QtWidgets:
        QApplication = QApp

    class Grace(Exception):
        pass
    ns = dict(GerberBase=parse_gerber.Gerber, Polygon=Polygon, MultiPolygon=MultiPolygon, LinearRing=LinearRing,
              LineString=LineString, MultiLineString=MultiLineString, Point=Point, unary_union=unary_union,
              deepcopy=deepcopy, np=np, base=base, QtWidgets=QtWidgets, grace=Grace, log=logging.getLogger("ncc"),
              _=lambda s: s)
    exec(compile(module, path, "exec"), ns)
    return ns["NccCore"]


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v


class Ui:
    pass


class GeoObj:
    def __init__(self):
        self.tools = {}


def wkt(g):
    return g.wkt if g is not None else ""


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    parse_gerber = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.defaults.update({"gerber_buffering": "full", "gerber_circle_steps": 64})
    camlib.Geometry.app = app
    parse_gerber.Gerber.app = app
    NccCore = load_core(parse_gerber)

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_ncc_isolation.py - no editar a mano.",
           "// Referencia: ToolNCC.get_tool_empty_area original con herramientas de aislamiento (isotooldia).",
           "#pragma once", "#include <vector>", "namespace golden_ncc_isolation {",
           "struct Scenario { const char* name; const char* gerber; bool isPath; std::vector<double> isoDias; bool climb;"
           " double margin; bool hasOffset; double offset; std::vector<std::vector<const char*>> isoPaths;"
           " std::vector<bool> broken; const char* empty; };",
           "inline const std::vector<Scenario> kScenarios = {"]
    for name, src, dias, mill, margin, has_off, off in SCENARIOS:
        is_path = os.path.exists(src)
        if is_path:
            gpath = src
        else:
            fd, gpath = tempfile.mkstemp(suffix=".gbr")
            with os.fdopen(fd, "w") as f:
                f.write(src)
        grb = parse_gerber.Gerber(steps_per_circle=64)
        grb.parse_file(gpath)
        grb.kind = "gerber"
        if not is_path:
            os.remove(gpath)

        ncc = NccCore(steps_per_circle=64)
        ncc.app = app
        ncc.decimals = 4
        ncc.ui = Ui()
        ncc.ui.milling_type_radio = Val(mill)
        tools_storage = {i + 1: {"tooldia": d, "solid_geometry": [], "data": {}} for i, d in enumerate(dias)}
        # Una herramienta de limpieza (no se usa acá: solo su agrandado del cobre).
        tools_storage[len(dias) + 1] = {"tooldia": 0.05, "solid_geometry": [], "data": {}}
        bbox_geo, kind = ncc.calculate_bounding_box(ncc_obj=grb, ncc_select=0)
        bbox = ncc.apply_margin_to_bounding_box(bbox=bbox_geo, box_kind=kind, ncc_select=0, ncc_margin=margin)
        geo_obj = GeoObj()
        empty, warning = ncc.get_tool_empty_area(name="x", ncc_obj=grb, geo_obj=geo_obj, isotooldia=dias,
                                                 has_offset=has_off, ncc_offset=off, ncc_margin=margin,
                                                 bounding_box=bbox, tools_storage=tools_storage)
        iso, broken = [], []
        for i, d in enumerate(dias):
            paths = tools_storage[i + 1]["solid_geometry"]
            iso.append("{" + ", ".join(cs(wkt(p)) for p in paths) + "}")
            broken.append(any(type(p) == MultiLineString for p in paths))
        print("%-22s %s  empty: %d polígonos, área %.6f, aviso %d" % (
            name, [len(tools_storage[i + 1]["solid_geometry"]) for i in range(len(dias))], len(empty.geoms),
            empty.area, warning), flush=True)
        gerber = os.path.relpath(src, DATA) if is_path else src
        out.append('{"%s", %s, %s, {%s}, %s, %s, %s, %s, {%s}, {%s}, %s},' % (
            name, cs(gerber), str(is_path).lower(), ", ".join(num(d) for d in dias), str(mill == "cl").lower(),
            num(margin), str(has_off).lower(), num(off), ", ".join(iso), ", ".join(str(b).lower() for b in broken),
            cs(empty.wkt)))
    out += ["};", "}  // namespace golden_ncc_isolation"]
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
