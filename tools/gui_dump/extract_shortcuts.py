#!/usr/bin/env python3
"""Extrae los textos de la pestaña "Lista de atajos" (`ShortcutsTab` de appGUI/MainGUI.py) ya traducidos al español.

Construye la ventana original (fc_runtime.build_app) y guarda el título y el HTML de las dos columnas tal como los arma
el original (`app_sh_msg`, `editor_sh_msg`) en `src/gui/resources/shortcuts.json`, que usa `ShortcutsTab` en C++.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old env QT_QPA_PLATFORM=offscreen python \
      tools/gui_dump/extract_shortcuts.py
"""
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fc_runtime as fr  # noqa: E402

OUT = os.path.join(fr.ROOT, "src", "gui", "resources", "shortcuts.json")


def main():
    qapp, fa, gui = fr.build_app()
    tab = gui.shortcuts_tab
    data = {"title": '<b>%s</b>' % __import__("builtins")._("Shortcut Key List"), "app": tab.app_sh_msg,
            "editor": tab.editor_sh_msg, "tab": __import__("builtins")._("Key Shortcut List")}
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False, indent=1)
    print("Escrito", OUT)


if __name__ == "__main__":
    main()
