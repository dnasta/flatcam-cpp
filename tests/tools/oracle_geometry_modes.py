#!/usr/bin/env python3
"""Oráculo de los modos del G-code de geometría: profundidad múltiple, corte extra, puntos, tolerancia, segmentación.

Ejecuta `CNCjob.geometry_tool_gcode_gen` ORIGINAL (FlatCAM 2024.4) sobre varias geometrías sintéticas con distintas
opciones y escribe el G-code resultante en `tests/golden_geometry_modes.h`, junto con el código C++ que arma las mismas
geometrías y parámetros. Los tests comparan el texto completo.

Requisitos (solo para regenerar el archivo): python3 -m pip install shapely numpy
Uso:  python3 tests/tools/oracle_geometry_modes.py
"""
import copy
import json
import os
import sys

from shapely import wkt
from shapely.ops import unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_geometry_modes.h")

SQUARE = "POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))"
SQUARE_HOLE = "POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0), (2 2, 4 2, 4 4, 2 4, 2 2))"
LINE = "LINESTRING (0 0, 10 0, 10 5)"
NOISY = "LINESTRING (0 0, 1 0.01, 2 -0.01, 3 0.02, 4 0, 5 0.01, 6 -0.02, 7 0, 8 0.01, 9 0, 10 0)"
LONG = "LINESTRING (0 0, 9 0, 9 7)"
CLOSED_LINESTRING = "LINESTRING (0 0, 6 0, 6 6, 0 6, 0 0)"

# nombre, geometrías (WKT), cambios sobre los parámetros, opciones del trabajo
SCENARIOS = [
    ("multidepth_open_line", [LINE], dict(cutz=-1.8, multidepth=True, depthperpass=0.6), {}),
    ("multidepth_boundary_float", [LINE], dict(cutz=-1.0, multidepth=True, depthperpass=0.1), {}),
    ("multidepth_ring_and_hole", [SQUARE_HOLE], dict(cutz=-1.0, multidepth=True, depthperpass=0.4), {}),
    ("multidepth_closed_linestring", [CLOSED_LINESTRING], dict(cutz=-1.5, multidepth=True, depthperpass=0.5), {}),
    ("multidepth_points", ["POINT (3 4)", "POINT (5 6)"], dict(cutz=-2.0, multidepth=True, depthperpass=0.5), {}),
    ("single_points", ["POINT (3 4)", "POINT (5 6)"], dict(cutz=-1.2), {}),
    ("single_two_lines", [LINE, "LINESTRING (20 0, 25 5)"], dict(cutz=-0.5), {}),
    ("extracut_default_length", [SQUARE], dict(cutz=-1.0, extracut=True, extracut_length=0.5), {}),
    ("extracut_zero_length", [SQUARE], dict(cutz=-1.0, extracut=True, extracut_length=0.0), {}),
    ("extracut_longer_than_ring", [SQUARE], dict(cutz=-1.0, extracut=True, extracut_length=500.0), {}),
    ("extracut_with_multidepth", [SQUARE], dict(cutz=-1.0, extracut=True, extracut_length=1.0, multidepth=True,
                                                depthperpass=0.5), {}),
    ("extracut_ignored_for_open_line", [LINE], dict(cutz=-1.0, extracut=True, extracut_length=1.0), {}),
    ("tolerance_simplifies", [NOISY], dict(cutz=-0.5), dict(tolerance=0.05)),
    ("segmentation", [LONG], dict(cutz=-0.5), dict(segx=2.0, segy=1.5)),
    ("segmentation_x_only", [LONG], dict(cutz=-0.5), dict(segx=2.5)),
    ("no_toolchange_with_startz", [LINE], dict(cutz=-0.5, startz=1.5, dwell=True, dwelltime=2.0), dict(toolchange=False)),
    ("laser_single_pass", [LINE], dict(cutz=-0.5, feedrate=500.0, spindlespeed=300, ppname_g="GRBL_laser"), {}),
    ("laser_no_toolchange", [LINE], dict(cutz=-0.5, feedrate=500.0, spindlespeed=300, ppname_g="GRBL_laser"),
     dict(toolchange=False)),
    ("inches", [LINE], dict(cutz=-0.07, travelz=0.08, depthperpass=0.02, multidepth=True), dict(units="IN")),
]


class ExcAreas:
    exclusion_areas_storage = []

    @staticmethod
    def travel_coordinates(start_point, end_point, tooldia):
        return [(None, end_point)]


def num(v):
    return repr(float(v))


def cs(text):
    return '"' + text.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n") + '"'


