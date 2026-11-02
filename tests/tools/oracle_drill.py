#!/usr/bin/env python3
"""Oráculo del G-code de taladrado: ejecuta `CNCjob.excellon_tool_gcode_gen` ORIGINAL (FlatCAM 2024.4).

Lee el Excellon de `tests/data` con el parser original, lo refleja en X como en el proyecto del usuario, toma los
parámetros `tools_drill_*` de `tests/data/dosificador_PCB_principal.params.json` y genera el G-code del trabajo
con el orden de agujeros del archivo (sin optimizar). Guarda en `tests/golden_drill.h` el texto de cada
herramienta, el código de inicio y el archivo completo, y la función que arma los mismos parámetros en C++.

Requisitos (solo para regenerar el archivo): python3 -m pip install shapely numpy
Uso:  python3 tests/tools/oracle_drill.py
"""
import copy
import json
import os
import sys
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_drill.h")
HEADER = dict(version="8.994", date="2020/11/7", name="PCB ppal-Drill.drl_cnc", type="Geometry", units="MM",
              created="Monday, 11 November 2024 at 15:40")


class ExcAreas:
    """Sin áreas de exclusión el viaje a cada agujero es un solo movimiento rápido."""
    exclusion_areas_storage = []

    @staticmethod
    def travel_coordinates(start_point, end_point, tooldia):
        return [(None, end_point)]


def num(v):
    if v is None:
        return None
    if isinstance(v, bool):
        return "PyNumber(%d)" % int(v)
    if isinstance(v, int):
        return "PyNumber(%d)" % v
    return "PyNumber(%s)" % repr(float(v))


def cs(text):
    return '"' + text.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n") + '"'


