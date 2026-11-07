#!/usr/bin/env python3
"""Oráculo de importar HPGL2: ejecuta `appParsers/ParseHPGL2.py` ORIGINAL sobre `tests/data/hpgl2/` y guarda la
geometría (unión) y la de cada pluma en WKT en `tests/golden_hpgl2.h`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_hpgl2.py
"""
import logging
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_hpgl2.h")
FILES = ["basic.plt"] if os.environ.get("ONLY_BASIC") else ["basic.plt", "multi_cmd.plt", "pen_down_first.plt", "aa_then_ci.plt", "single_point_path.plt", "at_aborts.plt"]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    ph = load("ParseHPGL2", os.path.join(REF, "appParsers", "ParseHPGL2.py"))
    app = FakeApp()
    import json
    app.defaults.update(json.load(open(os.path.join(ROOT, "src", "gui", "resources", "factory_defaults.json"))))
    app.defaults["geometry_circle_steps"] = 64
    app.log = logging.getLogger("o")
    app.abort_flag = False
    app.decimals = 4
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_hpgl2.py - no editar a mano.",
           "// Referencia: appParsers/ParseHPGL2.py original.", "#pragma once", "#include <utility>", "#include <vector>",
           "namespace golden_hpgl2 {",
           "struct Case { const char* file; bool ok; const char* solid; std::vector<std::pair<int, std::vector<const char*>>> tools; };",
           "inline const std::vector<Case> kCases = {"]
    for f in FILES:
        o = ph.HPGL2(app)
        o.parse_file(os.path.join(DATA, "hpgl2", f))
        ok = bool(o.solid_geometry)
        solid = o.solid_geometry.wkt if ok else ""
        tools = ", ".join("{%s, {%s}}" % (k, ", ".join(cs(g.wkt) for g in v["solid_geometry"])) for k, v in o.tools.items())
        out.append("{%s, %s, %s, {%s}}," % (cs(f), str(ok).lower(), cs(solid), tools if ok else ""))
        print(f, ok, {k: len(v["solid_geometry"]) for k, v in o.tools.items()}, flush=True)
    out += ["};", "}  // namespace golden_hpgl2"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
