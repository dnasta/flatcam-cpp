#!/usr/bin/env python3
"""Oráculo de Calibración: ejecuta `gcode_header`, `generate_verification_gcode`, `calculate_factors`,
`target_values_in_table`, `on_scale_button` y `on_skew_button` ORIGINALES de `appTools/ToolCalibration.py` (tomados
con `ast`) con una interfaz simulada y escribe en `tests/golden_calibration.h` lo que queda en los campos y el G-code.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_calibration.py
"""
import ast
import logging
import math
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import REF, ROOT  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_calibration.h")
TIME = "Thursday, 12 November 2026 at 10:00"


class Val:
    def __init__(self, v=None):
        self.v = v

    def get_value(self):
        return self.v

    def set_value(self, v):
        self.v = v


class Ev:
    """EvalEntry: guarda el texto y lo evalúa al leerlo."""

    def __init__(self):
        self.t = ""

    def set_value(self, v):
        self.t = str(v)

    def get_value(self):
        raw = self.t.strip(" ")
        try:
            return eval(raw)
        except Exception:
            return None


class FixedDate:
    @staticmethod
    def now():
        class D:
            def __format__(self, spec):
                return TIME
        return D()


class Editor:
    last = None

    def __init__(self, app=None, plain_text=True):
        Editor.last = self
        self.code_editor = Val()
        self.buttonRun = self.t_frame = Hid()
        self.buttonSave = Hid()

    def setObjectName(self, n):
        pass

    def load_text(self, t, move_to_start=True, clear_text=True):
        self.text = t


class Hid:
    def __getattr__(self, n):
        return Hid()

    def __call__(self, *a, **k):
        return None


