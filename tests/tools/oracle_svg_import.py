#!/usr/bin/env python3
"""Oráculo de importar SVG: ejecuta `camlib.Geometry.import_svg` y `ParseGerber.Gerber.import_svg` ORIGINALES (con
`appParsers/ParseSVG.py` y `svg.path` 7.1) sobre `tests/data/svg/` y guarda las figuras en WKT en
`tests/golden_svg_import.h`. Necesita `micromamba install -n old -c conda-forge svg.path`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_svg_import.py
"""
import logging
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_svg_import.h")
FILES = ["paths.svg", "shapes.svg", "factor.svg", "single.svg"]


def main():
    install_stubs()
    # oracle_common simula lxml, svg.path y ParseSVG: acá hacen falta los reales (ParseFont, solo para textos, no).
    for m in ("svg", "svg.path", "lxml", "lxml.etree", "appParsers.ParseSVG"):
        sys.modules.pop(m, None)
    from unittest import mock
    sys.modules["freetype"] = mock.MagicMock()
    sys.modules["fontTools"] = mock.MagicMock()
    sys.modules["fontTools.ttLib"] = mock.MagicMock()
    sys.path.insert(0, REF)
    pfont = load("appParsers.ParseFont", os.path.join(REF, "appParsers", "ParseFont.py"))
    load("appParsers.ParseSVG", os.path.join(REF, "appParsers", "ParseSVG.py"))
    # ParseGerber recibe `translate` y `scale` por `from appParsers.ParseDXF import *`, que los toma de ParseFont
    # (ParseDXF necesita ezdxf): se usa ParseFont en su lugar.
    sys.modules["appParsers.ParseDXF"] = pfont
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.defaults.update({"geometry_circle_steps": 64, "gerber_circle_steps": 64, "units": "MM",
                         "geometry_cnctooldia": 2.4})
    app.log = logging.getLogger("oracle")
    camlib.Geometry.app = app
    pg.Gerber.app = app
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_svg_import.py - no editar a mano.",
           "// Referencia: Geometry.import_svg y Gerber.import_svg originales (ParseSVG, svg.path 7.1, Shapely 1.8.4).",
           "#pragma once", "#include <vector>", "namespace golden_svg_import {",
           "struct Case { const char* file; bool gerber; bool ok; std::vector<const char*> wkt; };",
           "inline const std::vector<Case> kCases = {"]
    for f in FILES:
        path = os.path.join(DATA, "svg", f)
        for gerber in (False, True):
            try:
                if gerber:
                    o = pg.Gerber(steps_per_circle=64)
                    o.import_svg(path, "gerber", units="MM")
                else:
                    o = camlib.Geometry(geo_steps_per_circle=64)
                    o.options = {"name": f}
                    o.tools = {}
                    o.decimals = 4
                    o.import_svg(path, "geometry", units="MM")
                geos = list(o.solid_geometry)
                ok = True
            except Exception as e:  # noqa: BLE001
                print(f, gerber, "falla:", repr(e))
                geos, ok = [], False
            out.append("{%s, %s, %s, {%s}}," % (cs(f), str(gerber).lower(), str(ok).lower(),
                                                ", ".join(cs(g.wkt) for g in geos)))
            print(f, "gerber" if gerber else "geometry", len(geos), "figuras", flush=True)
    out += ["};", "}  // namespace golden_svg_import"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
