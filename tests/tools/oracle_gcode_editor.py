#!/usr/bin/env python3
"""Oráculo del editor de G-code (`appEditors/appGCodeEditor.py`) y de "Revisar código CNC" del CNC Job.

Con la interfaz PyQt5 real abre los dos CNC Job del proyecto del usuario (`luz_de_noche_reducido.FlatPrj`: el corte
y la perforación) como `open_project` (`from_dict`, `set_ui`, `build_ui`). El original no guarda `gc_start` en los
proyectos y el port lo reconstruye (decisión del usuario, AGENTS.md 11.13); para comparar lo mismo, el oráculo le pone
el código de inicio del `.nc` real del usuario (idéntico al reconstruido) y arma `source_file` otra vez como `set_ui`.
Después ejecuta el código ORIGINAL de:

- `AppGCodeEditor.edit_fcgcode`: la pestaña (título, nombre), el texto cargado, el panel (nombre, tablas visibles y
  sus celdas, los fragmentos);
- `on_row_selection_change` con varias combinaciones de filas: la selección del texto y las selecciones subrayadas.
  En un trabajo leído de un proyecto las claves de `cnc_tools` y `exc_cnc_tools` son textos y la fila de una
  herramienta falla (`KeyError` en geometría, `TypeError` en `dec_format` en un taladrado); por eso se repiten los
  dos con claves numéricas (como un trabajo creado en la sesión);
- `on_toggle_all_rows` y `update_fcgcode` (el texto editado pasa a `source_file`);
- `CNCJobObject.on_review_code_click`: la pestaña de solo lectura y los botones ocultos.

Escribe `tests/data/gcode_editor/oracle.json`.
Uso: MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
     python tests/tools/oracle_gcode_editor.py
"""
import json
import lzma
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402

DATA = os.path.join(ROOT, "tests", "data", "luz_de_noche")
OUT_DIR = os.path.join(ROOT, "tests", "data", "gcode_editor")
JOBS = {"luz de noche Pascual-PTH.drl_cnc": "luz de noche Pascual-PTH.drl_cnc.nc",
        "luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc": "luz de noche Pascual-Edge_Cuts.gbr_cutout_cnc.nc"}


def table_dump(t):
    rows = []
    for r in range(t.rowCount()):
        rows.append([t.item(r, c).text() if t.item(r, c) else None for c in range(t.columnCount())])
    return {"hidden": t.isHidden(), "rows": rows,
            "hiddenColumns": [c for c in range(t.columnCount()) if t.isColumnHidden(c)]}


def selection_state(edit):
    c = edit.textCursor()
    extra = [[s.cursor.selectionStart(), s.cursor.selectionEnd()] for s in edit.extraSelections()]
    return {"start": c.selectionStart(), "end": c.selectionEnd(), "text": c.selectedText(), "extra": extra}


