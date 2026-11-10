#!/usr/bin/env python3
"""Oráculo de Perforar Gerber: ejecuta `ToolPunchGerber.on_generate_object` y sus cuatro métodos ORIGINALES (tomados con
`ast`) con una interfaz simulada sobre los cobres del usuario, y escribe en `tests/golden_punch.h` el resultado: el mensaje,
los polígonos del Gerber nuevo resumidos (cantidad y áreas) y las aperturas agregadas con sus agujeros "clear".

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_punch.py
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

OUT = os.path.join(ROOT, "tests", "golden_punch.h")
BCU = "PCB principal-B_Cu.gbl"
DRL = "PCB principal-Drill.drl"
LUZ = "luz_de_noche/luz de noche Pascual-B_Cu.gbr"
ALL = (True, True, True, True, True)
RINGS = (0.2, 0.3, 0.25, 0.35, 0.2)
# (archivo, método, aperturas elegidas (None = todas), diámetro fijo, factor %, filtros (C, O, S, R, otros), Excellon)
CASES = [
    (BCU, "exc", None, 0.5, 50.0, ALL, DRL),
    (BCU, "exc", ["10", "13"], 0.5, 50.0, ALL, DRL),
    (BCU, "fixed", None, 0.4, 50.0, ALL, None),           # con macros y "otros": KeyError 'size' en el original
    (BCU, "fixed", None, 0.4, 50.0, (True, True, True, True, False), None),
    (BCU, "fixed", None, 2.0, 50.0, (True, True, True, True, False), None),  # más grande que algún pad: falla
    (BCU, "fixed", [], 0.4, 50.0, ALL, None),            # nada elegido: misma geometría
    (BCU, "ring", None, 0.5, 50.0, ALL, None),
    (BCU, "ring", None, 0.5, 50.0, (True, False, True, False, False), None),
    (BCU, "prop", None, 0.5, 40.0, ALL, None),
    (LUZ, "prop", None, 0.5, 30.0, ALL, None),
    (LUZ, "fixed", None, 0.0, 50.0, ALL, None),           # diámetro 0: aviso
]


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v


class Item:
    def __init__(self, t):
        self.t = t

    def text(self):
        return self.t


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
    from shapely.geometry import MultiPolygon, Point
    from shapely.ops import unary_union
    path = os.path.join(REF, "appTools", "ToolPunchGerber.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolPunchGerber")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in (
        "on_generate_object", "on_excellon_method", "on_fixed_method", "on_ring_method", "on_proportional_method")]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(MultiPolygon=MultiPolygon, Point=Point, unary_union=unary_union, deepcopy=deepcopy, _=lambda s: s,
              log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    messages = []
    app.inform.emit = lambda s: messages.append(s)
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
    cache = {}
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_punch.py - no editar a mano.",
           "// Referencia: ToolPunchGerber.on_generate_object y sus métodos originales.", "#pragma once",
           "#include <string>", "#include <vector>", "namespace golden_punch {",
           "struct NewAp { const char* id; double size; std::vector<std::string> clear; };",
           "struct PunchCase { const char* file; const char* method; bool allSelected; std::vector<std::string> sel;"
           " double dia; double factor; std::vector<bool> filters; const char* exc; bool ok; const char* name;"
           " const char* message; int polygons; double area; std::vector<double> areas; std::vector<NewAp> newAps; };",
           "inline const std::vector<PunchCase> kCases = {"]
    for f, method, sel, dia, factor, flt, exc in CASES:
        if f not in cache:
            g = pg.Gerber(steps_per_circle=64)
            g.parse_file(os.path.join(DATA, f))
            g.options = {"name": os.path.basename(f)}
            g.fill_color = g.outline_color = "#000"
            cache[f] = g
        g = cache[f]
        e = None
        if exc:
            if exc not in cache:
                e = pe.Excellon(geo_steps_per_circle=64)
                e.default_data = {}
                e.parse_file(os.path.join(DATA, exc))
                e.create_geometry()
                cache[exc] = e
            e = cache[exc]
        selected = list(g.apertures.keys()) if sel is None else sel

        def index(i, c, root, gg=g, ee=e):
            mi = mock.MagicMock()
            mi.internalPointer.return_value.obj = ee if root == "exc" else gg
            return mi
        app.collection = mock.MagicMock()
        app.collection.index = index
        core = ns["Core"]()
        core.app = app
        core.decimals = 4
        core.ui = mock.MagicMock()
        core.ui.gerber_object_combo.rootModelIndex.return_value = "grb"
        core.ui.exc_combo.rootModelIndex.return_value = "exc"
        core.ui.apertures_table.selectedItems.return_value = [Item(a) for a in selected]
        vals = dict(method_punch=method, dia_entry=dia, circular_ring_entry=RINGS[0], oblong_ring_entry=RINGS[1],
                    square_ring_entry=RINGS[2], rectangular_ring_entry=RINGS[3], other_ring_entry=RINGS[4],
                    factor_entry=factor, circular_cb=flt[0], oblong_cb=flt[1], square_cb=flt[2],
                    rectangular_cb=flt[3], other_cb=flt[4])
        for k, v in vals.items():
            setattr(core.ui, k, Val(v))
        created.clear()
        messages.clear()
        try:
            core.on_generate_object()
        except Exception as ex:  # noqa: BLE001  el original revienta (en la GUI, PyQt solo lo registra)
            messages.append("EXCEPTION %s" % type(ex).__name__)
        name, polys, area, areas, newaps = "", 0, 0.0, [], []
        if created:
            name, o = created[0]
            sg = o.solid_geometry
            parts = list(sg.geoms) if hasattr(sg, "geoms") else [sg]
            polys = len(parts)
            area = sum(p.area for p in parts)
            areas = sorted(round(p.area, 9) for p in parts)
            orig = set(g.apertures.keys())
            for ap, v in o.apertures.items():
                if ap in orig:
                    continue
                newaps.append((ap, v["size"], [W.dumps(x["clear"], rounding_precision=17, trim=True)
                                               for x in v["geometry"]]))
        msg = messages[-1] if messages else ""
        ap_txt = ", ".join("{%s, %r, {%s}}" % (cs(a), float(s), ", ".join(cs(c) for c in cl)) for a, s, cl in newaps)
        out.append("{%s, \"%s\", %s, {%s}, %r, %r, {%s}, %s, %s, %s, %s, %d, %r, {%s}, {%s}}," % (
            cs(f), method, "true" if sel is None else "false", ", ".join(cs(a) for a in (sel or [])), dia, factor,
            ", ".join("true" if x else "false" for x in flt), cs(exc or ""), "true" if created else "false", cs(name),
            cs(msg), polys, area, ", ".join(repr(a) for a in areas), ap_txt))
        print(f, method, sel, dia, bool(created), name, polys, round(area, 4), [(a, s, len(c)) for a, s, c in newaps],
              msg)
    out += ["};", "}  // namespace golden_punch"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
