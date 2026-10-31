#!/usr/bin/env python3
"""Oráculo del pintado estándar: ejecuta `Geometry.clear_polygon` y `geometry_tool_gcode_gen` ORIGINALES.

Necesita Shapely 1.8.4 **y el paquete `rtree` real** (el original lo usa para buscar el extremo más cercano).
Entrada: los polígonos del `Logo` del usuario (leídos con el parser original y reflejados en X como en su proyecto) y
varios casos sintéticos. Salida: `tests/golden_paint.h` con los polígonos de entrada, las trayectorias que genera la
limpieza y el G-code completo que sale de recorrerlas con el orden `R` y la tolerancia de simplificación.

Entorno:  micromamba create -n old -c conda-forge python=3.10 shapely=1.8.4 numpy=1.22 rtree
Uso:      python tests/tools/oracle_paint.py
"""
import copy
import json
import os
import sys

from shapely.geometry import LinearRing, LineString, MultiPolygon, Polygon

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import ExcAreas, cs, num, pair  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_paint.h")
TOLERANCE = 0.00025  # global_tolerance / 20, como en FlatCAMGeometry.generate_cncjob


def poly(ext, holes=()):
    return Polygon(ext, holes)


SQUARE_HOLE = poly([(0, 0), (8, 0), (8, 6), (0, 6)], [[(2.5, 1.5), (5.5, 1.5), (5.5, 4.5), (2.5, 4.5)]])
TWO = [poly([(0, 0), (4, 0), (4, 3), (0, 3)]), poly([(6, 1), (11, 1), (11, 5), (6, 5)])]
L_SHAPE = poly([(0, 0), (6, 0), (6, 2), (2, 2), (2, 6), (0, 6)])
SMALL = poly([(0, 0), (0.1, 0), (0.1, 0.1), (0, 0.1)])
MULTI = MultiPolygon([poly([(0, 0), (6, 0), (6, 6), (0, 6)]), poly([(10, 0), (16, 0), (16, 6), (10, 6)])])
RING = LinearRing([(0, 0), (9, 0), (9, 7), (0, 7), (0, 0)])

# nombre, entrada, opciones
SCENARIOS = [
    ("square_with_hole", [SQUARE_HOLE], dict(dia=0.5, over=0.4, conn=True, off=0.0)),
    ("square_with_hole_no_connect", [SQUARE_HOLE], dict(dia=0.5, over=0.4, conn=False, off=0.0)),
    ("two_shapes_with_offset", TWO, dict(dia=0.3, over=0.2, conn=True, off=0.1)),
    ("l_shape", [L_SHAPE], dict(dia=0.4, over=0.5, conn=True, off=0.0)),
    ("too_small_for_the_tool", [SMALL], dict(dia=0.5, over=0.4, conn=True, off=0.0)),
    ("multipolygon_input", [MULTI], dict(dia=0.6, over=0.3, conn=True, off=0.0)),
    ("closed_ring_input", [RING], dict(dia=0.5, over=0.5, conn=True, off=0.0)),
    ("fine_tool_high_overlap", [SQUARE_HOLE], dict(dia=0.3, over=0.6, conn=True, off=0.0)),
]


def flatten_input(geoms):
    """`recurse` de `paint_poly_all`: figuras simples, los anillos pasan a polígonos."""
    out = []

    def rec(g, top=True):
        try:
            for x in g:
                if x and not x.is_empty and x.is_valid:
                    rec(x, False)
        except TypeError:
            out.append(Polygon(g) if isinstance(g, LinearRing) else g)
    rec(geoms)
    return out


def paint_all(geo, geoms, dia, over, conn, off, steps=64):
    """Transcripción del pintado normal de `ToolPaint.paint_geo` (`job_normal_clear`) para una herramienta."""
    poly_buf = []
    for pol in flatten_input(geoms):
        b = pol.buffer(-off)
        if b and not b.is_empty:
            poly_buf.append(b)
    total = []
    for pp in poly_buf:
        res = geo.clear_polygon(pp, tooldia=dia, steps_per_circle=steps, overlap=over, contour=True, connect=conn)
        if res:
            total += list(res.get_objects())
    return [g for g in total if g and not g.is_empty]


def gcode_for(camlib, appPreProcessor, lines, tool_data, tooldia, pp_name, name):
    """G-code de las trayectorias con el orden R y la tolerancia, usando el generador original."""
    from shapely.ops import unary_union
    job = camlib.CNCjob(units="MM", kind="geometry", steps_per_circle=64, pp_geometry_name=pp_name)
    job.machinist_setting = 0
    job.coords_decimals = 4
    job.fr_decimals = 2
    job.oldx = 0.0
    job.oldy = 0.0
    job.segx = 0.0
    job.segy = 0.0
    xmin, ymin, xmax, ymax = unary_union(lines).bounds
    job.options = {"name": name, "type": "Geometry", "tool_dia": tooldia, "xmin": xmin, "ymin": ymin, "xmax": xmax,
                   "ymax": ymax}
    data = copy.deepcopy(tool_data)
    data["optimization_type"] = "R"
    data["ppname_g"] = pp_name
    tools = {1: {"tooldia": tooldia, "offset": "Path", "offset_value": 0.0, "solid_geometry": lines, "data": data}}
    gcode, start = job.geometry_tool_gcode_gen(1, tools, first_pt=(0.0, 0.0), tolerance=TOLERANCE, is_first=True,
                                               is_last=True, toolchange=False)
    return start + "\n" + gcode, data


def cpp_poly(g):
    if isinstance(g, LinearRing):
        return "flatcam::Shape::linearRing({%s})" % ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in g.coords)
    holes = ", ".join("{" + ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in h.coords[:-1]) + "}" for h in g.interiors)
    return "flatcam::Shape::polygon({%s}, {%s})" % (
        ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in g.exterior.coords[:-1]), holes)


