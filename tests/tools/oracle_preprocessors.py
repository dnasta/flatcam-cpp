#!/usr/bin/env python3
"""Oráculo de los postprocesadores `default` y `GRBL_laser` y del formato de números de Python.

Ejecuta los postprocesadores ORIGINALES de FlatCAM con varios juegos de parámetros y guarda el texto que
generan cada uno de sus métodos en `tests/golden_preprocessors.h`. Los tests de C++ arman los mismos
parámetros y comparan cada línea de G-code.

Uso:  python3 tests/tools/oracle_preprocessors.py
"""
import math
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import REF, ROOT, install_stubs, load  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_preprocessors.h")
METHODS = ["start_code", "startz_code", "lift_code", "down_code", "toolchange_code", "up_to_zero_code",
           "rapid_code", "linear_code", "end_code", "feedrate_code", "z_feedrate_code", "spindle_code",
           "dwell_code", "spindle_stop_code"]
CPP_METHODS = {"start_code": "startCode", "startz_code": "startzCode", "lift_code": "liftCode",
               "down_code": "downCode", "toolchange_code": "toolchangeCode", "up_to_zero_code": "upToZeroCode",
               "rapid_code": "rapidCode", "linear_code": "linearCode", "end_code": "endCode",
               "feedrate_code": "feedrateCode", "z_feedrate_code": "zFeedrateCode",
               "spindle_code": "spindleCode", "dwell_code": "dwellCode", "spindle_stop_code": "spindleStopCode"}


class AttrDict(dict):
    """Igual que `camlib.AttrDict`: un diccionario con acceso por atributo."""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.__dict__ = self


def tool(n, dia, fz, fr, cutz, off, dpp, tz):
    return {"tooldia": dia, "data": {"tools_drill_feedrate_z": fz, "tools_drill_feedrate_rapid": fr,
                                     "tools_drill_cutz": cutz, "tools_drill_offset": off,
                                     "tools_drill_depthperpass": dpp, "tools_drill_travelz": tz}}


def base(**over):
    p = dict(units="MM", coords_decimals=4, decimals=4, fr_decimals=2, feedrate=100.0, z_feedrate=100.0,
             feedrate_rapid=1500.0, z_cut=-4.5, multidepth=False, z_depthpercut=0.6, z_move=2.0, use_ui=False,
             toolchange=False, z_toolchange=19.0, xy_toolchange=None, startz=None, z_end=19.0,
             xy_end=[0.0, 0.0], steps_per_circle=64, pp_excellon_name="default", pp_geometry_name="default",
             spindlespeed=1000.0, spindledir="CW", dwelltime=1000, tool=1, toolC=1.2, f_plunge=False,
             x=1.5, y=-2.25, exc_tools={}, exc_cnc_tools={},
             options=dict(xmin=-37.65, xmax=37.65, ymin=-39.15, ymax=39.15, type="Geometry", tool_dia=1.2,
                          Tools_in_use=[]))
    p.update(over)
    return p


EXC_TOOLS = {1: tool(1, 0.8, 100.0, 1500, -3.5, 0.0, 0.7, 2), 2: tool(2, 1.0, 100.0, 1500, -3.5, 0.0, 0.7, 2),
             3: tool(3, 1.5, 100.0, 1500, -3.5, 0.0, 0.7, 2), 4: tool(4, 3.4, 100.0, 1500, -4.5, 0.0, 0.7, 2)}
EXC_CNC = {0.8: {"tool": 1, "data": {"tools_drill_offset": 0.0}}, 1.0: {"tool": 2, "data": {"tools_drill_offset": 0.0}},
           1.5: {"tool": 3, "data": {"tools_drill_offset": 0.5}}, 3.4: {"tool": 4, "data": {"tools_drill_offset": 0.0}}}
IN_USE = [[1, 0.8, 90, 0], [2, 1.0, 96, 0], [3, 1.5, 2, 0], [4, 3.4, 4, 0]]
EXC_OPTS = dict(xmin=-33.962, xmax=32.24, ymin=-33.554, ymax=32.286, type="Excellon", tool_dia=0.8, Tools_in_use=IN_USE)

