#!/usr/bin/env python3
"""Oráculo de la tabla de Perforación y de los números `T` del G-code (`ToolDrilling` original, interfaz PyQt5 real).

`build_tool_ui` ordena y renumera `excellon_tools` (1, 2, 3...) y `on_cnc_button_click` emite cada herramienta con ese
número, no con el del archivo. Sobre `tests/data/excellon_samples/renumber.drl` (T5 de 1,0, T2 de 0,5 y T9 de 0,8,
definidas en ese orden) arma la tabla con el orden "no", "fwd", "rev" y otra vez "no" (queda el último orden), y genera
el CNC Job de todas las herramientas, con cambio de herramienta, con "fwd" y con "no" (optimización 'T' con el orden del archivo, como
`oracle_drill.py`): las filas de la tabla, las líneas `T`/`M6` del G-code y `exc_cnc_tools`. Con "no" (la segunda
generación) el original suma el G-code de la anterior (`total_gcode` no se vacía con cambio de herramienta).

Escribe `tests/data/drill_renumber/oracle.json`.
Uso: MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
     python tests/tools/oracle_drill_renumber.py
"""
import json
import os
import sys
from unittest import mock

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402

FILE = os.path.join(ROOT, "tests", "data", "excellon_samples", "renumber.drl")
OUT = os.path.join(ROOT, "tests", "data", "drill_renumber", "oracle.json")


def table(t):
    rows = []
    for r in range(t.rowCount()):
        rows.append([t.item(r, c).text() if t.item(r, c) else None for c in range(5)])
    return rows


def main():
    qapp, fa, gui = fr.build_app()
    fr.import_original("appObjects.FlatCAMObj").FlatCAMObj.app = fa
    fa.preprocessors = fr.import_original("appPreProcessor").load_preprocessors()
    fa.dec_format = lambda val, dec=None: float('%.*f' % (dec if dec is not None else fa.decimals, val))
    exc_mod = fr.import_original("appObjects.FlatCAMExcellon")
    cnc_mod = fr.import_original("appObjects.FlatCAMCNCJob")
    fa.abort_flag = False  # si no, el analizador sale con GracefulException
    exc = exc_mod.ExcellonObject("renumber.drl")
    for option in fa.options:
        if option.find("excellon_") == 0:
            exc.options[option[len("excellon_"):]] = fa.options[option]
    exc.default_data = {}
    exc.parse_file(FILE)
    exc.create_geometry()
    xmin, ymin, xmax, ymax = exc.bounds()  # new_object pone los límites en las opciones
    exc.options.update({"xmin": xmin, "ymin": ymin, "xmax": xmax, "ymax": ymax})
    fa.collection.get_by_name = lambda n, *a, **k: exc
    dt = fa.drilling_tool
    dt.dec_format = fa.dec_format  # la herramienta guardó el de la aplicación simulada
    dt.excellon_obj = exc
    dt.excellon_tools = exc.tools
    for tk in dt.excellon_tools:
        dt.excellon_tools[tk]['data'] = {k: v for k, v in fa.defaults.items() if k.startswith("tools_drill_")}
        dt.excellon_tools[tk]['data']['offset'] = 0.0
    dt.t_ui.object_combo.blockSignals(True)
    dt.t_ui.pp_excellon_name_cb.clear()
    dt.t_ui.pp_excellon_name_cb.addItems(list(fa.preprocessors.keys()))
    dt.t_ui.pp_excellon_name_cb.set_value("default")
    out = {"tables": {}, "gcode": {}}
    for name, order in (("no", "no"), ("fwd", "fwd"), ("rev", "rev"), ("no after rev", "no")):
        dt.t_ui.order_radio.set_value(order)
        dt.build_tool_ui()
        out["tables"][name] = table(dt.t_ui.tools_table)
    fa.defaults["tools_drill_excellon_optimization_type"] = "T"
    fa.defaults["excellon_optimization_type"] = "T"
    orig_init = cnc_mod.CNCJobObject.__init__
    dt.t_ui.toolchange_cb.set_value(True)
    for name, order in (("fwd", "fwd"), ("no", "no")):
        dt.excellon_tools = {k: v for k, v in exc.tools.items()}  # el orden del archivo
        dt.t_ui.order_radio.set_value(order)
        dt.build_tool_ui()
        dt.t_ui.tools_table.selectAll()
        made = []

        def new_object(kind, jname, init, *a, **k):
            obj = cnc_mod.CNCJobObject(jname)
            for option in fa.options:
                if option.find("cncjob_") == 0:
                    obj.options[option[len("cncjob_"):]] = fa.options[option]
            obj.optimized_travelling_salesman = lambda points, start=None: list(points)
            init(obj, fa)
            made.append(obj)

        fa.app_obj.new_object = new_object
        fa.worker_task.emit.side_effect = lambda d: d['fcn'](*d['params'])
        with mock.patch.object(cnc_mod.CNCJobObject, "__init__", orig_init):
            dt.on_cnc_button_click()
        job = made[0]
        # Las líneas de cada herramienta (los movimientos a los agujeros no salen en esta simulación; los compara
        # oracle_drill.py).
        lines = [ln for ln in job.gcode.split("\n") if ln.startswith(("T", "M6"))]
        out["gcode"][name] = {"lines": lines,
                              "exc_cnc_tools": [[str(k), v['tool'], v['nr_drills']] for k, v in job.exc_cnc_tools.items()]}
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=1)
    print(json.dumps(out))


if __name__ == "__main__":
    main()