def pair_cpp(v):
    return "std::array<double, 2>{%s, %s}" % (repr(float(v[0])), repr(float(v[1]))) if v else None


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pexc = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    import appPreProcessor  # noqa: E402
    import math
    for f in sorted(os.listdir(os.path.join(REF, "preprocessors"))):
        if f.endswith(".py") and f != "__init__.py":
            load("pp_" + f[:-3], os.path.join(REF, "preprocessors", f)).math = math

    app = FakeApp()
    app.defaults.update({"cncjob_steps_per_circle": 64, "cncjob_coords_type": "G90", "excellon_search_time": 3})
    app.preprocessors = appPreProcessor.preprocessors
    app.exc_areas = ExcAreas()
    app.dec_format = lambda v, dec=4: float("%.*f" % (dec, v))
    import logging
    app.log = logging.getLogger("oracle")  # doformat registra el error y devuelve ""
    app.log.addHandler(logging.NullHandler())
    app.log.propagate = False
    # line_xyz lee `end_xy`, que CNCjob no define: en el original la cabecera queda vacía. El port la escribe con
    # `xy_end` (desvío consciente), y así se compara.
    lxyz = appPreProcessor.preprocessors["line_xyz"]
    orig_start = type(lxyz).start_code

    def start_with_end_xy(self, p):
        p["end_xy"] = p["xy_end"]
        return orig_start(self, p)
    type(lxyz).start_code = start_with_end_xy
    camlib.Geometry.app = app
    camlib.CNCjob.app = app
    pexc.Excellon.app = app

    # ---- Excellon original, reflejado en X (x -> -x) como en el proyecto
    d = app.defaults
    ex = pexc.Excellon(zeros=d["excellon_zeros"], excellon_format_upper_mm=3, excellon_format_lower_mm=3,
                       excellon_format_upper_in=2, excellon_format_lower_in=4, excellon_units="METRIC",
                       geo_steps_per_circle=64)
    ex.default_data = {}
    ex.parse_file(os.path.join(DATA, "PCB principal-Drill.drl"))
    ex.create_geometry()
    ex.mirror("Y", (0, 0))

    # ---- parámetros del proyecto (FlatCAM 8.994) del Excellon del taladro
    proj = json.load(open(os.path.join(DATA, "dosificador_PCB_principal.params.json")))
    # Los parámetros por herramienta salen del CNC Job guardado (exc_cnc_tools, indexado por diámetro): allí la
    # herramienta 4 corta a -4,5; el Excellon guarda el mismo diccionario para todas las herramientas.
    job_obj = [o for o in proj["objs"] if o.get("options", {}).get("name") == "DosCau_PPal-Drill.nc"][0]
    by_tool = {c["tool"]: c["data"] for c in job_obj["exc_cnc_tools"].values()}
    tools = {}
    for tid in sorted(ex.tools):
        t = ex.tools[tid]
        tools[tid] = {"tooldia": t["tooldia"], "drills": t["drills"], "slots": [], "data": dict(by_tool[tid])}
        # La sonda (Toolchange_Probe_MACH3) no está en el proyecto de la 8.994: valores de fábrica de la 2024.4.
        tools[tid]["data"].setdefault("tools_drill_z_pdepth", -0.02)
        tools[tid]["data"].setdefault("tools_drill_feedrate_probe", 75)

    def run(pp_name):
        job = camlib.CNCjob(units="MM", kind="excellon", steps_per_circle=64, pp_excellon_name=pp_name,
                            pp_geometry_name="default")
        job.machinist_setting = 0
        job.coords_decimals = 4
        job.fr_decimals = 2
        job.use_ui = True
        job.multitool = True
        xmin, ymin, xmax, ymax = ex.bounds()
        job.options = {"name": HEADER["name"], "type": "Excellon", "xmin": xmin, "ymin": ymin, "xmax": xmax,
                       "ymax": ymax,
                       "Tools_in_use": [["Tool_nr", "Diameter", "Drills_Nr", "Slots_Nr"]] + [
                           [str(t), str(tools[t]["tooldia"]), str(len(tools[t]["drills"])), "0"] for t in tools]}
        job.exc_cnc_tools = {tools[t]["tooldia"]: {"tool": t, "data": {"tools_drill_offset": tools[t]["data"]["tools_drill_offset"]}}
                             for t in tools}
        # El camino "sin optimizar" del original está roto (usa `locations[point]` con tuplas), así que se usa la rama
        # 'T' con un optimizador que devuelve los puntos tal cual: queda el orden del archivo con el resto del código.
        job.optimized_travelling_salesman = lambda points, start=None: list(points)
        # Como ToolDrilling: oldx/oldy arrancan en el X,Y de cambio de herramienta (o 0, 0).
        tcxy = tools[sel[0]]["data"]["tools_drill_toolchangexy"]
        job.oldx, job.oldy = (tcxy[0], tcxy[1]) if tcxy else (0.0, 0.0)
        first_pt = (0.0, 0.0)
        texts, start_gcode = [], ""
        for i, t in enumerate(sel):
            gcode, last_pt, start = job.excellon_tool_gcode_gen(t, tools[t]["drills"], tools, first_pt=first_pt,
                                                                is_first=(i == 0), is_last=(i == len(sel) - 1),
                                                                opt_type="T", toolchange=True)
            texts.append(gcode)
            first_pt = last_pt
            if start != "":
                start_gcode = start
        return start_gcode, texts

    sel = list(tools)
    start_gcode, texts = run("default")
    OTHER_PPS = ["Berta_CNC", "Default_no_M6", "grbl_11", "GRBL_11_no_M6", "GRBL_laser", "ISEL_CNC", "ISEL_ICP_CNC",
                 "line_xyz", "Marlin", "Marlin_laser_FAN_pin", "Marlin_laser_Spindle_pin", "Repetier",
                 "Roland_MDX_20", "Toolchange_Custom", "Toolchange_Manual", "Toolchange_Probe_MACH3", "Z_laser"]
    other_runs = [(n,) + run(n) for n in OTHER_PPS]
    header = ("(G-CODE GENERATED BY FLATCAM v%(version)s - www.flatcam.org - Version Date: %(date)s)\n\n"
              "(Name: %(name)s)\n(Type: G-code from %(type)s)\n(Units: %(units)s)\n\n"
              "(Created on %(created)s)\n\n") % HEADER
    full = header + start_gcode + "\n" + "".join(texts) + "\n"

    # ---- salida C++
    out = ["// ARCHIVO GENERADO por tests/tools/oracle_drill.py - no editar a mano.",
           "// Referencia: CNCjob.excellon_tool_gcode_gen original (FlatCAM 2024.4), orden de agujeros del archivo.",
           "#pragma once", "#include <array>", "#include <string>", "#include <vector>", '#include "cam/CncJob.h"',
           "namespace golden_drill {", "inline flatcam::DrillJobSettings settings() {",
           "    using flatcam::PyNumber;", "    flatcam::DrillJobSettings s;", '    s.units = "MM";',
           "    s.order = flatcam::DrillOrder::FileOrder;"]
    for tid, t in tools.items():
        dd = t["data"]
        out.append("    {")
        out.append("        flatcam::DrillToolSettings t;")
        for key, cpp in [("tools_drill_cutz", "cutZ"), ("tools_drill_feedrate_z", "feedrateZ"),
                         ("tools_drill_feedrate_rapid", "feedrateRapid"), ("tools_drill_travelz", "travelZ"),
                         ("tools_drill_depthperpass", "depthPerPass"), ("tools_drill_offset", "offsetZ"),
                         ("tools_drill_dwelltime", "dwellTime"), ("tools_drill_toolchangez", "toolchangeZ"),
                         ("tools_drill_endz", "endZ")]:
            out.append("        t.%s = %s;" % (cpp, num(dd[key])))
        for key, cpp in [("tools_drill_multidepth", "multidepth"), ("tools_drill_dwell", "dwell"),
                         ("tools_drill_f_plunge", "fPlunge"), ("tools_drill_f_retract", "fRetract")]:
            out.append("        t.%s = %s;" % (cpp, str(bool(dd[key])).lower()))
        out.append('        t.spindleDir = "%s";' % dd["tools_drill_spindledir"])
        if dd["tools_drill_spindlespeed"] is not None:
            out.append("        t.spindleSpeed = %s;" % num(dd["tools_drill_spindlespeed"]))
        if dd["tools_drill_startz"] is not None:
            out.append("        t.startZ = %s;" % num(dd["tools_drill_startz"]))
        for key, cpp in [("tools_drill_toolchangexy", "toolchangeXY"), ("tools_drill_endxy", "endXY")]:
            if dd[key]:
                out.append("        t.%s = %s;" % (cpp, pair_cpp(dd[key])))
        out.append("        s.tools[%d] = t;" % tid)
        out.append("    }")
    out.append("    return s;\n}")
    out.append("inline flatcam::GcodeHeaderInfo header() {\n    flatcam::GcodeHeaderInfo h;")
    for k, cpp in [("version", "version"), ("date", "versionDate"), ("name", "name"), ("type", "type"),
                   ("units", "units"), ("created", "createdOn")]:
        out.append("    h.%s = %s;" % (cpp, cs(HEADER[k])))
    out.append("    return h;\n}")
    out.append("inline const std::string kStartGcode = %s;" % cs(start_gcode))
    out.append("inline const std::vector<std::string> kToolGcodes = {%s};" % ",\n".join(cs(t) for t in texts))
    out.append("inline const std::string kFullFile = %s;" % cs(full))
    out.append("struct PpRun { const char* pp; const char* start; std::vector<std::string> tools; };")
    out.append("inline const std::vector<PpRun> kOtherPreprocessors = {")
    for n, st, tx in other_runs:
        out.append('{"%s", %s, {%s}},' % (n, cs(st), ",\n".join(cs(t) for t in tx)))
    out.append("};")
    out.append("}  // namespace golden_drill")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB); herramientas=%s; líneas=%d" % (os.path.getsize(OUT) / 1024, sel, full.count("\n")))


if __name__ == "__main__":
    main()