SCENARIOS = [
    ("cutout_mm", base()),
    ("cutout_inches_multidepth", base(units="IN", multidepth=True, z_depthpercut=0.025, z_cut=-0.07, z_move=0.08,
                                      feedrate=3.9, z_feedrate=2.36, feedrate_rapid=59, z_toolchange=0.75,
                                      z_end=0.75, xy_end=[0, 0], toolC=0.047, spindlespeed=1000)),
    ("drill_ui_t1", base(use_ui=True, toolchange=True, xy_toolchange=[0.0, 0.0], tool=1, toolC=0.8, z_cut=-3.5,
                         spindlespeed=1000, exc_tools=EXC_TOOLS, exc_cnc_tools=EXC_CNC, options=EXC_OPTS)),
    ("drill_ui_t4", base(use_ui=True, toolchange=True, xy_toolchange=[0.0, 0.0], tool=4, toolC=3.4, z_cut=-4.5,
                         spindlespeed=1000, exc_tools=EXC_TOOLS, exc_cnc_tools=EXC_CNC, options=EXC_OPTS)),
    ("drill_multidepth_startz", base(use_ui=True, toolchange=True, xy_toolchange=None, startz=1.5, tool=1, toolC=0.8,
                                     multidepth=True, f_plunge=True, z_depthpercut=0.7, z_cut=-3.5,
                                     exc_tools=EXC_TOOLS, exc_cnc_tools=EXC_CNC, options=EXC_OPTS)),
    ("drill_noui", base(use_ui=False, toolchange=False, tool=2, toolC=1.0, spindlespeed=1000,
                        options=dict(EXC_OPTS))),
    ("laser_geometry", base(feedrate=500.0, spindlespeed=300.0, pp_geometry_name="GRBL_laser", z_move=2.0,
                            toolC=0.1, options=dict(xmin=-35.4124, xmax=35.652, ymin=-36.7244, ymax=36.906,
                                                    type="Geometry", tool_dia=0.1, Tools_in_use=[]))),
    ("laser_ccw_nopower", base(feedrate=500.0, spindlespeed=None, spindledir="CCW", xy_end=None, dwelltime=0)),
]

FLOATS = [0.0, 1.0, 100.0, 0.8, 1500.0, 1e-05, 0.0001, 0.00012345, 123456789012345.0, 1e15, 1e16, 1.5e16,
          12345678901234567890.0, -3.5, 0.1 + 0.2, 2.54, 1 / 25.4, 19.0, 0.07, 25.4, 3e-10, 123.456, -0.0, 5e-324,
          1.7976931348623157e308, 0.5, 2.5e-05, 1234567.125]


def cs(text):
    return '"' + text.replace("\\", "\\\\").replace('"', '\\"').replace("\n", "\\n") + '"'


def num(v):
    if v is None:
        return None
    if isinstance(v, bool):
        return "PyNumber(%d)" % int(v)
    if isinstance(v, int):
        return "PyNumber(%d)" % v
    return "PyNumber(%s)" % repr(float(v))


def opt_num(v, name):
    return "    p.%s = %s;" % (name, num(v)) if v is not None else "    p.%s.reset();" % name


def pair(name, v):
    if v is None:
        return "    p.%s.reset();" % name
    return "    p.%s = std::array<PyNumber, 2>{%s, %s};" % (name, num(v[0]), num(v[1]))


