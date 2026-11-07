#!/usr/bin/env python3
"""Oráculo de exportar DXF: ejecuta `GeometryObject.export_dxf` ORIGINAL (tomado con `ast`, con ezdxf 0.14) sobre la
geometría del contorno y del logo de "luz de noche" más algunas líneas, y guarda el documento (sin los GUID, que son
nuevos cada vez) en `tests/golden_dxf_export.h`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_dxf_export.py
"""
import ast
import logging
import os
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_dxf_export.h")
CASES = [("luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr", ["LINESTRING (0 0, 10 0, 10 5)"]),
         ("luz_de_noche/luz de noche Pascual-Logo.gbr", ["MULTILINESTRING ((1 1, 2 2), (3 3, 4 5))", "POINT (5 5)"])]


def main():
    install_stubs()
    sys.modules.pop("ezdxf", None)
    import ezdxf
    from shapely import wkt
    from shapely.geometry import LinearRing, LineString, MultiLineString, MultiPolygon, Polygon
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.defaults.update({"gerber_buffering": "full", "gerber_circle_steps": 64})
    app.log = logging.getLogger("o")
    camlib.Geometry.app = app
    pg.Gerber.app = app
    tree = ast.parse(open(os.path.join(REF, "appObjects", "FlatCAMGeometry.py"), encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "GeometryObject")
    fn = next(f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name == "export_dxf")
    mod = ast.Module(body=[fn], type_ignores=[])
    ast.fix_missing_locations(mod)
    ns = dict(ezdxf=ezdxf, MultiPolygon=MultiPolygon, Polygon=Polygon, MultiLineString=MultiLineString,
              LineString=LineString, LinearRing=LinearRing, log=logging.getLogger("x"))
    exec(compile(mod, "FlatCAMGeometry.py", "exec"), ns)

    class Geo:
        export_dxf = ns["export_dxf"]
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_dxf_export.py - no editar a mano.",
           "// Referencia: GeometryObject.export_dxf original (ezdxf %s), sin los GUID ni las fechas." % ezdxf.__version__,
           "#pragma once", "#include <vector>", "namespace golden_dxf_export {",
           "struct Case { const char* gerber; std::vector<const char*> extra; const char* dxf; };",
           "inline const std::vector<Case> kCases = {"]
    for f, extra in CASES:
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        o = Geo()
        o.multigeo = True
        o.tools = {1: {"solid_geometry": list(g.solid_geometry) + [wkt.loads(w) for w in extra]}}
        doc = o.export_dxf()
        fd, path = tempfile.mkstemp(suffix=".dxf")
        os.close(fd)
        doc.saveas(path)
        lines = open(path, encoding="utf-8").read().split("\n")
        os.remove(path)
        keep = [l for i, l in enumerate(lines)
                if not (i > 1 and lines[i - 2] in ("$FINGERPRINTGUID", "$VERSIONGUID", "$TDCREATE", "$TDUPDATE"))]
        out.append("{%s, {%s}, %s}," % (cs(f), ", ".join(cs(w) for w in extra), cs("\n".join(keep))))
        print(f, len(lines), "líneas", flush=True)
    out += ["};", "}  // namespace golden_dxf_export"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
