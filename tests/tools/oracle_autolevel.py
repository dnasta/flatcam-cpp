#!/usr/bin/env python3
"""Oráculo de la Autonivelación: ejecuta `on_add_al_probepoints` (modo grilla), `generate_voronoi_geometry`,
`generate_bilinear_geometry`, `probing_gcode`, `import_height_map` y `on_grbl_jog` ORIGINALES de
`appObjects/FlatCAMCNCJob.py` (tomados con `ast`) y escribe `tests/golden_autolevel.h`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_autolevel.py
"""
import ast
import logging
import os
import random
import sys
import tempfile
from copy import deepcopy

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import REF, ROOT  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_autolevel.h")
TIME = "Thursday, 12 November 2026 at 10:00"


class Val:
    def __init__(self, v=None):
        self.v = v

    def get_value(self):
        return self.v

    def set_value(self, v):
        self.v = v


class Any:
    def __getattr__(self, n):
        return Any()

    def __call__(self, *a, **k):
        return Any()


class FixedDate:
    @staticmethod
    def now():
        class D:
            def __format__(self, spec):
                return TIME
        return D()


def main():
    from shapely.geometry import LineString, MultiPoint, Point, box
    from shapely.ops import unary_union, voronoi_diagram
    import shapely.affinity as affinity
    src = os.path.join(REF, "appObjects", "FlatCAMCNCJob.py")
    tree = ast.parse(open(src, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "CNCJobObject")
    names = {"on_add_al_probepoints", "generate_voronoi_geometry", "generate_bilinear_geometry", "probing_gcode",
             "import_height_map", "on_grbl_jog"}
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in names]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(unary_union=unary_union, voronoi_diagram=voronoi_diagram, MultiPoint=MultiPoint, Point=Point,
              affinity=affinity, random=random, deepcopy=deepcopy, datetime=FixedDate, VORONOI_ENABLED=True,
              _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, src, "exec"), ns)
    Core = ns["Core"]

    class App:
        version = "2024.4"
        version_date = "2024/04/15"
        decimals = 4
        defaults = {"cncjob_al_grbl_travelz": 15.0}
        inform = Any()

        @staticmethod
        def dec_format(val, dec=None):
            return float('%.*f' % (dec if dec is not None else 4, val))

    parsed = [
        {"geom": LineString([(0, 0), (-3.21, 5.5)]), "kind": ["T", "F"]},
        {"geom": LineString([(-3.21, 5.5), (40.123456, 5.5), (40.123456, 31.7)]), "kind": ["C", "F"]},
        {"geom": LineString([(40.123456, 31.7), (2.0, 31.7)]), "kind": ["C", "F"]},
        {"geom": LineString([(2.0, 31.7), (80, 80)]), "kind": ["T", "F"]},
    ]

    def new(units="MM", method="v", rows=4, cols=4, controller="MACH3"):
        c = Core()
        c.app = App()
        c.units = units
        c.coords_decimals = 4
        c.fr_decimals = 2
        c.options = {"name": "B_Cu_cnc"}
        c.gcode_parsed = parsed
        c.al_voronoi_geo_storage = {}
        c.al_bilinear_geo_storage = []
        c.probing_gcode_text = ""
        c.build_al_table_sig = Any()
        c.show_probing_geo = lambda **k: None
        c.plot_probing_geo = lambda *a, **k: None
        u = type("U", (), {})()
        u.al_probe_points_table = Any()
        u.al_mode_radio, u.al_method_radio = Val("grid"), Val(method)
        u.al_rows_entry, u.al_columns_entry = Val(rows), Val(cols)
        u.plot_probing_pts_cb = Val(False)
        u.ptravelz_entry, u.feedrate_probe_entry, u.pdepth_entry = Val(2.0), Val(120.0), Val(-1.0)
        u.al_controller_combo = Val(controller)
        u.jog_step_entry, u.jog_fr_entry = Val(5.0), Val(1500.0)
        c.ui = u
        sent = []
        c.send_grbl_command = lambda command, echo=True: sent.append(command)
        c.sent = sent
        return c

    def grid(c):
        c.on_add_al_probepoints()

    lines = []

    def record(name, c, gcode):
        pts, cells = [], []
        for k, v in c.al_voronoi_geo_storage.items():
            pts += [v['point'].x, v['point'].y, v['height']]
            g = v['geo']
            cells += ([g.area] + list(g.bounds)) if g is not None else [-1.0, 0, 0, 0, 0]
        lines.append("  {%s, {%s}, {%s}, %s}," % (cs(name), ", ".join(repr(float(x)) for x in pts),
                                                 ", ".join(repr(float(x)) for x in cells), cs(gcode or "")))

    for name, kw in [("grid4x4_v_mach3", {}), ("grid3x2_v_in_linuxcnc", dict(units="IN", rows=3, cols=2,
                                                                            controller="LinuxCNC")),
                     ("grid1x3_b_mach4", dict(method="b", rows=1, cols=3, controller="MACH4")),
                     ("grid2x2_v_grbl", dict(rows=2, cols=2, controller="GRBL"))]:
        c = new(**kw)
        grid(c)
        record(name, c, c.probing_gcode_text)
    # Manual: el primer punto es (xmin, xmin) (el original repite xmin) y después los clics; luego Voronoi.
    c = new()
    c.al_voronoi_geo_storage = {1: {'point': Point(-3.21, -3.21), 'geo': None, 'height': 0.0},
                                2: {'point': Point(10, 10), 'geo': None, 'height': 0.0},
                                3: {'point': Point(30.5, 20.25), 'geo': None, 'height': 0.0}}
    c.solid_geo = unary_union([g['geom'] for g in parsed if g['kind'][0] == 'C'])
    c.generate_voronoi_geometry(pts=[v['point'] for v in c.al_voronoi_geo_storage.values()])
    record("manual_v", c, c.probing_gcode(c.al_voronoi_geo_storage))
    # Mapa de alturas: completa los puntos que faltan.
    hm = os.path.join(tempfile.gettempdir(), "oracle_heightmap.txt")
    with open(hm, "w") as f:
        f.write("1.5 2.5 -0.125\n3,4,0.25\n5 6 0.5\n")
    c.import_height_map(hm)
    record("heightmap", c, "")
    # Avance manual con GRBL.
    jog = []
    for d in ["xplus", "xminus", "yplus", "yminus", "zplus", "zminus", "origin"]:
        c = new()
        c.on_grbl_jog(direction=d)
        jog.append(cs("|".join(c.sent)))
    out = ["// GENERADO por tests/tools/oracle_autolevel.py. No editar.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_autolevel {",
           "struct Case { const char* name; std::vector<double> points; std::vector<double> cells; std::string gcode; };",
           "inline const char* kTime = %s;" % cs(TIME), "inline const std::vector<Case> kCases = {"]
    out += lines
    out += ["};", "inline const std::vector<std::string> kJog = {%s};" % ", ".join(jog),
            "}  // namespace golden_autolevel", ""]
    open(OUT, "w").write("\n".join(out))
    print("escrito", OUT)


if __name__ == "__main__":
    main()
