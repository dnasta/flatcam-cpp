#!/usr/bin/env python3
"""Extrae los íconos y la traducción al español del FlatCAM 2024.4 original para la GUI en C++.

- Íconos: carga `resources.py` (recursos compilados por el rcc de PyQt5), recorre `:/` y escribe cada archivo en
  `src/gui/resources/` con la misma ruta; genera `src/gui/resources/flatcam.qrc` para que el C++ los use como `:/images/...`.
- Traducción: la arma `build_translations.py` (`src/gui/resources/i18n/`); acá solo se agrega al `.qrc`.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tools/gui_dump/extract_resources.py
"""
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fc_runtime as fr  # noqa: E402

OUT = os.path.join(fr.ROOT, "src", "gui", "resources")
# Las tablas de traducción (las arma build_translations.py).
I18N = ["languages", "de", "en", "es", "fr", "it", "pt_BR", "ro", "ru", "tr"]


def main():
    fr.install()
    from PyQt5 import QtCore
    fr.import_original("resources")  # registra los recursos en ':/'
    files = []
    it = QtCore.QDirIterator(":/", QtCore.QDirIterator.Subdirectories)
    while it.hasNext():
        path = it.next()
        info = QtCore.QFileInfo(path)
        if info.isDir() or not path.startswith(":/images/"):  # lo demás es de Qt, no de FlatCAM
            continue
        rel = path[2:]
        f = QtCore.QFile(path)
        f.open(QtCore.QIODevice.ReadOnly)
        data = bytes(f.readAll())
        dst = os.path.join(OUT, rel)
        os.makedirs(os.path.dirname(dst), exist_ok=True)
        with open(dst, "wb") as out:
            out.write(data)
        files.append(rel)
    files.sort()
    with open(os.path.join(OUT, "flatcam.qrc"), "w", encoding="utf-8") as f:
        f.write('<!DOCTYPE RCC>\n<!-- ARCHIVO GENERADO por tools/gui_dump/extract_resources.py - no editar a mano. -->\n')
        f.write('<RCC version="1.0">\n<qresource prefix="/">\n')
        for rel in files + ["i18n/%s.json" % l for l in I18N] + ["factory_defaults.json", "preferences_fields.json", "shortcuts.json",
                               "tcl_keywords.json", "tcl_commands.json"]:
            f.write("    <file>%s</file>\n" % rel)
        f.write("</qresource>\n</RCC>\n")
    print("%d archivos en %s" % (len(files), OUT))


if __name__ == "__main__":
    main()
