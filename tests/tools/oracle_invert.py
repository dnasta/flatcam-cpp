#!/usr/bin/env python3
"""Oráculo de Invertir Gerber: ejecuta `ToolInvertGerber.on_grb_invert` ORIGINAL (tomado con `ast`) sobre Gerber leídos
con el parser original y escribe en `tests/golden_invert.h` las figuras de la apertura "0" del Gerber nuevo (sólido y
contorno, WKT con 17 cifras).

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_invert.py
"""
import ast
import logging
import os
import sys
from copy import deepcopy
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_invert.h")
EDGE = "luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr"
LOGO = "PCB principal-B_Logo.gbr"
CASES = [(EDGE, 0.0, "s"), (EDGE, 2.0, "r"), (EDGE, 2.0, "b"), (EDGE, 2.0, "s"), (LOGO, 0.5, "s")]


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v

    def currentText(self):
        return self.v


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.log = logging.getLogger("oracle")
    pg.Gerber.app = app
    from shapely import wkt as W
    from shapely.geometry import box
    path = os.path.join(REF, "appTools", "ToolInvertGerber.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolInvertGerber")
    meth = next(f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "on_grb_invert")
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=[meth], decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(box=box, deepcopy=deepcopy, _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    created = []

    def new_object(kind, name, init):
        o = mock.MagicMock()
        o.options = {}
        init(o, app)
        created.append((name, o))
    app.app_obj = mock.MagicMock()
    app.app_obj.new_object = new_object
    app.f_handlers = mock.MagicMock()
    app.f_handlers.export_gerber.return_value = ""
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_invert.py - no editar a mano.",
           "// Referencia: ToolInvertGerber.on_grb_invert original.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_invert {",
           "struct InvertCase { const char* file; double margin; const char* join; const char* name;"
           " std::vector<std::string> solid; std::vector<std::string> follow; };",
           "inline const std::vector<InvertCase> kCases = {"]
    for f, margin, join in CASES:
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        g.options = {"name": os.path.basename(f)}
        g.fill_color = g.outline_color = "#000000"
        app.collection = mock.MagicMock()
        app.collection.get_by_name = lambda n, g=g: g
        core = ns["Core"]()
        core.app = app
        core.decimals = 4
        core.ui = mock.MagicMock()
        core.ui.margin_entry = Val(margin)
        core.ui.join_radio = Val(join)
        core.ui.gerber_combo = Val(os.path.basename(f))
        created.clear()
        core.on_grb_invert()
        name, o = created[0]
        geo = o.apertures["0"]["geometry"]
        d = lambda x: W.dumps(x, rounding_precision=17, trim=True)  # noqa: E731
        out.append("{%s, %r, \"%s\", %s, {%s}, {%s}}," % (cs(f), margin, join, cs(name),
                                                        ", ".join(cs(d(e["solid"])) for e in geo),
                                                        ", ".join(cs(d(e["follow"])) for e in geo)))
        print(f, margin, join, name, len(geo))
    out += ["};", "}  // namespace golden_invert"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
