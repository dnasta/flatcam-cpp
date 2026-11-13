#!/usr/bin/env python3
"""Oráculo de Film: ejecuta `on_film_creation` ORIGINAL de `appTools/ToolFilm.py` (con `generate_*_film`,
`export_negative` y `export_positive`, tomados con `ast`) con una interfaz simulada (el diálogo de guardar, los hilos,
`new_object` con su conversión de unidades, svglib/reportlab) sobre los archivos del usuario y escribe en
`tests/golden_film.h` el SVG de cada caso y, en el perforado, el Gerber `_punched`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_film.py
"""
import ast
import logging
import os
import sys
import tempfile
from contextlib import contextmanager
from copy import deepcopy

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_film.h")
CU = "luz_de_noche/luz de noche Pascual-B_Cu.gbr"
EDGE = "luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr"
LOGO = "luz_de_noche/luz de noche Pascual-Logo.gbr"
PTH = "luz_de_noche/luz de noche Pascual-PTH.drl"
PADS = "film/pads.gbr"


class Val:
    def __init__(self, v=None):
        self.v = v

    def get_value(self):
        return self.v

    def set_value(self, v):
        self.v = v

    def currentText(self):
        return self.v


class Emit:
    def __init__(self, sink=None):
        self.sink = sink

    def emit(self, *a):
        if self.sink is not None:
            self.sink.append(a[0] if len(a) == 1 else a)


