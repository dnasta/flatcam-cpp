#!/usr/bin/env python3
"""Oráculo del Asistente de PCB: ejecuta `load_inf`, `load_excellon`, `on_file_loaded` y `update_params` ORIGINALES de
`appTools/ToolPcbWizard.py` (tomados con `ast`) sobre los archivos de `tests/data/pcbwizard/` y lee el Excellon que
resulta con el parser original. Escribe en `tests/golden_pcbwizard.h` el texto armado (con la fecha fija), las
herramientas del `.INF`, el formato y los agujeros de cada herramienta.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_pcbwizard.py
"""
import ast
import logging
import os
import re
import sys
from io import StringIO

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_pcbwizard.h")
TIME = "Thursday, 12 November 2026 at 10:00"
# (excellon, inf, ceros elegidos en el panel o None)
CASES = [("board.drl", "board.inf", None), ("metric.drl", "metric.inf", None), ("board.drl", "board.inf", "TZ"),
         ("metric.drl", "nounits.inf", None), ("board.drl", "notools.inf", None)]


class Val:
    def __init__(self, v=None):
        self.v = v

    def get_value(self):
        return self.v

    def set_value(self, v):
        self.v = v

    def setVisible(self, v):
        pass


class FixedDate:
    @staticmethod
    def now():
        class D:
            def __format__(self, spec):
                return TIME
        return D()


def main():
    install_stubs()
    sys.path.insert(0, REF)
    load("camlib", os.path.join(REF, "camlib.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    app = FakeApp()
    app.defaults.update({"excellon_circle_steps": 64})
    app.log = logging.getLogger("oracle")
    app.version = "2024.4"
    app.version_date = "2024/04/15"
    msgs = []
    app.inform = type("I", (), {"emit": staticmethod(lambda m: msgs.append(m))})()
    pe.Excellon.app = app
    path = os.path.join(REF, "appTools", "ToolPcbWizard.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "PcbWizard")
    names = {"load_inf", "load_excellon", "on_file_loaded", "update_params", "build_ui"}
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in names]
    meths = [f for f in meths if f.name != "build_ui"]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(re=re, os=os, StringIO=StringIO, datetime=FixedDate, _=lambda s: s)
    exec(compile(m, path, "exec"), ns)
    Core = ns["Core"]

    out = ["// GENERADO por tests/tools/oracle_pcbwizard.py. No editar.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_pcbwizard {",
           "struct Drill { int tool; double dia; std::vector<double> xy; };",
           "struct Case { const char* exc; const char* inf; const char* zeros; bool failed; std::string text;"
           " int integral; int fractional; const char* units; std::vector<double> infTools;"
           " std::vector<Drill> drills; };",
           "inline const char* kTime = %s;" % cs(TIME), "inline const std::vector<Case> kCases = {"]
    for exc, inf, zeros in CASES:
        c = Core()
        c.app = app
        c.units, c.zeros, c.integral, c.fractional = 'INCH', 'LZ', 2, 4
        c.outname = 'file'
        c.exc_file_content = None
        c.tools_from_inf = {}
        c.excellon_loaded = c.inf_loaded = c.process_finished = False
        c.modified_excellon_file = ''
        c.ui = type("U", (), {})()
        c.ui.int_entry, c.ui.frac_entry = Val(2), Val(4)
        c.ui.zeros_radio, c.ui.units_radio, c.ui.tools_table = Val('LZ'), Val('INCH'), Val()
        c.build_ui = lambda: None
        c.file_loaded = type("S", (), {"emit": staticmethod(lambda s, f: c.on_file_loaded(s, f))})()
        c.load_excellon(os.path.join(DATA, "pcbwizard", exc))
        if zeros:
            c.ui.zeros_radio.set_value(zeros)
        ret = c.load_inf(os.path.join(DATA, "pcbwizard", inf))
        failed = ret == "fail"
        text = c.modified_excellon_file.getvalue() if c.process_finished else ""
        drills = []
        if c.process_finished:
            e = pe.Excellon()
            e.parse_file(file_obj=c.modified_excellon_file)
            for t in sorted(e.tools):
                xy = []
                for d in e.tools[t].get('drills', []):
                    xy += [d.x, d.y]
                drills.append("{%d, %r, {%s}}" % (int(t), float(e.tools[t].get('tooldia', -1)), ", ".join(repr(v) for v in xy)))
        tools = []
        for k, v in c.tools_from_inf.items():
            tools += [k, v]
        out.append("  {%s, %s, %s, %s, %s, %d, %d, %s, {%s}, {%s}}," % (
            cs(exc), cs(inf), cs(zeros or ""), "true" if failed else "false", cs(text), c.integral, c.fractional,
            cs(c.units), ", ".join(repr(float(v)) for v in tools), ", ".join(drills)))
    out += ["};", "}  // namespace golden_pcbwizard", ""]
    open(OUT, "w").write("\n".join(out))
    print("escrito", OUT)


if __name__ == "__main__":
    main()
