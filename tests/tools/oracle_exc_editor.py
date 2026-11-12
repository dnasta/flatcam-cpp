#!/usr/bin/env python3
"""Oráculo del editor de Excellon: toma con `ast` de `appEditors/AppExcEditor.py` `edit_fcexcellon`,
`update_fcexcellon`, `add_exc_shape`, `on_tool_delete`, `on_exc_shape_complete`, la elección con un clic
(`SelectEditorExc.click_release`), las figuras de agregar agujeros, ranuras y sus grillas (`DrillAdd`, `DrillArray`,
`SlotAdd`, `SlotArray`) y el cambio de diámetro (`ResizeEditorExc.make`), y los ejecuta sobre el taladro de "luz de
noche" y un Excellon con ranuras. Escribe `tests/golden_exc_editor.h`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tests/tools/oracle_exc_editor.py
"""
import ast
import logging
import math
import os
import sys
from collections import defaultdict
from copy import deepcopy
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from oracle_common import DATA, REF, ROOT, FakeApp, install_stubs, load  # noqa: E402
from oracle_geometry_modes import cs  # noqa: E402

OUT = os.path.join(ROOT, "tests", "golden_exc_editor.h")
FILES = ["luz_de_noche/luz de noche Pascual-PTH.drl", "excellon_samples/slots_g85.drl"]
EDITOR_METHODS = ("edit_fcexcellon", "update_fcexcellon", "add_exc_shape", "on_tool_delete", "on_exc_shape_complete",
                  "make_storage")
CLASSES = ("DrillAdd", "DrillArray", "SlotAdd", "SlotArray", "ResizeEditorExc", "SelectEditorExc")


class Val:
    def __init__(self, v):
        self.v = v

    def get_value(self):
        return self.v

    def set_value(self, v):
        self.v = v


