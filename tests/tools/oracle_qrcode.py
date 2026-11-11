#!/usr/bin/env python3
"""Oráculo del código QR: ejecuta la biblioteca `qrcode` 7.4.2 y `ToolQRCode.execute`, `convert_svg_to_geo`, `make`
y la exportación a SVG ORIGINALES (tomados con `ast`) y escribe `tests/golden_qrcode.h`: la versión y los módulos, el SVG
exportado, la geometría (módulos y unión) y el Gerber con el código insertado.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_qrcode.py
(con `python -m pip install qrcode==7.4.2` en el entorno).
"""
import ast
import logging
import math
import os
import sys
from collections.abc import Iterable
from copy import deepcopy
from io import BytesIO, StringIO
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_qrcode.h")
GBR = "luz_de_noche/luz de noche Pascual-B_Cu.gbr"
LONG = "FlatCAM-cpp: port de FlatCAM 2024.4 a C++ con Qt6. " * 6
# (texto, versión, nivel, box_size, borde, polaridad, esquinas, posición del clic o None)
CASES = [
    ("https://flatcam.org", 1, "L", 3, 4, "pos", "s", (0.0, 0.0)),
    ("HELLO WORLD", 1, "M", 3, 4, "neg", "r", (5.0, -3.0)),
    ("12345678901234567890123", 1, "Q", 10, 0, "pos", "s", (100.0, 100.0)),
    ("ABC12345678901234567890123xyz", 2, "H", 3, 2, "neg", "s", None),
    ("Pascual ñandú ¡hola!", 1, "L", 3, 4, "pos", "r", None),
    ("123\n", 1, "L", 3, 4, "pos", "s", None),
    ("1234567890", 1, "M", 5, 1, "pos", "s", None),
    ("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 3, "L", 3, 4, "pos", "s", None),
    ("ab", 5, "M", 3, 4, "pos", "s", None),
    ("version siete", 7, "L", 2, 4, "pos", "s", None),
    (LONG, 1, "H", 3, 4, "neg", "r", (-10.0, 8.0)),
    (LONG * 3, 1, "L", 1, 4, "pos", "s", None),
    ("x" * 3000, 1, "H", 3, 4, "pos", "s", None),   # no entra: DataOverflowError
]


def methods(path, clsname, names):
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == clsname)
    return [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in names]


def cbytes(s):
    out = '"'
    for b in s.encode("utf-8"):
        c = chr(b)
        if c in '"\\':
            out += "\\" + c
        elif 32 <= b < 127:
            out += c
        else:
            out += '\\x%02x""' % b
    return out + '"'


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v


