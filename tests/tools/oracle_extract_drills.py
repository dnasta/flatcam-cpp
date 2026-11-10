#!/usr/bin/env python3
"""Oráculo de Extraer taladros: ejecuta `ToolExtractDrills.on_extract_drills_click` ORIGINAL (tomado con `ast`) con una
interfaz simulada sobre los cobres del usuario leídos con el parser original, y escribe en `tests/golden_extract_drills.h`
el Excellon que crea (nombre, herramientas con su diámetro y sus agujeros) o el aviso.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_extract_drills.py
"""
import ast
import logging
import os
import sys
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_extract_drills.h")
BCU = "PCB principal-B_Cu.gbl"
LUZ = "luz_de_noche/luz de noche Pascual-B_Cu.gbr"
ALL = (True, True, True, True, True)
# (archivo, modo, diámetro fijo, anillos (C, O, S, R, otros), factor %, filtros (C, O, S, R, otros))
CASES = [
    (BCU, "fixed", 0.5, (0.2,) * 5, 50.0, ALL),
    (BCU, "fixed", 0.8, (0.2,) * 5, 50.0, (True, False, False, False, False)),
    (BCU, "ring", 0.5, (0.2, 0.3, 0.25, 0.35, 0.2), 50.0, ALL),
    (BCU, "ring", 0.5, (0.2, 0.3, 0.25, 0.35, 0.2), 50.0, (False, True, False, True, True)),
    (BCU, "prop", 0.5, (0.2,) * 5, 40.0, ALL),
    (LUZ, "ring", 0.5, (0.4, 0.4, 0.4, 0.4, 0.4), 50.0, ALL),
    (LUZ, "prop", 0.5, (0.2,) * 5, 60.0, (False, False, True, True, False)),
    (LUZ, "fixed", 0.5, (0.2,) * 5, 50.0, (False, False, False, False, False)),  # nada: aviso
]


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.log = logging.getLogger("oracle")
    app.defaults.update({"excellon_circle_steps": 64})
    pg.Gerber.app = app
    pe.Excellon.app = app
    from shapely import wkt as W
    from shapely.geometry import Point
    path = os.path.join(REF, "appTools", "ToolExtractDrills.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolExtractDrills")
    meth = next(f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "on_extract_drills_click")
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=[meth], decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(Point=Point, _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    messages = []
    app.inform.emit = lambda s: messages.append(s)
    created = []

    def new_object(kind, name, init):
        o = pe.Excellon(geo_steps_per_circle=64)
        o.default_data = {}
        o.options = {}
        init(o, app)
        created.append((name, o))
    app.app_obj = mock.MagicMock()
    app.app_obj.new_object = new_object
    app.f_handlers = mock.MagicMock()
    app.f_handlers.export_excellon.return_value = ""
    gerbers = {}
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_extract_drills.py - no editar a mano.",
           "// Referencia: ToolExtractDrills.on_extract_drills_click original.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_extract_drills {",
           "struct ToolOut { int id; double dia; std::vector<std::string> drills; };",
           "struct ExtractCase { const char* file; const char* mode; double fixedDia; std::vector<double> rings;"
           " double factor; std::vector<bool> filters; bool ok; const char* name; const char* message;"
           " std::vector<ToolOut> tools; };",
           "inline const std::vector<ExtractCase> kCases = {"]
    for f, mode, dia, rings, factor, flt in CASES:
        if f not in gerbers:
            g = pg.Gerber(steps_per_circle=64)
            g.parse_file(os.path.join(DATA, f))
            g.options = {"name": os.path.basename(f)}
            gerbers[f] = g
        g = gerbers[f]
        idx = mock.MagicMock()
        idx.internalPointer.return_value.obj = g
        app.collection = mock.MagicMock()
        app.collection.index.return_value = idx
        core = ns["Core"]()
        core.app = app
        core.decimals = 4
        core.ui = mock.MagicMock()
        vals = dict(dia_entry=dia, circular_ring_entry=rings[0], oblong_ring_entry=rings[1],
                    square_ring_entry=rings[2], rectangular_ring_entry=rings[3], other_ring_entry=rings[4],
                    factor_entry=factor, hole_size_radio=mode, circular_cb=flt[0], oblong_cb=flt[1],
                    square_cb=flt[2], rectangular_cb=flt[3], other_cb=flt[4])
        for k, v in vals.items():
            setattr(core.ui, k, Val(v))
        created.clear()
        messages.clear()
        core.on_extract_drills_click()
        name, tools = "", []
        if created:
            name, o = created[0]
            for t, v in o.tools.items():
                tools.append((t, v["tooldia"], [W.dumps(p, rounding_precision=17, trim=True) for p in v["drills"]]))
        msg = messages[-1] if messages else ""
        tool_txt = ", ".join("{%d, %r, {%s}}" % (t, float(dd), ", ".join(cs(x) for x in dr)) for t, dd, dr in tools)
        out.append("{%s, \"%s\", %r, {%s}, %r, {%s}, %s, %s, %s, {%s}}," % (
            cs(f), mode, dia, ", ".join(repr(r) for r in rings), factor,
            ", ".join("true" if x else "false" for x in flt), "true" if created else "false", cs(name), cs(msg),
            tool_txt))
        print(f, mode, flt, name, [(t, round(dd, 4), len(dr)) for t, dd, dr in tools], msg)
    out += ["};", "}  // namespace golden_extract_drills"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