def emit_builder(idx, name, d):
    o = d["options"]
    lines = ["inline flatcam::PreprocessorParams scenario%d() {  // %s" % (idx, name),
             "    using flatcam::PyNumber; using flatcam::ToolInUse; using flatcam::ExcToolInfo;",
             "    flatcam::PreprocessorParams p;"]
    lines += ['    p.units = "%s";' % d["units"], "    p.coordsDecimals = %d;" % d["coords_decimals"],
              "    p.decimals = %d;" % d["decimals"], "    p.frDecimals = %d;" % d["fr_decimals"]]
    for key, cpp in [("feedrate", "feedrate"), ("z_feedrate", "zFeedrate"), ("feedrate_rapid", "feedrateRapid"),
                     ("z_cut", "zCut"), ("z_depthpercut", "zDepthPerCut"), ("z_move", "zMove"),
                     ("z_toolchange", "zToolchange"), ("z_end", "zEnd"), ("dwelltime", "dwellTime"), ("toolC", "toolC")]:
        lines.append("    p.%s = %s;" % (cpp, num(d[key])))
    lines += ["    p.multidepth = %s;" % str(d["multidepth"]).lower(), "    p.useUi = %s;" % str(d["use_ui"]).lower(),
              "    p.toolchange = %s;" % str(d["toolchange"]).lower(),
              "    p.fPlunge = %s;" % str(d["f_plunge"]).lower(), "    p.stepsPerCircle = %d;" % d["steps_per_circle"],
              '    p.ppExcellonName = "%s";' % d["pp_excellon_name"], '    p.ppGeometryName = "%s";' % d["pp_geometry_name"],
              '    p.spindleDir = "%s";' % d["spindledir"], "    p.tool = %d;" % d["tool"],
              "    p.x = %s;" % repr(float(d["x"])), "    p.y = %s;" % repr(float(d["y"]))]
    lines.append(opt_num(d["startz"], "startZ"))
    lines.append(opt_num(d["spindlespeed"], "spindleSpeed"))
    lines.append(pair("xyToolchange", d["xy_toolchange"]))
    lines.append(pair("xyEnd", d["xy_end"]))
    for k in ("xmin", "ymin", "xmax", "ymax"):
        lines.append("    p.options.%s = %s;" % (k, num(o[k])))
    lines += ['    p.options.type = "%s";' % o["type"], "    p.options.toolDia = %s;" % num(o["tool_dia"])]
    for t in o["Tools_in_use"]:
        lines.append("    p.options.toolsInUse.push_back(ToolInUse{%d, %s, %d, %d});" % (t[0], num(t[1]), t[2], t[3]))
    for n, t in d["exc_tools"].items():
        dd = t["data"]
        lines.append("    p.excTools.push_back(ExcToolInfo{%d, %s, %s, %s, %s, %s, %s, %s});" % (
            n, num(t["tooldia"]), num(dd["tools_drill_feedrate_z"]), num(dd["tools_drill_feedrate_rapid"]),
            num(dd["tools_drill_cutz"]), num(dd["tools_drill_offset"]), num(dd["tools_drill_depthperpass"]),
            num(dd["tools_drill_travelz"])))
    for k, t in d["exc_cnc_tools"].items():
        lines.append("    p.excCncTools.push_back(ExcToolInfo{%d, PyNumber(), PyNumber(), PyNumber(), PyNumber(), %s, PyNumber(), PyNumber()});" % (
            t["tool"], num(t["data"]["tools_drill_offset"])))
    lines += ["    return p;", "}"]
    return "\n".join(lines)


def main():
    install_stubs()
    sys.path.insert(0, REF)
    import appPreProcessor  # noqa: E402  (registra los postprocesadores al importarlos)
    default_mod = load("pp_default", os.path.join(REF, "preprocessors", "default.py"))
    laser_mod = load("pp_laser", os.path.join(REF, "preprocessors", "GRBL_laser.py"))
    default_mod.math = math
    pps = {"default": appPreProcessor.preprocessors["default"], "GRBL_laser": appPreProcessor.preprocessors["GRBL_laser"]}

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_preprocessors.py - no editar a mano.",
           "// Referencia: postprocesadores originales de FlatCAM (default y GRBL_laser).",
           "#pragma once", "#include <array>", "#include <vector>", '#include "cam/Preprocessor.h"',
           "namespace golden_pp {"]
    for i, (name, d) in enumerate(SCENARIOS):
        out.append(emit_builder(i, name, d))
    out.append("inline flatcam::PreprocessorParams scenario(int i) {")
    out.append("    switch (i) {")
    for i in range(len(SCENARIOS)):
        out.append("        case %d: return scenario%d();" % (i, i))
    out.append("    }\n    return {};\n}")
    out.append('inline const char* const kScenarioNames[] = {%s};' % ", ".join(cs(n) for n, _ in SCENARIOS))
    out.append("struct Case { const char* pp; int scenario; const char* method; const char* expected; };")
    out.append("inline const std::vector<Case> kCases = {")
    count = 0
    for si, (sname, d) in enumerate(SCENARIOS):
        for ppname, pp in pps.items():
            for m in METHODS:
                p = AttrDict(d)
                p["options"] = AttrDict(d["options"])
                try:
                    r = getattr(pp, m)(p)
                except Exception as e:  # noqa: BLE001
                    print("  (omitido: %s / %s / %s: %s)" % (sname, ppname, m, e))
                    continue
                out.append('{"%s", %d, "%s", %s},' % (ppname, si, CPP_METHODS[m], cs(r if r is not None else "")))
                count += 1
    out.append("};")
    out.append("struct FloatCase { double value; const char* repr; };")
    out.append("inline const std::vector<FloatCase> kFloats = {")
    for v in FLOATS:
        out.append("{%s, %s}," % (repr(v) if repr(v) not in ("inf", "nan") else "0.0", cs(repr(v))))
    out.append("};")
    out.append("}  // namespace golden_pp")
    with open(OUT, "w") as f:
        f.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%d casos, %.0f KB)" % (count, os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
