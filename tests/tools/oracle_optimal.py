#!/usr/bin/env python3
"""Oráculo de Óptimo: ejecuta `ToolOptimal.find_minimum_distance` ORIGINAL (tomado con `ast`) con una interfaz simulada
sobre Gerber del usuario, y escribe en `tests/golden_optimal.h` el resultado (distancia mínima con su texto, cantidad,
los puntos de cada distancia y la lista de las demás) o el aviso.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_optimal.py
"""
import ast
import logging
import os
import sys
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_optimal.h")
CASES = [("luz_de_noche/luz de noche Pascual-B_Cu.gbr", 4), ("luz_de_noche/luz de noche Pascual-B_Cu.gbr", 2),
         ("PCB principal-B_Logo.gbr", 4), ("luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr", 4)]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.log = logging.getLogger("oracle")
    pg.Gerber.app = app
    import numpy as np
    from shapely.geometry import MultiPolygon
    from shapely.ops import nearest_points
    path = os.path.join(REF, "appTools", "ToolOptimal.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolOptimal")
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "find_minimum_distance"]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(MultiPolygon=MultiPolygon, nearest_points=nearest_points, np=np, _=lambda s: s,
              log=logging.getLogger("x"), grace=Exception)
    exec(compile(m, path, "exec"), ns)
    messages = []
    app.inform.emit = lambda s: messages.append(s)
    app.worker_task = mock.MagicMock()
    app.worker_task.emit = lambda d: d["fcn"](*d["params"])
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_optimal.py - no editar a mano.",
           "// Referencia: ToolOptimal.find_minimum_distance original.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_optimal {",
           "struct OptimalCase { const char* file; int precision; bool ok; const char* message; const char* result;"
           " const char* freq; std::vector<std::string> minLocations; std::vector<std::string> others; };",
           "inline const std::vector<OptimalCase> kCases = {"]
    for f, prec in CASES:
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        g.kind = "gerber"
        idx = mock.MagicMock()
        idx.internalPointer.return_value.obj = g
        app.collection = mock.MagicMock()
        app.collection.index.return_value = idx
        core = ns["Core"]()
        core.app = app
        core.ui = mock.MagicMock()
        core.ui.precision_spinner.get_value.return_value = prec
        got = {}
        core.ui.result_entry.set_value = lambda v: got.__setitem__("result", v)
        core.ui.freq_entry.set_value = lambda v: got.__setitem__("freq", v)
        core.update_text = mock.MagicMock()
        core.update_text.emit = lambda d: got.__setitem__("locs", d)
        core.update_sec_distances = mock.MagicMock()
        core.update_sec_distances.emit = lambda d: got.__setitem__("others", d)
        messages.clear()
        core.find_minimum_distance()
        ok = "result" in got
        locs = ["%s, %s" % (str(a), str(b)) for a, b in got.get("locs", [])]
        others = [str(k) for k in sorted(got.get("others", {}).keys())]
        out.append("{%s, %d, %s, %s, %s, %s, {%s}, {%s}}," % (
            cs(f), prec, "true" if ok else "false", cs(messages[-1] if messages else ""), cs(got.get("result", "")),
            cs(got.get("freq", "")), ", ".join(cs(x) for x in locs), ", ".join(cs(x) for x in others)))
        print(f, prec, ok, got.get("result"), got.get("freq"), len(others), messages[-1][:60] if messages else "")
    out += ["};", "}  // namespace golden_optimal"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
