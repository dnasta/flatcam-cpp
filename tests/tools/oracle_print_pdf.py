#!/usr/bin/env python3
"""Oráculo de Archivo -> Imprimir (PDF): ejecuta `save_pdf` ORIGINAL de `app_Main.py` (tomado con `ast`) con objetos
Gerber y Excellon leídos por los parsers originales y escribe en `tests/golden_print_pdf.h` el SVG que le pasa a svglib,
la hoja y el desplazamiento del PDF.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_print_pdf.py
"""
import ast
import logging
import os
import sys
from xml.dom.minidom import parseString as parse_xml_string

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_print_pdf.h")
EDGE = "luz_de_noche/luz de noche Pascual-Edge_Cuts.gbr"
LOGO = "luz_de_noche/luz de noche Pascual-Logo.gbr"
PTH = "luz_de_noche/luz de noche Pascual-PTH.drl"


def main():
    from lxml import etree as real_et
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pg = load("ParseGerber", os.path.join(REF, "appParsers", "ParseGerber.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.defaults.update({"excellon_circle_steps": 64, "units": "MM", "global_workspaceT": "A4",
                         "global_workspace_orientation": "p", "global_draw_color": "#FF0000"})
    app.log = logging.getLogger("oracle")
    pg.Gerber.app = app
    pe.Excellon.app = app

    def gerber(f, color):
        g = pg.Gerber(steps_per_circle=64)
        g.parse_file(os.path.join(DATA, f))
        g.kind = "gerber"
        g.fill_color = color
        return g

    def excellon(f, color):
        e = pe.Excellon(geo_steps_per_circle=64)
        e.default_data = {}
        e.parse_file(os.path.join(DATA, f))
        e.create_geometry()
        e.kind = "excellon"
        e.fill_color = color
        return e

    src = os.path.join(REF, "app_Main.py")
    tree = ast.parse(open(src, encoding="utf-8").read())
    fn = None
    for node in ast.walk(tree):
        if isinstance(node, ast.FunctionDef) and node.name == "save_pdf":
            fn = node
    m = ast.Module(body=[ast.ClassDef(name="C", bases=[], keywords=[], body=[fn], decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    captured = {}

    class Canvas:
        def __init__(self, f, pagesize=None):
            captured["page"] = pagesize

        def translate(self, x, y):
            captured["translate"] = (x, y)

        def save(self):
            pass

    def svg2rlg(doc):
        captured["svg"] = doc.getvalue()

    R = type("R", (), {"drawToFile": staticmethod(lambda *a: None), "draw": staticmethod(lambda *a: None)})
    ns = dict(ET=real_et, parse_xml_string=parse_xml_string, StringIO=__import__("io").StringIO, Inf=float("inf"),
              mm=72.0 / 25.4, inch=72.0, svg2rlg=svg2rlg, renderPDF=R, canvas=type("c", (), {"Canvas": Canvas}),
              portrait=lambda s: (min(s), max(s)), landscape=lambda s: (max(s), min(s)), _=lambda s: s)
    exec(compile(m, src, "exec"), ns)
    C = ns["C"]
    edge = gerber(EDGE, "#BBF268BF")
    logo = gerber(LOGO, "#BBF26807")
    pth = excellon(PTH, "#C40000BF")
    cases = [("edge_pth", [edge, pth], "A4", "p"), ("pth", [pth], "A4", "l"), ("edge", [edge], "LETTER", "p")]
    lines = []
    for name, objs, page, orient in cases:
        c = C()
        c.app = app
        c.defaults = dict(app.defaults, global_workspaceT=page, global_workspace_orientation=orient)
        c.pagesize = {}
        c.inform = type("I", (), {"emit": staticmethod(lambda *a: None)})()
        captured.clear()
        c.save_pdf("/tmp/x.pdf", objs)
        tr = captured.get("translate", (0.0, 0.0))
        pg_ = captured.get("page", (0.0, 0.0))
        lines.append("  {%s, %s, %s, %s, {%r, %r}, {%r, %r}}," % (cs(name), cs(page), cs(orient),
                                                               __import__("json").dumps(captured["svg"]),
                                                               float(pg_[0]), float(pg_[1]), tr[0], tr[1]))
        print(name, len(captured["svg"]), file=sys.stderr)
    out = ["// GENERADO por tests/tools/oracle_print_pdf.py. No editar.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_print_pdf {",
           "struct Case { const char* name; const char* page; const char* orientation; std::string svg;"
           " std::vector<double> pageSize; std::vector<double> translate; };",
           "inline const std::vector<Case> kCases = {"] + lines + ["};", "}  // namespace golden_print_pdf", ""]
    open(OUT, "w").write("\n".join(out))
    print("escrito", OUT)


if __name__ == "__main__":
    main()
