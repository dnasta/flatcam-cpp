#!/usr/bin/env python3
"""Escribe las opciones de fábrica de FlatCAM 2024.4 (`FlatCAMDefaults.factory_defaults`) en
`src/gui/resources/factory_defaults.json`, para que el port arme las opciones de cada objeto como `AppObject.new_object`
y las opciones del proyecto al guardarlo.

Uso:  MAMBA_ROOT_PREFIX=$HOME/.local/share/mamba micromamba run -n old python tools/gui_dump/extract_defaults.py
"""
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fc_runtime as fr  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(ROOT, "src", "gui", "resources", "factory_defaults.json")


def main():
    fr.install()
    mod = fr.import_original("defaults")
    d = dict(mod.FlatCAMDefaults.factory_defaults)
    bad = {k: type(v).__name__ for k, v in d.items() if not isinstance(v, (str, int, float, bool, list, tuple, dict, type(None)))}
    if bad:
        raise SystemExit("tipos no serializables: %s" % bad)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(d, f, indent=2, sort_keys=True)
    print("Escrito", OUT, len(d), "opciones")


if __name__ == "__main__":
    main()