def main():
    qapp, fa, gui = fr.build_app()
    from PyQt5 import QtCore
    camlib = fr.import_original("camlib")
    ou = fr.import_original("appGUI.ObjectUI")
    fr.import_original("appObjects.FlatCAMObj").FlatCAMObj.app = fa
    fa.preprocessors = fr.import_original("appPreProcessor").load_preprocessors()
    fa.myKeywords = []
    # La aplicación simulada no trae dec_format: el de app_Main.
    fa.dec_format = lambda val, dec=None: float('%.*f' % (dec if dec is not None else fa.decimals, val))
    cnc_mod = fr.import_original("appObjects.FlatCAMCNCJob")
    ge = fr.import_original("appEditors.appGCodeEditor")
    with lzma.open(os.path.join(DATA, "luz_de_noche_reducido.FlatPrj"), "rb") as f:
        d = json.loads(f.read().decode("utf-8"), object_hook=camlib.dict2obj)
    fa.options.update(d["options"])
    out = {"jobs": {}}
    for od in d["objs"]:
        name = od["options"]["name"]
        if od["kind"] != "cncjob" or name not in JOBS:
            continue
        for int_keys in (False, True):
            obj = cnc_mod.CNCJobObject(name)
            for option in fa.options:
                if option.find("cncjob_") == 0:
                    obj.options[option[len("cncjob_"):]] = fa.options[option]
            obj.from_dict(od)
            if int_keys:
                # Como un trabajo creado en la sesión: claves numéricas.
                obj.cnc_tools = {int(k): v for k, v in obj.cnc_tools.items()}
                obj.exc_cnc_tools = {float(k): v for k, v in obj.exc_cnc_tools.items()}
            obj.app.defaults = fa.defaults
            # gc_start: el del .nc real (lo que el port reconstruye).
            with open(os.path.join(DATA, JOBS[name]), newline="") as f:
                real = f.read().replace("\r\n", "\n")
            gcode = "".join(v["gcode"] for v in (obj.exc_cnc_tools or obj.cnc_tools).values() if v.get("gcode")) \
                if obj.multitool else obj.gcode
            body = real[len(obj.gc_header):]
            assert real.startswith(obj.gc_header) and body.endswith("\n" + gcode + "\n"), name
            obj.gc_start = body[:len(body) - len(gcode) - 2]
            obj.set_ui(ou.CNCObjectUI(app=fa))
            obj.build_ui()
            assert obj.source_file == real, name

            ed = ge.AppGCodeEditor(fa)
            fa.gcode_editor = ed
            ed.edit_fcgcode(obj)
            tab = ed.ui.gcode_editor_tab
            area = gui.plot_tab_area
            idx = area.indexOf(tab)
            edit = ed.edit_area
            job = {"tabTitle": area.tabText(idx), "tabName": tab.objectName(), "text": edit.toPlainText(),
                   "nameEntry": ed.ui.name_entry.get_value(), "cnc": table_dump(ed.ui.cnc_tools_table),
                   "exc": table_dump(ed.ui.exc_cnc_tools_table), "runHidden": tab.buttonRun.isHidden(),
                   "header": obj.gc_header, "start": obj.gc_start, "selections": []}
            table = ed.ui.cnc_tools_table if obj.cnc_tools else ed.ui.exc_cnc_tools_table
            n = table.rowCount()
            combos = [[0], [1], [2], [1, 2]] + [[r] for r in range(3, n)] + [[1, n - 1], [3, n - 1], [2, 3]]
            if n > 5:
                combos += [[3, 5], [4, 5, 6]]
            for rows in combos:
                edit.setExtraSelections([])
                table.clearSelection()
                for r in rows:
                    table.selectionModel().select(table.model().index(r, 0),
                                                  QtCore.QItemSelectionModel.Select | QtCore.QItemSelectionModel.Rows)
                err = None
                try:
                    ed.on_row_selection_change()
                except Exception as e:  # noqa: BLE001 (en la GUI la excepción corta el manejador)
                    err = type(e).__name__
                s = selection_state(edit)
                s["rows"] = rows
                s["error"] = err
                job["selections"].append(s)
            # on_toggle_all_rows: con una fila elegida, elige todas; con todas, ninguna.
            table.clearSelection()
            table.selectRow(3)
            ed.on_toggle_all_rows()
            all_rows = len({i.row() for i in table.selectionModel().selectedIndexes()})
            ed.on_toggle_all_rows()
            none_rows = len({i.row() for i in table.selectionModel().selectedIndexes()})
            job["toggle"] = [all_rows, none_rows]
            # update_fcgcode: el texto editado pasa a source_file.
            edit.moveCursor(edit.textCursor().End)
            edit.insertPlainText("M30\n")
            ed.update_fcgcode(obj)
            job["savedSourceTail"] = obj.source_file[-40:]
            # Revisar código CNC.
            obj.on_review_code_click()
            rv = obj.gcode_editor_tab
            job["review"] = {"tabTitle": area.tabText(area.indexOf(rv)), "tabName": rv.objectName(),
                             "readOnly": rv.code_editor.isReadOnly(), "text": rv.code_editor.toPlainText(),
                             "hidden": {k: getattr(rv, k).isHidden() for k in
                                        ("buttonSave", "buttonOpen", "buttonReplace", "sel_all_cb", "entryReplace",
                                         "buttonRun", "buttonFind", "buttonPrint", "buttonPreview")}}
            out["jobs"][name + (" (int)" if int_keys else "")] = job
            area.removeTab(area.indexOf(tab))
            area.removeTab(area.indexOf(rv))
    os.makedirs(OUT_DIR, exist_ok=True)
    with open(os.path.join(OUT_DIR, "oracle.json"), "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=1)
    print("ok", list(out["jobs"]))


if __name__ == "__main__":
    main()
