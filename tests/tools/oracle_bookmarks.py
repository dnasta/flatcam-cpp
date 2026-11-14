#!/usr/bin/env python3
"""Oráculo de los marcadores (Ayuda -> Marcadores): `Bookmark.BookmarkManager` y `App.install_bookmarks`,
`on_bookmarks_manager` originales con la interfaz PyQt5 real.

Pasos encadenados, guardando después de cada uno el menú (texto de cada acción, y si es el administrador o un
separador), `global_bookmarks` y la tabla (número, título, enlace y si la fila está en gris) y los avisos:
arranque (`install_bookmarks()`), abrir el administrador, agregar con los campos (con "/" final, título vacío,
enlace vacío, repetido), quitar FlatCAM (no se puede) y uno agregado, exportar, importar (con líneas raras), volver a
instalar con un diccionario de 12 marcadores y el límite en 3 (las claves como texto: "10" antes que "2") y
`rebuild_actions`.

Escribe `tests/data/bookmarks/oracle.json` (y los archivos de exportar/importar en esa carpeta).
Uso: MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen \\
     python tests/tools/oracle_bookmarks.py
"""
import json
import os
import sys
from unittest import mock

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, os.path.join(ROOT, "tools", "gui_dump"))
import fc_runtime as fr  # noqa: E402

DATA = os.path.join(ROOT, "tests", "data", "bookmarks")


def main():
    os.makedirs(DATA, exist_ok=True)
    qapp, fa, gui = fr.build_app()
    from PyQt5 import QtWidgets
    app_main = fr.import_original("app_Main")
    bm_mod = fr.import_original("Bookmark")
    fa.defaults["global_bookmarks"] = {}
    fa.defaults["global_bookmarks_limit"] = 10
    fa.get_last_save_folder = lambda: DATA
    fa.install_bookmarks = lambda book_dict=None: app_main.App.install_bookmarks(fa, book_dict)
    fa.on_backup_site = lambda: None
    fa.on_bookmarks_manager = lambda: app_main.App.on_bookmarks_manager(fa)
    fa.book_dialog_tab = None
    msgs = []
    fa.inform = mock.MagicMock()
    fa.inform.emit.side_effect = lambda m, *a: msgs.append(m)
    steps = []

    def snap(name, mgr=None):
        menu = []
        for a in gui.menuhelp_bookmarks.actions():
            menu.append({"text": a.text(), "separator": a.isSeparator(),
                         "manager": a is gui.menuhelp_bookmarks_manager})
        st = {"step": name, "menu": menu, "dict": [[k, list(v)] for k, v in fa.defaults["global_bookmarks"].items()],
              "msgs": [m.replace(DATA, "<DATA>") for m in msgs]}  # sin la ruta local (el repositorio es público)
        if mgr is not None:
            t = mgr.table_widget
            rows = []
            for r in range(t.rowCount()):
                w = t.cellWidget(r, 2)
                rows.append([t.item(r, 0).text(), t.item(r, 1).text(), w.toPlainText() if w else None,
                             t.item(r, 0).background().color().name()])
            st["table"] = rows
            st["bm"] = [[k, list(v)] for k, v in mgr.bm_dict.items()]
        steps.append(st)
        msgs.clear()

    fa.install_bookmarks()
    snap("install")
    fa.on_bookmarks_manager()
    mgr = fa.book_dialog_tab
    area = gui.plot_tab_area
    steps_tab = {"title": area.tabText(area.indexOf(mgr)), "name": mgr.objectName(),
                 "headers": [mgr.table_widget.horizontalHeaderItem(i).text() for i in range(3)],
                 "tips": [mgr.table_widget.horizontalHeaderItem(i).toolTip() for i in range(3)],
                 "link": mgr.link_entry.get_value(),
                 "buttons": [b.text() for b in mgr.findChildren(QtWidgets.QPushButton)],
                 "labels": [lb.text() for lb in mgr.findChildren(QtWidgets.QLabel)]}
    snap("manager", mgr)
    for title, link in [("Foro", "https://foro.example/"), ("", "https://x.example"), ("Otro", "http://"),
                        ("Foro", "https://y.example"), ("Z", "http://flatcam.org"), ("Docs", "https://docs.example")]:
        mgr.title_entry.set_value(title)
        mgr.link_entry.set_value(link)
        mgr.on_add_entry()
        snap("add %s %s" % (title, link), mgr)
    # Quitar FlatCAM (no se puede) y "Foro".
    for title in ("FlatCAM", "Foro"):
        mgr.table_widget.clearSelection()
        for r in range(mgr.table_widget.rowCount()):
            if mgr.table_widget.item(r, 1).text() == title:
                mgr.table_widget.selectRow(r)
        mgr.on_remove_entry()
        snap("remove " + title, mgr)
    # Sin nada elegido.
    mgr.table_widget.clearSelection()
    mgr.on_remove_entry()
    snap("remove nothing", mgr)
    exp = os.path.join(DATA, "exported.txt")
    with mock.patch.object(bm_mod.FCFileSaveDialog, "get_saved_filename", return_value=(exp, "")):
        mgr.on_export_bookmarks()
    with open(exp) as f:
        exported = f.read()
    snap("export", mgr)
    imp = os.path.join(DATA, "import.txt")
    with open(imp, "w") as f:
        f.write("Uno: http://uno.example\nDos:http://dos.example/\nsin dos puntos\nTres:http://\n"
                "Uno:http://otro.example\n:http://vacio.example\nCuatro:http://cuatro.example")
    with mock.patch.object(QtWidgets.QFileDialog, "getOpenFileName", return_value=(imp, "")):
        mgr.on_import_bookmarks()
    snap("import", mgr)
    # Lo exportado no se puede volver a importar como estaba (una línea en blanco corta la importación).
    imp2 = os.path.join(DATA, "import2.txt")
    with open(imp2, "w") as f:
        f.write("Cinco:http://cinco.example\n\nSeis:http://seis.example\n")
    with mock.patch.object(QtWidgets.QFileDialog, "getOpenFileName", return_value=(imp2, "")):
        mgr.on_import_bookmarks()
    snap("import blank", mgr)
    mgr.rebuild_actions()
    snap("rebuild", mgr)
    fa.defaults["global_bookmarks_limit"] = 3
    many = {str(i): ["M%d" % i, "http://m%d.example" % i] for i in (1, 10, 11, 12, 2, 3, 4, 5, 6, 7, 8, 9)}
    fa.install_bookmarks(book_dict=many)
    snap("install limit 3")
    fa.defaults["global_bookmarks_limit"] = 10
    fa.install_bookmarks()
    snap("install again")
    with open(os.path.join(DATA, "oracle.json"), "w", encoding="utf-8") as f:
        json.dump({"tab": steps_tab, "exported": exported, "steps": steps}, f, ensure_ascii=False, indent=1)
    print("ok", len(steps))


if __name__ == "__main__":
    main()