def main():
    install_stubs()
    sys.path.insert(0, REF)
    camlib = load("camlib", os.path.join(REF, "camlib.py"))
    pe = load("ParseExcellon", os.path.join(REF, "appParsers", "ParseExcellon.py"))
    from shapely import wkt as W
    import shapely.affinity as affinity
    from shapely.geometry import LineString, MultiLineString, Point, Polygon
    app = FakeApp()
    app.defaults = defaultdict(lambda: 0, app.defaults)
    app.defaults.update({"excellon_circle_steps": 64, "geometry_circle_steps": 64, "units": "MM",
                         "global_mselect_key": "Control", "excellon_editor_newdia": 1.0})
    app.options = {}
    app.log = logging.getLogger("oracle")
    app.jump_signal = mock.MagicMock()
    app.ui = mock.MagicMock()
    pe.Excellon.app = app
    path = os.path.join(REF, "appEditors", "AppExcEditor.py")
    tree = ast.parse(open(path, encoding="utf-8").read())
    body = [n for n in tree.body if isinstance(n, ast.ClassDef) and n.name in CLASSES]
    ed = next(n for n in tree.body if isinstance(n, ast.ClassDef) and n.name == "AppExcEditor")
    meths = [f for f in ed.body if isinstance(f, ast.FunctionDef) and f.name in EDITOR_METHODS]
    body.append(ast.ClassDef(name="Core", bases=[], keywords=[], body=meths, decorator_list=[]))
    ga = ast.parse(open(os.path.join(REF, "appEditors", "AppGeoEditor.py"), encoding="utf-8").read())
    body = [n for n in ga.body if isinstance(n, ast.ClassDef) and n.name in ("DrawToolShape", "DrawToolUtilityShape",
                                                                             "DrawTool", "FCShapeTool")] + body
    m = ast.Module(body=body, type_ignores=[])
    ast.fix_missing_locations(m)
    qt = mock.MagicMock()
    ns = dict(QtGui=qt, QtCore=qt, QtWidgets=qt, Qt=qt, arc=camlib.arc, distance=camlib.distance, math=math,
              LineString=LineString, MultiLineString=MultiLineString, Point=Point, Polygon=Polygon, affinity=affinity,
              deepcopy=deepcopy, np=__import__("numpy"), _=lambda s: s, log=logging.getLogger("x"),
              FlatCAMRTreeStorage=camlib.FlatCAMRTreeStorage)
    exec(compile(m, path, "exec"), ns)
    Core = ns["Core"]
    ns["AppExcEditor"] = Core
    ns["AppGeoEditor"] = Core

    def make_storage():
        s = camlib.FlatCAMRTreeStorage()
        s.get_points = ns["DrawToolShape"].get_pts
        return s
    Core.make_storage = staticmethod(make_storage)
    ns["DrawToolShape"].tolerance = None
    d = lambda x: W.dumps(x, rounding_precision=17, trim=True)  # noqa: E731

    out = ["// ARCHIVO GENERADO por tests/tools/oracle_exc_editor.py - no editar a mano.",
           "// Referencia: appEditors/AppExcEditor.py original.", "#pragma once", "#include <string>",
           "#include <vector>", "namespace golden_exc_editor {",
           "struct Storage { double dia; std::vector<std::string> shapes; };",
           "struct OutTool { double dia; std::vector<double> drills; std::vector<double> slots; };",
           "struct Shapes { const char* what; std::vector<double> args; std::vector<std::string> wkt; };",
           "struct Case { const char* file; std::vector<Storage> loaded; std::vector<double> click;"
           " std::vector<int> clickSelected; std::vector<int> resizeSel; double resizeDia; bool resized;"
           " std::vector<Storage> afterResize; std::vector<OutTool> saved; };"]
    cases = []
    for f in FILES:
        e = pe.Excellon(geo_steps_per_circle=64)
        e.default_data = {}
        e.parse_file(os.path.join(DATA, f))
        e.create_geometry()
        e.options = {"name": "exc"}
        e.geo_steps_per_circle = 64
        core = Core()
        core.app = app
        core.decimals = 4
        core.units = "MM"
        core.storage_dict, core.olddia_newdia, core.tool2tooldia = {}, {}, {}
        core.new_tools, core.new_drills, core.new_slots = {}, [], []
        core.selected = []
        core.is_modified = False
        core.edited_obj_name = "exc"
        core.ui = mock.MagicMock()
        core.dec_format = lambda v, dec=4: float('%.*f' % (dec, v))
        for n in ("deactivate", "activate", "set_ui", "replot", "select_tool", "on_tool_add", "plot_all", "on_tool_edit", "on_row_selected",
                  "delete_utility_geometry"):
            setattr(core, n, lambda *a, **k: None)
        core.build_ui = lambda *a, **k: None
        core.new_edited_excellon = lambda *a, **k: None
        core.get_selected = lambda: core.selected
        core.edit_fcexcellon(e)
        for dia in core.storage_dict:
            core.olddia_newdia[dia] = dia
        loaded = [(dia, [d(s.geo) for s in st.get_objects()]) for dia, st in core.storage_dict.items()]
        # Un clic cerca del primer agujero o ranura del primer diámetro.
        first = next(iter(core.storage_dict.values()))
        g0 = next(iter(first.get_objects())).geo
        c = g0.centroid
        click = (c.x + 0.05, c.y + 0.02)
        sel_tool = ns["SelectEditorExc"].__new__(ns["SelectEditorExc"])
        sel_tool.draw_app = core
        sel_tool.storage = core.storage_dict
        sel_tool.sel_tools = set()
        sel_tool.sel_storage = make_storage()
        ns["QtWidgets"].QApplication.keyboardModifiers.return_value = None
        sel_tool.click_release(click)
        allobjs = [s for st in core.storage_dict.values() for s in st.get_objects()]
        click_sel = [allobjs.index(s) for s in core.selected]
        # Cambiar el diámetro: lo elegido más la segunda figura del primer diámetro.
        objs0 = list(first.get_objects())
        extra = [o for o in objs0 if o not in core.selected]
        if extra:
            core.selected.append(extra[0])
        resize_sel = [allobjs.index(s) for s in core.selected]
        sel_dia = list(core.storage_dict.keys())[0]
        rz = ns["ResizeEditorExc"].__new__(ns["ResizeEditorExc"])
        rz.draw_app = core
        rz.geometry = []
        rz.selected_dia_set = set()
        rz.name = "drill_resize"
        core.active_tool = rz
        core.tool_shape = mock.MagicMock()
        core.ui.resdrill_entry = Val(1.5)
        idx = mock.MagicMock()
        idx.row.return_value = 0
        core.ui.tools_table_exc.selectedIndexes.return_value = [idx]
        core.ui.tools_table_exc.item.return_value.text.return_value = str(sel_dia)
        core.points_edit = {k: list(v) for k, v in core.points_edit.items()}
        resized = True
        try:
            rz.make()
        except Exception as ex:  # noqa: BLE001
            resized = False
            print("  resize falla", ex)
        after = [(dia, [d(s.geo) for s in st.get_objects()]) for dia, st in core.storage_dict.items()]
        core.app.worker_task = mock.MagicMock()
        core.update_fcexcellon(e)
        params = core.app.worker_task.emit.call_args[0][0]["params"]
        new_tools = params[3]
        saved = []
        for t in sorted(new_tools):
            v = new_tools[t]
            drills = [c for p in v.get("drills", []) for c in (p.x, p.y)]
            slots = [c for s in v.get("slots", []) for c in (s[0].x, s[0].y, s[1].x, s[1].y)]
            saved.append((v["tooldia"], drills, slots))
        cases.append((f, loaded, click, click_sel, resize_sel, 1.5, resized, after, saved))
        print(f, [(dia, len(w)) for dia, w in loaded], click_sel, resize_sel, resized,
              [(dia, len(w)) for dia, w in after], [(s[0], len(s[1]) // 2, len(s[2]) // 4) for s in saved])

    # Las figuras de las herramientas de dibujo.
    shapes = []
    draw = mock.MagicMock()
    draw.tool2tooldia = {1: 0.8}
    draw.last_tool_selected = 1
    draw.app = app
    for axis, length, angle in (("X", 3.0, 0.0), ("Y", 3.0, 0.0), ("A", 3.0, 30.0), ("A", 0.5, 30.0),
                                ("X", 0.5, 0.0)):
        draw.ui.slot_length_entry = Val(length)
        draw.ui.slot_angle_spinner = Val(angle)
        draw.ui.slot_axis_radio = Val(axis)
        for cls in ("SlotAdd", "SlotArray"):
            t = ns[cls].__new__(ns[cls])
            t.draw_app = draw
            shapes.append(("slot" if cls == "SlotAdd" else "slotArray", [ord(axis), length, angle],
                           [d(t.util_shape((1.25, -2.5)))]))
    t = ns["DrillAdd"].__new__(ns["DrillAdd"])
    t.draw_app = draw
    t.selected_dia = 0.8
    shapes.append(("drill", [], [d(t.util_shape((1.25, -2.5)))]))
    for axis, angle in (("X", 0.0), ("Y", 0.0), ("A", 30.0)):
        t = ns["DrillArray"].__new__(ns["DrillArray"])
        t.draw_app = draw
        t.selected_dia = 0.8
        t.drill_array_size, t.drill_pitch, t.drill_linear_angle, t.drill_axis = 4, 2.5, angle, axis
        t.drill_array = "linear"
        t.points = [1.0, 2.0]
        draw.points_edit = {}
        draw.storage_dict = {0.8: None}
        t.make()
        shapes.append(("drillLinear", [ord(axis), angle], [d(g.geo) for g in t.geometry]))
    for direction in ("CW", "CCW"):
        for cls, kind in (("DrillArray", "drillCircular"), ("SlotArray", "slotCircular")):
            t = ns[cls].__new__(ns[cls])
            t.draw_app = draw
            t.selected_dia = 0.8
            t.origin, t.destination = (0.0, 0.0), (-3.0, 4.0)
            if cls == "DrillArray":
                draw.ui.drill_array_dir_radio = Val(direction)
                draw.ui.drill_angle_entry = Val(45.0)
                t.drill_array_size = 5
                geo = t.circular_util_shape(-5.0, math.asin(4.0 / -5.0))
            else:
                draw.ui.slot_array_direction_radio = Val(direction)
                draw.ui.slot_array_angle_entry = Val(45.0)
                draw.ui.slot_array_size_entry = Val(5)
                draw.ui.slot_axis_radio = Val("X")
                draw.ui.slot_length_entry = Val(2.0)
                geo = t.circular_util_shape(-5.0, math.asin(4.0 / -5.0))
            shapes.append((kind, [1.0 if direction == "CW" else 0.0], [d(g.geo) for g in geo]))

    def stor(lst):
        return ", ".join("{%r, {%s}}" % (dia, ", ".join(cs(w) for w in ws)) for dia, ws in lst)
    out.append("inline const std::vector<Case> kCases = {")
    for f, loaded, click, csel, rsel, rdia, ok, after, saved in cases:
        out.append("{%s, {%s}, {%r, %r}, {%s}, {%s}, %r, %s, {%s}, {%s}}," % (
            cs(f), stor(loaded), click[0], click[1], ", ".join(map(str, csel)), ", ".join(map(str, rsel)), rdia,
            "true" if ok else "false", stor(after),
            ", ".join("{%r, {%s}, {%s}}" % (dia, ", ".join(repr(v) for v in dr), ", ".join(repr(v) for v in sl))
                      for dia, dr, sl in saved)))
    out.append("};")
    out.append("inline const std::vector<Shapes> kShapes = {")
    for what, args, wkts in shapes:
        out.append("{\"%s\", {%s}, {%s}}," % (what, ", ".join(repr(float(a)) for a in args),
                                             ", ".join(cs(w) for w in wkts)))
    out += ["};", "}  // namespace golden_exc_editor"]
    with open(OUT, "w") as fh:
        fh.write("\n".join(out) + "\n")
    print("Escrito", OUT, "(%.0f KB)" % (os.path.getsize(OUT) / 1024))


if __name__ == "__main__":
    main()
