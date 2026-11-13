#!/usr/bin/env python3
"""Oráculo de los objetos Script y Documento y de su editor de texto (`AppTextEditor`).

Ejecuta con la interfaz PyQt5 real (como `tools/gui_dump/dump_panels.py`) el código ORIGINAL de:

- `AppObject.new_object` con `new_script_object` y `new_document_object` (las opciones de cada objeto, en su orden, y
  el texto con que arranca el script), y `ObjectCollection.append`, que llama a `set_ui` y deja el editor cargado;
- `MenuFileHandlers.open_script` sobre `tests/data/scripts/` (el nombre y el texto leído) y `save_source_file` (el
  archivo que escribe, con la hora fija);
- un Documento con HTML y otro con texto (lo que queda en `source_file` después de `set_ui` y de editar el texto);
- `AppTextEditor`: los botones (texto, ayuda, ícono, si se ven) del editor de texto plano y del de texto con formato,
  buscar y reemplazar (`handleFindGCode`, `handleReplaceGCode`), comentar con Ctrl+/ (`comment`), los paréntesis
  automáticos y `handleSaveGCode` a un `.txt`.

Escribe `tests/data/scripts/oracle.json`.
Uso: MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
     python tests/tools/oracle_script_document.py
"""
import datetime as _dt
import json
import os
import sys
import tempfile
from unittest import mock

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402

DATA = os.path.join(ROOT, "tests", "data", "scripts")
OUT = os.path.join(DATA, "oracle.json")


def jsonable(v):
    if isinstance(v, (str, int, float, bool)) or v is None:
        return v
    if isinstance(v, (list, tuple)):
        return [jsonable(x) for x in v]
    if isinstance(v, dict):
        return {str(k): jsonable(x) for k, x in v.items()}
    return repr(v)


def obj_dict(obj):
    d = obj.to_dict()
    return {"options": [[k, jsonable(v)] for k, v in d["options"].items()], "kind": d["kind"],
            "source_file": d["source_file"]}


def widgets_of(editor):
    out = []
    for name in ("buttonFind", "entryFind", "buttonReplace", "entryReplace", "sel_all_cb", "button_update_code",
                 "buttonPreview", "buttonPrint", "buttonOpen", "buttonSave", "buttonRun"):
        w = getattr(editor, name)
        icon = fr.icon_path(w.icon()) if hasattr(w, "icon") else None
        text = w.text() if hasattr(w, "text") else ""
        out.append({"name": name, "class": type(w).__name__, "text": text, "toolTip": w.toolTip(), "icon": icon,
                    "hidden": w.isHidden()})
    return out


