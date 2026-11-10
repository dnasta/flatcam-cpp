#!/usr/bin/env python3
"""Oráculo del fresado de agujeros y ranuras de un Excellon ("Geometría de fresado" del panel del Excellon): ejecuta
`ExcellonObject.generate_milling_drills` y `generate_milling_slots` ORIGINALES (FlatCAMExcellon.py, tomados con `ast`)
sobre Excellon leídos con el parser original, y escribe en `tests/golden_milling.h` el resultado de cada caso: si se
generó, el nombre del objeto, el mensaje de error y los anillos (WKT con 17 cifras).

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_milling.py
"""
import ast
import logging
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_milling.h")
# (archivo, operación, herramientas elegidas (None = las de la tabla con agujeros/ranuras), diámetro de la fresa)
CASES = [
    ("luz_de_noche/luz de noche Pascual-PTH.drl", "drills", [1, 2, 3, 4], 0.8),
    ("luz_de_noche/luz de noche Pascual-PTH.drl", "drills", [3, 4], 1.0),
    ("luz_de_noche/luz de noche Pascual-PTH.drl", "drills", [1], 0.8),       # igual al agujero: buffer(1e-7)
    ("luz_de_noche/luz de noche Pascual-PTH.drl", "drills", [1, 4], 1.2),    # más grande que T1: falla
    ("luz_de_noche/luz de noche Pascual-PTH.drl", "drills", [], 0.8),        # sin herramientas
    ("excellon_samples/slots_g85.drl", "slots", "all_slots", 0.5),
    ("excellon_samples/routed_slots.drl", "slots", "all_slots", 0.5),
    ("excellon_samples/routed_slots.drl", "slots", "all_slots", 50.0),       # más grande: falla
]


def methods(path, clsname, names):
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == clsname)
    return [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in names]


class Geo:
    kind = "geometry"

    def __init__(self):
        self.options = {}
        self.solid_geometry = None


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.defaults.update({"excellon_circle_steps": 64, "geometry_multidepth": False})
    app.log = logging.getLogger("oracle")
    pe.Excellon.app = app
    from shapely.geometry import LineString, Point
    ns = dict(LineString=LineString, Point=Point, log=logging.getLogger("x"), _=lambda s: s)
    m = ast.Module(body=methods(os.path.join(REF, "appObjects", "FlatCAMExcellon.py"), "ExcellonObject",
                                {"generate_milling_drills", "generate_milling_slots"}), type_ignores=[])
    ast.fix_missing_locations(m)
    exec(compile(m, "FlatCAMExcellon.py", "exec"), ns)

    class Obj(pe.Excellon):
        generate_milling_drills = ns["generate_milling_drills"]
        generate_milling_slots = ns["generate_milling_slots"]

    messages = []
    app.inform.emit = lambda msg: messages.append(msg)
    created = []

    class AppObj:
        def new_object(self, kind, name, init, plot=False):
            g = Geo()
            init(g, app)
            created.append((name, g))
    app.app_obj = AppObj()
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_milling.py - no editar a mano.",
           "// Referencia: ExcellonObject.generate_milling_drills / generate_milling_slots originales.",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_milling {",
           "struct MillingCase { const char* file; const char* op; std::vector<int> tools; double tooldia; bool ok;"
           " const char* name; const char* message; std::vector<std::string> rings; };",
           "inline const std::vector<MillingCase> kCases = {"]
    for f, op, tools, dia in CASES:
        e = Obj(geo_steps_per_circle=64)
        e.parse_file(os.path.join(DATA, f))
        e.create_geometry()
        e.options = {"name": os.path.basename(f), "slot_tooldia": dia}
        e.decimals = 4
        e.get_selected_tools_table_items = lambda: []
        if tools == "all_slots":
            tools = [t for t in e.tools if e.tools[t].get("slots")]
        messages.clear()
        created.clear()
        if op == "drills":
            ok, _ = e.generate_milling_drills(tools=list(tools), tooldia=dia)
        else:
            ok, _ = e.generate_milling_slots(tools=list(tools), tooldia=dia)
        name, rings = "", []
        if created:
            name, g = created[0]
            rings = [r.wkt if hasattr(r, "wkt") else str(r) for r in g.solid_geometry]
            from shapely import wkt as W
            rings = [W.dumps(r, rounding_precision=17, trim=True) for r in g.solid_geometry]
        msg = messages[-1] if messages else ""
        out.append("{%s, \"%s\", {%s}, %r, %s, %s, %s, {%s}}," % (
            cs(f), op, ", ".join(str(t) for t in tools), dia, "true" if ok else "false", cs(name), cs(msg),
            ", ".join(cs(r) for r in rings)))
        print(f, op, tools, dia, ok, name, len(rings), msg)
    out += ["};", "}  // namespace golden_milling"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