def pair(v):
    return "std::array<double, 2>{%s, %s}" % (num(v[0]), num(v[1])) if v else None


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    import appPreProcessor  # noqa: E402
    default_mod = load("pp_default", os.path.join(REF, "preprocessors", "default.py"))
    load("pp_laser", os.path.join(REF, "preprocessors", "GRBL_laser.py"))
    import math
    default_mod.math = math

    app = FakeApp()
    app.defaults.update({"cncjob_steps_per_circle": 64, "cncjob_coords_type": "G90", "geometry_f_plunge": False})
    app.preprocessors = appPreProcessor.preprocessors
    app.exc_areas = ExcAreas()
    app.dec_format = lambda v, dec=4: float("%.*f" % (dec, v))
    camlib.Geometry.app = app
    camlib.CNCjob.app = app

    proj = json.load(open(os.path.join(DATA, "dosificador_PCB_principal.params.json")))
    geo_obj = [o for o in proj["objs"] if o.get("options", {}).get("name") == "PCB ppal-Edge_Cuts.gm1_cutout"][0]
    base_tool = geo_obj["tools"]["1"]

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_geometry_modes.py - no editar a mano.",
           "// Referencia: CNCjob.geometry_tool_gcode_gen original (FlatCAM 2024.4) con varias geometrías y opciones.",
           "#pragma once", "#include <array>", "#include <string>", "#include <vector>", '#include "cam/CncJob.h"',
           "namespace golden_geometry_modes {"]
    names = []
    for idx, (name, wkts, over, jobopts) in enumerate(SCENARIOS):
        shapes = [wkt.loads(w) for w in wkts]
        data = copy.deepcopy(base_tool["data"])
        data.update(over)
        data["optimization_type"] = "T"
        units = jobopts.get("units", "MM")
        toolchange = jobopts.get("toolchange", True)
        pp_name = data["ppname_g"]

        job = camlib.CNCjob(units=units, kind="geometry", steps_per_circle=64, pp_geometry_name=pp_name)
        job.machinist_setting = 0
        job.coords_decimals = 4
        job.fr_decimals = 2
        job.oldx = 0.0
        job.oldy = 0.0
        job.segx = float(jobopts.get("segx", 0.0))
        job.segy = float(jobopts.get("segy", 0.0))
        xmin, ymin, xmax, ymax = unary_union(shapes).bounds
        job.options = {"name": name, "type": "Geometry", "tool_dia": base_tool["tooldia"], "xmin": xmin, "ymin": ymin,
                       "xmax": xmax, "ymax": ymax}
        job.optimized_travelling_salesman = lambda points, start=None: list(points)
        tools = {1: {"tooldia": base_tool["tooldia"], "offset": "Path", "offset_value": 0.0, "solid_geometry": shapes,
                     "data": data}}
        gcode, start = job.geometry_tool_gcode_gen(1, tools, first_pt=(0.0, 0.0),
                                                   tolerance=jobopts.get("tolerance", 0), is_first=True,
                                                   is_last=True, toolchange=toolchange)
        expected = start + "\n" + gcode
        print("%-34s %4d líneas" % (name, expected.count("\n")), flush=True)

        out.append("inline flatcam::ShapeList geometry%d() {  // %s" % (idx, name))
        out.append("    return {%s};" % ", ".join("flatcam::Shape::fromWkt(%s)" % cs(w) for w in wkts))
        out.append("}")
        out.append("inline flatcam::GeometryJobSettings settings%d() {" % idx)
        out += ["    flatcam::GeometryJobSettings s;", '    s.units = "%s";' % units,
                '    s.preprocessor = "%s";' % pp_name, "    s.order = flatcam::DrillOrder::FileOrder;",
                "    s.tool = 1;", "    s.toolchange = %s;" % str(bool(toolchange)).lower(),
                "    flatcam::GeometryToolSettings& t = s.toolSettings;",
                "    t.toolDia = %s;" % num(base_tool["tooldia"])]
        for key, cpp in [("cutz", "cutZ"), ("depthperpass", "depthPerPass"), ("travelz", "travelZ"),
                         ("feedrate", "feedrate"), ("feedrate_z", "feedrateZ"), ("feedrate_rapid", "feedrateRapid"),
                         ("spindlespeed", "spindleSpeed"), ("dwelltime", "dwellTime"), ("endz", "endZ"),
                         ("toolchangez", "toolchangeZ"), ("extracut_length", "extracutLength")]:
            out.append("    t.%s = %s;" % (cpp, num(data[key])))
        out.append('    t.spindleDir = "%s";' % data["spindledir"])
        for key, cpp in [("multidepth", "multidepth"), ("dwell", "dwell"), ("extracut", "extracut")]:
            out.append("    t.%s = %s;" % (cpp, str(bool(data[key])).lower()))
        if data["startz"]:
            out.append("    t.startZ = %s;" % num(data["startz"]))
        if data["endxy"]:
            out.append("    t.endXY = %s;" % pair(data["endxy"]))
        if data["toolchangexy"]:
            out.append("    t.toolchangeXY = %s;" % pair(data["toolchangexy"]))
        out.append("    t.tolerance = %s;" % num(jobopts.get("tolerance", 0.0)))
        out.append("    t.segX = %s;" % num(job.segx))
        out.append("    t.segY = %s;" % num(job.segy))
        out += ["    return s;", "}"]
        names.append((name, expected))

    out.append("struct Scenario { const char* name; flatcam::ShapeList (*geometry)(); flatcam::GeometryJobSettings (*settings)(); const char* expected; };")
    out.append("inline const std::vector<Scenario> kScenarios = {")
    for idx, (name, expected) in enumerate(names):
        out.append('{"%s", geometry%d, settings%d, %s},' % (name, idx, idx, cs(expected)))
    out.append("};")
    out.append("}  // namespace golden_geometry_modes")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