def main():
    from lxml import etree as real_et  # antes de los simulacros de oracle_common
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.defaults.update({"excellon_circle_steps": 64, "gerber_def_units": "IN", "global_open_style": True, "tools_film_color": "#000000",
                         "units": "MM"})
    app.options = {"units": "MM"}
    app.log = logging.getLogger("oracle")
    msgs = []
    app.inform = Emit(msgs)
    app.file_opened = app.file_saved = Emit()
    app.get_last_save_folder = lambda: tempfile.gettempdir()
    app.worker_task = type("W", (), {"emit": staticmethod(lambda d: d['fcn'](*d['params']))})()

    @contextmanager
    def new_ctx(*a):
        yield

    app.proc_container = type("P", (), {"new": staticmethod(new_ctx),
                                        "update_view_text": staticmethod(lambda *a: None)})()
    class Defaults(dict):
        def report_usage(self, *a):
            pass

    app.defaults = Defaults(app.defaults)
    pg.Gerber.app = app
    pe.Excellon.app = app
    camlib.Geometry.app = app

    objs = {}

    def gerber(f, name=None):
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        g.kind = "gerber"
        g.options = {"name": name or os.path.basename(f)}
        objs[g.options["name"]] = g
        return g

    def excellon(f):
        e = pe.Excellon(geo_steps_per_circle=64)
        e.default_data = {}
        e.parse_file(os.path.join(DATA, f))
        e.create_geometry()
        e.kind = "excellon"
        objs[os.path.basename(f)] = e
        return e

    class Collection:
        def get_by_name(self, n):
            return objs.get(n)

    app.collection = Collection()
    created = {}

    def new_object(kind, name, init, **kw):
        o = pg.Gerber(steps_per_circle=64)
        o.kind = "gerber"
        o.options = {"name": name}
        init(o, app)
        # new_object: si las unidades no son las de la aplicación, las convierte.
        if app.options["units"].upper() != o.units.upper():
            o.convert_units(app.options["units"])
        objs[name] = o
        created[name] = o

    app.app_obj = type("AO", (), {"new_object": staticmethod(new_object)})()
    screen = {"dpi": 96}

    class Screen:
        def logicalDotsPerInch(self):
            return screen["dpi"]

    app.qapp = type("Q", (), {"screens": staticmethod(lambda: [Screen()])})()

    src = os.path.join(REF, "appTools", "ToolFilm.py")
    tree = ast.parse(open(src, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "Film")
    names = {"on_film_creation", "generate_positive_normal_film", "generate_positive_punched_film",
             "generate_negative_film", "export_negative", "export_positive"}
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in names]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ET = real_et
    from shapely.geometry import MultiPolygon, Point, Polygon
    import shapely.affinity as affinity
    from shapely.ops import unary_union
    from xml.dom.minidom import parseString as parse_xml_string
    from io import StringIO
    out_file = os.path.join(tempfile.gettempdir(), "oracle_film.svg")

    def dbg_parse(t):
        try:
            return parse_xml_string(t)
        except Exception:
            print("PARSE", t[:420], file=sys.stderr)
            raise
    captured = {}

    class Dialog:
        @staticmethod
        def get_saved_filename(caption=None, directory=None, ext_filter=None):
            captured["dir"] = directory
            return out_file, ""

    def svg2rlg(doc):
        captured["svg"] = doc.getvalue()
        return None

    class Render:
        @staticmethod
        def drawToFile(*a, **k):
            pass

        @staticmethod
        def draw(*a, **k):
            pass

    class Canvas:
        def __init__(self, filename, pagesize=None):
            captured["pagesize"] = pagesize

        def translate(self, x, y):
            captured["translate"] = (x, y)

        def save(self):
            pass

    ns = dict(MultiPolygon=MultiPolygon, Point=Point, Polygon=Polygon, affinity=affinity, unary_union=unary_union,
              deepcopy=deepcopy, ET=ET, parse_xml_string=dbg_parse, StringIO=StringIO,
              FCFileSaveDialog=Dialog, svg2rlg=svg2rlg, renderPM=Render, renderPDF=Render,
              canvas=type("C", (), {"Canvas": Canvas}), inch=72.0, mm=72.0 / 25.4,
              portrait=lambda s: (min(s), max(s)), landscape=lambda s: (max(s), min(s)),
              _=lambda s: s, log=type("L", (), {"debug": staticmethod(lambda m: errors.append(m))})())
    errors = []
    exec(compile(m, src, "exec"), ns)
    Core = ns["Core"]

    gerber(CU)
    gerber(EDGE)
    gerber(LOGO)
    gerber(PADS)
    excellon(PTH)
    # Una Geometría multiherramienta como caja (un rectángulo).
    geo = camlib.Geometry()
    geo.kind = "geometry"
    geo.multigeo = True
    from shapely.geometry import box as sbox
    geo.tools = {1: {"solid_geometry": [sbox(-40, -40, 40, 40).exterior]}}
    geo.solid_geometry = []
    geo.units = "MM"
    objs["box_geo"] = geo

    base = dict(film_type="neg", boundary=1.0, stroke=0, punch=False, source="exc", scale=False, sx=1.0, sy=1.0,
                skew=False, kx=0.0, ky=0.0, ref="bottomleft", mirror_cb=False, mirror="none", ftype="svg",
                orient="p", page="A4", dpi=96, obj=os.path.basename(EDGE), box=os.path.basename(EDGE),
                exc=os.path.basename(PTH), punch_size=0.3, screen=96)
    CASES = [
        ("neg", {}),
        ("neg_copper", dict(obj=os.path.basename(CU))),
        ("pos", dict(film_type="pos")),
        ("neg_transformed", dict(scale=True, sx=1.02, sy=0.98, skew=True, kx=1.0, mirror_cb=True, mirror="x")),
        ("pos_skew_topright", dict(film_type="pos", skew=True, kx=2.0, ky=-1.5, ref="topright", stroke=0.05)),
        ("pos_scale_only_y", dict(film_type="pos", scale=True, sy=1.1, skew=True, ref="center", mirror_cb=True,
                                  mirror="both")),
        ("neg_png_72dpi", dict(ftype="png", screen=72)),
        ("pos_pdf_a4", dict(film_type="pos", ftype="pdf", page="A4", orient="l")),
        ("neg_logo_box_geo", dict(obj=os.path.basename(LOGO), box="box_geo", boundary=2.5)),
        ("neg_no_box", dict(box="nothing")),
        ("pos_punch_exc", dict(film_type="pos", punch=True, source="exc", obj=os.path.basename(CU))),
        ("pos_punch_pad", dict(film_type="pos", punch=True, source="pad", obj="pads.gbr", box="pads.gbr")),
        ("pos_punch_pad_big", dict(film_type="pos", punch=True, source="pad", obj="pads.gbr", box="pads.gbr",
                                   punch_size=1.0)),
        ("pos_punch_pad_regions", dict(film_type="pos", punch=True, source="pad", obj=os.path.basename(CU))),
    ]
    lines = []
    for name, over in CASES:
        p = dict(base)
        p.update(over)
        c = Core()
        c.app = app
        c.units = "MM"
        c.decimals = 4
        c.screen_dpi = 96
        screen["dpi"] = p["screen"]
        u = type("U", (), {})()
        u.tf_object_combo, u.tf_box_combo, u.exc_combo = Val(p["obj"]), Val(p["box"]), Val(p["exc"])
        u.film_scale_stroke_entry, u.source_punch = Val(p["stroke"]), Val(p["source"])
        u.file_type_radio, u.film_type, u.punch_cb = Val(p["ftype"]), Val(p["film_type"]), Val(p["punch"])
        u.film_scale_cb, u.film_scalex_entry, u.film_scaley_entry = Val(p["scale"]), Val(p["sx"]), Val(p["sy"])
        u.film_skew_cb, u.film_skewx_entry, u.film_skewy_entry = Val(p["skew"]), Val(p["kx"]), Val(p["ky"])
        u.film_skew_reference, u.film_mirror_cb, u.film_mirror_axis = Val(p["ref"]), Val(p["mirror_cb"]), Val(p["mirror"])
        u.boundary_entry, u.pagesize_combo, u.orientation_radio = Val(p["boundary"]), Val(p["page"]), Val(p["orient"])
        u.png_dpi_spinner, u.punch_size_spinner = Val(p["dpi"]), Val(p["punch_size"])
        u.pagesize = {"Bounds": None, "A4": (210 * 72.0 / 25.4, 297 * 72.0 / 25.4)}
        c.ui = u
        captured.clear()
        created.clear()
        del msgs[:]
        if os.path.exists(out_file):
            os.remove(out_file)
        try:
            c.on_film_creation()
            err = ""
        except Exception as e:
            err = type(e).__name__
            import traceback
            traceback.print_exc(limit=-2)
        svg = ""
        if os.path.exists(out_file):
            svg = open(out_file).read()
        elif "svg" in captured:
            svg = captured["svg"]
        punched = ""
        area = 0.0
        nparts = 0
        bounds = [0.0] * 4
        for k, o in created.items():
            punched = k
            sg = o.solid_geometry
            area = sg.area
            nparts = len(sg.geoms) if hasattr(sg, "geoms") else 1
            bounds = list(sg.bounds)
        tr = captured.get("translate", (0.0, 0.0))
        warn = next((mm for mm in msgs if isinstance(mm, str) and ("WARNING" in mm or "ERROR" in mm)), "")
        full = svg if len(svg) < 300000 else ""
        h = 0xcbf29ce484222325
        for ch in svg.encode():
            h = ((h ^ ch) * 0x100000001b3) & 0xFFFFFFFFFFFFFFFF
        lines.append("  {%s, %s, %d, %duLL, %s, %s, %s, %s, %r, %d, {%s}, {%r, %r}}," % (
            cs(name), cs(full), len(svg), h, cs(svg[:3000]), cs(err), cs(warn), cs(punched), area, nparts, ", ".join(repr(b) for b in bounds),
            tr[0], tr[1]))
        print(name, len(svg), err, warn[:60], punched, [e for e in errors if "->" in e], file=sys.stderr)
        del errors[:]
    out = ["// GENERADO por tests/tools/oracle_film.py. No editar.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_film {",
           "struct Case { const char* name; std::string svg; std::size_t length; unsigned long long fnv; std::string head;"
           " const char* error; const char* warning;"
           " const char* punched; double area; int parts; std::vector<double> bounds; std::vector<double> translate; };",
           "inline const std::vector<Case> kCases = {"]
    out += lines
    out += ["};", "}  // namespace golden_film", ""]
    open(OUT, "w").write("\n".join(out))
    print("escrito", OUT)


if __name__ == "__main__":
    main()