def main():
    from shapely.affinity import scale, skew
    from shapely.geometry import Point
    path = os.path.join(REF, "appTools", "ToolCalibration.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    cls = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "ToolCalibration")
    names = {"gcode_header", "generate_verification_gcode", "calculate_factors", "target_values_in_table",
             "on_scale_button", "on_skew_button", "check_points"}
    meths = [f for f in cls.body if isinstance(f, ast.FunctionDef) and f.name in names]
    m = ast.Module(body=[ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[])],
                   type_ignores=[])
    ast.fix_missing_locations(m)
    ns = dict(math=math, Point=Point, scale=scale, skew=skew, datetime=FixedDate, AppTextEditor=Editor,
              _=lambda s: s, log=logging.getLogger("x"))
    exec(compile(m, path, "exec"), ns)
    Core = ns["Core"]

    class App:
        version = "2024.4"
        version_date = "2024/04/15"
        inform = Hid()
        ui = Hid()
        proc_container = Hid()

    def new(units="MM", decimals=4):
        c = Core()
        c.app = App()
        c.app.ui.plot_tab_area = Hid()
        c.decimals = decimals
        c.units = units
        c.disconnect_cal_events = lambda: None
        u = type("U", (), {})()
        for k in ("bottom_left", "bottom_right", "top_left", "top_right"):
            for a in ("x", "y"):
                setattr(u, "%s_coord%s_tgt" % (k, a), Ev())
                setattr(u, "%s_coord%s_found" % (k, a), Ev())
        u.scalex_entry, u.scaley_entry = Val(1.0), Val(1.0)
        u.skewx_entry, u.skewy_entry = Val(0.0), Val(0.0)
        u.travelz_entry, u.toolchangez_entry, u.verz_entry = Val(2.0), Val(15.0), Val(0.1)
        u.toolchange_xy_entry, u.zeroz_cb, u.second_point_radio = Val(""), Val(False), Val("tl")
        c.ui = u
        c.click_points = []
        return c

    def click(c, pts):
        for p in pts:
            c.click_points.append([float('%.*f' % (c.decimals, p[0])), float('%.*f' % (c.decimals, p[1]))])
            c.check_points()

    def gcode(c):
        Editor.last = None
        r = c.generate_verification_gcode()
        return "" if r == "fail" else Editor.last.text

    def state(c):
        u = c.ui
        tgt = []
        for k in ("bottom_left", "bottom_right", "top_left", "top_right"):
            for a in ("x", "y"):
                tgt.append(getattr(u, "%s_coord%s_tgt" % (k, a)).t)
        pts = [v for p in c.click_points for v in p]
        return tgt, pts, [u.scalex_entry.v, u.scaley_entry.v, u.skewx_entry.v, u.skewy_entry.v]

    P = [(-30.123456, -25.000049), (31.5, -25.2), (-29.8, 26.75), (31.2, 27.0)]
    cases = []

    def record(name, c, gc="", failed=False):
        tgt, pts, f = state(c)
        cases.append("  {%s, {%s}, {%s}, {%s}, %s, %s}," % (
            cs(name), ", ".join(cs(t) for t in tgt), ", ".join(repr(float(v)) for v in pts),
            ", ".join(repr(float(v)) for v in f), cs(gc), "true" if failed else "false"))

    # 1: puntos con clic, sin Z cero, "tl".
    c = new()
    click(c, P)
    record("gcode_tl", c, gcode(c))
    # 2: Z cero, X,Y de cambio, "br", pulgadas, 3 puntos (falla) y después 4.
    c = new("IN", 4)
    click(c, P[:3])
    g3 = gcode(c)
    assert g3 == ""
    click(c, P[3:])
    c.ui.zeroz_cb.v, c.ui.toolchange_xy_entry.v, c.ui.second_point_radio.v = True, "1.5, 2", "br"
    c.ui.travelz_entry.v, c.ui.toolchangez_entry.v, c.ui.verz_entry.v = 1.23456, 20, -0.05
    record("gcode_br_zeroz", c, gcode(c))
    # 3: factores con todo medido.
    c = new()
    click(c, P)
    c.ui.top_left_coordx_found.set_value(0.1)
    c.ui.top_left_coordy_found.set_value(0.2)
    c.ui.bottom_right_coordx_found.set_value(0.3)
    c.ui.bottom_right_coordy_found.set_value(-0.1)
    c.calculate_factors()
    record("factors_all", c)
    # 4: nada medido (los campos vacíos).
    c = new()
    click(c, P)
    c.ui.top_left_coordx_found.set_value('')
    c.calculate_factors()
    record("factors_none", c)
    # 5: escalar, inclinar y el G-code con los puntos sin redondear; después los factores sin medir.
    c = new()
    click(c, P)
    c.ui.scalex_entry.v, c.ui.scaley_entry.v = 1.0123, 0.9876
    c.on_scale_button()
    record("scaled", c)
    c.ui.skewx_entry.v, c.ui.skewy_entry.v = 0.5, -0.25
    c.on_skew_button()
    record("skewed", c, gcode(c))
    c.calculate_factors()
    record("factors_after_skew", c)
    # 6: división por cero (abajo a la derecha con la X del origen).
    c = new()
    click(c, [P[0], (P[0][0], -25.2), P[2], P[3]])
    c.ui.bottom_right_coordx_found.set_value(0.3)
    try:
        c.calculate_factors()
        failed = False
    except ZeroDivisionError:
        failed = True
    record("factors_zero", c, "", failed)
    # 7: tres decimales.
    c = new("MM", 2)
    click(c, P)
    c.ui.top_left_coordy_found.set_value("0.05*2")
    c.calculate_factors()
    record("factors_2dec", c, gcode(c))

    out = ["// GENERADO por tests/tools/oracle_calibration.py. No editar.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_calibration {",
           "struct Case { const char* name; std::vector<std::string> tgt; std::vector<double> points;"
           " std::vector<double> fields; std::string gcode; bool failed; };",
           "inline const char* kTime = %s;" % cs(TIME), "inline const std::vector<Case> kCases = {"]
    out += cases
    out += ["};", "}  // namespace golden_calibration", ""]
    open(OUT, "w").write("\n".join(out))
    print("escrito", OUT)


if __name__ == "__main__":
    main()
