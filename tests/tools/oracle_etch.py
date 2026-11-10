#!/usr/bin/env python3
"""Oráculo de Compensación de grabado: ejecuta `ToolEtchCompensation.on_compensate` ORIGINAL (tomado con `ast`) sobre
Gerber del usuario y escribe en `tests/golden_etch.h` el Gerber nuevo resumido: polígonos (cantidad y áreas) y cada
apertura (tamaño, ancho, alto y el área total de sus elementos).

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_etch.py
"""
import ast
import logging
import math
import os
import sys
from copy import deepcopy
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_etch.h")
LUZ = "luz_de_noche/luz de noche Pascual-B_Cu.gbr"
BCU = "PCB principal-B_Cu.gbl"
# (archivo, tipo, espesor en micras, factor, grabador, desplazamiento en micras)
CASES = [(LUZ, "factor", 18.0, 2.0, "CuCl2", 0.0), (LUZ, "etch_list", 35.0, 1.0, "CuCl2", 0.0),
         (BCU, "etch_list", 18.0, 1.0, "Fe3Cl", 0.0), (BCU, "offset", 18.0, 1.0, "CuCl2", 25.0)]


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v

    def text(self):
        return str(self.v)


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.log = logging.getLogger("oracle")
    app.inform_shell = mock.MagicMock()
    app.inform = mock.MagicMock()
    pg.Gerber.app = app
    from shapely.ops import unary_union
    path = os.path.join(REF, "appTools", "ToolEtchCompensation.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolEtchCompensation")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "on_compensate"]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(unary_union=unary_union, deepcopy=deepcopy, math=math, _=lambda s: s, log=logging.getLogger("x"))
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
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_etch.py - no editar a mano.",
           "// Referencia: ToolEtchCompensation.on_compensate original.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_etch {",
           "struct ApOut { const char* id; double size; double width; double height; double area; };",
           "struct EtchCase { const char* file; const char* type; double thickness; double factor; const char* etchant;"
           " double offset; const char* name; int polygons; std::vector<double> areas; std::vector<ApOut> apertures; };",
           "inline const std::vector<EtchCase> kCases = {"]
    for f, typ, th, factor, etchant, off in CASES:
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        g.options = {"name": os.path.basename(f)}
        g.fill_color = g.outline_color = "#000"
        app.collection = mock.MagicMock()
        app.collection.get_by_name = lambda n, g=g: g
        core = ns["Core"]()
        core.app = app
        core.ui = mock.MagicMock()
        for k, v in dict(ratio_radio=typ, thick_entry=th, factor_entry=factor, etchants_combo=etchant,
                         offset_entry=off, gerber_combo=os.path.basename(f)).items():
            setattr(core.ui, k, Val(v))
        core.ui.gerber_combo.currentText = lambda f=f: os.path.basename(f)
        created.clear()
        core.on_compensate()
        name, o = created[0]
        sg = o.solid_geometry
        parts = list(sg.geoms) if hasattr(sg, "geoms") else [sg]
        areas = sorted(round(p.area, 9) for p in parts)
        aps = []
        for ap, v in o.apertures.items():
            area = sum(e["solid"].area for e in v.get("geometry", []) if "solid" in e)
            aps.append((ap, float(v.get("size", -1) or 0), float(v.get("width", -1)), float(v.get("height", -1)), area))
        ap_txt = ", ".join("{%s, %r, %r, %r, %r}" % (cs(a), s, w, h, ar) for a, s, w, h, ar in aps)
        out.append("{%s, \"%s\", %r, %r, \"%s\", %r, %s, %d, {%s}, {%s}}," % (
            cs(f), typ, th, factor, etchant, off, cs(name), len(parts), ", ".join(repr(a) for a in areas), ap_txt))
        print(f, typ, name, len(parts), round(sum(areas), 4), len(aps))
    out += ["};", "}  // namespace golden_etch"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
