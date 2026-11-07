#!/usr/bin/env python3
"""Oráculo del G-code de una Geometría con VARIAS herramientas en un mismo CNC Job.

Repite lo que hace `GeometryObject.mtool_gen_cncjob` (rama multigeo) con el `CNCjob.geometry_tool_gcode_gen` ORIGINAL
(FlatCAM 2024.4): un mismo objeto CNCjob, una llamada por herramienta con `is_first` en la primera, `is_last` en la
última y `toolchange=True`, y el G-code total es la concatenación. Escribe `tests/golden_geometry_multitool.h`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_geometry_multitool.py
"""
import copy
import json
import os
import sys

from shapely import wkt
from shapely.ops import unary_union

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_exclusion import exc_areas  # noqa: E402
from oracle_geometry_modes import cs, num, pair  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_geometry_multitool.h")

SQUARE = "POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0))"
LINE = "LINESTRING (0 0, 10 0, 10 5)"
GAPS = "LINESTRING (12 0, 12 3)"

# nombre, [(uid, diámetro, [WKT], cambios en data)]
SCENARIOS = [
    ("cutout_with_thin_gaps", [(1, 1.2, [SQUARE], dict(cutz=-1.8)), (9999, 1.2, [GAPS], dict(cutz=-1.0))]),
    ("two_tools_different_params", [(1, 2.0, [LINE], dict(cutz=-0.5, feedrate=100.0, spindlespeed=8000)),
                                    (2, 0.8, [SQUARE], dict(cutz=-1.0, multidepth=True, depthperpass=0.4,
                                                            feedrate=50.0, dwell=True, dwelltime=2.0))]),
    ("three_tools_laser", [(1, 0.1, [LINE], dict(cutz=-0.5, feedrate=500.0, spindlespeed=300, ppname_g="GRBL_laser")),
                           (2, 0.2, [SQUARE], dict(cutz=-0.5, feedrate=400.0, spindlespeed=200, ppname_g="GRBL_laser")),
                           (3, 0.3, [GAPS], dict(cutz=-0.5, feedrate=300.0, spindlespeed=100, ppname_g="GRBL_laser"))]),
]


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
    app.exc_areas = exc_areas(app, [])
    camlib.Geometry.app = app
    camlib.CNCjob.app = app

    proj = json.load(open(os.path.join(DATA, "dosificador_PCB_principal.params.json")))
    geo_obj = [o for o in proj["objs"] if o.get("options", {}).get("name") == "PCB ppal-Edge_Cuts.gm1_cutout"][0]
    base = geo_obj["tools"]["1"]["data"]

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_geometry_multitool.py - no editar a mano.",
           "// Referencia: mtool_gen_cncjob (multigeo) con CNCjob.geometry_tool_gcode_gen original (FlatCAM 2024.4).",
           "#pragma once", "#include <array>", "#include <string>", "#include <vector>", '#include "cam/CncJob.h"',
           "namespace golden_geometry_multitool {"]
    names = []
    for idx, (name, tool_specs) in enumerate(SCENARIOS):
        tools = {}
        all_shapes = []
        for uid, dia, wkts, over in tool_specs:
            data = copy.deepcopy(base)
            data.update(over)
            data["optimization_type"] = "T"
            shapes = [wkt.loads(w) for w in wkts]
            all_shapes += shapes
            tools[uid] = {"tooldia": dia, "offset": "Path", "offset_value": 0.0, "solid_geometry": shapes, "data": data}
        pp_name = tool_specs[0][3].get("ppname_g", base["ppname_g"])
        job = camlib.CNCjob(units="MM", kind="geometry", steps_per_circle=64, pp_geometry_name=pp_name)
        job.machinist_setting = 0
        job.coords_decimals = 4
        job.fr_decimals = 2
        job.oldx = 0.0
        job.oldy = 0.0
        job.segx = 0.0
        job.segy = 0.0
        xmin, ymin, xmax, ymax = unary_union(all_shapes).bounds
        job.options = {"name": name, "type": "Geometry", "tool_dia": tool_specs[0][1], "xmin": xmin, "ymin": ymin,
                       "xmax": xmax, "ymax": ymax}
        job.optimized_travelling_salesman = lambda points, start=None: list(points)
        keys = list(tools.keys())
        total, gc_start = "", ""
        for uid in keys:
            res, start = job.geometry_tool_gcode_gen(uid, tools, first_pt=(0, 0), tolerance=0,
                                                     is_first=uid == keys[0], is_last=uid == keys[-1], toolchange=True)
            total += res
            if start != "":
                gc_start = start
        expected = gc_start + "\n" + total
        print("%-30s %4d líneas" % (name, expected.count("\n")), flush=True)

        out.append("inline std::vector<flatcam::GeometryToolJob> jobs%d() {  // %s" % (idx, name))
        out.append("    std::vector<flatcam::GeometryToolJob> v;")
        for uid, dia, wkts, over in tool_specs:
            data = tools[uid]["data"]
            out += ["    {", "        flatcam::GeometryToolJob j;",
                    "        j.geometry = {%s};" % ", ".join("flatcam::Shape::fromWkt(%s)" % cs(w) for w in wkts),
                    "        flatcam::GeometryJobSettings& s = j.settings;",
                    '        s.preprocessor = "%s";' % data["ppname_g"],
                    "        s.order = flatcam::DrillOrder::FileOrder;", "        s.tool = %d;" % uid,
                    "        s.toolchange = true;",
                    "        s.headerBounds = flatcam::BoundingBox{%s, %s, %s, %s};" % (num(xmin), num(ymin), num(xmax), num(ymax)),
                    "        flatcam::GeometryToolSettings& t = s.toolSettings;", "        t.toolDia = %s;" % num(dia)]
            for key, cpp in [("cutz", "cutZ"), ("depthperpass", "depthPerPass"), ("travelz", "travelZ"),
                             ("feedrate", "feedrate"), ("feedrate_z", "feedrateZ"), ("feedrate_rapid", "feedrateRapid"),
                             ("spindlespeed", "spindleSpeed"), ("dwelltime", "dwellTime"), ("endz", "endZ"),
                             ("toolchangez", "toolchangeZ"), ("extracut_length", "extracutLength")]:
                out.append("        t.%s = %s;" % (cpp, num(data[key])))
            out.append('        t.spindleDir = "%s";' % data["spindledir"])
            for key, cpp in [("multidepth", "multidepth"), ("dwell", "dwell"), ("extracut", "extracut")]:
                out.append("        t.%s = %s;" % (cpp, str(bool(data[key])).lower()))
            if data["startz"]:
                out.append("        t.startZ = %s;" % num(data["startz"]))
            if data["endxy"]:
                out.append("        t.endXY = %s;" % pair(data["endxy"]))
            if data["toolchangexy"]:
                out.append("        t.toolchangeXY = %s;" % pair(data["toolchangexy"]))
            out += ["        v.push_back(j);", "    }"]
        out += ["    return v;", "}"]
        names.append((name, expected, job.travel_distance, job.routing_time))

    out.append("struct Scenario { const char* name; std::vector<flatcam::GeometryToolJob> (*jobs)(); const char* expected;"
               " double travelDistance; double routingTime; };")
    out.append("inline const std::vector<Scenario> kScenarios = {")
    for idx, (name, expected, travel, routing) in enumerate(names):
        out.append('{"%s", jobs%d, %s, %r, %r},' % (name, idx, cs(expected), float(travel), float(routing)))
    out.append("};")
    out.append("}  // namespace golden_geometry_multitool")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
