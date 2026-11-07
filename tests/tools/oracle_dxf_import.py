#!/usr/bin/env python3
"""Oráculo de importar DXF: ejecuta `camlib.Geometry.import_dxf_as_geo` y `ParseGerber.Gerber.import_dxf_as_gerber`
ORIGINALES (con `ParseDXF.py`, `ParseDXF_Spline.py` y ezdxf 0.14) sobre `tests/data/dxf/` y guarda las figuras en WKT en
`tests/golden_dxf_import.h`. Necesita `python -m pip install ezdxf==0.14.2` en el entorno `old`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_dxf_import.py
"""
import logging
import os
import sys
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_dxf_import.h")
FILES = ["entities.dxf", "lines_r12.dxf", "spline_fit_points.dxf"]


def main():
    install_stubs()
    for m in ("ezdxf", "lxml", "lxml.etree", "svg", "svg.path", "appParsers.ParseSVG", "appParsers.ParseDXF"):
        sys.modules.pop(m, None)
    for m in ("freetype", "fontTools", "fontTools.ttLib"):
        sys.modules[m] = mock.MagicMock()
    sys.path.insert(0, REF)
    load("appParsers.ParseFont", os.path.join(REF, "appParsers", "ParseFont.py"))
    load("appParsers.ParseDXF_Spline", os.path.join(REF, "appParsers", "ParseDXF_Spline.py"))
    load("appParsers.ParseDXF", os.path.join(REF, "appParsers", "ParseDXF.py"))
    load("appParsers.ParseSVG", os.path.join(REF, "appParsers", "ParseSVG.py"))
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.defaults.update({"geometry_circle_steps": 64, "gerber_circle_steps": 64, "units": "MM",
                         "geometry_cnctooldia": 2.4})
    app.log = logging.getLogger("oracle")
    camlib.Geometry.app = app
    pg.Gerber.app = app
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_dxf_import.py - no editar a mano.",
           "// Referencia: Geometry.import_dxf_as_geo y Gerber.import_dxf_as_gerber originales (ezdxf 0.14).",
           "#pragma once", "#include <vector>", "namespace golden_dxf_import {",
           "struct Case { const char* file; bool gerber; bool ok; std::vector<const char*> wkt; };",
           "inline const std::vector<Case> kCases = {"]
    for f in FILES:
        path = os.path.join(DATA, "dxf", f)
        for gerber in (False, True):
            try:
                if gerber:
                    o = pg.Gerber(steps_per_circle=64)
                    o.options = {"name": f}
                    o.tools = {}
                    o.decimals = 4
                    o.import_dxf_as_gerber(path, units="MM")
                    geos = [el["solid"] for el in o.apertures["0"]["geometry"]]
                else:
                    o = camlib.Geometry(geo_steps_per_circle=64)
                    o.options = {"name": f}
                    o.tools = {}
                    o.decimals = 4
                    o.import_dxf_as_geo(path, units="MM")
                    geos = list(o.solid_geometry)
                ok = True
            except Exception as e:  # noqa: BLE001
                print(f, gerber, "falla:", repr(e))
                geos, ok = [], False
            out.append("{%s, %s, %s, {%s}}," % (cs(f), str(gerber).lower(), str(ok).lower(),
                                                ", ".join(cs(g.wkt) for g in geos)))
            print(f, "gerber" if gerber else "geometry", len(geos), "figuras", flush=True)
    out += ["};", "}  // namespace golden_dxf_import"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
