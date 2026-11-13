#!/usr/bin/env python3
"""Oráculo de Importar PDF: arma PDF sintéticos en `tests/data/pdf/` (streams FlateDecode con todos los operadores que
entiende el original) y ejecuta `PdfParser.parse_pdf`, `ToolPDF.open_pdf` (la extracción de los streams),
`layer_rendering_as_gerber` y `layer_rendering_as_excellon` ORIGINALES (tomados con `ast`). Escribe
`tests/golden_pdf.h` con las capas (aperturas, tamaños, elementos y áreas) y lo que queda en cada objeto.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_pdf.py
"""
import ast
import logging
import os
import re
import sys
import zlib
from copy import copy, deepcopy

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_pdf.h")

# Contenidos de página (cada uno es un stream). Coordenadas en puntos.
PAGE1 = """q
0 0 0 RG
0 0 0 rg
1.5 w
1 0 0 1 20 30 cm
0 0 m
100 0 l
100 50 l
S
Q
q 2 0 0 2 0 0 cm
10 10 30 20 re
f
Q
1.2 w
50 60 m
80 60 l
80 90 l
h
50 100 m
60 100 l
60 110 l
h
S
1 1 1 rg
200 200 m
210 200 210 210 200 210 c
190 210 190 200 200 200 c
f
230 200 m
236 200 236 206 230 206 c
224 206 224 200 230 200 c
f
0 0 0 rg
300 300 m
320 300 l
320 320 l
300 320 l
h
f
300 400 m
320 400 300 430 v
280 420 300 400 y
B
0.8 w
10 400 m
40 400 l
W n
10 500 m
40 500 l
S
1 0 0 RG
2 w
100 100 m
150 150 l
S
150 150 20 10 re
B
"""

PAGE2 = """0 0 1 RG
0.5 w
5 5 m
50 5 l
S
1 1 1 rg
100 100 m
104 100 104 104 100 104 c
96 104 96 100 100 100 c
f
"""