def main():
    qapp, fa, gui = fr.build_app()
    from PyQt5 import QtCore, QtGui, QtWidgets
    fa.myKeywords = []
    fa.main_thread = QtCore.QThread.currentThread()
    fr.import_original("appObjects.FlatCAMObj").FlatCAMObj.app = fa
    ao_mod = fr.import_original("appObjects.AppObject")
    created = []

    class FakeAppObject:
        new_object = ao_mod.AppObject.new_object
        new_script_object = ao_mod.AppObject.new_script_object
        new_document_object = ao_mod.AppObject.new_document_object

        def __init__(self):
            self.app = fa
            self.object_created = mock.MagicMock()
            self.object_created.emit.side_effect = lambda obj, *a: (fa.collection.append(obj), created.append(obj))

    fa.app_obj = FakeAppObject()
    out = {}

    # --- objetos nuevos
    fa.app_obj.new_script_object()
    s = created[-1]
    out["new_script"] = obj_dict(s)
    out["new_script_editor_text"] = s.script_editor_tab.code_editor.toPlainText()
    out["new_script_tab"] = [gui.plot_tab_area.tabText(gui.plot_tab_area.indexOf(s.script_editor_tab)),
                             s.script_editor_tab.objectName()]
    fa.app_obj.new_document_object()
    d = created[-1]
    out["new_document"] = obj_dict(d)
    out["new_document_tab"] = [gui.plot_tab_area.tabText(gui.plot_tab_area.indexOf(d.document_editor_tab)),
                               d.document_editor_tab.objectName()]
    out["document_ui"] = {"font_size": d.ui.font_size_cb.currentText(), "tab_size": d.ui.tab_size_spinner.get_value(),
                          "font_color": d.ui.font_color_entry.get_value(), "sel_color": d.ui.sel_color_entry.get_value(),
                          "font_color_style": d.ui.font_color_button.styleSheet(),
                          "autocomplete": d.ui.autocomplete_cb.get_value(),
                          "editor_style": d.document_editor_tab.code_editor.styleSheet()}
    out["script_ui"] = {"autocomplete": s.ui.autocomplete_cb.get_value()}

    # --- Documento con HTML y con texto: lo que queda en source_file al editar
    def new_doc(name, text):
        def init(obj, app):
            obj.source_file = text
        fa.app_obj.new_object("document", name, init, plot=False)
        return created[-1]

    html = "<html><body><p><b>Informe</b></p><p>linea 2</p></body></html>"
    dh = new_doc("doc_html", html)
    dt = new_doc("doc_texto", "linea 1\nlinea 2")
    dh.document_editor_tab.code_editor.insertPlainText("agregado ")
    out["documents"] = {"html": {"source_after_edit": dh.source_file,
                                 "plain": dh.document_editor_tab.code_editor.toPlainText()},
                        "text": {"source_after_edit": dt.source_file,
                                 "plain": dt.document_editor_tab.code_editor.toPlainText(),
                                 "might_be_rich": [bool(QtCore.Qt.mightBeRichText(x)) for x in (html, "linea 1\nlinea 2",
                                                                                               "a < b", "<b>x</b>")]}}

    # --- abrir un script (open_script) y guardar su fuente (save_source_file)
    mfh = fr.import_original("app_Main").MenuFileHandlers
    h = mock.MagicMock()
    h.app = fa
    h.defaults = fa.defaults
    h.inform = mock.MagicMock()
    opened = []
    for fname in ("ejemplo.FlatScript", "bucle.tcl"):
        path = os.path.join(DATA, fname).replace("\\", "/")
        mfh.open_script(h, path)
        o = created[-1]
        opened.append({"file": fname, "dict": obj_dict(o), "editor_text": o.script_editor_tab.code_editor.toPlainText()})
    out["opened"] = opened
    h.app.collection = fa.collection

    class FixedDate(_dt.datetime):
        @classmethod
        def now(cls, tz=None):
            return cls(2024, 5, 17, 9, 30)

    import app_Main
    tmp = tempfile.mkdtemp()
    target = os.path.join(tmp, "guardado.FlatScript")
    with mock.patch.object(app_Main, "datetime", FixedDate):
        mfh.save_source_file(h, created[-2].options["name"], target)
    with open(target, newline="") as f:
        out["saved_source"] = f.read()
    # el texto vacío no se guarda
    out["save_empty"] = mfh.save_source_file(h, d.options["name"], os.path.join(tmp, "vacio.txt"))

    # --- el editor de texto
    plain = fr.import_original("appEditors.AppTextEditor").AppTextEditor(app=fa, plain_text=True)
    rich = fr.import_original("appEditors.AppTextEditor").AppTextEditor(app=fa)
    out["editor_plain"] = widgets_of(plain)
    out["editor_rich"] = widgets_of(rich)
    out["editor_style"] = [plain.code_editor.styleSheet(), rich.code_editor.styleSheet()]
    out["script_editor"] = widgets_of(s.script_editor_tab)
    out["document_editor"] = widgets_of(d.document_editor_tab)

    # buscar: cada clic busca desde el cursor; al final vuelve al principio
    text = "G01 X1\nG00 Z5\nG01 X2\ng01 X3\n"
    plain.load_text(text, move_to_start=True)
    plain.entryFind.set_value("G01")
    finds = []
    for _ in range(4):
        plain.handleFindGCode()
        c = plain.code_editor.textCursor()
        finds.append([c.selectionStart(), c.selectionEnd()])
    out["find"] = finds

    # reemplazar uno (lo elegido) y todos
    plain.entryReplace.set_value("G1")
    plain.handleReplaceGCode()
    out["replace_one"] = plain.code_editor.toPlainText()
    plain.sel_all_cb.setChecked(True)
    plain.load_text(text, move_to_start=True)
    plain.code_editor.moveCursor(QtGui.QTextCursor.End)
    plain.handleReplaceGCode()
    out["replace_all"] = plain.code_editor.toPlainText()
    out["replace_all_cursor"] = plain.code_editor.textCursor().position()

    # comentar / descomentar con Ctrl+/
    def press(editor, key, mod=QtCore.Qt.NoModifier, text=""):
        ev = QtGui.QKeyEvent(QtCore.QEvent.KeyPress, key, mod, text)
        with mock.patch.object(QtWidgets.QApplication, "keyboardModifiers", return_value=mod):
            editor.keyPressEvent(ev)

    comments = []
    for src, pos in (("set a 1\nputs $a\n", 2), ("# set a 1\nputs $a\n", 4), ("  indentado\n", 5),
                     ("  # indentado\n", 6)):
        for ed, nm in ((plain.code_editor, "plain"), (rich.code_editor, "rich")):
            ed.setPlainText(src)
            c = ed.textCursor()
            c.setPosition(pos)
            ed.setTextCursor(c)
            press(ed, QtCore.Qt.Key_Slash, QtCore.Qt.ControlModifier, "/")
            comments.append({"editor": nm, "src": src, "pos": pos, "result": ed.toPlainText(),
                             "cursor": ed.textCursor().position()})
    out["comment"] = comments

    # paréntesis automáticos
    brackets = []
    for seq in (["{"], ["(", ")"], ["[", "x", "]"], ["a", "}"]):
        for ed, nm in ((plain.code_editor, "plain"), (rich.code_editor, "rich")):
            ed.setPlainText("")
            for ch in seq:
                key = {"{": QtCore.Qt.Key_BraceLeft, "}": QtCore.Qt.Key_BraceRight, "(": QtCore.Qt.Key_ParenLeft,
                       ")": QtCore.Qt.Key_ParenRight, "[": QtCore.Qt.Key_BracketLeft,
                       "]": QtCore.Qt.Key_BracketRight}.get(ch, QtCore.Qt.Key_A + ord(ch) - ord("a") if ch.isalpha()
                                                            else 0)
                press(ed, key, QtCore.Qt.NoModifier, ch)
            brackets.append({"editor": nm, "keys": "".join(seq), "result": ed.toPlainText(),
                             "cursor": ed.textCursor().position()})
    out["brackets"] = brackets

    # guardar el texto del editor (handleSaveGCode) en un .txt
    plain.load_text("linea 1\nlinea 2\n", move_to_start=True)
    fa.collection.get_active = lambda: None
    sv = os.path.join(tmp, "editor.txt")
    with mock.patch.object(fr.import_original("appEditors.AppTextEditor").FCFileSaveDialog, "get_saved_filename",
                           return_value=(sv, "")):
        plain.handleSaveGCode()
    with open(sv, newline="") as f:
        out["editor_saved"] = f.read()
    out["save_button_after_save"] = [plain.buttonSave.styleSheet(), fr.icon_path(plain.buttonSave.icon())]
    plain.code_editor.insertPlainText("x")
    out["save_button_after_edit"] = [plain.buttonSave.styleSheet(), fr.icon_path(plain.buttonSave.icon())]

    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=1)
    print("escrito", OUT)


if __name__ == "__main__":
    main()