def cpp_lines(lines):
    return "{" + ", ".join("{" + ", ".join("{%s, %s}" % (num(x), num(y)) for x, y in l.coords) + "}" for l in lines) + "}"


def settings_cpp(idx, data, tooldia, pp_name):
    out = ["inline flatcam::GeometryJobSettings settings%d() {" % idx, "    flatcam::GeometryJobSettings s;",
           '    s.units = "MM";', '    s.preprocessor = "%s";' % pp_name, "    s.order = flatcam::DrillOrder::RTree;",
           "    s.tool = 1;", "    s.toolchange = false;", "    s.flavor = flatcam::GcodeFlavor::FlatCAM2024_4;",
           "    flatcam::GeometryToolSettings& t = s.toolSettings;", "    t.toolDia = %s;" % num(tooldia),
           "    t.tolerance = %s;" % num(TOLERANCE)]
    for key, cpp in [("cutz", "cutZ"), ("depthperpass", "depthPerPass"), ("travelz", "travelZ"), ("feedrate", "feedrate"),
                     ("feedrate_z", "feedrateZ"), ("feedrate_rapid", "feedrateRapid"), ("spindlespeed", "spindleSpeed"),
                     ("dwelltime", "dwellTime"), ("endz", "endZ"), ("toolchangez", "toolchangeZ")]:
        out.append("    t.%s = %s;" % (cpp, num(data[key])))
    out.append('    t.spindleDir = "CW";')
    if data["endxy"]:
        out.append("    t.endXY = %s;" % pair(data["endxy"]))
    if data["toolchangexy"]:
        out.append("    t.toolchangeXY = %s;" % pair(data["toolchangexy"]))
    return out + ["    return s;", "}"]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    import appPreProcessor  # noqa: E402
    default_mod = load("pp_default", os.path.join(REF, "preprocessors", "default.py"))
    load("pp_laser", os.path.join(REF, "preprocessors", "GRBL_laser.py"))
    import math
    default_mod.math = math

    app = FakeApp()
    app.defaults.update({"cncjob_steps_per_circle": 64, "cncjob_coords_type": "G90", "geometry_f_plunge": False,
                         "geometry_circle_steps": 64, "geometry_spindledir": "CW"})
    app.preprocessors = appPreProcessor.preprocessors
    app.exc_areas = ExcAreas()
    app.dec_format = lambda v, dec=4: float("%.*f" % (dec, v))
    for cls in (camlib.Geometry, camlib.CNCjob, pg.Gerber):
        cls.app = app
    geo = camlib.Geometry(geo_steps_per_circle=64)

    proj = json.load(open(os.path.join(DATA, "dosificador_PCB_principal.params.json")))
    paint_obj = [o for o in proj["objs"] if o.get("options", {}).get("name") == "PCB principal-B_Logo.gbr_mt_paint"][0]
    tool_data = paint_obj["tools"]["1"]["data"]

    # ---- Logo del usuario con el parser original, reflejado en X
    g = pg.Gerber(steps_per_circle=64)
    g.parse_file(os.path.join(DATA, "PCB principal-B_Logo.gbr"))
    g.mirror("Y", (0, 0))
    logo = flatten_input(g.solid_geometry)
    scenarios = [("user_logo_mirrored", logo, dict(dia=0.1, over=0.8, conn=True, off=0.0))] + SCENARIOS

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_paint.py - no editar a mano.",
           "// Referencia: Geometry.clear_polygon y CNCjob.geometry_tool_gcode_gen originales (FlatCAM 2024.4), Shapely %s." %
           __import__("shapely").__version__,
           "#pragma once", "#include <string>", "#include <vector>", '#include "cam/CncJob.h"', '#include "tools/Paint.h"',
           "namespace golden_paint {", "using Lines = std::vector<std::vector<flatcam::Point2D>>;"]
    entries = []
    for idx, (name, geoms, o) in enumerate(scenarios):
        lines = paint_all(geo, geoms, o["dia"], o["over"], o["conn"], o["off"])
        print("%-30s trazados=%d puntos=%d" % (name, len(lines), sum(len(l.coords) for l in lines)), flush=True)
        pp_name = tool_data["ppname_g"] if idx == 0 else "default"
        text = ""
        data = tool_data
        if lines:
            text, data = gcode_for(camlib, appPreProcessor, lines, tool_data, o["dia"], pp_name, name)
        flat = flatten_input(geoms)
        out.append("inline flatcam::ShapeList input%d() {  // %s" % (idx, name))
        out.append("    return {%s};" % ", ".join(cpp_poly(p) for p in flat))
        out.append("}")
        out += settings_cpp(idx, data, o["dia"], pp_name)
        entries.append((name, o, lines, text))
    out.append("struct Scenario { const char* name; flatcam::ShapeList (*input)(); double dia; double overlap; bool connect;"
               " double offset; bool expectEmpty; Lines lines; flatcam::GeometryJobSettings (*settings)(); const char* gcode; };")
    out.append("inline const std::vector<Scenario> kScenarios = {")
    for idx, (name, o, lines, text) in enumerate(entries):
        out.append('{%s, input%d, %s, %s, %s, %s, %s, %s, settings%d, %s},' % (
            cs(name), idx, num(o["dia"]), num(o["over"]), str(o["conn"]).lower(), num(o["off"]),
            str(not lines).lower(), cpp_lines(lines), idx, cs(text)))
    out.append("};")
    out.append("}  // namespace golden_paint")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.1f MB)" % (os.path.getsize(OUT) / 1e6))


if __name__ == "__main__":
    main()
