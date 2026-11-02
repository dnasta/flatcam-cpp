#!/usr/bin/env python3
"""Oráculo del diámetro seguro (`find_optim_mp` ORIGINAL de ToolNCC, igual al cálculo de ToolIsolation).

`find_optim_mp` es un método estático: se toma del fuente con `ast` y se ejecuta sobre las aperturas de Gerbers leídos
por el parser original. Escribe `tests/golden_safe_tooldia.h`.
Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_safe_tooldia.py
"""
import ast
import os
import sys
import tempfile

from shapely.geometry import MultiPolygon
from shapely.ops import nearest_points

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs, num  # noqa: E402
from oracle_laser_lines import MIXED, SINGLE  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_safe_tooldia.h")
CASES = [("mixed", MIXED), ("single_polygon", SINGLE),
         ("user_logo", os.path.join(DATA, "luz_de_noche", "luz de noche Pascual-Logo.gbr")),
         ("user_b_cu", os.path.join(DATA, "luz_de_noche", "luz de noche Pascual-B_Cu.gbr"))]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    parse_gerber = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    camlib.Geometry.app = app
    parse_gerber.Gerber.app = app
    path = os.path.join(REF, "appTools", "ToolNCC.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "NonCopperClear")
    fn = next(f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "find_optim_mp")
    fn.decorator_list = []
    module = ast.Module(body=[fn], type_ignores=[])
    ast.fix_missing_locations(module)
    ns = dict(MultiPolygon=MultiPolygon, nearest_points=nearest_points, _=lambda s: s)
    exec(compile(module, path, "exec"), ns)
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_safe_tooldia.py - no editar a mano.",
           "// Referencia: ToolNCC.find_optim_mp original (diámetro de herramienta seguro).",
           "#pragma once", "#include <vector>", "namespace golden_safe_tooldia {",
           "struct Case { const char* name; const char* gerber; bool isPath; bool ok; double dist; };",
           "inline const std::vector<Case> kCases = {"]
    for name, src in CASES:
        is_path = os.path.exists(src)
        if is_path:
            gpath = src
        else:
            fd, gpath = tempfile.mkstemp(suffix=".gbr")
            with os.fdopen(fd, "w") as f:
                f.write(src)
        g = parse_gerber.Gerber(steps_per_circle=64)
        g.parse_file(gpath)
        if not is_path:
            os.remove(gpath)
        try:
            msg, dist = ns["find_optim_mp"](g.apertures, 4)
            ok = msg == "ok"
        except (TypeError, ValueError):  # un solo polígono: no se puede recorrer (el worker avisa y no sigue)
            ok, dist = False, 0.0
        print("%-16s %s %s" % (name, ok, dist), flush=True)
        gerber = os.path.relpath(src, DATA) if is_path else src
        out.append('{"%s", %s, %s, %s, %s},' % (name, cs(gerber), str(is_path).lower(), str(ok).lower(), num(dist)))
    out += ["};", "}  // namespace golden_safe_tooldia"]
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
