#!/usr/bin/env python3
"""Oráculo del SVG de un CNC Job: ejecuta `CNCjob.gcode_parse`, `CNCjob.export_svg`, `CNCjob.bounds`, `size` y el
cuerpo de `App.export_svg` ORIGINALES sobre G-code abierto de archivo (como `open_gcode`: un trabajo de una sola
herramienta) y escribe `tests/golden_cnc_svg.h` con el SVG (o el error).

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_cnc_svg.py
"""
import logging
import os
import sys
from xml.dom.minidom import parseString as parse_xml_string

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_cnc_svg.h")
FILES = ["DosCau_PPal-Edge_Cuts.nc", "DosCau_PPal-Drill.nc", "DosCau_PPal-Alinea.nc",
         "luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc.nc"]


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    app = FakeApp()
    app.log = logging.getLogger("oracle")
    app.abort_flag = False
    app.defaults.update({"geometry_toolchangexy": "", "tools_drill_toolchangexy": ""})
    camlib.CNCjob.app = app
    lines = []
    for f in FILES:
        path = os.path.join(DATA, f)
        if not os.path.exists(path):
            print("falta", f, file=sys.stderr)
            continue
        c = object.__new__(camlib.CNCjob)
        c.app = app
        c.decimals = 4
        c.units = "MM"
        c.steps_per_circle = 64
        c.toolchange_xy_type = None
        c.pp_geometry_name = "default"
        c.pp_excellon_name = "default"
        c.pp_solderpaste_name = None
        c.origin_kind = None
        c.exc_tools = {}
        c.multitool = False
        c.options = {"tooldia": 1.0}  # cncjob_tooldia, copiado por new_object
        c.gcode = open(path).read()
        c.gcode_parsed = c.gcode_parse()
        err, svg = "", ""
        try:
            exported = c.export_svg(scale_stroke_factor=0.0)
            bounds = c.bounds()
            size = c.size()
            svgwidth, svgheight = str(size[0]), str(size[1])
            minx, miny = str(bounds[0]), str(bounds[1] - size[1])
            uom = c.units.lower()
            head = '<svg xmlns="http://www.w3.org/2000/svg" version="1.1" xmlns:xlink="http://www.w3.org/1999/xlink" '
            head += 'width="' + svgwidth + uom + '" ' + 'height="' + svgheight + uom + '" '
            head += 'viewBox="' + minx + ' ' + miny + ' ' + svgwidth + ' ' + svgheight + '">'
            head += '<g transform="scale(1,-1)">'
            svg = parse_xml_string(head + exported + '</g> </svg>').toprettyxml()
        except Exception as e:
            err = type(e).__name__
        h = 0xcbf29ce484222325
        for ch in svg.encode():
            h = ((h ^ ch) * 0x100000001b3) & 0xFFFFFFFFFFFFFFFF
        lines.append("  {%s, %s, %d, %duLL, %s, %s}," % (cs(f), cs(svg if len(svg) < 200000 else ""), len(svg), h,
                                                      cs(svg[:2000]), cs(err)))
        print(f, len(svg), err, file=sys.stderr)
    out = ["// GENERADO por tests/tools/oracle_cnc_svg.py. No editar.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_cnc_svg {",
           "struct Case { const char* file; std::string svg; std::size_t length; unsigned long long fnv; std::string head;"
           " const char* error; };", "inline const std::vector<Case> kCases = {"]
    out += lines
    out += ["};", "}  // namespace golden_cnc_svg", ""]
    open(OUT, "w").write("\n".join(out))
    print("escrito", OUT)


if __name__ == "__main__":
    main()