def main():
    install_stubs()
    for m in ("svg", "svg.path", "lxml", "lxml.etree", "appParsers.ParseSVG"):
        sys.modules.pop(m, None)
    sys.modules["freetype"] = mock.MagicMock()
    sys.modules["fontTools"] = mock.MagicMock()
    sys.modules["fontTools.ttLib"] = mock.MagicMock()
    sys.path.insert(0, REF)
    pfont = load("appParsers.ParseFont", os.path.join(REF, "appParsers", "ParseFont.py"))
    psvg = load("appParsers.ParseSVG", os.path.join(REF, "appParsers", "ParseSVG.py"))
    sys.modules["appParsers.ParseDXF"] = pfont
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    app = FakeApp()
    app.defaults.update({"geometry_circle_steps": 64, "gerber_circle_steps": 64, "units": "MM"})
    app.log = logging.getLogger("oracle")
    app.plotcanvas = mock.MagicMock()
    app.is_legacy = False
    camlib.Geometry.app = app
    pg.Gerber.app = app
    import qrcode
    import qrcode.image.pil
    import qrcode.image.svg
    from lxml import etree as ET
    from shapely import wkt as W
    from shapely.affinity import scale, translate
    from shapely.geometry import MultiPolygon, box
    from shapely.ops import unary_union
    path = os.path.join(REF, "appTools", "ToolQRCode.py")
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], decorator_list=[], body=methods(
        path, "QRCode", {"execute", "make", "convert_svg_to_geo", "flatten_list", "export_svg_file"}))],
        type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(vars(psvg))
    ns.update(qrcode=qrcode, BytesIO=BytesIO, StringIO=StringIO, ET=ET, translate=translate, scale=scale, box=box,
              unary_union=unary_union, MultiPolygon=MultiPolygon, Iterable=Iterable, deepcopy=deepcopy, math=math,
              _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    app.worker_task = mock.MagicMock()
    app.worker_task.emit = lambda d: d["fcn"](*d["params"])
    app.proc_container = mock.MagicMock()
    app.inform = mock.MagicMock()
    d = lambda x: W.dumps(x, rounding_precision=17, trim=True)  # noqa: E731
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_qrcode.py - no editar a mano.",
           "// Referencia: qrcode 7.4.2 y ToolQRCode.execute / convert_svg_to_geo / make / export_svg_file originales.",
           "#pragma once", "#include <string>", "#include <vector>", "namespace golden_qrcode {",
           "struct QrCase { std::string data; int version; char ec; int box; int border; const char* polarity;"
           " const char* rounded; bool make; double px; double py; bool overflow; int finalVersion;"
           " std::vector<std::string> rows; std::string svgPath; int modules; double modulesArea;"
           " std::string firstModule; int utilityParts; double utilityArea; std::vector<double> utilityBounds;"
           " int solid; double solidArea; std::string newAp; double apSize; double apWidth; double apHeight;"
           " int apCount; std::string apFirst; std::string apFirstFollow; std::string clear; int zeroCount; };",
           "inline const std::vector<QrCase> kCases = {"]
    for data, version, ec, bsize, border, pol, rnd, pos in CASES:
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, GBR))
        g.options = {"name": "cu"}
        zero_before = len(g.apertures.get("0", {}).get("geometry", []))
        core = ns["Core"]()
        core.app = app
        core.units = "MM"
        core.origin = (0, 0)
        core.ui = mock.MagicMock()
        for k, v in dict(text_data=data, version_entry=version, error_radio=ec, bsize_entry=bsize,
                         border_size_entry=border, pol_radio=pol, bb_radio=rnd, fill_color_entry="#000000",
                         back_color_entry="#FFFFFF").items():
            setattr(core.ui, k, Val(v))
        idx = mock.MagicMock()
        idx.internalPointer.return_value.obj = g
        app.collection = mock.MagicMock()
        app.collection.index.return_value = idx
        core.on_exit = core.replot = lambda *a, **k: None
        core.on_mouse_move = core.on_mouse_release = core.on_key_release = None
        app.f_handlers = mock.MagicMock()
        app.f_handlers.export_gerber.return_value = ""
        overflow = False
        try:
            core.execute()
        except (ValueError, qrcode.exceptions.DataOverflowError):  # versión 41: el setter da ValueError
            overflow = True
        rows, svgp, fv = [], "", 0
        if not overflow:
            qr = qrcode.QRCode(version=version, error_correction={"L": 1, "M": 0, "Q": 3, "H": 2}[ec],
                               box_size=bsize, border=border)
            qr.add_data(data)
            qr.make()
            fv = qr.version
            rows = ["".join("1" if c else "0" for c in r) for r in qr.modules]
            # export_svg_file con un archivo en memoria.
            buf = BytesIO()
            qrcode.image.svg.SvgPathImage.save = (lambda self, stream, kind=None: self._write(buf))
            app.get_last_save_folder = lambda: ""
            with mock.patch.dict(ns, {"FCFileSaveDialog": mock.MagicMock()}):
                ns["FCFileSaveDialog"].get_saved_filename.return_value = ("x.svg", "")
                core.export_svg_file()
            svgp = buf.getvalue().decode("utf-8") if fv <= 10 else ""
        mods, mods_area, first, uparts, uarea, ub = 0, 0.0, "", 0, 0.0, []
        solid, sarea, newap, aps, apw, aph, apc, apf, apff, clear, zc = 0, 0.0, "", 0.0, 0.0, 0.0, 0, "", "", "", 0
        if not overflow:
            mods = len(core.qrcode_geometry)
            mods_area = sum(p.area for p in core.qrcode_geometry)
            first = d(core.qrcode_geometry[0])
            u = core.qrcode_utility_geometry
            uparts = len(u.geoms) if hasattr(u, "geoms") else 1
            uarea, ub = u.area, list(u.bounds)
            if pos is not None:
                before = set(g.apertures.keys())
                core.make(pos=pos)
                sg = g.solid_geometry
                solid, sarea = len(sg), sum(p.area for p in sg)
                newap = next(k for k in g.apertures if k not in before and k != "0")
                ap = g.apertures[newap]
                aps, apw, aph, apc = ap["size"], ap["width"], ap["height"], len(ap["geometry"])
                apf, apff = d(ap["geometry"][0]["solid"]), d(ap["geometry"][0]["follow"])
                clear = d(g.apertures["0"]["geometry"][-1]["clear"])
                zc = len(g.apertures["0"]["geometry"]) - zero_before
        out.append("{%s, %d, '%s', %d, %d, \"%s\", \"%s\", %s, %r, %r, %s, %d, {%s}, %s, %d, %r, \"%s\", %d, %r, {%s},"
                   " %d, %r, \"%s\", %r, %r, %r, %d, \"%s\", \"%s\", \"%s\", %d}," % (
                       cbytes(data), version, ec, bsize, border, pol, rnd, "true" if pos else "false",
                       (pos or (0, 0))[0], (pos or (0, 0))[1], "true" if overflow else "false", fv,
                       ", ".join('"%s"' % r for r in rows), cbytes(svgp), mods, mods_area, first, uparts, uarea,
                       ", ".join(repr(v) for v in ub), solid, sarea, newap, aps, apw, aph, apc, apf, apff, clear, zc))
        print(repr(data[:20]), version, ec, "->", fv, overflow, mods, uparts, round(uarea, 4), solid, newap, apc, zc)
    out += ["};", "}  // namespace golden_qrcode"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