def make_pdf(path, pages):
    objs = []
    content_ids = []
    for p in pages:
        data = zlib.compress(p.encode())
        objs.append(b"<< /Length %d /Filter /FlateDecode >>\nstream\r\n" % len(data) + data + b"\r\nendstream")
        content_ids.append(len(objs))
    body = b"%PDF-1.4\n"
    offsets = []
    for i, o in enumerate(objs, 1):
        offsets.append(len(body))
        body += b"%d 0 obj\n" % i + o + b"\nendobj\n"
    body += b"trailer\n<< /Size %d >>\n%%%%EOF\n" % (len(objs) + 1)
    open(path, "wb").write(body)


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    import numpy as np
    from shapely.geometry import LineString, MultiPolygon, Point, Polygon
    from shapely.ops import unary_union
    app = FakeApp()
    app.defaults.update({"gerber_circle_steps": 64, "units": "MM", "excellon_circle_steps": 64})
    app.log = logging.getLogger("oracle")
    app.abort_flag = False
    pe.Excellon.app = app

    def take(path, clsname, names, extra):
        tree = ast.parse(open(path, encoding="utf-8").read())
        cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == clsname)
        meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in names]
        m = ast.Module(body=[ast.ClassDef(name="C", bases=[], keywords=[], body=meths, decorator_list=[])],
                       type_ignores=[])
        ast.fix_missing_locations(m)
        ns = dict(extra)
        exec(compile(m, path, "exec"), ns)
        return ns["C"]

    base = dict(re=re, np=np, copy=copy, deepcopy=deepcopy, Polygon=Polygon, LineString=LineString,
                MultiPolygon=MultiPolygon, Point=Point, unary_union=unary_union, zlib=zlib,
                log=logging.getLogger("x"), _=lambda s: s, grace=Exception)
    Parser = take(os.path.join(REF, "appParsers", "ParsePDF.py"), "PdfParser", {"__init__", "parse_pdf",
                                                                               "bezier_to_points"}, base)
    Tool = take(os.path.join(REF, "appTools", "ToolPDF.py"), "ToolPDF",
                {"layer_rendering_as_excellon", "layer_rendering_as_gerber"}, base)

    files = {"two_pages.pdf": [PAGE1, PAGE2], "one_page.pdf": [PAGE2]}
    lines = []
    texts = []
    for f, pages in files.items():
        path = os.path.join(DATA, "pdf", f)
        make_pdf(path, pages)
        pdf = open(path, "rb").read()
        text = ""
        for s in re.findall(re.compile(b'.*?FlateDecode.*?stream(.*?)endstream', re.S), pdf):
            s = s.strip(b'\r\n')
            text += zlib.decompress(s).decode('UTF-8') + '\r\n'
        texts.append("  {%s, %s}," % (cs(f), __import__("json").dumps(text)))
        p = Parser.__new__(Parser)
        Parser.__init__.__wrapped__ if hasattr(Parser.__init__, "__wrapped__") else None
        p.app = app
        # __init__ llama a super().__init__() (QObject): se arma a mano
        src_init = Parser.__init__
        try:
            src_init(p, app)
        except Exception:
            pass
        layers = p.parse_pdf(pdf_content=text)
        for k in sorted(layers):
            aps = layers[k]
            desc = []
            for apid, ap in aps.items():
                solid = sum(e['solid'].area for e in ap['geometry'] if 'solid' in e)
                clear = sum(e['clear'].area for e in ap['geometry'] if 'clear' in e)
                desc.append("{%s, %r, %d, %r, %r}" % (cs(apid), float(ap['size']), len(ap['geometry']), solid, clear))
            # Lo que deja cada capa en su objeto
            t = Tool()
            t.app = app
            created = {}

            class AO:
                @staticmethod
                def new_object(kind, name, init, **kw):
                    if kind == "excellon":
                        o = pe.Excellon(geo_steps_per_circle=64)
                        o.default_data = {}
                        o.tools = {}
                    else:
                        o = type("G", (), {})()
                    init(o, app)
                    created[name] = o

            app.app_obj = AO()
            app.proc_container = type("P", (), {"new": staticmethod(lambda *a: __import__("contextlib").nullcontext())})()
            app.file_opened = type("E", (), {"emit": staticmethod(lambda *a: None)})()
            res = ""
            if k == 0:
                t.layer_rendering_as_excellon(path, aps, k)
                o = list(created.values())[0]
                parts = []
                for tool in sorted(o.tools, key=int):
                    td = o.tools[tool]
                    parts.append("{%r, {%s}}" % (float(td['tooldia']),
                                                 ", ".join("%r, %r" % (d.x, d.y) for d in td['drills'])))
                res = "{}, 0.0, {}, {%s}" % ", ".join(parts)
            else:
                t.layer_rendering_as_gerber(path, aps, k)
                o = list(created.values())[0]
                sg = o.solid_geometry
                res = "{%s}, %r, {%s}, {}" % (", ".join(repr(b) for b in sg.bounds), sg.area,
                                              "%d" % (len(sg.geoms) if hasattr(sg, "geoms") else 1))
            lines.append("  {%s, %d, {%s}, %s}," % (cs(f), k, ", ".join(desc), res))
            print(f, k, len(aps), file=sys.stderr)
    out = ["// GENERADO por tests/tools/oracle_pdf.py. No editar.", "#pragma once", "#include <string>",
           "#include <utility>", "#include <vector>", "namespace golden_pdf {",
           "struct Ap { const char* code; double size; int elements; double solidArea; double clearArea; };",
           "struct Tool { double dia; std::vector<double> drills; };",
           "struct Layer { const char* file; int layer; std::vector<Ap> apertures; std::vector<double> bounds;"
           " double area; std::vector<int> parts; std::vector<Tool> tools; };",
           "inline const std::vector<std::pair<const char*, std::string>> kTexts = {"] + texts + ["};",
           "inline const std::vector<Layer> kLayers = {"] + lines + ["};", "}  // namespace golden_pdf", ""]
    open(OUT, "w").write("\n".join(out))
    print("escrito", OUT)


if __name__ == "__main__":
    main()
